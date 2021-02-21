/*
    Project:    ShutTheFrontDoor
    File:       stfd_camera.c
    Author:     Benjamin Lanthier
    Date:       18/02/2021

    Desc:       Source file for programming the camera and taking pictures
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>

#include "driver/sdmmc_host.h"
#include "driver/sdmmc_defs.h"
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h"

//#include "esp_timer.h"
#include "img_converters.h"

#include "soc/soc.h" //disable brownout problems
#include "soc/rtc_cntl_reg.h"  //disable brownout problems

#include "stfd_peripherals.h"

static const char *TAG = "stfd_camera";

#define MOUNT_POINT "/sdcard"
#define DEFAULT_CAM_STREAM_PORT 80

#define BOARD_ESP32CAM_AITHINKER 1

// ESP32Cam (AiThinker) PIN Map
#ifdef BOARD_ESP32CAM_AITHINKER

#define CAM_PIN_PWDN 32
#define CAM_PIN_RESET -1 //software reset will be performed
#define CAM_PIN_XCLK 0
#define CAM_PIN_SIOD 26
#define CAM_PIN_SIOC 27

#define CAM_PIN_D7 35
#define CAM_PIN_D6 34
#define CAM_PIN_D5 39
#define CAM_PIN_D4 36
#define CAM_PIN_D3 21
#define CAM_PIN_D2 19
#define CAM_PIN_D1 18
#define CAM_PIN_D0 5
#define CAM_PIN_VSYNC 25
#define CAM_PIN_HREF 23
#define CAM_PIN_PCLK 22

#endif /* CAM PIN ASSIGNMENT */

#if CONFIG_IMAGE_FORMAT_QVGA
#define IMAGE_FORMAT_SIZE FRAMESIZE_QVGA
#elif CONFIG_IMAGE_FORMAT_CIF
#define IMAGE_FORMAT_SIZE FRAMESIZE_CIF
#elif CONFIG_IMAGE_FORMAT_VGA
#define IMAGE_FORMAT_SIZE FRAMESIZE_VGA
#elif CONFIG_IMAGE_FORMAT_SVGA
#define IMAGE_FORMAT_SIZE FRAMESIZE_SVGA
#elif CONFIG_IMAGE_FORMAT_XGA
#define IMAGE_FORMAT_SIZE FRAMESIZE_XGA
#elif CONFIG_IMAGE_FORMAT_SXGA
#define IMAGE_FORMAT_SIZE FRAMESIZE_SXGA
#elif CONFIG_IMAGE_FORMAT_UXGA
#define IMAGE_FORMAT_SIZE FRAMESIZE_UXGA
#else
#define IMAGE_FORMAT_SIZE FRAMESIZE_QVGA
#endif /* CONFIG_IMAGE_FORMAT_SIZE */

#if CONFIG_IMAGE_PIXEL_JPEG
#define IMAGE_PIXEL_FORMAT PIXFORMAT_JPEG
#elif CONFIG_IMAGE_PIXEL_YUV422
#define IMAGE_PIXEL_FORMAT PIXFORMAT_YUV422
#elif CONFIG_IMAGE_PIXEL_GRAYSCALE
#define IMAGE_PIXEL_FORMAT PIXFORMAT_GRAYSCALE
#elif CONFIG_IMAGE_PIXEL_RGB565
#define IMAGE_PIXEL_FORMAT PIXFORMAT_RGB565
#else
#define IMAGE_PIXEL_FORMAT PIXFORMAT_JPEG
#endif /* CONFIG_IMAGE_PIXEL_FORMAT */

#define PART_BOUNDARY "123456789000000000000987654321"
static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

httpd_handle_t stream_httpd = NULL;

static camera_config_t camera_config = {
    .pin_pwdn     = CAM_PIN_PWDN,
    .pin_reset    = CAM_PIN_RESET,
    .pin_xclk     = CAM_PIN_XCLK,
    .pin_sscb_sda = CAM_PIN_SIOD,
    .pin_sscb_scl = CAM_PIN_SIOC,

    .pin_d7     = CAM_PIN_D7,
    .pin_d6     = CAM_PIN_D6,
    .pin_d5     = CAM_PIN_D5,
    .pin_d4     = CAM_PIN_D4,
    .pin_d3     = CAM_PIN_D3,
    .pin_d2     = CAM_PIN_D2,
    .pin_d1     = CAM_PIN_D1,
    .pin_d0     = CAM_PIN_D0,
    .pin_vsync  = CAM_PIN_VSYNC,
    .pin_href   = CAM_PIN_HREF,
    .pin_pclk   = CAM_PIN_PCLK,

    //XCLK 20MHz or 10MHz for OV2640 double FPS (Experimental)
    .xclk_freq_hz = 20000000,
    .ledc_timer   = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = IMAGE_PIXEL_FORMAT, //YUV422,GRAYSCALE,RGB565,JPEG
    .frame_size   = IMAGE_FORMAT_SIZE,  //QQVGA-UXGA Do not use sizes above QVGA when not JPEG

    .jpeg_quality = 10, //0-63 lower number means higher quality
    .fb_count     = 2   //if more than one, i2s runs in continuous mode. Use only with JPEG
};

esp_err_t init_camera()
{
    esp_err_t err;
    //initialize the camera
    if (!(cam_content.cam_initiated)) {
        err = esp_camera_init(&camera_config);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Camera Init Failed");
            return err;
        }
        cam_content.cam_initiated = true;
    }
    return ESP_OK;
}

esp_err_t init_sdcard()
{
    esp_err_t ret = ESP_FAIL;

    if (!(cam_content.sdcard_initiated)) {
        esp_vfs_fat_sdmmc_mount_config_t mount_config = {
            .format_if_mount_failed = false,
            .max_files = 5,
            .allocation_unit_size = 16 * 1024
        };

        sdmmc_card_t *card;

        const char mount_point[] = MOUNT_POINT;
        ESP_LOGI(TAG, "Initializing SD card");

        sdmmc_host_t host = SDMMC_HOST_DEFAULT();
        sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();

        ESP_LOGI(TAG, "Mounting SD card...");
        gpio_set_pull_mode(15, GPIO_PULLUP_ONLY);   // CMD, needed in 4- and 1- line modes
        gpio_set_pull_mode(2, GPIO_PULLUP_ONLY);    // D0, needed in 4- and 1-line modes
        gpio_set_pull_mode(4, GPIO_PULLUP_ONLY);    // D1, needed in 4-line mode only
        gpio_set_pull_mode(12, GPIO_PULLUP_ONLY);   // D2, needed in 4-line mode only
        gpio_set_pull_mode(13, GPIO_PULLUP_ONLY);   // D3, needed in 4- and 1-line modes

        ret = esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config, &mount_config, &card);

        if (ret == ESP_OK)
        {
        ESP_LOGI(TAG, "SD card mount successfully!");
        }
        else
        {
        ESP_LOGE(TAG, "Failed to mount SD card VFAT filesystem. Error: %s", esp_err_to_name(ret));
        }
        // Card has been initialized, print its properties
        sdmmc_card_print_info(stdout, card);
    }
    return ret;
}

void cam_exec_recording_task(cam_content_t* cam_content) {
    camera_fb_t* camera_pic;
    
    if (cam_content->content_type == PICTURE) {
        camera_pic = camera_take_picture(cam_content->save_to_sdcard);
        if (cam_content->upload_content)
        {
            ESP_LOGI(TAG, "Tried to Upload to HTTP");
            //TODO: Upload picture
        }
    }
    else if (cam_content->content_type == STREAM) {
        if (!(cam_content->cam_server_init)) {
          startCameraServer();
        }
        else {
          stopCameraServer();
        }
    }
}

//================================================
//===== Section - Taking Pictures and upload =====
//================================================
bool save_image_to_sdcard(camera_fb_t *pic)
{
    bool err = false;
    char *pic_name = malloc(30 + sizeof(int64_t));
    sprintf(pic_name, MOUNT_POINT"/pic_%lli.jpg", cam_content.pic_counter);
    FILE *file = fopen(pic_name, "w");
    
    if (file != NULL)
    {
        fwrite(pic->buf, 1, pic->len, file);
        ESP_LOGI(TAG, "File saved: %s", pic_name);
        err = false;
    }
    else
    {
        ESP_LOGE(TAG, "Could not open file =(");
        fclose(file);
        free(pic_name);
        err = true;
    }

    fclose(file);
    free(pic_name);

    return !err;
}

camera_fb_t* camera_take_picture(bool save_to_sdcard)
{
    ESP_LOGI(TAG, "Taking picture...");
    camera_fb_t *pic = esp_camera_fb_get();
    cam_content.pic_counter++;

    if (save_to_sdcard)
        save_image_to_sdcard(pic);

    vTaskDelay(3000 / portTICK_RATE_MS);

    return pic;
}

//=======================================
//===== Section for video streaming =====
//=======================================

esp_err_t stream_handler(httpd_req_t *req) {
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  char * part_buf[64];

  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if(res != ESP_OK){
    return res;
  }

  while(true){
    fb = esp_camera_fb_get();
    if (!fb) {
      ESP_LOGE(TAG, "Camera capture failed");
      res = ESP_FAIL;
    } else {
      if(fb->width > 400){
        if(fb->format != PIXFORMAT_JPEG){
          bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
          esp_camera_fb_return(fb);
          fb = NULL;
          if(!jpeg_converted){
            ESP_LOGE(TAG, "JPEG compression failed");
            res = ESP_FAIL;
          }
        } else {
          _jpg_buf_len = fb->len;
          _jpg_buf = fb->buf;
        }
      }
    }
    if(res == ESP_OK){
      size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    if(res == ESP_OK){
      res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    }
    if(res == ESP_OK){
      res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
    }
    if(fb){
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    } else if(_jpg_buf){
      free(_jpg_buf);
      _jpg_buf = NULL;
    }
    if(res != ESP_OK){
      break;
    }
  }
  return res;
}

void startCameraServer() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = DEFAULT_CAM_STREAM_PORT;

    httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = stream_handler,
    .user_ctx  = NULL
    };

    ESP_LOGI(TAG,"Starting web server on port: '%d'\n", config.server_port);
    ESP_LOGI(TAG, "Connect to http://%s", cam_content.device_ip);
    if (httpd_start(&stream_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(stream_httpd, &index_uri);
    }
}

void stopCameraServer() {
  if (httpd_stop(&stream_httpd) == ESP_OK)
      ESP_LOGI(TAG,"Stopping web server");
  else
      ESP_LOGE(TAG, "Could not successfully stop the camera server on port %d", DEFAULT_CAM_STREAM_PORT);
}

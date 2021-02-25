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

#include "stfd_peripherals.h"

static const char *TAG = "stfd_camera";

#define MOUNT_POINT "/sdcard"

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

#define DEFAULT_STREAM_JPEG_QUALITY 12
#define DEFAULT_PIC_JPEG_QUALITY 10

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

esp_err_t init_camera(mcu_content_t* mcu_c, mcu_content_type_t type) {
    esp_err_t err = ESP_OK;
    if (!mcu_c->cam_initiated) {
        if (type == PICTURE) {
            camera_config.jpeg_quality = DEFAULT_PIC_JPEG_QUALITY;
            camera_config.fb_count = 1;
            ESP_LOGI(TAG, "Initializing Camera to Picture");
        } else if (type == STREAM) {
            camera_config.jpeg_quality = DEFAULT_STREAM_JPEG_QUALITY;
            camera_config.fb_count = 1;
            ESP_LOGI(TAG, "Initializing Camera to Stream");
        }
        else {
            ESP_LOGE(TAG, "Passed invalid type %i", (int) type);
            return ESP_FAIL;
        }
        //initialize the camera
        err = esp_camera_init(&camera_config);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Camera Init Failed");
            return err;
        }
        mcu_c->cam_initiated = true;
        mcu_c->content_type = type;
    }
    else if (type == PICTURE) {
        if (mcu_c->content_type == STREAM) {
            err = esp_camera_deinit();
            ESP_LOGI(TAG, "Changed camera content type from Stream to Picture");
            if (err != ESP_OK) {
                ESP_LOGE(TAG, "Camera De-Init Failed");
                return err;
            }

            //initialize the camera
            camera_config.jpeg_quality = DEFAULT_PIC_JPEG_QUALITY;
            camera_config.fb_count = 1;
            err = esp_camera_init(&camera_config);
            if (err != ESP_OK) {
                ESP_LOGE(TAG, "Camera Init Failed");
                return err;
            }
            mcu_c->cam_initiated = true;
            mcu_c->content_type = PICTURE;
        }
        else
            ESP_LOGI(TAG, "Camera already init");
    }
    else if (type == STREAM) {
        if (mcu_c->content_type == PICTURE) {
            err = esp_camera_deinit();
            ESP_LOGI(TAG, "Changed camera content type from Picture to Stream");
            if (err != ESP_OK)
            {
                ESP_LOGE(TAG, "Camera De-Init Failed");
                return err;
            }

            //initialize the camera
            camera_config.jpeg_quality = DEFAULT_STREAM_JPEG_QUALITY;
            camera_config.fb_count = 2;
            err = esp_camera_init(&camera_config);
            if (err != ESP_OK)
            {
                ESP_LOGE(TAG, "Camera Init Failed");
                return err;
            }
            mcu_c->cam_initiated = true;
            mcu_c->content_type = STREAM;
        }
        else
            ESP_LOGI(TAG, "Camera already init");
    }
    else {
        ESP_LOGE(TAG, "Passed invalid type %i", (int) type);
        return ESP_FAIL;
    }
    return err;
}

esp_err_t init_sdcard(mcu_content_t* mcu_c)
{
    esp_err_t ret = ESP_FAIL;

    if (!(mcu_c->sdcard_initiated)) {
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
        mcu_c->sdcard_initiated = true;
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



//================================================
//===== Section - Taking Pictures and upload =====
//================================================
bool save_image_to_sdcard(uint8_t* buf, size_t len, long long int pic_cnt)
{
    bool err = false;
    char *pic_name = malloc(30 + sizeof(int64_t));
    sprintf(pic_name, MOUNT_POINT"/pic_%lli.jpg", pic_cnt);
    FILE *file = fopen(pic_name, "w");
    
    if (file != NULL)
    {
        fwrite(buf, 1, len, file);
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

camera_fb_t* camera_take_picture(mcu_content_t* mcu_c)
{
    ESP_LOGI(TAG, "Taking picture...");
    camera_fb_t *pic = esp_camera_fb_get();
    if (!pic) {
      ESP_LOGW(TAG, "Camera Capture Failed!");
      esp_camera_fb_return(pic);
      pic = NULL;
    }
    else {
      mcu_c->pic_counter++;
    }

    vTaskDelay(3000 / portTICK_RATE_MS);

    return pic;
}

esp_err_t convert_to_jpeg(camera_fb_t* fb, uint8_t** jpeg_buf, size_t* jpeg_buf_len) {
    esp_err_t res = ESP_OK;

    if(fb->format != PIXFORMAT_JPEG){
        bool jpeg_converted = frame2jpg(fb, 80, jpeg_buf, jpeg_buf_len);

        if(!jpeg_converted){
            ESP_LOGE(TAG, "JPEG compression failed");
            res = ESP_FAIL;
        } 
    } 
    else {
        *jpeg_buf_len = fb->len;
        *jpeg_buf     = fb->buf;
    }
    return res;
}

/*
    Project:    ShutTheFrontDoor
    File:       main.c
    Author:     Benjamin Lanthier
    Date:       18/02/2021

    Desc:       This file contains the description of the main application of the device.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/param.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "stfd_peripherals.h"

#if (CONFIG_IMAGE_TO_SDCARD || CONFIG_IMAGE_TO_BOTH_PROT)
#define IMAGE_TO_SDCARD 1
#elif CONFIG_IMAGE_TO_HTTP_UPLOAD
#define IMAGE_TO_SDCARD 0
#else 
#define IMAGE_TO_SDCARD 0
#endif /* CONFIG IMAGE_TO_SD_CARD */

#if CONFIG_IMAGE_TO_HTTP_UPLOAD
#define IMAGE_TO_HTTP_UPLOAD 1
#else
#define IMAGE_TO_HTTP_UPLOAD 0
#endif /* CONFIG IMAGE_TO_HTTP_UPLOAD */

#if IMAGE_TO_SDCARD
#define INIT_SDCARD 1
#else
#define INIT_SDCARD 0
#endif /* CONFIG INIT_SDCARD */

static const char* TAG = "main";
static const char* DRBELL_MSG = "Doorbell pressed - Someone's at the Door!";
static const char* REEDSW_MSG = "The Door opened";

static httpd_handle_t stream_httpd = NULL;
static xQueueHandle gpio_evt_queue = NULL;

mcu_content_t _mcu_c = {
.cam_initiated      = false,
.sdcard_initiated   = false,
.cam_server_init    = false,
.save_to_sdcard     = false,
.upload_content     = false,
.trig_signal        = SIGNAL_LOW,
.content_type       = STANDBY,
.ap_info            = NULL,
.device_ip          = "",
.pic_counter        = 0
};
static mcu_content_t* mcu_c = &_mcu_c;

// Forward Declaration
void IRAM_ATTR gpio_isr_handler(void* arg);
static void gpio_trig_action(void* arg);
void exec_gpio_task(mcu_content_t* mcu_c);

/**
 * @brief Handler for GPIO interrupts
 * 
 * @param arg Allocated memory data passed to the handler.
 *            In this case, the data is the triggered GPIO 
 */
void IRAM_ATTR gpio_isr_handler(void* arg) {
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

/**
 * @brief Takes a picture when the input gpio is pulled down by a switch
 */
static void gpio_trig_action(void* arg)
{
    uint32_t io_num;
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
            get_io_type(io_num, mcu_c);
            mcu_c->save_to_sdcard = IMAGE_TO_SDCARD;
            mcu_c->upload_content = IMAGE_TO_HTTP_UPLOAD;

            if (trig_valid_gpio(io_num, mcu_c->trig_signal)) {
                gpio_blink(1);
                exec_gpio_task(mcu_c);
            }
            else {
                ESP_LOGI(TAG, "Triggered the wrong input pin or wrong signal level");
            }   
        }
    }
}

/**
 * @brief Selects the task to execute (picture or stream) according to the passed camera content
 * 
 * @param mcu_c Camera current status with its content
 */
void exec_gpio_task(mcu_content_t* mcu_c) {    
    camera_fb_t* camera_pic;
    uint8_t* jpeg_buf = NULL;
    size_t jpeg_buf_len = 0;

    switch (mcu_c->content_type) {
        case (mcu_content_type_t) PICTURE:
            init_camera(mcu_c, PICTURE);

            camera_pic = camera_take_picture(mcu_c);
            convert_to_jpeg(camera_pic, &jpeg_buf, &jpeg_buf_len);

            if (mcu_c->save_to_sdcard && mcu_c->sdcard_initiated)
                save_image_to_sdcard(jpeg_buf, jpeg_buf_len, mcu_c->pic_counter);

            if (mcu_c->upload_content) {   
                ESP_LOGI(TAG, "Uploading picture");
                //ESP_LOGI(TAG, "buffer data\n %s and its length: %i", (const char*) jpeg_buf, jpeg_buf_len);
                ESP_LOGI(TAG, "buffer length: %i", jpeg_buf_len);
                http_rest_with_url_upload_picture(jpeg_buf, jpeg_buf_len);
            }
            break;

        case (mcu_content_type_t) STREAM:

            if (!(mcu_c->cam_server_init)) {
                init_camera(mcu_c, STREAM);
                stream_httpd = startStreamServer(mcu_c->device_ip);
                mcu_c->cam_server_init = true;
            }

            // else {
            //     stopStreamServer(&stream_httpd);
            //     mcu_c->cam_server_init = false;

            //     if (esp_camera_deinit() != ESP_OK)
            //         ESP_LOGE(TAG, "Camera De-Init Failed");
            //     else
            //         mcu_c->cam_initiated = false;
            // }
            break;

        case (mcu_content_type_t) DRBELL:
            http_rest_with_url_notification(DRBELL_MSG);
            break;
        case (mcu_content_type_t) REEDSW:
            http_rest_with_url_notification(REEDSW_MSG);
            break;
        case (mcu_content_type_t) STANDBY:
            ESP_LOGI(TAG, "Standing by... 10sec");
            vTaskDelay(10000/portTICK_RATE_MS);
            break;
        case (mcu_content_type_t) INVALID:
        default:
            ESP_LOGE(TAG, "Invalid type detected when executing queue task");
    }
}

// static void http_client_task(void *pvParameters)
// {
//     /**
//      * NOTE: We only need rest_with_url();
//      */

//     http_rest_with_url(/*insert JPEG*/);

//     ESP_LOGI(TAG, "Finish http client task");
// }

void app_main(void) {
    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task
    xTaskCreate(&gpio_trig_action, "gpio_trig_action", 8192, NULL, 10, NULL);

    gpio_init_setup(gpio_isr_handler);
    //if (INIT_SDCARD)
        init_sdcard(mcu_c);

    wifi_scan(mcu_c);

    //Continuously sample ADC
    // while (1) {
    //     uint32_t adc_reading = 0;
    //     //Multisampling
    //     for (int i = 0; i < NO_OF_SAMPLES; i++) {
    //         if (unit == ADC_UNIT_1) {
    //             adc_reading += adc1_get_raw((adc1_channel_t)channel);
    //         } else {
    //             int raw;
    //             adc2_get_raw((adc2_channel_t)channel, width, &raw);
    //             adc_reading += raw;
    //         }
    //     }
    //     adc_reading /= NO_OF_SAMPLES;
    //     //Convert adc_reading to voltage in mV
    //     uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
    //     printf("Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);
    //     vTaskDelay(pdMS_TO_TICKS(1000));
    // }
}
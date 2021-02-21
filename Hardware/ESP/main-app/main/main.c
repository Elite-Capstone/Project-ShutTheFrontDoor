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

static xQueueHandle gpio_evt_queue = NULL;

void IRAM_ATTR gpio_isr_handler(void* arg) {
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

// Takes a picture when the input gpio is pulled down by a switch
void gpio_take_picture(void* arg)
{
    uint32_t io_num;
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));

            if (trig_valid_gpio(io_num, SIGNAL_LOW)) {
                gpio_blink_output(1);
                cam_content.save_to_sdcard = IMAGE_TO_SDCARD;
                cam_content.upload_content = IMAGE_TO_HTTP_UPLOAD;
                cam_content.content_type   = gpio_io_type(io_num);
                cam_exec_recording_task(&cam_content);
            }
            else {
                ESP_LOGI(TAG, "Triggered the wrong input pin");
            }   
        }
    }
}

void app_main(void) {
    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task
    xTaskCreate(gpio_take_picture, "gpio_take_picture", 2048, NULL, 10, NULL);

    gpio_setup_for_picture(gpio_isr_handler);
    init_camera();
    if (INIT_SDCARD)
        init_sdcard();

    cam_content.ap_info = wifi_scan();
}
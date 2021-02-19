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
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#include "stfd_peripherals.h"

#if (CONFIG_IMAGE_TO_SDCARD || CONFIG_IMAGE_TO_BOTH_PROT)
#define IMAGE_TO_SDCARD 1
#elif CONFIG_IMAGE_TO_HTTP_UPLOAD
#define IMAGE_TO_SDCARD 0
#else 
#define IMAGE_TO_SDCARD 0
#endif /* CONFIG IMAGE_TO_SD_CARD */

#if IMAGE_TO_SDCARD
#define INIT_SDCARD 1
#else
#define INIT_SDCARD 0
#endif /* CONFIG INIT_SDCARD */

static const char* TAG = "main";

// Takes a picture when the input gpio is pulled down by a switch
static void take_picture(void* arg)
{
    uint32_t io_num;
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
            camera_take_picture(IMAGE_TO_SDCARD);
        }
    }
}

void app_main(void)
{
    wifi_ap_record_t* ap_info;

    gpio_setup_for_picture();
    init_camera();
    if (INIT_SDCARD)
        init_sdcard();
        
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    if (getDefaultScanMethod() == WIFI_FAST_SCAN) {
        fast_scan();
    }
    else { //WIFI_ALL_CHANNEL_SCAN:
        ap_info = wifi_scan();
        // TODO: Send list to BlueTooth connected user
        // Set the default wifi to the returned selection from the user
    }
}
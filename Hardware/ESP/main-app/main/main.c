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
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "scan.c"

#if CONFIG_GPIO_INPUT_0_PIC
#define GPIO_INPUT_PIC 0
#elif CONFIG_GPIO_INPUT_2_PIC
#define GPIO_INPUT_PIC 4
#elif CONFIG_GPIO_INPUT_12_PIC
#define GPIO_INPUT_PIC 12
#elif CONFIG_GPIO_INPUT_13_PIC
#define GPIO_INPUT_PIC 13
#elif CONFIG_GPIO_INPUT_14_PIC
#define GPIO_INPUT_PIC 14
#elif CONFIG_GPIO_INPUT_15_PIC
#define GPIO_INPUT_PIC 15
#elif CONFIG_GPIO_INPUT_16_PIC
#define GPIO_INPUT_PIC 16
#else
#define GPIO_INPUT_PIC 16
#endif /* CONFIG_GPIO_INPUT_PIC */

#define GPIO_INPUT_PIN_SEL  (1ULL<<GPIO_INPUT_PIC)
#define ESP_INTR_FLAG_DEFAULT 0

static const char* TAG = "main";
static xQueueHandle gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static ReturnType gpio_take_picture(void* arg)
{
    uint32_t io_num;
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
            // TODO: Take picture here
        }
    }
}

static bool gpio_setup_pic(void)
{
    gpio_config_t io_conf;
    ///interrupt of rising edge
    io_conf.intr_type = GPIO_PIN_INTR_POSEDGE;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    //set as input mode    
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task
    xTaskCreate(gpio_take_picture, "gpio_take_picture", 2048, NULL, 10, NULL);

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_PIC);
}

void app_main(void)
{
    gpio_setup_pic();
    wifi_ap_record_t ap_info[DEFAULT_SCAN_LIST_SIZE];

    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    if (DEFAULT_SCAN_METHOD == WIFI_FAST_SCAN) {
        fast_scan();
    }
    else { //WIFI_ALL_CHANNEL_SCAN:
        ap_info = wifi_scan();
        // TODO: Send list to BlueTooth connected user
        // Set the default wifi to the returned selection from the user
    }
}
/*
    Project:    ShutTheFrontDoor
    File:       gpio.c
    Author:     Benjamin Lanthier
    Date:       18/02/2021

    Desc;       This file contains the gpio setup and interrupts
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#include "stfd_peripherals.h"

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

#if CONFIG_GPIO_INPUT_4_PIC
#define GPIO_OUTPUT_CONFIRM_UPLOAD 2
#else
#define GPIO_OUTPUT_CONFIRM_UPLOAD 4
#endif /* CONFIG GPIO_OUTPUT_CONFIRM_UPLOAD */

#define GPIO_INPUT_PIN_SEL  (1ULL<<GPIO_INPUT_PIC)
#define GPIO_OUTPUT_PIN_SEL (1ULL<<GPIO_OUTPUT_CONFIRM_UPLOAD)

#define ESP_INTR_FLAG_DEFAULT 0

static const char* TAG = "stfd_gpio"

bool trig_valid_gpio_base(uint32_t io_num, uint8_t sg_level) {
    return (io_num == GPIO_INPUT_PIC && gpio_get_level(io_num) == sg_level);
}

void IRAM_ATTR gpio_isr_handler(void* arg) {
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

void gpio_blink_output(uint32_t num_blinks) {
    ESP_LOGI(TAG, "blinking %u times...", num_blinks);

    for (b = 0; b < num_blinks; b++) {
        gpio_set_level(GPIO_OUTPUT_CONFIRM_UPLOAD, 1);
        vTaskDelay(2000 / portTICK_RATE_MS);
        gpio_set_level(GPIO_OUTPUT_CONFIRM_UPLOAD, 0);
        vTaskDelay(2000 / portTICK_RATE_MS);
    }
}

bool gpio_setup_input(void) {
    uint32_t err = 0;

    gpio_config_t io_conf;
    ///interrupt of rising edge
    io_conf.intr_type = GPIO_PIN_INTR_POSEDGE;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    //set as input mode    
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    err |= gpio_config(&io_conf);

    //install gpio isr service
    err |= gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    err |= gpio_isr_handler_add(GPIO_INPUT_PIC, gpio_isr_handler, (void*) GPIO_INPUT_PIC);

    return err == 0;
}

bool gpio_setup_output(void) {
    uint32_t err;

    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    err |= gpio_config(&io_conf);

    //Initialize to 0
    err |= gpio_set_level(GPIO_OUTPUT_CONFIRM_UPLOAD, 0);

    return err == 0;
}

bool gpio_setup_for_picture(void) {
    ESP_LOGI(TAG, "GPIO setup...");
    return gpio_setup_input() && gpio_setup_output();
}
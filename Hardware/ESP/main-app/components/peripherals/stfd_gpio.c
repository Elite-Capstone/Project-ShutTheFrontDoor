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

#if CONFIG_GPIO_INPUT_0_STREAM
#define GPIO_INPUT_STREAM 0
#elif CONFIG_GPIO_INPUT_2_STREAM
#define GPIO_INPUT_STREAM 4
#elif CONFIG_GPIO_INPUT_12_STREAM
#define GPIO_INPUT_STREAM 12
#elif CONFIG_GPIO_INPUT_13_STREAM
#define GPIO_INPUT_STREAM 13
#elif CONFIG_GPIO_INPUT_14_STREAM
#define GPIO_INPUT_STREAM 14
#elif CONFIG_GPIO_INPUT_15_STREAM
#define GPIO_INPUT_STREAM 15
#elif CONFIG_GPIO_INPUT_16_STREAM
#define GPIO_INPUT_STREAM 16
#else
#define GPIO_INPUT_STREAM 12
#endif /* CONFIG_GPIO_INPUT_STREAM */

#if CONFIG_GPIO_OUTPUT_0_CONFIRM
#define GPIO_OUTPUT_CONFIRM_UPLOAD 0
#elif CONFIG_GPIO_OUTPUT_2_CONFIRM
#define GPIO_OUTPUT_CONFIRM_UPLOAD 4
#elif CONFIG_GPIO_OUTPUT_12_CONFIRM
#define GPIO_OUTPUT_CONFIRM_UPLOAD 12
#elif CONFIG_GPIO_OUTPUT_13_CONFIRM
#define GPIO_OUTPUT_CONFIRM_UPLOAD 13
#elif CONFIG_GPIO_OUTPUT_14_CONFIRM
#define GPIO_OUTPUT_CONFIRM_UPLOAD 14
#elif CONFIG_GPIO_OUTPUT_15_CONFIRM
#define GPIO_OUTPUT_CONFIRM_UPLOAD 15
#elif CONFIG_GPIO_OUTPUT_16_CONFIRM
#define GPIO_OUTPUT_CONFIRM_UPLOAD 16
#else
#define GPIO_OUTPUT_CONFIRM_UPLOAD 4
#endif /* CONFIG_GPIO_OUTPUT_CONFIRM_UPLOAD */

#if (GPIO_INPUT_PIC == GPIO_INPUT_STREAM)
#error "Both input GPIO functions are associated to the same GPIO"
#endif

#if (GPIO_INPUT_STREAM == GPIO_OUTPUT_CONFIRM_UPLOAD)
#error "The output GPIO is also associated to the input GPIO for Streaming"
#endif

#if (GPIO_INPUT_PIC == GPIO_OUTPUT_CONFIRM_UPLOAD)
#error "The output GPIO is also associated to the input GPIO for Picture"
#endif

#define GPIO_INPUT_PIC_PIN_SEL      (1ULL<<GPIO_INPUT_PIC)
#define GPIO_INPUT_STREAM_PIN_SEL   (1ULL<<GPIO_INPUT_STREAM)
#define GPIO_OUTPUT_PIN_SEL         (1ULL<<GPIO_OUTPUT_CONFIRM_UPLOAD)

#define ESP_INTR_FLAG_DEFAULT 0

static const char* TAG = "stfd_gpio";

bool trig_valid_gpio(uint32_t io_num, uint8_t sg_level) {
    return ((io_num == GPIO_INPUT_PIC || io_num == GPIO_INPUT_STREAM) && gpio_get_level(io_num) == sg_level);
}

void gpio_blink_output(uint32_t num_blinks) {
    ESP_LOGI(TAG, "blinking %u times...", num_blinks);

    for (uint32_t b = 0; b < num_blinks; b++) {
        gpio_set_level(GPIO_OUTPUT_CONFIRM_UPLOAD, 1);
        vTaskDelay(1000 / portTICK_RATE_MS);
        gpio_set_level(GPIO_OUTPUT_CONFIRM_UPLOAD, 0);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

cam_content_type_t gpio_io_type(uint32_t io_num) {
    switch(io_num) {
        case GPIO_INPUT_PIC:
            return PICTURE;
        case GPIO_INPUT_STREAM:
            return STREAM;
        default:
            return INVALID;
    }
}

void gpio_setup_input(gpio_isr_t isr_handler) {
    gpio_config_t io_conf;

    ///interrupt of rising edge
    io_conf.intr_type = GPIO_PIN_INTR_POSEDGE;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = GPIO_INPUT_PIC_PIN_SEL;
    //set as input mode    
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    ///interrupt of rising edge
    io_conf.intr_type = GPIO_PIN_INTR_POSEDGE;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = GPIO_INPUT_STREAM_PIN_SEL;
    //set as input mode    
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    //install gpio isr service
    //Must only be done once. But each gpio can have their own isr handler
    ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT));
    //hook isr handler for specific gpio pin
    ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_INPUT_PIC, isr_handler, (void*) GPIO_INPUT_PIC));
    //hook isr handler for specific gpio pin
    ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_INPUT_STREAM, isr_handler, (void*) GPIO_INPUT_STREAM));
}

void gpio_setup_output(void) {
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
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    //Initialize to 0
    ESP_ERROR_CHECK(gpio_set_level(GPIO_OUTPUT_CONFIRM_UPLOAD, 0));
}

void gpio_setup_for_picture(gpio_isr_t isr_handler) {
    ESP_LOGI(TAG, "GPIO setup...");
    gpio_setup_input(isr_handler);
    gpio_setup_output();
}
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

#if  CONFIG_GPIO_INPUT_0_DRBELL_NOTIF
#define GPIO_INPUT_DRBELL_NOTIF 0
#elif  CONFIG_GPIO_INPUT_4_DRBELL_NOTIF
#define GPIO_INPUT_DRBELL_NOTIF 4
#elif  CONFIG_GPIO_INPUT_12_DRBELL_NOTIF
#define GPIO_INPUT_DRBELL_NOTIF 12
#elif  CONFIG_GPIO_INPUT_13_DRBELL_NOTIF
#define GPIO_INPUT_DRBELL_NOTIF 13
#elif  CONFIG_GPIO_INPUT_14_DRBELL_NOTIF
#define GPIO_INPUT_DRBELL_NOTIF 14
#elif  CONFIG_GPIO_INPUT_15_DRBELL_NOTIF
#define GPIO_INPUT_DRBELL_NOTIF 15
#elif  CONFIG_GPIO_INPUT_16_DRBELL_NOTIF
#define GPIO_INPUT_DRBELL_NOTIF 16
#else
#define GPIO_INPUT_DRBELL_NOTIF 13
#endif /* CONFIG_GPIO_INPUT_DRBELL_NOTIF */

#if  GPIO_INPUT_0_REEDSW_STATUS
#define GPIO_INPUT_REEDSW_STATUS 0
#elif  CONFIG_GPIO_INPUT_4_REEDSW_STATUS
#define GPIO_INPUT_REEDSW_STATUS 4
#elif  CONFIG_GPIO_INPUT_12_REEDSW_STATUS
#define GPIO_INPUT_REEDSW_STATUS 12
#elif  CONFIG_GPIO_INPUT_13_REEDSW_STATUS
#define GPIO_INPUT_REEDSW_STATUS 13
#elif  CONFIG_GPIO_INPUT_14_REEDSW_STATUS
#define GPIO_INPUT_REEDSW_STATUS 14
#elif  CONFIG_GPIO_INPUT_15_REEDSW_STATUS
#define GPIO_INPUT_REEDSW_STATUS 15
#elif  CONFIG_GPIO_INPUT_16_REEDSW_STATUS
#define GPIO_INPUT_REEDSW_STATUS 16
#else
#define GPIO_INPUT_REEDSW_STATUS 15
#endif /* CONFIG_GPIO_INPUT_REEDSW_STATUS */

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

// Wrong GPIO assignment check
#define GPIO_MULTA_ERR_MSG "Multiple functions assigned to same GPIO: "

#if (GPIO_INPUT_PIC == GPIO_INPUT_STREAM)
#error GPIO_MULTA_ERR_MSG "Picture & Stream"
#endif

#if (GPIO_INPUT_PIC == GPIO_INPUT_DRBELL_NOTIF)
#error GPIO_MULTA_ERR_MSG "Picture & Doorbell Notif."
#endif

#if (GPIO_INPUT_PIC == GPIO_INPUT_REEDSW_STATUS)
#error GPIO_MULTA_ERR_MSG "Picture & Reed Switch Status"
#endif

#if (GPIO_INPUT_PIC == GPIO_OUTPUT_CONFIRM_UPLOAD)
#error GPIO_MULTA_ERR_MSG "Picture & Output confirm"
#endif

#if (GPIO_INPUT_STREAM == GPIO_INPUT_DRBELL_NOTIF)
#error GPIO_MULTA_ERR_MSG "Stream & Doorbell Notif."
#endif

#if (GPIO_INPUT_STREAM == GPIO_INPUT_REEDSW_STATUS)
#error GPIO_MULTA_ERR_MSG "Stream & Reed Switch Status"
#endif

#if (GPIO_INPUT_STREAM == GPIO_OUTPUT_CONFIRM_UPLOAD)
#error GPIO_MULTA_ERR_MSG "Stream & Output confirm"
#endif

#if (GPIO_INPUT_DRBELL_NOTIF == GPIO_INPUT_REEDSW_STATUS)
#error GPIO_MULTA_ERR_MSG "Doorbell Notif & Reed Switch Status"
#endif

#if (GPIO_INPUT_DRBELL_NOTIF == GPIO_OUTPUT_CONFIRM_UPLOAD)
#error GPIO_MULTA_ERR_MSG "Doorbell Notif & Output confirm"
#endif

#define GPIO_INPUT_PIC_PIN_SEL      (1ULL<<GPIO_INPUT_PIC)
#define GPIO_INPUT_STREAM_PIN_SEL   (1ULL<<GPIO_INPUT_STREAM)
#define GPIO_INPUT_DRBELL_PIN_SEL   (1ULL<<GPIO_INPUT_DRBELL_NOTIF)
#define GPIO_INPUT_REEDSW_PIN_SEL   (1ULL<<GPIO_INPUT_REEDSW_STATUS)
#define GPIO_OUTPUT_PIN_SEL         (1ULL<<GPIO_OUTPUT_CONFIRM_UPLOAD)

#define ESP_INTR_FLAG_DEFAULT 0

static const char* TAG = "stfd_gpio";

bool trig_valid_gpio(uint32_t io_num, uint8_t sg_level) {
    //return ((io_num == GPIO_INPUT_PIC || io_num == GPIO_INPUT_STREAM) && gpio_get_level(io_num) == sg_level);
    return gpio_get_level(io_num) == sg_level;
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

mcu_content_type_t gpio_io_type(uint32_t io_num) {
    switch(io_num) {
        case GPIO_INPUT_PIC:
            return PICTURE;
        case GPIO_INPUT_STREAM:
            return STREAM;
        case GPIO_INPUT_DRBELL_NOTIF:
            return DRBELL;
        case GPIO_INPUT_REEDSW_STATUS:
            return REEDSW;
        default:
            return INVALID;
    }
}

// esp_err_t stfd_gpio_config() {
//     ///interrupt of rising edge
//     io_conf.intr_type = GPIO_PIN_INTR_POSEDGE;
//     //bit mask of the pins, use GPIO4/5 here
//     io_conf.pin_bit_mask = GPIO_INPUT_PIC_PIN_SEL;
//     //set as input mode    
//     io_conf.mode = GPIO_MODE_INPUT;
//     //enable pull-up mode
//     io_conf.pull_up_en = 1;
//     return ESP_ERROR_CHECK(gpio_config(&io_conf));
// }

void gpio_setup_input(gpio_isr_t isr_handler) {
    gpio_config_t io_conf;
    //esp_err_t err = ESP_OK;

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

    ///interrupt of rising edge
    io_conf.intr_type = GPIO_PIN_INTR_POSEDGE;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = GPIO_INPUT_DRBELL_PIN_SEL;
    //set as input mode    
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    ///interrupt of rising edge
    io_conf.intr_type = GPIO_PIN_INTR_POSEDGE;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = GPIO_INPUT_REEDSW_PIN_SEL;
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
    ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_INPUT_DRBELL_NOTIF, isr_handler, (void*) GPIO_INPUT_DRBELL_NOTIF));
    ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_INPUT_REEDSW_STATUS, isr_handler, (void*) GPIO_INPUT_REEDSW_STATUS));
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
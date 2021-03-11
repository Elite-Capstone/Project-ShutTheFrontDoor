/*
    Project:    ShutTheFrontDoor
    File:       stfd_gpio.c
    Author:     Benjamin Lanthier
    Date:       18/02/2021

    Desc;       This file contains the gpio setup and interrupts
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "stfd_peripherals.h"
#include "stfd_gpio_config.h"

#define ESP_INTR_FLAG_DEFAULT 0

static const char* TAG = "stfd_gpio";

bool trig_valid_gpio(uint32_t io_num, uint8_t sg_level) {
    return gpio_get_level(io_num) == sg_level;
}

// bool trig_motor_gpio(uint32_t io_num, uint8_t sg_level) {
//     if (io_num == GPIO_INPUT_BOOT)
//         return gpio_get_level(io_num) == sg_level;
//     return false;
// }

void get_io_type(uint32_t io_num, mcu_content_t* mcu_content) {
    switch(io_num) {
        // case GPIO_INPUT_BOOT:
        //     return BOOT;
        case GPIO_INPUT_MS:
            mcu_content->content_type = MS;
            mcu_content->trig_signal  = SIGNAL_HIGH;
            break;
        case GPIO_INPUT_NSW:
            mcu_content->content_type = NSW;
            mcu_content->trig_signal  = SIGNAL_LOW;
            break;
        case GPIO_INPUT_BATTERY:
            mcu_content->content_type = BATTERY;
            mcu_content->trig_signal  = SIGNAL_LOW;
            break;
        case GPIO_INPUT_DRBELL_NOTIF:
            mcu_content->content_type = DRBELL;
            mcu_content->trig_signal  = SIGNAL_LOW;
            break;
        case GPIO_INPUT_REEDSW_STATUS:
            mcu_content->content_type = REEDSW;
            mcu_content->trig_signal  = SIGNAL_LOW;
            break;
        case GPIO_OUTPUT_MOTOR_IN1:
        case GPIO_OUTPUT_MOTOR_IN2:
        // BOOT used for motor control temporarily
        case GPIO_INPUT_BOOT:
            mcu_content->content_type = MTR_CTRL;
            mcu_content->trig_signal  = SIGNAL_LOW;
            break;
        default:
            mcu_content->content_type = (mcu_content_type_t) INVALID;
            mcu_content->trig_signal  = SIGNAL_LOW;
    }
}

void exec_toggle_motor(void) {
    ESP_LOGI(TAG, "toggle e-motor...");

    // Rotate Clockwise
    gpio_set_level(GPIO_OUTPUT_MOTOR_IN1, 1);
    gpio_set_level(GPIO_OUTPUT_MOTOR_IN2, 0);
    vTaskDelay(3000 / portTICK_RATE_MS);

    // Rotate Counter-clockwise
    gpio_set_level(GPIO_OUTPUT_MOTOR_IN1, 0);
    gpio_set_level(GPIO_OUTPUT_MOTOR_IN2, 1);
    vTaskDelay(3000 / portTICK_RATE_MS);

    // Sleep mode
    gpio_set_level(GPIO_OUTPUT_MOTOR_IN1, 0);
    gpio_set_level(GPIO_OUTPUT_MOTOR_IN2, 0);
    vTaskDelay(1000 / portTICK_RATE_MS);
}

esp_err_t stfd_gpio_config(GPIO_INT_TYPE int_type, uint64_t bit_mask, gpio_mode_t gpio_mode, gpio_pulldown_t pull_down, gpio_pullup_t pull_up) {
    gpio_config_t io_conf;

    ///interrupt of rising edge
    io_conf.intr_type = int_type;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = bit_mask;
    //set as input mode    
    io_conf.mode = gpio_mode;

    if(pull_up && pull_down == 0x1) {
        ESP_LOGE(TAG, "Simulaneous Pull up and Pull down requested");
        return ESP_FAIL;
    }
    else {
        // enable/disable pull-up mode
        io_conf.pull_up_en = pull_up;
        // enable/disable pull-down mode
        io_conf.pull_down_en = pull_down;
    }
    ESP_ERROR_CHECK(gpio_config(&io_conf));
    return ESP_OK;
}

void gpio_setup_input(gpio_isr_t isr_handler) {
    // BOOT
    if( stfd_gpio_config(
        GPIO_PIN_INTR_DISABLE, 
        GPIO_0_PIN_SEL, 
        GPIO_MODE_INPUT, 
        GPIO_PULLDOWN_DISABLE, 
        GPIO_PULLUP_ENABLE
        ) != ESP_OK) 
    {
        ESP_LOGE(TAG, "GPIO %i config failed", GPIO_INPUT_MS);
    }
    if( stfd_gpio_config(
        GPIO_PIN_INTR_POSEDGE, 
        GPIO_INPUT_BOOT_PIN_SEL, 
        GPIO_MODE_INPUT, 
        GPIO_PULLDOWN_DISABLE, 
        GPIO_PULLUP_ENABLE
        ) != ESP_OK) 
    {
        ESP_LOGE(TAG, "GPIO %i config failed", GPIO_INPUT_MS);
    }
    // MS
    if( stfd_gpio_config(
        GPIO_PIN_INTR_POSEDGE, 
        GPIO_INPUT_MS_PIN_SEL, 
        GPIO_MODE_INPUT, 
        GPIO_PULLDOWN_ENABLE, 
        GPIO_PULLUP_DISABLE
        ) != ESP_OK) 
    {
        ESP_LOGE(TAG, "GPIO %i config failed", GPIO_INPUT_MS);
    }
    // NSW
    if( stfd_gpio_config(
        GPIO_PIN_INTR_POSEDGE, 
        GPIO_INPUT_NSW_PIN_SEL, 
        GPIO_MODE_INPUT, 
        GPIO_PULLDOWN_DISABLE, 
        GPIO_PULLUP_ENABLE
        ) != ESP_OK)
    {
        ESP_LOGE(TAG, "GPIO %i config failed", GPIO_INPUT_NSW);
    }
    // MOTOR FAULT
    if( stfd_gpio_config(
        GPIO_PIN_INTR_POSEDGE, 
        GPIO_INPUT_MOTOR_FAULT_PIN_SEL, 
        GPIO_MODE_INPUT, 
        GPIO_PULLDOWN_DISABLE, 
        GPIO_PULLUP_ENABLE
        ) != ESP_OK)
    {
        ESP_LOGE(TAG, "GPIO %i config failed", GPIO_INPUT_MOTOR_FAULT);
    }
    // DRBELL
    if( stfd_gpio_config(
        GPIO_PIN_INTR_POSEDGE, 
        GPIO_INPUT_DRBELL_PIN_SEL, 
        GPIO_MODE_INPUT, 
        GPIO_PULLDOWN_DISABLE, 
        GPIO_PULLUP_ENABLE
        ) != ESP_OK)
    {
        ESP_LOGE(TAG, "GPIO %i config failed", GPIO_INPUT_DRBELL_NOTIF);
    }
    // REEDSW
    if( stfd_gpio_config(
        GPIO_PIN_INTR_POSEDGE, 
        GPIO_INPUT_REEDSW_PIN_SEL, 
        GPIO_MODE_INPUT, 
        GPIO_PULLDOWN_DISABLE, 
        GPIO_PULLUP_ENABLE
        ) != ESP_OK)
    {
        ESP_LOGE(TAG, "GPIO %i config failed", GPIO_INPUT_REEDSW_STATUS);
    }

    // Setup ADC
    /*if( stfd_gpio_config(
        GPIO_PIN_INTR_POSEDGE, 
        GPIO_INPUT_BATTERY_PIN_SEL, 
        GPIO_MODE_INPUT, 
        GPIO_PULLDOWN_DISABLE, 
        GPIO_PULLUP_ENABLE
        ) != ESP_OK)
    {
        ESP_LOGE(TAG, "GPIO %i config failed", GPIO_INPUT_BATTERY);
    }
    */

    //install gpio isr service
    //hook isr handler for specific gpio pin
    //Must only be done once. But each gpio can have their own isr handler
    ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT));
    ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_INPUT_BOOT, isr_handler, (void*) GPIO_INPUT_BOOT));
    ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_INPUT_MS, isr_handler, (void*) GPIO_INPUT_MS));
    ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_INPUT_NSW, isr_handler, (void*) GPIO_INPUT_NSW));
    ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_INPUT_MOTOR_FAULT, isr_handler, (void*) GPIO_INPUT_MOTOR_FAULT));
    ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_INPUT_DRBELL_NOTIF, isr_handler, (void*) GPIO_INPUT_DRBELL_NOTIF));
    ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_INPUT_REEDSW_STATUS, isr_handler, (void*) GPIO_INPUT_REEDSW_STATUS));
}

void gpio_setup_output(void) {

    if( stfd_gpio_config(
        GPIO_PIN_INTR_DISABLE, 
        GPIO_OUTPUT_MOTOR_IN1_PIN_SEL, 
        GPIO_MODE_OUTPUT, 
        GPIO_PULLDOWN_DISABLE, 
        GPIO_PULLUP_DISABLE
        ) != ESP_OK)
    {
        ESP_LOGE(TAG, "GPIO %i config failed", GPIO_OUTPUT_MOTOR_IN1);
    }
    if( stfd_gpio_config(
        GPIO_PIN_INTR_DISABLE, 
        GPIO_OUTPUT_MOTOR_IN2_PIN_SEL, 
        GPIO_MODE_OUTPUT, 
        GPIO_PULLDOWN_DISABLE, 
        GPIO_PULLUP_DISABLE
        ) != ESP_OK)
    {
        ESP_LOGE(TAG, "GPIO %i config failed", GPIO_OUTPUT_MOTOR_IN2);
    }

    //Initialize to 0
    ESP_ERROR_CHECK(gpio_set_level((gpio_num_t) GPIO_OUTPUT_MOTOR_IN1_PIN_SEL, 0));
    ESP_ERROR_CHECK(gpio_set_level((gpio_num_t) GPIO_OUTPUT_MOTOR_IN2_PIN_SEL, 0));
}

void gpio_init_setup(gpio_isr_t isr_handler) {
    gpio_assign_check(TAG);
    ESP_LOGI(TAG, "GPIO setup...");
    gpio_setup_input(isr_handler);
    gpio_setup_output();
}
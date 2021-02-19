/*
    Project:    ShutTheFrontDoor
    File:       gpio.c
    Author:     Benjamin Lanthier
    Date:       18/02/2021

    Desc;       This file contains the gpio setup and interrupts
*/

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

#define GPIO_INPUT_PIN_SEL  (1ULL<<GPIO_INPUT_PIC)

#define ESP_INTR_FLAG_DEFAULT 0

static const char* TAG = "GPIO"

void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

bool gpio_setup_for_picture(void)
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
    gpio_isr_handler_add(GPIO_INPUT_PIC, gpio_isr_handler, (void*) GPIO_INPUT_PIC);
}
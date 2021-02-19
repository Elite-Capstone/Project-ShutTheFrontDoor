/*
    Project:    ShutTheFrontDoor
    File:       stfd_peripherals.h
    Author:     Benjamin Lanthier
    Date:       18/02/2021

    Desc:       Header file containing all the added peripheral functions
*/

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_camera.h"

static xQueueHandle gpio_evt_queue = NULL;

uint32_t getDefaultScanListSize(void);
wifi_scan_method_t getDefaultScanMethod(void);

// CAMERA
esp_err_t init_camera();

void init_sdcard();

bool save_image_to_sdcard(camera_fb_t *pic);

void camera_take_picture(bool save_to_sdcard);

// GPIO
void IRAM_ATTR gpio_isr_handler(void* arg);

bool gpio_setup_for_picture(void);

// WiFi Scan
void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data);

void print_auth_mode(int authmode);

void print_cipher_type(int pairwise_cipher, int group_cipher);

wifi_ap_record_t* wifi_scan(void);

void fast_scan(void);

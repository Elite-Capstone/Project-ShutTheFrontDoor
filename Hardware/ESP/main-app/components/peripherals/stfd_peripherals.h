/*
    Project:    ShutTheFrontDoor
    File:       stfd_peripherals.h
    Author:     Benjamin Lanthier
    Date:       18/02/2021

    Desc:       Header file containing all the added peripheral functions
*/
#include <esp_log.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_camera.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SIGNAL_HIGH 1
#define SIGNAL_LOW 0

static xQueueHandle gpio_evt_queue = NULL;

uint32_t getDefaultScanListSize(void);
wifi_scan_method_t getDefaultScanMethod(void);

// CAMERA
/**
 * @brief Initilize the camera
 */
esp_err_t init_camera();

/**
 * @brief Initilize the SD card
 */
void init_sdcard();

/**
 * @brief Saves the image to the SD card locally is the SD card has been initialized
 */
bool save_image_to_sdcard(camera_fb_t *pic);

void camera_take_picture(bool save_to_sdcard);

// GPIO

/**
 * @brief Checks if the triggered input GPIO is valid to perform the action and if it is the required logic value
 * 
 * @param io_num   GPIO input pin number,
 * @param sg_level Desired signal level
 */
bool trigValidGPIO(uint32_t io_num, uint8_t sg_level);

/**
 * @brief This function programs what to do upon interrupt
 * 
 * @param arg   arguments passed from the isr queue
 */
void IRAM_ATTR gpio_isr_handler(void* arg);

/**
 * @brief This function sends a signal pulse through an output GPIO. This function is used for 
 *        debugging communications. Helps confirming operations should have occured.
 * 
 * @param num_blinks number of times the LED should blink (one blink = 1 on/off cycle)
 */
void gpio_blink_output(uint32_t num_blinks);

/**
 * @brief Sets up the selected input and output GPIO from the configuration menu selection
 */
bool gpio_setup_for_picture(void);
bool gpio_setup_input(void);
bool gpio_setup_output(void);

// WiFi Scan
void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data);

void print_auth_mode(int authmode);

void print_cipher_type(int pairwise_cipher, int group_cipher);

/**
 * @brief Scans for all available access points (AP) and returns te compiled list
 */
wifi_ap_record_t* wifi_all_ch_scan(void);

/**
 * @brief Scans and finds the first compatible AP with the matching requirements (SSID, Password, Security level)
 */
void fast_scan(void);

/**
 * @brief Performs the wifi scan and connects to the AP if the scan mode if fast_scan()
 */
void wifi_scan(void);

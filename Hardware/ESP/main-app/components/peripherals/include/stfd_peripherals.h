/*
    Project:    ShutTheFrontDoor
    File:       stfd_peripherals.h
    Author:     Benjamin Lanthier
    Date:       18/02/2021

    Desc:       Header file containing all the added peripheral functions
*/

#ifndef STFD_PERIPHERALS_H_
#define STFD_PERIPHERALS_H_

#include <esp_log.h>
#include <esp_system.h>
#include <esp_event.h>
#include <esp_wifi.h>
#include <esp_http_server.h>
#include "http_server.h"
#include <esp_http_client.h>
#include "esp_camera.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#include "soc/soc.h" //disable brownout problems
#include "soc/rtc_cntl_reg.h"  //disable brownout problems

typedef enum {
    SIGNAL_IGNORED = -1,
    SIGNAL_LOW  = 0x0,
    SIGNAL_HIGH = 0x1
} gpio_sig_level_t;

typedef enum {
    INVALID     = -1,
    STANDBY     = 0,
    BOOT        = 1,
    PICTURE     = 2,
    STREAM      = 3,
    DRBELL      = 4,
    REEDSW      = 5,
    MS          = 6,
    NSW         = 7,
    BATTERY     = 8,
    MTR_CTRL    = 9
} mcu_content_type_t;

typedef struct {
    bool save_to_sdcard;
    bool upload_content;
    bool trig_signal;
    mcu_content_type_t content_type;
    esp_netif_t *netif;
    wifi_ap_record_t* ap_info;
    char* device_ip;
    char* pub_device_ip;
    char* device_path;
    char* jwt;
    uint64_t pic_counter;
} mcu_content_t;

typedef struct {
    bool got_wifi_ip;
    bool cam_initiated;
    bool sdcard_initiated;
    bool cam_server_init;
    bool iotc_core_init;
    bool iotc_server_online;
} mcu_status_t;

uint32_t getDefaultScanListSize(void);
wifi_scan_method_t getDefaultScanMethod(void);

//========== CAMERA ==========
/**
 * @brief Initilize the camera
 */
esp_err_t init_camera(mcu_content_t* mcu_c, mcu_status_t* mcu_s, mcu_content_type_t type);

/**
 * @brief Initilize the SD card
 */
esp_err_t init_sdcard(mcu_status_t* mcu_s);

/**
 * @brief Saves the image to the SD card locally is the SD card has been initialized
 * 
 * @param buf     Image pixel buffer
 * @param len     Size of the image buffer
 * @param pic_cnt Current picture counter
 */
bool save_image_to_sdcard(uint8_t* buf, size_t len, long long int pic_cnt);

/**
 * @brief Executes the picture taking routine and returns the picture framebuffer
 *        Must use esp_camera_return() after to free the memory allocated by esp_camera_fb_get()
 */
camera_fb_t* camera_take_picture(mcu_content_t* cam_c);

/**
 * @brief Converts the camera frame buffer of any format to a JPEG. 
 *        It is assumed that the framebuffer was not returned prior to this function call.
 * 
 * @param fb            The camera frame buffer of any pixel format
 * @param jpeg_buf      The buffer holding the JPEG conversion
 * @param jpeg_buf_len  JPEG Buffer length
 */
esp_err_t convert_to_jpeg(camera_fb_t* fb, uint8_t** jpeg_buf, size_t* jpeg_buf_len);

/**
 * @brief Gets the next frame from the camera frame buffer and outputs the JPEG converted image
 *        in the JPEG buffer
 * @note  The pixel format can only be JPEG for this function
 * 
 * @param jpg_buf       JPEG pointer reference where the pointer to the image buffer is stored
 * @param jpg_buf_len   JPEG buffer length reference.
 * @param frame_time    Time at which the frame is taken
 */
esp_err_t stfd_get_frame(uint8_t** jpg_buf, size_t* jpg_buf_len, int64_t frame_time);

//========== GPIO ==========

/**
 * @brief Checks if the triggered input GPIO is valid to perform the action and if it is the required logic value
 * 
 * @param io_num   GPIO input pin number,
 * @param sg_level Desired signal level
 * 
 * @return         Return value indicates if the correct gpio was triggered with the correct signal value
 */
bool trig_valid_gpio(uint32_t io_num, uint8_t sg_level);
//bool trig_motor_gpio(uint32_t io_num, uint8_t sg_level);

/**
 * @brief This function sends a signal pulse through the output GPIOs for motor control.
 *        This function toggles the motor to rotate once CW and once CCW
 */
void exec_toggle_motor(void);

/**
 * @brief performs the interrupt task for input gpios (Picutre or Stream)
 * 
 * @param io_num            GPIO used to create the interrupt
 * @param mcu_content   MCU content array to which will be passed the type and the desired signal level
 */
void get_io_type(uint32_t io_num, mcu_content_t* mcu_content);

/**
 * @brief
 */
uint32_t get_battery_level(void);

/**
 * @brief This function programs what to do upon interrupt
 * 
 * @param arg   arguments passed from the isr queue
 */
void IRAM_ATTR gpio_isr_handler(void* arg);

/**
 * @brief
 */
uint32_t get_adc_reading(adc_unit_t unit, adc_channel_t channel);

/**
 * @brief Does the configuration for the desired GPIO passed with its bit mask in @param bit_mask. 
 * 
 * @note The pull-down and pull-up functions cannot be used simultaneously. This function will return an error if both are set to 1.
 * 
 * @param int_type  Selects the interrupt type for the GPIO 
 *                  @note Possible values: DISABLE, POSEDGE, NEGEDGE, ANYEDGE, LOLEVEL, HILEVEL
 * @param bit_mask  The bit mask of the GPIO that will be configured
 * @param i2c_mode  The GPIO communication type (DISABLE, INPUT, OUTPUT, OUTPUT_OD, INPUT_OUTPUT_OD, INPUT_OUTPUT) 
 *                  *@note OD == Open-Drain mode
 * @param pull_down Pull-down enable/disable 
 *                  @note GPIO_PULLDOWN_DISABLE == 0 == pull-down deactivated, GPIO_PULLDOWN_ENABLE == 1 == pull-down activated
 * @param pull_up   Pull-up enable/disable 
 *                  @note GPIO_PULLUP_DISABLE == 0 == pull-up deactivated, GPIO_PULLUP_ENABLE == 1 == pull-up activated
 * @return          Returns an error code. The desired output is ESP_OK to indicate normal operations
 */
esp_err_t stfd_gpio_config(GPIO_INT_TYPE int_type, uint64_t bit_mask, gpio_mode_t gpio_mode, gpio_pulldown_t pull_down, gpio_pullup_t pull_up);

/**
 * @brief Sets up the selected input and output GPIO from the configuration menu selection.
 * 
 * @param isr_handler function that will handle the interrupt events on the gpio it is installed on
 */
void gpio_init_setup(gpio_isr_t isr_handler);
void gpio_setup_input(gpio_isr_t isr_handler);
void gpio_setup_adc(void);
void gpio_setup_output(void);

//========== WiFi Scan ==========

void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data);

void print_auth_mode(int authmode);

void print_cipher_type(int pairwise_cipher, int group_cipher);

/**
 * @brief Scans for all available access points (AP) and returns te compiled list
 */
void wifi_all_ch_scan(wifi_ap_record_t* ap_info);

/**
 * @brief Scans and finds the first compatible AP with the matching requirements (SSID, Password, Security level)
 */
void fast_scan(mcu_status_t* mcu_s, wifi_ap_record_t* ap_info);

/**
 * @brief Performs the wifi scan and connects to the AP if the scan mode if fast_scan().
 *        It assigns the wifi list or AP info to the passed pointer
 * 
 * @param ap_info Pointer to which the list is assigned to
 */
void wifi_scan(mcu_content_t* mcu_content, mcu_status_t* mcu_status);

#endif /*STFD_PERIPHERALS_H_*/
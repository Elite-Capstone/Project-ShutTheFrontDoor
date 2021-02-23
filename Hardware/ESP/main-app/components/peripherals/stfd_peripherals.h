/*
    Project:    ShutTheFrontDoor
    File:       stfd_peripherals.h
    Author:     Benjamin Lanthier
    Date:       18/02/2021

    Desc:       Header file containing all the added peripheral functions
*/
#include <esp_log.h>
#include <esp_system.h>
#include <esp_event.h>
#include <esp_wifi.h>
#include <esp_http_server.h>
#include <esp_http_client.h>
#include "esp_camera.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#include "soc/soc.h" //disable brownout problems
#include "soc/rtc_cntl_reg.h"  //disable brownout problems

#define SIGNAL_HIGH 1
#define SIGNAL_LOW 0

typedef enum {
    INVALID = -1,
    STANDBY = 0,
    PICTURE = 1,
    STREAM  = 2,
    DRBELL  = 3,
    REEDSW  = 4
} mcu_content_type_t;

typedef struct {
    bool cam_initiated;
    bool sdcard_initiated;
    bool cam_server_init;
    bool save_to_sdcard;
    bool upload_content;
    mcu_content_type_t content_type;
    wifi_ap_record_t* ap_info;
    char* device_ip;
    long long int pic_counter;
} mcu_content_t;

uint32_t getDefaultScanListSize(void);
wifi_scan_method_t getDefaultScanMethod(void);

//========== CAMERA ==========
/**
 * @brief Initilize the camera
 */
esp_err_t init_camera(mcu_content_t* cam_c);

/**
 * @brief Initilize the SD card
 */
esp_err_t init_sdcard(mcu_content_t* cam_c);

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
esp_err_t convert_to_jpeg(camera_fb_t* fb, uint8_t* jpeg_buf, size_t jpeg_buf_len);

//========== GPIO ==========

/**
 * @brief Checks if the triggered input GPIO is valid to perform the action and if it is the required logic value
 * 
 * @param io_num   GPIO input pin number,
 * @param sg_level Desired signal level
 */
bool trig_valid_gpio(uint32_t io_num, uint8_t sg_level);

/**
 * @brief This function sends a signal pulse through an output GPIO. This function is used for 
 *        debugging communications. Helps confirming operations should have occured.
 * 
 * @param num_blinks number of times the LED should blink (one blink = 1 on/off cycle)
 */
void gpio_blink_output(uint32_t num_blinks);

/**
 * @brief performs the interrupt task for input gpios (Picutre or Stream)
 * 
 * @param io_num            GPIO used to create the interrupt
 * @param http_upload       Boolean from configuration menu indicating if the content is to uploaded
 * @param save_to_sdcard    Boolean from configuration menu. Indicates if the content is to 
 *                          be saved on the local SD card
 */
mcu_content_type_t gpio_io_type(uint32_t io_num);

/**
 * @brief This function programs what to do upon interrupt
 * 
 * @param arg   arguments passed from the isr queue
 */
void IRAM_ATTR gpio_isr_handler(void* arg);

/**
 * @brief Sets up the selected input and output GPIO from the configuration menu selection
 */
void gpio_setup_for_picture(gpio_isr_t isr_handler);
void gpio_setup_input(gpio_isr_t isr_handler);
void gpio_setup_output(void);

//========== HTTP client ==========

/**
 * @brief Interrupt handler for an HTTP event
 * 
 * @param evt
 */
esp_err_t _http_event_handler(esp_http_client_event_t *evt);

/**
 * @brief Begins the HTTP transaction to the set URL
 * 
 * @param buf Transfer file content buffer
 * @param len Transfer file content length
 */
void http_rest_with_url_upload_picture(uint8_t* buf, size_t len);

/**
 * @brief Uploads the attached message as a notification to the backend
 * 
 * @param message The message to attach to the notification
 */
void http_rest_with_url_notification(const char* _message);

/**
 * @brief HTTP event handler for streaming
 * 
 * @param req HTTP request
 */
esp_err_t stream_handler(httpd_req_t *req);

/**
 * @brief Function begins camera server for streaming
 * 
 * @param device_ip ESP camera's IP address on which it is streaming on
 */
void startStreamServer(char* device_ip);

/**
 * @brief Function stop camera server from streaming and deallocates the memory
 */
void stopStreamServer(void);

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
void fast_scan(wifi_ap_record_t* ap_info);

/**
 * @brief Performs the wifi scan and connects to the AP if the scan mode if fast_scan().
 *        It assigns the wifi list or AP info to the passed pointer
 * 
 * @param ap_info Pointer to which the list is assigned to
 */
void wifi_scan(mcu_content_t* cam_content);

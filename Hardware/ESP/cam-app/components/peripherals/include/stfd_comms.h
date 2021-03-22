/*
    Project:    ShutTheFrontDoor
    File:       stfd_comms.h
    Author:     Benjamin Lanthier
    Date:       19/03/2021

    Desc:       Header file containing all the added external communication functions
*/

#ifndef STFD_COMMS_H_
#define STFD_COMMS_H_

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

#include "lwip/sockets.h"
#include "soc/soc.h" //disable brownout problems
#include "soc/rtc_cntl_reg.h"  //disable brownout problems

#define DEFAULT_DOOR_UUID  "00b288a8-3db1-40b5-b30f-532af4e12f4b"
#define DEFAULT_HTTP_URL   "http://34.117.160.50/"
#define DEFAULT_STREAM_URI "/stream"

//========== HTTP client ==========

/**
 * @brief Interrupt handler for an HTTP event
 * 
 * @param evt   ...
 * @return      Returns an error code. The desired output is ESP_OK to indicate normal operations
 */
esp_err_t _http_event_handler(esp_http_client_event_t *evt);

/**
 * 
 */
void http_rest_with_url_get_device_ip();

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
 * @return    Returns an error code. The desired output is ESP_OK to indicate normal operations
 */
esp_err_t stream_handler(httpd_req_t *req);

/**
 * @brief Function begins camera server for streaming
 * 
 * @param device_ip ESP camera's IP address on which it is streaming on
 */
httpd_handle_t startStreamServer(char* device_ip);

/**
 * @brief Function stop camera server from streaming and deallocates the memory
 */
void stopStreamServer(httpd_handle_t* httpd_handle);

//========== UDP Client ==========
/**
 * @brief Sets up the TCP client socket and connects it to the server port
 * 
 * @param sock  Pointer to socket to be initialized
 */
esp_err_t udp_setup_sock(int* sock, void* dest_addr, esp_netif_t* esp_netif);

/**
 * @brief Send the jpeg format image buffer to the TCP server
 * 
 * @param sock          Socket used to send the image
 * @param jpg_buf       JPEG converted image buffer
 * @param jpg_buf_len   JPEG converted buffer length
 */
esp_err_t udp_send_buf (int* sock, struct sockaddr* dest_addr, uint8_t* jpg_buf, size_t jpg_buf_len);

#endif /* STFD_COMMS_H_ */
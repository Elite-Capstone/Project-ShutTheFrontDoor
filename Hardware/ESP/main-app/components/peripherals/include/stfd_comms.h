/*
    Project:    ShutTheFrontDoor
    File:       stfd_comms.h
    Author:     Benjamin Lanthier
    Date:       19/03/2021

    Desc:       Header file containing all the added external communication functions
*/

#ifndef STFD_COMMS_H_
#define STFD_COMMS_H_

// #include "stfd_peripherals.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <cJSON.h> 

#include <time.h>
#include <esp_sntp.h>

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

#include "lwip/apps/sntp.h"
#include "lwip/sockets.h"
#include "soc/soc.h" //disable brownout problems
#include "soc/rtc_cntl_reg.h"  //disable brownout problems
#include "mqtt_client.h"
#include <iotc.h>

#undef  MQTT_MAX_LWT_TOPIC
#define MQTT_MAX_LWT_TOPIC          128
#undef  MQTT_MAX_LWT_MSG
#define MQTT_MAX_LWT_MSG            1024

#define DEFAULT_DOOR_UUID   "00b288a8-3db1-40b5-b30f-532af4e12f4b"

// HTTP connection
#define DEFAULT_HTTP_URL    "http://34.117.160.50/"
#define DEFAULT_STREAM_URI  "/stream"

// MQTT connection
#define MQTT_BROKER_URL     "mqtt://34.95.9.208:1883" //"mqtt://mqtt.eclipseprojects.io:1883" //"mqtt://192.168.1.17:1883"
#define NOTIFICATION_TOPIC  "notification/" DEFAULT_DOOR_UUID
#define STATUS_TOPIC        "status/" DEFAULT_DOOR_UUID
#define CMD_TOPIC           "command/" DEFAULT_DOOR_UUID

// TimeOfPublish JSON Format
#define TIME_YEAR   "year"
#define TIME_MONTH  "month"
#define TIME_DAY    "day"
#define TIME_HOUR   "hour"
#define TIME_MIN    "minute"
#define TIME_SEC    "second"
#define TIME_MS     "millisecond"

// Command JSON Format
#define COMMAND_NAME        "commandRequest"
#define COMMAND_TIME        "timeOfPublish"
#define COMMAND_TARGET      "targetDevice"
#define COMMAND_REQUEST     "commandRequest"
#define COMMAND_FLAG        "requestFlag"
#define COMMAND_DELAY       "commandDelayMS"

// Status JSON Format
#define STATUS_TIME           "timeOfPublish"
#define STATUS_LIST           "statusList"
#define STATUS_WIFI_IP        "gotWifiIP"
#define STATUS_CAM            "camInit"
#define STATUS_SDCARD         "sdcardInit"
#define STATUS_CAM_STREAM     "camStreamInit"
#define STATUS_DOOR_IS_LOCKED "doorLocked"
#define STATUS_DOOR_IS_CLOSED "doorClosed"
#define STATUS_BATTERY_LEVEL  "batteryLevel"

// Device commands
#define MCU_SHUTDOWN_STR     "MCU shutdown"
#define MCU_GETSTATUS_STR    "Get status"
#define MCU_GETNOTIF_STR     "Get notification"
#define TOGGLE_LOCK_DOOR_STR "Toggle door lock"
#define LOCK_DOOR_STR        "Lock door"
#define UNLOCK_DOOR_STR      "Unlock door"
#define STREAM_CAM_STR       "Stream camera"

typedef enum {
    MCU_INVALID      = -1,
    MCU_SHUTDOWN     = 0x0,
    MCU_GETSTATUS    = 0x1,
    LOCK_DOOR        = 0x2,
    UNLOCK_DOOR      = 0x3,
    STREAM_CAM       = 0x4,
    MCU_GETNOTIF     = 0x5,
    TOGGLE_LOCK_DOOR = 0x6,
} mcu_cmd_type_t;

typedef struct {
    struct tm timeinfo;
    mcu_cmd_type_t cmd;
    int  flag;
    int  cmd_delay_ms;
    bool exec_cmd; // true when the command is new and should be executed / false when it has been executed
} mcu_cmd_t;

typedef struct {
    esp_mqtt_client_handle_t client;
    bool   sntp_init;
    int    msg_id;
    cJSON* json_status;
    mcu_cmd_t cmd_info;
} mcu_mqtt_msg_t;

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
 * @brief Sets up the UDP client socket and sets the address of the server port
 * 
 * @param sock  Pointer to socket to be initialized
 */
esp_err_t udp_setup_sock(int* sock, struct sockaddr* dest_addr, esp_netif_t* esp_netif);

/**
 * @brief Send the jpeg format image buffer to the UDP server
 * 
 * @param sock           Socket used to send the image
 * @param dest_addr      Address structure that holds the IP and Port of the socket to send to
 * @param dest_addr_size Size of the passed structure (using sizeof())
 * @param jpg_buf        JPEG converted image buffer
 * @param jpg_buf_len    JPEG converted buffer length
 *
 * @return ESP_OK when sent correctly, ESP_FAIL if a problem occured
 */
esp_err_t udp_send_buf(int* sock, struct sockaddr* dest_addr, size_t dest_addr_size, void* buf, size_t buf_len);

/**
 * @brief Send the jpeg format image buffer to the UDP server
 * 
 * @param sock           Socket used to send the image
 * @param dest_addr      Address structure that holds the IP and Port of the socket to send to
 * @param dest_addr_size Size of the passed structure (using sizeof())
 * 
 * @return ESP_OK when sent correctly, ESP_FAIL if a problem occured
 */
esp_err_t udp_shutdown(int* sock, struct sockaddr* dest_addr, size_t dest_adrr_size);

//========== Google IoT Client ==========
esp_err_t iotc_init(char* device_path);

void iotc_init_context(char* jwt);

void stfd_publish_scheduled_events(iotc_context_handle_t context_handle, void *user_data);

void stfd_publish_action_event(iotc_context_handle_t context_handle, iotc_timed_task_handle_t timed_task, void *user_data);

void publish_telemetry_event(iotc_context_handle_t context_handle, iotc_timed_task_handle_t timed_task, void *user_data);

void stfd_mqtt_subscribe_to_commands(iotc_context_handle_t in_context_handle);

esp_err_t stfd_mqtt_task(char* device_path, char* jwt);

//========== MQTT Client ==========

void stfd_init_status_json(cJSON** root);
void stfd_set_status_json(cJSON** root, bool got_wifi_ip, bool cam_init, bool sdcard_init, bool cam_stream_init, bool door_locked, bool door_closed, int bat_level);
void stfd_parse_json_command(char* json_cmd, struct tm* timeinfo, mcu_cmd_type_t* cmd, int* flag, int* cmd_delay_ms);
/**
 * @brief Initialize the MQTT connection
 *
 * @param mcu_mqtt Structure which will hold the JSON status
 */
void stfd_mqtt_init(mcu_mqtt_msg_t* mcu_mqtt);

/**
 * @brief Scheduled tasks that must run periodically
 */
void stfd_mqtt_scheduled_task(mcu_mqtt_msg_t* mcu_mqtt);

/**
 * @brief
 */
void stfd_mqtt_publish_status(mcu_mqtt_msg_t* mcu_mqtt);

/**
 * @brief Sends a notification to the MQTT broker for redistribution to subs
 */
void stfd_mqtt_publish_notif(esp_mqtt_client_handle_t client, const char* msg);

void stfd_mqtt_close_client(mcu_mqtt_msg_t* mcu_mqtt);

#endif /* STFD_COMMS_H_ */
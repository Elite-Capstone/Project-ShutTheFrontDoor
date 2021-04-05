/*
    Project:    ShutTheFrontDoor
    File:       main.c
    Author:     Benjamin Lanthier
    Date:       18/02/2021

    Desc:       This file contains the description of the main application of the device.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/param.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"

#include "stfd_peripherals.h"
#include "stfd_comms.h"

#if (CONFIG_IMAGE_TO_SDCARD || CONFIG_IMAGE_TO_BOTH_PROT)
#define IMAGE_TO_SDCARD 1
#elif CONFIG_IMAGE_TO_HTTP_UPLOAD
#define IMAGE_TO_SDCARD 0
#else 
#define IMAGE_TO_SDCARD 0
#endif /* CONFIG IMAGE_TO_SD_CARD */

#if CONFIG_IMAGE_TO_HTTP_UPLOAD
#define IMAGE_TO_HTTP_UPLOAD 1
#else
#define IMAGE_TO_HTTP_UPLOAD 0
#endif /* CONFIG IMAGE_TO_HTTP_UPLOAD */

#if IMAGE_TO_SDCARD
#define INIT_SDCARD 1
#else
#define INIT_SDCARD 0
#endif /* CONFIG INIT_SDCARD */

#define GPIO_TASK 1
#define MQTT_TASK 1

static const char* TAG = "main";
static const char* DRBELL_MSG = "Doorbell pressed - Someone's at the Door!";
static const char* REEDSW_MSG = "The Door opened";
static const char* STREAM_INIT_MSG = "Started video streaming the door's view";
static const char* payload = "ESP32 Message";

// Holds the task statuses
typedef struct {
    bool gpio_task_init;
    bool gpio_task_created;
    bool stream_task_init;
    bool stream_task_created;
    bool mqtt_task_init;
    bool mqtt_task_created;
} mcu_tasklist_t;

static httpd_handle_t stream_httpd = NULL;
static xQueueHandle gpio_evt_queue = NULL;

mcu_content_t _mcu_c = {
    .save_to_sdcard     = false,
    .upload_content     = false,
    .trig_signal        = SIGNAL_IGNORED,
    .content_type       = STANDBY,
    .netif              = NULL,
    .ap_info            = NULL,
    .device_ip          = "",
    .pub_device_ip      = "",
    // .device_path        = "",
    // .jwt                = "",
    .pic_counter        = 0
};
static mcu_content_t* mcu_c = &_mcu_c;

mcu_status_t _mcu_s = {
    .got_wifi_ip        = false,
    .cam_initiated      = false,
    .sdcard_initiated   = false,
    .cam_server_init    = false,
    .door_is_locked     = false,
    .door_is_closed     = false,
    .bat_level          = 0
    // .iotc_core_init     = false,
    // .iotc_server_online = false
};
static mcu_status_t* mcu_s = &_mcu_s;

mcu_mqtt_msg_t _mcu_mqtt = {
    .client      = NULL,
    .sntp_init   = false,
    .msg_id      = 0,
    .json_status = NULL,
    .cmd_info = {
        .timeinfo       = {0},
        .cmd            = MCU_INVALID,
        .flag           = 0,
        .cmd_delay_ms   = 0,
        .exec_cmd       = false
    }
};
static mcu_mqtt_msg_t* mcu_mqtt = &_mcu_mqtt;

static mcu_tasklist_t mcu_tl = {
    .gpio_task_init      = false,
    .gpio_task_created   = false,
    .stream_task_init    = false,
    .stream_task_created = false,
    .mqtt_task_init      = false,
    .mqtt_task_created   = false,
};

// Forward Declaration
void IRAM_ATTR gpio_isr_handler(void* arg);
void exec_gpio_task(mcu_content_t* mcu_c);

static void task_listener(void* arg);
static void gpio_trig_task(void* arg);
static void mqtt_task(void* pvParameters);

/**
 * @brief Handler for GPIO interrupts
 * 
 * @param arg Allocated memory data passed to the handler.
 *            In this case, the data is the triggered GPIO 
 */
void IRAM_ATTR gpio_isr_handler(void* arg) {
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

/**
 * @brief Task handler that listens to requests and creates tasks accordingly
 * 
 */
static void task_listener(void* arg) {
    for(;;) 
    {
#if GPIO_TASK
        if (!mcu_tl.gpio_task_created && mcu_tl.gpio_task_init) {
            // Start gpio task
            ESP_LOGI(TAG, "Creating GPIO task");
            xTaskCreate(&gpio_trig_task, "gpio_trig_task", 8192, NULL, 3, NULL);
            mcu_tl.gpio_task_created = true;
            mcu_tl.gpio_task_init    = false;
        }
#endif

#if MQTT_TASK
        if (!mcu_tl.mqtt_task_created && mcu_tl.mqtt_task_init) {
            // Start Google IoT Cloud comms
            ESP_LOGI(TAG, "Creating MQTT task");
            xTaskCreate(&mqtt_task, "mqtt_task", 4096, NULL, 10, NULL);
            mcu_tl.mqtt_task_created = true;
        }
#endif
    }
    mcu_tl.gpio_task_created   = false;
    mcu_tl.stream_task_created = false;
    mcu_tl.mqtt_task_created   = false;
    vTaskDelete(NULL);
}

/**
 * @brief Takes a picture when the input gpio is pulled down by a switch
 */
static void gpio_trig_task(void* arg)
{
    uint32_t io_num;
    for(;;) {
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
            get_io_type(io_num, mcu_c);
            mcu_c->save_to_sdcard = IMAGE_TO_SDCARD;
            mcu_c->upload_content = IMAGE_TO_HTTP_UPLOAD;

            if (trig_valid_gpio(io_num, mcu_c->trig_signal)) {
                gpio_blink(1);
                exec_gpio_task(mcu_c);
            }
            else {
                ESP_LOGI(TAG, "Triggered the wrong input pin or wrong signal level");
            }   
        }
    }
}

/**
 * @brief Task used to communicate with Google IoT Core
 */
static void mqtt_task(void* pvParameters) {
    stfd_mqtt_init(mcu_mqtt);
    while(mcu_tl.mqtt_task_init) {
        
        if (mcu_mqtt->cmd_info.exec_cmd) {
            switch (mcu_mqtt->cmd_info.cmd) {
                case (mcu_cmd_type_t) MCU_GETSTATUS:
                    // Update json_status object                
                    stfd_set_status_json(
                        &mcu_mqtt->json_status,
                        mcu_s->got_wifi_ip,
                        mcu_s->cam_initiated,
                        mcu_s->sdcard_initiated,
                        mcu_s->cam_server_init,
                        mcu_s->door_is_locked,
                        mcu_s->door_is_closed,
                        mcu_s->bat_level
                    );
                    stfd_mqtt_publish_status(mcu_mqtt);
                    break;
                case (mcu_cmd_type_t) MCU_GETNOTIF:
                    stfd_mqtt_publish_notif(mcu_mqtt->client, "Notification Message");
                    break;
                case (mcu_cmd_type_t) TOGGLE_LOCK_DOOR:
                    //exec_toggle_motor();
                    break;
                case (mcu_cmd_type_t) LOCK_DOOR:
                    //exec_operate_lock(true);
                    break;
                case (mcu_cmd_type_t) UNLOCK_DOOR:
                    //exec_operate_lock(false);
                    break;
                case (mcu_cmd_type_t) STREAM_CAM:
                    if (!(mcu_s->cam_server_init)) {
                        stfd_mqtt_publish_notif(mcu_mqtt->client, STREAM_INIT_MSG);
                        init_camera(mcu_c, mcu_s, STREAM);
                        stream_httpd = startStreamServer(mcu_c->device_ip);
                        mcu_s->cam_server_init = true;
                        }
                        // else {
                        //     stopStreamServer(&stream_httpd);
                        //     mcu_c->cam_server_init = false;
                        
                        //     if (esp_camera_deinit() != ESP_OK)
                        //         ESP_LOGE(TAG, "Camera De-Init Failed");
                        //     else
                        //         mcu_c->cam_initiated = false;
                        //      }
                    break;
                default:
                    ESP_LOGW(TAG, "Invalid command type received from MQTT");
                    break;
            }
            mcu_mqtt->cmd_info.exec_cmd = false;
        }
        vTaskDelay(2000 / portTICK_RATE_MS);
    }
    stfd_mqtt_close_client(mcu_mqtt);
    mcu_tl.mqtt_task_created = false;
    ESP_LOGI(TAG, "Deleting MQTT task");
    vTaskDelete(NULL);
}


/**
 * @brief Selects the task to execute (picture or stream) according to the passed camera content
 * 
 * @param mcu_c Camera current status with its content
 */
void exec_gpio_task(mcu_content_t* mcu_c) {    

    switch (mcu_c->content_type) {
       
        case (mcu_content_type_t) STREAM:
            if (!mcu_tl.stream_task_created) {
                ESP_LOGI(TAG, "Starting Stream from GPIO");
                stfd_mqtt_publish_notif(mcu_mqtt->client, STREAM_INIT_MSG);
                mcu_tl.stream_task_init = true;
            }
            else {
                ESP_LOGI(TAG, "Stopping Stream from GPIO");
                mcu_tl.stream_task_init = false;
            }
            break;
        case (mcu_content_type_t) INVALID:
        default:
            ESP_LOGE(TAG, "Invalid type detected when executing queue task");
    }
}

void app_main(void) {
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    // Initialization
    if (INIT_SDCARD)
        init_sdcard(mcu_s);
    gpio_init_setup(gpio_isr_handler);
    init_camera(mcu_c, mcu_s, STREAM);
    wifi_scan(mcu_c, mcu_s);
    // if (iotc_init(mcu_c->device_path) == ESP_OK) {
    //     mcu_s->iotc_core_init = true;
    // }

    // Task listener
    xTaskCreate(&task_listener, "task_listener", 4096, NULL, portPRIVILEGE_BIT, NULL);
    // Initialize the GPIOs and MQTT by default
    mcu_tl.gpio_task_init = true;
    mcu_tl.mqtt_task_init = true;
}
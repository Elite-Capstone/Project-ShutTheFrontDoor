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
#define UDP_STREAM_TASK 1
#define MQTT_TASK 1

static const char* TAG = "main";
static const char* DRBELL_MSG = "Doorbell pressed - Someone's at the Door!";
static const char* REEDSW_MSG = "The Door opened";

static char* payload = "ESP32 Message";

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
    .mqtt_task_created   = false
};

// Forward Declaration
void IRAM_ATTR gpio_isr_handler(void* arg);
static void exec_gpio_task(mcu_content_t* mcu_c);

static void task_listener(void* arg);
static void gpio_trig_task(void* arg);
static void mqtt_task(void* pvParameters);
static void udp_client_task(void *pvParameters);

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
#endif /* GPIO_TASK */

#if UDP_STREAM_TASK
        if (!mcu_tl.stream_task_created && mcu_tl.stream_task_init) {
            // Must initialize Wifi with event IP_EVENT_STA_GOT_IP
            ESP_LOGI(TAG, "Creating UDP Stream task");
            xTaskCreate(&udp_client_task, "udp_client_task", 4096, NULL, 10, NULL);
            mcu_tl.stream_task_created = true;
        }
#endif /* UDP_STREAM_TASK */

#if MQTT_TASK
        if (!mcu_tl.mqtt_task_created && mcu_tl.mqtt_task_init) {
            ESP_LOGI(TAG, "Creating MQTT task");
            xTaskCreate(&mqtt_task, "mqtt_task", 8192, NULL, 5, NULL);
            mcu_tl.mqtt_task_created = true;
        }
#endif /* MQTT_TASK */
    }
    mcu_tl.gpio_task_created   = false;
    mcu_tl.stream_task_created = false;
    mcu_tl.mqtt_task_created   = false;
    vTaskDelete(NULL);
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
                    stfd_mqtt_publish_status(mcu_mqtt);
                    break;
                case (mcu_cmd_type_t) MCU_GETNOTIF:
                    stfd_mqtt_publish_notif(mcu_mqtt->client, "Notification Message");
                    break;
                case (mcu_cmd_type_t) LOCK_DOOR:
                    exec_toggle_motor();
                    break;
                case (mcu_cmd_type_t) UNLOCK_DOOR:
                    break;
                case (mcu_cmd_type_t) STREAM_CAM:
                    mcu_tl.stream_task_init = true;
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
 * @brief Task to send a video buffer through UDP.
 *        When the status of cam_server_init is set to false
 *        the server is sent a shut down command through
 *        the message buffer
 */
static void udp_client_task(void *pvParameters) {
    int sock;
#if defined(CONFIG_SERVER_IPV4)
    struct sockaddr_in dest_addr;
#elif defined(CONFIG_SERVER_IPV6)
    struct sockaddr_in6 dest_addr = { 0 };
#endif

    uint8_t* jpg_buf = NULL;
    size_t   jpg_buf_len = 0;

    int64_t  last_frame = 0;
    int64_t  fr_end;
    int64_t  frame_time = 0;

    if (!(mcu_s->cam_server_init)) {
        if (init_camera(mcu_c, mcu_s, STREAM) != ESP_OK) {
            ESP_LOGE(TAG, "Could not initialize during UPD initialization");
            mcu_s->cam_initiated   = false;
            mcu_s->cam_server_init = false;
            return;
        }
        mcu_s->cam_initiated   = true;
        mcu_s->cam_server_init = true;
    }
    else {
        ESP_LOGI(TAG, "Cam already init");
    }

    while(!mcu_s->got_wifi_ip) {
        // Loop until condition is met
        ESP_LOGI(TAG, "Waiting for condition IP_EVENT_STA_GOT_IP");
        vTaskDelay(1000/portTICK_RATE_MS);
    }

    for(;;) {
        if (udp_setup_sock(&sock, (struct sockaddr*) &dest_addr, mcu_c->netif) != ESP_OK)
            break;
        while (mcu_tl.stream_task_init) {
            if(!last_frame) {
                last_frame = esp_timer_get_time();
            }
            /*
                Receive buffer from ESP32-CAM instead
            */
            // if (stfd_get_frame(&jpg_buf, &jpg_buf_len, frame_time) != ESP_OK)
            //     break;

            fr_end = esp_timer_get_time();
            frame_time = fr_end - last_frame;
            last_frame = fr_end;
            frame_time /= 1000;

            if (udp_send_buf(&sock, (struct sockaddr*) &dest_addr, sizeof(dest_addr), payload, strlen(payload)) != ESP_OK)
            // if (udp_send_buf(&sock, (struct sockaddr*) &dest_addr, sizeof(dest_addr), jpg_buf, jpg_buf_len) != ESP_OK)
            {
                ESP_LOGI(TAG, "udp_send_buf failed");
                vTaskDelete(NULL);
                break;
            }
        }

        vTaskDelay(2000/portTICK_RATE_MS);

        if (sock != -1 && mcu_tl.stream_task_init) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
        else if (!mcu_tl.stream_task_init) {
            // Stream was manually shut down
            if (udp_shutdown(&sock, (struct sockaddr*) &dest_addr, sizeof(dest_addr)) != ESP_OK) 
            {
                ESP_LOGE(TAG, "Couldn't shut down server-side properly");
            }
            break;
        }
    }
    last_frame = 0;
    mcu_tl.stream_task_created = false;
    ESP_LOGI(TAG, "Deleting UDP Streaming task");
    vTaskDelete(NULL);
}

void IRAM_ATTR gpio_isr_handler(void* arg) {
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
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
                exec_gpio_task(mcu_c);
            }
            else {
                ESP_LOGI(TAG, "Triggered the wrong input pin or wrong signal level");
            }   
        }
    }
    mcu_tl.gpio_task_created = false;
    ESP_LOGI(TAG, "Deleting GPIO task");
    vTaskDelete(NULL);
}

/**
 * @brief Selects the task to execute (picture or stream) according to the passed camera content
 * 
 * @param mcu_c Camera current status with its content
 */
static void exec_gpio_task(mcu_content_t* mcu_c) {    
    camera_fb_t* camera_pic;
    uint8_t* jpeg_buf = NULL;
    size_t jpeg_buf_len = 0;

    switch (mcu_c->content_type) {
        case (mcu_content_type_t) PICTURE:
            init_camera(mcu_c, mcu_s, PICTURE);

            camera_pic = camera_take_picture(mcu_c);
            convert_to_jpeg(camera_pic, &jpeg_buf, &jpeg_buf_len);

            if (mcu_c->save_to_sdcard && mcu_s->sdcard_initiated)
                save_image_to_sdcard(jpeg_buf, jpeg_buf_len, mcu_c->pic_counter);

            if (mcu_c->upload_content) {   
                ESP_LOGI(TAG, "Uploading picture");
                //ESP_LOGI(TAG, "buffer data\n %s and its length: %i", (const char*) jpeg_buf, jpeg_buf_len);
                ESP_LOGI(TAG, "buffer length: %i", jpeg_buf_len);
                http_rest_with_url_upload_picture(jpeg_buf, jpeg_buf_len);
            }
            break;

        case (mcu_content_type_t) STREAM:
            if (!mcu_tl.stream_task_created) {
                ESP_LOGI(TAG, "Starting Stream from GPIO");
                mcu_tl.stream_task_init = true;
            }
            else {
                ESP_LOGI(TAG, "Stopping Stream from GPIO");
                mcu_tl.stream_task_init = false;
            }

            // if (!(mcu_s->cam_server_init)) {
            //     // init_camera(mcu_c, mcu_s, STREAM);
            //     // stream_httpd = startStreamServer(mcu_c->device_ip);
            //     // mcu_s->cam_server_init = true;
            // }
            //
            // else {
            //     stopStreamServer(&stream_httpd);
            //     mcu_c->cam_server_init = false;
            //
            //     if (esp_camera_deinit() != ESP_OK)
            //         ESP_LOGE(TAG, "Camera De-Init Failed");
            //     else
            //         mcu_c->cam_initiated = false;
            // }
            break;

        case (mcu_content_type_t) DRBELL:
            //http_rest_with_url_notification(DRBELL_MSG);
            stfd_mqtt_publish_notif(mcu_mqtt->client, DRBELL_MSG);
            break;
        case (mcu_content_type_t) REEDSW:
            //http_rest_with_url_notification(REEDSW_MSG);
            stfd_mqtt_publish_notif(mcu_mqtt->client, REEDSW_MSG);
            break;
        case (mcu_content_type_t) STANDBY:
            ESP_LOGI(TAG, "Standing by... 10sec");
            vTaskDelay(10000/portTICK_RATE_MS);
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
    //init_camera(mcu_c, mcu_s, STREAM);
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
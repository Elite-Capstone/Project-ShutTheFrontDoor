/*
    Project:    ShutTheFrontDoor
    File:       stfd_camera.c
    Author:     Benjamin Lanthier
    Date:       29/03/2021

    Desc:       Source file for communicating with the mqtt protocol
*/
#include "stfd_comms.h"

#include "freertos/semphr.h"

#include "lwip/dns.h"
#include "lwip/netdb.h"

static const char *TAG = "stfd_mqtt_client";

static void initialize_sntp(void)
{
    int retry = 0;
    const int retry_count = 10;

    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();

    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
} 

static struct tm obtain_time(void)
{
    // wait for time to be set
    time_t now = 0;
    struct tm timeinfo = {0};
    while (timeinfo.tm_year < (2016 - 1900)) {
        time(&now);
        localtime_r(&now, &timeinfo);
        // vTaskDelay(500 / portTICK_PERIOD_MS);
        // ESP_LOGI(TAG, "...");
    }
    ESP_LOGI(TAG, "Time is set...");
    return timeinfo;
}

char* obtain_time_string(void) {
    struct tm timeinfo = obtain_time();
    char* time = NULL;
    //DD/MM/YYYY - HH:MM:SS
    asprintf(&time, "%i/%i/%i - %i:%i:%i", 
            timeinfo.tm_mday, timeinfo.tm_mon, timeinfo.tm_year,
            timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec
            );
    return time;
}

static mcu_cmd_type_t get_mqtt_cmd_type(const char* cmd) {
    if (cmd != NULL) {
        if (!strcmp(cmd, MCU_SHUTDOWN_STR))
            return (mcu_cmd_type_t) MCU_SHUTDOWN;
        else if (!strcmp(cmd, MCU_GETSTATUS_STR))
            return (mcu_cmd_type_t) MCU_GETSTATUS;
        else if (!strcmp(cmd, LOCK_DOOR_STR))
            return (mcu_cmd_type_t) LOCK_DOOR;
        else if (!strcmp(cmd, MCU_GETNOTIF_STR))
            return (mcu_cmd_type_t) MCU_GETNOTIF;
        else if (!strcmp(cmd, TOGGLE_LOCK_DOOR_STR))
            return (mcu_cmd_type_t) TOGGLE_LOCK_DOOR;
        else if (!strcmp(cmd, UNLOCK_DOOR_STR))
            return (mcu_cmd_type_t) UNLOCK_DOOR;
        else if (!strcmp(cmd, STREAM_CAM_STR))
            return (mcu_cmd_type_t) STREAM_CAM;
        else
            return (mcu_cmd_type_t) MCU_INVALID;
    } 
    else return (mcu_cmd_type_t) MCU_INVALID;
}

// JSON status object format
void stfd_init_status_json(cJSON** root) {
    stfd_set_status_json(root, false, false, false, false, false, false, 0);

    // cJSON *publish_time, *statuses;
    // struct tm timeinfo = obtain_time();
    //
    // *root = cJSON_CreateObject();
    // publish_time = cJSON_CreateObject();
    // statuses = cJSON_CreateObject();
    //
    // cJSON_AddItemToObject(*root,        STATUS_TIME,           publish_time);
    // cJSON_AddItemToObject(*root,        STATUS_LIST,           statuses);
    // cJSON_AddItemToObject(publish_time, TIME_YEAR,             cJSON_CreateNumber(timeinfo.tm_year));
    // cJSON_AddItemToObject(publish_time, TIME_MONTH,            cJSON_CreateNumber(timeinfo.tm_mon));
    // cJSON_AddItemToObject(publish_time, TIME_DAY,              cJSON_CreateNumber(timeinfo.tm_mday));
    // cJSON_AddItemToObject(publish_time, TIME_HOUR,             cJSON_CreateNumber(timeinfo.tm_hour));
    // cJSON_AddItemToObject(publish_time, TIME_MIN,              cJSON_CreateNumber(timeinfo.tm_min));
    // cJSON_AddItemToObject(publish_time, TIME_SEC,              cJSON_CreateNumber(timeinfo.tm_sec));
    //
    // cJSON_AddItemToObject(statuses,     STATUS_WIFI_IP,        cJSON_CreateFalse());
    // cJSON_AddItemToObject(statuses,     STATUS_CAM,            cJSON_CreateFalse());
    // cJSON_AddItemToObject(statuses,     STATUS_SDCARD,         cJSON_CreateFalse());
    // cJSON_AddItemToObject(statuses,     STATUS_CAM_STREAM,     cJSON_CreateFalse());
    // cJSON_AddItemToObject(statuses,     STATUS_DOOR_IS_LOCKED, cJSON_CreateFalse());
    // cJSON_AddItemToObject(statuses,     STATUS_DOOR_IS_CLOSED, cJSON_CreateFalse());
    // cJSON_AddItemToObject(statuses,     STATUS_BATTERY_LEVEL,  cJSON_CreateNumber(0));
}

void stfd_set_status_json(cJSON** root, bool got_wifi_ip, bool cam_init, bool sdcard_init, bool cam_stream_init, bool door_locked, bool door_closed, int bat_level) {
    cJSON *publish_time, *statuses;
    struct tm timeinfo = obtain_time();
    if (*root != NULL)
        free(*root);
    *root = cJSON_CreateObject();
    publish_time = cJSON_CreateObject();
    statuses = cJSON_CreateObject();

    cJSON_AddItemToObject(*root,        STATUS_TIME,           publish_time);
    cJSON_AddItemToObject(*root,        STATUS_LIST,           statuses);
    cJSON_AddItemToObject(publish_time, TIME_YEAR,             cJSON_CreateNumber(timeinfo.tm_year));
    cJSON_AddItemToObject(publish_time, TIME_MONTH,            cJSON_CreateNumber(timeinfo.tm_mon));
    cJSON_AddItemToObject(publish_time, TIME_DAY,              cJSON_CreateNumber(timeinfo.tm_mday));
    cJSON_AddItemToObject(publish_time, TIME_HOUR,             cJSON_CreateNumber(timeinfo.tm_hour));
    cJSON_AddItemToObject(publish_time, TIME_MIN,              cJSON_CreateNumber(timeinfo.tm_min));
    cJSON_AddItemToObject(publish_time, TIME_SEC,              cJSON_CreateNumber(timeinfo.tm_sec));

    cJSON_AddItemToObject(statuses,     STATUS_WIFI_IP,        cJSON_CreateBool(got_wifi_ip));
    cJSON_AddItemToObject(statuses,     STATUS_CAM,            cJSON_CreateBool(cam_init));
    cJSON_AddItemToObject(statuses,     STATUS_SDCARD,         cJSON_CreateBool(sdcard_init));
    cJSON_AddItemToObject(statuses,     STATUS_CAM_STREAM,     cJSON_CreateBool(cam_stream_init));
    cJSON_AddItemToObject(statuses,     STATUS_DOOR_IS_LOCKED, cJSON_CreateBool(door_locked));
    cJSON_AddItemToObject(statuses,     STATUS_DOOR_IS_CLOSED, cJSON_CreateBool(door_closed));
    cJSON_AddItemToObject(statuses,     STATUS_BATTERY_LEVEL,  cJSON_CreateNumber(bat_level));
}

// JSON command object format
void stfd_parse_json_command(char* json_cmd, struct tm* timeinfo, mcu_cmd_type_t* cmd, int* flag, int* cmd_delay_ms) {
    char* target_device = NULL;
    char* cmd_string = NULL;
    if (json_cmd != NULL) {
        cJSON *command_root = cJSON_Parse(json_cmd);
        if (command_root != NULL) {
            cJSON* request_time = cJSON_GetObjectItem(command_root, COMMAND_TIME);

            if (request_time != NULL) {
                timeinfo->tm_mday = cJSON_GetObjectItem(request_time, TIME_DAY)->valueint;
                timeinfo->tm_mon  = cJSON_GetObjectItem(request_time, TIME_MONTH)->valueint;
                timeinfo->tm_year = cJSON_GetObjectItem(request_time, TIME_YEAR)->valueint;
                timeinfo->tm_hour = cJSON_GetObjectItem(request_time, TIME_HOUR)->valueint;
                timeinfo->tm_min  = cJSON_GetObjectItem(request_time, TIME_MIN)->valueint;
                timeinfo->tm_sec  = cJSON_GetObjectItem(request_time, TIME_SEC)->valueint;
            }
            else {
                ESP_LOGE(TAG, "Could not parse %s object of the JSON command", COMMAND_TIME);
            }
            
            asprintf(&target_device, cJSON_GetObjectItem(command_root, COMMAND_TARGET)->valuestring);
            asprintf(&cmd_string, cJSON_GetObjectItem(command_root, COMMAND_REQUEST)->valuestring);
            *flag = cJSON_GetObjectItem(command_root, COMMAND_FLAG)->valueint;
            *cmd_delay_ms = cJSON_GetObjectItem(command_root, COMMAND_DELAY)->valueint;

            *cmd = get_mqtt_cmd_type(cmd_string);

            ESP_LOGI(TAG, "Received Request to target device %s from %i/%i/%i - %i:%i:%i",
                    target_device,
                    timeinfo->tm_mday, timeinfo->tm_mon, timeinfo->tm_year,
                    timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec
                    );
            ESP_LOGI(TAG, "Received command request: %s, with flag %i, delayed by %i ms", cmd_string, *flag, *cmd_delay_ms);
            free(target_device);
            free(cmd_string);
        }
        else {
            ESP_LOGE(TAG, "Could not parse command JSON string");
        }
    }
    else {
        ESP_LOGE(TAG, "Passed command string is NULL");
    }
}

static esp_err_t mqtt_event_handler_cb(mcu_mqtt_msg_t* mcu_mqtt, esp_mqtt_event_handle_t event)
{
    //event->context;
    mcu_mqtt->client = event->client;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            /* 
                - Subscribe to command topics
                - Send current door status
            */
            stfd_mqtt_scheduled_task(mcu_mqtt);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            stfd_mqtt_close_client(mcu_mqtt);
            stfd_mqtt_init(mcu_mqtt);
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            // char* rcv_topic = malloc(event->topic_len*sizeof(char));
            // snprintf(rcv_topic, event->topic_len+1, event->topic);
            // ESP_LOGI(TAG, "rcv_topic: %s", rcv_topic);
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);

            //if (strcmp(rcv_topic, CMD_TOPIC) == 0) {
                // If another task is running
                while(mcu_mqtt->cmd_info.exec_cmd)
                    vTaskDelay(500 / portTICK_PERIOD_MS);

                stfd_parse_json_command(event->data, 
                                        &mcu_mqtt->cmd_info.timeinfo, 
                                        &mcu_mqtt->cmd_info.cmd, 
                                        &mcu_mqtt->cmd_info.flag, 
                                        &mcu_mqtt->cmd_info.cmd_delay_ms
                                        );
                vTaskDelay(mcu_mqtt->cmd_info.cmd_delay_ms/portTICK_PERIOD_MS); // Delay by required amount of time in ms
                mcu_mqtt->cmd_info.exec_cmd = true;
            // }
            // else {
            //     if (rcv_topic != NULL)
            //         free(rcv_topic);
            //     ESP_LOGE(TAG, "Unrecognized topic - Ignoring data");
            // }
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            return ESP_FAIL;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGI(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    if (mqtt_event_handler_cb(handler_args, event_data) != ESP_OK) {
        ESP_LOGE(TAG, "MQTT error occured");
    }
}

void stfd_mqtt_scheduled_task(mcu_mqtt_msg_t* mcu_mqtt) {
    int msg_id;
    if (mcu_mqtt->client != NULL) {
        // Command publishes from server should be retained also
        msg_id = esp_mqtt_client_subscribe(mcu_mqtt->client, CMD_TOPIC, 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        while (mcu_mqtt->cmd_info.exec_cmd) {
            vTaskDelay(1000 / portTICK_RATE_MS);
        }
        mcu_mqtt->cmd_info.timeinfo = obtain_time();
        mcu_mqtt->cmd_info.cmd = MCU_GETSTATUS;
        mcu_mqtt->cmd_info.cmd_delay_ms = 0;
        mcu_mqtt->cmd_info.flag = 0;
        mcu_mqtt->cmd_info.exec_cmd = true;
        //stfd_mqtt_publish_status(mcu_mqtt);
    }
    else ESP_LOGE(TAG, "Tried performing scheduled task on null client");
}

void stfd_mqtt_publish_status(mcu_mqtt_msg_t* mcu_mqtt) {
    int msg_id;
    if (mcu_mqtt->json_status != NULL) {
        char* status = cJSON_Print(mcu_mqtt->json_status);
        ESP_LOGI(TAG, "JSON Status: %s", status);
        // Status publishes should be retained if not subscribed
        if (status != NULL && mcu_mqtt->client != NULL) {
        msg_id = esp_mqtt_client_publish(mcu_mqtt->client, STATUS_TOPIC, status, 0, 1, 1);
        ESP_LOGI(TAG, "published status successfully, msg_id=%d", msg_id);
        free(status);
        }
        else if (status == NULL) ESP_LOGE(TAG, "status is NULL");
        else {
            ESP_LOGE(TAG, "Tried to publish to null client");
            free(status);
        }
    }
    else ESP_LOGW(TAG, "Tried sending a NULL JSON object");
}

void stfd_mqtt_publish_notif(esp_mqtt_client_handle_t client, const char* msg) {
    int msg_id;
    if (client != NULL) {
        msg_id = esp_mqtt_client_publish(client, NOTIFICATION_TOPIC, msg, 0, 1, 0);
        ESP_LOGI(TAG, "published notification successfully, msg_id=%d", msg_id);
    }
    else ESP_LOGW(TAG, "Tried to publish with null client - Initialization not complete");
}

void stfd_mqtt_close_client(mcu_mqtt_msg_t* mcu_mqtt) {
    int msg_id;
    if (mcu_mqtt->client != NULL) {
        msg_id = esp_mqtt_client_unsubscribe(mcu_mqtt->client, CMD_TOPIC);
        ESP_LOGI(TAG, "unsubscribed successfully, msg_id=%d", msg_id);
        if (esp_mqtt_client_disconnect(mcu_mqtt->client) != ESP_OK)
            ESP_LOGE(TAG, "Could not disconnect the MQTT client properly");
        if (esp_mqtt_client_destroy(mcu_mqtt->client) != ESP_OK)
            ESP_LOGE(TAG, "Could not destry the MQTT client properly");
    }
    else
        ESP_LOGE(TAG, "Could not destry the MQTT client properly");
}

void stfd_mqtt_init(mcu_mqtt_msg_t* mcu_mqtt) {
    if (!mcu_mqtt->sntp_init) {
        initialize_sntp();
        mcu_mqtt->sntp_init = true;
    }
    stfd_init_status_json(&mcu_mqtt->json_status);
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = MQTT_BROKER_URL,
    };
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, mcu_mqtt);
    esp_mqtt_client_start(client);
}
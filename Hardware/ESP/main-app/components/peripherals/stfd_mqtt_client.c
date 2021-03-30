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

#define MQTT_BROKER_URL     "mqtt://localhost:1883"
#define NOTIFICATION_TOPIC  "/topic/device_notif"
#define STATUS_TOPIC        "/topic/device_status"
#define CMD_TOPIC           "/topic/device_cmd"

#define MCU_SHUTDOWN_STR    "mcu shutdown"
#define MCU_GETSTATUS_STR   "get status"
#define LOCK_DOOR_STR       "lock door"
#define UNLOCK_DOOR_STR     "unlock door"
#define STREAM_CAM_STR      "stream camera"

static const char *TAG = "stfd_mqtt_client";

static void initialize_sntp(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "time.google.com");
    sntp_init();
} 

static struct tm obtain_time(void)
{
    initialize_sntp();
    // wait for time to be set
    time_t now = 0;
    struct tm timeinfo = {0};
    while (timeinfo.tm_year < (2021 - 1900)) {
        ESP_LOGI(TAG, "Waiting for system time to be set...");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
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

static mcu_cmd_type_t get_mqtt_cmd_type(char* cmd) {
    if (!strcmp(cmd, MCU_SHUTDOWN_STR))
        return (mcu_cmd_type_t) MCU_SHUTDOWN;
    else if (!strcmp(cmd, MCU_GETSTATUS_STR))
        return (mcu_cmd_type_t) MCU_GETSTATUS;
    else if (!strcmp(cmd, LOCK_DOOR_STR))
        return (mcu_cmd_type_t) LOCK_DOOR;
    else if (!strcmp(cmd, UNLOCK_DOOR_STR))
        return (mcu_cmd_type_t) UNLOCK_DOOR;
    else if (!strcmp(cmd, STREAM_CAM_STR))
        return (mcu_cmd_type_t) STREAM_CAM;
    else
        return (mcu_cmd_type_t) MCU_INVALID;
}

// JSON status object format
cJSON stfd_init_status_json(void) {
    cJSON *root, *status_time, *statuses;
    root = cJSON_CreateObject();
    status_time = cJSON_CreateObject();
    statuses = cJSON_CreateObject();

    cJSON_AddItemToObject(root,         "status_time",      status_time);
    cJSON_AddItemToObject(status_time,  "time_ms",          cJSON_CreateString(obtain_time_string()));
    cJSON_AddItemToObject(statuses,     "got_wifi_ip",      cJSON_CreateFalse());
    cJSON_AddItemToObject(statuses,     "cam_init",         cJSON_CreateFalse());
    cJSON_AddItemToObject(statuses,     "sdcard_init",      cJSON_CreateFalse());
    cJSON_AddItemToObject(statuses,     "cam_server_init",  cJSON_CreateFalse());
    cJSON_AddItemToObject(statuses,     "door_is_locked",   cJSON_CreateFalse());
    cJSON_AddItemToObject(statuses,     "door_is_closed",   cJSON_CreateFalse());
    cJSON_AddItemToObject(statuses,     "battery_level",    cJSON_CreateNumber(0));

    return *root;
}

// JSON command object format
void stfd_parse_json_command(char* json_cmd, struct tm* timeinfo, mcu_cmd_type_t* cmd, int* flag, int* cmd_delay_ms) {
    char* cmd_string = NULL;
    cJSON *command_root = cJSON_Parse(json_cmd);
    cJSON* request_time = cJSON_GetObjectItem(command_root, COMMAND_TIME);

    timeinfo->tm_mday = cJSON_GetObjectItem(request_time, COMMAND_TIME_DAY)->valueint;
    timeinfo->tm_mon  = cJSON_GetObjectItem(request_time, COMMAND_TIME_MONTH)->valueint;
    timeinfo->tm_year = cJSON_GetObjectItem(request_time, COMMAND_TIME_YEAR)->valueint;
    timeinfo->tm_hour = cJSON_GetObjectItem(request_time, COMMAND_TIME_HOUR)->valueint;
    timeinfo->tm_min  = cJSON_GetObjectItem(request_time, COMMAND_TIME_MIN)->valueint;
    timeinfo->tm_sec  = cJSON_GetObjectItem(request_time, COMMAND_TIME_SEC)->valueint;

    asprintf(&cmd_string, cJSON_GetObjectItem(command_root, COMMAND_REQUEST)->valuestring);
    *flag = cJSON_GetObjectItem(command_root, COMMAND_FLAG)->valueint;
    *cmd_delay_ms = cJSON_GetObjectItem(command_root, COMMAND_DELAY)->valueint;

    *cmd = get_mqtt_cmd_type(cmd_string);

    ESP_LOGI(TAG, "Received Request from %i/%i/%i - %i:%i:%i",
            timeinfo->tm_mday, timeinfo->tm_mon, timeinfo->tm_year,
            timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec
            );
    ESP_LOGI(TAG, "Received command request: %s, with flag %i, delayed by %i", cmd_string, *flag, *cmd_delay_ms);
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
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            // msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
            // ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);

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
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGI(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(handler_args, event_data);
}

void stfd_mqtt_scheduled_task(mcu_mqtt_msg_t* mcu_mqtt) {
    int msg_id;
    msg_id = esp_mqtt_client_publish(mcu_mqtt->client, STATUS_TOPIC, cJSON_Print(mcu_mqtt->json_status), 0, 1, 0);
    ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

    msg_id = esp_mqtt_client_subscribe(mcu_mqtt->client, CMD_TOPIC, 0);
    ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

    // msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
    // ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
}

void stfd_mqtt_notif_task(esp_mqtt_client_handle_t client, char* msg) {
    int msg_id;
    msg_id = esp_mqtt_client_publish(client, NOTIFICATION_TOPIC, msg, 0, 1, 0);
    ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
}

void stfd_mqtt_init(mcu_mqtt_msg_t* mcu_mqtt) {
    *(mcu_mqtt->json_status) = stfd_init_status_json();
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = MQTT_BROKER_URL,
    };
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, mcu_mqtt);
    esp_mqtt_client_start(client);
}
/*
    Project:    ShutTheFrontDoor
    File:       scan.c
    Author:     Benjamin Lanthier
    Date:       18/02/2021

    Desc:       File describing the top-level Wifi configurations and modes of Access Point (AP) detection.
                Two main methods of scanning are programmed in ESP32: Fast Scan and All Channel Scan. The
                Fast Scan connects the device to the first AP that meets the requirements (such as SSID and
                Security level), while the All Channel Scan collects all the available channels and returns
                the list as an object.
*/
#include <string.h>
#include "nvs_flash.h"
#include "esp_netif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "stfd_peripherals.h"
#include "stfd_comms.h"

/* Set the SSID and Password via project configuration, or can set directly here */
#define DEFAULT_SCAN_LIST_SIZE CONFIG_SCAN_LIST_SIZE

#define DEFAULT_SSID CONFIG_WIFI_SSID
#define DEFAULT_PWD CONFIG_WIFI_PASSWORD

#if CONFIG_WIFI_ALL_CHANNEL_SCAN_CONNECT
#define DEFAULT_SCAN_METHOD WIFI_ALL_CHANNEL_SCAN
#define AUTO_CONNECT 1
#elif CONFIG_WIFI_FAST_SCAN
#define DEFAULT_SCAN_METHOD WIFI_FAST_SCAN
#define AUTO_CONNECT 1
#elif CONFIG_WIFI_ALL_CHANNEL_SCAN_SHOW
#define DEFAULT_SCAN_METHOD WIFI_ALL_CHANNEL_SCAN
#define AUTO_CONNECT 0
#else
#define DEFAULT_SCAN_METHOD WIFI_FAST_SCAN
#endif /*CONFIG_SCAN_METHOD*/

#if CONFIG_WIFI_CONNECT_AP_BY_SIGNAL
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SIGNAL
#elif CONFIG_WIFI_CONNECT_AP_BY_SECURITY
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SECURITY
#else
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SIGNAL
#endif /*CONFIG_SORT_METHOD*/

#if CONFIG_FAST_SCAN_THRESHOLD
#define DEFAULT_RSSI CONFIG_FAST_SCAN_MINIMUM_SIGNAL
#if CONFIG_FAST_SCAN_WEAKEST_AUTHMODE_OPEN
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#elif CONFIG_FAST_SCAN_WEAKEST_AUTHMODE_WEP
#define DEFAULT_AUTHMODE WIFI_AUTH_WEP
#elif CONFIG_FAST_SCAN_WEAKEST_AUTHMODE_WPA
#define DEFAULT_AUTHMODE WIFI_AUTH_WPA_PSK
#elif CONFIG_FAST_SCAN_WEAKEST_AUTHMODE_WPA2
#define DEFAULT_AUTHMODE WIFI_AUTH_WPA2_PSK
#else
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#endif
#else
#define DEFAULT_RSSI -127
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#endif /*CONFIG_FAST_SCAN_THRESHOLD*/

#define WIFI_RETRY_LIMIT CONFIG_WIFI_RETRY_LIMIT

static const char *TAG = "stfd_wifi_scan";
#define IP_ADDR_BUF_LEN 15
static char esp_ip_addr[IP_ADDR_BUF_LEN];
static char esp_public_ip_addr[IP_ADDR_BUF_LEN];

static EventGroupHandle_t wifi_event_group;
const static int CONNECTED_BIT = BIT0;
                
uint32_t getDefaultScanListSize(void) {
    return DEFAULT_SCAN_LIST_SIZE;
}

wifi_scan_method_t getDefaultScanMethod(void) {
    return DEFAULT_SCAN_METHOD;
}

void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    uint16_t retry = 1;
    esp_err_t err = ESP_OK;
    
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        do {
            ESP_LOGI(TAG, "Connection attempt: %i", retry);
            err = esp_wifi_connect();
        } while (retry++ < WIFI_RETRY_LIMIT && err != ESP_OK);

        if (retry >= WIFI_RETRY_LIMIT && err != ESP_OK)
            ESP_LOGW(TAG, "Went over retry limit (%i)", retry);
            if (err == ESP_ERR_WIFI_SSID)
                ESP_LOGW(TAG, "Wrong SSID given.");
            else if (err == ESP_ERR_WIFI_CONN)
                ESP_LOGW(TAG, "Internal Error");

    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        do {
            ESP_LOGI(TAG, "Connection attempt: %i", retry);
            err = esp_wifi_connect();
        } while (retry++ < WIFI_RETRY_LIMIT && err != ESP_OK);

        if (retry >= WIFI_RETRY_LIMIT && err != ESP_OK)
            ESP_LOGW(TAG, "Went over retry limit (%i)", retry);
            if (err == ESP_ERR_WIFI_SSID)
                ESP_LOGW(TAG, "Wrong SSID given.");
            else if (err == ESP_ERR_WIFI_CONN)
                ESP_LOGW(TAG, "Internal Error");
        else
            xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);

    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        // Get Local IP
        esp_ip4addr_ntoa(&event->ip_info.ip, esp_ip_addr, IP_ADDR_BUF_LEN);
        // Get Public IP
        http_rest_with_url_get_device_ip(esp_public_ip_addr);
        ESP_LOGI(TAG, "got local ip:" IPSTR, IP2STR(&event->ip_info.ip));
        ESP_LOGI(TAG, "got public ip: %s", esp_public_ip_addr);
    }
}

void print_auth_mode(int authmode) {
    switch (authmode) {
    case WIFI_AUTH_OPEN:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_OPEN");
        break;
    case WIFI_AUTH_WEP:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WEP");
        break;
    case WIFI_AUTH_WPA_PSK:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA_PSK");
        break;
    case WIFI_AUTH_WPA2_PSK:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA2_PSK");
        break;
    case WIFI_AUTH_WPA_WPA2_PSK:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA_WPA2_PSK");
        break;
    case WIFI_AUTH_WPA2_ENTERPRISE:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA2_ENTERPRISE");
        break;
    case WIFI_AUTH_WPA3_PSK:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA3_PSK");
        break;
    case WIFI_AUTH_WPA2_WPA3_PSK:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA2_WPA3_PSK");
        break;
    default:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_UNKNOWN");
        break;
    }
}

void print_cipher_type(int pairwise_cipher, int group_cipher) {
    switch (pairwise_cipher) {
    case WIFI_CIPHER_TYPE_NONE:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_NONE");
        break;
    case WIFI_CIPHER_TYPE_WEP40:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_WEP40");
        break;
    case WIFI_CIPHER_TYPE_WEP104:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_WEP104");
        break;
    case WIFI_CIPHER_TYPE_TKIP:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_TKIP");
        break;
    case WIFI_CIPHER_TYPE_CCMP:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_CCMP");
        break;
    case WIFI_CIPHER_TYPE_TKIP_CCMP:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_TKIP_CCMP");
        break;
    default:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_UNKNOWN");
        break;
    }

    switch (group_cipher) {
    case WIFI_CIPHER_TYPE_NONE:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_NONE");
        break;
    case WIFI_CIPHER_TYPE_WEP40:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_WEP40");
        break;
    case WIFI_CIPHER_TYPE_WEP104:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_WEP104");
        break;
    case WIFI_CIPHER_TYPE_TKIP:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_TKIP");
        break;
    case WIFI_CIPHER_TYPE_CCMP:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_CCMP");
        break;
    case WIFI_CIPHER_TYPE_TKIP_CCMP:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_TKIP_CCMP");
        break;
    default:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_UNKNOWN");
        break;
    }
}

/* Initialize Wi-Fi as sta and set scan method. Return wifi list*/
void wifi_all_ch_scan(wifi_ap_record_t* pv_ap_info) {

    if (pv_ap_info != NULL) {
        free(pv_ap_info);
        pv_ap_info = NULL;
    }

    uint16_t number = DEFAULT_SCAN_LIST_SIZE;
    wifi_ap_record_t ap_info[DEFAULT_SCAN_LIST_SIZE];
    uint16_t ap_count = 0;
    memset(ap_info, 0, sizeof(ap_info));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_scan_start(NULL, true));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
    ESP_LOGI(TAG, "Total APs scanned = %u", ap_count);
    for (int i = 0; (i < DEFAULT_SCAN_LIST_SIZE) && (i < ap_count); i++) {
        // Log Info
        ESP_LOGI(TAG, "SSID \t\t%s", ap_info[i].ssid);
        ESP_LOGI(TAG, "RSSI \t\t%d", ap_info[i].rssi);
        print_auth_mode(ap_info[i].authmode);
        if (ap_info[i].authmode != WIFI_AUTH_WEP) {
            print_cipher_type(ap_info[i].pairwise_cipher, ap_info[i].group_cipher);
        }
        ESP_LOGI(TAG, "Channel \t\t%d\n", ap_info[i].primary);
    }
    pv_ap_info = ap_info;
}

/* Initialize Wi-Fi as sta and set scan method 
   The ESP connects to the AP with matching SSID and Password */
void fast_scan(wifi_ap_record_t* ap_info) {

    if (ap_info != NULL) {
        free(ap_info);
        ap_info = NULL;
    }
    ap_info = malloc(sizeof(wifi_ap_record_t));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, NULL));

    // Initialize and start WiFi
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = DEFAULT_SSID,
            .password = DEFAULT_PWD,
            .scan_method = DEFAULT_SCAN_METHOD,
            .sort_method = DEFAULT_SORT_METHOD,
            .threshold.rssi = DEFAULT_RSSI,
            .threshold.authmode = DEFAULT_AUTHMODE,
        },
    };
    memcpy(ap_info->ssid, DEFAULT_SSID, strlen(DEFAULT_SSID));
    ap_info->rssi = DEFAULT_RSSI;
    ap_info->authmode = DEFAULT_AUTHMODE;

    // Log output
    ESP_LOGI(TAG, "Fast Scan - Connected to SSID \t\t%s", DEFAULT_SSID);

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

void wifi_scan(mcu_content_t* mcu_c) {

    if (mcu_c->ap_info != NULL) {
        free(mcu_c->ap_info);
        mcu_c->ap_info = NULL;
    }

    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );
    ESP_ERROR_CHECK(esp_netif_init());
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Initialize default station as network interface instance (esp-netif)
    mcu_c->netif = esp_netif_create_default_wifi_sta();
    assert(mcu_c->netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    if (getDefaultScanMethod() == WIFI_FAST_SCAN) {
        ESP_LOGI(TAG, "Begin Fast Scan");
        fast_scan(mcu_c->ap_info);
        gpio_blink(2);
    }
    else { //WIFI_ALL_CHANNEL_SCAN:
        ESP_LOGI(TAG, "Begin All Channel Scan");
        wifi_all_ch_scan(mcu_c->ap_info);
        gpio_blink(3);
        // TODO: Send list to BlueTooth connected user
        // Set the default wifi to the returned selection from the user
    }

    ESP_LOGI(TAG, "Waiting for wifi");
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);

    // Pass device IP
    memcpy(mcu_c->device_ip, esp_ip_addr, IP_ADDR_BUF_LEN);
    memcpy(mcu_c->pub_device_ip, esp_public_ip_addr, IP_ADDR_BUF_LEN);
}
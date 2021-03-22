/* 
    Project:    ShutTheFrontDoor
    File:       stfd_opencv.cpp
    Author:     Benjamin Lanthier
    Date:       22/02/2021

    Desc:       TCP client used for converting MJPG to packages for ZMQ
*/

// #include <vector>
// #include <opencv2/core/mat.hpp>
// #include <opencv2/imgcodecs.hpp>

#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

// #if defined(CONFIG_IPV4)
// #define HOST_IP_ADDR CONFIG_IPV4_ADDR
// #elif defined(CONFIG_IPV6)
// #define HOST_IP_ADDR CONFIG_IPV6_ADDR
// #else
#define HOST_IP_ADDR "255.255.255.255"
// #endif

#define PORT 80//CONFIG_TCP_HOST_PORT

static const char* TAG = "stfd_udp_client";

//esp_err_t udp_setup_sock(int* sock, void* addr, esp_netif_t* esp_netif) {
esp_err_t udp_setup_sock(int* sock, struct sockaddr_in* addr, esp_netif_t* esp_netif) {
    //char rx_buffer[128];
    char host_ip[] = HOST_IP_ADDR;
    int addr_family = 0;
    int ip_protocol = 0;

#if defined(CONFIG_IPV4)
    struct sockaddr_in* dest_addr = addr;
    dest_addr->sin_addr.s_addr = inet_addr(host_ip);
    dest_addr->sin_family = AF_INET;
    dest_addr->sin_port = htons(PORT);
    addr_family = AF_INET;
    ip_protocol = IPPROTO_IP;
    char* esp_ip_addr;
    esp_ip_addr = inet_ntoa(dest_addr->sin_addr.s_addr);
    ESP_LOGW(TAG, "Host IP: %s", esp_ip_addr);
    ESP_LOGW(TAG, "Host Port: %i", dest_addr->sin_port);

#elif defined(CONFIG_IPV6)
    struct sockaddr_in6* dest_addr = addr;
    inet6_aton(host_ip, &dest_addr->sin6_addr);
    dest_addr->sin6_family = AF_INET6;
    dest_addr->sin6_port = htons(PORT);
    dest_addr->sin6_scope_id = esp_netif_get_netif_impl_index(esp_netif);
    addr_family = AF_INET6;
    ip_protocol = IPPROTO_IPV6;
#endif

    *sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
    if (*sock < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Socket created, connecting to %s:%d", host_ip, PORT);
    return ESP_OK;
}

esp_err_t udp_send_buf (int* sock, struct sockaddr* dest_addr, uint8_t* jpg_buf, size_t jpg_buf_len) {
    int err = sendto(*sock, jpg_buf, jpg_buf_len, 0, dest_addr, sizeof(dest_addr));
    if (err != 0) {
        ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Message sent");
    return ESP_OK;
        // int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
        // // Error occurred during receiving
        // if (len < 0) {
        //     ESP_LOGE(TAG, "recv failed: errno %d", errno);
        //     return ESP_FAIL;
        // }
        // // Data received
        // else {
        //     rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
        //     ESP_LOGI(TAG, "Received %d bytes from %s:", len, host_ip);
        //     ESP_LOGI(TAG, "%s", rx_buffer);
        // }
}
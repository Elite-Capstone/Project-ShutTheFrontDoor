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

#if defined(CONFIG_SERVER_IPV4)
#define HOST_IP_ADDR CONFIG_IPV4_ADDR
#elif defined(CONFIG_SERVER_IPV6)
#define HOST_IP_ADDR CONFIG_IPV6_ADDR
#else
#define HOST_IP_ADDR "34.95.11.51"
#endif

#define PORT CONFIG_UDP_HOST_PORT

#define SHUTDOWN_MSG "Shutting down socket..."

static const char* TAG = "stfd_udp_client";

esp_err_t udp_setup_sock(int* sock, struct sockaddr* addr, esp_netif_t* esp_netif) {
    //char rx_buffer[128];
    char host_ip[] = HOST_IP_ADDR;
    int addr_family = 0;
    int ip_protocol = 0;

    // Cast the pointer to a configured socket structure to be modified accordingly
#if defined(CONFIG_SERVER_IPV4)
    struct sockaddr_in* dest_addr = (struct sockaddr_in*) addr;
    dest_addr->sin_addr.s_addr = inet_addr(HOST_IP_ADDR);
    dest_addr->sin_family = AF_INET;
    dest_addr->sin_port = htons(PORT);
    addr_family = AF_INET;
    ip_protocol = IPPROTO_IP;
    char* esp_ip_addr;
    esp_ip_addr = inet_ntoa(dest_addr->sin_addr.s_addr);
    ESP_LOGW(TAG, "Host IP: %s", esp_ip_addr);
    ESP_LOGW(TAG, "Host Port: %i", dest_addr->sin_port);

#elif defined(CONFIG_SERVER_IPV6)
    struct sockaddr_in6* dest_addr = (struct sockaddr_in6*) addr;
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

esp_err_t udp_send_buf (int* sock, struct sockaddr* dest_addr, size_t dest_addr_size, void* buf, size_t buf_len) {
    int err = sendto(*sock, buf, buf_len, 0, dest_addr, dest_addr_size);
    if (err < 0) {
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

esp_err_t udp_shutdown (int* sock, struct sockaddr* dest_addr, size_t dest_addr_size) {
    int err = sendto(*sock, SHUTDOWN_MSG, strlen(SHUTDOWN_MSG), 0, dest_addr, dest_addr_size);
    if (err < 0) {
        ESP_LOGE(TAG, "Error occurred during sending shutdown message: errno %d", errno);
        return ESP_FAIL;
    }
    else {
        ESP_LOGI(TAG, "Shutdown Message sent");
    }
    ESP_LOGW(TAG, "%s", SHUTDOWN_MSG);
    shutdown(sock, 0);
    close(sock);
    return ESP_OK;
}
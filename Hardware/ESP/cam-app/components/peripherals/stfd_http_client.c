/* 
    Project:    ShutTheFrontDoor
    File:       stfd_http_client.c
    Author:     Olivier B. Vachet
    Date:       22/02/2021

    Desc:       HTTP client used for communicating with the system backend server
*/

#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_timer.h"
#include "esp_tls.h"

#include "stfd_peripherals.h"
#include "stfd_comms.h"

#define MAX_HTTP_RECV_BUFFER   512
#define MAX_HTTP_OUTPUT_BUFFER 2048
#define DEFAULT_STREAM_PORT CONFIG_LOCAL_HTTP_STREAM_PORT

#define PART_BOUNDARY "123456789000000000000987654321"
static const char* STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* PIC_CONTENT_TYPE = "multipart/mixed;boundary=" PART_BOUNDARY;
static const char* MEDIA_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* MEDIA_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

static const char *TAG = "stfd_http_client";

esp_err_t _http_event_handler(esp_http_client_event_t *evt) {
    static char *output_buffer;  // Buffer to store response of http request from event handler
    static int output_len;       // Stores number of bytes read
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            /*
             *  Check for chunked encoding is added as the URL for chunked encoding used in this example returns binary data.
             *  However, event handler can also be used in case chunked encoding is used.
             */
            if (!esp_http_client_is_chunked_response(evt->client)) {
                // If user_data buffer is configured, copy the response into the buffer
                if (evt->user_data) {
                    memcpy(evt->user_data + output_len, evt->data, evt->data_len);
                } else {
                    if (output_buffer == NULL) {
                        output_buffer = (char *) malloc(esp_http_client_get_content_length(evt->client));
                        output_len = 0;
                        if (output_buffer == NULL) {
                            ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
                            return ESP_FAIL;
                        }
                    }
                    memcpy(output_buffer + output_len, evt->data, evt->data_len);
                }
                output_len += evt->data_len;    
            }

            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
            if (output_buffer != NULL) {
                // Response is accumulated in output_buffer. Uncomment the below line to print the accumulated response
                // ESP_LOG_BUFFER_HEX(TAG, output_buffer, output_len);
                free(output_buffer);
                output_buffer = NULL;
                output_len = 0;
            }
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            int mbedtls_err = 0;
            esp_err_t err = esp_tls_get_and_clear_last_error(evt->data, &mbedtls_err, NULL);
            if (err != 0) {
                if (output_buffer != NULL) {
                    free(output_buffer);
                    output_buffer = NULL;
                    output_len = 0;
                }
                ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
                ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
            }
            break;
    }
    return ESP_OK;
}

//Get Public IP Address
void http_rest_with_url_get_device_ip(char* addr) {
    char local_response_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};
    uint32_t local_buf_len = 0;
    esp_err_t err = ESP_OK;
    
    esp_http_client_config_t config = {
        .url = DEFAULT_HTTP_URL,
        .path = "/device",
        .query = "esp",
        .event_handler = _http_event_handler,
        .user_data = local_response_buffer,    // Pass address of local buffer to get response
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    //GET
    esp_http_client_set_url(client, DEFAULT_HTTP_URL "device/" DEFAULT_DOOR_UUID);
    err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        local_buf_len = esp_http_client_get_content_length(client);
        ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %d",
                 esp_http_client_get_status_code(client),
                 local_buf_len);
    } else {
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
    }
    //esp_http_client_read(client, buf, len);
    //esp_http_client_read_response(client,buf,len);
    ESP_LOGW(TAG, "Device IP address: %s", local_response_buffer);
    memcpy(addr, local_response_buffer, local_buf_len*sizeof(char));
    esp_http_client_cleanup(client);
}

//Change file calling method to add 
void http_rest_with_url_upload_picture(uint8_t* buf, size_t len) {
    char local_response_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};
    esp_err_t err = ESP_OK;
    /**
     * NOTE: All the configuration parameters for http_client must be spefied either in URL or as host and path parameters.
     * If host and path parameters are not set, query parameter will be ignored. In such cases,
     * query parameter should be specified in URL.
     *
     * If URL as well as host and path parameters are specified, values of host and path will be considered.
     * NOTE: 
     *      - url: "http://34.117.160.50/"
     *      - path: name of the path in the API endpoint
     */
    esp_http_client_config_t config = {
        .url = DEFAULT_HTTP_URL,
        .path = "/file",
        .query = "esp",
        .event_handler = _http_event_handler,
        .user_data = local_response_buffer,        // Pass address of local buffer to get response
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    // POST
    /**
     * NOTE: *post_data is the data that will be sent in the body
     *      - set the complete url as shown the API endpoint
     *      - modify the Content-Type depending of the method
     */

    ESP_LOGI(TAG, DEFAULT_HTTP_URL "file/upload");
    esp_http_client_set_url(client, DEFAULT_HTTP_URL "file/upload");
    esp_http_client_set_method(client, HTTP_METHOD_POST);
    // Uploading strictly JPEG
    esp_http_client_set_header(client, "Content-Type", "image/jpeg");
    esp_http_client_set_header(client, "Content-Disposition", "inline; filename=capture.jpg");
    
    // buf contains pixel data and len is the length of the data
    esp_http_client_set_post_field(client, (const char*)buf, len);

    err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP POST Status = %d, content_length = %d",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "HTTP POST request failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
}

void http_rest_with_url_notification(const char* message) {
    char local_response_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};
    esp_err_t err = ESP_OK;
    /**
     * NOTE: All the configuration parameters for http_client must be spefied either in URL or as host and path parameters.
     * If host and path parameters are not set, query parameter will be ignored. In such cases,
     * query parameter should be specified in URL.
     *
     * If URL as well as host and path parameters are specified, values of host and path will be considered.
     * NOTE: 
     *      - url: "http://34.117.160.50/"
     *      - path: name of the path in the API endpoint
     */
    esp_http_client_config_t config = {
        .url = DEFAULT_HTTP_URL,
        .path = "/notif",
        .query = "esp",
        .event_handler = _http_event_handler,
        .user_data = local_response_buffer,        // Pass address of local buffer to get response
    };
     esp_http_client_handle_t client = esp_http_client_init(&config);

    //PUT
    char* data_format = "{\"notification\":\"%s\",\n\"doorId\":\"%s\"}";
    char* post_data = malloc(strlen(message) + strlen(DEFAULT_DOOR_UUID) + strlen(data_format));
    sprintf(post_data, data_format, message, DEFAULT_DOOR_UUID);
    esp_http_client_set_url(client, DEFAULT_HTTP_URL "notif/newNotif/");
    esp_http_client_set_method(client, HTTP_METHOD_PUT);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP PUT Status = %d, content_length = %d",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "HTTP PUT request failed: %s", esp_err_to_name(err));
    }

    free(post_data);

    //GET
    // esp_http_client_set_url(client, DEFAULT_HTTP_URL "notif/newTopic/DEFAULT_DOOR_UUID");
    // esp_err_t err = esp_http_client_perform(client);
    // if (err == ESP_OK) {
    //      ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %d",
    //              esp_http_client_get_status_code(client),
    //              esp_http_client_get_content_length(client));
    // } else {
    //     ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
    // }

    ESP_LOG_BUFFER_HEX(TAG, local_response_buffer, strlen(local_response_buffer));
    esp_http_client_cleanup(client);
}

// static esp_err_t http_send_frame_chunk(uint8_t* buf, size_t len, char* ) {
//     char * part_buf[64];
//     if(res == ESP_OK){
//       size_t hlen = snprintf((char *)part_buf, 64, MEDIA_PART, _jpg_buf_len);
//       res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
//     }
//     if(res == ESP_OK){
//       res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
//     }
//     if(res == ESP_OK){
//       res = httpd_resp_send_chunk(req, MEDIA_BOUNDARY, strlen(MEDIA_BOUNDARY));
//     }
// }

//=======================================
//===== Section for video streaming =====
//=======================================

esp_err_t stream_handler(httpd_req_t *req) {
    camera_fb_t * fb = NULL;
    esp_err_t res = ESP_OK;
    size_t _jpg_buf_len = 0;
    uint8_t * _jpg_buf = NULL;
    char * part_buf[64];
    static int64_t last_frame = 0;
    if(!last_frame) {
        last_frame = esp_timer_get_time();
    }

    res = httpd_resp_set_type(req, STREAM_CONTENT_TYPE);
    if(res != ESP_OK){
        return res;
    }

    while(true){
        fb = esp_camera_fb_get();
        if (!fb) {
            ESP_LOGE(TAG, "Camera capture failed");
            res = ESP_FAIL;
            break;
        }
        if (convert_to_jpeg(fb, &_jpg_buf, &_jpg_buf_len) != ESP_OK)
            break;

        if(res == ESP_OK){
            res = httpd_resp_send_chunk(req, MEDIA_BOUNDARY, strlen(MEDIA_BOUNDARY));
        }
        if(res == ESP_OK){
            size_t hlen = snprintf((char *)part_buf, 64, MEDIA_PART, _jpg_buf_len);

            res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
        }
        if(res == ESP_OK){
            res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
        }
        if(fb->format != PIXFORMAT_JPEG){
            free(_jpg_buf);
        }
        esp_camera_fb_return(fb);
        if(res != ESP_OK){
            break;
        }
        int64_t fr_end = esp_timer_get_time();
        int64_t frame_time = fr_end - last_frame;
        last_frame = fr_end;
        frame_time /= 1000;
        ESP_LOGI(TAG, "MJPG: %uKB %ums (%.1ffps)",
            (uint32_t)(_jpg_buf_len/1024),
            (uint32_t)frame_time, 1000.0 / (uint32_t)frame_time);
    }

    last_frame = 0;
    return res;
}

httpd_handle_t startStreamServer(char* device_ip) {
    httpd_handle_t httpd_handle = NULL;

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = DEFAULT_STREAM_PORT;
    config.recv_wait_timeout = 30;
    config.send_wait_timeout = 30;

    httpd_uri_t index_uri = {
    .uri       = DEFAULT_STREAM_URI,
    .method    = HTTP_GET,
    .handler   = stream_handler,
    .user_ctx  = NULL
    };

    ESP_LOGI(TAG,"Starting web server on port: '%d'\n", config.server_port);
    ESP_LOGI(TAG, "Connect to http://%s%s", device_ip, index_uri.uri);
    if (httpd_start(&httpd_handle, &config) == ESP_OK) {
    httpd_register_uri_handler(httpd_handle, &index_uri);
    }
    return httpd_handle;
}

void stopStreamServer(httpd_handle_t* httpd_handle) {
    ESP_LOGI(TAG, "Trying to stop stream");
    if (httpd_stop(httpd_handle) == ESP_OK)
        ESP_LOGI(TAG,"Stopping web server");
    else
        ESP_LOGE(TAG, "Could not successfully stop the camera server on port %d", DEFAULT_STREAM_PORT);
}

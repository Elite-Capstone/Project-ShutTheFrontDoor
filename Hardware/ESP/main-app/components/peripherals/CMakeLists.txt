idf_component_register(
    SRCS "stfd_camera.c" "stfd_gpio.c" "stfd_http_client.c" "stfd_wifi_scan.c" "http_server.c"
    INCLUDE_DIRS "include"
    REQUIRES log esp_system esp_event esp_wifi esp_http_server esp_http_client esp-tls esp_timer nvs_flash fatfs soc esp32-camera
    EMBED_TXTFILES howsmyssl_com_root_cert.pem
)
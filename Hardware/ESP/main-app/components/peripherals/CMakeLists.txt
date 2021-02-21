idf_component_register(
    SRCS "stfd_camera.c" "stfd_gpio.c" "stfd_wifi_scan.c"
    INCLUDE_DIRS "."
    REQUIRES log esp_system esp_event esp_wifi esp_http_server nvs_flash fatfs soc esp32-camera
)
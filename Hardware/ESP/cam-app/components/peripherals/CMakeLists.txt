idf_component_register(
    SRCS "stfd_gcp_client.c" "stfd_udp_client.c" "stfd_camera.c" "stfd_gpio.c" "stfd_http_client.c" "stfd_wifi_scan.c" "stfd_sdcard.c" "http_server.c"
    INCLUDE_DIRS "include"
    REQUIRES log json esp_system esp_event esp_wifi esp_http_server esp_http_client esp-tls esp_timer nvs_flash fatfs soc jsmn esp32-camera esp-google-iot
    EMBED_TXTFILES howsmyssl_com_root_cert.pem
)
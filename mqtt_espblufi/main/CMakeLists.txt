string(REPLACE "\\" "/" FIXED_IDF_PATH "$ENV{IDF_PATH}")

idf_component_register(SRCS "cie_esp_OTA.c" "cie_OTA.c" "cie_sntp.c" "cie_connect.c" "cie_main.c" "cie_uart.c" "cie_mqtt.c" "cie_state.c" "cie_tool.c" "cie_scan.c" 
"cie_mqtt_handle.c" "cie_wifi.c"  "cie_connect.c"  "cie_sntp.c"  "cie_pwm.c"   "blufi_example_main.c" "blufi_security.c"
 "blufi_init.c" "cie_OTA.c" "cie_esp_OTA.c"
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES  bt mqtt nvs_flash esp_netif esp_driver_uart esp_driver_gpio json esp_wifi esp_driver_ledc
                       wpa_supplicant esp_http_client app_update esp_https_ota mbedtls
 PRIV_INCLUDE_DIRS "${IDF_PATH}/components/mbedtls/esp_crt_bundle/include"


                       
                       )
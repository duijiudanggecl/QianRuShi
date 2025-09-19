#ifndef CIE_WIFI_H_
#define CIE_WIFI_H_
#include "esp_err.h"

#define CIE_CLEAR_SSID_PW   1//0 
#define WIFI_MANAGE         0//弃用

esp_err_t cie_wifi_connect(void);

#define CIE_WIFI_SSID           "413"//"CIE-2"
#define CIE_WIFI_PASSWORD       "15817481011"
#define CIE_WIFI_CONN_MAX_RETRY     6
#endif

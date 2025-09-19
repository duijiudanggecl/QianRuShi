#ifndef CIE_CONNECT_H_
#define CIE_CONNECT_H_

#include "esp_err.h"

esp_err_t cie_connect(void);
esp_err_t cie_disconnect(void);
esp_err_t cie_set_static_ip(void);
#endif
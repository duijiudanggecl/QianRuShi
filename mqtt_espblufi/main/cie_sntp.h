#ifndef CIE_SNTP_H_
#define CIE_SNTP_H_

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
//#include "esp_sntp.h"
#include "lwip/apps/sntp.h"
#include "cie_state.h"
#include "cie_uart.h"


// 定义北京时间的时区偏移量（UTC+8）
#define BEIJING_TIMEZONE_OFFSET (8 * 3600)  // 8小时转换为秒

void cie_get_sntp_time();
void cie_initialize_sntp();
#endif
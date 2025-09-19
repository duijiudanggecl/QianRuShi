/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
/* MQTT (over TCP) Example with custom outbox

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

// #include <stdio.h>
// #include <stdint.h>
// #include <stddef.h>
// #include <string.h>
// #include "esp_system.h"
// #include "nvs_flash.h"
// #include "esp_event.h"
// #include "esp_netif.h"
// #include "protocol_examples_common.h"
// #include "esp_log.h"
// #include "mqtt_client.h"

#include "freertos/FreeRTOS.h"

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_log.h"
#include "esp_task.h"

#include "cie_sntp.h"
#include "cie_uart.h"
#include "cie_mqtt.h"
#include "cie_scan.h"
#include "cie_state.h"
#include "cie_pwm.h"
#include "blufi_wifi_manage.h"

#define CIE_PROJECT 1

TaskHandle_t cie_168b_handle = NULL;
TaskHandle_t cie_mqtt_handle = NULL;
TaskHandle_t cie_net_handle = NULL;

extern UI_state ui_state;
extern char server_url[64];

static const char *TAG = "CIE_PROJECT";

void app_main(void)
{
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    // esp_log_level_set("*", ESP_LOG_NONE);            //禁止所有打印信息输出
    esp_log_level_set("mqtt_client", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_BASE", ESP_LOG_VERBOSE);
    esp_log_level_set("esp-tls", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("custom_outbox", ESP_LOG_VERBOSE);

    cie_uart_init();

#if 1
    cie_wifi_manage();
#else
    cie_mqtt_init();
#endif

    vTaskDelay(pdMS_TO_TICKS(1000));    // 1000
    cie_pwm_init();
    cie_read_param_from_flash();        // 开机读取flash中的参数
    ui_state.black_light = 100;

    ESP_LOGI(TAG, "smartconfig :%d ssid:%s pw:%s url:%s", ui_state.wifi_state.smartconfig, ui_state.wifi_state.SSID,
             ui_state.wifi_state.PASSWORD, server_url);

    BaseType_t xTaskCreated_NetTask = xTaskCreate(
        cie_net_task,        // 任务函数
        "net_task",          // 任务名称
        ESP_TASK_MAIN_STACK, // 栈大小
        NULL,                // 传递给任务的参数
        ESP_TASK_MAIN_PRIO,  // 任务优先级
        &cie_net_handle      // 任务句柄
    );

    BaseType_t xTaskCreated_168BTask = xTaskCreate(
        cie_168b_task,           // 任务函数
        "168b_task",             // 任务名称
        ESP_TASK_MAIN_STACK * 2, // 栈大小
        NULL,                    // 传递给任务的参数
        ESP_TASK_MAIN_PRIO + 1,  // 任务优先级
        &cie_168b_handle         // 任务句柄
    );

    BaseType_t xTaskCreated_MqttTask = xTaskCreate(
        cie_mqtt_task,          // 任务函数
        "mqtt_task",            // 任务名称
        ESP_TASK_MAIN_STACK,    // 栈大小
        NULL,                   // 传递给任务的参数
        ESP_TASK_MAIN_PRIO + 2, // 任务优先级
        &cie_mqtt_handle        // 任务句柄
    );

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

// C:\Users\44193\esp\v5.3.2\esp-idf\components\mqtt\esp-mqtt\CMakeLists.txt 更改了该文件
// #idf_component_register(SRCS "mqtt_client.c" "${srcs}" => idf_component_register(SRCS "esp-mqtt/mqtt_client.c" "${srcs}"

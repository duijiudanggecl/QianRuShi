#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_http_client.h"
#include "cie_OTA.h"
#include "cie_uart.h"
#include "cie_state.h"

#define BUFFER_SIZE 1024 // 512 // 4096  // 4KB 缓冲区
extern UI_state ui_state;
extern Send_168b_flag send_168b_flag;
extern Send_data send_data;
static const char *TAG = "HTTP_CLIENT";
static int total_file_size = 0;     // 文件总大小（Content-Length）
static int downloaded_size = 0;     // 已下载大小
static char content_type[64] = {0}; // 文件类型（Content-Type）
// static char last_modified[128] = {0}; // 最后修改时间（Last-Modified）

int ui_size_total = 0;

extern unsigned char Func_data[2500];

char server_url[64] = {0};
char mcu_url[128] = {0};
char ui_url[128] = {0};

char *MCU_file_url = "http://192.168.0.172:8080/XGF_003.bin";
char *UI_file_url = "http://192.168.0.172:8080/UartTFT-II_Flash.bin";
// char *text_file_url = "http://192.168.0.172:8080/hello_world.bin";

// char MCU_file_url[128]  sprintf(MCU_file_url, "/%s/XGF_003.bin",ui_state.cie_mqtt_num);
// char UI_file_url[128]  sprintf(UI_file_url, "/%s/UartTFT-II_Flash.bin",ui_state.cie_mqtt_num);

// HTTP 事件处理函数
esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    static int total_read = 0;
    static uint8_t *buffer = NULL;

    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGE(TAG, "HTTP_EVENT_ERROR"); // 升级失败的处理
        ui_state.OTA_flag = OTA_FAIL;
        send_168b_flag.enter_OTA = true;
        break;

    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
        break;

    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
        break;

    case HTTP_EVENT_ON_HEADER:
    {
        // 解析 Content-Length（文件大小）
        //  ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
        if (strcmp(evt->header_key, "Content-Length") == 0)
        {
            total_file_size = atoi(evt->header_value);
            ESP_LOGI(TAG, "File size: %d bytes", total_file_size);

            if (ui_state.OTA_flag == OTA_UI)
            {
                ui_size_total = total_file_size;
                // send_168b_flag.send_ui_size = true;
                {
                    send_168b_flag.send_ui_size = false;
                    send_data.data_len_h4 = 0;
                    send_data.data_len_l5 = 8;
                    send_data.cmd_id6 = UART_UI_SIZE;
                    send_data.data_type7 = 1;
                    send_data.func_len_h8 = 0;
                    send_data.func_len_l9 = 4;
                    Func_data[0] = (ui_size_total >> 24) & 0xff;
                    Func_data[1] = (ui_size_total >> 16) & 0xff;
                    Func_data[2] = (ui_size_total >> 8) & 0xff;
                    Func_data[3] = (ui_size_total) & 0xff;

                    ESP_LOGI(TAG, "send File size: %d bytes", ui_size_total);

                    CIE_SendData_168B();
                    vTaskDelay(pdMS_TO_TICKS(200));
                }
            }
        }

        // 解析 Content-Type（文件类型）
        else if (strcmp(evt->header_key, "Content-Type") == 0)
        {
            strncpy(content_type, evt->header_value, sizeof(content_type) - 1);
            ESP_LOGI(TAG, "Content-Type: %s", content_type);
        }

        // 解析 Last-Modified（最后修改时间）
        // else if (strcmp(evt->header_key, "Last-Modified") == 0)
        //{
        // strncpy(last_modified, evt->header_value, sizeof(last_modified) - 1);
        //  char parsed_time[128] = {0};
        // parse_http_time(last_modified, parsed_time);
        // ESP_LOGI(TAG, "Last-Modified: %s (%s)", last_modified, parsed_time);
        // }
        // 其他头字段（可选）
        else
        {
            ESP_LOGI(TAG, "Header: %s=%s", evt->header_key, evt->header_value);
        }
        break;
    }

    case HTTP_EVENT_ON_DATA:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        // 如果是第一次接收数据，分配缓冲区
        if (!buffer)
        {
            buffer = (uint8_t *)malloc(BUFFER_SIZE);
            if (!buffer)
            {
                ESP_LOGE(TAG, "Failed to allocate buffer");
                return ESP_FAIL;
            }
        }

        // 如果数据长度超过缓冲区大小，分块处理
        int remaining = evt->data_len;
        // static int current_frame = -1;
        uint8_t *data_ptr = (uint8_t *)evt->data;

        while (remaining > 0)
        {
            int copy_len = (remaining > BUFFER_SIZE) ? BUFFER_SIZE : remaining;
            memcpy(buffer, data_ptr, copy_len);

            // 更新已下载大小
            downloaded_size += copy_len;
            ESP_LOGI(TAG, "buffer:%x Downloaded %d/%d bytes (%.1f%%)",
                     buffer[0], downloaded_size,
                     total_file_size,
                     (total_file_size > 0) ? (downloaded_size * 100.0 / total_file_size) : 0);

            data_ptr += copy_len;
            remaining -= copy_len;  // 剩余传输数据字节数
            total_read += copy_len; // 已传输数据字节数

            cie_168B_OTA(copy_len, buffer);

            if (ui_state.OTA_flag == OTA_UI)
                vTaskDelay(pdMS_TO_TICKS(20)); // UI
            else
                vTaskDelay(pdMS_TO_TICKS(100)); // MCU
        }
        break;

    case HTTP_EVENT_ON_FINISH:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
        downloaded_size = 0;
        if (buffer)
        {
            free(buffer);
            buffer = NULL;
        }
        if (ui_state.OTA_flag == OTA_MCU)
            cie_download_over(0); // 0:MCU  1:UI
        else if (ui_state.OTA_flag == OTA_UI)
            cie_download_over(1);
        break;

    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
        break;

    case HTTP_EVENT_REDIRECT: // 处理 HTTP 重定向
        ESP_LOGI(TAG, "HTTP_EVENT_REDIRECT detected");
        // 尝试从 HTTP 头获取 Location（如果可用）
        // const char *location = esp_http_client_get_header_value(evt->client, "Location");
        // if (location) {
        //     ESP_LOGI(TAG, "Redirect target: %s", location);
        // } else {
        //     ESP_LOGW(TAG, "Redirect detected, but 'Location' header is missing");
        // }
        break;
    }
    return ESP_OK;
}

void http_download_file(const char *url)
{
    esp_http_client_config_t config = {
        .url = url,
        .event_handler = _http_event_handler,
        .timeout_ms = 500000,                   // 120000,           // 5000,
        .disable_auto_redirect = false,         // 允许自动重定向（默认）
        .buffer_size = 4096,                    // 尝试增大缓冲区
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (!client)
    {
        ui_state.OTA_flag = OTA_FAIL;
        send_168b_flag.enter_OTA = true;
        ESP_LOGE(TAG, "Failed to initialize HTTP client");
        return;
    }

    esp_err_t err = esp_http_client_perform(client);

    // esp_http_client_set_header(client, "Connection", "keep-alive"); //@add

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "HTTP request failed: %s", esp_err_to_name(err));
    }
    else
    {
        int status_code = esp_http_client_get_status_code(client);
        ESP_LOGI(TAG, "HTTP status code: %d", status_code);

        if (status_code == 404) // 未找到文件
        {
            ui_state.OTA_flag = OTA_FAIL;
            send_168b_flag.enter_OTA = true;
        }
    }

    esp_http_client_cleanup(client);
}

void cie_OTA(uint8_t value)
{
    sprintf(mcu_url, "%s/XGF_003.bin", server_url);
    sprintf(ui_url, "%s/UartTFT-II_Flash.bin", server_url);

    ESP_LOGI(TAG, "Starting HTTP download...");

#if 1
    if (value == Download_MCU)
        http_download_file(mcu_url);
    else if (value == Download_UI)
        http_download_file(ui_url);
#else
    if (value == Download_MCU)
        http_download_file(MCU_file_url);
    else if (value == Download_UI)
        http_download_file(UI_file_url);
#endif

    ESP_LOGI(TAG, "Download completed");
}

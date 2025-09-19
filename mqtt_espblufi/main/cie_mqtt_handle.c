#include "cie_mqtt_handle.h"
#include "cie_state.h"
#include <stdlib.h> // 用于 strtol
#include <errno.h>  // 用于 errno
#include "cie_esp_OTA.h"
#include "cie_OTA.h"
#include "cie_tool.h"

static const char *TAG = "cie_mqtt_handle";
extern esp_mqtt_client_handle_t client;
extern UI_state ui_state;
extern Send_168b_flag send_168b_flag;
extern unsigned char mqtt_state[13];
extern char server_url[128];
extern unsigned char Func_data[2500];
extern Send_data send_data;
// static UINT8 utf_8[300] = {0};

// 用于处理接收到的主题和数据 ---接收云端信息
void handle_mqtt_rev_data(const char *data)
{
    // ESP_LOGI(TAG, "Received data on data: %s", data);
    // ESP_LOGI(TAG, "Data: %.*s", strlen(data), data); // 打印接收到的数据

    cJSON *json = cJSON_Parse(data);
    if (json == NULL)
    {
        ESP_LOGE(TAG, "Failed to parse JSON data");
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            ESP_LOGE(TAG, "Error before: %s", error_ptr);
        }
        return;
    }

    cJSON *id_item = cJSON_GetObjectItem(json, "id");
    cJSON *value_item = cJSON_GetObjectItem(json, "value");
    cJSON *remark_item = cJSON_GetObjectItem(json, "remark");

    if (id_item && cJSON_IsString(id_item))
    {
        ESP_LOGI(TAG, "ID: %s\n", id_item->valuestring);
    }
    else
    {
        ESP_LOGI(TAG, "ID field is missing or not a string\n");
    }

    // 打印 value 字段
    if (value_item && cJSON_IsString(value_item))
    {
        ESP_LOGI(TAG, "Value: %s\n", value_item->valuestring);
    }
    else
    {
        ESP_LOGI(TAG, "Value field is missing or not a string\n");
    }

    // 打印 remark 字段
    if (remark_item && cJSON_IsString(remark_item))
    {
        ESP_LOGI(TAG, "Remark: %s\n", remark_item->valuestring);
    }
    else
    {
        ESP_LOGI(TAG, "Remark field is missing or not a string\n");
    }

    // 判断接收到的id判别
    if (strcmp(id_item->valuestring, TOPIC_TEMPSET) == 0)
    {
        ESP_LOGI(TAG, "The received topic is TOPIC_TEMPSET");
        char *endptr;
        long value = strtol(value_item->valuestring, &endptr, 10); // 十进制转换

        // 检查转换是否成功
        if (errno == ERANGE || *endptr != '\0')
        {
            ESP_LOGE(TAG, "Failed to convert string to integer: %s", value_item->valuestring);
        }
        else
        {
            XGF_set_Reg_Temperature((short)value);
        }
    }

    else if (strcmp(id_item->valuestring, TOPIC_OTA) == 0)
    {
        ESP_LOGI(TAG, "The received topic is TOPIC_OTA");

        if (remark_item && cJSON_IsString(remark_item))
        {
            size_t length = strlen(remark_item->valuestring);
            ESP_LOGI(TAG, "remark: %s length:%d\n", remark_item->valuestring, length); // 接收到的mark信息
            memcpy(server_url, remark_item->valuestring, length);
            cie_write_server_url_to_flash();
        }
        else
            memset(server_url, 0, sizeof(server_url));

        // cie_esp_OTA();  //测试升级esp32程序

        // cie_OTA(0);
        send_168b_flag.enter_OTA = true;
    }

    else if (strcmp(id_item->valuestring, TOPIC_CONTROL) == 0)
    {
        ESP_LOGI(TAG, "The received topic is TOPIC_CONTROL");
        // cie_scan(); // 扫描wifi信号强度
    }

    else if (strcmp(id_item->valuestring, TOPIC_MODE) == 0)
    {
        ESP_LOGI(TAG, "The received topic is TOPIC_MODE");
        char *endptr;
        long value = strtol(value_item->valuestring, &endptr, 10); // 十进制转换

        // 检查转换是否成功
        if (errno == ERANGE || *endptr != '\0')
        {
            ESP_LOGE(TAG, "Failed to convert string to integer: %s", value_item->valuestring);
        }
        else
        {
            XGF_set_Reg_FuncMode((short)value);
            if ((value >= MODE_COLD) && (value <= MODE_DINUAN_ICON))
                send_168b_flag.set_mode = true;
        }
    }

    else if (strcmp(id_item->valuestring, TOPIC_SWITCH) == 0)
    {
        ESP_LOGI(TAG, "The received topic is TOPIC_SWITCH");
        char *endptr;
        long value = strtol(value_item->valuestring, &endptr, 10); // 十进制转换

        // 检查转换是否成功
        if (errno == ERANGE || *endptr != '\0')
        {
            ESP_LOGE(TAG, "Failed to convert string to integer: %s", value_item->valuestring);
        }
        else
        {
            XGF_set_Reg_OnOff((short)value);
        }
    }

    else if (strcmp(id_item->valuestring, TOPIC_CHILD_LOCK) == 0)
    {
        ESP_LOGI(TAG, "The received topic is TOPIC_CHILD_LOCK"); // TOPIC_CHILD_LOCK
        char *endptr;
        long value = strtol(value_item->valuestring, &endptr, 10); // 十进制转换

        // 检查转换是否成功
        if (errno == ERANGE || *endptr != '\0')
        {
            ESP_LOGE(TAG, "Failed to convert string to integer: %s", value_item->valuestring);
        }
        else
        {
            XGF_set_Reg_Lock((short)value);
            // if ((value >= SPEED_AUTO) && (value <= SPEED_HIGH))
            send_168b_flag.set_child_lock = true;
        }
    }

    else if (strcmp(id_item->valuestring, TOPIC_GEAR) == 0)
    {
        ESP_LOGI(TAG, "The received topic is TOPIC_GEAR");
        char *endptr;
        long value = strtol(value_item->valuestring, &endptr, 10); // 十进制转换

        // 检查转换是否成功
        if (errno == ERANGE || *endptr != '\0')
        {
            ESP_LOGE(TAG, "Failed to convert string to integer: %s", value_item->valuestring);
        }
        else
        {
            XGF_set_Reg_Speed((short)value);
            if ((value >= SPEED_AUTO) && (value <= SPEED_HIGH))
                send_168b_flag.set_gear = true;
        }
    }

    else if (strcmp(id_item->valuestring, TOPIC_TIME_PAN) == 0)
    {
        // ESP_LOGI(TAG, "The received topic is TOPIC_MODE");
    }

    else if (strcmp(id_item->valuestring, TOPIC_TIME_ON) == 0)
    {
        // ESP_LOGI(TAG, "The received topic is TOPIC_MODE");
    }

    else if (strcmp(id_item->valuestring, TOPIC_TIME_OFF) == 0)
    {
        // ESP_LOGI(TAG, "The received topic is TOPIC_MODE");
    }

    else if (strcmp(id_item->valuestring, TOPIC_CLEAR_USAGE_TIME) == 0)
    {
        ESP_LOGI(TAG, "The received topic is TOPIC_CLEAR_USAGE_TIME");
        char *endptr;
        long value = strtol(value_item->valuestring, &endptr, 10); // 十进制转换

        // 检查转换是否成功
        if (errno == ERANGE || *endptr != '\0')
        {
            ESP_LOGE(TAG, "Failed to convert string to integer: %s", value_item->valuestring);
        }
        else
        {
            if (value == 1)
            {
                ui_state.clear_usage_time = 1;
                send_168b_flag.clear_usage_time = true;
            }
        }
    }

    else if (strcmp(id_item->valuestring, TOPIC_WEATHER) == 0)
    {
        ESP_LOGI(TAG, "The received topic is TOPIC_WEATHER");
        char *endptr;
        long value = strtol(value_item->valuestring, &endptr, 10); // 十进制转换

        memset(ui_state.weather_info, 0, sizeof(ui_state.weather_info));
        // memset(utf_8, 0, sizeof(utf_8));
        // 检查转换是否成功
        if (errno == ERANGE || *endptr != '\0')
        {
            ESP_LOGE(TAG, "Failed to convert string to integer: %s", value_item->valuestring);
        }
        else
        {
            ui_state.weather_info[0] = value / 1000000;       // 37 (前2位)
            ui_state.weather_info[1] = (value / 10000) % 100; // 25 (第3-4位)
            ui_state.weather_info[2] = (value / 1000) % 10;   // 1  (第5位)
            ui_state.weather_info[3] = (value / 10) % 100;    // 36 (第6-7位)
            ui_state.weather_info[4] = value % 10;            // 5  (最后1位)
            /*
            ui_state.weather_info[0] = value / 100000000; //天气状况
            ui_state.weather_info[1] = (value - ui_state.weather_info[0] * 100000000) / 1000000; //上限整数
            ui_state.weather_info[2] = (value - ui_state.weather_info[0] * 100000000 - ui_state.weather_info[1] * 1000000) / 10000;  //上限小数
            ui_state.weather_info[3] = (value - ui_state.weather_info[0] * 100000000 - ui_state.weather_info[1] * 1000000 - ui_state.weather_info[2] * 10000) / 100;
            ui_state.weather_info[4] = value % 100;
            */
        }

        if (remark_item && cJSON_IsString(remark_item))
        {
            size_t length = strlen(remark_item->valuestring);
            ESP_LOGI(TAG, "weather: %s length:%d\n", remark_item->valuestring, length);

            // utf8_TO_utf16(&remark_item->valuestring,length,ui_state.message_info+5,64);

            if (5 + length < sizeof(ui_state.weather_info))
            { // 确保不会溢出

                // memcpy(utf_8, remark_item->valuestring, length);
                // utf_8[length] = '\0'; // 添加字符串终止符
                // utf8_TO_utf16(utf_8,length,ui_state.weather_info+5,64);

                memcpy(ui_state.weather_info + 5, remark_item->valuestring, length);
                ui_state.weather_info[5 + length] = '\0'; // 添加字符串终止符
                send_168b_flag.send_weather_info = true;

                ESP_LOGI(TAG, "send_168b weather: %s \n", ui_state.weather_info + 5);

#if 0
                if (1)
                {
                    ESP_LOGI(TAG, "weather data (hex):");
                    for (int i = 0; i < 32; i++)
                    {
                        ESP_LOGI(TAG, "%02x  ", ui_state.weather_info[5 + i]); // 打印每个字节的十六进制值
                    }
                    ESP_LOGI(TAG, "over"); // 换行以美化输出
                }
#endif
            }
            else
            {
                ESP_LOGE(TAG, "Buffer overflow risk: weather_info is too small.\n");
            }
        }
    }

    else if (strcmp(id_item->valuestring, TOPIC_MESSAGE) == 0)
    {
        ESP_LOGI(TAG, "The received topic is TOPIC_MESSAGE");
        char *endptr;
        long value = strtol(value_item->valuestring, &endptr, 10); // 十进制转换

        memset(ui_state.message_info, 0, sizeof(ui_state.message_info));
        // memset(utf_8, 0, sizeof(utf_8));
        //  检查转换是否成功
        if (errno == ERANGE || *endptr != '\0')
        {
            ESP_LOGE(TAG, "Failed to convert string to integer: %s", value_item->valuestring);
        }
        else
        {
            ui_state.message_info[0] = value; // 邮件属性
        }

        if (remark_item && cJSON_IsString(remark_item))
        {
            size_t length = strlen(remark_item->valuestring);
            ESP_LOGI(TAG, "message: %s length:%d\n", remark_item->valuestring, length); // 接收到的邮件信息

            // utf8_TO_utf16(&remark_item->valuestring,length,ui_state.message_info+1,300);

            if (1 + length < sizeof(ui_state.message_info))
            { // 确保不会溢出  utf_8
                /*
                memcpy(utf_8, remark_item->valuestring, length);
                utf_8[length] = '\0'; // 添加字符串终止符
                utf8_TO_utf16(utf_8,length,ui_state.message_info+1,64);
                send_168b_flag.send_message_info = true;
                */

                memcpy(ui_state.message_info + 1, remark_item->valuestring, length);
                ui_state.message_info[1 + length] = '\0'; // 添加字符串终止符
                send_168b_flag.send_message_info = true;

                {
                    // send_data.data_len_h4 = (4 + sizeof(ui_state.message_info)) / 256;
                    // send_data.data_len_l5 = (4 + sizeof(ui_state.message_info)) % 256;
                    send_data.data_len_h4 = (4 + 2 + length) / 256;
                    send_data.data_len_l5 = (4 + 2 + length) % 256;

                    send_data.cmd_id6 = UART_REV_MQTT_MESSAGE_INFO;
                    send_data.data_type7 = 1;
                    // send_data.func_len_h8 = (sizeof(ui_state.message_info)) / 256;
                    // send_data.func_len_l9 = (sizeof(ui_state.message_info)) % 256;
                    send_data.func_len_h8 = (2 + length) / 256;
                    send_data.func_len_l9 = (2 + length) % 256;

                    // memcpy(Func_data, ui_state.message_info, sizeof(ui_state.message_info));
                    memcpy(Func_data, ui_state.message_info, sizeof(ui_state.message_info));
                    CIE_SendData_168B();
                }

                ESP_LOGI(TAG, "send_168b message: %s \n", ui_state.message_info + 1);
            }
            else
            {
                ESP_LOGE(TAG, "Buffer overflow risk: message_info is too small.\n");
            }
            vTaskDelay(pdMS_TO_TICKS(500));
            send_168b_flag.send_message_info = false;
        }
    }
}

// 辅助函数：创建并发布 JSON 消息
static int publish_json_message(const char *topic, cJSON *root)
{
    int err = ESP_FAIL;
    char *json_string = cJSON_PrintUnformatted(root);
    if (json_string == NULL)
    {
        ESP_LOGE(TAG, "Failed to print cJSON object");
        cJSON_Delete(root);
        return ESP_FAIL;
    }
    // 在发布前检查客户端状态
    if (ui_state.esp32_mqtt_state == MQTT_EVENT_CONNECTED)
    {
        // ESP_LOGI(TAG, "start push topic:%s", topic);
        err = esp_mqtt_client_publish(client, topic, json_string, 0, CIE_QoS, 0);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "MQTT publish failed: %d", err);
        }
        // ESP_LOGI(TAG, "end push topic:%s", topic);
    }
    else
    {
        ESP_LOGE(TAG, "MQTT client not connected");
    }

    // int err = esp_mqtt_client_publish(client, topic, json_string, 0, CIE_QoS, 0);
    // ESP_LOGI(TAG, "end push topic:%s", topic);

    free(json_string);  // 释放 JSON 字符串
    cJSON_Delete(root); // 删除 JSON 对象

    if (err < ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to publish message: %s", esp_err_to_name(err));
    }
    else
    {
        ESP_LOGI(TAG, "Message published successfully: %s", topic);
    }

    return err;
}

int CIE_publish(const char *publish_func)
{
    ESP_LOGI(TAG, "CIE_publish topic:%s", publish_func);
    char subscribe_topic[128];
    char property_subscribe_topic[128];
    char info_subscribe_topic[128];

    sprintf(subscribe_topic, "/%s/%s/event/post", ui_state.cie_mqtt_id, ui_state.cie_mqtt_num);
    sprintf(property_subscribe_topic, "/%s/%s/property/post", ui_state.cie_mqtt_id, ui_state.cie_mqtt_num);
    sprintf(info_subscribe_topic, "/%s/%s/info/post", ui_state.cie_mqtt_id, ui_state.cie_mqtt_num);

    if (strcmp(publish_func, TOPIC_TEMPSET) == 0)
    {
        cJSON *root = cJSON_CreateObject();
        if (root == NULL)
        {
            ESP_LOGE(TAG, "Failed to create cJSON object");
            return ESP_FAIL;
        }

        cJSON_AddStringToObject(root, "id", TOPIC_TEMPSET);
        char temp_str[32];
        snprintf(temp_str, sizeof(temp_str), "%d", ui_state.set_temperatre); // mqtt_state[MQTT_STATE_LOCK_5]
        cJSON_AddStringToObject(root, "value", temp_str);
        cJSON_AddStringToObject(root, "remark", "");

        return publish_json_message(subscribe_topic, root);
    }
    else if (strcmp(publish_func, TOPIC_MODE) == 0)
    {
        cJSON *root = cJSON_CreateObject();
        if (root == NULL)
        {
            ESP_LOGE(TAG, "Failed to create cJSON object");
            return ESP_FAIL;
        }

        cJSON_AddStringToObject(root, "id", TOPIC_MODE);
        char temp_str[32];
        snprintf(temp_str, sizeof(temp_str), "%d", ui_state.func_mode); // mqtt_state[MQTT_STATE_LOCK_5]
        cJSON_AddStringToObject(root, "value", temp_str);
        cJSON_AddStringToObject(root, "remark", "");

        return publish_json_message(subscribe_topic, root);
    }

    else if (strcmp(publish_func, TOPIC_SWITCH) == 0)
    {
        cJSON *root = cJSON_CreateObject();
        if (root == NULL)
        {
            ESP_LOGE(TAG, "Failed to create cJSON object");
            return ESP_FAIL;
        }

        cJSON_AddStringToObject(root, "id", TOPIC_SWITCH);
        char temp_str[32];
        snprintf(temp_str, sizeof(temp_str), "%d", ui_state.on_off); // mqtt_state[MQTT_STATE_LOCK_5]
        cJSON_AddStringToObject(root, "value", temp_str);
        cJSON_AddStringToObject(root, "remark", "");

        return publish_json_message(subscribe_topic, root);
    }

    else if (strcmp(publish_func, TOPIC_GEAR) == 0)
    {
        cJSON *root = cJSON_CreateObject();
        if (root == NULL)
        {
            ESP_LOGE(TAG, "Failed to create cJSON object");
            return ESP_FAIL;
        }

        cJSON_AddStringToObject(root, "id", TOPIC_GEAR);
        char temp_str[32];
        snprintf(temp_str, sizeof(temp_str), "%d", mqtt_state[MQTT_STATE_SPEED_5]); // mqtt_state[MQTT_STATE_SPEED_5]  ui_state.fan_speed
        cJSON_AddStringToObject(root, "value", temp_str);
        cJSON_AddStringToObject(root, "remark", "");

        return publish_json_message(subscribe_topic, root);
    }

    else if (strcmp(publish_func, TOPIC_CHILD_LOCK) == 0)
    {
        cJSON *root = cJSON_CreateObject();
        if (root == NULL)
        {
            ESP_LOGE(TAG, "Failed to create cJSON object");
            return ESP_FAIL;
        }

        cJSON_AddStringToObject(root, "id", TOPIC_CHILD_LOCK);
        char temp_str[32];
        snprintf(temp_str, sizeof(temp_str), "%d", mqtt_state[MQTT_STATE_LOCK_7]); // mqtt_state[MQTT_STATE_LOCK_5]  ui_state.lock_onoff
        cJSON_AddStringToObject(root, "value", temp_str);
        cJSON_AddStringToObject(root, "remark", "");

        return publish_json_message(subscribe_topic, root);
    }

    else if (strcmp(publish_func, TOPIC_CUR_TEMP) == 0)
    {
        cJSON *root = cJSON_CreateObject();
        if (root == NULL)
        {
            ESP_LOGE(TAG, "Failed to create cJSON object");
            return ESP_FAIL;
        }

        cJSON_AddStringToObject(root, "id", TOPIC_CUR_TEMP);
        char temp_str[32];
        snprintf(temp_str, sizeof(temp_str), "%d", ui_state.current_temperatre); // mqtt_state[MQTT_STATE_LOCK_5]
        cJSON_AddStringToObject(root, "value", temp_str);
        cJSON_AddStringToObject(root, "remark", "");

        return publish_json_message(property_subscribe_topic, root);
    }

    else if (strcmp(publish_func, TOPIC_RELAY_STATE) == 0)
    {
        cJSON *root = cJSON_CreateObject();
        if (root == NULL)
        {
            ESP_LOGE(TAG, "Failed to create cJSON object");
            return ESP_FAIL;
        }

        uint8_t relay_state = mqtt_state[RELAY_STATE_12]; // 示例值：1

        // 将二进制数据转为字符串
        char binary_str[9]; // 8位二进制 + '\0'
        for (int i = 7; i >= 0; i--)
        {
            binary_str[7 - i] = (relay_state & (1 << i)) ? '1' : '0';
        }
        binary_str[8] = '\0';

        cJSON_AddStringToObject(root, "id", TOPIC_RELAY_STATE);
        // char temp_str[32];
        // snprintf(temp_str, sizeof(temp_str), "%d", mqtt_state[RELAY_STATE_12]); // mqtt_state[MQTT_STATE_LOCK_5]
        cJSON_AddStringToObject(root, "value", binary_str);
        cJSON_AddStringToObject(root, "remark", "");

        // ESP_LOGE(TAG, "relay_state:%s",binary_str);

        return publish_json_message(subscribe_topic, root);
    }

    else if (strcmp(publish_func, TOPIC_DEV) == 0)
    {
        cJSON *root = cJSON_CreateObject();
        if (root == NULL)
        {
            ESP_LOGE(TAG, "Failed to create cJSON object");
            return ESP_FAIL;
        }

#if 1
        uint8_t relay_state = mqtt_state[RELAY_STATE_12];
        // 将二进制数据转为字符串
        char binary_str[9]; // 8位二进制 + '\0'
        for (int i = 7; i >= 0; i--)
        {
            binary_str[7 - i] = (relay_state & (1 << i)) ? '1' : '0';
        }
        binary_str[8] = '\0';

        cJSON_AddNumberToObject(root, "temperature", mqtt_state[MQTT_STATE_CUR_TEMP_0] * 256 + mqtt_state[MQTT_STATE_CUR_TEMP_1]);
        cJSON_AddNumberToObject(root, "tempset", mqtt_state[MQTT_STATE_TEMP_SET_2] * 256 + mqtt_state[MQTT_STATE_TEMP_SET_3]);
        cJSON_AddNumberToObject(root, "mode", mqtt_state[MQTT_STATE_MODE_4]);
        cJSON_AddNumberToObject(root, "gear", mqtt_state[MQTT_STATE_SPEED_5]);
        cJSON_AddNumberToObject(root, "switch", mqtt_state[MQTT_STATE_SWITCH_6]);
        cJSON_AddNumberToObject(root, "child_lock", mqtt_state[MQTT_STATE_LOCK_7]);
        cJSON_AddNumberToObject(root, "usage_time", mqtt_state[TOTAL_USAGE_TIME_8] * 0x1000000 + mqtt_state[TOTAL_USAGE_TIME_9] * 0x10000 + mqtt_state[TOTAL_USAGE_TIME_10] * 0x100 + mqtt_state[TOTAL_USAGE_TIME_11]);

        cJSON_AddStringToObject(root, "relay_state", binary_str);
#else
        cJSON_AddNumberToObject(root, "rssi", ui_state.wifi_state.test_RSSI);
        cJSON_AddNumberToObject(root, "firmwareVersion", 1.2);
        cJSON_AddNumberToObject(root, "status", 3);
        cJSON_AddStringToObject(root, "userId", "1");
        cJSON_AddNumberToObject(root, "longitude", 0);
        cJSON_AddNumberToObject(root, "latitude", 0);

        cJSON *summary = cJSON_CreateObject();
        cJSON_AddStringToObject(summary, "name", "wumei-smart");
        cJSON_AddStringToObject(summary, "chip", "esp32");
        cJSON_AddStringToObject(summary, "author", "");
        cJSON_AddStringToObject(summary, "version", "");
        cJSON_AddStringToObject(summary, "createTime", "");
        cJSON_AddItemToObject(root, "summary", summary);
#endif
        return publish_json_message(info_subscribe_topic, root);
    }

    ESP_LOGE(TAG, "Unknown publish function: %s", publish_func);
    return ESP_FAIL;
}

// 订阅主题  只接收指定的设备的信息
void CIE_power_on_subscribe(void)
{
    // int msg_id;
    char subscribe_topic[128]; // sprintf(subscribe_topic, "/%s/%s/event/post", ui_state.cie_mqtt_id, ui_state.cie_mqtt_num);
    sprintf(subscribe_topic, "/%s/%s/function/get", ui_state.cie_mqtt_id, ui_state.cie_mqtt_num);
    esp_mqtt_client_subscribe(client, subscribe_topic, CIE_QoS);

    // ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
    ESP_LOGI(TAG, "sent subscribe successful, topic:%s", subscribe_topic);
}
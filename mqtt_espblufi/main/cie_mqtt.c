#include "cie_mqtt.h"
#include "esp_log.h"
#include "cJSON.h"
#include "cie_scan.h"
#include "cie_mqtt_handle.h"
#include "cie_state.h"
#include "mqtt_client.h"
#include "cie_wifi.h"
#include "cie_connect.h"
#include "cie_state.h"
#include "cie_OTA.h"
#include "blufi_wifi_manage.h"

static const char *TAG = "cie_mqtt";

CIE_mqtt_publish_flag cie_mqtt_publish_flag;
esp_mqtt_client_handle_t client; // = event->client;

extern UI_state ui_state;
extern Send_168b_flag send_168b_flag;
extern unsigned char cie_mqtt_info[MQTT_INFO_LENGTH];

static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0)
    {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32 "", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    client = event->client;
    // esp_mqtt_client_handle_t client = event->client;
    // int msg_id;
    // char topic_buffer[event->topic_len + 1];
    char data_buffer[event->data_len + 1];

    // char subscribe_topic[50];

    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED: // 连接
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        ui_state.esp32_mqtt_state = MQTT_EVENT_CONNECTED;
        send_168b_flag.send_mqtt_state = true;

#if CIE_MQTT_TEST
        msg_id = esp_mqtt_client_subscribe(client, "control", 2); // 订阅一个/control的topic
        ESP_LOGI(TAG, "swiftchen sent subscribe successful, msg_id=%d", msg_id);
#endif
        CIE_power_on_subscribe();
        break;

    case MQTT_EVENT_DISCONNECTED: // 断开连接
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        ui_state.esp32_mqtt_state = MQTT_EVENT_DISCONNECTED;
        send_168b_flag.send_mqtt_state = false;
        break;

    case MQTT_EVENT_SUBSCRIBED:
        // ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        // msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
        // ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        break;

    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;

    case MQTT_EVENT_DATA: // 收到云端mqtt数据，进行处理
        memcpy(data_buffer, event->data, event->data_len);
        data_buffer[event->data_len] = '\0';
        handle_mqtt_rev_data(data_buffer);
        break;

    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT)
        {
            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno", event->error_handle->esp_transport_sock_errno);
            ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
        }
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

static void mqtt_app_stop(esp_mqtt_client_handle_t client)
{
    if (client != NULL)
    {
        // 停止 MQTT 客户端
        ESP_LOGI(TAG, "will stop mqtt");
        esp_mqtt_client_stop(client);

        // 注销事件处理程序（可选，但建议清理）
        ESP_LOGI(TAG, "will unregister mqtt event");
        esp_mqtt_client_unregister_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler);

        // 销毁 MQTT 客户端并释放资源
        // ESP_LOGI(TAG, "will destroy client");
        // esp_mqtt_client_destroy(client);

        ESP_LOGI(TAG, "MQTT client stopped and resources freed");
    }
    else
    {
        ESP_LOGE(TAG, "MQTT client handle is NULL, cannot stop");
    }
}

void debug_string(const unsigned char *str)
{
    ESP_LOGI(TAG, "String: %s", str);
    ESP_LOGI(TAG, "String length: %d", strlen((const char *)str));
    for (int i = 0; i < strlen((const char *)str); i++)
    {
        ESP_LOGI(TAG, "Byte[%d]: 0x%02x", i, str[i]);
    }
}

static void mqtt_app_start(void)
{
#if 1
    // 获取 ESP32 的 MAC 地址
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);

    // 格式化 MAC 地址为字符串
    char mac_str[18];
    snprintf(mac_str, sizeof(mac_str), "%02x:%02x:%02x:%02x:%02x:%02x",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    // 创建 client_id 字符串
    char client_id[32]; // 确保缓冲区足够大
    snprintf(client_id, sizeof(client_id), "ESP32_%s", mac_str);
    memcpy(ui_state.MAC_ADDR, client_id, 32); // 设置MAC地址
#endif
    // URL 端口 账号 密码 ID
    // ui_state.cie_url;
    // ui_state.cie_name;
    // ui_state.cie_port;
    // ui_state.cie_id;
    // ui_state.cie_password;
    // esp_mqtt_client_set_uri
    // 遍历数组，将空格字符替换为 '0'

    for (int i = 0; i < sizeof(ui_state.cie_url); i++)
    {
        if (ui_state.cie_url[i] == 0x20)
        {                            // 检查字符是否是空格
            ui_state.cie_url[i] = 0; // 替换为 '0'
        }
    }

    // 检查 URI 是否以 "mqtt://" 开头
    if (strncmp((const char *)ui_state.cie_url, "mqtt://", 7) != 0)
    {
        ESP_LOGE(TAG, "MQTT URI must start with 'mqtt://'");
        return;
    }

    esp_mqtt_client_config_t mqtt_cfg = {
#if 1 // CIE_MQTT_INFO_SELECT
        .broker.address.uri = (char *)ui_state.cie_url,
        //.broker.address.uri = MQTT_URL,
        .buffer.size = 8192, // 4096, // 2048,
        .broker.address.port = ui_state.cie_port,
        .credentials.username = (char *)ui_state.cie_name,
        .credentials.client_id = (char *)client_id,
        .credentials.authentication.password = (char *)ui_state.cie_password,
#else
        .broker.address.uri = MQTT_URL, // CONFIG_BROKER_URL,    //"mqtt://mqtt.eclipseprojects.io"  mqtt://mqtt.eclipseprojects.io
        // ui_state.cie_url;
        .buffer.size = 2048, // 增大缓冲区
        .broker.address.port = 1883,
        // ui_state.cie_port;
        .credentials.username = "CIE",
        // ui_state.cie_name;
        // .credentials.client_id = "123321",
        .credentials.client_id = client_id, // 确保唯一性 MAC地址
        .credentials.authentication.password = "123456",
    // ui_state.cie_password;
#endif

        //.session.last_will.topic = "cie",
        //.session.last_will.msg = "cie off",
        //.session.last_will.qos = 1,
        //.session.last_will.retain = 0,

        .session.keepalive = 30, // 60, // 心跳周期设置为120秒
        .session.disable_keepalive = false,
    };

    // memcpy(mqtt_cfg.broker.address.uri,ui_state.cie_url,64);
    // ESP_LOGI(TAG, "mqtt_url: %s \r\n", mqtt_cfg.broker.address.uri );
    // ESP_LOGI(TAG, "ui_state.cie_url: %s \r\n", ui_state.cie_url);
    // debug_string(ui_state.cie_url);

    client = esp_mqtt_client_init(&mqtt_cfg);

    ESP_LOGI(TAG, "Client ID: %s", mqtt_cfg.credentials.client_id);
    // ESP32_94:54:c5:a6:5a:64

    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);

    esp_mqtt_client_start(client);
}

// mqtt发布信息管理
static void cie_mqtt_publish_handle(void)
{
    if (cie_mqtt_publish_flag.dev_information == true)
    {
        cie_mqtt_publish_flag.dev_information = false;
        CIE_publish(TOPIC_DEV);
    }

    else if (cie_mqtt_publish_flag.set_temperatre == true)
    {
        cie_mqtt_publish_flag.set_temperatre = false;
        CIE_publish(TOPIC_TEMPSET);
    }

    else if (cie_mqtt_publish_flag.set_mode == true)
    {
        cie_mqtt_publish_flag.set_mode = false;
        CIE_publish(TOPIC_MODE);
    }

    else if (cie_mqtt_publish_flag.set_gear == true)
    {
        cie_mqtt_publish_flag.set_gear = false;
        CIE_publish(TOPIC_GEAR);
    }

    else if (cie_mqtt_publish_flag.report_temp == true)
    {
        cie_mqtt_publish_flag.report_temp = false;
        CIE_publish(TOPIC_CUR_TEMP);
    }

    else if (cie_mqtt_publish_flag.set_switch == true)
    {
        cie_mqtt_publish_flag.set_switch = false;
        CIE_publish(TOPIC_SWITCH);
    }

    else if (cie_mqtt_publish_flag.set_child_lock == true)
    {
        cie_mqtt_publish_flag.set_child_lock = false;
        CIE_publish(TOPIC_CHILD_LOCK);
    }

    else if (cie_mqtt_publish_flag.report_relay_state == true)
    {
        cie_mqtt_publish_flag.report_relay_state = false;
        CIE_publish(TOPIC_RELAY_STATE);
    }
}

void cie_mqtt_init(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    uint8_t mac[6];
    char mac_str[18];
    char client_id[32];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    snprintf(mac_str, sizeof(mac_str), "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    snprintf(client_id, sizeof(client_id), "ESP32_%s", mac_str);
    memcpy(ui_state.MAC_ADDR, client_id, 32); // 设置MAC地址
}

extern time_t now;
// extern SemaphoreHandle_t mqtt_Mutex;
//  此任务用于主动发布信息到云平台
void cie_mqtt_task(void *pvParameters)
{
    static int mqtt_change = 0;
    static int mqtt_init = 1;
    static int sntp_init = 1;

    while (1)
    {
        if (ui_state.wifi_state.connect_flag == ENABLE_1)
        {
            if (sntp_init == 1)
            {
                sntp_init = 0;
                cie_initialize_sntp();
                cie_get_sntp_time();
            }

            // 接收到168B发送的IP信息，有服务器地址等信息才开始连接网络  ui_state.rev_ip_info == true
            if ((mqtt_init == 1) && (strlen((const char *)ui_state.cie_url) > 0)) // 首次连上网
            {
                //cie_scan();
                //cie_get_sntp_time();
                mqtt_app_start();
                mqtt_init = 0;
                //cie_mqtt_publish_flag.dev_information = true;
                ui_state.wifi_state.mqtt_change_flag = 0;
                ui_state.rev_ip_info = 0;
            }

            else if ((ui_state.wifi_state.mqtt_change_flag == 1) || (ui_state.rev_ip_info == 1)) // 更改了mqtt信息 连上了网
            {
                mqtt_app_stop(client);
                mqtt_app_start();
                ui_state.wifi_state.mqtt_change_flag = 0;
                ui_state.rev_ip_info = 0;
            }

            if (strlen((const char *)ui_state.cie_url) == 0)
                send_168b_flag.get_mqtt_info = true;

            if (ui_state.esp32_mqtt_state == MQTT_EVENT_CONNECTED) // 发布主题
            {
                cie_mqtt_publish_handle();
            }

            if (ui_state.OTA_flag == 0)
            {
                cie_get_sntp_time();
            }
        }
        else
            now = 0;

        vTaskDelay(pdMS_TO_TICKS(2000));
        // ESP_LOGI(TAG, "cie_mqtt_task...");
    }
}

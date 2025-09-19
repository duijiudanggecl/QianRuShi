#ifndef CIE_MQTT_H_
#define CIE_MQTT_H_

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "cie_sntp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define CIE_MQTT_TEST 0

#define CIE_MQTT_INFO_SELECT 0 // 根据168B接收的IP MQTT信息进行连接
#define ENABLE_STATIC_IP 1
#define TOPIC_CONTROL "control"

#define CIE_QoS 1 // 恰好一次

#define PRODUCT_ID "168"	   //							ui_state.cie_id
#define DEVICE_NUM "swiftchen" // 产品唯一标识符，MAC地址     通过程序获取	ui_state.MAC_ADDR
#define MQTT_URL "mqtt://mqtt.eclipseprojects.io"

#define TOPIC_DEV "dev_information"
#define TOPIC_TEMPSET "tempset"
#define TOPIC_MODE "mode"
#define TOPIC_SWITCH "switch"
#define TOPIC_CHILD_LOCK "child_lock"
#define TOPIC_GEAR "gear"
#define TOPIC_TIME_PAN "time_pan"
#define TOPIC_TIME_ON "time_on"
#define TOPIC_TIME_OFF "time_off"
#define TOPIC_CLEAR_USAGE_TIME "clear_usage_time"
#define TOPIC_RELAY_STATE "relay_state"

#define TOPIC_WEATHER "weather"
#define TOPIC_MESSAGE "message"

#define TOPIC_CUR_TEMP "temperature"
#define TOPIC_OTA "OTA"
typedef struct
{
	bool dev_information;
	bool set_temperatre;
	bool set_mode;
	bool set_switch;
	bool set_child_lock;
	bool set_gear;
	bool set_time_pan;
	bool set_time_on;
	bool set_time_off;
	bool report_relay_state;
	bool report_temp;
	// bool report_state;
} CIE_mqtt_publish_flag;

void cie_mqtt_task(void *pvParameters);
void cie_mqtt_init(void);

#endif
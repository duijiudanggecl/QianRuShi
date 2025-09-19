#ifndef CIE_UART_H_
#define CIE_UART_H_

#include <string.h>
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "cie_tool.h"
#include "cie_mqtt.h"

#define MQTT_STATE_CUR_TEMP_0 0
#define MQTT_STATE_CUR_TEMP_1 1
#define MQTT_STATE_TEMP_SET_2 2
#define MQTT_STATE_TEMP_SET_3 3
#define MQTT_STATE_MODE_4 4
#define MQTT_STATE_SPEED_5 5
#define MQTT_STATE_SWITCH_6 6
#define MQTT_STATE_LOCK_7 7
#define TOTAL_USAGE_TIME_8 8
#define TOTAL_USAGE_TIME_9 9
#define TOTAL_USAGE_TIME_10 10
#define TOTAL_USAGE_TIME_11 11
#define RELAY_STATE_12 12
// typedef unsigned long long UINT64;
// typedef unsigned int UINT32;
// typedef unsigned short int UINT16;
// typedef unsigned char UINT8;

/*	联网信息
字段	偏移量（字节）	长度（字节）		描述
模式		0				1			0:自动, 1:手动
IP 地址		1				4			IPv4 地址
子网掩码	5				4			子网掩码
默认网关	9				4			默认网关
URL			13				64			网址（URL）
端口		77				4			端口号
账号		81				16			用户账号
密码		97				4			密码（4字节）
ID			101				4			标识符（4字节）

*/
#define MQTT_INFO_LENGTH 228 // 248
#define WIFI_INFO_LENGTH 78

#define IP_MODE_OFFSET 0
#define IP_ADDR_OFFSET 1
#define IP_ZIWANG_OFFSET 5
#define IP_WANGGUAN_OFFSET 9

#define MQTT_URL_OFFSET 13
#define MQTT_PORT_OFFSET 77
#define MQTT_ACCOUNT_OFFSET 81
#define MQTT_PW_OFFSET 97
#define MQTT_ID_OFFSET 105

#if 1 // XGF_WIFI_MQTT_MODIFY

#define IP_INFO_MODE_OFFSET 0	  // 1byte
#define IP_INFO_ADDR_OFFSET 1	  // 4byte
#define IP_INFO_ZIWANG_OFFSET 5	  // 4byte
#define IP_INFO_WANGGUAN_OFFSET 9 // 4byte
#define WIFI_INFO_SMART_OFFSET 13 // 1:��������  0:�ֶ�����  1byte
#define WIFI_INFO_SSID_OFFSET 14  // 32byte
#define WIFI_INFO_PW_OFFSET 46	  // 32byte    78byte

/*
#define MQTT_INFO_URL_OFFSET					0			//64byte
#define MQTT_INFO_PORT_OFFSET					64  		//4byte
#define MQTT_INFO_ACCOUNT_OFFSET				68			//64byte
#define MQTT_INFO_PW_OFFSET						132  		//64byte
#define MQTT_INFO_ID_OFFSET						196 		//4byte 		200byte  去掉
#define MQTT_INFO_STR_ID_OFFSET					196//200 	//24byte 		200byte
#define MQTT_INFO_STR_NUM_OFFSET				220//224	//24byte
*/

#define MQTT_INFO_URL_OFFSET 0		 // 64byte
#define MQTT_INFO_PORT_OFFSET 64	 // 4byte
#define MQTT_INFO_ACCOUNT_OFFSET 68	 // 50byte	//64byte
#define MQTT_INFO_PW_OFFSET 118		 // 50byte			//132  	//64byte
#define MQTT_INFO_STR_ID_OFFSET 168	 // 30byte			//196		//200 	//24byte 		200byte
#define MQTT_INFO_STR_NUM_OFFSET 198 // 30byte 	220	//	//224		//24byte

#endif

#define CIE_UART_TEST 0

#define ECHO_TEST_TXD 17 // 1 //(CONFIG_EXAMPLE_UART_TXD)		17
#define ECHO_TEST_RXD 16 // 3 //(CONFIG_EXAMPLE_UART_RXD)  	16
#define ECHO_TEST_RTS (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS (UART_PIN_NO_CHANGE)

#define ECHO_UART_PORT_NUM 2	   // 0	   //(CONFIG_EXAMPLE_UART_PORT_NUM)	2
#define ECHO_UART_BAUD_RATE 115200 // 512000//1000000//115200//9600//1000000//9600//115200 //(CONFIG_EXAMPLE_UART_BAUD_RATE)
#define ECHO_TASK_STACK_SIZE (CONFIG_EXAMPLE_TASK_STACK_SIZE)
#define BUF_SIZE (2500)

#define HEAD_OFFSET0 0 // 0x55
#define HEAD_OFFSET1 1 // 0xaa
#define HEAD_OFFSET2 2 // 0x03
#define CMD_WORD_OFFSET3 3
#define DATA_LEN_OFFSETH4 4
#define DATA_LEN_OFFSETL5 5
#define ID_OFFSET6 6
#define DATA_TYPE_OFFSET7 7
#define DATA_FUNC_LENH_OFFSET8 8
#define DATA_FUNC_LENL_OFFSET9 9
#define FUNC_CMD_OFFSET10 10

#define REV_WEEK_HOUR_H 10 // data[10]  + 6*WEEK  周一到周日
#define REV_WEEK_HOUR_L 11
#define REV_WEEK_MIN_H 12
#define REV_WEEK_MIN_L 13

// 以下为 与168B通信的指令
#define UART_ID_ON_OFF 0x01
#define UART_ID_WORK_MODE 0x02
#define UART_ID_TARGET_TEMP 0x10
#define UART_ID_CURRENT_TEMP 0x18 // 主动上报
#define UART_ID_GEAR 0x1C
#define UART_ID_CHILD_LOCK 0x28
#define UART_ID_WARN 0x2D // 主动上报
#define UART_ID_WEEK 0x48
#define UART_ID_TIME_ON 0x65
#define UART_ID_TIME_OFF 0x66

//==============================================
// #define UART_REPORT_WORK_MODE 		0x03	//反馈给esp32
// #define UART_REPORT_TARGET_TEMP		0x11
// #define UART_REPORT_GEAR				0x1D
// #define UART_REPORT_ON_OFF 			0x04
// #define UART_REPORT_CHILD_LOCK		0x29
// #define UART_REPORT_MESSAGE_INFO		0x73
// #define UART_REPORT_WEATHER_INFO		0x74
//=========================================

// 基础指令
#define UART_HEAT_TEST 0x30
#define UART_FIND_PRODUCTION 0x31
#define UART_FIND_MUCWORK 0x32
#define UART_REPORT_WIFI 0x33
#define UART_RESET_WIFI 0x34  // MCU主动发送
#define UART_WIFI_SELECT 0x35 // MCU主动发送
#define UART_FIND_MCUSTATE 0x36
#define UART_UPDATE_START 0x37
#define UART_UPDATE_TRANS 0x38
#define UART_GET_TIME 0x39	// MCU主动发送
#define UART_WIFI_TEST 0x3A // MCU主动发送
#define UART_WIFI_SSID 0x3B
#define UART_WIFI_PASSWORD 0x3C

#define UART_WIFI_STATE 0x3D	   // WIFI是否连接
#define UART_MQTT_STATE 0x3E	   // mqtt状态
#define UART_CLEAR_USAGE_TIME 0x3F // 清除使用时间
#define UART_RELAY_STATE 0x40

#define UART_BACKLIGHT 0x50
#define UART_IP_INFO 0x51
#define UART_WIFI_INFO 0x52

#define UART_GET_WIFI_INFO 0x60
#define UART_GET_MQTT_INFO 0x61

#define UART_CONFIRM_WIFI_INFO 0x62
#define UART_CONFIRM_MQTT_INFO 0x63

#define UART_REV_MQTT_MESSAGE_INFO 0x70
#define UART_REV_WEATHER_INFO 0x71
#define UART_CURRENT_STATE 0x72

#define UART_OTA 0x80 // 进入OTA升级指令
#define UART_MCU_FILE 0x81
#define UART_UI_FILE 0x82

#define UART_CONFIRM_FILE 0x83 // 接收数据帧id 逐帧增加

#define UART_DOWNLOAD_OVER 0x84
#define UART_UI_SIZE 0x85

typedef struct
{
	unsigned char cmd_word3;
	unsigned char data_len_h4; // 不会大于255,不会使用到
	unsigned char data_len_l5;
	unsigned char cmd_id6;
	unsigned char data_type7;
	unsigned char func_len_h8;
	unsigned char func_len_l9;
} Send_data;

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
	bool clear_usage_time;

	bool get_wifi_info;
	bool get_mqtt_info;

	bool confirm_back_light;

	bool confirm_wifi_info;
	bool confirm_mqtt_info;

	bool send_message_info;
	bool send_weather_info;

	bool send_mqtt_state;

	bool set_net_state;
	bool set_net_time;

	bool enter_OTA;
	bool send_ui_size;

} Send_168b_flag; // 需要发送指令给168B的标志位

void cie_uart_init(void);
void cie_uart_baud_9600(void);
void cie_uart_baud_1M(void);
void cie_168b_task(void *pvParameters);
void CIE_SendData_168B(void);
void cie_168B_OTA(int lenght, unsigned char *value);
void cie_download_over(unsigned char value);
#endif

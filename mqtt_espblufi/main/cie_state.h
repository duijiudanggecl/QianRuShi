#ifndef CIE_STATE_H_
#define CIE_STATE_H_

#include "cie_tool.h"
#include "mqtt_client.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"
#include "lwip/apps/sntp.h"
#include "cie_sntp.h"


typedef struct 
{
	unsigned char	week_1_hourh;		
 	unsigned char 	week_1_hourl;
 	unsigned char	week_1_minh;
 	unsigned char 	week_1_minl; 
	
	unsigned char	week_2_hourh;		
 	unsigned char 	week_2_hourl;
 	unsigned char	week_2_minh;
 	unsigned char 	week_2_minl; 

	unsigned char	week_3_hourh;		
 	unsigned char 	week_3_hourl;
 	unsigned char	week_3_minh;
 	unsigned char 	week_3_minl; 
	
	unsigned char	week_4_hourh;		
 	unsigned char 	week_4_hourl;
 	unsigned char	week_4_minh;
 	unsigned char 	week_4_minl; 

	unsigned char	week_5_hourh;		
 	unsigned char 	week_5_hourl;
 	unsigned char	week_5_minh;
 	unsigned char 	week_5_minl; 
	
	unsigned char	week_6_hourh;		
 	unsigned char 	week_6_hourl;
 	unsigned char	week_6_minh;
 	unsigned char 	week_6_minl; 

	unsigned char	week_7_hourh;		
 	unsigned char 	week_7_hourl;
 	unsigned char	week_7_minh;
 	unsigned char 	week_7_minl; 
}Week_worktime; 

typedef struct 
{
	unsigned char valid_flag;	
	unsigned char year;
	unsigned char month;
	unsigned char date;
	unsigned char hour;
	unsigned char min;
	unsigned char sec;
	unsigned char week;
}Local_Date;

typedef struct 
{
	char smartconfig;

	unsigned char test_flag;	
	int8_t test_RSSI;					//信号强度
	//char* SSID;
	char CONNECTED_SSID[32];
	char CONNECTED_PASSWORD[64];
	char SSID[32];
	char PASSWORD[64];
	//char* PASSWORD;
	unsigned char connect_flag;			//是否连上网

	unsigned char ssid_pw_change_flag;	//wifi信息发生了变更
	unsigned char mqtt_change_flag;		//mqtt信息发生了变更
}WIFI_state;

typedef struct 
{
	unsigned char OTA_flag;	//0:正常状态  >0 OTA状态

	unsigned char esp_running_state;

	unsigned char reg_value[18];	
	unsigned char send485_enable;
	unsigned char relay_state;			//bit0 :HIGH_SPEED   0:HIGH  1:LOW  
	unsigned char black_light;			//

	unsigned short int on_off;
	unsigned short int current_temperatre;
	unsigned short int set_temperatre;
	unsigned short int func_mode;		//0x0000制冷0x0001采暖0x0002通风
	unsigned short int fan_speed; 		//自动0x0000低速0x0001  中速0x0002  高速0x0003
	unsigned short int valve_onoff; 
	unsigned short int aux_valve_onoff; 
	unsigned short int lock_onoff; 
	unsigned short int min_temperatre; 
	unsigned short int max_temperature; 
	unsigned short int timing_on_flag; 
	unsigned short int timing_off_flag; 
	unsigned char clear_usage_time;
	
	//unsigned char jump_page; 
	//unsigned char beep_close; 
	//unsigned char fresh_count; 

	Week_worktime weektime;
	Local_Date local_date;
	WIFI_state wifi_state;

	unsigned char esp32_mqtt_state;					//esp32状态， 是否连接wifi,是否连接mqtt平台
	esp_mqtt_client_handle_t esp32_mqtt_client;		//WIFI_MQTT_CONNECTED	MQTT_EVENT_DISCONNECTED

	unsigned char weather_info[64];
	//unsigned char message_info[100];
	unsigned char message_info[302];
	unsigned char message_info_1[302];
	//unsigned char message_info_2[302];
	
	time_t 	beijing_time;							//开机联网获取的网络时间

	bool rev_ip_info;
	bool rev_wifi_info;
	bool static_ip;

	unsigned char rev_wifi_style; 			// 0 手动配网   1 智能配网
	unsigned char rev_wifi_ssid[32];
	unsigned char rev_wifi_pw[32];

	unsigned char cie_url[64];
	unsigned char cie_name[64];
	unsigned char MAC_ADDR[32];
	int cie_port;
	int cie_id;
	unsigned char cie_password[64];
	unsigned char cie_mqtt_id[32];	//设备id
	unsigned char cie_mqtt_num[32];	//设备编号
}UI_state;


typedef enum
{
	WIFI_DISCOUNT =	0,
	WIFI_CONNECTED_MQTT_DISCOUNT =	1,
	MQTT_COONNECTED =	2, 
}RUNNING_STATE;


typedef enum
{
	MODE_COLD 		=	0,
	MODE_HOT 		=	1,
	MODE_WIND		=	2,  		//通风
	MODE_DINUAN		=	3, 			//地暖-图标消失
	MODE_XINFENG	=	4, 			//新风
	MODE_KONGTIAO_DINUAN = 5,		//空地一体
	MODE_DINUAN_ICON = 6,			//地暖-显示图标
}FUNC_MODE;

typedef enum
{
	SPEED_AUTO 	=	0,
	SPEED_LOW	=	1,
	SPEED_MID	=	2, 
	SPEED_HIGH 	=	3,
}SPEED_MODE;

void cie_net_task(void *pvParameters);
void XGF_set_Reg_OnOff(uint16_t value);
void XGF_set_Reg_Temperature(uint16_t value);
void XGF_set_Reg_FuncMode(uint16_t value);
void XGF_set_Reg_Speed(uint16_t value);
void XGF_set_Reg_Valve(uint16_t value);
void XGF_set_Reg_AuxValve(uint16_t value);
void XGF_set_Reg_Lock(uint16_t value);
void XGF_set_Reg_MinTemp(uint16_t value);
void XGF_set_Reg_MaxTemp(uint16_t value);
void XGF_set_Reg_CurTemperature(uint16_t value);
void XGF_set_timing_off(uint16_t value);
void XGF_set_timing_on(uint16_t value);

#endif

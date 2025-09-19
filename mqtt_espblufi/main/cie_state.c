#include "cie_state.h"
#include "nvs.h"
#include "blufi_wifi_manage.h"
#include "cie_OTA.h"

static const char *TAG = "cie_state";

UI_state ui_state = {0};
extern Send_168b_flag send_168b_flag;
extern CIE_mqtt_publish_flag cie_mqtt_publish_flag;
extern unsigned char cie_wifi_info[WIFI_INFO_LENGTH];

void XGF_set_Reg_OnOff(uint16_t value)
{
	// if((ui_state.on_off != value) && (value <= ENABLE_1))
	{
		ui_state.on_off = value;
		send_168b_flag.set_switch = true;
	}
}

void XGF_set_Reg_CurTemperature(uint16_t value)
{
	if (ui_state.current_temperatre != value)
	{
		ui_state.current_temperatre = value;
	}
}

void XGF_set_Reg_Temperature(uint16_t value)
{
	ui_state.set_temperatre = value;
	send_168b_flag.set_temperatre = true;
}

void XGF_set_Reg_FuncMode(uint16_t value)
{
	// if((ui_state.func_mode != value) && (value <= MODE_WIND))
	{
		ui_state.func_mode = value;
	}
}

void XGF_set_Reg_Speed(uint16_t value)
{
	// if((ui_state.fan_speed != value) && (value <= SPEED_HIGH))
	{
		ui_state.fan_speed = value;
	}
}

void XGF_set_Reg_Valve(uint16_t value)
{
	if ((ui_state.valve_onoff != value) && (value <= ENABLE_1))
	{
		ui_state.valve_onoff = value;
	}
}

void XGF_set_Reg_AuxValve(uint16_t value)
{
	if ((ui_state.aux_valve_onoff != value) && (value <= ENABLE_1))
	{
		ui_state.aux_valve_onoff = value;
	}
}

void XGF_set_Reg_Lock(uint16_t value)
{
	ui_state.lock_onoff = value;
}

// 定时开机是否打开
void XGF_set_timing_on(uint16_t value)
{
	if ((ui_state.timing_on_flag != value) && (value <= ENABLE_1))
	{
		ui_state.timing_on_flag = value;
	}
}

// 定时关机是否打开
void XGF_set_timing_off(uint16_t value)
{
	if ((ui_state.timing_off_flag != value) && (value <= ENABLE_1))
	{
		ui_state.timing_off_flag = value;
	}
}

void XGF_set_Reg_MinTemp(uint16_t value)
{
	if (ui_state.min_temperatre != value) // && (value <= 15))
	{
		ui_state.min_temperatre = value;
	}
}

void XGF_set_Reg_MaxTemp(uint16_t value)
{
	if ((ui_state.max_temperature != value)) // && (value >= 20) && (value <= 30))
	{
		ui_state.max_temperature = value;
	}
}

void cie_net_task(void *pvParameters)
{
	static int start_on = 0;
	while (1)
	{
		if (start_on != 0)
		{
			if ((ui_state.wifi_state.connect_flag == ENABLE_1) && (ui_state.OTA_flag == 0)) // 如果已连上网，SSID有变化，则重连
			{
				int result = strcmp((const char *)ui_state.rev_wifi_ssid, (const char *)ui_state.wifi_state.CONNECTED_SSID);
				// int result = memcmp(ui_state.rev_wifi_ssid, ui_state.wifi_state.CONNECTED_SSID, 32);
				// ESP_LOGI(TAG, "connected ssid_xiangdeng:%d wifi:%s cooncted:%s ",result,ui_state.rev_wifi_ssid,ui_state.wifi_state.CONNECTED_SSID);

				if ((result != 0) && (ui_state.rev_wifi_style == 0)) // 手动配网
				{
					cie_change_ssid_password((const char *)ui_state.rev_wifi_ssid, (const char *)ui_state.rev_wifi_pw);
					ESP_LOGI(TAG, "wifi_connected  ssid changed wifi_stype_shoudong ");
				}

				if (((ui_state.static_ip == true) && (cie_wifi_info[IP_INFO_MODE_OFFSET] == 0)) ||
					((ui_state.static_ip == false) && (cie_wifi_info[IP_INFO_MODE_OFFSET] == 1)))
				// if( ui_state.static_ip != cie_wifi_info[IP_INFO_MODE_OFFSET]) //动态ip 与 静态IP  切换
				{
					cie_change_ssid_password((const char *)ui_state.rev_wifi_ssid, (const char *)ui_state.rev_wifi_pw);
					ESP_LOGI(TAG, "wifi_connected static ip changed ");
				}

				// if(((ui_state.rev_wifi_info == true) || (ui_state.wifi_state.ssid_pw_change_flag == true)) && (ui_state.rev_wifi_style == 0))
				if ((ui_state.wifi_state.ssid_pw_change_flag == true) && (ui_state.rev_wifi_style == 0)) //更换了WIFI信息
				{
					ui_state.rev_wifi_info = false;
					ui_state.wifi_state.ssid_pw_change_flag = false;
					cie_change_ssid_password((const char *)ui_state.rev_wifi_ssid, (const char *)ui_state.rev_wifi_pw);
					ESP_LOGI(TAG, "wifi_connected ssid or pw changed ");
				}
				// ESP_LOGI(TAG, "connected ssid_xiangdeng:%d wifi:%s cooncted:%s ",result,ui_state.rev_wifi_ssid,ui_state.wifi_state.CONNECTED_SSID);
			}

			else if (ui_state.wifi_state.connect_flag == DISABLE_0) // 未连上网
			{
				if ((ui_state.rev_wifi_info == true) || (ui_state.wifi_state.ssid_pw_change_flag == true))
				{
					ui_state.rev_wifi_info = false;
					ui_state.wifi_state.ssid_pw_change_flag = false;
					cie_change_ssid_password((const char *)ui_state.rev_wifi_ssid, (const char *)ui_state.rev_wifi_pw);
					ESP_LOGI(TAG, "wifi_not_connected has ssid password ");
				}
				else if (start_on == 9)
				{
					send_168b_flag.get_wifi_info = true; // 获取168B WIFI信息
				}
			}
		}

		start_on++;
		if (start_on >= 10)
			start_on = 1;
		// ESP_LOGI(TAG, "cie_net_task...");
		vTaskDelay(pdMS_TO_TICKS(10000));
	}
}
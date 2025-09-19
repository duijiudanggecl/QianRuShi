#include "cie_uart.h"
#include "cie_state.h"
#include "cie_pwm.h"
#include "cie_OTA.h"
#include "cie_esp_OTA.h"

unsigned char cie_mqtt_info[MQTT_INFO_LENGTH] = {0};
unsigned char cie_wifi_info[WIFI_INFO_LENGTH] = {0};

unsigned char Send_buf[2500] = {0x00};
unsigned char Func_data[2500] = {0x00};

unsigned char mqtt_state[13] = {0x00};

const unsigned char SCI_Start_1 = 0x55;
const unsigned char SCI_Start_2 = 0xAA;
const unsigned char SCI_Start_Send_3 = 0x00;
const unsigned char SCI_Start_Rev_3 = 0x03;

static const char *TAG = "cie_uart";

static unsigned char data[256] = {0};

Send_data send_data;
Send_168b_flag send_168b_flag;
extern CIE_mqtt_publish_flag cie_mqtt_publish_flag;
extern UI_state ui_state;
extern int ui_size_total;

static void XGF_SendData_init()
{
	Send_buf[HEAD_OFFSET0] = 0x55;
	Send_buf[HEAD_OFFSET1] = 0xAA;
	Send_buf[HEAD_OFFSET2] = 0x00;
	Send_buf[CMD_WORD_OFFSET3] = 0x06; // send_data.cmd_word;
}

void cie_uart_baud_9600(void)
{
	uart_config_t uart_config = {
		.baud_rate = 9600,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
		.source_clk = UART_SCLK_DEFAULT,
	};
	uart_param_config(ECHO_UART_PORT_NUM, &uart_config);
}

void cie_uart_baud_1M(void)
{
	uart_config_t uart_config = {
		.baud_rate = 1000000,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
		.source_clk = UART_SCLK_DEFAULT,
	};
	uart_param_config(ECHO_UART_PORT_NUM, &uart_config);
}

void cie_uart_init(void)
{
	uart_config_t uart_config = {
		.baud_rate = ECHO_UART_BAUD_RATE,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
		.source_clk = UART_SCLK_DEFAULT,
	};
	int intr_alloc_flags = 0;

	// uart_driver_install(ECHO_UART_PORT_NUM, BUF_SIZE, 0, 0, NULL, 0);  // 默认启用 DMA
	// uart_param_config(ECHO_UART_PORT_NUM, &uart_config);

	// ESP_ERROR_CHECK(uart_driver_install(ECHO_UART_PORT_NUM, BUF_SIZE, 0, 0, NULL, 0));
	// ESP_ERROR_CHECK(uart_param_config(ECHO_UART_PORT_NUM, &uart_config));

	ESP_ERROR_CHECK(uart_driver_install(ECHO_UART_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
	ESP_ERROR_CHECK(uart_param_config(ECHO_UART_PORT_NUM, &uart_config));
	ESP_ERROR_CHECK(uart_set_pin(ECHO_UART_PORT_NUM, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS));

	XGF_SendData_init();

	// Configure a temporary buffer for the incoming data
	// char *data = (char *) malloc(BUF_SIZE);
	return;
}

void CIE_SendData_168B(void)
{
	if ((send_data.data_len_h4 * 256 + send_data.data_len_l5) < 5)
	{
		return;
	}

	UINT16 func_cmd_len = send_data.func_len_h8 * 256 + send_data.func_len_l9;
	UINT16 total_len = send_data.data_len_h4 * 256 + send_data.data_len_l5 + 7; // 不包含crc
	UINT8 crc_val = 0x00;

	Send_buf[DATA_LEN_OFFSETH4] = send_data.data_len_h4;
	Send_buf[DATA_LEN_OFFSETL5] = send_data.data_len_l5;
	Send_buf[ID_OFFSET6] = send_data.cmd_id6;
	Send_buf[DATA_TYPE_OFFSET7] = send_data.data_type7;
	Send_buf[DATA_FUNC_LENH_OFFSET8] = send_data.func_len_h8;
	Send_buf[DATA_FUNC_LENL_OFFSET9] = send_data.func_len_l9;

	for (UINT16 func_index = 0; func_index < func_cmd_len; func_index++)
	{
		Send_buf[FUNC_CMD_OFFSET10 + func_index] = Func_data[func_index];
	}

	crc_val = CRC16_addition(Send_buf, total_len - 1); // 校验
	Send_buf[total_len - 1] = crc_val;

	// uart_write_bytes_with_dma(ECHO_UART_PORT_NUM, Send_buf, total_len);
	uart_write_bytes(ECHO_UART_PORT_NUM, (const unsigned char *)Send_buf, total_len);

#if 0 // CIE_UART_TEST
		if (total_len)
		{
			ESP_LOGI(TAG, "SEND data (hex): len:%d",total_len);
			for (int i = 0; i < total_len; i++)
			{
				ESP_LOGI(TAG, "%02x ", (unsigned char)Send_buf[i]); // 打印每个字节的十六进制值
			}
			ESP_LOGI(TAG, "over"); // 换行以美化输出
		}
#endif
	ESP_LOGI(TAG, "send id: 0x%x", Send_buf[ID_OFFSET6]);
	// ESP_LOGI(TAG, "send data: %s", (unsigned char *)Send_buf);
}

static void XGF_Receive_esp_handler(unsigned char *buf, unsigned char length)
{
	unsigned char data[256] = {0};
	unsigned char id = 0;
	static unsigned char sta_rev_wifi = 0;
	static unsigned char sta_rev_mqtt = 0;

	// static unsigned char heat = 0;
	memcpy(data, buf, length);

	send_data.data_len_h4 = data[DATA_LEN_OFFSETH4];
	send_data.data_len_l5 = data[DATA_LEN_OFFSETL5];
	send_data.cmd_id6 = data[ID_OFFSET6];
	send_data.data_type7 = data[DATA_TYPE_OFFSET7];
	send_data.func_len_h8 = data[DATA_FUNC_LENH_OFFSET8];
	send_data.func_len_l9 = data[DATA_FUNC_LENL_OFFSET9];
	id = send_data.cmd_id6;
	ESP_LOGI(TAG, "XGF_ReceiveCmd   id: 0x%x \r\n", id);
	switch (id)
	{
	case UART_OTA:
		ui_state.OTA_flag = data[FUNC_CMD_OFFSET10];
		ESP_LOGI(TAG, "ui_state.OTA_flag: %d \r\n", ui_state.OTA_flag); // 为1，则表示168b已进入boot模式

		if (ui_state.OTA_flag == OTA_NORMAL) // 接收168B 开始升级指令
		{
			vTaskDelay(500);
			esp_restart();
		}

		if ((ui_state.wifi_state.connect_flag == DISABLE_0) && (ui_state.OTA_flag > OTA_start))
			return;

		if (ui_state.OTA_flag == OTA_start) // 接收168B 开始升级指令
		{
			// ui_state.OTA_flag = OTA_MCU;	//告知168B 开始升级MCU
			vTaskDelay(500);
			send_168b_flag.enter_OTA = true;
		}

		else if (ui_state.OTA_flag == OTA_MCU) // 168B 进入升级MCU升级状态
		{
			cie_pwm_update(100);
			ui_state.black_light = 100;
			cie_OTA(Download_MCU);
		}

		else if (ui_state.OTA_flag == OTA_UI) // 168B 进入UI升级状态
		{
			cie_OTA(Download_UI);
		}

		else if (ui_state.OTA_flag == OTA_ESP32) // 168B 进入ESP32升级状态
		{
			cie_esp_OTA();
		}

		break;

	case UART_CONFIRM_FILE:
		// rev_OTA_file = 1;
		// OTA_file_frame = (data[FUNC_CMD_OFFSET10] << 24) |
		//				 (data[FUNC_CMD_OFFSET10 + 1] << 16) |
		//				 (data[FUNC_CMD_OFFSET10 + 2] << 8) |
		//				 (data[FUNC_CMD_OFFSET10 + 3]);
		break;

	case UART_ID_CURRENT_TEMP:

		break;

	case UART_ID_WARN: //	不用回复

		break;

	case UART_RESET_WIFI: // 需要回复

		break;

	case UART_WIFI_SELECT: // 需要回复

		break;

	case UART_GET_TIME: // 需要回复

		break;

	case UART_WIFI_TEST: // 需要回复

		break;

	case UART_ID_ON_OFF:
		mqtt_state[MQTT_STATE_SWITCH_6] = data[FUNC_CMD_OFFSET10];
		ui_state.on_off = data[FUNC_CMD_OFFSET10];
		cie_mqtt_publish_flag.set_switch = true;
		break;

	case UART_WIFI_SSID:
		// Func_data[0] = data[FUNC_CMD_OFFSET10];
		//  XGF_set_Reg_OnOff(Func_data[0]);
		cie_clear_wifi_ssid_pw();

		for (UINT16 func_index = 0; func_index < 32; func_index++)
		{
			ui_state.wifi_state.SSID[func_index] = data[FUNC_CMD_OFFSET10 + func_index]; // ssid
		}
		break;

	case UART_WIFI_PASSWORD:
		// Func_data[0] = data[FUNC_CMD_OFFSET10];
		//  XGF_set_Reg_OnOff(Func_data[0]);
		for (UINT16 func_index = 0; func_index < 32; func_index++)
		{
			ui_state.wifi_state.PASSWORD[func_index] = data[FUNC_CMD_OFFSET10 + func_index]; // ssid
		}

		// cie_reconnect_smart_config();
		break;

	case UART_ID_WORK_MODE:
		mqtt_state[MQTT_STATE_MODE_4] = data[FUNC_CMD_OFFSET10];
		Func_data[0] = data[FUNC_CMD_OFFSET10];
		XGF_set_Reg_FuncMode(Func_data[0]);
		cie_mqtt_publish_flag.set_mode = true;
		break;

	case UART_ID_TARGET_TEMP:
		mqtt_state[MQTT_STATE_TEMP_SET_2] = data[FUNC_CMD_OFFSET10 + 2];
		mqtt_state[MQTT_STATE_TEMP_SET_3] = data[FUNC_CMD_OFFSET10 + 3];
		// Func_data[3] = data[FUNC_CMD_OFFSET10 + 3];
		ui_state.set_temperatre = mqtt_state[MQTT_STATE_TEMP_SET_2] * 256 + mqtt_state[MQTT_STATE_TEMP_SET_3];
		// XGF_set_Reg_Temperature(Func_data[3]);
		cie_mqtt_publish_flag.set_temperatre = true;
		break;

		// case UART_ID_CURRENT_TEMP:
		// break;

	case UART_ID_GEAR:
		mqtt_state[MQTT_STATE_SPEED_5] = data[FUNC_CMD_OFFSET10];
		Func_data[0] = data[FUNC_CMD_OFFSET10];
		XGF_set_Reg_Speed(Func_data[0]);
		cie_mqtt_publish_flag.set_gear = true;
		break;

	case UART_RELAY_STATE:
		mqtt_state[RELAY_STATE_12] = data[FUNC_CMD_OFFSET10];
		cie_mqtt_publish_flag.report_relay_state = true;
		break;

	case UART_CURRENT_STATE:
		memcpy(mqtt_state, data + FUNC_CMD_OFFSET10, sizeof(mqtt_state));
		cie_mqtt_publish_flag.dev_information = true;
		break;

	case UART_ID_CHILD_LOCK:
		mqtt_state[MQTT_STATE_LOCK_7] = data[FUNC_CMD_OFFSET10];
		cie_mqtt_publish_flag.set_child_lock = true;
		break;

		// case UART_ID_CHILD_LOCK:
		// Func_data[0] = data[FUNC_CMD_OFFSET10];
		// XGF_set_Reg_Lock(Func_data[0]);
		// break;

		// case UART_ID_WARN:
		// break;

		// case UART_ID_WEEK:
		//	memcpy(&ui_state.weektime, &data[FUNC_CMD_OFFSET10],send_data.func_len_l9);	//更新周数据
		//	memcpy(Func_data, &ui_state.weektime, send_data.func_len_l9);	//更新发送数据

		// break;

		// case UART_ID_TIME_ON:
		// Func_data[0] = data[FUNC_CMD_OFFSET10];
		// XGF_set_timing_on(Func_data[0]);
		// break;

		// case UART_ID_TIME_OFF:
		// Func_data[0] = data[FUNC_CMD_OFFSET10];
		// XGF_set_timing_off(Func_data[0]);
		// break;

		// case UART_HEAT_TEST:
		// Func_data[0] = heat;
		// if(heat == 0)
		//	heat = 1;
		// break;

		// case UART_FIND_PRODUCTION:
		//	send_data.func_len_l9 = 0x2a + 4;

		// break;

		// case UART_FIND_MUCWORK:

		// break;

		// case UART_REPORT_WIFI:
		//	Func_data[0] = data[FUNC_CMD_OFFSET10];
		// break;

		// case UART_FIND_MCUSTATE:
		//	Func_data[0] = 0;	//返回工作状态，待定
		// break;

		// case UART_UPDATE_START:

		// break;

		// case UART_UPDATE_TRANS:

		// break;

		/*
				case UART_GET_TIME:
					if(Func_data[0] == ENABLE_1)
					{
						ui_state.local_date.valid_flag = Func_data[0];
						ui_state.local_date.year = Func_data[1];
						ui_state.local_date.month = Func_data[2];
						ui_state.local_date.date = Func_data[3];
						ui_state.local_date.hour = Func_data[4];
						ui_state.local_date.min	= Func_data[5];
						ui_state.local_date.sec = Func_data[6];
						ui_state.local_date.week = Func_data[7];
					}
					return;

				break;

				case UART_WIFI_TEST:
					ui_state.wifi_state.test_flag = Func_data[0];
					ui_state.wifi_state.test_RSSI = Func_data[1];
					return;
				break;
		*/

	case UART_BACKLIGHT:
		// ESP_LOGI(TAG, "UART_BACKLIGHT   backlight: 0x%x \r\n", data[FUNC_CMD_OFFSET10]);
		if (data[FUNC_CMD_OFFSET10] <= 100)
		{
			cie_pwm_update(data[FUNC_CMD_OFFSET10]);
			ui_state.black_light = data[FUNC_CMD_OFFSET10];
			// CIE_SendData_168B(); //反馈背光给给168B
			send_168b_flag.confirm_back_light = true;
		}
		break;

	case UART_IP_INFO:
		send_168b_flag.confirm_mqtt_info = true;
		ESP_LOGI(TAG, "get mqtt information  \r\n");
		memcpy(cie_mqtt_info, data + FUNC_CMD_OFFSET10, MQTT_INFO_LENGTH); // 得到mqtt信息

		ui_state.cie_port = (cie_mqtt_info[MQTT_INFO_PORT_OFFSET] << 24) |
							(cie_mqtt_info[MQTT_INFO_PORT_OFFSET + 1] << 16) |
							(cie_mqtt_info[MQTT_INFO_PORT_OFFSET + 2] << 8) |
							(cie_mqtt_info[MQTT_INFO_PORT_OFFSET + 3]);

		// ui_state.cie_id = (cie_mqtt_info[MQTT_INFO_ID_OFFSET] << 24) |
		//				  (cie_mqtt_info[MQTT_INFO_ID_OFFSET + 1] << 16) |
		//				  (cie_mqtt_info[MQTT_INFO_ID_OFFSET + 2] << 8) |
		//				  (cie_mqtt_info[MQTT_INFO_ID_OFFSET + 3]);

		memcpy(ui_state.cie_url, cie_mqtt_info + MQTT_INFO_URL_OFFSET, 64);
		memcpy(ui_state.cie_name, cie_mqtt_info + MQTT_INFO_ACCOUNT_OFFSET, 60);
		memcpy(ui_state.cie_password, cie_mqtt_info + MQTT_INFO_PW_OFFSET, 60);

		memcpy(ui_state.cie_mqtt_id, cie_mqtt_info + MQTT_INFO_STR_ID_OFFSET, 30);
		memcpy(ui_state.cie_mqtt_num, cie_mqtt_info + MQTT_INFO_STR_NUM_OFFSET, 30);

		ESP_LOGI(TAG, "ui_state.cie_mqtt_id: %s \r\n", ui_state.cie_mqtt_id);
		ESP_LOGI(TAG, "ui_state.cie_mqtt_num: %s \r\n", ui_state.cie_mqtt_num);

		ESP_LOGI(TAG, "ui_state.cie_port: %d \r\n", ui_state.cie_port);
		ESP_LOGI(TAG, "ui_state.cie_id: %d \r\n", ui_state.cie_id);
		ESP_LOGI(TAG, "ui_state.cie_url: %s \r\n", ui_state.cie_url);
		ESP_LOGI(TAG, "ui_state.cie_name : %s \r\n", ui_state.cie_name);
		ESP_LOGI(TAG, "ui_state.cie_password: %s \r\n", ui_state.cie_password);

		if (sta_rev_mqtt == 0)
			ui_state.rev_ip_info = true; // 第一次接收
		else
			ui_state.wifi_state.mqtt_change_flag = true; // mqtt信息有更改

		sta_rev_mqtt = 1;
		break;

	case UART_WIFI_INFO:
		send_168b_flag.confirm_wifi_info = true;
		ESP_LOGI(TAG, "get wifi information  \r\n");
		memcpy(cie_wifi_info, data + FUNC_CMD_OFFSET10, WIFI_INFO_LENGTH);

#if 0 // CIE_UART_TEST
		if (WIFI_INFO_LENGTH)
		{
			ESP_LOGI(TAG, "SEND data (hex):");
			for (int i = 0; i < WIFI_INFO_LENGTH; i++)
			{
				ESP_LOGI(TAG, "%02x ", (unsigned char)cie_wifi_info[i]); // 打印每个字节的十六进制值
			}
			ESP_LOGI(TAG, "over"); // 换行以美化输出
		}
#endif

		ui_state.rev_wifi_style = cie_wifi_info[WIFI_INFO_SMART_OFFSET];
		memcpy(ui_state.rev_wifi_ssid, cie_wifi_info + WIFI_INFO_SSID_OFFSET, 32);
		memcpy(ui_state.rev_wifi_pw, cie_wifi_info + WIFI_INFO_PW_OFFSET, 32);

		ESP_LOGI(TAG, "static_ip: %d \r\n", cie_wifi_info[IP_INFO_MODE_OFFSET]);
		ESP_LOGI(TAG, "ui_state.rev_wifi_style: %d \r\n", ui_state.rev_wifi_style);
		ESP_LOGI(TAG, "ui_state.rev_wifi_ssid: %s \r\n", ui_state.rev_wifi_ssid);
		ESP_LOGI(TAG, "ui_state.rev_wifi_pw: %s \r\n", ui_state.rev_wifi_pw);

		if (sta_rev_wifi == 0)
			ui_state.rev_wifi_info = true; // 第一次接收
		else
			ui_state.wifi_state.ssid_pw_change_flag = true; // 信息有更改

		sta_rev_wifi = 1;
		break;

	default:

	break;
	
	}

	// CIE_SendData_168B();
}

static void cie_ReceiveCmd_168B(unsigned char *buf, unsigned char length)
{
	unsigned char data[256] = {0};
	// static unsigned char heat = 0;
	unsigned char rxCrc, mcuCrc;

	ESP_LOGI(TAG, "XGF_ReceiveCmd   len: %d \r\n", length);

	memcpy(data, buf, length);
	if ((data[DATA_LEN_OFFSETL5] + 7) != length) // 长度判别
		return;

	if ((data[0] != SCI_Start_1) || (data[1] != SCI_Start_2) || (data[2] != SCI_Start_Rev_3) || (data[3] != 0x07))
		return;

	rxCrc = buf[length - 1];
	mcuCrc = CRC16_addition(buf, length - 1);

	// ESP_LOGI(TAG,"XGF_ReceiveCmd   cxCrc: %d mcuCrc:%d \r\n",rxCrc,mcuCrc);

	if (mcuCrc == rxCrc)
	{
		XGF_Receive_esp_handler(buf, length);
	}
}

static void cie_send_data(void)
{
	if (send_168b_flag.send_message_info == true)
		return;

	if (send_168b_flag.get_wifi_info == true)
	{
		send_168b_flag.get_wifi_info = false;
		send_data.data_len_h4 = 0;
		send_data.data_len_l5 = 5;
		send_data.cmd_id6 = UART_GET_WIFI_INFO;
		send_data.data_type7 = 1;
		send_data.func_len_h8 = 0;
		send_data.func_len_l9 = 1;
		Func_data[0] = 0;
		CIE_SendData_168B();
	}

	else if (send_168b_flag.get_mqtt_info == true)
	{
		send_168b_flag.get_mqtt_info = false;
		send_data.data_len_h4 = 0;
		send_data.data_len_l5 = 5;
		send_data.cmd_id6 = UART_GET_MQTT_INFO;
		send_data.data_type7 = 1;
		send_data.func_len_h8 = 0;
		send_data.func_len_l9 = 1;
		Func_data[0] = 0;
		CIE_SendData_168B();
	}

	else if (send_168b_flag.send_message_info == true)
	{
		send_168b_flag.send_message_info = false;
		send_data.data_len_h4 = (4 + sizeof(ui_state.message_info)) / 256;
		send_data.data_len_l5 = (4 + sizeof(ui_state.message_info)) % 256;
		send_data.cmd_id6 = UART_REV_MQTT_MESSAGE_INFO;
		send_data.data_type7 = 1;
		send_data.func_len_h8 = (sizeof(ui_state.message_info)) / 256;
		send_data.func_len_l9 = (sizeof(ui_state.message_info)) % 256;
		// Func_data[0] = 0;
		memcpy(Func_data, ui_state.message_info, sizeof(ui_state.message_info));
		CIE_SendData_168B();
	}

	else if (send_168b_flag.send_weather_info == true)
	{
		send_168b_flag.send_weather_info = false;
		send_data.data_len_h4 = 0;
		send_data.data_len_l5 = 4 + sizeof(ui_state.weather_info);
		send_data.cmd_id6 = UART_REV_WEATHER_INFO;
		send_data.data_type7 = 1;
		send_data.func_len_h8 = 0;
		send_data.func_len_l9 = sizeof(ui_state.weather_info);
		// Func_data[0] = 0;
		memcpy(Func_data, ui_state.weather_info, sizeof(ui_state.weather_info));
		CIE_SendData_168B();
	}

	else if (send_168b_flag.set_temperatre == true)
	{
		send_168b_flag.set_temperatre = false;
		send_data.data_len_h4 = 0;
		send_data.data_len_l5 = 8;
		send_data.cmd_id6 = UART_ID_TARGET_TEMP;
		send_data.data_type7 = 2;
		send_data.func_len_h8 = 0;
		send_data.func_len_l9 = 4;
		Func_data[0] = 0;
		Func_data[1] = 0;
		Func_data[2] = ui_state.set_temperatre / 256;
		Func_data[3] = ui_state.set_temperatre & 0xff;

		CIE_SendData_168B();
	}

	else if (send_168b_flag.confirm_back_light == true)
	{
		send_168b_flag.confirm_back_light = false;
		send_data.data_len_h4 = 0;
		send_data.data_len_l5 = 5;
		send_data.cmd_id6 = UART_BACKLIGHT;
		send_data.data_type7 = 2;
		send_data.func_len_h8 = 0;
		send_data.func_len_l9 = 1;
		Func_data[0] = ui_state.black_light;

		CIE_SendData_168B();
	}

	else if (send_168b_flag.set_net_time == true)
	{
		send_168b_flag.set_net_time = false;
		send_data.data_len_h4 = 0;
		send_data.data_len_l5 = 12;
		send_data.cmd_id6 = UART_GET_TIME;
		send_data.data_type7 = 1;
		send_data.func_len_h8 = 0;
		send_data.func_len_l9 = 8;

		// struct tm *beijing_tm = localtime(&ui_state.beijing_time);
		Func_data[0] = 1;
		Func_data[1] = ui_state.local_date.year;
		Func_data[2] = ui_state.local_date.month;
		Func_data[3] = ui_state.local_date.date;
		Func_data[4] = ui_state.local_date.hour;
		Func_data[5] = ui_state.local_date.min;
		Func_data[6] = ui_state.local_date.sec;
		Func_data[7] = 0;

		CIE_SendData_168B();
	}

	else if (send_168b_flag.set_net_state == true)
	{
		send_168b_flag.set_net_state = false;
		send_data.data_len_h4 = 0;
		send_data.data_len_l5 = 6;
		send_data.cmd_id6 = UART_WIFI_STATE;
		send_data.data_type7 = 1;
		send_data.func_len_h8 = 0;
		send_data.func_len_l9 = 2;
		Func_data[0] = ui_state.wifi_state.connect_flag;
		Func_data[1] = ui_state.black_light;
		CIE_SendData_168B();
	}

	else if (send_168b_flag.send_mqtt_state == true)
	{
		send_168b_flag.send_mqtt_state = false;
		send_data.data_len_h4 = 0;
		send_data.data_len_l5 = 5;
		send_data.cmd_id6 = UART_MQTT_STATE;
		send_data.data_type7 = 1;
		send_data.func_len_h8 = 0;
		send_data.func_len_l9 = 1;
		Func_data[0] = ui_state.esp32_mqtt_state;

		CIE_SendData_168B();
	}

	else if (send_168b_flag.set_mode == true)
	{
		send_168b_flag.set_mode = false;
		send_data.data_len_h4 = 0;
		send_data.data_len_l5 = 5;
		send_data.cmd_id6 = UART_ID_WORK_MODE;
		send_data.data_type7 = 1;
		send_data.func_len_h8 = 0;
		send_data.func_len_l9 = 1;
		Func_data[0] = ui_state.func_mode;
		CIE_SendData_168B();
	}

	else if (send_168b_flag.enter_OTA == true)
	{
		send_168b_flag.enter_OTA = false;
		send_data.data_len_h4 = 0;
		send_data.data_len_l5 = 5;
		send_data.cmd_id6 = UART_OTA;
		send_data.data_type7 = 1;
		send_data.func_len_h8 = 0;
		send_data.func_len_l9 = 1;
		Func_data[0] = ui_state.OTA_flag; // ui_state.func_mode;
		CIE_SendData_168B();

		// if(ui_state.OTA_flag == OTA_NORMAL)
		//	cie_uart_baud_1M();
	}

	else if (send_168b_flag.set_gear == true)
	{
		send_168b_flag.set_gear = false;
		send_data.data_len_h4 = 0;
		send_data.data_len_l5 = 5;
		send_data.cmd_id6 = UART_ID_GEAR;
		send_data.data_type7 = 1;
		send_data.func_len_h8 = 0;
		send_data.func_len_l9 = 1;
		Func_data[0] = ui_state.fan_speed;
		CIE_SendData_168B();
	}

	else if (send_168b_flag.set_switch == true)
	{
		send_168b_flag.set_switch = false;
		send_data.data_len_h4 = 0;
		send_data.data_len_l5 = 5;
		send_data.cmd_id6 = UART_ID_ON_OFF;
		send_data.data_type7 = 1;
		send_data.func_len_h8 = 0;
		send_data.func_len_l9 = 1;
		Func_data[0] = ui_state.on_off;
		CIE_SendData_168B();
	}

	else if (send_168b_flag.set_child_lock == true)
	{
		send_168b_flag.set_child_lock = false;
		send_data.data_len_h4 = 0;
		send_data.data_len_l5 = 5;
		send_data.cmd_id6 = UART_ID_CHILD_LOCK;
		send_data.data_type7 = 1;
		send_data.func_len_h8 = 0;
		send_data.func_len_l9 = 1;
		Func_data[0] = ui_state.lock_onoff;
		CIE_SendData_168B();
	}

	else if (send_168b_flag.clear_usage_time == true)
	{
		send_168b_flag.clear_usage_time = false;
		send_data.data_len_h4 = 0;
		send_data.data_len_l5 = 5;
		send_data.cmd_id6 = UART_CLEAR_USAGE_TIME;
		send_data.data_type7 = 1;
		send_data.func_len_h8 = 0;
		send_data.func_len_l9 = 1;
		Func_data[0] = ui_state.clear_usage_time;
		CIE_SendData_168B();
	}

	else if (send_168b_flag.confirm_wifi_info == true)
	{
		send_168b_flag.confirm_wifi_info = false;
		send_data.data_len_h4 = 0;
		send_data.data_len_l5 = 5;
		send_data.cmd_id6 = UART_CONFIRM_WIFI_INFO;
		send_data.data_type7 = 1;
		send_data.func_len_h8 = 0;
		send_data.func_len_l9 = 1;
		Func_data[0] = 0;
		CIE_SendData_168B();
	}

	else if (send_168b_flag.confirm_mqtt_info == true)
	{
		send_168b_flag.confirm_mqtt_info = false;
		send_data.data_len_h4 = 0;
		send_data.data_len_l5 = 5;
		send_data.cmd_id6 = UART_CONFIRM_MQTT_INFO;
		send_data.data_type7 = 1;
		send_data.func_len_h8 = 0;
		send_data.func_len_l9 = 1;
		Func_data[0] = 0;
		CIE_SendData_168B();
	}
}

// OTA传输数据
void cie_168B_OTA(int lenght, unsigned char *value)
{
	send_data.data_len_h4 = (lenght + 4) / 256;
	send_data.data_len_l5 = (lenght + 4) % 256;

	if (ui_state.OTA_flag == OTA_MCU)
	{
		send_data.cmd_id6 = UART_MCU_FILE;
	}
	else if (ui_state.OTA_flag == OTA_UI)
	{
		send_data.cmd_id6 = UART_UI_FILE;
	}

	send_data.data_type7 = 1;
	send_data.func_len_h8 = lenght / 256;
	send_data.func_len_l9 = lenght % 256;
	// Func_data[0] = value;
	memcpy(Func_data, value, lenght);
	CIE_SendData_168B();
}

void cie_download_over(unsigned char value)
{
	send_data.data_len_h4 = 0;
	send_data.data_len_l5 = 5;
	send_data.cmd_id6 = UART_DOWNLOAD_OVER;
	send_data.data_type7 = 1;
	send_data.func_len_h8 = 0;
	send_data.func_len_l9 = 1;
	Func_data[0] = value; // 0 MCU  1 UI 3 eps32
	CIE_SendData_168B();
}

// 发送开关指令
void cie_power_onoff(ON_OFF value)
{
	send_data.data_len_h4 = 0;
	send_data.data_len_l5 = 5;
	send_data.cmd_id6 = UART_ID_ON_OFF;
	send_data.data_type7 = 1;
	send_data.func_len_h8 = 0;
	send_data.func_len_l9 = 1;
	Func_data[0] = value;
	CIE_SendData_168B();
}

// 此任务用于与168B进行通信
void cie_168b_task(void *pvParameters)
{
	static unsigned char heart_count = 0;
	int max_heart_count = 10;
	while (1)
	{
		// Read data from the UART
		int len = uart_read_bytes(ECHO_UART_PORT_NUM, data, (BUF_SIZE - 1), 100 / portTICK_PERIOD_MS); 	//20=>100
		if (len > 0)
		{
			cie_ReceiveCmd_168B(data, len);
			memset(data, 0, len);
		}
		// memset(data, 0, len);

#if CIE_UART_TEST
		if (len)
		{
			ESP_LOGI(TAG, "Received data (hex):");
			for (int i = 0; i < len; i++)
			{
				ESP_LOGI(TAG, "%d : %02x ", i, (unsigned char)data[i]); // 打印每个字节的十六进制值
			}
			ESP_LOGI(TAG, ""); // 换行以美化输出
		}
#endif

		heart_count++;
		if ((heart_count >= max_heart_count) || (heart_count == 0))
		{
			heart_count = 0;
			if (max_heart_count <= 150) // 30S
				max_heart_count += 10;

			send_168b_flag.set_net_state = true;
		}

		if ((ui_state.OTA_flag == OTA_MCU) || (ui_state.OTA_flag == OTA_UI) || (ui_state.OTA_flag == OTA_ESP32)) // 传输文件的过程中，不发送其他指令
		{
			if (send_168b_flag.enter_OTA == true)
			{
				send_168b_flag.enter_OTA = false;
				send_data.data_len_h4 = 0;
				send_data.data_len_l5 = 5;
				send_data.cmd_id6 = UART_OTA;
				send_data.data_type7 = 1;
				send_data.func_len_h8 = 0;
				send_data.func_len_l9 = 1;
				Func_data[0] = ui_state.OTA_flag;
				CIE_SendData_168B();
			}
			/*
						else if (send_168b_flag.send_ui_size == true)
						{
							send_168b_flag.send_ui_size = false;
							send_data.data_len_h4 = 0;
							send_data.data_len_l5 = 8;
							send_data.cmd_id6 = UART_UI_SIZE;
							send_data.data_type7 = 1;
							send_data.func_len_h8 = 0;
							send_data.func_len_l9 = 4;
							Func_data[0] = (ui_size_total >> 24)&&0xff;
							Func_data[1] = (ui_size_total >> 16)&&0xff;
							Func_data[2] = (ui_size_total >> 8)&&0xff;
							Func_data[3] = (ui_size_total)&&0xff;

							ESP_LOGI(TAG, "send File size: %d bytes", ui_size_total);

							CIE_SendData_168B();
						}
			*/
		}

		else
		{
			if (ui_state.OTA_flag == OTA_start)
				ui_state.OTA_flag = OTA_MCU;

			cie_send_data();
		}

		// cie_send_data();
		// ESP_LOGE(TAG, "cie_168b_task...");
		vTaskDelay(pdMS_TO_TICKS(200));
	}
}

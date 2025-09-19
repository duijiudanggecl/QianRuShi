
#ifndef CIE_TOOL_H_
#define CIE_TOOL_H_

#include <string.h>
#include <stdlib.h>
#include "esp_wifi.h"
#include "esp_eap_client.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_mac.h"
#include "cie_wifi.h"
#include "cie_connect.h"
#include "cie_state.h"
#include "cie_connect.h"

typedef enum
{
	DISABLE_0 	=	0,
	ENABLE_1	=	1, 
}ON_OFF;


typedef unsigned long long UINT64;
typedef unsigned int UINT32;
typedef unsigned short int UINT16;
typedef unsigned char UINT8;

UINT16 utf8_TO_utf16(UINT8 *in, UINT16 inLen, UINT16 *out, UINT16 outLen);
char CRC16_addition(unsigned char *pData,UINT16 length);
void cie_read_param_from_flash(void);
void cie_reconnect_smart_config(void);
void cie_clear_wifi_ssid_pw(void);
void cie_clear_SSID_PW_from_flash(void);
void cie_write_smartconfig_disable_from_flash(void);
void cie_write_smartconfig_enable_from_flash(void);
void cie_write_server_url_to_flash(void);
#endif

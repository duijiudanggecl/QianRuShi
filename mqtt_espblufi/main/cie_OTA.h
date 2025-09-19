#ifndef CIE_OTA_H_
#define CIE_OTA_H_

enum OTA_state 
{  
	OTA_NORMAL = 0,
	OTA_start = 1,
	OTA_MCU = 2,
	OTA_UI = 3,
	OTA_ESP32 = 4,
	OTA_SUCCESS = 5,
	OTA_FAIL = 6
};

enum Download_file 
{  
	Download_MCU = 0,
	Download_UI = 1
	//Download_ESP32 = 2
};

void cie_OTA(uint8_t value);

#endif
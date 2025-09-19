#ifndef CIE_MQTT_HANDLE_H_
#define CIE_MQTT_HANDLE_H_

#include "cJSON.h"
#include "cie_mqtt.h"
#include "cie_uart.h"

void handle_mqtt_rev_data(const char *data);
int CIE_publish(const char *publish_func);
void CIE_power_on_subscribe(void);
#endif
/*
 * gpio_iic.h
 *
 *  Created on: 2024年5月30日
 *  Author: swiftchen
 */

#ifndef GPIO_IIC_H_
#define GPIO_IIC_H_

#include <stdint.h>


#define GPIO_SCL_OUTPUT      0//I2C->U8_I2CPCR.SCLPA = 1;I2C->U8_I2CDDR.SCL = 1;		EPORT1->EPDDR  0:output  1:input 
#define GPIO_SCL_PULLUP      0//I2C->U8_I2CPCR.PUI2C |= (1<<0)       				EPORT1->EPPUE 上拉寄存器
#define GPIO_SCL_HIGH        0//I2C->U8_I2CPDR.SCL = 1								EPORT1->EPDR  output_data
#define GPIO_SCL_LOW         0//I2C->U8_I2CPDR.SCL = 0

#define GPIO_SDA_OUTPUT      0//I2C->U8_I2CPCR.SDAPA = 1;I2C->U8_I2CDDR.SDA = 1;
#define GPIO_SDA_INPUT       0//I2C->U8_I2CPCR.SDAPA = 1;I2C->U8_I2CDDR.SDA = 0;
#define GPIO_SDA_PULLUP      0//I2C->U8_I2CPCR.PUI2C |= (1<<1)
#define GPIO_GetSDABit       0//I2C->U8_I2CPDR.SDA
#define GPIO_SDA_HIGH        0//I2C->U8_I2CPDR.SDA = 1
#define GPIO_SDA_LOW         0//I2C->U8_I2CPDR.SDA = 0

#define GPIO_IIC_ADDR		0x00


// IO IIC
void 	GPIOI2C_delay(uint32_t val);
uint8_t GPIOI2C_Start(void);
uint8_t GPIOI2C_Stop(void);
uint8_t GPIOI2C_write_byte(const uint8_t s);
uint8_t GPIOI2C_read_byte(uint8_t ack);


#endif /* GPIO_IIC_H_ */

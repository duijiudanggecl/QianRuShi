/*
 * gpio_iic.c
 *
 *  Created on: 2024年5月30日
 *      Author: swiftchen
 
 	CTP_BufferRead(READ_BUFF, 0xD000, 2, 7);
	CTP_BufferWrite(READ_BUFF, 0xD000, 2, 1);
 */
#include "gpio_iic.h"

void GPIOI2C_delay(uint32_t val)
{
	for (int32_t i = 0; i < val * 60; i++) // val*30  350kHZ
	{
		//__NOP;
		__asm("nop");
	}
}

void GPIOI2C_Init(void)  	//配置为GPIO口，输出上拉
{
	GPIO_SCL_OUTPUT;
	GPIO_SCL_PULLUP;
	GPIO_SDA_OUTPUT;
	GPIO_SDA_PULLUP;
}

uint8_t GPIOI2C_Start(void)
{
	GPIO_SCL_HIGH;
	GPIO_SDA_HIGH;
	GPIOI2C_delay(1);

	GPIO_SDA_LOW;
	GPIOI2C_delay(1);

	GPIO_SCL_LOW;
	GPIOI2C_delay(1);
	
	return 0;
}

uint8_t GPIOI2C_Stop(void)
{
	GPIO_SDA_LOW;
	GPIOI2C_delay(1);
	GPIO_SCL_HIGH;
	GPIOI2C_delay(1);
	GPIO_SDA_HIGH;
	GPIOI2C_delay(2);
	
	return 0;
}

uint8_t GPIOI2C_write_byte(const uint8_t s)
{
	uint8_t ch_data, i = 0;
	
	// GPIO_SCL_LOW;
	ch_data = s;
	for (i = 0; i < 8; i++)
	{
		if (ch_data & 0x80)
			GPIO_SDA_HIGH;
		else
			GPIO_SDA_LOW;
		GPIOI2C_delay(1);
		GPIO_SCL_HIGH;
		GPIOI2C_delay(1);
		GPIO_SCL_LOW;
		GPIOI2C_delay(1);
		ch_data <<= 1;
	}
	// ACK
	GPIO_SDA_INPUT;
	GPIO_SDA_HIGH;
	GPIOI2C_delay(1);
	GPIO_SCL_HIGH;
	GPIOI2C_delay(1);
	for (uint8_t i = 0; i < 200; i++)
	{
		if (!GPIO_GetSDABit)
		{
			GPIO_SCL_LOW;
			GPIO_SDA_OUTPUT;
			GPIOI2C_delay(1);
			// printf("ack 0k \r\n");
			return 0; // ACK
		}
	}
	GPIO_SCL_LOW;
	GPIO_SDA_OUTPUT;
	GPIOI2C_Stop(); // ERROR
	GPIOI2C_delay(1);
	// printf("ack error \r\n");
	return 1; // NACK
}

uint8_t GPIOI2C_read_byte(uint8_t ack)
{
	uint8_t ch_data = 0x00, i = 0;
	
	GPIO_SDA_INPUT;
	GPIO_SDA_HIGH;
	GPIOI2C_delay(1);
	for (i = 0; i < 8; i++)
	{
		GPIO_SCL_LOW;
		GPIOI2C_delay(1);
		GPIO_SCL_HIGH;
		GPIOI2C_delay(1);
		ch_data <<= 1;
		if (GPIO_GetSDABit)
			ch_data |= 0x01;
	}
	// ACK
	GPIO_SDA_OUTPUT;
	GPIO_SCL_LOW;
	GPIOI2C_delay(1);
	if (ack)
		GPIO_SDA_HIGH;
	else
		GPIO_SDA_LOW;
	GPIOI2C_delay(1);
	GPIO_SCL_HIGH;
	GPIOI2C_delay(1);
	GPIO_SCL_LOW;
	GPIOI2C_delay(1);
	
	return ch_data;
}

uint8_t GPIO_BufferWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint8_t AddrByte, uint16_t NumByte)
{
	uint32_t addr;
	
	GPIOI2C_Start();
	
	GPIOI2C_write_byte((GPIO_IIC_ADDR) | 0);
	for (; AddrByte > 0; AddrByte--)
	{
		addr = WriteAddr;
		for (uint8_t i = 1; i < AddrByte; i++)
			addr >>= 8;
		GPIOI2C_write_byte(addr & 0xFF);
	}
	
	while (NumByte--)
	{
		GPIOI2C_write_byte(*pBuffer);
		pBuffer++;
	}
	
	GPIOI2C_Stop();
	
	return 0;
}
	

uint8_t GPIO_BufferRead(uint8_t *pBuffer, uint32_t ReadAddr, uint8_t AddrByte, uint16_t NumByte)
{
	uint32_t addr;
	
	GPIOI2C_Start();
	GPIOI2C_write_byte(GPIO_IIC_ADDR | 0);
	for (; AddrByte > 0; AddrByte--)
	{
		addr = ReadAddr;
		for (uint8_t i = 1; i < AddrByte; i++)
			addr >>= 8;
		GPIOI2C_write_byte(addr & 0xFF);
	}
	GPIOI2C_Stop();
	
	GPIOI2C_Start();
	GPIOI2C_write_byte(GPIO_IIC_ADDR | 0x01);
	for (; NumByte > 1; NumByte--)
	{
		*pBuffer = GPIOI2C_read_byte(0);
		pBuffer++;
	}
	*pBuffer = GPIOI2C_read_byte(1);
	
	GPIOI2C_Stop();
	
	return 0;
}


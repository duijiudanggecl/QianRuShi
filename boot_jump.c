void APP(uint32_t appxaddr)  
{
	uint32_t jump_addr = *(uint32_t *)(appxaddr);
	uint32_t tCRC = 0xFFFFFFFF;
	EFlash_Init();
	EFLASH_Write(UPDATE_KEY_ADDR,(uint8_t *)&tCRC,4);

//	LT_BacklightClose();
	Stop_PWM1();

	EIC->IER = 0;								//Disable All Interrupts
	WDT->WCR = 0x0;

    ((void (*)(void))((uint32_t *)jump_addr))();

	while(1)
		  asm("nop");
}


#if Porject_YF002

void SCI1_IRQHandler(void)
{
	uint8_t ch = 0;
	if ((SCI1->SCISR1 & 0x20) == 0x20)
	{
		ch = SCI1->SCIDRL;
		if (Rx_Count_long > 4096 + 2)
			Rx_Count_long = 0;
		Rx_Buffer_long[Rx_Count_long++ % RX_SIZE] = ch;
		Rx_Num++;
	}
}


void YF_UartRxBuff(void)
{
	uint16_t i = 0;
	
	if (Respond_Num < Rx_Num)
	{
		for (i = 0; i < (Rx_Num - Respond_Num); i++)
		{
			if (Respond_C3_OK)
			{
				
				//gUsartRx.Buf[gUsartRx.Count++] = Rx_Buffer_long[(i + Respond_Count) % RX_SIZE];
				gUsartRx.Buf[gUsartRx.Count++] = Rx_Buffer_long[(i + Respond_Count) % RX_SIZE];
				
				if (Respond_Flag == 1)
				{	
						gUsartRx.Buf[0] = YF_SCI_T;
						gUsartRx.Buf[1] = YF_SCI_X;
						gUsartRx.Buf[2] = YF_SCI_T;
						gUsartRx.Buf[3] = YF_SCI_Start;
						Respond_Flag = 0;
						Respond_Length = 27;	//Rx_Buffer_long[(i + Respond_Count) % RX_SIZE];
				}
							
				if (gUsartRx.Count >= Respond_Length && Respond_Length != 0)
				{
					gUsartRx.Flag = 1;
					Respond_C3_OK = 0;
					Respond_C2_OK = 0;
					Respond_C1_OK = 0;
					Respond_C0_OK = 0;
					break;
				}
											
				//if ((gUsartRx.Count > Respond_Length && Respond_Length != 0) || (gUsartRx.Count > Respond_FlashLength && Respond_Length == 0 && Respond_Flag == 0))
				//{
				//	gUsartRx.Flag = 1;
				//	Respond_C3_OK = 0;
				//	Respond_C2_OK = 0;
				//	Respond_C1_OK = 0;
				//	Respond_C0_OK = 0;
				//	break;
				//}
			}
			
			//if (Respond_C0_OK && !Respond_C1_OK && Rx_Buffer_long[(i + Respond_Count) % RX_SIZE] != YF_SCI_T)	//T
			//{
			//	Respond_C0_OK = 0;
			//	gUsartRx.Count = 0;
			//}

			if (!Respond_C3_OK && Rx_Buffer_long[(i + Respond_Count) % RX_SIZE] != YF_SCI_Start && Respond_C1_OK && Respond_C0_OK && Respond_C2_OK)	//:
			{
				Respond_C2_OK = 0;
				Respond_C1_OK = 0;
				Respond_C0_OK = 0;
				gUsartRx.Count = 0;  
				XCJ_Printf("swiftchen ERROR :  \r\n");
			}
			
			else if (!Respond_C2_OK && !Respond_C3_OK && Rx_Buffer_long[(i + Respond_Count) % RX_SIZE] != YF_SCI_T && Respond_C1_OK && Respond_C0_OK)	//T
			{
				Respond_C1_OK = 0;
				Respond_C0_OK = 0;
				gUsartRx.Count = 0;
				XCJ_Printf("swiftchen ERROR T  \r\n");
			}
			
			else if (!Respond_C1_OK && !Respond_C2_OK && !Respond_C3_OK && Rx_Buffer_long[(i + Respond_Count) % RX_SIZE] != YF_SCI_X && Respond_C0_OK)	//X
			{
				Respond_C0_OK = 0;
				gUsartRx.Count = 0;
				XCJ_Printf("swiftchen ERROR X  \r\n");
			}



					
			if (!Respond_C0_OK && !Respond_C1_OK && !Respond_C2_OK && !Respond_C3_OK && Rx_Buffer_long[(i + Respond_Count) % RX_SIZE] == YF_SCI_T)	//T
			{
				Respond_C0_OK = 1;
				gUsartRx.Count = 0;
				XCJ_Printf("swiftchen start T 1  \r\n");
			}					
			else if (!Respond_C1_OK && !Respond_C2_OK && !Respond_C3_OK && Rx_Buffer_long[(i + Respond_Count) % RX_SIZE] == YF_SCI_X && Respond_C0_OK)	//X
			{
				Respond_C1_OK = 1;
				//Respond_Flag = 1;
				gUsartRx.Count = 0;
				XCJ_Printf("swiftchen start X 2  \r\n");
			}
			else if (!Respond_C2_OK && !Respond_C3_OK && Rx_Buffer_long[(i + Respond_Count) % RX_SIZE] == YF_SCI_T && Respond_C1_OK && Respond_C0_OK)	//T
			{
				Respond_C2_OK = 1;
				//Respond_Flag = 1;
				gUsartRx.Count = 0;
				XCJ_Printf("swiftchen start T 3  \r\n");
			}
			else if (!Respond_C3_OK && Rx_Buffer_long[(i + Respond_Count) % RX_SIZE] == YF_SCI_Start && Respond_C1_OK && Respond_C0_OK && Respond_C2_OK)	//:
			{
				Respond_C3_OK = 1;
				Respond_Flag = 1;
				//gUsartRx.Count = 0;  //start
				gUsartRx.Count = 4;//3;
				XCJ_Printf("swiftchen start : 4  \r\n");
			}

			
		}
		Respond_Count = (i + Respond_Count) % RX_SIZE;
		Respond_Num += i;
	}
	
	else if (Respond_Num == Rx_Num && Rx_Num != 0)
	{
		Respond_Num = 0;
		Rx_Num = 0;
		Respond_Count = 0;
		Rx_Count_long = 0;
	}
	
}
#endif


void YF_ReceiveCmd(uint8_t *buf)
{
	uint16_t rxCrc, mcuCrc;
	gUsartRx.Flag = 0;
	rxCrc = (buf[23] << 8)|buf[24];	
	mcuCrc = UART_CRC16_USB(buf, 23); 	//ÏÈ¸ß£¬ºóµÍ 

	XCJ_Printf("receive  OK !  mcuCrc = %d  rxCrc = %d \n", mcuCrc,rxCrc);  

	if((mcuCrc == rxCrc) && (buf[25] == YF_SCI_RR) && (buf[26] == YF_SCI_NN))
	{
		XCJ_Printf("CRC  OK !  File:%s ,Function: %s, Line: %d\n", __FILE__, __func__, __LINE__);  
		
		YF_Receive_handler(buf);
		
		YF_SendData_CRC_Frame();
	}

}


UINT16 UART_CRC16_USB(unsigned char *pData,const UINT8 length)
{
    UINT8 j;
    UINT8 len;
    UINT16  reg_crc = 0xFFFF;
    
    len = length;
    
    while(len--)
    {
        reg_crc^=*pData++;

        for(j=0;j<8;j++)
        {

            if(reg_crc&0x01)
                reg_crc = (reg_crc>>1)^0xA001;    
            else
                reg_crc = reg_crc>>1;
        }
    } 
    *pData = (unsigned char)(reg_crc>>8);
    
    pData++;
    *pData = (unsigned char)(reg_crc);

	return (reg_crc);
}

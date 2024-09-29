uint32_t ParaCompareString(uint8_t* str1, const uint8_t* str2 , uint32_t size)
{
    uint32_t i = 0;
    
    for(i = 0; i < size; i++)
    {
        if(*(str1 + i) != *(str2 + i))
        {
            return 0;
        }
    }
    
    if(((*(str1 + i) != ' ') && (*(str1 + i) != 0)) || (*(str2 + i) != 0))
        return 0;
    
    return 1;
}
                              
                              

uint8_t* GetFirstLineStart(uint8_t* buffer, uint32_t size)
{
    uint32_t i = 0;
    uint8_t* p = buffer;
    
    for(i = 0; i < size; i++)
    { 
        if((*p >= 'A') && (*p <= 'z'))
        {
            return p;
        }
    
        p++;
    }

    return NULL;
}

uint8_t* GetNextLineStart(uint8_t* buffer, uint32_t size)
{
    uint32_t i = 0;
    uint32_t newlineflag = 0;
    uint8_t* p = buffer;
    
    for(i = 0; i < size; i++)
    {
        if(newlineflag == 0)
        {
            if((*p == 0x0D) && ((*(p + 1) == 0x0A)))
            {
                newlineflag = 1;
                
                if((*(p + 2) >= 'A') && ((*(p + 2) <= 'z')))
                {
                    if((p + 2) < (buffer + size))
                    {
                        return (p + 2);
                    }
                    else
                    {
                        return NULL;
                    }
                }
                else
                {
                    p += 1;
                }
            }
        }
        else
        {
            if((*(p + 2) >= 'A') && ((*(p + 2) <= 'z')))
            {
                if((p + 2) < (buffer + size))
                {
                    return (p + 2);
                }
                else
                {
                    return NULL;
                }
            }
            else
            {
                p += 1;
            }
        }
        
        p++;
    }
    
    return NULL;
}

uint8_t* GetLineData(uint8_t* buffer, uint32_t bufferSize, uint8_t* lineBuffer, uint32_t* lineSize)
{
    uint32_t i = 0;
//    uint32_t newlineflag = 0;
    uint8_t* p = buffer;
    
    *lineSize = 0;
    
    for(i = 0; i < bufferSize; i++)
    {
        if(*p != '\r')
        {
            p++;
            (*lineSize)++;
        }
        else
        {
            (*lineSize)++;
            (*lineSize)++;
            break;
        }
    }
    
//    for(i = 0; i < (*lineSize); i++)
//    {
//        *(lineBuffer++) = *(buffer++);
//    }
    
    return NULL;
}

uint32_t CheckLineData(uint8_t* lineBuffer, uint32_t lineSize)
{
    uint32_t i = 0;
    uint32_t colonFlag = 0;
    uint8_t temp = 0;
	
	if((lineBuffer == NULL) || (lineSize <= 2))
		return 0;
    
    lineSize -= 2;
    
    for(i = 0; i < lineSize;i++)
    {
        temp = *(lineBuffer++);
        
        if(temp == ':')
        {
            colonFlag = 1;
            continue;
        }
        
        if(colonFlag == 1)
        {
            if(((temp > '9') || (temp < '0')) && (temp != ' '))
            {
                return 0;
            }
        }
    }
    
    return 1;
}

uint8_t* TextPreprogress(uint8_t* address, uint32_t size, uint8_t* buffer, uint32_t* pcounter)
{
    uint8_t* bufferPosi = NULL;
    uint8_t* bufferPosilater = NULL;
    uint8_t* pLine = 0;
    uint32_t lineCnt = 0;
    uint32_t i = 0;
    uint32_t annotationFlag = 0;
    uint32_t LRFlag = 0;
//    uint32_t bufferCnt = 0;
    uint32_t spaceCounter = 0;
    uint32_t colonCounter = 0;
    uint8_t temp = 0;
    uint8_t tempNext = 0;
    
    if((size + 2) > ParseBufferSize)
        return 0;
    
    if(buffer == NULL)
        return 0;
    
    bufferPosi = buffer;
    
    for(i = 0; i < size; i++)
    {
        temp = *(address + i);
        
        if((temp == ' ') || (temp == '\t'))
        {
            if((*(bufferPosi - 1) != ' ') && (*(bufferPosi - 1) != '\n'))
            {
                *(bufferPosi++) = ' ';
            }
            
            continue;
        }
        
        /* windows style. */
        if(temp == '\r')
        {
            tempNext = *(address + i + 1);
            if(tempNext == '\n')
            {
                i++;
                if(LRFlag == 0)
                {
                    if(*(bufferPosi - 1) != ' ')
                    {
                        *(bufferPosi++) = ' ';
                    }
                    
                    *(bufferPosi++) = '\r';
                    *(bufferPosi++) = '\n';
    
                    LRFlag = 1;   
                }
                
                annotationFlag = 0;
                
            }
            continue;
        }
        /* unix style. */
        else if(temp == '\n')
        {
            if(LRFlag == 0)
            {
                if(*(bufferPosi - 1) != ' ')
                {
                    *(bufferPosi++) = ' ';
                }
                
                *(bufferPosi++) = '\r';
                *(bufferPosi++) = '\n';
            
                LRFlag = 1;  
            }
            
            annotationFlag = 0;
            
            continue;
        }
        
        
        
        /* Get annotation flag symbel.*/
        if(temp == '%')
        {
            annotationFlag = 1;
            continue;
        }
        
        /* Ignore annotation.*/
        if(annotationFlag == 1)
        {
            /* windows style. */
            if(temp == '\r')
            {
                tempNext = *(address + i + 1);
                if(tempNext == '\n')
                {
                    
                    if(*(bufferPosi - 1) != ' ')
                    {
                        *(bufferPosi++) = ' ';
                    }
                    
                    *(bufferPosi++) = '\r';
                    *(bufferPosi++) = '\n';
                    
                    i++;
                    
                    annotationFlag = 0;
                    
                    LRFlag = 1;
                }
            }
            /* unix style. */
            else if(temp == '\n')
            {
                if(*(bufferPosi - 1) != ' ')
                {
                    *(bufferPosi++) = ' ';
                }
                    
                *(bufferPosi++) = '\r';
                *(bufferPosi++) = '\n';
                
                annotationFlag = 0;
                
                LRFlag = 1;
            }
//            /* Mac style. */
//            else if(temp != '\r')
//            {
////                *(bufferPosi++) = '\r';
//                *(bufferPosi++) = '\n';
//            }
            
            
            continue;
            
        }
        
        LRFlag = 0;
        
        if(temp == ':')
        {
            *(bufferPosi++) = ' ';
            *(bufferPosi++) = temp;
            *(bufferPosi++) = ' ';
            continue;
        }
        
        if((temp >= 'A') && (temp <= 'Z'))
            temp += 0x20;   
        
        *(bufferPosi++) = temp;
    }
    
    if(*(bufferPosi - 1) != '\n')
    {
        if(*(bufferPosi - 1) != ' ')
        {
            *(bufferPosi++) = ' ';
        }
        *(bufferPosi++) = '\r';
        *(bufferPosi++) = '\n';
    }
    
    *pcounter = bufferPosi - buffer;
    
    bufferPosi = buffer;
    bufferPosilater = buffer;
    
    pLine = bufferPosi;
    lineCnt = 0;
    for(i = 0; i < *pcounter; i++)
    {
        
        temp = *(bufferPosi++);
        lineCnt++;
        
        if(temp == ' ')
        {
            spaceCounter++;
        }
        
        if(temp == ':')
        {
            colonCounter++;
        }
        
        if(temp == '\r')
        {
            if((spaceCounter == 3) && (colonCounter == 1))
            {
                for(i = 0;i < lineCnt;i++)
                {
                    *(bufferPosilater++) = *(pLine++);
                }
                
                
                *(bufferPosilater++) = '\n';   
                
                pLine = bufferPosi;
            }
            
            bufferPosi++;
            pLine = bufferPosi;
            lineCnt = 0;
            spaceCounter = 0;
            colonCounter = 0;
        }
        
    }
    
    *pcounter = bufferPosilater - buffer;
    
    
    for(;bufferPosilater < bufferPosi;)
    {
        *(bufferPosilater++) = 0; 
    }
    
    return buffer;
}


uint32_t GetStringLine(uint8_t* address, uint32_t size, uint8_t* buffer, uint32_t* pcounter)
{
    uint8_t* bufferPosi = NULL;
    uint8_t* bufferPosilater = NULL;
    uint8_t* pLine = 0;
    uint32_t lineCnt = 0;
    uint32_t i = 0;
	uint32_t j = 0;
	uint32_t k = 0;
	uint32_t posiCnt = 0;
    uint32_t annotationFlag = 0;
    uint32_t LRFlag = 0;
//    uint32_t bufferCnt = 0;
    uint32_t spaceCounter = 0;
    uint32_t colonCounter = 0;
    uint8_t temp = 0;
    uint8_t tempNext = 0;
    
    if((size + 2) > ParseBufferSize)
        return 0;
    
    if(buffer == NULL)
        return 0;
    
    bufferPosi = buffer;
	posiCnt = 0;
	i = 0;
	
	while(i < size)
	{
		bufferPosi = buffer;
		LRFlag = 0;
		
		for(i = posiCnt; i < size; i++)
		{
			temp = *(address + i);
        
			if((temp == ' ') || (temp == '\t'))
			{
				if((*(bufferPosi - 1) != ' ') && (*(bufferPosi - 1) != '\n'))
				{
					*(bufferPosi++) = ' ';
				}
            
				continue;
			}
        
			/* windows style. */
			if(temp == '\r')
			{
				tempNext = *(address + i + 1);
				if(tempNext == '\n')
				{
					i++;
					if(LRFlag == 0)
					{
						if(*(bufferPosi - 1) != ' ')
						{
							*(bufferPosi++) = ' ';
						}
                    
						*(bufferPosi++) = '\r';
						*(bufferPosi++) = '\n';
    
						LRFlag = 1;   
					}
                
					annotationFlag = 0;
                
				}
				break;
			}
			/* unix style. */
			else if(temp == '\n')
			{
				if(LRFlag == 0)
				{
					if(*(bufferPosi - 1) != ' ')
					{
						*(bufferPosi++) = ' ';
					}
                
					*(bufferPosi++) = '\r';
					*(bufferPosi++) = '\n';
            
					LRFlag = 1;  
				}
            
				annotationFlag = 0;
            
				break;
			}
        
        
        
			/* Get annotation flag symbel.*/
			if(temp == '%')
			{
				annotationFlag = 1;
				continue;
			}
        
			/* Ignore annotation.*/
			if(annotationFlag == 1)
			{
				/* windows style. */
				if(temp == '\r')
				{
					tempNext = *(address + i + 1);
					if(tempNext == '\n')
					{
                    
						if(*(bufferPosi - 1) != ' ')
						{
							*(bufferPosi++) = ' ';
						}
                    
						*(bufferPosi++) = '\r';
						*(bufferPosi++) = '\n';
                    
						i++;
                    
						annotationFlag = 0;
                    
						LRFlag = 1;
					}
					
					break;
				}
				/* unix style. */
				else if(temp == '\n')
				{
					if(*(bufferPosi - 1) != ' ')
					{
						*(bufferPosi++) = ' ';
					}
                    
					*(bufferPosi++) = '\r';
					*(bufferPosi++) = '\n';
                
					annotationFlag = 0;
                
					LRFlag = 1;
					
					break;
				}
//            	/* Mac style. */
//            	else if(temp != '\r')
//            	{
////                	*(bufferPosi++) = '\r';
//                	*(bufferPosi++) = '\n';
//            	}

				continue;
            
			}
        
			LRFlag = 0;
        
			if(temp == ':')
			{
				*(bufferPosi++) = ' ';
				*(bufferPosi++) = temp;
				*(bufferPosi++) = ' ';
				continue;
			}
        
			if((temp >= 'A') && (temp <= 'Z'))
				temp += 0x20;   
        
			*(bufferPosi++) = temp;
			
			if((bufferPosi - buffer) >= ProductParaLineBufferMaxSize)
			{
				LRFlag = 1;
					
				break;
			}
		}
		
		
		if(*(bufferPosi - 1) != '\n')  //last line
		{
			if(*(bufferPosi - 1) != ' ')
			{
				*(bufferPosi++) = ' ';
			}
			*(bufferPosi++) = '\r';
			*(bufferPosi++) = '\n';
			
			LRFlag = 1;   
		}
		
		if(i != size)
		{
			posiCnt = i + 1;
		}
		else
		{
			posiCnt = size;
		}
		
		
		if(LRFlag == 1)
		{
			lineCnt = 0;
			spaceCounter = 0;
			colonCounter = 0;
				
			*pcounter = bufferPosi - buffer;
    
			bufferPosi = buffer;
			bufferPosilater = buffer;
    
			pLine = bufferPosi;
			lineCnt = 0;
			for(j = 0; j < *pcounter; j++)
			{
				temp = *(bufferPosi++);
				lineCnt++;
        
				if(temp == ' ')
				{
					spaceCounter++;
				}
        
				if(temp == ':')
				{
					colonCounter++;
				}
        
				if(temp == '\r')
				{
					if((spaceCounter == 3) && (colonCounter == 1))
					{
						for(k = 0;k < lineCnt;k++)
						{
							*(bufferPosilater++) = *(pLine++);
						}
                
                
						*(bufferPosilater++) = '\n';   
                
						pLine = bufferPosi;
						
						*pcounter = bufferPosilater - buffer;
    
    
						for(;bufferPosilater < bufferPosi;)
						{
							*(bufferPosilater++) = 0; 
						}
						
						return posiCnt;
					}
				}
        
			}
			
			
		}
		
		*pcounter = 0;
	
	}
    
    return posiCnt;
}

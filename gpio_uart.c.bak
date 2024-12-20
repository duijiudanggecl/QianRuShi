//UART波特率是9600,1 bit≈104us，
//1个起始位，1个停止位，8个数据位
//每个数据包帧头是0xAA，帧尾0x0D，校验值是除了帧尾所有数据的累加和。
//通讯方式采用一问一答形式，每500ms发送一次数据并接收应答数据，
//接收数据总共22byte，22*(1+1+8)*104us=22.88ms,即接收每帧数据的时间长度.
 
#define d_head  0xAA
#define d_tail  0x0D
 
volatile bit RXFLAG = 0;//外部中断标志位,在下降沿中断中置1;
u8	Rx_Tim=0;    //接收计时器，每次接收到数据时重置倒计时。倒计时为0则认为上一帧结束,        
                 //开始接收下一帧数据，复位接收计数器。
#define Set_Clear_RxTim()        Rx_Tim=50    //50ms倒计时，我的产品接收数据时间长度22.88ms
                                              //理论上23ms就可以了，不过要考虑时序存在误差。
                
void Uart_IO_GPIO_Init(void)//模拟串口的IO口初始化
{
    PORTA|= 0B00000001;		//Tx输出1
    TRISA &= 0B11111110;    //PA0-Output_Tx  PA1-Input_Rx
    WPUA  |= 0B00000010;    //PA1上拉电阻
}
 
void WaitTF0(void)//延时等待控制UART时序
{
     while(T2UIE);//定时器开始计时后，在定时器中断中清零T2UIE
     T2UIE=1;
}
 
void WByte(u8 input)//发送1 Byte
{
	u8 i=8;
	TXIO=0;     //Tx置0，发送起始位
    T2UIF = 1;  //清零定时器2中断标志位
	T2CEN =0;   //失能定时器2
	TIM2CNTRH=0;//清零定时器2计数值高位
	TIM2CNTRL=0;//清零定时器2计数值低位
	T2CEN=1;    //启动定时器2
	T2UIE =1;   //使能定时器2
	WaitTF0();  //等待定时器中断，等待时间取决于定时器2的装载值。这里的定时时间是104us
	while(i--)
	{
		if(input&0x01) //逐个bit发送，先传低位
			TXIO=1;
		else
			TXIO = 0;   
		WaitTF0(); //等待延时104us
		input=input>>1; 
	}
	TXIO=1;      //发送结束位
	WaitTF0();
	T2UIE=0;     //发送1 byte完成了     
}
 
 
u8 RByte()//接收1 Byte
{
	u8 Output=0;
	u8 i=8;
    T2UIF = 1;    //清零定时器2中断标志位
    T2CEN=0;      //失能定时器2
	TIM2CNTRH=0;  //清零定时器2计数值高位
	TIM2CNTRL=0;  //清零定时器2计数值低位
    TIM2ARRH =0x09; //设定时器2装载值
	TIM2ARRL =0x00;	//起始位1.5bit(156us)后再读取电平，确保在第一bit中间位置读取数据
    T2CEN=1;        //启动定时器2                
    T2UIE =1;       //使能定时器2
	WaitTF0();      //等待104us+52us=156us，刚好在第一个bit的中间位置读取数据
    T2CEN=0;        //失能定时器2
  	TIM2ARRH =0x06; //重新装载104us，确保在后续的bit中间位置读取数据
	TIM2ARRL =0x80; 
    T2CEN=1;        //使能定时器2
	while(i--)
	{
		Output =Output>>1;
		if(RXIO) 
        {
        	Output   |=0x80;      			//先接收收低位
        }
		WaitTF0();                 			//位间延时104us
	}
	T2UIE=0;                          		//停止Timer0
	return Output;                          //返回接收到的byte
}
 
 
u8 Rx_buff[Rx_size]={0};
volatile bit B_Rx_decode;
void UART_IO_RxProcess(vu8 *buff,vu8 size)//接收size长度的数据放在buff中
{
    u8 rdata;
    static u8 cnt,checksum;
	if(RXFLAG)							//RXFLAG在下降沿外部中断函数中置1
    {
        rdata=RByte();                  //有下降沿中断说明有起始位，开始接收数据
        if(!Rx_Tim)//倒计时结束，说明超过50ms没有接收到数据，清零接收计数值，开始接收下一帧.
        {
          cnt=0;
          checksum=0;
        }
        Set_Clear_RxTim();//接收到数据，复位倒计时
        buff[cnt]=rdata;
        if(cnt<(Rx_size-2))
            checksum+=buff[cnt];//计算数据校验和，与接收到的校验和对比。
         if(cnt==(Rx_size-1))//计数值与数据长度相等，说明接收完一帧数据。
        {
            if((buff[0]==d_head)&&(buff[Rx_size-1]==d_tail)&&(checksum==buff[Rx_size-2]))
				B_Rx_decode=1;//帧头，帧尾，校验和,全部正确，接收数据有效。
        }
		cnt++;
        EPIE0   = 0B00000010;//接收完一个数据，重新使能外部下降沿中断，准备接收下一个数据
		RXFLAG = 0;          //标志位清零
    } 
 }
 
u8 Tx_buff[Tx_size]={0};
void UART_IO_TxProcess(vu8 *buff,vu8 size)//发送数据
{
	u8 i;
    for(i=0;i<size;i++)
    {
        WByte(buff[i]);
    }
}
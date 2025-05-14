/*

 */
 
#include "tjc_usart_hmi.h" 
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>

#define STR_LENGTH 100

typedef struct
{
	uint16_t Head;
	uint16_t Tail;
	uint16_t Lenght;
	uint8_t  Ring_data[RINGBUFF_LEN];
}RingBuff_t;

RingBuff_t ringBuff;	//创建一个ringBuff的缓冲区


/********************************************************
函数名：  	uart1_init
作者：    	
日期：    	2024.08.18
功能：    	初始化uart1用于打印调试信息
输入参数：	
返回值： 		
修改记录：
**********************************************************/
void uart1_init(uint32_t __Baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);	
	
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin           = GPIO_Pin_9;//TX引脚
	GPIO_InitStructure.GPIO_Mode          = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed         = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin           = GPIO_Pin_10;//RX引脚
	GPIO_InitStructure.GPIO_Mode          = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	USART_InitTypeDef USART_InitStructure;//定义配置串口的结构体变量

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//开启串口1的时钟

	USART_DeInit(USART1);//大概意思是解除此串口的其他配置

	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate              = __Baud;//设置波特率
	USART_InitStructure.USART_WordLength            = USART_WordLength_8b;//字节长度为8bit
	USART_InitStructure.USART_StopBits              = USART_StopBits_1;//1个停止位
	USART_InitStructure.USART_Parity                = USART_Parity_No ;//没有校验位
	USART_InitStructure.USART_Mode                  = USART_Mode_Rx | USART_Mode_Tx;//将串口配置为收发模式
	USART_InitStructure.USART_HardwareFlowControl   = USART_HardwareFlowControl_None; //不提供流控 
	USART_Init(USART1,&USART_InitStructure);//将相关参数初始化给串口1
	
	USART_ClearFlag(USART1,USART_FLAG_RXNE);//初始配置时清除接收置位
	USART_ClearFlag(USART1,USART_FLAG_IDLE);//初始配置时清除空闲置位

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//初始配置接收中断
	//USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//初始配置空闲中断

	USART_Cmd(USART1,ENABLE);//开启串口1
	
	NVIC_InitTypeDef NVIC_InitStructure;//中断控制结构体变量定义

	NVIC_InitStructure.NVIC_IRQChannel                    = USART1_IRQn;//中断通道指定为USART1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 1;//主优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 1;//次优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;//确定使能
	NVIC_Init(&NVIC_InitStructure);//初始化配置此中断通道
		
}



/********************************************************
函数名：  	uart2_init
作者：    	
日期：    	2024.08.18
功能：    	初始化uart2,用于发送数据给串口屏
输入参数：	
返回值： 		
修改记录：
**********************************************************/
void uart2_init(uint32_t __Baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA,ENABLE);	
	
	
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin           = GPIO_Pin_2;//TX引脚
	GPIO_InitStructure.GPIO_Mode          = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed         = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin           = GPIO_Pin_3;//RX引脚
	GPIO_InitStructure.GPIO_Mode          = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	USART_InitTypeDef USART_InitStructure;//定义配置串口的结构体变量

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);//开启串口1的时钟

	USART_DeInit(USART2);//大概意思是解除此串口的其他配置

	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate              = __Baud;//设置波特率
	USART_InitStructure.USART_WordLength            = USART_WordLength_8b;//字节长度为8bit
	USART_InitStructure.USART_StopBits              = USART_StopBits_1;//1个停止位
	USART_InitStructure.USART_Parity                = USART_Parity_No ;//没有校验位
	USART_InitStructure.USART_Mode                  = USART_Mode_Rx | USART_Mode_Tx;//将串口配置为收发模式
	USART_InitStructure.USART_HardwareFlowControl   = USART_HardwareFlowControl_None; //不提供流控 
	USART_Init(USART2,&USART_InitStructure);//将相关参数初始化给串口1
	
	USART_ClearFlag(USART2,USART_FLAG_RXNE);//初始配置时清除接收置位
	USART_ClearFlag(USART2,USART_FLAG_IDLE);//初始配置时清除空闲置位

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//初始配置接收中断
	//USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//初始配置空闲中断

	USART_Cmd(USART2,ENABLE);//开启串口1
	
	NVIC_InitTypeDef NVIC_InitStructure;//中断控制结构体变量定义

	NVIC_InitStructure.NVIC_IRQChannel                    = USART2_IRQn;//中断通道指定为USART1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 0;//主优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 1;//次优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;//确定使能
	NVIC_Init(&NVIC_InitStructure);//初始化配置此中断通道
		
}

/********************************************************
函数名：  	TJC_SendData
作者：    	
日期：    	2024.08.18
功能：    	向串口打印数据,需要制定发送的字符串长度
输入参数：	
返回值： 		
修改记录：
**********************************************************/
void TJC_SendData(uint8_t *data, uint16_t len)
{
	while(len--)
	{
		//等待发送数据寄存器为空
		
		USART_SendData(USART2, *data++);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	}

}




/********************************************************
函数名：  	TJCPrintf
作者：    	wwd
日期：    	2022.10.08
功能：    	向串口打印数据,使用前请将USART_SCREEN_write这个函数改为你的单片机的串口发送单字节函数
输入参数：	0参考printf
返回值： 		打印到串口的数量
修改记录：
**********************************************************/

void TJCPrintf(const char *str, ...)
{


	uint8_t end = 0xff;
	char buffer[STR_LENGTH+1];  // 数据长度
	va_list arg_ptr;
	va_start(arg_ptr, str);
	int len = vsnprintf(buffer, STR_LENGTH+1, str, arg_ptr);
	va_end(arg_ptr);
	for(int i = 0; i < len; i++)
	{
		USART_SendData(USART2, buffer[i]);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	//等待发送完毕
	}

	USART_SendData(USART2, end);			//这个函数改为你的单片机的串口发送单字节函数
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	//等待发送完毕
	USART_SendData(USART2, end);			//这个函数改为你的单片机的串口发送单字节函数
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	//等待发送完毕
	USART_SendData(USART2, end);			//这个函数改为你的单片机的串口发送单字节函数
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	//等待发送完毕

}


/********************************************************
函数名：  	initRingBuff
作者：
日期：    	2022.10.08
功能：    	初始化环形缓冲区
输入参数：
返回值： 		void
修改记录：
**********************************************************/
void initRingBuff(void)
{
  //初始化相关信息
  ringBuff.Head = 0;
  ringBuff.Tail = 0;
  ringBuff.Lenght = 0;
}




/********************************************************
函数名：  	writeRingBuff
作者：
日期：    	2022.10.08
功能：    	往环形缓冲区写入数据
输入参数：
返回值： 		void
修改记录：
**********************************************************/
void writeRingBuff(uint8_t data)
{
  if(ringBuff.Lenght >= RINGBUFF_LEN) //判断缓冲区是否已满
  {
    return ;
  }
  ringBuff.Ring_data[ringBuff.Tail]=data;
  ringBuff.Tail = (ringBuff.Tail+1)%RINGBUFF_LEN;//防止越界非法访问
  ringBuff.Lenght++;

}



/********************************************************
函数名：  	deleteRingBuff
作者：
日期：    	2022.10.08
功能：    	删除串口缓冲区中相应长度的数据
输入参数：		要删除的长度
返回值： 		void
修改记录：
**********************************************************/
void deleteRingBuff(uint16_t size)
{
	if(size >= ringBuff.Lenght)
	{
	    initRingBuff();
	    return;
	}
	for(int i = 0; i < size; i++)
	{

		if(ringBuff.Lenght == 0)//判断非空
		{
		initRingBuff();
		return;
		}
		ringBuff.Head = (ringBuff.Head+1)%RINGBUFF_LEN;//防止越界非法访问
		ringBuff.Lenght--;

	}

}



/********************************************************
函数名：  	read1BFromRingBuff
作者：
日期：    	2022.10.08
功能：    	从串口缓冲区读取1字节数据
输入参数：		position:读取的位置
返回值： 		所在位置的数据(1字节)
修改记录：
**********************************************************/
uint8_t read1BFromRingBuff(uint16_t position)
{
	uint16_t realPosition = (ringBuff.Head + position) % RINGBUFF_LEN;

	return ringBuff.Ring_data[realPosition];
}




/********************************************************
函数名：  	getRingBuffLenght
作者：
日期：    	2022.10.08
功能：    	获取串口缓冲区的数据数量
输入参数：
返回值： 		串口缓冲区的数据数量
修改记录：
**********************************************************/
uint16_t getRingBuffLenght()
{
	return ringBuff.Lenght;
}


/********************************************************
函数名：  	isRingBuffOverflow
作者：
日期：    	2022.10.08
功能：    	判断环形缓冲区是否已满
输入参数：
返回值： 		1:环形缓冲区已满 , 2:环形缓冲区未满
修改记录：
**********************************************************/
uint8_t isRingBuffOverflow()
{
	return ringBuff.Lenght == RINGBUFF_LEN;
}


















#if !defined(__MICROLIB)
//不使用微库的话就需要添加下面的函数
#if (__ARMCLIB_VERSION <= 6000000)
//如果编译器是AC5  就定义下面这个结构体
struct __FILE
{
	int handle;
};
#endif

FILE __stdout;

//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}
#endif

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    USART_SendData(USART1, (uint8_t)ch);
	
	while( RESET == USART_GetFlagStatus(USART1, USART_FLAG_TXE) ){}
	
    return ch;
}



/******** 串口2 中断服务函数 ***********/
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)//判断是不是真的有中断发生
	{
		//USART_SendData(USART2,USART_ReceiveData(USART2));//又将数据发回去(用于验证)

		writeRingBuff(USART_ReceiveData(USART2));
		USART_ClearITPendingBit(USART2, USART_IT_RXNE); //已经处理就清楚标志位 
	}  
}



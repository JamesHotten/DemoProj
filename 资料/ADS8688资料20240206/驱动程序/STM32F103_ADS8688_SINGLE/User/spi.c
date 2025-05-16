//-----------------------------------------------------------------
// 程序描述:
//     LED显示驱动程序
// 作    者: 凌智电子
// 开始日期: 2014-01-28
// 完成日期: 2014-01-28
// 修改日期: 2014-02-16
// 当前版本: V1.0.1
// 历史版本:
//  - V1.0: (2014-02-07)LED IO 配置
// - V1.0.1:(2014-02-16)头文件中不包含其他头文件
// 调试工具: 凌智STM32核心开发板、LZE_ST_LINK2
// 说    明:
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include <stm32f10x.h>

#include "spi.h"

//-----------------------------------------------------------------
// void SPI2_Init(void)
//-----------------------------------------------------------------
//
// 函数功能: SPI口初始化
// 入口参数: 无
// 返 回 值: 无
// 全局变量: 无
// 调用模块: RCC_APB2PeriphClockCmd();GPIO_Init();
// 注意事项: 无
//
//-----------------------------------------------------------------
void SPI2_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  // 设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;												// 设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;										// 设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;													// 串行同步时钟的空闲状态为低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;												// 串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;														// NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; // 定义波特率预分频的值:波特率预分频值为32
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;									// 指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;														// CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  																// 根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	
	SPI_Cmd(SPI2, ENABLE); 		// 使能SPI外设
	SPI2_ReadWriteByte(0xff);	// 启动传输		 
}   

void SPI2_SetSpeed(uint8_t SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI2->CR1&=0XFFC7;
	SPI2->CR1|=SPI_BaudRatePrescaler;	//设置SPI2速度 
	SPI_Cmd(SPI2,ENABLE); 

} 

//-----------------------------------------------------------------
// uint8_t SPI2_ReadWriteByte(uint8_t TxData)
//-----------------------------------------------------------------
//
// 函数功能: SPI读写一个字节
// 入口参数: TxData：发送的字节
// 返 回 值: 读取的字节
// 全局变量: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
uint8_t SPI2_ReadWriteByte(uint8_t TxData)
{		
	uint8_t retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) 	// 检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)
			return 0;
	}			  
	SPI_I2S_SendData(SPI2, TxData); 	// 通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) // 检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200)
			return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI2);	// 返回通过SPIx最近接收的数据					    
}

//-----------------------------------------------------------------
// uint8_t SPI2_WriteByte(uint8_t TxData)
//-----------------------------------------------------------------
//
// 函数功能: SPI口初始化
// 入口参数: TxData：发送的字节
// 返 回 值: 发送状态：1，发送失败  0，发送成功
// 全局变量: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
uint8_t SPI2_WriteByte(uint8_t TxData)
{		
	uint8_t retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) 	// 检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)
			return 1;
	}			  
	SPI_I2S_SendData(SPI2, TxData); // 通过外设SPIx发送一个数据   
	return 0;
}

//-----------------------------------------------------------------
// uint8_t SPI2_ReadByte(void)
//-----------------------------------------------------------------
//
// 函数功能: SPI读写一个数据
// 入口参数: 无
// 返 回 值: 读取的字节
// 全局变量: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
uint8_t SPI2_ReadByte(void)
{		
	uint8_t retry=0;				 	

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) // 检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200)
			return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI2); // 返回通过SPIx最近接收的数据					    
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------

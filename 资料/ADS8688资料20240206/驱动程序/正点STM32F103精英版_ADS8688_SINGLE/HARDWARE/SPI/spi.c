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
// 调试工具: 正点原子精英版STM32F103开发板、LZE_ST LINK2
// 说    明:
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include <stm32f10x.h>

#include "spi.h"

//-----------------------------------------------------------------
// void SPI1_Init(void)
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
void SPI1_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1,  ENABLE );
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  // 设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;												// 设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;										// 设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;													// 串行同步时钟的空闲状态为低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;												// 串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;														// NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; // 定义波特率预分频的值:波特率预分频值为32
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;									// 指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;														// CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  																// 根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	
	SPI_Cmd(SPI1, ENABLE); 		// 使能SPI外设
	SPI1_ReadWriteByte(0xff);	// 启动传输		 
}   

void SPI1_SetSpeed(uint8_t SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI1->CR1&=0XFFC7;
	SPI1->CR1|=SPI_BaudRatePrescaler;	//设置SPI1速度 
	SPI_Cmd(SPI1,ENABLE); 

} 

//-----------------------------------------------------------------
// uint8_t SPI1_ReadWriteByte(uint8_t TxData)
//-----------------------------------------------------------------
//
// 函数功能: SPI读写一个字节
// 入口参数: TxData：发送的字节
// 返 回 值: 读取的字节
// 全局变量: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{		
	uint8_t retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) 	// 检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)
			return 0;
	}			  
	SPI_I2S_SendData(SPI1, TxData); 	// 通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) // 检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200)
			return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI1);	// 返回通过SPIx最近接收的数据					    
}

//-----------------------------------------------------------------
// uint8_t SPI1_WriteByte(uint8_t TxData)
//-----------------------------------------------------------------
//
// 函数功能: SPI口初始化
// 入口参数: TxData：发送的字节
// 返 回 值: 发送状态：1，发送失败  0，发送成功
// 全局变量: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
uint8_t SPI1_WriteByte(uint8_t TxData)
{		
	uint8_t retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) 	// 检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)
			return 1;
	}			  
	SPI_I2S_SendData(SPI1, TxData); // 通过外设SPIx发送一个数据   
	return 0;
}

//-----------------------------------------------------------------
// uint8_t SPI1_ReadByte(void)
//-----------------------------------------------------------------
//
// 函数功能: SPI读写一个数据
// 入口参数: 无
// 返 回 值: 读取的字节
// 全局变量: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
uint8_t SPI1_ReadByte(void)
{		
	uint8_t retry=0;				 	

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) // 检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200)
			return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI1); // 返回通过SPIx最近接收的数据					    
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------

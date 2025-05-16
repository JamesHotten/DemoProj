//-----------------------------------------------------------------
// 程序描述:
//     SPI驱动程序
// 作    者: 凌智电子
// 开始日期: 2020-09-15
// 完成日期: 2020-09-15
// 当前版本: V1.0.0
// 历史版本:
// - V1.0.0: (2020-09-15)SPI驱动
// 调试工具: 凌智STM32核心开发板、LZE_ST_LINK2
// 说    明:
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "spi.h"	

SPI_HandleTypeDef SPI_Handler;  // SPI句柄

//-----------------------------------------------------------------
// void SPI2_Init(void)
//-----------------------------------------------------------------
//
// 函数功能: SPI2初始化
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void SPI2_Init(void)
{
	SPI_Handler.Instance=SPI2;                         // SP2
	SPI_Handler.Init.Mode=SPI_MODE_MASTER;             // 设置SPI工作模式，设置为主模式
	SPI_Handler.Init.Direction=SPI_DIRECTION_2LINES;   // 设置SPI单向或者双向的数据模式:SPI设置为双线模式
	SPI_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       // 设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_Handler.Init.CLKPolarity=SPI_POLARITY_LOW;     // 串行同步时钟的空闲状态为低电平
	SPI_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         // 串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_Handler.Init.NSS=SPI_NSS_SOFT;                 // NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_Handler.Init.NSSPMode=SPI_NSS_PULSE_DISABLE;	 // NSS信号脉冲失能
  SPI_Handler.Init.MasterKeepIOState=SPI_MASTER_KEEP_IO_STATE_ENABLE;  // SPI主模式IO状态保持使能
	SPI_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_32;// 定义波特率预分频的值:波特率预分频值为32,设置为6.25M时钟,高速模式
	SPI_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        // 指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        // 关闭TI模式
	SPI_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;// 关闭硬件CRC校验
	SPI_Handler.Init.CRCPolynomial=7;                  // CRC值计算的多项式
	HAL_SPI_Init(&SPI_Handler);												 // 初始化
	
	__HAL_SPI_ENABLE(&SPI_Handler);                    // 使能SPI1

	SPI2_ReadWriteByte(0Xff);                          // 启动传输
}   

//-----------------------------------------------------------------
// void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
//-----------------------------------------------------------------
// 
// 函数功能: SPI底层驱动，时钟使能，引脚配置
// 入口参数: SPI_HandleTypeDef *hspi：SPI句柄
// 返 回 值: 无
// 注意事项: 此函数会被HAL_SPI_Init()调用
//
//-----------------------------------------------------------------
void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(spiHandle->Instance==SPI2)
  {

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI2;
    PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      while(1);
    }
    __HAL_RCC_SPI2_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }	
}

//-----------------------------------------------------------------
// uint8_t SPI2_ReadWriteByte(uint8_t TxData)
//-----------------------------------------------------------------
// 
// 函数功能: SPI2读写一个字节
// 入口参数: uint8_t TxData：要写入的字节
// 返 回 值: uint8_t Rxdata：读取到的字节
// 注意事项: 无
//
//-----------------------------------------------------------------
uint8_t SPI2_ReadWriteByte(uint8_t TxData)
{
	uint8_t Rxdata;
	HAL_SPI_TransmitReceive(&SPI_Handler,&TxData,&Rxdata,1, 1000);       
 	return Rxdata;          		    // 返回收到的数据		
}

//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 

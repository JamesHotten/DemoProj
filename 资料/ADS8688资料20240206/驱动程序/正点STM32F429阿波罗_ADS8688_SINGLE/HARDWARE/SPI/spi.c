//-----------------------------------------------------------------
// 程序描述:
//     SPI驱动程序
// 作    者: 凌智电子
// 开始日期: 2018-08-04
// 完成日期: 2018-08-04
// 修改日期:
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2018-08-04)SPI驱动
// 调试工具: 正点原子STM32F429阿波罗、LZE_ST_LINK2
// 说    明:
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "spi.h"

//-----------------------------------------------------------------
SPI_HandleTypeDef SPI2_Handler; // SPI句柄
//-----------------------------------------------------------------
// void SPI2_Init(void)
//-----------------------------------------------------------------
//
// 函数功能: SPI2驱动程序，配置成主机模式
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void SPI2_Init(void)
{
  SPI2_Handler.Instance               = SPI2;                     // SP5
  SPI2_Handler.Init.Mode              = SPI_MODE_MASTER;          // 设置SPI工作模式，设置为主模式
  SPI2_Handler.Init.Direction         = SPI_DIRECTION_2LINES;     // 设置SPI单向或者双向的数据模式:SPI设置为双线模式
  SPI2_Handler.Init.DataSize          = SPI_DATASIZE_8BIT;        // 设置SPI的数据大小:SPI发送接收8位帧结构
  SPI2_Handler.Init.CLKPolarity       = SPI_POLARITY_LOW;         // 串行同步时钟的空闲状态为低电平
  SPI2_Handler.Init.CLKPhase          = SPI_PHASE_2EDGE;          // 串行同步时钟的第二个跳变沿（上升或下降）数据被采样
  SPI2_Handler.Init.NSS               = SPI_NSS_SOFT;             // NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
  SPI2_Handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32; // 定义波特率预分频的值:波特率预分频值为32
  SPI2_Handler.Init.FirstBit          = SPI_FIRSTBIT_MSB;         // 指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
  SPI2_Handler.Init.CRCPolynomial     = 0;                        // CRC值计算的多项式
  HAL_SPI_Init(&SPI2_Handler);                                    // 初始化
  __HAL_SPI_ENABLE(&SPI2_Handler);                                // 使能SPI2
}

//-----------------------------------------------------------------
// void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
//-----------------------------------------------------------------
//
// 函数功能: SPI2底层驱动，时钟使能，引脚配置
// 入口参数: SPI_HandleTypeDef *hspi：SPI句柄
// 返 回 值: 无
// 注意事项: 此函数会被HAL_SPI_Init()调用
//
//-----------------------------------------------------------------
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef GPIO_Initure;

  __HAL_RCC_GPIOB_CLK_ENABLE(); // 使能GPIOF时钟
  __HAL_RCC_SPI2_CLK_ENABLE();  // 使能SPI2时钟

  GPIO_Initure.Pin       = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_Initure.Mode      = GPIO_MODE_AF_PP;   // 复用推挽输出
  GPIO_Initure.Pull      = GPIO_PULLUP;       // 上拉
  GPIO_Initure.Speed     = GPIO_SPEED_HIGH;   // 高速
  GPIO_Initure.Alternate = GPIO_AF5_SPI2; 		// 复用为SPI2
  HAL_GPIO_Init(GPIOB, &GPIO_Initure);    		// 初始化
}

//-----------------------------------------------------------------
// void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
//-----------------------------------------------------------------
//
// 函数功能: SPI速度设置函数
// 入口参数: u8 SPI_BaudRatePrescaler：SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
// 返 回 值: 无
// 注意事项: SPI速度=fAPB1/分频系数，fAPB1时钟一般为45Mhz
//
//-----------------------------------------------------------------
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler)); // 判断有效性
  __HAL_SPI_DISABLE(&SPI2_Handler);                               // 关闭SPI
  SPI2_Handler.Instance->CR1 &= 0XFFC7;                           // 位3-5清零，用来设置波特率
  SPI2_Handler.Instance->CR1 |= SPI_BaudRatePrescaler;            // 设置SPI速度
  __HAL_SPI_ENABLE(&SPI2_Handler);                                // 使能SPI
}

//-----------------------------------------------------------------
// u8 SPI2_ReadWriteByte(u8 TxData)
//-----------------------------------------------------------------
//
// 函数功能: SPI2 读写一个字节
// 入口参数: u8 TxData： 要写入的字节
// 返 回 值: u8 Rxdata：读取到的字节
// 注意事项: 无
//
//-----------------------------------------------------------------
u8 SPI2_ReadWrite_Byte(u8 dat)
{
  u8 Rxdata;
  HAL_SPI_TransmitReceive(&SPI2_Handler, &dat, &Rxdata, 1, 1000);
  return Rxdata; // 返回收到的数据
}

//-----------------------------------------------------------------
// void SPI2_Send_Byte(u8 dat)
//-----------------------------------------------------------------
//
// 函数功能: SPI2发送1个字节数据
// 入口参数: u8 dat： 待发送的数据
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void SPI2_Send_Byte(u8 dat)
{
  u8 Rxdata;
  HAL_SPI_TransmitReceive(&SPI2_Handler, &dat, &Rxdata, 1, 1000);
}

//-----------------------------------------------------------------
// void SPI2_Send_Byte(u8 dat)
//-----------------------------------------------------------------
//
// 函数功能: SPI2读取1个字节数据
// 入口参数: u8 dat： 待发送的数据
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
u8 SPI2_Read_Byte(void)
{
  u8 Txdata = 0xFF;
  u8 Rxdata;
  HAL_SPI_TransmitReceive(&SPI2_Handler, &Txdata, &Rxdata, 1, 1000);
  return Rxdata; // 返回收到的数据
}

//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 

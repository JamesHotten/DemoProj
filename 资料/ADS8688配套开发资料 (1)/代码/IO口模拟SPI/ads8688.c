#include "ads8688.h"

/**********************************************************************************************************
*	函 数 名: ADS8688_IO_Init
*	功能说明: ADS8688 IO口初始化
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/
void ADS8688_IO_Init(void) //ADS8688 IO口初始化
{
  DAISY_L;
  CS_H;
  RST_H;
  CLK_L;
  HAL_Delay(5);
}
/**********************************************************************************************************
*	函 数 名: ADS8688_Init
*	功能说明: ADS8688初始化
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/
void ADS8688_Init(uint8_t ch_en) //ADS8688初始化
{
  ADS8688_IO_Init();
  SOFT_RESET();                                 //软件复位
  HAL_Delay(1);                                 //手册上没找到具体的，基本上是ns级，这里设置1ms
  Set_Auto_Scan_Sequence(ch_en);                //使能通道
  ADS8688_WriteReg(CH_PD, ~ch_en);              //通道退出低功耗状态 通道上电
  HAL_Delay(20);                                //手册上说退出低功耗至少15ms
  while (ADS8688_ReadReg(AUTO_SEQ_EN) != ch_en) //是否使能成功
  {
    Set_Auto_Scan_Sequence(ch_en);   //使能通道
    ADS8688_WriteReg(CH_PD, ~ch_en); //通道退出低功耗状态 通道上电
    HAL_Delay(20);
  }
}

/**********************************************************************************************************
*	函 数 名: ADS8688_SPI_Read8Bit
*	功能说明: 从SPI总线读8个bit数据。 不带CS控制。
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/
uint8_t ADS8688_SPI_Read8Bit(void)
{
  uint8_t i = 0;
  uint8_t read = 0;
  for (i = 0; i < 8; i++)
  {
    read <<= 1;
    CLK_H;
    if (READ_SDO)
      read++;
    CLK_L;
  }
  return read;
}
/**********************************************************************************************************
*	函 数 名: ADS8688_SPI_Write8Bit
*	功能说明: 向SPI总线写8个bit数据。 不带CS控制。
*	形    参: data : 数据
*	返 回 值: 无
**********************************************************************************************************/
void ADS8688_SPI_Write8Bit(uint8_t data)
{
  uint8_t i = 0;
  CS_L;
  for (i = 0; i < 8; i++)
  {
    if (data & 0x80)
      SDI_H;
    else
      SDI_L;
    data <<= 1;
    CLK_H;
    CLK_L;
  }
}

/**********************************************************************************************************
*	函 数 名: ADS8688_WriteCmd
*	功能说明: 写命令寄存器
*	形    参:  cmd : 命令
*	返 回 值: 无
**********************************************************************************************************/
void ADS8688_WriteCmd(uint16_t cmd) //写ADS8688命令寄存器
{
  CS_L;
  ADS8688_SPI_Write8Bit(cmd >> 8 & 0XFF);
  ADS8688_SPI_Write8Bit(cmd & 0XFF);
  ADS8688_SPI_Write8Bit(0X00);
  ADS8688_SPI_Write8Bit(0X00);
  CS_H;
}

/*
*********************************************************************************************************
*	函 数 名: SOFT_RESET
*	功能说明: 软件复位
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SOFT_RESET(void) //软件复位
{
  ADS8688_WriteCmd(RST);
}

/**********************************************************************************************************
*	函 数 名: AUTO_RST_Mode
*	功能说明: 自动模式
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/
void AUTO_RST_Mode(void) //自动扫描模式
{
  ADS8688_WriteCmd(AUTO_RST);
}
/**********************************************************************************************************
*	函 数 名: MAN_CH_Mode
*	功能说明: 手动模式通道选择
*	形    参:  ch : 要选择的通道
*	返 回 值: 无
**********************************************************************************************************/
void MAN_CH_Mode(uint16_t ch) //手动模式
{
  ADS8688_WriteCmd(ch);
}

/**********************************************************************************************************
*	函 数 名: ADS8688_ReadReg
*	功能说明: 读指定的寄存器
*	形    参:  addr : 寄存器地址
*	返 回 值: data : 读到的寄存器值。
**********************************************************************************************************/
uint8_t ADS8688_ReadReg(uint8_t addr)
{
  uint8_t data = 0;
  CS_L;
  ADS8688_SPI_Write8Bit(addr << 1);
  data = ADS8688_SPI_Read8Bit();
  data = ADS8688_SPI_Read8Bit();
  CS_H;
  return data;
}
/**********************************************************************************************************
*	函 数 名: ADS8688_WriteReg
*	功能说明: 写指定的寄存器
*	形    参:  addr : 寄存器地址
*			  		data : 寄存器值
*	返 回 值: 无
**********************************************************************************************************/
void ADS8688_WriteReg(uint8_t addr, uint8_t data)
{
  CS_L;
  ADS8688_SPI_Write8Bit(addr << 1 | 0X01);
  ADS8688_SPI_Write8Bit(data);
  CS_H;
}
/**********************************************************************************************************
*	函 数 名: Set_Auto_Scan_Sequence
*	功能说明: 设置自动扫描序列通道
*	形    参:  seq：需要设置自动扫描序列使能寄存器的值
*	返 回 值: 无
**********************************************************************************************************/
void Set_Auto_Scan_Sequence(uint8_t seq)
{
  ADS8688_WriteReg(AUTO_SEQ_EN, seq);
}
/**********************************************************************************************************
*	函 数 名: Set_CH_Range
*	功能说明: 设置指定通道测量范围
*	形    参:  ch : 通道
*			  		range : 测量范围
*	返 回 值: 无
**********************************************************************************************************/
void Set_CH_Range(uint8_t ch, uint8_t range)
{
  ADS8688_WriteReg(ch, range);
}

/**********************************************************************************************************
*	函 数 名: Get_AUTO_RST_Mode_Data
*	功能说明: 读取自动扫描模式AD值
*	形    参:data : 数据指针
*			  	chnum : 通道个数
*	返 回 值: 无
**********************************************************************************************************/
void Get_AUTO_RST_Mode_Data(uint16_t *data, uint8_t chnum)
{
  uint8_t i = 0, datal = 0, datah = 0;
  for (i = 0; i < chnum; i++)
  {
    CS_L;
    ADS8688_SPI_Write8Bit(0X00);
    ADS8688_SPI_Write8Bit(0X00);
    datah = ADS8688_SPI_Read8Bit();
    datal = ADS8688_SPI_Read8Bit();
    //HAL_Delay(1);
    CS_H;
    *(data + i) = datah << 8 | datal;
  }
}
/**********************************************************************************************************
*	函 数 名: Get_MAN_CH_Mode_Data
*	功能说明: 读取手动模式AD值
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/
uint16_t Get_MAN_CH_Mode_Data(void)
{
  uint8_t datah = 0, datal = 0;
  CS_L;
  ADS8688_SPI_Write8Bit(0X00);
  ADS8688_SPI_Write8Bit(0X00);
  datah = ADS8688_SPI_Read8Bit();
  datal = ADS8688_SPI_Read8Bit();
  CS_H;
  return (datah << 8 | datal);
}


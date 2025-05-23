//-----------------------------------------------------------------
// 程序描述:
//     ADS8688驱动程序
// 作    者: 凌智电子
// 开始日期: 2022-06-27
// 完成日期: 2022-06-27
// 修改日期: 
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2022-06-27)ADS8688初始化及应用
// 调试工具: 正点原子精英版STM32F103开发板、LZE_ST LINK2、2.8寸液晶、DAS8688模块
// 说    明: 
//    
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include <stm32f10x.h>
#include "ads8688.h"
#include "delay.h"
#include "spi.h"	
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void GPIO_ADS8688_Configuration(void)
//-----------------------------------------------------------------
// 
// 函数功能: ADS8688引脚配置函数
// 入口参数: 无 
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void GPIO_ADS8688_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  // 使能IO口时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

//-----------------------------------------------------------------
// void ADS8688_Init(void)
//-----------------------------------------------------------------
// 
// 函数功能: ADS8361初始化
// 入口参数: 无 
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void ADS8688_Init(void)
{
	GPIO_ADS8688_Configuration();
	SPI1_Init();
	SPI1_SetSpeed(SPI_BaudRatePrescaler_2);
	ADS8688_RST = 0;
	delay_us(2);
	ADS8688_RST = 1;
	ADS8688_DAISY = 0;

	ADS8688_Write_Command(RST);	// 软件复位模式
	delay_us(2);
	ADS8688_Write_Program(CH0_INPUT_RANGE, VREF_B_125);	// 通道0的输入范围为±5.12V
	ADS8688_Write_Program(CH1_INPUT_RANGE, VREF_B_125);	// 通道1的输入范围为±5.12V
	ADS8688_Write_Program(CH2_INPUT_RANGE, VREF_B_125);	// 通道2的输入范围为±5.12V
	ADS8688_Write_Program(CH3_INPUT_RANGE, VREF_B_125);	// 通道3的输入范围为±5.12V
	ADS8688_Write_Program(CH4_INPUT_RANGE, VREF_B_125);	// 通道4的输入范围为±5.12V
	ADS8688_Write_Program(CH5_INPUT_RANGE, VREF_B_125);	// 通道5的输入范围为±5.12V
	ADS8688_Write_Program(CH6_INPUT_RANGE, VREF_B_125);	// 通道6的输入范围为±5.12V
	ADS8688_Write_Program(CH7_INPUT_RANGE, VREF_B_125);	// 通道7的输入范围为±5.12V
	ADS8688_Write_Program(CH_PWR_DN, 0x00);							// 8个通道退出低功耗
	ADS8688_Write_Program(AUTO_SEQ_EN, 0xFF);						// 8个通道自动扫描排序
	ADS8688_Write_Command(MAN_CH_0);
}

//-----------------------------------------------------------------
// void ADS8361_Write_Command(uint16_t com)
//-----------------------------------------------------------------
// 
// 函数功能: 写命令
// 入口参数: com：命令
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void ADS8688_Write_Command(uint16_t com)
{
	uint8_t wr_data[2] = {0x00, 0x00};
	
	wr_data[0] = (uint8_t)(com >> 8);
	wr_data[1] = (uint8_t)(com &0x00FF);
	
	ADS8688_CS = 0;
	SPI1_ReadWriteByte(wr_data[0]);
	SPI1_ReadWriteByte(wr_data[1]);
	ADS8688_CS = 1;
}

//-----------------------------------------------------------------
// void ADS8688_Write_Program(uint8_t addr, uint8_t data)
//-----------------------------------------------------------------
// 
// 函数功能: 写程序寄存器
// 入口参数: addr: 寄存器地址
//					 data：数据
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void ADS8688_Write_Program(uint8_t addr, uint8_t data)
{
	uint8_t wr_data[2] = {0x00, 0x00};
	
	wr_data[0] = (addr << 1) | 0x01;
	wr_data[1] = data;
	
	ADS8688_CS = 0;
	SPI1_ReadWriteByte(wr_data[0]);
	SPI1_ReadWriteByte(wr_data[1]);
	ADS8688_CS = 1;
}

//-----------------------------------------------------------------
// uint8_t ADS8361_Read_Program(uint8_t addr)
//-----------------------------------------------------------------
// 
// 函数功能: 读程序寄存器
// 入口参数: addr: 寄存器地址
// 返 回 值: 读取的数据
// 注意事项: 无
//
//-----------------------------------------------------------------
uint8_t ADS8688_Read_Program(uint8_t addr)
{
	uint8_t Rxdata;
	uint8_t wr_data[2] = {0x00, 0x00};
	
	wr_data[0] = addr << 1;
	
	ADS8688_CS = 0;
	SPI1_ReadWriteByte(wr_data[0]);
	SPI1_ReadWriteByte(wr_data[1]);
	Rxdata = SPI1_ReadWriteByte(0xFF);
	ADS8688_CS = 1;
	
	return Rxdata;
}

//-----------------------------------------------------------------
// void Get_AUTO_RST_Mode_ADC_Data(uint8_t ch_num, uint16_t *data)
//-----------------------------------------------------------------
// 
// 函数功能: 读取AUTO_RST模式下的输出数据
// 入口参数: ch_num：通道数量（与AUTO_SEQ_EN设置的参数有关）
//					 *data：读取得到的数据
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void Get_AUTO_RST_Mode_ADC_Data(uint8_t ch_num, uint16_t *data)
{
	uint8_t Rxdata[2],i;
	uint8_t wr_data[2] = {0x00, 0x00};
	
	for(i=0; i<ch_num; i++) 
	{
		ADS8688_CS = 0;
		SPI1_ReadWriteByte(wr_data[0]);
		SPI1_ReadWriteByte(wr_data[1]);
		Rxdata[0] = SPI1_ReadWriteByte(0x00);
		Rxdata[1] = SPI1_ReadWriteByte(0x00);
		ADS8688_CS = 1;
		*(data+i) = ((uint16_t)Rxdata[0] << 8) | Rxdata[1];
	}
}

//-----------------------------------------------------------------
// void Get_MAN_CH_Data(uint16_t ch, uint16_t *data)
//-----------------------------------------------------------------
// 
// 函数功能: 读取AUTO_RST模式下的输出数据
// 入口参数: ch_num：通道数量（与AUTO_SEQ_EN设置的参数有关）
//					 *data：读取得到的数据
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void Get_MAN_CH_Data(uint16_t ch, uint16_t *data)
{
	uint8_t Rxdata[2];
	uint8_t wr_data[2] = {0x00, 0x00};

	ADS8688_Write_Command(ch);
	__NOP();
	__NOP();
	ADS8688_CS = 0;
	SPI1_ReadWriteByte(wr_data[0]);
	SPI1_ReadWriteByte(wr_data[1]);
	Rxdata[0] = SPI1_ReadWriteByte(0x00);
	Rxdata[1] = SPI1_ReadWriteByte(0x00);
	ADS8688_CS = 1;
	*data = ((uint16_t)Rxdata[0] << 8) | Rxdata[1];
}

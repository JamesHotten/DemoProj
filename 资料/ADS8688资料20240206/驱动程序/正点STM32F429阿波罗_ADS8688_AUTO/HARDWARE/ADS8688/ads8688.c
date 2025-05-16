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
// 调试工具: 正点原子STM32F429阿波罗、LZE_ST_LINK2、ADS8688模块
// 说    明: 
//    
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
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
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitStructure.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
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
	SPI2_Init();
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
	ADS8688_Write_Program(CH_PWR_DN, 0x00);
	ADS8688_Write_Program(AUTO_SEQ_EN, 0xFF);
	ADS8688_Write_Command(AUTO_RST);
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
	HAL_SPI_Transmit(&SPI2_Handler, wr_data, 2, 0xFFFF);
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
	HAL_SPI_Transmit(&SPI2_Handler, wr_data, 2, 0xFFFF);
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
	HAL_SPI_Transmit(&SPI2_Handler, wr_data, 2, 0xFFFF);
	HAL_SPI_Receive(&SPI2_Handler, &Rxdata, 1, 0xFFFF);
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
	uint8_t Rxdata[4],i;
	uint8_t wr_data[4] = {0x00, 0x00, 0x00, 0x00};
	
	
	for(i=0; i<ch_num; i++) 
	{
		ADS8688_CS = 0;
		HAL_SPI_TransmitReceive(&SPI2_Handler, (uint8_t *)wr_data, (uint8_t *)Rxdata, 4, 0xFFFF);
		ADS8688_CS = 1;
		*(data+i) = ((uint16_t)Rxdata[2] << 8) | Rxdata[3];
	}
}

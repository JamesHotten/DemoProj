//-----------------------------------------------------------------
// 程序描述:
//    ADS8688实验
// 作    者: 凌智电子
// 开始日期: 2022-06-27
// 完成日期: 2022-06-27
//  - V1.0: (2022-06-27)	ADS8688实验
// 调试工具: 凌智STM32核心开发板、LZE_ST LINK2、2.8寸液晶、DAS8361模块
// 说    明: 硬件连接
//							STM32F103							ADS8688模块
//								+5V		<----------->		+5V
//								GND		<----------->		GND
//								PB15	<----------->		SDI
//								PB8		<----------->		RST
//								PB9		<----------->		DAISY
//								PB14	<----------->		SDO
//								PB13	<----------->		SCLK
//								PB12	<----------->		CS
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "stm32f10x.h"
#include "Delay.h"
#include "LED.h"
#include "lze_lcd.h"
#include "ads8688.h"
#include "PeripheralInit.h"

//-----------------------------------------------------------------
// 主程序
//-----------------------------------------------------------------
int main(void)
{
	uint16_t adc_data[8];
	uint8_t dis_buf[40], i;
  PeripheralInit();
  LCD_Clear(LCD_COLOR_WHITE);

	LCD_WriteString(30, 0, LCD_COLOR_RED, LCD_COLOR_WHITE, (uint8_t *)"STM32F103 Core Board");
	LCD_WriteString(30, 20, LCD_COLOR_RED, LCD_COLOR_WHITE, (uint8_t *)"ADS8688 TEST");
	LCD_WriteString(30, 40, LCD_COLOR_RED, LCD_COLOR_WHITE, (uint8_t *)"LZ Electronic");
	LCD_WriteString(30, 60, LCD_COLOR_BLUE, LCD_COLOR_WHITE, (uint8_t *)"CH0:    0.0000V");
	LCD_WriteString(30, 80, LCD_COLOR_BLUE, LCD_COLOR_WHITE, (uint8_t *)"CH1:    0.0000V");
	LCD_WriteString(30,100, LCD_COLOR_BLUE, LCD_COLOR_WHITE, (uint8_t *)"CH2:    0.0000V");
	LCD_WriteString(30,120, LCD_COLOR_BLUE, LCD_COLOR_WHITE, (uint8_t *)"CH3:    0.0000V");
	LCD_WriteString(30,140, LCD_COLOR_BLUE, LCD_COLOR_WHITE, (uint8_t *)"CH4:    0.0000V");
	LCD_WriteString(30,160, LCD_COLOR_BLUE, LCD_COLOR_WHITE, (uint8_t *)"CH5:    0.0000V");
	LCD_WriteString(30,180, LCD_COLOR_BLUE, LCD_COLOR_WHITE, (uint8_t *)"CH6:    0.0000V");
	LCD_WriteString(30,200, LCD_COLOR_BLUE, LCD_COLOR_WHITE, (uint8_t *)"CH7:    0.0000V");
  while (1)
  {
		Get_AUTO_RST_Mode_ADC_Data(8, adc_data);
		for(i=0; i < 8; i++)
		{
			sprintf ((char *)dis_buf,"CH%1d: %8.4lfmV  D: %4X    ", i, ((double)adc_data[i]-32768)*10240.0/65536, (uint16_t)adc_data[i]);
			LCD_WriteString(30,60+i*20, LCD_COLOR_BLUE, LCD_COLOR_WHITE, (uint8_t *)dis_buf);	//先在固定位置显示小数点 
		}
	  Delay_5ms(100);
  }
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------

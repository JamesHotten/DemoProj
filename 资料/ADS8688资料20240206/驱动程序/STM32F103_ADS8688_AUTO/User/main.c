//-----------------------------------------------------------------
// ��������:
//    ADS8688ʵ��
// ��    ��: ���ǵ���
// ��ʼ����: 2022-06-27
// �������: 2022-06-27
//  - V1.0: (2022-06-27)	ADS8688ʵ��
// ���Թ���: ����STM32���Ŀ����塢LZE_ST LINK2��2.8��Һ����DAS8361ģ��
// ˵    ��: Ӳ������
//							STM32F103							ADS8688ģ��
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
// ͷ�ļ�����
//-----------------------------------------------------------------
#include "stm32f10x.h"
#include "Delay.h"
#include "LED.h"
#include "lze_lcd.h"
#include "ads8688.h"
#include "PeripheralInit.h"

//-----------------------------------------------------------------
// ������
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
			LCD_WriteString(30,60+i*20, LCD_COLOR_BLUE, LCD_COLOR_WHITE, (uint8_t *)dis_buf);	//���ڹ̶�λ����ʾС���� 
		}
	  Delay_5ms(100);
  }
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------

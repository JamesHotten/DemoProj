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
	uint16_t adc_data;
	uint16_t aux_data;
	uint8_t dis_buf[40];
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
		// ��ȡͨ��0������
		Get_MAN_CH_Data(MAN_CH_0, &adc_data);
		sprintf ((char *)dis_buf,"CH0: %10.4lfmV  D: %04X", ((double)adc_data-32768)*10240.0/65536, (uint16_t)adc_data);
		LCD_WriteString(30, 60, LCD_COLOR_BLUE, LCD_COLOR_WHITE, (uint8_t *)dis_buf);

		
		// ��ȡͨ��1������
		Get_MAN_CH_Data(MAN_CH_1, &adc_data);
		sprintf ((char *)dis_buf,"CH1: %10.4lfmV  D: %04X", ((double)adc_data-32768)*10240.0/65536, (uint16_t)adc_data);
		LCD_WriteString(30, 80, LCD_COLOR_BLUE, LCD_COLOR_WHITE, (uint8_t *)dis_buf);
		
		// ��ȡͨ��2������
		Get_MAN_CH_Data(MAN_CH_2, &adc_data);
		sprintf ((char *)dis_buf,"CH2: %10.4lfmV  D: %04X", ((double)adc_data-32768)*10240.0/65536, (uint16_t)adc_data);
		LCD_WriteString(30,100, LCD_COLOR_BLUE, LCD_COLOR_WHITE, (uint8_t *)dis_buf);
		
		// ��ȡͨ��3������
		Get_MAN_CH_Data(MAN_CH_3, &adc_data);
		sprintf ((char *)dis_buf,"CH3: %10.4lfmV  D: %04X", ((double)adc_data-32768)*10240.0/65536, (uint16_t)adc_data);
		LCD_WriteString(30,120, LCD_COLOR_BLUE, LCD_COLOR_WHITE, (uint8_t *)dis_buf);
		
		// ��ȡͨ��4������
		Get_MAN_CH_Data(MAN_CH_4, &adc_data);
		sprintf ((char *)dis_buf,"CH4: %10.4lfmV  D: %04X", ((double)adc_data-32768)*10240.0/65536, (uint16_t)adc_data);
		LCD_WriteString(30,140, LCD_COLOR_BLUE, LCD_COLOR_WHITE, (uint8_t *)dis_buf);
		
		// ��ȡͨ��5������
		Get_MAN_CH_Data(MAN_CH_5, &adc_data);
		sprintf ((char *)dis_buf,"CH5: %10.4lfmV  D: %04X", ((double)adc_data-32768)*10240.0/65536, (uint16_t)adc_data);
		LCD_WriteString(30,160, LCD_COLOR_BLUE, LCD_COLOR_WHITE, (uint8_t *)dis_buf);
		
		// ��ȡͨ��6������
		Get_MAN_CH_Data(MAN_CH_6, &adc_data);
		sprintf ((char *)dis_buf,"CH6: %10.4lfmV  D: %04X", ((double)adc_data-32768)*10240.0/65536, (uint16_t)adc_data);
		LCD_WriteString(30,180, LCD_COLOR_BLUE, LCD_COLOR_WHITE, (uint8_t *)dis_buf);
		
		// ��ȡͨ��7������
		Get_MAN_CH_Data(MAN_CH_7, &adc_data);
		sprintf ((char *)dis_buf,"CH7: %10.4lfmV  D: %04X", ((double)adc_data-32768)*10240.0/65536, (uint16_t)adc_data);
		LCD_WriteString(30,200, LCD_COLOR_BLUE, LCD_COLOR_WHITE, (uint8_t *)dis_buf);
		
		// ��ȡAUXͨ�������ݣ����뷶ΧΪ0~4.096V��
		Get_MAN_CH_Data(MAN_AUX, &aux_data);
		Get_MAN_CH_Data(MAN_AUX, &aux_data);
		sprintf ((char *)dis_buf,"AUX: %10.4lfmV  D: %04X", (double)aux_data*4096.0/65536, (uint16_t)aux_data);
		LCD_WriteString(30,220, LCD_COLOR_BLUE, LCD_COLOR_WHITE, (uint8_t *)dis_buf);
		Get_MAN_CH_Data(MAN_CH_0, &adc_data);
		
	  Delay_5ms(100);
  }
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ��������:
//     ADS8688��������ͷ�ļ�
// ��    ��: ���ǵ���
// ��ʼ����: 2022-06-27
// �������: 2022-06-27
// �޸�����: 
// ��ǰ�汾: V1.0
// ��ʷ�汾:
//  - V1.0: (2022-06-27)ADS8688��ʼ����Ӧ��
// ���Թ���: ����ԭ��STM32F429�����ޡ�LZE_ST_LINK2��ADS8688ģ��
// ˵    ��: Ӳ������
//							STM32F429							ADS8688ģ��
//								+5V		<----------->		+5V
//								GND		<----------->		GND
//								PB15	<----------->		SDI
//								PB6		<----------->		RST
//								PB7		<----------->		DAISY
//								PB8		<----------->		CS
//								PB14	<----------->		SDO
//								PB13	<----------->		SCLK
//    
//-----------------------------------------------------------------
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "sdram.h"
#include "ads8688.h"
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ������
//-----------------------------------------------------------------
int main(void)
{
	uint16_t adc_data[8];
	uint8_t dis_buf[40], i;
	Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz
	delay_init(180);                //��ʼ����ʱ����
	uart_init(115200);              //��ʼ��USART
	LED_Init();                     //��ʼ��LED 
	KEY_Init();                     //��ʼ������
	SDRAM_Init();                   //SDRAM��ʼ��
	LCD_Init();                     //LCD��ʼ��
	ADS8688_Init();									// ��ʼ��ADS8688
	LCD_Clear(WHITE);
	POINT_COLOR=RED;
	LCD_ShowString(30,0,240,24,24,(uint8_t *)"Elite STM32F4 ^_^"); 
	LCD_ShowString(30,30,240,24,24,(uint8_t *)"ADS8688 TEST"); 
	LCD_ShowString(30,60,240,24,24,(uint8_t *)"LZ Electronic"); 
	POINT_COLOR=BLUE;
	LCD_ShowString(30,100,240,16,16,(uint8_t *)"CH0:    0.0000mV"); 
	LCD_ShowString(30,120,240,16,16,(uint8_t *)"CH1:    0.0000mV");
	LCD_ShowString(30,140,240,16,16,(uint8_t *)"CH2:    0.0000mV"); 
	LCD_ShowString(30,160,240,16,16,(uint8_t *)"CH3:    0.0000mV"); 
	LCD_ShowString(30,180,240,16,16,(uint8_t *)"CH4:    0.0000mV"); 
	LCD_ShowString(30,200,240,16,16,(uint8_t *)"CH5:    0.0000mV"); 
	LCD_ShowString(30,220,240,16,16,(uint8_t *)"CH6:    0.0000mV"); 
	LCD_ShowString(30,240,240,16,16,(uint8_t *)"CH7:    0.0000mV"); 
  while(1)
  {
		Get_AUTO_RST_Mode_ADC_Data(8, adc_data);
		for(i=0; i < 8; i++)
		{
			sprintf ((char *)dis_buf,"CH%1d: %8.4lfmV    ", i, ((double)adc_data[i]-32768)*10240.0/65536);
			LCD_ShowString(30,100+i*20, 240, 16, 16, (uint8_t *)dis_buf);	//���ڹ̶�λ����ʾС���� 
			printf("%s\r\n", (char *)dis_buf);
		}
		printf("\r\n");		   		 
		delay_ms(500);	
	}
}

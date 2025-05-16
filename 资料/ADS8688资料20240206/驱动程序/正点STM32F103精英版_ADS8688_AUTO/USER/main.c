//-----------------------------------------------------------------
// ��������:
//    ADS8688ʵ��
// ��    ��: ���ǵ���
// ��ʼ����: 2022-06-27
// �������: 2022-06-27
//  - V1.0: (2022-06-27)	ADS8688ʵ��
// ���Թ���: ����ԭ�Ӿ�Ӣ��STM32F103�����塢LZE_ST LINK2��2.8��Һ����DAS8688ģ��
// ˵    ��: Ӳ������
//							STM32F103							ADS8688ģ��
//								+5V		<----------->		+5V
//								GND		<----------->		GND
//								PA7		<----------->		SDI
//								PC0		<----------->		RST
//								PC1		<----------->		DAISY
//								PA6		<----------->		SDO
//								PA5		<----------->		SCLK
//								PC2		<----------->		CS
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "ads8688.h"

//-----------------------------------------------------------------
// ������
//-----------------------------------------------------------------

int main(void)
{	 
	uint16_t adc_data[8];
	uint8_t dis_buf[40], i;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	LCD_Init();
	ADS8688_Init();
	LCD_Clear(WHITE);
	POINT_COLOR=RED;
	LCD_ShowString(30,0,240,24,24,(uint8_t *)"Elite STM32F1 ^_^"); 
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


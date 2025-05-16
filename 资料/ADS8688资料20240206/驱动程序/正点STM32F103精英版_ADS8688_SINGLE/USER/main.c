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
	uint16_t adc_data;
	uint16_t aux_data;
	uint8_t dis_buf[40];
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
		// ��ȡͨ��0������
		Get_MAN_CH_Data(MAN_CH_0, &adc_data);
		sprintf ((char *)dis_buf,"CH0: %10.4lfmV    ", ((double)adc_data-32768)*10240.0/65536);
		LCD_ShowString(30, 100, 240, 16, 16, (uint8_t *)dis_buf);
		printf("%s\r\n", (char *)dis_buf);

		
		// ��ȡͨ��1������
		Get_MAN_CH_Data(MAN_CH_1, &adc_data);
		sprintf ((char *)dis_buf,"CH1: %10.4lfmV    ", ((double)adc_data-32768)*10240.0/65536);
		LCD_ShowString(30, 120, 240, 16, 16, (uint8_t *)dis_buf);
		printf("%s\r\n", (char *)dis_buf);
		
		// ��ȡͨ��2������
		Get_MAN_CH_Data(MAN_CH_2, &adc_data);
		sprintf ((char *)dis_buf,"CH2: %10.4lfmV    ", ((double)adc_data-32768)*10240.0/65536);
		LCD_ShowString(30, 140, 240, 16, 16, (uint8_t *)dis_buf);
		printf("%s\r\n", (char *)dis_buf);
		
		// ��ȡͨ��3������
		Get_MAN_CH_Data(MAN_CH_3, &adc_data);
		sprintf ((char *)dis_buf,"CH3: %10.4lfmV    ", ((double)adc_data-32768)*10240.0/65536);
		LCD_ShowString(30, 160, 240, 16, 16, (uint8_t *)dis_buf);
		printf("%s\r\n", (char *)dis_buf);
		
		// ��ȡͨ��4������
		Get_MAN_CH_Data(MAN_CH_4, &adc_data);
		sprintf ((char *)dis_buf,"CH4: %10.4lfmV    ", ((double)adc_data-32768)*10240.0/65536);
		LCD_ShowString(30, 180, 240, 16, 16, (uint8_t *)dis_buf);
		printf("%s\r\n", (char *)dis_buf);
		
		// ��ȡͨ��5������
		Get_MAN_CH_Data(MAN_CH_5, &adc_data);
		sprintf ((char *)dis_buf,"CH5: %10.4lfmV    ", ((double)adc_data-32768)*10240.0/65536);
		LCD_ShowString(30, 200, 240, 16, 16, (uint8_t *)dis_buf);
		printf("%s\r\n", (char *)dis_buf);
		
		// ��ȡͨ��6������
		Get_MAN_CH_Data(MAN_CH_6, &adc_data);
		sprintf ((char *)dis_buf,"CH6: %10.4lfmV    ", ((double)adc_data-32768)*10240.0/65536);
		LCD_ShowString(30, 220, 240, 16, 16, (uint8_t *)dis_buf);
		printf("%s\r\n", (char *)dis_buf);
		
		// ��ȡͨ��7������
		Get_MAN_CH_Data(MAN_CH_7, &adc_data);
		sprintf ((char *)dis_buf,"CH7: %10.4lfmV    ", ((double)adc_data-32768)*10240.0/65536);
		LCD_ShowString(30, 240, 240, 16, 16, (uint8_t *)dis_buf);
		printf("%s\r\n", (char *)dis_buf);
		
		// ��ȡAUXͨ�������ݣ����뷶ΧΪ0~4.096V��
		Get_MAN_CH_Data(MAN_AUX, &aux_data);
		Get_MAN_CH_Data(MAN_AUX, &aux_data);
		sprintf ((char *)dis_buf,"AUX: %10.4lfmV    ", (double)aux_data*4096.0/65536);
		LCD_ShowString(30, 260, 240, 16, 16, (uint8_t *)dis_buf);
		printf("%s\r\n", (char *)dis_buf);
		Get_MAN_CH_Data(MAN_CH_0, &adc_data);
		printf("\r\n");		   		 
		delay_ms(500);	
	} 
}


//-----------------------------------------------------------------
// ��������:
//     ADS8688ʵ��
// ��    ��: ���ǵ���
// ��ʼ����: 2022-06-27
// �������: 2022-06-27
// �޸�����: 
// ��ǰ�汾: V1.0
// ��ʷ�汾:
//  - V1.0: (2022-06-27)	ADS8688ʵ��
// ���Թ���: ����STM32F429+CycloneIV����ϵͳ��ƿ����塢LZE_ST_LINK2��4.3��/2.8�� TFTҺ������ADS8688ģ��
// ˵    ��: Ӳ������
//						   ������								ADS8688ģ��
//								+5V		<----------->		+5V
//								GND		<----------->		GND
//								PF9		<----------->		SDI
//								PC4		<----------->		RST
//								PC5		<----------->		DAISY
//								PF8		<----------->		SDO
//								PF7		<----------->		SCLK
//								PA4		<----------->		CS
//    
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include "stm32f429_winner.h"
#include "delay.h"
#include "lcd.h"
#include "led.h"
#include "sdram.h"
#include "usart.h"
#include "spi.h"	
#include "ads8688.h"
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ������
//-----------------------------------------------------------------
int main(void)
{
	uint16_t adc_data;
	uint16_t aux_data;
	uint8_t dis_buf[40];
  HAL_Init();          							// ��ʼ��HAL��
	SystemClock_Config(360,25,2,8); 	// ����ϵͳʱ��,180Mhz  
	LED_Init();                  			// ��ʼ��LED 
	uart_init(115200);								// ��ʼ������
	SDRAM_Init();                			// SDRAM��ʼ��
	LCD_Init();                  			// LCD��ʼ��
	ADS8688_Init();					// ��ʼ��ADS8688
	POINT_COLOR=RED; 				// �ʻ���ɫ
	LCD_ShowString(30,0,200,16,16,"STM32H750 Core Board");	
	LCD_ShowString(30,20,200,16,16,"ADS8688 TEST");	
	LCD_ShowString(30,40,200,16,16,"LZ Electronic");
	POINT_COLOR=BLUE;// ��������Ϊ��ɫ   
	while(1)
	{
		// ��ȡͨ��0������
		Get_MAN_CH_Data(MAN_CH_0, &adc_data);
		sprintf ((char *)dis_buf,"CH0: %10.4lfmV  D: %04X", ((double)adc_data-32768)*10240.0/65536, (uint16_t)adc_data);
		LCD_ShowString(30,60,300,16,16,(char *)dis_buf);	//���ڹ̶�λ����ʾС���� 
		printf("%s\r\n", (char *)dis_buf);
		
		// ��ȡͨ��1������
		Get_MAN_CH_Data(MAN_CH_1, &adc_data);
		sprintf ((char *)dis_buf,"CH1: %10.4lfmV  D: %04X", ((double)adc_data-32768)*10240.0/65536, (uint16_t)adc_data);
		LCD_ShowString(30,80,300,16,16,(char *)dis_buf);	//���ڹ̶�λ����ʾС���� 
		printf("%s\r\n", (char *)dis_buf);
		
		// ��ȡͨ��2������
		Get_MAN_CH_Data(MAN_CH_2, &adc_data);
		sprintf ((char *)dis_buf,"CH2: %10.4lfmV  D: %04X", ((double)adc_data-32768)*10240.0/65536, (uint16_t)adc_data);
		LCD_ShowString(30,100,300,16,16,(char *)dis_buf);	//���ڹ̶�λ����ʾС���� 
		printf("%s\r\n", (char *)dis_buf);
		
		// ��ȡͨ��3������
		Get_MAN_CH_Data(MAN_CH_3, &adc_data);
		sprintf ((char *)dis_buf,"CH3: %10.4lfmV  D: %04X", ((double)adc_data-32768)*10240.0/65536, (uint16_t)adc_data);
		LCD_ShowString(30,120,300,16,16,(char *)dis_buf);	//���ڹ̶�λ����ʾС���� 
		printf("%s\r\n", (char *)dis_buf);
		
		// ��ȡͨ��4������
		Get_MAN_CH_Data(MAN_CH_4, &adc_data);
		sprintf ((char *)dis_buf,"CH4: %10.4lfmV  D: %04X", ((double)adc_data-32768)*10240.0/65536, (uint16_t)adc_data);
		LCD_ShowString(30,140,300,16,16,(char *)dis_buf);	//���ڹ̶�λ����ʾС���� 
		printf("%s\r\n", (char *)dis_buf);
		
		// ��ȡͨ��5������
		Get_MAN_CH_Data(MAN_CH_5, &adc_data);
		sprintf ((char *)dis_buf,"CH5: %10.4lfmV  D: %04X", ((double)adc_data-32768)*10240.0/65536, (uint16_t)adc_data);
		LCD_ShowString(30,160,300,16,16,(char *)dis_buf);	//���ڹ̶�λ����ʾС���� 
		printf("%s\r\n", (char *)dis_buf);
		
		// ��ȡͨ��6������
		Get_MAN_CH_Data(MAN_CH_6, &adc_data);
		sprintf ((char *)dis_buf,"CH6: %10.4lfmV  D: %04X", ((double)adc_data-32768)*10240.0/65536, (uint16_t)adc_data);
		LCD_ShowString(30,180,300,16,16,(char *)dis_buf);	//���ڹ̶�λ����ʾС���� 
		printf("%s\r\n", (char *)dis_buf);
		
		// ��ȡͨ��7������
		Get_MAN_CH_Data(MAN_CH_7, &adc_data);
		sprintf ((char *)dis_buf,"CH7: %10.4lfmV  D: %04X", ((double)adc_data-32768)*10240.0/65536, (uint16_t)adc_data);
		LCD_ShowString(30,200,300,16,16,(char *)dis_buf);	//���ڹ̶�λ����ʾС���� 
		printf("%s\r\n", (char *)dis_buf);
		
		// ��ȡAUXͨ�������ݣ����뷶ΧΪ0~4.096V��
		Get_MAN_CH_Data(MAN_AUX, &aux_data);
		Get_MAN_CH_Data(MAN_AUX, &aux_data);
		sprintf ((char *)dis_buf,"AUX: %10.4lfmV  D: %04X", (double)aux_data*4096.0/65536, (uint16_t)aux_data);
		LCD_ShowString(30,220,300,16,16,(char *)dis_buf);	//���ڹ̶�λ����ʾС���� 
		printf("%s\r\n\r\n", (char *)dis_buf);
		Get_MAN_CH_Data(MAN_CH_0, &adc_data);
		
	  delay_ms(500);
	}
}

//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 

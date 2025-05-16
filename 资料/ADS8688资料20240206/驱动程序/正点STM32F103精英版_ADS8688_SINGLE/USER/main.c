//-----------------------------------------------------------------
// 程序描述:
//    ADS8688实验
// 作    者: 凌智电子
// 开始日期: 2022-06-27
// 完成日期: 2022-06-27
//  - V1.0: (2022-06-27)	ADS8688实验
// 调试工具: 正点原子精英版STM32F103开发板、LZE_ST LINK2、2.8寸液晶、DAS8688模块
// 说    明: 硬件连接
//							STM32F103							ADS8688模块
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
// 头文件包含
//-----------------------------------------------------------------
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "ads8688.h"

//-----------------------------------------------------------------
// 主程序
//-----------------------------------------------------------------

int main(void)
{	 
	uint16_t adc_data;
	uint16_t aux_data;
	uint8_t dis_buf[40];
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	LED_Init();			     //LED端口初始化
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
		// 获取通道0的数据
		Get_MAN_CH_Data(MAN_CH_0, &adc_data);
		sprintf ((char *)dis_buf,"CH0: %10.4lfmV    ", ((double)adc_data-32768)*10240.0/65536);
		LCD_ShowString(30, 100, 240, 16, 16, (uint8_t *)dis_buf);
		printf("%s\r\n", (char *)dis_buf);

		
		// 获取通道1的数据
		Get_MAN_CH_Data(MAN_CH_1, &adc_data);
		sprintf ((char *)dis_buf,"CH1: %10.4lfmV    ", ((double)adc_data-32768)*10240.0/65536);
		LCD_ShowString(30, 120, 240, 16, 16, (uint8_t *)dis_buf);
		printf("%s\r\n", (char *)dis_buf);
		
		// 获取通道2的数据
		Get_MAN_CH_Data(MAN_CH_2, &adc_data);
		sprintf ((char *)dis_buf,"CH2: %10.4lfmV    ", ((double)adc_data-32768)*10240.0/65536);
		LCD_ShowString(30, 140, 240, 16, 16, (uint8_t *)dis_buf);
		printf("%s\r\n", (char *)dis_buf);
		
		// 获取通道3的数据
		Get_MAN_CH_Data(MAN_CH_3, &adc_data);
		sprintf ((char *)dis_buf,"CH3: %10.4lfmV    ", ((double)adc_data-32768)*10240.0/65536);
		LCD_ShowString(30, 160, 240, 16, 16, (uint8_t *)dis_buf);
		printf("%s\r\n", (char *)dis_buf);
		
		// 获取通道4的数据
		Get_MAN_CH_Data(MAN_CH_4, &adc_data);
		sprintf ((char *)dis_buf,"CH4: %10.4lfmV    ", ((double)adc_data-32768)*10240.0/65536);
		LCD_ShowString(30, 180, 240, 16, 16, (uint8_t *)dis_buf);
		printf("%s\r\n", (char *)dis_buf);
		
		// 获取通道5的数据
		Get_MAN_CH_Data(MAN_CH_5, &adc_data);
		sprintf ((char *)dis_buf,"CH5: %10.4lfmV    ", ((double)adc_data-32768)*10240.0/65536);
		LCD_ShowString(30, 200, 240, 16, 16, (uint8_t *)dis_buf);
		printf("%s\r\n", (char *)dis_buf);
		
		// 获取通道6的数据
		Get_MAN_CH_Data(MAN_CH_6, &adc_data);
		sprintf ((char *)dis_buf,"CH6: %10.4lfmV    ", ((double)adc_data-32768)*10240.0/65536);
		LCD_ShowString(30, 220, 240, 16, 16, (uint8_t *)dis_buf);
		printf("%s\r\n", (char *)dis_buf);
		
		// 获取通道7的数据
		Get_MAN_CH_Data(MAN_CH_7, &adc_data);
		sprintf ((char *)dis_buf,"CH7: %10.4lfmV    ", ((double)adc_data-32768)*10240.0/65536);
		LCD_ShowString(30, 240, 240, 16, 16, (uint8_t *)dis_buf);
		printf("%s\r\n", (char *)dis_buf);
		
		// 获取AUX通道的数据（输入范围为0~4.096V）
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


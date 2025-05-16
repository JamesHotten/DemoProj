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
	uint16_t adc_data[8];
	uint8_t dis_buf[40], i;
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
		Get_AUTO_RST_Mode_ADC_Data(8, adc_data);
		for(i=0; i < 8; i++)
		{
			sprintf ((char *)dis_buf,"CH%1d: %8.4lfmV    ", i, ((double)adc_data[i]-32768)*10240.0/65536);
			LCD_ShowString(30,100+i*20, 240, 16, 16, (uint8_t *)dis_buf);	//先在固定位置显示小数点 
			printf("%s\r\n", (char *)dis_buf);
		}
		printf("\r\n");		   		 
		delay_ms(500);	
	} 
}


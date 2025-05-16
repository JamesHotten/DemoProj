//-----------------------------------------------------------------
// 程序描述:
//     ADS8688驱动程序头文件
// 作    者: 凌智电子
// 开始日期: 2022-06-27
// 完成日期: 2022-06-27
// 修改日期: 
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2022-06-27)ADS8688初始化及应用
// 调试工具: 正点原子STM32F429阿波罗、LZE_ST_LINK2、ADS8688模块
// 说    明: 硬件连接
//							STM32F429							ADS8688模块
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
// 主程序
//-----------------------------------------------------------------
int main(void)
{
	uint16_t adc_data[8];
	uint8_t dis_buf[40], i;
	Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
	delay_init(180);                //初始化延时函数
	uart_init(115200);              //初始化USART
	LED_Init();                     //初始化LED 
	KEY_Init();                     //初始化按键
	SDRAM_Init();                   //SDRAM初始化
	LCD_Init();                     //LCD初始化
	ADS8688_Init();									// 初始化ADS8688
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
			LCD_ShowString(30,100+i*20, 240, 16, 16, (uint8_t *)dis_buf);	//先在固定位置显示小数点 
			printf("%s\r\n", (char *)dis_buf);
		}
		printf("\r\n");		   		 
		delay_ms(500);	
	}
}

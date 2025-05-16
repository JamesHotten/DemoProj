//-----------------------------------------------------------------
// 程序描述:
//     ADS8688实验
// 作    者: 凌智电子
// 开始日期: 2022-06-27
// 完成日期: 2022-06-27
// 修改日期: 
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2022-06-27)	ADS8688实验
// 调试工具: 凌智STM32F429+CycloneIV电子系统设计开发板、LZE_ST_LINK2、4.3寸/2.8寸 TFT液晶屏、ADS8688模块
// 说    明: 硬件连接
//						   开发板								ADS8688模块
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
// 头文件包含
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
// 主程序
//-----------------------------------------------------------------
int main(void)
{
	uint16_t adc_data;
	uint16_t aux_data;
	uint8_t dis_buf[40];
  HAL_Init();          							// 初始化HAL库
	SystemClock_Config(360,25,2,8); 	// 设置系统时钟,180Mhz  
	LED_Init();                  			// 初始化LED 
	uart_init(115200);								// 初始化串口
	SDRAM_Init();                			// SDRAM初始化
	LCD_Init();                  			// LCD初始化
	ADS8688_Init();					// 初始化ADS8688
	POINT_COLOR=RED; 				// 笔画颜色
	LCD_ShowString(30,0,200,16,16,"STM32H750 Core Board");	
	LCD_ShowString(30,20,200,16,16,"ADS8688 TEST");	
	LCD_ShowString(30,40,200,16,16,"LZ Electronic");
	POINT_COLOR=BLUE;// 设置字体为蓝色   
	while(1)
	{
		// 获取通道0的数据
		Get_MAN_CH_Data(MAN_CH_0, &adc_data);
		sprintf ((char *)dis_buf,"CH0: %10.4lfmV  D: %04X", ((double)adc_data-32768)*10240.0/65536, (uint16_t)adc_data);
		LCD_ShowString(30,60,300,16,16,(char *)dis_buf);	//先在固定位置显示小数点 
		printf("%s\r\n", (char *)dis_buf);
		
		// 获取通道1的数据
		Get_MAN_CH_Data(MAN_CH_1, &adc_data);
		sprintf ((char *)dis_buf,"CH1: %10.4lfmV  D: %04X", ((double)adc_data-32768)*10240.0/65536, (uint16_t)adc_data);
		LCD_ShowString(30,80,300,16,16,(char *)dis_buf);	//先在固定位置显示小数点 
		printf("%s\r\n", (char *)dis_buf);
		
		// 获取通道2的数据
		Get_MAN_CH_Data(MAN_CH_2, &adc_data);
		sprintf ((char *)dis_buf,"CH2: %10.4lfmV  D: %04X", ((double)adc_data-32768)*10240.0/65536, (uint16_t)adc_data);
		LCD_ShowString(30,100,300,16,16,(char *)dis_buf);	//先在固定位置显示小数点 
		printf("%s\r\n", (char *)dis_buf);
		
		// 获取通道3的数据
		Get_MAN_CH_Data(MAN_CH_3, &adc_data);
		sprintf ((char *)dis_buf,"CH3: %10.4lfmV  D: %04X", ((double)adc_data-32768)*10240.0/65536, (uint16_t)adc_data);
		LCD_ShowString(30,120,300,16,16,(char *)dis_buf);	//先在固定位置显示小数点 
		printf("%s\r\n", (char *)dis_buf);
		
		// 获取通道4的数据
		Get_MAN_CH_Data(MAN_CH_4, &adc_data);
		sprintf ((char *)dis_buf,"CH4: %10.4lfmV  D: %04X", ((double)adc_data-32768)*10240.0/65536, (uint16_t)adc_data);
		LCD_ShowString(30,140,300,16,16,(char *)dis_buf);	//先在固定位置显示小数点 
		printf("%s\r\n", (char *)dis_buf);
		
		// 获取通道5的数据
		Get_MAN_CH_Data(MAN_CH_5, &adc_data);
		sprintf ((char *)dis_buf,"CH5: %10.4lfmV  D: %04X", ((double)adc_data-32768)*10240.0/65536, (uint16_t)adc_data);
		LCD_ShowString(30,160,300,16,16,(char *)dis_buf);	//先在固定位置显示小数点 
		printf("%s\r\n", (char *)dis_buf);
		
		// 获取通道6的数据
		Get_MAN_CH_Data(MAN_CH_6, &adc_data);
		sprintf ((char *)dis_buf,"CH6: %10.4lfmV  D: %04X", ((double)adc_data-32768)*10240.0/65536, (uint16_t)adc_data);
		LCD_ShowString(30,180,300,16,16,(char *)dis_buf);	//先在固定位置显示小数点 
		printf("%s\r\n", (char *)dis_buf);
		
		// 获取通道7的数据
		Get_MAN_CH_Data(MAN_CH_7, &adc_data);
		sprintf ((char *)dis_buf,"CH7: %10.4lfmV  D: %04X", ((double)adc_data-32768)*10240.0/65536, (uint16_t)adc_data);
		LCD_ShowString(30,200,300,16,16,(char *)dis_buf);	//先在固定位置显示小数点 
		printf("%s\r\n", (char *)dis_buf);
		
		// 获取AUX通道的数据（输入范围为0~4.096V）
		Get_MAN_CH_Data(MAN_AUX, &aux_data);
		Get_MAN_CH_Data(MAN_AUX, &aux_data);
		sprintf ((char *)dis_buf,"AUX: %10.4lfmV  D: %04X", (double)aux_data*4096.0/65536, (uint16_t)aux_data);
		LCD_ShowString(30,220,300,16,16,(char *)dis_buf);	//先在固定位置显示小数点 
		printf("%s\r\n\r\n", (char *)dis_buf);
		Get_MAN_CH_Data(MAN_CH_0, &adc_data);
		
	  delay_ms(500);
	}
}

//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 

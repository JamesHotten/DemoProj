/**********************************************************
                       康威电子
本程序仅供学习使用，未经作者许可，不得用于其它任何用途										 
功能：ADS8688，16位AD 8通道自动扫描采集   使用芯片内部基准4.096V 
			显示：12864cog
接口：接口请参照ads8688.h  可直接连接本店主控板TLV5608接口
时间：2020/12/13
版本：0.2
作者：康威电子
其他：本程序只做模块功能验证，以便快速测试模块好坏。
			如需进行性能验证，请自行编程.更换高速MCU或FPGA实现高速采集！！！！
			
			当选择量程为双极性时，0V对应32767，因此V=(ADC值-32767)*量程/65535；当量程为单极性时，则无需减去32767。

更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://kvdz.taobao.com

**********************************************************/
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "usart.h"
#include "wdg.h"
#include "usb_lib.h"
#include "math.h"
#include "hw_config.h"
#include "string.h"
#include "stdlib.h"
#include "lcd.h"
#include "ads8688.h"
/**********************************************************
* 函数功能 ---> USB连接/断开
* 入口参数 ---> usb_sw：USB开启/关闭控制。0：关闭
*                                         1：打开
* 返回数值 ---> none
* 功能说明 ---> none
**********************************************************/	   
void USB_Port_Set(u8 usb_sw)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //使能PORTA时钟
		   	 
	if(usb_sw)	_SetCNTR(_GetCNTR()&(~(1<<1)));//退出断电模式
	else
	{	  
		_SetCNTR(_GetCNTR()|(1<<1));  // 断电模式
		GPIOA->CRH &= 0XFFF00FFF;
		GPIOA->CRH |= 0X00033000;
		PAout(12) = 0;	    		  
	}
}
 
void display(void)
{	
	LCD_Show_CEStr(0,0,"I1");
	LCD_Show_CEStr(64,0,"I2");
	LCD_Show_CEStr(0,2,"I3");
	LCD_Show_CEStr(64,2,"I4");
	LCD_Show_CEStr(0,4,"I5");
	LCD_Show_CEStr(64,4,"I6");
	LCD_Show_CEStr(0,6,"I7");
	LCD_Show_CEStr(64,6,"I8");
	LCD_Refresh_Gram();
}	
int main(void)
{
	uint8_t i=0;
	char showLcd[30];
	char infoBackPC[64];
	uint16_t value[8];

	SystemInit();
	delay_init(72);	     //延时初始化
	uart_init(9600);   //串口初始化
	key_init();//按键初始化
	initial_lcd();//液晶初始化
	LCD_Clear();
	LCD_Show_CEStr(10,0,"ADS8688-16bit");//ADS8688 16位adc
	LCD_Show_CEStr(40,2,"Auto");//自动
	LCD_Show_CEStr(40,4,"Unit:1mV");//单位
	LCD_Refresh_Gram();
	delay_ms(1000);
	USB_Port_Set(0); 	//USB先断开
	delay_ms(600);
	USB_Port_Set(1);	//USB再次连接
	USB_Config();
	LCD_Clear();
	
	ADS8688_Init(CH0_EN|CH1_EN|CH2_EN|CH3_EN|CH4_EN|CH5_EN|CH6_EN|CH7_EN);//ADS8688初始化
  Set_CH_Range(CHIR_0,ADS8688_IR_N2_5V);
  Set_CH_Range(CHIR_1,ADS8688_IR_N2_5V);
  Set_CH_Range(CHIR_2,ADS8688_IR_N2_5V);
  Set_CH_Range(CHIR_3,ADS8688_IR_N2_5V);
  Set_CH_Range(CHIR_4,ADS8688_IR_N2_5V);
  Set_CH_Range(CHIR_5,ADS8688_IR_N2_5V);
  Set_CH_Range(CHIR_6,ADS8688_IR_N2_5V);
  Set_CH_Range(CHIR_7,ADS8688_IR_N2_5V);//设置通道输入范围：±10.24V   ±2.5*ref  ref=4.096V  
  AUTO_RST_Mode();//进入自动扫描模式
	display();
  while(1)
  {
		delay_ms(10);
    Get_AUTO_RST_Mode_Data(value,8);//自动扫描模式，自动扫描并转换8通道。数据存在value数组中
		for(i = 0; i < 4; i++) //显示
		{
			sprintf(showLcd, "%06d", (int16_t)((value[i*2]-32767)*CONST_N2_5V_LSB_mV));//测量范围为±需要减去32767
			LCD_ShowString(0,16 , i*16, (const u8*)showLcd);
			
			sprintf(showLcd, "%06d", (int16_t)((value[i*2+1]-32767)*CONST_N2_5V_LSB_mV));	
			LCD_ShowString(0,80 , i*16, (const u8*)showLcd);
		}
		LCD_Refresh_Gram();
		for(i = 0; i < 4; i++)//发送
		{
			sprintf(infoBackPC, "AIN%d=%06dmV  AIN%d=%06dmV\r\n",i*2+1,(int16_t)((value[i*2]-32767)*CONST_N2_5V_LSB_mV),i*2+2,(int16_t)((value[i*2+1]-32767)*CONST_N2_5V_LSB_mV));			
			USB_TxWrite((uint8_t*)infoBackPC, strlen(infoBackPC)); //usb发送
			printf("%s",infoBackPC);  //串口发送	
		}
		USB_TxWrite("\r\n", 2); //usb发送
		printf("\r\n");  //串口发送			
  }		
}

 












 
 
 
 
 
 
 

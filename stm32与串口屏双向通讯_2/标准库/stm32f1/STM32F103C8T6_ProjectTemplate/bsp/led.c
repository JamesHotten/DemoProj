#include "led.h"


/********************************************************
函数名：  	led_init
作者：
日期：    	2024.08.21
功能：    	初始化led引脚
输入参数：	
返回值： 		
修改记录：
**********************************************************/
void led_init()
{

	//初始化小板上的led(PC13)
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_LED_PC13, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_LED_PC13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORT_LED_PC13, &GPIO_InitStructure);
	GPIO_SetBits(PORT_LED_PC13,GPIO_InitStructure.GPIO_Pin);



	
}








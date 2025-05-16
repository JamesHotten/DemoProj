/**********************************************************
                       康威电子
功能：stm32f103rct6控制AD9910模块输出方波
接口：控制引脚接口请参照AD9910.H
时间：
版本：0.6
作者：康威电子
其他：

更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://kvdz.taobao.com/ 
**********************************************************/

#include "stm32_config.h"
#include "AD9910.h"

int main(void)
{
	MY_NVIC_PriorityGroup_Config(NVIC_PriorityGroup_2);	//设置中断分组
	delay_init(72);	//初始化延时函数
	delay_ms(300);	//延时一会儿，等待上电稳定
	
	
	
	//代码移植建议
	//1.修改头文件AD9910.H中，自己控制板实际需要使用哪些控制引脚。如UP_DAT脚改成PC3控制，则定义"#define UP_DAT PCout(3)" 
	//2.修改C文件AD9910V1.C中，AD9110_IOInit函数，所有用到管脚的GPIO输出功能初始化
	//3.完成
	
	
	Init_AD9910();					//AD9910控制脚及寄存器初始化
	AD9910_RAM_WAVE_Set(SQUARE_WAVE);	//设置模块输出方波(TRIG_WAVE：三角波，SQUARE_WAVE：方波，SINC_WAVE：SINC波)
	
	while(1);

}


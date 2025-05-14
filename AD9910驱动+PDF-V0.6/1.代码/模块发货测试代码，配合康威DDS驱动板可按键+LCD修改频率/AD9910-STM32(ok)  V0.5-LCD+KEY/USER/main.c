/**********************************************************
                       康威电子
功能：stm32f103rct6控制AD9910模块输出正弦波
接口：控制引脚接口请参照AD9910.H
时间：
版本：0.6
作者：康威电子
其他：

更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://kvdz.taobao.com/ 
**********************************************************/

#include "stm32_config.h"
#include "stdio.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "AD9910.h"
#include "task_manage.h"
#include "timer.h"

char str[30];	//显示缓存
extern u8 _return;
int main(void)
{
	u16 i=0;

	MY_NVIC_PriorityGroup_Config(NVIC_PriorityGroup_2);	//设置中断分组
	delay_init(72);	//初始化延时函数
	LED_Init();	//初始化LED接口
	key_init();//按键初始化
	initial_lcd();//液晶初始化
	LCD_Clear();
	delay_ms(300);
	LCD_Refresh_Gram();
	
	Timerx_Init(99,71);
	
	LCD_Clear();
	LCD_Show_CEStr(0,0,"5");//橙色 orange
	
	Init_ad9910();//9910初始化
	Freq_convert(1000000);//写频率1MHz
	AD9910_AmpWrite(16383);//0X0000~16383对应峰峰值0mv~800mv(左右)
	
	//1、
	//后续代码涉及界面及按键交互功能，频率或幅度或其他参数可能被重写，可能导致上面更改参数无效
	//上述情况，取消下面注释即可，可直接更改频率及幅度，
//	Freq_convert(10000);	//写频率10kHz
//	AD9910_AmpWrite(16383);//0X0000~16383对应峰峰值0mv~800mv(左右)
//	while(1);

	//2、
//	关于扫频的说明
//	该程序的扫频功能利用定时器中断，不断写入自加的频率实现，
//	在timer.C的TIM4_IRQHandler中将自加后的频率写入
	while(1)
	{
		KeyRead();//读取按键值 ） 
		Set_PointFre(Keycode, 0);//按键处理
		if(_return){_return=0;LCD_Refresh_Gram();}//更新显示
		KEY_EXIT();
	}	
}


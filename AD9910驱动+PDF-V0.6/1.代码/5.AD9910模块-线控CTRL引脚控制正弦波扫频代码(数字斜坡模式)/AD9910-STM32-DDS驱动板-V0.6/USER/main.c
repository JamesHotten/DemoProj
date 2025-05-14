/**********************************************************
                       康威电子
功能：stm32f103rct6控制AD9910模块DRCTL引脚控制正弦波扫频输出
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

	
	//设置数字斜坡频率扫频，并使能自动双向扫频
	AD9910_DRG_FreInit_AutoSet(DISABLE);		//ENABLE,自动扫频，无需外加控制；DISABLE,手动扫频，由DRCTL引脚控制扫频
	
	// 设置（下限频率，上限频率，上扫频频率步进，下扫频频率步进，上扫频频点维持时间，下扫频频点维持时间）
	AD9910_DRG_FrePara_Set(100000, 100000000, 100, 100, 100,100);						//慢速扫频，方便观察， 约800mS,扫频时间计算参考函数注解

	while(1)
	{
		DRCTL = 1;
		delay_ms(500);
		DRCTL = 0;
		delay_ms(500);
	}
}

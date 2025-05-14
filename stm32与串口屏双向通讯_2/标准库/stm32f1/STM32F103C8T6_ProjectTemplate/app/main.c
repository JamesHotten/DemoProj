/*
 * 淘晶驰串口通讯演示工程
 * 淘晶驰wiki网址：http://wiki.tjc1688.com/
 * stm32的GND接串口屏或串口工具的GND,共地
 * stm32的TX2(PA2)接串口屏或串口工具的RX
 * stm32的RX2(PA3)接串口屏或串口工具的TX
 * stm32的5V接串口屏的5V,如果是串口工具,不用接5V也可以
 * 
 * 使用的资源有
 * USART1(PA9,PA10):可以通过printf指令输出至USART1来打印调试信息
 * USART2(PA2,PA3):可以通过TJCPrintf指令输出至USART2来向串口屏发送数据
 * TIM2定时器:用于获取当前的毫秒数,当now_time-last_time >= 1000也就是1秒时,向屏幕发送1次数据
 * LED(PC13):用于提示用户当前灯的状态
 */
#include "stm32f10x.h"
#include "stdio.h"
#include "board.h"
#include "tjc_usart_hmi.h"
#include "led.h"
#include "timer2.h"


#define FRAME_LENGTH 7



int main(void)
{
	uint32_t now_time = 0;
	uint32_t last_time = 0;
	uint32_t  a = 0;
	//int a=0;
	board_init();
	led_init();
	uart1_init(115200);
	uart2_init(115200);
	initRingBuff();
	
  timer1_init();

	
	while(1)
	{

		get_time(&now_time);
		if(now_time-last_time >= 1000)
		{
			last_time = now_time;
			TJCPrintf("n0.val=%d",a);
			TJCPrintf("t0.txt=\"现在是%d\"",a);			
			//set_led_pc13(a % 2);  //翻转LED
			TJCPrintf("click b0,1");
			delay_ms(50);
			TJCPrintf("click b0,0");
			a++; 

		}
		

	//stm32f103的GND接串口屏或串口工具的GND,共地
	//stm32f103的TX1(PA9)接串口屏或串口工具的RX
	//stm32f103的RX1(PA10)接串口屏或串口工具的TX
	//stm32f103的5V接串口屏的5V,如果是串口工具,不用接5V也可以

   //串口数据格式：
   //串口数据帧长度：7字节
   //帧头     参数1    参数2   参数3       帧尾
   //0x55     1字节   1字节    1字节     0xffffff
   //当参数是01时
   //帧头     参数1    参数2   参数3       帧尾
   //0x55     01     led编号  led状态    0xffffff
   //例子1：上位机代码  printh 55 01 01 00 ff ff ff  含义：1号led关闭
   //例子2：上位机代码  printh 55 01 04 01 ff ff ff  含义：4号led打开
   //例子3：上位机代码  printh 55 01 00 01 ff ff ff  含义：0号led打开
   //例子4：上位机代码  printh 55 01 04 00 ff ff ff  含义：4号led关闭

   //当参数是02或03时
   //帧头     参数1    参数2   参数3       帧尾
   //0x55     02/03   滑动值    00    0xffffff
   //例子1：上位机代码  printh 55 02 64 00 ff ff ff  含义：h0.val=100
   //例子2：上位机代码  printh 55 02 00 00 ff ff ff  含义：h0.val=0
   //例子3：上位机代码  printh 55 03 64 00 ff ff ff  含义：h1.val=100
   //例子4：上位机代码  printh 55 03 00 00 ff ff ff  含义：h1.val=0

	  //当串口缓冲区大于等于一帧的长度时
	  while(usize >= FRAME_LENGTH)
	  {
		  //校验帧头帧尾是否匹配
		  if(usize >= FRAME_LENGTH && u(0) == 0x55 && u(4) == 0xff && u(5) == 0xff && u(6) == 0xff)
		  {
				//匹配，进行解析
				if(u(1) == 0x01)
				{
					TJCPrintf("msg.txt=\"led %d is %s\"", u(2), u(3) ? "on" : "off");
					if(u(2) == 0x00)
					{
						set_led_pc13(u(3));
					}
				}else if(u(1) == 0x02)
				{
					//下发的是h0进度条的信息
					TJCPrintf("msg.txt=\"h0.val is %d\"", u(2));
					
				}else if(u(1) == 0x03)
				{
					//下发的是h1进度条的信息
					TJCPrintf("msg.txt=\"h1.val is %d\"", u(2));
				}

				udelete(7);	//删除解析过的数据
			  
		  }else
		  {
			  
				//不匹配删除1字节
				udelete(1);
			  break;
		  }

	  }
	}
	
}

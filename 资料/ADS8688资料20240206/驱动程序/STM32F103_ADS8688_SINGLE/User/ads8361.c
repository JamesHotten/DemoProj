//-----------------------------------------------------------------
// 程序描述:
//     ADS8361驱动程序
// 作    者: 凌智电子
// 开始日期: 2022-06-27
// 完成日期: 2022-06-27
// 修改日期: 
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2022-06-27)ADS8361初始化及应用
// 调试工具: 凌智STM32核心开发板、LZE_ST LINK2、2.8寸液晶、DAS8361模块
// 说    明: 
//    
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include <stm32f10x.h>
#include "ads8361.h"
#include "delay.h"
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void GPIO_ADS8361_Configuration(void)
//-----------------------------------------------------------------
// 
// 函数功能: ADS8361引脚配置函数
// 入口参数: 无 
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void GPIO_ADS8361_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	  // 使能IO口时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9 |
																	GPIO_Pin_10| GPIO_Pin_11|
																	GPIO_Pin_13| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//-----------------------------------------------------------------
// void ADS8361_Init(void)
//-----------------------------------------------------------------
// 
// 函数功能: ADS8361引脚配置函数
// 入口参数: 无 
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void ADS8361_Init(void)
{
	GPIO_ADS8361_Configuration();
	
	M0_H;
	M1_H;
	A0_L;
	CS_H;	
}

//-----------------------------------------------------------------
// void ADS8361_Read_Data_Mode4(uint16_t *Data_A, uint16_t *Data_B)
//-----------------------------------------------------------------
// 
// 函数功能: 读取通道数据（模式：M0=1, M1=1, A=0）
// 入口参数: Data_A：通道A读取的数据
//					 Data_B：通道B读取的数据
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void ADS8361_Read_Data_Mode4(uint16_t *Data_A, uint16_t *Data_B)
{
	uint8_t i,j; 	
	uint16_t DAT = 0;
	uint16_t CH = 0;
	uint16_t AB = 0;

	CS_L;
	for(j=0; j<4; j++)
	{
		DAT = 0;
		for(i=0; i<20; i++)
		{
			CLK_H;
			if(i == 0)
			{
				RD_CONVST_H;
				M0_H;
				M1_H;
				A0_L;
				CLK_L;
				__NOP();
			}
			else if(i == 1)
			{
				RD_CONVST_L;
				CLK_L;
				CH = (uint16_t)(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14));
			}
			else if(i == 2)
			{
				CLK_L;
				AB = (uint16_t)(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14));
			}
			else if(i < 19)
			{
				CLK_L;
				DAT = DAT << 1;

				if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14) == Bit_SET)
					DAT++;

			}
			else
			{
				CLK_L;
				__NOP();
			}
		}
		if((CH == 0) && (AB == 0))
			Data_A[0] = (uint16_t)DAT;
		else if((CH == 0) && (AB == 1))
			Data_B[0] = (uint16_t)DAT;
		else if((CH == 1) && (AB == 0))
			Data_A[1] = (uint16_t)DAT;
		else if((CH == 1) && (AB == 1))
			Data_B[1] = (uint16_t)DAT;
	}
	CS_H;	
}

//-----------------------------------------------------------------
// void ADS8361_Read_Data_Mode3(uint16_t *Data_A, uint16_t *Data_B)
//-----------------------------------------------------------------
// 
// 函数功能: 读取通道数据（模式：M0=1, M1=0, A=0）
// 入口参数: Data_A：通道A读取的数据
//					 Data_B：通道B读取的数据
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void ADS8361_Read_Data_Mode3(uint16_t *Data_A, uint16_t *Data_B)
{
	uint8_t i,j; 	
	uint16_t DAT_A = 0;
	uint16_t DAT_B = 0;
	uint16_t CH = 0;

	CS_L;
	for(j=0; j<2; j++)
	{
		DAT_A = 0;
		DAT_B = 0;
		for(i=0; i<20; i++)
		{
			CLK_H;
			if(i == 0)
			{
				RD_CONVST_H;
				M0_H;
				M1_L;
				A0_L;
				CLK_L;
				__NOP();
			}
			else if(i == 1)
			{
				RD_CONVST_L;
				CLK_L;
				CH = (uint16_t)(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14));
			}
			else if(i == 2)
			{
				CLK_L;
				__NOP();
			}
			else if(i < 19)
			{
				CLK_L;
				DAT_A = DAT_A << 1;
				DAT_B = DAT_B << 1;

				if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14) == Bit_SET)
					DAT_A++;
				if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) == Bit_SET)
					DAT_B++;
			}
			else
			{
				CLK_L;
				__NOP();
			}
		}
		if(CH == 0)
		{
			Data_A[0] = (uint16_t)DAT_A;
			Data_B[0] = (uint16_t)DAT_B;
		}
		else
		{
			Data_A[1] = (uint16_t)DAT_A;
			Data_B[1] = (uint16_t)DAT_B;
		}
	}
	CS_H;	
}

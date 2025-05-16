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
#ifndef _ADS8361_H
#define _ADS8361_H
#include <stdio.h>
#include <stdint.h>

//-----------------------------------------------------------------
// 引脚配置
//-----------------------------------------------------------------
#define CLK_L 	 		  GPIO_ResetBits(GPIOB, GPIO_Pin_13)
#define CLK_H 	 		  GPIO_SetBits(GPIOB, GPIO_Pin_13)
#define CS_L 	 		  	GPIO_ResetBits(GPIOB, GPIO_Pin_11)
#define CS_H 	 		  	GPIO_SetBits(GPIOB, GPIO_Pin_11)
#define RD_CONVST_L 	GPIO_ResetBits(GPIOB, GPIO_Pin_15)
#define RD_CONVST_H 	GPIO_SetBits(GPIOB, GPIO_Pin_15)
#define A0_L 	 		  	GPIO_ResetBits(GPIOB, GPIO_Pin_8)
#define A0_H 	 		  	GPIO_SetBits(GPIOB, GPIO_Pin_8)
#define M0_L 	 		  	GPIO_ResetBits(GPIOB, GPIO_Pin_9)
#define M0_H 	 		  	GPIO_SetBits(GPIOB, GPIO_Pin_9)
#define M1_L 	 		  	GPIO_ResetBits(GPIOB, GPIO_Pin_10)
#define M1_H 	 		  	GPIO_SetBits(GPIOB, GPIO_Pin_10)

//-----------------------------------------------------------------
// 外部函数声明
//-----------------------------------------------------------------
extern void GPIO_ADS8361_Configuration(void);
extern void ADS8361_Init(void);
extern void ADS8361_Read_Data_Mode4(uint16_t *Data_A, uint16_t *Data_B);
extern void ADS8361_Read_Data_Mode3(uint16_t *Data_A, uint16_t *Data_B);

#endif
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------

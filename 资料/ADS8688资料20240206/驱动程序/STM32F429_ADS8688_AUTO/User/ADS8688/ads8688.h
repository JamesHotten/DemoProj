//-----------------------------------------------------------------
// 程序描述:
//     ADS8688驱动程序
// 作    者: 凌智电子
// 开始日期: 2022-06-27
// 完成日期: 2022-06-27
// 修改日期: 
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2022-06-27)ADS8688初始化及应用
// 调试工具: 凌智STM32F429+CycloneIV电子系统设计开发板、LZE_ST_LINK2、ADS8688模块
// 说    明: 
//    
//-----------------------------------------------------------------
#ifndef _ADS8688_H
#define _ADS8688_H
#include "stm32f429_winner.h"

//-----------------------------------------------------------------
// 宏定义
//-----------------------------------------------------------------
// 命令寄存器
#define NO_OP					0x0000	// 继续操作
#define STDBY					0x8200	// 进入待机状态
#define PWR_DN				0x8300	// 设备断电
#define RST						0x8500	// 复位
#define AUTO_RST			0xA000	// 重启后启动自动模式
#define MAN_CH_0			0xC000	// 选择通道0输入
#define MAN_CH_1			0xC400	// 选择通道1输入
#define MAN_CH_2			0xC800	// 选择通道2输入
#define MAN_CH_3			0xCC00	// 选择通道3输入
#define MAN_CH_4			0xD000	// 选择通道4输入
#define MAN_CH_5			0xD400	// 选择通道5输入
#define MAN_CH_6			0xD800	// 选择通道6输入
#define MAN_CH_7			0xDC00	// 选择通道7输入
#define MAN_AUX				0xE000	// 选择通道AUX输入

// 程序寄存器
#define AUTO_SEQ_EN							0x01	// 自动扫描排序控制寄存器
#define CH_PWR_DN			 					0x02	// 通道掉电寄存器
#define FEATURE_SELECT 					0x03	// 器件特性选择控制寄存器
#define CH0_INPUT_RANGE 				0x05	// 通道0输入范围选择寄存器
#define CH1_INPUT_RANGE 				0x06	// 通道1输入范围选择寄存器
#define CH2_INPUT_RANGE 				0x07	// 通道2输入范围选择寄存器
#define CH3_INPUT_RANGE 				0x08	// 通道3输入范围选择寄存器
#define CH4_INPUT_RANGE 				0x09	// 通道4输入范围选择寄存器
#define CH5_INPUT_RANGE 				0x0A	// 通道5输入范围选择寄存器
#define CH6_INPUT_RANGE 				0x0B	// 通道6输入范围选择寄存器
#define CH7_INPUT_RANGE 				0x0C	// 通道7输入范围选择寄存器
#define CH0_HYSTERESIS 					0x15	//
#define CH0_HIGH_THRESHOLD_MSB 	0x16	//
#define CH0_HIGH_THRESHOLD_LSB 	0x17	//
#define CH0_LOW_THRESHOLD_MSB 	0x18	//
#define CH0_LOW_THRESHOLD_LSB 	0x19	//
#define CH7_HYSTERESIS 					0x38	//
#define CH7_HIGH_THRESHOLD_MSB 	0x39	//
#define CH7_HIGH_THRESHOLD_LSB 	0x3A	//
#define CH7_LOW_THRESHOLD_MSB 	0x3B	//
#define CH7_LOW_THRESHOLD_LSB 	0x3C	//
#define COMMAND_READ_BACK 			0x3F	// 命令回读寄存器，只读

// 输入范围（VREF = 4.096V）
#define VREF_B_25								0x00	// 通道输入范围±2.5*VREF
#define VREF_B_125							0x01	// 通道输入范围±1.25*VREF
#define VREF_B_0625							0x02	// 通道输入范围±0.625*VREF
#define VREF_U_25								0x05	// 通道输入范围2.5*VREF
#define VREF_U_125							0x06	// 通道输入范围1.25*VREF

//-----------------------------------------------------------------
// 引脚配置
//-----------------------------------------------------------------
#define RST_L 	 		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4,  GPIO_PIN_RESET)
#define RST_H 	 		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4,  GPIO_PIN_SET)
#define DAISY_L 	 		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET)
#define DAISY_H 	 		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET)
#define CS_L 	 		  	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define CS_H 	 		  	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)


//-----------------------------------------------------------------
// 外部函数声明
//-----------------------------------------------------------------
extern void GPIO_ADS8688_Configuration(void);
extern void ADS8688_Init(void);
extern void ADS8688_Write_Command(uint16_t com);
extern void ADS8688_Write_Program(uint8_t addr, uint8_t data);
extern uint8_t ADS8688_Read_Program(uint8_t addr);
extern void Get_AUTO_RST_Mode_ADC_Data(uint8_t ch_num, uint16_t *data);

#endif
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------

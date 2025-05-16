/*
****************************************************************************
	*文件：ad9910.c
	*描述：修改自 康威科技AD9910 stm32f103驱动
						1、将f103改成f407驱动
						2、将标准库改为Hal库驱动
						

****************************************************************************
*/
#ifndef __AD9910_H__
#define __AD9910_H__

#include "stm32f4xx.h"
#include "sys.h"

#define uchar unsigned char
#define uint  unsigned int	
#define ulong  unsigned long int						

/*
#define DROVER 			PBout(1)   
#define UP_DAT 			PBout(2)    
#define AD9910_PWR 		PBout(5)	 
#define AD9910_SDIO 	PBout(6)    
#define DRHOLD 			PBout(7)   
#define PROFILE1 		PBout(12) 

#define MAS_REST 		PAout(2)  
#define SCLK 			PAout(3)     
#define DRCTL  			PAout(4)   
#define OSK 			PAout(5)       
#define PROFILE0 		PAout(6)  
#define PROFILE2 		PAout(7)  
#define CS  			PAout(8)      
*/

#define 	RESET 	 GPIO_PIN_RESET
#define 	SET 	 GPIO_PIN_SET


#define DROVER_L			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,RESET)	  
#define DROVER_H			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,SET)	  
#define UP_DAT_L 			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,RESET)	
#define UP_DAT_H 			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,SET)	
#define AD9910_PWR_L 		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,RESET)	
#define AD9910_PWR_H 		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,SET)	
#define AD9910_SDIO_L 	   	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,RESET)	
#define AD9910_SDIO_H 	   	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,SET)	
#define DRHOLD_L 			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,RESET)	 
#define DRHOLD_H 			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,SET)	 
#define PROFILE1_L 			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,RESET)	
#define PROFILE1_H 			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,SET)	
#define MAS_REST_L 			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,RESET)
#define MAS_REST_H			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,SET)
#define SCLK_L 				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,RESET)
#define SCLK_H 				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,SET)
#define DRCTL_L  			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,RESET)  
#define DRCTL_H 			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,SET)  
#define OSK_L 				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,RESET)  
#define OSK_H				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,SET)  
#define PROFILE0_L			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,RESET)   
#define PROFILE0_H			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,SET)   
#define PROFILE2_L 			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,RESET)  
#define PROFILE2_H 			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,SET)  
#define CS_L  				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,RESET) 
#define CS_H  				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,SET) 


typedef enum {
	TRIG_WAVE = 0,
	SQUARE_WAVE,
	SINC_WAVE,
} AD9910_WAVE_ENUM;//AD9910RAM模式的波形输出


           
void Init_ad9910(void);
void Freq_convert(ulong Freq);//写频率
void txd_8bit(uchar txdat);
void Txcfr(void);
void AD9910_RAM_WAVE_Set(AD9910_WAVE_ENUM wave);//RAM模式
void Write_Amplitude(uint16_t Amp); //写幅度，输入范围：1-650 mV
void AD9910_DRG_FreInit_AutoSet(FunctionalState autoSweepEn);//开启AD9910的数字斜坡模式DRG模式使能，使能数字斜坡控制频率，根据参数设置是否自动扫频
void AD9910_DRG_FrePara_Set(u32 lowFre, u32 upFre, u32 posStep, u32 negStep, u16 posRate, u16 negRate);//按设定的频率上限，频率下限，上扫频步进，下扫频步进，及上扫频频点停留时间，下扫频频点停留时间，进行自动上下限循环扫频




//扫频波下限频率，上限频率，频率步进（单位：Hz），步进时间间隔（单位：ns，范围：4*(1~65536)ns）
//void SweepFre(ulong SweepMinFre, ulong SweepMaxFre, ulong SweepStepFre, ulong SweepTime);
//void Square_wave(uint Sample_interval);//方波，采样时间间隔输入范围：4*(1~65536)ns




#endif



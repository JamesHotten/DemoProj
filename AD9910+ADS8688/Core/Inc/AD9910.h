/*
 ****************************************************************************
 *�ļ���ad9910.c
 *�������޸��� �����Ƽ�AD9910 stm32f103����
 1����f103�ĳ�f407����
 2������׼���ΪHal������


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

 #define MAS_REST 		PDout(0)
 #define SCLK 			PDout(1)
 #define DRCTL  			PDout(2)
 #define OSK 			PDout(3)
 #define PROFILE0 		PDout(4)
 #define PROFILE2 		PDout(5)
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
#define MAS_REST_L 			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,RESET)
#define MAS_REST_H			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,SET)
#define SCLK_L 				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,RESET)
#define SCLK_H 				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,SET)
#define DRCTL_L  			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,RESET)
#define DRCTL_H 			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,SET)
#define OSK_L 				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,RESET)
#define OSK_H				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,SET)
#define PROFILE0_L			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,RESET)
#define PROFILE0_H			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,SET)
#define PROFILE2_L 			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_5,RESET)
#define PROFILE2_H 			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_5,SET)
#define CS_L  				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,RESET)
#define CS_H  				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,SET)

typedef enum {
	TRIG_WAVE = 0, SQUARE_WAVE, SINC_WAVE,
} AD9910_WAVE_ENUM; //AD9910RAMģʽ�Ĳ������

void Init_ad9910(void);
void Freq_convert(ulong Freq); //дƵ��
void txd_8bit(uchar txdat);
void Txcfr(void);
void AD9910_RAM_WAVE_Set(AD9910_WAVE_ENUM wave); //RAMģʽ
void Write_Amplitude(uint16_t Amp); //д���ȣ����뷶Χ��1-650 mV
void AD9910_DRG_FreInit_AutoSet(FunctionalState autoSweepEn); //����AD9910������б��ģʽDRGģʽʹ�ܣ�ʹ������б�¿���Ƶ�ʣ����ݲ��������Ƿ��Զ�ɨƵ
void AD9910_DRG_FrePara_Set(u32 lowFre, u32 upFre, u32 posStep, u32 negStep,
		u16 posRate, u16 negRate); //���趨��Ƶ�����ޣ�Ƶ�����ޣ���ɨƵ��������ɨƵ����������ɨƵƵ��ͣ��ʱ�䣬��ɨƵƵ��ͣ��ʱ�䣬�����Զ�������ѭ��ɨƵ

//ɨƵ������Ƶ�ʣ�����Ƶ�ʣ�Ƶ�ʲ�������λ��Hz��������ʱ��������λ��ns����Χ��4*(1~65536)ns��
//void SweepFre(ulong SweepMinFre, ulong SweepMaxFre, ulong SweepStepFre, ulong SweepTime);
//void Square_wave(uint Sample_interval);//����������ʱ�������뷶Χ��4*(1~65536)ns

#endif


//-----------------------------------------------------------------
// ��������:
//     ADS8361��������
// ��    ��: ���ǵ���
// ��ʼ����: 2022-06-27
// �������: 2022-06-27
// �޸�����: 
// ��ǰ�汾: V1.0
// ��ʷ�汾:
//  - V1.0: (2022-06-27)ADS8361��ʼ����Ӧ��
// ���Թ���: ����STM32���Ŀ����塢LZE_ST LINK2��2.8��Һ����DAS8361ģ��
// ˵    ��: 
//    
//-----------------------------------------------------------------
#ifndef _ADS8361_H
#define _ADS8361_H
#include <stdio.h>
#include <stdint.h>

//-----------------------------------------------------------------
// ��������
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
// �ⲿ��������
//-----------------------------------------------------------------
extern void GPIO_ADS8361_Configuration(void);
extern void ADS8361_Init(void);
extern void ADS8361_Read_Data_Mode4(uint16_t *Data_A, uint16_t *Data_B);
extern void ADS8361_Read_Data_Mode3(uint16_t *Data_A, uint16_t *Data_B);

#endif
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------

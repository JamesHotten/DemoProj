//-----------------------------------------------------------------
// ��������:
//     ADS8688��������
// ��    ��: ���ǵ���
// ��ʼ����: 2022-06-27
// �������: 2022-06-27
// �޸�����:
// ��ǰ�汾: V1.0
// ��ʷ�汾:
//  - V1.0: (2022-06-27)ADS8688��ʼ����Ӧ��
// ���Թ���: ����STM32H750���İ塢LZE_ST_LINK2��ADS8688ģ��
// ˵    ��:
//
//-----------------------------------------------------------------
#ifndef _ADS8688_H
#define _ADS8688_H
#include "gpio.h"
#include "main.h"
#include <stdint.h>
//-----------------------------------------------------------------
// �궨��
//-----------------------------------------------------------------
// ����Ĵ���
#define NO_OP					0x0000	// ��������
#define STDBY					0x8200	// �������״̬
#define PWR_DN				0x8300	// �豸�ϵ�
#define RST						0x8500	// ��λ
#define AUTO_RST			0xA000	// �����������Զ�ģʽ
#define MAN_CH_0			0xC000	// ѡ��ͨ��0����
#define MAN_CH_1			0xC400	// ѡ��ͨ��1����
#define MAN_CH_2			0xC800	// ѡ��ͨ��2����
#define MAN_CH_3			0xCC00	// ѡ��ͨ��3����
#define MAN_CH_4			0xD000	// ѡ��ͨ��4����
#define MAN_CH_5			0xD400	// ѡ��ͨ��5����
#define MAN_CH_6			0xD800	// ѡ��ͨ��6����
#define MAN_CH_7			0xDC00	// ѡ��ͨ��7����
#define MAN_AUX				0xE000	// ѡ��ͨ��AUX����

// ����Ĵ���
#define AUTO_SEQ_EN							0x01	// �Զ�ɨ��������ƼĴ���
#define CH_PWR_DN			 					0x02	// ͨ������Ĵ���
#define FEATURE_SELECT 					0x03	// ��������ѡ����ƼĴ���
#define CH0_INPUT_RANGE 				0x05	// ͨ��0���뷶Χѡ��Ĵ���
#define CH1_INPUT_RANGE 				0x06	// ͨ��1���뷶Χѡ��Ĵ���
#define CH2_INPUT_RANGE 				0x07	// ͨ��2���뷶Χѡ��Ĵ���
#define CH3_INPUT_RANGE 				0x08	// ͨ��3���뷶Χѡ��Ĵ���
#define CH4_INPUT_RANGE 				0x09	// ͨ��4���뷶Χѡ��Ĵ���
#define CH5_INPUT_RANGE 				0x0A	// ͨ��5���뷶Χѡ��Ĵ���
#define CH6_INPUT_RANGE 				0x0B	// ͨ��6���뷶Χѡ��Ĵ���
#define CH7_INPUT_RANGE 				0x0C	// ͨ��7���뷶Χѡ��Ĵ���
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
#define COMMAND_READ_BACK 			0x3F	// ����ض��Ĵ�����ֻ��

// ���뷶Χ��VREF = 4.096V��
#define VREF_B_25								0x00	// ͨ�����뷶Χ��2.5*VREF
#define VREF_B_125							0x01	// ͨ�����뷶Χ��1.25*VREF
#define VREF_B_0625							0x02	// ͨ�����뷶Χ��0.625*VREF
#define VREF_U_25								0x05	// ͨ�����뷶Χ2.5*VREF
#define VREF_U_125							0x06	// ͨ�����뷶Χ1.25*VREF

//-----------------------------------------------------------------
// ��������
//-----------------------------------------------------------------
#define RST_L 	 		  HAL_GPIO_WritePin(ADCRST_GPIO_Port,ADCRST_Pin,  GPIO_PIN_RESET)
#define RST_H 	 		  HAL_GPIO_WritePin(ADCRST_GPIO_Port,ADCRST_Pin,   GPIO_PIN_SET)
#define DAISY_L 	 		HAL_GPIO_WritePin(ADCDAISY_GPIO_Port,ADCDAISY_Pin,  GPIO_PIN_RESET)
#define DAISY_H 	 		HAL_GPIO_WritePin(ADCDAISY_GPIO_Port,ADCDAISY_Pin,GPIO_PIN_SET)
#define CS_L 	 		  	HAL_GPIO_WritePin( ADCCS_GPIO_Port,ADCCS_Pin, GPIO_PIN_RESET)
#define CS_H 	 		  	HAL_GPIO_WritePin( ADCCS_GPIO_Port,ADCCS_Pin, GPIO_PIN_SET)

//-----------------------------------------------------------------
// �ⲿ��������
//-----------------------------------------------------------------
//extern void GPIO_ADS8688_Configuration(void);  //cubemx GPIO������
extern void ADS8688_Init(void);
extern void ADS8688_Write_Command(uint16_t com);
extern void ADS8688_Write_Program(uint8_t addr, uint8_t data);
extern uint8_t ADS8688_Read_Program(uint8_t addr);
extern void Get_AUTO_RST_Mode_ADC_Data(uint8_t ch_num, uint16_t *data);
#endif
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------

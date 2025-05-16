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
// ���Թ���: ����STM32���Ŀ����塢LZE_ST LINK2��DAS8688ģ��
// ˵    ��: 
//    
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include <stm32f10x.h>
#include "ads8688.h"
#include "delay.h"
#include "spi.h"	
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void GPIO_ADS8688_Configuration(void)
//-----------------------------------------------------------------
// 
// ��������: ADS8688�������ú���
// ��ڲ���: �� 
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void GPIO_ADS8688_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  // ʹ��IO��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//-----------------------------------------------------------------
// void ADS8688_Init(void)
//-----------------------------------------------------------------
// 
// ��������: ADS8361��ʼ��
// ��ڲ���: �� 
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void ADS8688_Init(void)
{
	GPIO_ADS8688_Configuration();
	SPI2_Init();
	SPI2_SetSpeed(SPI_BaudRatePrescaler_2);
	RST_L;
	Delay_1us(2);
	RST_H;
	DAISY_L;

	ADS8688_Write_Command(RST);	// �����λģʽ
	Delay_1us(2);
	ADS8688_Write_Program(CH0_INPUT_RANGE, VREF_B_125);	// ͨ��0�����뷶ΧΪ��5.12V
	ADS8688_Write_Program(CH1_INPUT_RANGE, VREF_B_125);	// ͨ��1�����뷶ΧΪ��5.12V
	ADS8688_Write_Program(CH2_INPUT_RANGE, VREF_B_125);	// ͨ��2�����뷶ΧΪ��5.12V
	ADS8688_Write_Program(CH3_INPUT_RANGE, VREF_B_125);	// ͨ��3�����뷶ΧΪ��5.12V
	ADS8688_Write_Program(CH4_INPUT_RANGE, VREF_B_125);	// ͨ��4�����뷶ΧΪ��5.12V
	ADS8688_Write_Program(CH5_INPUT_RANGE, VREF_B_125);	// ͨ��5�����뷶ΧΪ��5.12V
	ADS8688_Write_Program(CH6_INPUT_RANGE, VREF_B_125);	// ͨ��6�����뷶ΧΪ��5.12V
	ADS8688_Write_Program(CH7_INPUT_RANGE, VREF_B_125);	// ͨ��7�����뷶ΧΪ��5.12V
	ADS8688_Write_Program(CH_PWR_DN, 0x00);
	ADS8688_Write_Program(AUTO_SEQ_EN, 0xFF);
	ADS8688_Write_Command(AUTO_RST);
}

//-----------------------------------------------------------------
// void ADS8361_Write_Command(uint16_t com)
//-----------------------------------------------------------------
// 
// ��������: д����
// ��ڲ���: com������
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void ADS8688_Write_Command(uint16_t com)
{
	uint8_t wr_data[2] = {0x00, 0x00};
	
	wr_data[0] = (uint8_t)(com >> 8);
	wr_data[1] = (uint8_t)(com &0x00FF);
	
	CS_L;
	SPI2_ReadWriteByte(wr_data[0]);
	SPI2_ReadWriteByte(wr_data[1]);
	CS_H;
}

//-----------------------------------------------------------------
// void ADS8688_Write_Program(uint8_t addr, uint8_t data)
//-----------------------------------------------------------------
// 
// ��������: д����Ĵ���
// ��ڲ���: addr: �Ĵ�����ַ
//					 data������
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void ADS8688_Write_Program(uint8_t addr, uint8_t data)
{
	uint8_t wr_data[2] = {0x00, 0x00};
	
	wr_data[0] = (addr << 1) | 0x01;
	wr_data[1] = data;
	
	CS_L;
	SPI2_ReadWriteByte(wr_data[0]);
	SPI2_ReadWriteByte(wr_data[1]);
	CS_H;
}

//-----------------------------------------------------------------
// uint8_t ADS8361_Read_Program(uint8_t addr)
//-----------------------------------------------------------------
// 
// ��������: ������Ĵ���
// ��ڲ���: addr: �Ĵ�����ַ
// �� �� ֵ: ��ȡ������
// ע������: ��
//
//-----------------------------------------------------------------
uint8_t ADS8688_Read_Program(uint8_t addr)
{
	uint8_t Rxdata;
	uint8_t wr_data[2] = {0x00, 0x00};
	
	wr_data[0] = addr << 1;
	
	CS_L;
	SPI2_ReadWriteByte(wr_data[0]);
	SPI2_ReadWriteByte(wr_data[1]);
	Rxdata = SPI2_ReadWriteByte(0xFF);
	CS_H;
	
	return Rxdata;
}

//-----------------------------------------------------------------
// void Get_AUTO_RST_Mode_ADC_Data(uint8_t ch_num, uint16_t *data)
//-----------------------------------------------------------------
// 
// ��������: ��ȡAUTO_RSTģʽ�µ��������
// ��ڲ���: ch_num��ͨ����������AUTO_SEQ_EN���õĲ����йأ�
//					 *data����ȡ�õ�������
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void Get_AUTO_RST_Mode_ADC_Data(uint8_t ch_num, uint16_t *data)
{
	uint8_t Rxdata[2],i;
	uint8_t wr_data[2] = {0x00, 0x00};
	
	for(i=0; i<ch_num; i++) 
	{
		CS_L;
		SPI2_ReadWriteByte(wr_data[0]);
		SPI2_ReadWriteByte(wr_data[1]);
		Rxdata[0] = SPI2_ReadWriteByte(0x00);
		Rxdata[1] = SPI2_ReadWriteByte(0x00);
		CS_H;
		*(data+i) = ((uint16_t)Rxdata[0] << 8) | Rxdata[1];
	}
}

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
// ���Թ���: ����ԭ�Ӿ�Ӣ��STM32F103�����塢LZE_ST LINK2��2.8��Һ����DAS8688ģ��
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
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
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
	SPI1_Init();
	SPI1_SetSpeed(SPI_BaudRatePrescaler_2);
	ADS8688_RST = 0;
	delay_us(2);
	ADS8688_RST = 1;
	ADS8688_DAISY = 0;

	ADS8688_Write_Command(RST);	// �����λģʽ
	delay_us(2);
	ADS8688_Write_Program(CH0_INPUT_RANGE, VREF_B_125);	// ͨ��0�����뷶ΧΪ��5.12V
	ADS8688_Write_Program(CH1_INPUT_RANGE, VREF_B_125);	// ͨ��1�����뷶ΧΪ��5.12V
	ADS8688_Write_Program(CH2_INPUT_RANGE, VREF_B_125);	// ͨ��2�����뷶ΧΪ��5.12V
	ADS8688_Write_Program(CH3_INPUT_RANGE, VREF_B_125);	// ͨ��3�����뷶ΧΪ��5.12V
	ADS8688_Write_Program(CH4_INPUT_RANGE, VREF_B_125);	// ͨ��4�����뷶ΧΪ��5.12V
	ADS8688_Write_Program(CH5_INPUT_RANGE, VREF_B_125);	// ͨ��5�����뷶ΧΪ��5.12V
	ADS8688_Write_Program(CH6_INPUT_RANGE, VREF_B_125);	// ͨ��6�����뷶ΧΪ��5.12V
	ADS8688_Write_Program(CH7_INPUT_RANGE, VREF_B_125);	// ͨ��7�����뷶ΧΪ��5.12V
	ADS8688_Write_Program(CH_PWR_DN, 0x00);							// 8��ͨ���˳��͹���
	ADS8688_Write_Program(AUTO_SEQ_EN, 0xFF);						// 8��ͨ���Զ�ɨ������
	ADS8688_Write_Command(MAN_CH_0);
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
	
	ADS8688_CS = 0;
	SPI1_ReadWriteByte(wr_data[0]);
	SPI1_ReadWriteByte(wr_data[1]);
	ADS8688_CS = 1;
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
	
	ADS8688_CS = 0;
	SPI1_ReadWriteByte(wr_data[0]);
	SPI1_ReadWriteByte(wr_data[1]);
	ADS8688_CS = 1;
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
	
	ADS8688_CS = 0;
	SPI1_ReadWriteByte(wr_data[0]);
	SPI1_ReadWriteByte(wr_data[1]);
	Rxdata = SPI1_ReadWriteByte(0xFF);
	ADS8688_CS = 1;
	
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
		ADS8688_CS = 0;
		SPI1_ReadWriteByte(wr_data[0]);
		SPI1_ReadWriteByte(wr_data[1]);
		Rxdata[0] = SPI1_ReadWriteByte(0x00);
		Rxdata[1] = SPI1_ReadWriteByte(0x00);
		ADS8688_CS = 1;
		*(data+i) = ((uint16_t)Rxdata[0] << 8) | Rxdata[1];
	}
}

//-----------------------------------------------------------------
// void Get_MAN_CH_Data(uint16_t ch, uint16_t *data)
//-----------------------------------------------------------------
// 
// ��������: ��ȡAUTO_RSTģʽ�µ��������
// ��ڲ���: ch_num��ͨ����������AUTO_SEQ_EN���õĲ����йأ�
//					 *data����ȡ�õ�������
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void Get_MAN_CH_Data(uint16_t ch, uint16_t *data)
{
	uint8_t Rxdata[2];
	uint8_t wr_data[2] = {0x00, 0x00};

	ADS8688_Write_Command(ch);
	__NOP();
	__NOP();
	ADS8688_CS = 0;
	SPI1_ReadWriteByte(wr_data[0]);
	SPI1_ReadWriteByte(wr_data[1]);
	Rxdata[0] = SPI1_ReadWriteByte(0x00);
	Rxdata[1] = SPI1_ReadWriteByte(0x00);
	ADS8688_CS = 1;
	*data = ((uint16_t)Rxdata[0] << 8) | Rxdata[1];
}

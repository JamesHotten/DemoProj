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

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include "ads8688.h"
#include "spi.h"
//----------------------------------------------------------------
uint16_t count = 168;
//delay_us����
void delay_us(uint32_t us) {
	uint32_t i;
	for (i = 0; i < us; i++) {
		//168MHZ
		while (count--)
			;
	}
}
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
//void GPIO_ADS8688_Configuration(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//
//	__HAL_RCC_GPIOB_CLK_ENABLE();
//
//	GPIO_InitStructure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_12;
//	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
//	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStructure.Pull = GPIO_PULLUP;
//	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
//}

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
void ADS8688_Init(void) {

	//GPIO_ADS8688_Configuration();   //MX_GPIO_Init();
	//SPI2_Init();   //MX_SPI_Init();
	RST_L;
	delay_us(2);
	RST_H;
	DAISY_L;

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
void ADS8688_Write_Command(uint16_t com) {
	uint8_t wr_data[2] = { 0x00, 0x00 };

	wr_data[0] = (uint8_t) (com >> 8);
	wr_data[1] = (uint8_t) (com & 0x00FF);

	CS_L;
	HAL_SPI_Transmit(&hspi2, wr_data, 2, 0xFFFF);
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
void ADS8688_Write_Program(uint8_t addr, uint8_t data) {
	uint8_t wr_data[2] = { 0x00, 0x00 };

	wr_data[0] = (addr << 1) | 0x01;
	wr_data[1] = data;

	CS_L;
	HAL_SPI_Transmit(&hspi2, wr_data, 2, 0xFFFF);
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
uint8_t ADS8688_Read_Program(uint8_t addr) {
	uint8_t Rxdata;
	uint8_t wr_data[2] = { 0x00, 0x00 };

	wr_data[0] = addr << 1;
	HAL_SPI_Transmit(&hspi2, wr_data, 2, 0xFFFF);
	HAL_SPI_Receive(&hspi2, &Rxdata, 1, 0xFFFF);
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
void Get_AUTO_RST_Mode_ADC_Data(uint8_t ch_num, uint16_t *data) {
	uint8_t Rxdata[4], i;
	uint8_t wr_data[4] = { 0x00, 0x00, 0x00, 0x00 };

	for (i = 0; i < ch_num; i++) {
		CS_L;
		HAL_SPI_TransmitReceive(&hspi2, (uint8_t*) wr_data, (uint8_t*) Rxdata,
				4, 0xFFFF);
		CS_H;
		*(data + i) = ((uint16_t) Rxdata[2] << 8) | Rxdata[3];
	}
}

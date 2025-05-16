//-----------------------------------------------------------------
// ��������:
//     LED��ʾ��������
// ��    ��: ���ǵ���
// ��ʼ����: 2014-01-28
// �������: 2014-01-28
// �޸�����: 2014-02-16
// ��ǰ�汾: V1.0.1
// ��ʷ�汾:
//  - V1.0: (2014-02-07)LED IO ����
// - V1.0.1:(2014-02-16)ͷ�ļ��в���������ͷ�ļ�
// ���Թ���: ����ԭ�Ӿ�Ӣ��STM32F103�����塢LZE_ST LINK2
// ˵    ��:
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include <stm32f10x.h>

#include "spi.h"

//-----------------------------------------------------------------
// void SPI1_Init(void)
//-----------------------------------------------------------------
//
// ��������: SPI�ڳ�ʼ��
// ��ڲ���: ��
// �� �� ֵ: ��
// ȫ�ֱ���: ��
// ����ģ��: RCC_APB2PeriphClockCmd();GPIO_Init();
// ע������: ��
//
//-----------------------------------------------------------------
void SPI1_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1,  ENABLE );
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  // ����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;												// ����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;										// ����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;													// ����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;												// ����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;														// NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; // ���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ32
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;									// ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;														// CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  																// ����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	
	SPI_Cmd(SPI1, ENABLE); 		// ʹ��SPI����
	SPI1_ReadWriteByte(0xff);	// ��������		 
}   

void SPI1_SetSpeed(uint8_t SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI1->CR1&=0XFFC7;
	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(SPI1,ENABLE); 

} 

//-----------------------------------------------------------------
// uint8_t SPI1_ReadWriteByte(uint8_t TxData)
//-----------------------------------------------------------------
//
// ��������: SPI��дһ���ֽ�
// ��ڲ���: TxData�����͵��ֽ�
// �� �� ֵ: ��ȡ���ֽ�
// ȫ�ֱ���: ��
// ע������: ��
//
//-----------------------------------------------------------------
uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{		
	uint8_t retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) 	// ���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	{
		retry++;
		if(retry>200)
			return 0;
	}			  
	SPI_I2S_SendData(SPI1, TxData); 	// ͨ������SPIx����һ������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) // ���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
		retry++;
		if(retry>200)
			return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI1);	// ����ͨ��SPIx������յ�����					    
}

//-----------------------------------------------------------------
// uint8_t SPI1_WriteByte(uint8_t TxData)
//-----------------------------------------------------------------
//
// ��������: SPI�ڳ�ʼ��
// ��ڲ���: TxData�����͵��ֽ�
// �� �� ֵ: ����״̬��1������ʧ��  0�����ͳɹ�
// ȫ�ֱ���: ��
// ע������: ��
//
//-----------------------------------------------------------------
uint8_t SPI1_WriteByte(uint8_t TxData)
{		
	uint8_t retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) 	// ���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	{
		retry++;
		if(retry>200)
			return 1;
	}			  
	SPI_I2S_SendData(SPI1, TxData); // ͨ������SPIx����һ������   
	return 0;
}

//-----------------------------------------------------------------
// uint8_t SPI1_ReadByte(void)
//-----------------------------------------------------------------
//
// ��������: SPI��дһ������
// ��ڲ���: ��
// �� �� ֵ: ��ȡ���ֽ�
// ȫ�ֱ���: ��
// ע������: ��
//
//-----------------------------------------------------------------
uint8_t SPI1_ReadByte(void)
{		
	uint8_t retry=0;				 	

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) // ���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
		retry++;
		if(retry>200)
			return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI1); // ����ͨ��SPIx������յ�����					    
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------

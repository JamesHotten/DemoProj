//-----------------------------------------------------------------
// ��������:
//     SPI��������
// ��    ��: ���ǵ���
// ��ʼ����: 2020-09-15
// �������: 2020-09-15
// ��ǰ�汾: V1.0.0
// ��ʷ�汾:
// - V1.0.0: (2020-09-15)SPI����
// ���Թ���: ����STM32���Ŀ����塢LZE_ST_LINK2
// ˵    ��:
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include "spi.h"	

SPI_HandleTypeDef SPI_Handler;  // SPI���

//-----------------------------------------------------------------
// void SPI2_Init(void)
//-----------------------------------------------------------------
//
// ��������: SPI2��ʼ��
// ��ڲ���: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void SPI2_Init(void)
{
	SPI_Handler.Instance=SPI2;                         // SP2
	SPI_Handler.Init.Mode=SPI_MODE_MASTER;             // ����SPI����ģʽ������Ϊ��ģʽ
	SPI_Handler.Init.Direction=SPI_DIRECTION_2LINES;   // ����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ
	SPI_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       // ����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_Handler.Init.CLKPolarity=SPI_POLARITY_LOW;     // ����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
	SPI_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         // ����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_Handler.Init.NSS=SPI_NSS_SOFT;                 // NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_Handler.Init.NSSPMode=SPI_NSS_PULSE_DISABLE;	 // NSS�ź�����ʧ��
  SPI_Handler.Init.MasterKeepIOState=SPI_MASTER_KEEP_IO_STATE_ENABLE;  // SPI��ģʽIO״̬����ʹ��
	SPI_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_32;// ���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ32,����Ϊ6.25Mʱ��,����ģʽ
	SPI_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        // ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        // �ر�TIģʽ
	SPI_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;// �ر�Ӳ��CRCУ��
	SPI_Handler.Init.CRCPolynomial=7;                  // CRCֵ����Ķ���ʽ
	HAL_SPI_Init(&SPI_Handler);												 // ��ʼ��
	
	__HAL_SPI_ENABLE(&SPI_Handler);                    // ʹ��SPI1

	SPI2_ReadWriteByte(0Xff);                          // ��������
}   

//-----------------------------------------------------------------
// void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
//-----------------------------------------------------------------
// 
// ��������: SPI�ײ�������ʱ��ʹ�ܣ���������
// ��ڲ���: SPI_HandleTypeDef *hspi��SPI���
// �� �� ֵ: ��
// ע������: �˺����ᱻHAL_SPI_Init()����
//
//-----------------------------------------------------------------
void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(spiHandle->Instance==SPI2)
  {

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI2;
    PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      while(1);
    }
    __HAL_RCC_SPI2_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }	
}

//-----------------------------------------------------------------
// uint8_t SPI2_ReadWriteByte(uint8_t TxData)
//-----------------------------------------------------------------
// 
// ��������: SPI2��дһ���ֽ�
// ��ڲ���: uint8_t TxData��Ҫд����ֽ�
// �� �� ֵ: uint8_t Rxdata����ȡ�����ֽ�
// ע������: ��
//
//-----------------------------------------------------------------
uint8_t SPI2_ReadWriteByte(uint8_t TxData)
{
	uint8_t Rxdata;
	HAL_SPI_TransmitReceive(&SPI_Handler,&TxData,&Rxdata,1, 1000);       
 	return Rxdata;          		    // �����յ�������		
}

//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 

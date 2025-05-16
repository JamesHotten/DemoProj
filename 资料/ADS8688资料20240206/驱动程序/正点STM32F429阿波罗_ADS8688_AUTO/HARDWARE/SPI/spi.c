//-----------------------------------------------------------------
// ��������:
//     SPI��������
// ��    ��: ���ǵ���
// ��ʼ����: 2018-08-04
// �������: 2018-08-04
// �޸�����:
// ��ǰ�汾: V1.0
// ��ʷ�汾:
//  - V1.0: (2018-08-04)SPI����
// ���Թ���: ����ԭ��STM32F429�����ޡ�LZE_ST_LINK2
// ˵    ��:
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include "spi.h"

//-----------------------------------------------------------------
SPI_HandleTypeDef SPI2_Handler; // SPI���
//-----------------------------------------------------------------
// void SPI2_Init(void)
//-----------------------------------------------------------------
//
// ��������: SPI2�����������ó�����ģʽ
// ��ڲ���: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void SPI2_Init(void)
{
  SPI2_Handler.Instance               = SPI2;                     // SP5
  SPI2_Handler.Init.Mode              = SPI_MODE_MASTER;          // ����SPI����ģʽ������Ϊ��ģʽ
  SPI2_Handler.Init.Direction         = SPI_DIRECTION_2LINES;     // ����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ
  SPI2_Handler.Init.DataSize          = SPI_DATASIZE_8BIT;        // ����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
  SPI2_Handler.Init.CLKPolarity       = SPI_POLARITY_LOW;         // ����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
  SPI2_Handler.Init.CLKPhase          = SPI_PHASE_2EDGE;          // ����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
  SPI2_Handler.Init.NSS               = SPI_NSS_SOFT;             // NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
  SPI2_Handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32; // ���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ32
  SPI2_Handler.Init.FirstBit          = SPI_FIRSTBIT_MSB;         // ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
  SPI2_Handler.Init.CRCPolynomial     = 0;                        // CRCֵ����Ķ���ʽ
  HAL_SPI_Init(&SPI2_Handler);                                    // ��ʼ��
  __HAL_SPI_ENABLE(&SPI2_Handler);                                // ʹ��SPI2
}

//-----------------------------------------------------------------
// void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
//-----------------------------------------------------------------
//
// ��������: SPI2�ײ�������ʱ��ʹ�ܣ���������
// ��ڲ���: SPI_HandleTypeDef *hspi��SPI���
// �� �� ֵ: ��
// ע������: �˺����ᱻHAL_SPI_Init()����
//
//-----------------------------------------------------------------
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef GPIO_Initure;

  __HAL_RCC_GPIOB_CLK_ENABLE(); // ʹ��GPIOFʱ��
  __HAL_RCC_SPI2_CLK_ENABLE();  // ʹ��SPI2ʱ��

  GPIO_Initure.Pin       = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_Initure.Mode      = GPIO_MODE_AF_PP;   // �����������
  GPIO_Initure.Pull      = GPIO_PULLUP;       // ����
  GPIO_Initure.Speed     = GPIO_SPEED_HIGH;   // ����
  GPIO_Initure.Alternate = GPIO_AF5_SPI2; 		// ����ΪSPI2
  HAL_GPIO_Init(GPIOB, &GPIO_Initure);    		// ��ʼ��
}

//-----------------------------------------------------------------
// void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
//-----------------------------------------------------------------
//
// ��������: SPI�ٶ����ú���
// ��ڲ���: u8 SPI_BaudRatePrescaler��SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
// �� �� ֵ: ��
// ע������: SPI�ٶ�=fAPB1/��Ƶϵ����fAPB1ʱ��һ��Ϊ45Mhz
//
//-----------------------------------------------------------------
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler)); // �ж���Ч��
  __HAL_SPI_DISABLE(&SPI2_Handler);                               // �ر�SPI
  SPI2_Handler.Instance->CR1 &= 0XFFC7;                           // λ3-5���㣬�������ò�����
  SPI2_Handler.Instance->CR1 |= SPI_BaudRatePrescaler;            // ����SPI�ٶ�
  __HAL_SPI_ENABLE(&SPI2_Handler);                                // ʹ��SPI
}

//-----------------------------------------------------------------
// u8 SPI2_ReadWriteByte(u8 TxData)
//-----------------------------------------------------------------
//
// ��������: SPI2 ��дһ���ֽ�
// ��ڲ���: u8 TxData�� Ҫд����ֽ�
// �� �� ֵ: u8 Rxdata����ȡ�����ֽ�
// ע������: ��
//
//-----------------------------------------------------------------
u8 SPI2_ReadWrite_Byte(u8 dat)
{
  u8 Rxdata;
  HAL_SPI_TransmitReceive(&SPI2_Handler, &dat, &Rxdata, 1, 1000);
  return Rxdata; // �����յ�������
}

//-----------------------------------------------------------------
// void SPI2_Send_Byte(u8 dat)
//-----------------------------------------------------------------
//
// ��������: SPI2����1���ֽ�����
// ��ڲ���: u8 dat�� �����͵�����
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void SPI2_Send_Byte(u8 dat)
{
  u8 Rxdata;
  HAL_SPI_TransmitReceive(&SPI2_Handler, &dat, &Rxdata, 1, 1000);
}

//-----------------------------------------------------------------
// void SPI2_Send_Byte(u8 dat)
//-----------------------------------------------------------------
//
// ��������: SPI2��ȡ1���ֽ�����
// ��ڲ���: u8 dat�� �����͵�����
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
u8 SPI2_Read_Byte(void)
{
  u8 Txdata = 0xFF;
  u8 Rxdata;
  HAL_SPI_TransmitReceive(&SPI2_Handler, &Txdata, &Rxdata, 1, 1000);
  return Rxdata; // �����յ�������
}

//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 

#include "ADS8688.H" 
#include "sys.h"
#include "delay.h"
//#include "head_define.h" 
//#include "Algorithm.h" 
void ADS8688_SPI_WB(uint8_t com) 
{
	uint8_t com_temp=com,s;
	ADS_8688_nCS_L;
	for(s=0;s<8;s++)
	{
		if(com_temp&0x80)
		{
			ADS_8688_SDI_H; 
		}
		else 
		{ 
			ADS_8688_SDI_L;
		}
	ADS_8688_SCLK_H;
	com_temp<<=1; 
	ADS_8688_SCLK_L; 
	} 
}


uint8_t ADS8688_SPI_RB(void) 
{ 
	uint8_t Rdata=0,s; 
	ADS_8688_nCS_L;
	for(s=0;s<8;s++) 
	{ 
		Rdata<<=1;
		ADS_8688_SCLK_H;
		if(ADS_8688_SDO) 
		{ 
			Rdata|=0x01;
		}
		else 
		{ 
			Rdata&=0xFE;
		}
	ADS_8688_SCLK_L; 
	}
	return Rdata;
}


void ADS8688_WriteCommandReg(uint16_t command)//дADS8688����Ĵ��� 
{ 
	ADS_8688_nCS_L; 
	ADS8688_SPI_WB(command>>8 & 0XFF);
	ADS8688_SPI_WB(command & 0XFF);
	ADS_8688_nCS_H;
}


void ADS8688_Write_Program_Register(uint8_t Addr,uint8_t data)
{ 
	ADS_8688_nCS_L;
	ADS8688_SPI_WB(Addr<<1| 0X01);
	ADS8688_SPI_WB(data);
	ADS_8688_nCS_H; 
}


u8 ADS8688_READ_Program_Register(uint8_t Addr)
{ 
	u8 data = 0;
	ADS_8688_nCS_L;
	ADS8688_SPI_WB(Addr<<1); 
	data = ADS8688_SPI_RB();
	data = ADS8688_SPI_RB(); 
	ADS_8688_nCS_H; 
	return data;
}


 void Enter_RESET_MODE(void)//�����λģʽ����λ program registers
{
 ADS8688_WriteCommandReg(RST);
}

void AUTO_RST_Mode(void)//��??��ɨ��ģʽ 
{ 
	ADS8688_WriteCommandReg(AUTO_RST);
}

void MAN_Ch_n_Mode(uint16_t ch)//?��ģʽ 
{ 
	ADS8688_WriteCommandReg(ch);
}

void Set_CH_Range_Select(uint8_t ch,uint8_t range) //���ø���ͨ���ķ�Χ 
{ 
	ADS8688_Write_Program_Register(ch,range);
}

void Get_AUTO_RST_Mode_Data(uint16_t* outputdata, uint8_t chnum) 
{ 
	//��ȡɨ��ͨ�����е�ADת������code������������ 
	u8 i=0,datal=0,datah=0;
	u16 data=0; 
	for (i=0; i<chnum; i++) 
	{ 
		ADS_8688_nCS_L; ADS8688_SPI_WB(0X00); 
		ADS8688_SPI_WB(0X00); 
		datah = ADS8688_SPI_RB();;
		datal = ADS8688_SPI_RB();;
		ADS_8688_nCS_H;
		data = datah<<8 | datal; //��λ��ǰ����λ�ں� 
		*(outputdata+i) = data; 
	} 
}


uint16_t Get_MAN_Ch_n_Mode_Data(void) 
{ 
	u8 datah=0,datal=0;
	ADS_8688_nCS_L; 
	ADS8688_SPI_WB(0X00);
	ADS8688_SPI_WB(0X00);
	datah = ADS8688_SPI_RB();
	datal = ADS8688_SPI_RB();
	ADS_8688_nCS_H; 
	return (datah<<8 | datal);
}


void ADS8688_GPIO_Init(void)
{ 
	GPIO_InitTypeDef GPIO_Initure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	GPIO_Initure.GPIO_Pin=GPIO_Pin_7; 
	GPIO_Initure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_Initure.GPIO_OType=GPIO_OType_PP; //������� 
	GPIO_Initure.GPIO_PuPd=GPIO_PuPd_NOPULL; 
	GPIO_Initure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOG,&GPIO_Initure); 
	GPIO_Initure.GPIO_Pin=GPIO_Pin_2;
	GPIO_Init(GPIOG,&GPIO_Initure);
	GPIO_Initure.GPIO_Pin=GPIO_Pin_3; 
	GPIO_Init(GPIOG,&GPIO_Initure);
	GPIO_Initure.GPIO_Pin=GPIO_Pin_4;
	GPIO_Init(GPIOG,&GPIO_Initure);
	GPIO_Initure.GPIO_Pin=GPIO_Pin_5; 
	GPIO_Init(GPIOG,&GPIO_Initure);
	GPIO_Initure.GPIO_Pin=GPIO_Pin_6;
	GPIO_Initure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_Init(GPIOG,&GPIO_Initure);
}

//��ʼ��ͨ��1��Ĭ������10V��Χ 
void ADS8688_Init_Single() 
{ 
	ADS8688_GPIO_Init();
	ADS_8688_RST_PD_L;
	delay_us(2);
	ADS_8688_RST_PD_H;
	ADS_8688_DAISY_IN_L;
	Enter_RESET_MODE();
	ADS8688_Write_Program_Register(0X01,0XFF);
	ADS8688_READ_Program_Register(0X01);
	delay_us(2);
	ADS8688_Write_Program_Register(0x02,0x00);//����ͨ���˳��͹���״̬ 
	ADS8688_Write_Program_Register(0x01,0xff);//ʹ������ͨ�� 
	Set_CH_Range_Select(CH1,0x00);//����ͨ��1����?��Χ��+-2.5*Vref 
	// //0x00 -> +-2.5*ref 
	// //0x01 -> +-1.25*ref 
	// //0x02 -> +-0.625*ref
	// //0x03 -> +2.5*ref 
	// //0x04 -> +1.25*ref 
	MAN_Ch_n_Mode(MAN_Ch_1); 
	delay_ms(300);
 }

 
u16 get_ADS_ch1(void) 
{ 
	return Get_MAN_Ch_n_Mode_Data();//��ȡͨ��1����,����ͨ�����ɺ��� MAN_Ch_n_Mode()�������� 
}

double get_realdata(u16 x)
{
	double y;
	y=x-32768;
	if(y<0) y = y * 0.0001907+0.02;
	else y = y * 0.0001907+0.02;
	return y;
}


//��ʼ����ͨ����Ĭ��ȫ��������10V��Χ 
void ADS8688_Init_Mult(void)
{
	ADS8688_GPIO_Init();//���ų�ʼ��
	ADS_8688_RST_PD_H; 
	ADS_8688_DAISY_IN_L; 
	delay_us(5); 
	Enter_RESET_MODE();
	delay_us(5);
	//����IDLE
	ADS8688_Write_Program_Register(0X00,0x00);
	delay_us(5);
	//���ö�ͨ�� 0000 0011  �ٺ͢�ͨ��
	ADS8688_Write_Program_Register(0X01,0X03);
	delay_us(5);
	//�ص����õ�ͨ�� 1111 1100
	ADS8688_Write_Program_Register(0X02,0Xfc);
	delay_us(5);
	//��������
	ADS8688_Write_Program_Register(0X03,0X03);
	delay_us(5);
	Set_CH_Range_Select(CH1,0x00);//����ͨ��1�����뷶Χ��+-2.5*Vref 
	delay_us(5);
	Set_CH_Range_Select(CH2,0x00); 
	delay_us(5);
	//��������
	ADS8688_Write_Program_Register(0Xa0,0X03);
	delay_us(5);
	/*
	Set_CH_Range_Select(CH3,0x00); 
	Set_CH_Range_Select(CH4,0x00); 
	Set_CH_Range_Select(CH5,0x00); 
	Set_CH_Range_Select(CH6,0x00); 
	Set_CH_Range_Select(CH7,0x00); 
	Set_CH_Range_Select(CH8,0x00); // 0x00 -> +-2.5*ref 
	*/
	// //0x01 -> +-1.25*ref 
	// //0x02 -> +-0.625*ref 
	// //0x03 -> +2.5*ref 
	// //0x04 -> +1.25*ref 
	AUTO_RST_Mode();//�����Զ�ɨ��ģʽ 
}

static u16 ADS_Results[8]={0,};
void get_ADS_allch(float*result) 
{
	Get_AUTO_RST_Mode_Data(ADS_Results,8);//?��ɨ��ģʽ��?��ɨ�貢ת��8ͨ ����ת�����ݴ���Results������
}


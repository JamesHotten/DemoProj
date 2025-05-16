#include "ADS8688.h" 
//#include "head_define.h" 
//#include "Algorithm.h" 

u16 My_Ad[8]={0,0,};
float ad_real[2]={0.0,0.0};

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

void AUTO_RST_Mode(void)//�����Զ�ɨ��ģʽ 
{ 
	ADS8688_WriteCommandReg(AUTO_RST);
}

void MAN_Ch_n_Mode(uint16_t ch)//�Զ�ģʽ 
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
		datah = ADS8688_SPI_RB();
		datal = ADS8688_SPI_RB();
		ADS_8688_nCS_H;
		data = datah<<8 | datal; //��λ��ǰ����λ�ں� 
		*(outputdata+i) = data; 
	} 
}

//��ʼ��ͨ��1��Ĭ������10V��Χ 
void ADS8688_Init_Single() 
{ 
	ADS_8688_RST_PD_L;
	delay_us(2);
	ADS_8688_RST_PD_H;
	ADS_8688_DAISY_IN_L;
	Enter_RESET_MODE();
	ADS8688_Write_Program_Register(0X01,0Xff);
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
	//HAL_Delay(300);
 }

 void ADS8688_Init_Mult(void)
{
	ADS_8688_RST_PD_H; 
	ADS_8688_DAISY_IN_L; 
	delay_us(10);
	Enter_RESET_MODE();
	//��������
	ADS8688_Write_Program_Register(0X00,0X00);
	delay_us(10);
	//����ͨ��
	ADS8688_Write_Program_Register(0X01,0X07);
	delay_us(10);
	//����
	ADS8688_Write_Program_Register(0x02,0xf8);//����ͨ���˳��͹���״̬ 
	delay_us(10);
	//���ù���
	ADS8688_Write_Program_Register(0x03,0x03);
	delay_us(10);
	//ADS8688_Write_Program_Register(0x01,0xff);//ʹ������ͨ�� 
	Set_CH_Range_Select(CH1,0x00);//����ͨ��1�����뷶Χ��+-2.5*Vref 
	delay_us(10);
	
	Set_CH_Range_Select(CH2,0x00); 
	delay_us(10);
	
	Set_CH_Range_Select(CH3,0x00);
	delay_us(10);
	/*
	Set_CH_Range_Select(CH2,0x00);
	Set_CH_Range_Select(CH3,0x00);
	Set_CH_Range_Select(CH4,0x00); 
	Set_CH_Range_Select(CH5,0x00); 
	Set_CH_Range_Select(CH6,0x00); 
	Set_CH_Range_Select(CH7,0x00); 
	
	*/
	// //0x00 -> +-2. 5*ref 
	// //0x01 -> +-1.25*ref 
	// //0x02 -> +-0.625*ref 
	// //0x03 -> +2.5*ref 
	// //0x04 -> +1.25*ref 
	//AUTO_RST_Mode();//�����Զ�ɨ��ģʽ 
	ADS8688_Write_Program_Register(0xa0,0x03);
	delay_us(10);
  AUTO_RST_Mode();
	
}


void get_ADS_1and2(u16 *My_Ad,float *real_ad)
{
	Get_AUTO_RST_Mode_Data(My_Ad,2);
	gat_real_ad(real_ad);
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
u16 get_ADS_ch1(void) 
{ 
	return Get_MAN_Ch_n_Mode_Data();//��ȡͨ��1����,����ͨ�����ɺ��� MAN_Ch_n_Mode()�������� 
}



double get_realdata(u16 x,u8 t)//mv
{
	double y;
	
	if(!t)
	{
		y = (s16)(((double)(x-32762) *20.48/(double)65535)*1000);
	}
  else
	{
		y = (s16)(((double)(x-32725) *20.48/(double)65535)*1000);
	}
	return y;
}

//��ʼ����ͨ����Ĭ��ȫ��������10V��Χ 

void gat_real_ad(float *real_ad)
{
	real_ad[0]= (float)get_realdata(My_Ad[0],0);
	real_ad[1]= (float)get_realdata(My_Ad[1],1);
}

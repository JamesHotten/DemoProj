//-----------------------------------------------------------------
// 程序描述:
//     4.3寸/7寸RGB液晶显示屏驱动程序  
// 作    者: 凌智电子
// 开始日期: 2018-08-04
// 完成日期: 2018-08-04
// 修改日期: 
// 当前版本: V1.0
// 历史版本:
//	- V1.0：4.3寸/7寸 RGB液晶显示  
// 调试工具: 凌智STM32F429+CycloneIV电子系统设计开发板、LZE_ST_LINK2
// 说    明: 
//    
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "ltdc.h"
#include "lcd.h"
#include "delay.h"	 
//-----------------------------------------------------------------

#define LCD_SPI_CS(a)	\
						if (a)	\
						GPIOA->BSRR = GPIO_PIN_0;	\
						else		\
						GPIOA->BSRR  = (uint32_t)GPIO_PIN_0 << 16;
#define SPI_DCLK(a)	\
						if (a)	\
						GPIOC->BSRR = GPIO_PIN_2;	\
						else		\
						GPIOC->BSRR  = (uint32_t)GPIO_PIN_2 << 16;
#define SPI_SDA(a)	\
						if (a)	\
						GPIOA->BSRR = GPIO_PIN_1;	\
						else		\
						GPIOA->BSRR  = (uint32_t)GPIO_PIN_1 << 16;

LTDC_HandleTypeDef  LTDC_Handler;	    // LTDC句柄
DMA2D_HandleTypeDef DMA2D_Handler; 	  // DMA2D句柄

// 根据不同的颜色格式,定义帧缓存数组
#if LCD_PIXFORMAT==LCD_PIXFORMAT_ARGB8888||LCD_PIXFORMAT==LCD_PIXFORMAT_RGB888
	u32 ltdc_lcd_framebuf[1024][600] __attribute__((at(LCD_FRAME_BUF_ADDR)));	// 定义最大屏分辨率时,LCD所需的帧缓存数组大小
#else
	u16 ltdc_lcd_framebuf[1024][600] __attribute__((at(LCD_FRAME_BUF_ADDR)));	// 定义最大屏分辨率时,LCD所需的帧缓存数组大小
#endif

u32 *ltdc_framebuf[2];	// LTDC LCD帧缓存数组指针,必须指向对应大小的内存区域
_ltdc_dev lcdltdc;			// 管理LCD LTDC的重要参数

//-----------------------------------------------------------------
// void LTDC_Switch(u8 sw)
//-----------------------------------------------------------------
// 
// 函数功能: 打开LCD开关
// 入口参数: u8 sw：1 打开,0，关闭
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void LTDC_Switch(u8 sw)
{
	if(sw==1) 
		__HAL_LTDC_ENABLE(&LTDC_Handler);
	else if(sw==0)
		__HAL_LTDC_DISABLE(&LTDC_Handler);
}

//-----------------------------------------------------------------
// void LTDC_Layer_Switch(u8 layerx,u8 sw)
//-----------------------------------------------------------------
// 
// 函数功能: 开关指定层
// 入口参数: u8 layerx：层号,0,第一层; 1,第二层
//					 u8 sw：1 打开,0，关闭
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void LTDC_Layer_Switch(u8 layerx,u8 sw)
{
	if(sw==1) 
		__HAL_LTDC_LAYER_ENABLE(&LTDC_Handler,layerx);
	else if(sw==0) 
		__HAL_LTDC_LAYER_DISABLE(&LTDC_Handler,layerx);
	__HAL_LTDC_RELOAD_CONFIG(&LTDC_Handler);
}


//-----------------------------------------------------------------
// void LTDC_Select_Layer(u8 layerx)
//-----------------------------------------------------------------
// 
// 函数功能: 选择层
// 入口参数: u8 layerx：层号,0,第一层; 1,第二层
//					 u8 sw：1 打开,0，关闭
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void LTDC_Select_Layer(u8 layerx)
{
	lcdltdc.activelayer=layerx;
}

//-----------------------------------------------------------------
// void LTDC_Display_Dir(u8 dir)
//-----------------------------------------------------------------
// 
// 函数功能: 设置LCD显示方向
// 入口参数: u8 dir：dir:0,竖屏；1,横屏
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void LTDC_Display_Dir(u8 dir)
{
  lcdltdc.dir=dir;// 显示方向
	lcddev.dir=dir;	// 显示方向
	if(dir==DISPLAY_DIR_D || dir==DISPLAY_DIR_U)		
	{
		lcdltdc.width=lcdltdc.pheight;
		lcdltdc.height=lcdltdc.pwidth;	
		lcddev.width=lcdltdc.width;
		lcddev.height=lcdltdc.height;
	}
	else if(dir==DISPLAY_DIR_L || dir==DISPLAY_DIR_R)
	{
		lcdltdc.width=lcdltdc.pwidth;
		lcdltdc.height=lcdltdc.pheight;
		lcddev.width=lcdltdc.width;
		lcddev.height=lcdltdc.height;
	}
}

//-----------------------------------------------------------------
// void LTDC_Draw_Point(u16 x,u16 y,u32 color)
//-----------------------------------------------------------------
// 
// 函数功能: 画点函数
// 入口参数: u16 x：X坐标
//					 u16 y：Y坐标
//					 u32 color：颜色
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void LTDC_Draw_Point(u16 x,u16 y,u32 color)
{ 
#if LCD_PIXFORMAT==LCD_PIXFORMAT_ARGB8888||LCD_PIXFORMAT==LCD_PIXFORMAT_RGB888
	if(lcdltdc.dir==DISPLAY_DIR_R)			
	{
		if(lcddev.id == 0x7016)
			*(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-y-1)+x))=color;
		else if(lcddev.id == 0x8000)
			*(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pheight*(lcdltdc.pwidth-x-1)+y))=color; 
	}
	else if(lcdltdc.dir==DISPLAY_DIR_L)	
	{
		if(lcddev.id == 0x7016)
			*(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+(lcdltdc.pwidth-x-1)))=color;
		else if(lcddev.id == 0x8000)
			*(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pheight*x+(lcdltdc.pheight-y-1)))=color; 
	}
	else if(lcdltdc.dir==DISPLAY_DIR_U) 
	{
		if(lcddev.id == 0x7016)
			*(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-x-1)+(lcdltdc.pwidth-y-1)))=color; 
		else if(lcddev.id == 0x8000)
			*(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pheight*y+x))=color;
	}
	else																
	{
		if(lcddev.id == 0x7016)
			*(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*x+y))=color; 
		else if(lcddev.id == 0x8000)
			*(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pheight*(lcdltdc.pwidth-y-1)+(lcdltdc.pheight-x-1)))=color;
	}
#else
	if(lcdltdc.dir==DISPLAY_DIR_R)			
	{
		if(lcddev.id == 0x7016)
			*(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-y-1)+x))=color;
		else if(lcddev.id == 0x8000)
			*(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pheight*(lcdltdc.pwidth-x-1)+y))=color; 
	}
	else if(lcdltdc.dir==DISPLAY_DIR_L)	
	{
		if(lcddev.id == 0x7016)
			*(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+(lcdltdc.pwidth-x-1)))=color;
		else if(lcddev.id == 0x8000)
			*(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pheight*x+(lcdltdc.pheight-y-1)))=color; 
	}
	else if(lcdltdc.dir==DISPLAY_DIR_U) 
	{
		if(lcddev.id == 0x7016)
			*(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-x-1)+(lcdltdc.pwidth-y-1)))=color; 
		else if(lcddev.id == 0x8000)
			*(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pheight*y+x))=color;
	}
	else																
	{
		if(lcddev.id == 0x7016)
			*(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*x+y))=color; 
		else if(lcddev.id == 0x8000)
			*(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pheight*(lcdltdc.pwidth-y-1)+(lcdltdc.pheight-x-1)))=color;
	}
#endif
}

//-----------------------------------------------------------------
// u32 LTDC_Read_Point(u16 x,u16 y)
//-----------------------------------------------------------------
// 
// 函数功能: 读点函数
// 入口参数: u16 x：X坐标
//					 u16 y：Y坐标
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
u32 LTDC_Read_Point(u16 x,u16 y)
{ 
#if LCD_PIXFORMAT==LCD_PIXFORMAT_ARGB8888||LCD_PIXFORMAT==LCD_PIXFORMAT_RGB888
	if(lcdltdc.dir==DISPLAY_DIR_R)			
	{
		if(lcddev.id == 0x7016)
			return *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-y-1)+x));
		else if(lcddev.id == 0x8000)
			return *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pheight*(lcdltdc.pwidth-x-1)+y)); 
	}
	else if(lcdltdc.dir==DISPLAY_DIR_L)	
	{
		if(lcddev.id == 0x7016)
			return *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+(lcdltdc.pwidth-x-1)));
		else if(lcddev.id == 0x8000)
			return *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pheight*x+(lcdltdc.pheight-y-1))); 
	}
	else if(lcdltdc.dir==DISPLAY_DIR_U) 
	{
		if(lcddev.id == 0x7016)
			return *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-x-1)+(lcdltdc.pwidth-y-1))); 
		else if(lcddev.id == 0x8000)
			return *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pheight*y+x));
	}
	else																
	{
		if(lcddev.id == 0x7016)
			return *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*x+y)); 
		else if(lcddev.id == 0x8000)
			return *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pheight*(lcdltdc.pwidth-y-1)+(lcdltdc.pheight-x-1)));
	}
	return 0;
#else
	if(lcdltdc.dir==DISPLAY_DIR_R)			
	{
		if(lcddev.id == 0x7016)
			return *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-y-1)+x));
		else if(lcddev.id == 0x8000)
			return *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pheight*(lcdltdc.pwidth-x-1)+y)); 
	}
	else if(lcdltdc.dir==DISPLAY_DIR_L)	
	{
		if(lcddev.id == 0x7016)
			return *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+(lcdltdc.pwidth-x-1)));
		else if(lcddev.id == 0x8000)
			return *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pheight*x+(lcdltdc.pheight-y-1))); 
	}
	else if(lcdltdc.dir==DISPLAY_DIR_U) 
	{
		if(lcddev.id == 0x7016)
			return *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-x-1)+(lcdltdc.pwidth-y-1))); 
		else if(lcddev.id == 0x8000)
			return *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pheight*y+x));
	}
	else																
	{
		if(lcddev.id == 0x7016)
			return *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*x+y)); 
		else if(lcddev.id == 0x8000)
			return *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pheight*(lcdltdc.pwidth-y-1)+(lcdltdc.pheight-x-1)));
	}
	return 0;
#endif 
}

//-----------------------------------------------------------------
// void LTDC_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color)
//-----------------------------------------------------------------
// 
// 函数功能: LTDC填充矩形,DMA2D填充
// 入口参数: u16 sx：起始X坐标
//					 u16 sy：起始Y坐标
//					 u16 ex：终止X坐标
//					 u16 ey：终止Y坐标
//					 u32 color：颜色
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void LTDC_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color)
{ 
	u32 psx,psy,pex,pey;	// 以LCD面板为基准的坐标系,不随横竖屏变化而变化
	u32 timeout=0; 
	u16 offline;
	u32 addr; 
	// 坐标系转换
	if(lcdltdc.dir==DISPLAY_DIR_R)	
	{
		if(lcddev.id==0x8000)
		{
			psx=sy;
			psy=lcdltdc.pwidth-ex-1;
			pex=ey;
			pey=lcdltdc.pwidth-sx-1;
		}
		else if(lcddev.id==0x7016)
		{
			psx=sx;
			psy=lcdltdc.pheight-ey-1;
			pex=ex;
			pey=lcdltdc.pheight-sy-1;
		}
	}
	else if(lcdltdc.dir==DISPLAY_DIR_L)
	{
		if(lcddev.id==0x8000)
		{
			psx=lcdltdc.pheight-ey-1;
			psy=sx;
			pex=lcdltdc.pheight-sy-1;
			pey=ex;
		}
		else if(lcddev.id==0x7016)
		{
			psx=lcdltdc.pwidth-ex-1;
			psy=sy;
			pex=lcdltdc.pwidth-sx-1;
			pey=ey;
		}
	}
	else if(lcdltdc.dir==DISPLAY_DIR_U)			
	{
		if(lcddev.id==0x8000)
		{
			psx=sx;psy=sy;
			pex=ex;pey=ey;
		}
		else if(lcddev.id==0x7016)
		{
			psx=lcdltdc.pwidth-ey-1;
			psy=lcdltdc.pheight-ex-1;
			pex=lcdltdc.pwidth-sy-1;
			pey=lcdltdc.pheight-sx-1;
		}
	}
	else
	{
		if(lcddev.id==0x8000)
		{
			psx=lcdltdc.pheight-ex-1;
			psy=lcdltdc.pwidth-ey-1;
			pex=lcdltdc.pheight-sx-1;
			pey=lcdltdc.pwidth-sy-1;
		}
		else if(lcddev.id==0x7016)
		{
			psx=sy;
			psy=sx;
			pex=ey;
			pey=ex;
		}
	}
	if(lcddev.id==0x8000)
	{
		offline=lcdltdc.pheight-(pex-psx+1);
		addr=((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pheight*psy+psx));
	}
	else if(lcddev.id==0x7016)
	{
		offline=lcdltdc.pwidth-(pex-psx+1);
		addr=((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*psy+psx));
	}
	__HAL_RCC_DMA2D_CLK_ENABLE();	// 使能DM2D时钟
	DMA2D->CR&=~(DMA2D_CR_START);	// 先停止DMA2D
	DMA2D->CR=DMA2D_R2M;					// 寄存器到存储器模式
	DMA2D->OPFCCR=LCD_PIXFORMAT;	// 设置颜色格式
	DMA2D->OOR=offline;						// 设置行偏移 

	DMA2D->OMAR=addr;							// 输出存储器地址
	DMA2D->NLR=(pey-psy+1)|((pex-psx+1)<<16);	// 设定行数寄存器
	DMA2D->OCOLR=color;						// 设定输出颜色寄存器 
	DMA2D->CR|=DMA2D_CR_START;								// 启动DMA2D
	while((DMA2D->ISR&(DMA2D_FLAG_TC))==0)		// 等待传输完成
	{
		timeout++;
		if(timeout>0X1FFFFF)break;	// 超时退出
	} 
	DMA2D->IFCR|=DMA2D_FLAG_TC;		// 清除传输完成标志 		
}

//-----------------------------------------------------------------
// void LTDC_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
//-----------------------------------------------------------------
// 
// 函数功能: 在指定区域内填充指定颜色块,DMA2D填充	
// 入口参数: u16 sx：起始X坐标
//					 u16 sy：起始Y坐标
//					 u16 ex：终止X坐标
//					 u16 ey：终止Y坐标
//					 u32 color：颜色
// 返 回 值: 无
// 注意事项: sx,ex,不能大于lcddev.width-1;sy,ey,不能大于lcddev.height-1!!!
//color:要填充的颜色数组首地址
//
//-----------------------------------------------------------------
void LTDC_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{
	u16 height,width;
	u16 i,j;
	
	width=ex-sx+1; 			// 得到填充的宽度
	height=ey-sy+1;			// 高度
	for(i=0;i<height;i++)
			for(j=0;j<width;j++)
				LTDC_Draw_Point(sx+j,sy+i,color[i*width+j]);// 写入数据 
}  


//-----------------------------------------------------------------
// void LTDC_Clear(u32 color)
//-----------------------------------------------------------------
// 
// 函数功能: LCD清屏
// 入口参数: u32 color：颜色
// 返 回 值: 无
// 全局变量: lcdltdc.width：LCD宽度
//					 lcdltdc.height：LCD高度
// 注意事项: 无
//
//-----------------------------------------------------------------
void LTDC_Clear(u32 color)
{
	LTDC_Fill(0,0,lcdltdc.width-1,lcdltdc.height-1,color);	
}

//-----------------------------------------------------------------
// u8 LTDC_Clk_Set(u32 pllsain,u32 pllsair,u32 pllsaidivr)
//-----------------------------------------------------------------
// 
// 函数功能: LTDC时钟(Fdclk)设置函数
// 入口参数: u32 pllsain：SAI时钟倍频系数N,取值范围:50~432. 
//					 u32 pllsair：SAI时钟的分频系数R,取值范围:2~7
//					 u32 pllsaidivr：LCD时钟分频系数,取值范围:RCC_PLLSAIDIVR_2/4/8/16,对应分频2~16 
// 返 回 值: 0,成功;1,失败。
// 注意事项: 
//					 LTDC时钟(Fdclk)设置函数
//					 Fvco=Fin*pllsain; 
//					 Fdclk=Fvco/pllsair/2*2^pllsaidivr=Fin*pllsain/pllsair/2*2^pllsaidivr;
//					 Fvco:VCO频率
//					 Fin:输入时钟频率一般为1Mhz(来自系统时钟PLLM分频后的时钟,见时钟树图)
//					 假设:外部晶振为25M,pllm=25的时候,Fin=1Mhz.
//					 例如:要得到20M的LTDC时钟,则可以设置:pllsain=400,pllsair=5,pllsaidivr=RCC_PLLSAIDIVR_4
//					 Fdclk=1*400/5/4=400/20=20Mhz
//
//-----------------------------------------------------------------
u8 LTDC_Clk_Set(u32 pllsain,u32 pllsair,u32 pllsaidivr)
{
	RCC_PeriphCLKInitTypeDef PeriphClkIniture;
	
	// LTDC输出像素时钟，需要根据自己所使用的LCD数据手册来配置！
  PeriphClkIniture.PeriphClockSelection=RCC_PERIPHCLK_LTDC;	// LTDC时钟 	
	PeriphClkIniture.PLLSAI.PLLSAIN=pllsain;    
	PeriphClkIniture.PLLSAI.PLLSAIR=pllsair;  
	PeriphClkIniture.PLLSAIDivR=pllsaidivr;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkIniture)==HAL_OK)  // 配置像素时钟
  {
		return 0;   // 成功
  }
  else 
		return 1;   // 失败    
}

//-----------------------------------------------------------------
// void LTDC_Layer_Window_Config(u8 layerx,u16 sx,u16 sy,u16 width,u16 height)
//-----------------------------------------------------------------
// 
// 函数功能: LTDC层颜窗口设置,窗口以LCD面板坐标系为基准
// 入口参数: u8 layerx：层值,0/1.
//					 u16 sx：起始X坐标
//					 u16 sy：起始Y坐标
//					 u16 width：窗口宽度
//					 u16 height：窗口高度
// 返 回 值: 0,成功;1,失败。
// 注意事项: 此函数必须在LTDC_Layer_Parameter_Config之后再设置.
//
//-----------------------------------------------------------------
void LTDC_Layer_Window_Config(u8 layerx,u16 sx,u16 sy,u16 width,u16 height)
{
	HAL_LTDC_SetWindowPosition(&LTDC_Handler,sx,sy,layerx);  	// 设置窗口的位置
	HAL_LTDC_SetWindowSize(&LTDC_Handler,width,height,layerx);// 设置窗口大小   
}

//-----------------------------------------------------------------
// void LTDC_Layer_Parameter_Config(u8 layerx,u32 bufaddr,u8 pixformat,u8 alpha,u8 alpha0,u8 bfac1,u8 bfac2,u32 bkcolor)
//-----------------------------------------------------------------
// 
// 函数功能: LTDC,基本参数设置.
// 入口参数: u8 layerx：层值,0/1.
//					 u32 bufaddr：层颜色帧缓存起始地址
//					 u8 pixformat：颜色格式.0,ARGB8888;1,RGB888;2,RGB565;3,ARGB1555;4,ARGB4444;5,L8;6;AL44;7;AL88
//					 u8 alpha：层颜色Alpha值,0,全透明;255,不透明
//					 u8 alpha0：默认颜色Alpha值,0,全透明;255,不透明
//					 u8 bfac1：混合系数1,4(100),恒定的Alpha;6(101),像素Alpha*恒定Alpha
//					 u8 bfac2：混合系数2,5(101),恒定的Alpha;7(111),像素Alpha*恒定Alpha
//					 u32 bkcolor：层默认颜色,32位,低24位有效,RGB888格式
// 返 回 值: 无
// 注意事项: 此函数,必须在LTDC_Layer_Window_Config之前设置.
//
//-----------------------------------------------------------------
void LTDC_Layer_Parameter_Config(u8 layerx,u32 bufaddr,u8 pixformat,u8 alpha,u8 alpha0,u8 bfac1,u8 bfac2,u32 bkcolor)
{
	LTDC_LayerCfgTypeDef pLayerCfg;
	
	pLayerCfg.WindowX0=0;                       // 窗口起始X坐标
	pLayerCfg.WindowY0=0;                       // 窗口起始Y坐标
	pLayerCfg.WindowX1=lcdltdc.pwidth;          // 窗口终止X坐标
	pLayerCfg.WindowY1=lcdltdc.pheight;         // 窗口终止Y坐标
	pLayerCfg.PixelFormat=pixformat;		    		// 像素格式
	pLayerCfg.Alpha=alpha;				        			// Alpha值设置，0~255,255为完全不透明
	pLayerCfg.Alpha0=alpha0;			        			// 默认Alpha值
	pLayerCfg.BlendingFactor1=(u32)bfac1<<8;    // 设置层混合系数
	pLayerCfg.BlendingFactor2=(u32)bfac2<<8;		// 设置层混合系数
	pLayerCfg.FBStartAdress=bufaddr;	        	// 设置层颜色帧缓存起始地址
	pLayerCfg.ImageWidth=lcdltdc.pwidth;        // 设置颜色帧缓冲区的宽度    
	pLayerCfg.ImageHeight=lcdltdc.pheight;      // 设置颜色帧缓冲区的高度
	pLayerCfg.Backcolor.Red=(u8)(bkcolor&0X00FF0000)>>16;   // 背景颜色红色部分
	pLayerCfg.Backcolor.Green=(u8)(bkcolor&0X0000FF00)>>8;  // 背景颜色绿色部分
	pLayerCfg.Backcolor.Blue=(u8)bkcolor&0X000000FF;        // 背景颜色蓝色部分
	HAL_LTDC_ConfigLayer(&LTDC_Handler,&pLayerCfg,layerx);  // 设置所选中的层
}  


//-----------------------------------------------------------------
// void LTDC_Init(void)
//-----------------------------------------------------------------
// 
// 函数功能: LCD初始化函数
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void LTDC_Init(void)
{   
	u16 lcdid=0;

	lcdid=lcddev.id;						// 读取LCD面板ID	
	if(lcdid==0X8000)
	{
		Lcd_Initialize();					// 4.3寸RGB配置
		lcdltdc.pwidth=800;			  // 面板宽度,单位:像素
		lcdltdc.pheight=480;		  // 面板高度,单位:像素
		lcdltdc.hsw=1;				  	// 水平同步宽度
		lcdltdc.vsw=1;				  	// 垂直同步宽度
		lcdltdc.hbp=10;				  	// 水平后廊 0.5us
		lcdltdc.vbp=10;				  	// 垂直后廊
		lcdltdc.hfp=10;			 			// 水平前廊 0.5us
		lcdltdc.vfp=43;				    // 垂直前廊
    LTDC_Clk_Set(360,3,RCC_PLLSAIDIVR_4); // 设置像素时钟 33M(如果开双显,需要降低DCLK到:18.75Mhz  300/4/4,才会比较好)
	}
	else if(lcdid==0X7016)		
	{
		lcdltdc.pwidth=1024;			// 面板宽度,单位:像素
		lcdltdc.pheight=600;			// 面板高度,单位:像素
    lcdltdc.hsw=20;						// 水平同步宽度
		lcdltdc.vsw=3;				    // 垂直同步宽度
		lcdltdc.hbp=140;			    // 水平后廊
		lcdltdc.vbp=20;				    // 垂直后廊
		lcdltdc.hfp=160;			    // 水平前廊
		lcdltdc.vfp=12;				    // 垂直前廊
		LTDC_Clk_Set(360,2,RCC_PLLSAIDIVR_4);
		// 其他参数待定.
	}

	lcddev.width=lcdltdc.pwidth;
	lcddev.height=lcdltdc.pheight;
    
#if LCD_PIXFORMAT==LCD_PIXFORMAT_ARGB8888||LCD_PIXFORMAT==LCD_PIXFORMAT_RGB888 
	ltdc_framebuf[0]=(u32*)&ltdc_lcd_framebuf;
	lcdltdc.pixsize=4;				  // 每个像素占4个字节
#else 
    lcdltdc.pixsize=2;				// 每个像素占2个字节
	ltdc_framebuf[0]=(u32*)&ltdc_lcd_framebuf;
#endif 	
	if(lcdid==0X8000)
	{
		// LTDC配置
		LTDC_Handler.Instance=LTDC;
		LTDC_Handler.Init.HSPolarity=LTDC_HSPOLARITY_AL;         // 水平同步极性
		LTDC_Handler.Init.VSPolarity=LTDC_VSPOLARITY_AL;         // 垂直同步极性
		LTDC_Handler.Init.DEPolarity=LTDC_DEPOLARITY_AL;         // 数据使能极性
		LTDC_Handler.Init.PCPolarity=LTDC_PCPOLARITY_IIPC;       // 像素时钟极性
		LTDC_Handler.Init.HorizontalSync=lcdltdc.hsw-1;          // 水平同步宽度
		LTDC_Handler.Init.VerticalSync=lcdltdc.vsw-1;            // 垂直同步宽度
		LTDC_Handler.Init.AccumulatedHBP=lcdltdc.hsw+lcdltdc.hbp-1; // 水平同步后沿宽度
		LTDC_Handler.Init.AccumulatedVBP=lcdltdc.vsw+lcdltdc.vbp-1; // 垂直同步后沿高度
		LTDC_Handler.Init.AccumulatedActiveW=lcdltdc.hsw+lcdltdc.hbp+lcdltdc.pheight-1;// 有效宽度
		LTDC_Handler.Init.AccumulatedActiveH=lcdltdc.vsw+lcdltdc.vbp+lcdltdc.pwidth-1; // 有效高度
		LTDC_Handler.Init.TotalWidth=lcdltdc.hsw+lcdltdc.hbp+lcdltdc.pheight+lcdltdc.hfp-1;   // 总宽度
		LTDC_Handler.Init.TotalHeigh=lcdltdc.vsw+lcdltdc.vbp+lcdltdc.pwidth+lcdltdc.vfp-1;  	// 总高度
		LTDC_Handler.Init.Backcolor.Red=0;           // 屏幕背景层红色部分
		LTDC_Handler.Init.Backcolor.Green=0;         // 屏幕背景层绿色部分
		LTDC_Handler.Init.Backcolor.Blue=0;          // 屏幕背景色蓝色部分
		HAL_LTDC_Init(&LTDC_Handler);
		
		// 层配置
		LTDC_Layer_Parameter_Config(0,(u32)ltdc_framebuf[0],LCD_PIXFORMAT,255,0,6,7,0X000000);// 层参数配置
		LTDC_Layer_Window_Config(0,0,0,lcdltdc.pheight,lcdltdc.pwidth);	// 层窗口配置,以LCD面板坐标系为基准,不要随便修改!
	}
	else if(lcdid==0X7016)	
	{
		// LTDC配置
		LTDC_Handler.Instance=LTDC;
		LTDC_Handler.Init.HSPolarity=LTDC_HSPOLARITY_AL;         // 水平同步极性
		LTDC_Handler.Init.VSPolarity=LTDC_VSPOLARITY_AL;         // 垂直同步极性
		LTDC_Handler.Init.DEPolarity=LTDC_DEPOLARITY_AL;         // 数据使能极性
		LTDC_Handler.Init.PCPolarity=LTDC_PCPOLARITY_IPC;        // 像素时钟极性
		LTDC_Handler.Init.HorizontalSync=lcdltdc.hsw-1;          // 水平同步宽度
		LTDC_Handler.Init.VerticalSync=lcdltdc.vsw-1;            // 垂直同步宽度
		LTDC_Handler.Init.AccumulatedHBP=lcdltdc.hsw+lcdltdc.hbp-1; // 水平同步后沿宽度
		LTDC_Handler.Init.AccumulatedVBP=lcdltdc.vsw+lcdltdc.vbp-1; // 垂直同步后沿高度
		LTDC_Handler.Init.AccumulatedActiveW=lcdltdc.hsw+lcdltdc.hbp+lcdltdc.pwidth-1; // 有效宽度
		LTDC_Handler.Init.AccumulatedActiveH=lcdltdc.vsw+lcdltdc.vbp+lcdltdc.pheight-1;// 有效高度
		LTDC_Handler.Init.TotalWidth=lcdltdc.hsw+lcdltdc.hbp+lcdltdc.pwidth+lcdltdc.hfp-1;   // 总宽度
		LTDC_Handler.Init.TotalHeigh=lcdltdc.vsw+lcdltdc.vbp+lcdltdc.pheight+lcdltdc.vfp-1;  // 总高度
		LTDC_Handler.Init.Backcolor.Red=0;           // 屏幕背景层红色部分
		LTDC_Handler.Init.Backcolor.Green=0;         // 屏幕背景层绿色部分
		LTDC_Handler.Init.Backcolor.Blue=0;          // 屏幕背景色蓝色部分
		HAL_LTDC_Init(&LTDC_Handler);
		
		// 层配置
		LTDC_Layer_Parameter_Config(0,(u32)ltdc_framebuf[0],LCD_PIXFORMAT,255,0,6,7,0X000000);// 层参数配置
		LTDC_Layer_Window_Config(0,0,0,lcdltdc.pwidth,lcdltdc.pheight);	// 层窗口配置,以LCD面板坐标系为基准,不要随便修改!
	}
	LTDC_Display_Dir( LCD_DIR );// 设置显示方向
	LTDC_Select_Layer(0); 			// 选择第1层
	LTCD_LED=1;         		    // 点亮背光
	LTDC_Clear(0XFFFFFFFF);			// 清屏
}

//-----------------------------------------------------------------
// void LTDC_Init(void)
//-----------------------------------------------------------------
// 
// 函数功能: LTDC底层IO初始化和时钟使能
// 入口参数: LTDC_HandleTypeDef* hltdc：hltdc:LTDC句柄
// 返 回 值: 无
// 注意事项: 此函数会被HAL_LTDC_Init()调用
//					 LTDC_R3 -> PH9					
//					 LTDC_R4 -> PH10				RGB_BL 		 -> PC6
//					 LTDC_R5 -> PH11				LTDC_DE 	 -> PF10
//					 LTDC_R6 -> PH12				LTDC_VSYNC -> PI9
//					 LTDC_R7 -> PG6					LTDC_HSYNC -> PI10
//					 LTDC_G2 -> PH13				LTDC_CLK   -> PG7
//					 LTDC_G3 -> PH14
//					 LTDC_G4 -> PH15
//					 LTDC_G5 -> PI0
//					 LTDC_G6 -> PC7
//					 LTDC_G7 -> PI2
//					 LTDC_B3 -> PG11
//					 LTDC_B4 -> PI4
//					 LTDC_B5 -> PI5
//					 LTDC_B6 -> PI6
//					 LTDC_B7 -> PI7
//
//-----------------------------------------------------------------
void HAL_LTDC_MspInit(LTDC_HandleTypeDef* hltdc)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_LTDC_CLK_ENABLE();                // 使能LTDC时钟
    __HAL_RCC_DMA2D_CLK_ENABLE();               // 使能DMA2D时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();               // 使能GPIOB时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();               // 使能GPIOC时钟
    __HAL_RCC_GPIOF_CLK_ENABLE();               // 使能GPIOF时钟
    __HAL_RCC_GPIOG_CLK_ENABLE();               // 使能GPIOG时钟
    __HAL_RCC_GPIOH_CLK_ENABLE();               // 使能GPIOH时钟
    __HAL_RCC_GPIOI_CLK_ENABLE();               // 使能GPIOI时钟
    
    // 初始化PC6，背光引脚
    GPIO_Initure.Pin=GPIO_PIN_6;                // PC6推挽输出，控制背光
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;      // 推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;              // 上拉        
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;         // 高速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);					// 初始化
    
    // 初始化PF10
    GPIO_Initure.Pin=GPIO_PIN_10; 							// 初始化PF10
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          // 复用
    GPIO_Initure.Pull=GPIO_NOPULL;              // 无上拉下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;         // 高速
    GPIO_Initure.Alternate=GPIO_AF14_LTDC;      // 复用为LTDC
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);					// 初始化
    
    // 初始化PG6,7,11
    GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_11;
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);
    
    // 初始化PH9,10,11,12,13，14，15
    GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|\
                     GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
    
    // 初始化PI0,2,4,5,6,7,9,10
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|\
                     GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_10;
    HAL_GPIO_Init(GPIOI,&GPIO_Initure); 
    // 初始化PC7,
    GPIO_Initure.Pin=GPIO_PIN_7;                
    HAL_GPIO_Init(GPIOC,&GPIO_Initure); 
}

//-----------------------------------------------------------------
// static void LCD_GPIO_Config(void)
//-----------------------------------------------------------------
//
// 函数功能: 4.3寸RGB部分引脚分配,SPI配置
// 入口参数: 无
// 返回参数: 无
// 注意事项: 无
//					 
//-----------------------------------------------------------------
static void LCD_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_Initure;

	__HAL_RCC_GPIOA_CLK_ENABLE();			// 使能GPIOA时钟
	__HAL_RCC_GPIOC_CLK_ENABLE();			// 使能GPIOC时钟

	GPIO_Initure.Pin=GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2; // PA0 -> T_MISO, PA1 - > T_MOSI , PA2 -> T_CS
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  // 推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;          // 上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     // 高速
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);

	GPIO_Initure.Pin=GPIO_PIN_2 | GPIO_PIN_6; // PC2 -> T_CLK, PC6 -> RGB_BL
	HAL_GPIO_Init(GPIOC,&GPIO_Initure);

	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);	
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);	 
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET);	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);	
}

//-----------------------------------------------------------------
// void LCD_WriteByteSPI(unsigned char byte)
//-----------------------------------------------------------------
//
// 函数功能: SPI写数据
// 入口参数: unsigned char byte：写入的数据
// 返回参数: 无
// 注意事项: 无
//					 
//-----------------------------------------------------------------
void LCD_WriteByteSPI(unsigned char byte)
{
	int i;
	for(i=7;i>=0;i--)
	{        
		SPI_DCLK(0);
		if(byte & (1<<i))
			SPI_SDA(1)
		else
			SPI_SDA(0)
		delay_us(1);
		SPI_DCLK(1);
		delay_us(1);
	}
}
//-----------------------------------------------------------------
// void SPI_WriteComm(u16 CMD)
//-----------------------------------------------------------------
//
// 函数功能: SPI写命令
// 入口参数: uu16 CMD：命令
// 返回参数: 无
// 注意事项: 无
//					 
//-----------------------------------------------------------------
void SPI_WriteComm(u16 CMD)
{			
	delay_us(10);	
	LCD_SPI_CS(0);
	delay_us(1);	
	LCD_WriteByteSPI(0X20);
	delay_us(1);	
	LCD_WriteByteSPI(CMD>>8);
	delay_us(1);	
	LCD_SPI_CS(1);
	
	delay_us(10);	
	LCD_SPI_CS(0);
	delay_us(1);	
	LCD_WriteByteSPI(0X00);
	delay_us(1);	
	LCD_WriteByteSPI(CMD);
	delay_us(1);	
	LCD_SPI_CS(1);
	delay_us(1);	
}
//-----------------------------------------------------------------
// void SPI_WriteData(u16 tem_data)
//-----------------------------------------------------------------
//
// 函数功能: SPI写数据
// 入口参数: u16 tem_data：写入的数据
// 返回参数: 无
// 注意事项: 无
//					 
//-----------------------------------------------------------------
void SPI_WriteData(u16 tem_data)
{			
	delay_us(10);	
	LCD_SPI_CS(0);
	delay_us(1);	
	LCD_WriteByteSPI(0x40);
	delay_us(1);	
	LCD_WriteByteSPI(tem_data);
	delay_us(1);	
	LCD_SPI_CS(1);
	delay_us(1);	
}

//-----------------------------------------------------------------
// void Lcd_Initialize(void)
//-----------------------------------------------------------------
//
// 函数功能: RGB配置
// 入口参数: 无
// 返回参数: 无
// 注意事项: 无
//					 
//-----------------------------------------------------------------
void Lcd_Initialize(void)
{	
	LCD_GPIO_Config();
	delay_ms(20);	
	SPI_WriteComm(0x0000);		// NOP 
	SPI_WriteComm(0x0100);		// Software Reset 
	delay_ms(200);						// 200ms
	
	SPI_WriteComm(0xf000);SPI_WriteData(0x0055);	
	SPI_WriteComm(0xf001);SPI_WriteData(0x00aa);	
	SPI_WriteComm(0xf002);SPI_WriteData(0x0052);	
	SPI_WriteComm(0xf003);SPI_WriteData(0x0008);	
	SPI_WriteComm(0xf004);SPI_WriteData(0x0001);	
																						 
	SPI_WriteComm(0xbc01);SPI_WriteData(0x0086);	
	SPI_WriteComm(0xbc02);SPI_WriteData(0x006a);	
	SPI_WriteComm(0xbd01);SPI_WriteData(0x0086);	
	SPI_WriteComm(0xbd02);SPI_WriteData(0x006a);	
	SPI_WriteComm(0xbe01);SPI_WriteData(0x0067);	
																						 
	SPI_WriteComm(0xd100);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd101);SPI_WriteData(0x005d);	
	SPI_WriteComm(0xd102);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd103);SPI_WriteData(0x006b);	
	SPI_WriteComm(0xd104);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd105);SPI_WriteData(0x0084);	
	SPI_WriteComm(0xd106);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd107);SPI_WriteData(0x009c);	
	SPI_WriteComm(0xd108);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd109);SPI_WriteData(0x00b1);	
	SPI_WriteComm(0xd10a);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd10b);SPI_WriteData(0x00d9);	
	SPI_WriteComm(0xd10c);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd10d);SPI_WriteData(0x00fd);	
	SPI_WriteComm(0xd10e);SPI_WriteData(0x0001);	
	SPI_WriteComm(0xd10f);SPI_WriteData(0x0038);	
	SPI_WriteComm(0xd110);SPI_WriteData(0x0001);	
	SPI_WriteComm(0xd111);SPI_WriteData(0x0068);	
	SPI_WriteComm(0xd112);SPI_WriteData(0x0001);	
	SPI_WriteComm(0xd113);SPI_WriteData(0x00b9);	
	SPI_WriteComm(0xd114);SPI_WriteData(0x0001);	
	SPI_WriteComm(0xd115);SPI_WriteData(0x00fb);	
	SPI_WriteComm(0xd116);SPI_WriteData(0x0002);	
	SPI_WriteComm(0xd117);SPI_WriteData(0x0063);	
	SPI_WriteComm(0xd118);SPI_WriteData(0x0002);	
	SPI_WriteComm(0xd119);SPI_WriteData(0x00b9);	
	SPI_WriteComm(0xd11a);SPI_WriteData(0x0002);	
	SPI_WriteComm(0xd11b);SPI_WriteData(0x00bb);	
	SPI_WriteComm(0xd11c);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd11d);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd11e);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd11f);SPI_WriteData(0x0046);	
	SPI_WriteComm(0xd120);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd121);SPI_WriteData(0x0069);	
	SPI_WriteComm(0xd122);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd123);SPI_WriteData(0x008f);	
	SPI_WriteComm(0xd124);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd125);SPI_WriteData(0x00a4);	
	SPI_WriteComm(0xd126);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd127);SPI_WriteData(0x00b9);	
	SPI_WriteComm(0xd128);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd129);SPI_WriteData(0x00c7);	
	SPI_WriteComm(0xd12a);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd12b);SPI_WriteData(0x00c9);	
	SPI_WriteComm(0xd12c);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd12d);SPI_WriteData(0x00cb);	
	SPI_WriteComm(0xd12e);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd12f);SPI_WriteData(0x00cb);	
	SPI_WriteComm(0xd130);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd131);SPI_WriteData(0x00cb);	
	SPI_WriteComm(0xd132);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd133);SPI_WriteData(0x00cc);	
																						 
	SPI_WriteComm(0xd200);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd201);SPI_WriteData(0x005d);	
	SPI_WriteComm(0xd202);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd203);SPI_WriteData(0x006b);	
	SPI_WriteComm(0xd204);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd205);SPI_WriteData(0x0084);	
	SPI_WriteComm(0xd206);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd207);SPI_WriteData(0x009c);	
	SPI_WriteComm(0xd208);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd209);SPI_WriteData(0x00b1);	
	SPI_WriteComm(0xd20a);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd20b);SPI_WriteData(0x00d9);	
	SPI_WriteComm(0xd20c);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd20d);SPI_WriteData(0x00fd);	
	SPI_WriteComm(0xd20e);SPI_WriteData(0x0001);	
	SPI_WriteComm(0xd20f);SPI_WriteData(0x0038);	
	SPI_WriteComm(0xd210);SPI_WriteData(0x0001);	
	SPI_WriteComm(0xd211);SPI_WriteData(0x0068);	
	SPI_WriteComm(0xd212);SPI_WriteData(0x0001);	
	SPI_WriteComm(0xd213);SPI_WriteData(0x00b9);	
	SPI_WriteComm(0xd214);SPI_WriteData(0x0001);	
	SPI_WriteComm(0xd215);SPI_WriteData(0x00fb);	
	SPI_WriteComm(0xd216);SPI_WriteData(0x0002);	
	SPI_WriteComm(0xd217);SPI_WriteData(0x0063);	
	SPI_WriteComm(0xd218);SPI_WriteData(0x0002);	
	SPI_WriteComm(0xd219);SPI_WriteData(0x00b9);	
	SPI_WriteComm(0xd21a);SPI_WriteData(0x0002);	
	SPI_WriteComm(0xd21b);SPI_WriteData(0x00bb);	
	SPI_WriteComm(0xd21c);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd21d);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd21e);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd21f);SPI_WriteData(0x0046);	
	SPI_WriteComm(0xd220);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd221);SPI_WriteData(0x0069);	
	SPI_WriteComm(0xd222);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd223);SPI_WriteData(0x008f);	
	SPI_WriteComm(0xd224);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd225);SPI_WriteData(0x00a4);	
	SPI_WriteComm(0xd226);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd227);SPI_WriteData(0x00b9);	
	SPI_WriteComm(0xd228);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd229);SPI_WriteData(0x00c7);	
	SPI_WriteComm(0xd22a);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd22b);SPI_WriteData(0x00c9);	
	SPI_WriteComm(0xd22c);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd22d);SPI_WriteData(0x00cb);	
	SPI_WriteComm(0xd22e);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd22f);SPI_WriteData(0x00cb);	
	SPI_WriteComm(0xd230);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd231);SPI_WriteData(0x00cb);	
	SPI_WriteComm(0xd232);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd233);SPI_WriteData(0x00cc);	
																						 
																						 
	SPI_WriteComm(0xd300);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd301);SPI_WriteData(0x005d);	
	SPI_WriteComm(0xd302);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd303);SPI_WriteData(0x006b);	
	SPI_WriteComm(0xd304);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd305);SPI_WriteData(0x0084);	
	SPI_WriteComm(0xd306);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd307);SPI_WriteData(0x009c);	
	SPI_WriteComm(0xd308);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd309);SPI_WriteData(0x00b1);	
	SPI_WriteComm(0xd30a);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd30b);SPI_WriteData(0x00d9);	
	SPI_WriteComm(0xd30c);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd30d);SPI_WriteData(0x00fd);	
	SPI_WriteComm(0xd30e);SPI_WriteData(0x0001);	
	SPI_WriteComm(0xd30f);SPI_WriteData(0x0038);	
	SPI_WriteComm(0xd310);SPI_WriteData(0x0001);	
	SPI_WriteComm(0xd311);SPI_WriteData(0x0068);	
	SPI_WriteComm(0xd312);SPI_WriteData(0x0001);	
	SPI_WriteComm(0xd313);SPI_WriteData(0x00b9);	
	SPI_WriteComm(0xd314);SPI_WriteData(0x0001);	
	SPI_WriteComm(0xd315);SPI_WriteData(0x00fb);	
	SPI_WriteComm(0xd316);SPI_WriteData(0x0002);	
	SPI_WriteComm(0xd317);SPI_WriteData(0x0063);	
	SPI_WriteComm(0xd318);SPI_WriteData(0x0002);	
	SPI_WriteComm(0xd319);SPI_WriteData(0x00b9);	
	SPI_WriteComm(0xd31a);SPI_WriteData(0x0002);	
	SPI_WriteComm(0xd31b);SPI_WriteData(0x00bb);	
	SPI_WriteComm(0xd31c);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd31d);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd31e);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd31f);SPI_WriteData(0x0046);	
	SPI_WriteComm(0xd320);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd321);SPI_WriteData(0x0069);	
	SPI_WriteComm(0xd322);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd323);SPI_WriteData(0x008f);	
	SPI_WriteComm(0xd324);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd325);SPI_WriteData(0x00a4);	
	SPI_WriteComm(0xd326);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd327);SPI_WriteData(0x00b9);	
	SPI_WriteComm(0xd328);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd329);SPI_WriteData(0x00c7);	
	SPI_WriteComm(0xd32a);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd32b);SPI_WriteData(0x00c9);	
	SPI_WriteComm(0xd32c);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd32d);SPI_WriteData(0x00cb);	
	SPI_WriteComm(0xd32e);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd32f);SPI_WriteData(0x00cb);	
	SPI_WriteComm(0xd330);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd331);SPI_WriteData(0x00cb);	
	SPI_WriteComm(0xd332);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd333);SPI_WriteData(0x00cc);	
																						 
	SPI_WriteComm(0xd400);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd401);SPI_WriteData(0x005d);	
	SPI_WriteComm(0xd402);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd403);SPI_WriteData(0x006b);	
	SPI_WriteComm(0xd404);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd405);SPI_WriteData(0x0084);	
	SPI_WriteComm(0xd406);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd407);SPI_WriteData(0x009c);	
	SPI_WriteComm(0xd408);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd409);SPI_WriteData(0x00b1);	
	SPI_WriteComm(0xd40a);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd40b);SPI_WriteData(0x00d9);	
	SPI_WriteComm(0xd40c);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd40d);SPI_WriteData(0x00fd);	
	SPI_WriteComm(0xd40e);SPI_WriteData(0x0001);	
	SPI_WriteComm(0xd40f);SPI_WriteData(0x0038);	
	SPI_WriteComm(0xd410);SPI_WriteData(0x0001);	
	SPI_WriteComm(0xd411);SPI_WriteData(0x0068);	
	SPI_WriteComm(0xd412);SPI_WriteData(0x0001);	
	SPI_WriteComm(0xd413);SPI_WriteData(0x00b9);	
	SPI_WriteComm(0xd414);SPI_WriteData(0x0001);	
	SPI_WriteComm(0xd415);SPI_WriteData(0x00fb);	
	SPI_WriteComm(0xd416);SPI_WriteData(0x0002);	
	SPI_WriteComm(0xd417);SPI_WriteData(0x0063);	
	SPI_WriteComm(0xd418);SPI_WriteData(0x0002);	
	SPI_WriteComm(0xd419);SPI_WriteData(0x00b9);	
	SPI_WriteComm(0xd41a);SPI_WriteData(0x0002);	
	SPI_WriteComm(0xd41b);SPI_WriteData(0x00bb);	
	SPI_WriteComm(0xd41c);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd41d);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd41e);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd41f);SPI_WriteData(0x0046);	
	SPI_WriteComm(0xd420);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd421);SPI_WriteData(0x0069);	
	SPI_WriteComm(0xd422);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd423);SPI_WriteData(0x008f);	
	SPI_WriteComm(0xd424);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd425);SPI_WriteData(0x00a4);	
	SPI_WriteComm(0xd426);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd427);SPI_WriteData(0x00b9);	
	SPI_WriteComm(0xd428);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd429);SPI_WriteData(0x00c7);	
	SPI_WriteComm(0xd42a);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd42b);SPI_WriteData(0x00c9);	
	SPI_WriteComm(0xd42c);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd42d);SPI_WriteData(0x00cb);	
	SPI_WriteComm(0xd42e);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd42f);SPI_WriteData(0x00cb);	
	SPI_WriteComm(0xd430);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd431);SPI_WriteData(0x00cb);	
	SPI_WriteComm(0xd432);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd433);SPI_WriteData(0x00cc);	
																						 
																						 
	SPI_WriteComm(0xd500);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd501);SPI_WriteData(0x005d);	
	SPI_WriteComm(0xd502);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd503);SPI_WriteData(0x006b);	
	SPI_WriteComm(0xd504);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd505);SPI_WriteData(0x0084);	
	SPI_WriteComm(0xd506);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd507);SPI_WriteData(0x009c);	
	SPI_WriteComm(0xd508);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd509);SPI_WriteData(0x00b1);	
	SPI_WriteComm(0xd50a);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd50b);SPI_WriteData(0x00D9);	
	SPI_WriteComm(0xd50c);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd50d);SPI_WriteData(0x00fd);	
	SPI_WriteComm(0xd50e);SPI_WriteData(0x0001);	
	SPI_WriteComm(0xd50f);SPI_WriteData(0x0038);	
	SPI_WriteComm(0xd510);SPI_WriteData(0x0001);	
	SPI_WriteComm(0xd511);SPI_WriteData(0x0068);	
	SPI_WriteComm(0xd512);SPI_WriteData(0x0001);	
	SPI_WriteComm(0xd513);SPI_WriteData(0x00b9);	
	SPI_WriteComm(0xd514);SPI_WriteData(0x0001);	
	SPI_WriteComm(0xd515);SPI_WriteData(0x00fb);	
	SPI_WriteComm(0xd516);SPI_WriteData(0x0002);	
	SPI_WriteComm(0xd517);SPI_WriteData(0x0063);	
	SPI_WriteComm(0xd518);SPI_WriteData(0x0002);	
	SPI_WriteComm(0xd519);SPI_WriteData(0x00b9);	
	SPI_WriteComm(0xd51a);SPI_WriteData(0x0002);	
	SPI_WriteComm(0xd51b);SPI_WriteData(0x00bb);	
	SPI_WriteComm(0xd51c);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd51d);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd51e);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd51f);SPI_WriteData(0x0046);	
	SPI_WriteComm(0xd520);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd521);SPI_WriteData(0x0069);	
	SPI_WriteComm(0xd522);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd523);SPI_WriteData(0x008f);	
	SPI_WriteComm(0xd524);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd525);SPI_WriteData(0x00a4);	
	SPI_WriteComm(0xd526);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd527);SPI_WriteData(0x00b9);	
	SPI_WriteComm(0xd528);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd529);SPI_WriteData(0x00c7);	
	SPI_WriteComm(0xd52a);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd52b);SPI_WriteData(0x00c9);	
	SPI_WriteComm(0xd52c);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd52d);SPI_WriteData(0x00cb);	
	SPI_WriteComm(0xd52e);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd52f);SPI_WriteData(0x00cb);	
	SPI_WriteComm(0xd530);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd531);SPI_WriteData(0x00cb);	
	SPI_WriteComm(0xd532);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd533);SPI_WriteData(0x00cc);	
																						 
	SPI_WriteComm(0xd600);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd601);SPI_WriteData(0x005d);	
	SPI_WriteComm(0xd602);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd603);SPI_WriteData(0x006b);	
	SPI_WriteComm(0xd604);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd605);SPI_WriteData(0x0084);	
	SPI_WriteComm(0xd606);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd607);SPI_WriteData(0x009c);	
	SPI_WriteComm(0xd608);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd609);SPI_WriteData(0x00b1);	
	SPI_WriteComm(0xd60a);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd60b);SPI_WriteData(0x00d9);	
	SPI_WriteComm(0xd60c);SPI_WriteData(0x0000);	
	SPI_WriteComm(0xd60d);SPI_WriteData(0x00fd);	
	SPI_WriteComm(0xd60e);SPI_WriteData(0x0001);	
	SPI_WriteComm(0xd60f);SPI_WriteData(0x0038);	
	SPI_WriteComm(0xd610);SPI_WriteData(0x0001);	
	SPI_WriteComm(0xd611);SPI_WriteData(0x0068);	
	SPI_WriteComm(0xd612);SPI_WriteData(0x0001);	
	SPI_WriteComm(0xd613);SPI_WriteData(0x00b9);	
	SPI_WriteComm(0xd614);SPI_WriteData(0x0001);	
	SPI_WriteComm(0xd615);SPI_WriteData(0x00fb);	
	SPI_WriteComm(0xd616);SPI_WriteData(0x0002);	
	SPI_WriteComm(0xd617);SPI_WriteData(0x0063);	
	SPI_WriteComm(0xd618);SPI_WriteData(0x0002);	
	SPI_WriteComm(0xd619);SPI_WriteData(0x00b9);	
	SPI_WriteComm(0xd61a);SPI_WriteData(0x0002);	
	SPI_WriteComm(0xd61b);SPI_WriteData(0x00bb);	
	SPI_WriteComm(0xd61c);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd61d);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd61e);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd61f);SPI_WriteData(0x0046);	
	SPI_WriteComm(0xd620);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd621);SPI_WriteData(0x0069);	
	SPI_WriteComm(0xd622);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd623);SPI_WriteData(0x008f);	
	SPI_WriteComm(0xd624);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd625);SPI_WriteData(0x00a4);	
	SPI_WriteComm(0xd626);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd627);SPI_WriteData(0x00b9);	
	SPI_WriteComm(0xd628);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd629);SPI_WriteData(0x00c7);	
	SPI_WriteComm(0xd62a);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd62b);SPI_WriteData(0x00c9);	
	SPI_WriteComm(0xd62c);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd62d);SPI_WriteData(0x00cb);	
	SPI_WriteComm(0xd62e);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd62f);SPI_WriteData(0x00cb);	
	SPI_WriteComm(0xd630);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd631);SPI_WriteData(0x00cb);	
	SPI_WriteComm(0xd632);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xd633);SPI_WriteData(0x00cc);	
																						 
	SPI_WriteComm(0xba00);SPI_WriteData(0x0024);	
	SPI_WriteComm(0xba01);SPI_WriteData(0x0024);	
	SPI_WriteComm(0xba02);SPI_WriteData(0x0024);	
																						 
	SPI_WriteComm(0xb900);SPI_WriteData(0x0024);	
	SPI_WriteComm(0xb901);SPI_WriteData(0x0024);	
	SPI_WriteComm(0xb902);SPI_WriteData(0x0024);	

	SPI_WriteComm(0xf000);SPI_WriteData(0x0055);          
	SPI_WriteComm(0xf001);SPI_WriteData(0x00aa);	
	SPI_WriteComm(0xf002);SPI_WriteData(0x0052);	
	SPI_WriteComm(0xf003);SPI_WriteData(0x0008);	
	SPI_WriteComm(0xf004);SPI_WriteData(0x0000);	
																						 
																						 
	SPI_WriteComm(0xb100);SPI_WriteData(0x00cc);	
																						 
																						 
	SPI_WriteComm(0xbc00);SPI_WriteData(0x0005);	
	SPI_WriteComm(0xbc01);SPI_WriteData(0x0005);	
	SPI_WriteComm(0xbc02);SPI_WriteData(0x0005);	
																						 
	SPI_WriteComm(0xb800);SPI_WriteData(0x0001);	
	SPI_WriteComm(0xb801);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xb802);SPI_WriteData(0x0003);	
	SPI_WriteComm(0xb803);SPI_WriteData(0x0003);	
																						 
																						 
	SPI_WriteComm(0xbd02);SPI_WriteData(0x0007);	
	SPI_WriteComm(0xbd03);SPI_WriteData(0x0031);	
	SPI_WriteComm(0xbe02);SPI_WriteData(0x0007);	
	SPI_WriteComm(0xbe03);SPI_WriteData(0x0031);	
	SPI_WriteComm(0xbf02);SPI_WriteData(0x0007);	
	SPI_WriteComm(0xbf03);SPI_WriteData(0x0031);	
																						 
																						 
	SPI_WriteComm(0xff00);SPI_WriteData(0x00aa);	
	SPI_WriteComm(0xff01);SPI_WriteData(0x0055);	
	SPI_WriteComm(0xff02);SPI_WriteData(0x0025);	
	SPI_WriteComm(0xff03);SPI_WriteData(0x0001);	


	SPI_WriteComm(0xf304);SPI_WriteData(0x0011);	
	SPI_WriteComm(0xf306);SPI_WriteData(0x0010);	
	SPI_WriteComm(0xf308);SPI_WriteData(0x0000);	
																						 
	SPI_WriteComm(0x3500);SPI_WriteData(0x0000);	

	SPI_WriteComm(0x3A00);SPI_WriteData(0x0005);


	SPI_WriteComm(0x2a00);SPI_WriteData(0x0000);
	SPI_WriteComm(0x2a01);SPI_WriteData(0x0000);	
	SPI_WriteComm(0x2a02);SPI_WriteData(0x0001);	
	SPI_WriteComm(0x2a03);SPI_WriteData(0x00df);	
																						 
	SPI_WriteComm(0x2b00);SPI_WriteData(0x0000);  
	SPI_WriteComm(0x2b01);SPI_WriteData(0x0000);	
	SPI_WriteComm(0x2b02);SPI_WriteData(0x0003);	
	SPI_WriteComm(0x2b03);SPI_WriteData(0x001f);	



	SPI_WriteComm(0x1100);
	delay_ms(120);

	SPI_WriteComm(0x2900);

	SPI_WriteComm(0x2c00);
	SPI_WriteComm(0x3c00);

	SPI_WriteComm(0x3A); SPI_WriteData(0x75);
	SPI_WriteComm(0x36); SPI_WriteData(0x00);
	
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------  

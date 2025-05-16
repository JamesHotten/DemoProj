//-----------------------------------------------------------------
// 程序描述:
//     4.3寸/7寸RGB液晶显示屏驱动程序头文件
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
#ifndef _LCD_H
#define _LCD_H
#include "stm32f429_winner.h"
//-----------------------------------------------------------------
// 变量声明
//-----------------------------------------------------------------
// LCD LTDC重要参数集
typedef struct  
{							 
	u32 pwidth;				// LCD面板的宽度,固定参数,不随显示方向改变,如果为0,说明没有任何RGB屏接入
	u32 pheight;			// LCD面板的高度,固定参数,不随显示方向改变
	u16 hsw;					// 水平同步宽度
	u16 vsw;					// 垂直同步宽度
	u16 hbp;					// 水平后廊
	u16 vbp;					// 垂直后廊
	u16 hfp;					// 水平前廊
	u16 vfp;					// 垂直前廊 
	u8 activelayer;		// 当前层编号:0/1	
	u8 dir;						// 显示方向
	u16 width;				// LCD宽度
	u16 height;				// LCD高度
	u32 pixsize;			// 每个像素所占字节数
}_ltdc_dev; 

extern _ltdc_dev lcdltdc;		            		// 管理LCD LTDC参数
extern LTDC_HandleTypeDef LTDC_Handler;	    // LTDC句柄
extern DMA2D_HandleTypeDef DMA2D_Handler;   // DMA2D句柄

//-----------------------------------------------------------------
// 宏定义
//-----------------------------------------------------------------
//LCD背光	PC6	
#define LTCD_LED     PCout(6)   

#define LCD_PIXEL_FORMAT_ARGB8888       0X00    
#define LCD_PIXEL_FORMAT_RGB888         0X01    
#define LCD_PIXEL_FORMAT_RGB565         0X02       
#define LCD_PIXEL_FORMAT_ARGB1555       0X03      
#define LCD_PIXEL_FORMAT_ARGB4444       0X04     
#define LCD_PIXEL_FORMAT_L8             0X05     
#define LCD_PIXEL_FORMAT_AL44           0X06     
#define LCD_PIXEL_FORMAT_AL88           0X07      

// 用户修改配置部分:
// 定义颜色像素格式,一般用RGB565
#define LCD_PIXFORMAT				LCD_PIXEL_FORMAT_RGB565	
// 定义默认背景层颜色
#define LTDC_BACKLAYERCOLOR			0X00000000	
// LCD帧缓冲区首地址,这里定义在SDRAM里面.
#define LCD_FRAME_BUF_ADDR			0XC0000000  

//-----------------------------------------------------------------
// 函数声明
//-----------------------------------------------------------------
extern void LTDC_Switch(u8 sw);											// LTDC开关
extern void LTDC_Layer_Switch(u8 layerx,u8 sw);			// 层开关
extern void LTDC_Select_Layer(u8 layerx);						// 层选择
extern void LTDC_Display_Dir(u8 dir);								// 显示方向控制
extern void LTDC_Draw_Point(u16 x,u16 y,u32 color);	// 画点函数
extern u32 LTDC_Read_Point(u16 x,u16 y);						// 读点函数
extern void LTDC_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color);					// 矩形单色填充函数
extern void LTDC_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);	// 矩形彩色填充函数
extern void LTDC_Clear(u32 color);									// 清屏函数
extern u8 LTDC_Clk_Set(u32 pllsain,u32 pllsair,u32 pllsaidivr);// LTDC时钟配置
extern void LTDC_Layer_Window_Config(u8 layerx,u16 sx,u16 sy,u16 width,u16 height);// LTDC层窗口设置
extern void LTDC_Layer_Parameter_Config(u8 layerx,u32 bufaddr,u8 pixformat,u8 alpha,u8 alpha0,u8 bfac1,u8 bfac2,u32 bkcolor);// LTDC基本参数设置
extern void LTDC_Init(void);						// LTDC初始化函数
extern void Lcd_Initialize(void);
#endif 
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------  

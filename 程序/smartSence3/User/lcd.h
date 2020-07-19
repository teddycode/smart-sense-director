
#ifndef __LCD_H
#define __LCD_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h"

/* Exported types ------------------------------------------------------------*/
#define TFT_SPI_SCK_GPIO_PORT			GPIOA/*  SPI_CLK	   */	
#define TFT_SPI_SCK_PIN				    GPIO_Pin_5		   	/*    Pe.4     */
#define TFT_SPI_SCK_GPIO_CLK			RCC_APB2Periph_GPIOA

#define TFT_SPI_MOSI_GPIO_PORT 		    GPIOA           	/*  SPI_MOSI   */
#define TFT_SPI_MOSI_PIN				GPIO_Pin_7	   		/*     PE.3    */
#define TFT_SPI_MOSI_GPIO_CLK			RCC_APB2Periph_GPIOA

#define TFT_SPI_MISO_GPIO_PORT 	        GPIOA         	    /*  SPI_MISO   */
#define TFT_SPI_MISO_PIN				GPIO_Pin_6         /*    PA.6     */
#define TFT_SPI_MISO_GPIO_CLK		    RCC_APB2Periph_GPIOA

#define TFT_SPI_CS_GPIO_PORT			GPIOA				/*   SPI_CS    */
#define TFT_SPI_CS_PIN					GPIO_Pin_1			/*      PE.6   */ 
#define TFT_SPI_CS_GPIO_CLK			    RCC_APB2Periph_GPIOA

#define TFT_DC_GPIO_PORT				GPIOA				/*   TFT d/c   */
#define TFT_DC_GPIO_PIN			    	GPIO_Pin_0			/*    PE.2     */
#define TFT_DC_GPIO_CLK				    RCC_APB2Periph_GPIOA

#define TFT_RES_GPIO_PORT				GPIOB				/*    TFT RES */
#define TFT_RES_GPIO_PIN				GPIO_Pin_0			/*	  PE.1    */
#define TFT_RES_GPIO_CLK				RCC_APB2Periph_GPIOB

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

#define TFT_CS_LOW()			  	    TFT_SPI_CS_GPIO_PORT->BRR = TFT_SPI_CS_PIN		    //set CS bit is Reset(low)
#define TFT_CS_HIGH()				    TFT_SPI_CS_GPIO_PORT->BSRR= TFT_SPI_CS_PIN			//set CS bit is Set(high)

#define TFT_DC_LOW()					TFT_DC_GPIO_PORT->BRR = TFT_DC_GPIO_PIN			    //set D/C bit is Reset(low)
#define TFT_DC_HIGH()					TFT_DC_GPIO_PORT->BSRR= TFT_DC_GPIO_PIN			    //set D/C bit is Set(high)

#define TFT_RES_LOW()					(TFT_RES_GPIO_PORT->BRR = TFT_RES_GPIO_PIN)		    //set RES bit is Reset(low)
#define TFT_RES_HIGH()					(TFT_RES_GPIO_PORT->BSRR= TFT_RES_GPIO_PIN)			//set RES bit is Set(high)

#define TFT_SCK_LOW()					(TFT_SPI_SCK_GPIO_PORT->BRR = TFT_SPI_SCK_PIN)		//set SCK bit is Reset(low)
#define TFT_SCK_HIGH()					(TFT_SPI_SCK_GPIO_PORT->BSRR= TFT_SPI_SCK_PIN)	    //set SCK bit is Set(high)

#define TFT_MOSI_LOW()					(TFT_SPI_MOSI_GPIO_PORT->BRR = TFT_SPI_MOSI_PIN)	//set MOSI bit is Reset(low)
#define TFT_MOSI_HIGH()				    (TFT_SPI_MOSI_GPIO_PORT->BSRR= TFT_SPI_MOSI_PIN)    //set MOSI bit is Set(high)

#define TFT_MISO_INPUT()          ( (TFT_SPI_MISO_GPIO_PORT->IDR & TFT_SPI_MISO_PIN)? 1 :0 )

#define CMD 	0    //写命令
#define DATA 	1	 //写数据
/* Exported Values ------------------------------------------------------- */

#define TFT_NOP_CMD   0x00  //LCDNOP指令

/*X表示X镜像，Y表示Y镜像，E表示XY互换*/
typedef enum
{
	/*s竖屏*/
	YNormal = 0x48,
	NYNormal= 0x68,
	/*横屏*/
	XNormal = 0xE8,
	NXNormal= 0x28,
	
	/*更改*/
	XY_Exchange=0x10,
	TopBottom_Exchange=0x20,
	LeftRight_Exchange=0x04,	
	X_Mirror=0x40,
	Y_Mirror=0x80	
}LCD_DispMode;

	 
/*RGB-565 颜色表*/
//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 					 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


#define LCD_WIDTH		  240
#define LCD_HEIGHT		320
#define LCD_PIX_NUM		LCD_WIDTH    *    LCD_HEIGHT

u16 TFT_SendHalfWord(u16 byte);

void TFT_Init(void);
void LCD_DispCtrl(LCD_DispMode mode); 				 //设置显示模式
void LCD_SetCursor(u16 Xpos, u16 Ypos);  			//设置光标位置
void LCD_DrawPoint(u16 x,u16 y,u16 color); 		 //画点
u16  LCD_ReadPoint(u16 x,u16 y); 							 //读取某点颜色
void LCD_OpenWindow(u16 x1,u16 y1,u16 x2,u16 y2); 	 	//开窗
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color); //颜色填充
void LCD_Clear(u16 color);  //清屏
void LCD_DispChar(u16 x,u16 y, char num, u8 mode); //显示自符
void LCD_DispStr(u16 x,u16 y, u8* str);						 //显示字符串
void LCD_DispInt(u16 x,u16 y,u32 num,u8 len);  //右对齐显示整形数字
void LCD_DispImg(u16 X_pos,u16 Y_pos,u16 X_len,u16 Y_len,u16 *pImgData);  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);    //画线
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);   //画矩形
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r,u16 color);     //画圆
void LCD_DrawHLine( u16 x0, u16 y0, u16 x1 , u16 color );
void LCD_DrawVLine( u16 x0, u16 y0, u16 y1, u16 color ); // 
void LCD_FillRect( u16 x0, u16 y0, u16 x1, u16 y1 , u16 color );  //实心矩形
void LCD_Disp_CnEn(u16 x, u16 y, u16 fc, u16 bc, u8 *s);// 显示汉字
void LCD_Disp_BufCn(u16 x, u16 y,u16 fc, u16 bc, u8 *buf,u16 len);//显示自定义汉字
void LCD_Show_Image(u16 x,u16 y,u16 x_len,u16 y_len, const u8 *p); 


#endif 

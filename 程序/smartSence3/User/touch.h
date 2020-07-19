
#ifndef __TOUCH_H
#define __TOUCH_H	

#include "sys.h"

#define  CMD_RDX  0xD0	 //触摸IC读坐标积存器
#define  CMD_RDY  0x90

#define PEN  	PCin(15)  	//PC10 INT
#define PEN_SET  PCout(15) 
#define MISO 	PAin(6)   	//PC2  MISO
#define CS2  	PCout(14)  	//PC0  CS 

#define OLED_CS_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_1)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_1)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_0)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOB,GPIO_Pin_0)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_0)//DC
#define OLED_DC_Set() GPIO_SetBits(GPIOA,GPIO_Pin_0)


#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_7)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_7)


struct tp_pix_
{
	u16 x;
	u16 y;
};
struct tp_pixu32_
{
	u32 x;
	u32 y;
};

extern struct tp_pix_  tp_pixad,tp_pixlcd;	 //当前触控坐标的AD值,前触控坐标的像素值   
extern u16 vx,vy;  //比例因子，此值除以1000之后表示多少个AD值代表一个像素点
extern u16 chx,chy;//默认像素点坐标为0时的AD起始值
u8 Touch_State(void);
void spistar(void);  
void Touch_Adjust(void);
void point(void); //绘图函数
u16 ReadFromCharFrom7843(void);         //SPI 读数据
void Touch_Init(void);
void Touch_Read_Pos(void);  //读取坐标值
#endif  
	 
	 




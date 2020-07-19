#include "lcd.h"
#include "delay.h"
#include "font.h"
#include "HanZi.h"

/*由于写入数据先后问题，选择BGR方式*/
static u8 DispCtrlData=XY_Exchange;  //C8倒立	  //48 68竖屏//28 E8 横屏
u16 BACK_COLOR, POINT_COLOR;   //背景色，画笔色

/**
  * @brief  TFT 端口初始化
  * @param  None
  * @retval None
  */
	
void TFT_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  SPI_InitTypeDef  SPI_InitStructure;
    
	RCC_APB2PeriphClockCmd(TFT_SPI_SCK_GPIO_CLK|TFT_SPI_MOSI_GPIO_CLK|TFT_SPI_CS_GPIO_CLK| TFT_SPI_MISO_GPIO_CLK| \
						   TFT_DC_GPIO_CLK|TFT_RES_GPIO_CLK,   ENABLE);	

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1,  ENABLE );//SPI1时钟使能 	
  
//SPI IO init
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);  //PB13/14/15上拉


	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		//定义波特率预分频的值:波特率预分频值为16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设

	/*!< Configure TFT_SPI pin:  CS pin */
	GPIO_InitStructure.GPIO_Pin = TFT_SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(TFT_SPI_CS_GPIO_PORT, &GPIO_InitStructure);
	/*Deselect the TFT : Chip Select high  */
	TFT_CS_HIGH();

	/*!< Configure TFT_DC pin:  DC pin */
	GPIO_InitStructure.GPIO_Pin = TFT_DC_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(TFT_DC_GPIO_PORT, &GPIO_InitStructure);

	/*!< Configure TFT_RES pin:  RES pin */
	GPIO_InitStructure.GPIO_Pin = TFT_RES_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(TFT_RES_GPIO_PORT, &GPIO_InitStructure);
 
}

/**
  * @brief  TFT_SendByte
  * @param  
		u8 byte : byte to send
  * @retval 
		The value of the received byte
  */
u8 TFT_SendByte(u8 byte,u8 Com)
{
	u8 Dat=0;	
	u8 i;

	//           write data           write command 
	( Com ? ( TFT_DC_HIGH() ) : ( TFT_DC_LOW() ) );
	TFT_CS_LOW();

	SPI_DataSizeConfig(SPI1, SPI_DataSize_8b);

	i=0;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
			i++;
			if(i>200)
					return 0;
	}			  
	SPI_I2S_SendData(SPI1, byte); //通过外设SPIx发送一个数据
	i=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
			i++;
			if(i>200)
					return 0;
	}	  						    
	Dat = SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据	
			
	TFT_CS_HIGH();
	TFT_DC_HIGH();	

	return Dat;
}


u16 TFT_SendHalfWord(u16 byte)
{	
	u16 i;
	u16 Dat=0;	

	TFT_DC_HIGH();		//write data 	
	TFT_CS_LOW();

	SPI_DataSizeConfig(SPI1, SPI_DataSize_16b);

	i = 0;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		i++;
		if(i>200)return 0;
	}			  
	SPI_I2S_SendData(SPI1, byte); //通过外设SPIx发送一个数据

	i=0;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		i++;
		if(i>200)return 0;
	}	
	Dat = SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据	

	TFT_CS_HIGH();
	TFT_DC_HIGH();
	return 	Dat;
}



/**
  * @brief TFT初始化
  * @param  None
  * @retval None
  */
void TFT_Init(void)
{
	TFT_GPIO_Init();

	TFT_CS_LOW();	
	TFT_RES_LOW();
	DelayMs(30);
	TFT_RES_HIGH();
	DelayMs(30);

    /*  Power control A (CBh) */
	TFT_SendByte(0xCB,CMD);  
	TFT_SendByte(0x39,DATA); 
	TFT_SendByte(0x2C,DATA); 
	TFT_SendByte(0x00,DATA); 
	TFT_SendByte(0x34,DATA); 
	TFT_SendByte(0x02,DATA); 

	/*  Power control B (CFh)  */
	TFT_SendByte(0xCF,CMD);  
	TFT_SendByte(0x00,DATA); 
	TFT_SendByte(0XD9,DATA); 
	TFT_SendByte(0X30,DATA); 

	/*  Driver timing control A (E8h) */
	TFT_SendByte(0xE8,CMD);  
	TFT_SendByte(0x85,DATA); 
	TFT_SendByte(0x10,DATA); 
	TFT_SendByte(0x78,DATA); 

	/* Driver timing control B */
	TFT_SendByte(0xEA,CMD);  
	TFT_SendByte(0x00,DATA); 
	TFT_SendByte(0x00,DATA); 

	/*  Power on sequence control (EDh) */
	TFT_SendByte(0xED,CMD);  
	TFT_SendByte(0x64,DATA); 
	TFT_SendByte(0x03,DATA); 
	TFT_SendByte(0X12,DATA); 
	TFT_SendByte(0X81,DATA); 

	/* Pump ratio control (F7h) */
	TFT_SendByte(0xF7,CMD);  
	TFT_SendByte(0x20,DATA); 

	/* Power Control 1 (C0h) */
	TFT_SendByte(0xC0,CMD);    //Power control
	TFT_SendByte(0x21,DATA);   //VRH[5:0] 
	
	/* Power Control 2 (C1h) */
	TFT_SendByte(0xC1,CMD);    //Power control 
	TFT_SendByte(0x12,DATA);   //SAP[2:0];BT[3:0] 
	
	/* VCOM Control 1(C5h) */
	TFT_SendByte(0xC5,CMD);    //VCM control 
	TFT_SendByte(0x32,DATA); //对比度调节
	TFT_SendByte(0x3C,DATA); 

	/*  VCOM Control 2(C7h)  */
	TFT_SendByte(0xC7,CMD);    //VCM control2 
	TFT_SendByte(0XC1,DATA);  //--

	/*旋转设置*/
	TFT_SendByte(0x36,CMD);    // Memory Access Control 
	TFT_SendByte(DispCtrlData,DATA); //C8	  //48 68竖屏//28 E8 横屏

	TFT_SendByte(0x3A,CMD);    
	TFT_SendByte(0x55,DATA); 

	/* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
	TFT_SendByte(0xB1,CMD);    
	TFT_SendByte(0x00,DATA);  
	TFT_SendByte(0x18,DATA); 
	
	/*  Display Function Control (B6h) */
	TFT_SendByte(0xB6,CMD);    // Display Function Control 
	TFT_SendByte(0x0A,DATA); 
	TFT_SendByte(0xA2,DATA);
//	TFT_SendByte(0x27,DATA);  
	
	/* Enable 3G (F2h) */
	TFT_SendByte(0xF2,CMD);    // 3Gamma Function Disable 
	TFT_SendByte(0x00,DATA); 

	/* Gamma Set (26h) */
	TFT_SendByte(0x26,CMD);    //Gamma curve selected 
	TFT_SendByte(0x01,DATA); 
/* Positive Gamma Correction */
	TFT_SendByte(0xE0,CMD);    //Set Gamma 
	TFT_SendByte(0x0F,DATA);
	TFT_SendByte(0x20,DATA);
	TFT_SendByte(0x1E,DATA);
	TFT_SendByte(0x09,DATA);
	TFT_SendByte(0x12,DATA);
	TFT_SendByte(0x0B,DATA);
	TFT_SendByte(0x50,DATA);
	TFT_SendByte(0XBA,DATA);
	TFT_SendByte(0x44,DATA);
	TFT_SendByte(0x09,DATA);
	TFT_SendByte(0x14,DATA);
	TFT_SendByte(0x05,DATA);
	TFT_SendByte(0x23,DATA);
	TFT_SendByte(0x21,DATA);
	TFT_SendByte(0x00,DATA);

	TFT_SendByte(0XE1,CMD);    //Set Gamma 
	TFT_SendByte(0x00,DATA);
	TFT_SendByte(0x19,DATA);
	TFT_SendByte(0x19,DATA);
	TFT_SendByte(0x00,DATA);
	TFT_SendByte(0x12,DATA);
	TFT_SendByte(0x07,DATA);
	TFT_SendByte(0x2D,DATA);
	TFT_SendByte(0x28,DATA);
	TFT_SendByte(0x3F,DATA);
	TFT_SendByte(0x02,DATA);
	TFT_SendByte(0x0A,DATA);
	TFT_SendByte(0x08,DATA);
	TFT_SendByte(0x25,DATA);
	TFT_SendByte(0x2D,DATA);
	TFT_SendByte(0x0F,DATA);

	TFT_SendByte(0x11,CMD);    //Exit Sleep 
	DelayMs(120);

	TFT_SendByte(0x29,CMD);    //Display on 
//	TFT_SendByte(0x2c,CMD); 
	
	//	LCD_Clear(RED);
}
  

/**
  * @brief TFT初始化
  * @param  None
  * @retval None
  */

/*
void TFT_Init(void)
{
	TFT_GPIO_Init();


	TFT_RES_LOW();
	DelayMs(30);
	TFT_RES_HIGH();
	DelayMs(30);

    
	TFT_SendByte(0xCB,CMD);  
	TFT_SendByte(0x39,DATA); 
	TFT_SendByte(0x2C,DATA); 
	TFT_SendByte(0x00,DATA); 
	TFT_SendByte(0x34,DATA); 
	TFT_SendByte(0x02,DATA); 

	
	TFT_SendByte(0xCF,CMD);  
	TFT_SendByte(0x00,DATA); 
	TFT_SendByte(0XC1,DATA); 
	TFT_SendByte(0X30,DATA); 


	TFT_SendByte(0xE8,CMD);  
	TFT_SendByte(0x85,DATA); 
	TFT_SendByte(0x00,DATA); 
	TFT_SendByte(0x78,DATA); 

	TFT_SendByte(0xEA,CMD);  
	TFT_SendByte(0x00,DATA); 
	TFT_SendByte(0x00,DATA); 

	TFT_SendByte(0xED,CMD);  
	TFT_SendByte(0x64,DATA); 
	TFT_SendByte(0x03,DATA); 
	TFT_SendByte(0X12,DATA); 
	TFT_SendByte(0X81,DATA); 

	
	TFT_SendByte(0xF7,CMD);  
	TFT_SendByte(0x20,DATA); 

	
	TFT_SendByte(0xC0,CMD);    //Power control
	TFT_SendByte(0x23,DATA);   //VRH[5:0] 
	

	TFT_SendByte(0xC1,CMD);    //Power control 
	TFT_SendByte(0x10,DATA);   //SAP[2:0];BT[3:0] 
	

	TFT_SendByte(0xC5,CMD);    //VCM control 
	TFT_SendByte(0x45,DATA); //对比度调节
	TFT_SendByte(0x45,DATA); 

	
	TFT_SendByte(0xC7,CMD);    //VCM control2 
	TFT_SendByte(0x86,DATA);  //--


	TFT_SendByte(0x36,CMD);    // Memory Access Control 
	TFT_SendByte(DispCtrlData,DATA); //C8	  //48 68竖屏//28 E8 横屏

	TFT_SendByte(0x3A,CMD);    
	TFT_SendByte(0x55,DATA); 

	
	TFT_SendByte(0xB1,CMD);    
	TFT_SendByte(0x00,DATA);  
	TFT_SendByte(0x1B,DATA); 

	TFT_SendByte(0xB6,CMD);    // Display Function Control 
	TFT_SendByte(0x0A,DATA); 
	TFT_SendByte(0xA2,DATA);
//	TFT_SendByte(0x27,DATA);  

	TFT_SendByte(0xF2,CMD);    // 3Gamma Function Disable 
	TFT_SendByte(0x00,DATA); 

	TFT_SendByte(0x26,CMD);    //Gamma curve selected 
	TFT_SendByte(0x01,DATA); 

	TFT_SendByte(0xE0,CMD);    //Set Gamma 
	TFT_SendByte(0x0F,DATA);
	TFT_SendByte(0x26,DATA);
	TFT_SendByte(0x24,DATA);
	TFT_SendByte(0x0B,DATA);
	TFT_SendByte(0x0E,DATA);
	TFT_SendByte(0x09,DATA);
	TFT_SendByte(0x54,DATA);
	TFT_SendByte(0xA8,DATA);
	TFT_SendByte(0x46,DATA);
	TFT_SendByte(0x0C,DATA);
	TFT_SendByte(0x17,DATA);
	TFT_SendByte(0x09,DATA);
	TFT_SendByte(0x0F,DATA);
	TFT_SendByte(0x07,DATA);
	TFT_SendByte(0x00,DATA);

	TFT_SendByte(0XE1,CMD);    //Set Gamma 
	TFT_SendByte(0x00,DATA);
	TFT_SendByte(0x19,DATA);
	TFT_SendByte(0x1B,DATA);
	TFT_SendByte(0x04,DATA);
	TFT_SendByte(0x10,DATA);
	TFT_SendByte(0x07,DATA);
	TFT_SendByte(0x2A,DATA);
	TFT_SendByte(0x47,DATA);
	TFT_SendByte(0x39,DATA);
	TFT_SendByte(0x03,DATA);
	TFT_SendByte(0x06,DATA);
	TFT_SendByte(0x06,DATA);
	TFT_SendByte(0x30,DATA);
	TFT_SendByte(0x38,DATA);
	TFT_SendByte(0x0F,DATA);

	TFT_SendByte(0x11,CMD);    //Exit Sleep 
	DelayMs(120);

	TFT_SendByte(0x29,CMD);    //Display on 
	TFT_SendByte(0x2c,CMD); 
	
	//	LCD_Clear(RED);
}
  */

/**
  * @brief  设置显示模式
  * @param  mode 为LCD_DispMode结构体的值
  * @retval 
 **/
void LCD_DispCtrl(LCD_DispMode mode)
{
	u8 tmp;
	TFT_SendByte(0x36,CMD);
	if (mode&0x08)
	{	
		DispCtrlData=mode;
		/*模式设置*/
		TFT_SendByte(DispCtrlData,DATA);
		
	}
	else
	{
		tmp=(~DispCtrlData)&mode;
		DispCtrlData&=(~mode);
		if (tmp)
			DispCtrlData|=mode;
		TFT_SendByte(DispCtrlData,DATA);
	}
}

/**
  * @brief  bgr TO rgb
  * @param  Xpos:横坐标，Ypos:纵坐标
  * @retval 
 **/
u16 LCD_BGR2RGB(u16 c)
{
	u16  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
}	

/**
  * @brief  设置光标位置
  * @param  Xpos:横坐标，Ypos:纵坐标
  * @retval 
 **/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	
	TFT_SendByte(0X2A,CMD);
	TFT_SendByte(Xpos>>8,DATA);
	TFT_SendByte(Xpos&0XFF,DATA);

	TFT_SendByte(0X2B,CMD); 
	TFT_SendByte(Ypos>>8,DATA);
	TFT_SendByte(Ypos&0XFF,DATA);

}

/**
  * @brief  画点
  * @param  x,y:坐标
  * @retval POINT_COLOR:此点的颜色
 **/
void LCD_DrawPoint(u16 x,u16 y,u16 color)
{
	LCD_OpenWindow(x,y,x,y);
	TFT_SendHalfWord(color);
}

/**
  * @brief  读取点的颜色
  * @param  x,y:坐标
  * @retval POINT_COLOR:此点的颜色
 **/
u16 LCD_ReadPoint(u16 x,u16 y)
{
	u8 r,g,b;
	u16 R,G,B;
	u16 color;

	LCD_SetCursor(x,y);
	//    TFT_CS_LOW();
	TFT_SendByte(0x2E,CMD);  //第一次写入控制命令

	//    TFT_DC_HIGH();
	//    TFT_DC_LOW();  //产生脉冲

	TFT_SendByte(TFT_NOP_CMD,DATA); //第二次空读写DUMMY CLOCK

	r = TFT_SendByte(TFT_NOP_CMD,DATA);//高六位有效 RGB565模式则是高5位
	g = TFT_SendByte(TFT_NOP_CMD,DATA);//高六位有效
	b = TFT_SendByte(TFT_NOP_CMD,DATA);//高六位有效
	//printf("r=%02x, g=%02x, b=%02x  ",r,g,b);

	R = (r>>3)&0x00FF;
	G = (g>>2)&0x00FF;
	B = (b>>3)&0x00FF;
	//printf("R=%02x, G=%02x, B=%02x  ",R,G,B);


	color = 		(R<<8)|(G<<5)|(B>>3);
	//    color = LCD_BGR2RGB(color);
	//printf("color = %04x \n",color);
	return color;
}




/**
  * @brief  开辟一个绘图区域
  * @param  (x,y)起始坐标, xy_len延伸长度 x->(0,240),y->(0,320)
  * @retval 
 **/

void LCD_OpenWindow(u16 x1,u16 y1,u16 x2,u16 y2)
{ 
	TFT_SendByte(0x2a,CMD);
   TFT_SendByte(x1>>8,DATA);
   TFT_SendByte(x1,DATA);
   TFT_SendByte(x2>>8,DATA);
   TFT_SendByte(x2,DATA);
  
   TFT_SendByte(0x2b,CMD);
   TFT_SendByte(y1>>8,DATA);
   TFT_SendByte(y1,DATA);
   TFT_SendByte(y2>>8,DATA);
   TFT_SendByte(y2,DATA);

   TFT_SendByte(0x2C,CMD);					 						 
}


/**
  * @brief  矩形填充
  * @param  (x,y)起始坐标
  * @retval 
 **/
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{
	u16 i,j; 
	LCD_OpenWindow(xsta,ysta,xend,yend);      //设置光标位置 
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)
			TFT_SendHalfWord(color);//设置光标位置 	    
	} 	
}

/**
  * @brief  清屏函数
  * @param  color:要清屏的填充色
  * @retval 
 **/
void LCD_Clear(u16 color)
{
	u16 i,j;  	
	LCD_OpenWindow(0,0,LCD_WIDTH-1,LCD_HEIGHT-1);
	for(i=0;i<LCD_WIDTH;i++)
	{
		for (j=0;j<LCD_HEIGHT;j++)
		{
			TFT_SendHalfWord(color);	 			 
		}

	}
}

/**
  * @brief  显示一个字符
  * @param  
  * @retval 
//num:要显示的字符:" "--->"~"

//mode:叠加方式(1)还是非叠加方式(0)
 **/
void LCD_DispChar(u16 x,u16 y, char num, u8 mode)
{
	u8 temp;
  u8 pos,t;
	u16 x0=x;
	u16 colortemp=POINT_COLOR;      
  if(x>LCD_WIDTH-16||y>LCD_HEIGHT-16)
		return;	    
	//设置窗口		   
	num=num-' ';//得到偏移后的值
	LCD_OpenWindow(x,y,x+8-1,y+16-1);     //设置光标位置 
	if(!mode) //非叠加方式
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp=asc2_1608[(u16)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++)
			{                 
				if(temp&0x01)
					POINT_COLOR=colortemp;
				else 
					POINT_COLOR=BACK_COLOR;
					TFT_SendHalfWord(POINT_COLOR);	
					temp>>=1; 
					x++;
			}
			x=x0;
			y++;
		}	
	}else//叠加方式
	{
		for(pos=0;pos<16;pos++)
		{
		    temp=asc2_1608[(u16)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)
							LCD_DrawPoint(x+t,y+pos,POINT_COLOR);//画一个点     
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	    	   	 	  

}


/**
  * @brief  显示字符串吗，支持自动和手动换行（\n）
  * @param  
  * @retval 混合位置信息
 **/
void LCD_DispStr(u16 x,u16 y, u8* str)
{
	  while(*str!='\0')
    {       
        if(x>LCD_WIDTH-16){x=0;y+=16;}
        if(y>LCD_HEIGHT-16){y=x=0;LCD_Clear(RED);}
        LCD_DispChar(x,y,*str,0);
        x+=8;
        str++;
    }  
}


//m^n函数
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 

/**
  * @brief 右对齐显示整形数字
  * @param x:横像素点，y:竖段，number数据,cnt占符数不包括符号位（默认存在）
  * @retval 
 **/
void LCD_DispInt(u16 x,u16 y,u32 num,u8 len)
{
	u8 t,temp;
	u8 enshow=0;
	num=(u16)num;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_DispChar(x+8*t,y,' ',0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_DispChar(x+8*t,y,temp+48,0); 
	}
}


//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}   

void LCD_DrawHLine( u16 x0, u16 y0, u16 x1 , u16 color )
{
	LCD_DrawLine(x0, y0, x1, y0, color);
}

void LCD_DrawVLine( u16 x0, u16 y0, u16 y1, u16 color )
{
	LCD_DrawLine(x0, y0, x0, y1, color);
}

//画矩形
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}

void LCD_FillRect( u16 x0, u16 y0, u16 x1, u16 y1 , u16 color )
{
    u16 y;
	short temp;       /* 起点 终点大小比较 交换数据时的中间变量 */

#if 0
    u16 x;
    if( x0 > x1 )     /* X轴上起点大于终点 交换数据 */
    {
	    temp = x1;
		x1 = x0;
		x0 = temp;   
    }
	for(x = x0; x < x1; x++)
	{
		LCD_DrawVLine(x, y0, y1, color);
	}
#endif

    if( y0 > y1 )     /* Y轴上起点大于终点 交换数据 */
    {
		temp = y1;
		y1 = y0;
		y0 = temp;   
    }	
	for(y = y0; y < y1; y++)
	{
		LCD_DrawHLine(x0, y, x1, color);
	}	
	
}


//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r,u16 color)
{
	int a,b;
	int di;

	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,color);             //3           
		LCD_DrawPoint(x0+b,y0-a,color);             //0           
		LCD_DrawPoint(x0-a,y0+b,color);             //1       
		LCD_DrawPoint(x0-b,y0-a,color);             //7           
		LCD_DrawPoint(x0-a,y0-b,color);             //2             
		LCD_DrawPoint(x0+b,y0+a,color);             //4               
		LCD_DrawPoint(x0+a,y0-b,color);             //5
		LCD_DrawPoint(x0+a,y0+b,color);             //6 
		LCD_DrawPoint(x0-b,y0+a,color);             
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b,color);
	}
} 

/**
  * @brief 显示图像
  * @param	
  * @retval 
 **/
void LCD_DispImg(u16 X_pos,u16 Y_pos,u16 X_len,u16 Y_len,u16 *pImgData)
{
	u32 data=X_len*Y_len;
	LCD_OpenWindow(X_pos,Y_pos,X_len,Y_len);
	TFT_SendByte(0X2C,CMD);

	while (data--)
	{
		TFT_SendHalfWord(*pImgData);
		pImgData++;
	}
}

void LCD_Disp_CnEn(u16 x, u16 y, u16 fc, u16 bc, u8 *s)
{
	u16 i,j,k;
	u16 HZnum=sizeof(hz16)/sizeof(typFNT_GBK16);

	while(*s) 
	{	
		if((*s) >= 128) 
		{		
			if(x>LCD_WIDTH-16){x=1;y+=20;}
				if(y>LCD_HEIGHT-16){y=x=0;LCD_Clear(RED);}			
			for (k=0;k<HZnum;k++) 
			{
			  if ((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1)))
			  { 	
						LCD_OpenWindow(x,y,x+16-1,y+16-1);
				    for(i=0;i<16*2;i++)
				    {
							for(j=0;j<8;j++)
							{
								if(hz16[k].Msk[i]&(0x80>>j))	
									TFT_SendHalfWord(fc);
								else 
								{
									if (fc!=bc) TFT_SendHalfWord(bc);
								}
							}
					}				
				}
			}
			s+=2;x+=16;
		}else{
			if(x>LCD_WIDTH-16){x=1;y+=20;}
				if(y>LCD_HEIGHT-16){y=x=0;LCD_Clear(RED);}
				LCD_DispChar(x,y,*s,0);
				x+=8;			
				s+=1; 
		}
	}
}

void LCD_Disp_BufCn(u16 x, u16 y,u16 fc, u16 bc, u8 *buf, u16 len)
{
	u16 k, i,j,l;
	u16 HZnum=len/sizeof(typFNT_GBK16); //总长度
	typFNT_GBK16 *defZiKu=(typFNT_GBK16*)buf;  //转为GBK类型
	
	for(k=0;k<HZnum;++k)
	{
		if(x>LCD_WIDTH-16){x=2;y+=20;}
		if(y>LCD_HEIGHT-16){y=x=2;LCD_Clear(RED);}	
		LCD_OpenWindow(x,y,x+16-1,y+16-1);
		for(i=0;i<32;i++)
		{
			for(j=0;j<8;j++)
			{
				if(k>9){
					l=k;
				}
				if(defZiKu[k].Msk[i]&(0x80>>j))	
				{
					TFT_SendHalfWord(fc);
				}
				else 
				{
					if (fc!=bc)  TFT_SendHalfWord(bc);
				}
			}
		}
		x+=16;		
	}
}

void LCD_Show_Image(u16 x,u16 y,u16 x_len,u16 y_len,const u8 *p)
{
	u8 picH,picL;
	u16 i; 
	u32 len=x_len*y_len;
	LCD_OpenWindow(x,y,x+x_len-1,y+y_len-1);		//坐标设置
	for(i=0;i<len;i++)
	{	
		picL=*(p+i*2);	//数据低位在前
		picH=*(p+i*2+1);				
		TFT_SendHalfWord(picH<<8|picL);  						
	}
}

#ifndef __GPS_H
#define __GPS_H	 

#include "sys.h"  

//GPS NMEA-0183协议重要参数结构体定义 

//UBLOX NEO-6M 配置(清除,保存,加载等)结构体
__packed typedef struct  
{										    
 	u16 header;					//cfg header,固定为0X62B5(小端模式)
	u16 id;						//CFG CFG ID:0X0906 (小端模式)
	u16 dlength;				//数据长度 12/13
	u32 clearmask;				//子区域清除掩码(1有效)
	u32 savemask;				//子区域保存掩码
	u32 loadmask;				//子区域加载掩码
	u8  devicemask; 		  	//目标器件选择掩码	b0:BK RAM;b1:FLASH;b2,EEPROM;b4,SPI FLASH
	u8  cka;		 			//校验CK_A 							 	 
	u8  ckb;			 		//校验CK_B							 	 
}_ublox_cfg_cfg; 

//UBLOX NEO-6M 消息设置结构体
__packed typedef struct  
{										    
 	u16 header;					//cfg header,固定为0X62B5(小端模式)
	u16 id;						//CFG MSG ID:0X0106 (小端模式)
	u16 dlength;				//数据长度 8
	u8  msgclass;				//消息类型(F0 代表NMEA消息格式)
	u8  msgid;					//消息 ID 
								//00,GPGGA;01,GPGLL;02,GPGSA;
								//03,GPGSV;04,GPRMC;05,GPVTG;
								//06,GPGRS;07,GPGST;08,GPZDA;
								//09,GPGBS;0A,GPDTM;0D,GPGNS;
	u8  iicset;					//IIC消输出设置    0,关闭;1,使能.
	u8  uart1set;				//UART1输出设置	   0,关闭;1,使能.
	u8  uart2set;				//UART2输出设置	   0,关闭;1,使能.
	u8  usbset;					//USB输出设置	   0,关闭;1,使能.
	u8  spiset;					//SPI输出设置	   0,关闭;1,使能.
	u8  ncset;					//未知输出设置	   默认为1即可.
 	u8  cka;			 		//校验CK_A 							 	 
	u8  ckb;			    	//校验CK_B							 	 
}_ublox_cfg_msg; 

//UBLOX NEO-6M UART端口设置结构体
__packed typedef struct  
{										    
 	u16 header;					//cfg header,固定为0X62B5(小端模式)
	u16 id;						//CFG PRT ID:0X0006 (小端模式)
	u16 dlength;				//数据长度 20
	u8  portid;					//端口号,0=IIC;1=UART1;2=UART2;3=USB;4=SPI;
	u8  reserved;				//保留,设置为0
	u16 txready;				//TX Ready引脚设置,默认为0
	u32 mode;					//串口工作模式设置,奇偶校验,停止位,字节长度等的设置.
 	u32 baudrate;				//波特率设置
 	u16 inprotomask;		 	//输入协议激活屏蔽位  默认设置为0X07 0X00即可.
 	u16 outprotomask;		 	//输出协议激活屏蔽位  默认设置为0X07 0X00即可.
 	u16 reserved4; 				//保留,设置为0
 	u16 reserved5; 				//保留,设置为0 
 	u8  cka;			 		//校验CK_A 							 	 
	u8  ckb;			    	//校验CK_B							 	 
}_ublox_cfg_prt; 

//UBLOX NEO-6M 时钟脉冲配置结构体
__packed typedef struct  
{										    
 	u16 header;					//cfg header,固定为0X62B5(小端模式)
	u16 id;						//CFG TP ID:0X0706 (小端模式)
	u16 dlength;				//数据长度
	u32 interval;				//时钟脉冲间隔,单位为us
	u32 length;				 	//脉冲宽度,单位为us
	signed char status;			//时钟脉冲配置:1,高电平有效;0,关闭;-1,低电平有效.			  
	u8 timeref;			   		//参考时间:0,UTC时间;1,GPS时间;2,当地时间.
	u8 flags;					//时间脉冲设置标志
	u8 reserved;				//保留			  
 	signed short antdelay;	 	//天线延时
 	signed short rfdelay;		//RF延时
	signed int userdelay; 	 	//用户延时	
	u8 cka;						//校验CK_A 							 	 
	u8 ckb;						//校验CK_B							 	 
}_ublox_cfg_tp; 

//UBLOX NEO-6M 刷新速率配置结构体
__packed typedef struct  
{										    
 	u16 header;					//cfg header,固定为0X62B5(小端模式)
	u16 id;						//CFG RATE ID:0X0806 (小端模式)
	u16 dlength;				//数据长度
	u16 measrate;				//测量时间间隔，单位为ms，最少不能小于200ms（5Hz）
	u16 navrate;				//导航速率（周期），固定为1
	u16 timeref;				//参考时间：0=UTC Time；1=GPS Time；
 	u8  cka;					//校验CK_A 							 	 
	u8  ckb;					//校验CK_B							 	 
}_ublox_cfg_rate; 
				 
int NMEA_Str2num(u8 *buf,u8*dx);
void GPS_Analysis(u8 *buf);
void NMEA_GPGSV_Analysis(u8 *buf);
void NMEA_GPGGA_Analysis(u8 *buf);
void NMEA_GPGSA_Analysis(u8 *buf);
void NMEA_GPGSA_Analysis(u8 *buf);
void NMEA_GPRMC_Analysis(u8 *buf);
void NMEA_GPVTG_Analysis(u8 *buf);
u8 Ublox_Cfg_Cfg_Save(void);
u8 Ublox_Cfg_Msg(u8 msgid,u8 uart1set);
u8 Ublox_Cfg_Prt(u32 baudrate);
u8 Ublox_Cfg_Tp(u32 interval,u32 length,signed char status);
u8 Ublox_Cfg_Rate(u16 measrate,u8 reftime);
void Ublox_Send_Date(u8* dbuf,u16 len);

#endif  

 




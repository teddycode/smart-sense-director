#ifndef __USART_H
#define __USART_H

#include "stdio.h"	
#include "sys.h" 
#include "stdarg.h"
#include "stdlib.h"

#define USART2_MAX_REC_LEN  	4096 	//定义最大接收字节数 

#define EN_USART1_RX 			    1		//使能（1）/禁止（0）串口1接收
	  	
#define USART3_MAX_RECV_LEN		600					//最大接收缓存字节数
#define USART3_MAX_SEND_LEN		200					//最大发送缓存字节数			

extern vu16 USART3_RX_STA;   						//接收数据状态

void USART1_Init(u32 baudRate);
void USART2_Init(u32 baudRate);
void USART3_Init(u32 baudRate);
void USART_SendStr(USART_TypeDef *USARTx, char *str, u8 len);
void USART_Printf(USART_TypeDef *USARTx, char *fmt,...);
void Bt_Upload(u8*);


#endif



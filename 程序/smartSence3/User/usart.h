#ifndef __USART_H
#define __USART_H

#include "stdio.h"	
#include "sys.h" 
#include "stdarg.h"
#include "stdlib.h"

#define USART2_MAX_REC_LEN  	4096 	//�����������ֽ��� 

#define EN_USART1_RX 			    1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
#define USART3_MAX_RECV_LEN		600					//�����ջ����ֽ���
#define USART3_MAX_SEND_LEN		200					//����ͻ����ֽ���			

extern vu16 USART3_RX_STA;   						//��������״̬

void USART1_Init(u32 baudRate);
void USART2_Init(u32 baudRate);
void USART3_Init(u32 baudRate);
void USART_SendStr(USART_TypeDef *USARTx, char *str, u8 len);
void USART_Printf(USART_TypeDef *USARTx, char *fmt,...);
void Bt_Upload(u8*);


#endif



#include "sys.h"
#include "usart.h"	
#include "led.h"
#include "delay.h"
#include "timer.h"
#include "string.h"
#include "lcd.h"

//����3������ 	
u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; 	//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.

//����2������

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART2->SR&0X40)==0);//ѭ������,ֱ���������   
    USART2->DR = (u8) ch;      
	return ch;
}
#endif 
  
void USART1_Init(u32 baudRate){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
		
	//ʹ��USART1��GPIOAʱ��	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = baudRate;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}
 
void USART2_Init(u32 baudRate)
{
	GPIO_InitTypeDef GPIO_InitStrue;
	USART_InitTypeDef USART_InitStrue;
	NVIC_InitTypeDef NVIC_InitStrue;
	
	// ����ʹ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	USART_DeInit(USART2);  //��λ����2 -> ����û��
	
	// ��ʼ�� ���ڶ�ӦIO��  TX-PA2  RX-PA3
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_3;
  GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	// ��ʼ�� ����ģʽ״̬
	USART_InitStrue.USART_BaudRate=baudRate; // ������
	USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None; // Ӳ��������
	USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx; // ���� ���� ģʽ��ʹ��
	USART_InitStrue.USART_Parity=USART_Parity_No; // û����żУ��
	USART_InitStrue.USART_StopBits=USART_StopBits_1; // һλֹͣλ
	USART_InitStrue.USART_WordLength=USART_WordLength_8b; // ÿ�η������ݿ��Ϊ8λ
	USART_Init(USART2,&USART_InitStrue);
	
	USART_Cmd(USART2,ENABLE);//ʹ�ܴ���
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//���������ж�
	
	// ��ʼ�� �ж����ȼ�
	NVIC_InitStrue.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStrue);
}


void USART3_Init(u32 baudRate)
{
 
	GPIO_InitTypeDef gpioInitStruct;
	USART_InitTypeDef usartInitStruct;
	NVIC_InitTypeDef nvicInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	//USART1_TX   GPIOB.10
	gpioInitStruct.GPIO_Pin = GPIO_Pin_10; //PB.10
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	gpioInitStruct.GPIO_Mode = GPIO_Mode_AF_PP;			//�����������
	GPIO_Init(GPIOB, &gpioInitStruct);					//��ʼ��GPIOB.10
   
	//USART1_RX	  GPIOB.11��ʼ��
	gpioInitStruct.GPIO_Pin = GPIO_Pin_11;				//PB.11
	gpioInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//��������
	GPIO_Init(GPIOB, &gpioInitStruct);					//��ʼ��GPIOB.11 
	
	usartInitStruct.USART_BaudRate = baudRate;
	usartInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//��Ӳ������
	usartInitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;				//���պͷ���
	usartInitStruct.USART_Parity = USART_Parity_No;						//��У��
	usartInitStruct.USART_StopBits = USART_StopBits_1;					//1λֹͣλ
	usartInitStruct.USART_WordLength = USART_WordLength_8b;					//8λ����λ
	USART_Init(USART3, &usartInitStruct);
	
	USART_Cmd(USART3, ENABLE);														//ʹ�ܴ���
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);						//ʹ�ܽ����ж�
	
	nvicInitStruct.NVIC_IRQChannel = USART3_IRQn;
	nvicInitStruct.NVIC_IRQChannelCmd = ENABLE;
	nvicInitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	nvicInitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&nvicInitStruct);
 
 	
	TIM3_Init(100-1,7200-1); // ��ʱ��3��ʼ��,100ms��ʱ
	USART3_RX_STA=0;		//����
	TIM_Cmd(TIM3, DISABLE); //�رն�ʱ��3
}

/*
************************************************************
*	�������ƣ�	UsartPrintf
*
*	�������ܣ�	��ʽ����ӡ
*
*	��ڲ�����	USARTx��������
*				fmt����������
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void USART_Printf(USART_TypeDef *USARTx, char *fmt,...)
{ 
	unsigned char UsartPrintfBuf[296];
	va_list ap;
	unsigned char *pStr = UsartPrintfBuf;
	
	va_start(ap, fmt);
	vsprintf((char *)UsartPrintfBuf, fmt, ap);				//��ʽ��
	va_end(ap);
	
	while(*pStr != 0)
	{
		USART_SendData(USARTx, *pStr++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}
}


/*
************************************************************
*	�������ƣ�	Usart_SendString
*
*	�������ܣ�	�������ݷ���
*
*	��ڲ�����	USARTx��������
*				str��Ҫ���͵�����
*				len�����ݳ���
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
 
void USART_SendStr(USART_TypeDef *USARTx, char *str, u8 len)
{
 
	unsigned short count = 0;
	
	for(; count < len; count++)
	{
		USART_SendData(USARTx, *str++);						//��������
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//�ȴ��������
	}
}

// �������ڴ����� 
// 0x45������ѡȡ 0x15����������Ϣ��0x35��ͼƬ����
// [0x5A,0x5A],[0x45],[0x00,0x00] [0x01(����1)]
vu8 Receive_area_ok=0;
vu8 USART2_BUF[USART2_MAX_REC_LEN]={0};
vu32 USART2_len=0;
extern u8 cur_page,last_page,cur_sec;
#define PAGE_HOME       0 
#define PAGE_LOCATION   1
#define PAGE_SENCE_1    2
#define PAGE_SENCE_2    3
#define PAGE_SECTOR     4
#define PAGE_DIRECT_1   5
#define PAGE_DIRECT_2   6

void USART2_IRQHandler(void) // ����2�жϷ�����
{
	static vu32 i=0;
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET) // �жϱ�־
	{	
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
		USART2_BUF[i++]=USART_ReceiveData(USART2);//��ȡ�������ݣ�ͬʱ����ձ�־
		if(USART2_BUF[0]!=0x5a)//֡ͷ����
			i=0;	
		if((i==2)&&(USART2_BUF[1]!=0x5a))//֡ͷ����
			i=0;
	
		if(i>4)//i����4ʱ���Ѿ����յ��������ֽ�rebuf[3]|rebuf[4]
		{
			if(i==5) 
			{
				USART2_len=(USART2_BUF[3]<<8|USART2_BUF[4]);  // ȡ����
				if(USART2_len>USART2_MAX_REC_LEN) 
				{ 
					i=0;
					return;
				}  // ���ȳ�����ֵ
			}
			if(i!=USART2_len) 
				return;	//�ж��Ƿ����һ֡�������
			switch(USART2_BUF[2])//������Ϻ���
			{
				case 0x45: //0x45������ѡȡ 
					if(USART2_BUF[5]>=0 && USART2_BUF[5]<5) 
					{
						last_page= PAGE_DIRECT_1;
						cur_page = PAGE_DIRECT_2;
						cur_sec = USART2_BUF[5]+1;
					}
					Receive_area_ok=0;					
					break;
				case 0x15: //0x15��������Ϣ
						last_page= PAGE_HOME;
						Receive_area_ok=1;//������ɱ�־
						cur_page = PAGE_SECTOR;
					break;//ԭʼ���ݽ���
					
				case 0x35:
					// ����ͼƬ,�ֶ�
				  // TFT_SendHalfWord(USART2_BUF[i]<<8);
					break;
			}
			i=0;//������0
			USART2_BUF[0]=USART2_BUF[1]=0;
		}		
	}
}



//ͨ���жϽ�������2���ַ�֮���ʱ������100ms�������ǲ���һ������������.
//���2���ַ����ռ������100ms,����Ϊ����1����������.Ҳ���ǳ���100msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
vu16 USART3_RX_STA=0; 
void USART3_IRQHandler(void)  //����3�жϺ���
{
	u8 res;	    
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//���յ�����
	{	  
		res=USART_ReceiveData(USART3);		
		if((USART3_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
		{ 
			if(USART3_RX_STA<USART3_MAX_RECV_LEN)		//�����Խ�������
			{
				TIM_SetCounter(TIM3,0);//���������        				 
				if(USART3_RX_STA==0) TIM_Cmd(TIM3, ENABLE);  //ʹ�ܶ�ʱ��3 
				USART3_RX_BUF[USART3_RX_STA++]=res;		//��¼���յ���ֵ	 
			}else 
			{
				USART3_RX_STA|=1<<15;					//ǿ�Ʊ�ǽ������
			} 
		}  	
 }										
}

void Bt_Upload(u8* pStr)
{
	while(*pStr != 0)
	{
		USART_SendData(USART2, *pStr++);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	}
}




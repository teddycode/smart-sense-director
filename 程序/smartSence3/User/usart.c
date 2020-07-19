#include "sys.h"
#include "usart.h"	
#include "led.h"
#include "delay.h"
#include "timer.h"
#include "string.h"
#include "lcd.h"

//串口3缓存区 	
u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; 	//发送缓冲,最大USART3_MAX_SEND_LEN字节
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//接收缓冲,最大USART3_MAX_RECV_LEN个字节.

//串口2缓存区

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART2->SR&0X40)==0);//循环发送,直到发送完毕   
    USART2->DR = (u8) ch;      
	return ch;
}
#endif 
  
void USART1_Init(u32 baudRate){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
		
	//使能USART1，GPIOA时钟	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = baudRate;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}
 
void USART2_Init(u32 baudRate)
{
	GPIO_InitTypeDef GPIO_InitStrue;
	USART_InitTypeDef USART_InitStrue;
	NVIC_InitTypeDef NVIC_InitStrue;
	
	// 外设使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	USART_DeInit(USART2);  //复位串口2 -> 可以没有
	
	// 初始化 串口对应IO口  TX-PA2  RX-PA3
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_3;
  GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	// 初始化 串口模式状态
	USART_InitStrue.USART_BaudRate=baudRate; // 波特率
	USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None; // 硬件流控制
	USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx; // 发送 接收 模式都使用
	USART_InitStrue.USART_Parity=USART_Parity_No; // 没有奇偶校验
	USART_InitStrue.USART_StopBits=USART_StopBits_1; // 一位停止位
	USART_InitStrue.USART_WordLength=USART_WordLength_8b; // 每次发送数据宽度为8位
	USART_Init(USART2,&USART_InitStrue);
	
	USART_Cmd(USART2,ENABLE);//使能串口
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//开启接收中断
	
	// 初始化 中断优先级
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
	gpioInitStruct.GPIO_Mode = GPIO_Mode_AF_PP;			//复用推挽输出
	GPIO_Init(GPIOB, &gpioInitStruct);					//初始化GPIOB.10
   
	//USART1_RX	  GPIOB.11初始化
	gpioInitStruct.GPIO_Pin = GPIO_Pin_11;				//PB.11
	gpioInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//浮空输入
	GPIO_Init(GPIOB, &gpioInitStruct);					//初始化GPIOB.11 
	
	usartInitStruct.USART_BaudRate = baudRate;
	usartInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件流控
	usartInitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;				//接收和发送
	usartInitStruct.USART_Parity = USART_Parity_No;						//无校验
	usartInitStruct.USART_StopBits = USART_StopBits_1;					//1位停止位
	usartInitStruct.USART_WordLength = USART_WordLength_8b;					//8位数据位
	USART_Init(USART3, &usartInitStruct);
	
	USART_Cmd(USART3, ENABLE);														//使能串口
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);						//使能接收中断
	
	nvicInitStruct.NVIC_IRQChannel = USART3_IRQn;
	nvicInitStruct.NVIC_IRQChannelCmd = ENABLE;
	nvicInitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	nvicInitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&nvicInitStruct);
 
 	
	TIM3_Init(100-1,7200-1); // 定时器3初始化,100ms定时
	USART3_RX_STA=0;		//清零
	TIM_Cmd(TIM3, DISABLE); //关闭定时器3
}

/*
************************************************************
*	函数名称：	UsartPrintf
*
*	函数功能：	格式化打印
*
*	入口参数：	USARTx：串口组
*				fmt：不定长参
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void USART_Printf(USART_TypeDef *USARTx, char *fmt,...)
{ 
	unsigned char UsartPrintfBuf[296];
	va_list ap;
	unsigned char *pStr = UsartPrintfBuf;
	
	va_start(ap, fmt);
	vsprintf((char *)UsartPrintfBuf, fmt, ap);				//格式化
	va_end(ap);
	
	while(*pStr != 0)
	{
		USART_SendData(USARTx, *pStr++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}
}


/*
************************************************************
*	函数名称：	Usart_SendString
*
*	函数功能：	串口数据发送
*
*	入口参数：	USARTx：串口组
*				str：要发送的数据
*				len：数据长度
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
 
void USART_SendStr(USART_TypeDef *USARTx, char *str, u8 len)
{
 
	unsigned short count = 0;
	
	for(; count < len; count++)
	{
		USART_SendData(USARTx, *str++);						//发送数据
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//等待发送完成
	}
}

// 蓝牙串口处理函数 
// 0x45：景点选取 0x15：景点区信息，0x35：图片推送
// [0x5A,0x5A],[0x45],[0x00,0x00] [0x01(景点1)]
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

void USART2_IRQHandler(void) // 串口2中断服务函数
{
	static vu32 i=0;
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET) // 中断标志
	{	
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
		USART2_BUF[i++]=USART_ReceiveData(USART2);//读取串口数据，同时清接收标志
		if(USART2_BUF[0]!=0x5a)//帧头不对
			i=0;	
		if((i==2)&&(USART2_BUF[1]!=0x5a))//帧头不对
			i=0;
	
		if(i>4)//i等于4时，已经接收到数据量字节rebuf[3]|rebuf[4]
		{
			if(i==5) 
			{
				USART2_len=(USART2_BUF[3]<<8|USART2_BUF[4]);  // 取长度
				if(USART2_len>USART2_MAX_REC_LEN) 
				{ 
					i=0;
					return;
				}  // 长度超过阈值
			}
			if(i!=USART2_len) 
				return;	//判断是否接收一帧数据完毕
			switch(USART2_BUF[2])//接收完毕后处理
			{
				case 0x45: //0x45：景点选取 
					if(USART2_BUF[5]>=0 && USART2_BUF[5]<5) 
					{
						last_page= PAGE_DIRECT_1;
						cur_page = PAGE_DIRECT_2;
						cur_sec = USART2_BUF[5]+1;
					}
					Receive_area_ok=0;					
					break;
				case 0x15: //0x15：景区信息
						last_page= PAGE_HOME;
						Receive_area_ok=1;//接收完成标志
						cur_page = PAGE_SECTOR;
					break;//原始数据接收
					
				case 0x35:
					// 接收图片,分段
				  // TFT_SendHalfWord(USART2_BUF[i]<<8);
					break;
			}
			i=0;//缓存清0
			USART2_BUF[0]=USART2_BUF[1]=0;
		}		
	}
}



//通过判断接收连续2个字符之间的时间差不大于100ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过100ms,则认为不是1次连续数据.也就是超过100ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
vu16 USART3_RX_STA=0; 
void USART3_IRQHandler(void)  //串口3中断函数
{
	u8 res;	    
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//接收到数据
	{	  
		res=USART_ReceiveData(USART3);		
		if((USART3_RX_STA&(1<<15))==0)//接收完的一批数据,还没有被处理,则不再接收其他数据
		{ 
			if(USART3_RX_STA<USART3_MAX_RECV_LEN)		//还可以接收数据
			{
				TIM_SetCounter(TIM3,0);//计数器清空        				 
				if(USART3_RX_STA==0) TIM_Cmd(TIM3, ENABLE);  //使能定时器3 
				USART3_RX_BUF[USART3_RX_STA++]=res;		//记录接收到的值	 
			}else 
			{
				USART3_RX_STA|=1<<15;					//强制标记接收完成
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




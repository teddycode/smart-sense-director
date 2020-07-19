#include "syn6288.h"
#include "usart.h"
#include "string.h"
#include "delay.h"

void SYN_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure; 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA端口时钟	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 		//端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化
}

//Music:选择背景音乐。0:无背景音乐，1~15：选择背景音乐
//选择背景音乐2。(0：无背景音乐  1-15：背景音乐可选)
//v[0~16]:0背景音乐为静音，16背景音乐音量最大
//m[0~16]:0朗读音量为静音，16朗读音量最大
//t[0~5]:0朗读语速最慢，5朗读语速最快
//其他不常用功能请参考数据手册
void SYN_Speaker(u8 Music, u8 *HZdata)
{
  /****************需要发送的文本**********************************/
  u8  Frame_Info[50];
  u8  HZ_Length;
  u8  ecc  = 0;  			//定义校验字节
  u16 i = 0;
  HZ_Length = strlen(HZdata); 			//需要发送文本的长度

  /*****************帧固定配置信息**************************************/
  Frame_Info[0] = 0xFD ; 			//构造帧头FD
  Frame_Info[1] = 0x00 ; 			//构造数据区长度的高字节
  Frame_Info[2] = HZ_Length + 3; 		//构造数据区长度的低字节
  Frame_Info[3] = 0x01 ; 			//构造命令字：合成播放命令
  Frame_Info[4] = 0x01 | Music << 4 ; //构造命令参数：背景音乐设定

  /*******************校验码计算***************************************/
  for(i = 0; i < 5; i++)   				//依次发送构造好的5个帧头字节
  {
    ecc = ecc ^ (Frame_Info[i]);		//对发送的字节进行异或校验
  }

  for(i = 0; i < HZ_Length; i++)   		//依次发送待合成的文本数据
  {
    ecc = ecc ^ (HZdata[i]); 				//对发送的字节进行异或校验
  }
  /*******************发送帧信息***************************************/
  memcpy(&Frame_Info[5], HZdata, HZ_Length);
  Frame_Info[5 + HZ_Length] = ecc;
  USART_SendStr(USART1,Frame_Info, 5 + HZ_Length + 1);
}


/***********************************************************
* 名    称： YS_SYN_Set(u8 *Info_data)
* 功    能： 主函数	程序入口
* 入口参数： *Info_data:固定的配置信息变量
* 出口参数：
* 说    明：本函数用于配置，停止合成、暂停合成等设置 ，默认波特率9600bps。
* 调用方法：通过调用已经定义的相关数组进行配置。
**********************************************************/
void YS_SYN_Set(u8 *Info_data)
{
  u8 Com_Len;
  Com_Len = strlen(Info_data);
  USART_SendStr(USART1,Info_data, Com_Len);
}


#include "key.h"
#include "delay.h"

/****************************************************************************
* Function Name  : KEY_Config
* Description    : 初始化按键是用的IO口
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void KEY_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 开启GPIO时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    /*  配置GPIO的模式和IO口 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_KEY;        //选择你要设置的IO口
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //设置传输速率
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;      //设置输入上拉模式

    /* 初始化GPIO */
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/****************************************************************************
* Function Name  : KEY_Scan
* Description    : 按键扫描，注意该按键扫描支持一次按1个键，不支持同时按多个键
* Input          : None
* Output         : None
* Return         : keyValue：按键的键值
****************************************************************************/
//#define KEY0 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)  // down
//#define KEY1 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)  // left
//#define KEY2 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)  // right
//#define KEY3 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)  // up

u8 KEY_Scan(void)
{
	// 记住上次长按状态，下次等待更长时间
		static u8 Key_Long=0;
    u8 keyValue = KEY_FAIL, timeCount = 0;

    if((KEY0 == 0) || (KEY1 == 0) || (KEY2 == 0) || (KEY3 == 0)) //检测是否有按键按下
    {
			if(Key_Long) DelayMs(200);
			else DelayMs(10);                                      //延时消抖

			/* 检测是哪个按键按下 */
			if(KEY0 == 0)
					keyValue = KEY_DOWN;
			else if(KEY1 == 0)
					keyValue = KEY_LEFT;
			else if(KEY2 == 0)
					keyValue = KEY_RIGHT;
			else if(KEY3 == 0)
					keyValue = KEY_UP;

			if(keyValue == KEY_FAIL)
			{ 
				return keyValue;
			}
					
			/* 有按键按下时，做松手、长按检测，大于1.5s判断为长按*/
			while(((KEY0 == 0) || (KEY1 == 0) || (KEY2 == 0) || (KEY3 == 0)) && (timeCount < 150))
			{
					DelayMs(10); 
					timeCount++;        
			}
			if(timeCount==150) 
			{
				Key_Long=1;
				keyValue|=0x10;
			}else
			{
				Key_Long=0;
			}
//				DelayMs(10); 
//            KEY_Delay10ms();//由于主函数中程序较少，连续扫描的速度太快，加一个松手消抖减少误读   
		 
	}
	return keyValue;
}

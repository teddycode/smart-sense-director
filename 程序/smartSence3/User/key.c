#include "key.h"
#include "delay.h"

/****************************************************************************
* Function Name  : KEY_Config
* Description    : ��ʼ���������õ�IO��
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void KEY_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* ����GPIOʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    /*  ����GPIO��ģʽ��IO�� */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_KEY;        //ѡ����Ҫ���õ�IO��
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //���ô�������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;      //������������ģʽ

    /* ��ʼ��GPIO */
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/****************************************************************************
* Function Name  : KEY_Scan
* Description    : ����ɨ�裬ע��ð���ɨ��֧��һ�ΰ�1��������֧��ͬʱ�������
* Input          : None
* Output         : None
* Return         : keyValue�������ļ�ֵ
****************************************************************************/
//#define KEY0 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)  // down
//#define KEY1 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)  // left
//#define KEY2 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)  // right
//#define KEY3 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)  // up

u8 KEY_Scan(void)
{
	// ��ס�ϴγ���״̬���´εȴ�����ʱ��
		static u8 Key_Long=0;
    u8 keyValue = KEY_FAIL, timeCount = 0;

    if((KEY0 == 0) || (KEY1 == 0) || (KEY2 == 0) || (KEY3 == 0)) //����Ƿ��а�������
    {
			if(Key_Long) DelayMs(200);
			else DelayMs(10);                                      //��ʱ����

			/* ������ĸ��������� */
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
					
			/* �а�������ʱ�������֡�������⣬����1.5s�ж�Ϊ����*/
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
//            KEY_Delay10ms();//�����������г�����٣�����ɨ����ٶ�̫�죬��һ�����������������   
		 
	}
	return keyValue;
}

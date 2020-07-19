#ifndef __KEY_H
#define __KEY_H

#include "sys.h"

//#define USE_EXTI    //ʹ���ⲿ�ж�

/* ����Ҫ���õİ���ʹ�õ�PIN�� */
#define GPIO_Pin_KEY (GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15)

/* ������ȡ */
#define KEY0 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)  // down
#define KEY1 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)  // left
#define KEY2 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)  // right
#define KEY3 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)  // up

/* ���尴���ļ�ֵ */
// �̰�
#define KEY_UP    0x01
#define KEY_DOWN  0x02
#define KEY_LEFT  0x04
#define KEY_RIGHT 0x08
// ����
#define KEY_UP_LONG    0x11
#define KEY_DOWN_LONG  0x12
#define KEY_LEFT_LONG  0x14
#define KEY_RIGHT_LONG 0x18

#define KEY_FAIL  0x00

/* ����ȫ�ֺ��� */
void KEY_Config(void);
uint8_t KEY_Scan(void);
void KEY_NVIC_Config(uint8_t key);

#endif

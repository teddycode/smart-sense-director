#ifndef __SYN6288_H
#define __SYN6288_H

#include "sys.h"

#define SYN_State  PAin(8)

#define BUSY   1 
#define FREE   0

void SYN_Init(void);
void SYN_Speaker(u8 Music, u8 *HZdata);
void YS_SYN_Set(u8 *Info_data);

#endif


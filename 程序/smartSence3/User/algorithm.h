#ifndef __ALG_H
#define __ALG_H

#include "sys.h"


typedef struct tagST_POINT {
    int32_t x;
    int32_t y;
} ST_POINT;
 

u8 PtInPolygon(int32_t lat,int32_t lng);//判断坐标点是否在多边形内


#endif

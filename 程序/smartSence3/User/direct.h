#ifndef __DIRRRECT_H
#define __DIRRRECT_H

#include "sys.h"

#define PI                      3.1415926
#define EARTH_RADIUS            6378.137        //µØÇò½üËÆ°ë¾¶

double Get_Distance(double lat1, double lng1, double lat2, double lng2);
u8 Get_Direction(double lat1, double lng1, double lat2, double lng2);


#endif

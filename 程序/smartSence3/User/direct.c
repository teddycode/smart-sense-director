#include "direct.h"
#include "math.h"


// 求弧度
double rad(double d)
{
    return d * PI /180.0;
}

//计算距离
double Get_Distance(double lat1, double lng1, double lat2, double lng2)
{
	double a,b,s,radLat1,radLat2;
	radLat1 = rad(lat1);
	radLat2 = rad(lat2);
	a = radLat1 - radLat2;
	b = rad(lng1) - rad(lng2);
	s = 2 * asin(sqrt(pow(sin(a/2),2) + cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));
	s = s * EARTH_RADIUS;
	s = s * 1000;
	return s;
}

// 计算方位角
 double get_jiaodu(double lat1, double lng1, double lat2, double lng2)
{
		double w1 = lat1 / 180 * PI;
		double j1 = lng1 / 180 * PI;
		double w2 = lat2 / 180 * PI;
		double j2 = lng2 / 180 * PI;
		double ret,temp;
	
		if (j1 == j2)
		{
				if (w1 > w2) return 270; //北半球的情况，南半球忽略
				else if (w1 < w2) return 90;
				else return -1;//位置完全相同
		}
		ret = 4 * pow(sin((w1 - w2) / 2), 2) - pow(sin((j1 - j2) / 2) * (cos(w1) - cos(w2)), 2);
		ret = sqrt(ret);
		temp = (sin(fabs(j1 - j2) / 2) * (cos(w1) + cos(w2)));
		ret = ret / temp;
		ret = atan(ret) / PI * 180;
		
		if (j1 > j2) // 1为参考点坐标
		{
				if (w1 > w2) ret += 180;
				else ret = 180 - ret;
		}
		
		else if (w1 > w2) ret = 360 - ret;
		return ret;
}
				
u8 Get_Direction(double lat1, double lng1, double lat2, double lng2)
{
		double jiaodu = get_jiaodu(lat1, lng1, lat2, lng2);
		if ((jiaodu <= 10 ) || (jiaodu > 350)) return 0;
		if ((jiaodu > 10) && (jiaodu <= 80)) return 1;
		if ((jiaodu > 80) && (jiaodu <= 100)) return 2;
		if ((jiaodu > 100) && (jiaodu <= 170)) return 3;
		if ((jiaodu > 170) && (jiaodu <= 190)) return 4;
		if ((jiaodu > 190) && (jiaodu <= 260)) return 5;
		if ((jiaodu > 260) && (jiaodu <= 280)) return 6;
		if ((jiaodu > 280) && (jiaodu <= 350)) return 7;
		return 0xFF;

}



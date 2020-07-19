#include "algorithm.h"
#include "math.h"

const ST_POINT defPoints[]=  //景区范围坐标点
{
	{2527700,11031119},  //1
	{2527960,11031480},  //2
	{2528321,11031589},  //3
	{2527818,11032727},  //4
	{2527591,11032763}, //5
	{2527461,11032400},  //6
	{2527252,11032375},  //7
	{2527474,11031793}  //8 纬度、经度
};

u32 min(u32 a, u32 b)
{
	if(a>b)
		return b;
	return a;
}

u32 max(u32 a, u32 b)
{
	if(a>b)
		return a;
	return b;
}


/**
 * 功能：判断点是否在多边形内
 * 方法：求解通过该点的水平线（射线）与多边形各边的交点
 * 结论：单边交点为奇数，成立!
 * 参数：p 指定的某个点
         ptPolygon 多边形的各个顶点坐标（首末点可以不一致） 
         nCount 多边形定点的个数
 * 说明：
 */
u8 PtInPolygon(int32_t lat,int32_t lng) 
{ 
    int nCross = 0, i;
    double x;
    ST_POINT p1, p2;
    
    for (i = 0; i < 8; i++) 
    { 
        p1 = defPoints[i]; 
        p2 = defPoints[(i + 1) % 8];
        // 求解 y=lng与 p1p2 的交点
        if ( p1.y == p2.y ) // p1p2 与 y=p.y平行 
            continue;
        if ( lng< min(p1.y, p2.y) ) // 交点在p1p2延长线上 
            continue; 
        if ( lng>= max(p1.y, p2.y) ) // 交点在p1p2延长线上 
            continue;
        // 求交点的 X 坐标 -------------------------------------------------------------- 
        x = (long)(lng- p1.y) * (long)(p2.x - p1.x) / (long)(p2.y - p1.y) + p1.x;
        if ( x > lat ) 
        {
            nCross++; // 只统计单边交点 
        }
    }
    // 单边交点为偶数，点在多边形之外 --- 
    return (nCross % 2 == 1); 
}
 
// 注意：在有些情况下x值会计算错误，可把double类型改为long类型即可解决。

#include "algorithm.h"
#include "math.h"

const ST_POINT defPoints[]=  //������Χ�����
{
	{2527700,11031119},  //1
	{2527960,11031480},  //2
	{2528321,11031589},  //3
	{2527818,11032727},  //4
	{2527591,11032763}, //5
	{2527461,11032400},  //6
	{2527252,11032375},  //7
	{2527474,11031793}  //8 γ�ȡ�����
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
 * ���ܣ��жϵ��Ƿ��ڶ������
 * ���������ͨ���õ��ˮƽ�ߣ����ߣ������θ��ߵĽ���
 * ���ۣ����߽���Ϊ����������!
 * ������p ָ����ĳ����
         ptPolygon ����εĸ����������꣨��ĩ����Բ�һ�£� 
         nCount ����ζ���ĸ���
 * ˵����
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
        // ��� y=lng�� p1p2 �Ľ���
        if ( p1.y == p2.y ) // p1p2 �� y=p.yƽ�� 
            continue;
        if ( lng< min(p1.y, p2.y) ) // ������p1p2�ӳ����� 
            continue; 
        if ( lng>= max(p1.y, p2.y) ) // ������p1p2�ӳ����� 
            continue;
        // �󽻵�� X ���� -------------------------------------------------------------- 
        x = (long)(lng- p1.y) * (long)(p2.x - p1.x) / (long)(p2.y - p1.y) + p1.x;
        if ( x > lat ) 
        {
            nCross++; // ֻͳ�Ƶ��߽��� 
        }
    }
    // ���߽���Ϊż�������ڶ����֮�� --- 
    return (nCross % 2 == 1); 
}
 
// ע�⣺����Щ�����xֵ�������󣬿ɰ�double���͸�Ϊlong���ͼ��ɽ����

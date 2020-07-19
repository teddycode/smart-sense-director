#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "syn6288.h"
#include "timer.h"
#include "lcd.h"
#include "touch.h"
#include "gps.h"
#include "direct.h"
#include "key.h"
#include "string.h"
#include "algorithm.h"

#include "image.h"

extern u16 POINT_COLOR;
extern u16 BACK_COLOR;

extern vu8 Receive_area_ok;
extern vu8 USART2_BUF[USART2_MAX_REC_LEN];
extern vu32 USART2_len;

typedef struct{
	double latitude; //纬度
	double longitude;	//经度
} Zuobiao;

u8 FMT_BUF[128];   //打印缓存器
u8 SPK_BUF[128];  //声音播报缓存

u8 GPS_BUF[USART3_MAX_RECV_LEN]; 	//GPS数据缓存区
u8* fixmode_tbl[]={"失败","失败","二维","三维"};	//fix mode字符串 
u8* home_dict[]={"景点信息","景区信息","景点位置指引","当前位置","景区自动导游"};
u8* sence_dict[]={"伴月亭","世纪宝鼎","桂林动物园","三将军及八百将士墓","七星欢乐谷"};
u8* direction[]={"东","东北","北","西北","西","西南","南","东南"}; //方位角

const Zuobiao sence_pos[5]={
	{25.278767,110.314439},
	{25.279085,110.316370},
	{25.274474,110.320624},
	{25.279483,110.319150},
	{25.277119,110.317974}
};

// GPS信息
extern u32 m_latitude;				//纬度 分扩大100000倍,实际要除以100000
extern u32 m_longitude;			    //经度 分扩大100000倍,实际要除以100000	
extern u8 m_svnum;					//可见卫星数
extern u8 m_nshemi;					//北纬/南纬,N:北纬;S:南纬				  
extern u8 m_ewhemi;					//东经/西经,E:东经;W:西经
extern u8 m_gpssta;					//GPS状态:0,未定位;1,非差分定位;2,差分定位;6,正在估算.				  
extern u8 m_posslnum;				//用于定位的卫星数,0~12.
extern u8 m_fixmode;					//定位类型:1,没有定位;2,2D定位;3,3D定位	
extern u16 m_pdop;					//位置精度因子 0~500,对应实际值0~50.0
extern u16 m_hdop;					//水平精度因子 0~500,对应实际值0~50.0
extern u16 m_vdop;					//垂直精度因子 0~500,对应实际值0~50.0 
extern int m_altitude;			 	//海拔高度,放大了10倍,实际除以10.单位:0.1m	 
extern u16 m_speed;					//地面速率,放大了1000倍,实际除以10.单位:0.001公里/小时	

extern u16 m_utc_year;	//年份
extern u8 m_utc_month;	//月份
extern u8 m_utc_date;	//日期
extern u8 m_utc_hour; 	//小时
extern u8 m_utc_min; 	//分钟
extern u8 m_utc_sec; 	//秒钟

//触屏信息
extern struct tp_pix_  tp_pixad;

// UI tag
#define PAGE_HOME       0 
#define PAGE_LOCATION   1
#define PAGE_SENCE_1    2
#define PAGE_SENCE_2    3
#define PAGE_SECTOR     4
#define PAGE_DIRECT_1   5
#define PAGE_DIRECT_2   6
#define PAGE_AUTO_DIRECT 7

// UI section
#define SECTION1    1  
#define SECTION2		2  
#define SECTION3    3 
#define SECTION4		4 
#define SECTION5    5  
#define SECTION_OK		6  
#define SECTION_RET		7  

// ui section edge
#define SECTION_NUM    6
#define SECTION_BASE   LCD_HEIGHT/SECTION_NUM
#define EDGE_SEC_1     SECTION_BASE*SECTION1
#define EDGE_SEC_2     SECTION_BASE*SECTION2
#define EDGE_SEC_3     SECTION_BASE*SECTION3
#define EDGE_SEC_4     SECTION_BASE*SECTION4
#define EDGE_SEC_5     SECTION_BASE*SECTION5


u8 Get_Section(void);
void Show_page(u8 page,u8 sec);
void show_home(void);    
void show_location(void);
void show_sence1(void);	
void show_sence2(u8 pos);  
void show_sector(void); 	
void show_direct1(void); 
void show_direct2(u8 pos);
void show_auto_direct(void);
void show_dianzan(u8 pos);

void Key_Event(u8 key);
void SPK_add(u8 *str);
void Welcome(void);
void Scan_key(void);
void Call_help(void);

u8 spk_cur=0,auto_direct=0,update_timer=0;
u8 cur_page=0,last_page=0,cur_sec=1,dianzan_sec=0;

int main(void)
{	
	extern u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; 	//发送缓冲,最大USART3_MAX_SEND_LEN字节
  extern u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 	
	
	u16 i,rxlen;
	
	POINT_COLOR=BLUE;	
	BACK_COLOR=WHITE;
	
	DelayInit();	
	
	USART1_Init(9600);
	USART2_Init(9600);
	USART3_Init(9600);
	
	LED_Init();	
	SYN_Init();	
	TFT_Init();  			//lcd初始化
	KEY_Config();
	LCD_Clear(BACK_COLOR);	
	
	LCD_OpenWindow(0,0,240,320);
	
	//Touch_Init(); 		//触摸屏初始化
	//Touch_Adjust();		//校准

	LCD_DrawRectangle(0,0,239,319,DARKBLUE); //画边框

	Welcome();
	
	Show_page(PAGE_HOME,SECTION1);
		
	while(1)
	{
		// 处理GPS数据
		if(USART3_RX_STA&0X8000)		//接收到一次数据了
		{
			if(cur_page==PAGE_LOCATION || cur_page == PAGE_DIRECT_2 || cur_page==PAGE_AUTO_DIRECT){				
				rxlen=USART3_RX_STA&0X7FFF;	//得到数据长度
				for(i=0;i<rxlen;i++) GPS_BUF[i]=USART3_RX_BUF[i];	   
				GPS_BUF[i]=0;					//自动添加结束符			
				GPS_Analysis((u8*)GPS_BUF);//分析字符串			
			}				
			USART3_RX_STA=0;		  //启动下一次接收
			
			if(cur_page==PAGE_LOCATION)	show_location();
			if(cur_page==PAGE_AUTO_DIRECT && auto_direct) //每10s更新
			{		
				auto_direct=0;
				update_timer=1;
				show_auto_direct();
			}
 		}
		
		if(cur_page!=last_page)   // 界面切换
		{
			Show_page(cur_page,cur_sec);
			last_page=cur_page;
		}
		
		// 按键事件处理
		Key_Event(KEY_Scan());
		
		/*
		if(Touch_State()==0)  // 触屏被按下
		{
			sec=Get_Section();
			switch(sec)
			{
				case SECTION1:
					if(cur_page==PAGE_HOME) cur_page=PAGE_SENCE_1;
					else if(cur_page==PAGE_SENCE_1) cur_page=PAGE_SENCE_2;
					else if(cur_page==PAGE_DIRECT_1) cur_page=PAGE_DIRECT_2;
						break;
				
				case SECTION2:
					if(cur_page==PAGE_HOME) cur_page=PAGE_SECTOR;
					else if(cur_page==PAGE_SENCE_1) cur_page=PAGE_SENCE_2;
					else if(cur_page==PAGE_DIRECT_1) cur_page=PAGE_DIRECT_2;
						break;
				
				case SECTION3:
					if(cur_page==PAGE_HOME) cur_page=PAGE_DIRECT_1;
					else if(cur_page==PAGE_SENCE_1) cur_page=PAGE_SENCE_2;
					else if(cur_page==PAGE_DIRECT_1) cur_page=PAGE_DIRECT_2;
					break;
				
				case SECTION4:				
					if(cur_page==PAGE_SENCE_1) cur_page=PAGE_SENCE_2;
					else if(cur_page==PAGE_DIRECT_1) cur_page=PAGE_DIRECT_2;
					break;
				
				case SECTION5:				
					if(cur_page==PAGE_SENCE_1) cur_page=PAGE_SENCE_2;
					else if(cur_page==PAGE_DIRECT_1) cur_page=PAGE_DIRECT_2;
					break;
				
				case SECTION_OK:				
					if(cur_page==PAGE_HOME) cur_page=PAGE_DIRECT_1;
					else if(cur_page==PAGE_SENCE_1) cur_page=PAGE_SENCE_2;
					else if(cur_page==PAGE_DIRECT_1) cur_page=PAGE_DIRECT_2;
					else if(cur_page==PAGE_SENCE_2) SYN_Speaker(0,AREA_BUF);
					break;
				
				case SECTION_RET:				
					if(cur_page==PAGE_SENCE_1) cur_page=PAGE_HOME;
					else if(cur_page==PAGE_SENCE_2) cur_page=PAGE_SENCE_1;
					else if(cur_page==PAGE_DIRECT_1) cur_page=PAGE_HOME;
					else if(cur_page==PAGE_DIRECT_2) cur_page=PAGE_DIRECT_1;
					else if(cur_page==PAGE_SECTOR) cur_page=PAGE_HOME;
					break;					
			}
		}
		*/
	}
}

// 显示欢迎
void Welcome(void)
{
	u8 cnt=0,state=0;
	u16 i,rxlen;
	extern u8 USART3_RX_BUF[USART3_MAX_RECV_LEN];
	
	SYN_Speaker(0,"[v10][m10][t5]");	
	DelayS(1);
	SYN_Speaker(0,"欢迎使用智能景区导游系统，正在定位中");
//	DelayS(1);
	LCD_Disp_CnEn(24,80,POINT_COLOR,BACK_COLOR,"欢迎使用智能景区导游系统");
	LCD_Disp_CnEn(40,100,POINT_COLOR,BACK_COLOR,"正在确定您的位置...");
	
	while(1){
		if(cnt>100)	break;	 
		if(USART3_RX_STA&0X8000)		//接收到一次数据了
		{						
			rxlen=USART3_RX_STA&0X7FFF;	//得到数据长度
			for(i=0;i<rxlen;i++)
				GPS_BUF[i]=USART3_RX_BUF[i];	   
			GPS_BUF[i]=0;					//自动添加结束符			
			GPS_Analysis((u8*)GPS_BUF);//分析字符串						
			USART3_RX_STA=0;		  //启动下一次接收				
			cnt++;
			
			if(m_gpssta==1 || m_gpssta==2)
			{
				state=PtInPolygon(m_latitude,m_longitude);
				break;				
			}			
		}
	}
	
	i=0;
	while(SYN_State==BUSY)  // 等待至多两秒钟
	{
		if(i++>10) break;
		DelayMs(300);		
	}
		
	if(cnt>10){
		SYN_Speaker(0,"定位失败，请在室外定位！");
		LCD_Disp_CnEn(90,120,BLUE,BACK_COLOR,"定位失败");	
	}else if(state == 1){
		SYN_Speaker(0,"定位成功！欢迎来到桂林七星公园，祝您游玩愉快");
		LCD_Disp_CnEn(10,120,POINT_COLOR,BACK_COLOR,"欢迎来到七星公园，祝您游玩愉快");	
	}else{
		SYN_Speaker(0,"定位成功！您当前不在景区内！");
		LCD_Disp_CnEn(64,120,POINT_COLOR,BACK_COLOR,"您不在景区内！");	
	}
		
	DelayS(2);
}


void Key_Event(u8 key)
{	
		if(key!=KEY_FAIL)
		{
			switch(key)
			{
				case KEY_DOWN: 
					if(cur_sec < (SECTION_NUM-1) && (cur_page==PAGE_HOME||cur_page==PAGE_SENCE_1||cur_page==PAGE_DIRECT_1))
					{
						LCD_Fill(5,SECTION_BASE*cur_sec-40,35,SECTION_BASE*cur_sec-10,BACK_COLOR);
						//LCD_DispStr(5,SECTION_BASE*cur_sec-35,"  ");
						cur_sec++;	
						//LCD_DispStr(5,SECTION_BASE*cur_sec-35,"->");	
						LCD_Show_Image(5,SECTION_BASE*cur_sec-40,30,30,gImage_jiantou); //显示箭头						
					}
					break;
					
				case KEY_UP: 
					if(cur_sec > 1 && (cur_page==PAGE_HOME||cur_page==PAGE_SENCE_1||cur_page==PAGE_DIRECT_1))
					{
						//LCD_DispStr(5,SECTION_BASE*cur_sec-35,"  ");
						LCD_Fill(5,SECTION_BASE*cur_sec-40,35,SECTION_BASE*cur_sec-10,BACK_COLOR);
						cur_sec--;	
						LCD_Show_Image(5,SECTION_BASE*cur_sec-40,30,30,gImage_jiantou); //显示箭头
						//LCD_DispStr(5,SECTION_BASE*cur_sec-35,"->");						
					}
					break;
				case KEY_LEFT: 	// 确定
					if(cur_page==PAGE_HOME) {
						if(cur_sec==SECTION1)cur_page=PAGE_SENCE_1;
						else if(cur_sec==SECTION2) {cur_page=PAGE_SECTOR; cur_sec=SECTION1;}
						else if(cur_sec==SECTION3) {cur_page=PAGE_DIRECT_1; cur_sec=SECTION1;}
						else if(cur_sec==SECTION4) {cur_page=PAGE_LOCATION; cur_sec=SECTION1;}
						else if(cur_sec==SECTION5) {cur_page=PAGE_AUTO_DIRECT; cur_sec=SECTION1;} // 自动导游
					}
					else if(cur_page==PAGE_SENCE_1) cur_page=PAGE_SENCE_2;
					else if(cur_page==PAGE_DIRECT_1) cur_page=PAGE_DIRECT_2;
					else if(cur_page==PAGE_SENCE_2)	 {SPK_BUF[spk_cur]=0;SYN_Speaker(0,SPK_BUF);spk_cur=0;}
					else if(cur_page==PAGE_DIRECT_2) {SPK_BUF[spk_cur]=0;SYN_Speaker(0,SPK_BUF);spk_cur=0;}
					break;
				case KEY_RIGHT: // 返回
					if(cur_page==PAGE_SENCE_1 ||cur_page==PAGE_LOCATION||cur_page==PAGE_DIRECT_1||cur_page==PAGE_SECTOR||cur_page==PAGE_AUTO_DIRECT) 
					{cur_page=PAGE_HOME; update_timer=0;}
					else if(cur_page==PAGE_SENCE_2) cur_page=PAGE_SENCE_1;
					else if(cur_page==PAGE_DIRECT_2) cur_page=PAGE_DIRECT_1;
					break;
					
				case KEY_LEFT_LONG: // 长按确定 点赞
					if(cur_page == PAGE_SENCE_1 || cur_page == PAGE_DIRECT_1)	{ show_dianzan(cur_sec-1);}
					break;
				case KEY_RIGHT_LONG: 
					Call_help();
					break;// 长按取消	报警				
					
				default:					
					break;
			}
		}
}
		
// 上传报警消息
void Call_help()
{
	sprintf((char*)FMT_BUF,"*1lat:%f,lng:%f#",m_latitude/100000.0,m_longitude/100000.0);
	//sprintf((char*)FMT_BUF,"*1lat:%f,lng:%f#",sence_pos[0].latitude,sence_pos[0].longitude);
	Bt_Upload(FMT_BUF);
	DelayMs(100);
}

// 上传点赞消息
void show_dianzan(u8 sec)
{		
	LCD_Fill(200,SECTION_BASE*dianzan_sec-40,230,SECTION_BASE*dianzan_sec-10,BACK_COLOR);
	dianzan_sec=sec+1;
	LCD_Show_Image(200,SECTION_BASE*dianzan_sec-40,30,30,gImage_dianzan);
	sprintf((char*)FMT_BUF,"*2%d#",sec);
	Bt_Upload(FMT_BUF);
	DelayMs(100);
}

u8 Get_Section(void)
{
	Touch_Read_Pos();
	if(tp_pixad.y<EDGE_SEC_1)
		return SECTION1;
	else if(tp_pixad.y<EDGE_SEC_2)
		return SECTION2;
	else if(tp_pixad.y<EDGE_SEC_3)
		return SECTION3;
	else if(tp_pixad.y<EDGE_SEC_4)
		return SECTION4;
	else if(tp_pixad.y<EDGE_SEC_5)
		return SECTION5;
	else if(tp_pixad.x<120)
		return SECTION_OK;
	else
		return SECTION_RET;
}

// 显示UI
void Show_page(u8 page,u8 sec)
{
	LCD_OpenWindow(1,1,286,319);
	LCD_Fill(1,1,238,286,BACK_COLOR);
	
	if(page == PAGE_SENCE_2 || page==PAGE_DIRECT_2) 
		LCD_Disp_CnEn(44,300,POINT_COLOR,BACK_COLOR,"语音");
	else
		LCD_Disp_CnEn(44,300,POINT_COLOR,BACK_COLOR,"确定");
	
	LCD_Disp_CnEn(160,300,POINT_COLOR,BACK_COLOR,"返回");
	
	LCD_DrawHLine(1,286,239,POINT_COLOR);
	LCD_DrawVLine(120,286,319,POINT_COLOR);
	
	switch(page)
	{
		case PAGE_HOME      :	show_home();    	break;
		case PAGE_LOCATION  :	show_location();	break;
		case PAGE_SENCE_1   : show_sence1();	  break;
		case PAGE_SENCE_2   : show_sence2(sec-1);    break;
		case PAGE_SECTOR    : show_sector(); 	  break;
		case PAGE_DIRECT_1  : show_direct1();   break;
		case PAGE_DIRECT_2  : show_direct2(sec-1); 	break;
		case PAGE_AUTO_DIRECT : show_auto_direct(); update_timer=1; break;
		
	}
	
}

//显示主页
void show_home()
{	
	u8 i;
	for(i=0;i<5;++i)
	{
		LCD_Disp_CnEn(40,(SECTION_BASE*i)+SECTION_BASE/3,POINT_COLOR,BACK_COLOR,home_dict[i]);
		LCD_DrawHLine(1,(SECTION_BASE*(i+1)),238,POINT_COLOR);			
	}			
//	LCD_DispStr(5,SECTION_BASE*cur_sec-35,"->");		
	LCD_Show_Image(5,SECTION_BASE*cur_sec-40,30,30,gImage_jiantou); //显示箭头
}
   
//显示GPS定位信息 
void show_location()
{	
	switch(m_gpssta)//GPS状态:0,未定位;1,非差分定位;2,差分定位;6,正在估算.	
	{
		case 0: 
			sprintf((char*)FMT_BUF,"定位状态:未定位");	//定位状态
			break;
		case 1: 
			sprintf((char*)FMT_BUF,"定位状态:非差分定位");	//定位状态
			break;
		case 2: 
			sprintf((char*)FMT_BUF,"定位状态:差分定位");	//定位状态
			break;
		case 6: 
			sprintf((char*)FMT_BUF,"定位状态:正在估算");	//定位状态
			break;			
	}
 	LCD_Disp_CnEn(30,EDGE_SEC_1-20,POINT_COLOR,BACK_COLOR,FMT_BUF);	 	 
	
	sprintf((char *)FMT_BUF,"经度:%.5f %1c ",m_longitude/100000.0,m_ewhemi);	//得到经度字符串
 	LCD_Disp_CnEn(30,EDGE_SEC_1,POINT_COLOR,BACK_COLOR,FMT_BUF);	 	   
  
	sprintf((char *)FMT_BUF,"纬度:%.5f %1c ",m_latitude/100000.0,m_nshemi);	//得到纬度字符串
 	LCD_Disp_CnEn(30,EDGE_SEC_1+20,POINT_COLOR,BACK_COLOR,FMT_BUF);	 
	  
 	sprintf((char *)FMT_BUF,"高度:%.1fm   ",m_altitude/10.0);	    			//得到高度字符串
 	LCD_Disp_CnEn(30,EDGE_SEC_1+40,POINT_COLOR,BACK_COLOR,FMT_BUF);	 	
	
 	sprintf((char *)FMT_BUF,"速度:%.3fkm/h ",m_speed/1000.0);		    		//得到速度字符串	 
 	LCD_Disp_CnEn(30,EDGE_SEC_1+60,POINT_COLOR,BACK_COLOR,FMT_BUF);	 	
	if(m_fixmode<=3)														//定位状态
	{  
		sprintf((char *)FMT_BUF,"定位模式:%s",fixmode_tbl[m_fixmode]);	
		LCD_Disp_CnEn(30,EDGE_SEC_1+80,POINT_COLOR,BACK_COLOR,FMT_BUF);			   
	}	 	   
	sprintf((char *)FMT_BUF,"定位卫星数:%02d",m_posslnum);	 		//用于定位的卫星数
 	LCD_Disp_CnEn(30,EDGE_SEC_1+100,POINT_COLOR,BACK_COLOR,FMT_BUF);	    
	
	sprintf((char *)FMT_BUF,"可见卫星数:%02d",m_svnum%100);	 		//可见卫星数
 	LCD_Disp_CnEn(30,EDGE_SEC_1+120,POINT_COLOR,BACK_COLOR,FMT_BUF);		 
	
	sprintf((char *)FMT_BUF,"UTC日期:%04d/%02d/%02d ",m_utc_year,m_utc_month,m_utc_date);	//显示UTC日期	
	LCD_Disp_CnEn(30,EDGE_SEC_1+140,POINT_COLOR,BACK_COLOR,FMT_BUF);		    
	
	sprintf((char *)FMT_BUF,"UTC时间:%02d:%02d:%02d ",m_utc_hour,m_utc_min,m_utc_sec);	//显示UTC时间
  LCD_Disp_CnEn(30,EDGE_SEC_1+160,POINT_COLOR,BACK_COLOR,FMT_BUF);
	
}


// 显示景区信息
void show_sence1()
{
	u8 i;
	for(i=0;i<5;++i)
	{
		LCD_Disp_CnEn(40,(SECTION_BASE*i)+SECTION_BASE/3,POINT_COLOR,BACK_COLOR,sence_dict[i]);
		LCD_DrawHLine(1,(SECTION_BASE*(i+1)),238,POINT_COLOR);		
	}
	
	LCD_Show_Image(5,SECTION_BASE*cur_sec-40,30,30,gImage_jiantou); //显示箭头
	//LCD_DispStr(5,SECTION_BASE*cur_sec-35,"->");	  
	// 显示已点赞
	LCD_Show_Image(200,SECTION_BASE*dianzan_sec-40,30,30,gImage_dianzan);
		
}

// 显示景点详情
void show_sence2(u8 pos)
{
	spk_cur=0; //语音重新开始
	
	LCD_Disp_CnEn((LCD_WIDTH-strlen(sence_dict[pos])*8)/2,EDGE_SEC_3+SECTION_BASE/3,POINT_COLOR,BACK_COLOR,sence_dict[pos]);
	SPK_add(sence_dict[pos]);
	
	sprintf((char *)FMT_BUF,"位置经度: %f",sence_pos[pos].longitude);
	LCD_Disp_CnEn(30,EDGE_SEC_4,POINT_COLOR,BACK_COLOR,FMT_BUF);
	SPK_add(FMT_BUF);
	
	sprintf((char *)FMT_BUF,"位置纬度: %f",sence_pos[pos].latitude);
	LCD_Disp_CnEn(30,EDGE_SEC_4+20,POINT_COLOR,BACK_COLOR,FMT_BUF);
	SPK_add(FMT_BUF);
}  

// 显示景区消息
void show_sector()
{
	LCD_Disp_CnEn(80,SECTION_BASE/3,POINT_COLOR,BACK_COLOR,"消息");
	if(Receive_area_ok)
	{
		LCD_Disp_BufCn(20,EDGE_SEC_1+SECTION_BASE/3,POINT_COLOR,BACK_COLOR,USART2_BUF+5,USART2_len-5);	
	}else
	{
		LCD_Disp_CnEn(80,EDGE_SEC_1+SECTION_BASE/3,POINT_COLOR,BACK_COLOR,"No Data!");	
	}	
}

// 显示导航
void show_direct1()
{
	show_sence1();
}	

// 显示导航
void show_direct2(u8 pos)
{
	u8 dir;
	double dis;
	
	spk_cur=0; //语音重新开始
	
	dir=Get_Direction(m_latitude/100000.0,m_longitude/100000.0,sence_pos[pos].latitude,sence_pos[pos].longitude);
	dis=Get_Distance(m_latitude/100000.0,m_longitude/100000.0,sence_pos[pos].latitude,sence_pos[pos].longitude);
	
	sprintf((char *)FMT_BUF,"%s在当前位置的%s方向,距离大约%.2fm",sence_dict[pos],direction[dir],dis);
	LCD_Disp_CnEn(20,EDGE_SEC_4,POINT_COLOR,BACK_COLOR,FMT_BUF);
	SPK_add(FMT_BUF);
	
//sprintf((char *)FMT_BUF,"",dis);
	//LCD_Disp_CnEn(40,EDGE_SEC_4+20,POINT_COLOR,BACK_COLOR,FMT_BUF);
	//SPK_add(FMT_BUF);
	
}

// 自动导游景区
void show_auto_direct(void)
{
	u8 i,pos,dir;
	u16 min_dis=65534;
	double dis;
	// 显示标题
	LCD_Disp_CnEn(60,40,POINT_COLOR,BACK_COLOR,"正在游览七星公园");
	// 获取最近的景点
	for(i=0;i<5;++i)
	{
		dis=Get_Distance(m_latitude/100000.0,m_longitude/100000.0,sence_pos[i].latitude,sence_pos[i].longitude);
		if(dis<min_dis)
		{
			min_dis = dis;
			pos = i;
		}
	}
	// 判断是否在一定值内	
	if(min_dis>1000)
	{
		LCD_Disp_CnEn(65,80,POINT_COLOR,BACK_COLOR,"距离景点太远");
		SYN_Speaker(0,"当前所在位置距离景点超过1千米");
	}else{
		//  获取方位
		dir=Get_Direction(m_latitude/100000.0,m_longitude/100000.0,sence_pos[pos].latitude,sence_pos[pos].longitude);
		sprintf((char*)FMT_BUF,"前方景点为:%s,位于%s方向 ",sence_dict[pos],direction[dir]);
		LCD_Disp_CnEn(10,80,POINT_COLOR,BACK_COLOR,FMT_BUF);
		SYN_Speaker(0,FMT_BUF);					
	}
	
	if(m_gpssta==1 || m_gpssta== 2 )
		LCD_Disp_CnEn(60,120,POINT_COLOR,BACK_COLOR,"定位状态:成功");
	else
		LCD_Disp_CnEn(60,120,POINT_COLOR,BACK_COLOR,"定位状态:失败");
	
	sprintf(FMT_BUF,"移动速度:%.3fkm/h",m_speed/1000.0);
	LCD_Disp_CnEn(60,140,POINT_COLOR,BACK_COLOR,FMT_BUF);
	
	sprintf((char *)FMT_BUF,"当前时间:%02d:%02d:%02d ",m_utc_hour+8,m_utc_min,m_utc_sec);	//显示GMT时间
	LCD_Disp_CnEn(60,160,POINT_COLOR,BACK_COLOR,FMT_BUF);
	
}

// 添加语音消息
void SPK_add(u8 *str)
{
	u8 i=0,len=0;
	len=strlen(str);
	if(len>126) return;
	while(i<len){if(spk_cur>126)break; SPK_BUF[spk_cur++]=FMT_BUF[i++];}
}


//测试函数
u8 jiance() //检测触摸和按键
{
	if (Touch_State()==0) //如果触摸按下，则进入绘图程序
	 {
	 	LCD_Clear(WHITE); //清屏
		BACK_COLOR=WHITE;
		POINT_COLOR=RED;
	 	point();
		return 1;
	 }	

	   return 0;
}

void Scan_key()
{
	u8 key_s;
	key_s = KEY_Scan();
	switch(key_s)
	{
		case KEY_UP: 
			LCD_DispStr(50,200,"up    ");break;
		case KEY_DOWN: 
			LCD_DispStr(50,200,"down   ");break;
		case KEY_LEFT:
			LCD_DispStr(50,200,"left   ");break;
		case KEY_RIGHT:
			LCD_DispStr(50,200,"right  ");break;

		case KEY_UP_LONG: 
			LCD_DispStr(50,200,"up_l  ");break;
		case KEY_DOWN_LONG: 
			LCD_DispStr(50,200,"down_1");break;
		case KEY_LEFT_LONG:
			LCD_DispStr(50,200,"left_1");break;
		case KEY_RIGHT_LONG:
			LCD_DispStr(50,200,"right_1");break;
		default:
			break;
	}
}


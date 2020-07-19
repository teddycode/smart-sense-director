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
	double latitude; //γ��
	double longitude;	//����
} Zuobiao;

u8 FMT_BUF[128];   //��ӡ������
u8 SPK_BUF[128];  //������������

u8 GPS_BUF[USART3_MAX_RECV_LEN]; 	//GPS���ݻ�����
u8* fixmode_tbl[]={"ʧ��","ʧ��","��ά","��ά"};	//fix mode�ַ��� 
u8* home_dict[]={"������Ϣ","������Ϣ","����λ��ָ��","��ǰλ��","�����Զ�����"};
u8* sence_dict[]={"����ͤ","���ͱ���","���ֶ���԰","���������˰ٽ�ʿĹ","���ǻ��ֹ�"};
u8* direction[]={"��","����","��","����","��","����","��","����"}; //��λ��

const Zuobiao sence_pos[5]={
	{25.278767,110.314439},
	{25.279085,110.316370},
	{25.274474,110.320624},
	{25.279483,110.319150},
	{25.277119,110.317974}
};

// GPS��Ϣ
extern u32 m_latitude;				//γ�� ������100000��,ʵ��Ҫ����100000
extern u32 m_longitude;			    //���� ������100000��,ʵ��Ҫ����100000	
extern u8 m_svnum;					//�ɼ�������
extern u8 m_nshemi;					//��γ/��γ,N:��γ;S:��γ				  
extern u8 m_ewhemi;					//����/����,E:����;W:����
extern u8 m_gpssta;					//GPS״̬:0,δ��λ;1,�ǲ�ֶ�λ;2,��ֶ�λ;6,���ڹ���.				  
extern u8 m_posslnum;				//���ڶ�λ��������,0~12.
extern u8 m_fixmode;					//��λ����:1,û�ж�λ;2,2D��λ;3,3D��λ	
extern u16 m_pdop;					//λ�þ������� 0~500,��Ӧʵ��ֵ0~50.0
extern u16 m_hdop;					//ˮƽ�������� 0~500,��Ӧʵ��ֵ0~50.0
extern u16 m_vdop;					//��ֱ�������� 0~500,��Ӧʵ��ֵ0~50.0 
extern int m_altitude;			 	//���θ߶�,�Ŵ���10��,ʵ�ʳ���10.��λ:0.1m	 
extern u16 m_speed;					//��������,�Ŵ���1000��,ʵ�ʳ���10.��λ:0.001����/Сʱ	

extern u16 m_utc_year;	//���
extern u8 m_utc_month;	//�·�
extern u8 m_utc_date;	//����
extern u8 m_utc_hour; 	//Сʱ
extern u8 m_utc_min; 	//����
extern u8 m_utc_sec; 	//����

//������Ϣ
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
	extern u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; 	//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
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
	TFT_Init();  			//lcd��ʼ��
	KEY_Config();
	LCD_Clear(BACK_COLOR);	
	
	LCD_OpenWindow(0,0,240,320);
	
	//Touch_Init(); 		//��������ʼ��
	//Touch_Adjust();		//У׼

	LCD_DrawRectangle(0,0,239,319,DARKBLUE); //���߿�

	Welcome();
	
	Show_page(PAGE_HOME,SECTION1);
		
	while(1)
	{
		// ����GPS����
		if(USART3_RX_STA&0X8000)		//���յ�һ��������
		{
			if(cur_page==PAGE_LOCATION || cur_page == PAGE_DIRECT_2 || cur_page==PAGE_AUTO_DIRECT){				
				rxlen=USART3_RX_STA&0X7FFF;	//�õ����ݳ���
				for(i=0;i<rxlen;i++) GPS_BUF[i]=USART3_RX_BUF[i];	   
				GPS_BUF[i]=0;					//�Զ���ӽ�����			
				GPS_Analysis((u8*)GPS_BUF);//�����ַ���			
			}				
			USART3_RX_STA=0;		  //������һ�ν���
			
			if(cur_page==PAGE_LOCATION)	show_location();
			if(cur_page==PAGE_AUTO_DIRECT && auto_direct) //ÿ10s����
			{		
				auto_direct=0;
				update_timer=1;
				show_auto_direct();
			}
 		}
		
		if(cur_page!=last_page)   // �����л�
		{
			Show_page(cur_page,cur_sec);
			last_page=cur_page;
		}
		
		// �����¼�����
		Key_Event(KEY_Scan());
		
		/*
		if(Touch_State()==0)  // ����������
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

// ��ʾ��ӭ
void Welcome(void)
{
	u8 cnt=0,state=0;
	u16 i,rxlen;
	extern u8 USART3_RX_BUF[USART3_MAX_RECV_LEN];
	
	SYN_Speaker(0,"[v10][m10][t5]");	
	DelayS(1);
	SYN_Speaker(0,"��ӭʹ�����ܾ�������ϵͳ�����ڶ�λ��");
//	DelayS(1);
	LCD_Disp_CnEn(24,80,POINT_COLOR,BACK_COLOR,"��ӭʹ�����ܾ�������ϵͳ");
	LCD_Disp_CnEn(40,100,POINT_COLOR,BACK_COLOR,"����ȷ������λ��...");
	
	while(1){
		if(cnt>100)	break;	 
		if(USART3_RX_STA&0X8000)		//���յ�һ��������
		{						
			rxlen=USART3_RX_STA&0X7FFF;	//�õ����ݳ���
			for(i=0;i<rxlen;i++)
				GPS_BUF[i]=USART3_RX_BUF[i];	   
			GPS_BUF[i]=0;					//�Զ���ӽ�����			
			GPS_Analysis((u8*)GPS_BUF);//�����ַ���						
			USART3_RX_STA=0;		  //������һ�ν���				
			cnt++;
			
			if(m_gpssta==1 || m_gpssta==2)
			{
				state=PtInPolygon(m_latitude,m_longitude);
				break;				
			}			
		}
	}
	
	i=0;
	while(SYN_State==BUSY)  // �ȴ�����������
	{
		if(i++>10) break;
		DelayMs(300);		
	}
		
	if(cnt>10){
		SYN_Speaker(0,"��λʧ�ܣ��������ⶨλ��");
		LCD_Disp_CnEn(90,120,BLUE,BACK_COLOR,"��λʧ��");	
	}else if(state == 1){
		SYN_Speaker(0,"��λ�ɹ�����ӭ�����������ǹ�԰��ף���������");
		LCD_Disp_CnEn(10,120,POINT_COLOR,BACK_COLOR,"��ӭ�������ǹ�԰��ף���������");	
	}else{
		SYN_Speaker(0,"��λ�ɹ�������ǰ���ھ����ڣ�");
		LCD_Disp_CnEn(64,120,POINT_COLOR,BACK_COLOR,"�����ھ����ڣ�");	
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
						LCD_Show_Image(5,SECTION_BASE*cur_sec-40,30,30,gImage_jiantou); //��ʾ��ͷ						
					}
					break;
					
				case KEY_UP: 
					if(cur_sec > 1 && (cur_page==PAGE_HOME||cur_page==PAGE_SENCE_1||cur_page==PAGE_DIRECT_1))
					{
						//LCD_DispStr(5,SECTION_BASE*cur_sec-35,"  ");
						LCD_Fill(5,SECTION_BASE*cur_sec-40,35,SECTION_BASE*cur_sec-10,BACK_COLOR);
						cur_sec--;	
						LCD_Show_Image(5,SECTION_BASE*cur_sec-40,30,30,gImage_jiantou); //��ʾ��ͷ
						//LCD_DispStr(5,SECTION_BASE*cur_sec-35,"->");						
					}
					break;
				case KEY_LEFT: 	// ȷ��
					if(cur_page==PAGE_HOME) {
						if(cur_sec==SECTION1)cur_page=PAGE_SENCE_1;
						else if(cur_sec==SECTION2) {cur_page=PAGE_SECTOR; cur_sec=SECTION1;}
						else if(cur_sec==SECTION3) {cur_page=PAGE_DIRECT_1; cur_sec=SECTION1;}
						else if(cur_sec==SECTION4) {cur_page=PAGE_LOCATION; cur_sec=SECTION1;}
						else if(cur_sec==SECTION5) {cur_page=PAGE_AUTO_DIRECT; cur_sec=SECTION1;} // �Զ�����
					}
					else if(cur_page==PAGE_SENCE_1) cur_page=PAGE_SENCE_2;
					else if(cur_page==PAGE_DIRECT_1) cur_page=PAGE_DIRECT_2;
					else if(cur_page==PAGE_SENCE_2)	 {SPK_BUF[spk_cur]=0;SYN_Speaker(0,SPK_BUF);spk_cur=0;}
					else if(cur_page==PAGE_DIRECT_2) {SPK_BUF[spk_cur]=0;SYN_Speaker(0,SPK_BUF);spk_cur=0;}
					break;
				case KEY_RIGHT: // ����
					if(cur_page==PAGE_SENCE_1 ||cur_page==PAGE_LOCATION||cur_page==PAGE_DIRECT_1||cur_page==PAGE_SECTOR||cur_page==PAGE_AUTO_DIRECT) 
					{cur_page=PAGE_HOME; update_timer=0;}
					else if(cur_page==PAGE_SENCE_2) cur_page=PAGE_SENCE_1;
					else if(cur_page==PAGE_DIRECT_2) cur_page=PAGE_DIRECT_1;
					break;
					
				case KEY_LEFT_LONG: // ����ȷ�� ����
					if(cur_page == PAGE_SENCE_1 || cur_page == PAGE_DIRECT_1)	{ show_dianzan(cur_sec-1);}
					break;
				case KEY_RIGHT_LONG: 
					Call_help();
					break;// ����ȡ��	����				
					
				default:					
					break;
			}
		}
}
		
// �ϴ�������Ϣ
void Call_help()
{
	sprintf((char*)FMT_BUF,"*1lat:%f,lng:%f#",m_latitude/100000.0,m_longitude/100000.0);
	//sprintf((char*)FMT_BUF,"*1lat:%f,lng:%f#",sence_pos[0].latitude,sence_pos[0].longitude);
	Bt_Upload(FMT_BUF);
	DelayMs(100);
}

// �ϴ�������Ϣ
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

// ��ʾUI
void Show_page(u8 page,u8 sec)
{
	LCD_OpenWindow(1,1,286,319);
	LCD_Fill(1,1,238,286,BACK_COLOR);
	
	if(page == PAGE_SENCE_2 || page==PAGE_DIRECT_2) 
		LCD_Disp_CnEn(44,300,POINT_COLOR,BACK_COLOR,"����");
	else
		LCD_Disp_CnEn(44,300,POINT_COLOR,BACK_COLOR,"ȷ��");
	
	LCD_Disp_CnEn(160,300,POINT_COLOR,BACK_COLOR,"����");
	
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

//��ʾ��ҳ
void show_home()
{	
	u8 i;
	for(i=0;i<5;++i)
	{
		LCD_Disp_CnEn(40,(SECTION_BASE*i)+SECTION_BASE/3,POINT_COLOR,BACK_COLOR,home_dict[i]);
		LCD_DrawHLine(1,(SECTION_BASE*(i+1)),238,POINT_COLOR);			
	}			
//	LCD_DispStr(5,SECTION_BASE*cur_sec-35,"->");		
	LCD_Show_Image(5,SECTION_BASE*cur_sec-40,30,30,gImage_jiantou); //��ʾ��ͷ
}
   
//��ʾGPS��λ��Ϣ 
void show_location()
{	
	switch(m_gpssta)//GPS״̬:0,δ��λ;1,�ǲ�ֶ�λ;2,��ֶ�λ;6,���ڹ���.	
	{
		case 0: 
			sprintf((char*)FMT_BUF,"��λ״̬:δ��λ");	//��λ״̬
			break;
		case 1: 
			sprintf((char*)FMT_BUF,"��λ״̬:�ǲ�ֶ�λ");	//��λ״̬
			break;
		case 2: 
			sprintf((char*)FMT_BUF,"��λ״̬:��ֶ�λ");	//��λ״̬
			break;
		case 6: 
			sprintf((char*)FMT_BUF,"��λ״̬:���ڹ���");	//��λ״̬
			break;			
	}
 	LCD_Disp_CnEn(30,EDGE_SEC_1-20,POINT_COLOR,BACK_COLOR,FMT_BUF);	 	 
	
	sprintf((char *)FMT_BUF,"����:%.5f %1c ",m_longitude/100000.0,m_ewhemi);	//�õ������ַ���
 	LCD_Disp_CnEn(30,EDGE_SEC_1,POINT_COLOR,BACK_COLOR,FMT_BUF);	 	   
  
	sprintf((char *)FMT_BUF,"γ��:%.5f %1c ",m_latitude/100000.0,m_nshemi);	//�õ�γ���ַ���
 	LCD_Disp_CnEn(30,EDGE_SEC_1+20,POINT_COLOR,BACK_COLOR,FMT_BUF);	 
	  
 	sprintf((char *)FMT_BUF,"�߶�:%.1fm   ",m_altitude/10.0);	    			//�õ��߶��ַ���
 	LCD_Disp_CnEn(30,EDGE_SEC_1+40,POINT_COLOR,BACK_COLOR,FMT_BUF);	 	
	
 	sprintf((char *)FMT_BUF,"�ٶ�:%.3fkm/h ",m_speed/1000.0);		    		//�õ��ٶ��ַ���	 
 	LCD_Disp_CnEn(30,EDGE_SEC_1+60,POINT_COLOR,BACK_COLOR,FMT_BUF);	 	
	if(m_fixmode<=3)														//��λ״̬
	{  
		sprintf((char *)FMT_BUF,"��λģʽ:%s",fixmode_tbl[m_fixmode]);	
		LCD_Disp_CnEn(30,EDGE_SEC_1+80,POINT_COLOR,BACK_COLOR,FMT_BUF);			   
	}	 	   
	sprintf((char *)FMT_BUF,"��λ������:%02d",m_posslnum);	 		//���ڶ�λ��������
 	LCD_Disp_CnEn(30,EDGE_SEC_1+100,POINT_COLOR,BACK_COLOR,FMT_BUF);	    
	
	sprintf((char *)FMT_BUF,"�ɼ�������:%02d",m_svnum%100);	 		//�ɼ�������
 	LCD_Disp_CnEn(30,EDGE_SEC_1+120,POINT_COLOR,BACK_COLOR,FMT_BUF);		 
	
	sprintf((char *)FMT_BUF,"UTC����:%04d/%02d/%02d ",m_utc_year,m_utc_month,m_utc_date);	//��ʾUTC����	
	LCD_Disp_CnEn(30,EDGE_SEC_1+140,POINT_COLOR,BACK_COLOR,FMT_BUF);		    
	
	sprintf((char *)FMT_BUF,"UTCʱ��:%02d:%02d:%02d ",m_utc_hour,m_utc_min,m_utc_sec);	//��ʾUTCʱ��
  LCD_Disp_CnEn(30,EDGE_SEC_1+160,POINT_COLOR,BACK_COLOR,FMT_BUF);
	
}


// ��ʾ������Ϣ
void show_sence1()
{
	u8 i;
	for(i=0;i<5;++i)
	{
		LCD_Disp_CnEn(40,(SECTION_BASE*i)+SECTION_BASE/3,POINT_COLOR,BACK_COLOR,sence_dict[i]);
		LCD_DrawHLine(1,(SECTION_BASE*(i+1)),238,POINT_COLOR);		
	}
	
	LCD_Show_Image(5,SECTION_BASE*cur_sec-40,30,30,gImage_jiantou); //��ʾ��ͷ
	//LCD_DispStr(5,SECTION_BASE*cur_sec-35,"->");	  
	// ��ʾ�ѵ���
	LCD_Show_Image(200,SECTION_BASE*dianzan_sec-40,30,30,gImage_dianzan);
		
}

// ��ʾ��������
void show_sence2(u8 pos)
{
	spk_cur=0; //�������¿�ʼ
	
	LCD_Disp_CnEn((LCD_WIDTH-strlen(sence_dict[pos])*8)/2,EDGE_SEC_3+SECTION_BASE/3,POINT_COLOR,BACK_COLOR,sence_dict[pos]);
	SPK_add(sence_dict[pos]);
	
	sprintf((char *)FMT_BUF,"λ�þ���: %f",sence_pos[pos].longitude);
	LCD_Disp_CnEn(30,EDGE_SEC_4,POINT_COLOR,BACK_COLOR,FMT_BUF);
	SPK_add(FMT_BUF);
	
	sprintf((char *)FMT_BUF,"λ��γ��: %f",sence_pos[pos].latitude);
	LCD_Disp_CnEn(30,EDGE_SEC_4+20,POINT_COLOR,BACK_COLOR,FMT_BUF);
	SPK_add(FMT_BUF);
}  

// ��ʾ������Ϣ
void show_sector()
{
	LCD_Disp_CnEn(80,SECTION_BASE/3,POINT_COLOR,BACK_COLOR,"��Ϣ");
	if(Receive_area_ok)
	{
		LCD_Disp_BufCn(20,EDGE_SEC_1+SECTION_BASE/3,POINT_COLOR,BACK_COLOR,USART2_BUF+5,USART2_len-5);	
	}else
	{
		LCD_Disp_CnEn(80,EDGE_SEC_1+SECTION_BASE/3,POINT_COLOR,BACK_COLOR,"No Data!");	
	}	
}

// ��ʾ����
void show_direct1()
{
	show_sence1();
}	

// ��ʾ����
void show_direct2(u8 pos)
{
	u8 dir;
	double dis;
	
	spk_cur=0; //�������¿�ʼ
	
	dir=Get_Direction(m_latitude/100000.0,m_longitude/100000.0,sence_pos[pos].latitude,sence_pos[pos].longitude);
	dis=Get_Distance(m_latitude/100000.0,m_longitude/100000.0,sence_pos[pos].latitude,sence_pos[pos].longitude);
	
	sprintf((char *)FMT_BUF,"%s�ڵ�ǰλ�õ�%s����,�����Լ%.2fm",sence_dict[pos],direction[dir],dis);
	LCD_Disp_CnEn(20,EDGE_SEC_4,POINT_COLOR,BACK_COLOR,FMT_BUF);
	SPK_add(FMT_BUF);
	
//sprintf((char *)FMT_BUF,"",dis);
	//LCD_Disp_CnEn(40,EDGE_SEC_4+20,POINT_COLOR,BACK_COLOR,FMT_BUF);
	//SPK_add(FMT_BUF);
	
}

// �Զ����ξ���
void show_auto_direct(void)
{
	u8 i,pos,dir;
	u16 min_dis=65534;
	double dis;
	// ��ʾ����
	LCD_Disp_CnEn(60,40,POINT_COLOR,BACK_COLOR,"�����������ǹ�԰");
	// ��ȡ����ľ���
	for(i=0;i<5;++i)
	{
		dis=Get_Distance(m_latitude/100000.0,m_longitude/100000.0,sence_pos[i].latitude,sence_pos[i].longitude);
		if(dis<min_dis)
		{
			min_dis = dis;
			pos = i;
		}
	}
	// �ж��Ƿ���һ��ֵ��	
	if(min_dis>1000)
	{
		LCD_Disp_CnEn(65,80,POINT_COLOR,BACK_COLOR,"���뾰��̫Զ");
		SYN_Speaker(0,"��ǰ����λ�þ��뾰�㳬��1ǧ��");
	}else{
		//  ��ȡ��λ
		dir=Get_Direction(m_latitude/100000.0,m_longitude/100000.0,sence_pos[pos].latitude,sence_pos[pos].longitude);
		sprintf((char*)FMT_BUF,"ǰ������Ϊ:%s,λ��%s���� ",sence_dict[pos],direction[dir]);
		LCD_Disp_CnEn(10,80,POINT_COLOR,BACK_COLOR,FMT_BUF);
		SYN_Speaker(0,FMT_BUF);					
	}
	
	if(m_gpssta==1 || m_gpssta== 2 )
		LCD_Disp_CnEn(60,120,POINT_COLOR,BACK_COLOR,"��λ״̬:�ɹ�");
	else
		LCD_Disp_CnEn(60,120,POINT_COLOR,BACK_COLOR,"��λ״̬:ʧ��");
	
	sprintf(FMT_BUF,"�ƶ��ٶ�:%.3fkm/h",m_speed/1000.0);
	LCD_Disp_CnEn(60,140,POINT_COLOR,BACK_COLOR,FMT_BUF);
	
	sprintf((char *)FMT_BUF,"��ǰʱ��:%02d:%02d:%02d ",m_utc_hour+8,m_utc_min,m_utc_sec);	//��ʾGMTʱ��
	LCD_Disp_CnEn(60,160,POINT_COLOR,BACK_COLOR,FMT_BUF);
	
}

// ���������Ϣ
void SPK_add(u8 *str)
{
	u8 i=0,len=0;
	len=strlen(str);
	if(len>126) return;
	while(i<len){if(spk_cur>126)break; SPK_BUF[spk_cur++]=FMT_BUF[i++];}
}


//���Ժ���
u8 jiance() //��ⴥ���Ͱ���
{
	if (Touch_State()==0) //����������£�������ͼ����
	 {
	 	LCD_Clear(WHITE); //����
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


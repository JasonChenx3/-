/* ͷ�ļ��� */
#include <STC15F2K60S2.H>
#include <INIT.H>
#include <LED.H>
#include <KEY.H>
#include <SEG.H>

/* ������ */
unsigned char Key_Slow_Down, Seg_Slow_Down;  // ���ٱ���
unsigned char Key_Val, Key_Old, Key_Down, Key_Up;  // ����ɨ��ר�ñ���
unsigned char Seg_Pos; // �����ɨ�����
unsigned char Seg_Buf[8] = {10, 10, 10, 10, 10, 10, 10, 10};  // �������ʾ���飬Ĭ��ȫϨ��
unsigned char Seg_Point[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // // �����С������ʾ���飬Ĭ��ȫϨ��

/* ���������� */
void Key_Proc() 
{
	if (Key_Slow_Down) return;
	Key_Slow_Down = 1;
	
	Key_Val = Key_Read(0);  // ʵʱ��ȡ����ֵ
	Key_Down = Key_Val & (Key_Val ^ Key_Old);  // ��ȡ�½���
	Key_Up = ~Key_Val & (Key_Val ^ Key_Old);  // ��ȡ������
	Key_Old = Key_Val;  // ��������
	
	if (Key_Down)
	{
		Seg_Buf[7] = Key_Down % 10;
		Seg_Buf[6] = (Key_Down / 10) % 10;
		Led_Disp(0, Key_Down % 2);
	}
}

/* ��Ϣ������ */
void Seg_Proc() 
{
	if (Seg_Slow_Down) return;
	Seg_Slow_Down = 1;
	
	
}

/* ������ʾ���� */
void Led_Proc() 
{
	
}

/* ��ʱ��0��ʼ������ */
void Timer0_Init()		//1����@12.000MHz
{
	AUXR &= 0x7F;			//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TL0 = 0x18;				//���ö�ʱ��ʼֵ
	TH0 = 0xFC;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
	ET0 = 1;				//ʹ�ܶ�ʱ��0�ж�
	EA = 1;
}


/* ��ʱ��0�жϷ�����*/
void Timer0_Isr() interrupt 1 
{
	if ( ++ Key_Slow_Down == 10) Key_Slow_Down = 0;
	if ( ++ Seg_Slow_Down == 500) Key_Slow_Down = 0;
	if ( ++ Seg_Pos == 8) Seg_Pos = 0;
	Seg_Disp(Seg_Pos, Seg_Buf[Seg_Pos], Seg_Point[Seg_Pos]);
	
	
}

/* Main */
void main() 
{
	Sys_Init();
	Timer0_Init();
	while (1) 
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}
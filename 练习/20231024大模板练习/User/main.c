/* ͷ�ļ��������� */
#include <STC15F2K60S2.H>  // 15��Ƭ���Ĵ���ר��ͷ�ļ�
#include <KEY.H>  // �����ײ�ר��ͷ�ļ�
#include <SEG.H>  // ����ܵײ�ר��ͷ�ļ�
#include <INIT.h>  // ��ʼ���ײ�ר��ͷ�ļ�
#include <LED.H>  // LED�ײ�ר��ͷ�ļ�

/* ������������ */
unsigned char Key_Slow_Down;  // ��������ר�ñ���
unsigned char Key_Val, Key_Old, Key_Down, Key_Up;  // ����ɨ��ר�ñ���
unsigned int Seg_Slow_Down;  // ����ܼ���ר�ñ���
unsigned char Seg_Pos;  // �����ɨ��ר�ñ���
unsigned char Seg_Buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};  // �����������ʾ���� Ĭ��ȫ��Ϩ��
unsigned char Seg_Point[8] = {1, 1, 1, 1, 1, 1, 1, 0};  // �����С������������ Ĭ��ȫ��Ϩ��

/* ���������� */
void Key_Proc()
{
	if (Key_Slow_Down) return;
	Key_Slow_Down = 1;  // �������ٳ���
	
	Key_Val = Key_Read();  // ʵʱ��ȡ����ֵ
	Key_Down = Key_Val & (Key_Old ^ Key_Val);  // ��ȡ�����½���
	Key_Up = ~Key_Val & (Key_Old ^ Key_Val);  // ��ȡ����������
	Key_Old = Key_Val;  // ����ɨ�����
	
	
}

/* ��Ϣ������ */
void Seg_Proc()
{
	if (Seg_Slow_Down) return;
	Seg_Slow_Down = 1;  // ����ܼ��ٳ���
	
	
}

/* ������ʾ���� */
void Led_Proc()
{
	
}

/* ��ʱ��0��ʼ������ */
void Timer0_Init(void)		//1����@12.000MHz
{
	AUXR &= 0x7F;			//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TL0 = 0x18;				//���ö�ʱ��ʼֵ
	TH0 = 0xFC;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
	ET0 = 1;
	EA = 1;
}

/* ��ʱ��0�жϷ����� */
void Timer0_Server() interrupt 1
{
	if ( ++ Key_Slow_Down == 10) Key_Slow_Down = 0;
	if ( ++ Seg_Slow_Down == 500) Seg_Slow_Down = 0; 
	if ( ++ Seg_Pos == 8) Seg_Pos = 0;
	Seg_Disp(Seg_Pos, Seg_Buf[Seg_Pos], Seg_Point[Seg_Pos]);
	
	
}

/* Main */
void main()
{
	Sys_Init();
	Timer0_Init();
	while(1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}
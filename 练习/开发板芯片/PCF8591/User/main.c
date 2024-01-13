// Header
#include <STC15F2K60S2.H>
#include <SYS.H>
#include <LED.H>
#include <SEG.H>
#include <KEY.H>
#include <IIC.H>

// ����
unsigned int i, j, k, dat;
unsigned char Key_Slow_Down, Seg_Slow_Down;  // ���ٱ���
unsigned char Key_Val, Key_Old, Key_Down, Key_Up;  // ����ɨ��ר�ñ���
unsigned int Seg_Pos; // �����ɨ�����
unsigned char Seg_Buf[8] = {10, 10, 10, 10, 10, 10, 10, 10};  // �������ʾ���飬Ĭ��ȫϨ��
unsigned char Seg_Point[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // // �����С������ʾ���飬Ĭ��ȫϨ��

// ����
void Key_Proc()
{
	if (Key_Slow_Down) return;
	Key_Slow_Down = 1;
	
	Key_Val = Key_Read();  // ʵʱ��ȡ����ֵ
	Key_Down = Key_Val & (Key_Val ^ Key_Old);  // ��ȡ�½���
	Key_Up = ~Key_Val & (Key_Val ^ Key_Old);  // ��ȡ������
	Key_Old = Key_Val;  // ��������
	
	
}

// �����
void Seg_Proc() 
{
	if (Seg_Slow_Down) return;
	Seg_Slow_Down = 1;
	
	dat = PCF8591_ADC();
	Seg_Buf[0] = dat / 100 % 10;
	Seg_Buf[1] = dat / 10 % 10;
	Seg_Buf[2] = dat / 1 % 10;
}

// LED
void Led_Proc() 
{
	for (i = 0; i < 8; i ++ )
	{
		Led_Disp(i, 0);
	}
}

// ��ʱ��0�жϷ���
void Timer0_Isr() interrupt 1 
{
	if ( ++ Key_Slow_Down == 10) Key_Slow_Down = 0;
	if ( ++ Seg_Slow_Down == 500) Seg_Slow_Down = 0;
	if ( ++ Seg_Pos == 8) Seg_Pos = 0;
	Seg_Disp(Seg_Pos, Seg_Buf[Seg_Pos], Seg_Point[Seg_Pos]);
	
	
}

// ��ʱ����ʼ��
void Timer0_Init(void)		//1����@12.000MHz
{
	AUXR |= 0x80;			//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TL0 = 0x20;				//���ö�ʱ��ʼֵ
	TH0 = 0xD1;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
	ET0 = 1;				//ʹ�ܶ�ʱ��0�ж�
	EA = 1;
}


// Main
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
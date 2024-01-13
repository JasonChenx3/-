/* ͷ�ļ� */
#include <STC15F2K60S2.H>
#include "INIT.H"
#include "LED.H"
#include "KEY.H"
#include "SEG.H"

/* ������ */
unsigned char Key_Slow_Down;  // ��������
unsigned char Key_Val, Key_Down, Key_Up, Key_Old;  // �������
unsigned char Seg_Slow_Down;  // ����ܼ���
unsigned char Seg_Pos;  // �����ɨ��
unsigned char Seg_Buf[8] = {10, 10, 10, 10, 10, 10, 10, 10};  // �������ʾ
unsigned char Seg_Point[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // �����С����
unsigned char ms, _50ms, s, min;  // ʱ����ʾ
unsigned char run;

/* �������� */
void Key_Proc()
{
	if (Key_Slow_Down) return;
	Key_Slow_Down = 1;
	
	Key_Val = Key_Read(1);  // ��ȡ��ʱ���󰴼�
	Key_Down = Key_Val & (Key_Val ^ Key_Old);  // ��ȡ�����½���
	Key_Up = ~Key_Val & (Key_Val ^ Key_Old);  // ��ȡ����������
	Key_Old = Key_Val;
	
	switch (Key_Down)
	{
		case 4:
			run ^= 1;
		break;
		case 5:
		{
				if (run == 0)
				{
					ms = 0;
					_50ms = 0;
					s = 0;
					min = 0;
				}
				break;
		}
	}
	
	Seg_Buf[0] = (min / 10) % 10;
	Seg_Buf[1] = (min % 10);
	Seg_Buf[2] = 11;
	Seg_Buf[3] = (s / 10) % 10;
	Seg_Buf[4] = s % 10;
	Seg_Buf[5] = 11;
	Seg_Buf[6] = (_50ms / 10) % 10;
	Seg_Buf[7] = _50ms % 10;
}


/* ��Ϣ���� */
void Seg_Proc()
{
	if (Seg_Slow_Down) return;
	Seg_Slow_Down = 1;
	
	
}


/* ������ʾ */
void Led_Proc()
{
	
}

/* ��ʱ��0��ʼ�� */
void Timer0_Init()		//1����@12.000MHz
{
	AUXR |= 0x80;			//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TL0 = 0x20;				//���ö�ʱ��ʼֵ
	TH0 = 0xD1;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
	ET0 = 1;				//ʹ�ܶ�
	EA = 1;  //�����ж�
}

/* ��ʱ��0�жϷ����� */
void Timer0_Isr() interrupt 1
{
	if ( ++ Key_Slow_Down == 10) Key_Slow_Down = 0;
	if ( ++ Seg_Slow_Down == 500) Seg_Slow_Down = 0;
	if ( ++ Seg_Pos == 8) Seg_Pos = 0;
	Seg_Disp(Seg_Pos, Seg_Buf[Seg_Pos], Seg_Point[Seg_Pos]);
	
	if (run)
	{
		ms ++ ;
		if (ms == 50)
		{
			ms = 0;
			_50ms ++ ;
			if (_50ms == 20)
			{
				_50ms = 0;
				s ++ ;
				if (s == 60)
				{
					s = 0;
					min ++ ;
					if (min == 60)
					{
						min = 0;
					}
				}
			}
		}
	}
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
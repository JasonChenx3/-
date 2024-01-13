// Header
#include <STC15F2K60S2.H>
#include <SYS.H>
#include <LED.H>
#include <SEG.H>
#include <KEY.H>

// ����
unsigned char Key_Slow_Down, Seg_Slow_Down;  // ���ٱ���
unsigned char Key_Val, Key_Old, Key_Down, Key_Up;  // ����ɨ��ר�ñ���
unsigned int Seg_Pos; // �����ɨ�����
unsigned char Seg_Buf[8] = {10, 10, 10, 10, 10, 10, 10, 10};  // �������ʾ���飬Ĭ��ȫϨ��
unsigned char Seg_Point[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // // �����С������ʾ���飬Ĭ��ȫϨ��
unsigned char time_h = 23, time_m = 59, time_s = 59;  // ʱ��
unsigned char alarm_h = 0, alarm_m = 0, alarm_s = 0;  // ����
unsigned int ms_cnt;
unsigned int led_time, led_cnt;
bit led_state;

// ����
void Key_Proc()
{
	if (Key_Slow_Down) return;
	Key_Slow_Down = 1;
	
	Key_Val = Key_Read();  // ʵʱ��ȡ����ֵ
	Key_Down = Key_Val & (Key_Val ^ Key_Old);  // ��ȡ�½���
	Key_Up = ~Key_Val & (Key_Val ^ Key_Old);  // ��ȡ������
	Key_Old = Key_Val;  // ��������
	
	// ��ʱ����������ֹͣ��˸
	if (led_time > 0 && Key_Down != 0)
	{
		led_time = 0;
		Led_Disp(0, 0);
	}
}

// �����
void Seg_Proc() 
{
	if (Seg_Slow_Down) return;
	Seg_Slow_Down = 1;
	
	// ����������
	Seg_Buf[0] = time_h / 10;
	Seg_Buf[1] = time_h % 10;
	Seg_Buf[2] = 11;
	Seg_Buf[3] = time_m / 10;
	Seg_Buf[4] = time_m % 10;
	Seg_Buf[5] = 11;
	Seg_Buf[6] = time_s / 10;
	Seg_Buf[7] = time_s % 10;
	
	// ���ﶨʱ���ӣ����ö�ʱ����
	if (time_h == alarm_h && time_m == alarm_m && time_s == alarm_s)
	{
		led_time = 5000;
		led_cnt = 0;
	}
}

// LED
void Led_Proc() 
{
	
}

// ��ʱ��0�жϷ���
void Timer0_Isr() interrupt 1 
{
	if ( ++ Key_Slow_Down == 10) Key_Slow_Down = 0;
	if ( ++ Seg_Slow_Down == 50) Seg_Slow_Down = 0;
	if ( ++ Seg_Pos == 8) Seg_Pos = 0;
	Seg_Disp(Seg_Pos, Seg_Buf[Seg_Pos], Seg_Point[Seg_Pos]);
	
	// ����ʱ��
	if ( ++ ms_cnt == 1000)
	{
		ms_cnt = 0;
		if ( ++ time_s == 60)
		{
			time_s = 0;
			if ( ++ time_m == 60)
			{
				time_m = 0;
				if ( ++ time_h == 24)
				{
					time_h = 0;
				}
			}
		}
	}
	
	// ����LED��ʾ
	if (led_time > 0)
	{
		if (led_cnt == 0)
		{
			led_state = ~led_state;
			Led_Disp(0, led_state);
		}
		led_cnt = (led_cnt + 1) % 200;
		led_time -= 1;
	}
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
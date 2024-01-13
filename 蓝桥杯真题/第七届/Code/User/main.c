// Header
#include <STC15F2K60S2.H>
#include <SYS.H>
#include <LED.H>
#include <SEG.H>
#include <KEY.H>
#include <ONEWIRE.H>

// ����
unsigned char Key_Slow_Down;  // ����ɨ����ٱ���
unsigned int Seg_Slow_Down;  // �����ɨ����ٱ���
unsigned char Key_Val, Key_Old, Key_Down, Key_Up;  // ����ɨ��ר�ñ���
unsigned int Seg_Pos; // �����ɨ�����
unsigned char Seg_Buf[8] = {10, 10, 10, 10, 10, 10, 10, 10};  // �������ʾ���飬Ĭ��ȫϨ��
unsigned char Seg_Point[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // // �����С������ʾ���飬Ĭ��ȫϨ��
unsigned char work_mode;  // ���ȹ���ģʽ
unsigned int work_time;  // ���ȹ���ʱ��
unsigned char time_cnt;  // �����˼���
bit show_mode;  // ��ʾģʽ��0��ʾ���ȣ�1��ʾ�¶�
unsigned int ms;  // 1ms����
int temper;  // �¶ȣ�ȥ��С�����ֵ��
unsigned int temper_read_cnt;
unsigned char PWM, timer1_cnt;

// ����
void Key_Proc()
{
	if (Key_Slow_Down) return;
	Key_Slow_Down = 1;
	
	Key_Val = Key_Read();  // ʵʱ��ȡ����ֵ
	Key_Down = Key_Val & (Key_Val ^ Key_Old);  // ��ȡ�½���
	Key_Up = ~Key_Val & (Key_Val ^ Key_Old);  // ��ȡ������
	Key_Old = Key_Val;  // ��������
	
	if (Key_Down == 4)  // �л�����ģʽ
	{
		if (show_mode == 0)  // ��ʾ���Ȳ�����Ч
		{
			work_mode = (work_mode + 1) % 3;
			work_time = 0;
			time_cnt = 0;
			timer1_cnt = 0;
			switch (work_mode)
			{
				case 0:
					PWM = 20;
					break;
				case 1:
					PWM = 30;
					break;
				case 2:
					PWM = 70;
					break;
			}
		}
	}
	else if (Key_Down == 5)  // �л���ʱ
	{
		if (show_mode == 0)  // ��ʾ���Ȳ�����Ч
		{
			ms = 0;
			time_cnt ++ ;
			work_time = time_cnt * 60;
			if (work_time > 120)
			{
				work_time = 0;
				time_cnt = 0;
			}
		}
	}
	else if (Key_Down == 6)  // ��ʱ����
	{
		if (show_mode == 0)  // ��ʾ���Ȳ�����Ч
		{
			work_time = 0;
		}
	}
	else if (Key_Down == 7)  // ��ʾ�л�
	{
		show_mode = ~show_mode;
	}
}

// �����
void Seg_Proc() 
{
	if (Seg_Slow_Down) return;
	Seg_Slow_Down = 1;
	
	if (show_mode)  // ��ʾ�¶Ȳ���
	{
		Seg_Buf[0] = 11;
		Seg_Buf[1] = 4;
		Seg_Buf[2] = 11;
		Seg_Buf[3] = 10;
		Seg_Buf[4] = 10;
		Seg_Buf[5] = temper >= 10 ? temper / 10 % 10 : 0;
		Seg_Buf[6] = temper >= 1 ? temper % 10 : 0;
		Seg_Buf[7] = 12;
	}
	else  // ��ʾ���Ȳ���
	{
		Seg_Buf[0] = 11;
		Seg_Buf[1] = work_mode + 1;
		Seg_Buf[2] = 11;
		Seg_Buf[3] = 10;
		Seg_Buf[4] = 0;
		Seg_Buf[5] = work_time >= 100 ? work_time / 100 : 0;
		Seg_Buf[6] = work_time >= 10 ? work_time / 10 % 10 : 0;
		Seg_Buf[7] = work_time >= 1 ? work_time % 10 : 0;
	}
}

// LED
void Led_Proc() 
{
	unsigned char i;
	if (show_mode == 0)
	{
		if (work_time == 0) 
		{
			for (i = 0; i < 8; i ++ )
			{
				Led_Disp(i, 0);
			}
			return;
		}
		Led_Disp(work_mode, 1);
	}
	else
	{
		for (i = 0; i < 8; i ++ )
		{
			Led_Disp(i, 0);
		}
	}
}

// ��ʱ���жϷ���
void Timer0_Isr() interrupt 1 
{
	if ( ++ Key_Slow_Down == 10) Key_Slow_Down = 0;  // �������10msһ��
	if ( ++ Seg_Slow_Down == 2) Seg_Slow_Down = 0;  // ����ܸ���2msһ��
	if ( ++ Seg_Pos == 8) Seg_Pos = 0;
	Seg_Disp(Seg_Pos, Seg_Buf[Seg_Pos], Seg_Point[Seg_Pos]);
	if (work_time > 0)
	{
		if ( ++ ms == 1000)
		{
			ms = 0;
			work_time -= 1;
		}
	}
	else
	{
		ms = 0;
	}
	if ( ++ temper_read_cnt == 1000)
	{
		temper_read_cnt = 0;
	}
}

void Timer1_Isr() interrupt 3
{
	++ timer1_cnt;
	if (timer1_cnt <= PWM)
	{
		P34 = 1;
	}
	else 
	{
		P34 = 0;
	}
}

// ��ʱ����ʼ��
void Timer0_Init()		//1����@12.000MHz
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

void Timer1_Init()		//10΢��@11.0592MHz
{
	AUXR |= 0x40;			//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;			//���ö�ʱ��ģʽ
	TL1 = 0x91;				//���ö�ʱ��ʼֵ
	TH1 = 0xFF;				//���ö�ʱ��ʼֵ
	TF1 = 0;				//���TF1��־
	TR1 = 1;				//��ʱ��1��ʼ��ʱ
	ET1 = 1;				//ʹ�ܶ�ʱ��1�ж�
}



// Main
void main()
{
	Sys_Init();
	Timer0_Init();
	Timer1_Init();
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
		if (temper_read_cnt == 0)
		{
			temper = (int)read_t();
		}
	}
}
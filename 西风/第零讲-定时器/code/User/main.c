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
unsigned char Seg_Disp_Mode;  // �������ʾģʽ 0-ʱ����ʾ���� 1-ʱ�����ý��� 2-�������ý���
unsigned char Clock_Disp[3] = {23, 59, 55};  // ʱ����ʾ
unsigned char Clock_Set[3];  // ʱ��������ʾ
unsigned char Clock_Set_Index;  // ʱ��ѡ��ָ�� 0-Сʱ 1-���� 2-����
unsigned char Alarm[3] = {0, 0, 0};  // ����ʱ��
unsigned char Alarm_Set[3];  // ����������ʾ
unsigned int Timer_1000Ms;
unsigned int Timer_500Ms;
bit Seg_Flag;  // �������˸����
bit Alarm_Flag;  // ����ʹ��/��ֹ����
bit Alarm_Enable_Flag;  // ���ӿ���/�رտ���

// ����
void Key_Proc()
{
	if (Key_Slow_Down) return;
	Key_Slow_Down = 1;
	
	Key_Val = Key_Read();  // ʵʱ��ȡ����ֵ
	Key_Down = Key_Val & (Key_Val ^ Key_Old);  // ��ȡ�½���
	Key_Up = ~Key_Val & (Key_Val ^ Key_Old);  // ��ȡ������
	Key_Old = Key_Val;  // ��������
	if (Key_Down != 0) 
	{
		Alarm_Enable_Flag = 0;
	}
	switch (Key_Down)
	{
		case 4:  // չʾʱ�����ý���
		{
			// ʱ��������ʾ���ǰ������µ�ʱ��
			Alarm_Set[0] = Alarm[0];
			Alarm_Set[1] = Alarm[1];
			Alarm_Set[2] = Alarm[2];
			Seg_Disp_Mode = 1;
			Clock_Set_Index = 0;
			break;
		}
		case 5:  // �������ý���
		{
			Clock_Set[0] = Clock_Disp[0];
			Clock_Set[1] = Clock_Disp[1];
			Clock_Set[2] = Clock_Disp[2];
			Seg_Disp_Mode = 2;
			Clock_Set_Index = 0;
			break;
		}
		case 6:  // ʱ������ȡ��
		{
			if (Seg_Disp_Mode != 0)
			{
				Seg_Disp_Mode = 0;
			}
			break;
		}
		case 7:  // ����ȷ�ϣ���չʾʱ����ʾ����
		{
			if (Seg_Disp_Mode == 1)
			{
				Clock_Disp[0] = Clock_Set[0];
				Clock_Disp[1] = Clock_Set[1];
				Clock_Disp[2] = Clock_Set[2];
				Seg_Disp_Mode = 0;
			} 
			else if (Seg_Disp_Mode == 2) 
			{
				Alarm[0] = Alarm_Set[0];
				Alarm[1] = Alarm_Set[1];
				Alarm[2] = Alarm_Set[2];
				Seg_Disp_Mode = 0;
			}
			break;
		}
		case 8:  // ʱ�������л�
		{
			if (Seg_Disp_Mode != 0)
			{
				Clock_Set_Index = (Clock_Set_Index + 1) % 3;
			}
			break;
		}
		case 12:
		{
			switch (Seg_Disp_Mode)
			{
				case 1:  // ����ʱ�����ý���
				{
					Clock_Set[Clock_Set_Index] ++ ;
					switch (Clock_Set_Index)
					{
						case 0:  // Сʱλ
						{
							if (Clock_Set[Clock_Set_Index] == 24)
							{
								Clock_Set[Clock_Set_Index] = 0;
							}
							break;
						}
						default:  // ���Ӻ�����λ
						{
							if (Clock_Set[Clock_Set_Index] == 60)
							{
								Clock_Set[Clock_Set_Index] = 0;
							}
							break;
						}
					}
					break;
				}
				case 2:  // �����������ý���
				{
					Alarm_Set[Clock_Set_Index] ++ ;
					switch (Clock_Set_Index)
					{
						case 0:  // Сʱλ
						{
							if (Alarm_Set[Clock_Set_Index] == 24)
							{
								Alarm_Set[Clock_Set_Index] = 0;
							}
							break;
						}
						default:  // ���Ӻ�����λ
						{
							if (Alarm_Set[Clock_Set_Index] == 60)
							{
								Alarm_Set[Clock_Set_Index] = 0;
							}
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case 13: 
		{
			switch (Seg_Disp_Mode)
			{
				case 1:  // ����ʱ�����ý���
				{
					Clock_Set[Clock_Set_Index] -- ;
					switch (Clock_Set_Index)
					{
						case 0:  // Сʱλ
						{
							if (Clock_Set[Clock_Set_Index] == 255)
							{
								Clock_Set[Clock_Set_Index] = 23;
							}
							break;
						}
						default:  // ���Ӻ�����λ
						{
							if (Clock_Set[Clock_Set_Index] == 255)
							{
								Clock_Set[Clock_Set_Index] = 59;
							}
							break;
						}
					}
					break;
				}
				case 2:  // �����������ý���
				{
					Alarm_Set[Clock_Set_Index] -- ;
					switch (Clock_Set_Index)
					{
						case 0:  // Сʱλ
						{
							if (Alarm_Set[Clock_Set_Index] == 255)
							{
								Alarm_Set[Clock_Set_Index] = 23;
							}
							break;
						}
						default:  // ���Ӻ�����λ
						{
							if (Alarm_Set[Clock_Set_Index] == 255)
							{
								Alarm_Set[Clock_Set_Index] = 59;
							}
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case 16:  // ����ʹ��
		{
			Alarm_Flag ^= 1;
			break;
		}
	}
}

// �����
void Seg_Proc() 
{
	if (Seg_Slow_Down) return;
	Seg_Slow_Down = 1;
	
	switch (Seg_Disp_Mode) 
	{
		case 0:  // ʱ����ʾ����
		{
			// ��ʾ����
			Seg_Buf[0] = Clock_Disp[0] / 10 % 10;
			Seg_Buf[1] = Clock_Disp[0]% 10;
			Seg_Buf[2] = 11;
			Seg_Buf[3] = Clock_Disp[1] / 10 % 10;
			Seg_Buf[4] = Clock_Disp[1] % 10;
			Seg_Buf[5] = 11;
			Seg_Buf[6] = Clock_Disp[2] / 10 % 10;
			Seg_Buf[7] = Clock_Disp[2] % 10;
			Seg_Point[1] = Seg_Point[4] = Seg_Point[7] = 1;
			break;
		}
		case 1:  // ʱ�����ý���
		{
			// ��ʾ����
			Seg_Buf[0] = Clock_Set[0] / 10 % 10;
			Seg_Buf[1] = Clock_Set[0]% 10;
			Seg_Buf[2] = 11;
			Seg_Buf[3] = Clock_Set[1] / 10 % 10;
			Seg_Buf[4] = Clock_Set[1] % 10;
			Seg_Buf[5] = 11;
			Seg_Buf[6] = Clock_Set[2] / 10 % 10;
			Seg_Buf[7] = Clock_Set[2] % 10;
			Seg_Point[1] = Seg_Point[4] = Seg_Point[7] = 1;
			// ��˸����
			Seg_Buf[0 + 3 * Clock_Set_Index] = Seg_Flag ? Clock_Set[Clock_Set_Index] / 10 % 10 : 10;
			Seg_Buf[1 + 3 * Clock_Set_Index] = Seg_Flag ? Clock_Set[Clock_Set_Index] % 10 : 10;
			break;
		}
		case 2:  // ����������ʾ����
		{
			// ��ʾ����
			Seg_Buf[0] = Alarm_Set[0] / 10 % 10;
			Seg_Buf[1] = Alarm_Set[0]% 10;
			Seg_Buf[2] = 11;
			Seg_Buf[3] = Alarm_Set[1] / 10 % 10;
			Seg_Buf[4] = Alarm_Set[1] % 10;
			Seg_Buf[5] = 11;
			Seg_Buf[6] = Alarm_Set[2] / 10 % 10;
			Seg_Buf[7] = Alarm_Set[2] % 10;
			Seg_Point[1] = Seg_Point[4] = Seg_Point[7] = 1;
			// ��˸����
			Seg_Buf[0 + 3 * Clock_Set_Index] = Seg_Flag ? Alarm_Set[Clock_Set_Index] / 10 % 10 : 10;
			Seg_Buf[1 + 3 * Clock_Set_Index] = Seg_Flag ? Alarm_Set[Clock_Set_Index] % 10 : 10;
			break;
		}
	}
}

// LED
void Led_Proc() 
{
	unsigned char i;
	if (Alarm_Flag)
	{
		if (Clock_Disp[0] == Alarm[0] && Clock_Disp[1] == Alarm[1] && Clock_Disp[2] == Alarm[2])
		{
			Alarm_Enable_Flag = 1;
		}
		if (Alarm_Enable_Flag)
		{
			for (i = 0; i < 8; i ++ )
			{
				if (Clock_Disp[0] >= 12)
				{
					if (i >= 4)
					{
						Led_Disp(i, Seg_Flag);
					} 
					else 
					{
						Led_Disp(i, 0);
					}
				}
				else
				{
					if (i < 4)
					{
						Led_Disp(i, Seg_Flag);
					} 
					else 
					{
						Led_Disp(i, 0);
					}
				}
			}
		}
		else
		{
			for (i = 0; i < 8; i ++ )
			{
				Led_Disp(i, 0);
			}
		}
	}
	else
	{
		for (i = 0; i < 8; i ++ )
		{
			Led_Disp(i, 0);
		}
	}
}

// ��ʱ��0�жϷ���
void Timer0_Isr() interrupt 1 
{
	if ( ++ Key_Slow_Down == 10) Key_Slow_Down = 0;
	if ( ++ Seg_Slow_Down == 500) Seg_Slow_Down = 0;
	if ( ++ Seg_Pos == 8) Seg_Pos = 0;
	Seg_Disp(Seg_Pos, Seg_Buf[Seg_Pos], Seg_Point[Seg_Pos]);
	
	Timer_1000Ms ++ ;
	if (Timer_1000Ms == 1000)
	{
		Timer_1000Ms = 0;
		Clock_Disp[2] ++ ;
		if (Clock_Disp[2] == 60) 
		{
			Clock_Disp[2] = 0;
			Clock_Disp[1] ++ ;
			if (Clock_Disp[1] == 60) 
			{
				Clock_Disp[1] = 0;
				Clock_Disp[0] ++ ;
				if (Clock_Disp[0] == 24) 
				{
					Clock_Disp[0] = 0;
				}
			}
		}
	}
	
	Timer_500Ms ++ ;
	if (Timer_500Ms == 500) 
	{
		Timer_500Ms = 0;
		Seg_Flag ^= 1;
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
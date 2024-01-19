// Header
#include <STC15F2K60S2.H>
#include <SYS.H>
#include <LED.H>
#include <SEG.H>
#include <KEY.H>

// ����
unsigned char Key_Slow_Down = 0, Seg_Slow_Down = 0;  					// ���ٱ���
unsigned char Key_Val, Key_Old, Key_Down, Key_Up;  						// ����ɨ��
unsigned char Seg_Buf[8] = {10, 10, 10, 10, 10, 10, 10, 10};  // �������ʾ����
unsigned char Seg_Point[8] = {0, 0, 0, 0, 0, 0, 0, 0};  			// С�������: 0-Ϩ�� 1-����

unsigned int Timer_1ms;
unsigned int Timer_400ms;

unsigned int Led_Time[] = {400, 400, 400, 400};  // Ledÿ��ģʽ��ѭ��ʱ��
unsigned int Led_Time_Set[4];  // LED����ʱ��
unsigned char Led_Time_Set_Index;  // LED����ʱ��ָ��
unsigned char Led_Data;

bit Sys_Run_Flag;  // ϵͳ������־ 0-δ���� 1-����
bit Set_Flag;  // 0-ģʽ��� 1-��תʱ��
bit Seg_Flashing_Flag;  // �������˸��־
bit Data_Disp_Mode;  // 0-��ʾ״̬ 1-��ʾ����

unsigned char Led_Mode;  // Led��ʾģʽ
unsigned char Seg_Disp_Mode;  // 0-����״̬���� 1-��תʱ�����ý��� 2-������ʾ����

// ����
void Key_Proc()
{
	unsigned char i;
	if (Key_Slow_Down) return;
	Key_Slow_Down = 1;
	
	Key_Val = Key_Read();  // ʵʱ��ȡ����ֵ
	Key_Down = Key_Val & (Key_Val ^ Key_Old);  // ��ȡ�½���
	Key_Up = ~Key_Val & (Key_Val ^ Key_Old);  // ��ȡ������
	Key_Old = Key_Val;  // ��������
	
	if (Sys_Run_Flag == 0)
	{
		if (Key_Old == 4 && Seg_Disp_Mode == 0)
		{
			Data_Disp_Mode = 1;
		}
		else
		{
			Data_Disp_Mode = 0;
		}
	}
	else
	{
		Data_Disp_Mode = 0;
	}
	
	switch (Key_Down)
	{
		case 4:  // ������
		{
			if (Seg_Disp_Mode == 1)
			{
				if (Set_Flag == 0)
				{
					if ( -- Led_Time_Set_Index == 255)
					{
						Led_Time_Set_Index = 3;
					}
				}
				else
				{
					Led_Time_Set[Led_Time_Set_Index] -= 100;
					if (Led_Time_Set[Led_Time_Set_Index] == 300)
					{
						Led_Time_Set[Led_Time_Set_Index] = 1200;
					}
				}
			}
			break;
		}
		case 5:  // ������
		{
			if (Seg_Disp_Mode == 1)
			{
				if (Set_Flag == 0)
				{
					if ( ++ Led_Time_Set_Index == 4)
					{
						Led_Time_Set_Index = 0;
					}
				}
				else
				{
					Led_Time_Set[Led_Time_Set_Index] += 100;
					if (Led_Time_Set[Led_Time_Set_Index] == 1300)
					{
						Led_Time_Set[Led_Time_Set_Index] = 400;
					}
				}
			}
			break;
		}
		case 6:  // ���ý���
		{
			if (Sys_Run_Flag == 0)
			{
				if (Seg_Disp_Mode == 0)
				{
					for (i = 0; i < 4; i ++ )
					{
						Led_Time_Set[i] = Led_Time[i];
					}
					Seg_Disp_Mode = 1;
				}
				else if (Seg_Disp_Mode == 1)
				{
					Set_Flag ^= 1;
					if (Set_Flag == 0)
					{
						for (i = 0; i < 4; i ++ )
						{
							Led_Time[i] = Led_Time_Set[i];
						}
						Seg_Disp_Mode = 0;
					}
				}
			}
			break;
		}
		case 7:  // ����/��ͣ
		{
			Sys_Run_Flag ^= 1;
			break;
		}
	}
}

// �����
void Seg_Proc() 
{
	unsigned char i;
	if (Seg_Slow_Down) return;
	Seg_Slow_Down = 1;
	
	for (i = 0; i < 8; i ++ )
	{
		Seg_Buf[i] = 10;
		Seg_Point[i] = 0;
	}
	
	switch (Seg_Disp_Mode)
	{
		case 0:  // ����״̬����
		{
			if (Data_Disp_Mode == 0)
			{
				Seg_Buf[0] = Sys_Run_Flag ? 11 : 5;
				Seg_Buf[1] = Led_Mode + 1;
				Seg_Buf[4] = Led_Time[Led_Mode] / 1000 % 10;
				Seg_Buf[5] = Led_Time[Led_Mode] / 100 % 10;
				Seg_Buf[6] = Led_Time[Led_Mode] / 10 % 10;
				Seg_Buf[7] = Led_Time[Led_Mode] % 10;
				Seg_Buf[4] = Seg_Buf[4] == 0 ? 10 : Seg_Buf[4];
			}
			else
			{
				Seg_Buf[0] = 13;
				Seg_Point[0] = 1;
				Seg_Buf[1] = 11;
				Seg_Point[1] = 1;
				
				Seg_Buf[2] = 12;
				Seg_Buf[3] = Led_Mode + 1;
				
				Seg_Buf[4] = Led_Data / 10 % 10;
				Seg_Buf[5] = Led_Data % 10;
			}
			break;
		}
		case 1:  // ��תʱ�����ý���
		{
			Seg_Buf[0] = 12;
			Seg_Buf[1] = Led_Time_Set_Index + 1;
			Seg_Buf[4] = Led_Time_Set[Led_Time_Set_Index] / 1000 % 10;
			Seg_Buf[5] = Led_Time_Set[Led_Time_Set_Index] / 100 % 10;
			Seg_Buf[6] = Led_Time_Set[Led_Time_Set_Index] / 10 % 10;
			Seg_Buf[7] = Led_Time_Set[Led_Time_Set_Index] % 10;
			Seg_Buf[4] = Seg_Buf[4] == 0 ? 10 : Seg_Buf[4];
			if (Set_Flag == 0)
			{
				Seg_Buf[0] = Seg_Flashing_Flag ? Seg_Buf[0] : 10;
				Seg_Buf[1] = Seg_Flashing_Flag ? Seg_Buf[1] : 10;
			}
			else
			{
				Seg_Buf[4] = Seg_Flashing_Flag ? Seg_Buf[4] : 10;
				Seg_Buf[5] = Seg_Flashing_Flag ? Seg_Buf[5] : 10;
				Seg_Buf[6] = Seg_Flashing_Flag ? Seg_Buf[6] : 10;
				Seg_Buf[7] = Seg_Flashing_Flag ? Seg_Buf[7] : 10;
			}
			break;
		}
	}
}

// LED
void Led_Proc() 
{
	unsigned char i;
	static unsigned char Led_Cnt = 0;
	if (Timer_1ms == Led_Time[Led_Mode])
	{
		Timer_1ms = 0;
		
		if (Led_Mode == 0 || Led_Mode == 1)
		{
			if (Led_Cnt == 8)
			{
				Led_Cnt = 0;
				Led_Mode = (Led_Mode + 1) % 4;
			}
		}
		else
		{
			if (Led_Cnt == 4)
			{
				Led_Cnt = 0;
				Led_Mode = (Led_Mode + 1) % 4;
			}
		}
		
		for (i = 0; i < 8; i ++ )
		{
			Led_Disp(i, 0);
		}
		
		switch (Led_Mode)
		{
			case 0:
			{
				Led_Disp(Led_Cnt, 1);
				Led_Data = Led_Cnt + 1;
				break;
			}
			case 1:
			{
				Led_Disp(7 - Led_Cnt, 1);
				Led_Data = 7 - Led_Cnt + 1;
				break;
			}
			case 2:
			{
				Led_Disp(Led_Cnt, 1);
				Led_Disp(7 - Led_Cnt, 1);
				Led_Data = (Led_Cnt + 1) * 10 + 7 - Led_Cnt + 1;
				break;
			}
			case 3:
			{
				Led_Disp(3 - Led_Cnt, 1);
				Led_Disp(4 + Led_Cnt, 1);
				Led_Data = (3 - Led_Cnt + 1) * 10 + 4 + Led_Cnt + 1;
				break;
			}
		}
		Led_Cnt ++ ;
	}
}

// ��ʱ��0�жϷ���
void Timer0_Isr() interrupt 1 
{
	static unsigned int Pos = 0;  // λ��ָ��
	
	Key_Slow_Down = (Key_Slow_Down + 1) % 10;  // ����ɨ�� 10ms һ��
	Seg_Slow_Down = (Seg_Slow_Down + 1) % 50;  // �����ɨ�� 50ms һ��
	Pos = (Pos + 1) % 8;
	
	Seg_Disp(Pos, Seg_Buf[Pos], Seg_Point[Pos]);
	
	if (Sys_Run_Flag)
	{
		Timer_1ms ++ ;
	} 
	else
	{
		Timer_1ms = 0;
	}
	
	if ( ++ Timer_400ms == 400) 
	{
		Timer_400ms = 0;
		Seg_Flashing_Flag ^= 1;
	}
}

// ��ʱ����ʼ��
void Timer0_Init(void)		// 1����@12.000MHz
{
	AUXR |= 0x80;			// ��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;			// ���ö�ʱ��ģʽ
	TL0 = 0x20;				// ���ö�ʱ��ʼֵ
	TH0 = 0xD1;				// ���ö�ʱ��ʼֵ
	TF0 = 0;				  // ���TF0��־
	TR0 = 1;					// ��ʱ��0��ʼ��ʱ
	ET0 = 1;					// ʹ�ܶ�ʱ��0�ж�
	EA = 1;					  // ���жϿ���
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
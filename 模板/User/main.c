#include "main.h"

unsigned char Key_Slow_Down, Seg_Slow_Down, Led_Slow_Down;  	// ���ٱ���
unsigned char Key_Val, Key_Old, Key_Down, Key_Up;  						// ����ɨ��
unsigned char Seg_Buf[8] = {10, 10, 10, 10, 10, 10, 10, 10};  // �������ʾ����
unsigned char Seg_Point[8] = {0, 0, 0, 0, 0, 0, 0, 0};  			// С�������: 0-Ϩ�� 1-����


void Key_Proc()
{
	if (Key_Slow_Down) return;
	Key_Slow_Down = 1;
	
	Key_Val = Key_Read();  											// ʵʱ��ȡ����ֵ
	Key_Down = Key_Val & (Key_Val ^ Key_Old);  	// ��ȡ�½���
	Key_Up = ~Key_Val & (Key_Val ^ Key_Old);  	// ��ȡ������
	Key_Old = Key_Val;  												// ��������
	
	
}

void Seg_Proc() 
{
	if (Seg_Slow_Down) return;
	Seg_Slow_Down = 1;
	
	
}

void Led_Proc() 
{
	unsigned char i;
	if (Led_Slow_Down) return;
	Led_Slow_Down = 1;
	
	for (i = 0; i < 8; i ++ )
	{
		Led_Disp(i, 0);
	}

	
}

void Timer0_Isr() interrupt 1 
{
	static unsigned int Pos = 0;  // λ��ָ��
	
	Key_Slow_Down = (Key_Slow_Down + 1) % 10;  // Key����Ƶ�� 10ms
	Seg_Slow_Down = (Seg_Slow_Down + 1) % 2;   // Seg����Ƶ��  2ms
	Led_Slow_Down = (Led_Slow_Down + 1) % 2;   // LED����Ƶ��  2ms
	Pos = (Pos + 1) % 8;
	
	Seg_Disp(Pos, Seg_Buf[Pos], Seg_Point[Pos]);
	
	
}

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
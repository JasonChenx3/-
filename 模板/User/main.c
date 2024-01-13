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
unsigned char Led_State[8] = {0, 0, 0, 0, 0, 0, 0, 0};  			// LED����: 0-Ϩ�� 1-����

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
	
}

// LED
void Led_Proc() 
{
	
}

// ��ʱ��0�жϷ���
void Timer0_Isr() interrupt 1 
{
	static unsigned int Pos = 0;  // λ��ָ��
	
	Key_Slow_Down = (Key_Slow_Down + 1) % 10;  // ����ɨ�� 10ms һ��
	Seg_Slow_Down = (Seg_Slow_Down + 1) % 50;  // �����ɨ�� 50ms һ��
	Pos = (Pos + 1) % 8;
	
	Seg_Disp(Pos, Seg_Buf[Pos], Seg_Point[Pos]);
	Led_Disp(Pos, Led_State[Pos]);
	
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
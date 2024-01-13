#include <LED.H>

void Led_Disp(unsigned char addr, enable)
{
	static unsigned char temp = 0x00;
	static unsigned char temp_old = 0xFF;
	
	if (enable) temp |= 0x01 << addr;
	else temp &= ~(0x01 << addr);
	
	// ����������ˢ��
	if (temp != temp_old)
	{
		P0 = ~temp;  // ������Ӧ��LED
		P2 = P2 & 0x1F | 0x80;  // ѡ��LEDͨ��
		P2 &= 0x1F;  // �ر�LEDͨ�� 
		temp_old = temp;  // ������ֵ�����
	}
}
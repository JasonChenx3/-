#include <SYS.H>

void Sys_Init()
{
	// �ر�LED��
	P0 = 0xFF;
	P2 = P2 & 0x1F | 0x80;
	P2 &= 0x1F;
	
	// �رշ������ͼ̵���
	P0 = 0x00;
	P2 = P2 & 0x1F | 0xA0;
	P2 &= 0x1F;
}
//������ʵ�ֶ��ⲿ�ж�0�жϴ�����������ʾ
#include "reg52.h"
#include "absacc.h"
unsigned char code duan_code_ca[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff}; //��������ܶ����0~9��ȫϨ�������������Ϊ�������������Ž�����ܶ������ţ��ʿ��ù�������
unsigned char disp_buf[8];
unsigned int dat;
void disp();
void Timer0_Isr(void) interrupt 1
{
	TL0 = 0x30;				//���ö�ʱ��ʼֵ
	TH0 = 0xF8;				//���ö�ʱ��ʼֵ�������й���Ϊ���¼��ض�ʱ����
	disp();//����һλ�������ʾ������ʾһλ����
}

void Timer0_Init(void)		//2����@12.000MHz
{
	//AUXR &= 0x7F;			//��ʱ��ʱ��12Tģʽ  ע�⣺ʵ�������õ�Ƭ���Ķ�ʱ��ʱ��ֻ��12Tģʽ����û�и����Ĵ���AURX��������STC��������������ʱ����ʼ������Ҫȥ������
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TMOD |= 0x01;			//���ö�ʱ��ģʽ   ע�⣺��ʱ������ģʽ1Ϊ16λ��ʽ�����õ�Ƭ�����Ķ�ʱ����16λ�Զ���װ��ģʽ�����ڶ�ʱ���жϷ�����������װ��ʱ����
	TL0 = 0x30;				//���ö�ʱ��ʼֵ
	TH0 = 0xF8;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
	ET0 = 1;				//ʹ�ܶ�ʱ��0�ж�
}

//
void disp_buf_change(unsigned char one,two,three,four,five,six)
{
	disp_buf[0]=duan_code_ca[one];
	disp_buf[1]=duan_code_ca[two];
	disp_buf[2]=duan_code_ca[three];
	disp_buf[3]=duan_code_ca[four];
	disp_buf[4]=duan_code_ca[five];
	disp_buf[5]=duan_code_ca[six];
}

//
void disp()
{
	static unsigned char disp_com;
	P1=0xff;//����
	XBYTE[0x9000]=disp_buf[disp_com];
	P1=~(32>>disp_com);//1<<disp_comΪ1����1��7����0�����������Ϊ�����������������7ֻ������һֻ��������λ�����������ܱ�ֻ֤��ָ��λ���ˡ�;
	disp_com=(disp_com+1)%7;
}

void int0_init()
{
	IT0=1;//�ⲿ�ж�0�趨Ϊ�½��ش���
	PX0=1;//�ⲿ�ж�0��Ϊ�����ȼ�
	EX0=1;//�ⲿ�ж�0�ж�����
}

void int0_isr() interrupt 0
{
	dat+=1;
	disp_buf_change(dat>=100000?dat/100000:10,dat>=10000?dat%100000/10000:10,dat>=1000?dat%10000/1000:10,dat>=100?dat%1000/100:10,dat>=10?dat%100/10:10,dat%10);//
}

void int1_init()
{
	IT1=1;//�ⲿ�ж�0�趨Ϊ�½��ش���
	PX1=1;//�ⲿ�ж�0��Ϊ�����ȼ�
	EX1=1;//�ⲿ�ж�0�ж�����
}

void int1_isr() interrupt 2
{
	dat-=1;
	disp_buf_change(dat>=100000?dat/100000:10,dat>=10000?dat%100000/10000:10,dat>=1000?dat%10000/1000:10,dat>=100?dat%1000/100:10,dat>=10?dat%100/10:10,dat%10);//
}

//
void main()
{
	Timer0_Init();//��ʱ��0��ʼ��
	int0_init();//�ⲿ�ж�0��ʼ��
	int1_init();
	EA=1;//�ܵ��ж�����
	disp_buf_change(dat>=100000?dat/100000:10,dat>=10000?dat%100000/10000:10,dat>=1000?dat%10000/1000:10,dat>=100?dat%1000/100:10,dat>=10?dat%100/10:10,dat%10);//��ʼ��ʾ0
	while(1);
}
	
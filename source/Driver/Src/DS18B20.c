#include <string.h>
#include "..\inc\44b.h"
#include "DS18B20.h"

/*---------------------------------------------------------------------------------------------------
 ���ܣ�ʵ�ֶ�DS18B20�Ķ�ȡ
 ԭ��������Э��
 ע�⣺������Э�����ʱҪ��Ƚ��ϸ񣬴˳����в��õ���11.0592M�ľ������ʹ�������ľ��������DS18B20�������޸���ʱ����
 �汾��1.4b
 ����޸�ʱ�䣺2004��11��8��
 �����ˣ��ϴ���
 ��Ȩ���������ڰ����������޹�˾  www.hitzbl.com   ��Ƭ����������www.mcuzb.com
====================================================================================================*/

#define sPDATAC    (*(volatile U32 *)0x01d20014)
#define sPCONC	   (*(volatile U32 *)0x01d20010)

static int sPC0Mask   = 0xF4000055;
static int sPDATAMask = 0x00;

static void nSetIOPC0( char bOut )
{
	sPC0Mask &= 0xFFFFFFFC;
	
	sPC0Mask |= bOut ? 0x01 : 0x00;
	
	sPCONC = sPC0Mask;	
}

static void nWriteDQ( unsigned char val )
{
	nSetIOPC0( 1 );
	
	sPDATAMask &= 0xFFFFFFFE;
	sPDATAMask |= (val & 0x01);
	
	sPDATAC = sPDATAMask;
}

static unsigned char nReadDQ()
{
	unsigned char aVal = 0;
	
	nSetIOPC0( 0 );
	
	aVal = sPDATAC;
	
	return aVal & 0x01;
}

static void ndelay( int s )
{
	while ( s-- );
}

 unsigned char a;
void TestPC0()
{
 	nSetIOPC0( 1 );
 while ( 1 )
 {
 	nSetIOPC0( 0 );
 	a = sPDATAC;
 }
 
}

//��ʼ������
void Init_DS18B20(void)
{
	 static unsigned char x=0;
	
	 nWriteDQ( 1 );    //DQ��λ
	 ndelay(500);  //������ʱ
	
	 nWriteDQ( 0 );    //��Ƭ����DQ����
	 ndelay(500); //��ȷ��ʱ ���� 480us
	
	 //nWriteDQ( 1 );    //��������
	 nSetIOPC0(0 );
	 //ndelay(500);
	
	 x = nReadDQ();      //������ʱ�� ���x=0���ʼ���ɹ� x=1���ʼ��ʧ��
	 ndelay(150);
}

//��һ���ֽ�
unsigned char ReadOneChar(void)
{
	unsigned char i=0;
	unsigned char dat = 0;
	
	for (i=8;i>0;i--)
	 {
	    nWriteDQ(1 );
	    ndelay(500);

	  nWriteDQ( 0 ); // �������ź�
	  dat>>=1;
	  ndelay(100);
	  nSetIOPC0( 0 ); // �������ź�
	 
	  ndelay(200);
	  if ( nReadDQ() )
	   dat|=0x80;
	  ndelay(450);
	 }
	 
	 return(dat);
}

//дһ���ֽ�
void WriteOneChar(unsigned char dat)
{
	 unsigned char i=0;
	 for (i=8; i>0; i--)
	 {
	    nWriteDQ(1 );
	    ndelay(500);
	    
	 	if ( dat&0x01 )
	 	{
			nWriteDQ( 0 );
			ndelay(200);
			
			nSetIOPC0( 0 );
			ndelay(450);
		}
		else
		{
			nWriteDQ( 0 );			
			ndelay(600);
			
			nSetIOPC0( 0 );			
			ndelay(100);		
		}
	
		dat>>=1;
	 }
	
}

//��ȡ�¶�
double ReadTemperature(void)
{
	unsigned char a=0;
	unsigned char b=0;
	unsigned int t=0;
	double tt=0;
	
	Init_DS18B20();
	WriteOneChar(0xCC); // ����������кŵĲ���
	WriteOneChar(0x44); // �����¶�ת��
	
	Init_DS18B20();
	WriteOneChar(0xCC); //����������кŵĲ���
	WriteOneChar(0xBE); //��ȡ�¶ȼĴ����ȣ����ɶ�9���Ĵ����� ǰ���������¶�
	
	a = ReadOneChar();
	b = ReadOneChar();
	
	t=b;
	t<<=8;
	t=t|a;
	tt=t*0.0625;
	//t= tt*10+0.5; //�Ŵ�10���������������---����û��
	
	return(tt);
}

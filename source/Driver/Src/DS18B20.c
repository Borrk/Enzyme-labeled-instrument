#include <string.h>
#include "..\inc\44b.h"
#include "DS18B20.h"

/*---------------------------------------------------------------------------------------------------
 功能：实现对DS18B20的读取
 原理：单总线协议
 注意：单总线协议对延时要求比较严格，此程序中采用的是11.0592M的晶振，如果使用其他的晶振请跟据DS18B20的资料修改延时参数
 版本：1.4b
 最后修改时间：2004年11月8号
 开发人：鞠春阳
 版权：哈尔滨众邦龙开发有限公司  www.hitzbl.com   单片机坐标网：www.mcuzb.com
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

//初始化函数
void Init_DS18B20(void)
{
	 static unsigned char x=0;
	
	 nWriteDQ( 1 );    //DQ复位
	 ndelay(500);  //稍做延时
	
	 nWriteDQ( 0 );    //单片机将DQ拉低
	 ndelay(500); //精确延时 大于 480us
	
	 //nWriteDQ( 1 );    //拉高总线
	 nSetIOPC0(0 );
	 //ndelay(500);
	
	 x = nReadDQ();      //稍做延时后 如果x=0则初始化成功 x=1则初始化失败
	 ndelay(150);
}

//读一个字节
unsigned char ReadOneChar(void)
{
	unsigned char i=0;
	unsigned char dat = 0;
	
	for (i=8;i>0;i--)
	 {
	    nWriteDQ(1 );
	    ndelay(500);

	  nWriteDQ( 0 ); // 给脉冲信号
	  dat>>=1;
	  ndelay(100);
	  nSetIOPC0( 0 ); // 给脉冲信号
	 
	  ndelay(200);
	  if ( nReadDQ() )
	   dat|=0x80;
	  ndelay(450);
	 }
	 
	 return(dat);
}

//写一个字节
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

//读取温度
double ReadTemperature(void)
{
	unsigned char a=0;
	unsigned char b=0;
	unsigned int t=0;
	double tt=0;
	
	Init_DS18B20();
	WriteOneChar(0xCC); // 跳过读序号列号的操作
	WriteOneChar(0x44); // 启动温度转换
	
	Init_DS18B20();
	WriteOneChar(0xCC); //跳过读序号列号的操作
	WriteOneChar(0xBE); //读取温度寄存器等（共可读9个寄存器） 前两个就是温度
	
	a = ReadOneChar();
	b = ReadOneChar();
	
	t=b;
	t<<=8;
	t=t|a;
	tt=t*0.0625;
	//t= tt*10+0.5; //放大10倍输出并四舍五入---此行没用
	
	return(tt);
}

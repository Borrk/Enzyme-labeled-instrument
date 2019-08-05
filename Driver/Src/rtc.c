#include "rtc.h"
#include "44b.h"
#include "44blib.h"


void __irq rtc_Tick(void);
void __irq rtc_Int(void);

volatile unsigned char HaveRTC=0;
volatile unsigned char isRtcInt;


typedef enum {
	RTC_ENABLE,
	RTC_DISABLE
} RTC_ACCESS;


static  void SetRTC_Access(RTC_ACCESS a)
{
	switch (a) 
	{
	case RTC_ENABLE:
		rRTCCON |= 0x01; 
		break;

	case RTC_DISABLE:
		rRTCCON &= ~0x01; 
		break;
	}
}

static unsigned int  bcd2bin (unsigned char n)
{
	return ((((n >> 4) & 0x0F) * 10) + (n & 0x0F));
}

static unsigned char bin2bcd (unsigned int n)
{
	return (((n / 10) << 4) | (n % 10));
}

/* ------------------------------------------------------------------------- */


void rtc_get (struct rtc_time *tmp)
{
	unsigned char sec, min, hour, mday, wday, mon, year;
	unsigned char  a_sec,a_min, a_hour, a_date, a_mon, a_year, a_armed;

	/* enable access to RTC registers */
	SetRTC_Access(RTC_ENABLE);

	/* read RTC registers */
	do {

		sec	= rBCDSEC;
		min	= rBCDMIN;
		hour	= rBCDHOUR;
		mday	= rBCDDAY ;
		wday	= rBCDDATE;
		mon	= rBCDMON;
		year	= rBCDYEAR;
	} while ( sec != rBCDSEC );

	/* disable access to RTC registers */
	SetRTC_Access(RTC_DISABLE);


	tmp->tm_sec  = bcd2bin(sec  & 0x7F);
	tmp->tm_min  = bcd2bin(min  & 0x7F);
	tmp->tm_hour = bcd2bin(hour & 0x3F);
	tmp->tm_mday = bcd2bin(mday & 0x3F);
	tmp->tm_mon  = bcd2bin(mon & 0x1F);
	tmp->tm_year = bcd2bin(year);
	tmp->tm_wday = bcd2bin(wday & 0x07);
	if(tmp->tm_year<70)
		tmp->tm_year+=2000;
	else
		tmp->tm_year+=1900;
	tmp->tm_yday = 0;
	tmp->tm_isdst= 0;
}

void rtc_set (struct rtc_time *tmp)
{
	unsigned char sec, min, hour, mday, wday, mon, year;

	year	= bin2bcd(tmp->tm_year % 100);
	mon	= bin2bcd(tmp->tm_mon);
	wday	= bin2bcd(tmp->tm_wday);
	mday	= bin2bcd(tmp->tm_mday);
	hour	= bin2bcd(tmp->tm_hour);
	min	= bin2bcd(tmp->tm_min);
	sec	= bin2bcd(tmp->tm_sec);

	/* enable access to RTC registers */
	SetRTC_Access(RTC_ENABLE);

	/* write RTC registers */

	rBCDSEC	 = sec;
	rBCDMIN	 = min;
	rBCDHOUR = hour;
	rBCDDAY = mday;
	//rBCDDATE	 = wday;
	rBCDMON	 = mon;
	rBCDYEAR = year;

	/* disable access to RTC registers */
	SetRTC_Access(RTC_DISABLE);
}

void rtc_reset (void)
{
	rRTCCON = (rRTCCON & ~0x06) | 0x08;
	rRTCCON &= ~(0x08|0x01);
}


void Rtc_Init(void)
{
    rRTCCON = 0x01;	// R/W enable, 1/32768, Normal(merge), No reset

/*
    rBCDYEAR = 0x05;
    rBCDMON  = 0x06;
    rBCDDAY  = 0x01;	
    rBCDDATE = 0x03;     // SUN:1 MON:2 TUE:3 WED:4 THU:5 FRI:6 SAT:7
    rBCDHOUR = 0x08;
    rBCDMIN  = 0x00;
    rBCDSEC  = 0x00;
*/    
    pISR_TICK=(unsigned int)rtc_Tick;
    pISR_RTC=(unsigned int)rtc_Int;
    rTICINT = 0x7f | (0x01<<7);
   // rINTMSK &= ~(BIT_TICK); 
}

/* ------------------------------------------------------------------------- */


void __irq rtc_Tick(void)
{
    HaveRTC=1; 
    rI_ISPC=BIT_TICK;
}

void __irq rtc_Int(void)
{
    isRtcInt=1;  
    rI_ISPC=BIT_RTC;   
}

///////////////////////////////////////////////////////////////////

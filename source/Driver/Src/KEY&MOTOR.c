#include "44blib.h"
#include "44b.h"
#include "key&Motor.h"
#include "rtc.h"


#define  KEY_IN_PORT (*(volatile unsigned char *)0x6000009)
#define  KEY_OUT_PORT (*(volatile unsigned char *)0x6000008)
unsigned char KEY_OUT_PORT_MIRROR=0xff;

#define  MOTOR_KEY_PORT (*(volatile unsigned char *)0x6000000)
#define  MOTOR_START_POS_PORT (*(volatile unsigned char *)0x600000b)
	#define MOTOR_WAVE_DIRECTION	    (1<<4)
	#define MOTOR_WAVE_STEP	   			(1<<5)
	#define MOTOR_PLATE_DIRECTION	   	(1<<6)
	#define MOTOR_PLATE_STEP	   		(1<<7)

unsigned char MOTOR_KEY_PORT_MIRROR=0xff;

/// Lamp control port
#define  LAMP_CTRL_PORT (*(volatile unsigned char *)0x600000a)
#define	LAMP_CTRL 	 	0x01
#define LAMP_OPTION 	0x02

unsigned char LAMP_CTRL_PORT_MIRROR=0xff;

void LampOn( )
{
	LAMP_CTRL_PORT = ~LAMP_CTRL;
}

void LampOff( )
{
	LAMP_CTRL_PORT |= LAMP_CTRL;
}


#if 1

static unsigned short  nKeyMappingCode[] = 
{
	SCANCODE_KEY_0_N 	,	KEY_0_N 	,
	SCANCODE_KEY_1_A 	,	KEY_1_A 	,
	SCANCODE_KEY_2_B 	,	KEY_2_B 	,
	SCANCODE_KEY_3_C 	,	KEY_3_C 	,
	SCANCODE_KEY_4_D 	,	KEY_4_D 	,
	SCANCODE_KEY_5_E 	,	KEY_5_E 	,
	SCANCODE_KEY_6_F 	,	KEY_6_F 	,
	SCANCODE_KEY_7_G 	,	KEY_7_G 	,
	SCANCODE_KEY_8_H 	,	KEY_8_H 	,
	SCANCODE_KEY_9_P 	,	KEY_9_P 	,
	SCANCODE_KEY_Up 	,	KEY_Up 	    ,
	SCANCODE_KEY_Down	,	KEY_Down	,
	SCANCODE_KEY_Right	,	KEY_Right	,
	SCANCODE_KEY_Left	,	KEY_Left	,
	SCANCODE_KEY_OK		,	KEY_GO		,
	SCANCODE_KEY_ESC	,	KEY_ESC	    ,
	SCANCODE_KEY_TEST   ,	KEY_OK, //KEY_TEST    ,
	SCANCODE_KEY_PRG	,	KEY_PRG     ,
	SCANCODE_KEY_Par	,	KEY_Par	    ,
	SCANCODE_KEY_Data	,	KEY_Data	,
	SCANCODE_KEY_Prn	,	KEY_Prn	    ,
	SCANCODE_KEY_FUN1	,	KEY_FUN2	,
	SCANCODE_KEY_FUN2	,	KEY_FUN1	,
	SCANCODE_KEY_FUN3	,	KEY_FUN3	,
	SCANCODE_KEY_FUN4	,	KEY_FUN4	,
	SCANCODE_KEY_Sign	,	KEY_Sign	,
	SCANCODE_KEY_DOT_L	,	KEY_DOT_L
	
};

#else
static unsigned short  nKeyMappingCode[] = 
{
	SCANCODE_KEY_0_N 	,	KEY_0_N 	,
	SCANCODE_KEY_1_A 	,	KEY_1_A 	,
	SCANCODE_KEY_2_B 	,	KEY_2_B 	,
	SCANCODE_KEY_3_C 	,	KEY_3_C 	,
	SCANCODE_KEY_4_D 	,	KEY_4_D 	,
	SCANCODE_KEY_5_E 	,	KEY_5_E 	,
	SCANCODE_KEY_6_F 	,	KEY_6_F 	,
	SCANCODE_KEY_7_G 	,	KEY_7_G 	,
	SCANCODE_KEY_8_H 	,	KEY_8_H 	,
	SCANCODE_KEY_9_P 	,	KEY_9_P 	,
	SCANCODE_KEY_Up 	,	KEY_Up 	    ,
	SCANCODE_KEY_Down	,	KEY_Down	,
	SCANCODE_KEY_Right	,	KEY_Right	,
	SCANCODE_KEY_Left	,	KEY_Left	,
	SCANCODE_KEY_OK		,	KEY_OK		,
	SCANCODE_KEY_ESC	,	KEY_ESC	    ,
	SCANCODE_KEY_TEST   ,	KEY_TEST    ,
	SCANCODE_KEY_PRG	,	KEY_PRG	    ,
	SCANCODE_KEY_Par	,	KEY_Par	    ,
	SCANCODE_KEY_Data	,	KEY_Data	,
	SCANCODE_KEY_Prn	,	KEY_Prn	    ,
	SCANCODE_KEY_FUN1	,	KEY_FUN1	,
	SCANCODE_KEY_FUN2	,	KEY_FUN2	,
	SCANCODE_KEY_FUN3	,	KEY_FUN3	,
	SCANCODE_KEY_FUN4	,	KEY_FUN4	,
	SCANCODE_KEY_Sign	,	KEY_Sign	,
	SCANCODE_KEY_DOT_L	,	KEY_DOT_L
	
};
#endif

unsigned short GetMapCode ( unsigned short keycode )
{
	short i;
	
	for ( i = 0; i < sizeof(nKeyMappingCode) / sizeof(nKeyMappingCode[0]); i++ )
	{
		if ( nKeyMappingCode[i*2] == keycode )
		{
			return nKeyMappingCode[i*2+1];
		}
	}
	
	return 0xFFFF;
}

static void nResetTm( struct rtc_time* oldkeyTime )
{
	oldkeyTime->tm_sec = 0;
	oldkeyTime->tm_min = 0;
	oldkeyTime->tm_hour= 0;
	oldkeyTime->tm_mday= 0;
	oldkeyTime->tm_mon = 0;
	oldkeyTime->tm_year= 0;
}

static long nTimeInterval( struct rtc_time*  keyTime, struct rtc_time* oldkeyTime )
{
	long t1, t2;
	
	t1 = oldkeyTime->tm_sec + oldkeyTime->tm_min *60 + oldkeyTime->tm_hour * 3600 + oldkeyTime->tm_mday * 3600 * 24;
	t2 = keyTime->tm_sec + keyTime->tm_min *60 + keyTime->tm_hour * 3600 +keyTime->tm_mday * 3600 * 24;
	
	return t2 - t1;
	
}

unsigned short nScanKey(void) 
{
	unsigned char Key_Val, i;
	unsigned short key;

	for ( i = 0; i < 4; i++ )
	{
		KEY_OUT_PORT=  ~(0x01 << i ); //KEY_OUT_PORT_MIRROR;
		Delay(10);
		Key_Val=~(KEY_IN_PORT);
		
		if ( Key_Val != 0 )
		{
			while(KEY_IN_PORT!=0xff)
			{
				Delay( 1 );
			}
			key = (0x01 << (i+8) | Key_Val );	
			
			beep(100);
					
			return key;
		}	
	}

	return 0xffff;
}

unsigned short Scan_KEY(void)
{
	unsigned short 	Return_KEY;
	unsigned short  key;
	static unsigned short oldKey = 0xFFFF;
	static struct rtc_time oldkeyTime, keyTime;
	static unsigned char keyCnt = 0;

	long l;
	
	key = nScanKey();
	if ( key != 0xFFFF )
	{
		rtc_get(&keyTime );
		
		if ( key == oldKey )
		{
			l = nTimeInterval( &keyTime, &oldkeyTime );
			if ( l < 2 )
			{
				keyCnt++;
				//oldKey = 0xFFFF;
			}	
			else
			{
				keyCnt = 0;	
				oldKey = key;
			}	
		}
		else
		{
			keyCnt = 0;
			oldKey = key;
		}
		
		
		oldkeyTime = keyTime;
		Return_KEY = GetMapCode( key ) | (keyCnt << 8);
	}
	else
	{
		Return_KEY = 0xFFFF;
	}
	
	return 	Return_KEY;
}

/*
unsigned short Scan_KEY(void)
{
	unsigned short 	Return_KEY;
	unsigned short  key;
	
	key = nScanKey();
	if ( key != 0xFFFF )
	{
		Return_KEY = (unsigned char) GetMapCode( key );
	}
	else
	{
		Return_KEY = 0xFFFF;
	}
	
	return 	Return_KEY;
}
/*/


/// Motor Control routines
#if 1

void Plate_Motor_On( char bOn )
{  
	if ( bOn == 1 )
		MOTOR_KEY_PORT_MIRROR= MOTOR_KEY_PORT_MIRROR & (~0x04);	
	else
		MOTOR_KEY_PORT_MIRROR=MOTOR_KEY_PORT_MIRROR | 0x04;	
		
	MOTOR_KEY_PORT=MOTOR_KEY_PORT_MIRROR;
}

//void Plate_Motor_CCW_ONE_STEP(void)
void Plate_Motor_CW_ONE_STEP(void)
{ 
	/// Set direction
	MOTOR_KEY_PORT_MIRROR=MOTOR_KEY_PORT_MIRROR | 0x08;
	
	/// Run Motor
	MOTOR_KEY_PORT_MIRROR=MOTOR_KEY_PORT_MIRROR | 0x10;
	MOTOR_KEY_PORT=MOTOR_KEY_PORT_MIRROR;
	Delay(10);
	MOTOR_KEY_PORT_MIRROR=MOTOR_KEY_PORT_MIRROR & 0xEF;	
	MOTOR_KEY_PORT=MOTOR_KEY_PORT_MIRROR;
	Delay(20);	
}

//void Plate_Motor_CW_ONE_STEP(void)
void Plate_Motor_CCW_ONE_STEP(void)
{ 
	/// Set direction
	MOTOR_KEY_PORT_MIRROR=MOTOR_KEY_PORT_MIRROR & 0xF7;
	
	/// Run Motor
	MOTOR_KEY_PORT_MIRROR=MOTOR_KEY_PORT_MIRROR | 0x10;
	MOTOR_KEY_PORT=MOTOR_KEY_PORT_MIRROR;
	Delay(10);
	MOTOR_KEY_PORT_MIRROR=MOTOR_KEY_PORT_MIRROR & 0xeF;	
	MOTOR_KEY_PORT=MOTOR_KEY_PORT_MIRROR;
	Delay(20);	
}

unsigned char Is_Plate_Motor_Reset(void)
{
	unsigned char aVal;
	aVal = MOTOR_START_POS_PORT;
	aVal = (aVal&0x01 == 1 ? 0 : 1);
	
	return aVal;	
}

unsigned char Is_Light_Motor_Reset(void)
{
	unsigned char aVal;
	aVal = MOTOR_START_POS_PORT;
	aVal = ( (aVal&0x02) ? 0 : 1);
	
	return aVal;	
}

void Light_Motor_On( char bOn )
{  
	if ( bOn == 1 )
		MOTOR_KEY_PORT_MIRROR= MOTOR_KEY_PORT_MIRROR & (~0x20 );	
	else
		MOTOR_KEY_PORT_MIRROR=MOTOR_KEY_PORT_MIRROR | 0x20;	
		
	MOTOR_KEY_PORT=MOTOR_KEY_PORT_MIRROR;
}


void Light_Motor_CCW_ONE_STEP(void)
{ 
	MOTOR_KEY_PORT_MIRROR=MOTOR_KEY_PORT_MIRROR | 0x40;
	MOTOR_KEY_PORT_MIRROR=MOTOR_KEY_PORT_MIRROR | 0x80;
	MOTOR_KEY_PORT=MOTOR_KEY_PORT_MIRROR;
	Delay(20);
	MOTOR_KEY_PORT_MIRROR=MOTOR_KEY_PORT_MIRROR & 0x7F;	
	MOTOR_KEY_PORT=MOTOR_KEY_PORT_MIRROR;
	Delay(20);	
}

void Light_Motor_CW_ONE_STEP(void)
{  
	MOTOR_KEY_PORT_MIRROR=MOTOR_KEY_PORT_MIRROR & 0xBF;
	MOTOR_KEY_PORT_MIRROR=MOTOR_KEY_PORT_MIRROR | 0x80;
	MOTOR_KEY_PORT=MOTOR_KEY_PORT_MIRROR;
	Delay(20);
	MOTOR_KEY_PORT_MIRROR=MOTOR_KEY_PORT_MIRROR & 0x7F;	
	MOTOR_KEY_PORT=MOTOR_KEY_PORT_MIRROR;
	Delay(20);	
}

#else
void Plate_Motor_CW_ONE_STEP(void)
{
}
unsigned char Is_Light_Motor_Reset(void)
{
	return 1;	
}
unsigned char Is_Plate_Motor_Reset(void)
{
	return 1;	
}


void Plate_Motor_CCW_ONE_STEP(void)
{
}
void Light_Motor_CW_ONE_STEP(void)
{
}

void Light_Motor_CCW_ONE_STEP(void)
{
}

#endif



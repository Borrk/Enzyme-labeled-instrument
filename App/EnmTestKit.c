#include "EnmTestKit.h"

#include "EnmCfg.h"
#include "EnmApi.h"
#include "EnmFs.h"
#include "FlashMng.h"
#include "..\inc\DS18B20.h"
#include "..\inc\km29ux.h"

/////////////////////////////////////////////////////////////////////////////////////
//   Motor Test
//  
/////////////////////////////////////////////////////////////////////////////////////
extern void LampOn( );
extern void LampOff( );

extern void TestPC0();

void TestTempSensor()
{

	double aTemp = 0;
	
	//TestPC0();
	 
	while ( 1 )
	{
		aTemp = ReadTemperature();
	}
}

void TestSerailPort()
{
	unsigned char ch;
	
	Uart_Init( 0, 4800 );
	Uart_Select( 0 );
	
	ch = 0x55; 
	while ( 1 )
	{
		Uart_SendByte( ch );
	}	
	
}
 

void TestPlateTripmeter()
{
	long cnt = 0;
#if 1
	Plate_Motor_On( 1 );
//*	
	for ( cnt = 0; cnt < 200; cnt++ )
	{
		 LampOn();
		 EnmDelay( 50);
		 LampOff();
	}
	
	for ( cnt = 0; cnt < 400; cnt++ )
	{
		 Plate_Motor_CW_ONE_STEP();
	}
//*/

	for ( cnt = 0; cnt < 100; cnt++ )
	{		
		 beep( 5 );
	}

	while (1)
	{	
#ifdef  ENM_VER_TARGET
		if ( EnmRun(Is_Plate_Motor_Reset()) )
#endif
		{		 
		  break;
		}	
			
		EnmRun( Plate_Motor_CCW_ONE_STEP() );
		EnmDelay(20);
		cnt++;
	}
	
	Plate_Motor_On(0);
#endif	
	PutNum16x16( 100, 300, cnt, 0, CBLACK, CWHITE );
	while ( 1);
		
}

///  Optical motor reset 
static char  FilterReset( char bBack )
{
	int  i;
	char aRet = 1;
	unsigned short cnt = 0;
#if 1		
	Light_Motor_On( 1 );
	
	/// move forward first
	if ( bBack )
	{
		for ( i = 0; i < 80; i++ )
		{
			EnmRun(Light_Motor_CW_ONE_STEP());
		}
		EnmDelay( 100 );
	}
	
	while (1)
	{	
#ifdef  ENM_VER_TARGET
		if ( EnmRun(Is_Light_Motor_Reset()) )
#endif
		{		 
		  break;
		}	
			
		EnmRun(Light_Motor_CCW_ONE_STEP());
		/// if step larger than max step, quit
	}	
#endif
		
	return aRet;	
}

void TestLightTripmeter()
{
	int i;
	static long cnt = 0;
#if 1	
	Light_Motor_On( 1 );
	
	for ( cnt = 0; cnt < 200; cnt++ )
	{
	//	 Light_Motor_CW_ONE_STEP();
	}
	
	for ( cnt = 0; cnt < 200; cnt++ )
	{
	//	 Light_Motor_CCW_ONE_STEP();
	}

	//FilterReset(1);
	
	/// Reset Position
	cnt = 0;
	while (1)
	{	
#ifdef  ENM_VER_TARGET
		if ( EnmRun(Is_Light_Motor_Reset()) )
#endif
		{		 
		  break;
		}	
			
		EnmRun( Light_Motor_CCW_ONE_STEP() );
		cnt++;
	}

	/// backword 10 step
	for ( i = 0; i < 10; i++ )
		EnmRun( Light_Motor_CCW_ONE_STEP() );
		
	cnt = 100;
	
	while (1)
	{	
#ifdef  ENM_VER_TARGET
		if ( EnmRun(Is_Light_Motor_Reset()) )
#endif
		{		 
		  break;
		}	
			
		EnmRun( Light_Motor_CCW_ONE_STEP() );
		cnt++;
	}
	
	PutNum16x16( 100, 300, cnt, 0, CBLACK, CWHITE );
#endif		
}

void TestPlateAPI()
{
	static int i;
#if 0
	/// reset
	EnmPlateReset(1);
	
	/// PreScan pos
	EnmPlatePrePos();
	
	/// Step 
	for ( i = 0; i < 12; i++ )
	{
		EnmPlateForwardOne();
	}
	
	/// test post
	for ( i = 0; i < 100; i++ )
	{
		EnmRun( Plate_Motor_CW_ONE_STEP() );
	}
#endif
}

void TestLightMotorAPI()
{
	static int i;
#if 0	
	/// reset
	EnmFilterReset(1);
	
	/// PreScan pos
	EnmFilterPrePos();
	
	/// Step 
	for ( i = 0; i < 8; i++ )
	{
		EnmFilterPos(i);
	}
	
	/// test post
	for ( i = 0; i < 100; i++ )
	{
		EnmRun( Plate_Motor_CW_ONE_STEP() );
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////////////
//   FS Test
//  
/////////////////////////////////////////////////////////////////////////////////////
unsigned char nGetKey( )
{
	unsigned char key = 0xff;

	while ( key  == 0xff )
	{
		key =  Scan_KEY();
	}
	
	return key;
}

char nMemCmp( unsigned char* pMem1, unsigned char *pMem2, unsigned long cnt )
{
	char bSame = 0;
	unsigned long i;
	
	for ( i = 0; i < cnt; i++ )
	{
		if ( pMem1[i] != pMem2[i] )
		{
			bSame = i;
			break;
		}
	}
	
	return bSame;
}

void TestFlashDrv()
{
	char bCheck = 0;
	int i, j;
	unsigned char key;
	
	unsigned short  buf[1024] = { 0 }, buf2[1024];
	for ( i = 0; i < 600; i++ )
	{
		buf[i] = i;
	}
	
	while ( 1 )
	{
		Nf_Set_ALE();
		Delay(10);
		Nf_Clr_ALE();
		Delay(10);
	
		Nf_Set_CLE();
		Delay(10);
		Nf_Clr_CLE();
		Delay(10);
		
		Nf_Set_CE();
		Delay(10);
		Nf_Clr_CE();
		Delay(10);
	}
		
	_CheckId();

	EraseFlush();
	for ( i = 0; i <32; i++ )
	{		
		_WrPage( i, i, (U8*)buf );
		
		memset( buf2, 0, sizeof(buf2)/sizeof(buf2[0]) );
		_RdPage( i, i, (U8 *)buf2);
		
		if ( nMemCmp( (unsigned char*)buf, (unsigned char*)buf2, 512 ) != 0 )
		{
			Glib_ClearScr( CWHITE );
			PutStr( 20,130, "block's rd/wr not equal", CBLACK );	
			PutNum16x16( 20, 160, i, 0, CBLACK, CWHITE );
			EnmDelay(40000);		
		}
	}

	
	//PutStr( 20,100, "press key 0 to write data", CBLACK );	
	//key =  nGetKey();	
	key = KEY_0_N;
	
	if ( key != KEY_ESC )
	{
		if ( key == KEY_0_N )
		{
			EraseFlush();
		}
			
		Glib_ClearScr( CWHITE );
		PutStr( 20,100, "Test block data read/write", CBLACK );	;
		for ( i = 0; i <32; i++ )
		{
			if ( key == KEY_0_N )
				_WrPage( i, i, (U8*)buf );
			
			memset( buf2, 0, sizeof(buf2)/sizeof(buf2[0]) );
			_RdPage( i, i, (U8 *)buf2);
			
			if ( nMemCmp( (unsigned char*)buf, (unsigned char*)buf2, 512 ) != 0 )
			{
				Glib_ClearScr( CWHITE );
				PutStr( 20,130, "block's rd/wr not equal", CBLACK );	
				PutNum16x16( 20, 160, i, 0, CBLACK, CWHITE );
				EnmDelay(40000);		
			}

		   	//Glib_ClearScr( CWHITE );
			
			//for ( j = 0; j < 100; j++ )
			{
			//	PutNum16x16( j % 10 *40, j/10 * 20, buf2[j], 0, CBLACK, CWHITE );
			}
	 		//EnmDelay(40000);
		   	//Glib_ClearScr( CWHITE );
		}
	}
	
	
	PutStr( 20,100, "Test block's spare data rd/wr", CBLACK );	
	PutStr( 20,130, "press key 0 to write data", CBLACK );	
	key =  nGetKey();
	key = KEY_OK;

	if ( key != KEY_ESC )
	{
		if ( key == KEY_0_N )
		{
			EraseFlush();
		}
			
		Glib_ClearScr( CWHITE );
		PutStr( 20,100, "Test block data read/write", CBLACK );	;
		for ( i = 0; i <32; i++ )
		{
			if ( key == KEY_0_N )
				_WrPageSpare( i, i, (U8*)buf );
			
			memset( buf2, 0, sizeof(buf2)/sizeof(buf2[0]) );
			_RdPageSpare( i, i, (U8 *)buf2);

			if ( nMemCmp( (unsigned char*)buf, (unsigned char*)buf2, 16 ) != 0 )
			{
				Glib_ClearScr( CWHITE );
				PutStr( 20,130, "block's rd/wr not equal", CBLACK );	
				PutNum16x16( 20, 160, i, 0, CBLACK, CWHITE );
				EnmDelay(40000);		
			}

		   	//Glib_ClearScr( CWHITE );
			
			//for ( j = 0; j < 100; j++ )
			{
			//	PutNum16x16( j % 10 *40, j/10 * 20, buf2[j], 0, CBLACK, CWHITE );
			}
	 		//EnmDelay(40000);
		   	//Glib_ClearScr( CWHITE );
		}
	}
	
	
	PutStr( 20,100, "Test block's all data rd/wr", CBLACK );	
	PutStr( 20,130, "press key 0 to write data", CBLACK );	
	key =  nGetKey();

	if ( key != KEY_ESC )
	{
		if ( key == KEY_0_N )
		{
			EraseFlush();
		}
			
		Glib_ClearScr( CWHITE );
		PutStr( 20,100, "Test block all data read/write", CBLACK );	;
		for ( i = 0; i <32; i++ )
		{
			if ( key == KEY_0_N )
				_WrPageAll( i, i, (U8*)buf );
			
			memset( buf2, 0, sizeof(buf2)/sizeof(buf2[0]) );
			_RdPageAll( i, i, (U8 *)buf2);

			if ( nMemCmp( (unsigned char*)buf, (unsigned char*)buf2, 528 ) != 0 )
			{
				Glib_ClearScr( CWHITE );
				PutStr( 20,130, "block's rd/wr not equal", CBLACK );	
				PutNum16x16( 20, 160, i, 0, CBLACK, CWHITE );
				EnmDelay(40000);		
			}

		   	//Glib_ClearScr( CWHITE );
			
			//for ( j = 0; j < 100; j++ )
			{
			//	PutNum16x16( j % 10 *40, j/10 * 20, buf2[j], 0, CBLACK, CWHITE );
			}
	 		//EnmDelay(40000);
		   	//Glib_ClearScr( CWHITE );
		}
	}
		
	PutStr( 20,100, "Test block  ok", CBLACK );
	nGetKey();

}

void TestFs()
{
	int i, j;
	U16 fp;
	unsigned char key;
	
	unsigned short  buf[1024] = { 0 }, buf2[1024];
	for ( i = 0; i < 600; i++ )
	{
		buf[i] = i;
	}
	
	_CheckId();

	PutStr( 20,100, "press key 0 to write data", CBLACK );
//*
	i =  Scan_KEY();
	while ( i  == 0xff )
	 i =  Scan_KEY();
	 
		Glib_ClearScr( CWHITE );
		PutNum16x16( 0, 20,i, 0, CBLACK, CWHITE );
	if ( i == KEY_0_N )
	{
		efMount( NF_ERASE );

		if ( (fp = efOpen( "test.dat", EF_OPEN_CREATE| EF_OPEN_WRITE )) == NF_INVALID_FILE_ID )
		{
			PutStr( 20,100, "open file error", CBLACK );
			while(1);
		}


		efWrite( (BYTE*)&buf, sizeof(buf)/sizeof(buf[0]), fp );	
		efFlush( fp );	
		efClose( fp );
		efSaveFS();
	}
	else
		efMount( NF_NORMAL );
		
	if ( (fp = efOpen( "test.dat", EF_OPEN_WRITE )) == NF_INVALID_FILE_ID )
	{
		PutStr( 20,100, "open file error", CBLACK );
		while(1);
	}


   	Glib_ClearScr( CWHITE );
   	memset( buf2, 0, sizeof(buf2)/sizeof(buf2[0]) );
	efRead( (BYTE*)&buf2,   sizeof(buf)/sizeof(buf[0]), fp );	
	for ( j = 0; j < 100; j++ )
	{
		PutNum16x16( j % 10 *40, j/10 * 20, buf2[j], 0, CBLACK, CWHITE );
	}
	EnmDelay(1000);

while ( 1 );
//*/
	
	for ( i = 0; i < 600; i++ )
	{
		buf[i] = i;
	}
	
	key =  Scan_KEY();
	while ( key  == 0xff )
	 key =  Scan_KEY();
	 
	Glib_ClearScr( CWHITE );
	PutNum16x16( 0, 20,i, 0, CBLACK, CWHITE );
	if ( key == KEY_0_N )
	{
		EraseFlush();
	}
	
		for ( i = 0; i <32; i++ )
		{
			if ( key == KEY_0_N )
			_WrPage( 0, i, (U8*)buf );
			
			memset( buf2, 0, sizeof(buf2)/sizeof(buf2[0]) );
			_RdPage( 0, i, (U8 *)buf2);

		   	Glib_ClearScr( CWHITE );
			
			for ( j = 0; j < 100; j++ )
			{
				PutNum16x16( j % 10 *40, j/10 * 20, buf2[j], 0, CBLACK, CWHITE );
			}
	 		EnmDelay(100000);
		   	Glib_ClearScr( CWHITE );
		}
		
PutStr( 20,100, "write ok", CBLACK );
while(1);


	for ( j = 0; j < 1024; j++ )
	{
		for ( i = 0; i <32; i++ )
		{
			_WrPage( j, i, (U8*)buf );
			memset( buf2, 0, sizeof(buf2)/sizeof(buf2[0]) );
			_RdPage( j, i, (U8 *)buf2);
		}
	}
	
	
	_WrPage( 0, 0, (U8*)buf );
	memset( buf, 0, sizeof(buf)/sizeof(buf[0]) );
	_RdPage(0, 0, (U8 *)buf);


	for ( i = 10; i < 120; i++ )
	{
		buf[i] = i;
	}
	_WrPage( 12, 12, (U8*)buf );
	memset( buf, 0, sizeof(buf)/sizeof(buf[0]) );
	_RdPage(12, 12, (U8 *)buf);

	while(1)
	{
		EraseFlush();
		_WrPage( 0, 0, (U8*)buf );
		_RdPage(0, 0, (U8 *)buf);
	
	}

}

#pragma pack(1)
typedef struct 
{
	short  buf[1200];
}tagItem;

typedef struct 
{
	short cnt;
	short cur;
	tagItem   stItem[16];
}tagTemp;

#pragma pack()

void TestFile()
{
	short i;
	unsigned short fp;
	tagTemp  val;
	memset( &val, 0, sizeof(val) );
	for ( i = 0; i < 1200;i++ )
	{
	  val.stItem[0].buf[i] = i;
	  val.stItem[1].buf[i] = 1200+i;
	  val.stItem[2].buf[i] = 2400+i;
	}
	
	PutStr( 20,100, "press key 0 to write data", CBLACK );

	i =  Scan_KEY();
	while ( i  == -1 )
	 i =  Scan_KEY();
	 
		Glib_ClearScr( CWHITE );
		PutNum16x16( 0, 20,i, 0, CBLACK, CWHITE );
	if ( i == KEY_0_N )
	{
		efMount( NF_ERASE );

		if ( (fp = efOpen( "test.dat", EF_OPEN_CREATE| EF_OPEN_WRITE )) == NF_INVALID_FILE_ID )
		{
			PutStr( 20,100, "open file error", CBLACK );
			while(1);
		}


		efWrite( (BYTE*)&val, sizeof(val), fp );	
		efFlush( fp );	
		efClose( fp );
		efSaveFS();
	}
	else
		efMount( NF_NORMAL );
		
	if ( (fp = efOpen( "test.dat", EF_OPEN_WRITE )) == NF_INVALID_FILE_ID )
	{
		PutStr( 20,100, "open file error", CBLACK );
		while(1);
	}

	efRead( (BYTE*)&val, sizeof(val), fp );	
	
	for ( i = 10; i < 20; i++ )
		val.stItem[1].buf[i] = 0xffff;
	efSeek( fp, 4 + sizeof(tagItem), ESEEK_SET );
	efWrite((BYTE*)&val.stItem[1], sizeof(tagItem), fp );
	efFlush( fp );
	
	for ( i = 0; i < 1200;i++ )
	{
	  val.stItem[3].buf[i] = 3600+i;
	}
	val.cnt = 4; val.cur = 3;
	efSeek( fp, 4 + sizeof(tagItem)*3, ESEEK_SET );
	efWrite( (BYTE*)&val.stItem[3], sizeof(tagItem), fp );
	efFlush( fp );
	
	efSeek( fp,0, ESEEK_SET );
	efRead( (BYTE*)&val, sizeof(val), fp );	

}

/////////////////////////////////////////////////////////////////////////////////////
//   FS Test
//  
/////////////////////////////////////////////////////////////////////////////////////

extern  void ADC_SelectChanel( unsigned char chanel );
extern  void ADC_Start();
extern  short  ADC_ReadData();
extern  void PutStr( int x, int y, char * str , char bSel/*, unsigned short pos*/ );


void  TestADC()
{
	short i;
	short x, y;
	char buf[32];
	char str[32];

	ENMADC  aData[ENM_REAGENT_LN_MAX];
	
	int val = 1;
	float v;
	
	unsigned char channelMap[] = 
	{
		0, 2,6,7,5,4,3,1
	};
	
	for( i = 0; i < 31; i++ ) 
		buf[i] = ' ';
		
	x = 10; 
	y = 00;
	

	ADC_SelectChanel( 0); 
	while ( 1 )
	{	
	
	 #if 1
	 	Plate_Motor_On(1);
	 	Light_Motor_On(1);
	 	EnmSampling( aData );
		for ( i = 0; i < ENM_REAGENT_LN_MAX; i++ )
		{
		    val = aData[i];
			v=(20.0/(unsigned int)0xffffff)*val;
			PutStr( x, y + i * 20, buf, 0 );	
			
			FormatFP( v, str, 5, 2 );
			PutStr( x, y + i * 20,str, 0 );			
			PutNum16x16( x+ 100, y + i * 20, val, 0, CBLACK, CWHITE );
		}
	 
	 #else
	 
		for ( i = 0; i < ENM_REAGENT_LN_MAX; i++ )
		//for ( i = 0; i <1; i++ )
		{
			ADC_SelectChanel( channelMap[i]  );
			EnmDelay(10);
			ADC_Start();
			EnmDelay(10 );
			val = ADC_ReadData();	
			EnmDelay(1);

			v=(20.0/(unsigned int)0xffffff)*val;
			PutStr( x, y + i * 20, buf, 0 );	
			
			FormatFP( v, str, 5, 2 );
			PutStr( x, y + i * 20,str, 0 );			
			PutNum16x16( x+ 100, y + i * 20, val, 0, CBLACK, CWHITE );
		}
		
	 #endif
		
		EnmDelay(12000);
		ClearScreen( CWHITE );
	}	
}

void TestKey()
{
	short key;
	char buf[128], buf1[128];

	while (1 )
	{
		key = nScanKey();
		if ( key != -1 )
		{
			ClearScreen( CWHITE );
			//sprintf( buf, "0x%04x, %d", GetKeyValue(key), GetKeyCount(key) );
			sprintf( buf, "0x%04x", (key) );
			PutStr(50, 100, buf, 0 );
		}

	}

}

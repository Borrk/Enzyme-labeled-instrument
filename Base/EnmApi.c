#include <time.h>
#include <math.h>
#include "EnmApi.h"


#define  PRNT_STORE_PORT   (*(volatile unsigned char *)0x6000005)
	#define  PRNT_STORE	   (1<<0)	
#define  PRNT_STATUS_PORT  (*(volatile unsigned char *)0x6000006)
	#define PRNT_ACK	   (1<<0)
	#define PRNT_BUSY	   (1<<1)
	#define PRNT_PE	   	   (1<<2)
	#define PRNT_SELECT	   (1<<3)
	#define PRNT_ERR	   (1<<4)
#define  PRNT_DATA_PORT    (*(volatile unsigned char *)0x6000007)

void PrntStore(unsigned char level )
{
	PRNT_STORE_PORT = level;
}

unsigned char PrntIsBusy( void )
{
	unsigned char aStatus = PRNT_STATUS_PORT;
	return (aStatus&PRNT_BUSY);
}

unsigned char PrntReadStatus( void )
{
	unsigned char aStatus = PRNT_STATUS_PORT;
	return aStatus;
}

void PrntSendByte( unsigned char data )
{
#ifdef ENM_VER_TARGET
	unsigned char ch;
	int i = 0;
	while ( PrntIsBusy() )
	{
		ch = PrntReadStatus();
		EnmDelay( 2 );
		if ( ++i > 100 )
			return;
	}
	
	
	PrntStore( 0xff );
	
	PRNT_DATA_PORT = data;
	
	EnmDelay(1);
	PrntStore( 0 );
	
	EnmDelay(10);
	PrntStore( 0xff );

#endif	
}

unsigned short  PrntSendData( unsigned char *pData, unsigned short cnt )
{
	unsigned short i;
	unsigned char data;
	
	if ( !pData )
		return 0;
	for ( i = 0; i < cnt; i++ )
	{
		data = pData[i];
		PrntSendByte( data );
		EnmDelay( 100 );
	}
	
	return cnt;
}


/// printer control routines
static unsigned short      PRNT_MAX_COL = 360;

static unsigned char		ESC_INIT[2]			= {0x1B, 0x40 };
static unsigned char		ESC_DIRECTION[3]	= {0x1B, 0x63, 0x00 };
static unsigned char		ESC_CHARSET_1[2]	= {0x1B, 0x36 };
static unsigned char		ESC_CHARSET_2[2]	= {0x1B, 0x37 };
static unsigned char		ESC_PAPER_FW[3]     = {0x1B, 0x4A, 0x00 };
static unsigned char		ESC_GRAPH_HLINE[3]  = {0x1B, 0x27, 0x00 };
static unsigned char		ESC_GRAPH_PIXEL[4]  = {0x1B, 0x4B, 0x00, 0x00 };
static unsigned char		ESC_LN_INTERVAL[3]  = {0x1B, 0x31, 0x00 };
static unsigned char		ESC_COL_INTERVAL[3] = {0x1B, 0x70, 0x00 };
static unsigned char		ESC_ZOOM[3] 		= {0x1B, 0x57, 0x00 };
static unsigned char		ESC_CHARSET_CN[2] 	= {0x1C, 0x26 };

static unsigned char		ESC_LF[] = { 0x0A };
static unsigned char		ESC_CR[] = { 0x0D };


static void nInitPrinter(void)
{
	PrntSendData( ESC_INIT, sizeof(ESC_INIT) );
}

static unsigned short nSendData2Printer( unsigned char *pData, unsigned short dataSize )
{
	return PrntSendData( pData, dataSize );
}

char	EnmSetDirection( char direction  )
{
	ESC_DIRECTION[2] = direction == 0 ? 0 : 1;
	PrntSendData( ESC_DIRECTION, sizeof(ESC_DIRECTION) );
	
	return 1;
}

char	EnmCharZoom( unsigned char zoom  )
{
	ESC_ZOOM[2] =zoom;
	PrntSendData( ESC_ZOOM, sizeof(ESC_DIRECTION) );
	
	return 1;
}

char      EnmPrntInit( void )
{
	PrntSendData( ESC_INIT, sizeof(ESC_INIT) );
	
	EnmSetDirection( 0 );
	EnmCharZoom(1);

	return 1;
}

	
char	  EnmPrntLF(  void )
{
	nSendData2Printer( ESC_LF , 1 );	
	
	return 1;
}

char	  EnmPrntCR(  void )
{
	nSendData2Printer( ESC_CR, 1);	
	
	return 1;
}

char EnmPrntSetCharInterval(  unsigned char pixels )
{		
	ESC_COL_INTERVAL[2] = pixels;
	nSendData2Printer( ESC_COL_INTERVAL, 3);	

	return 1;
}

char EnmPrntSetLineInterval(  unsigned char pixels )
{		
	ESC_LN_INTERVAL[2] = pixels;
	nSendData2Printer( ESC_LN_INTERVAL, 3);	

	return 1;
}

char	  EnmPrntSetCharSet(  unsigned char charset )
{		
	switch ( charset )
	{
	case 0:
		nSendData2Printer(   ESC_CHARSET_1, 2 );	
		break;	
	case 1:
		nSendData2Printer(   ESC_CHARSET_2, 2 );	
		break;	
	case 2:
		nSendData2Printer(   ESC_CHARSET_CN, 2 );	
		break;	
	default:
		break;	
	}
	
	return 1;	
}

char EnmPrntVLine(  unsigned short  from, unsigned short to )
{
#if 1

	static unsigned char aPixels[1024] = { 1 };
	unsigned short i;
	unsigned short aPnts = to - from + 1;

	if ( aPnts > (PRNT_MAX_COL - from) )
		aPnts = PRNT_MAX_COL - from;
	ESC_GRAPH_PIXEL[2] = aPnts&0xff;
	ESC_GRAPH_PIXEL[3] = (aPnts >> 8)&0xff;

	nSendData2Printer( ESC_GRAPH_PIXEL, 4 );
	nSendData2Printer( aPixels, aPnts );
#else
	static unsigned char aPixels[512] = { 0 };
	unsigned short i;
	unsigned short aPnts = to - from + 1;

	if ( aPnts > (PRNT_MAX_COL - from) )
		aPnts = PRNT_MAX_COL - from;
	ESC_GRAPH_HLINE[2] = aPnts;

	for ( i = 0; i < aPnts; i++ )
		aPixels[i] = i + from;
	aPixels[i] = ESC_CR;

	nSendData2Printer( ESC_GRAPH_HLINE, 3 );
	nSendData2Printer( aPixels, aPnts );

#endif
	
	return 1;
}

unsigned char  EnmPrntOneLine(  unsigned char  x, char *pData, char bLF )
{
	unsigned char cmd[] = {0x1b, 0x6c, 0x00 };
	
	cmd[2] = x;
	PrntSendData( cmd, 3 );
	
	PrntSendData( (unsigned char*)pData, strlen(pData) );

	//cmd[2] = 1;
	//PrntSendData( cmd, 3 );

	if ( bLF )
	{
		EnmPrntCR();
		EnmPrntLF();
	}

	return strlen(pData);
}

char	  EnmPrntPaperForward(  unsigned char  lineCnt )
{
	ESC_PAPER_FW[2] = lineCnt;
	nSendData2Printer( ESC_PAPER_FW, 3 );
	
	return 1;
}

void EnmPrntCRLF( void )
{
		EnmPrntCR();
		EnmPrntLF();
}
	
/**
 *    Device status Manage Routines
 *
 */
 
void EnmSetDate( tagDate date )
{
#ifdef ENM_VER_TARGET
 	struct rtc_time tmp;
	rtc_get( &tmp );

	tmp.tm_year = date.year;
	tmp.tm_mon  = date.month;
	tmp.tm_mday = date.day;	
	
	rtc_set( &tmp );
#else
	/// do nothing
#endif	
}
 
void EnmSetTime( tagTime time )
{
#ifdef ENM_VER_TARGET
 	struct rtc_time tmp;
	rtc_get( &tmp );

	tmp.tm_hour = time.hour;
	tmp.tm_min  = time.min;
	tmp.tm_sec  = time.sec;	
	
	rtc_set( &tmp );
#else
	/// do nothing
#endif	
}

tagDate	EnmGetDate( void )
{
	tagDate	aDate;
	
#ifdef ENM_VER_TARGET
	struct rtc_time tmp;
	rtc_get( &tmp );

	aDate.year 	= tmp.tm_year;
	aDate.month = tmp.tm_mon;
	aDate.day 	= tmp.tm_mday;	
#else
	aDate.year 	= 2005;
	aDate.month = 10;
	aDate.day 	= 10; 	
#endif	

	return aDate;
}

tagTime	EnmGetTime( void )
{
	tagTime	aTime;

#ifdef ENM_VER_TARGET
	struct rtc_time tmp;
	rtc_get( &tmp );

	aTime.hour = tmp.tm_hour;
	aTime.min  = tmp.tm_min;
	aTime.sec  = tmp.tm_sec;

#else
	aTime.hour = 8;
	aTime.min  = 0;
	aTime.sec  = 0;
#endif

	return aTime;
}

/// get total seconds start from 1980-1-1 00:00:00
unsigned long EnmGetTick()
{
	long aDayOfMonth[] = 
	{ 
		31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 
	};
	unsigned long aSec = 0;

#ifdef ENM_VER_TARGET
	struct rtc_time tmp;
	rtc_get( &tmp );

	/// notes: here seem 1 year as 365 days, which is not accury
	if ( tmp.tm_year >= 1980 )
	{
		aSec = (tmp.tm_year - 1980) * 365 + aDayOfMonth[tmp.tm_mon-1] + tmp.tm_wday;
		aSec += tmp.tm_hour * 3600 + tmp.tm_min * 60 + tmp.tm_sec;
	}
#endif

	return aSec;
}

void PDelay( unsigned short seconds )
{
	unsigned long aStart, aEnd;

#ifdef ENM_VER_TARGET
	struct rtc_time tmp;
	rtc_get( &tmp );
	
	aStart = tmp.tm_hour * 3600 + tmp.tm_min * 60 + tmp.tm_sec;
	do
	{
		rtc_get( &tmp );
		aEnd = tmp.tm_hour * 3600 + tmp.tm_min * 60 + tmp.tm_sec;

	}while ( (aEnd - aStart) < seconds );
#endif
}


void  FormatFP( double val, char * pDst, unsigned char m, unsigned char n )
{
	int i, j;
	char aPrexCh;
	char *p;
	int aInterger, aFracInt;
	double aTemp;
	double aFrac;


	int aIntCnt, aFracCnt; ///< counter of integer and frac part of the value

	/// check 
	if ( val < 0 )
	{
		aPrexCh = '-';
		val = -val;
	}
	else
		aPrexCh = ' ';

	/// pick integer and frac of the value
	aInterger = (int)val;
	aFrac    = val - aInterger;
	aFracInt = (int)(aFrac * pow(10, n) + 0.5 );

	/// calculate char count of the value's integer
	aIntCnt = 0;
	aTemp = aInterger;
	while ( aTemp >= 1.0 )
	{
		aIntCnt++;
		aTemp /= 10.0;
	}
	if ( aIntCnt == 0 )
		aIntCnt = 1;

	/// calculate char count of the value's frac part
	if ( aIntCnt + 2 >= m )
	{
		aFracCnt = 1;
	}
	else
	{
		aFracCnt = m - aIntCnt - 2;	/// dot and prexch occupy 2 chars
		if ( aFracCnt > n )
			aFracCnt = n;

	}
		
	aFracInt = (int)(aFrac* pow( 10, aFracCnt) + 0.5);

	/// fix prex-char
	if ( aFracInt == 0 && aInterger == 0 )
	{
		aPrexCh = ' ';
	}

	/// format
	pDst[0] = aPrexCh;

	p = pDst+1;
	for( i = 0; i < aIntCnt; i++ )
	{
		p[aIntCnt-i-1] = '0' + aInterger %10;
		aInterger /= 10;
	}
	
	p += aIntCnt;
	*p++ = '.';

	*(p+aFracCnt) = '\0';
	for( i = 0; i < aFracCnt; i++ )
	{
		p[aFracCnt-i-1] = '0' + aFracInt %10;
		aFracInt /= 10;
	}
}

void  FormatFP2( double val, char * pDst, unsigned char m, unsigned char n )
{
	short i, aLen;
	if ( pDst )
	{
		FormatFP( val, pDst,m,  n );
		if ( strchr( pDst, '.' ) == NULL )
			return;

		i = strlen( pDst ) - 1;		
		while ( i > 0 && pDst[i-1] != '.' )
		{
			if ( pDst[i] != '0' )
				break;
		
			if ( pDst[i] == '0' && pDst[i-1] != '.' )
				pDst[i] = '\0';

			i--;
		}
	}
}

double Str2FP( char *pStr )
{
	double   aVal = 0, aRatio = 10.0;
	unsigned char  aFirst, aSigned = 0;
	char  aInt[16];
	char  aCh;
	char  aFraction[32] = { '0','\0' };

	char *apCur = pStr;
	if ( *pStr == '-' )
	{
		aSigned = 1;
		apCur = &pStr[1];
	}
	
	aFirst = 1;
	while ( (aCh = *apCur) != 0 )
	{
		if ( aCh == '.' )
		{
			if ( aFirst )	///< invalid string
				break;
			else if ( aRatio < 10.0 )	///< second '.' occurent
				break;
			else
			{
				aRatio = 1.0;
				apCur++;
				aFirst = 0;
				continue;
			}
		}

		if (  '0' <= aCh && aCh <= '9' )
		{
			if ( aRatio > 1.0 )
				aVal = aVal * aRatio + (double)(aCh - '0');
			else
			{
				aRatio *= 0.1;
				aVal = aVal + aRatio * (double)(aCh - '0');
			}
		}
		else
		{
			break;
		}

		apCur++;
		aFirst = 0;
	}

	if ( aSigned)
		aVal = -aVal;

	return aVal;
}


#if 0

unsigned char	BIT( long val, char idx ) 	    
{ return (((val)>>(idx))&0x01 ); }

void	SetBit( long val, char idx ) 	
{ 
	val |= (0x01 << idx); 
}

void	ResetBit( long val, char idx ) 
{ val &= ~(0x01 << idx);   }
#endif


/**
 *    Moter Routines
 *
 */
#if 0
 static tagMotorInfo nPlateMotorInfo = { 33.0, 15.0, 0.18, 9.0 };
 static tagMotorInfo nLightMotorInfo = { -1.5, 0.5, 0.18, 10.0 };
 
 static BYTE nCurPlatePos = 0;
 static BYTE nCurLightPos = 0;


void	EnmCfgPlateMotor( tagMotorInfo info )
{
	nPlateMotorInfo = info;
}

void EnmCfgLightMotor( tagMotorInfo info )
{
	nLightMotorInfo = info;
}

char  EnmPlateReset( char bBack )
{
	int i;
	char aRet = 1;
	unsigned short cnt = 0;
	
	Plate_Motor_On( 1 );
	
	/// move forward first
	if ( bBack )
	{
		for ( i = 0; i < (int)(nPlateMotorInfo.fPostDist / nPlateMotorInfo.fStepDist); i++ )
		{
			EnmRun(Plate_Motor_CW_ONE_STEP());
		}
		EnmDelay( 100 );
	}

	#if 1	
	while (1)
	{	
#ifdef  ENM_VER_TARGET
		if ( EnmRun(Is_Plate_Motor_Reset()) )
#endif
		{		 
		  break;
		}	
			
		EnmRun( Plate_Motor_CCW_ONE_STEP() );
		/// if step larger than max step, quit
		if ( (++cnt) > ENM_PLATE_MAX_STEP )
		{
			aRet = 0;
			break;
		}				
	}	
	#endif
	
	nCurPlatePos = 0;

	//Plate_Motor_On( 0 );
	
	return aRet;	
}

unsigned short  EnmGetPlatePos( void)
{
	unsigned short  aPos = 0;
	
	// TBA
	
	return aPos;	
}

void  EnmPlateForwardOne( void)
{
	short  i;
	for ( i = 0; i < (short)( 0.5 + nPlateMotorInfo.fHoleDist / nPlateMotorInfo.fStepDist); i++ )
	{
		EnmRun(  Plate_Motor_CW_ONE_STEP() );
		//EnmDelay(50);
	}
}

void  EnmPlateForward( void)
{
	short  i;
	for ( i = 0; i < (short)( 0.5 + nPlateMotorInfo.fHoleDist / nPlateMotorInfo.fStepDist); i++ )
		EnmRun(  Plate_Motor_CW_ONE_STEP() );
}

void  EnmPlateBackwardOne( void)
{
	short  i;
	for ( i = 0; i < (short)( 0.5 + nPlateMotorInfo.fHoleDist / nPlateMotorInfo.fStepDist); i++ )		
	{
		EnmRun(  Plate_Motor_CCW_ONE_STEP() );
		//EnmDelay(50);
	}	
}

void  EnmPlateBackward( void)
{
	short  i;
	for ( i = 0; i < (short)( 0.5 + nPlateMotorInfo.fHoleDist / nPlateMotorInfo.fStepDist); i++ )
	{
		EnmRun(  Plate_Motor_CCW_ONE_STEP() );	
	}
}

void  EnmPlatePrePos( void) 
{
	short i, aCnt;
	/// if not in start position, reset it
	if ( nCurPlatePos != 0 )
	{
		EnmPlateReset( 0 );
	}

	/// move to the one-hole-distance position before the first hole
	for ( i = 0; i < (short)( 0.5 + nPlateMotorInfo.fPreDist / nPlateMotorInfo.fStepDist); i++ )
	{
		EnmRun( Plate_Motor_CW_ONE_STEP() );
	}

	nCurPlatePos = 1;
}

///  Optical motor reset 
char  EnmFilterReset( char bBack )
{
	int  i;
	char aRet = 1;
	unsigned short cnt = 0;
		
	Light_Motor_On( 1 );
	
	/// move forward first
	if ( bBack )
	{
		for ( i = 0; i < (nLightMotorInfo.fPostDist / nLightMotorInfo.fStepDist); i++ )
		{
			EnmRun(Light_Motor_CW_ONE_STEP());
		}
		EnmDelay( 10 );
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
		if ( (++cnt) > ENM_LIGHT_MAX_STEP )
		{
			aRet = 0;
			break;
		}				
	}	
	
	nCurLightPos = 0;
	
	/// Set to first filter pos
	if ( aRet == 1 )
		EnmFilterPrePos();
		
	//Light_Motor_On( 0 );
	
	return aRet;	
}

void EnmFilterPrePos()
{
	char bForward;
	short i, aCnt;
	/// if not in start position, reset it
	if ( nCurLightPos != 0 )
	{
		EnmFilterReset( 0 );
	}

	/// move to specified filter
	bForward = 1;
	aCnt = (short)( 0.5 + nLightMotorInfo.fPreDist / nLightMotorInfo.fStepDist);
	if ( aCnt < 0 )
	{
		bForward = 0;
		aCnt = -aCnt;
	}
	
	for ( i = 0; i < aCnt; i++ )
	{
		if ( bForward )
			EnmRun( Light_Motor_CW_ONE_STEP() );
		else	
			EnmRun( Light_Motor_CCW_ONE_STEP() );
	}

	nCurPlatePos = 0;
}

unsigned short  EnmGetFilterPos( void)
{
	unsigned short  aPos = 0;
	
	// TBA
	
	return aPos;		
}

void  EnmFilterForward( void)
{
	// TBD
	EnmRun(  Light_Motor_CW_ONE_STEP() );
}

void  EnmFilterBackward( void)
{
	// TBD
	EnmRun(  Light_Motor_CCW_ONE_STEP() );	
}

/// Step forward one wave filter
BYTE  EnmFilterPos(  unsigned short pos )
{
	short i, aCnt;
	if ( pos > 8 )
		return 0;

	/// if not in start position, reset it
	if ( nCurLightPos != 0 )
	{
		EnmFilterReset( 0 );
	}

	//aCnt = (pos+1) * ENM_LIGHT_MAX_STEP/9;
	aCnt = pos * (ENM_LIGHT_STEP_LEN / nLightMotorInfo.fStepDist);
	/// move to specified filter
	for ( i = 0; i < aCnt; i++ )
	{
		EnmFilterForward();
	}

	nCurLightPos = pos+1;

	return 1;
}

#endif

/**
 *    ADC Sampling  Routines
 */

#if 1

/*
AD_STS:   0100   bit3
AD_R/C:   0010   bit5
MUX_A0:   0001   bit4
MUX_A1:   0001   bit3
MUX_A2:   0001   bit2
*/

#define ADC_DATA_PORT1 (*(volatile unsigned char *)0x6000003)	///< data port
#define ADC_DATA_PORT2 (*(volatile unsigned char *)0x6000004)	///< data port

#define ADC_CTRL_PORT (*(volatile unsigned char *)0x6000002)	///< 
	#define ADC_CTRL_RC    (0x01 << 5)
	//#define ADC_CTRL_CS    0x01 << 7
	//#define ADC_CTRL_BYTE  0x01 << 5

#define ADC_STATUS_PORT (*(volatile unsigned char *)0x6000004)
	#define ADC_STATUS_BUSY  (0x01 << 3)

#define  ADC_AMP_MUX_PORT (*(volatile unsigned char *)0x6000001)	/// Amplify and mux port 
	#define ADC_AMP_A0    0x01 << 7
	#define ADC_AMP_A1    0x01 << 6
	#define ADC_AMP_A2    0x01 << 5
	#define ADC_MUX_A0    0x01 << 4
	#define ADC_MUX_A1    0x01 << 3
	#define ADC_MUX_A2    0x01 << 2


unsigned char ADC_CTRL_PORT_MIRROR=0xff;
unsigned char ADC_AMP_PORT_MIRROR=0xff;


void ADCOutpCtrl( unsigned char bit, unsigned char val )
{
		
	if ( val == 0 )
	{
		ADC_CTRL_PORT_MIRROR	&= ~( bit);
		ADC_CTRL_PORT = ADC_CTRL_PORT_MIRROR;
	}
	else
	{
		ADC_CTRL_PORT_MIRROR	|= (bit);
		ADC_CTRL_PORT = ADC_CTRL_PORT_MIRROR;
	}
}

void ADCOutpAmp( unsigned char bit, char val )
{
	unsigned char *apMirror = &ADC_AMP_PORT_MIRROR;
		
	if ( val == 0 )
	{
		*apMirror	&= ~(bit);
		ADC_AMP_MUX_PORT = *apMirror;
	}
	else
	{
		*apMirror	|= (bit);
		ADC_AMP_MUX_PORT = *apMirror;
	}
}

unsigned char ADCInpData( char part )
{
	short p;
	
	if ( part == 0 )
	{
		p = ADC_DATA_PORT1;
	}
	else
	{
		p = ADC_DATA_PORT2;	
		p = (p & 0xf0) >> 4;
	}	

	return p;
}

unsigned char ADCInpStatus( )
{
	unsigned char p;
	p = ADC_STATUS_PORT;
	return p;
}

unsigned char ADC_Busy()
{
	unsigned char p;
	p = ADC_STATUS_PORT;
	return ( p&ADC_STATUS_BUSY);
}


void  ADC_Convert()
{
	ADCOutpCtrl( ADC_CTRL_RC, 0 );
}

void  ADC_Read()
{
	ADCOutpCtrl( ADC_CTRL_RC, 1 );
}

short  ADC_ReadData()
{
	short val = 0;
	
	ADC_Read();
	
	EnmDelay( 4 );
	
	val = ADCInpData( 0 );	
	val |= (ADCInpData( 1 ) << 8);
			
	return (val&0xffff);
}


void ADC_Start()
{
	ADC_Convert();		
	EnmDelay( 1 );
}


void ADC_Wait()
{
	while ( ADC_Busy() );
}

void ADC_SelectChanel( unsigned char chanel )
{
	unsigned char aChanel[8][3] = 
	{
	 ///A0,A1, A2 ,Log-chn, Phy-chn  
		0,0,0,	/// 0 --> 1
		1,1,1,  /// 1 --> 8		
		1,0,0,  /// 2 --> 2		
		0,1,1,  /// 3 --->7
		1,0,1,  /// 4 --->6
		0,0,1,  /// 5 --->5
		0,1,0,  /// 6 --->3 
		1,1,0   /// 7 --->4
	};
	
	if ( chanel > 7 )
		return;
		
	ADCOutpAmp( ADC_MUX_A0, aChanel[chanel][0] );
	ADCOutpAmp( ADC_MUX_A1, aChanel[chanel][1] );
	ADCOutpAmp( ADC_MUX_A2, aChanel[chanel][2] );		
}

/// support only ratio: 1, 2, 4, 8, 16
/// 1 --- 000b
/// 2 --- 001b
/// 4 --- 010b
/// 8 --- 011b
/// 16--- 1xxb
void ADC_SelectAmp( unsigned char ampRatio )
{
	ADCOutpAmp( ADC_AMP_A0, ampRatio&0x01 );
	ADCOutpAmp( ADC_AMP_A1, ampRatio&0x02 );
	ADCOutpAmp( ADC_AMP_A2, ampRatio&0x04 );			
}


#else
 
// #define ADC_DATA_PORT (*(volatile unsigned char *)0x6000004)	///< data port
//
//#define ADC_CTRL_PORT (*(volatile unsigned char *)0x6000002)	///< 
//	#define ADC_CTRL_CS    0x01 << 7
//	#define ADC_CTRL_RC    0x01 << 5
//	#define ADC_CTRL_BYTE  0x01 << 5
//
//#define ADC_STATUS_PORT (*(volatile unsigned char *)0x6000003)
//	#define ADC_STATUS_BUSY  0x01 << 3
//
//#define  ADC_AMP_MUX_PORT (*(volatile unsigned char *)0x6000001)	/// Amplify and mux port 
//	#define ADC_AMP_A0    0x01 << 7
//	#define ADC_AMP_A1    0x01 << 6
//	#define ADC_AMP_A2    0x01 << 5
//	#define ADC_MUX_A0    0x01 << 4
//	#define ADC_MUX_A1    0x01 << 3
//	#define ADC_MUX_A2    0x01 << 2
//
//
//unsigned char ADC_CTRL_PORT_MIRROR=0xff;
//unsigned char ADC_AMP_PORT_MIRROR=0xff;
//
//
//void ADCOutpCtrl( unsigned char bit, unsigned char val )
//{
//		
//	if ( val == 0 )
//	{
//		ADC_CTRL_PORT_MIRROR	&= ~( bit);
//		ADC_CTRL_PORT = ADC_CTRL_PORT_MIRROR;
//	}
//	else
//	{
//		ADC_CTRL_PORT_MIRROR	|= (bit);
//		ADC_CTRL_PORT = ADC_CTRL_PORT_MIRROR;
//	}
//}
//
//void ADCOutpAmp( unsigned char bit, char val )
//{
//	unsigned char *apMirror = &ADC_AMP_PORT_MIRROR;
//		
//	if ( val == 0 )
//	{
//		*apMirror	&= ~(bit);
//		ADC_AMP_MUX_PORT = *apMirror;
//	}
//	else
//	{
//		*apMirror	|= (bit);
//		ADC_AMP_MUX_PORT = *apMirror;
//	}
//}
//
//unsigned char ADCInpData( )
//{
//	unsigned char p;
//	
//	p = ADC_DATA_PORT;
//
//	return p;
//}
//
//unsigned char ADCInpStatus( )
//{
//	unsigned char p;
//	p = ADC_STATUS_PORT;
//	return p;
//}
//
//unsigned char ADC_Busy()
//{
//	unsigned char p;
//	p = ADC_STATUS_PORT;
//	return ( p&0x80);
//}
//
//
//void  ADC_Convert()
//{
//	ADCOutpCtrl( ADC_CTRL_RC, 0 );
//}
//
//void  ADC_Read()
//{
//	ADCOutpCtrl( ADC_CTRL_RC, 1 );
//}
//
//void  ADC_Byte( char val )
//{
//	//ADCOutpCtrl( ADC_CTRL_BYTE, val );
//}
//
//void  ADC_CS( unsigned char val )
//{
//	//ADCOutpCtrl( ADC_CTRL_CS, val );
//}
//
//short  ADC_ReadData()
//{
//	short val = 0;
//	
//	ADC_Read();
//	
//	ADC_Byte( 0 );
//	ADC_CS( 0 );
//	EnmDelay( 2 );
//	
//	val = ADCInpData();// << 8;
//	
//	ADC_Byte( 1 );
//	EnmDelay( 2 );
//	
//	val |= (ADCInpData()&0x0f) << 8;
//	
//	
//	ADC_CS( 1 );
//	ADC_Byte( 0 );
//	
//	//return -(val&0xffff);
//	return (val&0xffff);
//}
//
//
//void ADC_Start()
//{
//
//	ADC_Convert();	
//	ADC_CS( 0 );
//	
//	EnmDelay( 1 );
//		
//	ADC_CS( 1 );	
//}
//
//
//void ADC_Wait()
//{
//	while ( ADC_Busy() );
//}
//
//void ADC_SelectChanel( unsigned char chanel )
//{
//	unsigned char aChanel[8][3] = 
//	{
//	 ///A0,A1, A2 ,Log-chn, Phy-chn  
//		0,0,0,	/// 0 --> 1
//		1,1,1,  /// 1 --> 8		
//		1,0,0,  /// 2 --> 2		
//		0,1,1,  /// 3 --->7
//		1,0,1,  /// 4 --->6
//		0,0,1,  /// 5 --->5
//		0,1,0,  /// 6 --->3 
//		1,1,0   /// 7 --->4
//	};
//	
//	if ( chanel > 7 )
//		return;
//		
//	ADCOutpAmp( ADC_MUX_A0, aChanel[chanel][0] );
//	ADCOutpAmp( ADC_MUX_A1, aChanel[chanel][1] );
//	ADCOutpAmp( ADC_MUX_A2, aChanel[chanel][2] );		
//}
//
///// support only ratio: 1, 2, 4, 8, 16
///// 1 --- 000b
///// 2 --- 001b
///// 4 --- 010b
///// 8 --- 011b
///// 16--- 1xxb
//void ADC_SelectAmp( unsigned char ampRatio )
//{
//	ADCOutpAmp( ADC_AMP_A0, ampRatio&0x01 );
//	ADCOutpAmp( ADC_AMP_A1, ampRatio&0x02 );
//	ADCOutpAmp( ADC_AMP_A2, ampRatio&0x04 );			
//}

#endif

 
void  EnmADC_Init( unsigned char amp )
 {
 	ADC_SelectAmp( amp );
}

/// pDstData: sampling data buffer;  return : sampled  data count
unsigned short	  EnmSampling( ENMADC  *pDstData )
{
	unsigned char  i, j;
	int val = 0;
	unsigned char channelMap[] = 
	{		
		0, 2,6,7,5,4,3,1
		//0, 4, 3, 5, 6, 1, 2, 7
	};
				
#ifndef ENM_VER_TARGET
	ENMADC aSimData[ENM_REAGENT_LN_MAX] = { 800, 810, 820, 830, 840, 850, 860, 870 };
#endif

	for ( i = 0; i < ENM_REAGENT_LN_MAX; i++ )
	{
#ifdef ENM_VER_TARGET
		val = 0;
		ADC_SelectChanel(  channelMap[i] );
		EnmDelay( 10 );
		for ( j = 0; j < 10; j++ )
		{
			ADC_Start();		
			EnmDelay( 4 );
			val += ADC_ReadData();	
			EnmDelay( 12 );
		}
		pDstData[i] = val / 10.0;
#else
		pDstData[i] = aSimData[i] + rand()%3;
#endif
	}
	
	return   ENM_REAGENT_LN_MAX;	
}


#ifdef  ENM_VER_TARGET

void ClearScreen( INT8U clr  )
{
	Glib_ClearScr( clr );
}

void PutStr( int x, int y, char * str , char bSel/*, unsigned short pos*/ )
{
	if ( bSel )
	{
		PutCh16x16Str( x, y, str, 0, CWHITE, CBLACK );
	}
	else
	{
		PutCh16x16Str( x, y, str, 0, CBLACK, CWHITE );
	}
}

void PutStrThin( int x, int y, char * str , char bSel/*, unsigned short pos*/ )
{
	if ( bSel )
	{
		dis6x12( x, y, str, 0, CWHITE, CBLACK );
	}
	else
	{
		dis6x12( x, y, str, 0, CBLACK, CWHITE );
	}
}

void PutRect( int x, int y, int w, int h )
{
	Glib_Rectangle( x, y, w, h, CBLACK );
}

void PutLine( int x, int y, int x1, int y1 )
{
	Glib_Line( x, y, x1, y1, CBLACK );
}

void PutBox( int x, int y, int w, int h )
{
	Glib_FilledRectangle( x, y, w, h, CWHITE );
}

void PutRectV( int x, int y, int w, int h, char bTrans )
{
	int aClr = bTrans ?CWHITE : CBLACK;
	Glib_Rectangle( x, y, w, h, aClr );	
}

void PutLineV( int x, int y, int x1, int y1 )
{
	Glib_Line( x, y, x1, y1, CBLACK );	
}

#endif


#ifndef  ENM_VER_TARGET
void Uart_SendString(char *pt)
{
}
#endif


///< 0: center, 1: left, 2: right
void  PadFormat( char *pSrc, char *pDst, short width, char aligned)
{
	short i, aLen , aPre, aPost;

	aLen = strlen( pSrc );

	if ( aligned == 0 )
	{
		aPost = (width - aLen) / 2;
		aPre = (width -aLen - aPost );
	}
	else if ( aligned == 1 )	///< left
	{
		aPre = 0;
		aPost = width - aLen;
	}
	else 
	{
		aPre = width - aLen;
		aPost = 0;
	}

	/// pre blank char
	for ( i = 0; i < aPre; i++ )
	{
		sprintf( &pDst[i], "%c", ' ' );
	}

	/// main body
	strcpy( &pDst[i], pSrc );

	/// post blank char
	for ( i = 0; i < aPost; i++ )
	{
		sprintf( &pDst[i+aLen+aPre], "%c", ' ' );
	}
}

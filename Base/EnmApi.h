// Date declare file

#ifndef __ENM_API_H__
#define __ENM_API_H__

// include section
#include "EnmDef.h"
#include "Matrix.h"

#ifdef ENM_VER_TARGET
#include "include.h"
#include  "key&motor.h"
#include  "rtc.h"
#endif


#ifdef ENM_VER_TARGET
#define		EnmRun( routines )  routines
#define		EnmDelay( tm )  Delay( (tm) )
#else
#define		EnmRun( routines )  
#define		EnmDelay( tm )
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 *    Device status Manage Routines
 *
 */
tagDate	EnmGetDate( void );
tagTime	EnmGetTime( void );
unsigned long EnmGetTick();

void 	EnmSetDate( tagDate date );
void 	EnmSetTime( tagTime time );

/// dealy seconds time
void PDelay( unsigned short seconds );

/// Format Float point data
/// param: val ---data, pDst ---buffer for formatted string,
/// m --- totol count, n -- frag count
void 	FormatFP( double val, char * pDst, unsigned char m, unsigned char n );
void  FormatFP2( double val, char * pDst, unsigned char m, unsigned char n );
double Str2FP( char *pStr );
 
#if 1
#define	BIT( val, idx ) 	 (((val)>>(idx))&0x01 )
#define	SetBit( val, idx ) 	((val) |= (0x01 << idx))
#define ResetBit( val, idx )  ((val) &= ~(0x01 << idx))
#else
extern unsigned char BIT( long val, char idx );
extern void	SetBit( long val, char idx ) ;
extern void	ResetBit( long val, char idx ) ;
#endif
 
/**
 *    Moter Routines
 *
 */
 
/// Micro Define
#define		MOTOR_ONE   		60
#define		MOTOR_STEP  		60

/// E20
#if 1
#define		ENM_PLATE_MAX_STEP		1000
#define		ENM_LIGHT_MAX_STEP		400

/// Filter width(mm)
#define 	ENM_LIGHT_STEP_LEN		10		

#else
/// E10
#define		ENM_PLATE_MAX_STEP		1670
#define		ENM_LIGHT_MAX_STEP		1200
#endif

void	EnmCfgPlateMotor( tagMotorInfo info );
void 	EnmCfgLightMotor( tagMotorInfo info );
 
void  Plate_Motor_On( char bOn );
void  Light_Motor_On( char bOn ); 
char  EnmFilterReset( char bBack );				///< return to start position
unsigned short  EnmGetFilterPos( void);		///< get  current  position
void  EnmFilterForward( void);
void  EnmFilterBackward( void);
BYTE  EnmFilterPos(  unsigned short pos );
void  EnmFilterPrePos( void);	///<  At the first filter pos
 
char  EnmPlateReset( char bBack );				///< return to start position
unsigned short  EnmGetPlatePos( void);		///< get  current  position
void  EnmPlateForwardOne( void);
void  EnmPlateForward( void);
void  EnmPlateBackwardOne( void);
void  EnmPlateBackward( void);
void  EnmPlatePrePos( void);	///<  before the first sampling pos


/// Plate shake
void	EnmPlateShake( unsigned char  mode, unsigned short  tm );	///< shake plate, tm's unit: second


/**
 *    ADC Sampling  Routines
 */
void  EnmADC_Init( unsigned char amp );
unsigned short	  EnmSampling(  ENMADC  *pDstData );	///< return: sampled data's count

/**
 *    print  Routines
 */
char	  EnmPrntInit(  void );						///< printer LF
char	  EnmPrntLF(  void );	///< printer LF
char	  EnmPrntCR(  void );	///< printerCR
char	  EnmPrntSetCharInterval(  unsigned char pixels );
char	  EnmPrntSetLineInterval(  unsigned char pixels );
char	  EnmPrntSetCharSet(  unsigned char charset );	///<  0 ---English  1 ---Chinese  2 ---User define
char	  EnmPrntVLine(  unsigned short  from, unsigned short to );
unsigned char  EnmPrntOneLine(  unsigned char  x, char *pData, char bLF );
char	  EnmPrntPaperForward(  unsigned char  lineCnt );	///< paper forward
void 	  EnmPrntCRLF( void );

unsigned char PrntReadStatus( void );
unsigned char PrntIsBusy( void );

/**
 *    display  Routines
 */
void ClearScreen( unsigned char clr );
//void PutPixel( int x, int y, long clr );
void PutStr( int x, int y, char * str , char bSel/*, unsigned short pos*/ );
void PutStrThin( int x, int y, char * str , char bSel/*, unsigned short pos*/ ); ///< little font size
void PutRect( int x, int y, int w, int h );
void PutLine( int x, int y, int x1, int y1 );
void PutBox( int x, int y, int w, int h );

void PutLineV( int x, int y, int x1, int y1 );
void PutRectV( int x, int y, int w, int h, char bTrans );


///< 0: center, 1: left, 2: right
void  PadFormat( char *pSrc, char *pDst, short width, char aligned);


#ifdef __cplusplus
}
#endif

#endif  // __ENM_API_H__

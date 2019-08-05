#include "include.h"
extern int Image$$RO$$Limit;
extern int Image$$RW$$Base;

void Isr_Init(void);

//****** *********************************************************************

//***************************************************************************

#include "EnmApp.hpp"
#include "EnmBeeper.hpp"
#include "EnmPlateMotor.h"
#include "EnmFilterMotor.h"
#include "..\\MUI\\MStrMngr.h"
#include "..\\App\\EnmStringTbl.hpp"

#include "EnmTestKit.h"


void  __irq Time5Done()
{
	HaveRTC = 1;
	rI_ISPC = BIT_TIMER5;
	
}

void InitSysTimer(unsigned short Freq)
{	

#if 0
	rTCON   &= 0xfffffff0;			//clear manual update bit, stop Timer 5 	
	rTCFG0  &= 0xffffff00;			//��ʱ��4/5��Ԥ��ƵΪ1
	rTCFG0  |= (250-1)<<0;
	rTCFG1  &= 0xfffffff0;			//��ʱ��5��MUXΪ1/16
	rTCFG1  |= 3<<0;	
	rTCNTB0  = 10 * Freq;//��ʱ��5����װֵ,��ENABLE֮ǰ�趨	
	rTCMPB0  = 0;//��ʱ��5����װֵ,��ENABLE֮ǰ�趨	
	rTCON   |= 0x00000005;			//��ʱ��5 start, ��ΪINTERVALģʽ
	pISR_TIMER0 = (unsigned)Time5Done;
#else	
	rTCON   &= 0xf0ffffff;			//clear manual update bit, stop Timer 5 	
	rTCFG0  &= 0xff00ffff;			//��ʱ��4/5��Ԥ��ƵΪ1
	rTCFG0  |= (250-1)<<16;
	rTCFG1  &= 0xff0fffff;			//��ʱ��5��MUXΪ1/16
	rTCFG1  |= 3<<20;	
	rTCNTB5  = 10 * Freq;//��ʱ��5����װֵ,��ENABLE֮ǰ�趨	
	rTCON   |= 0x02000000;			//��ʱ��5��MANUAL UPDATE BIT��Ϊ1
	rTCON   &= 0xf0ffffff;			//MANUAL UPDATE BIT����
	rTCON   |= 0x05000000;			//��ʱ��5 start, ��ΪINTERVALģʽ
	pISR_TIMER5 = (unsigned)Time5Done;
#endif
	
}


int Main(void)
{	
	rINTMSK = 0x07ffffff;
    rSYSCFG = CACHECFG;
    //8K�ֽ�cache��д����ʹ�ܣ�data abortʹ��
	rNCACHBE0 = ((Non_Cache_End>>12)<<16)|(Non_Cache_Start>>12);
   
    //���������������ʹ�ø��ٻ���
    Port_Init();    
	ChangePllValue( 24, 2, 1 );		//����8MHz���޸�ϵͳ��ƵΪ4��Ƶ(32MHz)    

  	Rtc_Init();
    EnmADC_Init( 2 );	/// init AD converter   
    EnmPrntInit();	/// init printer 
    Uart_Init( 0, 9600 );
    Uart_Select( 0 );
 
 	Lcd_Init(MODE_G16);	/// Init LCD
    Glib_Init(MODE_G16);
 
  	Glib_ClearScr( CWHITE );
	DisplayTrigOn();
		
	//test plate motor
	//TestPlateTripmeter();
	//TestLightTripmeter();
	//TestPlateAPI();
	//TestLightMotorAPI();
	//TestFlashDrv();
	//TestFs();
	//TestADC();
	//TestKey();
	//TimerTest();
	//TestSerailPort();
	//TestFile();
	//TestTempSensor();
	//TestMotorPlate();

#if 0	
	DisplayTrigOn();
	MCHAR* apStr = MStrMngr::Instance().GetString( STR_STARTUP_CHECK );
	PutStr( 20,100, apStr, CBLACK );

	//////////////////////////////////////////////////////////////////////
	/// Reset device
	//////////////////////////////////////////////////////////////////////
	/// Reset Filter motor to start pos
#if 1
	if ( CEnmPlateMotor::Instance().IReset( TRUE ) == -1 )
	{
		apStr = MStrMngr::Instance().GetString( STR_STARTUP_ERROR );
		PutStr( 20,100, apStr, 0 );
		while(1);		
	}
  	
	/// Reset Plate motor to start pos
	if ( CEnmFilterMotor::Instance().IReset( TRUE ) == -1 )
	{
		apStr = MStrMngr::Instance().GetString( STR_STARTUP_ERROR );
		PutStr( 20,100, apStr, 0 );
		while(1);	
	}
 #endif 
 
    /// 
  	DisplayTrigOff();
#endif
  	
   	CEnmApp  &theApp = CEnmApp::Instance();
   	theApp.InitApp();

   	DisplayTrigOn();
  	  	
  	//InitSysTimer( 1000 );
 	//rINTMSK =~(BIT_GLOBAL | BIT_TIMER5 ); 
 	//rINTMSK =~(BIT_GLOBAL | BIT_TICK);
  	//rINTCON = 0x01;
  	
	CEnmBeeper::ParttenBeep( BEEP_HINT );

  	theApp.AppRun();
   
    return 0;	
}

//*****************************************************************************
/**/
int pIrqStart 	=  0;
int pIrqHandler = 0;
int pIrqFinish 	= 0;


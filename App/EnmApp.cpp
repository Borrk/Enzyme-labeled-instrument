/**
 * @file   f:\Workshop\EnMarker\Enm\App\CEnmApp.cpp
 * @brief 
 *
 * @purpose 
 * 
 *
 * @author  
 * @date    2004/09/08
 */

#include "EnmFs.h"

#include "EnmBeeper.hpp"
#include "EnmApp.hpp"

/// Work flow steps
#include "EnmWfo.h"
#include "EnmWfsFactory.h"

/// Mode
#include "EnmModeLib.h"
#include "EnmSysCfgLib.h"
#include "EnmCutoffLib.h"
#include "EnmPlateMotor.h"
#include "EnmFilterMotor.h"

#ifndef  ENM_VER_TARGET
#include "..\\WinSimu\\SimuScanEngine.h"
#endif


INT16U		CEnmApp::mMsg = 0;
enEnmSysMode CEnmApp::mSysMode = ENM_MODE_ALONE;


enEnmSysMode  CEnmApp::GetSysMode()
{
	return mSysMode;
}

char  IsTimerOut( )
{
	char aTimeOut = 0;

#if 1
	static unsigned long aLast = EnmGetTick();
	
	unsigned long aNow = EnmGetTick();
	if ( aNow - aLast >= 1 )
	{
		aTimeOut = 1;
		aLast = aNow;
	}
	else if ( aNow < aLast ) ///< in case the time is setted by user
	{
		aLast = aNow;
	}

#else
	tagTime aNow;
	static tagTime aLast = EnmGetTime( );
	
	aNow =  EnmGetTime( );
	if ( (aNow.hour * 3600 + aNow.min * 60 + aNow.sec) - 
	     (aLast.hour * 3600 + aLast.min * 60 + aLast.sec) >= 1 )
	{
		aTimeOut = 1;
		aLast = aNow;
	}
	else
	{
		aTimeOut = 0;
	}	
#endif

	return aTimeOut;	
}


static long nTimeInterval( tagTime* oldkeyTime, tagTime* keyTime )
{
	long t1, t2;
	
	t1 = oldkeyTime->sec + oldkeyTime->min *60 + oldkeyTime->hour * 3600;
	t2 = keyTime->sec + keyTime->min *60 + keyTime->hour * 3600;
	
	return t2 - t1;	
}

INT16U nGetKey()
{
	INT16U  KeyVal;
	static tagTime oldkeyTime, keyTime;
	static char bFirst = 1;

	if ( bFirst )
	{
		bFirst = 0;
		oldkeyTime = EnmGetTime();
	}

	keyTime = EnmGetTime( );
	INT32U l = nTimeInterval( &oldkeyTime, &keyTime );
	if ( 1 <= l && l <= 2 )
	{
	#ifdef ENM_VER_TARGET
			KeyVal = Scan_KEY( );
	#endif
	}
	else
	{
		KeyVal = -1;
	}

	oldkeyTime = keyTime;

	return KeyVal;
}



//////////////////////////////////////////////////////////////////////////
/// 
/// 
//////////////////////////////////////////////////////////////////////////

CEnmApp::CEnmApp()
{
	Init();
}

CEnmApp::~CEnmApp()
{
}

CEnmApp& CEnmApp::Instance()
{
	static CEnmApp sInst;
	return sInst;
}

void CEnmApp::Terminate()
{
	/// Save Mode
	CEnmSysCfgLib::Instance().Unload();
	CEnmModeLib::Instance().Unload();
	CEnmCutoffLib::Instance().Unload();

	delete mpKeyEngine;
	delete mpScanEngine;
	
	efUnmount();
}

///< Return: 1 ---ok, 0 ---failed
#ifdef ENM_VER_TARGET
extern unsigned char  DisplayTrigOn();
extern unsigned char DisplayTrigOff();
#else
unsigned char  DisplayTrigOn(){ return 1; }
unsigned char DisplayTrigOff(){ return 1; }
#endif

INT8U  CEnmApp::InitApp(void)
{

	/// Init File System
#if 0
	efMount( NF_ERASE );
#else
 	efMount( NF_NORMAL );
#endif
	
	CEnmSysCfgLib& rSysCfgLib = CEnmSysCfgLib::Instance();
	rSysCfgLib.Load();
	
	/// Load Saved Cutoff 
	CEnmCutoffLib& rCutoffLib = CEnmCutoffLib::Instance();
	rCutoffLib.Load();

	/// Load Saved program and test data
	CEnmModeLib &rModLib = CEnmModeLib::Instance();
	rModLib.Load();

// for debug
#if 0
	mScanContext.mProtocol.stMeasure.kFactor = 1.2345;
	mScanContext.mProtocol.stMeasure.uID = 1;
	rModLib.Add(  mScanContext.mProtocol );
	mScanContext.mProtocol.stMeasure.uID = 2;
	mScanContext.mProtocol.stMeasure.uMeasMode = MEASURE_QUALITATIVE;
	mScanContext.mProtocol.stMeasure.kFactor = 2.345;
	rModLib.Add(  mScanContext.mProtocol );
	mScanContext.mProtocol.stMeasure.uID = 3;
	mScanContext.mProtocol.stMeasure.kFactor = 3.4567;
	rModLib.Add(  mScanContext.mProtocol );
	
	rModLib.Sync();
#else
	tagProtocol aProt;

	rModLib.GetAt(  0, aProt );
	rModLib.GetAt(  1, aProt );
	rModLib.GetAt(  2, aProt );

#endif
	
	/// Set String Table
	enMigLangue aLanguage = (enMigLangue)CEnmSysCfgLib::Instance().GetSysCfg().uLanguage;
	MStrMngr::Instance().SetLangue( aLanguage );
	MStrMngr::Instance().SetStrTable( (MCHAR**)::gGetStringTbl(), ::gGetStrTblItemCount() );

	DisplayTrigOn();
	MCHAR* apStr = MStrMngr::Instance().GetString( STR_STARTUP_CHECK );
	PutStr( 20,100, apStr, 0 );

	/// Reset Filter motor to start pos
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
   	DisplayTrigOff();

	/// Add Engine
	mpKeyEngine = new CEnmKeyEngine;

#ifdef  ENM_VER_TARGET
	mpScanEngine = new CEnmScanEngine;
#else
//	mpScanEngine = new CSimuScanEngine;
	mpScanEngine = new CEnmScanEngine;
#endif

	mMainProc.InitEngine();
	mConsoleProc.InitEngine();
	mpScanEngine->InitEngine();
	mpKeyEngine->InitEngine();

	RegisterEngine( mMainProc );
//	RegisterEngine( mConsoleProc );
	RegisterEngine( *mpScanEngine );
	RegisterEngine( *mpKeyEngine );


	/// Config Motor paramter
	tagDeviceOption& rDevOpt = rSysCfgLib.GetDevOpt();
	tagMotorInfo aInfo = rDevOpt.stMotorOpt.stPlateInfo;
	aInfo.fPreDist	= rDevOpt.stMotorOpt.stPlateInfo.fPreDist+ rDevOpt.stMotorOpt.fPlateMotorDist; 
	CEnmPlateMotor::Instance().SetMotorInfo( aInfo );

	aInfo = rDevOpt.stMotorOpt.stLightInfo;
	aInfo.fPreDist	= rDevOpt.stMotorOpt.stLightInfo.fPreDist+ rDevOpt.stMotorOpt.fLightMotorDist;
	CEnmFilterMotor::Instance().SetMotorInfo( aInfo );

	return 1;
}

void  CEnmApp::Init( void)
{	
	INT8U i;
		
	/// Init Engine
	for ( i = 0; i < APP_ENGINE_MAX; i++ )
	{
		mProcEngineList[i]	= NULL;						
	}
	mProcEngineCnt = 0;

	mpScanEngine = NULL;
	mpKeyEngine  = NULL;
}

void  CEnmApp::AppRun( void )
{
	unsigned short KeyVal = -1;
	MigPage *apLastPage = NULL; 
	
	unsigned short kk, k;
	ClearScreen( WHITE);
	char aStr[123];
	static int y = 0;

	long aSaveFlag = 0;
	#ifdef  ENM_VER_TARGET
	while ( 1 )
	#endif
	{
		static INT16U	aCounter = 0;
		aCounter++;
		if ( GetEngineCnt() > 0 || aCounter > 40 )
		{
			MigPage::mpCurPage->Update();
			aCounter = 0;
		}

		for ( U16 i = 0; i < GetEngineCnt(); i++ )
		{
			mProcEngineList[i]->IEnterProc();

	#ifdef ENM_VER_TARGET
		if ( apLastPage !=  MigPage::mpCurPage )
			ClearScreen( WHITE);
	#endif		
		apLastPage =  MigPage::mpCurPage;	
		}
	}		
}


BOOLEAN  CEnmApp::ChangeMode( enEnmSysMode mode )
{
	BOOLEAN bChanged = FALSE;

	if ( mode == GetSysMode() )
		return TRUE;

	if ( mode == ENM_MODE_PC )
	{
		mSysMode = ENM_MODE_PC;
		mMainProc.ISuspend( -1, TRUE );
		mConsoleProc.IResume();
		bChanged = TRUE;
	}
	else
	{
		mSysMode = ENM_MODE_ALONE;
		mConsoleProc.ISuspend( -1, TRUE );
		mMainProc.IResume( );
		bChanged = TRUE;
	}

	return bChanged;
}


BOOLEAN	 CEnmApp::RegisterEngine( IProcEngine& rEngine )
{
	if ( mProcEngineCnt >= APP_ENGINE_MAX )
		return FALSE;

	mProcEngineList[mProcEngineCnt++] = &rEngine;

	return TRUE;
}

void	 CEnmApp::UnRegisterEngine( IProcEngine& rEngine )
{
	INT16U aIdx = APP_ENGINE_MAX + 1;

	/// Find 
	INT16U i;
	for ( i = 0; i < mProcEngineCnt; i++ )
	{
		if ( mProcEngineList[i] == &rEngine )
		{
			aIdx = i;
			break;
		}
	}

	/// Remove	
	for ( i = aIdx; i < mProcEngineCnt - 1; i++ )
	{
		mProcEngineList[i] = mProcEngineList[i+1];
	}
	mProcEngineList[mProcEngineCnt] = NULL;

	if ( mProcEngineCnt > 0 )
		mProcEngineCnt--;
}

#include <math.h>
#include "EnmconsolePage.h"
#include "EnmScanContext.h"
#include "EnmApp.hpp"
#include "MigEventMng.h"
#include "EnmSysCfgLib.h"


CEnmConsolePage::CEnmConsolePage()
	:inherited( "", -1 )
{
	Init();
}

CEnmConsolePage::CEnmConsolePage(const tagCS& cs )
: inherited( cs )
{ 
	Init();
}

BOOLEAN  CEnmConsolePage::IsCanExit()
{
	return FALSE;
}

void  CEnmConsolePage::SetConsole( CEnmConsole* pConsole )
{
	mpConsole = pConsole;
}

tagScanData*	CEnmConsolePage::GetScanData()
{
	tagScanData *apData = GetScanContext().GetScanData();
	return mbHaveData ? apData : NULL;
}

void CEnmConsolePage::Init(void)
{
	mbInScan = FALSE;
	mbHaveData = FALSE;

	tagCS  cs( ENM_CTRL_LABEL,	 20, 120, 260,	20, ENM_OBJ_FRAME|ENM_OBJ_TITLE | ENM_CTRL_STATIC, ENM_CHECKTIP_TIP, ENM_T(ENM_CHINESE, "正在检测样品，请稍候...:", "Scanning, Please Wait...") );
	mpTips = AddControl( cs );

	mpTips->Move( 30, 200 );
	SetTip( " " );
}

void CEnmConsolePage::SetTip( MCHAR* pTip )
{
	if ( pTip )
		mpTips->SetTitle( pTip );
}

void CEnmConsolePage::Draw(void)
{
	inherited::Draw();

	/// show all the scan option
	INT16U x = 45, y = 40;
	INT16U step = 24;
	INT16U aCnt;
	MCHAR aszTip[128];

	ConsoleOption aOption;
#ifdef ENM_VER_TARGET
	aOption = mpConsole->GetOption();
#endif

	/// Title
	PutRect( x - 10, y, 320 - 40 - 30, step * 6 );

	sprintf( aszTip, GetString( STR_PPCMODE_SCAN_OPTION ) );
	PutStr( x + 70, y - 7, aszTip, 0 );
	y += step;

	tagSystemCfg&  rSysCfg = CEnmSysCfgLib::Instance().GetSysCfg();

	/// filter
	MCHAR *p1, *p2, *p3;
	p1 = GetString( STR_PPROGRAM_FILTER );

	p2  = rSysCfg.szFilter[aOption.wave];
	sprintf( aszTip, "%s %s", p1, p2 );
	PutStr( x, y, aszTip, 0 );
	y += step;

	/// shake plate 
	MCHAR	*apSpeed[] = 
	{ 
		GetString( 	STR_MIXMODE_SLOW),
		GetString( 	STR_MIXMODE_MIDDLE),
		GetString( 	STR_MIXMOE_FAST),
	};

	p1 = GetString( STR_PPCMODE_MIXING_MODE );
	p2 = GetString( STR_TIME );
	p3 = GetString( STR_PPCMODE_SECOND );
	sprintf( aszTip, "%s %s  %s %d%s", p1, apSpeed[aOption.shakeSpeed], p2, aOption.shakeTime, p3 );
	PutStr( x, y, aszTip, 0 );
	y += step;

	p1 = GetString( STR_PPCMODE_START_COLUMN );
	p2 = GetString( STR_PPCMODE_COLUMN_COUNT );
	sprintf( aszTip, "%s %d  %s %d", p1, aOption.startColumn + 1, p2,aOption.columnCnt );
	PutStr( x, y, aszTip, 0 );
	y += step;
}

void    CEnmConsolePage::FocusOn()
{ 
	mbHaveData = FALSE;
	SetTip( "  " ); 

	CEnmApp::Instance().ChangeMode( ENM_MODE_PC );
	MigEventMng::Instance().PushMsg( ENM_MSG_MODE_CHANGE, ENM_MODE_PC, 0 );

	inherited::FocusOn();
}

void CEnmConsolePage::OnUserMsg( const tagMigMSG& stMsg )
{		
	INT16U x = 45, y = 40;
	INT16U step = 24;
	tagRect rc;

	rc.left = x - 10;
	rc.top = y;
	rc.right = 320 - 40 - 30;
	rc.bottom = step * 6;

	switch ( stMsg.msg )
	{
	case ENM_MSG_CFG_CHG:
		InvalidateRect( rc, TRUE );
		break;
		
	case ENM_MSG_SCAN:
	//	ClearScreen( WHITE );
		InvalidateRect( rc, TRUE );
		SetTip( GetString(STR_PPCMODE_HINT_TESTING) );
		Scan();
		SetTip( GetString(STR_PPCMODE_HINT_TESTED) );
		break;

	case ENM_MSG_SCAN_OK:
		/// transfer to console proc 
		MigEventMng::Instance().PushMsg( ENM_MSG_SCAN_OK, stMsg.param1, stMsg.param2 );
	}
}

void CEnmConsolePage::Scan( void )
{
	mbHaveData = FALSE;

	GetScanContext().Reset();
	GenerateScanOption();
	StartScan();

	mbHaveData = TRUE;
}

void CEnmConsolePage::GenerateScanOption()
{	
	ConsoleOption aOption = mpConsole->GetOption();
	tagMeasure *apMeas = &GetScanContext().mProtocol.stMeasure;
	apMeas->uMeasMode	 = MEASURE_COMMON;
	apMeas->uMainFilter  = aOption.wave;
	apMeas->uReferFilter = 0;
	apMeas->uMixingMode  = aOption.shakeSpeed;
	apMeas->uMixingTime  = aOption.shakeTime;
	apMeas->uID			 = -1;

	U32 aMask1 = (U32)pow( 2.0, (FP64)aOption.startColumn ) - 1;
	U32 aMask2 = (U32)pow( 2.0, (FP64)aOption.startColumn + aOption.columnCnt ) - 1;
	apMeas->uColumnMask = aMask2 & (~aMask1);
}

BOOLEAN CEnmConsolePage::StartScan()
{
	CEnmApp& rApp = CEnmApp::Instance();
	if ( rApp.GetScanEngine() )
	{
		rApp.GetScanEngine()->Attach( *this );
		GetScanContext().SetFlag( SC_SCAN_FULL );
		rApp.GetScanEngine()->StartScan( &rApp.GetScanContext(), TRUE );

		mbInScan = TRUE;
	}
	else
	{
		mbInScan = FALSE;
	}

	return mbInScan;
}

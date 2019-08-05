#include <math.h>
#include "EnmFmCmPage.h"
#include "EnmBase.hpp"
#include "EnmApp.hpp"
#include "EnmWfo.h"

CEnmFmCmModePage::CEnmFmCmModePage( )
: CEnmFmModePage( PID_FM_WP_SETUP )
{
	SetTitle( STR_PCOLUMN_TITLE );
	CreateControls();
}

void CEnmFmCmModePage::CreateControls()
{
	INT32U aItem[] = 
	{
		STR_BLANK,
		STR_PWHOLE_FILTER,
		STR_PWHOLE_WL,
		STR_PWHOLE_COLUMNS,
		STR_PWHOLE_MIX_TIME,
		STR_PWHOLE_MIX_MODE,
		STR_PCOLUMN_PERIOD,
	}; 
	
	U16 aLeft = mLeftMargin;
	INT32U aStyleStatic = ENM_CTRL_NORMAL | ENM_CTRL_STATIC ; 

	tagCS aCS[] = 
	{
		tagCS( ENM_CTRL_LABEL, 		aLeft,	mTopMargin+20*0,	60,	20,  aStyleStatic, 1,	0, aItem[1] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft+60,	mTopMargin+20*0,	20,	20,  aStyleStatic, 2,	0, aItem[0] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft+80,	mTopMargin+20*0,	40,20,  aStyleStatic, 3,	0, aItem[2] ),             
	
		tagCS( ENM_CTRL_LABEL, 		aLeft,	mTopMargin+20*1,	80,20,  aStyleStatic, 3,	0, aItem[3] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft,	mTopMargin+20*2,	160,20,  aStyleStatic, 4,	0, aItem[4] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft,	mTopMargin+20*3,	160,20,  aStyleStatic, 4,	0, aItem[5] ),             

		tagCS( ENM_CTRL_LABEL, 		aLeft,	mTopMargin+20*4,	160,20,  aStyleStatic, 4,	0, aItem[6] ),             
	
		tagCS( ENM_CTRL_UPDOWNBOX, 		aLeft+150,	mTopMargin+20*0,	40,20,  ENM_CTRL_NORMAL, 4,	0, STR_BLANK ),             
		tagCS( ENM_CTRL_UPDOWNBOX, 		aLeft+150,	mTopMargin+20*3,	20,20,  ENM_CTRL_NORMAL, 4,	0, STR_BLANK ),             

		tagCS( ENM_CTRL_EDITBOX, 		aLeft+150,	mTopMargin+20*1,	20,20,  ENM_CTRL_NORMAL, 3,	0, STR_BLANK ),             
		tagCS( ENM_CTRL_EDITBOX, 		aLeft+150,	mTopMargin+20*2,	20,20,  ENM_CTRL_NORMAL, 4,	0, STR_BLANK ),             
		tagCS( ENM_CTRL_EDITBOX, 		aLeft+150,	mTopMargin+20*4,	20,20,  ENM_CTRL_NORMAL, 4,	0, STR_BLANK ),             
	};

	MigControl *apCtrl = NULL;
	for ( INT8U i = 0; i < 7; i++ )
	{
		aCS[i].hParent = this;
		apCtrl = AddControl( aCS[i] );
	}

	/// Create filter list updown box
	tagSystemCfg& rSyscfg = CEnmSysCfgLib::Instance().GetSysCfg();
	U16  aFilterCnt = 0;
	MCHAR*  aListItem[FILTER_MAX] = { 0 };

	for ( U16 n = 0; n < FILTER_MAX; n++ )
	{
		if ( strcmp( rSyscfg.szFilter[n], "N/A" ) != 0 )
			aListItem[aFilterCnt++] = rSyscfg.szFilter[n];
	}

	mpMeasFilterCtrl = new MigUpDn( aCS[7] );
	AddControl( mpMeasFilterCtrl );
	mpMeasFilterCtrl->SetItems( aListItem, aFilterCnt );

	tagEditBoxCS  aECS;
	aECS.maxVal =  12;
	aECS.minVal = 1;
	aECS.iVal    = 12;
	aECS.maxChar = 2;
	aECS.type = ENM_EB_TYPE_INT;
	aECS.titleWidth = 1;

	/// Create editbox
	mpColumnCtrl = new MigEditBox( aCS[9], aECS );
	AddControl( mpColumnCtrl );

	aECS.maxVal =  25;
	aECS.minVal = 0;
	aECS.iVal    = 0;
	mpMixTimeCtrl = new MigEditBox( aCS[10], aECS );
	AddControl( mpMixTimeCtrl );

	U16 aItems[] = 
	{
		STR_MIXMODE_SLOW,
		STR_MIXMODE_MIDDLE,
		STR_MIXMOE_FAST,
	};

	mpMixModeCtrl = new MigUpDn( aCS[8] );
	mpMixModeCtrl->SetItems( aItems, sizeof(aItems) / sizeof(aItems[0]) );
	AddControl( mpMixModeCtrl );

	aECS.maxVal  =  20;
	aECS.minVal  = 2;
	aECS.iVal    = 2;
	mpPeriodCtrl = new MigEditBox( aCS[11], aECS );
	AddControl( mpPeriodCtrl );
}

void     CEnmFmCmModePage::FocusOff()
{
	GetCfgFromUI();
	
	inherited::FocusOff();
}

BOOLEAN  CEnmFmCmModePage::GetCfgFromUI( )
{
	tagMeasure *apMeas = &GetScanContext().mProtocol.stMeasure;

	apMeas->uMainFilter  = mpMeasFilterCtrl->GetCurSel();

	apMeas->uMixingMode  = mpMixModeCtrl->GetCurSel() + 1;
	apMeas->uMixingTime  = mpMixTimeCtrl->GetInt();
	apMeas->uPeriod = mpPeriodCtrl->GetInt();
	
	U8 aColumns = mpColumnCtrl->GetInt();
	if ( aColumns > 0 )
		aColumns--;
	apMeas->uColumnMask = (U32)1 << aColumns ;

	return TRUE;
}

/// CEnmFmKnModePage
CEnmFmKnModePage::CEnmFmKnModePage( )
: CEnmFmModePage( PID_FM_KN_MODE )
{
	SetTitle( STR_PKINETIC_MODE_TITLE );
	CreateControls();
}

void CEnmFmKnModePage::CreateControls()
{
	INT32U aItem[] = 
	{
		STR_BLANK,
		STR_PWHOLE_FILTER,
		STR_PWHOLE_WL,
		STR_PWHOLE_COLUMNS,
		STR_PWHOLE_MIX_TIME,
		STR_PWHOLE_MIX_MODE,
		STR_PKINETIC_MODE_PERIOD,
	}; 
	
	U16 aLeft = mLeftMargin;
	INT32U aStyleStatic = ENM_CTRL_NORMAL | ENM_CTRL_STATIC ; 

	tagCS aCS[] = 
	{
		tagCS( ENM_CTRL_LABEL, 		aLeft,	mTopMargin+20*0,	60,	20,  aStyleStatic, 1,	0, aItem[1] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft+60,	mTopMargin+20*0,	20,	20,  aStyleStatic, 2,	0, aItem[0] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft+80,	mTopMargin+20*0,	40,20,  aStyleStatic, 3,	0, aItem[2] ),             
	
		tagCS( ENM_CTRL_LABEL, 		aLeft,	mTopMargin+20*1,	80,20,  aStyleStatic, 3,	0, aItem[3] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft,	mTopMargin+20*2,	160,20,  aStyleStatic, 4,	0, aItem[4] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft,	mTopMargin+20*3,	160,20,  aStyleStatic, 4,	0, aItem[5] ),             

		tagCS( ENM_CTRL_LABEL, 		aLeft,	mTopMargin+20*4,	160,20,  aStyleStatic, 4,	0, aItem[6] ),             
	
		tagCS( ENM_CTRL_UPDOWNBOX, 		aLeft+150,	mTopMargin+20*0,	40,40,  ENM_CTRL_NORMAL, 4,	0, STR_BLANK ),             
		tagCS( ENM_CTRL_UPDOWNBOX, 		aLeft+150,	mTopMargin+20*3,	20,40,  ENM_CTRL_NORMAL, 4,	0, STR_BLANK ),             

		tagCS( ENM_CTRL_EDITBOX, 		aLeft+150,	mTopMargin+20*1,	20,20,  ENM_CTRL_NORMAL, 3,	0, STR_BLANK ),             
		tagCS( ENM_CTRL_EDITBOX, 		aLeft+150,	mTopMargin+20*2,	20,20,  ENM_CTRL_NORMAL, 4,	0, STR_BLANK ),             
		tagCS( ENM_CTRL_EDITBOX, 		aLeft+150,	mTopMargin+20*4,	40,20,  ENM_CTRL_NORMAL, 4,	0, STR_BLANK ),             
	};

	MigControl *apCtrl = NULL;
	for ( INT8U i = 0; i < 7; i++ )
	{
		aCS[i].hParent = this;
		apCtrl = AddControl( aCS[i] );
	}

	/// Create filter list updown box
	tagSystemCfg& rSyscfg = CEnmSysCfgLib::Instance().GetSysCfg();
	U16  aFilterCnt = 0;
	MCHAR*  aListItem[FILTER_MAX] = { 0 };

	for ( U16 n = 0; n < FILTER_MAX; n++ )
	{
		if ( strcmp( rSyscfg.szFilter[n], "N/A" ) != 0 )
			aListItem[aFilterCnt++] = rSyscfg.szFilter[n];
	}

	mpMeasFilterCtrl = new MigUpDn( aCS[7] );
	AddControl( mpMeasFilterCtrl );
	mpMeasFilterCtrl->SetItems( aListItem, aFilterCnt );

	tagEditBoxCS  aECS;
	aECS.maxVal =  12;
	aECS.minVal = 1;
	aECS.iVal    = 12;
	aECS.maxChar = 4;
	aECS.type = ENM_EB_TYPE_INT;
	aECS.titleWidth = 1;

	/// Create editbox
	mpColumnCtrl = new MigEditBox( aCS[9], aECS );
	AddControl( mpColumnCtrl );

	aECS.maxVal =  20;
	aECS.minVal = 0;
	aECS.iVal   = 2;
	mpMixTimeCtrl = new MigEditBox( aCS[10], aECS );
	AddControl( mpMixTimeCtrl );

	U16 aItems[] = 
	{
		STR_MIXMODE_SLOW,
		STR_MIXMODE_MIDDLE,
		STR_MIXMOE_FAST,
	};

	mpMixModeCtrl = new MigUpDn( aCS[8] );
	mpMixModeCtrl->SetItems( aItems, sizeof(aItems) / sizeof(aItems[0]) );
	AddControl( mpMixModeCtrl );

	aECS.maxVal =  1000;
	aECS.minVal = 20;
	aECS.iVal    = 20;
	aECS.maxChar = 4;
	mpPeriodCtrl = new MigEditBox( aCS[11], aECS );
	AddControl( mpPeriodCtrl );
}

void  CEnmFmKnModePage::OnKeyOK()
{
	inherited::OnKeyOK();
}

void     CEnmFmKnModePage::FocusOff()
{
	GetCfgFromUI();
	
	inherited::FocusOff();
}


//////////////////////////////////////////////////////////////////////////


CEnmFmKnMeasPage::CEnmFmKnMeasPage()
: CEnmBasePage( "", PID_FM_KN_MEASURE )
{
	SetTitle( STR_PKINETIC_MODE_TITLE );
	mTotalTime = 0;
}

void CEnmFmKnMeasPage::Draw()
{
	CEnmBasePage::Draw();
	DrawHints();
}

void FormatAbsorb2( FP64 val, MCHAR *pDst, U8 m, U8 n )
{
	FormatFP( val, pDst, m, n ); 
	U16 aLen;
	MCHAR *ap = strstr( pDst, "." );
	if ( ap )
	{
		ap++;
		aLen = strlen( ap );
		while ( aLen < n )
		{
			*(ap+aLen) = '0';
			*(ap+aLen+1) = '\0';
			aLen = strlen( ap );
		}
	
		ap = strstr( pDst, "." );
		ap++;
		if ( strlen(ap) > n )
		{
			*(ap+n) = '\0';
		}
	}
}

BOOLEAN  CEnmFmKnModePage::GetCfgFromUI( )
{
	tagMeasure *apMeas = &GetScanContext().mProtocol.stMeasure;

	apMeas->uMainFilter  = mpMeasFilterCtrl->GetCurSel();

	apMeas->uMixingMode  = mpMixModeCtrl->GetCurSel();
	apMeas->uMixingTime  = mpMixTimeCtrl->GetInt();
	apMeas->uPeriod      = mpPeriodCtrl->GetInt();
	
	S32 aColumns = mpColumnCtrl->GetInt();
	apMeas->uColumnMask = (U32)pow( 2.0, (FP64)aColumns ) - 1;

	return TRUE;
}

void CEnmFmKnMeasPage::DrawHints()
{
	MCHAR  aszHint[6][100] = { 0 };
	MCHAR  aszTemp[32] = {0};


	tagMeasure *apMeas = &GetScanContext().mProtocol.stMeasure;
	MCHAR *apszWL = CEnmSysCfgLib::Instance().GetSysCfg().szFilter[apMeas->uMainFilter];
	
	U16   aColumn = GetScanContext().GetScanColumns( TRUE );

	/// Measure info
	sprintf( aszHint[0], "%s %d", MLoadStr(STR_PCOLUMN_MONITOR_COLUMN), aColumn );
	sprintf( aszHint[1], "%s %s", MLoadStr(STR_PWHOLE_MEAS_WL), apszWL );
	sprintf( aszHint[2], "%s %d", MLoadStr( STR_PCOLUMN_MONITOR_TIME ), mTotalTime );
//	sprintf( aszHint[3], "%s", MLoadStr( STR_PCOLUMN_MONITOR_ABSORB ) );

	U16 i;
	/// Format Absorbance 
//	MCHAR *ap = aszHint[3];
//	FP64 aAbsord[12] = { 1.23456 };
//	for ( i = 0; i < 4; i++ )
//	{
//		sprintf( ap, " %c ", i + 'A' );
//		ap += 3;
//		FormatAbsorb2( aAbsord[i], ap, 6, 3 );
//		ap = aszHint[3] + strlen( aszHint[3] );
//	}
//
//	ap = aszHint[4];
//	for ( i = 4; i < 8; i++ )
//	{
//		sprintf( ap, " %c ", i + 'A' );
//		ap += 3;
//		FormatAbsorb2( aAbsord[i], ap, 6, 3 );
//		ap = aszHint[4] + strlen( aszHint[4] );
//	}
//
//	ap = aszHint[5];
//	*ap++ = ' '; 
//	for ( i = 8; i < 12; i++ )
//	{
//		sprintf( ap, " %c ", i + 'A' );
//		ap += 3;
//		FormatAbsorb2( aAbsord[i], ap, 6, 3 );
//		ap = aszHint[3] + strlen( aszHint[3] );
//		ap = aszHint[5] + strlen( aszHint[5] );
//	}

	/// draw
	U16 x = mLeftMargin + 20;
	U16 y = 60;
	U16 step = 20;

	for ( i = 0; i < 3; i++ )
	{
		PutStr( x, y, aszHint[i], 0 );
		y += step;
	}
}

void  CEnmFmKnMeasPage::FocusOn()
{
	StartupScan( 0 );

	inherited::FocusOn();
}

void CEnmFmKnMeasPage::FocusOff()
{
	inherited::FocusOff();

}

BOOLEAN  CEnmFmKnMeasPage::OnExit()
{
	BOOLEAN bRet = FALSE;

	CEnmApp& rApp = CEnmApp::Instance();
	if ( rApp.GetScanEngine() )
	{
		rApp.GetScanEngine()->StopScan( );
		GetScanContext().SetFlag( 0 );
	}

	mTotalTime = 0;

	inherited::OnExit();
	
	return bRet;
}

void	CEnmFmKnMeasPage::OnUserMsg( const tagMigMSG& stMsg )
{
	if ( stMsg.msg == ENM_MSG_SCAN_OK )
	{
//		if ( IWorkStep::GetActiveStep() == GetStep() )
//			GetStep()->NextStep();
		mTotalTime += GetScanContext().mProtocol.stMeasure.uPeriod;
	}
}

void CEnmFmKnMeasPage::StartupScan( BOOLEAN bFirst )
{	
	GetScanContext().mProtocol.stMeasure.uMeasMode = MEASURE_COMMON;
	CEnmApp& rApp = CEnmApp::Instance();

	if ( PROC_IDLE != rApp.GetScanEngine()->GetCurState() && (SC_SCAN_KINETIC == GetScanContext().GetFlag()) )
		return;

	mTotalTime = 0;
	
	if ( rApp.GetScanEngine() )
	{
		rApp.GetScanEngine()->Attach( *this );
		GetScanContext().SetFlag( SC_SCAN_KINETIC );
		rApp.GetScanEngine()->StartScan( &rApp.GetScanContext(), TRUE );
	}

	if ( bFirst )
	{
//		mbInScan = TRUE;
//		memset( mConcData, 0, sizeof(mConcData) );
	}
}

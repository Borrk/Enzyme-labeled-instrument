// EnmModePage.cpp: implementation of the CEnmProgramScanPage class.
//
//////////////////////////////////////////////////////////////////////

#include <math.h>
#include "EnmProgramScanPage.h"
#include "EnmApp.hpp"
#include "EnmScanContext.h"
#include "EnmModeLib.h"
#include "IWorkStep.h"


CEnmProgramScanPage::CEnmProgramScanPage(  )
: CEnmModeBasePage( "", 0 )
{
	mTipInfo.mTipID = -1;
	mbInScan = FALSE;
}

CEnmProgramScanPage::CEnmProgramScanPage( MCHAR* pTitle, U32 id )
: CEnmModeBasePage( pTitle, id )
{
	mTipInfo.mTipID = -1;
	mbInScan = FALSE;

	SetFuncKey( ENM_FUNCKEY_GO, FALSE, TRUE, STR_STEP_GO );
	SetFuncKey( ENM_FUNCKEY_EXIT, TRUE, TRUE, STR_STEP_ABORT_MEAS );
	SetFuncKey( ENM_FUNCKEY_PCOPY, FALSE, TRUE, STR_STEP_HCOPY );
}

CEnmProgramScanPage::~CEnmProgramScanPage()
{
}

BOOLEAN	 CEnmProgramScanPage::IsCanForward()
{
	return !mbInScan;
}

void CEnmProgramScanPage::ShowTips( const tagTipInfo& tipInfo )
{
	mTipInfo = tipInfo;
}

void  CEnmProgramScanPage::DrawTips()
{
	if ( mTipInfo.mTipID == -1 )
		return;

	MCHAR* apStr = MLoadStr( mTipInfo.mTipID );
	if ( apStr )
	{
		PutBox( mLeftMargin, 160, 200, 30 );
		PutStr( mLeftMargin + 20, 166, apStr, 0 );
	}
}

void CEnmProgramScanPage::FocusOn()
{
	CEnmModeBasePage::FocusOn();
	
	CEnmScanContext& aProt = GetScanContext();
	U16 aCount = aProt.GetSampleCount( TRUE );
	if ( aCount )
	{
		aCount = (aCount / 8) + (aCount%8 ? 1 : 0);
		aProt.mProtocol.stMeasure.uColumnMask = (U32) (pow(2.0, aCount) - 1);
	}
	else
		aProt.mProtocol.stMeasure.uColumnMask = 0;

	Draw();
	
	CEnmApp& rApp = CEnmApp::Instance();
	if ( rApp.GetScanEngine() )
	{
		mbInScan = TRUE;

		rApp.GetScanEngine()->Attach( *this );
		GetScanContext().SetFlag( SC_SCAN_NORMAL );
		rApp.GetScanEngine()->StartScan( &rApp.GetScanContext(), TRUE );
	}
}

void CEnmProgramScanPage::FocusOff()
{
	CEnmModeBasePage::FocusOff();

	CEnmApp& rApp = CEnmApp::Instance();
	if ( rApp.GetScanEngine() )
	{
		rApp.GetScanEngine()->StopScan();
		GetScanContext().SetFlag( 0 );
	}

	mbInScan = FALSE;
}

void	CEnmProgramScanPage::OnUserMsg( const tagMigMSG& stMsg )
{
	if ( stMsg.msg == ENM_MSG_SCAN_OK )
	{
		mbInScan = FALSE;
		GetStep()->NextStep();
	}
}

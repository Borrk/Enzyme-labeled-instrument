// EnmDataCompute.cpp: implementation of the CEnmDataCompute class.
//
//////////////////////////////////////////////////////////////////////

#include "EnmCutoffCompute.h"
#include "EnmConcDataUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnmCutoffCompute::CEnmCutoffCompute()
:CEnmBasePage( "", ENM_PAGE_ID_CUTOFFPROC )
{
	mCutoffCount = 0;
	
	CreateControls();	
}

CEnmCutoffCompute::~CEnmCutoffCompute()
{
}

void CEnmCutoffCompute::CreateControls( void)
{
	INT32U aItem[] = 
	{
		STR_PDATACOMPUTE_TIPS,
		STR_PDATACOMPUTE_SELECTCUTOFF,
		STR_PDATACOMPUTE_CONTROL_NC,
		STR_PDATACOMPUTE_CONTROL_PC,
	}; 
	
	U16 aLeft = mLeftMargin + 30;
	INT32U aStyleStatic = ENM_CTRL_NORMAL | ENM_CTRL_STATIC ; 

	tagCS aCS[] = 
	{
		tagCS( ENM_CTRL_LABEL, 		aLeft,	mTopMargin+20*0,	90,	20,  aStyleStatic, 1,	0, aItem[0] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft,	mTopMargin+20*1,	90,	20,  aStyleStatic, 2,	0, aItem[1] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft,	mTopMargin+20*3,	100,20,  aStyleStatic, 3,	0, aItem[2] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft,	mTopMargin+20*4,	100,20,  aStyleStatic, 4,	0, aItem[3] ),             
	
		tagCS( ENM_CTRL_UPDOWNBOX, 		aLeft+40,	mTopMargin+20*2,	120,20,  ENM_CTRL_NORMAL, 4,	0, STR_BLANK ),             
		tagCS( ENM_CTRL_EDITBOX, 		aLeft+100,	mTopMargin+20*3,	100,20,  ENM_CTRL_NORMAL, 3,	0, STR_BLANK ),             
		tagCS( ENM_CTRL_EDITBOX, 		aLeft+100,	mTopMargin+20*4,	100,20,  ENM_CTRL_NORMAL, 4,	0, STR_BLANK ),             
	};

	MigControl *apCtrl = NULL;
	for ( INT8U i = 0; i < 4; i++ )
	{
		aCS[i].hParent = this;
		apCtrl = AddControl( aCS[i] );
	}

	/// Create cutoff list updown box
	U16  aListItem[] = 
	{
		STR_LAST_CUTOFF,
	};

	mpCutoffCtrl = new MigUpDn( aCS[4] );
	AddControl( mpCutoffCtrl );
	mpCutoffCtrl->SetItems( aListItem, sizeof(aListItem) / sizeof(aListItem[0]) );

	tagEditBoxCS  aECS;
	aECS.fmaxVal =  8.0;
	aECS.fminVal = -8.0;
	aECS.fVal    = 1.000;
	aECS.maxChar = 3;
	aECS.type = ENM_EB_TYPE_FLOAT;
	aECS.titleWidth = 1;

	/// Create PC, NC editbox
	mpCtrlPc = new MigEditBox( aCS[5], aECS );
	mpCtrlNc = new MigEditBox( aCS[6], aECS );
	AddControl( mpCtrlPc );
	AddControl( mpCtrlNc );

}

void CEnmCutoffCompute::FocusOn()
{
	CEnmScanContext& rScanContex = GetScanContext();
	CEnmProcessedData	&rData = rScanContex.mCalcData;

	if ( rData.Hasconc() )
	{
		CEnmConcDataUtil::CalcPcNc( rScanContex );
	}
	
	mpCtrlNc->SetFP( rData.fNc );
	mpCtrlPc->SetFP( rData.fPc );

	/// Fill History Cutoff Data List
	FillHistoryCutoff();

	inherited::FocusOn();
}

void CEnmCutoffCompute::OnKeyOK()
{
	if ( !GetCurCtrl()->IsInEdit() )
	{
		if ( GetCurCtrl() == mpCtrlPc )
		{
			GetScanContext().mCalcData.fPc = mpCtrlPc->GetFP();
		}
		else if ( GetCurCtrl() == mpCtrlNc )
		{
			GetScanContext().mCalcData.fNc = mpCtrlNc->GetFP();
		}
	}

	inherited::OnKeyOK();
}

void CEnmCutoffCompute::OnKeyUp()
{
	if ( GetCurCtrl() == mpCutoffCtrl )
	{
		SyncPNc2UI();
	}

	inherited::OnKeyUp();
}

void CEnmCutoffCompute::OnKeyDown()
{
	if ( GetCurCtrl() == mpCutoffCtrl )
	{
		SyncPNc2UI();
	}

	inherited::OnKeyDown();
}

void CEnmCutoffCompute::SyncPNc2UI()
{
	U16 aCurSel = mpCutoffCtrl->GetCurSel();
	if ( aCurSel == 0 )
	{
		CEnmScanContext& rScanContex = GetScanContext();
		CEnmProcessedData	&rData = rScanContex.mCalcData;

		if ( rData.Hasconc() )
		{
			CEnmConcDataUtil::CalcPcNc( rScanContex );
		}
		
		mpCtrlNc->SetFP( rData.fNc );
		mpCtrlPc->SetFP( rData.fPc );
	}
	else
	{
		if ( aCurSel <= mCutoffCount )
		{
			mpCtrlNc->SetFP( mCutoffList[aCurSel-1].mNC );
			mpCtrlPc->SetFP( mCutoffList[aCurSel-1].mPC );			
		}
	}
}

void CEnmCutoffCompute::FillHistoryCutoff()
{
	CEnmScanContext& rScanContex = GetScanContext();
	CEnmProcessedData	&rData = rScanContex.mCalcData;
	
	MCHAR* pProtName = rScanContex.mProtocol.stMeasure.szName;
	CEnmCutoffLib  &rCutoffLib = CEnmCutoffLib::Instance();
	mCutoffCount = rCutoffLib.Find( pProtName, mCutoffList, 32 );

	MCHAR aszCutoffItems[CUTOFF_COUNT][48] = { 0 };
	MCHAR *apItem[] = { aszCutoffItems[0], NULL };

	/// First set current memory cutoff 
	MCHAR* apMemoryCutoff = MStrMngr::Instance().GetString(STR_LAST_CUTOFF);
	memcpy( aszCutoffItems[0], apMemoryCutoff, strlen(apMemoryCutoff) );

	/// Add history cutoff
	for ( U16 i = 0; i < mCutoffCount; i++ )
	{
		FormatCutoffName( mCutoffList[i], aszCutoffItems[i+1] );
		apItem[i+1] = aszCutoffItems[i + 1];
	}

	mpCutoffCtrl->SetItems( apItem, mCutoffCount+1 );	
}

BOOLEAN CEnmCutoffCompute::FormatCutoffName( tagCutoffEntry& rCutoff, MCHAR* pName )
{
	if ( !pName )
		return FALSE;

	tagDate aDate;
	tagTime aTime;

	aDate.year  = (rCutoff.mDate >> 16) & 0xFFFF;
	aDate.month = (rCutoff.mDate >> 8 ) & 0xFF;
	aDate.day   = rCutoff.mDate & 0xFF;

	aTime.hour  = (rCutoff.mTime >> 16) & 0xFF;
	aTime.min   = (rCutoff.mTime >> 8) & 0xFF;
	aTime.sec   = rCutoff.mTime & 0xFF;

	sprintf( pName, "%02d:%02d %02d/%02d/%04d", aTime.hour, aTime.min, aDate.day, aDate.month, aDate.year );
	return TRUE;
}

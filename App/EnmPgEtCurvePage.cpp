// EnmPgEtCurvePage.cpp: implementation of the CEnmPgEtCurvePage class.
//
//////////////////////////////////////////////////////////////////////

#include "EnmPgEtCurvePage.h"
#include "EnmModeLib.h"


CEnmPgEtCurvePage::CEnmPgEtCurvePage()
: CEnmBasePage( "", PID_PG_CURVE_SELECT )
{
	SetTitle( STR_PCURVE_TITLE );
	CreateControl();
}

void CEnmPgEtCurvePage::CreateControl()
{
	INT32U aStyleStatic = ENM_CTRL_NORMAL | ENM_CTRL_STATIC ; 

	U32  aItem[] = 
	{
		STR_PCURVE_CURVE,
		STR_PCURVE_AXISX,
		STR_PCURVE_AXISY,
	};

	U16 aLeft = mLeftMargin + 20;
	tagCS aCS[] = 
	{
		tagCS( ENM_CTRL_LABEL, 	aLeft,	mTopMargin+20*2,	130,	20,  aStyleStatic, 3,	0, aItem[0] ),             
		tagCS( ENM_CTRL_LABEL, 	aLeft,	mTopMargin+20*3,	130,	20,  aStyleStatic, 4,	0, aItem[1] ),             
		tagCS( ENM_CTRL_LABEL, 	aLeft,	mTopMargin+20*4,	130,	20,  aStyleStatic, 5,	0, aItem[2] ),             
	};

	tagCS aUpDnCS[] = 
	{
		tagCS( ENM_CTRL_UPDOWNBOX, 	aLeft+100,	mTopMargin+20*2,	60,20,  ENM_CTRL_NORMAL, 6,0, STR_BLANK ),             
		tagCS( ENM_CTRL_UPDOWNBOX, 	aLeft+100,	mTopMargin+20*3,	60,20,  ENM_CTRL_NORMAL, 7,0, STR_BLANK ),             
		tagCS( ENM_CTRL_UPDOWNBOX, 	aLeft+100,	mTopMargin+20*4,	60,20,  ENM_CTRL_NORMAL, 8,0, STR_BLANK ),             
	};

	MigControl *apCtrl = NULL;
	for ( INT8U i = 0; i < sizeof(aCS) / sizeof(aCS[0]); i++ )
	{
		aCS[i].hParent = this;
		apCtrl = AddControl( aCS[i] );
	}

	/// Create New  UpDn control
	U16 aCurveItem[] = 
	{
		STR_PCURVE_LINEAR,
		STR_PCURVE_SEGMENT,
	};

	mpCurve = new MigUpDn( aUpDnCS[0] );
	AddControl( mpCurve );
	mpCurve->SetItems( aCurveItem, sizeof(aCurveItem) / sizeof(aCurveItem[0]) );
	
	MCHAR *apAxis[] = { "LIN", "LOG" };
	mpAxisX = new MigUpDn( aUpDnCS[1] );
	AddControl( mpAxisX );
	mpAxisX->SetItems( apAxis, sizeof(apAxis) / sizeof(apAxis[0]) );

	mpAxisY = new MigUpDn( aUpDnCS[2] );
	AddControl( mpAxisY );
	mpAxisY->SetItems( apAxis,  sizeof(apAxis) / sizeof(apAxis[0]) );
}

void  CEnmPgEtCurvePage::FocusOn()
{
	SyncProtocol2UI();
	ControlFocusSw( mpCurve );
	
	CEnmBasePage::FocusOn();
}

void CEnmPgEtCurvePage::SyncProtocol2UI()
{
	tagQualititive *apProt = &GetScanContext().mProtocol.stQuality;

	mpCurve->SetCurSel( apProt->uCurve );	
	mpAxisX->SetCurSel( apProt->uAxisX );
	mpAxisY->SetCurSel( apProt->uAxisY );
}

BOOLEAN  CEnmPgEtCurvePage::OnForward()
{	
	/// Save protocol
	tagQualititive *apProt = &GetScanContext().mProtocol.stQuality;

	apProt->uCurve		= mpCurve->GetCurSel();	
	apProt->uAxisX		= mpAxisX->GetCurSel();
	apProt->uAxisY		= mpAxisY->GetCurSel();

	/// Save the protocol
	if ( GetScanContext().GetFlag() & SC_PROT_NEW )
		CEnmModeLib::Instance().Add( GetScanContext().mProtocol );
	else if ( GetScanContext().GetFlag() & SC_PROT_EDIT )
		CEnmModeLib::Instance().UpdateByID( GetScanContext().mProtocol.stMeasure.uID, GetScanContext().mProtocol );
	
	/// return to upper page
	return inherited::OnExit();
}

BOOLEAN  CEnmPgEtCurvePage::OnExit()
{	
	/// Set default protocol to scan context
	CEnmScanContext& rScanContext = GetScanContext();
	rScanContext.Release();
	rScanContext.Reset();
	rScanContext.SetFlag( 0 );

	return inherited::OnExit();
}

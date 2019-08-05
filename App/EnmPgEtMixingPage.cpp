#include "EnmPgEtMixingPage.h"

CEnmPgEtMixingPage::CEnmPgEtMixingPage()
: CEnmBasePage( "", PID_PG_MIXING_MODE )
{
	SetTitle( STR_PMIXING_TITLE );
	CreateControl();
}

void CEnmPgEtMixingPage::CreateControl()
{
	INT32U aStyleStatic = ENM_CTRL_NORMAL | ENM_CTRL_STATIC ; 

	U32  aItem[] = 
	{
		STR_PMIXING_K_FACTOR,
		STR_PWHOLE_MIX_TIME,
		STR_PWHOLE_MIX_MODE,
		STR_PMIXING_TEMPRATURE,
	};

	U16 aLeft = mLeftMargin + 20;
	tagCS aCS[] = 
	{
		tagCS( ENM_CTRL_LABEL, 	aLeft,	mTopMargin+20*0,	80,	20,  aStyleStatic, CTRL_TEXT_K,	0, aItem[0] ),             
		tagCS( ENM_CTRL_LABEL, 	aLeft,	mTopMargin+20*1,	100,	20,  aStyleStatic, CTRL_TEXT_MIXTIME,	0, aItem[1] ),             
		tagCS( ENM_CTRL_LABEL, 	aLeft,	mTopMargin+20*2,	100,	20,  aStyleStatic, CTRL_TEXT_MIXMODE,	0, aItem[2] ),             
		tagCS( ENM_CTRL_LABEL, 	aLeft,	mTopMargin+20*4,	100,	20,  aStyleStatic, CTRL_TEXT_TEMP,	0, aItem[3] ),             
	};

	tagCS  aEditCS[] = 
	{
		tagCS( ENM_CTRL_EDITBOX, 	aLeft+80,	mTopMargin+20*0,	20,20,  ENM_CTRL_NORMAL, 10,	0, STR_BLANK ),             
		tagCS( ENM_CTRL_EDITBOX, 	aLeft+140,	mTopMargin+20*1,	20,20,  ENM_CTRL_NORMAL, 11,	0, STR_BLANK ),             
		tagCS( ENM_CTRL_EDITBOX, 	aLeft+80,	mTopMargin+20*4,	20,20,  ENM_CTRL_NORMAL, 12,	0, STR_BLANK ),             
	};

	tagCS aUpDnCS[] = 
	{
		tagCS( ENM_CTRL_UPDOWNBOX, 	aLeft+140,	mTopMargin+20*2,	20,20,  ENM_CTRL_NORMAL, 13,0, STR_BLANK ),             
	};

	tagCS aCheckCS[] = 
	{
		tagCS( ENM_CTRL_RADIOBOX, 	aLeft,	mTopMargin+20*3,		100,20,  ENM_CTRL_NORMAL, 13,0, STR_PMIXING_TEMPRATURE_CHK ),             
	};
	
	MigControl *apCtrl = NULL;
	for ( INT8U i = 0; i < sizeof(aCS) / sizeof(aCS[0]); i++ )
	{
		aCS[i].hParent = this;
		apCtrl = AddControl( aCS[i] );
	}

	/// Create Edit Box
	tagEditBoxCS  aECS;
	aECS.fmaxVal = 1000;
	aECS.fminVal  = -1000;
	aECS.fVal    = 1;
	aECS.maxChar = 8;
	aECS.type	 = ENM_EB_TYPE_FLOAT;
	aECS.titleWidth = 30;
	aECS.titleId = MIG_STR_ID_INVALID;

	mpKfactorBox = new MigEditBox( aEditCS[0], aECS ) ;	
	AddControl( mpKfactorBox );

	aECS.maxVal  = 25;
	aECS.minVal  = 0;
	aECS.iVal    = 2;
	aECS.maxChar = 8;
	aECS.type	 = ENM_EB_TYPE_INT;
	aECS.titleWidth = 20;
	aECS.titleId = MIG_STR_ID_INVALID;

	mpMixTimeBox = new MigEditBox( aEditCS[1], aECS ) ;	
	AddControl( mpMixTimeBox );
	
	/// Create New  UpDn control
	U16    aMixMode[]  = { 	STR_MIXMODE_SLOW, STR_MIXMODE_MIDDLE, STR_MIXMOE_FAST };
	mpMixMode		= new MigUpDn( aUpDnCS[0] );
	AddControl( mpMixMode );
	mpMixMode->SetItems( aMixMode, 3 );

	/// Temp check
	mpChkTemp = new  MigRadioBox( aCheckCS[0] );
	AddControl( mpChkTemp );
	mpChkTemp->SetState( FALSE );

	/// Temprature check
	aECS.fmaxVal = 100;
	aECS.fminVal = 0;
	aECS.fVal    = 37.0;
	aECS.maxChar = 8;
	aECS.type	 = ENM_EB_TYPE_FLOAT;
	aECS.titleWidth = 30;
	aECS.titleId = MIG_STR_ID_INVALID;
	mpTempratureBox = new MigEditBox( aEditCS[2], aECS ) ;	
	AddControl( mpTempratureBox );
}

void  CEnmPgEtMixingPage::FocusOn()
{
	SyncProtocol2UI();
	ControlFocusSw( mpKfactorBox );

	CEnmBasePage::FocusOn();
}

void  CEnmPgEtMixingPage::OnKeyOK()
{
	if ( mpChkTemp == mpCurCtrl && !mpChkTemp->IsInEdit() )
	{
		SyncTempratureState( mpChkTemp->GetState() );
	}
}

void  CEnmPgEtMixingPage::SyncTempratureState( BOOLEAN bEnable )
{
	mpTempratureBox->Visible( bEnable );
	GetControl( CTRL_TEXT_TEMP )->Visible( bEnable );
	if ( !bEnable )
	{
		tagRect rc;
		rc.left = 0;
		rc.top = mpTempratureBox->mY;
		rc.right = GetFrameW();
		rc.bottom = rc.top + mpTempratureBox->mHeight;
		InvalidateRect( rc );
		Update();
	}
}

void CEnmPgEtMixingPage::SyncProtocol2UI()
{
	tagProtocol *apProt = &GetScanContext().mProtocol;

	mpKfactorBox->SetFP( (U32)(apProt->stMeasure.kFactor * 10000) / 10000.0 );	
	mpMixTimeBox->SetInt( apProt->stMeasure.uMixingTime );
	mpTempratureBox->SetFP( (apProt->stMeasure.fTemprature * 10) / 10.0 );	
	mpChkTemp->SetState( apProt->stMeasure.uTempratureCtrl );	
	mpMixMode->SetCurSel( apProt->stMeasure.uMixingMode );

	SyncTempratureState( apProt->stMeasure.uTempratureCtrl );
}

BOOLEAN  CEnmPgEtMixingPage::OnForward()
{	
	/// Save protocol
	tagProtocol *apProt = &GetScanContext().mProtocol;

	apProt->stMeasure.kFactor			= mpKfactorBox->GetFP();	
	apProt->stMeasure.uMixingTime		= mpMixTimeBox->GetInt();
	apProt->stMeasure.fTemprature		= mpTempratureBox->GetFP();	
	apProt->stMeasure.uTempratureCtrl	= mpChkTemp->GetState() ? 1 : 0;	
	apProt->stMeasure.uMixingMode		= mpMixMode->GetCurSel();

	return inherited::OnForward();
}

BOOLEAN  CEnmPgEtMixingPage::OnExit()
{	
	/// Set default protocol to scan context
	CEnmScanContext& rScanContext = GetScanContext();
	rScanContext.Release();
	rScanContext.Reset();
	rScanContext.SetFlag( 0 );

	return inherited::OnExit();
}

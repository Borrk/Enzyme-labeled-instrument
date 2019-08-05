// EnmPgEtQualitativePage.cpp: implementation of the CEnmPgEtQualitativePage class.
//
//////////////////////////////////////////////////////////////////////
#include "EnmPgEtQualitativePage.h"
#include "MIG_EditBox.Hpp"

CEnmPgEtQualitativePage::CEnmPgEtQualitativePage()
: CEnmBasePage( "", PID_PG_EDIT_QUALITY )
{
	SetTitle( STR_PQUALITATIVE_TITLE );
	CreateControl();
}

void CEnmPgEtQualitativePage::CreateControl()
{
	INT32U aStyleStatic = ENM_CTRL_NORMAL | ENM_CTRL_STATIC ; 

	U32  aItem[] = 
	{
		STR_PQUALITY_BLANKS,		
		STR_PQUALITATIVE_NC,
		STR_PQUALITATIVE_CO,
		STR_PQUALITATIVE_LP,
		STR_PQUALITATIVE_PC,
		STR_PQUALITATIVE_HP,
	};

	U16 aLeft = mLeftMargin + 20;
	tagCS aCS[] = 
	{
		tagCS( ENM_CTRL_LABEL, 	aLeft,	mTopMargin+20*0,	100,	20,  aStyleStatic, 1,	0, aItem[0] ),             
		tagCS( ENM_CTRL_LABEL, 	aLeft,	mTopMargin+20*1,	100,	20,  aStyleStatic, 2,	0, aItem[1] ),             
		tagCS( ENM_CTRL_LABEL, 	aLeft,	mTopMargin+20*2,	130,	20,  aStyleStatic, 3,	0, aItem[2] ),             
		tagCS( ENM_CTRL_LABEL, 	aLeft,	mTopMargin+20*3,	130,	20,  aStyleStatic, 4,	0, aItem[3] ),             
		tagCS( ENM_CTRL_LABEL, 	aLeft,	mTopMargin+20*4,	130,	20,  aStyleStatic, 5,	0, aItem[4] ),             
		tagCS( ENM_CTRL_LABEL, 	aLeft,	mTopMargin+20*5,	130,	20,  aStyleStatic, 5,	0, aItem[5] ),             
	};

	tagCS  aEditCS[] = 
	{
		tagCS( ENM_CTRL_EDITBOX, 	aLeft+150,	mTopMargin+20*0,	20,20,  ENM_CTRL_NORMAL, 12,	0, STR_BLANK ),             
		tagCS( ENM_CTRL_EDITBOX, 	aLeft+150,	mTopMargin+20*1,	20,20,  ENM_CTRL_NORMAL, 10,	0, STR_BLANK ),             
		tagCS( ENM_CTRL_EDITBOX, 	aLeft+150,	mTopMargin+20*2,	20,20,  ENM_CTRL_NORMAL, 11,	0, STR_BLANK ),             
		tagCS( ENM_CTRL_EDITBOX, 	aLeft+150,	mTopMargin+20*3,	20,20,  ENM_CTRL_NORMAL, 12,	0, STR_BLANK ),             
		tagCS( ENM_CTRL_EDITBOX, 	aLeft+150,	mTopMargin+20*4,	20,20,  ENM_CTRL_NORMAL, 12,	0, STR_BLANK ),             
		tagCS( ENM_CTRL_EDITBOX, 	aLeft+150,	mTopMargin+20*5,	20,20,  ENM_CTRL_NORMAL, 12,	0, STR_BLANK ),             
	};

	U8 i;
	MigControl *apCtrl = NULL;
	for ( i = 0; i < sizeof(aCS) / sizeof(aCS[0]); i++ )
	{
		aCS[i].hParent = this;
		apCtrl = AddControl( aCS[i] );
	}

	/// Create Edit Box
	tagEditBoxCS aECS;
	aECS.maxVal  = 8;
	aECS.minVal  = 0;
	aECS.iVal    = 2;
	aECS.maxChar = 1;
	aECS.type	 = ENM_EB_TYPE_INT;
	aECS.titleWidth = 1;
	aECS.titleId = MIG_STR_ID_INVALID;

	MigEditBox  **apEditBox[] = 
	{
		&mpBlank,
		&mpNC,	
		&mpCO,	
		&mpLP,	
		&mpPC,	
		&mpHP,	
	};
	
	for ( i = 0; i < sizeof(apEditBox) / sizeof(apEditBox[0]); i++ )
	{
		*apEditBox[i] = new MigEditBox( aEditCS[i], aECS ) ;
		AddControl( *apEditBox[i] );
	}
}

void  CEnmPgEtQualitativePage::FocusOn()
{
	SyncProtocol2UI();
	ControlFocusSw( mpBlank );
	
	inherited::FocusOn();
}

void CEnmPgEtQualitativePage::SyncProtocol2UI()
{
	tagQualitative *apProt = &GetScanContext().mProtocol.stQualitative;

	mpBlank->SetInt( apProt->uBlank );
	mpNC->SetInt( apProt->uNC );	
	mpCO->SetInt( apProt->uCO );	
	mpLP->SetInt( apProt->uLP );	
	mpPC->SetInt( apProt->uPC );	
	mpHP->SetInt( apProt->uHP );	
}

BOOLEAN  CEnmPgEtQualitativePage::OnForward()
{	
	/// Save protocol
	tagQualitative *apProt = &GetScanContext().mProtocol.stQualitative;

	apProt->uBlank  = mpBlank->GetInt( );
	apProt->uNC		= mpNC->GetInt( );	
	apProt->uCO		= mpCO->GetInt( );	
	apProt->uLP		= mpLP->GetInt( );	
	apProt->uPC		= mpPC->GetInt( );	
	apProt->uHP		= mpHP->GetInt( );	

	return inherited::OnForward();
}

BOOLEAN  CEnmPgEtQualitativePage::OnExit()
{	
	/// Set default protocol to scan context
	CEnmScanContext& rScanContext = GetScanContext();
	rScanContext.Release();
	rScanContext.Reset();
	rScanContext.SetFlag( 0 );

	return inherited::OnExit();
}


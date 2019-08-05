// EnmPgEtCutoffPage.cpp: implementation of the CEnmPgEtCutoffPage class.
//
//////////////////////////////////////////////////////////////////////
#include "EnmPgEtCutoffPage.h"
#include "EnmModeLib.h"


CEnmPgEtCutoffPage::CEnmPgEtCutoffPage()
: CEnmBasePage( "", PIG_PG_EDIT_CUTOFF )
{
	SetTitle( STR_PCUTOFF_TITLE );
	CreateControl();
}

void CEnmPgEtCutoffPage::CreateControl()
{
	INT32U aStyleStatic = ENM_CTRL_NORMAL | ENM_CTRL_STATIC ; 

	U32  aItem[] = 
	{		
		STR_PCUTOFF_CUTOFF,
		STR_PCUTOFF_P_GRAY,	
		STR_PCUTOFF_N_GRAY,	
		STR_PCUTOFF_FORMAT_PLUS,	
		STR_PCUTOFF_FORMAT_SUB,	
		STR_PCUTOFF_FORMAT_SCUTOFF,	
	};

	U16 aLeft = mLeftMargin + 20;
	tagCS aCS[] = 
	{
		tagCS( ENM_CTRL_LABEL, 	aLeft,	mTopMargin+20*0,		60,	20,  aStyleStatic, 1,	0, aItem[0] ),             
		tagCS( ENM_CTRL_LABEL, 	aLeft,	mTopMargin+20*1,		60,	20,  aStyleStatic, 2,	0, aItem[1] ),             
		tagCS( ENM_CTRL_LABEL, 	aLeft,	mTopMargin+20*2,		60,	20,  aStyleStatic, 3,	0, aItem[2] ),             
		tagCS( ENM_CTRL_RADIOBOX, aLeft,	mTopMargin+20*4,	30,	20,  ENM_CTRL_NORMAL, 4,0,	aItem[3] ),             
		tagCS( ENM_CTRL_RADIOBOX, aLeft+80, mTopMargin+20*4,	30,	20,  ENM_CTRL_NORMAL, 5,0,	aItem[4] ),             
		tagCS( ENM_CTRL_RADIOBOX, aLeft+160,mTopMargin+20*4,	80,20,  ENM_CTRL_NORMAL, 6,0,	aItem[5] ),             
	};

	tagCS  aEditCS[] = 
	{
		tagCS( ENM_CTRL_EDITBOX, 	aLeft+60,	mTopMargin+20*0,	120,20,  ENM_CTRL_NORMAL, 10,	0, STR_BLANK ),             
		tagCS( ENM_CTRL_EDITBOX, 	aLeft+90,	mTopMargin+20*1,	120,20,  ENM_CTRL_NORMAL, 11,	0, STR_BLANK ),             
		tagCS( ENM_CTRL_EDITBOX, 	aLeft+90,	mTopMargin+20*2,	120,20,  ENM_CTRL_NORMAL, 12,	0, STR_BLANK ),             
	};

	MigControl *apCtrl = NULL;
	for ( INT8U i = 0; i < 3; i++ )
	{
		aCS[i].hParent = this;
		apCtrl = AddControl( aCS[i] );
	}

	/// Create Edit Box
	tagEditBoxCS aECS;
	memset( &aECS, 0, sizeof(aECS) );
	aECS.maxChar = 40;
	aECS.type = ENM_EB_TYPE_CHAR;
	aECS.titleWidth = 40; 
	aECS.titleId = MIG_STR_ID_INVALID;

	mpCutoff = new MigEditBox( aEditCS[0], aECS ) ;	
	AddControl( mpCutoff );
	mpCutoff->SetStr( "PC*2.1" );

	aECS.fmaxVal = 10000;
	aECS.fminVal = 0;
	aECS.fVal    = 30;
	aECS.maxChar = 5;
	aECS.type = ENM_EB_TYPE_FLOAT;
	aECS.titleId = MIG_STR_ID_INVALID;

	mpGrayP = new MigEditBox( aEditCS[1], aECS ) ;	
	AddControl( mpGrayP );
	mpGrayP->SetRangeFP( 0, 10000, 1 );

	aECS.fmaxVal = 10000;
	aECS.fminVal = 0;
	aECS.fVal    = 30;
	aECS.maxChar = 5;
	aECS.type = ENM_EB_TYPE_FLOAT;
	aECS.titleId = MIG_STR_ID_INVALID;

	mpGrayN = new MigEditBox( aEditCS[2], aECS ) ;	
	AddControl( mpGrayN );
	mpGrayN->SetRangeFP( 0, 10000, 1 );

	mpFormat1 = new MigRadioBox( aCS[3] );
	mpFormat2 = new MigRadioBox( aCS[4] );
	mpFormat3 = new MigRadioBox( aCS[5] );

	AddControl( mpFormat1 );
	AddControl( mpFormat2 );
	AddControl( mpFormat3 );

}

void  CEnmPgEtCutoffPage::FocusOn()
{
	SyncProtocol2UI();
	ControlFocusSw( mpCutoff );
	
	CEnmBasePage::FocusOn();
}

void CEnmPgEtCutoffPage::SyncProtocol2UI()
{
	tagQualitative *apProt = &GetScanContext().mProtocol.stQualitative;

	mpCutoff->SetStr( apProt->szCutoff );
	mpGrayP->SetFP( apProt->fGrayP * 100 );	
	mpGrayN->SetFP( apProt->fGrayN * 100 );	

	SelectFormat( apProt->uPrintFormat, TRUE );
}

BOOLEAN  CEnmPgEtCutoffPage::OnForward()
{	
	/// Save protocol
	tagQualitative *apProt = &GetScanContext().mProtocol.stQualitative;

	MCHAR *aszCutoff = mpCutoff->GetStr( );
	strcpy ( apProt->szCutoff, aszCutoff );

	apProt->fGrayP = mpGrayP->GetFP() / 100.0;	
	apProt->fGrayN = mpGrayN->GetFP() / 100.0;	
	
	/// Save the protocol
	if ( GetScanContext().GetFlag() & SC_PROT_NEW )
		CEnmModeLib::Instance().Add( GetScanContext().mProtocol );
	else if ( GetScanContext().GetFlag() & SC_PROT_EDIT )
		CEnmModeLib::Instance().UpdateByID( GetScanContext().mProtocol.stMeasure.uID, GetScanContext().mProtocol );

	/// return to upper page
	return inherited::OnExit();
}

BOOLEAN  CEnmPgEtCutoffPage::OnExit()
{	
	/// Set default protocol to scan context
	CEnmScanContext& rScanContext = GetScanContext();
	rScanContext.Release();
	rScanContext.Reset();
	rScanContext.SetFlag( 0 );

	return inherited::OnExit();
}

void	CEnmPgEtCutoffPage::OnKeyOK()
{
	inherited::OnKeyOK();

	if ( GetCurCtrl() == mpFormat1 )
	{
		SelectFormat( 0, mpFormat1->GetState() );
		GetScanContext().mProtocol.stQualitative.uPrintFormat = ENM_CUTOFF_PRINT_ASEND;
	}
	else if ( GetCurCtrl() == mpFormat2 )
	{
		SelectFormat( 1, mpFormat2->GetState() );
		GetScanContext().mProtocol.stQualitative.uPrintFormat = ENM_CUTOFF_PRINT_DEASEND;
	}
	else if ( GetCurCtrl() == mpFormat3 )
	{
		SelectFormat( 2, mpFormat3->GetState() );
		GetScanContext().mProtocol.stQualitative.uPrintFormat = ENM_CUTOFF_PRINT_CUTOFF;
	}
	
}

void	CEnmPgEtCutoffPage::SelectFormat( INT8U type, BOOLEAN bSel )
{
	INT8S aType;

	if ( type == 0 )
	{
		if ( bSel )
		{
			mpFormat1->SetState( TRUE );
			mpFormat2->SetState( FALSE );
			mpFormat3->SetState( FALSE );
		
			aType = ENM_CUTOFF_PRINT_ASEND;
		}
		else
		{
		}
	}
	else if ( type == 1 )
	{
		if ( bSel )
		{
			mpFormat1->SetState( FALSE );
			mpFormat2->SetState( TRUE );
			mpFormat3->SetState( FALSE );
			
			aType = ENM_CUTOFF_PRINT_DEASEND;
		}
		else
		{
		}
	}
	else
	{
		if ( bSel )
		{
			mpFormat1->SetState( FALSE );
			mpFormat2->SetState( FALSE );
			mpFormat3->SetState( TRUE );
			
			aType = ENM_CUTOFF_PRINT_CUTOFF;
		}
		else
		{
		}
	}

}

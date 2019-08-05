// EnmPgEtModePage.cpp: implementation of the CEnmPgEtModePage class.
//
//////////////////////////////////////////////////////////////////////
#include "EnmPgEtModePage.h"
#include "EnmWfStepHub.h"
#include "EnmSysCfgLib.h"
#include "EnmModeLib.h"

CEnmPgEtModePage::CEnmPgEtModePage( )
: CEnmBasePage( "", PID_PG_EDIT_MODE )
, mCurMode( MODE_EDIT )
{
	SetEditMode( MODE_NEW );
	CreateControl();
}

void CEnmPgEtModePage::CreateControl()
{
	INT32U aStyleStatic = ENM_CTRL_NORMAL | ENM_CTRL_STATIC ; 

	U32  aItem[] = 
	{
		STR_PPROGRAM_DEL_PID,
		STR_PPROGRAM_EDIT_NAME,
		STR_PPROGRAM_EDIT_MAIN_FILTER,
		STR_PPROGRAM_EDIT_SLAVE_FILTER,
		STR_PPROGRAM_EDIT_REF_FILTER,
		STR_PPROGRAM_EDIT_MODE,
	};

	U16 aLeft = mLeftMargin + 20;
	tagCS aCS[] = 
	{
		tagCS( ENM_CTRL_LABEL, 	aLeft,	mTopMargin+20*0,	150,	20,  aStyleStatic, 1,	0, aItem[0] ),             
		tagCS( ENM_CTRL_LABEL, 	aLeft,	mTopMargin+20*1,	100,	20,  aStyleStatic, 2,	0, aItem[1] ),             
		tagCS( ENM_CTRL_LABEL, 	aLeft,	mTopMargin+20*2,	130,	20,  aStyleStatic, 3,	0, aItem[2] ),             
		tagCS( ENM_CTRL_LABEL, 	aLeft,	mTopMargin+20*3,	130,	20,  aStyleStatic, 4,	0, aItem[3] ),             
		tagCS( ENM_CTRL_LABEL, 	aLeft,	mTopMargin+20*4,	130,	20,  aStyleStatic, 5,	0, aItem[4] ),             
		tagCS( ENM_CTRL_LABEL, 	aLeft,	mTopMargin+20*5,	130,	20,  aStyleStatic, 6,	0, aItem[5] ),             
	};

	tagCS  aEditCS[] = 
	{
		tagCS( ENM_CTRL_EDITBOX, 	aLeft+125,	mTopMargin+20*1,	20,20,  ENM_CTRL_NORMAL, 10,	0, STR_BLANK ),             
	};

	tagCS aUpDnCS[] = 
	{
		tagCS( ENM_CTRL_UPDOWNBOX, 	aLeft+165,	mTopMargin+20*0,	100,20,  ENM_CTRL_NORMAL, 13,0, STR_BLANK ),             
		tagCS( ENM_CTRL_UPDOWNBOX, 	aLeft+165,	mTopMargin+20*2,	100,20,  ENM_CTRL_NORMAL, 14,0, STR_BLANK ),             
		tagCS( ENM_CTRL_UPDOWNBOX, 	aLeft+165,	mTopMargin+20*3,	100,20,  ENM_CTRL_NORMAL, 15,0, STR_BLANK ),             
		tagCS( ENM_CTRL_UPDOWNBOX, 	aLeft+165,	mTopMargin+20*4,	100,20,  ENM_CTRL_NORMAL, 16,0, STR_BLANK ),             
		tagCS( ENM_CTRL_UPDOWNBOX, 	aLeft+165,	mTopMargin+20*5,	100,20,  ENM_CTRL_NORMAL, 17,0, STR_BLANK ),             
	};

    INT8U i;
	MigControl *apCtrl = NULL;
	for ( i = 0; i < sizeof(aCS) / sizeof(aCS[0]); i++ )
	{
		aCS[i].hParent = this;
		apCtrl = AddControl( aCS[i] );
	}

	/// Program ID
	mpProgramID		= new MigUpDn( aUpDnCS[0] );
	AddControl( mpProgramID );
	
	/// Create Edit Box
	tagEditBoxCS aECS;
	aECS.maxChar  = 8;
	aECS.szVal[0] = '\0';
	aECS.type	  = ENM_EB_TYPE_CHAR;
	aECS.titleWidth = 40;//strlen(apItem[2]) * GetFontW();
	aECS.titleId = MIG_STR_ID_INVALID;

	mpNameBox = new MigEditBox( aEditCS[0], aECS ) ;	
	AddControl( mpNameBox );
	
	/// Create New  UpDn control
	U16    aMesaMode[] = { STR_QUALITITIVE, STR_QUALITATIVE };
	
	MCHAR* aszFilter[8] = { 0 };
	for ( i = 0; i < 8; i++ )
	{
		aszFilter[i] = (MCHAR*)CEnmSysCfgLib::Instance().GetSysCfg().szFilter[i];
	}
	
	mpMainWl		= new MigUpDn( aUpDnCS[1] );
	AddControl( mpMainWl );
	mpMainWl->SetItems( aszFilter, 8 );
	
	mpRefWl			= new MigUpDn( aUpDnCS[2] );
	AddControl( mpRefWl );
	mpRefWl->SetItems( aszFilter, 8 );
	
	mpSlaveWl		= new MigUpDn( aUpDnCS[3] );
	AddControl( mpSlaveWl );
	mpSlaveWl->SetItems( aszFilter, 8 );
	
	mpMeasMode		= new MigUpDn( aUpDnCS[4] );
	AddControl( mpMeasMode );
	mpMeasMode->SetItems( aMesaMode, 2 );
	
}

void CEnmPgEtModePage::SetEditMode( enPgEditMode mode )
{
	if ( mCurMode == mode )
		return;

	mCurMode = mode;

	if ( mCurMode == MODE_NEW )
	{
		SetTitle( STR_PPROGRAM_NEW_TITLE );
	}
	else
	{
		SetTitle( STR_PPROGRAM_EDIT_TITLE );
	}
}

BOOLEAN  CEnmPgEtModePage::OnForward()
{
	BOOLEAN bRet = FALSE;
	
	/// Save protocol
	tagProtocol *apProt = &GetScanContext().mProtocol;
	if ( strcmp( mpNameBox->GetStr(), apProt->stMeasure.szName ) != 0 )
	{
		strcpy( apProt->stMeasure.szName, mpNameBox->GetStr() );
	}

	apProt->stMeasure.uMeasMode		= mpMeasMode->GetCurSel();
	apProt->stMeasure.uMainFilter	= mpMainWl->GetCurSel();
	apProt->stMeasure.uReferFilter	= mpRefWl->GetCurSel();
	apProt->stMeasure.uSlaveFilter	= mpSlaveWl->GetCurSel();

	/// Set next step path
	U16 aCurSel = mpMeasMode->GetCurSel();
	CEnmWfStepHub *apStep = (CEnmWfStepHub*)GetStep();
	apStep->Switch( aCurSel );

	return inherited::OnForward();
}

BOOLEAN  CEnmPgEtModePage::OnExit()
{	
	/// Set default protocol to scan context
	CEnmScanContext& rScanContext = GetScanContext();
	rScanContext.Release();
	rScanContext.Reset();
	rScanContext.SetFlag( 0 );

	return inherited::OnExit();
}

void CEnmPgEtModePage::FocusOn(void)
{
	mpMeasMode->SetCurSel( 0 );
	CEnmWfStepHub *apStep = (CEnmWfStepHub*)GetStep();
	apStep->Switch( 0 );

	/// Set default protocol to scan context
	CEnmScanContext& rScanContext = GetScanContext();
	rScanContext.Release();
	rScanContext.Reset();
	rScanContext.SetFlag( (GetEditMode() == MODE_NEW) ? SC_PROT_NEW : SC_PROT_EDIT );
	
	InitUI();
	
	inherited::FocusOn();
}

void CEnmPgEtModePage::InitUI()
{
	mpNameBox->SetStr( " " );	
	
	/// Create New  UpDn control
	if ( MODE_NEW == mCurMode )
	{
		MCHAR aID[8] = { 0 };
		MCHAR *apID = (MCHAR*)aID;
		sprintf( aID, "%d", CEnmModeLib::Instance().GetCount() + 1 );
		mpProgramID->SetItems( (MCHAR**)&apID, 1 );
		mpProgramID->SetCurSel( 0 );
		GetScanContext().mProtocol.stMeasure.uID = CEnmModeLib::Instance().GetCount();
		ControlFocusSw( mpNameBox );
	}
	else
	{
		MCHAR aID[200][6] = { 0 };
		MCHAR *apID[200] = { NULL };
		for ( U16 i = 0; i < CEnmModeLib::Instance().GetCount(); i++ )
		{
			sprintf( aID[i], "%d", i + 1 );
			apID[i] = aID[i];
		}

		mpProgramID->SetItems( (MCHAR**)&apID, CEnmModeLib::Instance().GetCount() );
		mpProgramID->SetCurSel( 0 );
	
		tagProtocol aProt;
		if ( CEnmModeLib::Instance().GetAt( 0, aProt) )
		{
			GetScanContext().mProtocol = aProt;
		}
	
		ControlFocusSw( mpProgramID );
	}

	SyncProtocol2UI();
}

void CEnmPgEtModePage::OnKeyOK(void)
{	
	if ( (GetCurCtrl() == mpNameBox) && (!mpNameBox->IsInEdit()) )
	{
		strcpy( GetScanContext().mProtocol.stMeasure.szName, mpNameBox->GetStr() );
	}

	inherited::OnKeyOK();
}

void CEnmPgEtModePage::OnKeyUp(void)
{	
	inherited::OnKeyUp();

	if ( MODE_EDIT != mCurMode )
		return;
	
	if ( mpProgramID == GetCurCtrl() )
	{
		tagProtocol aProt;
		U16 aCurSel = mpProgramID->GetCurSel();
		if ( CEnmModeLib::Instance().GetAt( aCurSel, aProt) )
		{
			GetScanContext().mProtocol = aProt;
			SyncProtocol2UI();
		}
	}
	
}

void CEnmPgEtModePage::OnKeyDown(void)
{	
	inherited::OnKeyDown();

	if ( MODE_EDIT != mCurMode )
		return;
	
	if ( mpProgramID == GetCurCtrl() )
	{
		tagProtocol aProt;
		U16 aCurSel = mpProgramID->GetCurSel();
		if ( CEnmModeLib::Instance().GetAt( aCurSel, aProt) )
		{
			GetScanContext().mProtocol = aProt;
			SyncProtocol2UI();
		}
	}
}

void CEnmPgEtModePage::SyncProtocol2UI()
{
	tagProtocol *apProt = &GetScanContext().mProtocol;

	mpNameBox->SetStr( apProt->stMeasure.szName );	
	mpMainWl->SetCurSel( apProt->stMeasure.uMainFilter );	
	mpRefWl->SetCurSel( apProt->stMeasure.uReferFilter );
	mpSlaveWl->SetCurSel( apProt->stMeasure.uSlaveFilter );	
	mpMeasMode->SetCurSel( apProt->stMeasure.uMeasMode );
}

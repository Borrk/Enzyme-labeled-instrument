#include  "EnmModeBasePage.hpp"
#include  "EnmWfStepHub.h"
#include  "EnmApp.hpp"
#include  "EnmModeLib.h"
#include  "EnmSysCfgLib.h"

CEnmModeBasePage::CEnmModeBasePage(  MCHAR* pTitle, U32 id )
: CEnmBasePage( pTitle, id )
, mApp( CEnmApp::Instance() )
{
	Init();
}

CEnmModeBasePage::~CEnmModeBasePage()
{
}

void   CEnmModeBasePage::Init()
{
	mLineStep   = 20;
	mTopMargin  = 20;

// 	mbShowFuncKey = TRUE;
	ShowFuncKey( TRUE );
	SetFuncKey( ENM_FUNCKEY_PCOPY, TRUE, TRUE, STR_STEP_HCOPY );

	CreateControl();
}

void   CEnmModeBasePage::CreateControl()
{
	U16 	aItem[] = 
	{
		STR_STEP_GO,
		STR_STEP_EXIT,
		STR_STEP_HCOPY 
	};

	INT32U aStyleStatic = ENM_CTRL_NORMAL | ENM_CTRL_STATIC ; 

//	tagCS aCS[] = 
//	{
//		tagCS( ENM_CTRL_LABEL, 		5,		40+20*8,	90,	20,  aStyleStatic, MODE_CTRL_GO,	0, aItem[0] ),             
//		tagCS( ENM_CTRL_LABEL, 		5+100,	40+20*8,	90,	20,  aStyleStatic, MODE_CTRL_STOP,	0, aItem[1] ),             
//		tagCS( ENM_CTRL_LABEL, 		10+200,	40+20*8,	60,		20,  aStyleStatic, MODE_CTRL_HCOPY,	0, aItem[2] ),             
//	};
//
//	MigControl *apCtrl = NULL;
//	for ( INT8U i = 0; i < sizeof(aCS)/sizeof(aCS[0]); i++ )
//	{
//		aCS[i].hParent = this;
//		apCtrl = AddControl( aCS[i] );
//	}
//
//	ShowFuncKey( TRUE );

	tagCS aTestCS( ENM_CTRL_LABEL, mLeftMargin-8, mTopMargin, 50, 20, aStyleStatic, 1000, 0, STR_PPROGRAM_TEST );
	aTestCS.hParent = this;
	AddControl( aTestCS );
	
	tagCS aUpCS( ENM_CTRL_UPDOWNBOX, mLeftMargin+50, mTopMargin, 30, 20, ENM_CTRL_NORMAL, 1001, 0, -1 );
	mpProtoNo = new MigUpDn( aUpCS );
	AddControl( mpProtoNo );
	
	
	tagCS aNameLabelCS( ENM_CTRL_LABEL, mLeftMargin+ 100, mTopMargin, 40, 20, aStyleStatic, 1002, 0, STR_PPROGRAM_NAME );
	aNameLabelCS.hParent = this;
	AddControl( aNameLabelCS );
	
	tagCS aNameCS( ENM_CTRL_LABEL, mLeftMargin+ 140, mTopMargin, 150, 20, aStyleStatic, 10032,"123 ", -1 );
	aNameCS.hParent = this;
	mpProtoName = 	AddControl( aNameCS );
}

//void  CEnmModeBasePage::ShowFuncKey( BOOLEAN bShow )
//{
//	mbShowFuncKey = bShow;
//
//	MigControl *apCtrl;
//	if ( IsShowFuncKey() )
//	{
//		GetControl( MODE_CTRL_GO )->Visible( TRUE );
//		GetControl( MODE_CTRL_STOP )->Visible( TRUE );
//		GetControl( MODE_CTRL_HCOPY )->Visible( TRUE );
//	}
//	else
//	{
//		GetControl( MODE_CTRL_GO )->Visible( FALSE );
//		GetControl( MODE_CTRL_STOP )->Visible( FALSE );
//		GetControl( MODE_CTRL_HCOPY )->Visible( FALSE );
//	}
//}

void    CEnmModeBasePage::FocusOn()
{
	CEnmBasePage::FocusOn();
	
	if ( GetScanContext().GetRefProtID() == -1 )
	{
		SetScanProtocol( 0 ); 
	}
	
	FocusControl( mpProtoNo->GetID() );
	FillProtoNo();

	SyncWfs();
}

void       CEnmModeBasePage::SyncWfs()
{
	CEnmScanContext& rProt = GetScanContext();

	IWorkStep *apStep = GetStep();
	if ( !apStep )
		return;
	
	U16 aNewPath = rProt.mProtocol.stMeasure.uMeasMode == MEASURE_QUALITATIVE ? 0 : 1;
	apStep->Switch( aNewPath );
}

BOOLEAN    CEnmModeBasePage::SetScanProtocol( U16 idx )
{
	CEnmScanContext& rProt = GetScanContext();
	CEnmModeLib& rML = CEnmModeLib::Instance();

	if ( idx >= rML.GetCount() )
		return FALSE;

	tagProtocol aProt;
	if ( !rML.GetAt( idx, aProt ) )
		return FALSE;

	rProt.mProtocol = aProt;
	rProt.SetRefProtID( idx );
	SyncUI();
}

void    CEnmModeBasePage::SyncUI()
{	
	CEnmScanContext &aScanProt = GetScanContext();
	if ( aScanProt.mProtocol.stMeasure.uID == PROT_ID_INVALID )
		return;

	MCHAR *apName = aScanProt.mProtocol.stMeasure.szName;
	if ( apName )
		mpProtoName->SetTitle( apName );
	else
		mpProtoName->SetTitle( "  "  );		
}

void	CEnmModeBasePage::Draw(void)
{

	if ( mStyle & ENM_OBJ_HIDEN )
		return;

	DrawFrame( );

	if ( mStyle & ENM_OBJ_TITLE )
	{
		DrawTitle( );
	}

	/// draw Mode info
	DrawModeInfo();

	// draw control
	DrawControls();

	DrawTips();

	DrawFuncKey();
}

void CEnmModeBasePage::DrawModeInfo()
{
	if (GetScanContext().mProtocol.stMeasure.uID == PROT_ID_INVALID )
		return;

	MCHAR aszModeInfo[256] = { 0 };
	U16 aCnt = FormatModeInfo( aszModeInfo );

	MCHAR *ap = aszModeInfo;
	for ( U16 i = 0; i < aCnt; i++ )
	{
		if ( ap )
		{
			PutStr( mX + mLeftMargin, mY + mTopMargin + 4 + (i+1) * mLineStep, ap, 0 );
		}

		ap += strlen( ap ) + 1;
	}
}

U16 CEnmModeBasePage::FormatModeInfo( MCHAR* pszInfo )
{
	U16 aCnt = 0;
	U16 aId;
	MCHAR *apCur, *p1, *p2, *p3, *p4;

	apCur = pszInfo;

	tagProtocol* apProt = &mApp.GetScanContext().mProtocol;
	MStrMngr& rMngr = MStrMngr::Instance();

	/// format 1st line
	// no need
	//p2 = rMngr.GetString( STR_PPROGRAM_NAME );
	//p3 = apProt->stMeasure.szName; // get mode's name
	//aId = apProt->stMeasure.uID; /// get id
	//sprintf( apCur, "%20c %s %s", ' ',  p2, p3 );
	//apCur += strlen( apCur ) + 1;
	//aCnt++; 

	/// format 2nd line
	p1 = rMngr.GetString( STR_PPROGRAM_TYPE );
	p3 = rMngr.GetString( STR_PPROGRAM_CUTOFF );
	p2 = nGetMeasureModeStr( apProt->stMeasure.uMeasMode ); // get program's mode
	p4 = "08 Mar 2006"; /// get cut off
	sprintf( apCur, "%s %s%2c%s %s", p1, p2, ' ', p3, p4 );
	apCur += strlen( apCur ) + 1;
	aCnt++; 

	/// format 3rd line	
	if ( apProt->stMeasure.uMeasMode == MEASURE_QUALITATIVE )
	{
		U8	blk = apProt->stQualitative.uBlank;
		U8  nc  = apProt->stQualitative.uNC;
		U8  co  = apProt->stQualitative.uCO;
		U8  lp  = apProt->stQualitative.uLP;
		U8  pc  = apProt->stQualitative.uPC;
		U8  hp  = apProt->stQualitative.uHP;
		sprintf( apCur, "BLK*%d, NC*%d, CO*%d, LP*%d, PC*%d, HP*%d", blk, nc, co, lp, pc, hp );
	}
	else
	{
		U8 blk   = apProt->stQuality.uBlank;
		U8	stds = apProt->stQuality.uStd;
		sprintf( apCur, "BLK*%d, Std*%d", blk, stds );
	}

	apCur += strlen( apCur ) + 1;
	aCnt++; 

	/// format 4th line
	p1 = rMngr.GetString( STR_PPROGRAM_FILTER );
	p2 = rMngr.GetString( STR_PPROGRAM_MAINFILTER );
	p3 = rMngr.GetString( STR_PPROGRAM_REFERFILTER );
	p4 = rMngr.GetString( STR_PPROGRAM_SLAVEFILTER );

	MCHAR *pFilterMain, *pFilterSlave, *pFilterRef;
	tagSystemCfg&  rSysCfg = CEnmSysCfgLib::Instance().GetSysCfg();
	pFilterMain  = rSysCfg.szFilter[apProt->stMeasure.uMainFilter];
	pFilterSlave = rSysCfg.szFilter[apProt->stMeasure.uSlaveFilter];
	pFilterRef   = rSysCfg.szFilter[apProt->stMeasure.uReferFilter];

	sprintf( apCur, "%s %s%s %s%s %s%s", p1, p2, pFilterMain, p3, pFilterSlave, p4, pFilterRef );
	apCur += strlen( apCur ) + 1;
	aCnt++; 

	return aCnt;
}

MCHAR*	CEnmModeBasePage::nGetMeasureModeStr( U8 mode )
{
	if ( MEASURE_QUALITITIVE == mode )
	{
		return MStrMngr::Instance().GetString( STR_QUALITITIVE );
	}
	else if ( MEASURE_QUALITATIVE == mode )
	{
		return MStrMngr::Instance().GetString( STR_QUALITATIVE );
	}
	else
	{
		return NULL;
	}
}

void CEnmModeBasePage::	SaveScanProtocol()
{
	CEnmScanContext &aScanProt = GetScanContext();
	if ( aScanProt.mProtocol.stMeasure.uID == PROT_ID_INVALID )
		return;

	CEnmModeLib::Instance().SetAt( aScanProt.GetRefProtID(), aScanProt.mProtocol );
}

void CEnmModeBasePage::	FillProtoNo()
{
	CEnmModeLib& rML = CEnmModeLib::Instance();

	BOOLEAN bValid = FALSE;
	CEnmScanContext &aScanProt = GetScanContext();
	if ( aScanProt.mProtocol.stMeasure.uID == PROT_ID_INVALID )
	{
		tagProtocol aProt;
		if ( rML.GetAt( 0, aProt ) )
		{
			aScanProt.mProtocol = aProt;
			aScanProt.SetRefProtID( 0 );
			bValid = TRUE;
		}
	}
	else
		bValid = TRUE;
	
	if ( !bValid )
	{
		mpProtoNo->SetItems( (MCHAR**)0, 0 );
		return;
	}

	MCHAR aProtID[256][6] = {0};
	MCHAR *appID[256] = {0 };

	for ( U16 i = 0; i < rML.GetCount(); i++ )
	{
		sprintf( aProtID[i], "%d", i+1 );
		appID[i] = aProtID[i];
	}
	mpProtoNo->SetItems( (MCHAR**)appID, rML.GetCount() );	
}

BOOLEAN	 CEnmModeBasePage::IsCanActive() const
{
	return CEnmModeLib::Instance().GetCount() > 0 ? TRUE : FALSE;
}

BOOLEAN	 CEnmModeBasePage::IsCanForward()
{
	return CEnmModeLib::Instance().GetCount() > 0 ? TRUE : FALSE;
}

void	CEnmModeBasePage::OnKeyUp( )
{
	CEnmBasePage::OnKeyUp();

	if ( GetCurCtrl() == mpProtoNo && mpProtoNo->IsInEdit() )
	{
		int aSel = mpProtoNo->GetCurSel();
		if ( aSel != GetScanContext().GetRefProtID() )
		{
			UpdateModeInfoRect( 1, 2 );

			SaveScanProtocol();
			SetScanProtocol( aSel );

			SyncWfs();
			SyncUI();
		}
	}
}

void CEnmModeBasePage::OnKeyDown()
{
	CEnmBasePage::OnKeyDown();

	if ( GetCurCtrl() == mpProtoNo && mpProtoNo->IsInEdit() )
	{
		int aSel = mpProtoNo->GetCurSel();
		if ( aSel != GetScanContext().GetRefProtID() )
		{
			UpdateModeInfoRect( 1, 2 );

			SaveScanProtocol();
			SetScanProtocol( aSel );

			SyncWfs();
			SyncUI();
		}
	}
}

void CEnmModeBasePage::UpdateModeInfoRect( U8 from, U8 to )
{	
	tagRect rc;
	rc.left = mX + mLeftMargin;
	rc.top  = mY + mTopMargin + (from+1) * mLineStep;;
	rc.right = mWidth;
	rc.bottom = mY + mTopMargin + (to+1) * mLineStep;

	InvalidateRect( rc, 0 );
}

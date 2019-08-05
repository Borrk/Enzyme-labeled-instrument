// EnmDataCompute.cpp: implementation of the CEnmDataCompute class.
//
//////////////////////////////////////////////////////////////////////

#include "EnmControlConcPage.h"
#include "EnmConcDataUtil.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnmControlConcPage::CEnmControlConcPage()
:CEnmBasePage( "", ENM_PAGE_ID_CONTROLCONC )
{
	SetTitle( STR_PPROCESS_EDIT );
	CreateControls();
}

CEnmControlConcPage::~CEnmControlConcPage()
{
}

void CEnmControlConcPage::CreateControls( void)
{
	U16 aLeft = mLeftMargin + 20;

	tagCS aCS[] = 
	{
		tagCS( ENM_CTRL_GRIDBOX, aLeft,	mTopMargin+20*0,	140,	20,  ENM_CTRL_NORMAL ,    1,	"", STR_BLANK ),             
		tagCS( ENM_CTRL_GRIDBOX, aLeft + 145,	mTopMargin+20*0, 140,	20,  ENM_CTRL_NORMAL, 2,	"", STR_BLANK ),             
	};

	
	INT16U type[3] = { ENM_EB_TYPE_CHAR, ENM_EB_TYPE_CHAR, ENM_EB_TYPE_FLOAT };
	INT16U width[3] = { 40, 40, 70 };
	MCHAR* p[] = {	"CTRL",  " Well", " OD." };

	tagGridBoxCS gcs;
	gcs.columnDisp	= 3;
	gcs.columnMax	= 3;
	gcs.lineDisp	= 8;
	gcs.lineMax		= 8;
	gcs.pColumnWidth = width;
	gcs.itemHeight = 20;
	gcs.titleHeight = 20;
	gcs.pItemType = type;
	gcs.pszItemTitle = p;//NULL;

	aCS[0].hParent = this;
	mpCtrlPc = new MigGrid( aCS[0],  gcs );
	AddControl( mpCtrlPc );

	U16 i, j;
	for ( i = 0; i < gcs.lineDisp; i++ )
	{
		mpCtrlPc->SetItemRangeCHAR( i, 0, 2 );
		mpCtrlPc->SetItemRangeCHAR( i, 1, 2 );
		mpCtrlPc->SetItemRangeFP( i, 2, -8.0, 8.0 , 3 );
	}

	aCS[1].hParent = this;
	mpCtrlNc = new MigGrid( aCS[1],  gcs );
	AddControl( mpCtrlNc );

	for ( i = 0; i < gcs.lineDisp; i++ )
	{
		mpCtrlNc->SetItemRangeCHAR( i, 0, 2 );
		mpCtrlNc->SetItemRangeCHAR( i, 1, 2 );
		mpCtrlNc->SetItemRangeFP( i, 2, -8.0, 8.0 , 3 );
	}
}

void CEnmControlConcPage::FocusOn()
{
	UpdateControlConc( TRUE );
	inherited::FocusOn();
}

void CEnmControlConcPage::FocusOff()
{
	UpdateControlConc( FALSE );
	inherited::FocusOff();
}

void CEnmControlConcPage::OnKeyOK()
{
	if ( GetCurCtrl() == mpCtrlPc )
	{
		if ( !mpCtrlPc->IsInEdit() )
		{
			Sync2DataConc();
		}
	}
	else if ( GetCurCtrl() == mpCtrlNc )
	{
		if ( !mpCtrlNc->IsInEdit() )
		{
			Sync2DataConc();
		}
	}

	inherited::OnKeyOK();
}

BOOLEAN	CEnmControlConcPage::UpdateControlConc( BOOLEAN bToUI )
{
	BOOLEAN bOk;

	if ( bToUI )
		bOk = Sync2UIConc();
	else
		bOk = Sync2DataConc();

	return bOk;
}

BOOLEAN	CEnmControlConcPage::Sync2DataConc()
{
	CEnmScanContext& rScanContex = GetScanContext();
	tagScanData	*apData = rScanContex.GetScanData();

	if ( !apData )
		return FALSE;
	
	if ( rScanContex.mProtocol.stMeasure.uMeasMode != MEASURE_QUALITATIVE )
		return FALSE;

	U16 i;
	FP64 aConc, aMin, aMax;
	FP64 aConc2;

	S16 aPcStartPos = rScanContex.GetConcStartPos( CONC_PC );
	U16 aPcCnt =  rScanContex.GetConcCount( CONC_PC );

	FP64 *apConc = (FP64*)rScanContex.mCalcData.fConc;	
	for ( i = 0; i < aPcCnt; i++ )
	{
		//if ( CEnmConcDataUtil::PickConcData( *apData, aPcStartPos + i, aConc, aMin, aMax ) )
		if ( CEnmConcDataUtil::PickConcData( apConc, aPcStartPos + i, aConc ) )
		if ( mpCtrlPc->GetItemVal( i, 2, &aConc2 ) )
		{
			if ( aConc2 != aConc )
			{
				//CEnmConcDataUtil::PutConcData( *apData, aPcStartPos + i, aConc2, aMin, aMax );
				CEnmConcDataUtil::PutConcData( apConc, aPcStartPos + i, aConc2 );
			}
		}
	}

	S16 aNcStartPos = rScanContex.GetConcStartPos( CONC_NC );
	U16 aNcCnt =  rScanContex.GetConcCount( CONC_NC );

	for ( i = 0; i < aNcCnt; i++ )
	{
		//if ( CEnmConcDataUtil::PickConcData( *apData, aNcStartPos + i, aConc, aMin, aMax ) )
		if ( CEnmConcDataUtil::PickConcData( apConc, aNcStartPos + i, aConc ) )
		if ( mpCtrlNc->GetItemVal( i, 2, &aConc2 ) )
		{
			if ( aConc2 != aConc )
			{
				CEnmConcDataUtil::PutConcData( apConc, aNcStartPos + i, aConc2 );
			}
		}
	}

	return TRUE;
}

BOOLEAN	CEnmControlConcPage::Sync2UIConc( )
{
	CEnmScanContext& rScanContex = GetScanContext();
	tagScanData	*apData = rScanContex.GetScanData();

	if ( !apData )
		return FALSE;
	
	if ( rScanContex.mProtocol.stMeasure.uMeasMode != MEASURE_QUALITATIVE )
		return FALSE;

	FP64 aConc, aMin, aMax;

	/// update PC conc
	mpCtrlPc->DeleteAllLine();

	U16 i;
	U16 aLn = 0;
	MCHAR *aszPc = "Pc";
	MCHAR  aszPos[8];

	S16 aPcStartPos = rScanContex.GetConcStartPos( CONC_PC );
	U16 aPcCnt =  rScanContex.GetConcCount( CONC_PC );

	FP64 *apConc = (FP64*)rScanContex.mCalcData.fConc;
	for ( i = 0; i < aPcCnt; i++ )
	{
		//if ( CEnmConcDataUtil::PickConcData( *apData, aPcStartPos + i, aConc, aMin, aMax ) )
		if ( CEnmConcDataUtil::PickConcData( apConc, aPcStartPos + i, aConc ) )
		{
			aLn = mpCtrlPc->AddLine();
			
			/// Set Conc Data
			sprintf( aszPos, "%c%1d", 'A' + GETPLATEROW(aPcStartPos + i), GETPLATECOLUMN(aPcStartPos + i)+1 );

			mpCtrlPc->SetItemVal( aLn, 0, aszPc );
			mpCtrlPc->SetItemVal( aLn, 1, aszPos );
			mpCtrlPc->SetItemVal( aLn, 2, &aConc );
		}
	}

	MCHAR *aszNc = "Nc";
	S16 aNcStartPos = rScanContex.GetConcStartPos( CONC_NC );
	U16 aNcCnt =  rScanContex.GetConcCount( CONC_NC );
	
	mpCtrlNc->DeleteAllLine();
	for ( i = 0; i < aNcCnt; i++ )
	{
		//if ( CEnmConcDataUtil::PickConcData( *apData, aNcStartPos + i, aConc, aMin, aMax ) )
		if ( CEnmConcDataUtil::PickConcData( apConc, aNcStartPos + i, aConc ) )
		{
			aLn = mpCtrlNc->AddLine();

			sprintf( aszPos, "%c%1d", 'A' + GETPLATEROW(aNcStartPos + i), GETPLATECOLUMN(aNcStartPos + i)+1 );

			mpCtrlNc->SetItemVal( aLn, 0, aszNc );
			mpCtrlNc->SetItemVal( aLn, 1, aszPos );
			mpCtrlNc->SetItemVal( aLn, 2, &aConc );
		}
	}

	mpCtrlPc->ItemFocusOn( 0, 2 );
	mpCtrlPc->EnableColumn( 0,  FALSE );
	mpCtrlPc->EnableColumn( 1,  FALSE );

	mpCtrlNc->EnableColumn( 0,  FALSE );
	mpCtrlNc->EnableColumn( 1,  FALSE );
	
	return TRUE;
}

void	CEnmControlConcPage::OnKeyLeft( void )
{
	MigGrid *apCtrl;

	if ( mpCurCtrl && mpCurCtrl->Previous() )
	{
		apCtrl = (MigGrid*)mpCurCtrl->Previous();
		apCtrl->ItemFocusOn( 0, 2 );
		ControlFocusSw( apCtrl, FALSE );
	}
}

void	CEnmControlConcPage::OnKeyRight( void )
{
	MigGrid *apCtrl;

	if ( mpCurCtrl && mpCurCtrl->Next() )
	{
		apCtrl = (MigGrid*)mpCurCtrl->Next();
		apCtrl->ItemFocusOn( 0, 2 );
		ControlFocusSw( apCtrl, TRUE );
	}
}

void	CEnmControlConcPage::OnKeyUp( void )
{
	MigGrid *apCtrl = (MigGrid*)mpCurCtrl;
	if ( !apCtrl->OverTop() )
		return;

	if ( mpCurCtrl && mpCurCtrl->Previous() )
	{
		apCtrl = (MigGrid*)mpCurCtrl->Previous();
		apCtrl->ItemFocusOn( 0, 2 );

		ControlFocusSw( apCtrl, FALSE );
	}
}
	
void	CEnmControlConcPage::OnKeyDown( void )
{
	MigGrid *apCtrl = (MigGrid*)mpCurCtrl;
	if ( !apCtrl->OverButtom() )
		return;

	if ( mpCurCtrl && mpCurCtrl->Next() )
	{
		apCtrl = (MigGrid*)mpCurCtrl->Next();
		apCtrl->ItemFocusOn( 0, 2 );

		ControlFocusSw( apCtrl, TRUE );
	}
}

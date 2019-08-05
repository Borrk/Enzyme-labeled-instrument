#include "EnmPgEtQualityPage.h"

CEnmPgEtQualityPage::CEnmPgEtQualityPage()
: CEnmBasePage( "", PID_PG_EDIT_QUALITY )
{
	SetTitle( STR_PQUALITY_TITLE );
	CreateControl();
}

void CEnmPgEtQualityPage::CreateControl()
{
	INT32U aStyleStatic = ENM_CTRL_NORMAL | ENM_CTRL_STATIC ; 

	U32  aItem[] = 
	{
		STR_PQUALITY_BLANKS,
		STR_PQUALITY_STD_COUNT,
		STR_PQUALITY_STD_REPT,
		STR_PQUALITY_SAMPLE_REPT,
		STR_PQUALITY_UNIT,
	};

	U16 aLeft = mLeftMargin + 20;
	U16 aTop  = mTopMargin - 10;

	tagCS aCS[] = 
	{
		tagCS( ENM_CTRL_LABEL, 	aLeft,	aTop+20*0,	40,	20,  aStyleStatic, 1,	0, aItem[0] ),             
		tagCS( ENM_CTRL_LABEL, 	aLeft,	aTop+20*1,	100,	20,  aStyleStatic, 2,	0, aItem[1] ),             
		tagCS( ENM_CTRL_LABEL, 	aLeft,	aTop+20*2,	130,	20,  aStyleStatic, 3,	0, aItem[2] ),             
		tagCS( ENM_CTRL_LABEL, 	aLeft,	aTop+20*3,	130,	20,  aStyleStatic, 4,	0, aItem[3] ),             
#if STD_SHOW_POS
		tagCS( ENM_CTRL_LABEL, 	aLeft+190,	aTop+20*4 + 5, 40,	20,  aStyleStatic, 5,	0, aItem[4] ),             
#else
		tagCS( ENM_CTRL_LABEL, 	aLeft+170,	aTop+20*4 + 5, 40,	20,  aStyleStatic, 5,	0, aItem[4] ),             
#endif
	};

	tagCS  aEditCS[] = 
	{
		tagCS( ENM_CTRL_EDITBOX, 	aLeft+195,	aTop+20*0,	20,20,  ENM_CTRL_NORMAL, 10,	0, STR_BLANK ),             
		tagCS( ENM_CTRL_EDITBOX, 	aLeft+190,	aTop+20*1,	20,20,  ENM_CTRL_NORMAL, 11,	0, STR_BLANK ),             
		tagCS( ENM_CTRL_EDITBOX, 	aLeft+190,	aTop+20*2,	20,20,  ENM_CTRL_NORMAL, 12,	0, STR_BLANK ),             
		tagCS( ENM_CTRL_EDITBOX, 	aLeft+190,	aTop+20*3,	20,20,  ENM_CTRL_NORMAL, 12,	0, STR_BLANK ),             
	};

	tagCS aUpDnCS[] = 
	{
#if STD_SHOW_POS
		tagCS( ENM_CTRL_UPDOWNBOX, 	aLeft+238,	aTop+20*4+5,	50,20,  ENM_CTRL_NORMAL, 13,0, STR_BLANK ),             
#else
		tagCS( ENM_CTRL_UPDOWNBOX, 	aLeft+224,	aTop+20*4+5,	50,20,  ENM_CTRL_NORMAL, 13,0, STR_BLANK ),             
#endif
	};

	MigControl *apCtrl = NULL;
	for ( INT8U i = 0; i < sizeof(aCS) / sizeof(aCS[0]); i++ )
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
	aECS.titleWidth = 1;//strlen(apItem[2]) * GetFontW();
	aECS.titleId = MIG_STR_ID_INVALID;

	mpBlankBox = new MigEditBox( aEditCS[0], aECS ) ;	
	AddControl( mpBlankBox );

	aECS.maxVal  = 8;
	aECS.minVal  = 0;
	aECS.iVal    = 2;
	aECS.maxChar = 1;
	aECS.type	 = ENM_EB_TYPE_INT;
	aECS.titleWidth = 1;//strlen(apItem[2]) * GetFontW();
	aECS.titleId = MIG_STR_ID_INVALID;

	mpStdBox = new MigEditBox( aEditCS[1], aECS ) ;	
	AddControl( mpStdBox );

	aECS.maxVal  = 8;
	aECS.minVal  = 0;
	aECS.iVal    = 2;
	aECS.maxChar = 1;
	aECS.type	 = ENM_EB_TYPE_INT;
	aECS.titleWidth = 1;//strlen(apItem[2]) * GetFontW();
	aECS.titleId = MIG_STR_ID_INVALID;

	mpStdReptBox = new MigEditBox( aEditCS[2], aECS ) ;	
	AddControl( mpStdReptBox );
	
	aECS.maxVal  = 8;
	aECS.minVal  = 0;
	aECS.iVal    = 2;
	aECS.maxChar = 1;
	aECS.type	 = ENM_EB_TYPE_INT;
	aECS.titleWidth = 1;//strlen(apItem[2]) * GetFontW();
	aECS.titleId = MIG_STR_ID_INVALID;

	mpSampleReptBox = new MigEditBox( aEditCS[3], aECS ) ;	
	AddControl( mpSampleReptBox );

	/// Create New  UpDn control
	MCHAR* aszFilter[] = { "None", "g/l", "mg/l" };
	mpUnit = new MigUpDn( aUpDnCS[0] );
	AddControl( mpUnit );
	mpUnit->SetItems( aszFilter, 3 );

	CreateStdListControl();

}

void CEnmPgEtQualityPage::CreateStdListControl()
{
	tagCS aCS[] = 
	{
		tagCS( ENM_CTRL_GRIDBOX, mLeftMargin + 10,	mTopMargin+20*4 - 5, 240, 20, ENM_CTRL_NORMAL, 10,	"" ),             
	};

	U16 aTitleId[] = 
	{
		STR_PQUALITY_STD_NO,
		STR_PQUALITY_STD_POS,
		STR_PQUALITY_STD_CONC
	};

#if  STD_SHOW_POS
	INT16U width[] =  { 80, 50, 230 };
	INT16U type[] = { ENM_EB_TYPE_INT, ENM_EB_TYPE_CHAR, ENM_EB_TYPE_FLOAT};
//	MCHAR *title[] = { ENM_T(ENM_CHINESE,"标准品序号", " No."), ENM_T(ENM_CHINESE,"位置", "Pos"), ENM_T(ENM_CHINESE,"浓度", "Density") };
	MCHAR *title[] = {  " No.", "Pos", "Density" };

	tagGridBoxCS gcs;
	gcs.columnDisp	= 3;
	gcs.columnMax	= 3;
	gcs.lineDisp	= 4;
	gcs.lineMax		= 8;
	gcs.pColumnWidth = width;
	gcs.itemHeight = 20;
	gcs.titleHeight = 20;
	gcs.pItemType = type;
	gcs.pszItemTitle = title;

	aCS[0].hParent = this;
	mStdList = new CEnmSampleList( aCS[0],  gcs ) ;
	AddControl( mStdList );
	mStdList->ItemFocusOffAll();
	
	for ( int i = 0; i < gcs.lineDisp; i++ )
	{
		mStdList->SetItemRangeInt( i, 0, 1, 8 );
		mStdList->SetItemRangeCHAR( i, 1, 3 );
		mStdList->SetItemRangeFP( i, 2, 0.0, 40000.0 , 2 );
		mStdList->SetItemTitle( i, aTitleId[i] );
	}
	
	mStdList->EnableColumn( 1, FALSE );
#else
	INT16U width[] =  { 80, 160 };
	INT16U type[] = { ENM_EB_TYPE_INT, ENM_EB_TYPE_FLOAT };
	MCHAR *title[] = { ENM_T(ENM_CHINESE,"标准品序号", " No."), ENM_T(ENM_CHINESE,"浓度", "Density") };
	//INT16U title[] = { STR_PQUALITY_STD_NO, STR_PQUALITY_STD_CONC };

	tagGridBoxCS gcs;
	gcs.columnDisp	= 2;
	gcs.columnMax	= 2;
	gcs.lineDisp	= 4;
	gcs.lineMax		= 8;
	gcs.pColumnWidth = width;
	gcs.itemHeight = 20;
	gcs.titleHeight = 20;
	gcs.pItemType = type;
	gcs.pszItemTitle = title;


	aCS[0].hParent = this;
	mStdList = new CEnmSampleList( aCS[0],  gcs ) ;
	AddControl( mStdList );
	mStdList->ItemFocusOffAll();

	mStdList->SetItemTitle( (INT16U)0, STR_PQUALITY_STD_NO );
	mStdList->SetItemTitle( 1, STR_PQUALITY_STD_CONC );
	
	for ( int i = 0; i < gcs.lineDisp; i++ )
	{
		mStdList->SetItemRangeInt( i, 0, 1, 8 );
		mStdList->SetItemRangeFP( i, 1, 0.0, 40000.0 , 2 );
		mStdList->SetItemTitle( i, aTitleId[i] );
	}	

#endif

}

void  CEnmPgEtQualityPage::OnKeyOK()
{
	inherited::OnKeyOK();

	U16 aCurRow = mStdList->GetCurLine();

	BOOLEAN bSetStd = FALSE;
	if ( mpStdBox == GetCurCtrl() )
	{
		if ( !mpStdBox->IsInEdit() &&  mpStdBox->GetInt() != GetScanContext().mProtocol.stQuality.uStd )
		{
			GetScanContext().mProtocol.stQuality.uStd  = mpStdBox->GetInt();
			bSetStd = TRUE;
		}
	}
	else if ( mpStdReptBox == GetCurCtrl() )
	{
		if ( !mpStdReptBox->IsInEdit() &&  mpStdReptBox->GetInt() != GetScanContext().mProtocol.stQuality.uStdRepeats )
		{
			GetScanContext().mProtocol.stQuality.uStdRepeats  = mpStdReptBox->GetInt();
			bSetStd = TRUE;
		}
	}
	else if ( mpBlankBox == GetCurCtrl() )
	{
		if ( !mpBlankBox->IsInEdit() &&  mpBlankBox->GetInt() != GetScanContext().mProtocol.stQuality.uBlank )
		{
			GetScanContext().mProtocol.stQuality.uBlank  = mpBlankBox->GetInt();
			bSetStd = TRUE;
		}
	}
	else if ( mStdList == GetCurCtrl() )
	{
		if ( !mStdList->IsInEdit() )
		{
			tagSampleData aStdData;
			U16 aLn = mStdList->GetCurLine();
			U16 aTrueLn = mStdList->GetCurLineBox();
			mStdList->GetOneSample( aTrueLn, aStdData );
			GetScanContext().mProtocol.stQuality.stStdDensity[aLn] = aStdData;
		}
	}

	if ( bSetStd )
	{
		SetStdList();
		//mStdList->ItemFocusOn( aCurRow, 2 );
	}
}

void  CEnmPgEtQualityPage::OnKeyUp()
{
	if ( mStdList == GetCurCtrl() && !mStdList->OverTop() )
	{
		mStdList->EraseColumn( 2 );
		return;
	}

	inherited::OnKeyUp();
}

void  CEnmPgEtQualityPage::OnKeyDown()
{
	if ( mStdList == GetCurCtrl() && !mStdList->OverButtom() )
	{
		mStdList->EraseColumn( 2 );
		return;
	}

	inherited::OnKeyDown();
}

void  CEnmPgEtQualityPage::FocusOn()
{
	mStdList->SetRefProtocol ( &GetScanContext().mProtocol );
	SyncProtocol2UI();
	ControlFocusSw( mpBlankBox );
	
	CEnmBasePage::FocusOn();
}

void CEnmPgEtQualityPage::SyncProtocol2UI()
{
	tagQualititive *apProt = &GetScanContext().mProtocol.stQuality;

	mpBlankBox->SetInt( apProt->uBlank );
	mpSampleReptBox->SetInt( apProt->uSample );
	mpStdReptBox->SetInt( apProt->uStdRepeats );
	mpStdBox->SetInt( apProt->uStd );
	mpUnit->SetCurSel( apProt->uUnit );

	/// Fill std data
	SetStdList();
}

void CEnmPgEtQualityPage::SetStdList()
{
	mStdList->DeleteAllSample();

	CEnmScanContext& aContext = GetScanContext();

	U16 aStdRepeat = aContext.mProtocol.stQuality.uStdRepeats;
	U16 aBlankCnt = aContext.mProtocol.stQuality.uBlank;
	U16 aCol = aBlankCnt / ENM_DATA_COUNT;
	U16 aRow = aBlankCnt % ENM_DATA_COUNT;

	tagSampleData aStd;
	for ( int i = 0; i < aContext.mProtocol.stQuality.uStd; i++ )
	{
		aStd = aContext.mProtocol.stQuality.stStdDensity[i];	
		aStd.xPos = aCol;
		aStd.yPos = aRow;

		//mStdList->AddSample( aStd );
		mStdList->AddSample( i );

		aRow += aStdRepeat;
		if ( aRow > ENM_DATA_COUNT - 1 )
		{
			aCol++;
			aRow = aRow - ENM_DATA_COUNT;
		}
	}

	mStdList->ItemFocusOffAll();
}

BOOLEAN  CEnmPgEtQualityPage::OnForward()
{	
	/// Save protocol
	tagQualititive *apProt = &GetScanContext().mProtocol.stQuality;

	apProt->uBlank = mpBlankBox->GetInt( );
	apProt->uSample = mpSampleReptBox->GetInt( );
	apProt->uStdRepeats  = mpStdReptBox->GetInt( );
	apProt->uStd = mpStdBox->GetInt( );
	apProt->uUnit = mpUnit->GetCurSel( );

	/// Save Stds
//	tagSampleData aData;
//	for ( U16 i = 0; i < apProt->uStd; i++ )
//	{
//		if ( mStdList->GetOneSample( i, aData ) )
//			//apProt->fStdConc[i] = aData.density;
//			apProt->stStdDensity[i] = aData;
//	}

	return inherited::OnForward();
}

BOOLEAN  CEnmPgEtQualityPage::OnExit()
{	
	/// Set default protocol to scan context
	CEnmScanContext& rScanContext = GetScanContext();
	rScanContext.Release();
	rScanContext.Reset();
	rScanContext.SetFlag( 0 );

	return inherited::OnExit();
}


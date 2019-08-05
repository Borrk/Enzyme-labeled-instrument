// EnmConcListPage.cpp: implementation of the CEnmConcListPage class.
//
//////////////////////////////////////////////////////////////////////

#include "EnmConcListPage.h"
#include "MIG_Base.hpp"
#include "EnmConcDataUtil.h"
#include  "EnmApp.hpp"
#include  "EnmModeLib.h"

CEnmConcList::CEnmConcList( MigObj *pParent )
: MigGrid( pParent )
{ 
	SetList(); 

	mLeftPos = mRightPos = 1;
}

CEnmConcList::CEnmConcList( const tagCS& cs,  const tagGridBoxCS& bcs ) 
 : MigGrid( cs, bcs )
{ 
	SetList(); 

	mLeftPos = mRightPos = 1;
}

void	CEnmConcList::SetList( void )
{
//	mCurColumn = 1;
//	EnableColumn( 0, FALSE );
//	EnableMultiKeyPress( 1, FALSE );
	Layout();
}

void	CEnmConcList::Layout( void)	///< set layout by current line and comumn
{
	INT16U i;
	INT16U	aLines = min( mGCS.lineDisp, mUsedLine - mUpperLine );
	
	for ( i = 0; i < aLines; i++ )
	{
		VisibleLine( i );
	}

//	for ( i = aLines; i < mGCS.lineDisp; i++ )
//	{
//		//ClearOneSample( i );
//		VisibleLine( i, 0 );
//	}
}

void  CEnmConcList::OnKeyLeft()
{	
	if ( mCurColumn == 1 )
	{
		mLeftPos++;
	}
	else
		mLeftPos = 1;

	mRightPos = 1;
	
	inherited::OnKeyLeft();
}

void  CEnmConcList::OnKeyRight()
{
	if ( mCurColumn == 6 )
		mRightPos++;
	else
		mRightPos = 1;

	mLeftPos = 1;

	inherited::OnKeyRight();
}

U16  CEnmConcList::GetConc( void* pData, U16 count )
{
	FP64 *ap = (FP64*)pData;
	FP64 aData = 0;
	U16 aCount = 0;
	
	if ( !ap )
		return 0;

	for ( U8 i = 0; i < 6; i++ )
	{
		for ( U8 j = 0; j < 8; j++ )
		{
			if ( GetItemVal( j, i+1, &aData ) )
				ap[aCount] = aData;
			else
				return aCount; ///ap[aCount] = 0;

			if ( ++aCount >= count )
				return aCount;
		}
	}

	return aCount;
}

void  CEnmConcList::SetConc( void* pData, INT32U count )
{
	MCHAR aLn[2] = {0};
	FP64 *ap = (FP64*)pData;

	if ( NULL == ap )
		return;

    U8 i;
	for ( i = 0; i < 8; i++ )
	{
		aLn[0] = 'A' + i;
		SetItemVal( i, 0, aLn );
	}

	for ( i = 0; i < 6; i++ )
	{
		for ( U8 j = 0; j < 8; j++ )
		{
			SetItemVal( j, i+1, &ap[i*8+j] );
		}
	}

	Draw();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnmConcListPage::CEnmConcListPage()
: CEnmBasePage( "", 1 )
, mCurPage( 0 )
, mbEditable( TRUE )

{
	CreateControls();
}

CEnmConcListPage::~CEnmConcListPage()
{

}

void CEnmConcListPage::CreateControls()
{
	tagCS aCS( ENM_CTRL_GRIDBOX, 10, 30, 280, 20, ENM_CTRL_NORMAL, 1,	"", STR_BLANK );
	aCS.hParent = this;

	INT16U type[13];
	INT16U width[13];

	type[0] = ENM_EB_TYPE_CHAR;
	width[0] = 20;

	U8 i, j;
	for ( i = 1; i < 13; i++ )
	{
		type[i] = ENM_EB_TYPE_FLOAT;
		width[i] = 48;
	}

	MCHAR* p[] = 
	{
		//" ",  "  1","  2","  3","  4","  5","  6","  7","  8","  9"," 10"," 11"," 12",
		" ",  "1","2","3","4","5","6","7","8","9","10","11","12",
	};

	tagGridBoxCS gcs;
	gcs.columnDisp	= 7;
	gcs.columnMax	= 13;
	gcs.lineDisp	= 8;
	gcs.lineMax		= 8;
	gcs.pColumnWidth = width;
	gcs.itemHeight = 20;
	gcs.titleHeight = 20;
	gcs.pItemType = type;
	gcs.pszItemTitle = p;//NULL;

	mpConcList = new CEnmConcList( aCS,  gcs );
	AddControl( mpConcList );

	for ( i = 0; i < gcs.lineDisp; i++ )
	{
		mpConcList->SetItemRangeCHAR( i, 0, 1 );
		for ( j = 1; j < gcs.columnDisp; j++ )
			mpConcList->SetItemRangeFP( i, j, -8.0, 8.0 , 3 );
	}

	for ( j = 0; j < 8; j++ )
	{
		mpConcList->AddLine();
	}

	mpConcList->EnableColumn( 0, FALSE );
}

void    CEnmConcListPage::FocusOn()
{
	CEnmBasePage::FocusOn();

	BOOLEAN bValid = FALSE;
	CEnmScanContext &aScanProt = GetScanContext();
	
	mCurPage = 0;
	mpConcList->ItemFocusOn( 0, 1 );

	INT32U aCount = 0;
	if ( aScanProt.HasScanData() )
	{
		mpConcList->Visible( TRUE );
		tagScanData* apData = aScanProt.GetScanData();
		if ( apData )
		{
			bValid = TRUE;
			aCount = aScanProt.GetSampleCount( TRUE );
			if ( !aScanProt.IsConcCalculated() )
				CalculateConc( );
			
			//mpConcList->SetConc( apData->dbData, aCount );
		}
	}

	if ( !bValid )
	{
		aCount = 0;
	}

	SetPageData( mCurPage );
	//VisibleListFrom( aCount, FALSE );
}

void   CEnmConcListPage::VisibleListFrom( U16 pos, BOOLEAN bVisible )
{
	U16 aCol = GETPLATECOLUMN( pos );
	U16 aRow = GETPLATEROW( pos );

	/// visible all, 
	U16 i;
	for ( i = 1; i < ENM_DATA_GROUP/2 + 1; i++ )
		mpConcList->VisibleColumn( i );

	if ( aCol ==0 && aRow == 0 )
	{
		for ( i = 1; i < ENM_DATA_GROUP/2 + 1; i++ )
			mpConcList->VisibleColumn( bVisible );
	}	
	else if ( aCol < ENM_DATA_GROUP )
	{
		aCol++; ///< because the first column is hint one

		for ( i = aRow; i < ENM_DATA_COUNT; i++ )
			mpConcList->VisibleItem( i, aCol, bVisible );

		if ( aRow > 0 )
			aCol++;
		for ( i = aCol; i < ENM_DATA_GROUP/2 + 1; i++ )
		{
			mpConcList->VisibleColumn( i, bVisible );
		}
	}
}

void CEnmConcListPage::SetPageData( S8 page )
{
	CEnmScanContext &aScanProt = GetScanContext();
	
	FP64 fConc, fBase, fBlank;

	U16 aFocusLn = mpConcList->GetCurLine();
	U16 aStart;
	U16 aCount = 0;
	if ( !aScanProt.HasScanData() )
	{
		mpConcList->SetConc( NULL, 0 );
		VisibleListFrom( 0, FALSE );
		return;
	}
	
	FP64	aConcData[ENM_DATA_GROUP * ENM_DATA_COUNT] = { 0 };
	FP64 *apConc = (FP64*)aScanProt.mCalcData.GetConc( TRUE );
	if ( apConc )
	{
		aStart = page * ENM_DATA_GROUP * ENM_DATA_COUNT / 2;
		aCount = aScanProt.GetSampleCount( TRUE ) - aStart;
		for ( U16 i = aStart; i < (aStart + aCount); i++ )
		{
			if ( CEnmConcDataUtil::PickConcData( apConc, i, fConc ) )
			{
				aConcData[i-aStart] = fConc;
			}
		}

		if ( aCount > 0 )
		{
			mpConcList->SetConc( aConcData, aCount );
		}
		else
		{
			mpConcList->SetConc( NULL, 0 );
		}

		VisibleListFrom( 0, TRUE );
		VisibleListFrom( aCount, FALSE );

		U16 aCurLn = mpConcList->GetCurLine();
		if ( mCurPage <= page )
			mpConcList->ItemFocusOn( aCurLn, 1 );
		else
			mpConcList->ItemFocusOn( aCurLn, 6 );
	}

	mCurPage = page;
}

U16 CEnmConcListPage::CalculateConc()
{
	CEnmScanContext &aScanProt = GetScanContext();
	
	FP64 fConc, fBase, fBlank;

	U16 aCount = 0;
	if ( aScanProt.HasScanData() )
	{
		FP64 *apConc = (FP64*)aScanProt.mCalcData.fConc;
		tagScanData* apData = aScanProt.GetScanData();
		if ( apData )
		{
			aCount = aScanProt.GetSampleCount( TRUE );
			for ( U16 i = 0; i < aCount; i++ )
			{
				if ( CEnmConcDataUtil::PickConcData( *apData, i, fConc, fBase, fBlank ) )
				{
					fConc = CEnmConcDataUtil::CalcConc( fConc, fBase, fBlank, 3 );
					//CEnmConcDataUtil::PutConcData( *apData, i, fConc, fBase, fBlank );
					CEnmConcDataUtil::PutConcData( apConc, i, fConc );
				}
				else
				{
					//CEnmConcDataUtil::PutConcData( *apData, i, 0, fBase, fBlank );
					CEnmConcDataUtil::PutConcData( apConc, i, 0 );
				}
			}

			aScanProt.mCalcData.SetConcFlag( TRUE );
		}
	}

	return aCount;
}

void	CEnmConcListPage::OnKeyOK()
{
	inherited::OnKeyOK();

	if ( mpConcList->IsInEdit() )
		return;

	CEnmScanContext& aScanProt = GetScanContext();
	FP64 *apConc = (FP64*)aScanProt.mCalcData.GetConc( FALSE );
	if ( !apConc )
		return;

	FP64 aData[ENM_DATA_COUNT * ENM_DATA_GROUP] = { 0 };
	U16 aCount = mpConcList->GetConc( aData, sizeof(aData) / sizeof(aData[0]) );

	FP64 fConc;
	U16 aStart = mCurPage * ENM_DATA_GROUP * ENM_DATA_COUNT / 2;
	for ( U16 i = aStart; i < (aStart + aCount); i++ )
	{
		if ( CEnmConcDataUtil::PickConcData( apConc, i, fConc ) )
		{
			if ( fConc != aData[i - aStart] )
				CEnmConcDataUtil::PutConcData( apConc, i, aData[i - aStart] );
		}
	}
}

void	CEnmConcListPage::OnKeyLeft()
{
	inherited::OnKeyLeft();
	
	if ( mCurPage > 0 )
	{
		if ( mpConcList->OverLeft() )
		{
			MCHAR* p[] = {"1","2","3","4","5","6" };
			for ( U8 i = 1; i <= 6; i++ )
			{
				mpConcList->SetItemTitle( i, p[i-1] );
			}
			
			SetPageData( mCurPage - 1 );
		}
	}
}

void	CEnmConcListPage::OnKeyRight()
{
	if ( mCurPage < (MAX_PAGE - 1) )
	{
		if ( mpConcList->OverRight() )
		{
			MCHAR* p[] = { "7","8","9","10","11","12" };
			for ( U8 i = 1; i <= 6; i++ )
			{
				mpConcList->SetItemTitle( i, p[i-1] );
			}
			
			//mCurPage++;
			SetPageData( mCurPage + 1 );
			//CalculateConc();
		}
	}
}

#include "EnmPrinter.hpp"
void	CEnmConcListPage::OnKeyPrnt()
{
	CEnmPrinter aPrinter;
	aPrinter.SetPrintContext( &CEnmApp::Instance().GetScanContext() );
	
	aPrinter.Print();

}

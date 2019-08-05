#include "EnmDensityListPage.h"
#include "EnmConcDataUtil.h"

CEnmDensityListPage::CEnmDensityListPage()
{
	Init();
}

void	CEnmDensityListPage::Init()
{
	mDataCount = 0;
	memset( mDensity, 0, sizeof(mDensity) );
}

void    CEnmDensityListPage::FocusOn()
{
	CalculateDensity();

	inherited::FocusOn();
}

void	CEnmDensityListPage::CalculateDensity()
{
	CEnmScanContext &aScanProt = GetScanContext();

	if ( aScanProt.IsConcCalculated() )
	{
		FP64 *apConc = (FP64*)aScanProt.mCalcData.GetConc( TRUE );
		mDataCount = aScanProt.GetSampleCount( );;
	
		tagDateFormulate aFormulate;
		if ( Convert2Formulate( aFormulate ) )
		{
			EnmCompareCal( &aFormulate, apConc, mDensity, ENM_DATA_GROUP, ENM_DATA_COUNT );
		}
		else
		{
			mDataCount = 0;
			memset( mDensity, 0, sizeof(mDensity) );
		}
	}
	else
	{
		mDataCount = 0;
		memset( mDensity, 0, sizeof(mDensity) );
	}
}

BOOLEAN CEnmDensityListPage::Convert2Formulate( tagDateFormulate& rFormulate )
{
	CEnmScanContext &aScanProt = GetScanContext();
	U16 aStdCnt = aScanProt.mProtocol.stQuality.uStdRepeats * aScanProt.mProtocol.stQuality.uStd;

	rFormulate.calMethod = ENM_FORMULA_COMPARE;
	rFormulate.unFormul.stCompareCal.culFormula = 0;
	rFormulate.unFormul.stCompareCal.sampleCnt = aStdCnt;
	rFormulate.unFormul.stCompareCal.xUnit = aScanProt.mProtocol.stQuality.uAxisX;
	rFormulate.unFormul.stCompareCal.yUnit = aScanProt.mProtocol.stQuality.uAxisY;

	U16 aCnt = 0;
	for ( U16 i = 0; i < aScanProt.mProtocol.stQuality.uStd; i++ )
	{
		rFormulate.unFormul.stCompareCal.stSampleData[aCnt++] = aScanProt.mProtocol.stQuality.stStdDensity[i];

		for ( U16 j = 1; j < aScanProt.mProtocol.stQuality.uStdRepeats; j++ )
		{
			rFormulate.unFormul.stCompareCal.stSampleData[aCnt] = aScanProt.mProtocol.stQuality.stStdDensity[i];
			if ( rFormulate.unFormul.stCompareCal.stSampleData[aCnt-1].yPos < ENM_DATA_COUNT - 1 )
			{
				rFormulate.unFormul.stCompareCal.stSampleData[aCnt].yPos = rFormulate.unFormul.stCompareCal.stSampleData[aCnt-1].yPos + 1;
			}
			else
			{
				rFormulate.unFormul.stCompareCal.stSampleData[aCnt].yPos = 0;
				rFormulate.unFormul.stCompareCal.stSampleData[aCnt].xPos = rFormulate.unFormul.stCompareCal.stSampleData[aCnt-1].xPos + 1;
			}

			aCnt++;
		}
	}

	return TRUE;
}

void CEnmDensityListPage::SetPageData( S8 page )
{
	FP64 fConc;

	U16 aFocusLn = mpConcList->GetCurLine();
	U16 aStart;
	U16 aCount = 0;
	if ( mDataCount <= 0 )
	{
		mpConcList->SetConc( NULL, 0 );
		VisibleListFrom( 0, FALSE );
		return;
	}
	
	aStart = page * ENM_DATA_GROUP * ENM_DATA_COUNT / 2;
	aCount = mDataCount - aStart;

	if ( aCount > 0 )
	{
		mpConcList->SetConc( &mpConcList[aStart], aCount );
	}
	else
	{
		mpConcList->SetConc( NULL, 0 );
		aCount = 0;
	}

	VisibleListFrom( 0, TRUE );
	VisibleListFrom( aCount, FALSE );

	U16 aCurLn = mpConcList->GetCurLine();
	if ( mCurPage <= page )
		mpConcList->ItemFocusOn( aCurLn, 1 );
	else
		mpConcList->ItemFocusOn( aCurLn, 6 );

	mCurPage = page;
}



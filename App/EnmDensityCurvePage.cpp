#include "EnmDensityCurvePage.h"
#include <math.h>

CEnmDensityCurvePage::CEnmDensityCurvePage()
: CEnmBasePage( "", 1 )
{
	Init();
}

void CEnmDensityCurvePage::Init()
{
	mX0 = 36;
	mY0 = 240 - 30;
	mLenX = 270;
	mLenY = 180;

	mStepX = 30;
	mStepY = 30;

	mDataCount = 0;
	memset( mDensity, 0, sizeof(mDensity) );

	mConcMax = 9.0;
	mDensityMax = 6;
}

void  CEnmDensityCurvePage::FocusOn()
{
	CalculateDensity();

	inherited::FocusOn();
}


void CEnmDensityCurvePage::Draw()
{
	inherited::Draw();
	
	DrawCurve();
}

void CEnmDensityCurvePage::DrawCurve()
{
	DrawAxis();

	CEnmScanContext& rScanContext = GetScanContext();
	switch ( rScanContext.mProtocol.stQuality.uCurve )
	{
	case 0:
		DrawCurveLinear();
		break;

	case 1:
		DrawCurveSegment();
		break;
	}

}

void CEnmDensityCurvePage::DrawAxis()
{	
	tagPoint aArrowX[] = { {mX0 + mLenX - 7, mY0 - 3}, {mX0 + mLenX, mY0}, {mX0 + mLenX - 7, mY0 + 3} };
	tagPoint aArrowY[] = { {mX0-3, mY0 - mLenY + 7}, {mX0, mY0 - mLenY}, {mX0 + 3, mY0 - mLenY + 7} };

	PutLine( mX0, mY0 + 5, mX0, mY0 - mLenY );
	PutLine( mX0 - 5, mY0, mX0 + mLenX, mY0  );

	/// Draw axis-x arrow 
	PutLine( aArrowX[0].x, aArrowX[0].y, aArrowX[1].x, aArrowX[1].y );
	PutLine( aArrowX[2].x, aArrowX[2].y, aArrowX[1].x, aArrowX[1].y );

	/// Draw aixs-y arrow
	PutLine( aArrowY[0].x, aArrowY[0].y, aArrowY[1].x, aArrowY[1].y );
	PutLine( aArrowY[2].x, aArrowY[2].y, aArrowY[1].x, aArrowY[1].y );

	/// Draw scaler
	U16 i;
	U16 aScalerLen = 4;
	U16 aSteps = mLenX / mStepX;
	for ( i = 1; i < aSteps; i++ )
	{
		PutLine( mX0 + i * mStepX, mY0 - aScalerLen, mX0 + i * mStepX, mY0 ); 
	}

	aSteps = mLenY / mStepY;
	for ( i = 1; i < aSteps; i++ )
	{
		PutLine( mX0, mY0 - i * mStepY, mX0 + aScalerLen, mY0 - i * mStepY );
	}

	DrawAxisUnit();
}

void CEnmDensityCurvePage::DrawAxisUnit()
{
	U16 i;
	U16 aSteps = mLenX / mStepX;
	FP64 aStepConc = ( mConcMax / aSteps );

	MCHAR aStr[8] = { 0 };
	for ( i = 1; i < aSteps; i++ )
	{
		FormatFP2( i * aStepConc, aStr, 4, 3 );
		PutStr( mX0 + i * mStepX - 16, mY0 + 0, aStr, 0 );
	}

	aSteps = mLenY / mStepY;
	FP64 aStepDensity = mDensityMax / aSteps;
	for ( i = 1; i < aSteps; i++ )
	{
		FormatFP2( i * aStepDensity, aStr, 4, 3 );
		PutStr( mX0 - 28, mY0 - i * mStepY - 10, aStr, 0 );
		//PutLine( mX0 - aScalerLen, mY0 - i * mStepY, mX0 + aScalerLen, mY0 - i * mStepY );
	}
}

void CEnmDensityCurvePage::DrawCurveLinear()
{
	if ( mDataCount <= 0 )
		return;

	if ( abs( mK ) < 0.0001 )
		return;

	FP64 x0, y0, x1, y1, aX, aY;
	x0 = 0.0;
	x1 = mConcMax;
	y0 = mK * x0 + mC;
	y1 = mK * x1 + mC;

	aY = mDensityMax;
	aX = (aY - mC) / mK;

	if ( y1 > mDensityMax )
	{
		x1 = aX;
		y1 = aY;
	}

	PutLine( mX0 + x0 * mStepX, mY0 - y0 * mStepY, mX0 + x1 * mStepX, mY0 - y1 * mStepY );
}

void CEnmDensityCurvePage::DrawCurveSegment()
{
}

U16  CEnmDensityCurvePage::CalculateDensity()
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
			PickMax();
			mK = aFormulate.stCoefficent.a;
			mC = aFormulate.stCoefficent.b;
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

	return mDataCount;
}

BOOLEAN CEnmDensityCurvePage::Convert2Formulate( tagDateFormulate& rFormulate )
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

void  CEnmDensityCurvePage::PickMax()
{
	FP64 aConc = 0;
	FP64 aDensity = 0;

	FP64 *apConc = (FP64*)GetScanContext().mCalcData.GetConc( TRUE );
	for ( U16 i = 0; i < mDataCount; i++ )
	{
		if ( mDensity[i] > aDensity )
			aDensity = mDensity[i];

		if ( apConc[i] > aConc )
			aConc = apConc[i];
	}

//	mDensityMax = aDensity;
//	mConcMax = aConc;
}
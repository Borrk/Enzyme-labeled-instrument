// EnmPlatePage.cpp: implementation of the CEnmPlatePage class.
//
//////////////////////////////////////////////////////////////////////

#include "EnmPlatePage.h"
#include "EnmScanContext.h"
#include "EnmApp.hpp"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnmPlatePage::CEnmPlatePage()
: CEnmBasePage( "", ENM_PAGE_ID_PLATE )
{
	Init();
	SetTitle( STR_PSAMPLEPLATE_TITLE );
}

CEnmPlatePage::~CEnmPlatePage()
{

}

void CEnmPlatePage::Init(void)
{
	mStartX = 6;
	mStartY = 30;
	mStepX = 25 * 2;
	mStepY = 20 ;

	mXFrom  = 0;
	mXTo	= 12;
	mStepX	= 24;
	mYFrom  = 0;
	mYTo    = 8;
	mStepY  = 20;

}

void  CEnmPlatePage::Draw( void )
{
	CEnmBasePage::Draw();

	DrawTips();
	DrawMatrix();
}

void  CEnmPlatePage::DrawTips( void )
{
	MCHAR aTips[4];
	for ( short i = mXFrom; i < mXTo; i++ )
	{
		sprintf( aTips, "%2d", i+1 );
		PutStr( 20 + mStartX + mStepX * (i - mXFrom), mStartY, aTips, 0 );
	}

	aTips[1] = '\0';
	for ( short j = mYFrom; j < mYTo; j++ )
	{
		aTips[0] = 'A' + j;
		PutStr( mStartX, 20 + mStartY + mStepY * (j - mYFrom), aTips, 0 );
	}		
}

void  CEnmPlatePage::DrawMatrix( void )
{ 
	if ( CEnmApp::Instance().GetScanContext().mProtocol.stMeasure.uMeasMode == MEASURE_QUALITATIVE )
		DrawQualitativeMatrix();
	else
		DrawQualititiveMatrix();
}

void CEnmPlatePage::DrawQualititiveMatrix()
{
	tagQualititive *ap = &CEnmApp::Instance().GetScanContext().mProtocol.stQuality;

	U16 aIdx = 0; //ap->uBlank + ap->uCO + ap->uHP + ap->uLP + ap->uNC + ap->uPC + ap->uSample;

	U16 i;
	MCHAR  aCell[4];

	for ( i = 0; i < ap->uBlank; i++ )
	{
		sprintf( aCell, "B%d", i + 1 );
		DrawCell( TRUE, aIdx, aCell );
		aIdx++;
	}

	for ( i = 0; i < ap->uStd; i++ )
	{
		for ( U16 j = 0; j < ap->uStdRepeats; j++ )
		{
			sprintf( aCell, "S%d", i + 1 );
			DrawCell( TRUE, aIdx, aCell );
			aIdx++;
		}
	}

	for ( i = 0; i < ap->uSample; i++ )
	{
		sprintf( aCell, "%d", i + 1 );
		DrawCell( TRUE, aIdx, aCell );
		aIdx++;
	}
}

void CEnmPlatePage::DrawQualitativeMatrix()
{
	tagQualitative *ap = &CEnmApp::Instance().GetScanContext().mProtocol.stQualitative;

	U16 aIdx = 0; //ap->uBlank + ap->uCO + ap->uHP + ap->uLP + ap->uNC + ap->uPC + ap->uSample;

	U16 i;
	MCHAR  aCell[4];
	for ( i = 0; i < ap->uBlank; i++ )
	{
		sprintf( aCell, "B%d", i + 1 );
		DrawCell( TRUE, aIdx, aCell );
		aIdx++;
	}

	for ( i = 0; i < ap->uPC; i++ )
	{
		sprintf( aCell, "P%d", i + 1 );
		DrawCell( TRUE, aIdx, aCell );
		aIdx++;
	}

	for ( i = 0; i < ap->uNC; i++ )
	{
		sprintf( aCell, "N%d", i + 1 );
		DrawCell( TRUE, aIdx, aCell );
		aIdx++;
	}

	for ( i = 0; i < ap->uCO; i++ )
	{
		sprintf( aCell, "C%d", i + 1 );
		DrawCell( TRUE, aIdx, aCell );
		aIdx++;
	}

	for ( i = 0; i < ap->uLP; i++ )
	{
		sprintf( aCell, "L%d", i + 1 );
		DrawCell( TRUE, aIdx, aCell );
		aIdx++;
	}
	for ( i = 0; i < ap->uHP; i++ )
	{
		sprintf( aCell, "H%d", i + 1 );
		DrawCell( TRUE, aIdx, aCell );
		aIdx++;
	}

	/// Samples
	for ( i = 0; i < ap->uSample; i++ )
	{
		sprintf( aCell, "%02d", i + 1 );
		DrawCell( TRUE, aIdx, aCell );
		aIdx++;
	}
	
}

void  CEnmPlatePage::DrawCell( BOOLEAN bHort, U16 idx, MCHAR* pStr )
{
	if ( !pStr )
		return;

	U16 x = idx / 8;
	U16	y = idx % 8;
	PutStr( 16 + mStartX + mStepX * x, 20 + mStartY + mStepY * y, pStr, 0 );
}
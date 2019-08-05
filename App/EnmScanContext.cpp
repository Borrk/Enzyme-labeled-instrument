// ScanContext.cpp: implementation of the CScanContext class.
//
//////////////////////////////////////////////////////////////////////

#include "EnmScanContext.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnmScanContext::CEnmScanContext()
{
	Init();
}

CEnmScanContext::~CEnmScanContext()
{

}

void  CEnmScanContext::Init()
{
	mRefProtID = -1;
	
	strcpy( mProtocol.stMeasure.szName, "Hbs" );
	mProtocol.stMeasure.uID				= 1;
	mProtocol.stMeasure.uMeasMode		= MEASURE_QUALITATIVE;
	mProtocol.stMeasure.uMainFilter		= 1;
	mProtocol.stMeasure.uReferFilter	= 2;
	mProtocol.stMeasure.uSlaveFilter	= 3;
	mProtocol.stMeasure.uMixingMode		= MIX_NONE;
	mProtocol.stMeasure.uMixingTime		= 2;
	mProtocol.stMeasure.uTempratureCtrl = 0;
	mProtocol.stMeasure.fTemprature		= 37.0;
	mProtocol.stMeasure.kFactor			= 1.0;
	
	/// Qualitative
	mProtocol.stQualitative.fGrayN		= 0.3;
	mProtocol.stQualitative.fGrayP		= 1.0;
	mProtocol.stQualitative.uBlank		= 2;
	mProtocol.stQualitative.uCO			= 0;
	mProtocol.stQualitative.uHP			= 0;
	mProtocol.stQualitative.uLP			= 0;
	mProtocol.stQualitative.uPC			= 2;
	mProtocol.stQualitative.uNC			= 2;
	mProtocol.stQualitative.uSample		= 88;
	mProtocol.stQualitative.uPrintFormat= ENM_CUTOFF_PRINT_ASEND;
	strcpy( mProtocol.stQualitative.szCutoff, "K*2.1" );
	
	/// Qualititive
	mProtocol.stQuality.uBlank			= 2;
	mProtocol.stQuality.uStd			= 4;
	mProtocol.stQuality.uStdRepeats		= 1;
	mProtocol.stQuality.uSample			= 0;
	mProtocol.stQuality.uUnit			= 0;
	mProtocol.stQuality.uAxisX			= AXIS_LN;
	mProtocol.stQuality.uAxisY			= AXIS_LN;
	
	for ( U8 i = 0; i < STD_MAX; i ++ )
	{
		mProtocol.stQuality.stStdDensity[i].density = 0.0;
		mProtocol.stQuality.stStdDensity[i].xPos = 0;
		mProtocol.stQuality.stStdDensity[i].yPos = i;
	}

	mPeriod = 0;
	mRepeatCnt = 1;
}

void CEnmScanContext::SetScanData( tagScanData& data )
{
	/// if all the 12 columns is zero, has no scan data
	if ( data.uColumnMask && 0x0FFF != 0 ) 
	{
		mData = data;
		mbHasNewData = TRUE;
		mCalcData.ResetData();
	}
}


U16	CEnmScanContext::GetSampleCount( BOOLEAN bIncludeCtrl )
{
	U16 aCnt = 0;
	if ( mProtocol.stMeasure.uMeasMode == MEASURE_QUALITATIVE )
	{
		aCnt = mProtocol.stQualitative.uSample;
		if ( bIncludeCtrl )  
			aCnt += mProtocol.stQualitative.uBlank + mProtocol.stQualitative.uCO
			     +  mProtocol.stQualitative.uHP + mProtocol.stQualitative.uLP
				 +  mProtocol.stQualitative.uNC + mProtocol.stQualitative.uPC;
	}
	else if ( mProtocol.stMeasure.uMeasMode == MEASURE_QUALITITIVE )
	{
		aCnt = mProtocol.stQuality.uSample;
		if ( bIncludeCtrl )
			aCnt += mProtocol.stQuality.uBlank 
			     +  mProtocol.stQuality.uStd * mProtocol.stQuality.uStdRepeats;
	}
	else
	{
		if ( mFlag == SC_SCAN_KINETIC || mFlag == SC_SCAN_FULL )
		{
			aCnt = ENM_DATA_COUNT * GetScanColumns( TRUE );
		}
		else if ( mFlag == SC_SCAN_CM )
		{
			if ( GetScanColumns( TRUE ) )
				aCnt = ENM_DATA_COUNT;
			else
				aCnt = 0;
		}
		else
			aCnt = 0;
	}
		
	return aCnt;
}

U16	CEnmScanContext::GetConcCount( U32 type )
{
	U16 aCnt = 0;
	if ( mProtocol.stMeasure.uMeasMode == MEASURE_QUALITATIVE )
	{
		if ( type >= QUALITATIVE_CONC_UNKNOW )
			return 0;

		U16 pos[] = 
		{
			mProtocol.stQualitative.uBlank,
			mProtocol.stQualitative.uSample,
			mProtocol.stQualitative.uPC,
			mProtocol.stQualitative.uNC,
			mProtocol.stQualitative.uCO,
			mProtocol.stQualitative.uLP,
			mProtocol.stQualitative.uHP,
		};

		aCnt = pos[type];
	}
	else
	{
		if ( type > QUALITY_CONC_UNKNOW )
			return 0;

		S16 pos[] = 
		{
			mProtocol.stQuality.uBlank,
			mProtocol.stQuality.uSample,
			mProtocol.stQuality.uStd * mProtocol.stQuality.uStdRepeats,
		};

		aCnt = pos[type];
	}

	return aCnt;
}

S16	CEnmScanContext::GetConcStartPos( U32 type )
{
	S16 aPos = -1;
	if ( mProtocol.stMeasure.uMeasMode == MEASURE_QUALITATIVE )
	{
		if ( type >= QUALITATIVE_CONC_UNKNOW )
			return -1;

		S16 pos[] = 
		{
			0, 
		
			mProtocol.stQualitative.uBlank + mProtocol.stQualitative.uNC + mProtocol.stQualitative.uCO
			+ mProtocol.stQualitative.uLP  + mProtocol.stQualitative.uPC + mProtocol.stQualitative.uHP,
		
			mProtocol.stQualitative.uBlank,
		
			mProtocol.stQualitative.uBlank + mProtocol.stQualitative.uPC,
		
			mProtocol.stQualitative.uBlank + mProtocol.stQualitative.uPC + mProtocol.stQualitative.uNC,
			
			mProtocol.stQualitative.uBlank + mProtocol.stQualitative.uPC + mProtocol.stQualitative.uNC
			+ mProtocol.stQualitative.uCO,
		
			mProtocol.stQualitative.uBlank + mProtocol.stQualitative.uPC + mProtocol.stQualitative.uNC
			+ mProtocol.stQualitative.uCO + mProtocol.stQualitative.uLP,
		};

		aPos = pos[type];
	}
	else
	{
		if ( type > QUALITY_CONC_UNKNOW )
			return -1;

		S16 pos[] = 
		{
			0, 
			mProtocol.stQuality.uBlank + mProtocol.stQuality.uStd * mProtocol.stQuality.uStdRepeats,
			mProtocol.stQuality.uBlank,
		};

		aPos = pos[type];
	}

	return aPos;
}

U16	CEnmScanContext::GetScanColumns( BOOLEAN bForce )
{
	tagScanData* apData = GetScanData( bForce );
	if ( !apData && !bForce )
		return 0;


	U32 aColMask = apData->uColumnMask;
	if ( bForce )
		aColMask = mProtocol.stMeasure.uColumnMask;
	
	U16 aColumns = 0;
	if ( mProtocol.stMeasure.uMeasMode == MEASURE_COMMON )
	{
		for ( U8 i = 0; i < 12; i++ )
		{
			if ( aColMask & 1 )
				aColumns++;
			aColMask >>= 1;
		}
	}
	else
	{
		aColumns = GetSampleCount() / 8;
		if ( GetSampleCount() % 8 )
			aColumns++;
	}

	return aColumns;
}

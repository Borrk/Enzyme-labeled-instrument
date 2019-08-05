#include "EnmDataProc.h"
#include "EnmFormater.hpp"

CEnmFormater::CEnmFormater()
{
	Init();	
}

INT16U  CEnmFormater::FormatDataIndex( INT16U dataCnt, MCHAR* pszDst )
{
	MCHAR aszIdx[16];
	INT16U aLen;

	sprintf( pszDst, "%2c", ' ' );
	aLen = strlen(pszDst);

	for ( INT16U i = 0; i < dataCnt; i++ )
	{
		sprintf( aszIdx, "%d", i+1 );
		PadFormat( aszIdx, &pszDst[aLen], mUnitLen, 0 );
		//PadFormat( " ", &pszDst[strlen(pszDst)], 1 );
		aLen = strlen(pszDst );
	}

	return 1;
}

INT16U  CEnmFormater::Format( INT16U ln, FP64 *pData, INT16U dataCnt, MCHAR* pszDst )
{
	INT16U	aLnCnt = 1;

	if ( !pData || !pszDst )
		return 0;

	INT8U i;
	INT16U aLen;
	MCHAR  aszTemp[16];

	//pszDst[0] = 'A' + ln;
	//pszDst[1] = ' ';
	//pszDst[2] = '\0';
	pszDst[0] = '\0';
	
	for ( i = 0; i < dataCnt; i++ )
	{
		aLen = strlen(pszDst);
	
		FormatFP( pData[i], aszTemp, mUnitLen, mUnitLen-3 );
		PadFormat( aszTemp, &pszDst[aLen], mUnitLen, 0 );
		
		aLen = strlen(pszDst);
	//	pszDst[aLen+1] = 0;
	//	pszDst[aLen] = ' ';
	}

	return aLnCnt;
}
	
void  CEnmFormater::Init()
{
	mUnitLen = 6;
	mInterval = 0;
	mLeftAlign = 3;
	
	memset( &mOption, 0 , sizeof(mOption) );
	
	mbOptionSet = FALSE;
	
}
/*
///< 0: center, 1: left, 2: right
void  CEnmFormater::PadFormat( MCHAR *pSrc, MCHAR *pDst, INT16U width, INT8U aligned)
{
	INT16U i, aLen , aPre, aPost;

	aLen = strlen( pSrc );

	if ( aligned == 0 )
	{
		aPost = (width - aLen) / 2;
		aPre = (width -aLen - aPost );
	}
	else if ( aligned == 1 )	///< left
	{
		aPre = 0;
		aPost = width - aLen;
	}
	else 
	{
		aPre = width - aLen;
		aPost = 0;
	}

	/// pre blank char
	for ( i = 0; i < aPre; i++ )
	{
		sprintf( &pDst[i], "%c", ' ' );
	}

	/// main body
	strcpy( &pDst[i], pSrc );

	/// post blank char
	for ( i = 0; i < aPost; i++ )
	{
		sprintf( &pDst[i+aLen+aPre], "%c", ' ' );
	}
}
*/

/**
 *	CEnmFormaterRange 
 *
 */
INT16U  CEnmFormaterRange::Format( INT16U ln, FP64 *pData, INT16U dataCnt, MCHAR* pszDst )
{
	MCHAR   aszTemp[32];
	INT16U	aLnCnt = 1;

	if ( !pData || !pszDst )
		return 0;
	
	if ( !mbOptionSet )
		return 0;
	
	MCHAR  aBig, aSmall, ch;
	if ( mSortType == 0 ) 
	{
		aBig = '+';
		aSmall = '-';
	}
	else
	{
		aBig = '-';
		aSmall = '+';
	}
		
	INT8U i, j;
	INT8U aVal;
	INT16U aLen;

	//pszDst[0] = 'A' + ln;
	//pszDst[1] = ' ';
	//pszDst[2] = '\0';
	pszDst[0] = '\0';
	
	for ( i = 0; i < dataCnt; i++ )
	{
		aLen = strlen(pszDst);
		if ( pData[i] >= mHigh )
		{
			ch = aBig;
		}
		else if ( pData[i] < mLow )
		{
			ch = aSmall;
		}
		else
		{
			ch = ' ';
		}

		sprintf( aszTemp, "%c", ch );
		PadFormat( aszTemp, &pszDst[aLen], mUnitLen, 0 );

		//sprintf( &pszDst[aLen], "%c", ch );
		//for ( j = 0; j < mUnitLen - 1; j++ )
		//{
		//	sprintf( &pszDst[aLen+1+j], "%c", ' ' );
		//}

		aLen += mUnitLen;
		//aLen = strlen(pszDst);
		//pszDst[aLen+1] = 0;
		//pszDst[aLen] = ' ';
	}

	return aLnCnt;
}

/**
 *	CEnmFormaterSegment 
 *
 */
INT16U  CEnmFormaterSegment::Format( INT16U ln, FP64 *pData, INT16U dataCnt, MCHAR* pszDst )
{
	MCHAR   aszTemp[32];
	INT16U	aLnCnt = 1;

	if ( !pData || !pszDst )
		return 0;
	
	if ( !mbOptionSet )
		return 0;
		
	INT8U i, j;
	INT8U aVal;
	INT16U aLen;
	FP64   aRange = mOption.stCalFormula.unFormul.stRangeCal.maxVal / 10.0;
	if ( aRange < 0.00001 )
		return 0;

	//pszDst[0] = 'A' + ln;
	//pszDst[1] = ' ';
	//pszDst[2] = '\0';
	pszDst[0] = '\0';
	
	for ( i = 0; i < dataCnt; i++ )
	{
		aLen = strlen(pszDst);
		if ( pData[i] > mOption.stCalFormula.unFormul.stRangeCal.maxVal )
		{
			aVal = 9;
		}
		else if ( pData[i] < 0.0 )
		{
			aVal = 0;
		}
		else
		{
			aVal = (INT8U)(pData[i] / aRange);
		}

		sprintf( aszTemp, "%d", aVal );
		PadFormat( aszTemp, &pszDst[aLen], mUnitLen, 0 );

		aLen += mUnitLen;
		//aLen = strlen(pszDst);
		//pszDst[aLen+1] = 0;
		//pszDst[aLen] = ' ';
	}

	return aLnCnt;
}

/**
 *	CEnmFormaterCutoff 
 *
 */
INT16U  CEnmFormaterCutoff::Format( INT16U ln, FP64 *pData, INT16U dataCnt, MCHAR* pszDst )
{
	INT16U aLn = 0;
	FP64  aZoneP, aZoneN;

	if ( mbValidCutoff )
	{
		aZoneP = mCutoff * mOption.stCalFormula.unFormul.stPNCal.grayAreaP / 100.0;
		aZoneN = mCutoff * mOption.stCalFormula.unFormul.stPNCal.grayAreaN / 100.0;
	}
	else
		return aLn;

	switch( mOption.stCalFormula.unFormul.stPNCal.printType )
	{
	case ENM_CUTOFF_PRINT_ASEND:
		SetSortType( 0 );
		SetThreshold( aZoneP, aZoneN );
		aLn = CEnmFormaterRange::Format( ln, pData, dataCnt, pszDst );
		break;

	case ENM_CUTOFF_PRINT_DEASEND:
		SetSortType( 1 );
		SetThreshold( aZoneP, aZoneN );
		aLn = CEnmFormaterRange::Format( ln, pData, dataCnt, pszDst );
		break;

	case ENM_CUTOFF_PRINT_CUTOFF:
		aLn = FormatCutoff( ln, pData, dataCnt, pszDst, aZoneP, mCutoff, aZoneN );
		break;
	}

	return aLn;
}

void	CEnmFormaterCutoff::SetCoefficent( const tagPNRefSample& pnl, FP64 cutoff )
{
	mPNL = pnl;
	mCutoff = cutoff;
	mbValidCutoff = TRUE;
}

INT16U  CEnmFormaterCutoff::FormatCutoff( INT16U ln, FP64 *pData, INT16U dataCnt, MCHAR* pszDst, FP64 zoneP, FP64 cutoff, FP64 zoneN )
{
	INT16U	aLnCnt = 3;

	if ( !pData || !pszDst )
		return 0;
	
	if ( !mbOptionSet )
		return 0;
		
	INT8U i, j;
	INT8U aVal;
	INT16U aLen;

	MCHAR aszTemp[16];
	MCHAR *apDst = pszDst;
	MCHAR aszData[256] = { 0 };
	static int aSampleIndex = 1;


	/// first line, sample flag( P, N, L, sample number...)
	MCHAR aTag[6] = { 'P', 'N', 'B', 'L', 'Q', 'O' };
	INT8S *pTag = (INT8S*)mOption.stCalFormula.unFormul.stPNCal.sampleMatrix[ln];

	for ( i = 0; i < dataCnt; i++ )
	{
		aLen = strlen(aszData);

		if ( pTag[i] != ENM_CUTOFF_O )
		{
			sprintf( aszTemp, "%c", aTag[pTag[i]] );
			PadFormat( aszTemp, &aszData[aLen], mUnitLen, 0 );
		}
		else
		{
			sprintf( aszTemp, "%d", aSampleIndex++ );
			PadFormat( aszTemp, &aszData[aLen], mUnitLen, 0 );
		}

		aLen += mUnitLen;
	}

	strcpy( apDst, aszData );
	apDst += (strlen(aszData)+1);

	/// 2nd line, OD value
//	aszData[0] = ' '; //'A' + ln;
//	aszData[1] = ' ';
//	aszData[2] = '\0';
	aszData[0] = '\0';
	
	for ( i = 0; i < dataCnt; i++ )
	{
		aLen = strlen(aszData);

		FormatFP( pData[i], aszTemp, mUnitLen, mUnitLen-3 );
		PadFormat( aszTemp, &aszData[aLen], mUnitLen, 0 );
		aLen = strlen(aszData);
	}
	strcpy( apDst, aszData );
	apDst += (strlen(aszData)+1);

	/// 3rd line, P,N result
	INT8U aIdx;
	MCHAR *pPNTag[] = 
	{
		" +  ",	///< P
		" ++ ",	///< P gray
		" -  ",	///< N
		" -- ",	///< N gray
		"    "		///< black 
	};

//	aszData[0] = ' '; //'A' + ln;
//	aszData[1] = ' ';
//	aszData[2] = '\0';
	aszData[0] = '\0';
	for ( i = 0; i < dataCnt; i++ )
	{
		aLen = strlen(aszData);

		if ( pData[i] >= zoneP )
		{
			aIdx = 0;
		}
		else if ( pData[i] >= cutoff )
		{
			aIdx = 1;
		}
		else if ( pData[i] >= zoneN )
		{
			aIdx = 2;
		}
		else
		{
			aIdx = 3;
		}

		if ( pTag[i] == ENM_CUTOFF_P || pTag[i] == ENM_CUTOFF_N || pTag[i] == ENM_CUTOFF_L || pTag[i] == ENM_CUTOFF_B )
		{
			aIdx = 4;
		}

		PadFormat( pPNTag[aIdx], &aszData[aLen], mUnitLen, 0 );

		aLen += mUnitLen;
	}
	strcpy( apDst, aszData );

	return aLnCnt;
}

BOOLEAN  CEnmFormaterCutoff::PLNCalCutoff( tagPNCal& cal, tagEnmData& pData )
{
	mbValidCutoff = EnmPLNCalCutoff( cal, (FP64*)pData.dbData, ENM_REAGENT_COL_MAX, ENM_REAGENT_LN_MAX, &mCutoff, mPNL );

	return mbValidCutoff;
}

#include <math.h>
#include "EnmApi.h"
#include "EnmDataProc.h"
#include "EnmPrinter.hpp"
#include "EnmFormater.hpp"
#include "EnmScanContext.h"
#include "MStrMngr.h"
#include "EnmStringTbl.hpp"
#include "EnmSysCfgLib.h"

/**
 *		¥Ú”°
 *
 */ 	

CEnmPrinter::CEnmPrinter()
{
	Init();
}

void CEnmPrinter::Init()
{	
	mLeftAlign = 4;
	mpScanContext = NULL;
}

void CEnmPrinter::SetPrintContext( CEnmScanContext *pContext )
{
	mpScanContext = pContext;
}

U16 CEnmPrinter::PickRowConcData( FP64 *pData, U16 row, FP64* pfConc, U16 uMax )
{
	if ( !pData || !pfConc || uMax < 1 )
		return 0;

	U16 aCnt = uMax / ENM_DATA_COUNT;
	if ( (uMax % ENM_DATA_COUNT) > row )
		aCnt++;

	for ( U16 i = 0; i < aCnt; i++ )
	{
		pfConc[i] = pData[i * ENM_DATA_COUNT + row];
	}
	
	return aCnt;
}

BOOLEAN CEnmPrinter::Print( void )
{
	if ( mpScanContext == NULL )
		return FALSE;
	
	SetPrinter();

	PrintHeader();

	CEnmFormater  aFmt;
//	CEnmFormaterSegment  aSeg;
//	CEnmFormaterRange aRange;
//	CEnmFormaterCutoff  aCutoff;
//	tagPNRefSample aPNL;
//
//	switch( mOption.stCalFormula.calMethod )
//	{
//	case ENM_FORMULA_SEGMENT:
//		mpCurFormater = &aSeg;
//		break;
//
//	case ENM_FORMULA_THRESHOLD:
//		mpCurFormater = &aRange;
//		aRange.SetSortType( 0 );
//		aRange.SetThreshold( mOption.stCalFormula.unFormul.stThresholdCal.threshold, mOption.stCalFormula.unFormul.stThresholdCal.threshold );
//		break;
//
//	case ENM_FORMULA_BI_THRESHOLD:
//		mpCurFormater = &aRange;
//		aRange.SetSortType( 0 );
//		aRange.SetThreshold( mOption.stCalFormula.unFormul.stBiThresholdCal.thresholdHi, mOption.stCalFormula.unFormul.stBiThresholdCal.thresholdLo );
//		break;
//
//	case ENM_FORMULA_CRITICAL_VAL:			
//		mpCurFormater = &aCutoff;
//		aPNL.P = mOption.stCalFormula.stCoefficent.a;
//		aPNL.N = mOption.stCalFormula.stCoefficent.b;
//
//		aCutoff.SetCoefficent( aPNL, mOption.stCalFormula.stCoefficent.c );
//
//		break;
//	
//
//	default:
//		mpCurFormater = &aFmt;
//	}

	tagDataOption	aOption;
	//aOption.curReagent = 1;
	mpCurFormater = &aFmt;
	mpCurFormater->SetOption( aOption );

	PrintMainData();
	PrintAttachData();
	
	return TRUE;
}

void CEnmPrinter::PrintHeader( void )
{
	EnmPrntCRLF();
	EnmPrntCRLF();

	/// test time, tester ,etc
	PrintTestInfo();

	PrintTestMode();
}

void CEnmPrinter::PrintTestInfo( void )
{
	INT16U	aCurIdx = 0;
	MCHAR	aTitle[8][128];

	tagDate aDate = mpScanContext->GetScanData(TRUE)->stDate;

	sprintf( aTitle[aCurIdx++], "Test Time: %4d/%02d/%02d",  aDate.year, aDate.month, aDate.day ); 

	/// Print Test Mode info
	for ( INT16U i = 0; i < aCurIdx; i++ )
	{
		EnmPrntOneLine( mLeftAlign, aTitle[i], 1 );
	}
}

void CEnmPrinter::PrintTestMode( void )
{
	MCHAR aszModeInfo[256] = { 0 };
	U16 aCnt = FormatModeInfo( aszModeInfo );

	MCHAR *ap = aszModeInfo;
	for ( U16 i = 0; i < aCnt; i++ )
	{
		if ( ap )
		{
			EnmPrntOneLine( mLeftAlign, ap, 1 );
		}

		ap += strlen( ap ) + 1;
	}	
}

U16 CEnmPrinter::FormatModeInfo( MCHAR* pszInfo )
{
	if ( !mpScanContext )
		return 0;

	U16 aCnt = 0;
	U16 aId;
	MCHAR *apCur, *p1, *p2, *p3, *p4;

	apCur = pszInfo;

	tagProtocol* apProt = &mpScanContext->mProtocol;
	MStrMngr& rMngr = MStrMngr::Instance();

	/// format 1st line
	p1 = rMngr.GetString( STR_PPROGRAM_TEST );
	p2 = rMngr.GetString( STR_PPROGRAM_NAME );
	p3 = apProt->stMeasure.szName; // get mode's name
	aId = apProt->stMeasure.uID; /// get id
	sprintf( apCur, "%s %d, %s %s", p1, aId, p2, p3 );
	apCur += strlen( apCur ) + 1;
	aCnt++; 

	/// format 2nd line
	p1 = rMngr.GetString( STR_PPROGRAM_TYPE );
	p3 = rMngr.GetString( STR_PPROGRAM_CUTOFF );
	p2 = nGetMeasureModeStr( apProt->stMeasure.uMeasMode ); // get program's mode
	p4 = "08 Mar 2006"; /// get time
	sprintf( apCur, "%s %s%3c %s %s", p1, p2, ' ', p3, p4 );
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

MCHAR*	CEnmPrinter::nGetMeasureModeStr( U8 mode )
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

void CEnmPrinter::PrintAttachData( void )
{
}

void CEnmPrinter::PrintMainData( void )
{
	if ( !mpScanContext )
		return;

	INT16U i, j;
	INT16U aLn;
	MCHAR *apTitle;
	MCHAR aszLn[3] = { ' ' , ' ', '\0' };
	MCHAR aTitle[1024];
	FP64 aVal[24];

	EnmPrntCRLF();
	EnmPrntCRLF();

	mpCurFormater->FormatDataIndex( ENM_DATA_GROUP, aTitle );
	EnmPrntOneLine( mLeftAlign, aTitle, 1 );
	EnmPrntCRLF();

	/// print data
	FP64 *apConc = (FP64*)mpScanContext->mCalcData.fConc;
	U16 aCount = mpScanContext->GetSampleCount( TRUE );
	for ( i = 0; i < ENM_DATA_COUNT; i++ )
	{
		U16 aValidDataCnt = PickRowConcData( apConc, i, aVal, aCount );
		if ( aValidDataCnt < 1 )
			continue;
		
		aLn = mpCurFormater->Format( i, aVal, aValidDataCnt, aTitle );
		apTitle = aTitle;
		if ( aLn > 1 )	///< cutoff format is 3 lines
		{
			EnmPrntOneLine( mLeftAlign+2, apTitle, 1 );
			apTitle += strlen(apTitle) + 1;
	
			aszLn[0] = 'A' + i;
			EnmPrntOneLine( mLeftAlign, aszLn, 0 );
			EnmPrntOneLine( mLeftAlign + strlen(aszLn), apTitle, 1 );
			apTitle += strlen(apTitle) + 1;
		
			EnmPrntOneLine( mLeftAlign+2, apTitle, 1 );
			apTitle += strlen(apTitle) + 1;
		
			EnmPrntCRLF();
		}
		else
		{
			aszLn[0] = 'A' + i;
			EnmPrntOneLine( mLeftAlign, aszLn, 0 );
			EnmPrntOneLine( mLeftAlign + strlen(aszLn), apTitle, 1 );
			apTitle += strlen(apTitle) + 1;
		}

	}
}

INT16U CEnmPrinter::FormatData( FP64 *pData, MCHAR *pszDst, INT8U idx )
{
	INT8U i;
	INT16U aLen;

	pszDst[0] = 'A' + idx;
	pszDst[1] = ' ';
	pszDst[2] = '\0';
	srand(100);
	for ( i = 0; i < ENM_DATA_GROUP; i++ )
	{
		aLen = strlen(pszDst);
		FormatFP( pData[i], &pszDst[aLen], 6, 3 );

		aLen = strlen(pszDst);
		pszDst[aLen+1] = 0;
		pszDst[aLen] = ' ';
	}

	return 1;
}

void CEnmPrinter::FormatRawData( MCHAR *pszDst, INT8U idx )
{
	INT8U i;
	
/*
	pszDst[0] = 'A' + idx;
	pszDst[1] = ' ';
	pszDst[2] = '\0';
	srand(100);
	for ( i = 0; i < ENM_DATA_GROUP; i++ )
	{
		//sprintf( &pszDst[strlen(pszDst)], " %.3f ", pData[i] );
		//FormatFP( pData[i], &pszDst[strlen(pszDst)], 5, 3 );
		FormatFP( (double)(rand()%1000)/100.0, &pszDst[strlen(pszDst)], 5, 3 );
		pszDst[strlen(pszDst)+1] = 0;
		pszDst[strlen(pszDst)] = ' ';
	}
	*/
}

void CEnmPrinter::FormatBlankPos( MCHAR *pszDst, INT8U mode )
{
	INT8U i, j, aType;

//	strcpy( pszDst, "  Blank Position: " );
//	switch ( mode )
//	{
//	case ENM_REAGENT_MULTI:
//		for (  i = 0; i < ENM_REAGENT_LN_MAX; i++ )
//		{
//			for ( j = 0; j < ENM_REAGENT_COL_MAX; j++ )
//			{
//				if ( BIT( mOption.reagent[i], j ) )
//				{
//					sprintf( &pszDst[strlen(pszDst)], "%c%d " , 'A'+i, j+1 );
//				}
//			}
//		}
//		break;
//		
//	case ENM_REAGENT_COL_AVE:
//	case ENM_REAGENT_COL:
//		for ( j = 0; j < ENM_REAGENT_COL_MAX; j++ )
//		{
//			if ( BIT( mOption.reagent[0], j ) )
//			{
//				sprintf( &pszDst[strlen(pszDst)], "%d " , j+1 );
//			}
//		}
//		break;
//		
//	case ENM_REAGENT_LN_AVE :
//	case ENM_REAGENT_LN:
//		for (  i = 0; i < ENM_REAGENT_LN_MAX; i++ )
//		{
//			if ( BIT( mOption.reagent[i], 0 ) )
//			{
//				sprintf( &pszDst[strlen(pszDst)], "%d " , i+1 );
//			}
//		}
//		break;
//
//	default:
//		break;		
//	}
}

BOOLEAN CEnmPrinter::DetectPrinter()
{
	BOOLEAN aPrntExist = TRUE;

	/// temp code 
#ifdef ENM_VER_TARGET
	unsigned char ch;  
	int i = 0; 
	while ( PrntIsBusy() ) 
	{ 
		ch = PrntReadStatus();
		EnmDelay( 2 );
		if ( ++i > 100 )
		{
			aPrntExist = FALSE;
			break;
		}
	} 

#endif

	return aPrntExist;
}

void CEnmPrinter::SetPrinter()
{
	EnmPrntSetCharInterval( 0 );
	EnmPrntSetLineInterval( 0 );
}


void CEnmEmbedPrinter::PrintMainData( void )
{
	INT16U i, j;
	INT16U aLn, aSum;
	MCHAR *apTitle;
	MCHAR temp[90]= { 0 };
	MCHAR *apIndex[32] = { NULL };
	MCHAR aszLn[3] = { ' ' , ' ', '\0' };
	MCHAR aTitle[(12+3)*6 * 8*3] = { 0 };
	FP64 aVal[24];

	/// format header
	MCHAR  aszHeader[128];
	mpCurFormater->FormatDataIndex( ENM_DATA_GROUP, aszHeader );

	/// format data
	aSum = 0;
	apTitle = aTitle;
	for ( i = 0; i < ENM_DATA_COUNT; i++ )
	{
		//CEnmDataMng::PickDataLn( mstResultData, aVal, i );
		aLn = mpCurFormater->Format( i, aVal, ENM_DATA_GROUP, apTitle );

		for ( j = 0; j < aLn; j++ )
		{
			apIndex[aSum+j] = apTitle;
			apTitle += strlen(apTitle) + 1;
		}

		aSum += aLn;
	}

	/// print 	
	/// replace EnmPrntCRLF(), for some bug of embed printer
	EnmPrntOneLine( 2, " ", 1 );
	EnmPrntOneLine( 2, " ", 1 );
	//EnmPrntCRLF();
	//EnmPrntCRLF();

	INT16U aSegLen = strlen(apIndex[0]) / 3;
	for ( j = 0; j < 3; j++ )
	{
		/// print header
		strncpy( temp, &aszHeader[aSegLen*j], aSegLen );
		temp[aSegLen] = '\0';
		EnmPrntOneLine( mLeftAlign, temp, 1 );
		EnmPrntOneLine( 2, " ", 1 );
	//	EnmPrntCRLF();

		for ( i = 0; i < ENM_DATA_COUNT; i++ )
		{
			if ( aLn > 1 )	///< cutoff format is 3 lines
			{
				strncpy( temp, &apIndex[i*3][aSegLen*j], aSegLen );
				temp[aSegLen] = '\0';
				aszLn[0] = ' ';
				EnmPrntOneLine( mLeftAlign, aszLn, 0 );
				EnmPrntOneLine( mLeftAlign + strlen(aszLn), temp, 1 );

				aszLn[0] = 'A' + i;
				EnmPrntOneLine( mLeftAlign, aszLn, 0 );
				strncpy( temp, &apIndex[i*3+1][aSegLen*j], aSegLen );
				temp[aSegLen] = '\0';
				EnmPrntOneLine( mLeftAlign + strlen(aszLn), temp, 1 );
			
				strncpy( temp, &apIndex[i*3+2][aSegLen*j], aSegLen );
				temp[aSegLen] = '\0';
				aszLn[0] = ' ';
				EnmPrntOneLine( mLeftAlign, aszLn, 0 );
				EnmPrntOneLine( mLeftAlign + strlen(aszLn), temp, 1 );
			
				/// replace EnmPrntCRLF(), for some bug of embed printer
				//EnmPrntOneLine( 2, " ", 1 );
				//EnmPrntCRLF();
			}
			else
			{
				aszLn[0] = 'A' + i;
				EnmPrntOneLine( mLeftAlign, aszLn, 0 );
				strncpy( temp, &apIndex[i][aSegLen*j], aSegLen );
				temp[aSegLen] = '\0';
				EnmPrntOneLine( mLeftAlign + strlen(aszLn), temp, 1 );
			}
		}

		/// replace EnmPrntCRLF(), for some bug of embed printer
		EnmPrntOneLine( 2, " ", 1 );
		//EnmPrntCRLF();
	}
}

void CEnmEmbedPrinter::PrintAttachData( void )
{
	/// No attachData in E20
	
}

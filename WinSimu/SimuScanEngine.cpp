// SimuScanEngine.cpp: implementation of the CSimuScanEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "enmark.h"
#include "SimuScanEngine.h"
#include "EnmScanContext.h"
#include "MigEventMng.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSimuScanEngine::CSimuScanEngine()
{
	mCnt = 0;
}

BOOLEAN CSimuScanEngine::StartScan( CEnmScanContext	*pScanContext, BOOLEAN bForceExec )
{
	mCnt = 0;
	return CEnmScanEngine::StartScan( pScanContext, bForceExec );
}

enProcState CSimuScanEngine::ScanProc()
{
	if ( ++mCnt > 2 )
	{
		int i, j;
		tagScanData data;

		data.uColumnMask = mpScanContext->mProtocol.stMeasure.uColumnMask;

		FillSimuData( data );
	
		/// Get Scan Time
		SYSTEMTIME aTm;
		GetSystemTime( &aTm );
		CTime aTime( aTm );

		data.stDate.year  = aTime.GetYear();
		data.stDate.month = aTime.GetMonth();
		data.stDate.day   = aTime.GetDay();
		data.stTime.hour  = aTime.GetHour();
		data.stTime.min  = aTime.GetMinute();
		data.stTime.sec  = aTime.GetSecond();

		mpScanContext->SetScanData( data );
	
		StopScan();

		MigEventMng::Instance().PushMsg( ENM_MSG_SCAN_OK, (INT32U)GetAttchedPage() );
	
	
		return mState;
	}

	/// Set Simulate Scan data
	if ( mCnt == 1 )
	{
		tagTipInfo aInfo;
		aInfo.mTipID = STR_PPROGRAM_MEASURING;
		if ( GetAttchedPage() )
			GetAttchedPage()->ShowTips( aInfo );
	}

	return mState;
}

void CSimuScanEngine::FillSimuData( tagScanData& rData )
{
	static FP64 afBase[]  = { 100, 100, 100, 100, 100, 100, 100, 100 }; 
	static FP64 afBlank[] = { 20000, 20010, 20020, 20030, 20030, 20020, 20010, 20000 }; 

	static FP64 afData[12][8] = 
	{
		{ 15000, 15000, 15000, 15000, 15000, 15000, 15000, 15000 },
		{ 15000, 15000, 15000, 15000, 15000, 15000, 15000, 15000 },
		{ 15000, 15000, 15000, 15000, 15000, 15000, 15000, 15000 },
		{ 15000, 15000, 15000, 15000, 15000, 15000, 15000, 15000 },
		{ 15000, 15000, 15000, 15000, 15000, 15000, 15000, 15000 },
		{ 15000, 15000, 15000, 15000, 15000, 15000, 15000, 15000 },
		{ 15000, 15000, 15000, 15000, 15000, 15000, 15000, 15000 },
		{ 15000, 15000, 15000, 15000, 15000, 15000, 15000, 15000 },
		{ 15000, 15000, 15000, 15000, 15000, 15000, 15000, 15000 },
		{ 15000, 15000, 15000, 15000, 15000, 15000, 15000, 15000 },
		{ 15000, 15000, 15000, 15000, 15000, 15000, 15000, 15000 },
		{ 15000, 15000, 15000, 15000, 15000, 15000, 15000, 15000 },
	};	

	/// Fill data		
	for ( U16 i = 0; i < 8; i++ )
	{
		rData.dbMinVal[i] = afBase[i];
		rData.dbRefVal[i] = afBlank[i];
	}

	memcpy( rData.dbData, afData, sizeof(rData.dbData) );
}

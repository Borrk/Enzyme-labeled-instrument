// EnmFmWpMeasurePage.cpp: implementation of the CEnmFmWpMeasurePage class.
//
//////////////////////////////////////////////////////////////////////
#include <math.h>

#include "EnmFmWpMeasurePage.h"
#include "EnmSysCfgLib.h"
#include "EnmApp.hpp"
#include "EnmWfo.h"
#include "MStrMngr.h"

CEnmFmWpMeasurePage::CEnmFmWpMeasurePage()
: CEnmBasePage( "", PID_FM_WP_MEASURE )
{
	SetTitle( STR_PWHOLE_MEAS_TITLE );
	CreateControl();

	ShowFuncKey( TRUE );
	SetFuncKey( ENM_FUNCKEY_GO, FALSE, TRUE, STR_STEP_GO );
	SetFuncKey( ENM_FUNCKEY_EXIT, TRUE, TRUE, STR_STEP_ABORT_MEAS );
	SetFuncKey( ENM_FUNCKEY_PCOPY, FALSE, TRUE, STR_STEP_HCOPY );

	mbInScan = FALSE;
}

BOOLEAN	 CEnmFmWpMeasurePage::IsCanForward()
{
	return !mbInScan;
}

void CEnmFmWpMeasurePage::Draw()
{
	inherited::Draw();
	DrawHints();
}

void CEnmFmWpMeasurePage::DrawHints()
{
	MCHAR  aszHint[4][64] = { 0 };

	tagSystemCfg& rSysCfg = CEnmSysCfgLib::Instance().GetSysCfg();
	tagMeasure *apMeas = &GetScanContext().mProtocol.stMeasure;

	MCHAR *apszMainFilter  = rSysCfg.szFilter[apMeas->uMainFilter];
	MCHAR *apszReferFilter = rSysCfg.szFilter[apMeas->uReferFilter];

	U32 aColumnMask = apMeas->uColumnMask;
	U8 aColumns = 0;
	while ( aColumnMask & 0x01 )
	{
		aColumns++;
		aColumnMask >>= 1;
	}

	sprintf( aszHint[0], "%s %d %s", MLoadStr( STR_PWHOLE_MEAS_ON ), aColumns, MLoadStr( STR_PWHOLE_MEAS_COLUMN ) );
	sprintf( aszHint[1], "%s %s", MLoadStr( STR_PWHOLE_MEAS_WL ),  apszMainFilter );
	sprintf( aszHint[2], "%s %s", MLoadStr( STR_PWHOLE_REFER_WL ), apszReferFilter );
	sprintf( aszHint[3], "%s", MLoadStr( STR_PPROGRAM_MEASURING ) );

	/// draw
	U16 x = mLeftMargin + 20;
	U16 y = 80;
	U16 step = 20;

	for ( U16 i = 0; i < 3; i++ )
	{
		PutStr( x, y, aszHint[i], 0 );
		y += step;
	}

	PutStr( x + 40, y + 30, aszHint[3], 0 );
}

void CEnmFmWpMeasurePage::CreateControl()
{
//	tagCS aCS( ENM_CTRL_LABEL, 	mLeftMargin+20,	40+20*8,90,	20,  ENM_CTRL_NORMAL | ENM_CTRL_STATIC, 1,	0, STR_STEP_ABORT_MEAS );
//	aCS.hParent = this;
//	
//	AddControl( aCS );	
}

void CEnmFmWpMeasurePage::FocusOn()
{

	GetScanContext().mProtocol.stMeasure.uMeasMode = MEASURE_COMMON;
	CEnmApp& rApp = CEnmApp::Instance();
	if ( rApp.GetScanEngine() )
	{
		rApp.GetScanEngine()->Attach( *this );
		GetScanContext().SetFlag( SC_SCAN_FULL );
		rApp.GetScanEngine()->StartScan( &rApp.GetScanContext(), TRUE );

		mbInScan = TRUE;
	}

	inherited::FocusOn();
}

void CEnmFmWpMeasurePage::FocusOff()
{
	inherited::FocusOff();

	CEnmApp& rApp = CEnmApp::Instance();
	if ( rApp.GetScanEngine() )
	{
		rApp.GetScanEngine()->StopScan( );
		//GetScanContext().SetFlag( 0 );
	}

	mbInScan = FALSE;
}

void	CEnmFmWpMeasurePage::OnUserMsg( const tagMigMSG& stMsg )
{
	if ( stMsg.msg == ENM_MSG_SCAN_OK )
	{
		GetStep()->NextStep();
	}
}


CEnmFmWpReportPage::CEnmFmWpReportPage()
: CEnmBasePage( "", PID_FM_WP_MEASURE )
{
	SetTitle( STR_PWHOLE_REPORT_TITLE );

	ShowFuncKey( TRUE );

	Reset();
}

void CEnmFmWpReportPage::Reset()
{
	mCount = 0;
	mMean = 0; 
	mCV = 0;   
	mSD = 0;  
}

void CEnmFmWpReportPage::FocusOn()
{
	CEnmScanContext &rScanContext = GetScanContext();
	if ( rScanContext.IsConcCalculated() )
	{
		Evaluate();
	}
	else
		Reset();
	
	inherited::FocusOn();
}

BOOLEAN CEnmFmWpReportPage::Evaluate()
{
	CEnmScanContext &aScanProt = GetScanContext();
	
	BOOLEAN bRet = FALSE;

	FP64 fSum = 0;
	FP64 fConc;

	U16 aCount = 0;
	if ( aScanProt.IsConcCalculated() )
	{
		U16 i, j;
	
		U16 aColumns = aScanProt.GetScanColumns( TRUE );
		for ( j = 0; j < aColumns; j++ )
		{
			aCount = ENM_DATA_COUNT;
			for ( i = 0; i < aCount; i++ )
			{
				fSum += aScanProt.mCalcData.fConc[j][i];
			}
		}

		mCount = ENM_DATA_COUNT * aColumns;

		/// Calc mean value
		if ( mCount > 0 )
		{
			mMean = fSum / mCount;
			bRet = TRUE;
		}

		/// Calc SD value
		fSum = 0.0;
		for ( j = 0; j < aColumns; j++ )
		{
			aCount = ENM_DATA_COUNT;
			for ( i = 0; i < aCount; i++ )
			{
				fConc = mMean - aScanProt.mCalcData.fConc[j][i];

				fSum += (fConc * fConc);
			}
		}
		mSD = pow( fSum / (mCount - 1), 0.5 );

		/// Calc CV value
		mCV = mSD / mMean;
	}

	return bRet;
}

void CEnmFmWpReportPage::Draw()
{
	CEnmBasePage::Draw();
	DrawHints();
}

void   CEnmFmWpReportPage::FormatDate( tagDate& date, tagTime& time, MCHAR *pBuff, S32 langue )
{
	if ( langue == M_CHINESE )
	{
		sprintf( pBuff, "%4dÄê%02dÔÂ%02dÈÕ %02d:%02d:%02d", 
			    date.year, date.month, date.day, time.hour, time.min, time.sec );
	}
	else
	{
		MCHAR *szMonths[] = {"Jan", "Feb", "Mar", "Apr", "May", "June", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
		sprintf( pBuff, "%s %02d %4d  %02d:%02d:%02d", 
			    szMonths[date.month-1], date.day, date.year, time.hour, time.min, time.sec );
	}
}

void CEnmFmWpReportPage::DrawHints()
{
	tagScanData* apScanData = GetScanContext().GetScanData();
	if ( !apScanData )
		return;

	MCHAR  aszHint[10][100] = { 0 };

	tagSystemCfg& rSysCfg = CEnmSysCfgLib::Instance().GetSysCfg();
	tagMeasure *apMeas = &GetScanContext().mProtocol.stMeasure;

	MCHAR *apszMainFilter  = rSysCfg.szFilter[apMeas->uMainFilter];
	MCHAR *apszReferFilter = rSysCfg.szFilter[apMeas->uReferFilter];

	U32 aColumnMask = apMeas->uColumnMask;
	U8 aColumns = GetScanContext().GetScanColumns();
//	while ( aColumnMask & 0x01 )
//	{
//		aColumns++;
//		aColumnMask >>= 1;
//	}

	MCHAR aszDate[64];
	FormatDate( apScanData->stDate, apScanData->stTime, aszDate, MStrMngr::Instance().GetLangue() );
	
	/// Measure info
	sprintf( aszHint[0], "%s    %s", MLoadStr( STR_DATE ), aszDate );
	sprintf( aszHint[1], "%s %d", MLoadStr( STR_PWHOLE_REPORT_COLUMNS ), aColumns );
	sprintf( aszHint[2], "%s %s", MLoadStr( STR_PWHOLE_MEAS_WL ),  apszMainFilter );
	sprintf( aszHint[3], "%s %s", MLoadStr( STR_PWHOLE_REFER_WL ), apszReferFilter );

	/// Plate info
	U16  aSamples = aColumns; //GetScanContext().GetScanColumns();

	sprintf( aszHint[4], "%s", MLoadStr( STR_PWHOLE_REPORT_PLATE_INFO ) );
	sprintf( aszHint[5], "%8c%s %d", ' ',	 MLoadStr( STR_N_VALUE ), aSamples );

	FormatFP2( mMean, aszDate, 6, 3 );
	sprintf( aszHint[6], "%8c%s %s", ' ',	 MLoadStr( STR_MEAN_VALUE ), aszDate );

	FormatFP2( mCV, aszDate, 6, 3 );
	sprintf( aszHint[7], "%8c%s %s%%", ' ', MLoadStr( STR_CV_VALUE ), aszDate );

	FormatFP2( mSD, aszDate, 6, 3 );
	sprintf( aszHint[8], "%8c%s %s", ' ',  MLoadStr( STR_SD_VALUE ), aszDate );

	/// draw
	U16 x = mLeftMargin + 20;
	U16 y = 30;
	U16 step = 20;

	for ( U16 i = 0; i < 9; i++ )
	{
		PutStr( x, y, aszHint[i], 0 );
		y += step;
	}
}

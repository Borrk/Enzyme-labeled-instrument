// EnmFmWpMeasurePage.cpp: implementation of the CEnmFmWpMeasurePage class.
//
//////////////////////////////////////////////////////////////////////
#include "EnmFmCmMeasPage.h"
#include "EnmSysCfgLib.h"
#include "EnmConcDataUtil.h"
#include "EnmScanEngine.h"
#include "EnmApp.hpp"
#include "EnmWfo.h"

CEnmFmCmMeasPage::CEnmFmCmMeasPage()
: CEnmBasePage( "", PID_FM_CM_MEASURE )
{
	SetTitle( STR_PCOLUMN_MONITOR_TITLE );

	mbInScan = FALSE;
	memset( mConcData, 0, sizeof(mConcData) );
}

void CEnmFmCmMeasPage::Draw()
{
	CEnmBasePage::Draw();
	DrawHints();
}

void FormatAbsorb( FP64 val, MCHAR *pDst, U8 m, U8 n )
{
	FormatFP( val, pDst, m, n ); 
	U16 aLen;
	MCHAR *ap = strstr( pDst, "." );
	if ( ap )
	{
		ap++;
		aLen = strlen( ap );
		while ( aLen < n )
		{
			*(ap+aLen) = '0';
			*(ap+aLen+1) = '\0';
			aLen = strlen( ap );
		}
	
		ap = strstr( pDst, "." );
		ap++;
		if ( strlen(ap) > n )
		{
			*(ap+n) = '\0';
		}
	}
}

void CEnmFmCmMeasPage::DrawHints()
{
	MCHAR  aszHint[6][100] = { 0 };
	MCHAR  aszTemp[32] = {0};

	U8   aColumn = GetColumn( GetScanContext().mProtocol.stMeasure.uColumnMask );
	if ( aColumn > 12 )
		aColumn = 0;

	U16   aTotalTime = 20;
	tagMeasure* apMeas = &GetScanContext().mProtocol.stMeasure;
	MCHAR *apszWL = CEnmSysCfgLib::Instance().GetSysCfg().szFilter[apMeas->uMainFilter];
	
	/// Measure info
	sprintf( aszHint[0], "%s %d, %s %s", MLoadStr(STR_PCOLUMN_MONITOR_COLUMN), aColumn, MLoadStr(STR_PWHOLE_MEAS_WL), apszWL );
	sprintf( aszHint[1], "%s %d", MLoadStr( STR_PCOLUMN_MONITOR_TIME ), aTotalTime );
	sprintf( aszHint[2], "%s", MLoadStr( STR_PCOLUMN_MONITOR_ABSORB ) );

	/// Format Absorbance
	U16 i; 
	MCHAR *ap = aszHint[3];
	for ( i = 0; i < 4; i++ )
	{
		sprintf( ap, "%2c%c", ' ', i + 'A' );
		ap += 3;
		FormatAbsorb( mConcData[i], ap, 6, 3 );
		ap = aszHint[3] + strlen( aszHint[3] );
	}

	ap = aszHint[4];
	for ( i = 4; i < 8; i++ )
	{
		sprintf( ap, "%2c%c", ' ', i + 'A' );
		ap += 3;
		FormatAbsorb( mConcData[i], ap, 6, 3 );
		ap = aszHint[4] + strlen( aszHint[4] );
	}

	/// draw
	U16 x = mLeftMargin + 10;
	U16 y = 60;
	U16 step = 20;

	for ( i = 0; i < 5; i++ )
	{
		PutStr( x, y, aszHint[i], 0 );
		y += step;
	}
}

void	CEnmFmCmMeasPage::OnUserMsg( const tagMigMSG& stMsg )
{
	if ( stMsg.msg == ENM_MSG_SCAN_OK )
	{
		CalcConc();
		Update();
	}
}

BOOLEAN  CEnmFmCmMeasPage::OnForward()
{
	BOOLEAN bRet = FALSE;

	StopScan( );

	IWorkStep *apStep = GetStep();
	if ( apStep )
	{
		if ( apStep->GetParent() )
		{
			bRet = ((CEnmWfo*)apStep->GetParent())->PreStep();
		}
	}

	return bRet;
}

BOOLEAN  CEnmFmCmMeasPage::OnExit()
{
	BOOLEAN bRet = FALSE;

	if ( mbInScan  )
	{
		StopScan();
	}

	return inherited::OnExit();
}

void  CEnmFmCmMeasPage::FocusOn()
{
	inherited::FocusOn();

	StartupScan( !mbInScan );
}

U8	 CEnmFmCmMeasPage::GetColumn( U32 columnMask )
{
	U32  aColumnMask = columnMask;
	U8 aCol = 0;
	while ( aCol < 12 )
	{
		aCol++;
		if ( aColumnMask & 0x01 )
			break;

		aColumnMask >>= 1;
	}

	return aCol;
}


void CEnmFmCmMeasPage::CalcConc()
{
	CEnmScanContext &aScanProt = GetScanContext();	
	tagScanData* apData = aScanProt.GetScanData();
	if ( !apData )
		return;

	FP64 fConc, fBase, fBlank;
	U8 aCol = GetColumn( aScanProt.mProtocol.stMeasure.uColumnMask );
	if ( aCol > 12 || aCol <= 0 )
		return;

	aCol--;
	
	for ( U16 i = 0; i < ENM_REAGENT_LN_MAX; i++ )
	{
		if ( CEnmConcDataUtil::PickConcData( *apData, i + aCol * ENM_REAGENT_LN_MAX, fConc, fBase, fBlank ) )
		{
		//	fConc = CEnmConcDataUtil::CalcConc( fConc, fBase, fBlank, 3 );
		}
		else
		{
			fConc = 0;
		}
	
		mConcData[i] = fConc;
	}
}

void CEnmFmCmMeasPage::StartupScan( BOOLEAN bFirst )
{	
	GetScanContext().mProtocol.stMeasure.uMeasMode = MEASURE_COMMON;
	CEnmApp& rApp = CEnmApp::Instance();
	if ( rApp.GetScanEngine() )
	{
		rApp.GetScanEngine()->Attach( *this );
		GetScanContext().SetFlag( SC_SCAN_CM );
		rApp.GetScanEngine()->StartScan( &rApp.GetScanContext(), TRUE );
	}

	if ( bFirst )
	{
		mbInScan = TRUE;
		memset( mConcData, 0, sizeof(mConcData) );
	}
}

void CEnmFmCmMeasPage::StopScan( )
{	
	CEnmApp& rApp = CEnmApp::Instance();
	if ( rApp.GetScanEngine() )
	{
		rApp.GetScanEngine()->StopScan( FALSE );
	}

	mbInScan = TRUE;
	memset( mConcData, 0, sizeof(mConcData) );
}
// EnmScanEngine.cpp: implementation of the CEnmScanEngine class.
//
//////////////////////////////////////////////////////////////////////
#include "EnmScanEngine.h"
#include "EnmScanContext.h"
#include "MigEventMng.h"
#include "EnmPlateMotor.h"
#include "EnmFilterMotor.h"

CEnmScanEngine::CEnmScanEngine()
{
	Init();
}

CEnmScanEngine::~CEnmScanEngine()
{
	if ( SCAN_OFF != GetScanState() )
		StopScan();
}

void  CEnmScanEngine::Init()
{
	mpScanContext = NULL;
	mScanState    = SCAN_OFF;
}

enProcState CEnmScanEngine::IEnterProc( ) 
{
	if ( PROC_IDLE == mState )
		return mState;

	U32	aEndTick;
	U32 at = mStartTick;
	S32 a = mElipse;
	if ( PROC_SUSPEND == mState )
	{
		aEndTick = EnmGetTick();

		if ( mElipse == -1 )
			return mState;
		else if ( (aEndTick - mStartTick) >= mElipse )
		{
			mState = PROC_BUSY;
			mElipse = -1;
		}
		else
			return mState;
	}

	/// Process scan operation
	ScanProc();

	return mState;
}

void CEnmScanEngine::SetStateHints( )
{
	if ( !GetAttchedPage() )
		return;

	/// now simply show "Measuring"
	tagTipInfo aTips;
	aTips.mTipID = STR_PPROGRAM_MEASURING;
	mpPage->ShowTips( aTips );
}

BOOLEAN  CEnmScanEngine::StartScan( CEnmScanContext	*pScanContext, BOOLEAN bForceExec )
{
	if ( SCAN_OFF != GetScanState() && !bForceExec )
		return FALSE;

	/// Stop Current scan
	if (  SCAN_OFF != GetScanState() )
	{
		StopScan();
	}

	/// Start new scan proc
	return StartNewScan( pScanContext );
}

void CEnmScanEngine::StopScan( BOOLEAN bReset )
{
	//mpScanContext->ClearScanData();
	mpScanContext->Release();

	/// Stop Device
	if ( bReset )
		PostScan();
	
	if ( GetAttchedPage() )
	{
		tagTipInfo aTipInfo;
		aTipInfo.mTipID = -1;
		GetAttchedPage()->ShowTips( aTipInfo );
		mpPage = NULL;
	}

	mScanState = SCAN_OFF;
	mState = PROC_IDLE;
}

BOOLEAN CEnmScanEngine::StartNewScan( CEnmScanContext *pScanContext )
{
	if ( !pScanContext )
		return FALSE;

	SetStateHints();
	
	mpScanContext = pScanContext;

	mpScanContext->Use( TRUE );
	mpScanContext->ClearScanData();

	mScanState = SCAN_ON;
	mState = PROC_BUSY;

	mCurPlatePos = -1;

	return TRUE;
}


enProcState CEnmScanEngine::ScanProc()
{
	switch ( mScanState )
	{
	case SCAN_ON:
		PreScan();
		break;	

	case SCAN_WARM:
		WarmPlate();
		break;	

	case SCAN_SHAKE:
		ShakePlate();
		break;	
	
	case SCAN_READ_BASENOISE:
		ReadBaseNoise();
		break;

	case SCAN_SET_FILTER:
		SetFilter();
		break;
		
	case SCAN_READ_ABSORB:
		ReadAbsorbency();
		break;
		
	case SCAN_READ:
		ReadConc();
		break;	
	
	case SCAN_DONE:
		PostScan();
		break;
		
	default:
		break;			
	}
	
	return mState;
}

void CEnmScanEngine::PreScan()
{
#if 1
	CEnmPlateMotor& aPlateMotor = CEnmPlateMotor::Instance();
	CEnmFilterMotor& aFilterMotor = CEnmFilterMotor::Instance();
	aPlateMotor.IMotorOn( TRUE );
	aPlateMotor.IReset( FALSE );

    aFilterMotor.IMotorOn( TRUE );
    aFilterMotor.IReset( TRUE );

    LightEnable( TRUE );

#else
	/// Motor power on
	MoterEnable( true );
	/// Light On
	LightEnable( true );

	/// set plate & Filter to start position
	EnmPlateReset( 0 );
	EnmFilterReset( 0 );

#endif	
	SetScanDataPool( DATA_MAIN );
	
	mScanState = SCAN_WARM;
}

U16 CEnmScanEngine::WarmPlate()
{
	if ( mpScanContext->mProtocol.stMeasure.uTempratureCtrl == 0 )
	{
		mScanState = SCAN_SHAKE;
		return 1;
	}

	/// Start Warmming, till to the specified temprature
	//  Warm_StartWarm();

	FP64	aCurTemp //= GetDevTemprature(); 
					   = 37.0; 
	if ( aCurTemp >= mpScanContext->mProtocol.stMeasure.fTemprature )
	{
		/// Stop Warm
		// Warm_StopWarm();

		mScanState = SCAN_SHAKE;
	}

	return 1;
}

U16 CEnmScanEngine::ShakePlate()
{
	if ( mpScanContext->mProtocol.stMeasure.uMixingMode != MIX_NONE )
	{
		CEnmPlateMotor::Instance().IMotorOn( TRUE );

		U16  aTm = mpScanContext->mProtocol.stMeasure.uMixingTime;
		nShakePlate( mpScanContext->mProtocol.stMeasure.uMixingMode, aTm );
		
		CEnmPlateMotor::Instance().IMotorOn( FALSE );
	}

	mScanState = SCAN_READ_BASENOISE;	
	return 1;
}

U16 CEnmScanEngine::ReadBaseNoise()
{
	U16 i, j;
	ENMADC  aBlankData[ENM_REAGENT_LN_MAX] = { 0 };
	tagScanData *apScanData = mpCurDataPool;

//	CEnmPlateMotor::Instance().IMotorOn( FALSE );
//	CEnmFilterMotor::Instance().IMotorOn( FALSE );

	for (  i = 0; i < ENM_REAGENT_LN_MAX; i++ )
	{
		apScanData->dbMinVal[i] = 0;
	}

	for (  i = 0; i < 10; i++ )
	{
		EnmSampling( (ENMADC*)aBlankData );
		for ( j = 0; j < ENM_REAGENT_LN_MAX; j++ )
			apScanData->dbMinVal[j] += aBlankData[j]; 
	}

	for (  i = 0; i < ENM_REAGENT_LN_MAX; i++ )
	{
		apScanData->dbMinVal[i] /= 10.0;
	}

	mScanState = SCAN_SET_FILTER;	
	return 1;
}

U16  CEnmScanEngine::SetFilter( )
{
	U8 aFilter = 0;
	tagMeasure *apMeas = &mpScanContext->mProtocol.stMeasure;
		
	if ( mDataType == DATA_MAIN )
	{
		aFilter = apMeas->uMainFilter;
	}
	else if ( mDataType == DATA_SLAVE )
	{
		aFilter = apMeas->uSlaveFilter;		
	}
	else if ( mDataType == DATA_REF )
	{
		aFilter = apMeas->uReferFilter;				
	}				

   	CEnmFilterMotor& aFilterMotor = CEnmFilterMotor::Instance();
	aFilterMotor.IMotorOn( TRUE );
    aFilterMotor.IToHole( aFilter );
	aFilterMotor.IMotorOn( FALSE );
	EnmDelay( 1000 );

	mScanState = SCAN_READ_ABSORB;
	return 1;
}

U16 CEnmScanEngine::ReadConc()
{
	CEnmPlateMotor& aPlateMotor = CEnmPlateMotor::Instance();

	ENMADC  aSamplingData[ENM_DATA_GROUP][ENM_DATA_COUNT] = { 0 };
	tagMeasure *apMeas = &mpScanContext->mProtocol.stMeasure;
				
	SamplingData( (ENMADC*)aSamplingData, apMeas->uColumnMask );
	
	/// Save Data
	tagScanData *aScanData = mpCurDataPool;
	aScanData->uColumnMask = mpScanContext->mProtocol.stMeasure.uColumnMask;
	
	for ( int i = 0; i < ENM_DATA_GROUP; i++ )
	{
		for ( int j = 0; j < ENM_DATA_COUNT; j++ )
	 		aScanData->dbData[i][j] = aSamplingData[i][j];	 
	}
	 
	if ( mDataType == DATA_MAIN )
	{
		/// Get Scan Time
		tagDate aDate;
		tagTime aTime;
		
		aDate = EnmGetDate();
		aTime = EnmGetTime();
		
		aScanData->stDate = aDate;
		aScanData->stTime = aTime;

		#ifndef ENM_VER_TARGET
			FillSimuData( *aScanData );
		#endif

		mpScanContext->SetScanData( *aScanData );		
	}

	/// set next step
	U32 aScanMode = mpScanContext->GetFlag();
	if ( (aScanMode & SC_SCAN_FULL) )
	{
		MigEventMng::Instance().PushMsg( ENM_MSG_SCAN_OK, (INT32U)GetAttchedPage() );
		mScanState = SCAN_DONE;	
	}
	else if ( (aScanMode & SC_SCAN_CM) || (aScanMode & SC_SCAN_KINETIC) )
	{
		MigEventMng::Instance().PushMsg( ENM_MSG_SCAN_OK, (INT32U)GetAttchedPage() );
		mScanState = SCAN_READ;
		mState = PROC_SUSPEND;
		mElipse = mpScanContext->mProtocol.stMeasure.uPeriod;
	}
	else if ( aScanMode & SC_SCAN_MAINTENANCE )
	{
		MigEventMng::Instance().PushMsg( ENM_MSG_SCAN_OK, (INT32U)GetAttchedPage() );
		mScanState = SCAN_READ;
		mState = PROC_SUSPEND;
		mElipse = mpScanContext->mProtocol.stMeasure.uPeriod;
	}
	else if ( aScanMode & SC_SCAN_NORMAL )
	{
		switch ( mDataType )
		{
		case DATA_MAIN:
			SetScanDataPool( DATA_SLAVE );
			mScanState = SCAN_SET_FILTER;
			break;

		case DATA_SLAVE:
			SetScanDataPool( DATA_REF );
			mScanState = SCAN_SET_FILTER;
			break;

		case DATA_REF:
		default:
			MigEventMng::Instance().PushMsg( ENM_MSG_SCAN_OK, (INT32U)GetAttchedPage() );
			mScanState = SCAN_DONE;	
		}
	}

	/// Log Current Tick count
	if ( PROC_SUSPEND == mState && mElipse > 0 )
	{
		mStartTick = EnmGetTick();
	}

	return 1;	
}

U16 CEnmScanEngine::PostScan()
{
#if 1
	CEnmPlateMotor& aPlateMotor = CEnmPlateMotor::Instance();
   	CEnmFilterMotor& aFilterMotor = CEnmFilterMotor::Instance();

    aPlateMotor.IReset( TRUE );
	aPlateMotor.IMotorOn( FALSE );
    aPlateMotor.IReset( FALSE );

	/// Motor power off
	aFilterMotor.IMotorOn( TRUE );
    aFilterMotor.IReset( FALSE );
	aFilterMotor.IMotorOn( FALSE );

   	LightEnable( FALSE );

#else
	EnmPlateReset( 1 );
	EnmFilterReset( 0 );

	/// Motor power off
	MoterEnable( FALSE );
	LightEnable( FALSE );
#endif
	
	mElipse = -1;
	
	mScanState = SCAN_DONE;
	mState = PROC_IDLE;
	
	return 1;
}

U16	CEnmScanEngine::LightEnable( BOOLEAN bEnable )
{
		// TBA
		return 1;
}

U16	CEnmScanEngine::MoterEnable( BOOLEAN bEnable )
{
	char aEnable = bEnable ? 1 : 0;
	Plate_Motor_On( aEnable );
	Light_Motor_On( aEnable );
	
	return 1;
}

void	  CEnmScanEngine::SetScanDataPool( U16 type )
{
//	if ( mDataType == type )
//		return;
		
	mDataType = type;
	if ( mDataType == DATA_MAIN )
	{
		mpCurDataPool = mpScanContext->GetScanData( TRUE );
	}
	else if ( mDataType == DATA_SLAVE )
	{
		mpCurDataPool = mpScanContext->GetSlaveScanData( 0 );		
	}
	else if ( mDataType == DATA_REF )
	{
		mpCurDataPool = mpScanContext->GetSlaveScanData( 1 );				
	}				
}

void	CEnmScanEngine::SampleOnce( ENMADC *pData )
{
	if ( !pData )
		return;
		
	/// Sampling one column data
	//EnmPlateForwardOne();
	//EnmDelay( 10 );
	EnmSampling( (ENMADC*)pData );	///< sampling data	
}

void	CEnmScanEngine::SamplingData( ENMADC *pData, U32 uColumnMask )
{
	if ( !pData )
		return;
	
	U8 i;
	U32 aColumnMask = uColumnMask << (32 - ENM_DATA_GROUP);
	U8 aMaxPos = ENM_DATA_GROUP;
	for ( i = 0; i < ENM_DATA_GROUP; i++ )
	{
		if ( aColumnMask & 0x80000000)
		{
			break;
		}
		aMaxPos--;
		aColumnMask <<= 1;
	}

	CEnmPlateMotor& aPlateMotor = CEnmPlateMotor::Instance();
	aColumnMask = uColumnMask;

	aPlateMotor.IMotorOn( TRUE );

	for ( i = 0; i < aMaxPos; i++ )
	{
		if ( aColumnMask & 0x01)
		{
//			aPlateMotor.IMotorOn( TRUE );
			aPlateMotor.IToHole( i );
//			aPlateMotor.IMotorOn( FALSE );
			mCurPlatePos = i;
			EnmSampling( (pData + i * ENM_DATA_COUNT) );
		}

		aColumnMask >>= 1;
	}

	aPlateMotor.IMotorOn( TRUE );
}

U16   CEnmScanEngine::ReadAbsorbency() 
{
	INT8U i, j;	

   	/// Move plate berfore the laser 
	CEnmPlateMotor::Instance().IMotorOn( TRUE );
    CEnmPlateMotor::Instance().IToHead();
//	CEnmPlateMotor::Instance().IMotorOn( FALSE );

    ENMADC  aBlankData[ENM_REAGENT_LN_MAX] = { 0 };    
    tagScanData *apScanData = mpCurDataPool;
	for (  i = 0; i < ENM_REAGENT_COL_MAX; i++ )
	{
		apScanData->dbRefVal[i] = 0;
	}

	U8 aCnt = 4;
	for (  i = 0; i < aCnt; i++ )
	{
		EnmSampling( (ENMADC*)aBlankData );
		for ( j = 0; j < ENM_REAGENT_LN_MAX; j++ )
			apScanData->dbRefVal[j] += aBlankData[j]; 
	}

	for (  i = 0; i < ENM_REAGENT_COL_MAX; i++ )
	{
		apScanData->dbRefVal[i] /= aCnt;
	}	

	CEnmPlateMotor::Instance().IMotorOn( FALSE );
	mScanState = SCAN_READ;
	return 1;
}


void CEnmScanEngine::nShakePlate( INT8S speed, U16 sec )
{

	U16 i, j;

	MCHAR   aszTitle[64];
	U16		aStepTm	= speed * 400 + 1;
	U32		aStart = EnmGetTick();
	U32		aElipse = 0;

    CEnmPlateMotor& aPlateMotor = CEnmPlateMotor::Instance();
	do 
	{
		for ( j = 0; j < 1; j++ )
		{
			aPlateMotor.IFwOne();
			EnmDelay( aStepTm );
		}
		for ( j = 0; j < 1; j++ )
		{
			aPlateMotor.IBwOne( );
			EnmDelay( aStepTm );
		}

		aElipse = EnmGetTick() - aStart;
	}while ( aElipse < sec );    
}

#ifndef ENM_VER_TARGET
void CEnmScanEngine::FillSimuData( tagScanData& rData )
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
#endif
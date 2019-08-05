#include "EnmSysCfgLib.h"
#include "EnmFs.h"

CEnmSysCfgLib::CEnmSysCfgLib()
{
	mbSysCfgChanged = FALSE;
	mbDevOptChanged = FALSE;
}

CEnmSysCfgLib::~CEnmSysCfgLib()
{
}

CEnmSysCfgLib& CEnmSysCfgLib::Instance()
{
	static CEnmSysCfgLib aInst;
	return aInst;
}

void  CEnmSysCfgLib::Init()
{
	mfSysCfg = NF_INVALID_FILE_ID;
	mfDevOpt = NF_INVALID_FILE_ID;
	
	mbSysCfgChanged = TRUE;
	mbDevOptChanged = TRUE;

	SetDefaultSysCfg( mSysCfg );
	SetDefaultDevOpt( mDevOpt );
}

void CEnmSysCfgLib::SetDefaultSysCfg( tagSystemCfg& cfg )
{
	char *aWave[] = 
	{
		"405",  "414",  "450",  "492" , 
		"546",  "570",  "630",  "690"
	};

	/// wave length setting
	U16 i;
	for ( i = 0; i < FILTER_MAX; i++ )
	{
		strcpy( cfg.szFilter[i], aWave[i] );
	}

	/// default time
	cfg.stDate.year  = 2006;
	cfg.stDate.month = 6;
	cfg.stDate.day   = 1;
	cfg.stTime.hour  = 00;
	cfg.stTime.min   = 00;
	cfg.stTime.sec   = 00;

	cfg.uLanguage = M_CHINESE;
	
	cfg.bPlateCheck = FALSE;

	/// Printer
	cfg.stPrinter.type		 = ENM_PRINTER_INTERNAL;
	cfg.stPrinter.bAutoPrint = 0;
	cfg.stPrinter.mode		 = EMN_PRINTER_MODE_MR;

	strcpy( cfg.szDeviceName, " " );

	cfg.stSerialCfg.uBandRate = 4800;
	cfg.stSerialCfg.uCheck	  = 1;
	cfg.stSerialCfg.uDataBit  = 8;
	cfg.stSerialCfg.uStopBit  = 1;
}

void CEnmSysCfgLib::SetDefaultDevOpt( tagDeviceOption& opt )
{
//	opt.stMotorOpt.stPlateInfo.fPreDist  = 54.0;	///<  192.0 - .fHoleDist;
//	opt.stMotorOpt.stPlateInfo.fPostDist = 18.0;		///<  distance after the sample plate
//	opt.stMotorOpt.stPlateInfo.fStepDist = 0.18;	///<  motor mm/step
//	opt.stMotorOpt.stPlateInfo.fHoleDist = 9.0;		///<  distance between 2 hole.
//
//	opt.stMotorOpt.stLightInfo.fPreDist  = -1.5;
//	opt.stMotorOpt.stLightInfo.fPostDist = 0.5;
//	opt.stMotorOpt.stLightInfo.fStepDist = 0.18;
//	opt.stMotorOpt.stLightInfo.fHoleDist = 10.0;

	tagMotorInfo aPlateMotorInfo  = { 33.0, 140.0, 0.18, 9.0, 30.0, 12, MOTOR_PLATE };
	tagMotorInfo aFilterMotorInfo = { 0.0, 80.0, 0.18, 10.0, 0.0, 8, MOTOR_PLATE };

	opt.stMotorOpt.stPlateInfo = aPlateMotorInfo;	
	opt.stMotorOpt.stLightInfo = aFilterMotorInfo;

	opt.stMotorOpt.fPlateMotorDist = 0.0;
	opt.stMotorOpt.fLightMotorDist = 0.0;
}

BOOLEAN	CEnmSysCfgLib::Load(void)
{
	return LoadSysCfg() && LoadDevOpt();
}

void  CEnmSysCfgLib::Unload()
{
	UnloadSysCfg();
	UnloadDevOpt();
}

BOOLEAN		CEnmSysCfgLib::LoadSysCfg()
{
	U16  aFileId;
	U32  aSize = sizeof(mSysCfg);

	SetDefaultSysCfg( mSysCfg );

	aFileId = efOpen( "E20.sys", EF_OPEN_WRITE | EF_OPEN_READ );

	if ( NF_INVALID_FILE_ID == aFileId )
	{
		aFileId = efOpen( "E20.sys", EF_OPEN_WRITE | EF_OPEN_CREATE );
	
		efWrite( (U8*)&mSysCfg, aSize, aFileId );
		efClose( aFileId );
	
		aFileId = efOpen( "E20.sys", EF_OPEN_WRITE | EF_OPEN_READ );
	}

	BOOLEAN bValid = TRUE;
	if ( aSize != efGetSize( aFileId ) )
	{
		bValid = FALSE;
	}
	
	if ( bValid )
	{
		if( aSize != efRead( (U8*)&mSysCfg, aSize, aFileId ) )
			bValid = FALSE;
	}

	if ( !bValid )
	{
		efSeek( aFileId, 0, ESEEK_SET );
		efWrite( (U8*)&mSysCfg, aSize, aFileId );
	}
	
	mfSysCfg = aFileId;

	return TRUE;
}

BOOLEAN		CEnmSysCfgLib::LoadDevOpt()
{
	U16  aFileId;
	U32  aSize = sizeof(mDevOpt);

	SetDefaultDevOpt( mDevOpt );

	aFileId = efOpen( "E20.dev", EF_OPEN_WRITE | EF_OPEN_READ );

	if ( NF_INVALID_FILE_ID == aFileId )
	{
		aFileId = efOpen( "E20.dev", EF_OPEN_WRITE | EF_OPEN_CREATE );
	
		efWrite( (U8*)&mDevOpt, aSize, aFileId );
		efClose( aFileId );
	
		aFileId = efOpen( "E20.dev", EF_OPEN_WRITE | EF_OPEN_READ );
	}

	BOOLEAN bValid = TRUE;
	if ( aSize != efGetSize( aFileId ) )
	{
		bValid = FALSE;
	}
	
	if ( bValid )
	{
		if( aSize != efRead( (U8*)&mDevOpt, aSize, aFileId ) )
			bValid = FALSE;
	}

	if ( !bValid )
	{
		efSeek( aFileId, 0, ESEEK_SET );
		efWrite( (U8*)&mDevOpt, aSize, aFileId );
	}
	
	mfDevOpt = aFileId;

	return TRUE;
}

void  CEnmSysCfgLib::UnloadSysCfg()
{
	if ( NF_INVALID_FILE_ID == mfSysCfg )
		return;

	SyncSysCfg();
	efClose( mfSysCfg );
	mfSysCfg = NF_INVALID_FILE_ID;
}

void  CEnmSysCfgLib::UnloadDevOpt()
{
	if ( NF_INVALID_FILE_ID == mfDevOpt )
		return;

	SyncDevOpt();
	efClose( mfDevOpt );
	mfDevOpt = NF_INVALID_FILE_ID;
}

void CEnmSysCfgLib::SyncSysCfg()
{
	efSeek( mfSysCfg, 0, ESEEK_SET );
	efWrite( (U8*)&mSysCfg, sizeof(mSysCfg), mfSysCfg );
	efFlush( mfSysCfg );
}

void CEnmSysCfgLib::SyncDevOpt()
{
	efSeek( mfDevOpt, 0, ESEEK_SET );
	efWrite( (U8*)&mDevOpt, sizeof(mDevOpt), mfDevOpt );
	efFlush( mfDevOpt );
}

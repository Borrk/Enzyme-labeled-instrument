#include "EnmCfgServiceTestPage.h"
#include "EnmSysCfgLib.h"
#include "EnmScanEngine.h"
#include "EnmApp.hpp"
#include "EnmPlateMotor.h"
#include "EnmFilterMotor.h"


CEnmCfgServiceTestPage::CEnmCfgServiceTestPage()
: inherited( "", PID_CFG_FILTER )
{
	CreateControls();
}

CEnmCfgServiceTestPage::~CEnmCfgServiceTestPage()
{
}

void CEnmCfgServiceTestPage::CreateControls( void)
{
	U16 aLeft = mLeftMargin + 20;
	
	U16 aLabel[] = 
	{
		STR_PCFG_SERVICETEST_LAMP,
		STR_PCFG_SERVICETEST_FILTER,
		STR_PCFG_SERVICETEST_COLUMN,
		STR_PCFG_SERVICETEST_AVERAGING,
	};
	
	tagCS aCS[] = 
	{
		tagCS( ENM_CTRL_LABEL, 		aLeft,		mTopMargin+20*1,	180, 20,  ENM_CTRL_NORMAL | ENM_CTRL_STATIC, 1,0, aLabel[0] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft,		mTopMargin+20*2,	180, 20,  ENM_CTRL_NORMAL | ENM_CTRL_STATIC, 1,0, aLabel[1] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft,		mTopMargin+20*3,	180, 20,  ENM_CTRL_NORMAL | ENM_CTRL_STATIC, 1,0, aLabel[2] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft,		mTopMargin+20*4,	180, 20,  ENM_CTRL_NORMAL | ENM_CTRL_STATIC, 1,0, aLabel[3] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft,		mTopMargin+20*6,	240, 20,  ENM_CTRL_NORMAL | ENM_CTRL_STATIC, 1,0, MIG_STR_ID_INVALID ),             
	};

	tagCS aECS[] = 
	{
		tagCS( ENM_CTRL_EDITBOX, 	aLeft+200,	mTopMargin+20*1,	30, 20,  ENM_CTRL_NORMAL, 2,0, STR_BLANK ),             
		tagCS( ENM_CTRL_EDITBOX, 	aLeft+200,	mTopMargin+20*2,	30, 20,  ENM_CTRL_NORMAL, 2,0, STR_BLANK ),             
		tagCS( ENM_CTRL_EDITBOX, 	aLeft+200,	mTopMargin+20*3,	30, 20,  ENM_CTRL_NORMAL, 2,0, STR_BLANK ),             
		tagCS( ENM_CTRL_EDITBOX,	aLeft+200,	mTopMargin+20*4,	30, 20,  ENM_CTRL_NORMAL, 2,0, STR_BLANK ),             
	};
	
	U16 i;
	for ( i = 0; i < sizeof(aCS) / sizeof(aCS[0]); i++ )
	{
		aCS[i].hParent = this;
		mpConc = AddControl( aCS[i] );
	}
	//mpConc->SetTitle( "156 181 195 196 236 227 251 260 " );
	
	tagEditBoxCS ecs;
	ecs.minVal = 1;
	ecs.maxVal = 8;
	ecs.iVal   = 1;
	ecs.titleWidth = 0;
	ecs.type = ENM_EB_TYPE_INT;

	mpLamp = new MigEditBox( aECS[0], ecs );
	AddControl ( mpLamp );

	ecs.minVal = 1;
	ecs.maxVal = 8;
	ecs.iVal   = 1;
	mpFilter = new MigEditBox( aECS[1], ecs );
	AddControl ( mpFilter );

	ecs.minVal = 1;
	ecs.maxVal = 8;
	ecs.iVal   = 1;
	mpColumn = new MigEditBox( aECS[2], ecs );
	AddControl ( mpColumn );
	
	ecs.minVal = 1;
	ecs.maxVal = 255;
	ecs.iVal   = 1;
	mpAverage = new MigEditBox( aECS[3], ecs );
	AddControl ( mpAverage );
}

BOOLEAN   CEnmCfgServiceTestPage::OnExit()
{
	BOOLEAN bChanged = FALSE;

	tagSystemCfg& rCfg = CEnmSysCfgLib::Instance().GetSysCfg();

	CEnmApp& rApp = CEnmApp::Instance();
	if ( rApp.GetScanEngine() )
	{
		rApp.GetScanEngine()->StopScan( );
	}
		
	return inherited::OnExit();
}

void   CEnmCfgServiceTestPage::FocusOn()
{
	SetMaintenanceEnv();
	UpdateConcValue();
	
	tagSystemCfg& rCfg = CEnmSysCfgLib::Instance().GetSysCfg();

	GetScanContext().mProtocol.stMeasure.uMeasMode = MEASURE_COMMON;
	CEnmApp& rApp = CEnmApp::Instance();
	if ( rApp.GetScanEngine() )
	{
		rApp.GetScanEngine()->Attach( *this );
		GetScanContext().SetFlag( SC_SCAN_MAINTENANCE );
		rApp.GetScanEngine()->StartScan( &rApp.GetScanContext(), TRUE );
	}

	inherited::FocusOn();
}

void	CEnmCfgServiceTestPage::OnUserMsg( const tagMigMSG& stMsg )
{
	if ( stMsg.msg == ENM_MSG_SCAN_OK )
	{
		/// Update conc value
		UpdateConcValue();
	}
}

void  CEnmCfgServiceTestPage::OnKeyOK()
{
	inherited::OnKeyOK();

	if ( GetCurCtrl()->IsInEdit() )
		return;
	
	CEnmScanContext &rScanContext = GetScanContext();
	
	if ( GetCurCtrl() == mpLamp )
	{
		/// Set lamp intensity
		/// TBA
	}
	else if ( GetCurCtrl() == mpFilter )
	{
		if ( rScanContext.mProtocol.stMeasure.uMainFilter != mpFilter->GetInt() )
		{
			rScanContext.mProtocol.stMeasure.uMainFilter = mpFilter->GetInt();
			CEnmFilterMotor::Instance().IToHole( rScanContext.mProtocol.stMeasure.uMainFilter );
		}
	}
	else if ( GetCurCtrl() == mpColumn )
	{
		U8 aColumns = mpColumn->GetInt();
		if ( aColumns > 0 )
			aColumns--;
		rScanContext.mProtocol.stMeasure.uColumnMask = (U32)1 << aColumns;
		CEnmPlateMotor::Instance().IToHole( aColumns );
	}
	else if ( GetCurCtrl() == mpAverage )
	{
		rScanContext.mProtocol.stMeasure.uPeriod = mpAverage->GetInt();
	}

}

void  CEnmCfgServiceTestPage::OnKeyLeft()
{
	CEnmPlateMotor::Instance().IMotorFW();
	
	//inherited::OnKeyLeft();
}

void  CEnmCfgServiceTestPage::OnKeyRight()
{
	CEnmPlateMotor::Instance().IMotorBW();

	//inherited::OnKeyRight();
}

void  CEnmCfgServiceTestPage::SetMaintenanceEnv()
{
	mpLamp->SetInt( 1 );
	mpFilter->SetInt( 1 );
	mpColumn->SetInt( 1 );
	mpAverage->SetInt( 1 );

	CEnmScanContext &rScanContext = GetScanContext();
	rScanContext.mProtocol.stMeasure.uMainFilter = mpFilter->GetInt();
	rScanContext.mProtocol.stMeasure.uColumnMask = 1;
	rScanContext.mProtocol.stMeasure.uPeriod = mpAverage->GetInt();
}

void  CEnmCfgServiceTestPage::UpdateConcValue()
{
	CEnmScanContext &rScanContext = GetScanContext();

	U16 aCurCol = 0;
	if ( rScanContext.HasScanData() )
	{
		U32 aCol = rScanContext.mProtocol.stMeasure.uColumnMask;
		for ( U16 i = 0; i < ENM_DATA_GROUP; i++ )
		{
			if ( aCol & 0x01 )
			{
				aCurCol = i + 1;
				break;
			}

			aCol = aCol >> 1;
		}

		if ( aCol )
		{
/// TBA, should display the ADC value or conc value?			
//			MCHAR aszTemp[8];
//			MCHAR aszConc[128] = { 0 };
//			for ( U16 i = 0; i < ENM_DATA_COUNT; i++ )
//			{
//				FormatFP( rScanContext.mCalcData., aszTemp, 6, 2 );
//			}
//			mpConc->SetTitle( "156 181 195 196 236 227 251 260 " );
		}
	}

	if ( aCurCol == 0 )
		mpConc->SetTitle( "0.0  0.0  0.0  0.0  0.0  0.0  0.0  0.0" );

}


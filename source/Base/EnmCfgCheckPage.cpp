#include "EnmCfgCheckPage.h"
#include "EnmSysCfgLib.h"


CEnmCfgCheckPage::CEnmCfgCheckPage()
: inherited( "", PID_CFG_FILTER )
{
	CreateControls();
}

CEnmCfgCheckPage::~CEnmCfgCheckPage()
{

}

void CEnmCfgCheckPage::CreateControls( void)
{
	U16 aLeft = mLeftMargin + 10;
	
	U16 aLabel[] = 
	{
		STR_PCFG_CHECK_RUN,
		STR_PCFG_CHECK_POWER_ON,
		STR_PCFG_CHECK_CLEAR_TIME
	};
	
	tagCS aCS[] = 
	{
		tagCS( ENM_CTRL_LABEL, 		aLeft,		mTopMargin+20*2,	180, 20,  ENM_CTRL_NORMAL | ENM_CTRL_STATIC, 1,0, aLabel[0] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft,		mTopMargin+20*3,	180, 20,  ENM_CTRL_NORMAL | ENM_CTRL_STATIC, 1,0, aLabel[1] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft,		mTopMargin+20*4,	180, 20,  ENM_CTRL_NORMAL | ENM_CTRL_STATIC, 1,0, aLabel[2] ),             
	};

	tagCS aECS[] = 
	{
		tagCS( ENM_CTRL_EDITBOX, 	aLeft+230,	mTopMargin+20*2,	30, 20,  ENM_CTRL_NORMAL, 2,0, STR_BLANK ),             
		tagCS( ENM_CTRL_EDITBOX, 	aLeft+230,	mTopMargin+20*3,	30, 20,  ENM_CTRL_NORMAL, 2,0, STR_BLANK ),             
		tagCS( ENM_CTRL_UPDOWNBOX, 	aLeft+120,	mTopMargin+20*4,	30, 20,  ENM_CTRL_NORMAL, 2,0, STR_BLANK ),             
	};
	
	U16 i;
	for ( i = 0; i < sizeof(aCS) / sizeof(aCS[0]); i++ )
	{
		aCS[i].hParent = this;
		AddControl( aCS[i] );
	}
	
	tagEditBoxCS ecs;
	ecs.iVal   = 1;
	ecs.minVal = 1;
	ecs.maxVal = 24;
	ecs.titleWidth = 0;
	ecs.type = ENM_EB_TYPE_INT;

	mpRunTime = new MigEditBox( aECS[0], ecs );
	AddControl ( mpRunTime );

	ecs.iVal   = 1;
	ecs.minVal = 1;
	ecs.maxVal = 24;
	ecs.titleWidth = 0;
	ecs.type = ENM_EB_TYPE_INT;

	mpPowerOnTime = new MigEditBox( aECS[1], ecs );
	AddControl ( mpPowerOnTime );


	U16 aClearTime[] = 
	{
		STR_N, STR_Y
	};

	mpClearTime = new MigUpDn( aECS[2] );
	mpClearTime->SetItems( aClearTime, 2 );
	AddControl( mpClearTime );

}

BOOLEAN   CEnmCfgCheckPage::OnExit()
{
	BOOLEAN bChanged = FALSE;

	tagSystemCfg& rCfg = CEnmSysCfgLib::Instance().GetSysCfg();

//	if ( mpRunTime->GetInt() != rCfg.stPrinter.type )
//	{
//		rCfg.stPrinter.type = mpRunTime->GetInt();
//		bChanged = TRUE;
//	}
//
//	if ( mpClearTime->GetCurSel() != rCfg.stPrinter.bAutoPrint )
//	{
//		rCfg.stPrinter.bAutoPrint = mpClearTime->GetCurSel();
//		bChanged = TRUE;
//	}
//
//	if ( mpPrintMode->GetCurSel() != rCfg.stPrinter.mode )
//	{
//		rCfg.stPrinter.mode = mpPrintMode->GetCurSel();
//		bChanged = TRUE;
//	}
		
	return inherited::OnExit();
}

void   CEnmCfgCheckPage::FocusOn()
{
	tagSystemCfg& rCfg = CEnmSysCfgLib::Instance().GetSysCfg();

//	mpPrinter->SetCurSel( rCfg.stPrinter.type );
//	mpAutoPrint->SetCurSel( rCfg.stPrinter.bAutoPrint );
//	mpPrintMode->SetCurSel( rCfg.stPrinter.mode );
	
	inherited::FocusOn();
}
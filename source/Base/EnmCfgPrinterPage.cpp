#include "EnmCfgPrinterPage.h"
#include "EnmSysCfgLib.h"


CEnmCfgPrinterPage::CEnmCfgPrinterPage()
: inherited( "", PID_CFG_FILTER )
{
	CreateControls();
}

CEnmCfgPrinterPage::~CEnmCfgPrinterPage()
{

}

void CEnmCfgPrinterPage::CreateControls( void)
{
	U16 aLeft = mLeftMargin + 20;
	
	U16 aLabel[] = 
	{
		STR_PCFG_PRINTER_TYPE,
		STR_PCFG_PRINTER_AUTOPRINT,
		STR_PCFG_PRINTER_MODE
	};
	
	tagCS aCS[] = 
	{
		tagCS( ENM_CTRL_LABEL, 		aLeft,		mTopMargin+20*2,	120, 20,  ENM_CTRL_NORMAL | ENM_CTRL_STATIC, 1,0, aLabel[0] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft,		mTopMargin+20*3,	120, 20,  ENM_CTRL_NORMAL | ENM_CTRL_STATIC, 1,0, aLabel[1] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft,		mTopMargin+20*4,	120, 20,  ENM_CTRL_NORMAL | ENM_CTRL_STATIC, 1,0, aLabel[2] ),             
	};

	tagCS aUDCS[] = 
	{
		tagCS( ENM_CTRL_UPDOWNBOX, 	aLeft+150,	mTopMargin+20*2,	100, 20,  ENM_CTRL_NORMAL, 2,0, STR_BLANK ),             
		tagCS( ENM_CTRL_UPDOWNBOX, 	aLeft+150,	mTopMargin+20*3,	100, 20,  ENM_CTRL_NORMAL, 2,0, STR_BLANK ),             
		tagCS( ENM_CTRL_UPDOWNBOX, 	aLeft+150,	mTopMargin+20*4,	100, 20,  ENM_CTRL_NORMAL, 2,0, STR_BLANK ),             
	};
	
	U16 i;
	for ( i = 0; i < sizeof(aCS) / sizeof(aCS[0]); i++ )
	{
		aCS[i].hParent = this;
		AddControl( aCS[i] );
	}
	
	U16 aPrinterSrc[] = 
	{
		STR_PCFG_PRINTER_INTERNAL,
		STR_PCFG_PRINTER_EXT
	};

	U16 aAutoPrnt[] = 
	{
		STR_N, STR_Y
	};

	U16 aPrintMode[] = 
	{
		STR_PCFG_PRINTER_MODE_STD, 
		STR_PCFG_PRINTER_MODE_STD
	};
	mpPrinter = new MigUpDn( aUDCS[0] );
	mpPrinter->SetItems( aPrinterSrc, 2 );
	AddControl( mpPrinter );

	mpAutoPrint = new MigUpDn( aUDCS[1] );
	mpAutoPrint->SetItems( aAutoPrnt, 2 );
	AddControl( mpAutoPrint );


	mpPrintMode = new MigUpDn( aUDCS[2] );
	mpPrintMode->SetItems( aPrintMode, sizeof(aPrintMode)/sizeof(aPrintMode[0]) );
	AddControl( mpPrintMode );
}

void   CEnmCfgPrinterPage::FocusOff()
{
	BOOLEAN bChanged = FALSE;

	tagSystemCfg& rCfg = CEnmSysCfgLib::Instance().GetSysCfg();

	if ( mpPrinter->GetCurSel() != rCfg.stPrinter.type )
	{
		rCfg.stPrinter.type = mpPrinter->GetCurSel();
		bChanged = TRUE;
	}

	if ( mpAutoPrint->GetCurSel() != rCfg.stPrinter.bAutoPrint )
	{
		rCfg.stPrinter.bAutoPrint = mpAutoPrint->GetCurSel();
		bChanged = TRUE;
	}

	if ( mpPrintMode->GetCurSel() != rCfg.stPrinter.mode )
	{
		rCfg.stPrinter.mode = mpPrintMode->GetCurSel();
		bChanged = TRUE;
	}
		
	if ( bChanged )
	{
		NotifySysCfgChange( CEnmSysCfgLib::CFG_FILTER );
	}

	inherited::FocusOff();
}

void   CEnmCfgPrinterPage::FocusOn()
{
	tagSystemCfg& rCfg = CEnmSysCfgLib::Instance().GetSysCfg();

	mpPrinter->SetCurSel( rCfg.stPrinter.type );
	mpAutoPrint->SetCurSel( rCfg.stPrinter.bAutoPrint );
	mpPrintMode->SetCurSel( rCfg.stPrinter.mode );
	
	inherited::FocusOn();
}
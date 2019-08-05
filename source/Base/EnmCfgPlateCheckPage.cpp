#include "EnmCfgPlateCheckPage.h"
#include "EnmSysCfgLib.h"


CEnmCfgPlateCheckPage::CEnmCfgPlateCheckPage()
: inherited( "", PID_CFG_FILTER )
{
	CreateControls();
}

CEnmCfgPlateCheckPage::~CEnmCfgPlateCheckPage()
{

}

void CEnmCfgPlateCheckPage::CreateControls( void)
{
	U16 aLeft = mLeftMargin + 20;

	tagCS aCS[] = 
	{
		tagCS( ENM_CTRL_LABEL, 		aLeft,		mTopMargin+20*2,	150, 20,  ENM_CTRL_NORMAL | ENM_CTRL_STATIC, 1,0, STR_PCFG_PLATECHECK ),             
		tagCS( ENM_CTRL_UPDOWNBOX, 	aLeft+190,	mTopMargin+20*2,	100, 20,  ENM_CTRL_NORMAL, 2,0, STR_BLANK ),             
	};
	
	aCS[1].hParent = this;
	AddControl( aCS[0] );
	
	U16 aLin[] = 
	{
		STR_Y, STR_N
	};

	mpCheck = new MigUpDn( aCS[1] );
	mpCheck->SetItems( aLin, 2 );
	AddControl( mpCheck );
}

void   CEnmCfgPlateCheckPage::FocusOn()
{
	tagSystemCfg& rCfg = CEnmSysCfgLib::Instance().GetSysCfg();

	mpCheck->SetCurSel( rCfg.bPlateCheck ? 0 : 1 );

	inherited::FocusOn();
}

void   CEnmCfgPlateCheckPage::FocusOff()
{
	tagSystemCfg& rCfg = CEnmSysCfgLib::Instance().GetSysCfg();
	
	BOOLEAN bChanged = FALSE;
	BOOLEAN abCheck = mpCheck->GetCurSel() == 0 ? TRUE : FALSE;
	if ( abCheck != rCfg.bPlateCheck )
	{
		rCfg.bPlateCheck = abCheck;
		bChanged = TRUE;
	}
		
	if ( bChanged )
	{
		NotifySysCfgChange( CEnmSysCfgLib::CFG_FILTER );
	}

	inherited::FocusOff();
}

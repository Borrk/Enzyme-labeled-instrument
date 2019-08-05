#include "EnmCfgCommuPage.h"
#include "EnmSysCfgLib.h"


CEnmCfgCommuPage::CEnmCfgCommuPage()
: inherited( "", PID_CFG_FILTER )
{
	CreateControls();
}

CEnmCfgCommuPage::~CEnmCfgCommuPage()
{

}

void CEnmCfgCommuPage::CreateControls( void)
{
	U16 aLeft = mLeftMargin + 20;

	tagCS aCS[] = 
	{
		tagCS( ENM_CTRL_LABEL, 		aLeft,		mTopMargin+20*2,	60, 20,  ENM_CTRL_NORMAL | ENM_CTRL_STATIC, 1,0, STR_PCFG_COMMU_BAUD ),             
		tagCS( ENM_CTRL_UPDOWNBOX, 	aLeft+70,	mTopMargin+20*2,	100, 20,  ENM_CTRL_NORMAL, 2,0, STR_BLANK ),             
	};
	
	aCS[1].hParent = this;
	AddControl( aCS[0] );
	
	MCHAR *aszBaud[] = 
	{
		"4800",
		"9600",
		"19200"
	};
	mpBaud = new MigUpDn( aCS[1] );
	mpBaud->SetItems( aszBaud, sizeof(aszBaud) / sizeof(aszBaud[0]) );
	AddControl( mpBaud );
}

//BOOLEAN   CEnmCfgCommuPage::OnExit()
//{
//	BOOLEAN bChanged = FALSE;
//
//	tagSystemCfg& rCfg = CEnmSysCfgLib::Instance().GetSysCfg();
//
//	MCHAR *apItem =	mpBaud->GetItemStr( mpBaud->GetCurSel() );
//	if ( apItem )
//	{
//		U32 aBaud = atoi( apItem );
//		if ( aBaud != rCfg.stSerialCfg.uBandRate )
//		{
//			rCfg.stSerialCfg.uBandRate = aBaud;
//			bChanged = TRUE;
//		}
//	}
//
//	if ( bChanged )
//		NotifySysCfgChange( CEnmSysCfgLib::CFG_FILTER );
//
//	return inherited::OnExit();
//}

void   CEnmCfgCommuPage::FocusOn()
{
	tagSystemCfg& rCfg = CEnmSysCfgLib::Instance().GetSysCfg();

	U16 aCnt = mpBaud->GetTotalItem();
	for ( U16 i = 0; i < aCnt; i++ )
	{
		MCHAR *apItem =	mpBaud->GetItemStr( i );
		if ( apItem )
		{
			U32 aBaud = atoi( apItem );
			if ( aBaud == rCfg.stSerialCfg.uBandRate )
			{
				mpBaud->SetCurSel( i );
				break;
			}
		}
	}

	inherited::FocusOn();
}

void   CEnmCfgCommuPage::FocusOff()
{
	tagSystemCfg& rCfg = CEnmSysCfgLib::Instance().GetSysCfg();

	U16 aCurSel = mpBaud->GetCurSel();
	MCHAR *apItem =	mpBaud->GetItemStr( aCurSel );
	if ( apItem )
	{
		U32 aBaud = atoi( apItem );
		if ( aBaud != rCfg.stSerialCfg.uBandRate )
		{
			rCfg.stSerialCfg.uBandRate = aBaud;
			CEnmSysCfgLib::Instance().SysCfgChanged();
			NotifySysCfgChange( CEnmSysCfgLib::CFG_COM_BAUD );
		}
	}

	inherited::FocusOff();
}
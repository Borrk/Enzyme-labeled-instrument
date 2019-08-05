#include "EnmCfgTimePage.h"
#include "EnmSysCfgLib.h"


CEnmCfgTimePage::CEnmCfgTimePage()
: inherited( "", PID_CFG_TIME )
{
	CreateControls();
	mbChanged = FALSE;
}

void  CEnmCfgTimePage::CreateControls( void)
{
	U16 apItem[] = 
	{
		STR_DEV_DATE,
		STR_DEV_TIME,
		STR_DEV_LANGUAGE,
		STR_DEV_NAME
	};

	tagCS aCS[] = 
	{
		tagCS( ENM_CTRL_DATEBOX, 20,	80+20*0, 160,	20, ENM_CTRL_NORMAL, ENM_DATE_DATE,	 "", apItem[0] ),
		tagCS( ENM_CTRL_TIMEBOX, 20,	80+20*1, 160,	20, ENM_CTRL_NORMAL, ENM_DATE_TIME,	 "", apItem[1] ),
		tagCS( ENM_CTRL_LABEL,   20,	80+20*2, 80,	20, ENM_CTRL_NORMAL |ENM_CTRL_STATIC, ENM_DATE_LANGUAGE_TITLE,	 "", apItem[2] ),
		tagCS( ENM_CTRL_UPDOWNBOX, 100,	80+20*2, 120,	20, ENM_CTRL_NORMAL, ENM_DATE_LANGUAGE,	 "", STR_BLANK ),
		tagCS( ENM_CTRL_EDITBOX, 28,	80+20*3, 240,	20, ENM_CTRL_NORMAL, ENM_DATE_NAME,	 "", apItem[3] ),
	};
	
	aCS[0].hParent = this;
	mpDateBox = new CEnDateBox( aCS[0], 0 );
	AddControl( mpDateBox );

	aCS[1].hParent = this;
	mpTimeBox = new CEnTimeBox( aCS[1] );
	AddControl( mpTimeBox );

	U16 aLanguage[] = { STR_CHINESE, STR_ENGLISH };
	AddControl( aCS[2] );
	mpLanguage = new MigUpDn( aCS[3] );
	AddControl( mpLanguage );
	mpLanguage->SetItems( aLanguage, sizeof(aLanguage) / sizeof(aLanguage[0]) );
	
	/// Create Edit Box
	tagEditBoxCS aECS;
	aECS.maxChar  = 20;
	aECS.szVal[0] = '\0';
	aECS.type	  = ENM_EB_TYPE_CHAR;
	aECS.titleWidth = 80;//strlen(apItem[2]) * GetFontW();

	mpDevName = new MigEditBox( aCS[4], aECS );
	AddControl( mpDevName );
}

void	CEnmCfgTimePage::OnUserMsg( const tagMigMSG& stMsg )
{
	if ( stMsg.msg == ENM_MSG_TIMER )
	{
		if ( !mpDateBox->IsInEdit() )
		{
			tagDate  aDate = EnmGetDate();
			mpDateBox->SetDate( aDate );
		}

		if ( !mpTimeBox->IsInEdit() )
		{
			tagTime  aTime = EnmGetTime();
			mpTimeBox->SetTime( aTime );
		}	
	}
}

void	CEnmCfgTimePage::OnKeyOK( void )
{
	tagDate aDate;
	if (mpDateBox == mpCurCtrl && !mpDateBox->IsInEdit() )
	{
		aDate = mpDateBox->GetDate();
		EnmSetDate( aDate );
		aDate.day = 0;
		aDate = EnmGetDate(  );
		aDate.day =aDate.day;
	}

	aDate = EnmGetDate(  );

	if ( mpTimeBox == mpCurCtrl &&  !mpTimeBox->IsInEdit() )
	{
		tagTime aTime = mpTimeBox->GetTime();
		EnmSetTime( aTime );
	}

	/// Change language
	if ( mpLanguage == mpCurCtrl && !mpLanguage->IsInEdit() )
	{
		U16 aCurSel = mpLanguage->GetCurSel();
		aCurSel = aCurSel == 0 ? M_CHINESE : M_ENGLISH;
		if ( CEnmSysCfgLib::Instance().GetSysCfg().uLanguage != aCurSel )
		{
			CEnmSysCfgLib::Instance().GetSysCfg().uLanguage = aCurSel;
			mbChanged = TRUE;
			//NotifySysCfgChange( CEnmSysCfgLib::CFG_DEV_LANGUAGE );
		}
	}

	if ( mpDevName == mpCurCtrl && !mpDevName->IsInEdit() )
	{
		MCHAR* apOldName = (MCHAR*)CEnmSysCfgLib::Instance().GetSysCfg().szDeviceName;
		MCHAR* apName = mpDevName->GetStr();
		if ( strcmp( apOldName, apName ) != 0 )
		{
			strcpy( apOldName, apName );
			NotifySysCfgChange( CEnmSysCfgLib::CFG_DEV_NAME );
		}
	}
}

BOOLEAN  CEnmCfgTimePage::OnForward()
{	
	if ( mbChanged )
	{
		MStrMngr::Instance().SetLangue( CEnmSysCfgLib::Instance().GetSysCfg().uLanguage ? M_CHINESE : M_ENGLISH );
		NotifySysCfgChange( CEnmSysCfgLib::CFG_DEV_LANGUAGE );
		mbChanged = FALSE;
	}

	/// return to upper page
	return inherited::OnForward();
}

BOOLEAN  CEnmCfgTimePage::OnExit()
{	
	if ( mbChanged )
	{
		MStrMngr::Instance().SetLangue( CEnmSysCfgLib::Instance().GetSysCfg().uLanguage ? M_CHINESE : M_ENGLISH );
		NotifySysCfgChange( CEnmSysCfgLib::CFG_DEV_LANGUAGE );
		mbChanged = FALSE;
	}

	return inherited::OnExit();
}

void  CEnmCfgTimePage::SetParam( void )
{
	tagDate  aDate = EnmGetDate();
	mpDateBox->SetDate( aDate );

	tagTime  aTime = EnmGetTime();
	mpTimeBox->SetTime( aTime );

	mpLanguage->SetCurSel( CEnmSysCfgLib::Instance().GetSysCfg().uLanguage );
	MCHAR* apName = (MCHAR*)CEnmSysCfgLib::Instance().GetSysCfg().szDeviceName;
	mpDevName->SetStr( apName );
}

void	CEnmCfgTimePage::SetSysDate( tagDate date )
{
	EnmSetDate( date );
}

void	CEnmCfgTimePage::SetSysTime( tagTime time )
{
	EnmSetTime( time );
}

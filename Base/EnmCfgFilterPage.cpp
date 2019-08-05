#include "EnmCfgFilterPage.h"
#include "EnmSysCfgLib.h"


CEnmCfgFilterPage::CEnmCfgFilterPage()
: inherited( "", PID_CFG_FILTER )
{
	for ( U8 i = 0; i < FILTER_MAX; i++ )
		mpFilters[i] = NULL;

	CreateControls();
}

CEnmCfgFilterPage::~CEnmCfgFilterPage()
{

}

void CEnmCfgFilterPage::CreateControls( void)
{
	U16 aLeft = mLeftMargin + 20;
	U16 aStep = 20;

	tagCS aCS( ENM_CTRL_EDITBOX, aLeft,	mTopMargin,	240,	20,  ENM_CTRL_NORMAL, 1, "", STR_BLANK );

	tagEditBoxCS aECS;
	aECS.maxChar  = 4;
	aECS.szVal[0] = '\0';
	aECS.type	  = ENM_EB_TYPE_CHAR;
	aECS.titleWidth = 180;

	U16 aFilter = STR_FILTER_1;

	for ( U16 i = 0; i < FILTER_MAX; i++ )
	{
		aCS.y += aStep;
		aCS.titleID = aFilter + i;
		mpFilters[i] = new MigEditBox( aCS, aECS );
		AddControl( mpFilters[i] );
	}
}

BOOLEAN   CEnmCfgFilterPage::OnExit()
{
	BOOLEAN bChanged = FALSE;

	MCHAR *apNew;
	tagSystemCfg& rCfg = CEnmSysCfgLib::Instance().GetSysCfg();

	for ( U8 i = 0; i < FILTER_MAX; i++ )
	{
		apNew = mpFilters[i]->GetStr();
		if ( strcmp( apNew, rCfg.szFilter[i] ) != 0 )
		{
			strcpy( rCfg.szFilter[i], apNew );
			bChanged = TRUE;
		}
	}

	if ( bChanged )
		NotifySysCfgChange( CEnmSysCfgLib::CFG_FILTER );

	return inherited::OnExit();
}

void   CEnmCfgFilterPage::FocusOn()
{
	tagSystemCfg& rCfg = CEnmSysCfgLib::Instance().GetSysCfg();

	MCHAR *apFilter;
	for ( U8 i = 0; i < FILTER_MAX; i++ )
	{
		apFilter = ValidateFilterValue( rCfg.szFilter[i] );
		mpFilters[i]->SetStr( apFilter ? apFilter : (MCHAR*)"N/A" );
	}

	inherited::FocusOn();
}

void   CEnmCfgFilterPage::OnKeyOK()
{
	tagSystemCfg& rCfg = CEnmSysCfgLib::Instance().GetSysCfg();

	if ( !MigPage::GetCurCtrl()->IsInEdit() )
	{
		U8 j;
		MigControl *ap = GetCurCtrl();
		for ( j = 0; j < FILTER_MAX; j++ )
		{
			if ( ap == mpFilters[j] )
				break;
		}

		MCHAR *apFilter = ValidateFilterValue( mpFilters[j]->GetStr() );
		if ( apFilter )
		{
			strcpy( rCfg.szFilter[j], apFilter );
		}
		else
		{
			strcpy( rCfg.szFilter[j], "N/A" );
			mpFilters[j]->SetStr( rCfg.szFilter[j] );
		}

		NotifySysCfgChange( CEnmSysCfgLib::CFG_FILTER );
	}

	inherited::OnKeyOK();
}

MCHAR*	CEnmCfgFilterPage::ValidateFilterValue( MCHAR* pszFilter )
{
	if ( !pszFilter )
		return NULL;

	MCHAR ch;
	U16	  aLen = strlen( pszFilter );

	U16  aZeroCnt = 0;
	BOOLEAN bValid = TRUE;
	while ( aLen-- )
	{
		ch = pszFilter[aLen];
		if ( !('0' <= ch && ch <= '9') )
		{
			bValid = FALSE;
			break;
		}

		if ( '0' == ch )
			aZeroCnt++;
	}
	
	
	if ( !bValid || aZeroCnt == strlen(pszFilter) )
		bValid = FALSE;

	return bValid ? pszFilter : NULL;
}
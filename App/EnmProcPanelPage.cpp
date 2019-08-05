// EnmProcPanelPage.cpp: implementation of the CEnmProcPanelPage class.
//
//////////////////////////////////////////////////////////////////////

#include "EnmProcPanelPage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnmProcPanelPage::CEnmProcPanelPage()
:CEnmBasePage( "", ENM_PAGE_ID_MAIN )
{
	CreateControls();
	CreateChildPage();
}

CEnmProcPanelPage::~CEnmProcPanelPage()
{

}
	
void CEnmProcPanelPage::CreateControls( void)
{
	INT32U apItem[] = 
	{
		STR_PPROCESS_COMPUTE,
		STR_PPROCESS_EDIT,
		STR_PPROCESS_DELETE,
		STR_PPROCESS_SAVE
	}; 
	
	tagCS cs;
	cs.w = 160;
	cs.h = 20;
	cs.x = 20;
	cs.hParent = this;
	cs.y = 80;
	cs.dwStyle = ENM_OBJ_NORMAL;

	MigControl *apCtrl;
	for ( INT8U i = 0; i < sizeof(apItem)/sizeof(apItem[0]); i++ )
	{
		cs.id = ENM_PROCPANEL_START + i;
		cs.titleID = apItem[i];
		apCtrl = AddControl( cs );
		cs.y +=20;
	}
}

void CEnmProcPanelPage::CreateChildPage(void)
{
//	SetChild( &mModePage  );
//	SetChild( &mPlatePage );

	SetLinkRange( ENM_PROCPANEL_START,ENM_PROCPANEL_END );
}

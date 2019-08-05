// EnmFastModePage.cpp: implementation of the CEnmFastModePage class.
//
//////////////////////////////////////////////////////////////////////

#include "EnmFastModePage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnmFastModePage::CEnmFastModePage()
:CEnmBasePage( "", ENM_PAGE_ID_MAIN )
{
	SetTitle( STR_PFAST_TITLE );
	CreateControls();
	CreateChildPage();
}

CEnmFastModePage::~CEnmFastModePage()
{

}

void CEnmFastModePage::CreateControls( void)
{
	INT32U apItem[] = 
	{
		STR_PFAST_WHOLEPLATE,
		STR_PFAST_COLUMNMONITOR,
		STR_PFAST_MULTIFILTER,
		STR_PFAST_KINETIC,
	}; 
	
	tagCS cs;
	cs.w = 160;
	cs.h = 20;
	cs.x = 100;
	cs.hParent = this;
	cs.y = 80;
	cs.dwStyle = ENM_OBJ_NORMAL;

	MigControl *apCtrl;
	for ( INT8U i = 0; i < sizeof(apItem)/sizeof(apItem[0]); i++ )
	{
		cs.id = ENM_FAST_START + i;
		cs.titleID = apItem[i];
		apCtrl = AddControl( cs );
		cs.y +=20;
	}
}

void CEnmFastModePage::CreateChildPage(void)
{
//	SetChild( &mModePage  );
//	SetChild( &mPlatePage );

	SetLinkRange( ENM_FAST_START,ENM_FAST_END );
}

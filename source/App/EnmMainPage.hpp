#ifndef __ENK_MAIN_PAGE_H__
#define __ENK_MAIN_PAGE_H__

#include "EnmBasePage.hpp"

#include  "EnmModePage.h"

/// temp code
#include "EnmPlatePage.h"
#include "EnmProcPanelPage.h"
#include "EnmFastModePage.h"
#include "EnmConcListPage.h"
#include "EnmCutoffCompute.h"
/**
 *		自动多功能酶标仪 
 *      "1 Programmed Mode",
 *      "2 Fast Mode" 
 *      "3 Programming",    
 *      "4 Programmed Test List"
 *		"5 Reader Setup"
 *                       
 */ 

class CEnmMainPage : public CEnmBasePage
{
public:
	CEnmMainPage(MCHAR* pTitle = NULL)
	:CEnmBasePage( "自动多功能酶标仪", ENM_PAGE_ID_MAIN )
	{
		CreateControls();
		CreateChildPage();
	}
	
	CEnmMainPage(const tagCS& cs ): CEnmBasePage( cs ){}

	enum 
	{
		ENM_MAIN_START = 0, ENM_MAIN_MODE = ENM_MAIN_START, ENM_MAIN_FASTMODE, 
		ENM_MAIN_PROGRAMMING, ENM_MAIN_TESTLIST, ENM_MAIN_DMM, ENM_MAIN_SYSCFG, 
		ENM_MAIN_END
	};

protected:
	void CreateControls( void)
	{
		INT32U apItem[] = 
		{
			STR_MAIN_MODE,
			STR_MAIN_FAST_MODE,
			STR_MAIN_PROGRAMMING,
			STR_MAIN_PROGRAMLIST,
			STR_MAIN_DATA_LIST,
			STR_MAIN_SYS_SETUP,
			STR_MAIN_HOST_CONTROL
		}; 
		
		tagCS cs;
		cs.w = 160;
		cs.h = 20;
		cs.x = 20;
		cs.hParent = this;
		cs.y = 60;
		cs.dwStyle = ENM_OBJ_NORMAL;

		MigControl *apCtrl;
		for ( INT8U i = 0; i < sizeof(apItem)/sizeof(apItem[0]); i++ )
		{
			cs.id = ENM_MAIN_START + i;
			cs.titleID = apItem[i];
			apCtrl = AddControl( cs );
			cs.y +=20;
		}
	}
	
	void CreateChildPage(void)
	{
		SetChild( &mModePage  );
		SetChild( &mFastMode );
		SetChild( &mConcPage );
		SetChild( &mDataCompute );
		SetChild( &mPlatePage );
		SetChild( &mProcPage );
	
		SetLinkRange( ENM_MAIN_START, ENM_MAIN_END );
	}

void OnNumKey( INT16U key )
{
	INT16U aKey = NumKey2Index( GetKeyValue(key) );	
	if ( aKey < 5 )
		MigPage::OnNumKey( key );
	else
		MStrMngr::Instance().SetLangue( MStrMngr::Instance().GetLangue() == M_CHINESE ? M_ENGLISH : M_CHINESE );
}

// member variables
private:
	CEnmModePage	mModePage;

	/// temp code
	CEnmPlatePage		mPlatePage;
	CEnmProcPanelPage	mProcPage;
	CEnmFastModePage	mFastMode;
	CEnmConcListPage    mConcPage;
	CEnmCutoffCompute		mDataCompute;
};

#endif	// __ENK_MAIN_PAGE_H__

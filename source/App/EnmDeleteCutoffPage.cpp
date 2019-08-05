#include "EnmDeleteCutoffPage.h"
#include "EnmCutoffLib.h"
#include "EnmConcDataUtil.h"


CEnmDeleteCutoffPage::CEnmDeleteCutoffPage()
:CEnmBasePage( "", ENM_PAGE_ID_CUTOFF_DELETE )
{
	SetTitle( STR_PDELETECUTOFF_TITLE );
	CreateControls();
}

CEnmDeleteCutoffPage::~CEnmDeleteCutoffPage()
{
}

void CEnmDeleteCutoffPage::CreateControls( void)
{
	U16 aLeft = mLeftMargin + 20;

	tagCS aCS[] = 
	{
		tagCS( ENM_CTRL_LABEL, aLeft,	mTopMargin+20*3,	140,	20,  ENM_CTRL_STATIC | ENM_CTRL_NORMAL, 1, "", STR_PDELETECUTOFF_TIPS ),             
		//tagCS( ENM_CTRL_GRIDBOX, aLeft + 190,	mTopMargin+20*3, 140,	20,  ENM_CTRL_NORMAL, 2,	"", STR_BLANK ),             
	};

	aCS[0].hParent = this;
	mpDeleteTip = new MigControl( aCS[0] );
	AddControl( mpDeleteTip );

	/// Create New Cutoff UpDn control
	U16 aLin[] = 
	{
		STR_Y, STR_N
	};

//	mpSaveConfirm = new MigUpDn( aCS[1] );
//	AddControl( mpSaveConfirm );
//	mpSaveConfirm->SetItems( aLin, 2 );
}

void CEnmDeleteCutoffPage::FocusOn()
{
//	mpSaveConfirm->SetCurSel( 0 );
//	inherited::FocusOn();
//
//	/// temp
//	DeleteCutoff();
}

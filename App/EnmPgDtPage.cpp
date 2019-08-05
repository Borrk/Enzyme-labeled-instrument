// EnmPgDtPage.cpp: implementation of the CEnmPgDtPage class.
//
//////////////////////////////////////////////////////////////////////

#include "EnmPgDtPage.h"
#include "EnmModeLib.h"


CEnmPgDtPage::CEnmPgDtPage()
: CEnmBasePage( "", PID_PG_DELETE )
{
	SetTitle( STR_PPROGRAM_DEL_TITLE );
	CreateControl();

	/// temp
	mpPgName->SetTitle( "      " );
}

void CEnmPgDtPage::CreateControl()
{
	INT32U aItem[] = 
	{
		STR_PPROGRAM_DEL_PID,
		STR_PPROGRAM_DEL_DELALL,
	}; 
	
	U16 aLeft = mLeftMargin;
	INT32U aStyleStatic = ENM_CTRL_NORMAL | ENM_CTRL_STATIC ; 

	tagCS aCS[] = 
	{
		tagCS( ENM_CTRL_LABEL, 		aLeft,	mTopMargin+20*2,	160,20,  aStyleStatic, 1,	0, aItem[0] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft,	mTopMargin+20*4,	160,20,  aStyleStatic, 2,	"", MIG_STR_ID_INVALID ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft,	mTopMargin+20*6,	160,20,  aStyleStatic, 3,	0, aItem[1] ),             	
	
		tagCS( ENM_CTRL_UPDOWNBOX, 	aLeft+170,	mTopMargin+20*2,40, 20,  ENM_CTRL_NORMAL, 4,	0, STR_BLANK ),             
	};

	aCS[0].hParent = this;
	AddControl( aCS[0] );

	aCS[1].hParent = this;
	mpPgName = AddControl( aCS[1] );

	aCS[2].hParent = this;
	AddControl( aCS[2] );

	mpProgram = new MigUpDn( aCS[3] );
	AddControl( mpProgram );
}

void  CEnmPgDtPage::FocusOn()
{
	MCHAR aID[200][6] = { 0 };
	MCHAR *apID[200] = { NULL };
	for ( U16 i = 0; i < CEnmModeLib::Instance().GetCount(); i++ )
	{
		sprintf( aID[i], "%d", i + 1 );
		apID[i] = aID[i];
	}

	mpProgram->SetItems( (MCHAR**)&apID, CEnmModeLib::Instance().GetCount() );
	mpProgram->SetCurSel( 0 );
	
	
	inherited::FocusOn();
}

BOOLEAN  CEnmPgDtPage::OnForward()
{	
	/// Delete protocol
	U16 aCurSel = mpProgram->GetCurSel();

	tagProtocol aProt;
	if ( !CEnmModeLib::Instance().GetAt( aCurSel, aProt ) )
		return FALSE;

	ShowDeleteTips( aProt.stMeasure.szName );

	BOOLEAN bDeleted = CEnmModeLib::Instance().DeleteAt( aCurSel, TRUE );
	GetScanContext().Reset();

	U32 aStart = ::EnmGetTick();
	ShowDeleteTips( aProt.stMeasure.szName, FALSE );
	
#ifdef ENM_VER_TARGET
	while ( ::EnmGetTick() - aStart < 3 );
#else
	EnmDelay( 1000 );
#endif

	return inherited::OnExit();
}

void  CEnmPgDtPage::OnKeyUp()
{
	S16 aCurSel = -1;
	if ( GetCurCtrl() == mpProgram && mpProgram->IsInEdit() )
	{
		aCurSel = mpProgram->GetCurSel();
		SyncProtName( aCurSel );
	}
		
	inherited::OnKeyUp();
}

void  CEnmPgDtPage::OnKeyDown()
{
	S16 aCurSel = -1;
	if ( GetCurCtrl() == mpProgram && mpProgram->IsInEdit() )
	{
		aCurSel = mpProgram->GetCurSel();
		SyncProtName( aCurSel );
	}
	
	inherited::OnKeyDown();
}

void  CEnmPgDtPage::SyncProtName( S16 protId )
{
	if ( protId == -1 )
	{
		mpPgName->SetTitle( " " );
	}
	else
	{
		tagProtocol aProt;
		if ( CEnmModeLib::Instance().GetAt( protId, aProt) )
		{
			mpPgName->SetTitle( aProt.stMeasure.szName );
			mpPgName->Update();
		}
	}

}

void  CEnmPgDtPage::ShowDeleteTips( MCHAR *pName, BOOLEAN bPreDelete )
{
	if ( !pName )
		return;
	
	S32 x, y, w, h;
	x = mLeftMargin;
	y = mTopMargin ;
	w = mWidth - x ;
	h = mHeight - y;

	MCHAR aszTip[128] = {0};
	if ( bPreDelete )
	{
		MCHAR *apTipHead = MStrMngr::Instance().GetString( STR_PDELETE_TIP_DELETING );
		sprintf( aszTip, "%s %s ...", apTipHead, pName );
	}
	else
	{
		MCHAR *apTipHead = MStrMngr::Instance().GetString( STR_PDELETE_TIP_HEAD );
		MCHAR *apTipTail = MStrMngr::Instance().GetString( STR_PDELETE_TIP_DELETED );
		sprintf( aszTip, "%s %s %s", apTipHead, pName, apTipTail );
	}

	::PutBox( x, y, w, h );
	::PutStr( x + 10, y + 4, aszTip, 0 );
}
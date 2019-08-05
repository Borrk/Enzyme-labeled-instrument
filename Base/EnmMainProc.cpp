// EnmMainProc.cpp: implementation of the CEnmMainProc class.
//
//////////////////////////////////////////////////////////////////////

#include "EnmMainProc.h"
#include "MigEventMng.h"
#include "EnmApp.hpp"
#include "EnmBeeper.hpp"

#include "EnmWfoSelecter.h"
#include "EnmWfsFactory.h"
#include "EnmSysCfgLib.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnmMainProc::CEnmMainProc()
{
}

CEnmMainProc::~CEnmMainProc()
{
	delete mpWfRoot;
}

BOOLEAN  CEnmMainProc::InitEngine()
{
	CEnmWfsFactory a;
	mpWfRoot = a.CreateWfs();
	mpWfRoot->Activate( TRUE );

	return TRUE;
}

enProcState  CEnmMainProc::IResume( )
{
	inherited::IResume();

	if ( mState == PROC_BUSY )
	{
		mpWfRoot->Activate( TRUE );
	}

	return mState;
}

enProcState CEnmMainProc::IEnterProc( )
{
	if ( PROC_SUSPEND == mState )
		return mState;
	
	MigEventMng &aEventMng = MigEventMng::Instance();

	tagMigMSG *ap = aEventMng.GetMsg();
	while ( ap )
	{
		OnMessage( *ap );
		ap = aEventMng.GetMsg();
	}
	
	return mState;
}

void  CEnmMainProc::OnMessage( tagMigMSG& msg )
{
	IWorkStep* ap = IWorkStep::GetActiveStep();

	U32 aKey;	
	switch ( msg.msg )
	{
	case ENM_MSG_KEY:
		
		if ( !ap )
			return;

		aKey = GetKeyValue( msg.param1 );
		if ( aKey == KEY_GO )
		{
			if ( ap->IsCanForward() )
			{
				ap->OnForward();
			}
			else
			{
				CEnmBeeper::ParttenBeep( BEEP_ALARM );
			}
//			ap->NextStep();
		}
		else if ( aKey == KEY_ESC )
		{
			if ( ap->IsCanExit() )
			{
				ap->OnExit();
			}
			else
			{
				CEnmBeeper::ParttenBeep( BEEP_ALARM );
			}
//			ap->Return();
		}
		else
		{
			if( ap->GetPage() )
				ap->GetPage()->OnMessage( msg );
		}
		break;

	case ENM_MSG_CFG_CHG:
		OnSysCfgChange( msg );
		break;

	case ENM_MSG_SCAN_OK:
		if ( msg.param1 )
		{
			((MigPage*)msg.param1)->OnMessage( msg );
		}
		break;

	default:
		break;
	}

}

BOOLEAN	 CEnmMainProc::OnSysCfgChange( tagMigMSG& msg )
{
	if ( msg.param1 == CEnmSysCfgLib::CFG_DEV_LANGUAGE )
	{
		enMigLangue aCurSel = (enMigLangue)CEnmSysCfgLib::Instance().GetSysCfg().uLanguage;
		MStrMngr::Instance().SetLangue( aCurSel );
	}

	if ( CEnmSysCfgLib::Instance().IsSysCfgChanged() )
		CEnmSysCfgLib::Instance().SyncSysCfg();

	return TRUE;
}

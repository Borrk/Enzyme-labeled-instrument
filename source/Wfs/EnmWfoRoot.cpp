#include "EnmWfoRoot.h"
#include "MigEventMng.h"
#include "EnmApp.hpp"

CEnmWfoRoot::CEnmWfoRoot()
: CEnmWfoSelector(0)
{
}

BOOLEAN  CEnmWfoRoot::OnForward()
{

//	if ( mSelectOpt == 6 )
//	{
//		ChangeSysMode( 1 );
//		MigEventMng::Instance().PushMsg( ENM_MSG_MODE_CHANGE, ENM_MODE_PC, 0 );
//		return TRUE;
//	}

	BOOLEAN bCanForward = FALSE;
	IWorkStep *apWfo = GetKidAt( mSelectOpt );
	if ( apWfo )
	{
		bCanForward = apWfo->Activate( TRUE ); 
	}

	return bCanForward;
}

BOOLEAN CEnmWfoRoot::ChangeSysMode( int mode )
{
	return CEnmApp::Instance().ChangeMode( mode == 0 ? ENM_MODE_ALONE : ENM_MODE_PC );	
}
// EnmKeyEngine.cpp: implementation of the CEnmKeyEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "EnmKeyEngine.h"
#include "EnmApi.h"
#include "MigEventMng.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnmKeyEngine::CEnmKeyEngine()
{
	mState = PROC_BUSY;
}

CEnmKeyEngine::~CEnmKeyEngine()
{

}

enProcState CEnmKeyEngine::IEnterProc( )
{
	if ( mState == PROC_IDLE )
		return mState;
	else if ( mState == PROC_SUSPEND )
	{
		if ( mElipse == -1 )
			return mState;
		else
		{
			/// TBM
			if ( --mElipse <= 0 )
				mState = PROC_BUSY;
		}
	}

	INT16U KeyVal = (INT16U)-1;
	INT16U aCnt = 0;
	
	#ifdef ENM_VER_TARGET
		while ( aCnt++ < 4 )
		{
			KeyVal = Scan_KEY();
			if ( KeyVal != (INT16U)-1 )
				break;
		}
	#endif

	if ( KeyVal != (INT16U)-1 )
		MigEventMng::Instance().PushMsg( ENM_MSG_KEY, KeyVal, 0 );

	return PROC_BUSY;
}

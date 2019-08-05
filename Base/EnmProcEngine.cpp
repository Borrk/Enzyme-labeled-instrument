#include "EnmProcEngine.h"

IProcEngine::IProcEngine()
{
	Init();
}

IProcEngine::~IProcEngine()
{
}

void  IProcEngine::Init( )
{
	mState = PROC_IDLE;
	mpPage = NULL;
	mElipse = -1;
}

enProcState  IProcEngine::ISuspend( S32 timeout, BOOLEAN bForced )
{
	if ( mState == PROC_BUSY || bForced )
	{
		mState = PROC_SUSPEND;
		mElipse = timeout;
	}

	return mState;
}

enProcState  IProcEngine::IResume( )
{
	if ( mState == PROC_SUSPEND )
	{
		mState = PROC_BUSY;
		mElipse = -1;
	}

	return mState;
}

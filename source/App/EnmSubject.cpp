#include "EnmSubject.h"
#include "EnmObserver.h"

CEnmSubject::CEnmSubject()
{
	mObserverCnt = 0;
	for ( U16 i = 0; i < OBJ_MAX; i++ )
		mpObservers[i] = NULL;
}

CEnmSubject::~CEnmSubject()
{

}

BOOLEAN		CEnmSubject::Attach( CEnmObserver& observer )
{
	S16 aIdx = FindObserver( observer );
	if ( aIdx != -1 ) ///< exist
		return TRUE;

	if ( mObserverCnt < OBJ_MAX - 1 )
	{
		mpObservers[mObserverCnt] = &observer;
		mObserverCnt++;
		return TRUE;
	}
	
	return FALSE;
}

void		CEnmSubject::Dettach( CEnmObserver& observer )
{
	S16 aIdx = FindObserver( observer );
	if ( aIdx == -1 ) ///< not exist
		return ;
	
	if ( aIdx == mObserverCnt - 1 )
		mpObservers[aIdx] = NULL;

	for ( U16 i = aIdx; i < mObserverCnt - 2; i++ )
	{
		mpObservers[i] = mpObservers[i+1];
	}
	
	mObserverCnt--;	
}

void		CEnmSubject::Notify( CEnmNotify& reason )
{
	for ( U16 i = 0; i < mObserverCnt; i++ )
	{
		mpObservers[i]->Update( *this, reason );
	}
}

S16			CEnmSubject::FindObserver( CEnmObserver& observer ) const
{
	S16 aIdx = -1;
	for ( U16 i = 0; i < mObserverCnt; i++ )
	{
		if ( mpObservers[i] == &observer )
		{
			aIdx = i;
			break;
		}
	}

	return aIdx;
}

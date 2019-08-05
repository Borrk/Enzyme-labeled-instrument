// MigEventMng.cpp: implementation of the MigEventMng class.
//
//////////////////////////////////////////////////////////////////////

#include "MigEventMng.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MigEventMng& MigEventMng::Instance()
{
	static MigEventMng aInst;
	return aInst;
}

MigEventMng::MigEventMng()
{
	Init();
}

void  MigEventMng::Init()
{
	mFirst  = 0;
	mCount  = 0;
}

tagMigMSG*	MigEventMng::GetMsg()
{
	if ( mCount == 0 )
		return NULL;

	tagMigMSG *ap = &mMsgPool[mFirst];
	mFirst = (mFirst+1) % MIG_MSG_POOL_SIZE;
	mCount--;

	return ap;
}

BOOLEAN		MigEventMng::PushMsg( tagMigMSG& msg )
{
	if ( mCount >= MIG_MSG_POOL_SIZE )
		return FALSE;

	S16 aPos = (mFirst + mCount) % MIG_MSG_POOL_SIZE;
	mMsgPool[aPos] = msg;
	mCount++;

	return TRUE;
}

BOOLEAN		MigEventMng::PushMsg( INT32U msg, INT32U p1, INT32U p2 )
{
	if ( mCount >= MIG_MSG_POOL_SIZE )
		return FALSE;

	S16 aPos = (mFirst + mCount) % MIG_MSG_POOL_SIZE;
	mMsgPool[aPos].msg    = msg;
	mMsgPool[aPos].param1 = p1;
	mMsgPool[aPos].param2 = p2;
	mCount++;

	return TRUE;
}

void MigEventMng::ClearAll()
{
	mCount = mFirst = 0;
}

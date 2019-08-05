#include "CDtKid.h"

CDtKid::CDtKid()
: mKidCount(0)
{
	mKidMax = KID_MAX;
	for ( U16 i = 0; i < mKidMax; i++ )
		mKidList[i] = NULL;
}

CDtKid::~CDtKid()
{
	mKidCount = 0;
}

//##ModelId=441BBCA5009C
S16 CDtKid::AddKidAt(IWorkStep& pStep)
{
	if ( mKidCount < mKidMax )
	{
		mKidList[mKidCount] = &pStep;
		mKidCount++;
		return mKidCount - 1;
	}
	else
	{
		return -1;
	}
}

//##ModelId=441BBD2C005D
BOOLEAN CDtKid::RemoveKid(U16 idx)
{
	if ( idx >= mKidCount )
		return TRUE;

	memcpy( &mKidList[idx], &mKidList[idx+1], (mKidCount - idx) );	
	for ( U16 i = idx; i < mKidCount; i++ )
		mKidList[i] = mKidList[i+1];

	mKidList[mKidCount] =0;
	mKidCount--;

	return TRUE;
}

//##ModelId=441BBCA5009C
S16 CDtKid::SetKidAt(IWorkStep& pStep, U16 idx )
{
	if ( idx < mKidCount )
	{
		mKidList[idx] = &pStep;
	}

	return idx;
}

//##ModelId=441BBC9F0119
IWorkStep* CDtKid::GetKidAt(U16 idx) const
{
	if ( idx < mKidCount )
	{
		return mKidList[idx];
	}
	else
	{
		return NULL;
	}
}

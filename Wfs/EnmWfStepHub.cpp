#include "EnmWfStepHub.h"

CEnmWfStepHub::CEnmWfStepHub( IWorkStep* pParent )
: IWorkStep( pParent )
, mCurPath( 0 )
{
}

CEnmWfStepHub::~CEnmWfStepHub()
{
	for ( U16 i = 0; i < WFS_MAX; i++ )
	{
		if ( i == mCurPath )
			continue;

		/// delete other path's steps
		IWorkStep *apStep;
		for ( U16 j = 0; j < mPath[i].GetKidCount(); j++ )
		{
			apStep = mPath[i].GetKidAt( j );
			if ( !IsStepInParent( *apStep ) )
				delete apStep;
		}
	}
}

BOOLEAN	 CEnmWfStepHub::IsStepInParent( IWorkStep& rStep )
{
	BOOLEAN bIsIn = FALSE;

	for ( U16 i = GetParent()->GetKidCount() - 1; i > 0; i-- )
	{
		if ( &rStep == GetParent()->GetKidAt( i ) )
		{
			bIsIn = TRUE;
			break;
		}
	}

	return bIsIn;
}

BOOLEAN  CEnmWfStepHub::Switch( U16 uPath )
{
	if ( uPath >= WFS_MAX )
		return FALSE;

	if ( mPath[uPath].GetKidCount() <= 0 )
		return FALSE;

//	if ( uPath == mCurPath )
//		return TRUE;

	U16 aIdx = 0;
	IWorkStep *apStep = GetParent()->GetKidAt( 0 );
	while ( apStep != this && aIdx < GetParent()->GetKidCount() )
	{
		apStep = GetParent()->GetKidAt( ++aIdx );
	}

	/// remove the kids after this step
	U16 i;
	for ( i = GetParent()->GetKidCount() - 1; i > aIdx; i-- )
	{
		GetParent()->RemoveKid( i );
	}

	/// Add new path's steps
	for ( i = 0; i < mPath[uPath].GetKidCount(); i++ )
	{
		GetParent()->AddKidAt( *mPath[uPath].GetKidAt( i ) );
	}

	mCurPath = uPath;
	return TRUE;
}

BOOLEAN	 CEnmWfStepHub::AddStep( IWorkStep& rStep, U16 path, U16 pos )
{
	if (  path >= WFS_MAX )
		return FALSE;

	S16 aIdx = mPath[path].AddKidAt( rStep );

	return aIdx == -1 ? FALSE : TRUE;	
}

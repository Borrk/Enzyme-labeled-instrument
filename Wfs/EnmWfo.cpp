#include "EnmWfo.h"

CEnmWfo::CEnmWfo( IWorkStep* pStep )
: IWorkStep( pStep )
{
	mCurStep = 0;
}

BOOLEAN CEnmWfo::Activate(BOOLEAN bActive)
{
	BOOLEAN bRet = IWorkStep::Activate( bActive );

	if ( bRet && bActive )
		mCurStep = 0;
 
	if ( GetKidCount() > 0 )
		GetKidAt( mCurStep )->Activate( bActive );
	
	return bRet;
}

CEnmWfo::~CEnmWfo()
{
	IWorkStep *ap;
	S16 aCnt = GetKidCount()-1;
	for ( S16 i = aCnt; i >= 0; i-- )
	{
		if ( ap = GetKidAt( i ) )
			delete ap;
	}
}

BOOLEAN CEnmWfo::NextStep()
{
	if ( mCurStep >= GetKidCount() - 1 )
	{
		return FALSE;
	}

	if ( IsActive() )
	{
		Activate( FALSE );
	}

#if 0
	BOOLEAN bRet = nDeActiveStep( mCurStep );
	if ( bRet && GetKidAt( mCurStep+1 )->IsCanForward() )
	{
		bRet = nActiveStep( mCurStep+1 );
	}
	
	return bRet;
#else
	GetKidAt( mCurStep )->Activate( FALSE );
	GetKidAt( mCurStep+1 )->Activate( TRUE );
	mCurStep++;

	return TRUE;
#endif
}

//##ModelId=441BDBFA02EE
BOOLEAN CEnmWfo::PreStep()
{
	if ( mCurStep <=0 )
	{
		return FALSE;
	}

#if 0
	BOOLEAN bRet = nDeActiveStep( mCurStep );
	if ( bRet && GetKidAt( mCurStep-1 )->IsCanForward() )
	{
		bRet = nActiveStep( mCurStep-1 );
	}
	
	return bRet;
#else
	GetKidAt( mCurStep )->Activate( FALSE );
	GetKidAt( mCurStep-1 )->Activate( TRUE );
	mCurStep--;
	return TRUE;
#endif
}

BOOLEAN CEnmWfo::RequestReturn()
{
	if ( GetParent() )
	{
		Activate( FALSE );
		return GetParent()->RequestReturn( );
	}

	return FALSE;
}

BOOLEAN  CEnmWfo::IsCanForward()
{
	BOOLEAN bCanForward = FALSE;

	IWorkStep *apStep = GetKidAt( 0 );
	if ( apStep )
	{
		bCanForward = apStep->IsCanForward();
	}

	return bCanForward;
}

BOOLEAN  CEnmWfo::OnForward()
{
	BOOLEAN bRet = ActiveStep( 0, TRUE );
	
	return bRet;
}

BOOLEAN  CEnmWfo::IsCanExit()
{
	return GetParent() ? TRUE : FALSE;
}

BOOLEAN  CEnmWfo::OnExit()
{
	return RequestReturn();
}

BOOLEAN CEnmWfo::ActiveStep( U16 step, BOOLEAN bActive )
{
	return bActive ? nActiveStep( step ) : nDeActiveStep( step );
}

BOOLEAN CEnmWfo::nActiveStep( U16 step )
{
	BOOLEAN bRet = FALSE;
	IWorkStep *apStep = GetKidAt( step );

	if ( apStep )
	{
		bRet = apStep->IsCanForward();
	}

	if ( bRet )
	{
		if ( apStep->OnForward() )
		{
			bRet = apStep->Activate( TRUE );
			if ( apStep->IsActive() )
				mCurStep = step;
		}
	}

	return bRet;
}

BOOLEAN CEnmWfo::nDeActiveStep( U16 step )
{
	BOOLEAN bRet = FALSE;
	IWorkStep *apStep = GetKidAt( step );

	if ( apStep )
	{
		bRet = apStep->IsCanExit();
	}

	if ( bRet )
	{
		if ( apStep->OnExit() )
		{
			bRet = apStep->Activate( FALSE );
			//mCurStep = 0;
		}
	}

	return bRet;
}
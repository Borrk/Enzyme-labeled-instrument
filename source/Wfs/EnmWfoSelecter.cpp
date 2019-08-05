#include "EnmWfoSelecter.h"
#include "EnmWfo.h"
#include "EnmWfSelectPage.h"

CEnmWfoSelector::CEnmWfoSelector( IWorkStep* pParent )
: IWorkStep( pParent )
, mSelectOpt ( 0 )
{
}

CEnmWfoSelector::~CEnmWfoSelector()
{
	CEnmWfo *ap;
	S16 aCnt = GetKidCount()-1;
	for ( S16 i = aCnt; i >= 0; i-- )
	{
		if ( ap = (CEnmWfo*)GetKidAt( i ) )
			delete ap;
	}
}

BOOLEAN CEnmWfoSelector::Activate(BOOLEAN bActive)
{
	BOOLEAN bRet = IWorkStep::Activate( bActive );

	if ( bRet )
		SyncState();

	return bRet;
}

BOOLEAN CEnmWfoSelector::RequestReturn( )
{
	Activate( TRUE );
	
	return TRUE;
}

BOOLEAN CEnmWfoSelector::NextStep()
{
	if ( !IsActive() )
		return FALSE;

	if ( GetKidCount() <= 0 )
		return FALSE;

	IWorkStep *apWfo = NULL;
	Activate( FALSE );
	 
	apWfo = GetKidAt( mSelectOpt );
	if ( apWfo )
	{
		apWfo->Activate( TRUE );
		return TRUE;
	}

	return TRUE;
}

BOOLEAN CEnmWfoSelector::PreStep()
{
	return TRUE;
}

BOOLEAN  CEnmWfoSelector::SelectStep( U16 sel )
{
	if ( GetKidAt( sel ) )
	{
		mSelectOpt = sel;
		return TRUE;
	}
	else
		return FALSE;
}

void  CEnmWfoSelector::SyncState()
{
	if ( GetKidCount() <= 0 )
		return;

	if ( !GetPage() )
		return;
	
	CEnmWfSelectPage *apPage = static_cast<CEnmWfSelectPage*> ( GetPage() );
	if ( !apPage )
		return;

	if ( mSelectOpt >= GetKidCount() )
		mSelectOpt = 0;

	apPage->SelectItem( mSelectOpt );
}


BOOLEAN  CEnmWfoSelector::IsCanForward()
{
	BOOLEAN bCanForward = FALSE;

	IWorkStep *apWfo = GetKidAt( mSelectOpt );
	if ( apWfo )
	{
		bCanForward = apWfo->IsCanForward();
	}

	return bCanForward;
}

BOOLEAN  CEnmWfoSelector::IsCanExit()
{
	return GetParent() ? GetParent()->IsCanExit() : FALSE;
}

BOOLEAN  CEnmWfoSelector::OnForward()
{
	BOOLEAN bCanForward = FALSE;

	IWorkStep *apWfo = GetKidAt( mSelectOpt );
	if ( apWfo )
	{
		bCanForward = apWfo->Activate( TRUE ); //OnForward();
	}

	return bCanForward;
}

BOOLEAN  CEnmWfoSelector::OnExit()
{
	BOOLEAN bRet = GetParent()->RequestReturn();
	if ( bRet )
	{
		mSelectOpt = 0;
		SyncState();
	}
	
	return bRet;
}

#include "IWorkStep.h"
#include "EnmBasePage.hpp"


IWorkStep  *IWorkStep::mpActiveStep = NULL;

IWorkStep* IWorkStep::GetActiveStep()
{
	return mpActiveStep;
}

//##ModelId=441BB9D20196
IWorkStep::IWorkStep(IWorkStep* pParent)
: mpParent( pParent )
, mpPage( NULL )
, mbActive( TRUE )
{

}

//##ModelId=441BB9DE0128
IWorkStep::~IWorkStep()
{
	if ( mpPage )
		delete mpPage;
}

//##ModelId=441BD5540242
BOOLEAN IWorkStep::Activate(BOOLEAN bActive)
{
	mbActive = bActive;
	if ( mpPage )
	{
		mpPage->Visible( bActive );
	
		MigPage::mpCurPage = mpPage;
		if ( bActive)
			mpPage->FocusOn();
		else
			mpPage->FocusOff();
	}

	if ( bActive )
		mpActiveStep = this;
	else
		mpActiveStep = NULL;

	return TRUE;
}

BOOLEAN IWorkStep::NextStep()
{
	GetParent()->NextStep();

	return TRUE;
}

BOOLEAN IWorkStep::PreStep()
{
	GetParent()->PreStep();

	return TRUE;
}

BOOLEAN IWorkStep::Return()
{
	if ( GetParent() )
	{
		Activate( FALSE );
		GetParent()->RequestReturn( );
	}

	return TRUE;
}

BOOLEAN IWorkStep::RequestReturn()
{
	Activate( TRUE );

	return TRUE;
}

BOOLEAN IWorkStep::SetPage(CEnmBasePage* pPage)
{
	if ( mpPage == pPage )
		return FALSE;

	mpPage = pPage;
	
	if ( mpPage )
		mpPage->SetStep( this );

	return TRUE;
}

BOOLEAN  IWorkStep::IsCanForward()
{
	return GetPage() ? GetPage()->IsCanForward() : FALSE;
}

BOOLEAN  IWorkStep::IsCanExit()
{
	return GetPage() ? GetPage()->IsCanExit() : FALSE;
}

BOOLEAN  IWorkStep::OnForward()
{
	return GetPage() ? GetPage()->OnForward() : FALSE;
}

BOOLEAN  IWorkStep::OnExit()
{
	return GetPage() ? GetPage()->OnExit() : FALSE;
}

BOOLEAN  IWorkStep::OnPrint()
{
	return GetPage() ? GetPage()->OnPrint() : FALSE;
}

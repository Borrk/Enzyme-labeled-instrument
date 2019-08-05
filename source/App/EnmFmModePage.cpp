// EnmFmModePage.cpp: implementation of the CEnmFmModePage class.
//
//////////////////////////////////////////////////////////////////////
#include "EnmFmModePage.h"

CEnmFmModePage::~CEnmFmModePage()
{
}

CEnmFmModePage::CEnmFmModePage( U32 id )
: CEnmBasePage( "", id )
{
	mpMeasFilterCtrl = 0;
	mpRefFilterCtrl  = 0;
	mpMixModeCtrl    = 0;

	mpColumnCtrl  = 0;
	mpMixTimeCtrl = 0;
	mpPeriodCtrl  = 0;
}

void CEnmFmModePage::FocusOn()
{
	CEnmBasePage::FocusOn();
	ControlFocusSw( mpColumnCtrl );
}
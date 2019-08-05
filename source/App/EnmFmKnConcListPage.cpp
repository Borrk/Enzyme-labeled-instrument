#include "EnmFmKnConcListPage.h"
#include "EnmScanEngine.h"
#include "EnmApp.hpp"
#include "EnmWfo.h"


CEnmFmKnConcListPage::CEnmFmKnConcListPage()
{
	mbEditable = FALSE;
}
	
BOOLEAN  CEnmFmKnConcListPage::OnExit()
{
	BOOLEAN bRet = FALSE;

	CEnmApp& rApp = CEnmApp::Instance();
	if ( rApp.GetScanEngine() )
	{
		rApp.GetScanEngine()->StopScan( );
		GetScanContext().SetFlag( 0 );
	}

	return inherited::OnExit();
}

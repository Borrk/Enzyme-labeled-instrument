// EnmWfsFactory.cpp: implementation of the CEnmWfsFactory class.
//
//////////////////////////////////////////////////////////////////////
#include "EnmWfStepHub.h"
#include "EnmWfoRoot.h"
#include "EnmWfsFactory.h"

#include "EnmMainPage.hpp"
#include "EnmScanContext.h"
#include "EnmModePage.h"
#include "EnmPlatePage.h"
#include "EnmProcPanelPage.h"
#include "EnmFastModePage.h"
#include "EnmConcListPage.h"
#include "EnmCutoffCompute.h"
#include "EnmModePage.h"
#include "EnmWfSelectPage.h"
#include "EnmDeleteCutoffPage.h"
#include "EnmDensityListPage.h"
#include "EnmDensityCurvePage.h"

#include "EnmConsolePage.h"

#include "EnmFmWpModePage.h"
#include "EnmFmWpMeasurePage.h"

#include "EnmFmCmPage.h"
#include "EnmFmCmMeasPage.h"
#include "EnmFmKnConcListPage.h"

#include "EnmPgListPage.h"
#include "EnmPgDtPage.h"
#include "EnmPgEtModePage.h"
#include "EnmPgEtQualityPage.h"
#include "EnmPgEtMixingPage.h"
#include "EnmPgEtCurvePage.h"
#include "EnmPgEtQualitativePage.h"
#include "EnmProgramScanPage.h"
#include "EnmPgEtCutoffPage.h"
#include "EnmSampleConcListPage.h"
#include "EnmControlConcPage.h"
#include "EnmSaveCutoffPage.h"

/// Reader setup
#include "EnmCfgFilterPage.h"
#include "EnmCfgTimePage.h"
#include "EnmCfgCommuPage.h"
#include "EnmCfgPlateCheckPage.h"
#include "EnmCfgPrinterPage.h"
#include "EnmCfgCheckPage.h"
#include "EnmCfgServiceTestPage.h"


CEnmWfoSelector* CEnmWfsFactory::CreateWfs()
{
	U32 aMainItem[] = 
	{
		STR_MAIN_MODE,
		STR_MAIN_FAST_MODE,
		STR_MAIN_PROGRAMMING,
		STR_MAIN_PROGRAMLIST,
		STR_MAIN_DATA_LIST,
		STR_MAIN_SYS_SETUP,
		STR_MAIN_HOST_CONTROL
	};

	CEnmBasePage *apPage = new CEnmWfSelectPage( (U32)STR_BLANK, 2 );
	((CEnmWfSelectPage*)apPage)->CreateItems( aMainItem, sizeof(aMainItem) / sizeof(aMainItem[0]) );

	CEnmWfoRoot   *apSel = new CEnmWfoRoot(  );
	apSel->SetPage( apPage );

	CreateProgramModeTree( *apSel );
	CreateFastModeTree( *apSel );
	CreateProgrammingTree( *apSel );
	CreateProgramListTree( *apSel );
	CreateDataListTree( *apSel );
	CreateSysConfigTree( *apSel );
	CreateHostConsoleTree( *apSel );

	return apSel;
}

BOOLEAN		CEnmWfsFactory::CreateProgramModeTree( CEnmWfoSelector& root )
{
	CEnmWfo *apWfoPm = new CEnmWfo( &root );
	root.AddKidAt( *apWfoPm );
	
	/// Create Programmed Mode Tree
	CEnmBasePage *apPage;
	CEnmWfStepHub *apStepHub;
	IWorkStep  *apStep;

	apStepHub = new CEnmWfStepHub( apWfoPm );
	apPage = new CEnmModePage();
	apPage->SetTitle( STR_PPROGRAMEDIT_TITLE );
	apStepHub->SetPage( apPage );
	apWfoPm->AddKidAt( *apStepHub );

	apStep = new IWorkStep( apWfoPm );
	apPage = new CEnmPlatePage();
	apStep->SetPage( apPage );
	apWfoPm->AddKidAt( *apStep );
	
	apStepHub->AddStep( *apStep, 0, -1 );
	apStepHub->AddStep( *apStep, 1, -1 );

	apStep = new IWorkStep( apWfoPm );
	apPage = new CEnmProgramScanPage( "", 0 );
	apPage->SetTitle( STR_PPROGRAMEDIT_TITLE );
	apStep->SetPage( apPage );
	apWfoPm->AddKidAt( *apStep );

	apStepHub->AddStep( *apStep, 0, -1 );
	apStepHub->AddStep( *apStep, 1, -1 );

	apStep = new IWorkStep( apWfoPm );
	apPage = new CEnmConcListPage();
	apStep->SetPage( apPage );
	apWfoPm->AddKidAt( *apStep );

	apStepHub->AddStep( *apStep, 0, -1 );
	apStepHub->AddStep( *apStep, 1, -1 );

	/// Qualitative steps
	apStep = new IWorkStep( apWfoPm );
	apPage = new CEnmDensityListPage();
	apStep->SetPage( apPage );
	apWfoPm->AddKidAt( *apStep );

	apStepHub->AddStep( *apStep, 1, -1 );

	apStep = new IWorkStep( apWfoPm );
	apPage = new CEnmDensityCurvePage();
	apPage->SetForWardMode( STEP_BW );
	apStep->SetPage( apPage );
	apWfoPm->AddKidAt( *apStep );

	apStepHub->AddStep( *apStep, 1, -1 );

	/// Qualititive steps
	/// Comput Step Selector
	U32 aItem[] = 
	{
		STR_PPROCESS_COMPUTE,
		STR_PPROCESS_EDIT,
		STR_PPROCESS_DELETE,
		STR_PPROCESS_SAVE,
	};

	apPage = new CEnmWfSelectPage( STR_PPROCESS_TITLE, 1 );
	((CEnmWfSelectPage*)apPage)->CreateItems( aItem, sizeof(aItem) / sizeof(aItem[0]) );

	CEnmWfoSelector *apSelDataProc = new CEnmWfoSelector( apWfoPm );
	apSelDataProc->SetPage( apPage );
	apWfoPm->AddKidAt( *apSelDataProc );

	apStepHub->AddStep( *apSelDataProc, 0, -1 );

	CEnmWfo  *apWfoCompute		= new CEnmWfo( apSelDataProc );
	CEnmWfo  *apWfoEditCutoff	= new CEnmWfo( apSelDataProc );
	CEnmWfo  *apWfoDelCutoff     = new CEnmWfo( apSelDataProc );
	CEnmWfo  *apWfoSaveCutoff    = new CEnmWfo( apSelDataProc );

	apSelDataProc->AddKidAt( *apWfoCompute );
	apSelDataProc->AddKidAt( *apWfoEditCutoff );
	apSelDataProc->AddKidAt( *apWfoDelCutoff );
	apSelDataProc->AddKidAt( *apWfoSaveCutoff );
	
	apStep = new IWorkStep( apWfoCompute );
	apPage = new CEnmCutoffCompute();
	apStep->SetPage( apPage );
	apWfoCompute->AddKidAt( *apStep );

	apStep = new IWorkStep( apWfoCompute );
	apPage = new CEnmSampleConcListPage();
	apStep->SetPage( apPage );
	apWfoCompute->AddKidAt( *apStep );

	/// Edit Cutoff
	apStep = new IWorkStep( apWfoEditCutoff );
	apPage = new CEnmControlConcPage();
	apStep->SetPage( apPage );
	apWfoEditCutoff->AddKidAt( *apStep );

	/// Delete Cutoff
	apStep = new IWorkStep( apWfoDelCutoff );
	apPage = new CEnmDeleteCutoffPage();
	apStep->SetPage( apPage );
	apWfoDelCutoff->AddKidAt( *apStep );

	/// Save Cutoff
	apStep = new IWorkStep( apWfoSaveCutoff );
	apPage = new CEnmSaveCutoffPage();
	apStep->SetPage( apPage );
	apWfoSaveCutoff->AddKidAt( *apStep );

	return TRUE;
}

BOOLEAN		CEnmWfsFactory::CreateFastModeTree( CEnmWfoSelector& root )
{
	CEnmBasePage *apPage;
	IWorkStep  *apStep;

	CEnmWfo *apWfoFm = new CEnmWfo( &root );
	root.AddKidAt( *apWfoFm );

	/// Fast Mode Item
	U32  aFmItem[] = 
	{
		STR_PFAST_WHOLEPLATE,
		STR_PFAST_COLUMNMONITOR,
		STR_PFAST_KINETIC
	};

	apPage = new CEnmWfSelectPage( STR_PFAST_TITLE, 4 );
	((CEnmWfSelectPage*)apPage)->CreateItems( aFmItem, sizeof(aFmItem) / sizeof(aFmItem[0]) );

	CEnmWfoSelector *apMeasType = new CEnmWfoSelector( apWfoFm );
	apMeasType->SetPage( apPage );
	apWfoFm->AddKidAt( *apMeasType );

	CEnmWfo  *apWfoFmWp	= new CEnmWfo( apMeasType );
	apMeasType->AddKidAt( *apWfoFmWp );

	/// Whole mode
	apStep = new IWorkStep( apWfoFmWp );
	apPage = new CEnmFmWpModePage();
	apStep->SetPage( apPage );
	apWfoFmWp->AddKidAt( *apStep );

	apStep = new IWorkStep( apWfoFmWp );
	apPage = new CEnmFmWpMeasurePage();
	apStep->SetPage( apPage );
	apWfoFmWp->AddKidAt( *apStep );

	apStep = new IWorkStep( apWfoFmWp );
	apPage = new CEnmSampleConcListPage();
	apStep->SetPage( apPage );
	apWfoFmWp->AddKidAt( *apStep );

	apStep = new IWorkStep( apWfoFmWp );
	apPage = new CEnmFmWpReportPage();
	apPage->SetForWardMode( STEP_BW );
	apStep->SetPage( apPage );
	apWfoFmWp->AddKidAt( *apStep );

	/// column monitor
	CEnmWfo  *apWfoFmCm	= new CEnmWfo( apMeasType );
	apMeasType->AddKidAt( *apWfoFmCm );
	
	apStep = new IWorkStep( apWfoFmCm );
	apPage = new CEnmFmCmModePage();
	apStep->SetPage( apPage );
	apWfoFmCm->AddKidAt( *apStep );

	apStep = new IWorkStep( apWfoFmCm );
	apPage = new CEnmFmCmMeasPage();
	apStep->SetPage( apPage );
	apWfoFmCm->AddKidAt( *apStep );

	/// Kinetic 
	CEnmWfo  *apWfoFmKn =  new CEnmWfo( apMeasType );
	apMeasType->AddKidAt( *apWfoFmKn );
		
	apStep = new IWorkStep( apWfoFmKn );
	apPage = new CEnmFmKnModePage();
	apStep->SetPage( apPage );
	apWfoFmKn->AddKidAt( *apStep );

	apStep = new IWorkStep( apWfoFmKn );
	apPage = new CEnmFmKnMeasPage();
	apStep->SetPage( apPage );
	apWfoFmKn->AddKidAt( *apStep );

	apStep = new IWorkStep( apWfoFmKn );
	apPage = new CEnmFmKnConcListPage();
	apPage->SetForWardMode( STEP_BW );

	apStep->SetPage( apPage );
	apWfoFmKn->AddKidAt( *apStep );

	return TRUE;
}

BOOLEAN		CEnmWfsFactory::CreateProgrammingTree( CEnmWfoSelector& root )
{
	CEnmBasePage *apPage;
	CEnmWfStepHub *apStepHub;
	IWorkStep  *apStep;

	CEnmWfo *apWfoPg = new CEnmWfo( &root );
	root.AddKidAt( *apWfoPg );

	U32  aPgItem[] = 
	{
		STR_MAIN_PROGRAMLIST,
		STR_PPROGRAM_NEW,
		STR_PPROGRAM_EDIT,
		STR_PPROGRAM_DELETE
	};
	
	apPage = new CEnmWfSelectPage( STR_PPROGRAM_TITLE, 5 );
	((CEnmWfSelectPage*)apPage)->CreateItems( aPgItem, sizeof(aPgItem) / sizeof(aPgItem[0]) );

	CEnmWfoSelector *apProgram = new CEnmWfoSelector( apWfoPg );
	apProgram->SetPage( apPage );
	apWfoPg->AddKidAt( *apProgram );

	CEnmWfo  *apWfoPgList	= new CEnmWfo( apProgram );
	CEnmWfo  *apWfoPgNew	= new CEnmWfo( apProgram );
	CEnmWfo  *apWfoPgEdit	= new CEnmWfo( apProgram );
	CEnmWfo  *apWfoPgDel	= new CEnmWfo( apProgram );

	apProgram->AddKidAt( *apWfoPgList );
	apProgram->AddKidAt( *apWfoPgNew );
	apProgram->AddKidAt( *apWfoPgEdit );
	apProgram->AddKidAt( *apWfoPgDel );


#if 1
	/************************************************************************/
	/*      Program List                                                    */
	/************************************************************************/
	apPage = new CEnmPgListPage();
	apStep = new IWorkStep( apWfoPgList );
	apStep->SetPage( apPage );
	apWfoPgList->AddKidAt( *apStep );


	/************************************************************************/
	/* Create New Program                                                   */
	/************************************************************************/ 
	apStepHub = new CEnmWfStepHub( apWfoPgNew );
	apPage = new CEnmPgEtModePage();
	//((CEnmPgEtModePage*)apPage)->SetEditMode( CEnmPgEtModePage::MODE_NEW );
	apStepHub->SetPage( apPage );
	apWfoPgNew->AddKidAt( *apStepHub );
		
	apStep = new IWorkStep( apWfoPgNew );
	apPage = new CEnmPgEtQualityPage();
	apStep->SetPage( apPage );
	apWfoPgNew->AddKidAt( *apStep );
	apStepHub->AddStep( *apStep, 0, -1 );
	
	apStep = new IWorkStep( apWfoPgNew );
	apPage = new CEnmPgEtMixingPage();
	apStep->SetPage( apPage );
	apWfoPgNew->AddKidAt( *apStep );
	apStepHub->AddStep( *apStep, 0, -1 );
	
	IWorkStep *apMixingStep = apStep;

	apStep = new IWorkStep( apWfoPgNew );
	apPage = new CEnmPgEtCurvePage();
	apStep->SetPage( apPage );
	apWfoPgNew->AddKidAt( *apStep );
	apStepHub->AddStep( *apStep, 0, -1 );

	/// Qualitative path
	apStep = new IWorkStep( apWfoPgNew );
	apPage = new CEnmPgEtQualitativePage();
	apStep->SetPage( apPage );
	apStepHub->AddStep( *apStep, 1, -1 );

	apStepHub->AddStep( *apMixingStep, 1, -1 );

	apStep = new IWorkStep( apWfoPgNew );
	apPage = new CEnmPgEtCutoffPage();
	apStep->SetPage( apPage );
	apStepHub->AddStep( *apStep, 1, -1 );

	
	/************************************************************************/
	/* Edit program                                                         */
	/************************************************************************/ 
	apStepHub = new CEnmWfStepHub( apWfoPgEdit );
	apPage = new CEnmPgEtModePage();
	((CEnmPgEtModePage*)apPage)->SetEditMode( CEnmPgEtModePage::MODE_EDIT );
	apStepHub->SetPage( apPage );
	apWfoPgEdit->AddKidAt( *apStepHub );
		
	apStep = new IWorkStep( apWfoPgEdit );
	apPage = new CEnmPgEtQualityPage();
	apStep->SetPage( apPage );
	apWfoPgEdit->AddKidAt( *apStep );
	apStepHub->AddStep( *apStep, 0, -1 );
	
	apStep = new IWorkStep( apWfoPgEdit );
	apPage = new CEnmPgEtMixingPage();
	apStep->SetPage( apPage );
	apWfoPgEdit->AddKidAt( *apStep );
	apStepHub->AddStep( *apStep, 0, -1 );
	
	apMixingStep = apStep;

	apStep = new IWorkStep( apWfoPgEdit );
	apPage = new CEnmPgEtCurvePage();
	apStep->SetPage( apPage );
	apWfoPgEdit->AddKidAt( *apStep );
	apStepHub->AddStep( *apStep, 0, -1 );

	/// Qualitative path
	apStep = new IWorkStep( apWfoPgEdit );
	apPage = new CEnmPgEtQualitativePage();
	apStep->SetPage( apPage );
	apStepHub->AddStep( *apStep, 1, -1 );

	apStepHub->AddStep( *apMixingStep, 1, -1 );

	apStep = new IWorkStep( apWfoPgEdit );
	apPage = new CEnmPgEtCutoffPage();
	apStep->SetPage( apPage );
	apStepHub->AddStep( *apStep, 1, -1 );


	/************************************************************************/
	/* Delete program                                                       */
	/************************************************************************/
	apPage = new CEnmPgDtPage();
	apStep = new IWorkStep( apWfoPgDel );
	apStep->SetPage( apPage );
	apWfoPgDel->AddKidAt( *apStep );

#else
	/// Program List
	apPage = new CEnmPgListPage();
	apStep = new IWorkStep( apWfoPgList );
	apStep->SetPage( apPage );
	apWfoPgList->AddKidAt( *apStep );
		
	/// Create New Program
	apStepHub = new CEnmWfStepHub( apWfoPgNew );
	apPage = new CEnmPgEtModePage();
	//((CEnmPgEtModePage*)apPage)->SetEditMode( CEnmPgEtModePage::MODE_EDIT );
	apStepHub->SetPage( apPage );
	apWfoPgNew->AddKidAt( *apStepHub );
		
	apStep = new IWorkStep( apWfoPgNew );
	apPage = new CEnmPgEtQualityPage();
	apStep->SetPage( apPage );
	apWfoPgNew->AddKidAt( *apStep );
	apStepHub->AddStep( *apStep, 0, -1 );
	
	apStep = new IWorkStep( apWfoPgNew );
	apPage = new CEnmPgEtMixingPage();
	apStep->SetPage( apPage );
	apWfoPgNew->AddKidAt( *apStep );
	apStepHub->AddStep( *apStep, 0, -1 );
	
	IWorkStep *apMixingStep = apStep;

	apStep = new IWorkStep( apWfoPgNew );
	apPage = new CEnmPgEtCurvePage();
	apStep->SetPage( apPage );
	apWfoPgNew->AddKidAt( *apStep );
	apStepHub->AddStep( *apStep, 0, -1 );


	apStep = new IWorkStep( apWfoPgNew );
	apPage = new CEnmPgEtQualitativePage();
	apStep->SetPage( apPage );
	//apWfoPgNew->AddKidAt( *apStep );
	apStepHub->AddStep( *apStep, 1, -1 );

	apStepHub->AddStep( *apMixingStep, 1, -1 );

	apStep = new IWorkStep( apWfoPgNew );
	apPage = new CEnmPgEtCutoffPage();
	apStep->SetPage( apPage );
	//apWfoPgNew->AddKidAt( *apStep );
	apStepHub->AddStep( *apStep, 1, -1 );

	/// Edit program
	
	/// Delete program
	apPage = new CEnmPgDtPage();
	apStep = new IWorkStep( apWfoPgDel );
	apStep->SetPage( apPage );
	apWfoPgDel->AddKidAt( *apStep );
#endif

	return TRUE;
}

BOOLEAN		CEnmWfsFactory::CreateProgramListTree( CEnmWfoSelector& root )
{
	CEnmBasePage *apPage;
	IWorkStep  *apStep;

	CEnmWfo *apWfoPl = new CEnmWfo( &root );
	root.AddKidAt( *apWfoPl );

	apPage = new CEnmPgListPage();
	apPage->SetTitle( STR_MAIN_PROGRAMLIST );
	apStep = new IWorkStep( apWfoPl );
	apStep->SetPage( apPage );
	apWfoPl->AddKidAt( *apStep );

	return TRUE;
}

BOOLEAN		CEnmWfsFactory::CreateDataListTree( CEnmWfoSelector& root )
{
	CEnmBasePage *apPage;
	IWorkStep  *apStep;

	CEnmWfo *apWfoCfg = new CEnmWfo( &root );
	root.AddKidAt( *apWfoCfg );

	return TRUE;
}

BOOLEAN		CEnmWfsFactory::CreateSysConfigTree( CEnmWfoSelector& root )
{
	CEnmBasePage *apPage;
	IWorkStep  *apStep;

	CEnmWfo *apWfoCfg = new CEnmWfo( &root );
	root.AddKidAt( *apWfoCfg );

	U32  aPgItem[] = 
	{
		STR_PCFG_FILTER,
		STR_PCFG_TIME,
		STR_PCFG_SERIALPORT,
		STR_PCFG_PRINTER,
		STR_PCFG_MAINTENANCE,
		STR_PCFG_CHECK_PLATE
	};
	
	apPage = new CEnmWfSelectPage( STR_PCFG_TITLE, 5 );
	((CEnmWfSelectPage*)apPage)->CreateItems( aPgItem, sizeof(aPgItem) / sizeof(aPgItem[0]) );

	CEnmWfoSelector *apConfig = new CEnmWfoSelector( apWfoCfg );
	apConfig->SetPage( apPage );
	apWfoCfg->AddKidAt( *apConfig );

//////////////////////////////////////////////////

	CEnmWfo  *apWfoFilter	= new CEnmWfo( apConfig );
	CEnmWfo  *apWfoTime		= new CEnmWfo( apConfig );
	CEnmWfo  *apWfoCommu	= new CEnmWfo( apConfig );
	CEnmWfo  *apWfoPrinter	= new CEnmWfo( apConfig );
	CEnmWfo  *apWfoMaintenance	= new CEnmWfo( apConfig );
	CEnmWfo  *apWfoCheckPlate	= new CEnmWfo( apConfig );

	apConfig->AddKidAt( *apWfoFilter );
	apConfig->AddKidAt( *apWfoTime );
	apConfig->AddKidAt( *apWfoCommu );
	apConfig->AddKidAt( *apWfoPrinter );
	apConfig->AddKidAt( *apWfoMaintenance );
	apConfig->AddKidAt( *apWfoCheckPlate );

	/// Filter 
	apStep = new IWorkStep( apWfoFilter );
	apPage = new CEnmCfgFilterPage();
	apPage->SetTitle( STR_PCFGFILTER_TITLE );
	apPage->SetForWardMode( STEP_RETURN );
	apStep->SetPage( apPage );
	apWfoFilter->AddKidAt( *apStep );

	/// Time-Language-Name
	apStep = new IWorkStep( apWfoTime );
	apPage = new CEnmCfgTimePage();
	apPage->SetTitle( STR_PCFG_TIME_TITLE );
	apPage->SetForWardMode( STEP_RETURN );
	apStep->SetPage( apPage );
	apWfoTime->AddKidAt( *apStep );

	/// Serial port
	apStep = new IWorkStep( apWfoCommu );
	apPage = new CEnmCfgCommuPage();
	apPage->SetTitle( STR_PCFG_COMMU_TITLE );
	apPage->SetForWardMode( STEP_RETURN );
	apStep->SetPage( apPage );
	apWfoCommu->AddKidAt( *apStep );

	/// printer check
	apStep = new IWorkStep( apWfoPrinter );
	apPage = new CEnmCfgPrinterPage();
	apPage->SetTitle( STR_PCFG_PLATECHECK_TITLE );
	apPage->SetForWardMode( STEP_RETURN );
	apStep->SetPage( apPage );
	apWfoPrinter->AddKidAt( *apStep );

	/// Maintenance
	apStep = new IWorkStep( apWfoMaintenance );
	apPage = new CEnmCfgCheckPage();
	apPage->SetTitle( STR_PCFG_CHECK_TITLE );
	apStep->SetPage( apPage );
	apWfoMaintenance->AddKidAt( *apStep );

	apStep = new IWorkStep( apWfoMaintenance );
	apPage = new CEnmCfgServiceTestPage();
	apPage->SetTitle( STR_PCFG_SERVICETEST_TITLE );
	apStep->SetPage( apPage );
	apWfoMaintenance->AddKidAt( *apStep );

	/// Plate check
	apStep = new IWorkStep( apWfoCheckPlate );
	apPage = new CEnmCfgPlateCheckPage();
	apPage->SetTitle( STR_PCFG_PLATECHECK_TITLE );
	apPage->SetForWardMode( STEP_RETURN );
	apStep->SetPage( apPage );
	apWfoCheckPlate->AddKidAt( *apStep );

	return TRUE;
}

BOOLEAN		CEnmWfsFactory::CreateHostConsoleTree( CEnmWfoSelector& root )
{
	CEnmBasePage *apPage;
	IWorkStep  *apStep;

	CEnmWfo *apWfoPl = new CEnmWfo( &root );
	root.AddKidAt( *apWfoPl );

	apPage = new CEnmConsolePage();
	apPage->SetTitle( STR_PPCMODE_TITLE );
	apStep = new IWorkStep( apWfoPl );
	apStep->SetPage( apPage );
	apWfoPl->AddKidAt( *apStep );

	return TRUE;
}

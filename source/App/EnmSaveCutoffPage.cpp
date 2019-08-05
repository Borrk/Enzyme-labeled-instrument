#include "EnmSaveCutoffPage.h"
#include "EnmCutoffLib.h"
#include "EnmConcDataUtil.h"


CEnmSaveCutoffPage::CEnmSaveCutoffPage()
:CEnmBasePage( "", ENM_PAGE_ID_CONTROLCONC )
{
	SetTitle( STR_PSAVECUTOFF_TITLE );
	CreateControls();
}

CEnmSaveCutoffPage::~CEnmSaveCutoffPage()
{
}

void CEnmSaveCutoffPage::CreateControls( void)
{
	U16 aLeft = mLeftMargin + 20;

	tagCS aCS[] = 
	{
		tagCS( ENM_CTRL_LABEL, aLeft,	mTopMargin+20*3,	140,	20,  ENM_CTRL_STATIC | ENM_CTRL_NORMAL, 1, "", STR_PSAVECUTOFF_SAVING_CUTOFF ),             
		tagCS( ENM_CTRL_GRIDBOX, aLeft + 190,	mTopMargin+20*3, 140,	20,  ENM_CTRL_NORMAL, 2,	"", STR_BLANK ),             
	};

	aCS[0].hParent = this;
	mpSaveTip = new MigControl( aCS[0] );
	AddControl( mpSaveTip );

	/// Create New Cutoff UpDn control
	U16 aLin[] = 
	{
		STR_Y, STR_N
	};

	mpSaveConfirm = new MigUpDn( aCS[1] );
	AddControl( mpSaveConfirm );
	mpSaveConfirm->SetItems( aLin, 2 );
}

void CEnmSaveCutoffPage::FocusOn()
{
	mpSaveConfirm->SetCurSel( 0 );
	inherited::FocusOn();
}

BOOLEAN  CEnmSaveCutoffPage::OnForward()
{
	BOOLEAN bRet = FALSE;

	SaveCutoff();

	return inherited::OnExit();
}

BOOLEAN	 CEnmSaveCutoffPage::SaveCutoff()
{
	/// User select 'No'
	if ( 1 == mpSaveConfirm->GetCurSel() )
		return FALSE;

	CEnmScanContext &rScanContext = GetScanContext();
	//if ( !rScanContext.IsPNcCalculated() )
	{
		if ( !CEnmConcDataUtil::CalcPcNc( rScanContext ) )
			return FALSE;
	}

	tagScanData *apData = rScanContext.GetScanData();
	if ( !apData )
		return FALSE;

	tagCutoffEntry aCutoff;
	
	Convert2Int( apData->stDate, apData->stTime, aCutoff.mDate, aCutoff.mTime );
	strcpy( aCutoff.mEquation, rScanContext.mProtocol.stQualitative.szCutoff );
	strcpy( aCutoff.mProtName, rScanContext.mProtocol.stMeasure.szName );
	aCutoff.mPC = rScanContext.mCalcData.fPc;
	aCutoff.mNC = rScanContext.mCalcData.fNc;

	CEnmCutoffLib& rCutoffLib =  CEnmCutoffLib::Instance();
	BOOLEAN bAdded = rCutoffLib.Add( aCutoff );
	if ( rCutoffLib.IsChanged() )
	{
		rCutoffLib.Sync( FALSE );
	}
	
	return bAdded;
}

void	 CEnmSaveCutoffPage::Convert2Int( tagDate& date, tagTime& time, U32& uDate, U32& uTm )
{
	U32   aYear = date.year;
	U32   aMonth = date.month;
	U32   aDay  = date.day;

	uDate = (aYear << 16) + (aMonth << 8) + aDay;
	uTm   = (time.hour << 16) + (time.min << 8) + time.sec;
}

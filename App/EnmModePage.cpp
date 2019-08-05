// EnmModePage.cpp: implementation of the CEnmModePage class.
//
//////////////////////////////////////////////////////////////////////

#include "EnmModePage.h"
#include "EnmApp.hpp"
#include "EnmScanContext.h"
#include "EnmModeLib.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnmModePage::CEnmModePage(  )
: CEnmModeBasePage( "", 0 )
{
	mTipInfo.mTipID = -1;
	CreateSpecialControl();
}

CEnmModePage::CEnmModePage( MCHAR* pTitle, U32 id )
: CEnmModeBasePage( pTitle, id )
{
	mTipInfo.mTipID = -1;
	CreateSpecialControl();
}

CEnmModePage::~CEnmModePage()
{

}

void   CEnmModePage::CreateSpecialControl()
{
	U16 	aItem[] = 
	{
		STR_PPROGRAM_SAMPLES,
		STR_BLANK,
		STR_PPROGRAM_NEWCUTOFF 
	};

	INT32U aStyleStatic = ENM_CTRL_NORMAL | ENM_CTRL_STATIC ; 

	tagCS aCS[] = 
	{
		tagCS( ENM_CTRL_LABEL, 		mLeftMargin,	mTopMargin+20*4,	90,	20,  aStyleStatic, MODE_CTRL_SAMPLE_LABEL,	0, aItem[0] ),             
		tagCS( ENM_CTRL_LABEL, 		mLeftMargin+160,mTopMargin+20*4,	90,	20,  aStyleStatic, MODE_CTRL_MIXMODE,	0, aItem[1] ),             
		tagCS( ENM_CTRL_LABEL, 		mLeftMargin,	mTopMargin+20*5,	100,20,  aStyleStatic, MODE_CTRL_NEWCUTOFF_LABEL,	0, aItem[2] ),             
		tagCS( ENM_CTRL_EDITBOX, 	mLeftMargin+160,	mTopMargin+20*4,	20,20,  ENM_CTRL_NORMAL, MODE_CTRL_SAMPLE,	0, STR_BLANK ),             
		tagCS( ENM_CTRL_UPDOWNBOX, 	mLeftMargin+110,	mTopMargin+20*5,	100,20,  ENM_CTRL_NORMAL, MODE_CTRL_CUTOFF,	0, STR_BLANK ),             
	};

	MigControl *apCtrl = NULL;
	for ( INT8U i = 0; i < 3; i++ )
	{
		aCS[i].hParent = this;
		apCtrl = AddControl( aCS[i] );
	}

	/// Create Edit Box
	tagEditBoxCS aECS;
	aECS.maxVal  = 88;
	aECS.minVal  = 0;
	aECS.iVal    = 88;
	aECS.maxChar = 2;
	aECS.type	 = ENM_EB_TYPE_INT;
	aECS.titleWidth = 1;//strlen(apItem[2]) * GetFontW();
	aECS.titleId = MIG_STR_ID_INVALID;

	mpSampleBox = new MigEditBox( aCS[3], aECS ) ;	
	AddControl( mpSampleBox );
	
	/// Create New Cutoff UpDn control
	U16 aLin[] = 
	{
		STR_Y, STR_N
	};

	MigUpDn *apUD = new MigUpDn( aCS[4] );
	AddControl( apUD );
	apUD->SetItems( aLin, 2 );
}

void CEnmModePage::ShowTips( const tagTipInfo& tipInfo )
{
	mTipInfo = tipInfo;
}

void  CEnmModePage::DrawTips()
{
	if ( mTipInfo.mTipID == -1 )
		return;

	MCHAR* apStr = MLoadStr( mTipInfo.mTipID );
	if ( apStr )
	{
		PutBox( mLeftMargin, 160, 200, 30 );
		PutStr( mLeftMargin + 20, 166, apStr, 0 );
	}
}

void CEnmModePage::	FillProtoNo()
{
	CEnmModeBasePage::FillProtoNo();
}

void    CEnmModePage::SyncUI()
{	
	CEnmModeBasePage::SyncUI();
	
	CEnmModeLib& rML = CEnmModeLib::Instance();

	CEnmScanContext &aScanProt = GetScanContext();
	if ( aScanProt.mProtocol.stMeasure.uID == PROT_ID_INVALID )
		return;
	
	U32 aSampleCnt = 0;
	if ( aScanProt.mProtocol.stMeasure.uMeasMode == MEASURE_QUALITATIVE )
	{
		aSampleCnt = aScanProt.mProtocol.stQualitative.uSample;
		mpSampleBox->SetInt( aSampleCnt );
	}
	else
	{
		aSampleCnt = aScanProt.mProtocol.stQuality.uSample;
		mpSampleBox->SetInt( aSampleCnt );
	}
	
}

void CEnmModePage::FocusOn()
{
	CEnmModeBasePage::FocusOn();
}

void CEnmModePage::OnKeyOK()
{
	CEnmBasePage::OnKeyOK();

	if ( GetCurCtrl() == mpSampleBox && !mpSampleBox->IsInEdit() )
	{
		CEnmScanContext &rScanProt = GetScanContext();
		U32 aSampleCnt = mpSampleBox->GetInt();
		if ( rScanProt.mProtocol.stMeasure.uMeasMode == MEASURE_QUALITATIVE )
			rScanProt.mProtocol.stQualitative.uSample = aSampleCnt;
		else
			rScanProt.mProtocol.stQuality.uSample = aSampleCnt;
	}
}

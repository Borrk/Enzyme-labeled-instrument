
#include <math.h>
#include "EnmFmWpModePage.h"
#include "EnmBase.hpp"
#include "EnmSysCfgLib.h"


CEnmFmWpModePage::CEnmFmWpModePage( )
: CEnmFmModePage( PID_FM_WP_SETUP )
{
	SetTitle( STR_PWHOLE_TITLE );
	CreateControls();

	ShowFuncKey( TRUE );
}

void CEnmFmWpModePage::CreateControls()
{
	INT32U aItem[] = 
	{
		STR_BLANK,
		STR_PWHOLE_FILTER,
		STR_PWHOLE_WL,
		STR_PWHOLE_COLUMNS,
		STR_PWHOLE_MIX_TIME,
		STR_PWHOLE_MIX_MODE,
	}; 
	
	U16 aLeft = mLeftMargin;
	INT32U aStyleStatic = ENM_CTRL_NORMAL | ENM_CTRL_STATIC ; 

	tagCS aCS[] = 
	{
		tagCS( ENM_CTRL_LABEL, 		aLeft,	mTopMargin+20*0,	60,	20,  aStyleStatic, 1,	0, aItem[1] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft+60,	mTopMargin+20*0,	20,	20,  aStyleStatic, 2,	0, aItem[0] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft+80,	mTopMargin+20*0,	40,20,  aStyleStatic, 3,	0, aItem[2] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft,	mTopMargin+20*1,	60,20,  aStyleStatic, 4,	0, aItem[1] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft+60,	mTopMargin+20*1,	20,	20,  aStyleStatic, 1,	0, aItem[0] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft+80,	mTopMargin+20*1,	40,	20,  aStyleStatic, 2,	0, aItem[2] ),             
	
		tagCS( ENM_CTRL_LABEL, 		aLeft,	mTopMargin+20*2,	80,20,  aStyleStatic, 3,	0, aItem[3] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft,	mTopMargin+20*3,	160,20,  aStyleStatic, 4,	0, aItem[4] ),             
		tagCS( ENM_CTRL_LABEL, 		aLeft,	mTopMargin+20*4,	160,20,  aStyleStatic, 4,	0, aItem[5] ),             
	
		tagCS( ENM_CTRL_UPDOWNBOX, 		aLeft+150,	mTopMargin+20*0,	40,20,  ENM_CTRL_NORMAL, 4,	0, STR_BLANK ),             
		tagCS( ENM_CTRL_UPDOWNBOX, 		aLeft+150,	mTopMargin+20*1,	40,20,  ENM_CTRL_NORMAL, 4,	0, STR_BLANK ),             

		tagCS( ENM_CTRL_UPDOWNBOX, 		aLeft+150,	mTopMargin+20*4,	20,20,  ENM_CTRL_NORMAL, 4,	0, STR_BLANK ),             

		tagCS( ENM_CTRL_EDITBOX, 		aLeft+150,	mTopMargin+20*2,	20,20,  ENM_CTRL_NORMAL, 3,	0, STR_BLANK ),             
		tagCS( ENM_CTRL_EDITBOX, 		aLeft+150,	mTopMargin+20*3,	20,20,  ENM_CTRL_NORMAL, 4,	0, STR_BLANK ),             
	};

	MigControl *apCtrl = NULL;
	for ( INT8U i = 0; i < 11; i++ )
	{
		aCS[i].hParent = this;
		apCtrl = AddControl( aCS[i] );
	}

	/// Create filter list updown box
	tagSystemCfg& rSyscfg = CEnmSysCfgLib::Instance().GetSysCfg();
	U16  aFilterCnt = 0;
	MCHAR*  aListItem[FILTER_MAX] = { 0 };

	for ( U16 n = 0; n < FILTER_MAX; n++ )
	{
		if ( strcmp( rSyscfg.szFilter[n], "N/A" ) != 0 )
			aListItem[aFilterCnt++] = rSyscfg.szFilter[n];
	}


	mpMeasFilterCtrl = new MigUpDn( aCS[9] );
	mpRefFilterCtrl = new MigUpDn( aCS[10] );
	AddControl( mpMeasFilterCtrl );
	AddControl( mpRefFilterCtrl );
	mpMeasFilterCtrl->SetItems( aListItem, aFilterCnt );
	mpRefFilterCtrl->SetItems( aListItem, aFilterCnt );

	tagEditBoxCS  aECS;
	aECS.maxVal =  12;
	aECS.minVal = 1;
	aECS.iVal    = 12;
	aECS.maxChar = 2;
	aECS.type = ENM_EB_TYPE_INT;
	aECS.titleWidth = 1;

	/// Create PC, NC editbox
	mpColumnCtrl = new MigEditBox( aCS[12], aECS );
	AddControl( mpColumnCtrl );

	aECS.maxVal =  25;
	aECS.minVal = 0;
	aECS.iVal    = 0;
	mpMixTimeCtrl = new MigEditBox( aCS[13], aECS );
	AddControl( mpMixTimeCtrl );

	U16 aItems[] = 
	{
		STR_MIXMODE_SLOW,
		STR_MIXMODE_MIDDLE,
		STR_MIXMOE_FAST,
	};

	mpMixModeCtrl = new MigUpDn( aCS[11] );
	mpMixModeCtrl->SetItems( aItems, sizeof(aItems) / sizeof(aItems[0]) );
	AddControl( mpMixModeCtrl );
}

void CEnmFmWpModePage::FocusOn()
{
	inherited::FocusOn();
}

void CEnmFmWpModePage::FocusOff()
{
	//GetCfgFromUI();
	inherited::FocusOff();
}

BOOLEAN  CEnmFmWpModePage::OnForward()
{
	GetCfgFromUI();
	BOOLEAN bRet = inherited::OnForward();

	return bRet;
}

BOOLEAN  CEnmFmWpModePage::GetCfgFromUI( )
{
	tagMeasure *apMeas = &GetScanContext().mProtocol.stMeasure;

	apMeas->uMainFilter  = mpMeasFilterCtrl->GetCurSel();
	apMeas->uReferFilter = mpRefFilterCtrl->GetCurSel();

	apMeas->uMixingMode  = mpMixModeCtrl->GetCurSel();
	apMeas->uMixingTime  = mpMixTimeCtrl->GetInt();

	U8 aColumns = mpColumnCtrl->GetInt();
	apMeas->uColumnMask = (U32)pow( 2.0, (FP64)aColumns ) - 1;

	return TRUE;
}

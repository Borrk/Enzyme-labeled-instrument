#include "EnmBasePage.hpp"
#include "EnmApp.hpp"
#include "EnmWfo.h"
#include "MStrMngr.h"


CEnmBasePage::CEnmBasePage( MCHAR* pTitle, INT32U id )
	: MigPage( ), mpStep( NULL )
{
	mID = id;

	mX = mY = 1;
	mWidth =  318;
	mHeight = 238;	

	mLeftMargin = 5;
	mTopMargin	= 40;

	strcpy( mszTitle, pTitle );
	SetParam();

	InitStepMode();
	InitFuncKey();
}

CEnmScanContext& CEnmBasePage::GetScanContext() const
{
	return CEnmApp::Instance().GetScanContext();
}

BOOLEAN	CEnmBasePage::IsScanContextUsable() const
{
	return !CEnmApp::Instance().GetScanContext().IsInUse();
}

void CEnmBasePage::OnKeyOK( void )
{
	INT32U aID;
	if ( mpCurCtrl )
	{		
		aID = mpCurCtrl->GetID();
		if ( mMinID <= aID && aID <= mMaxID )
		{				
			SetCurFocusPage( (MigPage*)this->Child( aID - mMinID+1 ) );
		}
	}
}

void CEnmBasePage::OnKeyGO( void )
{
	INT32U aID;
	if ( mpCurCtrl )
	{		
		aID = mpCurCtrl->GetID();
		if ( mMinID <= aID && aID <= mMaxID )
		{				
			SetCurFocusPage( (MigPage*)this->Child( aID - mMinID+1 ) );
		}
	}
}

void CEnmBasePage::OnNumKey( INT16U key )
{
	INT16U aKey = NumKey2Index( GetKeyValue(key) );	
	
	MigControl *apCtrl = GetControlByIndex( aKey );
	if ( apCtrl == NULL )
		return;

	INT32U aID = apCtrl->GetID();

	if ( 0 <= aKey && aKey <= 9 )
	{
		if ( mpCurCtrl && mpCurCtrl->GetType() == ENM_CTRL_LABEL )
			ControlFocusSw( GetControlByIndex( (INT16U)aKey ) );
		
		if ( mMinID <= aID && aID <= mMaxID )
			SetCurFocusPage( (MigPage*)Child( aKey ) );
	}
}

void CEnmBasePage::SetLinkRange( INT32U minID, INT32U maxID )
{
	mMinID = minID; mMaxID = maxID;
}

void	CEnmBasePage::Draw()
{
	MigPage::Draw();
	
	DrawTips();
	DrawFuncKey();
}

BOOLEAN  CEnmBasePage::IsCanForward()
{
	return TRUE;
}

BOOLEAN  CEnmBasePage::IsCanExit()
{
	return TRUE;
}

BOOLEAN  CEnmBasePage::OnForward()
{
//	BOOLEAN bRet = FALSE;
//	IWorkStep *apStep = GetStep();
//
//	if ( apStep )
//	{
//		if ( apStep->GetParent() )
//		{
//			bRet = ((CEnmWfo*)apStep->GetParent())->NextStep();
//		}
//	}
//
//	return bRet;
	
	return StepSwitch( mForwardMode );
}

BOOLEAN  CEnmBasePage::OnExit()
{
//	BOOLEAN bRet = FALSE;
//	IWorkStep *apStep = GetStep();
//
//	if ( apStep )
//	{
//		if ( apStep->GetParent() )
//			bRet = ((CEnmWfo*)apStep->GetParent())->RequestReturn();
//	}
//	
//	return bRet;

	return StepSwitch( mEscMode );
}

BOOLEAN  CEnmBasePage::OnPrint()
{
	return TRUE;
}

BOOLEAN  CEnmBasePage::StepSwitch( enStepMode mode )
{
	BOOLEAN bRet = FALSE;
	switch ( mode )
	{
	case STEP_FW:
		bRet = StepForward();
		break;

	case STEP_BW:
		bRet = StepBackward();
		break;

	case STEP_RETURN:
		bRet = StepReturn();
		break;
	}

	return bRet;
}

BOOLEAN	 CEnmBasePage::StepForward()
{
	BOOLEAN bRet = FALSE;
	IWorkStep *apStep = GetStep();

	if ( apStep )
	{
		if ( apStep->GetParent() )
		{
			bRet = ((CEnmWfo*)apStep->GetParent())->NextStep();
		}
	}

	return bRet;
}

BOOLEAN	 CEnmBasePage::StepBackward()
{
	BOOLEAN bRet = FALSE;

	IWorkStep *apStep = GetStep();
	if ( apStep )
	{
		if ( apStep->GetParent() )
		{
			bRet = ((CEnmWfo*)apStep->GetParent())->PreStep();
		}
	}

	return bRet;
}

BOOLEAN	 CEnmBasePage::StepReturn()
{
	BOOLEAN bRet = FALSE;
	IWorkStep *apStep = GetStep();

	if ( apStep )
	{
		if ( apStep->GetParent() )
			bRet = ((CEnmWfo*)apStep->GetParent())->RequestReturn();
	}
	
	return bRet;
}


BOOLEAN	 CEnmBasePage::SetFuncKey( U8 idx, BOOLEAN bVisible, BOOLEAN bResponse, S32 titleId )
{
	if ( idx > 2 )
		return FALSE;

	mFuncKeyCfg[idx].Set( titleId, bVisible, bResponse );
	return TRUE;
}

void  CEnmBasePage::InitFuncKey()
{
	mFuncKeyTop = 46 + 20 * 8;

	mbShowFuncKey = FALSE;
	mFuncKeyCfg[0].Set( STR_STEP_GO,TRUE, TRUE );
	mFuncKeyCfg[1].Set( STR_STEP_EXIT, TRUE, TRUE );
	mFuncKeyCfg[2].Set( STR_STEP_HCOPY, FALSE, FALSE );
}

void  CEnmBasePage::DrawFuncKey()
{
	if ( !IsShowFuncKey() )
		return;


	U8 aVisibleCnt = 0;
	U8 aVisibleIdx = 0;
	for ( U8 i = 0; i < 3; i++ )
	{
		if ( mFuncKeyCfg[i].mbShow )
		{
			aVisibleCnt++;
			aVisibleIdx = i;
		}
	}

	MCHAR *pTitle;
	U16 aCurPos = mLeftMargin + 10;
	U16 aTitleWidth = 0;
	if ( aVisibleCnt == 1 )
	{
		if ( pTitle = MStrMngr::Instance().GetString( mFuncKeyCfg[aVisibleIdx].mTitleID ) )
		{
			aTitleWidth = strlen( pTitle ) * 8;
			aCurPos = (GetFrameW() - aTitleWidth) / 2;
			PutStr( aCurPos, mFuncKeyTop, pTitle, 0 );
		}
	}
	else
	{
		for ( U8 i = 0; i < 3; i++ )
		{
			if ( mFuncKeyCfg[i].mbShow )
			{
				pTitle = MStrMngr::Instance().GetString( mFuncKeyCfg[i].mTitleID );
				if ( pTitle )
				{
					aTitleWidth = strlen( pTitle ) * 8;
					PutStr( aCurPos, mFuncKeyTop, pTitle, 0 );
					aCurPos += (aTitleWidth + 16);
				}
			}
		}
	}

}


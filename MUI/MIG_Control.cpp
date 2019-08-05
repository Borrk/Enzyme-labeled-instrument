
#include "MIG_Control.hpp"
#include "Mig_Page.hpp"

BOOLEAN MigControl::mErasable = TRUE;
INT32U	MigControl::mCtrlID = (INT32U)-1;
MigControl::MigControl( MigObj *pParent )
{
	Init();
	
	SetParent( pParent ); 
}

MigControl::MigControl( const tagCS& cs )
:MigObj( cs )
{
	Init();
	mID = cs.id;
	mStyle = cs.dwStyle;
	SetParent( (MigObj*)cs.hParent );
};

MigControl::~MigControl()
{
	
}

void	MigControl::Init(void)
{
	mType    = ENM_CTRL_LABEL;

	mbDocked = FALSE;	///< flag, when add to a page, docked, otherwise undocked
	mbNotify = FALSE;	///< flag, when content change, notify its parent 

	mbErase  = TRUE;

	mStyle = ENM_CTRL_NORMAL;
	mID = mCtrlID--;
}

void	MigControl::SetParent( MigObj* pParent )
{
	if ( pParent )
	{
		mpParent = pParent;
		mbDocked = TRUE;
	}
}

void	MigControl::SetChild( MigObj* pChild )
{
	// do nothing.
	// controls don't have child
}

void	MigControl::Enable( BOOLEAN bEnable )
{
	MigObj::Enable( bEnable );
#if 1
	if ( bEnable )
	{
		if (mpParent && ((MigPage*)mpParent)->GetCurCtrl() == NULL )
			((MigPage*)mpParent)->ControlFocusSw( this );
	}
	else
	{
		if (mpParent && ((MigPage*)mpParent)->GetCurCtrl() == this )
			((MigPage*)mpParent)->ControlFocusSw( (MigControl*)mpNext );
	}
#endif

}

BOOLEAN	 MigControl::IsActive(void) 
{ 
	BOOLEAN	aIsActive = TRUE;

	if ( mStyle & ENM_CTRL_STATIC) 
	{
		aIsActive = FALSE;
	}
	else if ( mStyle & ENM_CTRL_DISABLE )
	{
		aIsActive = FALSE;
	}
	else if ( mStyle & ENM_OBJ_HIDEN )
	{
		aIsActive = FALSE;
	}

	return aIsActive;
}

void	MigControl::Draw(void)
{
	if ( NULL == mpParent )
		return;

	/// Refresh client
	if ( mbErase )
	{
		PutBox( mpParent->mX + mX, mpParent->mY + mY, mWidth, mHeight );
		mbErase = FALSE;
	}

	if ( mStyle & ENM_OBJ_HIDEN )
		return;

	if ( mStyle & ENM_CTRL_DISABLE )
		return;

	DrawFrame( );

	if ( mStyle & ENM_OBJ_TITLE )
	{
		DrawTitle( );
	}
}

/// Draw routines
void	MigControl::DrawTitle(void)
{
	MCHAR *apTitle;
	if ( mTitleID == MIG_STR_ID_INVALID )
		apTitle = mszTitle;
	else
		apTitle = MLoadStr(mTitleID);

	// temp code
	if ( mStyle & ENM_CTRL_FOCUSED )
	{
		if ( mStyle & ENM_CTRL_IN_EDIT)
			PutStr( mpParent->mX + mX+8, mpParent->mY + mY+2, apTitle, 0 );
		else
			PutStr( mpParent->mX + mX+8, mpParent->mY + mY+2, apTitle, 1 );
	}
	else
	{
		PutStr( mpParent->mX + mX+8, mpParent->mY + mY + 2, apTitle, 0 );
	}

	/// if is focused, draw focus frame
	if ( this->IsFocusOn() )
	{
	// don't draw now
	//	PutRectV( mpParent->mX + mX, mpParent->mY + mY, strlen(mszTitle) * GetFontW(), mHeight, 0 );
	}
}

void	MigControl::DrawFrame(void)
{
	if ( mStyle & ENM_OBJ_FRAME )
	{
		PutRect( mpParent->mX + mX, mpParent->mY + mY, mWidth, mHeight );
	}
	else
	{
		PutBox( mpParent->mX + mX, mpParent->mY + mY, mWidth, mHeight );
	}
	
	/// if is focused, draw focus frame
	if ( this->IsFocusOn() )
	{
//		PutRectV( mpParent->mX + mX, mpParent->mY + mY, mWidth, mHeight, 0 );
	}
}

/// Message proc
void	MigControl::OnMessage( const tagMigMSG& stMsg )
{
	if ( !this->IsActive( ) )
		return;

	INT32U aMsg = stMsg.msg;
	switch ( aMsg )
	{
	case ENM_MSG_KEY:
		OnKey( (INT16U)stMsg.param1 );
		break;
	case ENM_MSG_PAINT:
		Draw();
		break;
    default:
		break;
	}

}

void	MigControl::FocusOn(void)
{
	mStyle |= ENM_CTRL_FOCUSED;
}

void	MigControl::FocusOff(void)
{
	mStyle &= ~ENM_CTRL_FOCUSED;
	OutEditMode();
}



//////////////////  MigUpDn ///////////////
MigUpDn::MigUpDn( MigObj* pParent )
	:MigControl( pParent )
{
	Init();
}

MigUpDn::MigUpDn( const tagCS& cs )
:MigControl( cs )
{
	Init( );
};

MigUpDn::~MigUpDn()
{
	ReleaseMem();
}

void	MigUpDn::ReleaseMem( void )
{

	//MCHAR *apItem;
	if ( mppszItem )
	{
/*		for ( INT16U i = 0; i < mItemMax; i++ )
		{
			apItem = mppszItem[i];
			delete []apItem;
		}
*/
//		delete []mppszItem;
	}

}

void	MigUpDn::Init(void)
{
	mbCycled  = TRUE;	///< Item switch cyclely
	mItemType = MIG_UD_INT;	///< MIG_UD_INT, MIG_UD_STR
	mCurSel	  = 0;	
	mItemMax  = 0;	

	mMinVal	  = 0;
	mMaxVal	  = 0;
	mStep     = 1;

	memset( mItemId, 0, sizeof(mItemId) );
	memset( mppszItem, 0, sizeof(mppszItem) );
	mbStringIdx = FALSE;
}

INT16U	MigUpDn::GetCurSel( void )
{
	return mCurSel;
}

INT16U	MigUpDn::GetTotalItem( void )
{
	return mItemMax;
}

void	MigUpDn::SetCurSel( INT16U sel )
{
	if ( sel < mItemMax )
	{
		mCurSel = sel;
		Update();
	}
}

MCHAR*   MigUpDn::GetItemStr( U16 idx )
{
	if ( idx >= mItemMax )
		return NULL;

	MCHAR* apCurItem;
	if ( mbStringIdx )
		apCurItem = MStrMngr::Instance().GetString( mItemId[idx] );
	else
		apCurItem = mppszItem[idx];
	
	return apCurItem;
}
BOOLEAN	MigUpDn::SetItems( INT16U* pItems, INT16U total )
{
	if ( pItems == NULL && total > 0 )
		return FALSE;

	MStrMngr& rMngr = MStrMngr::Instance();
	INT16U	aItemSum = 0;
	for ( INT16U i = 0; i < total; i++ )
	{
		if ( rMngr.GetString( pItems[i] ) )
		{		
			mItemId[aItemSum] = pItems[aItemSum];
				aItemSum++;			
		}
	}

	mItemMax  = aItemSum;
	mCurSel   = 0;

	mItemType = MIG_UD_STR;	
	mbStringIdx = TRUE;
	
	return TRUE;
}

BOOLEAN	MigUpDn::SetItems( MCHAR** pszItems, INT16U total )
{
	if ( pszItems == NULL && total > 0 )
		return FALSE;

	/// Release memory 
	if ( mppszItem )
	{
		ReleaseMem();
	}

	INT16U	aItemSum = 0;
	for ( INT16U i = 0; i < total; i++ )
	{
		if ( *pszItems && **pszItems != '\0' )
		{		
			mppszItem[aItemSum] = mszItem[aItemSum];
			strcpy( mszItem[aItemSum], *pszItems++ );
				aItemSum++;			
		}
	}

	mItemMax  = aItemSum;
	mCurSel   = 0;

	mItemType = MIG_UD_STR;	
	
	mbStringIdx = FALSE;

	return TRUE;
}

BOOLEAN MigUpDn::SetItemInt( INT16U minVal, INT16U maxVal, INT16U step )
{
	if ( minVal >= maxVal )
		return FALSE;

	mMinVal	  = minVal;
	mMaxVal	  = maxVal;
	mStep     = step;

	mItemMax  = mMaxVal - mMinVal + 1;
	mCurSel   = 0;

	mItemType = MIG_UD_INT;	
	
	return TRUE;
}

void MigUpDn::Draw(void)
{
	if ( mStyle & ENM_OBJ_HIDEN )
		return;
	if  ( FALSE == mbDocked )
		return;

	DrawFrame( );

	if ( mStyle & ENM_OBJ_TITLE )
	{
		DrawTitle( );
	}

	DrawBoxText( );	
}

void MigUpDn::DrawBoxText()
{
	static INT16U aShow = 0;
	
	MCHAR *apCurItem;
	MCHAR aIntItem[32];
	if ( mItemType == MIG_UD_INT )
	{
		sprintf( aIntItem, "%d", mMinVal + mCurSel * mStep );
		apCurItem = aIntItem;
	}
	else
	{
		if ( mbStringIdx )
			apCurItem = MStrMngr::Instance().GetString( mItemId[mCurSel] );
		else
			apCurItem = mppszItem[mCurSel];
	}

	if ( mStyle & ENM_CTRL_DISABLE )
	{
		// temp code
		PutStr( BoxPosX() + 2, BoxPosY(),  apCurItem, 0 );	
	}
	else
	{
		// Being Focus On
		if ( IsFocusOn() )
		{
			if ( IsInEdit() )
			{
				if ( aShow > ENM_BLACK_TIME/2 )
				{
					PutStr( BoxPosX(), BoxPosY(),  apCurItem, 1 );	
					if ( aShow > ENM_WHITE_TIME/2 )
						aShow = 0;
				}
				else
				{
					PutStr( BoxPosX(), BoxPosY(),  apCurItem, 0 );	
				}
				aShow ++;
			}
			else
			{
				PutStr( BoxPosX(), BoxPosY(),  apCurItem, 1 );	
			}
//			PutStr( BoxPosX(), BoxPosY(),  apCurItem, 1 );	
		}
		else
		{
			PutStr( BoxPosX(), BoxPosY(),  apCurItem, 0 );	
		}
	}
}


void	MigUpDn::DrawTitle(void)
{
	//PutStr( mpParent->mX + mX, mpParent->mY + mY, mszTitle );	
}

void	MigUpDn::DrawFrame(void)
{
	/// Clear box's client
	PutBox( BoxPosX(), BoxPosY(), mWidth + 4, mHeight );

	MigControl::DrawFrame();
	
	/// Draw Up/Dn Icon
	if ( mStyle & ENM_CTRL_THINK )	// only underscore
	{
		//  no need 2005-03-27
	//	PutLine( BoxPosX(), BoxPosY() + mHeight, mpParent->mX + mX + mWidth, mpParent->mY + mY + mHeight);
	}
	else
	{
		// add later
		PutRect( BoxPosX(), BoxPosY(), mWidth -2, mHeight );
	}
}

void	MigUpDn::FocusOn(void)
{
	MigControl::FocusOn();

//	InEditMode();
}

void	MigUpDn::FocusOff(void)
{
	MigControl::FocusOff();

	OutEditMode();

}

void	MigUpDn::OnKeyUp( void )
{
	if ( !this->IsInEdit( ) )
		return;

	if ( mCurSel < mItemMax - 1 )
	{
		mCurSel++;
	}
	else
	{
		if ( mbCycled)
		{
			mCurSel = 0;
		}
	}

	Update();
}

void	MigUpDn::OnKeyDown( void )
{
	if ( !this->IsInEdit( ) )
		return;

	if ( mCurSel > 0 )
	{
		mCurSel--;
	}
	else
	{
		if ( mbCycled)
		{
			mCurSel = mItemMax - 1;
		}
	}
	
	Update();
}

void	MigUpDn::OnKeyESC( void )
{
	OutEditMode();
}

void	MigUpDn::OnKeyOK( void )
{
	if ( IsInEdit() )
		OutEditMode();
	else
		InEditMode();
}

//////////////////////////////////////////////////////////////////////////////
/// 
///   Radio box 
///
//////////////////////////////////////////////////////////////////////////////

MigRadioBox::MigRadioBox( MigObj* pParent )
	:MigControl( pParent )
{
	Init();
}

MigRadioBox::MigRadioBox( const tagCS& cs )
:MigControl( cs )
{
	Init( );
};

MigRadioBox::~MigRadioBox()
{
	
}

void	MigRadioBox::Init(void)
{
	mbChecked = FALSE;
	mBoxWidth = 12;
}

BOOLEAN	 MigRadioBox::GetState( void )
{
	return mbChecked;
}

void	MigRadioBox::SetState( BOOLEAN bCheck )
{
	mbChecked = bCheck;
}

void	MigRadioBox::OnKeyOK( void )
{
	mbChecked = !mbChecked;
	Refresh();
//	Update( );
}

void	MigRadioBox::Draw(void)
{
	if ( NULL == mpParent )
		return;

	/// Refresh client
	if ( mbErase )
	{
		PutBox( mpParent->mX + mX, mpParent->mY + mY, mWidth, mHeight );
		mbErase = FALSE;
	}

	if ( mStyle & ENM_OBJ_HIDEN )
		return;

	if ( mStyle & ENM_CTRL_DISABLE )
		return;

	DrawFrame( );

	if ( mStyle & ENM_OBJ_TITLE )
	{
		DrawTitle( );
	}

	DrawCheckBox();
}

/// Draw routines
void	MigRadioBox::DrawTitle(void)
{
	MCHAR *apTitle;
	if ( mTitleID == MIG_STR_ID_INVALID )
		apTitle = mszTitle;
	else
		apTitle = MLoadStr(mTitleID);

	if ( mStyle & ENM_CTRL_FOCUSED )
	{
		if ( mStyle & ENM_CTRL_IN_EDIT)
			PutStr( mpParent->mX + mX+8 + mBoxWidth, mpParent->mY + mY+2, apTitle, 0 );
		else
			PutStr( mpParent->mX + mX+8 + mBoxWidth, mpParent->mY + mY+2, apTitle, 1 );
	}
	else
	{
		PutStr( mpParent->mX + mX+8 + mBoxWidth, mpParent->mY + mY + 2, apTitle, 0 );
	}

	/// if is focused, draw focus frame
	if ( this->IsFocusOn() )
	{
	// don't draw now
	//	PutRectV( mpParent->mX + mX, mpParent->mY + mY, strlen(mszTitle) * GetFontW(), mHeight, 0 );
	}
}

void	MigRadioBox::DrawFrame(void)
{
	if ( mStyle & ENM_OBJ_FRAME )
	{
		PutRect( mpParent->mX + mX, mpParent->mY + mY, mWidth, mHeight );
	}
	else
	{
		//PutBox( mpParent->mX + mX, mpParent->mY + mY, mWidth, mHeight );
	}
	
	/// if is focused, draw focus frame
	if ( this->IsFocusOn() )
	{
//		PutRectV( mpParent->mX + mX, mpParent->mY + mY, mWidth, mHeight, 0 );
	}
}

void	MigRadioBox::DrawCheckBox(void)
{
	if ( mbChecked )
	{
		PutStr( mpParent->mX + mX+6, mpParent->mY + mY+3, "X", 0  );
	}
	else
	{
		PutStr( mpParent->mX + mX+6, mpParent->mY + mY+4, " ", 0  );
	}
	PutRect( mpParent->mX + mX + 3, mpParent->mY + mY + 3, mBoxWidth, mHeight-8 );
}

//////////////////////////////////////////////////////////////////////////////
/// 
///   Radio box 
///
//////////////////////////////////////////////////////////////////////////////

EnmComb::EnmComb( MigObj* pParent )
	:MigControl( pParent )
{
	Init();
}

EnmComb::EnmComb( const tagCS& cs )
:MigControl( cs )
{
	Init( );
};

EnmComb::~EnmComb()
{
	if ( mCellInfo.stateCnt > 0 )
	{
		delete []mCellInfo.pTag;
		mCellInfo.pTag = NULL;

		if ( mpCellState )
			delete []mpCellState;	
	}
}

void	EnmComb::Init(void)
{
	mpCellState = NULL;
	mCurLn = 0;
	mCurCol = 0;
	
	mStep = 20;
	mTopPos = 0;
	mBottomPos = 0;

	memset( &mCellInfo, 0, sizeof(mCellInfo) );
}

INT8U	 EnmComb::GetState( INT32U ln, INT32U col  )
{
	if ( mCellInfo.stateCnt <= 0 )
		return 0;
	else
		return mpCellState[ln * mCellInfo.colCnt + col];
}

void	EnmComb::SetState( INT32U ln, INT32U col, INT8U state )
{
	if ( mCellInfo.stateCnt > 0 )
		mpCellState[ln * mCellInfo.colCnt + col] = state;
}

void	EnmComb::SetAllState( INT8U state )
{
	if ( mCellInfo.stateCnt <= 0 )
		return;

	for ( INT32U i = 0; i < mCellInfo.lnCnt * mCellInfo.colCnt; i++ )
	{
			mpCellState[i] = state;
	}
}

void	EnmComb::SetCellInfo( const tagCombCellInfo& cci )
{
	if ( mCellInfo.stateCnt > 0 )
	{
		delete []mCellInfo.pTag;
		mCellInfo.pTag = NULL;

		if ( mpCellState )
			delete []mpCellState;	
	}

	mCellInfo.lnCnt = cci.lnCnt;
	mCellInfo.colCnt = cci.colCnt;
	mCellInfo.width = cci.width;
	mCellInfo.height = cci.height;
	mCellInfo.stateCnt = cci.stateCnt;
	mCellInfo.pTag = new INT8S[cci.stateCnt];
	memcpy( mCellInfo.pTag, cci.pTag, cci.stateCnt );

	mpCellState = new INT8U[cci.lnCnt*cci.colCnt];
	SetAllState( 0 );
}

void	EnmComb::OnKeyOK( void )
{
	if ( mCellInfo.stateCnt <= 0 )
		return;

	INT8U aCurState = mpCellState[mCurLn * mCellInfo.colCnt+mCurCol];
	if ( aCurState == mCellInfo.stateCnt - 1 )
		aCurState = 0;
	else
		aCurState++;

	SetState( mCurLn, mCurCol, aCurState );
}

void	EnmComb::Draw(void)
{
	if ( NULL == mpParent )
		return;

	/// Refresh client
	if ( mbErase )
	{
		PutBox( mpParent->mX + mX, mpParent->mY + mY, mWidth, mHeight );
		mbErase = FALSE;
	}

	if ( mStyle & ENM_OBJ_HIDEN )
		return;

	if ( mStyle & ENM_CTRL_DISABLE )
		return;

	DrawFrame( );

	DrawTips();
	DrawAllCells();

}

/// Draw routines
void	EnmComb::DrawAllCells(void)
{
	for ( INT32U i = 0; i < mCellInfo.lnCnt; i++ )
	{
		for ( INT32U j = 0; j < mCellInfo.colCnt; j++ )
		{
			DrawCell( i,j );
		}
	}

	DrawCurSel( 0 );
}

void	EnmComb::DrawCell( INT32U ln, INT32U col )
{
	if ( mCellInfo.stateCnt <= 0 )
		return;

	INT8S aCh[4] = {' ', 0 };
	INT8U type = mpCellState[ln*mCellInfo.colCnt+col];
	if ( type < mCellInfo.stateCnt )
		aCh[0] = mCellInfo.pTag[type];

	INT32S x = 26 + mpParent->mX + mX + mStep * col;
	INT32S y = 20 + mpParent->mY + mY + 20 * ln;
	PutStr( x, y, (char*)aCh, 0 );
}

void  EnmComb::DrawCurSel( BOOLEAN bTrans )
{
	INT16U	aWidth = 0, aHeight = 0;

	/// Individual cell 
	INT32S x = 18 + mpParent->mX + mX + mStep * mCurCol;
	INT32S y = 18 + mpParent->mY + mY + 20 * mCurLn;
	aWidth	= mStep - 2;
	aHeight = mStep - 2;

	::PutRectV( x, y, aWidth, aHeight, bTrans );
}

void  EnmComb::DrawTips( void )
{
	MCHAR aTips[4];
	for ( short i = 0; i < mCellInfo.colCnt; i++ )
	{
		sprintf( aTips, "%2d", i+1 );
		PutStr( 20 + mpParent->mX + mX + mStep * i, mpParent->mY + mY, aTips, 0 );
	}

	aTips[1] = '\0';
	for ( short j = 0; j < mCellInfo.lnCnt; j++ )
	{
		aTips[0] = 'A' + j;
		PutStr( mpParent->mX + mX-2, 20 + mpParent->mY + mY + 20 * j, aTips, 0 );
	}
	
}

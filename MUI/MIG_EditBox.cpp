#include "MIG_OS_Base.hpp"
#include <math.h>
#include <stdlib.h>
#include "MIG_EditBox.hpp"
#include "Mig_Page.hpp"

MCHAR  MigEditBox::mEditBuf[ENM_TITLE_MAX] = {'\0'};
MCHAR  MigEditBox::mCharTable[CHAR_TABLE_MAX] = { '0', '1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','+','-','*','/','=','(',')' };

MigEditBox::MigEditBox( MigObj* pParent, INT8U type )
	:MigControl( pParent )
{
	mCurTablePos = 0;
	mType = type;	
}

MigEditBox::MigEditBox( const tagEditBoxCS& eCs, MigObj* pParent )
	:MigControl( pParent )
{
	Init( eCs );
}

MigEditBox::MigEditBox( const tagCS& cs, const tagEditBoxCS& eCs )
:MigControl( cs )
{
	Init( eCs );
	//mStyle = cs.dwStyle;
};

MigEditBox::~MigEditBox()
{
	
}

void	MigEditBox::Init( const tagEditBoxCS& eCs )
{
	mCurTablePos = 0;

	mBoxType    = eCs.type;
	mType		= eCs.type;
	mTitleWidth	= eCs.titleWidth;	///< Title's width in the whole editbox

	memset( mBuf, 0, sizeof(mBuf) ); 
	mCursorPos	= 0; ///< cursor's pos in editbox
	mStrSize	= 0;	///< Char's count in editbox

	mbChanged	= FALSE;	///< flag, for Editbox's contant changing

	mbMultiKeyPress = TRUE;

	switch ( mBoxType )
	{
	case ENM_EB_TYPE_CHAR:
		InitCh( eCs );
		break;

	case ENM_EB_TYPE_INT:
		InitInt( eCs );
		break;
	case ENM_EB_TYPE_FLOAT:
		InitFP( eCs );
		break;
	}

	/// calculate max char size edit box can receive
	mMaxChar = (mWidth - mTitleWidth)/ 8;
}

void	MigEditBox::InitCh(const tagEditBoxCS& eCs)
{
	mMaxChar = eCs.maxChar;	///< Limited char's count	
	strcpy( mBuf, eCs.szVal );
	DDVCh();
}

void	MigEditBox::InitInt(const tagEditBoxCS& eCs)
{
	mData.iMaxVal = eCs.maxVal;
	mData.iMinVal = eCs.minVal;
	mData.iVal = eCs.iVal;

	DDVInt();
	//mMaxChar = CalCharRangeInt();
	mMaxChar = CalRangeInt( eCs.maxVal, eCs.minVal );
	Int2Str( mData.iVal, mBuf );
}

void	MigEditBox::InitFP(const tagEditBoxCS& eCs)
{
	mData.fMaxVal = eCs.fmaxVal;
	mData.fMinVal = eCs.fminVal;
	mData.fVal	  = eCs.fVal;

	DDVFP();
	mPrecision = eCs.maxChar;
	//mMaxChar = mPrecision + CalCharRangeFP();
	mMaxChar = mPrecision + CalRangeFP( eCs.fmaxVal, eCs.fminVal );
	FP2Str( mData.fVal, mBuf );
}

void MigEditBox::Draw(void)
{
	/// Refresh client
	if ( mbErase )
	{
		PutBox( mpParent->mX + mX, mpParent->mY + mY, mWidth, mHeight );
		mbErase = FALSE;
	}

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

/// Draw page routines
void	MigEditBox::DrawTitle(void)
{
	if ( mTitleID == MIG_STR_ID_INVALID )
		PutStr( mpParent->mX + mX, mpParent->mY + mY+2, mszTitle, 0 );	
	else
		PutStr( mpParent->mX + mX, mpParent->mY + mY+2, MLoadStr(mTitleID), 0 );	
}

void	MigEditBox::DrawFrame(void)
{
	// temp code
	MigControl::DrawFrame();
	
	/// clear box's client
//	PutBox( BoxPosX(), BoxPosY(), mWidth - mTitleWidth, mHeight );
	
	if ( mStyle & ENM_CTRL_THINK )	// only underscore
	{
	//	PutLine( BoxPosX(), BoxPosY() + mHeight, mpParent->mX + mX + mWidth, mpParent->mY + mY + mHeight);
	}
	else
	{
		// add later
	//	PutRect( BoxPosX(), BoxPosY(), mWidth - mTitleWidth-2, mHeight-2 );
	}		

	/// if is focused, draw focus frame
	if ( this->IsFocusOn() )
	{
		//PutRectV( BoxPosX(), BoxPosY(), mWidth - mTitleWidth-2, mHeight, 0 );
	}

}

void MigEditBox::DrawBoxText()
{
	MCHAR  aBuf[ENM_TITLE_MAX] = { 0 };

	static INT16U aShow = 0;
	
	if ( mStyle & ENM_CTRL_DISABLE )
	{
		// temp code
		PutStr( BoxPosX(), BoxPosY(),  mBuf, 0 );	
	}
	else
	{
		// Being Focus On
		if ( IsFocusOn() ) 
		{
#if 1
			if ( IsInEdit()	)
			{
				strncpy( aBuf, mEditBuf, sizeof(mEditBuf) );
				if ( aBuf[0] == '\0')
				{
					aBuf[0] = ' ';
					aBuf[1] = '\0';
				}
				
				/// test for cur pos reverse display
				MCHAR *p1, *p2, *p3;
				S32 x = BoxPosX();
				
				MCHAR aTempBuf[ENM_TITLE_MAX] = { 0 };
				strncpy( aTempBuf, aBuf, mCursorPos );			
				PutStr( x, BoxPosY(), aTempBuf, 0 );
				x += strlen(aTempBuf) * 8;
			
				strncpy( aTempBuf, &aBuf[mCursorPos], 1 );	
				aTempBuf[1] = '\0';
				PutStr( x, BoxPosY(), aTempBuf, 1 );
				x += 8;
			
				strcpy( aTempBuf, &aBuf[mCursorPos+1] );
				PutStr( x, BoxPosY(), aTempBuf, 0 );
				
//				if ( aShow >= ENM_BLACK_TIME )
//				{
//					PutStr( BoxPosX(), BoxPosY(),  aBuf, 1 );
//				//	PutStr( BoxPosX(), BoxPosY(),  mEditBuf, 1, mCursorPos );
//					if ( aShow > ENM_WHITE_TIME )
//						aShow = 0;
//				}
//				else
//				{
//					PutStr( BoxPosX(), BoxPosY(),  aBuf, 0 );
//				//	PutStr( BoxPosX(), BoxPosY(),  mEditBuf, 0, mCursorPos );
//				}
				
				aShow ++;
			}
			else
			{
				PutStr( BoxPosX(), BoxPosY(),  mBuf, 1 );
			}
			
//			if ( IsInEdit()	)
//				PutStr( BoxPosX(), BoxPosY(),  mEditBuf, 1, mCursorPos );
//			else
//				PutStr( BoxPosX(), BoxPosY(),  mBuf, 1, mCursorPos );
#else
			// temp code
			// draw text before cursor
			strncpy( aBuf, mEditBuf, mCursorPos );
			aBuf[mCursorPos] = '\0';
			PutStr( BoxPosX(), BoxPosY(),  aBuf, 0 );	
		
			// draw cursor
			strcpy( aBuf, "|" );
			PutStr( BoxPosX() + mCursorPos*8, BoxPosY(),  aBuf, 0 );	
		
			// draw text after cursor
			strncpy( aBuf, &mEditBuf[mCursorPos], mStrSize - mCursorPos );
			aBuf[mStrSize - mCursorPos] = '\0';
			PutStr( BoxPosX() + mCursorPos*8, BoxPosY(),  aBuf, 0 );	
#endif
		}
		else
		{
			// temp code
			PutStr( BoxPosX(), BoxPosY(),  mBuf, 0 );	
		}
	}
}


void	MigEditBox::OnMessage( const tagMigMSG& stMsg )
{
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

void  MigEditBox::OnKey( INT16U key )
{
	INT16U aKey;

	switch( GetKeyValue(key) )
	{
		case KEY_0_N:
		case KEY_1_A:
		case KEY_2_B:
		case KEY_3_C:
		case KEY_4_D:
		case KEY_5_E:
		case KEY_6_F:
		case KEY_7_G:
		case KEY_8_H:
		case KEY_9_P:
		case KEY_Sign:
		case KEY_DOT_L:
			if ( IsInEdit() )
			{
				InputKeyProc( key );
			}
			break;

		case KEY_Right:
			CursorMove( 1 );	///< forward
			Update();
			break;

		case KEY_Left:
			CursorMove( -1 );  ///< backward
			Update();
			break;

		case KEY_Up:
			if ( IsInEdit() )
			{
				aKey = mCharTable[mCurTablePos];
				InputKeyProc( aKey, FALSE );
				mCurTablePos = (++mCurTablePos) % CHAR_TABLE_MAX;
			}

			break;
		case KEY_Down:
			if ( IsInEdit() )
			{
				aKey = mCharTable[mCurTablePos];
				InputKeyProc( aKey, FALSE );
				if ( mCurTablePos <= 0 )
					mCurTablePos = CHAR_TABLE_MAX - 1;
				else
					mCurTablePos--;
			}
			break;

		case KEY_OK:
			OnKeyOK();
			break;

		case KEY_ESC:
			OutEditMode();
			Flush( FALSE );
			break;

		case KEY_TEST:
			break;
		case KEY_PRG:
			break;
		case KEY_Par:
			break;
		case KEY_Data:
			break;
		case KEY_Prn:
			break;
		default:
			break;
	}
}

void MigEditBox::OnKeyOK( void )
{
	if ( IsInEdit() )
	{
		Flush();
		OutEditMode();
	}
	else
	{
		InEditMode();
		Flush( FALSE );
	}
}

INT16S FindChr( char *pStr, INT16U maxSize, MCHAR ch )
{
	INT16S i = 0;
	while( i < maxSize )
	{
		if( *(pStr + i) == ch )
		{
			break;
		}
		i++;
	}

	return ( i < maxSize) ? i : -1;
}

void MigEditBox::InputKeyProc(INT16U key, BOOLEAN bCurMove )
{
	MCHAR aCh;
	INT8U aKeyCnt  = 0;
	INT16U aKeyVal = GetKeyValue(key);

	if ( mCursorPos >= mMaxChar )	///< buf full
		return;

	if ( ENM_EB_TYPE_INT == mBoxType )
	{	
		aCh = Key2Char( aKeyVal, aKeyCnt );
		if ( !( aCh >= '0' && aCh <= '9') )
			return;

		if ( mCursorPos < mStrSize )
		{
		}
		else
		{
			if ( bCurMove )
			{			
				mStrSize++;
			}
		}

		mEditBuf[mCursorPos] = aCh;

		if ( (mCursorPos < mMaxChar-1) && bCurMove )
			mCursorPos++;

		mbErase = TRUE;
		mbChanged = TRUE;

		Update();
	}
	else if ( ENM_EB_TYPE_FLOAT == mBoxType )
	{	
		INT16S aPos;
		aCh = Key2Char( aKeyVal, aKeyCnt );
		if ( aCh != 0 )
		{

			if ( aCh == '.' )
			{
				if ( mCursorPos == 0 )
					return;

				if ( (aPos = FindChr( mEditBuf, mStrSize, '.' )) != -1 )
				{
					if ( aPos < mCursorPos ) /// if dot has existed, do nothing
					{
						return;
					}
					else	/// replace dot with '0'
					{
						mEditBuf[aPos] = '0';
					}
				}
			}

			if ( aCh == '-' )
			{
				if ( mCursorPos != 0 )
					return;

				aPos = FindChr( mEditBuf, mStrSize, '-' );
				if ( aPos != -1 )
				{
					return;
				}
			}

			mEditBuf[mCursorPos] = aCh;
			if ( bCurMove )
			{			
				mStrSize++;
				mCursorPos++;
			}

			mbErase = TRUE;
			mbChanged = TRUE;
			Update();
		}
	}
	else
	{
		aKeyCnt = mbMultiKeyPress ? GetKeyCount(key) : 0;	
		aCh = Key2Char( aKeyVal, aKeyCnt );

		/// repeat input, cursor not move
		if ( aKeyCnt > 0 )
			CursorMove( -1 );

		if ( aCh != 0 )
		{
			if ( mCursorPos < mStrSize )
			{
			}
			else
			{
				if ( bCurMove )
				{			
					mStrSize++;
				}
			}

			mEditBuf[mCursorPos] = aCh;

			if ( (mCursorPos < mMaxChar-1) && bCurMove )
				mCursorPos++;

			mbErase = TRUE;
			mbChanged = TRUE;

			Update();
		}
	}
}

void	MigEditBox::CursorMove( INT16S step )
{
	INT16U aPos = mCursorPos;

	if ( step > 0 )	///< right move
	{
		mCursorPos += 1; //min( step, (mStrSize - mCursorPos - 1) );

		if ( ENM_EB_TYPE_FLOAT == mBoxType )
		{
			if ( mEditBuf[mCursorPos] == '.' )
			{
				mCursorPos++;
			}
		}
	}
	else if ( step < 0 )	///< left move
	{
		mCursorPos -= min( -step,  mCursorPos );

		if ( ENM_EB_TYPE_FLOAT == mBoxType )
		{
			if ( mEditBuf[mCursorPos] == '.' )
			{
				mCursorPos--;
			}
		}
	}

	/// Move to new pos, set curtable pos to 0
	if ( aPos != mCursorPos )
	{
		mCurTablePos = 0;
	}

	Update();
}

void	MigEditBox::FocusOn(void)
{
	MigControl::FocusOn();

	// EditBox's special actions
	memset( mEditBuf, 0, sizeof(mEditBuf) );
	//strcpy( mEditBuf, mBuf );

	Flush( FALSE );

	mCursorPos = 0;
	mStrSize = strlen(mEditBuf);
	mbChanged = FALSE;
}

void	MigEditBox::FocusOff(void)
{
	MigControl::FocusOff();

	// EditBox's special actions
	OutEditMode();
	if ( mbChanged == TRUE )
		Flush();

	memset( mEditBuf, 0, sizeof(mEditBuf) );
}

void MigEditBox::DDV(void)
{
	switch ( mBoxType )
	{
	case ENM_EB_TYPE_CHAR:
		DDVCh();
		break;

	case ENM_EB_TYPE_INT:
		DDVInt();
		break;
	case ENM_EB_TYPE_FLOAT:
		DDVFP();
		break;
	}
}

void MigEditBox::DDVCh(void)
{
	if ( mStrSize > mMaxChar )
	{
		mBuf[mMaxChar] = '\0';
	}
}

void MigEditBox::DDVInt(void)
{
	if ( mData.iVal > mData.iMaxVal )
	{
		mData.iVal = mData.iMaxVal;
	}

	if ( mData.iVal < mData.iMinVal )
	{
		mData.iVal = mData.iMinVal;
	}
}

void MigEditBox::DDVFP(void)
{
	if ( mData.fVal > mData.fMaxVal )
	{
		mData.fVal = mData.fMaxVal;
	}

	if ( mData.fVal < mData.fMinVal )
	{
		mData.fVal = mData.fMinVal;
	}
}

void	MigEditBox::Flush(BOOLEAN bToVal )
{
	if ( !mbChanged )
	{
		bToVal = FALSE;
	}

	switch ( mBoxType )
	{
	case ENM_EB_TYPE_CHAR:
		FlushCh( bToVal );
		break;

	case ENM_EB_TYPE_INT:
		FlushInt( bToVal );
		break;
	case ENM_EB_TYPE_FLOAT:
		FlushFP( bToVal );
		break;
	}

	mCursorPos = 0;
	mStrSize = strlen(mBuf);	
	mbChanged = FALSE;
}

void	MigEditBox::FlushCh(BOOLEAN bToVal)
{
	if ( bToVal )
	{
		strcpy( mBuf, mEditBuf );
		DDVCh();
		strcpy( mEditBuf, mBuf );
	}
	else
	{
		memset( mEditBuf, 0, sizeof(mEditBuf));
		//strcpy( mEditBuf, mBuf );
	}
}

void	MigEditBox::FlushInt(BOOLEAN bToVal)
{
	if ( bToVal  )
	{
		mEditBuf[mCursorPos+1] = '\0';
		mData.iVal = atoi( mEditBuf );
		mData.iVal = Str2Int( mEditBuf );
		DDVInt();
		Int2Str( mData.iVal, mBuf );
		strcpy( mEditBuf, mBuf );
	}
	else
	{
		memset( mEditBuf, 0, sizeof(mEditBuf));
		Int2Str( mData.iVal, mBuf );
		//strcpy( mEditBuf, mBuf );
	}
}

void	MigEditBox::FlushFP(BOOLEAN bToVal)
{
	if ( bToVal )
	{
		mEditBuf[mCursorPos+1] = '\0';
		//mData.fVal = atof( mEditBuf );
		mData.fVal = Str2FP( mEditBuf );
		
		DDVFP();
		FP2Str( mData.fVal, mBuf );
		//strcpy( mEditBuf, mBuf );
		memset( mEditBuf, 0, sizeof(mEditBuf));
	}
	else
	{
		memset( mEditBuf, 0, sizeof(mEditBuf) );

		FP2Str( mData.fVal, mBuf );
//		strcpy( mEditBuf, mBuf );
	}
}

void	MigEditBox::SetTitle( MCHAR* pszTitle, BOOLEAN bErase )
{
	mTitleWidth = strlen(pszTitle);
	MigObj::SetTitle( pszTitle, bErase );
}

void	MigEditBox::SetStr( MCHAR *pStr )
{
	INT16U aLen = 0;
	if ( pStr )
	{
		aLen = min( ENM_TITLE_MAX-1, strlen( pStr ) );
		strncpy( mBuf, pStr, aLen);
		mBuf[aLen] = '\0';

		// limit check
		DDVCh();

		if (mStyle & ENM_CTRL_FOCUSED )
			Flush( FALSE );
	}

}

MCHAR*	MigEditBox::GetStr(void)
{
	return mBuf;
}

/*
INT16U	MigEditBox::CalCharRangeInt(void)
{
	INT16U aMax = 0;
	INT32U aVal = max( abs(mData.iMaxVal), abs(mData.iMinVal) );
	while ( aVal % 10 )
	{
		aMax++;
		aVal /= 10;
	}

	if ( mData.iMaxVal < 0 || mData.iMinVal < 0 )
		aMax++;
	
	return aMax;
}

INT16U	MigEditBox::CalCharRangeFP(void)
{
	INT16U aMax = mPrecision + 1;
	INT32U aVal = (INT32U)max( FABS(mData.fMaxVal), FABS(mData.fMinVal) );
	while ( aVal / 10 )
	{
		aMax++;
		aVal /= 10;
	}

	if ( mData.fMaxVal < 0 || mData.fMinVal < 0 )
		aMax++;

	return aMax;
}
*/

void	MigEditBox::Int2Str( INT32S val, MCHAR* pDst )
{
	MCHAR aFormat[32] = { '%', '\0' };
	sprintf( &aFormat[1], "%d", mMaxChar );
	aFormat[strlen(aFormat)] = 'd';
	aFormat[strlen(aFormat)] = '\0';
	sprintf( pDst, aFormat, val );
}

void	MigEditBox::FP2Str( FP64 val, MCHAR* pDst )
{
#if 1

	INT8U	i, j;
	INT32S	a;
	FP64	b;
	
	INT32S aPrex = 0;	
	MCHAR aBuf[16] = { '\0' };
	MCHAR aDec[16] = { 0 };
	
	if ( val < 0 )
	{
		aPrex = 1;
		val = - val;
	}

	a = (INT32S)val;
	b = val - a;
				
	/// format integer part
	i = 0;
	do
	{
		aBuf[i++] = '0' + (a%10);
		a = a / 10;
	}while ( a );

	/// Reverse
	for ( j = 0; j < i; j++ )
	{
		pDst[j+aPrex] = aBuf[i -j-1] ;
	}
	
	if ( aPrex )
	{
		pDst[0] = '-';
		i++;
	}
	pDst[i] = '\0';

	MCHAR aFormat[32] = { '%', '\0' };
	//sprintf( &aFormat[1], "0.%-df", mPrecision > 0 ? mPrecision : 1 );
	//sprintf( aDec, aFormat, b );
	sprintf( aDec, "%.12f", b );

	aDec[1] = '.';
	while ( aDec[strlen(aDec) - 1] == '0' && aDec[strlen(aDec) - 2] != '.' )
	{
		aDec[strlen(aDec) - 1] = '\0';
	}

	strcat( pDst, &aDec[1] );	
#endif
}


INT32S MigEditBox::Str2Int( MCHAR *pStr )
{
	INT32S aVal = 0;
	INT8U  aSigned = 0;
	MCHAR *apCur = &pStr[0];

	if ( *pStr == '-' )
	{
		aSigned = 1;
		apCur = &pStr[1];
	}
	
	while ( *apCur )
	{
		aVal = aVal * 10 + (*apCur - '0');
		apCur++;
	}

	if ( aSigned)
		aVal = -aVal;

	return aVal;
}

FP64 MigEditBox::Str2FP( MCHAR *pStr )
{
	FP64   aVal = 0, aRatio = 10.0;
	INT8U  aFirst, aSigned = 0;
	MCHAR  aInt[16];
	MCHAR  aCh;
	MCHAR  aFraction[32] = { '0','\0' };

	MCHAR *apCur = pStr;
	if ( *pStr == '-' )
	{
		aSigned = 1;
		apCur = &pStr[1];
	}
	
	aFirst = 1;
	while ( (aCh = *apCur) != 0 )
	{
		if ( aCh == '.' )
		{
			if ( aFirst )	///< invalid string
				break;
			else if ( aRatio < 10.0 )	///< second '.' occurent
				break;
			else
			{
				aRatio = 1.0;
				apCur++;
				aFirst = 0;
				continue;
			}
		}

		if (  '0' <= aCh && aCh <= '9' )
		{
			if ( aRatio > 1.0 )
				aVal = aVal * aRatio + (FP64)(aCh - '0');
			else
			{
				aRatio *= 0.1;
				aVal = aVal + aRatio * (FP64)(aCh - '0');
			}
		}
		else
		{
			break;
		}

		apCur++;
		aFirst = 0;
	}

	/*
	MCHAR *apInt = strstr( apCur, "." );

	if ( apInt)
	{
		apInt[0] = '\0';
		strcpy( aFraction, apInt+1);
	}
	strcpy( aInt, apCur );
	
	aVal = Str2Int( aFraction ) ;
	while ( aVal > 1.0 )
	{
		aVal /= 10.0;
	}

	aVal += Str2Int( apCur );
	*/
	if ( aSigned)
		aVal = -aVal;

	return aVal;
}


//////////////////////////  Editbox Group /////////////////////////

MigEditBoxGrp::MigEditBoxGrp( MigObj *pParent )
	: MigControl()
{
	Init();
}

MigEditBoxGrp::MigEditBoxGrp( const tagCS& cs )
	: MigControl( cs )
{
	Init();
}

void	MigEditBoxGrp::Init( void )
{
	mType  = ENM_CTRL_EDITBOX;

	mItemCnt = 0;
	mCurItem = 0;	///< current active editbox
	
}

void	MigEditBoxGrp::SetBoxGrpParam( tagEditBoxCS* eCs, INT8U cnt )
{
	mItemCnt = min ( EB_MAX, cnt );
	tagCS cs;

	cs.x = mWidth + GetFontW() / 2;
	cs.y = mY;
	cs.w = 0;
	cs.h = mHeight;
	cs.hParent = this->mpParent;

	for ( char i = 0; i < mItemCnt; i++ )
	{
		eCs[i].titleWidth = CalRange( eCs[i] )*8;

		sprintf( cs.pszTitle, eCs[i].pszTitle );
		cs.x += GetItemW( i-1 );
		cs.w = strlen(cs.pszTitle) * GetFontW();
		
		SetItemParam( i, cs, (tagEditBoxCS)eCs[i] );
	}
}

void	MigEditBoxGrp::SetItemParam( INT8U index, const tagCS& cs, const tagEditBoxCS& ecs )
{
	if ( index >= EB_MAX )
		return;

	MigControl* aTitle = &mItemSet[index].title;
	MigEditBox* aBox   = &mItemSet[index].box;

	/// set editbox control
	aBox->mX	  = cs.x;  
	aBox->mY	  = cs.y;
	aBox->mWidth  = ecs.titleWidth;  
	aBox->mHeight = cs.h;	

	tagEditBoxCS aECS = ecs;
	aECS.titleWidth = 0;
	
	aBox->Init( aECS );
	aBox->SetParent( this->Parent() );
	aBox->SetTitle( "" );
	aBox->SetStyle( mStyle );	
	aBox->SetInt( aECS.iVal );

	/// Set title control
	aTitle->mX	    = cs.x + aBox->mWidth;  
	aTitle->mY	    = cs.y;
	aTitle->mWidth  = cs.w;  
	aTitle->mHeight = cs.h;	

	aTitle->SetParent( this->Parent() );
	aTitle->SetTitle( (MCHAR*)cs.pszTitle );
	aTitle->SetStyle( mStyle );		
}

INT16U	MigEditBoxGrp::CalRange( tagEditBoxCS& ecs )
{
	INT16U  aSize;
	switch ( ecs.type )
	{
	case ENM_EB_TYPE_CHAR:
		aSize = ecs.maxChar;
		break;
	case ENM_EB_TYPE_INT:
		aSize = CalRangeInt( ecs.maxVal, ecs.minVal );
		break;
	case ENM_EB_TYPE_FLOAT:
		aSize = CalRangeFP( ecs.fmaxVal, ecs.fminVal );
		break;
	default:
		aSize = 0;
	}
	
	return aSize;
}

BOOLEAN	 MigEditBoxGrp::IsInEdit(void)
{ 
	return mItemSet[mCurItem].box.IsInEdit();
	//return (mStyle & ENM_CTRL_IN_EDIT) ? TRUE : FALSE; 
}

INT32U	MigEditBoxGrp::GetItemW( INT8U index )
{
	INT32U aWidth;
	if ( index < EB_MAX )
	{
		aWidth = mItemSet[index].title.mWidth + mItemSet[index].box.mWidth;
	}
	else
	{
		aWidth = 0;
	}

	return aWidth;
}

void MigEditBoxGrp::Draw(void)
{
	MigControl::Draw();
	
	for ( INT8U i = 0; i < mItemCnt; i++ )
	{
		mItemSet[i].box.Update();
		mItemSet[i].title.Update();
	}
}

void	MigEditBoxGrp::DrawTitle(void)
{
	if ( mTitleID == MIG_STR_ID_INVALID )
		PutStr( mpParent->mX + mX+8, mpParent->mY + mY+2, mszTitle, 0 );
	else
		PutStr( mpParent->mX + mX+8, mpParent->mY + mY+2, MLoadStr(mTitleID), 0 );
}

/// message process
void  MigEditBoxGrp::OnKey( INT16U key )
{
	INT8S aItem;
	INT16U  aKey = GetKeyValue( key );
	BOOLEAN aPass = TRUE;
	// special procss
	if ( aKey == KEY_Left )
	{
		//if ( mItemSet[mCurItem].box.mCursorPos == 0 )
		if ( !mItemSet[mCurItem].box.IsInEdit() )
		{
			if ( mCurItem > 0 )
			{
				aItem = mCurItem-1;
				while ( !mItemSet[aItem].box.IsActive() )
				{
					aItem--;
					if ( aItem < 0 )
						return;
				}
				if ( mItemSet[aItem].box.IsActive() )
				{
					mCurItem = aItem;
					mItemSet[mCurItem+1].box.FocusOff();
					mItemSet[mCurItem].box.FocusOn();
				}
	
	//			mItemSet[mCurItem].box.FocusOff();
	//			mItemSet[--mCurItem].box.FocusOn();
				aPass = FALSE;
			}
		}
	}
	else if ( aKey == KEY_Right  )
	{
		if ( !mItemSet[mCurItem].box.IsInEdit() )
		{
			if ( mCurItem < mItemCnt - 1 )
			{
				mItemSet[mCurItem].box.FocusOff();
				mItemSet[++mCurItem].box.FocusOn();

				aPass = FALSE;
			}
		}
	}
	
	if ( aPass )
	{
	//	if ( &mItemSet[mCurItem].box != this )
			mItemSet[mCurItem].box.OnKey( key );
	}	
}

void	MigEditBoxGrp::FocusOn(void)
{
	mCurItem = 0;
	while ( !mItemSet[mCurItem].box.IsActive() )
	{
		mCurItem++;
		if ( mCurItem >= mItemCnt )
			return;
	}

	mItemSet[mCurItem].box.FocusOn();		
	MigControl::FocusOn();
}

void	MigEditBoxGrp::FocusOff(void)
{
	mItemSet[mCurItem].box.FocusOff();
	mCurItem = 0;

	MigControl::FocusOff();
}

void	MigEditBoxGrp::SetTitle( MCHAR* pszTitle )
{

}

void	MigEditBoxGrp::SetX( INT32S x )
{

}

void	MigEditBoxGrp::SetY( INT32S y )
{

}


void	MigEditBoxGrp::SetBoxStr( MCHAR* pStr, INT8U index )
{
	mItemSet[index].box.SetStr( pStr );
}

void	MigEditBoxGrp::SetBoxInt( INT32S iVal, INT8U index )
{
	mItemSet[index].box.SetInt( iVal );
}

void	MigEditBoxGrp::SetBoxFP ( FP64 fVal,   INT8U index )
{
	mItemSet[index].box.SetFP( fVal );
}

MCHAR*	MigEditBoxGrp::GetBoxStr( INT8U index )
{
	return (index < EB_MAX) ? mItemSet[index].box.GetStr() : NULL;
}

INT32S	MigEditBoxGrp::GetBoxInt( INT8U index )
{
	return (index < EB_MAX) ? mItemSet[index].box.GetInt() : 0;
}

FP64	MigEditBoxGrp::GetBoxFP ( INT8U index )
{
	return (index < EB_MAX) ? mItemSet[index].box.GetFP() : 0.0;

}


///////////////////////////////////////////
////    Grid box
///////////////////////////////////////////

MigGrid::MigGrid( MigObj *pParent )
	:MigControl( pParent )
{
	MCHAR  *aszTitle[] = { " ", " ", " " };
	INT16U aWidth[] = { 10, 10, 10 };
	INT16U aType[]  = { ENM_EB_TYPE_CHAR, ENM_EB_TYPE_CHAR, ENM_EB_TYPE_CHAR };

	tagGridBoxCS bcs;

	bcs.lineDisp		= 2;
	bcs.lineDisp	= 2;
	bcs.columnDisp	= 3;
	bcs.columnDisp	= 3;
	bcs.titleHeight = 20;
	bcs.itemHeight	= 20;

	bcs.pColumnWidth = aWidth;
	bcs.pItemType	 = aType;
	bcs.pszItemTitle = aszTitle;

	Init( bcs );
}

MigGrid::MigGrid( const tagCS& cs,  const tagGridBoxCS& bcs )
	:MigControl( cs )
{
	Init( bcs );
}

MigGrid::~MigGrid( )
{ 
	if ( mpItemBox )
		delete []mpItemBox;
}

void	MigGrid::Init( const tagGridBoxCS& bcs )
{	
	mGCS = bcs;
	mGCS.pColumnWidth = mColumnWidth;
	mGCS.pItemType	  = mBoxType;
	mGCS.pszItemTitle = (MCHAR**)mpszItemTitle;

	if ( !bcs.pszItemTitle )
	{
		mGCS.titleHeight = 1;
		mbHaveTitle = FALSE;
	}
	else
	{
		mbHaveTitle = TRUE;
	}

	U16 i;
	for ( i = 0; i < bcs.columnDisp; i++ )
	{
		if ( bcs.pszItemTitle && bcs.pszItemTitle[i] )
			strcpy( mpszItemTitle[i], bcs.pszItemTitle[i] );
		else
			mpszItemTitle[i][0] = '\0';
		
		mGCS.pColumnWidth[i] = bcs.pColumnWidth[i];
		mGCS.pItemType[i]	 = bcs.pItemType[i];
	}

	for ( i = 0; i < MIG_GRIDBOX_COLUMN_MAX; i++ )
		mItemTitleID[i] = MIG_STR_ID_INVALID;
	
	ResetFocusItem();

	mpItemBox 	=  new MigEditBox[mGCS.lineDisp * mGCS.columnDisp];	
	InitItem();

	mTopPos = mBottomPos = 0;
	mLeftPos = mRightPos = 0;

	Layout();
}

void	MigGrid::ResetFocusItem(void)
{
	mCurColumn	= 0;
	mCurLine	= 0;
	mUsedLine	= 0;
	mUpperLine	= 0;	///< current upper line index
	mLeftColumn	= 0;	///< current left column index
}

void	MigGrid::InitItem(void)
{
	if ( NULL == mpItemBox )
		return;

	tagCS cs;
	cs.x = mX + GetFontW() /2;
	cs.y = mY + mGCS.titleHeight;
	cs.w = 0;
	cs.h = mGCS.itemHeight;
	cs.dwStyle = ENM_OBJ_TITLE;
	cs.hParent = this->mpParent;

	tagEditBoxCS ecs;
	memset( &ecs, 0, sizeof(ecs) );
	ecs.pszTitle[0] = '\0';
	
	for ( INT16U i = 0; i < mGCS.columnDisp; i++ )
	{
		//cs.w = mGCS.pColumnWidth[i];
		ecs.titleWidth = mGCS.pColumnWidth[i];
		ecs.type = mGCS.pItemType[i];

		for( INT16U	j = 0; j < mGCS.lineDisp; j++ )
		{
			SetItemParam( j, i, cs, ecs);

			cs.y += mGCS.itemHeight;
		}

		cs.x += mGCS.pColumnWidth[i];
		cs.y = mY + mGCS.titleHeight;
	}

}

void	MigGrid::SetItemParam( INT16U ln, INT16U column, const tagCS& cs, const tagEditBoxCS& ecs )
{
	MigEditBox *aBox   = &mpItemBox[ln*mGCS.columnDisp+column];

	/// set editbox control
	aBox->mX	  = cs.x;  
	aBox->mY	  = cs.y;
	aBox->mWidth  = ecs.titleWidth;  
	aBox->mHeight = cs.h;	
	aBox->SetStyle( cs.dwStyle );	

	tagEditBoxCS aEcs = ecs;
	aEcs.titleWidth = 0;
	aBox->Init( aEcs );
	aBox->SetParent( (MigObj*)cs.hParent );//this->Parent() );
	aBox->SetTitle( "", FALSE );
	aBox->SetStyle( ENM_CTRL_NORMAL );	//mStyle );	
	//aBox->SetInt( ecs.iVal );
}


void	MigGrid::SetItemTitle( MCHAR* pszTitle, INT16U count )
{
	
}
void	MigGrid::SetItemTitle( INT16U idx, MCHAR* pszTitle )
{
	if ( idx < mGCS.columnDisp )
	{
		strcpy( mpszItemTitle[idx], pszTitle );
	}
}

void	MigGrid::SetItemTitle( INT16U idx, INT16U titleID )
{
	if ( idx < mGCS.columnDisp )
	{
		mItemTitleID[idx] = titleID;
	}
}

void	MigGrid::SetItemRangeCHAR( INT16U	ln, INT16U	column, INT16U maxChar )
{
	MigEditBox *apItem = &mpItemBox[ln*mGCS.columnDisp+column];

	if ( apItem->GetType() == ENM_EB_TYPE_CHAR )
	{
		apItem->SetRangeCHAR( maxChar );
	}
}

void	MigGrid::SetItemRangeFP( INT16U	ln, INT16U	column, FP64 fmin, FP64 fmax, INT8U precision )
{
	MigEditBox *apItem = &mpItemBox[ln*mGCS.columnDisp+column];

	if ( apItem->GetType() == ENM_EB_TYPE_FLOAT )
	{
		apItem->SetRangeFP( fmin, fmax, precision );
	}
}

void	MigGrid::SetItemRangeInt( INT16U	ln, INT16U	column, INT32S min, INT32S max )
{
	MigEditBox *apItem = &mpItemBox[ln*mGCS.columnDisp+column];

	if ( apItem->GetType() == ENM_EB_TYPE_INT )
	{
		apItem->SetRangeInt( min, max );
	}
}

void	MigGrid::SetItemVal( INT16U	ln, INT16U	column, void *pVal )
{
	if ( NULL == pVal )
		return;

	INT8U aSet = 0;
	if ( mpItemBox[ln*mGCS.columnDisp+column].GetStyle() & ENM_CTRL_FOCUSED )
	{
		mpItemBox[ln*mGCS.columnDisp+column].FocusOff();
		aSet = 1;
	}

	switch ( mGCS.pItemType[column] )
	{
	case ENM_EB_TYPE_CHAR:
		mpItemBox[ln*mGCS.columnDisp+column].SetStr( (MCHAR*)pVal );
		break;

	case ENM_EB_TYPE_INT:
		mpItemBox[ln*mGCS.columnDisp+column].SetInt( *(INT32S*)pVal );
		break;
	case ENM_EB_TYPE_FLOAT:
		mpItemBox[ln*mGCS.columnDisp+column].SetFP( *(FP64*)pVal );
		break;
	}

	if ( aSet )
		mpItemBox[ln*mGCS.columnDisp+column].FocusOn();

}

BOOLEAN	MigGrid::GetItemVal( INT16U	ln, INT16U	column, void *pVal )
{
	if ( NULL == pVal )
		return FALSE;

//	if ( !mpItemBox[ln*mGCS.columnDisp+column].IsActive())
//		return FALSE;
	
	switch ( mGCS.pItemType[column] )
	{
	case ENM_EB_TYPE_CHAR:
		strcpy( (MCHAR*)pVal, mpItemBox[ln*mGCS.columnDisp+column].GetStr() );
		break;

	case ENM_EB_TYPE_INT:
		*(INT32S*)pVal = mpItemBox[ln*mGCS.columnDisp+column].GetInt();
		break;
	case ENM_EB_TYPE_FLOAT:
		*(FP64*)pVal = mpItemBox[ln*mGCS.columnDisp+column].GetFP();
		break;

	}

	return TRUE;
}

void	MigGrid::Draw()
{
	if ( NULL == mpParent )
		return;

	if ( mStyle & ENM_OBJ_HIDEN )
		return;

	if ( mStyle & ENM_CTRL_DISABLE )
		return;

	DrawFrame();
	DrawTitle();
	DrawItem();
}

void	MigGrid::DrawFrame()
{
	/// Refresh client
	if ( mbErase )
	{
		/// Clear Client Rect
		INT32U aPosX = mpParent->mX + mX+4;
		INT32U aPosY = mpParent->mY + mY+2+mGCS.titleHeight;
		INT32U aWidth = 0;
		INT32U aHeight = mGCS.itemHeight * mGCS.lineDisp;

		for ( INT16U i = 0; i < mGCS.columnDisp; i++ )
		{
			aWidth += mGCS.pColumnWidth[i];	
		}
		
		PutBox( aPosX, aPosY, aWidth, aHeight );

		mbErase = FALSE;
	}

	if ( mStyle & ENM_OBJ_FRAME )
	{
		PutRect( mpParent->mX + mX, mpParent->mY + mY, mWidth, mHeight );
	}
}

void	MigGrid::DrawItem()
{
	if ( NULL == mpItemBox )
		return;

	for ( INT16U i = 0; i < mGCS.columnDisp; i++ )
	{
		for( INT16U	j = 0; j < mGCS.lineDisp; j++ )
		{
			mpItemBox[j*mGCS.columnDisp+i].Update();
		}
	}
}

void	MigGrid::DrawTitle()
{
	if ( !mbHaveTitle )
		return;

	INT32U aPosX = mpParent->mX + mX+4;

	for ( INT16U i = 0; i < mGCS.columnDisp; i++ )
	{
		if ( mItemTitleID[i] == MIG_STR_ID_INVALID )
			PutStr( aPosX, mpParent->mY + mY+2, mpszItemTitle[i], 0 );
		else
			PutStr( aPosX, mpParent->mY + mY+2, MLoadStr(mItemTitleID[i]), 0 );
	
		aPosX += mGCS.pColumnWidth[i];
	}	
}	

void	MigGrid::OnKeyLeft()
{
	/// do nothing
}

void	MigGrid::OnKeyRight()
{
	/// do nothing
}

void	MigGrid::OnKey( INT16U key )
{
	BOOLEAN aPass = TRUE;
	BOOLEAN aReLayout = FALSE;
	
	INT8U	aCtlrSW = 0;

	INT16U	aCurLn	= mCurLine - mUpperLine;
	INT16U	aCurCol	= mCurColumn - mLeftColumn;

	INT16U	aNewLn	 = mCurLine;
	INT16U	aNewCol	 = mCurColumn;
	INT16U	aUpperLn = mUpperLine;
	INT16U	aLeftCol = mLeftColumn;

	INT8U   aKey = GetKeyValue( key );

	MigEditBox *apItem = &mpItemBox[aCurLn * mGCS.columnDisp + aCurCol];

	/// in edit mode , only send key event to it
	if ( apItem->IsInEdit() )
	{
		apItem->OnKey( key );
		if ( aKey == KEY_OK )
		{
			mpItemBox[aCurLn * mGCS.columnDisp + aCurCol].Refresh();
			OnKeyOK();
		}
	
		return;
	}

	if ( aKey == KEY_Up )
	{
		if ( mCurLine > 0 )
		{
			aNewLn--;

			if ( aCurLn <= 0 )
			{
				aUpperLn--;
				aReLayout = TRUE;
			}

			aPass = FALSE;

			mTopPos = 0;
		}
		/// when up to upper, switch to other control 
		else
		{
			aCtlrSW = 1; 
		}
	}
	
	if ( aKey == KEY_Down  )
	{
		if ( mCurLine < mUsedLine - 1 )
		{
			aNewLn++;

			if ( aCurLn >= mGCS.lineDisp )
			{
				aUpperLn++;
				aReLayout = TRUE;
			}

			aPass = FALSE;

			mBottomPos = 0;
		}
		/// when down to bottom, switch to other control 
		else
		{
			aCtlrSW = 2;
		}
	}

	if ( aKey == KEY_Left )
	{
		if ( !apItem->IsInEdit() )
		{
			if ( mCurColumn > 0 )
			{
				aNewCol--;
				aPass = FALSE;
				//mLeftPos = 0;
			}
			else
			{
				//mLeftPos++;
			}
		}

		OnKeyLeft();
	}

	if ( aKey == KEY_Right  )
	{
		if ( !apItem->IsInEdit() )
		{
			if ( mCurColumn < mGCS.columnDisp - 1 )
			{
				aNewCol++;
				aPass = FALSE;
			}

			//mLeftPos = 0;
		}

		OnKeyRight();
	}

/*
	if ( aCtlrSW )
	{
		OutEditMode();
		(aCtlrSW == 1 ) ? ((MigPage*)Parent())->ControlFocusUp() : ((MigPage*)Parent())->ControlFocusDown() ;
	}
*/

	if ( aReLayout == TRUE )
	{
	//	Layout();
	}

	if ( aPass == FALSE )
	{
		//apItem->FocusOff();
		ItemFocusOn( aNewLn, aNewCol );
	}
	else
	{
		apItem->OnKey( key );
	}

	if ( mCurLine == mUsedLine - 1 )
	{
		mBottomPos++;
	}

	if ( mCurLine == 0 )
	{
		mTopPos++; 
	}

	/// temp 
	if ( aKey == KEY_OK )
	{
		OnKeyOK();
		mpItemBox[aCurLn * mGCS.columnDisp + aCurCol].Refresh();
	}
}

void	MigGrid::FocusOn(void)
{
	InEditMode();
	ItemFocusOn( mCurLine, mCurColumn );
	
	MigControl::FocusOn();
}

void	MigGrid::FocusOff(void)
{
	OutEditMode();
	ItemFocusOff( mCurLine, mCurColumn );
	ItemFocusOffAll();

	MigControl::FocusOff();
}

BOOLEAN	 MigGrid::IsActive(void) 
{ 
	BOOLEAN	aIsActive;
	if ( mUsedLine <= 0 )
		aIsActive = FALSE;
	else
		aIsActive = MigControl::IsActive();

	return aIsActive;
}

void	MigGrid::ItemFocusOn(INT16U ln, INT16U col )
{
	if ( mUsedLine <= 0 )
		return;

	if ( ln < mUpperLine )
	{
		mUpperLine = ln;
		Layout();
	}
	else if ( (ln - mUpperLine) >= mGCS.lineDisp )
	{
		mUpperLine++;
		Layout();
	}

	if ( mpItemBox[(ln - mUpperLine) * mGCS.columnDisp + (col - mLeftColumn)].IsActive() )
	{
		mpItemBox[(mCurLine - mUpperLine) * mGCS.columnDisp + (mCurColumn - mLeftColumn)].FocusOff();
		mpItemBox[(ln - mUpperLine) * mGCS.columnDisp + (col - mLeftColumn)].FocusOn();			
		mCurLine	= ln;
		mCurColumn	= col;
	}
}

void	MigGrid::ItemFocusOff(INT16U ln, INT16U col )
{
	mpItemBox[(ln - mUpperLine) * mGCS.columnDisp + (col - mLeftColumn)].FocusOff();
}

void	MigGrid::ItemFocusOffAll( )
{
	if ( NULL == mpItemBox )
		return;

	for ( INT16U i = 0; i < mGCS.columnDisp; i++ )
	{
		for( INT16U	j = 0; j < mGCS.lineDisp; j++ )
		{
			mpItemBox[j*mGCS.columnDisp+i].FocusOff();
		}
	}
}

INT16U	MigGrid::AddLine( void )
{
	INT16U	aRet = (INT16U)-1;

	if ( mUsedLine < mGCS.lineMax )
	{
		ItemFocusOff( mCurLine, mCurColumn );

		aRet = mUsedLine++;		
		if ( mUsedLine <= mGCS.lineDisp )
		{
			VisibleLine( mUsedLine-1 );		
		}

		mbErase = TRUE;
	}

	return aRet;
}

void	MigGrid::DeleteAllLine( void )
{
	ResetFocusItem();
	VisibleAll( FALSE );
}

INT16U	MigGrid::DeleteLine( INT16U ln )
{
	INT16U	aRet = (INT16U)-1;
	if ( ln < mUsedLine )
	{
		if ( mUsedLine <= mGCS.lineDisp )
			VisibleLine( mUsedLine-1, FALSE );	
		
		aRet = --mUsedLine;
		if ( mCurLine >= mUsedLine )
		{
			if ( mCurLine > 0 )
				mCurLine--;


			if ( mCurLine < mUpperLine )
				mUpperLine--;
	
			ItemFocusOn( mCurLine, mCurColumn );

		//	mpItemBox[(mCurLine - mUpperLine) * mGCS.columnDisp + (mCurColumn - mLeftColumn)].FocusOn();
		}

		mbErase = TRUE;
		Layout();
	}

	return aRet;
}

INT16U	MigGrid::TopLine( void )		///< move display window to top; return: current line's index
{
	mUpperLine  = 0;
	mCurLine	= 0;
	
	/// relayout 
	Layout();

	return mCurLine - mUpperLine;;
}

INT16U	MigGrid::BottomLine( void )	///< move display window to bottom line; return: current line's index
{
	ItemFocusOff( mCurLine, mCurColumn );
	mCurLine	= mUsedLine - 1;

	mUpperLine  = (mUsedLine >= mGCS.lineDisp) ? (mCurLine - mGCS.lineDisp + 1) : 0; //(mUsedLine - 1) / mGCS.lineDisp * mGCS.lineDisp;
	
	/// relayout 
	Layout();

	ItemFocusOn( mCurLine, mCurColumn );

	return mCurLine - mUpperLine;
}

void	MigGrid::EnableLine( INT16U	ln, BOOLEAN bEnable )
{
	for ( INT16U i = 0; i < mGCS.columnDisp; i++ )
	{
		mpItemBox[ln*mGCS.columnDisp+i].Enable( bEnable );
	}
}

void	MigGrid::EraseLine( INT16U	ln )
{
	for ( INT16U i = 0; i < mGCS.columnDisp; i++ )
	{
		mpItemBox[ln*mGCS.columnDisp+i].Refresh( );
	}
}

void	MigGrid::VisibleLine( INT16U	ln, BOOLEAN bVisible )
{
	for ( INT16U i = 0; i < mGCS.columnDisp; i++ )
	{
		mpItemBox[ln*mGCS.columnDisp+i].Visible( bVisible );
	}

	if ( bVisible == TRUE && mCurLine == ln )	/// focus is on the column
	{
		if ( ln < mGCS.lineDisp - 1 )
		{
			ItemFocusOn( ln, mCurColumn );
		}
		else if ( ln >= 0 )
		{
			ItemFocusOn( ln, mCurColumn );
		}

		/// 
		if ( !mpItemBox[(ln - mUpperLine) * mGCS.columnDisp + (mCurColumn - mLeftColumn)].IsFocusOn() )
		{
			for ( INT16U i = 0; i < mGCS.columnMax; i++ )
			{
				ItemFocusOn( ln, i );
				if ( mpItemBox[(ln - mUpperLine) * mGCS.columnDisp + (i - mLeftColumn)].IsFocusOn() )
					break;
			}
		}
	}
}

void	MigGrid::VisibleItem( INT16U ln, INT16U col, BOOLEAN bVisible )
{
	if ( ln >= mGCS.lineDisp || col >= mGCS.columnDisp )
		return;

	mpItemBox[ln*mGCS.columnDisp+col].Visible( bVisible );

	if ( bVisible == TRUE && mCurLine == ln && mCurColumn == col )	/// focus is on the Item
	{
//		if ( ln < mGCS.lineDisp - 1 )
//		{
//			ItemFocusOn( ln, mCurColumn );
//		}
//		else if ( ln >= 0 )
//		{
//			ItemFocusOn( ln, mCurColumn );
//		}

		/// 
//		if ( !mpItemBox[(ln - mUpperLine) * mGCS.columnDisp + (mCurColumn - mLeftColumn)].IsFocusOn() )
//		{
//			for ( INT16U i = 0; i < mGCS.columnMax; i++ )
//			{
//				ItemFocusOn( ln, i );
//				if ( mpItemBox[(ln - mUpperLine) * mGCS.columnDisp + (i - mLeftColumn)].IsFocusOn() )
//					break;
//			}
//		}
	}
}

void	MigGrid::EnableAll( BOOLEAN bEnable )
{
	for ( INT16U i = 0; i < mGCS.columnDisp * mGCS.lineDisp; i++ )
	{
		mpItemBox[i].Enable( bEnable );
	}

	ResetFocusItem();
}

void	MigGrid::VisibleAll( BOOLEAN bVisible )
{
	for ( INT16U i = 0; i < mGCS.columnDisp * mGCS.lineDisp; i++ )
	{
		mpItemBox[i].Visible( bVisible );
	}

	ResetFocusItem();
}

void	MigGrid::EnableColumn( INT16U	column, BOOLEAN bEnable )
{
	for ( INT16U i = 0; i < mGCS.lineDisp; i++ )
	{
		mpItemBox[i*mGCS.columnDisp+column].Enable( bEnable );
	}
}


void	MigGrid::EraseColumn( INT16U	column )
{
	for ( INT16U i = 0; i < mGCS.lineDisp; i++ )
	{
		mpItemBox[i*mGCS.columnDisp+column].Refresh( );
	}
}

void	MigGrid::EnableMultiKeyPress( INT16U	column, BOOLEAN bEnable )
{
	for ( INT16U i = 0; i < mGCS.lineDisp; i++ )
	{
		mpItemBox[i*mGCS.columnDisp+column].EnableMultiKeyPress( bEnable );
	}
}

void	MigGrid::VisibleColumn( INT16U	column, BOOLEAN bVisible )
{
	for ( INT16U i = 0; i < mGCS.lineDisp; i++ )
	{
		mpItemBox[i*mGCS.columnDisp+column].Visible( bVisible );
	}

	if ( bVisible == FALSE && mCurColumn == column )	/// focus is on the column
	{
		if ( column < mGCS.columnDisp - 1 )
		{
			ItemFocusOn( mCurLine, column + 1 );
		}
		else if ( column > 0 )
		{
			ItemFocusOn( mCurLine, column - 1 );
		}
	}
}

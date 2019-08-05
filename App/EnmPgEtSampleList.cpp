#include "EnmPgEtSampleList.h"
#include "EnmData.h"

CEnmSampleList::CEnmSampleList( MigObj *pParent )
 : MigGrid( pParent )
 , mpRefProtocol( NULL )
{ 
	SetList(); 
}

CEnmSampleList::CEnmSampleList( const tagCS& cs,  const tagGridBoxCS& bcs ) 
 : MigGrid( cs, bcs )
 , mpRefProtocol( NULL )
{ 
	SetList(); 
}

void	CEnmSampleList::SetList( void )
{
	mCurColumn = 1;
	EnableColumn( 0, FALSE );
	EnableMultiKeyPress( 1, FALSE );
	Layout();
}

void	CEnmSampleList::SetOneSample( INT16U ln, tagSampleData& data )
{
	MCHAR aTitle[32];

	/// set index
	INT32S aIdx = ln + 1;
	SetItemVal( ln, 0, &aIdx );

	/// set pos string
	{
#if STD_SHOW_POS
		sprintf( aTitle, "%c%02d", 'A' + data.yPos, data.xPos + 1 );
		SetItemVal( ln, 1, aTitle );

		SetItemVal( ln, 2, &(data.density) );
#else
		SetItemVal( ln, 1, &(data.density) );
#endif
	}
}

void	CEnmSampleList::SetOneSample( INT16U ln, INT16U sample )
{
	MCHAR aTitle[32];
	INT32S aIdx = sample + 1;

	/// set index
	SetItemVal( ln, 0, &aIdx );

	if ( !mpRefProtocol || sample >= mpRefProtocol->stQuality.uStd )
		return;
	
	/// set pos string
	U16 aStdRepeat = mpRefProtocol->stQuality.uStdRepeats;
	U16 aBlankCnt = mpRefProtocol->stQuality.uBlank;
	U16 aCol = aBlankCnt / ENM_DATA_COUNT;
	U16 aRow = aBlankCnt % ENM_DATA_COUNT + sample * aStdRepeat;
	if ( aRow > ENM_DATA_COUNT - 1 )
	{
		aCol++;
		aRow = aRow - ENM_DATA_COUNT;
	}

	tagSampleData aData = mpRefProtocol->stQuality.stStdDensity[sample];
	aData.xPos = aCol;
	aData.yPos = aRow;
	
	//SetOneSample( ln, aData );

	/// set pos string
	{
#if STD_SHOW_POS
		sprintf( aTitle, "%c%02d", 'A' + aData.yPos, aData.xPos + 1 );
		SetItemVal( ln, 1, aTitle );

		SetItemVal( ln, 2, &(aData.density) );
#else
		SetItemVal( ln, 1, &(aData.density) );
#endif
	}

}

void	CEnmSampleList::OnKey( INT16U key )
{
	BOOLEAN  aTrans = TRUE;
	INT16U    aKey = GetKeyValue( key );

#if STD_SHOW_POS
	if ( 1 == mCurColumn ) ///< position editbox
	{
		BOOLEAN aCheck = FALSE;
		INT16U	aCurLn	= mCurLine - mUpperLine;
		INT16U	aCurCol	= mCurColumn - mLeftColumn;
		MigEditBox *apItem = &mpItemBox[aCurLn * mGCS.columnDisp + aCurCol];

		/// when input sample's first position, specialize the key 
		if ( apItem->IsInEdit() )
		{
			MCHAR aCh = Key2Char( aKey );
			if ( 0 == apItem->mCursorPos )
			{
				if ( '1' <= aCh && aCh <= '8' )
				{
					apItem->OnKey( key );
					MigEditBox::mEditBuf[0] = 'A' + (aCh - '1');
				}
			}
			else if ( 1 == apItem->mCursorPos )
			{
				if ( '0' <= aCh && aCh <= '1' )
				{
					apItem->OnKey( key );
				
					aCheck = TRUE;
				}
			}
			else if ( 2 == apItem->mCursorPos )
			{
				if ( '0' <= aCh && aCh <= '9' )
				{
					apItem->OnKey( key );
			
					aCheck = TRUE;
				}
			}

			/// check if in range[1,12]
			if ( aCheck )
			{
				if ( MigEditBox::mEditBuf[1] == '0' && MigEditBox::mEditBuf[2] == '0' )
				{
					MigEditBox::mEditBuf[2] = '1';
				}
			
				if ( MigEditBox::mEditBuf[1] == '1' && MigEditBox::mEditBuf[2] > '2' )
				{
					MigEditBox::mEditBuf[2] = '2';
				}
			}

			if ( '0' <= aCh && aCh <= '9' )
			{
				aTrans = FALSE;
			}

		}
	}
#endif

	if ( aTrans )
		MigGrid::OnKey( key );
}

void	CEnmSampleList::OnKeyOK(void) 
{ 
	INT16U	aCurLn	= mCurLine - mUpperLine;
	INT16U	aCurCol	= mCurColumn - mLeftColumn;
	MigEditBox *apItem = &mpItemBox[aCurLn * mGCS.columnDisp + aCurCol];
	
	tagSampleData data;
	if ( !apItem->IsInEdit() )
	{
		if ( GetOneSample(aCurLn, data ) )
		{
		//	EnmSetStdSample( mSampleType, mCurLine, data );
		}
	}
}

INT16U	CEnmSampleList::AddSample( tagSampleData& data )
{
	INT16U  aLn;
	if ( AddLine() != (INT16U)-1 )
	{
		aLn = BottomLine();
		SetOneSample( aLn, data );
	}

	Draw();

	return aLn;
}

INT16U	CEnmSampleList::AddSample( INT16U sample )
{
	INT16U  aLn;
	if ( AddLine() != (INT16U)-1 )
	{
		aLn = BottomLine();
		SetOneSample( aLn, sample );
	}

	Draw();

	return aLn;
}

void	CEnmSampleList::DeleteSample( INT16U sample )
{
	DeleteLine( sample );
	Draw();
}

void	CEnmSampleList::DeleteAllSample( )
{
	DeleteAllLine( );
	Draw();
}

void	CEnmSampleList::ClearOneSample( INT16U ln )
{
	MCHAR aTitle[2] = {'\0'};
	INT32S aIdx = 0;

	SetItemVal( ln, 0, &aIdx );
#if STD_SHOW_POS
	SetItemVal( ln, 1, aTitle );
	SetItemVal( ln, 2, aTitle );
#else
	SetItemVal( ln, 1, aTitle );
#endif
}

BOOLEAN	CEnmSampleList::GetOneSample( INT16U ln, tagSampleData& data )
{
	BOOLEAN abGot = FALSE;
	MCHAR aszPos[32];
	INT32S aIdx;

	data.density = 0;
	data.xPos = data.yPos = 0;
	
	INT16S aX, aY;
	if ( ln < mGCS.lineMax )
	{
#if STD_SHOW_POS
		GetItemVal( ln, 1, aszPos );
		GetItemVal( ln, 2, &(data.density) );

		aY = aszPos[0] - 'A';
		aX = (aszPos[1] - '0') * 10 + (aszPos[2]-'0') - 1; //atoi( &aszPos[1] ) - 1;

		if ( (0 <= aX && aX < 8) &&
			 (0 <= aY && aY < 12) )
		{
			data.xPos = aX;
			data.yPos = aY;
	
			abGot = TRUE;
		}
#else
		GetItemVal( ln, 1, &(data.density) );
		data.xPos = 0;
		data.yPos = ln;
#endif
	}

	return abGot;
}


void	CEnmSampleList::Layout( void)	///< set layout by current line and comumn
{
	INT16U	aLines = min( mGCS.lineDisp, mUsedLine - mUpperLine );

	INT16U i;	
	for ( i = 0; i < aLines; i++ )
	{
		VisibleLine( i );
		SetOneSample( i, mUpperLine + i );
	}

	for ( i = aLines; i < mGCS.lineDisp; i++ )
	{
		ClearOneSample( i );
		VisibleLine( i, 0 );
	}
}


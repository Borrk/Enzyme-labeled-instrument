#ifndef __MIG_EDITBOX_H__
#define __MIG_EDITBOX_H__

#include  "MIG_Base.hpp"
#include  "MIG_Control.hpp"

class MigGrid;
class CEnmSampleList;
class MigEditBxoGrp;

class MigEditBox : public MigControl
{
	friend class MigEditBoxGrp;
	friend class MigGrid;
	friend class CEnmSampleList;

public:
	MigEditBox( MigObj* pParent = NULL, INT8U type = ENM_EB_TYPE_CHAR );
	MigEditBox( const tagEditBoxCS& eCs, MigObj* pParent = NULL );
	MigEditBox( const tagCS& cs, const tagEditBoxCS& eCs );
	virtual ~MigEditBox();

	INT32S Str2Int( MCHAR *pStr );
	FP64 Str2FP( MCHAR *pStr );

	virtual void	OnMessage( const tagMigMSG& stMsg );
	virtual void	OnKeyOK( void );
	virtual void	FocusOn(void);
	virtual void	FocusOff(void);

	virtual void	InEditMode( void ) 
	{ 
		MigControl::InEditMode();
		mCurTablePos = 0;
		mbErase = TRUE;
	}

	virtual void	OutEditMode( void ) 
	{ 
		mCurTablePos = 0;
		MigControl::OutEditMode();
	}

	/// Char Editbox
	void	SetTitle( MCHAR* pszTitle, BOOLEAN bErase = TRUE );
	void	SetStr( MCHAR *pStr );
	MCHAR*	GetStr(void);

	/// Integer EditBox
	void	SetInt( INT32U iVal ){ mData.iVal = iVal; DDVInt(); Int2Str( mData.iVal, mBuf ); }
	INT32U 	GetInt(void){ return mData.iVal; }

	/// float Editbox
	void	SetFP( FP64 fVal ){ mData.fVal = fVal; DDVFP(); 	FP2Str( mData.fVal, mBuf ); }
	FP64 	GetFP(void){ return mData.fVal; }

	INT32U	GetType( void ) { return mBoxType; }
	void	SetRangeCHAR( INT16U maxchar ) 
	{ 
		mMaxChar = maxchar;
	}

	void	SetRangeInt( INT32S min, INT32S max ) 
	{ 
		if ( mBoxType == ENM_EB_TYPE_INT ) 
		{
			mData.iMinVal = min;
			mData.iMaxVal = max;
		
			mMaxChar = CalRangeInt( max, min );
			Int2Str( mData.iVal, mBuf );

			Update();
		}
	}

	void	SetRangeFP( FP64 min, FP64 max, INT8U precision )
	{ 
		if ( mBoxType == ENM_EB_TYPE_FLOAT ) 
		{
			mData.fMinVal = min;
			mData.fMaxVal = max;

			mPrecision = precision;
			mMaxChar = mPrecision + CalRangeFP( max, min );
			FP2Str( mData.fVal, mBuf );
		
			Update();
		}
	}

	void  EnableMultiKeyPress( BOOLEAN bEnable = TRUE )
	{
		mbMultiKeyPress = bEnable;
	}

	virtual void	Flush(BOOLEAN bToVal=TRUE);	///< editbuf --> internal buf

 protected:
	void	Init( const tagEditBoxCS& eCs );
	virtual void	OnKey( INT16U key );

	/// Draw page routines
	virtual void	Draw(void);
	virtual void	DrawTitle(void);
	virtual void	DrawFrame(void);
	virtual void	DrawBoxText(void);
	
	virtual void	InputKeyProc( INT16U key, BOOLEAN bCurMove = TRUE );

	virtual void	DDV(void);	///< verify input char's validate

	INT32S	BoxPosX(void) { return mpParent->mX + mX + mTitleWidth; }
	INT32S	BoxPosY(void) { return mpParent->mY + mY+2; }
	void	CursorMove(INT16S step = 1);

	/// catalog routines
	void	InitCh(const tagEditBoxCS& eCs);
	void	FlushCh(BOOLEAN bToVal=TRUE);	///< editbuf --> internal buf
	void	DDVCh(void);	///< verify input char's validate

	void	InitInt(const tagEditBoxCS& eCs);
	void	FlushInt(BOOLEAN bToVal=TRUE);	///< editbuf --> internal buf
	void	DDVInt(void);	///< verify input char's validate
	void	Int2Str( INT32S val, MCHAR* pDst );
	//INT16U	CalCharRangeInt(void);

	void	InitFP(const tagEditBoxCS& eCs);
	void	FlushFP(BOOLEAN bToVal=TRUE);	///< editbuf --> internal buf
	void	DDVFP(void);	///< verify input char's validate
	void	FP2Str( FP64 val, MCHAR* pDst );
	//INT16U	CalCharRangeFP(void);

private:


/// member variables
	struct tagBoxVal
	{
		union
		{
			INT32S	iVal;
			FP64	fVal;
		};
		union
		{
			INT32S	iMaxVal;
			FP64	fMaxVal;
		};
		union
		{
			INT32S	iMinVal;
			FP64	fMinVal;
		};
	};

public:

protected:
	static	MCHAR mEditBuf[ENM_TITLE_MAX];	///< for char input temp buf

	BOOLEAN		mbChanged;	///< flag, for Editbox's contant changing
	MCHAR		mBuf[ENM_TITLE_MAX];	///< buf for keeping user's input char
	INT16U		mTitleWidth;	///< Title's width in the whole editbox
	

	INT16U		mCursorPos; ///< cursor's pos in editbox
	INT16U		mStrSize;	///< Char's count in editbox
	INT16U		mMaxChar;	///< Limited char's count
	
	INT16U		mBoxType;	///< Limited char's count
	tagBoxVal	mData;

	INT16U		mPrecision;	///< Limited frag's count(only for float editbox)

	/// multi key response flag
	BOOLEAN		mbMultiKeyPress;

	/// Key up/down char table
	enum  { CHAR_TABLE_MAX = 43 };
	static MCHAR	mCharTable[CHAR_TABLE_MAX];
	INT8U		mCurTablePos;
};



class MigEditBoxGrp : public MigControl
{
/// member routines
public:
	MigEditBoxGrp( MigObj *pParent = NULL );
	MigEditBoxGrp( const tagCS& cs );
	~MigEditBoxGrp( ){ }
	
	void	SetBoxGrpParam( tagEditBoxCS* eCs, INT8U cnt );
	
	void	SetTitle( MCHAR* pszTitle );
	void	SetX( INT32S x );
	void	SetY( INT32S y );
	void	SetXY( INT32S x, INT32S y ){ SetX( x ); SetY( y); }

	void	SetBoxStr( MCHAR* pStr, INT8U index );
	void	SetBoxInt( INT32S iVal, INT8U index );
	void	SetBoxFP ( FP64 fVal,   INT8U index );

	MCHAR*	GetBoxStr( INT8U index );
	INT32S	GetBoxInt( INT8U index );
	FP64	GetBoxFP ( INT8U index );

	BOOLEAN		IsInEdit(void);
	
	void VisibleItem( INT16U item, BOOLEAN bVisible )
	{
		mItemSet[item].box.Visible( bVisible );
		mItemSet[item].title.Visible( bVisible );
	}

protected:
	void	Init(void);	
	void	SetItemParam( INT8U index, const tagCS& cs, const tagEditBoxCS& ecs );
	INT32U	GetItemW( INT8U index );
	INT16U	CalRange( tagEditBoxCS& ecs );
	
	virtual void	Draw();
	virtual void	DrawTitle(void);

		virtual void	OnKey( INT16U key );
	virtual void	FocusOn(void);
	virtual void	FocusOff(void);


/// member variables
struct tagBoxItem
{
	MigControl  title;
	MigEditBox	box;
};

protected:
	enum { EB_MAX = 4 };
	tagBoxItem	mItemSet[EB_MAX];
	INT8U		mItemCnt;
	INT8U		mCurItem;	///< current active editbox
};

class MigCtrlGrp
{
public:
	MigCtrlGrp(INT8U s = 32 )
	{
		mSize 	= s;
		mCount 	= 0;
		mArray = new MigControl *[32];//mSize+1];
	}
	
	~MigCtrlGrp() { delete []mArray; }
		
	void 	AddToGroup( MigControl *p )
	{
	//	if ( p && mCount < mSize )
			mArray[mCount++] = p;	
	}

	void	DelFromGroup( MigControl *p )
	{
		INT8U i;
		if ( p )
		{
			for ( i = 0; i < mCount; i++ )
			{
				if ( mArray[i] == p )
				{
					mArray[i] = 0;
					memcpy( &mArray[i], &mArray[i+1], sizeof(MigControl*) * (mCount - i-1) );
					mCount--;
					break;
				}
			}
		}

	}

	void	Enable( BOOLEAN bEnable )
	{
		for ( INT8U i = 0; i < mCount; i++ )
		{
		//	mArray[i]->SetStyle( ENM_CTRL_DISABLE);
			mArray[i]->Enable( bEnable );
		}
	}

	void	Visible( BOOLEAN bVisible )
	{
		// temp code, now suitable
		for ( INT8U i = 0; i < mCount; i++ )
		{
		//	mArray[i]->SetStyle( ENM_CTRL_DISABLE);
			mArray[i]->Visible( bVisible );
		}
	}

	void	VisibleItem( INT32U itemID, BOOLEAN bVisible )
	{

	}

	void	EnableItem( INT32U itemID, BOOLEAN bEnable )
	{
	}
	void 	Update( void )
	{

	}

	void 	Clear( void ) { mCount = 0; }

protected:
	INT8U		mCount;
	INT8U		mSize;
	MigControl 	**mArray;	
};

///////////////////////////////////////////
////    Grid box
///////////////////////////////////////////
class MigGrid : public MigControl
{
/// member routines
public:
	MigGrid( MigObj *pParent = NULL );
	MigGrid( const tagCS& cs,  const tagGridBoxCS& bcs );
	~MigGrid( );

	void	SetItemTitle( MCHAR* pszTitle, INT16U count );	///< set all title, count: title's count
	void	SetItemTitle( INT16U idx, MCHAR* pszTitle );
	void	SetItemTitle( INT16U idx, INT16U titleID );
	void	SetItemVal( INT16U	ln, INT16U	column, void *pVal );
	BOOLEAN	GetItemVal( INT16U	ln, INT16U	column, void *pVal );
	
	void	SetItemRangeCHAR( INT16U	ln, INT16U	column, INT16U maxChar );
	void	SetItemRangeFP( INT16U	ln, INT16U	column, FP64 fmin, FP64 fmax, INT8U precision );
	void	SetItemRangeInt( INT16U	ln, INT16U	column, INT32S min, INT32S max );

	void	EnableLine( INT16U	ln, BOOLEAN bEnable = TRUE );
	void	VisibleLine( INT16U	ln, BOOLEAN bVisible = TRUE );
	void	VisibleItem( INT16U	ln, INT16U col, BOOLEAN bVisible = TRUE );
	void	EnableColumn( INT16U	column, BOOLEAN bEnable = TRUE );
	void	VisibleColumn( INT16U	column, BOOLEAN bVisible = TRUE );
	//void	EraseCell( INT16U ln, INT16U column );
	void	EraseColumn( INT16U column );
	void	EraseLine( INT16U ln );

	void	EnableMultiKeyPress( INT16U	column, BOOLEAN bEnable = TRUE );

	void	EnableAll( BOOLEAN bEnable = TRUE );
	void	VisibleAll( BOOLEAN bVisible = TRUE );

	INT16U	GetCurLine(void) { return mCurLine; }
	INT16U	GetCurLineBox(void) { return mCurLine - mUpperLine; }
	INT16U	GetTotalLine(void) { return mUsedLine; }

	INT16U	GetCurColumn(void) { return mCurColumn; }
	MigEditBox* GetFocusedItem() { return &mpItemBox[(mCurLine - mUpperLine) * mGCS.columnDisp + (mCurColumn - mLeftColumn)]; }

	INT16U	AddLine( void );		///< Return: -1 ---failed, others ---added line's index
	INT16U	DeleteLine( INT16U ln );
	void	DeleteAllLine();

	INT16U	TopLine( void );		///< move display window to top; return: current line's index
	INT16U	BottomLine( void );		///< move display window to bottom line; return: current line's index


	virtual void	FocusOn(void);
	virtual void	FocusOff(void);
	virtual void	ItemFocusOffAll( );
	virtual void	ItemFocusOn(INT16U ln, INT16U col );
	virtual void	ItemFocusOff(INT16U ln, INT16U col );

	BOOLEAN	 IsActive(void);
	//virtual BOOLEAN		IsInEdit(void){ return mUsedLine <= 0 ? FALSE : MigControl::IsInEdit(); }
	virtual BOOLEAN		IsInEdit(void)
	{ 
		return mUsedLine <= 0 ? FALSE : mpItemBox[(mCurLine - mUpperLine) * mGCS.columnDisp + (mCurColumn - mLeftColumn)].IsInEdit(); 
	}

	BOOLEAN OverButtom() { return mBottomPos > 1 ? TRUE : FALSE; }
	BOOLEAN OverTop() { return mTopPos > 1 ? TRUE : FALSE; }

	BOOLEAN OverLeft() { return mLeftPos > 1 ? TRUE : FALSE; }
	BOOLEAN OverRight() { return mRightPos > 1 ? TRUE : FALSE; }

protected:
	void	Init( const tagGridBoxCS& bcs );	
	void	InitItem( void );
	void	SetItemParam( INT16U ln, INT16U column, const tagCS& cs, const tagEditBoxCS& ecs  );
	void	ResetFocusItem( void );

	void	Draw();
	void	DrawTitle( void );
	void	DrawFrame( void );
	void	DrawItem( void );
	
	virtual void	OnKey( INT16U key );
	virtual void	Layout(void){}

	virtual void	OnKeyLeft();
	virtual void	OnKeyRight();

/// member variables
protected:
	enum  { MIG_GRIDBOX_COLUMN_MAX = 16 };

	MCHAR		mpszItemTitle[MIG_GRIDBOX_COLUMN_MAX][32];	///< Item title
	INT32U		mItemTitleID[MIG_GRIDBOX_COLUMN_MAX];	///< Item title
	INT16U		mColumnWidth[MIG_GRIDBOX_COLUMN_MAX];	///< column width
	INT16U		mBoxType[MIG_GRIDBOX_COLUMN_MAX];		///< column box type

	MigEditBox	*mpItemBox;				///< Item box

	tagGridBoxCS	mGCS;

	INT16U		mUsedLine;		///< current used lines

	INT16U		mUpperLine;		///< current upper line index
	INT16U		mLeftColumn;	///< current left column index

	INT16U		mCurColumn;
	INT16U		mCurLine;

	INT16U		mTopPos, mBottomPos;
	INT16U		mLeftPos, mRightPos;

	BOOLEAN		mbHaveTitle;
};

#endif	// __MIG_EDITBOX_H__

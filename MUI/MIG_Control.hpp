#ifndef __MIG_CONTROL_H__
#define __MIG_CONTROL_H__

#include  "MIG_Base.hpp"
#include  "MIG_Obj.hpp"

class MigControl : public MigObj
{
public:
	MigControl( MigObj *pParent = NULL );
	MigControl( const tagCS& cs );
	virtual ~MigControl();

	virtual void	SetParent( MigObj* pParent );
	virtual void	SetChild( MigObj* pChild );

	virtual  void	Enable( BOOLEAN bEnable = TRUE );

	virtual void	OnMessage( const tagMigMSG& stMsg );
	virtual void	FocusOn(void);
	virtual void	FocusOff(void);

	virtual BOOLEAN		IsInEdit(void){ return (mStyle & ENM_CTRL_IN_EDIT) ? TRUE : FALSE; }
	virtual BOOLEAN		IsFocusOn(void) { return (mStyle & ENM_CTRL_FOCUSED) ? TRUE : FALSE; }
	virtual BOOLEAN		IsActive(void);

	INT8U		GetType(void) { return mType; }

	virtual void	InEditMode( void ) 
	{ 
		if ( !(mStyle & (ENM_CTRL_STATIC | ENM_CTRL_DISABLE | ENM_OBJ_HIDEN)) )
		{
			mStyle |= ENM_CTRL_IN_EDIT; 
		}
	}

	virtual void	OutEditMode( void ) { mStyle &= ~ENM_CTRL_IN_EDIT; }

	virtual void	Update(void) { if ( mErasable ) Draw(); }
	static  void	EnableErase(void ) { mErasable = TRUE; }
	void  Refresh() { mbErase = TRUE; }

	void  Move( INT16S x, INT16U y ) { mX = x; mY = y; Refresh(); }

protected:
	void			Init(void);
	virtual void	Draw(void);

	/// Draw page routines
	virtual void	DrawTitle(void);
	virtual void	DrawFrame(void);


private:


/// member variables
public:

protected:
	BOOLEAN		mbDocked;	///< flag, when add to a page, docked, otherwise undocked
	BOOLEAN		mbNotify;	///< flag, when content change, notify its parent 
	
	INT8U		mType;		///< Control type
	static	INT32U	mCtrlID;
	BOOLEAN		mbErase;	/// flag of erase background

private:		
	static  BOOLEAN mErasable;
};

/////////////// Up/Down control

class MigUpDn : public  MigControl
{
public:
	enum { ITEM_MAX = 32 };
	enum MigUpDnType { MIG_UD_INT, MIG_UD_STR };

	MigUpDn( MigObj *pParent = NULL );
	MigUpDn( const tagCS& cs );
	virtual ~MigUpDn();

	INT16U	GetCurSel( void );
	INT16U	GetTotalItem( void );
	void	SetCurSel( INT16U sel );
	BOOLEAN	SetItems( MCHAR** pszItems, INT16U total );
	BOOLEAN SetItemInt( INT16U minVal, INT16U maxVal, INT16U step );

	BOOLEAN	SetItems( U16* pItems, INT16U total );
	MCHAR*   GetItemStr( U16 idx );

	virtual void	FocusOn(void);
	virtual void	FocusOff(void);

protected:
	void	Init(void);

	/// Draw page routines
	virtual void	Draw(void);
	virtual void	DrawTitle(void);
	virtual void	DrawFrame(void);
	virtual void	DrawBoxText(void);

	virtual void OnKeyUp( void );
	virtual void OnKeyDown( void );
	virtual void OnKeyESC( void );
	virtual void OnKeyOK( void );

private:
	INT32S	BoxPosX(void) { return mpParent->mX + mX ; }
	INT32S	BoxPosY(void) { return mpParent->mY + mY+2; }
	void	ReleaseMem( void );

/// member variables
protected:	
	BOOLEAN		mbCycled;	///< Item switch cyclely
	INT8U		mItemType;	///< MIG_UD_INT, MIG_UD_STR; default: MIG_UD_INT

	INT8U		mCurSel;	
	INT8U		mItemMax;
	
	INT16U		mMinVal;
	INT16U		mMaxVal;
	INT16U		mStep;

	MCHAR		*mppszItem[32];	///< Item's string
	MCHAR		mszItem[ITEM_MAX][32];	///< Item's string
	INT16U		mItemId[ITEM_MAX];
	BOOLEAN		mbStringIdx;

};

/////////////// radio control

class MigRadioBox : public  MigControl
{
public:
	MigRadioBox( MigObj *pParent = NULL );
	MigRadioBox( const tagCS& cs );
	virtual ~MigRadioBox();

	BOOLEAN	GetState( void );
	void	SetState( BOOLEAN bCheck );

//	virtual void	FocusOn(void);
//	virtual void	FocusOff(void);

protected:
	void	Init(void);

	/// Draw page routines
	virtual void	Draw(void);
	virtual void	DrawTitle(void);
	virtual void	DrawFrame(void);
	virtual void	DrawCheckBox(void);

//	virtual void OnKeyESC( void );
	virtual void OnKeyOK( void );

private:

/// member variables
protected:	
	BOOLEAN		mbChecked;
	INT32U		mBoxWidth;
};


/////////////// Comb control  
struct tagCombCellInfo
{
	INT32U width,  height;
	INT32U lnCnt,  colCnt;
	INT8S* pTag; 
	INT8U  stateCnt;

};

class EnmComb : public  MigControl
{
public:
	EnmComb( MigObj *pParent = NULL );
	EnmComb( const tagCS& cs );
	virtual ~EnmComb();

	void	SetCellInfo( const tagCombCellInfo& cci );
	INT8U	GetState( INT32U ln, INT32U col );
	void	SetState( INT32U ln, INT32U col, INT8U state = 0 );
	void	SetAllState( INT8U state = 0 );

	INT32U	GetMaxLn() { return mCellInfo.lnCnt; }
	INT32U	GetMaxCol() { return mCellInfo.colCnt; }
	INT32U  GetCurLn() { return mCurLn; }
	INT32U  GetCurCol() { return mCurCol; }

	BOOLEAN OverButtom() { return mBottomPos > 1 ? TRUE : FALSE; }
	BOOLEAN OverTop() { return mTopPos > 1 ? TRUE : FALSE; }

	virtual void	FocusOn(void)
	{
		MigControl::FocusOn();

		mCurLn = mCurCol = 0;
		mBottomPos = mCurLn < mCellInfo.lnCnt-1 ? 0 : 1; 
		mTopPos = mCurLn > 0 ? 0 : 1; 
	}

	//virtual void	FocusOff(void);

protected:
	void	Init(void);

	/// Draw page routines
	virtual void	Draw(void);
	void	DrawTips( void );
	void	DrawAllCells(void);
	void	DrawCell(INT32U ln, INT32U col );
	void	DrawCurSel( BOOLEAN bTrans );

	virtual void OnKeyOK( void );

	void  OnKeyUp( void )
	{
		DrawCurSel( 1 );
	
		if ( mCurLn > 0 )
		{
			mCurLn--;
			mTopPos = 0;
		}
		
		if ( mCurLn == 0 )
			mTopPos++;

		DrawCurSel( 0 );
	}
		
	void	OnKeyDown( void )
	{
		DrawCurSel( 1 );
	
		if ( mCurLn < mCellInfo.lnCnt-1 )
		{
			mCurLn++;
			mBottomPos = 0;
		}
		
		if ( mCurLn == mCellInfo.lnCnt-1 )
			mBottomPos++;

		DrawCurSel( 0 );
	}
		
	void	OnKeyLeft( void )
	{
		DrawCurSel( 1 );
	
		if ( mCurCol > 0 )
			mCurCol--;
		DrawCurSel( 0 );
	}

	void	OnKeyRight(void )
	{
		DrawCurSel( 1 );
	
		if ( mCurCol < mCellInfo.colCnt-1 )
			mCurCol++;
		DrawCurSel( 0 );
	}

private:

/// member variables
protected:	
	tagCombCellInfo	mCellInfo;
	INT8U	*mpCellState;
	INT32U	mCurLn, mCurCol;

	INT32U mStartX ,mStartY, mStep;

	INT8S	mTopPos, mBottomPos;
};

#endif	// __MIG_CONTROL_H__
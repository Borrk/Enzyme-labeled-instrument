#ifndef __MIG_OBJ_H__
#define __MIG_OBJ_H__

#include  "MIG_Base.hpp"
#include  "MStrMngr.hpp"
 
class MigObj
{
public:
	MigObj( MigObj *pParent = NULL );
	MigObj( const tagCS& cs );
	virtual ~MigObj();
	
	/// node pointer
	MigObj*	Next(void){ return mpNext; }
	MigObj*	Previous(void) { return mpPre; }
	MigObj*	Parent(void) { return mpParent; }
	MigObj*	Child(void) { return mpChild; }
	MigObj*	Child(INT32U ChildNo );

	void	SetNext( MigObj* pNext );
	void	SetPrevious( MigObj* pPre );
	virtual void	SetParent( MigObj* pParent );
	virtual void	SetChild( MigObj* pChild );

	/// Object ID routines
	INT32U		GetID(void) { return mID; }
	void		SetID( INT32U id ) { mID = id; }
	void		GetTitle( MCHAR* pszDst, INT16U size );
	void		SetTitle( MCHAR* pszTitle, BOOLEAN bErase = TRUE );
	void		SetTitle( INT32U titleID, BOOLEAN bErase = TRUE );
	INT32U		GetStyle(void) { return mStyle; }
	void		SetStyle( INT32U style ) { mStyle = style; }

	virtual  void	Enable( BOOLEAN bEnable = TRUE )
	{ 
		bEnable ? (mStyle &= ~ENM_CTRL_DISABLE ) : (mStyle |= ENM_CTRL_DISABLE);
	}

	virtual  void	Visible( BOOLEAN bVisible = TRUE )
	{ 
		INT32U	aFlag = ENM_OBJ_HIDEN | ENM_CTRL_IN_EDIT;
		bVisible ? (mStyle &= ~aFlag ) : (mStyle |= aFlag);
	}
	/// rect
	void		SetSize( const tagRect& rc );

	// Message proc
	virtual void	OnMessage( const tagMigMSG& stMsg ){}
	virtual void	FocusOn(void){}
	virtual void	FocusOff(void){}

	virtual void	Update(void) { Draw(); }


//protected:
	void	Init(void);
	
	virtual void	Draw(void) = 0;
	virtual void	BeginDraw(void){}
	virtual void	EndDraw(void){ }		///< 
	/// Draw routines
	virtual void	DrawTitle(void){}
	virtual void	DrawFrame(void){}

	virtual void	OnKey( INT16U key );

	virtual	void	OnNumKey( INT16U key ){}
	virtual	void	OnKeyOK( void ){}
	virtual	void	OnKeyGO( void ){}
	virtual	void	OnKeyESC( void ){}
	virtual	void	OnKeyUp( void ){}
	virtual	void	OnKeyDown( void ){}
	virtual	void	OnKeyLeft( void ){}
	virtual	void	OnKeyRight(void ){}
	virtual	void	OnKeyTest( void ){}
	virtual	void	OnKeyPrg( void ){}
	virtual	void	OnKeyParam( void ){}
	virtual	void	OnKeyData( void ){}
	virtual	void	OnKeyPrnt( void ){}
	virtual	void	OnKeySign( void ){}
	virtual	void	OnKeyDot( void ){}
	virtual	void	OnKeyFunc1( void ){}
	virtual	void	OnKeyFunc2( void ){}
	virtual	void	OnKeyFunc3( void ){}
	virtual	void	OnKeyFunc4( void ){}

	MCHAR Key2Char( INT16U key, INT16U cnt = 0 );
private:


/// member variables
public:
	INT32S	mX, mY;
	INT32S	mWidth, mHeight;

protected:
	MCHAR 	mszTitle[ENM_TITLE_MAX];
	INT16U	mChildCnt;
	INT32U	mStyle;

	INT32U	mTitleID;
//private:		

	INT32U		mID;

	MigObj*	mpNext;
	MigObj*	mpPre;
	MigObj*	mpChild;
	MigObj*	mpParent;

};

#endif	// __MIG_OBJ_H__
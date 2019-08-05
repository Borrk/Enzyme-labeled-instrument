#ifndef __MIG_PAGE_H__
#define __MIG_PAGE_H__

#include  "MIG_Obj.hpp"
#include  "MIG_Control.hpp"

class MigPage : public MigObj
{
public:
	MigPage( MigObj *pParent = NULL );
	MigPage( const tagCS& cs );
	virtual ~MigPage();

	/// Focus switch
	static MigPage* GetCurFocusPage(void);
	static void		SetCurFocusPage(MigPage* pPage );
	static MigPage*		Switch2Page( INT32U id, MigPage *pReturnPage = NULL );
	static MigPage* FindPageByID( MigPage* p, INT32U id);

	void	SetReturnPage( MigPage* pPage ) { mpReturnPage = pPage; }

	/// Message proc
	virtual void	OnMessage( const tagMigMSG& stMsg );
	virtual void	OnUserMsg( const tagMigMSG& stMsg );
	virtual void	BroadcastMsg( tagMigMSG msg );
	virtual void	FocusOn(void);
	virtual void	FocusOff(void);
 
	/// Control relative routines
	void        AddControl( MigControl* pCtrl );	///< Add a control to the page
	MigControl* AddControl( const tagCS& cs );		///< Create and Add a control to the page
	void		DeleteAllCtrls(void);
	
	MigControl* GetCurCtrl( void ){ return mpCurCtrl; } 
	MigControl* GetControl( INT32U id );			///< by id
	MigControl* GetControlByIndex( INT16U idx );	///< by index
	MigControl* GetLastControl(  );	
	void		FocusControl( INT32U id );				///< set Focus to control by id

	void		ControlFocusSw( MigControl *pCtrl = NULL, BOOLEAN bDown = TRUE );	///< pCtrl == NULL, switch forward
	void		ControlFocusUp( void ){ ControlFocusSw( mpCurCtrl ? (MigControl*)(mpCurCtrl->Previous()) : NULL); }
	void		ControlFocusDown( void ){ ControlFocusSw( mpCurCtrl ? (MigControl*)(mpCurCtrl->Next()) : NULL); }

	void		InvalidateRect( tagRect& rc, BOOLEAN bRepaint = FALSE );
	
	// temp code
static void DrawAll( void)
{
#if 1
	mpCurPage->Update();
#else
	 MigPage* p = mpFirstPage;
	 DrawEx( p );
#endif
}
static void DrawEx( MigPage* p )
{
	if ( p->Child() == 0 )
	{
		p->Draw();
		if ( p->Next() )
		{
			p = (MigPage*)p->Next();
			DrawEx( p );
		}
		else
		{
			p->Draw();
		}
	}
	else
	{
		p = (MigPage*)p->Child();
		DrawEx( p);
	}
}

virtual void SetParam(void) {}

protected:
	void		 Init(void);
	virtual void Draw(void);
	
	/// Draw page routines
	virtual void	DrawTitle(void);
	virtual void	DrawFrame(void);
	void			DrawControls(void);

	/// Message proc
//	void	OnKey( INT16U key );

	virtual	void	OnNumKey( INT16U key );
//	virtual	void	OnKeyOK( void );
	virtual	void	OnKeyESC( void );
	virtual	void	OnKeyUp( void );
	virtual	void	OnKeyDown( void );
/*	virtual	void	OnKeyLeft( void );
	virtual	void	OnKeyRight(void );
	virtual	void	OnKeyTest( void );
	virtual	void	OnKeyPrg( void );
	virtual	void	OnKeyParam( void );
	virtual	void	OnKeyData( void );
	virtual	void	OnKeyPrnt( void );
	virtual	void	OnKeySign( void );
	virtual	void	OnKeyDot( void );
	virtual	void	OnKeyFunc1( void );
	virtual	void	OnKeyFunc2( void );
*/
	/// Control Focus proc
	MigControl*		FindActiveCtrl( MigControl* pStartCtrl = NULL, BOOLEAN bDown = TRUE );

	// other assist routines
	INT16U	MigPage::NumKey2Index( INT16U key );	///< convert num-key to index: 0 ~ 9, -1 invalid


private:


/// member variables
public:
	
//protected:
	static	INT32U		mPageCnt;	///< all MigPage instances count

	static MigPage		*mpCurPage;
	static MigPage		*mpFirstPage;


	MigPage				*mpReturnPage;	///< return page if not equal NULL, else return to parent

	MigControl			*mpCurCtrl;
	MigControl			*mpChildCtrlList;
	 
private:		

};

#endif	// __MIG_PAGE_H__
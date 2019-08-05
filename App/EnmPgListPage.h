// EnmPgListPage.h: interface for the CEnmPgListPage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef  __EnmPgListPage_H
#define  __EnmPgListPage_H

#include "EnmBasePage.hpp"
#include "Mig_EditBox.hpp"

class CEnmPgList : public MigGrid
{
/// member routines
public:
	CEnmPgList( MigObj *pParent = NULL );
	CEnmPgList( const tagCS& cs,  const tagGridBoxCS& bcs );
	~CEnmPgList( ){ }

protected:	
//	virtual void	OnKey( INT16U key );
	virtual void	OnKeyOK(void);

	virtual void	Layout( void);	///< set layout by current line and comumn

protected:
	void			SetList(void);

/// member variables
protected:
};

class CEnmPgListPage : public CEnmBasePage  
{
	typedef CEnmBasePage inherited;
	
public:
	CEnmPgListPage();

public:	
	BOOLEAN		OnForward();
	BOOLEAN		OnExit();

protected:
	void		CreateControl();

	void		FocusOn();
	
protected:
	void		SyncProtocol2UI();

protected:
	CEnmPgList		*mpPgList;
};

#endif ///< __EnmPgListPage_H

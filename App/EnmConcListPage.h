// EnmConcListPage.h: interface for the CEnmConcListPage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENMCONCLISTPAGE_H__F2DC0461_2071_4178_9337_62720A867863__INCLUDED_)
#define AFX_ENMCONCLISTPAGE_H__F2DC0461_2071_4178_9337_62720A867863__INCLUDED_

#include "EnmBasePage.hpp"
#include "Mig_Editbox.hpp"
#include "EnmDef.h"	// Added by ClassView

/**
 *		OD. 清单列表类
 *      继承自MigGrid
 */
class CEnmConcList : public MigGrid
{
	typedef MigGrid  inherited;

/// member routines
public:
	CEnmConcList( MigObj *pParent = NULL );
	CEnmConcList( const tagCS& cs,  const tagGridBoxCS& bcs );
	~CEnmConcList( ){ }

	void	SetConc( void* pData, INT32U count );
	U16		GetConc( void* pData, U16 count );

protected:	
//	virtual void	OnKey( INT16U key );
//	virtual void	OnKeyOK(void);
	void			OnKeyLeft();
	void			OnKeyRight();
	
	virtual void	Layout( void);	///< set layout by current line and comumn

protected:
	void			SetList(void);

/// member variables
protected:
	void*			mpData;
};

class CEnmConcListPage : public CEnmBasePage  
{
	enum { MAX_PAGE = 2 };
	typedef  CEnmBasePage  inherited;
	
public:
	CEnmConcListPage();
	virtual ~CEnmConcListPage();

public:
	BOOLEAN			IsEditable() const;
	void			EnableEdit( BOOLEAN bEnable = TRUE );

protected:
	void			CreateControls();
	void			FocusOn();
	void			OnKeyOK();
	void			OnKeyLeft();
	void			OnKeyRight();
	
	void			OnKeyPrnt();

protected:
	void			VisibleListFrom( U16 pos, BOOLEAN bVisible );
	virtual void	SetPageData( S8 page );
	
protected:
	U16				CalculateConc();

protected:
	CEnmConcList		*mpConcList;
	S8					mCurPage;
	BOOLEAN				mbEditable;
};


inline BOOLEAN	 CEnmConcListPage::IsEditable() const
{
	return mbEditable;
}

inline void	 CEnmConcListPage::EnableEdit( BOOLEAN bEnable )
{
	mbEditable = bEnable;
}

#endif // !defined(AFX_ENMCONCLISTPAGE_H__F2DC0461_2071_4178_9337_62720A867863__INCLUDED_)

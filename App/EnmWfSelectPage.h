#ifndef __ENK_WFO_PAGE_H__
#define __ENK_WFO_PAGE_H__

#include "EnmBasePage.hpp"

class  CEnmWfoSelector;

class CEnmWfSelectPage : public CEnmBasePage
{
public:
	CEnmWfSelectPage(MCHAR* pTitle, U32 pid );
	CEnmWfSelectPage(U32 titleID, U32 pid );

	BOOLEAN				CreateItems( U32 *pItems, U32 count );
	BOOLEAN				SelectItem( S32 item );

	S32					GetCurSel()
	{
		GetCurCtrl() ? GetCurCtrl()->GetID() - 1 : -1;
	}

	void				SetWfoSelector( CEnmWfoSelector& selector );
	CEnmWfoSelector*	GetWfoSelector( );
	
protected:
	BOOLEAN				CreateControls( U32 *pItems, U32 count );

	void				OnNumKey( INT16U key );
	void				OnKeyUp( );
	void				OnKeyDown( );

protected: 
	CEnmWfoSelector   *mpWfoSelector;
};

#endif	// __ENK_WFO_PAGE_H__
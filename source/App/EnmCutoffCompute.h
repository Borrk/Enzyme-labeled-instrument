// EnmDataCompute.h: interface for the CEnmDataCompute class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENMDATACOMPUTE_H__4DAFA07F_6881_46E6_8406_8CBB5E004450__INCLUDED_)
#define AFX_ENMDATACOMPUTE_H__4DAFA07F_6881_46E6_8406_8CBB5E004450__INCLUDED_


#include "EnmBasePage.hpp"
#include "EnmCutoffLib.h"

class CEnmCutoffCompute : public CEnmBasePage  
{
	typedef CEnmBasePage inherited;

	enum { CUTOFF_COUNT = CEnmCutoffLib::CUTOFF_COUNT };
public:
	CEnmCutoffCompute();
	virtual ~CEnmCutoffCompute();

protected:
	void		CreateControls();
	void		FocusOn();
	void		OnKeyOK();
	void		OnKeyUp();
	void		OnKeyDown();

protected:
	void		FillHistoryCutoff();
	BOOLEAN		FormatCutoffName( tagCutoffEntry& rCutoff, MCHAR* pName );
	void		SyncPNc2UI();

//////////////////////////////////////////////////////////////////////////
protected:
	MigEditBox		*mpCtrlPc, *mpCtrlNc;
	MigUpDn			*mpCutoffCtrl;

	tagCutoffEntry  mCutoffList[CUTOFF_COUNT];
	U16				mCutoffCount;
};

#endif // !defined(AFX_ENMDATACOMPUTE_H__4DAFA07F_6881_46E6_8406_8CBB5E004450__INCLUDED_)

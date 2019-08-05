
#ifndef __EnmDeleteCutoffPage_H
#define __EnmDeleteCutoffPage_H

#include "EnmBasePage.hpp"
#include "MIG_EditBox.Hpp"

class CEnmDeleteCutoffPage : public CEnmBasePage  
{
	typedef CEnmBasePage inherited;

public:
	CEnmDeleteCutoffPage();
	virtual ~CEnmDeleteCutoffPage();

protected:
	void		CreateControls();
	void		FocusOn();

protected:

//////////////////////////////////////////////////////////////////////////
protected:
	MigControl	*mpDeleteTip;
};

#endif ///< __EnmDeleteCutoffPage_H

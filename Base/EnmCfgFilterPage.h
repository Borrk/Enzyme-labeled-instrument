#ifndef __EnmCfgFilterPage_H
#define __EnmCfgFilterPage_H

#include "EnmSysCfgBasePage.h"

class CEnmCfgFilterPage : public CEnmSysCfgBasePage  
{
	typedef CEnmSysCfgBasePage inherited;
	
public:
	CEnmCfgFilterPage();
	virtual ~CEnmCfgFilterPage();

	BOOLEAN		OnExit();
	
protected:
	void		CreateControls();

	void		FocusOn();
	void		OnKeyOK();

protected:
	MCHAR*	ValidateFilterValue( MCHAR* pszFilter );
	
protected:
	MigEditBox	*mpFilters[FILTER_MAX];
};

#endif	///< __EnmCfgFilterPage_H


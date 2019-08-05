#ifndef __EnmCfgCommuPage_H
#define __EnmCfgCommuPage_H

#include "EnmSysCfgBasePage.h"

class CEnmCfgCommuPage : public CEnmSysCfgBasePage  
{
	typedef CEnmSysCfgBasePage inherited;
	
public:
	CEnmCfgCommuPage();
	virtual ~CEnmCfgCommuPage();

//	BOOLEAN		OnExit();
	
protected:
	void		CreateControls();

	void		FocusOn();
	void		FocusOff();
	
protected:
	MigUpDn		*mpBaud;
};

#endif	///< __EnmCfgCommuPage_H


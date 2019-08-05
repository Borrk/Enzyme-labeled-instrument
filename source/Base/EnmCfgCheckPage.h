#ifndef __EnmCfgCheckPage_H
#define __EnmCfgCheckPage_H

#include "EnmSysCfgBasePage.h"

class CEnmCfgCheckPage : public CEnmSysCfgBasePage  
{
	typedef CEnmSysCfgBasePage inherited;
	
public:
	CEnmCfgCheckPage();
	virtual ~CEnmCfgCheckPage();

	BOOLEAN		OnExit();
	
protected:
	void		CreateControls();

	void		FocusOn();

protected:
	MigEditBox		*mpRunTime;
	MigEditBox		*mpPowerOnTime;
	MigUpDn			*mpClearTime;
};

#endif	///< __EnmCfgCheckPage_H


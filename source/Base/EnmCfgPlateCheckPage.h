#ifndef __EnmCfgPlateCheckPage_H
#define __EnmCfgPlateCheckPage_H

#include "EnmSysCfgBasePage.h"

class CEnmCfgPlateCheckPage : public CEnmSysCfgBasePage  
{
	typedef CEnmSysCfgBasePage inherited;
	
public:
	CEnmCfgPlateCheckPage();
	virtual ~CEnmCfgPlateCheckPage();

	//BOOLEAN		OnExit();
	
protected:
	void		CreateControls();

	void		FocusOn();
	void		FocusOff();

protected:
	MigUpDn		*mpCheck;
};

#endif	///< __EnmCfgPlateCheckPage_H


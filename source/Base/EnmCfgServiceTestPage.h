#ifndef __EnmCfgServiceTestPage_H
#define __EnmCfgServiceTestPage_H

#include "EnmSysCfgBasePage.h"

class CEnmCfgServiceTestPage : public CEnmSysCfgBasePage  
{
	typedef CEnmSysCfgBasePage inherited;
	
public:
	CEnmCfgServiceTestPage();
	virtual ~CEnmCfgServiceTestPage();

	BOOLEAN		OnExit();
	
protected:
	void		CreateControls();

	void		FocusOn();
	void		OnKeyOK();
	void		OnKeyLeft();
	void		OnKeyRight();

	void		OnUserMsg( const tagMigMSG& stMsg );

protected:
	void		SetMaintenanceEnv();
	void		UpdateConcValue();

protected:
	MigEditBox		*mpLamp;
	MigEditBox		*mpFilter;
	MigEditBox		*mpColumn;
	MigEditBox		*mpAverage;
	MigControl		*mpConc;
};

#endif	///< __EnmCfgServiceTestPage_H


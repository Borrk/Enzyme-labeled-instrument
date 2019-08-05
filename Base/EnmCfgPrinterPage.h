#ifndef __EnmCfgPrinterPage_H
#define __EnmCfgPrinterPage_H

#include "EnmSysCfgBasePage.h"

class CEnmCfgPrinterPage : public CEnmSysCfgBasePage  
{
	typedef CEnmSysCfgBasePage inherited;
	
public:
	CEnmCfgPrinterPage();
	virtual ~CEnmCfgPrinterPage();

	//BOOLEAN		OnExit();
	
protected:
	void		CreateControls();

	void		FocusOn();
	void		FocusOff();

protected:
	MigUpDn		*mpPrinter;
	MigUpDn		*mpAutoPrint;
	MigUpDn		*mpPrintMode;
};

#endif	///< __EnmCfgPrinterPage_H


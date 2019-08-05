
#ifndef __EnmCfgTimePage_H
#define __EnmCfgTimePage_H

#include "EnmSysCfgBasePage.h"

class CEnmCfgTimePage : public CEnmSysCfgBasePage
{
	typedef CEnmSysCfgBasePage inherited;

public:
	CEnmCfgTimePage();

	enum 
	{
		ENM_DATE_START = 1, 
		ENM_DATE_DATE = ENM_DATE_START, ENM_DATE_TIME, ENM_DATE_LANGUAGE, ENM_DATE_LANGUAGE_TITLE, ENM_DATE_NAME, ENM_DATE_END
	};

public:
	BOOLEAN		OnForward();
	BOOLEAN		OnExit();

protected:
	void CreateControls( void);

	virtual void    FocusOn()
	{ 
		mbChanged = FALSE;
		SetParam(); 
		MigPage::FocusOn();
	}

	void	OnUserMsg( const tagMigMSG& stMsg );
	void	OnKeyOK( void );

	void  SetParam( void );
	void  SetSysDate( tagDate date );
	void  SetSysTime( tagTime time );

/// member variables
	CEnTimeBox	*mpTimeBox;
	CEnDateBox  *mpDateBox;
	MigUpDn		*mpLanguage;
	MigEditBox	*mpDevName;

	BOOLEAN		mbChanged;
};

#endif ///< __EnmCfgTimePage_H

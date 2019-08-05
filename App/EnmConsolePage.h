#ifndef __ENK_CONSOLE_PAGE_H__
#define __ENK_CONSOLE_PAGE_H__

#include "EnmBasePage.hpp"
#include "EnmConsole.h"

/**
 *		console
 */ 	
class CEnmConsolePage : public CEnmBasePage
{
	typedef CEnmBasePage inherited;
	enum { ENM_CHECKTIP_TIP = 1 };
	
public:
	CEnmConsolePage();
	CEnmConsolePage(const tagCS& cs );

	void			SetConsole( CEnmConsole* pConsole );
	tagScanData*	GetScanData();
	void			SetTip( MCHAR* pTip );

public:
	BOOLEAN  IsCanExit();

protected:
	void			Init(void);
	virtual void	Draw(void);
	virtual void    FocusOn();
	virtual void	OnUserMsg( const tagMigMSG& stMsg );
	void			Scan( void );
	BOOLEAN			StartScan();
	void			GenerateScanOption();
	MCHAR*			GetString( U32 strId );

/// memeber variabls
private:
	CEnmConsole	 *mpConsole;
	BOOLEAN		 mbHaveData;
	MigControl	 *mpTips;
	BOOLEAN		 mbInScan;
};
	
inline
MCHAR*	CEnmConsolePage::GetString( U32 strId )
{
	return MStrMngr::Instance().GetString( strId );
}

#endif	// __ENK_CONSOLE_PAGE_H__

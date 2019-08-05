// EnmConsoleProc.h: 

#ifndef __EnmConsoleProc_H__
#define __EnmConsoleProc_H__

#include "EnmProcEngine.h"
#include "EnmConsole.h"

class CEnmConsolePage;

class CEnmConsoleProc : public IProcEngine  
{
	typedef IProcEngine  inherited;

public:
	CEnmConsoleProc();
	virtual ~CEnmConsoleProc();

public:
	enProcState IEnterProc( ) ;
	BOOLEAN     InitEngine();
 	enProcState IResume( );
 
protected:
	void		OnMessage( tagMigMSG& msg );
	void		OnModeChange( enEnmSysMode mode );

	enEnmSysMode  GetMode();

protected:
	void		ProcConsoleCommand();
	BOOLEAN		SendScanData( tagScanData* pData );
	void		SendAckMsg();

protected:
	CEnmConsole		mConsole;
	CEnmConsolePage	*mpConsolePage;
	U16				mScanCount;
};

#endif // __EnmConsoleProc_H__

#ifndef   __ENM_APP_HPP__
#define   __ENM_APP_HPP__

#include "EnmApi.h"
#include "matrix.h"
#include "EnmFs.h"

#include "MStrMngr.hpp"

#include "EnmBase.hpp"
#include "EnmMainPage.hpp"

/// Mode 
#include "EnmScanContext.h"

/// Engines
#include "EnmMainProc.h"
#include "EnmKeyEngine.h"
#include "EnmScanEngine.h"
#include "EnmConsoleProc.h"

class CEnmWfo;


class  CEnmApp
{
	
/// member routines	
private:
	CEnmApp();
public:
	~CEnmApp();
	static CEnmApp& Instance();

	INT8U					InitApp(void);	///< Return: 1 ---ok, 0 ---failed
	void					AppRun( void );

	void					OnMessage( tagMigMSG& msg );
	
	static  enEnmSysMode	GetSysMode();

public:
	BOOLEAN					RegisterEngine( IProcEngine& rEngine );
	void					UnRegisterEngine( IProcEngine& rEngine );
	INT16U					GetEngineCnt() const;

	CEnmScanContext			&GetScanContext();
	CEnmScanEngine*         GetScanEngine() ;
 	CEnmKeyEngine*			GetKeyEngine() ;

	void					Terminate();

	BOOLEAN					ChangeMode( enEnmSysMode mode );

protected:
	void		Init( void);
	void		DispatchMsg( INT16U msg  );

/// member variables
protected:
	enum {  ENM_APP_MSGENTRY_MAX = 16 };
	enum {  APP_ENGINE_MAX  = 16 };
	
	/// Proc Engine 
	IProcEngine				*mProcEngineList[APP_ENGINE_MAX];
	INT16U					mProcEngineCnt;
	
	/// Main Control proc engine
	CEnmMainProc			mMainProc;
	CEnmConsoleProc			mConsoleProc;
	CEnmKeyEngine			*mpKeyEngine;
	CEnmScanEngine			*mpScanEngine;

	//// Scan context
	CEnmMainPage			*mpMainPage;
	CEnmScanContext			mScanContext;
	
	static INT16U 			mMsg;	
	static enEnmSysMode		mSysMode;
};


//////////////////////////////////////////////////////////////////////////
///  inline routine section
//////////////////////////////////////////////////////////////////////////

inline INT16U CEnmApp::GetEngineCnt() const
{
	return mProcEngineCnt;
}

inline  CEnmScanContext& CEnmApp::GetScanContext() 
{
	return mScanContext;
}

inline 	CEnmScanEngine* CEnmApp::GetScanEngine() 
{
	return mpScanEngine;
}

inline 	CEnmKeyEngine* CEnmApp::GetKeyEngine() 
{
	return mpKeyEngine;
}



#endif  /// __ENM_APP_HPP__
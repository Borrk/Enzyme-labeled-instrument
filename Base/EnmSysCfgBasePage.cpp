#include "EnmSysCfgBasePage.h"
#include "EnmSysCfgLib.h"
#include "MigEventMng.h"

CEnmSysCfgBasePage::CEnmSysCfgBasePage(  MCHAR* pTitle, INT32U id )
: CEnmBasePage( pTitle, id )
{
}

void CEnmSysCfgBasePage::NotifySysCfgChange( U32 uContent, BOOLEAN bSync )
{
	if ( bSync )
		CEnmSysCfgLib::Instance().SysCfgChanged();

	MigEventMng::Instance().PushMsg( ENM_MSG_CFG_CHG, uContent, 0 );
}
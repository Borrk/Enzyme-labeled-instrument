
#ifndef __EnmSysCfgBasePage_H
#define __EnmSysCfgBasePage_H

#include "EnmBasePage.hpp"

class CEnmSysCfgBasePage : public CEnmBasePage
{
public:
	CEnmSysCfgBasePage(  MCHAR* pTitle, INT32U id );

protected:
	void  NotifySysCfgChange( U32 uContent, BOOLEAN bSync = TRUE );	

};

#endif ///< __EnmSysCfgBasePage_H

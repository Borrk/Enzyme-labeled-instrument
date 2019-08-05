#ifndef _EnmWfoRoot_H__
#define _EnmWfoRoot_H__

#include "EnmWfoSelecter.h"

class CEnmWfoRoot : public CEnmWfoSelector
{
public:
	CEnmWfoRoot();

	virtual BOOLEAN  OnForward();

protected:
	BOOLEAN	ChangeSysMode( int mode );
	
};

#endif ///< _EnmWfoRoot_H__
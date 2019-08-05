#ifndef __EnmObserver_H
#define __EnmObserver_H

#include "EnmSubject.h"

class CEnmObserver  
{
public:
	CEnmObserver();
	virtual ~CEnmObserver();

public:
	virtual void	Update( CEnmSubject& subject, CEnmNotify& reson );

};

#endif ///< __EnmObserver_H

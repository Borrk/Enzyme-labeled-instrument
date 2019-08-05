#ifndef _EnmSubject_H
#define _EnmSubject_H

#include "EnmBase.hpp"

class CEnmObserver;

class CEnmNotify
{
public:
	CEnmNotify():mReason(0), mContent(0), mpData(NULL){}
	virtual ~CEnmNotify() { }
	
public:
	U32		mReason;
	U32		mContent;
	void	*mpData;
};

class CEnmSubject  
{
	enum { OBJ_MAX = 16 };
public:
	CEnmSubject();
	virtual ~CEnmSubject();

public:
	BOOLEAN		Attach( CEnmObserver& observer );
	void		Dettach( CEnmObserver& observer );
	void		Notify( CEnmNotify& reason );

protected:
	S16			FindObserver( CEnmObserver& observer ) const;

/// member variables
protected:
	CEnmObserver	*mpObservers[OBJ_MAX];	///< avoid using dynamic memory allocate
	U16				mObserverCnt;
};

#endif ///< _EnmSubject_H

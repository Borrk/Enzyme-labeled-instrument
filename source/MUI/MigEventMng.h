// MigEventMng.h: interface for the MigEventMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIGEVENTMNG_H__D361DB1E_D3C1_4C46_86B7_127DDDFCFE5C__INCLUDED_)
#define AFX_MIGEVENTMNG_H__D361DB1E_D3C1_4C46_86B7_127DDDFCFE5C__INCLUDED_

#include "Mig_Base.hpp"

class MigEventMng  
{
public:
	static MigEventMng& Instance();

public:
	tagMigMSG*	GetMsg();
	BOOLEAN		PushMsg( tagMigMSG& msg );
	BOOLEAN		PushMsg( INT32U msg, INT32U p1 = 0, INT32U p2 = 0 );
	void		ClearAll();

private:
	MigEventMng();

	void Init();

//////////////////////////////////////////////////////////////////////////
/// member variables
	enum { MIG_MSG_POOL_SIZE = 32 };
protected:
	tagMigMSG	mMsgPool[MIG_MSG_POOL_SIZE];
	S16			mFirst;
	S16			mCount;
};

#endif // !defined(AFX_MIGEVENTMNG_H__D361DB1E_D3C1_4C46_86B7_127DDDFCFE5C__INCLUDED_)

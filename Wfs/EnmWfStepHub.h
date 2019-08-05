#ifndef __EnmWfStepHub_H
#define __EnmWfStepHub_H

#include "IWorkStep.h"
#include "CDtKid.h"

class CEnmWfStepHub : public IWorkStep  
{
	enum { WFS_MAX = 2 };

public:
	CEnmWfStepHub( IWorkStep* pParent );
	virtual ~CEnmWfStepHub();

	virtual BOOLEAN  Switch( U16 uPath );
	U16		 GetPathCount() const;
	BOOLEAN	 AddStep( IWorkStep& rStep, U16 path, U16 pos );	

protected:
	BOOLEAN	 IsStepInParent( IWorkStep& rStep );

//////////////////////////////////////////////////////////////////////////
  protected:
	  U16		mCurPath;
	  U16		mPathCount;
	  CDtKid	mPath[WFS_MAX];
};

inline U16 CEnmWfStepHub::GetPathCount() const
{
	return mPathCount;
}

#endif ///< __EnmWfStepHub_H

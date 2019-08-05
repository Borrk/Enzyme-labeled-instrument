// EnmWfoSelecter.h: interface for the CEnmWfoSelecter class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __EnmWfoSelector_H
#define __EnmWfoSelector_H

#include "IWorkStep.h"


class CEnmWfoSelector : public IWorkStep  
{
public:
	CEnmWfoSelector( IWorkStep* pParent );
	virtual ~CEnmWfoSelector();

    virtual BOOLEAN NextStep();
    virtual BOOLEAN PreStep();

	BOOLEAN  Activate(BOOLEAN bActive);
	BOOLEAN  RequestReturn();

	BOOLEAN  SelectStep( U16 sel );

  public:
	virtual BOOLEAN  IsCanForward();
	virtual BOOLEAN  IsCanExit();
	virtual BOOLEAN  OnForward();
	virtual BOOLEAN  OnExit();

//	BOOLEAN  ActiveStep( U16 step, BOOLEAN bActive = TRUE );

  protected:
	void    SyncState();

//////////////////////////////////////////////////////////////////////////
  protected:
	  U16	mSelectOpt;
};

#endif ///< __EnmWfoSelector_H

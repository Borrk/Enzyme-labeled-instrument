#ifndef ENMWFO_H_HEADER_INCLUDED_BBE44C68
#define ENMWFO_H_HEADER_INCLUDED_BBE44C68

#include "IWorkStep.h"

//##ModelId=441BB6DF01F4
class CEnmWfo : public IWorkStep
{
  public:
	CEnmWfo( IWorkStep* pStep );
	~CEnmWfo();
	
    //##ModelId=441BDAFA002E
    virtual BOOLEAN NextStep();

    //##ModelId=441BDBFA02EE
    virtual BOOLEAN PreStep();

	BOOLEAN Activate(BOOLEAN bActive);

	BOOLEAN  RequestReturn();

  public:
	virtual BOOLEAN  IsCanForward();
	virtual BOOLEAN  IsCanExit();
	virtual BOOLEAN  OnForward();
	virtual BOOLEAN  OnExit();

	BOOLEAN  ActiveStep( U16 step, BOOLEAN bActive = TRUE );

  protected:
	BOOLEAN  nActiveStep( U16 step );
	BOOLEAN  nDeActiveStep( U16 step );
	  
//////////////////////////////////////////////////////////////////////////
  protected:
	  U16	mCurStep;
};



#endif /* ENMWFO_H_HEADER_INCLUDED_BBE44C68 */

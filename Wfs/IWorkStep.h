#ifndef IWORKSTEP_H_HEADER_INCLUDED_BBE4350E
#define IWORKSTEP_H_HEADER_INCLUDED_BBE4350E

#include "CDtKid.h"
#include "EnmBasePage.hpp"

//##ModelId=441BB82103D8
class IWorkStep
{
  public:
    IWorkStep(IWorkStep* pParent);
    virtual ~IWorkStep();

    virtual BOOLEAN IStepInit(){ return 1;} // = 0;
    virtual void	IStepDone(){} // = 0;
    virtual BOOLEAN Activate(BOOLEAN bActive);
    U16				GetKidCount() const;
    BOOLEAN			IsActive() const;
	virtual BOOLEAN IsCanActive() const;
    virtual BOOLEAN SetPage(CEnmBasePage* pPage);
    CEnmBasePage*	GetPage() const;
    IWorkStep*		GetParent() const;
    virtual BOOLEAN NextStep();
    virtual BOOLEAN PreStep();

	virtual BOOLEAN Return();
	virtual BOOLEAN RequestReturn();

	virtual BOOLEAN   Switch( U16 uPath ){ return TRUE; }

	static IWorkStep* GetActiveStep();

  public:
	virtual BOOLEAN  IsCanForward();
	virtual BOOLEAN  IsCanExit();
	virtual BOOLEAN  OnForward();
	virtual BOOLEAN  OnExit();
	virtual BOOLEAN  OnPrint();

  public:
    IWorkStep*	GetKidAt(U16 idx) const;
    S16			AddKidAt(IWorkStep& pStep);
	S16			SetKidAt(IWorkStep& pStep, U16 idx );
    BOOLEAN		RemoveKid(U16 idx);

  protected:
    IWorkStep			*mpParent;
	static IWorkStep	*mpActiveStep;
    CEnmBasePage*		mpPage;
    CDtKid				mKid;
    BOOLEAN				mbActive;
};

//##ModelId=441BBD8F0138
inline U16 IWorkStep::GetKidCount() const
{
	return mKid.GetKidCount();
}

//##ModelId=441BD599038A
inline BOOLEAN IWorkStep::IsActive() const
{
	return mbActive;
}

inline 	BOOLEAN  IWorkStep::IsCanActive() const
{
	return GetPage() ? GetPage()->IsCanActive() : FALSE;
}

//##ModelId=441BDA580000
inline CEnmBasePage* IWorkStep::GetPage() const
{
	return mpPage;
}

//##ModelId=441BB9AB0261
inline IWorkStep* IWorkStep::GetParent() const
{
	return mpParent;
}

inline S16 IWorkStep::AddKidAt(IWorkStep& pStep)
{
	return mKid.AddKidAt( pStep );
}

inline BOOLEAN IWorkStep::RemoveKid(U16 idx)
{
	return mKid.RemoveKid( idx );
}

inline S16 IWorkStep::SetKidAt(IWorkStep& pStep, U16 idx )
{
	return mKid.SetKidAt( pStep, idx );
}

inline IWorkStep* IWorkStep::GetKidAt(U16 idx) const
{
	return mKid.GetKidAt( idx );
}


#endif /* IWORKSTEP_H_HEADER_INCLUDED_BBE4350E */

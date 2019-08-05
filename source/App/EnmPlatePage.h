// EnmPlatePage.h: interface for the CEnmPlatePage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENMPLATEPAGE_H__3BDB8C34_4FCF_4EAF_9CC7_DF4F5C98DA36__INCLUDED_)
#define AFX_ENMPLATEPAGE_H__3BDB8C34_4FCF_4EAF_9CC7_DF4F5C98DA36__INCLUDED_

#include "EnmBasePage.hpp"
#include "Mig_EditBox.hpp"


class CEnmPlatePage : public CEnmBasePage 
{
public:
	CEnmPlatePage();
	virtual ~CEnmPlatePage();

protected:
	void			Init(void);
	virtual void	Draw( void );
	void			DrawMatrix(void);
	void			DrawCell( BOOLEAN bHort, U16 idx, MCHAR* pStr );
	void			DrawTips( void );
	void			DrawQualititiveMatrix();
	void			DrawQualitativeMatrix();


//////////////////////////////////////////////////////////////////////////
protected:
	INT16S  mStartX, mStartY, mStepX, mStepY;
	INT16S	mXFrom, mXTo, mYFrom, mYTo;

};

#endif // !defined(AFX_ENMPLATEPAGE_H__3BDB8C34_4FCF_4EAF_9CC7_DF4F5C98DA36__INCLUDED_)

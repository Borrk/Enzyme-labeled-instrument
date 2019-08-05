// SimDrawer.h: interface for the SimDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMDRAWER_H__C3DE9B07_D4F9_4907_86BE_792A19D2283F__INCLUDED_)
#define AFX_SIMDRAWER_H__C3DE9B07_D4F9_4907_86BE_792A19D2283F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\MUI\Mig_Drawer44b0.h"

class SimDrawer : public MigDrawer44b0  
{
public:
	static SimDrawer& Instance();
	virtual ~SimDrawer();

protected:
	SimDrawer();

public:
	void SetDC( HDC hDC );
	virtual void	IBegineDraw();
	virtual void	IEndDraw();

protected:
	HDC		mHDC;
};

#endif // !defined(AFX_SIMDRAWER_H__C3DE9B07_D4F9_4907_86BE_792A19D2283F__INCLUDED_)

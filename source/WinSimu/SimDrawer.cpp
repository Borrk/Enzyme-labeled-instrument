// SimDrawer.cpp: implementation of the SimDrawer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "enmark.h"
#include "SimDrawer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SimDrawer& SimDrawer::Instance()
{
	static SimDrawer aInst;
	return aInst;
}

SimDrawer::SimDrawer()
{
	mHDC = 0;
}

SimDrawer::~SimDrawer()
{

}

void SimDrawer::IBegineDraw()
{
	/// do nothing
}

void SimDrawer::IEndDraw()
{
	S32 i, j, wpln;
	COLORREF clr;

	wpln = GetFrameWidth() / 32;
	for ( i = 0; i < GetFrameHeight(); i++ )
	{
		for ( j = 0; j < GetFrameWidth(); j++ )
		{
			clr = (*(mpVScreen + (i * wpln) + j/32 ) & 0x01 << (32-1-j%32)) ? RGB(255,255,255): RGB(0,0,0);
			::SetPixel( mHDC, j, i, clr );
		}
	}
}

void SimDrawer::SetDC(HDC hDC)
{
	mHDC = hDC;
}

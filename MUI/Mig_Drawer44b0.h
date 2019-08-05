#ifndef __MIG_DRAWER_ARM44B0_H
#define __MIG_DRAWER_ARM44B0_H

#include "Mig_IDrawer.h"

class MigDrawer44b0 : public IMigIDrawer
{
public:
	MigDrawer44b0();
	
public:
	virtual void	IBegineDraw();
	virtual void	IEndDraw();

public:
	virtual void PutStr( int x, int y, char * str , char bSel/*, unsigned short pos*/ );
	virtual void PutStrThin( int x, int y, char * str , char bSel/*, unsigned short pos*/ ) = 0; ///< little font size
	virtual void PutRect( int x, int y, int w, int h ) = 0;
	virtual void PutLine( int x, int y, int x1, int y1 ) = 0;
	virtual void PutBox( int x, int y, int w, int h ) = 0;

	virtual void PutLineV( int x, int y, int x1, int y1 ) = 0;
	virtual void PutRectV( int x, int y, int w, int h, char bTrans ) = 0;


protected:
	virtual void OnSetFrame();

protected:
	U32	_GetRowWordLen() const;
	
	void	_PutPixelMono(U32 x,U32 y,U8 color );

//////////////////////////////////////////////////////////////////////////
/// member variables
//////////////////////////////////////////////////////////////////////////

protected:
	U32		*mpVScreen;
	U32		mWordLen;
};

inline
U32	 MigDrawer44b0::_GetRowWordLen() const
{
	return mWordLen;
}


#endif ///< __MIG_DRAWER_ARM44B0_H

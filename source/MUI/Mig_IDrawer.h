#ifndef __MIG_IDRAWER_H
#define __MIG_IDRAWER_H

#include "EnmBase.hpp"

class IMigIDrawer
{
public:
	IMigIDrawer():mFrameWidth(1), mFrameHeight(1), mBitPerPixel(1){}
	virtual ~IMigIDrawer(){}
	
public:
	INT32U  GetFrameHeight( void ) const;
	INT32U  GetFrameWidth( void )  const;
	INT16U  GetBpp( void ) const;

	void	SetFrameSize( INT32U width, INT32U height );
	void	SetBpp( INT16U bpp );

public:
	virtual void	IBegineDraw() = 0;
	virtual void	IEndDraw() = 0;
	
public:
	virtual void PutStr( int x, int y, char * str , char bSel/*, unsigned short pos*/ ) = 0;
	virtual void PutStrThin( int x, int y, char * str , char bSel/*, unsigned short pos*/ ) = 0; ///< little font size
	virtual void PutRect( int x, int y, int w, int h ) = 0;
	virtual void PutLine( int x, int y, int x1, int y1 ) = 0;
	virtual void PutBox( int x, int y, int w, int h ) = 0;

	virtual void PutLineV( int x, int y, int x1, int y1 ) = 0;
	virtual void PutRectV( int x, int y, int w, int h, char bTrans ) = 0;


protected:
	virtual void OnSetFrame() = 0;

//////////////////////////////////////////////////////////////////////////
/// member variables
//////////////////////////////////////////////////////////////////////////

protected:
	INT32U		mFrameHeight;
	INT32U		mFrameWidth;
	INT16U		mBitPerPixel;
};

inline 	void  IMigIDrawer::SetFrameSize( INT32U height, INT32U width  ) 
{ 
	mFrameHeight = height; 
	mFrameWidth  = width; 
}

inline 	INT32U  IMigIDrawer::GetFrameHeight( void ) const { return mFrameHeight; }
inline 	INT32U  IMigIDrawer::GetFrameWidth( void )  const { return mFrameWidth;  }

inline 	INT16U  IMigIDrawer::GetBpp( void )  const { return mBitPerPixel;  }
inline 	void  IMigIDrawer::SetBpp( INT16U bpp )   { mBitPerPixel  = bpp;  }


#endif ///< __MIG_IDRAWER_H

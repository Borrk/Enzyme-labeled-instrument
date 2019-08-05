#include "Mig_Drawer44b0.h"

#ifdef  ENM_VER_TARGET
#include "include.h"
#else
#include "..\WinSimu\SimuGDI.h"
#endif

MigDrawer44b0::MigDrawer44b0()
{
	mWordLen = 1;
	SetBpp( 1 );
}

void MigDrawer44b0::IBegineDraw()
{
}

void MigDrawer44b0::IEndDraw()
{
}

void MigDrawer44b0::OnSetFrame()
{
	if ( mpVScreen )
	{
		delete []mpVScreen;
		mpVScreen = NULL;
	}

	INT32U aLnSize = 1;
	switch ( mBitPerPixel )
	{
	case 1:
		aLnSize = (mFrameWidth + 7 ) / 8;
		break;

	case 2:
		aLnSize = (mFrameWidth + 3 ) / 4;
		break;

	case 4:
		aLnSize = (mFrameWidth + 1 ) / 2;
		break;

	case 8:
		aLnSize = mFrameWidth;
		break;

	default:
		///now only support 8 bits max
		aLnSize = mFrameWidth;
	}

	mpVScreen = new U32[aLnSize * mFrameHeight];
	for ( U16 i = 0; i < aLnSize * mFrameHeight; i++ )
		mpVScreen[i] = 0;
}

void MigDrawer44b0::PutStr( int x, int y, char * str , char bSel/*, unsigned short pos*/ )
{
//	::PutStr( x, y, str, bSel, -1 );
}

void MigDrawer44b0::PutRect( int x, int y, int w, int h )
{
	::PutRect( x, y, w, h );
}

void MigDrawer44b0::PutLine( int x, int y, int x1, int y1 )
{
	::PutLine( x, y, x1, y1 );
}

void MigDrawer44b0::PutBox( int x, int y, int w, int h )
{
	::PutBox( x, y, w, h );
}

void MigDrawer44b0::PutLineV( int x, int y, int x1, int y1 )
{
	::PutLineV( x, y, x1, y1 );
}
void MigDrawer44b0::PutRectV( int x, int y, int w, int h, char bTrans )
{
	::PutRectV( x, y, w, h, bTrans );
}


void MigDrawer44b0::_PutPixelMono(U32 x,U32 y,U8 color )
{	
    if( x < GetFrameWidth() && y < GetFrameHeight() )
	{
		U32 aPos = y * GetFrameWidth()/32 + x/32;
		*(mpVScreen + aPos ) = (*(mpVScreen + aPos ) &  ~(0x80000000>>(x%32)*1) )
            | ( (color)<< ((32-1-(x%32))*1) );

//		frameBuffer1[(y)][(x)/32]=( frameBuffer1[(y)][(x)/32] & ~(0x80000000>>((x)%32)*1) )
//            | ( (color)<< ((32-1-((x)%32))*1) );
	}
}

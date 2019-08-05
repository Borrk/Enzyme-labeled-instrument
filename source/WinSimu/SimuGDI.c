// Draw routines in Window platform
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "enmark.h"
#include "SimuGDI.h"

static  HDC gpDC = 0;
static  CFont gFont8x16, gFont4x6;


static void CreateFont( CFont& fnt, INT32U height, INT32U lWeight, INT8U charset, CString fntName )
{
	LOGFONT logFont;
	memset(&logFont, 0, sizeof(LOGFONT));
	
	_tcscpy( logFont.lfFaceName, fntName.GetBuffer( fntName.GetLength()) );
	logFont.lfCharSet	= charset;
	logFont.lfHeight	= height;
	logFont.lfQuality   = 1;
	//logFont.lfUnderline = 60;
	logFont.lfWeight = lWeight;
	fnt.CreateFontIndirect(&logFont);
}

void SimuSetDC( HDC hdc )
{
	gpDC = hdc;
}

void SimuCreateFont()
{
	CreateFont( gFont8x16, 16, 0, DEFAULT_CHARSET, _T("ËÎÌו")  );
	CreateFont( gFont4x6, 24, 0, DEFAULT_CHARSET, _T("ËÎÌו")  );
}

void ClearScreen( INT8U clr  )
{
	// do nothing in VC++
}

void PutPixel( int x, int y, long clr )
{
	::SetPixel( gpDC, x, y,  clr );
}

void PutStrThin( int x, int y, char * str,  char bSel )
{
	::SelectObject( gpDC, &gFont4x6 );
	if ( bSel )
	{
#if 1
		// Derive a region from that path 
		SelectClipPath(gpDC, RGN_AND); 
		// This generates the same result as SelectClipPath() 
		SelectClipRgn(gpDC, PathToRegion(gpDC)); 
		// Fill the region with grayness 
		RECT r; 
		r.left = x-3; r.top=y;
		r.right = x + strlen(str)*8;
		r.bottom = y+20;
		::FillRect(gpDC, &r, (HBRUSH)GetStockObject(BLACK_BRUSH)); 
#endif
		SetBkMode(gpDC, TRANSPARENT); 
		// Bracket begin a path 
		//BeginPath(gpDC); 
		// Send some text out into the world 
		COLORREF  aOldClr = ::SetTextColor( gpDC, RGB(255,255,255) );
		::TextOut(gpDC, x,y, str, strlen(str) ); 
		// Bracket end a path 
		//EndPath(gpDC); 
		::SetTextColor( gpDC, aOldClr);
	}
	else
	{
		::TextOut( gpDC, x, y+3,  str, strlen(str) );
#if 0
		if ( pos != (unsigned short)-1 )
		{
			char aBuf[2] = { 0 };
			aBuf[0] = str[pos];
			COLORREF  aOldClr = ::SetTextColor( gpDC, RGB(255,0,0) );
			::TextOut(gpDC, x + pos*8, y+3, aBuf, 1 ); 
			::SetTextColor( gpDC, aOldClr);
		}
#endif
	}
	
}

void PutStr( int x, int y, char * str,  char bSel )
{
	if ( !str )
		return;
	
	::SelectObject( gpDC, &gFont8x16 );
	if ( bSel )
	{
#if 1
		// Derive a region from that path 
		SelectClipPath(gpDC, RGN_AND); 
		// This generates the same result as SelectClipPath() 
		SelectClipRgn(gpDC, PathToRegion(gpDC)); 
		// Fill the region with grayness 
		RECT r; 
		r.left = x-3; r.top=y;
		r.right = x + strlen(str)*8;
		r.bottom = y+20;
		::FillRect(gpDC, &r, (HBRUSH)GetStockObject(BLACK_BRUSH)); 
#endif
		SetBkMode(gpDC, TRANSPARENT); 
		// Bracket begin a path 
		//BeginPath(gpDC); 
		// Send some text out into the world 
		COLORREF  aOldClr = ::SetTextColor( gpDC, RGB(255,255,255) );
		::TextOut(gpDC, x,y, str, strlen(str) ); 
		// Bracket end a path 
		//EndPath(gpDC); 
		::SetTextColor( gpDC, aOldClr);
	}
	else
	{
		::TextOut( gpDC, x, y+3,  str, strlen(str) );
#if 0
		if ( pos != (unsigned short)-1 )
		{
			char aBuf[2] = { 0 };
			aBuf[0] = str[pos];
			COLORREF  aOldClr = ::SetTextColor( gpDC, RGB(255,0,0) );
			::TextOut(gpDC, x + pos*8, y+3, aBuf, 1 ); 
			::SetTextColor( gpDC, aOldClr);
		}
#endif
	}
	
}

void PutBox( int x, int y, int w, int h )
{
#if 1
RECT r; 
r.left = x;
r.top = y;
r.right = x+w;
r.bottom = y+h;
::FillRect(gpDC, &r, (HBRUSH)GetStockObject(WHITE_BRUSH));
#endif
}

void PutRect( int x, int y, int w, int h )
{
	::Rectangle( gpDC, x, y, x+w, y+h );
}

void PutLine( int x, int y, int x1, int y1 )
{
	::MoveToEx( gpDC, x, y ,0);
	::LineTo( gpDC, x1, y1 );
}

void PutRectV( int x, int y, int w, int h, char bTrans )
{
	::Rectangle( gpDC, x, y, x+w, y+h );	
}

void PutLineV( int x, int y, int x1, int y1 )
{
	::MoveToEx( gpDC, x, y ,0);
	::LineTo( gpDC, x1, y1 );	
}

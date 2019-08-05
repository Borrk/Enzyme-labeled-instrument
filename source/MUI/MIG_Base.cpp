///

#include "Mig_OS_Base.hpp"
#include "Mig_Base.hpp"
#include "MStrMngr.h"


MCHAR*		MLoadStr( INT32U strId )
{
	static MStrMngr& aStrMngr = MStrMngr::Instance();

	return aStrMngr.GetString( strId );
}

#if 0
#ifdef  ENM_VER_TARGET

void ClearScreen( INT8U clr  )
{
	Glib_ClearScr( clr );
}

void PutStr( int x, int y, char * str , char bSel, unsigned short pos )
{
	if ( bSel )
	{
		PutCh16x16Str( x, y, str, 0, CWHITE, CBLACK );
	}
	else
	{
		PutCh16x16Str( x, y, str, 0, CBLACK, CWHITE );
	}
}

void PutStrThin( int x, int y, char * str , char bSel, unsigned short pos )
{
	if ( bSel )
	{
		dis6x12( x, y, str, 0, CWHITE, CBLACK );
	}
	else
	{
		dis6x12( x, y, str, 0, CBLACK, CWHITE );
	}
}

void PutRect( int x, int y, int w, int h )
{
	Glib_Rectangle( x, y, w, h, CBLACK );
}

void PutLine( int x, int y, int x1, int y1 )
{
	Glib_Line( x, y, x1, y1, CBLACK );
}

void PutBox( int x, int y, int w, int h )
{
	Glib_FilledRectangle( x, y, w, h, CWHITE );
}

void PutRectV( int x, int y, int w, int h, char bTrans )
{
	int aClr = bTrans ?CWHITE : CBLACK;
	Glib_Rectangle( x, y, w, h, aClr );	
}

void PutLineV( int x, int y, int x1, int y1 )
{
	Glib_Line( x, y, x1, y1, CBLACK );	
}

#endif
#endif

INT16U	CalRangeInt( INT32S iMaxVal, INT32S iMinVal )
{
	INT16U aMax = 0;
	INT32U aVal = max( abs(iMaxVal), abs(iMinVal) );
	while ( aVal )
	{
		aMax++;
		aVal = (INT32U)(aVal/10.0);
	}

	if ( iMaxVal < 0 || iMinVal < 0 )
		aMax++;
	
	return aMax;
}

INT16U	CalRangeFP(FP64 fMaxVal, FP64 fMinVal )
{
	INT16U aMax = 1;
	INT32U aVal = (INT32U)max( FABS(fMaxVal), FABS(fMinVal) );
	while ( aVal)
	{
		aMax++;
		aVal = (INT32U)(aVal/10.0);
	}

	if ( fMaxVal < 0 || fMinVal < 0 )
		aMax++;

	return aMax;
}

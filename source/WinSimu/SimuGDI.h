#ifndef __SimuGDI_H_
#define __SimuGDI_H_

#include "AfxWin.h"


void SimuSetDC( HDC hdc );
void SimuCreateFont();

void ClearScreen( INT8U clr );

void PutStr( int x, int y, char * str , char bSel=0, unsigned short pos = -1 );
void PutRect( int x, int y, int w, int h );
void PutLine( int x, int y, int x1, int y1 );
void PutBox( int x, int y, int w, int h );
void PutLineV( int x, int y, int x1, int y1 );
void PutRectV( int x, int y, int w, int h, char bTrans );


#endif  ///< __SimuGDI_H_

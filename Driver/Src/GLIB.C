#include "include.h"
#include "Glib.h"

/// for display control
static unsigned char nShowOn = 1;
#define  LCD_SHOW_CHECK() \
if ( 0 == nShowOn )\
	return;
	
unsigned char DisplayTrigOn()
{
	nShowOn = 1;
}

unsigned char DisplayTrigOff()
{
	nShowOn = 0;
}
	
void (*PutPixel)(U32 x,U32 y,U8 c);
//void (*ClrScreen)(U8 c);
void Glib_Init(int depth)
{
    switch(depth)
    {
    case 1:
    	PutPixel=_PutPixelMono;
    	break;
    case 4:
       	PutPixel=_PutPixelG4;
       	break;
    case 16:
        PutPixel=_PutPixelG16;
       // ClrScreen = _ClrScrG16;
        break;
    case 256:
    	PutPixel=_PutPixelColor;
    	break;   
    default: break;
    }
}

void _PutPixelMono(U32 x,U32 y,U8 c)
{
	LCD_SHOW_CHECK();
	
    if(x<SCR_XSIZE && y<SCR_YSIZE)
	frameBuffer1[(y)][(x)/32]=( frameBuffer1[(y)][(x)/32] & ~(0x80000000>>((x)%32)*1) )
            | ( (c)<< ((32-1-((x)%32))*1) );
}


void _PutPixelG4(U32 x,U32 y,U8 c)
{
	LCD_SHOW_CHECK();
	
    if(x<SCR_XSIZE && y<SCR_YSIZE)
        frameBuffer4[(y)][(x)/16]=( frameBuffer4[(y)][x/16] & ~(0xc0000000>>((x)%16)*2) )
            | ( (c)<<((16-1-((x)%16))*2) );
}


void _PutPixelG16(U32 x,U32 y,U8 c)
{

	U32 x0, y0;		//for the 240-320LCD program, exchange x and y position.

	LCD_SHOW_CHECK();

	x0 = x; //LCD_XSIZE - x;
	y0 = y; //LCD_YSIZE - y;
//    if(x<SCR_XSIZE && y<SCR_YSIZE)
//       frameBuffer16[(y)][(x)/8]=( frameBuffer16[(y)][x/8] & ~(0xf0000000>>((x)%8)*4) )
//            | ( (c)<<((8-1-((x)%8))*4) );
   if(x0<SCR_XSIZE && y0<SCR_YSIZE)
        frameBuffer16[(y0)][(x0)/8]=( frameBuffer16[(y0)][x0/8] & ~(0xf0000000>>((x0)%8)*4) )
            | ( (c)<<((8-1-((x0)%8))*4) );
}

void _ClrScrG16( U8 c )
{
	int i, j;

	LCD_SHOW_CHECK();

	for ( i = 0; i < SCR_YSIZE/2; i++ )
	{
		for ( j = 0; j< SCR_XSIZE/16; j++ )
		{
    		frameBuffer16[i][j] = 0xffffffff;
        }                
    }
}

void _PutPixelColor(U32 x,U32 y,U8 c)
{
	LCD_SHOW_CHECK();

    if(x<SCR_XSIZE && y<SCR_YSIZE)
        frameBuffer256[(y)][(x)/4]=( frameBuffer256[(y)][x/4] & ~(0xff000000>>((x)%4)*8) )
            | ( (c)<<((4-1-((x)%4))*8) );
}


void Glib_Rectangle(int x1,int y1,int x2,int y2,int color)
{
  #if 1
  	int xr = x1 + x2, yb = y1 + y2;
    Glib_Line(x1,y1, xr,y1,color);
    Glib_Line(xr,y1,xr,yb,color);
    Glib_Line(x1,yb,xr,yb,color);
    Glib_Line(x1,y1,x1,yb,color);
  #else  
    Glib_Line(x1,y1,x2,y1,color);
    Glib_Line(x2,y1,x2,y2,color);
    Glib_Line(x1,y2,x2,y2,color);
    Glib_Line(x1,y1,x1,y2,color);
  #endif  
}


void Glib_FilledRectangle(int x1,int y1,int x2,int y2,int color)
{
	int i, j;
#if 1
	int xr = x1 + x2, yb = y1 + y2;

    for(i = y1; i <= yb; i++)
    {
    	for( j = x1; j <= xr; j++ )
	    {
	    	PutPixel(j, i, color );
    	}
	}
	return;
	
	for ( i = y1; i <= yb; i++ )
	{
		Glib_Line( x1, i, xr, i, color );
	}
#else  
	
	for(i=y1;i<=y2;i++)
	Glib_Line(x1,i,x2,i,color);
#endif
}

void Glib_DrawSymDot(int xs, int ys, int x,int y,int color)
{
	PutPixel(xs + x, ys + y,color);
	PutPixel(xs - x, ys + y,color);
	PutPixel(xs + x, ys - y,color);
	PutPixel(xs - x, ys - y,color);
	PutPixel(xs + y, ys + x,color);
	PutPixel(xs - y, ys + x,color);
	PutPixel(xs + y, ys - x,color);
	PutPixel(xs - y, ys - x,color);
}
void Glib_DrawHLine(int x0, int y0,  int len,int color )
{
     U8 temp;
	 for(temp = 0;temp <= len;temp++)
	 {
        PutPixel(x0+temp, y0, color);
	 }
}
void Glib_DrawSymLine(int xs, int ys, int x, int y,int color)
{
	Glib_DrawHLine(xs-x,ys+y,x+x,color);
	Glib_DrawHLine(xs-x,ys-y,x+x,color);
	Glib_DrawHLine(xs-y,ys+x,y+y,color);
	Glib_DrawHLine(xs-y,ys-x,y+y,color);
}

void Glib_DrawCircle(int xs, int ys, int r,int color) 
{
    int	x =	0;
	int	y =	r;
	int	p =	3 -	2 *	r;
	while (x < y)
	{
		Glib_DrawSymDot(xs, ys, x,y,color);
		if (p <	0)
		{
			p += 4 * (x++) + 6;
		}
		else
		{
			p += 4 * ((x++)	- (y--)) + 10;
		}
	}
	if (x == y)
		Glib_DrawSymDot(xs, ys, x,y,color);
}

void Glib_FillCircle(int xs, int ys, int r,int color )
{
    int	x =	0;
	int	y =	r;
	int	p =	3 -	2 *	r;
	while (x < y)
	{
		Glib_DrawSymLine(xs, ys, x,y,color);
		if (p <0)
		{
			p += 4 * (x++) + 6;
		}
		else
		{
			p += 4 * ((x++)	- (y--)) + 10;
		}
	}
	if (x == y)
		Glib_DrawSymLine(xs, ys, x,y,color);
}

void Glib_Line(int x1,int y1,int x2,int y2,int color)
{
	int dx,dy,e;
    dx=x2-x1; 
	dy=y2-y1;
    
	if(dx>=0)
	{
		if(dy >= 0) // dy>=0
		{
			if(dx>=dy) // 1/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					PutPixel(x1,y1,color);
					if(e>0){y1+=1;e-=dx;}	
					x1+=1;
					e+=dy;
				}
			}
			else		// 2/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					PutPixel(x1,y1,color);
					if(e>0){x1+=1;e-=dy;}	
					y1+=1;
					e+=dx;
				}
			}
		}
		else		   // dy<0
		{
			dy=-dy;   // dy=abs(dy)

			if(dx>=dy) // 8/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					PutPixel(x1,y1,color);
					if(e>0){y1-=1;e-=dx;}	
					x1+=1;
					e+=dy;
				}
			}
			else		// 7/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					PutPixel(x1,y1,color);
					if(e>0){x1+=1;e-=dy;}	
					y1-=1;
					e+=dx;
				}
			}
		}	
	}
	else //dx<0
	{
		dx=-dx;		//dx=abs(dx)
		if(dy >= 0) // dy>=0
		{
			if(dx>=dy) // 4/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					PutPixel(x1,y1,color);
					if(e>0){y1+=1;e-=dx;}	
					x1-=1;
					e+=dy;
				}
			}
			else		// 3/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					PutPixel(x1,y1,color);
					if(e>0){x1-=1;e-=dy;}	
					y1+=1;
					e+=dx;
				}
			}
		}
		else		   // dy<0
		{
			dy=-dy;   // dy=abs(dy)

			if(dx>=dy) // 5/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					PutPixel(x1,y1,color);
					if(e>0){y1-=1;e-=dx;}	
					x1-=1;
					e+=dy;
				}
			}
			else		// 6/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					PutPixel(x1,y1,color);
					if(e>0){x1-=1;e-=dy;}	
					y1-=1;
					e+=dx;
				}
			}
		}	
	}
}
	

extern unsigned int (*frameBuffer16)[SCR_XSIZE/8];

void Glib_ClearScr(U8 c)
{	
 #if 1
 
  _ClrScrG16( c );

 #else
    int i,j;
	
    for(j=0;j<SCR_YSIZE ;j++)
    	for(i=0;i<SCR_XSIZE ;i++)
	    	PutPixel(i,j,c);
#endif
	    	
}



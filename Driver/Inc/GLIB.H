
#ifndef __ENM_GLIB_H__
#define __ENM_GLIB_H__

#ifdef __cplusplus
extern "C" 
{
#endif

void Glib_Init(int depth);
void Glib_ClearScr(unsigned char  c);


#ifdef  ENM_VER_TARGET
#define CBLACK    0x0
#define CWHITE    0xf

void Glib_Line(int x1,int y1,int x2,int y2,int color);
void Glib_Rectangle(int x1,int y1,int x2,int y2,int color);
void Glib_FilledRectangle(int x1,int y1,int x2,int y2,int color);

void _PutPixelMono(unsigned int  x,unsigned int y,unsigned char c);
void _PutPixelG4(unsigned int x,unsigned int y,unsigned char c);
void _PutPixelG16(unsigned int x,unsigned int y,unsigned char c);
void _PutPixelColor(unsigned int x,unsigned int y,unsigned char c);
void Glib_DrawSymDot(int xs, int ys, int x,int y,int color);
void Glib_DrawSymLine(int xs, int ys, int x, int y,int color);
void Glib_DrawCircle(int xs, int ys, int r,int color) ;
void Glib_FillCircle(int xs, int ys, int r,int color );


unsigned char DisplayTrigOn();
unsigned char DisplayTrigOff();

#else

#define CBLACK    0x0f
#define CWHITE    0x00

#endif


#ifdef __cplusplus
}
#endif

#endif

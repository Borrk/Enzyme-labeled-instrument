#ifndef TEXT_H
#define TEXT_H

#ifdef __cplusplus
extern "C" {
#endif

/*input:
     x,y         :   (x=0-239   y=0-319  左上角=(0,0) )
     transparence:   1=透明   0=不透明
     forecolor   :   前景色
     bkcolor     :   背景色
*/

void dis6x12(short x,short y,char *s, unsigned char transparence,unsigned char forecolor,unsigned char bkcolor);
void dis8x16chart(short x,short y,char c, unsigned char transparence,unsigned char forecolor,unsigned char bkcolor);
void PutCh16x16Str(short x,short y,char *s,unsigned char transparence,unsigned char forecolor,unsigned char bkcolor);
void PutCh24x24Str(short x,short y,char *s,unsigned char transparence,unsigned char forecolor,unsigned char bkcolor);

void PutNum16x16(short x,short y,float Num,unsigned char transparence,unsigned char forecolor,unsigned char bkcolor);
float GetNum16x16(char * Source_Str);

#ifdef __cplusplus
}
#endif

#endif

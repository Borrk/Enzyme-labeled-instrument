#ifndef __MIG_BASE_H__
#define __MIG_BASE_H__


#include "Mig_OS_Base.hpp"
#include "EnmApi.h"

#ifdef  ENM_VER_TARGET
#include "include.h"
#endif

#define		ENK_EDIT_REPLACE	1

#define		MIG_STR_ID_INVALID	(INT32U)-1

enum	enMigLangue
{
	M_CHINESE,
	M_ENGLISH,
};

#define		GetFontW()		12
#define		GetFontH()		8
#define     GetFrameW()     320
#define     GetFrameH()     240


#ifndef  ENM_VER_TARGET

#define KEY_0_N 		'0'
#define KEY_1_A 		'1'
#define KEY_2_B 		'2'
#define KEY_3_C 		'3'
#define KEY_4_D 		'4'
#define KEY_5_E 		'5'
#define KEY_6_F 		'6'
#define KEY_7_G 		'7'
#define KEY_8_H 		'8'
#define KEY_9_P 		'9'
#define KEY_Up 			'U'	
#define KEY_Down		'D'
#define KEY_Right		'R'
#define KEY_Left		'L'
#define KEY_OK			'O'
#define KEY_ESC			'Q'
#define	KEY_TEST    	'T'
#define KEY_GO			'I'
#define KEY_PRG			'W'
#define	KEY_Par			'p'
#define KEY_Data		's'
#define KEY_Prn			'P'
#define KEY_FUN1		'a'
#define KEY_FUN2		'b'
#define KEY_FUN3		'c'
#define KEY_FUN4		'd'
#define KEY_Sign		'+'
#define KEY_DOT_L		'.'



#define	KEY_0		    '0'         
#define	KEY_1		    '1'        
#define	KEY_2		    '2'        
#define	KEY_3		    '3'        
#define	KEY_4		    '4'        
#define	KEY_5		    '5'        
#define	KEY_6		    '6'        
#define	KEY_7		    '7'        
#define	KEY_8		    '8'        
#define	KEY_9		    '9'        
#define	KEY_A		    'A'        
#define	KEY_B		    'B'        
#define	KEY_C		    'C'        
#define	KEY_D		    'D'        
#define	KEY_E		    'E'        
#define	KEY_F		    'F'        
#define	KEY_G		    'G'        
#define	KEY_H		    'H'        
#define	KEY_I		    'I'        
#define	KEY_J		    'J'        
#define	KEY_K		    'K'        
#define	KEY_L		    'L'        
#define	KEY_M		    'M'        
#define	KEY_N		    'N'        
#define	KEY_O		    'O'        
#define	KEY_P		    'P'        
#define	KEY_Q			'Q'        
#define	KEY_R			'R'        
#define	KEY_S		 	'S'        
#define	KEY_T		 	'T'        
#define	KEY_U		    'U'        
#define	KEY_V		    'V'        
#define	KEY_W		    'W'        
#define	KEY_X		    'X'        
#define	KEY_Y		    'Y'        
#define	KEY_Z			'Z'        
#define	KEY_ADD				'+'   
#define	KEY_SUB				'-'      
#define	KEY_MUL				'*' 
#define	KEY_DIV				'/'
#define	KEY_EQUAL			'=' 
#define	KEY_L_BRACKET		'('	
#define	KEY_R_BRACKET		')'
#define KEY_SPACE			' '

#define	KEY_a		    'a'        
#define	KEY_b		    'b'        
#define	KEY_c		    'c'        
#define	KEY_d		    'd'        
#define	KEY_e		    'e'        
#define	KEY_f		    'f'        
#define	KEY_g		    'g'        
#define	KEY_h		    'h'        
#define	KEY_i		    'i'        
#define	KEY_j		    'j'        
#define	KEY_k		    'k'        
#define	KEY_l		    'l'        
#define	KEY_m		    'm'        
#define	KEY_n		    'n'        
#define	KEY_o		    'o'        
#define	KEY_p		    'p'        
#define	KEY_q				'q'        
#define	KEY_r				'r'        
#define	KEY_s		 		's'        
#define	KEY_t		 		't'        
#define	KEY_u		    'u'        
#define	KEY_v		    'v'        
#define	KEY_w		    'w'        
#define	KEY_x		    'x'        
#define	KEY_y		    'y'        
#define	KEY_z				'z'        

#endif
////


typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned short INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   short INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned long  INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   long  INT32S;                   /* Signed   32 bit quantity                           */
typedef float          FP32;                     /* Single precision floating point                    */
typedef double         FP64;                     /* Double precision floating point  */
  
typedef	char			MCHAR;

typedef unsigned char  BOOLEAN;

#ifndef  ENM_VER_TARGET
#define		TRUE		(1)
#define		FALSE		(0)
#endif

#undef	NULL
#define		NULL		(0)

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define	FABS( val ) ((val) < 0 ? -(val) : (val))

/// Macro define
#define		ENM_TITLE_MAX	(64)

typedef void*	MigHandle;

typedef struct __CS
{
	INT32U	type;
	INT32S	x, y, w, h;
	MCHAR	pszTitle[ENM_TITLE_MAX];
	INT32U	dwStyle;
	MigHandle hParent;
	INT32U	id;
	INT32U  titleID;

	__CS(){ titleID = MIG_STR_ID_INVALID;}
	__CS( INT32U uType, INT32S iX, INT32S iY, INT32S iW, INT32S iH, INT32U uStyle, INT32U uID, MCHAR* pStr, INT32U tid = MIG_STR_ID_INVALID )
		: type(uType), x(iX), y(iY), w(iW), h(iH), dwStyle(uStyle), id(uID), titleID(tid)
	{ if (titleID == MIG_STR_ID_INVALID && pStr) strcpy( pszTitle, pStr); }

}tagCS;

#define		ENM_CTRL_LABEL		1
#define		ENM_CTRL_EDITBOX	2
#define		ENM_CTRL_TIMEBOX	3
#define		ENM_CTRL_DATEBOX	4
#define		ENM_CTRL_UPDOWNBOX	5
#define		ENM_CTRL_GRIDBOX	6
#define		ENM_CTRL_RADIOBOX	7


#define		ENM_EB_TYPE_CHAR	0x11
#define		ENM_EB_TYPE_INT		0x12
#define		ENM_EB_TYPE_FLOAT	0x13

//////////////////////////////////////////////////////////////////////////
/// 
#define		ENM_BLACK_TIME			25
#define		ENM_WHITE_TIME			(ENM_BLACK_TIME * 2)

#if 1
typedef struct __EditBoxCS
{
	INT16U	type;
	INT16U	titleWidth;

	INT16U	maxChar;
	union
	{
		INT32S  iVal;
		FP64	fVal;
		MCHAR	szVal[ENM_TITLE_MAX];
	};

	union
	{
	
		INT32S  maxVal;
		FP64	fmaxVal;	
	};
	
	union
	{
		INT32S	minVal;
		FP64	fminVal;
	};

	MCHAR	pszTitle[ENM_TITLE_MAX];
	INT32U  titleId;

	__EditBoxCS(): titleId(MIG_STR_ID_INVALID), maxChar(0)
	{
	}
	
}tagEditBoxCS;
#else
typedef struct __EditBoxCS
{
	INT16U	type;
	INT16U	titleWidth;

	union
	{
		INT32S  iVal;
		FP64	fVal;
		MCHAR	szVal[ENM_TITLE_MAX];
	};

	union
	{
		INT16U	maxChar;
		struct 
		{
		INT32S	minVal;
		INT32S  maxVal;
		};
		struct 
		{
		FP64	fminVal;
		FP64	fmaxVal;
		};
	};

	MCHAR	pszTitle[ENM_TITLE_MAX];
	INT32U  titleId;
}tagEditBoxCS;
#endif

typedef struct
{
	INT16U	lineMax;
	INT16U	lineDisp;

	INT16U	columnMax;
	INT16U	columnDisp;

	INT16U  itemHeight;
	INT16U  titleHeight;
	INT16U	*pItemType;
	INT16U	*pColumnWidth;
	MCHAR	**pszItemTitle;

}tagGridBoxCS;

/// Object Style define
#define		ENM_OBJ_FRAME		0x00000001
#define		ENM_OBJ_TITLE		0x00000002
#define		ENM_OBJ_HIDEN		0x00000004

#define		ENM_PAGE_CLOSED		0x00000100
#define		ENM_PAGE_RESIZE		0x00000200
#define		ENM_PAGE_MINI		0x00000300
#define		ENM_PAGE_TOPMOST	0x00000400
#define		ENM_PAGE_BOTTOM		0x00000800

#define		ENM_CTRL_FOCUSED	0x01000000
#define		ENM_CTRL_STATIC		0x02000000
#define		ENM_CTRL_DISABLE	0x04000000
#define		ENM_CTRL_THINK		0x08000000
#define		ENM_CTRL_IN_EDIT	0x10000000


#define		ENM_OBJ_NORMAL		ENM_OBJ_TITLE	//(ENM_OBJ_FRAME  | ENM_OBJ_TITLE )
#define		ENM_PAGE_NORMAL		(ENM_OBJ_NORMAL)
#define		ENM_CTRL_NORMAL		(ENM_OBJ_TITLE | ENM_CTRL_THINK )


typedef struct __MigMSG
{
	INT32U	msg;
	INT32U	param1;
	INT32U	param2;

	__MigMSG(): msg(0), param1(0), param2(0){}
	__MigMSG( INT32U uMsg, INT32U p1 = 0, INT32U p2 = 0 )
		: msg( uMsg), param1(p1), param2(p2){}

}tagMigMSG;

/// System Message define
enum  enMigSystemMSG
{
	ENM_MSG_SYS_START	= 100,
	ENM_MSG_KEY			=	ENM_MSG_SYS_START,
	ENM_MSG_TIMER,
	ENM_MSG_PAINT,
	ENM_MSG_CFG_CHG,
	
	// ADD Later
	ENM_MSG_SCAN,
	ENM_MSG_PRINT,
	ENM_MSG_ADJUST_PLATE,

	ENM_MSG_MODE_CHANGE,
	ENM_MSG_SCAN_OK,
	ENM_MSG_SYS_END, 
	
	// User MSG start
	ENM_MSG_USER   = ENM_MSG_SYS_END + 1

};

typedef struct 
{
	INT32S	x;
	INT32S	y;
}tagPoint;

typedef struct 
{
	INT32S	left;
	INT32S	top;
	INT32S	bottom;
	INT32S	right;	
}tagRect;


//////////////////////////////////////////////////////////////////////
///
///
//////////////////////////////////////////////////////////////////////

#define		WHITE	0x0f

MCHAR*		MLoadStr( INT32U strId );


#if 0
extern void ClearScreen( INT8U clr );
//extern void PutPixel( int x, int y, long clr );
extern void PutStr( int x, int y, char * str , char bSel=0, unsigned short pos = -1 );
extern void PutStrThin( int x, int y, char * str , char bSel=0, unsigned short pos = -1 ); ///< little font size
extern void PutRect( int x, int y, int w, int h );
extern void PutLine( int x, int y, int x1, int y1 );
extern void PutBox( int x, int y, int w, int h );

extern void PutLineV( int x, int y, int x1, int y1 );
extern void PutRectV( int x, int y, int w, int h, char bTrans );
#endif

extern	INT16U	CalRangeInt( INT32S iMaxVal, INT32S iMinVal );
extern  INT16U	CalRangeFP(FP64 fMaxVal, FP64 fMinVal );

#endif	// __MIG_BASE_H__
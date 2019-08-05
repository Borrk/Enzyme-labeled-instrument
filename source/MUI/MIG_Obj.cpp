
#include "MIG_OS_base.hpp"

#include "MIG_Obj.hpp"

MigObj::MigObj( MigObj *pParent )
: mTitleID( MIG_STR_ID_INVALID )
{
	Init();
	mpParent = pParent;
}

MigObj::MigObj( const tagCS& cs )
{
	Init();

	mX = cs.x;
	mY = cs.y;
	mWidth = cs.w;
	mHeight = cs.h; 
	mStyle = cs.dwStyle;

	if ( cs.titleID == MIG_STR_ID_INVALID )
	{
		strcpy( mszTitle, cs.pszTitle );
	}
	else
	{
		mTitleID = cs.titleID;
	}

	mID = cs.id;

	mpParent = (MigObj*)cs.hParent;

};

MigObj::~MigObj()
{
	
}

void	MigObj::Init(void)
{
	mpNext   = (MigObj*)NULL;
	mpPre    = (MigObj*)NULL;
	mpChild  = (MigObj*)NULL;
	mpParent = (MigObj*)NULL;
	
	mChildCnt = 0;

	mX = mY = 0;
	mWidth =  4;
	mHeight = 4;	
	
	mTitleID = MIG_STR_ID_INVALID;
	for ( short i = 0; i < ENM_TITLE_MAX; i++ )
	{
		mszTitle[i] = '\0';
	}
 
	mStyle = ENM_OBJ_NORMAL;

}

void	MigObj::SetNext( MigObj* pNext ) 
{ 
	mpNext = pNext; 
	if ( pNext && pNext->Previous() != this )
	{
		pNext->SetPrevious( this );
	}
}

void	MigObj::SetPrevious( MigObj* pPre ) 
{ 
	mpPre = pPre; 
	if ( pPre && pPre->Next() != this )
	{
		pPre->SetNext( this );
	}
}

void	MigObj::SetParent( MigObj* pParent ) 
{ 
	mpParent = pParent;
 /*	if ( pParent && pParent->Child() != this )
	{
		pParent->SetChild( this );
	}
*/
}

void	MigObj::SetChild( MigObj* pChild ) 
{ 
	MigObj *apChild = mpChild;
	
	if ( NULL == mpChild )
	{
		mpChild = pChild;
	}
	else
	{
		while ( apChild->Next() )
		{
			apChild = apChild->Next();
		}
		apChild->SetNext( pChild );
	}

 	if ( pChild ) 
	{
		mChildCnt++;
		if ( pChild->Parent() != this )
		{
			pChild->SetParent( this );
		}
	}
}

MigObj*	 MigObj::Child(INT32U ChildNo )
{
	INT32U	aCnt = 0;
	MigObj *apChild = NULL;
	if ( 0 < ChildNo && ChildNo <= mChildCnt )
	{
		apChild = mpChild;
		while ( ++aCnt < ChildNo )
		{
			apChild = apChild->Next();
		}
	}

	return apChild;
}

// Title relative routines
void	MigObj::GetTitle( MCHAR* pszDst, INT16U size )
{
	assert( pszDst != NULL );

	INT16U	 aTitleSize = strlen( mszTitle );
	if ( aTitleSize >= size )
	{
		aTitleSize = size - 1;
	}

	strncpy( pszDst, mszTitle, aTitleSize );
	pszDst[aTitleSize] = '\0';
}

void	MigObj::SetTitle( MCHAR* pszTitle, BOOLEAN bErase )
{
	assert( pszTitle != NULL );

	INT16U	 aTitleSize = strlen( pszTitle );
	if ( aTitleSize >= ENM_TITLE_MAX )
	{
		aTitleSize = ENM_TITLE_MAX - 1;
	}

	strncpy( mszTitle, pszTitle, aTitleSize );
	mszTitle[aTitleSize] = '\0';

	mTitleID = MIG_STR_ID_INVALID;

	if ( bErase )
		Update();
}

void	MigObj::SetTitle( INT32U titleID, BOOLEAN bErase )
{
	MCHAR *pszTitle = MLoadStr( titleID );
	if ( pszTitle )
		mTitleID = titleID;
	else
		mTitleID = MIG_STR_ID_INVALID;

	mszTitle[0] = '\0';

	if ( bErase )
		Update();
}

void  MigObj::SetSize( const tagRect& rc )
{
	mX = rc.left;
	mY = rc.top;
	mWidth = rc.right;
	mHeight = rc.bottom;
}

MCHAR MigObj::Key2Char(INT16U key, INT16U cnt )
{
	struct tagKey2Char
	{
		INT16U  key;
		MCHAR   ch;
	};
	
	INT8U aIdx = 0;
	aIdx = cnt%2;

	tagKey2Char aKeyMap[2][80] = 
	{
		{
		{	KEY_0_N,	'0'	},
		{	KEY_1_A,	'1'	},
		{	KEY_2_B,	'2'	},
		{	KEY_3_C,	'3'	},
		{	KEY_4_D,	'4'	},
		{	KEY_5_E,	'5'	},
		{	KEY_6_F,	'6'	},
		{	KEY_7_G,	'7'	},
		{	KEY_8_H,	'8'	},
		{	KEY_9_P,	'9'	},							
		{	KEY_Sign,	'+'	},
		{	KEY_DOT_L,	'.'	},
		{	KEY_MUL,    '*' },
		{	KEY_DIV,    '/' },
		{	KEY_EQUAL,  '=' },
		{	KEY_L_BRACKET,	 '('},
		{	KEY_R_BRACKET,    ')'},
		{	KEY_SPACE,		' ' },        
		{	KEY_0,		    '0' },        
		{	KEY_1,		    '1' },       
		{	KEY_2,		    '2' },       
		{	KEY_3,		    '3' },       
		{	KEY_4,		    '4' },       
		{	KEY_5,		    '5' },       
		{	KEY_6,		    '6' },       
		{	KEY_7,		    '7' },       
		{	KEY_8,		    '8' },       
		{	KEY_9,		    '9' },       
		{	KEY_A,		    'A' },       
		{	KEY_B,		    'B' },       
		{	KEY_C,		    'C' },       
		{	KEY_D,		    'D' },       
		{	KEY_E,		    'E' },       
		{	KEY_F,		    'F' },       
		{	KEY_G,		    'G' },       
		{	KEY_H,		    'H' },       
		{	KEY_I,		    'I' },       
		{	KEY_J,		    'J' },       
		{	KEY_K,		    'K' },       
		{	KEY_L,		    'L' },       
		{	KEY_M,		    'M' },       
		{	KEY_N,		    'N' },       
		{	KEY_O,		    'O' },       
		{	KEY_P,		    'P' },       
		{	KEY_Q,			'Q'   },     
		{	KEY_R,			'R'   },     
		{	KEY_S,		 	'S'   },     
		{	KEY_T,		 	'T'   },     
		{	KEY_U,		    'U' },       
		{	KEY_V,		    'V' },       
		{	KEY_W,		    'W' },       
		{	KEY_X,		    'X' },       
		{	KEY_Y,		    'Y' },       
		{	KEY_Z,			'Z'   },     
		{	KEY_a,		    'a' },       
		{	KEY_b,		    'b' },       
		{	KEY_c,		    'c' },       
		{	KEY_d,		    'd' },       
		{	KEY_e,		    'e' },       
		{	KEY_f,		    'f' },       
		{	KEY_g,		    'g' },       
		{	KEY_h,		    'h' },       
		{	KEY_i,		    'i' },       
		{	KEY_j,		    'j' },       
		{	KEY_k,		    'k' },       
		{	KEY_l,		    'l' },       
		{	KEY_m,		    'm' },       
		{	KEY_n,		    'n' },       
		{	KEY_o,		    'o' },       
		{	KEY_p,		    'p' },       
		{	KEY_q,				'q' },       
		{	KEY_r,				'r' },       
		{	KEY_s,		 		's' },       
		{	KEY_t,		 		't' },       
		{	KEY_u,		    'u' },       
		{	KEY_v,		    'v' },       
		{	KEY_w,		    'w' },       
		{	KEY_x,		    'x' },       
		{	KEY_y,		    'y' },       
		{	KEY_z,				'z' }       
		
		},

		{
		{	KEY_0_N,	'N'	},
		{	KEY_1_A,	'A'	},
		{	KEY_2_B,	'B'	},
		{	KEY_3_C,	'C'	},
		{	KEY_4_D,	'D'	},
		{	KEY_5_E,	'E'	},
		{	KEY_6_F,	'F'	},
		{	KEY_7_G,	'G'	},
		{	KEY_8_H,	'H'	},
		{	KEY_9_P,	'P'	},							
		{	KEY_Sign,	'-'	},
		{	KEY_DOT_L,	'L'	},
		{	KEY_MUL,    '*' },
		{	KEY_DIV,    '/'},
		{	KEY_EQUAL,  '=' },
		{	KEY_L_BRACKET,	 '('},
		{	KEY_R_BRACKET,    ')'},
		{	KEY_SPACE,		' ' },        
		{	KEY_0,		    '0' },        
		{	KEY_1,		    '1' },       
		{	KEY_2,		    '2' },       
		{	KEY_3,		    '3' },       
		{	KEY_4,		    '4' },       
		{	KEY_5,		    '5' },       
		{	KEY_6,		    '6' },       
		{	KEY_7,		    '7' },       
		{	KEY_8,		    '8' },       
		{	KEY_9,		    '9' },       
		{	KEY_A,		    'A' },       
		{	KEY_B,		    'B' },       
		{	KEY_C,		    'C' },       
		{	KEY_D,		    'D' },       
		{	KEY_E,		    'E' },       
		{	KEY_F,		    'F' },       
		{	KEY_G,		    'G' },       
		{	KEY_H,		    'H' },       
		{	KEY_I,		    'I' },       
		{	KEY_J,		    'J' },       
		{	KEY_K,		    'K' },       
		{	KEY_L,		    'L' },       
		{	KEY_M,		    'M' },       
		{	KEY_N,		    'N' },       
		{	KEY_O,		    'O' },       
		{	KEY_P,		    'P' },       
		{	KEY_Q,			'Q'   },     
		{	KEY_R,			'R'   },     
		{	KEY_S,		 	'S'   },     
		{	KEY_T,		 	'T'   },     
		{	KEY_U,		    'U' },       
		{	KEY_V,		    'V' },       
		{	KEY_W,		    'W' },       
		{	KEY_X,		    'X' },       
		{	KEY_Y,		    'Y' },       
		{	KEY_Z,			'Z'   },     
		{	KEY_a,		    'a' },       
		{	KEY_b,		    'b' },       
		{	KEY_c,		    'c' },       
		{	KEY_d,		    'd' },       
		{	KEY_e,		    'e' },       
		{	KEY_f,		    'f' },       
		{	KEY_g,		    'g' },       
		{	KEY_h,		    'h' },       
		{	KEY_i,		    'i' },       
		{	KEY_j,		    'j' },       
		{	KEY_k,		    'k' },       
		{	KEY_l,		    'l' },       
		{	KEY_m,		    'm' },       
		{	KEY_n,		    'n' },       
		{	KEY_o,		    'o' },       
		{	KEY_p,		    'p' },       
		{	KEY_q,				'q' },       
		{	KEY_r,				'r' },       
		{	KEY_s,		 		's' },       
		{	KEY_t,		 		't' },       
		{	KEY_u,		    'u' },       
		{	KEY_v,		    'v' },       
		{	KEY_w,		    'w' },       
		{	KEY_x,		    'x' },       
		{	KEY_y,		    'y' },       
		{	KEY_z,				'z' }       
		}
		
	};

	MCHAR aCh = 0;
	
	MCHAR aMap[6] = { '-', '+', '*', '/', '(', ')' };
	{
	};

	
	for ( INT16U i = 0; i < sizeof(aKeyMap[0]) / sizeof(aKeyMap[0][0]); i++ )
	{
		if ( key == aKeyMap[aIdx][i].key )
		{
			if ( key == KEY_Sign )
				aCh = aMap[cnt%6];
			else
				aCh = (MCHAR)aKeyMap[aIdx][i].ch;
			break;
		}
	}
	
	return aCh;	
}

void	MigObj::OnKey( INT16U key )
{
	switch( GetKeyValue(key) )
	{
	case KEY_0_N:
	case KEY_1_A:
	case KEY_2_B:
	case KEY_3_C:
	case KEY_4_D:
	case KEY_5_E:
	case KEY_6_F:
	case KEY_7_G:
	case KEY_8_H:
	case KEY_9_P:
		OnNumKey( key );
		break;

	case KEY_Up:
	{
		OnKeyUp();
		break;
	}
	case KEY_Down:
	{
		OnKeyDown();
		break;
	}
	case KEY_Right:
	{
		OnKeyRight();
		break;
	}
	case KEY_Left:
	{
		OnKeyLeft();
		break;
	}
	case KEY_OK:
	{
		OnKeyOK();
		break;
	}
	case KEY_ESC:
	{
		OnKeyESC();
		break;
	}
	case KEY_TEST:
	{
		OnKeyTest();
		break;
	}
	case KEY_PRG:
	{
		OnKeyPrg();
		break;
	}
	case KEY_Par:
	{
		OnKeyParam();
		break;
	}
	case KEY_Data:
	{
		OnKeyData();
		break;
	}
	case KEY_GO:
	{
		OnKeyGO();
		break;
	}
	case KEY_Prn:
	{
		OnKeyPrnt();
		break;
	}
	case KEY_Sign:
	{
		OnKeySign();
		break;
	}
	case KEY_DOT_L:
	{
		OnKeyDot();
		break;
	}
	case KEY_FUN1:
	{
		OnKeyFunc1();
		break;
	}
	case KEY_FUN2:
	{
		OnKeyFunc2();
		break;
	}

	case KEY_FUN3:
	{
		OnKeyFunc3();
		break;
	}
	case KEY_FUN4:
	{
		OnKeyFunc4();
		break;
	}
	default:
		break;
	}
}

#include "MIG_OS_Base.hpp"

#include "MIG_Page.hpp"

INT32U   MigPage::mPageCnt    = 0;
MigPage* MigPage::mpCurPage   = (MigPage*)0;
MigPage* MigPage::mpFirstPage = (MigPage*)0;

MigPage::MigPage( MigObj *pParent )
	:MigObj( pParent )
{
	Init();
}

MigPage::MigPage( const tagCS& cs )
  :MigObj( cs )
{
	Init();
};

MigPage::~MigPage()
{
	DeleteAllCtrls();
}

void	MigPage::Init(void)
{
	if ( 0 == mPageCnt )
	{
		mpCurPage = this;
		mpFirstPage = this;
	}
	mPageCnt++;

	mpReturnPage = NULL;

	mpCurCtrl = NULL;
	mpChildCtrlList = NULL;

}

void	MigPage::Draw(void)
{

	if ( mStyle & ENM_OBJ_HIDEN )
		return;

	DrawFrame( );

	if ( mStyle & ENM_OBJ_TITLE )
	{
		DrawTitle( );
	}

	// draw control
	DrawControls();
}


void	MigPage::DrawTitle(void)
{
	MCHAR *ap;
	if ( mTitleID == MIG_STR_ID_INVALID )
		ap = mszTitle;
	else
		ap = MLoadStr(mTitleID);

	PutStr( mX + ((mWidth - strlen(ap)*8)/2), mY, ap, 0 );
}

void	MigPage::DrawFrame(void)
{
	
	// add later
	if ( mStyle & ENM_OBJ_FRAME )
	{
		PutRect( mX, mY, mWidth, mHeight );
	}
	else
	{
		PutRect( mX, mY, mWidth, mHeight );
	//	PutBox( mX, mY, mWidth, mHeight );
	}
}


/// Control relative routines
void  MigPage::AddControl( MigControl* pCtrl )
{
	assert( pCtrl != NULL );

	MigObj *apCtrl = mpChildCtrlList;

	if ( NULL == mpChildCtrlList )
	{
		//mpCurCtrl = pCtrl;
		mpChildCtrlList = pCtrl;
		mpChildCtrlList->SetNext( NULL );
	}
	else
	{
		while ( apCtrl->Next() )
		{
			apCtrl = apCtrl->Next();
		}
		apCtrl->SetNext( pCtrl );
	}

	// set control's parent
	pCtrl->SetParent( this );

	if ( mpCurCtrl == NULL )
		ControlFocusSw( pCtrl );

}

MigControl*	 MigPage::AddControl( const tagCS& cs )
{
	MigControl *apCtrl = new MigControl( cs );
	AddControl( apCtrl );
	
	return apCtrl;
}

void  MigPage::DeleteAllCtrls(void)
{
	MigControl* apCtrl = mpChildCtrlList;
	MigControl* apNextCtrl;

	while ( apCtrl )
	{
		apNextCtrl = (MigControl*)apCtrl->Next();
		delete apCtrl;
		apCtrl = apNextCtrl;
	}
}

void	MigPage::DrawControls(void)
{
	// add later
	MigControl* apCtrl = mpChildCtrlList;
	while ( apCtrl )
	{
		apCtrl->Update();
		apCtrl = (MigControl*)apCtrl->Next();
	}
}

void	MigPage::FocusControl( INT32U id )
{
	MigObj* apCtrl = mpChildCtrlList;

	while ( apCtrl )
	{
		if ( id == apCtrl->GetID() )
			break;
		apCtrl = apCtrl->Next();
	}

	if ( apCtrl )
	{
		mpCurCtrl->FocusOff();
		apCtrl->FocusOn();
		mpCurCtrl = (MigControl*)apCtrl;
	}
}


MigControl* MigPage::FindActiveCtrl(MigControl *pStartCtrl, BOOLEAN bDown )
{
	INT8U aFound = 0;
	MigControl	*apCtrl= pStartCtrl ? pStartCtrl : mpChildCtrlList;
	MigControl	*apLastCtrl= apCtrl;
	
	while ( apCtrl )
	{
		if ( apCtrl->IsActive() )
		{
			aFound = 1;
			break;
		}
		
		apCtrl = (MigControl*) (bDown ? apCtrl->Next() : apCtrl->Previous());
	}

	if ( aFound == 0 )
	{
/// ring switch control
#if 1
		if ( /*pStartCtrl == mpChildCtrlList && */!bDown )
		{
			apCtrl = GetLastControl();
		//	bDown = TRUE;
		}
		else
		{
			apCtrl = mpChildCtrlList;
		}
#else
		apCtrl = mpChildCtrlList;
#endif

		while ( apCtrl && pStartCtrl != apCtrl ) //apCtrl < apLastCtrl )
		{
			if ( apCtrl && apCtrl->IsActive() )
			{
				aFound = 1;
				break;
			}
			
			apCtrl = (MigControl*)(bDown ? apCtrl->Next() : apCtrl->Previous());
		}
	}

	return  (aFound == 1) ? apCtrl : NULL;
}

void	MigPage::ControlFocusSw( MigControl *pCtrl, BOOLEAN bDown )
{
	/// when current focused control in edit mode, cannot switch focus
	if ( mpCurCtrl && mpCurCtrl->IsInEdit() )
	{
		// Hyq, delete whenever control's focus switch
		return;
	}

#if 1

	MigControl	*apCtrl, *apLastCtrl = mpCurCtrl;
	
	if ( mpCurCtrl == NULL )
	{
		apCtrl= mpChildCtrlList;
	}
	else if ( pCtrl == NULL )
	{
#if 1	/// ring switch control
		if ( bDown )
		{
			apCtrl= (MigControl*)mpCurCtrl->Next();
			apCtrl = (MigControl*)(bDown ? mpCurCtrl->Next() : mpCurCtrl->Previous());	
		}
		else
		{
			apCtrl = GetLastControl();
		}
#else
		apCtrl= (MigControl*)mpCurCtrl->Next();
		apCtrl = (MigControl*)(bDown ? mpCurCtrl->Next() : mpCurCtrl->Previous());
#endif
	}
	else
	{
		apCtrl = pCtrl;
	}

	apCtrl = FindActiveCtrl( apCtrl, bDown );

	if ( apCtrl )
	{
	//	if ( apLastCtrl != apCtrl )
		{
			if ( apLastCtrl )
				apLastCtrl->FocusOff();

			if ( apCtrl ) ///< found
			{
				apCtrl->FocusOn();
			}
			mpCurCtrl = apCtrl;
		}
	//	else
	//	{
	//		mpCurCtrl->SetStyle( mpCurCtrl->GetStyle() | ENM_CTRL_FOCUSED );
	//	}
	}

#else
	if ( mpCurCtrl == NULL )
	{
		mpCurCtrl = mpChildCtrlList;
		if ( mpCurCtrl )
		{
			mpCurCtrl->FocusOn();
		}
		return;
	}

	MigControl	*apCtrl= mpCurCtrl;
	if ( pCtrl == NULL )
	{
		if ( mpCurCtrl->Next() )
		{
			apCtrl = (MigControl*)mpCurCtrl->Next();
		}
		else
		{
			apCtrl = mpChildCtrlList;
		}
		
	}
	else if ( mpCurCtrl != pCtrl )
	{
		apCtrl = pCtrl;
	}

	mpCurCtrl->FocusOff();
	apCtrl->FocusOn();
	mpCurCtrl = apCtrl;
#endif

	// temp code
	 Update();
}

MigControl* MigPage::GetControl( INT32U id )
{
	MigObj* apCtrl = mpChildCtrlList;

	while ( apCtrl )
	{
		if ( id == apCtrl->GetID() )
			break;
		apCtrl = apCtrl->Next();
	}

	return  (MigControl*)apCtrl;
}

MigControl* MigPage::GetLastControl( )
{
	MigObj* apCtrl = mpChildCtrlList;

	while ( apCtrl )
	{
		if ( apCtrl->Next() == NULL )
			break;

		apCtrl = apCtrl->Next();
	}

	return  (MigControl*)apCtrl;
}

MigControl* MigPage::GetControlByIndex( INT16U idx )
{
	INT16U i = 1;
	MigObj* apCtrl = mpChildCtrlList;

	while ( apCtrl && i++ < idx )
	{
		apCtrl = apCtrl->Next();
	}

	return  (MigControl*)apCtrl;
}

/// Focus switch
MigPage* MigPage::GetCurFocusPage(void)
{
	return  mpCurPage;
}

void	MigPage::SetCurFocusPage(MigPage* pPage )
{
	if ( pPage == NULL )
		return;

	if ( pPage != mpCurPage )
	{
		mpCurPage->FocusOff();
		//EnmDelay(2000);	///< for draw twice bug
		pPage->FocusOn();
		mpCurPage = pPage;
	}

}

MigPage*	MigPage::Switch2Page( INT32U id, MigPage *pReturnPage )
{
	 MigPage* p = mpFirstPage;
	 p = FindPageByID( p, id );

	 if ( p )
	 {
		 if ( pReturnPage != p )
			p->SetReturnPage( pReturnPage );
		SetCurFocusPage( p );
	 }

	 return p;
}

MigPage* MigPage::FindPageByID( MigPage* p, INT32U id )
{
	static MigPage *aPage = NULL;
	static INT16U	aNestCnt = 0;

	MigPage *ap = aPage;

	aNestCnt++;

	if ( p->GetID() == id )
	{
		aPage = p;
		if ( --aNestCnt == 0 )
		{
			aPage = NULL;
		}
		return p;
	}

	if ( p->Child() )
	{
		FindPageByID( (MigPage*)p->Child(), id );
	}

	/// find in brother page
	if ( aPage == NULL )
	{
		if ( p->Next() )
		{
			FindPageByID( (MigPage*)p->Next(), id );
		}
	}

	ap = aPage;
	if ( --aNestCnt == 0 )
	{
		aPage = NULL;
	}
	return ap;
}

/// Message proc
void	MigPage::OnMessage( const tagMigMSG& stMsg )
{
	INT32U aMsg = stMsg.msg;

	if ( mpCurCtrl )
		mpCurCtrl->OnMessage( stMsg);

	switch ( aMsg )
	{
	case ENM_MSG_KEY:
		OnKey( (INT16U)stMsg.param1 );
		break;
	//case ENM_MSG_TIMER:
	//	break;
	case ENM_MSG_PAINT:
		Update();
		break;

	case ENM_MSG_CFG_CHG:
		SetParam();
		break;


    default:
		OnUserMsg( stMsg );
		break;
	}
}

void	MigPage::OnUserMsg( const tagMigMSG& stMsg )
{
	/// do nothing
}

void	MigPage::BroadcastMsg( tagMigMSG msg )
{
	MigPage *p = this;
	if ( p->Child() == 0 )
	{
		p->OnMessage( msg );
		if ( p->Next() )
		{
			p = (MigPage*)p->Next();
			p->BroadcastMsg( msg );
		}
		else
		{
		//	p->OnMessage( msg );
		}
	}
	else
	{
		p = (MigPage*)p->Child();
		p->BroadcastMsg( msg );

		if ( p->Next() )
		{
			p = (MigPage*)p->Next();
			p->BroadcastMsg( msg );
		}
	}

}

void	MigPage::FocusOn(void)
{
	ControlFocusSw( mpCurCtrl);
	// temp code
	Draw();
}

void	MigPage::FocusOff(void)
{
//	if (mpCurCtrl)
//		mpCurCtrl->FocusOff();
}

INT16U	MigPage::NumKey2Index( INT16U key )
{
	INT16U aKeyMap[10][2] = 
	{
		{	KEY_0_N,	0	},
		{	KEY_1_A,	1	},
		{	KEY_2_B,	2	},
		{	KEY_3_C,	3	},
		{	KEY_4_D,	4	},
		{	KEY_5_E,	5	},
		{	KEY_6_F,	6	},
		{	KEY_7_G,	7	},
		{	KEY_8_H,	8	},
		{	KEY_9_P,	9	}							
	};

	INT16U aKey = (INT16U)-1;
	for ( INT16U i = 0; i < 10; i++ )
	{
		if ( key == aKeyMap[i][0] )
		{
			aKey = aKeyMap[i][1];
			break;
		}
	}
	
	return aKey;	
}

void	MigPage::OnNumKey( INT16U key )
{
	key = GetKeyValue(key);

	INT16U aKey = NumKey2Index( GetKeyValue(key) );	
	if ( aKey == (INT16U)-1 )
		return;

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
		if ( mpCurCtrl && mpCurCtrl->GetType() == ENM_CTRL_LABEL )
			ControlFocusSw( GetControlByIndex( (INT16U)aKey ) );
		
		SetCurFocusPage( (MigPage*)Child( aKey ) );
		break;
	default:
		break;
	}
}

void	MigPage::OnKeyUp( void )
{
	if ( mpCurCtrl ) //&& mpCurCtrl->Previous() )
	{
		ControlFocusSw( (MigControl*)mpCurCtrl->Previous(), FALSE );
	}
}
	
void	MigPage::OnKeyDown( void )
{
	if ( mpCurCtrl ) //&& mpCurCtrl->Next() )
	{
		ControlFocusSw( (MigControl*)mpCurCtrl->Next() );
	}
}

void	MigPage::OnKeyESC( void )
{
	if ( mpReturnPage )
		SetCurFocusPage( mpReturnPage );	
	else
		SetCurFocusPage( (MigPage*)mpParent );	

	mpReturnPage = NULL;
}

void  MigPage::InvalidateRect( tagRect& rc, BOOLEAN bRepaint )
{
	if ( bRepaint )
		ClearScreen( WHITE );
	else
	{
		PutBox( rc.left, rc.top, (rc.right - rc.left), (rc.bottom - rc.top) );
	}
}

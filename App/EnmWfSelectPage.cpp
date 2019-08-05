#include "EnmWfSelectPage.h"
#include "EnmWfoSelecter.h"

CEnmWfSelectPage::CEnmWfSelectPage(MCHAR* pTitle, U32 pid )
: CEnmBasePage( pTitle, pid )
, mpWfoSelector(NULL)
{
}

CEnmWfSelectPage::CEnmWfSelectPage(U32 titleID, U32 pid )
: CEnmBasePage( "", pid )
, mpWfoSelector(NULL)
{
	SetTitle( titleID );
}

BOOLEAN	 CEnmWfSelectPage::CreateItems( U32 *pItems, U32 count )
{
	return CreateControls( pItems, count );
}

void  CEnmWfSelectPage::SetWfoSelector( CEnmWfoSelector& selector )
{
	SetStep( &selector );
}

CEnmWfoSelector* CEnmWfSelectPage::GetWfoSelector( )
{
	return (CEnmWfoSelector*)GetStep();
}

BOOLEAN CEnmWfSelectPage::SelectItem( S32 item )
{
	MigControl *ap = GetControl( item + 1 );
	if ( ap && ap != GetCurCtrl() )
	{
		this->ControlFocusSw( ap );
		return TRUE;
	}
	
	return FALSE;
}


BOOLEAN  CEnmWfSelectPage::CreateControls( U32 *pItems, U32 count )
{	
	if ( !pItems || count <= 0 )
		return TRUE;

	MCHAR  *ap;
	U16 aMaxLength = 0;

	U8 i;
	for ( i = 0; i < count; i++ )
	{
		if ( ap = MLoadStr( pItems[i] ) )
			if ( strlen(ap) > aMaxLength )
				aMaxLength = strlen( ap );
	}

	U16  aX = (GetFrameW() - aMaxLength * GetFontW() ) / 2;
	tagCS cs;
	cs.w = 160;
	cs.h = 20;
	cs.x = aX;
	cs.hParent = this;
	cs.y = 60;
	cs.dwStyle = ENM_OBJ_NORMAL;

	MigControl *apCtrl;
	for ( i = 0; i < count; i++ )
	{
		cs.id = 1 + i;
		cs.titleID = pItems[i];
		apCtrl = AddControl( cs );
		cs.y +=20;
	}

	return TRUE;
}

void CEnmWfSelectPage::OnNumKey( INT16U key )
{
	BOOLEAN bOK = FALSE;

	INT16U aKey = NumKey2Index( GetKeyValue(key) );	
	if ( GetControl( aKey ) && GetWfoSelector() )
	{
		bOK = GetWfoSelector()->SelectStep( aKey - 1 );
	}

	if ( bOK )
		MigPage::OnNumKey( key );
}

void CEnmWfSelectPage::OnKeyUp( )
{
	CEnmBasePage::OnKeyUp( );

	MigControl *ap = GetCurCtrl();
	if ( ap )
	{
		ap->GetID();
		if ( GetWfoSelector() )
			GetWfoSelector()->SelectStep( ap->GetID() - 1 );
	}
}

void CEnmWfSelectPage::OnKeyDown( )
{
	CEnmBasePage::OnKeyDown( );

	MigControl *ap = GetCurCtrl();
	if ( ap )
	{
		ap->GetID();
		if ( GetWfoSelector() )
			GetWfoSelector()->SelectStep( ap->GetID() - 1 );
	}
}

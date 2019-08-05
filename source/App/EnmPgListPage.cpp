#include "EnmPgListPage.h"
#include "EnmModeLib.h"


CEnmPgList::CEnmPgList( MigObj *pParent )
 : MigGrid( pParent )
{ 
	SetList(); 
}

CEnmPgList::CEnmPgList( const tagCS& cs,  const tagGridBoxCS& bcs ) 
 : MigGrid( cs, bcs )
{ 
	SetList(); 
}

void	CEnmPgList::SetList( void )
{
	Layout();
}

void	CEnmPgList::Layout( void)	///< set layout by current line and comumn
{
	INT16U i;
	INT16U	aLines = min( mGCS.lineDisp, mUsedLine - mUpperLine );
	
	for ( i = 0; i < aLines; i++ )
	{
		VisibleLine( i );
	}

//	for ( i = aLines; i < mGCS.lineDisp; i++ )
//	{
//		//ClearOneSample( i );
//		VisibleLine( i, 0 );
//	}
}

void	CEnmPgList::OnKeyOK( void )
{
	// do nothing
	if ( GetFocusedItem()->IsInEdit() )
		GetFocusedItem()->OutEditMode();
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnmPgListPage::CEnmPgListPage()
: CEnmBasePage( "", PID_PG_LIST )
{
	CreateControl();
}

void CEnmPgListPage::CreateControl()
{
	tagCS aCS( ENM_CTRL_GRIDBOX, 30, 30, 280, 20, ENM_CTRL_NORMAL, 1,	"", STR_BLANK );
	aCS.hParent = this;

	INT16U type[] = { ENM_EB_TYPE_CHAR, ENM_EB_TYPE_CHAR, ENM_EB_TYPE_CHAR };
	INT16U width[]= { 32, 70, 120 };

	tagGridBoxCS gcs;
	gcs.columnDisp	= 3;
	gcs.columnMax	= 3;
	gcs.lineDisp	= 8;
	gcs.lineMax		= CEnmModeLib::PROT_MAX;
	gcs.pColumnWidth = width;
	gcs.itemHeight = 20;
	gcs.titleHeight = 20;
	gcs.pItemType = type;
	gcs.pszItemTitle = NULL;

	mpPgList = new CEnmPgList( aCS,  gcs );
	AddControl( mpPgList );

	mpPgList->EnableColumn( 0, FALSE );
	mpPgList->EnableColumn( 1, TRUE );
	mpPgList->EnableColumn( 2, FALSE );
	U8 i, j;
	for ( i = 0; i < gcs.lineDisp; i++ )
	{
		mpPgList->SetItemRangeCHAR ( i, 0, 3 );
		mpPgList->SetItemRangeCHAR( i, 1, 16 );
		mpPgList->SetItemRangeCHAR( i, 2, 16);
	}
}

void  CEnmPgListPage::FocusOn()
{
	SyncProtocol2UI();
	
	CEnmBasePage::FocusOn();
}

void CEnmPgListPage::SyncProtocol2UI()
{	
	/// Clear old protocol list
	mpPgList->DeleteAllLine();
	
	MCHAR aName[32];
	MCHAR aID[8];
	U16 aType[2] = { STR_QUALITITIVE, STR_QUALITATIVE };

	tagProtocol aProt;
	CEnmModeLib& rMl = CEnmModeLib::Instance();
	for ( U16 i = 0; i < rMl.GetCount(); i++ )
	{
		if ( !rMl.GetAt( i, aProt ) )
			continue;
		
		mpPgList->AddLine();

		sprintf( aName, aProt.stMeasure.szName );
		sprintf( aID, "%d", i+1 );		
		mpPgList->SetItemVal( i, 0, aID );
		mpPgList->SetItemVal( i, 1, aName );
		mpPgList->SetItemVal( i, 2, MStrMngr::Instance().GetString( aType[aProt.stMeasure.uMeasMode]) );
	}
}

BOOLEAN  CEnmPgListPage::OnForward()
{	
	return inherited::OnForward();
}

BOOLEAN  CEnmPgListPage::OnExit()
{	
	return inherited::OnExit();
}

// EnMarkView.cpp : implementation of the CEnMarkView class
//

#include "stdafx.h"
#include "EnMark.h"

#include "EnMarkDoc.h"
#include "EnMarkView.h"
#include "SimuScanEngine.h"
#include "SimuGDI.h"
#include "EnmModeLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CEnMarkView

IMPLEMENT_DYNCREATE(CEnMarkView, CView)

BEGIN_MESSAGE_MAP(CEnMarkView, CView)
	//{{AFX_MSG_MAP(CEnMarkView)
	ON_COMMAND(ID_ENK_1, OnEnk1)
	ON_COMMAND(ID_ENK_2, OnEnk2)
	ON_COMMAND(ID_ENK_3, OnEnk3)
	ON_COMMAND(ID_ENK_4, OnEnk4)
	ON_COMMAND(ID_ENK_5, OnEnk5)
	ON_COMMAND(ID_ENK_6, OnEnk6)
	ON_COMMAND(ID_ENK_7, OnEnk7)
	ON_COMMAND(ID_ENK_8, OnEnk8)
	ON_COMMAND(ID_ENK_0, OnEnk0)
	ON_COMMAND(ID_ENK_9, OnEnk9)
	ON_COMMAND(ID_UP, OnUp)
	ON_COMMAND(ID_LEFT, OnLeft)
	ON_COMMAND(ID_RIGHT, OnRight)
	ON_COMMAND(ID_DOWN, OnDown)
	ON_COMMAND(ID_ENK_ADD, OnEnkAdd)
	ON_COMMAND(ID_ENK_OK, OnEnkOk)
	ON_COMMAND(ID_ENK_CHECK, OnEnkCheck)
	ON_COMMAND(ID_ENK_DATA, OnEnkData)
	ON_COMMAND(ID_ENK_EXIT, OnEnkExit)
	ON_COMMAND(ID_ENK_PROGRAM, OnEnkProgram)
	ON_COMMAND(ID_ENK_PRINT, OnEnkPrint)
	ON_COMMAND(ID_ENK_PARAM, OnEnkParam)
	ON_COMMAND(ID_ENK_DOT, OnEnkDot)
	ON_COMMAND(ID_ENK_F1, OnEnkF1)
	ON_COMMAND(ID_ENK_F2, OnEnkF2)
	ON_COMMAND(ID_ENK_F3, OnEnkF3)
	ON_COMMAND(ID_BTN_ENTER, OnBtnEnter)
	ON_WM_TIMER()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnMarkView construction/destruction
#define SLEEP( val ) //::Sleep( (val) );

HDC	gpDC;
CFont gFont8x16, gFont4x6;
#include "EnmApp.hpp"
#include "MigEventMng.h"

void CreateFont( CFont& fnt, INT32U height, INT32U lWeight, INT8U charset, CString fntName )
{
	LOGFONT logFont;
	memset(&logFont, 0, sizeof(LOGFONT));
	
	_tcscpy( logFont.lfFaceName, fntName.GetBuffer( fntName.GetLength()) );
	logFont.lfCharSet	= charset;
	logFont.lfHeight	= height;
	logFont.lfQuality   = 1;
	//logFont.lfUnderline = 60;
	logFont.lfWeight = lWeight;
	fnt.CreateFontIndirect(&logFont);
}

void ClearScreen( INT8U clr  )
{
	// do nothing in VC++
//	InvalidateRect(0, NULL , 1);
}

void PutPixel( int x, int y, long clr )
{
	::SetPixel( gpDC, x, y,  clr );
}

void PutStrThin( int x, int y, char * str,  char bSel )
{
	::SelectObject( gpDC, &gFont4x6 );
	if ( bSel )
	{
#if 1
		// Derive a region from that path 
		SelectClipPath(gpDC, RGN_AND); 
		// This generates the same result as SelectClipPath() 
		SelectClipRgn(gpDC, PathToRegion(gpDC)); 
		// Fill the region with grayness 
		RECT r; 
		r.left = x-3; r.top=y;
		r.right = x + strlen(str)*8;
		r.bottom = y+20;
		::FillRect(gpDC, &r, (HBRUSH)GetStockObject(BLACK_BRUSH)); 
#endif
		SetBkMode(gpDC, TRANSPARENT); 
		// Bracket begin a path 
		//BeginPath(gpDC); 
		// Send some text out into the world 
		COLORREF  aOldClr = ::SetTextColor( gpDC, RGB(255,255,255) );
		::TextOut(gpDC, x,y, str, strlen(str) ); 
		// Bracket end a path 
		//EndPath(gpDC); 
		::SetTextColor( gpDC, aOldClr);
	}
	else
	{
		::TextOut( gpDC, x, y+3,  str, strlen(str) );
#if 0
		if ( pos != (unsigned short)-1 )
		{
			char aBuf[2] = { 0 };
			aBuf[0] = str[pos];
			COLORREF  aOldClr = ::SetTextColor( gpDC, RGB(255,0,0) );
			::TextOut(gpDC, x + pos*8, y+3, aBuf, 1 ); 
			::SetTextColor( gpDC, aOldClr);
		}
#endif
	}
	
}

void PutStr( int x, int y, char * str,  char bSel )
{
	if ( !str )
		return;
	
	::SelectObject( gpDC, &gFont8x16 );
	if ( bSel )
	{
#if 1
		// Derive a region from that path 
		SelectClipPath(gpDC, RGN_AND); 
		// This generates the same result as SelectClipPath() 
		SelectClipRgn(gpDC, PathToRegion(gpDC)); 
		// Fill the region with grayness 
		RECT r; 
		r.left = x-3; r.top=y;
		r.right = x + strlen(str)*8;
		r.bottom = y+20;
		::FillRect(gpDC, &r, (HBRUSH)GetStockObject(BLACK_BRUSH)); 
#endif
		SetBkMode(gpDC, TRANSPARENT); 
		// Bracket begin a path 
		//BeginPath(gpDC); 
		// Send some text out into the world 
		COLORREF  aOldClr = ::SetTextColor( gpDC, RGB(255,255,255) );
		::TextOut(gpDC, x,y, str, strlen(str) ); 
		// Bracket end a path 
		//EndPath(gpDC); 
		::SetTextColor( gpDC, aOldClr);
	}
	else
	{
		::TextOut( gpDC, x, y+3,  str, strlen(str) );
#if 0
		if ( pos != (unsigned short)-1 )
		{
			char aBuf[2] = { 0 };
			aBuf[0] = str[pos];
			COLORREF  aOldClr = ::SetTextColor( gpDC, RGB(255,0,0) );
			::TextOut(gpDC, x + pos*8, y+3, aBuf, 1 ); 
			::SetTextColor( gpDC, aOldClr);
		}
#endif
	}
	
}

void PutBox( int x, int y, int w, int h )
{
#if 1
RECT r; 
r.left = x;
r.top = y;
r.right = x+w;
r.bottom = y+h;
::FillRect(gpDC, &r, (HBRUSH)GetStockObject(WHITE_BRUSH));
#endif
}

void PutRect( int x, int y, int w, int h )
{
	::Rectangle( gpDC, x, y, x+w, y+h );
}

void PutLine( int x, int y, int x1, int y1 )
{
	::MoveToEx( gpDC, x, y ,0);
	::LineTo( gpDC, x1, y1 );
}

void PutRectV( int x, int y, int w, int h, char bTrans )
{
	::Rectangle( gpDC, x, y, x+w, y+h );	
}

void PutLineV( int x, int y, int x1, int y1 )
{
	::MoveToEx( gpDC, x, y ,0);
	::LineTo( gpDC, x1, y1 );	
}




CEnmApp &app = CEnmApp::Instance();


CEnMarkView::CEnMarkView()
{

}

CEnMarkView::~CEnMarkView()
{
//	CEnmModeLib::Instance().Unload();
}

BOOL CEnMarkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CEnMarkView drawing
void CEnMarkView::OnDraw(CDC* pDC)
{
	CEnMarkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	
	gpDC = this->GetDC()->GetSafeHdc();

	//app.AppRun( );
	MigPage::DrawAll();
}



/////////////////////////////////////////////////////////////////////////////
// CEnMarkView printing

BOOL CEnMarkView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CEnMarkView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CEnMarkView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CEnMarkView diagnostics

#ifdef _DEBUG
void CEnMarkView::AssertValid() const
{
	CView::AssertValid();
}

void CEnMarkView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEnMarkDoc* CEnMarkView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEnMarkDoc)));
	return (CEnMarkDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEnMarkView message handlers

void CEnMarkView::SendMSG(long key)
{
	tagMigMSG msg;
	msg.msg = ENM_MSG_KEY;
	msg.param1 = key;

#if 0
	MigPage::mpCurPage->OnMessage( msg );
#else
	MigEventMng::Instance().PushMsg( msg );
	app.AppRun();
	//InvalidateRect( NULL );
	return;
#endif

}

void CEnMarkView::OnEnk0() 
{
	// TODO: Add your command handler code here
	SendMSG( KEY_0_N );
	
}

void CEnMarkView::OnEnk1() 
{
	// TODO: Add your command handler code here
	SendMSG( KEY_1_A );
}

void CEnMarkView::OnEnk2() 
{
	// TODO: Add your command handler code here
	SendMSG( KEY_2_B );
}

void CEnMarkView::OnEnk3() 
{
	// TODO: Add your command handler code here
	SendMSG( KEY_3_C );
}

void CEnMarkView::OnEnk4() 
{
	// TODO: Add your command handler code here
	SendMSG( KEY_4_D );
}

void CEnMarkView::OnEnk5() 
{
	// TODO: Add your command handler code here
	SendMSG( KEY_5_E );
}

void CEnMarkView::OnEnk6() 
{
	// TODO: Add your command handler code here
	SendMSG( KEY_6_F );
}

void CEnMarkView::OnEnk7() 
{
	// TODO: Add your command handler code here
	SendMSG( KEY_7_G );
}

void CEnMarkView::OnEnk8() 
{
	// TODO: Add your command handler code here
	SendMSG( KEY_8_H );
}

void CEnMarkView::OnEnk9() 
{
	// TODO: Add your command handler code here
	SendMSG( KEY_9_P );
	
}

void CEnMarkView::OnUp() 
{
	// TODO: Add your command handler code here
	SendMSG( KEY_Up );
	
}

void CEnMarkView::OnLeft() 
{
	// TODO: Add your command handler code here
	SendMSG( KEY_Left );
	
}

void CEnMarkView::OnRight() 
{
	// TODO: Add your command handler code here
	SendMSG( KEY_Right );
	
}

void CEnMarkView::OnDown() 
{
	// TODO: Add your command handler code here
	SendMSG( KEY_Down );
	
}

void CEnMarkView::OnEnkAdd() 
{
	// TODO: Add your command handler code here
	SendMSG( KEY_DOT_L );
	
}

void CEnMarkView::OnEnkOk() 
{
	// TODO: Add your command handler code here
	SendMSG( KEY_GO );	
}

void CEnMarkView::OnEnkCheck() 
{
	SendMSG( KEY_TEST );	
}

void CEnMarkView::OnEnkData() 
{
	SendMSG( KEY_Data );
}

void CEnMarkView::OnEnkExit() 
{
	// TODO: Add your command handler code here
	SendMSG( KEY_ESC );
}

void CEnMarkView::OnEnkProgram() 
{
	SendMSG( KEY_PRG );	
}

void CEnMarkView::OnEnkPrint() 
{
	SendMSG( KEY_Prn ); //KEY_TEST );
}

void CEnMarkView::OnEnkParam() 
{
	SendMSG( KEY_Par );
	
}

void CEnMarkView::OnEnkDot() 
{
	// TODO: Add your command handler code here
	SendMSG( KEY_DOT_L );
	
}

void CEnMarkView::OnEnkF1() 
{
	// TODO: Add your command handler code here
	SendMSG( KEY_FUN1 );

}

void CEnMarkView::OnEnkF2() 
{
	// TODO: Add your command handler code here
	SendMSG( KEY_FUN2 );
	
}

void CEnMarkView::OnEnkF3() 
{
	SendMSG( KEY_FUN3 );
}

void CEnMarkView::OnBtnEnter() 
{
	SendMSG( KEY_OK );	
}

void CEnMarkView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CEnmApp::Instance().AppRun();

	CRect rc;
	GetClientRect( &rc );
	InvalidateRect( &rc );

	CView::OnTimer(nIDEvent);
}

int CEnMarkView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	/// Create SimuGDI font
	CreateFont( gFont8x16, 16, 0, DEFAULT_CHARSET, _T("ËÎÌו")  );
	CreateFont( gFont4x6, 24, 0, DEFAULT_CHARSET, _T("ËÎÌו")  );

	app.InitApp();
	SetTimer( 1, 400, 0 );

	return 0;
}

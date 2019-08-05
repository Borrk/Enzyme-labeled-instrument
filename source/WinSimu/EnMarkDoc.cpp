// EnMarkDoc.cpp : implementation of the CEnMarkDoc class
//

#include "stdafx.h"
#include "EnMark.h"

#include "EnMarkDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnMarkDoc

IMPLEMENT_DYNCREATE(CEnMarkDoc, CDocument)

BEGIN_MESSAGE_MAP(CEnMarkDoc, CDocument)
	//{{AFX_MSG_MAP(CEnMarkDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnMarkDoc construction/destruction

CEnMarkDoc::CEnMarkDoc()
{
	// TODO: add one-time construction code here

}

CEnMarkDoc::~CEnMarkDoc()
{
}

BOOL CEnMarkDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CEnMarkDoc serialization

void CEnMarkDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CEnMarkDoc diagnostics

#ifdef _DEBUG
void CEnMarkDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEnMarkDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEnMarkDoc commands

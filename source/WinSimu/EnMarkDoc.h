// EnMarkDoc.h : interface of the CEnMarkDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENMARKDOC_H__F8FD93BC_7C21_436E_B0FD_64A84562B8F8__INCLUDED_)
#define AFX_ENMARKDOC_H__F8FD93BC_7C21_436E_B0FD_64A84562B8F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CEnMarkDoc : public CDocument
{
protected: // create from serialization only
	CEnMarkDoc();
	DECLARE_DYNCREATE(CEnMarkDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnMarkDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEnMarkDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CEnMarkDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENMARKDOC_H__F8FD93BC_7C21_436E_B0FD_64A84562B8F8__INCLUDED_)

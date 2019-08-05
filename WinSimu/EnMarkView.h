// EnMarkView.h : interface of the CEnMarkView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENMARKVIEW_H__C60A1286_5BCE_43E5_8F04_BB1F2E15A43A__INCLUDED_)
#define AFX_ENMARKVIEW_H__C60A1286_5BCE_43E5_8F04_BB1F2E15A43A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CEnMarkView : public CView
{
protected: // create from serialization only
	CEnMarkView();
	DECLARE_DYNCREATE(CEnMarkView)

// Attributes
public:
	CEnMarkDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnMarkView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEnMarkView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	void SendMSG( long key );
	//{{AFX_MSG(CEnMarkView)
	afx_msg void OnEnk1();
	afx_msg void OnEnk2();
	afx_msg void OnEnk3();
	afx_msg void OnEnk4();
	afx_msg void OnEnk5();
	afx_msg void OnEnk6();
	afx_msg void OnEnk7();
	afx_msg void OnEnk8();
	afx_msg void OnEnk0();
	afx_msg void OnEnk9();
	afx_msg void OnUp();
	afx_msg void OnLeft();
	afx_msg void OnRight();
	afx_msg void OnDown();
	afx_msg void OnEnkAdd();
	afx_msg void OnEnkOk();
	afx_msg void OnEnkCheck();
	afx_msg void OnEnkData();
	afx_msg void OnEnkExit();
	afx_msg void OnEnkProgram();
	afx_msg void OnEnkPrint();
	afx_msg void OnEnkParam();
	afx_msg void OnEnkDot();
	afx_msg void OnEnkF1();
	afx_msg void OnEnkF2();
	afx_msg void OnEnkF3();
	afx_msg void OnBtnEnter();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in EnMarkView.cpp
inline CEnMarkDoc* CEnMarkView::GetDocument()
   { return (CEnMarkDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENMARKVIEW_H__C60A1286_5BCE_43E5_8F04_BB1F2E15A43A__INCLUDED_)

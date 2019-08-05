// EnMark.h : main header file for the ENMARK application
//

#if !defined(AFX_ENMARK_H__C7F70E21_9A85_4B11_8A43_4CC67BFF96DC__INCLUDED_)
#define AFX_ENMARK_H__C7F70E21_9A85_4B11_8A43_4CC67BFF96DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#include "EnmModeLib.h"

/////////////////////////////////////////////////////////////////////////////
// CEnMarkApp:
// See EnMark.cpp for the implementation of this class
//

class CEnMarkApp : public CWinApp
{
public:
	CEnMarkApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnMarkApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CEnMarkApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	void ReleaseSimResource();
	void CreateSimRunContext();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENMARK_H__C7F70E21_9A85_4B11_8A43_4CC67BFF96DC__INCLUDED_)

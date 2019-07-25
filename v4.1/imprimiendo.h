#if !defined(AFX_IMPRIMIENDO_H__0C5212D1_5E8B_11D2_A79C_0000E832DAC0__INCLUDED_)
#define AFX_IMPRIMIENDO_H__0C5212D1_5E8B_11D2_A79C_0000E832DAC0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Imprimiendo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImprimiendo dialog

class CImprimiendo : public CDialog
{
// Construction
public:
	void SetPage (char *page);
	CImprimiendo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CImprimiendo)
	enum { IDD = IDD_IMPRIMIENDO };
	CString	m_pagina;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImprimiendo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CImprimiendo)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPRIMIENDO_H__0C5212D1_5E8B_11D2_A79C_0000E832DAC0__INCLUDED_)

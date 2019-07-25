#if !defined(AFX_PRESENT_H__2A332B03_1D53_11D2_A748_0000E832DAC0__INCLUDED_)
#define AFX_PRESENT_H__2A332B03_1D53_11D2_A748_0000E832DAC0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Present.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPresent dialog

class CPresent : public CDialog
{
// Construction
public:
	CPresent(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPresent)
	enum { IDD = IDD_DIALOG4 };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPresent)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPresent)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRESENT_H__2A332B03_1D53_11D2_A748_0000E832DAC0__INCLUDED_)

#if !defined(AFX_NEWPASSDLG_H__2A332B02_1D53_11D2_A748_0000E832DAC0__INCLUDED_)
#define AFX_NEWPASSDLG_H__2A332B02_1D53_11D2_A748_0000E832DAC0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// NewPassDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewPassDlg dialog

class CNewPassDlg : public CDialog
{
// Construction
public:
	CNewPassDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewPassDlg)
	enum { IDD = IDD_DIALOG3 };
	CString	m_NewPass;
	CString	m_NewPass2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewPassDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewPassDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPASSDLG_H__2A332B02_1D53_11D2_A748_0000E832DAC0__INCLUDED_)

#if !defined(AFX_DIALOGCLRCAB_H__EDFCF402_1BC0_11D2_A746_0000E832DAC0__INCLUDED_)
#define AFX_DIALOGCLRCAB_H__EDFCF402_1BC0_11D2_A746_0000E832DAC0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DialogClrCab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogClrCab dialog

class CDialogClrCab : public CDialog
{
// Construction
public:
	CDialogClrCab(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogClrCab)
	enum { IDD = IDD_DIALOG1 };
	CSpinButtonCtrl	m_spin;
	UINT	m_Cabina;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogClrCab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogClrCab)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGCLRCAB_H__EDFCF402_1BC0_11D2_A746_0000E832DAC0__INCLUDED_)

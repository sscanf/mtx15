#if !defined(AFX_DIALOGCLRCHAN_H__EDFCF401_1BC0_11D2_A746_0000E832DAC0__INCLUDED_)
#define AFX_DIALOGCLRCHAN_H__EDFCF401_1BC0_11D2_A746_0000E832DAC0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DialogClrChan.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogClrChan dialog

class CDialogClrChan : public CDialog
{
// Construction
public:
	CDialogClrChan(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogClrChan)
	enum { IDD = IDD_DIALOG_BORRAR };
	CEdit	m_CtrlCanal;
	CSpinButtonCtrl	m_spin;
	UINT	m_Canal;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogClrChan)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogClrChan)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGCLRCHAN_H__EDFCF401_1BC0_11D2_A746_0000E832DAC0__INCLUDED_)

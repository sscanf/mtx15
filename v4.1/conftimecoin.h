#if !defined(AFX_CONFTIMECOIN_H__9CBBC191_AC0D_11D2_A878_0000E832DAC0__INCLUDED_)
#define AFX_CONFTIMECOIN_H__9CBBC191_AC0D_11D2_A878_0000E832DAC0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ConfTimeCoin.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfTimeCoin dialog

class CConfTimeCoin : public CDialog
{
// Construction
public:
	CConfTimeCoin(CWnd* pParent = NULL);   // standard constructor

	int m_ActuacionTeclas;
// Dialog Data
	//{{AFX_DATA(CConfTimeCoin)
	enum { IDD = IDD_CONFIG_COINTIME };
	int		m_time;
	int		m_TotalCabinas;
	int		m_TotalCanales;
	CString	m_ComPort;
	BOOL	m_bOcultarCanal;
	BOOL	m_bSaltoCanal;
	UINT	m_SegundosCredito;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfTimeCoin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush m_brush;

	// Generated message map functions
	//{{AFX_MSG(CConfTimeCoin)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnTeclaActua();
	afx_msg void OnTeclaNoActua();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFTIMECOIN_H__9CBBC191_AC0D_11D2_A878_0000E832DAC0__INCLUDED_)

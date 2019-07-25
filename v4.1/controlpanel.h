#if !defined(AFX_CONTROLPANEL_H__04E70940_A529_4C49_92F8_65203D10727D__INCLUDED_)
#define AFX_CONTROLPANEL_H__04E70940_A529_4C49_92F8_65203D10727D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HoverButton.h"

// ControlPanel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CControlPanel dialog

#define IDT_TIMER_TX	0x100
#define IDT_TIMER_RX	0x101

class CControlPanel : public CDialog
{
// Construction
public:
	BOOL m_bEncendido;
	void DatosTx (void);
	void DatosRx (void);
	CControlPanel(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CControlPanel)
	enum { IDD = IDD_CONTROL };
	CStatic	m_DatosTx;
	CStatic	m_DatosRx;
	CStatic	m_led;
	CHoverButton m_switch;
	CHoverButton m_reset;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlPanel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush m_brush;
	HICON m_LuzOn;
	CBitmap m_LuzOff;

	// Generated message map functions
	//{{AFX_MSG(CControlPanel)
	virtual BOOL OnInitDialog();
	afx_msg void OnEncender();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTROLPANEL_H__04E70940_A529_4C49_92F8_65203D10727D__INCLUDED_)

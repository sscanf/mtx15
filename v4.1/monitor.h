#if !defined(AFX_MONITOR_H__DEA11321_4628_11D2_BCBE_0000E832DAC0__INCLUDED_)
#define AFX_MONITOR_H__DEA11321_4628_11D2_BCBE_0000E832DAC0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Monitor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMonitor dialog
// Esta clase controla el monitor
// El monitor está pensado para monitorizar mensajes
// de control que envía el equipo al PC.
// Estos mensajes de control pueden ser warnings en el sistema,
// errores o PINGS.

class CMonitor : public CDialog
{
// Construction
public:
	CMonitor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMonitor)
	enum { IDD = IDD_MONITOR };
	CButton	m_boton2;
	CListBox	m_ListBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonitor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_modo;

	// Generated message map functions
	//{{AFX_MSG(CMonitor)
		afx_msg LRESULT OnRxPing (WPARAM, LPARAM);
		afx_msg LRESULT OnRxPong (WPARAM, LPARAM);
		afx_msg LRESULT OnErrorCabina (WPARAM, LPARAM);
		afx_msg LRESULT OnRxByte (WPARAM, LPARAM);
		afx_msg LRESULT OnRxReset (WPARAM,LPARAM);
		afx_msg void OnRadio1();
		afx_msg void OnRadio2();
		afx_msg void OnButton2();
		afx_msg LRESULT OnMoneda(WPARAM, LPARAM);
		afx_msg void OnTimer(UINT nIDEvent);
		virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONITOR_H__DEA11321_4628_11D2_BCBE_0000E832DAC0__INCLUDED_)

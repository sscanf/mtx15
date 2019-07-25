#if !defined(AFX_HISTORIAL_H__4ECAFF51_5C2F_11D2_A799_0000E832DAC0__INCLUDED_)
#define AFX_HISTORIAL_H__4ECAFF51_5C2F_11D2_A799_0000E832DAC0__INCLUDED_

#include "SexShopDoc.h"
#include "Dia.h"	// Added by ClassView
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Historial.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHistorial dialog

class CHistorial : public CDialog
{
// Construction
public:
	CSexShopDoc *m_pDoc;
	CHistorial(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHistorial)
	enum { IDD = IDD_HISTORIAL };
	CListBox	m_historial;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistorial)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHistorial)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnMostrarAcumulado();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTORIAL_H__4ECAFF51_5C2F_11D2_A799_0000E832DAC0__INCLUDED_)

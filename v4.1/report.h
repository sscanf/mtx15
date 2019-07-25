#if !defined(AFX_REPORT_H__4ECAFF52_5C2F_11D2_A799_0000E832DAC0__INCLUDED_)
#define AFX_REPORT_H__4ECAFF52_5C2F_11D2_A799_0000E832DAC0__INCLUDED_

#include "Dia.h"	// Added by ClassView
#include "Canal.h"	// Added by ClassView
#include "Imprimiendo.h"	// Added by ClassView
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Report.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReport dialog

class CReport : public CDialog
{
// Construction
public:
	CDia *m_pDia;
	BOOL m_bAcumulado;
	CTypedPtrList <CObList,CDia *> *m_pDias;
	CReport(CWnd* pParent = NULL);   // standard constructor
	~CReport ();
	CArray <CCabina *,CCabina *> m_newCabinas;
	CArray <CCanal *,CCanal *> m_newCanales;

// Dialog Data
	//{{AFX_DATA(CReport)
	enum { IDD = IDD_DIALOG_HISTO };
	CListCtrl	m_canales;
	CListCtrl	m_cabinas;
	UINT	m_TotalPts;
	UINT	m_TotalPtsTarjetas;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReport)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReport)
	virtual BOOL OnInitDialog();
	afx_msg void OnImprimir();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnImprimirCanales();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORT_H__4ECAFF52_5C2F_11D2_A799_0000E832DAC0__INCLUDED_)

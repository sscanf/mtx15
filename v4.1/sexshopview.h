// SexShopView.h : interface of the CSexShopView class
//
/////////////////////////////////////////////////////////////////////////////
//{{AFX_INCLUDES()
//}}AFX_INCLUDES

#if !defined(AFX_SEXSHOPVIEW_H__4529334C_10D4_11D2_A730_0000E832DAC0__INCLUDED_)
#define AFX_SEXSHOPVIEW_H__4529334C_10D4_11D2_A730_0000E832DAC0__INCLUDED_

#include "ControlPanel.h"	// Added by ClassView
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "SexShopDoc.h"
#include "Monitor.h"	// Added by ClassView
#include "digistring.h"
#include "hoverbutton.h"

class CSexShopView : public CFormView
{
protected: // create from serialization only
	CSexShopView();
	DECLARE_DYNCREATE(CSexShopView)

public:
	//{{AFX_DATA(CSexShopView)
	enum { IDD = IDD_SEXSHOP_FORM };
	CStatic	m_ledRX;
	CStatic	m_ledTX;
	CStatic	m_ledOnOff;
	CHoverButton m_onoff;
	CHoverButton m_reset;
	CDigistring m_DispSalidas;
	CDigistring m_DispEntradas;
	CComboBox	m_CtrlSalidas;
	CComboBox	m_CtrlEntradas;
	//}}AFX_DATA

// Attributes
public:
	CSexShopDoc* GetDocument();
	UINT m_TotalCabinas;
	UINT m_TotalCanales;
	BOOL m_bEncendido;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSexShopView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSexShopView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


public:
	void ShowError (int NumError, int NumCabina);

protected:

	//{{AFX_MSG(CSexShopView)
	afx_msg void OnPaint();
	afx_msg void OnMatrizActivarto();
	afx_msg void OnMatrizDetenertodaslascabinas();
	afx_msg void OnMatrizReiniciar();
	afx_msg void OnEdicinTiempomon();
	afx_msg void OnEncender();
	afx_msg void OnEnviar();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

#ifndef _DEBUG  // debug version in SexShopView.cpp
inline CSexShopDoc* CSexShopView::GetDocument()
   { return (CSexShopDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEXSHOPVIEW_H__4529334C_10D4_11D2_A730_0000E832DAC0__INCLUDED_)

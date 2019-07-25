#if !defined(AFX_CABINAPROP_H__346BDB89_DBDF_4E4F_9715_5EE34EDA7135__INCLUDED_)
#define AFX_CABINAPROP_H__346BDB89_DBDF_4E4F_9715_5EE34EDA7135__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dia.h"
#include "Cabina.h"
#include "hoverbutton.h"
#include "digistring.h"
// CabinaProp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCabinaProp dialog

class CCabinaProp : public CDialog
{
// Construction
public:
	CCabinaProp(CWnd* pParent = NULL);   // standard constructor
	CDia *m_pDia;

// Dialog Data
	//{{AFX_DATA(CCabinaProp)
	enum { IDD = IDD_CABINAS_PROP };
	CDigistring m_canalAct;
	CSpinButtonCtrl	m_ctrlSpin;
	CStatic	m_LuzEstado;
	CHoverButton m_onoff;
	int		m_CanalFinal;
	int		m_CanalInicial;
	BOOL	m_bDeshabilitada;
	int		m_ValorCredito1;
	int		m_ValorCredito2;
	int		m_ValorDescuento;
	CString	m_msg;
	CString	m_strNumero;
	//}}AFX_DATA

	CCabina *m_pCabina;
	int m_cabina;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCabinaProp)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCabinaProp)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEncender();
	afx_msg void OnChangeCanalAct();
	afx_msg void OnUpdateCanalAct();
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAplicar();
	afx_msg void OnBorrarEstadisticas();
	afx_msg void OnEnviar();
	afx_msg void OnBorrarTexto();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPeticionNo();
	afx_msg void OnPeticionSi();
	afx_msg void OnCanalmas();
	afx_msg void OnCanalmenos();
	afx_msg void OnSelchangeNumero();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CBrush m_brush;
	HACCEL m_hAccelTable;

	int m_CanalActual;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CABINAPROP_H__346BDB89_DBDF_4E4F_9715_5EE34EDA7135__INCLUDED_)

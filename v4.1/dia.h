#if !defined(AFX_DIA_H__984D3BF1_5A0B_11D2_A798_0000E832DAC0__INCLUDED_)
#define AFX_DIA_H__984D3BF1_5A0B_11D2_A798_0000E832DAC0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Dia.h : header file
//
#include <afxtempl.h>
#include "cabina.h"
#include "canal.h"
#include "afxmt.h"
/////////////////////////////////////////////////////////////////////////////
// CDia document

class CDia : public CObject
{
protected:
	BOOL m_TipoMoneda;
	DECLARE_SERIAL (CDia);
	CMutex mutex,CanalMutex;

// Attributes
public:
	CTime m_fecha;
	CTypedPtrList <CObList,CCabina *> m_pCabinas;
	CTypedPtrList <CObList,CCanal *> m_pCanales;
	UINT m_TotalCanales;
	UINT m_TotalCabinas;
	CArray <CCabina *, CCabina *> m_CabinasArr;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDia)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	//}}AFX_VIRTUAL

// Implementation
public:
	CCabina * BuscaCabina (UINT cabina);
	CCanal * BuscaCanal (UINT canal);
	CDia();           // protected constructor used by dynamic creation
	CDia(UINT TotalCabinas,UINT TotalCanales, BOOL TipoMoneda );           // protected constructor used by dynamic creation
	virtual ~CDia();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_DIA_H__984D3BF1_5A0B_11D2_A798_0000E832DAC0__INCLUDED_)

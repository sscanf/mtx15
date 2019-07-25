#if !defined(AFX_MENSAJE_H__6EFDB1CC_C2B4_457D_8F72_CFD127F7B56F__INCLUDED_)
#define AFX_MENSAJE_H__6EFDB1CC_C2B4_457D_8F72_CFD127F7B56F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Mensaje.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMensaje dialog

class CMensaje : public CDialog
{
// Construction
public:
	void mensaje (CString mensaje);
	CMensaje(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMensaje)
	enum { IDD = IDD_MENSAJE };
	CString	m_mensaje;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMensaje)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMensaje)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENSAJE_H__6EFDB1CC_C2B4_457D_8F72_CFD127F7B56F__INCLUDED_)

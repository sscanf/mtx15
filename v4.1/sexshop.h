// SexShop.h : main header file for the SEXSHOP application
//

#if !defined(AFX_SEXSHOP_H__45293344_10D4_11D2_A730_0000E832DAC0__INCLUDED_)
#define AFX_SEXSHOP_H__45293344_10D4_11D2_A730_0000E832DAC0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "Present.h"	// Added by ClassView


/////////////////////////////////////////////////////////////////////////////
// CSexShopApp:
// See SexShop.cpp for the implementation of this class
//

class CSexShopApp : public CWinApp
{
public:
	CSexShopApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSexShopApp)
	public:
	virtual BOOL InitInstance();
	virtual void DoWaitCursor(int nCode);
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSexShopApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:

	void abrir (CString puerto);
    void CommRx(unsigned char *msg, DWORD len);

	CPresent *m_pPresent;
	CString pass;


public:
	BOOL m_ebslkey;
	BOOL m_administrador;
	CString m_PassAdministrador;
	CString m_PassDependiente;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL m_flag;
	CSingleDocTemplate* m_pDocTemplate;
};

extern CSexShopApp near theApp;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEXSHOP_H__45293344_10D4_11D2_A730_0000E832DAC0__INCLUDED_)


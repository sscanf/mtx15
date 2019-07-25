// Imprimiendo.cpp : implementation file
//

#include "stdafx.h"
#include "SexShop.h"
#include "Imprimiendo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImprimiendo dialog


CImprimiendo::CImprimiendo(CWnd* pParent /*=NULL*/)
	: CDialog(CImprimiendo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImprimiendo)
	m_pagina = _T("");
	//}}AFX_DATA_INIT
}


void CImprimiendo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImprimiendo)
	DDX_Text(pDX, IDC_PAGINA, m_pagina);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImprimiendo, CDialog)
	//{{AFX_MSG_MAP(CImprimiendo)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImprimiendo message handlers

void CImprimiendo::SetPage(char * page)
{
	m_pagina="Imprimiendo linea ";
	m_pagina+=page;
	UpdateData (FALSE);
}

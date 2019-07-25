// NewPassDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SexShop.h"
#include "NewPassDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewPassDlg dialog


CNewPassDlg::CNewPassDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewPassDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewPassDlg)
	m_NewPass = _T("");
	m_NewPass2 = _T("");
	//}}AFX_DATA_INIT
}


void CNewPassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewPassDlg)
	DDX_Text(pDX, IDC_EDIT1, m_NewPass);
	DDX_Text(pDX, IDC_EDIT2, m_NewPass2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewPassDlg, CDialog)
	//{{AFX_MSG_MAP(CNewPassDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewPassDlg message handlers

void CNewPassDlg::OnOK() 
{
	UpdateData (TRUE);
	CDialog::OnOK();
}

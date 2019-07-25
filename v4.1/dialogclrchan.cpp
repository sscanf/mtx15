// DialogClrChan.cpp : implementation file
//

#include "stdafx.h"
#include "SexShop.h"
#include "DialogClrChan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogClrChan dialog


CDialogClrChan::CDialogClrChan(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogClrChan::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogClrChan)
	m_Canal = 0;
	//}}AFX_DATA_INIT
}


void CDialogClrChan::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogClrChan)
	DDX_Control(pDX, IDC_EDIT1, m_CtrlCanal);
	DDX_Control(pDX, IDC_SPIN1, m_spin);
	DDX_Text(pDX, IDC_EDIT1, m_Canal);
	DDV_MinMaxUInt(pDX, m_Canal, 1, 64);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogClrChan, CDialog)
	//{{AFX_MSG_MAP(CDialogClrChan)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogClrChan message handlers

BOOL CDialogClrChan::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	m_spin.SetRange (64,1);
	m_Canal=1;
	UpdateData (FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogClrChan::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
	UpdateData (TRUE);
}

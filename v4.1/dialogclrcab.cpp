// DialogClrCab.cpp : implementation file
//

#include "stdafx.h"
#include "SexShop.h"
#include "DialogClrCab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogClrCab dialog


CDialogClrCab::CDialogClrCab(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogClrCab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogClrCab)
	m_Cabina = 0;
	//}}AFX_DATA_INIT
}


void CDialogClrCab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogClrCab)
	DDX_Control(pDX, IDC_SPIN1, m_spin);
	DDX_Text(pDX, IDC_EDIT1, m_Cabina);
	DDV_MinMaxUInt(pDX, m_Cabina, 1, 20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogClrCab, CDialog)
	//{{AFX_MSG_MAP(CDialogClrCab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogClrCab message handlers

BOOL CDialogClrCab::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Cabina=1;
	UpdateData (FALSE);
	
	m_spin.SetRange (20,1);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDialogClrCab::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();

	UpdateData (TRUE);
}

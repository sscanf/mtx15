// Present.cpp : implementation file
//

#include "stdafx.h"
#include "SexShop.h"
#include "Present.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPresent dialog


CPresent::CPresent(CWnd* pParent /*=NULL*/)
	: CDialog(CPresent::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPresent)
	//}}AFX_DATA_INIT

	Create (IDD);
	ShowWindow (SW_SHOW);
}


void CPresent::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPresent)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPresent, CDialog)
	//{{AFX_MSG_MAP(CPresent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPresent message handlers

BOOL CPresent::OnInitDialog() 
{
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

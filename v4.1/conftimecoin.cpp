// ConfTimeCoin.cpp : implementation file
//

#include "stdafx.h"
#include "SexShop.h"
#include "ConfTimeCoin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfTimeCoin dialog


CConfTimeCoin::CConfTimeCoin(CWnd* pParent /*=NULL*/)
	: CDialog(CConfTimeCoin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfTimeCoin)
	m_time = 0;
	m_TotalCabinas = 0;
	m_TotalCanales = 0;
	m_ComPort = _T("");
	m_bOcultarCanal = FALSE;
	m_bSaltoCanal = FALSE;
	m_SegundosCredito = 0;
	//}}AFX_DATA_INIT
}


void CConfTimeCoin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfTimeCoin)
	DDX_CBIndex (pDX, IDC_TOTAL_CABINAS, m_TotalCabinas);
	DDX_CBIndex (pDX, IDC_TOTAL_CANALES, m_TotalCanales);
	DDX_CBString(pDX, IDC_COMPORT, m_ComPort);
	DDX_Check(pDX, IDC_OCULTAR_CANAL, m_bOcultarCanal);
	DDX_Check(pDX, IDC_SALTAR_CANALES, m_bSaltoCanal);
	DDX_Text(pDX, IDC_SEGUNDOS_CREDITO, m_SegundosCredito);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfTimeCoin, CDialog)
	//{{AFX_MSG_MAP(CConfTimeCoin)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_TECLA_ACTUA, OnTeclaActua)
	ON_BN_CLICKED(IDC_TECLA_NO_ACTUA, OnTeclaNoActua)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfTimeCoin message handlers

HBRUSH CConfTimeCoin::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_DLG ||nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor (RGB (192,192,192));
		return m_brush;
	}
	return hbr;
}

BOOL CConfTimeCoin::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_brush.CreateSolidBrush(RGB (192,192,192));
	
	UpdateData (FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfTimeCoin::OnTeclaActua()
{
	UpdateData (FALSE);
}

void CConfTimeCoin::OnTeclaNoActua()
{
	UpdateData (FALSE);

}

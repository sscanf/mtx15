// Historial.cpp : implementation file
//

#include "stdafx.h"
#include "SexShop.h"
#include "Historial.h"
#include "report.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistorial dialog


CHistorial::CHistorial(CWnd* pParent /*=NULL*/)
	: CDialog(CHistorial::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHistorial)
	//}}AFX_DATA_INIT
}


void CHistorial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHistorial)
	DDX_Control(pDX, IDC_HISTORIAL, m_historial);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHistorial, CDialog)
	//{{AFX_MSG_MAP(CHistorial)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnMostrarAcumulado)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistorial message handlers

BOOL CHistorial::OnInitDialog() 
{
	CDialog::OnInitDialog();

	char buffer[80];
	
	CDia *pDia;

	POSITION pos;

	int count = m_pDoc->m_pDias.GetCount();

	pos=m_pDoc->m_pDias.GetHeadPosition();
	while( pos != NULL )
	{
		pDia = m_pDoc->m_pDias.GetNext (pos);
		
		int dia = pDia->m_fecha.GetDay();
		int mes = pDia->m_fecha.GetMonth();
		int any = pDia->m_fecha.GetYear();

		sprintf (buffer,"%d/%d/%d",dia,mes,any);
		m_historial.AddString (buffer);
	}



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHistorial::OnButton1() 
{
	CReport report;
	POSITION pos;

	int sel = m_historial.GetSel(0);

	pos=m_pDoc->m_pDias.GetHeadPosition();
	
	int n=0;
	while (pos && !m_historial.GetSel (n))
	{
		m_pDoc->m_pDias.GetNext(pos);
		n++;
	}

	if (!pos) 
		return ;

	report.m_pDia = m_pDoc->m_pDias.GetAt (pos);
	report.DoModal();

}


void CHistorial::OnMostrarAcumulado() 
{
	CReport report;
	POSITION pos;

	report.m_bAcumulado=TRUE;
	report.m_pDias = &m_pDoc->m_pDias;
	report.DoModal();

}

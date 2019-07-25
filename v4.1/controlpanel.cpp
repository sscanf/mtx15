// ControlPanel.cpp : implementation file
//

#include "stdafx.h"
#include "sexshop.h"
#include "ControlPanel.h"
#include "SexShopdoc.h"
#include "sexshopview.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControlPanel dialog


CControlPanel::CControlPanel(CWnd* pParent /*=NULL*/)
	: CDialog(CControlPanel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CControlPanel)
	//}}AFX_DATA_INIT
}


void CControlPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CControlPanel)
	DDX_Control(pDX, IDC_DATOS_TX, m_DatosTx);
	DDX_Control(pDX, IDC_DATOS_RX, m_DatosRx);
	DDX_Control(pDX, IDC_LED_APAGADO, m_led);
	DDX_Control(pDX, IDC_ENCENDER, m_switch);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CControlPanel, CDialog)
	//{{AFX_MSG_MAP(CControlPanel)
	ON_BN_CLICKED(IDC_ENCENDER, OnEncender)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlPanel message handlers

BOOL CControlPanel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CMainFrame *pFrame = (CMainFrame *)GetParent();
	CSexShopView *pView = (CSexShopView *)pFrame->GetActiveView();
	CSexShopDoc *pDoc = (CSexShopDoc *)pView->GetDocument();
	
	m_brush.CreateSolidBrush(RGB (192,192,184));

	m_switch.LoadBitmap(IDB_BITMAP2);
	CString text=_T("Activar/Desactivar");
	m_switch.SetToolTipText(&text);
	m_led.SetIcon (AfxGetApp()->LoadIcon(IDI_LED_ENCENDIDO));
	m_bEncendido=TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CControlPanel::OnEncender() 
{
	CMainFrame *pFrame = (CMainFrame *)GetParent();
	CSexShopView *pView = (CSexShopView *)pFrame->GetActiveView();
	CSexShopDoc *pDoc = (CSexShopDoc *)pView->GetDocument();

	if (!pDoc->m_SysStatus.GetStatus()&01)	//Está apagado
	{
		m_switch.LoadBitmap(IDB_BITMAP2);

	
		m_led.SetIcon (AfxGetApp()->LoadIcon(IDI_LED_ENCENDIDO));

		m_bEncendido=TRUE;
		POSITION posDias=pDoc->m_pDias.GetTailPosition();
		CDia *pDia=pDoc->m_pDias.GetNext(posDias);
		
		POSITION pos=pDia->m_pCabinas.GetHeadPosition();

		for (int n=0;n<pDia->m_TotalCabinas;n++)
		{
			CCabina *pCab=pDia->m_pCabinas.GetNext(pos);	
			pCab->ClearErrors();
		}
		pDoc->m_SysStatus.Ping();
		pDoc->m_SysStatus.Enciende(&pDoc->m_serial); //Enciende el equipo
		pDoc->m_SysStatus.m_intentos=0; //Enciende el equipo
	}
	else
	{
		m_bEncendido=FALSE;
		m_switch.LoadBitmap(IDB_BITMAP2);
		pDoc->m_SysStatus.Apaga(&pDoc->m_serial); //Apaga el equipo
		m_led.SetIcon (AfxGetApp()->LoadIcon(IDI_LED_APAGADO));

		// Desactivamos todas las cabinas.

		POSITION pos=pDoc->m_pDias.GetTailPosition();
		CDia *pDia=pDoc->m_pDias.GetNext(pos);
		
		POSITION pos2=pDia->m_pCabinas.GetHeadPosition();
		for (int n=0;n<pDia->m_TotalCabinas;n++)
		{
			CCabina *pCabina=pDia->m_pCabinas.GetNext(pos2);
			pCabina->SetStatus (0x00);
		}

		pDoc->OnSaveDocument("ebsl.log");

	}
}


HBRUSH CControlPanel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (nCtlColor == CTLCOLOR_DLG ||nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor (RGB (192,192,192));
		return m_brush;
	}
	return hbr;
}

void CControlPanel::DatosRx()
{
	m_DatosRx.SetIcon (AfxGetApp()->LoadIcon(IDI_LED_ENCENDIDO_VERDE1));
	SetTimer (IDT_TIMER_RX,300,NULL);

}

void CControlPanel::DatosTx()
{
	m_DatosTx.SetIcon (AfxGetApp()->LoadIcon(IDI_LED_ENCENDIDO_VERDE1));
	SetTimer (IDT_TIMER_TX,300,NULL);
}

void CControlPanel::OnTimer(UINT nIDEvent) 
{
	
	if (nIDEvent == IDT_TIMER_RX)
	{
		m_DatosRx.SetIcon (AfxGetApp()->LoadIcon(IDI_LED_APAGADO_VERDE));
		KillTimer (IDT_TIMER_RX);
	}

	if (nIDEvent == IDT_TIMER_TX)
	{
		m_DatosTx.SetIcon (AfxGetApp()->LoadIcon(IDI_LED_APAGADO_VERDE));
		KillTimer (IDT_TIMER_TX);
	}

	CDialog::OnTimer(nIDEvent);
}

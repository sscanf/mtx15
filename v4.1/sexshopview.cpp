// SexShopView.cpp : implementation of the CSexShopView class
//

#include "stdafx.h"
#include "SexShop.h"
#include "ConfTimeCoin.h"
#include "afxmt.h"

#include "SexShopDoc.h"
#include "SexShopView.h"
#include "MainFrm.h"
#include <stdlib.h>
#include <stdio.h>
#include "canal.h"
#include "historial.h"
#include "CBmpMenu.h"
#include "CabinaProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDT_SIMULACION	1
/////////////////////////////////////////////////////////////////////////////
// CSexShopView

IMPLEMENT_DYNCREATE(CSexShopView, CFormView)

BEGIN_MESSAGE_MAP(CSexShopView, CFormView)
	//{{AFX_MSG_MAP(CSexShopView)
	ON_WM_PAINT()
	ON_COMMAND(ID_MATRIZ_ACTIVARTO, OnMatrizActivarto)
	ON_COMMAND(ID_MATRIZ_DETENERTODASLASCABINAS, OnMatrizDetenertodaslascabinas)
	ON_COMMAND(ID_MATRIZ_REINICIAR, OnMatrizReiniciar)
	ON_COMMAND(ID_EDICIN_TIEMPOMON, OnEdicinTiempomon)
	ON_BN_CLICKED(IDC_ENCENDER, OnEncender)
	ON_BN_CLICKED(IDC_ENVIAR, OnEnviar)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RESET, OnReset)
	//}}AFX_MSG_MAP
	// Standard printing commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSexShopView construction/destruction

CSexShopView::CSexShopView()
	: CFormView(CSexShopView::IDD)
{
	//{{AFX_DATA_INIT(CSexShopView)
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	m_TotalCabinas = (AfxGetApp()->GetProfileInt ("mtx15","TotalCabinas",4)+1);
	m_TotalCanales = (AfxGetApp()->GetProfileInt ("mtx15","TotalCanales",4)+1)*64;
	m_bEncendido=FALSE;
}


CSexShopView::~CSexShopView()
{
}

void CSexShopView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSexShopView)
	DDX_Control(pDX, IDC_DATOS_RX, m_ledRX);
	DDX_Control(pDX, IDC_DATOS_TX, m_ledTX);
	DDX_Control(pDX, IDC_LED_APAGADO, m_ledOnOff);
	DDX_Control(pDX, IDC_ENCENDER, m_onoff);
	DDX_Control(pDX, IDC_RESET, m_reset);
	DDX_Control(pDX, IDC_DISPLAY_SALIDAS, m_DispSalidas);
	DDX_Control(pDX, IDC_DISPLAY_ENTRADAS, m_DispEntradas);
	DDX_Control(pDX, IDC_SALIDAS, m_CtrlSalidas);
	DDX_Control(pDX, IDC_ENTRADAS, m_CtrlEntradas);
	//}}AFX_DATA_MAP
}

BOOL CSexShopView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style|=WS_HSCROLL;

	return CFormView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSexShopView diagnostics

#ifdef _DEBUG
void CSexShopView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSexShopView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSexShopDoc* CSexShopView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSexShopDoc)));
	return (CSexShopDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSexShopView message handlers

void CSexShopView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit(FALSE);
	ResizeParentToFit(TRUE);

	for (int n=0;n<m_TotalCanales;n++)
	{
		CString str;
		str.Format ("%d",n+1);
		m_CtrlEntradas.AddString (str);
	}

	m_CtrlEntradas.SetWindowText ("1");
	m_CtrlSalidas.SelectString (0,"1");

	m_DispEntradas.SetText("01"); 
	m_DispEntradas.SetColor( RGB(10,10,10), RGB(0,255,0) );
	m_DispEntradas.SetBackColor( RGB(0,0,0) );
	m_DispEntradas.ModifyDigiStyle(0, CDigistring::DS_SMOOTH);

	m_DispSalidas.SetText("01"); 
	m_DispSalidas.SetColor( RGB(10,10,10), RGB(0,255,0) );
	m_DispSalidas.SetBackColor( RGB(0,0,0) );
	m_DispSalidas.ModifyDigiStyle(0, CDigistring::DS_SMOOTH);


	m_onoff.LoadBitmap(IDB_BITMAP2);
	CString text=_T("Conectar/Desconectar");
	m_onoff.SetToolTipText(&text);
	m_ledOnOff.SetIcon (AfxGetApp()->LoadIcon(IDI_LED_APAGADO));

	m_reset.LoadBitmap(IDB_RESET);
	text=_T("Reiniciar matriz");
	m_reset.SetToolTipText(&text);

	UpdateData (FALSE);
	
	SetTimer (1000,100,NULL);
}

void CSexShopView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CFormView::OnPaint() for painting messages
}


void CSexShopView::ShowError(int NumError, int NumCabina)
{
	char buff[80];
	switch (NumError)
	{
		case ERROR_GENERAL:
			sprintf (buff,"Ha ocurrido un error general en el controlador de cabina %d",NumCabina);
			AfxMessageBox (buff,MB_OK|MB_ICONSTOP);
		break;
		case ERROR_COMUNICACIONES:
			sprintf (buff,"Error de comunicacion en direccion %03x",NumCabina);
			AfxMessageBox (buff,MB_OK|MB_ICONSTOP);
		break;
	}
}


void CSexShopView::OnMatrizActivarto() 
{
	CSexShopDoc *pDoc = GetDocument();

	for (int n=0;n<pDoc->m_pCabinas.GetSize();n++)
	{
		pDoc->m_pCabinas[n]->SetStatus(0);
		pDoc->m_pCabinas[n]->SetActiva (TRUE);
		pDoc->m_pCabinas[n]->Activate(TRUE);
		
	}
}


void CSexShopView::OnMatrizDetenertodaslascabinas() 
{
	CSexShopDoc *pDoc = GetDocument();

	for (int n=0;n<m_TotalCabinas;n++)
	{
		pDoc->m_pCabinas[n]->SetActiva (FALSE);
		pDoc->m_pCabinas[n]->SetStatus (0);
		pDoc->m_pCabinas[n]->Activate(FALSE);
	}
}

void CSexShopView::OnMatrizReiniciar() 
{
	CSexShopDoc *pDoc = GetDocument();

	for (int n=0;n<m_TotalCabinas;n++)
	{
		pDoc->m_pCabinas[n]->SetActiva (TRUE);
		pDoc->m_pCabinas[n]->SetActiva (FALSE);
		pDoc->m_pCabinas[n]->SetCanalActual (0);
	}	
}

void CSexShopView::OnEdicinTiempomon() 
{
	char buffer[80];
	CSexShopDoc *pDoc = GetDocument();
	CCabina *pCabina;
	CDia *pDia;
	
	CConfTimeCoin dialog;
	dialog.m_time =pDoc->m_TimeCoin;

	dialog.m_time = AfxGetApp()->GetProfileInt ("mtx15","TiempoMonedas",0);
	
	dialog.m_ComPort = AfxGetApp()->GetProfileString ("mtx15","ComPort","COM1");

	dialog.m_bOcultarCanal = AfxGetApp()->GetProfileInt ("mtx15","OcultaCanal",0);
	dialog.m_ActuacionTeclas= AfxGetApp()->GetProfileInt ("mtx15","ActuacionTeclas",0);
	dialog.m_bSaltoCanal= AfxGetApp()->GetProfileInt ("mtx15","SaltoCanal",0);
	dialog.m_SegundosCredito= AfxGetApp()->GetProfileInt ("mtx15","SegundosCredito",6);

	int moneda;

	dialog.m_TotalCanales = AfxGetApp()->GetProfileInt ("mtx15","TotalCanales",4);
	dialog.m_TotalCabinas = AfxGetApp()->GetProfileInt ("mtx15","TotalCabinas",1);


	if (dialog.DoModal()==IDOK)
	{

		pDoc->m_TimeCoin = dialog.m_time;

		AfxGetApp()->WriteProfileInt ("mtx15","TiempoMonedas",pDoc->m_TimeCoin);
		AfxGetApp()->WriteProfileInt ("mtx15","OcultaCanal",dialog.m_bOcultarCanal);
		AfxGetApp()->WriteProfileInt ("mtx15","ActuacionTeclas",dialog.m_ActuacionTeclas);
		AfxGetApp()->WriteProfileInt ("mtx15","SaltoCanal",dialog.m_bSaltoCanal);
		AfxGetApp()->WriteProfileInt ("mtx15","TotalCanales",dialog.m_TotalCanales);
		AfxGetApp()->WriteProfileInt ("mtx15","TotalCabinas",dialog.m_TotalCabinas);
		AfxGetApp()->WriteProfileInt ("mtx15","SegundosCreDito",dialog.m_SegundosCredito);
		AfxGetApp()->WriteProfileString ("mtx15","ComPort",dialog.m_ComPort);

		CCabina *pCabina;
		for (int n=0;n<m_TotalCabinas;n++)
		{
			pDoc->m_pCabinas[n]->SetCanalInicial (dialog.m_time);
			pDoc->m_pCabinas[n]->SetOcultaCanal (dialog.m_bOcultarCanal);
			pDoc->m_pCabinas[n]->SetSegundosCredito (dialog.m_SegundosCredito);
			pDoc->m_pCabinas[n]->SetActuacionTeclas (dialog.m_ActuacionTeclas);
			pDoc->m_pCabinas[n]->SetSaltoCanal (dialog.m_bSaltoCanal);

		}
		GetDocument()->UpdateAllViews(FALSE);
	}
}

void CSexShopView::OnEncender() 
{
	CSexShopDoc *pDoc = GetDocument();


	if (!m_bEncendido)
	{
		pDoc->m_bStop=FALSE;
		pDoc->m_bStartTime=FALSE; 
		pDoc->m_pPullingThread = AfxBeginThread ((AFX_THREADPROC) PollingThread,(LPVOID)pDoc);
		pDoc->m_pPullingTimerThread = AfxBeginThread ((AFX_THREADPROC) PollingTimerThread,(LPVOID)pDoc);

		pDoc->m_pLedsThread = AfxBeginThread ((AFX_THREADPROC) ledsThread,(LPVOID)pDoc);
		OnMatrizActivarto ();
	}
	else
	{
		OnMatrizDetenertodaslascabinas();
		pDoc->m_bStop=TRUE;
		HANDLE hThread[3]= {pDoc->m_pPullingThread->m_hThread,
							pDoc->m_pPullingTimerThread->m_hThread,
							pDoc->m_pLedsThread->m_hThread}; 

		::WaitForMultipleObjects (3,hThread,TRUE,2);
	}
}

void CSexShopView::OnEnviar() 
{
	CSexShopDoc *pDoc = GetDocument();

	UpdateData (TRUE);
	CString str;

	m_CtrlEntradas.GetWindowText (str);

	int entrada = atoi (str);
	int salida= m_CtrlSalidas.GetCurSel();


	if (entrada<1 || entrada>m_TotalCanales)
	{
		str.Format ("El numero de entradas es de 1 - %d",m_TotalCanales);
		AfxMessageBox (str,MB_OK|MB_ICONEXCLAMATION);
	}
	CCabina *pCabina = pDoc->m_pCabinas[salida];

	pCabina->SetCanalActual (entrada-1);


}

void CSexShopView::OnTimer(UINT nIDEvent) 
{
	CFormView::OnTimer(nIDEvent);

	CSexShopDoc *pDoc = GetDocument();
	CCabina *pCabina;
	CString strEntrada,strSalida;

	int salida= m_CtrlSalidas.GetCurSel();

	pCabina=pDoc->m_pCabinas[salida];
	strEntrada.Format ("%02d",pCabina->GetCanalActual()+1);

	m_DispEntradas.SetText(strEntrada); 
}

void CSexShopView::OnReset() 
{
	OnMatrizReiniciar();
}

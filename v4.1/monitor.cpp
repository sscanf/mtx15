// Monitor.cpp : implementation file
//

#include "stdafx.h"
#include "SexShop.h"
#include "Monitor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMonitor dialog


CMonitor::CMonitor(CWnd* pParent /*=NULL*/)
	: CDialog(CMonitor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMonitor)
		m_modo=FALSE;
		m_modo=2;
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMonitor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonitor)
	DDX_Control(pDX, IDC_BUTTON2, m_boton2);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMonitor, CDialog)
	//{{AFX_MSG_MAP(CMonitor)
	ON_MESSAGE (MON_PING,OnRxPing)
	ON_MESSAGE (MON_PONG,OnRxPong)
	ON_MESSAGE (ERROR_CABINA,OnErrorCabina)
	ON_MESSAGE (RX_BYTE,OnRxByte)
	ON_MESSAGE (MON_RESET,OnRxReset)
	ON_MESSAGE (MON_MONEDA,OnMoneda)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonitor message handlers

LRESULT CMonitor::OnRxPing(WPARAM wParam, LPARAM lParam)
{
	if (m_modo==FALSE)
		m_ListBox.InsertString (0,"Ping");
	return TRUE;
}

LRESULT CMonitor::OnRxPong(WPARAM wParam, LPARAM lParam)
{
	if (m_modo==FALSE)
		m_ListBox.InsertString (0,"Pong");
	return TRUE;
}

LRESULT CMonitor::OnErrorCabina(WPARAM wParam, LPARAM lParam)
{
	char buff[80];

	if (m_modo==FALSE)
	{
		sprintf (buff,"Warnig Cabina %d ",wParam);
		m_ListBox.InsertString (0,buff);
	}

	return TRUE;
}

LRESULT CMonitor::OnRxByte(WPARAM wParam, LPARAM lParam)
{
	UCHAR buff2[80];
	static UCHAR buff[20];
	static ndx;
	static int linea=0;

	buff[ndx]=(UCHAR)lParam;

	if (m_modo==TRUE)
	{
		for (int n=0;n<ndx;n++)
			sprintf ((char*)buff2+(n*3),"%x     ",buff[n]);

		if (ndx==19)
		{
			m_ListBox.DeleteString (0);
			m_ListBox.InsertString (0,(char*)buff2);
		}

		ndx++;
		if (ndx>=20)
		{
			ndx=0;
			m_ListBox.InsertString (0," ");
		}
	}
	return TRUE;
}

LRESULT CMonitor::OnRxReset(WPARAM wParam, LPARAM lParam)
{
	char buff[80];


	sprintf (buff,"Reset - %d",wParam);

	if (m_modo==FALSE)
		m_ListBox.InsertString (0,buff);
	return TRUE;
}

LRESULT CMonitor::OnMoneda (WPARAM wParam, LPARAM lParam)
{
	char buff[80];

	if (m_modo==FALSE)
	{
		sprintf (buff,"Moneda Cabina %d            TO = %d",wParam,lParam);
		m_ListBox.InsertString (0,buff);
	}

	return TRUE;
}
void CMonitor::OnRadio1() 
{
	m_modo=FALSE;	
}

void CMonitor::OnRadio2() 
{
	m_modo=TRUE;
}

void CMonitor::OnButton2() 
{
	static AntModo;

	if (m_modo!=2)
	{
		AntModo=m_modo;
		m_modo=2;
		m_boton2.SetWindowText ("Empezar");
	}
	else
	{
		m_modo=AntModo;
		m_boton2.SetWindowText ("Parar");
	}
}

void CMonitor::OnTimer(UINT nIDEvent) 
{	
}

BOOL CMonitor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
//	SetTimer (IDT_TIMER2,1000,0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

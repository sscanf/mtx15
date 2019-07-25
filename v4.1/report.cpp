// Report.cpp : implementation file
//

#include "stdafx.h"
#include "SexShop.h"
#include "Report.h"
#include "cabina.h"
#include "canal.h"
#include <winspool.h>
#include "printmanager\ListCtrlPrint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

 
/////////////////////////////////////////////////////////////////////////////
// CReport dialog


CReport::CReport(CWnd* pParent /*=NULL*/)
	: CDialog(CReport::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReport)
	m_TotalPts = 0;
	m_TotalPtsTarjetas = 0;
	//}}AFX_DATA_INIT

	m_bAcumulado=FALSE;
}

CReport::~CReport()
{

}


void CReport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReport)
	DDX_Control(pDX, IDC_CANALES, m_canales);
	DDX_Control(pDX, IDC_CABINAS, m_cabinas);
	DDX_Text(pDX, IDC_EDIT1, m_TotalPts);
	DDX_Text(pDX, IDC_TOTAL_TARJETAS, m_TotalPtsTarjetas);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReport, CDialog)
	//{{AFX_MSG_MAP(CReport)
	ON_BN_CLICKED(IDC_IMPRIMIR, OnImprimir)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_IMPRIMIR_CANALES, OnImprimirCanales)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReport message handlers

BOOL CReport::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CCabina *pCab;
	CCanal *pCan;
	int val;
	char str[80];
	CString strTime;
	CTimeSpan time,TotalTime,tTime;
	char buffer[10];
	CString strVal;
	char TotalCabinas = AfxGetApp()->GetProfileInt ("Sexshop","TotalCabinas",15);	
	char TotalCanales = AfxGetApp()->GetProfileInt ("Sexshop","TotalCanales",64);

	CWnd *pWnd;
	pWnd = new CWnd ();

	static _TCHAR *_gszColumnLabel[NUM_COLUMNS-1] = 
	{
		_T("Número"), _T("Importe M"),_T("Importe T"),_T("Acumulado")
	};

	static int _gnColumnFmt[NUM_COLUMNS-1] = 
	{
		LVCFMT_RIGHT, 
		LVCFMT_RIGHT,
		LVCFMT_RIGHT,
		LVCFMT_CENTER, 
		LVCFMT_CENTER
	};

	static int _gnColumnWitdh[NUM_COLUMNS-1] =
	{
		50,90,90,80,110
	};
	
	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for (int n=0;n<NUM_COLUMNS-1;n++)
	{
		lvc.iSubItem=n;
		lvc.pszText = _gszColumnLabel[n];
		lvc.cx = _gnColumnWitdh[n];
		lvc.fmt= _gnColumnFmt[n];
		m_cabinas.InsertColumn(n,&lvc);
	}
		
	lvc.iSubItem=0;
	lvc.pszText = "Nº Canal";
	lvc.cx = 60;
	lvc.fmt= LVCFMT_LEFT;
	m_canales.InsertColumn(0,&lvc);

	lvc.iSubItem=1;
	lvc.pszText = "Total tiempo";
	lvc.cx = 120;
	lvc.fmt= LVCFMT_LEFT;
	m_canales.InsertColumn(1,&lvc);

	POSITION pos;

	if (!m_bAcumulado)
	{
		pos=m_pDia->m_pCabinas.GetHeadPosition();

		m_cabinas.DeleteAllItems();
		
		for (n=0;n<m_pDia->m_TotalCabinas;n++)
		{
			char str[80];

			pCab=m_pDia->m_pCabinas.GetNext(pos);
			LV_ITEM lvi;

			itoa (n+1,str,10);
			lvi.mask = LVIF_TEXT | LVIF_STATE;
			lvi.iItem = n;
			lvi.iSubItem=0;
			lvi.iImage=0;
			lvi.pszText = str;
			lvi.state=INDEXTOSTATEIMAGEMASK(1);
			m_cabinas.InsertItem (&lvi);

			strVal.Format ("%0d",pCab->GetPts());
			m_TotalPts+=pCab->GetPts();
			m_TotalPtsTarjetas+=pCab->GetPtsTarjeta();
			lvi.iItem = n;
			lvi.iSubItem=1;
			lvi.pszText = strVal.GetBuffer(10);
			strVal.ReleaseBuffer();
			m_cabinas.SetItem (&lvi);

			strVal.Format ("%0d",pCab->GetPtsTarjeta());
			m_TotalPts+=pCab->GetPts();
			m_TotalPtsTarjetas+=pCab->GetPtsTarjeta();
			lvi.iItem = n;
			lvi.iSubItem=2;
			lvi.pszText = strVal.GetBuffer(10);
			strVal.ReleaseBuffer();
			m_cabinas.SetItem (&lvi);


			time=pCab->GetTotalTiempo();
			strTime = time.Format ("%H:%M.%S");
			itoa (val,str,10);
			lvi.iItem = n;
			lvi.iSubItem=3;
			lvi.pszText = strTime.GetBuffer(1);
			m_cabinas.SetItem (&lvi);
			strTime.ReleaseBuffer();
		}

		pos=m_pDia->m_pCanales.GetHeadPosition();

		for (n=0;n<m_pDia->m_TotalCanales;n++)
		{
			pCan=m_pDia->m_pCanales.GetNext(pos);
			LV_ITEM lvi;

			itoa (n+1,str,10);
			lvi.mask = LVIF_TEXT | LVIF_STATE;
			lvi.iItem = n;
			lvi.iSubItem=0;
			lvi.iImage=0;
			lvi.pszText = str;
			lvi.state=INDEXTOSTATEIMAGEMASK(1);
			m_canales.InsertItem (&lvi);

			time=pCan->GetTime();

			strTime = time.Format ("%H:%M.%S");
			itoa (val,str,10);
			lvi.iItem = n;
			lvi.iSubItem=1;
			lvi.pszText = strTime.GetBuffer(1);
			m_canales.SetItem(&lvi);
			strTime.ReleaseBuffer();		
		}

		sprintf (str,"%d/%d/%d",m_pDia->m_fecha.GetDay(),
			m_pDia->m_fecha.GetMonth(),m_pDia->m_fecha.GetYear());

		SetWindowText (str);
	}
	else
	{
		for (n=0;n<TotalCabinas;n++)
		{
			CCabina *pnewCab = new CCabina;
			m_newCabinas.Add (pnewCab);
		}

		for (n=0;n<TotalCanales;n++)
		{
			CCanal *pnewCanal = new CCanal (n);
			m_newCanales.Add (pnewCanal);
		}
		CDia *pDia;

		POSITION pos,posCabinas,posCanales;

		int count = m_pDias->GetCount();
		pos = m_pDias->GetHeadPosition();

		for (int i=0;i<count;i++)
		{
			m_pDia = m_pDias->GetNext (pos);;
			posCabinas=m_pDia->m_pCabinas.GetHeadPosition();	
			
			for (n=0;n<m_pDia->m_TotalCabinas;n++)
			{
				pCab=m_pDia->m_pCabinas.GetNext(posCabinas);

				m_newCabinas.GetAt(n)->SetPts (m_newCabinas.GetAt(n)->GetPts()+pCab->GetPts());
				m_newCabinas.GetAt(n)->SetPtsTarjeta(m_newCabinas.GetAt(n)->GetPtsTarjeta()+pCab->GetPtsTarjeta());		
				m_newCabinas.GetAt(n)->SetTotalTiempo (m_newCabinas.GetAt(n)->GetTotalTiempo()+pCab->GetTotalTiempo());
			}

			posCanales=m_pDia->m_pCanales.GetHeadPosition();
			for (n=0;n<m_pDia->m_TotalCanales;n++)
			{
				pCan=m_pDia->m_pCanales.GetNext(posCanales);
				m_newCanales.GetAt(n)->SetTime (m_newCanales.GetAt(n)->GetTime()+pCan->GetTime());
			}

		}

		for (n=0;n<m_newCabinas.GetSize();n++)
		{
			char str[80];

			pCab=m_newCabinas.GetAt (n);
			LV_ITEM lvi;

			itoa (n+1,str,10);
			lvi.mask = LVIF_TEXT | LVIF_STATE;
			lvi.iItem = n;
			lvi.iSubItem=0;
			lvi.iImage=0;
			lvi.pszText = str;
			lvi.state=INDEXTOSTATEIMAGEMASK(1);
			m_cabinas.InsertItem (&lvi);

			strVal.Format ("%0d",pCab->GetPts());
			m_TotalPts+=pCab->GetPts();
			m_TotalPtsTarjetas+=pCab->GetPtsTarjeta();
			lvi.iItem = n;
			lvi.iSubItem=1;
			lvi.pszText = strVal.GetBuffer(10);
			strVal.ReleaseBuffer();
			m_cabinas.SetItem (&lvi);

			strVal.Format ("%0d",pCab->GetPtsTarjeta());
			m_TotalPts+=pCab->GetPts();
			m_TotalPtsTarjetas+=pCab->GetPtsTarjeta();
			lvi.iItem = n;
			lvi.iSubItem=2;
			lvi.pszText = strVal.GetBuffer(10);
			strVal.ReleaseBuffer();
			m_cabinas.SetItem (&lvi);

			time=pCab->GetTotalTiempo();
			strTime = time.Format ("%H:%M.%S");
			itoa (val,str,10);
			lvi.iItem = n;
			lvi.iSubItem=3;
			lvi.pszText = strTime.GetBuffer(1);
			m_cabinas.SetItem (&lvi);
			strTime.ReleaseBuffer();
		}

		pos=m_pDia->m_pCanales.GetHeadPosition();

		for (n=0;n<m_pDia->m_TotalCanales;n++)
		{
			pCan=m_newCanales.GetAt (n);
			LV_ITEM lvi;

			itoa (n+1,str,10);
			lvi.mask = LVIF_TEXT | LVIF_STATE;
			lvi.iItem = n;
			lvi.iSubItem=0;
			lvi.iImage=0;
			lvi.pszText = str;
			lvi.state=INDEXTOSTATEIMAGEMASK(1);
			m_canales.InsertItem (&lvi);

			time=pCan->GetTime();

			strTime = time.Format ("%H:%M.%S");
			itoa (val,str,10);
			lvi.iItem = n;
			lvi.iSubItem=1;
			lvi.pszText = strTime.GetBuffer(1);
			m_canales.SetItem(&lvi);
			strTime.ReleaseBuffer();		
		}

	}

	UpdateData (FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CReport::OnImprimir() 
{
	CCabina *pCab;
	CCanal *pCanal;
	CListCtrlPrint cJob;
	CString header,footer;
	int TotalPts=0;
	int TotalPtsTarjetas=0;

	header.Format ("Listado de Cabinas\r\nfecha %d/%d/%d",m_pDia->m_fecha.GetDay(),
		m_pDia->m_fecha.GetMonth(),m_pDia->m_fecha.GetYear());


	POSITION pos=m_pDia->m_pCabinas.GetHeadPosition();
	for (int n=0;n<m_pDia->m_TotalCabinas;n++)
	{
		pCab=m_pDia->m_pCabinas.GetNext(pos);	
		TotalPts+=pCab->GetPts();
		TotalPtsTarjetas+=pCab->GetPtsTarjeta();
	}

	footer.Format ("Total importe monedas = %d   Total importe tarjetas",TotalPts, TotalPtsTarjetas);

	cJob.csPageHead = header;
	cJob.csPageFooter = footer;
	cJob.csPage = "Página %d de %d";
	cJob.pList = &m_cabinas;
	cJob.OnFilePrint();
}



void CReport::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
}


void CReport::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
}


void CReport::OnImprimirCanales() 
{
	CCanal *pCan;
	CString footer,header;
	CListCtrlPrint cJob;

	header.Format ("Listado de Canales\r\nfecha %d/%d/%d",m_pDia->m_fecha.GetDay(),
		m_pDia->m_fecha.GetMonth(),m_pDia->m_fecha.GetYear());

	CTimeSpan TotalTime (0,0,0,0);

	POSITION pos=m_pDia->m_pCabinas.GetHeadPosition();
	for (int n=0;n<m_pDia->m_TotalCabinas;n++)
	{
		pCan=m_pDia->m_pCanales.GetNext(pos);	
		TotalTime+=pCan->GetTime();
	}

	footer.Format ("Total tiempo = %d:%d.%d",TotalTime.GetHours(),
		TotalTime.GetMinutes(),TotalTime.GetSeconds());

	cJob.csPageHead = header;
	cJob.csPageFooter = footer;
	cJob.csPage = "Página %d de %d";
	cJob.pList = &m_canales;
	cJob.OnFilePrint();
	
}

void CReport::OnClose() 
{
	CDialog::OnClose();
}

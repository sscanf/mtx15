// SexShopDoc.cpp : implementation of the CSexShopDoc class
//

#include "stdafx.h"
#include "SexShop.h"
#include "SexShopView.h"
#include "DialogClrChan.h"
#include "DialogClrCab.h"
#include "Comm.h"	// Added by ClassView
#include "SexShopDoc.h"
#include "fcs.h"
#include "buffer.h"
#include "splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT PollingTimerThread (LPVOID pParam)
{
	CSexShopDoc *pDoc = (CSexShopDoc *) pParam;  // Get serial device
	while (pDoc->m_bStop==FALSE)
	{
		while (pDoc->m_bStartTime==FALSE && pDoc->m_bStop==FALSE)
			Sleep(10);

		Sleep(100);

		pDoc->m_bStartTime=FALSE;
	}
	return 0;
}

UINT ledsThread(LPVOID pParam)
{
	CSexShopDoc *pDoc = (CSexShopDoc *) pParam;  // Get serial device
	POSITION pos;
	pos = pDoc->GetFirstViewPosition();
	CSexShopView* pView = (CSexShopView *)pDoc->GetNextView(pos);

	while (pDoc->m_bStop==FALSE)
	{
		if (pDoc->m_TxThread)
		{
			pView->m_ledTX.SetIcon (AfxGetApp()->LoadIcon(IDI_LED_ENCENDIDO_VERDE1));
			Sleep (50);
			pView->m_ledTX.SetIcon (AfxGetApp()->LoadIcon(IDI_LED_APAGADO_VERDE));
			pDoc->m_TxThread=FALSE;
		}

		if (pDoc->m_RxThread)
		{
			pView->m_ledRX.SetIcon (AfxGetApp()->LoadIcon(IDI_LED_ENCENDIDO_VERDE1));
			Sleep (50);
			pView->m_ledRX.SetIcon (AfxGetApp()->LoadIcon(IDI_LED_APAGADO_VERDE));
			pDoc->m_RxThread=FALSE;
		}
		Sleep(100);
	}
	return 0;
}

UINT PollingThread (LPVOID pParam)
{
	CEvent event(FALSE, TRUE);
	CSexShopDoc *pDoc = (CSexShopDoc *) pParam;  // Get serial device
	CArray <CCabina *, CCabina *> hPriority;
	CArray <CCabina *, CCabina *> lPriority;
	CCabina *pCabina;
	int ret;
	BOOL bExit=FALSE;

	char TotalCabinas = (AfxGetApp()->GetProfileInt ("mtx15","TotalCabinas",4))+1;
	char cabina=0;
	POSITION pos;
	pos = pDoc->GetFirstViewPosition();
	CSexShopView* pView = (CSexShopView *)pDoc->GetNextView(pos);

	//Buscamos el último dia (o sea, hoy).

	for (int n=0;n<TotalCabinas;n++)
		hPriority.Add (pDoc->m_pCabinas[n]);

	pDoc->m_bStartTime=TRUE;

	pView->m_ledOnOff.SetIcon (AfxGetApp()->LoadIcon(IDI_LED_ENCENDIDO));
	pView->m_bEncendido=TRUE;

	while (pDoc->m_bStop==FALSE || bExit==FALSE)
	{
		BOOL bFlag=TRUE;

		for (cabina=0;cabina<hPriority.GetSize();cabina++)
		{
			bFlag=FALSE;
	
			pCabina = hPriority.GetAt (cabina);
			ret = pDoc->DoCabina (pCabina);

			if (!ret)
			{
				int IddleTime=pCabina->GetIddleTime();
				if (IddleTime>2)
				{
					hPriority.RemoveAt (cabina);
					lPriority.Add (pCabina);
				}
			}
			else
				pDoc->m_bStop=TRUE;
		}

		if (pDoc->m_bStartTime==FALSE)
		{
			for (cabina=0;cabina<lPriority.GetSize();cabina++)
			{
				bFlag=FALSE;
				pCabina = lPriority.GetAt (cabina);
				pDoc->DoCabina (pCabina);

				if (!ret)
				{
					int IddleTime=pCabina->GetIddleTime();
					if (IddleTime<2)
					{
						lPriority.RemoveAt (cabina);
						hPriority.Add (pCabina);
					}
				}
				else
					pDoc->m_bStop=TRUE;
			}
			pDoc->m_bStartTime=TRUE;
		}

		if (bFlag==TRUE)
			Sleep (10);

		bExit=FALSE;

		int count=0;
		for (cabina=0;cabina<hPriority.GetSize();cabina++)
		{
			pCabina = hPriority.GetAt (cabina);
			count+=pCabina->m_mensajes.m_MesajeCue.GetSize();
		}

		if (!count)
			bExit=TRUE;

	}

	pView->m_ledOnOff.SetIcon (AfxGetApp()->LoadIcon(IDI_LED_APAGADO));
	pView->m_bEncendido=FALSE;

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CSexShopDoc

IMPLEMENT_DYNCREATE(CSexShopDoc, CDocument)

BEGIN_MESSAGE_MAP(CSexShopDoc, CDocument)
	//{{AFX_MSG_MAP(CSexShopDoc)
	ON_COMMAND(ID_FILE_ERASE, OnFileErase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSexShopDoc construction/destruction

CSexShopDoc::CSexShopDoc()
{
	m_timer=0;
	m_bStop=FALSE;
	m_TxThread = FALSE;
	m_RxThread = FALSE;
	
	m_pPullingThread=NULL;
	m_pPullingTimerThread=NULL;
	m_pLedsThread=NULL;
}

CSexShopDoc::~CSexShopDoc()
{
}

BOOL CSexShopDoc::OnNewDocument()
{
	int ret;
//	SetTitle ("MTX15 Control");
	m_TotalCabinas = AfxGetApp()->GetProfileInt ("mtx15","TotalCabinas",4)+1;
	UINT TotalCanales = (AfxGetApp()->GetProfileInt ("mtx15","TotalCanales",64)+1)*64;
	m_TipoMoneda = ID_MONEDA_EUROS;

	for (int n=0;n<m_TotalCabinas;n++)
	{
		CCabina *pCabina = new CCabina(n);
		m_pCabinas.Add (pCabina);
		pCabina->SetSaltoCanal (AfxGetApp()->GetProfileInt ("mtx15","SaltoCanal",0));
		pCabina->SetOcultaCanal(AfxGetApp()->GetProfileInt ("mtx15","OcultaCanal",0));
		pCabina->SetActuacionTeclas (AfxGetApp()->GetProfileInt ("mtx15","OcultaCanal",0));
		pCabina->SetSegundosCredito (AfxGetApp()->GetProfileInt ("mtx15","SegundosCredito",6));
		pCabina->SetTiempo (AfxGetApp()->GetProfileInt ("mtx15","TiempoMonedas",0));
	}

//	DeleteContents();
	do
	{
		CString ComPort = AfxGetApp()->GetProfileString ("mtx15","ComPort","COM1");

		//
		int port;
		if (ComPort == "COM1") port =1;
		if (ComPort == "COM2") port =2;
		if (ComPort == "COM3") port =3;
		if (ComPort == "COM4") port =4;
		if (ComPort == "COM5") port =5;
		if (ComPort == "COM6") port =6;
		if (ComPort == "COM7") port =7;
		if (ComPort == "COM8") port =8;
		if (ComPort == "COM9") port =9;

		ret= m_serial.Open (port,9600);
		if (!ret)
		{
			AfxMessageBox ("No puedo abrir el puerto de comunicaciones");
			POSITION pos;
			pos = GetFirstViewPosition();
			CSexShopView* pView = (CSexShopView *)GetNextView(pos);
			pView->OnInitialUpdate();
			pView->SendMessage (WM_COMMAND,ID_EDICIN_TIEMPOMON,0);
		}
	}while (!ret);

	COMMTIMEOUTS tm;

	m_serial.GetTimeouts(tm);

	tm.ReadIntervalTimeout = MAXDWORD;
	tm.ReadTotalTimeoutMultiplier = 0;
	tm.ReadTotalTimeoutConstant = 200;
	tm.WriteTotalTimeoutMultiplier = 0;
	tm.WriteTotalTimeoutConstant = 0;

	m_serial.SetTimeouts (tm);
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSexShopDoc serialization

void CSexShopDoc::Serialize(CArchive& ar)
{
	POSITION pos;

	if (ar.IsStoring())
	{

	}
	else
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSexShopDoc diagnostics

#ifdef _DEBUG
void CSexShopDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSexShopDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSexShopDoc commands

BOOL CSexShopDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	
//	UpdateAllViews (NULL,0);
	return CDocument::OnSaveDocument(lpszPathName);
}

int CSexShopDoc::DoCabina (CCabina *pCabina)
{
	static CBuffer m_RxBuffer(255);
	CFcs fcs;
	POSITION pos,pos2;
	
	pos = GetFirstViewPosition();
	CSexShopView* pView = (CSexShopView *)GetNextView(pos);

	UCHAR buff[20];
	UCHAR RxBuff[20];
	static char start=0, antByte=0;
	char *pBuffer;
	int n,bufLen;
	UCHAR crc,RxCrc;
	BOOL bFlag;
	DWORD dwRead=0;
    DWORD dwMask;
	
	long count;

	int cabina = pCabina->GetNumCabina();

	int error=FALSE;

	buff[0]=0xec;
	buff[1]=0xa1;
	buff[2]=cabina;
	
	count = pCabina->m_mensajes.m_MesajeCue.GetSize();
		
	if (!count)
	{
		pCabina->MsgStatus();
		count = pCabina->m_mensajes.m_MesajeCue.GetSize();
	}

	if (count)
	{
		bFlag=TRUE;
		CBuffer *pBuffer = pCabina->m_mensajes.m_MesajeCue.GetAt (0);

		count = pBuffer->GetLen ();
		pBuffer->get (buff+3,count);

		int crc = fcs.Calc8 ((unsigned char *)buff+2,count+1);

		buff[count+3]=MSG_EOM;	//En of message 0xff
		buff[count+4]=crc;

		m_TxThread=TRUE;
		m_serial.Write(buff, count+5);
		bufLen=0;

		do
		{
			dwRead=m_serial.Read(RxBuff, 1);

			if (dwRead)
			{
				m_RxThread=TRUE;
				if (start)
				{
					m_RxBuffer.put (RxBuff,dwRead);
					bufLen = m_RxBuffer.GetLen();

					if (bufLen==4)
					{
						RxCrc = m_RxBuffer[bufLen-1];
						crc = fcs.Calc8 (m_RxBuffer.GetBuff(),bufLen-1);
						m_RxBuffer.ReleaseBuff();
						start=0;

						if (crc==RxCrc)
						{
							pCabina->ClearErrors();

							unsigned char cabina = m_RxBuffer.get();
							unsigned char status1 = m_RxBuffer.get();
							unsigned char status2 = m_RxBuffer.get();
							
							if (pView)
								RxStatus (cabina,status1,status2);
						}
						else
						{
							pCabina->SetFault(TRUE);
						}
					}
				}

				if ((unsigned char)*RxBuff==0xa1 && (unsigned char)antByte == 0xec)	//Begin of message
				{			
					m_RxBuffer.Clear();
					start = 1;
				}
				antByte=*RxBuff;
			}
			else
			{
				pCabina->AddError();
				if (pCabina->GetErrors()>4)
				{
					pCabina->ClearErrors();
					pCabina->SetFault(TRUE);
					pView->ShowError (ERROR_COMUNICACIONES,pCabina->GetNumCabina());
					error=TRUE;
				}	
				
				break;
			}
		}while (bufLen<4);
		pCabina->m_mensajes.RemoveMsg (0);
	}
	return error;
}

void CSexShopDoc::RxStatus(BYTE cabina, BYTE status1, BYTE status2)
{
	static int ndx;

	POSITION pos;
	pos = GetFirstViewPosition();
    CSexShopView* pView = (CSexShopView *)GetNextView(pos);

   if (cabina==7)
   {
	   TRACE1 ("%d ",ndx++);
		TRACE3 (" %d %x %x\n",cabina,status1,status2);
   }

	if (cabina>m_TotalCabinas)
		return;

	if (cabina&0x40)
	{
		status1=0xff;
		status2=0xff;
	}

	m_pCabinas[cabina]->DoMsg (status1,status2);

}


BOOL CSexShopDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	m_TimeCoin = AfxGetApp()->GetProfileInt ("mtx15","TiempoMonedas",0);

	return TRUE;
}

void CSexShopDoc::DeleteContents() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDocument::DeleteContents();
}

BOOL CSexShopDoc::CanClose()
{
	m_bStop=TRUE;	

	if (m_pPullingThread && m_pPullingTimerThread && m_pLedsThread)
	{
		HANDLE hThread[3]= {m_pPullingThread->m_hThread,
							m_pPullingTimerThread->m_hThread,
							m_pLedsThread->m_hThread}; 

		::WaitForMultipleObjects (3,hThread,TRUE,1000);
	}

	return FALSE;
}

BOOL CSexShopDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	// TODO: Add your specialized code here and/or call the base class

	if (CanClose()) 
		return FALSE;

	return CDocument::CanCloseFrame(pFrame);
}

void CSexShopDoc::OnFileErase() 
{
	UINT TotalCabinas = AfxGetApp()->GetProfileInt ("mtx15","TotalCabinas",15)+1;
	UINT TotalCanales = (AfxGetApp()->GetProfileInt ("mtx15","TotalCanales",64)+1)*64;

	if (AfxMessageBox ("¿Borrar el archivo historial?",MB_OKCANCEL|MB_ICONSTOP)==IDOK)
	{
		DeleteContents();
//		m_pDias.AddTail (new CDia(TotalCabinas, TotalCanales));

		OnNewDocument();
//		UpdateAllViews(NULL,NULL);
	}

}


void CSexShopDoc::SetTitle(LPCTSTR lpszTitle) 
{
	// TODO: Add your specialized code here and/or call the base class
	
//	lpszTitle = "MTX-15 Control";
//	CDocument::SetTitle(lpszTitle);
}


BOOL CSexShopDoc::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	return CDocument::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

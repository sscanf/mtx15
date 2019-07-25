// Comm.cpp: implementation of the CComm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SexShop.h"
#include "SexShopDoc.h"
#include "Comm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define WM_RXCHAR (WM_USER+80)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
UINT monitor (LPVOID param)
{
	HANDLE	hCom;
	HANDLE	hEvent;
	DWORD	dwEvtMask;
	BOOL	fReadStat;
	DWORD	BytesLeidos;

	unsigned char BufferIn[80];
	unsigned char buffer[80];
	int cnt=0;
	int idx=0;

	CComm *pCom = (CComm *)param;
	hCom = pCom->hCom;

	hEvent = CreateEvent (NULL,FALSE,FALSE,NULL);
	SetCommMask(hCom,EV_RXCHAR);

	while (1)
	{
		WaitCommEvent(hCom, &dwEvtMask,NULL);
		if (dwEvtMask & EV_RXCHAR)
		{
			do
			{
				ReadFile (hCom,BufferIn,20,&BytesLeidos,NULL);
				if (BytesLeidos>0)
					::SendMessage (pCom->m_hWnd, WM_RXCHAR, (DWORD)BytesLeidos, (long)BufferIn);

			}while (BytesLeidos>0);


		}
	}
	return 0;
}

CComm::CComm()
{

}

CComm::~CComm()
{

}

CComm::CComm (CString puerto, HWND hWnd)
{
	memset (m_message,NULL,80);
	abrir (puerto, hWnd);
}


CComm::abrir (CString puerto, HWND hWnd)
{
	DCB dcb;
	BOOL fSuccess;
	COMMTIMEOUTS CommTimeOuts;

//	m_pDoc = ptr;
	m_hWnd = hWnd;

	hCom = CreateFile(puerto,
		GENERIC_READ | GENERIC_WRITE,
		0,    /* comm devices must be opened w/exclusive-access */
		NULL, /* no security attrs */
		OPEN_EXISTING, /* comm devices must use OPEN_EXISTING */
		NULL,    /* not overlapped I/O */
		NULL  /* hTemplate must be NULL for comm devices */
		);

	if (hCom == INVALID_HANDLE_VALUE) return FALSE;

	fSuccess = GetCommState(hCom, &dcb);

	if (!fSuccess) 
		return FALSE;

	/* Fill in the DCB: baud=9600, 8 data bits, no parity, 1 stop bit. */

	dcb.DCBlength    = sizeof(dcb);
	dcb.BaudRate	 = 9600;
	dcb.ByteSize	 = 8;
	dcb.Parity		 = NOPARITY;
	dcb.StopBits	 = ONESTOPBIT;
//	dcb.fBinary		 = TRUE;
//	dcb.fNull		 = FALSE;
//	dcb.fRtsControl  = RTS_CONTROL_DISABLE;
//	dcb.fDtrControl  = DTR_CONTROL_DISABLE;
//	dcb.fOutxCtsFlow = FALSE;
//	dcb.fOutxDsrFlow = FALSE;
//	dcb.fDtrControl  = FALSE;
//	dcb.fOutX		 = FALSE;
//	dcb.fInX		 = FALSE;



	fSuccess = SetCommState(hCom, &dcb);
	if (!fSuccess) 	return FALSE;

	SetupComm (hCom,512,512);
    CommTimeOuts.ReadIntervalTimeout = MAXDWORD;
    CommTimeOuts.ReadTotalTimeoutMultiplier = 0 ;
    CommTimeOuts.ReadTotalTimeoutConstant = 0 ;
    SetCommTimeouts(hCom, &CommTimeOuts ) ;

	AfxBeginThread (&monitor, this);
	return TRUE;
}

void CComm::CommRx(unsigned char *msg, DWORD len)
{
//	DWORD idx=0;
	MsgOk=TRUE;
	int idx=0,n;
	static int flag=0;
	static int head=0;
	static int tail=0;

	if (head>=tail)
	{
		for (n=0;n<len;n++)
		{
			m_message[head]=msg[n];
			head++;
			if (head==80) 
				head=0;
			
		}
		flag=0;	
	}

	do
	{
		if (m_message[tail]==0xa1 && m_message[tail-1]==0xec)
		{
			flag=1;
			idx=tail+3;
		}

		if (flag && tail==idx)
		{
			flag=0;
			WPARAM wParam;
			LPARAM lParam;
			
			wParam = m_message[tail-2];
			wParam<<=16;
			wParam|=m_message[tail-1];
			lParam = m_message[tail];
			::PostMessage (m_hWnd, RX_BYTE, NULL, wParam>>16);
			::PostMessage (m_hWnd, RX_BYTE, NULL, LOBYTE(wParam));
			::PostMessage (m_hWnd, RX_BYTE, NULL, (UCHAR)lParam);
			::PostMessage (m_hWnd, RX_STATUS, wParam, lParam);

		}

		tail++;
		if (tail==80) tail=0;

	}while (tail!=head);
}

int CComm::CommTx(char * msg, DWORD largo)
{
	DWORD bytes;

	WriteFile (hCom,msg,largo,&bytes,NULL);
	return (bytes);
}

// Cabina.cpp: implementation of the CCabina class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SexShop.h"
#include "Cabina.h"
#include "fcs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void OcultarThread (LPVOID pParam)
{

	CCabina *pCabina = (CCabina *) pParam;  // Get serial device

	pCabina->m_bStop=FALSE;

	while (!pCabina->m_bStop)
	{
		if (pCabina->GetStatus()&0x80)
		{
			Sleep (1000);
			if (pCabina->m_TimerShow)
				pCabina->m_TimerShow--;
		}
		else
			Sleep (100);
	}
}


void IddleTimerThread (LPVOID pParam)
{
	int time;
	CCabina *pCabina = (CCabina *) pParam;  // Get serial device

	pCabina->m_bStop=FALSE;
	while (!pCabina->m_bStop)
	{
		Sleep (1000);
		time=pCabina->GetIddleTime ()+1;
		pCabina->SetIddleTime (time);
	}
}

void TimerThread(LPVOID pParam)
{
	CCabina *pCabina = (CCabina *) pParam;  // Get serial device

	pCabina->m_bTimerStop=FALSE;
	while (!pCabina->m_bTimerStop)
	{
		if (pCabina->GetTimerTick())
		{
			Sleep (pCabina->GetTimerTick()/pCabina->GetValorCredito1());
			pCabina->GetTimerTick();
		}
		else
			Sleep (200);
		pCabina->Timer();
	}
}

void AntiRebThrd (LPVOID pParam)
{
	CCabina *pCabina = (CCabina *) pParam;  // Get serial device
	pCabina->m_bCanalesStop=FALSE;

	while (!pCabina->m_bCanalesStop)
	{
		pCabina->m_antiRebTrgr=FALSE;

		while (!pCabina->m_antiRebTrgr && !pCabina->m_bCanalesStop)
			Sleep (10);

		pCabina->m_antiReb=FALSE;
		Sleep (pCabina->m_TiempoAntirreb);

		pCabina->m_antiReb=TRUE;
	}
}

void AntiRebThrdRetro (LPVOID pParam)
{
	CCabina *pCabina = (CCabina *) pParam;  // Get serial device
	pCabina->m_bCanalesStop=FALSE;

	while (!pCabina->m_bCanalesStop)
	{
		pCabina->m_antiRebTrgrRetro=FALSE;

		while (!pCabina->m_antiRebTrgrRetro && !pCabina->m_bCanalesStop)
			Sleep (10);

		pCabina->m_antiReb=FALSE;
		Sleep (pCabina->m_TiempoAntirrebRetro);

		pCabina->m_antiRebRetro=TRUE;
	}
}


void CanalesMasTimer (LPVOID pParam)
{
	int n;
	CCabina *pCabina = (CCabina *) pParam;  // Get serial device
	pCabina->m_bCanalesStop=FALSE;

	while (!pCabina->m_bCanalesStop)
	{
		pCabina->m_TimerCanalesMasTrigger=FALSE;
		pCabina->m_TimerCanalesMas=FALSE;

		while (!pCabina->m_TimerCanalesMasTrigger && !pCabina->m_bCanalesStop)
			Sleep (100);

		for (n=0;n<110 && pCabina->m_TimerCanalesMasTrigger;n++)
			Sleep(1);

		pCabina->m_TimerCanalesMasTrigger=FALSE;
		pCabina->m_TimerCanalesMas=FALSE;
	}
}

void CanalesMenosTimer (LPVOID pParam)
{
	int n;
	CCabina *pCabina = (CCabina *) pParam;  // Get serial device
	pCabina->m_bCanalesStop=FALSE;

	while (!pCabina->m_bCanalesStop)
	{
		pCabina->m_TimerCanalesMenosTrigger=FALSE;
		pCabina->m_TimerCanalesMenos=FALSE;

		while (!pCabina->m_TimerCanalesMenosTrigger && !pCabina->m_bCanalesStop)
			Sleep (100);

		for (n=0;n<110 && pCabina->m_TimerCanalesMenosTrigger;n++)
			Sleep(1);

		pCabina->m_TimerCanalesMenosTrigger=FALSE;
		pCabina->m_TimerCanalesMenos=FALSE;
	}
}

void MuteThread(LPVOID pParam)
{
	CCabina *pCabina = (CCabina *) pParam;  // Get serial device

	pCabina->m_bMuteError = TRUE;
	Sleep (10);
	pCabina->m_bMuteError = FALSE;

}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CCabina, CObject, 1)
CCabina::CCabina()
{
	Init();
}

CCabina::CCabina (int nCabina)
{

	Init();
	m_numCabina=nCabina;
}

void CCabina::Init()
{
	m_TiempoAntirreb=100;
	m_TiempoAntirrebRetro=100;
	m_bActivated=FALSE;
	m_flag=FALSE;
	m_estado=FALSE;
	m_pesetas=0;
	m_PesetasTarjeta=0;
	m_credito=0;
	m_tiempo=CTimeSpan (0,0,0,0);
	m_TotalTiempo=CTimeSpan (0,0,0,0);
	m_CanalActual=0;
	m_CanalInicial=1;
	m_CanalFinal=64;
	m_bOcultaCanal = AfxGetApp()->GetProfileInt ("mtx15","OcultaCanal",1);
	m_bOcultaCredito = AfxGetApp()->GetProfileInt ("mtx15","OcultaCredito",1);
	m_TotalErrors=0;
	m_TimeLastCoin=0;
	m_TiempoDescuento=1;
	m_TimerTick=100;
	m_TimerCount=0;
	m_TimerShow=0;
	m_bShowCredito=FALSE;
	m_ValorCredito1=100;
	m_ValorCredito2=200;
	m_bDisabled=FALSE;
	m_SegundosCredito=6;
	m_bStop=FALSE;
	m_bMuteError=FALSE;
	m_b100=FALSE;
	m_b200=FALSE;
	m_b500=FALSE;
	m_bCanalMas=FALSE;
	m_bCanalMenos=FALSE;
	m_ultimaTecla=0;
	m_bMonedero =0;
//	m_bTimerStop=TRUE;
	m_TimerCanalesMas =0;
	m_TimerCanalesMasTrigger=0;
	m_TimerCanalesMenos =0;
	m_TimerCanalesMenosTrigger=0;
	m_IddleTime=0;
	m_numCabina=0;

	m_antiReb=FALSE;
	m_antiRebRetro = FALSE;
	m_antiRebTrgr=FALSE;
	m_antiRebTrgrRetro=FALSE;

	CBuffer *pBuffer = m_mensajes.NewMessage();
	pBuffer->put (MSG_CABINA_ON);
	pBuffer->put (0);
	pBuffer->put (0);
	pBuffer->put (0);

//	m_pTimerThread= AfxBeginThread ((AFX_THREADPROC) TimerThread,(LPVOID)this);
}

CCabina::~CCabina()
{
	if (m_bActivated)
		Activate (FALSE);
}

void CCabina::Activate (bool bActivate)
{
	if (bActivate)
	{
		m_bTimerStop=TRUE;
		m_bCanalesStop=FALSE;
		m_bActivated=TRUE;
		m_credito=100;
		m_TimerShow=m_SegundosCredito; 
		m_pThread[0] = AfxBeginThread ((AFX_THREADPROC) OcultarThread,(LPVOID)this);
		m_pThread[1] = AfxBeginThread ((AFX_THREADPROC) CanalesMasTimer,(LPVOID)this);
		m_pThread[2] = AfxBeginThread ((AFX_THREADPROC) CanalesMenosTimer,(LPVOID)this);
		m_pThread[3] = AfxBeginThread ((AFX_THREADPROC) IddleTimerThread, (LPVOID)this);
		m_pThread[4] = AfxBeginThread ((AFX_THREADPROC) AntiRebThrd, (LPVOID)this);
		m_pThread[5] = AfxBeginThread ((AFX_THREADPROC) AntiRebThrdRetro, (LPVOID)this);
		m_pThread[6]= AfxBeginThread ((AFX_THREADPROC) TimerThread,(LPVOID)this);
		ShowCredito (TRUE);
	}
	else
	{
		m_bCanalesStop=TRUE;
		m_bTimerStop=TRUE;
		m_bStop=TRUE;
		m_bActivated=FALSE;
		
		HANDLE hThread[7];
		for (int n=0;n<7;n++)
			hThread[n]=m_pThread[n]->m_hThread;

		::WaitForMultipleObjects(7, hThread, TRUE, INFINITE);
	}

}

int CCabina::GetStatus()
{
	return m_estado;
}

int CCabina::GetPts()
{
	return m_pesetas;
}
int CCabina::GetPtsTarjeta()
{
	return m_PesetasTarjeta;
}

int CCabina::GetNumCabina()
{
	return m_numCabina;
}
int CCabina::GetValorCredito1()
{
	return m_ValorCredito1;
}

int CCabina::GetValorCredito2()
{
	return m_ValorCredito2;
}
int CCabina::GetCredito()
{
	return m_credito;
}
void CCabina::SetValorCredito1(int credito)
{
	m_ValorCredito1=credito;
}
void CCabina::SetValorCredito2(int credito)
{
	m_ValorCredito2=credito;
}
void CCabina::SetSegundosCredito (int segundos)
{
	m_SegundosCredito = segundos;
}

int CCabina::GetSegundosCredito ()
{
	return m_SegundosCredito;
}

int CCabina::GetImporte()
{
	return m_pesetas;
}

int CCabina::GetTimerTick ()
{
	return m_TimerTick;
}

void CCabina::SetTimerTick (int timer)
{
	m_TimerTick = timer;
}

int CCabina::GetIddleTime ()
{
	return m_IddleTime;
}

void CCabina::SetIddleTime (int time)
{
	m_IddleTime=time;
}

CTimeSpan CCabina::GetTiempo()
{
	return m_tiempo;
}

UINT CCabina::GetTiempoDescuento ()
{
	return m_TiempoDescuento;
}

void CCabina::SetTiempoDescuento (UINT tiempo)
{
	m_TiempoDescuento=tiempo;
}

int CCabina::GetCanalActual()
{
	return m_CanalActual;
}

CTimeSpan CCabina::GetTotalTiempo()
{
	return m_TotalTiempo;
}

void CCabina::Serialize (CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

void CCabina::SetStatus(int status)
{
	m_estado = status;

	if (status==FALSE)
		m_tiempo=0;
}

void CCabina::SetActuacionTeclas (int actuacion)
{
	m_ActuacionTeclas=actuacion;
}

void CCabina::SetSaltoCanal (int salto)
{
	m_SaltoCanal=salto;
}

void CCabina::SetPtsTarjeta (int pts)
{
	m_PesetasTarjeta = pts;
}

void CCabina::SetPts(int pts)
{
	m_pesetas = pts;
}

void CCabina::SetDisabled (BOOL disabled)
{
	m_bDisabled=disabled;
	SetCredito (0);

	if (!disabled)
		m_mensajes.ClearQue();
		
}

BOOL CCabina::GetDisabled ()
{
	return m_bDisabled;
}

void CCabina::SetCanalActual(int canal)
{
	m_CanalActual = canal;


	CBuffer *pBuffer = m_mensajes.NewMessage();

	pBuffer->put (MSG_GOTO_CHANNEL);
	pBuffer->put (m_CanalActual);
	pBuffer->put (0);
	pBuffer->put (0);
	m_bMuteError=FALSE;				
	m_IddleTime=0;
	m_TimerShow=m_SegundosCredito; 
}

CString CCabina::GetMsg()
{
	return m_TxtMessage;
}

void CCabina::SendMsg (CString str)
{

	char pepe[10];
	char *msg;

	m_TxtMessage = str;

	str = str.Left (26);

	for (int sec=0,int n=0;n<str.GetLength();n+=10,sec++)
	{
		strcpy (pepe,str.Mid (n,10));
		CBuffer *pBuffer = m_mensajes.NewMessage();

		pBuffer->put (MSG_PRINT_MSG);
		pBuffer->put (sec);
		pBuffer->put (0);

		msg = pepe;
		while (*msg!=0)
		{
			pBuffer->put (*msg);
			msg++;
		}

		pBuffer->put (0);

	}
	m_IddleTime=0;
}

void CCabina::SetActiva (BOOL activa)
{
	m_credito=0;

	CBuffer *pBuffer = m_mensajes.NewMessage();
	pBuffer->put (MSG_MODO_TEST);
	pBuffer->put (activa);
	pBuffer->put (0);
	pBuffer->put (0);

	if (activa)
	{
		SetCanalActual (m_CanalActual);
	}
	else
		m_b500=FALSE;

	m_bTimerStop=TRUE;
	m_IddleTime=0;
}

void CCabina::SetTiempo (CTimeSpan tiempo)
{
	if (m_estado==FALSE)
		return;

	m_tiempo= tiempo;
//	m_TotalTiempo=tiempo;
}

UINT CCabina::GetCanalInicial()
{
	return m_CanalInicial;
}

UINT CCabina::GetCanalFinal()
{
	return m_CanalFinal;
}

void CCabina::SetCanalInicial(UINT canal)
{
	m_CanalInicial=canal;
}

void CCabina::SetCanalFinal(UINT canal)
{
	m_CanalFinal=canal;
}

void CCabina::SetTotalCanales (int TotalCanales)
{
	m_TotalCanales=TotalCanales;
}

int CCabina::GetTotalCanales ()
{
	return m_TotalCanales;
}

void CCabina::SetTotalTiempo (CTimeSpan tiempo)
{
	m_TotalTiempo = tiempo;
}

int CCabina::GetErrors()
{
	return m_TotalErrors;
}

void CCabina::AddError()
{
	m_TotalErrors++;
}

void CCabina::ClearErrors()
{
	m_TotalErrors=0;
}

UINT CCabina::GetTimeLastCoin(void)
{
	return m_TimeLastCoin;
}

void CCabina::SetTimeLastCoin(UINT time)
{
	m_TimeLastCoin=time;
}

void CCabina::SetFault (BOOL fault)
{
	if (fault)
	{
		MessageBeep (MB_ICONEXCLAMATION);
		m_estado=STATUS_FAULT;
	}
	else
	{
		SetDisabled (FALSE);
		m_estado=0;
		m_TimerCount=m_TiempoDescuento;
		
		StartSequence();
	}
}

void CCabina::MsgStatus ()
{
	if (m_estado!=STATUS_FAULT && !m_bDisabled)
	{
		CBuffer *pBuffer = m_mensajes.NewMessage();
		pBuffer->put (MSG_STATUS);
		pBuffer->put (0);
		pBuffer->put (0);
		pBuffer->put (0);
	}
}

BOOL CCabina::GetShowCredito ()
{
	int ret;
	switch (m_ActuacionTeclas)
	{
		case 0:
			ret = m_bShowCredito;
		break;

		case 1:
			ret = TRUE;
		break;

	}
	return ret;
}

void CCabina::SetOcultaCanal (BOOL oculta)
{
	m_bOcultaCanal=oculta;
}
void CCabina::SetOcultaCredito (BOOL oculta)
{
	m_bOcultaCredito=oculta;
}
void CCabina::SetCredito (int credito)
{
	m_credito=credito;
}

void CCabina::ShowCredito (BOOL bShow)
{
	if (bShow)
	{
/*		CBuffer *pBuffer = m_mensajes.NewMessage();
		pBuffer->put (MSG_SHOW_PESETAS);
		pBuffer->put (1);
		pBuffer->put (HIBYTE (m_credito));
		pBuffer->put (LOBYTE (m_credito));*/
	    m_bShowCredito=TRUE;

//		if (!m_pTimerThread)
//			m_pTimerThread= AfxBeginThread ((AFX_THREADPROC) TimerThread,(LPVOID)this);
//		m_pTimerThread->ResumeThread ();

	}
	else
	{
/*		CBuffer *pBuffer = m_mensajes.NewMessage();
		pBuffer->put (MSG_SHOW_PESETAS);
		pBuffer->put (0);
		pBuffer->put (0);
		pBuffer->put (0);*/
	    m_bShowCredito=FALSE;
	}
}

void CCabina::SetPeticion (BOOL peticion)
{
	CBuffer *pBuffer = m_mensajes.NewMessage();
	pBuffer->put (MSG_PULSO_MONEDERO);
	pBuffer->put (peticion);
	pBuffer->put (0);
	pBuffer->put (0);
}

void CCabina::ShowCanal (BOOL bShow)
{
	CBuffer *pBuffer = m_mensajes.NewMessage();

	if (bShow)
	{
		pBuffer->put (MSG_SHOW_CANAL);
		pBuffer->put (1);
		pBuffer->put (0);
		pBuffer->put (0);
//		if (!m_pTimerThread)
//			m_pTimerThread= AfxBeginThread ((AFX_THREADPROC) TimerThread,(LPVOID)this);
//		m_pTimerThread->ResumeThread ();

	}
	else
	{
		pBuffer->put (MSG_SHOW_CANAL);
		pBuffer->put (0);
		pBuffer->put (0);
		pBuffer->put (0);
	}
}

void CCabina::StartSequence ()
{
	CBuffer *pBuffer;

	pBuffer = m_mensajes.NewMessage();
	pBuffer->put (MSG_START);
	pBuffer->put (0);
	pBuffer->put (0);
	pBuffer->put (0);

	SetPeticion (TRUE);


	SetCanalActual(m_CanalActual);
	ShowCredito (TRUE);
	if (m_bTimerStop)
	{
		m_bTimerStop=FALSE;
		while (m_bTimerStop);
		m_pThread[6]= AfxBeginThread ((AFX_THREADPROC) TimerThread,(LPVOID)this);
	}
}

void CCabina::DoMsg (BYTE status1, BYTE status2)
{
	CBuffer *pBuffer;

	int pesetas = GetPts();
	int pesetasTarjeta = GetPtsTarjeta ();
	int canal = status2;
	int pError;

	int p100 = status1 & 0x10;
	int p200 = status1 & 0x20;
	int p500 = status1 & 0x40;
//	int pError = status1 & 0x01;

	int pCanalMas = status1 & 0x08;
	int pCanalMenos=status1 & 0x04;

//	if (m_numCabina==7)
//		TRACE3 ("%d %x %x \n",m_numCabina, status1, status2);

	if (pError && m_SaltoCanal)
	{
		if (!m_bMuteError)
		{
			m_bMuteError=TRUE;
			if (!m_ultimaTecla || m_ultimaTecla==0x08)
			{
				m_CanalActual++;
				if (m_CanalActual>(int)m_CanalFinal-1)
					m_CanalActual=m_CanalInicial-1;

				SetCanalActual (m_CanalActual);
			}
			
			if (m_ultimaTecla==0x04)
			{
				m_CanalActual--;
				if (m_CanalActual<(int)m_CanalInicial-1)
					m_CanalActual=m_CanalFinal-1;

				SetCanalActual (m_CanalActual);
			}
		}
	}

	if (!pError && m_bMuteError)
	{
		m_bMuteError=FALSE;
	}
	if (p100 || p200 || p500)
	{
		if (p200) 
		{
			if (!m_b200)
			{
				m_b200=TRUE;
				m_ultimaTecla=0;
				m_TimerShow=m_SegundosCredito;
				m_TimerCount=m_TiempoDescuento;
				m_credito+=m_ValorCredito2;
				SetPts(pesetas+2);

				StartSequence ();

				if (!GetStatus()&0x80)
					ShowCredito (TRUE);
			}
		
		}

		
		if (p100)  //1 Euro o Insertan tarjeta.
		{
			if (!m_b100)
			{
				m_b100=TRUE;
				m_ultimaTecla=0;
				m_TimerShow=m_SegundosCredito;
				m_TimerCount=m_TiempoDescuento;
				m_credito+=m_ValorCredito1;
				SetPts(pesetas+1);

				StartSequence();

				if (!GetStatus()&0x80)
					ShowCredito (TRUE);
			}
		}

		if (p500)
		{
			if (!m_b500)
			{
				m_b500=TRUE;
				m_ultimaTecla=0;
				m_TimerShow=m_SegundosCredito;
				m_TimerCount=m_TiempoDescuento;
				m_credito+=m_ValorCredito1;
				SetPtsTarjeta(pesetasTarjeta+1);

				StartSequence();

				if (!GetStatus()&0x80)
					ShowCredito (TRUE);
			}
		}
		m_IddleTime=0;

	}
	else
	{
		m_b200=FALSE;
		m_b100=FALSE;
	}

	if (pCanalMas && (GetStatus()&0x80))
	{
		if (!m_antiReb)
		{
			m_antiRebTrgr=TRUE;
		}
		else if (pCanalMas && (!m_TimerCanalesMas && !m_TimerCanalesMasTrigger))
		{
			m_ultimaTecla=pCanalMas;

			m_TimerShow=m_SegundosCredito;
			if (!GetShowCredito())
				ShowCredito(TRUE);
			else
			{
				m_CanalActual++;
				if (m_CanalActual>(int)m_CanalFinal-1)
					m_CanalActual=m_CanalInicial-1;

				SetCanalActual (m_CanalActual);
			}

			if (!m_bCanalMas)
			{
				m_antiReb=FALSE;
				m_TimerCanalesMasTrigger=TRUE;
				m_bCanalMas=TRUE;
			}
		}
		m_IddleTime = 0;
	}
	else
	{
		m_bCanalMas=0;
		m_TimerCanalesMasTrigger=FALSE;
		m_TimerCanalesMas=FALSE;
	}

	if (pCanalMenos && (GetStatus()&0x80))
	{
		if (!m_antiRebRetro)	
		{
			m_antiRebTrgrRetro=TRUE;
		}
		else if (pCanalMenos &&  (!m_TimerCanalesMenos && !m_TimerCanalesMenosTrigger))
		{
			m_ultimaTecla=pCanalMenos;

			m_TimerShow=m_SegundosCredito;
			if (!GetShowCredito())
				ShowCredito(TRUE);
			else
			{
				m_CanalActual--;
				if (m_CanalActual<(int)m_CanalInicial-1)
					m_CanalActual=m_CanalFinal-1;

				SetCanalActual (m_CanalActual);
			}

			if (!m_bCanalMenos)
			{
				m_antiRebRetro=FALSE;
				m_TimerCanalesMenosTrigger=TRUE;
				m_bCanalMenos=TRUE;
			}
		}
		m_IddleTime = 0;
	}
	else
	{
		m_bCanalMenos=FALSE;
		m_TimerCanalesMenosTrigger=FALSE;
		m_TimerCanalesMenos=FALSE;
	}
 	SetTimeLastCoin(0);
	SetStatus(status1);
}

void CCabina::Timer ()
{
	static bool bFlag=FALSE;
//	if (!m_TimerTick)	//Hacemos un tick cada segundo
//	{
//		m_TimerTick=10;

		if ((m_estado & 0x80) && !m_bStop)
		{	
			if (m_credito)
			{
//				m_credito--;
				if (m_credito>10)
				{
					if (!m_TimerShow && m_bOcultaCredito)
					{
						if (!bFlag)
						{
	//						ShowCredito (FALSE);
							ShowCanal (FALSE);
							bFlag=TRUE;
						}
					}
					else
					{
						if (bFlag)
						{
	//						ShowCredito (TRUE);
							ShowCanal (TRUE);
							bFlag=FALSE;
						}
					}
				}
				else
				{
					SetPeticion (FALSE);
//					ShowCredito (TRUE);
					m_b500=FALSE;
				}
			}
//			else
//			{
//				CBuffer *pBuffer = m_mensajes.NewMessage();
//				pBuffer->put (MSG_CABINA_ON);
//				pBuffer->put (0);
//				pBuffer->put (0);
//				pBuffer->put (0);
//				m_bTimerStop=TRUE;
//			}
		}
//	}
//	else
//		m_TimerTick--;
}

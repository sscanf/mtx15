#if !defined cabinas
#define cabinas
#include "monitor.h"
#include "buffer.h"
#include "txmensajes.h"
#include "afxmt.h"
#include <afxtempl.h>

#define MSG_STATUS			1
#define MSG_PRINT_MSG		2
#define MSG_CABINA_ON		3
#define MSG_GOTO_CHANNEL	4
#define MSG_START			5
#define MSG_STOP			6
#define MSG_SHOW_PESETAS	7
#define MSG_SHOW_CANAL		8
#define MSG_MODO_TEST		9
#define MSG_PULSO_MONEDERO	10

#define MSG_BOM				0xfe		//Principio de mensaje
#define MSG_EOM				0xff		//Final de mensaje


#define STATUS_ACTIVA	0x80
#define STATUS_FAULT	0x7f
#define STATUS_TESTING	0x7e


class CCabina  : public CObject
{
public:

	BOOL m_bCanalesStop;
	BOOL m_bTimerStop;
	BOOL m_bStop;
	BOOL m_bMuteError;
	BOOL m_flag;
	BOOL m_TipoMoneda;
	BOOL m_TimerCanalesMasTrigger;
	BOOL m_TimerCanalesMas;
	BOOL m_TimerCanalesMenosTrigger;
	BOOL m_TimerCanalesMenos;
	BOOL m_antiReb;
	BOOL m_antiRebRetro;
	BOOL m_antiRebTrgr;
	BOOL m_antiRebTrgrRetro;
	BOOL m_TiempoAntirreb;
	BOOL m_TiempoAntirrebRetro;

	int m_TimerShow;

	void ClearErrors(void);
	void AddError(void);
	int GetErrors (void);
	CCabina();
	CCabina(int nCabina);

	int GetCanalActual(void);
	UINT GetCanalInicial(void);
	UINT GetCanalFinal(void);
	int  GetTimerTick ();
	void SetTimerTick(int timer);
	void SetCanalInicial(UINT canal);
	void SetCanalFinal(UINT canal);
	int GetNumCabina();

	int GetIddleTime (void);
	void SetIddleTime (int time);
	CTimeSpan GetTiempo (void);
	CTimeSpan GetTotalTiempo(void);
	int GetPts (void);
	int GetPtsTarjeta (void);
	int GetValorCredito1();
	int GetValorCredito2();
	void SetValorCredito1(int);
	void SetValorCredito2(int);
	void SetPeticion (BOOL peticion);
	
	CString GetMsg();
	int GetCredito (void);
	int GetStatus (void);
	void SetCanalActual (int);
	void SetPts(int pts);
	void SetPtsTarjeta(int pts);
	void SetStatus (int status);
	void SetTiempo (CTimeSpan time);
	void SetTotalTiempo (CTimeSpan time);
	virtual void Serialize (CArchive& ar);
	virtual ~CCabina();
	UINT GetTimeLastCoin(void);
	void SetTimeLastCoin(UINT);
	void DoMsg (BYTE status1, BYTE status2);
	void SetTiempoDescuento (UINT tiempo);
	UINT GetTiempoDescuento ();

	void MsgStatus ();
	void SetFault(BOOL fault);
	void SetTotalCanales (int TotalCanales);
	int GetTotalCanales ();

	void SetActiva (BOOL activa);
	void ShowCredito (BOOL show);
	void ShowCanal (BOOL show);
	void SetCredito (int credito);
	void SetOcultaCredito (BOOL oculta);
	void SetOcultaCanal (BOOL oculta);
	BOOL GetShowCredito ();
	int GetImporte();
	BOOL GetDisabled ();
	void SetDisabled (BOOL);
	void SetActuacionTeclas (int);
	void SetSaltoCanal (int);
	void SetSegundosCredito (int);
	int GetSegundosCredito ();
	void SendMsg (CString str);
	CTxMensajes m_mensajes;
	void Activate (bool bActivate);
	
protected:
	DECLARE_SERIAL (CCabina);
	void Timer ();
	void StartSequence ();
	CWinThread *m_pThread[7];
	void Init();
	
private:

	BOOL m_bActivated;
	CString m_TxtMessage;
	UINT m_CanalInicial;
	UINT m_CanalFinal;
	int m_TiempoDescuento;
	int m_TimerCount;
	int m_TimerTick;
	int m_CanalActual;
	BOOL m_estado;
	
	int m_IddleTime;
	CTimeSpan m_tiempo;
	CTimeSpan m_TotalTiempo;
	int m_pesetas;
	int m_PesetasTarjeta;

	int m_credito;
	UINT m_TimeLastCoin;
	int m_TotalErrors;
	int m_TotalCanales;
	BOOL m_bShowCredito;
	UINT m_ValorCredito1;
	UINT m_ValorCredito2;
	BOOL m_bDisabled;
	BOOL m_bOcultaCredito;
	BOOL m_bOcultaCanal;
	int m_ActuacionTeclas;
	int m_SaltoCanal;
	int m_SegundosCredito;
	int m_ultimaTecla;
	int m_numCabina;

	BOOL m_b100;
	BOOL m_b200;
	BOOL m_b500;
	BOOL m_bCanalMas;
	BOOL m_bCanalMenos;
	BOOL m_bMonedero;
};

#endif

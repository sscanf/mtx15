// CabinaProp.cpp : implementation file
//

#include "stdafx.h"
#include "sexshop.h"
#include "CabinaProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCabinaProp dialog


CCabinaProp::CCabinaProp(CWnd* pParent /*=NULL*/)
	: CDialog(CCabinaProp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCabinaProp)
	m_CanalFinal = 0;
	m_CanalInicial = 0;
	m_bDeshabilitada = FALSE;
	m_ValorCredito1 = 0;
	m_ValorCredito2 = 0;
	m_ValorDescuento = 0;
	m_msg = _T("");
	m_strNumero = _T("");
	//}}AFX_DATA_INIT
}


void CCabinaProp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCabinaProp)

	DDX_Control(pDX, IDC_CANAL_ACT, m_canalAct);
	DDX_Control(pDX, IDC_SPIN1, m_ctrlSpin);
	DDX_Control(pDX, IDC_LUZ_ESTADO, m_LuzEstado);
	DDX_Control(pDX, IDC_ENCENDER, m_onoff);
	DDX_Text(pDX, IDC_CANAL_FINAL, m_CanalFinal);
	DDV_MinMaxInt(pDX, m_CanalFinal, 1, 64);
	DDX_Text(pDX, IDC_CANAL_INICIAL, m_CanalInicial);
	DDX_Check(pDX, IDC_CHECK1, m_bDeshabilitada);
	DDX_Text(pDX, IDC_VALOR_1, m_ValorCredito1);
	DDX_Text(pDX, IDC_VALOR_2, m_ValorCredito2);
	DDX_Text(pDX, IDC_VALOR_DESCUENTO, m_ValorDescuento);
	DDX_Text(pDX, IDC_ENVIAR_TEXTO, m_msg);
	DDX_Text(pDX, IDC_NUMERO, m_strNumero);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCabinaProp, CDialog)
	//{{AFX_MSG_MAP(CCabinaProp)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_ENCENDER, OnEncender)
	ON_EN_CHANGE(IDC_CANAL_ACT, OnChangeCanalAct)
	ON_EN_UPDATE(IDC_CANAL_ACT, OnUpdateCanalAct)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_BN_CLICKED(IDC_APLICAR, OnAplicar)
	ON_BN_CLICKED(IDC_BORRAR_ESTADISTICAS, OnBorrarEstadisticas)
	ON_BN_CLICKED(IDC_ENVIAR, OnEnviar)
	ON_BN_CLICKED(IDC_BORRAR_TEXTO, OnBorrarTexto)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_PETICION_NO, OnPeticionNo)
	ON_BN_CLICKED(IDC_PETICION_SI, OnPeticionSi)
	ON_COMMAND(ID_CANALMAS, OnCanalmas)
	ON_COMMAND(ID_CANALMENOS, OnCanalmenos)
	ON_CBN_SELCHANGE(IDC_NUMERO, OnSelchangeNumero)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCabinaProp message handlers

BOOL CCabinaProp::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_hAccelTable = LoadAccelerators(AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDR_CABINAPROP)); 

	CCabina *pCabina=m_pDia->BuscaCabina (m_cabina);
	m_pCabina=pCabina;


	m_ctrlSpin.SetRange (1,m_pCabina->GetCanalFinal());
	
	m_CanalInicial=m_pCabina->GetCanalInicial();
	m_CanalFinal=m_pCabina->GetCanalFinal();
	m_ValorDescuento=m_pCabina->GetTiempoDescuento();
	m_ValorCredito1 = m_pCabina->GetValorCredito1();
	m_ValorCredito2 = m_pCabina->GetValorCredito2();
	m_bDeshabilitada = m_pCabina->GetDisabled();

	m_brush.CreateSolidBrush(RGB (192,192,192));


	m_msg = m_pCabina->GetMsg ();

	UpdateData (FALSE);

	CComboBox *pCombo = (CComboBox *) GetDlgItem (IDC_NUMERO);
	CString str;

	for (int n=0;n<m_strNumero.GetLength ();n++)
	{
		str=str+m_strNumero.Mid (n,1);
		if (str.Right (1)=="\n")
		{
			pCombo->AddString (str.Left (str.GetLength()-1));
			str.Empty();
		}
	}

	pCombo->SetCurSel (m_cabina);

	SetTimer (1000,1000,NULL);

	m_canalAct.SetText("00"); 
	m_canalAct.SetColor( RGB(10,10,10), RGB(0,255,0) );
	m_canalAct.SetBackColor( RGB(0,0,0) );
	m_canalAct.ModifyDigiStyle(0, CDigistring::DS_SMOOTH);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CCabinaProp::OnTimer(UINT nIDEvent) 
{
	CString strCredito,strImporte,strTiempo,strAcumulado,strCanal,strCanalActual,strEstado;

	strCredito.Format ("%d",m_pCabina->GetCredito());
	strImporte.Format ("%d",m_pCabina->GetImporte()/100);
	strTiempo=m_pCabina->GetTiempo().Format ("%H:%M.%S");
	strAcumulado=m_pCabina->GetTotalTiempo().Format ("%H:%M.%S");
	strCanal.Format ("%2d",m_pCabina->GetCanalActual()+1);
	m_CanalActual=m_pCabina->GetCanalActual();

	if (m_pCabina->GetStatus()&0x80 && m_pCabina->GetStatus()!=0xff) 
	{
		strEstado="Ocupado";
	}
	else
	{
		strEstado="Libre";
	}

	if (m_pCabina->GetStatus()==0xFF) 
	{
		strEstado="Error";
		m_bDeshabilitada=TRUE;
	}

	m_canalAct.SetText(strCanal); 

	CStatic *pStatic = (CStatic *)GetDlgItem (IDC_CREDITO);
	pStatic->SetWindowText (strCredito);

	pStatic = (CStatic *)GetDlgItem (IDC_IMPORTE);
	pStatic->SetWindowText (strImporte);

	pStatic = (CStatic *)GetDlgItem (IDC_TIEMPO);
	pStatic->SetWindowText (strTiempo);

	pStatic = (CStatic *)GetDlgItem (IDC_ACUMULADO);
	pStatic->SetWindowText (strAcumulado);

	pStatic = (CStatic *)GetDlgItem (IDC_CANAL);
	pStatic->SetWindowText (strCanal);

	pStatic = (CStatic *)GetDlgItem (IDC_ESTADO);
	pStatic->SetWindowText (strEstado);

	CDialog::OnTimer(nIDEvent);
}

void CCabinaProp::OnEncender() 
{
	UpdateData (TRUE);

	m_cabina = atoi (m_strNumero)-1;
	m_pCabina=m_pDia->BuscaCabina (m_cabina);


	if (m_pCabina->GetStatus()==0xff)
	{
		m_pCabina->SetFault (FALSE);
	}
	else
		m_pCabina->SetActiva (!(m_pCabina->GetStatus ()&0x80));
}

void CCabinaProp::OnChangeCanalAct() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	
}

void CCabinaProp::OnUpdateCanalAct() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	
}

void CCabinaProp::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here


	*pResult = 0;
	if (pNMUpDown->iDelta>0)
		OnCanalmas();
	else
		OnCanalmenos();

}

void CCabinaProp::OnAplicar() 
{
	UpdateData (TRUE);
	CString str;

	if (m_CanalInicial<1 || m_CanalInicial>m_pCabina->GetTotalCanales())
	{
		str.Format ("El canal inicial ha de ser entre 1 y %d",m_pCabina->GetTotalCanales);
		AfxMessageBox (str);
		return;
	}

	if (m_CanalFinal<1 || m_CanalFinal>m_pCabina->GetTotalCanales())
	{
		str.Format ("El canal final ha de ser entre 1 y %d",m_pCabina->GetTotalCanales);
		AfxMessageBox (str);
		return;
	}

	if (m_ValorDescuento<1)
	{
		AfxMessageBox ("El tiempo de descuento ha de ser mínimo 1 segundo");
		return;
	}
	m_pCabina->SetCanalInicial (m_CanalInicial);
	m_pCabina->SetCanalFinal (m_CanalFinal);
	m_pCabina->SetTiempoDescuento (m_ValorDescuento);
	m_pCabina->SetTimerTick (m_ValorDescuento*1000);
	m_pCabina->SetValorCredito1 (m_ValorCredito1);
	m_pCabina->SetValorCredito2 (m_ValorCredito2);
	m_pCabina->SetDisabled(m_bDeshabilitada);
}

void CCabinaProp::OnBorrarEstadisticas() 
{
	m_pCabina->SetPts(0);
	m_pCabina->SetPtsTarjeta(0);
	m_pCabina->SetTiempo (0);
	m_pCabina->SetTotalTiempo(0);
	m_pCabina->SetActiva (!(m_pCabina->GetStatus ()|0x80));
}	

void CCabinaProp::OnEnviar() 
{
	UpdateData (TRUE);

	m_pCabina->SendMsg (m_msg);

}

void CCabinaProp::OnBorrarTexto() 
{
	m_msg = "                          ";
	UpdateData (FALSE);
	OnEnviar();
	m_msg.Empty();
	UpdateData (FALSE);
}

HBRUSH CCabinaProp::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_DLG ||nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor (RGB (192,192,192));
		return m_brush;
	}
	return hbr;
}

void CCabinaProp::OnPeticionNo() 
{
	m_cabina = atoi (m_strNumero)-1;
	m_pCabina=m_pDia->BuscaCabina (m_cabina);

	m_pCabina->SetPeticion (FALSE);
}

void CCabinaProp::OnPeticionSi() 
{
	m_cabina = atoi (m_strNumero)-1;
	m_pCabina=m_pDia->BuscaCabina (m_cabina);
	m_pCabina->SetPeticion (TRUE);
}

BOOL CCabinaProp::PreTranslateMessage(MSG* pMsg) 
{
   if (m_hAccelTable) 
   {
	  if (pMsg->lParam==21823489 ||pMsg->lParam==21692417)
		return CDialog::PreTranslateMessage(pMsg);

      if (::TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg)) {
         return(TRUE);
      }
   }

   return CDialog::PreTranslateMessage(pMsg);
}

void CCabinaProp::OnCanalmas() 
{
	UpdateData (TRUE);
	m_cabina = atoi (m_strNumero)-1;
	m_pCabina=m_pDia->BuscaCabina (m_cabina);

	m_CanalActual++;
	if (m_CanalActual==64)
		m_CanalActual=0;
	m_pCabina->SetCanalActual (m_CanalActual);

	CString str;
	str.Format ("%2d",m_CanalActual+1);
	m_canalAct.SetText (str);
	
}

void CCabinaProp::OnCanalmenos() 
{
	UpdateData (TRUE);
	m_cabina = atoi (m_strNumero)-1;
	m_pCabina=m_pDia->BuscaCabina (m_cabina);

	m_CanalActual--;
	if (m_CanalActual<(int)(m_pCabina->GetCanalInicial()-1))
		m_CanalActual=m_pCabina->GetCanalFinal()-1;

	m_pCabina->SetCanalActual (m_CanalActual);

	CString str;
	str.Format ("%2d",m_CanalActual+1);
	m_canalAct.SetText (str);
}

void CCabinaProp::OnSelchangeNumero() 
{
	UpdateData (TRUE);
	m_cabina = atoi (m_strNumero)-1;
	m_pCabina=m_pDia->BuscaCabina (m_cabina);


	m_ctrlSpin.SetRange (1,m_pCabina->GetCanalFinal());
	
	m_CanalInicial=m_pCabina->GetCanalInicial();
	m_CanalFinal=m_pCabina->GetCanalFinal();
	m_ValorDescuento=m_pCabina->GetTiempoDescuento();
	m_ValorCredito1 = m_pCabina->GetValorCredito1();
	m_ValorCredito2 = m_pCabina->GetValorCredito2();
	m_bDeshabilitada = m_pCabina->GetDisabled();

	m_msg = m_pCabina->GetMsg ();

	UpdateData (FALSE);

}

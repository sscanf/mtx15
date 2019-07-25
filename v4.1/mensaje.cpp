// Mensaje.cpp : implementation file
//

#include "stdafx.h"
#include "sexshop.h"
#include "Mensaje.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMensaje dialog


CMensaje::CMensaje(CWnd* pParent /*=NULL*/)
	: CDialog(CMensaje::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMensaje)
	m_mensaje = _T("");
	//}}AFX_DATA_INIT
}


void CMensaje::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMensaje)
	DDX_Text(pDX, IDC_MENSAJE, m_mensaje);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMensaje, CDialog)
	//{{AFX_MSG_MAP(CMensaje)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMensaje message handlers

void CMensaje::mensaje(CString mensaje)
{
	m_mensaje=mensaje;
	UpdateData (FALSE);
}

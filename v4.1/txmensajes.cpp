// TxMensajes.cpp: implementation of the CTxMensajes class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sexshop.h"
#include "TxMensajes.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTxMensajes::CTxMensajes()
{
	
}

void CTxMensajes::RemoveMsg(int pos)
{
	if (m_MesajeCue.GetSize()==0)
		return;

	if (pos>m_MesajeCue.GetSize()-1)
		return;

	CBuffer *pBuffer = m_MesajeCue[pos];
	delete pBuffer;
	m_MesajeCue.RemoveAt (pos);
}

void CTxMensajes::ClearQue ()
{
	for (int n=0; n<m_MesajeCue.GetSize();n++)
		delete m_MesajeCue[n];

	m_MesajeCue.RemoveAll();
}

CTxMensajes::~CTxMensajes()
{
	ClearQue();
}

CBuffer * CTxMensajes::NewMessage ()
{
	CBuffer *pBuffer = new CBuffer(255);

	m_MesajeCue.Add (pBuffer);
	return pBuffer;
}
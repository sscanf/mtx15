// Canal.cpp: implementation of the CCanal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SexShop.h"
#include "Canal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CCanal, CObject, 1)
CCanal::CCanal()
{
	m_flag=FALSE;
	m_CanalNumero=0;
	m_tiempo=CTimeSpan (0,0,0,0);
	m_activo=FALSE;
}

CCanal::CCanal (int numero)
{
	m_activo=FALSE;
	m_tiempo=CTimeSpan (0,0,0,0);
	m_CanalNumero = numero;
	m_tiempo=0;
	m_flag=FALSE;
}

CCanal::~CCanal()
{

}

void CCanal::Serialize (CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar<<(int)m_activo;
		ar<<(int)m_CanalNumero;
		ar<<(CTimeSpan)m_tiempo;
	}
	else
	{
		ar>>m_activo;
		ar>>m_CanalNumero;
		ar>>m_tiempo;
	}
}

void CCanal::SetTime(CTimeSpan tiempo)
{
	m_tiempo=tiempo;
}

CTimeSpan CCanal::GetTime()
{
	return m_tiempo;
}

int CCanal::GetCanal()
{
	return m_CanalNumero;
}

void CCanal::SetActivo(int activo)
{
	m_activo=activo;
}

int CCanal::GetActivo()
{
	return m_activo;
}

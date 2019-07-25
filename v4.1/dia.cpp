// Dia.cpp : implementation file
//

#include "stdafx.h"
#include "SexShop.h"
#include "Dia.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDia

IMPLEMENT_SERIAL(CDia, CObject, 1)

CDia::CDia(UINT TotalCabinas, UINT TotalCanales, BOOL TipoMoneda)
{
	m_fecha = CTime::GetCurrentTime();
	if (m_pCabinas.IsEmpty())
	{
		for (int n=0;n<TotalCabinas;n++)
		{
			CCabina *pCabina = new CCabina(n);
			pCabina->m_TipoMoneda=TipoMoneda;
			pCabina->SetTotalCanales (TotalCanales);
			m_pCabinas.AddTail (pCabina);
		}
	}

	if (m_pCanales.IsEmpty())
	{
		for (int n=0;n<TotalCanales;n++)
			m_pCanales.AddTail (new CCanal(n+1));
	}

	m_TotalCabinas = TotalCabinas;
	m_TotalCanales = TotalCanales;
	m_TipoMoneda = TipoMoneda;


	POSITION pos;

	mutex.Lock();
	pos=m_pCabinas.GetHeadPosition();

	for (int i=0;i<m_TotalCabinas;i++)
		m_CabinasArr.Add (m_pCabinas.GetNext (pos));

	mutex.Unlock();
}
CDia::CDia()
{
}


CDia::~CDia()
{
	CCabina *pCabina;
	POSITION pos = m_pCabinas.GetHeadPosition();

	while (pos!=NULL)
	{
		pCabina = m_pCabinas.GetNext(pos);
		delete pCabina;
	}
	
	CCanal *pCanal;
	pos = m_pCanales.GetHeadPosition();

	while (pos!=NULL)
	{
		pCanal = m_pCanales.GetNext(pos);
		delete pCanal;
	}

}

/////////////////////////////////////////////////////////////////////////////
// CDia serialization

void CDia::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar<<(CTime)m_fecha;
		ar<<m_TotalCabinas;
		ar<<m_TotalCanales;
		ar<<m_TipoMoneda;
	}
	else
	{
		ar>>m_fecha;
		ar>>m_TotalCabinas;
		ar>>m_TotalCanales;
		ar>>m_TipoMoneda;
		m_pCabinas.RemoveAll();
		m_pCanales.RemoveAll();
	}

	m_pCabinas.Serialize(ar);
	m_pCanales.Serialize(ar);

	int count = m_pCabinas.GetCount();

	if (m_pCabinas.IsEmpty())
	{
		for (int n=0;n<m_TotalCabinas;n++)
		{
			CCabina *pCabina = new CCabina (n);
			pCabina->m_TipoMoneda=m_TipoMoneda;
			pCabina->SetTotalCanales (m_TotalCanales);
			m_pCabinas.AddTail (pCabina);
		}
	}

	if (m_pCanales.IsEmpty())
	{
		for (int n=0;n<m_TotalCanales;n++)
			m_pCanales.AddTail (new CCanal(n+1));
	}

	POSITION pos;

	if (!ar.IsStoring())
	{
		if (m_CabinasArr.GetSize())
		{
			for (int n=0;n<m_CabinasArr.GetSize();n++)
				delete m_CabinasArr[n];

			m_CabinasArr.RemoveAll();

		}
		pos=m_pCabinas.GetHeadPosition();

		for (int i=0;i<m_TotalCabinas;i++)
			m_CabinasArr.Add (m_pCabinas.GetNext (pos));
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDia commands

CCanal * CDia::BuscaCanal(UINT canal)
{
	CCanal *pCanal;
	POSITION pos;

	CanalMutex.Lock();
	pos=m_pCanales.GetHeadPosition();

	for (int i=0;i<m_TotalCanales;i++)
	{
		pCanal=m_pCanales.GetNext (pos);
		if (pCanal->GetCanal() == canal+1)
			return pCanal;
	}
	CanalMutex.Unlock();

	return (CCanal *)0;
}

CCabina * CDia::BuscaCabina(UINT cabina)
{
	if (cabina<m_TotalCabinas)
		return m_CabinasArr[cabina];
	else
		return (CCabina *)NULL;
}

// Canal.h: interface for the CCanal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CANAL_H__7434D580_1277_11D2_A734_0000E832DAC0__INCLUDED_)
#define AFX_CANAL_H__7434D580_1277_11D2_A734_0000E832DAC0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CCanal  : public CObject
{
public:
	BOOL m_flag;
	int GetActivo (void);
	void SetActivo (int activo);
	int GetCanal (void);
	CCanal (int numero);
	CTimeSpan GetTime ();
	void SetTime (CTimeSpan tiempo);
	virtual ~CCanal();
	virtual void Serialize (CArchive& ar);

protected:
	int m_activo;
	CCanal();
	CTimeSpan m_tiempo;
	int m_CanalNumero;
	DECLARE_SERIAL (CCanal);


};

#endif // !defined(AFX_CANAL_H__7434D580_1277_11D2_A734_0000E832DAC0__INCLUDED_)

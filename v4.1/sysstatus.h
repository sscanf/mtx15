// SysStatus.h: interface for the SysStatus class.
//
//////////////////////////////////////////////////////////////////////
#include "serialport.h"

#if !defined(AFX_SYSSTATUS_H__108B21A1_4566_11D2_BCBE_0000E832DAC0__INCLUDED_)
#define AFX_SYSSTATUS_H__108B21A1_4566_11D2_BCBE_0000E832DAC0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CSysStatus  
{
public:
	CSysStatus();
	virtual ~CSysStatus();

	void Ping(void);
	CTime GetLastPing (void);
	void Apaga(CSerialPort *pSerial);
	void Reset(void);
	void Enciende(CSerialPort *pSerial);
	int GetStatus(void);
	
	int m_intentos;
private:

	CTime m_LastPing;
	int m_status;
};

#endif // !defined(AFX_SYSSTATUS_H__108B21A1_4566_11D2_BCBE_0000E832DAC0__INCLUDED_)

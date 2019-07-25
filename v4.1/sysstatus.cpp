// SysStatus.cpp: implementation of the SysStatus class.
//
//////////////////////////////////////////////////////////////////////

#include <conio.h>
#include "stdafx.h"
#include "SexShop.h"
#include "SysStatus.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern "C" int _outp( unsigned short port, int databyte );
extern "C" int _inp( unsigned short port);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSysStatus::CSysStatus()
{
	m_LastPing = CTime::GetCurrentTime();
	m_status=0;
	m_intentos=0;
}

CSysStatus::~CSysStatus()
{

}

void CSysStatus::Ping (void)
{
	m_LastPing = CTime::GetCurrentTime();
}

CTime CSysStatus::GetLastPing (void)
{
	return m_LastPing;
}

void CSysStatus::Apaga(CSerialPort *pSerial)
{
//	pSerial->ClearDTR();
	m_status&=0xfc;
}

void CSysStatus::Reset(void)
{
}

void CSysStatus::Enciende(CSerialPort *pSerial)
{

//	pSerial->SetDTR();
	m_status|=0x01;
}

int CSysStatus::GetStatus(void)
{
	return m_status;
}
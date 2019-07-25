// Comm.h: interface for the CComm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMM_H__5017E541_1762_11D2_A73D_0000E832DAC0__INCLUDED_)
#define AFX_COMM_H__5017E541_1762_11D2_A73D_0000E832DAC0__INCLUDED_

#if _MSC_VER >= 1000
#include "MainFrm.h"
#pragma once
#endif // _MSC_VER >= 1000

class CComm  
{
public:
	CComm();
	CComm(CString puerto, HWND hWnd);

	abrir (CString puerto, HWND hWnd);
	virtual ~CComm();
	void CommRx(unsigned char *msg, DWORD len);

public:
	int CommTx (char *msg, DWORD largo);
	HANDLE hCom;
	HWND m_hWnd;
protected:
	DWORD m_bytes;
	UCHAR m_message[80];
	CMainFrame *m_pDoc;
	UINT MsgOk;
};

#endif // !defined(AFX_COMM_H__5017E541_1762_11D2_A73D_0000E832DAC0__INCLUDED_)

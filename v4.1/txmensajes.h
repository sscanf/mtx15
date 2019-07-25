// TxMensajes.h: interface for the CTxMensajes class.
//
//////////////////////////////////////////////////////////////////////

#include <afxtempl.h>
#if !defined(AFX_TXMENSAJES_H__6184BCB5_E35B_409B_A79A_7676228262BC__INCLUDED_)
#define AFX_TXMENSAJES_H__6184BCB5_E35B_409B_A79A_7676228262BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "buffer.h"

class CTxMensajes  
{
public:
	CTxMensajes();

	virtual ~CTxMensajes();

	CBuffer * NewMessage ();
	void ClearQue();
	void RemoveMsg(int pos);

	CArray <CBuffer *, CBuffer *> m_MesajeCue;
};

#endif // !defined(AFX_TXMENSAJES_H__6184BCB5_E35B_409B_A79A_7676228262BC__INCLUDED_)

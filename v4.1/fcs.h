// Fcs.h: interface for the CFcs class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FCS_H__E1C56977_C104_40B9_A06D_C536679881E8__INCLUDED_)
#define AFX_FCS_H__E1C56977_C104_40B9_A06D_C536679881E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFcs  
{
public:
	CFcs();
	virtual ~CFcs();

	USHORT Calc16 (UCHAR *buff, USHORT len);
	USHORT Calc8 (UCHAR *buff, USHORT len);

};

#endif // !defined(AFX_FCS_H__E1C56977_C104_40B9_A06D_C536679881E8__INCLUDED_)

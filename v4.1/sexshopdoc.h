// SexShopDoc.h : interface of the CSexShopDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEXSHOPDOC_H__4529334A_10D4_11D2_A730_0000E832DAC0__INCLUDED_)
#define AFX_SEXSHOPDOC_H__4529334A_10D4_11D2_A730_0000E832DAC0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxtempl.h>
#include "cabina.h"
#include "canal.h"
#include "dia.h"
#include "SysStatus.h"
#include "serialPort.h"	// Added by ClassView


extern UINT PollingThread (LPVOID pParam);
extern UINT PollingTimerThread (LPVOID pParam);
extern UINT ledsThread(LPVOID pParam);


class CComm;
class CSexShopDoc : public CDocument
{

public:	
	CArray <CCabina *,CCabina *> m_pCabinas;
//	CTypedPtrList <CObList,CCanal *> m_pCanales;

protected: // create from serialization only
	CSexShopDoc();
	DECLARE_DYNCREATE(CSexShopDoc)

// Attributes
public:
	CSerialPort m_serial;
	BOOL m_bStop;
	BOOL m_TxThread;
	BOOL m_RxThread;


	OVERLAPPED m_ol;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSexShopDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	virtual void SetTitle(LPCTSTR lpszTitle);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void RxStatus (BYTE cabina, BYTE status1, BYTE status2);
	virtual ~CSexShopDoc();
	int DoCabina (CCabina *pCabina);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	BOOL m_bStartTime;
	BOOL m_TipoMoneda;
	UINT m_TimeCoin;
	UINT m_TotalCabinas;
	CWinThread *m_pPullingThread;
	CWinThread *m_pPullingTimerThread;
	CWinThread *m_pLedsThread;

// Generated message map functions
protected:
	BOOL CanClose ();
	int m_timer;

//	CComm *pCom;
	//{{AFX_MSG(CSexShopDoc)
	afx_msg void OnFileErase();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEXSHOPDOC_H__4529334A_10D4_11D2_A730_0000E832DAC0__INCLUDED_)

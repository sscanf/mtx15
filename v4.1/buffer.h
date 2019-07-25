#include "afxmt.h"
#define BUFFER_SIZE  512

#pragma once
class CBuffer
{
public:
	~CBuffer(void);
	CBuffer(void);
	CBuffer(int len);

	void Init (int len = BUFFER_SIZE);
	unsigned char kbhit();
	void put (UCHAR Chr);
	void put (UCHAR *buff,int len);
	void Clear (void);
	unsigned char * GetBuff ();
	void ReleaseBuff ();

	UCHAR get();
	void  get (unsigned char *buff, int len);
	int GetLen();

	char operator[] (unsigned char pos);
		

protected:

	CCriticalSection g_m;
	int _max_len;
	unsigned char *_ptrBuffer;
	int _next_in;
	int _next_out;
	int _TotalBytes;
	unsigned char *tmpBuff;

};


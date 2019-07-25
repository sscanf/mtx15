#include "stdafx.h"
#include "SexShop.h"
#include "buffer.h"

CBuffer::CBuffer (void)
{
}

char CBuffer::operator[] (unsigned char pos)
{
	pos= (pos+_next_out) %_max_len;
	return _ptrBuffer[pos];
}
CBuffer::CBuffer (int len)
{
	Init(len);
}

CBuffer::~CBuffer(void)
{
	free (_ptrBuffer);
	

}


void CBuffer::Clear()
{
	g_m.Lock();
	memset (_ptrBuffer,0,_max_len);
	_next_in=0;
	_next_out=0;
   _TotalBytes=0;

	g_m.Unlock();

}
void CBuffer::Init (int len /* = 10*/)
{
	_max_len=len;
	_ptrBuffer = new unsigned char[len];
	Clear();

	if (!tmpBuff)
		tmpBuff=0;
}
unsigned char CBuffer::kbhit()
{
	return (_next_in!=_next_out);
}

int CBuffer::GetLen()
{
	return abs (_TotalBytes);
}

void CBuffer::put (UCHAR *buff,int len)
{
	g_m.Lock();
	int n;
	for (n=0;n<len;n++)
		put (buff[n]);
	g_m.Unlock();
}


void CBuffer::put (UCHAR Chr)
{
   int t;
	g_m.Lock();

   _ptrBuffer[_next_in]=Chr;
   t=_next_in;
   _next_in++;
   if (_next_in>=_max_len)
	   _next_in=0;

   if(_next_in==_next_out)
     _next_in=t;           // Buffer full !!
	else
		_TotalBytes++;

	g_m.Unlock();
}


void CBuffer::get (unsigned char *buff, int len)
{
	for (int n=0;n<len;n++)
		buff[n]=get();
}

UCHAR CBuffer::get()
{
   unsigned char c;

   c=_ptrBuffer[_next_out];
   _next_out++;
   if (_next_out>=_max_len)
	   _next_out=0;

   _TotalBytes--;
   return(c);
}

unsigned char * CBuffer::GetBuff ()
{
	tmpBuff = new unsigned char[GetLen()];

	int tmpOut = _next_out;
	get (tmpBuff,GetLen());
	_next_out = tmpOut;

	return tmpBuff;
}

void CBuffer::ReleaseBuff()
{
	if (tmpBuff);
		free (tmpBuff);

	tmpBuff=0;
}

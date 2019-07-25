#include <hidef.h>
#include <ctype.h>
#include <string.h>

#define CLI()	asm cli
#define SEI()	asm sei

#ifndef BUFFER_SIZE
	#define BUFFER_SIZE 10
#endif


#pragma DATA_SEG ram0
char buffer[BUFFER_SIZE];

#pragma DATA_SEG SHORT _DATA_ZERO_PAGE /* place following variables into zeropage */
unsigned char next_in = 0;
unsigned char next_out = 0;
unsigned char TotalBytes=0;

#pragma DATA_SEG DEFAULT 

char task (void)
{
	return (next_in!=next_out);
} 

void TaskInit ()
{
	next_in=0;
	next_out=0;
	TotalBytes=0;
}

char TaskQuery (int pos)
{
   return buffer[0];
}

char TaskGet (void)
{
   unsigned char c;

	if (next_in==next_out)
		return 0xff;

   c=buffer[next_out];
   next_out++;
   if (next_out>=BUFFER_SIZE-1)
	   next_out=0;

   TotalBytes--;
   return(c);
}
void TaskAddTail (char val)
{
	unsigned char t;

   buffer[next_in]=val;
   t=next_in;
   next_in++;
   if (next_in>=BUFFER_SIZE-1)
	   next_in=0;

   if(next_in==next_out)
     next_in=t;           // Buffer full !!

    TotalBytes++;
}

int TaskLen (void)
{
	return TotalBytes;
}

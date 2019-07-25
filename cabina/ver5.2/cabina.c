//4860
#include <stdlib.h>
#include <hidef.h>
#include <ctype.h>
#include "convrt.h"
#include "osd.h"
#include "cabina.h"
#include "delay.h"
#include "rs232.h"
#include "fcs.h"
#include "task.h"
#include <math.h>

#pragma LOOP_UNROLL

#pragma DATA_SEG SHORT  DS_RegsHC05
TS_RegsHC05 RegsHC05;

#pragma DATA_SEG DEFAULT

#define FRCT (*((unsigned char*)(0x19)))
#define CCR (*((unsigned char*)(0x1fdf)))
extern char _SEX[4], _LEX[4], _JSR[3];

#define CLI()	asm cli
#define SEI()	asm sei

#define FALSE 0	
#define TRUE  1
#define BOOL  unsigned int
#define ULONG unsigned long

#define ZOOM_REGISTER           0xcc
#define COLOR_REGISTER          0xcd
#define CONTROL_REGISTER        0xce
#define POSITION_REGISTER       0xcf
#define MODE_REGISTER           0xd0
#define PAGE_ADDRESS            0x0
#define DATA_REGISTER           0x1000
#define ROW_ATTRIBUTES          0xc0
#define REQUEST                 0xec

#define MUTE            b_PB0
#define EL2020DIS       0x2
#define CABINA_ACTIVA   0x80

#define TARJETERO		b_PC0
//#define EL2020_ON       b_PC1
#define CANMAS          b_PC3
#define CANMENOS        b_PC2
#define PTS_100         b_PC4
#define PTS_200         b_PC5
#define PTS_TARJETA     b_PC6

#define MAXIMO_CANALES  64
#define MEM_SIZE  20

#define CANAL_MAS    0
#define CANAL_MENOS  1

#define MSG_STATUS			1
#define MSG_PRINT_MSG		2
#define MSG_CABINA_ON		3
#define MSG_GOTO_CHANNEL	4
#define MSG_START			5
#define MSG_STOP			6
#define MSG_SHOW_PESETAS	7
#define MSG_SHOW_CANAL  	8
#define MSG_MODO_TEST		9
#define MSG_PULSO_TARJETERO	10

#pragma DATA_SEG ram1
unsigned char RsBuffer[20];


#pragma DATA_SEG ram1   //Segmento RAM en 100


void joper(void);
void CanalMas (void);
void CanalMenos (void);
void ShowPesetas (char show, int credito);
void ShowCanal (char show);
void pts100(void);
void pts200(void);
void ptsTarjeta (void);
void CabinaOn (BOOL stat, BOOL bProtectOn);
void ResetCabina (void);
void GotoChannel (char canal);
void Start (BOOL bProtectOn);
void Stop();
void MsgStatus(void);
void PrintMsg(char *msg, unsigned char secuencia);
void ModoTest (BOOL test);
void PulsoTarjetero (char pulso);




////////////////////// VARIABLES GLOBALES //////////////////////////////////

struct 
{
        unsigned int  CanalReal;
}cabina;

struct _timers
{
        int contador;
        int BlinkCount;
}timers;

struct
{
	char nuevo;
	char tipo;
	char datos1;
	int  datos2;
	char *msg;
}mensaje;

union
{
   unsigned char valor;
   struct
   {
       unsigned int acError           :1;             //Bits 0,1,
	   unsigned int ModoTest		  :1;             //Bit 2
	   unsigned int acCanalMenos	  :1;             //Bit 2
	   unsigned int acCanalMas		  :1;             //Bit 3
       unsigned int acPts100          :1;             //Bit 4
       unsigned int acPts200          :1;             //Bit 5
       unsigned int acPtsTarjeta      :1;             //Bit 6
       unsigned int activa            :1;             //Bit 7
   }_status;
}status;


unsigned char ChipSelect;
unsigned char CanalAct;          //Canal actual.
char head;
unsigned char indicativo;
unsigned char bAutonomo;
void doRX(void)
{
	unsigned char crc,RxCrc;
	static char idx=0;
	static char bStart=0;
	static char antByte;

	RsBuffer[idx]=RS_GetChar();

	switch (bStart)
	{
		case 0:
			if (RsBuffer[idx]==0xa1 && antByte==0xec)	//Principio mensaje
				bStart=1;
			idx=0;
		break;

		case 1:
			if (RsBuffer[idx]==0xFF)
				bStart=2;
			else
				idx++;
		break;

		case 2:
			RxCrc =  (BYTE)RsBuffer[idx];
			crc = crc8 ((unsigned char *)RsBuffer,idx);
			
			bStart=0;
			idx=0;
			
			if ( RxCrc == crc && *RsBuffer==indicativo)
			{
				timers.contador=0x45;

				mensaje.tipo=RsBuffer[1];
				mensaje.datos1=RsBuffer[2];
				mensaje.datos2=(RsBuffer[3]<<8)|RsBuffer[4];
				mensaje.msg = RsBuffer+4;
				mensaje.nuevo=TRUE;
			}
		break;
	}


	antByte = RsBuffer[idx];
	if (idx>20)
		idx=0;
}

void doMSG (void)
{

	switch (mensaje.tipo)
	{
		case MSG_MODO_TEST:
		case MSG_CABINA_ON:
				CabinaOn (mensaje.datos1,mensaje.datos2);
		break;

		case MSG_GOTO_CHANNEL:
			GotoChannel (mensaje.datos1);
		break;

		case MSG_START:
			Start (mensaje.datos1);
		break;

		case MSG_STOP:
			Stop ();
		break;

		case MSG_SHOW_PESETAS:
			ShowPesetas (mensaje.datos1, mensaje.datos2);
		break;

		case MSG_SHOW_CANAL:
			ShowCanal (mensaje.datos1);
		break;

		case MSG_PRINT_MSG:
			PrintMsg (mensaje.msg,mensaje.datos1);
		break;

		case MSG_PULSO_TARJETERO:
			PulsoTarjetero (mensaje.datos1);
		break;
	}

	MsgStatus();
}


void main (void)
{
	char antPTC=0;
	char antMUTE=0;
	unsigned char NumCanal;


   CCR = (unsigned char)0xc0;

	SEI();

	asm
	{
		CLRA
		CLRX
	INIT:
		STA $0x30,X
		INCX
		CPX #0x9f
		BNE INIT
	}

	asm
	{
		CLRA
		CLRX
	INIT2:
		STA $0x100,X
		INCX
		CPX #0x5F
		BNE INIT2
	}
	//Esto está extraido del módulo que STARTUPX.C que viene con el compilador
	//Inicializa el entorno para que el C funcione correctamente.
	 _LEX[0]= lda; _LEX[3]= rts; _SEX[0]= sta; _SEX[3]= rts;
	 _JSR[0]= jmp;  


// Configuración de los puertos
	DDRC = 0x83;
	DDRA = 0xff;
	DDRB = 0xfe;	


	b_COPF = FALSE;
	b_CME  = FALSE;
	b_COPE = FALSE;
	b_CM1  = 0;
	b_CM0  = 0;
	b_TOIE = FALSE;        //Timer Overflow Interrupt Enabled

	PORTC= EL2020DIS;
	PORTA = 0xff;
	PORTB = 0xff;
	delay_ms (255);
	PORTA = 0x00;
	PORTB = 0X00;

	TaskInit();

	NumCanal=0x0;
	bAutonomo=TRUE;
	head=0;
	status.valor=0;

	//El indicativo son los bits 2,3,4,5 del port D
	indicativo=PORTD & 0x3c;
	indicativo>>=2;

	OSD_Reset();
	OSD_ScreenPos (20,58); // OSD_ScreenPos (6,2);
	OSD_FullMode (FALSE);
	OSD_CharMode (10);
	OSD_VideoFullPageMode (FALSE);
	CabinaOn(FALSE,FALSE);     //Desactivamos la cabina
      
	RS_Init(0x30,0x2c);  //01 = 38400,n,8,1

   PORTC|= CABINA_ACTIVA;          //Activamos la cabina
   status._status.activa=TRUE;
	GotoChannel (NumCanal);
    
   RS_Enable (TRUE);	//Esto ahora es el EL2020

	CLI();
	for (; ; )
	{
		if (RS_kbhit())	//Entra algo por el puerto RS
			doRX();

		if (mensaje.nuevo)
		{
			doMSG();
			mensaje.nuevo=FALSE;
		}
		

		if (antPTC!=(PORTC&0x7c))
		{
			antPTC=(PORTC&0x7c);

			status._status.acPtsTarjeta = ~PTS_TARJETA;
			status._status.acPts100 = ~PTS_100;
			status._status.acPts200 = ~PTS_200;
			status._status.acCanalMas=~CANMAS; 
			status._status.acCanalMenos=~CANMENOS; 
			status._status.acError=MUTE;
			TaskAddTail (status.valor);

			if (bAutonomo)
			{
				if (!CANMAS)
				{				
					delay_ms(100);
					if (!CANMAS)
					{
						delay_ms(100);
						if (!CANMAS)
						{
							NumCanal = (NumCanal+1)&63;
							GotoChannel (NumCanal);
							ShowCanal (TRUE);
							while (!CANMAS);
						}
					}
				}

				if (!CANMENOS)
				{
					delay_ms(100);
					if (!CANMENOS)
					{
						delay_ms(100);
						if (!CANMENOS)
						{
							NumCanal = (NumCanal-1)&63;
							GotoChannel (NumCanal);
							ShowCanal (TRUE);
							while (!CANMENOS);
						}
					}
				}
			}
		}

		if (antMUTE!=MUTE)
		{
			status._status.acError=MUTE;
			antMUTE=MUTE;
			TaskAddTail (status.valor);
		}
	}
}

void PulsoTarjetero (char pulso)
{
	TARJETERO=pulso;		//Activamos linea para TARJETERO
}

void ShowPesetas (char show, int credito)
{
	char str[3];

	if (show)
	{
		itoa (credito,str,10);
		OSD_TextPos (1,9);
		OSD_PrintStr (str);
		OSD_PrintStr ("         ");	// 9 espacios
	}
	else
	{
		OSD_TextPos (1,9);
		OSD_PrintStr ("     ");
	}

}
void PrintMsg(char *msg, unsigned char secuencia)
{
	static char pos=0;

	if (secuencia==0)
		pos=1;

	OSD_TextPos (pos,8);
    while (*msg!=0)
    {
		OSD_PrintChar (*msg, 0);
		pos++;
		msg++;
    }
}

void ShowCanal (char show)
{
	char str[10];

	if (show)
	{
		itoa (cabina.CanalReal+1,str,10);
		OSD_TextPos (25,9);
		OSD_PrintStr (str);
		OSD_PrintStr ("  ");	// 2 espacios

	}
	else
	{
		OSD_TextPos (25,9);
		OSD_PrintStr ("         ");	// 9 espacios
	}
}

void ResetCabina (void)
{
   CanalAct=0;
   ChipSelect=1;
   status.valor =0;
   TARJETERO = FALSE;
   ShowPesetas(FALSE,FALSE);
}

void CabinaOn (BOOL stat, BOOL bProtectOn)
{       
   if (stat==TRUE)
   {
	   bAutonomo=FALSE;
       PORTC|= CABINA_ACTIVA;          //Activamos la cabina
	   status._status.activa=TRUE;
  	   RS_Enable (TRUE);	//Esto ahora es el EL2020
   }
   else
   {
	   bAutonomo=TRUE;
       PORTC&=~CABINA_ACTIVA;          //Desactivamos la cabina
       PORTA=0;
       PORTB=0;
       ResetCabina();
	   status._status.activa=FALSE;
  	   RS_Enable (FALSE);

   }

   b_TOIE = bProtectOn;        //Timer Overflow Interrupt Enabled
}

void ModoTest (BOOL test)
{
	CabinaOn (test,FALSE);
	status._status.ModoTest=test;
}

void joper (void)
{
   char temp;
   char temp2;

    asm
	{
		CLC
		CLRX
		LDA CanalAct

		RORA
		ROLX
		RORA
		ROLX
		RORA
		ROLX
		RORA
		ROLX
		RORA
		ROLX
		RORA
		ROLX
		RORA
		ROLX
		RORA
		ROLX
		TXA
		AND #0XF0
		STA $0
		
		LDA ChipSelect
		CLC
		CLRX
		RORA
		ROLX
		RORA
		ROLX
		RORA
		ROLX
		RORA
		ROLX
		TXA
		AND #0X0F
		STA temp
		
		LDA $0
		AND #0xF0
		ORA temp
		STA $0
		
		LDA temp
		LSLA
		LSLA
		LSLA
		LSLA
		STA temp2
		
		LDA $1
		AND #0x0F
		ORA temp2
		STA $1
	}
}

////////////////////////// COMUNICACIONES ///////////////////////////////////

char SendByteSPI (unsigned char byte)
{
   SPDR = byte;
   while (!b_SPIF);     //Transmite el byte
   return FALSE;
}

char SendWordSPI (unsigned long byte)
{
   SPDR = HIBYTE(byte);
   while (!b_SPIF);     //Transmite el byte

   SPDR = LOBYTE(byte);
   while (!b_SPIF);     //Transmite el byte

   return FALSE;
}


///////////////////////////// NENSAJES ////////////////////////////////

void MsgStatus (void)
{
	UCHAR n;
	UCHAR buff[3];
	char task;

	SEI();	
	task=TaskGet();

	if (task==0xff)
		task=status.valor;

//	RS_Enable (TRUE);
	for (n=0;n<0xff ;n++ );

	buff[0]=indicativo;
	buff[1]=task;
	buff[2]=cabina.CanalReal;

	RS_PutChar (0xec);
	RS_PutChar (0xa1);
	
	RS_PutString (buff,3);
	
	RS_PutChar ((UCHAR)crc8 ((UCHAR *)buff,(USHORT)3));
//	RS_Enable (FALSE);

	status._status.acPtsTarjeta = FALSE;
	status._status.acPts100 = FALSE;
	status._status.acPts200 = FALSE;
//		status._status.acCanalMas=FALSE; 
//		status._status.acCanalMenos=FALSE; 
//		status._status.acError=FALSE;

	CLI();
}

void GotoChannel (char canal)
{
	unsigned int n;
	ChipSelect = 1;

	ChipSelect <<= (canal/16);

	CanalAct= canal%16;
	cabina.CanalReal = canal;
	joper();
	ShowCanal(TRUE);
}

void Start (BOOL bProtectOn)
{
	CabinaOn (TRUE, bProtectOn);            //Activamos la cabina
//	ShowPesetas(TRUE);             //Muestra las pesetas en el monitor
//	ShowCanal(TRUE);               //Muestra el canal en el monitor
//	b_TOIE=TRUE;
}

void Stop ()
{
	CabinaOn (FALSE, FALSE);       //Desactiva la cabina
	b_TOIE=FALSE;
}

///////////////////////////////////////////////////////////////////////

// ----------------------- RUTINAS INTERRUPCION --------------------------

interrupt void SPI()
{
}

interrupt void TIMER()
{
   if (b_TOF)
   {	   timers.contador--;

	   if (!timers.contador)
		   CabinaOn (FALSE,FALSE);
   }

   asm LDA 0x18;
   asm LDA 0x19;
}

interrupt void IRQ()
{
}

interrupt  void SWI()
{
}

interrupt void SCI()
{
	RS_isr();
}

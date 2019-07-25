#include <stdlib.h>
#include <hidef.h>
#include <ctype.h>
#include "osd.h"
#include "cabina.h"
#include "delay.h"
#include "rs232.h"
#include "convrt.h"
#include "task.h"

#pragma DATA_SEG SHORT  DS_RegsHC05
TS_RegsHC05 RegsHC05;

#pragma DATA_SEG DEFAULT

#define CCR (*((unsigned char*)(0x3fdf)))
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

#define MONEDERO		b_PC0
#define EL2020_ON       b_PC1
#define CANMAS          b_PC3
#define CANMENOS        b_PC2
#define PTS_100         b_PC4
#define PTS_200         b_PC5
#define PTS_500         b_PC6
#define RS485_ENABLE	b_PD4
#define TARJETA_IN		b_PD7

#define MAXIMO_CANALES  64
#define MEM_SIZE  20

#define CANAL_MAS    0
#define CANAL_MENOS  1

#define MSG_STATUS		1
#define MSG_DESCUENTO	2
#define MSG_PRINT_MSG	3
#define MSG_CABINA_ON	4
#define MSG_CABINA_OFF	5



void joper(void);
void CanalMas (void);
void CanalMenos (void);
void pts100(void);
void pts200(void);
void pts500(void);
void CabinaOn (BOOL stat);
void ShowPesetas (void);
void ShowCanal (void);
void ResetCabina (void);
void HidePesetas (void);
void CanalSiguiente();
void CanalPrevio (void);
void ShowTecnico (void);
char *itoa(int valor, char *str);

////////////////////// VARIABLES GLOBALES //////////////////////////////////

union
{
   unsigned char valor;
   struct
   {
        unsigned int stCanalMas        :1;
        unsigned int stCanalMenos      :1;
        unsigned int stPts100          :1;
        unsigned int stPts200          :1;
        unsigned int stPts500          :1;
        unsigned int ultimoPulsado     :1;
   }_teclas;
}teclas;

struct _osd
{
    BOOL bOcultaPts :1;
    BOOL bBlinking  :1;
    BOOL bBlink     :1;
}osd;

union
{
   unsigned char valor;
   struct
   {
       unsigned int nousado           :4;             //Bits 0,1,2,3
       unsigned int acPts100          :1;             //Bit 4
       unsigned int acPts200          :1;             //Bit 5
       unsigned int acPts500          :1;             //Bit 6
       unsigned int activa            :1;             //Bit 7
   }_status;
}status;

struct 
{
        unsigned int  CanalReal;
        unsigned long pesetas;                                  //Total pesetas acumuladas.
        unsigned long divisor;
        unsigned char count;
        BOOL ModoTecnico;
        BOOL flMostrarPelas;
}cabina;

struct _timers
{
        int contador;
        int BlinkCount;
}timers;

unsigned char ChipSelect;
unsigned char CanalAct;          //Canal actual.
char head;


char doRX(void)
{
	static char buffer[10];
	static char idx=0;
	static char bStart=0;

	buffer[idx]=RS_GetChar();

	if (!bStart)
		bStart=(buffer[idx-1]==0xec && buffer[idx]==0xa1);
	else
	{
		if (buffer[idx]==0x00)	//Final de mensaje
		{
			bStart=0;
			idx=0;
			if (buffer[2]==indicativo)
				return buffer[3];
			else
				return FALSE;
		}
	}
	
	if (idx++>10)
		idx=0;

	return FALSE;
}

void doMSG (char mensaje, char datos)
{
	switch (mensaje)
	{
		case MSG_CABINA_ON:
			CabinaOn (TRUE);
		break;

		case MSG_CABINA_OFF:
			CabinaOn (FALSE);
		break;

		case MSG_DESCUENTO:
			
	
	}
}


BOOL IsTecnico ()
{
   long n;

   for (n=0;n<500;n++)
   {
     if (PTS_100 || PTS_200 || PTS_500)
          return FALSE;
   }
   return TRUE;
}

void main (void)
{
   CCR = (unsigned char)0xc0;

	//Esto está extraido del módulo que STARTUPX.C que viene con el compilador
	//Inicializa el entorno para que el C funcione correctamente.
	 _LEX[0]= lda; _LEX[3]= rts; _SEX[0]= sta; _SEX[3]= rts;
	 _JSR[0]= jmp;  

	SEI();
// Configuración de los puertos
	DDRC = 0x83;
	DDRA = 0xff;
	DDRB = 0xfe;	
	DDRD = 0X22;


	b_COPF = FALSE;
	b_CME  = FALSE;
	b_COPE = FALSE;
	b_CM1  = 0;
	b_CM0  = 0;
	b_TOIE = TRUE;        //Timer Overflow Interrupt Enabled

	PORTC= EL2020DIS;
	PORTA = 0xff;
	PORTB = 0xff;
	delay_ms (255);
	PORTA = 0x00;
	PORTB = 0X00;

	timers.contador=100;
	cabina.divisor=1;
	RS485_ENABLE=FALSE;

	head=0;
	teclas.valor=0;
	status.valor=0;

	osd.bOcultaPts=TRUE;
	osd.bBlinking=FALSE;
	osd.bBlink=FALSE;

	cabina.CanalReal=0;
	cabina.pesetas=0;
	cabina.divisor=0;
	cabina.count=0;
	cabina.flMostrarPelas=0;
	cabina.ModoTecnico=FALSE;

	timers.contador=0;
	timers.BlinkCount=0;

	OSD_Reset();
	OSD_ScreenPos (6,1);

	OSD_FullMode (FALSE);
	OSD_VideoFullPageMode (TRUE);
	OSD_BlinkOption (FALSE);

	CabinaOn(FALSE);     //Desactivamos la cabina
//	RS_Init(0x30,0xc2);  //9600,n,8,1
	TaskInit();

	CLI();
   for (; ; )
   {
	   if (RS_kbhit())	//Entra algo por el puerto RS
	   {
		 if(doRX())
			 doMSG();	//Hemos recibido un mensaje.
	   }

      if (cabina.ModoTecnico==FALSE)
      {
         pts100();
         pts200();               
         pts500();
      }
   
      if (cabina.ModoTecnico==FALSE)
      {
         if (!PTS_100 && !PTS_200 && !PTS_500)
         {
            if (IsTecnico())
            {
               cabina.ModoTecnico=TRUE;
               CanalAct=0;
               cabina.CanalReal=0;
               ChipSelect=1;
               CabinaOn(TRUE);
               joper();
               HidePesetas();
               ShowCanal();
               ShowTecnico();
               b_TOIE = FALSE;        //Timer Overflow Interrupt Disabled
            }
         }
       }

      if (PORTC&CABINA_ACTIVA)
      {
         CanalMas();
         CanalMenos();
   
         if (cabina.ModoTecnico==FALSE)
         {
            if (osd.bOcultaPts==TRUE)
               HidePesetas();     
            else
               ShowPesetas();
         }
   
         if (MUTE==TRUE) //No hay video, miramos en el siguiente canal
         {
              delay_ms(160);
              if (MUTE==TRUE)
              {
                 EL2020_ON=FALSE;
                 if (teclas._teclas.ultimoPulsado==CANAL_MAS)
                  CanalSiguiente();
                 else
                  CanalPrevio();
              }
         }
         else
         {
           if (EL2020_ON==FALSE)
           {
              delay_ms(100);
              if (MUTE==FALSE)
                 EL2020_ON=TRUE;
           }
         }
   
         if (cabina.ModoTecnico==TRUE)
         {
            if (PTS_100 && PTS_200 && PTS_500)  //Quitan el modo tecnico
            {
               if (!IsTecnico())
               {
                  CabinaOn(FALSE);
                  b_TOIE = TRUE;        //Timer Overflow Interrupt Enabled
               }
            }
         }
      }//<---
   }
}


void pts200(void)
{
    if (!PTS_200)
    {
        //---------- bucle anti-rebotes -------------//
        if (teclas._teclas.stPts200==FALSE)
        {
            delay_ms(10);
            if (PTS_200)
                return;
            delay_ms(10);
            if (PTS_200)
                return;
            delay_ms(10);
            if (PTS_200)
                return;
         
         if (!PTS_100 || !PTS_500)
            return;
    //-------------------------------------------

            if (!(PORTC&CABINA_ACTIVA))
            {
                CanalAct=0;
                cabina.CanalReal=0;
                ChipSelect=1;
			    CabinaOn (TRUE);
            }

            MONEDERO = TRUE; //Se¤al que indica al monedero que se acaban
                             //las pelas

            osd.bOcultaPts = FALSE;
            osd.bBlink=FALSE;

            cabina.pesetas+=200;
            teclas._teclas.stPts200=TRUE;

            status._status.acPts200=TRUE;
            status._status.acPts100=FALSE;
            status._status.acPts500=FALSE;

            timers.contador=100;
            osd.bBlinking=FALSE;
            ShowPesetas();
            ShowCanal();
            joper();
            delay_ms (100);
        }
    }
    else
    {
        teclas._teclas.stPts200=FALSE;
    }
}

void pts100(void)
{
   if (!PTS_100)
   {
      if (teclas._teclas.stPts100==FALSE)
       {

        //---------- bucle anti-rebotes -------------//
            delay_ms(10);
           
           if (PTS_100)
                return;
           delay_ms(10);
           
           if (PTS_100)
                return;
            delay_ms(10);

            if (PTS_100)
               return;
            delay_ms(10);

            if (PTS_100)
               return;
            delay_ms(10);
            if (PTS_100)
               return;

         if (!PTS_200 || !PTS_500)
            return;
            
            if (!(PORTC&CABINA_ACTIVA))
            {
                CanalAct=0;
                cabina.CanalReal=0;
                ChipSelect=1;
	            CabinaOn (TRUE);                //Activamos la cabina
            }

            MONEDERO = TRUE;         //Señal que indica al monedero que se acaban
                                    //las pelas

            cabina.pesetas+=100;    //Incrementamos pesetas +100
            osd.bOcultaPts = FALSE;  //No queremos que se oculten las pesetas en el monitor
            osd.bBlink = FALSE;               //Inicializamos el flag de parpadeo.

            teclas._teclas.stPts100=TRUE;   //Hasta que no suelten la tecla, no volverá a pasar por aquí

            status._status.acPts100=TRUE;   //Pone flag de 100pts para enviarlo al master cuando pregunte
            status._status.acPts200=FALSE;   //Pone flag de 100pts para enviarlo al master cuando pregunte
            status._status.acPts500=FALSE;   //Pone flag de 100pts para enviarlo al master cuando pregunte

            timers.contador=100;    //Resetea el contador del timer.
            osd.bBlinking=FALSE;             //No queremos parpadeo
            ShowPesetas();                  //Muestra las pesetas en el monitor
            ShowCanal();                    //Muestra el canal en el monitor
            joper();                        //Hace la conmutación en la matriz
            delay_ms (100);
       }
   }
   else
   {
      teclas._teclas.stPts100=FALSE;
   }
}

void pts500(void)
{
   if (!PTS_500)
   {
       if (teclas._teclas.stPts500==FALSE)
       {
           delay_ms(10);
           if (PTS_500)
                   return;
           delay_ms(10);
           if (PTS_500)
                   return;
           delay_ms(10);
           if (PTS_500)
                   return;

         if (!PTS_200 || !PTS_100)
            return;

           if (!(PORTC&CABINA_ACTIVA))
           {
			   CanalAct=0;
			   cabina.CanalReal=0;
			   ChipSelect=1;
			   CabinaOn (TRUE);
           }

           MONEDERO = TRUE; //Se¤al que indica al monedero que se acaban
                             //las pelas

           osd.bOcultaPts = FALSE;
           osd.bBlink=FALSE;

           cabina.pesetas+=500;
           teclas._teclas.stPts500=TRUE;
           
           status._status.acPts500=TRUE;
           status._status.acPts100=FALSE;
           status._status.acPts200=FALSE;

           timers.contador=100;
           osd.bBlinking = FALSE;
           ShowPesetas();
           ShowCanal();
           joper();
            delay_ms (100);
       }
   }
   else 
   {
       teclas._teclas.stPts500=FALSE;
   }
}


void ResetCabina (void)
{
   cabina.count=1;
   cabina.pesetas=0;
   CanalAct=0;
   cabina.CanalReal=0;
   ChipSelect=1;
   status.valor =0;
   teclas._teclas.ultimoPulsado=CANAL_MAS;
   cabina.ModoTecnico=FALSE;
   EL2020_ON = TRUE;
   MONEDERO = FALSE;
}

void CabinaOn (BOOL stat)
{       
   if (stat==TRUE)
   {
       PORTC|= CABINA_ACTIVA;          //Activamos la cabina
       cabina.flMostrarPelas=FALSE;
       status._status.activa=TRUE;
   }
   else
   {
       PORTC&=~CABINA_ACTIVA;          //Desactivamos la cabina
       status._status.activa=FALSE;
       PORTA=0;
       PORTB=0;
       ResetCabina();
       status._status.acPts100=FALSE;   //Pone flag de 100pts para enviarlo al master cuando pregunte
       status._status.acPts200=FALSE;   //Pone flag de 100pts para enviarlo al master cuando pregunte
       status._status.acPts500=FALSE;   //Pone flag de 100pts para enviarlo al master cuando pregunte
   }
}

void CanalSiguiente()
{
    cabina.CanalReal++;
    if (cabina.CanalReal==MAXIMO_CANALES)
    {
        cabina.CanalReal=0;
        CanalAct=0;
        ChipSelect=1;
    }
    else
    {
         CanalAct++;
         if (CanalAct==16)
         {
            CanalAct=0;
            ChipSelect<<=1;
   
            if (ChipSelect==0x10)
            ChipSelect=1;
         }
    }
   status._status.acPts100=FALSE;   //Pone flag de 100pts para enviarlo al master cuando pregunte
   status._status.acPts200=FALSE;   //Pone flag de 100pts para enviarlo al master cuando pregunte
   status._status.acPts500=FALSE;   //Pone flag de 100pts para enviarlo al master cuando pregunte
   joper();
   ShowCanal();
}

void CanalPrevio (void)
{
   char tmp,tmp2;

   cabina.CanalReal--;
   if (cabina.CanalReal==0xffff)
   {
      ChipSelect=1;
      cabina.CanalReal=MAXIMO_CANALES-1;
      tmp=(MAXIMO_CANALES/16)-1;
      ChipSelect<<=tmp;
      
      tmp2=MAXIMO_CANALES-(tmp*16)-1;
      CanalAct=tmp2;
   }
   else
   {
      CanalAct--;
      if (CanalAct==0xff)
      {
         CanalAct=0xf;
         ChipSelect>>=1;
      
         if (!ChipSelect)
         ChipSelect=8;
      }
   }
   status._status.acPts100=FALSE;   //Pone flag de 100pts para enviarlo al master cuando pregunte
   status._status.acPts200=FALSE;   //Pone flag de 100pts para enviarlo al master cuando pregunte
   status._status.acPts500=FALSE;   //Pone flag de 100pts para enviarlo al master cuando pregunte
   joper();
   ShowCanal();
}

void CanalMas(void)
{
   if (CANMAS==FALSE)
   {
        if (teclas._teclas.stCanalMas==FALSE)
       {
         delay_ms(10);
           if (CANMAS==TRUE)
                   return; //Rebote detectado, ignora la pulsación
           delay_ms(10);
           if (CANMAS) 
                   return; //Rebote detectado, ignora la pulsación
           delay_ms(10);
           if (CANMAS) 
                   return; //Rebote detectado, ignora la pulsación
           delay_ms(10);
           if (CANMAS) 
                   return; //Rebote detectado, ignora la pulsación

           teclas._teclas.stCanalMas=TRUE;
           teclas._teclas.ultimoPulsado=CANAL_MAS;
           if (cabina.flMostrarPelas==TRUE)
           {
                cabina.flMostrarPelas=FALSE;
                osd.bOcultaPts=FALSE;
                ShowPesetas();
           }
           else
           {
             CanalSiguiente();
           }
           timers.contador=100;
       }
   }
   else
   {
    teclas._teclas.stCanalMas=FALSE;
   }
}

void CanalMenos (void)
{
    if (CANMENOS==FALSE)
    {
       if (teclas._teclas.stCanalMenos==FALSE)
       {
            delay_ms(10);
            //Bucle anti-rebotes
            if (CANMENOS)
                return; //Rebote detectado, ignora la pulsación
            delay_ms(10);
            if (CANMENOS) 
                return; //Rebote detectado, ignora la pulsación
            delay_ms(10);
            if (CANMENOS) 
                return; //Rebote detectado, ignora la pulsación
            delay_ms(10);
            if (CANMENOS) 
                return; //Rebote detectado, ignora la pulsación

            teclas._teclas.ultimoPulsado=CANAL_MENOS;
            teclas._teclas.stCanalMenos=TRUE;

            if (cabina.flMostrarPelas==TRUE)
            {
               cabina.flMostrarPelas=FALSE;
               osd.bOcultaPts=FALSE;
               ShowPesetas();
            }
            else
            {
               CanalPrevio();
            }
         timers.contador=100;
       }
    }
   else
   {
      teclas._teclas.stCanalMenos=FALSE;
   }
}

void HidePesetas (void)
{
	int n;
	
	SEI();
	OSD_TextPos (1,1);
	OSD_PrintStr ("     ");
	CLI();
}

void ShowPesetas (void)
{
	char str[6];

	SEI();
	itoa (cabina.pesetas,str,10);
    OSD_TextPos (1,1);
	OSD_PrintStr (str);
	OSD_PrintStr ("         ");	// 9 espacios
	CLI();

}

void ShowTecnico (void)
{
	OSD_TextPos (1,1);
	OSD_PrintStr ("Tecnico         ");
}

void ShowCanal (void)
{
	char str[6];

	SEI();
	itoa (cabina.CanalReal+1,str,10);
    OSD_TextPos (25,1);
	OSD_PrintStr (str);
	OSD_PrintStr ("         ");	// 9 espacios
	CLI();
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


///////////////////////////////////////////////////////////////////////

// ----------------------- RUTINAS INTERRUPCION --------------------------

interrupt void SPI()
{
}

interrupt void TIMER()
{
   if (b_TOF)
   {
	   asm LDA 0x18;
	   asm LDA 0x19;

	   if (!timers.contador)
	   {
		  cabina.flMostrarPelas=TRUE;
		  HidePesetas();
		  timers.contador=100;
	   }
	   else
			timers.contador--;


	   if (!timers.BlinkCount && osd.bBlinking)
	   {
			osd.bBlink=~osd.bBlink;     //Complementa el flag, para cambiarlo de estado y así hacer parpadeo

			if (!osd.bBlink)          //Si el flag es 0 muestra pesetas
				osd.bOcultaPts=FALSE;
			else
				osd.bOcultaPts=TRUE;

		  timers.BlinkCount=1;
	   }
	   else
		  timers.BlinkCount--;
   }		
}

interrupt void IRQ()
{
    if (cabina.ModoTecnico==TRUE)
      return;

   if (!cabina.divisor)
   {
       if (PORTC&CABINA_ACTIVA)
       {
         if (!cabina.pesetas)    
         {
              osd.bBlinking = FALSE;
              CabinaOn (FALSE);       //Desactiva la cabina
         }

            cabina.pesetas--;
               
         if (cabina.pesetas<15)
         {
            osd.bBlinking=TRUE;
            timers.BlinkCount=1;
         }

         if (cabina.pesetas<6)
            MONEDERO=FALSE;  //Activamos linea para monedero
      }
        cabina.divisor=1;
   }
   else
       cabina.divisor--;
}

interrupt  void SWI()
{
}

interrupt void SCI()
{
	RS_isr();
}

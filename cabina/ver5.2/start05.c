/*****************************************************
 startup.h - Data structures for startup code
 ----------------------------------------------------
   Copyright (c) HIWARE AG, Basel, Switzerland
               All rights reserved
                  Do not modify!
 *****************************************************/

/* Startup code for large memory model (-ml).  
   The fuction Init does the zero out of global variable (ANSI-C).
   The function Copy does the initialisation of global variables. 
   If neither is used in a application these two procedures may be
   commented out.
*/

#include <hidef.h>

#define lda 0xD6        /* indexed long */
#define sta 0xD7        /* indexed long */
#define jmp 0xCC        /* ext */
#define rts 0x81

#pragma DATA_SEG SHORT _ZEROPAGE


extern char _SEX[4], _LEX[4], _JSR[3];

#include <startup.h>

#pragma DATA_SEG _STARTUP

_startupDesc _startupData;


static void Init (void)
  /**** Zero out global variables */
{
  _RangePtr p                 = _startupData.pZeroOut;
  unsigned int n, nofZeroOuts = _startupData.nofZeroOuts; /* << ES 03/28/95 */
  char *q;
 
    asm{
                 BRA    LBL116
        LBL22:   LDA    p:1                    ; n = p->size
                 STA    _LEX:2
                 LDX    p
                 STX    _LEX:1
                 LDX    #0x03
                 JSR    _LEX
                 STA    n:1
                 DECX   
                 JSR    _LEX
                 STA    n
                 DECX                         ; q = p->beg
                 JSR    _LEX
                 STA    q:1
                 CLRX   
                 JSR    _LEX
                 STA    q
                 BRA    LBL84                 ; test while (n)
        LBL53:   LDA    q:1                   ; *q = \0
                 STA    _SEX:2
                 CLRA   
                 LDX    q
                 STX    _SEX:1
                 CLRX   
                 JSR    _SEX
                 INC    q:1                   ; q++
                 BNE    LBL71
                 INC    q
        LBL71:   TST    n:1                   ; n--
                 BNE    LBL83
                 DEC    n
        LBL83:   DEC    n:1
        LBL84:                                ; test while(n)
                 TST    n:1
                 BNE    LBL53
                 TST    n
                 BNE    LBL53
                 LDA    p:1                   ; p++
                 ADD    #0x04
                 STA    p:1
                 LDA    p
                 ADC    #0x00
                 STA    p
                 TST    nofZeroOuts:1         ; nofZeroOuts--
                 BNE    LBL114
                 DEC    nofZeroOuts
        LBL114:  DEC    nofZeroOuts:1
        LBL116:  TST    nofZeroOuts:1         ; test while(nofZeroOuts)
                 BNE    LBL22
                 TST    nofZeroOuts    
                 BNE    LBL22
       } 
} /* end Init */


static void Copy (void)
  /**** Copy global initialization data */
{
  _CopyPtr  p = _startupData.toCopyDownBeg;
  int  n = p->size;
  char *q, *far r;

    asm{
                 BRA    LBL150             ; test while(n)
        LBL66:   LDA    p:1                ; q = p->destination
                 STA    _LEX:2
                 LDX    p
                 STX    _LEX:1
                 LDX    #0x03
                 JSR    _LEX
                 STA    q:1
                 DECX   
                 JSR    _LEX
                 STA    q
                 LDA    _LEX:2                   ; r = (char *far) (p + 1); 
                 ADD    #0x04
                 STA    r:1
                 LDA    _LEX:1
                 ADC    #0x00
                 STA    r
        LBL67:   LDA    r:1                      ; *q = *r
                 STA    _LEX:2
                 LDX    r
                 STX    _LEX:1
                 CLRX   
                 JSR    _LEX
                 TAX    
                 LDA    q:1
                 STA    _SEX:2
                 LDA    q
                 STA    _SEX:1
                 TXA    
                 CLRX   
                 JSR    _SEX
                 INC    q:1                      ; q++
                 BNE    LBL97
                 INC    q
        LBL97:   INC    r:1                      ; r++
                 BNE    LBL103
                 INC    r               
        LBL103:  TST    n:1                      ; n--
                 BNE    LBL111
                 DEC    n
        LBL111:  DEC    n:1
                 BNE    LBL67                    ; test while(n)
                 TST    n
                 BNE    LBL67                 
                 LDA    r:1                      ; p = (_CopyPtr) r
                 STA    _LEX:2    
                 STA    p:1
                 LDA    r
                 STA    _LEX:1
                 STA    p
                 CLRX                            ; n = p->size
                 JSR    _LEX
                 STA    n
                 INCX   
                 JSR    _LEX
                 STA    n:1
        LBL150:  TST    n:1                       ; test while(n)
                 BNE    LBL66
                 TST    n
                 BNE    LBL66
       }
} /* end Copy */


#ifdef __cplusplus
  extern "C"
#endif
void _Startup (void)
  /**** Entry point of application */
{
#ifdef __cplusplus
  PROC *far funp;
#endif
  
  _LEX[0]= lda; _LEX[3]= rts; _SEX[0]= sta; _SEX[3]= rts;
  _JSR[0]= jmp;  
  for (;;) {
    asm {
                  ; Initialize stack?
                  /* << ES 12/25/94: we must always initialize SP
                  LDA  _startupData.flags:1
                  BEQ  InitStuff
                  */
                  RSP
      InitStuff:
    } /* end asm */;

    Init ();
    Copy ();
#ifdef __cplusplus  // call global constructors
    for(funp = _startupData.mInits; *funp != NULL; funp++) {
      (*funp)();
    }
#endif
    _startupData.main();
  } /* end for */
} /* end _Startup */



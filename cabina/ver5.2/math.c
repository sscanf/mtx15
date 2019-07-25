/*****************************************************
   math.c - Float library: mathematical functions
 ----------------------------------------------------
   Copyright (c) HIWARE AG, Basel, Switzerland
               All rights reserved
                  Do not modify!
 *****************************************************/
/*
 * Copyright (c) 1985 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted provided
 * that: (1) source distributions retain this entire copyright notice and
 * comment, and (2) distributions including binaries display the following
 * acknowledgement:  ``This product includes software developed by the
 * University of California, Berkeley and its contributors'' in the
 * documentation or other materials provided with the distribution and in
 * all advertising materials mentioning features or use of this software.
 * Neither the name of the University nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
/*************************************************************************
 Copyright (c) 1992 HIWARE AG, Basel, Switzerland
               All rights reserved
                           
Version: Double Precsion 1.1;
Author:  Marcel Dasen
Date:    19.8.1992
Updates: 20.8.1992 -md
         9.10.1992  -md
         12.12.1992 -md
         5.3.1993 -md
         07.07.1993  rs     Corrected 'is_special'. 
         12.7.1995  -ES  Handling domain error in atan2.
         11/17/95  ChB   adaptions for little endian machines (XA)
************************************************************************/

#include <errno.h>
#include "math.h" 
#include <float.h>

#ifdef __HC12__
#pragma MESSAGE DISABLE  C12056 /* WARNING C12056: SP debug info incorrect because of optimization or inline assembler */
#endif


/* values returned by is_sepcial */
#define nan 1
#define inf 2
#define minf 3

#ifdef __DOUBLE_IS_IEEE64__
#define EXTENDED_PRECISION_ARCTAN 1   /* otherwise atan(0.7) is about 11 digits precise */
#define EXTENDED_PRECISION_COSSIN 0   /* cos/sin is precise enough */
#else
#define EXTENDED_PRECISION_ARCTAN 0   /* not needed for IEE32 and DSP */
#define EXTENDED_PRECISION_COSSIN 0 
#endif
   
#if defined(__DOUBLE_IS_IEEE32__)
  const intdouble infinity={0x7f800000};
  const intdouble nan_union = {0x7fc00000};
#else 
  #if defined(__LITTLE_ENDIAN__)
    const intdouble infinity={0, 0x7ff00000};
    const intdouble nan_union = {0, 0x7ff10000}; 
  #else 
    const intdouble infinity={0x7ff00000};
    const intdouble nan_union = {0x7ff10000};
  #endif
#endif

#define DOUBLE_NAN nan_union.f
#define DOUBLE_INF infinity.f


static double copysign(double x,double y) {
#if defined(__DOUBLE_IS_IEEE32__)
  intfloat ix,iy;

/*  if(x < 0.0F) x=-x; 
  if((y >= 0.0F))
        return x;  
  return -x;*/
  ix.f = x;
  iy.f = y;
  ix.i = (iy.i & 0x80000000) | (ix.i & 0x7fffffff);
  return ix.f; 
#else
  intdouble ix,iy; 
  
/*  if(x < 0.0 ) x=-x;
  if((y >= 0.0 ))
        return x;  
  return -x;  */
  ix.f=x;
  iy.f=y;
#if defined(__LITTLE_ENDIAN__)
  ix.i[3] &= 0x7FFF;
  ix.i[3] |= (iy.i[3] & 0x8000);
#elif defined(__MCORE__)
  ix.l[0] &= 0x7FFFFFFF;
  ix.l[0] |= (iy.l[0] & 0x80000000);
#else
  ix.i[0] &= 0x7FFF;
  ix.i[0] |= (iy.i[0] & 0x8000);
#endif
  return ix.f;
#endif
} 

static int logb(double x) {
  int exp;

  x=frexp(x,&exp);
  return exp;
}
/************************************************************/
#ifdef fabs
  #undef fabs /* inline version is used in math.h */
#endif
double fabs(double x)
{  
#if defined(__DOUBLE_IS_IEEE32__)
   intfloat ix;

/*        if( x < 0.0 )
                return -x;
        return x; */
  ix.f=x;
  ix.i &= 0x7fffffff;
  return ix.f;
#else
    intdouble ix;
 /*       if( x < 0.0 )
                return -x;
        return x; */
    ix.f=x;
#if defined(__LITTLE_ENDIAN__)
    ix.i[3] &= 0x7fff;
#elif defined(__MCORE__) /* << TG 03/12/98 */
    ix.l[0] &= 0x7fffffff;            /* INT and LONG have the same size */    
#else
    ix.i[0] &= 0x7fff;
#endif
    return ix.f;
#endif
}

/**************************************************************/


static int isint(double x,long int *iy) {

  double y;
  
  (void)modf(x, &y);
  if(x==y) {
    *iy=(long int)y;
    return(1);
 }
 return 0;
}


static int is_special(double x) {
#ifdef __DOUBLE_IS_IEEE32__    

  intfloat ugly;
  
  ugly.f=x;
  if(ugly.i == 0xff800000)
    return minf;
  else if(ugly.i == 0x7f800000)
    return inf;
  else if((ugly.i & 0x7f800000)==0x7f800000){
    return nan;
  }
  return 0;
#else 

  intdouble ugly;
 
  ugly.f=x;
#ifdef __LITTLE_ENDIAN__
  if((ugly.l[1] ==0xfff00000) && (ugly.l[0]==0x00))
    return minf;
  else if((ugly.l[1] == 0x7ff00000) && (ugly.l[0]==0x00))
    return inf;
  else if((ugly.l[1] & 0x7ff00000)==0x7ff00000){
    return nan;
  }
#else
  if((ugly.l[0] ==0xfff00000) && (ugly.l[1]==0x00))
    return minf;
  else if((ugly.l[0] == 0x7ff00000) && (ugly.l[1]==0x00))
    return inf;
  else if((ugly.l[0] & 0x7ff00000)==0x7ff00000){
    return nan;
  }
#endif
  return 0;
#endif
}

/***************************************************************/
/* Method:
 *        Right to left binary method 
 *        
 * Complexity: Approx: log2(arg) multiplications   
 */

double pow_i(double x,int y) {
  
   double z,r;
  
   if(y < 0) {
    x=1/x;
    y=-y;
   }
   switch(y) {
   case 0:return 1.0 ;
   case 1:return x;
   case 2:return x*x;
   case 3:return x*x*x;
   case 4:{r=x*x;return r*r;}
   }
   z=x;r=1.0 ;
   while( y > 0)
   {
      if( y & 1) 
           r*=z;
      y>>=1;
          if(y != 0)
            z*=z;
   }   
   return r;   
}
  

/*******************************************************/
#define    ln2hi   6.9314718036912381649E-1 
#define    ln2lo    1.9082149292705877000E-10 
#define   invln2    1.4426950408889633870E0 
#define       p1    1.6666666666666601904E-1 
#define       p2    -2.7777777777015593384E-3 
#define       p3    6.6137563214379343612E-5 
#define       p4    -1.6533902205465251539E-6 
#define       p5    4.1381367970572384604E-8 
/* Method:
 *      1. Argument Reduction: given the input x, find r and integer k such 
 *         that
 *                         x = k*ln2 + r,  |r| <= 0.5*ln2 .  
 *         r will be represented as r := z+c for better accuracy.
 *
 *      2. Compute exp(r)
 *
 *              exp(r) = 1 + r + r*R1/(2-R1),
 *         where
 *              R1 = x - x^2*(p1+x^2*(p2+x^2*(p3+x^2*(p4+p5*x^2)))).
 *
 *      3. exp(x) = 2^k * exp(r) .
 */

double exp_r(double x)
{
        double  z,hi,lo,c;
        int k;


         if (x >= _M_EXP_MAX) {
                errno = ERANGE; return DOUBLE_INF;
         };
     if (fabs(x) < 2.220446049e-16 ) return 1.0 ;

                if( x >=  -7.5137154372698068983E2  ) {

                    /* argument reduction : x --> x - k*ln2 */

                        k=invln2*x+copysign(0.5 ,x);     /* k=NINT(x/ln2) */

                    /* express x-k*ln2 as hi-lo and let x=hi-lo rounded */

                        hi=x-k*ln2hi;
                        x=hi-(lo=k*ln2lo);

                    /* return 2^k*[1+x+x*c/(2+c)]  */
                        z=x*x;
                        c= x - z*(p1+z*(p2+z*(p3+z*(p4+z*p5))));
                        return  ldexp(1.0 +(hi-(lo-(x*c)/(2.0 -c))),k);

                }

            else 
                     return(ldexp(1.0,-5000));
}
                

double exp (double x)
/* Special cases:
 *      exp(INF) is INF, exp(NaN) is NaN;
 *      exp(-INF)=  0;
 *      for finite argument, only exp(0)=1 is exact.
 */

{
  int r;
  if((r=is_special(x)) != 0) {
    switch(r) {
    case nan:return x;
    case inf:
        errno = ERANGE;
        return DOUBLE_INF;
    case minf:return 0.0;
    }
  }
  return exp_r(x);      
}

/******************************************************/
        
double log_r (double x)
{
  int n;
  double f, znum, zden, z, w, aw, bw, rz2, rz, xn;

  
  f = frexp(x, &n);
  if (f > _M_S1_2) {
    znum = (f - 1.0  );
    zden = ldexp(f, -1) + .5  ;
  } else {
    --n;
    znum = f - .5  ;
    zden = ldexp(znum,-1) + .5  ;
  }
  z = znum/zden;
  w = z * z;
  aw = (-.789561128874912572767  * w + 16.383943563021534222 ) * w -
       64.124943423745581147  ;
  bw = ((w + -35.667977739034646171 ) * w + 312.03222091924532844 ) * w -
       769.49932108494879777  ;
  rz2 = aw/bw * w;
  rz = z + z * rz2;
  xn = n;
  return ((xn * -2.121944400546905827679E-4  + rz) + xn * 0.693359375 );
}


double log (double x)
{
  int r;
  if((r=is_special(x)) != 0) {
    switch(r) {
    case nan:return x;
    case inf:
        errno = ERANGE;
        return DOUBLE_INF;
    case minf:
        errno = EDOM;
        return DOUBLE_NAN;
    }
  }
  if (x <= 0.0 ) {
    if (x == 0.0 ) {  
        errno = ERANGE;
        return -DOUBLE_INF;
    } else{
        errno = EDOM;
        return DOUBLE_NAN;
    }
  }
  return log_r(x);      
}


double log10(double x)
{
        return(4.3429448190325181667E-1*log(x));
}

/*************************************************************************/
/* Method
 *       exp(y*log(x)); for all legal cases with x > 0.0 
 *       x**y using pow_i for integer y
 */
double pow(double x, double y)
        
{
  long int iy;
  int r;
    
  if(y == 0.0 && x != 0.0) {
    return 1.0;
  } else if(y == 1) {
    return x;
  } else if((r=is_special(y))!=0) {
    switch(r) {
      case nan: return y;
      case inf:{
        if(fabs(x) < 1.0 ) {
            return 0.0 ;
        }
        else  {
          errno=ERANGE;
          return DOUBLE_INF;
        }
      }
      case minf:{
        if(fabs(x) < 1.0 ) {
          errno=ERANGE;
          return DOUBLE_INF; 
        }
        else
          return 0.0 ;
      }  
    }        
  }      
    
  if((r=is_special(x)) != 0) {
    switch(r) {
      case nan: return x;
      case inf:{
        if(y > 0.0 )  
            return x;
        else {
            return 0.0 ;
        }
      }
      case minf:{
        if( isint(y,&iy) ) {
          if(iy & 1 )  /* odd y */ {
            return x;
          }
          else {
            return -x;
          }
        }
        return DOUBLE_NAN;
      } 
    }
  }
  else if (x == 0.0 ) {
    if (y <= 0.0 ) {
      errno = EDOM;
      return DOUBLE_INF;
    }
    return x;
  }
  else if(x < 0.0F) {
    if( !isint(y,&iy)  )
    {
      errno = EDOM;
      return DOUBLE_NAN;
    }
    else {
      if(fabs(y) < 32767.0)
        return pow_i(x,iy);
      else
        x=exp_r(y*log_r(-x));
      if(iy & 1)    /* if odd */
        return -x;
      return x;
    }   
  }
  if(isint(y,&iy) && (fabs(y) < 32767.0)) {
    return pow_i(x,iy);
  } 
  return exp_r(y*log_r(x));
}


#define _SIN_HUGE  1 /* sin of more than MAXINT * Pi is not currect,  if set to 0 (old version) */
/******************************************************************************************/
double sincos(double x, int sgn, int flg)
{
/* Method:
     1. reduce argument on -Pi/2..Pi/2
     2. taylor expansion of sin at x0=0;
          precision, computed with maple, 40 decimal digits 
          "f:= (a,x,n)->evalf(subs(y=x,convert(taylor(sin(y),y=a,n),polynom))-sin(x));"
          "i:=0: while (i < 30) do x:=f(0, Pi/2,i): print(i, x, -evalf(log(abs(x))/log(10.0),4)); i:=i+1: od:"
              1, -1., 0
              3, .570796326794896619231321691639751442099, .2435
              5, -751677707113496344244348722581942912805.0E-40, 1.124
              7, 4524855534817410696070677232283731240.0E-39, 2.344
              9, -1568986005012774046147867016697066195.0E-40, 3.804
              11, 3542584286082417257874168493766710.0E-39, 5.449
              13, -562589491296680825843715244416794.0E-40, 7.247
              15, 662780090011185533381028594242.0E-39, 9.175
              17, -60234209699611898667940393363.0E-40, 11.22
              19, 43514761145005800307406329.0E-39, 13.36
              21, -2558935281316224644363841.0E-40, 15.59
              23, 1248761154424922179002.0E-39, 17.90
              25, -51383861105354863381.0E-40, 20.29
              27, 18069066044795996.0E-39, 22.74
              29, -549270836928788.0E-40, 25.25

  so we use a polynom of degree 23 for IEEE64 and one of degree 17 for IEEE32

                  taylor(sin(x),x=0, 30) ==
                x-.1666666666666666666666666666666666666667*x^3+
                8333333333333333333333333333333333333333.0E-42*x^5-
                1984126984126984126984126984126984126984.0E-43*x^7+
                2755731922398589065255731922398589065256.0E-45*x^9-
                2505210838544171877505210838544171877505.0E-47*x^11+
                1605904383682161459939237717015494793273.0E-49*x^13-
                7647163731819816475901131985788070444155.0E-52*x^15+
                2811457254345520763198945583010320016233.0E-54*x^17-
                8220635246624329716955981236872280749221.0E-57*x^19+
                1957294106339126123084757437350543035529.0E-59*x^21-
                3868170170630684037716911931522812323179.0E-62*x^23+
                6446950284384473396194853219204687205299.0E-65*x^25-
                9183689863795546148425716836473913397862.0E-68*x^27+
                1130996288644771693155876457693831699244.0E-70*x^29
  */

#if !_SIN_HUGE
  int n;
#endif
  double y, frac, xn, f, t1, rv;

  y = x;
  if (flg) {
    y +=  _M_PI2;
  }

  frac = modf(y *  _M_1PI, &xn);   
#if !_SIN_HUGE
  n = xn;
  if (frac > 0.5 ) 
    xn = ++n;  
  if (n & 1)
    sgn = !sgn;  
#else
  if (frac > 0.5 ) {
    xn++;
  }
  if ((long)xn & 1) {
    sgn = !sgn;  
  }
#endif
  if (flg) {
    xn -= 0.5 ;
  }
#ifdef __DOUBLE_IS_IEEE32__
  f = (x - xn * 3.1416016 ) + xn * 8.908910206E-6 ; /* correction term for Pi rounding */
#else
  f = x - xn * _M_PI;
#endif
  if (fabs(f) <  _M_EPS) {
    rv = f;
  } else {         
     t1 = f*f; 
#if !_SIN_HUGE /* more summands for higher precision */
     rv = (((((((0.27204790957888846175e-14  * t1 - 0.76429178068910467734e-12 ) * t1 +
             0.16058936490371589114e-9 ) * t1 - 0.25052106798274584544e-7 )  * t1 +
             0.27557319210152756119e-5 ) * t1 - 0.19841269841201840457e-3 )  * t1 +
             0.83333333333331650314e-2 ) * t1 - 0.16666666666666665052 )     * t1;
#else
    rv = ((((((((
#if EXTENDED_PRECISION_COSSIN 
           (((
                 3868170170630684037716911931522812323179.0E-62) * t1 +
                 1957294106339126123084757437350543035529.0E-59) * t1 -
                 8220635246624329716955981236872280749221.0E-57) * t1 +
#endif
                 2811457254345520763198945583010320016233.0E-54) * t1 - 
                 7647163731819816475901131985788070444155.0E-52) * t1 +
                 1605904383682161459939237717015494793273.0E-49) * t1 - 
                 2505210838544171877505210838544171877505.0E-47) * t1 +
                 2755731922398589065255731922398589065256.0E-45) * t1 -
                 1984126984126984126984126984126984126984.0E-43) * t1 +
                 8333333333333333333333333333333333333333.0E-42) * t1 - 
                 0.1666666666666666666666666666666666666667 )    * t1;

#endif
      rv=f+rv*f; 
  }
  if(sgn) {
    return (-rv);
  }
  return (rv);
}




/*****************************************************/
double tan(double x)
{
  int n;
  double y, frac, xn, f, xnum, xden, g;

  if((n=is_special(x))!=0) {
                switch(n) {
                case nan:return x;
                case inf:return 0.0 ;
                case minf:return 0.0 ;
                }     
  }
  y=fabs(x);
#if !_SIN_HUGE 
  if (y > 149078413.0 ) {
    errno = ERANGE; return DOUBLE_INF;
  }
#endif
  frac = modf( x * 0.63661977236758134308 , &xn);
#if !_SIN_HUGE
  n = xn;
  if (frac > 0.5 ) {
    xn = ++n;
  }
#else
  if (frac > 0.5 ) {
    xn++;
  }
#endif

#ifdef __DOUBLE_IS_IEEE32__
  f = (x - xn * 1.57080078125 ) - xn * -4.454455103380768678308e-6 ;
#else
  f = x - xn * (_M_PI/2);
#endif
  if (fabs(f) < _M_EPS) {
      xnum = f;
      xden = 1;
  } else {
      g = f * f;
      xnum = ((-0.17861707342254426711e-4  * g + 0.34248878235890589960e-2 ) * g -
                0.13338350006421960681 )* g * f + f;
      xden = (((0.49819433993786512270e-6  * g - 0.31181531907010027307e-3 ) * g +
                0.25663832289440112864e-1 )* g - 0.46671683339755294240 ) * g + 1.0 ;
  }
#if 0
  if(n & 1) {
#else
  if((long)xn & 1) {
#endif
    xnum = -xnum;
    return (xden/xnum);
  } else {
    return (xnum/xden);
  }
}


double sin(double x)
{
  int sgn,r;
  
  if((r=is_special(x))!=0) {
                switch(r) {
                case nan:return x;
                case inf:return 0.0 ;
                case minf:{
                  return 0.0 ;
                }
                }     
  }
  else if (x == 0.0 ) 
      return 0.0 ;
  if (x < 0.0 ) 
    {
      sgn = 1;
      x = -x;
    }
  else 
    sgn = 0;
  return (sincos(x,sgn,0));
}

/*****************************************************/

double cos(double x)
{

 int r;

  if((r=is_special(x))!=0) {
                switch(r) {
                case nan:return x;
                case inf:return 0.0 ;
                case minf:return 0.0 ;
                }
  }
  else if(x == 0.0 )
      return 1.0 ;

  return (sincos(fabs(x),0,1));
}

/*****************************************************/
/* Method:
 *       repetitiv approx. using Heron's method
 */ 
double sqrt_r(double x)
{
  int n;
  double f, z, y0, y2, y3;

 
  if (x == 0.0 ) 
    return (x);
  f = frexp(x,&n);
  y0 = .41731  + .59016  * f;
      
  z = y0 + f/y0;
  y2 = ldexp(z,-2) + f/z;
  y3 = ldexp(y2 + f/y2,-1);
  if (n & 1) /* exponent is odd */
    {
      y3 = y3 * _M_S1_2;
      ++n;
    }
  n >>=1;
  return (ldexp(y3,n));
}

double sqrt(double x) {
  unsigned r;

  if((r=is_special(x))!=0) {
                switch(r) {
                case nan:return x;
                case inf:return DOUBLE_INF;
                case minf:{
                   errno = EDOM;
                   return DOUBLE_NAN;
                }
       }
  }
  else if (x < 0.0 )
{
      errno = EDOM;
      return DOUBLE_NAN;
  }

 return sqrt_r(x);
}
/*************************************************************/
#define athfhi  4.6364760900080611433E-1 
#define athflo  1.9338828231967579916E-19 
#define PIo4    7.8539816339744830676E-1 
#define at1fhi  9.8279372324732906796E-1 
#define at1flo  -3.5540295636764633916E-18 
#define PIo2    1.5707963267948966135E0 
#define PI          3.1415926535897932270E0 
#if 0
#define a1         0.3333333333333 
#define a2         -0.200000000000 
#define a3          1.4285714286694640301E-1 
#define a4         -1.1111111135032672795E-1 
#define a5      9.0909091380563043783E-2 
#define a6         -7.6922954286089459397E-2 
#define a7      6.6663180891693915586E-2  
#define a8         -5.8772703698290408927E-2 
#define a9          5.2170707402812969804E-2 
#define a10    -4.4895863157820361210E-2 
#define a11         3.3006147437343875094E-2 
#define a12        -1.4614844866464185439E-2 
#else
/* polynom coefficient calculated with maple taylor(arctan(x). x=0, 30); */
/* here the negated coefficients are given */
#define a0         -1
#define a1          0.33333333333333333333333333333333333333333333333333333333333333333333333333
#define a2         -0.20000000000000000000000000000000000000000000000000000000000000000000000000
#define a3          0.14285714285714285714285714285714285714285714285714285714285714285714285714
#define a4         -0.11111111111111111111111111111111111111111111111111111111111111111111111111
#define a5          0.09090909090909090909090909090909090909090909090909090909090909090909090909
#define a6         -0.07692307692307692307692307692307692307692307692307692307692307692307692307
#define a7          0.06666666666666666666666666666666666666666666666666666666666666666666666666
#define a8         -0.05882352941176470588235294117647058823529411764705882352941176470588235294
#define a9          0.05263157894736842105263157894736842105263157894736842105263157894736842105
#define a10        -0.04761904761904761904761904761904761904761904761904761904761904761904761904
#define a11         0.04347826086956521739130434782608695652173913043478260869565217391304347826
#define a12        -0.04000000000000000000000000000000000000000000000000000000000000000000000000
#define a13         0.03703703703703703703703703703703703703703703703703703703703703703703703703
#define a14        -0.03448275862068965517241379310344827586206896551724137931034482758620689655
#define a15         0.03225806451612903225806451612903225806451612903225806451612903225806451612
#define a16        -0.03030303030303030303030303030303030303030303030303030303030303030303030303
#define a17         0.02857142857142857142857142857142857142857142857142857142857142857142857142
#define a18        -0.02702702702702702702702702702702702702702702702702702702702702702702702702
#define a19         0.02564102564102564102564102564102564102564102564102564102564102564102564102
#define a20        -0.02439024390243902439024390243902439024390243902439024390243902439024390243
#define a21         0.02325581395348837209302325581395348837209302325581395348837209302325581395

/* so for x close to 0, arctan(x ) == - (a0*x + a1*x^3+a2*x^5+a3*x^7+...an*x^(2*n+1) */


 
#endif
/* Method :
 *      1. Reduce y to positive by atan2(y,x)=-atan2(-y,x).
 *      2. Reduce x to positive by (if x and y are unexceptional): 
 *              ARG (x+iy) = arctan(y/x)           ... if x > 0,
 *              ARG (x+iy) = pi - arctan[y/(-x)]   ... if x < 0,
 *      3. According to the integer k=4t+0.25 truncated , t=y/x, the argument 
 *         is further reduced to one of the following intervals and the 
 *         arctangent of y/x is evaluated by the corresponding formula:
 *
 *         [0,7/16]        atan(y/x) = t - t^3*(a1+t^2*(a2+...(a10+t^2*a11)...)
 *         [7/16,11/16]    atan(y/x) = atan(1/2) + atan( (y-x/2)/(x+y/2) )
 *         [11/16.19/16]   atan(y/x) = atan( 1 ) + atan( (y-x)/(x+y) )
 *         [19/16,39/16]   atan(y/x) = atan(3/2) + atan( (y-1.5x)/(x+1.5y) )
 *         [39/16,INF]     atan(y/x) = atan(INF) + atan( -x/y )
 *
 */

double atan2(double y,double x)
{  
    static const double zero=0.0 , one=1.0 , small=1.0E-9 , big=1.0E18 ;
    double t,z,signy,signx,hi,lo;
    int k,m;

    if (x == zero && y == zero) {  /* << ES 12.7.1995 */
      errno = EDOM;
      return 0.0F;
    } 
    /* copy down the sign of y and x */
    signy = copysign(one,y) ;  
    signx = copysign(one,x) ;
    if((m=is_special(x))!=0)
          switch(m) {
          case nan:return y;
          case inf:case minf:if(is_special(y)!=0) 
                return(copysign((signx==one)?PIo4:3*PIo4,signy));
            else               
                return(copysign((signx==one)?zero:PI,signy));
      }
        if((m=is_special(y))!=0)
          switch(m) {
          case nan:return y;
          case inf:case minf:return(copysign(PIo2,signy));
      }
  
    /* if x is 1.0 */
        if(x==1.0 ) { 
           y=copysign(y,one); t=y;
        }
        else {
                /* when y = 0 */
                if(y==zero) if(signx==one) return(y); else return(copysign(PI,signy));
                if(x==zero) return(copysign(PIo2,signy));
                x=copysign(x,one); 
                y=copysign(y,one); 
                if((m=(k=logb(y))-logb(x)) > 60) t=big+big; 
                        else if(m < -80 ) t=y/x;
                        else { t = y/x ; y = ldexp(y,-k); x=ldexp(x,-k); }
    }
    /* begin argument reduction */
        if (t < 2.4375 ) {                

        /* truncate 4(t+1/16) to integer for branching */
            k = 4 * (t+0.0625 );
             switch (k) {

            /* t is in [0,7/16] */
            case 0:                    
            case 1:
                if (t < small) {
                  /*   if(signx>zero) {
                      return (copysign(t,signy)); 
                     }
                     else {
                       return(copysign(PI-t,signy));
                     } */
                     return (copysign((signx>zero)?t:PI-t,signy));
                }  
                        
                hi = zero;  lo = zero;  break;

            /* t is in [7/16,11/16] */
            case 2:                    
                hi = athfhi; lo = athflo;
                z = x+x;
                t = ( (y+y) - x ) / ( z +  y ); break;

            /* t is in [11/16,19/16] */
            case 3:                    
            case 4:
                hi = PIo4; lo = zero;
                t = ( y - x ) / ( x + y ); break;

            /* t is in [19/16,39/16] */
            default:                   
                hi = at1fhi; lo = at1flo;
                z = y-x; y=y+y+y; t = x+x;
                t = ( (z+z)-x ) / ( t + y ); break;
            }
        }
        /* end of if (t < 2.4375) */

        else                           
        {
            hi = PIo2; lo = zero;

            /* t is in [2.4375, big] */
            if (t <= big)  t = - x / y;

            /* t is in [big, INF] */
            else          
              { 
                t = zero; }
        }
    /* end of argument reduction */

    /* compute atan(t) for t in [-.4375, .4375] */
        z = t*t;
        z = t*(z*(a1+z*(
                  a2+z*(
                  a3+z*(
                  a4+z*(
                  a5+z*(
                  a6+z*(
                  a7+z*(
                  a8+z*(
                  a9+z*(
                  a10+z*(
#if EXTENDED_PRECISION_ARCTAN 
                  a11+z*(
                  a12+z*(
                  a13+z*(
                  a14+z*(
                  a15+z*(
                  a16+z*(
                  a17+z*(
                  a18+z*(
                  a19+z*(
                  a20+z*(
                  a21
                  ))))))))))
#else
                  a11
#endif
                  ))))))))))));

        z = lo - z; z += t; z += hi;
     /*   if(signx > 0.0 ) {
           return copysign(z,signy);
        }
        else {
           return copysign(PI-z,signy);
        } */
      return(copysign((signx>zero)?z:PI-z,signy));   
}


double asin(double x)
{

        unsigned r;
        
         if((r=is_special(x))!=0) {
             switch(r) {
                case nan:return x;
                case inf:{
                   errno = EDOM;
                   return DOUBLE_NAN;
                }
                case minf:{
                   errno = EDOM;
                   return DOUBLE_NAN;
                }
             }
        }
        if(fabs(x) > 1.0 ) {
                errno = EDOM;
                return DOUBLE_NAN;
        }
        return(atan2(x,sqrt(1.0 -x*x)));
}


double acos(double x)
{
        double t;
    unsigned r;
        
        if((r=is_special(x))!=0) {
                switch(r) {
                case nan:return x;
                case inf:{
                   errno = EDOM;
                   return DOUBLE_NAN;
                }
                case minf:{
                   errno = EDOM;
                   return DOUBLE_NAN;
                }
       }
    }
        if(fabs(x) > 1.0 ) {
                errno = EDOM;
                return DOUBLE_NAN;
        }
        if( x != -1.0 )
            t=atan2(sqrt((1.0 -x)/(1.0 +x)),1.0 );
        else
            /* t=atan2(1.0 ,0.0 );  t = PI/2 */
t=PIo2;
        return(t+t);
}

double atan(double x)
{
        return(atan2(x,1.0 ));
}

/*************************************************************/
double sncsh(double y, int flg)
{
  double w, z;

  if (y > _M_EXP_MAX)
    {
      w = y - 0.69316101074218750000 ;
      if (w > _M_EXP_MAX)
        errno = ERANGE;
        /* range err */;
      z = exp(w);
      return (z + 0.13830277879601902638e-4  * z);
    }
  z = exp(y);
  if(flg) 
    return ((z - 1/z)/2);
  return ((z + 1/z)/2);
}

/*****************************************************/

double sinh(double x)
{
  unsigned r;
  double y, f, pf, qf, rf, sign;

  if((r=is_special(x))!=0) {
     switch(r) {
     case nan:return x;
     case inf:{
       errno = ERANGE;
       return DOUBLE_INF;
     }
     case minf:{
       errno = ERANGE;
       return -DOUBLE_INF;
     }
     }
  }
  sign=copysign(1.0,x);
  y=copysign(x,1.0);
  if (y > 1.0)
      return copysign(sncsh(y,1),sign); 
  if (y < _M_EPS)
    return (x);
  f = y * y;
  pf = ((-0.78966127417357099479  * f - 0.16375798202630751372e+3 ) * f
        - 0.11563521196851768270e+5 ) * f - 0.35181283430177117881e+6 ;

  qf = ((f - 0.27773523119650701667e+3 ) * f
           + 0.36162723109421836460e+5 ) * f
           - 0.21108770058106271242e+7 ;

  rf = f * (pf/qf);

  return (x + x * rf);
}
/*****************************************************/

double cosh(double x)
{
 unsigned r;
  
  if((r=is_special(x))!=0) {
     switch(r) {
     case nan:return x;
     case inf:{
       errno = ERANGE;
       return DOUBLE_INF;
     }
     case minf:{
       errno = ERANGE;
       return DOUBLE_INF;
     }
     }
  }
  x=fabs(x);
  return (sncsh(x,0));
}

/*****************************************************/

double tanh(double x)
{
  unsigned r;
  double sign,f, g, gpg, qg, rg, rv;

  if((r=is_special(x))!=0) {
     switch(r) {
     case nan:return x;
     case inf:{
       return 1.0;
     }
     case minf:{
       return -1.0;
     }
     }
  }
  sign=copysign(1.0,x);
  f=copysign(x,1.0);
  if (f < _M_EPS) 
    return (x);

  if (f > 19.06154747 )
  {
    return copysign(1.0,sign);
  }
  if (f > 0.54930614433405484570 )
    {
      rv = 0.5  - 1.0 /(exp(f + f) + 1.0 );
      rv = ldexp(rv,1);
      return copysign(rv,sign);
    }

  g = f * f;

  gpg = ((-0.96437492777225469787  * g
         - 0.99225929672236083313e+2 ) * g
         - 0.16134119023996228053e+4 ) * g;

  qg = ((g + 0.11274474380534949335e+3 ) * g
           + 0.22337720718962312926e+4 ) * g
           + 0.48402357071988688686e+4 ;

  rg = gpg / qg;
  rv = f + f * rg;
  return copysign(rv,sign);
}

/***********************************************************************/
double ceil    (double x)
{
  double y;
  
  (void)modf(x, &y);
  if (x != y && x > 0.0 ) return (y + 1.0 );
  return(y);
}

double floor   (double x)
{
  double y;
  
  (void)modf(x, &y);
  if (x != y && x < 0.0 ) return (y - 1.0 );
  return(y);
}


double fmod    (double x, double y)
{ 
  double z;
  
  if (y == 0.0 ) return (0.0 );
  (void) modf (x / y, &z);
  return (x - z * y);
}

/****************************************************************/


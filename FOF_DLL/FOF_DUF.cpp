/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_duff
* Desc: functions for doing duff calcuations
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include   "fof_ci.h"
#include   "fof_duf.h"
#include   "fof_util.h"

#include "fof_ansi.h"

//int Pocosin_Duff (d_CI *a_CI, d_DUF *a_DUF);
//float  _Equ_20_Pocosin (d_CI *a_CI, d_DUF *a_DUF);

//int Ceil (float f);



/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
#define   e_Adj   1.4                /* Adjust for Duff Code: Adj-NFDR       */

float _dufPer (float red, float dep);
void  _defRed (d_CI *a_CI, d_DUF *a_DUF);

/**************************************************
*
***************************************************/
void  _defRed (d_CI *a_CI, d_DUF *a_DUF)
{
float p;
  p = a_DUF->f_Per / 100.0; 
  a_DUF->f_Red =  p * a_CI->f_DufDep;
} 


/********************************************************
* get percent of duff depth reduction / total duff depth 
********************************************************/
float _dufPer (float red, float dep)
{
float f; 
   if ( dep <= 0 ) 
     return 0;
   f = (red / dep) * 100.0;
   return f; 
}

/*******************************************************
* Name: BUE_Duff 
* --->  this is used by Batch, there's a different function
*       used for the GUI processing stuff  
* Desc: Calc duff amount consumed, depth reduction and MSE based 
*        on given eq numbers
* NOTE: When doing batch the user is allowed to select any
*       amt or red equation to calculate values for  
*       amt or red. See code below 
* Note-1: When using a depth reduction equation to calc 
*         a percent consumned amount, we calc amount of duff 
*         depth reduction (inches) and then use that with 
*         the pre-fire duff depth to calc the percent consumed. 
*         And do a similar thing to get depth red when using
*         a consumed amount equation.  
*
*     The return/Out values are based on the User Equation
*     numbers that the user supplied in the input file, if the 
*     user left it blank or -1 then there is not equation number
*     to be used so -1 is return for the Out value
*  Out: af_Per....Duff consumed percent or -1
*       af_Red....Duff Depth Reduction percent or -1
*       af_MSE....Mineral Soil exposed or -1
*     
********************************************************/
int  BUE_Duff (d_CI *a_CI,  float *af_Per, float *af_Red, float *af_MSE, char cr_ErrMes[])
{
int i, i_PerEqu; 
float f_Per; 
char CT[100]; 
d_DUF DUF;

   strcpy (cr_ErrMes,""); 
   DUF_Init (&DUF);

   *af_Per = -1.0;
   *af_Red = -1.0;
   *af_MSE = -1.0; 

/* Cover Type - might need this... */
   strcpy (CT,"");                    
   if ( CI_isRedJacPin(a_CI))         /* Red Pine - Jack Pine         */
     strcpy (CT, "JACK"); 
   if ( CI_isBalBRWSpr (a_CI))        /* Balsam, Blk Red Whit Spruce  */
     strcpy (CT, "BALSAM"); 

    if ( a_CI->i_EgDufLoa <= 0 ) 
      goto Red; 
/*........................................*/
/* Duff Load Consumption percent equations */
    switch (a_CI->i_EgDufLoa ) {
      case 1:
        Equ_1_Per (a_CI, &DUF);
        break;
      case 2:
        Equ_2_Per (a_CI, &DUF);
        break;
      case 3:
        Equ_3_Per (a_CI, &DUF);  
        break; 
      case 4:
        Equ_4_Per (a_CI, &DUF);
        break;
 
      case 5:
        Equ_5_Red (a_CI, &DUF);      /* See Note-1 above */
        DUF.f_Per = _dufPer (DUF.f_Red,a_CI->f_DufDep);
        break; 

      case 6:
        Equ_6_Red (a_CI, &DUF);     /*  See Note-1 above */
        DUF.f_Per = _dufPer (DUF.f_Red,a_CI->f_DufDep);
        break; 

      case 7:
        Equ_7_Red (a_CI, &DUF);  /*  See Note-1 above */
        DUF.f_Per = _dufPer (DUF.f_Red,a_CI->f_DufDep); 
        break; 

      case 15:
        Equ_15_RedPer(a_CI, &DUF, CT);
        break; 

      case 16:
        Equ_16_Per (a_CI, &DUF);
        break; 
  
      case 17:
        Equ_17_Per (&DUF);
        break; 

      case 19:
        Equ_19_Per (a_CI, &DUF);
        break; 

      case 20:
        Equ_20_PerRed_Pocosin (a_CI, &DUF);
        break; 

      default:
        sprintf (cr_ErrMes,"Invalid duff load consumption equation specified: %d\n",a_CI->i_EgDufLoa);
        strcat  (cr_ErrMes,"    Valid Numbers: 1, 2, 3, 4, 5, 15, 16, 17, 20");
        return 0;
    } /* switch */

/* Send out the Percent of duff consumed */
    *af_Per = DUF.f_Per;


/*.....................................................*/
/* Duff Depth reduction - inches of duff removed       */
Red:
    if ( a_CI->i_EqDufDep <= 0 ) 
       goto MSE;

    switch ( a_CI->i_EqDufDep ) {
      case 1:
        Equ_1_Per (a_CI, &DUF);          /*  See Note-1 above */
       _defRed (a_CI, &DUF); 
        break; 
      case 2:
        Equ_2_Per (a_CI, &DUF);           /*  See Note-1 above */ 
       _defRed (a_CI, &DUF); 
        break; 
      case 3:
        Equ_3_Red (a_CI, &DUF);
        break;
      case 4:
        Equ_4_Per (a_CI, &DUF);           /*  See Note-1 above */ 
       _defRed (a_CI, &DUF); 
        break; 
      case 5:
        Equ_5_Red (a_CI, &DUF);   
        break; 
      case 6:
        Equ_6_Red (a_CI, &DUF);
        break; 
      case 7:
        Equ_7_Red (a_CI, &DUF);
        break; 
      case 19:
        Equ_19_Red (a_CI, &DUF);
        break; 
  
      case 15:                          
        Equ_15_RedPer(a_CI, &DUF, CT);  
        break; 

      case 16:
        Equ_16_Per (a_CI, &DUF);           /*  See Note-1 above */ 
       _defRed (a_CI, &DUF); 
        break; 

      case 17:
        Equ_17_Per (&DUF);           /*  See Note-1 above */ 
       _defRed (a_CI, &DUF); 
        break; 

      case 20:
        Equ_20_PerRed_Pocosin (a_CI, &DUF);
        break; 
 
      default:
         sprintf (cr_ErrMes,"Invalid duff depth reduction equation specified: %d\n",a_CI->i_EqDufDep);
         strcat  (cr_ErrMes,"    Valid Numbers:  3, 5, 6, 7, 15, 20"); 
         return 0; 
     } /* switch */

/* Send out the duff depth reduction */
    *af_Red = DUF.f_Red;

/* Mineral Soil Exposed */ 
MSE:
   if ( a_CI->i_EqMSE <= 0 ) 
     goto Ret; 

   switch ( a_CI->i_EqMSE ) {
     case 9:
       Equ_9_MSE (a_CI, &DUF);
       break; 
     case 10: 
       Equ_10_MSE (a_CI, &DUF);
       break;  
     case 11:
       Equ_11_MSE (a_CI, &DUF);
       break; 
     case 12:
       Equ_12_MSE (a_CI, &DUF);
       break; 
     case 13:
       Equ_13_MSE (a_CI, &DUF);
       break;
     case 14:
       Equ_14_MSE (&DUF);
       break; 
     case 18:
       Equ_18_MSE (&DUF);
       break;
     case 19:
       Equ_19_MSE (&DUF);
       break;


     case 202:
       Equ_202_MSE (&DUF);
       break; 
     default:
       sprintf (cr_ErrMes,"Invalid mineral soil exposed equation specified: %d\n",a_CI->i_EqMSE);
       strcat  (cr_ErrMes,"    Valid Numbers: 9, 10, 11, 12, 13, 14, 18, 202"); 
       return 0; 
    }
   *af_MSE = DUF.f_MSEPer; 

Ret:
   return 1; 
}



/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: DUF_Mngr
* Desc: Do Duff Calcs
* Note-1: Doing a check to make sure the percent stays with in limits of
*          0 to 100, I think the old Fortran code did this, I guess some
*          of the Equations can go out of limits.
* Note-2: I put this check in here because, some of the calcs for duff dep
*         reduction will calc an amount even when the duff dep that comes
*         in is 0
* Note-3: This prevents the amount of duff depth reduction from exceeding
*          the orginal duff depth, some of the Equations will calc the
*          reduction to be more than what it starts with, so I check
*          it here before its time to return to caller
* Note-4: When there is 0 duff load on input then the Mineral Soil Exposed
*          (MSE) should be set to 100, when duff load is 0 so is the duff depth
*          input, the MSE equations make calcs based on Duff moist and come
*          up with varing answers, SO I ask ER about it decied to plug in
*          100 percent when duff and depth = 0
* Note-5: The Duff Reduction Equations are no longer used to calculate duff
*          depth reduction.
*         The results from Duff Reduction equations were always inconsistant 
*         with the results from the Duff Consumed equations, and the output 
*         show to the user were confusing, and it got even more confusing when
*         we started doing the improved soil simulation and show those results
*         DL said stop using the Reduction eqs and just base the reduction 
*         based on the amounts consumed.
*         The easiest way was to just put that line of code at the end of this
*         function, and leave everything else in place - just incase we want to
*         use it again in the future. 
*   In: a_CI........Consume Input struct
*  Out: a_DUF
*       a_DUF->cr_ErrMes if error
*  Ret: 1 ok
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  DUF_Mngr (d_CI *a_CI, d_DUF *a_DUF )
{
float f; 
float f_Per, f_Red, f_MSE;
char cr_ErrMes[1000]; 

/* NOTE NOTE NOTE...... */
/* Duff Depth - see the calc at the bottom of this function */
/*  and see Note-5 above */

   if ( CI_isPiles(a_CI) ) {              /* Fuel Category: Piles, all Regions */
     Equ_17_Per (a_DUF);
     a_DUF->f_Red = 0;                   /* There is no Duff Reduction Eq     */
     a_DUF->i_RedEqu = 0;                /*  for this                         */
     Equ_18_MSE (a_DUF);
     goto X; }

   if ( CI_isShrubGroupChaparral(a_CI) ) {  /* Chaparral consumes 100 percent */
     Equ_19_Per (a_CI,a_DUF);               /* Added 4-26-17 */
     Equ_19_Red (a_CI,a_DUF); 
     Equ_19_MSE(a_DUF); 
     goto X; }

   if ( CI_isInteriorWest (a_CI) ) {
     DUF_InteriorWest (a_CI, a_DUF);
     goto X; }

   if ( CI_isPacificWest(a_CI) ) {
     DUF_PacificWest (a_CI, a_DUF);
     goto X; }

   if ( CI_isNorthEast(a_CI) ) {
     DUF_NorthEast (a_CI, a_DUF);
     goto X; }

   if ( CI_isSouthEast (a_CI) ) {
     DUF_SouthEast (a_CI, a_DUF);
     goto X; }

X:
/* If we're running a Batch and we have user assigned equations numbers */ 
  if ( !xstrcmpi( a_CI->cr_BatchEqu, "Yes" ) ) {
    BUE_Duff (a_CI, &f_Per, &f_Red, &f_MSE, cr_ErrMes); 
    if ( f_Per >= 0 ) {
      a_DUF->f_Per = f_Per;
      a_DUF->i_PerEqu = a_CI->i_EgDufLoa; }
    if ( f_Red >= 0 ) {
      a_DUF->f_Red = f_Red;
      a_DUF->i_RedEqu = a_CI->i_EqDufDep; }
    if ( f_MSE >= 0 ) {
      a_DUF->f_MSEPer = f_MSE;
      a_DUF->i_MSEEqu= a_CI->i_EqMSE; }
   }

/*-------------------------------------------------*/
/* Change 7-29-2012                                */ 
/* We want a Duff Moist of 10 to consume all duff  */
/*  10 is the lowest allowable moist amount        */
/* this change was put in place to deal with the   */
/* duff soil heating problem                       */
   if ( a_CI->f_MoistDuff <= 10.0 ) { 
     a_DUF->f_Per = 100.0;
     a_DUF->f_Red = a_CI->f_DufDep;
     goto MSE; }

  if ( a_DUF->f_Per < 0 )                    /* see Note-1 above             */
     a_DUF->f_Per = 0;
   if ( a_DUF->f_Per > 100 )
     a_DUF->f_Per = 100;

   if ( a_DUF->f_Red > a_CI->f_DufDep )      /* see Note-3                   */
      a_DUF->f_Red = a_CI->f_DufDep;

   if ( a_CI->f_DufDep <= 0 )                /* see Note-2 above             */
      a_DUF->f_Red = 0;

MSE: 
/* Change 9-28-05, see Note-4 above                                          */
   if ( a_CI->f_Duff <=0 )                   /* No Duff Load                 */
      a_DUF->f_MSEPer = 100.0;


/* If using Batch User Equations */ 
  if ( !xstrcmpi( a_CI->cr_BatchEqu, "Yes" ) ) 
     return 1; 

/*---------------------------------------------------*/
/* 03-11-2016 See Note-5 above                       */
/* Running off GUI or Batch Non-User Equation  */
   a_DUF->f_Red = a_CI->f_DufDep * ( a_DUF->f_Per / 100.0 ); 

  return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: DUF_SouthEast
* Desc: South East
* Note-1: These are the old functions we used, the actual functions are 
*         still in this file but #ifdef'd out
*   In: a_CI.......Fuel Loading struct
*  Out: a_DUF.......Duff Values Struct, loads values and equ #
*  Ret: 1 Calcs done,    0 Fuel Category was Piles
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int DUF_SouthEast (d_CI *a_CI, d_DUF *a_DUF)
{

  if ( CI_isPocosin(a_CI)){                  /*  Pond Pine - Pocosin         */
    Equ_20_PerRed_Pocosin (a_CI, a_DUF); 
//   Pocosin_Duff (a_CI, a_DUF);   /* See Note-1 Above */  
//   Equ_20_RedPer (a_CI, a_DUF);
     Equ_202_MSE (a_DUF);                     /* Soil Exposure                */
    return 1; }

/* test larry  */
  if ( CI_isCoastPlain(a_CI)){
    Equ_CP_Per (a_CI, a_DUF);
    Equ_CP_Red (a_CI, a_DUF);
    Equ_CP_MSE  (a_DUF);
    return 1;   }


  Equ_16_Per (a_CI, a_DUF);
  a_DUF->f_Red = (a_CI->f_DufDep * a_DUF->f_Per) / 100;
  a_DUF->i_RedEqu = 16;
  Equ_14_MSE (a_DUF);                        /* Soil Exposure                */
  return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: DUF_NorthEast
* Desc:
*   In: a_CI.......Fuel Loading struct
*  Out: a_DUF.......Duff Values Struct, loads values and equ #
*  Ret: 1 Calcs done,    0 Fuel Category was Piles
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int DUF_NorthEast (d_CI *a_CI, d_DUF *a_DUF)
{

  if ( CI_isWhiPinHem(a_CI) ) {              /* White Pine - Hemlock         */
     DUF_InteriorWest (a_CI, a_DUF);
     return 1; }

  if ( CI_isRedJacPin(a_CI)){                /* Red Pine - Jack Pine         */
    goto Jack_Red_Pine; }

  if ( CI_isBalBRWSpr (a_CI) ) {                  /* Balsam, Blk Red Whit Spruce  */
    goto Balsam_Fir_Spruce; }

  Duf_Default(a_CI,a_DUF);                  /* if Nothing else to use       */
  return 1;

/*._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._.*/
Jack_Red_Pine:
   if ( CI_isDufEntire(a_CI)) {                /* Duff Code - Entire           */
     Equ_15_RedPer (a_CI,a_DUF,"JACK");
     Equ_14_MSE (a_DUF); }
   else if ( CI_isDufLower(a_CI)){
      Duf_Default(a_CI,a_DUF);                  /* if Nothing else to use       */
      return 1; }
   else {
     Equ_3_Red (a_CI,a_DUF);
     Equ_14_MSE (a_DUF); }
   return 1;

/*._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._.*/
Balsam_Fir_Spruce:
   if ( CI_isDufLower(a_CI)) {               /* Duff Code - Lower            */
     Equ_5_RedPer (a_CI,a_DUF);
     Equ_14_MSE (a_DUF); }
   else if ( CI_isDufEntire(a_CI)) {         /* Duff Code - Entire           */
     Equ_15_RedPer (a_CI,a_DUF,"BALSAM");
     Equ_14_MSE (a_DUF); }
   else {                                    /* Duff Code - NFDR & Adj-MFDR  */
     Equ_3_Red (a_CI,a_DUF);
     Equ_14_MSE (a_DUF); }

   return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: DUF_PacificWest
* Desc: Duff Calculations for the Pacific  West Region
* Note-1: This was changed when burnup code was put in program, this
*          use to call equation 8 which used woody fuel to calc duff
*          and we didn't want to do that
* NOTE: Does NOT do Fuel Category Piles, do Slash & Natural only
*       Piles should have been done before coming here.
*   In: a_CI.......Fuel Loading struct
*  Out: a_DUF.......Duff Values Struct, loads values and equ #
*  Ret: 1 Calcs done,    0 Fuel Category was Piles
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int DUF_PacificWest  (d_CI *a_CI, d_DUF *a_DUF)
{

  if ( CI_isPiles(a_CI) )                    /* Piles should have been done  */
    return 0;                                /*  by caller                   */

/* Fuel Category:  Slash ................................................... */
  if ( CI_isSlash(a_CI)) {
    DUF_InteriorWest (a_CI,a_DUF);          /* Note-1                       */
    return 1; }

/* Natural - Ponderosa.......................................................*/

  if ( CI_isPonderosa (a_CI)){               /* if Ponderosa Cover Type      */
    if ( CI_isDufLower (a_CI)){              /* Duff Code - Lower            */
      Equ_4_Per (a_CI,a_DUF);
      Equ_5_Red (a_CI,a_DUF);
      Equ_13_MSE(a_CI,a_DUF);               /* Soil Exposure                */
      return 1; }

    if ( CI_isDufEntire(a_CI)){             /* Duff Code - Entire           */
      Equ_2_Per (a_CI,a_DUF);
      Equ_6_Red (a_CI,a_DUF);
      Equ_10_MSE (a_CI,a_DUF);              /* Soil Exposure                */
      return 1;}

    Equ_3_Per (a_CI,a_DUF);                 /* Duff Code - NFDR & Adj-NFDR  */
    Equ_7_Red (a_CI,a_DUF);
    Equ_12_MSE(a_CI,a_DUF);
    return 1;}

/* Natural - Other Cover Types (non-Pondersoa)...............................*/
  if ( CI_isDufLower(a_CI)){                /* Duff Code - Lower            */
    Equ_1_Per (a_CI,a_DUF);
    Equ_5_Red (a_CI,a_DUF);
    Equ_13_MSE(a_CI,a_DUF);                 /* Soil Exposure                */
    return 1;}

  if ( CI_isDufEntire(a_CI)) {               /* Duff Code - Entire           */
    Equ_2_Per (a_CI,a_DUF);
    Equ_6_Red (a_CI,a_DUF);
    Equ_10_MSE (a_CI,a_DUF);                /* Soil Exposure                */
    return 1;}

  Equ_3_Per (a_CI,a_DUF);                 /* Duff Code - NFDR & Adj-NFDR  */
  Equ_7_Red (a_CI,a_DUF);
  Equ_12_MSE(a_CI,a_DUF);
  return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: DUF_InteriorWest
* Desc: Duff Calculations for the Interior West Region
* NOTE: Does NOT do Fuel Category Piles, do Slash & Natural only
*       Piles should have been done before coming here.
*   In: a_CI.......Fuel Loading struct
*  Out: a_DUF.......Duff Values Struct, loads values and equ #
*  Ret: 1 Calcs done,    0 Fuel Category was Piles
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int DUF_InteriorWest  (d_CI *a_CI, d_DUF *a_DUF)
{

  if ( CI_isPiles(a_CI))                    /* Piles should have been done  */
    return 0;                                /*  by caller                   */

  if ( CI_isNatural(a_CI) )                 /* go do Natural                */
    goto Natural;

/* Fuel Category:  Slash ................................................... */
  if ( CI_isSlash(a_CI) ) {
    if ( CI_isDufLower (a_CI)) {             /* Duff Code - Lower            */
      Equ_1_Per (a_CI,a_DUF);
      Equ_5_Red (a_CI,a_DUF);
      Equ_9_MSE (a_CI,a_DUF);                /* Soil Exposure                */
      return 1; }

    if ( CI_isDufEntire(a_CI)) {             /* Duff Code - Entire           */
      Equ_2_Per (a_CI,a_DUF);
      Equ_6_Red (a_CI,a_DUF);
      Equ_10_MSE (a_CI,a_DUF);               /* Soil Exposure                */
      return 1; }

    Equ_3_Per (a_CI,a_DUF);                  /* Duff Code - NFDR & Adj-NFDR  */
    Equ_7_Red (a_CI,a_DUF);
    Equ_11_MSE(a_CI,a_DUF);
    return 1; }

/* Fuel Category:  Natural.................................................. */
Natural:

/* Ponderosa Cover Type, Fuel Category Natural ..............................*/
  if ( CI_isPonderosa (a_CI)){               /* Is it a Ponderosa Cover Type */
    if ( CI_isDufLower (a_CI)) {             /* Duff Code - Lower            */
      Equ_4_Per (a_CI,a_DUF);
      Equ_5_Red (a_CI,a_DUF);
      Equ_13_MSE(a_CI,a_DUF);                /* Soil Exposure                   */
      return 1; }

    if ( CI_isDufEntire(a_CI)){              /* Duff Code - Entire              */
      Equ_2_Per (a_CI,a_DUF);
      Equ_6_Red (a_CI,a_DUF);
      Equ_10_MSE(a_CI,a_DUF);                /* Soil Exposure                   */
      return 1; }

    Equ_3_Per (a_CI,a_DUF);                  /* Duff Code - NFDR & Adj-NFDR     */
    Equ_7_Red (a_CI,a_DUF);
    Equ_12_MSE(a_CI,a_DUF);
    return 1; }

/* Non-Ponderosa Cover Type .................................................*/
  if ( CI_isDufLower (a_CI)) {               /* Duff Code - Lower            */
    Equ_1_Per (a_CI,a_DUF);
    Equ_5_Red (a_CI,a_DUF);
    Equ_13_MSE(a_CI,a_DUF);
    return 1;}

  if ( CI_isDufEntire(a_CI)) {               /* Duff Code - Entire           */
    Equ_2_Per (a_CI,a_DUF);
    Equ_6_Red (a_CI,a_DUF);
    Equ_10_MSE(a_CI,a_DUF);
    return 1;}

  Equ_3_Per (a_CI,a_DUF);
  Equ_7_Red (a_CI,a_DUF);
  Equ_12_MSE(a_CI,a_DUF);
  return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_1_Per
* Desc: Calculate Percent of Duff Amount Consumed 
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  Equ_1_Per (d_CI *a_CI, d_DUF *a_DUF)
{
   if ( a_CI->f_MoistDuff <= 160 )
     a_DUF->f_Per = 97.1 - (0.519 * a_CI->f_MoistDuff);
   else
     a_DUF->f_Per = 13.6;
   a_DUF->i_PerEqu = 1;

   if ( a_DUF->f_Per < 0 )
     a_DUF->f_Per = 0;

}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_2_Per
* Desc: Calculate Percent Duff Amount Consumed 
* Note-1: The percent from this calculation can go below 0 when the
*          #Moisture-Conditions = wet, the DuffMoist gets set to 200
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  Equ_2_Per (d_CI *a_CI, d_DUF *a_DUF)
{
   a_DUF->f_Per = 83.7 - (0.426 * a_CI->f_MoistDuff);
   a_DUF->i_PerEqu = 2;
   if (a_DUF->f_Per < 0 )       /* see Note-1 above */
    a_DUF->f_Per = 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_3_Red
* Desc: Calculate Percent of Duff Depth based on the Percent Amount consumed
*   In: a_CI....Fuel Input Value struct
*  Out: a_DUF......calulated percent & amount of reduction
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  Equ_3_Red (d_CI *a_CI, d_DUF *a_DUF)
{
   Equ_3_Per (a_CI, a_DUF);     /* Do the Percent                  */

/* Now use the Percent to get the Amount of Depth Reduction in Inches        */
   a_DUF->f_Red  =  ( a_CI->f_DufDep * a_DUF->f_Per ) / 100;
   a_DUF->i_RedEqu = 3;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_3_Per
* Desc: Calculate Percent of Duff Amount Consumed 
*   In: a_CI....Fuel Input Value struct
*  Out: a_DUF......calulated percent & equation # is put in
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  Equ_3_Per (d_CI *a_CI, d_DUF *a_DUF)
{
float f_Moist;

// Change 4-27-17 DL, use 1000k moist and no adjustment
//   f_Moist = a_CI->f_MoistDuff;    /* See Define definition                 */
//   if ( CI_isDufAdjNFDR(a_CI) )
//     f_Moist = f_Moist / e_Adj;
   f_Moist = a_CI->f_MoistDW1000; 

   a_DUF->f_Per = 114.7 - (4.2 * f_Moist);
   a_DUF->i_PerEqu = 3;
   if ( a_DUF->f_Per < 0 )
     a_DUF->f_Per = 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_4_Per
* Desc: Calculate Percent of Duff Amount Consumed
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  Equ_4_Per (d_CI *a_CI, d_DUF *a_DUF)
{
   a_DUF->f_Per = 89.9 - (0.55 * a_CI->f_MoistDuff);
   a_DUF->i_PerEqu = 4;
   if ( a_DUF->f_Per < 0 )
     a_DUF->f_Per = 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_5_RedPer
* Desc: Calculate the percent amount of duff consumed using
*       the duff depth reduction Equation 
*  Out:  NOTE: this populates BOTH a_DUF->f_Per f_Red
*              and equation numbers
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void Equ_5_RedPer (d_CI *a_CI, d_DUF *a_DUF)
{
  Equ_5_Red (a_CI,a_DUF);          /* Calc Amount Depth Inches              */
  if ( a_CI->f_DufDep > 0 )
    a_DUF->f_Per = (a_DUF->f_Red / a_CI->f_DufDep) * 100;
  else
    a_DUF->f_Per = 0;
  a_DUF->i_PerEqu = 5;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_5_Red
* Desc: Calculate Amount of Duff Depth Reduction in inches
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  Equ_5_Red (d_CI *a_CI, d_DUF *a_DUF)
{
  a_DUF->f_Red = 1.028 - (0.0089 * a_CI->f_MoistDuff)
                       + ( 0.417 * a_CI->f_DufDep) ;
  a_DUF->i_RedEqu = 5;
  if ( a_DUF->f_Red < 0 )
    a_DUF->f_Red = 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_6_Red
* Desc: Calculate Amount of Duff Depth Reduction in inches
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  Equ_6_Red (d_CI *a_CI, d_DUF *a_DUF)
{
  a_DUF->f_Red = 0.8811 - (0.0096 * a_CI->f_MoistDuff)
                        + (0.439  * a_CI->f_DufDep) ;
  a_DUF->i_RedEqu = 6;

  if ( a_DUF->f_Red < 0 )
    a_DUF->f_Red = 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_6_RedPer
* Desc: Calculate the amount of duff depth reduction and also
*       calculate the percent of duff load consumed
* NOTE: how this call has to call the depth reduction function
*       first to use the result in getting the load consumed. 
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void Equ_6_RedPer (d_CI *a_CI, d_DUF *a_DUF)
{
d_DUF D;

  Equ_6_Red (a_CI,&D);          /* Calc Amount Depth Inches              */
  if ( a_CI->f_DufDep > 0 )
    a_DUF->f_Per = (D.f_Red / a_CI->f_DufDep) * 100;
  else
    a_DUF->f_Per = 0;
  a_DUF->i_PerEqu = 5;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_7_Red
* Desc: Calculate Amount of Duff Reduction in Inches
*   In: a_CI....Fuel Input Value struct
*  Out: a_DUF......calulated percent & equation # is put in
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  Equ_7_Red (d_CI *a_CI, d_DUF *a_DUF)
{
float f_Moist;

// Change 4-27-17 DL, use 1000k moist and no adjustment
//  f_Moist = a_CI->f_MoistDuff;
//  if ( CI_isDufAdjNFDR(a_CI) )
//    f_Moist = f_Moist / e_Adj;
  f_Moist = a_CI->f_MoistDW1000; 

  a_DUF->f_Red = 1.773 - (0.1051 * f_Moist)
                        + (0.399  * a_CI->f_DufDep) ;
  a_DUF->i_RedEqu = 7;
  if ( a_DUF->f_Red < 0 )
    a_DUF->f_Red = 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_15_PerRed
* Desc: Calculate Percent of Duff Reduction Inches & Percent
* NOTE-1: this equations calculates the residual duff, which I think is
*          the amount of duff depth remains after fire, not the amount
*          reducded by the fire, so have to subtract that from original
*          depth to get the amount of reduction
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  Equ_15_RedPer (d_CI *a_CI, d_DUF *a_DUF, char cr_Pine[])
{
int i_Pine;
float f_RDD;

  if  (!strcmp (cr_Pine,"JACK") )      /* Jack Pine                         */
    i_Pine = 1;
  else
    i_Pine = 0;                         /* other                             */

/* see NOTE-1 above,   get residual amount  ......................          */
  f_RDD = -0.791  + (0.004 * a_CI->f_MoistDuff)
      + (0.8   * a_CI->f_DufDep) + (0.56  * i_Pine);

  a_DUF->f_Red = a_CI->f_DufDep - f_RDD;  /* now get amount of reduction    */
  if ( a_DUF->f_Red > a_CI->f_DufDep )    /* don't reduce by more than we   */
    a_DUF->f_Red = a_CI->f_DufDep;        /*  start with                    */
  if ( a_DUF->f_Red < 0 )
    a_DUF->f_Red = 0;

  if ( a_CI->f_DufDep > 0 )
    a_DUF->f_Per =  (a_DUF->f_Red / a_CI->f_DufDep) * 100;
  else
    a_DUF->f_Per = 0;

  a_DUF->i_PerEqu = 15;
  a_DUF->i_RedEqu = 15;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_16_Per
* Desc: Percent Duff Amount Consumed 
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  Equ_16_Per (d_CI *a_CI, d_DUF *a_DUF)
{
float f_WPRE, f_W, f_L;

  f_WPRE = a_CI->f_Lit + a_CI->f_Duff + a_CI->f_DW10 + a_CI->f_DW1;

  if ( f_WPRE > 0 )
    f_W = 3.4958 + (0.3833 * f_WPRE) - (0.0237 * a_CI->f_MoistDuff)
          - (5.6075 / f_WPRE);
  else
    f_W = 0;

  f_L = a_CI->f_Lit + a_CI->f_DW10 + a_CI->f_DW1;
  if ( f_W <= f_L ) {
    a_DUF->f_Per = 0;
    goto PerEqu; }

  if ( (f_WPRE - f_L) != 0 )
     a_DUF->f_Per = 100 * ( (f_W - f_L) / (f_WPRE - f_L) );
  else
     a_DUF->f_Per = 0;

  if ( a_DUF->f_Per < 0 )
    a_DUF->f_Per = 0;

PerEqu:
  a_DUF->i_PerEqu = 16;

}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_9_MSE
* Desc: Calculate Percent or Mineral Soil Exposure
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void Equ_9_MSE  (d_CI *a_CI, d_DUF *a_DUF)
{
  if ( a_CI->f_MoistDuff <= 135 )
    a_DUF->f_MSEPer = 80 - (.507 * a_CI->f_MoistDuff);
  else
    a_DUF->f_MSEPer = 23.5 - (.0914 * a_CI->f_MoistDuff);
  a_DUF->i_MSEEqu = 9;

  if ( a_DUF->f_MSEPer < 0 )           /* Make sure don't go below 0        */
    a_DUF->f_MSEPer = 0;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_10_MSE
* Desc: Calculate Percent or Mineral Soil Exposure
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void Equ_10_MSE  (d_CI *a_CI, d_DUF *a_DUF)
{
   a_DUF->f_MSEPer = 167.4 - (31.6 * log(a_CI->f_MoistDuff) );
   a_DUF->i_MSEEqu = 10;
   if ( a_DUF->f_MSEPer < 0 )           /* Make sure don't go below 0        */
     a_DUF->f_MSEPer = 0;
}



/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_11_MSE
* Desc: Calculate Percent or Mineral Soil Exposure
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void Equ_11_MSE  (d_CI *a_CI, d_DUF *a_DUF)
{
float f_Moist;
  f_Moist = a_CI->f_MoistDuff;
  if ( CI_isDufAdjNFDR(a_CI) )
     f_Moist = f_Moist / e_Adj;
   a_DUF->f_MSEPer =  93.3 - ( 3.55 * f_Moist) ;
   a_DUF->i_MSEEqu = 11;
   if ( a_DUF->f_MSEPer < 0 )           /* Make sure don't go below 0        */
     a_DUF->f_MSEPer = 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_12_MSE
* Desc: Calculate Percent or Mineral Soil Exposure
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void Equ_12_MSE  (d_CI *a_CI, d_DUF *a_DUF)
{
float f_Moist;
   f_Moist = a_CI->f_MoistDuff;
  if ( CI_isDufAdjNFDR(a_CI) )
     f_Moist = f_Moist / e_Adj;
   a_DUF->f_MSEPer =  94.3 - ( 4.96 * f_Moist) ;
   a_DUF->i_MSEEqu = 12;

  if ( a_DUF->f_MSEPer < 0 )           /* Make sure don't go below 0        */
    a_DUF->f_MSEPer = 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_13_MSE
* Desc: Calculate Percent or Mineral Soil Exposure
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void Equ_13_MSE  (d_CI *a_CI, d_DUF *a_DUF)
{
   a_DUF->f_MSEPer =  60.4 - ( 0.44 * a_CI->f_MoistDuff) ;
   a_DUF->i_MSEEqu = 13;

  if ( a_DUF->f_MSEPer < 0 )           /* Make sure don't go below 0        */
    a_DUF->f_MSEPer = 0;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_14_MSE
* Desc: Calculate Percent or Mineral Soil Exposure
* NOTE: Duff Percent has to be done be comming
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void Equ_14_MSE  (d_DUF *a_DUF)
{
   a_DUF->f_MSEPer =  -8.98 + ( 0.44 * a_DUF->f_Per );
   a_DUF->i_MSEEqu = 14;
   if ( a_DUF->f_MSEPer < 0 )        /* Just in case */
     a_DUF->f_MSEPer = 0;
   if ( a_DUF->f_MSEPer > 100 )
     a_DUF->f_MSEPer = 100;

  if ( a_DUF->f_MSEPer < 0 )           /* Make sure don't go below 0        */
    a_DUF->f_MSEPer = 0;

}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_202_MSE
* Desc: Calculate Percent or Mineral Soil Exposure
* NOTE: Duff Percent has to be done be comming
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void Equ_202_MSE  (d_DUF *a_DUF)
{
   a_DUF->f_MSEPer =  0;
   a_DUF->i_MSEEqu = 202;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: DUF_Calc
* Desc: Calculate Duff Amount Consumed, Post, and Percent
* NOTE: this is done using the Percent amount in the DUF struct put
*       there by the Equation function.
* Note-1: See comments in the called function.
*   In: a_CI, a_DUF
*  Out: see code
*  Ret: equation number
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int DUF_Calc (d_CI *a_CI, d_DUF *a_DUF, float *af_Con, float *af_Post,
                float *af_Percent)
{
  if ( a_CI->f_Duff != 0 ) {
    *af_Con =  (a_CI->f_Duff * a_DUF->f_Per) / 100;
    *af_Post = a_CI->f_Duff - *af_Con;
    *af_Percent = a_DUF->f_Per; }
  else
    *af_Con = *af_Post = *af_Percent = 0;

  if ( *af_Con > a_CI->f_Duff )      /* don't consumed amount to be more    */
     *af_Con = a_CI->f_Duff;         /* then we started with amount         */

  return  a_DUF->i_PerEqu;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: DUF_GetDepRed
* Desc: Get the Depth Reduction Post and Percent, uses the amount of
*       Reduction (inches) put in the DUF struct by the Reduction
*       equation. Checks for any error message in the DUF that would say
*       no equation was available for use.
* Note-1: This is asking if a reduction equation was inplemented and used
*        This should always be the case. Even when there is no Reduction
*        equation for use, the Duff Percent equation should have been used
*        to fill in the DUF Reduction Depth fields.
*    In: a_CI........Consumed Input struct
*        a_DUF.......Duff Values Struct
*   Out: af_Post.....Post Depth in inches
*   Out: af_Per......Percent of Duff Depth reduction
*   Ret: 1 OK,  0 = found error message in DUF struct, meaning there was
*                    No Reduction Equation implemented,
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int DUF_GetDepRed(d_CI *a_CI, d_DUF *a_DUF, float *af_Post, float *af_Per)
{
  if ( strcmp (a_DUF->cr_ErrMess,"") )  /* if there is an error message      */
     return 0;                          /* so don't need to do any calcs     */

  if ( a_CI->f_DufDep == 0 ) {         /* No Duff amount to work with       */
    *af_Post = 0;
    *af_Per = 0;
     return 1; }

/* changed 1-5-00, decided to let this go thru, report will show Equation 0  */
/*  if ( a_DUF->i_RedEqu == 0 ) {             Note-1 above                   */
/*     strcpy (a_DUF->cr_ErrMess,"No Duff Reduction Equation Implemented");  */
/*     return 0; }                                                           */
/* ...........................                                               */

  *af_Post = a_CI->f_DufDep - a_DUF->f_Red;
  if ( a_CI->f_DufDep != 0 )
     *af_Per = (a_DUF->f_Red / a_CI->f_DufDep) * 100;
  else
     *af_Per = 0;
  return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Duf_Default
* Desc: This is the defualt Duff and Mineral Soil Reduction Equations
*       that get called when there is nothing else to use.
*       There where conditions in the code where there was no Equations
*        to use, so it was decided that the ones below should be the
*        defualt ones used.
* In/Out: a_CI, a_DUF
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  Duf_Default (d_CI *a_CI, d_DUF *a_DUF)
{
   Equ_2_Per (a_CI,a_DUF);
   Equ_6_Red (a_CI,a_DUF);
   Equ_10_MSE (a_CI,a_DUF);              /* Soil Exposure                */
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name:
* Desc:
*  Ret:
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void   DUF_Init (d_DUF *a_DUF)
{
 
   a_DUF->f_Per = 0;
   a_DUF->i_PerEqu = 0;
   a_DUF->f_Red = 0;
   a_DUF->i_RedEqu = 0;
   strcpy (a_DUF->cr_ErrMess,"");

   a_DUF->f_MSEPer = 0;
   a_DUF->i_MSEEqu = 0;
   strcpy (a_DUF->cr_MSEMess,"");

   a_DUF->f_PerLit = 0;
   a_DUF->i_LitEqu = 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_17_Per
* Desc: Calculate Percent Duff Amount Consumed 
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  Equ_17_Per (d_DUF *a_DUF)
{
   a_DUF->f_Per = 10;                  /* 10 percent                         */
   a_DUF->i_PerEqu = 17;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_18_MSE
* Desc: Calculate Percent or Mineral Soil Exposure
* NOTE: Duff Percent has to be done be comming
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void Equ_18_MSE  (d_DUF *a_DUF)
{
   a_DUF->f_MSEPer =  10;
   a_DUF->i_MSEEqu = 18;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_CP_Red       Coastal Plain
* Desc: Calc amount of Duff+Lit layer reduction
*       Calc amount of Duff layer reduction
*       This equations was developed by Geoff Wang, Clemson Univ. South
*         Carolina,
*       This equation works different that all other duff reduction eqs
*       in that it works on duff + litter depth,
* Note-1: Wang said to assume that there is 2 centimeters (20 millimeters)
*        of litter in the forest floor depth, so I check, and as long as
*        there is some litter (load) present then I add the 2 cm
*    In: a_CI->f_DufDep
*   Out: a_Duf->i_RedEqu..Equation number
*        a_Duf->f_Red.....Amuont of duff reduction
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  Equ_CP_Red (d_CI *a_CI, d_DUF *a_DUF)
{
float Y, Td, Pl, Ld, Dr,pL;
#define e_LitLay  20.0           /* 20 millimeter litter layer               */

  a_DUF->i_RedEqu = e_CP_RedEq;
  a_DUF->f_Red = 0;

  if ( a_CI->f_DufDep <= 0 )
     return;

  Td = InchToMilMeter (a_CI->f_DufDep); /* Tot depth in millimeters          */

  if ( a_CI->f_Lit > 0 )                /* See Note-1 above                  */
    Td  += e_LitLay ;                   /* 2 centimetr = 20 millimetrs       */

  Y = -9.939+(0.896 * Td)-(29.582 / Td);/* Amt of tot layer reduction       */

  if ( Y > Td )                         /* Shouldn't Happen, check  Consumd  */
    Y = Td;                             /* doesn't exceed, total depth       */

  if ( Y < 0 )                          /* See Note-1 above                  */
    Y = 0;

  Dr = Y - e_LitLay;                    /* take away the lit thickness       */
  if ( Dr < 0 )                         /* would mean lit wasn't all consmed */
    Dr = 0;

  a_DUF->f_Red = MilMeterToInch (Dr);   /* Amount of reductin in inches      */

}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name:  Equ_CP_Per      Coastal Plain
* Desc:  Duff AND Litter percent of reduction
*        This equations was developed by Geoff Wang, Clemson Univ. South
*         Carolina,
*        Unlike all other duff reduction equations this one also does
*         litter and uses litter moisture (not duff moisture)
* How it Works:
*        The eq gives the total amount consumed, from that we determine
*         how much litter was consumed and how much duff gets consumed.
*        We assume that any & all litter gets consumed before any duff.
*        So if:
*            prefire Lit = 5 and Duf = 5
*            Total Consumed = 2.5
*            Then, 50 percent of the litter is consumed, but no Duff
*        If:
*            Total Consumed = 7.5
*            Then, 100 percent of litter and 50 percent of duff is consumed
* Note-1: I ask and they said this was in 0 -> 100
*
*   In:  a_CI.....see below
*  Out:  a_DUF....see below this return Litter and Duff percents
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  Equ_CP_Per (d_CI *a_CI, d_DUF *a_DUF)
{
float  Ll, Dl, Tc, Dc,Lc;
float X1_Load, X2_LitMoi;

  a_DUF->f_Per = 0;                          /* percent consumed           */
  a_DUF->f_PerLit = 0;
  a_DUF->i_PerEqu = e_CP_PerEq;              /* the equations number       */
  a_DUF->i_LitEqu = e_CP_PerEq;              /* the equations number       */

  if ((a_CI->f_Lit + a_CI->f_Duff) <= 0)     /* Need these combined        */
    return;                                  /* make sure we have some     */

   Ll = TPA_To_MTPH (a_CI->f_Lit);           /* Get to Metric Tons         */
   Dl = TPA_To_MTPH (a_CI->f_Duff);          /* Lit Load and Duff Load     */

   X1_Load = Ll + Dl;
   X2_LitMoi = a_CI->f_LitMoi;               /* See Note-1 above           */

/* Total amount consumed, includes litter and duff                         */
   Tc = -3.893 + ( (0.944*X1_Load) - (0.078*X2_LitMoi));

/* f_Con is negative */
   if ( Tc > X1_Load )               /* Don't consume mor thn we strg with*/
     Tc = X1_Load;

   if ( Tc < 0 )                     /* check for negative                */
     Tc = 0;

   Dc = Tc - Ll;                     /* Tot consmd minus pre-fir Lit load    */
   if ( Dc <= 0 )                    /* get amount of duff consumed          */
     Dc = 0;

   Lc = Tc - Dc;                     /* Lit consmd is total - Duf consmd     */

/* Now determine the results as percents,  0 -> 100                          */
/* Duff and Litter percents consumed are each kept separate                  */
   if ( Dl <= 0 )
     a_DUF->f_Per = 0;
   else
     a_DUF->f_Per = 100.0 * ( Dc / Dl );

   if ( Ll <= 0 )
     a_DUF->f_PerLit = 0;
   else
     a_DUF->f_PerLit = 100.0 * ( Lc / Ll );

/* checks prevent any small over runs like - 100.00123                      */
   if ( a_DUF->f_PerLit > 100.0 )
     a_DUF->f_PerLit = 100.0;

   if ( a_DUF->f_Per > 100.0 )
     a_DUF->f_Per = 100.0;

}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_CP_MSE
* Desc: Coastal Plain - Calculate Percent or Mineral Soil Exposure
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void Equ_CP_MSE  (d_DUF *a_DUF)
{
   a_DUF->f_MSEPer =  5.0;
   a_DUF->i_MSEEqu = e_CP_MSEEq;
}


/******************************************************************
* Name: Equ_20_Pocosin
* Desc: New formula May 5 2016 based on Reardon and Lutes
*       Search for the related files in one Folder/Directories 
*       for this Solution.  
*         Pocosin Duff Consumption Calc Logistic.xlsx
*         Pocosin Duff Consumption.docx
*       This is my functions based on documents, it's pretty much
*       the same as the Excel program and DL's algorithm with 
*       a correction to how the layers amounts are handled. 
*******************************************************************/
float  Equ_20_PerRed_Pocosin (d_CI *a_CI, d_DUF *a_DUF)
{
int i; 
float f, f_DufRem, f_DufDep, f_DuffMoist, MoiInc; 
float MCLayer, f_Per, f_DufPerLay, f_DufTotCon, f_10th, f_DepRem ; 
#define e_Mineral 5.0               /* Mineral Content */ 

   f = a_CI->f_DufDep * 10; 
   f_10th = a_CI->f_Duff / f;             /* Amt of Duff Per 10th of inch */ 

   if ( a_CI->f_DufDep >= 4.0 )           /* Have a least one layer */
      f_DufPerLay = f_10th * 40.0;  /* Duff in a 4 inch layer */ 
   else 
      f_DufPerLay = a_CI->f_Duff;         /* Only one layer - so use all duff */

   f_DepRem = a_CI->f_DufDep;        /* Duff Depth remaining to be processed */ 
   f_DufRem = a_CI->f_Duff;         /* Duff Remaining to process */ 
   MCLayer = a_CI->f_MoistDuff; 

   f_DufTotCon = 0; 
   MoiInc = 0; 
/* Loop thru the layers */ 
   for ( i = 1; i < 1000; i++ ) { 
     
     if ( MCLayer < 10.0 )         // If MCLayer < 10 then MCLayer = 1
        f_Per = 1.0; 

     if ( MCLayer >= 10 && MCLayer < 30.0 )  // If 10 > MCLayer > 30 then DuffConLayer = 1-(MC*0.00167)
        f_Per = 1.0 - (MCLayer * 0.00167); 

     if ( MCLayer >= 30.0 && MCLayer < 140.0 )                                                 // If MCLayer >30 and <140 then DuffConLayer = 
        f_Per = (1.0 /(1.0 + ( exp(-1.0 *(2.033 - (0.043 * MCLayer) + ( 0.44 * e_Mineral)))))); // 1/(1+(EXP(-1*(2.033-(0.043* MCLayer)+(0.44*MinCon)))))

     if ( MCLayer >= 140.0 && MCLayer < 170.0 ) // If 140 > MCLayer and <170 then DuffConLayer = 0.143441  ((MCLayer  140)*0.0049)
        f_Per = (0.143441 - ( (MCLayer - 140.0) * 0.0049)); 

     if ( MCLayer >= 170.0 ) // If MCLayer >170 then DuffConLayer = 0 
        f_Per = 0;  

     f = f_DufPerLay * f_Per; 
     f_DufTotCon += f; 

     if ( f_DepRem < 4.0 )
        break; 

     f_DepRem = f_DepRem - 4.0; 
     f_DufRem = f_DufRem - f_DufPerLay; 

     if ( f_DepRem < 4.0 ) 
       f_DufPerLay = f_DufRem; 


     MoiInc += 3.0; 
     if ( MoiInc > 12.0 ) 
        MoiInc = 12.0; 
     MCLayer += MoiInc; 
     
     } /* for */ 

   f_Per =  f_DufTotCon / a_CI->f_Duff;  /* Total Consumed / Duff Load = percent consumed */
   a_DUF->f_Per =  ( f_Per * 100.0 );    /* Percent consumed - as a whole number */ 
   a_DUF->i_PerEqu = 20;

   a_DUF->f_Red = a_CI->f_DufDep * f_Per;  
   a_DUF->i_RedEqu = 20;  

   return 1; 
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_19_Per, Equ_19_Red, Equ_19_MSE
* Desc: These equations are all 100 percent and were setup for use
*       with the Shrub Group Chaparral SGC
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  Equ_19_Per (d_CI *a_CI, d_DUF *a_DUF)
{
   a_DUF->f_Per = 100;                  /* 100 percent     */
   a_DUF->i_PerEqu = 19;
} 

void  Equ_19_Red (d_CI *a_CI, d_DUF *a_DUF)
{
   a_DUF->f_Red  =  a_CI->f_DufDep;
   a_DUF->i_RedEqu = 19;
}

void Equ_19_MSE  (d_DUF *a_DUF)
{
  a_DUF->f_MSEPer =  100.0;
  a_DUF->i_MSEEqu = 19;
}



////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
#ifdef XXXXXXXXXXX
  This was a function I wrote based on DL's Excell/algorithm (see below ) 
  which didn't quite work right, See my Eq_20_Pocosin() above 
  I'm keeping this stuff here just incase we want to refer back to
  it for some reason. 
/****************************************************************************************/


/***************************************************************************
* Name: Equ_20_Pocosin
* Desc: Duff Consumption, new equation J Reardon
*       Implement from DL's algorithm see 'ifdef' code following this
*       function. 
* Date: May 3 2016
*
*
***************************************************************************/
float  Equ_20_Pocosin (d_CI *a_CI, d_DUF *a_DUF)
{
float   f,g; 
float	DuffLoad = a_CI->f_Duff;    // = duff load (t/ac)			*From UI
float 	DuffDep = a_CI->f_DufDep;   // = duff depth (in)				*From UI
float   Layers;                     // = number of layers of the duff
float 	MCTop = a_CI->f_MoistDuff;  // = % moisture of top duff layer		*Duff MC from UI
float 	MCAdd;                      // = % moisture to be added to the surface 
float 	MCLayer;                    // = % moisture of the current layer
float 	DuffConLayer;               // = proportion duff depth consumed in layer
float	DuffLoadLayer;              // = duff load in layer
float	DuffConTot;                 // = total duff load consumed 
int	Count;                      // = loop counter
float	MinCon = 5.0;                     // = Mineral Content (%) (Default to 5%)

float LayerProp;  
int   LayerTot; 

   LayerTot = Ceil (DuffDep / 4.0);          // *Calc number of duff layers
   LayerProp = DuffLoad / LayerTot;   // *Calc load per layer (t/ac)
   DuffConTot = 0;                    // *Zero out total consumption counter

   Count = 1;      // *Set loop counter
   MCAdd = 0;      // *Set moisture content counter  

   while  ( Count <= LayerTot ) {
     if ( Count == 1 )             // If Layer = 1 then MCLayer = MCTop
        MCLayer = MCTop; 
 
     if ( MCLayer < 10.0 )         // If MCLayer < 10 then MCLayer = 1
        MCLayer = 1.0; 

     if ( MCLayer >= 10 && MCLayer < 30.0 )  // If 10 > MCLayer > 30 then DuffConLayer = 1-(MC*0.00167)
        DuffConLayer = 1.0 -( MCLayer * 0.00167 );

     if ( MCLayer >= 30.0  && MCLayer < 140.0 )                                                 // If MCLayer >30 and <140 then DuffConLayer = 
        DuffConLayer = 1.0 /(1.0 + ( exp(-1.0 *(2.033 - (0.043 * MCLayer) + ( 0.44 * MinCon))))); // 1/(1+(EXP(-1*(2.033-(0.043* MCLayer)+(0.44*MinCon)))))

     if ( MCLayer >= 140.0 && MCLayer < 170.0 ) // If 140 > MCLayer and <170 then DuffConLayer = 0.143441  ((MCLayer  140)*0.0049)
        DuffConLayer =  0.143441 - ( (MCLayer - 140.0) * 0.0049); 

     if ( MCLayer >= 170.0 ) // If MCLayer >170 then DuffConLayer = 0 
        DuffConLayer = 0; 

     g = DuffLoad / LayerProp;
     DuffConTot = DuffConTot + (DuffConLayer * g);    // DufConTot = DufConTot + (DuffConLayer * (DuffLoad / LayerProp))

     MCAdd = MCAdd + 3.0;    // MCAdd = MCAdd + 3
     if ( MCAdd > 12.0 )     // If MCAdd > 12 then MCAdd = 12
       MCAdd = 12.0; 
     MCLayer = MCLayer + MCAdd;  // MCLayer = MCLayer + MCAdd

     Count ++; 

     }  /* while - End Do */ 

   a_DUF->f_Per = DuffConTot;           /* Percent consumed */ 
   a_DUF->i_PerEqu = 20;

   a_DUF->f_Red = a_CI->f_DufDep * a_DUF->f_Per;  
   a_DUF->i_RedEqu = 20;  
   return 1; 
} 

/***********************************************
*
*
************************************************/
int Ceil (float f)
{
int i; 
float g; 
   i = (int) f; 
   g = f - i;
   if ( g < 0.000001 )  
     return i; 
   i++;
   return i;
}

/**************************************************************************/

CODE LOGIC:
	DuffLoad = duff load (t/ac)			*From UI
	DuffDep = duff depth (in)				*From UI
	Layers = number of layers of the duff
	MCTop = % moisture of top duff layer		*Duff MC from UI
	MCAdd = % moisture to be added to the surface 
	MCLayer = % moisture of the current layer
	DuffConLayer = proportion duff depth consumed in layer
	DuffLoadLayer = duff load in layer
	DuffConTot = total duff load consumed 
	Count = loop counter
	MinCon = Mineral Content (%) (Default to 5%)
NEW:
LayerTot = Ceil (Dep/4) 					*Calc number of duff layers
LayerProp = DuffLoad/LayerTot				*Calc load per layer (t/ac)
DuffConTot = 0						*Zero out total consumption counter
Count = 1						*Set loop counter
MCAdd = 3						*Set moisture content counter  
If Duff Bulk Density <8 or >30 then ERROR
Do while Count < LayerTot
If Layer = 1 then MCLayer = MCTop
If MCLayer < 10 then MCLayer = 1
If 10 > MCLayer > 30 then DuffConLayer = 1-(MC*0.00167)
If MCLayer >30 and <140 then DuffConLayer = 
1/(1+(EXP(-1*(2.033-(0.043* MCLayer)+(0.44*MinCon)))))
If 140 > MCLayer and <170 then DuffConLayer = 0.143441  ((MCLayer  140)*0.0049)
If MCLayer >170 then DuffConLayer = 0 
DufConTot = DufConTot + (DuffConLayer * (DuffLoad / LayerProp))
 	MCAdd = MCAdd + 3
	If MCAdd > 12 then MCAdd = 12
	MCLayer = MCLayer + MCAdd
End Do
Save DuffConTot for consumed report and emissions
#endif 

#ifdef OLD_POCOSIN 
/*************************************************************************
* Name Pocosin_Duff
* Desc: Duff consumption from Jim Readon approx Mar 2016
*
* Extreme :   if moisture content  <26 %  consumption = 99 %  
* High:       if mc >26 and < 68 then consumption= 95%
* Moderate :  if mc <68 and >128   then consumption = 180-1.39*mc
* Low:        if mc <170 and > 128 then consumption=1%
* Very Low:    if mc > 170 then consumption=0% 
*
* SAF98  Pocosin
*    168 - gallberry- fetterbush shrubland
*    170 - pond pine gallberry /fetterbush shrubland
* NVCS-  pond pine saturated  woodland
*
*
*
************************************************************************/
int Pocosin_Duff (d_CI *a_CI, d_DUF *a_DUF)
{
float f, mc, mcp, f_Con;
float pc;
    mc = a_CI->f_MoistDuff; 
    if ( mc < 26.0 )                   /* Extreme */ 
       pc = 99.0;                      /* % consumed */
    else if ( mc < 68.0 )              /* High */ 
       pc = 95.0;
    else if ( mc < 128.0 ) {           /* Moderate */ 
       mcp = mc / 100.0;
       pc = 180.0 - ( 1.39 * mc ); }
    else if ( mc < 170.0 )             /* Low */   
       pc = 1.0; 
    else                               /* Very Low */ 
       pc = 0;       
   a_DUF->f_Per =  pc; 
   a_DUF->i_PerEqu = 201;

   f = 1.0 - pc; 
   a_DUF->f_Red = a_CI->f_DufDep * f;  
   a_DUF->i_RedEqu = 20;    
   return 1; 
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_20_RedPer
* Desc: Calculate Duff Reduction Amount and then do the Percent
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  Equ_20_RedPer (d_CI *a_CI, d_DUF *a_DUF)
{
  if ( CI_isDuffWet(a_CI) )             /*  Moisture Condtion - Wet    */
    a_DUF->f_Red = 0;
  else
    a_DUF->f_Red = a_CI->f_DufDep - 4;      /* reduction depth by 4 inches  */

  if ( a_DUF->f_Red < 0 )                    /* don't let it get negative    */
    a_DUF->f_Red = 0;

  a_DUF->i_RedEqu = 20;                      /* Equation #                   */

  Equ_201_Per (a_CI, a_DUF);                /* go do percent                */
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_201_Per
* Desc: Calculate Percent of Duff Reduction
*       Duff Reduction Amount MUST be done first and set in the a_DUF
*
* ---->See Page 51 of FOFEM 4.0 user guide
*
* NOTE: below 11 converts root mat depth to biomass
*             22 converts muck depth to biomass
*             88 because we assume top 8 inches of root mat are consumend
*   In: a_DUF->f_Red.....reduction amount
*  Out: a_DUF->f_Per.....percent of reduction Duff
*       a_DUF->i_PerEqu..201
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  Equ_201_Per (d_CI *a_CI, d_DUF *a_DUF)
{
float f;
   if ( a_CI->f_Duff <= 0 ) {
     a_DUF->f_Per = 0;
     goto Per; }

/* See NOTE above in header */
   if ( a_DUF->f_Red <=  8 )                      /* 8 inches                */
      f = a_DUF->f_Red * 11.0;                    /* Calc actual amount of   */
   else                                           /*  Duff Reduction         */
      f = 88.0 + (22.0 * (a_DUF->f_Red - 8.0) );

   a_DUF->f_Per = 100.0 * ( f / a_CI->f_Duff );  /* Now Calc the percent    */

Per:
   a_DUF->i_PerEqu = 201;
}

#endif 

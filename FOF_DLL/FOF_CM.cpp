
/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_cm      Fofem Consumed Manager
* Desc: Makes calls to Burnup and HSF Fuels (Shr,Hrb,Fol,Mineral )
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include  "bur_brn.h"
#include "fof_sgv.h"
#include  "fof_co.h"
#include  "fof_co2.h"
#include  "fof_ci.h"
#include  "fof_bcm.h"
#include  "fof_hsf.h"
#include  "fof_cm.h"
#include  "fof_ansi.h"

void  _CM_Tot (d_CO *a_CO);



/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_cm
* Desc: Consume Manager, Calls burnup consume manager and the other
*        consume manager that does Herb,Shrub, Foliage, Mineral
*
* NOTE: if Burnup model doesn't ignite any Herb,Shrub,Foliage,Duff consumed
*        amounts will be 0'ed out.
* Note-1: 4/10/18 - Ticket-188 when doing batch with User Equation option
*          use the Duff Depth Reduction percent (percent of duff removed)
*
* Out: a_CO......
*      cr_ErrMes.......Any error message, function Return will equal 0
* Ret: 1 OK,
*      2 Burnup Didn't ignite, see 'NOTE' above
*      0 Error - see cr_ErrMes
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int CM_Mngr (d_CI *a_CI,  d_CO *a_CO, char cr_ErrMes[])
{
int i;
   strcpy (cr_ErrMes,""); 
   if ( !HSF_Mngr (a_CI, a_CO,cr_ErrMes))           /* Herb,shrub,Foliage,Mineral*/
     return 0;

/* Inputs needed for Burnup................................................. */
   a_CI->f_HSFCon = a_CO->f_HerCon + a_CO->f_ShrCon +
                    a_CO->f_FolCon + a_CO->f_BraCon;

/* See Note-1 above */ 
  if ( !xstrcmpi( a_CI->cr_BatchEqu, "Yes" ) ) 
     a_CI->f_PerDufCon = a_CO->f_DufDepPer;
  else
     a_CI->f_PerDufCon = a_CO->f_DufPer;       /* % of Duff Consumed         */


   i = BCM_Mngr (a_CI, a_CO,cr_ErrMes);      /* Burnup Consumed Manager    */
   if ( i == 0 )                             /* Error occured                */
      return 0;
   if ( i == 2 ) {                           /* Burnup didn't ignite         */
     return 2; }                             /*  consmed amounts             */

   _CM_Tot (a_CO);


   return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name:
* Desc:
* Out: a_CO......
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  _CM_Tot (d_CO *a_CO)
{

   a_CO->f_TotPre = a_CO->f_LitPre + a_CO->f_DW1Pre + a_CO->f_DW10Pre +
         a_CO->f_DW100Pre + a_CO->f_Snd_DW1kPre + a_CO->f_Rot_DW1kPre +
         a_CO->f_DufPre + a_CO->f_HerPre + a_CO->f_ShrPre + a_CO->f_FolPre +
         a_CO->f_BraPre;

   a_CO->f_TotCon = a_CO->f_LitCon + a_CO->f_DW1Con + a_CO->f_DW10Con +
         a_CO->f_DW100Con + a_CO->f_Snd_DW1kCon + a_CO->f_Rot_DW1kCon +
         a_CO->f_DufCon + a_CO->f_HerCon + a_CO->f_ShrCon + a_CO->f_FolCon +
         a_CO->f_BraCon;

   a_CO->f_TotPos = a_CO->f_TotPre - a_CO->f_TotCon;

   if ( a_CO->f_TotPre > 0 )
      a_CO->f_TotPer = a_CO->f_TotCon / a_CO->f_TotPre;
   else
      a_CO->f_TotPer = 0;

}

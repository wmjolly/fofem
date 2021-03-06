/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: FOF_DLL.cpp
* Desc: FOFEM DLL class functions
*
*
* Date: 10-20-10
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <direct.h>
#include <conio.h> 
#include <process.h>


#include  "bur_brn.h"
#include "fof_dll.h"
#include "fof_ci.h"
#include "fof_sgv.h"
#include "fof_co.h"
#include "fof_co2.h"
#include "fof_cm.h"

#include "..\\FOF_GUI\\fof_mis.h"
#include "fof_mrt.h"

#include "fof_lcc.h"
#include "fof_nes.h"

#include "fof_Util.h" 



/*******************************************************************************/
FOFEM::FOFEM() 
{  
  lcc = new d_LCC();
}

FOFEM::~FOFEM() 
{  
  delete lcc;
}

/*********************************************************************
* Name: Init
* Desc: init the DLL
*   In: iS....1 read in the Extended Emission Factor file 
*             0 don't read it in
*********************************************************************/
int FOFEM::Init (char cr_ErrMes[], int iS)
{
int i;
  lcc->CI_init(); 
  lcc->CO_init(); 

  if ( iS == 0 ) 
    return 1; 

/* Read in the Extended Emissions Factor file, in case it's needed */
  i = NES_Read ("", cr_ErrMes);
  if ( i <= 0 ) 
     return 0; 
  return 1; 
}

/************************************************************************
* Name: CI_SetEmis
* Desc: Set the Default or Extended emissions info into the DLL's s_CI
*        (input structure). The s_CI info later gets loaded into burnup.
*   ---> To use/set the default emission code place "0" in the input
*         parameters. 
*   In: cr_FLa....Extended Emission Short-Term Flaming and Smoldering 
*                 numeric code 
*       cr_Duff...Extended Duff RSC numeric emis code
*       cr_Woody..Extended Coarse Woody RSC numeric emis code
*  Out: cr_ErrMes...error message - wrong Extended Emis code sent in
*************************************************************************/
int    FOFEM::CI_SetEmis (char cr_Fla[], char cr_Duff[], char cr_Woody[], char cr_ErrMes[])
{ 
int i; 

  strcpy (cr_ErrMes,""); 

  strcpy (lcc->s_CI.cr_EmiFla, cr_Fla);
  Trim_LT (lcc->s_CI.cr_EmiFla);   /* Make sure no lead or trail blnk chars */

  strcpy (lcc->s_CI.cr_EmiDuf,cr_Duff);
  Trim_LT (lcc->s_CI.cr_EmiDuf);

  strcpy (lcc->s_CI.cr_EmiSmo,cr_Woody); 
  Trim_LT (lcc->s_CI.cr_EmiSmo);

/* We only have to set the f_CriInt for burnup to use the Default Emis */ 
  if ( !strcmp (lcc->s_CI.cr_EmiFla, "0") ) {
     lcc->s_CI.f_CriInt = -1.0; 
     return 1; }

/* Set up for Extended Emis (basic 7 emis components */
  lcc->s_CI.f_CriInt = NES_Get_CriFirInt ();  

  i = NES_Get_MajFactor(lcc->s_CI.cr_EmiFla,
                        &lcc->s_CI.f_fCO, 
                        &lcc->s_CI.f_fCO2,
                        &lcc->s_CI.f_fCH4, 
                        &lcc->s_CI.f_fPM25, 
                        &lcc->s_CI.f_fPM10, 
                        &lcc->s_CI.f_fNOX, 
                        &lcc->s_CI.f_fSO2);
  if ( i == 0 ) {
    strcpy (cr_ErrMes, "Invalid Short-term Flame Smolder code");  
    return 0; }
 
  i = NES_Get_MajFactor(lcc->s_CI.cr_EmiDuf,
                        &lcc->s_CI.f_dCO, 
                        &lcc->s_CI.f_dCO2,
                        &lcc->s_CI.f_dCH4, 
                        &lcc->s_CI.f_dPM25, 
                        &lcc->s_CI.f_dPM10, 
                        &lcc->s_CI.f_dNOX, 
                        &lcc->s_CI.f_dSO2);
  if ( i == 0 ) {
    strcpy (cr_ErrMes, "Invalid Duff RSC code");  
    return 0; }

  i = NES_Get_MajFactor(lcc->s_CI.cr_EmiSmo,
                        &lcc->s_CI.f_sCO, 
                        &lcc->s_CI.f_sCO2,
                        &lcc->s_CI.f_sCH4, 
                        &lcc->s_CI.f_sPM25, 
                        &lcc->s_CI.f_sPM10, 
                        &lcc->s_CI.f_sNOX, 
                        &lcc->s_CI.f_sSO2);
  if ( i == 0 ) {
    strcpy (cr_ErrMes, "Invalid Coarse Wood RSC code");  
    return 0; }

  return 1;
}


/************************************************************************/

void  FOFEM::CI_f_Duff    (float f)   { lcc->CI_f_Duff(f); }
void  FOFEM::CI_f_DufDep  ( float f)  { lcc->CI_f_DufDep(f);}
void  FOFEM::CI_f_MoistDuf ( float f) { lcc->CI_f_MoistDuf(f);}

void   FOFEM::CI_f_Herb          ( float f)  { lcc->CI_f_Herb        (f);}
void   FOFEM::CI_f_Shrub         ( float f)  { lcc->CI_f_Shrub       (f);}
void   FOFEM::CI_f_CroFol        ( float f)  { lcc->CI_f_CroFol      (f);}
void   FOFEM::CI_f_CroBra        ( float f)  { lcc->CI_f_CroBra      (f);}
void   FOFEM::CI_f_Pc_CroBrn     ( float f)  { lcc->CI_f_Pc_CroBrn   (f);}
void   FOFEM::CI_f_MoistDW10     ( float f)  { lcc->CI_f_MoistDW10   (f);}
void   FOFEM::CI_f_MoistDW1000   ( float f)  { lcc->CI_f_MoistDW1000 (f);}
void   FOFEM::CI_f_Lit           ( float f)  { lcc->CI_f_Lit         (f);}
void   FOFEM::CI_f_DW1           ( float f)  { lcc->CI_f_DW1         (f);}
void   FOFEM::CI_f_DW10          ( float f)  { lcc->CI_f_DW10        (f);}
void   FOFEM::CI_f_DW100         ( float f)  { lcc->CI_f_DW100       (f);}
void   FOFEM::CI_f_Snd_DW3       ( float f)  { lcc->CI_f_Snd_DW3     (f);}
void   FOFEM::CI_f_Snd_DW6       ( float f)  { lcc->CI_f_Snd_DW6     (f);}
void   FOFEM::CI_f_Snd_DW9       ( float f)  { lcc->CI_f_Snd_DW9     (f);}
void   FOFEM::CI_f_Snd_DW20      ( float f)  { lcc->CI_f_Snd_DW20    (f);}
void   FOFEM::CI_f_Rot_DW3       ( float f)  { lcc->CI_f_Rot_DW3     (f);}
void   FOFEM::CI_f_Rot_DW6       ( float f)  { lcc->CI_f_Rot_DW6     (f);}
void   FOFEM::CI_f_Rot_DW9       ( float f)  { lcc->CI_f_Rot_DW9     (f);}
void   FOFEM::CI_f_Rot_DW20      ( float f)  { lcc->CI_f_Rot_DW20    (f);}

void   FOFEM::CI_cr_Region       (char cr[]) { lcc->CI_cr_Region(cr); }


void   FOFEM::CI_cr_Season       (char cr[]) { lcc->CI_cr_Season       (cr); }
void   FOFEM::CI_cr_FuelCategory (char cr[]) { lcc->CI_cr_FuelCategory (cr); }

void   FOFEM::CI_cr_DufMoiMet    (char cr[]) { lcc->CI_cr_DufMoiMet    (cr); }
void   FOFEM::CI_cr_CoverGroup   (char cr[]) { lcc->CI_cr_CoverGroup   (cr); }
void   FOFEM::CI_cr_LoadFN       (char cr[]) { lcc->CI_cr_LoadFN       (cr); }
void   FOFEM::CI_cr_EmiFN        (char cr[]) { lcc->CI_cr_EmiFN        (cr); }

/****************************************************************************************/
void   FOFEM::CI_EmisFactor (char cr_Type[], char cr_GrpNum[], float f_CO, float f_CO2, float f_CH4, float f_PM25, float f_PM10, float f_NOX, float f_SO2)
{
   lcc->CI_Set_EmisFactor(cr_Type,cr_GrpNum, f_CO, f_CO2, f_CH4, f_PM25, f_PM10, f_NOX, f_SO2);
}



/*..........................................................*/
int  FOFEM::CM_mngr (char cr_ErrMes[])
{
int i;
   i = lcc->CM_mngr(cr_ErrMes);
   return i;
}


 void  FOFEM::CO_cr_BrnIg (char cr[]) { lcc->CO_cr_BrnIg(cr); }

/***********************************************************************************/
int  FOFEM::CO_GetEm_cFOFEM (int iX, float *PM25, float *PM10, float *CH4, float *CO2,
                                     float *CO,   float *NOX,  float *SO2, float *Intensity)
{
int i;
   i = lcc->CO_GetEm(iX, PM25, PM10, CH4, CO2, CO, NOX, SO2, Intensity);
   return i; 
} 

/**********************************************************************************/

int FOFEM::CO_GetFlDuCo (float *af_Flame,  float *af_Duff,  float *af_Coarse)
{
int i; 
  i = lcc->CO_GetFlDuCo (af_Flame, af_Duff, af_Coarse);
  return i;  
}

/***********************************************************************************/
void  FOFEM::CO_GetEmTotal (char cr[],  float *PM25, float *PM10, float *CH4, float *CO2,
                                        float *CO,   float *NOX,  float *SO2)
{
  lcc->CO_GetEmTot( cr, PM25, PM10, CH4, CO2, CO, NOX, SO2); 
} 







float FOFEM::CO_BullShit()
{
  return 1; 
}

/***************************************************************************/
 float FOFEM::CO_f_DufPos      () { return lcc->CO_f_DufPos      ();}
 float FOFEM::CO_f_LitPos      () { return lcc->CO_f_LitPos      ();}
 float FOFEM::CO_f_HerPos      () { return lcc->CO_f_HerPos      ();}
 float FOFEM::CO_f_ShrPos      () { return lcc->CO_f_ShrPos      ();}
 float FOFEM::CO_f_DW1Pos      () { return lcc->CO_f_DW1Pos      ();}
 float FOFEM::CO_f_DW10Pos     () { return lcc->CO_f_DW10Pos     ();}
 float FOFEM::CO_f_DW100Pos    () { return lcc->CO_f_DW100Pos    ();}



 float FOFEM::CO_f_S3Pos       () { return lcc->CO_f_S3Pos       ();}
 float FOFEM::CO_f_S6Pos       () { return lcc->CO_f_S6Pos       ();}
 float FOFEM::CO_f_R3Pos       () { return lcc->CO_f_R3Pos       ();}
 float FOFEM::CO_f_R6Pos       () { return lcc->CO_f_R6Pos       ();}
 float FOFEM::CO_f_S9Pos       () { return lcc->CO_f_S9Pos       ();}
 float FOFEM::CO_f_S20Pos      () { return lcc->CO_f_S20Pos      ();}
 float FOFEM::CO_f_R9Pos       () { return lcc->CO_f_R9Pos       ();}
 float FOFEM::CO_f_R20Pos      () { return lcc->CO_f_R20Pos      ();}

 float FOFEM::CO_f_S3Con       () { return lcc->CO_f_S3Con       ();}
 float FOFEM::CO_f_S6Con       () { return lcc->CO_f_S6Con       ();}
 float FOFEM::CO_f_R3Con       () { return lcc->CO_f_R3Con       ();}
 float FOFEM::CO_f_R6Con       () { return lcc->CO_f_R6Con       ();}
 float FOFEM::CO_f_S9Con       () { return lcc->CO_f_S9Con       ();}
 float FOFEM::CO_f_S20Con      () { return lcc->CO_f_S20Con      ();}
 float FOFEM::CO_f_R9Con       () { return lcc->CO_f_R9Con       ();}
 float FOFEM::CO_f_R20Con      () { return lcc->CO_f_R20Con      ();}






 float FOFEM::CO_f_Rot_DW1kCon () { return lcc->CO_f_Rot_DW1kCon ();}

 float FOFEM::CO_f_HerCon      () { return lcc->CO_f_HerCon (); } 
 float FOFEM::CO_f_LitCon      () { return lcc->CO_f_LitCon (); } 
 float FOFEM::CO_f_ShrCon      () { return lcc->CO_f_ShrCon (); } 


 float FOFEM::CO_f_DufCon      () { return lcc->CO_f_DufCon      ();}
 float FOFEM::CO_f_DW1Con      () { return lcc->CO_f_DW1Con      ();}
 float FOFEM::CO_f_DW10Con     () { return lcc->CO_f_DW10Con     ();}
 float FOFEM::CO_f_DW100Con    () { return lcc->CO_f_DW100Con    ();}
 float FOFEM::CO_f_Snd_DW1kCon () { return lcc->CO_f_Snd_DW1kCon ();}
 float FOFEM::CO_f_TotPer      () { return lcc->CO_f_TotPer      ();}

void FOFEM::MRT_LoadSpec (char cr_FN[], char cr_ErrMes[])
{
  lcc->MRT_LoadSpec (cr_FN, cr_ErrMes);
}

float FOFEM::MRT_Calculate (char cr_ErrMes[])
{
float f;
  f = lcc->MRT_Calculate (cr_ErrMes); 
  return f; 
}


void FOFEM::MO_Initial ()
{
  lcc->MO_Initial();
}

void FOFEM::MIS_cr_Spec (char cr[]) { lcc->MIS_cr_Spe (cr); } 
void FOFEM::MIS_f_CroRat (float f)  { lcc->MIS_f_CroRat (f); } 
void FOFEM::MIS_f_Den (float f)     { lcc->MIS_f_Den (f); }
void FOFEM::MIS_f_DBH (float f)     { lcc->MIS_f_DBH (f); }
void FOFEM::MIS_f_Hgt (float f)     { lcc->MIS_f_Hgt (f); }
void FOFEM::MIS_f_FS  (float f)     { lcc->MIS_f_FS  (f); }

void FOFEM::MIS_cr_FS     (char cr[]) { lcc->MIS_cr_FS(cr) ; }
void FOFEM::MIS_cr_FivSev (char cr[]) { lcc->MIS_cr_FivSev(cr) ; }


 void  FOFEM::MIS_f_CrnDam  (float f)    { lcc->MIS_CrnDam (f); }
 void  FOFEM::MIS_f_CKR     (float f)    { lcc->MIS_CKR    (f); }
 void  FOFEM::MIS_cr_BeeDam (char cr[])  { lcc->MIS_BeeDam (cr); }
 void  FOFEM::MIS_f_BolCha  (float f)    { lcc->MIS_BolCha (f); }     
 void  FOFEM::MIS_cr_EquTyp (char cr[])  { lcc->MIS_EquTyp (cr); }








float FOFEM::MO_f_Mort ()      { return lcc->MO_f_Mort(); }
float FOFEM::MO_f_TotKilled () { return lcc->MO_f_TotKilled(); }
float FOFEM::MO_f_AvgMort ()   { return lcc->MO_f_AvgMort(); }


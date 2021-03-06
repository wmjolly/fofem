/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_lcc.cpp    FOFEM Library Container Class
* Desc:
*
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
#include <windows.h> 

#include <stdio.h>
#include <string.h> 

#include "fof_ansi.h"
#include  "bur_brn.h"
#include "fof_ci.h"
#include "fof_sgv.h"
#include "fof_co.h"
#include "fof_co2.h"
#include "fof_cm.h"
//#include "fof_iss.h"
#include "..\\FOF_GUI\\fof_mis.h"
#include "fof_mrt.h"
#include "fof_lcc.h"


void d_LCC::CI_init() {  CI_Init (&this->s_CI); }

void d_LCC::CO_init() {  CO_Init (&this->s_CO); }


void d_LCC::CI_f_Duff     ( float f) { s_CI.f_Duff    = f; }
void d_LCC::CI_f_DufDep   ( float f) { s_CI.f_DufDep  = f; }
void d_LCC::CI_f_MoistDuf ( float f) { s_CI.f_MoistDuff = f; }

void   d_LCC::CI_f_Herb          ( float f) { s_CI.f_Herb        = f ;}
void   d_LCC::CI_f_Shrub         ( float f) { s_CI.f_Shrub       = f ; }
void   d_LCC::CI_f_CroFol        ( float f) { s_CI.f_CroFol      = f ; }
void   d_LCC::CI_f_CroBra        ( float f) { s_CI.f_CroBra      = f ; }
void   d_LCC::CI_f_Pc_CroBrn     ( float f) { s_CI.f_Pc_CroBrn   = f ; }
void   d_LCC::CI_f_MoistDW10     ( float f) { s_CI.f_MoistDW10   = f ; }
void   d_LCC::CI_f_MoistDW1000   ( float f) { s_CI.f_MoistDW1000 = f ; }
void   d_LCC::CI_f_Lit           ( float f) { s_CI.f_Lit         = f ; }
void   d_LCC::CI_f_DW1           ( float f) { s_CI.f_DW1         = f ; }
void   d_LCC::CI_f_DW10          ( float f) { s_CI.f_DW10        = f ; }
void   d_LCC::CI_f_DW100         ( float f) { s_CI.f_DW100       = f ; }
void   d_LCC::CI_f_Snd_DW3       ( float f) { s_CI.f_Snd_DW3     = f ; }
void   d_LCC::CI_f_Snd_DW6       ( float f) { s_CI.f_Snd_DW6     = f ; }
void   d_LCC::CI_f_Snd_DW9       ( float f) { s_CI.f_Snd_DW9     = f ; }
void   d_LCC::CI_f_Snd_DW20      ( float f) { s_CI.f_Snd_DW20    = f ; }
void   d_LCC::CI_f_Rot_DW3       ( float f) { s_CI.f_Rot_DW3     = f ; }
void   d_LCC::CI_f_Rot_DW6       ( float f) { s_CI.f_Rot_DW6     = f ; }
void   d_LCC::CI_f_Rot_DW9       ( float f) { s_CI.f_Rot_DW9     = f ; }
void   d_LCC::CI_f_Rot_DW20      ( float f) { s_CI.f_Rot_DW20    = f ; }

void   d_LCC::CI_cr_Region       (char cr[]) { strcpy (s_CI.cr_Region      ,cr);}
void   d_LCC::CI_cr_Season       (char cr[]) { strcpy (s_CI.cr_Season      ,cr);}
void   d_LCC::CI_cr_FuelCategory (char cr[]) { strcpy (s_CI.cr_FuelCategory,cr);}
void   d_LCC::CI_cr_DufMoiMet    (char cr[]) { strcpy (s_CI.cr_DufMoiMet   ,cr);}
void   d_LCC::CI_cr_CoverGroup   (char cr[]) { strcpy (s_CI.cr_CoverGroup  ,cr);}
void   d_LCC::CI_cr_LoadFN       (char cr[]) { strcpy (s_CI.cr_LoadFN      ,cr);}
void   d_LCC::CI_cr_EmiFN        (char cr[]) { strcpy (s_CI.cr_EmiFN       ,cr);}


/******************************************************************************
* Name: CI_Set_EmisFactor
* Desc: Set the Emission Factors into the CI Consumed Input Structure
*        these factors will get used in Burnup
******************************************************************************/
void   d_LCC::CI_Set_EmisFactor (char cr_Type[], char cr_GrpNum[], float f_CO, float f_CO2, float f_CH4, float f_PM25, float f_PM10, float f_NOX, float f_SO2)
{

/* Critical Fire Intensity */ 
  if ( !stricmp (cr_Type, "CriFirInt") ) {
    s_CI.f_CriInt = f_CO; }

  if ( !stricmp (cr_Type,"Flame") ) {
    s_CI.f_fCO   = f_CO; 
    s_CI.f_fCO2  = f_CO2; 
    s_CI.f_fCH4  = f_CH4; 
    s_CI.f_fPM25 = f_PM25;
    s_CI.f_fPM10 = f_PM10;
    s_CI.f_fNOX  = f_NOX; 
    s_CI.f_fSO2  = f_SO2; }

  if ( !stricmp (cr_Type,"Smolder") ) {
    s_CI.f_sCO   = f_CO; 
    s_CI.f_sCO2  = f_CO2; 
    s_CI.f_sCH4  = f_CH4; 
    s_CI.f_sPM25 = f_PM25;
    s_CI.f_sPM10 = f_PM10;
    s_CI.f_sNOX  = f_NOX; 
    s_CI.f_sSO2  = f_SO2; }

  if ( !stricmp (cr_Type,"Duff") ) {
    s_CI.f_dCO   = f_CO; 
    s_CI.f_dCO2  = f_CO2; 
    s_CI.f_dCH4  = f_CH4; 
    s_CI.f_dPM25 = f_PM25;
    s_CI.f_dPM10 = f_PM10;
    s_CI.f_dNOX  = f_NOX; 
    s_CI.f_dSO2  = f_SO2; }
} 

/***********************************************************************************/
int   d_LCC::CM_mngr (char cr_ErrMes[])
{
int i;
 i = CM_Mngr (&s_CI, &s_CO, cr_ErrMes);
 return i;
  }


/********************************************************************************
* Name: CO_GetEmTot
* Desc: Get the Emission totals/factors for Flaming or Smoldering
*   In: cr.....see below
********************************************************************************/
void d_LCC::CO_GetEmTot (char cr[], float *PM25, float *PM10, float *CH4, float *CO2, 
                                   float *CO,  float *NOX, float *SO2)
{
/* Get the emissions amounts */ 
  if ( !stricmp (cr,"Flame") ) {
   *PM25 = this->s_CO.f_PM25F;
   *PM10 = this->s_CO.f_PM10F;
   *CH4  = this->s_CO.f_CH4F;
   *CO2  = this->s_CO.f_CO2F;
   *CO   = this->s_CO.f_COF;
   *NOX  = this->s_CO.f_NOXF;
   *SO2  = this->s_CO.f_SO2F;}

  else if ( !stricmp(cr,"Smolder") ) {
   *PM25 = this->s_CO.f_PM25S;
   *PM10 = this->s_CO.f_PM10S;
   *CH4  = this->s_CO.f_CH4S;
   *CO2  = this->s_CO.f_CO2S;
   *CO   = this->s_CO.f_COS;
   *NOX  = this->s_CO.f_NOXS;
   *SO2  = this->s_CO.f_SO2S;}


/* Get the Emission Factors used by Burnup */ 
 else if ( !stricmp(cr,"FlameFactors") ) {
   *PM25 = this->s_CO.d_pm25f; 
   *PM10 = this->s_CO.d_pm10f; 
   *CH4  = this->s_CO.d_ch4f;  
   *CO2  = this->s_CO.d_co2f;   
   *CO   = this->s_CO.d_cof;  
   *NOX  = this->s_CO.d_noxf;  
   *SO2  = this->s_CO.d_so2f; }

 else if ( !stricmp(cr,"SmolderFactors") ) {
   *PM25 = this->s_CO.d_pm25s; 
   *PM10 = this->s_CO.d_pm10s;
   *CH4  = this->s_CO.d_ch4s;
   *CO2  = this->s_CO.d_co2s;
   *CO   = this->s_CO.d_cos;
   *NOX  = this->s_CO.d_noxs; 
   *SO2  = this->s_CO.d_so2s; }

}


/********************************************************************************
*
*
********************************************************************************/
int   d_LCC::CO_GetEm (int iX, float *PM25, float *PM10, float *CH4, float *CO2, 
                               float *CO,  float *NOX, float *SO2, float *Intensity)
{
   if ( iX >= s_CO.iC_SGV )
     return 0; 

   *PM25 = s_CO.sr_SGV[iX].f_PM2_5;
   *PM10 = s_CO.sr_SGV[iX].f_PM10;
   *CH4  = s_CO.sr_SGV[iX].f_CH4;
   *CO2  = s_CO.sr_SGV[iX].f_CO2;
   *CO   = s_CO.sr_SGV[iX].f_CO;
   *NOX  = s_CO.sr_SGV[iX].f_NOX;
   *SO2  = s_CO.sr_SGV[iX].f_SO2;
   *Intensity = s_CO.sr_SGV[iX].f_Inten;
   return 1; 
}

/********************************************************************
* Name: CO_GetFlDuCo
* Desc: Get the consumed amounts for flaming, smoldering(coarse) and
*       duff which we calculated in burnup.
*  -->  Fuel Amounts are Kg/Sq m, Killograms per square meter 
*********************************************************************/
int  d_LCC::CO_GetFlDuCo (float *af_Flame, float *af_Duff,  float *af_Smolder)
{ 
int i;
float fla,duf,smo;
   fla = duf = smo = 0; 

/* add up amounts in each burnup time step */
   for ( i = 0; i < s_CO.iC_SGV; i++ ) {
     if ( s_CO.sr_SGV[i].f_Sec < 0 ) 
       break; 
     fla += s_CO.sr_SGV[i].f_FlaCon;
     duf += s_CO.sr_SGV[i].f_DuffCon;
     smo += s_CO.sr_SGV[i].f_SmoCon;
  } 

  *af_Flame = fla; 
  *af_Duff = duf; 
  *af_Smolder = smo; 
   return 1; 
}


/*********************************************************/
void  d_LCC::CO_cr_BrnIg (char cr[])   {  strcpy (cr,s_CO.cr_BrnIg); }

float d_LCC::CO_f_DufPos   ()   { return  s_CO.f_DufPos; }
float d_LCC::CO_f_LitPos   ()   { return  s_CO.f_LitPos; }
float d_LCC::CO_f_HerPos   ()   { return  s_CO.f_HerPos; }
float d_LCC::CO_f_ShrPos   ()   { return  s_CO.f_ShrPos; }

float d_LCC::CO_f_DW1Pos   ()   { return  s_CO.f_DW1Pos; }
float d_LCC::CO_f_DW10Pos  ()   { return  s_CO.f_DW10Pos; }
float d_LCC::CO_f_DW100Pos ()   { return  s_CO.f_DW100Pos; }

float d_LCC::CO_f_S3Pos    ()   { return  s_CO.f_S3Pos; }
float d_LCC::CO_f_S6Pos    ()   { return  s_CO.f_S6Pos; }
float d_LCC::CO_f_R3Pos    ()   { return  s_CO.f_R3Pos; }
float d_LCC::CO_f_R6Pos    ()   { return  s_CO.f_R6Pos; }
float d_LCC::CO_f_S9Pos    ()   { return  s_CO.f_S9Pos; }
float d_LCC::CO_f_S20Pos   ()   { return  s_CO.f_S20Pos; }
float d_LCC::CO_f_R9Pos    ()   { return  s_CO.f_R9Pos ; }
float d_LCC::CO_f_R20Pos   ()   { return  s_CO.f_R20Pos; }


float d_LCC::CO_f_S3Con   ()    { return  s_CO.f_S3Con; }
float d_LCC::CO_f_S6Con    ()   { return  s_CO.f_S6Con; }
float d_LCC::CO_f_R3Con    ()   { return  s_CO.f_R3Con; }
float d_LCC::CO_f_R6Con    ()   { return  s_CO.f_R6Con; }
float d_LCC::CO_f_S9Con    ()   { return  s_CO.f_S9Con; }
float d_LCC::CO_f_S20Con   ()   { return  s_CO.f_S20Con; }
float d_LCC::CO_f_R9Con    ()   { return  s_CO.f_R9Con ; }
float d_LCC::CO_f_R20Con   ()   { return  s_CO.f_R20Con; }






float d_LCC::CO_f_Rot_DW1kCon (){ return  s_CO.f_Rot_DW1kCon ; }
float d_LCC::CO_f_DufCon      () { return  s_CO.f_DufCon      ; }

float d_LCC::CO_f_LitCon      () { return  s_CO.f_LitCon      ; }
float d_LCC::CO_f_HerCon      () { return  s_CO.f_HerCon      ; }
float d_LCC::CO_f_ShrCon      () { return  s_CO.f_ShrCon      ; }

float d_LCC::CO_f_DW1Con      () { return  s_CO.f_DW1Con      ; }
float d_LCC::CO_f_DW10Con     () { return  s_CO.f_DW10Con     ; }
float d_LCC::CO_f_DW100Con    () { return  s_CO.f_DW100Con    ; }
float d_LCC::CO_f_Snd_DW1kCon () { return  s_CO.f_Snd_DW1kCon ; }
float d_LCC::CO_f_TotPer      () { return  s_CO.f_TotPer      ; }

/******************************************************/
void d_LCC::MRT_LoadSpec (char cr_FN[],char cr_ErrMes[])
{
  MRT_LoadSpe (cr_FN,"",cr_ErrMes);   
}
float d_LCC::MRT_Calculate(char cr_ErrMes[])
{
float f;
// f = MRT_Calc (&s_MIS, &s_MI, &s_MO, cr_ErrMes); 

 f = MRT_CalcMngr (&s_MIS, &s_MO, cr_ErrMes); 

 return f; 
}
void d_LCC::MO_Initial () 
{
  MO_Init (&s_MO);
}

 void d_LCC::MIS_cr_Spe (char cr[]) { strcpy (s_MIS.cr_Spe,cr); }

 void d_LCC::MIS_f_CroRat (float f) { s_MIS.f_CR = f; }
 void d_LCC::MIS_f_Den (float f)    { s_MIS.f_Den = f; }
 void d_LCC::MIS_f_DBH (float f)    { s_MIS.f_DBH = f; }
 void d_LCC::MIS_f_Hgt (float f)    { s_MIS.f_Hgt = f; }
 void d_LCC::MIS_f_FS  (float f)    { s_MIS.f_FS = f; }
 void d_LCC::MIS_cr_FS (char cr[])  { strcpy (s_MIS.cr_FS,cr); }
 void d_LCC::MIS_cr_FivSev (char cr[]) { strcpy (s_MIS.cr_FirSev,cr); } 

 void  d_LCC::MIS_CrnDam (float f)   { s_MIS.f_CrnDam = f; }
 void  d_LCC::MIS_CKR    (float f)   { s_MIS.f_CKR = f; }
 void  d_LCC::MIS_BeeDam (char cr[]) { strcpy (s_MIS.cr_BeeDam,cr); }
 void  d_LCC::MIS_BolCha (float f)   { s_MIS.f_BolCha = f; }     
 void  d_LCC::MIS_EquTyp (char cr[]) { strcpy (s_MIS.cr_EquTyp,cr); }

 
 float d_LCC::MO_f_Mort ()      { return s_MO.f_Mort; }
 float d_LCC::MO_f_TotKilled () { return s_MO.f_TotKilled; }
 float d_LCC::MO_f_AvgMort ()   { return s_MO.f_AvgMort; }
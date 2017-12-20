//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             FORMAT.CPP
// Project:			 M4FMT32.DLL
// Author:           Meta Software M.S. , S.A
// Date:			 18/02/98 - 12/06/98 - 20/04/99 (Manu)
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

// INCLUDES        

#include "m4time.hpp"
#include "m4date.hpp"

#include <stdio.h>
#include <stdlib.h>

#include "fmt.h"
#include "fmtdf.cpp"

#include "clargs.h"
#include "clstr.h"
#include "format.h"
#include "cadena.h"
#include "rctables.h"

// Bug 141465. Se implementa la funcionalidad de Regional Settings en UNIX
// Se utiliza las funciones de la m4ini para cargar las propiedades desde un archivo INI.
// Estas funciones son equivalentes a las que existen en Windows
#include "m4ini.hpp"

///////////////////////////////////////////////////////////////////////////////

#define M_NUEVO					0
#define M_DENTRO				1
#define M_CERRADO				2
   
#define DIA						0
#define MES						1
#define ANIO					2

#define FLAG_ACENTLESS			1
#define FLAG_IKIND              2

#define M4_STANDARD_SEPARATORS	" ,.\t"

// Bug 141465. Se implementa la funcionalidad de Regional Settings en UNIX
// Se definen en qué fichero se encuentra la configuración
// En el caso de Windows la seccion [intl] del WIN.INI automáticamente se redirige
// (por compatibilidad) a la rama del registro
// HKEY_CURRENT_USER\ControlPanel\International
#ifdef _UNIX
#define M4_PROFILE_FILE			"m4regionalsettings.ini"
#else
#define M4_PROFILE_FILE			"WIN.INI"
#endif

// Nombre del nodo donde se puede leer los settings de formato Windows
// por parámetro de aplicación
#define M4NODE_WINDOWSFORMAT	"SRP_WINDOWSFORMAT"

///////////////////////////////////////////////////////////////////////////////

/* static const char s_TablaQuitaAcentos[]=
 "                                                                 "\
 "                          "\
 "      "\
 "                          "\
 "                                                                     "\
 "AAA A   EEEEIIII  OOO O  UUUUY  aaa a   eeeeiiii  ooo o  uuuuy y"; */
static const char s_TablaQuitaAcentos[]= "                                                                 " "                          " "      " "                          " "                                                                     " "AAA A   EEEEIIII  OOO O  UUUUY  aaa a   eeeeiiii  ooo o  uuuuy y";

/* static const m4char_t s_pcIndicesFechHora[]=
 "                                                                 "\
 "                          "\
 "      "\
 "   \x4   \x5    \x2\x6    \x7   \x3 \x1 "\
 "                                                                     "\
 "                                                                ";
 */
static const m4char_t s_pcIndicesFechHora[]= "                                                                 " "                          " "      " "   \x4   \x5    \x2\x6    \x7   \x3 \x1 " "                                                                     " "                                                                ";

static const m4char_t s_pcIndicesCurrNume[]= "                                                                 " "                          " "      " "            \x8\x9            " "                                                                     " "                                                                ";

 
/* m4char_t s_TablaMayToMin[257]=
 "                                                                 "\
 "abcdefghijklmnopqrstuvwxyz"\
 "      "\
 "                          "\
 "                                                                     "\
 "àáâ ä  çèéêëìíîï ñòóô ö  ùúûüý                                  ";

static const m4char_t s_TablaMinToMay[]=
 "                                                                 "\
 "                          "\
 "      "\
 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"\
 "                                                                     "\
 "                                ÀÁÂ Ä  ÇÈÉÊËÌÍÎÏ ÑÒÓÔ Ö  ÙÚÛÜÝ Y";
*/
static const m4char_t s_TablaMinToMay[]= "                                                                 " "                          " "      " "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "                                                                     " "                                ÀÁÂ Ä  ÇÈÉÊËÌÍÎÏ ÑÒÓÔ Ö  ÙÚÛÜÝ Y";

/* m4char_t s_TablaMayToMin[257]=
 "                                                                 "\
 "abcdefghijklmnopqrstuvwxyz"\
 "      "\
 "                          "\
 "                                                                     "\
 "àáâ ä  çèéêëìíîï ñòóô ö  ùúûüý                                  ";
*/
m4char_t s_TablaMayToMin[257]= "                                                                 " "abcdefghijklmnopqrstuvwxyz" "      " "                          " "                                                                     " "àáâ ä  çèéêëìíîï ñòóô ö  ùúûüý                                  ";


static const m4char_t*  s_TablaMonedaPos[]=
{
   "%m%n",
   "%n%m",
   "%m %n",
   "%n %m"
}; 

static const m4char_t*  s_TablaMonedaNeg[]=
{
   "(%m%n)",
   "-%m%n",
   "%m-n",
   "%m %n-",
   "(%n%m)",
   "-%n%m",
   "%n-%m",
   "%n%m-",
   "-%n %m",
   "-%m %n",
   "%n %m-"
}; 

static m4bool_t EsCero(m4char_t*  a_pc)
{
   m4bool_t b=M4_TRUE;
   
   for(;;){
      m4char_t a=*a_pc;
      
      if(a){
         if(a>'0' && a<='9'){
            // Encontrado un caracter en [1,9] -> no es cero
            b=M4_FALSE;
            break;
         }
         
         a_pc++;
      }
      else{
         break;
      }      
   }
   
   return b;
}


///////////////////////////////////////////////////////////////////////////////
// FMT_ClFormato

FMT_ClFormato::FMT_ClFormato(m4char_t*  a_pcNombre) : FMT_ClElement()
{                   
   m_pcNombre=NULL;
   IniciaNombre(a_pcNombre);

   D.Flags=0;      
   D.iAlig=ALIG_DER;        // alineado a la derecha por defecto
   D.iNoffsetstr=0;
   D.iCase=0;
   sCharsFill.Crea((m4char_t* )NULL); // Por defecto NULL   
   D.nCadenasSubst=0;      
   D.iLength=0;
   D.iBlank=0;
   sPlusInfDate.Crea((m4char_t* )NULL);
   sMinusInfDate.Crea((m4char_t* )NULL);

   D.iWordMode=0;
   D.iPrefixeMode=0;
   D.iSeparatorMode=0;

   m_spSubst=NULL;

   m4bool_t b=M4_TRUE;                                                                                     
   if(a_pcNombre!=NULL){
      // Hay que convertir a mayusculas
      b=M4_BOOL( strcmp(a_pcNombre, "WINDOWS") );
      if(!b){      
         // Bug 141465. Se implementa la funcionalidad de Regional Settings en UNIX
         CogeFormatoWinIni();                  
      }
      else{
         b=M4_BOOL( strcmp(a_pcNombre, "ISO") );
         if(!b){
            CogeFormatoIso();            
         }         
      }
   }
   
   if(b){
      // No inicializada, coge formato nulo :
      CogeFormatoNulo();
   }

	m_StPlugInInfo=NULL;
}

FMT_ClFormato::FMT_ClFormato(FMT_ClFormato  *a_pfFormatoOriginal) : FMT_ClElement()
{     
   // ojo, que no se inserta                     
   D.nCadenasSubst=0;   


   m_pcNombre=NULL;        

      m4int16_t i;
      ClCadena  *pp;

      memcpy(&D, &(a_pfFormatoOriginal->D), sizeof(D));      
      
      sThousand.CopiaDe( &(a_pfFormatoOriginal->sThousand      ));
      sDecimal.CopiaDe( &(a_pfFormatoOriginal->sDecimal       ));
      sCharsFill.CopiaDe( &(a_pfFormatoOriginal->sCharsFill     ));
      sCurrency.CopiaDe( &(a_pfFormatoOriginal->sCurrency      ));
      sMaskDate.CopiaDe( &(a_pfFormatoOriginal->sMaskDate      ));
      sMaskTimeStamp.CopiaDe( &(a_pfFormatoOriginal->sMaskTimeStamp ));
      sMaskTime24.CopiaDe( &(a_pfFormatoOriginal->sMaskTime24    ));
      sMaskTime12.CopiaDe( &(a_pfFormatoOriginal->sMaskTime12    ));
      sMaskCurPos.CopiaDe( &(a_pfFormatoOriginal->sMaskCurPos    ));
      sMaskCurNeg.CopiaDe( &(a_pfFormatoOriginal->sMaskCurNeg    ));
      sMaskNeg.CopiaDe( &(a_pfFormatoOriginal->sMaskNeg       ));

      m_spSubst=NULL;   
      pp=m_spSubst= new ClCadena[D.nCadenasSubst<<1];
      
      ClCadena  *pp2;      
      pp2=a_pfFormatoOriginal->m_spSubst;
      
      for(i=(D.nCadenasSubst<<1); i; i--)
         (pp++)->CopiaDe(pp2++);
}


void FMT_ClFormato::IniciaNombre(m4char_t*  a_pcNombre)
{  
   
   m4int16_t l=strlen(a_pcNombre);
   if(l){
      m_pcNombre=new m4char_t [l+1];
      strcpy(m_pcNombre, a_pcNombre);
   }
}                                                                             


FMT_ClFormato::~FMT_ClFormato()
{                                                                                
   if(m_pcNombre)
      delete m_pcNombre;
   
   if( m_spSubst!=NULL ) 
      delete []m_spSubst;    
      
   m_spSubst=NULL;
   AutoFree();

}
                        
        
m4int16_t FMT_ClFormato::SustituyePor(FMT_ClFormato  *a_pfFormatoNuevo)
{                                                  
   m4uint32_t h;      
   m4int16_t iError;
   
   h=m_uHandle;
                     
   AutoFree();               
   
   a_pfFormatoNuevo->AutoInsertIn(h);

   m_uHandle=NULL;  // para que el destructor no la cague
   delete this;
   iError=0;      
   
   return iError;
}

///////////////////////////////////////////////////////////////////////////////

void FMT_ClFormato::IniciaCadenaEntrada(ClCadena  *pCadIn, m4char_t*  a_pcIn)
{             
   m4uint32_t lon=0;
   m4uint32_t ini=0;
   
   if(a_pcIn){
      lon=strlen(a_pcIn);      
      if(D.Flags & FLAG_IBEGINSTR){
         // der a izq
         ini=0;
         lon=(D.iNoffsetstr<lon)? lon-D.iNoffsetstr : 0;
      }                              
      else{
         // izq a der
         ini=(D.iNoffsetstr<lon)? D.iNoffsetstr : lon;
         lon-=D.iNoffsetstr;
      }
   }
   
   pCadIn->Asigna(a_pcIn+ini, lon);
}

///////////////////////////////////////////////////////////////////////////////

void FMT_ClFormato::VerifyWindowsSettings(void)
{
	ClNode *oNode = NULL;
	m4VariantType value;
	ClAccess *oAccess = m_poTaskEnv->g_AccessDesign;

	// Se verifica si existe nodo de parametrización y si está cargado
	if (oAccess != NULL)
	{
		oNode = oAccess->Node.Get(M4NODE_WINDOWSFORMAT);
		if (oNode != NULL)
		{
			if ((M4_SUCCESS == oNode->Item["LOADED"].Value.Get(value)) && (value.Type == M4CL_CPP_TYPE_NUMBER)
				&& (value.Data.DoubleData == 0))
			{
				// No está cargado. Lo cargamos
				oNode->RecordSet.Item["LOAD_BLK"].Call();
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

UINT FMT_ClFormato::ReadWindowsSettingAsInt(LPCTSTR lpKeyName, INT nDefault)
{
	UINT iRetValue;

	ClNode *oNode = NULL;
	m4VariantType value;
	ClAccess *oAccess = m_poTaskEnv->g_AccessDesign;

	// Se lee primero por el nodo de parametrización
	if (oAccess != NULL)
	{
		oNode = oAccess->Node.Get(M4NODE_WINDOWSFORMAT);
		if (oNode != NULL)
		{
			if (M4_SUCCESS == oNode->Item[lpKeyName].Value.Get(value))
			{
				switch (value.Type)
				{
				case M4CL_CPP_TYPE_NUMBER:
					return value.Data.DoubleData ;
				
				case M4CL_CPP_TYPE_STRING_VAR:
					if (value.Data.PointerChar != NULL)
					{
						return atoi(value.Data.PointerChar);
					}
				}
			}
		}
	}

	// Si no está parametrizado se lee por los regional settings en Windows o su emulación en Unix
	iRetValue = GetPrivateProfileInt("intl", lpKeyName, nDefault, M4_PROFILE_FILE);

	return iRetValue;
}

///////////////////////////////////////////////////////////////////////////////

DWORD FMT_ClFormato::ReadWindowsSettingAsString(LPCTSTR lpKeyName, 
			      LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize)
{
	DWORD iRetValue = 0;

	ClNode *oNode = NULL;
	m4VariantType value;
	m4pchar_t stringValue;
	ClAccess *oAccess = m_poTaskEnv->g_AccessDesign;

	// Se lee primero por el nodo de parametrización
	if (oAccess != NULL && nSize > 0)
	{
		oNode = oAccess->Node.Get(M4NODE_WINDOWSFORMAT);
		if (oNode != NULL)
		{
			if ((M4_SUCCESS == oNode->Item[lpKeyName].Value.Get(value)) && (value.Type == M4CL_CPP_TYPE_STRING_VAR)
				&& (value.Data.PointerChar != NULL))
			{
				stringValue = value.Data.PointerChar;
				iRetValue = strlen(stringValue);
				if (iRetValue >= nSize) iRetValue = nSize - 1;
				strncpy(lpReturnedString, stringValue, iRetValue);
				lpReturnedString[iRetValue] = '\0';
				return iRetValue;
			}
		}
	}

	// Si no está parametrizado se lee por los regional settings en Windows o su emulación en Unix
	iRetValue = GetPrivateProfileString("intl", lpKeyName, lpDefault, lpReturnedString, nSize, M4_PROFILE_FILE);

	return iRetValue;
}

///////////////////////////////////////////////////////////////////////////////

// Bug 141465. Se implementa la funcionalidad de Regional Settings en UNIX
void FMT_ClFormato::CogeFormatoWinIni(void)
{
   m4uint32_t l;
   m4char_t buf[256];
   

#ifdef _UNIX
   // No es necesario realizar esta comprobación en Windows puesto que la sección
   // de los Regional Settings siempre existe
   if (0 == GetPrivateProfileSectionNames(buf, sizeof(buf), M4_PROFILE_FILE))
   {
	   // No ha sido posible encontrar el INI => Se mantiene el comportamiento por defecto
	   // (Se carga formato ISO)
	   CogeFormatoIso();
	   return;
   }
#endif

   // Se utiliza el nuevo lector de propiedades para obtener valores del canal
   // (parámetros de aplicación y prioritarios) y/o de la configuración de regional settings (Windows) o
   // su simulación en UNIX ("m4regionalsettings.ini")

   // Primero se verifica si los datos de nodo de propiedades están cargados
   VerifyWindowsSettings();

   // NUMERO ------------------------------------------------------------------                                                                            
   
   l=ReadWindowsSettingAsString("sThousand",  ".",   buf,  sizeof(buf));
   sThousand.Crea(buf, l);
   
   l=ReadWindowsSettingAsString("sDecimal",   ",",   buf,  sizeof(buf));
   sDecimal.Crea(buf, l);
   
   D.iCompDec=ReadWindowsSettingAsInt("iDigits",  2);
   D.iCompInt=0xffff;          // todos
   D.iDecGrup=0xffff;         // no se agrupan, van tal cual

   
   sMaskNeg.Crea("-%n");   // por defecto siempre es así en windows   
   
   if(ReadWindowsSettingAsInt("iLZero",  1))
      D.Flags |= FLAG_ILEFTZERO;   
   
   
   // FECHA -------------------------------------------------------------------
   
   // Coge el formato de fecha corta   
      
   // La implementación GetPrivateProfileString para UNIX necesita un carácter adicional
   // puesto que finaliza con un doble \0
   m4char_t sep[3];
   
   ReadWindowsSettingAsString("sDate",  "/", sep,  sizeof(sep));
      
   D.iDay=0;   
   D.iMonth=0;   
   D.iYear=0;
                   
   m4char_t estado[3]={M_NUEVO, M_NUEVO, M_NUEVO};                
   m4char_t mask[20];                              
         
   sprintf(mask, "d%cM%cyy", *sep, *sep);
   ReadWindowsSettingAsString("sShortDate",  mask, buf,  sizeof(buf));
   *mask='\0';
   
   m4char_t*  p=buf;
   m4bool_t sigue=M4_TRUE;
   for(;;){      
      switch(*p){
         case 'D':
         case 'd':            
            if(estado[DIA]!=M_CERRADO){
               if(estado[DIA]==M_NUEVO){
                  strcat(mask, "%d");               
                  estado[DIA]=M_DENTRO;
               }
               D.iDay++;
            }
            
            break;            
         
         case 'm':   
         case 'M':
            if(estado[MES]!=M_CERRADO){
               if(estado[MES]==M_NUEVO){
                  strcat(mask, "%m");               
                  estado[MES]=M_DENTRO;
               }
               D.iMonth++;
            }

            break;            
                  
         case 'Y':         
         case 'y':
            if(estado[ANIO]!=M_CERRADO){
               if(estado[ANIO]==M_NUEVO){
                  strcat(mask, "%y");               
                  estado[ANIO]=M_DENTRO;
               }
               D.iYear++;
            }            
            break;            
            
         case '\0':     
            // Fin
            sigue=M4_FALSE;            
            break;         
               
         default: 
            if(*p=*sep){   
               for(m4char_t i=DIA; i<=ANIO; i++){
                  if(estado[i]==M_DENTRO)
                     estado[i]=M_CERRADO;            
               }
               
               strcat(mask, sep);                                    
            }
            else{
               // Error en el WIN.INI -> ignora el caracter
            }            
      }
      
      if(sigue){
         p++;
      }
      else
         break;      
   }
   
   D.iDay = (D.iDay==2)? DNUMEROS_DOS : DNUMEROS_UNO;
   
   D.iMonth = (D.iMonth==2)? MNUMEROS_DOS : MNUMEROS_UNO;
          
   D.iYear = (D.iYear==2)? ANUMEROS_DOS : ANUMEROS_CUATRO;
      
   D.iWeekDay = WLETRAS_TODAS;   
   
   // comprueba ahora que el formato de la mascara sea correcto -> todos los           
   // estados deben ser M_CERRADO
   m4bool_t ok=M4_TRUE;
   for(m4char_t i=DIA; i<=ANIO; i++){
      if(estado[i]==M_NUEVO){
         ok=M4_FALSE;
         break;
      }
   }
      
   if(!ok){
       // error en el WIN.INI -> se coge la cadena por defecto
       sprintf(mask, "%%d%c%%m%c%%y", sep, sep);       
   }         
   
   sMaskDate.Crea(mask);

   
   
   // HORA --------------------------------------------------------------------
                             
   
   ReadWindowsSettingAsString("sTime",  ":", sep,  sizeof(sep));
   
   m4char_t buf2[64];
         
   ReadWindowsSettingAsString("s2359",  "", buf, sizeof(buf));
   
   l=sprintf(buf2, "%%h%c%%n%c%%s%s", *sep, *sep, buf);
   sMaskTime24.Crea(buf2, l+1);  // para que se null-termine
      
   l=ReadWindowsSettingAsString("s1159",  "", buf2, sizeof(buf2));
   
   if(strcmp(buf, buf2)){
      // si son diferentes:
      l=sprintf(buf, "%%h%c%%n%c%%s%s", *sep, *sep, buf2);
      sMaskTime12.Crea(buf, l+1);
   }   
   else
      sMaskTime12.Crea((m4char_t* )NULL);   
      
   D.iHour = ( ReadWindowsSettingAsInt("iTLZero",  0)==1 )? HNUMEROS_DOS : HNUMEROS_UNO;
   D.iMinute = NNUMEROS_DOS;
   D.iSecond = SNUMEROS_DOS;                                                                                   
                                                                               
   // TIMESTAMP ---------------------------------------------------------------
                                                    
   sprintf(buf, "%s %s", mask, sMaskTime24.DirIni());
   sMaskTimeStamp.Crea(buf);   

   D.Flags |= FLAG_ICAPAR;   // Por defecto capa
   
   
   // MONEDA ------------------------------------------------------------------
   
   m4uint32_t a;                           
   
   a=(UINT)ReadWindowsSettingAsInt("iCurrency",  0);
   if(a>3)
      a=0;      
   sMaskCurPos.Crea(s_TablaMonedaPos[a]);
      
   a=(UINT)ReadWindowsSettingAsInt("iNegCurr",  0);
   if(a>10)
      a=0;   
   sMaskCurNeg.Crea(s_TablaMonedaNeg[a]);      
      
   g_CargaCadena(IDS_NOMBRE_MONEDA, mask, sizeof(mask));   // para aprovechar mask

   l=ReadWindowsSettingAsString("sCurrency",  mask, buf, sizeof(buf));
   sCurrency.Crea(buf, l);
   
   D.iCurrDec=ReadWindowsSettingAsInt("iCurrDigits",  0);
}
    
///////////////////////////////////////////////////////////////////////////////

void FMT_ClFormato::CogeFormatoIso(void)
{                                           
   // NUMEROS -----------------------------------------------------------------
   sThousand.Crea((m4char_t* )NULL);   
   sDecimal.Crea(".");   
   D.iCompDec=0xffff;         // tal cual
   D.iCompInt=0xffff;
   D.iDecGrup=0xffff;         // no se agrupan, van tal cual

   sMaskNeg.Crea("-%n");
   D.Flags |= FLAG_ITRUNC | FLAG_ILEFTZERO;
      
   // FECHA -------------------------------------------------------------------
   D.iDay=DNUMEROS_DOS;
   D.iMonth=MNUMEROS_DOS;
   D.iYear=ANUMEROS_CUATRO;      
   D.iWeekDay=WLETRAS_TODAS;   // por ejemplo      
   sMaskDate.Crea("%y-%m-%d");   
   
   // HORA --------------------------------------------------------------------
   sMaskTime12.Crea((m4char_t* )NULL);      
   sMaskTime24.Crea("%h:%n:%s");   
   D.iHour = HNUMEROS_DOS;
   D.iMinute = NNUMEROS_DOS;
   D.iSecond = SNUMEROS_DOS;    
   
   // TIMESTAMP ---------------------------------------------------------------                                                       
   sMaskTimeStamp.Crea("%y-%m-%d %h:%n:%s");   
   
   // MONEDA ------------------------------------------------------------------
   sMaskCurPos.Crea((m4char_t* )NULL);
   sMaskCurNeg.Crea((m4char_t* )NULL);      
   sCurrency.Crea((m4char_t* )NULL);   
   D.iCurrDec=0xffff;   
}

///////////////////////////////////////////////////////////////////////////////

void FMT_ClFormato::CogeFormatoNulo(void)
{                                           
   // NUMEROS -----------------------------------------------------------------
   sThousand.Crea((m4char_t* )NULL);   
   sDecimal.Crea((m4char_t* )NULL);   
   D.iCompDec=0xffff;         // tal cual
   D.iCompInt=0xffff;   
   D.iDecGrup=0xffff;         // no se agrupan, van tal cual

   sMaskNeg.Crea((m4char_t* )NULL);         // tal cual
   D.Flags |= FLAG_ILEFTZERO;
      
   // FECHA -------------------------------------------------------------------
   D.iDay=DNUMEROS_DOS;
   D.iMonth=MNUMEROS_DOS;
   D.iYear=ANUMEROS_CUATRO;      
   D.iWeekDay=WLETRAS_TODAS;   // por ejemplo      
   sMaskDate.Crea((m4char_t* )NULL);   
   
   // HORA --------------------------------------------------------------------
   sMaskTime24.Crea((m4char_t* )NULL);         
   sMaskTime12.Crea((m4char_t* )NULL);         
   D.iHour = HNUMEROS_DOS;
   D.iMinute = NNUMEROS_DOS;
   D.iSecond = SNUMEROS_DOS;    
   
   // MONEDA ------------------------------------------------------------------
   sMaskCurPos.Crea((m4char_t* )NULL);
   sMaskCurNeg.Crea((m4char_t* )NULL);      
   sCurrency.Crea((m4char_t* )NULL);   
   D.iCurrDec=0xffff;   
}

///////////////////////////////////////////////////////////////////////////////    

m4int16_t FMT_ClFormato::Desformatea(ClCadena  *a_pCad, m4int16_t a_tipo)
{     
   m4int16_t iError;           
         
   // DES-ALINEADOS VARIOS ----------------------------------------------------
        
   m4uint32_t l=(UINT)a_pCad->LonReal();   
   m4uint32_t ini;
   m4uint32_t lon;     
   
   m_poTaskEnv->ClM4Fmt_g_pDD->sCharsFill.CopiaDe(&sCharsFill);
   
   m4int16_t il=m_poTaskEnv->ClM4Fmt_g_pDD->sCharsFill.LonReal();
   if(il){
      if(D.iCase)      
         m_poTaskEnv->ClM4Fmt_g_pDD->sCharsFill.Mapeo((D.iCase==1)? s_TablaMinToMay : s_TablaMayToMin);
      
      m_poTaskEnv->ClM4Fmt_g_pDD->sCharsFill.Posiciona(0, INICIO);   
      // il no es cero   
      m4char_t*  fill=new char[il];               
      m_poTaskEnv->ClM4Fmt_g_pDD->sCharsFill.LeeCadena(fill, il);    
         
      switch(D.iAlig){         
         case 1:       
         {
            // Centrado     
            ini=DesrellenaIzq(a_pCad, fill, il);
            lon=l-ini-DesrellenaDer(a_pCad, fill, il);                  
         }   
         break;
            
         case 2:      
         {
            // Izquierda         
            ini=0;
            lon=l-DesrellenaDer(a_pCad, fill, il);         
         }   
         break;   
         
         default:   
            // Derecha         
            ini=DesrellenaIzq(a_pCad, fill, il);
            lon=l-ini;         
      }   
      
      delete fill;
   }
   else{            
      ini=0;
      lon=l;
   }
   
   ClCadena cadAux;   
   cadAux.Asigna(a_pCad->DirIni()+ini, lon);         
   
   // Llamada a la función virtual --------------------------------------------
      
   m_poTaskEnv->ClM4Fmt_g_pDD->sCadenaOriginal.Asigna(a_pCad->DirIni()+ini, lon);            
            
   iError=DesformateaIdioma(&cadAux, a_tipo);
   
   
   // DES-SUSTITUCION::ANTES --------------------------------------------------
      
   // -------------------------------------------------------------------------
   
   return iError;
}

m4int16_t FMT_ClFormato::DesformateaIdioma(ClCadena  *a_pCad, m4int16_t a_tipo)
{
   m4int16_t iError;
                             
   iError=0;            
         
   switch(a_tipo){
      
      // ALFA -----------------------------------------------------------------
            
      // NUMERO ---------------------------------------------------------------
      
      case TIPONUMERO:
      {        
         m_poTaskEnv->ClM4Fmt_g_pDD->sThousand.CopiaDe(&sThousand);
         m_poTaskEnv->ClM4Fmt_g_pDD->sDecimal.CopiaDe(&sDecimal);   
            
         const m4char_t*  pcTabla;         
         if(D.iCase){
            pcTabla=(D.iCase==1)? s_TablaMinToMay : s_TablaMayToMin;      
            
            m_poTaskEnv->ClM4Fmt_g_pDD->sThousand.Mapeo(pcTabla);
            m_poTaskEnv->ClM4Fmt_g_pDD->sDecimal.Mapeo(pcTabla);                  
         }
         
         ClCadena psNum;
                                                       
         if(D.Flags & FLAG_ICUR){                     
            // MONEDA :          
            m_poTaskEnv->ClM4Fmt_g_pDD->sCurrency.CopiaDe(&sCurrency);
            if(D.iCase)
               m_poTaskEnv->ClM4Fmt_g_pDD->sCurrency.Mapeo(pcTabla);  
            
            iError=DesformateaNumero1(&psNum, a_pCad, &sMaskCurPos);
            
            if(!iError)
               m_poTaskEnv->ClM4Fmt_g_pDD->bNumNeg=M4_FALSE;
            else{
               iError=DesformateaNumero1(&psNum, a_pCad, &sMaskCurNeg);
               if(!iError)
                  m_poTaskEnv->ClM4Fmt_g_pDD->bNumNeg=M4_TRUE;
               else   
                  break;
            }            
         }
         else{            
            // NUMERO :
            m_poTaskEnv->ClM4Fmt_g_pDD->sCurrency.Asigna((m4char_t* )NULL);
            
            m4bool_t negativo;

            if(sMaskNeg.EsNula()){
               negativo=M4_FALSE;
            }
            else{
               iError=DesformateaNumero1(&psNum, a_pCad, &sMaskNeg);
               negativo= (!iError)? M4_TRUE : M4_FALSE;
            }

            if(negativo){
               m_poTaskEnv->ClM4Fmt_g_pDD->bNumNeg=M4_TRUE;               
            }
            else{
               m_poTaskEnv->ClM4Fmt_g_pDD->bNumNeg=M4_FALSE;
               psNum.Asigna(a_pCad->DirIni(), a_pCad->m_iLonBuffer);                     
            }                                           
         }         
         
         iError=DesformateaNumero2(&psNum);
      }
      break;
      
      
      // FECHA ----------------------------------------------------------------
      
      case TIPOFECHA:
      {            
         iError=DesformateaFechaHora(a_pCad, &sMaskDate);
      }
      break;
      
      
      // HORA -----------------------------------------------------------------
      
      case TIPO_TIME:
      {  
         m4bool_t nula=sMaskTime12.EsNula();
         iError=ERROR_DESFORTIME;
         
         if(!nula)
            iError=DesformateaFechaHora(a_pCad, &sMaskTime12);
            
         if(iError){
            iError=DesformateaFechaHora(a_pCad, &sMaskTime24);
               
            if(!iError){   
               if(!nula){
                  // la cadena encaja con la máscara de 24h
                  m4char_t d=*(m_poTaskEnv->ClM4Fmt_g_pDD->sHora);
                  m4char_t u=m_poTaskEnv->ClM4Fmt_g_pDD->sHora[1];
                  if(!u){
                     u=d;
                     d='0';
                  }
                  
                  u+=2;
                  if(u>'9'){
                     u+='0'-'9'-1;
                     d++;                  
                  }
                  
                  d++;
                  *(m_poTaskEnv->ClM4Fmt_g_pDD->sHora)=d;
                  m_poTaskEnv->ClM4Fmt_g_pDD->sHora[1]=u;
               }
            }
         }         
      }
      break;
      
      
      // TIMESTAMP ------------------------------------------------------------
      
      case M4ISOPOSDAY:
      {
         iError=DesformateaFechaHora(a_pCad, &sMaskTimeStamp);      
      }
      break;
       
      
      // TIPO NO CONTEMPLADO -------------------------------------------------- 
       
      // no hace nada.
   }
   
   return iError;
}


static m4int16_t (FMT_ClFormato::*TablaFuncionesLectura[])(m4int16_t, m4int16_t) =
{    
   NULL,
   &FMT_ClFormato::LeeAnio,
   &FMT_ClFormato::LeeMes,
   &FMT_ClFormato::LeeDiaSemana,
   &FMT_ClFormato::LeeDia,
   &FMT_ClFormato::LeeHora,
   &FMT_ClFormato::LeeMinuto,
   &FMT_ClFormato::LeeSegundo,
   &FMT_ClFormato::LeeMoneda,
   &FMT_ClFormato::LeeNumero
};

// Bug 0107547.
// static const m4char_t*  s_pcIndices;

m4int16_t FMT_ClFormato::DesformateaFechaHora(ClCadena  *a_pCad, ClCadena  *a_pMask)
{
   // Extrae datos de fecha y hora a partir de la mascara correspondiente

   m4int16_t iError;
   
   iError=0;
      
   strcpy(m_poTaskEnv->ClM4Fmt_g_pDD->sAnio, "0");
   strcpy(m_poTaskEnv->ClM4Fmt_g_pDD->sMes, "0");
   strcpy(m_poTaskEnv->ClM4Fmt_g_pDD->sDiaSemana, "0");
   strcpy(m_poTaskEnv->ClM4Fmt_g_pDD->sDia, "0");
   strcpy(m_poTaskEnv->ClM4Fmt_g_pDD->sHora, "0");
   strcpy(m_poTaskEnv->ClM4Fmt_g_pDD->sMinuto, "0");
   strcpy(m_poTaskEnv->ClM4Fmt_g_pDD->sSegundo, "0");            
   
   if(!a_pMask->EsNula()){
      m_poTaskEnv->Cadena_g_pCad=a_pCad;
      m_poTaskEnv->Cadena_g_pCad->Posiciona(0, INICIO);                
            
      m_poTaskEnv->Cadena_g_pMask=a_pMask;      
      m_poTaskEnv->Cadena_g_pMask->Posiciona(0, INICIO);                
            
      s_pcIndices = s_pcIndicesFechHora;
            
      iError=DesfRecur();
   }   

   return iError;
}

m4int16_t FMT_ClFormato::DesformateaNumero1(ClCadena  *a_pNum, ClCadena  *a_pCad, ClCadena  *a_pMask)
{
   // Extrae el número a partir de la mascara correspondiente

   m4int16_t iError;

   iError=0;
   
   if(a_pMask->EsNula()){
      a_pNum->Asigna(a_pCad->DirIni(), a_pCad->m_iLonBuffer);      
   }
   else{      
      m_poTaskEnv->Cadena_g_pNum=a_pNum;                                                   
      
      m_poTaskEnv->Cadena_g_pCad=a_pCad;                                 
            
      m_poTaskEnv->Cadena_g_pMask=a_pMask;
      m_poTaskEnv->Cadena_g_pMask->Posiciona(0, INICIO);   
      
      s_pcIndices=s_pcIndicesCurrNume;
            
      iError=DesfRecur();
   }      

   return iError;
}


m4int16_t FMT_ClFormato::DesformateaNumero2(ClCadena  *a_pNum)
{
   // dado un número, extrae la parte entera y decimal :
   m4int16_t iError;   
   m4int16_t l;
   ClCadena  *psCadenaDestino;
      
   iError=0;      
   l=a_pNum->m_iLonBuffer;      
   
   m_poTaskEnv->ClM4Fmt_g_pDD->sParteEntera.Crea(l);
   m_poTaskEnv->ClM4Fmt_g_pDD->sParteDecimal.Crea(l);
   
   psCadenaDestino=&(m_poTaskEnv->ClM4Fmt_g_pDD->sParteEntera);
      
   sDecimal.Posiciona(0, INICIO); 
   m4char_t cd=sDecimal.LeeChar();
   m4uint32_t ld=sDecimal.m_iLonBuffer-1;
   
   m4bool_t kk=M4_TRUE;
   
   for(;;){
      m4char_t c;
      c=a_pNum->LeeCharNA();

      if(c!='\0'){
         // comparamos con la cadena de los miles
         if(m_poTaskEnv->ClM4Fmt_g_pDD->sThousand.ComparaConNA(a_pNum)){
            // no coincide:                      
            // comparamos con la cadena de la coma
            if(m_poTaskEnv->ClM4Fmt_g_pDD->sDecimal.ComparaConNA(a_pNum)){
               // no coincide:
               // ... pues se escribe:         
                              
               psCadenaDestino->EscribeChar(c);
               kk=M4_FALSE;               
               
               // ... se avanza al siguiente caracter:
               a_pNum->Posiciona(1, ACTUAL);               
            }
            else{
               if(kk){
                  m_poTaskEnv->ClM4Fmt_g_pDD->sParteEntera.EscribeChar('0');
               
               }
               psCadenaDestino=&(m_poTaskEnv->ClM4Fmt_g_pDD->sParteDecimal);
            }
         }
      }
      else
         break;
   }                             
   
   m_poTaskEnv->ClM4Fmt_g_pDD->sParteEntera.EscribeChar('\0');
   m_poTaskEnv->ClM4Fmt_g_pDD->sParteDecimal.EscribeChar('\0');
      
   return iError;
}

m4int16_t FMT_ClFormato::DesfRecur(void)
{     
   m4char_t c, f;
   
   for(;;){
      c=m_poTaskEnv->Cadena_g_pMask->LeeChar();
          
      if(c=='%'){    
         c=m_poTaskEnv->Cadena_g_pMask->LeeChar();
         
         if(c!='%'){                
            m4uint32_t im, ic;
                        
            im=m_poTaskEnv->Cadena_g_pMask->m_iPos;
            ic=m_poTaskEnv->Cadena_g_pCad->m_iPos;            

            m4char_t indice;
            
            indice=(s_pcIndices[c]);
            if(indice!=' ')               
               return (this->*TablaFuncionesLectura[indice])(im, ic);
            
            f=m_poTaskEnv->Cadena_g_pCad->LeeChar();
               
            if(f!='%')
               return ERROR_FORMATOENTRADA;            
         }         
      }                   

      f=m_poTaskEnv->Cadena_g_pCad->LeeChar();
                     
      if(c!=f)
         return ERROR_FORMATOENTRADA;               
         
      if(!c)
         return 0;   
   }

   return 0;
}

m4int16_t FMT_ClFormato::LeeAnio(m4int16_t im, m4int16_t ic)
{
   m4int16_t iError;

   switch(D.iYear){
      case ANUMEROS_DOS:      
         m_poTaskEnv->Cadena_g_pCad->Posiciona(2, ACTUAL);
         iError=DesfRecur();
         if(!iError){           
            m_poTaskEnv->Cadena_g_pCad->Posiciona(ic, INICIO);
            m_poTaskEnv->Cadena_g_pCad->LeeCadena(m_poTaskEnv->ClM4Fmt_g_pDD->sAnio, 2);
            m_poTaskEnv->ClM4Fmt_g_pDD->sAnio[2]='\0';
         }                        
         break;                        
                        
      case ANUMEROS_CUATRO:
         m_poTaskEnv->Cadena_g_pCad->Posiciona(4, ACTUAL);
         iError=DesfRecur();
         if(!iError){           
            m_poTaskEnv->Cadena_g_pCad->Posiciona(ic, INICIO);
            m_poTaskEnv->Cadena_g_pCad->LeeCadena(m_poTaskEnv->ClM4Fmt_g_pDD->sAnio, 4);
            m_poTaskEnv->ClM4Fmt_g_pDD->sAnio[4]='\0';
         }                        
         break;
                        
      case ANUMEROS_ALETRAS:
      {                          
         m4int16_t l=0;
         m4int16_t k=ic;
         for(;;){
            iError=DesfRecur();
            if(!iError){
               m_poTaskEnv->Cadena_g_pCad->Posiciona(ic, INICIO);
               m_poTaskEnv->Cadena_g_pCad->LeeCadena(m_poTaskEnv->ClM4Fmt_g_pDD->sAnio, l);
               m_poTaskEnv->ClM4Fmt_g_pDD->sAnio[l]='\0';
               // Convierte buf a cadena numérica
               // no implementado
               break;
            }
            else{
               m_poTaskEnv->Cadena_g_pMask->Posiciona(im, INICIO);
               m_poTaskEnv->Cadena_g_pCad->Posiciona(++k, INICIO);               
               
               if(m_poTaskEnv->Cadena_g_pCad->IsEOF()){
                  iError=ERROR_DESFORANIO;     
                  break;
               }
               
               l++;
            }
         }
         break;                        
      }
                        
      default:
         iError=ERROR_DESFORANIO;     
   }                  

   return iError;
}


m4int16_t FMT_ClFormato::LeeMes(m4int16_t im, m4int16_t ic)
{        
   m4int16_t iError;

   switch(D.iMonth){
      case MNUMEROS_UNO:      
         m_poTaskEnv->Cadena_g_pCad->Posiciona(1, ACTUAL);
         iError=DesfRecur();
         if(!iError){           
            m_poTaskEnv->Cadena_g_pCad->Posiciona(ic, INICIO);
            m_poTaskEnv->Cadena_g_pCad->LeeCadena(m_poTaskEnv->ClM4Fmt_g_pDD->sMes, 1);
            m_poTaskEnv->ClM4Fmt_g_pDD->sMes[1]='\0';
         }
                        
         if(!iError)
            break;
         else{                            
            m_poTaskEnv->Cadena_g_pMask->Posiciona(im, INICIO);
            m_poTaskEnv->Cadena_g_pCad->Posiciona(ic, INICIO);
         }
         //break;
                        
         case MNUMEROS_DOS:
            m_poTaskEnv->Cadena_g_pCad->Posiciona(2, ACTUAL);
            iError=DesfRecur();
            if(!iError){           
               m_poTaskEnv->Cadena_g_pCad->Posiciona(ic, INICIO);
               m4char_t z;
               z=m_poTaskEnv->Cadena_g_pCad->LeeChar();
               m4char_t*  p=m_poTaskEnv->ClM4Fmt_g_pDD->sMes;
               if(z!='0')
                  *(p++)=z;
               *(p++)=m_poTaskEnv->Cadena_g_pCad->LeeChar();
               *p='\0';
            }                        
            break;
                        
         case MLETRAS_TRES:
            m_poTaskEnv->Cadena_g_pCad->Posiciona(3, ACTUAL);
            iError=DesfRecur();
            if(!iError){           
               m4char_t buf[4];
               m_poTaskEnv->Cadena_g_pCad->Posiciona(ic, INICIO);
               m_poTaskEnv->Cadena_g_pCad->LeeCadena(buf, 3);
               
               // Convierte buf a cadena numérica
               m4char_t m=BuscaCadenaMes(buf,3,12);
               if(m){                   
                  m4char_t*  p=m_poTaskEnv->ClM4Fmt_g_pDD->sMes;
                  
                  if(m>9){           
                     *(p++)='1';                     
                     *(p++)=m-10+'0';                     
                  }   
                  else{
                     *(p++)=m+'0';                     
                  }                     
                  
                  *p='\0';
               }
               else
                  iError=ERROR_DESFORMES1;
            }                        
            break;
   
         case MLETRAS_TODAS:
         {                          
            m4int16_t l=0;
            m4int16_t k=ic;
            for(;;){
               iError=DesfRecur();
               if(!iError){
                  m4char_t buf[64];
                  m_poTaskEnv->Cadena_g_pCad->Posiciona(ic, INICIO);
                  m_poTaskEnv->Cadena_g_pCad->LeeCadena(buf, l);
                  
                  // Convierte buf a cadena numérica
                  m4char_t m=BuscaCadenaMes(buf,l,12);
                  if(m){                   
                     m4char_t*  p=m_poTaskEnv->ClM4Fmt_g_pDD->sMes;
                  
                     if(m>9){
                        *(p++)='1';                     
                        *(p++)=m-10+'0';                                        
                     }   
                     else{
                        *(p++)=m+'0';                     
                     }                     
                  
                     *p='\0';
                  }
                  else
                     iError=ERROR_DESFORMES1;
                  break;
               }
               else{
                  m_poTaskEnv->Cadena_g_pMask->Posiciona(im, INICIO);
                  m_poTaskEnv->Cadena_g_pCad->Posiciona(++k, INICIO);
                  
                  if(m_poTaskEnv->Cadena_g_pCad->IsEOF()){
                     iError=ERROR_DESFORANIO;     
                     break;
                  }  
                  
                  l++;
               }
            }
            break;                        
         }
                        
      default:
         iError=ERROR_DESFORMES;     
   }

   return iError;
}


m4int16_t FMT_ClFormato::LeeDiaSemana(m4int16_t im, m4int16_t ic)
{                                
   m4int16_t iError;

   switch(D.iWeekDay){
      case WNUMEROS_UNO:      
         m_poTaskEnv->Cadena_g_pCad->Posiciona(1, ACTUAL);
         iError=DesfRecur();
         if(!iError){           
            m_poTaskEnv->Cadena_g_pCad->Posiciona(ic, INICIO);
            m_poTaskEnv->Cadena_g_pCad->LeeCadena(m_poTaskEnv->ClM4Fmt_g_pDD->sDiaSemana, 1);
            m_poTaskEnv->ClM4Fmt_g_pDD->sDiaSemana[1]='\0';
         }                                     
         break;
                        
         case WLETRAS_TRES:
            m_poTaskEnv->Cadena_g_pCad->Posiciona(3, ACTUAL);
            iError=DesfRecur();
            if(!iError){           
               m4char_t buf[4];
               m_poTaskEnv->Cadena_g_pCad->Posiciona(ic, INICIO);
               m_poTaskEnv->Cadena_g_pCad->LeeCadena(buf, 3);               
               
               // Convierte buf a cadena numérica
               m4char_t w=BuscaCadenaSemana(buf, 3, 7);
               if(w){               
                  *m_poTaskEnv->ClM4Fmt_g_pDD->sDiaSemana=w+'0';
                  m_poTaskEnv->ClM4Fmt_g_pDD->sDiaSemana[1]='\0';
               }
               else
                  iError=ERROR_DESFORSEM1;
            }                        
            break;
   
         case WLETRAS_TODAS:
         {                          
            m4int16_t l=0;
            m4int16_t k=ic;
            for(;;){
               iError=DesfRecur();
               if(!iError){
                  m4char_t buf[64];
                  m_poTaskEnv->Cadena_g_pCad->Posiciona(ic, INICIO);
                  m_poTaskEnv->Cadena_g_pCad->LeeCadena(buf, l);
                  
                  // Convierte buf a cadena numérica                  
                  m4char_t w=BuscaCadenaSemana(buf, l, 7);
                  if(w){               
                     *m_poTaskEnv->ClM4Fmt_g_pDD->sDiaSemana=w+'0';
                     m_poTaskEnv->ClM4Fmt_g_pDD->sDiaSemana[1]='\0';
                  }
                  else
                     iError=ERROR_DESFORSEM1;
                  break;
               }
               else{
                  m_poTaskEnv->Cadena_g_pMask->Posiciona(im, INICIO);
                  m_poTaskEnv->Cadena_g_pCad->Posiciona(++k, INICIO);
                  
                  if(m_poTaskEnv->Cadena_g_pCad->IsEOF()){
                     iError=ERROR_DESFORANIO;     
                     break;
                  }
                  
                  l++;
               }
            }
            break;                        
         }
                        
      default:
         iError=ERROR_DESFORSEM;
   }               

   return iError;
}


m4int16_t FMT_ClFormato::LeeDia(m4int16_t im, m4int16_t ic)
{
   m4int16_t iError;

   switch(D.iDay){
      case DNUMEROS_UNO:      
         m_poTaskEnv->Cadena_g_pCad->Posiciona(1, ACTUAL);
         iError=DesfRecur();
         if(!iError){           
            m_poTaskEnv->Cadena_g_pCad->Posiciona(ic, INICIO);
            m_poTaskEnv->Cadena_g_pCad->LeeCadena(m_poTaskEnv->ClM4Fmt_g_pDD->sDia, 1);
            m_poTaskEnv->ClM4Fmt_g_pDD->sDia[1]='\0';
         }
                        
         if(!iError)
            break;
         else{                            
            m_poTaskEnv->Cadena_g_pMask->Posiciona(im, INICIO);
            m_poTaskEnv->Cadena_g_pCad->Posiciona(ic, INICIO);
         }
         //break;
                        
      case DNUMEROS_DOS:
         m_poTaskEnv->Cadena_g_pCad->Posiciona(2, ACTUAL);
         iError=DesfRecur();
         if(!iError){           
            m_poTaskEnv->Cadena_g_pCad->Posiciona(ic, INICIO);
            m4char_t z;
            z=m_poTaskEnv->Cadena_g_pCad->LeeChar();
            m4char_t*  p=m_poTaskEnv->ClM4Fmt_g_pDD->sDia;
            if(z!='0')
               *(p++)=z;
            *(p++)=m_poTaskEnv->Cadena_g_pCad->LeeChar();
            *p='\0';
         }
         break;                        
  
      case DNUMEROS_ALETRAS:
      {                          
         m4int16_t l=0;
         m4int16_t k=ic;
         for(;;){
            iError=DesfRecur();
            if(!iError){
               m4char_t buf[64];
               m_poTaskEnv->Cadena_g_pCad->Posiciona(ic, INICIO);
               m_poTaskEnv->Cadena_g_pCad->LeeCadena(buf, l);
               buf[l]='\0';
               // Convierte buf a cadena numérica
               // no implementado
               break;
            }
            else{
               m_poTaskEnv->Cadena_g_pMask->Posiciona(im, INICIO);
               m_poTaskEnv->Cadena_g_pCad->Posiciona(++k, INICIO);
               
               if(m_poTaskEnv->Cadena_g_pCad->IsEOF()){
                  iError=ERROR_DESFORANIO;     
                  break;
               }
               
               l++;
            }
         }
         break;                        
      }
                        
      default:
         iError=ERROR_DESFORDIA;     
   }

   return iError;
}


m4int16_t FMT_ClFormato::LeeHora(m4int16_t im, m4int16_t ic)
{
   m4int16_t iError;

   switch(D.iHour){
      case HNUMEROS_UNO:      
         m_poTaskEnv->Cadena_g_pCad->Posiciona(1, ACTUAL);
         iError=DesfRecur();
         if(!iError){           
            m_poTaskEnv->Cadena_g_pCad->Posiciona(ic, INICIO);
            m_poTaskEnv->Cadena_g_pCad->LeeCadena(m_poTaskEnv->ClM4Fmt_g_pDD->sHora, 1);
            m_poTaskEnv->ClM4Fmt_g_pDD->sHora[1]='\0';
         }
                        
         if(!iError)
            break;
         else{                            
            m_poTaskEnv->Cadena_g_pMask->Posiciona(im, INICIO);
            m_poTaskEnv->Cadena_g_pCad->Posiciona(ic, INICIO);
         }
         //break;
                        
      case HNUMEROS_DOS:
         m_poTaskEnv->Cadena_g_pCad->Posiciona(2, ACTUAL);
         iError=DesfRecur();
         if(!iError){           
            m_poTaskEnv->Cadena_g_pCad->Posiciona(ic, INICIO);
            m4char_t z;
            z=m_poTaskEnv->Cadena_g_pCad->LeeChar();
            m4char_t*  p=m_poTaskEnv->ClM4Fmt_g_pDD->sHora;
            if(z!='0')
               *(p++)=z;
            *(p++)=m_poTaskEnv->Cadena_g_pCad->LeeChar();
            *p='\0';
         }
         break;
                        
      default:
         iError=ERROR_DESFORHORA;
   }      

   return iError;
}


m4int16_t FMT_ClFormato::LeeMinuto(m4int16_t im, m4int16_t ic)
{
   m4int16_t iError;
      
   switch(D.iMinute){
      case NNUMEROS_UNO:      
         m_poTaskEnv->Cadena_g_pCad->Posiciona(1, ACTUAL);
         iError=DesfRecur();
         if(!iError){           
            m_poTaskEnv->Cadena_g_pCad->Posiciona(ic, INICIO);
            m_poTaskEnv->Cadena_g_pCad->LeeCadena(m_poTaskEnv->ClM4Fmt_g_pDD->sMinuto, 1);
            m_poTaskEnv->ClM4Fmt_g_pDD->sMinuto[1]='\0';
         }
                        
         if(!iError)
            break;
         else{                            
            m_poTaskEnv->Cadena_g_pMask->Posiciona(im, INICIO);
            m_poTaskEnv->Cadena_g_pCad->Posiciona(ic, INICIO);
         }
         //break;
                        
      case NNUMEROS_DOS:
         m_poTaskEnv->Cadena_g_pCad->Posiciona(2, ACTUAL);
         iError=DesfRecur();
         if(!iError){           
            m_poTaskEnv->Cadena_g_pCad->Posiciona(ic, INICIO);
            m4char_t z;            
            z=m_poTaskEnv->Cadena_g_pCad->LeeChar();
            m4char_t*  p=m_poTaskEnv->ClM4Fmt_g_pDD->sMinuto;
            if(z!='0')
               *(p++)=z;
            *(p++)=m_poTaskEnv->Cadena_g_pCad->LeeChar();
            *p='\0';
         }                        
         break;
                        
      default:
         iError=ERROR_DESFORMIN;        
   }

   return iError;
}


m4int16_t FMT_ClFormato::LeeSegundo(m4int16_t im, m4int16_t ic)
{
   m4int16_t iError; 
   m4int16_t l=0;
   m4int16_t k=ic;
   m4bool_t st=M4_FALSE;
   
   for(;;){
      iError=DesfRecur();
      if(!iError){
         m4char_t buf[2];         
         *buf='\0';
         m_poTaskEnv->Cadena_g_pCad->Posiciona(ic, INICIO);
         m_poTaskEnv->Cadena_g_pCad->LeeCadena(buf, 2); 
                  
         m4char_t z=*buf;         
         m4char_t y=buf[1];
         m4char_t*  p=m_poTaskEnv->ClM4Fmt_g_pDD->sSegundo;
         
         if(z=='0'){    
            if(y=='\0'){
               iError=ERROR_DESFORSEG;
               break;
            }
               
            *(p++)= y;            
         }
         else{                  
            if(z=='\0'){
               iError=ERROR_DESFORSEG;
               break;                           
            }
            else{               
               *(p++)=z;                 
               
               if(y!='.'){               
                  *(p++)=y;               
               }            
            }
         }         
         
         *p='\0';
         
         break;
      }
      else{
         if(m_poTaskEnv->Cadena_g_pCad->IsEOF()){
            if(st){
               iError=ERROR_DESFORSEG;
               break;
            }
            else{
               st=M4_TRUE;
            }
         }
         
         m_poTaskEnv->Cadena_g_pMask->Posiciona(im, INICIO);
         m_poTaskEnv->Cadena_g_pCad->Posiciona(++k, INICIO);         
         
         l++;
      }
   }     

   return iError;
}

m4int16_t FMT_ClFormato::LeeMoneda(m4int16_t, m4int16_t)
{     
   return m_poTaskEnv->ClM4Fmt_g_pDD->sCurrency.ComparaCon(m_poTaskEnv->Cadena_g_pCad)? ERROR_DESFORMONEDA : DesfRecur();
}

m4int16_t FMT_ClFormato::LeeNumero(m4int16_t im, m4int16_t ic)
{
   m4int16_t iError;

   // Longitud del numero desconocida

   m4int16_t l=0;
   m4int16_t k=ic;

   for(;;){
      iError=DesfRecur();
      if(!iError){
         m_poTaskEnv->Cadena_g_pNum->Asigna(m_poTaskEnv->Cadena_g_pCad->DirIni()+ic, l);
         break;
      }
      else{
         m_poTaskEnv->Cadena_g_pMask->Posiciona(im, INICIO);
         m_poTaskEnv->Cadena_g_pCad->Posiciona(++k, INICIO);                    
         l++;
      }
   }
   
   return iError;
}


///////////////////////////////////////////////////////////////////////////////    

m4int16_t FMT_ClFormato::CaseFormat(m4char_t *ai_pcCadena, m4int16_t a_tipo)
{
	ClMIT_Str Key, oStrAux, oStrBuffer, oStrSeparators;
	ClCadena  Cadena;
	ClMIT_Args Args ;
	StPlugInData::List_t::iterator FoundIt ;
	m4int16_t Size, iError = EXITO;
	m4uint32_t DataSource;
	m4bool_t  Found;
	m4char_t  *TmpBuffer ;
	size_t	  LenSep, LenWord;

	if(a_tipo!=TIPOALFA) goto exit;

	TmpBuffer = new m4char_t[strlen(ai_pcCadena)+1];
	strcpy(TmpBuffer,ai_pcCadena);

	//->Formamos la cadena de separadores para el strspn
	oStrSeparators.StrCpy(M4_STANDARD_SEPARATORS);
	if(D.iSeparatorMode==2 || D.iSeparatorMode==3){
		FoundIt=m_StPlugInInfo->SepList_t.begin() ;
		while ( FoundIt!=m_StPlugInInfo->SepList_t.end() ) {
			Key=(*FoundIt).first;
			oStrSeparators.StrCat(Key.InChar(0));
			++FoundIt ;
		} ;
	};
	if(D.iSeparatorMode==1 || D.iSeparatorMode==3){
		FoundIt=ExtSepList_t.begin() ;
		while ( FoundIt!=ExtSepList_t.end() ) {
			Key=(*FoundIt).first;
			oStrSeparators.StrCat(Key.InChar(0));
			++FoundIt ;
		} ;
	};
	//<-Fin
	
	while(*TmpBuffer){

		LenSep = strspn(TmpBuffer,oStrSeparators.InChar(0)) ; //Len de cadena inicial con todo Separadores
		
		if(LenSep){
			oStrAux.ToASCIZ(TmpBuffer,LenSep);
			oStrBuffer.StrCat(oStrAux.InChar(0));
			TmpBuffer+=LenSep;
		};

		LenWord = strcspn(TmpBuffer,oStrSeparators.InChar(0)) ; //Len de cadena inicial sin separadores

		if(LenWord){
			Args.Init(TmpBuffer,ClMIT_Args::NO_SUBARG,oStrSeparators.InChar(0));
			Key.ToASCIZ(Args.GetArg(0),strlen(Args.GetArg(0)));
			TmpBuffer+=Key.StrLen();

			//->Buscamos palabras reservadas
			Found=M4_FALSE;
			if(D.iWordMode==3 || D.iWordMode==2){
				FoundIt=m_StPlugInInfo->WordList_t.begin() ;
				while ( FoundIt!=m_StPlugInInfo->WordList_t.end() ) {
					if ( (*FoundIt).first==Key ) {
						Found=M4_TRUE ;
						Key=(*FoundIt).first;
						break ;
					} ;
					++FoundIt ;
				} ;
			};
			if(D.iWordMode==1 || D.iWordMode==3){
				FoundIt=ExtWordList_t.begin() ;
				while ( FoundIt!=ExtWordList_t.end() ) {
					if ( (*FoundIt).first==Key ) {
						Found=M4_TRUE ;
						Key=(*FoundIt).first;
						break ;
					} ;
					++FoundIt ;
				} ;
			};
			if(Found){
				//ENCONTRADA
				oStrBuffer.StrCat(Key.InChar(0));
			} else {
				//NO //->Buscamos Prefijos
				Found=M4_FALSE;
				if(D.iPrefixeMode==3 || D.iPrefixeMode==2){
					FoundIt=m_StPlugInInfo->PrefixList_t.begin() ;
					while ( FoundIt!=m_StPlugInInfo->PrefixList_t.end() ) {
						if ( !strnicmp(Key.InChar(0),(*FoundIt).first.InChar(0), (*FoundIt).first.StrLen()) ) {
							Found=M4_TRUE ;
							oStrAux.ToASCIZ((*FoundIt).first.InChar(0),(*FoundIt).first.StrLen());
							break ;
						} ;
						++FoundIt ;
					};
				};
				if(D.iPrefixeMode==1 || D.iPrefixeMode==3){
					FoundIt=ExtPrefList_t.begin() ;
					while ( FoundIt!=ExtPrefList_t.end() ) {
						if ( !strnicmp(Key.InChar(0),(*FoundIt).first.InChar(0), (*FoundIt).first.StrLen()) ) {
							Found=M4_TRUE ;
							oStrAux.ToASCIZ((*FoundIt).first.InChar(0),(*FoundIt).first.StrLen());
							break ;
						} ;
						++FoundIt ;
					};
				};
				if(Found && Key.StrLen()>oStrAux.StrLen()){
					oStrBuffer.StrCat(oStrAux.InChar(0));
					oStrAux.ToASCIZ(Key.InChar(oStrAux.StrLen()),Key.StrLen()-oStrAux.StrLen());
					Key.ToASCIZ(oStrAux.InChar(0),oStrAux.StrLen());
				};
				//->NO ENCONTRADO NADA. PROCESO NORMAL
				Cadena.Crea(Key.InChar(0),1);
				Cadena.Mapeo(s_TablaMinToMay);
				oStrAux.ToASCIZ(Cadena.DirIni(),strlen(Cadena.DirIni()));
				if(Key.StrLen()>1){
					Cadena.Crea(Key.InChar(1));
					Cadena.Mapeo(s_TablaMayToMin);
					oStrAux.StrCat(Cadena.DirIni());
				};
				oStrBuffer.StrCat(oStrAux.InChar(0));
			};
			//<-Fin
		};

	};

	strcpy(ai_pcCadena,oStrBuffer.InChar(0));
	
exit:

	return iError;
}


m4int16_t FMT_ClFormato::SentenceCase(m4char_t *ai_pcCadena, m4int16_t a_tipo)
{
	m4int16_t iError = EXITO;
	ClMIT_Str oStrBuffer, KeySentence;
	ClCadena  Cadena;

	if(a_tipo!=TIPOALFA) {
		goto exit;
	};

	KeySentence.ToASCIZ(ai_pcCadena, strlen(ai_pcCadena));
	Cadena.Crea(KeySentence.InChar(0), 1);
	Cadena.Mapeo(s_TablaMinToMay);
	oStrBuffer.ToASCIZ(Cadena.DirIni(),strlen(Cadena.DirIni()));
	if(KeySentence.StrLen()>1) {
		Cadena.Crea(KeySentence.InChar(1));
		Cadena.Mapeo(s_TablaMayToMin);
		oStrBuffer.StrCat(Cadena.DirIni());
	};
	
	strcpy(ai_pcCadena,oStrBuffer.InChar(0));

exit:

	return iError;
}


m4int16_t FMT_ClFormato::Formatea(ClCadena  *a_pCad, m4int16_t a_tipo)
{     
   m4int16_t iError;
      
   // Llamada a la función virtual --------------------------------------------
   
   ClCadena cadAux;   
   cadAux.Crea(a_pCad->m_iLonBuffer);
   
   iError=FormateaIdioma(&cadAux, a_tipo);
   
   if(!iError){

	  // ACABA EN '\0' -----------------------------------------------------------
                                
      cadAux.EscribeChar('\0');
                                   
      cadAux.Posiciona(0, FINAL);                          
      cadAux.EscribeChar('\0');   
      
      
      // ALINEADOS VARIOS --------------------------------------------------------
      m4uint32_t l=(UINT)cadAux.LonReal();   
      m4uint32_t L=D.iLength;
      if(!L)
         L=l;
                      
      if(L>l){
         if(sCharsFill.EsNula()){
            cadAux.Posiciona(0, INICIO);            
            a_pCad->EscribeCadena(&cadAux, L);
         }
         else{   
            switch(D.iAlig){         
               case 1:       
               {
                  // Centrado                 
                  m4int16_t k=(L-l)>>1;
                  cadAux.Posiciona(0, INICIO);      
                  RellenaIzq(a_pCad, k);            
                  a_pCad->EscribeCadena(&cadAux, l);
                  RellenaDer(a_pCad, L-l-k);
            
            
               }   
               break;
                  
               case 2:      
               {
                  // Izquierda                     
                  cadAux.Posiciona(0, INICIO);      
                  a_pCad->EscribeCadena(&cadAux, l);            
                  RellenaDer(a_pCad, L-l);            
            
               }   
               break;   
               
               default:   
                  // Derecha            
                  cadAux.Posiciona(0, INICIO);      
                  RellenaIzq(a_pCad, L-l);                        
                  a_pCad->EscribeCadena(&cadAux, l);                  
            
            }
         }
      }  
      else{                    
         switch(D.iAlig){         
            case 1:       
            {
               // Centrado           
               m4int16_t k=(l-L)>>1;
               cadAux.Posiciona(k, INICIO);
               a_pCad->EscribeCadena(&cadAux, L);
               
            }   
            break;
                  
            case 2:      
            {
               // Izquierda                           
               cadAux.Posiciona(0, INICIO);      
               a_pCad->EscribeCadena(&cadAux, L);
      
            }   
            break;   
               
            default:   
               // Derecha                  
               cadAux.Posiciona(l-L, INICIO);
               a_pCad->EscribeCadena(&cadAux, L);         
      
         }
      }
      
      a_pCad->EscribeChar('\0');      
   }
      
   return iError;
}  

m4int16_t FMT_ClFormato::SustituyeAntes(ClCadena  **a_ppCad)
{
   return (D.Flags & FLAG_ISUBST)? 0 : Sustituye(a_ppCad);
}

m4int16_t FMT_ClFormato::SustituyeDespues(ClCadena  **a_ppCad)
{
   return (D.Flags & FLAG_ISUBST)? Sustituye(a_ppCad) : 0;
}

void FMT_ClFormato::RellenaIzq(ClCadena  *a_pCad, m4int16_t a_iL)
{     
   m4int16_t il=sCharsFill.LonReal();   
   // il no es cero
   
   while(a_iL>=il){
      a_pCad->EscribeCadena(&sCharsFill, il);      
      a_iL-=il;
   }
   
   if(a_iL)
      a_pCad->EscribeCadena(&sCharsFill, a_iL);   
}

void FMT_ClFormato::RellenaDer(ClCadena  *a_pCad, m4int16_t a_iL)
{     
   m4int16_t il=sCharsFill.LonReal();
   m4int16_t resto=a_iL%il;
   
   if(resto){
      a_pCad->EscribeCadena(sCharsFill.m_pDirIni+il-resto, resto);
      a_iL-=resto;
   }      
   
   while(a_iL>=il){
      a_pCad->EscribeCadena(&sCharsFill, il);      
      a_iL-=il;
   }   
}

m4int16_t FMT_ClFormato::DesrellenaIzq(ClCadena  *a_pCad, m4char_t*  fill, m4int16_t il)
{                 
   // devuelve la posicion del primer caracter en la cadena que no se corresponde con los de relleno
   // lee la cadena de izquierda a derecha
   m4int16_t coincidentes=0;
            
   m4int16_t i=0;   
   for(;;){   
      m4char_t c=a_pCad->LeeChar();
      if(c==fill[i++]){            
         coincidentes++;
            
         if(i==il)
            i=0;
      }
      else{
         // no coincide :
         a_pCad->Posiciona(-1, ACTUAL);            
         break;
      }      
   }
      
   return coincidentes;
}

m4int16_t FMT_ClFormato::DesrellenaDer(ClCadena  *a_pCad, m4char_t*  fill, m4int16_t il)
{     
   // devuelve la posicion del primer caracter en la cadena que no se corresponde con los de relleno
   // lee la cadena de derecha a izquierda
   m4int16_t coincidentes=0;
      
   a_pCad->Posiciona(0, FINAL);
         
   m4int16_t i=il-1;   
   for(;;){   
      m4char_t c=a_pCad->LeeCharPaTras();
      if(c==fill[i--]){            
         coincidentes++;
   
         if(i==-1)
            i=il-1;
      }
      else{
         // no coincide :
         //a_pCad.Posiciona(-1, ACTUAL);            
         break;
      }      
   }
         
   return coincidentes;
}

m4int16_t FMT_ClFormato::Sustituye(ClCadena  **a_ppCad)
{                   
   if(D.nCadenasSubst){
    if(!(*a_ppCad)->EsNula()){
      ClCadena  *pCadDestino=NULL;
         
      (*a_ppCad)->Posiciona(0, INICIO);      
      m4uint32_t lon=(*a_ppCad)->LonReal()+256*D.nCadenasSubst;
      
      m4bool_t bSus=M4_FALSE;
      
      m4char_t buf[256];
      m4char_t*  p=buf;
            
      for(;;){
         m4int16_t i;  
         i=D.nCadenasSubst;                             
         
         ClCadena  *ppOrigen=m_spSubst;
         ClCadena  *ppDestino=m_spSubst+D.nCadenasSubst;
         
         m4bool_t loc=M4_FALSE;
         
         for(;;){
            if(!ppOrigen->ComparaConNA(*a_ppCad)){
               // coincide:
               if(!bSus){
                  bSus=M4_TRUE;
                  pCadDestino=new ClCadena;
                  pCadDestino->Crea(lon);
               }
               pCadDestino->EscribeCadena(buf, p-buf);
               pCadDestino->EscribeCadena(ppDestino, ppDestino->LonReal());
               p=buf;      
               
               loc=M4_TRUE;
            }   
            
            if(--i){         
               ppOrigen++;
               ppDestino++;
            }
            else
               break;
         }
                        
         if(!loc){               
            m4char_t c=(*a_ppCad)->LeeChar();
         
            if(c)
               *(p++)=c;         
            else
               break;
         }
      }
      
      if(bSus){
         // ha habido alguna sustitucion
         pCadDestino->EscribeCadena(buf, p-buf);
         pCadDestino->EscribeChar('\0');
         pCadDestino->Posiciona(0, INICIO);
         
         delete *a_ppCad;
         
         *a_ppCad=pCadDestino;
      }
    }
   } 
   
   return 0;
}

m4int16_t FMT_ClFormato::DesSustituye(void)
{                
   m4int16_t iError=0;
   
   return iError;
}


m4int16_t FMT_ClFormato::FormateaIdioma(ClCadena *a_pCad, m4int16_t a_tipo)
{             
   m4int16_t iError;
   
   // TRATAMIENTO DE ENTRADA --------------------------------------------------
   
   switch(a_tipo){
      
      // ALFA -----------------------------------------------------------------
   
      
      // NUMERO ---------------------------------------------------------------
      
      case TIPONUMERO:
      {                      
         s_pcIndices=s_pcIndicesCurrNume;
         
         if(D.Flags & FLAG_ICUR){
            // MONEDA :            
            iError=FormateaMascara(a_pCad, (m_poTaskEnv->ClM4Fmt_g_pDD->bNumNeg)? &sMaskCurNeg : &sMaskCurPos);
         }
         else{
            // NUMERO :            
            iError=(m_poTaskEnv->ClM4Fmt_g_pDD->bNumNeg)? FormateaMascara(a_pCad, &sMaskNeg) : EscribeNumero(a_pCad);               
         }         
      }
      break;
      
      
      // FECHA ----------------------------------------------------------------
      
      case TIPOFECHA:
      {           
         s_pcIndices=s_pcIndicesFechHora;            
         
         iError=FormateaMascara(a_pCad, &sMaskDate);
      }
      break;
      
      
      // HORA -----------------------------------------------------------------
      
      case TIPO_TIME:
      {                         
         iError=0;
         s_pcIndices=s_pcIndicesFechHora;            
         
         if(!sMaskTime12.EsNula()){
            m4char_t d=*(m_poTaskEnv->ClM4Fmt_g_pDD->sHora);
            m4char_t u=m_poTaskEnv->ClM4Fmt_g_pDD->sHora[1];
                        
            if( u && ( (d-'0')*10+u-'0' > 12 )){
               u-=2;
               if(u<'0'){
                  u+='9'-'0'+1;
                  d--;
               }
               d--;
                   
               if(d=='0'){
                  d=u;
                  u='\0';               
               }
                              
               *(m_poTaskEnv->ClM4Fmt_g_pDD->sHora)=d;
               m_poTaskEnv->ClM4Fmt_g_pDD->sHora[1]=u;
            }        
            else{                              
               iError=FormateaMascara(a_pCad, &sMaskTime12);             
               break;               
            }
         }
               
         if(!iError)      
            iError=FormateaMascara(a_pCad, &sMaskTime24);         
      }
      break;
            
      
      // TIMESTAMP ------------------------------------------------------------
      
      case M4ISOPOSDAY:
      {           
         s_pcIndices=s_pcIndicesFechHora;            
         
         ClCadena  *pmascara = &sMaskTimeStamp;
         
         if(D.Flags & FLAG_ICAPAR){
            //Capar si h=0, m=0, s=0
            
            if(EsCero(m_poTaskEnv->ClM4Fmt_g_pDD->sHora)){
               if(EsCero(m_poTaskEnv->ClM4Fmt_g_pDD->sMinuto)){
                  if(EsCero(m_poTaskEnv->ClM4Fmt_g_pDD->sSegundo)){
                     pmascara = &sMaskDate;
                  }
               }
            }

         }
         
         iError=FormateaMascara(a_pCad, pmascara);
      }
      break;
       
      
      // TIPO NO CONTEMPLADO -------------------------------------------------- 
       
      default:
      {           
         a_pCad->EscribeCadena(&(m_poTaskEnv->ClM4Fmt_g_pDD->sCadenaOriginal), m_poTaskEnv->ClM4Fmt_g_pDD->sCadenaOriginal.m_iLonBuffer);
         
         iError=0;
      }      
   }
   
   // CAPITALIZACION ----------------------------------------------------------
   
   if(D.iCase!=0){
      a_pCad->Mapeo((D.iCase==1)? s_TablaMinToMay : s_TablaMayToMin);
   }
      
   if(!iError && D.iCountry=='S'*0x100+'P'){
   
      // ACENTOS --------------------------------------------------------------
   
      if(DC.Flags & FLAG_ACENTLESS){               
         a_pCad->Mapeo(s_TablaQuitaAcentos);
      };   
   };

   return iError;
}


static m4int16_t (FMT_ClFormato::*TablaFuncionesEscritura[])(ClCadena  *) =
{                      
	NULL,
	&FMT_ClFormato::EscribeAnio,
	&FMT_ClFormato::EscribeMes,
	&FMT_ClFormato::EscribeDiaSemana,
	&FMT_ClFormato::EscribeDia,
	&FMT_ClFormato::EscribeHora,
	&FMT_ClFormato::EscribeMinuto,
	&FMT_ClFormato::EscribeSegundo,
	&FMT_ClFormato::EscribeMoneda,
	&FMT_ClFormato::EscribeNumero
};


m4int16_t FMT_ClFormato::FormateaMascara(ClCadena  *a_pCad, ClCadena  *a_pMask)
{
   m4int16_t iError;
   
   iError=0;
   
   if( (a_pMask==NULL)? M4_TRUE : a_pMask->EsNula() ){
      // mascara nula
      a_pCad->EscribeCadena(&(m_poTaskEnv->ClM4Fmt_g_pDD->sCadenaOriginal), m_poTaskEnv->ClM4Fmt_g_pDD->sCadenaOriginal.m_iLonBuffer);
   }
   else{
      a_pMask->Posiciona(0, INICIO);
      for(;;){
         m4char_t c;
            
         c=a_pMask->LeeChar();
         if(c!='\0'){
            if(c=='%'){
               c=a_pMask->LeeChar();
                              
               m4char_t indice;

               indice=(s_pcIndices[c]);
               if(indice!=' '){
                  iError=(this->*TablaFuncionesEscritura[indice])(a_pCad);
                  
                  if(iError)
                     break;
                  else   
                     continue;
               }               
                  
               if(c!='%')   
                  a_pCad->EscribeChar('%');
            }
            
            a_pCad->EscribeChar(c);
         }
         else
            break;
      }
   }  
   
   return iError;
}

m4int16_t FMT_ClFormato::EscribeAnio(ClCadena  *a_pCad)
{                  
	m4int16_t l;
	l=strlen(m_poTaskEnv->ClM4Fmt_g_pDD->sAnio);
   
	switch(D.iYear){
		case ANUMEROS_DOS:
		case ANUMEROS_CUATRO:
		{     
			m4char_t c[5];         
			if(l<4){
	            struct tm newtime;
				time_t aclock;
				time( &aclock );                 /* Get time in seconds */
                M4ClDate::LocalTime(aclock, newtime);

                m4char_t buffer[64];
                M4ClDate::AscTime(newtime, buffer);
				strcpy(c, buffer+20);            
				strcpy(c+4-l, m_poTaskEnv->ClM4Fmt_g_pDD->sAnio);   
				c[4]='\0';         
			} else {
				strncpy(c, m_poTaskEnv->ClM4Fmt_g_pDD->sAnio, 4);            
			};
			if(D.iYear==ANUMEROS_DOS){                 
				a_pCad->EscribeCadena(c+2, 2);
			} else { 
				a_pCad->EscribeCadena(c, 4);
			};
		}
		break;
      
		case ANUMEROS_ALETRAS:
		{                                                          
			// Pasar a letras :
			ClCadena temp;
			temp.Asigna(m_poTaskEnv->ClM4Fmt_g_pDD->sAnio, l);
			if(m_StPlugInInfo->m_StPlugInEnv){
				m_StPlugInInfo->m_StPlugInEnv->YearToText(a_pCad, &temp, (DC.Flags & FLAG_IKIND)? M4_TRUE: M4_FALSE);
			};
		}
		break;
	};
   
	return 0;
}


m4int16_t FMT_ClFormato::EscribeMes(ClCadena  *a_pCad)
{                              
   m4int16_t l;
   l=strlen(m_poTaskEnv->ClM4Fmt_g_pDD->sMes);
   
   switch(D.iMonth){
      case MNUMEROS_UNO:
      case MNUMEROS_DOS:
      {           
         if(l==1 && D.iMonth==MNUMEROS_DOS){            
            m_poTaskEnv->ClM4Fmt_g_pDD->sMes[1]=*m_poTaskEnv->ClM4Fmt_g_pDD->sMes;            
            *m_poTaskEnv->ClM4Fmt_g_pDD->sMes='0';
            l=2;
         }            
            
         a_pCad->EscribeCadena(m_poTaskEnv->ClM4Fmt_g_pDD->sMes, l);               
      }
      break;
      
      case MLETRAS_TRES:
      case MLETRAS_TODAS:
      {                            
         m4int16_t mes=atoi(m_poTaskEnv->ClM4Fmt_g_pDD->sMes);
         if(mes>0){
            if(mes<13){         
               m4int16_t k;
               m4char_t buf[32];                            
			   
			   if(m_StPlugInInfo->m_StPlugInEnv){
				  k=strlen(m_StPlugInInfo->m_StPlugInEnv->GetMonth(mes));
				  strncpy(buf,m_StPlugInInfo->m_StPlugInEnv->GetMonth(mes), k);
			      buf[k]='\0' ;
			   };
			   
               if(D.iMonth==MLETRAS_TRES)
                  k=3;
               a_pCad->EscribeCadena(buf, k);
            }            
         }         
      }
      break;
   }
   
   return 0;
}


m4int16_t FMT_ClFormato::EscribeDiaSemana(ClCadena  *a_pCad)
{                                
   switch(D.iWeekDay){
      case WNUMEROS_UNO:      
      {         
         a_pCad->EscribeChar(*m_poTaskEnv->ClM4Fmt_g_pDD->sDiaSemana);            
      }
      break;
      
      case WLETRAS_TRES:
      case WLETRAS_TODAS:
      {       
         m4int16_t diasemana=atoi(m_poTaskEnv->ClM4Fmt_g_pDD->sDiaSemana);
         
         if(diasemana>0){
            if(diasemana<8){         
               m4int16_t k;
               m4char_t buf[32];                 
      
			   if(m_StPlugInInfo->m_StPlugInEnv){
					k=strlen(m_StPlugInInfo->m_StPlugInEnv->GetDay(diasemana));
					strncpy(buf,m_StPlugInInfo->m_StPlugInEnv->GetDay(diasemana), k);
					buf[k]='\0' ;
			   };
			   
               if(D.iWeekDay==WLETRAS_TRES)
                  k=3;
            
               a_pCad->EscribeCadena(buf, k);
            }
         }
      }
      break;      
   }   
   
   return 0;
}


m4int16_t FMT_ClFormato::EscribeDia(ClCadena  *a_pCad)
{
   m4int16_t l;

   l=strlen(m_poTaskEnv->ClM4Fmt_g_pDD->sDia);

   switch(D.iDay){
      case DNUMEROS_UNO:
      case DNUMEROS_DOS:
      {
         if(l==1 && D.iDay==DNUMEROS_DOS){            
            m_poTaskEnv->ClM4Fmt_g_pDD->sDia[1]=*m_poTaskEnv->ClM4Fmt_g_pDD->sDia;            
            *m_poTaskEnv->ClM4Fmt_g_pDD->sDia='0';
            l=2;
         }            
            
         a_pCad->EscribeCadena(m_poTaskEnv->ClM4Fmt_g_pDD->sDia, l);               
      }
      break;
      
      case DNUMEROS_ALETRAS:
      {
         // Pasar a letras :
         ClCadena temp;
         temp.Asigna(m_poTaskEnv->ClM4Fmt_g_pDD->sDia, l);         
		 if(m_StPlugInInfo->m_StPlugInEnv){
			m_StPlugInInfo->m_StPlugInEnv->NumberToText(a_pCad, &temp, M4_TRUE, M4_TRUE);         
		 };
      }
      break;
   }
   
   return 0;
}


m4int16_t FMT_ClFormato::EscribeHora(ClCadena  *a_pCad)
{
   m4int16_t l;
   l=strlen(m_poTaskEnv->ClM4Fmt_g_pDD->sHora);

   switch(D.iHour){
      case HNUMEROS_UNO:
      case HNUMEROS_DOS:
      {
         if(l==1 && D.iHour==HNUMEROS_DOS){            
            m_poTaskEnv->ClM4Fmt_g_pDD->sHora[1]=*m_poTaskEnv->ClM4Fmt_g_pDD->sHora;            
            *m_poTaskEnv->ClM4Fmt_g_pDD->sHora='0';
            l=2;
         }            
            
         a_pCad->EscribeCadena(m_poTaskEnv->ClM4Fmt_g_pDD->sHora, l);               
      }      
      break;      
   }
   
   return 0;
}


m4int16_t FMT_ClFormato::EscribeMinuto(ClCadena  *a_pCad)
{
   m4int16_t l;
   l=strlen(m_poTaskEnv->ClM4Fmt_g_pDD->sMinuto);

   switch(D.iMinute){
      case NNUMEROS_UNO:
      case NNUMEROS_DOS:
      {
         if(l==1 && D.iMinute==NNUMEROS_DOS){            
            m_poTaskEnv->ClM4Fmt_g_pDD->sMinuto[1]=*m_poTaskEnv->ClM4Fmt_g_pDD->sMinuto;            
            *m_poTaskEnv->ClM4Fmt_g_pDD->sMinuto='0';
            l=2;
         }            
            
         a_pCad->EscribeCadena(m_poTaskEnv->ClM4Fmt_g_pDD->sMinuto, l);               
      }      
      break;      
   }
   
   return 0;
}


m4int16_t FMT_ClFormato::EscribeSegundo(ClCadena  *a_pCad)
{
   m4int16_t l;
   l=strlen(m_poTaskEnv->ClM4Fmt_g_pDD->sSegundo);

   switch(D.iSecond){
      case SNUMEROS_UNO:
      case SNUMEROS_DOS:
      {
         if(l==1 && D.iSecond==SNUMEROS_DOS){            
            m_poTaskEnv->ClM4Fmt_g_pDD->sSegundo[1]=*m_poTaskEnv->ClM4Fmt_g_pDD->sSegundo;            
            *m_poTaskEnv->ClM4Fmt_g_pDD->sSegundo='0';
            l=2;
         }            
            
         a_pCad->EscribeCadena(m_poTaskEnv->ClM4Fmt_g_pDD->sSegundo, l);               
      }      
      break;      
   }
   
   return 0;
}

m4int16_t FMT_ClFormato::EscribeMoneda(ClCadena  *a_pCad)
{
   a_pCad->EscribeCadena(sCurrency.m_pDirIni, sCurrency.m_iLonBuffer);
   
   return 0;
}

//----------------------------------------------------------------------------


m4int16_t FMT_ClFormato::EscribeNumero(ClCadena  *a_pCad)
{  
   m4int16_t iError;
   m4int16_t i;      
   m4char_t c, q;
   m4char_t*  buf = new char[256];   
   m4char_t*  dec = new char[256];
   m4char_t*  ent = new char[256];                                                              
     
   // initializes
   *buf = *dec = *ent = '\0';
     
   //Guardamos parte entera y decimal en buffers auxiliares
   m4uint32_t ld=m_poTaskEnv->ClM4Fmt_g_pDD->sParteDecimal.LonReal();
   m_poTaskEnv->ClM4Fmt_g_pDD->sParteDecimal.Posiciona(0, INICIO);                            
   m_poTaskEnv->ClM4Fmt_g_pDD->sParteDecimal.LeeCadena(dec, ld);  
   
   m4uint32_t le=m_poTaskEnv->ClM4Fmt_g_pDD->sParteEntera.LonReal();
   m_poTaskEnv->ClM4Fmt_g_pDD->sParteEntera.Posiciona(0, INICIO);                                                                                                  
   m_poTaskEnv->ClM4Fmt_g_pDD->sParteEntera.LeeCadena(ent, le);
     

   //Eliminamos los ceros no significativos si es necesario
   if(!(D.Flags & FLAG_ILEADZERO)){  //Si NO deben respetarse los ceros no significativos (a la derecha)
    	  // Quitar ceros
   	  ld=QuitaCerosFinal(dec, ld);
   }

     
   m4char_t*  p;
     
   if(D.iCompDec!=0xffff){ //Cuando hay un num. obligatorio de decimales
      // Ajuste del número de decimales
      if(ld>D.iCompDec){ //Cuando hay mas decimales de los obligatorios       
           // sobran decimales -> se recortan
         if(!(D.Flags & FLAG_ITRUNC)){ //Si se debe redondear los decimales
            // Redondeo:
            // Todo esto para el p... redondeo :              
            p=dec+D.iCompDec;
            c=*p;
            *(p--)='\0';
            if(c>'4'){ //Cuando es necesario el redondeo (si no basta con truncar)
               m4int16_t inc=1;
              
               m4bool_t bLlevadas=M4_TRUE;  //Podriamos llamarle tambien ACARREO de una suma
              
               for(i=D.iCompDec; i; i--){  //recorremos las cifras decimales (obligatorias) propagando el acarreo
                  q=*p;
                  q+=inc;  //Incrementamos cifra
   
                  if(q>'9' || q<'0')  //Si pasamos de 9, debemos seguir propagando el ACARREO
                     *(p--)='0';               
                  else{               //Si NO pasamos de 9, NO debemos seguir propagando el ACARREO
                     *p=q;
                     bLlevadas=M4_FALSE;
                     break;               
                  }               
               }               
              
               if(bLlevadas){  //Si hubo ACARREO desde la parte decimal ...
                  p=ent+le-1;               
                  for(i=le; i; i--){ //recorremos las cifras enteras propagando el acarreo
                     q=*p;
                     q+=inc;
                 
                     if(q>'9' || q<'0')
                        *(p--)='0';               
                     else{
                        *p=q;
                        bLlevadas=M4_FALSE;
                        break;               
                     }               
                  }
                 
                  if(bLlevadas){  //Si despues de la ultima cifra entera, siguio habiendo ACARREO
                     memcpy(buf, ent, le);               
                     memcpy(ent+1, buf, le);
                     *ent='1';   //Añadimos un '1' al final               
                     le++;       //... e incrementamos el num. de cifras enteras
                  }
               }                        
            }                 
         }  
         
         //Esto es lo que unicamente necesita al truncar (y tambien al redondear)
         ld = D.iCompDec; //Le hemos puesto el num. obligatorio de ceros
           
         if(!(D.Flags & FLAG_ILEADZERO)){ //Siempre es prioritario quitar ceros no significativos
            ld=QuitaCerosFinal(dec, ld);                           
         }
      }
      else{ //Cuando NO hay mas decimales de los necesarios (y hay un numero obligatorio de decimales)
         // Faltan decimales, se añaden
         if(D.Flags & FLAG_ILEADZERO){ //Cuando deben respetarse los ceros no significativos (a la derecha)
            p = dec + ld;
            for(i=(int)(D.iCompDec-ld); i; i--)
               *(p++) = '0';
                 
            ld = D.iCompDec;
         }           
      }
   }  //Termina la condicion de num. obligatorio de decimales
        

     
   if(D.Flags & FLAG_ISTYLE){  //Cuando debe pasarse las cifras a literal
	 ClCadena cadAux;
     // Escribe la parte entera :
   	 cadAux.Crea(ent, le);
	 if(m_StPlugInInfo->m_StPlugInEnv) {
		iError=m_StPlugInInfo->m_StPlugInEnv->NumberToText(a_pCad, &cadAux, (DC.Flags & FLAG_IKIND)? M4_TRUE: M4_FALSE, M4_FALSE);             
	 };
     if(!iError){                         
        if(ld){
		   // Escribe el separador decimal
           m4char_t espacio[]=" ";
		   if (sDecimal.m_iLonBuffer != 0) {
			   a_pCad->EscribeCadena(espacio, 1);
			   a_pCad->EscribeCadena(&sDecimal, sDecimal.m_iLonBuffer);
			   a_pCad->EscribeCadena(espacio, 1);
		   }
		   else {
			   // sólo se escribe un espacio	
			   a_pCad->EscribeCadena(espacio, 1);
		   }

		   // Escribe la parte decimal
		   cadAux.Crea(dec, ld);
		   if(m_StPlugInInfo->m_StPlugInEnv){
			   if(D.iDecGrup == 0xffff){  //No se agrupan cifras decimales
           		  iError=m_StPlugInInfo->m_StPlugInEnv->NumberToText(a_pCad, &cadAux, (DC.Flags & FLAG_IKIND)? M4_TRUE: M4_FALSE, M4_FALSE);   
			   } else {
           		  iError=m_StPlugInInfo->m_StPlugInEnv->DecimalToText(a_pCad, &cadAux, D.iDecGrup, (DC.Flags & FLAG_IKIND)? M4_TRUE: M4_FALSE);             
			   };
		   };
        }
     }
   }
   else{ //Cuando no debe pasarse las cifras a literal
     // Escribe la parte entera :
                 
     *buf='\0';
          
     
     if(D.iCompInt!=0xffff){  //Cuando hay un num. obligado de cifras enteras
        if(le<D.iCompInt){
           m4char_t*  p=buf;      
           
           for(i=(int)(D.iCompInt-le); i; i--)
              *(p++)='0';                   
              
           memcpy(p, ent, le);                                
	       le=D.iCompInt;             
        }
        else{ //No truncamos parte entera, que está muy feo                              
           memcpy(buf, ent, le);
        }  
     }
     else{  //Cuando NO hay obligacion en cuanto al num. de cifras enteras
        memcpy(buf, ent, le);
        
        if(!(D.Flags & FLAG_ILEFTZERO)){ //Cuando NO debe escribirse la parte entera cuando sea 0
           m4bool_t cero=M4_TRUE;
           m4char_t*  q=ent;
           for(m4int16_t i=le; i; i--){
              if(*(q++)!='0'){
                 cero=M4_FALSE;
                 break;
              }
           }
           
           if(ld &&  cero){
              // si hay decimales y la parte entera es '0', no se escribe la parte entera
              le=0;
           }
        }
     }   
                                  
     
        
     p=buf;
     if(sThousand.EsNula()){      
        a_pCad->EscribeCadena(p, le);      
     }
     else{
        m4int16_t cuenta=le/3;   
        m4int16_t resto=le-cuenta*3;
        
        p=buf;
        if(resto){
           a_pCad->EscribeCadena(p, resto);
           p+=resto;
        }
        
        for(i=cuenta; i; i--){
           if(resto || i<cuenta)
              a_pCad->EscribeCadena(sThousand.m_pDirIni, sThousand.m_iLonBuffer);
              
           a_pCad->EscribeCadena(p, 3);
           p+=3;
        }      
     }
     
     if((!sDecimal.EsNula()) && ld){        
        // Escribe coma :      
        a_pCad->EscribeCadena(sDecimal.m_pDirIni, sDecimal.m_iLonBuffer);

        // Escribe la parte decimal :      
        a_pCad->EscribeCadena(dec, ld);      
     }
     
     iError=0;
   }
   
   delete ent;
   delete dec;   
   delete buf;        
     
   return iError;
}
                      

///////////////////////////////////////////////////////////////////////////////    

static const StComando s_pTablaComandosGeneral[]=
{     
// Por orden alfabetico
   { "iFAlig",					&FMT_ClFormato::co_iFAlig },
   { "iFBeginstr",				&FMT_ClFormato::co_iFBeginstr },
   { "iFBlank",					&FMT_ClFormato::co_iFBlank },
   { "iFCase",					&FMT_ClFormato::co_iFCase },   
   { "iFCompDec",				&FMT_ClFormato::co_iFCompDec },
   { "iFCompInt",				&FMT_ClFormato::co_iFCompInt },
   { "iFCur",					&FMT_ClFormato::co_iFCur },   
   { "iFDecGrup",				&FMT_ClFormato::co_iFDecGrup },
   { "iFLeadZero",				&FMT_ClFormato::co_iFLeadZero },
   { "iFLeftZero",				&FMT_ClFormato::co_iFLeftZero },
   { "iFLength",				&FMT_ClFormato::co_iFLength },
   { "iFNoffsetstr",			&FMT_ClFormato::co_iFNoffsetstr },
   { "iFPrefixeMode",			&FMT_ClFormato::co_iFPrefixeMode },
   { "iFRemoveNullTime",		&FMT_ClFormato::co_iFCapar },
   { "iFSeparatorMode",			&FMT_ClFormato::co_iFSeparatorMode },
   { "iFStyle",					&FMT_ClFormato::co_iFStyle },   
   { "iFSubst",					&FMT_ClFormato::co_iFSubst },
   { "iFTrunc",					&FMT_ClFormato::co_iFTrunc },
   { "iFWordToKeepSubstitute",	&FMT_ClFormato::co_iFWordToKeepSubtitute },

   { "sFCharsFill",			&FMT_ClFormato::co_sFCharsFill },   
   { "sFCurrency",			&FMT_ClFormato::co_sFCurrency },   
   { "sFDay",				&FMT_ClFormato::co_sFDay },
   { "sFDecimal",			&FMT_ClFormato::co_sFDecimal },
   { "sFHour",				&FMT_ClFormato::co_sFHour },   
   { "sFMaskCurNeg",		&FMT_ClFormato::co_sFMaskCurNeg },
   { "sFMaskCurPos",		&FMT_ClFormato::co_sFMaskCurPos },
   { "sFMaskDate",			&FMT_ClFormato::co_sFMaskDate },   
   { "sFMaskNeg",			&FMT_ClFormato::co_sFMaskNeg },
   { "sFMaskTime12",		&FMT_ClFormato::co_sFMaskTime12 },
   { "sFMaskTime24",		&FMT_ClFormato::co_sFMaskTime24 },
   { "sFMaskTimeStamp",		&FMT_ClFormato::co_sFMaskTimeStamp },
   { "sFMinusInfDate",		&FMT_ClFormato::co_sFMinusInfDate },
   { "sFMinute",			&FMT_ClFormato::co_sFMinute },
   { "sFMonth",				&FMT_ClFormato::co_sFMonth },
   { "sFPlusInfDate",		&FMT_ClFormato::co_sFPlusInfDate },
   { "sFPrefixes",			&FMT_ClFormato::co_sFPrefixes },
   { "sFSecond",			&FMT_ClFormato::co_sFSecond },
   { "sFSeparators",		&FMT_ClFormato::co_sFSeparators },
   { "sFSubst",				&FMT_ClFormato::co_sFSubst },      
   { "sFThousand",			&FMT_ClFormato::co_sFThousand },
   { "sFWeekDay",			&FMT_ClFormato::co_sFWeekDay },   
   { "sFWordsToSubstitute",	&FMT_ClFormato::co_sFWordsToSubtitute },   
   { "sFYear",				&FMT_ClFormato::co_sFYear },      
 
   //{ "sFCountry",     &FMT_ClFormato::co_sFCountry },
   //{ "",  &FMT_ClFormato::co_ },
   { "\xFF",   0 }
};

//////////////////// PROPIO DEL CASTELLANO /////////////////////////////////////////////

static const StComando s_pTablaComandosCastellano[]=
{  
// Por orden alfabetico
	{ "iFAcentLess",   &FMT_ClFormato::co_iFAcentLess },         
    { "iFDecKind",     &FMT_ClFormato::co_iFDecKind },
    { "iFIntKind",     &FMT_ClFormato::co_iFIntKind },   
    { "\xFF",   0 }
};

m4int16_t FMT_ClFormato::co_iFAcentLess(m4char_t* a_pcValor)
{        
   return CambiaFlag(a_pcValor, &(DC.Flags), FLAG_ACENTLESS);
}
      
m4int16_t FMT_ClFormato::co_iFIntKind(m4char_t* a_pcValor)
{
   return CambiaFlag(a_pcValor, &(DC.Flags), FLAG_IKIND);
}

m4int16_t FMT_ClFormato::co_iFDecKind(m4char_t* a_pcValor)
{  
   return 0;
}

m4int16_t FMT_ClFormato::co_sFValueInt(m4char_t* a_pcValor)
{           
   return 0;
}

m4int16_t FMT_ClFormato::co_sFValueDec(m4char_t* a_pcValor)
{           
   return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////

m4int16_t FMT_ClFormato::CambiaPropiedad(m4char_t* a_pcNombre, m4char_t* a_pcValor)
{                   
    m4int16_t iError;

	iError=BuscaPropiedad(a_pcNombre, a_pcValor, s_pTablaComandosCastellano);
	
	if(iError!=ERROR_PROPIEDADNOE){
		return iError;
	} else {
		return BuscaPropiedad(a_pcNombre, a_pcValor, s_pTablaComandosGeneral);
	};
}

m4int16_t FMT_ClFormato::BuscaPropiedad(m4char_t*  a_pcNombre, m4char_t*  a_pcValor,const StComando  *a_pT)
{
   m4int16_t iError;         
              
   iError=ERROR_PROPIEDADNOE;     
      
   for(;a_pT->Comando;){  
      #ifdef _WINDOWS
		m4int16_t val=_stricmp(a_pcNombre, a_pT->Nombre);
      #else 
		m4int16_t val=stricmp(a_pcNombre, a_pT->Nombre);
	  #endif
      if(val>0){
         a_pT++;
      }
      else{   
         if(!val){
            iError = (this->*(a_pT->Comando))(a_pcValor);  
         }
         
         break;
      }
   }
   
   return iError;
}


///////////////////////////////////////////////////////////////////////////////    

m4int16_t FMT_ClFormato::co_iFAlig(m4char_t*  a_pcValor)
{                
   m4int16_t iError;
   
   iError=0;
   
   if(a_pcValor){
      switch(*a_pcValor){
         case '0':
            D.iAlig=0;            
            break;
            
         case '1':
            D.iAlig=1;
            break;

         case '2':
            D.iAlig=2;
            break;            
            
         default:
            iError=ERROR_VALORNOCORRECTO;            
      }
   }   
   else
      iError=ERROR_VALORNOCORRECTO;
   
   return 0;
}

m4int16_t FMT_ClFormato::co_iFLength(m4char_t*  a_pcValor)
{          
   return CambiaEntero(a_pcValor, &(D.iLength));
}

m4int16_t FMT_ClFormato::co_iFBeginstr(m4char_t*  a_pcValor)
{                               
   return CambiaFlag(a_pcValor, &(D.Flags), FLAG_IBEGINSTR);
}

m4int16_t FMT_ClFormato::co_iFNoffsetstr(m4char_t*  a_pcValor)
{
   return CambiaEntero(a_pcValor, &(D.iNoffsetstr));
}

m4int16_t FMT_ClFormato::co_sFCharsFill(m4char_t*  a_pcValor)
{   
   return sCharsFill.Crea(a_pcValor);
}

m4int16_t FMT_ClFormato::co_sFMaskCurPos(m4char_t*  a_pcValor)
{   
   return sMaskCurPos.Crea(a_pcValor);
}

m4int16_t FMT_ClFormato::co_sFMaskCurNeg(m4char_t*  a_pcValor)
{   
   return sMaskCurNeg.Crea(a_pcValor);
}

m4int16_t FMT_ClFormato::co_sFCurrency(m4char_t*  a_pcValor)
{
   return sCurrency.Crea(a_pcValor);
}

m4int16_t FMT_ClFormato::co_iFCase(m4char_t*  a_pcValor)
{     
   m4int16_t iError;                  
   
   iError=0;

   if(a_pcValor){   
      switch(*a_pcValor){
         case '0':
            D.iCase = 0;
            break;
            
         case '1':
            D.iCase = 1;
            break;
            
         case '2':
            D.iCase = 2;
            break;   

		 case '3':
            D.iCase = 3;
            break; 

		 case '4':
            D.iCase = 4;
            break; 
            
         default:
            iError=ERROR_VALORNOCORRECTO;               
      }
   }
   else
      iError=ERROR_VALORNOCORRECTO;               
      
   return 0;   
}

m4int16_t FMT_ClFormato::co_iFWordToKeepSubtitute(m4char_t*  a_pcValor)
{     
   m4int16_t iError;                  
   
   iError=0;

   if(a_pcValor){   
      switch(*a_pcValor){
         case '0':
            D.iWordMode = 0;
            break;
            
         case '1':
            D.iWordMode = 1;
            break;
            
         case '2':
            D.iWordMode = 2;
            break;  
			
		case '3':
            D.iWordMode = 3;
            break;
            
         default:
            iError=ERROR_VALORNOCORRECTO;               
      }
   }
   else
      iError=ERROR_VALORNOCORRECTO;               
      
   return 0;   
}

m4int16_t FMT_ClFormato::co_iFSeparatorMode(m4char_t*  a_pcValor)
{     
   m4int16_t iError;                  
   
   iError=0;

   if(a_pcValor){   
      switch(*a_pcValor){
         case '0':
            D.iSeparatorMode = 0;
            break;
            
         case '1':
            D.iSeparatorMode = 1;
            break;
            
         case '2':
            D.iSeparatorMode = 2;
            break;  
			
		case '3':
            D.iSeparatorMode = 3;
            break;
            
         default:
            iError=ERROR_VALORNOCORRECTO;               
      }
   }
   else
      iError=ERROR_VALORNOCORRECTO;               
      
   return 0;   
}

m4int16_t FMT_ClFormato::co_sFWordsToSubtitute(m4char_t* a_pcValor)
{
	ClCadena sExternalWords;
	m4uint16_t iCounter;
	ClMIT_Str oStrAux;
	ClMIT_Args oArgs;

	sExternalWords.Crea(a_pcValor,strlen(a_pcValor));

	if(!sExternalWords.DirIni()) goto exit;

	oArgs.Init(sExternalWords.DirIni(),ClMIT_Args::NO_SUBARG, ", ");

	for ( iCounter=0; iCounter<oArgs.GetArgCount(); ++iCounter ) {
		oStrAux.ToASCIZ(oArgs.GetArg(iCounter),strlen(oArgs.GetArg(iCounter)));
		ExtWordList_t.insert(ExtList_t::value_type(oStrAux,1));
	};

exit:
	
	return 0;
}

m4int16_t FMT_ClFormato::co_sFPrefixes(m4char_t* a_pcValor)
{
	ClCadena sExternalPrefixes;
	m4uint16_t iCounter;
	ClMIT_Str oStrAux;
	ClMIT_Args oArgs;

	sExternalPrefixes.Crea(a_pcValor,strlen(a_pcValor));

	if(!sExternalPrefixes.DirIni()) goto exit;

	oArgs.Init(sExternalPrefixes.DirIni(),ClMIT_Args::NO_SUBARG, ", ");

	for ( iCounter=0; iCounter<oArgs.GetArgCount(); ++iCounter ) {
		oStrAux.ToASCIZ(oArgs.GetArg(iCounter),strlen(oArgs.GetArg(iCounter)));
		ExtPrefList_t.insert(ExtList_t::value_type(oStrAux,1));
	};

exit:

	return 0;
}

m4int16_t FMT_ClFormato::co_sFSeparators(m4char_t* a_pcValor)
{
	ClCadena sExternalSeparators;
	m4uint16_t iCounter;
	ClMIT_Str oStrAux;
	ClMIT_Args oArgs;
	m4char_t AuxBuff[2];

	sExternalSeparators.Crea(a_pcValor,strlen(a_pcValor));
	
	if(!sExternalSeparators.DirIni()) goto exit;

	oArgs.Init(sExternalSeparators.DirIni(),ClMIT_Args::NO_SUBARG, ", ");

	for ( iCounter=0; iCounter<oArgs.GetArgCount(); ++iCounter ) {
		sprintf(AuxBuff,"%c",atol(oArgs.GetArg(iCounter)));
		oStrAux.ToASCIZ(AuxBuff,strlen(AuxBuff));
		ExtSepList_t.insert(ExtList_t::value_type(oStrAux,1));
	};

exit:
	
	return 0;
}

m4int16_t FMT_ClFormato::co_iFPrefixeMode(m4char_t*  a_pcValor)
{     
   m4int16_t iError;                  
   
   iError=0;

   if(a_pcValor){   
      switch(*a_pcValor){
         case '0':
            D.iPrefixeMode = 0;
            break;
            
         case '1':
            D.iPrefixeMode = 1;
            break;
            
         case '2':
            D.iPrefixeMode = 2;
            break;  
			
		case '3':
            D.iPrefixeMode = 3;
            break;
            
         default:
            iError=ERROR_VALORNOCORRECTO;               
      }
   }
   else
      iError=ERROR_VALORNOCORRECTO;               
      
   return 0;   
}


m4int16_t FMT_ClFormato::co_iFSubst(m4char_t*  a_pcValor)
{                                     
   return CambiaFlag(a_pcValor, &(D.Flags), FLAG_ISUBST);   
}

m4int16_t FMT_ClFormato::co_sFSubst(m4char_t*  a_pcValor)
{     
   m4int16_t iError;  
   
   iError=0;
   
   // primera pasada: se deternima el numero de sustituciones:   
   m4char_t*  p=a_pcValor;
      
   union tagCuenta{
      m4uint32_t B[2];
      struct tD{         
         m4uint32_t Destino;                  
         m4uint32_t Origen;                            
      }C;
   }Cuenta;
         
   Cuenta.C.Origen=0;
   Cuenta.C.Destino=0;
   
   m4char_t seguidos=1;
   m4char_t seguidosAnt=1;
   m4bool_t alt=M4_FALSE;
   
   m4char_t c;
   
   do{                       
      for(;;){
         c=*(p++);   
         
         if(c){
            if(c=='~')
               break;              
         }
         else
            break;
      }
      
      if(seguidos<2){
         Cuenta.B[seguidos]++;      
         
         if(c){
            seguidos=0;
      
            for(;;){  
               c=*(p++);   
               
               if(c){
                  if(c=='~')
                     seguidos++;
                  else
                     break;
               }      
               else
                  break;
            }
         }
      }      
   }while(c);
      
   if(Cuenta.C.Destino>Cuenta.C.Origen)
      iError=ERROR_VALORNOCORRECTO;
   else{
      if(m_spSubst!=NULL)
         delete []m_spSubst;
         
      m_spSubst=NULL;
      
      m4int16_t total=(Cuenta.C.Origen<<1);  
      D.nCadenasSubst=Cuenta.C.Origen;
      m_spSubst=new ClCadena[total];
      
      ClCadena  *ppOrigen=m_spSubst;
      ClCadena  *ppDestino=m_spSubst+D.nCadenasSubst;
      
      // Segunda pasada: se crean:
      m4bool_t yaori=M4_FALSE;
      p=a_pcValor;
      seguidos=1;
      m4char_t*  meca=a_pcValor;
      m4int16_t lon=0;
      
      do{                       
         for(;;){
            c=*(p++);   
         
            if(c){
               if(c=='~')
                  break;              
            }
            else
               break;
               
            lon++;   
         }
      
         if(seguidos<2){
            if(seguidos==1){
               // origen 
               (ppOrigen++)->Crea(meca, lon);
               if(yaori)
                  (ppDestino++)->Crea((m4char_t* )NULL);
               else   
                  yaori=M4_TRUE;                     
            }
            else{
               // destino
               (ppDestino++)->Crea(meca, lon);
               yaori=M4_FALSE;
            }                  
            
                     
            if(c){
               lon=0;
               seguidos=0;
      
               for(;;){  
                  c=*(p);   
               
                  if(c){
                     if(c=='~')
                        seguidos++;
                     else{
                        meca=p;
                        break;
                     }   
                  }      
                  else
                     break;
                     
                  p++;   
               }
            }
         }               
      }while(c);
      
      if(yaori)
         (ppDestino++)->Crea((m4char_t* )NULL);

   }
   
   return iError;
}

m4int16_t FMT_ClFormato::co_sFDay(m4char_t*  a_pcValor)
{        
   m4int16_t iError;   
   m4int16_t l;     
      
   l=CuentaLetras(a_pcValor, 'd');
   
   if(l>0 && l<4){
      m4char_t a[]={0, DNUMEROS_UNO, DNUMEROS_DOS, DNUMEROS_ALETRAS};
      D.iDay=a[l];
      iError=0;
   }               
   else
      iError=ERROR_VALORNOCORRECTO;               
   
   return iError;
}

m4int16_t FMT_ClFormato::co_sFMonth(m4char_t*  a_pcValor)
{
   m4int16_t iError;   
   m4int16_t l;     
      
   l=CuentaLetras(a_pcValor, 'm');
   
   if(l>0 && l<5){
      m4char_t a[]={0, MNUMEROS_UNO, MNUMEROS_DOS, MLETRAS_TRES, MLETRAS_TODAS};
      D.iMonth=a[l];
      iError=0;
   }               
   else
      iError=ERROR_VALORNOCORRECTO;               
      
   return iError;   
}

m4int16_t FMT_ClFormato::co_sFYear(m4char_t*  a_pcValor)
{                
   m4int16_t iError;   
   m4int16_t l;
   
   l=CuentaLetras(a_pcValor, 'y');
   
   if(l==2 || l==4 || l==5){
      m4char_t a[]={0, 0, ANUMEROS_DOS, 0, ANUMEROS_CUATRO, ANUMEROS_ALETRAS};
      D.iYear=a[l];
      iError=0;
   }               
   else
      iError=ERROR_VALORNOCORRECTO;               
   
   return iError;
}

m4int16_t FMT_ClFormato::co_sFWeekDay(m4char_t*  a_pcValor)
{                
   m4int16_t iError;   
   m4int16_t l;
   
   l=CuentaLetras(a_pcValor, 'w');
   
   if(l>0 && l<4){
      m4char_t a[]={0, WNUMEROS_UNO, WLETRAS_TRES, WLETRAS_TODAS};
      D.iWeekDay=a[l];
      iError=0;
   }               
   else
      iError=ERROR_VALORNOCORRECTO;               
   
   return iError;
}

m4int16_t FMT_ClFormato::co_sFHour(m4char_t*  a_pcValor)
{
   m4int16_t iError;   
   m4int16_t l;
   
   l=CuentaLetras(a_pcValor, 'h');
   
   if(l>0 && l<3){ 
      m4char_t a[]={0, HNUMEROS_UNO, HNUMEROS_DOS};
      D.iHour=a[l];
      iError=0;
   }
   else
      iError=ERROR_VALORNOCORRECTO;               
   
   return iError;
}

m4int16_t FMT_ClFormato::co_sFMinute(m4char_t*  a_pcValor)
{
   m4int16_t iError;   
   m4int16_t l;
   
   l=CuentaLetras(a_pcValor, 'n');
   
   if(l>0 && l<3){ 
      m4char_t a[]={0, NNUMEROS_UNO, NNUMEROS_DOS};
      D.iMinute=a[l];
      iError=0;
   }
   else
      iError=ERROR_VALORNOCORRECTO;               
   
   return iError;
}

m4int16_t FMT_ClFormato::co_sFSecond(m4char_t*  a_pcValor)
{                
   m4int16_t iError;   
   m4int16_t l;
   
   l=CuentaLetras(a_pcValor, 's');
   
   if(l>0 && l<3){ 
      m4char_t a[]={0, SNUMEROS_UNO, SNUMEROS_DOS};
      D.iSecond=a[l];
      iError=0;
   }
   else
      iError=ERROR_VALORNOCORRECTO;               
   
   return iError;   
}

m4int16_t FMT_ClFormato::co_sFMaskDate(m4char_t*  a_pcValor)
{  
   return sMaskDate.Crea(a_pcValor);
}


m4int16_t FMT_ClFormato::co_sFPlusInfDate(m4char_t*  a_pcValor)
{  
	return sPlusInfDate.Crea(a_pcValor);
}


m4int16_t FMT_ClFormato::co_sFMinusInfDate(m4char_t*  a_pcValor)
{  
   return sMinusInfDate.Crea(a_pcValor,strlen(a_pcValor));
}

 
m4int16_t FMT_ClFormato::co_sFMaskTime12(m4char_t*  a_pcValor)
{  
   return sMaskTime12.Crea(a_pcValor);
}


m4int16_t FMT_ClFormato::co_sFMaskTime24(m4char_t*  a_pcValor)
{  
   return sMaskTime24.Crea(a_pcValor);
}


m4int16_t FMT_ClFormato::co_sFMaskTimeStamp(m4char_t*  a_pcValor)
{  
   return sMaskTimeStamp.Crea(a_pcValor);
}

 
m4int16_t FMT_ClFormato::co_sFThousand(m4char_t*  a_pcValor)
{
   return sThousand.Crea(a_pcValor);
}


m4int16_t FMT_ClFormato::co_sFDecimal(m4char_t*  a_pcValor)
{
   return sDecimal.Crea(a_pcValor);
}


m4int16_t FMT_ClFormato::co_iFCompDec(m4char_t*  a_pcValor)
{  
   m4int16_t iError;
   if(*a_pcValor){      
      iError=CambiaEntero(a_pcValor, &(D.iCompDec));   
      
      if(D.iCompDec>255)
         iError=ERROR_VALORNOCORRECTO;      
   }
   else{  
      D.iCompDec=0xffff;
      iError=0;      
   }
   
   return iError;
}


m4int16_t FMT_ClFormato::co_iFCompInt(m4char_t*  a_pcValor)
{      
   m4int16_t iError;
   if(*a_pcValor){      
      iError=CambiaEntero(a_pcValor, &(D.iCompInt));   
      
      if(D.iCompInt>255)
         iError=ERROR_VALORNOCORRECTO;      
   }
   else{  
      D.iCompInt=0xffff;
      iError=0;      
   }
   
   return iError;
}


m4int16_t FMT_ClFormato::co_iFTrunc(m4char_t*  a_pcValor)
{
   return CambiaFlag(a_pcValor, &(D.Flags), FLAG_ITRUNC);
}


m4int16_t FMT_ClFormato::co_iFLeadZero(m4char_t*  a_pcValor)
{                                 
   return CambiaFlag(a_pcValor, &(D.Flags), FLAG_ILEADZERO);
}


m4int16_t FMT_ClFormato::co_sFMaskNeg(m4char_t*  a_pcValor)
{
   return sMaskNeg.Crea(a_pcValor);
}


m4int16_t FMT_ClFormato::co_iFStyle(m4char_t*  a_pcValor)
{
   return CambiaFlag(a_pcValor, &(D.Flags), FLAG_ISTYLE);
}


m4int16_t FMT_ClFormato::co_iFCur(m4char_t*  a_pcValor)
{
   return CambiaFlag(a_pcValor, &(D.Flags), FLAG_ICUR);
}


m4int16_t FMT_ClFormato::co_iFLeftZero(m4char_t*  a_pcValor)
{                                                      
   return CambiaFlag(a_pcValor, &(D.Flags), FLAG_ILEFTZERO);
}


m4int16_t FMT_ClFormato::co_iFCapar(m4char_t*  a_pcValor)
{
   return CambiaFlag(a_pcValor, &(D.Flags), FLAG_ICAPAR);
}

///////////////////////////////////////////////////////////////////////////////    

m4int16_t FMT_ClFormato::QuitaCerosFinal(m4char_t*  dir, m4int16_t l)
{     

   if (l < 1) {
	return 0;
   }

   m4char_t*  q=dir+l-1;
   while(l){      
      if(*q=='0'){
         *(q--)='\0';               
         l--;
      }
      else
         break;             
   }
   
   return l;
}


m4char_t FMT_ClFormato::BuscaCadenaMes(m4char_t*  a_pcCadena, m4int16_t a_iCadena, m4int16_t a_iN)
{                                                               
   m4char_t bEncontrado=0;
   
   m4int16_t id=0; 
         
   m4int16_t i, k;
   m4char_t buf[64];
   
   if(m_StPlugInInfo->m_StPlugInEnv){
	for(i=a_iN; i; i--){
	  k=strlen(m_StPlugInInfo->m_StPlugInEnv->GetMonth(i));
	  strncpy(buf,m_StPlugInInfo->m_StPlugInEnv->GetMonth(i), k);
	  buf[k]='\0' ;
      if(!strncmp(buf, a_pcCadena, a_iCadena? a_iCadena : strlen(buf))){
         bEncontrado=i;
         break;
      }; 
    };
   };
   
   return bEncontrado;
}

m4char_t FMT_ClFormato::BuscaCadenaSemana(m4char_t*  a_pcCadena, m4int16_t a_iCadena, m4int16_t a_iN)
{                                                               
   m4char_t bEncontrado=0;
   
   m4int16_t id=0; 
         
   m4int16_t i, k;
   m4char_t buf[64];
   
   if(m_StPlugInInfo->m_StPlugInEnv){
	for(i=a_iN; i; i--){
	  k=strlen(m_StPlugInInfo->m_StPlugInEnv->GetDay(i));
	  strncpy(buf,m_StPlugInInfo->m_StPlugInEnv->GetDay(i), k);
	  buf[k]='\0' ;
      if(!strncmp(buf, a_pcCadena, a_iCadena? a_iCadena : strlen(buf))){
         bEncontrado=id+1;
         break;
      }; 
	};                                                                            
   };

   return bEncontrado;
}


m4int16_t FMT_ClFormato::CambiaFlag(m4char_t*  a_pcValor, DWORD  *a_plF, DWORD a_flag)
{   
   m4int16_t iError;                  
   
   iError=0;
   
   if(a_pcValor){
      switch(*a_pcValor){
         case '0':
            *a_plF &= ~a_flag;
            break;
            
         case '1':      
            *a_plF |= a_flag; 
            break;         
            
         default:
            iError=ERROR_VALORNOCORRECTO;            
      }
   }   
   else
      iError=ERROR_VALORNOCORRECTO;      
   
   return iError;   
}


m4int16_t FMT_ClFormato::CambiaEntero(m4char_t*  a_pcValor, m4uint32_t  *a_pi)
{
   m4int16_t iError;                  
   
   if(a_pcValor){
      *a_pi=atoi(a_pcValor);
      iError=0;
   }   
   else
      iError=ERROR_VALORNOCORRECTO;
   
   return iError;
}


m4int16_t FMT_ClFormato::CuentaLetras(m4char_t*  dir, m4char_t c)
{                      
   m4int16_t i;
   
   i=0;
   if(dir!=NULL){
      m4char_t k;
      do{
         k=*(dir++);
         if(k==c)
            i++;
      }while(k);
   }
   
   return i;
}
      

m4int16_t FMT_ClFormato::DameNombre(m4char_t*  a_pcBuffer, m4int16_t a_iLonMax)
{               
   m4int16_t iResul=0;
   
   if(m_pcNombre){
      strncpy(a_pcBuffer, m_pcNombre, a_iLonMax);
      iResul=strlen(a_pcBuffer);
   }
   
   return iResul;
}
  

///////////////////////////////////////////////////////////////////////////////

m4int16_t FMT_ClFormato::DesformateaLight(ClCadena  *a_pCad, m4int16_t a_tipo)
{     
   m4int16_t iError;
      
   // DES-ALINEADOS VARIOS ----------------------------------------------------
        
   m4uint32_t l=(UINT)a_pCad->LonReal();   
   m4uint32_t ini;
   m4uint32_t lon;     
   
   m_poTaskEnv->ClM4Fmt_g_pDD->sCharsFill.CopiaDe(&sCharsFill);
   
   m4int16_t il=m_poTaskEnv->ClM4Fmt_g_pDD->sCharsFill.LonReal();
   if(il){
      if(D.iCase)      
         m_poTaskEnv->ClM4Fmt_g_pDD->sCharsFill.Mapeo((D.iCase==1)? s_TablaMinToMay : s_TablaMayToMin);
      
      m_poTaskEnv->ClM4Fmt_g_pDD->sCharsFill.Posiciona(0, INICIO);   
      // il no es cero   
      m4char_t*  fill=new char[il];               
      m_poTaskEnv->ClM4Fmt_g_pDD->sCharsFill.LeeCadena(fill, il);    
         
      switch(D.iAlig){         
         case 1:       
         {
            // Centrado     
            ini=DesrellenaIzq(a_pCad, fill, il);
            lon=l-ini-DesrellenaDer(a_pCad, fill, il);                  
         }   
         break;
            
         case 2:      
         {
            // Izquierda         
            ini=0;
            lon=l-DesrellenaDer(a_pCad, fill, il);         
         }   
         break;   
         
         default:   
            // Derecha         
            ini=DesrellenaIzq(a_pCad, fill, il);
            lon=l-ini;         
      }   
      
      delete fill;
   }
   else{            
      ini=0;
      lon=l;
   }
   
   ClCadena cadAux;   
   cadAux.Asigna(a_pCad->DirIni()+ini, lon);         
   
   // Llamada a la función virtual --------------------------------------------
      
   m_poTaskEnv->ClM4Fmt_g_pDD->sCadenaOriginal.Asigna(a_pCad->DirIni()+ini, lon);            
            
   iError=DesformateaIdiomaLight(&cadAux, a_tipo);
   
   
   // DES-SUSTITUCION::ANTES --------------------------------------------------
      
   // -------------------------------------------------------------------------
   
   return iError;
}


m4int16_t FMT_ClFormato::DesformateaIdiomaLight(ClCadena  *a_pCad, m4int16_t a_tipo)
{
   m4int16_t iError;
                             
   iError=0;            
      
   switch(a_tipo){
      
      // ALFA -----------------------------------------------------------------
            
      // NUMERO ---------------------------------------------------------------
      
      case TIPONUMERO:
      {        
         m_poTaskEnv->ClM4Fmt_g_pDD->sThousand.CopiaDe(&sThousand);
         m_poTaskEnv->ClM4Fmt_g_pDD->sDecimal.CopiaDe(&sDecimal);   
            
         const m4char_t*  pcTabla;         
         if(D.iCase){
            pcTabla=(D.iCase==1)? s_TablaMinToMay : s_TablaMayToMin;      
            
            m_poTaskEnv->ClM4Fmt_g_pDD->sThousand.Mapeo(pcTabla);
            m_poTaskEnv->ClM4Fmt_g_pDD->sDecimal.Mapeo(pcTabla);                  
         }
         
         ClCadena psNum;
                                                       
         if(D.Flags & FLAG_ICUR){         
            // MONEDA :          
            m_poTaskEnv->ClM4Fmt_g_pDD->sCurrency.CopiaDe(&sCurrency);
            if(D.iCase)
               m_poTaskEnv->ClM4Fmt_g_pDD->sCurrency.Mapeo(pcTabla);  
            
            iError=DesformateaNumero1(&psNum, a_pCad, &sMaskCurPos);
            
            if(!iError)
               m_poTaskEnv->ClM4Fmt_g_pDD->bNumNeg=M4_FALSE;
            else{
               iError=DesformateaNumero1(&psNum, a_pCad, &sMaskCurNeg);
               if(!iError)
                  m_poTaskEnv->ClM4Fmt_g_pDD->bNumNeg=M4_TRUE;
               else   
                  break;
            }            
         }
         else{
            // NUMERO :
            m_poTaskEnv->ClM4Fmt_g_pDD->sCurrency.Asigna((m4char_t* )NULL);
            
            m4bool_t negativo;

            if(sMaskNeg.EsNula()){
               negativo=M4_FALSE;
            }
            else{
               iError=DesformateaNumero1(&psNum, a_pCad, &sMaskNeg);
               negativo= (!iError)? M4_TRUE : M4_FALSE;
            }

            if(negativo){
               m_poTaskEnv->ClM4Fmt_g_pDD->bNumNeg=M4_TRUE;
            }
            else{
               m_poTaskEnv->ClM4Fmt_g_pDD->bNumNeg=M4_FALSE;
               psNum.Asigna(a_pCad->DirIni(), a_pCad->m_iLonBuffer);
            }                                           
         }         
         
         iError=DesformateaNumero2Light(&psNum);
      }
      break;
      
      
      // FECHA ----------------------------------------------------------------
      
      case TIPOFECHA:
      {            
         iError=DesformateaFechaHoraLight(a_pCad, &sMaskDate);
      }
      break;
      
      
      // HORA -----------------------------------------------------------------
      
      case TIPO_TIME:
      {           
         iError=DesformateaFechaHoraLight(a_pCad, &sMaskTime24);                  
      }
      break;
      
      
      // TIMESTAMP ------------------------------------------------------------
      
      case M4ISOPOSDAY:
      {
         iError=DesformateaFechaHoraLight(a_pCad, &sMaskTimeStamp);      
      }
      break;
       
      
      // TIPO NO CONTEMPLADO -------------------------------------------------- 
       
      // no hace nada.
   }
   
   return iError;
}

/* static const m4char_t s_TablaLight[]=
 "                                                cccccccccc       "\
 "llllllllllllllllllllllllll"\
 "      "\
 "llllllllllllllllllllllllll"\
 "                                                                     "\
 "lllllllllllllllllllllll lllllllllllllllllllllllllllllll llllllll";
*/
static const m4char_t s_TablaLight[]= "                                                cccccccccc       " "llllllllllllllllllllllllll" "      " "llllllllllllllllllllllllll" "                                                                     " "lllllllllllllllllllllll lllllllllllllllllllllllllllllll llllllll";

void FMT_ClFormato::Normaliza(ClCadena  *pCadena)
{     
   pCadena->Mapeo(s_TablaMayToMin);
   if(D.iCountry=='S'*0x100+'P'){
	  pCadena->Mapeo(s_TablaQuitaAcentos);
   };
}
   

#define ASIGNADOS_ANIO            0
#define ASIGNADOS_MES             1
#define ASIGNADOS_DIASEMANA       2
#define ASIGNADOS_DIA             3
#define ASIGNADOS_HORA            4
#define ASIGNADOS_MINUTO          5
#define ASIGNADOS_SEGUNDO         6
                   
#define SUPOSICION_ANIO        1<<ASIGNADOS_ANIO
#define SUPOSICION_MES         1<<ASIGNADOS_MES
#define SUPOSICION_DIASEMANA   1<<ASIGNADOS_DIASEMANA
#define SUPOSICION_DIA         1<<ASIGNADOS_DIA
#define SUPOSICION_HORA        1<<ASIGNADOS_HORA
#define SUPOSICION_MINUTO      1<<ASIGNADOS_MINUTO
#define SUPOSICION_SEGUNDO     1<<ASIGNADOS_SEGUNDO


static void Reduccion(void)
{     
   m4bool_t cambio;                           
   ClMIT_TM *TM = GET_TM() ;
   StFMT_TaskEnv* poTaskEnv = TM_GET_TASK_ENV(TM) ;
   
   do{
      cambio=M4_FALSE;
      StSuposiciones  *pSup=poTaskEnv->Formato_g_Suposiciones;   
                     
      while(pSup->cuenta!=-1){
         if(pSup->cuenta==1){                        
            m4bool_t repe=M4_FALSE;
            StSuposiciones  *pSupObjetivo=pSup;
                        
            pSup=poTaskEnv->Formato_g_Suposiciones;   
            
            while(pSup->cuenta!=-1){
               if(pSup != pSupObjetivo){
                  if(pSup->cuenta==1){
                     if(pSup->tipo & pSupObjetivo->tipo){
                        repe=M4_TRUE;
                        break;
                     }
                  }   
               }
               
               pSup++;
            }
            
            if(repe){
               pSup=pSupObjetivo;
            }
            else{
               m4int16_t idi=0;
               m4int16_t idf=1;
                     
               while(pSupObjetivo->tipo != idf){
                  idi++;
                  idf<<=1;
               }
               
               poTaskEnv->Formato_g_Asignados[idi]=pSupObjetivo->valor;
               pSupObjetivo->cuenta=0;
               poTaskEnv->Formato_g_iSuposiciones--;        
               
               StSuposiciones  *pSup2=poTaskEnv->Formato_g_Suposiciones;   
                     
               while(pSup2->cuenta!=-1){
                  if(pSup2 != pSupObjetivo){                              
                     if(idf & pSup2->tipo){
                        pSup2->tipo &= ~idf;
                        pSup2->cuenta--;               
                        cambio=M4_TRUE;
                     }                             
                  }
                        
                  pSup2++;
               }         
               
               break;
            }                              
         }  
         
         pSup++;      
      }      
         
   }while(cambio && poTaskEnv->Formato_g_iSuposiciones);
}                  
   
m4int16_t FMT_ClFormato::DesformateaFechaHoraLight(ClCadena  *a_pCad, ClCadena  *a_pMask)
{
   m4int16_t iError=0;   
   
   m4int16_t i;
   
   StSuposiciones *pSup;
      
   m4char_t flagmaestro;
   
   for(i=0; i<7; i++)
      m_poTaskEnv->Formato_g_Asignados[i]=-2;
         
   if(!a_pMask->EsNula()){
      a_pMask->Posiciona(0, INICIO);                
      
      flagmaestro=0;
            
      // se obtienen pistas según los parámetros que aparecen en la máscara:
      
      m4int16_t  *pto=m_poTaskEnv->Formato_g_TablaOrden;
      m_poTaskEnv->Formato_g_iCuenta=0;
            
      for(;;){
         m4char_t c;
         c=a_pMask->LeeChar();
         if(c=='%'){          
            m4int16_t idi, idf;
            c=a_pMask->LeeChar();
            
            idi=s_pcIndicesFechHora[c];
            if(idi!=' '){
               idf=1<<(--idi);
               
               m_poTaskEnv->Formato_g_Asignados[idi]=-1;
                        
               flagmaestro |= idf;
               
               *(pto++)=idi;
               m_poTaskEnv->Formato_g_iCuenta++;
            }
         }
         
         if(!c)
            break;
      }
   }
   else
      flagmaestro=-1;
                  
   //--------------------------------------------------------------------------
   // SE CREA LA TABLA DE SUPOSICIONES
   
   pSup=m_poTaskEnv->Formato_g_Suposiciones;   
   
   ClCadena Cad;
   
   Cad.Crea(256);
   
   m_poTaskEnv->Formato_g_iSuposiciones=0;   
      
   a_pCad->Posiciona(0, INICIO);                
         
   for(i=0; i<8; i++)
      m_poTaskEnv->Formato_g_Suposiciones[i].cuenta=-1;      
   
   m4char_t tipoa=0;
   
   m4int16_t l;
   
   l=0;
   
   for(;;){
      m4char_t c=a_pCad->LeeChar();
      m4char_t tipo=s_TablaLight[c];
      
      if(tipoa){
         if(tipo!=tipoa){
            Cad.EscribeChar('\0');
                        
            m4int16_t valor;
            pSup->cuenta=0;            
            pSup->tipo=0;
            
            switch(tipoa)
            {            
               // Cifras:
               case 'c':
               {                        
                  valor=atoi(Cad.m_pDirIni);                        
                                       
                  // Dia:       
                  if(valor>0 && valor<32){                     
                     if(flagmaestro & SUPOSICION_DIA){
                        pSup->tipo |= SUPOSICION_DIA;
                        pSup->cuenta++;
                     }
                  }
               
                  // Mes:      
                  if(valor>0 && valor<13){                     
                     if(flagmaestro & SUPOSICION_MES){
                        pSup->tipo |= SUPOSICION_MES;
                        pSup->cuenta++;
                     }
                  }
               
                  // Año:
                  if(valor>-1){                     
                     if(flagmaestro & SUPOSICION_ANIO){
                        pSup->tipo |= SUPOSICION_ANIO;
                        pSup->cuenta++;
                     }
                  }
                
                  // DiaSemana:      
                  if(valor>0 && valor<8){                     
                     if(flagmaestro & SUPOSICION_DIASEMANA){
                        pSup->tipo |= SUPOSICION_DIASEMANA;
                        pSup->cuenta++;
                     }
                  } 
               
                  // Hora:
                  if(valor>-1 && valor<25){                     
                     if(flagmaestro & SUPOSICION_HORA){
                        pSup->tipo |= SUPOSICION_HORA;
                        pSup->cuenta++;
                     }
                  }
               
                  // Minuto:
                  if(valor>-1 && valor<60){                     
                     if(flagmaestro & SUPOSICION_MINUTO){
                        pSup->tipo |= SUPOSICION_MINUTO;
                        pSup->cuenta++;
                     }
                  }
               
                  // Segundo:
                  if(valor>-1 && valor<60){                     
                     if(flagmaestro & SUPOSICION_SEGUNDO){
                        pSup->tipo |= SUPOSICION_SEGUNDO;
                        pSup->cuenta++;
                     }
                  }  
                                    
                  if(pSup->cuenta){                                          
                     pSup->valor = valor;
                  }                  
               }
               break;
                        
               // Letras:                                                                                    
               case 'l':
               {           
				  Normaliza(&Cad);
                  valor=BuscaCadenaMes(Cad.DirIni(), 0, 12);
                  
                  if(valor){                     
                     pSup->tipo = SUPOSICION_MES;                                     
                  }
                  else{
					 Normaliza(&Cad);
                     valor=BuscaCadenaSemana(Cad.DirIni(), 0, 7);
                     
                     if(valor){                                       
                        pSup->tipo = SUPOSICION_DIASEMANA;                     
                     }
                  }
                  
                  if(valor){         
                     pSup->cuenta=1;                      
                  }
               }
               break;               
            }  
            
            if(pSup->cuenta){
               pSup->valor=valor;               
               if(m_poTaskEnv->Formato_g_iSuposiciones<6){
                  pSup++;
                  m_poTaskEnv->Formato_g_iSuposiciones++;
               }
               else
                  break;
            }
            
            tipoa=tipo;
            Cad.Posiciona(0, INICIO);                         
            
            l=0;
         }
      }   
      else
         tipoa=tipo;
      
      if(c)
         Cad.EscribeChar(c);
      else
         break;
         
      l++;   
   }
                        
   // Reducción lógica:
   Reduccion();                
   
   iError= (m_poTaskEnv->Formato_g_iSuposiciones)? SupRecur(m_poTaskEnv->Formato_g_Suposiciones) : Verifica();
      
   //--------------------------------------------------------------------------
   // GRABACION DE VALORES
   
   
   if(!iError){      
      // Año:   
      m4int16_t anio=m_poTaskEnv->Formato_g_Asignados[ASIGNADOS_ANIO];
      if(anio<100){
         struct tm newtime;
         time_t aclock;
         time( &aclock );                 /* Get time in seconds */
         M4ClDate::LocalTime( aclock, newtime );  /* Convert time to struct tm form  */
         anio+=(newtime.tm_year/100)*100+1900;
      }
      sprintf(m_poTaskEnv->ClM4Fmt_g_pDD->sAnio, "%d", anio);
         
      // Mes             
      sprintf(m_poTaskEnv->ClM4Fmt_g_pDD->sMes, "%d", m_poTaskEnv->Formato_g_Asignados[ASIGNADOS_MES]);
         
      // DiaSemana:        
      m4char_t p[2];
      strcpy(p, "0");
      m4int16_t diasemana=m_poTaskEnv->Formato_g_Asignados[ASIGNADOS_DIASEMANA];   
      p[0]=diasemana+'0';         
      strcpy(m_poTaskEnv->ClM4Fmt_g_pDD->sDiaSemana, p);
      
      // Dia:                    
      sprintf(m_poTaskEnv->ClM4Fmt_g_pDD->sDia, "%d", m_poTaskEnv->Formato_g_Asignados[ASIGNADOS_DIA]);
         
      // Hora:                                               
      sprintf(m_poTaskEnv->ClM4Fmt_g_pDD->sHora, "%d", m_poTaskEnv->Formato_g_Asignados[ASIGNADOS_HORA]);
         
      // Minuto:                                               
      sprintf(m_poTaskEnv->ClM4Fmt_g_pDD->sMinuto, "%d", m_poTaskEnv->Formato_g_Asignados[ASIGNADOS_MINUTO]);
         
      // Segundo:                                                  
      sprintf(m_poTaskEnv->ClM4Fmt_g_pDD->sSegundo, "%d", m_poTaskEnv->Formato_g_Asignados[ASIGNADOS_SEGUNDO]);
   }
               
   return iError;
}

//-----------------------------------------------------------------------------
   

m4int16_t FMT_ClFormato::DesformateaNumero2Light(ClCadena  *a_pNum)
{
   // dado un número, extrae la parte entera y decimal :
   m4int16_t iError;   
   m4int16_t l;
   ClCadena  *psCadenaDestino;
   
   iError=0;      
   l=a_pNum->m_iLonBuffer;      
   
   m_poTaskEnv->ClM4Fmt_g_pDD->sParteEntera.Crea(l);
   m_poTaskEnv->ClM4Fmt_g_pDD->sParteDecimal.Crea(l);
   
   psCadenaDestino=&(m_poTaskEnv->ClM4Fmt_g_pDD->sParteEntera);
      
   sDecimal.Posiciona(0, INICIO); 
   m4char_t cd=sDecimal.LeeChar();
   m4uint32_t ld=sDecimal.m_iLonBuffer-1;
   
   m4bool_t kk=M4_TRUE;
   
   for(;;){
      m4char_t c;
      c=a_pNum->LeeCharNA();

      if(c!='\0'){
         // comparamos con la cadena de los miles
         if(m_poTaskEnv->ClM4Fmt_g_pDD->sThousand.ComparaConNA(a_pNum)){
            // no coincide:                      
            // comparamos con la cadena de la coma
            if(m_poTaskEnv->ClM4Fmt_g_pDD->sDecimal.ComparaConNA(a_pNum)){
               // no coincide:
               // ... pues se escribe:         
               
               m4bool_t ok;

               if(c!=' '){
                  if(s_TablaLight[c]!='c'){
                     iError=ERROR_MALNUMERO;
                     break;
                  }
                  else{
                     ok=M4_TRUE;                     
                  }
               }
               else
                  ok=M4_FALSE;   // se salta los espacios
               
               if(ok){
                  psCadenaDestino->EscribeChar(c);
                  kk=M4_FALSE;
               }
               
               // ... se avanza al siguiente caracter:
               a_pNum->Posiciona(1, ACTUAL);               
            }
            else{
               if(kk){
                  m_poTaskEnv->ClM4Fmt_g_pDD->sParteEntera.EscribeChar('0');
               
               }
               psCadenaDestino=&(m_poTaskEnv->ClM4Fmt_g_pDD->sParteDecimal);
            }
         }
      }
      else
         break;
   }                             
   
   m_poTaskEnv->ClM4Fmt_g_pDD->sParteEntera.EscribeChar('\0');
   m_poTaskEnv->ClM4Fmt_g_pDD->sParteDecimal.EscribeChar('\0');
           
   return iError;
}

m4int16_t FMT_ClFormato::SupRecur(StSuposiciones  *pSup)
{
   m4int16_t iError;
   
   iError=0;
   
   if(m_poTaskEnv->Formato_g_iSuposiciones){
      while(pSup->cuenta!=-1){    
       if(pSup->cuenta){
         m4int16_t  *pto=m_poTaskEnv->Formato_g_TablaOrden;
         for(m4int16_t i=m_poTaskEnv->Formato_g_iCuenta; i; i--){
            m4int16_t idi=*(pto++);
            m4int16_t idf=1<<idi;
                        
            if(pSup->tipo & idf){            
               if(m_poTaskEnv->Formato_g_Asignados[idi]<0){               
                  m4int16_t Suposiciones=m_poTaskEnv->Formato_g_iSuposiciones;
                  m4int16_t tAsignados[7];
                  StSuposiciones tSuposiciones[8];               
                  memcpy(tAsignados, m_poTaskEnv->Formato_g_Asignados, sizeof(tAsignados));                  
                  memcpy(tSuposiciones, m_poTaskEnv->Formato_g_Suposiciones, sizeof(tSuposiciones));
                  
                  m_poTaskEnv->Formato_g_Asignados[idi]=pSup->valor;                                 
                  pSup->tipo = idf;
                  pSup->cuenta = 0;
                  
                  StSuposiciones  *pSup2=m_poTaskEnv->Formato_g_Suposiciones;                        
                  while(pSup2->cuenta!=-1){
                     if(pSup2 != pSup){                              
                        if(idf & pSup2->tipo){
                           pSup2->tipo &= ~idf;
                           pSup2->cuenta--;                                       
                        }                             
                     }
                           
                     pSup2++;
                  }                
                  
                  m_poTaskEnv->Formato_g_iSuposiciones--;
                                                                  
                  Reduccion();
                  
                  if(SupRecur(pSup+1)){
                     // Error:                              
                     m_poTaskEnv->Formato_g_iSuposiciones=Suposiciones;
                     memcpy(m_poTaskEnv->Formato_g_Asignados, tAsignados, sizeof(m_poTaskEnv->Formato_g_Asignados));                  
                     memcpy(m_poTaskEnv->Formato_g_Suposiciones, tSuposiciones, sizeof(m_poTaskEnv->Formato_g_Suposiciones));                                    
                  }
                  else{
                     // Ok:
                     // m_poTaskEnv->Formato_g_iSuposiciones deberia ser 0
                     break;               
                  }
                  
               }
            }
         }      
       }        
       
       if(m_poTaskEnv->Formato_g_iSuposiciones)                  
          pSup++;
       else
          break;       
      } 
   }
   
   return Verifica();
}
   
// Verificación:   
m4int16_t FMT_ClFormato::Verifica(void)
{
   m4int16_t iError;
   iError=0;
   
   m4int16_t anio;
   m4int16_t mes;
   m4int16_t diasemana;
   m4int16_t dia;
   m4int16_t hora;
   m4int16_t minuto;
   m4int16_t segundo;   
   
   // Año:   
   anio=m_poTaskEnv->Formato_g_Asignados[ASIGNADOS_ANIO];
   
   if(anio<0){
      if(anio==-1)
         iError=ERROR_MALANIO;      
   }
      
   // Mes          
   mes=m_poTaskEnv->Formato_g_Asignados[ASIGNADOS_MES];
   if(mes<0){
      if(mes==-1)
         iError=ERROR_MALMES;      
   }
      
   // DiaSemana:           
   diasemana=m_poTaskEnv->Formato_g_Asignados[ASIGNADOS_DIASEMANA];
   if(diasemana<0){      
      //
   }
      
   // Dia:              
   dia=m_poTaskEnv->Formato_g_Asignados[ASIGNADOS_DIA];
   if(dia<0){
      if(dia==-1)
         iError=ERROR_MALDIA;
   }
   else{ 
      
      m4char_t TablaDiasMeses[12]={31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
         
      if(dia>TablaDiasMeses[mes-1])
         iError=ERROR_MALDIA;
      else{         
         if(mes==2 && dia==29){
            // Febrero:          
            m4int16_t anio2=(anio%100)? anio: anio/100;               
            if(anio2%4){
               // no es bisiesto:
               iError=ERROR_MALDIA;                  
            }
         }                 
      }         
   }
      
   // Hora:                 
   hora=m_poTaskEnv->Formato_g_Asignados[ASIGNADOS_HORA];
   if(hora<0){                 
      m_poTaskEnv->Formato_g_Asignados[ASIGNADOS_HORA]=0;
   }
      
   // Minuto:                 
   minuto=m_poTaskEnv->Formato_g_Asignados[ASIGNADOS_MINUTO];
   if(minuto<0){
      m_poTaskEnv->Formato_g_Asignados[ASIGNADOS_MINUTO]=0;
   }
      
   // Segundo:                 
   segundo=m_poTaskEnv->Formato_g_Asignados[ASIGNADOS_SEGUNDO];
   if(segundo<0){
      m_poTaskEnv->Formato_g_Asignados[ASIGNADOS_SEGUNDO]=0;
   }
      
   return iError;
}                    

//-----------------------------------------------------------------------------
      
m4bool_t FMT_ClFormato::ComparaNombre(void  *a_pcNombre, void  *a_piHandle)
{                                            
   m4bool_t b;
   
   if( ! strcmp((m4char_t* )a_pcNombre, m_pcNombre) ){
      *((m4int16_t  *)a_piHandle) = m_uHandle;
      b=M4_FALSE;    // no busca más
   }
   else
      b=M4_TRUE;
      
   return b;   
}


m4int16_t FMT_ClFormato::co_iFDecGrup(m4char_t*  a_pcValor)
{  
   m4int16_t iError;
   if(*a_pcValor){      
      iError=CambiaEntero(a_pcValor, &(D.iDecGrup));   
      
      if(D.iDecGrup>3 || D.iDecGrup<1)
      {
         D.iDecGrup=0xffff;
         iError=ERROR_VALORNOCORRECTO;      
      }
   }
   else{  
      D.iDecGrup=0xffff;
      iError=0;      
   }
   
   return iError;
}


m4int16_t FMT_ClFormato::co_iFBlank( m4char_t* a_pcValor )
{
   m4int16_t iError;
   
   if(*a_pcValor){      
      iError=CambiaEntero(a_pcValor, &(D.iBlank));   
      
      /*if(D.iDecGrup>1 || D.iDecGrup<0)
      {
         D.iDecGrup=0xffff;
         iError=ERROR_VALORNOCORRECTO;      
      }
   }
   else{  
      D.iDecGrup=0xffff;
      iError=0;      */
   };
   
   return iError;
}

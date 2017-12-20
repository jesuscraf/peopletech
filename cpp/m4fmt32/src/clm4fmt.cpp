//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             CLM4FMT.HPP
// Project:			 M4FMT32.DLL
// Author:           Meta Software M.S. , S.A
// Date:			 18/02/98 - 12/06/98
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

///////////////////////////////////////////////////////////////////////////////                                                                                             ///////////////////////////////////////////////////////////////////////////////
// INCLUDES        

#include "m4types.hpp"
#define _INCLUDE_BUILDING_RESOURCES_
#include "rctables.h"

#include "m4win.hpp"    //Para LoadString y SQL. Resolver en sinc

#include <stdlib.h>
#include <stdio.h>
                
#include "fmt.h"
#include "fmtdf.cpp"
   
#include "format.h"
#include "cadena.h"
#include "castella.hpp"

#define INCLUDING_FROM_CLM4FMT
#include "clm4fmt.hpp"
#include "clm4fmt.hmg"

#define SQL_VARCHAR        12

///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//  GLOBAL: Tabla de conversion de minusculas a mayusculas
//-----------------------------------------------------------------------------

extern m4char_t s_TablaMayToMin[257];


//-----------------------------------------------------------------------------
// Funcion global: Carga una cadena soliciata de los recursos definidos en
//                 el modulo rctables.h
//-----------------------------------------------------------------------------

m4int16_t g_CargaCadena(m4uint32_t id, m4char_t* buf, m4int16_t lbuf)
{
	size_t SizeTable=sizeof(g_stResources)/sizeof(StResources) ;
	size_t Aux ;

	m4int32_t Beg, End, Center ;	//Deben tener signo
	const StResources *FoundIt, *Pos ;

	FoundIt=NULL ; Beg=0; End=SizeTable-1 ; 
	while (End>=Beg) {
		Center=(Beg+End)/2 ;
		Pos=&g_stResources[Center] ;
		if (Pos->m_iResId==id) {
			FoundIt=Pos ;
			break ;
		} else {
			if (Pos->m_iResId>id) {
				End=Center-1 ;
			} else {
				Beg=Center+1 ;
			} ;
		} ;
	}  ;
	
exit:
	if (FoundIt) {
		Aux=strlen(FoundIt->m_pcResStr) ;
		if (Aux+1>lbuf ) Aux=lbuf-1 ;
		strncpy( buf, FoundIt->m_pcResStr, Aux) ;
		buf[Aux]='\0' ;
		return Aux ;
	} else {
		buf[0]='\0' ;
		return 0 ;
	} ;

}

//-------------------------------------------------------------------------------
//   Funcion de uso global: Convierte un número en una cadena
//-------------------------------------------------------------------------------

m4int16_t g_IntToStr(m4int16_t a_n, m4char_t* a_buf, m4int16_t a_lbuf)
{        
	m4int16_t resto;                             
	m4int16_t i;
	m4int16_t l;
	m4char_t buf[256]; // maximo 256 caracteres
         
	l=0;
	if(a_lbuf){      
		i=254;
		resto = (a_n<0)? -a_n : a_n;
   
		do{        
			m4int16_t k;
			m4int16_t u;
      
			k=resto/10;
			u=resto-k*10;
			resto=k;
			buf[i--]=(char)(u+'0');               
		} while(resto);   
   
		if(a_n<0){
			buf[i--]='-';      
		};
            
		l=i+a_lbuf;
		if(l>255) {
			l=255;
		};
      
		buf[l]='\0';      
		strcpy(a_buf, buf+i+1);
   };
   
   return l-i-1;
}

//-----------------------------------------------------------------------------
// s_DameTipoInterno: Convierte al tipo de datos interno a partir del tipo 
//                    SQL o M4 mediante las tablas de conversion estaticas
//-----------------------------------------------------------------------------

// La tabla está ordenada por el valor del identificador SQL :   

static const m4int16_t TablaSQLToInterno[]=
{               
   20,                  // TAMAÑO DE LA TABLA
   TIPO_ERROR,          // 0
   TIPOALFA,            // SQL_m4char_t (1)
   TIPONUMERO,          // SQL_NUMERIC (2)
   TIPONUMERO,          // SQL_DECIMAL (3)
   TIPONUMERO,          // SQL_INTEGER (4)
   TIPONUMERO,          // SQL_SMALLINT (5)
   TIPONUMERO,          // SQL_FLOAT (6)
   TIPONUMERO,          // SQL_REAL (7)
   TIPONUMERO,          // SQL_DOUBLE (8)
   TIPOFECHA,           // SQL_DATE (9)
   TIPO_TIME,           // SQL_TIME (10)
   M4ISOPOSDAY,         // SQL_TIMESTAMP (11)
   TIPOALFA,            // SQL_VARm4char_t (12)
   TIPOALFA,            // SQL_LONGVARm4char_t (-1)
   TIPO_BINARY,         // SQL_BINARY (-2)
   TIPO_BINARY,         // SQL_VARBINARY (-3)
   TIPO_BINARY,         // SQL_LONGVARBINARY (-4)
   TIPONUMERO,          // SQL_BIGINT (-5)
   TIPONUMERO,          // SQL_TINYINT (-6)
   TIPO_BIT             // SQL_BIT (-7)
};


// La tabla está ordenada por el valor del identificador M4 :   

static const m4int16_t TablaM4ToInterno[]=
{               
   9,                   // TAMAÑO DE LA TABLA
   TIPOALFA,            // TIPOALFA    (0)
   TIPOFECHA,           // TIPOFECHA   (1)
   TIPONUMERO,          // TIPONUMERO  (2)
   TIPO_ERROR,          // 3
   TIPO_ERROR,          // 4
   TIPO_ERROR,          // 5
   TIPO_ERROR,          // 6
   TIPO_ERROR,          // 7
   M4ISOPOSDAY,         // M4ISOPOSDAY (8)
};

static m4int16_t s_DameTipoInterno(m4int16_t a_iTipo, m4int16_t a_iIsSQLM4)
{                  
	m4int16_t iResul;
	m4int16_t indice;                           
	const m4int16_t  *pTabla;
           
	switch(a_iIsSQLM4){
		case TIPO_SQL:   
			pTabla=TablaSQLToInterno;
			indice = (a_iTipo<0)? SQL_VARCHAR-a_iTipo : a_iTipo;
			iResul=(indice >= *pTabla)? TIPO_ERROR : pTabla[indice+1];
			break;   
   
		case TIPO_M4:
			pTabla=TablaM4ToInterno;
			indice=a_iTipo;
			iResul=(indice >= *pTabla)? TIPO_ERROR : pTabla[indice+1];
			break;
         
		default:   
			iResul=TIPO_ERROR;
   };
      
   return iResul;
}

///////////////////////////////////////////////////////////////////////////////


static void DestroyLight(void)
{
	ClMIT_TM *TM = GET_TM() ;
	StFMT_TaskEnv* poTaskEnv = TM_GET_TASK_ENV(TM) ;
	m4int16_t i;

	for(i=0; i<(12+7); i++){              
		delete []poTaskEnv->g_LightString[i];
	};
}

//-------------------------------------------------------------------------------
// Devuelve la posición del primer caracter distinto en las cadenas
//	Ejemplo: _Compara(pepe, pepito) -> 3
//	Ejemplo: _Compara(hola, pepito) -> 0
//-------------------------------------------------------------------------------

static m4int16_t _Compara(m4char_t* p, m4char_t* q)
{
	m4int16_t i = 0;
   
	for(;;){
		m4char_t cp = *p;
		m4char_t cq = *q;
      
		if( (cp == '\0') || (cp == '\0') || (cq != cp) ){
			break;
		};
		p++;
		q++;
		i++;
	};
   
	return i;
}

static void CreateLight(void)
{
	ClMIT_TM *TM = GET_TM() ;
	StFMT_TaskEnv* poTaskEnv = TM_GET_TASK_ENV(TM) ;
	m4int16_t aux[12+7];
   
	m4int16_t i;
	for(i=0; i<(12+7); i++){              
		m4char_t* buf = new char[8];
		g_CargaCadena(i + IDS_MESES, buf, 8);
		m4char_t c = s_TablaMayToMin[*buf]; // a minuscula
		if(c!=' '){
			*buf = c;
		};
      
		poTaskEnv->g_LightString[i] = buf;
		aux[i] = 0;
	};
   
	for(i=0; i<(12+7-1); i++){
		m4int16_t t;               
		for(t=i+1; t<(12+7); t++){
			m4int16_t k;
			k = _Compara(poTaskEnv->g_LightString[i], poTaskEnv->g_LightString[t]);
			if(k>aux[i]){
	            aux[i] = k;
			};
			if(k>aux[t]){
	            aux[t] = k;
			};
		};
	};
   
	for(i=0; i<(12+7); i++){
		if (*(poTaskEnv->g_LightString[i]+aux[i])) {
				*(poTaskEnv->g_LightString[i]+aux[i]+1) = '\0';      
		} ;
	};   
}

///////////////////////////////////////////////////////////////////////////////
// FUNCIONES DEL API DE FORMATEO
///////////////////////////////////////////////////////////////////////////////    

CLM4FMT::CLM4FMT()
{
	ClMIT_TM *TM = GET_TM() ;
	m_poTaskEnv = TM_GET_TASK_ENV(TM) ;
	m_poInsp=GET_INSP();

	_M4FMTInitialice ();
}

void CLM4FMT::_M4FMTInitialice(void)
{                                        
	m4int16_t iError;
	m_poTaskEnv->ClM4Fmt_g_uCount = 0;
   
	m_poTaskEnv->ClM4Fmt_g_pFormatoWINDOWS = new FMT_ClFormato("WINDOWS");
	if(m_poTaskEnv->ClM4Fmt_g_pFormatoWINDOWS!=NULL){
		iError=m_poTaskEnv->ClM4Fmt_g_pFormatoWINDOWS->GetError();
		if(!iError){
			m_poTaskEnv->ClM4Fmt_g_pFormatoWINDOWS->AutoInsertIn(HANDLE_WINDOWS);
		};
	};
   
	m_poTaskEnv->ClM4Fmt_g_pFormatoISO = new FMT_ClFormato("ISO");
	if(m_poTaskEnv->ClM4Fmt_g_pFormatoISO!=NULL){           
		iError=m_poTaskEnv->ClM4Fmt_g_pFormatoISO->GetError();
		if(!iError){
			m_poTaskEnv->ClM4Fmt_g_pFormatoISO->AutoInsertIn(HANDLE_ISO);
		};
	};
   
	CreateLight();
	m_poTaskEnv->ClM4Fmt_g_bInitialize=M4_TRUE;   
}

void CLM4FMT::_M4FMTTerminate(void)
{              
	FMT_ClContainer  *pac=g_GetAutoContainer();
         
	if(pac != NULL){
		pac->AutoDestroy();
		DestroyLight();
	};

	//DestroyLight();         
	m_poTaskEnv->ClM4Fmt_g_bInitialize=M4_FALSE;           
}
         

//-----------------------------------------------------------------------------
//  Crea un formato nuevo  
//-----------------------------------------------------------------------------

m4int16_t CLM4FMT::_M4FMTCreateNewFormat(m4char_t* a_pcNombre, m4int16_t * a_piH)
{     
	m4int16_t iError=0;
	FMT_ClFormato  *pf;   

	if(!m_poTaskEnv->ClM4Fmt_g_bInitialize){
		_M4FMTInitialice();
	};

	pf=NULL;     
	*a_piH=0;
	pf=new FMT_ClFormato(a_pcNombre);  // por defecto en castellano
	if(pf!=NULL){
		iError=pf->GetError();
		if(!iError){        
			*a_piH=pf->AutoInsert();
			iError=pf->GetError();         
		};
      
		if(iError){
			delete pf;
		} else {
			m_poTaskEnv->ClM4Fmt_g_uCount++;
		};
	} else {
      iError=ERROR_NOMEMORY;
	};

	//============= Trace File =================
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLM4FMT_MSG_10 ] ) <<
		       a_pcNombre << *a_piH << SEND_MSG ;
	};	
	//==========================================

	//Español por defecto
	_M4FMTSetProperty(*a_piH,"sFCountry","SP");

	return (iError)? iError : EXITO;
}


//-----------------------------------------------------------------------------
//  Modifica una propiedad de un formato
//-----------------------------------------------------------------------------

m4int16_t CLM4FMT::_M4FMTSetProperty(m4int16_t a_iHandle, m4char_t* a_pcNombre, m4char_t* a_pcValor)
{
	m4int16_t iError=0;   
	FMT_ClFormato  *pf;   
      
	//Obtiene el formato del array de formatos internos mediante el handle
	pf=(FMT_ClFormato  *)g_M4CTGetElement(a_iHandle);
      
	if(pf != NULL){
		if(strcmp(a_pcNombre, "sFCountry")){
			iError=pf->CambiaPropiedad(a_pcNombre, a_pcValor);
		} else {
			m4uint16_t id=a_pcValor[0]*0x100+a_pcValor[1];
			if(id=='S'*0x100+'P') {
				//Si idioma español, activa por defecto los acentos
				iError=pf->CambiaPropiedad("iFAcentLess", "0");
			};
			if(id!=pf->DameIdioma()){
				pf->D.iCountry=id;
				iError=m_poTaskEnv->m_poPlugIn->GetPlugInData(id, &pf->m_StPlugInInfo);
			};
		};         
	} else {
      iError=ERROR_INVALIDHANDLE;
	};


	//============ Trace File ==============================
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLM4FMT_MSG_20 ] ) <<
			  a_pcValor << a_pcNombre << a_iHandle << SEND_MSG ;
	};
	//=======================================================

   return (iError)? iError : EXITO;
}

//-----------------------------------------------------------------------------
// Destruye un formato correspondiente al Handle que le pasamos
//-----------------------------------------------------------------------------

m4int16_t CLM4FMT::_M4FMTDestroyFormat(m4int16_t a_iHandle)
{
	m4int16_t iResult;   
	FMT_ClFormato  *pf;
	m4int16_t iError=0;

	//Obtiene el formato del array interno mediante el Handle
	pf=(FMT_ClFormato  *)g_M4CTGetElement(a_iHandle);
   
	if(pf != NULL){
   		delete pf;  
	    iResult=EXITO;                           
	    if(!--m_poTaskEnv->ClM4Fmt_g_uCount){
			_M4FMTTerminate();
	    };
	} else {
      iResult=ERROR_INVALIDHANDLE;
	};

	//======== Trace File =============================
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLM4FMT_MSG_30 ] ) << a_iHandle << SEND_MSG ;
	};
	//=================================================

	return iResult;
}

//-----------------------------------------------------------------------------
// Crea un formato a partir de otro ya existente
//-----------------------------------------------------------------------------

m4int16_t CLM4FMT::_M4FMTCreateAddFormat(m4int16_t a_iHandleIn, m4char_t* a_pcNombre, m4int16_t * a_piHandleOut)
{
	m4int16_t iError=0;
	FMT_ClFormato  *pfIn;
	FMT_ClFormato  *pfOut;
	
	if(!m_poTaskEnv->ClM4Fmt_g_bInitialize){
		_M4FMTInitialice();
	};
	
	*a_piHandleOut=0;       
   
	pfIn=(FMT_ClFormato  *)g_M4CTGetElement(a_iHandleIn);
      
	if(pfIn != NULL){                  
		// idioma de entrada: el del formato origen.
		pfOut=new FMT_ClFormato(pfIn);
		iError=m_poTaskEnv->m_poPlugIn->GetPlugInData(pfOut->DameIdioma(), &pfOut->m_StPlugInInfo);     
		if(!iError){                        
			pfOut->IniciaNombre(a_pcNombre);
			*a_piHandleOut=pfOut->AutoInsert();                   
			iError=pfOut->GetError();                  
		};
      
		if(iError){
			delete pfOut;
		} else {
			m_poTaskEnv->ClM4Fmt_g_uCount++;
		};
      
	} else {
		iError=ERROR_INVALIDHANDLE;
	};
   
	//=============== Trace File =====================
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLM4FMT_MSG_50 ] ) << 
		       a_pcNombre << *a_piHandleOut << a_iHandleIn << SEND_MSG ;
	};
	//================================================

	return (iError)? iError : EXITO;
}

//-----------------------------------------------------------------------------
//  Convierte una cadena de un formato a otro
//-----------------------------------------------------------------------------

m4int16_t CLM4FMT::_M4FMTConvertFmtToFmt(m4int16_t a_iHandleIn, m4char_t* a_pcIn, m4int16_t a_iTipo,
										 m4int16_t a_iIsSQLM4, m4int16_t a_iHandleOut, m4char_t* a_pcOut,
										 m4int16_t a_iLonOut, m4int16_t * a_piLonEscrita)
{
    m4int16_t iError=0;      
	FMT_ClFormato  *pfIn;                           

	if(!m_poTaskEnv->ClM4Fmt_g_bInitialize){
		_M4FMTInitialice();
	};
   
	pfIn=(FMT_ClFormato  *)g_M4CTGetElement(a_iHandleIn);
   
	if(pfIn != NULL){
		FMT_ClFormato *pfOut;    
		pfOut=(FMT_ClFormato *)g_M4CTGetElement(a_iHandleOut);
     
		if(pfOut != NULL){
			m4int16_t iTipo; 
			iTipo=s_DameTipoInterno(a_iTipo, a_iIsSQLM4);
         
			if(iTipo!=TIPO_ERROR){
	            ClCadena  *pCadIn  = new ClCadena;
				pfOut->IniciaCadenaEntrada(pCadIn, a_pcIn);
            
				ClCadena  *pCadOut = new ClCadena;      
				pCadOut->Asigna(a_pcOut, a_iLonOut);
            
				m_poTaskEnv->ClM4Fmt_g_pDD = new StDatosDesformateados;
               
				// SUSTITUCION::ANTES ------------------------------------------------------      
				pfOut->SustituyeAntes(&pCadIn);
            
				// DESFORMATEO -------------------------------------------------------------      
				iError=pfIn->Desformatea(pCadIn, iTipo);
            
				if(!iError){                                                                
					// FORMATEO -------------------------------------------------------------
					iError=pfOut->Formatea(pCadOut, iTipo);
               
					if(!iError){   
						// SUSTITUCION::DESPUES ----------------------------------------------
						pfOut->SustituyeDespues(&pCadOut);
						*a_piLonEscrita=pCadOut->LonReal();      
					};
				};

				// U-CASE  Y SENTENCE FORMAT -------------------------------------------------

				if(pfOut->GetCase()==3) {
					iError=pfOut->SentenceCase(pCadOut->DirIni(), iTipo);
				};

				if(pfOut->GetCase()==4) {
					if(pfOut->GetWordMode() || pfOut->GetPrefixeMode()|| pfOut->GetSeparatorMode()){
						iError=pfOut->CaseFormat(pCadOut->DirIni(), iTipo);
					};
				};

				delete m_poTaskEnv->ClM4Fmt_g_pDD;
				delete pCadOut;      
				delete pCadIn;      
			} else {
	            iError=ERROR_MALTIPO;
			};
		};

//====== CODIGO NUEVO PARA TRATAR LA FECHA MAS INFINITA 
	//Tipo Date
		if(a_iTipo==1 && (pfOut->DameBlank())==1 && a_pcIn==NULL) {
			a_pcIn = new m4char_t[10];
			if(pfOut->DamePlusInfDate()){
				strcpy(a_pcOut,pfOut->DamePlusInfDate());
			} else {
				strcpy(a_pcOut,"4000-01-01");
			};
		} else if(pfOut->DamePlusInfDate()){ 
			if(a_iTipo==1 && (pfOut->DameBlank())==1 && !strcmp(a_pcIn,pfOut->DamePlusInfDate())) {
				a_pcIn = new m4char_t[10];
				strcpy(a_pcOut,"");
			};
		} else if(!pfOut->DamePlusInfDate()){
			if(a_iTipo==1 && (pfOut->DameBlank())==1 && !strcmp(a_pcIn,"4000-01-01")) {
				a_pcIn = new m4char_t[10];
				strcpy(a_pcOut,"");
			};
		};

	//Tipo TimeStamp(ISO)
		if(a_iTipo==8 && (pfOut->DameBlank())==1 && a_pcIn==NULL) {
			a_pcIn = new m4char_t[10];
			strcpy(a_pcOut,"4000-01-01 00:00:00");
		} else if(a_iTipo==8 && (pfOut->DameBlank())==1 && !strcmp(a_pcIn,"4000-01-01 00:00:00")) {
			a_pcIn = new m4char_t[10];
			strcpy(a_pcOut,"");
		};

/////////////////////////////////////////////
//====== CODIGO NUEVO PARA TRATAR LA FECHA MINUS INFINITA 
	//Tipo Date
		if(a_iTipo==1 && (pfOut->DameBlank())==1 && a_pcIn==NULL) {
			a_pcIn = new m4char_t[10];
			if(pfOut->DameMinusInfDate()){
				strcpy(a_pcOut,pfOut->DameMinusInfDate());
			} else {
				strcpy(a_pcOut,"1800-01-01");
			};
		} else if(pfOut->DameMinusInfDate()){ 
			if(a_iTipo==1 && (pfOut->DameBlank())==1 && !strcmp(a_pcIn,pfOut->DameMinusInfDate())) {
				a_pcIn = new m4char_t[10];
				strcpy(a_pcOut,"");
			};
		} else if(!pfOut->DameMinusInfDate()){
			if(a_iTipo==1 && (pfOut->DameBlank())==1 && !strcmp(a_pcIn,"4000-01-01")) {
				a_pcIn = new m4char_t[10];
				strcpy(a_pcOut,"");
			};
		};

	//Tipo TimeStamp(ISO)
		if(a_iTipo==8 && (pfOut->DameBlank())==1 && a_pcIn==NULL) {
			a_pcIn = new m4char_t[10];
			strcpy(a_pcOut,"1800-01-01 00:00:00");
		} else if(a_iTipo==8 && (pfOut->DameBlank())==1 && !strcmp(a_pcIn,"1800-01-01 00:00:00")) {
			a_pcIn = new m4char_t[10];
			strcpy(a_pcOut,"");
		};

/////////////////////////////////////////////

	} else {
		iError=ERROR_INVALIDHANDLE;
	};

	//=============== Trace File =====================
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLM4FMT_MSG_50 ] ) << 
		       a_pcOut << SEND_MSG ;
	};
	//================================================

	return (iError)? iError : EXITO;
}


//-----------------------------------------------------------------------------
//   Aplica el formato a una cadena, devolviendo el resultado en el buffer pasado
//------------------------------------------------------------------------------

m4int16_t CLM4FMT::_M4FMTApplyFormat(m4int16_t a_iHandle, m4char_t* a_pcIn, m4int16_t a_iTipo,
									 m4int16_t a_iIsSQLM4, m4char_t* a_pcOut, m4int16_t a_iLonOut,
									 m4int16_t * a_piLonEscrita)
{

	//=============== Trace File =====================
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLM4FMT_MSG_40 ] ) << 
		       a_iHandle << a_pcIn << SEND_MSG ;
	};
	//=================================================	

	return _M4FMTConvertFmtToFmt(HANDLE_ISO, a_pcIn, a_iTipo, a_iIsSQLM4, a_iHandle, a_pcOut, a_iLonOut, a_piLonEscrita);
}


//-----------------------------------------------------------------------------
//   Convierte una cadena de formato ISO a formato win.ini
//-----------------------------------------------------------------------------

m4int16_t CLM4FMT::_M4FMTConvertIsoToWin(m4char_t* a_pcIn, m4int16_t a_iTipo, m4int16_t a_iIsSQLM4,
										 m4char_t* a_pcOut, m4int16_t a_iLonOut, m4int16_t * a_piLonEscrita)
{
	//=============== Trace File =====================
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLM4FMT_MSG_70 ] ) << a_pcIn << SEND_MSG ;
	};
	//=================================================	

	return _M4FMTConvertFmtToFmt(HANDLE_ISO, a_pcIn, a_iTipo, a_iIsSQLM4, HANDLE_WINDOWS, a_pcOut, a_iLonOut, a_piLonEscrita);   
}


//-----------------------------------------------------------------------------
//   Convierte una cadena de formato win.ini a formato ISO
//-----------------------------------------------------------------------------

m4int16_t CLM4FMT::_M4FMTConvertWinToIso(m4char_t* a_pcIn, m4int16_t a_iTipo, m4int16_t a_iIsSQLM4,
										 m4char_t* a_pcOut, m4int16_t a_iLonOut, m4int16_t * a_piLonEscrita)
{
	//=============== Trace File =====================
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLM4FMT_MSG_60 ] ) << a_pcIn << SEND_MSG ;
	};
	//=================================================	

	return _M4FMTConvertFmtToFmt(HANDLE_WINDOWS, a_pcIn, a_iTipo, a_iIsSQLM4, HANDLE_ISO, a_pcOut, a_iLonOut, a_piLonEscrita);
}

//-----------------------------------------------------------------------------
//   Convierte una cadena del formato pasado a ISO
//------------------------------------------------------------------------------

m4int16_t CLM4FMT::_M4FMTConvertFmtToIso(m4int16_t a_iHandle, m4char_t* a_pcIn, m4int16_t a_iTipo,
										 m4int16_t a_iIsSQLM4, m4char_t* a_pcOut, m4int16_t a_iLonOut,
										 m4int16_t * a_piLonEscrita)
{
	//=============== Trace File =====================
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLM4FMT_MSG_80 ] ) << 
		       a_pcIn << a_iHandle << SEND_MSG ;
	};
	//=================================================	
	
	return _M4FMTConvertFmtToFmt(a_iHandle, a_pcIn, a_iTipo, a_iIsSQLM4, HANDLE_ISO, a_pcOut, a_iLonOut, a_piLonEscrita);   
}

//-----------------------------------------------------------------------------
//   Convierte de un formato dado por el usuario a un formato interno
//------------------------------------------------------------------------------

m4int16_t CLM4FMT::_M4FMTConvertUserToFmt(m4int16_t a_iHandleIn, m4char_t* a_pcIn, m4int16_t a_iTipo,
										  m4int16_t a_iIsSQLM4, m4int16_t a_iHandleOut, m4char_t* a_pcOut,
										  m4int16_t a_iLonOut, m4int16_t * a_piLonEscrita)
{
	m4int16_t iError=0;      
	FMT_ClFormato  *pfIn;                           
   
	if(!m_poTaskEnv->ClM4Fmt_g_bInitialize){
		_M4FMTInitialice();
	};

	pfIn=(FMT_ClFormato  *)g_M4CTGetElement(a_iHandleIn);
   
	if(pfIn != NULL){
		FMT_ClFormato  *pfOut;    
		pfOut=(FMT_ClFormato  *)g_M4CTGetElement(a_iHandleOut);
      
		if(pfOut!=NULL){         
			m4int16_t iTipo;         
			iTipo=s_DameTipoInterno(a_iTipo, a_iIsSQLM4);
         
			if(iTipo!=TIPO_ERROR){
				ClCadena  *pCadIn  = new ClCadena;
				pfOut->IniciaCadenaEntrada(pCadIn, a_pcIn);
				ClCadena  *pCadOut = new ClCadena;      
				pCadOut->Asigna(a_pcOut, a_iLonOut);
				m_poTaskEnv->ClM4Fmt_g_pDD = new StDatosDesformateados;
               
				// SUSTITUCION::ANTES ------------------------------------------------------      
				pfOut->SustituyeAntes(&pCadIn);
            
				// DESFORMATEO LIGHT -------------------------------------------------------
				iError=pfIn->DesformateaLight(pCadIn, iTipo);
            
				if(!iError){                                                                
					// FORMATEO -------------------------------------------------------------
					iError=pfOut->Formatea(pCadOut, iTipo);
               
					if(!iError){   
						// SUSTITUCION::DESPUES ----------------------------------------------
						pfOut->SustituyeDespues(&pCadOut);
						*a_piLonEscrita=pCadOut->LonReal();      
					};
				};
            
				delete m_poTaskEnv->ClM4Fmt_g_pDD;
				delete pCadOut;      
				delete pCadIn;      
			} else {
				iError=ERROR_MALTIPO;
			};
		};
	} else {
		iError=ERROR_INVALIDHANDLE;
	};   

	//=============== Trace File =====================
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLM4FMT_MSG_90 ] ) << 
		       a_pcIn << a_iHandleIn << a_iHandleOut << SEND_MSG ;
	};
	//=================================================	

	return (iError)? iError : EXITO;
}


//-----------------------------------------------------------------------------
// Convierte un identificador de tipo SQL a tipo M4
//-----------------------------------------------------------------------------

// La tabla está ordenada por el valor del identificador SQL :   
static const m4int16_t s_TablaSQLToM4[]=
{               
   TIPO_ERROR,          // 0
   TIPOALFA,            // SQL_m4char_t (1)
   TIPONUMERO,          // SQL_NUMERIC (2)
   TIPONUMERO,          // SQL_DECIMAL (3)
   TIPONUMERO,          // SQL_INTEGER (4)
   TIPONUMERO,          // SQL_SMALLINT (5)
   TIPONUMERO,          // SQL_FLOAT (6)
   TIPONUMERO,          // SQL_REAL (7)
   TIPONUMERO,          // SQL_DOUBLE (8)
   TIPOFECHA,           // SQL_DATE (9)
   TIPO_ERROR,          // SQL_TIME (10)
   M4ISOPOSDAY,         // SQL_TIMESTAMP (11)
   TIPOALFA,            // SQL_VARm4char_t (12)
   TIPOALFA,            // SQL_LONGVARm4char_t (-1)
   TIPO_ERROR,          // SQL_BINARY (-2)
   TIPO_ERROR,          // SQL_VARBINARY (-3)
   TIPO_ERROR,          // SQL_LONGVARBINARY (-4)
   TIPONUMERO,          // SQL_BIGINT (-5)
   TIPONUMERO,          // SQL_TINYINT (-6)
   TIPO_ERROR           // SQL_BIT (-7)
};

m4int16_t CLM4FMT::_M4FMTConvertSQLToM4(m4int16_t a_iTipoSql, m4int16_t  *a_piTipoM4)
{  
	m4int16_t indice;      
   
	indice = (a_iTipoSql<0)? SQL_VARCHAR-a_iTipoSql : a_iTipoSql;
   
   
	return  (!indice || indice> (sizeof(s_TablaSQLToM4)/sizeof(int)))?
				TIPO_ERROR :
				((*a_piTipoM4 = s_TablaSQLToM4[indice])==TIPO_ERROR)?
					ERROR_TIPOSQL :
					EXITO;   
}

//-----------------------------------------------------------------------------
//   Devuelve el nombre del formato correspondiente al handle pasado
//-----------------------------------------------------------------------------

m4int16_t CLM4FMT::_M4FMTGetFormatName(m4int16_t a_iHandle, m4char_t* a_pcOut, m4int16_t a_iLonOut,
									   m4int16_t  *a_piLonEscrita)
{
	m4int16_t iError=0;   
	FMT_ClFormato  *pf;   
      
	if(!m_poTaskEnv->ClM4Fmt_g_bInitialize){
		_M4FMTInitialice();
	};

	pf=(FMT_ClFormato  *)g_M4CTGetElement(a_iHandle);
      
	if(pf != NULL){
		*a_piLonEscrita=pf->DameNombre(a_pcOut, a_iLonOut);
		iError=0;
	} else {
		iError=ERROR_INVALIDHANDLE;
	};

	return (iError)? iError : EXITO;
}
      
//--------------------------------------------------------------------------
//  Devuelve el Handle del formato que le pasamos como parametro
//--------------------------------------------------------------------------

m4int16_t CLM4FMT::_M4FMTGetFormatHandle(m4int16_t  *a_piHandle, m4char_t* a_pcIn)
{
	m4int16_t iResul=0;
	*a_piHandle=0;

	if(!m_poTaskEnv->ClM4Fmt_g_bInitialize){
		_M4FMTInitialice();
	};         
      
	FMT_ClContainer  *pac=g_GetAutoContainer();
     
	if(pac != NULL){     
		pac->ForEachElement( (m4bool_t (FMT_ClElement::*)(void  *, void  *)) &FMT_ClFormato::ComparaNombre, a_pcIn, a_piHandle);
            
        if(*a_piHandle){
			iResul=EXITO;
        } else {
             iResul = ERROR_NOENOMBRE;
		};
    } else {
		// ERROR_NOEXISTEARRAY;
    };

	return iResul;
}

//-----------------------------------------------------------------------------
//  Devuleve la descripcion de un error a partir de su código
//-----------------------------------------------------------------------------

m4int16_t CLM4FMT::_M4FMTGetErrorName(m4int16_t a_iError, m4char_t* a_pcErrorName,
									  m4int16_t a_iLonErrorName, m4int16_t  *a_iErrorNamel)
{               
	m4int16_t l;
	l = g_CargaCadena(-a_iError, a_pcErrorName, a_iLonErrorName);
   
	if(!l){
		l = g_CargaCadena(-ERROR_ERRORNOEXISTE, a_pcErrorName, a_iLonErrorName);
	};

	*a_iErrorNamel=l;   

	return EXITO;
}


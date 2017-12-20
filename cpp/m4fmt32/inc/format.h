//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             FORMAT.H
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

#ifndef __FORMAT_H
#define __FORMAT_H

#include "m4types.hpp"
#include "m4stl.hpp"
#include "rctables.h"
#include "cadena.h"
#include "contain.hpp"

#include "clstr.h"

/////////////////////////////////////////////////////////////////////////////////////


struct StComando;
struct StSuposiciones;
      
class FMT_ClFormato  : MIT_PUBLIC FMT_ClElement
{

MIT_PUBLIC:                                                  

//------ Constructores -------------------------------------------------------------

	FMT_ClFormato(m4char_t*);
	FMT_ClFormato(FMT_ClFormato   *a_pfFormatoOriginal);
   
	virtual ~FMT_ClFormato();
   
//------ Miembros ------------------------------------------------------------------

	//Nombre del formato (enviado desde la engine.dll)
	m4char_t* m_pcNombre;
   
   	//Link al PlugIn del formato actual. Cada instancia de la clase formato
	// que haga referencia a un idioma debe tener un link a un Plug-In
	StPlugInData* m_StPlugInInfo;

//------- Datos internos del formato -----------------------------------------------

	struct DatosCFormato
	{
		m4char_t iYear;
		m4char_t iMonth;
		m4char_t iWeekDay;
		m4char_t iDay;
		m4char_t iHour;
		m4char_t iMinute;
		m4char_t iSecond;
      
		m4char_t iAlig;
		m4char_t iCase;
            
		m4uint32_t iNoffsetstr;

		m4uint32_t iDecGrup;
		m4uint32_t iBlank;
      
		m4uint32_t iCompDec;
		m4uint32_t iCompInt;
		m4uint32_t iCurrDec;
		m4uint32_t iLength;      
      
		m4uint32_t Flags;			//D=0

		m4uint32_t iWordMode;	    //D=0
		m4uint32_t iPrefixeMode;	//D=0
		m4uint32_t iSeparatorMode;  //D=0
      
		m4uint32_t iCountry;
            
		m4uint32_t nCadenasSubst;
	
		// initializes
		DatosCFormato () {
		 iYear  = 0;
		 iMonth  = 0;
		 iWeekDay  = 0;
		 iDay  = 0;
		 iHour  = 0;
		 iMinute  = 0;
		 iSecond  = 0;
      
		 iAlig  = 0;
		 iCase  = 0;
            
		 iNoffsetstr  = 0;

		 iDecGrup  = 0;
		 iBlank  = 0;
      
		 iCompDec  = 0;
		 iCompInt  = 0;
		 iCurrDec  = 0;
		 iLength  = 0;      
      
		 Flags  = 0;			//D=0

		 iWordMode  = 0;	    //D=0
		 iPrefixeMode  = 0;	//D=0
		 iSeparatorMode  = 0;  //D=0
      
		 iCountry  = 0;
            
		 nCadenasSubst  = 0;
		}

	} D;      

	struct StDatosFormatoCastellano
	{
		m4uint32_t Flags;

		// initializes
		StDatosFormatoCastellano () {
			Flags = 0;
		}

	} DC; 
   
	ClCadena sThousand;
	ClCadena sDecimal;
	ClCadena sCharsFill; 
	ClCadena sCurrency;
   
	ClCadena sMaskDate;    
	ClCadena sMaskTimeStamp;  
	ClCadena sMaskTime24;
	ClCadena sMaskTime12;      
	ClCadena sMaskCurPos;
	ClCadena sMaskCurNeg;
	ClCadena sMaskNeg; 
   
	ClCadena sPlusInfDate;
	ClCadena sMinusInfDate;

	typedef map<ClMIT_Str, m4uint32_t, less<ClMIT_Str> > ExtList_t;
	ExtList_t ExtWordList_t, ExtPrefList_t, ExtSepList_t;
      
	ClCadena* m_spSubst;   

//---------------- Metodos -------------------------------------------------

	void      IniciaCadenaEntrada(ClCadena   *pCadIn, m4char_t* a_pcIn);
   
	m4int16_t Desformatea(ClCadena   *a_pcad, m4int16_t a_tipo);
	m4int16_t DesformateaIdioma(ClCadena   *a_pcad, m4int16_t a_tipo);  
	m4int16_t DesformateaLight(ClCadena   *a_pCad, m4int16_t a_tipo);
	m4int16_t DesformateaIdiomaLight(ClCadena   *a_pCad, m4int16_t a_tipo);
	m4int16_t DesformateaFechaHoraLight(ClCadena   *a_pCad, ClCadena   *a_pMask);   
	m4int16_t DesformateaNumero2Light(ClCadena   *a_pNum);
   
	m4int16_t Formatea(ClCadena   *a_pcad, m4int16_t a_tipo);      
	m4int16_t FormateaIdioma(ClCadena *a_pCad, m4int16_t a_tipo);
    
	m4int16_t CaseFormat(m4char_t *ai_pcCadena, m4int16_t a_tipo);
	m4int16_t SentenceCase(m4char_t *ai_pcCadena, m4int16_t a_tipo);


	m4int16_t  SustituyePor(FMT_ClFormato   *a_pfFormatoNuevo);   
	m4uint32_t DameIdioma(void){ return D.iCountry; };      // inline
	void	   IniciaNombre(m4char_t* a_pcNombre);
	m4char_t*  DameNombre(void){ return m_pcNombre; };
	m4uint32_t DameBlank(void){ return D.iBlank; };
	m4int16_t  DameNombre(m4char_t* a_pcBuffer, m4int16_t a_iLonMax);
	m4char_t*  DamePlusInfDate(void){return sPlusInfDate.DirIni(); };
	m4char_t*  DameMinusInfDate(void){return sMinusInfDate.DirIni(); };

	m4char_t   GetCase() { return  D.iCase; };
	m4uint32_t GetWordMode(void) {return D.iWordMode; };
	m4uint32_t GetPrefixeMode(void) {return D.iPrefixeMode; };	
	m4uint32_t GetSeparatorMode(void) {return D.iSeparatorMode; };

	m4int16_t CargaCadena(m4uint32_t id, m4char_t* buf, m4int16_t lbuf);             
	m4char_t  BuscaCadenaMes(m4char_t* a_pc, m4int16_t a_i, m4int16_t a_iN);
	m4char_t  BuscaCadenaSemana(m4char_t* a_pc, m4int16_t a_i, m4int16_t a_iN);

	m4int16_t SustituyeAntes(ClCadena   **);
	m4int16_t SustituyeDespues(ClCadena   **);
      
	m4int16_t CambiaPropiedad(m4char_t* nombre, m4char_t* valor);
   
//--------- Funciones de comando ------------------------------------------------

    m4int16_t co_iFAlig(m4char_t* a_pcValor);
    m4int16_t co_iFLength(m4char_t* a_pcValor);
    m4int16_t co_iFBeginstr(m4char_t* a_pcValor);
    m4int16_t co_iFNoffsetstr(m4char_t* a_pcValor);
    m4int16_t co_sFCharsFill(m4char_t* a_pcValor);
    m4int16_t co_sFMaskCurPos(m4char_t* a_pcValor);
    m4int16_t co_sFMaskCurNeg(m4char_t* a_pcValor);
    m4int16_t co_sFCurrency(m4char_t* a_pcValor);
    m4int16_t co_iFCase(m4char_t* a_pcValor);    
    m4int16_t co_iFSubst(m4char_t* a_pcValor);
    m4int16_t co_sFSubst(m4char_t* a_pcValor);
    m4int16_t co_sFDay(m4char_t* a_pcValor);
    m4int16_t co_sFMonth(m4char_t* a_pcValor);
    m4int16_t co_sFYear(m4char_t* a_pcValor);
    m4int16_t co_sFWeekDay(m4char_t* a_pcValor);
    m4int16_t co_sFHour(m4char_t* a_pcValor);
    m4int16_t co_sFMinute(m4char_t* a_pcValor);
    m4int16_t co_sFSecond(m4char_t* a_pcValor);
    m4int16_t co_sFMaskDate(m4char_t* a_pcValor);
	m4int16_t co_sFPlusInfDate(m4char_t* a_pcValor);
	m4int16_t co_sFMinusInfDate(m4char_t* a_pcValor);
    m4int16_t co_sFThousand(m4char_t* a_pcValor);
    m4int16_t co_sFDecimal(m4char_t* a_pcValor);
    m4int16_t co_iFCompDec(m4char_t* a_pcValor);
    m4int16_t co_iFCompInt(m4char_t* a_pcValor);
    m4int16_t co_iFTrunc(m4char_t* a_pcValor);
    m4int16_t co_iFLeadZero(m4char_t* a_pcValor);
    m4int16_t co_sFMaskNeg(m4char_t* a_pcValor);
    m4int16_t co_iFStyle(m4char_t* a_pcValor);
    m4int16_t co_iFLeftZero(m4char_t* a_pcValor);
    m4int16_t co_iFCur(m4char_t* a_pcValor);
	m4int16_t co_iFDecGrup(m4char_t* a_pcValor);
	m4int16_t co_iFBlank(m4char_t* a_pcValor);
    m4int16_t co_sFMaskTime12(m4char_t* a_pcValor);
    m4int16_t co_sFMaskTime24(m4char_t* a_pcValor);
    m4int16_t co_sFMaskTimeStamp(m4char_t* a_pcValor);
    m4int16_t co_iFCapar(m4char_t* a_pcValor);

	m4int16_t co_iFWordToKeepSubtitute(m4char_t* a_pcValor);
	m4int16_t co_iFSeparatorMode(m4char_t* a_pcValor);
	m4int16_t co_iFPrefixeMode(m4char_t* a_pcValor);

	m4int16_t co_sFPrefixes(m4char_t* a_pcValor);
	m4int16_t co_sFSeparators(m4char_t* a_pcValor);
	m4int16_t co_sFWordsToSubtitute(m4char_t* a_pcValor);

//----- Comandos propios del castellano --------------------------------------

	m4int16_t co_iFAcentLess(m4char_t* a_pcValor);
	m4int16_t co_iFIntKind(m4char_t* a_pcValor);
	m4int16_t co_iFDecKind(m4char_t* a_pcValor);
	m4int16_t co_sFValueInt(m4char_t* a_pcValor);
	m4int16_t co_sFValueDec(m4char_t* a_pcValor);
	
//------ Funciones de desformateo ---------------------------------------------
    
    m4int16_t LeeAnio(m4int16_t, m4int16_t);
    m4int16_t LeeMes(m4int16_t, m4int16_t);
    m4int16_t LeeDiaSemana(m4int16_t, m4int16_t);
    m4int16_t LeeDia(m4int16_t, m4int16_t);
    m4int16_t LeeHora(m4int16_t, m4int16_t);
    m4int16_t LeeMinuto(m4int16_t, m4int16_t);
    m4int16_t LeeSegundo(m4int16_t, m4int16_t);
    m4int16_t LeeMoneda(m4int16_t, m4int16_t);
    m4int16_t LeeNumero(m4int16_t, m4int16_t);
                            
//-------- Funciones de formateo -----------------------------------------------------
    
    m4int16_t EscribeAnio(ClCadena   *a_pcad);
    m4int16_t EscribeMes(ClCadena   *a_pcad);
    m4int16_t EscribeDiaSemana(ClCadena   *a_pcad);
    m4int16_t EscribeDia(ClCadena   *a_pcad);
    m4int16_t EscribeHora(ClCadena   *a_pcad);
    m4int16_t EscribeMinuto(ClCadena   *a_pcad);
    m4int16_t EscribeSegundo(ClCadena   *a_pcad);
    m4int16_t EscribeMoneda(ClCadena   *a_pcad);
    m4int16_t EscribeNumero(ClCadena   *a_pcad);
    
	m4bool_t ComparaNombre(void   *, void   *);
    
//--------- StartUp de los formateos base ----------------------------------------------

	m4int16_t Init(m4char_t*);
   
	// Bug 141465. Se implementa la funcionalidad de Regional Settings en UNIX
	void CogeFormatoWinIni(void);

	void VerifyWindowsSettings(void);
	UINT ReadWindowsSettingAsInt(LPCTSTR lpKeyName, INT nDefault);
	DWORD ReadWindowsSettingAsString(LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize);

	void AuxWinIni(m4char_t*, m4char_t   *, char,  m4char_t   *);
	void CogeFormatoIso(void);
	void CogeFormatoNulo(void);
    

	m4int16_t FormateaMascara(ClCadena   *a_pcad, ClCadena   *a_pmask);   
    
	m4int16_t DesformateaFechaHora(ClCadena   *a_pcad, ClCadena   *a_pmask);
	m4int16_t DesformateaNumero1(ClCadena   *a_pNum, ClCadena   *a_pCad, ClCadena   *a_pMask);
	m4int16_t DesformateaNumero2(ClCadena   *a_pNum);
	m4int16_t DesfRecur(void);
   
	void	  RellenaIzq(ClCadena   *a_pCad, m4int16_t a_iL);
	void	  RellenaDer(ClCadena   *a_pCad, m4int16_t a_iL);
	m4int16_t DesrellenaIzq(ClCadena   *a_pCad, m4char_t*, m4int16_t);
	m4int16_t DesrellenaDer(ClCadena   *a_pCad, m4char_t*, m4int16_t);
	m4int16_t QuitaCerosFinal(m4char_t* p, m4int16_t l);   
   
	m4int16_t CambiaFlag(m4char_t* a_pcValor, m4uint32_t    *a_plF, m4uint32_t  a_flag);
	m4int16_t CambiaEntero(m4char_t* a_pcValor, m4uint32_t   *a_pi);
	m4int16_t CuentaLetras(m4char_t* dir, m4char_t c);	
   
	m4int16_t BuscaPropiedad(m4char_t* a_pcNombre, m4char_t* a_pcValor, const StComando   *a_pT);
   
	m4int16_t Sustituye(ClCadena   **);
	m4int16_t DesSustituye(void);
   
	void	  Normaliza(ClCadena   *pCadena); 

	m4int16_t SupRecur(StSuposiciones   *pSup);
	m4int16_t Verifica(void);

MIT_PRIVATE:
	// Bug 0107547. Originalmente s_pcIndices estaba definida como un array estatico.
	// Esto provocaba que se compartiera entre todas las instancias de la clase.
	// En multithreading, por esta razón no se aplican bien los formatos porque cuando
	// un thread pone apuntando s_pcIndices a s_pcIndicesCurrNume para formatear un número,
	// viene otro thread y pone s_pcIndices apuntando a s_pcIndicesFechHora. Cuando el primer
	// thread quiere recuperar lo que busca de este array no lo encuentra porque se lo 
	// han cambiado. Esto provoca que se dé un error de formato incorrecto.
	const m4char_t*  s_pcIndices;
};

/////////////////////////////////////////////////////////////////////////////////////////

//Estructura que gestiona la tabla de comandos del formato segun el nombre

struct StComando
{
	m4char_t* Nombre;
	m4int16_t (FMT_ClFormato::*Comando)(m4char_t*);
};
      

#endif

///////////////////////////////////////////////////////////////////////////////


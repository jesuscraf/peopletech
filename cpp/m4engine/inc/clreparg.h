//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             clreparg.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:17/07/1997
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//    Traduce la cadena de parametros de report
//
//
//==============================================================================

#ifndef __CLREPARG__HPP__
#define __CLREPARG__HPP__

#include "clargs.h"
#include "clrepreq.h"


class ClENG_ReportArgs : public ClMIT_Args , public ClENG_ReportRequest {
	
MIT_PRIVATE:

	StMIT_Inspector* m_poInsp;	//Simple link

	enum ARGS_INDEX { 
		ID_REPORT=1, EXECUTION_TYPE=2, DVC_TYPE=3, //Para todos Dvc: Id report y device
		OPTIONAL_ARGS_BEG=4 } ;

	enum PACK_CHANNELS { PACK_DESIGN, PACK_DATA } ;

	static const ClMIT_Args::EnumTrans_t m_EnumBin[] ; 
	static const ClMIT_Args::EnumTrans_t m_EnumDPI[] ; 
	static const ClMIT_Args::EnumTrans_t m_EnumDvc[] ; 
	static const ClMIT_Args::EnumTrans_t m_EnumDetail[] ; 
	static const ClMIT_Args::EnumTrans_t m_EnumGDebug[] ; 
	static const ClMIT_Args::EnumTrans_t m_EnumBreak[] ; 
	static const ClMIT_Args::EnumTrans_t m_EnumPack[] ;
	static const ClMIT_Args::EnumTrans_t m_EnumAutoload[] ;
	static const ClMIT_Args::EnumTrans_t m_EnumDuplex[] ;
	static const ClMIT_Args::EnumTrans_t m_EnumTumble[] ;
	static const ClMIT_Args::EnumTrans_t m_EnumHTMLNavigation[] ;
	static const ClMIT_Args::EnumTrans_t m_EnumHTMLMenu[] ;
	static const ClMIT_Args::EnumTrans_t m_EnumMoveTraceMode[] ;
	static const ClMIT_Args::EnumTrans_t m_EnumIdDesc[] ;
	static const ClMIT_Args::EnumTrans_t m_EnumDeclareExtFile[] ;

	static const m4char_t Separator ;
	static const m4char_t *const ArgSeparator ;

MIT_PUBLIC:

	ClENG_ReportArgs() ;
	virtual ~ClENG_ReportArgs() { ClENG_ReportArgs::End() ;}
	virtual size_t GetSizeof() { return sizeof(ClENG_ReportArgs) ; }
	virtual void End() ;

	// Traduce todo, menos los /BIN que no son de defecto, aunque si chequea que los 
	//	unicos argumentos parametros que no se traduzcan sean los /BIN.
	// Si se pasan argumentos sintacticamente incorectos, se produce un error.

	m4bool_t Init( const m4char_t **ai_pcArgV, m4int16_t ai_iArgC,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;

	// Como la anterior, pero los argumentos estan en una cadena

	m4bool_t Init( const m4char_t *ai_pcCommand,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;


	m4bool_t Init( ClENG_ReportArgs *ai_poFrom,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;

	//Deja en las ClMIT_Str unas cadenas con los parametros fijos y en otra los variables.
	//Util, entre otras cosas, para cadenas de trace y para reinicializaciones Init que 
	//	concatenan parametros. 
	//Mete todos los parametros entre ClosingChars de defecto, para evitar problemas con 
	//blancos.

	void GetInitString(ClMIT_Str *ai_poFixedPar, ClMIT_Str *ai_poVblePar) ;

	// Como la anterior, pero debe usarse sobre un objeto ya inicializado. La recarga 
	//	respeta los valores no parametrizables via ai_pcCommand que ya se inicializaron
	//	en la primera carga.
	// El comando en este caso se AÑADE ANTES a la linea de comandos que ya existia, por
	//	lo que tiene precedencia la primera peticion.

	m4bool_t ReLoad( const m4char_t *ai_pcCommand,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;

	// Rango de paginas en /BIN:n, de 1 a n.
	// Si no encuentra el parametro /BIN:Page, pero este no tiene o usa un  valor bin 
	//	inadecuado, devuelve el valor por defecto y genera un error local.

	virtual ClENG_PagePrinter::PAPER_SOURCE GetBin( m4int16_t ai_iPage ) ;
	virtual m4int32_t GetBinPcl(m4int16_t ai_iPage);

MIT_PRIVATE:

	// Para uso comun en los Init

	m4bool_t Load(ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;

	//Deja en ao_poStr un path concatenando el directirio de trabajo de reports con ai_pcFile.
	void SetOutputPath(const m4char_t *ai_pcFile, ClMIT_Str *ao_poStr) ;

	m4bool_t CheckOutputDir(const m4char_t *ai_pcFile, m4bool_t ai_bRelativeDir, ClMIT_Str * ao_oOutputDir);
	
} ;

#endif

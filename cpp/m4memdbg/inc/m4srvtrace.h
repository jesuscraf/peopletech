//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             m4srvtrace.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:11-Sep-97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//		Debugger de memoria
//
//==============================================================================

#ifndef __M4_SRV_TRACE__H__
#define __M4_SRV_TRACE__H__ 

#include <stdio.h>
#include "m4memdbg_dll.hpp"
#include "m4ipcs.hpp"
#include "basiclog.hpp"


//Para que las macros funcionen, debe haber definido localmente en el CPP la macro
//#define M4_SRV_TRACE_COMP_ID "DPCH"
//Donde el texto representa al modulo que lanza el mensaje.

//Los errores y warnings no tienen level: salen siempre, y a LOG y cout.
//
//-M4LogCode. Codigo numerico para la M4Log. Mapea el texto del mensaje. Un 0 se trata como un
//	codigo de error interno, y NO se manda a la Log.
//-MsgText tal cual esta en el logmsg.ini. NO lo usa la M4Log, pero si  nosotros para sacarlo
//	por pantalla. Admiten %0:s, %1:s, etc, que serán luego mapeados conforme a sus parametros.
//-Params. Bloque de parametros en el formato << Param << Param etc. Es utilizado tanto por la
//	M4Log como para la salida a pantalla.

#define M4_SRV_ERROR( M4LogCode, MsgText, Params ) \
	{if (g_bM4SwSrvM4LogOn && M4LogCode) { BL_ERRORF(M4LogCode, Params) ; } ; \
		if (g_bM4SwSrvCoutOn ) \
		g_oM4SrvTrace << M4_SRV_MSG_B << M4_SRV_ERROR << M4_SRV_TRACE_COMP_ID << MsgText << Params << M4_SRV_MSG_E ; }

#define M4_SRV_WARNING(M4LogCode, MsgText, Params) \
	{if (g_bM4SwSrvM4LogOn  && M4LogCode) { BL_WARNINGF(M4LogCode, Params) ; } ; \
		if (g_bM4SwSrvCoutOn ) \
		g_oM4SrvTrace << M4_SRV_MSG_B << M4_SRV_WARNING << M4_SRV_TRACE_COMP_ID << MsgText << Params << M4_SRV_MSG_E ; }


//Versiones sin parametros

#define M4_SRV_ERROR_N( M4LogCode, MsgText ) \
	{if (g_bM4SwSrvM4LogOn  && M4LogCode ) { BL_ERROR(M4LogCode) ; } ; \
		if (g_bM4SwSrvCoutOn ) \
		g_oM4SrvTrace << M4_SRV_MSG_B << M4_SRV_ERROR << M4_SRV_TRACE_COMP_ID << MsgText << M4_SRV_MSG_E ; }

#define M4_SRV_WARNING_N( M4LogCode, MsgText ) \
	{if (g_bM4SwSrvM4LogOn && M4LogCode ) { BL_WARNING(M4LogCode) ; } ; \
	if (g_bM4SwSrvCoutOn ) \
		g_oM4SrvTrace << M4_SRV_MSG_B << M4_SRV_WARNING << M4_SRV_TRACE_COMP_ID << MsgText << M4_SRV_MSG_E ;}

//Los mensajes de debug SI tienen level, nos interesará sacarlos o no en según que ocasiones.
//
//Por ahora, salen solo a cout.
//
//Vamos a usar 3 levels:
//	-NIVEL presentacion. Siempre salen cara al usuario final. Siguen la ejecucion de forma general.
//		-Nivel 1. GENERAL.
//		-Nivel 2. DETAILED. 
//	-NIVEL 3. DEBUG. Normalmente no salen, solo los activamos cuando queremos depurar. 
//	-Opcionalmente, puede usarse el ejuego de macros DEBUG_X para poner levels de debug
//		privados. La mcro expande el LevelX pasado a 3+LevelX, por lo que el LevelX 0 coincide
//		con el nivel nomral de Debug.
//
//Por ejemplo, muchos mensajes de dispatcher seran de nivel presentacion, al ser un componente
//	final. Es decir, seguro que queremos ver siempre (GENERAL) cuando un client pide una 
//	conexion, cuando se desconecta, etc. Es posible que otros detalles como la recepcion de 
//	pulsos "estoy vivo" del server solo los queramos a un nivel DETAILED, pero no siempre para
//	no llenearnos la traza con estos mensajes.
//Sin embargo, la m4communication, al ser una componente de utilidad, tendrá casi todos sus
//	mensajes a nivel DEBUG. No queremos que al arrancar el dipatcher salgan un monton de 
//	mensajes con informacion detallada de comunicaciones. Podrá haber alguno de nivel más bien 
//	DETAILED, como el enganche a un puerto TCP, pero serán los menos.

#define M4_SRV_MSG_AUX( MsgText, Params, Level ) \
	{if (g_bM4SwSrvCoutOn && g_oM4SrvTrace.GetCurrentLevel()>=Level ) \
		g_oM4SrvTrace << M4_SRV_MSG_B << M4_SRV_TRACE_COMP_ID << MsgText << Params << M4_SRV_MSG_E ;}

#define M4_SRV_GENERAL(		MsgText, Params )	M4_SRV_MSG_AUX(MsgText, Params, 1) 
#define M4_SRV_DETAILED(	MsgText, Params )	M4_SRV_MSG_AUX(MsgText, Params, 2)
#define M4_SRV_DEBUG(		MsgText, Params )	M4_SRV_MSG_AUX(MsgText, Params, 3)
#define M4_SRV_DEBUG_X(		LevelX, MsgText, Params )	M4_SRV_MSG_AUX(MsgText, Params, 3+(LevelX))

//Version sin parametros

#define M4_SRV_MSG_AUX_N( MsgText, Level ) \
	{if (g_bM4SwSrvCoutOn && g_oM4SrvTrace.GetCurrentLevel()>=Level ) \
		g_oM4SrvTrace << M4_SRV_MSG_B << M4_SRV_TRACE_COMP_ID << MsgText << M4_SRV_MSG_E ; }

#define M4_SRV_GENERAL_N(	MsgText )	M4_SRV_MSG_AUX_N( MsgText, 1 )
#define M4_SRV_DETAILED_N(	MsgText )	M4_SRV_MSG_AUX_N( MsgText, 2 )
#define M4_SRV_DEBUG_N(		MsgText )	M4_SRV_MSG_AUX_N( MsgText, 3 )
#define M4_SRV_DEBUG_X_N(	LevelX,	MsgText )	M4_SRV_MSG_AUX_N( MsgText, 3+(LevelX) )

//Además permitiremos trazas que marquen I/O de funciones, o bloques. Estas no serán ni de 
//	error ni warning, si no siempre de traza. Producen indentaciones. El formato es, para
//	funciones:
//
//	void F() {
//		M4_SRV_GENERAL_BLOCK(  Blk, ..... ) ;	
//		...
//	} ;
//
//Y para bloques:
//
//	{
//		M4_SRV_GENERAL_BLOCK( Blk, ..... ) ;	
//		....
//	} ;
//Si tenemos problemas la hacer bloques, por desparecer vbles internas de bloque, tambien
// podemos hacer:
//	
//  ...
//	M4_SRV_GENERAL_BLOCK( Blk, ..... ) ;	
//	....
//  Blk.End() ;
//  ....

#define M4_SRV_AUX_BLOCK(	VbleName, MsgText, Params, Level ) \
	ClM4_SrvTraceBlock VbleName(Level) ; \
	if (g_bM4SwSrvCoutOn && g_oM4SrvTrace.GetCurrentLevel()>=Level ) \
		g_oM4SrvTrace << M4_SRV_MSG_B << M4_SRV_TRACE_COMP_ID << MsgText << Params << M4_SRV_INDENT << M4_SRV_MSG_E ;

#define M4_SRV_GENERAL_BLOCK(	VbleName, MsgText, Params )	M4_SRV_AUX_BLOCK( VbleName, MsgText, Params, 1 )
#define M4_SRV_DETAILED_BLOCK(  VbleName, MsgText, Params )	M4_SRV_AUX_BLOCK( VbleName, MsgText, Params, 2 )
#define M4_SRV_DEBUG_BLOCK(		VbleName, MsgText, Params )	M4_SRV_AUX_BLOCK( VbleName, MsgText, Params, 3 )
#define M4_SRV_DEBUG_BLOCK_X(	LevelX, VbleName, MsgText, Params )	M4_SRV_AUX_BLOCK( VbleName, MsgText, Params, 3+(LevelX) )


//Versiones sin parametros 

#define M4_SRV_AUX_BLOCK_N(	VbleName, MsgText, Level ) \
	ClM4_SrvTraceBlock VbleName(Level) ; \
	if (g_bM4SwSrvCoutOn && g_oM4SrvTrace.GetCurrentLevel()>=Level ) \
		g_oM4SrvTrace << M4_SRV_MSG_B << M4_SRV_TRACE_COMP_ID << MsgText << M4_SRV_INDENT << M4_SRV_MSG_E ;

#define M4_SRV_GENERAL_BLOCK_N(		VbleName, MsgText )	M4_SRV_AUX_BLOCK_N( VbleName, MsgText, 1 )
#define M4_SRV_DETAILED_BLOCK_N(	VbleName, MsgText )	M4_SRV_AUX_BLOCK_N( VbleName, MsgText, 2 )
#define M4_SRV_DEBUG_BLOCK_N(		VbleName, MsgText )	M4_SRV_AUX_BLOCK_N( VbleName, MsgText, 3 )
#define M4_SRV_DEBUG_BLOCK_X_N(		LevelX, VbleName, MsgText )	M4_SRV_AUX_BLOCK_N( VbleName, MsgText, 3+(LevelX) )

//Todo el sistema de traza se activa y desactiva con estos flags, uno es para sacar a
// cout (objeto ClM4_SrvTrace) y otro para la M4Log. 
//Por defecto, a cout desactivado y a M4Log activado

extern M4_DECL_M4MEMDBG m4bool_t g_bM4SwSrvCoutOn ; 
extern M4_DECL_M4MEMDBG m4bool_t g_bM4SwSrvM4LogOn ; 

//===============================================================================>

//NO USAR ESTAS CLASES DIRECTAMENTE, USAR LAS MACROS !!!
//Usar solo g_oM4SrvTrace.SetCurrentLevel(Level)

//===============================================================================>ClM4_SrvTrace

enum M4_SRV_TRACE_MANIP {	//Manipuladores <<
	M4_SRV_MSG_B,	//Begin Msg. Despues viene el texto del msg. Pone Mutex	
	M4_SRV_MSG_E,	//Marca fin de msg. Libera Mutex.
	M4_SRV_ERROR,
	M4_SRV_WARNING,
	M4_SRV_TRACE,
	M4_SRV_INDENT 
}  ;	

class M4_DECL_M4MEMDBG ClM4_SrvTrace {

private:

	ClMutex m_oMutex ;	//Regiones criticas 

	enum { MAX_PARAMS=30,SIZE_PARAM=512, SIZE_STR_OUT=1024*4 } ;

	m4char_t m_pcParams[MAX_PARAMS][SIZE_PARAM] ;	//Param 0, el CompId, 1 el msg, resto param.
	m4char_t m_pcStrOut[SIZE_STR_OUT] ;

	m4int16_t  m_iIndentCount ;
	m4int16_t  m_iParamCount ;
	m4bool_t   m_bSwIndent ;

	m4int16_t  m_iCurrentLevel ;	//Si 0, no sale nada. Defecto 1.

	M4_SRV_TRACE_MANIP  m_MsgType ;	//Guarda solo Error/Warning/Debug. Def, Debug

	FILE *m_poFile ;	//Si no NULL, saca a este fichero. Defecto NULL.
	m4bool_t m_bFlush ;	//SI TRUE, al sacar a fichero se hace un flush por mensaje. Def FALSE.

public:

	ClM4_SrvTrace() ;	
	virtual ~ClM4_SrvTrace() ;	

	//Intenta abrir fichero auxiliar de volcado de traza. Da TRUE  si tiene exito.
	//Si se pasa un NULL, se desactiva el fichero.
	//Cualuier llamada a esta funcion provoca un close si hay un fichero ya activado.
	//Protegido.

	m4bool_t SetTraceFile( const m4char_t *ai_pcFile ) ;

	//Tipo basico, no mutex. 

	void SetFlush(m4bool_t ai_bSw) { m_bFlush = ai_bSw ;  }
	m4bool_t IsFlush() { return m_bFlush ; } 

	//Tipo basico, no mutex. 

	m4int16_t GetCurrentLevel() { return m_iCurrentLevel ; } 
	void	  SetCurrentLevel(m4int16_t ai_iLevel) { m_iCurrentLevel=ai_iLevel ; } 

	//Tipo basico, no mutex. 

	void Indent() { ++m_iIndentCount ; }

	//Tipo basico, no mutex. 

	void UnIndent() { if (m_iIndentCount) --m_iIndentCount ; }

	//Msg fin bloque. Protegido

	void EndBlock() ;

	//Manipuladores y parametro. Lock en M4_SRV_MSG_B, Unlock en M4_SRV_MSG_E

	ClM4_SrvTrace &operator << (M4_SRV_TRACE_MANIP ai_Manip) ;
	ClM4_SrvTrace &operator << (m4int_t ai_Par) ;
#ifdef _WIN64
	ClM4_SrvTrace &operator << (uintptr_t ai_Par);
#endif
	ClM4_SrvTrace &operator << (m4uint_t ai_Par);
	ClM4_SrvTrace &operator << (m4int16_t ai_Par) ;
	ClM4_SrvTrace &operator << (m4uint16_t ai_Par) ;
	ClM4_SrvTrace &operator << (m4int32_t ai_Par) ;
	ClM4_SrvTrace &operator << (m4uint32_t ai_Par) ;
	ClM4_SrvTrace &operator << (m4int64_t ai_Par);
	ClM4_SrvTrace &operator << (m4double_t ai_Par) ;
	ClM4_SrvTrace &operator << (m4bool_t ai_Par) ;
	ClM4_SrvTrace &operator << (m4char_t ai_Par) ;			
	ClM4_SrvTrace &operator << (const m4char_t *ai_Par) ;	//Admite NULL, convierte a cadena "NULL"

	static void CreateRecursivePath(const m4char_t *ai_pcPath) ;

private:

	//Copia a To, si sobrepasa, no sobreescribe, y mete 3 puntos al final.
	//Devuelven Len total de la cadena

	static void     SafeStrCpy( m4char_t *ai_pcTo, size_t ai_SizeTo, const m4char_t *ai_pcFrom ) ;
	static size_t   SafeStrNCpy( m4char_t *ai_pcTo, size_t ai_SizeTo, const m4char_t *ai_pcFrom, size_t ai_Count ) ;
	static void     SafeStrCat( m4char_t *ai_pcTo, size_t ai_SizeTo, const m4char_t *ai_pcFrom ) ;
	static size_t   SafeStrNCat( m4char_t *ai_pcTo, size_t ai_SizeTo, const m4char_t *ai_pcFrom, size_t ai_Count ) ;

	enum STYLE { NO_STYLE, BLOCK_ENTRY, BLOCK_EXIT  } ;

	void AddIndent(STYLE ai_Style) ;	//Resetea m_pcStrOut con cadena con blancos por indent y el Id de comp.

	void CatPrintF() ;

	void WrapOutput(m4char_t *ai_pcStr, STYLE ai_Style ) ;

} ;

extern M4_DECL_M4MEMDBG ClM4_SrvTrace g_oM4SrvTrace ; 

//==========================================================================>ClM4_SrvTraceBlock

class M4_DECL_M4MEMDBG ClM4_SrvTraceBlock {

	m4int16_t m_iLevel ;	//-1, ya se ejecutó End
public:

	ClM4_SrvTraceBlock(m4int16_t ai_iLevel)		{ m_iLevel=ai_iLevel ; } 

	virtual ~ClM4_SrvTraceBlock() { End() ; } 				
	void End() ;								//Si debe, saca msg y desindenta.
} ;


#endif

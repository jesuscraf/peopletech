//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             clargs.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:20/10/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//    Gestiona cadenas de argumentos
//
//
//==============================================================================
#ifndef __CLARGS__H__
#define __CLARGS__H__

#include "clstr.h"
#include "mit.h"

class M4_DECL_M4MIENV ClMIT_Args {

	// Esta clase manipula Args (argumentos). Un argumento es una cadena que puede
	//	contener subcadenas separadas por el caracter Separator (o el nulo de fin de
	//	cadena).
MIT_PRIVATE:
	StMIT_Inspector *m_poInsp ;	//Simple link

	m4char_t m_cSeparator ;	//Caracter que se usa como separador entre valores de un 
							//	mismo argumento.
	ClMIT_Str m_oArgSep ;	//Conjunto de caracteres que se usan como separadores entre argumentos.
	ClMIT_Str * m_poArg ;
	m4int16_t m_iArgCount ;		//Numero de elementos del array Arg

MIT_PUBLIC:

	typedef struct { 
		m4char_t *pcStr ;	//Cadena a comparar. Comparaciones NO son case-sensitive.
							//El ultimo elemento debe tener esto a NULL: es el flag de fin
		m4int16_t iValue ;	//Valor enum
	} EnumTrans_t ;	//Traducciones texto a enum

MIT_PUBLIC:

	ClMIT_Args() ;
	virtual ~ClMIT_Args() { ClMIT_Args::End() ;}
	virtual size_t GetSizeof() { return sizeof(ClMIT_Args) ; }
	virtual void End() ;

	// Si se calcula ArgCount=0, Arg se pone a NULL, y cualquier llamada a GetArg fallara.
	// A partir de Command, se va construyendo el array de argumentos. Para ello, se usa
	//	como separadores entre argumento cualquier caracter de ArgSep. Cualquier subcadena
	//	que empiece por el primer caracter de ClosingChar y acabe por el segundo caracter de
	//	ClosingChar, es tratado como un argumento, aunque contenga caracteres de ArgSep. 
	//	Ejemplo:Una cadena <Arg:File A> genera los argumentos <Arg:File> y <A>. Sin embargo,
	//	una cadena <"Arg:File A"> genera el argumento <Arg:File A>.
	// La segunda version solo tiene en cuenta los szNChar primeros de la cadena. 
	// ai_cSeparator no puede ser el caracter fin de cadena. Si lo es, error inmmediato. Si
	//	no se quiere separar por subargumentos, puede usarse el enum NO_SUBARG.

	enum { NO_SUBARG=0xFF } ;

	//Caracteres defecto de apertura (0) y cierre (1). Ahora mismo ##
	//cadena ASCIIZ por comodidad.
	static const m4char_t m_pcClosingChar[3] ;	

	void Init( const m4char_t *ai_pcCommand, 
		m4char_t ai_cSeparator='.', const m4char_t * ArgSep=" \t", 
		const m4char_t * ClosingChar = m_pcClosingChar ) ;
	void Init( const m4char_t *ai_pcCommand, size_t ai_szNChar, 
		m4char_t ai_cSeparator='.', const m4char_t * ArgSep=" \t", 
		const m4char_t * ClosingChar = m_pcClosingChar  ) ;

	// Como el constructor anterior, solo que el array se da y se copia directamente.
	
	void Init( const m4char_t ** ai_pcArgV, m4int16_t ai_iArgC, 
		m4char_t ai_cSeparator='.', const m4char_t * ArgSep=" \t" ) ;

	void GetArgSep(ClMIT_Str *ai_poStr) { ai_poStr->StrCpy(m_oArgSep); } 
	m4char_t GetSubArgSep() {return m_cSeparator;}

	// Puntero de uso inmediato, contenido del item correspondiente del array. Se testean
	//	rangos.
	// El resto de metodos operan sobre una cadena, porque asi estos metodos son validos
	//	tanto para cadenas de ArgV, como para cualquier cadena.

	const m4char_t * GetArg(m4int16_t ai_iArg) ;
	
	m4int16_t GetArgCount() { return m_iArgCount ; }
	m4int16_t GetSubArgCount(m4int16_t ai_iArg) ;


	static const m4char_t GetDefaultOpenChar() { return m_pcClosingChar[0]; } 
	static const m4char_t GetDefaultCloseChar() { return m_pcClosingChar[1]; } 
	static const m4char_t * const GetDefaultClosingString() { return m_pcClosingChar; } 

	// Devuelve la posicion de comienzo de la cadena y su tamaño (que puede ser 0). La
	//	direccion devuelta apunta a una posicion dentro de la cadena ai_pcArg, por lo que
	//	el contenido al que se apunta no deberia ser reescrito.
	// Retorna TRUE si se encontro SubArg, FALSE si no. Si SwOblig TRUE	y no se encuentra,
	//	error immediato. Para controlar los errores, es mejor usar IsValidSubArg (que lanza
	//	tambien mensajes si se quiere).
	// No se llaman igual que sus homologas (ver mas adelante), porque como solo se 
	//	diferencian en el tipo del primer argumento m4char_t *, o m4int16_t, cuando le 
	//	pasamos un 0, el compiler no sabe si es un 0 m4int16_t, o un NULL.

	m4bool_t GetStringFromStr( 
		const m4char_t *ai_pcArg, m4int16_t ai_iSubArg,
		const m4char_t **ao_ppcBeginStr, size_t * ao_pszLen, m4bool_t ai_bSwOblig=M4_TRUE	) ;

	m4bool_t GetStringFromStr( 
		const m4char_t *ai_pcArg, m4int16_t ai_iSubArg,
		ClMIT_Str * ao_poStr, m4bool_t ai_bSwOblig=M4_TRUE
	) ;

	// Devuelve el argumento como long.

	m4int32_t GetLongFromStr( const m4char_t *ai_pcArg, m4int16_t ai_iSubArg ) ;

	// Devuelve el argumento como double.

	m4double_t GetDoubleFromStr( const m4char_t *ai_pcArg, m4int16_t ai_iSubArg ) ;

	// Si pcArg no coincide con ningun pcStr del array ai_Trans, se produce un error 
	//	immediato si SwFound es NULL. Si no lo es, en Found se carga true si todo va bien, 
	//	o false si no. Si es false, ai_SwMsg dice si se quiere disparar un mensaje de 
	//	warning o no.
	// Si se tiene exito, el metodo devuelve el valor int equivalente al enum.

	m4int16_t GetEnumFromStr( 
		const m4char_t *ai_pcArg, m4int16_t ai_iSubArg,
		const EnumTrans_t * ai_Trans, m4bool_t *ao_SwFound=NULL, 
		m4bool_t ai_SwMsg=M4_TRUE
	) ;

	
	// Busca en el array un item cuyo SubArg sea igual a la cadena pasada. Devuelve el
	//	indice de este item, o un valor negativo si no lo encuentra. Si SwOblig, provoca
	//	error immediato si no lo encuentra.
	// La comparacion NO es case-sensitive, y se comparan tantos caracteres como se 
	//	indiquen en NChar (un valor 0 hara que se coja la longitud de Value). 
	//	Para que la localizacion tenga exito, ademas de que coincidan todos los caracteres,
	//	el siguiente caracter en el item debe ser el separador o fin de cadena.
	// Por ello, es posible que una busqueda abarque varios SubArg de golpe: puede hacerse,
	//	por ejemplo, suponiendo que el separador es :, una busqueda de (1,"PAGE:1"), lo
	//	cual haya el item cuyo segundo argumento es PAGE y el tercero 1 
	//	(un item "Hola:Page:1" seria exitoso, y un "Hola:Page:10" no). 

	m4int16_t Find( 
		m4int16_t ai_iSubArg, const m4char_t *ai_pcValue, 
		size_t ai_szNChar=0, m4bool_t ai_bSwOblig=M4_TRUE 
	) ;

	// En casi todos los metodos, si Arg o SubArg fuera de rango, error immediato.
	// Este metodo sirve para testear previamente si existe o no un argumento y un 
	//	subargumento. Si no existe y se pasa alguna cadena en NameArg, se genera un 
	//	además un warning, informando del subargumento que falta.

	m4bool_t IsValidSubArg(m4int16_t ai_iArg, m4int16_t ai_iSubArg, 
		const m4char_t *ai_pcNameArg=NULL) ;

	//Sobrecargas para versiones sobre items del array.

	m4bool_t GetString( 
		m4int16_t ai_iArg, m4int16_t ai_iSubArg,
		const m4char_t ** ao_ppcBeginStr, size_t * ao_pszLen, m4bool_t ai_bSwOblig=M4_TRUE ) 
		{ return GetStringFromStr( GetArg(ai_iArg), ai_iSubArg, ao_ppcBeginStr, ao_pszLen, ai_bSwOblig ) ; }

	m4bool_t GetString( 
		m4int16_t ai_iArg, m4int16_t ai_iSubArg,
		ClMIT_Str * ao_poStr, m4bool_t ai_bSwOblig=M4_TRUE ) 
		{ return GetStringFromStr( GetArg(ai_iArg), ai_iSubArg, ao_poStr, ai_bSwOblig ) ; }

	m4int32_t GetLong( m4int16_t ai_iArg, m4int16_t ai_iSubArg ) 
		{ return GetLongFromStr(GetArg(ai_iArg),ai_iSubArg); }

	m4double_t GetDouble( m4int16_t ai_iArg, m4int16_t ai_iSubArg ) 
		{ return GetDoubleFromStr(GetArg(ai_iArg),ai_iSubArg); }

	m4int16_t GetEnum( 
		m4int16_t ai_iArg, m4int16_t ai_iSubArg,
		const EnumTrans_t *ai_Trans, m4bool_t *ao_SwFound=NULL, 
		m4bool_t ai_SwMsg=M4_TRUE ) 
	{ return GetEnumFromStr(GetArg(ai_iArg),ai_iSubArg,ai_Trans,ao_SwFound,ai_SwMsg) ; }


MIT_PRIVATE:

	static m4int16_t ParseCommand( 
		const m4char_t *ai_pcCommand, m4char_t ai_cSeparator, 
		const m4char_t * ai_ArgSep, const m4char_t * ai_ClosingChar,ClMIT_Str *ai_poArg ) ;

} ;

#endif


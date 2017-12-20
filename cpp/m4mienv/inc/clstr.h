//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             clstr.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:26/01/98
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Manuel Lazcano (01/06/98) - StrDeleteSegment (static & member)
//
// Definition:
//
//    Clase ClStr con optimizacion de memoria dinamica. 
//
//
//==============================================================================


#ifndef __CLSTR__H__
#define __CLSTR__H__

#include "mit.h"

/*
	Clase cadena con allocs optimizados.
*/

class M4_DECL_M4MIENV ClMIT_Str {

	//ACCESO LIMITADO A INSPECTOR

	//-------------------------Miembros-------------------------------------------------

MIT_PRIVATE:

	StMIT_Inspector *m_poInsp ;	//Simple link

	enum OTHERS { SIZE_STRING=260, DEFAULT_INTERBLOCK=100 } ;

	//Cadena estatica de almacenamiento. Siempre que se pueda, se usa esta cadena, para no
	//	tener que alocar nada.

	m4char_t  m_pcStcStr[SIZE_STRING] ;

	//NULL, o cadena dinamica. Esta cadena se reutiliza constantemente para almacenar 
	//	informacion. Nunca decrece, sino que solo crece, y ademas a intervalos minimos de
	//	tamaño m_Interblock, para minimizar las operaciones alloc.

	m4char_t *m_pcDynStr ;		//Solo estara alocada, si hubo algun tamaño>SIZE_STRING
	size_t    m_szDynSize ;		
	size_t    m_Interblock ;	//Por defecto, DEFAULT_INTERBLOCK
	size_t    m_LastAccommodate ;	//Valor del aultimo Accommodate

	//Dice cual es el buffer valido actual SOBRE EL QUE HAY DATOS, si el estatico o el 
	//	dinamico. Siempre que hay un buffer dinamico, las operaciones de copia, etc, se 
	//	ejecutan sobre el, aunque quepan en la estatica. Se hace asi para que siempre
	//	coincida el mayor buffer que se esta usando, con el lugar donde se almacenan las
	//	cadenas, aunque sean estas menores que SIZE_STRING

	m4bool_t  m_bSwStatic ;

MIT_PUBLIC:

	//Cuidado, no confundir con alineaciones de fields
	enum STR_ALIGN { STR_CENTER, STR_LEFT, STR_RIGHT } ;

	//-------------------------Inicializaciones-----------------------------------------

	// Un objeto de este tipo, de entrada, es una cadena estatica. Solo las operaciones
	//	que llamen a Accommodate (StrCpy, StrNCpy, StrInsertChar, StrCat, ToASCIZ) pueden hacer 
	//	que se cambie a una cadena dinamica, y  que esta crezca, a intervalos de InterBlock 
	//	bytes. 
	//  AddSize son bytes que se añaden a la hora de acomodar la cadena, pero que no 
	//	intervienen en la copia.

	ClMIT_Str() ;	//Por defecto, apunta a cadena estatica ""
	virtual ~ClMIT_Str() ; 
	virtual void End() { ClMIT_Str::Accommodate(0); }
	ClMIT_Str(const m4char_t *ai_pcStr) ;

	
	void Init(const ClMIT_Str * ai_poSource) ;	//Valido para cadenas ASCIIZ y no ASCIIZ

	void StrCpy(const m4char_t *ai_pcStr, size_t ai_AddSize=0) ;

	void StrCpy(const ClMIT_Str *ai_poStr, size_t ai_AddSize=0) 
		{ StrCpy(ai_poStr->InChar(0),ai_AddSize) ; }

	void StrNCpy(const m4char_t *ai_pcStr, size_t ai_szSize, size_t ai_AddSize=0) ;

	//Como un StrNCpy, solo que al final de la cadena mete un caracter nulo.
	//Si se especifica AddBlanks, los caracteres adicionales se rellenan a blanco, y el
	//	fin de cadena se mete tras estos. 
	void ToASCIZ(const m4char_t *ai_pcStr, size_t ai_szSize, size_t ai_AddBlanks=0) ;

	// Pone el m_bSwEstatic, y aloca espacio dinamico si es preciso, para poder contener
	//	ai_szSize bytes. (Osea, es TAMAÑO, no longitud). Si ai_bSwCopy es true, ademas se
	//	conserva lo que habia en la cadena ASCIIZ (lo que implica copiar todos los bytes).
	// Se usa como auxiliar de metodos de esta clase, pero tambien puede ser util si vamos
	//	a usar la sobrecarga (char *) con el fin de obtener el puntero para escribir, y
	//	queremos asegurarnos previamente de que quepan ai_szSize caracteres.
	// Un Accommodate(0) tiene el efecto de borrar la cadena dinamica, y hacer que se 
	//	apunte a la cadena estatica "".

	void Accommodate(size_t ai_szSize, m4bool_t ai_bSwCopy=M4_FALSE) ;

	// Auxiliar AnaLex. Mete en el objeto una cadena ASCIIZ, conteniendo desde el primer 
	//	caracter ai_pcStr y metiendo todos caracteres siguientes, hasta que:
	//		-Modo TO_LIMIT. Mientras NO sean caracteres pertenecientes a la cadena ai_pcDelimit. 
	//		-Modo WHILE_CHAR. Mientras SI lo sean.
	// Puede meter en el objeto una cadena vacia.
	// Se devuelve el indice del caracter ai_pcStr donde se paró la busqueda (osea, o bien es 
	//	un caracter de ai_pcDelimit (modo TO_LIMIT), o uno que no lo es (modo WHILE_CHAR) o
	//	es fin de cadena.

	enum MODE_GET_STRING { TO_LIMIT, WHILE_CHAR } ;

	size_t GetString( MODE_GET_STRING ai_Mode, const m4char_t *ai_pcStr, m4char_t *ai_pcDelimit ) ;

	//Inserta ai_szNumChars caracteres  ai_cFillChar en la cadena ASCIIZ de este objeto,
	//	desde la posicion ai_szFromChar (o desde el final si se mete STR_CAT). 
	//Insertar implica que NO se sobreescriben caracteres en el objeto, sino que se mueven 
	//	si es preciso.
	//Si es preciso, acomoda mas bytes.
	//CUIDADO, ao_pcStr debe ser una cadena ASCIIZ (se usa strlen sobre ella).

	enum { STR_CAT=-1 } ;
	void StrInsertChar( 
		size_t ai_szFromChar, size_t ai_szNumChars, m4char_t ai_cFillChar
	) ;

	//Como la anterior, pero sobreescribe caracteres. La cadena puede crecer.
	void StrDuplicateChar( 
		size_t ai_szFromChar, size_t ai_szNumChars, m4char_t ai_cFillChar
	) ;

	//Como sus estaticas, pero la cadena puede crecer.
	void StrInsertStr( size_t ai_szFromChar, const m4char_t *ai_pcStr2, size_t ai_szLen2 )  ;
	void StrInsertStr( size_t ai_szFromChar, const m4char_t *ai_pcStr2)  ;

	
	//Elimina ai_szNumChars caracteres de la cadena ASCIIZ de este objeto,
	//	desde la posicion ai_szFromChar. 
	//Eliminar implica que sino que se mueven los caracteres a la derecha 
	//	si es preciso.
	//CUIDADO, ao_pcStr debe ser una cadena ASCIIZ (se usa strlen sobre ella).

	void StrDeleteSegment( 
		size_t ai_szFromChar, size_t ai_szNumChars
	) ;

	//Homologa a su version estatica, solo que ademas la cadena crece si es preciso.
	void StrAlign( const m4char_t *ao_pcInputStr, size_t ai_szSizeAlign, 
		STR_ALIGN ai_AlignMode) ;

	//-------------------------Operaciones----------------------------------------------

	size_t     StrLen() const ;
	size_t     StrLenTrim() const ;

	//La cadena del propio objeto, y la otra, debe ser ASCIIZ
	void  StrCat(const m4char_t *ai_pcStr) ;
	void  StrCatChar(const m4char_t ai_pcChar) ;


	//Recorta una cadena. Simplemente pone la marca de fin de cadena. Error immediato si
	//	se sobrepasa el maximo.
	//Sepasa la len deseada (no el size).
	void Trim(size_t ai_szLen) ;	

	// Recorta los blancos finales de una cadena 

	void Trim() { Trim(StrLenTrim()) ; } 

	// Recorta blancos iniciales y finales de una cadena

	void FullTrim() ;

	// Pasa la cadena a mayusculas/minusculas

	void ToUpper() { ToUpper(InChar(0)) ; }
	void ToLower() { ToLower(InChar(0)) ; }

	// Tiene efecto en sucesivas alocaciones.

	void    SetInterBlock(size_t ai_Interblock) { m_Interblock=ai_Interblock; } 
	size_t  GetInterBlock() { return m_Interblock; } 


	// Obtener el puntero a la cadena nos da la psoibilidad de operar sobre ella con 
	//	sprintf, etc. SIEMPRE que hagamos esto, se debe ejecutar un OverStringValid para
	//	testear que no se ha sobreescrito memoria.
	// Los testeos SOLO hacen falta en este contexto.

	operator m4char_t *  ( ) 
	{
		return  m_bSwStatic ? m_pcStcStr : m_pcDynStr ;
	};

	operator const m4char_t *  ( ) const  
	{
		return  m_bSwStatic ? m_pcStcStr : m_pcDynStr ;
	};

	m4char_t * InChar(size_t ai_szIndex) const {
		return  (m4char_t *)(m_bSwStatic ? &m_pcStcStr[ai_szIndex] : &m_pcDynStr[ai_szIndex]) ; }

	//Dan la posicion del END_STR, util para concatenar.
	m4char_t * EndChar() const { return InChar(StrLen()); }

	size_t GetMaxSize() const { return  m_bSwStatic ? SIZE_STRING : m_szDynSize ; } 
	size_t GetLastAccommodate() const { return m_LastAccommodate ; }

	// Testea la sobreescritura. Valido si lo que hemos escrito es una cadena ASCIIZ 
	//	(acabada en nulo). Se hace un strlen para hayar lo escrito.

	void OverStringValid() ;

	// Esta es de uso obligatorio si lo que hemos escrito NO es una cadena ASCIIZ. Le
	//	pasamos el numero de bytes escritos.

	void OverStringValid(size_t ai_szSize) ;

	//----------------------------Metodos estaticos---------------------------------------
	/*
	Estos metodos son estaticos para que sirvan tambien para cadenas de caracteres normales.
	*/

	//OverStringValid validan que no se haya sobreescrito la cadena.
	//Si no se pasa BytesWritten, se hace un strlen de la cadena, si no
	//	se usa el valor pasado.
	//Ojo, se estan usando Sizes, no strlens
	//Si se sobrepasó el tamaño, se provoca un error inmediato.

	static void OverStringValid( const m4char_t * ai_pcStr, size_t ai_szMaxSize );
	static void OverStringValid( const m4char_t * ai_pcStr, size_t ai_szMaxSize, 
		size_t ai_szBytesWritten );

	// SafeStrCpy y SafeStrNCpy son como strcpy y strncpy normales, pero validan que no se 
	//	sobrescribe la cadena.

	static void SafeStrCpy( m4char_t *ao_pcDest, size_t ai_szMaxSizeDest, const m4char_t * ai_pccSource);
	static void SafeStrNCpy( m4char_t *ao_pcDest, size_t ai_szMaxSizeDest, 
		const m4char_t * ai_pccSource, size_t ai_szBytesToCopy);

	// Devuelve la longitud de la cadena, de forma que se ignoran todos los blancos finales

	static size_t StrLenTrim( const m4char_t * ai_pcStr ) ;

	// Recorta la cadena

	static void StrTrim(m4char_t * ai_poStr) { ai_poStr[StrLenTrim(ai_poStr)]=M4_END_STR ; } 

	// Es un strncmp sobre cadenas StrTrim
 
	static m4int16_t StrCmpTrim( const m4char_t * ai_pcStr1, const m4char_t * ai_pcStr2 ) ;

	// Es un strnicmp sobre cadenas StrTrim

	static m4int16_t StrICmpTrim( const m4char_t * ai_pcStr1, const m4char_t * ai_pcStr2 ) ;

	static void ToUpper( m4char_t * ai_pcStr ) ;
	static void ToLower( m4char_t * ai_pcStr ) ;

	//--------->Inserciones y borrados

	//Inserta ai_szNumChars caracteres  ai_cFillChar en la cadena ASCIIZ ao_pcStr, desde la
	//	posicion ai_szFromChar (o desde el final si se mete STR_CAT). Insertar implica que
	//	NO se sobreescriben caracteres en ao_pcStr, sino que se mueven si es preciso.
	//ai_szmaxSize es el maximo tamaño de la cadena, para testear OverStrings.
	//CUIDADO, ao_pcStr debe ser una cadena ASCIIZ (se usa strlen sobre ella).
	
	static void StrInsertChar( 
		m4char_t *ao_pcStr, size_t ai_szFromChar, size_t ai_szNumChars, 
		m4char_t ai_cFillChar, size_t ai_szMaxSize 
	) ;

	//Igual que la anterior, solo que NO se inserta (no se mueven caracteres), si no que se
	//	machacan carateres tal cual, metiendo el fin de cadena si se precisa. 
	static void StrDuplicateChar( 
		m4char_t *ao_pcStr, size_t ai_szFromChar, size_t ai_szNumChars, 
		m4char_t ai_cFillChar, size_t ai_szMaxSize 
	) ;

	//Parecida a la anterior, hace uso de esta. La unica diferencia es que en vez de 
	//rellenar el hueco dejado con la iteracion de un caracter, se copian tantos 
	//caracteres de ai_pcStr2 como se indique (esta no tiene porque ser ASCIIZ). 

	static void StrInsertStr( 
		m4char_t *ao_pcStr, size_t ai_szFromChar, const m4char_t *ai_pcStr2, 
		size_t ai_szLen2, size_t ai_szMaxSize 
	) ;
	//Como la anterior, se copia toda la cadena (que es ASCIIZ)
	static void StrInsertStr( 
		m4char_t *ao_pcStr, size_t ai_szFromChar, const m4char_t *ai_pcStr2, size_t ai_szMaxSize ) ;

	//Elimina ai_szNumChars caracteres en la cadena ASCIIZ ao_pcStr, desde la
	//	posicion ai_szFromChar. Eliminar implica que
	//	se mueven los caracteres necesarios si es preciso.
	//ai_szmaxSize es el maximo tamaño de la cadena, para testear OverStrings.
	//CUIDADO, ao_pcStr debe ser una cadena ASCIIZ (se usa strlen sobre ella).

	static void StrDeleteSegment( 
		m4char_t *ao_pcStr, size_t ai_szFromChar, size_t ai_szNumChars, 
		size_t ai_szMaxSize 
	) ;

	//--------------Otros

	//Alinea el valor de ao_pcInputStr, de forma que siempre ocupan ai_szSizeAlign caracteres,
	//dejando el resultado en ao_pcStr.
	//Si la longitud de ao_pcInputStr es mayor o igual que ai_szSizeAlign, simplemente se 
	//copia ao_pcInputStr en ao_pcStr.
	static void StrAlign( 
		const m4char_t *ao_pcInputStr, size_t ai_szSizeAlign, 
		STR_ALIGN ai_AlignMode, m4char_t * ao_pcStr, size_t ai_szMaxSize ) ;

	//Como strstr(), busca  ai_pcSubStr en ai_pcStr, dando el puntero a ai_pcStr donde comienza
	//o NULL si no lo encuentra. Si ai_pcSubStr es "", retorna ai_pcStr.
	static m4char_t *StrStr(m4char_t *ai_pcStr, m4char_t *ai_pcSubStr ) ;

	//Como la anterior, pero no es case-sensitive
	static m4char_t *StrIStr(m4char_t *ai_pcStr, m4char_t *ai_pcSubStr ) ;

	// ------------------------------Sobrecargas para STL---------------------------------	

	ClMIT_Str(const ClMIT_Str& ai_oSource) ;
	ClMIT_Str & operator = (const ClMIT_Str &ai_oOpe);
	m4bool_t operator == (const ClMIT_Str &ai_oOpe) const;	//Comparaciones stricmp
	m4bool_t operator < (const ClMIT_Str &ai_oOpe) const;

} ;



#endif


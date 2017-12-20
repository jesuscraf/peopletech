//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             sysinfo.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:18/11/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//	El *.h es multiplataforma.
//	El *.cpp es TEMPORAL Y DEPENDIENTE DE PLATAFORMA; usa el registry. 
//
//==============================================================================

#ifndef _SYSINFO_
#define _SYSINFO_

#include "m4stl.hpp"
#include "clstr.h"

//========================================================================ClMIT_BinaryAdapter
/*
	Esta clase es util para codificar y decodificar ficheros binarios portables entre 
plataformas hardware. Cada plataforma graba la informacion binaria en un orden determinado,
o bien grabando byte a byte de LSB a MSB,  o viceversa.
	Con esta clase podemos convertir datos de un orden a otro, con lo cual podemos 
decodificar ficheros que no sean de la plataforma actual (o generar ficheros directamente
legibles en otras plataformas).
*/

class M4_DECL_M4MIENV ClMIT_BinaryAdapter
{

	//ACCESO LIMITADO A INSPECTOR

MIT_PUBLIC:

	//LITLE_ENDIAN. (Plataformas PC). El byte LSB va primero (en la posicion mas baja de memoria).
	//BIG_ENDIAN.   (Plataformas Mac, y algunas UNIX) . Al reves, el byte MSB va primero.

	enum ENDIAN_ORDER { LITLE_ENDIAN, BIG_ENDIAN } ;

MIT_PRIVATE:

	ENDIAN_ORDER m_FileStorage, m_RAMStorage ;

MIT_PUBLIC:

	//-----------------------------------Inicializacion---------------------------------------

	virtual ~ClMIT_BinaryAdapter() { ClMIT_BinaryAdapter::End(); }
	virtual size_t GetSizeof() { return sizeof(ClMIT_BinaryAdapter) ; }

	//El constructor llama a Init (no hace falta una call explicita).
	ClMIT_BinaryAdapter() { Init() ; } 
	virtual void End() {;}

	//Por defecto, pone el File y RAM Storage al de la maquina (ver SysInfo).
	void Init() ;

	//-----------------------------------Get&Set--------------------------------------------

	void SetFileStorage(ENDIAN_ORDER ai_Storage) { m_FileStorage=ai_Storage; }
	ENDIAN_ORDER GetFileStorage() { return m_FileStorage; }

	void SetRAMStorage(ENDIAN_ORDER ai_Storage) { m_RAMStorage=ai_Storage; }
	ENDIAN_ORDER GetRAMStorage() { return m_RAMStorage; }

	//-----------------------------------Conversiones---------------------------------------
	//Encode significa pasar de RAM a File storage
	//Decode significa pasar de File a RAM storage
	//Segun necesitemos conversiones a otros tipos, iremos poniendo mas metodos.

	m4int16_t EncodeInt( m4int16_t ai_iInt) ;
	m4int16_t DecodeInt( m4int16_t ai_iInt) ;

	m4int32_t EncodeLong( m4int32_t ai_iLong) ;
	m4int32_t DecodeLong( m4int32_t ai_iLong) ;

	// LIMITACION:Este metodo debe poder ejecutarse autonomamente, sin necesidad de que haya
	//	ninguna Task creada. (No puede acceder a Insp, ni a ninguna de nuestras clases, etc).

	static void Flip(void *ai_Value, size_t ai_szBytes) ;

	// Devuelve el ENDIAN_ORDER de la maquina donde se esta ejecutando.
	// Las mismas LIMITACIONES que Flip
	static ENDIAN_ORDER TestEndian() ;

};

//===========================================================================ClMIT_SysInfo

class M4_DECL_M4MIENV ClMIT_SysInfo {

	//ACCESO LIMITADO A INSPECTOR

MIT_PRIVATE:

	StMIT_Inspector *m_poInsp ;	//Simple link

	// En la lista almacena valores leidos de registry. Solo se leen una vez, pues esta es una
	//	operacion lenta, se guarda la cadena, y todos los gets accceden a estas cadenas.

	typedef map < MIT_Id_t, ClMIT_Str, less<MIT_Id_t> > ValueList_t ;
	ValueList_t m_oVList ;	

	ClMIT_BinaryAdapter::ENDIAN_ORDER m_BinaryStorage ;	

	enum { FIRST_EXTEND_ANSI=128, COUNT_EXTEND_ANSI=128, COUNT_HTML_ESCAPE=52 } ;
	static const m4uint8_t  m_pcANSIToASCII[COUNT_EXTEND_ANSI] ;
	static const m4uint16_t  m_pcANSIToISO[COUNT_EXTEND_ANSI] ;
	static const m4char_t * m_pcHTML_Escape[COUNT_HTML_ESCAPE] ;

	// Gestión concurrente de directorios temporales (Bug 169941)
	// Se utiliza un directorio por thread
	// Elementos necesarios: Mutex de concurrencia, Mapa de Id Thread con número de directorio
	static ClCriticalSection s_oMutexMap;

	typedef map< m4iden_t, ClMIT_Str, less<m4iden_t> > M4ThreadDirectoryMap_t;
	static M4ThreadDirectoryMap_t s_MapThread;


MIT_PUBLIC:

	ClMIT_SysInfo() ;

	virtual ~ClMIT_SysInfo() { ClMIT_SysInfo::End(); }
	virtual void End();
	void Init();	

	m4bool_t PrepareRegistryPath(ClMIT_Str* ao_poBuff);

	// Mete un nuevo valor de registry. KeyId es el Id con el que se referenciara el valor a
	//	posteriori (probablemente sera un enum), y KeyDir y Key identifican el lugar de donde
	//  leer la cadena.
	// Se da TRUE si no ha habido problemas (la Key no existia, y se ha insertado). Si no se
	//	testea no pasa nada, a posteriori se tratara el error.
	// Fijarse que introducir varias veces un mismo KeyId con este metodo tiene el efecto de
	//	que solo se le hace caso a la primera llamada.

	m4bool_t NewValue( MIT_Id_t ai_KeyId, m4char_t *ai_pcKeyDir, m4char_t *ai_pcKey ) ;

	// Se lee un valor que previamente se ha debido introducir via NewValue.
	// Si ao_poBuff=NULL, simplemente no se actualiza.
	// Da true si todo va bien (KeyId encontrada), o false si no. Como antes, puede o no 
	//	tratarse el error. Siempre se actualiza Buff, ya sea con el valor encontrado, o con
	//	la cadena ASCIIZ "ERROR!!!!".

	m4bool_t  GetValue( MIT_Id_t ai_KeyId, ClMIT_Str *ao_poBuff ) ;

	m4bool_t  ExistValue(MIT_Id_t ai_KeyId) { return GetValue(ai_KeyId,NULL) ; }

	//	Obtiene el binary storage para la plataforma actual. Varia por plataforma hardware.
		
	ClMIT_BinaryAdapter::ENDIAN_ORDER GetEndian() { return m_BinaryStorage; }

	//Añade a Path la cadena Str. Si Path NO acaba en \, se le inserta uno.
	//Tambien hace un OverStringValid
	//Funciona correctamente en UNIX: segun la plataforma, usa \ ó /
	static void AppendToPath( ClMIT_Str *ao_poPath, const m4char_t *ai_pcStr ) ;	

	//Añade a Path la cadena Str. Se entiende que añade al nombre del fichero, por
	//	lo que no mira nada respecto al \. 
	//Si ai_bTestExt es true, si el nombre tiene extension, inserta antes de esta.
	static void AppendToFile( ClMIT_Str *ao_poPath, const m4char_t *ai_pcStr, 
		m4bool_t ai_bTestExt=M4_TRUE ) ;	

	//Rompe el Path del fichero de salida en dos y devuelve el nombre del Path
	// (ao_poPath) y el nombre del fichero (ao_poName).
	//Funciona correctramente en Win32 y en UNIX
	static void ExtractFromPath( const m4char_t *ao_piPath, ClMIT_Str *ao_poPath, 
		ClMIT_Str *ao_poName );

	//Convierten los codigos ANSI extendidos de windows que encuentre en una cadena.
	//Los caracteres sin traduccion directa no se modifican.
	//Se hacen algunos ajustes sobre ANSI no existentes en ASCII. Estos casos son:
	//	-El acento a derecha por el acento a izquierda.
	//	-Las mayusculas acentuadas o con cualquier otro tipo de acento, que no tengan 
	//		traduccion, se traducen libremente a la letra mayuscula o minuscula con la 
	//		acentuacion mas parecida. 
	static void ANSIToASCII(m4char_t *ai_pcStr, size_t ai_szNChar ) ;
	static void ANSIToASCII(m4char_t *ai_pcStr) ;

	//Convierten los codigos ANSI extendidos de windows a codigo ISO.
	//Los caracteres sin traduccion directa no se modifican.
	static void ANSIToISO(m4char_t *ai_pcStr, size_t ai_szNChar ) ;
	static void ANSIToISO(m4char_t *ai_pcStr) ;

	//Convierte una cadena en cosigo ANSI a una cadena en codigo ISO, pero con los 
	// codigos numéricos precedidos de un codigo de escape.
	// En principio sólo va a hacer unso de este metodo el Device de PostScript
	// puesto que necesita reconocer los codigos extendidos mediante un codigo
	// de escape
	static void ConvertToIsoWithEscape(ClMIT_Str *ai_poStr, size_t *ai_szNChar);

	//Igual que la anterior, pero no convierte el caracter en si, solo lo pone en forma de escape

	static void ConvertToEscape(ClMIT_Str *ai_poStr, size_t *ai_szNChar);

	//Deje en ao_Str una cadena ASCIZ con las conversiones a escape HTML pertinentes 
	//	(pe, convierte un "char >" a "char&nbsp;&gt;"
	static void EscapeToHTML(m4char_t *ai_pcStr, ClMIT_Str *ao_Str) ;
	static void EscapeToHTML(m4char_t *ai_pcStr, size_t ai_szNChar, ClMIT_Str *ao_Str) ;
} ;




#endif

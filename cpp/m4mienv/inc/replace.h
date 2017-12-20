
#ifndef __COPYREPLACE_H__
#define __COPYREPLACE_H__

#include "clstr.h"
#include "clfile.h"

#include "m4stl.hpp"

class ClMIT_CopyReplace ;

//========================================================================ClMIT_ReplacePattern

class M4_DECL_M4MIENV ClMIT_ReplacePattern {

	friend class ClMIT_CopyReplace ;

MIT_PUBLIC:

	typedef vector<ClMIT_Str> MatchingVector_t ;

MIT_PRIVATE:

	ClMIT_Str m_oPattern ;
	ClMIT_Str m_oValue ;
	MatchingVector_t m_oMatching ;

MIT_PUBLIC:

	ClMIT_ReplacePattern(const m4char_t *ai_Pattern) ;

	const m4char_t * GetPattern() { return m_oPattern.InChar(0) ; }
	const m4char_t * GetValue() { return m_oValue.InChar(0) ; }

	void ChangeValue(const m4char_t *ai_Value) { m_oValue.StrCpy(ai_Value) ; }

	//Como normalmente va a haber un array fijo de patterns que se rautilizan, para evitar
	//  porblemas de recursiones y accesos a matchings que se borran, NO damos acceso
	//	deliberadamente a la liusta de matchings, si no que se hacen copias locales con
	//	este metodo.

	void GetMatchings( MatchingVector_t *ao_Matchings ) {  *ao_Matchings=m_oMatching ; }
} ;

//========================================================================ClMIT_CopyReplace

class M4_DECL_M4MIENV ClMIT_CopyReplace {

MIT_PUBLIC:

	//La clave debe ser el PatternId 
	typedef map < ClMIT_Str, ClMIT_ReplacePattern, less<ClMIT_Str> > Patterns_t ;

MIT_PRIVATE:

	StMIT_Inspector *m_poInsp ;	//Simple link

	ClMIT_Str m_oTemplate ;
	ClMIT_Str m_oTagDelim[2] ;	//0 ->Open, 1->Close
	ClMIT_Str m_oValueDelim ;

MIT_PUBLIC:

	ClMIT_CopyReplace() ;

	virtual ~ClMIT_CopyReplace() { ClMIT_CopyReplace::End(); }
	virtual void End() ;

	//Copiara desde ai_TemplateFile al fichero ai_OutFile, sustituyendo todas las ocurrencias de
	//	los patrones pasados con los correspondientes valores. Se usara como delimitadores la
	//	cadena pasada en ai_Delim.
	// Da false si hay problemas.
	// Por ejemplo, si el fichero origen contiene:
	//
	//    Don %%NAME%% vive en %%DOMICILIO%%. ¿Es correcto, don %%NAME%%?
	//	  Su fichero de configuración está en %%ROOT_PATH==data.txt%%
	//
	// El array de patrones contendria 2 elementos:
	//	
	//		"NAME"			"Fernando"
	//		"DOCIMICILIO"	"C/Pepon"
	//		"ROOTPATH"		"c:\configs\"
	//
	// Delim seria:
	//
	//		Tag   "%%"
	//		Value "=="
	//
	// Y el resultado almacenado en el fichero de salida seria:
	//
	//    Don Fernando vive en C/Pepon. ¿Es correcto, don Fernando?
	//	  Su fichero de configuración está en c:\configs\data.txt
	//
	//
	// También admite recuperar valores de salida asociados a tags. En este caso,
	//	a la salida de la función habría un valor "data.txt" almacenado en la lista de 
	//	matchings del pattern ROOTPATH.
	//
	// Fijarse que estas dos lines en el template:
	//
	//	  Su fichero de configuración está en %%ROOT_PATH==data.txt%%
	//	  Su fichero de configuración está en %%ROOT_PATH%%data.txt
	//
	// Producen la misma salida, es decir:
	//
	//	  Su fichero de configuración está en c:\configs\data.txt
	//
	// La diferencia es que en la primera si se notificaria "data.txt" en la lista de matchings
	//	del pattern ROOTPATH, pero en la segunda no. Es decir, cualquier valor comprendido 
	//	entre == y el %% final se considera valor a notificar 
	//	(y por supuesto a almacenar en salida).

	//Inicializa leyendo el template desde un fichero.
	//Los delimitadores de tags y de valor NO pueden ser iguales 
	//false si error (no lo encuentra).

	m4bool_t InitFromFile( const m4char_t *ai_TemplateFile, 
		const m4char_t *ai_TagDelimOpen, const m4char_t *ai_TagDelimClose, 
		const m4char_t *ai_ValueDelim ) ;

	//Incializa leyendo el template directamente  de una cadena.

	void InitFromString( const m4char_t *ai_TemplateString, 
		const m4char_t *ai_TagDelimOpen, const m4char_t *ai_TagDelimClose, 
		const m4char_t *ai_ValueDelim ) ;

	//Ejecuta una instancia de replace, añadiendo el resultado en un fichero ya abierto.
	//Puede ejecutarse n veces, que serian como sucesivos cut&paste al fichero de salida con 
	//  los datos pasados.
	//Todos los patterns que aparecen en la plantilla deben tener su homologo en el array de
	//	valores de patterns pasados, o se generará un error.
	//La lista de matchings se resetea en cada llamada.

	m4bool_t Replace( ClMIT_File *ai_F, Patterns_t *ao_Patterns ) ;

	//Como el anterior, pero abre el fichero (si existe, machaca contenido), y almacena ahi una
	//	sola instancia (n llamadas machacarian el fichero, equivaldría a una).

	m4bool_t Replace( const m4char_t *ai_OutFile, Patterns_t *ao_Patterns ) ;

	//Como el anterior, pero almacena en una cadena (segun ai_bSwStrCat, resetea la cadena 
	//	o hace un StrCat).

	m4bool_t Replace( ClMIT_Str *ao_Str, Patterns_t *ao_Patterns, 
		m4bool_t ai_bSwStrCat=M4_FALSE ) ;

	//Util para obtener información de matching de una template. No hace falta pasar todos los
	//	patterns del array, no se genera error si alguno no aparece.

	m4bool_t GetMatching( Patterns_t *ao_Patterns ) ;


	//Carga una lista en base a un array de pattern Ids.
	//Meter el ultimo como NULL o "" para indicar final.

	static void LoadPatternList(Patterns_t *ao_Pattern, const m4char_t *ai_pcPattenIds[], 
		m4bool_t ai_bSwAppend=M4_FALSE ) ;

} ;


#endif

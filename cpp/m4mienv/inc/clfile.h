//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             clfile.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/28/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//    Gestion basica de ficheros.
//
//
//==============================================================================
#ifndef __CLFILE__H__
#define __CLFILE__H__

#include <stdio.h>

struct StMIT_Inspector ;
class ClMIT_Str ;

//==============================================================================ClMIT_File

class M4_DECL_M4MIENV ClMIT_File  
{

	//ACCESO LIMITADO A INSPECTOR 
	//Ojo, se usa TaskEnv en Compare. No usar este metodo en construccion ni destruccion de
	//	este objeto.

MIT_PRIVATE:

	StMIT_Inspector *m_poInsp ;	//Simple link

	FILE *m_pstF ;

	struct {
		m4char_t * Str ;			//Cadena dinámica
		size_t   Size ;
	} m_StPath ;

MIT_PUBLIC:

	//-----------------------------------Inicializacion---------------------------------------

	ClMIT_File() ;  

	virtual ~ClMIT_File() { ClMIT_File::Close() ;}
	virtual size_t GetSizeof() { return sizeof(ClMIT_File) ; }

	// EXIST     : fichero debe existir o error. Abre el fichero en modo 
	//				READ o READ_WRITE.
	// NEW       : fichero no debe existir o error . Crea el fichero. Abre en modo	
	//				WRITE  o READ_WRITE.
	// OVERWRITE : si el fichero existe lo machaca, si no crea uno nuevo. Abre en modo
	//				WRITE o READ_WRITE.

	enum MODE_EXIST  { EXIST, NEW, OVERWRITE } ;
	enum MODE_WRITE  { READ, WRITE, READ_WRITE } ;

	// Hacer open antes de utilizar otros metodos.
	// Abre siempre en modo binario.
	// Si ya se abrio un fichero, y no se cerro, da un error inmediato.
	// Si no puede abrir fichero, o se usa una combinacion de modos invalida, da 
	//	un error.
	// Retorna TRUE si tiene exito. 
	// Con FORCE_OPEN, se genera un error immediato si el fichero no puede abrirse.
	// Con TRY_OPEN, el error se propaga.
	// Un SILENT_TRY_OPEN es como un TRY_OPEN, pero nunca se generarn mensajes ni se usa
	//	el trace para nada (importante; este modo es usado en el ClTrace::Init).

	enum ACTION_ON_OPEN { FORCE_OPEN, TRY_OPEN, SILENT_TRY_OPEN } ;

	m4bool_t Open( const m4char_t *ai_pcPath, MODE_EXIST ai_Exist, MODE_WRITE ai_Write,
		ACTION_ON_OPEN ai_Mode, m4bool_t ai_bAddBOM ) ;

	// Cierra solo si existe fichero abierto.
	void Close() ;

	// NOTA: Todos metodos testean que haya algun fichero abierto, dando errores locales si
	//	no lo hay.

	//-------------------------Read&Write&Seek---------------------------------------------

	// Se escribe desde la posicion actual, e incrementa esta.
	// Genera errores locales si hay problemas de escritura. 
	// Si ai_szNBytes es 0, no hace nada.
	void W( const void *ai_pvBuff, size_t ai_szNBytes ) ;


	// Escribe cadenas
	void WStr( const m4char_t * ai_pcStr ) ;

	// Escribe char
	void WChar(m4char_t ai_pcChar);

	// Da NumBytes leidos, siempre=ai_szNBytes, excepto si eof, que puede dar menos.
	// Se lee desde la posicion actual, e incrementa esta.
	size_t R( void * ai_pvBuff, size_t ai_szNBytes ) ;

	//Vuelca el contenido de todo el fichero a una cadena.
	//Da el numero de bytes leidos.

	size_t DumpToString( ClMIT_Str *ao_Str ) ;

	//Copia un fichero completo. Da false si no puede abrir el fichero.
	//Si ao_szWritten se pasa, da el numero de bytes escritos.

	m4bool_t WFile( const m4char_t * ai_pcFile, size_t *ao_szWritten=NULL ) 
		{ return WFile(ai_pcFile,0,-1L,ao_szWritten);  }

	//Copia parte de un fichero, desde  ai_From a ai_Count. Si se excede la cuenta, solo se
	//	graba lo posible.
	//(Un ai_Count de -1 graba todo el fichero)
	//Da false si no puede abrir el fichero.
	//Si ao_szWritten se pasa, da el numero de bytes escritos.

	m4bool_t WFile( const m4char_t * ai_pcFile, m4uint32_t ai_From, size_t ai_Count,
		size_t *ao_szWritten=NULL ) ;

	// Dan la posicion actual, y la ponen. La posicion se pondera en bytes.
	
	m4uint32_t GetPos() ;
	void SetPos( m4uint32_t ai_uiOff ) ;

	// Da el tamaño en bytes. 

	m4uint32_t GetSize() ;

	m4bool_t IsEOF() ;
	
	//-------------------------Otros-------------------------------------------------------

	// Hace un flush del fichero

	void Flush() ;

	// Para uso immediato, nombre del fichero, o NULL si no hay ninguno abierto.

	const m4char_t * GetName() { return m_StPath.Str ; }

	//Da true si hay algun fichero abierto

	m4bool_t IsOpen() { return M4_BOOL(m_pstF != NULL); } 

	//Borra un fichero que debe existir Da true si lo borra, false si hay error. Si no puede
	//	borrarlo, genera un error local si ai_SwMsg (desactivarlo solo en situaciones en las 
	//	que no esta el fichero trace disponible).
	static m4bool_t Remove(const m4char_t *ai_pcStr, m4bool_t ai_SwMsg=M4_TRUE) ;

	//Mueve (renombra) un fichero que debe existir. Da true si lo mueve, false si hay error. 
	//	Si no puede moverlo, genera un error local. Lo de SwMsg, como en Remove.
	
	static m4bool_t Move(const m4char_t *ai_pcSource, const m4char_t *ai_pcDest, m4bool_t ai_SwMsg=M4_TRUE) ;

	// Puede ejecutarse sobre ficheros NO abiertos o abiertos: dice si existe o no un fichero.
	static m4bool_t Exist(const m4char_t *ai_pcStr) ;

	// Puede ejecutarse sobre ficheros NO abiertos o abiertos.
	// Hace una comparacion binaria de dos ficheros. Da true si son iguales.
	// No se testean fechas ni nada parecido, sino el contenido de los ficheros en si (por lo
	//	tanto, un tamaño distinto ya provoca un false).
	// Tambien dara false si cualquiera de los dos ficheros no existe o no puede abrirse.

	static m4bool_t Compare( const m4char_t * ai_pcFileA, const m4char_t * ai_pcFileB ) ;

	//-------------------------Tratamiento de paths-----------------------------------------

MIT_PRIVATE:

	// Separadores de elementos path. Segun el SO, varian.

	static const m4char_t m_cPathSep ;
	static const m4char_t m_cExtSep;
	static const m4char_t m_cDriveSep ;	

MIT_PUBLIC:
	/*
	  No hay metodos tipo transformacion de un path de formato \ a formato /, pues no tiene
	sentido: la parametrizacion de paths cambia radicalmente de Windows a Unix, y no deberia 
	de ser reutilizable de ninguna forma.
	  El metodo base que se utiliza en casi todos los metodos que tratan paths, diferencia 
	entre fichero y path, y es el metodo SplitPath. Vamos a comentarlo en profundidad, y lo 
	que vale para este metodo, vale para todos.

	Reconocimiento de paths en Windows:

		-El separador de paths es \
		-Una secuencia inicial de Letra: SIEMPRE forma parte de un path, nunca de un nombre 
		de  fichero. Deben aparecer al principio de la cadena (no se admiten blancos iniciales).
		-La ultima cadena que no acaba en \, se interpreta por convencion como nombre de 
			fichero,aunque puede ser que sea un directorio. Cada metodo en concreto, ya sabe 
			si debe	utilizar esta cadena como fichero o path. Si un metodo trata paths sin 
			fichero, puede usar ConvertToPath para eliminar ambiguedades.
		-Si el Path acaba en \, nunca se interpretara nada como fichero.
		-El nombre de fichero (no el path) puede contener la extension. Esta estara formada
			por la ultima cadena que empieza por punto.
		-No se interpretan variables de entorno dentro del path. Es decir, un c:\a\$VARIABLE no
			es correcto. ???? NO ESTOY SEGURO, PROBAR.
		-Ejemplos:
			""			Path=""			File=""			Ext=""
			\			Path=\			File=""			Ext=""	//Acaba en \, no ambiguo
			c:\			Path=c:\		File=""			Ext=""	//Acaba en \, no ambiguo
			c:a\		Path=c:a\		File=""			Ext=""	//Acaba en \, no ambiguo
			c:a			Path=c:			File=a			Ext=""
			c:\a.b.abcd	Path=c:\		File=a.b.abcd	Ext=abcd
			c:\.abc		Path=c:\		File=""			Ext=abc

	Reconocimiento de paths en UNIX:

		-El separador de paths es /
		-En UNIX no existen letras de drive, no nos preocupamos de eso.
		-Formalmente, no hay extensiones, pero nosotros las interpretamos de la misma forma
		-Tampoco se interpretan variables ???? NO ESTOY SEGURO, PROBAR
		-Por lo de demas, es igual que para Windows.

	*/

	//	Notas adicionales a lo ya comentado para SplitPath: 
	//	-Si ao_poExt es NULL, ao_poFile contendra tanto nombre como extension, 
	//		pero si no lo es, ao_poFile contendra el nombre SIN la extension, y ao_poExt la 
	//		extension.
	//	-Se devuelve true si el path obtenido es divisible en mas paths. Esto se hace testeando
	//		que  ao_poPath contenga algun separador \ que no sea el ultimo caracter, y que venga
	//		seguido de algun caracter distinto de \ (para evitar recursiones en paths invalidos).
	//		Aunque se devuelva false, un metodo que maneje solo paths sin ficheros, tambien deberia
	//		tener en cuenta que File y Ext (si hay), puede contener un nombre de directorio final.
	//		Pero lo mejor en este caso es usar ConvertToPath, con lo cual nunca habra nada en 
	//		fichero.

	static m4bool_t SplitPath( const m4char_t * ai_pcFullPath, ClMIT_Str *ao_poPath, 
		ClMIT_Str *ao_poFile, ClMIT_Str *ao_poExt=NULL ) ;

	// Convierte un path sin o con fichero a path sin fichero. Lo que hace es ver si la cadena 
	//	acaba en \, y si no es asi se concatena un \, pero solo si el path contiene algo distinto
	//	de vacio y de Drive:, es decir, un c: NO se convierte a c:\, o un "vacio" no se convierte
	//	a \, pero un a\b si se convierte a a\b\.
	//	Util para metodos que manejan solo Paths, pues convierte un path ambiguo no acabado en \ 
	//	(ambiguo pues un \a no se sabe si es directorio o fichero), a un path no ambiguo. 
	//	(\a\ seguro que es path).

	static void ConvertToPath(ClMIT_Str *ao_poPath) ;

	// Convierte un path de cualquier sistema operativo a un path apropiado para
	//  una URL de un web server desde un navegador. Basicamente sustituye los '\'
	//  por '/'

	static void ConvertToWebPath(ClMIT_Str *ao_poPath) ;

	// Convierte un path a el formato de la plataforma en la que se esta ejecutando esta
	//	DLL. Es decir, si estamos en UNIX sustituira los \ por /, y viceversa.
	
	static void ConvertToThisPlatform(ClMIT_Str *ao_poPath) ;

	//Concatena a ao_poFile un . y la extension. Si no hay extension, no hace nada.

	static void ConcatFileExt(ClMIT_Str *ao_poFile,const m4char_t *ai_pcExt) ;

	// Concatena en ao_poPath otro path o fichero. Añade \ antes de concatenar si es necesario.
	// Ejemplos: c:\a + b = c:\a\b, c:\a\ + b\c = c:\a\b\c

	static void ConcatToPath( ClMIT_Str *ao_poPath, const m4char_t *ai_pcStr ) ;

	//Expande el nombre de un path. Pe, si tenemos "c:\temp\" y expandimos con "_2", tendremos
	//	"c:\temp_2" (fijarse que no es igual que ConcatToPath, que crearia "c:\temp\_2" )

	static void ExpandPathName( ClMIT_Str *ao_poPath, const m4char_t *ai_pcPathExt ) ;

	// Inserta, justo antes de la extension si la hay, un nombre de fichero.  
	// Ejemplos: c:\a + b = c:\ab, c:\a.ext + b = c:\ab.ext, c: + b = c:b

	static void InsertFileName( ClMIT_Str *ao_poPath, const m4char_t *ai_pcStr ) ;

	/*
	* Obtiene de un path absoluto dado, un path relativo, siendo este un path que referencia a 
	* un fichero o a un directorio, depende del PathTo.
	*
	* -ai_FromPath  Path absoluto desde el que queremos 'acceder' o referenciar a otro path.
	* -ai_ToPath    Path a referenciar (directorio o fichero).
	* -ao_Relative  Cadena con el path relativo devuelto. Puete tener todo el ToPath si 
	*		ai_ToPath no es relativo a ai_FromPath (da false en este caso).
	*  IMPORTANTE: para poder hayar paths relativos es importante saver si el ultimo elemento 
	* es un directorio o un fichero. Para el algoritmo, lo que no acabe en el caracter
	* separador es path, por lo que es muy recomendable hacer un ComvertToPath  previo a lo
	* que sepamos que es path. El flag path los pone automaticamente a ambos. Ejemplos:
	* 
	*  From							To								Resultado
	*  c:\temp\rpt\					c:\temp\rpt\img\				img\
	*  c:\temp\rpt\a.html			c:\temp\rpt\img\logo.gif		img\logo.gif	
	*  c:\temp\rpt\break\a.html		c:\temp\rpt\img\logo.gif		..\img\logo.gif
	*/
	static m4bool_t GetRelativePath(
		const m4char_t *ai_pcFromPath, const m4char_t *ai_pcToPath, ClMIT_Str *ao_RelativePath,
		m4bool_t ai_bSwAllIsPath=M4_FALSE) ;


	// Crea un path. Es un metodo recursivo, mas potente que los que suelen dar los SO, ya que 
	//	crea toda la rama de paths, si es necesario. Si no tiene exito en algun subpath, y
	//	ya se crearon previemante otros directorios, estos son borrados.
	// Los posibles valores de retorno son:
	//	-PATH_CREATED. Todo fue bien.
	//	-PATH_ERROR. No se ha creado el path. No se pudo.
	//	-PATH_EXIST. Puede no ser indicativo de error. simplemente no se creo porque ya existe.
	//  Solo si PATH_ERROR, el parametro ai_Action entra en juego para decir que hacer. Un
	//FORCE_CREATE genera un error immediato, un TRY_CREATE genera un error y lo propaga, y 
	//un SILENT_TRY_CREATE solo lo propaga.

	enum CREATE_PATH_RETURN { PATH_CREATED, PATH_ERROR, PATH_EXIST } ;
	enum ACTION_ON_CREATE_PATH { FORCE_CREATE, TRY_CREATE, SILENT_TRY_CREATE } ;

	static CREATE_PATH_RETURN CreatePath( const m4char_t *ai_pcPath, ACTION_ON_CREATE_PATH ai_Action=FORCE_CREATE ) ;

	m4bool_t TryToDownloadFromURL(const m4char_t * ai_pFromPath, m4int32_t ai_iTargetFilePathLen, m4char_t * aio_pTargetFilePath);

};

#endif


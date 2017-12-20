//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             designin.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:8/01/98
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//
//
//==============================================================================

#ifndef __DESIGNIN__H__
#define __DESIGNIN__H__

/*
Contiene enums y declaraciones comunes para designdd y designdt, y algunas generales en otros
	sitios.
*/

#include "clstr.h"
#include "m4stl.hpp"

class ClMIT_Chn;

//----------------Estructura básica del report

enum ENG_MODE_METRIC { ENG_MILIM, ENG_DINCHES } ;

enum ENG_TYPE_CONTAINER { 
	ENG_CNT_BEFORE, ENG_CNT_AFTER, ENG_CNT_HEADER, ENG_CNT_FOOTER, 
	ENG_CNT_PRERECORD,  ENG_CNT_POSTRECORD, ENG_CNT_RECORD, 
	ENG_CNT_COUNT,
	ENG_CNT_FIRST=ENG_CNT_BEFORE, ENG_CNT_LAST=ENG_CNT_RECORD //Uso interno, para bucles
} ;

enum ENG_FIELD_TYPE { 
	ENG_FLD_STRING, ENG_FLD_MULTIL, ENG_FLD_BOX	, ENG_FLD_RASTERG, ENG_FLD_BARCODE, 
	ENG_FLD_LINE=10	 
} ; 


enum ENG_FIELD_OWNER { 
	ENG_REPORT_CONTAINER, ENG_PAGE_CONTAINER, ENG_CT_CELL 
} ;

enum {
	ENG_SECION_ID_EMPTY=-1L		//Id que pone VB en RootSection si no hay sección a la que enganchar
								//	util también para otras cosas, siempre en capa loader.
} ;

//----------------Enums especiales de carga

enum ENG_BAR_CODE_TYPE { 
	ENG_NO_BAR_CODE, ENG_BAR_CODE_39, 
	ENG_BAR_CODE_25_INTERLEAVED, ENG_BAR_CODE_128, ENG_BAR_CODE_POSTNET };

//----------------CT

enum ENG_CT_CELL_TYPE { 
	//Orden de los enum importante: marca el orden de impresión
	ENG_CT_PRE_T, ENG_CT_PRE_S, ENG_CT_SUB, ENG_CT_POST_S, ENG_CT_POST_T, 
	ENG_CT_TYPE_COUNT } ;

enum ENG_CT_MERGE_CNT { ENG_CT_NO_MERGE, ENG_CT_HEADER_MERGE, ENG_CT_ROW_MERGE } ;

enum ENG_CT_MERGE_PAGE { ENG_CT_NONE_PAGE, ENG_CT_FIRST_PAGE, ENG_CT_ALL_PAGES } ;

//Pos relativa a Cell estatica, eje Y
enum ENG_CT_CELL_Y_POS { ENG_CT_OVER_STC, ENG_CT_DOWN_STC, ENG_CT_ALIGN_STC } ;	
		
//Pos relativa a Cell estatica, eje X
enum ENG_CT_CELL_X_POS { ENG_CT_RIGHT_STC } ;						

//Alineacion de la celda de parametrizacion dentro de la de display, eje Y.
enum ENG_CT_CELL_Y_ALIGN  { ENG_CT_TOP_DISPLAY , ENG_CT_BOTTOM_DISPLAY, ENG_CT_CENTER_Y_DISPLAY } ;

//Alineacion de la celda de parametrizacion dentro de la de display, eje X.
enum ENG_CT_CELL_X_ALIGN  { ENG_CT_LEFT_DISPLAY, ENG_CT_RIGHT_DISPLAY,  ENG_CT_CENTER_X_DISPLAY } ;

//Tipos de Field (datos de parametrización) de una celda de display
enum ENG_CT_CELL_FIELDS_TYPE { ENG_TEMPLATE_H_FIELDS,ENG_TEMPLATE_R_FIELDS,ENG_STATIC_FIELDS } ;

enum ENG_FIND_ACTION { 
	ENG_TRY_FIND,			//Intenta busqueda, no da error encuentre o no.
	ENG_EXIST_REQUIRED,		//Intenta busqueda, error local si no encuentra
	ENG_NOT_EXIST_REQUIRED  //Intenta busqueda, error local si encuentra
} ;

//----------------Proceso carga/insatnciacion diseño

enum ENG_DVC_INTERBLOCK { 
	ENG_PCL_INTERBLOCK=2024, ENG_PSC_INTERBLOCK=2024, ENG_ESC_INTERBLOCK=2024, 
	ENG_RVW_INTERBLOCK=2024, ENG_HTML_INTERBLOCK=2024, ENG_PDF_INTERBLOCK=2024
} ;

enum ENG_LINK_DESIGN_MODE {
	ENG_BUILD_DESIGN,	//Defecto, el canal de diseño se levanta con ClMIT_Chn::Init("Diseño"), 
					//se cargan los parametros m_oReportId y m_iExecutionType, y se
					//hace un Load. El canal es filtrado. 
					//En el End se descarga y destruye todo.
	ENG_LINK_LOAD_DESIGN,//Se usa un canal ya levantado, con ClMIT_Chn::Init(m_oDesignContext),
					// y en el que ya se han cargado todos los parametros necesarios 
					// para hacer el Load. Se hace un Load, filtrando el canal.
					//en el End no se descarga ni destruye nada.
	ENG_LINK_DESIGN,	//Se usa un canal ya levantado, con ClMIT_Chn::Init(m_oDesignContext).
					//  Ya se supone que el Load esta hecho, por lo que el canal se usa
					//  directamente. Además, en el el nodo root ya debe estar 
					//	seleccionado el report deseado.
					//En el End no se descarga ni destruye nada.
	ENG_LINK_LOAD_UNLOAD_DESIGN, //Como LINK_LOAD_DESIGN haciendo Unload al salir.
	ENG_LINK_UNLOAD_DESIGN //Como LINK_DESIGN haciendo Unload al salir.
} ;



typedef list<m4uint32_t> ENG_ULongList_t ;	

//=====================================================================>ClENG_FileInterestList

class ClFileManager ;

class ClENG_FileInterestList
{

MIT_PRIVATE:

	StMIT_Inspector* m_poInsp;	//Simple link

	ClFileManager *m_poFileManager ;	//Simple acceso

	typedef list<ClMIT_Str> FileList_t;		//Lista de intereses declarados
	FileList_t m_oFileList ;

MIT_PUBLIC:

	//-------------------------Inicializaciones-----------------------------------------

	ClENG_FileInterestList();
	virtual ~ClENG_FileInterestList() { ClENG_FileInterestList::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_FileInterestList) ; }
	virtual void End() ;	//Hace un UnsetInterest()

	//Init normal.

	void Init() ;	

	//Init copia. Especial: hereda lista y VUELVE a delcarar interes sobre los ficheros.

	void Init(ClENG_FileInterestList *ai_poList) ;	

	//Declara interes sobre un fichero. Puede hacerse sobre el mismo varias veces: entrara
	//	varias veces en lista, y se liberara varias veces en UnsetInterest

	void DeclareInterest( m4char_t *ai_pcFile ) ;

	//Libera todos ficheros sobre los que se declaro interes
	
	void UnsetInterest() ;	

} ;

//=====================================================================>StFileParams

struct StFileParams {
	// path más raiz fija del fichero
	//m4char_t m_pcPath[MAX_FILE_NAME_SIZE];				
	ClMIT_Str m_oPath ;
	// número de caracteres que se reserva para secuencializar los ficheros
	// generados por break
	// Un valor negativo significa que se ponen justo los cracateres necesarios para 
	//	representar el numero. Un 0 inhabilita el poner ningun caracter.
	m4int16_t m_iNumOfSecChars;
	// número de chars reservados para generar ficheros por layout. Los valores pueden ser
	//	como en m_iNumOfSecChars, negativos 0 y positivo.
	m4int16_t m_iNumOfPageChars;
	// carácter separador entre caraceters de breaks y caracteres de pags
	// por defecto m_cSeparator = 0, es decir, un nulo. Servirá como flag
	// por si existe o no.
	m4char_t m_cSeparator ;
	// Extensión del fichero
	//m4char_t m_pcExtensionName[MAX_FILE_EXTENSION_SIZE];
	ClMIT_Str m_oExtension ;
	// índice por el que debe empezar la secuencialización de breaks
	// por defecto = 0
	m4int16_t m_iOffSet ;

	// flag que indica si es file o path, porque si es file
	// habrá que ejecutar la expresión y luego concatenarle el fichero
	// de trabajo
	m4bool_t m_bIsFile ;

	// flag que indica si debe ignorar los valores preferentes de la ruta o del fichero
	// (de forma efectiva sirve para indicar si se deben obviar estos parámetros en la
	// definición del informe)
	m4bool_t m_bIgnoreFilePathPref ;

	//  Para manejar la capacidad de borrar ficheros breaks. No tiene sentido, ni se
	//usa, si m_iNumOfSecChars es 0. Si esta activa esta capacidad, a la hora de cerrar 
	//un fichero, se testea si alguna capa ha puesto el flag de borrado de break. Si es
	//asi, borra el fichero.
	//	m_bUseDelBreak dice si se usa esta capacidad, m_bDefaultDelBreak es el valor por
	//defecto del flag de borrado de break, y m_bConsecutiveBreaks dice si, en caso de
	//que haya ficheros borrados, los siguientes son numerados consecutivamente o no.
	//  El flag es actualizado con m_bDefaultDelBreak tras abrir los ficheros del nuevo
	//break y, preventivamente, en el Init.

	m4bool_t m_bUseDelBreak, m_bDefaultDelBreak, m_bConsecutiveBreaks  ;
		
	// Root Path del Web Server para devolver el fichero de salida
	ClMIT_Str m_oWebPath;
	// Fichero de definicion del menu de navegacion HTML
	ClMIT_Str m_oHTMLMenuFile;

	// Flags de valor cargado explicitamente
	m4bool_t m_bPathLoaded, m_bNumOfSecCharsLoaded, m_bNumOfPageCharsLoaded,
		m_bSeparatorLoaded, m_bExtensionLoaded, m_bOffSetLoaded, m_bIsFileLoaded,
		m_bUseDelBreakLoaded, m_bDefaultDelBreakLoaded, m_bConsecutiveBreaksLoaded,
		m_bWebPathLoaded, m_bHTMLMenuFileLoaded;

	// Nombre de fichero en función de una expresión (reevaluable cuando hay rupturas)
	ClMIT_Str m_oFileNameExpression;
	// Acceso al canal de datos para evaluación de expresiones
	ClMIT_Chn *m_poChnData ;

	StFileParams() { Init() ; }
	virtual ~StFileParams() ;

	void Init(const StFileParams * ai_From ) ;

	// D:Todo a 0, cadenas vacias, m_bIsFile a TRUE, m_iNumOfPageChars a -1
	// m_bUseDelBreak, def a M4_FALSE, m_bDefaultDelBreak a M4_TRUE y 
	//		m_bConsecutiveBreaks a M4_TRUE

	void Init() ;

	// Carga los valores de la siguiente forma:
	//	-Si no se cargo valor en ningun objeto, se usa el defecto.
	//	-Si solo en uno de los dos objetos se cargo valor, se usa ese valor.
	//	-Si se cargo el valor en los dos objetos tiene precedencia el valor de A.
	//	-Todos flags de Loaded se ponen a true, excepto si en ningun objeto habia valor,
	//		en cuyo caso se queda almacenado el valor defecto.
	//  -Se controla de forma especial el m_iNumOfSecChars, en el caso de poner defecto. 
	//		El fefecto es el que le pasemos.
	void Init( const StFileParams * ai_A, const StFileParams * ai_B, m4int16_t ai_DefNumOfSecChars ) ;
};
 

#endif


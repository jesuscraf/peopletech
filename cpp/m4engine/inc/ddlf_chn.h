//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             DDLF_Chn.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/31/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	David Fernández 15/05/98
//
// Definition:
//
//    Cargador de diseños almacenados en el canal
//
//
//==============================================================================
#ifndef __DDLF_CHN_H__
#define __DDLF_CHN_H__

class ClENG_DesignData ;
struct StENG_DD_Defaults ;
struct StENG_DD_FieldData ;
class ClENG_StorageDep ;

#include "clformat.h"
#include "designdt.h"
#include "xapichn.h"
#include "clfile.h"

#include "m4stl.hpp"

//NOTA IMPORTANTE: El rango de 5.000 al 15.000 NO DEBE SER USADO POR EL ENGINE.
//Esta reservado para componentes ajenas que quieran aprovechar el canal de diseño.

class ClENG_DDLF_Chn
{
MIT_PRIVATE:

	StMIT_Inspector* m_poInsp;	//Simple link

MIT_PUBLIC:

	// Compartidas por todas tablas con properties

	enum DEFAULT_PROPERTIES {

		DEFAULT_ID_FONT=0,						//Numero de fuente
		//DEFAULT_ID_FORMAT=1,						//Numero de formato
		DEFAULT_ID_ALIGNX=2, DEFAULT_ID_ALIGNY=3, 	//ClENG_FieldStr::ALIGN
		DEFAULT_CLIP=4,							//Boolean
		DEFAULT_GRAPH_PATH=5,				//Boolean
		DEFAULT_DISPLAY_NULL=6,				//Cadena
		DEFAULT_FORMAT_NULL=7,				//Boolean
		DEFAULT_ID_COLOUR=8,				//Id (Numero) del Colour

		DEFAULT_LIMIT=100 
	} ;

	enum REPORT_PROPERTIES { 

		RPT_PROP_BEGIN=DEFAULT_LIMIT,
		ROOT_SYNCHRONIZE=101,
		BREAK_MODE=102,
		BREAK_SECTION=103,
		TOTALIZE_COUNT=105,
		CT_COUNT=106,
		// tienen precedencia a los parámetros
		FILE_PATH=200,
		FILE_NAME=201,
		FILE_NUMSECCHARS=202,
		FILE_NUMPAGECHARS=203,
		FILE_SEPARATOR=204,
		FILE_EXTENSION=205,
		FILE_OFFSET=206,
		USE_DEL_BREAK=207,
		DEFAULT_DEL_BREAK=208,
		CONSECUTIVE_BREAKS=209, 
		EXECUTION_PARS=210,
		IGNORE_TXT_WARNING=211  //Solo para la Warning de String Overlapping del DvcTXT
	} ;

	enum PAGE_PROPERTIES { 

		PAGE_PROP_BEGIN=DEFAULT_LIMIT

	} ;

	enum CT_PROPERTIES { 

		CT_PROP_BEGIN=DEFAULT_LIMIT,
		CT_PROP_MERGE_PAGE=101,
		CT_PROP_XCOLUMN_LEFT=102,
		CT_PROP_XCOLUMN_RIGHT=103,
		CT_PROP_TOG_LEVEL=104,
		CT_DERIVED_CT_LIST=105,
		CT_PROP_WIZARD_DATA=106,
		CT_ROW_PROPS=5000,
		CT_COL_PROPS=5100,
		CT_TOT_PROPS=5200,
		CT_LIMIT=5500
	} ;

	enum CONTAINER_PROPERTIES {

		CNT_PROP_BEGIN=DEFAULT_LIMIT,

		CNT_LOWER_LIMIT=101,			//Para Cnt no fijos.
		CNT_CT_MERGE=102,

		CNT_REGION_BORDER=650,
		CNT_REGION_COLOUR=651
	} ;

	enum FIELD_PROPERTIES { 

		FLD_DESIGN_BEGIN=DEFAULT_LIMIT,		//----------Zona fields FLD_FROM_DESIGN_CHN

		FLD_DESIGN_OPTIMIZE=103,	//Boolean
		FLD_DESIGN_STATIC=104,	//Boolean
		FLD_ID_CT_FIELD=105,
		FLD_INDEX_LEVEL=106,
		FLD_INDEX_EXPR=107,
		FLD_HYPERLINK=108,

		FLD_RASTER_MAP_BEGIN=200,	    //-----Zona FLD_RASTER_MAP
		FLD_RASTER_TEMP_WEB_IMAGE=201,  //Boolean

		FLD_STRING_BEGIN=300,	//----------Zona FLD_STRING		
		FLD_FILL_MODE=301,
		FLD_FILL_CHAR=302,

		FLD_MULTIL_BEGIN=350,	//----------Zona FLD_MULTIL		
		FLD_MULTIL_INTERL=351,
		FLD_MULTIL_VAR_HEIGHT=352,   //Boolean
		FLD_MULTIL_JUSTIFIED=353,    //Boolean

		FLD_BOX_BEGIN=400,		//----------Zona FLD_BOX		
		FLD_MAX_BOXES=401,
		//FLD_FILL_MODE y FLD_FILL_CHAR			Las mismas que en la zona FLD_STRING

		FLD_BARCODE=500,		//----------Zona FLD_CODEBAR   
		FLD_BARCODE_TYPE=501,	

		FLD_LINE_BEGIN=600,		//----------Zona FLD_LINE
		FLD_LINE_ADJUST_TO_CONTAINER=601,


		FLD_COMMOM_BEGIN=649,	//----------Zona common a todos Field
		FLD_REGION_BORDER,
		FLD_REGION_COLOUR,

		FLD_WRAPVB_BEGIN=3000,	//------------WrapperVB
		FLD_SAVE_NODE=3001,
		FLD_SAVE_ITEM=3002,
		FLD_DUPLICATED=3003,
		// ISA
		FLD_ID_PI = 3004   //-------------- Identificador de PI (si Pi= -1 ) no existe
	} ;

	enum SECTION_PROPERTIES {	

		SEC_PROP_BEGIN=DEFAULT_LIMIT,

		SEC_TOGETHER=DEFAULT_LIMIT+2,	//Seccion final, Boolean, OPCIONAL. Defecto false 

		SEC_SLICE_BEGIN=DEFAULT_LIMIT+100,
		SEC_SLC_GUIDE_NODE=204,
		SEC_SLC_GUIDE_ITEM=205,

		SEC_NULL_ON_SONS=215,		//Boolean
		SEC_ONE_ONE=216	,			//Cadena

		SEC_CROSS_ROW=217,			//Actualmente solo lo usa el CT Wizard
		SEC_CROSS_COLUMN=218		//Actualmente solo lo usa el CT Wizard
	} ;

	enum FONT_PROPERTIES {

		PCL_PROP_BEGIN=0,

		PCL_HEIGHT,
		PCL_PITCH,
		PCL_TYPEFACE,
		PCL_SYMBOLSET,
		PCL_MAP_CHARSET,
		PCL_MAP_FAMILY,
		PCL_MAP_FACENAME,

		ESC_PROP_BEGIN=100,
		ESC_PITCH,
		ESC_DOUBLEX,
		ESC_DOUBLEY,
		ESC_COMPR,
		ESC_TYPEFACE,
		ESC_MAP_CHARSET,
		ESC_MAP_FAMILY,
		ESC_MAP_FACENAME,

		HTML_PROP_BEGIN=200,
		HTML_FONT_SIZE

	} ;

MIT_PRIVATE:

	ENG_LINK_DESIGN_MODE m_LinkDesign ;

	ClMIT_Chn	m_oChnDef ;
	ClMIT_ChnNode  
		* m_poNodeRpr, * m_poNodeRprProp, * m_poNodeFont, 
		* m_poNodeCT, *m_poNodeCTProp, *m_poNodeLongExpression,
		* m_poNodeFormat, * m_poNodeSec, * m_poNodeSecCnt, * m_poNodeSecCntProp, 
		* m_poNodeSecCntFld, * m_poNodeSecCntFldProp, 
		* m_poNodeSecProp, * m_poNodeSecSon,
		* m_poNodePage, * m_poNodePageProp, * m_poNodePageCntFld, * m_poNodePageCntFldProp,
		* m_poNodeMasterFont, * m_poNodeMasterFormat, *m_poNodeExe,
		* m_poNodeColour, * m_poNodeMasterColour, *m_poReportString ; 

	// Tiempo en preparar el diseño (en levantar el canal de diseño+load+pack).

	m4uint32_t m_oTimerUpDesignValue ; 


	// Auxiliar, relacion NombreItemCanal - NombrePropertieM4Fmt32

	static struct StAuxFormat {
		m4char_t *m_pcItem ;
		m4char_t *m_pcProp ;
	} const m_oAuxFormat[ClENG_Format::MAX_NUM_PROPS] ;

	ClMIT_Str m_oChannelID;

MIT_PUBLIC:

	//--------------------------------------------------------------------->Inicializaciones

	ClENG_DDLF_Chn();
	virtual ~ClENG_DDLF_Chn() { ClENG_DDLF_Chn::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_DDLF_Chn) ; }


	//Levanta o se engancha al canal y prepara los nodos de acceso.

	m4bool_t Init( 
		m4char_t *ai_pcReportId, m4int16_t ai_iExecutionType,
		ENG_LINK_DESIGN_MODE ai_LinkDesign, m4int16_t ai_iMetadataLang,
		m4char_t *ai_pcChannelID, ClAccess *ai_poAccess=NULL,
		ClMIT_Chn::AUTOLOAD_MODE ai_AutoLoadMode=ClMIT_Chn::AUTOLOAD_OFF,
		m4bool_t ai_bPack=M4_FALSE,	ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ; 

	//Realiza las tareas necesarias para desengancharse del canal de diseño.

	virtual void End() ;

	//--------------------------------------------------------------------->Get & Set

	ClMIT_Chn* GetDefChn() { return &m_oChnDef; }
	
	//--------------------------------------------------------------------->Cargas

	// Resetea y carga en ao_poDD los datos GlobalData, las Fonts, Formatos, Colores,  
	//	datos de Pagina Layout, LE. También resetea la lista de CT (por si no vamos a usar
	//	LoadAllCT).

	void LoadBaseData( ClENG_DesignData *ao_poDD ) ;

	//Carga todas las secciones, con sus datos SonSection.

	void LoadAllSections( ClENG_DesignData *ao_poDD ) ;

	//Carga todo el diseño, empaqueta las llamadas anteriores

	void LoadAll(ClENG_DesignData *ao_poDD) ;

	//Simplemente resetea la lista de CT y hace el LoadCT para todos los CT.

	void LoadAllCT(ClENG_DesignData *ao_poDD) ;

	// Resetea y carga en ao_poDD los datos del CT pasado. FALSE si no lo encontró.
	// Por dependencias de este modulo, debe haberse llamado ya a LoadAllSections, pues lo
	//	que se hace es ir extrayendo -y borrando- de DD las secciones de parametrización, para
	//	formar la template.

	m4bool_t LoadCT( ClENG_DesignData *ao_poDD, m4char_t *ai_pcCTId ) ;

	// Cargan una Font/Format/Colour en concreto, por su Id. Falla si en en canal no se encuantra el Id.
	// En caso de fallo, disparan un error.

	m4bool_t LoadFont(  ClENG_DesignData *ao_poDD, m4uint32_t ai_IdFont ) ;
	m4bool_t LoadFormat( ClENG_DesignData *ao_poDD, m4uint32_t ai_IdFormat ) ;
	m4bool_t LoadColour(  ClENG_DesignData *ao_poDD, m4uint32_t ai_IdColour ) ;


	//---------------------------------------------------------------->Synchronize

	// Por ahora muy puenteado. 
	// Actualiza en canal un CT. Estas son las condiciones de ejecucion:
	//	-SOLO recorre y procesa componentes del CT en cuestion.
	//	-Persiste lo siguiente:
	//		-CTData. Solo ha sido MODIFICADO, no se ha creado ni borrado de canal.
	//		-CTCell. Se han podido CREAR, BORRAR y MODIFICAR. Lo que NO se ha podido crear ni
	//			borrar son las secciones de parametrización sobre las que persisten.
	//			-FieldData. Fields de la CTCell. Se han podido CREAR, BORRAR y MODIFICAR.
	//		-CntData. Solo se han podido MODIFICAR, y solo cuentan Cnt pertenecientes a 
	//			secciones de CT. Lo que NO se ha podido crear ni borrar ni modificar son las 
	//			secciones sobre las que persisten.	
	//			-FieldData. Fields del Cnt. Se han podido CREAR, BORRAR y MODIFICAR. 

	m4bool_t SynchronizeCT( m4char_t *ai_pcCT, ClENG_DesignData *ao_poDD ) ;

	//Sincroniza la relacion de Fonts/Formatos usados por el report. Es decir, NO mira cambios
	//	en las BD maestras de Fonts/Formatos, pero si sincroniza completamente la lista de
	//	IdReport-IdFont/IdFormato.

	void  SynchronizeFontsUsedByReport(ClENG_DesignData *ao_poDD) ;
	void  SynchronizeFormatsUsedByReport(ClENG_DesignData *ao_poDD) ;
	void  SynchronizeColoursUsedByReport(ClENG_DesignData *ao_poDD) ;

	//---------------------------------------------------------------->EXTRACCION DE LITERALES

	//Funcion auxiliar que trocea la expresion en literales traducibles y actualiza a nivel de canal
	void ExtractString(const m4char_t* ai_pcString, ClMIT_Str *ai_poModifiedStr, const m4char_t* ai_pcIDReport);
	// Guarda el literal traducible a nivel de canal
	void StoreExtractedString(const m4char_t *ai_pcString, ClMIT_Str* ao_poID, const m4char_t* ai_pcIDReport);

	//Ejecuta el proceso de extraccion de literales
	// recorriendose los nodos del canal de datos
	m4bool_t ExtractLiterals();

	// Extraccion de literales por nodo
	m4bool_t ExtractReportProps();
	m4bool_t ExtractSectionProps();
	m4bool_t ExtractContainerProps();
	m4bool_t ExtractCntFldStrings();
	m4bool_t ExtractLongExpressions();
	m4bool_t ExtractPageCntFldStrings();

	//Graba en los registros apropiados las propiedades de la cadena serializada
	m4bool_t SetCTProps(m4char_t *ai_pcCTId, m4char_t* ai_pcCTProps);
	m4bool_t GetCTProps(m4char_t *ai_pcCTId, m4char_t* ai_pcCTProps);

MIT_PRIVATE:

	//---------->Auxiliares carga

	void LoadDefault( StENG_DD_Defaults *ao_poDef, 
		ClMIT_ChnNode *ai_poFieldNode, ClMIT_ChnNode *ai_poPropNode ) ;

	void LoadField( 
		ClENG_DesignData *ao_poDD,
		StENG_DD_FieldData *ao_pstData, 
		ClMIT_ChnNode *ai_poFieldNode, ClMIT_ChnNode *ai_poPropNode) ;

	//Mete una nueva Font en la lista.
	//Simplemente, dejar posicionado m_poNodeFont en la font que se desea meter.

	void LoadFont(ClENG_DesignData *ao_poDD) ;

	//Como LoadFont, pero para formatos y colores.
	//Dejar posicionado m_poNodeFormat m_poNodeColour.

	void LoadFormat(ClENG_DesignData *ao_poDD) ;
	void LoadColour(ClENG_DesignData *ao_poDD) ;

	//---------->Auxiliares Synchronize

	//Operacion de sincronizacion.
	//
	//Por sincronizacion se entiende reflejar en el canal de diseño lo que tenemos en memoria,
	//	en DD.
	//Esto significa:
	//	-Registros que esten en canal, pero no en DD: marcar como borrado en canal.
	//	-Registros que esten en canal y en DD, pero difieran en contenido: modificar canal.
	//	-Registros que no esten en canal pero si en DD: crear y actualizar registro en canal.
	//Los metodos Synchronize borran los registros que preoceda, crean registros -metiendo PK
	//	si hace falta y reseteando valores- y llaman a los metodos Save, y modifican los 
	//	registros pertinentes llamando a los metodos Save.
	//Los metodos Save ya tienen de entrada los registros posicionados. Simplemente graban los
	//	datos que se hayan modificado.

	//Auxiliar de propiedades, contiene clave propiedad-contenido propiedad.

	typedef map<m4uint32_t,ClMIT_Str, less<m4uint32_t> > PropertieList_t;

	//Ojo, debria llamarse una vez con todos los datos a grabar insertados. Los que no lo 
	//	esten seran borrados.

	void SynchronizeProperties( ClMIT_ChnNode *ai_poPropNode, PropertieList_t *ai_poList, m4bool_t SwCT=M4_FALSE ) ; 	

	//Llama a SaveField
	m4bool_t SynchronizeField( ENG_DD_FieldList_t *ai_poFieldList, 
		ClMIT_ChnNode *ai_poFieldNode, ClMIT_ChnNode *ai_poPropNode ) ; 	

	//Llama a SaveCell
	m4bool_t SynchronizeCTCell( StENG_DD_CTData *ai_poCT ) ;
	
	//Sincroniza Cnts (solo MODIFICACION) y sincroniza sus fields (todas operaciones).
	//Recursivo, recorre rama.

	m4bool_t SynchronizeSectionCnt( m4uint32_t ai_uiIdSection, ClENG_DesignData *ao_poDD ) ;

	//Graba en el registro posicionado TODOS los datos que esten marcados como cargados, y
	//	las properties asociadas.

	void SaveField(ClMIT_ChnNode *ai_poFieldNode, ClMIT_ChnNode *ai_poPropNode, 
		StENG_DD_FieldData *ai_poData ) ;

	//Graba el IdFormat en el reg posicionado, y añade a la lista las properties pertinentes.

	void AddDefaultProperties( ClMIT_ChnNode *ai_poMasterNode, 
		PropertieList_t *ai_poList, StENG_DD_Defaults *ai_poData ) ;

	//Añade a la lista las propiedades de alineacion de celdas.
	void AddCellAlignProps( ClMIT_ChnNode *ai_poMasterNode, 
		PropertieList_t *ai_poList, StENG_DD_CTCell *ai_poCntData ) ;

	//Graba en el registro posicionado TODOS los datos que esten marcados como cargados, y
	//	las properties asociadas.

	void SaveCTData(StENG_DD_CTData *ai_poData) ;

	//Graba en el registro posicionado TODOS los datos que esten marcados como cargados, y
	//	las properties asociadas.

	void SaveCntData(StENG_DD_CntData *ai_poData) ;

	//Graba en el registro posicionado TODOS los datos que esten marcados como cargados, y
	//	las properties asociadas.

	void SaveCellData(StENG_DD_CTCell *ai_poData) ;
};


//===========================================================================>ClENG_StorageDepChn

class ClENG_StorageDepChn : public ClENG_StorageDep
{

MIT_PRIVATE:

	StMIT_Inspector* m_poInsp;	//Simple link

MIT_PUBLIC:

	//-------------------------Inicializaciones-----------------------------------------

	ClENG_StorageDepChn() ;
	virtual ~ClENG_StorageDepChn() { ; }
	virtual size_t GetSizeof() { return sizeof(ClENG_StorageDepChn) ; }

	virtual m4bool_t Init(ClMIT_Chn *ai_poChn);

	//-------------------------Storage-----------------------------------------------------

	// Guarda la dependencia de Item en la tabla de dependencias
	m4bool_t StoreItemDependency(const m4char_t *ai_pcIdTI, const m4char_t *ai_pcIdItem);

	//Guarda la dependencia de Nodo en la tabla de dependencias
	m4bool_t StoreNodeDependency(const m4char_t *ai_pcIdT3, const m4char_t *ai_pcIdNode);

	//Borra todos los registros del recordset
	m4bool_t DeleteAllDependencies();

//-------------------------Miembros-------------------------------------------------

MIT_PRIVATE:

	ClMIT_Chn	*m_poChnDef ; //Puntero al canal de definicion

	ClMIT_File   m_oFile;

	ClMIT_ChnNode * m_poNodePageFieldDep, * m_poNodeSectionDep, 
				  * m_poNodeContainerDep, * m_poNodeContainerFieldDep,
				  * m_poNodePageFieldNodeDep, * m_poNodeSectionNodeDep,
				  * m_poNodeContainerNodeDep, * m_poNodeContainerFieldNodeDep,
				  * m_poNodeReportNodeDep, * m_poNodeReportDep,
				  * m_poNodeSec, * m_poNodeSecCnt, 
				  * m_poNodeSecCntFld, * m_poNodePageCntFld;
} ;


#endif


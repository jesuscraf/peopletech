//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             ct_templ.h
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

#ifndef __CT_TEMPL__H__
#define __CT_TEMPL__H__

#include "designin.h"


class ClENG_DesignTree ;
struct StENG_DT_CTData ;
struct StENG_DD_CTData ;
struct StENG_DT_CTCell ;
struct StENG_DT_PageData ;
struct StENG_DT_FieldData ;
struct StENG_DT_CntData ;
struct StENG_DT_SectionData ;
struct StENG_CellList ;	
class  ClENG_CT_T ;
class  ClENG_CntDisplay ;
struct StExpressionEnv ;
class ClENG_VB_Serialize ;
class ClENG_KeyTot ;
class ClENG_SessReportDesigner ;
class ClENG_CntDisplayBuilder ;

#include "m4stl.hpp"

//MUY IMPORTANTE:
//
//	-La resolucion de fusion de container CT se lleva a cabo en el DT, por ello, 
//		para realizar esta tarea SOLO se conocen datos de DT, NUNCA nada a nivel de ClReport.
//		Esta independencia es importante para procesos de visualizacion en el designer.
//	-La clase ClENG_CT_T (template de CT) gestiona TODA esta parte. Por ello, esta restringida
//		a operaciones en el ambito de DT, no sabe NADA de ClENG_CT. 
//	-El ciclo de vida de estos objetos esta ligado al DT que se usa para generarlos.
//	-Documentación sobre el funcionamiento: en CT_comportamiento.doc

//=======================================================================ClENG_CT_T (template)

class ClENG_CT_T {

MIT_PRIVATE:

	StMIT_Inspector* m_poInsp;	//Simple link

	StENG_DT_CTData *m_poCT ;	//Link al CT propietario

	m4int16_t m_iLevelCount ;	//Numero de niveles del CT

	//Array dinámico, de m_iLevelCount*ENG_CT_TYPE_COUNT de punteros a celdas.
	//Preguntar por m_bLoaded antes de usar.

	StENG_DT_CTCell *m_poCell;	

	//Array dinámico, alocado a m_iLevelCount.
	//Contiene la altura de level, calculado en Init. 0, no hay cell en el nivel.

	DvcUnit_t  *m_poLevelHeight ;		

	//Array dinámico, alocado a m_iLevelCount.
	//Contiene la seccion de parametrizacion correspondiente al nivel en cuestion

	m4uint32_t *m_puiIdParamSec ;

MIT_PUBLIC:

	// ----------------------------- Inicialización---------------------------------------	

	ClENG_CT_T();
	virtual ~ClENG_CT_T() { ClENG_CT_T::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_CT_T) ; }
	virtual void End() ;	

	//-------------->Constructores y sobrecargas para STL

	ClENG_CT_T(const ClENG_CT_T & ai_oSource)  ;
	ClENG_CT_T & operator = (const ClENG_CT_T &ai_oOpe) 
		{ Init(&ai_oOpe) ; return *this ;} 

	void Init(const ClENG_CT_T *ai_poSource) ;

	//Inicializa la template, segun los datos pasados
	//El ai_poCT propietario ya debe estar inicializado.

	m4bool_t Init( 
		StENG_DT_CTData *ai_poCT_Owner, StENG_DD_CTData *ai_poCT_DD, 
		ClENG_DesignTree *ai_poDT, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR
	) ;

	//Usar solo para consulta. 
	//Si Level incorrecto, error local y da NULL.

	StENG_DT_CTCell * GetCell( m4int16_t ai_Level, ENG_CT_CELL_TYPE ai_CntType ) const ;

	DvcUnit_t GetLevelHeight( m4int16_t ai_Level ) {  return m_poLevelHeight[ai_Level] ; }

	//Obtiene el alto acumulativo de un nivel: es decir, para el nivel 0, suma todos los
	//	altos de nivel, para el 1 todos menos el nivel 0, para el 2 todos menos el 0 y 1, ect.

	DvcUnit_t GetAccLevelHeight( m4int16_t ai_Level ) ;

	//Solo uso, CT propietario

	StENG_DT_CTData *GetCT() { return m_poCT ; } 

	//Por comodidad, equivalente a GetCT()->m_iLevelCount

	m4int16_t GetLevelCount() { return m_iLevelCount; }

	//NULL o Field en cuestion.

	StENG_DT_FieldData	* FindIdCTField( ENG_FIND_ACTION ai_Action, m4char_t *ai_pcIdCtField ) ;

	//Saca el contenido a la traza

	void Trace() ;

	//Ver CountFontFormatColourInstances en DesignTree, auxiliar de este. 
	// Esta funcion solo incrementa si debe. Los valores ya deben estar reseteados de antes.
	//	Los punteros pueden ser NULL.

	void IncrCountFontFormatColourInstances( 
		m4uint32_t ai_uiIdFont, m4uint32_t ai_uiIdFormat, m4uint32_t ai_uiIdColour,
		m4uint32_t *aoi_puiFontCount, m4uint32_t *aoi_puiFormatCount,
		m4uint32_t *aoi_puiColourCount ) ;


	//Crean un Id unico por template CT.
	//Ojo, crean el Id unico en funcion de lo que contiene la template. Es decir, n llamadas
	//	sucesivas a GenerateNewCTIdField dan el mismo resultado. Si se quieren crear varios
	//	Fields, hay que hacer un GenerateNewCTIdField, meter el Field en la lista, y luego
	//	pedir el siguiente GenerateNewCTIdField para que de claves diferentes.

	void GenerateNewCTIdField( ClMIT_Str *ao_poStr ) ;

} ;

//=======================================================================ClENG_DisplayCell
//
//Contiene información de una instancia concreta de un Cnt, fusionado o no con un CT.


class ClENG_DisplayCell {

	friend class ClENG_CntDisplay ;
	friend class ClENG_CntDisplayBuilder ;

MIT_PRIVATE:

	StMIT_Inspector* m_poInsp;	//Simple link

	ClENG_CntDisplay *m_poOwner ;	//Simple link

	//Segun este valor:
	//
	//-ENG_TEMPLATE_H_FIELDS (modo Header). 
	//		-Datos parametrizacion en la celda de template m_poCT / m_iLevel / m_Cell
	//		-Deben salir todos los fields de dicha celda de template.
	//
	//-ENG_TEMPLATE_R_FIELDS (modo Row). 
	//		-Datos parametrizacion en la celda de template m_poCT / m_iLevel / m_Cell
	//		-Se recorren los fields de la celda de template CTId / Level / Cell. Por cada 
	//			field, se busca por su Id si existe tambien en los fields de 
	//			IdSection/IdContainer. Si es asi, el field sale, tomando como datos base los 
	//			de IdSection/IdContainer, aunque heredando el Left y Width de template.	
	//-ENG_STATIC_FIELDS	 (modo Static). 
	//		-Datos parametrizacion en m_poCnt
	//		-Deben salir todos los fiels del container normales, NO de fusion.

	ENG_CT_CELL_FIELDS_TYPE m_FieldsType ;	

	m4int16_t  m_iLevel ;
	ENG_CT_CELL_TYPE m_Cell ;
		
	//Si TRUE, la celda NO tiene ningún field, simplemente se procesan regiones

	m4bool_t m_bDummy ;
	
	//Datos celda display, tomando como (0,0) la esquina superior izda del container.

	DvcUnit_t m_Top, m_Left, m_Height, m_Width ;	

	//Offset a aplicar a los Field de la celda de parametrizacion, resultado de 
	//	aplicar la alineacion de la celda de parametrizacion dentro de la de display.

	DvcUnit_t m_FieldYOffset, m_FieldXOffset ;

	StENG_CellList *m_poSon ;	//Declarado asi para evitar definiciones recursivas

	//Auxiliar para el proceso de instanciar cells.
	//Contiene o una cadena con la secuencia "IndexLevel0_IndexLevel1_..._LastIndex" resuelta.

	ClMIT_Str m_oCTIndex ;

	//El resto de parametros de field son los de parametrización.

MIT_PUBLIC:

	ClENG_DisplayCell() ;
	virtual ~ClENG_DisplayCell() { ClENG_DisplayCell::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_DisplayCell) ; }
	virtual void End() ;	//Borra también la rama hija
	void Init(ClENG_CntDisplay *ai_poOwner) ;

	//-------------->Constructores y sobrecargas para STL

	ClENG_DisplayCell(const ClENG_DisplayCell& ai_oSource) ;
	ClENG_DisplayCell & operator = (const ClENG_DisplayCell &ai_oOpe) ;

	void Init(const ClENG_DisplayCell *ai_poSource) ;

MIT_PRIVATE:

	void Trace() ;

	//Ver doc serializacion VB
	//Recursiva
	void Serialize( ClENG_VB_Serialize *ai_poSerial ) ;

	//Auxiliar de ClENG_CntDisplay::CreateCntData, recursiva.
	//En una primera pasada se añaden los Fields que simulan celdas, deben ir todos juntos al
	//	principio del Cnt. 
	//Una segunda llamada añade los Fields.
	//ai_pstEnv y ai_IdIndexFather se pasan para el PrepareForAnalexCT, ver esta funcion.
	//	En la llamada raiz deben estar una con el m_poReport inicializado, y el otro con "".

	m4bool_t AddFields( 
		ClENG_DesignTree *ai_poDT, StENG_DT_CntData *ao_poCntData, 
		m4bool_t ai_bSwAddFields, m4bool_t ai_bSwTestCell ) ; 

} ;
	
typedef list<ClENG_DisplayCell> ENG_CellList_t;	
struct StENG_CellList {
	ENG_CellList_t m_oList ;
} ;


//=======================================================================ClENG_CntDisplay

class ClENG_CntDisplay {

	friend class ClENG_CntDisplayBuilder ;

MIT_PRIVATE:
	
	StMIT_Inspector* m_poInsp;	//Simple link


	//Link al container de  seccion que se usó para generar la instancia. 
	//Una instancia SIEMPRE debe de generarse via container.

	m4uint32_t m_uiIdSection ;
	ENG_TYPE_CONTAINER m_TypeCnt ;
	StENG_DT_CntData *m_poCnt ; 

	//Simple Link. 
	//-Modo NO_MERGE, NULL.
	//-Resto de los modos, NO puede ser NULL, apunta al template CT con el que se fusiona.

	ClENG_CT_T *m_poCT_T ;	

	struct StPage {

		//Medidas del container de display final, pueden variar respecto de las originales.
		//El resto de parametros -incluido regiones- no varia, es el del container. 

		DvcUnit_t m_Top, m_Left, m_Height, m_Width ;

		//Lista Cells de la pagina en cuestion (cada una puede tener ramas hijas).

		ENG_CellList_t m_oLevel0 ;

		//De 0 a n. Varia en funcion de la fusion. Si es fusion, es calculado, si no 
		//	es el que originalmente tiene los datos de Cnt.

		m4int16_t m_iPage ;

		//-------------->Constructores y sobrecargas para STL
		
		StPage() { StPage::End() ; }
		virtual ~StPage() { StPage::End(); }

		StPage(const StPage& ai_oSource) { Init(&ai_oSource) ; } 
		StPage & operator = (const StPage &ai_oOpe) { Init(&ai_oOpe) ; return *this ;} 

		void Init(const StPage *ai_poSource) ;
		void End() ;	
	} ;
	
	//Lista de containers generados, 1 por pagina layout. Es decir, el tamaño de esta lista es
	// el numero de paginas layout creadas. Influye el tamaño del CT, de la página, y m_Break.
	//Como minimo debe haber 1 container.

	typedef list<StPage> PageList_t;	
	PageList_t m_oPage ;	


MIT_PUBLIC:

	// ----------------------------- Inicialización---------------------------------------	

	ClENG_CntDisplay();
	virtual ~ClENG_CntDisplay() { ClENG_CntDisplay::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_CntDisplay) ; }
	virtual void End() ;	

	//-------------->Constructores y sobrecargas para STL

	ClENG_CntDisplay(const ClENG_CntDisplay & ai_oSource) ;
	ClENG_CntDisplay & operator = (const ClENG_CntDisplay &ai_oOpe) ;
	void Init(const ClENG_CntDisplay *ai_poSource) ;

	//Construye un Cnt de display.
	//Solo si ai_MergeMode=NO_MERGE, ai_poCT y ai_poDataTree pueden ser NULL , pues el Cnt de 
	//	display se construye por completo basándose en el container de seccion. De hecho,
	//	internamente en este modo es como si se hubiesen pasado a NULL.
	//Si ai_MergeMode!=NO_MERGE, el resultado es una combinación de los datos del container
	//	seccion y de la plantilla de CT.
	//Si por cualquier motivo NO se pasa ai_poCT_T ó ai_poDataTree y el es de fusión, se
	//	generará error.
	//ai_poDataTree suministra el arbol de claves con el numero de instancias a generar. 
	//ai_iTogLevel es el nivel Together. Puede pasarse un valor 'normal', (1 a n ó 0 para 
	//	indicar que no hay TogLevel), o los valores especiales:
	//	-TOG_LOAD_FROM_DESIGN. Usa el ai_iTogLevel original del CT.
	//	-TOG_NO_LIMIT. Nunca se generará más de una página, es como si tuviesemos una de 
	//		anchura infinita.
	//En caso de que se vaya a fusionar el container, se testea que el numero de niveles del
	//	ai_poDataTree y ai_poCT_T coinciden y son mayores de 1.
	
	enum { TOG_LOAD_FROM_DESIGN=-20, TOG_NO_LIMIT=-21 } ;
	
	m4bool_t Init( 	
		m4uint32_t ai_uiIdSection, ENG_TYPE_CONTAINER ai_TypeCnt, StENG_DT_CntData *ai_poCnt, 
		ClENG_CT_T *ai_poCT_T, 
		ClENG_KeyTot *ai_poDataTree, m4int16_t ai_iTogLevel) ;

	//----------------------------------------Gets-------------------------------------------

	m4int16_t GetPageCount() { return m_oPage.size(); }
	ClENG_CT_T * GetTemplate() { return m_poCT_T; }
	m4uint32_t GetIdSection() { return m_uiIdSection ; }
	ENG_TYPE_CONTAINER GetCntType() { return m_TypeCnt ; }
	StENG_DT_CntData * GetCntData() { return m_poCnt ; }

	//----------------------------------------Varios-----------------------------------------

	//Recrea por completo todos los datos de ao_poCntData, dejando un container construido
	//conforme a los datos de fusion, etc. 
	//Si ai_bSwTestCell TRUE, incondicionalmente mete bordes negros simples en los fields 
	//  region que simulan celdas

	m4bool_t CreateCntData( m4int16_t ai_iPage, ClENG_DesignTree *ai_poDT, 
		StENG_DT_CntData *ao_poCntData, m4bool_t ai_bSwTestCell ) ; 

	void Trace() ;

	//Ver doc serializacion VB
	//Auxiliar para ClENG_CntSample
	void Serialize( ClENG_VB_Serialize *ai_poSerial ) ;

MIT_PRIVATE:

	//Convierte todo a modo de instanciacion ENG_CT_ROW_MERGE.

	void ToRowMerge() ;

	//Auxiliar del proceso ToRowMerge.
	//Recursiva, recorre la rama ai_poNode y va insertando en la lista ai_poInsertList tras 
	//	el iterador ai_poInsertFrom aquellos nodos que sean pagina (sin hijos).

	void AddPageNode( ClENG_DisplayCell *ai_poNode, 
		ENG_CellList_t *ai_poInsertList, ENG_CellList_t::iterator &ai_InsertFrom  ) ;

} ;


//=====================================================Auxiliares ClENG_CntDisplayBuilder

struct StENG_BuildingCellList ;
struct StENG_BuildingCell ;

struct StENG_BuildingCell {
MIT_PRIVATE:

	StMIT_Inspector* m_poInsp;	//Simple link

	struct StKey {
		m4int16_t m_iLevel ;			//De 0 a n
		ENG_CT_CELL_TYPE m_CellType ;	
		m4int16_t m_iIndex ; //-1 para celdas que no se iteran por clave			
	}  ; 

	//Claves de los padres de este nodo y del nodo. La ultima es la del nodo + profundo
	typedef vector<StKey> KeyVector_t;	
	KeyVector_t m_Keys ;	

	m4bool_t m_bDummy ;		
	DvcUnit_t m_MinWidth ;

	//Declarado asi para evitar definiciones recursivas
	StENG_BuildingCellList * m_poSon ; //Siempre 1 objeto

MIT_PUBLIC:
	//-------------->Constructores y sobrecargas para STL
		
	StENG_BuildingCell()  ;
	virtual ~StENG_BuildingCell() { StENG_BuildingCell::End(); }

	StENG_BuildingCell(const StENG_BuildingCell& ai_oSource) ;
	StENG_BuildingCell & operator = (const StENG_BuildingCell &ai_oOpe) ;

	void Init(const StENG_BuildingCell *ai_poSource) ;
	void End() ;	

	//-------------->
	
	//Inserta como hijo en ai_poFather si este no es NULL.
	//Si es NULL, se trata de un nod raiz y lo inserta en ai_poTree.
	//En cualquier caso, devuelve una referencia la objeto ya insertado.

	StENG_BuildingCell * Init(
		StENG_BuildingCellList *ai_poTree,
		StENG_BuildingCell *ai_poFather, m4int16_t ai_iLevel, 
		ENG_CT_CELL_TYPE ai_CellType, m4int16_t ai_iIndex, 	
		m4bool_t ai_bDummy, DvcUnit_t ai_MinWidth
	) ;

	StENG_BuildingCellList * GetSonList() { return m_poSon; }

	DvcUnit_t GetMinWidth() { return m_MinWidth ; }
	m4int16_t GetLevel() { return m_Keys.back().m_iLevel ; }
	ENG_CT_CELL_TYPE GetCellType() { return m_Keys.back().m_CellType ; }
	m4int16_t GetIndex() { return m_Keys.back().m_iIndex ; }
	m4bool_t IsDummy() { return m_bDummy ; }
	m4bool_t IsLeaf()  ;

	//Si al restar MinWidth es <=0, deja el que habia
	void MarkAsDummy(DvcUnit_t ai_SubstractMinWidth) ;

	//Copia esta celda en otro arbol, insertandola en la misma posicion.
	//false si no puiede hacerse porque en el nuevo arbol falta algun padre.
	//NO copia hijos, solo esta celda

	m4bool_t CopyToAnotherTree(StENG_BuildingCellList *ai_poTree) ;

	//Busca el Cell pedido en el arbol. Si ai_Level es <0, usa toda la clave  ai_Keys,
	//	si no limita esta al level pasado.
	//Puede dar NULL (no da errror) si no encuentra.

	static StENG_BuildingCell *GetCell(StENG_BuildingCellList *ai_poTree,
		KeyVector_t *ai_Keys, m4int16_t ai_Level=-1) ;
	
	//Version de la anterior que usa las claves de la celda 
	StENG_BuildingCell *GetCell(StENG_BuildingCellList *ai_poTree, m4int16_t ai_Level=-1) ;

	
	//Obtiene el siguiente hermano de esta celda o de algun padre de esta celda, según 
	//	el nivel de profundidad indicado. Si este es <0, significa al mismo nivel que 
	//	esta celda. 
	//Da NULL si no lo hay.
	
	StENG_BuildingCell *GetNextBrother(
		StENG_BuildingCellList *ai_poTree, m4int16_t ai_Deep=-1 ) ;

	//Dice si beria intentarse un break por together, ver ApplyBreaks. Debe invocarse
	//	desde la celda LastSelected.
	//ao_ExcludeLevel y ao_ExcludeIndex se ponen a -1 si no se necesita break.
	m4bool_t TogetherBreakNeeded( 
		StENG_BuildingCellList *ai_Tree,  
		m4int16_t ai_TogLevel, m4int16_t *ao_ExcludeLevel,	m4int16_t *ao_ExcludeIndex ) ;
	
	//Igual si coincide Level, CellType e Index (solo ultimo nivel)
	m4bool_t EqualsTo(StENG_BuildingCell *ai_poCell) ;
	m4bool_t EqualsTo(StKey *ai_poKey) ;

	//Forma una cadena con la concatenacion de indice_indice_ ... 
	void GetKeyString(ClMIT_Str *ao_poKeyStr) ;

	static void Trace(StENG_BuildingCellList *ai_poTree) ;

MIT_PRIVATE:
	
	static void TraceLevel(StENG_BuildingCellList *ai_poList) ;


} ;

typedef list<StENG_BuildingCell> ENG_BuildingCellList_t;	
struct StENG_BuildingCellList {
	ENG_BuildingCellList_t m_oList ;
} ;


//=============================================================ClENG_CntDisplayBuilder

// Clase de utilería para crear el ClENG_CntDisplay
// Todos los datos que contiene son datos auxiliares de proceso. Los datos utiles de 
//	salida los crea directamente en el ClENG_CntDisplay que esta procesando.

class ClENG_CntDisplayBuilder {

MIT_PRIVATE:

	StMIT_Inspector* m_poInsp;	//Simple link

	//Link, Cnt que está procesando (receptor de la salida del proceso).
	ClENG_CntDisplay *m_poDisplayCnt ;  
	//Link, datos de paramterizacion del container
	StENG_DT_CntData *m_poParamCnt ; 
	ENG_TYPE_CONTAINER m_TypeCnt ;
	//Link, template de diseño
	ClENG_CT_T			* m_poTemplate ;
	StENG_DT_CTData	    * m_poCT ;

	//Todos estos son Links o copias, por comodidad.
	ClENG_KeyTot		*m_poDataTree ;
	ENG_CT_MERGE_PAGE	m_MergePage ;	
	ENG_CT_CELL_Y_POS	m_MergeYMode ;
	ENG_CT_CELL_X_POS	m_MergeXMode ;
	DvcUnit_t			m_XColumnLeft ;		
	DvcUnit_t			m_XColumnRight ;	
	//0 no hay togetehr, 1 a n hay a esos niveles, o 
	//	TOG_NO_LIMIT (ancho infinito de página) (TOG_LOAD_FROM_DESIGN NO, ya resuelto)
	m4int16_t			m_iTogLevel ; 
	m4int16_t			m_iLevelCount ; 

	//Offset a aplicar al Top de las celdas NO estaticas, debido a los modos Y de
	//	container (over/down/align).
		
	DvcUnit_t m_OffTopYMode ;

	//Tantos elementos como páginas se generen.
	//Cada elemento es un arbol de BuildingCells (el elemento es la lista de roots).
	//Estas celdas contienen datos relativos SOLO al ancho minimo. Partimos de una 
	//	unica lista (pagina) con tantas celdas como datos vaya a haber (es decir, si
	//	en el nivel PRODUCTOS hay 8 claves, se crearán 8 celdas con indices 0..7), y
	//	sucesivos procesos, en función de las rupturas de pagina si las celdas no caben,
	//	iran moviendo celdas (y sub-ramas) a otras paginas, creando si es necesario 
	//	celdas dummy en los niveles padre.
	//Cuando ya se han distribuido todas las celdas, un ultimo paso creara DisplayCells, 
	//	en el CntDisplay, 1 a 1, según haya quedado la distribución. Este paso afinará
	//	en los DisplayCell offsets, anchos, altos, etc, pero la distribución ya es fija
	//	según indiquen las BuildingCells.

	typedef list<StENG_BuildingCellList> PageList_t;	
	PageList_t m_oPage ;	

	//Array auxilar para proceso BuildDisplayCnt, contiene una copia de las 
	//  alturas de la template, pero el ultimo level valido (Height>0) puede tener 
	//	incrementos calculados, debidos a los modos de fusion y a las celdas estaticas.
	DvcUnit_t *m_piLevelHeight ;
	m4int16_t m_iLevelCountAux ;

MIT_PUBLIC:

	// ----------------------------- Inicialización---------------------------------------	

	ClENG_CntDisplayBuilder();
	virtual ~ClENG_CntDisplayBuilder() { ClENG_CntDisplayBuilder::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_CntDisplayBuilder) ; }
	virtual void End() ;	

	m4bool_t Init(
		ClENG_CntDisplay *ai_poDisplayCnt,
		ClENG_KeyTot *ai_poDataTree, m4int16_t ai_iTogLevel
	) ;

	// -----------------------------Calculos---------------------------------------------	

MIT_PRIVATE:

	//Recursiva, monta el arbol de BuildingCells  (primer paso)

	void BuildCellsForLevel(
		StENG_BuildingCellList *ai_poTree, 
		m4int16_t ai_iLevel, StENG_BuildingCell *ai_poFather) ;

	//Selecciona un rango de celdas del arbol ai_Page.
	//El criterio de selección se basa en nodos HOJA y sus padres; mientras quepan en
	//	el ancho ai_DvcMaxWidth, se seleccionan. No se permite seleccionar una nodo
	//	con hijos aunque quepa, si no puede seleccionarse ninguno de sus nodos hoja.
	//Si ai_ExcludeIndex es >0, se para la selección (no se incluye), en el primer nodo
	//	de indice ai_ExcludeIndex del nivel ai_ExcludeLevel.
	//ao_LeafCount es la cuenta de nodos HOJA incluidos. ao_LastLeaf es el ultimo nodo
	//	hoja incluido en la selección, y ao_SwAllSelected es TRUE si se pudieron 
	//	seleccionar todos los nodos. 
	//Si ai_MoveToPage no es NULL, los nodos NO seleccionados son movidos a esta nueva 
	//	página. Internamente, se mueve todo ai_Page a ai_NewPage, se hace el recorrido en
	//	ai_NewPage y se mueven los seleccionados a ai_Page. Los nodos seleccionados sin
	//	nodos hijo pendientes de seleccionar, son borrados del origen. Los que tengan 
	//	algun seleccionado y alguno pendiente, son marcados en el origen como dummy.
	
	void SelectCells(
		StENG_BuildingCellList *ai_Page, DvcUnit_t ai_DvcMaxWidth, 
		m4int16_t &ao_LeafCount, 
		ENG_BuildingCellList_t::iterator &ao_LastLeaf, 
		m4bool_t &ao_SwAllSelected,
		StENG_BuildingCellList *ai_NewPage=NULL,
		m4int16_t ai_ExcludeLevel=-1, m4int16_t ai_ExcludeIndex=-1
	) ;
	
	//Auxiliar de SelectCells
	void SelectCellsAux(
		StENG_BuildingCellList *ai_Cells, DvcUnit_t &ao_DvcLeft, 
		m4int16_t &ao_LeafCount, 
		ENG_BuildingCellList_t::iterator &ao_LastLeaf, 
		m4bool_t &ao_SwAllSelected,
		StENG_BuildingCellList *ai_NewPage,
		m4int16_t ai_ExcludeLevel, m4int16_t ai_ExcludeIndex ) ;


	//Genera, si procede, varias paginas de celdas valiendose de SelectCells
	//Tiene en cuenta Together. Lo procesa de la siguiente forma, solo si hay Togeteher:
	//  -Si la celda seleccionada a nivel together tiene indice <0, es decir, si 
	//		el ultimo seleccionado a nivel together es pre/post total, NO hay break
	//		candidato, pues no estamos en mitad de ninguna seleccion.
	//	-Sin embargo, si el indice es >=0, puede haber break candidato por dos motivos, 
	//		o bien algun nivel hijo no se selecciono por completo (es decir, algun nivel
	//		hijo tiene hermanos), o bien aunque el nivel hijo se selecciono por completo
	//		en el nivel together hay un siguiente con el mismo indice.
	//  -Si lo anterior se cumple, hay un intento de break. Digo intento porque se hace 
	//		lo siguiente:
	//		-Se hayan las celdas a seleccionar pero con ExcludeIndex = indice de  la selecc,
	//			y SOLO si asi se selecciona algo, se produce un break por together 
	//			excluyendo estas celdas. La unica excepcion es que no se seleccione nada, pero
	//			vaya a salir la celda estatica en un modo ALIGN y con ONLY_FIRST_PAGE.
	//			En este caso, se dehja un segundo intento en la siguiente pagina.
	//También puede haber breaks naturales, porque no caben más celdas.

	m4bool_t ApplyBreaks() ;

	//Calcula el ancho máximo permisible por hoja de diseño
	//Si se pasa ao_bSwStatic, se actualiza a TRUE si la celda estática va a salir.
	//Si se pasa ao_bSwDummyStatic, y  ao_bSwStatic es TRUE, se actualiza el valor a
	//	TRUE si la celda estática va a ser dummy.

	DvcUnit_t GetMaxPageWidth(m4int16_t ai_IndexPage, 
		m4bool_t *ao_bSwStatic=NULL, m4bool_t *ao_bSwDummyStatic=NULL) ;
	
	void BuildDisplayCnt() ;

	// Auxiliar recursiva de BuildDisplayCnt
	//	-ai_Cells Lista a usar como maestra en el recorrido
	//	-ai_dpCnt. Display Cnt que se esta procesando
	//	-ai_dpCellList  List donde dejar las celdas
	//	-ai_iLevel Level que se esta procesando.
	//	-ai_LevelX Offset de comienzo de celdas no estaticas
	//	-ao_poLevelWidth contiene el ancho consumido por este level (0 si level vacio) No tiene
	//		en cuenta celda la estatica.

	void BuildDisplayLevel( 
		StENG_BuildingCellList *ai_Cells,
		ClENG_CntDisplay::StPage *ai_dpCnt, ENG_CellList_t *ai_dpCellList,
		m4int16_t ai_iLevel, DvcUnit_t ai_LevelX, 
		DvcUnit_t *ao_poLevelWidth) ;

	//Auxiliar de BuildDisplayLevel
	//Distribuye el incremento de anchura en la lista de celdas. Recursiva, va propagando los
	//	incrementos.

	void DistributeWidthIncr( ENG_CellList_t *ai_poList, DvcUnit_t ai_Incr, DvcUnit_t ai_LeftIncr ) ;

	//Obtiene el alto acumulativo de un nivel: es decir, para el nivel 0, suma todos los
	//	altos de nivel, para el 1 todos menos el nivel 0, para el 2 todos menos el 0 y 1, ect.
	DvcUnit_t GetAccLevelHeight( m4int16_t ai_Level ) ;
	DvcUnit_t GetTopLevelHeight( m4int16_t ai_Level ) ;
	DvcUnit_t GetLevelHeight( m4int16_t ai_Level ) ;


} ;


//=======================================================================ClENG_CntSample
//
// Esta clase es un recolector de instancias de ClENG_CntDisplay que forman un Sample.
// Los ClENG_CntDisplay se van generando por varios criterios parametrizables, recorriendo
// el arbol de secciones.

class ClENG_CntSample {

MIT_PRIVATE:
	
	StMIT_Inspector* m_poInsp;	//Simple link

	typedef list<ClENG_CntDisplay> ENG_CntList_t ;	
	ENG_CntList_t m_oCnt ;

MIT_PUBLIC:

	// ----------------------------- Inicialización---------------------------------------	

	ClENG_CntSample();
	virtual ~ClENG_CntSample() { ClENG_CntSample::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_CntSample) ; }
	virtual void End() ;	

	//Crea una muestra basanadose en un CT.

	m4bool_t Init(
		//Datos para localizar el CT (se recorrerá la  rama FirstCrossRow).
		ClENG_DesignTree *ai_poDT, m4char_t *ai_pcCT,
		//Si NO es NULL, se usa este arbol de claves, si es NULL se construye uno auxiliar 
		//	de prueba en base a ai_pcLevelInfo, ai_bSwIncr y a los valores de DT.
		ClENG_KeyTot *ai_poKeyTot,
		//Datos para crear el arbol de claves ejemplo
		const m4char_t * ai_pcLevelInfo, m4bool_t ai_bSwIncr,
		m4int16_t ai_iTogLevel,
		//Si TRUE, se meten instancias de Cnt, sean o no de fusion de CT. Si false, solo se
		//meten estos ultimos.
		m4bool_t ai_bSwAllCnt,
		
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR
	) ;

	//Ver doc serializacion VB
	//Resetea buffer y va llamando a los Serialize asociados

	void Serialize( ClENG_VB_Serialize *ai_poSerial ) ;

	void Trace() ;

	//Devuelve el numero de paginas layout correspondiente al container de sample que ocupe 
	//más paginas.

	m4int16_t GetLayoutPageCount() ;

MIT_PRIVATE:

	// --------------------------------- Auxiliares -----------------------------------------	

	//Recursiva, va añadiendo CT

	m4bool_t CT_AddCnt( 
		StENG_DT_SectionData *ai_poSection, StENG_DT_CTData *ai_poCT,
		ClENG_KeyTot *ai_poKeyTot, m4int16_t ai_iTogLevel, m4bool_t ai_bSwAllCnt
	) ;

} ;

#endif


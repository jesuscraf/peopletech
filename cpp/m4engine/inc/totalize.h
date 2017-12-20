//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             totalize.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/31/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//    Gestion de totalizadores
//
//
//==============================================================================

#ifndef __TOTALIZE__H__
#define __TOTALIZE__H__

#include <stdarg.h>

#include "m4stl.hpp"
#include "clstr.h"
#include "strindex.h"

#include "m4clflag.h"
#include "xengval.h"

//
//NOTA: es importante que en este modulo NO haya dependencias con elementos de report:
//	Se usa a nivel de DT.
//

//============================================================================ClENG_Tot

class ClENG_Tot
{
MIT_PRIVATE:
	StMIT_Inspector* m_poInsp;	//Simple link
	
	m4bool_t m_bNoInit ;	//Si TRUE, aun no hay ninguna entrada

	enum TYPE { NUMERIC, DATE, BOOLEAN, VARIANT, NO_INIT /*Este ultimo es un flag*/ } ;
	TYPE m_Type ;

	ClMIT_ChnValue m_oFirst, m_oLast, m_oMin, m_oMax ;	
	m4uint32_t m_iEntryCount, m_iTrueCount, m_iNullCount ;
	m4double_t m_dEntrySum ;

MIT_PUBLIC:

	ClENG_Tot();
	virtual ~ClENG_Tot() { ClENG_Tot::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_Tot) ; }
	virtual void End() ;

	void Init() ;

	void Reset() ;

	//Testea y pone el de acumulador.
	//Si el valor metido esta marcado como IsNULL, no se hace nada.
	void Entry(ClMIT_ChnValue *ai_poValue) ;

	// Toma como entry otro totalizador. En vez de meter un nuevo valor unico, se hace un
	//	m_iEntryCount += ai_poValue->m_iEntryCount, m_iTrueCount += ai_poValue->m_iTrueCount,
	//	m_dEntrySum += ai_poValue->m_dEntrySum
	// Tambien se actualizan correctamente m_oMin, m_oMax, etc. 
	// Los entrys de totalizadores NO_INIT solo influyen en el GetCount(). En booleanos se
	//	toma como un FALSE a afectos de GetTrueCount y GetFalseCount

	void Entry(ClENG_Tot *ai_poValue) ;

	m4uint32_t GetCount() { return m_iEntryCount; }
	m4uint32_t GetNULLCount() { return m_iNullCount; }

	//****Cualquier metodo tipo Get sobre un NO_INIT, es valido y retorna un valor 
	//	por defecto. Esto evita errores si no se ha hecho ningun entry.

	//Validos para tipo BOOLEAN (error local si no es asi)
	m4uint32_t GetTrueCount() ;
	m4uint32_t GetFalseCount() ;

	void GetFirst(ClMIT_ChnValue *ao_poValue) { ao_poValue->Init(&m_oFirst) ; }
	void GetLast(ClMIT_ChnValue *ao_poValue) { ao_poValue->Init(&m_oLast) ; }

	//Valido para tipo NUMERIC (error local si no es asi)
	m4double_t GetSum() ;
	m4double_t GetAvg() ;	

	//Valido para tipo NUMERIC y DATE (error local si no es asi)
	void GetMax(ClMIT_ChnValue *ao_poValue) ;
	void GetMin(ClMIT_ChnValue *ao_poValue) ;

	TYPE GetType() { return m_Type ; }
} ;

//============================================================================ClENG_KeyTot

class ClENG_KeyTot ;
struct StKeyTotSon ;

class ClENG_KeyTotNode {
	friend class ClENG_KeyTot ;
private:
	enum SORT_BY {SEQUENTIAL,ASCENDING,DESCENDING} ;
		
	SORT_BY m_SortBy ;	//< == en este nodo (no en hijos) actuan conforme a esto.

	m4uint32_t m_uiSeqOrder ;	//Ordinal que representa al orden.
	ClMIT_ChnValue m_oKey, m_oHeader ;
	ClENG_Tot *m_poTot ;	//Array dinámico de dos dimensiones. 
							//No hay que alocar m_poTot en copias.
	StKeyTotSon *m_poSon ;	//Declarado asi para evitar definiciones circulares
							//Tampoco hay que alocar m_poTot en copias.

public:
	// Sobrecargas para STL
	ClENG_KeyTotNode();
	virtual ~ClENG_KeyTotNode() ;
	ClENG_KeyTotNode(const ClENG_KeyTotNode& ai_oSource) ;
	ClENG_KeyTotNode & operator = (const ClENG_KeyTotNode &ai_oOpe);
	m4bool_t operator == (const ClENG_KeyTotNode &ai_oOpe) const;	
	m4bool_t operator < (const ClENG_KeyTotNode &ai_oOpe) const;
} ;

//IMPORTANTE, es una lista porque el orden debe ser el de entrada, y porque en run-time 
//	puede ser rordenada de diversas formas via sort.

typedef list<ClENG_KeyTotNode> KeyTotList_t;	

struct StKeyTotSon {
	KeyTotList_t m_oSon ;	
} ;

class ClENG_KeyTot
{
MIT_PRIVATE:

	StMIT_Inspector* m_poInsp;	//Simple link


	KeyTotList_t m_RootLevel ;	//Mantiene un ARBOL JERARQUICO de claves

	m4int16_t m_iLevelCount ;	//Profundidad maxima del arbol
	m4int16_t * m_piIndexKey ;	//Array para procesos por indice
	ClMIT_ChnValue * m_poValueKey ;	//Array para procesos por valor

	// Cada nodo del arbol puede contener n totalizadores diferentes, que comparten clave.
	// Como también puede haber varios arboles distintos, con la misma estructura, se
	//implementan como un mismo arbol, con varios juegos de totalizadores por nodo. Asi
	//pues, cada nodo contiene:
	//	m_poTot[TreeIndex][SubIndex]

	m4int16_t m_iTotCount, m_iTreeCount ;
	ClENG_StrIndex m_oTreeIndex ;
	ClENG_StrIndex m_oTotIndex ;

	//  Un ClENG_KeyTot puede mantener una lista de hijos derivados, y estos a su vez pueden tener 
	//mas derivados. Se le llama Master al arbol del que se deriva.
	//  La unica diferencia de un ClENG_KeyTot derivado es que esta compuesto por un conjunto o 
	//subconjunto de claves del maestro, formados al reorganizar los niveles en otro orden.
	//Por ello varia la inicializacion, al no necesitar de ningun parametro exterior: 
	//simplemente con el ClENG_KeyTot master hay suficiente. El Reset maestro tambien actua en todos 
	//sus ClENG_KeyTot derivados. Y en un derivado no se hacen Entrys individuales, si no que se 
	//debe llamar a un SynchronizeDerived en el master para que actualize los totalizadores en 
	//los hijos. Todo lo demas esta disponible como si de un ClENG_KeyTot normal se tratase.
	//	Los ClENG_KeyTot derivados son construidos y destruidos por los ClENG_KeyTot master.

	//Mantenidos en el master
	ClENG_StrIndex m_oDerivedIndex ;	
	ClENG_KeyTot *m_poDerived ;		//NULL si no hay.

	//Mantenidos en el derived
	ClENG_KeyTot *m_poMaster ;	//NULL si no deriva de nadie, o puntero del padre que deriva.
	m4int16_t * m_piDerivedLevel ;	//NULL si no deriva de nadie
									//Array para procesos por indice, alocado a m_iLevelCount
									//Tiene el level padre referenciado (de 1 a n).

MIT_PUBLIC:

	ClENG_KeyTot();
	virtual ~ClENG_KeyTot() { ClENG_KeyTot::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_KeyTot) ; }
	virtual void End() ;	//Borra todo el arbol de keys

MIT_PRIVATE:
	void EndDerived() ;	//Auxiliar de End() y de Init() de derivados.

MIT_PUBLIC:


	//  TotCount o TreeCount se calculan en base a cuantos identificadores producen las listas 
	//ai_pcIdTree y ai_pcIdTot. Si alguno de estos valores es 0, nunca podran crearse nodos.
	//ai_LevelCount sirve para saber cuantos niveles hay, como minimo 1, y para alocar 
	//m_piIndexKey  y m_poValueKey.

	m4bool_t Init( m4int16_t ai_LevelCount, 
		const m4char_t * ai_pcIdTree, const m4char_t * ai_pcIdTot, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;

	//Además de hacer el Init, crea automáticamente un arbol de claves para testeo.
	//Lo forma conforme ai_pcLevelInfo, que es una lista separada por comas con un contador
	//de ocurrencias por nivel. Si ai_bIncr es TRUE, cada instancia de clave va añadiendo un
	//elemento más, para crear un arbol con un numero creciente de hijos. Se hace solo en el
	//ultimo nivel, para evitar un crecimiento exponencial.
	//Los niveles que no pasen o tengan valores ilegales, se pondrán a 1.

	m4bool_t InitTest( m4int16_t ai_LevelCount, 
		const m4char_t * ai_pcIdTree, const m4char_t * ai_pcIdTot, 
		const m4char_t * ai_pcLevelInfo, m4bool_t ai_bSwIncr,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;

	//  Inicializa y enlaza todos los objetos derivados de este. A partir de ese momento, es
	//tarea del padre borrar los hijos.
	//	El metodo es reentrante. Monta todo en funcion de los datos que hay en el momento, por
	//lo que debe ser llamado despues de hacer todos los NewKey pertinentes.
	//	La linea de comandos tiene el formato:
	//	IdDerivedCT.Level.Level. ..., IdDerivedCT.Level.Level.... 
	//  Como maximo el arbol puede ser tan profundo como el master, ya que no pueden repetirse
	//niveles. Debe haber algun nivel forzosamente.
	//	Si no se pasa nada en Command, se borran los objetos derivados que haya.

	m4bool_t InitDerived( m4char_t *ai_pcCommand,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ; 


	//---Nota:
	//Hay varios procesos que pueden hacerse indicando la clave por indices o por valor. Por
	//	supuesto, los indices van de 0 a n.
	//Segun lo que competa, hay que hacer las llamadas a SetIndexKey ó SetValueKey que 
	//proceda.
	//ai_iLevel siempre indica la profundidad de la clave (o del arbol), expresado de 1 a n.
	//	Cuando se diga que 'no se pasa la subclave', quiere decirse que que ai_iLevel es 0. 

	//-------------------------Mantenimiento dinamico del arbol de claves---------------

	//Funciona forzosamente en modo ValueKey.
	//Cuelga la nueva NewKey de la subclave indicada, o de la raiz si no se pasa ninguna 
	//subclave.
	//ai_iLevel debe valer al menos 1, o se producira un error local.
	// Para tener exito la subclave, si se pasa, debe existir (no pueden crearse subclaves 
	//intermedias), y la clave no debe de existir. 
	// Si TotCount o TreeCount son 0, siempre da FALSE.
	// Si ai_poHeader es NULL se mete como cabecera el valor clave
	// Da TRUE si tiene exito, y FALSE si no (además en este caso genera un error local).

	m4bool_t NewKey( m4int16_t ai_iLevel, ClMIT_ChnValue *ai_poHeader=NULL ) ;

	//Funciona forzosamente en modo ValueKey.
	// Borra la clave o subclave (si es subclave, borra todas claves hijas).
	//Si no se pasa la clave/subclave, no hace nada y devuelve FALSE.
	// Da FALSE si no tiene exito debido a no haber encontrado la clave (además en este 
	//caso genera un error local).

	m4bool_t RemoveKey( m4int16_t ai_iLevel ) ;

	//-------------------------Consultas------------------------------------------------

	struct StInfo {
		ClENG_Tot *m_poTot ;		//Relacion solo de uso
		ClMIT_ChnValue *m_poKey ;	//Relacion solo de uso
		ClMIT_ChnValue *m_poHeader ;	//Relacion solo de uso
		size_t m_oSonCount ;
	} ;

	// Busca la clave/subclave y carga los valores indicados. Puede dar:
	// -TRUE si ha habido exito, en cuyo caso cargara la informacion asociada. Si no se pasa
	//	la clave, se da TRUE, pero la unica informacion valida es m_oSonCount (numero de 
	//	items raiz), y los otros datos apuntaran a NULL.
	// -FALSE. No se ha localizado una clave que se ha pasado, o bien algun indice esta fuera
	//	de rango (esto ultimo produce un error local incondicional). Info tiene todo a NULL.

	enum FIND_ACTION { 
		FIND_TEST,	//Aunque no encuentre, no genera un error local.
		FIND_REQ	//Si no encuentra, genera un error local.
	} ;
	m4bool_t GetInfoByValue( 
		FIND_ACTION ai_Action, m4int16_t ai_iLevel,
		m4int16_t ai_iTreeIndex, m4int16_t ai_iSubIndex, 
		StInfo *ao_pstInfo
	) ; 
	m4bool_t GetInfoByIndex( 
		FIND_ACTION ai_Action, m4int16_t ai_iLevel,
		m4int16_t ai_iTreeIndex, m4int16_t ai_iSubIndex, 
		StInfo *ao_pstInfo
	) ; 

	//Funcionan como ClENG_StrIndex::GetIndex()
	m4int16_t GetTreeIndex(const m4char_t *ai_pcKey, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) 
		{	return m_oTreeIndex.GetIndex(ai_pcKey,NULL,ai_ErrorEffect) ;}
	m4int16_t GetTotIndex(const m4char_t *ai_pcKey, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) 
		{	return m_oTotIndex.GetIndex(ai_pcKey,NULL,ai_ErrorEffect) ;}

	//-------------------------Mantenimiento de los totalizadores-----------------------

	// Da false si el totalizador no se encuentra. En este caso, no se actualiza ningun
	//	totalizador, y se produce un error local.
	// Da true si todo va bien. Se actualizan todos los totalizadores que forman parte 
	//	del recorrido hasta llegar al ultimo nodo.
	// Si SubIndex es ALL_TREES actua sobre todos los arboles, si no en el indicado.
	// Si se pasa ai_poTotEntry se hara un entry por totalizador, si no por valor.
	// No tiene sentido llamar a este metodo en objetos derivados, pero puede hacerse.

	enum { ON_ALL_TREES=-1  } ;

	m4bool_t TotEntryByValue( 
		m4int16_t ai_iLevel, m4int16_t ai_iTreeIndex, m4int16_t ai_iSubIndex, 
		ClMIT_ChnValue *ai_poEntry, ClENG_Tot *ai_poTotEntry=NULL
	) ; 

	// Para objetos con derivados (aunque no pasa nada si se llama en objetos sin derivados).
	// Hace la sincronizacion entry de todos sus derivados, y luego llama al SynchronizeEntry
	//	de los derivados, por si hay que provocar una cascada de actualizaciones.

	void SynchronizeEntry() ;

	//Exactamente como la anterior, pero hace un Reset (de un totalizador)
	//Tambien resetea los de objetos derivados llamando a sus TotResetByValue, lo que
	//	provoca, si debe, una cascada de actualizaciones.

	m4bool_t TotResetByValue( 
		m4int16_t ai_iLevel, m4int16_t ai_iTreeIndex, m4int16_t ai_iSubIndex
	) ; 

	//Resetea TODOS los totalizadores de un arbol, o de todos si se pasa ON_ALL_TREES
	//Da false y genera un error local si no encuentra el indice pertinente.
	//Tambien resetea los de objetos derivados llamando a sus TotResetByValue, lo que
	//	provoca, si debe, una cascada de actualizaciones.

	m4bool_t TotReset( m4int16_t ai_iTreeIndex=ON_ALL_TREES ) ; 

	//-------------------------Especial para calculos parciales-------------------------
	
	//  Metodo parecido a GetInfo. GetInfo localiza en ultima instancia un unico totalizador,
	//y hay que pasarle siempre indices concretos. En este metodo, en cambio, puede
	//pasarse en cualquier indice un valor TOTALIZE_LEVEL, con lo que queremos decir que
	//queremos obtener la informacion agrupando por ese nivel.
	//	Este metodo es mas lento que los GetInfo, ya que hay que recorrer ramas e ir 
	//agrupando. Por ejemplo, si tenemos los niveles Empresa/Dpto/Empleado, un 
	//0,TOTALIZE_LEVEL,1 calcula los datos del empleado 1 en la empresa 0, 
	//independientemente del Dpto donde este (hay que recorrer todos dptos y buscarle).
	//	Fijarse que cualquier secuencia final de -1 equivale a un GetInfo normal, pero con
	//el nivel recortado. Osea, un 0,-1,-1 equivale a un GetInfo con profundidad 1. El 
	//metodo tiene en cuenta esto, aunque no deja de ser mas lento que un GetInfo.
	//	Da false si hay errores (no encontrar algun indice, por ejemplo). Genera error local.
	//ai_poTot es usado para almacenar el resultado de todos los calculos. Este metodo se
	//encarga de hacerle el reset cuando competa.

	enum { TOTALIZE_LEVEL=-1  } ;
	m4bool_t GetPartialInfoByIndex( 
		m4int16_t ai_iLevel, m4int16_t ai_iTreeIndex, m4int16_t ai_iSubIndex, 
		ClENG_Tot * ai_poTot
	) ; 

	//Devuelve el nivel bajo el cual todo son TOTALIZE_LEVEL, y 0 si todos indices son
	//	TOTALIZE_LEVEL.

	m4int16_t GetConsLevelByIndex(m4int16_t ai_iLevel) ;

	//Tras hacer un GetConsLevel, mira en los indices restantes si hay carga parcial. 
	//La hay si no hay indices, o si queda algun -1.
	m4bool_t IsPartialLoad(m4int16_t ai_iLevel) ;

	//-------------------------Ordenaciones--------------------------------------------

	typedef list <ClMIT_ChnValue> KeyList_t ;

	//Ordena todo el arbol
	void SortAll( ClENG_KeyTotNode::SORT_BY ai_Sort) 
		{ Sort(&m_RootLevel,ai_Sort,-1,M4_FALSE,NULL) ; }

	//Ordena solo el nivel indicado
	void SortLevel( ClENG_KeyTotNode::SORT_BY ai_Sort, m4int16_t ai_Level) 
		{ Sort(&m_RootLevel,ai_Sort,ai_Level,M4_TRUE,NULL) ; }

	//  Ordena los valores subclave pendientes de la clave pasada. Si Level es 0, se ordenan
	//los valores raices.
	//  Si ao_poList no es NULL, se copian los valores ordenados.
	//  Da FALSE si la calve no se encuentra.

	m4bool_t SortKey( ClENG_KeyTotNode::SORT_BY ai_Sort, m4int16_t ai_Level, KeyList_t *ao_KeyList=NULL) ; 

	//-------------------------Arboles derivados---------------------------------------

	// Invocado en un arbol derivado, da el level derivado correspondiente al level padre 
	//	pasado en el metodo. 
	// Si se invoca en objetos no derivados, o no hay level correspondiente en el derivado,
	//	da un valor negativo.

	m4int16_t LevelFatherToDerived(m4int16_t ai_iFatherLevel) ;

	// La inversa a la anterior.

	m4int16_t DerivedLevelToFather(m4int16_t ai_iDerivedLevel) ;

	//-------------------------Get & Set-----------------------------------------------

	m4int16_t GetTotCount() { return m_iTotCount ; }
	m4int16_t GetTreeCount() { return m_iTreeCount ; }

	m4int16_t GetLevelCount() { return m_iLevelCount ; }

	//Ponen/Leen el IndexKey indicado. Index va de 0 a n.
	//Si se sale de margen, produce error local. Set no hace nada y Get da 0.
	void SetIndexKey(m4int16_t ai_iIndex, m4int16_t ai_iValue) ;
	m4int16_t GetIndexKey(m4int16_t ai_iIndex) ;

	//Como los anteriores, para las claves. Fijarse que se puede usar directamente el value
	//por ejemplo asi:
	//	Key.GetValueKey(0)->InitString("Hola") ;
	void SetValueKey(m4int16_t ai_iIndex, ClMIT_ChnValue * ai_iValue) ;
	ClMIT_ChnValue * GetValueKey(m4int16_t ai_iIndex) ;	//Link, uso inmediato

	//-------------------------Trace----------------------------------------------------

	//Solo en version debug, saca al fichero trace un listado del contenido del arbol, y
	//	llama al trace de los derivados.

	void Trace() ;

MIT_PRIVATE:

	void TraceThis(KeyTotList_t * ao_poList) ;	//Recursiva

	//-----------------------------------Privados---------------------------------------

MIT_PRIVATE:

	//Borra la rama y el item correspondiente a It (si este apunta a end, no hace nada)

	void Remove(KeyTotList_t * ai_poList, KeyTotList_t::iterator & ai_poIt) ;

	void Remove(KeyTotList_t * ao_poList) ;	//Recursivo, borra el nodo y sus ramas

	//Recursivo, resetea los totalizadores del nodo y sus ramas
	void TotReset(KeyTotList_t * ao_poList, m4int16_t ai_iTreeIndex=ON_ALL_TREES) ;	

	//  Puede devolver:
	//	-TRUE. Se encontro la clave. ai_poList apunta a la lista que la contiene, y ai_poIt 
	//		al elemento en cuestion. 
	//	-FALSE. No se encontro la clave o esta no se paso. ai_poList apunta a la lista donde 
	//		no se encontro la clave (si esta se paso, si no no tiene sentido).
	//
	//	Cualquier indice fuera de rango producira un error local, y que se devuelva FALSE.
	//		Solo se permite que ai_iTreeIndex sea ON_ALL_TREES si la accion no es NONE_ACTION
	//  TotAction indica si hay una accion extra a ejecutar sobre el totalizador. 
	//		Si TreeIndex es ON_ALL_TREES la accion se ejecuta sobre todos arboles, si no en 
	//		el indicado, siempre dentro del mismo SubIndex. La accion se va disparando
	//		en los totalizadores a medida que se hace el Find (ojo, se hace 
	//		independientemente de si en ultima instancia existe o no la clave). 

	enum TOT_ACTION { NONE_ACTION, ENTRY_VALUE_ACTION, ENTRY_TOT_ACTION, RESET_ACTION } ;
	m4bool_t Find( FIND_ACTION ai_Action,
		KeyTotList_t * &ao_poList, KeyTotList_t::iterator & ao_poIt, 
		ClMIT_ChnValue *ai_poSubKey, m4int16_t ai_iLevel, 
		m4int16_t ai_iTreeIndex, m4int16_t ai_iSubIndex, 
		TOT_ACTION ai_TotAction=NONE_ACTION, 
		ClMIT_ChnValue *ai_poEntry=NULL, ClENG_Tot *ai_poTotEntry=NULL ) ;
	m4bool_t Find( FIND_ACTION ai_Action,
		KeyTotList_t * &ao_poList, KeyTotList_t::iterator & ao_poIt, 
		m4int16_t *ai_piSubKey, m4int16_t ai_iLevel, 
		m4int16_t ai_iTreeIndex, m4int16_t ai_iSubIndex, 
		TOT_ACTION ai_TotAction=NONE_ACTION, 
		ClMIT_ChnValue *ai_poEntry=NULL, ClENG_Tot *ai_poTotEntry=NULL) ;

	//Da true si encuentra elemento. Si no lo encuentra, el iterador apunta a end
	m4bool_t GetItFromIndex(KeyTotList_t * ao_poList, m4int16_t ai_iIndex, 
		KeyTotList_t::iterator & ao_poIt) ;
	m4bool_t GetItFromKey(KeyTotList_t * ao_poList, ClMIT_ChnValue * ai_poKey, 
		KeyTotList_t::iterator & ao_poIt) ;

	//Metodo recursivo.
	//SubKey y ai_iLevel son RELATIVOS a la posicion actual. SubKey[0] es el indice actual
	//	a buscar, y SubIndex cuantos faltan de procesar (1 al menos).
	//Si el indice es TOTALIZE_LEVEL da el resultado de recorrer todos los hijos, mientras 
	//	que si es un indice en concreto, solo procesa ese.
	m4bool_t GetPartialInfo( 
		KeyTotList_t * ao_poList, m4int16_t *ai_piSubKey, m4int16_t ai_iLevel,
		m4int16_t ai_iTreeIndex, m4int16_t ai_iSubIndex, 
		ClENG_Tot * ai_poTot
	) ; 

	//  Puede ser un metodo recursivo.
	//  Hace un sort de los mienbros de la lista pasada. Ademas, si se le pasa ao_KeyList (osea, 
	//el puntero no es NULL), se copia la lista de claves en la lista. 
	//  -Si ai_CountLevel es <0, recorre todas la ramas hijas de la lista pasada.
	//	-Si ai_CountLevel es 0 no hace nada.
	//  -Si es otro valor, opera como antes, solo que el recorrido alcanza como maximo 
	//		ai_CountLevel de profundidad, a partir y contando la lista pasada.
	//	-Si ai_bOnLastLevel solo se ordena en el ultimo nivel accedido, si no en todos. En caso
	//		de indicarse ai_bOnLastLevel, y un CountLevel x, si el ultimo nodo accedido no tiene 
	//		la profundidad pedida, no se hara nada sobre el.
	//  Ejemplos:
	//		Sort( &RootLevel, ASCENDING, -1, M4_FALSE )  ordena todo el arbol
	//		Sort( &RootLevel, ASCENDING, -1, M4_TRUE  )  ordena solo el ultimo nivel.
	//		Sort( &RootLevel, ASCENDING,  1 )  ordena solo el nivel pasado. 

	void Sort( KeyTotList_t * ao_poList, ClENG_KeyTotNode::SORT_BY ai_Sort,
		m4int16_t ai_CountLevel=1, m4bool_t ai_bOnLastLevel=M4_TRUE,  
		KeyList_t *ao_KeyList=NULL ) ;

	//Este metodo realiza todas las acciones que relacionan un arbol padre con su derivado.
	//Se invoca en el padre, y es el arbol padre el que se recorre.
	//Tiene como entrada:
	//	-m_poValueKey DEL DERIVADO. Lista de claves buscadas. Conforme a esto se monta en el
	//		padre el m_poValueKey, que contiene valores de claves concretos o, si se quiere que
	//		se recorra todo el nivel, se meten valores NO_INIT. Este array es reutilizado por
	//		el metodo. Este metodo usa y cambia los valores m_poValueKey de padre y derivado.
	//		Los m_poValueKey del padre pueden contener cualquier valor. Los del hijo, sin
	//		embargo, pueden ser utiles aun: lo que puede cambiar son los valores NO_INIT del
	//		principio: los otros deben permanecer inalterados.
	//	-ai_iMaxLevelDerived. Es el maximo nivel de profundidad de proceso en el derivado.
	//		Si es 0 no se hace nada, ya que un valor 1 combinado con el m_poValueKey[0] cubre 
	//		tanto el caso de buscar un valor concreto (m_poValueKey[0] contiene un valor), como
	//		recorrer todos los valores raiz (m_poValueKey[0] no contiene un valor).
	//		Nunca o lo que es lo mismo,Si es 0, se esta buscando valores raices.
	//		Conforme a este valor se haya el ai_iMaxLevel, que es el homologo para el padre.
	//		Cuando se encuentra una clave de este nivel que coincide con los values concretos 
	//		pasados en  m_poValueKey se dispara un procesamiento en el hijo, y ya no se 
	//		profundiza mas en el arbol.
	//		Cuando hay que operar en el derivado, se ordenan las claves en el m_poValueKey 
	//		derivado conforme a la estructura de este y su relacion con el padre.
	//	-ai_Action. Puede ser:
	//		-DERIVED_INIT. Mete claves en el derivado, siempre que no esten duplicadas.
	//		-DERIVED_ENTRY. Hace un entry con las ocurrencias en el derivado. 
    
	enum SYNC_ACTION { DERIVED_INIT, DERIVED_ENTRY } ;

	void Synchronize( ClENG_KeyTot *ao_poDerived, SYNC_ACTION ai_Action, m4int16_t ai_iMaxLevelDerived) ;

	//Recursiva auxiliar de la anterior, recorre el padre. 
	//ao_poList es la lista padre que se recorre actualmente. Conforme se recorre el padre, se
	//  va montando la posible clave en el derivado.

	void SynchronizeAux( 
		KeyTotList_t * ao_poList, 
		m4int16_t ai_iActualLevel, m4int16_t ai_iMaxLevel, m4int16_t ai_iMaxLevelDerived,
		ClENG_KeyTot *ao_poDerived, SYNC_ACTION ai_Action ) ;

	// Auxiliar de SynchronizeAux, el metodo se ejecuta en el objeto derivado.
	// ai_poSource es el nodo padre correspondiente al nivel que se va a actualizar. Ahora
	//	mismo lo usa DERIVED_INIT para cojer el valor header, y DERIVED_ENTRY para acceder a los
	//	totalizadores.
	void DoDerivedAction(
		m4int16_t ai_iMaxLevelDerived, SYNC_ACTION ai_Action, ClENG_KeyTotNode *ai_poSource) ;


	//Recursiva, hace uso de Synchronize. Llena los valores de un nodo, y luego pasa a disparar
	//  la inicializacion de los hijos. 
	//Se dispara en objetos derivados.

	void InitDerivedNode(KeyTotList_t * ao_poList,m4int16_t ai_iActualLevel) ;

	//Como la anterior, pero hace el Entry. El momento de dipsrarlo es radicalmente distinto al
	//	de InitDerivedNode, ya que esta llama a Synchronize por cada valor en todos nodos, 
	//	mientras que este metodo solo llama a Synchronize en valores de nodos de ultimo nivel.

	void EntryDerivedNode(KeyTotList_t * ao_poList,m4int16_t ai_iActualLevel) ;
	

	//AUXILIAR de InitTest. 
	//ai_iLevel de 0 a n.

	void ResetTestLevel(m4int16_t ai_iLevel, ClMIT_Args *ai_poArg, m4bool_t ai_bSwIncr,
		m4int16_t *ao_piLastBK ) ;
} ;

#endif

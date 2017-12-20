//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             memprof.h
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
//		Profiler de memoria. Gestiona toda la memoria dinamica.   
//
//
//==============================================================================
#ifndef __MEMPROF__H__
#define __MEMPROF__H__

// Si no esta definida esta macro, no se procesa el Profiler de memoria.
// NO definirla en este cabecera, es mejor en el XXdefine.hpp que tiene cada aplicacion,
//	porque asi se pueden tener todos estos defines en un mismo sitio, lo cual es muy comodo
//	para cambiar settings.

//#define MIT_MEM_PROF

class ClMIT_MemInfo {

	//ACCESO LIMITADO A INSPECTOR

MIT_PUBLIC:

	enum { SIZE_STR_GROUP=80 } ;

	m4char_t m_pcNameGroup[SIZE_STR_GROUP] ;
		
	// Numero de news, deletes, y NULL deletes hechos hasta el momento.

	m4int32_t m_iNews, m_iDeletes, m_iNullDeletes ;

	// Items alocados y borrados y memoria alocada y borrada hasta el momento.

	m4int32_t m_iItemsAllocated, m_iItemsDeleted, m_iMemAllocated, m_iMemDeleted ;

	// Maximo pico de memoria (momento en que mas memoria ha habido alocada a la vez).
	
	m4int32_t m_iMaxMem ;

	// Numero maximo de items que ha habido a la vez en memoria.

	m4int32_t m_iMaxItemsMem ;

	// Maximo sizeof alocado, que puede no cincidir con la mayor alocacion hecha.

	m4int32_t m_iMaxSizeof ;

	// Mayor alocacion de memoria realizada, e items creados en esa alocacion.

	m4int32_t m_iMaxAlloc, m_iMaxItemAlloc ;


	ClMIT_MemInfo() ;

	m4int32_t GetSpendMem() { return m_iMemAllocated-m_iMemDeleted ; }
	m4int32_t GetSpendItem() { return m_iItemsAllocated-m_iItemsDeleted ; }
	m4int32_t GetMaxSizeofAlloc() { return m_iMaxAlloc/m_iMaxItemAlloc ; }
} ; 

struct StMIT_Inspector ;

class M4_DECL_M4MIENV ClMIT_MemProf {

	//ACCESO LIMITADO A INSPECTOR

MIT_PRIVATE:

	StMIT_Inspector *m_poInsp ;	//Simple link

	ClMIT_MemInfo *m_poInfo ;	//Array dinamico
	m4int16_t m_iNItems ;

	m4int32_t m_iUnInitNews, m_iUnInitDeletes ;

	//Def false, si true ejecuta un CheckPoint al salir. Si hay error, siempre se ejecuta.
	m4bool_t m_bCheckPointAtEnd ;	

MIT_PUBLIC:

	ClMIT_MemProf() ;

	//Hace el Init correspondiente, mas comodo si hacemos un objeto estatico.
	
	ClMIT_MemProf( 
		m4int16_t ai_iNItems, m4int16_t ai_iNSharedItems, 
		const m4char_t *const ai_pcArrNameA[], 
		const m4char_t *const ai_pcArrNameB[]) ;

	virtual ~ClMIT_MemProf() { ClMIT_MemProf::End(); } 

	// Sobre un objeto inicializado, produce salida DEBUG con estadisticas, indicando si 
	//	hay alguna discordancia en la memoria, y cual es. Concretamente se testea:
	//	-Si hay alguna discrepancia entre m_iNews y m_iDeletes, tanto a nivel de grupo
	//		como global (Error inmediato).
	//	-Lo mismo, para m_iItemsAllocated, m_iItemsDeleted, m_iMemAllocated, m_iMemDeleted.
	//		(Error inmediato)
	//  -Independientemente de si el objeto esta o no inicializado, un problema con 
	//		m_iUnInitNews o m_iUnInitDeletes siempre se notifica (Warning).
	// Osea, si el mensaje nos dice que todo va bien, es que todos estos filtros se han
	//superado con exito.

	virtual void End() ;

	// Siempre aloca uno mas para almacenar los totales. Los picos de los totales NO se 
	//	calculan sumando los picos de los subgrupos, porque esto es incorrecto, ya que
	//	estos picos pueden no coincidir en el tiempo.
	// No resetea m_iUnInitNews ni m_iUnInitDeletes, porque si el objeto se reutiliza 
	//	varias veces, asi siempre hay constancia de si en alguna de ellas se produjo esta
	//	situacion anomala.

	void Init( m4int16_t ai_iNItems ) ;

	// Como un Init, pero actualiza el array de nombres.
	// Se hace desde 2 tablas (es un append de una con otra), porque hay algunos nombres y
	//	grupos ya predefinidos.
	// NItems es el total de items, se los cuales se leeran los ai_iNSharedItems primeros
	//	items de ai_pcArrNameA, y el resto (si queda), de ai_pcArrNameB.
	
	void Init( 
		m4int16_t ai_iNItems, m4int16_t ai_iNSharedItems, 
		const m4char_t *const ai_pcArrNameA[], 
		const m4char_t *const ai_pcArrNameB[]) ;

	// Si algun grupo se queda sin nombre no pasa nada: este se pone vacio.
	// Si indice fuera de rango, error inmediato.

	void ResetName( m4int16_t ai_iIndexGroup, const m4char_t *ai_pcName ) ;

	// Actualiza datos. Si new fue fallido, error inmediato.
	// Si indice fuera de rango (no admisible el indice del total), error inmediato.
	// Si se llama antes de que el objeto se haya inicializado o despues de destruirse, 
	//	no se hace nada excepto incrementar m_iUnInitNews y lanzar un Warning. 
	//	Asi se detectan problemas con constructores de objetos estaticos.

	void ProcessNew(  m4int16_t ai_iIndexGroup, void * ai_pvPtr,   m4int32_t ai_iNItems, 
		m4int32_t ai_iSizeof ) ;

	// Actualiza datos, controlando si fue un delete fallido o no, devolviendo true si
	//	el delete es normal.
	// Si indice fuera de rango (no admisible el indice del total), error inmediato.
	// Si se llama antes de que el objeto se haya inicializado o despues de destruirse, 
	//	no se hace nada excepto incrementar m_iUnInitDeletes y lanzar un Warning. 
	// Asi se detectan problemas con destructores de objetos estaticos.

	m4bool_t ProcessDelete( m4int16_t ai_iIndexGroup, void * ai_pvPtr, m4int32_t ai_iNItems, 
		m4int32_t ai_iSizeof ) ;

	// Si se llama antes de que el objeto se haya inicializado, no hace nada. La cadena no
	//	puede ser NULL.
	// Saca mensajes de DEBUG, mostrando la informacion actual de memoria.

	void CheckPoint( const m4char_t * ai_pcStr ) ;

	void SetCheckPointAtEnd(m4bool_t ai_bSw)  { m_bCheckPointAtEnd=ai_bSw; } 
	m4bool_t IsCheckPointAtEnd() { return m_bCheckPointAtEnd ; } 

MIT_PRIVATE:
	// Calcula la fila de totales

	void CalculateTotal() ;
} ;


#ifdef MIT_MEM_PROF


// NOTAS: 
//	-Ojo con los defines; no usar operadores tipo ++, etc, porque si salen referenciados
//		mas de una vez en la macro los valores pueden ser incorrectos.
//	-Cuidado al pasar el parametro PtrDest. En cualquier macro siempre es inicializado a
//		NULL o lo alocado, por lo que se debe pasar el puntero real que se vaya a utilizar 
//		en el resto de la aplicacion, y no un puntero que sea una copia local, ya que en 
//		este caso la inicializacion a NULL o a lo alocado se pierde.
// 
//	Con polimorfismo se da un problema, solo al hacer el delete, que consiste en que es
//incorrecto hacer un sizeof del puntero, porque este sizeof siempre se resuelve en compilacion
//y dara siempre el sizeof de la clase base. Para evitar esto, en todas clases con las que se
//vaya a usar polimorfismo (o mejor aun, en todas clases), definir:	
//
// class ClClase {
//		....
//		virtual size_t GetSizeof() { return sizeof(ClClase) ; }
//	} ;
//	
// Asi, en ejecucion puede saberse el sizeof correcto. La unica razon para que este metodo 
//	no sea estatico es que no podria ser a la vez virtual.
// ¿Que macros usar en cada momento?. Muy facil:
//
//	-M4_NEW_ITEM. Para news de un solo item. Al hacer news nunca hay ambiguedad con los sizeof.
//	-M4_NEW_ARRAY. Para news de array.

//	-M4_DELETE_ITEM. Borra un item no polimorfico, se usa sizeof.
//	-M4_DELETE_VIRTUAL_ITEM. Borra un item polimorfico, se usa GetSizeof().	   
//	-M4_DELETE_ARRAY. Para deletes de array. Cuando se borra un array, este nunca puede ser de
//		miembros polimorficos. En todo caso, sera de punteros a objetos polimorficos, pero en
//		ese caso habra que hacer previamente un delete individual para estos.
//
//	Se supone que siempre se usan los constructores defecto, no se implementa la capacidad de
//llamar a otros constructores. Si hace falta, crear otras macros que lo hagan, pero es mejor
//hacer todas operaciones de creacion/destruccion con metodos Init/End.		
	

#define M4_NEW_ITEM( ObjMemProf, IdGroup, PtrDest, Type) { (PtrDest)=new Type ; (ObjMemProf)->ProcessNew( (IdGroup), (PtrDest), 1,  sizeof(Type) ) ; } ;

#define M4_NEW_ARRAY( ObjMemProf, IdGroup, PtrDest, Type, ItemsCount ) { (PtrDest)=new Type[(ItemsCount)] ; (ObjMemProf)->ProcessNew( (IdGroup), (PtrDest), (ItemsCount), sizeof(Type) ) ; } ;

#define M4_DELETE_ITEM( ObjMemProf, IdGroup, PtrDest, Type ) if ( (ObjMemProf)->ProcessDelete( (IdGroup),(PtrDest), 1, sizeof(Type) ) ) { delete (PtrDest) ; (PtrDest)=NULL ; } ; 

#define M4_DELETE_VIRTUAL_ITEM( ObjMemProf, IdGroup, PtrDest ) if ( (PtrDest) ) { if ( (ObjMemProf)->ProcessDelete( (IdGroup),(PtrDest), 1, (PtrDest)->GetSizeof() ) ) { delete (PtrDest) ; (PtrDest)=NULL ; } ; } ; 

#define M4_DELETE_ARRAY( ObjMemProf, IdGroup, PtrDest, Type, ItemsCount ) if ( (ObjMemProf)->ProcessDelete( (IdGroup),(PtrDest), (ItemsCount), sizeof(Type) ) ) { delete [](PtrDest) ; (PtrDest)=NULL ; } ; 

#else

#define M4_NEW_ITEM( ObjMemProf, IdGroup, PtrDest, Type) { (PtrDest)=new Type ; } ;

#define M4_NEW_ARRAY( ObjMemProf, IdGroup, PtrDest, Type, ItemsCount ) { (PtrDest)=new Type[(ItemsCount)] ;	} ;

#define M4_DELETE_ITEM( ObjMemProf, IdGroup, PtrDest, Type ) if ( (PtrDest) ) { delete (PtrDest) ; (PtrDest)=NULL ; } ; 

#define M4_DELETE_VIRTUAL_ITEM( ObjMemProf, IdGroup, PtrDest ) if ( (PtrDest) ) { delete (PtrDest) ; (PtrDest)=NULL ; } ; 

#define M4_DELETE_ARRAY( ObjMemProf, IdGroup, PtrDest, Type, ItemsCount ) if ( (PtrDest) ) { delete [](PtrDest) ; (PtrDest)=NULL ; } ; 

#endif

#endif


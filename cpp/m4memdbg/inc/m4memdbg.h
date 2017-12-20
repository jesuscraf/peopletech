//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             m4memdbg.h
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
//		Debugger de memoria
//
//==============================================================================

#ifndef __M4MEMDBG__H__
#define __M4MEMDBG__H__

#include <stdio.h>
#include "m4memdbg_dll.hpp"
#include "m4stl.hpp"
#include "m4ipcs.hpp"
#include "m4string.hpp"



//=============================================>Struct auxiliar

#define M4_MEM_DBG_COMP_ID_SIZE  10
#define M4_MEM_DBG_LOCAL_ID_SIZE 20

struct StM4_MemDbgInfo {
private:
	m4char_t	m_pcCompId[M4_MEM_DBG_COMP_ID_SIZE] ;
	m4char_t	m_pcSrcId[M4_MEM_DBG_LOCAL_ID_SIZE] ;
	m4uint16_t	m_uiLocalId ;
public:

	StM4_MemDbgInfo( m4char_t *	ai_pcCompId, m4char_t *ai_pcSrcId, m4uint16_t	ai_uiLocalId  ) ;	
	StM4_MemDbgInfo() ;	
	virtual ~StM4_MemDbgInfo() {; }	

	StM4_MemDbgInfo(const StM4_MemDbgInfo& ai_oSource) ;
	StM4_MemDbgInfo & operator = (const StM4_MemDbgInfo &ai_oOpe);
	m4bool_t operator == (const StM4_MemDbgInfo &ai_oOpe) const;	
	m4bool_t operator < (const StM4_MemDbgInfo &ai_oOpe) const;

	const m4char_t	* GetCompId() const { return m_pcCompId ; }
	const m4char_t	* GetSrcId() const { return m_pcSrcId ; }
	m4uint16_t	GetLocalId() const { return m_uiLocalId ; }

} ;


//=============================================>MACROS NEW y DELETE
//
//	CompId
//	------
// -Cada componente debe tener un CompId único, a fin de localizar mejor todo.
// -Son pequeñas cadenas de 9 char maximo.
// -En SSC, proyecto m4memdbg, fichero compid.key estan los Id utilizados. Para reservar un 
//	Id nuevo, hacer un Check-Out del fichero, registrar el nuevo Id, y hacer Check-In.
//	Ninguna de estas acciones implica recompilar nada, simplemente es un control de reserva.
//
//	SrcId
//	-----
//	-Cada src a ser controlado debería tener un Id unico. Ahora mismo, por defecto, se usa
//		__FILE__ .
//
//	LocalId
//	-------
//	-Son m4uint16_t.
//	-Por defecto, se usa automaticamente __LINE__
//
//
//================================================================================>ClM4_MemDbg

#define M4_MEM_DBG_SEED_NAME_SIZE 250

class M4_DECL_M4MEMDBG ClM4_MemDbg {

private:

	ClMutex m_oMutex ;	//Regiones criticas 

	//Map, la key es el Ptr alocado.
	//  El mapa SOLO contiene información de punteros NO borrados. SI un puntero se borra, se
	//busca en el mapa y se borra de él.
	typedef map < uintptr_t, StM4_MemDbgInfo, less<uintptr_t> > AllocList_t ;
	AllocList_t m_oPtrs ;	
	AllocList_t m_oDeleteFailed ;	//Por ptr no encontrado en lista

	//first =  Key =  second de Ptrs. second=Count

	typedef map < StM4_MemDbgInfo, m4uint32_t, less<StM4_MemDbgInfo> > ReduceByLine_t ;

	m4uint32_t m_uiNewFailed ;	//Raro, por repetido en lista

	//Nombre usado como Seed para generar los nombres de SnapShot automaticamente (def UNKNOW).
	//Puede contener paths.
	//Se genera:
	//	-Shutdown de esta DLL: m_pcSeedName_FINAL.SNS
	//	-SnapShotToFile() sin parametro de file: m_pcSeedName_n.SNS (n de 0 a n, num secuencial) 

	m4char_t m_pcSeedName[M4_MEM_DBG_SEED_NAME_SIZE] ;
	m4uint32_t m_uiSeed ;

	//Dice cada cuantos News lanza automaticamente un SnapShot secuencial. 
	//<=0, no lanza ninguno.
	//Por defecto 50

	m4int32_t m_iAutoSnapShot ;	

	//Guarda los nombres de fichero de los SnapShots generados por AutoSnapShot (no los 
	//	generados por peticiones explicitas). Util para la propiadad WindowSNS.
	
	typedef list < m4string_t > SnapShotFiles_t ;
	SnapShotFiles_t m_oWindowList ;

	//Si <=0, propiedad de Window desactivada. 
	//Si >0, solo se mantienen el numero de ficheros indicados, borrando los mas viejos.
	//Def, 10.

	m4int16_t m_iWindowSNS ;

public:

	ClM4_MemDbg() ;	
	virtual ~ClM4_MemDbg() ;	

	void NotifyNew( void *ai_Ptr,    m4char_t	* ai_pcCompId, m4char_t * ai_pcSrcId, m4uint16_t ai_uiLocalId  ) ;
	void NotifyDelete( void *ai_Ptr, m4char_t	* ai_pcCompId, m4char_t * ai_pcSrcId, m4uint16_t ai_uiLocalId ) ;
	
	m4uint32_t GetUndeletedCount() ;	

	void SetSeedName( const m4char_t *ai_Name ) ;

	void SetAutoSnapShot( m4int32_t ai_Value ) { m_iAutoSnapShot=ai_Value; }	
	m4int32_t GetAutoSnapShot( ) { return m_iAutoSnapShot; }	

	void SetWindowSNS( m4int16_t ai_Value ) { m_iWindowSNS=ai_Value; }	
	m4int16_t GetWindowSNS( ) { return m_iWindowSNS; }	

	//Tipo para pasar listas de CompId y limitar la salida de informacion a los grupos que
	//	queramos. 
	//Map para find. Meter en ambos miembros el CompId.

	typedef map < m4string_t, m4string_t, less<m4string_t> > CompIdList_t ;

	//Tipos de listado de información:
	//	-ALL. Lista de todos items. 1 item	                      -> 1 CompId-SrcId-LineId-Ptr
	//	-REDUCE_BY_LINE. n items con el mismo CompId-SrcId-LineId -> 1 CompId-SrcId-LineId-Count
	//Tanto REDUCE_BY_LINE como  REDUCE_BY_COMP implican la contrucción de una lista auxiliar,
	//	y es más lento.

	enum TYPE_SNAP_SHOT { ALL, REDUCE_BY_LINE } ;

	//Si lista=NULL, TODOS, si no solo los indicados.
	//Devuelve TRUE si hay alguno seleccionado. Aunque falle en la creación del fichero,
	//	la información devuelta es correcta.

	m4bool_t SnapShotToFile(
		m4char_t *ai_pcFile, TYPE_SNAP_SHOT ai_Type = REDUCE_BY_LINE, 
		CompIdList_t *ai_poCompId=NULL) ;

	//Llama a la anterior, formando nombres de ficheros secuenciales, en el formato 
	//	SNAP_SHOT_n.TXT, variando n desde 0 a n.

	m4bool_t SnapShotToFile(
		TYPE_SNAP_SHOT ai_Type = REDUCE_BY_LINE, 
		CompIdList_t *ai_poCompId=NULL) ;

private:

	//Como sus homologas, sin Mutex (para evitar anidar llamadas con Mutex)

	m4bool_t _SnapShotToFile(
		m4char_t *ai_pcFile, TYPE_SNAP_SHOT ai_Type = REDUCE_BY_LINE, 
		CompIdList_t *ai_poCompId=NULL) ;
	m4bool_t _SnapShotToFile(
		TYPE_SNAP_SHOT ai_Type = REDUCE_BY_LINE, 
		CompIdList_t *ai_poCompId=NULL) ;



	
	static void ReduceByLine(AllocList_t *ai_poList, ReduceByLine_t *ao_poReduceList, CompIdList_t *ai_poCompId  ) ;

	static void ShowAllocList(FILE *ai_poF, AllocList_t *ai_poList) ;
	static void ShowReduceList(FILE *ai_poF, ReduceByLine_t *ai_poList) ;

} ;

//================================================================================>Globales

//VARIABLE GLOBAL, activa o desactiva el traceo de memoria. Al ser un tipo basico, no se 
//	sincroniza. Por defecto, false.

extern M4_DECL_M4MEMDBG m4bool_t g_bM4SwMemDbgOn ; 

//OBJETO GLOBAL, ESTATICO, UNICO POR PROCESO 

extern M4_DECL_M4MEMDBG ClM4_MemDbg g_oM4MemDbg ; 

//========================================================================>MACROS NEW y DELETE

//****New utilizando  M4_SRV_TRACE_COMP_ID (la misma que para traza), M4_MEM_DBG_MODULE, 
//		y __LINE__ como defecto

#define M4_MDBG_NEW(Ptr,New) M4_MDBG_NEW_2( M4_SRV_TRACE_COMP_ID, __FILE__, __LINE__, Ptr, New ) 

//****New sin defectos

#define M4_MDBG_NEW_2(CompId,SrcId,LocalId,Ptr,New) { New ; if (g_bM4SwMemDbgOn) g_oM4MemDbg.NotifyNew( (Ptr), (CompId), (SrcId), (LocalId) ) ; }  

//****Delete

#define M4_MDBG_DELETE(Ptr,Delete)  M4_MDBG_DELETE_2(M4_SRV_TRACE_COMP_ID, __FILE__, __LINE__, Ptr, Delete) ;

//****Delete sin defectos

#define M4_MDBG_DELETE_2(CompId,SrcId,LocalId,Ptr,Delete) { if ( (Ptr) ) { if (g_bM4SwMemDbgOn) g_oM4MemDbg.NotifyDelete((Ptr),(CompId), (SrcId), (LocalId)) ; Delete ; } ; }

//================================================================================>Static

//Este metodo es una forma opcional y estandar para que el componente final configure por 
//	completo el entorno de desarrollo, entendiendo por esto los mecansimos de memeoria y traza
//	que suministra este componente. Bastaría una llamada al principio del módulo, y ya está.
//	Funciona leyendo varaibales de entorno, y esto es lo que hace:
//
//	-M4_SRV_MDBG=0 no, otro valor si. Por defecto es 0.
//		Pone el flag de g_bM4SwMemDbgOn (activa/desactiva el seguimiento de la memoria).
//
//	-M4_SRV_SNS_PATH=Path. Si no existe, defecto vacio. 
//		Hace un g_oM4MemDbg.SetSeedName con el resultado de concatenar este Path,el valor 
//		ai_pcTraceId pasado, y otra vez este valor. ai_pcTraceId está HardCoded y puede ser 
//		perfectamente el TraceId  definido por  M4_SRV_TRACE_COMP_ID. 
//		Pe, corremos el DPCH y el valor de M4_SRV_SNS_PATH es c:\temp. Se hará un SetSeedName
//		con c:\temp\DPCH\DPCH, con lo que tendremos en el directorio c:\temp\DPCH un listado
//		de ficheros DPCH_0.SNS, DPCH_1.SNS, etc. 
//		Por ahora, si el directorio no existe, NO se crea, pendiente de hacer.
//
//	-M4_SRV_AUTO_SNS=50 valor autosnapshot, defecto 0. <=0 no hay, solo sale el XXXX_FINAL.SNS.
//		Llama a g_oM4MemDbg.SetAutoSnapShot().
//
//	-M4_SRV_WINDOW_SNS=10 valor windowSNS, defecto 10. <=0, desactivado. Si se pasa un valor
//		>0, se esta indicando cuantos ficheros de autosnapshot se quieren como maximo (se 
//		guardan los mas recientes y se van borrando los mas viejos).
//		Llama a g_oM4MemDbg.SetWindowSNS()
//
//	-M4_SRV_TRACE_LEVEL=level. Por defecto, 1 (level GENERAL). 2 es DETAILED y 3 DEBUG.   
//		Llama a g_oM4SrvTrace.SetCurrentLevel.
//		Puede llamarse a otros numeros superiores si se desea. Algunos componentes usan niveles
//		de traza más allá de 3 para tener mas niveles de detalle. 
//		El nivel 100 (DEBUG_X=97) es de este componente, para notificar todos new y deletes.
//
//	-M4_SRV_M4LOG=0 no, otro valor si. Salida de errores y warnings a M4Log. Defecto SI.
//		Resetea g_bM4SwSrvM4LogOn 
//
//	-M4_SRV_COUT=0 no, otro valor si. Salida de traza a cout. Defecto 1 para DEBUG, y 0 para 
//		Release.
//		Resetea g_bM4SwSrvCoutOn 
//
//	-M4_SRV_COUT_FILE=0 no, 1 si, otro valor, con flush a fichero por cada mensaje.
//		Si está activado, la salida a cout también se copiará a un fichero ubicado en el mismo
//		lugar que los SNS, solo que se llamara ai_pcTraceId.LOG, (pe, c:\temp\DPCH\DPCH.log).

void M4_DECL_M4MEMDBG M4SrvConfigDevelopEnvByEnvVble( m4char_t *ai_pcTraceId ) ;

#endif


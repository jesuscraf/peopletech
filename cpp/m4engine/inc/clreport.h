//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             clreport.h
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
//    Parte LOADER de la capa Core
//
//
//==============================================================================

#ifndef __CLREPORT_H__
#define __CLREPORT_H__

//============================================================================ClENG_Report

#include "reportenv.h"
#include "strindex.h"
#include "designin.h"

#include "field.h"

class ClENG_DesignTree ;
struct StENG_DT_CntData ;
struct StENG_DT_FieldData ;
struct StENG_DT_SectionData ;
class ClENG_Section ;
class ClENG_CntGroup ;
class ClENG_Tot ;
class ClENG_Container ;
class ClENG_Section ;
class ClENG_KeyTot ;

class ClENG_Report
{
MIT_PRIVATE:

	StMIT_Inspector* m_poInsp;	//Simple link

 	ClMIT_Str m_oReportId, m_oReportName ;
	
	// ClENG_Section:.End() se encarga de borrar recursivamente todo el arbol. 

	ClENG_Section   *m_poRoot ;

	// Array que contiene los containers de pagina.

	m4int16_t   m_iNContainers ;
	ClENG_CntGroup *m_oCntPage ; //Siempre X_Y_FIXED y TOGETHER

	StENG_ReportEnv m_oReportEnv ;
	StENG_FieldEnv  m_oFieldEnv ;	//Apunta a miembros de m_oReportEnv

	ClENG_KeyTot *m_oCT ;			//Array de CT_TreeKey
	ClENG_StrIndex m_oCTIndex ;

	// Breaks de report. Las reglas que guian a los breaks son:
	//	-La minima unidad de break es un FlushLayout, y es justo despues de ese momento 
	//		cuando se testea la condicion de break. Eso quiere decir que en un report con 
	//		3 paginas layout, es despues de que el dispositivo vuelque 3 paginas cuando
	//		de testea la condicion de break, y este es disparado si compete.
	//	-A la seccion que controla el break se le activa automaticamente el break de 
	//		registro y after, para que se provoquen EOPs cuando haya breaks. Osea, un 
	//		break en la seccion A se disparara:
	//		-Rewind(), imprimiendo 1. NO hay break.
	//		-Acabe con 1, me muevo al 2, hay break y EOP. 
	//		-Acabe con 2, me muevo al 3, hay break y EOP. 
	//		-Acabo con 3, me muevo a EOF, hay break y EOP.
	//	-El significado de hacer un break depende del device en cuestion. Por regla general
	//		implica cambio de fichero.

MIT_PUBLIC:
	enum BREAK_MODE { NO_BREAK, RECORD_BREAK, RECORD_SLICE_BREAK } ;
MIT_PRIVATE:

	BREAK_MODE  m_Break ;	
	ClENG_Section *m_poBreakSec ;	//Link a una sección del arbol m_oRoot

	m4bool_t  m_bEvaluateExpressionByBreak ; // Evaluar expresión de fichero por cada ruptura

	//Array de totalizadores, y lista para sus indices

	ClENG_Tot *m_oTotalize ;
	m4int16_t m_iCountTotalize ;
	ClENG_StrIndex m_oTotIndex ;
	
	//Para heredar interes de DT

	ClENG_FileInterestList m_oFileInterest ;


	//---Publicacion

	m4bool_t  m_bSwPublish ;

MIT_PUBLIC:

	// ----------------------------- Inicialización---------------------------------------	

	ClENG_Report();
	virtual ~ClENG_Report() { ClENG_Report::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_Report) ; }

	// Entre otras cosas, borra el arbol de secciones m_oRoot.

	virtual void End() ;

	//Hace el Init & Load del canal de datos segun indique ai_poExeParams
	//Si se le pasa ai_poReport, hace el UpdateOrderByItem

	static m4bool_t InitChnData( ClENG_ReportArgs *ai_poExeParams, ClMIT_Chn *ai_poChnData,
		ClENG_DesignTree *ai_poDT, ClENG_Report *ai_poReport=NULL) ;

	//La complementaria del metdo anterior
	static void EndChnData( ClENG_ReportArgs *ai_poExeParams, ClMIT_Chn *ai_poChnData ) ;
	
	// -----------------------------Ejecucion report---------------------------------------	
	//
	// ***********Las formas de ejecutar todo el impreso:
	//
	// -Ejecucion atomica:
	//		Execute(). En una unica llamada se ejecuta todo. Simplemente hace uso de los
	//			metodos de la ejecucion parcial.
	// -Ejecucion parcial, break a break.
	//		PrepareExecution(&DT) ;
	//		//Opcional : DT.End() ;
	//		while ( ExecuteBreak(M4_FALSE,&BreakCount) && !ExitTest ) {
	//		} ;
	//		TerminateExecution() ;
	//	-Ejecucion parcial, todos breaks de golpe
	//		PrepareExecution(&DT) ;
	//		//Opcional : DT.End() ;
	//		ExecuteBreak(M4_TRUE,&BreakCount) ;
	//		TerminateExecution() ;
	//
	//  Se ejecuta secuenciado por breaks, Si se pasa BreakCount, se almacena la cuenta de
	//		breaks validos hasta la fecha. 
	//  El valor de retorno de ExecuteBreak indica si hay mas breaks por procesar o no.
	//	SIEMPRE se debe ejecutar 1 sola vez PrepareExecution y TerminateExecution, pues hacen 
	//		tareas de cracion y limpieza respectivamente. ExecuteBreak puede ejecutarse
	//		n veces. 
	//	En caso de que PrepareExecution() de errores NO debe llamarse a ExecuteBreak. 
	//		Sin embargo, puede llamarse a TerminateExecution().
	//	Cuando se trabaja de forma parcial hay que tener cuidado si se hacen cosas como
	//		compartir un fichero que este generando un device: en este contexto, los
	//		devices escriben secuencialmente, y siempre suponen que la posicion del fichero
	//		es la adecuada: por ello, si esta posicion se altera, debe de restaurarse antes
	//		de la siguiente llamada parcial.
	//
	// Se secuencia el tratamiento de fields de pagina. El comportamiento varia segun el 
	//dispositivo:
	//
	//	-------->Sobre un device con IsPreloadOnDevice()==M4_TRUE.
	//
	//	-Containers pertenecientes a pagina. Se hace lo siguiente:
	//		-Recorre los field de los containers de pagina, y se hace un Show de los que 
	//			sean estaticos optimizables, y los borra.
	//		-Llama a a FixOptimizedPreload().
	//		-Hace otra pasada como la primera vez, pero con los campos estaticos no 
	//			optimizables.
	//		-Llama a FixPreload().
	//		-Posteriormente, cada vez que se haga un FlushLayout, siempre se hace un Show
	//			de los field que quedan (que son los no estaticos). Fijarse que los 
	//			estaticos no vuelven a generarse via Show, lo que significa que no pueden
	//			cambiar de coordenadas ni nada similar.
	//	-Containers pertenecientes a secciones. No llevan ningun proceso Preload, ni 
	//		optimize. Lo que si pueden es tener fields estaticos, o no. Si son estaticos,
	//		si que pueden cambiar cada vez de coordenadas, porque se dispara siempre un
	//		metodo Show.
	//
	//	-------->Sobre un device con IsPreloadOnDevice()==M4_FALSE.
	//
	//	-Containers pertenecientes a pagina. Se hace lo siguiente:
	//		-Se llamara a FixOptimizedPreload() y FixPreload(), simplemente por convencion.
	//		-Posteriormente, cada vez que se haga un FlushLayout, siempre se hace un Show
	//			de los field que quedan (osea, todos, sean o no estaticos).
	//	-Containers pertenecientes a secciones. Exactamente igual a antes.
	//
	// Pondremos un ejemplo, de que pasa con un field A tipo RasterG, segun donde se ponga 
	//	este.
	//
	//	-------->A se imprime en un device con IsPreloadOnDevice()=M4_TRUE (ejemplo: PCL).
	//
	//	-A esta en un container de pagina.
	//		-A es estatico (RasterGLit). Se lee y decodifica el BMP UNA sola vez, y se 
	//			imprime en unas coordenadas fijas y traduce a PCL UNA sola vez.
	//			-Ademas es optimizable (SwOptimize en Init=M4_TRUE). El bloque PCL generado
	//				se manda UNA sola vez a impresora, dentro de una macro. Cada vez que
	//				se llama a FlushLayout solo se envia la llamada a la macro, no la 
	//				informacion PCL (que de hecho ya NO reside en RAM).
	//			-No es optimizable (SwOptimize en Init=M4_FALSE). El bloque PCL generado 
	//				reside en RAM y se envia cada vez que se hace un FlushLayout.
	//		-No es estatico (RasterGChn). Se comporta igual que cuando esta en un container
	//			de seccion. 
	//	-A esta en un container de seccion.
	//		-A es estatico (RasterGLit). Se lee y decodifica el BMP UNA sola vez, y se 
	//			imprime en unas coordenadas cualquiera y traduce a PCL tantas veces como
	//			se muestre el container. La propiedad de optimize no tiene sentido.
	//		-No es estatico (RasterGChn). Se lee y decodifica el BMP, se imprime en unas 
	//			coordenadas cualquiera y se traduce a PCL tantas veces como se muestre el
	//			container.
	//
	//	-------->A se imprime en un device con IsPreloadOnDevice()=M4_FALSE.
	//
	//	-El comportamiento siempre es como si estuviese en un container de seccion.	

	//Carga m_oReportEnv y todo el diseño de un report.
	//Fijarse que el DT es completamente independiente del Report: este lo usa en el Init
	//	para construir el report, pero despues no guarda ningún link ni relación con el DT,
	//	con lo cual podría borrarse el DT sin ningúntipo de problema..
	//El objeto de TaskEnv m_oMVC debe estar inicializado.
	//Da FALSE si hubo error.
	
	m4bool_t PrepareExecution( 
		ClENG_DesignTree *ai_poDT, ClENG_ReportArgs	*ai_poExeParams,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;


	// Devuelve TRUE mientras haya mas breaks que procesar. Da FALSE en el ultimo break.
	//Si ao_poFile no es NULL, se almacena el HasFlush de la pagina 0.

	m4bool_t ExecuteBreak(
		m4bool_t ai_bExecuteAllBreaks=M4_TRUE, 
		m4uint32_t *ao_uiBreakCount=NULL,
		ClENG_PhysicalFile **ao_poFile=NULL) ;

	// Devuelve FALSE si hubo error.
	// Si se pasa ao_NavigationInfo almacena la NavigationInfo

	m4bool_t TerminateExecution( ClMIT_Str *ao_NavigationPathMask=NULL ) ;

	// Devuelve FALSE si hubo error.
	// Empaqueta las llamadas anteriores

	m4bool_t Execute(ClENG_DesignTree *ai_poDT, ClENG_ReportArgs *ai_poExeParams,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;

MIT_PRIVATE:

	//=========================== ORDER_BY =============================================
	//Realiza el recorrido de todas las secciones de un report antes de la fase de carga
	// del canal de datos y ejecucion del Report propiamente dicha. Se encarga de buscar 
	// agrupaciones para generar la sentancia APISQL para el ORDER BY Item.
	// Se le pasa el contador de Items para generar la sentencia correctamente y el nombre
	// del nodo del que tira el padre, por si de alguna forma se mete incorrectamente este nodo
	// Esto ultimo solo se tiene en cuenta en caso de que sea subgrupo
	m4bool_t UpdateOrderByItem(ClENG_DesignTree *ai_poDT);

	m4bool_t TreeSectionForOrderBy(StENG_DT_SectionData *ai_poSection, m4uint16_t ai_uiOrderCount, m4char_t* ai_pcNodeName=NULL);

	//Preanaliza todo el report. 
	//Respecto del contexto, pone estos defectos:
	//	-m_poChn. Al de datos.
	//	-m_poReport. Al propio objeto.
	//	-m_poDvc. Al del ReportEnv
	//	-m_poFieldManager. Al del ReportEnv
	//	-m_poExeParams. Al del ReportEnv
	//	-m_poDBFormat. Al del ReportEnv.
	//Da true si todo va bien.

	m4bool_t PreAnalex(ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;

MIT_PUBLIC:

	// ----------------------------- Get&Set------------------------------------------------	

	// Busca una seccion. Da NULL si no la encuentra, o la direccion de la seccion si 
	//	localiza la seccion. 

	ClENG_Section * FindSec(m4uint32_t  ai_iIdSec, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR ) ;

	const m4char_t * GetReportId() { return m_oReportId.InChar(0) ; }
	const m4char_t * GetReportName() { return m_oReportName.InChar(0) ; }

	m4int16_t GetCountTotalize() { return m_iCountTotalize ; }

	//Da un link para uso. Error si no se encuntra el totalizador. 
	//Si hay error, devuelve NULL

	ClENG_KeyTot * GetKeyTotalize(const m4char_t *ai_pcCTId,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;

	//Da un link para uso. Error si no se encuntra el totalizador. 
	//Si hay error, devuelve NULL
	ClENG_Tot * GetTotalize(const m4char_t *ai_pcTotalize,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;

	BREAK_MODE  GetBreakMode() { return m_Break ;}

	ClENG_Section * GetBreakSection() { return m_poBreakSec ; }

	//Solo para uso! Minimizar uso
	
	ClENG_OutDevice	* GetDvc() { return m_oReportEnv.m_poDvc ; } 

	m4bool_t IsPublish() { return m_bSwPublish ; }
	
MIT_PRIVATE:

	// -----------------------------Auxiliares Init(ai_poDT)-------------------------------	

	m4bool_t LoadDBFont(ClENG_DesignTree *ai_poDT) ;
	m4bool_t LoadDBFormat(ClENG_DesignTree *ai_poDT) ;
	m4bool_t LoadDvc( ClENG_DesignTree *ai_poDT, m4int16_t ai_PageLayoutCount, m4bool_t ai_bReplicatePages) ;
	m4bool_t LoadPageContainers(ClENG_DesignTree *ai_poDT, m4bool_t ai_bReplicatePages) ;
	ClENG_Container * CreateContainer( 
		StENG_DT_CntData *ai_poCntData, DvcUnit_t ai_OffsetTop, DvcUnit_t ai_OffsetLeft,
		m4bool_t ai_bSwTestCnt, m4bool_t ai_bSwTestField ) ;
	ClENG_Field * CreateField( StENG_DT_FieldData *ai_poFieldData, 
		DvcUnit_t ai_OffsetTop, DvcUnit_t ai_OffsetLeft, m4bool_t ai_bSwTestField ) ;

	ClENG_Section * LoadTreeSection( 
		ClENG_DesignTree *ai_poDT,
		StENG_DT_SectionData *ai_poSection, ClENG_Section *ai_poFather,
		m4bool_t ai_bSwSync, m4int16_t ai_iExeOrder ) ;

	struct StCTLevelData {
		ClMIT_ChnNode *NodeKey ;	ClMIT_ChnItem ItemKey ;
		ClMIT_ChnNode *NodeHeader ; ClMIT_ChnItem ItemHeader ;
	} ;
	m4bool_t LoadCT( 
		ClENG_DesignTree *ai_poDT, m4char_t *ai_pcCTId , ClENG_KeyTot *ai_poKeyTot,
		m4int16_t *ao_piPageCount ) ;
	m4bool_t LoadCTLevel( ClENG_KeyTot *ai_poKeyTot,
		StCTLevelData *ai_poLevel, m4int16_t ai_iActLevel, m4int16_t ai_iTopLevel ) ;

	
	//------------------------------------------------------------------------------------

MIT_PRIVATE:

	ClENG_Section * FindSec( ClENG_Section *ai_poSec, m4uint32_t  ai_iIdSec) ;

	// Recursiva
	void BLResetNotSync( ClENG_Section *ai_poSection ) ;

} ;


#endif

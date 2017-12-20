//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             core.h
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
//    Nucleo funcional de los reports
//
//
//==============================================================================

#ifndef __CORE__H__
#define __CORE__H__

#include "m4stl.hpp"
#include "clstr.h"
#include "strindex.h"

#include "m4clflag.h"
#include "field.h"
#include "xapichn.h"
#include "totalize.h"
#include "corevr.h"

//============================================================================ClENG_Container

class ClENG_Container  
{
MIT_PRIVATE:

	StMIT_Inspector* m_poInsp;	//Simple link
	StENG_ReportEnv *m_poReportEnv ;	//Simple link

	//-------------------------Miembros-------------------------------------------------

MIT_PUBLIC:

	enum FLAGS { TOGETHER=ClMIT_Flag::BIT0, X_Y_FIXED=ClMIT_Flag::BIT1 } ;

	enum STATE { 
		NOT_INITIALIZED, 
		INITIALIZED, 
		LOADING_FIELDS, 
		WAITING_FOR_PRELOAD, 
		START_PRINTING,
		PARTIAL_PRINTED,
		PRINTED
	} ;

	//PRINT_ATTEMP es el maximo numero de reintentos de print parciales que no impriman nada.
	enum OTHERS { PRINT_ATTEMP= 3 } ;	

MIT_PRIVATE:

	STATE m_State ;

	//  Lista de fields del container.
	//  Las coordenadas del Field son relativas a su Container.
	//  Puede haber 0 elementos.

	typedef list<ClENG_Field *> FieldList_t;	
	FieldList_t m_oField ;

	//  Switchs varios.
	//  -X_Y_FIXED.
	//		-TRUE. Siempre se imprime en unas coordenadas fijas. Si TRUE, TOGETHER tambien 
	//			siempre es TRUE, pues no pueden producirse impresiones parciales.
	//		-FALSE. Se imprime a partir de la BaseLine, y esta se actualiza.
	//	-TOGETHER. 
	//		-TRUE. El Container no puede romperse: si no cabe en la pagina, no se imprime.
	//		-FALSE. Puede romperse: se imprime lo que se pueda hasta el limite impuesto, y
	//			queda un trozo pendiente de imprimirse en la siguiente hoja.

	ClMIT_Flag m_oFlag ;

	//Todos campos de un container van en una misma pagina. Un container siempre tiene una X
	//	y una altura fija.
	//Si es X_Y_FIXED, la altura donde se imprime es Y, y nunca se actualiza la BaseLine.
	//Si no es X_Y_FIXED, se usan los limites m_Y, m_SizeYLimit, y la BaseLineY para calcular
	//	donde imprimir. Ademas actualiza la BaseLine.
	
	m4int16_t   m_iPage ;

	//m_HeightBackup guardara la altura de diseño del Container, ya que los multilineas 
	//	variables pueden hacer variar el m_Height y es posible tener que cambiar este
	//	a psoteriori.

	DvcUnit_t m_X, m_Height, m_HeightBackup, m_Y, m_SizeYLimit, m_Width ;

	// Si el filtro da TRUE, se cancelara la impresion el container. En una impresion 
	//	parcial, solo se evaluara la primera vez.
	
	// Util en impresiones parciales, mantien el alto del container ya impreso.

	DvcUnit_t m_HeightPrinted ;

	ClENG_Expression m_oCancel ;

	// Auxiliar, reintentos sin poder imprimir nada.
	m4int16_t m_iAttemp ;

MIT_PUBLIC:

	// Publico para usar con total libertad. El Init lo resetea y el Show lo muestra.
	
	ClENG_Region m_oRegion ;

MIT_PUBLIC:

	//-------------------------Inicializaciones-----------------------------------------

	ClENG_Container();
	virtual ~ClENG_Container() { ClENG_Container::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_Container) ; }

	// Los objetos pasados por refererencia son COPIADOS en el container. 
	// Utilizar uno de estos dos constructores, segun sea X_Y_FIXED o no.
	// Si numero de pagina fuera de rango, error immediato.
	void InitFixed( 	
		StENG_ReportEnv *ai_poReportEnv,
		m4int16_t ai_iPage, DvcUnit_t ai_X, DvcUnit_t ai_Y, 
		DvcUnit_t ai_Height, DvcUnit_t ai_Width, const m4char_t * ai_pcCancel		
	) ;
	void InitNotFixed( 	
		StENG_ReportEnv *ai_poReportEnv,
		m4int16_t ai_iPage, DvcUnit_t ai_X, 
		DvcUnit_t ai_Height, DvcUnit_t ai_Width,
		DvcUnit_t ai_Y, DvcUnit_t ai_SizeYLimit,
		m4bool_t ai_SwTogether, const m4char_t * ai_pcCancel
	) ;

	// VIRTUAL INIT.
	// Si el field se sale de los limites de altura o anchura impuestos por el container, 
	//	provocara una WARNING.
	// Los field pasados por refererencia deben estar alocados, esta clase los borrara.

	void InitField( ClENG_Field *ai_poField ) ;

	// Borra tanto el array, como los objetos a los que apunta.
	
	virtual void End() ;

	// Especial, se usa en Preload. Lo que hace es imprimir y BORRAR todos los field que 
	//	cumplan la siguiente condicion:
	//	-Si Optimize, deben ser estaticos y optimizados.
	//	-Si !Optimize, deben ser estaticos y no optimizados.
	// Siempre los imprime en el X/Y del container
	
	void PrintPreload( m4bool_t ai_bOptimize ) ;

	//Pasa el PreAnalex a todos field.
	// Da true si todo va bien. Si hay problemas, da false y genera errores locales.

	m4bool_t PreAnalex( ClENG_Analex::StExpressionEnv *ai_pstEnv ) ;


	//-------------------------Gets-----------------------------------------------------

	m4int16_t GetNFields() { return m_oField.size(); }
	m4bool_t IsEmpty() { return M4_BOOL( GetNFields()==0 ); }
	m4bool_t IsXYFixed() { return m_oFlag.True(X_Y_FIXED) ; }
	m4bool_t IsTogether() { return m_oFlag.True(TOGETHER) ; }
	m4int16_t GetNumPage() { return m_iPage ; } 
	DvcUnit_t GetX() { return m_X; }
	DvcUnit_t GetY() { return m_Y; }
	DvcUnit_t GetHeight() { return m_Height; }
	DvcUnit_t GetWidth() {  return m_Width ; }
	DvcUnit_t GetSizeYLimit() { return m_SizeYLimit; }
	STATE GetState() { return m_State ; }

	//-------------------------Ciclo de vide normal--------------------------------------

	//Ejecuta la cancelacion container. Si se cancela, pasa a estado PRINTED y no 
	//	hace nada. Se hace antes que nada para que los load de canal que disparan los
	//	NewPrint ya hayan corrido el filtro, ya que hay reports que -INDEBIDAMENTE- usan
	//	el filtro para ejecutar metodos que posicionan el canal.
	//Corre el NewPrint para todos campos
	//Ajusta posible alto variable.
	//Pasa a estado START_PRINTING

	void NewPrint( ) ;

	// Solo hace algo si estamos en el estado START_PRINTING o PARTIAL_PRINTED.
	// Muestra el container. Pueden suceder estas cosas:
	//  -No se imprime nada porque no hay sitio. Se sigue en el estado en que se estaba.
	//  -Se imprimio todo, no queda nada por imprimir. Se pudo volcar todo, 
	//		BaseLine += m_Height o si es una impresion parcial, lo realmente impreso. 
	//		Se pasa a estado PRINTED.
	//  -Quedan cosas pendientes de imprimir. El Container es de tipo TOGETHER=FALSE, 
	//		se imprimio un trozo, es preciso saltar a otra pagina, y quedan cosas pendientes 
	//		de imprimir. 
	//		BaseLine+=Trozo impreso. El siguiente Print imprimira un trozo a partir de donde
	//		se quedo este Print. Para imprimir un Container pueden ejecutarse N Prints,
	//		no solamente dos.
	//
	//  Para todo container NOT X_Y_FIXED:
	//		-La X es fija, se imprime a partir de m_Y.
	//		-La m_Y es valida si m_Y>=BaseLineY. Si m_Y<BaseLineY, m_Y=BaseLineY. 
	//		-El area desde m_Y con m_SizeYLimit de alto marca el ultimo m_Y valido.
	//			Si a pesar de todo, m_Y sobrepasa este limite, se notificara EMPTY_PRINT, 
	//			solicitando asi un EOP.
	//		-Si se detecta que se ha caido en bucle infinito de peticion de EOPs, se 
	//			generara un error de efecto local, y se devolvera FULL_PRINT

	void Print( ) ;

	//-------------------------Auxiliares de proceso--------------------------------------
MIT_PRIVATE:
	
	//Teniendo en ceunta el estado del container, devuelve el ai_CntInterval ajustado para 
	//	no imprimir de forma parcial fields que no pueden partirse.
	//Puede devolver 0 si esto no es posible.

	DvcUnit_t AdjustCntInterval(DvcUnit_t ai_CntInterval)  ;

	//----------------------------------------------------------------------------------
};

//============================================================================ClENG_CntGroup

class ClENG_CntGroup  
{
MIT_PRIVATE:

	StMIT_Inspector* m_poInsp;	//Simple link
	StENG_ReportEnv *m_poReportEnv;	//Simple link

	ClMIT_Str m_oLocation ;	//Simple cadena de trace location. La pone el Init y no cambia.
	
	//-------------------------Miembros-------------------------------------------------

MIT_PUBLIC:

	enum STATE {
		NOT_INITIALIZED,
		INITIALIZED,
		LOADING_CONTAINERS,
		WAITING_FOR_PRELOAD,
		START_PRINTING,
		NOT_ALL_PRINTED
	} ;

	//  Lista de containers.
	//  Los conatiners deben estar alocados. Esta clase los desaloca.
	typedef list<ClENG_Container  *> CntList_t;	

MIT_PRIVATE:

	CntList_t m_oList ;
	STATE m_State ;

MIT_PUBLIC:

	//-------------------------Inicializaciones-----------------------------------------

	ClENG_CntGroup();
	virtual ~ClENG_CntGroup() { ClENG_CntGroup::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_CntGroup) ; }
	virtual void End() ;

	void Init(StENG_ReportEnv *ai_poReportEnv, m4char_t *ai_pcLocationStr) ;

	//ai_poCnt debe estar alocado. Esta clase los desaloca.
	void AddCnt( ClENG_Container *ai_poCnt ) ;


	//-------------------------Operacion------------------------------------------------

	//Simplemente aplica el metodo en toda la lista.
	void PrintPreload( m4bool_t ai_bOptimize ) ;
	m4bool_t PreAnalex( ClENG_Analex::StExpressionEnv *ai_pstEnv ) ;

	m4bool_t AreAllXYFixed() ;
	m4bool_t AreAllNotXYFixed() ;

	//  Llama a los Print asociados, Simplifica la tarea de impresion. Devuelve TRUE solo 
	//cuando todos los containers se han procesado, esto es, cuando ninguno esta en estado
	//EMPTY_PRINT o PARTIAL_PRINT. Dicho de otra forma, si devuelve false se precisa un EOP.
	//  Cuando un Print de un container de FULL_PRINT o CANCELED_PRINT, su metodo no volvera
	//a ser llamado hasta que el resto de containers esten en el mismo estado. 

	m4bool_t Print( ) ;

	m4int16_t GetCount() { return m_oList.size(); }
	STATE GetState() { return m_State ; }

	//Recorre la lista, util para testeos o reajustes en containers.
	//It posibilita recorridos en multithread. No hace falta usarlo explicitamente para nada,
	//	ya que se devuelve NULL cuando se esta en el fin de la lista.
	//Osea, para hacer recorridos:
	//	Cnt=Obj.NextCnt(It,ClENG_CntGroup::GO_FIRST) ;
	//	while (Cnt) {
	//		//Procesar Cnt
	//		Obj.NextCnt(It,ClENG_CntGroup::NEXT) ;
	//	} ;

	enum NEXT_OPERATION { GO_FIRST, NEXT } ;
	ClENG_Container *NextCnt( CntList_t::iterator &ao_It, NEXT_OPERATION ai_Ope) ;

	//NULL si no hay containers
	ClENG_Container *GetFirst() ;
	ClENG_Container *GetLast() ;

	//Uso inmediato para traza
	ClMIT_Str *GetLocation() { return &m_oLocation; } 
} ; 

//============================================================================ClENG_Section

class ClENG_Section {

MIT_PRIVATE:

	StMIT_Inspector* m_poInsp;	//Simple link
	StENG_ReportEnv *m_poReportEnv	;	//Simple link
	
	friend class ClENG_Report ;

	ClENG_Report *m_poReport ;	//link
	ClMIT_Str m_oDesc ;
	ClMIT_IdNumDesc m_oLocation ;	//Simple cadena de trace location. La pone el Init y no cambia.

	//-------------------------Miembros-------------------------------------------------
MIT_PUBLIC:

	//Modos BEFORE sobre Cnt inexistentes deben convertirse a modos AFTER
	enum BREAK_MODE  { NONE_BREAK, BEFORE_CONTAINER_BREAK, AFTER_CONTAINER_BREAK } ;
	enum HEADER_MODE { ONLY_FIRST_PAGE, H_ALL_PAGES, EXCEPT_FIRST_PAGE } ;
	enum FOOTER_MODE { ONLY_LAST_PAGE,  F_ALL_PAGES, EXCEPT_LAST_PAGE } ;

MIT_PRIVATE:

	//---------Gestión del arbol

	// Identificador de seccion
	
	m4uint32_t m_iIdSec ;
	
	// Enlace con padre. Si NULL, nodo raiz.

	ClENG_Section *m_poFather ;

	// Si TRUE, la seccion es sincronizada 

	m4bool_t m_bSynchronized ;
	
	//  Array dinamico de punteros secciones hijas (que pueden ser ClENG_Section o ClENG_EndSection).
	//Las secciones se alocan fuera de esta clase, pero esta clase se encargará de borrar
	//solo el array. Las secciones las borrara en orden correcto ClENG_Report.
	//  Un nodo ClENG_EndSection no tiene hijos. Siempre tendrá su NSons=0.
	//  Tambien aloca y borra Parallel.
	//  Las secciones se imprimen por orden secuencial, desde el m_poSom[0] al n. Bloques
	//consecutivos de secciones que tengan el mismo Parallel se ejecutan paralelamente. 
	//Ejemplo:
	//	m_pstSon[6]  ={ 
	//		{&SecA, 500}, {&SecB,500}, {&SecC,1} , 
	//		{&SecD, 300}, {&SecE,300}, {&SecF,1} 
	//} ;
	//Imprimiria : primero, en paralelo,  SecA y SecB, luego SecC, luego en paralelo 
	//   SecD y SecE, y por ultimo SecF.  

	m4int16_t     m_iNSons ;
	struct StSon {
		ClENG_Section     *m_poSon ;						 
		m4int16_t     m_iParallel ;
	} *m_pstSon ;

	//---------Influencia sobre fields e items

	ClENG_FieldExternProp m_oFieldMod ;
	ClMIT_ItemExternProp  m_oItemMod ;

	//---------Zona Before. Se procesa antes de imprimir el primer registro de nodo.

	ClENG_CntGroup    *m_poBeforeCnt ;	//Permitidos X_Y_FIXED o NOT X_Y_FIXED.
	BREAK_MODE    m_BeforePageBreak;

	ClENG_Expression m_oPrepareRecordset ;
	ClENG_Expression m_oResetBeforeTotal ;

	//--------Zona Header. Se procesa al comenzar una pagina nueva o una seccion.

	ClENG_CntGroup *m_poHeaderCnt ;		//Siempre X_Y_FIXED.
	HEADER_MODE m_HeaderMode ;

	// Antes de imprimir el header.
	ClENG_Expression m_oResetHeaderTotals ;

	//--------Zona VR ( PreVR & VR & PostVR )

	//PreVR se imprime antes de imprimir el VR, y PostVR despues. 
	//Permitidos X_Y_FIXED o NOT X_Y_FIXED.
	
	ClENG_CntGroup * m_poPreVRCnt, * m_poPostVRCnt ;

	// Container que se usa por registro. 
	// Para secciones finales debe ser NOT X_Y_FIXED si aparece, y es opcional. Ademas, 
	//	en dichas secciones, los limites que tiene el container son los usados para 
	//	imprimir el detalle final.

	ClENG_CntGroup * m_poVRCnt ;

	// Si TRUE, tras cada registro mete un Break. Si es el ultimo registro, no lo mete: 
	//	esto puede implementarse en el After.

	m4bool_t m_bBreakVR ;

MIT_PRIVATE:

	ClENG_Expression m_oSetPostVRTotal ;

	//--------Zona Footer. Se procesa al acabar paginas, antes de saltar a una nueva o al 
	//		acabar la seccion.

	ClENG_CntGroup * m_poFooterCnt ;		//Siempre X_Y_FIXED.
	FOOTER_MODE m_FooterMode ;

	//---------Zona After. se ejecuta tras haber procesado todos los registros del nodo.

	ClENG_CntGroup   * m_poAfterCnt ;	//Permitidos X_Y_FIXED o NOT X_Y_FIXED.
	BREAK_MODE    m_AfterPageBreak;

	//-------------------------Recorrido & VRs------------------------------------------

	//--------------Parametrización de recorrido

	friend class ClENG_VR ;

MIT_PUBLIC:
	
	ClENG_VR m_oVR ;	//Objeto public solo para consultas

	//	Solo actua en secciones finales. Si en la pagina actual no caben todos 
	//VR, EOP. Si se da esta situacion, y se empezó a imprimir en el comienzo del area VR, 
	//entonces se ignora esta caracteristica, porque caeriamos en un bucle sin fin al no 
	//poder imprimirse nunca todos VR juntos y sacar EOPs sin fin.
	//	Los distintos flags indican qué se considera que se imprime por VR, a efectos de 
	//este calculo.Todos los container indicados deben estar en la misma pàgina, ser 
	//variables, y tener los mismos limites. Si no es asi, se generara error. 
	//Los containers indicados que no existan, o sean fijos, simplemente son ignorados, en 
	//vez de generar error.
	//	Si hay together, hay incondicionalmente prerecorrido VR_COUNT.


MIT_PUBLIC:
	enum {
		TOG_CNT_BEFORE=ClMIT_Flag::BIT0, 
		TOG_CNT_PRE_VR=ClMIT_Flag::BIT1, TOG_CNT_VR=ClMIT_Flag::BIT2, 
		TOG_CNT_POST_VR=ClMIT_Flag::BIT3, TOG_CNT_AFTER=ClMIT_Flag::BIT4
	} ; //La mascara se usa al crear la seccion. Luego ya no hace falta.

MIT_PRIVATE:
	DvcUnit_t m_TogCntHeight,	//Auxiliar, se calcula. 0, no hay together
		m_TogBeforeHeight, m_TogAfterHeight ; //Auxiliares 
	m4int16_t m_iTogPage ; DvcUnit_t m_TogTop, m_TogBottom ;	//Auxiliares
	m4bool_t m_bTogBeforeDone ;	//Auxiliar

	//-------------Datos obtenidos en el recorridos y pre-recorrido

	//  GetNumPageDvc(0) en el momento de entrar en la seccion. Sirve para calcular el 
	//		numero de pagina actual de seccion.
	m4uint32_t m_uiPageSec ;
	
MIT_PUBLIC:

	//-------------------------Inicializaciones-----------------------------------------

	ClENG_Section();
	virtual ~ClENG_Section() { ClENG_Section::End(); }

	virtual size_t GetSizeof() { return sizeof(ClENG_Section) ; }

	// Los ClENG_Container son linkados al objeto: deben de estar alocados, porque esta clase
	//	se encargara de borrarlos (incluso si Init falla).
	// Si los container header o footer se pasan y no son XYFixed, error.
	// Actualmente Init usa g_poDvc, por lo que este debe estar ya inicializado.
	// El IdSec debe ser unico por todos las secciones, aunque esto no es testeado. Para lo
	//	unico que sirve es para la funcion ClENG_Report::FindSec. Si los datos de IdSec se pasan
	//	mal, lo unico que sucede es que esta funcion operara de forma incorrecta.
	// Cualquier modo BreakBeforeContainer sobre un container inexistente, se convierte a un 
	//	modo AfterBeforeContainer.

	// Devuelve true si todo va bien.
	// Si no hay secciones hijas, pone ai_bNullOnSons incondicionalmente a FALSE.

	m4bool_t Init( 
		ClENG_Report *ai_Report, 
		StENG_ReportEnv *ai_poReportEnv,
		ClMIT_IdNumDesc *ai_poIdDesc, ClENG_Section *ai_poFather, m4int16_t ai_iNSons,  
		ClENG_VR::NULL_IF_EMPTY ai_NoPrintEmpty, 

		ClENG_CntGroup * ai_poBeforeCnt, BREAK_MODE ai_BeforePageBreak, 
		ClENG_CntGroup * ai_poAfterCnt,  BREAK_MODE ai_AfterPageBreak, 
		ClENG_CntGroup * ai_poHeaderCnt, HEADER_MODE ai_HeaderMode, 
		ClENG_CntGroup * ai_poFooterCnt, FOOTER_MODE ai_FooterMode, 

		ClENG_CntGroup * ai_poPreVRCnt,  ClENG_CntGroup * ai_poVRCnt, 
		ClENG_CntGroup * ai_poPostVRCnt,

		const m4char_t * ai_pcPrepareRecordset,
		const m4char_t * ai_pcCancelSection,
		const m4char_t * ai_pcResetHeaderTotals,
		const m4char_t * ai_pcCancelVR,
		const m4char_t * ai_pcResetBeforeTotal, 
		const m4char_t * ai_pcSetPostVRTotal, 

		ClENG_VR::GO_THROUGH_MODE ai_GoT, ClENG_VR::RANGE_MODE ai_RangeMode,
		ClMIT_ChnNode * ai_poRecordNode, ClMIT_ChnItem * ai_poGuideItem,
		ClENG_VR::StGroup::GroupKey_t * ai_poGroupKey, 

		m4bool_t ai_bSwSynchronized, m4bool_t ai_bBreakVR, 
		m4bool_t ai_bNullOnSons,     ClMIT_Flag::Value_t ai_MaskTogether,

		ClENG_VR::OneOneList_t *ai_poOneOne,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR
	) ;

	// Error inmediato si NSon fuera de rango o ya inicializado. 
	// Ejecutar solo para secciones no finales, o da error inmediato.
	// Las secciones deben estar alocadas. Esta clase se encarga de borrarlas.

	void InitLinks( m4int16_t ai_iNSon, 
		ClENG_Section * ai_poSon, m4int16_t ai_iParallel) ;
	
	// Antes de borrar la propia seccion, borra las secciones hijas que tenga linkadas.

	virtual void End() ;

	//Recursiva, preanaliza la seccion propia, y luego las hijas.
	//Se preanalizan todos containers y expresiones de la sección.
	//Respecto del contexto, pone estos defectos:
	//	-m_poDefaultNode (al nodo de la propia seccion)
	//	-m_LoadMode, siempre a ITEM_LOAD.
	//	-m_poItemExternProp y m_poFieldExternProp a los de la seccion.
	//	-m_poSect, a la propia.
	//Tambien llama al PreAnalex de ClENG_VR.
	//Da true si todo va bien.

	m4bool_t PreAnalex( ClENG_Analex::StExpressionEnv *ai_pstEnv ) ;

	//-------------------------Logica de impresion--------------------------------------

MIT_PRIVATE:

	enum REENTRY_MODE { 
		NONE_REENTRY, NONE_REENTRY_1,
		DETAIL_REENTRY, DETAIL_REENTRY_1, DETAIL_REENTRY_2, DETAIL_REENTRY_3,
		END_REENTRY,
		BEFORE_REENTRY, AFTER_REENTRY
	} ;

	enum RETURN_SECTION { END_SEC_EOP, END_SEC_NOT_EOP,  NOT_END_SEC_EOP, NOT_END_SEC_EOP_DUPLEX, END_SEC_NOT_EOP_DUPLEX} ;

	//Util en trace, dan una cadena correspondiente al enum
	static void GetTraceStr( RETURN_SECTION ai_Enum, ClMIT_Str *ao_poStr ) ; 


MIT_PUBLIC:
	// PrintSection. Se llama una vez por registro, pero con posibles salidas y reentradas. 
	//	Procesa la impresion de todas las zonas y, respecto a la impresion de un registro,
	//	si no es nodo final secuencia la impresion de las secciones hijas llamandose 
	//	recursivamente. Ver antes el algoritmo para saber más detalles.
	// Para poder ser reentrante, se apoya en el miembro Reentry, que es actualizado, a la 
	//	hora de inicializarse, por los nodos padres, y, a la hora de procesar las reentradas,
	//	por el propio nodo. Tambien se apoya en SonReentry, para saber el camino de 
	//	reentrada.
	// Reentry puede ser (hay mas valores posibles menos importantes):
	//		-NONE_REENTRY. Primera vez que se entra.
	//		-BEFORE_REENTRY. Se reentra tras satisfacer un EOP disparado por before.
	//		-AFTER_REENTRY. Se reentra tras satisfacer un EOP disparado por after.
	//		-DETAIL_REENTRY. Se reentra tras satisfacer un EOP disparado por la zona de 
	//			detalle (ya sea de seccion final o no). 
	// Cuando se dispara un EOP, este metodo va retornando y poniendo los
	//	footer y el modo de reentrada. Cuando se llega al raiz, se hace un flush, se prepara
	//	una nueva pagina, y se va llamando otra vez al thread de ejecucion afectado, que
	//	va reentrando, poniendo los header, y limpiando el modo de reentrada.
	// Al entrar, restura la posicion, lo que evita interferencias entre secciones que compartan
	//	nodo.
	// Aunque la seccion se cancele, siempre se dispara el ResetBeforeTotals, ya que este es el
	//	unico sitio donde hay actualizacion de totalizadores que pueden usarse en secciones padre.
	// Los valores de retorno son:
	//	-NOT_END_SEC_EOP. La seccion aun tiene datos pendientes de salir, pide un salto de 
	//		pagina. Puede que se este en EOF, pero aun haya cosas pendientes de imprimir. 
	//	-END_SEC_EOP. La sección ha impreso todo lo que tenia que imprimir, y pide un EOP.
	//	-END_SEC_NOT_EOP. Como END_SEC_EOP, pero no se pide un EOP.
	//
	// *******Aqui esta la secuencia en la que se realizan las acciones******
	//
	//	--------------Una vez por recordset---------------
	//
	//	-EntryInRecordSet()			------------>Cancelacion CancelRecordSet o NullIfEmpty
	//	-Rewind() (aunque sea EOF, no cancela)
	//	-Print del HeaderCnt y ejecucion ResetHeaderTotals (cnt fijos, no hay EOPs)
	//	-EOPTogether o BeforePageBreak==BEFORE_CONTAINER_BREAK	------------>1 EOP (nunca ambos)
	//	-Impresion del BeforeCnt	------------------------------->n EOPs por impresion parcial
	//	-Ejecucion ResetBeforeTotal
	//	-BeforePageBreak==AFTER_CONTAINER_BREAK	(*1)-------------------------------------->1 EOP 
	//
	//	--------------n veces por recordset (detalle)---------------
	//
	//	-EOPTogether en detalle  --------------------------------------------------------->1 EOP 
	//	-Bucle procesamiento registros recordset while( !IsEOF() )
	//		-Impresion del PreVRCnt	------------------------------->n EOPs por impresion parcial
	//		-Impresion del VRCnt  --------------------------------->n EOPs por impresion parcial
	//		-Impresion secciones hijas	(*2)----------------------->n EOPs request de sec. hijas
	//		-Impresion del PostVRCnt  ----------------------------->n EOPs por impresion parcial
	//		-Ejecucion SetPostVRTotal
	//		-Si IsBreakVR() && !IsLastVR()  ----------------------->1 EOP por VR, excepto ultimo
	//		-Next() y  reseteos Next
	//	-Fin bucle 
	//
	//	--------------Una vez por recordset---------------
	//
	//	-AfterPageBreak==BEFORE_CONTAINER_BREAK	------------------------------------------>1 EOP 
	//	-Impresion del AfterCnt	----------------------------------->n EOPs por impresion parcial
	//	-AfterPageBreak==AFTER_CONTAINER_BREAK	------------------------------------------>1 EOP 
	//	-Print del FooterCnt (cnt fijos, no hay EOPs)
	//
	//  Para evitar imprimir hojas en blanco, PrintSection realiza unos testeos adicionales para
	//ver si debe transformar un NOT_END_SEC_EOP en un END_SEC_EOP. Estas situaciones son:
	//	-*1. Solo si el EOP es por BeforePageBreak==AFTER_CONTAINER_BREAK (si es por BEFORE, 
	//		forzosamente hay container de Before), SE ESTA EN EOF, y no hay container  
	//		de After, se convierte el NOT_END_SEC_EOP en END_SEC_EOP. Ademas, antes de salir se
	//		ejecuta:
	//		-Print del FooterCnt (cnt fijos, no hay EOPs). El Header ya se imprimio.
	//	-*2. Tras procesar las secciones hijas, puede pasar esto:
	//		-Si alguna pide NOT_END_SEC_EOP, hay un NOT_END_SEC_EOP forzoso, ya que a alguna 
	//			hija le falta algo por imprimir.
	//		-Si TODAS piden END_SEC_NOT_EOP, no hay EOP por el momento, y el bucle sigue su
	//			curso normal.
	//		-Si alguna pide END_SEC_EOP (y ninguna NOT_END_SEC_EOP, en todo caso 
	//			END_SEC_NOT_EOP), hay que ver si estamos en el ultimo VR (IsLastVR). Si es asi,
	//			y ademas NO hay PostVRCnt NI AfterCnt, se convierte el NOT_END_SEC_EOP en 
	//			END_SEC_EOP. Ademas, antes de salir se ejecuta:
	//			-Ejecucion SetPostVRTotal.
	//			-Print del FooterCnt (cnt fijos, no hay EOPs)

	RETURN_SECTION PrintSection( ) ;

	//Auxiliar PrintSection
	m4bool_t IsEOPTogether(m4bool_t ai_bSwBeforeHanging) ;

MIT_PRIVATE:
	
	//Todas las impesiones de container, a nivel de seccion, deben de pasar por este metodo.
	//Subsana un problema, y es el siguiente:
	//	-Cuando hay varias secciones subagrupadas, estan tiran del mismo nodo de datos.
	//	-Cuando una subagrupada no tiene datos, o se cancela por CancelSection, NullIfEmpty, 
	//		o NULLOnSons, la seccion en cuestion se pone en EOF.
	//	-En teoria, esta seccion puede imprimir datos de la seccion padre; pero como 
	//		fisicamente el nodo es el mismo, este esta en EOF y generara errores.
	//	-Lo que hace este metodo es que, si se esta en EOF, y es un subgrupo, antes de 
	//		imprimir restaura la posicion del padre, y despues de imprimir vuelve a dejar
	//		todo como estaba. Esto tambien habilita el acceso a posibles datos de secciones
	//		'abuelas' subagrupadas, ya que en este caso tiran del mismo nodo, y cualquier 
	//		registro que se subagrupa contine la clave completa.
	//	-Devuelve lo mismo que ClENG_CntGroup::Print().

	m4bool_t PrintCntGroup( ClENG_CntGroup *ai_CntGroup ) ;

MIT_PUBLIC:

	
	//------Para procesar breaks

	// Todo el proceso es controlado por ClENG_Report::Go
	// Pone breaks de pagina en break record, y en after. En after, si no hay break mete
	//	el modo AFTER_CONTAINER_BREAK, y si lo hay respeta el existente. Limpia el flag.

	void PrepareBreakSection() ;

	void SetBreakFlag() { m_bBreakRequest=M4_TRUE ; }
	void ClearBreakFlag() { m_bBreakRequest=M4_FALSE ; }
	m4bool_t IsBreakFlag() { return m_bBreakRequest ; }

MIT_PRIVATE:

	//-----Auxiliares de proceso para reentradas

	m4int16_t m_iSonReentry ;	//Indice a m_poSons, el actual que se esta procesando.
	REENTRY_MODE m_Reentry;		//Dice en que zona reentrar. 
	m4bool_t   m_bSwPreVR ;	//TRUE si PreVR pendiente de salir
	m4bool_t   m_bSwPostVR ;//Idem PostVR
	m4bool_t   m_bSwVR ;	//Idem VR

	//Para controlar que se ejecute el ResetBeforeDone acunque se cANCELEN EXPRESIONES.
	m4bool_t m_bResetBeforeTotalsDone ;	

	// Lo debe poner a TRUE PrintSection cada vez que se produzca un cambio de registro.
	m4bool_t	m_bBreakRequest ;

	//-----Auxiliares de proceso para control de baseLines.

	DvcUnit_t *m_BLPrivate ;		//m_BLPrivate[NumPagLayout]
	DvcUnit_t *m_BLSave ;			//m_BLSave[NumPagLayout]
	DvcUnit_t *m_BLMax;				//m_BLMax[NumPagLayout]	
	m4int16_t  m_iCountBLItems ;

	/*
	BLPagLayout. Una caracteristica comun a todos devices es que tienen una BL por page.
		Esto se almacena en el propio device. El nombre que he dado a este array teorico 
		es ficticio, solo sirve para explicar mas facilmente el funcionamiento del 
		algoritmo. 
	m_BLPrivate contiene las BL actuales y privadas de una seccion asincrona. Solo es 
		utilizado este array por este tipo de secciones.
	m_BLSave contiene las BL actuales y comunes para secciones sincronas. El array es 
		utilizado cooperativamente por secciones asincronas y por cualquier seccion con
		secciones hijas.
	m_BLMax almacena los maximos para las BL de secciones sincronas. Solo es utilizado
		por secciones con hijos.

	*** ALGORITMO usado para controlar las BL ***

	-Esta implementado en ClENG_Section::PrintSection.
	-La impresion de cualquier container fijo ni altera ni tiene en cuenta las BL_PagLayout.
	-Un container variable se imprime desde la actual BL_PagLayout, y altera esta para que
		sucesivos containers variables (o impresiones parciales del mismo container) se 
		impriman a continuacion.
	-Las m_BLPrivate son puestas a 0 por cada salto de hoja en ClENG_Report::Go(). 
		Las BLPagLayout tambien se ponen a 0 indirectamente en este punto, debido a que
		las resetea el device al recibir este un EOP.

	ClENG_Section::PrintSection() {

		if ( ! IsSynchronized() ) {
			m_BLSave <-- BLPagLayout ;	//Salva las BL para secciones sincronas
			BLPagLayout <---- m_BLPrivate ;	//Carga la BL privada.
		} else {
			//No hace nada; se imprime desde la BLPagLayout
		} ;

		....Impresiones de before y otras alteran BLPagLayout
		
		if ( IsEndSection() ) {

			SobrecargaVirtual::PrintDetail() ; //AHORA HA DESAPARECIDO PRINTDETAIL, ACTUALIZAR

			....Impresion de detalle alteran las BLPagLayout
			
		} else {

			if ( ! IsSynchronized() ) {
				m_BLPrivate <------ BLPagLayout ;	//Salva BL privada, m_BLSave ya guardada.
			} else {
				m_BLSave <-------- BLPagLayout ;	//m_BLSave pendiente de salvar.
			} ;

			m_BLMax <--- 0 ;	//Reseteo de maximos

			for (Para todas Secciones Hijas Paralelas) {

				BLPagLayout <---- m_BLSave;	 //Todas paralelas sincronas desde la misma BL

				SecHijaParalela.PrintSection() ;

				....Impresion de secciones hijas alteran las BLPagLayout SOLO si alguna 
					seccion de la sub-hebra es sincrona.

				m_BLMax <---- MAX( m_BLMax, BLPagLayout );	//Maximos para sincronizar luego

			} ;
	  
			if ( ! IsSynchronized() ) {
				m_BLSave <--- m_BLMax;	//Guarda BL sincronas para restaurarlas a la salida
				BLPagLayout <--- m_BLPrivate ;	//Restaura BL privada.
			} else {
				BLPagLayout <--- m_BLMax;	//Sincroniza
			} ;

	
		} ;

		....Impresiones de after y otras alteran BLPagLayout

		if ( ! IsSynchronized() ) {

			m_BLPrivate <----- BLPagLayout;	//Salva BL privadas.
			BLPagLayout <----- m_BLSave ;	//Recupera BL para secciones padre sincronas.

		} else {

			// No se hace nada. Secciones padre sincronas usan la BLPagLayout

		} ;
	}

	*/

	inline void BLSaveOnEntry() ;	
	inline void BLPreLoopSon() ;
	inline void BLPrePrintSon() ;
	inline void BLPostPrintSon() ;
	inline void BLPostLoopSon() ;
	inline void BLRestoreOnExit() ;	

	void PrintFooter() ;
	void PrintFooter(ClENG_VR::StSave *ai_Pos) ;
	void PrintHeader() ;

	void UpdateBaseLineY(ClENG_CntGroup * ai_CntGroup);
MIT_PUBLIC:
	
	//--------------------------------------Gets----------------------------------------

	m4bool_t IsEndSection() { return M4_BOOL( m_iNSons==0 ); }
	m4bool_t IsTogether() { return M4_BOOL( m_TogCntHeight>0 ); }

	m4int16_t GetNumSons() { return m_iNSons ; }

	m4bool_t IsSynchronized() { return m_bSynchronized ; } 
	m4uint32_t GetIdSec() { return m_iIdSec ; }

	m4bool_t IsBreakVR() { return m_bBreakVR ; }

	// Da el numero de pagina de seccion correspondiente a la ai_iPage de Layout 
	//	(PageLayout va de 0 a n, PageSec de 1 a n).
	
	m4uint32_t GetNumPageSec(m4int16_t ai_iPage) ;

	ClENG_Report *GetReport() { return m_poReport ;	}

	m4bool_t CheckRecordSetPosition ( m4bool_t ai_bForceReentry = M4_FALSE ) ;

MIT_PRIVATE:

	//----------------------------------------------------------------------------------

} ;

#endif



//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             corevr.h
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
//    Subconjunto del nucleo funcional de los reports, genera recorridos VR.
//
//
//==============================================================================

#ifndef __COREVR__H__
#define __COREVR__H__

/*

  El engine trabaja con registros virtuales (VRs). Un VR puede estar fisicamente formado por:

-Un unico registro fisico.
-Una agrupacion de registros fisicos.
-Un slice, osea, un registro fisico con el item guia posicionado en un tramo en concreto. 

  Hay varias capas de recorrido. Cada una usa los servicios de su capa anterior y recubre a 
esta con nueva funcionalidad.

-Capa fisica. Es la capa de canal.
-ClENG_VR, recorrido basico de VR. Se procesa:
	-GoT y RangeMode.
	-CancelVR.
	-OneOne.
	-Groups.
	-PrepareRecordSet.
	-CancelRecordSet.
	-Servicio TestNRecord() para recordsets finales, util para Together.
-ClENG_VR, filtros especiales.
	-Modos NULL_IF_EMPTY. Cancelan recordsets enteros. Ademas de recorrer el actual recordset,
		puede ser necesario recorrer y filtrar recordsets hijos.
	-Modos NullOnSons. Cancela el VR actual haciendo comprobaciones NULL_IF_EMPTY en los 
		hijos.

	
RECORRIDO BASICO
-----------------------------------------------------------------------------------------------	  

 Limitaciones a tener en cuenta:

-Cargas parciales de canal. Hay que evitar en la medida de los posible recorrer muchos registros
	hacia adelante para volver a uno previo. Admisible pequeños recorridos de uno o hacia 
	adelante y otro hacia atras.
-Siempre se deberia de mover registros fisicos dentro del actual recordset, y solo mover 
	recordsets hijos cuando se va a descender a una seccion para procedsar esta.

Combinaciones permitidas:

-GoT=Record y Range=All. 
-GoT=Record y Range=Subgroup. Requiere de seccion padre que sea tipo GoT=Record y con
	Group. Ambas secciones tiran del mismo nodo y lo recorren cooperativamente.
-GoT=Slice y Range=All. Requiere de una seccion padre de cualquier tipo, que le marque
	el registro. Jamas mueve este registro , sino que mueve slices dentro de el.
-LAS AGRUPACIONES SOLO ESTAN PERMITIDAS para GoT=Record !!!!

  Asi pues, vamos a mostrar los posibles casos que nos podemos encontrar. Se muestra un recordset
en el cual un x es un registro fisico u slice invalido (que no pasa algun filtro), un * es 
uno valido, un ? es que da igual que sea o no valido, no se procesa. El numero representa el 
numero de registro absoluto. 
Los registros agrupados con | forman grupo, el area delimitada entre >>> es el rango valido de
registros impuesto por el Group padre.

A) MODOS NO Group

  A.1 Record,All,No Group   A.2  Record,SubGroup,No Group

	0 x					>>>	0 x
	1 *					>>>	1 *
	2 x						2 x
	3 *						3 *
	4 x						4 x
	5 *						5 *
	6 *						6 *
	7 x					>>> 7 x


  A.3 Slice,All,No Group (recorre slices del record padre)

	0 x						
	1 *						
	2 x						
	3 *						
	4 x						
	5 *						
	6 *						
	7 x						


B) MODOS Con Group, Record forzoso

	B.1 Record,All,Group					B.2 Record,SubGroup,Group

	  0 x									>>>>> 0 x
	A|1 *									>>>>> 1 x
	 |2 ?	Posibles SubGroup					A|2 *	Posibles SubGroup
	 |3 ?	---------------->					 |3 ?   ---------------->
	  4 x										  4 x
	B|5 *										B|5 *
	 |6 ?										 |6 ?
	  7 x									>>>>> 7 x

*****Formacion de VR en cada caso****

			Records (o slices)
		-----------------------------------
VRs		A.1		A.2		A.3		B.1		B.2
------  ---		---		---		---		---
1       1		3		1		1-3		2-3
2       3		5		3		5-6		5-6
3       5		6		5		-		-
5       6		-		6		-		-

*****Recorrido de los VR*************

El recorrido generico de un VR es:

//Disparado una vez, prepara el recordset para que sea recorrido. Puede cancelarlo
Pos.EntryInRecordSet() ; 

//Puede recorrerse las veces que se desee, sin necesidad de hacer un EntryInRecordSet().
Pos.Rewind() ; 
while ( !Pos.IsEOF() ) {
	//Proceso actual VR (y posibles recordsets hijos). 
	//Si hay grupos, puede estar en cualquier registro del grupo, da igual cual, mientras 
	//	pertenezca al grupo.

	//Opcional, si no se hace lo calculara Next(). Haya siguiente, pero no se cambia a él
	//Si hay grupos, y no se ha hayado el ultimo en el grupo, tambien lo hayara.
	Pos.FindNext() ;	

	//Se cambia al hayado por FindNext() (lo ejecuta previamente si no se hizo).
	Pos.Next() ;
} ;	




*/

#include "m4stl.hpp"
#include "xapichn.h"
#include "analex.h"

class ClENG_VR ;
class ClENG_Section ;

class ClENG_VR {

MIT_PRIVATE:

	StMIT_Inspector* m_poInsp;	//Simple link

	//-------------------------Miembros-------------------------------------------------

MIT_PUBLIC:

	enum GO_THROUGH_MODE { RECORD_GOT, SLICE_GOT } ;
	enum RANGE_MODE { ALL_VR, SUBGROUP_VR } ;
	enum NULL_IF_EMPTY { NEVER_NULL, THIS_SECTION, SON_OR_SECTIONS, SON_AND_SECTIONS  } ;

MIT_PRIVATE:

	//Simple Link
	//NULL si raiz. Debe tener padre si GoT=Slice (cualquier padre) o 
	//	Range=SubGroup (padre con GoT=Record). 
	ClENG_VR *m_poVRFather ;

	//Simple link.
	//Jamas es NULL, apunta a la seccion propietaria de esta clase. Util para saber de ClENG_VR
	//	hijas sitas en secciones hijas de las propietarias.

	ClENG_Section * m_poOwnerSection ;

	//Combinaciones permitidas:
	//-GoT=Record y Range=All. 
	//-GoT=Record y Range=Subgroup. Requiere de seccion padre que sea tipo GoT=Record y con
	//	Group. Ambas secciones tiran del mismo nodo y lo recorren cooperativamente.
	//-GoT=Slice y Range=All. Requiere de una seccion padre de cualquier tipo, que le marque
	//	el registro. Jamas mueve este registro , sino que mueve slices dentro de el.
	//LAS AGRUPACIONES SOLO ESTAN PERMITIDAS para GoT=Record !!!!

	GO_THROUGH_MODE m_GoT ;
	RANGE_MODE m_Range ;

	//Nodo selector de records, obligatorio, simple link.
	//Copiado de la padre si GoT=Slice o Range=SubGroup.

	ClMIT_ChnNode *m_poRecordNode ; 	

	//Item guía, obligatorio si GoT=Slice, ignorado si GoT=Record.

	ClMIT_ChnItem m_oGuideItem ;		

	//m_ActualRecord siempre se mantiene en el record actual (RECORD_EOF si en EOF). En grupos,
	//	siempre es igual al ultimo valor m_oGParam.m_NearestToLast producido por un Rewind() o
	//	por un SubGroupInRange(), y no por un FindNext(). Todo este rollo es para no provocar
	//	saltos innecesarios con carga parcial.
	//m_NextRecord solo esta actualizado si asi lo indica m_bNextVRCalculated.
	//Con m_RewindRecord pasa lo ismo, esta actualizado si lo esta m_bRewindVRCalculated. 
	//	Sirve para optimizacion en Rewind(), para que mas de una llamada a este metodo solo haga
	//el filtro y busqueda en la primera llamada.
	//Un VR con GoT=Slice siempre COPIA los numeros de registro de su ClENG_VR padre, sin JAMAS 
	//	alterar la posicion del padre. 
	
	ClMIT_ChnNode::Record_t m_ActualRecord ;
	ClMIT_ChnNode::Record_t m_NextRecord ; m4bool_t m_bNextVRCalculated ;
	ClMIT_ChnNode::Record_t m_RewindRecord ; m4bool_t m_bRewindVRCalculated ;


	//Solo se usa si GoT=Slice.
	//Homologo al anterior, usa tambien m_bNextVRCalculated y m_bRewindVRCalculated.

	ClMIT_ChnItem::SliceIndex_t m_ActualSlice, m_NextSlice, m_RewindSlice ;	

MIT_PUBLIC:
	struct StGroup {

		//LAS AGRUPACIONES SOLO ESTAN PERMITIDAS para GoT=Record !!!!

		//m_oGroup contiene la lista de items que forman la clave para agrupar. La concatenacion
		//	de sus m_oValue contienen la clave para el GRUPO ACTUAL.
		//La clase debe destruir los objetos alocados.
		//Para m_oAuxCompare aloca y copia otros ClMIT_ChnItem.

		typedef list<ClMIT_ChnItem *> GroupKey_t;
		GroupKey_t m_oGroup, m_oAuxCompare  ;

		//m_FirstInG es el primer registro en el grupo, y esta siempre actualizado (puede ser
		//	RECORD_EOF solo si m_ActualRecord lo es). Sin embargo, m_LastInG puede no estar
		//	actualizado, si esta en RECORD_EOF quiere decir que no esta actualizado, y si 
		//	tiene algun valor significa que	alguna seccion hija con Range=SubGroup o algun 
		//	otro calculo ha provocado que se haye el fin del grupo. 
		//m_NearestToLast contiene al principio m_FirstInG, o el ultimo registro que
		//	pertenece al grupo y que esta mas cercano a m_LastInG. Por supuesto, una vez se
		//	haya m_LastInG, este valor es igual a m_LastInG. Util para optimizar recorridos
		//	con grupos y subgrupos.

		ClMIT_ChnNode::Record_t  m_FirstInG, m_NearestToLast, m_LastInG ;	

	} ;

MIT_PRIVATE:

	StGroup m_oGParam ;
	

MIT_PRIVATE:

	//  Lista de nodos relacionados uno a uno. Cada vez que nos movemos de registro de 
	//recordset (no VR) en la sección, se hace un GoFirst en los nodos especificados en 
	//la lista.
	//	Puede usarse siempre, aunque el GoT=Slice.
	//  Es importante el orden en que se ejecutan. Este viene impuesto por la clave.
	//  Sirve para acceder a items que están en nodos con relaciones uno a uno, y evitan
	//la necesidad de crearse secciones para ese unico proposito.
	//	Guarda links, no hay que borrar los objetos.
MIT_PUBLIC:
	typedef map<m4int16_t, ClMIT_ChnNode *, less<m4int16_t> > OneOneList_t;
MIT_PRIVATE:
	OneOneList_t m_oOneOne ;

	//Expresion ejecutada siempre que se entra a procesar un recordset (independientemente 
	//	del GoT).

	ClENG_Expression m_oPrepareRecordset ;

	// m_oCancelRecordset se ejecuta en el EntryInRecordSet, y si es TRUE se cancela el 
	//	recordset. A todos los efectos, es como si este estuviese vacío.

	ClENG_Expression  m_oCancelRecordset ;

	// Tras posicionarnos en el VR, se ejecuta este filtro. Si TRUE, el VR se ignora.
	// La relacion OneOne (si la hay) ya se ha realizado antes de pasar este filtro, por lo
	//	que es licito usar items de estos nodos para filtrar.

	ClENG_Expression  m_oCancelVR ;

	//Contador de VR procesados desde el Rewind. Dicho de otra forma, numero de Next realizados.

	m4uint32_t m_VRProcessed ; 

	//Simple link

	ClMIT_ItemExternProp *m_poItemMod ;

	//-------------------------Inicializaciones-----------------------------------------

MIT_PUBLIC:

	ClENG_VR();
	virtual ~ClENG_VR() { ClENG_VR::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_VR) ; }
	virtual void End() ;

	m4bool_t Init( 
		ClENG_Section * ai_poOwnerSection,
		ClENG_VR *ai_poVRFather,
		ClMIT_ItemExternProp *ai_poItemMod,

		GO_THROUGH_MODE ai_GoT, RANGE_MODE ai_RangeMode,
		ClMIT_ChnNode * ai_poRecordNode, ClMIT_ChnItem * ai_poGuideItem,

		StGroup::GroupKey_t * ai_poGroupKey,

		OneOneList_t *ai_poOneOne,

		const m4char_t * ai_pcPrepareRecordset,
		const m4char_t * ai_pcCancelRecordset,
		const m4char_t * ai_pcCancelVR,

		NULL_IF_EMPTY 	ai_NullIfEmpty,
		m4bool_t    ai_bNullOnSons,

		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) ;

	//Preanalizaciones
	m4bool_t PreAnalex( ClENG_Analex::StExpressionEnv *ai_pstEnv ) ;

MIT_PUBLIC:

	//-------------------------ClENG_VR, recorrido basico de VR-----------------------------
	/*
	Simplificando, y como primera aproximacion, lo que se hace para formar un VR es:
	-Se localiza el primer record fisico (registro u slice) que pase el CancelVR. Antes se
		realiza el OneOne, para que sea valido un filtro que use items movidos por el OneOne.
	-Este registro el el primero del grupo. NUNCA MAS se ejecutan CancelVR dentro de 
		los registros que pertenecen al grupo.
	-El siguiente Next saltara primero todos los registros del grupo, y localizara el primero
		de un siguiente grupo repitiendo el ciclo.
	-Por supuesto, si no hay grupos es como si todos los registros fueran un grupo por si solos.
	*/

	//Se ejecuta para preparar el recordset. Solo hace falta hacerlo una vez, aunque el 
	//	recordset se recorra n veces.
	//Retorna FALSE si cancela el recordset, o hay algun problema.
	//Ejecuta, por este orden:
	//	-m_oCancelRecordset. 
	//	-m_oPrepareRecordset.
	//  -El parametro de entrada ai_NullIfEmpty dice como ejecutar el NullIfEmpty. 

MIT_PRIVATE:
	m4bool_t EntryInRecordSet(NULL_IF_EMPTY ai_NullIfEmpty) ; 

MIT_PUBLIC:
	m4bool_t EntryInRecordSet() { return EntryInRecordSet(m_NullIfEmpty); } 


	//Opcional, si no se pasa lo pasara Next.
	//CUIDADO, el metodo nos deja por defecto en el siguiente registro.
	//Varios FindNext seguidos o estar en EOF no tendran efecto de calcular nada;solo lo tiene 
	//	el primer FindNext. Lo que si hacen es dejarnos posicionados en el Next.

	void FindNext() ;	

	void Rewind() ;
	m4bool_t Next() ;

	m4bool_t IsEOF() ;
	m4bool_t IsNextEOF() ;	//Valido si ya se hizo un FindNext. TRUE si actual o siguiente EOF.

	// Dice si el VR actual es el ultimo. Para ello, calcula si debe  el FindNext(), aunque si 
	//	dispara este, se restaura la posicion actual antes de salir.

	m4bool_t IsLast() ;

	m4bool_t IsGrouped() { return M4_BOOL( m_oGParam.m_oGroup.size() > 0 ); }

	//Esta funcion simplemente mueve la posicion fisica a donde le digamos, y hace el OneOne.
	//Para evitar problemas con carga parcial, en ClENG_VR con grupos fijarse que el ActualRecord 
	//	restaurado es uno de los ultimos valores NearestToLast.
	enum RELOCATE_MODE { RELOC_ACTUAL, RELOC_NEXT } ;
	void ReLocate(RELOCATE_MODE ai_Mode=RELOC_ACTUAL) ;

	// *** Grupos y subgrupos
MIT_PRIVATE:

	//Auxiliar de Next() (y SOLO de Next). Solo se llama si hay agrupaciones, y ya se debe estar 
	//	posicionado en el registro a testear.
	//Los subgrupos ejecutan este metodo del VR padre, para preguntarle si el registro fisico
	//	esta en rango o no. Siempre se llama antes de cualquier CancelVR o cualquier otra cosa 
	//	que pueda saltar registros, ya que por el padre deben pasar TODOS los registros del hijo
	//	CONSECUTIVAMENTE, para detectar correctamente de esta forma el m_LastInG.
	//Estas acciones pueden provocar la actualizacion en el padre de m_NearestToLast o 
	//	m_LastInG con el valor del registro actual, asi como del ActualRecord.
	//El metodo se encarga de que la posicion fisica no sea alterada. 
	//Pueden producirse cascadas de llamadas a SubGroupInRange
	//Da TRUE si se esta en rango.

	inline m4bool_t SubGroupInRange() ;

	//Auxiliar, hace el OneOne sobre el registro actual DEL RECORDSET.

	inline void DoOneOne() ;

	//Auxiliar, carga la lista m_oGroup o de m_oAuxCompare segun el registro actual 
	//	DEL RECORDSET.
	//Si ai_LoadAuxCompare, ademas compara una lista con otra, dando TRUE si los valores
	//	son iguales. Si !ai_LoadAuxCompare siempre da M4_TRUE.
	//Si la calve anterior no contenia valores nulos y la actual si, o viceversa, se 
	//	considera un cambio de grupo. Si las dos claves eran NULL, se consideran el mismo
	//	grupo, independientemente de si los valores no-NULL coinciden o no.

	inline m4bool_t LoadGroupKey(m4bool_t ai_LoadAuxCompare=M4_FALSE) ;


MIT_PUBLIC:
	//****Otros

	//Util en trace, dan una cadena correspondiente al enum

	static void GetTraceStr( GO_THROUGH_MODE ai_Enum, ClMIT_Str *ao_poStr ) ; 
	static void GetTraceStr( RANGE_MODE ai_Enum, ClMIT_Str *ao_poStr ) ; 

	//Si la expresion no está vacia, la ejecuta. Da igual el valor devuelto.

	static void		ExecuteAction( ClENG_Expression *ai_poExpr ) ;

	//Si la expresion no está vacia, la ejecuta. El tipo de vuelta debe ser boolean, o el
	//	modulo APICHN producira un error local. 
	//Devuelve el valor de la expresion, o false si no hay filtro o se produce un error local.

	static	m4bool_t	ExecuteFilter( ClENG_Expression *ai_poExpr ) ;

	//-------------------------ClENG_VR, filtros especiales---------------------------------

MIT_PRIVATE:

	//  Cancela recordsets segun encuentre o no registros.
	//  Se ejecuta en EntryInRecordSet, osea n Rewinds adicionales no provocan llamadas extra a
	//esta propiedad.
	//  Puede provocar recorridos parciales de recordsets tanto en el VR padre como en la rama
	//hija.
	//	Ahora mismo se testean todas las hijas, sean o no del mismo thread de impresion.
	//	-NEVER_NULL. Nunca comprueba nada, siempre es una seccion no vacia.
	//	-THIS_SECTION. Comprueba que haya algun registro solo en esta seccion. Si hay 
	//		alguno, la seccion es valida, independientemente de que haya secciones hija 
	//		sin nada. 
	//	-SON_OR_SECTIONS. Comprueba que haya algun registro que tenga registros en las 
	//		secciones hijas. 
	//		(Logicamente nunca podra haber si no hay en la padre, por lo que implica 
	//		tambien un THIS_SECTION). 
	//		Si hay algun registro en alguna seccion hija, la seccion es valida.
	//	-SON_AND_SECTIONS. Como antes, solo que para ser valida la seccion debe haber 
	//		algun registro que tenga registros en TODAS las secciones hijas.

	NULL_IF_EMPTY 	m_NullIfEmpty ;

	//  Auxiliar de EntryInRecordSet. Va llamando a su vez a los EntryInRecordSet de las
	//secciones hijas.

	m4bool_t IsNullIfEmpty(NULL_IF_EMPTY ai_NullIfEmpty) ; 

	//  Cancela VR actual si todas todas las VR hijas están vacías.  Si está a true, en el 
	//registro actual se testea que las VR hijas tengan algun VR valido, usando para ello 
	//los modos NullIfEmpty de las hijas. Concretamente, se mira el modo de NULL_IF_EMPTY de 
	//las hijas, y si el modo es NEVER_NULL, se testean en modo THIS_SECTION, y si no es 
	//NEVER_NULL, se testean en el modo original de la VR hija.

	m4bool_t    m_bNullOnSons ;

MIT_PRIVATE:
	// El metodo, de por si, nunca mueve la posicion actual. Llama, como IsNullIfEmpty, a los
	//	EntryInRecordSet de los ClENG_VR hijos.
	m4bool_t IsNullOnSons() ; 

MIT_PUBLIC:

	//Devuelve TRUE si al menos hay ai_uiNRecords disponibles, incluyendo el actual (se mueve a
	//	la posicion actual).
	//A la salida restaura la posicion fisica a la actual.

	m4bool_t TestAlmostNRecords( m4uint32_t ai_uiNRecords ) ;

	//---------------------------------Gets-----------------------------------------------------

	GO_THROUGH_MODE GetGoT() { return m_GoT ; }
	RANGE_MODE GetRangeMode() { return m_Range ; }
	ClMIT_ChnNode *GetRecordNode() { return m_poRecordNode ; }	//Puntero solo para consulta.
	ClMIT_ChnItem *GetGuideItem() { return &m_oGuideItem ; } //Idem

	ClMIT_ChnNode::Record_t GetActualRecord() { return m_ActualRecord; }

	m4uint32_t GetVRProcessed() { return m_VRProcessed ; }

	//Este bloque obtiene informacion de slices. Si recorrido no es de slice, error local.
	m4date_t GetFirstDate() ;
	m4date_t GetLastDate() ;
	m4date_t GetStartDate(ClMIT_ChnItem::SliceIndex_t ai_Index) ;
	m4date_t GetEndDate(ClMIT_ChnItem::SliceIndex_t ai_Index) ;
	ClMIT_ChnItem::SliceIndex_t GetSliceIndex()  ;	//Da m_ActualSlice
	ClMIT_ChnItem::SliceIndex_t GetSliceCount() ;

MIT_PUBLIC:
	//Para salvar y restaurar posiciones (no el objeto entero). 
	struct StSave {

		ClMIT_ChnNode::Record_t m_ActualRecord ;
		ClMIT_ChnNode::Record_t m_NextRecord ; m4bool_t m_bNextVRCalculated ;
		ClMIT_ChnNode::Record_t m_RewindRecord ; m4bool_t m_bRewindVRCalculated ;

		ClMIT_ChnItem::SliceIndex_t m_ActualSlice, m_NextSlice, m_RewindSlice ;	

		//El valor de clave se recarga una vez nos reposicionamos
		ClMIT_ChnNode::Record_t  m_GroupFirstInG, m_GroupNearestToLast, m_GroupLastInG ;	

		m4uint32_t m_VRProcessed ; 

		//Los rangos de slice en m_poItemMod tambien se recargan una vez nos reposicionamos
	} ;


	void Save(StSave *ao_poSave) ;
	void Restore(StSave *ao_poSave) ;

MIT_PRIVATE:

	//IMPORTANTE: Este metodo simplemente llama al ClMIT_ChnNode::MoveRec, excepto si el record es
	//	EOF, en cuyo caso no hace nada. 
	//Esto es para, en caso de subagrupaciones y similares, cuando hay varios ClENG_Vr que tiran del
	//	mismo nodo, y hay fines de recordset logicos, NO irse al EOF, porque cuando el ClENG_VR 
	//	padre se mueva por el sigueinte registro de grupo, provocara cargas parciales.
	//Por ELLO:
	//	-Los usuarios de ClENG_VR y la propia siempre testean el EOF por ClENG_VR::IsEOF() ó 
	//		ClENG_VR::IsNextEOF(), jamas por ClMIT_ChnNode::GetNumRecord()==ClMIT_ChnNode::EOF_RECORD

	inline void MoveToRec(ClMIT_ChnNode::Record_t ai_Record) ;
} ;
	


#endif





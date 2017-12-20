//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             trace.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:11/11/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//	Gestiona todo el trace. 
//
//==============================================================================

#ifndef __TRACE__H__
#define __TRACE__H__

#include "m4clflag.h"
#include "clfile.h"


//TODAS ESTAS CLASES TIENEN EL ACCESO LIMITADO A INSPECTOR


/*
	Nuestra traza puede tener dos tipos de salida, simultaneas si queremos. 
Las salidas disponibles son:
	-Fichero traza. Generado en local, orientado a debug y modos verbose. Saca
		tanto mensajes de error, warning y debug. Orientado a desarrollador y
		back-end.
	-M4Log. Orientado a sacar mensajes de error y warning, y cuantos menos mejor.
		Orientado a usuario final y front-end.
	
	Los datos de entrada para generar los mensajes son los mismos, lo unico que 
varia es el formato de salida y el destino de esta. 

	DATOS DE ENTRADA

	-Cadena de formateo multilingue. El formato es parecido a un printf, y consiste en:
		-%Ordinal:s. Se sustituye esto por el parametro indicado por ordinal. Los parametros
			pueden reutilizarse varias veces, en el orden de aparicion que queramos. Si un
			parametro no se encuentra, se sustituye por una cadena en blanco.
			Cuidado con esta cadena si son mensajes que iran a front-end: es la cadena que se
			usara, no meter aqui tecnicismos ni nada parecido en ese caso..
		-Parametros de tipos sobrecargados por << (luego se ve con mas detalle).
		-Ejemplos: "No se encuentra nodo %0:s item %1:s (nodo %0:s)"
	
	DATOS DE SALIDA:

	-Salida a fichero. Se escribe la cadena ya formateada al fichero. 
	-Salida a M4Log. Se empila el codigo de error y una cadena con todos los parametros. 
		Esta cadena, en nuestro caso, siempre tiene datos tipo cadena (por eso al formatear
		solo tenemos disponible el %s). El formato es:
		"#*Nnnn*S1*#Par0#Par1#...#"
		*Nnnn es opcional, e indica el numero de mensaje usado para obtener el testo de
			formateo. Si no se indica, se usa el empilado en la M4Log.
		*S1* busca la cadena de formateo en el logsys.ini, *S0* en canal.
		Se pueden concatenar mensajes, que saldran como uno solo, con ||
		Siempre debe acabar la cadena con #. Un #*S0*# no tiene parametros, un #*S0*## tiene
		un parametro vacio.
		Dentro de uno de estos parametros pueden especificarse pares de valores 
		identificador-descripcion con este formato:
		#Identificador&&Descripcion#
		-Ejemplos: "#*S0*#Par0#IdPar1&&DescPar1#Par2#||#*N100*S0*#Par0#"

	Para formar los mensajes se hara uso de <<. Sobre este operador se sobrecargan los tipos 
de datos reconocibles por el formateo. La forma que tiene este operador de sobrecargarse es
por el tipo de entrada, y este no tiene en cuenta typedefs, pero si enums, structs y clases.
De ahi la necesidad de crear ciertas estructuras adicionales para distinguirlas en &&.
	Como el orden de los parametros y su interpretacion por salida puede variar, estos se
van guardando hasta que se saca completamente el mensaje, momento en el que se procede a los
formateos que competan en cada caso.

*/

class ClMIT_Trace ;
struct StMIT_Inspector ;

//=========================================================================ClMIT_TraceCheck
/*
Esta clase no debe tener ninguna dependencia de ninguna otra. Debe poderse usar exista o no
objetos trace, etc.
El prefijo EC significa contabilizaciones de errores o warnings en External Components.
El prefijo IC es para Internal Component
*/

class M4_DECL_M4MIENV ClMIT_TraceCheck{

MIT_PUBLIC:

	m4uint32_t m_uiErrorCount, m_uiEC_ErrorCount ;	
	m4uint32_t m_uiWarningCount , m_uiEC_WarningCount ;
	m4uint32_t m_uiDummyObjects ;


	void Reset() { m_uiErrorCount=m_uiWarningCount=m_uiDummyObjects=
		m_uiEC_ErrorCount=m_uiEC_WarningCount=0; }

	ClMIT_TraceCheck() { Reset(); }
	ClMIT_TraceCheck(const ClMIT_TraceCheck &ai_Ope) ;
	ClMIT_TraceCheck & operator = (const ClMIT_TraceCheck & ai_Ope) ;
	ClMIT_TraceCheck & operator - (const ClMIT_TraceCheck & ai_Ope) ;
	ClMIT_TraceCheck & operator + (const ClMIT_TraceCheck & ai_Ope) ;

	//Contabilizan errores del propio componente y de otros tambien.

	m4uint32_t GetErrorCount() { return m_uiErrorCount+m_uiEC_ErrorCount ; }
	m4uint32_t GetWarningCount() { return m_uiWarningCount+m_uiEC_WarningCount ; }

	m4uint32_t GetOkCount() { return m_uiErrorCount+m_uiWarningCount+m_uiDummyObjects+
		m_uiEC_ErrorCount+m_uiEC_WarningCount; } 

	m4bool_t OnProblem() { return M4_BOOL(GetOkCount() > 0); }
	m4bool_t AllOk() { return M4_BOOL(!OnProblem()); }
	m4bool_t IsError() { return M4_BOOL(GetErrorCount() > 0); }
	m4bool_t OnlyWarnings() { return M4_BOOL(GetErrorCount() == 0 && GetWarningCount() > 0); }

	// Suelen servir solo para prints. 

	m4uint32_t GetEC_ErrorCount() {   return m_uiEC_ErrorCount ; }
	m4uint32_t GetEC_WarningCount() { return m_uiEC_WarningCount ; }
	m4uint32_t GetIC_ErrorCount() {   return m_uiErrorCount ; }
	m4uint32_t GetIC_WarningCount() { return m_uiWarningCount ; }
	m4uint32_t GetDummyCount()      { return m_uiDummyObjects ; }

} ; 


//===============================================================> Items a procesar por <<

//Todos estos items se ponen fuera de ClTrace por comodidad. Son items que aparecen en
//muchos sitios, y seria tedioso poner el ClTrace:: siempre antes.

// Los TRACE_COMMANDS no crean nuevos parametros, simplemente efectuan acciones sobre
//	los datos ya empilados o ponen flags de formateo que se aplicaran cuando se haga el
//	send.
// En caso de enviar varios comandos excluyentes, siempre se coje el ultimo. Por ejemplo, 
//	un ... << ENTRY_MSG << EXIT_MSG equivale a un ... << EXIT_MSG.

enum TRACE_MSG_COMMAND {

	//----------->Formateo de mensaje

	//Solo tiene efecto en salida a fichero. Formatea como punto de entrada a funcion, e
	//indenta (excluyente con EXIT_MSG). 
	ENTRY_MSG,	

	//Solo tiene efecto en salida a fichero. Formatea como punto de salida a funcion, y
	//quita una indentacion. (excluyente con  ENTRY_MSG)
	EXIT_MSG,	

	//Evita el wrapping del mensaje incondicionalmente. Un ENTRY_MSG u EXIT_MSG activan 
	//	incondicionalente este flag.

	NO_WRAP_MSG,

	//----------->Nivel de detalle

	//  El nivel del mensaje  es un valor de peso. Es decir, un DETAILED_MSG tambien es un 
	//GENERAL_MSG, mientras que un CRAZY_MSG tambien es un DETAILED_MSG y GENERAL_MSG.
	//  Estos valores tienen significado para mensajes de debug, para decir que nivel de
	//debug queremos y poder obviar mensajes por niveles. No tienen sentido ni para warnings
	//ni errores, ya que estos siempre deben notificarse.

	GENERAL_MSG,	//DEFECTO.
	DETAILED_MSG,
	CRAZY_MSG,

	//---------->Comandos 

	//Envia el mensaje actual y resetea los valores defecto para el siguiente mensaje.
	SEND_MSG,

	END_LOC,	//Para locations, marca fin de localizacion: los parametros metidos son de la 
				//localizacion actual.
	POP_LOC	//Para locations, elimina un location.
} ;


//==================================================================================> ClMIT_IdDesc

class M4_DECL_M4MIENV ClMIT_IdDesc {
MIT_PRIVATE:
	const m4char_t *m_pcId, *m_pcDesc ;	//Simples links
MIT_PUBLIC:
	ClMIT_IdDesc() { m_pcId = m_pcDesc = "?" ; }
	ClMIT_IdDesc( const m4char_t *ai_pcId, const m4char_t *ai_pcDesc ) 
		{ m_pcId=ai_pcId ; m_pcDesc=ai_pcDesc; }	

	void Init( const  m4char_t *ai_pcId, const  m4char_t *ai_pcDesc ) 
		{ m_pcId=ai_pcId ; m_pcDesc=ai_pcDesc; }	

	void Init( ClMIT_IdDesc * ai_poFrom )  
		{ m_pcId=ai_poFrom->m_pcId ; m_pcDesc=ai_poFrom->m_pcDesc; } 

	const m4char_t *GetId() const { return m_pcId ; }
	const m4char_t *GetDesc() const { return m_pcDesc ; }
} ;

class M4_DECL_M4MIENV ClMIT_IdNumDesc : public ClMIT_IdDesc {

MIT_PUBLIC:
	enum { ID_STR_SIZE=14 } ;
MIT_PRIVATE:
	m4char_t m_IdNum[ID_STR_SIZE] ;
MIT_PUBLIC:
	ClMIT_IdNumDesc() { m_IdNum[0]=M4_END_STR ; }
	ClMIT_IdNumDesc( m4uint32_t ai_uiId, const m4char_t *ai_pcDesc ) ;
	void Init( m4uint32_t ai_uiId, const  m4char_t *ai_pcDesc ) ;
	void Init( ClMIT_IdNumDesc * ai_poFrom )  ;
	m4uint32_t GetIdNum() const ;
} ;


//==================================================================================> ClMIT_Msg 
// Tambien ClMIT_Msg es un item a procesar por <<.
//  Esta clase almacena los datos que se entienden que son inherentes al mensaje, que siempre
//deben de saberse.


class M4_DECL_M4MIENV ClMIT_Msg {
MIT_PUBLIC:
	friend class ClMIT_Trace ;

	//BE_OUT y FE_OUT son usados tambien como indices
	enum MSG_OUT { BE_OUT, FE_OUT, MIXED_OUT, OUT_COUNT=2 } ;

	typedef m4int16_t  MsgGroup_t ;
	typedef m4uint32_t MsgCode_t ;

	enum MSG_TYPE { 
		MSG_ERROR, MSG_WARNING, MSG_DEBUG, //Tambien usados estos valores como indices
		TYPE_MSG_COUNT						//Count del numero de tipos, uso interno
	} ;

	enum { 
		//  Si se activa, el error o warning no contabiliza. Util para errores o warnings con
		//el unico proposito de notificacion a capa front-end de otros errores back-end ya
		//producidos.

		SOFT_NOTIFY=ClMIT_Flag::BIT0,	//Tambien se puede enviar via <<

			
		//Estos falgas estan pensados para ser metidos via <<, aunque pueden meterse a pelo
		ENTRY_FORMAT=ClMIT_Flag::BIT9,	
		EXIT_FORMAT= ClMIT_Flag::BIT10,	//Por defecto estan vacios.
		NO_WRAP=ClMIT_Flag::BIT11,		

		EC_MSG=ClMIT_Flag::BIT12,	//Interno, no usar. Si true, msg provocado por un EC


		NONE_BITS=ClMIT_Flag::NONE_BITS 
	} ; 

	//Este valor se mete via <<
	enum MSG_LEVEL { 
		IGNORE_ALL, //Usar este valor solo con grupos
		GENERAL, DETAILED, CRAZY 
	} ;

MIT_PRIVATE:

	MsgGroup_t m_Group;
	MsgCode_t  m_Code ;
	MSG_TYPE   m_Type ;
	MSG_OUT    m_Out  ;	
	MSG_LEVEL  m_MsgLevel ;
	//Ojo, es un simple link. El objeto apuntado debe existir hasta que el mensaje salga.
	const m4char_t  *m_pcMsg ;	

	ClMIT_Flag m_Flag ;

MIT_PUBLIC:

	ClMIT_Msg() { m_pcMsg=NULL ; } 
	ClMIT_Msg( const ClMIT_Msg *ai_From ) { Init(ai_From) ; } 
	void Init( const ClMIT_Msg *ai_From ) ;

	//Constructor generico. Raramente utilizado
	ClMIT_Msg(
		MSG_TYPE ai_Type, MsgGroup_t ai_Group, MsgCode_t ai_Code, const m4char_t  *ai_pcMsg,
		MSG_OUT ai_Out=ClMIT_Msg::BE_OUT, MSG_LEVEL ai_MsgLevel=GENERAL, 
		ClMIT_Flag::Value_t ai_Flags=ClMIT_Flag::NONE_BITS 
	) ;

	// Mas normal que la anterior. pone el codigo de mensaje a uno auxiliar, visible por la
	//	m4log
	ClMIT_Msg(
		MSG_TYPE ai_Type, MsgGroup_t ai_Group, const m4char_t  *ai_pcMsg,
		MSG_OUT ai_Out=ClMIT_Msg::BE_OUT, MSG_LEVEL ai_MsgLevel=GENERAL, 
		ClMIT_Flag::Value_t ai_Flags=ClMIT_Flag::NONE_BITS 
	) ;

	//Constructor para mensajes debug:
	//ai_Type=MSG_DEBUG
	//ai_Code=Sin relevancia, se pone un codigo auxiliar del grupo de trace.
	//ai_Out=BE_OUT.

	ClMIT_Msg(
		MsgGroup_t ai_Group, const m4char_t  *ai_pcMsg, ClMIT_Flag::Value_t ai_Flags=ClMIT_Flag::NONE_BITS 
	) ;

	//Constructor para warnings y errores. Pillan los valores de los arrays estaticos pertinentes.

	struct StMsgInfo { 
		MsgGroup_t m_Group ;
		MsgCode_t  m_Code ;
		MSG_TYPE m_Type ;
		MSG_OUT m_Out ;
		ClMIT_Flag::Value_t m_Flags ;
		const m4char_t  *m_pcMsg ;
	} ;
	ClMIT_Msg( const StMsgInfo *ai_Info) ;

} ;

//================================================================================> ClMIT_PushLoc

class M4_DECL_M4MIENV ClMIT_PushLoc {
MIT_PRIVATE:
	ClMIT_Msg::MsgCode_t  m_Code ;
	//Ojo, es un simple link. El objeto apuntado debe existir hasta que el mensaje salga.
	const m4char_t  *m_pcMsg ;	
MIT_PUBLIC:
	friend class ClMIT_Trace ;

	ClMIT_PushLoc() { m_pcMsg = "" ; }
	ClMIT_PushLoc( const ClMIT_Msg::StMsgInfo *ai_Info ) { m_Code=ai_Info->m_Code ;  m_pcMsg=ai_Info->m_pcMsg ; } 
	ClMIT_PushLoc( ClMIT_Msg::MsgCode_t ai_Code, const m4char_t  *ai_pcMsg ) { m_Code=ai_Code ;  m_pcMsg=ai_pcMsg ; }
} ;

//==================================================================================> ClMIT_StrNLink

class M4_DECL_M4MIENV ClMIT_StrNLink {
MIT_PRIVATE:
	//Ojo, es un simple link. El objeto apuntado debe existir hasta que el mensaje salga.
	const m4char_t  *m_pcMsg ;	
	size_t m_LinkStrLen ;	//Numero de caracteres validos
MIT_PUBLIC:
	friend class ClMIT_Trace ;

	ClMIT_StrNLink() { m_pcMsg = "" ; }
	ClMIT_StrNLink( const m4char_t  *ai_pcMsg, size_t ai_LinkStrLen ) 
		{ m_pcMsg=ai_pcMsg ; m_LinkStrLen=ai_LinkStrLen ; } 
	void Init( const m4char_t  *ai_pcMsg, size_t ai_LinkStrLen ) 
		{ m_pcMsg=ai_pcMsg ; m_LinkStrLen=ai_LinkStrLen ; } 
} ;


//==================================================================================> ClMIT_Trace
/*
	Unas notas respecto a la capacidad de poner cadenas de localizacion.
	-NO olvidarse que, despues de enviar un << ClMsgLoc(), hay que hacer un END_LOC.
	-Entre el ClMsgLoc() y el END_LOC puede enviarse cualquier parametro, pero NUNCA un
		nuevo mensaje o un SEND_MSG ni nada parecido. De la misma forma, dentro de la creacion
		de un mensaje normal no pueden meterse mensajes de localizacion.
	-Mensajes creados en subfunciones u otros sitios pueden acceder sin problema a la pila de
		localizaciones.
	-El uso mas habitual es que la funcion que empile n parametros desempile n parametros antes
		de salir.
*/

class M4_DECL_M4MIENV ClMIT_Trace {

MIT_PROTECTED:

	StMIT_Inspector *m_poInsp ;	//Simple link

	//----Parametros del mensaje que se esta procesando
MIT_PUBLIC:
	enum { MAX_ITEMS_PARAM=80, MAX_SIZE_PARAM=30 } ;	

MIT_PRIVATE:

	ClMIT_Msg m_oMsg ;	//Msg que se esta procesando.

	struct StParam {								
		enum { LINK_STR, NUMERIC, ID_DESC, BOOLEAN, SINGLE_CHAR } m_Type ;
		union {
			const m4char_t *m_pcLinkStr ;	
			m4char_t m_pcNumStr[MAX_SIZE_PARAM] ;	//Numero en formato cadena
			const m4char_t *m_pcIdDesc[2] ;	//[0] Identifier, [1] Description
			m4bool_t m_bSw ;
			m4char_t m_cChar ;
		} ;
		//Usado por LINK_STR, si -1 ASCIIZ, si no solo los caracteres indicados
		size_t m_LinkStrLen ;	
	} m_stParamArray[MAX_ITEMS_PARAM] ;	//Array de paramatros del actual Msg.
	m4int16_t m_iParamCount, m_iLocParamCount ;	

MIT_PUBLIC:
	m4int16_t GetMsgParamCount() { return m_iParamCount-m_iLocParamCount ; }
	m4int16_t GetMsgFirstParam() { return m_iLocParamCount ; }

	//----Move Trace

MIT_PUBLIC:

	//Al hacer el End se hace el Move (si debe). ON_SUCCESS se dispara si no hay errores ni
	//	warnings, ON_PROBLEM si hay cualquiera error o warning y ON_ERROR solo si hay errores.
	//Por defecto, a NEVER_MOVE.

	enum MOVE_TRACE_MODES { 
		NEVER_MOVE, MOVE_ALWAYS, MOVE_ON_SUCCESS, MOVE_ON_PROBLEM, MOVE_ON_ERROR 
	} ;
	enum {SIZE_MOVE_FILE=2000} ;

MIT_PRIVATE:

	MOVE_TRACE_MODES m_MoveMode ;

	//NO puede ser ClStr o destructor tendra problemas al no existir el objeto trace
	m4char_t m_oMoveFile[SIZE_MOVE_FILE] ;	


	//----Push-Pop Location

	//Para guardar referencias los Push y Pop. 

MIT_PUBLIC:
	enum { LOCATION_ITEMS=40 } ;
MIT_PRIVATE:
	struct {
		ClMIT_Msg::MsgCode_t m_MsgCode ;
		const m4char_t *m_pcMsgStr ;	//Simple link
		m4int16_t m_iFirstParam ;	
		m4int16_t m_iParamCount ;	
	} m_stLocation[LOCATION_ITEMS] ;
	m4int16_t m_iLocationCount ;

	//---Flags que varian por tipo de mensaje (error,warning,...) y por salida.

MIT_PUBLIC:

	enum TYPE_MSG_FLAGS { 
		SHOW_TYPE    =ClMIT_Flag::BIT0,	//Si true, se añade una cadena con el tipo de mensaje
		SHOW_CODE    =ClMIT_Flag::BIT1,	//Si true, se añade el codigo de mensaje
		SHOW_GROUP   =ClMIT_Flag::BIT2,	//Si true, se añade el codigo de grupo
		ADD_LF       =ClMIT_Flag::BIT4,	//Si true, se añade un LF al final del mensaje.
		ALWAYS_LEVEL =ClMIT_Flag::BIT5,	//Si true, siempre se saca el mensaje independientemente 
										//	del valor DETAIL_LEVEL del grupo al que pertenece.
		ALWAYS_OUT   =ClMIT_Flag::BIT6,	//Si true, siempre se saca el mensaje en este output
										//	independientemente del valor Out del mensaje.
		PUSH_LOCATION=ClMIT_Flag::BIT7	//Si true, añade la pila de cadenas de localizacion 

	} ;	

	//Objeto directamente modificable

	ClMIT_Flag m_oTypeMsgFlags[ClMIT_Msg::TYPE_MSG_COUNT][ClMIT_Msg::OUT_COUNT] ;
			
	//----Flags que varian solo por salida, y no por tipo de mensaje.

MIT_PUBLIC:

	enum OUTPUT_FLAGS { 
		INDENT=ClMIT_Flag::BIT0,		//Activa/desactiva las indentaciones
		FLUSH_SBS=ClMIT_Flag::BIT1,	//Flush paso a paso. Cada vez que se saca un mensaje, se
									//	hace un flush de fichero. Util para localizar GPs
									//	Por ahora, solo se usa en BE_OUT.
		//Estos tres flags solo sirven en BE_OUT. Indican que genera la escritura de un
		//	ClMIT_IdDesc (por defecto, ID_OUT).
		ID_OUT=ClMIT_Flag::BIT2,		
		DESC_OUT=ClMIT_Flag::BIT3,
		ID_DESC_OUT=ClMIT_Flag::BIT2|ClMIT_Flag::BIT3
	} ;

	//Objeto directamente modificable

	ClMIT_Flag m_oOutputFlags[ClMIT_Msg::OUT_COUNT] ;	//Usa TRACE_FLAGS

	//----Parametrizaciones por grupos 

MIT_PUBLIC:
	enum OTHERS { 
		GROUP_NAME_SIZE=9,
		GROUP_COUNT=100		//Maximo numero de grupos (van en un array).
	} ;

MIT_PRIVATE:

	m4int16_t m_iGroupCount ;	//Numero real de grupos usados

	struct StGroup {
		m4char_t GroupName[GROUP_NAME_SIZE] ;
		ClMIT_Msg::MSG_LEVEL Detail ;
		m4uint32_t uiErrorCount ;
		m4uint32_t uiWarningCount ;
	} m_stGroup[GROUP_COUNT] ;

	//---Flags de este objeto

MIT_PRIVATE:
	enum PRIVATE_FLAGS {
		INIT_TRACE=ClMIT_Flag::BIT0,	//True si objeto ya inicializado
		LOCAL_TRACE=ClMIT_Flag::BIT1,	//Si hay problemas, se pone a FALSE
		M4LOG_TRACE=ClMIT_Flag::BIT2, //Si hay problemas, se pone a FALSE
		CANCEL_NOTIFIED=ClMIT_Flag::BIT3, //Si true, cancel notificado.
		SILENT_CANCEL=ClMIT_Flag::BIT4,   //Si true y se notifico cancel, no se saca mas traza.
										//	por defecto activado.
		//INTERNO, Bloquea en contruccion/destruccion la EC prop.
		LOCK_EC_PROPAGATE=ClMIT_Flag::BIT5,
		TRACE_CODE_RIPPED=ClMIT_Flag::BIT6 //Flag que indica si el codigo de Traza ha de ser rippeado (No Traza en Release)
	} ;
	ClMIT_Flag m_oObjFlags ;

	//---Otros

MIT_PRIVATE:
	ClMIT_File m_oFile ;	//Solo es usado en trace local

	ClMIT_TraceCheck m_oOkCount ;

	//Sirven como identificativos en otros ficheros de traza al notificarles errores.

	m4uint32_t m_uiErrorId, m_uiWarningId ;	
	
	//Si tiene un valor>0, los mensajes se ajustan a m_iWrap caracteres por fila, generando
	//	varias filas INDENTADAS si es preciso. Siempre que sea posible, no recortara una 
	//	palabra, sino que la enviara a la siguiente linea por completo.
	//Solo se activa esta capacidad si, ademas, ADD_LF esta activo, y solo para trace a 
	//	fichero local.

	m4int16_t m_iWrap ;

	// Cualquier mensaje se saca si su nivel de detalle es <= que el del grupo al que 
	//	pertenece, o se saca siempre si el tipo de mensaje tiene el PUT_ALWAYS activado 
	//	(normalmente errores y warnings)

	m4int16_t m_iIndent, m_iBlanksPerIndent ;

	//----Para gestion de errores con efectos propagables

MIT_PUBLIC:
	enum ERROR_EFFECT { 
		IMMEDIATE_ERROR,	//Se provoca un error de efecto inmediato.
		PROPAGATE_ERROR		//Se propaga el error.
	} ; 

MIT_PUBLIC:

	// ----------------------------- Inicialización---------------------------------------	

	ClMIT_Trace();
	virtual ~ClMIT_Trace() { ClMIT_Trace::End(); }
	virtual size_t GetSizeof() { return sizeof(ClMIT_Trace) ; }

	// Dispara un CheckPoint de esta clase
	// Si se pasa ao_poCheck, se almacenab los contadores de errores, etc, hasta el momento.
	// Si se pasa ao_pcTraceFile, almacena el nombre de fichero de salida (depende del modo
	//	MoveTrace y del numero de errores producidos). Si no puede ejecutar la operacion de
	//	mover, contendra el fichero trace original.
	virtual void End(ClMIT_TraceCheck *ao_poCheck=NULL, m4char_t *ao_pcTraceFile=NULL, size_t ao_TraceSize=0);

	//  El fichero de trace local es ai_pcLocalTraceFile. Debe de ser unico.  El fichero para 
	//		que la M4LOG lea los datos es ai_pcM4INIFile. Cualquier de estos dos punteros
	//		puede ser NULL, indicando que no se quiere sacar traza al sistema correspondiente.
	//	Si no se puede inicializar el fichero de traza, se intenta lanza un error FE.
	//	GroupCount sirve simplemente para testear que no hay mas grupos del maximo permitido,
	//		y para lamcenar el numero real de los que se van a usar.
	//	Aunque ningun sistema de traza pueda activarse, la ejecucion puede continuar, con
	//      lo cual, el testeo del valor de vuelta de Init es opcional.
	//  Desde todas las componentes se llama a este inicializador. Dependiendo del valor de la
	//      entrada del registry SystemDebugEnable, se pone ai_pcM4INIFile a NULL para que el
	//      sistema de archivos de traza no se inicialice.
	
	m4bool_t Init( 	
		const m4char_t *ai_pcLayerId, const m4char_t *ai_pcComponentId,
		const m4char_t *ai_pcLocalTraceFile, const m4char_t *ai_pcM4INIFile, 
		const m4char_t *ai_GroupNames[GROUP_NAME_SIZE], m4int16_t ai_iGroupCount) ;

MIT_PUBLIC:

	// ----------------------------- Get & Set -------------------------------------------	

	m4uint32_t GetErrorCount(ClMIT_Msg::MsgGroup_t ai_Group) { return m_stGroup[ai_Group].uiErrorCount ; }
	m4uint32_t GetWarningCount(ClMIT_Msg::MsgGroup_t ai_Group) { return m_stGroup[ai_Group].uiWarningCount ; }
	m4uint32_t GetDummyCount() { return m_oOkCount.GetDummyCount() ; }

	m4uint32_t GetErrorCount() { return m_oOkCount.GetErrorCount() ; }
	m4uint32_t GetWarningCount() { return m_oOkCount.GetWarningCount(); }

	m4uint32_t GetEC_ErrorCount() { return m_oOkCount.GetEC_ErrorCount() ; }
	m4uint32_t GetEC_WarningCount() { return m_oOkCount.GetEC_WarningCount() ; }
	m4uint32_t GetIC_ErrorCount() { return m_oOkCount.GetIC_ErrorCount() ; }
	m4uint32_t GetIC_WarningCount() { return m_oOkCount.GetIC_WarningCount() ; }

	m4int16_t  GetGroupCount() { return m_iGroupCount ; }

	const m4char_t * GetGroupName(ClMIT_Msg::MsgGroup_t ai_Group) { return m_stGroup[ai_Group].GroupName ; } 

	// Util para saber si dentro de un intervalo se han producido errores locales. Si la 
	// cuanta varia del punto A al B, ha habido errores entre medias.

	m4uint32_t GetOkCount() { return m_oOkCount.GetOkCount() ; } 

	// Solo da OK si no ha habido errores, ni warnings, ni objetos warning.
	m4bool_t IsAllOK() ;

	m4bool_t IsCancelNotified() { return m_oObjFlags.True(CANCEL_NOTIFIED) ; }
	m4bool_t IsSilentCancel() { return m_oObjFlags.True(SILENT_CANCEL) ; }
	void SetSilentCancel(m4bool_t ai_bSw) { m_oObjFlags.Switch(SILENT_CANCEL,ai_bSw) ; }

	m4bool_t IsTraceCodeRipped() { return m_oObjFlags.True(TRACE_CODE_RIPPED) ; }

	m4bool_t IsCancelOutput() { return m_oObjFlags.TrueAND(CANCEL_NOTIFIED|SILENT_CANCEL) ; }

	// TRUE si objeto ya inicializado

	m4bool_t IsInicialized() { return m_oObjFlags.TrueAND(INIT_TRACE) ; }

	// Puede invocarse varias veces, pero solo tiene efecto la primera.
	// Mete un mensaje de error para indicar que se notifico el cancel y pone el flag de 
	//	CANCEL_NOTIFIED a true (con lo cual, segun el modo SILENT_CANCEL, puede no haber 
	//	mas salida a traza).

	void NotifyCancel() ;

	// Exactamente como la anterior, pero sabiendo los valores por separado. En este caso, 
	//lo normal es hacer un:
	//	ClTraceCheck Count ;
	//	Trace.StartCheck(&Count) ;
	//	...realizar cosas que pueden disparar errores
	//	Trace.StopCheck(&Count) ;	//Count esta actualizado segun el intervalo 

	void StartCheck(ClMIT_TraceCheck *ao_pstCount) { *ao_pstCount = m_oOkCount ;  } 
	void StopCheck(ClMIT_TraceCheck *ao_pstCount) { *ao_pstCount = *ao_pstCount - m_oOkCount ;  } 
	
	// Devuelve el nombre del fichero trace. Si no hay traza local, la cadena estara vacia.

	const m4char_t * GetFileName() ;

	// Puede llamarse tantas veces como se quiera. Tendra efecto la ultima realizada antes de
	//hacer el End del trace.	
	
	void SetMove( MOVE_TRACE_MODES ai_MoveMode, m4char_t *ai_pcStr )  ;
	MOVE_TRACE_MODES GetMoveMode() { return m_MoveMode; }

	//Puntero de uso inmmediato.
	m4char_t * GetMoveFile() { return m_oMoveFile ; }	

	//---------------Indentaciones

	void SetBlanksPerIndent( m4int16_t ai_iBlanksPerIndent ) 
		{ m_iBlanksPerIndent = ai_iBlanksPerIndent ; }
	m4int16_t GetBlanksPerIndent() { return m_iBlanksPerIndent ; }

	// Siempre incrementan y decrementan el contador de indentacion. Si la capacidad de
	//	indentar esta activa, este hecho se reflejara en la salida.
	// Un Unindent nunca va a poner el contador negativo, pues se controla que el minimo 
	//	sea 0.

	void Indent( m4int16_t ai_iCount=1 ) ;
	void Unindent( m4int16_t ai_iCount=1 ) { Indent(-ai_iCount) ; }

	//---------------Wrap

	m4int16_t GetWrap() { return m_iWrap ; }
	void SetWrap(m4int16_t ai_Wrap) { m_iWrap=ai_Wrap ; }

	//---------------Comportamiento por tipo de mensaje

	//	Fijarse que m_oTypeInfo es publico: puede activarse/desactivarse/consultarse
	//		flags a voluntad.

	//-------------Nivel de detalle de grupos

	ClMIT_Msg::MSG_LEVEL GetGroupDetail( ClMIT_Msg::MsgGroup_t ai_Group ) 
		{ return m_stGroup[ai_Group].Detail ; }

	void SetGroupDetail( ClMIT_Msg::MsgGroup_t ai_Group, ClMIT_Msg::MSG_LEVEL ai_Detail ) 
		{ SetGroupDetail(ai_Group, ai_Group, ai_Detail) ; }
	void SetGroupDetail( ClMIT_Msg::MsgGroup_t ai_From, ClMIT_Msg::MsgGroup_t ai_To, 
		ClMIT_Msg::MSG_LEVEL ai_Detail ) ;

	//-------------------------------Operators << --------------------------------------------

	ClMIT_Trace &operator << (const ClMIT_Msg & ai_Msg) ;
	ClMIT_Trace &operator << (TRACE_MSG_COMMAND ai_Command) ;

	//Operators << para argumentos. El orden en que se mandan imponen el ordinal a aplicar.

	ClMIT_Trace &operator << (m4int16_t ai_Par) ;
	ClMIT_Trace &operator << (m4uint16_t ai_Par) ;
	ClMIT_Trace &operator << (m4int32_t ai_Par) ;
	ClMIT_Trace &operator << (m4uint32_t ai_Par) ;
	ClMIT_Trace &operator << (m4double_t ai_Par) ;
	ClMIT_Trace &operator << (m4bool_t ai_Par) ;
	ClMIT_Trace &operator << (m4char_t ai_Par) ;
	ClMIT_Trace &operator << (const m4char_t *ai_Par) ;
	ClMIT_Trace &operator << (const ClMIT_PushLoc &ai_Par) ;
	ClMIT_Trace &operator << (const ClMIT_IdDesc &ai_Par) ;
	ClMIT_Trace &operator << (const ClMIT_StrNLink &ai_Par) ;

	// ----------------------------- Otras acciones --------------------------------------	
	
	//Otra forma mejor de preservar la pila: a la entrada de la funcion se guarda el estado,
	//	y se restaura a la salida.

	m4int16_t GetPushState() { return m_iLocationCount ;} 
	void RestorePushState(m4int16_t ai_State) ; 

	// Vuelca los datos a fichero a  al dispositivo. Sirve para ver la salida sin necesidad
	//	de cerrar el Trace.
	// Sin efecto si no hay traza local

	void Flush() ;

	// Pensado para utilizarse tras un error o warning. Incrementa el contador de dummys
	//	y lanza un warning.

	void NotifyDummy() ;


	// Sirve para notificar los erroresy warnings que se producen en componentes asociados 
	//	de la misma tarea. El PManager se encarga de llamar a este metodo cuando deba.
	// ai_Type debe de ser un ClMIT_Msg::MSG_ERROR o ClMIT_Msg::MSG_WARNING.
	// Saca un mensaje SOFT_NOTIFY solo BE, de tipo warning o error, mostrando en que 
	//	componente se produjo el error, el error en si, y una cadena identificando la posicion
	//	del error en el fichero en que se genero.
	// USA el TM, no debe ser llamado en tiempo de construccion/destruccion.

	void NotifyEC_Problem(ClMIT_Msg::MSG_TYPE ai_Type, 
		m4uint32_t ai_IdComp, m4char_t *ai_pcIdComp, m4uint32_t ai_uiLocalId, m4char_t *ai_pcMsg) ;


	// Si se llama antes de que el objeto se haya inicializado, no hace nada. La cadena no
	//	puede ser NULL.
	// Si !IsAllOK, saca una warning avisando de esta situacion. 
	//	Retorna el valor IsAllOk()

	m4bool_t CheckPoint( const m4char_t * ai_pcStr ) ;
	
	// Si SwOK, no hace nada.
	// Si !SwOK, y el efecto es inmediato, aborta llamando a ImmediateError() ;

	void TestEffect( m4bool_t ai_SwOK,  ERROR_EFFECT ai_ErrorEffect ) ;

	// Lanza un mensaje de error, y provoca un ShutDown(TRUE). 
	void ImmediateError() ;

MIT_PRIVATE:

	// Pide un nuevo indice para meter un parametro. Si no hay disponibles, da un NULL
	//	y lanza un mensaje de error. Si hay disponibles, da el puntero al parametro 
	//	a modificar, e incrementa m_iParamCount.

	StParam * RequestNewParam() ;

	//Ambos metodos usan los parametros actualmente almacenados en el objeto.
	//LogPrintF es un printf cuya cadena de formateo esta en formato m4log. Si hay algun % con
	//	formato incorrecto, no hace nada en ese parametro, y lo copia tal cual.
	//LogParams simplemente genera la cadena de parametros interpretable por la M4Log. (Solo 
	//	mete los parametros, concatenando #ParN, no mete los *S0* ni nada parecido, ni ningun 
	//	#final).
	//Ninguna de las funciones testea un overstring, debe hacerlo el metodo llamador.
	//Ambas funciones devuelven siempre cadenas correctas, aunque no haya parametros.

	void LogPrintF(const m4char_t *ai_pcFormatStr, m4char_t *ai_pcTarget, 
		m4int16_t ai_iFirstParam, m4int16_t ai_iParamCount ) ;

//---- NUEVA FUNCIONALIDAD PARA M4LOG en 3.1
	//A partir de la 3.1 no se debe llamar directamente a SETCODEF, puesto que se quiere que no
	// se deba saber nada acerca del formato interno de la m4log. Por ello, en lugar de pasarle una 
	// cadena ya formateada con el SETCODEF, le pasaremos los argumentos del mensaje de error uno por 
	// uno. Se he implementado de nuevo las funciones FEOUT y LogParams, y se ha dejado las antiguas con
	// los nombres de OldLogParams y OldFEOut, por si se vuelve atras.
	//Para usar esta nueva funcionalidad, hay qye incluir el fichero "basiclog.hpp" y linkar contra la
	// m4log.dll. El paso del error y los parametros se realiza mediante macros:
	//
	//  BL_BEGIN(Tipo_Error, Codigo_Error);
	//  BL_OBJ << Arg1 << Arg2;
	//  BL_OBJ << A2rg3;
	//  BL_END; 
	void LogParams(m4char_t *ai_pcTarget, StParam* ai_Param) ;
	void OldLogParams(m4char_t *ai_pcTarget, m4int16_t ai_iFirstParam, m4int16_t ai_iParamCount) ;
	//Sacan el msg al dipositivo. Usan Trace_g_pcBuffAux. Ambas solo deben ser llamadas si
	//	esta activado el trace correspondiente.
	void FEOut() ;	
	void OldFEOut() ;	

//-------------------------------------------

	void BEOut() ;	//Diga lo que digan los flags, NO se invoca si el fichero de traza ya se cerro

	//Auxiliar de BEOut, usa Trace_g_pcBuffAux2. Solo debe llamarse si hay traza local
	void WrapOutput(m4char_t *ai_pcStr) ;	

} ;

#endif


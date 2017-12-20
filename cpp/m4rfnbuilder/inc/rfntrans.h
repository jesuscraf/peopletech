//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             RFNtrans.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:20/10/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//
//
//==============================================================================

#ifndef __RFNTRANS__H__
#define __RFNTRANS__H__

#include "clstr.h"
#include "m4clflag.h"
#include "fontm.h"
#include "rfnstrt.h"
#include "m4stl.hpp"

//==========================================================================ClRFN_ParsePAR

class ClRFN_ParsePAR
{
	friend class ClRFN_FlexLexer ;
	friend class ClRFN_Parser ;

	//Esta clase esta pensada como clase stand-alone por thread. Osea, teniendo un objeto
	//	de este tipo por thread, basta para traducir cadenas a arboles de parseo.

MIT_PUBLIC:

	StMIT_Inspector  *m_poInsp ;	//Simple link

	//================>m_oVbleDesc
	//
	// m_oVbleDesc es la lista de variables exitentes. Sirve para (de mas a menos generico):
	//	-Validar que las variables como tal existen.
	//	-Validar que son validas en un device en concreto. Saber de que tipo son en ese caso.
	//	-Validar que son validas en el bloque header ó height.
	//  -OPT_DI_VBLE significa opcional, Device Inherit. El valor se coje del parametro 
	//		m_oInheritFrom	en el Device block homologo del  HEADER. 
	//		Solo permitido en bloques Dvc de HEIGHT
	//	-OPT_FAI_VBLE significa FOR ALL inherit. El valor se coje del valor m_oInheritFrom en el
	//		bloque FOR ALL del propio bloque en proceso.
	//		Solo permitido en bloques Dvc de HEADER/HEIGHT

MIT_PUBLIC:

	enum VBLE_TYPE { STR_VBLE, NUM_VBLE, BOOL_VBLE, VBLE_TYPE_COUNT  } ;
	enum VBLE_SUITABLE { NOT_SUIT_VBLE, REQUIRED_VBLE, OPT_DI_VBLE, OPT_FAI_VBLE } ;

MIT_PUBLIC:

	struct StDescVbleData {
		struct StSuit {
			VBLE_SUITABLE m_SuitInHeader ;
			VBLE_SUITABLE m_SuitInHeight ;
			VBLE_TYPE     m_Type ;
			ClMIT_Str     m_oInheritFrom ;
		} m_DvcSuit[ClFONT_M_RFNStruct::DVC_COUNT] ;

		VBLE_SUITABLE m_SuitInHeader ;	//Estas referencian a bloques FOR_ALL
		VBLE_SUITABLE m_SuitInHeight ;
		VBLE_TYPE     m_Type ;
		ClMIT_Str m_oInheritFrom ;

		StDescVbleData() {}
		StDescVbleData(const StDescVbleData& ai_oSource) { Init(ai_oSource); }
		StDescVbleData & operator = (const StDescVbleData &ai_oOpe) { Init(ai_oOpe); return *this; } 
		void Init(const StDescVbleData& ai_oSource) ;
	} ;
	typedef map <ClMIT_Str, StDescVbleData, less<ClMIT_Str> > VbleDesc_t ;	//Key, name de Vble
	VbleDesc_t m_oVbleDesc ;

	
	//<================

	//================>m_oVbleStack
	//Según se parsea, esta lista contiene las variables que van apareciendo.
	//Donde se almacenan la final estas variables, es algo que se sabrá a posteiori, pero no
	//en el momento de reconocer la lista de valores.
	//Esta lista se reutiliza continuamente. Basicamente todo el parseo hace los siguiente 
	//(por orden):
	//	-Monta una lista de valores en m_oVbleStack.
	//	-Se reconoce un NT al que pertenece la lista:
	//		-Se verifica que todas vbles metidas estan permitidas en el actual contexto.
	//		-Se verififica que el tipo de vble es correcto.
	//		-Se copian los valores de la lista donde proceda.
	//	-Se LIMPIA la lista.
	//	-Vuelta al primer paso.
	
	struct StVbleValue {

		VBLE_TYPE m_Type ;
		ClMIT_Str m_oValue ;

		StVbleValue() {}
		StVbleValue(const StVbleValue& ai_oSource) { Init(ai_oSource); }
		StVbleValue & operator = (const StVbleValue &ai_oOpe) { Init(ai_oOpe); return *this; }
		void Init(const StVbleValue& ai_oSource) ;
	} ;
	typedef map <ClMIT_Str, StVbleValue, less<ClMIT_Str> > VbleList_t ;	//Key, name de Vble
	VbleList_t m_oVbleStack ;

	ClMIT_Str m_oVbleName, m_oLiteralData ;	//Auxiliar de parseo
	
	//<================

	//================>Datos almacenados en la resolucion de NT (copiados de m_oVbleStack)

MIT_PUBLIC:


	struct StSelectionBlock {

		m4bool_t   m_SwItemLoaded[ClFONT_M_RFNStruct::SELECTION_COUNT] ; 
		VbleList_t m_oDvcVble[ClFONT_M_RFNStruct::SELECTION_COUNT] ;

		StSelectionBlock() {}
		StSelectionBlock(const StSelectionBlock& ai_oSource) { Init(ai_oSource); }
		StSelectionBlock & operator = (const StSelectionBlock &ai_oOpe) { Init(ai_oOpe); return *this; }
		void Init(const StSelectionBlock& ai_oSource) ;

		void Reset() ;
	} ;
	
	//Se copia desde m_oVbleStack cuando reconoce un bloque <ModeSelector>
	//El NT que reconoce bloques PCL, etc, se encarga de copiar los datos y limpiarlos.
	//Los datos iran a un bloque Dvc de header o de height.
	
	StSelectionBlock m_oSelection ;	//Auxiliar de proceso

	struct StBlockData {

		VbleList_t m_oVble ; m4bool_t m_SwForAllBlockLoaded ;
		
		struct StDvcBlockData {
			m4bool_t m_SwDvcBlockLoaded;
			m4bool_t m_SwActivated ;	//Flag para bloque Dvc calificado
			StSelectionBlock m_oDvcSelection ;
		} m_oDvcBlock[ClFONT_M_RFNStruct::DVC_COUNT] ;

		StBlockData() {}
		StBlockData(const StBlockData& ai_oSource) { Init(ai_oSource); }
		StBlockData & operator = (const StBlockData &ai_oOpe) { Init(ai_oOpe); return *this; }
		void Init(const StBlockData& ai_oSource) ;

		void Reset() ;
	} ;

	StBlockData m_oBlockData ;	//Axiliar de parseo

	//<================

	//================>Datos finales, todo listo para paso a RFN

	m4bool_t m_bSwHeaderLoaded ;
	StBlockData m_oHeaderData ;	//Datos bloque HEADER
	typedef map <m4uint32_t, StBlockData, less<m4uint32_t> > BlockDataList_t ;	//Key, DesignHeight
	BlockDataList_t m_oHeightData ;	


	//<================

MIT_PUBLIC:

	ClRFN_ParsePAR() ;  

	virtual ~ClRFN_ParsePAR() { ClRFN_ParsePAR::End() ;}
	virtual size_t GetSizeof() { return sizeof(ClRFN_ParsePAR) ; }

	virtual void End() ;

	void Init() ;

	// Parsea el fichero PAR. 
	// Da TRUE o FALSE en funcion de si tiene exito o no.
	// Se muestra un error local en caso de que falle la analizacion.

	m4bool_t ParsePAR( m4char_t *ai_pcFile, 
		m4char_t * ai_pcPCL_Driver, m4char_t * ai_pcESCP_Driver, 
		m4char_t *ai_pcRVW_Driver, 	m4char_t * ai_pcPSC_Driver
	 ) ;


MIT_PUBLIC:

	//============================================================>Auxiliares testeo Vble

	//Chequea solo el nombre de la variable.
	//Si se pasa ao_poData, se devuelve la direccion de la descripcion de la variable.

	m4bool_t CheckVbleName(m4char_t *ai_pcVbleName, StDescVbleData **ao_poData=NULL) ;

	//  Chequea que en el bloque HEADER/HEIGHT ó Dvc/ForAll es correcta toda la lista de 
	//variables, en cuanto a que es correcto donde aparecen y que el tipo sea correcto.
	//	También chequea que las variables obligatorias en el bloque aparecen en la lista.
	//	Falta de poner los defectos.
	//Si !ai_bSwForAll, hay que especificar el Dvc.

	m4bool_t CheckVbleList(  VbleList_t *ai_poVbleList, 
		m4bool_t ai_bSwInHeader, m4bool_t ai_bSwForAll, 
		ClFONT_M_RFNStruct::DVC ai_Dvc=ClFONT_M_RFNStruct::ESCP) ;

	//  Pone los defectos a ai_poBlockData. Si ai_poHeaderBlock es NULL, ai_poBlockData es un
	//bloque tipo Header. Si no es NULL, es un bloque tipo HEIGHT y los defectos tipo DI se
	//cojen de ai_poHeaderBlock.
	//  Para el caso de items Selections puede pasar que no hay defecto en el HEADER, por lo que
	//si no se especifica valor se genera tambien un error.
	//	Da FALSE si tiene alguno problema.

	m4bool_t SetBlockDefaults( StBlockData *ai_poHeaderBlock, StBlockData *ai_poBlockData ) ;


	//============================================================>Auxiliares de parseo

	void Reset() ;

	//Da false si no pude meter la vble (debido a que esta repetida)
	//Pasa el CheckVbleName.

	m4bool_t PushVbleOnStack( m4char_t *ai_pcVbleName, VBLE_TYPE ai_Type, m4char_t *ai_pcValue ) ;

	enum BOLD_SELECT { BOLD_ON, BOLD_OFF, BOLD_ALL } ; 
	enum ITALIC_SELECT { ITALIC_ON, ITALIC_OFF, ITALIC_ALL } ; 
	enum UNDERLINE_SELECT { UNDERLINE_ON, UNDERLINE_OFF, UNDERLINE_ALL } ; 

	//Auxiliares de parseo
	BOLD_SELECT m_BoldSelect ; ITALIC_SELECT m_ItalicSelect ; UNDERLINE_SELECT m_UnderlineSelect ;

	//Traspasa la informacion de VbleStack al m_oSelection correcto (puede generar una o varias
	//	ocurrencias). Si ya habia alguna cargada previamente, dara error.
	//Da false si no tiene exito.

	m4bool_t ReduceSelection( BOLD_SELECT ai_Bold, ITALIC_SELECT ai_Italic, UNDERLINE_SELECT ai_Underline ) ;

	//Traspasa la informacion de VbleStack al m_oBlockData.m_oVble. Si ya se habia especificado
	//	el bloque, da error.
	//Da false si no tiene exito.

	m4bool_t ReduceForAllBlock() ;	

	ClFONT_M_RFNStruct::DVC m_oBlockType ;	//Auxiliar de proceso

	
	//Traspasa la informacion de m_oSelection al 
	//	m_oBlockData.m_oDvcBlock[m_BlockType].m_oDvcSelection 
	//Si ya se habia especificado el bloque, da error.
	//Da false si no tiene exito.

	m4bool_t ReduceForDvcBlock() ;	
	
	//Para bloques calificados, limpia el flag m_oBlockData.m_oDvcBlock[m_BlockType].m_SwActivated 
	void ClearDvcActivatedFlag() ;

	//Traspasa la informacion de m_oBlockData a m_oHeaderData.
	//Si ya se habia especificado el bloque, da error.
	//Chequea que se han especificado todos los bloques Dvc y el FOR ALL.
	//Pasa el test CheckVbleList a todas las variables pertinentes (no para bloques Dvc 
	//	desactivados ni para items Selection no cargados).
	//Pasa el SetBlockDefaults().
	//Da false si no tiene exito.

	m4bool_t ReduceHeaderBlock() ;	
	
	//Traspasa la informacion de m_oBlockData a un nuevo elemento de m_oHeightData.
	//Si ya se habia especificado un bloque Height con la misma altura de diseño, da error.
	//Testea que exista el bloque FOR ALL y que si un bloque Dvc esta activado en el HEADER, lo
	//	este cargado en este. Para los que aprezcan en el bloque HEIGHT, pero no en el HEADER, 
	//	este metodo desactiva el bloque del Height.
	//Pasa el test CheckVbleList a todas las variables pertinentes (no para bloques Dvc 
	//	desactivados ni para items Selection no cargados).
	//Pasa el SetBlockDefaults().
	//Da false si no tiene exito.

	m4bool_t ReduceHeightBlock() ;	


	//============================================================>Auxiliares obtencion valores

	//Obtienen el valor de una variable.
	//Si no encuentran la variable o el tipo es incorrecto, generan errores locales y devuelven 
	//	un valor dummy.

	m4bool_t		GetBoolVble(	VbleList_t *ai_poVbleList, m4char_t *ai_pcVbleName ) ;
	m4char_t *		GetStrVble(		VbleList_t *ai_poVbleList, m4char_t *ai_pcVbleName ) ;
	m4int32_t 		GetLongVble(	VbleList_t *ai_poVbleList, m4char_t *ai_pcVbleName ) ;
	m4double_t		GetDoubleVble(	VbleList_t *ai_poVbleList, m4char_t *ai_pcVbleName ) ;


} ;


#endif


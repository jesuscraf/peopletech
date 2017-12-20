//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:           vvm.hpp
// Project:        Calculo
// Author:         Meta Software M.S. , S.A
// Date:            21 may 97
// Language:        C++ 
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//
//   Clase base de todas las que implementan funciones de vm
////
//==============================================================================


#ifndef _ClVVM_H
#define _ClVVM_H

#include "vmres.hpp"

#include "vmobjdef.hpp"
#include "vmtypes.hpp"
#include "vmdefine.hpp"
#include "fixstackvm.hpp" //es un template
#include "clstringpool.hpp"
#include "medvarin.hpp"
#include "vmdebug.hpp"

class ClVMCSContext;

class ClVVM{
public:
	ClVVM (){
		m_oRunTimeStack=0;
		m_oDynamicPool=0;
		m_oStateStack=0;
		m_oVariantMediator=0;
		m_poContext=0;

	}
	virtual ~ClVVM(){}
	
	void SetRuntimeStack (tpFixedStack<m4VariantType> *ai_oRunTimeStack){
		m_oRunTimeStack=ai_oRunTimeStack;
	}
	
	void SetStateStack	(tpFixedStack<m4VMState_t>	*ai_oStateStack){
		m_oStateStack=ai_oStateStack;
	}
	
	void SetPool			(ClStringPool			*ai_oDynamicPool){
		m_oDynamicPool=ai_oDynamicPool;
	}
	
	void SetMediator		(ClVariantConversor			*ai_oVariantMediator){
		m_oVariantMediator=ai_oVariantMediator;
	}

	virtual void SetVMCSContext	(ClVMCSContext				*ai_poContext){
		m_poContext=ai_poContext;
	}

	tpFixedStack<m4VariantType> * GetRuntimeStack	(void){
		return m_oRunTimeStack;
	}
	
	tpFixedStack<m4VMState_t>	*GetStateStack		(void){
		return m_oStateStack;
	}

	ClStringPool				*GetPool			(void){
		return m_oDynamicPool;
	}

	ClVariantConversor			*GetMediator		(void){
		return m_oVariantMediator;
	}

	ClVMCSContext				*GetVMCSContext		(void){
		return m_poContext;
	}

	string DumpCallStack(const m4VMState_t *poState=0, m4bool_t ai_bIdsOnly=M4_FALSE);	//Volcado de la Pila de Llamadas


protected:
	tpFixedStack<m4VariantType> *m_oRunTimeStack;
	ClStringPool				*m_oDynamicPool;
	tpFixedStack<m4VMState_t>	*m_oStateStack;
	ClVariantConversor			*m_oVariantMediator;
	ClVMCSContext				*m_poContext;
    
	
	//funciones comunes

	//--------------------------------------------------------------------------
	//funciones inline para el manejo de la pila de runtime y el pool de cadenas
	//--------------------------------------------------------------------------
	m4return_t RuntimePop(m4VariantType &ai_vTemp);
	m4return_t RuntimeTop(m4VariantType &ai_vTemp , m4uint32_t ai_ipos=0);
	m4return_t RuntimePush(const m4VariantType &ai_vTemp);
	m4return_t RuntimePushDouble(m4double_t ai_iTemp);
	m4return_t RuntimePushDate(m4date_t ai_dTemp);
	m4return_t RuntimePushString(const m4char_t *ai_pszTemp, m4bool_t ai_bIsUser=M4_FALSE);
	m4return_t RuntimePopNumber(m4VariantType &ai_vTemp);
	m4return_t RuntimePopDouble(m4VariantType &ai_vTemp);
	m4return_t RuntimePopDate(m4VariantType &ai_vTemp);
	m4return_t RuntimePopString(m4VariantType &ai_vTemp);
	m4return_t RuntimePopAndDelete(void);
    //nulos
    m4bool_t IsNull (const m4VariantType &ai_var);
    m4return_t RuntimePushNull (void);
	//--------------------------------------------------------------------------
	//funciones inline para Check de Variants
	//--------------------------------------------------------------------------

	// Si un Variant es String, se borra del Pool
	void DeleteVarString ( m4VariantType &ai_var );

	//Comprueba si el Variant es un Number, y en caso de error
	//hace un PushDouble del valor pasado o Null (si el variant es Null)
	m4bool_t CheckNumberIfErrorPush (m4VariantType &ai_var, const m4double_t ai_dResError, const m4bool_t ai_bNoNull = M4_FALSE);
	//Comprueba si el Variant es un Number, y en caso de error
	//hace un PushString del valor pasado o Null (si el variant es Null)
	m4bool_t CheckNumberIfErrorPush (m4VariantType &ai_var, m4pcchar_t ai_pcStrError, const m4bool_t ai_bNoNull = M4_FALSE);
	//Comprueba si el Variant es un Number, y en caso de error
	//hace un Push del Variant pasado o Null (si el variant es Null)
	m4bool_t CheckNumberIfErrorPush (m4VariantType &ai_var, const m4VariantType &vRes, const m4bool_t ai_bNoNull = M4_FALSE);
	//Comprueba si el Variant es un Date, y en caso de error
	//hace un PushDouble del valor pasado o Null (si el variant es Null)
	m4bool_t CheckDateIfErrorPush (m4VariantType &ai_var, const m4double_t ai_dResError, const m4bool_t ai_bNoNull = M4_FALSE);
	//Comprueba si el Variant es un Date, y en caso de error
	//hace un PushString del valor pasado o Null (si el variant es Null)
	m4bool_t CheckDateIfErrorPush (m4VariantType &ai_var, const m4pcchar_t ai_pcStrError, const m4bool_t ai_bNoNull = M4_FALSE);
	//Comprueba si el Variant es un Date, y en caso de error
	//hace un Push del Variant pasado o Null (si el variant es Null)
	m4bool_t CheckDateIfErrorPush (m4VariantType &ai_var, const m4VariantType &vRes, const m4bool_t ai_bNoNull = M4_FALSE);

	//Comprueba si el Variant es un String, y en caso de error
	//hace un PushDouble del valor pasado o Null (si el variant es Null)
	m4bool_t CheckStringIfErrorPush (m4VariantType &ai_var, const m4double_t ai_dResError, const m4bool_t ai_bNoNull = M4_FALSE);
	//Comprueba si el Variant es un String, y en caso de error
	//hace un PushString del valor pasado o ( Null | string, si el variant es Null, segun se desee)
	m4bool_t CheckStringIfErrorPush (m4VariantType &ai_var, m4pcchar_t ai_pcStrError, const m4bool_t ai_bNoNull = M4_FALSE);
	//Comprueba si el Variant es un String, y en caso de error
	//hace un Push del Variant pasado o Null (si el variant es Null)
	m4bool_t CheckStringIfErrorPush (m4VariantType &ai_var, const m4VariantType &vRes,  const m4bool_t ai_bNoNull = M4_FALSE);

	// para liberar el pool de cadenas, de un array de variants.
	m4return_t FreePoolStrings ( m4VariantType * ai_pavVariants , m4int32_t ai_iNumVariants );

	ClAccess*	_FindChannel( const m4VariantType& ai_vM4obj, m4pcchar_t ai_pccMethodName, const m4VMState_t &roState ) ;
	ClNode*		_FindNode( ClAccess *ai_poAccess, const m4VariantType& ai_vNodo, m4pcchar_t ai_pccMethodName, const m4VMState_t &roState, m4bool_t ai_bDumpError ) ;
};


typedef	BasicLogManip1<ChLog, const m4VMState_t*>	ChLogManip_line_dump_t ;
ChLogManip_line_dump_t	DumpLine( const m4VMState_t* ai_poState ) ;


#include "vvm.inl"

#endif


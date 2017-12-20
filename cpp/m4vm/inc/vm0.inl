//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:                ExeFurep.cpp
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
//    Implementación de la clase vmachine0->funciones inline
////
//==============================================================================


//--------------------------------------------------------------------------
//funciones inline para el manejo de la pila de runtime y el pool de cadenas
//--------------------------------------------------------------------------
inline m4return_t ClVVM::RuntimePop(m4VariantType &ai_vTemp){
	m4return_t ret=M4_SUCCESS;
	ret=m_oRunTimeStack->Top(ai_vTemp);
	
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "Error performing pop from RTS.")	;
	
	m_oRunTimeStack->Pop();	
	return ret;	
	
}

inline m4return_t ClVVM::RuntimeTop(m4VariantType &ai_vTemp , m4uint32_t ai_ipos){
	return m_oRunTimeStack->Top(ai_vTemp, ai_ipos);
}

inline m4return_t ClVVM::RuntimePush(const m4VariantType &ai_vTemp){
	return (m_oRunTimeStack->Push(ai_vTemp));
}


inline m4return_t ClVVM::RuntimePushDouble(m4double_t ai_iTemp){
	m4VariantType vTemp;
	vTemp.Type=M4CL_CPP_TYPE_NUMBER;
	vTemp.Data.DoubleData=ai_iTemp;
	return (m_oRunTimeStack->Push(vTemp));
}


inline m4return_t ClVVM::RuntimePushDate(m4date_t ai_dTemp){
	m4VariantType vTemp;
	vTemp.Type=M4CL_CPP_TYPE_DATE;
	vTemp.Data.DoubleData=ai_dTemp;
	return (m_oRunTimeStack->Push(vTemp));
}


inline m4return_t ClVVM::RuntimePushString(const m4char_t *ai_pszTemp){
	m4VariantType vTemp;
	m4uint32_t iHandle;
	vTemp.Type=M4CL_CPP_TYPE_STRING_POOL;
	m_oDynamicPool->AddString(ai_pszTemp,iHandle);
	vTemp.Data.CharData=iHandle;
	return (m_oRunTimeStack->Push(vTemp));
}


inline m4return_t ClVVM::RuntimePopNumber(m4VariantType &ai_vTemp){
	m4return_t ret=M4_SUCCESS;
	m_oRunTimeStack->Top(ai_vTemp);
	if (ai_vTemp.Type!=M4CL_CPP_TYPE_NUMBER && ai_vTemp.Type!=M4CL_CPP_TYPE_DATE){
		ret=M4_ERROR;
		SETCODE ( M4_CH_VM_RTS_POP, ERRORLOG, "Error performing pop of a number from RTS.");
	}
	m_oRunTimeStack->Pop();	
	return ret;	
}


inline m4return_t ClVVM::RuntimePopDouble(m4VariantType &ai_vTemp){
	
	m4return_t ret=M4_SUCCESS;
	m_oRunTimeStack->Top(ai_vTemp);
	if (ai_vTemp.Type!=M4CL_CPP_TYPE_NUMBER){
		ret=M4_ERROR;
		SETCODE ( M4_CH_VM_RTS_POP, ERRORLOG, "Error performing pop of a number from RTS.");
	}
	m_oRunTimeStack->Pop();	
	return ret;	
}



inline m4return_t ClVVM::RuntimePopDate(m4VariantType &ai_vTemp){
	
	m4return_t ret=M4_SUCCESS;
	m_oRunTimeStack->Top(ai_vTemp);
	if (ai_vTemp.Type!=M4CL_CPP_TYPE_DATE){
		ret=M4_ERROR;
		SETCODE ( M4_CH_VM_RTS_POP, ERRORLOG, "Error performing pop of a date from RTS.");
	}
	m_oRunTimeStack->Pop();	
	return ret;	
}


inline m4return_t ClVVM::RuntimePopString(m4VariantType &ai_vTemp){
	
	m4return_t ret=M4_SUCCESS;
	m_oRunTimeStack->Top(ai_vTemp);
	if (ai_vTemp.Type!=M4CL_CPP_TYPE_STRING_POOL){
		ret=M4_ERROR;
		SETCODE ( M4_CH_VM_RTS_POP, ERRORLOG, "Error performing pop of a string from RTS.");
	}
	m_oRunTimeStack->Pop();	
	return ret;	
}


inline m4return_t ClVVM::RuntimePopAndDelete(void){
	
	m4VariantType vTemp;
	m4return_t ret=M4_SUCCESS;
	
	ret=m_oRunTimeStack->Top(vTemp);
	if (ret!=M4_SUCCESS){
		SETCODE ( M4_CH_VM_RTS_POP, ERRORLOG, "Error performing pop from RTS.");
		return ret;
	}
	m_oRunTimeStack->Pop(); 
	if (vTemp.Type==M4CL_CPP_TYPE_STRING_POOL){
		//eliminarlso del POOL DE CADENAS
		m_oDynamicPool->DeleteString(vTemp.Data.CharData);
	}
	return ret;
}


//nulos
inline m4bool_t ClVVM::IsNull (const m4VariantType &ai_var){
	return ( (ai_var.Type == M4CL_CPP_TYPE_NULL) ? M4_TRUE : M4_FALSE );
}


inline m4return_t ClVVM::RuntimePushNull (void){
	m4VariantType vTemp;
	
	vTemp.Data.DoubleData=0.0;
	vTemp.Type=M4CL_CPP_TYPE_NULL;
	return m_oRunTimeStack->Push(vTemp);
}


//--------------------------------------------------------------------------
//funciones inline para Check de Variants
//--------------------------------------------------------------------------

// Si un Variant es String, se borra del Pool
inline void ClVVM::DeleteVarString ( m4VariantType &ai_var ) {
	if (ai_var.Type == M4CL_CPP_TYPE_STRING_POOL)
		m_oDynamicPool->DeleteString(ai_var.Data.CharData);
}

//Comprueba si el Variant es un Number, y en caso de error
//hace un PushDouble del valor pasado o Null (si el variant es Null)
inline m4bool_t ClVVM::CheckNumberIfErrorPush (m4VariantType &ai_var, const m4double_t ai_dResError, const m4bool_t ai_bNoNull) {
	
	m4return_t ret = M4_SUCCESS;
	
	if (IsNull (ai_var) ){ //Null behaviour: Valor nulo->Nulo
		if (ai_bNoNull)
			RuntimePushDouble (ai_dResError);
		else
			RuntimePushNull ();
		return M4_FALSE;
	}   
	if (ai_var.Type!=M4CL_CPP_TYPE_NUMBER){
		ret=m_oVariantMediator->ToDouble(ai_var);
	}   
	if (ret!=M4_SUCCESS){
		//liberamos las posibles cadenas
		DeleteVarString (ai_var);
		RuntimePushDouble(ai_dResError);
		return M4_FALSE;
	}
	return M4_TRUE;
}

//Comprueba si el Variant es un Number, y en caso de error
//hace un PushString del valor pasado o Null (si el variant es Null)
inline m4bool_t ClVVM::CheckNumberIfErrorPush (m4VariantType &ai_var, m4pcchar_t ai_pcStrError, const m4bool_t ai_bNoNull) {
	
	m4return_t ret = M4_SUCCESS;
	
	if (IsNull (ai_var) ){ //Null behaviour: Valor nulo->Nulo
		if (ai_bNoNull)
			RuntimePushString (ai_pcStrError);
		else
			RuntimePushNull ();
		return M4_FALSE;
	}   
	if (ai_var.Type!=M4CL_CPP_TYPE_NUMBER){
		ret=m_oVariantMediator->ToDouble(ai_var);
	}   
	if (ret!=M4_SUCCESS){
		//liberamos las posibles cadenas
		DeleteVarString (ai_var);
		RuntimePushString(ai_pcStrError);
		return M4_FALSE;
	}
	return M4_TRUE;
}

//Comprueba si el Variant es un Number, y en caso de error
//hace un Push del Variant pasado o Null (si el variant es Null)
inline m4bool_t ClVVM::CheckNumberIfErrorPush (m4VariantType &ai_var, const m4VariantType &vRes, const m4bool_t ai_bNoNull) {
	
	m4return_t ret = M4_SUCCESS;
	
	if (IsNull (ai_var) ){ //Null behaviour: Valor nulo->Nulo
		if (ai_bNoNull)
			RuntimePush (vRes);
		else
			RuntimePushNull ();
		return M4_FALSE;
	}   
	if (ai_var.Type!=M4CL_CPP_TYPE_NUMBER){
		ret=m_oVariantMediator->ToDouble(ai_var);
	}   
	if (ret!=M4_SUCCESS){
		//liberamos las posibles cadenas
		DeleteVarString (ai_var);
		RuntimePush (vRes);
		return M4_FALSE;
	}
	return M4_TRUE;
}


//Comprueba si el Variant es un Date, y en caso de error
//hace un PushDouble del valor pasado o Null (si el variant es Null)
inline m4bool_t ClVVM::CheckDateIfErrorPush (m4VariantType &ai_var, const m4double_t ai_dResError, const m4bool_t ai_bNoNull ) {
	
	m4return_t ret = M4_SUCCESS;
	
	if (IsNull (ai_var) ){ //Null behaviour: Valor nulo->Nulo
		if (ai_bNoNull)
			RuntimePushDouble(ai_dResError);
		else
			RuntimePushNull ();
		return M4_FALSE;
	}   
	if (ai_var.Type!=M4CL_CPP_TYPE_DATE){
		ret=m_oVariantMediator->ToDate(ai_var);
	}   
	if (ret!=M4_SUCCESS){
		//liberamos las posibles cadenas
		DeleteVarString (ai_var);
		RuntimePushDouble(ai_dResError);
		return M4_FALSE;
	}
	return M4_TRUE;
}

//Comprueba si el Variant es un Date, y en caso de error
//hace un PushString del valor pasado o Null (si el variant es Null)
inline m4bool_t ClVVM::CheckDateIfErrorPush (m4VariantType &ai_var, const m4pcchar_t ai_pcStrError, const m4bool_t ai_bNoNull ) {
	
	m4return_t ret = M4_SUCCESS;
	
	if (IsNull (ai_var) ){ //Null behaviour: Valor nulo->Nulo
		if (ai_bNoNull)
			RuntimePushString (ai_pcStrError);
		else
			RuntimePushNull ();
		return M4_FALSE;
	}   
	if (ai_var.Type!=M4CL_CPP_TYPE_DATE){
		ret=m_oVariantMediator->ToDate(ai_var);
	}   
	if (ret!=M4_SUCCESS){
		//liberamos las posibles cadenas
		DeleteVarString (ai_var);
		RuntimePushString(ai_pcStrError);
		return M4_FALSE;
	}
	return M4_TRUE;
}

//Comprueba si el Variant es un Date, y en caso de error
//hace un Push del Variant pasado o Null (si el variant es Null)
inline m4bool_t ClVVM::CheckDateIfErrorPush (m4VariantType &ai_var, const m4VariantType &vRes, const m4bool_t ai_bNoNull) {
	
	m4return_t ret = M4_SUCCESS;
	
	if (IsNull (ai_var) ){ //Null behaviour: Valor nulo->Nulo
		if (ai_bNoNull)
			RuntimePush (vRes);
		else
			RuntimePushNull ();
		return M4_FALSE;
	}   
	if (ai_var.Type!=M4CL_CPP_TYPE_DATE){
		ret=m_oVariantMediator->ToDate(ai_var);
	}   
	if (ret!=M4_SUCCESS){
		//liberamos las posibles cadenas
		DeleteVarString (ai_var);
		RuntimePush (vRes);
		return M4_FALSE;
	}
	return M4_TRUE;
}

//Comprueba si el Variant es un String, y en caso de error
//hace un PushDouble del valor pasado o Null (si el variant es Null)
inline m4bool_t ClVVM::CheckStringIfErrorPush (m4VariantType &ai_var, const m4double_t ai_dResError, const m4bool_t ai_bNoNull ) {
	
	m4return_t ret = M4_SUCCESS;
	
	if (IsNull (ai_var) ){ //Null behaviour: Valor nulo->Nulo
		if (ai_bNoNull)
			RuntimePushDouble(ai_dResError);
		else
			RuntimePushNull ();
		return M4_FALSE;
	}   
	if (ai_var.Type!=M4CL_CPP_TYPE_STRING_POOL){
		ret=m_oVariantMediator->ToPoolString (ai_var);
	}   
	if (ret!=M4_SUCCESS){
		RuntimePushDouble(ai_dResError);
		return M4_FALSE;
	}
	return M4_TRUE;
}

//Comprueba si el Variant es un String, y en caso de error
//hace un PushString del valor pasado o ( Null | string, si el variant es Null, segun se desee)
inline m4bool_t ClVVM::CheckStringIfErrorPush (m4VariantType &ai_var, m4pcchar_t ai_pcStrError, const m4bool_t ai_bNoNull) {
	
	m4return_t ret = M4_SUCCESS;
	
	if (IsNull (ai_var) ){ //Null behaviour: Valor nulo->Nulo
		if (ai_bNoNull)
			RuntimePushString (ai_pcStrError);
		else
			RuntimePushNull ();
		return M4_FALSE;
	}   
	if (ai_var.Type!=M4CL_CPP_TYPE_STRING_POOL){
		ret=m_oVariantMediator->ToPoolString (ai_var);
	}   
	if (ret!=M4_SUCCESS){
		RuntimePushString (ai_pcStrError);
		return M4_FALSE;
	}
	return M4_TRUE;
}

//Comprueba si el Variant es un String, y en caso de error
//hace un Push del Variant pasado o Null (si el variant es Null)
inline m4bool_t ClVVM::CheckStringIfErrorPush (m4VariantType &ai_var, const m4VariantType &vRes,  const m4bool_t ai_bNoNull) {
	
	m4return_t ret = M4_SUCCESS;
	
	if (IsNull (ai_var) ){ //Null behaviour: Valor nulo->Nulo
		if (ai_bNoNull)
			RuntimePush (vRes);
		else
			RuntimePushNull ();
		return M4_FALSE;
	}   
	if (ai_var.Type!=M4CL_CPP_TYPE_STRING_POOL){
		ret=m_oVariantMediator->ToPoolString (ai_var);
	}   
	if (ret!=M4_SUCCESS){
		RuntimePush (vRes);
		return M4_FALSE;
	}
	return M4_TRUE;
}

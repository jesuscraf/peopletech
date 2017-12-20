

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:                TablFunc.cpp
// Project:        Cálculo
// Author:         Meta Software M.S. , S.A
// Date:            21 may 97
// Language:        C++ 
// Operating System: WINDOWS
// Design Document:   
//
// 
// Definition: 
//
//    This module implementa la clase tabla de funciones
////
//==============================================================================
#include "tablafuncdesc.hpp"
#include "vmres.hpp"
#include "tablfunc.h"

#include "syncro.hpp"
#include "chlog.hpp"


//*****************
//Constructor
//*****************

ClFuncTableDesc::ClFuncTableDesc(){


		
}

//*****************
//Instancia unica!!!!!
//*****************
ClFuncTableDesc* ClFuncTableDesc::m_poInstance = NULL;

// Mutex para la Creacion de la Instancia unica de la Tabla de Funciones (Seccion Critica)
ClMutex oMutexFuncTableDesc (M4_TRUE);

//*****************
//GetInstance	- Obtener la instancia unica!!!!
//crea (si es necesario) y devuelve la referencia a la instancia unica de la clase
//*****************
ClFuncTableDesc* ClFuncTableDesc::GetInstance() {
	//Comprobamos que la instancia no existe (debe ser unica)
	if (m_poInstance == NULL) {


		//Si no existia, se entra en la Seccion Critica, con doble-comprobacion
		oMutexFuncTableDesc.Lock();
		if (m_poInstance == NULL) {
			//Si no existia, se crea
			if ( (m_poInstance=new ClFuncTableDesc) != NULL)
				m_poInstance->InitLn4Semantic();
			else	//Error de no hay memoria
				m_poInstance = NULL;
		}
		oMutexFuncTableDesc.Unlock();
	}
	return m_poInstance;
}



//*******************
//AddFunction
//*******************
m4return_t ClFuncTableDesc::AddFunction(const m4char_t *ai_Name, m4int8_t ai_NumArgs, m4uint16_t ai_Id, m4char_t *ai_ParamDesc,  m4int8_t ai_iType, m4int8_t ai_iFlags, m4bool_t ai_bIsDatabase){
	
	m4char_t acName[M4CL_MAX_NOMBRE_FUNC];
	
	strcpy(acName,ai_Name);
	//pasamos a mayusculas
	strupr(acName);
	
	ClFunctionDesc oFuncTemp(acName,ai_NumArgs,ai_Id , ai_ParamDesc, ai_iType, ai_iFlags, ai_bIsDatabase);
	oTable.push_back(oFuncTemp);
	return M4_SUCCESS;
}

//EDU:
m4uint32_t ClFuncTableDesc::GetSize (void) const
{
    m4uint32_t length = 0;

    length += sizeof (ClFuncTableDesc);
    length += oTable.size () * sizeof (ClFunctionDesc);

    return length;
}

//**********************
//INIT
//**********************

void ClFuncTableDesc::InitLn4Semantic(){

	ClFuncTable::GetInstance();
	//Nos inicia otro!!!!
} 



/**
Acceso Datos de las funciones ***

*/
m4return_t ClFuncTableDesc::GetIdChannel(const m4char_t *ai_name, m4uint16_t &ao_Id, m4int8_t &ao_NumArgs, m4char_t *ao_ParamDesc){
	
	itVectorFuncionesDesc it;

	
	ClFunctionDesc oFuncTemp(ai_name, 0, 0, "", M4CL_CHANNEL_FUNC, 0, M4_FALSE);
	
	it=oTable.begin();
	while (it!=oTable.end()){
		
		it=find(it,oTable.end(),oFuncTemp);
		
		if (it==oTable.end()){
			return M4_ERROR;
		}
		else if ( (*it).GetType()==M4CL_CHANNEL_FUNC){ 
			break;
		}
        it++;
	}
	
	if (it==oTable.end()){
		return M4_ERROR;
	}

	ao_Id=(*it).GetId();
	ao_NumArgs=(*it).GetNumArgs();
	strcpy(ao_ParamDesc,(*it).GetParamDescription());

	return M4_SUCCESS;

}


m4return_t ClFuncTableDesc::GetIdSemantic(const m4char_t *ai_name, m4uint16_t &ao_Id, m4int8_t &ao_NumArgs, m4char_t *ao_ParamDesc){
	itVectorFuncionesDesc it;

	ClFunctionDesc oFuncTemp(ai_name, 0, 0, "", M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
	
	it=oTable.begin();
	while (it!=oTable.end()){
		
		it=find(it,oTable.end(),oFuncTemp);
		
		if (it==oTable.end()){
			return M4_ERROR;
		}
		else if ( (*it).GetType()==M4CL_SEMANTIC_FUNC || (*it).GetType()==M4CL_SEMANTIC_FUNC1 ||
				  (*it).GetType()==M4CL_SEMANTIC_FUNC2){ 
			break;
		}
        it++;
	}
	
	if (it==oTable.end()){
		return M4_ERROR;
	}

	ao_Id=(*it).GetId();
	ao_NumArgs=(*it).GetNumArgs();
	strcpy(ao_ParamDesc,(*it).GetParamDescription());
	return M4_SUCCESS;

}


m4return_t ClFuncTableDesc::GetId(const m4char_t *ai_name, m4uint16_t &ao_Id, m4int8_t &ao_NumArgs, m4char_t *ao_ParamDesc,  m4int8_t &ao_iType) {

	itVectorFuncionesDesc it;
	
	ClFunctionDesc oFuncTemp(ai_name, 0, 0, "", M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

	
	it=find(oTable.begin(),oTable.end(),oFuncTemp);
	if (it==oTable.end()) {
		return M4_ERROR;
	}
	else{
		ao_Id=(*it).GetId();
		ao_NumArgs=(*it).GetNumArgs();
		strcpy(ao_ParamDesc,(*it).GetParamDescription());
		ao_iType=(*it).GetType();
		return M4_SUCCESS;
	}
}


m4return_t ClFuncTableDesc::GetId(const m4char_t *ai_name, m4uint16_t &ao_Id) {

	itVectorFuncionesDesc it;
	
	ClFunctionDesc oFuncTemp(ai_name, 0, 0, "", M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

	
	it=find(oTable.begin(),oTable.end(),oFuncTemp);
	if (it==oTable.end()) {
		return M4_ERROR;
	}
	else{
		ao_Id=(*it).GetId();
		return M4_SUCCESS;
	}
}

m4return_t ClFuncTableDesc::GetChannelId(const m4char_t *ai_name, m4uint16_t &ao_Id) {

	itVectorFuncionesDesc it;
	m4char_t acName[M4CL_MAX_NOMBRE_FUNC];
	
	strcpy(acName,ai_name);

	//pasamos a mayusculas
	strupr(acName);
	
	ClFunctionDesc oFuncTemp(acName, 0, 0, "", M4CL_CHANNEL_FUNC, 0, M4_FALSE);

	it=oTable.begin();

	while (it!=oTable.end()){
		
		it=find(it,oTable.end(),oFuncTemp);
		
		if (it==oTable.end()){
			return M4_ERROR;
		}
		else if ( (*it).GetType()==M4CL_CHANNEL_FUNC){ 
			break;
		}
        it++;
	}

	if (it==oTable.end()){
		return M4_ERROR;
	}

	ao_Id=(*it).GetId();
	return M4_SUCCESS;	

}

m4return_t ClFuncTableDesc::Exists(m4uint16_t ai_Id, m4int8_t &ao_NumArg) {
	itVectorFuncionesDesc it;
	
	it=oTable.begin()+ai_Id;
	if (it>=oTable.end()) {
		return M4_ERROR;
	}
	if ( (*it).GetId() !=ai_Id){
		for (it=oTable.begin(); it<oTable.end(); it++){
			if ((*it).GetId()==ai_Id) {
				ao_NumArg=GetObjectFunction(ai_Id).GetNumArgs();
				return M4_SUCCESS;
			}
		}
		return M4_ERROR;

	}
	ao_NumArg=GetObjectFunction(ai_Id).GetNumArgs();
	return M4_SUCCESS;
}


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
//    This module implementa la clase tabla de attributos función
////
//==============================================================================
#include "tablatt.hpp"
#include "cpdefine.hpp"
#include "syncro.hpp"
#include "m4string.hpp"
#include <string.h>

//*****************
//Constructor
//*****************

ClAtributeFuncTable::ClAtributeFuncTable(){

		
}

//*****************
//Instancia unica!!!!!
//*****************
ClAtributeFuncTable* ClAtributeFuncTable::m_poInstance = NULL;

// Mutex para la Creacion de la Instancia unica de la Tabla de Atributos (Seccion Critica)
ClMutex oMutexAtributeTable (M4_TRUE);

//*****************
//GetInstance	- Obtener la instancia unica!!!!
//crea (si es necesario) y devuelve la referencia a la instancia unica de la clase
//*****************
ClAtributeFuncTable* ClAtributeFuncTable::GetInstance() {
	//Comprobamos que la instancia no existe (debe ser unica)
	if (m_poInstance == NULL) {
		//Si no existia, se entra en la Seccion Critica, con doble-comprobacion
		oMutexAtributeTable.Lock();
		if (m_poInstance == NULL) {
			//Si no existia, se crea
			if ( (m_poInstance=new ClAtributeFuncTable) != NULL)
				m_poInstance->InitAttribSemantic();
			else	//Error de no hay memoria
				m_poInstance = NULL;
		}
		oMutexAtributeTable.Unlock();
	}
	return m_poInstance;
}






//*******************
//AddAttribute
//*******************
m4return_t ClAtributeFuncTable::AddAttribute(const m4char_t *ai_Name, m4int8_t ai_NumArgs, m4uint16_t ai_Id, m4char_t *ai_ParamDesc, m4bool_t ai_bGenDep){
	
	m4char_t acName[M4CL_MAX_NOMBRE_FUNC];
	
	strcpy(acName,ai_Name);
	//pasamos a mayusculas
	strupr(acName);
	
	ClAttribute oFuncTemp(acName,ai_NumArgs,ai_Id,ai_ParamDesc, ai_bGenDep);
	oTable.push_back(oFuncTemp);
	return M4_SUCCESS;
}


m4uint32_t ClAtributeFuncTable::GetSize (void) const
{
    m4uint32_t length = 0;

    length += sizeof (ClAtributeFuncTable);
    length += oTable.size () * sizeof (ClAttribute);

    return length;
}



m4return_t ClAtributeFuncTable::GetId(const m4char_t *ai_name, m4uint16_t &ao_Id, m4int8_t &ao_NumArgs, m4char_t *ao_ParamDesc,  m4int8_t &ao_iType) {

	itVectorAtributos it;
	//m4char_t acName[M4CL_MAX_NOMBRE_FUNC];
	
	//strcpy(acName,ai_name);

	//pasamos a mayusculas
	//strupr(acName); ->Ya no se pasa a mayusculas
	
	ClAttribute oFuncTemp(ai_name,0,0,"",M4_FALSE);

	
	it=find(oTable.begin(),oTable.end(),oFuncTemp);
	if (it==oTable.end()) {
		return M4_ERROR;
	}
	else{
		ao_Id=(*it).GetId();
		ao_NumArgs=(*it).GetNumArgs();
		strcpy(ao_ParamDesc,(*it).GetParamDescription());
		return M4_SUCCESS;
	}
}


m4return_t ClAtributeFuncTable::GetId(const m4char_t *ai_name, m4uint16_t &ao_Id) {

	itVectorAtributos  it;
	//m4char_t acName[M4CL_MAX_NOMBRE_FUNC];
	
	//strcpy(acName,ai_name);

	//pasamos a mayusculas
	//strupr(acName); ->ya no se pasa a mayusculas
	
	ClAttribute oFuncTemp(ai_name,0,0,"",M4_FALSE);

	
	it=find(oTable.begin(),oTable.end(),oFuncTemp);
	if (it==oTable.end()) {
		return M4_ERROR;
	}
	else{
		ao_Id=(*it).GetId();
		return M4_SUCCESS;
	}
}


void ClAtributeFuncTable::InitAttribSemantic(void){

	
	AddAttribute("SysAddNewSlice",  2,  M4CL_ATT_MTHD_SYS_ADD_NEW_SLICE,     "RR",M4_FALSE);
	AddAttribute("SysGetItemName",  1,  M4CL_ATT_MTHD_SYS_GET_ITEM_NAME,        "r",M4_FALSE);
	AddAttribute("SysCreateSlice",  3,  M4CL_ATT_MTHD_SYS_CREATE_SLICE,        "RRR",M4_FALSE);
	AddAttribute("SysSplitSlice",   1,  M4CL_ATT_MTHD_SYS_SPLIT_SLICE,        "R", M4_FALSE);
	AddAttribute("SysTotalItems", 1, M4CL_ATT_MTHD_SYS_TOTALIZE_ITEMS,      "R",	M4_TRUE);
	AddAttribute("SysTotalSlices",1, M4CL_ATT_MTHD_SYS_TOTALIZE_SLICES,      "R",	M4_TRUE);
	AddAttribute("SysCreateBlobFile", 2, M4CL_ATT_MTHD_SYS_CREATE_BLOB_FILE,      "RR",	M4_FALSE);
	AddAttribute("SysSetBlobDescription",1, M4CL_ATT_MTHD_SYS_SET_BLOB_DESCRIP,      "R",	M4_FALSE);

}

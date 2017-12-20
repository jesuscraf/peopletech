//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:                TablFunc.h
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
//    This module defines la clase tabla de funciones
////
//==============================================================================
#ifndef _CLFUNCTABLE_H
#define _CLFUNCTABLE_H

#include "function.hpp"
#include "m4system.hpp"
#include "m4stl.hpp"
#include "vvm.hpp"

#include "vmtypes.hpp"
#include "vmdefine.hpp"
#include "functiondesc.hpp"

	 
//using namespace std;
//#pragma warning(disable:4786)  //warning de identificador muy largo

typedef vector<ClFunction> VectorFunciones;
typedef VectorFunciones::iterator itVectorFunciones;
typedef m4return_t ((ClVVM ::*pmfVM0) (void)  );

class ClFuncTable{
public:	
	static ClFuncTable* GetInstance(void);	//Crea (si es necesario) y devuelve la referencia a la instancia unica de la clase

protected:	//No podremos crear instancias de la clase directamente con "new"
	ClFuncTable();
	//OJO!!! No existe destructor... se crea una vez, y se mantiene en el "heap" durante toda la ejecucion

private:
	static ClFuncTable* m_poInstance;	//Instancia unica de la Clase: Singleton Pattern


public:
    //los id's deben ir en mayusculas
	m4return_t GetId(const m4char_t *ai_name, m4uint16_t &ao_Id, m4int8_t &ao_NumArgs, m4char_t *ao_ParamDesc,  m4int8_t &ao_iType) ;
	
	m4return_t GetIdChannel(const m4char_t *ai_name, m4uint16_t &ao_Id, m4int8_t &ao_NumArgs, m4char_t *ao_ParamDesc);
	m4return_t GetIdSemantic(const m4char_t *ai_name, m4uint16_t &ao_Id, m4int8_t &ao_NumArgs, m4char_t *ao_ParamDesc);
 
	m4return_t GetId(const m4char_t *ai_name, m4uint16_t &ao_Id) ;
	m4return_t GetChannelId (const m4char_t *ai_name, m4uint16_t &ao_Id) ;

	m4return_t GetExeFunc(m4uint16_t ai_Id, m4return_t(ClVVM::*&ao_pfFunc)(void), m4int8_t &ao_NumArg) ;

	void InitLn4Semantic(void);

	m4return_t AddFunction(const m4char_t *ai_Name, m4int8_t ai_NumArgs, m4uint16_t ai_Id, m4uint16_t ai_IdObj, m4char_t *ai_ParamDesc, m4return_t(ClVVM::*ai_pExeFunc)(void), m4int8_t ai_iType, m4int8_t ai_iFlags, m4bool_t ai_bIsDatabase);

    m4uint32_t GetSize (void) const;

	pmfVM0	GetExeFunc(m4uint16_t ai_id){
		return oTable[ai_id].GetExeFunc();	
	};


	m4uint16_t GetIdObject (m4uint16_t ai_id){
		return oTable[ai_id].GetIdObject();
	};

	ClFunctionDesc& GetObjectFunction(m4uint16_t ai_id) ;
	m4bool_t	HasReferedArgs (m4uint16_t ai_id);
	m4uint16_t	GetNumReferedArgs (m4uint16_t ai_id) ;
	m4int8_t	GetType (m4uint16_t ai_id);

private:
	
	VectorFunciones oTable;
};


#endif

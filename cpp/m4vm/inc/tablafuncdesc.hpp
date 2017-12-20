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


#ifndef _CLFUNCTABLEDESC_H
#define _CLFUNCTABLEDESC_H

#include "m4vm_dll.hpp"
#include "m4stl.hpp"

#include "functiondesc.hpp"

#include  "m4system.hpp"


typedef vector<ClFunctionDesc> VectorFuncionesDesc;
typedef VectorFuncionesDesc::iterator itVectorFuncionesDesc;

class M4_DECL_M4VM IFuncTableDesc{

public:
    //los id's deben ir en mayusculas
	virtual m4return_t GetId(const m4char_t *ai_name, m4uint16_t &ao_Id, m4int8_t &ao_NumArgs, m4char_t *ao_ParamDesc,  m4int8_t &ao_iType)=0 ;
	
	virtual m4return_t GetIdChannel(const m4char_t *ai_name, m4uint16_t &ao_Id, m4int8_t &ao_NumArgs, m4char_t *ao_ParamDesc) =0;
	virtual m4return_t GetIdSemantic(const m4char_t *ai_name, m4uint16_t &ao_Id, m4int8_t &ao_NumArgs, m4char_t *ao_ParamDesc) =0;
 
	virtual m4return_t GetId(const m4char_t *ai_name, m4uint16_t &ao_Id) =0;
	virtual m4return_t GetChannelId (const m4char_t *ai_name, m4uint16_t &ao_Id) =0;

	virtual m4return_t Exists(m4uint16_t ai_Id, m4int8_t &ao_NumArg) =0;
	
	virtual void InitLn4Semantic(void) =0;
	
	virtual m4return_t AddFunction(const m4char_t *ai_Name, m4int8_t ai_NumArgs, m4uint16_t ai_Id, m4char_t *ai_ParamDesc,  m4int8_t ai_iType, m4int8_t ai_iFlags, m4bool_t ai_bIsDatabase) =0;
    
	virtual m4uint32_t GetSize (void) const=0;
	
	virtual m4int8_t	GetType (m4uint16_t ai_id)=0;
	virtual	m4int8_t	GetFlags(m4uint16_t ai_id)=0;
	virtual	m4bool_t	GetIsDatabase(m4uint16_t ai_id)=0;
	virtual	m4pcchar_t	GetName (m4uint16_t ai_id)=0;
	
	virtual ClFunctionDesc& GetObjectFunction(m4uint16_t ai_id) =0;
	
	virtual m4bool_t	HasReferedArgs (m4uint16_t ai_id)=0;
 	virtual m4uint16_t	GetNumReferedArgs (m4uint16_t ai_id)=0;
};



class M4_DECL_M4VM ClFuncTableDesc:public IFuncTableDesc{
public:	
	static ClFuncTableDesc* GetInstance();	//Crea (si es necesario) y devuelve la referencia a la instancia unica de la clase

protected:	//No podremos crear instancias de la clase directamente con "new"
	ClFuncTableDesc();
	//OJO!!! No existe destructor... se crea una vez, y se mantiene en el "heap" durante toda la ejecucion

private:
	static ClFuncTableDesc* m_poInstance;	//Instancia unica de la Clase: Singleton Pattern


public:
    //los id's deben ir en mayusculas
	m4return_t GetId(const m4char_t *ai_name, m4uint16_t &ao_Id, m4int8_t &ao_NumArgs, m4char_t *ao_ParamDesc,  m4int8_t &ao_iType) ;
	
	m4return_t GetIdChannel(const m4char_t *ai_name, m4uint16_t &ao_Id, m4int8_t &ao_NumArgs, m4char_t *ao_ParamDesc);
	m4return_t GetIdSemantic(const m4char_t *ai_name, m4uint16_t &ao_Id, m4int8_t &ao_NumArgs, m4char_t *ao_ParamDesc);
 
	m4return_t GetId(const m4char_t *ai_name, m4uint16_t &ao_Id) ;
	m4return_t GetChannelId (const m4char_t *ai_name, m4uint16_t &ao_Id) ;

	m4return_t Exists(m4uint16_t ai_Id, m4int8_t &ao_NumArg) ;

	void InitLn4Semantic(void);

	m4return_t AddFunction(const m4char_t *ai_Name, m4int8_t ai_NumArgs, m4uint16_t ai_Id, m4char_t *ai_ParamDesc,  m4int8_t ai_iType, m4int8_t ai_iFlags, m4bool_t ai_bIsDatabase);

    //EDU:
    m4uint32_t GetSize (void) const;


	m4int8_t GetType(m4uint16_t ai_id){
		return oTable[ai_id].GetType();
	}
	m4int8_t GetFlags(m4uint16_t ai_id)	{
		return oTable[ai_id].GetFlags();
	}
	m4bool_t GetIsDatabase(m4uint16_t ai_id)	{
		return oTable[ai_id].GetIsDatabase();
	}
	m4pcchar_t	GetName(m4uint16_t ai_id) {
		return oTable[ai_id].GetName();
	}

	ClFunctionDesc& GetObjectFunction(m4uint16_t ai_id) {
		return oTable[ai_id];
	}

	m4bool_t	HasReferedArgs (m4uint16_t ai_id){
		return oTable[ai_id].HasReferedArgs();
	}
 
	m4uint16_t	GetNumReferedArgs (m4uint16_t ai_id) {
		return oTable[ai_id].GetNumReferedArgs ();
	}

private:
	
	VectorFuncionesDesc oTable;
};


#endif

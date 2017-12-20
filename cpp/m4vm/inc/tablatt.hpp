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

#ifndef _CLATTTABLE_H
#define _CLATTTABLE_H

#include "m4vm_dll.hpp"

#include "m4stl.hpp"
#include "attrib.hpp"
#include "m4system.hpp"


	 
typedef vector<ClAttribute> VectorAtributos;
typedef VectorAtributos::iterator itVectorAtributos;

class M4_DECL_M4VM IAtributeFuncTable{

public:
      //los id's deben ir en mayusculas
	virtual m4return_t GetId(const m4char_t *ai_name, m4uint16_t &ao_Id, m4int8_t &ao_NumArgs, m4char_t *ao_ParamDesc,  m4int8_t &ao_iType) =0;
	virtual m4return_t GetId(const m4char_t *ai_name, m4uint16_t &ao_Id) =0;
	virtual m4int8_t GetType (m4uint16_t ai_id)=0;
	virtual m4int8_t GetNumArgs (m4uint16_t ai_id)=0;

	virtual m4bool_t   GetGenDependencie (m4uint16_t ai_id)=0;

	virtual ClAttribute& GetObjectAttribute(m4uint16_t ai_id) =0;
	virtual m4bool_t	HasReferedArgs (m4uint16_t ai_id)=0;

	virtual m4uint16_t	GetNumReferedArgs (m4uint16_t ai_id)=0;

	virtual m4return_t AddAttribute(const m4char_t *ai_Name, m4int8_t ai_NumArgs, m4uint16_t ai_Id, m4char_t *ai_ParamDesc, m4bool_t ai_bGenDep)=0;
    virtual m4uint32_t GetSize (void) const=0;
	
	virtual void InitAttribSemantic(void)=0;
};

class M4_DECL_M4VM ClAtributeFuncTable: public IAtributeFuncTable{
public:	
	static ClAtributeFuncTable* GetInstance();	//Crea (si es necesario) y devuelve la referencia a la instancia unica de la clase

protected:	//No podremos crear instancias de la clase directamente con "new"
	ClAtributeFuncTable();
	//OJO!!! No existe destructor... se crea una vez, y se mantiene en el "heap" durante toda la ejecucion

private:
	static ClAtributeFuncTable* m_poInstance;	//Instancia unica de la Clase: Singleton Pattern


public:
      //los id's deben ir en mayusculas
	m4return_t GetId(const m4char_t *ai_name, m4uint16_t &ao_Id, m4int8_t &ao_NumArgs, m4char_t *ao_ParamDesc,  m4int8_t &ao_iType) ;
	m4return_t GetId(const m4char_t *ai_name, m4uint16_t &ao_Id) ;
	m4int8_t GetType (m4uint16_t ai_id){
		return oTable[ai_id].GetType();
	};
	m4int8_t GetNumArgs (m4uint16_t ai_id){
		return oTable[ai_id].GetNumArgs();
	};

	m4bool_t   GetGenDependencie (m4uint16_t ai_id){
		return oTable[ai_id].GetGenDependencie();
	}

	ClAttribute& GetObjectAttribute(m4uint16_t ai_id) {
		return oTable[ai_id];
	}

	m4bool_t	HasReferedArgs (m4uint16_t ai_id){
		return oTable[ai_id].HasReferedArgs();
	}

	m4uint16_t	GetNumReferedArgs (m4uint16_t ai_id) {
		return oTable[ai_id].GetNumReferedArgs ();
	}

	m4return_t AddAttribute(const m4char_t *ai_Name, m4int8_t ai_NumArgs, m4uint16_t ai_Id, m4char_t *ai_ParamDesc, m4bool_t ai_bGenDep);
    m4uint32_t GetSize (void) const;
	
	void InitAttribSemantic(void);
private:
	
	VectorAtributos oTable;



};


#endif

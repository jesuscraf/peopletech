//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                vmdefine.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                15-05-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    for debug only
//
//
//==============================================================================
#ifndef _CL_VM_TRACER_H
#define _CL_VM_TRACER_H

#include "m4stl.hpp"
#include "vmdefine.hpp"
#include "vmtypes.hpp"
#include "syncro.hpp"


class ClVMBaseEnv;

class VMTracer{
public:
	static VMTracer& GetInstance(ClVMBaseEnv *ai_poEnv);	//Crea (si es necesario) y devuelve la referencia a la instancia unica de la clase

protected:
	VMTracer (const char *ai_pszFileName);	//Constructor
	//OJO!!! No existe destructor... se crea una vez, y se mantiene en el "heap" durante toda la ejecucion

private:
	static VMTracer* m_poInstance;	//Instancia unica de la Clase: Singleton Pattern

public:
	//VMTracer (const char *ai_pszFileName);
	virtual ~VMTracer ();

	VMTracer &operator << (m4double_t ai_d);
	VMTracer &operator << (m4uint32_t ai_ui);
	VMTracer &operator << (m4int32_t ai_i);
	VMTracer &operator << (m4int_t ai_i);
	VMTracer &operator << (m4pcchar_t ai_s);
	VMTracer &operator << (m4char_t ai_c);
	VMTracer &operator << (m4bool_t ai_b);
	VMTracer &operator << (m4float_t ai_f);

	void flush (void);
	
	void Lock(void);
	void Unlock(void);
	
	friend 	VMTracer& operator<< (VMTracer&f, VMTracer& (*_f)( VMTracer& )) ;
	
protected:
	ofstream os;
	ClMutex	m_oMutex;
};

	VMTracer &operator<< (VMTracer&f, VMTracer& (*_f)( VMTracer& ) ) ;
 	VMTracer &endl			(VMTracer & aio_vmt);
	VMTracer &beginthread	(VMTracer & aio_vmt);
	VMTracer &endthread		(VMTracer & aio_vmt);
	
#endif	//_CL_VM_TRACER_H


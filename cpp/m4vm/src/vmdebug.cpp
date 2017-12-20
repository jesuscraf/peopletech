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


#include "vmdefine.hpp"
#include "vmdebug.hpp"
#include "m4stl.hpp"

#include "m4thread.hpp"

#include "cldbgutil.hpp"
#include "m4unicode.hpp"


//*****************
//Instancia unica!!!!!
//*****************
VMTracer* VMTracer::m_poInstance = 0;

// Mutex para la Creacion de la Instancia unica del VMTracer (Seccion Critica)
//ClMutex oMutexVMTracer(M4_TRUE);
//Reutilizamos el mismo mutex para este singletton que para la tabla de funciones,
//en vez de crear otro global
extern ClMutex oMutexFuncTable;
ClMutex& oMutexSingletton = oMutexFuncTable;


//*****************
//GetInstance	- Obtener la instancia unica!!!!
//crea (si es necesario) y devuelve la referencia a la instancia unica de la clase
//*****************
VMTracer& VMTracer::GetInstance(ClVMBaseEnv *ai_poEnv)
{
	//Comprobamos que la instancia no existe (debe ser unica)
	if (!m_poInstance) {
		//Si no existia, se entra en la Seccion Critica, con doble-comprobacion
		oMutexSingletton.Lock();
		if (!m_poInstance) {
			//Si no existia, se crea
			m_poInstance=new VMTracer( M4VMGetDebugFileName(M4CL_VM_SLICES_DEBUG_FILE, ai_poEnv).c_str() );
			if ( !m_poInstance) {
				//Error de no hay memoria
				m_poInstance = 0;
			}
		}
		oMutexSingletton.Unlock();
	}
	return *m_poInstance;
}



VMTracer::VMTracer (const char *ai_pszFileName):m_oMutex (M4_TRUE)
{
	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniNative ;
	M4Open( os, ai_pszFileName, M4UniWrite, eEncoding );
}


VMTracer::~VMTracer ()
{
	os.close();
}


VMTracer &VMTracer::operator << (m4double_t ai_d)
{
	m4char_t pszTemp [M4CL_MAX_STRING];
	sprintf(pszTemp,"%.6f", ai_d);
	/*
	//GP en Win32 (no en Unix) 
	//double DoubleData = -123456789012345678901.0;
	os  << setiosflags(ios::fixed)
	<< setw (10)
	<< setprecision (6)
	<<ai_d;
	*/
	os << pszTemp;
	return *this;
}

VMTracer &VMTracer::operator << (m4uint32_t ai_ui)
{
    os<<ai_ui;
	return *this;
    
}

VMTracer &VMTracer::operator << (m4int_t ai_i)
{
    os<<ai_i;
	return *this;
}

VMTracer &VMTracer::operator << (m4int32_t ai_i)
{
    os<<ai_i;
	return *this;

}

VMTracer &VMTracer::operator << (m4pcchar_t ai_s)
{
    os<<ai_s;
	return *this;

}

VMTracer &VMTracer::operator << (m4char_t ai_c)
{
    os<<ai_c;
	return *this;
}

VMTracer &VMTracer::operator << (m4bool_t ai_b)
{
    os<<ai_b;
	return *this;
}

VMTracer &VMTracer::operator << (m4float_t ai_f)
{
    os<<ai_f;
	return *this;
}

void VMTracer::flush (void)
{
	os.flush();
}


//exclusion mutua
void VMTracer::Lock(void)
{
	m_oMutex.Lock();
}


void VMTracer::Unlock(void)
{
	m_oMutex.Unlock();
}

//conversion para manipuladores

VMTracer& operator<< (VMTracer&f, VMTracer& (*_f)( VMTracer& ) )
{
    return (*_f) (f);
    
}
//manipuladores

VMTracer &endl	(VMTracer & aio_vmt)
{
	
	aio_vmt<<"\n";
	aio_vmt.flush();
	return aio_vmt;

}

VMTracer &beginthread	(VMTracer & aio_vmt)
{
	aio_vmt.Lock();
	return aio_vmt<<"@ Id("<<(m4uint32_t)M4Thread::GetCurrentIdThread()<<"):\t";
   
}

VMTracer &endthread		(VMTracer & aio_vmt)
{
	
	aio_vmt<<"\t|"<<endl;
	aio_vmt.Unlock();
	return aio_vmt;
}



	
#ifdef _DEBUG
void StopIfWanted(void)
{
	return ;
}
#endif




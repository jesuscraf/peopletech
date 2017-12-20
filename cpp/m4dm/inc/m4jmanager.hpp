
//==============================================================================
//
// (c) Copyright  1991-2015 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4dm.dll
// File:                m4jmanager.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                05-01-2015
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines a class for java execution manager.
//
//
//==============================================================================

#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include "m4stl.hpp"
#include "m4jobject.hpp"
#include "m4jhandleprovider.hpp"


class ClChannel ;
class M4JavaContext ;



#ifndef __M4JMANAGER_HPP__
#define __M4JMANAGER_HPP__


struct m4j_m4pchar_t_less
{
	m4bool_t operator()( m4pchar_t ai_pccString1, m4pchar_t ai_pccString2 ) const
	{
		return( strcmp( ai_pccString1, ai_pccString2 ) < 0 ? M4_TRUE : M4_FALSE ) ;
	}
};

typedef map<m4pchar_t, M4JavaContext*, m4j_m4pchar_t_less>		MapJavaContext ;
typedef MapJavaContext::iterator								itMapJavaContext ;




//=================================================================================
//
// M4JavaManager
//
// Clase que define un manager de ejecución de java
//
//=================================================================================

class M4_DECL_M4DM	M4JavaManager
{

protected:

	M4JavaHandleProvider	m_oHandleProvider ;
	MapJavaContext			m_oContexts ;

public:

// Funciones de inicialización ================================================
	
        M4JavaManager( m4uint8_t ai_iCsType ) ;
        ~M4JavaManager( void ) ;

// Funciones de lectura =======================================================

	m4pcchar_t	GetJavaClassName( m4double_t ai_dHandle ) const ;

// Funciones de ejecución =====================================================

	m4return_t	CreateObject ( ClChannel *ai_poChannel, m4pcchar_t ai_pccClassPath, m4pcchar_t ai_pccIdentifier, m4pcchar_t ai_pccClassName,                          M4JavaObject ai_apvArguments[], m4uint16_t ai_iLength, m4double_t &ao_rdHandle,   m4pcchar_t ai_pccSource ) ;
	m4return_t	DestroyObject( ClChannel *ai_poChannel, m4double_t ai_dHandle,                                                                                                                                                                          m4pcchar_t ai_pccSource ) ;
	m4return_t	InvokeMethod ( ClChannel *ai_poChannel, m4double_t ai_dHandle,                                                               m4pcchar_t ai_pccMethod, M4JavaObject ai_apvArguments[], m4uint16_t ai_iLength, M4JavaObject &ao_rvResult, m4pcchar_t ai_pccSource ) ;
	m4return_t	InvokeMethod ( ClChannel *ai_poChannel, m4pcchar_t ai_pccClassPath, m4pcchar_t ai_pccIdentifier, m4pcchar_t ai_pccClassName, m4pcchar_t ai_pccMethod, M4JavaObject ai_apvArguments[], m4uint16_t ai_iLength, M4JavaObject &ao_rvResult, m4pcchar_t ai_pccSource ) ;

protected:

// Funciones internas =========================================================

	static m4int16_t	_NewIdentifier( m4uint8_t ai_iCsType ) ;
	M4JavaContext*		_GetContext( ClChannel *ai_poChannel, m4pcchar_t ai_pccSource, m4pcchar_t ai_pccClassPath, m4pcchar_t ai_pccIdentifier ) ;

// Funciones de ciclo de vida =================================================

	m4return_t	UnReferenceJavaContext( M4JavaContext *ai_poContext ) ;

friend class ClChannel;
} ;

#endif


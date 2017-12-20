//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             sesschn.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/31/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//
// Definition:
//
//
//==============================================================================

#ifndef __SESSCHN__H__
#define __SESSCHN__H__

//=========================================================================

#include "xapichn.h"


//============================================================================ClMIT_SessionChn


class ClMIT_SessionChn : private ClMIT_Chn
{
MIT_PRIVATE:

	ClMIT_ChnNode *m_poSessNode ;
	ClMIT_ChnItem m_oLanguage ;
	ClMIT_ChnItem m_poIsRTL_Language ;
	ClMIT_ChnItem m_poReadOnlyPDF ;
	ClMIT_ChnItem m_poRestrictOutputDir ;
	ClMIT_ChnItem m_poM4UnitActive;
	
MIT_PUBLIC:


	// ----------------------------- Inicialización---------------------------------------	

	ClMIT_SessionChn();
	virtual ~ClMIT_SessionChn() { ClMIT_SessionChn::End(); }
	virtual size_t GetSizeof() { return sizeof(ClMIT_SessionChn) ; }

	virtual void End();

	// Contruye un acceso privado al canal sesion. Lo mas normal es no pasar el Manager, en
	//	cuyo caso se usara el canal sesion asociado al manager por defecto (si aun no se
	//	levanto, se genera error, pues la llamada se ha hecho fuera de contexto) .

	m4bool_t Init( ClChannelManager *ai_poManager=NULL, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR ) ;


	m4int16_t GetLanguage() ;

	m4bool_t IsRTL_Language() ;

	m4bool_t IsReadOnlyPDF();

	m4bool_t GetRestrictOutputDir();

	m4bool_t M4UnitActive();
};


#endif



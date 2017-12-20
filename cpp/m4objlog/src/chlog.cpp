//## begin module%391285880357.cm preserve=no
//## end module%391285880357.cm

//## begin module%391285880357.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//## end module%391285880357.cp

//## Module: chlog%391285880357; Package body
//## Subsystem: M4ObjLog::src%3912860F02F7
//## Source file: d:\new\m4objlog\src\chlog.cpp

//## begin module%391285880357.additionalIncludes preserve=no
//## end module%391285880357.additionalIncludes

//## begin module%391285880357.includes preserve=yes
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                chlog.cpp
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                19-11-1998
// Language:            C++
// Operating System:    WINDOWS, UNIX
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    Class implementation that is a wrapper for M4log in Chlib
//
//
//==============================================================================


#include "chlog.hpp"
#include <string.h>


#include "m4varbase.hpp"
#include "m4log.hpp"
#include "syncro.hpp"
//## end module%391285880357.includes

// basiclog
#include <basiclog.hpp>
//## begin module%391285880357.declarations preserve=no
//## end module%391285880357.declarations

//## begin module%391285880357.additionalDeclarations preserve=yes



// Global object for Log
ChLog g_oChLog;



//## end module%391285880357.additionalDeclarations


// Class ChLog 


//## Other Operations (implementation)
m4return_t ChLog::GetSize (m4uint32_t& ao_iSize)
{
  //## begin ChLog::GetSize%-880623417.body preserve=yes
	// Tamaño ocupado = objeto + BufferSize + Mutex
	ao_iSize = sizeof(*this) + m_iSize + sizeof(*m_poMutex);
	return M4_SUCCESS;
  //## end ChLog::GetSize%-880623417.body
}

// Additional Declarations
  //## begin ChLog%3912858801E5.declarations preserve=yes
  //## end ChLog%3912858801E5.declarations

//## begin module%391285880357.epilog preserve=yes


// Sobrecarga de operador <<  para otros tipos

ChLog &operator << (ChLog& aio_oLog,const m4VariantTypeBase &ai_v)
{
	if (aio_oLog.IsDumpActive()) {
		m4pchar_t pcBuffer;
		switch (ai_v.Type)
		{
			case M4CL_CPP_TYPE_STRING:
			case M4CL_CPP_TYPE_STRING_VAR:
				aio_oLog.PrepareOutput("%s", pcBuffer, (ai_v.Data.PointerChar? (m4uint32_t)(strlen(ai_v.Data.PointerChar)) : 1) );
				sprintf(pcBuffer, aio_oLog.m_pcCadFormato, (ai_v.Data.PointerChar? ai_v.Data.PointerChar : " ") );
				break;

			case M4CL_CPP_TYPE_NUMBER:
				aio_oLog.PrepareOutput("%10.8f", pcBuffer, 30);
				sprintf(pcBuffer, aio_oLog.m_pcCadFormato, ai_v.Data.DoubleData );
				break;

			case M4CL_CPP_TYPE_DATE:
				aio_oLog.PrepareOutput("%10.8f", pcBuffer, 30);
				sprintf(pcBuffer, aio_oLog.m_pcCadFormato, ai_v.Data.DoubleData );
				break;

			default:
				break;

		} //end-switch
		aio_oLog.m_iPos += strlen(pcBuffer);
	}
	return aio_oLog;
}


// Función global que devuelve si un error está ya en la pila o no

m4uint8_t	M4ChLogIsErrorIn( m4int32_t ai_iError )
{

	m4uint8_t	iResult ;
	m4int32_t	i, j ;
	m4int32_t	iLength ;
	m4int32_t	iError ;


	iLength = GET_ERROR_QUEUE_SIZE() ;

	iResult = 0 ;
	j = iLength ;
	for( i = 0 ; i < iLength && iResult == 0; i++ )
	{
		GETH_DECIMAL_N_ERRORCODE( --j, iError ) ;

		if( iError == ai_iError )
		{
			iResult = 1 ;
		}
	}

	return( iResult ) ;
}


//## end module%391285880357.epilog

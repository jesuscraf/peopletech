//## begin module%3912A6BB03D2.cm preserve=no
//## end module%3912A6BB03D2.cm

//## begin module%3912A6BB03D2.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.hpp
//	 Project:            M4ComSrv
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
//	=
//## end module%3912A6BB03D2.cp

//## Module: cltypes%3912A6BB03D2; Package specification
//## Subsystem: m4objglb::inc%3912A6EC01C8
//## Source file: d:\new\m4objglb\inc\cltypes.hpp

#ifndef __CLTYPES_HPP__
#define __CLTYPES_HPP__ 1

//## begin module%3912A6BB03D2.additionalIncludes preserve=no
//## end module%3912A6BB03D2.additionalIncludes

//## begin module%3912A6BB03D2.includes preserve=yes


#include "m4types.hpp"
//## end module%3912A6BB03D2.includes

//## begin module%3912A6BB03D2.declarations preserve=no
//## end module%3912A6BB03D2.declarations

//## begin module%3912A6BB03D2.additionalDeclarations preserve=yes
//## end module%3912A6BB03D2.additionalDeclarations


//## begin module%3912A6BB03D2.epilog preserve=yes




// Para el metacanal


typedef struct StArgumentMappig_tag
{
    m4uint8_t   m_iFatherRealOrder ;
    m4uint8_t   m_iFatherArgumentType ;
    m4uint8_t   m_iSonRealOrder ;
    m4uint8_t   m_iMappingType ;

} StArgumentMappig_t ;


typedef m4uint8_t priority_t;
//#pragma warning(disable:4786)


// funcion de comparacíon de la qort
typedef	int( *m4comparef_t )( const void*, const void* ) ;


//## end module%3912A6BB03D2.epilog


#endif

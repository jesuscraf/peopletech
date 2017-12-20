//## begin module%3912A6BB031C.cm preserve=no
//## end module%3912A6BB031C.cm

//## begin module%3912A6BB031C.cp preserve=no
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
//## end module%3912A6BB031C.cp

//## Module: clextole%3912A6BB031C; Package specification
//## Subsystem: m4objglb::inc%3912A6EC01C8
//## Source file: d:\new\m4objglb\inc\clextole.hpp

#ifndef __CLEXTOLE__HPP__
#define __CLEXTOLE__HPP__ 1

//## begin module%3912A6BB031C.additionalIncludes preserve=no
//## end module%3912A6BB031C.additionalIncludes

//## begin module%3912A6BB031C.includes preserve=yes

#include "clextobj.hpp"


/* COMENTADO PARA COMPILACIÓN EN UNIX PORQUE POR AHORA NO SE USA
//----Para los Objetos Ole
class	M4ClExternOle: public M4ClExternObject
{
private:
	IDispatch* m_pDsp;

	//Quizas poner una hash con todas las funciones de la dll ya cargadas.

public :

	M4ClExternOle( void ) ;
	m4return_t Init( m4pcchar_t ai_pccOleName ) ;
	~M4ClExternOle( void ) ;

	m4return_t Execute ( m4pcchar_t ai_pccMethodName, m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext) ;

} ;
*/

//## end module%3912A6BB031C.includes

//## begin module%3912A6BB031C.declarations preserve=no
//## end module%3912A6BB031C.declarations

//## begin module%3912A6BB031C.additionalDeclarations preserve=yes
//## end module%3912A6BB031C.additionalDeclarations


//## begin module%3912A6BB031C.epilog preserve=yes
//## end module%3912A6BB031C.epilog


#endif

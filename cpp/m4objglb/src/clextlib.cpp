//## begin module%3912A6BD0292.cm preserve=no
//## end module%3912A6BD0292.cm

//## begin module%3912A6BD0292.cp preserve=no
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
//## end module%3912A6BD0292.cp

//## Module: clextlib%3912A6BD0292; Package body
//## Subsystem: m4objglb::src%3912A6F00372
//## Source file: d:\new\m4objglb\src\clextlib.cpp

//## begin module%3912A6BD0292.additionalIncludes preserve=no
//## end module%3912A6BD0292.additionalIncludes

//## begin module%3912A6BD0292.includes preserve=yes
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:             clextlib.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             06/07/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================


#include "clextlib.hpp"
#include "chlog.hpp"
#include "m4glbres.hpp"
//## end module%3912A6BD0292.includes

// clextobj
#include <clextobj.hpp>
// m4types
#include <m4types.hpp>
// m4loadll
#include <m4loadll.hpp>
//## begin module%3912A6BD0292.declarations preserve=no
//## end module%3912A6BD0292.declarations

//## begin module%3912A6BD0292.additionalDeclarations preserve=yes

// Type definition of Dll function pointers
typedef m4int32_t (* LPFNDLLFUNC)(...);
typedef m4return_t (* LPFNDLLTYPEDFUNC)( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );



//## end module%3912A6BD0292.additionalDeclarations


// Class M4ClExternLibrary 




M4ClExternLibrary::M4ClExternLibrary (void )
  //## begin M4ClExternLibrary::M4ClExternLibrary%-997596868.hasinit preserve=no
  //## end M4ClExternLibrary::M4ClExternLibrary%-997596868.hasinit
  //## begin M4ClExternLibrary::M4ClExternLibrary%-997596868.initialization preserve=yes
  //## end M4ClExternLibrary::M4ClExternLibrary%-997596868.initialization
{
  //## begin M4ClExternLibrary::M4ClExternLibrary%-997596868.body preserve=yes
	m_bInit = M4_FALSE;
	m_pszObjectName=0;

  //## end M4ClExternLibrary::M4ClExternLibrary%-997596868.body
}


M4ClExternLibrary::~M4ClExternLibrary (void )
{
  //## begin M4ClExternLibrary::~M4ClExternLibrary%-1277333319.body preserve=yes
	m4return_t ret;

	if (m_bInit) {

		ret = m_oDll.Close ();
				
		//if (ret != M4_SUCCESS) {
		//	SETCODE(2,2,"Error closing Dll");
		//}		
	}
	if (m_pszObjectName)
		delete []m_pszObjectName;

  //## end M4ClExternLibrary::~M4ClExternLibrary%-1277333319.body
}



//## Other Operations (implementation)
m4return_t M4ClExternLibrary::Init (m4pcchar_t ai_pccDllName)
{
  //## begin M4ClExternLibrary::Init%-1655963059.body preserve=yes
	m4return_t ret;

	if  (ai_pccDllName){
		m_pszObjectName=new char[strlen(ai_pccDllName)+1];
		strcpy (m_pszObjectName,ai_pccDllName);
	}

	ret = m_oDll.Open (ai_pccDllName);
	

	if ( ret != M4_SUCCESS)
	{
		m_bInit = M4_FALSE;
		DUMP_CHLOG_ERRORF(M4_OBJGLB_CANT_FIND_LIBRARY, ai_pccDllName );
		return M4_ERROR;
	}

	m_bInit = M4_TRUE;

	return M4_SUCCESS;

  //## end M4ClExternLibrary::Init%-1655963059.body
}

m4return_t M4ClExternLibrary::Execute (m4pcchar_t ai_pccMethodName, m4VariantType* ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType& ai_pvReturn, m4pvoid_t ai_pvContext)
{
  //## begin M4ClExternLibrary::Execute%-59165089.body preserve=yes

	m4return_t ret;	

	m4typefunreturn		fpM4function;
	LPFNDLLTYPEDFUNC	fpProc;

	if (!m_bInit)
		return M4_ERROR;

	// Get Dll function pointer through M4loadll
	ret = m_oDll.GetFunc(ai_pccMethodName, fpM4function);
	if (ret != M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_OBJGLB_CANT_FIND_METHOD, ai_pccMethodName<<m_pszObjectName );
		return M4_ERROR;
	}
		
	fpProc = (LPFNDLLTYPEDFUNC) fpM4function;

	if ( !fpProc ){
		DUMP_CHLOG_ERRORF(M4_OBJGLB_CANT_FIND_METHOD, ai_pccMethodName<<m_pszObjectName );
		return M4_ERROR;
	}

	// Execute function
	ret = fpProc ( ai_pvArg, ai_iLongArg, ai_pvReturn, ai_pvContext );
	if (ret==M4_ERROR){
		g_oChLog<<BeginError(M4_OBJGLB_EXTERNAL_METHOD_ERROR)<<ai_pccMethodName<<m_pszObjectName<<EndLog;
	}

	return ret;

  //## end M4ClExternLibrary::Execute%-59165089.body
}

// Additional Declarations
  //## begin M4ClExternLibrary%3912A6B800AE.declarations preserve=yes
  //## end M4ClExternLibrary%3912A6B800AE.declarations

//## begin module%3912A6BD0292.epilog preserve=yes
//## end module%3912A6BD0292.epilog

//## begin module%37E602150307.cm preserve=no
//## end module%37E602150307.cm

//## begin module%37E602150307.cp preserve=no
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
//## end module%37E602150307.cp

//## Module: jsjobbuilder%37E602150307; Package body
//## Subsystem: M4JSRecover::src%37F8B6A102CB
//## Source file: C:\Trabajo\Server\m4jsrecover\version\src\jsjobbuilder.cpp

//## begin module%37E602150307.additionalIncludes preserve=no
//## end module%37E602150307.additionalIncludes

//## begin module%37E602150307.includes preserve=yes
//## end module%37E602150307.includes

// jsjobbuilder
#include <jsjobbuilder.hpp>
// jsjob
#include <jsjob.hpp>
// jscompoundjob
#include <jscompoundjob.hpp>
// jssimplejob
#include <jssimplejob.hpp>
//## begin module%37E602150307.declarations preserve=no
//## end module%37E602150307.declarations

//## begin module%37E602150307.additionalDeclarations preserve=yes
//## end module%37E602150307.additionalDeclarations


// Class ClJSJobBuilder 



ClJSJobBuilder::ClJSJobBuilder ()
  //## begin ClJSJobBuilder::ClJSJobBuilder%941016332.hasinit preserve=no
  //## end ClJSJobBuilder::ClJSJobBuilder%941016332.hasinit
  //## begin ClJSJobBuilder::ClJSJobBuilder%941016332.initialization preserve=yes
  //## end ClJSJobBuilder::ClJSJobBuilder%941016332.initialization
{
  //## begin ClJSJobBuilder::ClJSJobBuilder%941016332.body preserve=yes
	m_uiRemainingJobs = 0;
  //## end ClJSJobBuilder::ClJSJobBuilder%941016332.body
}


ClJSJobBuilder::~ClJSJobBuilder ()
{
  //## begin ClJSJobBuilder::~ClJSJobBuilder%941120254.body preserve=yes
	CleanProductList();
  //## end ClJSJobBuilder::~ClJSJobBuilder%941120254.body
}



//## Other Operations (implementation)
ClJSJob * ClJSJobBuilder::GetpProduct ()
{
  //## begin ClJSJobBuilder::GetpProduct%939977800.body preserve=yes
	ClJSJob * pProducto = NULL      ;/* Puntero a devolver                    */
	ProductList::iterator iterador  ;/* Iterador para recorrer la lista       */

	if (m_lProducts.size() > 0)
	{
		iterador = m_lProducts.begin();
		pProducto = (* iterador);
		m_lProducts.remove( pProducto);
	}

	return pProducto;
  //## end ClJSJobBuilder::GetpProduct%939977800.body
}

m4return_t ClJSJobBuilder::GetRemainingJobs (m4uint_t &ao_uiNumJobs)
{
  //## begin ClJSJobBuilder::GetRemainingJobs%941016330.body preserve=yes
	ao_uiNumJobs = m_uiRemainingJobs;
	return M4_SUCCESS;
  //## end ClJSJobBuilder::GetRemainingJobs%941016330.body
}

m4return_t ClJSJobBuilder::GetNumberOfProducts (m4uint_t &ao_uiNumJobs)
{
  //## begin ClJSJobBuilder::GetNumberOfProducts%941016331.body preserve=yes
	ao_uiNumJobs = m_lProducts.size();
	return M4_SUCCESS;
  //## end ClJSJobBuilder::GetNumberOfProducts%941016331.body
}

m4return_t ClJSJobBuilder::CleanProductList ()
{
  //## begin ClJSJobBuilder::CleanProductList%941120253.body preserve=yes
	ProductList::iterator iterador ;/* Iterador para usar la lista            */

	iterador = m_lProducts.begin();

	while ( iterador != m_lProducts.end() )
	{
		delete (* iterador);
		iterador ++;
	}

	m_lProducts.clear();

	return M4_SUCCESS;
  //## end ClJSJobBuilder::CleanProductList%941120253.body
}

// Additional Declarations
  //## begin ClJSJobBuilder%3767ACF5032E.declarations preserve=yes
  //## end ClJSJobBuilder%3767ACF5032E.declarations

//## begin module%37E602150307.epilog preserve=yes
//## end module%37E602150307.epilog

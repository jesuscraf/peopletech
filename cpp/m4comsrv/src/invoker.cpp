//## begin module.cm preserve=no
//## end module.cm

//## begin module.cp preserve=no
//	=========================================================
//	=====================
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
//	=========================================================
//	=====================
//## end module.cp

//## Module: invoker; Package body
//## Subsystem: CPM4ComSrv::version
//## Source file: X:\M4REL\M4SOURCE\3x_work\Core\communication\working\oscar\M4ComSrv\version\src\Invoker.cpp

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
//## end module.includes

// invoker
#include "invoker.hpp"
//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes
//## end module.additionalDeclarations


// Class ClInvoker 




ClInvoker::ClInvoker ()
  //## begin ClInvoker::ClInvoker%-731737672.hasinit preserve=no
  //## end ClInvoker::ClInvoker%-731737672.hasinit
  //## begin ClInvoker::ClInvoker%-731737672.initialization preserve=yes
  //## end ClInvoker::ClInvoker%-731737672.initialization
{
  //## begin ClInvoker::ClInvoker%-731737672.body preserve=yes
	m_commands = NULL;
	m_manager = NULL;

	m_commands = new ClCommandTable();
  //## end ClInvoker::ClInvoker%-731737672.body
}

ClInvoker::ClInvoker (void* manager)
  //## begin ClInvoker::ClInvoker%1651197723.hasinit preserve=no
  //## end ClInvoker::ClInvoker%1651197723.hasinit
  //## begin ClInvoker::ClInvoker%1651197723.initialization preserve=yes
  //## end ClInvoker::ClInvoker%1651197723.initialization
{
  //## begin ClInvoker::ClInvoker%1651197723.body preserve=yes
	m_commands = NULL;

	m_manager = manager;

	m_commands = new ClCommandTable();
 //## end ClInvoker::ClInvoker%1651197723.body
}


ClInvoker::~ClInvoker ()
{
  //## begin ClInvoker::~ClInvoker%-1270919577.body preserve=yes

	delete ( m_commands ) ;

	m_commands = NULL;
  //## end ClInvoker::~ClInvoker%-1270919577.body
}



//## Other Operations (implementation)
m4return_t ClInvoker::CreateCommand (m4char_t ai_iCommand, void* ai_manager)
{
  //## begin ClInvoker::CreateCommand%-2145844697.body preserve=yes
	if ( ! ai_manager )
	{
		ai_manager = m_manager ;
	}
	return	m_commands -> CreateCommand( ai_manager, ai_iCommand );
  //## end ClInvoker::CreateCommand%-2145844697.body
}

m4return_t ClInvoker::DeleteCommand (m4char_t ai_iCommand)
{
  //## begin ClInvoker::DeleteCommand%-1978394898.body preserve=yes
	return m_commands -> DeleteCommand( ai_iCommand );
  //## end ClInvoker::DeleteCommand%-1978394898.body
}

m4return_t ClInvoker::SetManager (void* manager)
{
  //## begin ClInvoker::SetManager%495790292.body preserve=yes
	m_manager = manager;
	return M4_SUCCESS;
  //## end ClInvoker::SetManager%495790292.body
}

m4return_t ClInvoker::ExecuteCommand (m4char_t ai_iCommand, void *&ao_poReturn, ClCmdArguments *ai_Arguments)
{
  //## begin ClInvoker::ExecuteCommand%877335854.body preserve=yes
	M4ClString	header ;
	ClCommand	*command;


	
	if ( ! m_commands )
	{
		return M4_ERROR;
	}


//	arguments = new ClArguments (ai_Arguments);

	// tengo que buscar en el mapa y llamar a la función que tenga
	// asignada la cabezera, antes he debido rellenar este mapa.

	if ( !( command =  m_commands -> GetCommand( ai_iCommand )  ) )
	{
		return M4_ERROR;
	}

	if ( command  -> Execute( ao_poReturn ,  ai_Arguments ) == M4_ERROR )
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
  //## end ClInvoker::ExecuteCommand%877335854.body
}

// Additional Declarations
  //## begin ClInvoker.declarations preserve=yes
  //## end ClInvoker.declarations



//## begin module.epilog preserve=yes
//## end module.epilog

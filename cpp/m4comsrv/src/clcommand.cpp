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
//	====================
//## end module.cp

//## Module: Command; Package body
//## Subsystem: CPM4ComSrv::version
//## Source file: X:\m4rel\m4source\3x_work\core\communication\working\oscar\M4ComSrv\version\src\clcommand.cpp

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
//## end module.includes

// ClCmdArguments
#include <clcmdarguments.hpp>
// Command
#include <clcommand.hpp>
//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes
//## end module.additionalDeclarations


// Class ClCommand 




ClCommand::ClCommand ()
  //## begin ClCommand::ClCommand%918762194.hasinit preserve=no
  //## end ClCommand::ClCommand%918762194.hasinit
  //## begin ClCommand::ClCommand%918762194.initialization preserve=yes
  //## end ClCommand::ClCommand%918762194.initialization
{
  //## begin ClCommand::ClCommand%918762194.body preserve=yes
	m_manager = NULL;
  //## end ClCommand::ClCommand%918762194.body
}

ClCommand::ClCommand (void* manager)
  //## begin ClCommand::ClCommand%1493720844.hasinit preserve=no
  //## end ClCommand::ClCommand%1493720844.hasinit
  //## begin ClCommand::ClCommand%1493720844.initialization preserve=yes
  //## end ClCommand::ClCommand%1493720844.initialization
{
  //## begin ClCommand::ClCommand%1493720844.body preserve=yes
	m_manager = manager;
  //## end ClCommand::ClCommand%1493720844.body
}


ClCommand::~ClCommand ()
{
  //## begin ClCommand::~ClCommand%541022595.body preserve=yes
	m_manager = NULL;
  //## end ClCommand::~ClCommand%541022595.body
}



//## Other Operations (implementation)
m4int16_t ClCommand::SetManager (void* manager)
{
  //## begin ClCommand::SetManager%-1393546573.body preserve=yes
	if ( m_manager )
	{
//		SETCODEF( M4_SRV_LM_MANAGER_CHANGED , DEBUGINFOLOG , "Se ha cambiado la direccion a la que apunta m_manager en ClCommand" );

		m_manager = manager;

		return M4_WARNING;
	}
	m_manager = manager;

	return M4_SUCCESS;
  //## end ClCommand::SetManager%-1393546573.body
}

m4return_t ClCommand::Execute (void*& exit, ClCmdArguments *args)
{
  //## begin ClCommand::Execute%-1378648674.body preserve=yes
/*	if  ( ! m_manager )
	{
		exit = "The ThreadManager is not asigned";
		return M4_ERROR;
	}
*/
	return M4_SUCCESS;	
  //## end ClCommand::Execute%-1378648674.body
}

// Additional Declarations
  //## begin ClCommand.declarations preserve=yes
  //## end ClCommand.declarations



//## begin module.epilog preserve=yes
//## end module.epilog

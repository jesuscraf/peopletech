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

//## Module: cmdtable; Package body
//## Subsystem: CPM4ComSrv::version
//## Source file: X:\m4rel\m4source\3x_work\core\communication\working\oscar\M4ComSrv\version\src\cmdtable.cpp

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
#include <m4log.hpp>
#include <m4srvres.hpp>
#ifdef _WINDOWS
#include <windows.h>
#endif
//## end module.includes

// m4string
#include <m4string.hpp>
// ClAdminFuncs
#include <cladminfuncs.hpp>
// cmdtable
#include <cmdtable.hpp>
//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes
//## end module.additionalDeclarations


// Class ClCommandTable 



ClCommandTable::ClCommandTable ()
  //## begin ClCommandTable::ClCommandTable%1961923900.hasinit preserve=no
  //## end ClCommandTable::ClCommandTable%1961923900.hasinit
  //## begin ClCommandTable::ClCommandTable%1961923900.initialization preserve=yes
  //## end ClCommandTable::ClCommandTable%1961923900.initialization
{
  //## begin ClCommandTable::ClCommandTable%1961923900.body preserve=yes
  //## end ClCommandTable::ClCommandTable%1961923900.body
}


ClCommandTable::~ClCommandTable ()
{
  //## begin ClCommandTable::~ClCommandTable%1431135431.body preserve=yes
	COMMANDMAP::iterator iter;
	ClCommand *command;

	EnterWriter ( ) ;

	for( iter = m_commands.begin(); iter != m_commands.end(); iter ++ )
	{
		command = (*iter).second;

		delete command;

		command = NULL ;

	}

	m_commands.clear () ;

	LeaveWriter () ;
  //## end ClCommandTable::~ClCommandTable%1431135431.body
}



//## Other Operations (implementation)
m4return_t ClCommandTable::CreateCommand (void* manager, m4char_t ai_iCommand)
{
  //## begin ClCommandTable::CreateCommand%-363128453.body preserve=yes
	ClCommand	*command = NULL; 


	EnterWriter ( ) ;

	if ( m_commands.find( ai_iCommand ) != m_commands.end() )
	{
		SETCODEF ( M4_SRV_LM_COMMAND_ALLREADY_EXIST , ERRORLOG ,"The command %d allready exist in the commands table" , ai_iCommand ) ;

		LeaveWriter () ;

		return M4_ERROR;
	}

//////////////////////////////////////////////////
// Inicio de la parte configurable por el usuario
//////////////////////////////////////////////////
	switch ( ai_iCommand )
	{


	case CREATESERVICE:
		command = new ClCreateService( manager );
		break;


	case CREATECONNMANAGER:
		command = new ClCreateCM( manager );
		break;

	case CREATELAUNCHER:
		command = new ClCreateLauncher( manager );
		break;

	case CREATEADMINISTRATOR:
		command = new ClCreateAdministrator( manager );
		break;


	case SHUTDOWNSERVICE:
		command = new ClShutDownService( manager );
		break;


	case SHUTDOWNCONNMANAGER:
		command = new ClShutDownCM( manager );
		break;

	case SHUTDOWNLAUNCHER:
		command = new ClShutDownLauncher( manager );
		break;

	case SHUTDOWNADMINISTRATOR:
		command = new ClShutDownAdministrator ( manager );
		break;

	case NEWCONNECTION:
		command = new ClNewConnection( manager );
		break;

	case DOWNCONNECTION:
		command = new ClDownConnection( manager );
		break;

	}
  //////////////////////////////////////////////////
// Fin de la parte configurable por el usuario
//////////////////////////////////////////////////

	if ( ! command )
	{
		LeaveWriter () ;
		return M4_ERROR;
	}

	m_commands.insert( COMMANDMAP::value_type( ai_iCommand, command ) );

	LeaveWriter () ;


	return M4_SUCCESS;
  //## end ClCommandTable::CreateCommand%-363128453.body
}

m4return_t ClCommandTable::DeleteCommand (m4char_t ai_command)
{
  //## begin ClCommandTable::DeleteCommand%1567923947.body preserve=yes
	COMMANDMAP::iterator iter;

	EnterWriter ( );

	iter = m_commands.find( ai_command );

	if ( iter == m_commands.end() )
	{
		SETCODEF ( M4_SRV_LM_COMMAND_DOES_NOT_EXIST, ERRORLOG , "Trying to delete the non existing command %d" , ai_command);		
	
		LeaveWriter () ;

		return NULL;
	}

	delete (*iter).second;		

	m_commands.erase( iter );


	LeaveWriter ( );

	return M4_SUCCESS;
  //## end ClCommandTable::DeleteCommand%1567923947.body
}

ClCommand* ClCommandTable::GetCommand (m4char_t ai_iCommand)
{
  //## begin ClCommandTable::GetCommand%-351209901.body preserve=yes
	COMMANDMAP::iterator iter;
	ClCommand * poCommand ;

	EnterReader ( ) ;

	iter = m_commands.find( ai_iCommand );

	if ( iter == m_commands.end() )
	{
		SETCODEF ( M4_SRV_LM_NOT_DEFINED_COMMAND , ERRORLOG , "Trying to get a not defined command "  );				

		LeaveReader ( ) ;

		return NULL;
	}

	poCommand = (*iter).second ;

	LeaveReader () ;

	return	poCommand ;
  //## end ClCommandTable::GetCommand%-351209901.body
}

// Additional Declarations
  //## begin ClCommandTable.declarations preserve=yes
  //## end ClCommandTable.declarations





//## begin module.epilog preserve=yes
//## end module.epilog

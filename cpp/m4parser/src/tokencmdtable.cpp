//## begin module%3494023602CA.cm preserve=no
//## end module%3494023602CA.cm

//## begin module%3494023602CA.cp preserve=no
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
//## end module%3494023602CA.cp

//## Module: TokenCommandTable%3494023602CA; Package body
//## Subsystem: M4Parser::src%379D926A02BD
//## Source file: D:\FuentesServidor\M4Parser\version\src\tokencmdtable.cpp

//## begin module%3494023602CA.additionalIncludes preserve=no
//## end module%3494023602CA.additionalIncludes

//## begin module%3494023602CA.includes preserve=yes
//## end module%3494023602CA.includes

#include "m4stl.hpp"
// TokenCommandTable
#include <tokencmdtable.hpp>
// ParserDefs
#include <parserdefs.hpp>
// PairIdCommand
#include <pairidcommand.hpp>
//## begin module%3494023602CA.declarations preserve=no
//## end module%3494023602CA.declarations

//## begin module%3494023602CA.additionalDeclarations preserve=yes
//## end module%3494023602CA.additionalDeclarations


// Class ClTokenCommandTable 




//## Other Operations (implementation)
m4return_t ClTokenCommandTable::Add (ClPairIdCommand *ai_pCommand)
{
  //## begin ClTokenCommandTable::Add%882623311.body preserve=yes
	if ( Exists( ai_pCommand -> GetId() ) )
	{
		return M4_ERROR;
	}
	m_oCommands.push_back( ai_pCommand );
	return M4_SUCCESS;

  //## end ClTokenCommandTable::Add%882623311.body
}

ClPairIdCommand * ClTokenCommandTable::Get (m4command_t ai_pId)
{
  //## begin ClTokenCommandTable::Get%882623312.body preserve=yes
	CLTOKENCOMMANDTABLE::iterator	iter;
	ClPairIdCommand	*pCommand;

	for ( iter = m_oCommands.begin()
			; iter != m_oCommands.end()
			; iter ++ )
	{
		pCommand =  * iter;

		if ( strcmp( ( char * ) pCommand -> GetId(), ai_pId ) == 0 )
		{
			return pCommand;
		}
	}

	return NULL;
  //## end ClTokenCommandTable::Get%882623312.body
}

void ClTokenCommandTable::Quit (m4command_t ai_pId)
{
  //## begin ClTokenCommandTable::Quit%882623313.body preserve=yes
	CLTOKENCOMMANDTABLE::iterator	iter;
	ClPairIdCommand	*pCommand;

	for ( iter = m_oCommands.begin()
			; iter != m_oCommands.end()
			; iter ++ )
	{
		pCommand = * iter;

		if ( strcmp( pCommand -> GetId(), ai_pId ) == 0 )
		{
			m_oCommands.erase( iter );
			delete	pCommand;
			break;
		}
	}
  //## end ClTokenCommandTable::Quit%882623313.body
}

void ClTokenCommandTable::Clear ()
{
  //## begin ClTokenCommandTable::Clear%882623314.body preserve=yes
	CLTOKENCOMMANDTABLE::iterator	iter;
	ClPairIdCommand	*pCommand;

	for ( iter = m_oCommands.begin()
			; iter != m_oCommands.end()
			; iter ++ )
	{
		pCommand = * iter;
		if ( pCommand != NULL )
			delete pCommand;
	}

	m_oCommands.clear();

  //## end ClTokenCommandTable::Clear%882623314.body
}

m4bool_t ClTokenCommandTable::Exists (m4command_t ai_pId)
{
  //## begin ClTokenCommandTable::Exists%882623315.body preserve=yes
	CLTOKENCOMMANDTABLE::iterator	iter;
	ClPairIdCommand	*pCommand;

	for ( iter = m_oCommands.begin()
			; iter != m_oCommands.end()
			; iter ++ )
	{
		pCommand = * iter;

		if ( strcmp( pCommand -> GetId(), ai_pId ) == 0 )
		{
			return M4_TRUE;
		}
	}

	return M4_FALSE;


  //## end ClTokenCommandTable::Exists%882623315.body
}

//## begin module%3494023602CA.epilog preserve=yes
//## end module%3494023602CA.epilog

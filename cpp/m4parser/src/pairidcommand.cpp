//## begin module%349BE75C0113.cm preserve=no
//## end module%349BE75C0113.cm

//## begin module%349BE75C0113.cp preserve=no
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
//## end module%349BE75C0113.cp

//## Module: PairIdCommand%349BE75C0113; Package body
//## Subsystem: M4Parser::src%379D926A02BD
//## Source file: D:\FuentesServidor\M4Parser\version\src\pairidcommand.cpp

//## begin module%349BE75C0113.additionalIncludes preserve=no
//## end module%349BE75C0113.additionalIncludes

//## begin module%349BE75C0113.includes preserve=yes
//## end module%349BE75C0113.includes

// ParserDefs
#include <parserdefs.hpp>
// PairIdCommand
#include <pairidcommand.hpp>
// ClCommand
#include <clcommand.hpp>
//## begin module%349BE75C0113.declarations preserve=no
//## end module%349BE75C0113.declarations

//## begin module%349BE75C0113.additionalDeclarations preserve=yes
//## end module%349BE75C0113.additionalDeclarations


// Class ClPairIdCommand 


ClPairIdCommand::ClPairIdCommand (m4command_t ai_pCommandId, void *ai_pManager)
  //## begin ClPairIdCommand::ClPairIdCommand%882623309.hasinit preserve=no
  //## end ClPairIdCommand::ClPairIdCommand%882623309.hasinit
  //## begin ClPairIdCommand::ClPairIdCommand%882623309.initialization preserve=yes
  //## end ClPairIdCommand::ClPairIdCommand%882623309.initialization
{
  //## begin ClPairIdCommand::ClPairIdCommand%882623309.body preserve=yes
//	strcpy( m_pCommandId, ai_pCommandId );
	m_pCommandId = ai_pCommandId;
	m_manager = ai_pManager;
  //## end ClPairIdCommand::ClPairIdCommand%882623309.body
}


ClPairIdCommand::~ClPairIdCommand ()
{
  //## begin ClPairIdCommand::~ClPairIdCommand%884190881.body preserve=yes
	if ( m_pCommandId != NULL )
	{
		delete [] m_pCommandId;
	}
  //## end ClPairIdCommand::~ClPairIdCommand%884190881.body
}



//## Other Operations (implementation)
m4command_t ClPairIdCommand::GetId ()
{
  //## begin ClPairIdCommand::GetId%882623310.body preserve=yes
	return m_pCommandId;
  //## end ClPairIdCommand::GetId%882623310.body
}

m4return_t ClPairIdCommand::Execute (m4parsercommandargs_t ai_pInput, M4DataStorage *aio_pIOputData, ClStatistic *aio_poStats)
{
  //## begin ClPairIdCommand::Execute%889434846.body preserve=yes
	
	return M4_SUCCESS;
  //## end ClPairIdCommand::Execute%889434846.body
}

m4return_t ClPairIdCommand::Execute (m4parsercommandargs_t ai_pInput, M4DataStorage *aio_pIOputData)
{
  //## begin ClPairIdCommand::Execute%899143378.body preserve=yes
	return M4_SUCCESS;
  //## end ClPairIdCommand::Execute%899143378.body
}

m4return_t ClPairIdCommand::Execute (void*& exit, ClCmdArguments *args)
{
  //## begin ClPairIdCommand::Execute%933074743.body preserve=yes
	return M4_SUCCESS;
  //## end ClPairIdCommand::Execute%933074743.body
}

//## begin module%349BE75C0113.epilog preserve=yes
//## end module%349BE75C0113.epilog

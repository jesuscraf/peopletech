//## begin module%34929EA70367.cm preserve=no
//## end module%34929EA70367.cm

//## begin module%34929EA70367.cp preserve=no
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
//## end module%34929EA70367.cp

//## Module: ConnBase%34929EA70367; Package body
//## Subsystem: M4ServerBase::src%379431BE02D0
//## Source file: F:\integration\m4serverbase\src\connbase.cpp

//## begin module%34929EA70367.additionalIncludes preserve=no
//## end module%34929EA70367.additionalIncludes

//## begin module%34929EA70367.includes preserve=yes
#include <stdlib.h>
#include <tsap.hpp>
//## end module%34929EA70367.includes

// ConnBase
#include <connbase.hpp>
// conninstance
#include <conninstance.hpp>
//## begin module%34929EA70367.declarations preserve=no
//## end module%34929EA70367.declarations

//## begin module%34929EA70367.additionalDeclarations preserve=yes
//## end module%34929EA70367.additionalDeclarations


// Class ClConnBase 



ClConnBase::ClConnBase (m4objid_t ai_lIdConnection, ClTSAP *ai_pTSAP)
  //## begin ClConnBase::ClConnBase%882023677.hasinit preserve=no
  //## end ClConnBase::ClConnBase%882023677.hasinit
  //## begin ClConnBase::ClConnBase%882023677.initialization preserve=yes
  :ClConnInstance ( ai_lIdConnection , M4_CONNECTION_ORIENTED_PROTOCOL)
  //## end ClConnBase::ClConnBase%882023677.initialization
{
  //## begin ClConnBase::ClConnBase%882023677.body preserve=yes
	m_poTSAP = ai_pTSAP ;

  //## end ClConnBase::ClConnBase%882023677.body
}

ClConnBase::ClConnBase (m4uint32_t ai_lIdConnection)
  //## begin ClConnBase::ClConnBase%882023680.hasinit preserve=no
  //## end ClConnBase::ClConnBase%882023680.hasinit
  //## begin ClConnBase::ClConnBase%882023680.initialization preserve=yes
  :ClConnInstance ( ai_lIdConnection , M4_CONNECTION_ORIENTED_PROTOCOL)
  //## end ClConnBase::ClConnBase%882023680.initialization
{
  //## begin ClConnBase::ClConnBase%882023680.body preserve=yes
	m_poTSAP = NULL ;
  //## end ClConnBase::ClConnBase%882023680.body
}

ClConnBase::ClConnBase (ClConnInstance *ai_poConnInst)
:ClConnInstance(ai_poConnInst)
{
	m_poTSAP = NULL;
}

ClConnBase::~ClConnBase ()
{
  //## begin ClConnBase::~ClConnBase%902904500.body preserve=yes
	if ( m_poTSAP )
	{
		m_poTSAP -> Close () ;

		delete m_poTSAP ;

		m_poTSAP = NULL ;
	}
  //## end ClConnBase::~ClConnBase%902904500.body
}



//## Other Operations (implementation)
ClProtocol ClConnBase::GetProtocol ()
{
  //## begin ClConnBase::GetProtocol%884258328.body preserve=yes
	return m_oProtocol;
  //## end ClConnBase::GetProtocol%884258328.body
}

// Additional Declarations
  //## begin ClConnBase%34929E7900FE.declarations preserve=yes
  //## end ClConnBase%34929E7900FE.declarations

//## begin module%34929EA70367.epilog preserve=yes
//## end module%34929EA70367.epilog

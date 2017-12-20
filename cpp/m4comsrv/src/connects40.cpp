//## begin module%3993A3B1033E.cm preserve=no
//## end module%3993A3B1033E.cm

//## begin module%3993A3B1033E.cp preserve=no
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
//## end module%3993A3B1033E.cp

//## Module: connects40%3993A3B1033E; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: F:\integration\m4comsrv\src\connects40.cpp

//## begin module%3993A3B1033E.additionalIncludes preserve=no
//## end module%3993A3B1033E.additionalIncludes

//## begin module%3993A3B1033E.includes preserve=yes
#include <basepdus.hpp>
//## end module%3993A3B1033E.includes

// conninstance
#include <conninstance.hpp>
// connects40
#include <connects40.hpp>

class ClConnection;

//## begin module%3993A3B1033E.declarations preserve=no
//## end module%3993A3B1033E.declarations

//## begin module%3993A3B1033E.additionalDeclarations preserve=yes
//## end module%3993A3B1033E.additionalDeclarations


// Class ClConnection40 

ClConnection40::ClConnection40 (m4uint32_t ai_lID)
  //## begin ClConnection40::ClConnection40%965977089.hasinit preserve=no
  //## end ClConnection40::ClConnection40%965977089.hasinit
  //## begin ClConnection40::ClConnection40%965977089.initialization preserve=yes
  :	ClConnInstance ( ai_lID , M4_NON_CONNECTION_ORIENTED_PROTOCOL ) 
  //## end ClConnection40::ClConnection40%965977089.initialization
{
  //## begin ClConnection40::ClConnection40%965977089.body preserve=yes
	m_oNumRequest.SetValue ( 0 ) ;
	m_oProtocol.SetVersion ( M4_PDU_40_VERSION ); 
  //## end ClConnection40::ClConnection40%965977089.body
}

ClConnection40::ClConnection40 ()
  //## begin ClConnection40::ClConnection40%965977090.hasinit preserve=no
  //## end ClConnection40::ClConnection40%965977090.hasinit
  //## begin ClConnection40::ClConnection40%965977090.initialization preserve=yes
  :	ClConnInstance ( 0 , M4_NON_CONNECTION_ORIENTED_PROTOCOL )
  //## end ClConnection40::ClConnection40%965977090.initialization
{
  //## begin ClConnection40::ClConnection40%965977090.body preserve=yes
	m_oProtocol.SetVersion ( M4_PDU_40_VERSION ); 
  //## end ClConnection40::ClConnection40%965977090.body
}

ClConnection40::ClConnection40 (ClConnection *ai_poConnection)
  //## begin ClConnection40::ClConnection40%967450713.hasinit preserve=no
  //## end ClConnection40::ClConnection40%967450713.hasinit
  //## begin ClConnection40::ClConnection40%967450713.initialization preserve=yes
  :	ClConnInstance ( (ClConnInstance*)ai_poConnection)
  //## end ClConnection40::ClConnection40%967450713.initialization
{
  //## begin ClConnection40::ClConnection40%967450713.body preserve=yes
	SetConnProtocol ( M4_NON_CONNECTION_ORIENTED_PROTOCOL ) ;
  //## end ClConnection40::ClConnection40%967450713.body
}



//## Other Operations (implementation)
m4bool_t ClConnection40::CanDestroy ()
{
  //## begin ClConnection40::CanDestroy%965977076.body preserve=yes
	return M4_TRUE ;
  //## end ClConnection40::CanDestroy%965977076.body
}

m4bool_t ClConnection40::CanRecycle ()
{
  //## begin ClConnection40::CanRecycle%965977077.body preserve=yes
	return M4_TRUE ;
  //## end ClConnection40::CanRecycle%965977077.body
}

m4return_t ClConnection40::Destroy ()
{
  //## begin ClConnection40::Destroy%965977078.body preserve=yes
	delete this ;

	return M4_SUCCESS ;
  //## end ClConnection40::Destroy%965977078.body
}

m4return_t ClConnection40::Recycle ()
{
  //## begin ClConnection40::Recycle%965977081.body preserve=yes
	m_oNumRequest.SetValue ( 0 ) ;
	m_oStatus.SetValue (  0 ) ;

	return M4_SUCCESS ;
  //## end ClConnection40::Recycle%965977081.body
}

m4return_t ClConnection40::SetStatus (m4uint32_t ai_iValue)
{
  //## begin ClConnection40::SetStatus%967450719.body preserve=yes
	m_oStatus.SetValue ( (m4int_t) ai_iValue ) ;

	return M4_SUCCESS ;
  //## end ClConnection40::SetStatus%967450719.body
}

// Additional Declarations
  //## begin ClConnection40%3992D587023E.declarations preserve=yes
ClConnection40::~ClConnection40 ()
{
}
  //## end ClConnection40%3992D587023E.declarations

//## begin module%3993A3B1033E.epilog preserve=yes
//## end module%3993A3B1033E.epilog

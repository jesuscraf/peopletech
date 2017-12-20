//## begin module%3992D03700C6.cm preserve=no
//## end module%3992D03700C6.cm

//## begin module%3992D03700C6.cp preserve=no
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
//## end module%3992D03700C6.cp

//## Module: conninstance%3992D03700C6; Package body
//## Subsystem: M4ServerBase::src%379431BE02D0
//## Source file: F:\integration\m4serverbase\src\conninstance.cpp

//## begin module%3992D03700C6.additionalIncludes preserve=no
//## end module%3992D03700C6.additionalIncludes

//## begin module%3992D03700C6.includes preserve=yes
//## end module%3992D03700C6.includes

// Protocol
#include <protocol.hpp>
// clbaseobject
#include <clbaseobject.hpp>
// ExVariables
#include <exvariables.hpp>
// conninstance
#include <conninstance.hpp>
//## begin module%3992D03700C6.declarations preserve=no
//## end module%3992D03700C6.declarations

//## begin module%3992D03700C6.additionalDeclarations preserve=yes
#include <m4compress.hpp>
//## end module%3992D03700C6.additionalDeclarations


// Class ClConnInstance 











ClConnInstance::ClConnInstance (m4objid_t ai_lIdConnection, eConnProtocol ai_lConnProtocol)
  //## begin ClConnInstance::ClConnInstance%967113588.hasinit preserve=no
  //## end ClConnInstance::ClConnInstance%967113588.hasinit
  //## begin ClConnInstance::ClConnInstance%967113588.initialization preserve=yes
  : m_lConnProtocol ( ai_lConnProtocol ) , m_oStatus ((m4uint32_t)ai_lIdConnection)
  //## end ClConnInstance::ClConnInstance%967113588.initialization
{
  //## begin ClConnInstance::ClConnInstance%967113588.body preserve=yes
	m_lCompressionType=M4_COMPRESSION_TYPE_NULL;
	m_lCompressionLevel=M4_COMPRESSION_CLASSIC_DEFAULT_LEVEL;

	m_oNumRequest.SetValue (0) ;
	m_lClientType = M4_PROTOCOL ;
	m_oStatus.SetValue (PENDING) ;
	SetId ( ai_lIdConnection ) ;

  //## end ClConnInstance::ClConnInstance%967113588.body
}

ClConnInstance::ClConnInstance (ClConnInstance *ai_poConnection)
  //## begin ClConnInstance::ClConnInstance%967450714.hasinit preserve=no
  //## end ClConnInstance::ClConnInstance%967450714.hasinit
  //## begin ClConnInstance::ClConnInstance%967450714.initialization preserve=yes
  :m_oStatus ((m4uint32_t)ai_poConnection ->GetId())
  //## end ClConnInstance::ClConnInstance%967450714.initialization
{
  //## begin ClConnInstance::ClConnInstance%967450714.body preserve=yes
//	m_lConnProtocol = ai_poConnection ->GetConnProtocol ( ) ;
	m_oNumRequest.SetValue ( ai_poConnection ->m_oNumRequest.GetValue()) ;
	m_lClientType = ai_poConnection ->GetClientType () ;
	m_oStatus.SetValue (ai_poConnection ->m_oStatus.GetValue() ) ;
	SetId ( ai_poConnection ->GetId() ) ;
	m_lCompressionType=ai_poConnection ->GetCompressionType( ) ;
	m_lCompressionLevel=ai_poConnection ->GetCompressionlevel() ;
	m_lConnProtocol = M4_NON_CONNECTION_ORIENTED_PROTOCOL ; 
	SetHost ( ai_poConnection ->GetHost ( ) ) ;

  //## end ClConnInstance::ClConnInstance%967450714.body
}


ClConnInstance::~ClConnInstance ()
{
  //## begin ClConnInstance::~ClConnInstance%967113589.body preserve=yes
  //## end ClConnInstance::~ClConnInstance%967113589.body
}



//## Other Operations (implementation)
eConnProtocol ClConnInstance::GetConnProtocol ()
{
  //## begin ClConnInstance::GetConnProtocol%967113594.body preserve=yes
	return m_lConnProtocol ;
  //## end ClConnInstance::GetConnProtocol%967113594.body
}

void ClConnInstance::SetConnProtocol (eConnProtocol ai_lConnProtocol)
{
  //## begin ClConnInstance::SetConnProtocol%967450715.body preserve=yes
	m_lConnProtocol = ai_lConnProtocol ;
  //## end ClConnInstance::SetConnProtocol%967450715.body
}

void ClConnInstance::SetHost (m4string_t ai_szHostName)
{
  //## begin ClConnInstance::SetHost%967450709.body preserve=yes
	m_szHost = ai_szHostName ;
  //## end ClConnInstance::SetHost%967450709.body
}

m4pcchar_t ClConnInstance::GetHost ()
{
  //## begin ClConnInstance::GetHost%967450710.body preserve=yes
		return  m_szHost.c_str() ;
  //## end ClConnInstance::GetHost%967450710.body
}

void ClConnInstance::SetClientType (m4uint32_t ai_lClientType)
{
  //## begin ClConnInstance::SetClientType%967450711.body preserve=yes
	m_lClientType = ai_lClientType ;
  //## end ClConnInstance::SetClientType%967450711.body
}

m4uint32_t ClConnInstance::GetClientType ()
{
  //## begin ClConnInstance::GetClientType%967450712.body preserve=yes
	return m_lClientType ;
  //## end ClConnInstance::GetClientType%967450712.body
}

m4return_t ClConnInstance::SetStatus (m4uint32_t ai_iValue)
{
  //## begin ClConnInstance::SetStatus%967450720.body preserve=yes
	m_oStatus.SetValue ( (m4int16_t)ai_iValue ) ;

	return M4_SUCCESS ;
  //## end ClConnInstance::SetStatus%967450720.body
}

m4uint32_t ClConnInstance::GetStatus ()
{
  //## begin ClConnInstance::GetStatus%967450721.body preserve=yes
	return m_oStatus.GetValue () ;
  //## end ClConnInstance::GetStatus%967450721.body
}

void ClConnInstance::SetIP (m4string_t ai_szIP)
{
  //## begin ClConnInstance::SetIP%1003855114.body preserve=yes
	m_szIP = ai_szIP ;
  //## end ClConnInstance::SetIP%1003855114.body
}

m4pcchar_t ClConnInstance::GetIP ()
{
  //## begin ClConnInstance::GetIP%1003855115.body preserve=yes
	return m_szIP.c_str () ;
  //## end ClConnInstance::GetIP%1003855115.body
}

// Additional Declarations
  //## begin ClConnInstance%39A5365C034C.declarations preserve=yes
  //## end ClConnInstance%39A5365C034C.declarations

//## begin module%3992D03700C6.epilog preserve=yes
//## end module%3992D03700C6.epilog

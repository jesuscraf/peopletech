//## begin module%342BCA2B02AE.cm preserve=no
//## end module%342BCA2B02AE.cm

//## begin module%342BCA2B02AE.cp preserve=no
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
//## end module%342BCA2B02AE.cp

//## Module: Connects%342BCA2B02AE; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: F:\integration\m4comsrv\src\connects.cpp

//## begin module%342BCA2B02AE.additionalIncludes preserve=no
//## end module%342BCA2B02AE.additionalIncludes

//## begin module%342BCA2B02AE.includes preserve=yes
#include <m4types.hpp>
#include <m4win.hpp>
#include <m4compress.hpp>
#include <tsap.hpp>
#include <basepdus.hpp>
#include <usinterface.hpp>
#include <connbase.hpp>
#include <statistics.hpp>
#include <connects.hpp>

class ClUSSessionInterface;

//## begin module%342BCA2B02AE.declarations preserve=no
//## end module%342BCA2B02AE.declarations

//## begin module%342BCA2B02AE.additionalDeclarations preserve=yes
//## end module%342BCA2B02AE.additionalDeclarations


// Class ClConnection 







ClConnection::ClConnection (m4objid_t ai_lIdConnection)
  //## begin ClConnection::ClConnection%876138363.hasinit preserve=no
      : m_iSessId(0), m_iUserId(0)
  //## end ClConnection::ClConnection%876138363.hasinit
  //## begin ClConnection::ClConnection%876138363.initialization preserve=yes
  ,ClConnBase ( ai_lIdConnection )
  //## end ClConnection::ClConnection%876138363.initialization
{
  //## begin ClConnection::ClConnection%876138363.body preserve=yes
	m_lId = 0;
	m_pMainSession = NULL ;
	m_oNumRequest.SetValue (0) ;
	m_oProtocol.SetVersion ( M4_PDU_40_VERSION ); 
  //## end ClConnection::ClConnection%876138363.body
}

ClConnection::ClConnection (m4uint32_t ai_lIdConnection, ClTSAP *ai_pTSAP)
  //## begin ClConnection::ClConnection%882023676.hasinit preserve=no
      : m_iSessId(0), m_iUserId(0)
  //## end ClConnection::ClConnection%882023676.hasinit
  //## begin ClConnection::ClConnection%882023676.initialization preserve=yes
  , ClConnBase ( ai_lIdConnection , ai_pTSAP ) 
  //## end ClConnection::ClConnection%882023676.initialization
{
  //## begin ClConnection::ClConnection%882023676.body preserve=yes
	m_pMainSession = NULL ;
	m_oProtocol.SetVersion ( M4_PDU_40_VERSION ) ;
  //## end ClConnection::ClConnection%882023676.body
}

ClConnection::ClConnection (ClConnection40 *ai_poConn40)
: ClConnBase ((ClConnInstance*)ai_poConn40)
{
	SetConnProtocol (M4_CONNECTION_ORIENTED_PROTOCOL);
	m_pMainSession = NULL;
}

ClConnection::~ClConnection ()
{
  //## begin ClConnection::~ClConnection%902904499.body preserve=yes
  //## end ClConnection::~ClConnection%902904499.body
}



//## Other Operations (implementation)
void ClConnection::Accept (ClConnectionVisitor &ai_oVisitor)
{
  //## begin ClConnection::Accept%878029381.body preserve=yes
	ai_oVisitor.VisitConnection ( *this ) ;
  //## end ClConnection::Accept%878029381.body
}

ClUSSessionInterface * ClConnection::GetMainSession ()
{
  //## begin ClConnection::GetMainSession%893147831.body preserve=yes
	return m_pMainSession ;
  //## end ClConnection::GetMainSession%893147831.body
}

void ClConnection::SetMainSesion (ClUSSessionInterface *ai_poSession)
{
  //## begin ClConnection::SetMainSesion%893147832.body preserve=yes
	ClUSUserInterface * poUser ;

	m_pMainSession = ai_poSession ;
	m_iSessId = ai_poSession -> GetId () ;
	
	if ( poUser = ai_poSession ->GetUser ( ) )
	{
		m_iUserId = poUser -> GetId () ;
	}
  //## end ClConnection::SetMainSesion%893147832.body
}

m4return_t ClConnection::Recycle ()
{
  //## begin ClConnection::Recycle%941721547.body preserve=yes
	if ( m_poTSAP )
	{
		delete m_poTSAP ;
		m_poTSAP = NULL ;
	}

	SetId (0 ) ;

	
	m_pMainSession = NULL ;
	m_oNumRequest.SetValue (0) ;
	SetCompressionType ( M4_COMPRESSION_TYPE_NULL );
	SetCompressionLevel ( M4_COMPRESSION_CLASSIC_DEFAULT_LEVEL );
	m_oStatus.SetValue (PENDING) ;

	return M4_TRUE ;
  //## end ClConnection::Recycle%941721547.body
}

m4bool_t ClConnection::CanRecycle ()
{
  //## begin ClConnection::CanRecycle%941721548.body preserve=yes
	return ( ((m_pMainSession==NULL)&&(m_oNumRequest.GetValue()==0))?M4_TRUE:M4_FALSE ) ;
  //## end ClConnection::CanRecycle%941721548.body
}

m4return_t ClConnection::Destroy ()
{
  //## begin ClConnection::Destroy%941721549.body preserve=yes
	delete this;
	return M4_SUCCESS ;
  //## end ClConnection::Destroy%941721549.body
}

m4bool_t ClConnection::CanDestroy ()
{
  //## begin ClConnection::CanDestroy%941721550.body preserve=yes
	return ( ((m_pMainSession==NULL)&&(m_oNumRequest.GetValue()==0))?M4_TRUE:M4_FALSE ) ;
  //## end ClConnection::CanDestroy%941721550.body
}

m4return_t ClConnection::SetStatus (m4uint32_t ai_iValue)
{
  //## begin ClConnection::SetStatus%967450718.body preserve=yes
	m4uint32_t iOldValue ;

	iOldValue = m_oStatus.GetValue () ;

	if ( (( iOldValue != BROKEN )&&( iOldValue != BROKEN )) || (  ai_iValue == CLOSED ) )
	{
		m_oStatus.SetValue ( (m4int8_t) ai_iValue ) ;
	}

	switch ( ai_iValue )
	{
	case ACTIVE :
		if ( iOldValue != PENDING )
		{
			m_oStatus.SetValue ( (m4int8_t) iOldValue ) ;
			return M4_ERROR ;
		}
		break ;


	}

	return M4_SUCCESS ;
  //## end ClConnection::SetStatus%967450718.body
}

m4objid_t ClConnection::GetSessId ()
{
  //## begin ClConnection::GetSessId%1010489723.body preserve=yes
	return m_iSessId ;
  //## end ClConnection::GetSessId%1010489723.body
}

m4objid_t ClConnection::GetUserId ()
{
  //## begin ClConnection::GetUserId%1010489724.body preserve=yes
	return m_iUserId ;
  //## end ClConnection::GetUserId%1010489724.body
}

// Additional Declarations
  //## begin ClConnection%342240440006.declarations preserve=yes
  //## end ClConnection%342240440006.declarations

// Class ClConnectionVisitor 


//## Other Operations (implementation)
void ClConnectionVisitor::VisitConnection (ClConnection &ai_oConnection)
{
  //## begin ClConnectionVisitor::VisitConnection%878029380.body preserve=yes
  //## end ClConnectionVisitor::VisitConnection%878029380.body
}

// Additional Declarations
  //## begin ClConnectionVisitor%344F5427004D.declarations preserve=yes
  //## end ClConnectionVisitor%344F5427004D.declarations

//## begin module%342BCA2B02AE.epilog preserve=yes
//## end module%342BCA2B02AE.epilog

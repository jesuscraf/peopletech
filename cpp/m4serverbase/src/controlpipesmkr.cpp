//## begin module%37E678470284.cm preserve=no
//## end module%37E678470284.cm

//## begin module%37E678470284.cp preserve=no
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
//## end module%37E678470284.cp

//## Module: controlpipesmkr%37E678470284; Package body
//## Subsystem: M4ServerBase::src%379431BE02D0
//## Source file: F:\integration\m4serverbase\src\controlpipesmkr.cpp

//## begin module%37E678470284.additionalIncludes preserve=no
//## end module%37E678470284.additionalIncludes

//## begin module%37E678470284.includes preserve=yes
#include <gettsap.hpp>
#include <m4log.hpp>
#include <m4srvres.hpp>
#include <m4trace.hpp>
#include <basiclog.hpp>
#include <m4srvtrace.h>
#include <tsap.hpp>
#include <controlpipesmkr.hpp>

#define M4_SRV_TRACE_COMP_ID "SRVB"

//## end module%37E678470284.includes

//## begin module%37E678470284.declarations preserve=no
//## end module%37E678470284.declarations

//## begin module%37E678470284.additionalDeclarations preserve=yes
//## end module%37E678470284.additionalDeclarations


// Class ClControlPipesMaker 

//## begin ClControlPipesMaker::m_iPort%37E6770102CB.attr preserve=no  private: static m4uint32_t {UA} 
m4uint32_t ClControlPipesMaker::m_iPort;
//## end ClControlPipesMaker::m_iPort%37E6770102CB.attr

//## begin ClControlPipesMaker::m_poControlMaker%37E675B001FE.role preserve=no  protected: static ClControlPipesMaker { -> RHAN}
ClControlPipesMaker *ClControlPipesMaker::m_poControlMaker = NULL;
//## end ClControlPipesMaker::m_poControlMaker%37E675B001FE.role


//## begin ClControlPipesMaker::m_poServer%37E67728017D.role preserve=no  protected: static ClServerTSAP { -> RHAN}
ClServerTSAP *ClControlPipesMaker::m_poServer;
//## end ClControlPipesMaker::m_poServer%37E67728017D.role

ClControlPipesMaker::ClControlPipesMaker ()
  //## begin ClControlPipesMaker::ClControlPipesMaker%937841143.hasinit preserve=no
  //## end ClControlPipesMaker::ClControlPipesMaker%937841143.hasinit
  //## begin ClControlPipesMaker::ClControlPipesMaker%937841143.initialization preserve=yes
  //## end ClControlPipesMaker::ClControlPipesMaker%937841143.initialization
{
  //## begin ClControlPipesMaker::ClControlPipesMaker%937841143.body preserve=yes
  //## end ClControlPipesMaker::ClControlPipesMaker%937841143.body
}



//## Other Operations (implementation)
m4return_t ClControlPipesMaker::GetPipes (ClTSAP *&ao_poControlA, ClTSAP *&ao_poControlB)
{
  //## begin ClControlPipesMaker::GetPipes%937841140.body preserve=yes
	ClGetTSAP * poGetTSAP = NULL;
	static m4bool_t bInitialized = M4_FALSE;
	m4uint_t iteraciones = 0;



	if (!m_poServer )
		return M4_ERROR ;

	poGetTSAP = new ClGetTSAP (  m_iPort ) ;



	do {
		poGetTSAP -> Start ( "ClGetTSAP-GetPipes" ) ;


		//do the accept to get the accepted socket
		// Bug 0130800
		ao_poControlA = m_poServer -> AcceptControl (bInitialized ) ;

		((ClControlReadTSAP*)ao_poControlA)->SetWriteTSAP(poGetTSAP->GetTSAP());

		bInitialized = M4_TRUE;

		iteraciones ++ ;

	} while ( ( iteraciones<4) && ( ! ao_poControlA) ) ;

	if  ( ! ao_poControlA )
	{
		M4_SRV_ERROR( M4_SRV_TSAP_ERROR , "Accept call on port %0:s returned NULL" , m_iPort ) ;

		return M4_ERROR ;
	}

//	m_poServer -> Close ( ) ;


	//Get the other socket side
	ao_poControlB = poGetTSAP -> GetTSAP ( ) ;

	if ( ! ao_poControlB )
	{
		return M4_ERROR ;
	}

	while ( ! poGetTSAP -> IsExit ( ) ) 
		m4sleep ( 1 ) ;

	delete poGetTSAP ;

	poGetTSAP = NULL ;

	return M4_SUCCESS ;
  //## end ClControlPipesMaker::GetPipes%937841140.body
}

m4return_t ClControlPipesMaker::Init (m4uint32_t ai_iPort)
{
  //## begin ClControlPipesMaker::Init%937841142.body preserve=yes

	static m4bool_t bInitialized = M4_FALSE ;

	if ( bInitialized )
		return M4_SUCCESS ;


	m_iPort = ai_iPort ;

// bg 150028
#ifdef _WIN32
	ClInetAddress	oLocalHostAddress( "localhost" ) ;

	if ( ! ( m_poServer = new ClServerTSAP ( &oLocalHostAddress, (m4uint16_t) m_iPort ) ) )
#else
	if ( ! ( m_poServer = new ClServerTSAP ( (m4uint16_t) m_iPort ) ) )
#endif

	{
		M4_SRV_ERROR( M4_SRV_TSAP_ERROR , "ClServerTSAP constructor failed on port %0:s" , m_iPort ) ;

		return M4_ERROR ;
	}

	//Si ha habido erores enel bindado se aborta la ejecución
	if ( m_poServer -> Error ( ) )
	{
		M4_SRV_WARNING( M4_SRV_TSAP_ERROR , "Port %0:s: not available at the moment" , m_iPort ) ;

		delete m_poServer ;

		m_poServer = NULL ;

		return M4_ERROR ;
	}

	bInitialized = M4_TRUE ;

	return M4_SUCCESS ;
  //## end ClControlPipesMaker::Init%937841142.body
}

m4return_t ClControlPipesMaker::Close ()
{
  //## begin ClControlPipesMaker::Close%947499514.body preserve=yes
	if ( ! m_poServer )
		return M4_ERROR ;

	m_poServer -> Close () ;

	if ( m_poServer -> Error () )
		return M4_ERROR ;

	delete m_poServer ;

	m_poServer = NULL ;


	return M4_SUCCESS ;
  //## end ClControlPipesMaker::Close%947499514.body
}

// Additional Declarations
  //## begin ClControlPipesMaker%37E6757A021F.declarations preserve=yes
  //## end ClControlPipesMaker%37E6757A021F.declarations

//## begin module%37E678470284.epilog preserve=yes
//## end module%37E678470284.epilog

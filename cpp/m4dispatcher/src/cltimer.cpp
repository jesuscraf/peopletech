//## begin module%396AE24E03A4.cm preserve=no
//## end module%396AE24E03A4.cm

//## begin module%396AE24E03A4.cp preserve=no
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
//## end module%396AE24E03A4.cp

//## Module: cltimer%396AE24E03A4; Package body
//## Subsystem: M4Dispatcher::src%3923BBFB0332
//## Source file: C:\WorkingFolder\m4dispatcher\src\cltimer.cpp

//## begin module%396AE24E03A4.additionalIncludes preserve=no
//## end module%396AE24E03A4.additionalIncludes

//## begin module%396AE24E03A4.includes preserve=yes
#include <m4date.hpp>

#include <m4memdbg.h>
#define M4_SRV_TRACE_COMP_ID "DPCH"

//## end module%396AE24E03A4.includes

// cltimer
#include <cltimer.hpp>
// m4thread
#include <m4thread.hpp>
// iexecutable
#include <iexecutable.hpp>
//## begin module%396AE24E03A4.declarations preserve=no
//## end module%396AE24E03A4.declarations

//## begin module%396AE24E03A4.additionalDeclarations preserve=yes
//## end module%396AE24E03A4.additionalDeclarations


// Class ClTimer 





ClTimer::ClTimer (ClExecutableInterface *ai_poExecutive, m4uint32_t ai_iSlapsedTime, eActionNameList *ai_vActionList)
  //## begin ClTimer::ClTimer%963302389.hasinit preserve=no
  //## end ClTimer::ClTimer%963302389.hasinit
  //## begin ClTimer::ClTimer%963302389.initialization preserve=yes
  //## end ClTimer::ClTimer%963302389.initialization
{
  //## begin ClTimer::ClTimer%963302389.body preserve=yes
	m_iSlapsedTime = ai_iSlapsedTime ;
	m_vActions = ai_vActionList ;
	m_poExecutiveObj = ai_poExecutive ;
  //## end ClTimer::ClTimer%963302389.body
}


ClTimer::~ClTimer ()
{
  //## begin ClTimer::~ClTimer%972579512.body preserve=yes

	M4_MDBG_DELETE(m_vActions,delete m_vActions) ;
	m_vActions=NULL ;
	
  //## end ClTimer::~ClTimer%972579512.body
}



//## Other Operations (implementation)
void ClTimer::Run (void )
{
  //## begin ClTimer::Run%963302390.body preserve=yes
	m4uint32_t iNumActions , iAction ;

	if ( ! m_poExecutiveObj )
		return ;

	if (! m_vActions )
		return ;

	if (! m_iSlapsedTime )
		return ;

	m4sleep ( 30 );

	while ( ! IsTerminate () )
	{
		iNumActions = m_vActions->size() ;

		for ( iAction = 0 ; iAction < iNumActions ; iAction ++ )
		{
			m_poExecutiveObj -> ExecuteAction ( (char*)((*m_vActions) [iAction]).c_str(), NULL ) ;
		}
		
		m4sleep ( m_iSlapsedTime );
	}
  //## end ClTimer::Run%963302390.body
} 

// Additional Declarations
  //## begin ClTimer%396ADE890067.declarations preserve=yes
  //## end ClTimer%396ADE890067.declarations

//## begin module%396AE24E03A4.epilog preserve=yes
//## end module%396AE24E03A4.epilog

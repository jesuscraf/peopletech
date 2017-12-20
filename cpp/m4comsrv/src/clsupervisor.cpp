//## begin module%347EEAA7000E.cm preserve=no
//## end module%347EEAA7000E.cm

//## begin module%347EEAA7000E.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            Supervisor
//	 File:              ClSupervisor.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:				980109
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
//## end module%347EEAA7000E.cp

//## Module: clsupervisor%347EEAA7000E; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: F:\integration\m4comsrv\src\clsupervisor.cpp

//## begin module%347EEAA7000E.additionalIncludes preserve=no
//## end module%347EEAA7000E.additionalIncludes

//## begin module%347EEAA7000E.includes preserve=yes
//	DEBUG
//JRM 
#if defined(M4_DEBUG) && ( defined(_WINDOWS) || defined(WINDOWS) )
#include <crtdbg.h>
#endif

#include <m4date.hpp>
#include <clsubsystem.hpp>
#include <clconfiguration.hpp>
#include <m4log.hpp>
#include <m4srvres.hpp>
//## end module%347EEAA7000E.includes

// m4thread
#include <m4thread.hpp>
// m4condition
#include <m4condition.hpp>
// clssinterfaz
#include <clssinterfaz.hpp>
// clsupervisor
#include <clsupervisor.hpp>

class ClServerEvent;

//## begin module%347EEAA7000E.declarations preserve=no
//## end module%347EEAA7000E.declarations

//## begin module%347EEAA7000E.additionalDeclarations preserve=yes
//## end module%347EEAA7000E.additionalDeclarations


// Class ClCSSupervisor 






ClCSSupervisor::ClCSSupervisor (m4uint32_t ai_iDelayTime)
  //## begin ClCSSupervisor::ClCSSupervisor%878114679.hasinit preserve=no
  //## end ClCSSupervisor::ClCSSupervisor%878114679.hasinit
  //## begin ClCSSupervisor::ClCSSupervisor%878114679.initialization preserve=yes
  //## end ClCSSupervisor::ClCSSupervisor%878114679.initialization
{
  //## begin ClCSSupervisor::ClCSSupervisor%878114679.body preserve=yes
	m_pSystem = NULL;

	if ( ai_iDelayTime > 0 )
		m_iDelayTime = ai_iDelayTime ;

	m_oStarter.Init( ) ;

  //## end ClCSSupervisor::ClCSSupervisor%878114679.body
}


ClCSSupervisor::~ClCSSupervisor ()
{
  //## begin ClCSSupervisor::~ClCSSupervisor%878114680.body preserve=yes


	while ( IsRunning ( ) ) 
	{
		m4sleep (1) ;
	}

  //## end ClCSSupervisor::~ClCSSupervisor%878114680.body
}



//## Other Operations (implementation)
void ClCSSupervisor::Run (void )
{
  //## begin ClCSSupervisor::Run%884283581.body preserve=yes
	M4ClCrono	tickCounter;
	m4int32_t	syncTime, sleepTime = 0;

	ClSystem * pSystem = ClSystem::Instance () ;
//No se descomenta esta linea hasta que se subscriba el subsistema 
//	de supervisor a los eventos del subsistema server
	m_oStarter.Wait ( ) ;

	while ( ! IsTerminate() )
	{
	sleepTime = 0;

//	DEBUG
#ifdef	M4_DEBUG
		_CrtMemState	s1, s2, s3;
#endif
//		startTime = (m4int32_t)GetTickCount() - sleepTime;
		tickCounter.Start();

//	DEBUG
#ifdef	M4_DEBUG
		_CrtMemCheckpoint(&s1);
#endif
//		pSystem->GetSubsystem()->Synchronize();

		//Ejecucion de todos los comandos parametrizados en obl
		m_oCommandList.ExecuteAll () ;

//	DEBUG
#ifdef	M4_DEBUG
		_CrtMemCheckpoint(&s2);

	   if ( _CrtMemDifference( &s3, &s1, &s2 ) )
		  _CrtMemDumpStatistics( &s3 );
#endif
   
		syncTime = (m4int32_t) tickCounter.Stop();

		sleepTime += ( 1000 * m_iDelayTime ) - syncTime;

		sleepTime /= 1000 ;

		if (sleepTime > 0)
		{
			m4uint32_t iSec ;

			for ( iSec = 0 ;iSec < sleepTime ;iSec ++ )
			{
				m4sleep (1) ;

				if ( IsTerminate () )
				{
					return ;
				}
			}

			sleepTime = 0;

		}

		tickCounter.Reset();

        // en cada iteración reseteo la pila de errores en memoria.
        // bugid: 94197
        CLEAN();
  }

  //## end ClCSSupervisor::Run%884283581.body
}

m4return_t ClCSSupervisor::RegisterCommands (ClConfiguration *ai_poConfiguration)
{
  //## begin ClCSSupervisor::RegisterCommands%933760136.body preserve=yes
	m4char_t szSubsystem [ 200] ;
	m4char_t szCommand [200] ;
	m4uint32_t iCommands;
	m4uint32_t iTamSubsystem ;
	m4uint32_t iTamCommand;
	m4uint32_t iHndProp;
	m4void_t * iHndMatch;
	m4return_t iRet ;
	ClSystem * poSystem ;
	ClAdminTagList aAdminTags ;
	m4bool_t bCommand = M4_FALSE ;
	ClAdminTagList::iterator Iterator ;
	ClSSInterfaz * poSubsystem ;
	m4char_t szId [20] ;


	if (! ai_poConfiguration )
	{
		return M4_ERROR ;
	}


	poSystem = ClSystem::Instance () ;

	iRet = ai_poConfiguration -> GetFirstProp ("", szSubsystem, 200, iTamSubsystem, szCommand, 200, iTamCommand, iHndProp, iHndMatch) ;

	while ( iRet == M4_SUCCESS )
	{
		if ( poSubsystem = poSystem -> FindSubsystemByName ( szSubsystem ) )		
		{
			aAdminTags.clear();

			sprintf ( szId , "%ld" , ( m4uint32_t ) poSubsystem -> GetId () ) ;

			if ( M4_SUCCESS == poSubsystem -> GetCommandsId ( szId , aAdminTags ) )
			{
				iCommands = aAdminTags.size() ;

				for ( Iterator = aAdminTags.begin() ; Iterator != aAdminTags.end() ; Iterator ++)
				{
					if (! strcmp ( (*Iterator) -> m_tagStringId , szCommand ) )
					{
						bCommand = M4_TRUE ;

						break ;
					}
				}

				if ( bCommand )
				{
					m_oCommandList.AddCommand( poSubsystem , (*Iterator)->m_tagId ) ;

					bCommand = M4_FALSE ;
				}
				else
				{
					SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "Command %s not found in %s subsystem" , szCommand , szSubsystem ) ;

					return M4_ERROR ;
				}

			}
		}

      iRet = ai_poConfiguration -> GetNextProp ("", szSubsystem, 200, iTamSubsystem, szCommand, 200, iTamCommand, iHndProp, iHndMatch );
	}

	return M4_SUCCESS ;
  //## end ClCSSupervisor::RegisterCommands%933760136.body
}

void ClCSSupervisor::Update (EVENTTYPE ai_iEvent, ClServerEvent *ai_oSubject)
{
  //## begin ClCSSupervisor::Update%943966401.body preserve=yes
	switch ( ai_iEvent )
	{
	case SHUTTING_DOWN :
	case SERVER_READY :
		m_oStarter.Signal ( ) ;
		break ;
	}

  //## end ClCSSupervisor::Update%943966401.body
}

// Additional Declarations
  //## begin ClCSSupervisor%3455E6740113.declarations preserve=yes
  //## end ClCSSupervisor%3455E6740113.declarations

// Class ClCommandsMap 




//## Other Operations (implementation)
void ClCommandsMap::AddCommand (ClSSInterfaz *ai_poSubsystem, m4uint32_t ai_iCommand)
{
  //## begin ClCommandsMap::AddCommand%933760133.body preserve=yes
	insert ( ClCommandsmapTypedef::value_type ( ai_poSubsystem, ai_iCommand ) ) ;
  //## end ClCommandsMap::AddCommand%933760133.body
}

m4return_t ClCommandsMap::ExecuteAll ()
{
  //## begin ClCommandsMap::ExecuteAll%933760134.body preserve=yes
	ClSSInterfaz * poSubsystem ;
	ClAdminTagList oParamList;
	ClAdminTagList oResult;
	ClCommandsmapTypedef::iterator Iterator ;
	m4return_t iRet ;
	m4char_t szId [20] ;



	for ( Iterator = begin() ; Iterator != end() ; Iterator ++)
	{
		poSubsystem = (*Iterator).first ;

		if ( poSubsystem )
		{
			sprintf ( szId , "%ld" , (m4uint32_t) poSubsystem -> GetId () ) ;
			iRet = poSubsystem -> ExecuteCommand ( szId , (m4char_t) (*Iterator).second , oParamList , oResult );
		}
	}
	return M4_SUCCESS ;
  //## end ClCommandsMap::ExecuteAll%933760134.body
}

// Additional Declarations
  //## begin ClCommandsMap%37A843BE0270.declarations preserve=yes
  //## end ClCommandsMap%37A843BE0270.declarations

//## begin module%347EEAA7000E.epilog preserve=yes
//## end module%347EEAA7000E.epilog

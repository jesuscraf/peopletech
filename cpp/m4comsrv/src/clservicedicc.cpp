//## begin module%3422406903C0.cm preserve=no
//## end module%3422406903C0.cm

//## begin module%3422406903C0.cp preserve=no
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
//## end module%3422406903C0.cp

//## Module: clservicedicc%3422406903C0; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: F:\integration\m4comsrv\version\src\clservicedicc.cpp

//## begin module%3422406903C0.additionalIncludes preserve=no
//## end module%3422406903C0.additionalIncludes

//## begin module%3422406903C0.includes preserve=yes
#include "cllocalmonitor.hpp"
#include <request.hpp>
#include <clserviceinfo.hpp>
#include <servicestats.hpp>
//## end module%3422406903C0.includes

// clbasedictionary
#include <clbasedictionary.hpp>
// clservicedicc
#include <clservicedicc.hpp>

class ClLocalMonitor;
class ClServiceInfo;

//## begin module%3422406903C0.declarations preserve=no
//## end module%3422406903C0.declarations

//## begin module%3422406903C0.additionalDeclarations preserve=yes
//## end module%3422406903C0.additionalDeclarations


// Class ClServiceDictionary 


ClServiceDictionary::ClServiceDictionary ()
  //## begin ClServiceDictionary::ClServiceDictionary%876496968.hasinit preserve=no
  //## end ClServiceDictionary::ClServiceDictionary%876496968.hasinit
  //## begin ClServiceDictionary::ClServiceDictionary%876496968.initialization preserve=yes
  //## end ClServiceDictionary::ClServiceDictionary%876496968.initialization
{
  //## begin ClServiceDictionary::ClServiceDictionary%876496968.body preserve=yes
  //## end ClServiceDictionary::ClServiceDictionary%876496968.body
}


ClServiceDictionary::~ClServiceDictionary ()
{
  //## begin ClServiceDictionary::~ClServiceDictionary%876496969.body preserve=yes
// OSCAR los servicios ya no se borran aquí sino en el subsistema del servicio correspondiente
/*	
	IterDictionary Iterator ;
	ClServiceInfo * poServiceInfo ;
	

		for ( Iterator = m_mapDictionary.begin () ; 
		Iterator != m_mapDictionary.end () ;
		Iterator ++ )
		{
			if ( poServiceInfo = ( ClServiceInfo *) ( * Iterator).second  )
			{
				delete ( poServiceInfo ) ;

				poServiceInfo = NULL ;

			}
		}
*/
		m_mapDictionary.clear ( ) ;

  //## end ClServiceDictionary::~ClServiceDictionary%876496969.body
}



//## Other Operations (implementation)
ClServiceInfo * ClServiceDictionary::AddServiceOnLine (m4int32_t ai_lIdService, ClLocalMonitor *ai_poLocalMonitor, m4uint16_t ai_iMaxNumQItem)
{
  //## begin ClServiceDictionary::AddServiceOnLine%876496970.body preserve=yes
		m4return_t iRet = M4_SUCCESS ;

		ClServiceStats * poStatistics = new ClServiceStats ;
		ClServiceInfo * poServiceInfo = new ClServiceInfo ( ai_poLocalMonitor , poStatistics , ai_iMaxNumQItem ) ;

		//Si el id de servicio es M4_NO_CHECK_IN_SERVICE entonces el servicio no se registra
		if ( M4_INTERNAL_SERVICE != ai_lIdService )
			iRet = AddItem ( ai_lIdService , ( ClBaseObject * ) poServiceInfo ) ;

		if ( iRet == M4_ERROR )
		{
//			SETCODEF ( M4_SRV_LM_SERVICE_NOT_CREATED , "Can't create the service %d , not was possible insert it into the distionary" , ai_IdService ) ;
		
			return NULL ;
		}
		return poServiceInfo ;
  //## end ClServiceDictionary::AddServiceOnLine%876496970.body
}

m4return_t ClServiceDictionary::RemoveService (m4int32_t ai_lIdService)
{
  //## begin ClServiceDictionary::RemoveService%876496971.body preserve=yes
		return  ( ( ( ClServiceInfo * )GetItem ( ai_lIdService ) ) -> Remove( ) ) ;
  //## end ClServiceDictionary::RemoveService%876496971.body
}

m4return_t ClServiceDictionary::DownItem (m4int32_t ai_lIdService)
{
  //## begin ClServiceDictionary::DownItem%876496972.body preserve=yes
	return M4_SUCCESS;
  //## end ClServiceDictionary::DownItem%876496972.body
}

void ClServiceDictionary::CheckThreads ()
{
  //## begin ClServiceDictionary::CheckThreads%878200305.body preserve=yes
	IterDictionary Iterator ;
	ClServiceInfo * poService ;
	m4int32_t iNumLaunchers = 0 ;

	

		for ( Iterator = m_mapDictionary.begin () ; 
		Iterator != m_mapDictionary.end () ;
		Iterator ++ )
		{
			poService = ( ClServiceInfo * )( * Iterator).second  ; 

			iNumLaunchers = poService -> m_oLauncherList.m_oActiveMap.size () ;

			
			if ( iNumLaunchers == 0 )
			{
				delete poService ;
				
				poService = NULL ;
			}

			
		}

  //## end ClServiceDictionary::CheckThreads%878200305.body
}

m4return_t ClServiceDictionary::ShutDown ()
{
  //## begin ClServiceDictionary::ShutDown%879854976.body preserve=yes
	IterDictionary Iterator ;
	ClServiceInfo * poService ;
	m4return_t iRet;



		for ( Iterator = m_mapDictionary.begin () ; 
		Iterator != m_mapDictionary.end () ;
		Iterator ++ )
		{
			poService = ( ClServiceInfo * ) ( * Iterator).second  ; 

			iRet = poService -> m_oLauncherList.ShutDown () ;

			if ( iRet == M4_ERROR )
			{
				return M4_ERROR ;
			}

		}
		
		m_mapDictionary.clear ( ) ;

		return M4_SUCCESS;
  //## end ClServiceDictionary::ShutDown%879854976.body
}

// Additional Declarations
  //## begin ClServiceDictionary%3422406200B2.declarations preserve=yes
  //## end ClServiceDictionary%3422406200B2.declarations

//## begin module%3422406903C0.epilog preserve=yes
//## end module%3422406903C0.epilog

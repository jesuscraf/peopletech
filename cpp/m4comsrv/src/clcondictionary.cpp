//## begin module%342240670100.cm preserve=no
//## end module%342240670100.cm

//## begin module%342240670100.cp preserve=no
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
//## end module%342240670100.cp

//## Module: clcondictionary%342240670100; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: F:\integration\m4comsrv\src\clcondictionary.cpp

//## begin module%342240670100.additionalIncludes preserve=no
//## end module%342240670100.additionalIncludes

//## begin module%342240670100.includes preserve=yes
#include <request.hpp>
#include <m4log.hpp>
#include <m4srvres.hpp>
#include <clssinvoker.hpp>
#include <cllocalmonitor.hpp>
#include <connects.hpp>
#include <m4actionconstants.hpp>
#include "tracerequest.hpp"
#include <clbasedictionary.hpp>
#include <clcondictionary.hpp>

class ClUSSession;


//## begin module%342240670100.declarations preserve=no
//## end module%342240670100.declarations

//## begin module%342240670100.additionalDeclarations preserve=yes
//## end module%342240670100.additionalDeclarations


// Class ClConDictionary 





//## Other Operations (implementation)
m4return_t ClConDictionary::RemoveConnection (m4objid_t ai_IdConnection)
{
  //## begin ClConDictionary::RemoveConnection%-1963334655.body preserve=yes
		ClConnInstance * poConnection ;
		m4return_t iRet; 


		poConnection = ( ClConnInstance * ) GetItem ( ai_IdConnection ) ;

		if ( ! poConnection )
		{
			SETCODEF ( M4_SRV_LM_DICTIONARY_ERROR , ERRORLOG , "Connection with Id %d has a NULL entry" , ai_IdConnection ) ;

			return M4_ERROR ;
		}

		iRet = RemoveItem ( ai_IdConnection ) ;

		return iRet ;
  //## end ClConDictionary::RemoveConnection%-1963334655.body
}

m4return_t ClConDictionary::Synchronize ()
{
  //## begin ClConDictionary::Synchronize%898585987.body preserve=yes
	ClUSSession * poSession = NULL ;
	IterDictionary Iterator , Iteraux;
	ClConnInstance * poConnection = NULL ;
	m4uint32_t iNumRequest ;
	ClLocalMonitor * poLocalMonitor = ClLocalMonitor::Instance () ;

    ClTraceRequest tRequest("ConDictionary.Synchronize");
    m4uint32_t iConnections = 0;

    tRequest.StartCount();

	Lock ( ) ;

		Iteraux = m_mapDictionary.begin () ;

		while ( Iteraux != m_mapDictionary.end () )
		{
			Iterator = Iteraux ;

			Iteraux ++ ;
            iConnections++;

			if ( poConnection = ( ClConnInstance *) ( * Iterator).second  )
			{

//				if ( 4 > poConnection -> GetConnProtocol () -> GetVersion() )
//				{

					iNumRequest = poConnection -> m_oNumRequest.GetValue ( ) ;
	
					if ( ! iNumRequest )
					{

						delete poConnection ;

						m_mapDictionary.erase ( Iterator ) ;

					}
//				}

			}
		}

//		m_mapDictionary.clear ( ) ;

		Unlock ( );

        tRequest.StopCount();

        if (iConnections > 0)
        {
            char msg[256];
            sprintf (msg, "\tConnections = %lu", iConnections);
            tRequest.SetMsg(msg);
            tRequest.Trace();
        }

		return M4_SUCCESS ;
  //## end ClConDictionary::Synchronize%898585987.body
}

m4return_t ClConDictionary::Replace (m4objid_t ai_lConnectionID, ClConnInstance *ai_poConection)
{
  //## begin ClConDictionary::Replace%967446812.body preserve=yes
	IterDictionary Iterator ;
	m4return_t error = M4_SUCCESS ;

	Lock ( ) ;

		Iterator = m_mapDictionary.find (ai_lConnectionID) ;

		if ( Iterator == m_mapDictionary.end () )
		{
			error = M4_ERROR ;
		}
		else
		{
			m_mapDictionary.erase ( Iterator ) ;

			m_mapDictionary.insert ( ClDictionary::value_type ( ai_lConnectionID, ai_poConection ) ) ;
		}

	Unlock ( ) ;

	return error ;
  //## end ClConDictionary::Replace%967446812.body
}

ClConnInstance * ClConDictionary::GetAndAddRequest (m4int64_t ai_Id)
{
  //## begin ClConDictionary::GetAndAddRequest%989340981.body preserve=yes
		IterDictionary	Iterator;
		ClConnInstance * poConnection ;

		Lock();
		
		Iterator = m_mapDictionary.find ( ai_Id ) ;


	if ( Iterator == m_mapDictionary.end() )
	{
	  	SETCODEF ( M4_SRV_LM_DICTIONARY_ERROR , WARNINGLOG , "Trying to get a Connection with the Id %d and there is one element in the dictionary with this Id. The connection could be closed jet" , (m4int32_t)ai_Id ) ;

		Unlock ( ) ;

		return NULL;
	}


	poConnection = (ClConnInstance * ) ( ( * Iterator).second ) ;

	++ (poConnection -> m_oNumRequest);  


	Unlock( );

	return poConnection ;
  //## end ClConDictionary::GetAndAddRequest%989340981.body
}

m4return_t ClConDictionary::GetUserAndSessionId (m4objid_t ai_iConnectionId, m4objid_t &ao_iUserId, m4objid_t &ao_iSessionId)
{
  //## begin ClConDictionary::GetUserAndSessionId%1010489722.body preserve=yes
		IterDictionary	Iterator;
		ClConnection * poConnection ;
		
		ao_iUserId = 0;
		ao_iSessionId = 0;

		Lock();
		
		Iterator = m_mapDictionary.find ( ai_iConnectionId ) ;


	if ( Iterator == m_mapDictionary.end() )
	{
	  	SETCODEF ( M4_SRV_LM_DICTIONARY_ERROR , WARNINGLOG , "Trying to get a Connection with the Id %d and there is one element in the dictionary with this Id. The connection could be closed jet" , (m4int32_t)ai_iConnectionId) ;

		Unlock ( ) ;

		return M4_ERROR ;
	}


	poConnection = (ClConnection * ) ( ( * Iterator).second ) ;

	if ( M4_CONNECTION_ORIENTED_PROTOCOL != poConnection -> GetConnProtocol( ) )
	{
		// bugid: 107287
        Unlock ( ) ;
		return M4_ERROR ;
	}

	ao_iSessionId = ((ClConnection *)poConnection ) -> GetSessId () ;
	ao_iUserId = ((ClConnection *)poConnection ) -> GetUserId () ;


	Unlock( );

	return M4_SUCCESS ;
  //## end ClConDictionary::GetUserAndSessionId%1010489722.body
}

// Additional Declarations
  //## begin ClConDictionary%342240220277.declarations preserve=yes
  //## end ClConDictionary%342240220277.declarations

//## begin module%342240670100.epilog preserve=yes
//## end module%342240670100.epilog

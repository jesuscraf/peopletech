//## begin module%34C8630D0301.cm preserve=no
//## end module%34C8630D0301.cm

//## begin module%34C8630D0301.cp preserve=no
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
//## end module%34C8630D0301.cp

//## Module: sessiondic%34C8630D0301; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: F:\integration\m4comsrv\src\sessiondic.cpp

//## begin module%34C8630D0301.additionalIncludes preserve=no
//## end module%34C8630D0301.additionalIncludes

//## begin module%34C8630D0301.includes preserve=yes
#include <cllocalmonitor.hpp>
#include <usinterface.hpp>
#include <ussession.hpp>
#include <m4srvtrace.h>
#include <m4srvres.hpp>
#include <m4log.hpp>
#include <clssinvoker.hpp>
#include <m4actionconstants.hpp>
#include <commactionsname.h>

#include "tracerequest.hpp"
//## end module%34C8630D0301.includes

// clbasedictionary
#include <clbasedictionary.hpp>
// sessiondic
#include <sessiondic.hpp>

class ClConnection;

//## begin module%34C8630D0301.declarations preserve=no
//## end module%34C8630D0301.declarations

//## begin module%34C8630D0301.additionalDeclarations preserve=yes
#define M4_SRV_TRACE_COMP_ID	"COMSRV"
//## end module%34C8630D0301.additionalDeclarations


// Class ClSessionDic 






//## Other Operations (implementation)
m4return_t ClSessionDic::AddSession (ClUSSessionInterface *ai_poSession)
{
  //## begin ClSessionDic::AddSession%885550081.body preserve=yes
	return AddItem ( ai_poSession -> GetId ( ) , ai_poSession) ;
  //## end ClSessionDic::AddSession%885550081.body
}

m4return_t ClSessionDic::RemoveSession (m4uint32_t ai_lIdSession)
{
  //## begin ClSessionDic::RemoveSession%885897765.body preserve=yes
	m4return_t iRet ;
	
	iRet = RemoveItem ( ai_lIdSession ) ;

	if ( iRet == M4_ERROR )
	{
		return M4_ERROR ;
	}
	
	return M4_SUCCESS ;
  //## end ClSessionDic::RemoveSession%885897765.body
}

void ClSessionDic::Synchronize ()
{
  //## begin ClSessionDic::Synchronize%901531333.body preserve=yes
	ClUSSessionInterface * poSession = NULL ;
	IterDictionary	Iterator , iteraux;
	m4uint32_t iNumRequest ;
	ClConnInstance * poConnection = NULL ;
	ClLocalMonitor * poLocalMonitor = NULL ;
	ClUSUserInterface * poUser ;
	ClParamList * poParamList  ;
	ClSSInvoker * poInvoker = ClSSInvoker::GetInstance () ;

    ClTraceRequest tRequest("SessionDic.Synchronize");
    m4uint32_t iSessions = 0;

    tRequest.StartCount();
	Lock();

	poLocalMonitor = ClLocalMonitor::Instance ( ) ;

	iteraux = m_mapDictionary.begin () ;

	while ( iteraux != m_mapDictionary.end () )
	{
		Iterator = iteraux ;
		
		iteraux++ ;
        iSessions++;

		if ( poSession= ( ClUSSessionInterface *) ( * Iterator).second  )
			{
				ClConnInstance * poConnection = poSession -> GetConnection()  ;

				if ( poConnection )
				{
					iNumRequest = poConnection -> m_oNumRequest.GetValue ( ) ;

					if (iNumRequest == 0 )
					{
						//Cerrar la conexión sin borrarlo
						poInvoker -> GetParamList ( poParamList ) ;

						poParamList -> SetInt ( M4_PARAM_CONNECTIONID , poConnection -> GetId() ) ;

						poInvoker ->ExecuteAction ( REMOVE_CONNECTION_ACTION , poParamList ) ;

//						poInvoker -> SetParamList ( *poParamList ) ;

						//POR SI EN EL TRANSCURSO HA LLEGADO UNA NUEVA REQUEST
						iNumRequest = poConnection -> m_oNumRequest.GetValue ( ) ;
						
						if (iNumRequest == 0 )
						{
							if ( M4_CONNECTION_ORIENTED_PROTOCOL == poConnection ->GetConnProtocol() )
							{
								poLocalMonitor -> RemoveConnection ( poConnection ) ;
							}

							poUser = poSession -> GetUser () ;
							
							if (poUser )
							{
								--( poUser -> m_oNumSessions ) ;
								
								if ( poUser -> m_oNumSessions.GetValue () == 0 ) 
								{
									delete poUser -> GetStats() ;
									delete poUser ;
								}
							}
							// (ADD FJSM 980104 Hay conexion y no hay request
							delete poSession -> GetStats () ;

							// Bg 115991. Mantenemos los patrones zDelta, se eliminaran cuando llegue la PDU de 
							// desconexión. Solo con http ya que en TCP no llega PDU de desconexión.
							if( poSession->GetConnProtocol() == M4_NON_CONNECTION_ORIENTED_PROTOCOL )
							{
								poSession->SetMustCleanDelta( M4_FALSE );
							}

							delete poSession ;

							m_mapDictionary.erase ( Iterator ) ;
							// ADD FJSM 980104)
						}

					}
				}
				// (ADD FJSM 980104 si no hay conexion se borra la sesion
				else 
				{
					if ( poSession -> CanDestroy () )
					{
						poUser = poSession -> GetUser () ;
					
						if (poUser )
						{
							--( poUser -> m_oNumSessions ) ;

							if ( poUser -> m_oNumSessions.GetValue () == 0 ) 
							{
								delete poUser -> GetStats() ;
								delete poUser ;
							}
						}
						delete poSession -> GetStats () ;

						// Bg 115991. Mantenemos los patrones zDelta, se eliminaran cuando llegue la PDU de 
						// desconexión. Solo con http ya que en TCP no llega PDU de desconexión.
						if( poSession->GetConnProtocol() == M4_NON_CONNECTION_ORIENTED_PROTOCOL )
						{
							poSession->SetMustCleanDelta( M4_FALSE );
						}

						delete poSession ;

						m_mapDictionary.erase ( Iterator ) ;
					}
				}
				// ADD FJSM 980104)
			}

				Iterator = iteraux ;
		}

	Unlock ( ) ;

    tRequest.StopCount();

    if (iSessions > 0)
    {
        char msg[256];
        sprintf (msg, "\tSessions = %lu", iSessions);
        tRequest.SetMsg(msg);
        tRequest.Trace();
    }
  //## end ClSessionDic::Synchronize%901531333.body
}

m4return_t ClSessionDic::CheckTimeouts (vector <m4objid_t> &ao_vSessions, vector <m4objid_t> &ao_vUsers)
{
  //## begin ClSessionDic::CheckTimeouts%985177782.body preserve=yes
	ClUSSession * poSession = NULL ;
	IterDictionary	Iterator , iteraux;


	Lock();

	for ( iteraux = m_mapDictionary.begin () ; iteraux != m_mapDictionary.end () ; iteraux ++)
	{
		poSession = (ClUSSession *)(*iteraux).second ;

		if ( SESSION_TIMED_OUT == poSession -> CheckTimeout () )
		{
			ao_vSessions.push_back ( poSession -> GetId () ) ;
			ao_vUsers.push_back ( poSession -> GetUser () -> GetId () ) ;
		}
	}

	Unlock();

	return M4_SUCCESS ;
  //## end ClSessionDic::CheckTimeouts%985177782.body
}

ClUSSessionInterface * ClSessionDic::GetAndAddRequest (m4int64_t ai_Id)
{
  //## begin ClSessionDic::GetAndAddRequest%989337235.body preserve=yes
		IterDictionary	Iterator;
		ClUSSessionInterface * poSession ;

		Lock();
		
		Iterator = m_mapDictionary.find ( ai_Id ) ;


	if ( Iterator == m_mapDictionary.end() )
	{
	  	SETCODEF ( M4_SRV_LM_DICTIONARY_ERROR , WARNINGLOG , "Trying to get a Session with the Id %d and there is one element in the dictionary with this Id. The session could be closed jet " , (m4int32_t)ai_Id ) ;
		Unlock ( ) ;

		return NULL;
	}


	poSession = (ClUSSessionInterface * ) ( ( * Iterator).second ) ;

	poSession -> AddRequest () ;

	Unlock( );

	return poSession ;
  //## end ClSessionDic::GetAndAddRequest%989337235.body
}

// Additional Declarations
  //## begin ClSessionDic%34C7A3850236.declarations preserve=yes
  //## end ClSessionDic%34C7A3850236.declarations

//## begin module%34C8630D0301.epilog preserve=yes
//## end module%34C8630D0301.epilog

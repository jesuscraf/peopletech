//## begin module%396C25FA0314.cm preserve=no
//## end module%396C25FA0314.cm

//## begin module%396C25FA0314.cp preserve=no
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
//## end module%396C25FA0314.cp

//## Module: executiveagent%396C25FA0314; Package body
//## Subsystem: M4Dispatcher::src%3923BBFB0332
//## Source file: F:\integration\m4dispatcher\src\executiveagent.cpp

//## begin module%396C25FA0314.additionalIncludes preserve=no
//## end module%396C25FA0314.additionalIncludes

//## begin module%396C25FA0314.includes preserve=yes
#include <dspinvoker.hpp>
#include <actionidentifiers.hpp>
#include <basiclog.hpp>
#include <m4dpchres.hpp>
#include <m4log.hpp>
#include <logsys.hpp>
#include <m4memdbg.h>
#include <m4srvtrace.h>
#include <dspreq.hpp>


#define M4_SRV_TRACE_COMP_ID "DPCH"

//## end module%396C25FA0314.includes

// dspagent
#include <dspagent.hpp>
// executiveagent
#include <executiveagent.hpp>
// actionworker
#include <actionworker.hpp>
//## begin module%396C25FA0314.declarations preserve=no
//## end module%396C25FA0314.declarations

//## begin module%396C25FA0314.additionalDeclarations preserve=yes
//## end module%396C25FA0314.additionalDeclarations


// Class ClDspWorker 


ClDspWorker::ClDspWorker (ClActionsQ &ai_poActionsQ, ClActionsQ &ai_poRecycleBin)
  //## begin ClDspWorker::ClDspWorker%963989741.hasinit preserve=no
  //## end ClDspWorker::ClDspWorker%963989741.hasinit
  //## begin ClDspWorker::ClDspWorker%963989741.initialization preserve=yes
  :ClActionWorker ( ai_poActionsQ )
  //## end ClDspWorker::ClDspWorker%963989741.initialization
{
  //## begin ClDspWorker::ClDspWorker%963989741.body preserve=yes
	m_poRecycleBin= &ai_poRecycleBin ;
  //## end ClDspWorker::ClDspWorker%963989741.body
}



//## Other Operations (implementation)
void ClDspWorker::ExecuteAction (ClBaseAction &ai_oAction)
{
  //## begin ClDspWorker::ExecuteAction%963989740.body preserve=yes
	ClParamList * poParamList = NULL ;
	m4bool_t bChangedLog = M4_FALSE ;
	ClLogSystem * poNewLog , * poOldLog;
	ClDspRequest * poRequest ;

	if ( poParamList = ai_oAction.GetParams () )
	{
		if ( M4_SUCCESS == poParamList -> GetPointer ( "REQUEST" , (void *&) poRequest  ) )
		{
			if ( poNewLog = poRequest -> GetLog () )
			{
				bChangedLog = M4_TRUE ;
				poOldLog = SETLOGSYSTEM ( poNewLog ) ;
			}
		}
	}

	ai_oAction.Execute ( ) ;

	if ( bChangedLog )
	{
		SETLOGSYSTEM ( poOldLog ) ;
	}

	m_poRecycleBin -> Push ( &ai_oAction) ;
  //## end ClDspWorker::ExecuteAction%963989740.body
}

// Additional Declarations
  //## begin ClDspWorker%3975656502E6.declarations preserve=yes
  //## end ClDspWorker%3975656502E6.declarations

// Class ClDspExecutiveAgent 







ClDspExecutiveAgent::ClDspExecutiveAgent ()
  //## begin ClDspExecutiveAgent::ClDspExecutiveAgent%963388992.hasinit preserve=no
  //## end ClDspExecutiveAgent::ClDspExecutiveAgent%963388992.hasinit
  //## begin ClDspExecutiveAgent::ClDspExecutiveAgent%963388992.initialization preserve=yes
  :ClDspAgent ( NULL , NULL )
  //## end ClDspExecutiveAgent::ClDspExecutiveAgent%963388992.initialization
{
  //## begin ClDspExecutiveAgent::ClDspExecutiveAgent%963388992.body preserve=yes
  //## end ClDspExecutiveAgent::ClDspExecutiveAgent%963388992.body
}


ClDspExecutiveAgent::~ClDspExecutiveAgent ()
{
  //## begin ClDspExecutiveAgent::~ClDspExecutiveAgent%963388993.body preserve=yes
	
	ClActionWorkerList::iterator It ;

	for (It=m_oWorkerList.begin(); It!=m_oWorkerList.end(); ++It ) {
		if ( (*It) ) {
			M4_MDBG_DELETE(  (*It), delete (*It) ) ; 
		} ;
	} ;
	m_oWorkerList.clear() ;
	
  //## end ClDspExecutiveAgent::~ClDspExecutiveAgent%963388993.body
}



//## Other Operations (implementation)
m4return_t ClDspExecutiveAgent::ProcessAction (ClActionIdentifier ai_oActionIdentifier, ClParamList *ai_poParamList, ClBaseAction *&ao_poAction)
{
  //## begin ClDspExecutiveAgent::ProcessAction%963388994.body preserve=yes
	switch ( ai_oActionIdentifier )
	{
	case GARBAGE_COLLECTION_ID  :
		GarbageCollection () ;
			break ;
	}

	return M4_SUCCESS ;
  //## end ClDspExecutiveAgent::ProcessAction%963388994.body
}

m4return_t ClDspExecutiveAgent::Start (m4uint32_t ai_iNumWorkers)
{
  //## begin ClDspExecutiveAgent::Start%963388995.body preserve=yes
	m4char_t szThread [100];
	m4uint32_t iWorkers ;
    ClActionWorker * poWorker = 0;

	m_iNumWorkers = ai_iNumWorkers ;

	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;

	if ( M4_ERROR == poInvoker -> RegisterAction ( GARBAGE_COLLECTION , GARBAGE_COLLECTION_ID , this ))
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOINITIALIZEAGENTACTION, 
			"Error starting Agent for %0:s. Action %1:s not registered.",
			"Executing Actions" << GARBAGE_COLLECTION);

		return M4_ERROR;
	}

	for ( iWorkers = 0 ;iWorkers < m_iNumWorkers ; iWorkers ++ )
	{
		sprintf ( szThread , "Dispatcher Worker %d" , iWorkers ) ;

//		M4_MDBG_NEW ( poWorker, poWorker = new ClActionWorker ( m_oActionsQ ) ) ;
		M4_MDBG_NEW( poWorker,  poWorker = new ClDspWorker ( m_oActionsQ , m_oRecycleBin ) ) ;

		if (poWorker)
		{
			poWorker -> Start ( szThread ) ;

			m_oWorkerList.push_back ( poWorker ) ;
		}
		else
		{
			M4_SRV_ERROR_N(M4_ERR_DPCH_NOMEMORYALLOCATION,
				"Error: Cannot allocate memory dynamically." );
			
			return M4_ERROR ;
		}
	}

	return M4_SUCCESS ;
  //## end ClDspExecutiveAgent::Start%963388995.body
}

void ClDspExecutiveAgent::PushAction (ClBaseAction *ai_poAction)
{
  //## begin ClDspExecutiveAgent::PushAction%963388996.body preserve=yes
	m_oActionsQ.Push ( ai_poAction ) ;
  //## end ClDspExecutiveAgent::PushAction%963388996.body
}

void ClDspExecutiveAgent::GarbageCollection ()
{
  //## begin ClDspExecutiveAgent::GarbageCollection%963989742.body preserve=yes
	ClBaseAction * poAction ;

	while ( m_oRecycleBin.Size() > 0 )
	{
		m_oRecycleBin.Pop ( poAction ) ;

		M4_MDBG_DELETE( (ClBaseAction * )poAction, delete poAction ) ;
	}

  //## end ClDspExecutiveAgent::GarbageCollection%963989742.body
}

void ClDspExecutiveAgent::Terminate ()
{
  //## begin ClDspExecutiveAgent::Terminate%972919261.body preserve=yes

	ClActionWorkerList::iterator It, ItNext ;

	//Pone Flag 'acaba ya!' para el Run

	for (It=m_oWorkerList.begin(); It!=m_oWorkerList.end(); ++It ) {
		(*It)->Terminate() ;	
	} ;

	//Espera a que acaben todos, y los va destruyendo.

	while ( m_oWorkerList.size()>0 ) {
		for (It=m_oWorkerList.begin(); It!=m_oWorkerList.end(); It = ItNext ) {
			ItNext = It ; ++ItNext ;
			if ( (*It)->IsExit() ) 
			{
				M4_MDBG_DELETE(  (*It), delete (*It) ) ; 
				m_oWorkerList.erase(It) ;
			} 
			else
			{
				if ( ! m_oActionsQ.Size () )
					PushAction ( NULL ) ;
			}
		} ;
	} ;

	//Borra la lista (aunque ya no debe haber nada)

	m_oWorkerList.clear() ;
	
  //## end ClDspExecutiveAgent::Terminate%972919261.body
}

m4return_t ClDspExecutiveAgent::Start ()
{
  //## begin ClDspExecutiveAgent::Start%1000984419.body preserve=yes
	return M4_ERROR ;
  //## end ClDspExecutiveAgent::Start%1000984419.body
}

// Additional Declarations
  //## begin ClDspExecutiveAgent%396C24DE009E.declarations preserve=yes
  //## end ClDspExecutiveAgent%396C24DE009E.declarations

//## begin module%396C25FA0314.epilog preserve=yes
//## end module%396C25FA0314.epilog

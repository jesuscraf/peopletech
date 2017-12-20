//## begin module%377C885700D1.cm preserve=no
//## end module%377C885700D1.cm

//## begin module%377C885700D1.cp preserve=no
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
//## end module%377C885700D1.cp

//## Module: logmanag%377C885700D1; Package body
//## Subsystem: M4Log::src%377C88560044
//	d:\compon\m4log\version\src
//## Source file: D:\ricardo\m4log\src\logmanag.cpp

//## begin module%377C885700D1.additionalIncludes preserve=no
//## end module%377C885700D1.additionalIncludes

//## begin module%377C885700D1.includes preserve=yes

#include "logsys.hpp"
#include "m4thread.hpp"



//## end module%377C885700D1.includes

// cnfglogs
#include <cnfglogs.hpp>
// logmanag
#include <logmanag.hpp>
// perslog
#include <perslog.hpp>
// debuglog
#include <debuglog.hpp>
// dbgresou
#include <dbgresou.hpp>
// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
// syncro
#include <syncro.hpp>
//## begin module%377C885700D1.declarations preserve=no
//## end module%377C885700D1.declarations

//## begin module%377C885700D1.additionalDeclarations preserve=yes

;



//## end module%377C885700D1.additionalDeclarations


// Class ClLogManager 































ClLogManager::ClLogManager ()
  //## begin ClLogManager::ClLogManager%1721961973.hasinit preserve=no
  //## end ClLogManager::ClLogManager%1721961973.hasinit
  //## begin ClLogManager::ClLogManager%1721961973.initialization preserve=yes
  //## end ClLogManager::ClLogManager%1721961973.initialization
{
  //## begin ClLogManager::ClLogManager%1721961973.body preserve=yes

	m_hmainThread = (m4int32_t)M4Thread::GetCurrentIdThread();
	m_configLogs.SetLogManager( this );
//	m_configLogs.SetResources( &m_resources );
	
	m_pModuleList = new ClStaticModuleList();
	m_configLogs.SetModuleList(m_pModuleList);
	m_GlobalCounterMutex.Init();
	m_AccessMutex.Init();
	m_GlobalMessageCounter = 0;

	m_bInitialized = M4_FALSE;
	m_ierror = 0;
	m_iwarning = 0;
	m_idebugInfo = 0;
	m_itrace = 0;
	m_cEndMessage = NULL;

	m_creturn = 13;

    m_bRemoteLog = M4_FALSE;

	m_bshowTime = M4_FALSE;
	m_bStorageAlways = M4_TRUE;
	m_bcrono = M4_TRUE;
	m_dTotalTime = 0;
	m_translateMessages = M4_FALSE;
	m_uidefaultIdiom = 2;

	m_MaxQueueSize = 100;

	if (getenv("M4LOG_TRACEALL")) 
		m_errorExists = M4_TRUE;
	else 
		m_errorExists = M4_FALSE;

	m_internalMessages= new ClAppointments(&m_lstorages,M4_TRUE);

  //## end ClLogManager::ClLogManager%1721961973.body
}


ClLogManager::~ClLogManager ()
{
  //## begin ClLogManager::~ClLogManager%1090247477.body preserve=yes

// borrar todos los logSystems

	LOGMAP::iterator	iter;
	ClLogSystem	*plogSystem;

	m_AccessMutex.Lock();

	iter = m_logSystems.begin();
	while (iter !=  m_logSystems.end())
	{
		plogSystem = (*iter).second;
		if ( plogSystem )	
			delete plogSystem;
		iter++;

	}
	
	m_logSystems.clear();
	m_AccessMutex.Unlock();
	m_lstorages.DestroyStorages();

	delete m_pModuleList;
	delete m_internalMessages;

  //## end ClLogManager::~ClLogManager%1090247477.body
}



//## Other Operations (implementation)
m4int16_t ClLogManager::SetIniFile (M4ClString sfile)
{
  //## begin ClLogManager::SetIniFile%1935879282.body preserve=yes

	return m_configLogs.SetIniFile( sfile );

  //## end ClLogManager::SetIniFile%1935879282.body
}

m4int16_t ClLogManager::Initialize ()
{
  //## begin ClLogManager::Initialize%1019146843.body preserve=yes

	if ( m_bInitialized )
		return M4_ERROR;


	m4int16_t iret = m_configLogs.Initialize();

	if ( iret != M4_ERROR )
	{
		m_bInitialized = M4_TRUE;
	}

	return iret;

  //## end ClLogManager::Initialize%1019146843.body
}

m4return_t ClLogManager::updateLogConfiguration (const M4ClString& ai_conigFileName)
{
  //## begin ClLogManager::updateLogConfiguration%467300552.body preserve=yes


	return m_configLogs.updateConfig(ai_conigFileName );

  //## end ClLogManager::updateLogConfiguration%467300552.body
}

m4int16_t ClLogManager::SetCode ()
{
  //## begin ClLogManager::SetCode%-1048504906.body preserve=yes

	return M4_SUCCESS;

  //## end ClLogManager::SetCode%-1048504906.body
}

m4int16_t ClLogManager::CreateCurrentThreadLogSystem ()
{
  //## begin ClLogManager::CreateCurrentThreadLogSystem%-1531797207.body preserve=yes

	m4int32_t	hthread;

	hthread = (m4int32_t)M4Thread::GetCurrentIdThread();

	return CreateLogSystem( hthread );


  //## end ClLogManager::CreateCurrentThreadLogSystem%-1531797207.body
}

m4int16_t ClLogManager::CreateLogSystem (m4int32_t hthread)
{
  //## begin ClLogManager::CreateLogSystem%1331215295.body preserve=yes


	m_AccessMutex.Lock();

	if ( m_logSystems.find( hthread ) != m_logSystems.end() )
	{
		//Ya existe un LogSystem para el hthread.
		m_AccessMutex.Unlock();
		return M4_ERROR;
	}
	
	ClLogSystem	*plogsys;

	plogsys = new ClLogSystem( this, m_bshowTime );

	m_logSystems.insert( LOGMAP::value_type( hthread, plogsys ) );
	plogsys->SetIdThread( hthread );
	m_AccessMutex.Unlock();

	return M4_SUCCESS;

  //## end ClLogManager::CreateLogSystem%1331215295.body
}

m4bool_t ClLogManager::ExistLogSystem (m4int32_t hthread)
{
  //## begin ClLogManager::ExistLogSystem%-750215146.body preserve=yes

	m4bool_t ret = M4_FALSE;
	m_AccessMutex.Lock();
	if ( m_logSystems.find( hthread ) != m_logSystems.end() )
		ret = M4_TRUE;
	m_AccessMutex.Unlock();
	return ret;

  //## end ClLogManager::ExistLogSystem%-750215146.body
}

ClLogSystem* ClLogManager::GetCurrentLogSystem ()
{
  //## begin ClLogManager::GetCurrentLogSystem%901078942.body preserve=yes

	ClLogSystem	*plogSystem;
	m4int32_t	lthread = (m4int32_t)M4Thread::GetCurrentIdThread();

	LOGMAP::iterator	iter;
	
	m_AccessMutex.Lock();
	if( ( iter = m_logSystems.find( lthread ) ) == m_logSystems.end() )
	{
		m_AccessMutex.Unlock();
		return NULL;
	}
	plogSystem = (*iter).second;
	m_AccessMutex.Unlock();
	return plogSystem;


  //## end ClLogManager::GetCurrentLogSystem%901078942.body
}

ClLogSystem* ClLogManager::SetLogSystem (ClLogSystem* ai_LogSystem)
{
  //## begin ClLogManager::SetLogSystem%1992430839.body preserve=yes

	m4iden_t	lthread = (m4int32_t)M4Thread::GetCurrentIdThread();
	m4iden_t currentThread = 0;

    // updates the referenced info
    if (NULL != ai_LogSystem)
    {
        currentThread = ai_LogSystem->GetIdThread();
        ai_LogSystem->SetIdThread( lthread );

		/* Bug 0108256
		Hay que resetear el nivel de log, por si venía de otro cliente
		*/
		ai_LogSystem->ResetOverwriteMask( this ) ;
    }

	LOGMAP::iterator	iter;
	ClLogSystem *pLogSystem;
	
	m_AccessMutex.Lock();
	
	// releases old references
	// fix bug 0089549
	if ( currentThread != 0 && currentThread != lthread ) {
		
		iter = m_logSystems.find( currentThread );
		if (iter != m_logSystems.end()) {
			
			pLogSystem = (*iter).second;
			
			// if still referenced in the old thread deletes the entry
			if ( pLogSystem == ai_LogSystem ) {
				m_logSystems.erase( iter );
			}
		}
	}

	if( ( iter = m_logSystems.find( lthread ) ) == m_logSystems.end() )
	{	
        pLogSystem = NULL;
		if (NULL != ai_LogSystem)
        {
		    m_logSystems.insert( LOGMAP::value_type( lthread, ai_LogSystem ) );
        }
	}
	else
	{
		pLogSystem = (*iter).second;
		if (NULL != ai_LogSystem)
        {
            (*iter).second = ai_LogSystem;
        }
        else
        {
            m_logSystems.erase(iter);
        }
	}
	
	m_AccessMutex.Unlock();
	return pLogSystem;

  //## end ClLogManager::SetLogSystem%1992430839.body
}

m4return_t ClLogManager::GetLogSystem (m4int32_t hthread, ClLogSystem*& plogSystem)
{
  //## begin ClLogManager::GetLogSystem%1860498566.body preserve=yes

	LOGMAP::iterator	iter;
	
	m_AccessMutex.Lock();
	if( ( iter = m_logSystems.find( hthread ) ) == m_logSystems.end() )
	{
		plogSystem = NULL;
		m_AccessMutex.Unlock();
		return M4_ERROR;
	}
	plogSystem = (*iter).second;
	m_AccessMutex.Unlock();
	return M4_SUCCESS;

  //## end ClLogManager::GetLogSystem%1860498566.body
}

m4uint32_t ClLogManager::GetDefaultLevelInfo ()
{
  //## begin ClLogManager::GetDefaultLevelInfo%-1746903291.body preserve=yes


	return m_configLogs.GetDefaultLevelInfo();

  //## end ClLogManager::GetDefaultLevelInfo%-1746903291.body
}

void ClLogManager::SetCReturn (m4char_t creturn)
{
  //## begin ClLogManager::SetCReturn%-447715444.body preserve=yes

	m_creturn = creturn;

  //## end ClLogManager::SetCReturn%-447715444.body
}

void ClLogManager::SetEndMessageChar (m4char_t cEndMessage)
{
  //## begin ClLogManager::SetEndMessageChar%-1983658373.body preserve=yes

	m_cEndMessage = cEndMessage;

  //## end ClLogManager::SetEndMessageChar%-1983658373.body
}

m4uint32_t ClLogManager::GetMaxQueueSize ()
{
  //## begin ClLogManager::GetMaxQueueSize%1572944467.body preserve=yes

	return m_MaxQueueSize;

  //## end ClLogManager::GetMaxQueueSize%1572944467.body
}

void ClLogManager::SetMaxQueueSize (m4uint32_t ai_MaxQueueSize)
{
  //## begin ClLogManager::SetMaxQueueSize%-869905716.body preserve=yes

	m_MaxQueueSize = ai_MaxQueueSize;

  //## end ClLogManager::SetMaxQueueSize%-869905716.body
}

m4bool_t ClLogManager::Error ()
{
  //## begin ClLogManager::Error%698462692.body preserve=yes

	if ( ! m_ierror )
		return M4_FALSE;
	return M4_TRUE;

  //## end ClLogManager::Error%698462692.body
}

m4bool_t ClLogManager::Warning ()
{
  //## begin ClLogManager::Warning%2111473341.body preserve=yes

	if ( ! m_iwarning )
		return M4_FALSE;
	return M4_TRUE;

  //## end ClLogManager::Warning%2111473341.body
}

m4bool_t ClLogManager::DebugInfo ()
{
  //## begin ClLogManager::DebugInfo%638440848.body preserve=yes

	if ( ! m_idebugInfo )
		return M4_FALSE;
	return M4_TRUE;

  //## end ClLogManager::DebugInfo%638440848.body
}

m4bool_t ClLogManager::TraceInfo ()
{
  //## begin ClLogManager::TraceInfo%1886886352.body preserve=yes

	if ( ! m_itrace )
		return M4_FALSE;
	return M4_TRUE;

  //## end ClLogManager::TraceInfo%1886886352.body
}

m4bool_t ClLogManager::TestErrorMode (m4int16_t berror)
{
  //## begin ClLogManager::TestErrorMode%-1205211492.body preserve=yes

	switch ( berror )
	{
	case DEBUGINFOLOG:
		if ( m_idebugInfo )
			return M4_TRUE;
		break;
	case WARNINGLOG:
		if ( m_iwarning )
			return M4_TRUE;
                                                                                                                                                         		break;
	case ERRORLOG:
		if ( m_ierror )
			return M4_TRUE;
		break;

	case TRACEINFOLOG:
		if ( m_itrace )
			return M4_TRUE;
		break;
	}

	return M4_FALSE;

  //## end ClLogManager::TestErrorMode%-1205211492.body
}

m4int16_t ClLogManager::ActiveError ()
{
  //## begin ClLogManager::ActiveError%1996652263.body preserve=yes

	m_ierror = 1;
	return M4_SUCCESS;

  //## end ClLogManager::ActiveError%1996652263.body
}

m4int16_t ClLogManager::ActiveWarning ()
{
  //## begin ClLogManager::ActiveWarning%-1295001793.body preserve=yes

	m_iwarning = 1;
	return M4_SUCCESS;

  //## end ClLogManager::ActiveWarning%-1295001793.body
}

m4int16_t ClLogManager::ActiveDebugInfo ()
{
  //## begin ClLogManager::ActiveDebugInfo%-948510497.body preserve=yes

	m_idebugInfo = 1;
	return M4_SUCCESS;

  //## end ClLogManager::ActiveDebugInfo%-948510497.body
}

m4int16_t ClLogManager::ActiveTraceInfo ()
{
  //## begin ClLogManager::ActiveTraceInfo%406593264.body preserve=yes

	m_itrace = 1;
	return M4_SUCCESS;

  //## end ClLogManager::ActiveTraceInfo%406593264.body
}

m4int16_t ClLogManager::EnableRemoteLog ()
{
  //## begin ClLogManager::EnableRemoteLog%2126153487.body preserve=yes

	m_bRemoteLog = M4_TRUE;
	return M4_SUCCESS;

  //## end ClLogManager::EnableRemoteLog%2126153487.body
}

m4int16_t ClLogManager::DefuseError ()
{
  //## begin ClLogManager::DefuseError%-592654900.body preserve=yes

	m_ierror = 0;
	return M4_SUCCESS;

  //## end ClLogManager::DefuseError%-592654900.body
}

m4int16_t ClLogManager::DefuseWarning ()
{
  //## begin ClLogManager::DefuseWarning%1274363651.body preserve=yes

	m_iwarning = 0;
	return M4_SUCCESS;

  //## end ClLogManager::DefuseWarning%1274363651.body
}

m4int16_t ClLogManager::DefuseDebugInfo ()
{
  //## begin ClLogManager::DefuseDebugInfo%1650042316.body preserve=yes

	m_idebugInfo = 0;
	return M4_SUCCESS;

  //## end ClLogManager::DefuseDebugInfo%1650042316.body
}

m4int16_t ClLogManager::DefuseTraceInfo ()
{
  //## begin ClLogManager::DefuseTraceInfo%-24588130.body preserve=yes

	m_itrace = 0;
	return M4_SUCCESS;

  //## end ClLogManager::DefuseTraceInfo%-24588130.body
}

m4int16_t ClLogManager::DisableRemoteLog ()
{
  //## begin ClLogManager::DisableRemoteLog%489463073.body preserve=yes

	m_bRemoteLog = M4_FALSE;
	return M4_SUCCESS;

  //## end ClLogManager::DisableRemoteLog%489463073.body
}

m4bool_t ClLogManager::GetRemoteLog ()
{
  //## begin ClLogManager::GetRemoteLog%849195721.body preserve=yes

	return m_bRemoteLog;

  //## end ClLogManager::GetRemoteLog%849195721.body
}

m4bool_t ClLogManager::StorageAlways ()
{
  //## begin ClLogManager::StorageAlways%-1337757572.body preserve=yes

	return m_bStorageAlways;

  //## end ClLogManager::StorageAlways%-1337757572.body
}

void ClLogManager::ActiveStorageAlways ()
{
  //## begin ClLogManager::ActiveStorageAlways%818978504.body preserve=yes

	m_bStorageAlways = M4_TRUE;

  //## end ClLogManager::ActiveStorageAlways%818978504.body
}

void ClLogManager::DefuseStorageAlways ()
{
  //## begin ClLogManager::DefuseStorageAlways%935149151.body preserve=yes

	m_bStorageAlways = M4_FALSE;

  //## end ClLogManager::DefuseStorageAlways%935149151.body
}

m4return_t ClLogManager::DestroyLogSystem ()
{
  //## begin ClLogManager::DestroyLogSystem%-1406380236.body preserve=yes

	ClLogSystem	*plogSystem;
	m4int32_t	lthread = (m4int32_t)M4Thread::GetCurrentIdThread();

	LOGMAP::iterator	iter;
	
	m_AccessMutex.Lock();
	if( ( iter = m_logSystems.find( lthread ) ) == m_logSystems.end() )
	{
		m_AccessMutex.Unlock();
		return M4_ERROR;
	}
	plogSystem = (*iter).second;
	m_logSystems.erase(iter);
	m_AccessMutex.Unlock();

	delete plogSystem;
	
	
	return M4_SUCCESS;

  //## end ClLogManager::DestroyLogSystem%-1406380236.body
}

m4return_t ClLogManager::IncMessageCounter ()
{
  //## begin ClLogManager::IncMessageCounter%2101165916.body preserve=yes

	m_GlobalCounterMutex.Lock();
	m_GlobalMessageCounter++;
	m_GlobalCounterMutex.Unlock();
	return M4_SUCCESS;



  //## end ClLogManager::IncMessageCounter%2101165916.body
}

m4uint32_t ClLogManager::GetMessageCounter ()
{
  //## begin ClLogManager::GetMessageCounter%766650907.body preserve=yes


	m4uint32_t ret;
	
	m_GlobalCounterMutex.Lock();
	ret = m_GlobalMessageCounter;
	m_GlobalCounterMutex.Unlock();
	return ret;


  //## end ClLogManager::GetMessageCounter%766650907.body
}

ClStaticModuleList* ClLogManager::GetModuleList ()
{
  //## begin ClLogManager::GetModuleList%-494877376.body preserve=yes

	return m_pModuleList;

  //## end ClLogManager::GetModuleList%-494877376.body
}

m4return_t ClLogManager::AddStorage (ClPersistentStorage* pstorage)
{
  //## begin ClLogManager::AddStorage%-1672270263.body preserve=yes

	return m_lstorages.AddStorage( pstorage );

  //## end ClLogManager::AddStorage%-1672270263.body
}

ClStorageList* ClLogManager::GetStorageList ()
{
  //## begin ClLogManager::GetStorageList%976551028.body preserve=yes

	return &m_lstorages;

  //## end ClLogManager::GetStorageList%976551028.body
}

m4uint32_t ClLogManager::isTranslatable ()
{
  //## begin ClLogManager::isTranslatable%942747997.body preserve=yes
  
	return m_translateMessages;
	
  //## end ClLogManager::isTranslatable%942747997.body
}

void ClLogManager::enableTranslateMessages ()
{
  //## begin ClLogManager::enableTranslateMessages%942747998.body preserve=yes
  
	m_translateMessages = M4LOG_TRANSLATABLE;
	
  //## end ClLogManager::enableTranslateMessages%942747998.body
}

void ClLogManager::disableTranslateMessages ()
{
  //## begin ClLogManager::disableTranslateMessages%942747999.body preserve=yes

	m_translateMessages = M4LOG_NO_TRANSLATABLE;

  //## end ClLogManager::disableTranslateMessages%942747999.body
}

void ClLogManager::bothTranslateMessages ()
{
  //## begin ClLogManager::bothTranslateMessages%943956490.body preserve=yes
  
		m_translateMessages = M4LOG_BOTH_TRANSLATABLE;
	
  //## end ClLogManager::bothTranslateMessages%943956490.body
}

m4uint32_t ClLogManager::getDefaultIdiom ()
{
  //## begin ClLogManager::getDefaultIdiom%942748000.body preserve=yes
 
	return m_uidefaultIdiom;
	
  //## end ClLogManager::getDefaultIdiom%942748000.body
}

void ClLogManager::setDefaultIdiom (m4uint32_t ai_defaultIdiom)
{
  //## begin ClLogManager::setDefaultIdiom%942748001.body preserve=yes
  
	m_uidefaultIdiom = ai_defaultIdiom;
	
  //## end ClLogManager::setDefaultIdiom%942748001.body
}

m4return_t ClLogManager::addInternalMessage (ClAppItem* pappoint)
{
  //## begin ClLogManager::addInternalMessage%944066181.body preserve=yes

	if (m_errorExists == M4_FALSE) {
		if (!strcmp( (pappoint->GetErrType().c_str()) ,"_error_" )) {
		
			m4uint32_t iter = 0;

			ClAppItem* auxpappoint;

			while( auxpappoint = (m_internalMessages->GetAppoint(iter)) )
			{			
				m_internalMessages-> GetStorageList() -> GetStorage(0) -> AddAppoint(auxpappoint);
				iter++;
			}	

			m_errorExists=M4_TRUE;
			m_internalMessages-> GetStorageList() -> GetStorage(0) -> AddAppoint(pappoint);
		
		} else 
			m_internalMessages->AddAppoint(pappoint->CopyAppItem());
	} else 
		m_internalMessages-> GetStorageList() -> GetStorage(0) -> AddAppoint(pappoint);
	

	return M4_SUCCESS;	
	
  //## end ClLogManager::addInternalMessage%944066181.body
}

// Additional Declarations
  //## begin ClLogManager%377C883E034C.declarations preserve=yes
  //## end ClLogManager%377C883E034C.declarations

//## begin module%377C885700D1.epilog preserve=yes
//## end module%377C885700D1.epilog

//## begin module%373C16D202BE.cm preserve=no
//## end module%373C16D202BE.cm

//## begin module%373C16D202BE.cp preserve=no
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
//## end module%373C16D202BE.cp

//## Module: USInterface%373C16D202BE; Package body
//## Subsystem: M4UserInterface::src%3795DCBF0109
//## Source file: F:\integration\m4userinterface\src\usinterface.cpp

//## begin module%373C16D202BE.additionalIncludes preserve=no
//## end module%373C16D202BE.additionalIncludes

//## begin module%373C16D202BE.includes preserve=yes
#include "objdatacacheimpl.hpp"
#include "objdatacachemanager.hpp"
#include "commprotocols.hpp"                // compresión zdelta.
//## end module%373C16D202BE.includes

// USInterface
#include <usinterface.hpp>
// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
// ConnBase
#include <connbase.hpp>
// clbaseobject
#include <clbaseobject.hpp>
// ExVariables
#include <exvariables.hpp>


//## begin module%373C16D202BE.declarations preserve=no
//## end module%373C16D202BE.declarations

//## begin module%373C16D202BE.additionalDeclarations preserve=yes
//## end module%373C16D202BE.additionalDeclarations


// Class ClUSSessionInterface 










ClUSSessionInterface::ClUSSessionInterface (const m4uint32_t ai_uiId, const m4string_t &ai_strSessID, ClConnBase *ai_poConnection, ClUSUserInterface *ai_poUser, ClStatistic *ai_poStats, ClUSSessionTicketInterface *ai_poUSTicket, ClVMBaseEnv* ai_poEnvironment, const m4uint32_t ai_uiCompType, const m4uint32_t ai_uiCompLevel, m4bool_t ai_bAuditSession)
  //## begin ClUSSessionInterface::ClUSSessionInterface%885806463.hasinit preserve=no
      : m_szHost("")
  //## end ClUSSessionInterface::ClUSSessionInterface%885806463.hasinit
  //## begin ClUSSessionInterface::ClUSSessionInterface%885806463.initialization preserve=yes
  ,	ClBaseObject ( ai_poStats ), m_strSessionID(ai_strSessID), m_poSessionTicket(ai_poUSTicket),m_oStatus (ai_uiId)
  //## end ClUSSessionInterface::ClUSSessionInterface%885806463.initialization
{
  //## begin ClUSSessionInterface::ClUSSessionInterface%885806463.body preserve=yes
	m_lId = ai_uiId;
	m_pUser = ai_poUser ;
	m_pConnection = ai_poConnection ;
	m_lType = SESSION_OBJECT_TYPE ;

	m_lCompressionType  = ai_uiCompType;
	m_lCompressionLevel = ai_uiCompLevel;

	m_bMustCleanDelta = M4_TRUE;

	m_bAuditSession = ai_bAuditSession;

    DeleteSessionReferences(ai_poEnvironment);
    //## end ClUSSessionInterface::ClUSSessionInterface%885806463.body
}


ClUSSessionInterface::~ClUSSessionInterface ()
{
  //## begin ClUSSessionInterface::~ClUSSessionInterface%893349364.body preserve=yes
	if (NULL != m_poSessionTicket)
		delete m_poSessionTicket;

	m_poSessionTicket = NULL;
    DeleteSessionReferences(NULL);

  //## end ClUSSessionInterface::~ClUSSessionInterface%893349364.body
}

m4return_t ClUSSessionInterface::DeleteSessionReferences(ClVMBaseEnv* ai_poEnvironment)
{
    // Borro los objetos asociados a esta sesión en la cache de auto. En teoría no hay
    // objetos asociados a este número, pero si alguna sesion se ha muerto sin control
    // y reutilizamos el número podría haber objetos.
    // Esto garantiza que limpiamos los objetos que ahora ya no tienen sentido.
    ClObjDataCache* poObjDataCache = ClObjDataCacheManager::GetObjDataCache(ai_poEnvironment);
    if (NULL != poObjDataCache)
    {
        m4char_t pcSessionId[64];
        sprintf(pcSessionId, "%u", m_oStatus.GetIdObject());
        ClObjDataCacheBySessionId* poObjDataCacheBySessionId = new ClObjDataCacheBySessionId(pcSessionId, poObjDataCache);
        if (NULL != poObjDataCacheBySessionId)
        {
            poObjDataCacheBySessionId->DeleteAll();
            delete poObjDataCacheBySessionId;
        }
    }
    
    // Borro los objetos asociados en el mapa de zdelta.

	// Bg 115991.
	if( m_bMustCleanDelta == true )
	{
		M4ZDeltaManager* poDeltaManager = M4ZDeltaManager::GetInstance();
		if (NULL != poDeltaManager)
		{
			m4uint64_t llKey = ClPDUCommZDelta::MakeKey( m_oStatus.GetIdObject(), 0, M4_FALSE ) ;
			poDeltaManager->erase( llKey ) ;
		}
	}
        
    return M4_SUCCESS;
}


//## Other Operations (implementation)
eConnProtocol ClUSSessionInterface::GetConnProtocol ()
{
  //## begin ClUSSessionInterface::GetConnProtocol%971774178.body preserve=yes
	return m_lConnProtocol ;
  //## end ClUSSessionInterface::GetConnProtocol%971774178.body
}

void ClUSSessionInterface::SetConnProtocol (eConnProtocol ai_lConnProtocol)
{
  //## begin ClUSSessionInterface::SetConnProtocol%971774179.body preserve=yes
	m_lConnProtocol = ai_lConnProtocol ;
  //## end ClUSSessionInterface::SetConnProtocol%971774179.body
}

void ClUSSessionInterface::SetHost (m4pchar_t ai_szHost)
{
  //## begin ClUSSessionInterface::SetHost%982693606.body preserve=yes
	m_szHost = ai_szHost ;
  //## end ClUSSessionInterface::SetHost%982693606.body
}

m4pcchar_t ClUSSessionInterface::GetHost ()
{
  //## begin ClUSSessionInterface::GetHost%982693607.body preserve=yes
	return m_szHost.c_str() ;
  //## end ClUSSessionInterface::GetHost%982693607.body
}

void ClUSSessionInterface::SetMustCleanDelta( m4bool_t ai_bMustClean )
{
	m_bMustCleanDelta = ai_bMustClean ;
}

m4bool_t ClUSSessionInterface::GetAuditSession( void ) const
{
	return( m_bAuditSession ) ;
}



// Class ClUSUserInterface 






ClUSUserInterface::ClUSUserInterface (const m4string_t &ai_strUserID, const m4string_t &ai_strName, ClStatistic *ai_poStatistic, const m4string_t ai_strUserAlias, const m4string_t ai_strUserImpersonator)
  //## begin ClUSUserInterface::ClUSUserInterface%876496976.hasinit preserve=no
  //## end ClUSUserInterface::ClUSUserInterface%876496976.hasinit
  //## begin ClUSUserInterface::ClUSUserInterface%876496976.initialization preserve=yes
  : ClBaseObject ( ai_poStatistic ), m_strUserID(ai_strUserID), m_strName(ai_strName), m_strUserAlias(ai_strUserAlias), m_strUserImpersonator(ai_strUserImpersonator)
  //## end ClUSUserInterface::ClUSUserInterface%876496976.initialization
{
  //## begin ClUSUserInterface::ClUSUserInterface%876496976.body preserve=yes
	m_lType = USER_OBJECT_TYPE ;

  //## end ClUSUserInterface::ClUSUserInterface%876496976.body
}


ClUSUserInterface::~ClUSUserInterface ()
{
  //## begin ClUSUserInterface::~ClUSUserInterface%876496975.body preserve=yes
  //## end ClUSUserInterface::~ClUSUserInterface%876496975.body
}


// Class ClUSSessionTicketInterface 




ClUSSessionTicketInterface::ClUSSessionTicketInterface (m4char_t *ai_poTicketCode)
  //## begin ClUSSessionTicketInterface::ClUSSessionTicketInterface%926583154.hasinit preserve=no
  //## end ClUSSessionTicketInterface::ClUSSessionTicketInterface%926583154.hasinit
  //## begin ClUSSessionTicketInterface::ClUSSessionTicketInterface%926583154.initialization preserve=yes
  :	m_poTicketCode(ai_poTicketCode)
  //## end ClUSSessionTicketInterface::ClUSSessionTicketInterface%926583154.initialization
{
  //## begin ClUSSessionTicketInterface::ClUSSessionTicketInterface%926583154.body preserve=yes
  //## end ClUSSessionTicketInterface::ClUSSessionTicketInterface%926583154.body
}


ClUSSessionTicketInterface::~ClUSSessionTicketInterface ()
{
  //## begin ClUSSessionTicketInterface::~ClUSSessionTicketInterface%926583151.body preserve=yes
  //## end ClUSSessionTicketInterface::~ClUSSessionTicketInterface%926583151.body
}


// Class ClUSSessionInterface40 





ClUSSessionInterface40::ClUSSessionInterface40 (const m4uint32_t ai_uiId, const m4string_t &ai_strSessID, ClUSUserInterface *ai_poUser, ClStatistic *ai_poStats, ClUSSessionTicketInterface *ai_poUSTicket)
  //## begin ClUSSessionInterface40::ClUSSessionInterface40%965977100.hasinit preserve=no
  //## end ClUSSessionInterface40::ClUSSessionInterface40%965977100.hasinit
  //## begin ClUSSessionInterface40::ClUSSessionInterface40%965977100.initialization preserve=yes
  :	ClBaseObject ( ai_poStats ), m_strSessionID(ai_strSessID), m_poSessionTicket(ai_poUSTicket),m_oStatus (ai_uiId)
  //## end ClUSSessionInterface40::ClUSSessionInterface40%965977100.initialization
{
  //## begin ClUSSessionInterface40::ClUSSessionInterface40%965977100.body preserve=yes
  	m_lId = ai_uiId;
	m_poUser = ai_poUser ;
	m_lType = SESSION_OBJECT_TYPE ;
  //## end ClUSSessionInterface40::ClUSSessionInterface40%965977100.body
}


ClUSSessionInterface40::~ClUSSessionInterface40 ()
{
  //## begin ClUSSessionInterface40::~ClUSSessionInterface40%965977101.body preserve=yes
  //## end ClUSSessionInterface40::~ClUSSessionInterface40%965977101.body
}


// Additional Declarations
  //## begin ClUSSessionInterface40%3993BECC01A1.declarations preserve=yes
  //## end ClUSSessionInterface40%3993BECC01A1.declarations

//## begin module%373C16D202BE.epilog preserve=yes
//## end module%373C16D202BE.epilog

//## begin module%38D9EC8E00EA.cm preserve=no
//## end module%38D9EC8E00EA.cm

//## begin module%38D9EC8E00EA.cp preserve=no
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
//## end module%38D9EC8E00EA.cp

//## Module: cachessubsystem%38D9EC8E00EA; Package body
//## Subsystem: M4CacheSubsystem::src%37987870022B
//## Source file: D:\Work\v600\M4CacheSubsystem\src\cachessubsystem.cpp

//## begin module%38D9EC8E00EA.additionalIncludes preserve=no
//## end module%38D9EC8E00EA.additionalIncludes

//## begin module%38D9EC8E00EA.includes preserve=yes
#include <cachesubsystem.hpp>
//## end module%38D9EC8E00EA.includes

// cachessubsystem
#include <cachessubsystem.hpp>
// Statistics
#include <statistics.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>
#include "replaceliterals.hpp"


class ClCacheTest;

//## begin module%38D9EC8E00EA.declarations preserve=no
//## end module%38D9EC8E00EA.declarations

//## begin module%38D9EC8E00EA.additionalDeclarations preserve=yes
//## end module%38D9EC8E00EA.additionalDeclarations


// Class ClCachesSubsystem 

ClCachesSubsystem::ClCachesSubsystem (ClConfiguration *ai_poConfiguration,  ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClCachesSubsystem::ClCachesSubsystem%895476048.hasinit preserve=no
  //## end ClCachesSubsystem::ClCachesSubsystem%895476048.hasinit
  //## begin ClCachesSubsystem::ClCachesSubsystem%895476048.initialization preserve=yes
  : ClSSImplementation ( ai_poConfiguration,  ai_poTables , ai_poNavigator)
  //## end ClCachesSubsystem::ClCachesSubsystem%895476048.initialization
{
  //## begin ClCachesSubsystem::ClCachesSubsystem%895476048.body preserve=yes
  //## end ClCachesSubsystem::ClCachesSubsystem%895476048.body
}


ClCachesSubsystem::~ClCachesSubsystem ()
{
  //## begin ClCachesSubsystem::~ClCachesSubsystem%895476049.body preserve=yes
	if ( m_poNavigator )
		delete m_poNavigator ;

  //## end ClCachesSubsystem::~ClCachesSubsystem%895476049.body
}



//## Other Operations (implementation)
m4return_t ClCachesSubsystem::GetCommandsId (M4DataStorage *ao_poDataStorage)
{
  //## begin ClCachesSubsystem::GetCommandsId%895476025.body preserve=yes

	return TOTAL_COMMANDS ;
  //## end ClCachesSubsystem::GetCommandsId%895476025.body
}

m4return_t ClCachesSubsystem::ShutDown ()
{
  //## begin ClCachesSubsystem::ShutDown%895476021.body preserve=yes
	ClSSImplementation::ShutDown() ;
	return M4_SUCCESS ;
  //## end ClCachesSubsystem::ShutDown%895476021.body
}

m4return_t ClCachesSubsystem::Synchronize ()
{
  //## begin ClCachesSubsystem::Synchronize%895476022.body preserve=yes
	ClSSImplementation::Synchronize() ;

	return M4_SUCCESS ;
  //## end ClCachesSubsystem::Synchronize%895476022.body
}

m4return_t ClCachesSubsystem::Initialize ()
{
  //## begin ClCachesSubsystem::Initialize%895476023.body preserve=yes
	eStatus eState ;

	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	m_poVisitor = new ClCachesVisitor ;

	m_poStatistics = new ClCachesStats ;

	if (InitializeAttributes () == M4_ERROR) {
		return M4_ERROR;
	}

	return M4_SUCCESS ;
  //## end ClCachesSubsystem::Initialize%895476023.body
}

m4return_t ClCachesSubsystem::ExecuteCommand (m4char_t ai_cCommand, M4DataStorage *ai_poParams, M4DataStorage *ai_poResult)
{
  //## begin ClCachesSubsystem::ExecuteCommand%895476024.body preserve=yes
	return M4_SUCCESS ;
  //## end ClCachesSubsystem::ExecuteCommand%895476024.body
}

m4return_t ClCachesSubsystem::InitializeAttributes ()
{
  //## begin ClCachesSubsystem::InitializeAttributes%1085388500.body preserve=yes

	SetConfigStringProperty	( ACTION_CLEAN_ALL_CACHES ,  "0" );
	m4date_t dDate;
	m4char_t aDate[DEF_STRING_SIZE + 1];
	ClActualDate (dDate);
	ClDateToString (dDate, aDate, DEF_STRING_SIZE + 1);
	SetConfigStringProperty	(LAST_CLEAN_ALL_CACHES , aDate);

	return M4_SUCCESS;
  //## end ClCachesSubsystem::InitializeAttributes%1085388500.body
}

ClCacheTest * ClCachesSubsystem::GetCache (m4pchar_t ai_szName)
{
  //## begin ClCachesSubsystem::GetCache%895476054.body preserve=yes
	ClCacheTest * poCacheTest ;


	poCacheTest = ( ClCacheTest *) m_poNavigator -> GetSubsystemElement ( ai_szName , "CACHE" ) ;

	return poCacheTest ;
  //## end ClCachesSubsystem::GetCache%895476054.body
}

m4return_t ClCachesSubsystem::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClCachesSubsystem::ExecuteCommand%910862274.body preserve=yes
	m4return_t iRet ;
	ClAdminTagList::iterator it;

	switch ( ai_cCommand )
	{

	case CLEAN_ALL_CACHES :

		{
			m4bool_t bFlushed = M4_FALSE;
			it = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));
			//Escenarios:
			//Caso1: Si no vienen argumentos es pq desde el monitor deseamos hacer el clean.
			//Caso2: Si viene un argumento y es 1 es pq desde SNMP deseamos hacer clean.
			//Si no hay argumentos es pq estamos ejecutando el comando desde el monitor.
			if (it == ai_oParamList.end()) {
				iRet = _CleanAllCaches ();
				bFlushed = M4_TRUE;
			}
			else {
				//Si hay argumentos tenemos que comprobar el valor. El clean sólo se realiza
				//si indicamos un 1.
				m4pchar_t pcFlush = (m4pchar_t)((*it)->m_tagValue);
				if (pcFlush != NULL) {
					if (!strcmp(pcFlush, "1")) {
						iRet = _CleanAllCaches ();
						bFlushed = M4_TRUE;
					}
					else {
						//Si es 0 no hacemos nada.
						if (!strcmp(pcFlush, "0")) {
							iRet = M4_SUCCESS;
						}
						else {
							//Si no es ni 1 ni 0 damos error.
							iRet = M4_ERROR;
						}
					}
				}
				else {
					//El argumento no puede ser nulo.
					iRet = M4_ERROR;
				}
			}

			// Se borran también los literales
			M4CleanAllLiterals();

			SetConfigStringProperty	( ACTION_CLEAN_ALL_CACHES ,  "0" );
			if (bFlushed == M4_TRUE) {
				m4date_t dDate;
				m4char_t aDate[DEF_STRING_SIZE + 1];
				ClActualDate (dDate);
				ClDateToString (dDate, aDate, DEF_STRING_SIZE + 1);				
				SetConfigStringProperty	(LAST_CLEAN_ALL_CACHES , aDate);
			}
			break;
		}

	default :

		return M4_ERROR ;
	}
	
	return iRet ;
  //## end ClCachesSubsystem::ExecuteCommand%910862274.body
}

m4return_t ClCachesSubsystem::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClCachesSubsystem::GetCommandsId%910862275.body preserve=yes
	ClAdminTag *newCommand;

	ClSSImplementation::GetCommandsId ( aio_oCommandList ) ;

	newCommand = ClAdminTag::GetNewAdminTag(CLEAN_ALL_CACHES , "CLEAN_ALL_CACHES", M4_FALSE);

	if (newCommand == NULL) return M4_ERROR;
	aio_oCommandList.push_back(newCommand);
	

	return M4_SUCCESS;
  //## end ClCachesSubsystem::GetCommandsId%910862275.body
}

m4return_t ClCachesSubsystem::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oCommnadList)
{
  //## begin ClCachesSubsystem::GetCommandParams%910862276.body preserve=yes
	return M4_SUCCESS;
  //## end ClCachesSubsystem::GetCommandParams%910862276.body
}

m4return_t ClCachesSubsystem::GetCommandParams (m4char_t ai_cCommand, M4DataStorage *ao_poDataStorage)
{
  //## begin ClCachesSubsystem::GetCommandParams%910862283.body preserve=yes
	return M4_SUCCESS;
  //## end ClCachesSubsystem::GetCommandParams%910862283.body
}

m4return_t ClCachesSubsystem::Start ()
{
  //## begin ClCachesSubsystem::Start%958548531.body preserve=yes
	return ClSSImplementation::Start();
  //## end ClCachesSubsystem::Start%958548531.body
}

// Additional Declarations
  //## begin ClCachesSubsystem%355FDE2A03AF.declarations preserve=yes
m4return_t ClCachesSubsystem::_CleanAllCaches () 
{
	INTERFACESMAP::iterator Iterator ;
	ClSSInterfaz * poInterfaz ;
	m4char_t szId [10] ;
	ClAdminTagList  oParams ;
	m4return_t iRet ;


	for ( Iterator = m_oBellowLevel.begin() ; Iterator != m_oBellowLevel.end() ; Iterator ++ )
	{
		poInterfaz = (*Iterator).second ;

		sprintf ( szId , "%d" , (m4uint32_t) poInterfaz -> GetId () ) ;

		iRet = poInterfaz -> ExecuteCommand ( szId , ClCacheSubSystem::CLEAN_CACHE , oParams , oParams ) ;
	}


	return M4_SUCCESS ;
}
  //## end ClCachesSubsystem%355FDE2A03AF.declarations
// Class ClCachesStats 


//## Other Operations (implementation)
m4return_t ClCachesStats::Reset ()
{
  //## begin ClCachesStats::Reset%895476045.body preserve=yes
	return M4_SUCCESS ;
  //## end ClCachesStats::Reset%895476045.body
}

m4return_t ClCachesStats::GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_poData)
{
  //## begin ClCachesStats::GetStat%895476046.body preserve=yes
	return M4_SUCCESS ;
  //## end ClCachesStats::GetStat%895476046.body
}

m4return_t ClCachesStats::ResetStat (ClStatHandle &ai_hStatId)
{
  //## begin ClCachesStats::ResetStat%895476047.body preserve=yes
	return M4_SUCCESS ;
  //## end ClCachesStats::ResetStat%895476047.body
}

// Additional Declarations
  //## begin ClCachesStats%355FDE470357.declarations preserve=yes
  //## end ClCachesStats%355FDE470357.declarations

// Class ClCachesVisitor 


//## Other Operations (implementation)
void ClCachesVisitor::VisitCaches (ClCachesSubsystem &ai_oCaches)
{
  //## begin ClCachesVisitor::VisitCaches%895476020.body preserve=yes
  //## end ClCachesVisitor::VisitCaches%895476020.body
}

// Additional Declarations
  //## begin ClCachesVisitor%355FDE7F00AF.declarations preserve=yes
  //## end ClCachesVisitor%355FDE7F00AF.declarations

//## begin module%38D9EC8E00EA.epilog preserve=yes
//## end module%38D9EC8E00EA.epilog

//## begin module%34CE2CC6012B.cm preserve=no
//## end module%34CE2CC6012B.cm

//## begin module%34CE2CC6012B.cp preserve=no
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
//## end module%34CE2CC6012B.cp

//## Module: LDBSubsystem%34CE2CC6012B; Package body
//## Subsystem: M4LDBSubsystem::src%379D76DE0186
//## Source file: b:\M4LDBSubsy\src\ldbsubsystem.cpp

//## begin module%34CE2CC6012B.additionalIncludes preserve=no
//## end module%34CE2CC6012B.additionalIncludes

//## begin module%34CE2CC6012B.includes preserve=yes
#include  <m4srvres.hpp>
#include <m4ldb.hpp>
#include "m4log.hpp"
#include "clfactm4objservice.hpp"
#include "m4ldb_notif.hpp"
#include "blocksynchronization.hpp"
//## end module%34CE2CC6012B.includes

// Statistics
#include <statistics.hpp>
// LDBSubsystem
#include <ldbsubsystem.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>
// m4notifier
#include <m4notifier.hpp>
//## begin module%34CE2CC6012B.declarations preserve=no
//## end module%34CE2CC6012B.declarations

//## begin module%34CE2CC6012B.additionalDeclarations preserve=yes
static ClMutex s_oLDBCSEvent (M4_TRUE);
//## end module%34CE2CC6012B.additionalDeclarations


// Class ClLDBVisitor 


//## Other Operations (implementation)
void ClLDBVisitor::VisitLDB (void *ai_oLDB)
{
  //## begin ClLDBVisitor::VisitLDB%885927764.body preserve=yes
  //## end ClLDBVisitor::VisitLDB%885927764.body
}

// Additional Declarations
  //## begin ClLDBVisitor%34CE2A7B02CC.declarations preserve=yes
  //## end ClLDBVisitor%34CE2A7B02CC.declarations

// Class ClLDBSubsystem 



ClLDBSubsystem::ClLDBSubsystem (ClConfiguration *ai_poObl, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClLDBSubsystem::ClLDBSubsystem%885977163.hasinit preserve=no
  //## end ClLDBSubsystem::ClLDBSubsystem%885977163.hasinit
  //## begin ClLDBSubsystem::ClLDBSubsystem%885977163.initialization preserve=yes
  :ClSSImplementation  ( ai_poObl , ai_poTables, ai_poNavigator)
  //## end ClLDBSubsystem::ClLDBSubsystem%885977163.initialization
{
  //## begin ClLDBSubsystem::ClLDBSubsystem%885977163.body preserve=yes
	m_nRetry = -1;
  //## end ClLDBSubsystem::ClLDBSubsystem%885977163.body
}


ClLDBSubsystem::~ClLDBSubsystem ()
{
  //## begin ClLDBSubsystem::~ClLDBSubsystem%885977164.body preserve=yes
  //## end ClLDBSubsystem::~ClLDBSubsystem%885977164.body
}



//## Other Operations (implementation)
m4return_t ClLDBSubsystem::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClLDBSubsystem::ExecuteCommand%885927759.body preserve=yes

	ILdb *pLDB = ClFactM4ObjService::GetpLDB();
	if (pLDB == NULL ) 
		return M4_ERROR;

	ClAdminTagList::iterator it;
	m4return_t iRet=M4_ERROR;

	switch(ai_cCommand)
	{
//	case FLUSH_SENTENCE_CACHE:
		
//			iRet= pLDB->FlushSentenceCache();
					
//			break;

	case SET_DEBUG_LEVEL:

		 
			m4int32_t ulDebugLevel;

			it=find_if (ai_oParamList.begin(),ai_oParamList.end(),search_by_tag_id(DEBUG_LEVEL));
			
			if(it!=ai_oParamList.end())
			{				
				iRet = M4_SUCCESS;
				
				if		(!strcmp((*it)->m_tagValue,"0"))
					ulDebugLevel = 0;
				else if (!strcmp((*it)->m_tagValue,"1"))
					ulDebugLevel = 1;
				else if (!strcmp((*it)->m_tagValue,"2"))
					ulDebugLevel = 2;
				else if (!strcmp((*it)->m_tagValue,"3"))
					ulDebugLevel = 3;
				else {
					iRet=M4_ERROR;
					SETCODEF(M4_SRV_SS_COMMAND_PARAM_ERROR , ERRORLOG , "Invalid LDB debug level value:%s, valid range is 0-3", (*it)->m_tagValue);
					break;
				}
			}

			if (iRet == M4_ERROR) {
			
				SETCODEF(M4_SRV_SS_COMMAND_PARAM_ERROR , ERRORLOG , "Invalid LDB debug level param");

			} else
				iRet= pLDB->SetSysDebugLevel(ulDebugLevel);

			SetConfigStringProperty	( LDB_DEBUG_LEVEL , (m4pchar_t)pLDB -> GetDebugLevel() );


			break;

	//Detalle de trazas en ldbinsp.
	case SET_DEBUG_DETAIL_LEVEL:
		 
			m4int32_t ulDebugDetailLevel;

			it=find_if (ai_oParamList.begin(),ai_oParamList.end(),search_by_tag_id(DEBUG_DETAIL_LEVEL));
			
			if(it!=ai_oParamList.end())
			{				
				iRet = M4_SUCCESS;
				
				if		(!strcmp((*it)->m_tagValue,"0"))
					ulDebugDetailLevel = 0;
				else if (!strcmp((*it)->m_tagValue,"1"))
					ulDebugDetailLevel = 1;
				else if (!strcmp((*it)->m_tagValue,"2"))
					ulDebugDetailLevel = 2;
				else if (!strcmp((*it)->m_tagValue,"3"))
					ulDebugDetailLevel = 3;
				else if (!strcmp((*it)->m_tagValue,"4"))
					ulDebugDetailLevel = 4;
				else {
					iRet=M4_ERROR;
					SETCODEF(M4_SRV_SS_COMMAND_PARAM_ERROR , ERRORLOG , "Invalid LDB debug detail level value:%s, valid range is 0-4", (*it)->m_tagValue);
					break;
				}
			}

			if (iRet == M4_ERROR) {
			
				SETCODEF(M4_SRV_SS_COMMAND_PARAM_ERROR , ERRORLOG , "Invalid LDB debug detail level param");

			} else
				iRet= pLDB->SetSystemDebugDetailLevel(ulDebugDetailLevel);

			SetConfigStringProperty	( LDB_DEBUG_DETAIL_LEVEL , (m4pchar_t)pLDB -> GetStrSystemDebugDetailLevel() );

			break;

	case FLUSH_DICTIONARY_CACHE:
		{
			m4bool_t bFlushed = M4_FALSE;
			it = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));
			//Escenarios:
			//Caso1: Si no vienen argumentos es pq desde el monitor deseamos hacer el flush.
			//Caso2: Si viene un argumento y es 1 es pq desde SNMP deseamos hacer flush.
			//Si no hay argumentos es pq estamos ejecutando el comando desde el monitor.
			if (it == ai_oParamList.end()) {
				iRet= pLDB->ClearAll();
				bFlushed = M4_TRUE;
			}
			else {
				//Si hay argumentos tenemos que comprobar el valor. El flush sólo se realiza
				//si indicamos un 1.				
				m4pchar_t pcFlush = (m4pchar_t)((*it)->m_tagValue);
				if (pcFlush != NULL) {
					if (!strcmp(pcFlush, "1")) {
						iRet = pLDB->ClearAll();
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

			SetConfigStringProperty	( ACTION_FLUSH_DICTIONARY_CACHE ,  "0" );
			if (bFlushed == M4_TRUE) {
				m4date_t dDate;
				m4char_t aDate[M4LDB_SIZE_TIMESTAMP + 1];
				ClActualDate (dDate);
				ClDateToString (dDate, aDate, M4LDB_SIZE_TIMESTAMP + 1);
				SetConfigStringProperty	(LAST_FLUSH_DICTIONARY_CACHE , aDate);
			}
			break;

		}

	case RETRY_CONNECTION:
			if ((m_vcEvent.empty ()==0) && 
				(m_nRetry != -2)) {

				if (m_nRetry == -1) {
					if (GetConfigNumericValue (LDB_ATTEMPTS_NUMBER , m_nRetry) == M4_ERROR) {
						// Por defecto.
						m_nRetry = 100;
					}
					if (m_nRetry < 1) {
						// Reintento como mínimo una vez.
						m_nRetry = 1;
					}
				}

				if (m_nRetry != 0) {
					iRet = RetryConnection ();
					m_nRetry--;
				}
				else {
					ClSSImplementation::Notify (LDB_SHUTDOWN);
					// Deshabilitamos el reintento.
					m_nRetry = -2;
					iRet = M4_SUCCESS;
				}
			}
			break;

	default:

		SETCODEF ( M4_SRV_SS_COMMAND_PARAM_ERROR , ERRORLOG , "Invalid Administrator List SubSystem command Id: %d", ai_cCommand );

		iRet=M4_ERROR;
		
	}


	return iRet;
		
  //## end ClLDBSubsystem::ExecuteCommand%885927759.body
}

m4return_t ClLDBSubsystem::Synchronize ()
{
  //## begin ClLDBSubsystem::Synchronize%885927760.body preserve=yes
	return M4_SUCCESS;
  //## end ClLDBSubsystem::Synchronize%885927760.body
}

m4return_t ClLDBSubsystem::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClLDBSubsystem::GetCommandsId%885927761.body preserve=yes

	ClAdminTag *newCommand;

/*	En el FLUSH_DICTIONARY_CACHE se borra todo
	newCommand = ClAdminTag::GetNewAdminTag(FLUSH_SENTENCE_CACHE,"FLUSH_SENTENCE_CACHE",M4_FALSE);
	M4_ASSERT(newCommand!=NULL);
	if (newCommand==NULL)return M4_ERROR;
	aio_oCommandList.push_back(newCommand);*/ 
/*
	newCommand = ClAdminTag::GetNewAdminTag(SET_DEBUG_LEVEL,"SET_DEBUG_LEVEL",M4_TRUE);
//	M4_ASSERT(newCommand!=NULL);
//	if (newCommand==NULL)return M4_ERROR;
//	aio_oCommandList.push_back(newCommand);
	if (newCommand) aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(SET_DEBUG_DETAIL_LEVEL,"SET_DEBUG_DETAIL_LEVEL",M4_TRUE);
	if (newCommand) aio_oCommandList.push_back(newCommand);
*/
	newCommand = ClAdminTag::GetNewAdminTag(FLUSH_DICTIONARY_CACHE,"FLUSH_DICTIONARY_CACHE",M4_FALSE);
//	M4_ASSERT(newCommand!=NULL);
//	if (newCommand==NULL)return M4_ERROR;
//	aio_oCommandList.push_back(newCommand);
	if (newCommand) aio_oCommandList.push_back(newCommand);
	
	newCommand = ClAdminTag::GetNewAdminTag(RETRY_CONNECTION,"RETRY_CONNECTION",M4_FALSE);
//	M4_ASSERT(newCommand!=NULL);
//	if (newCommand==NULL)return M4_ERROR;
//	aio_oCommandList.push_back(newCommand);
	if (newCommand) aio_oCommandList.push_back(newCommand);
	
	return M4_SUCCESS;

  //## end ClLDBSubsystem::GetCommandsId%885927761.body
}

m4return_t ClLDBSubsystem::Initialize ()
{
  //## begin ClLDBSubsystem::Initialize%885927762.body preserve=yes
	m_poVisitor=new ClLDBVisitor;

	if (NULL==m_poVisitor)
	{
		SETCODEF(M4_ERR_LDBSUB_POINTERNULL,ERRORLOG,"Pointer null.Pointer:<m_poVisitor>");
		return M4_ERROR;
	}
	//m_poStatistics=new ClLDBStats;OJO cuando tengamos estadísticas hay que descomentarlo

	// Inicializamos las propiedades del Subsistema.

	if (InitializeAttributes () == M4_ERROR) {
	
		SETCODEF(M4_ERR_LDBSUB_INITLDB,ERRORLOG,"Can`t init LDBSubsystem properties");
	
		return M4_ERROR;
	}
	
	return M4_SUCCESS;

  //## end ClLDBSubsystem::Initialize%885927762.body
}

m4return_t ClLDBSubsystem::ShutDown ()
{
  //## begin ClLDBSubsystem::ShutDown%885927763.body preserve=yes
	if (m_poVisitor!=NULL)
	{
		delete m_poVisitor;

		m_poVisitor = NULL ;
	}

	//delete m_poStats;OJO , cuando tengamos estadísticas hay que descomentarlo

	/* 
	//DE MOMENTO NO SE CORRIGE haría falta una funcion en la Oeinit que me diera el master. Ademas, es necesario
	//que tb se haga un shutdown ordenado del resto de servicios y actualmente no se está haciendo!!.

	//Bug 0076390
	//Ojo!!. Se asume que lo último que se destruye es la LDB, aquí nos estamos cargando la LDB (que es el Master en
	//la Oeinit).	
	ClM4ObjService * pM4ObjService = NULL;
	pM4ObjService = ClFactM4ObjService::GetpM4ObjServiceMaster ();
	if (pM4ObjService) {
		ClFactM4ObjService::DestroyObjService (pM4ObjService);
		//Si pM4ObjService es NULL es pq se ha podido destuir!.
	}
	*/

	return M4_SUCCESS;

  //## end ClLDBSubsystem::ShutDown%885927763.body
}

m4return_t ClLDBSubsystem::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
{
  //## begin ClLDBSubsystem::GetCommandParams%891421985.body preserve=yes
//	ClAdminCommandTag *newParam;
	
	switch(ai_cCommand)
	{
//	case FLUSH_SENTENCE_CACHE:
//		break;
/*
	case SET_DEBUG_LEVEL:

		newParam= ClAdminCommandTag::GetNewAdminCommandTag(DEBUG_LEVEL,"DEBUG_LEVEL",adminLong,"0");
//		M4_ASSERT(newParam!=NULL);
//		if (newParam==NULL)return M4_FALSE;
//		aio_oParamList.push_back(newParam);
		if (newParam) aio_oParamList.push_back(newParam);
		
		break;

	case SET_DEBUG_DETAIL_LEVEL:

		newParam= ClAdminCommandTag::GetNewAdminCommandTag(DEBUG_DETAIL_LEVEL,"DEBUG_DETAIL_LEVEL",adminLong,"0");
		if (newParam) aio_oParamList.push_back(newParam);
		
		break;
*/
	case FLUSH_DICTIONARY_CACHE:
	case RETRY_CONNECTION:
		break;
	
	default:
		M4_ASSERT(" Cache command not recognized " != NULL);
		break;
	}

	return  M4_SUCCESS;

  //## end ClLDBSubsystem::GetCommandParams%891421985.body
}

void ClLDBSubsystem::Update (EVENTTYPE ai_iEvent, ClServerEvent *ai_oSubject)
{
  //## begin ClLDBSubsystem::Update%919863071.body preserve=yes

	ILdb *pLDB = ClFactM4ObjService::GetpLDB();
	if (pLDB == NULL ) {
		return;
	}

	//Cuando un usuario se desloga se envía un evento, desde m4comserv, de CLOSE_SESSION a todos los subsistemas.
	//El proceso es mediante Notify(CLOSE_SESSION), El Notify invoca al metodo Update para cada subsistema.
	m4uint64_t iSessionId = 0;

	switch (ai_iEvent )
	{
		case CLOSE_SESSION:
			iSessionId = ((ClSSImplementation*) ai_oSubject)->GetId();
			#ifdef _DEBUG
				//Ojo, hacemos el casting sólo para mostrar el valor.
				cout << "Warning: LDBSubsytem: desconnection event for session id " << (m4uint32_t) iSessionId << endl;
			#endif
			pLDB->UserSessionClosedByIdSession( (m4uint64_t) iSessionId );
			break;
		default:
			return;
	}

  //## end ClLDBSubsystem::Update%919863071.body
}

m4return_t ClLDBSubsystem::InitializeAttributes ()
{
  //## begin ClLDBSubsystem::InitializeAttributes%939655437.body preserve=yes
 
	ILdb *pLDB = ClFactM4ObjService::GetpLDB();
	if (pLDB == NULL ) {
		return M4_ERROR;
	}

	pLDB->Attach_DBRyptureConnection (this);

	SetConfigStringProperty  ( LDB_CONNECTION_STRING , (m4pchar_t)(pLDB -> GetDictConnStr()) );
	
	SetConfigNumericProperty ( LDB_MAX_NUM_CONN , pLDB -> GetNumConn() );

	SetConfigNumericProperty ( LDB_MAX_NUM_CURSOR , pLDB -> GetNumCursor() );

	m4pchar_t pcConnExec = (m4pchar_t)(pLDB -> GetConExec());

	if ((pcConnExec==NULL)||(!strcmp(pcConnExec,""))) 
		SetConfigStringProperty  ( LDB_CON_EXEC , "No SQL sentence");
	else 
		SetConfigStringProperty  ( LDB_CON_EXEC , pcConnExec );

	SetConfigNumericProperty ( LDB_MAX_NUM_STATMNT_CACHE , pLDB -> GetNumStmtCache() );

	SetConfigNumericProperty ( LDB_MAX_NUM_LOGIC_OBJECT , pLDB -> GetNumObject() );

	SetConfigStringProperty ( LDB_ISOLATION_LEVEL , (m4pchar_t)pLDB -> GetIsolationLevel() );

	SetConfigStringProperty ( LDB_DATABASE_TYPE , (m4pchar_t)pLDB -> GetDataBaseType() );

	SetConfigStringProperty	( LDB_DEBUG_LEVEL , (m4pchar_t)pLDB -> GetDebugLevel() );

	SetConfigStringProperty	( LDB_DEBUG_DETAIL_LEVEL , (m4pchar_t)pLDB -> GetStrSystemDebugDetailLevel() );

	SetConfigNumericProperty ( LDB_NUM_CONN_IN_USE , 0 );

	SetConfigNumericProperty ( LDB_NUM_LOGIC_OBJECT_IN_USE , 0 );

	SetConfigNumericProperty ( LDB_NUM_STATMNT_IN_USE , 0 );

	SetConfigStringProperty	( ACTION_FLUSH_DICTIONARY_CACHE ,  "0" );
	m4date_t dDate;
	m4char_t aDate[M4LDB_SIZE_TIMESTAMP + 1];
	ClActualDate (dDate);
	ClDateToString (dDate, aDate, M4LDB_SIZE_TIMESTAMP + 1);
	SetConfigStringProperty	(LAST_FLUSH_DICTIONARY_CACHE , aDate);

	return M4_SUCCESS;
	
	
  //## end ClLDBSubsystem::InitializeAttributes%939655437.body
}

m4return_t ClLDBSubsystem::GetSubsystemAttribValue (m4uint16_t ai_iAttribId, M4DataStorage *ao_poDataStorage)
{
  //## begin ClLDBSubsystem::GetSubsystemAttribValue%941557940.body preserve=yes
	ILdb *pLDB = ClFactM4ObjService::GetpLDB();
	if (pLDB == NULL ) 
		return M4_ERROR;
	
	SetConfigNumericProperty ( LDB_NUM_CONN_IN_USE , pLDB -> GetNumConnInUse() );

	SetConfigNumericProperty ( LDB_NUM_LOGIC_OBJECT_IN_USE , pLDB -> GetNumLogicObjectInUse() );

	SetConfigNumericProperty ( LDB_NUM_STATMNT_IN_USE , pLDB -> GetNumStatmntCacheInUse() );
	
	SetConfigStringProperty	( LDB_DEBUG_LEVEL , (m4pchar_t)pLDB -> GetDebugLevel() );

	SetConfigStringProperty	( LDB_DEBUG_DETAIL_LEVEL , (m4pchar_t)pLDB -> GetStrSystemDebugDetailLevel() );

	return ClSSImplementation::GetSubsystemAttribValue (ai_iAttribId, ao_poDataStorage);
  //## end ClLDBSubsystem::GetSubsystemAttribValue%941557940.body
}

m4return_t ClLDBSubsystem::RetryConnection ()
{
  //## begin ClLDBSubsystem::RetryConnection%958495555.body preserve=yes
//	DUMP_LDBINSP_WARNING (m_pGlobVars->GetpLConn (), M4_CH_LDB_SHUTDOWN_REQUEST);

	ClMutBlock oLDBCS (s_oLDBCSEvent);

	for (VcClEventIntInt::iterator it=m_vcEvent.begin (); it!=m_vcEvent.end (); it++) {
		ILdb *pLDB = ClFactM4ObjService::GetpLDB();
		if (pLDB == NULL ) {
			return M4_ERROR;
		}
		if (pLDB->RefreshInvalidConnections((*it).m_oArg1,(*it).m_oArg2) == M4_ERROR) {
			return (M4_ERROR);
		}
	}

	m_vcEvent.clear ();

	m_nRetry = -1;

	return (M4_SUCCESS);
  //## end ClLDBSubsystem::RetryConnection%958495555.body
}

m4bool_t ClLDBSubsystem::Notify (ClEvent &ai_oEvent)
{
  //## begin ClLDBSubsystem::Notify%958495556.body preserve=yes
//	ClMutBlock oLDBCS (s_oLDBCSEvent);

	switch(ai_oEvent.GetId ()){
	case M4_NOTIF_DB_RYPTURE_CONNECTION:
		// El evento es del tipo ClEventIntInt.

		s_oLDBCSEvent.Lock () ;
		m_vcEvent.push_back ((ClEventIntInt &)ai_oEvent);
		s_oLDBCSEvent.Unlock () ;

		ClServerEvent::Notify ( LDB_SHUTDOWN ) ;
		break;
	case M4_NOTIF_RETRY_CONNECTION:

		//Bug 0090288, 0092128
		s_oLDBCSEvent.Lock () ;
		m_vcEvent.push_back ((ClEventIntInt &)ai_oEvent);
		s_oLDBCSEvent.Unlock () ;

		RetryConnection	();
		break;
	default:
		break;
	}
	// Devolvemos FALSE porque no queremos que nos saquen de la lista.
	return (M4_FALSE);
  //## end ClLDBSubsystem::Notify%958495556.body
}

// Additional Declarations
  //## begin ClLDBSubsystem%34CDDADC00B2.declarations preserve=yes
  //## end ClLDBSubsystem%34CDDADC00B2.declarations

//## begin module%34CE2CC6012B.epilog preserve=yes
/*
#include "cldefine.hpp"  // include de chlib
#include "regedit.hpp"  // include de chlib
#include "m4objreg.hpp"  // include de chlib

m4return_t getDataBaseUserAndServerFromRegistry(char *ai_pcUser, char *ai_pcDataBaseServer)
{
	//
	// leemos la connection string del registro o del fichero regmeta4.reg
	char cConnectionString[1024];
	if (GetRegKeyData(M4_REGEDIT_KEY, M4_INIT_CONNECTION_STRING, cConnectionString) == FALSE)
    {
		return M4_ERROR;
	}


	// la connection_string es algo como:
	// DSN=ORA32_INT_01;SRVR=ATENEATCP;UID=testuser12'PWD=testuser12
	// de aqui sacamos el usuario y el servidor.
	const char *separators = "=;";
	char *cDSN, *cSRVR, *cUID;

	cDSN = strtok(cConnectionString, separators);
	cDSN = strtok(NULL, separators);
	cSRVR = strtok(NULL, separators);
	cSRVR = strtok(NULL, separators);
	cUID = strtok(NULL, separators);
	cUID = strtok(NULL, separators);

	if ((NULL == cSRVR) || (NULL == cUID))
	{
		return M4_ERROR;
	}
	else
	{
		strcpy (ai_pcUser, cUID);
		strcpy (ai_pcDataBaseServer, cSRVR);
		
		return M4_SUCCESS;
	}
}
*/
//## end module%34CE2CC6012B.epilog

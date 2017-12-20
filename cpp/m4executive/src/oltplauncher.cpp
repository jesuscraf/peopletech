//## begin module%354838040397.cm preserve=no
//## end module%354838040397.cm

//## begin module%354838040397.cp preserve=no
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
//## end module%354838040397.cp

//## Module: OLTPLauncher%354838040397; Package body
//## Subsystem: M4Executive::src%379462FF0256
//## Source file: F:\integration\m4executive\src\oltplauncher.cpp

//## begin module%354838040397.additionalIncludes preserve=no
//## end module%354838040397.additionalIncludes

//## begin module%354838040397.includes preserve=yes
//	Body include en rose98
#include <clconfiguration.hpp>
#include <request.hpp>
#include <conninstance.hpp>
#include <m4exception.hpp>
#include <m4compress.hpp>
#include <subsystemids.hpp>
#include <m4srvres.hpp>
#include <ssnames.hpp>
#include <compressdatastorage.hpp>
#include <m4actionconstants.hpp>
//## end module%354838040397.includes

// servicepdus
#include <servicepdus.hpp>
// USInterface
#include <usinterface.hpp>
// LauncherInterface
#include <launcher.hpp>
// OLTPLauncher
#include <oltplauncher.hpp>
// Executive
#include <executive.hpp>
// queuebasedss
#include <queuebasedss.hpp>

class ClNavigatorFirewall;

//## begin module%354838040397.declarations preserve=no
//## end module%354838040397.declarations

//## begin module%354838040397.additionalDeclarations preserve=yes
//## end module%354838040397.additionalDeclarations


// Class ClLauncherForOLTP 



ClLauncherForOLTP::ClLauncherForOLTP (ClExecutive *ai_pExecutive, ClStatistic *ai_poStats, ClQueueBasedSS *ai_poServiceSS)
  //## begin ClLauncherForOLTP::ClLauncherForOLTP%893925593.hasinit preserve=no
  //## end ClLauncherForOLTP::ClLauncherForOLTP%893925593.hasinit
  //## begin ClLauncherForOLTP::ClLauncherForOLTP%893925593.initialization preserve=yes
  :	ClLauncherInterface(ai_pExecutive,ai_poStats, NULL)
  //## end ClLauncherForOLTP::ClLauncherForOLTP%893925593.initialization
{
  //## begin ClLauncherForOLTP::ClLauncherForOLTP%893925593.body preserve=yes
	m_poServiceSS = ai_poServiceSS ;
  //## end ClLauncherForOLTP::ClLauncherForOLTP%893925593.body
}


ClLauncherForOLTP::~ClLauncherForOLTP ()
{
  //## begin ClLauncherForOLTP::~ClLauncherForOLTP%893925594.body preserve=yes
	if (M4_TRUE == m_bInitialized)
		Destroy();
  //## end ClLauncherForOLTP::~ClLauncherForOLTP%893925594.body
}



//## Other Operations (implementation)
m4return_t ClLauncherForOLTP::Destroy ()
{
  //## begin ClLauncherForOLTP::Destroy%894305680.body preserve=yes
	if (M4_FALSE == m_bInitialized)
		return M4_ERROR;


	if (NULL != m_pExecutive)
	{
		if ( ClExecutive::STATE_HALTED != m_pExecutive -> GetOwnState ( ) )
		{
			m_pExecutive->ShutDown();
		}
	}

	return M4_SUCCESS;
  //## end ClLauncherForOLTP::Destroy%894305680.body
}

m4return_t ClLauncherForOLTP::_Dump ()
{
  //## begin ClLauncherForOLTP::_Dump%921058930.body preserve=yes
	SetUserHandler(&LogCallback, "Fatal Error trying to dump Info!!!");
	return ClLauncherInterface::_Dump();
  //## end ClLauncherForOLTP::_Dump%921058930.body
}

m4return_t ClLauncherForOLTP::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClLauncherForOLTP::ExecuteCommand%922189094.body preserve=yes

	if (M4_TRUE == IsExit())
		return M4_ERROR;

	if ( ClExecutive::STATE_HALTED == m_pExecutive -> GetOwnState ( ) )
	{
		return M4_ERROR ;
	}

	m4return_t iResult = M4_ERROR;
	
	switch (ai_cCommand)
	{
	case QUIT:
	case PAUSE:
	case RESUME:
	case USED_MEMORY:
		if (m_pExecutive)
			iResult = m_pExecutive->ExecuteCommand(ai_cCommand, ai_oParamList, ao_oResultList);
		break ;
		
	default:
		SETCODEF ( M4_SRV_SS_COMMAND_PARAM_ERROR , ERRORLOG , "OLTP Launcher: Invalid Administrator List SubSystem command Id: %d", ai_cCommand );
		return M4_ERROR;
	}
	
	return iResult;

  //## end ClLauncherForOLTP::ExecuteCommand%922189094.body
}

m4return_t ClLauncherForOLTP::_GetNextRequest (ClBaseRequest *&ao_poRequest)
{
  //## begin ClLauncherForOLTP::_GetNextRequest%943284073.body preserve=yes


	if ( ClExecutive::STATE_HALTED == m_pExecutive -> GetOwnState ( ) )
	{
		return M4_ERROR ;
	}

	if (NULL == m_poServiceSS)
	{
		SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "Executor has not been initialized properly");
		return M4_ERROR;
	}
	
	ClServiceItem		poItem;
	ClRequest			*poRequest;
	m4return_t			iRet;
	m4bool_t			bIsReadyToRun;

	do
	{
		do
		{
/*			if (M4_TRUE == m_pServiceQ->IsLocked())
			{
				SETCODEF ( M4_LOCKED_QUEUE, ERRORLOG , "#*s1*#");
				return M4_ERROR;
			}
*/
			iRet = m_poServiceSS ->Pop(poItem);

			if ( IsTerminate() )
				return M4_ERROR;

			if (M4_ERROR == iRet)
			{
				SETCODEF ( M4_LOCKED_QUEUE, ERRORLOG , "#*s1*#");
				return M4_ERROR;
			}

		}
		while ( poItem == NULL );

		bIsReadyToRun = M4_TRUE;

		poRequest = (ClRequest *)poItem;
		m4uint_t	iStatus =  poRequest -> m_oStatus.GetValue () ;

		switch ( iStatus )
		{
		case  REQ_CANCELING :
			poRequest -> Cancel ( ) ;
		case REQ_CANCELED :
			_ReplyRequest ( poItem ) ;
			bIsReadyToRun = M4_FALSE;
			break;
		default:
			;
		}

		if (M4_TRUE == bIsReadyToRun)
		{
			ClUSSessionInterface	*session = (ClUSSessionInterface	*) poRequest->GetpSession();
			if  (NULL != session)
			{
				iStatus =  session -> m_oStatus.GetValue () ;
				if (SessCanceled == iStatus) 
				{
					poRequest -> Cancel ( ) ;
					_ReplyRequest ( poItem ) ;
					bIsReadyToRun = M4_FALSE;
				}
			}
		}

	}
	while (M4_FALSE == bIsReadyToRun);

	ClRequestStatistics		*pReqStats = (ClRequestStatistics*)poRequest->GetStats();
	pReqStats -> Sett ( ClRequestStatistics::InService);
	pReqStats -> StartCPUCount ((void*)m_thid) ;

	ClLauncherStats		*poLauncherStats = (ClLauncherStats	*)m_pStats;
	if (NULL != poLauncherStats)
		poLauncherStats->AddWaitTime(pReqStats -> Gett (ClRequestStatistics::InServiceQ) );


	ao_poRequest = (ClBaseRequest *)poItem;

	return M4_SUCCESS;
  //## end ClLauncherForOLTP::_GetNextRequest%943284073.body
}

m4return_t ClLauncherForOLTP::_PrepareExecution (ClBaseRequest *ai_poRequest)
{
  //## begin ClLauncherForOLTP::_PrepareExecution%943284074.body preserve=yes
	if (NULL == ai_poRequest)
	{
		SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "Executor has not been initialized properly");
		return M4_ERROR;
	}
	
	ClRequest			*poRequest = (ClRequest *)ai_poRequest;
	ClLogSystem			*logSystem = poRequest->GetLog();
	if (NULL != logSystem)
	{
		SETLOGSYSTEM(logSystem);
	}

	ClRequestPDU	*inputPDU	= (ClRequestPDU *)poRequest->GetInputPDU();

	/* Bug 0108256
	Se pone el nivel de log del cliente
	*/
	if( inputPDU != NULL && logSystem != NULL )
	{
		m4uint8_t iHasOverwriteMask = inputPDU->GetHasOverwriteMask() ;

		if( iHasOverwriteMask == 1 )
		{
			m4uint8_t iOverwriteMask = inputPDU->GetOverwriteMask() ;
			logSystem->SetOverwriteMask( iOverwriteMask ) ;
		}
	}

	ClConnInstance	*connection = poRequest -> GetpConnection( ) ;
	ClReplyPDU		*outputPDU	= (ClReplyPDU *)connection -> m_oProtocol.GetPDU(M4_ID_PDU_REPLY);

	poRequest->SetPDUOutput(outputPDU);

 	M4DataStorage *poInCompressDataStore, *poInPlainDataStore;
 	M4DataStorage *poInDataStore, *poOutDataStore;
	

	ClUSSessionInterface	*poSession = (ClUSSessionInterface *)poRequest->GetpSession();
	if (NULL == poSession)
		return M4_ERROR;

    // decompress input data storage. The ds is retireved/deleted from the input PDU (M4_FALSE).
    
	// bg 124194
	// bg 125359
	if (M4_COMPRESSION_TYPE_NULL != poSession->GetCompressionType())
	{
		poInDataStore= inputPDU->GetDataStorage( M4_FALSE ) ;
		poOutDataStore = outputPDU->GetDataStorage( M4_FALSE ) ;

		poInCompressDataStore = poInDataStore;
		poInPlainDataStore = M4DataStorage::GetNewDataStorage();

		m4return_t iRet=M4DSDecompress(*poInCompressDataStore, *poInPlainDataStore, "Z", poSession->GetCompressionType());
		if(M4_SUCCESS == iRet)
		{
			M4DataStorage::DeleteDataStorage(poInCompressDataStore);
            poInDataStore = poInPlainDataStore;
		}
        else
        {
			M4DataStorage::DeleteDataStorage(poInPlainDataStore);
        }

	}
	else {
		
		// we need GetOwnerState() to avoid changing onwer state inside the pdu.
		poOutDataStore = outputPDU->GetDataStorage( outputPDU->GetOwnerState() ) ;
		poInDataStore = inputPDU->GetDataStorage( inputPDU->GetOwnerState() ) ;
	}

	ClEngineArgs *poArgs = new ClEngineArgs(poInDataStore, poOutDataStore);
	if (NULL == poArgs)
		return M4_ERROR;

	m4char_t pcBuffer[128]="";

	ClUSUserInterface	*user = poSession->GetUser();
	if (0 == user)
		return M4_ERROR;

	m4string_t	strUsername;
	user->GetName(strUsername);
	
	m4string_t	strSessionID;
	poSession->GetSessionID(strSessionID);

	_UpdateConfigProperty(OBL_LAUCHER_INFO_USER_ID, strUsername.c_str());
	_UpdateConfigProperty(OBL_LAUCHER_INFO_SESSION_ID, strSessionID.c_str());

	m_oExecImage.SetArgs(poArgs);
	m_oExecImage.SetEnv(poSession);
	m_oExecImage.SetStatus(&(poRequest->m_oStatus));
	m_oExecImage.SetStats(poRequest->GetStats());

	// eh 280657
	m4string_t sUniqueRequestID;
	poRequest->GetUniqueRequestID(sUniqueRequestID);
	m_oExecImage.SetReqUniqueId(sUniqueRequestID);

	return M4_SUCCESS;
  //## end ClLauncherForOLTP::_PrepareExecution%943284074.body
}

m4return_t ClLauncherForOLTP::_ExecuteRequest (ClBaseRequest *ai_poRequest)
{
  //## begin ClLauncherForOLTP::_ExecuteRequest%943284075.body preserve=yes
	if ((NULL == m_pExecutive))
	{
		SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "Executor has not been initialized properly");
		return M4_ERROR;
	}

	ClStatus	*poStatus;
	m_oExecImage.GetStatus(poStatus);
	if (NULL == poStatus)
		return M4_ERROR;

	ClUSSessionInterface	*poSession;
	m_oExecImage.GetEnv(poSession);

	if (ClStatus::InfiniteTimeOut == m_iExecutionTimeout)
	{
		poStatus->ResetTimeOut();
	}
	else
	{
		if (M4_ERROR == poStatus->SetTimeOut(m_iExecutionTimeout))
			poStatus->ResetTimeOut();
	}

	m_pExecutive->SetMsgContainer ( ai_poRequest ->GetMsgContainer () ) ;
	m_pExecutive->SetSession(poSession);

	// sets thread properties
	m4objid_t idSessionId = poSession->GetId();
	M4Thread::AddProperty(M4_THREADPROP_SESSION_ID,&idSessionId);

	// executes
	m_pExecutive->Execute(m_oExecImage);

	// reset thread properties
	M4Thread::AddProperty(M4_THREADPROP_SESSION_ID,NULL);

	// reset execution variables
	m_pExecutive->SetSession(NULL);
	m_pExecutive->SetMsgContainer ( NULL ) ;

	return M4_SUCCESS;
  //## end ClLauncherForOLTP::_ExecuteRequest%943284075.body
}

m4return_t ClLauncherForOLTP::_ReplyRequest (ClBaseRequest *ai_poRequest)
{
  //## begin ClLauncherForOLTP::_ReplyRequest%943284076.body preserve=yes
	ClEngineArgs *poArgs;

 	M4DataStorage *poOutCompressDataStore, *poOutPlainDataStore;
 	M4DataStorage *poInDataStore, *poOutDataStore;


	if ((NULL == ai_poRequest) || (NULL == m_pExecutive))
	{
		SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "Executor has not been initialized properly");
		return M4_ERROR;
	}

	ClRequest			*poRequest = (ClRequest *)ai_poRequest;

	ClConnInstance		*connection = poRequest -> GetpConnection() ;

	m_oExecImage.GetArgs(poArgs);
    if (NULL == poArgs)
    {
        return M4_ERROR;
    }

	ClUSSessionInterface	*poSession = (ClUSSessionInterface *)poRequest->GetpSession();
	if (NULL == poSession)
    {
		return M4_ERROR;
    }
	
    ClReplyPDU *outputPDU	= (ClReplyPDU *)poRequest->GetOutputPDU();
    poArgs->GetOutputData(poOutDataStore);

	// bg 124194
	// bg 125359
	if (M4_COMPRESSION_TYPE_NULL != poSession->GetCompressionType())
	{
        poOutPlainDataStore = poOutDataStore;
		poOutCompressDataStore = M4DataStorage::GetNewDataStorage();

        m4return_t iRet = M4DSCompress(*poOutCompressDataStore, *poOutPlainDataStore, "Z", poSession->GetCompressionType(), poSession->GetCompressionLevel());
		if(M4_SUCCESS == iRet)
		{
			M4DataStorage::DeleteDataStorage(poOutPlainDataStore);
            poOutDataStore = poOutCompressDataStore;
		}
        else
        {
			M4DataStorage::DeleteDataStorage(poOutCompressDataStore);
        }

        outputPDU->SetDataStorage(poOutDataStore);
		
		if (NULL != poArgs)
		{
			if (poArgs->GetInputData(poInDataStore)== M4_SUCCESS)
			{
				if(poInDataStore)
				{
					M4DataStorage::DeleteDataStorage(poInDataStore);
				}
			}
			// Bg 124696. Memory leaks varios.
			// Lo liberamos más abajo, en el reset.
			//delete poArgs;
			//poArgs = NULL;
		}
    }

	m_oExecImage.Reset();
	m_pExecutive->Reset();


	//devolver la request
	ClRequestStatistics	*poStats = (ClRequestStatistics	*)ai_poRequest->GetStats();

	if (NULL != poStats)
	{
		poStats -> StopCPUCount (M4Thread::GetCurrentIdThread()) ;
	}

	if ( M4_ERROR == m_poServiceSS ->Reply(ai_poRequest) )
	{
		M4_ASSERT ( "Error retornando la request desde el launcher" ) ;
		return M4_ERROR ;
	}


	SETLOGSYSTEM(m_pLog);

	return M4_SUCCESS;
  //## end ClLauncherForOLTP::_ReplyRequest%943284076.body
}

m4return_t ClLauncherForOLTP::_CleanUp (ClBaseRequest *ai_poRequest)
{
  //## begin ClLauncherForOLTP::_CleanUp%943284079.body preserve=yes
	ClBaseRequest * poRequest ;

	if (NULL == ai_poRequest) 
	{
		SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "Executor has not been initialized properly");
		return M4_ERROR;
	}
	
	ClLogSystem	*pSOSLogSystem = new ClLogSystem;
	pSOSLogSystem -> StartCode ( M4_EXCEPTION_UNKNOWN, ERRORLOG) ;
	pSOSLogSystem -> AddText ( "Execution aborted by server") ;
	pSOSLogSystem -> SetCode() ;

//	ClRequest	*poRequest = (ClRequest	*)ai_poRequest;
	poRequest = ((ClRequest	*)ai_poRequest ) ->DuplicateWrapper();

	if ( m_poSSInvoker )
	{
		ClParamList * poParamList = new ClParamList ;;

		poParamList -> SetPointer ( "OLD_REQUEST" , ai_poRequest ) ;
		poParamList -> SetPointer ( "NEW_REQUEST" , poRequest ) ;

		m_poSSInvoker -> ExecuteAction ( REPLACE_REQUEST , poParamList ) ;

		delete poParamList ;
	}

	m_poServiceSS ->Reply(poRequest) ;

//	m_pOutputQ->Push(ai_poRequest);

	return ClLauncherInterface::_CleanUp(ai_poRequest);
  //## end ClLauncherForOLTP::_CleanUp%943284079.body
}

// Additional Declarations
  //## begin ClLauncherForOLTP%35472FA20307.declarations preserve=yes
  //## end ClLauncherForOLTP%35472FA20307.declarations

//## begin module%354838040397.epilog preserve=yes
//## end module%354838040397.epilog

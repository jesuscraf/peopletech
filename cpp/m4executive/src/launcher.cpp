//## begin module%343E2BEC0373.cm preserve=no
//## end module%343E2BEC0373.cm

//## begin module%343E2BEC0373.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:			Executive
//	 File:				Launcher.cpp
//	 Project:           M4ComSrv
//	 Author:			Meta Software M.S. , S.A
//	 Date:				971016
//	 Language:			C++
//	 Operating System:	WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	==============================================================================
//## end module%343E2BEC0373.cp

//## Module: LauncherInterface%343E2BEC0373; Package body
//## Subsystem: M4Executive::src%379462FF0256
//## Source file: F:\integration\m4executive\src\Launcher.cpp

//## begin module%343E2BEC0373.additionalIncludes preserve=no
//## end module%343E2BEC0373.additionalIncludes

//## begin module%343E2BEC0373.includes preserve=yes
#include <m4srvres.hpp>
//	Rose 98 Body Include Dependences
#include <clconfiguration.hpp>
#include <m4exception.hpp>
#include <m4eventhandlers.hpp>
#include <m4log.hpp>
#include <executive.hpp>
#include <clssinvoker.hpp>
#include <request.hpp>
//## end module%343E2BEC0373.includes

// m4types
#include <m4types.hpp>
// Statistics
#include <statistics.hpp>
// CSUtil
#include <csutil.hpp>
// NavigatorF
#include <navigatorf.hpp>
// staterecord
#include <staterecord.hpp>
// ClAdminTag
#include <cladmintag.hpp>
// ServiceQ
#include <serviceq.hpp>
// LauncherInterface
#include <launcher.hpp>



//## begin module%343E2BEC0373.declarations preserve=no
//## end module%343E2BEC0373.declarations

//## begin module%343E2BEC0373.additionalDeclarations preserve=yes
#include <subsystemids.hpp>

#define M4_SUBSYSTEM_TIMEOUT				"MAX_EXEC_SECS"
const m4pchar_t		M4SubsystemStartDate	= "START_DATE";
const m4pchar_t		M4SubsystemEndDate		= "END_DATE";
const m4pchar_t		M4ExceptionHandlingDomain = "65553";
const m4pchar_t		M4ExceptionMessage		= "Exception caught. Exiting Thread: ";

// Función de lanzamiento de excepciones para este módulo
static void M4ExecutiveThrow( M4RuntimeException ai_oException )
{
	throw( ai_oException ) ;
}

//## end module%343E2BEC0373.additionalDeclarations


// Class ClLauncherInterface 









//## begin ClLauncherInterface::m_poSSInvoker%3B4476290120.role preserve=no  protected: static ClSSInvokerAbstract { -> RFHAN}
ClSSInvokerAbstract *ClLauncherInterface::m_poSSInvoker = NULL;
//## end ClLauncherInterface::m_poSSInvoker%3B4476290120.role

ClLauncherInterface::ClLauncherInterface (ClExecutive *ai_pExecutive, ClStatistic *ai_poStats, ClServiceQ *ai_poServiceQ)
  //## begin ClLauncherInterface::ClLauncherInterface%875605497.hasinit preserve=no
      : m_pNavigator(NULL), m_pConfig(NULL), m_bInitialized(M4_FALSE)
  //## end ClLauncherInterface::ClLauncherInterface%875605497.hasinit
  //## begin ClLauncherInterface::ClLauncherInterface%875605497.initialization preserve=yes
  , ClActiveClass(), m_pServiceQ(ai_poServiceQ),
	m_pExecutive(ai_pExecutive), m_pLog(NULL), m_iExecutionTimeout(0)
  //## end ClLauncherInterface::ClLauncherInterface%875605497.initialization
{
  //## begin ClLauncherInterface::ClLauncherInterface%875605497.body preserve=yes
	m_pStats=ai_poStats;
  //## end ClLauncherInterface::ClLauncherInterface%875605497.body
}


ClLauncherInterface::~ClLauncherInterface ()
{
  //## begin ClLauncherInterface::~ClLauncherInterface%875605498.body preserve=yes
	if (NULL != m_pExecutive)
	{
		delete m_pExecutive;

		m_pExecutive = NULL;
	}

// >>> EFV 230399
	if (NULL != m_pNavigator)
	{
		delete m_pNavigator;
		m_pNavigator = NULL;
	}
  //## end ClLauncherInterface::~ClLauncherInterface%875605498.body
}



//## Other Operations (implementation)
m4return_t ClLauncherInterface::Initialize (ClNavigatorFirewall *ai_pNavigator, ClConfiguration *ai_pConfig)
{
  //## begin ClLauncherInterface::Initialize%893925596.body preserve=yes
	if (M4_TRUE == m_bInitialized)
	{
		SETCODEF ( M4_ERR_UUSS_UNKNOWN , WARNINGLOG , "Trying to initialize %s twice.", GetStringsId());
		return M4_SUCCESS;
	}

	m_pConfig = ai_pConfig;
	m_pNavigator = ai_pNavigator;

	_UpdateConfigProperty(OBL_LAUCHER_INFO_USER_ID,		OBL_LAUCHER_INFO_NO_USER_ID);
	_UpdateConfigProperty(OBL_LAUCHER_INFO_SESSION_ID,	OBL_LAUCHER_INFO_NO_SESSION_ID);
	_UpdateConfigProperty(OBL_LAUCHER_INFO_START,		OBL_LAUCHER_INFO_NO_START);
	_UpdateConfigProperty(OBL_LAUCHER_INFO_END,			OBL_LAUCHER_INFO_NO_END);

	m4double_t	dTimeout = 0.0;
	m4return_t	iRet;
	
	iRet = ai_pConfig -> GetNumericValue ( "" , M4_SUBSYSTEM_TIMEOUT,   dTimeout ) ;
	if (M4_ERROR != iRet && (0.0 < dTimeout))
	{
		m_iExecutionTimeout = 1000 * (m4uint_t)dTimeout;
	}

//	ADD	EMN	990409			//	Hay que quitar este Magic Number
	m4char_t	pszTemp[64];
	strcpy(pszTemp, M4ExceptionMessage);
	strncat(pszTemp, m_pszName, 29 /* 64-34-1*/);
	strncpy(m_pszName, pszTemp, 63);
	m_pszName[63] = 0;

	SetUserHandler(&LogCallback, (m4pvoid_t)m_pszName);

	m_bInitialized = M4_TRUE;
	if (NULL != m_pExecutive)
		return m_pExecutive->StartUp( ai_pNavigator, m_pConfig );

	return M4_ERROR;
  //## end ClLauncherInterface::Initialize%893925596.body
}

void ClLauncherInterface::Run (void )
{
  //## begin ClLauncherInterface::Run%943284072.body preserve=yes
	ClBaseRequest		*poNextRequest;
	ClImage				aImage;
	m4char_t			pcBuffer[128]="";
	ClLauncherStats		*poLauncherStats = (ClLauncherStats	*)m_pStats;
	M4ThrowFunction_t	pfOldFunction = NULL ;
	M4CheckJump			oJump;

	m_pLog = GETPTHREADLOGSYSTEM();

	ClActualDate(pcBuffer,sizeof(pcBuffer)-1);
	_UpdateConfigProperty(M4SubsystemStartDate, pcBuffer);
	_UpdateConfigProperty(M4SubsystemEndDate, OBL_LAUCHER_INFO_NO_END);
	while (!IsTerminate())
	{
		_ChangeState(ClLauncherStats::WaitState);
		if (M4_SUCCESS != _GetNextRequest(poNextRequest))
			return;

		_ChangeState(ClLauncherStats::PrepareState);
		ClActualDate(pcBuffer,sizeof(pcBuffer)-1);
		_UpdateConfigProperty(OBL_LAUCHER_INFO_START, pcBuffer);
		_UpdateConfigProperty(OBL_LAUCHER_INFO_END, OBL_LAUCHER_INFO_NO_END);
		if (M4_SUCCESS != _PrepareExecution(poNextRequest))
			return;

		_ChangeState(ClLauncherStats::ExecuteState);
		SetUserHandler(&LogCallback, (m4pvoid_t)"Fatal Error trying to recover from error!!!");

		pfOldFunction = SetThrowFunction( M4ExecutiveThrow ) ;
		
		try
		{
			// fix bug 0079564
			oJump.Check();
			if (M4_SUCCESS != _ExecuteRequest(poNextRequest))
			{
				M4CheckJump *ref = M4CheckJump::CheckThrow();
				if (ref != NULL) {
					throw ref->getException();
				}
				return;
			}
		}
		catch(M4RuntimeException &e)
		{
			_ChangeState(ClLauncherStats::DiscardedState);

            // bugid: 0087079
            SETCODEF (M4_EXCEPTION_CAUGHT, ERRORLOG, "#*s1*#%d#%s#%d#%p#", e.GetException(), e.what(), M4Thread::GetCurrentIdThread(), m_pExecutive);

			if (M4_SUCCESS == _Dump())
			{
				SETCODEF(M4_EXCEPTION_DUMP, WARNINGLOG, "Dump completed successfully!");
			}

			if (M4_SUCCESS == _CleanUp(poNextRequest))
			{
				SETCODEF(M4_EXCEPTION_UNKNOWN, DEBUGINFOLOG, "Cleanup completed successfully!");
			}

			ClActualDate(pcBuffer,sizeof(pcBuffer)-1);
			_UpdateConfigProperty(M4SubsystemEndDate, pcBuffer);

			SetThrowFunction( pfOldFunction ) ;
			return;

		}			

		SetThrowFunction( pfOldFunction ) ;

		if (NULL != poLauncherStats)
		{
/*
			if ( ( ( (ClRequest*)poNextRequest ) -> m_oStatus).GetValue () == REQ_CANCELED )
			{
				poLauncherStats->AddnUserAbortedRequest(1);
			}
			else
			{
				if ( ( ( (ClRequest*)poNextRequest ) -> m_oStatus).GetIntrValue () == ClStatus::STATUS_INTR_QUIT )
				{
					poLauncherStats->AddnAdminAbortedRequest(1);
				}else
				{
					poLauncherStats->AddnSuccessRequest(1);
				}
			}
*/
			poLauncherStats->AddnSuccessRequest(1);
			poLauncherStats->AddnProcessedItems(1);
		}


		_ChangeState(ClLauncherStats::ReleaseState);
		ClActualDate(pcBuffer,sizeof(pcBuffer)-1);
		_UpdateConfigProperty(OBL_LAUCHER_INFO_END, pcBuffer);
		if (M4_SUCCESS != _ReplyRequest(poNextRequest))
			return;
		_UpdateConfigProperty(OBL_LAUCHER_INFO_SESSION_ID,	OBL_LAUCHER_INFO_NO_SESSION_ID);
		_UpdateConfigProperty(OBL_LAUCHER_INFO_USER_ID, OBL_LAUCHER_INFO_NO_USER_ID);
	}

	ClActualDate(pcBuffer,sizeof(pcBuffer)-1);
	_UpdateConfigProperty(M4SubsystemEndDate, pcBuffer);
	_UpdateConfigProperty(OBL_LAUCHER_INFO_SESSION_ID,	OBL_LAUCHER_INFO_NO_SESSION_ID);
	_UpdateConfigProperty(OBL_LAUCHER_INFO_USER_ID, OBL_LAUCHER_INFO_NO_USER_ID);
	_ChangeState(ClLauncherStats::FinishedState);
  //## end ClLauncherInterface::Run%943284072.body
}

m4return_t ClLauncherInterface::_ChangeState (const m4pchar_t ai_pszState)
{
  //## begin ClLauncherInterface::_ChangeState%942750028.body preserve=yes
	if (NULL == m_pStats)
		return M4_SUCCESS;

	ClLauncherStats		*poLauncherStats = (ClLauncherStats	*)m_pStats;
	return poLauncherStats->m_oStateRecord.ChangeState(ai_pszState);
  //## end ClLauncherInterface::_ChangeState%942750028.body
}

m4return_t ClLauncherInterface::_Dump ()
{
  //## begin ClLauncherInterface::_Dump%921058929.body preserve=yes
	m4int_t		iNLevels, iLevel;
	m4return_t	ret = M4_SUCCESS;
	M4CheckJump			oJump, oJump2;

	try
	{
		// fix bug 0079564
		oJump.Check();
		iNLevels = m_pExecutive->GetNDumpLevels();
		for (iLevel=0; iLevel<iNLevels; iLevel++)
		{
			try
			{
				// fix bug 0079564
				oJump2.Check();
/*RM OSCAR 3-12-99
//  >> ADD EFV 990322
				// state
				EL ESTADO NO ES UNA PROPIEDAD SINO UN ATRIBUTO
				if (m_pConfig->SetStringValue("",OBL_LAUCHER_INFO_STATE,"Exception detected") != M4_SUCCESS)
				{
					SETCODEF ( M4_SRV_SS_OBL_ERROR , ERRORLOG , "Can't reset property value <%s>" , OBL_LAUCHER_INFO_STATE);
					return M4_ERROR;
				}
//  << END EFV 990322
END RM OSCAR 3-12-99 */

				ret = m_pExecutive->DumpLevel(iLevel);
				// fix bug 0079564
				M4CheckJump *ref = M4CheckJump::CheckThrow();
				if (ref != NULL) {
					throw ref->getException();
				}

				if (M4_SUCCESS != ret)
				{
					SETCODEF(M4_EXCEPTION_DUMP, ERRORLOG, "Error Dumping Info for level %d !!!!", iLevel);
				}
			}
			catch(M4RuntimeException &)
			{
				SETCODEF(M4_EXCEPTION_DUMP, ERRORLOG, "Fatal Error Dumping Info for level %d !!!!\nTrying to recover...", iLevel);
			}

		}
	}
	catch(M4RuntimeException &)
	{
		SETCODEF(M4_EXCEPTION_DUMP, ERRORLOG, "Fatal Error Dumping Info: Unable to resume!");
		ret = M4_ERROR;
	}

	m4char_t	buffer[256];
	strcpy(buffer, this->m_pszName);
	strcat(buffer, "#");
	M4PublishEvent(M4ExceptionHandlingDomain, buffer);

	return ret;
  //## end ClLauncherInterface::_Dump%921058929.body
}

m4return_t ClLauncherInterface::_CleanUp (ClBaseRequest *ai_poRequest)
{
  //## begin ClLauncherInterface::_CleanUp%927196071.body preserve=yes
	m4return_t iRet ;


	if ((NULL == m_pExecutive))
	{
		SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "Executor has not been initialized properly");
		return M4_ERROR;
	}
	
	iRet = m_pExecutive->ReleaseResources() ;

	m_pExecutive ->SetOwnState ( ClExecutive::STATE_HALTED ) ;

	return iRet ;
  //## end ClLauncherInterface::_CleanUp%927196071.body
}

m4return_t ClLauncherInterface::_UpdateConfigProperty (const m4pcchar_t ai_pszPropertyTag, const m4pcchar_t ai_pszPropertyValue)
{
  //## begin ClLauncherInterface::_UpdateConfigProperty%942750029.body preserve=yes
	if (NULL == m_pConfig)
		return M4_ERROR;

	// !!! Cast to non-const to compile
	if (M4_SUCCESS != m_pConfig->SetStringValue("", (char *)ai_pszPropertyTag, (char *)ai_pszPropertyValue))
	{
		// !!! Cast to non-const to compile
		if (M4_SUCCESS != m_pConfig->AddStringProperty("", (char *)ai_pszPropertyTag, (char *)ai_pszPropertyValue))
		{
			SETCODEF ( M4_SRV_SS_OBL_ERROR , ERRORLOG , "Can't generate new property value <%s>" , ai_pszPropertyTag);
			return M4_ERROR;
		}
	}

	return M4_SUCCESS;
  //## end ClLauncherInterface::_UpdateConfigProperty%942750029.body
}

m4int16_t ClLauncherInterface::GetStatus ()
{
  //## begin ClLauncherInterface::GetStatus%982580498.body preserve=yes
	return m_pExecutive->GetStatus () ;
  //## end ClLauncherInterface::GetStatus%982580498.body
}

// Additional Declarations
  //## begin ClLauncherInterface%34200012029B.declarations preserve=yes
  //## end ClLauncherInterface%34200012029B.declarations

// Class ClLauncherVisitor 

ClLauncherVisitor::ClLauncherVisitor ()
  //## begin ClLauncherVisitor::ClLauncherVisitor%879608295.hasinit preserve=no
  //## end ClLauncherVisitor::ClLauncherVisitor%879608295.hasinit
  //## begin ClLauncherVisitor::ClLauncherVisitor%879608295.initialization preserve=yes
  : ClActiveClassVisitor()
  //## end ClLauncherVisitor::ClLauncherVisitor%879608295.initialization
{
  //## begin ClLauncherVisitor::ClLauncherVisitor%879608295.body preserve=yes
  //## end ClLauncherVisitor::ClLauncherVisitor%879608295.body
}



//## Other Operations (implementation)
void ClLauncherVisitor::VisitLauncher (ClLauncherInterface &ai_oLauncher)
{
  //## begin ClLauncherVisitor::VisitLauncher%877356014.body preserve=yes
	//	Processed Requests
	//	Discarded Requests
	//	AvgProcessedRequests
	//	Virtual Machine Count
	//	ItemsProcessed
	VisitActiveClass((ClActiveClass &)ai_oLauncher);
	
	ClLauncherStats	*pStats = (ClLauncherStats *)ai_oLauncher.GetStats();
  //## end ClLauncherVisitor::VisitLauncher%877356014.body
}

// Additional Declarations
  //## begin ClLauncherVisitor%344B7EA1000A.declarations preserve=yes
  //## end ClLauncherVisitor%344B7EA1000A.declarations

// Class ClLauncherStats 

//## begin ClLauncherStats::PrepareState%38313B7C01EC.attr preserve=no  public: static m4pchar_t {VAC} "Preparing input request"
const m4pchar_t  ClLauncherStats::PrepareState = "Preparing input request";
//## end ClLauncherStats::PrepareState%38313B7C01EC.attr

//## begin ClLauncherStats::DiscardedState%383141080099.attr preserve=no  public: static m4pchar_t {UAC} "Aborted by unhandled exception"
const m4pchar_t  ClLauncherStats::DiscardedState = "Aborted by unhandled exception";
//## end ClLauncherStats::DiscardedState%383141080099.attr

//## begin ClLauncherStats::InQueueState%38341651000B.attr preserve=no  public: static m4pchar_t {VAC} "In Queue"
const m4pchar_t  ClLauncherStats::InQueueState = "In Queue";
//## end ClLauncherStats::InQueueState%38341651000B.attr

//## begin ClLauncherStats::ReleaseState%38342A36019F.attr preserve=no  public: static m4pchar_t {VAC} "Preparing output request"
const m4pchar_t  ClLauncherStats::ReleaseState = "Preparing output request";
//## end ClLauncherStats::ReleaseState%38342A36019F.attr

//## begin ClLauncherStats::WaitState%383434AD001B.attr preserve=no  public: static m4pchar_t {UAC} "Waiting for Input"
const m4pchar_t  ClLauncherStats::WaitState = "Waiting for Input";
//## end ClLauncherStats::WaitState%383434AD001B.attr

//## begin ClLauncherStats::ExecuteState%383434AD0057.attr preserve=no  public: static m4pchar_t {UAC} "Executing"
const m4pchar_t  ClLauncherStats::ExecuteState = "Executing";
//## end ClLauncherStats::ExecuteState%383434AD0057.attr

//## begin ClLauncherStats::FinishedState%3857896B031F.attr preserve=no  public: static m4pchar_t {UAC} "Finished Execution"
const m4pchar_t  ClLauncherStats::FinishedState = "Finished Execution";
//## end ClLauncherStats::FinishedState%3857896B031F.attr







ClLauncherStats::ClLauncherStats ()
  //## begin ClLauncherStats::ClLauncherStats%884795084.hasinit preserve=no
      : m_nUserAbortedRequest(0), m_nErrorRequest(0), m_nAdminAbortedRequest(0), m_nSuccessRequest(0), m_nTimedOutRequest(0)
  //## end ClLauncherStats::ClLauncherStats%884795084.hasinit
  //## begin ClLauncherStats::ClLauncherStats%884795084.initialization preserve=yes
  //## end ClLauncherStats::ClLauncherStats%884795084.initialization
{
  //## begin ClLauncherStats::ClLauncherStats%884795084.body preserve=yes

	if ( m_uiNumberOfStats < TOTAL_NUM )
	{
		m_uiNumberOfStats = TOTAL_NUM;
	}

  //## end ClLauncherStats::ClLauncherStats%884795084.body
}


ClLauncherStats::~ClLauncherStats ()
{
  //## begin ClLauncherStats::~ClLauncherStats%884795085.body preserve=yes
  //## end ClLauncherStats::~ClLauncherStats%884795085.body
}



//## Other Operations (implementation)
m4return_t ClLauncherStats::Reset ()
{
  //## begin ClLauncherStats::Reset%884795086.body preserve=yes
	ClActiveClassStats::Reset();

	m_oStateRecord.Clear();

	return M4_SUCCESS;
  //## end ClLauncherStats::Reset%884795086.body
}

m4return_t ClLauncherStats::GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_poData)
{
  //## begin ClLauncherStats::GetStat%886671657.body preserve=yes
	m4uint16_t				lowIndex;
	eLauncherStatsID		statIndex;
	m4return_t				res = M4_SUCCESS;
	M4ClString				strState;

//	if (M4_ERROR == _CheckStatID(ai_hStatId, M4_LAUNCHER_SUBSYSTEM))
//		return M4_ERROR;
//	if (M4_SUCCESS != m_oStateRecord.GetCurrentState(strState))
//		return M4_ERROR;
	
//	if (DiscardedState == strState.c_str())
//		return M4_ERROR;

	lowIndex = ai_hStatId.GetLowID();
	if (ClActiveClassStats::TOTAL_NUM  > lowIndex)
		return ClActiveClassStats::GetStat(ai_hStatId, ao_poData);

	statIndex = (eLauncherStatsID)lowIndex;

	if (0 < ao_poData.GetSize())
		return M4_ERROR;

	ao_poData.SetSize(2 * DEF_STRING_SIZE);
	m4pchar_t	buffer = ao_poData.GetBuffer();
//    

/*  enum {
			ERROR_REQUEST_COUNT=ClActiveClassStats::TOTAL_NUM,
			TIMED_OUT_REQUEST_COUNT,
			USER_ABORTED_REQUEST_COUNT ,
			ADMIN_ABORTED_REQUEST_COUNT ,
			SUCCESS_REQUEST_COUNT ,
			CURRENT_STATE,
			FIV_SEC_TPS,
			THIR_SEC_TPS,
			FIV_MIN_TPS,
			FIV_SEC_AVG_EXEC,
			THIR_SEC_AVG_EXEC,
			FIV_MIN_AVG_EXEC,
			FIV_SEC_SSUSE,
			THIR_SEC_SSUSE,
			FIV_MIN_SSUSE,
			FIV_SEC_WAIT_TIME,
			THIR_SEC_WAIT_TIME,
			FIV_MIN_WAIT_TIME,
			TOTAL_NUM};
*/
	m4size_t		iChanges = 0;
	m4uint_t		iTime = 0, iAbsTime = m_oStateRecord.GetElapsedTime();
	m4float_t		ptgTime;
	switch(statIndex)
	{
    case ERROR_REQUEST_COUNT :
		sprintf(buffer, "%010d", m_nErrorRequest);
		break;
	case TIMED_OUT_REQUEST_COUNT:
		sprintf(buffer, "%010d", m_nTimedOutRequest);
		break;
	case USER_ABORTED_REQUEST_COUNT :
		sprintf(buffer, "%010d", m_nUserAbortedRequest);
		break;
	case ADMIN_ABORTED_REQUEST_COUNT :
		sprintf(buffer, "%010d", m_nAdminAbortedRequest);
		break;
	case SUCCESS_REQUEST_COUNT :
		sprintf(buffer, "%010d", m_nSuccessRequest);
		break;
	case CURRENT_STATE:
		if (M4_SUCCESS == m_oStateRecord.GetCurrentState(strState))
			sprintf(buffer, "%s", strState.c_str());
		break;
	case FIV_SEC_TPS:
		if (iAbsTime > 5000)
			iAbsTime = 5000;
		if (M4_SUCCESS != m_oStateRecord.GetStateInfo(ReleaseState, iChanges, iTime, 5000))
			iChanges = 0;

		ptgTime = 0;
		if (0 != iAbsTime)
			ptgTime = (m4float_t)iChanges * 1000 / iAbsTime;

		sprintf ( buffer , "%10.2f" , ptgTime) ;

		break ;

	case THIR_SEC_TPS:
		if (iAbsTime > 30000)
			iAbsTime = 30000;
		if (M4_SUCCESS != m_oStateRecord.GetStateInfo(ReleaseState, iChanges, iTime, 30000))
			iChanges = 0;

		ptgTime = 0;
		if (0 != iAbsTime)
			ptgTime = (m4float_t)iChanges * 1000 /iAbsTime;

		sprintf ( buffer , "%10.2f" , ptgTime) ;

		break ;

	case FIV_MIN_TPS:
		if (iAbsTime > 300000)
			iAbsTime = 300000;
		if (M4_SUCCESS != m_oStateRecord.GetStateInfo(ReleaseState, iChanges, iTime, 300000))
			iChanges = 0;

		ptgTime = 0;
		if (0 != iAbsTime)
			ptgTime = (m4float_t)iChanges * 1000 /iAbsTime;

		sprintf ( buffer , "%10.2f" , ptgTime) ;

		break ;

	case FIV_SEC_AVG_EXEC:
		if (M4_SUCCESS != m_oStateRecord.GetStateInfo(ExecuteState, iChanges, iTime, 5000))
			iTime = 0;

		ptgTime = 0;
		if (0 != iChanges)
			ptgTime = (m4float_t)iTime / (1000*iChanges);

		sprintf ( buffer , "%10.2f" , ptgTime) ;
		break ;

	case THIR_SEC_AVG_EXEC:
		if (M4_SUCCESS != m_oStateRecord.GetStateInfo(ExecuteState, iChanges, iTime, 30000))
			iTime = 0;

		ptgTime = 0;
		if (0 != iChanges)
			ptgTime = (m4float_t)iTime / (1000*iChanges);

		sprintf ( buffer , "%10.2f" , ptgTime) ;
		break ;

	case FIV_MIN_AVG_EXEC:
		if (M4_SUCCESS != m_oStateRecord.GetStateInfo(ExecuteState, iChanges, iTime, 300000))
			iTime = 0;

		ptgTime = 0;
		if (0 != iChanges)
			ptgTime = (m4float_t)iTime / (1000*iChanges);

		sprintf ( buffer , "%10.2f" , ptgTime) ;
		break;

	case FIV_SEC_SSUSE:
		if (iAbsTime > 5000)
			iAbsTime = 5000;
		if (M4_SUCCESS != m_oStateRecord.GetStateInfo(WaitState, iChanges, iTime, 5000))
			iTime = iAbsTime;

		ptgTime = 100;
		if (0 < iTime)
		{
			iTime = iAbsTime - iTime;
			ptgTime = 100 * (m4float_t)iTime/iAbsTime;
		}
		sprintf ( buffer , "%6.2f" , ptgTime) ;
		break ;

	case THIR_SEC_SSUSE:
		iAbsTime = m_oStateRecord.GetElapsedTime();
		if (iAbsTime > 30000)
			iAbsTime = 30000;
		if (M4_SUCCESS != m_oStateRecord.GetStateInfo(WaitState, iChanges, iTime, 30000))
			iTime = iAbsTime;

		ptgTime = 100;
		if (0 < iTime)
		{
			iTime = iAbsTime - iTime;
			ptgTime = 100 * (m4float_t)iTime/iAbsTime;
		}
		sprintf ( buffer , "%6.2f" , ptgTime) ;
		break ;

	case FIV_MIN_SSUSE:
		iAbsTime = m_oStateRecord.GetElapsedTime();
		if (iAbsTime > 300000)
			iAbsTime = 300000;
		if (M4_SUCCESS != m_oStateRecord.GetStateInfo(WaitState, iChanges, iTime, 300000))
			iTime = iAbsTime;

		ptgTime = 100;
		if (0 < iTime)
		{
			iTime = iAbsTime - iTime;
			ptgTime = 100 * (m4float_t)iTime/iAbsTime;
		}
		sprintf ( buffer , "%6.2f", ptgTime) ;
		break ;

	case FIV_SEC_WAIT_TIME:
		if (M4_SUCCESS != m_oStateRecord.GetStateInfo(ClLauncherStats::InQueueState, iChanges, iTime, 5000))
			iTime = 0;

		ptgTime = 0;
		if (0 != iChanges)
		{
			ptgTime = (m4float_t)iTime/(1000 * iChanges);
		}
		sprintf ( buffer , "%10.2f" , ptgTime) ;
		break ;

	case THIR_SEC_WAIT_TIME:
		if (M4_SUCCESS != m_oStateRecord.GetStateInfo(ClLauncherStats::InQueueState, iChanges, iTime, 30000))
			iTime = 0;

		ptgTime = 0;
		if (0 != iChanges)
		{
			ptgTime = (m4float_t)iTime/(1000 * iChanges);
		}
		sprintf ( buffer , "%10.2f" , ptgTime) ;
		break ;

	case FIV_MIN_WAIT_TIME:
		if (M4_SUCCESS != m_oStateRecord.GetStateInfo(ClLauncherStats::InQueueState, iChanges, iTime, 300000))
			iTime = 0;

		ptgTime = 0;
		if (0 != iChanges)
		{
			ptgTime = (m4float_t)iTime/(1000 * iChanges);
		}
		sprintf ( buffer , "%10.2f" , ptgTime) ;
		break ;

	default:
		sprintf(buffer, "%010d", 0);
		res = M4_ERROR;
	}

	return res;
  //## end ClLauncherStats::GetStat%886671657.body
}

m4return_t ClLauncherStats::ResetStat (ClStatHandle &ai_hStatId)
{
  //## begin ClLauncherStats::ResetStat%886671658.body preserve=yes
	m4uint16_t				lowIndex;
	eLauncherStatsID		statIndex;
	m4return_t				res = M4_TRUE;

	if (M4_ERROR == _CheckStatID(ai_hStatId, M4_LAUNCHER_SUBSYSTEM))
		return M4_ERROR;

	lowIndex = ai_hStatId.GetLowID();
	statIndex = (eLauncherStatsID)lowIndex;
//    

/*		enum {
				ERROR_REQUEST_COUNT=ClActiveClassStats::TOTAL_NUM,
				TIMED_OUT_REQUEST_COUNT,
				USER_ABORTED_REQUEST_COUNT ,
				ADMIN_ABORTED_REQUEST_COUNT ,
				SUCCESS_REQUEST_COUNT ,
				CURRENT_STATE,
				FIV_SEC_TPS,
				THIR_SEC_TPS,
				FIV_MIN_TPS,
				FIV_SEC_AVG_EXEC,
				THIR_SEC_AVG_EXEC,
				FIV_MIN_AVG_EXEC,
				FIV_SEC_SSUSE,
				THIR_SEC_SSUSE,
				FIV_MIN_SSUSE,
				FIV_SEC_WAIT_TIME,
				THIR_SEC_WAIT_TIME,
				FIV_MIN_WAIT_TIME,
				TOTAL_NUM
			} */

	switch(statIndex)
	{
    case ERROR_REQUEST_COUNT :
		m_nErrorRequest = 0;
		break;
	case TIMED_OUT_REQUEST_COUNT:
		m_nTimedOutRequest = 0;
		break;
	case USER_ABORTED_REQUEST_COUNT :
		m_nUserAbortedRequest = 0;
		break;
	case ADMIN_ABORTED_REQUEST_COUNT :
		m_nAdminAbortedRequest = 0;
		break;
	case SUCCESS_REQUEST_COUNT :
		m_nSuccessRequest = 0;
		break;
	case FIV_SEC_TPS:
	case THIR_SEC_TPS:
	case FIV_MIN_TPS:
	case FIV_SEC_AVG_EXEC:
	case THIR_SEC_AVG_EXEC:
	case FIV_MIN_AVG_EXEC:
	case FIV_SEC_SSUSE:
	case THIR_SEC_SSUSE:
	case FIV_MIN_SSUSE:
	case FIV_SEC_WAIT_TIME:
	case THIR_SEC_WAIT_TIME:
	case FIV_MIN_WAIT_TIME:
		m_oStateRecord.Clear();
		break ;

	default:
		res = M4_FALSE;
	}

	return res;
  //## end ClLauncherStats::ResetStat%886671658.body
}

void ClLauncherStats::AddWaitTime (m4uint_t ai_iEllapsedTime)
{
  //## begin ClLauncherStats::AddWaitTime%942928609.body preserve=yes
	if (0 == ai_iEllapsedTime)
		return;

	m4millisec_t	iStart;
	m4uint_t		iEllapsed;

	iStart = ClPrecisionTicker::GetTimeTick();
	iEllapsed = ai_iEllapsedTime;
	
	iStart -= iEllapsed;
	m_oStateRecord.RecordChange(ClLauncherStats::InQueueState, iStart, iEllapsed);
  //## end ClLauncherStats::AddWaitTime%942928609.body
}

// Additional Declarations
  //## begin ClLauncherStats%346220B60258.declarations preserve=yes
  //## end ClLauncherStats%346220B60258.declarations

//## begin module%343E2BEC0373.epilog preserve=yes
//## end module%343E2BEC0373.epilog

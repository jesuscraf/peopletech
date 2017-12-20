//## begin module%350915C103E7.cm preserve=no
//## end module%350915C103E7.cm

//## begin module%350915C103E7.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            Protocol
//	 File:              Request.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:				980508
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
//## end module%350915C103E7.cp

//## Module: Request%350915C103E7; Subprogram body
//## Subsystem: M4Request::src%379439BF01B7
//## Source file: D:\Work\v600\m4request\src\request.cpp

//## begin module%350915C103E7.additionalIncludes preserve=yes

#include <m4win.hpp>
#include <stdio.h>
#include <m4srvres.hpp>
#include <m4trace.hpp>
#include <connbase.hpp>
#include <usinterface.hpp>
#include <servicepdus.hpp>
#include <basepdus.hpp>
#include <blocksynchronization.hpp>
#include <m4credentials.hpp>
#include <credentialsfactory.hpp>
#include <rawsessionid.hpp>
#include <staterecord.hpp>
#include <controlpdus.hpp>
#include <m4cryptc.hpp>
#include "m4objreg.hpp"
#include <msgcontainers.hpp>
#include <requestcollection.hpp>
#include <request.hpp>
#include <m4types.hpp>
#include <rollingfile.hpp>
#include <baserequest.hpp>
#include <statistics.hpp>
#include <exvariables.hpp>
#include <logsys.hpp>
#include <m4date.hpp>
#include <criticalsection.hpp>
#include <clarraylibres.hpp>

#ifdef _WINDOWS
	#include <io.h>
#endif

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>



//## begin module%350915C103E7.declarations preserve=no
//## end module%350915C103E7.declarations

//## begin module%350915C103E7.additionalDeclarations preserve=yes
#define M4_SRV_TRACE_COMP_ID	"REQ"

//## end module%350915C103E7.additionalDeclarations


// Class ClRequestStatistics 

//## begin ClRequestStatistics::Reading%350E691003C2.attr preserve=no  public: static m4pchar_t {VAC} "Reading"
const m4pchar_t  ClRequestStatistics::Reading = "Reading";
//## end ClRequestStatistics::Reading%350E691003C2.attr

//## begin ClRequestStatistics::Creating%347091390237.attr preserve=no  public: static m4pchar_t {VAC} "Creating"
const m4pchar_t  ClRequestStatistics::Creating = "Creating";
//## end ClRequestStatistics::Creating%347091390237.attr

//## begin ClRequestStatistics::GettingSession%350E691003D6.attr preserve=no  public: static m4pchar_t {VAC} "GettingSession"
const m4pchar_t  ClRequestStatistics::GettingSession = "GettingSession";
//## end ClRequestStatistics::GettingSession%350E691003D6.attr

//## begin ClRequestStatistics::Registering%347091B90187.attr preserve=no  public: static m4pchar_t {VAC} "Registering"
const m4pchar_t  ClRequestStatistics::Registering = "Registering";
//## end ClRequestStatistics::Registering%347091B90187.attr

//## begin ClRequestStatistics::GettingService%3A0929AB0314.attr preserve=no  public: static m4pchar_t {UAC} "GettingService"
const m4pchar_t  ClRequestStatistics::GettingService = "GettingService";
//## end ClRequestStatistics::GettingService%3A0929AB0314.attr

//## begin ClRequestStatistics::InServiceQ%347093910077.attr preserve=no  public: static m4pchar_t {VAC} "InServiceQ"
const m4pchar_t  ClRequestStatistics::InServiceQ = "InServiceQ";
//## end ClRequestStatistics::InServiceQ%347093910077.attr

//## begin ClRequestStatistics::InService%350E691003CC.attr preserve=no  public: static m4pchar_t {VAC} "InService"
const m4pchar_t  ClRequestStatistics::InService = "InService";
//## end ClRequestStatistics::InService%350E691003CC.attr

//## begin ClRequestStatistics::Writing%347091E4002A.attr preserve=no  public: static m4pchar_t {VAC} "Writing"
const m4pchar_t  ClRequestStatistics::Writing = "Writing";
//## end ClRequestStatistics::Writing%347091E4002A.attr

//## begin ClRequestStatistics::Deregistering%347096FB01F1.attr preserve=no  public: static m4pchar_t {VAC} "Deregistering"
const m4pchar_t  ClRequestStatistics::Deregistering = "Deregistering";
//## end ClRequestStatistics::Deregistering%347096FB01F1.attr

//## begin ClRequestStatistics::InCommQueue%3AB61B620250.attr preserve=no  public: static m4pchar_t {UAC} "InCommQueue"
const m4pchar_t  ClRequestStatistics::InCommSubsystem = "InCommSubsystem";
//## end ClRequestStatistics::InCommQueue%3AB61B620250.attr

//## begin ClRequestStatistics::InSSQueue%3AB61B62039B.attr preserve=yes  public: static m4pchar_t {UAC} "InSSQueue"
const m4pchar_t  ClRequestStatistics::InSSQueueIn = "InSSQueueIn";
const m4pchar_t  ClRequestStatistics::InSSQueueOut = "InSSQueueOut";
//## end ClRequestStatistics::InSSQueue%3AB61B62039B.attr





ClRequestStatistics::ClRequestStatistics (m4uint64_t ai_uiStartRecordTime)
  //## begin ClRequestStatistics::ClRequestStatistics%890732550.hasinit preserve=no
  //## end ClRequestStatistics::ClRequestStatistics%890732550.hasinit
  //## begin ClRequestStatistics::ClRequestStatistics%890732550.initialization preserve=yes
  :m_oStateRecord ( ClStateRecord::M4DefWindowSize , ai_uiStartRecordTime )
  //## end ClRequestStatistics::ClRequestStatistics%890732550.initialization
{
  //## begin ClRequestStatistics::ClRequestStatistics%890732550.body preserve=yes
	Sett ( Reading , ai_uiStartRecordTime);

	m_iStartKernelCPUCountTime = 0;
	m_iStartUserCPUCountTime = 0;
	m_iUserCPUTime = 0 ;
	m_iKernelCPUTime = 0 ;
	m_bCPUCounting = M4_FALSE;
	m_iThreadId = 0 ;
	m_iInputSize = 0;
	m_iOutputSize = 0;
	m_iUserInfo = 0;
	m_iRowsQuotaCurrent = 0;
	m_iRowsQuotaPeak = 0;
	m_iRowsQuotaLimit = 0;
	m_lRequestStartTime = time( NULL );
	m_pcSessionKey = NULL;


  //## end ClRequestStatistics::ClRequestStatistics%890732550.body
}


ClRequestStatistics::~ClRequestStatistics ()
{
  //## begin ClRequestStatistics::~ClRequestStatistics%890732551.body preserve=yes
	if (m_pcSessionKey != NULL)
	{
		delete m_pcSessionKey;
	}
  //## end ClRequestStatistics::~ClRequestStatistics%890732551.body
}



//## Other Operations (implementation)
m4return_t ClRequestStatistics::Reset ()
{
  //## begin ClRequestStatistics::Reset%879794950.body preserve=yes
	m_oStateRecord.Clear();

	return M4_SUCCESS;
  //## end ClRequestStatistics::Reset%879794950.body
}

void ClRequestStatistics::Dump (m4string_t &ao_sMessage, m4pcchar_t ai_pccService, m4bool_t ai_bOnlyHeader)
{
	if (M4_TRUE == ai_bOnlyHeader)
	{
		DumpHeader(ao_sMessage);
	}
	else
	{
		m4char_t pBuffer[2048];
		m4string_t sBuffer;
		int	iYYYY = 0, iMM = 0, iDD = 0, iHH = 0, imm = 0, iSS = 0;

		if( m_lRequestStartTime != -1 )
		{
			struct tm*	stReqTime = gmtime( &m_lRequestStartTime );
			iYYYY	= stReqTime->tm_year + 1900;
			iMM		= stReqTime->tm_mon + 1;
			iDD		= stReqTime->tm_mday;
			iHH		= stReqTime->tm_hour;
			imm		= stReqTime->tm_min;
			iSS		= stReqTime->tm_sec;
		}

		sprintf(pBuffer, "\t%u\t%d-%02d-%02d %02d:%02d:%02d\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u", 
			(m4uint32_t) GetStartTime(), 
			iYYYY, iMM, iDD, iHH, imm, iSS,
			(m4uint32_t) Gett(Reading), 
			(m4uint32_t) Gett(InCommSubsystem), 
			(m4uint32_t) Gett(InSSQueueIn),
			(m4uint32_t) Gett(Creating), 
			(m4uint32_t) Gett(GettingSession), 
			(m4uint32_t) Gett(Registering), 
			(m4uint32_t) Gett(GettingService), 
			(m4uint32_t) Gett(InServiceQ), 
			(m4uint32_t) Gett(InService), 
			(m4uint32_t) Gett(InSSQueueOut),
			(m4uint32_t) Gett(Writing), 
			(m4uint32_t) Gett(Deregistering), 
			(m4uint32_t) GetInProcess());	
			sBuffer = pBuffer;


		/* Bug 0113962
		Se vuelcan los tamaños de pdu
		*/
        sprintf(pBuffer, "\t%u\t%u", m_iInputSize, m_iOutputSize );
		sBuffer.append(pBuffer);

		// bg 199240
		if( m_iUserInfo == 0 && ai_pccService != NULL && !strcmpi( ai_pccService, "DISCONNECT" ) )
		{
			sprintf(pBuffer, "\t-1");
		}
		else
		{
			sprintf(pBuffer, "\t%u", m_iUserInfo);
		}
		sBuffer.append(pBuffer);

		// bugid 99142
        sprintf(pBuffer, "\t%u", m_iThreadId);
		sBuffer.append(pBuffer);

#ifdef _WINDOWS
		sprintf(pBuffer, "\t%u\t%u", (m4uint32_t) m_iKernelCPUTime/10000, (m4uint32_t) m_iUserCPUTime/10000);
		sBuffer.append(pBuffer);
#endif

		sprintf(pBuffer, "\t%u", m_iRowsQuotaCurrent);
		sBuffer.append(pBuffer);

		sprintf(pBuffer, "\t%u", m_iRowsQuotaLimit);
		sBuffer.append(pBuffer);
		
		sprintf(pBuffer, "\t%u", m_iRowsQuotaPeak);
		sBuffer.append(pBuffer);

		sBuffer.append( "\t" );
		if( m_pcSessionKey != NULL )
		{
			sBuffer.append( m_pcSessionKey );
		}

		ao_sMessage = sBuffer;
	}
}

void ClRequestStatistics::Dump (m4string_t &ao_sMessage, m4bool_t ai_bOnlyHeader)
{
	//## begin ClRequestStatistics::Dump%940581805.body preserve=yes
	Dump( ao_sMessage, NULL, ai_bOnlyHeader );
	//## end ClRequestStatistics::Dump%940581805.body
}

void ClRequestStatistics::DumpHeader (m4string_t &ao_sHeader)
{
  //## begin ClRequestStatistics::DumpHeader%959772392.body preserve=yes
	m4char_t pBuffer[2048];
	m4string_t sBuffer;
	sprintf(pBuffer, "\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s", 
		"StartTime", 
		"Time",
		Reading, 
		InCommSubsystem, 
		InSSQueueIn, 
		Creating, 
		GettingSession, 
		Registering, 
		GettingService, 
		InServiceQ, 
		InService, 
		InSSQueueOut, 
		Writing, 
		Deregistering, 
		"TotalTime");
		
		sBuffer = pBuffer;

	/* Bug 0113962
	Se vuelcan los tamaños de pdu
	*/
    sprintf(pBuffer, "\t%s\t%s", "InputSize", "OutputSize");
	sBuffer.append(pBuffer);

    sprintf(pBuffer, "\t%s", "UserInfo");
	sBuffer.append(pBuffer);

    // bugid 99142
    sprintf(pBuffer, "\t%s", "Thread ID");
	sBuffer.append(pBuffer);

#ifdef _WINDOWS
	sprintf(pBuffer, "\t%s\t%s", "Kernel CPU", "User CPU");
	sBuffer.append(pBuffer);
#endif

	sprintf(pBuffer, "\t%s", "Rows");
	sBuffer.append(pBuffer);

	sprintf(pBuffer, "\t%s", "MaxRows");
	sBuffer.append(pBuffer);

	sprintf(pBuffer, "\t%s", "PeakRows");
	sBuffer.append(pBuffer);

	sprintf( pBuffer, "\t%s", "SessionKey" );
	sBuffer.append( pBuffer );

	sprintf( pBuffer, "\t%s", "ServerHost" );
	sBuffer.append( pBuffer );

	sprintf( pBuffer, "\t%s", "ServerPort" );
	sBuffer.append( pBuffer );

	sprintf( pBuffer, "\t%s", "SessionId" );
	sBuffer.append( pBuffer );

	ao_sHeader = sBuffer;
  //## end ClRequestStatistics::DumpHeader%959772392.body
}

m4return_t ClRequestStatistics::GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_poData)
{
  //## begin ClRequestStatistics::GetStat%962640091.body preserve=yes
	return M4_ERROR;
  //## end ClRequestStatistics::GetStat%962640091.body
}

m4return_t ClRequestStatistics::ResetStat (ClStatHandle &ai_hStatId)
{
  //## begin ClRequestStatistics::ResetStat%962640092.body preserve=yes
	return M4_ERROR;
  //## end ClRequestStatistics::ResetStat%962640092.body
}

ClStatTimeType ClRequestStatistics::_ElapsedTime (const m4pchar_t ai_pszWhere)
{
  //## begin ClRequestStatistics::_ElapsedTime%1068803580.body preserve=yes
	m4uint_t		uiMillisecs;
	m4size_t		iSamples;
	if (M4_SUCCESS != m_oStateRecord.GetStateInfo(ai_pszWhere, iSamples, uiMillisecs))
		return 0;

	if (0 == iSamples)
		return 0;

	return (ClStatTimeType)uiMillisecs;
  //## end ClRequestStatistics::_ElapsedTime%1068803580.body
}

m4return_t ClRequestStatistics::StartCPUCount (void* ai_hThreadHandle)
{
  //## begin ClRequestStatistics::StartCPUCount%1068803581.body preserve=yes

	if ( m_bCPUCounting )
		return M4_ERROR ;

    // bugid 99142
	m_iThreadId = (m4uint32_t) ai_hThreadHandle ;

#ifdef _WINDOWS
	FILETIME lpCreationTime,  // thread creation time
		lpExitTime,      // thread exit time
		lpKernelTime,    // thread kernel-mode time
		lpUserTime ;	// thread user-mode time

	if (! GetThreadTimes ( (void*) m_iThreadId ,&lpCreationTime, &lpExitTime, &lpKernelTime, &lpUserTime ) )
	{
		return M4_ERROR ;
	}

	m_iStartKernelCPUCountTime = 0x100000000 ;
	m_iStartKernelCPUCountTime *= lpKernelTime.dwHighDateTime ;
	m_iStartKernelCPUCountTime += lpKernelTime.dwLowDateTime ;
	m_iStartUserCPUCountTime = 0x100000000 ;
	m_iStartUserCPUCountTime *= lpUserTime.dwHighDateTime ;
	m_iStartUserCPUCountTime += lpUserTime.dwLowDateTime ;
#endif

	m_bCPUCounting = M4_TRUE ;

	return M4_SUCCESS ;
  //## end ClRequestStatistics::StartCPUCount%1068803581.body
}

m4return_t ClRequestStatistics::StopCPUCount (m4iden_t ai_iThreadId)
{
  //## begin ClRequestStatistics::StopCPUCount%1068803582.body preserve=yes
	if ( !m_bCPUCounting )
		return M4_ERROR ;

#ifdef _WINDOWS
	m4uint64_t iKernelCPU2 , iUserCPU2 ;
	FILETIME lpCreationTime,  // thread creation time
		lpExitTime,      // thread exit time
		lpKernelTime,    // thread kernel-mode time
		lpUserTime ;	// thread user-mode time
	
	if ( ! GetThreadTimes ( (void*) m_iThreadId ,&lpCreationTime, &lpExitTime, &lpKernelTime, &lpUserTime ) )
	{
		return M4_ERROR ;
	}

	iKernelCPU2 = 0x100000000 ;
	iKernelCPU2 *= lpKernelTime.dwHighDateTime ;
	iKernelCPU2 += lpKernelTime.dwLowDateTime ;
	iUserCPU2 = 0x100000000 ;
	iUserCPU2 *= lpUserTime.dwHighDateTime ;
	iUserCPU2 += lpUserTime.dwLowDateTime ;

	m_iKernelCPUTime += iKernelCPU2 - m_iStartKernelCPUCountTime;
	m_iUserCPUTime += iUserCPU2 - m_iStartUserCPUCountTime;

#endif 

	// replaces the handler to a real id, so at the end the dumper will display an ID
	// instead of a handler.
	// fix bug 0089302
	m_iThreadId = ai_iThreadId;

	m_bCPUCounting = M4_FALSE ;
	m_iStartKernelCPUCountTime = 0 ;
	m_iStartUserCPUCountTime = 0 ;

	return M4_SUCCESS ;

  //## end ClRequestStatistics::StopCPUCount%1068803582.body
}

// Additional Declarations
  //## begin ClRequestStatistics%34708E67010D.declarations preserve=yes
  //## end ClRequestStatistics%34708E67010D.declarations

// Class ClRequest 


m4string_t _CreateHeader (void)
{
	M4ClTimeStamp date;
	m4int32_t a_year;
	m4int32_t a_month;
	m4int32_t a_day ;
	m4int32_t ai_ihour;
	m4int32_t ai_imin;
	m4int32_t ai_isec;

	/* Bug 0175418
	Se toma el tickcount en el mismo momento que la fecha
	*/
	m4millisec_t dReferenceTime = ClPrecisionTicker::GetTimeTick();
	
	date.now();
	date.get( a_year, a_month, a_day, ai_ihour, ai_imin, ai_isec );
	
	m4char_t pBuffer[2048];
	m4string_t sHeader;
	sprintf(pBuffer, "\t%s%d\t%s%d\t%s%d\t%s%d\t%s%d\t%s%d\t%s%d\n\t%s\t%s", 
		"Tick Count: ", (m4uint32_t) dReferenceTime, 
		"Year: ", a_year, 
		"Month: ", a_month, 
		"Day: ", a_day, 
		"Hour: ", ai_ihour, 
		"Minute: ", ai_imin, 
		"Second: ", ai_isec, 
		"REQUEST ID", 
		"SERVICE ID");
	
	sHeader = pBuffer;

	m4string_t sStatHeader;
	ClRequestStatistics::DumpHeader(sStatHeader);
	sHeader.append(sStatHeader);
	
	sprintf(pBuffer, "\t%s", "SERVICE TRACE");
	sHeader.append(pBuffer);
	
	return sHeader;
}


M4_DECL_M4REQUEST M4LogFile g_oBenchmark("m4benchmark", 10 * 1024 * 1024, _CreateHeader().c_str());



//## begin ClRequest::s_pszPDUType%3815B79103A6.role preserve=yes  protected: static m4pcchar_t { -> 6VHAN}
m4pcchar_t ClRequest::s_pszPDUType[8]=
{"UNKNOWN", "CONNECT", "AUTHENTICATE", "CHANGE PWD", "SET ROLE", "CANCEL", "DISCONNECT", "PING"};
//## end ClRequest::s_pszPDUType%3815B79103A6.role

//## begin ClRequest::s_oDumpBlock%39491CD400B8.role preserve=yes  protected: static ClCriticalSection { -> 1VHAN}
ClCriticalSection ClRequest::s_oDumpBlock(M4_TRUE);
//## end ClRequest::s_oDumpBlock%39491CD400B8.role

//## begin ClRequest::s_oGarbageCollector%3950CD740255.role preserve=no  public: static ClRequestCollector { -> 1VHAN}
ClRequestCollector ClRequest::s_oGarbageCollector;
//## end ClRequest::s_oGarbageCollector%3950CD740255.role



ClRequest::ClRequest (m4uint32_t ai_uiReqType, m4pchar_t ai_szServiceId, ClRequestStatistics *ai_poRequestStats, m4handle_t ai_uiReqId, const m4string_t &ai_strDumpFile)
  //## begin ClRequest::ClRequest%875278536.hasinit preserve=no
      : m_iSessionId(0)
  //## end ClRequest::ClRequest%875278536.hasinit
  //## begin ClRequest::ClRequest%875278536.initialization preserve=yes
  ,	ClServiceRequest (ai_uiReqType, 0, ai_uiReqId, ai_poRequestStats), m_oStatus(ai_uiReqId), m_poConnection(0), m_pSession(0)
  , m_oLog (GETLOGMANAGER () , M4_TRUE , M4_TRUE )
  //## end ClRequest::ClRequest%875278536.initialization
{
  //## begin ClRequest::ClRequest%875278536.body preserve=yes
	m_oStatus.SetValue ( REQ_ACTIVE ) ;

	SetBin(s_oGarbageCollector);

	m_poMsgContainer = new ClSingleMsgContainer () ;

	m_poCredential = NULL ;

    m_pcServiceId = ai_szServiceId;
  //## end ClRequest::ClRequest%875278536.body
}


ClRequest::~ClRequest ()
{
  //## begin ClRequest::~ClRequest%878377069.body preserve=yes
	delete m_poMsgContainer ;

	if ( m_poCredential )
	{
		delete m_poCredential ;

		m_poCredential = NULL ;
	}

	if (0 != m_poStatistic)
    {
		delete m_poStatistic;
        m_poStatistic = NULL;
    }

  //## end ClRequest::~ClRequest%878377069.body
}


//## Other Operations (implementation)
ClStatistic * ClRequest::GetStats ()
{
  //## begin ClRequest::GetStats%890222207.body preserve=yes
	return m_poStatistic;
  //## end ClRequest::GetStats%890222207.body
}

m4pcchar_t ClRequest::GetServiceId ()
{
  //## begin ClRequest::GetServiceId%894621703.body preserve=yes
	if (NULL == m_pInputPDU)
    {
		return m_pcServiceId;
    }

	m4uint16_t pduType = m_pInputPDU -> GetTypePDU();

	m4pchar_t pServiceId ;
	if (M4_TYPE_PDU_SERVICE != pduType)
	{
		m4uint_t iOpId;
		iOpId = _GetReqType(m_pInputPDU ->GetIdPDU());
		return s_pszPDUType[iOpId];
	}

	(( ClRequestPDU * )m_pInputPDU) -> GetServiceId(pServiceId ) ;

	return pServiceId ;
  //## end ClRequest::GetServiceId%894621703.body
}

ClLogSystem * ClRequest::GetLog ()
{
  //## begin ClRequest::GetLog%896346104.body preserve=yes
	return &m_oLog ;
  //## end ClRequest::GetLog%896346104.body
}

m4return_t ClRequest::Cancel ()
{
  //## begin ClRequest::Cancel%898178462.body preserve=yes
	m4uint32_t iStatus ;
	m4uint32_t iRequestId ;
	ClPDU	*poOutputPDU = NULL;
	ClProtocol oProtocol ;		
	m4char_t	pszMessage[64];
		
	iStatus = m_oStatus.GetValue () ;

//	ADD	EMN	981026
	if (NULL == m_pInputPDU)
		return M4_FALSE;
	iRequestId = m_pInputPDU -> GetRequestId () ;
//	END	ADD

	switch ( iStatus ) 
	{

	case REQ_ACTIVE :
	case REQ_CANCELING :

		if ( m_pOutputPDU )
		{
//	DEL	EMN	981026			iRequestId = m_pOutputPDU -> GetRequestId () ;

			delete m_pOutputPDU;

			m_pOutputPDU  = NULL ;
		}

//		poOutputPDU  =  m_poConnection -> m_oProtocol.GetPDU(M4_ID_PDU_ERROR);

		if ( m_poConnection )
			poOutputPDU  =  ((ClConnBase*)m_poConnection ) -> m_oProtocol.GetPDU(M4_ID_PDU_ERROR);

		if ( ! poOutputPDU  )
		{
			return M4_ERROR ;
		}

		poOutputPDU  -> SetRequestId ( iRequestId ) ;

		m_oStatus.SetValue ( REQ_CANCELED ) ;

		SetPDUOutput(poOutputPDU);

		m_oLog.StartCode( M4_SRV_ERROR_IN_PDU , ERRORLOG );

		sprintf(pszMessage, "Request %0d Canceled.", iRequestId);
		m_oLog.AddText( pszMessage );

		m_oLog.SetCode();

		break ;

	case REQ_CANCELED :

		m4Trace( cout << "Canceling a canceled request" << endl ) ;

		break ;

	default :

		m4Trace( cout << "Canceling a request without status" << endl ) ;

		return M4_ERROR ;
	}

	return M4_SUCCESS ;
  //## end ClRequest::Cancel%898178462.body
}

m4return_t ClRequest::Recycle ()
{
  //## begin ClRequest::Recycle%941721563.body preserve=yes

	if ( ! CanRecycle () )
	{
		return M4_ERROR ;
	}


	if ( m_poStatistic)
	{
		delete m_poStatistic;

		m_poStatistic = NULL ;
	}

	if ( m_pOutputPDU )
	{
		delete m_pOutputPDU ;

		m_pOutputPDU = NULL ;
	}
	if ( m_pInputPDU )
	{
		delete m_pInputPDU ;

		m_pInputPDU = NULL ;
	}

	if ( m_szServiceId )
	{
		delete m_szServiceId ;

		m_szServiceId = NULL ;
	}

	m_poConnection = NULL ;

	m_pSession = NULL ;

	m_uiRequestID = 0 ;

	m_uiPriority = 0 ;

	m_uiRequestType = 0 ;

	return M4_SUCCESS ;
  //## end ClRequest::Recycle%941721563.body
}

m4bool_t ClRequest::CanRecycle ()
{
  //## begin ClRequest::CanRecycle%941721564.body preserve=yes
	return M4_TRUE;
  //## end ClRequest::CanRecycle%941721564.body
}

m4return_t ClRequest::Destroy ()
{
  //## begin ClRequest::Destroy%941721565.body preserve=yes
	_Dump();

	delete this ;

	return M4_SUCCESS ;
  //## end ClRequest::Destroy%941721565.body
}

m4bool_t ClRequest::CanDestroy ()
{
  //## begin ClRequest::CanDestroy%941721566.body preserve=yes
	return M4_TRUE ;
  //## end ClRequest::CanDestroy%941721566.body
}

m4return_t ClRequest::Initialize (m4string_t &ai_strUserName)
{
  //## begin ClRequest::Initialize%962640087.body preserve=yes
	m4objid_t idRequest = 0 ;
	m4char_t szID [20] ="";
	m4string_t szCredential , strID , szUser , szServer , strHost , strBasePort;
	m4objid_t idSession = 0;


	if ( ! GetId() )
	{
		if ( m_pInputPDU )
		{
			ClLogSystem * poLog = GetLog() ;
			
			poLog -> SetAttributeValue ( "M4_LOGSYSTEM_USERID" , ai_strUserName.c_str() ) ;
			
			m4uint32_t idPDU = m_pInputPDU -> GetRequestId ( ) ;
			
			SetRequestId ( idPDU ) ;

			//aquí se debe de buscar el ID de sesión de la credencial
			if ( ! m_iSessionId )
			{
				GetCredential ( ) ;

				if ( m_poCredential)
				{
					m_iSessionId = m_poCredential -> GetSessId () ;
					szUser = m_poCredential->m_strLogin;
				}
			}
	
			if ( m_iSessionId )
			{
				sprintf ( szID , "%lu" , (m4uint32_t)m_iSessionId ) ;
			}

			ClIdGenerator::GetServerKey ( strHost, strBasePort ) ;

			strID = szID ;

			SetUniqueRequestID ( strHost , strBasePort , strID, szUser ) ;

			if (M4_ERROR ==	GenerateId (idPDU , idRequest ) )
			{
				return M4_ERROR ;
			}

			SetId ( idRequest ) ;

		}
	}		

	return M4_SUCCESS;
  //## end ClRequest::Initialize%962640087.body
}


m4uint_t ClRequest::_GetReqType (m4uint_t ai_uPDUType)
{
  //## begin ClRequest::_GetReqType%941108816.body preserve=yes
	m4uint_t	uIndex = 0;

	switch(ai_uPDUType)
	{
		case M4_ID_PDU_PING:			// TIMEOUT PING
			uIndex = 7;
			break;

		case M4_ID_PDU_DISCONNECT:
		case M4_ID_PDU_NEWDISCONNECT:
			uIndex = 6;
			break;

		case M4_ID_PDU_CANCEL:			//	REQUEST CANCELATION
		case M4_ID_PDU_NEWCANCEL:			//	REQUEST CANCELATION
			uIndex = 5;
			break;

		case M4_ID_PDU_SETROLE:
		case M4_ID_PDU_NEWSETROLE:
			uIndex = 4;
			break;

		case M4_ID_PDU_CHANGEPASSWORD:			//	USER IDENTIFICATION
		case M4_ID_PDU_NEWCHANGEPASSWORD:			//	USER IDENTIFICATION
			uIndex = 3;
			break;

		case M4_ID_PDU_AUTHENTICATION:			//	USER IDENTIFICATION
			uIndex = 2;
			break;

 		case M4_ID_PDU_CONNECT:
			uIndex = 1;
			break;

		default:
			uIndex = 0;						// PDU UNKNOWN

	}

	return uIndex;
  //## end ClRequest::_GetReqType%941108816.body
}

void ClRequest::_Dump ()
{
  //## begin ClRequest::_Dump%940939221.body preserve=yes

    if (NULL == m_poStatistic)
	{
		return;
	}

	if (ClVMDebugEnv::getInstance()->getDebugLevel() < 1)
	{
		return;
	}

	ClBlockSync	oBlocker(s_oDumpBlock);

    m4string_t sMessage;

	// request id.
    m4string_t sRequestID = "-";
	GetUniqueRequestID(sRequestID);
	sMessage.append("\t");
	sMessage.append(sRequestID);

	// header.
    m4string_t sServiceId = "-";
    m4pcchar_t pcServiceId = GetServiceId();
    if (NULL != pcServiceId)
    {
        sServiceId = pcServiceId;
    }
	sMessage.append("\t");
	sMessage.append(sServiceId);
	
	// statistics.
    m4string_t sStats;
	if (NULL != m_poStatistic)
	{
		((ClRequestStatistics *)m_poStatistic)->Dump(sStats, pcServiceId);
		sMessage.append(sStats);
	}

	// eh 279583. Adds user-timed-actions-auditory primary key columns in trace files.
	if( m_iSessionId )
	{
		m4string_t strHost, strBasePort;
		ClIdGenerator::GetServerKey( strHost, strBasePort );

		sMessage.append( "\t" );
		sMessage.append( strHost );

		sMessage.append( "\t" );
		sMessage.append( strBasePort );

		m4char_t szID [20] = "";
		sprintf( szID , "%lu" , m_iSessionId ) ;
		sMessage.append( "\t" );
		sMessage.append( szID );
	}

	// trace.
    m4string_t sTrace;
    m_poMsgContainer->GetTraceMessage(sTrace);
    sMessage.append(sTrace);

	// Print message to file.
	g_oBenchmark.WriteLine("%s", sMessage.c_str());
  //## end ClRequest::_Dump%940939221.body
}

m4return_t ClRequest::SetError (m4return_t ai_iError)
{
  //## begin ClRequest::SetError%975430311.body preserve=yes
	m4uint32_t iRequestId ;
	ClPDU	*poOutputPDU = NULL;
	ClProtocol oProtocol ;		

		

	if (NULL == m_pInputPDU)
		return M4_FALSE;
	iRequestId = m_pInputPDU -> GetRequestId () ;


	if ( m_pOutputPDU )
	{
		delete m_pOutputPDU;

		m_pOutputPDU  = NULL ;
	}


	if ( m_poConnection )
	{
		if (! ( poOutputPDU  =  ((ClConnBase*)m_poConnection ) -> m_oProtocol.GetPDU(M4_ID_PDU_ERROR) ) )
		{
			return M4_ERROR ;
		}

		if ( ai_iError )
		{
			if ( M4_PDU_40_VERSION != ((ClConnBase*)m_poConnection ) -> m_oProtocol.GetVersion() )
//			if ( M4_CONNECTION_ORIENTED_PROTOCOL == m_poConnection -> GetConnProtocol () )
			{
				((ClErrorPDU *) poOutputPDU  ) ->SetErrorCode ( ai_iError ) ;
			}
			else
			{
				((ClNewErrorPDU *) poOutputPDU  ) ->SetErrorCode ( ai_iError ) ;
			}
		}
	}

	poOutputPDU  -> SetRequestId ( iRequestId ) ;

	SetPDUOutput(poOutputPDU);


	return M4_SUCCESS ;
  //## end ClRequest::SetError%975430311.body
}

m4return_t ClRequest::GenerateId (m4uint32_t ai_iClientReqId, m4objid_t &ao_lServerReqId)
{
  //## begin ClRequest::GenerateId%978434330.body preserve=yes
	m4uint32_t idSession ;

	idSession = m_iSessionId ;
			
	if ( (! idSession ) && ( m_poConnection) )
	{
		idSession = m_poConnection -> GetId () ;
	}
			
	ao_lServerReqId = idSession ;
	
	ao_lServerReqId <<= 32 ;
			
	ao_lServerReqId |= ai_iClientReqId ;

	return M4_SUCCESS ;
  //## end ClRequest::GenerateId%978434330.body
}

ClRequest * ClRequest::DuplicateWrapper ()
{
  //## begin ClRequest::DuplicateWrapper%990443888.body preserve=yes
	ClPDU * poOutputPDU  , * poInputPDU  ;
	ClRequest * poRequest ;
	ClMessageContainer * poMsgContainer ;
	m4pchar_t szServiceId , pServiceId;
	m4uint32_t iServiceIdLen ;


	poRequest = new ClRequest (GetRequestType(), NULL, (ClRequestStatistics*) GetStats () , GetRequestId () ) ;

	if ( ! m_poConnection )
	{
		return NULL ;
	}

	if (! ( poOutputPDU  =  ((ClConnBase*)m_poConnection ) -> m_oProtocol.GetPDU(M4_ID_PDU_ERROR) ) )
	{
		return NULL ;
	}

	if (! ( poInputPDU  =  ((ClConnBase*)m_poConnection ) -> m_oProtocol.GetPDU(M4_ID_PDU_REQUEST ) ) )
	{
		return NULL ;
	}

	if ( M4_PDU_40_VERSION != ((ClConnBase*)m_poConnection ) -> m_oProtocol.GetVersion() )
//	if ( M4_CONNECTION_ORIENTED_PROTOCOL == m_poConnection -> GetConnProtocol () )
	{
		((ClErrorPDU *) poOutputPDU  ) ->SetErrorCode ( M4_ERROR ) ;
	}
	else
	{
		((ClNewErrorPDU *) poOutputPDU  ) ->SetErrorCode ( M4_ERROR) ;
	}

//	poRequest ->SetPDUOutput ( poOutputPDU ) ;
	poRequest ->m_bOwnerOutputPDU = m_bOwnerOutputPDU;

	poRequest ->m_pOutputPDU = poOutputPDU ;

	poRequest ->m_pInputPDU = poInputPDU ;

	poOutputPDU -> SetRequestId ( m_pInputPDU -> GetRequestId () ) ;

	poOutputPDU -> Inicialize(m_pInputPDU);

	poInputPDU -> Inicialize(m_pInputPDU);

	iServiceIdLen = (( ClRequestPDU * )m_pInputPDU) -> GetServiceId ( pServiceId ) ;

	szServiceId = new m4char_t [ iServiceIdLen +1 ] ;

	memcpy (szServiceId , pServiceId , iServiceIdLen ) ;

	szServiceId [ iServiceIdLen ] = 0;

	(( ClRequestPDU * )poInputPDU) -> SetServiceId( szServiceId ,  iServiceIdLen ) ;

	poRequest ->SetSession ( GetpSession () ) ;

	poRequest ->SetConnection ( GetpConnection () ) ;

	poRequest ->m_poCredential = m_poCredential ;

	poRequest -> SetId ( GetId () ) ;

	poRequest -> m_strUniqueId = m_strUniqueId ;

	poMsgContainer = poRequest -> m_poMsgContainer = m_poMsgContainer ;

	poMsgContainer -> StoreTraceMessage ( "FirstChanceException Executing Request" ) ;

	poRequest -> m_uiRequestType = m_uiRequestType ;

	poRequest -> m_iSessionId = m_iSessionId ;

	return poRequest ;
  //## end ClRequest::DuplicateWrapper%990443888.body
}

M4Credentials * ClRequest::GetCredential ()
{
  //## begin ClRequest::GetCredential%1068803570.body preserve=yes
	m4string_t strSessionID ;

	if ( m_poCredential ) 
	{
		return m_poCredential ;
	}


	if ( M4_ERROR == m_pInputPDU -> GetCredential ( strSessionID ) )
	{
		return NULL ;
	}

	m_poCredential =  ClCredentialsFactory::CreateCredential ( strSessionID ) ;


	return m_poCredential ;
  //## end ClRequest::GetCredential%1068803570.body
}


// Additional Declarations
  //## begin ClRequest%342522EA0065.declarations preserve=yes
  //## end ClRequest%342522EA0065.declarations
//## begin module%350915C103E7.epilog preserve=yes
//## end module%350915C103E7.epilog

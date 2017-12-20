//## begin module%3459D97B03DF.cm preserve=no
//## end module%3459D97B03DF.cm

//## begin module%3459D97B03DF.cp preserve=no
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
//## end module%3459D97B03DF.cp

//## Module: LMStats%3459D97B03DF; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: F:\integration\m4comsrv\src\lmstats.cpp

//## begin module%3459D97B03DF.additionalIncludes preserve=no
//## end module%3459D97B03DF.additionalIncludes

//## begin module%3459D97B03DF.includes preserve=yes
#include <fcntl.h>
#include <sys/stat.h>
#include <request.hpp>
#include <clserviceinfo.hpp>
#include <stdio.h>
#include <subsystemids.hpp>
#include <cllocalmonitor.hpp>
#include <pdus.hpp>
#include <m4srvres.hpp>


#include <sys/types.h>

#ifdef _UNIX
#include <unistd.h>
#else
#include <io.h>
#endif
//## end module%3459D97B03DF.includes

// Statistics
#include <statistics.hpp>
// LMStats
#include <lmstats.hpp>


//## begin module%3459D97B03DF.declarations preserve=no
//## end module%3459D97B03DF.declarations

//## begin module%3459D97B03DF.additionalDeclarations preserve=yes
//## end module%3459D97B03DF.additionalDeclarations


// Class ClLMVisitor 


ClLMVisitor::ClLMVisitor ()
  //## begin ClLMVisitor::ClLMVisitor%879606157.hasinit preserve=no
  //## end ClLMVisitor::ClLMVisitor%879606157.hasinit
  //## begin ClLMVisitor::ClLMVisitor%879606157.initialization preserve=yes
  : ClVisitor (  )
  //## end ClLMVisitor::ClLMVisitor%879606157.initialization
{
  //## begin ClLMVisitor::ClLMVisitor%879606157.body preserve=yes
  //## end ClLMVisitor::ClLMVisitor%879606157.body
}


ClLMVisitor::~ClLMVisitor ()
{
  //## begin ClLMVisitor::~ClLMVisitor%878302313.body preserve=yes
  //## end ClLMVisitor::~ClLMVisitor%878302313.body
}



//## Other Operations (implementation)
void ClLMVisitor::VisitLM (ClLocalMonitor &ai_oLocalMonitor)
{
  //## begin ClLMVisitor::VisitLM%878200307.body preserve=yes
  //## end ClLMVisitor::VisitLM%878200307.body
}

// Class ClLMStats 























ClLMStats::ClLMStats (m4bool_t ai_bOuttoFile)
  //## begin ClLMStats::ClLMStats%887021576.hasinit preserve=no
      : m_oTotalRequests(0), m_oLastInputBytes(0), m_oLastOutputBytes(0), m_iNumConnections(0)
  //## end ClLMStats::ClLMStats%887021576.hasinit
  //## begin ClLMStats::ClLMStats%887021576.initialization preserve=yes
  , m_oLastRecTime ( 0 ) ,m_oLastSendTime ( 0 ) 
  , m_oLastRegTime ( 0 ) ,m_oLastDeRegTime ( 0 ) 
  , m_oLastGetSessTime ( 0 ) ,m_oLastTotalTime ( 0 ) 
  , m_oAvgRecTime(12, 180), m_oAvgSendTime(12, 180)
  , m_oAvgRegTime(12, 180), m_oAvgDeRegTime(12, 180)
  , m_oAvgOutputBytes(12, 180), m_oAvgInputBytes(12, 180)
  , m_oAvgGetSessTime(12, 180), m_oAvgTotalTime(12, 180) ,m_oAvgProcessedReq (12,180)
  //## end ClLMStats::ClLMStats%887021576.initialization
{
  //## begin ClLMStats::ClLMStats%887021576.body preserve=yes
/*	m_oAvgRecTime.AddTerm(5);
	m_oAvgRecTime.AddTerm(3);

	m_oAvgSendTime.AddTerm(5);
	m_oAvgSendTime.AddTerm(3);

	m_oAvgRegTime.AddTerm(5);
	m_oAvgRegTime.AddTerm(3);

	m_oAvgDeRegTime.AddTerm(5);
	m_oAvgDeRegTime.AddTerm(3);

	m_oAvgGetSessTime.AddTerm(5);
	m_oAvgGetSessTime.AddTerm(3);

	m_oAvgInputBytes.AddTerm(5);
	m_oAvgInputBytes.AddTerm(3);

	m_oAvgOutputBytes.AddTerm(5);
	m_oAvgOutputBytes.AddTerm(3);

	m_oAvgTotalTime.AddTerm(5);
	m_oAvgTotalTime.AddTerm(3);

	m_oAvgProcessedReq.AddTerm(5);
	m_oAvgProcessedReq.AddTerm(3);
*/


	m_bOuttoFile = ai_bOuttoFile ;
	m_uiNumberOfStats = TOTAL_NUM ;

	m_poRequestOutputFile = NULL ;
  //## end ClLMStats::ClLMStats%887021576.body
}


ClLMStats::~ClLMStats ()
{
  //## begin ClLMStats::~ClLMStats%887021577.body preserve=yes
  //## end ClLMStats::~ClLMStats%887021577.body
}



//## Other Operations (implementation)
m4return_t ClLMStats::Reset ()
{
  //## begin ClLMStats::Reset%879417135.body preserve=yes
	return M4_SUCCESS ;
  //## end ClLMStats::Reset%879417135.body
}

m4return_t ClLMStats::GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_oValue)
{
  //## begin ClLMStats::GetStat%886498831.body preserve=yes
	m4uint16_t				lowIndex;
	eLocalMonitorStatsID		statIndex;
	m4return_t				res = M4_SUCCESS ;

	if (M4_ERROR == _CheckStatID(ai_hStatId, M4_LM_SUBSYSTEM))
		return M4_ERROR;

	lowIndex = ai_hStatId.GetLowID();

	statIndex = (eLocalMonitorStatsID)lowIndex;

	if (0 < ao_oValue.GetSize())
		return M4_ERROR;

	ao_oValue.SetSize(LM_DEF_STRING_SIZE);
	m4pchar_t	buffer = ao_oValue.GetBuffer();


//
/*	enum{
			INCORRECT_PDUS_ID,
			UNKNOWN_HOSTS_ID,
			UNKNOWN_USERS_ID,
			INVALID_TIME_MARKS_ID,
			INVALID_PROTOCOLV_ID,
			INVALID_USER_AUT_ID,
			FORGED_SIGN_ID,
			DENIED_CONNECTS_ID,
			TIMEOUTS_ID,
			TOTAL_NUM
			} */

	switch(statIndex)
	{
	case FIF_M_AVG_REC_TIME:
		sprintf ( buffer , "%10.0f" , m_oAvgRecTime.GetAvg(0) ) ;
		break;
	case FIV_S_AVG_REC_TIME:
		sprintf ( buffer , "%10.0f" , m_oAvgRecTime.GetAvg(1) ) ;
		break;
	case THIR_S_AVG_REC_TIME:
		sprintf ( buffer , "%10.0f" , m_oAvgRecTime.GetAvg(6) ) ;
		break;
	case FIV_M_AVG_REC_TIME:
		sprintf ( buffer , "%10.0f" , m_oAvgRecTime.GetAvg(60) ) ;
		break;

	case FIF_M_AVG_SEND_TIME:
		sprintf ( buffer , "%10.0f" , m_oAvgSendTime.GetAvg(0) ) ;
		break;
	case FIV_S_AVG_SEND_TIME:
		sprintf ( buffer , "%10.0f" , m_oAvgSendTime.GetAvg(1) ) ;
		break;
	case THIR_S_AVG_SEND_TIME:
		sprintf ( buffer , "%10.0f" , m_oAvgSendTime.GetAvg(6) ) ;
		break;
	case FIV_M_AVG_SEND_TIME:
		sprintf ( buffer , "%10.0f" , m_oAvgSendTime.GetAvg(60) ) ;
		break;

	case FIF_M_AVG_TOTAL_TIME:
		sprintf ( buffer , "%10.0f" , m_oAvgTotalTime.GetAvg(0) ) ;
		break;
	case FIV_S_AVG_TOTAL_TIME:
		sprintf ( buffer , "%10.0f" , m_oAvgTotalTime.GetAvg(1) ) ;
		break;
	case THIR_S_AVG_TOTAL_TIME:
		sprintf ( buffer , "%10.0f" , m_oAvgTotalTime.GetAvg(6) ) ;
		break;
	case FIV_M_AVG_TOTAL_TIME:
		sprintf ( buffer , "%10.0f" , m_oAvgTotalTime.GetAvg(60) ) ;
		break;

	case FIF_M_AVG_REG_TIME:
		sprintf ( buffer , "%10.0f" , m_oAvgRegTime.GetAvg(0) ) ;
		break;
	case FIV_S_AVG_REG_TIME:
		sprintf ( buffer , "%10.0f" , m_oAvgRegTime.GetAvg(1) ) ;
		break;
	case THIR_S_AVG_REG_TIME:
		sprintf ( buffer , "%10.0f" , m_oAvgRegTime.GetAvg(6) ) ;
		break;
	case FIV_M_AVG_REG_TIME:
		sprintf ( buffer , "%10.0f" , m_oAvgRegTime.GetAvg(60) ) ;
		break;

	case FIF_M_AVG_DEREG_TIME:
		sprintf ( buffer , "%10.0f" , m_oAvgDeRegTime.GetAvg(0) ) ;
		break;
	case FIV_S_AVG_DEREG_TIME:
		sprintf ( buffer , "%10.0f" , m_oAvgDeRegTime.GetAvg(1) ) ;
		break;
	case THIR_S_AVG_DEREG_TIME:
		sprintf ( buffer , "%10.0f" , m_oAvgDeRegTime.GetAvg(6) ) ;
		break;
	case FIV_M_AVG_DEREG_TIME:
		sprintf ( buffer , "%10.0f" , m_oAvgDeRegTime.GetAvg(60) ) ;
		break;

	case FIF_M_AVG_GETSESS_TIME:
		sprintf ( buffer , "%10.0f" , m_oAvgGetSessTime.GetAvg(0) ) ;
		break;
	case FIV_S_AVG_GETSESS_TIME:
		sprintf ( buffer , "%10.0f" , m_oAvgGetSessTime.GetAvg(1) ) ;
		break;
	case THIR_S_AVG_GETSESS_TIME:
		sprintf ( buffer , "%10.0f" , m_oAvgGetSessTime.GetAvg(6) ) ;
		break;
	case FIV_M_AVG_GETSESS_TIME:
		sprintf ( buffer , "%10.0f" , m_oAvgGetSessTime.GetAvg(60) ) ;
		break;

	case FIF_M_AVG_INPUT_BYTES:
		sprintf ( buffer , "%10.0f" , m_oAvgInputBytes.GetAvg(0) ) ;
		break;
	case FIV_S_AVG_INPUT_BYTES:
		sprintf ( buffer , "%10.0f" , m_oAvgInputBytes.GetAvg(1) ) ;
		break;
	case THIR_S_AVG_INPUT_BYTES:
		sprintf ( buffer , "%10.0f" , m_oAvgInputBytes.GetAvg(6) ) ;
		break;
	case FIV_M_AVG_INPUT_BYTES:
		sprintf ( buffer , "%10.0f" , m_oAvgInputBytes.GetAvg(60) ) ;
		break;

	case FIF_M_AVG_OUTPUT_BYTES:
		sprintf ( buffer , "%10.0f" , m_oAvgOutputBytes.GetAvg(0) ) ;
		break;
	case FIV_S_AVG_OUTPUT_BYTES:
		sprintf ( buffer , "%10.0f" , m_oAvgOutputBytes.GetAvg(1) ) ;
		break;
	case THIR_S_AVG_OUTPUT_BYTES:
		sprintf ( buffer , "%10.0f" , m_oAvgOutputBytes.GetAvg(6) ) ;
		break;
	case FIV_M_AVG_OUTPUT_BYTES:
		sprintf ( buffer , "%10.0f" , m_oAvgOutputBytes.GetAvg(60) ) ;
		break;

	case FIF_M_AVG_PROCESSED:
		sprintf ( buffer , "%10.0f" , m_oAvgProcessedReq.GetAvg(0) ) ;
		break;
	case FIV_S_AVG_PROCESSED:
		sprintf ( buffer , "%10.0f" , m_oAvgProcessedReq.GetAvg(1) ) ;
		break;
	case THIR_S_AVG_PROCESSED:
		sprintf ( buffer , "%10.0f" , m_oAvgProcessedReq.GetAvg(6) ) ;
		break;
	case FIV_M_AVG_PROCESSED:
		sprintf ( buffer , "%10.0f" , m_oAvgProcessedReq.GetAvg(60) ) ;
		break;

	case PROCESSED_REQUEST:
		sprintf ( buffer , "%10d" , m_oTotalRequests ) ;
		break;
	case NUM_CONNECTION :
		sprintf ( buffer , "%10d" , m_iNumConnections ) ;
		break;
	default:
		sprintf(buffer, "%010d", 0);
		res = M4_ERROR;
	}
	
	return res ;
  //## end ClLMStats::GetStat%886498831.body
}

m4return_t ClLMStats::SetStat (ClStatHandle &ai_hStatId, const m4pchar_t ao_pszValue)
{
  //## begin ClLMStats::SetStat%886498832.body preserve=yes
	return M4_SUCCESS ;
  //## end ClLMStats::SetStat%886498832.body
}

m4return_t ClLMStats::ResetStat (ClStatHandle &ai_hStatId)
{
  //## begin ClLMStats::ResetStat%886498833.body preserve=yes
	return M4_SUCCESS ;
  //## end ClLMStats::ResetStat%886498833.body
}

m4void_t ClLMStats::AddProcessedRequests (m4uint32_t ai_NewValue)
{
  //## begin ClLMStats::AddProcessedRequests%890061476.body preserve=yes

	m_oTotalRequests += ai_NewValue;

  //## end ClLMStats::AddProcessedRequests%890061476.body
}

m4void_t ClLMStats::AddAvgRecTime (ClStatAverageType ai_NewValue)
{
  //## begin ClLMStats::AddAvgRecTime%983982351.body preserve=yes
	m_oAvgRecTime.Update(ai_NewValue);
  //## end ClLMStats::AddAvgRecTime%983982351.body
}

m4void_t ClLMStats::AddAvgSendTime (ClStatAverageType ai_NewValue)
{
  //## begin ClLMStats::AddAvgSendTime%983982352.body preserve=yes
	m_oAvgSendTime.Update(ai_NewValue);
  //## end ClLMStats::AddAvgSendTime%983982352.body
}

m4void_t ClLMStats::AddAvgRegTime (ClStatAverageType ai_NewValue)
{
  //## begin ClLMStats::AddAvgRegTime%983982353.body preserve=yes
	m_oAvgRegTime.Update(ai_NewValue);
  //## end ClLMStats::AddAvgRegTime%983982353.body
}

m4void_t ClLMStats::AddAvgGetSessTime (ClStatAverageType ai_NewValue)
{
  //## begin ClLMStats::AddAvgGetSessTime%983982354.body preserve=yes
	m_oAvgGetSessTime.Update(ai_NewValue);
  //## end ClLMStats::AddAvgGetSessTime%983982354.body
}

m4void_t ClLMStats::AddAvgDeRegTime (ClStatAverageType ai_NewValue)
{
  //## begin ClLMStats::AddAvgDeRegTime%983982355.body preserve=yes
	m_oAvgDeRegTime.Update(ai_NewValue);
  //## end ClLMStats::AddAvgDeRegTime%983982355.body
}

m4void_t ClLMStats::AddAvgTotalTime (ClStatAverageType ai_NewValue)
{
  //## begin ClLMStats::AddAvgTotalTime%983982356.body preserve=yes
	m_oAvgTotalTime.Update(ai_NewValue);
  //## end ClLMStats::AddAvgTotalTime%983982356.body
}

m4void_t ClLMStats::AddAvgInputBytes (ClStatAverageType ai_NewValue)
{
  //## begin ClLMStats::AddAvgInputBytes%983982357.body preserve=yes
	m_oAvgInputBytes.Update(ai_NewValue);

  //## end ClLMStats::AddAvgInputBytes%983982357.body
}

m4void_t ClLMStats::AddAvgOutputBytes (ClStatAverageType ai_NewValue)
{
  //## begin ClLMStats::AddAvgOutputBytes%983982358.body preserve=yes
	m_oAvgOutputBytes.Update(ai_NewValue);
  //## end ClLMStats::AddAvgOutputBytes%983982358.body
}

m4void_t ClLMStats::AddAvgProcessedReq (ClStatAverageType ai_NewValue)
{
  //## begin ClLMStats::AddAvgProcessedReq%983982359.body preserve=yes
	m_oAvgProcessedReq.Update(ai_NewValue);
  //## end ClLMStats::AddAvgProcessedReq%983982359.body
}

m4void_t ClLMStats::AddInputBytes (m4int32_t ai_NewValue)
{
  //## begin ClLMStats::AddInputBytes%983982360.body preserve=yes
	m_oLastInputBytes += ai_NewValue ;
  //## end ClLMStats::AddInputBytes%983982360.body
}

m4void_t ClLMStats::AddOutputBytes (m4int32_t ai_NewValue)
{
  //## begin ClLMStats::AddOutputBytes%983982361.body preserve=yes
	m_oLastOutputBytes += ai_NewValue ;
  //## end ClLMStats::AddOutputBytes%983982361.body
}

m4uint32_t ClLMStats::GetProcessedReq ()
{
  //## begin ClLMStats::GetProcessedReq%983982362.body preserve=yes
	return m_oTotalRequests ;
  //## end ClLMStats::GetProcessedReq%983982362.body
}

m4uint32_t ClLMStats::GetLastInputBytes ()
{
  //## begin ClLMStats::GetLastInputBytes%983982363.body preserve=yes
	return m_oLastInputBytes ;
  //## end ClLMStats::GetLastInputBytes%983982363.body
}

m4uint32_t ClLMStats::GetLastOutputBytes ()
{
  //## begin ClLMStats::GetLastOutputBytes%983982364.body preserve=yes
	return m_oLastOutputBytes ;
  //## end ClLMStats::GetLastOutputBytes%983982364.body
}

m4return_t ClLMStats::Computer (ClRequest *ai_poRequest)
{
  //## begin ClLMStats::Computer%984138917.body preserve=yes
	ClRequestStatistics * poStats ;


	if (! ai_poRequest )
	{
		return M4_ERROR ;
	}

	poStats = (ClRequestStatistics*) ai_poRequest->GetStats() ;

	AddProcessedRequests (1) ;
	

	ClPDU	*poPDU = ai_poRequest->GetOutputPDU() ;

	if( poPDU != NULL )
	{
		AddOutputBytes ( poPDU->GetSize() ) ;

		/* Bug 0113962
		Se vuelca el tamaño de la pdu
		*/
		m4uint32_t iSize = poPDU->GetDeltaSize() ;

		if( iSize == 0 )
		{
			// Si no es de tipo delta se toma el tamaño normal
			iSize = poPDU->GetSize() ;
		}

		((ClRequestStatistics*) ai_poRequest->GetStats () )->SetOutputSize( iSize );
	}

	if ( ! poStats )
	{
		return M4_ERROR ;
	}

	m_oLastRecTime += poStats -> Gett(ClRequestStatistics::Reading  ) ;
	m_oLastSendTime += poStats -> Gett ( ClRequestStatistics::Writing  ) ;
	m_oLastRegTime += poStats -> Gett ( ClRequestStatistics::Registering  ) ;
	m_oLastDeRegTime += poStats -> Gett ( ClRequestStatistics::Deregistering  ) ;
	m_oLastGetSessTime += poStats -> Gett ( ClRequestStatistics::GettingSession  ) ;
	m_oLastTotalTime += poStats -> GetInProcess( ) ;

	return M4_SUCCESS ;
  //## end ClLMStats::Computer%984138917.body
}

m4void_t ClLMStats::AddRecTime (ClStatTimeType ai_iTime)
{
  //## begin ClLMStats::AddRecTime%984138918.body preserve=yes
	m_oLastRecTime += ai_iTime ;
  //## end ClLMStats::AddRecTime%984138918.body
}

m4void_t ClLMStats::AddSendTime (ClStatTimeType ai_iTime)
{
  //## begin ClLMStats::AddSendTime%984138919.body preserve=yes
	m_oLastSendTime += ai_iTime ;
  //## end ClLMStats::AddSendTime%984138919.body
}

m4void_t ClLMStats::AddRegTime (ClStatTimeType ai_iTime)
{
  //## begin ClLMStats::AddRegTime%984138920.body preserve=yes
	m_oLastRegTime += ai_iTime ;
  //## end ClLMStats::AddRegTime%984138920.body
}

m4void_t ClLMStats::AddDeRegTime (ClStatTimeType ai_iTime)
{
  //## begin ClLMStats::AddDeRegTime%984138921.body preserve=yes
	m_oLastDeRegTime += ai_iTime ;
  //## end ClLMStats::AddDeRegTime%984138921.body
}

m4void_t ClLMStats::AddGetSessTime (ClStatTimeType ai_iTime)
{
  //## begin ClLMStats::AddGetSessTime%984138922.body preserve=yes
	m_oLastGetSessTime += ai_iTime ;
  //## end ClLMStats::AddGetSessTime%984138922.body
}

m4void_t ClLMStats::AddTotalTime (ClStatTimeType ai_iTime)
{
  //## begin ClLMStats::AddTotalTime%984138923.body preserve=yes
	m_oLastTotalTime += ai_iTime ;
  //## end ClLMStats::AddTotalTime%984138923.body
}

ClStatTimeType ClLMStats::GetLastRecTime ()
{
  //## begin ClLMStats::GetLastRecTime%984138924.body preserve=yes
	return m_oLastRecTime ;
  //## end ClLMStats::GetLastRecTime%984138924.body
}

ClStatTimeType ClLMStats::GetLastSendTime ()
{
  //## begin ClLMStats::GetLastSendTime%984138925.body preserve=yes
	return m_oLastSendTime ;
  //## end ClLMStats::GetLastSendTime%984138925.body
}

ClStatTimeType ClLMStats::GetLastRegTime ()
{
  //## begin ClLMStats::GetLastRegTime%984138926.body preserve=yes
	return m_oLastRegTime ;
  //## end ClLMStats::GetLastRegTime%984138926.body
}

ClStatTimeType ClLMStats::GetLastDeRegTime ()
{
  //## begin ClLMStats::GetLastDeRegTime%984138927.body preserve=yes
	return m_oLastDeRegTime ;
  //## end ClLMStats::GetLastDeRegTime%984138927.body
}

ClStatTimeType ClLMStats::GetLastGetSessTime ()
{
  //## begin ClLMStats::GetLastGetSessTime%984138928.body preserve=yes
	return m_oLastGetSessTime ;
  //## end ClLMStats::GetLastGetSessTime%984138928.body
}

ClStatTimeType ClLMStats::GetLastTotalTime ()
{
  //## begin ClLMStats::GetLastTotalTime%984138929.body preserve=yes
	return m_oLastTotalTime ;
  //## end ClLMStats::GetLastTotalTime%984138929.body
}

m4void_t ClLMStats::AddNewConnections (m4int32_t ai_iNumConn)
{
  //## begin ClLMStats::AddNewConnections%993565383.body preserve=yes
	m_iNumConnections += ai_iNumConn ;
  //## end ClLMStats::AddNewConnections%993565383.body
}

m4uint32_t ClLMStats::GetNumConn ()
{
  //## begin ClLMStats::GetNumConn%993565384.body preserve=yes
	return m_iNumConnections ;
  //## end ClLMStats::GetNumConn%993565384.body
}

// Additional Declarations
  //## begin ClLMStats%3459D7B40092.declarations preserve=yes
  //## end ClLMStats%3459D7B40092.declarations

// Class ClLMGBVisitor 

ClLMGBVisitor::ClLMGBVisitor ()
  //## begin ClLMGBVisitor::ClLMGBVisitor%936177779.hasinit preserve=no
  //## end ClLMGBVisitor::ClLMGBVisitor%936177779.hasinit
  //## begin ClLMGBVisitor::ClLMGBVisitor%936177779.initialization preserve=yes
  //## end ClLMGBVisitor::ClLMGBVisitor%936177779.initialization
{
  //## begin ClLMGBVisitor::ClLMGBVisitor%936177779.body preserve=yes
  //## end ClLMGBVisitor::ClLMGBVisitor%936177779.body
}


ClLMGBVisitor::~ClLMGBVisitor ()
{
  //## begin ClLMGBVisitor::~ClLMGBVisitor%936177780.body preserve=yes
  //## end ClLMGBVisitor::~ClLMGBVisitor%936177780.body
}



//## Other Operations (implementation)
void ClLMGBVisitor::VisitLM (ClLocalMonitor &ai_oLocalMonitor)
{
  //## begin ClLMGBVisitor::VisitLM%936177781.body preserve=yes
	m4uint32_t iLastOutputBytes , iLastInputBytes ;
	static m4millisec_t LastTime = 0 ;
	static m4uint32_t iLastProcessedReq = 0 ;
	m4uint32_t iProcessedReq ;
	m4millisec_t now = ClPrecisionTicker::GetTimeTick();
	m4millisec_t difftime ; 
	ClStatAverageType NewValue , iProcessedDiff;
	ClStatTimeType iLastRequestTime , iAvgReqTime;
	


	if (! LastTime )
	{
		LastTime = ClPrecisionTicker::GetTimeTick();

		return ;
	}

	difftime = now - LastTime ;

	ClLMStats	*pStats = (ClLMStats*) ai_oLocalMonitor.GetStats();

	iProcessedReq = pStats ->GetProcessedReq () ;
	iLastOutputBytes = pStats ->GetLastOutputBytes () ;
	iLastInputBytes = pStats ->GetLastInputBytes ();

	pStats ->AddOutputBytes ( iLastOutputBytes - 2*iLastOutputBytes);
	pStats ->AddInputBytes ( iLastInputBytes - 2*iLastInputBytes);

	if ( ! difftime )
		return ;

	NewValue = iLastInputBytes *1000/difftime  ;
	pStats ->AddAvgInputBytes ( NewValue) ;

	NewValue = iLastOutputBytes *1000/difftime  ;
	pStats ->AddAvgOutputBytes ( NewValue ) ;

	iProcessedDiff = iProcessedReq-iLastProcessedReq ;
	NewValue = (iProcessedDiff)*1000/difftime ;
	pStats ->AddAvgProcessedReq ( NewValue ) ;

	iLastRequestTime = pStats ->GetLastRecTime () ;
	iAvgReqTime = 0 ;
	if ( iProcessedDiff )
		iAvgReqTime = iLastRequestTime / iProcessedDiff ;
	pStats ->AddAvgRecTime ( iAvgReqTime  ) ;
	pStats ->AddRecTime ( iLastRequestTime - 2*iLastRequestTime ) ;
	
	iLastRequestTime = pStats ->GetLastSendTime () ;
	iAvgReqTime = 0 ;
	if ( iProcessedDiff )
		iAvgReqTime = iLastRequestTime / iProcessedDiff ;
	pStats ->AddAvgSendTime ( iAvgReqTime ) ;
	pStats ->AddSendTime ( iLastRequestTime - 2*iLastRequestTime ) ;
	
	iLastRequestTime = pStats ->GetLastRegTime () ;
	iAvgReqTime = 0 ;
	if ( iProcessedDiff )
		iAvgReqTime = iLastRequestTime / iProcessedDiff ;
	pStats ->AddAvgRegTime ( iAvgReqTime ) ;
	pStats ->AddRegTime ( iLastRequestTime - 2*iLastRequestTime ) ;
	
	iLastRequestTime = pStats ->GetLastDeRegTime () ;
	iAvgReqTime = 0 ;
	if ( iProcessedDiff )
		iAvgReqTime = iLastRequestTime / iProcessedDiff ;
	pStats ->AddAvgDeRegTime ( iAvgReqTime ) ;
	pStats ->AddDeRegTime ( iLastRequestTime - 2*iLastRequestTime ) ;
	
	iLastRequestTime = pStats ->GetLastGetSessTime () ;
	iAvgReqTime = 0 ;
	if ( iProcessedDiff )
		iAvgReqTime = iLastRequestTime / iProcessedDiff ;
	pStats ->AddAvgGetSessTime ( iAvgReqTime ) ;
	pStats ->AddGetSessTime ( iLastRequestTime - 2*iLastRequestTime ) ;
	
	iLastRequestTime = pStats ->GetLastTotalTime () ;
	iAvgReqTime = 0 ;
	if ( iProcessedDiff )
		iAvgReqTime = iLastRequestTime / iProcessedDiff ;
	pStats ->AddAvgTotalTime ( iAvgReqTime ) ;
	pStats ->AddTotalTime ( iLastRequestTime - 2*iLastRequestTime ) ;
	
	LastTime = ClPrecisionTicker::GetTimeTick();
	iLastProcessedReq = pStats ->GetProcessedReq () ;

  //## end ClLMGBVisitor::VisitLM%936177781.body
}

// Additional Declarations
  //## begin ClLMGBVisitor%37CCEF680351.declarations preserve=yes
  //## end ClLMGBVisitor%37CCEF680351.declarations

//## begin module%3459D97B03DF.epilog preserve=yes
//## end module%3459D97B03DF.epilog

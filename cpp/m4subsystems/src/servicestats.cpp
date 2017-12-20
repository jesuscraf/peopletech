//## begin module%37FB0CA802D9.cm preserve=no
//## end module%37FB0CA802D9.cm

//## begin module%37FB0CA802D9.cp preserve=no
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
//## end module%37FB0CA802D9.cp

//## Module: servicestats%37FB0CA802D9; Package body
//## Subsystem: M4Subsystems::src%37D3A22003E4
//## Source file: F:\integration\m4subsystems\src\servicestats.cpp

//## begin module%37FB0CA802D9.additionalIncludes preserve=no
//## end module%37FB0CA802D9.additionalIncludes

//## begin module%37FB0CA802D9.includes preserve=yes
#include <clservicebasess.hpp>
#include <datastorages.hpp>
//## end module%37FB0CA802D9.includes

// Statistics
#include <statistics.hpp>
// LauncherInterface
#include <launcher.hpp>
// servicestats
#include <servicestats.hpp>

class ClServiceInterface;

//## begin module%37FB0CA802D9.declarations preserve=no
//## end module%37FB0CA802D9.declarations

//## begin module%37FB0CA802D9.additionalDeclarations preserve=yes
//## end module%37FB0CA802D9.additionalDeclarations


// Class ClServiceStats 















ClServiceStats::ClServiceStats (ClServiceBaseSS *ai_pServiceSS)
  //## begin ClServiceStats::ClServiceStats%889521621.hasinit preserve=no
      : m_uQueuedItems(0), m_fTPSFiveSec(0), m_fTPSThirtySec(0), m_fTPSFiveMin(0), m_fAvgExecFiveSec(0), m_fAvgExecThirtySec(0), m_fAvgExecFiveMin(0), m_fWaitTimeFiveSec(0), m_fWaitTimeThirtySec(0), m_fWaitTimeFiveMin(0), m_fSSUseFiveSec(0), m_fSSUseThirtySec(0), m_fSSUseFiveMin(0)
  //## end ClServiceStats::ClServiceStats%889521621.hasinit
  //## begin ClServiceStats::ClServiceStats%889521621.initialization preserve=yes
	, m_AvgQueuedItems (12 ,180)
  //## end ClServiceStats::ClServiceStats%889521621.initialization
{
  //## begin ClServiceStats::ClServiceStats%889521621.body preserve=yes
  //## end ClServiceStats::ClServiceStats%889521621.body
}


ClServiceStats::~ClServiceStats ()
{
  //## begin ClServiceStats::~ClServiceStats%889521622.body preserve=yes
  //## end ClServiceStats::~ClServiceStats%889521622.body
}



//## Other Operations (implementation)
m4void_t ClServiceStats::SetQueuedItems (ClStatNumericalType ai_uNewValue)
{
  //## begin ClServiceStats::SetQueuedItems%889521620.body preserve=yes
  //## end ClServiceStats::SetQueuedItems%889521620.body
}

m4return_t ClServiceStats::Reset ()
{
  //## begin ClServiceStats::Reset%889521626.body preserve=yes
	return M4_SUCCESS;
  //## end ClServiceStats::Reset%889521626.body
}

m4return_t ClServiceStats::ResetStat (ClStatHandle &ai_hStatId)
{
  //## begin ClServiceStats::ResetStat%889521627.body preserve=yes
	return M4_SUCCESS;
  //## end ClServiceStats::ResetStat%889521627.body
}

m4return_t ClServiceStats::GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_oValue)
{
  //## begin ClServiceStats::GetStat%889521628.body preserve=yes

	m4uint16_t				lowIndex;
	eServiceStatsId			statIndex;
	m4return_t				res = M4_SUCCESS ;
/*A ver como hago este chequeo porque la macro está definida en comsrv
	if (M4_ERROR == _CheckStatID(ai_hStatId, M4_SERVICE_SUBSYSTEM))
	{
		return M4_ERROR;
	}
	*/
	lowIndex = ai_hStatId.GetLowID();
	if (ClActiveClassStats::TOTAL_NUM  > lowIndex)
		return ClActiveClassStats::GetStat(ai_hStatId, ao_oValue);

	statIndex = (eServiceStatsId)lowIndex;

	if (0 < ao_oValue.GetSize())
		return M4_ERROR;

	ao_oValue.SetSize(SERVICE_DEF_STRING_SIZE);
	m4pchar_t	buffer = ao_oValue.GetBuffer();

    
	switch(statIndex)
	{
	case QUEUED_ITEMS:
		sprintf ( buffer , "%10d" , 5 ) ;

		break ;

	case FIV_SEC_QUEUED_ITEMS:
		sprintf ( buffer , "%10d" , m_AvgQueuedItems.GetAvg ( 1 ) ) ;

		break ;

	case THIR_SEC_QUEUED_ITEMS:
		sprintf ( buffer , "%10d" , m_AvgQueuedItems.GetAvg ( 6 ) ) ;

		break ;

	case FIV_MIN_QUEUED_ITEMS:
		sprintf ( buffer , "%10d" , m_AvgQueuedItems.GetAvg ( 60 ) ) ;

		break ;
	case FIV_SEC_TPS:
		sprintf ( buffer , "%10.2f" , m_fTPSFiveSec) ;

		break ;

	case THIR_SEC_TPS:
		sprintf ( buffer , "%10.2f" , m_fTPSThirtySec) ;

		break ;

	case FIV_MIN_TPS:
		sprintf ( buffer , "%10.2f" , m_fTPSFiveMin) ;

		break ;

	case FIV_SEC_AVG_EXEC:
		sprintf ( buffer , "%10.2f" , m_fAvgExecFiveSec) ;
		break ;

	case THIR_SEC_AVG_EXEC:
		sprintf ( buffer , "%10.2f" , m_fAvgExecThirtySec) ;
		break ;

	case FIV_MIN_AVG_EXEC:
		sprintf ( buffer , "%10.2f" , m_fAvgExecFiveMin) ;
		break;

	case FIV_SEC_SSUSE:
		sprintf ( buffer , "%6.2f" , m_fSSUseFiveSec) ;
		break ;

	case THIR_SEC_SSUSE:
		sprintf ( buffer , "%6.2f" , m_fSSUseThirtySec) ;
		break ;

	case FIV_MIN_SSUSE:
		sprintf ( buffer , "%6.2f" , m_fSSUseFiveMin) ;
		break ;

	case FIV_SEC_WAIT_TIME:
		sprintf ( buffer , "%10.2f" , m_fWaitTimeFiveSec) ;
		break ;

	case THIR_SEC_WAIT_TIME:
		sprintf ( buffer , "%10.2f" , m_fWaitTimeThirtySec) ;
		break ;

	case FIV_MIN_WAIT_TIME:
		sprintf ( buffer , "%10.2f" , m_fWaitTimeFiveMin) ;
		break ;

	default:
		ClDataUnit		aDataunit;
		if ( statIndex <20 )
			res = ClLauncherStats::GetStat(ai_hStatId, aDataunit);
		else
		{
			sprintf(buffer, "%010d", 0);
			res = M4_ERROR ;
		}
		if (M4_SUCCESS == res)
			strncpy (buffer, aDataunit.GetBuffer(), aDataunit.GetSize());
	}

	return res;
  //## end ClServiceStats::GetStat%889521628.body
}

ClStatNumericalType ClServiceStats::GetAvgQueuedItems ()
{
  //## begin ClServiceStats::GetAvgQueuedItems%937815750.body preserve=yes
	return (ClStatNumericalType)m_AvgQueuedItems.GetAvg ( )  ;
  //## end ClServiceStats::GetAvgQueuedItems%937815750.body
}

void ClServiceStats::AddnQueuedItems (ClStatNumericalType ai_lTransactions)
{
  //## begin ClServiceStats::AddnQueuedItems%937815752.body preserve=yes
	m_uQueuedItems = ai_lTransactions;
	ClStatAverageType	fTrasanctions = ai_lTransactions;
	m_AvgQueuedItems.Update(fTrasanctions);
  //## end ClServiceStats::AddnQueuedItems%937815752.body
}

// Additional Declarations
  //## begin ClServiceStats%35050300034E.declarations preserve=yes
  //## end ClServiceStats%35050300034E.declarations

// Class ClServiceVisitor 


//## Other Operations (implementation)
void ClServiceVisitor::VisitService (ClSSInterfaz &ai_oServiceSS, ClServiceInterface& ai_oService)
{
  //## begin ClServiceVisitor::VisitService%878029379.body preserve=yes
	ClSSInterfaz		*poSubsystem = NULL ;
	ClServiceStats		*pServiceStats ;
	ClStatAverageType	iAvgStat ;
	ClStatNumericalType	iProcessedItems = 0,
						iErrorRequest = 0,
						iTimedOutRequest = 0,
						iUserAbortedRequest =0,
						iAdminAbortedRequest =0,
						iSuccessRequest = 0;
	ClStatPercentType	fSSUseFiveSec = 0,
						fSSUseThirtySec = 0,
						fSSUseFiveMin = 0,
						fTPSFiveSec = 0,
						fTPSThirtySec = 0,
						fTPSFiveMin = 0,
						fAvgExecFiveSec = 0,
						fAvgExecThirtySec = 0,
						fAvgExecFiveMin = 0,
						fWaitTimeFiveSec = 0,
						fWaitTimeThirtySec = 0,
						fWaitTimeFiveMin =0;
	ClStatNumericalType iSwapValue ;
	m4uint16_t			iSubsystems = 0,
						iSubsystem = 0;
	vector <m4objid_t> vExecutivesSS ;
	m4char_t szId [20] ;


	if ( ! ( pServiceStats = (ClServiceStats *)ai_oService.GetStats() ) )
	{
		return  ;
	}

	sprintf ( szId , "%ld" , (m4uint32_t) ai_oServiceSS.GetId () ) ;

	if ( M4_SUCCESS == ai_oServiceSS.GetSubsystemsId ( szId , vExecutivesSS ) )
	{
		iSubsystems = vExecutivesSS.size() ;

		for ( iSubsystem = 0 ;iSubsystem < iSubsystems ; iSubsystem++ )
		{
			if ( poSubsystem = ai_oServiceSS.FindSubsystemById ( vExecutivesSS [iSubsystem] ) )
			{

				if ( 20 >= poSubsystem -> GetType () ) //si es un subsistema de ejecutor
				{

					iProcessedItems		+= _GetNumericalValue( *poSubsystem, ClLauncherStats::PROC_ITEMS_ID);
//	ERROR_REQUEST_COUNT
					iErrorRequest		+= _GetNumericalValue( *poSubsystem, ClLauncherStats::ERROR_REQUEST_COUNT);
//	TIMED_OUT_REQUEST_COUNT,
					iTimedOutRequest	+= _GetNumericalValue( *poSubsystem, ClLauncherStats::TIMED_OUT_REQUEST_COUNT);
//	USER_ABORTED_REQUEST_COUNT ,
					iUserAbortedRequest += _GetNumericalValue( *poSubsystem, ClLauncherStats::USER_ABORTED_REQUEST_COUNT);
//	ADMIN_ABORTED_REQUEST_COUNT ,
					iAdminAbortedRequest+= _GetNumericalValue( *poSubsystem, ClLauncherStats::ADMIN_ABORTED_REQUEST_COUNT);
//	SUCCESS_REQUEST_COUNT ,
					iSuccessRequest		+= _GetNumericalValue( *poSubsystem, ClLauncherStats::SUCCESS_REQUEST_COUNT);
//	FIV_SEC_TPS
					fTPSFiveSec			+= _GetPercentValue( *poSubsystem, ClLauncherStats::FIV_SEC_TPS);
//	THIR_SEC_TPS,
					fTPSThirtySec		+= _GetPercentValue( *poSubsystem, ClLauncherStats::THIR_SEC_TPS);
//	FIV_MIN_TPS,
					fTPSFiveMin			+= _GetPercentValue( *poSubsystem, ClLauncherStats::FIV_MIN_TPS);
//	FIV_SEC_AVG_EXEC,
					fAvgExecFiveSec		+= _GetPercentValue( *poSubsystem, ClLauncherStats::FIV_SEC_AVG_EXEC);
//	THIR_SEC_AVG_EXEC,
					fAvgExecThirtySec	+= _GetPercentValue( *poSubsystem, ClLauncherStats::THIR_SEC_AVG_EXEC);
//	FIV_MIN_AVG_EXEC,
					fAvgExecFiveMin		+= _GetPercentValue( *poSubsystem, ClLauncherStats::FIV_MIN_AVG_EXEC);
//	FIV_SEC_SSUSE,
					fSSUseFiveSec		+= _GetPercentValue( *poSubsystem, ClLauncherStats::FIV_SEC_SSUSE);
//	THIR_SEC_SSUSE,
					fSSUseThirtySec		+= _GetPercentValue( *poSubsystem, ClLauncherStats::THIR_SEC_SSUSE);
//	FIV_MIN_SSUSE,
					fSSUseFiveMin		+= _GetPercentValue( *poSubsystem, ClLauncherStats::FIV_MIN_SSUSE);
//	FIV_SEC_WAIT_TIME,
					fWaitTimeFiveSec	+= _GetPercentValue( *poSubsystem, ClLauncherStats::FIV_SEC_WAIT_TIME);
//	THIR_SEC_WAIT_TIME,
					fWaitTimeThirtySec	+= _GetPercentValue( *poSubsystem, ClLauncherStats::THIR_SEC_WAIT_TIME);
//	FIV_MIN_WAIT_TIME,
					fWaitTimeFiveMin	+= _GetPercentValue( *poSubsystem, ClLauncherStats::FIV_MIN_WAIT_TIME);
				}
			}
		}
	}

	m4uint_t	iQueueSize = ai_oService.GetServiceQ()->Size();
	pServiceStats -> AddnQueuedItems(iQueueSize);
	iSwapValue = pServiceStats -> GetnProcessedItems () ;
	pServiceStats -> AddnProcessedItems  ( iProcessedItems - iSwapValue ) ;
	iSwapValue = pServiceStats -> GetnUserAbortedRequest () ;
	pServiceStats -> AddnUserAbortedRequest  ( iUserAbortedRequest  ) ;
	iSwapValue = pServiceStats -> GetnErrorRequest () ;
	pServiceStats -> AddnErrorRequest  ( iErrorRequest - iSwapValue ) ;
	iSwapValue = pServiceStats -> GetnTimedOutRequest () ;
	pServiceStats -> AddnTimedOutRequest  ( iTimedOutRequest - iSwapValue ) ;
	iSwapValue = pServiceStats -> GetnAdminAbortedRequest () ;
	pServiceStats -> AddnAdminAbortedRequest  ( iAdminAbortedRequest - iSwapValue ) ;
	iSwapValue = pServiceStats -> GetnSuccessRequest () ;
	pServiceStats -> AddnSuccessRequest  ( iSuccessRequest - iSwapValue ) ;
	
	pServiceStats->m_fTPSFiveSec = fTPSFiveSec / iSubsystems;
	pServiceStats->m_fTPSThirtySec = fTPSThirtySec / iSubsystems;
	pServiceStats->m_fTPSFiveMin = fTPSFiveMin / iSubsystems;
	pServiceStats->m_fAvgExecFiveSec = fAvgExecFiveSec / iSubsystems;
	pServiceStats->m_fAvgExecThirtySec = fAvgExecThirtySec / iSubsystems;
	pServiceStats->m_fAvgExecFiveMin = fAvgExecFiveMin / iSubsystems;
	pServiceStats->m_fWaitTimeFiveSec = fWaitTimeFiveSec / iSubsystems;
	pServiceStats->m_fWaitTimeThirtySec = fWaitTimeThirtySec / iSubsystems;
	pServiceStats->m_fWaitTimeFiveMin = fWaitTimeFiveMin / iSubsystems;
	pServiceStats->m_fSSUseFiveSec = fSSUseFiveSec / iSubsystems;
	pServiceStats->m_fSSUseThirtySec = fSSUseThirtySec / iSubsystems;
	pServiceStats->m_fSSUseFiveMin = fSSUseFiveMin / iSubsystems;

	iAvgStat = (ClStatAverageType)(iProcessedItems - iSwapValue ) / 5 ;

	pServiceStats -> AddAvgProcessedItems ( iAvgStat  );
  //## end ClServiceVisitor::VisitService%878029379.body
}

ClStatNumericalType ClServiceVisitor::_GetNumericalValue (ClSSInterfaz &ai_oSubsystem, m4uint_t ai_iStatId)
{
  //## begin ClServiceVisitor::_GetNumericalValue%943003259.body preserve=yes
	ClStatNumericalType	iValue = 0;
	m4char_t szId [10] ;

	sprintf ( szId , "%d" , (m4uint32_t) ai_oSubsystem.GetId () ) ;
	M4DataStorage	*poDataStorage = M4DataStorage::GetNewDataStorage () ;
	if ( M4_SUCCESS == ai_oSubsystem.GetStatisticAttribValue ( szId , ai_iStatId , poDataStorage  ) )
	{
		ClDataUnitInterface	*pDataUnit;
		poDataStorage -> GetNext (pDataUnit);
		if( pDataUnit )
		{
			iValue = atol (pDataUnit -> GetBuffer ()) ;
		}
	}
	delete poDataStorage ;

	return iValue;
  //## end ClServiceVisitor::_GetNumericalValue%943003259.body
}

ClStatPercentType ClServiceVisitor::_GetPercentValue (ClSSInterfaz &ai_oSubsystem, m4uint_t ai_iStatId)
{
  //## begin ClServiceVisitor::_GetPercentValue%943003260.body preserve=yes
	ClStatPercentType	fValue;
	m4char_t szId [10] ;

	sprintf ( szId , "%d" , (m4uint32_t) ai_oSubsystem.GetId () ) ;

	M4DataStorage	*poDataStorage = M4DataStorage::GetNewDataStorage () ;
	if ( M4_SUCCESS == ai_oSubsystem.GetStatisticAttribValue ( szId , ai_iStatId , poDataStorage  ) )
	{
		ClDataUnitInterface	*pDataUnit;
		poDataStorage -> GetNext (pDataUnit);
		if( pDataUnit )
		{
			fValue = atof (pDataUnit -> GetBuffer ()) ;
		}
	}
	delete poDataStorage ;

	return fValue;
  //## end ClServiceVisitor::_GetPercentValue%943003260.body
}

//## begin module%37FB0CA802D9.epilog preserve=yes
//## end module%37FB0CA802D9.epilog

//## begin module%34BCE23D015D.cm preserve=no
//## end module%34BCE23D015D.cm

//## begin module%34BCE23D015D.cp preserve=no
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
//## end module%34BCE23D015D.cp

//## Module: ssvisitors%34BCE23D015D; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: F:\integration\m4comsrv\src\ssvisitors.cpp

//## begin module%34BCE23D015D.additionalIncludes preserve=no
//## end module%34BCE23D015D.additionalIncludes

//## begin module%34BCE23D015D.includes preserve=yes
#include <request.hpp>
#include <csutil.hpp>
#include <ssstats.hpp>
#include <subsystems.hpp>
#include <ssccsubsystems.hpp>
//## end module%34BCE23D015D.includes

// ssvisitors
#include <ssvisitors.hpp>
//## begin module%34BCE23D015D.declarations preserve=no
//## end module%34BCE23D015D.declarations

//## begin module%34BCE23D015D.additionalDeclarations preserve=yes
//## end module%34BCE23D015D.additionalDeclarations


//## begin module%34BCE23D015D.epilog preserve=yes
//## end module%34BCE23D015D.epilog


// Detached code regions:
// WARNING: this code will be lost if code is regenerated.
#if 0
//## begin ClSessManagerListVisitor::VisitSessManagerList%885288583.body preserve=no
	ClSessManagerStats * poStats ;
	ClSessManagerListStats * poListStats;
	list <ClActiveClassStats * >::iterator Iterator ;
	ClStatAverageType iAvgStat ;
	ClStatNumericalType iIOOperations = 0, 
		iPageFaults = 0,
		iRunPriority = 0,
		iDeniedConnections =0 ,
		iForgedSignatures = 0 ,
		iIncorrectPDUs = 0 ,
		iIncorrectProtocolVersion = 0,
		iInvalidTimeMasks = 0 ,
		iInvalidUserAuthentifications = 0 ,
		iTimeOuts = 0 ,
		iUnknownHosts = 0 ,
		iUnknownUsers = 0 ,
		iProcessedItems = 0;
	ClStatNumericalType iSwapIOOperations = 0, 
		iSwapPageFaults = 0,
		iSwapRunPriority = 0,
		iSwapValue,
		iSwapDeniedConnections =0 ,
		iSwapForgedSignatures = 0 ,
		iSwapIncorrectPDUs = 0 ,
		iSwapIncorrectProtocolVersion = 0,
		iSwapInvalidTimeMasks = 0 ,
		iSwapInvalidUserAuthentifications = 0 ,
		iSwapTimeOuts = 0 ,
		iSwapUnknownHosts = 0 ,
		iSwapUnknownUsers = 0 ;
	m4uint16_t iSubsystems = 0;


	poListStats = (ClSessManagerListStats * ) ai_oSessManagerList.GetStats ( ) ;
	

	if ( ! poListStats )
	{
		return ;
	}

	iSwapValue = poListStats -> GetnProcessedItems () ;
	
	iSwapDeniedConnections = poListStats -> GetnDeniedConnections ( ) ;
	iSwapForgedSignatures = poListStats -> GetnForgedSignatures ( ) ;
	iSwapIncorrectPDUs = poListStats -> GetnIncorrectPDUs () ;
	iSwapIncorrectProtocolVersion = poListStats -> GetnInvalidProtocolVersions ( ) ;
	iSwapInvalidTimeMasks = poListStats -> GetnInvalidTimeMarks  ( ) ;
	iSwapInvalidUserAuthentifications = poListStats -> GetnInvalidUserAuthentications  ( ); 
	iSwapTimeOuts = poListStats -> GetnTimeouts ( ) ; 
	iSwapUnknownHosts = poListStats -> GetnUnknownHosts ( ) ;
	iSwapUnknownUsers = poListStats -> GetnUnknownUsers ( ) ;


	for ( Iterator = ( ( ClSessManagerListStats * )ai_oSessManagerList.GetStats ( ) ) -> m_oStatsList.begin ( ) ;
	Iterator != ( ( ClSessManagerListStats * )ai_oSessManagerList.GetStats ( ) ) -> m_oStatsList.end ( ) ;
	Iterator ++ )
	{
		iSubsystems++;

		poStats = (ClSessManagerStats * ) (* Iterator) ;

		if (  poStats )
		{
			iProcessedItems += poStats -> GetnProcessedItems () ;

			iDeniedConnections += poStats -> GetnDeniedConnections ( ) ;
		
			iForgedSignatures += poStats -> GetnForgedSignatures ( ) ;
		
			iIncorrectPDUs += poStats -> GetnIncorrectPDUs () ;
		
			iIncorrectProtocolVersion += poStats -> GetnInvalidProtocolVersions ( ) ;
		
			iInvalidTimeMasks += poStats -> GetnInvalidTimeMarks  ( ) ;
		
			iInvalidUserAuthentifications += poStats -> GetnInvalidUserAuthentications  ( ); 
		
			iTimeOuts += poStats -> GetnTimeouts ( ) ; 

			iUnknownHosts += poStats -> GetnUnknownHosts ( ) ;

			iUnknownUsers += poStats -> GetnUnknownUsers ( ) ;

		}
	}

	iAvgStat = (ClStatAverageType)(iProcessedItems - iSwapValue ) / 5 ;

	poListStats -> AddAvgProcessedItems ( iAvgStat  );

	poListStats -> AddnProcessedItems ( iProcessedItems - iSwapValue);

	poListStats -> AddnDeniedConnections ( iDeniedConnections - iSwapDeniedConnections) ;
		
	poListStats -> AddnForgedSignatures ( iForgedSignatures - iSwapForgedSignatures ) ;
		
	poListStats -> AddnIncorrectPDUs (iIncorrectPDUs - iSwapIncorrectPDUs ) ;
		
	poListStats -> AddnInvalidProtocolVersions ( iIncorrectProtocolVersion - iSwapIncorrectProtocolVersion ) ;
		
	poListStats -> AddnInvalidTimeMarks  ( iInvalidTimeMasks - iSwapInvalidTimeMasks ) ;
		
	poListStats -> AddnInvalidUserAuthentications  ( iInvalidUserAuthentifications - iSwapInvalidUserAuthentifications ); 
		
	poListStats -> AddnTimeouts ( iTimeOuts - iSwapTimeOuts ) ; 

	poListStats -> AddnUnknownHosts ( iUnknownHosts - iSwapUnknownHosts ) ;

	poListStats -> AddnUnknownUsers ( iUnknownUsers - iSwapUnknownUsers ) ;


//## end ClSessManagerListVisitor::VisitSessManagerList%885288583.body

#endif

//## begin module%37D7C64A0307.cm preserve=no
//## end module%37D7C64A0307.cm

//## begin module%37D7C64A0307.cp preserve=no
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
//## end module%37D7C64A0307.cp

//## Module: JSServiceCancel%37D7C64A0307; Package body
//## Subsystem: M4JSSubsystem::src%37BD2F0A02AE
//## Source file: D:\eduardoma\m4jssubsystem\src\jsservicecancel.cpp

//## begin module%37D7C64A0307.additionalIncludes preserve=no
//## end module%37D7C64A0307.additionalIncludes

//## begin module%37D7C64A0307.includes preserve=yes
#include <request.hpp>
#include <subsystemids.hpp>
#include <clserviceinfo.hpp>
#include <cloblconfiguration.hpp>
#include <clssinterfaz.hpp>
#include <ssnames.hpp>
#include <jsserviceexecute.hpp>
#include <configtree.hpp>
//## end module%37D7C64A0307.includes

// JSSCancel
#include <jsscancel.hpp>
// JSServiceCancel
#include <jsservicecancel.hpp>
// clservicebasess
#include <clservicebasess.hpp>
//## begin module%37D7C64A0307.declarations preserve=no
//## end module%37D7C64A0307.declarations

//## begin module%37D7C64A0307.additionalDeclarations preserve=yes
//## end module%37D7C64A0307.additionalDeclarations


// Class ClJSServiceCancel 




ClJSServiceCancel::ClJSServiceCancel (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClJSServiceCancel::ClJSServiceCancel%936888037.hasinit preserve=no
  //## end ClJSServiceCancel::ClJSServiceCancel%936888037.hasinit
  //## begin ClJSServiceCancel::ClJSServiceCancel%936888037.initialization preserve=yes
  :ClServiceBaseSS(ai_poConfiguration,ai_poTables,ai_poNavigator)
  //## end ClJSServiceCancel::ClJSServiceCancel%936888037.initialization
{
  //## begin ClJSServiceCancel::ClJSServiceCancel%936888037.body preserve=yes
	m_pcDumpFile = NULL;
	m_oServiceName = "";
	m_oServiceName = "";
  //## end ClJSServiceCancel::ClJSServiceCancel%936888037.body
}



//## Other Operations (implementation)
m4return_t ClJSServiceCancel::Initialize ()
{
  //## begin ClJSServiceCancel::Initialize%936888038.body preserve=yes
	m4return_t ret;
	m4char_t szServiceId [OBL_PROPVALUE_MAX_LEN] ;
	m4uint32_t lServiceId;
	size_t iLen ;

//	m_poStatistics = new ClStatsJSServiceExe;
//	m_poVisitor = new ClServiceVisitor ;
//	poLocalMonitor = (ClLocalMonitorInterface*)m_poNavigator -> GetLocalMonitor () ;

	/////////////////
	//SERVICE_ID
	/////////////////
	ret = GetConfigStringValue ( SERVICE_ID , szServiceId , OBL_PROPVALUE_MAX_LEN , iLen ) ;

	if ( ret == M4_ERROR ) {
		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "None Service Id especified for executor, property %s missed", SERVICE_ID ) ;
		lServiceId = 1 ;
	}
	lServiceId = m_poTables -> GetId ( szServiceId ) ;

	if ( lServiceId == M4_ERROR ) {
		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "Physical Service Id : %s not found in physical services table", szServiceId ) ;
		return M4_ERROR;
	}

	m_uMaxInstances = 5;
	m_uMinActiveInstances = 1;

	SetConfigNumericProperty ( "MAX_INSTANCES", 5);
	SetConfigNumericProperty ( "MIN_INSTANCES", 1);
	SetConfigNumericProperty ( "EXECUTOR_COUNT", 0);
	SetConfigNumericProperty ( "EXITED_EXECUTORS", 0);

	//////////////
	//Generacion del ejecutor
	//////////////

	//BEGIN EXECUTOR BLOCK
	M4ClString strConfigCancel = "\
TYPE=\"EXECUTOR_JOB_CANCEL\";\
STRINGID=\"JS Cancel Executor\";\
DATA_CACHE=\"DataCache\"";

	ClConfiguration * poConfiguration = new ClM4ConfigTree(strConfigCancel);
	ClSSInterfaz *newSSCancel = GenerateSubsystem(M4_EXECUTOR_JOB_CANCEL_SUBSYSTEM, poConfiguration, m_poTables);
	if (newSSCancel==NULL)
		return M4_ERROR;
	if (AddSubsystem(newSSCancel)==M4_ERROR)
		return M4_ERROR;
	if (newSSCancel -> Initialize() == M4_ERROR)
		return M4_ERROR;
	return newSSCancel -> Init(poConfiguration);

  //## end ClJSServiceCancel::Initialize%936888038.body
}

m4return_t ClJSServiceCancel::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClJSServiceCancel::ExecuteCommand%936888040.body preserve=yes
	return ClServiceBaseSS::ExecuteCommand (ai_cCommand, ai_oParamList, ao_oResultList);
  //## end ClJSServiceCancel::ExecuteCommand%936888040.body
}

m4return_t ClJSServiceCancel::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
{
  //## begin ClJSServiceCancel::GetCommandParams%936888041.body preserve=yes
	return ClServiceBaseSS::GetCommandParams (ai_cCommand, aio_oParamList);
  //## end ClJSServiceCancel::GetCommandParams%936888041.body
}

m4return_t ClJSServiceCancel::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClJSServiceCancel::GetCommandsId%936888042.body preserve=yes
	return ClServiceBaseSS::GetCommandsId (aio_oCommandList);
  //## end ClJSServiceCancel::GetCommandsId%936888042.body
}

ClSSInterfaz * ClJSServiceCancel::GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables)
{
  //## begin ClJSServiceCancel::GenerateSubsystem%936888043.body preserve=yes
	ClSSInterfaz *poInterfaz = NULL;
	ClJSSCancel *poSSCancel;

	switch (ai_lType) {
		case M4_EXECUTOR_JOB_CANCEL_SUBSYSTEM:
			poSSCancel = new ClJSSCancel(ai_poConfiguration, ai_poTables, m_poNavigator);
			if (poSSCancel != NULL) {
				poInterfaz = new ClSSInterfaz ( poSSCancel  );
				if (poInterfaz != NULL) {
					poSSCancel -> SetInterfaz ( poInterfaz ) ;
					poSSCancel -> SetOutputQ( (ClOutputQ*) m_poOutputQ);
					poSSCancel -> SetServiceQ( (ClServiceQ*) m_poServiceQ);
					poSSCancel -> SetDumpFile(m_pcDumpFile);
					poSSCancel -> SetUseExceptions(GetUseExceptions());
					poSSCancel -> SetIdentification(m_oServerName, m_oServiceName);
					//Las estadisticas del ejecutor se muestran
					//en el subsistema de JS
					poSSCancel -> SetStats(m_poStatistics);
				}
				else
					delete poSSCancel;
			}
			break;
		default:
			poInterfaz = NULL;
			break;
	};
	return poInterfaz;
  //## end ClJSServiceCancel::GenerateSubsystem%936888043.body
}

m4return_t ClJSServiceCancel::Stop (m4uint32_t ai_lDeferedTime)
{
  //## begin ClJSServiceCancel::Stop%937812720.body preserve=yes
	return ClServiceBaseSS::Stop (ai_lDeferedTime);
  //## end ClJSServiceCancel::Stop%937812720.body
}

m4return_t ClJSServiceCancel::SetDumpFile (m4pchar_t ai_pDumpFile)
{
  //## begin ClJSServiceCancel::SetDumpFile%940237694.body preserve=yes
	m_pcDumpFile = ai_pDumpFile;
	return M4_SUCCESS;
  //## end ClJSServiceCancel::SetDumpFile%940237694.body
}

m4return_t ClJSServiceCancel::SetIdentification (M4ClString& ai_ServerName, M4ClString& ai_ServiceName)
{
  //## begin ClJSServiceCancel::SetIdentification%940337548.body preserve=yes
	m_oServerName = ai_ServerName;
	m_oServiceName = ai_ServiceName;
	return M4_SUCCESS;
  //## end ClJSServiceCancel::SetIdentification%940337548.body
}

// Additional Declarations
  //## begin ClJSServiceCancel%37D7C4E90092.declarations preserve=yes
  //## end ClJSServiceCancel%37D7C4E90092.declarations

//## begin module%37D7C64A0307.epilog preserve=yes
//## end module%37D7C64A0307.epilog

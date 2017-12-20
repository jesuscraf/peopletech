//## begin module%39105E6D0278.cm preserve=no
//## end module%39105E6D0278.cm

//## begin module%39105E6D0278.cp preserve=no
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
//## end module%39105E6D0278.cp

//## Module: ExecutiveForOESS%39105E6D0278; Subprogram body
//## Subsystem: M4ExeForChannel::src%379C5C480311
//## Source file: F:\integration\m4exeforchannel\src\executiveforoess.cpp

//## begin module%39105E6D0278.additionalIncludes preserve=no
//## end module%39105E6D0278.additionalIncludes

//## begin module%39105E6D0278.includes preserve=yes
#include <ssnames.hpp>
#include <exvariables.hpp>
#include <m4srvtrace.h>
//## end module%39105E6D0278.includes

// ClAdminTag
#include <cladmintag.hpp>
// ExecutiveForOESS
#include <executiveforoess.hpp>
// clexecutivebasess
#include <clexecutivebasess.hpp>
//## begin module%39105E6D0278.declarations preserve=no
//## end module%39105E6D0278.declarations

//## begin module%39105E6D0278.additionalDeclarations preserve=yes
#define M4_SRV_TRACE_COMP_ID	"EXECHANNEL"
//## end module%39105E6D0278.additionalDeclarations


// Class ClExecutiveForOESS 

ClExecutiveForOESS::ClExecutiveForOESS (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator, ClServiceBaseSS *ai_poServiceSS)
  //## begin ClExecutiveForOESS::ClExecutiveForOESS%957346242.hasinit preserve=no
  //## end ClExecutiveForOESS::ClExecutiveForOESS%957346242.hasinit
  //## begin ClExecutiveForOESS::ClExecutiveForOESS%957346242.initialization preserve=yes
  :ClExecutiveBaseSS (ai_poConfiguration, ai_poTables, ai_poNavigator , ai_poServiceSS)
  //## end ClExecutiveForOESS::ClExecutiveForOESS%957346242.initialization
{
  //## begin ClExecutiveForOESS::ClExecutiveForOESS%957346242.body preserve=yes
  //## end ClExecutiveForOESS::ClExecutiveForOESS%957346242.body
}



//## Other Operations (implementation)
m4return_t ClExecutiveForOESS::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClExecutiveForOESS::ExecuteCommand%957346245.body preserve=yes
	eStatus eState ;
	ClAdminCommandTag *newParam;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;


	if (ai_cCommand < ClExecutiveBaseSS::TOTAL_COMMANDS)
		return ClExecutiveBaseSS::ExecuteCommand (ai_cCommand, ai_oParamList, ao_oResultList);
	
	m4return_t iResult = M4_ERROR;
	
	switch (ai_cCommand)
	{
	case QUIT:
		if (m_poLauncher)
		{
			iResult = m_poLauncher->ExecuteCommand(ClLauncherInterface::QUIT, ai_oParamList, ao_oResultList);
			if (iResult == M4_SUCCESS)
				SetState(SS_ACTIVE, SS_STR_ACTIVE);
		}
		break ;

	case PAUSE:
		if (m_poLauncher)
		{
			iResult = m_poLauncher->ExecuteCommand(ClLauncherInterface::PAUSE, ai_oParamList, ao_oResultList);
			if (iResult == M4_SUCCESS)
				SetState(SS_PAUSED, SS_STR_PAUSED);
		}
		break;

	case RESUME:
		if (m_poLauncher)
		{
			iResult = m_poLauncher->ExecuteCommand(ClLauncherInterface::RESUME, ai_oParamList, ao_oResultList);
			if (iResult == M4_SUCCESS)
				SetState(SS_ACTIVE, SS_STR_ACTIVE);
		}
		break ;
		
	case GET_USED_MEMORY :
		if (m_poLauncher)
		{
			iResult = m_poLauncher->ExecuteCommand(ClLauncherInterface::USED_MEMORY, ai_oParamList, ao_oResultList);

			if ( iResult != M4_SUCCESS )
			{
				newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "USED_MEMORY", adminLong, "0", M4_TRUE);
				M4_ASSERT(newParam != NULL);
				if (newParam == NULL) 
					return M4_ERROR;
				ao_oResultList.push_back(newParam);
			}

			return M4_SUCCESS;
		}
		break ;

	default:
		M4_SRV_ERROR ( M4_SRV_COMMAND_ID_ERROR, "Error Executing command in '%0:s' Subsystem. Unknown command id: %1:d" , GetName() << ai_cCommand ) ;
//		SETCODEF ( M4_SRV_SS_COMMAND_PARAM_ERROR , ERRORLOG , "Executive Subsystem: Invalid Administrator List SubSystem command Id: %d", ai_cCommand );
		return M4_ERROR;
	}
	
	return iResult;
  //## end ClExecutiveForOESS::ExecuteCommand%957346245.body
}

m4return_t ClExecutiveForOESS::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
{
  //## begin ClExecutiveForOESS::GetCommandParams%957346246.body preserve=yes

	// checks if the command is a base class command.
	eStatus eState ;

	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	if(ai_cCommand < ClExecutiveBaseSS::TOTAL_COMMANDS)
		return ClExecutiveBaseSS::GetCommandParams (ai_cCommand, aio_oParamList);
	
	// builds the parameters list.
	switch (ai_cCommand)
	{
	case QUIT:
		break;

	case GET_USED_MEMORY :
		ClAdminCommandTag *newParam;
		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "USED_MEMORY", adminLong, "", M4_TRUE);
		M4_ASSERT(newParam != NULL);
		if (newParam == NULL) 
			return M4_ERROR;
		aio_oParamList.push_back(newParam);

		break ;
	default:
		return M4_ERROR;
	}

	return M4_SUCCESS;
  //## end ClExecutiveForOESS::GetCommandParams%957346246.body
}

m4return_t ClExecutiveForOESS::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClExecutiveForOESS::GetCommandsId%957346247.body preserve=yes
	// calls the base class to get its commands
	eStatus eState;
	m4int16_t iState ;

	iState = m_poLauncher -> GetStatus () ;

	GetState(eState);
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR;

	if ( M4_ERROR == ClExecutiveBaseSS::GetCommandsId ( aio_oCommandList ) )
	{
		return M4_ERROR ;
	}
	// inserts commands for this subsystem
	ClAdminTag *newCommand;

	// Determinamos los comandos disponibles dependiendo del estado del SS
	if (iState != ClStatus::STATUS_INTR_QUIT )
	{
		newCommand = ClAdminTag::GetNewAdminTag(QUIT, "EXECUTIVE_SS_QUIT", M4_FALSE);
		M4_ASSERT(newCommand != NULL);
		if (newCommand == NULL) 
			return M4_ERROR;
		aio_oCommandList.push_back(newCommand);

		switch (iState)
		{
		case ClStatus::STATUS_INTR_NORMAL:
			newCommand = ClAdminTag::GetNewAdminTag(PAUSE, "PAUSE_EXECUTION", M4_FALSE);
			M4_ASSERT(newCommand != NULL);
			if (newCommand == NULL) 
				return M4_ERROR;
			aio_oCommandList.push_back(newCommand);
			
			break;
			
		case ClStatus::STATUS_INTR_PAUSING:
		case ClStatus::STATUS_INTR_PAUSED:
			newCommand = ClAdminTag::GetNewAdminTag(RESUME, "RESUME_EXECUTION", M4_FALSE);
			M4_ASSERT(newCommand != NULL);
			if (newCommand == NULL) 
				return M4_ERROR;
			aio_oCommandList.push_back(newCommand);
			
			break;
		}
	}

	newCommand = ClAdminTag::GetNewAdminTag(GET_USED_MEMORY , "GET_USED_MEMORY", M4_TRUE);
	M4_ASSERT(newCommand != NULL);
	if (newCommand == NULL) 
		return M4_ERROR;
	aio_oCommandList.push_back(newCommand);
	
	return  M4_SUCCESS;
  //## end ClExecutiveForOESS::GetCommandsId%957346247.body
}

// Additional Declarations
  //## begin ClExecutiveForOESS%39105AD200C2.declarations preserve=yes
  //## end ClExecutiveForOESS%39105AD200C2.declarations

//## begin module%39105E6D0278.epilog preserve=yes
//## end module%39105E6D0278.epilog

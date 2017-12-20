//## begin module%36E651F00369.cm preserve=no
//## end module%36E651F00369.cm

//## begin module%36E651F00369.cp preserve=no
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
//## end module%36E651F00369.cp

//## Module: LauncherForCancelExecutor28%36E651F00369; Package body
//## Subsystem: M4JSRecover::src%37F8B6A102CB
//## Source file: D:\FuentesServidor\m4jsrecover\src\launcherccexe28.cpp

//## begin module%36E651F00369.additionalIncludes preserve=no
//## end module%36E651F00369.additionalIncludes

//## begin module%36E651F00369.includes preserve=yes
#include <ccexecutor.hpp> //Forward en hpp
#include <jsrequest.hpp> //Forward en hpp
#include <m4exception.hpp> //only en cpp
#include <jsjob.hpp> //forward en hpp
#include <m4jsres.hpp>
//## end module%36E651F00369.includes

// LauncherInterface
#include <launcher.hpp>
// Executive
#include <executive.hpp>
// LauncherForCancelExecutor28
#include <launcherccexe28.hpp>


//## begin module%36E651F00369.declarations preserve=no
//## end module%36E651F00369.declarations

//## begin module%36E651F00369.additionalDeclarations preserve=yes
static void M4JsrecoverThrow( M4RuntimeException ai_oException )
{
	throw( ai_oException ) ;
}
//## end module%36E651F00369.additionalDeclarations


// Class ClLauncherForCancelExecutor28 

ClLauncherForCancelExecutor28::ClLauncherForCancelExecutor28 (ClExecutive *ai_pExecutive, ClStatistic *ai_poStats, ClLog *ai_poLog, ClServiceQ *ai_poServiceQ, ClOutputQ *ai_poOutputQ)
  //## begin ClLauncherForCancelExecutor28::ClLauncherForCancelExecutor28%921053038.hasinit preserve=no
  //## end ClLauncherForCancelExecutor28::ClLauncherForCancelExecutor28%921053038.hasinit
  //## begin ClLauncherForCancelExecutor28::ClLauncherForCancelExecutor28%921053038.initialization preserve=yes
  :ClLauncherInterface(ai_pExecutive, ai_poStats, ai_poServiceQ) , m_pOutputQ ( ai_poOutputQ )
  //## end ClLauncherForCancelExecutor28::ClLauncherForCancelExecutor28%921053038.initialization
{
  //## begin ClLauncherForCancelExecutor28::ClLauncherForCancelExecutor28%921053038.body preserve=yes
  //## end ClLauncherForCancelExecutor28::ClLauncherForCancelExecutor28%921053038.body
}



//## Other Operations (implementation)
m4return_t ClLauncherForCancelExecutor28::Destroy ()
{
  //## begin ClLauncherForCancelExecutor28::Destroy%921053041.body preserve=yes
	//Creamos una request para informar de la finalización
	//de un ejecutor
	ClJSRequest *poJSRequestOut=new ClJSRequest(M4JS_REQ_EXECUTOR_DELETED);

	//Añadimos como parámetro de la request el ejecutor
	poJSRequestOut->AddParameter(m_pExecutive);
	poJSRequestOut->AddParameter((m4uint32_t)1);

	//Encolamos la request en la cola de salida del ejecutor
	((ClJSQueue *)m_pOutputQ)->Push(poJSRequestOut);

	return M4_SUCCESS;
  //## end ClLauncherForCancelExecutor28::Destroy%921053041.body
}

m4return_t ClLauncherForCancelExecutor28::Initialize (ClNavigatorFirewall *ai_pNavigator, ClConfiguration *ai_pConfig)
{
  //## begin ClLauncherForCancelExecutor28::Initialize%941102054.body preserve=yes
	
	//Llamamos a la inicialización del launcher 
	m4return_t ret=ClLauncherInterface::Initialize (ai_pNavigator, ai_pConfig);

	if (M4_SUCCESS != ret)
		return M4_ERROR;

	//Informamos al administrador que se ha creado
	//un nuevo ejecutor con éxito
	ClJSRequest *poJSRequestOut=new ClJSRequest(M4JS_REQ_EXECUTOR_CREATED);

	//Almacenamos en la request el ejecutor que se 
	//ha creado
	poJSRequestOut->AddParameter(m_pExecutive);
	poJSRequestOut->AddParameter((m4uint32_t)1);

	//Almacenamos en la cola la request
	((ClJSQueue *)m_pOutputQ)->Push(poJSRequestOut);

	return M4_SUCCESS;
  //## end ClLauncherForCancelExecutor28::Initialize%941102054.body
}

m4return_t ClLauncherForCancelExecutor28::_ExecuteRequest (ClBaseRequest *ai_poRequest)
{
  //## begin ClLauncherForCancelExecutor28::_ExecuteRequest%943433795.body preserve=yes
	m4return_t ret = M4_SUCCESS;
	m4uint32_t uiTypeRequest;
	ClJSRequest *pJSOutputRequest;
	ClJSRequest *pJSInputRequest = (ClJSRequest*)ai_poRequest;
	uiTypeRequest = pJSInputRequest->GetRequestType();
	ClJSQueue *pOutputQ = (ClJSQueue*) m_pOutputQ;
	switch (uiTypeRequest) {
		case M4JS_REQ_SHUTDOWN:
			pJSOutputRequest = new ClJSRequest(M4JS_REQ_SHUTDOWN_OK);
			pOutputQ->Push(pJSOutputRequest);
			Terminate();
			break;
		default:
			ret = ((ClCcExecutor*)m_pExecutive) -> ProccessRequest (pJSInputRequest, *pOutputQ);
			break;
	};
	return ret;
  //## end ClLauncherForCancelExecutor28::_ExecuteRequest%943433795.body
}

m4return_t ClLauncherForCancelExecutor28::_PrepareExecution (ClBaseRequest *ai_poRequest)
{
  //## begin ClLauncherForCancelExecutor28::_PrepareExecution%943433796.body preserve=yes
	return M4_SUCCESS;
  //## end ClLauncherForCancelExecutor28::_PrepareExecution%943433796.body
}

m4return_t ClLauncherForCancelExecutor28::_ReplyRequest (ClBaseRequest *ai_poRequest)
{
  //## begin ClLauncherForCancelExecutor28::_ReplyRequest%943433797.body preserve=yes
	//Borramos la request
	if (ai_poRequest!=NULL)
		delete ai_poRequest;
	return M4_SUCCESS;
  //## end ClLauncherForCancelExecutor28::_ReplyRequest%943433797.body
}

m4return_t ClLauncherForCancelExecutor28::_GetNextRequest (ClBaseRequest *&ao_poRequest)
{
  //## begin ClLauncherForCancelExecutor28::_GetNextRequest%943433798.body preserve=yes
	m4return_t iRet;
	ClJSRequest *poJSRequest = (ClJSRequest *)ao_poRequest;
	iRet = ((ClJSQueue *)m_pServiceQ)->Pop(poJSRequest);
	ao_poRequest = poJSRequest;
	return iRet;
  //## end ClLauncherForCancelExecutor28::_GetNextRequest%943433798.body
}

m4return_t ClLauncherForCancelExecutor28::_CleanUp (ClBaseRequest *ai_poRequest)
{
  //## begin ClLauncherForCancelExecutor28::_CleanUp%943433799.body preserve=yes
	ClJSRequest *poJSRequestOut;

 	//Creamos una request de JS que informe que se ha producido 
	//un GP en el ejecutor al administrador
	poJSRequestOut=new ClJSRequest(M4JS_REQ_GP);

	//Almacenamos en los parámetros de la request el puntero al
	//subsistema
	poJSRequestOut->AddParameter(m_pExecutive);
	poJSRequestOut->AddParameter((m4uint32_t)1);
	
	//Encolamos la request que informa del GP en la cola de salida 
	//del ejecutor para que lo recoja el administrador 
	((ClJSQueue *)m_pOutputQ)->Push(poJSRequestOut);

	return M4_SUCCESS;
  //## end ClLauncherForCancelExecutor28::_CleanUp%943433799.body
}

void ClLauncherForCancelExecutor28::Run ()
{
  //## begin ClLauncherForCancelExecutor28::Run%943459244.body preserve=yes
	ClBaseRequest		*poNextRequest;
//	ClLauncherStats		*poStats = (ClLauncherStats *)m_pStats;
//	ClImage				aImage;
//	m4char_t			pcBuffer[128]="";
	M4ThrowFunction_t	pfOldFunction = NULL ;
	M4CheckJump			oJump;

//	ClActualDate(pcBuffer,sizeof(pcBuffer)-1);
//	_UpdateConfigProperty(M4SubsystemStartDate, pcBuffer);
//	_UpdateConfigProperty(M4SubsystemEndDate, OBL_LAUCHER_INFO_NO_END);
	while (!IsTerminate())
	{
//		_ChangeState(ClLauncherStats::WaitState);
		if (M4_SUCCESS != _GetNextRequest(poNextRequest))
			return;

//		_ChangeState(ClLauncherStats::PrepareState);
//		ClActualDate(pcBuffer,sizeof(pcBuffer)-1);
//		_UpdateConfigProperty(OBL_LAUCHER_INFO_START, pcBuffer);
//		_UpdateConfigProperty(OBL_LAUCHER_INFO_END, OBL_LAUCHER_INFO_NO_END);
		if (M4_SUCCESS != _PrepareExecution(poNextRequest))
			return;

//		_ChangeState(ClLauncherStats::ExecuteState);
		SetUserHandler(&LogCallback, (m4pvoid_t)"Fatal Error trying to recover from error!!!");
		
		pfOldFunction = SetThrowFunction( M4JsrecoverThrow ) ;

		try
		{
			// fix bug 0079564
			oJump.Check();
			if (M4_SUCCESS != _ExecuteRequest(poNextRequest))
					return;
		}
		catch(M4RuntimeException &e)
		{
//			_ChangeState(ClLauncherStats::DiscardedState);

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

//			ClActualDate(pcBuffer,sizeof(pcBuffer)-1);
//			_UpdateConfigProperty(M4SubsystemEndDate, pcBuffer);

			SetThrowFunction( pfOldFunction ) ;

			return;

		}			

		SetThrowFunction( pfOldFunction ) ;

//		_ChangeState(ClLauncherStats::ReleaseState);
//		ClActualDate(pcBuffer,sizeof(pcBuffer)-1);
//		_UpdateConfigProperty(OBL_LAUCHER_INFO_END, pcBuffer);
		if (M4_SUCCESS != _ReplyRequest(poNextRequest))
			return;
	}

//	ClActualDate(pcBuffer,sizeof(pcBuffer)-1);
//	_UpdateConfigProperty(M4SubsystemEndDate, pcBuffer);
  //## end ClLauncherForCancelExecutor28::Run%943459244.body
}

// Additional Declarations
  //## begin ClLauncherForCancelExecutor28%36E4EB2702BC.declarations preserve=yes
  //## end ClLauncherForCancelExecutor28%36E4EB2702BC.declarations

//## begin module%36E651F00369.epilog preserve=yes
//## end module%36E651F00369.epilog

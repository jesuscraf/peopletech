//## begin module%37FB5F0E00F3.cm preserve=no
//## end module%37FB5F0E00F3.cm

//## begin module%37FB5F0E00F3.cp preserve=no
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
//## end module%37FB5F0E00F3.cp

//## Module: ClJSLauncherExecutor%37FB5F0E00F3; Package body
//## Subsystem: M4JSExeJob::src%37F8BA6602AA
//## Source file: E:\maite_work\m4jsexejob\src\cljslauncherexecutor.cpp

//## begin module%37FB5F0E00F3.additionalIncludes preserve=no
//## end module%37FB5F0E00F3.additionalIncludes

//## begin module%37FB5F0E00F3.includes preserve=yes
#include <m4jsres.hpp>
#include <m4exception.hpp> //only en cpp
#include <jsjob.hpp> //forward en hpp
//exeforjobschecexecute
#include <exeforjobschecexecute.hpp>

//jsrequest
#include <jsrequest.hpp> 
//## end module%37FB5F0E00F3.includes

// LauncherInterface
#include <launcher.hpp>
// ClAdminTag
#include <cladmintag.hpp>
// ClJSLauncherExecutor
#include <cljslauncherexecutor.hpp>


//## begin module%37FB5F0E00F3.declarations preserve=no
//## end module%37FB5F0E00F3.declarations

//## begin module%37FB5F0E00F3.additionalDeclarations preserve=yes
//## end module%37FB5F0E00F3.additionalDeclarations


// Class ClJSLauncherExecutor 

ClJSLauncherExecutor::ClJSLauncherExecutor (ClExecutive *ai_pExecutive, ClStatistic *ai_poStats, ClLog *ai_poLog, ClServiceQ *ai_poServiceQ, ClOutputQ *ai_poOutputQ)
  //## begin ClJSLauncherExecutor::ClJSLauncherExecutor%939810520.hasinit preserve=no
  //## end ClJSLauncherExecutor::ClJSLauncherExecutor%939810520.hasinit
  //## begin ClJSLauncherExecutor::ClJSLauncherExecutor%939810520.initialization preserve=yes
  :ClLauncherInterface(ai_pExecutive, ai_poStats, ai_poServiceQ), m_pOutputQ ( ai_poOutputQ )
  //## end ClJSLauncherExecutor::ClJSLauncherExecutor%939810520.initialization
{
  //## begin ClJSLauncherExecutor::ClJSLauncherExecutor%939810520.body preserve=yes
  //## end ClJSLauncherExecutor::ClJSLauncherExecutor%939810520.body
}



//## Other Operations (implementation)
m4return_t ClJSLauncherExecutor::Initialize (ClNavigatorFirewall *ai_pNavigator, ClConfiguration *ai_pConfig)
{
  //## begin ClJSLauncherExecutor::Initialize%939810523.body preserve=yes
	
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
	poJSRequestOut->AddParameter((m4uint32_t)0);

	//Almacenamos en la cola la request
	((ClJSQueue *)m_pOutputQ)->Push(poJSRequestOut);

	return M4_SUCCESS;



  //## end ClJSLauncherExecutor::Initialize%939810523.body
}

m4return_t ClJSLauncherExecutor::Destroy ()
{
  //## begin ClJSLauncherExecutor::Destroy%939810524.body preserve=yes
	m4return_t ret;

	if (NULL != m_pExecutive)
	{
		ret=m_pExecutive->ShutDown();
	

		if (M4_SUCCESS != ret)
		{
			return M4_ERROR;
		}
	}


	//Creamos una request para informar de la finalización
	//de un ejecutor
	ClJSRequest *poJSRequestOut=new ClJSRequest(M4JS_REQ_EXECUTOR_DELETED);

	//Añadimos como parámetro de la request el ejecutor
	poJSRequestOut->AddParameter(m_pExecutive);
	poJSRequestOut->AddParameter((m4uint32_t)0);

	//Encolamos la request en la cola de salida del ejecutor
	((ClJSQueue *)m_pOutputQ)->Push(poJSRequestOut);

	return M4_SUCCESS;
  //## end ClJSLauncherExecutor::Destroy%939810524.body
}

m4return_t ClJSLauncherExecutor::_GetNextRequest (ClBaseRequest *&ao_poRequest)
{
  //## begin ClJSLauncherExecutor::_GetNextRequest%943433950.body preserve=yes

			m4return_t ret;

		//Recuperamos la request del Job Scheduler de la cola de entrada 
		do
		{
			ret = ((ClJSQueue *)m_pServiceQ)->Pop((ClJSRequest *&)ao_poRequest);

			//Si fallamos al recuperar el valor de la pila no continuamos 
			//ejecutando
			if (M4_ERROR == ret)
			{
				SETCODEF ( M4_LOCKED_QUEUE, ERRORLOG , "#*s1*#");

				return M4_ERROR;
			}

		}
		while ( ao_poRequest == NULL ) ;

		return M4_SUCCESS;

  //## end ClJSLauncherExecutor::_GetNextRequest%943433950.body
}

m4return_t ClJSLauncherExecutor::_PrepareExecution (ClBaseRequest *ai_poRequest)
{
  //## begin ClJSLauncherExecutor::_PrepareExecution%943433951.body preserve=yes

	return M4_SUCCESS;

  //## end ClJSLauncherExecutor::_PrepareExecution%943433951.body
}

m4return_t ClJSLauncherExecutor::_ExecuteRequest (ClBaseRequest *ai_poRequest)
{
  //## begin ClJSLauncherExecutor::_ExecuteRequest%943433952.body preserve=yes

	m4pvoid_t pParameter=NULL; //Se obtiene un void y luego hay que hacer un casting explicito
	ClJSJob *poBox;
	m4return_t ret;


	//Obtenemos el tipo de la request
	m4uint32_t uiType=((ClJSRequest *)ai_poRequest)->GetRequestType();

	switch (uiType)
	{
		case M4JS_REQ_EXECUTE_JOB:
			{
				//Obtenemos el primer parametro de la request
				//que es la tarea a realizar
				ret=((ClJSRequest *)ai_poRequest)->GetParameter(0,pParameter);

				poBox = (ClJSJob *)pParameter;

				if (M4_SUCCESS != ret)
				{
					SETCODEF(M4_ERROR_GET_PARAMETER_JSREQUEST,ERRORLOG,"#*S1*#");
				}

				if (NULL == poBox)
				{
					SETCODEF(M4_ERROR_GET_PARAMETER_JSREQUEST,ERRORLOG,"#*S1*#");
				}

				//Indicamos al administrador que hemos comenzado la tarea

				//Para ello creamos una jsrequest del tipo que informa sobre el
				//comienzo de una tarea
				ClJSRequest *poJSRequestOut=new ClJSRequest(M4JS_REQ_BEGIN_JOB);

				//Añadimos un parametro que es el ejecutor
				poJSRequestOut->AddParameter(m_pExecutive);

				//Añadimos el segundo parámetro que es la tarea a realizar
				poJSRequestOut->AddParameter(poBox);
				
				//Hacemos un push de la jsrequest en la cola de salida
				m_pOutputQ->Push(poJSRequestOut);

				//Mandamos ejecutar la tarea
				ret=((ClExeForJobSchecEXECUTE*)m_pExecutive)->Execute(poBox);

				if (M4_SUCCESS != ret)
				{
					return M4_ERROR;
				}
			}
			break;
		case M4JS_REQ_DEL_EXECUTOR:
			{
				Terminate();
			}
	}


	return M4_SUCCESS;

  //## end ClJSLauncherExecutor::_ExecuteRequest%943433952.body
}

m4return_t ClJSLauncherExecutor::_ReplyRequest (ClBaseRequest *ai_poRequest)
{
  //## begin ClJSLauncherExecutor::_ReplyRequest%943433953.body preserve=yes

	//Informamos al administrador que la ejecución ha finalizado
	//o que el ejecutor ha sido borrado

	//Obtenemos de la jsrequest de entrada eltipo 
	//de comando :ejecución de tarea o borrado de ejecutor
	m4uint32_t uiType=((ClJSRequest *)ai_poRequest)->GetRequestType();


	if (M4JS_REQ_EXECUTE_JOB == uiType)
	{

		//Como es de ejecución de tarea informamos
		//de la finalización de la tarea

		//PAra ello obtenemos de la propia request 
		//la tarea que ha finalizado
		ClJSJob *poBox;
		m4pvoid_t pParameter=NULL; //Se obtiene un void y luego hay que hacer un casting explicito

		m4return_t ret=((ClJSRequest *)ai_poRequest)->GetParameter(0,pParameter);
		poBox = (ClJSJob *) pParameter;

		if (NULL == poBox)
		{
			SETCODEF(M4_ERROR_GET_PARAMETER_JSREQUEST,ERRORLOG,"#*S1*#");
		}


		ClJSRequest *poJSRequestOut=new ClJSRequest(M4JS_REQ_END_JOB);

		//Añadimos el primer parámetro que es el ejecutor
		poJSRequestOut->AddParameter(m_pExecutive);

		//Añadimos el segundo parámetro que es la tarea
		poJSRequestOut->AddParameter(poBox);

		//Hacemos un push en la cola de la request de salida
		((ClJSQueue *)m_pOutputQ)->Push(poJSRequestOut);

	}


	delete ai_poRequest;

	return M4_SUCCESS;

  //## end ClJSLauncherExecutor::_ReplyRequest%943433953.body
}

m4return_t ClJSLauncherExecutor::_CleanUp (ClBaseRequest *ai_poRequest)
{
  //## begin ClJSLauncherExecutor::_CleanUp%943433954.body preserve=yes

	m4return_t ret;

	//Creamos una request de JS que informe que se ha producido 
	//un GP en el ejecutor al administrador
	ClJSRequest *poJSRequestOut=new ClJSRequest(M4JS_REQ_GP);

	//Almacenamos en los parámetros de la request el puntero al
	//subsistema
	poJSRequestOut->AddParameter(m_pExecutive);
	poJSRequestOut->AddParameter((m4uint32_t)0);
			
	//Encolamos la request que informa del GP en la cola de salida 
	//del ejecutor para que lo recoja el administrador 
	((ClJSQueue *)m_pOutputQ)->Push(poJSRequestOut);
			
			
	ret=ClLauncherInterface::_CleanUp (ai_poRequest);

	return ret;



  //## end ClJSLauncherExecutor::_CleanUp%943433954.body
}

// Additional Declarations
  //## begin ClJSLauncherExecutor%37FB5DA103DE.declarations preserve=yes
  //## end ClJSLauncherExecutor%37FB5DA103DE.declarations

//## begin module%37FB5F0E00F3.epilog preserve=yes
//## end module%37FB5F0E00F3.epilog

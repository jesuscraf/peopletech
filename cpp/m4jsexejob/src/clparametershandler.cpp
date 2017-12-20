//## begin module%37E7419D02C5.cm preserve=no
//## end module%37E7419D02C5.cm

//## begin module%37E7419D02C5.cp preserve=no
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
//## end module%37E7419D02C5.cp

//## Module: ClParametersHandler%37E7419D02C5; Package body
//## Subsystem: M4JSExeJob::src%37F8BA6602AA
//## Source file: e:\m4server\m4jsexejob\src\clparametershandler.cpp

//## begin module%37E7419D02C5.additionalIncludes preserve=no
//## end module%37E7419D02C5.additionalIncludes

//## begin module%37E7419D02C5.includes preserve=yes
#include <m4jsres.hpp>

//node_x
#include <node_x.hpp>
//access
#include <access.hpp>
//executor
#include <executor.hpp>
//m4date
#include <m4date.hpp>
//jsjob
#include <jsjob.hpp>
//jsparameter
#include <jsparameter.hpp>
// defines
#include <m4jsdefinechannel.hpp>
//## end module%37E7419D02C5.includes

// ClParametersHandler
#include <clparametershandler.hpp>
#include "usractaudit.hpp"


//## begin module%37E7419D02C5.declarations preserve=no
//## end module%37E7419D02C5.declarations

//## begin module%37E7419D02C5.additionalDeclarations preserve=yes
//## end module%37E7419D02C5.additionalDeclarations


// Class ClJSParametersHandlers 









ClJSParametersHandlers::ClJSParametersHandlers (ClAccess *ai_poAccessJExec)
  //## begin ClJSParametersHandlers::ClJSParametersHandlers%937902673.hasinit preserve=no
  //## end ClJSParametersHandlers::ClJSParametersHandlers%937902673.hasinit
  //## begin ClJSParametersHandlers::ClJSParametersHandlers%937902673.initialization preserve=yes
  //## end ClJSParametersHandlers::ClJSParametersHandlers%937902673.initialization
{
  //## begin ClJSParametersHandlers::ClJSParametersHandlers%937902673.body preserve=yes

	m_poAccessExec=ai_poAccessJExec;
	m_poAccessJSJIT=NULL;
	m_poNodeJExec=NULL;
	m_poNodeParamJSJIT=NULL;
	m_uiNumberParameters=0;
	m_poBox=NULL;

  //## end ClJSParametersHandlers::ClJSParametersHandlers%937902673.body
}


ClJSParametersHandlers::~ClJSParametersHandlers ()
{
  //## begin ClJSParametersHandlers::~ClJSParametersHandlers%937902674.body preserve=yes

	m_poAccessExec=NULL;
	m_poAccessJSJIT=NULL;
	m_poNodeJExec = NULL;
	m_poNodeParamJSJIT=NULL;
	m_uiNumberParameters=0;

  //## end ClJSParametersHandlers::~ClJSParametersHandlers%937902674.body
}



//## Other Operations (implementation)
m4return_t ClJSParametersHandlers::RecoverParams ()
{
  //## begin ClJSParametersHandlers::RecoverParams%937902675.body preserve=yes
		M4ClString sNameParam;
		M4ClString sTypeParam;
		M4ClString sValueParam;
		m4bool_t IsAnyError=M4_FALSE;
		m4bool_t Continue=M4_TRUE;
	
	//Obtenemos el primer par�metro de salida de la tarea
	m4return_t ret=GetFirstOutParam(sNameParam,sTypeParam,sValueParam);

	if (M4_ERROR == ret)
	{
		IsAnyError=M4_TRUE;	

		Continue=M4_FALSE;

	}

	//Si el m�todo nos devuelve M4_WARNING significa
	//que ya no hay m�s par�metros de salida
	while (M4_WARNING != ret)
	{
		if (Continue == M4_TRUE)
		{
			//Almacenamos el par�metro de salida
			//en BBDD en el canal CH_JS_EXECUTIONS
			ret=StorageParamOut(sNameParam,sValueParam,sTypeParam);

			if (M4_SUCCESS != ret)
			{
				IsAnyError=M4_TRUE;	
			}
		}

		//Obtenemos el siguiente par�metro
		//de salida
		ret=GetParamOut(sNameParam,sTypeParam,sValueParam);

		if (M4_ERROR == ret)
		{
			IsAnyError=M4_TRUE;	

			Continue=M4_FALSE;
		}
		else
		{
			Continue=M4_TRUE;
		}

	}
	
	if (IsAnyError == M4_TRUE)
	{
		return  M4_ERROR;
	}

	return M4_SUCCESS;

  //## end ClJSParametersHandlers::RecoverParams%937902675.body
}

m4return_t ClJSParametersHandlers::RecoverParamOut (m4pcchar_t ai_pcParamName, m4pcchar_t ai_pcParamType)
{
  //## begin ClJSParametersHandlers::RecoverParamOut%937902679.body preserve=yes


	//El par�metro que nos pasan tenemos que almacenarlo en 
	//el nodo de par�metros del canal C4_JS_EXE_JIT
	m4VariantType arg;

	//Le pasamos el nonbre del par�mero
	arg.Type=M4CL_CPP_TYPE_STRING_VAR;
	arg.Data.PointerChar=(m4pchar_t)ai_pcParamName;

	m4return_t ret=m_poAccessJSJIT->GetpExecutor()->Stack.Push(arg);

	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_PUSH_STACK_ERROR, ERRORLOG, "#*S1*#%s#%s#", "ADD_PARAM_OUT", "C4_JS_EXE_JIT");
		SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}

	//Le pasamos el tipo del par�metro
	arg.Type=M4CL_CPP_TYPE_STRING_VAR;
	arg.Data.PointerChar=(m4pchar_t)ai_pcParamType;

	ret=m_poAccessJSJIT->GetpExecutor()->Stack.Push(arg);

	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_PUSH_STACK_ERROR, ERRORLOG, "#*S1*#%s#%s#", "ADD_PARAM_OUT", "C4_JS_EXE_JIT");
		SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}

	//Llamamos al m�todo que crea una nueva entrada
	ret = m_poNodeParamJSJIT->RecordSet.Current.Item["ADD_PARAM_OUT"].Call();
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#%s#", "ADD_PARAM_OUT", "C4_JS_EXE_JIT");
		SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}

	//Recogemos el valor que devuelve el m�todo
	ret=m_poAccessJSJIT->GetpExecutor()->Stack.Pop(arg);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_POP_STACK_ERROR, ERRORLOG, "#*S1*#%s#%s#", "ADD_PARAM_OUT", "C4_JS_EXE_JIT");
		SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}

	if (M4_PARAM_REPEAT == arg.Data.DoubleData)
	{
		SETCODEF(M4_ERROR_PARAM_REPEAT,ERRORLOG,"#*S1*#%s#",ai_pcParamName);
		SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
	}

	return M4_SUCCESS;


  //## end ClJSParametersHandlers::RecoverParamOut%937902679.body
}

m4return_t ClJSParametersHandlers::RecoverParamIn (m4pcchar_t ai_pcParamName, m4pcchar_t ai_pcParamType, m4pcchar_t ai_pcParamValue)
{
  //## begin ClJSParametersHandlers::RecoverParamIn%937902680.body preserve=yes

	//Determinamos el tipo de par�metro
	m4uint16_t IsNumber=-1;
	m4uint16_t IsDate=-1;
	m4uint16_t IsString=-1;
	m4pchar_t cadena;
	m4double_t fecha;
	m4double_t numero;

	//Determinamos que tipo de par�metro 
	//de entrada nos han pasado
	if ((IsNumber=strcmp(ai_pcParamType,"NUMBER"))!=0)
	{
		if ((IsDate=strcmp(ai_pcParamType,"DATE"))!=0)
		{
			if (((IsString=strcmp(ai_pcParamType,"STRING"))!=0) &&
				 ((IsString=strcmp(ai_pcParamType,"EXECUTABLE"))!=0))
			{
				//Si el tipo que nos han pasado nos se corresponde
				//con ninguno conocidos se indica con un mensaje de error
				SETCODEF(M4_TYPE_PARM_ERROR,ERRORLOG,"#*S1*#%s#%s#",ai_pcParamName,ai_pcParamType);

				SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");

				return M4_ERROR;
			}
			else
			{	
				//Comprobamos si el valor de una cadena es nulo 
				//en cuyo caso se considera que la cadena es vac�a
				if (ai_pcParamValue == NULL)
				{
					cadena = "";
				}
				else
				{
					//Como es un par�metro de tipo cadena no 
					//tenemos que realizar conversi�n
					cadena=(m4pchar_t)ai_pcParamValue;
				}
			}
		}
		else
		{
			//Comprobamos si el valor para un tipo fecha
			//es nulo en cuyo caso lo consideramos un error
			if ( ai_pcParamValue == NULL)
			{
				SETCODEF(M4_ERROR_PARAM_NULL,ERRORLOG,"#*S1*#%s#",ai_pcParamName);			
				SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*s1*#");
				return M4_ERROR;
			}
			else
			{						
				//Tenemos que convertir la cadena que representa
				//la fecha en una fecha en juliano
				M4ClTimeStamp convert;
				convert.set(ai_pcParamValue);
				convert.get(fecha);
			}
		}
	}
	else
	{
		//Comprobamos si el valor de un tipo num�rico
		//es nulo en cuyo caso lo consideramos un error
		if ( ai_pcParamValue == NULL)
		{
			SETCODEF(M4_ERROR_PARAM_NULL,ERRORLOG,"#*S1*#%s#",ai_pcParamName);
			SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*s1*#");
			return M4_ERROR;
		}
		else
		{
			//Tenemos que convertir la cadena que representa
			// el n�mero en un n�mero
				numero=atof(ai_pcParamValue);
		}
	}
	
	//Almacenamos el nombre y el valor en la pila de MVC
	//Almacenamos el nombre del par�metro
	m4VariantType arg;
	arg.Type=M4CL_CPP_TYPE_STRING_VAR;
	arg.Data.PointerChar=(m4pchar_t)ai_pcParamName;

	m4return_t ret=m_poAccessJSJIT->GetpExecutor()->Stack.Push(arg);

	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_PUSH_STACK_ERROR, ERRORLOG, "#*S1*#%s#%s", "EXE_JIT_WITH_PARAMS", "C4_JS_EXE_JIT");
		SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}

    m_uiNumberParameters++;	

	if (IsNumber == 0)
	{
		arg.Type=M4CL_CPP_TYPE_NUMBER;
		arg.Data.DoubleData=numero;
	}

	if (IsDate == 0)
	{
		arg.Type=M4CL_CPP_TYPE_NUMBER;
		arg.Data.DoubleData=fecha;
	}

	if (IsString ==0)
	{
		arg.Type=M4CL_CPP_TYPE_STRING_VAR;
		arg.Data.PointerChar=cadena;
	}

	ret=m_poAccessJSJIT->GetpExecutor()->Stack.Push(arg);

	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_PUSH_STACK_ERROR, ERRORLOG, "#*S1*#%s#%s", "EXE_JIT_WITH_PARAMS","C4_JS_EXE_JIT");
		SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}

	m_uiNumberParameters++;

	return M4_SUCCESS;

  //## end ClJSParametersHandlers::RecoverParamIn%937902680.body
}

m4return_t ClJSParametersHandlers::AddParams (m4VariantType ai_oCode, m4uint32_t ai_idJobDEF, m4uint32_t ai_idBoxExe, m4uint32_t ai_idJobGroupDEF)
{
  //## begin ClJSParametersHandlers::AddParams%937902676.body preserve=yes
		
	M4ClString oName, oValue, oType;
	m4return_t ret = M4_ERROR;
	m4VariantType arg1,arg2,arg3,resultarg;

	m4pcchar_t		pccEventArgs = NULL ;
	m4char_t		acBuffer[ 128 ] ;
	ClUserAction	*poAction = NULL ;


	//Obtenemos el acceso al canal de ejecuciones del JIT
	ClAccess *poAccessJIT=m_poNodeParamJSJIT->GetpAccess();

	// Auditor�a de usuario
	if( poAccessJIT->GetpChannel()->GetpChannelManager()->UserActionsAudit.HasAuditory( ClUserAction::EV_JOB, NULL ) == M4_TRUE )
	{
		poAction = new ClUserAction() ;
	}


	//Almacenamos el jit en la pila de la m�quina virtual
	ret=poAccessJIT->GetpExecutor()->Stack.Push(ai_oCode);

	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_PUSH_STACK_ERROR,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}

	//Obtenemos el objeto que contiene toda la informaci�n de
	//los par�metros de la tarea
	ClJSParameter *poParameter=m_poBox->GetFirstInputParameter();

	//Mientras siga habiendo par�metros
	while (NULL != poParameter)
	{	
		//Obtenemos el nombre , tipo y valor del par�metro
		//de entrada
		//NOMBRE
		ret=poParameter->GetName(oName);
		if (M4_SUCCESS != ret)
		{
			SETCODEF(M4_ERROR_GET_NAME_PARAMETER,ERRORLOG,"#*S1*#");
			return M4_ERROR;
		}

		//TIPO	
		ret=poParameter->GetTypeAsString(oType);
		if (M4_SUCCESS != ret)
		{
			SETCODEF(M4_ERROR_GET_TYPE_PARAMETER,ERRORLOG,"#*S1*#");
			return M4_ERROR;
		}
	
		//VALOR
		ret=poParameter->GetValue(oValue);
		if (M4_SUCCESS != ret)
		{
			SETCODEF(M4_ERROR_GET_VALUE_PARAMETER,ERRORLOG,"#*S1*#");
			return M4_ERROR;
		}

		//Grabamos el valor del par�metro en la 
		//base de datos
		// PARAM_NAME
		arg1.Type=M4CL_CPP_TYPE_STRING_VAR;
		arg1.Data.PointerChar=(m4pchar_t)oName.c_str();

		ret=m_poAccessExec->GetpExecutor()->Stack.Push(arg1);
		if (M4_SUCCESS != ret)
		{
			SETCODEF(M4_PUSH_STACK_ERROR,ERRORLOG,"#*S1*#");
			return M4_ERROR;
		}

		// PARAM_VALUE
		arg2.Type=M4CL_CPP_TYPE_STRING_VAR;
		arg2.Data.PointerChar=(m4pchar_t)oValue.c_str();
		ret = m_poAccessExec->GetpExecutor()->Stack.Push(arg2);
		if (M4_SUCCESS != ret)
		{
			SETCODEF(M4_PUSH_STACK_ERROR,ERRORLOG,"#*S1*#");
			return M4_ERROR;
		}
		
		ret = m_poNodeJExec->RecordSet.Current.Item["ADD_INPUT_PARAM"].Call();
		if (M4_SUCCESS != ret)
		{
			SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#C4_JS_TASK_EXE#", "ADD_INPUT_PARAM");
			return M4_ERROR;
		}

		// Recuperamos el valor que retorna la funci�n
		ret = m_poAccessExec->GetpExecutor()->Stack.Pop(resultarg);
		if (ret != M4_SUCCESS)
		{
			SETCODEF(M4_POP_STACK_ERROR, ERRORLOG, "#*s1*#%s#C4_JS_TASK_EXE#", "ADD_INPUT_PARAM");
			return M4_ERROR;
		}

		if (resultarg.Data.DoubleData != 0)
		{
			SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#C4_JS_TASK_EXE#", "ADD_INPUT_PARAM");
			return M4_ERROR;
		}

		// Almacenamos en donde se indique los valores del par�metro
		// ret = RecoverParamIn(oName.c_str(), oType.c_str(), oValue.c_str());
		ret = RecoverParamIn(oName.c_str(), oType.c_str(), oValue.c_str());
		if (M4_SUCCESS != ret)
		{
			return M4_ERROR;
		}

		//Repetimos los pasos como antes para obtener el 
		//siguiente par�metro
		poParameter=m_poBox->GetNextInputParameter();			

		// Se vuelca los par�metros para uditor�a
		if( poAction != NULL )
		{
			pccEventArgs = poAction->GetEventArgs();

			if( pccEventArgs != NULL && *pccEventArgs != '\0' )
			{
				poAction->AppendEventArgs( " | " ) ;
			}
			poAction->AppendEventArgs( oName.c_str() ) ;
			poAction->AppendEventArgs( " = <" ) ;
			poAction->AppendEventArgs( oValue.c_str() ) ;
			poAction->AppendEventArgs( ">" ) ;
		}
	}

	// Auditor�a de usuario
	if( poAction != NULL )
	{
		poAction->SetEventID( ClUserAction::EV_JOB ) ;

		poAction->AppendEventObject( "Task = " ) ;
		sprintf( acBuffer, "%0.d", ai_idJobGroupDEF ) ;
		poAction->AppendEventObject( acBuffer ) ;

		poAction->AppendEventObject( " | " ) ;

		poAction->AppendEventObject( "Subtask = " ) ;
		sprintf( acBuffer, "%0.d", ai_idJobDEF ) ;
		poAction->AppendEventObject( acBuffer ) ;

		poAction->AppendEventObject( " | " ) ;

		poAction->AppendEventObject( "Execution = " ) ;
		sprintf( acBuffer, "%0.d", ai_idBoxExe ) ;
		poAction->AppendEventObject( acBuffer ) ;

		poAccessJIT->GetpChannel()->GetpChannelManager()->UserActionsAudit.AddAction( poAction ) ;
	}


	//Ahora obtenemos los par�metro de SALIDA de la tarea
	poParameter=m_poBox->GetFirstOutputParameter();
		
	//Mientras existan par�metros de salida continuamos ejecutando
	while (NULL != poParameter)
	{
		//REcuperamos los datos del par�metro de salida

		//NOMBRE
		ret=poParameter->GetName(oName);
		if (M4_SUCCESS != ret)
		{
			SETCODEF(M4_ERROR_GET_NAME_PARAMETER,ERRORLOG,"#*S1*#");
			return M4_ERROR;
		}

		ret=poParameter->GetTypeAsString(oType);
		if (M4_SUCCESS != ret)
		{
			SETCODEF(M4_ERROR_GET_TYPE_PARAMETER,ERRORLOG,"#*S1*#");
			return M4_ERROR;
		}

		//Llamamos al m�todo que trata los par�metros de entrada
		ret=RecoverParamOut(oName.c_str(),oType.c_str());
		if (M4_SUCCESS != ret)
		{
			return M4_ERROR;
		}
		
		//REpetimos los pasos anteriores para obtener el 
		//siguiente par�metro de salida
		poParameter=m_poBox->GetNextOutputParameter();
	}

	return M4_SUCCESS;

  //## end ClJSParametersHandlers::AddParams%937902676.body
}

m4return_t ClJSParametersHandlers::GetParamOut (M4ClString &ao_sParamName, M4ClString &ao_sParamType, M4ClString &ao_sParamValue)
{
  //## begin ClJSParametersHandlers::GetParamOut%937902681.body preserve=yes

	//Almacenamos en la pila de la MVC los par�metros 
	//del m�todo que recupera los valores de los par�metros 
	//de salida
	m4VariantType arg,arg1,arg2,arg3;

	m4return_t ret=m_poAccessJSJIT->GetpExecutor()->Stack.Push(arg);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_PUSH_STACK_ERROR, ERRORLOG, "#*S1*#%s#%s#", "GET_NEXT_OUTPUT_PARAM", "C4_JS_EXE_JIT");
		SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}

	ret=m_poAccessJSJIT->GetpExecutor()->Stack.Push(arg);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_PUSH_STACK_ERROR, ERRORLOG, "#*S1*#%s#%s#", "GET_NEXT_OUTPUT_PARAM", "C4_JS_EXE_JIT");
		SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}

	ret=m_poAccessJSJIT->GetpExecutor()->Stack.Push(arg);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_PUSH_STACK_ERROR, ERRORLOG, "#*S1*#%s#%s#", "GET_NEXT_OUTPUT_PARAM", "C4_JS_EXE_JIT");
		SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}

	//Llamamos al m�todo que nos obtiene el primer par�metro de salida de la tarea
	ret = m_poNodeParamJSJIT->RecordSet.Current.Item["GET_NEXT_OUTPUT_PARAM"].Call();
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#%s#", "GET_NEXT_OUTPUT_PARAM", "C4_JS_EXE_JIT");
		SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}

	//Recuperamos el primer par�metro del m�todo que es 
	//el nombre del primer par�metro de la tarea
	ret=m_poAccessJSJIT->GetpExecutor()->Stack.Pop(arg1);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_POP_STACK_ERROR, ERRORLOG, "#*S1*#%s#%s#", "GET_NEXT_OUTPUT_PARAM", "C4_JS_EXE_JIT");
		SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}

	//Recuperamos el segundo par�metro del m�todo que es 
	//el tipo de un par�metro de la tarea
	ret=m_poAccessJSJIT->GetpExecutor()->Stack.Pop(arg2);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_POP_STACK_ERROR, ERRORLOG, "#*S1*#%s#%s#", "GET_NEXT_OUTPUT_PARAM", "C4_JS_EXE_JIT");
		SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");	
		return M4_ERROR;
	}

	//Recuperamos el tercer par�metro del m�todo que es 
	//el valor  de un  par�metro de la tarea
	ret=m_poAccessJSJIT->GetpExecutor()->Stack.Pop(arg3);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_POP_STACK_ERROR, ERRORLOG, "#*S1*#%s#%s#", "GET_NEXT_OUTPUT_PARAM", "C4_JS_EXE_JIT");
		SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");	
		return M4_ERROR;
	}

	//REcuperamos el valor de retorno de la funci�n
	ret=m_poAccessJSJIT->GetpExecutor()->Stack.Pop(arg);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_POP_STACK_ERROR, ERRORLOG, "#*S1*#%s#%s#", "GET_NEXT_OUTPUT_PARAM", "C4_JS_EXE_JIT");
		return M4_ERROR;
	}

	//Si el m�todo ha devuelto una condici�n de error
	//no continuamos ejecutando
	if (COND_ERROR == arg.Data.DoubleData)
	{
		SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#%s#", "GET_NEXT_OUTPUT_PARAM", "C4_JS_EXE_JIT");
		SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}

	//Si el m�todo ha devuelto una condici�n de finalizaci�n
	//devolvemos una valor de warning
	if (COND_EOF == arg.Data.DoubleData)
	{
		return M4_WARNING;
	}

	//Antes de devolver los valores tenemos que comprobar 
	//que ninguno de ellos es nulo
	if( M4CL_CPP_TYPE_NULL ==arg1.Type)
	{
		SETCODEF(M4_WARNING_PARAMETER_INFO,WARNINGLOG,"#*S1*#");
	}
	else
	{
		if( M4CL_CPP_TYPE_NULL ==arg2.Type)
		{
			SETCODEF(M4_WARNING_PARAMETER_INFO,WARNINGLOG,"#*S1*#");
		}
		else
		{
			if( M4CL_CPP_TYPE_NULL ==arg3.Type)
			{
				SETCODEF(M4_WARNING_PARAMETER_INFO,WARNINGLOG,"#*S1*#");
				arg3.Data.PointerChar="";
			}

			//Si no se ha producido un error o no hemos finalizado
			//el recorrido de los par�metros de salida en el canal
			//de ejecuciones del JIT 
			ao_sParamName=arg1.Data.PointerChar;
			ao_sParamType=arg2.Data.PointerChar;
			ao_sParamValue=arg3.Data.PointerChar;
		}
	}
	
	return M4_SUCCESS;

  //## end ClJSParametersHandlers::GetParamOut%937902681.body
}

m4return_t ClJSParametersHandlers::StorageParamOut (M4ClString ai_sParamName, M4ClString ai_sParamValue, M4ClString ai_sParamType)
{
  //## begin ClJSParametersHandlers::StorageParamOut%937902682.body preserve=yes

	//Almacenamos el valor del par�metro de salida cuyo nombre
	//coincide con el proporcionado a este m�todo

	//Propagamos el par�metro a todos los par�metros 
	//de entrada cuyo valor est� conectado con �l
	m4return_t ret=m_poBox->SetParamValue(ai_sParamName,ai_sParamValue);

	//Almacenamos primero el nombre
	m4VariantType arg;
	arg.Type=M4CL_CPP_TYPE_STRING_VAR;
	arg.Data.PointerChar=ai_sParamName;

	ret=m_poAccessExec->GetpExecutor()->Stack.Push(arg);
	if (M4_SUCCESS != ret)
	{	
		SETCODEF(M4_PUSH_STACK_ERROR, ERRORLOG, "#*S1*#%s#%s#", "ADD_OUTPUT_PARAM", "C4_JS_TASK_EXE");
		SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}

	//Almacenamos el valor del par�metro de salida
	arg.Type=M4CL_CPP_TYPE_STRING_VAR;
	arg.Data.PointerChar=ai_sParamValue;

	ret=m_poAccessExec->GetpExecutor()->Stack.Push(arg);
	if (M4_SUCCESS != ret)
	{	
		SETCODEF(M4_PUSH_STACK_ERROR, ERRORLOG, "#*S1*#%s#%s#", "ADD_OUTPUT_PARAM", "C4_JS_TASK_EXE");
		SETCODEF(M4_ERROR_CLIENT_RECOVER_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}

	//Llamamos al m�todo que busca el par�metro en el nodo de 
	//recursos del canal C4_JS_TASK_EXE y le asigna el valor de
	//salida
	ret = m_poNodeJExec->RecordSet.Current.Item["ADD_OUTPUT_PARAM"].Call();
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#%s#", "ADD_OUTPUT_PARAM", "C4_JS_TASK_EXE");
		SETCODEF(M4_ERROR_CLIENT_RECOVER_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}

	//Recuperamos el valor que devuelve la funci�n
	ret=m_poAccessExec->GetpExecutor()->Stack.Pop(arg);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_POP_STACK_ERROR, ERRORLOG, "#*S1*#%s#%s#", "ADD_OUTPUT_PARAM", "C4_JS_TASK_EXE");
		SETCODEF(M4_ERROR_CLIENT_RECOVER_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}

	if (arg.Data.DoubleData == COND_ERROR)
	{
		SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#%s#", "ADD_OUTPUT_PARAM", "C4_JS_TASK_EXE");
		SETCODEF(M4_ERROR_CLIENT_RECOVER_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}

	return M4_SUCCESS ;
		
  //## end ClJSParametersHandlers::StorageParamOut%937902682.body
}

void ClJSParametersHandlers::GetParam ()
{
  //## begin ClJSParametersHandlers::GetParam%937902683.body preserve=yes
  //## end ClJSParametersHandlers::GetParam%937902683.body
}

m4return_t ClJSParametersHandlers::SetExecutionAccess (ClAccess *ai_poAccess)
{
  //## begin ClJSParametersHandlers::SetExecutionAccess%937902678.body preserve=yes

	m_poAccessJSJIT=ai_poAccess;

	//Accedemos al nodo de recursos del canal de CH_JS_EXE_JIT
	m_poNodeParamJSJIT = &(m_poAccessJSJIT->Node["N4_JS_OUT_PARAMS"]);

	if (m_poNodeParamJSJIT == NULL)
	{
		SETCODEF(M4_ERROR_CREATE_NODE,ERRORLOG, "#*S1*#%s#%s", "N4_JS_OUT_PARAMS", "C4_JS_EXE_JIT");
		SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}

	return M4_SUCCESS;

  //## end ClJSParametersHandlers::SetExecutionAccess%937902678.body
}

m4uint32_t ClJSParametersHandlers::GetNumberParameters ()
{
  //## begin ClJSParametersHandlers::GetNumberParameters%937996396.body preserve=yes

	return m_uiNumberParameters;

  //## end ClJSParametersHandlers::GetNumberParameters%937996396.body
}

m4return_t ClJSParametersHandlers::GetFirstOutParam (M4ClString &ao_sParamName, M4ClString &ao_sParamType, M4ClString &ao_sParamValue)
{
  //## begin ClJSParametersHandlers::GetFirstOutParam%938420311.body preserve=yes

	//Almacenamos en la pila de la MVC los par�metros 
	//del m�todo que recupera los valores de los par�metros 
	//de salida
	m4VariantType arg,arg1,arg2,arg3;

	m4return_t ret=m_poAccessJSJIT->GetpExecutor()->Stack.Push(arg);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_PUSH_STACK_ERROR,ERRORLOG, "#*S1*#%s#%s#", "GET_FIRST_OUTPUT_PARAM", "C4_JS_EXE_JIT");
		SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}

	ret=m_poAccessJSJIT->GetpExecutor()->Stack.Push(arg);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_PUSH_STACK_ERROR, ERRORLOG, "#*S1*#%s#%s#", "GET_FIRST_OUTPUT_PARAM", "C4_JS_EXE_JIT");
		SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}

	ret=m_poAccessJSJIT->GetpExecutor()->Stack.Push(arg);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_PUSH_STACK_ERROR, ERRORLOG, "#*S1*#%s#%s#", "GET_FIRST_OUTPUT_PARAM", "C4_JS_EXE_JIT");
		SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}

	//Llamamos al m�todo que nos obtiene el primer par�metro de salida de la tarea
	ret = m_poNodeParamJSJIT->RecordSet.Current.Item["GET_FIRST_OUTPUT_PARAM"].Call();
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#%s#", "GET_FIRST_OUTPUT_PARAM", "C4_JS_EXE_JIT");
		SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}

	//Recuperamos el primer par�metro del m�todo que es 
	//el nombre del primer par�metro de la tarea
	ret=m_poAccessJSJIT->GetpExecutor()->Stack.Pop(arg1);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_POP_STACK_ERROR, ERRORLOG, "#*S1*#%s#%s#", "GET_FIRST_OUTPUT_PARAM", "C4_JS_EXE_JIT");
		SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}

	//Recuperamos el segundo par�metro del m�todo que es 
	//el tipo de un par�metro de la tarea
	ret=m_poAccessJSJIT->GetpExecutor()->Stack.Pop(arg2);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_POP_STACK_ERROR, ERRORLOG, "#*S1*#%s#%s#", "GET_FIRST_OUTPUT_PARAM", "C4_JS_EXE_JIT");
		SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}

	//Recuperamos el tercer par�metro del m�todo que es 
	//el valor  de un  par�metro de la tarea
	ret=m_poAccessJSJIT->GetpExecutor()->Stack.Pop(arg3);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_POP_STACK_ERROR, ERRORLOG, "#*S1*#%s#%s#", "GET_FIRST_OUTPUT_PARAM", "C4_JS_EXE_JIT");
		SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");	
		return M4_ERROR;
	}

	//REcuperamos el valor de retorno de la funci�n
	ret=m_poAccessJSJIT->GetpExecutor()->Stack.Pop(arg);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_POP_STACK_ERROR, ERRORLOG, "#*S1*#%s#%s#", "GET_FIRST_OUTPUT_PARAM", "C4_JS_EXE_JIT");
		SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}

	//Si el m�todo ha devuelto una condici�n de error
	//no continuamos ejecutando
	if (COND_ERROR == arg.Data.DoubleData)
	{
		SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#%s#", "GET_FIRST_OUTPUT_PARAM", "C4_JS_EXE_JIT");
		SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER,ERRORLOG,"#*S1*#");
		return M4_ERROR;
	}	

	//Si el m�todo ha devuelto una condici�n de finalizaci�n
	//devolvemos una valor de warning
	if (COND_EOF == arg.Data.DoubleData)
	{
		return M4_WARNING;
	}

	//Antes de devolver los valores tenemos que comprobar 
	//que ninguno de ellos es nulo
	if( M4CL_CPP_TYPE_NULL ==arg1.Type)
	{
		SETCODEF(M4_ERROR_PARAMETER_NAME,ERRORLOG,"#*S1*#");
	}
	else
	{
		if( M4CL_CPP_TYPE_NULL ==arg2.Type)
		{
			SETCODEF(M4_ERROR_PARAMETER_TYPE,ERRORLOG,"#*S1*#");
		}
		else
		{
			if( M4CL_CPP_TYPE_NULL ==arg3.Type)
			{
				SETCODEF(M4_WARNING_PARAMETER_INFO,WARNINGLOG,"#*S1*#%s#",arg1.Data.PointerChar);

				arg3.Data.PointerChar="";
			}

			//Si no se ha producido un error o no hemos finalizado
			//el recorrido de los par�metros de salida en el canal
			//de ejecuciones del JIT 
			ao_sParamName=arg1.Data.PointerChar;
			ao_sParamType=arg2.Data.PointerChar;
			ao_sParamValue=arg3.Data.PointerChar;
		}
	}

	return M4_SUCCESS;
  //## end ClJSParametersHandlers::GetFirstOutParam%938420311.body
}

void ClJSParametersHandlers::SetBox (ClJSJob *ai_poBox)
{
  //## begin ClJSParametersHandlers::SetBox%941717918.body preserve=yes

    m_poBox=ai_poBox;

  //## end ClJSParametersHandlers::SetBox%941717918.body
}

m4return_t ClJSParametersHandlers::Initialize ()
{
  //## begin ClJSParametersHandlers::Initialize%941717919.body preserve=yes

	//Inicializamos el n�mero de par�metros
	m_uiNumberParameters=1;

	//Inicializamos el nodo de par�metros del canal
	//de ejecuciones
    m_poNodeJExec=&(m_poAccessExec->Node["N4_JS_SUBTASK_EXE"]);
	if (NULL == m_poNodeJExec)
	{
		SETCODEF(M4_ERROR_CREATE_NODE, ERRORLOG, "#*S1*#%s#C4_JS_TASK_EXE#", "N4_JS_SUBTASK_EXE");
		return M4_ERROR;
	}

	return M4_SUCCESS ;

  //## end ClJSParametersHandlers::Initialize%941717919.body
}

m4return_t ClJSParametersHandlers::RecoverCompoundJobParams (m4uint32_t ai_bInParamValue, m4uint32_t ai_bOutParamValue)
{
  //## begin ClJSParametersHandlers::RecoverCompoundJobParams%958997022.body preserve=yes

	m4return_t	ret;
	M4ClString	stParamName, stParamValue;
	m4VariantType vParamName, vParamValue, vFinalParamValue, vResult;
	ClJSJob * pTarea;
	
	// Se propagan los valores de los par�metros de entrada/salida
	// del trabajo. De esta manera, se obtendr�n los valores de entrada
	// y los de salida de la tarea compuesta.
	pTarea = m_poBox->GetParent();
	pTarea->PropagateAllParamValues();
	
	// Se inicializa el nodo de tareas del canal de ejecuciones.
    ClNode * poNodeJGExe = &(m_poAccessExec->Node["N4_JS_TASK_EXE"]);
	if (poNodeJGExe == NULL)
	{
		SETCODEF(M4_ERROR_CREATE_NODE, ERRORLOG, "#*S1*#%s#C4_JS_TASK_EXE#", "N4_JS_TASK_EXE");
		return M4_ERROR;
	}

	ClJSParameter * poParameter;
	if (ai_bInParamValue)
	{
		// Se almacena en base de datos los par�metros y su valor.
		// Para ello, se obtiene el objeto que contiene toda la 
		// informaci�n de los par�metros de la tarea compuesta.
		poParameter = pTarea->GetFirstInputParameter();

		// Para todos los par�metros de entrada.
		while (poParameter != NULL)
		{	
			// Se obtiene el nombre y valor del par�metro
			// de entrada
		
			// Se obtiene el nombre del par�metro.
			ret = poParameter->GetName(stParamName);
			if (ret != M4_SUCCESS)
			{
				SETCODEF(M4_ERROR_GET_NAME_PARAMETER, ERRORLOG, "#*S1*#");
				return M4_ERROR;
			}
	
			// Se obtiene el valor del par�metro.
			ret = poParameter->GetValue(stParamValue);
			if (ret != M4_SUCCESS)
			{
				SETCODEF(M4_ERROR_GET_VALUE_PARAMETER, ERRORLOG, "#*S1*#");
				return M4_ERROR;
			}

			// Se graba en base de datos el valor del par�metro.
			vParamName.Type = M4CL_CPP_TYPE_STRING_VAR;
			vParamName.Data.PointerChar = (m4pchar_t)stParamName.c_str();

			ret = m_poAccessExec->GetpExecutor()->Stack.Push(vParamName);
			if (ret != M4_SUCCESS)
			{
				SETCODEF(M4_PUSH_STACK_ERROR, ERRORLOG, "#*S1*#");
				return M4_ERROR;
			}

			vParamValue.Type = M4CL_CPP_TYPE_STRING_VAR;
			vParamValue.Data.PointerChar = (m4pchar_t)stParamValue.c_str();
		
			ret = m_poAccessExec->GetpExecutor()->Stack.Push(vParamValue);
			if (ret != M4_SUCCESS)
			{
				SETCODEF(M4_PUSH_STACK_ERROR, ERRORLOG, "#*S1*#");
				return M4_ERROR;
			}
		
			ret = m_poAccessExec->GetpExecutor()->Stack.Push(vFinalParamValue);
			if (ret != M4_SUCCESS)
			{
				SETCODEF(M4_PUSH_STACK_ERROR, ERRORLOG, "#*S1*#");
				return M4_ERROR;
			}
		
			ret = poNodeJGExe->RecordSet.Current.Item["ADD_INPUT_PARAM"].Call();
			if (ret != M4_SUCCESS)
			{
				SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#C4_JS_TASK_EXE#", "ADD_INPUT_PARAM");
				return M4_ERROR;
			}

			// Se recupera el valor que retorna del m�todo LN4 ejecutado.
			if ((m_poAccessExec->GetpExecutor()->Stack.Pop(vFinalParamValue) != M4_SUCCESS) ||
				(m_poAccessExec->GetpExecutor()->Stack.Pop(vResult) != M4_SUCCESS))
			{
				SETCODEF(M4_POP_STACK_ERROR, ERRORLOG, "#*S1*#%s#%s#", "ADD_INPUT_PARAM", "C4_JS_TASK_EXE");
				SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER, ERRORLOG, "#*S1*#");
				return M4_ERROR;
			}

			// Si el m�todo ha devuelto 1, es un warning avisando
			// que el par�metro est� ya en base de datos. No interesa
			// recoger esta informaci�n, s�lo si ha habido un error.
			if (vResult.Data.DoubleData == COND_ERROR)
			{
				SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#C4_JS_TASK_EXE#", "ADD_INPUT_PARAM");
				return M4_ERROR;
			}

			// Si el valor final del par�metro es distinto del valor candidato
			// proporcionado como argumento de entrada, significa que el par�metro
			// es dependiente de la sociedad y tiene un valor diferente.
			// Este es el valor real del par�metro de la tarea.
			if (strcmp(stParamValue.c_str(), vFinalParamValue.Data.PointerChar))
			{
				// Se guarda el nuevo valor del par�metro
				pTarea->SetParamValue(stParamName.c_str(), vFinalParamValue.Data.PointerChar);

				// Se  propagan su valor.
				pTarea->PropagateAllParamValues();
			}
			
			// Se repiten los pasos anteriores  para obtener el 
			// siguiente par�metro.
			poParameter = pTarea->GetNextInputParameter();			
		}
	}

	// Se obtienen los par�metros de salida de la tarea.
	poParameter = pTarea->GetFirstOutputParameter();
		
	// Mientras existan par�metros de salida continuamos 
	// ejecutando.
	while (poParameter != NULL)
	{
		// Se recuperamos los datos de los posibles
		// par�metros de salida.

		// Nombre del par�metro de salida.
		ret = poParameter->GetName(stParamName);
		if (ret != M4_SUCCESS)
		{
			SETCODEF(M4_ERROR_GET_NAME_PARAMETER, ERRORLOG, "#*S1*#");
			return M4_ERROR;
		}

		// Se graba en base de datos el valor del par�metro.
		vParamName.Type = M4CL_CPP_TYPE_STRING_VAR;
		vParamName.Data.PointerChar = (m4pchar_t)stParamName.c_str();

		ret = m_poAccessExec->GetpExecutor()->Stack.Push(vParamName);
		if (ret != M4_SUCCESS)
		{
			SETCODEF(M4_PUSH_STACK_ERROR, ERRORLOG, "#*S1*#");
			return M4_ERROR;
		}

		// Se obtiene el valor del par�metro.
		// S�lo se obtiene el valor del par�metro de salida, si el
		// par�metro de entrada ai_bNullOutParamValue = 1.
		// Si ai_bNullOutParamValue = 0, significa que el 
		// par�metro de salida no tiene valor asignado porque
		// todav�a no hemos ejecutado la subtarea, pero se quiere
		// salvar el registro correspondiente a este par�metro en
		// la base de datos con valor null. Si ai_bNullOutParamValue
		// fuese 1, se grabar�a el valor que tiene porque se supone 
		// que ya se ha ejecutado la subtarea y tiene el valor asignado.
		stParamValue = "";
		if (ai_bOutParamValue)
		{
			ret = poParameter->GetValue(stParamValue);
			if (ret != M4_SUCCESS)
			{
				SETCODEF(M4_ERROR_GET_VALUE_PARAMETER, ERRORLOG, "#*S1*#");
				return M4_ERROR;
			}
		}
		
		vParamValue.Type = M4CL_CPP_TYPE_STRING_VAR;
		vParamValue.Data.PointerChar = (m4pchar_t)stParamValue.c_str();

		ret = m_poAccessExec->GetpExecutor()->Stack.Push(vParamValue);
		if (ret != M4_SUCCESS)
		{
			SETCODEF(M4_PUSH_STACK_ERROR, ERRORLOG, "#*S1*#");
			return M4_ERROR;
		}
		
		ret = poNodeJGExe->RecordSet.Current.Item["ADD_OUTPUT_PARAM"].Call();
		if (ret != M4_SUCCESS)
		{
			SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#C4_JS_TASK_EXE#", "ADD_OUTPUT_PARAM");
			return M4_ERROR;
		}

		// Se recupera el valor que retorna del m�todo LN4 ejecutado.
		ret = m_poAccessExec->GetpExecutor()->Stack.Pop(vResult);
		if (ret != M4_SUCCESS)
		{
			SETCODEF(M4_POP_STACK_ERROR, ERRORLOG, "#*S1*#%s#%s#", "ADD_OUTPUT_PARAM", "C4_JS_TASK_EXE");
			SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER, ERRORLOG, "#*S1*#");
			return M4_ERROR;
		}
		
		// Si el m�todo ha devuelto 1, es un warning avisando
		// que el par�metro est� ya en base de datos. No interesa
		// recoger esta informaci�n, s�lo si ha habido un error.
		if (vResult.Data.DoubleData == COND_ERROR)
		{
			SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#C4_JS_TASK_EXE#", "ADD_OUTPUT_PARAM");
			return M4_ERROR;
		}

		// Se repiten los pasos anteriores para el 
		// siguiente par�metro de salida.
		poParameter = pTarea->GetNextOutputParameter();
	}

	return M4_SUCCESS;

  //## end ClJSParametersHandlers::RecoverCompoundJobParams%958997022.body
}

// Additional Declarations
  //## begin ClJSParametersHandlers%37E738CE01F0.declarations preserve=yes
  //## end ClJSParametersHandlers%37E738CE01F0.declarations

//## begin module%37E7419D02C5.epilog preserve=yes
//## end module%37E7419D02C5.epilog

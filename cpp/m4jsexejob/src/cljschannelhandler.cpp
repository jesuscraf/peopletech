//## begin module%380F4A1202FF.cm preserve=no
//## end module%380F4A1202FF.cm

//## begin module%380F4A1202FF.cp preserve=no
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
//## end module%380F4A1202FF.cp

//## Module: ClJSChannelHandler%380F4A1202FF; Package body
//## Subsystem: M4JSExeJob::src%37F8BA6602AA
//## Source file: D:\Fuentes\m4jsexejob\src\cljschannelhandler.cpp

//## begin module%380F4A1202FF.additionalIncludes preserve=no
//## end module%380F4A1202FF.additionalIncludes

//## begin module%380F4A1202FF.includes preserve=yes
#include <m4jsres.hpp>
#include <m4jsdefinechannel.hpp>
#include <cllgadap.hpp>

//access
#include <access.hpp>
//Channel
#include <channel.hpp>
//exvariables
#include <exvariables.hpp>
#include "clm4objservice.hpp"
#include "clfactm4objservice.hpp"
#include "iexecutor.hpp"
#include "conn_provider.hpp"
#include "nodedef.hpp"
//## end module%380F4A1202FF.includes

// boolcondition
#include <boolcondition.hpp>
// ClJSChannelHandler
#include <cljschannelhandler.hpp>

class ClNode;
class ClChannelManager;

//## begin module%380F4A1202FF.declarations preserve=no
//## end module%380F4A1202FF.declarations

//## begin module%380F4A1202FF.additionalDeclarations preserve=yes
//## end module%380F4A1202FF.additionalDeclarations


// Class ClJSChannelHandler 








ClJSChannelHandler::ClJSChannelHandler ()
  //## begin ClJSChannelHandler::ClJSChannelHandler%940526264.hasinit preserve=no
  //## end ClJSChannelHandler::ClJSChannelHandler%940526264.hasinit
  //## begin ClJSChannelHandler::ClJSChannelHandler%940526264.initialization preserve=yes
  //## end ClJSChannelHandler::ClJSChannelHandler%940526264.initialization
{
  //## begin ClJSChannelHandler::ClJSChannelHandler%940526264.body preserve=yes
	m_uiTimeOut = 0;
	m_poChannelManager=NULL;
	m_poStatusTimeOut=NULL;
	m_oBCStatus.Init();
	m_poObjService=0;
  //## end ClJSChannelHandler::ClJSChannelHandler%940526264.body
}


ClJSChannelHandler::~ClJSChannelHandler ()
{
  //## begin ClJSChannelHandler::~ClJSChannelHandler%940526265.body preserve=yes

	DestroyEnvToChannels();

  //## end ClJSChannelHandler::~ClJSChannelHandler%940526265.body
}



//## Other Operations (implementation)
m4return_t ClJSChannelHandler::BuildEnvToChannels (ClM4ObjService*& aio_pM4ObjService, ClCache *ai_pCacheCMCR, ClCache *ai_pCacheCSCR, ClCache *ai_pCachePres, ClCache *ai_pMapCache, ClCache *ai_pCacheData)
{
  //## begin ClJSChannelHandler::BuildEnvToChannels%940526266.body preserve=yes

			m4return_t ret;


	//Creamos el channel manager
	aio_pM4ObjService = m_poObjService = ClFactM4ObjService::CreateObjService(ai_pCacheCMCR, ai_pCacheCSCR, ai_pCachePres, ai_pMapCache, ai_pCacheData, M4_TRUE);
	if (!m_poObjService) {
		SETCODEF(M4_POINTER_NULL,ERRORLOG,"#*s1*m_poObjService");
		return M4_ERROR;
	}

	m_poChannelManager = m_poObjService->GetpChannelManager();

	 ret=m_poChannelManager->GetLogonAdaptor()->InsertUserData("M4ADM", 2 ,0, NULL, NULL, NULL, NULL, NULL, NULL);

	 if (M4_SUCCESS != ret)
	 {
		 SETCODEF(M4_ERROR_CHANGE_SECURITY,ERRORLOG,"#*S1*#");
		
		 m_poChannelManager = 0;

		 ClFactM4ObjService::DestroyObjService(m_poObjService);

		 m_poObjService=NULL;

		 return M4_ERROR;
	 }


	return M4_SUCCESS;


  //## end ClJSChannelHandler::BuildEnvToChannels%940526266.body
}

m4return_t ClJSChannelHandler::LoadChannel (m4pcchar_t ai_pccNodeRoot, ClChannel *ai_poChannel, ClAccess *ai_poAccess, ClNode * &ao_poNode,  ClListParameterVariant &ai_oParameterFilter)
{
  //## begin ClJSChannelHandler::LoadChannel%940526268.body preserve=yes

		ClListParameterVariant::iterator oIteratorVariant;
		m4return_t ret;
		m4bool_t Stop=M4_FALSE;
		m4VariantType *arg;
		m4VariantType arg1;

	//Obtenemos el nombre del canal que queremos cargar
	//para poder realizar correctamente la gestión de errores
	 m4pcchar_t pcChannel=ai_poChannel->GetpChannelDef()->Id();

	if (pcChannel == NULL)
	{
		SETCODEF(M4_ERROR_GET_NAME_META4OBJECT,ERRORLOG,"#*S1*#");

		return M4_ERROR;
	}

	//Accedemos al nodo raíz del canal que queremos cargar
	ao_poNode=&(ai_poAccess->Node[ai_pccNodeRoot]);

	if(NULL == ao_poNode)
	{
		SETCODEF(M4_ERROR_CREATE_NODE,ERRORLOG,"#*S1*#%s#%s#",ai_pccNodeRoot,pcChannel);

		return M4_ERROR;
	}

	//Establecemos el filtro por el que vamos a cargar

	//En la lista nos vienen los parámetros por los que vamos a filtrar
	oIteratorVariant=ai_oParameterFilter.begin();

	if (oIteratorVariant != ai_oParameterFilter.end() && Stop==M4_FALSE)
	{
			arg=*oIteratorVariant;

			if (M4CL_CPP_TYPE_NULL == arg->Type)
			{
				Stop=M4_TRUE;
			}
			else
			{
				
				arg1.Type=arg->Type;
				arg1.Data=arg->Data;

				ret=SetData(ao_poNode,M4_FILTER_ID_BOX,arg1);

				if (M4_SUCCESS != ret)
				{
					Stop=M4_TRUE;
				}

			}

			oIteratorVariant++;

			if (oIteratorVariant != ai_oParameterFilter.end() && Stop == M4_FALSE)
			{
				arg=*oIteratorVariant;

				if (M4CL_CPP_TYPE_NULL == arg->Type)
				{
					Stop = M4_TRUE;
				}
				else
				{
					arg1.Type=arg->Type;
					arg1.Data=arg->Data;

					ret=SetData(ao_poNode,M4_FILTER_ID_JOB,arg1);

					if (M4_SUCCESS != ret)
					{
						Stop=M4_TRUE;
					}
				}
			}
	}
	
	if (Stop ==M4_TRUE)
	{
		SETCODEF(M4_LOAD_CHANNEL_ERROR,ERRORLOG,"#*S1*#%s#",pcChannel);

		return M4_ERROR;
	}

	//Establecemos la carga en modo autload
	m4VariantType arg_autoload;

	arg_autoload.Type=M4CL_CPP_TYPE_NUMBER;
	arg_autoload.Data.DoubleData=1.0;

	ret=ai_poChannel->WantsAutoLoad.Set(arg_autoload);

	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_LOAD_CHANNEL_ERROR,ERRORLOG,"#*S1*#%s#",pcChannel);
		 
		return M4_ERROR;
	}

	
	//Nos situamos al comienzo del nodo
	ao_poNode->RecordSet.Current.Begin();

	return M4_SUCCESS;

		
		
  //## end ClJSChannelHandler::LoadChannel%940526268.body
}

m4return_t ClJSChannelHandler::ReleaseChannel (ClChannel *ai_poChannel)
{
  //## begin ClJSChannelHandler::ReleaseChannel%940526272.body preserve=yes

	//Obtenemos el nombre del canal que queremos vaciar;
	
	m4pcchar_t pcChannel=ai_poChannel->GetpChannelDef()->Id();

	if (NULL == pcChannel)
	{
		SETCODEF(M4_ERROR_GET_NAME_META4OBJECT,ERRORLOG,"#*S1*#%s#",pcChannel);
	}
	

	//Vaciamos el contenido de canal 
	//cuyo puntero nos pasan por parámetro
	m4return_t ret=ai_poChannel->ReleaseAll();

	if (M4_ERROR == ret)
	{
		if (pcChannel!= NULL)
		{
			SETCODEF(M4_ERROR_RELEASE_META4OBJECT,ERRORLOG,"#*s1*#%s#",pcChannel);
		}
		else
		{
			SETCODEF(M4_ERROR_RELEASE_META4OBJECT,ERRORLOG,"#*s1*#");
		}
		return M4_ERROR;
	}

	return M4_SUCCESS;

  //## end ClJSChannelHandler::ReleaseChannel%940526272.body
}

m4return_t ClJSChannelHandler::ExecuteMethod (m4pcchar_t ai_pccMethod, ClListParameterVariant ai_oParameterMethod, ClNode *ai_poNode, ClAccess *ai_poAccess, ClListParameterVariant &ao_oListParameterOut, m4uint32_t ai_uiNumParamRef)
{
  //## begin ClJSChannelHandler::ExecuteMethod%940526270.body preserve=yes

		ClListParameterVariant::iterator oIteratorVariant;
		m4return_t ret;
		m4bool_t Stop=M4_FALSE;
		m4VariantType *arg;
		m4VariantType arg1;
		m4uint32_t uiNumParameter=0;

	//Obtenemos el nombre del canal que queremos cargar
	//para poder realizar correctamente la gestión de errores
	ClChannel *poChannel=ai_poAccess->GetpChannel();

	if (NULL == poChannel)
	{
		SETCODEF(M4_ERROR_CHANNEL_NULL,ERRORLOG,"#*S1*#");

		return M4_ERROR;
	}

	m4pcchar_t pcChannel=poChannel->GetpChannelDef()->Id();

	if (pcChannel == NULL)
	{
		SETCODEF(M4_ERROR_GET_NAME_META4OBJECT,ERRORLOG,"#*S1*#");

		return M4_ERROR;
	}

	//Establecemos el filtro por el que vamos a cargar
	oIteratorVariant=ai_oParameterMethod.begin();

	while (oIteratorVariant != ai_oParameterMethod.end() && Stop==M4_FALSE)
	{
			arg=*oIteratorVariant;

			if (M4CL_CPP_TYPE_NULL == arg->Type)
			{
				Stop=M4_TRUE;
			}
			else
			{
				arg1.Type=arg->Type;
				arg1.Data=arg->Data;

				ret=PushStackMVC(ai_poAccess,arg1);

				if (M4_SUCCESS != ret)
				{
					Stop=M4_TRUE;
				}

				uiNumParameter++;

			}

			oIteratorVariant++;
	}
	
	if (Stop ==M4_TRUE)
	{
		SETCODEF(M4_EXECUTE_METHOD_ERROR,ERRORLOG,"#*S1*#%s#%s#",ai_pccMethod,pcChannel);

		return M4_ERROR;
	}


	m4return_t retExecution=ai_poNode->RecordSet.Current.Item[ai_pccMethod].Call(0,uiNumParameter);

	m4bool_t AnyError =M4_FALSE;
	
	if (M4_ERROR == retExecution)
	{
		SETCODEF(M4_EXECUTE_METHOD_ERROR,ERRORLOG,"#*S1*#%s#%s#",ai_pccMethod,pcChannel);

		AnyError=M4_TRUE;
	}


		
	//Si el método tiene parámetros por referencia
	//tenemos que recuperar su valor
	while (ai_uiNumParamRef>0)
	{

		ret=PopStackMVC(ai_poAccess,arg1);
		
		if (M4_SUCCESS != ret)
		{
			SETCODEF(M4_EXECUTE_METHOD_ERROR,ERRORLOG,"#*S1*#%s#%s#",ai_pccMethod,pcChannel);

			AnyError=M4_TRUE;
		}

		arg=new m4VariantType;

		arg->Type=arg1.Type;
		arg->Data=arg1.Data;

		ao_oListParameterOut.push_back(arg);

		ai_uiNumParamRef--;
	}

	//Por último e independiente de si tenemos 
	//parámetros por referencia o no recuperamos el 
	//valor de retorno del método
	ret=PopStackMVC(ai_poAccess ,arg1);

	arg=new m4VariantType;

	arg->Type=arg1.Type;
	arg->Data=arg1.Data;

	//Almacenamos el valor en la lista de parámetros de salida
	ao_oListParameterOut.push_back(arg);
	
	if ( AnyError==M4_TRUE)
	{
		SETCODEF(M4_EXECUTE_METHOD_ERROR,ERRORLOG,"#*S1*#%s#%s#",ai_pccMethod,pcChannel);

		return M4_ERROR;
	}
		
	return retExecution ;

  //## end ClJSChannelHandler::ExecuteMethod%940526270.body
}

/* Bug 0275388
Los punteros deben ser por referencia para que se pongan a nulo
*/
m4return_t ClJSChannelHandler::DestroyChannel (ClAccess* &aio_poAccess, ClChannel* &aio_poChannel)
{
  //## begin ClJSChannelHandler::DestroyChannel%940526273.body preserve=yes

		m4return_t ret;
		m4bool_t AreError=M4_FALSE;

	//Recuperammos el nombre del canal que queremos 
	//destruir por motivo de gestión de errores
	m4pcchar_t pcChannel=aio_poChannel->GetpChannelDef()->Id();
	
	if (NULL == pcChannel)
	{
		SETCODEF(M4_ERROR_GET_NAME_META4OBJECT,ERRORLOG,"#*S1*#");

	}

	//destruimos primero el acceso al canal
	ret=aio_poAccess->Destroy();

	if (M4_SUCCESS != ret)
	{
		if (NULL != pcChannel)
		{
			SETCODEF(M4_DESTROY_ACCESS_ERROR,ERRORLOG,"#*s1*#%s#",pcChannel);

		}
		else
		{	

			SETCODEF(M4_DESTROY_ACCESS_ERROR,ERRORLOG,"#*s1*#");
		}

		AreError = M4_TRUE;
	}

	aio_poAccess=NULL;

	//Destruimos el propio canal
	ret=aio_poChannel->Destroy();
	

	if (M4_SUCCESS != ret)
	{
		if (NULL != pcChannel)
		{
			SETCODEF(M4_DESTROY_CHANNEL_ERROR,ERRORLOG,"#*s1*#%s#",pcChannel);

		}
		else
		{	

			SETCODEF(M4_DESTROY_CHANNEL_ERROR,ERRORLOG,"#*s1*#");
		}

		AreError = M4_TRUE;
	}
			

	aio_poChannel=NULL;

	//Liberemos la conexión lógica
	SetConnection();

	if (AreError == M4_TRUE)
	{
		return M4_ERROR;
	}
	
	
	return M4_SUCCESS;

  //## end ClJSChannelHandler::DestroyChannel%940526273.body
}

m4return_t ClJSChannelHandler::DestroyEnvToChannels ()
{
  //## begin ClJSChannelHandler::DestroyEnvToChannels%940526274.body preserve=yes

	m4return_t ret = M4_SUCCESS;

 
	if (m_poObjService) {
		ret = ClFactM4ObjService::DestroyObjService(m_poObjService);
		m_poObjService=NULL;
	}

   if (m_poStatusTimeOut != NULL)
   {
	   delete m_poStatusTimeOut;

	   m_poStatusTimeOut=NULL;
   }

   m_poChannelManager=NULL;

   return ret;

  //## end ClJSChannelHandler::DestroyEnvToChannels%940526274.body
}

m4return_t ClJSChannelHandler::PersistChannel (ClChannel *ai_poChannel)
{
  //## begin ClJSChannelHandler::PersistChannel%940526275.body preserve=yes

	//Grabamos los datos almacenados en el canal
	m4return_t ret=ai_poChannel->Persist();

	if (M4_SUCCESS != ret)
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;

  //## end ClJSChannelHandler::PersistChannel%940526275.body
}

m4return_t ClJSChannelHandler::GetObjService (ClM4ObjService*& aio_pObjService)
{
  //## begin ClJSChannelHandler::GetObjService%940526276.body preserve=yes

	aio_pObjService=m_poObjService;

	return M4_SUCCESS;

  //## end ClJSChannelHandler::GetObjService%940526276.body
}

ClChannelManager* ClJSChannelHandler::GetChannelManager ()
{
  //## begin ClJSChannelHandler::GetChannelManager%940526278.body preserve=yes

	return m_poChannelManager;

  //## end ClJSChannelHandler::GetChannelManager%940526278.body
}

m4return_t ClJSChannelHandler::ResetMVC (m4bool_t &ao_bIsCancel, m4bool_t &ao_bIsTimeOut)
{
  //## begin ClJSChannelHandler::ResetMVC%940526280.body preserve=yes

	
			m4return_t ret=M4_SUCCESS;
		
		if (NULL != m_poStatusTimeOut)
		{

			//Este método nos devuelve un valor booleano
			//según se haya o no consumido el tiempo máximo
			//de ejecución
			ao_bIsTimeOut=m_poStatusTimeOut->HasTimedOut();

			
			//Quitamos el time out de la mvc
			m_poStatusTimeOut->ResetTimeOut();
			m_uiTimeOut = 0;
		}
		else
		{
			ao_bIsTimeOut=M4_FALSE;
		}
		

		//Reseteamos la máquina virtual para limpiarla 
		//del flag de interrupción que se pone si se pide
		//una cancelación o expira el tiempo máximo permitido 
		//a una ejecución

		//limpiamos el flag de la máquina virtual
		ao_bIsCancel=m_poObjService->GetpExecutor()->GetRequestToAbortWithClStatus();
	
		m_poObjService->GetpExecutor()->RequestToAbortWithClStatus(M4_FALSE);
		
		if (NULL != m_poStatusTimeOut) {
			m_poObjService->GetpExecutor()->SetpCancelStatus(NULL);
			delete m_poStatusTimeOut;
			m_poStatusTimeOut = NULL;
		}
		
		/* Bug 0262489
		Hay que resetear el pool de cadenas de MVC
		*/
		m_poObjService->GetpExecutor()->Reset();

		return M4_SUCCESS;


  //## end ClJSChannelHandler::ResetMVC%940526280.body
}

m4return_t ClJSChannelHandler::GetData (ClNode *ai_poNode, m4pcchar_t ai_pccData, m4VariantType &ao_oData, const m4VariantType *ai_pvDefault )
{
  //## begin ClJSChannelHandler::GetData%940526282.body preserve=yes

	//Obtenemos el nombre del nodo 
	//del que vamos a obtener información
		m4pcchar_t pcChannel=NULL;
		
	m4pcchar_t pcNode=ai_poNode->GetpNodeDef()->Id();

	if (NULL == pcNode)
	{
		SETCODEF(M4_ERROR_GET_NAME_NODE,ERRORLOG,"#*S1*#");
	}

	//Obtenemos el acceso por el que obtuvimos el nodo
	ClAccess *poAccess=ai_poNode->GetpAccess();

	if (NULL == poAccess)
	{
		if (pcNode != NULL )
		{
			SETCODEF(M4_ERROR_ACCESS_NULL,ERRORLOG,"#*S1*#%s#",pcNode);
		}
		else
		{
			SETCODEF(M4_ERROR_ACCESS_NULL,ERRORLOG,"#*S1*#");
		}
	}

	if (poAccess != NULL)
	{
		ClChannel *poChannel=poAccess->GetpChannel();

		if (NULL == poChannel)
		{
			SETCODEF(M4_ERROR_CHANNEL_NULL,ERRORLOG,"#*S1*#");
		}
		else
		{
			pcChannel=poChannel->GetpChannelDef()->Id();
		}
	}

	
 
	//Obtenemos el dato almacenado en la base de datos
	//en el campo que nos indican
	m4return_t ret=ai_poNode->RecordSet.Current.Item[ai_pccData].Value.Get(ao_oData);

	if (M4_SUCCESS != ret)
	{
		if (pcNode != NULL)
		
		{
			if (pcChannel != NULL)
			{
				SETCODEF(M4_GET_VALUE_ERROR,ERRORLOG,"#*S1*#%s#%s#%s#",ai_pccData,pcNode,pcChannel);
			}
			else
			{
				SETCODEF(M4_GET_VALUE_ERROR,ERRORLOG,"#*S1*#%s#%s#",ai_pccData,pcNode);
			}
		}
		else
		{
			SETCODEF(M4_GET_VALUE_ERROR,ERRORLOG,"#*S1*#%s#",ai_pccData);
		}

		return M4_ERROR;
	}


	//Si el valor que obtenemos es nulo entonces no devolvemos un error
	//sino un warning

	if (ao_oData.Type == M4CL_CPP_TYPE_NULL)
	{
		// Si nos pasan un valor por defecto lo asignamos y no damos warning, hombre
		if( ai_pvDefault != NULL )
		{
			ao_oData = *ai_pvDefault ;
		}
		else
		{
			
			if (pcNode != NULL)
			
			{
				if (pcChannel != NULL)
				{
					SETCODEF(M4_GET_NULL_VALUE,WARNINGLOG,"#*S1*#%s#%s#%s#",ai_pccData,pcNode,pcChannel);
				}
				else
				{
					SETCODEF(M4_GET_NULL_VALUE,WARNINGLOG,"#*S1*#%s#%s#",ai_pccData,pcNode);
				}
			}
			else
			{
				SETCODEF(M4_GET_NULL_VALUE,WARNINGLOG,"#*S1*#%s#",ai_pccData);
			}

			return M4_WARNING;
		}
	}

	return M4_SUCCESS;

				


  //## end ClJSChannelHandler::GetData%940526282.body
}

m4return_t ClJSChannelHandler::SetData (ClNode *ai_poNode, m4pcchar_t ai_pccData, const m4VariantType &ai_roData)
{
  //## begin ClJSChannelHandler::SetData%940526283.body preserve=yes
	
	///Obtenemos el nombre del nodo 
	//del que vamos a obtener información
		m4pcchar_t pcChannel=NULL;
		
	m4pcchar_t pcNode=ai_poNode->GetpNodeDef()->Id();

	if (NULL == pcNode)
	{
		SETCODEF(M4_ERROR_GET_NAME_NODE,ERRORLOG,"#*S1*#");
	}

	//Obtenemos el acceso por el que obtuvimos el nodo
	ClAccess *poAccess=ai_poNode->GetpAccess();

	if (NULL == poAccess)
	{
		if (pcNode != NULL )
		{
			SETCODEF(M4_ERROR_ACCESS_NULL,ERRORLOG,"#*S1*#%s#",pcNode);
		}
		else
		{
			SETCODEF(M4_ERROR_ACCESS_NULL,ERRORLOG,"#*S1*#");
		}
	}

	if (poAccess != NULL)
	{
		ClChannel *poChannel=poAccess->GetpChannel();

		if (NULL == poChannel)
		{
			SETCODEF(M4_ERROR_CHANNEL_NULL,ERRORLOG,"#*S1*#");
		}
		else
		{
			pcChannel=poChannel->GetpChannelDef()->Id();
		}
	}

	
	//Obtenemos el dato almacenado en la base de datos
	//en el campo que nos indica

	m4return_t ret=ai_poNode->RecordSet.Current.Item[ai_pccData].Value.Set(ai_roData);

	if (M4_SUCCESS != ret)
	{
		if (pcNode != NULL)
		
		{
			if (pcChannel != NULL)
			{
				SETCODEF(M4_SET_VALUE_ERROR,ERRORLOG,"#*S1*#%s#%s#%s#",ai_pccData,pcNode,pcChannel);
			}
			else
			{
				SETCODEF(M4_SET_VALUE_ERROR,ERRORLOG,"#*S1*#%s#%s#",ai_pccData,pcNode);
			}
		}
		else
		{
			SETCODEF(M4_SET_VALUE_ERROR,ERRORLOG,"#*S1*#%s#",ai_pccData);
		}

		return M4_ERROR;
	}


	return M4_SUCCESS;



  //## end ClJSChannelHandler::SetData%940526283.body
}

m4return_t ClJSChannelHandler::PushStackMVC (ClAccess *ai_poAccess, m4VariantType ai_poData)
{
  //## begin ClJSChannelHandler::PushStackMVC%940526284.body preserve=yes

	//Almacenamos el valor en la pila de meta4objects
	m4return_t ret=ai_poAccess->GetpExecutor()->Stack.Push(ai_poData);

	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_PUSH_STACK_ERROR,ERRORLOG,"#*S1*#");

		return M4_ERROR;
	}

	return M4_SUCCESS;

  //## end ClJSChannelHandler::PushStackMVC%940526284.body
}

m4return_t ClJSChannelHandler::PopStackMVC (ClAccess *ai_poAccess, m4VariantType &ao_oParam)
{
  //## begin ClJSChannelHandler::PopStackMVC%940526285.body preserve=yes

	//Obtenemos un valor de la pila de canales
	m4return_t ret=ai_poAccess->GetpExecutor()->Stack.Pop(ao_oParam);

	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_POP_STACK_ERROR,ERRORLOG,"#*S1*#");

		return M4_ERROR;
	}

	return M4_SUCCESS;


  //## end ClJSChannelHandler::PopStackMVC%940526285.body
}

m4return_t ClJSChannelHandler::SetTimeOutInMVC (m4uint32_t ai_uiTimeOut)
{
  //## begin ClJSChannelHandler::SetTimeOutInMVC%940526281.body preserve=yes

		m_uiTimeOut = ai_uiTimeOut;

		return M4_SUCCESS;


  //## end ClJSChannelHandler::SetTimeOutInMVC%940526281.body
}

m4return_t ClJSChannelHandler::GetNode (m4pcchar_t ai_pccNode, ClAccess *ai_poAccess, ClNode* &ao_poNode)
{
  //## begin ClJSChannelHandler::GetNode%940526269.body preserve=yes

	//Obtenemos el nodo al canal que nos pasan
	ao_poNode=&(ai_poAccess->Node[ai_pccNode]);

	//Obtenemos el nombre del canal que queremos cargar
	//para poder realizar correctamente la gestión de errores
	ClChannel *poChannel=ai_poAccess->GetpChannel();

	if (NULL== poChannel)
	{
		SETCODEF(M4_ERROR_CHANNEL_NULL,ERRORLOG,"#*S1*#");

		return M4_ERROR;
	}

	m4pcchar_t pcChannel=poChannel->GetpChannelDef()->Id();

	if (pcChannel == NULL)
	{
		SETCODEF(M4_ERROR_GET_NAME_META4OBJECT,ERRORLOG,"#*S1*#");

		return M4_ERROR;
	}

	if(NULL == ao_poNode)
	{
		SETCODEF(M4_ERROR_CREATE_NODE,ERRORLOG,"#*S1*#%s#%s#",ai_pccNode,pcChannel);

		return M4_ERROR;
	}

	//Nos situamos al comienzo del nodo
	ao_poNode->RecordSet.Current.Begin();


	return M4_SUCCESS;

  //## end ClJSChannelHandler::GetNode%940526269.body
}

m4return_t ClJSChannelHandler::ReloadChannelExecution (ClChannel *ai_poChannel, ClAccess *ai_poAccess, m4pcchar_t ai_pccNodeRoot, ClNode *ao_poNode, m4pcchar_t ai_pccNodeChild, ClListParameterVariant ai_oParameterToFilter, ClNode *ao_poNodeChild)
{
  //## begin ClJSChannelHandler::ReloadChannelExecution%940526271.body preserve=yes

	m4return_t ret;
	m4VariantType argfilter;
	m4VariantType *poarg;

	//VACIAMOS EL CANAL DE LOS DATOS QUE HABIAMOS OBTENIDO
	ret=ReleaseChannel(ai_poChannel);

	if (M4_ERROR == ret)
	{
		SETCODEF(M4_ERROR_RELEASE_META4OBJECT, ERRORLOG, "#*s1*#C4_JS_TASK_EXE#");
		return M4_ERROR;
	}

	//Accedemos al nodo raíz del canal
	ret=GetNode(ai_pccNodeRoot,ai_poAccess,ao_poNode);
	
	if (M4_SUCCESS != ret)
	{
		return M4_ERROR;
	}
	
	//Llamamos al método que carga el canal
	ClListParameterVariant oListParameterOut;	
	ret = ExecuteMethod("LOAD_TASK_EXE", ai_oParameterToFilter, ao_poNode, ai_poAccess, oListParameterOut);

	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_LOAD_CHANNEL_ERROR, ERRORLOG, "#*S1*#C4_JS_TASKS_EXE#");
		return M4_ERROR;
	}

	ClListParameterVariant::iterator  oIteratorParameter;
    oIteratorParameter=oListParameterOut.begin();

	if (oIteratorParameter != oListParameterOut.end())
	{
		poarg=*oIteratorParameter;
	}

	if (-1 == poarg->Data.DoubleData)
	{
		SETCODEF(M4_LOAD_CHANNEL_ERROR, ERRORLOG, "#*S1*#C4_JS_TASKS_EXE#");
		delete poarg;

		return M4_ERROR;
	}

	delete poarg;

	oListParameterOut.clear();

	//Accedemos de nuevo el nodo hijo
	ret=GetNode(ai_pccNodeChild,ai_poAccess,ao_poNode);

	if (M4_SUCCESS!=ret)
	{
		SETCODEF(M4_LOAD_CHANNEL_ERROR, ERRORLOG, "#*S1*#C4_JS_TASK_EXE#");
		return M4_ERROR;
	}
	
	//Nos situamos en la primera posición del nodo de trabajos
	ao_poNode->RecordSet.Current.Begin();

	//Nos situamos en la primera posición del nodo  de tareas
	ao_poNodeChild->RecordSet.Current.Begin();

	return M4_SUCCESS;

  //## end ClJSChannelHandler::ReloadChannelExecution%940526271.body
}

m4return_t ClJSChannelHandler::CreateNewData (ClNode * &ai_poNode)
{
  //## begin ClJSChannelHandler::CreateNewData%941010642.body preserve=yes

	m4return_t ret=ai_poNode->RecordSet.Current.Add();

	 return ret;

  //## end ClJSChannelHandler::CreateNewData%941010642.body
}

m4bool_t ClJSChannelHandler::IsEndedNode (ClNode *ai_poNode)
{
  //## begin ClJSChannelHandler::IsEndedNode%941096540.body preserve=yes

	// return (ai_poNode->RecordSet.Current.Index.IsEOF());
	return (ai_poNode->RecordSet.Current.Index == M4DM_EOF_INDEX);

  //## end ClJSChannelHandler::IsEndedNode%941096540.body
}

m4return_t ClJSChannelHandler::GetNextData (ClNode * &ai_poNode)
{
  //## begin ClJSChannelHandler::GetNextData%941096541.body preserve=yes

	return (ai_poNode->RecordSet.Current.Next());

  //## end ClJSChannelHandler::GetNextData%941096541.body
}

m4return_t ClJSChannelHandler::ExecuteCodeLN4 (ClNode *ai_poNode, ClAccess *ai_poAccess, ClListParameterVariant &ao_oListParameter, m4uint32_t ai_uiNumberParameter)
{
  //## begin ClJSChannelHandler::ExecuteCodeLN4%941541646.body preserve=yes
	
	m4return_t ret;
	m4VariantType arg;
	m4VariantType * arg1;

	// Creamos un objeto Status que nos permite establecer un 
	// TIME OUT a la máquina virtual.
	// El status se crea para cada petición para que se resete
	// el estado.
	m_poStatusTimeOut = new ClStatus(0);
	if (m_uiTimeOut != 0)
		m_poStatusTimeOut->SetTimeOut(m_uiTimeOut);
	m_poStatusTimeOut->SetBoolCondIntrVal(&m_oBCStatus);

	// Asignamos el status a la máquina virtual así a través del
	// status podemos establecer el time out del tiempo que le 
	// indicamos es decir el tiempo máximo que puede estar 
	// ejecutando una tarea.
	m_poObjService->GetpExecutor()->SetpCancelStatus(m_poStatusTimeOut);

	// Obtenemos el nombre del canal que queremos cargar
	// para poder realizar correctamente la gestión de errores.
	ClChannel *poChannel = ai_poAccess->GetpChannel();
	if (NULL == poChannel)
	{
		SETCODEF(M4_ERROR_CHANNEL_NULL, ERRORLOG, "#*S1*#");
		return M4_ERROR;
	}

	m4pcchar_t pcChannel = poChannel->GetpChannelDef()->Id();
	if (pcChannel == NULL)
	{
		SETCODEF(M4_ERROR_GET_NAME_META4OBJECT, ERRORLOG, "#*S1*#");
		return M4_ERROR;
	}

	m4return_t retExecution = ai_poNode->RecordSet.Current.Item["EXE_JIT_WITH_PARAMS"].Call(0, ai_uiNumberParameter);

	// Por último e independiente de si tenemos parámetros por 
	// referencia o no recuperamos el valor de retorno del método.
	ret = PopStackMVC(ai_poAccess, arg);

	arg1 = new m4VariantType;
	arg1->Type=arg.Type;
	arg1->Data=arg.Data;

	// Almacenamos el valor en la lista de parámetros de salida.
	ao_oListParameter.push_back(arg1);

	// El error de ejecución del método EXE_JIT_WITH_PARAMS sólo
	// debe ponerse si realmente ha fallado, por error de sintaxis,
	// por ejempli. Si la tarea ha sido abortada o ha habido un 
	// timeout (M4CL_USER_ABORT_ALL_REQUEST), este error no debe 
	// ponerse.
	if (retExecution != M4_SUCCESS && retExecution != M4CL_USER_ABORT_ALL_REQUEST)
	{
		SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#%s#", "EXE_JIT_WITH_PARAMS", pcChannel);
		return M4_ERROR;
	}
		
	return retExecution ;

  //## end ClJSChannelHandler::ExecuteCodeLN4%941541646.body
}

m4return_t ClJSChannelHandler::SetConnection ()
{
  //## begin ClJSChannelHandler::SetConnection%941807672.body preserve=yes

	//Liberamos la conexión lógica
		m4return_t ret=m_poObjService->GetpConnProvider()->SetConnection();


		if (M4_ERROR==ret)
		{
			
			SETCODEF(M4_ERROR_FREE_CONNECTION,ERRORLOG,"#*s1*#");			

			return M4_ERROR;
		}

		return M4_SUCCESS;

  //## end ClJSChannelHandler::SetConnection%941807672.body
}

m4return_t ClJSChannelHandler::AbortMVC ()
{
  //## begin ClJSChannelHandler::AbortMVC%942683108.body preserve=yes

	m_poObjService->GetpExecutor()->RequestToAbortWithClStatus(M4_TRUE);

	return M4_SUCCESS;

  //## end ClJSChannelHandler::AbortMVC%942683108.body
}

m4uint32_t ClJSChannelHandler::GetUsedMemory ()
{
  //## begin ClJSChannelHandler::GetUsedMemory%951385117.body preserve=yes
		if (m_poStatusTimeOut == NULL)
			return -1;

		// Paro la ejecucion
		m4return_t iRet = m_poStatusTimeOut->SetIntrValue(ClStatus::STATUS_INTR_PAUSING);
		m4uint32_t iSize;
		if (iRet == M4_SUCCESS)	{
			// Mido la memoria
			iSize = m_poChannelManager->GetSize();

			// Continuo la ejecucion
			// iRet = m_poStatusTimeOut->SetIntrValue(ClStatus::STATUS_INTR_RESUMING);
		}

		// Sea cual sea el resultado de poner el estado a pausing 
		// hay que reestablecerlo.
		iRet = m_poStatusTimeOut->SetIntrValue(ClStatus::STATUS_INTR_RESUMING);		// Continuo la ejecucion

		if (iRet == M4_SUCCESS)
			return iSize;
		else
			return -1;
  //## end ClJSChannelHandler::GetUsedMemory%951385117.body
}

// Additional Declarations
  //## begin ClJSChannelHandler%380F3439039D.declarations preserve=yes
  //## end ClJSChannelHandler%380F3439039D.declarations

//## begin module%380F4A1202FF.epilog preserve=yes
//## end module%380F4A1202FF.epilog

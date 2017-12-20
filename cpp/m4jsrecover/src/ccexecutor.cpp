//## begin module%36E6252A02D1.cm preserve=no
//## end module%36E6252A02D1.cm

//## begin module%36E6252A02D1.cp preserve=no
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
//## end module%36E6252A02D1.cp

//## Module: CcExecutor%36E6252A02D1; Package body
//## Subsystem: M4JSRecover::src%37F8B6A102CB
//## Source file: D:\Fuentes\m4jsrecover\src\ccexecutor.cpp

//## begin module%36E6252A02D1.additionalIncludes preserve=no
//## end module%36E6252A02D1.additionalIncludes

//## begin module%36E6252A02D1.includes preserve=yes
#include <m4trace.hpp>
#include <dm.hpp> //Forward de ClChannelManager
#include <access.hpp> //Forward de ClAccess
#include <channel.hpp> // Forward de ClChannel
#include <metadatasubsys.hpp> //Solo en el cpp
#include <jsrequest.hpp> //Forward de ClJSRequest
//#include <executor.hpp> //Solo en el cpp
#include <navigatorf.hpp> //Forward en hpp
#include <jsjobbuilderfromm4object.hpp>
#include <statsjssubsystem.hpp> //solo en cpp
#include <m4jsres.hpp>
#include <cllgadap.hpp>
#include "clfactm4objservice.hpp"
#include "clm4objservice.hpp"
#include "conn_provider.hpp"
//## end module%36E6252A02D1.includes

// Executive
#include <executive.hpp>
// m4types
#include <m4types.hpp>
// CcExecutor
#include <ccexecutor.hpp>


//## begin module%36E6252A02D1.declarations preserve=no
//## end module%36E6252A02D1.declarations

//## begin module%36E6252A02D1.additionalDeclarations preserve=yes
//## end module%36E6252A02D1.additionalDeclarations


// Class ClCcExecutor 












ClCcExecutor::ClCcExecutor (ClEngine* ai_pEngine, ClStatistic* ai_pStats)
  //## begin ClCcExecutor::ClCcExecutor%921053001.hasinit preserve=no
  //## end ClCcExecutor::ClCcExecutor%921053001.hasinit
  //## begin ClCcExecutor::ClCcExecutor%921053001.initialization preserve=yes
  :ClExecutive(ai_pEngine,ai_pStats)
  //## end ClCcExecutor::ClCcExecutor%921053001.initialization
{
  //## begin ClCcExecutor::ClCcExecutor%921053001.body preserve=yes

	m_poStatis = ai_pStats;
	m_bIsInitialized = M4_FALSE;
	m_poBuilder = new ClJSJobBuilderFromM4Object();
	m_pRecoveryM4O = NULL;
	m_pRecoveryM4OAccess = NULL;

	if (m_poBuilder == NULL)
	{
		/* Tratamiento del error: no se ha podido crear el builder             */
		/* Este ejecutor no va a valer para mucho ...                          */
		m_bIsValid = M4_FALSE;

      BL_BEGIN( ERRORLOG, M4_JS_REC_BUILDING_INSTANCE);
      BL_OBJ << "ClJSJobBuilderFromM4Object" << "ClCcExecutor::Start";
      BL_END;
	}
	else
		m_bIsValid = M4_TRUE;
  //## end ClCcExecutor::ClCcExecutor%921053001.body
}


ClCcExecutor::~ClCcExecutor ()
{
  //## begin ClCcExecutor::~ClCcExecutor%921053002.body preserve=yes
	if (m_poBuilder != NULL)
		delete m_poBuilder;
  //## end ClCcExecutor::~ClCcExecutor%921053002.body
}



//## Other Operations (implementation)
m4return_t ClCcExecutor::StartUp (ClNavigatorFirewall* ai_pNavigator, ClConfiguration* ai_pConfig)
{
  //## begin ClCcExecutor::StartUp%921053005.body preserve=yes
	ClCache		*poCMCRCache = NULL          ;/* Puntero a caché                */
    ClCache		*poCSCRCache = NULL          ;/* Otro puntero a caché           */
	ClCache		*poPresentationCache = NULL  ;/* Caché de presentaciones        */
	ClCache		*poMapCache = NULL	          ;/* Caché de mapeos                */
	ClCache		*poDataCache = NULL          ;/* Caché de datos                 */ 
	m4return_t	res = M4_SUCCESS            ;/* Resultado del método           */
	M4ClString	stError                     ;/* Parámetro para errores         */
	m4uint32_t	uiNumError = 0              ;/* Código de error                */
	size_t		iLenValue = 0;
	m4char_t	pszValue[512];
	
	//Get MD caches from MetadataSubsystem
	ClSSInterfaz * pMetadataSubsystemInterfaz = (ClSSInterfaz*)ai_pNavigator->GetSubsystemByName(ClMetaDataSubsystem::M4MetaDataSubsystemName);

	if (NULL != pMetadataSubsystemInterfaz)
	{
		poCMCRCache = (ClCache*)pMetadataSubsystemInterfaz->GetElement(ClExecutiveForMDCaches::M4ExecutiveForMDCMCRCache);
		poCSCRCache = (ClCache*)pMetadataSubsystemInterfaz->GetElement(ClExecutiveForMDCaches::M4ExecutiveForMDCSCRCache);
		poPresentationCache = (ClCache*)pMetadataSubsystemInterfaz->GetElement(ClExecutiveForMDCaches::M4ExecutiveForMDPresentationCache);
		poMapCache = (ClCache*)pMetadataSubsystemInterfaz->GetElement(ClExecutiveForMDCaches::M4ExecutiveForMDMapCache);

		// DATACACHE
		res = ai_pConfig->GetStringValue("", ClExecutiveForMDCaches::M4ExecutiveForMDDataCache, pszValue, sizeof(pszValue), iLenValue);
		if (res == M4_SUCCESS)
		{
			if ( ! ( poDataCache = ( ClCache * )ai_pNavigator-> GetSubsystemElement ( pszValue , "CACHE") ) )
			{
				SETCODEF ( M4_SRV_SS_CACHE_ERROR , ERRORLOG , "MetaDataSubsystem. Cannot find cache <%s>, in caches subsystem" , pszValue ) ;
			}
		}

		m_poObjService = ClFactM4ObjService::CreateObjService(poCMCRCache, poCSCRCache, poPresentationCache, poMapCache, poDataCache, M4_TRUE);
		if (m_poObjService) 
		{
			m_pClChannelManager = m_poObjService->GetpChannelManager();
			if (m_pClChannelManager) 
			{
				m_pClChannelManager->GetLogonAdaptor()->InsertUserData( "M4ADM", 2, 0, NULL, NULL, NULL, NULL, NULL, NULL );
				if (InitializeRecoveryM4O() != M4_SUCCESS)
				{
					stError = "ClCcExecutor->InitializeRecoveryM4O";
					uiNumError = M4_JS_REC_EXECUTING_METHOD;
				}
			}
			else
			{
				stError = "m_poObjService->GetpChannelManager";
				uiNumError = M4_JS_REC_EXECUTING_METHOD;
			}
		}
		else
		{
			stError = "ClFactM4ObjService::CreateObjSevice";
			uiNumError = M4_JS_REC_EXECUTING_METHOD;
		}
	}

	if (uiNumError > 0)
	{
		m_bIsValid = M4_FALSE;
      BL_BEGIN( ERRORLOG, uiNumError);
      BL_OBJ << stError << "ClCcExecutor::StartUp";
      BL_END;
      res = M4_ERROR;
   } /* endif */

	return res;
  //## end ClCcExecutor::StartUp%921053005.body
}

m4return_t ClCcExecutor::ShutDown ()
{
  //## begin ClCcExecutor::ShutDown%921053006.body preserve=yes
   M4ClString stError			  ;/* Mensaje de error                         */
	m4uint32_t uiNumError = 0    ;/* Código de error                          */
   m4return_t res = M4_SUCCESS  ;/* Resultado del método                     */

	if (m_pRecoveryM4OAccess != NULL)
		m_pRecoveryM4OAccess->Destroy();

	if (m_pRecoveryM4O != NULL)
		m_pRecoveryM4O->Destroy();

	if (ClFactM4ObjService::DestroyObjService(m_poObjService) == M4_ERROR) {
		stError = "ClFactM4ObjService::DestroyObjService";
		uiNumError = M4_JS_REC_EXECUTING_METHOD;
	}

	if (uiNumError > 0)
	{
      BL_BEGIN( ERRORLOG, uiNumError);
      BL_OBJ << stError << "ClCcExecutor::Shutdown";
      BL_END;
      res = M4_ERROR;
	} /* endif */

	return res;
  //## end ClCcExecutor::ShutDown%921053006.body
}

m4return_t ClCcExecutor::Execute (ClImage& aio_pImage)
{
  //## begin ClCcExecutor::Execute%921053007.body preserve=yes

	return M4_SUCCESS;

  //## end ClCcExecutor::Execute%921053007.body
}

m4return_t ClCcExecutor::Reset ()
{
  //## begin ClCcExecutor::Reset%921053008.body preserve=yes

	return M4_SUCCESS;

  //## end ClCcExecutor::Reset%921053008.body
}

m4return_t ClCcExecutor::ProccessRequest (ClJSRequest *ai_poJSRequest, ClJSQueue &ao_poJSQueue)
{
  //## begin ClCcExecutor::ProccessRequest%937557557.body preserve=yes
	m4return_t ret = M4_SUCCESS ;/* Resultado del método                      */
	m4uint32_t uiRequestType    ;/* Tipo de petición                          */
	m4uint32_t uiNumError = 0   ;/* Código de error a generar                 */
	M4ClString stError          ;/* Parámetros de error                       */

	if ( m_bIsValid == M4_TRUE)
	{
		if (ai_poJSRequest)
		{
			uiRequestType = ai_poJSRequest->GetRequestType();
  			switch (uiRequestType) 
  			{
   			case M4JS_REQ_FIND_CANCEL:
				{
					if ( m_bIsInitialized == M4_TRUE)
						CheckCancel(ao_poJSQueue);
				
  					break;
				}
				case M4JS_REQ_FIND_EXPIRED:
				{
					if ( m_bIsInitialized == M4_TRUE)
   					CheckBoxExpiration();
   				break;
				}
   			case M4JS_REQ_FIND_RES_EXPIRED:
				{
					if ( m_bIsInitialized == M4_TRUE)
					{
			   		M4ClTimeStamp ActualDate;
						m4double_t JulianDate;

	   				ActualDate.now();
						ActualDate.get(JulianDate);
   					CheckResExpiration(JulianDate);
					}
					break;
				}
   			case M4JS_REQ_FIND_JOBS:
				{
					m4uint32_t maxNumJobs;
					m4uint_t uiMaxNumJobs;

					if ( m_bIsInitialized == M4_TRUE)
					{
						/*	Aquí hay que sacar los parámetros de la request (número */
						/* de trabajos a recuperar) y pasarlo                      */
						if ( ai_poJSRequest->GetParameter(0, maxNumJobs) == M4_SUCCESS)
						{
							uiMaxNumJobs = maxNumJobs;
							CheckScheduledJobs( uiMaxNumJobs, ao_poJSQueue);
						}
						else 
						{
							uiNumError = M4_JS_REC_INVALID_INSTANCE;
							stError = "ClJSRequest-M4JS_REQ_FIND_JOBS";
						}
					}
   				break;
				}
				case M4JS_REQ_INITIALIZE:
				{
					m4uint32_t uiFlag;

					if ( ai_poJSRequest->GetParameter( 0, uiFlag) == M4_SUCCESS)
					{
						/* Aquí hay que lanzar el proceso de limpieza de la base   */
						/* de datos                                                */
						if ( CleanDatabase( uiFlag ) == M4_SUCCESS)
						{
							m_bIsInitialized = M4_TRUE;
						}
						else
						{
							uiNumError = M4_JS_REC_EXECUTING_METHOD;
							stError = "ClCcExecutor->CleanDatabase";
						}	
					}
					else
					{
						uiNumError = M4_JS_REC_INVALID_INSTANCE;
						stError = "ClJSRequest-M4JS_REQ_INITIALIZE";
					}
					break;
				}

				case M4JS_REQ_UNMARK_JOB:
				{
					m4pvoid_t pParameter;

					if ( m_bIsInitialized == M4_TRUE)
					{
						/*	Aquí hay que sacar los parámetros de la request        */
						if ( ai_poJSRequest->GetParameter(0,pParameter) == M4_SUCCESS)
						{
							if (pParameter != NULL)
							{
								if ( UnmarkExecution( (ClJSJob *) pParameter) != M4_SUCCESS)
								{
									uiNumError = M4_JS_REC_EXECUTING_METHOD;
									stError = "CcExecutor->UnmarkExecution";
								}

								delete pParameter;
							}
							else
							{
								uiNumError = M4_JS_REC_MISSING_PARAMETER;
								stError = "CcExecutor->ProcessRequest: M4JS_REQ_UMARK_JOB";
							}
						}
						else 
						{
							uiNumError = M4_JS_REC_INVALID_INSTANCE;
							stError = "ClJSRequest-M4JS_REQ_UMARK_JOB";
						}
					}

					break;
				}
		
	   		default:
					uiNumError = M4_JS_REC_INVALID_INSTANCE;
					stError = "ClJSRequest-DEFAULT";

   		}
		}
		else
		{
			uiNumError = M4_JS_REC_MISSING_PARAMETER;
			stError = "ClJsRequest";
		}
	}
	else
	{
		uiNumError = M4_JS_REC_INVALID_INSTANCE;
		stError = "ClCcExecutor";
	}

	if (uiNumError != 0)
	{
      BL_BEGIN( ERRORLOG, uiNumError);
      BL_OBJ << stError << "ClCcExecutor::ProcessRequest";
      BL_END;

	  /*ret = M4_ERROR; 
	  DEL FJSM Si se devuelve error el thread se muere, y eso no es lo que 
	  queremos, ya que no son errores críticos y podemos seguir funcionando.
	  Lo único que hacemos es volcar un error. */
	}

	// Liberamos la conexión para toda la máquina virtual.
	m_poObjService -> GetpConnProvider() -> SetConnection ();

 	return ret;
  //## end ClCcExecutor::ProccessRequest%937557557.body
}

m4return_t ClCcExecutor::LoadChannel (m4pchar_t ai_strChannel)
{
  //## begin ClCcExecutor::LoadChannel%937559766.body preserve=yes
    M4ClString stError	        ;/* Parámetro del mensaje de error           */
	 m4uint32_t uiNumError = 0   ;/* Código de error a generar                */
    m4return_t res = M4_SUCCESS ;/* Resultado del método                     */

    m_pClChannelManager -> CreateChannel (m_pClChannel);

    if (m_pClChannel != 0)
    {
		 m_pClChannel->Role_ID.Set( M4CL_SEC_SUPER_USER ) ;
		 if (m_pClChannel->BuildFromId( ai_strChannel) != M4_ERROR)
       {
			 m_pClAccess = m_pClChannel -> CreateAccess ();
          if ( m_pClAccess == 0 )
          {
				 stError = "ClChannel->CreateAccess";
				 uiNumError = M4_JS_REC_EXECUTING_METHOD;
          }
       } 
       else 
       {
			 stError = "ClChannel->BuildFromId";
			 uiNumError = M4_JS_REC_EXECUTING_METHOD;
       } /* endif */
    }
    else 
    {
		 stError = "ClChannelManager->CreateChannel";
		 uiNumError = M4_JS_REC_EXECUTING_METHOD;
    } 

   if (uiNumError > 0)
   {
      BL_BEGIN( ERRORLOG, uiNumError);
      BL_OBJ << stError << "ClCcExecutor::LoadChannel";
      BL_END;
      res = M4_ERROR;
   } /* endif */

    return res;
  //## end ClCcExecutor::LoadChannel%937559766.body
}

m4return_t ClCcExecutor::UnLoadChannel ()
{
  //## begin ClCcExecutor::UnLoadChannel%937559767.body preserve=yes
	if ( m_pClAccess ) 
	{
		m_pClAccess -> Destroy ();
		m_pClAccess = 0;
	}

	if ( m_pClChannel )
	{
		m_pClChannel -> Destroy ();
		m_pClChannel = 0;
	}
	
	return M4_SUCCESS;
  //## end ClCcExecutor::UnLoadChannel%937559767.body
}

m4return_t ClCcExecutor::CheckCancel (ClJSQueue &ai_oJSQueue)
{
  //## begin ClCcExecutor::CheckCancel%937559768.body preserve=yes
	m4return_t res = M4_SUCCESS ;/* Resultado del método                      */
	M4ClString stError = ""     ;/* Mensajes de error                         */
	m4uint32_t uiNumError = 0   ;/* Código de error a generar                 */
	m4uint_t uiLeidos = 0       ;/* Número de trabajos leídos por el builder  */
	ClJSJob * pUnTrabajo        ;/* Puntero para ir recuperando trabajos      */
	m4uint_t uiContador = 0     ;/* Contador para el bucle                    */

	/* Pedimos al builder que lea trabajos                                    */
	if ( m_poBuilder->ReadCancelledJobs() == M4_SUCCESS )
	{
		m_poBuilder->GetNumberOfProducts( uiLeidos );
		if (uiLeidos > 0)
		{
			/* Bucle que recorre la lista de trabajos                           */
			for ( uiContador = 0; ((uiContador < uiLeidos) & (uiNumError == 0)); uiContador ++)
			{
				/* Sacamos uno de la cola                                        */
				pUnTrabajo = m_poBuilder->GetpProduct();
				if (pUnTrabajo != NULL)
				{
					/* Generamos una petición para el administrador               */
					if (SendCancelRequest( pUnTrabajo, ai_oJSQueue) == M4_ERROR )
					{
						uiNumError = M4_JS_REC_EXECUTING_METHOD;
						stError = "ClCcExecutor::SendCancelRequest";
					}
				}
			}
		}
	}
	else
	{
		uiNumError = M4_JS_REC_EXECUTING_METHOD;
		stError = "JobBuilder->ReadCancelledJobs";
	}

	/* Información de errores                                                 */
   if (uiNumError > 0)
   {
      BL_BEGIN( ERRORLOG, uiNumError);
      BL_OBJ << stError << "ClCcExecutor::CheckCancel";
      BL_END;
      res = M4_ERROR;
   } /* endif */

	return res;

  //## end ClCcExecutor::CheckCancel%937559768.body
}

m4return_t ClCcExecutor::CheckBoxExpiration ()
{
  //## begin ClCcExecutor::CheckBoxExpiration%937559769.body preserve=yes
	m4return_t res = M4_SUCCESS ;/* Resultado del método                      */
	M4ClString stError          ;/* Parámetros de error                       */
	m4uint32_t uiNumError = 0   ;/* Código de error a generar                 */
	m4VariantType arg1          ;/* Resultado de la ejecución del LN4         */
	ClNode * poNode             ;/* Puntero al nodo principal del M4Object    */

	if ( LoadChannel("C4_JS_TASK_EXPIRATION") == M4_SUCCESS)
	{
		/* Pedimos un puntero al nodo principal del M4Object                   */
		poNode = &(m_pClAccess->Node ["N4_JS_TASK_EXPIRATION"]);
		if ( poNode )
		{
			/* Nos posicionamos en el método EXPIRE_BOXES y lo ejecutamos             */
			if ( poNode->RecordSet.Current.Item["EXPIRE_TASK_EXE"].Call () == M4_SUCCESS )
			{
				/* El resultado del LN4 lo sacamos del stack                     */
				if ( poNode->RecordSet.Current.Item["EXPIRE_TASK_EXE"].StackPop (arg1) == M4_SUCCESS)
				{
					m4Trace(cout << "Expiradas " << arg1 << " ejecuciones" << endl);
					((ClStatsJSSubsystem*)m_poStatis)->AddJobsExpired((m4uint32_t)arg1.Data.DoubleData);
				}
				else 
				{
					stError = "C4_JS_TASK_EXPIRATION.EXPIRE_TASK_EXE";
					uiNumError = M4_JS_REC_POPPING_PARAMETERS;
				}
			}
			else
			{
				stError = "C4_JS_TASK_EXPIRATION.EXPIRE_TASK_EXE";
				uiNumError = M4_JS_REC_CALLING_METHOD;
			}
		}
		else
		{
			stError = "C4_JS_TASK_EXPIRATION.N4_JS_TASK_EXPIRATION";
			uiNumError = M4_JS_REC_ACCESSING_NODE;
		}
	}
	else
	{
		stError = "C4_JS_TASK_EXPIRATION";
		uiNumError = M4_JS_REC_BUILDING_M4O;
	}
                                                                             
   /* Descarga del M4Object                                                  */
	UnLoadChannel ();

	/* Información de errores                                                 */
   if (uiNumError > 0)
   {
      BL_BEGIN( ERRORLOG, uiNumError);
      BL_OBJ << stError << "ClCcExecutor::CheckBoxExpiration";
      BL_END;
      res = M4_ERROR;
   } /* endif */

	return res;
  //## end ClCcExecutor::CheckBoxExpiration%937559769.body
}

m4return_t ClCcExecutor::CheckResExpiration (m4double_t JulianDate)
{
  //## begin ClCcExecutor::CheckResExpiration%937559770.body preserve=yes
	m4return_t res = M4_SUCCESS    ;/* Resultado del método                   */
	M4ClString stError             ;/* Parámetros para errores                */
	m4uint32_t uiNumError = 0      ;/* Código de error                        */
	m4VariantType arg              ;/* Parámetro para el LN4                  */
	m4VariantType arg1             ;/* Parámetro para el LN4                  */

	if (LoadChannel ("C4_JS_RES_EXPIRATION") == M4_SUCCESS)
	{
		ClNode *poNode = &(m_pClAccess -> Node ["N4_JS_RES_EXPIRATION"]);
		if ( poNode )
		{
			/* Establecemos el parámetro para el método                         */
			arg.Type = M4CL_CPP_TYPE_DATE;
			arg.Data.DoubleData = JulianDate;

			if (poNode -> RecordSet.Current.Item["EXPIRE_RESOURCES"].StackPush (arg) == M4_SUCCESS) 
			{
				/* Llamando al método LN4 EXPIRE_RESOURCES                            */
				if ( poNode -> RecordSet.Current.Item["EXPIRE_RESOURCES"].Call () == M4_SUCCESS )
				{
					/* Vaciamos el stack                                          */
					if (poNode -> RecordSet.Current.Item["EXPIRE_RESOURCES"].StackPop (arg1) == M4_SUCCESS)
					{
						m4Trace(cout << "Expirados " << arg1 << " recursos" << endl);
						((ClStatsJSSubsystem*)m_poStatis)->AddResExpired((m4uint32_t)arg1.Data.DoubleData);
					}
					else
					{
						stError = "C4_JS_RES_EXPIRATION.EXPIRE_RESOURCES";
						uiNumError = M4_JS_REC_POPPING_PARAMETERS;
					}
				}
				else
				{
					stError = "C4_JS_RES_EXPIRATION.EXPIRE_RESOURCES";
					uiNumError = M4_JS_REC_CALLING_METHOD;
				}
			}
			else
			{
				stError = "C4_JS_RES_EXPIRATION.EXPIRE_RESOURCES";
				uiNumError = M4_JS_REC_PUSHING_PARAMETERS;
			}
		}
		else
		{
			stError = "C4_JS_RES_EXPIRATION.N4_JS_RES_EXPIRATION";
			uiNumError = M4_JS_REC_ACCESSING_NODE;
		}
	}
	else
	{
		stError = "C4_JS_RES_EXPIRATION";
		uiNumError = M4_JS_REC_BUILDING_M4O;
	}

	UnLoadChannel ();

	/* Información de errores                                                 */
   if (uiNumError > 0)
   {
      BL_BEGIN( ERRORLOG, uiNumError);
      BL_OBJ << stError << "ClCcExecutor::CheckBoxExpiration";
      BL_END;
      res = M4_ERROR;
   } /* endif */

	return res;
  //## end ClCcExecutor::CheckResExpiration%937559770.body
}

m4return_t ClCcExecutor::CheckScheduledJobs (m4uint_t &ai_uiMaxNumJobs, ClJSQueue &ai_oJSQueue)
{
  //## begin ClCcExecutor::CheckScheduledJobs%939977799.body preserve=yes
	m4return_t res = M4_SUCCESS ;/* Resultado del método                      */
	M4ClString stError = ""     ;/* Mensajes de error                         */
	m4uint32_t uiNumError = 0   ;/* Código de error a generar                 */
	m4uint_t uiQuedan = 0       ;/* Número de trabajos que quedan pendientes  */
	m4uint_t uiLeidos = 0       ;/* Número de trabajos leídos por el builder  */
	ClJSJob * pUnTrabajo        ;/* Puntero para ir recuperando trabajos      */
	m4uint_t uiContador = 0     ;/* Contador para el bucle                    */

	/* Pedimos al builder que lea trabajos                                    */
	res = m_poBuilder->ReadScheduledJobs(ai_uiMaxNumJobs);
	m_poBuilder->GetNumberOfProducts(uiLeidos);
	if (uiLeidos > 0)
	{
		/* Bucle que recorre la lista de trabajos                           */
		for (uiContador = 0; ((uiContador < uiLeidos) & (uiNumError == 0)); uiContador++)
		{
			/* Sacamos uno de la cola                                        */
			pUnTrabajo = m_poBuilder->GetpProduct();
			if (pUnTrabajo != NULL)
			{
				if (res == M4_SUCCESS)
				{
					/* Generamos una petición para el administrador               */
					if (SendNewJobRequest(pUnTrabajo, ai_oJSQueue) == M4_ERROR)
					{
						uiNumError = M4_JS_REC_EXECUTING_METHOD;
						stError = "ClCcExecutor::SendNewJobRequest";

						/* En caso de que se produzca este error, desmarcamos el   */
						/* trabajo, y lo borramos.                                 */
						/* Si se llega a esta situación, las cosas van bastante    */
						/* mal, y debe haber un problema serio de memoria          */
						UnmarkExecution(pUnTrabajo);
						delete pUnTrabajo;
					}
				}
				else
				{
					// Bug 0291201. Si el método que recupera las tareas pendientes de ser
					// ejecutadas falla, hay que desmarcar la ejecución.
					uiNumError = M4_JS_REC_EXECUTING_METHOD;
					stError = "JobBuilder->ReadScheduledJobs";

					UnmarkExecution(pUnTrabajo);
					delete pUnTrabajo;
				}
			}
		}
	}

	/* La request de fin de trabajos se tiene que mandar siempre              */
	m_poBuilder->GetRemainingJobs( uiQuedan);
	if (SendEndOfJobsRequest( uiQuedan == 0 ? M4_FALSE : M4_TRUE, ai_oJSQueue) == M4_ERROR)
	{
		if (uiNumError == 0)
		{
			uiNumError = M4_JS_REC_EXECUTING_METHOD;
			stError = "ClCcExecutor::SendEndOfJobsRequest";
		}
	}


	/* Información de errores                                                 */
   if (uiNumError > 0)
   {
      BL_BEGIN( ERRORLOG, uiNumError);
      BL_OBJ << stError << "ClCcExecutor::CheckScheduledJobs";
      BL_END;
      res = M4_ERROR;
   } /* endif */

	return res;
  //## end ClCcExecutor::CheckScheduledJobs%939977799.body
}

m4return_t ClCcExecutor::UnmarkExecution (ClJSJob *ai_pJob)
{
  //## begin ClCcExecutor::UnmarkExecution%949062689.body preserve=yes
	m4uint32_t uiGroup           ;/* Identificador del grupo                  */
	m4uint32_t uiExecution       ;/* Identificador de la ejecución            */
	m4return_t res = M4_SUCCESS  ;/* Resultado del método                     */
	M4ClString stError           ;/* Parámetro de error							  */
	m4uint32_t uiNumError = 0    ;/* Código de error                          */
	ClNode * poNode              ;/* Puntero al nodo principal del M4Object   */
	m4VariantType vIdGrupo		  ;/* Parámetro para el LN4: id del grupo      */
	m4VariantType vIdExecution   ;/* Parámetro para el LN4: id de la exec     */
	m4VariantType vRes           ;/* Resultado de las llamadas al LN4         */

	if (m_bIsValid == M4_TRUE)
	{
		/* Pedimos un puntero al nodo principal del M4Object                   */
		poNode = &(m_pRecoveryM4OAccess->Node ["N4_JS_SCHED_DATES"]);
		if ( poNode )
		{
			stError = "C4_JS_SCHED_DATES->UNMARK_EXE";

			ai_pJob->GetSchedJobId( uiGroup);
			ai_pJob->GetExecutionId( uiExecution);			

			vIdGrupo.Type = M4CL_CPP_TYPE_NUMBER;
			vIdGrupo.Data.DoubleData = uiGroup;
			vIdExecution.Type = M4CL_CPP_TYPE_NUMBER;
			vIdExecution.Data.DoubleData = uiExecution;

		 	if (poNode->RecordSet.Current.Item[ "UNMARK_EXE" ].StackPush(vIdGrupo)     == M4_SUCCESS &&
				poNode->RecordSet.Current.Item[ "UNMARK_EXE" ].StackPush(vIdExecution) == M4_SUCCESS )
			{
				if (poNode->RecordSet.Current.Item[ "UNMARK_EXE" ].Call() == M4_SUCCESS )
				{
					if (poNode->RecordSet.Current.Item[ "UNMARK_EXE" ].StackPop(vRes) == M4_SUCCESS )
					{
						if (vRes.Data.DoubleData != 0)
							uiNumError = M4_JS_REC_EXECUTING_METHOD;
					}
					else
						uiNumError = M4_JS_REC_POPPING_PARAMETERS;
				}
				else
					uiNumError = M4_JS_REC_CALLING_METHOD;
			}
			else
				uiNumError = M4_JS_REC_PUSHING_PARAMETERS;
		}
		else 
		{
			stError = "C4_JS_SCHED_DATES->N4_JS_SCHED_DATES";
			uiNumError = M4_JS_REC_ACCESSING_NODE;
		} 
	}

   if (uiNumError > 0)
   {
      BL_BEGIN( ERRORLOG, uiNumError);
      BL_OBJ << stError << "ClCcExecutor::UnmarkExecution";
      BL_END;
      res = M4_ERROR;
   } 

   return res;
  //## end ClCcExecutor::UnmarkExecution%949062689.body
}

m4return_t ClCcExecutor::SendNewJobRequest (ClJSJob *ai_poJob, ClJSQueue &ai_oJSQueue)
{
  //## begin ClCcExecutor::SendNewJobRequest%939977803.body preserve=yes
   ClJSRequest * poRequest       ;/* Request a enviar a la cola              */
   m4return_t res = M4_SUCCESS   ;/* Resultado del m‚todo                    */
   M4ClString stError = ""       ;/* Par metro de los mensajes de error      */
   m4uint32_t uiNumError = 0     ;/* C¢digo de error a generar               */

	poRequest = new ClJSRequest(M4JS_REQ_NEW_JOB);

   if (poRequest)
   {
   	poRequest -> AddParameter( ai_poJob) ;
   	if ( ai_oJSQueue.Push(poRequest) == M4_ERROR)
      {
         stError = "M4JS_REQ_NEW_JOB";
      	uiNumError = M4_JS_REC_CREATING_REQUEST;
      }
   } 
   else 
   {
      stError = "M4JS_REQ_NEW_JOB";
   	uiNumError = M4_JS_REC_PUSHING_REQUEST;
   } /* endif */

   if (uiNumError > 0)
   {
      BL_BEGIN( ERRORLOG, uiNumError);
      BL_OBJ << stError << "ClCcExecutor::SendNewJobsRequest";
      BL_END;
      res = M4_ERROR;
   } /* endif */

   return res;
  //## end ClCcExecutor::SendNewJobRequest%939977803.body
}

m4return_t ClCcExecutor::SendEndOfJobsRequest (const m4bool_t ai_bSetInmediateRequest, ClJSQueue &ai_oJSQueue)
{
  //## begin ClCcExecutor::SendEndOfJobsRequest%939977804.body preserve=yes
   ClJSRequest * poRequest       ;/* Request a enviar a la cola              */
   m4return_t res = M4_SUCCESS   ;/* Resultado del método                    */
   M4ClString stError = ""       ;/* Parámetro de los mensajes de error      */
   m4uint32_t uiNumError = 0     ;/* Código de error a generar               */

	poRequest = new ClJSRequest(M4JS_REQ_END_NEW_JOBS);

   if (poRequest)
   {
   	poRequest -> AddParameter( ai_bSetInmediateRequest == M4_TRUE ? 1 : 0) ;
   	if ( ai_oJSQueue.Push(poRequest) == M4_ERROR)
      {
         stError = "M4JS_REQ_END_NEW_JOBS";
      	uiNumError = M4_JS_REC_CREATING_REQUEST;
      }
   } 
   else 
   {
      stError = "M4JS_REQ_END_NEW_JOBS";
   	uiNumError = M4_JS_REC_PUSHING_REQUEST;
   } /* endif */

   if (uiNumError > 0)
   {
      BL_BEGIN( ERRORLOG, uiNumError);
      BL_OBJ << stError << "ClCcExecutor::SendEndOfJobsRequest";
      BL_END;
      res = M4_ERROR;
   } /* endif */

   return res;
  //## end ClCcExecutor::SendEndOfJobsRequest%939977804.body
}

m4return_t ClCcExecutor::SendCancelRequest (ClJSJob *ai_poJob, ClJSQueue &ai_oJSQueue)
{
  //## begin ClCcExecutor::SendCancelRequest%941016333.body preserve=yes
   ClJSRequest * poRequest       ;/* Request a enviar a la cola              */
   m4return_t res = M4_SUCCESS   ;/* Resultado del método                    */
   M4ClString stError = ""       ;/* Parámetro de los mensajes de error      */
   m4uint32_t uiNumError = 0     ;/* C¢digo de error a generar               */

	poRequest = new ClJSRequest(M4JS_REQ_UNCONTROLED_INTERRUPT);

   if (poRequest)
   {
   	poRequest -> AddParameter( ai_poJob) ;
   	if ( ai_oJSQueue.Push(poRequest) == M4_ERROR)
      {
         stError = "M4JS_REQ_UNCONTROLED_INTERRUPT";
      	uiNumError = M4_JS_REC_CREATING_REQUEST;
      }
   } 
   else 
   {
      stError = "M4JS_REQ_UNCONTROLED_INTERRUPT";
   	uiNumError = M4_JS_REC_PUSHING_REQUEST;
   } /* endif */

   if (uiNumError > 0)
   {
      BL_BEGIN( ERRORLOG, uiNumError);
      BL_OBJ << stError << "ClCcExecutor::SendCancelRequest";
      BL_END;
      res = M4_ERROR;
   } /* endif */

   return res;

  //## end ClCcExecutor::SendCancelRequest%941016333.body
}

m4return_t ClCcExecutor::SetServerName (const M4ClString &ai_stServerName)
{
  //## begin ClCcExecutor::SetServerName%940334696.body preserve=yes
	m_stServerName = ai_stServerName;

	return M4_SUCCESS;
  //## end ClCcExecutor::SetServerName%940334696.body
}

m4return_t ClCcExecutor::SetServiceName (const M4ClString &ai_stServiceName)
{
  //## begin ClCcExecutor::SetServiceName%940334697.body preserve=yes
	m_stServiceName = ai_stServiceName;

	return M4_SUCCESS;
  //## end ClCcExecutor::SetServiceName%940334697.body
}

m4return_t ClCcExecutor::SetIdentification (const M4ClString &ai_stServerName, const M4ClString &ai_stServiceName)
{
  //## begin ClCcExecutor::SetIdentification%940334698.body preserve=yes
	SetServerName( ai_stServerName);
	SetServiceName( ai_stServiceName);

	return M4_SUCCESS;
  //## end ClCcExecutor::SetIdentification%940334698.body
}

m4return_t ClCcExecutor::InitializeRecoveryM4O ()
{
  //## begin ClCcExecutor::InitializeRecoveryM4O%940947251.body preserve=yes
	m4return_t res = M4_SUCCESS  ;/* Resultado del método                     */
	M4ClString stError           ;/* Parámetro de error							  */
	m4uint32_t uiNumError = 0    ;/* Código de error                          */
	ClNode * poNode              ;/* Puntero al nodo principal del M4Object   */
	m4VariantType vParam         ;/* Parámetro para el LN4                    */
	m4VariantType vRes           ;/* Resultado de las llamadas al LN4         */

	/* Construimos el M4Object del recuperador                                */
	/* Guardamos su puntero en un atributo propio, porque va a estar siempre  */
	/* en memoria                                                             */
	if (m_bIsValid == M4_TRUE)
	{
		m_pClChannelManager -> CreateChannel (m_pRecoveryM4O);

		if (m_pRecoveryM4O != 0)
		{
			m_pRecoveryM4O->Role_ID.Set( M4CL_SEC_SUPER_USER );
			if (m_pRecoveryM4O->BuildFromId( "C4_JS_SCHED_DATES") == M4_SUCCESS)
			{
				m_pRecoveryM4OAccess = m_pRecoveryM4O->CreateAccess ();
				if ( m_pRecoveryM4OAccess != 0 )
				{
					/* Pedimos un puntero al nodo principal del M4Object                   */
					poNode = &(m_pRecoveryM4OAccess->Node ["N4_JS_SCHED_DATES"]);
					if ( poNode )
					{
						vParam.Type = M4CL_CPP_TYPE_STRING_VAR;
						vParam.Data.PointerChar = m_stServerName;

						if ( poNode->RecordSet.Current.Item[ "SET_SERVER_NAME" ].StackPush(vParam) == M4_SUCCESS )
						{
							stError = "C4_JS_SCHED_DATES.SET_SERVER_NAME";
							if (poNode->RecordSet.Current.Item[ "SET_SERVER_NAME" ].Call() == M4_SUCCESS )
							{
								if (poNode->RecordSet.Current.Item[ "SET_SERVER_NAME" ].StackPop (vRes) == M4_SUCCESS )
								{
									if (vRes.Data.DoubleData != 0)
										uiNumError = M4_JS_REC_EXECUTING_METHOD;
								}
								else
									uiNumError = M4_JS_REC_POPPING_PARAMETERS;
							}
							else
								uiNumError = M4_JS_REC_CALLING_METHOD;
						}
						else
							uiNumError = M4_JS_REC_PUSHING_PARAMETERS;

						if (uiNumError == 0)
						{
							vParam.Type = M4CL_CPP_TYPE_STRING_VAR;
							vParam.Data.PointerChar = m_stServiceName;

							if ( poNode->RecordSet.Current.Item[ "SET_SERVICE_NAME" ].StackPush(vParam) == M4_SUCCESS )
							{
								stError = "C4_JS_SCHED_DATES.SET_SERVICE_NAME";
								if (poNode->RecordSet.Current.Item[ "SET_SERVICE_NAME" ].Call() == M4_SUCCESS )
								{
									if (poNode->RecordSet.Current.Item[ "SET_SERVICE_NAME" ].StackPop (vRes) == M4_SUCCESS )
									{
										if (vRes.Data.DoubleData != 0)
											uiNumError = M4_JS_REC_EXECUTING_METHOD;
										else
											/* Liberamos la conexión lógica             */
											m_poObjService -> GetpConnProvider() -> SetConnection ();
									}
									else
										uiNumError = M4_JS_REC_POPPING_PARAMETERS;
								}
								else
									uiNumError = M4_JS_REC_CALLING_METHOD;
							}
							else
								uiNumError = M4_JS_REC_PUSHING_PARAMETERS;
						}
					}
					else
					{
						stError = "C4_JS_SCHED_DATES.N4_JS_SCHED_DATES";
						uiNumError = M4_JS_REC_ACCESSING_NODE;
					}
				}
				else
				{
					stError = "ClChannel->CreateAccess";
					uiNumError = M4_JS_REC_EXECUTING_METHOD;
				}
			} 
			else 
			{
				stError = "ClChannel->BuildFromId( C4_JS_SCHED_DATES)";
				uiNumError = M4_JS_REC_EXECUTING_METHOD;
			} /* endif */
		}
		else 
		{
			stError = "ClChannelManager->CreateChannel";
			uiNumError = M4_JS_REC_EXECUTING_METHOD;
		} 
	}

	if (uiNumError > 0)
	{
		m_poObjService -> GetpConnProvider() -> SetConnection ();

		if ( m_pRecoveryM4O != NULL) 
			m_pRecoveryM4O->Destroy();

		if ( m_pRecoveryM4OAccess != NULL)
			m_pRecoveryM4OAccess->Destroy();

      BL_BEGIN( ERRORLOG, uiNumError);
      BL_OBJ << stError << "ClCcExecutor::InitializeRecoveryM4O";
      BL_END;
      res = M4_ERROR;
	} 
	else
	{
		if (m_poBuilder != NULL)
			m_poBuilder->SetSource( m_pRecoveryM4OAccess);
	}

   return res;
  //## end ClCcExecutor::InitializeRecoveryM4O%940947251.body
}

m4return_t ClCcExecutor::CleanDatabase (m4uint32_t &ai_uiFlag)
{
  //## begin ClCcExecutor::CleanDatabase%941731803.body preserve=yes
	m4return_t res = M4_SUCCESS  ;/* Resultado del método                     */
	M4ClString stError           ;/* Parámetro de error							  */
	m4uint32_t uiNumError = 0    ;/* Código de error                          */
	ClNode * poNode              ;/* Puntero al nodo principal del M4Object   */
	m4VariantType vServerName    ;/* Parámetro para el LN4                    */
	m4VariantType vServiceName   ;/* Parámetro para el LN4                    */
	m4VariantType vAction        ;/* Parámetro para LN4: acción a ejecutar    */
	m4VariantType vNumReservados ;/* Parámetro para el LN4: nº trabajos res.  */
	m4VariantType vNumEjecutando ;/* Parámetro para el LN4: nº trabajos int.  */
	m4VariantType vRes           ;/* Resultado de las llamadas al LN4         */

	if (m_bIsValid == M4_TRUE)
	{
		/* Pedimos un puntero al nodo principal del M4Object                   */
		poNode = &(m_pRecoveryM4OAccess->Node ["N4_JS_SCHED_DATES"]);
		if ( poNode )
		{
			stError = "C4_JS_SCHED_DATES->CLEAN_DB";
			
			vServerName.Type = M4CL_CPP_TYPE_STRING_VAR;
			vServerName.Data.PointerChar = m_stServerName;
			vServiceName.Type = M4CL_CPP_TYPE_STRING_VAR;
			vServiceName.Data.PointerChar = m_stServiceName;
			vAction.Type = M4CL_CPP_TYPE_NUMBER;
			/* Si viene un 0, primera inicialización, pedimos que haga todo > 2 */
			/* Si viene un 1, es una inicialización posterior, y sólo pedimos   */
			/* quitar las marcas de reserva > 0                                 */
			vAction.Data.DoubleData = 2 * (1 - ai_uiFlag);

		   if ( poNode->RecordSet.Current.Item[ "CLEAN_DB" ].StackPush(vServerName)    == M4_SUCCESS &&
		        poNode->RecordSet.Current.Item[ "CLEAN_DB" ].StackPush(vServiceName)   == M4_SUCCESS &&
		        poNode->RecordSet.Current.Item[ "CLEAN_DB" ].StackPush(vAction)        == M4_SUCCESS &&
		        poNode->RecordSet.Current.Item[ "CLEAN_DB" ].StackPush(vNumReservados) == M4_SUCCESS &&
		        poNode->RecordSet.Current.Item[ "CLEAN_DB" ].StackPush(vNumEjecutando) == M4_SUCCESS ) 
			{
	         if (poNode->RecordSet.Current.Item[ "CLEAN_DB" ].Call() == M4_SUCCESS )
				{
			   	if (poNode->RecordSet.Current.Item[ "CLEAN_DB" ].StackPop(vNumReservados) == M4_SUCCESS &&
			   	    poNode->RecordSet.Current.Item[ "CLEAN_DB" ].StackPop(vNumEjecutando) == M4_SUCCESS &&
			   	    poNode->RecordSet.Current.Item[ "CLEAN_DB" ].StackPop(vRes)           == M4_SUCCESS )
					{
						if (vRes.Data.DoubleData == 0)
						{
							/* Habría que sacar alguna información a la log         */
							/* diciendo qué hemos hecho                             */
						}
						else
							uiNumError = M4_JS_REC_EXECUTING_METHOD;
					}
					else
						uiNumError = M4_JS_REC_POPPING_PARAMETERS;
				}
				else
					uiNumError = M4_JS_REC_CALLING_METHOD;
			}
			else
				uiNumError = M4_JS_REC_PUSHING_PARAMETERS;
		}
		else 
		{
			stError = "C4_JS_SCHED_DATES->N4_JS_SCHED_DATES";
			uiNumError = M4_JS_REC_ACCESSING_NODE;
		} 
	}

   if (uiNumError > 0)
   {
      BL_BEGIN( ERRORLOG, uiNumError);
      BL_OBJ << stError << "ClCcExecutor::CleanDatabase";
      BL_END;
      res = M4_ERROR;
   } 

   return res;
  //## end ClCcExecutor::CleanDatabase%941731803.body
}

m4return_t ClCcExecutor::ReleaseResources ()
{
  //## begin ClCcExecutor::ReleaseResources%942857887.body preserve=yes
	m4return_t res; /* Resultado del método                                   */

	res = m_poObjService -> GetpConnProvider() ->SetConnection(NULL,M4_TRUE);

	return res;
  //## end ClCcExecutor::ReleaseResources%942857887.body
}

// Additional Declarations
  //## begin ClCcExecutor%36E4EA4102BC.declarations preserve=yes
  //## end ClCcExecutor%36E4EA4102BC.declarations

//## begin module%36E6252A02D1.epilog preserve=yes
//## end module%36E6252A02D1.epilog

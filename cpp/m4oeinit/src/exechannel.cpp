//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              serial 
// File:                exechannel.cpp
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                02-10-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module is the executor of OLTP in the server
//
//
//==============================================================================

#include "exechannel.hpp"

#include "access.hpp"
#include "chan_knl.hpp"
#include "chaninfo.hpp"
#include "cldatstoiod.hpp"
#include "conn_provider.hpp"
#include "cs_result.hpp"
#include "csres.hpp"
#include "execntxt.hpp"
#include "exertinfo.hpp"
#include "exvariables.hpp"
#include "iexecutor.hpp"
#include "lconn_interface.hpp"
#include "m4srvres.hpp"
#include "objdir.hpp"
#include "cldefine.hpp"
#include "m4trace.hpp"
#include "execdumpinfo.hpp"

#include "channel.hpp"

#include "licensemanagerinterface.hpp"
#include "threadobjs.hpp"
#include "rowsquota.hpp"
// --------------------------------------------------------


// Function: PrepareError
//
// Parameters:
//      ai_iErrorType - Tipo de error que se ha producido
//      ai_oDirectory - Directorio de objetos
//      ai_iResultCode - El codigo de retorno de la ejecucion dle metodo
//
// Return: void
//
// Definition:
//      Prepara el objeto para retornar al cliente la informacion sobre el error 
static void PrepareError(const m4int16_t      ai_iErrorType,
                         ClCMObjectDirectory& ai_oDirectory,
                         const m4return_t     ai_iResultCode = -1)
{
    m4return_t iRet = M4_SUCCESS;
    ClChannel* pChannel;
    ClProperty* pProp = NULL;
    m4char_t pAuxString[128];
    ClClientServerResult* pCSResult;
    m4int16_t i;
    m4VariantType oVar;

    switch(ai_iErrorType)
    {
    case M4_CSR_ERROR_DESERIALIZE:
        // Ha habido un error en la deserializacion del objDir
#ifdef _DEBUG
        DUMP_CHLOG_ERROR(M4_CS_DESERIALIZE_IN_SERVER);
#else
        DUMP_CHLOG_DEBUG(M4_CS_DESERIALIZE_IN_SERVER);
#endif

        pCSResult = ai_oDirectory.CreateClientServerResult();
        pCSResult->SetResponseCode(M4_CSR_ERROR_DESERIALIZE);
        break;

    case M4_CSR_ERROR_NO_CHANNEL:
        // No se encontro ningun canal para ejecutar
        DUMP_CHLOG_ERROR(M4_CS_NO_CHANNEL);
        pCSResult = ai_oDirectory.CreateClientServerResult();
        pCSResult->SetResponseCode(M4_CSR_ERROR_NO_CHANNEL);
        break;

    case M4_CSR_ERROR_NO_EXECONTEXT:
        // No se encontro ningun contexto de ejecucion
        DUMP_CHLOG_ERROR(M4_CS_NO_EXECONTEXT);
        pCSResult = ai_oDirectory.CreateClientServerResult();
        pCSResult->SetResponseCode(M4_CSR_ERROR_NO_EXECONTEXT);
        break;

    case M4_CSR_ERROR_EXECUTE:
        // Se ha producido un error durente la ejecucion del metodo
        DUMP_CHLOG_DEBUG(M4_CS_ERROR_EXECUTE);
        
        pCSResult = ai_oDirectory.CreateClientServerResult(M4_FALSE);

        // Tengo que comprobar si ha habido un error de concurrencia
        // Me basta con buscar si alguno de los canales implicados en la
        // transaccion tiene la propiedad M4CMPROP_CHANNEL_NEW_RECORD_VERSION

        i = ai_oDirectory.GetFirstObjectIndex(ChannelObjectType);
        for (; i != -1; i = ai_oDirectory.GetNextObjectIndex())
        {
            pChannel = (ClChannel*)ai_oDirectory.GetObjectAddress(i);
            
            pProp = pChannel->Property.GetProperty(M4CMPROP_CHANNEL_NEW_RECORD_VERSION);
            
            if (pProp != NULL)
            {
				pCSResult->SetResponseCode(M4_CSR_ERROR_CONCURRENCY);
				
				// Meto parejas de mensajes (indice del canal en el objDir, propiedad)
				sprintf(pAuxString, "%d", i);
				pCSResult->AddMess(pAuxString);
				
				pProp->Get(oVar);
				pCSResult->AddMess(oVar.Data.PointerChar);
            }
        }

        if (pCSResult->GetResponseCode() == M4_CSR_SUCCESS)
        {
            // Ha sido un error en la ejecucion
            pCSResult->SetResponseCode(M4_CSR_ERROR_EXECUTE);
            
            sprintf(pAuxString, "%d", ai_iResultCode);
            pCSResult->AddMess(pAuxString);
        }
        break;

    default:
        M4_ASSERT(0);
    }
}

void ExecCheckSum2(M4DataStorage *ai_poDataStorage)
{
   // ClDataUnit *poDataUnit;
   m4uint32_t sum=0, 
              total_len=0;
   m4uint16_t n_blocks=0;

   ai_poDataStorage->InitializeIteration();
/*
   while (ai_poDataStorage->GetNext(poDataUnit)==M4_SUCCESS)
   {
      n_blocks++;
      m4uint32_t size=poDataUnit->GetSize();

      total_len+=size;

      m4puint8_t pBuffer=(m4puint8_t) poDataUnit->GetBuffer();

      while (size--)
         sum += *(pBuffer++);
   }
*/
   cout << "ExeForChannel: nblocks = " << n_blocks << 
           " total_len = " << total_len << " and sum = " << sum << endl;

   ai_poDataStorage->InitializeIteration();
}

m4return_t ExecChannel(ClChannelManager* poChannelManager,
                       M4DataStorage*    ai_poInputDataStorage,
                       M4DataStorage*    ao_poOutputDataStorage,
					   ClStatus*		 ai_poClStatus,
					   ExecDumpInfo*	 aio_poDumpInfo,
					   ClConfiguration*  ai_poConfiguration,
					   ClLConn_Provider* ai_lConnProvider,
					   IObjDataCache*	 ai_pIObjDataCache)
{
#ifdef _DEBUG
	cerr << "OLTP EXECUTOR" << endl;
#else
	m4TraceLevel(2,cerr << "OLTP+" << endl);
#endif
	
	// Initial step
	if (NULL != aio_poDumpInfo)
		aio_poDumpInfo->SetCurrentStage("Preparing to Execute", 0);
	
	m4return_t iResult;
	
	ClDataStoreIODriver IOD;
	if (IOD.Init(GEN_IO_MODE_READ, ai_poInputDataStorage) != M4_SUCCESS)
		return M4_ERROR;
	
	// Reset
	if (NULL != aio_poDumpInfo)
		aio_poDumpInfo->SetCurrentStage("Reseting Old Directory");
	
	//
	// Create an object directory ... (begin the game)
	//
	ClCMObjectDirectory oDirectory (poChannelManager);
	
	if (NULL != aio_poDumpInfo)
		aio_poDumpInfo->m_pObjectDirectory = &oDirectory;
	
	m4int16_t iIndex;
	
	// Deserializo el Directory
	if (NULL != aio_poDumpInfo)
		aio_poDumpInfo->SetCurrentStage("Deserializing");
	
	iResult = oDirectory.DeSerialize(IOD, ai_pIObjDataCache, M4_TRUE);
	
	if (iResult != M4_SUCCESS)
	{
		PrepareError(M4_CSR_ERROR_DESERIALIZE, oDirectory);
	}
	else
	{
		// Localizamos el ClOLTPExecutionContext que sabe que Access hay que ejecutar
		iIndex = oDirectory.GetFirstObjectIndex(OLTPExecutionContextType);
		
		if (iIndex == -1)
			PrepareError(M4_CSR_ERROR_NO_EXECONTEXT, oDirectory);
		else
		{
			ClOLTPExecutionContext* poOLTPExecutionContext =
				(ClOLTPExecutionContext*)oDirectory.GetObjectAddress(iIndex);
			
			// Obtenemos el ejecutor
			IExecutor* poExecutor = poOLTPExecutionContext->GetExecutor();

			ClChannel* poChannel = poOLTPExecutionContext->GetAccess()->GetpChannel();
			
			if (NULL != aio_poDumpInfo)
				aio_poDumpInfo->m_pExecutor = poExecutor;
			
#ifdef _DEBUG
			cerr << "Ejecutando metodo del canal " << poChannel->GetpChannel_Data()->GetpChannelDef()->Id() << endl;
#else
			m4TraceLevel(3,cerr << poChannel->GetpChannel_Data()->GetpChannelDef()->Id() << endl);
#endif
			
			poExecutor->SetpCancelStatus(ai_poClStatus);
			
			//
			// Hacemos la ejecución del método
			//
			if (NULL != aio_poDumpInfo)
			{
				aio_poDumpInfo->SetCurrentStage("Executing method of channel");
				*aio_poDumpInfo << poChannel->GetpChannel_Data()->GetpChannelDef()->Id() << endl;
				*aio_poDumpInfo << "Stack State: " << *poExecutor << endl;


				aio_poDumpInfo->SetCurrentStage("Executing");
			}
			
			if (ai_poConfiguration)
			{
				// Se fijan las propiedades para que se puedan ver desde la herramienta
				// de administración del servidor.
				
				// 1st: method executing...
				ClExecutorRuntimeInfo oInfo;
			
				poExecutor->GetRTInfo(&oInfo);
				string info=oInfo.GetInfoAsString(); 
				oInfo.Reset();
				
				ai_poConfiguration->SetStringValue("",PROP_EXECUTION_DESCRIPTION,(m4pchar_t)info.c_str());
				
				// 2nd: initial channel list...
				ClChannelsInfo oAuxChannelsInfo(oDirectory);
				
				ai_poConfiguration->SetStringValue("",PROP_OBJECT_LIST,(m4pchar_t)oAuxChannelsInfo.ConvertToString());
			}
			
			//Bug:63638   Begin  -----------------------------------------------------
			// Comprobamos si existe alguna transacción en curso antes de la ejecución OLTP. Si es
			//  así, tiene que haber sido creada en una ejecución PROXY, con lo cual no debe ser cerrada en
			//  la ejecución OLTP. Esta situación se da cuando estamos depurando con el Stepper un canal que
			//  ha abierto transacciones (ejecución proxy por depuración). El stepper utiliza canales OLTP (siempre
			//  sin transacciones) en ejecuciones de reentrada para hacer refrescos de pantalla. Al salir 
			//  de estas ejecuciones OLTP es cuando se cierran las transacciones abiertas por el canal que se está
			//  depurando y esto provoca el error.
			m4int32_t iTransaccionesBeforeOLTPExecution = 0;
			if ((poChannelManager != NULL) && (NULL != ai_lConnProvider))
			{
				ClLConn_Interface* pLConn = ai_lConnProvider->GetConnection(0, M4_FALSE);
				if (pLConn != NULL)
				{
					iTransaccionesBeforeOLTPExecution = pLConn->GetNumTrans();
				}
			}
			//Bug:63638   End    -----------------------------------------------------


			/* Bug 0141976
			Hay que crear una referencia artificial antes de la ejecución y borrarla después.
			Esto es para que no se borre este canal si hay borrado de niveles 2.
			*/
			ClAccess	*poAccess = poChannel->CreateAccess() ;
			iResult = poExecutor->Execute() ;
			poAccess->Destroy() ;
			
			// Switches off the transaction rows quota control because the object directory is going to be be destroyed 
			// when it goes out of scope, decreasing then the quota's counter.
			ClRowsQuotaController* pRQC = (ClRowsQuotaController*)ClThreadObjects::Get(ClThreadObjects::ROWS_QUOTA_CONTROLLER);
			if (pRQC != NULL)
			{
				pRQC->SwitchOff();
			}

			if (ai_poConfiguration)
			{
				ai_poConfiguration->SetStringValue("", PROP_EXECUTION_DESCRIPTION, "-");
				ai_poConfiguration->SetStringValue("", PROP_OBJECT_LIST, "-");
			}
			
			if (ai_poClStatus != NULL && iResult == M4CL_USER_ABORT_ALL_REQUEST)
			{
				m4char_t buffer[128];
				sprintf(buffer, "In oltp execution. Cancelation processed in request id %d.", ai_poClStatus->GetIdObject() ) ;
				DUMP_CHLOG_DEBUGF ( M4_CANCELLATION_DETECTED , buffer);
			}
			
			//Bug:63638
			if(iTransaccionesBeforeOLTPExecution <= 0) {
				// Hacemos un RollBack incondicional para canales OLTP
				if ((poChannelManager != NULL) && (NULL != ai_lConnProvider))
				{
					ClLConn_Interface* pLConn = ai_lConnProvider->GetConnection(0, M4_FALSE);
					if (pLConn != NULL)
					{
						m4bool_t bFree = M4_FALSE;
						if (pLConn->EndTransaction (NULL, M4_FALSE, bFree) != M4_SUCCESS)
						{
							iResult = M4_ERROR;
							DUMP_CHLOG_ERROR(M4_CS_ROLL_BACK_INVOQUED_IN_SERVER_SIDE);
						}
					}
				}
			}
			
			if (NULL != aio_poDumpInfo)
				aio_poDumpInfo->SetCurrentStage("Processing execution results");
			
			poExecutor->SetpCancelStatus(NULL);
			
			if (iResult != M4_SUCCESS)
				PrepareError(M4_CSR_ERROR_EXECUTE, oDirectory, iResult);
		}
	}
	
	//
	// Preparamos el output buffer
	//
	if (IOD.Init(GEN_IO_MODE_WRITE, ao_poOutputDataStorage)!=M4_SUCCESS)
		return M4_SUCCESS;
	
	if (NULL != aio_poDumpInfo)
		aio_poDumpInfo->SetCurrentStage("Serialize");
	
	if (oDirectory.Serialize(IOD) != M4_SUCCESS)
	{
#ifdef _DEBUG
		DUMP_CHLOG_ERROR(M4_CS_SERIALIZE_IN_SERVER);
#else
		DUMP_CHLOG_DEBUG(M4_CS_SERIALIZE_IN_SERVER);
#endif
		return M4_ERROR;
	}
	

	if (NULL != aio_poDumpInfo)
		aio_poDumpInfo->SetCurrentStage("Deletes the directory");
	
#ifdef _DEBUG
	cerr << endl << "End of OLTP remote execution" << endl;
#else
	m4TraceLevel(2,cerr << "OLTP-" << endl);
#endif
	
	return M4_SUCCESS;
}

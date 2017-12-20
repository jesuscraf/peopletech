#include "dsres.hpp"

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             channel.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             13/06/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================


#include "chan_knl.hpp"

#include "access.hpp"
#include "blockref.hpp"
#include "bnihil.hpp"
#include "channel.hpp"
#include "chlog.hpp"
#include "cldmcontext.hpp"
#include "compst_common.hpp"
#include "cstracesize.hpp"
#include "m4mdrt.hpp"
#include "node_knl.hpp"
#include "sttlib.hpp"
#include "vrb.hpp"

//----------------------------------------------------------------------------
// class ClChannel
//----------------------------------------------------------------------------


// 19/04/2005	Se han introducido los miembros MetaIdentifier y MetaOrganization
// 22/06/2005	Se ha introducido el miembro ApplyMappings y se ha quitado el MetaOrganization
#define CLCHANNEL_SERIALIZATION_VERSION 12

m4uint16_t ClChannel::GetSerializationVersion(void)
{
   return CLCHANNEL_SERIALIZATION_VERSION +
          ClChannel_Data::GetSerializationVersion();
   // + la suma de los elementos de los que depende
}

m4return_t ClChannel::Serialize (ClGenericIODriver& IOD, ClChannelProcess* const ai_pChannelProcess, ClCsTraceSize* const ai_pCsTraceSize, const m4uint32_t ai_iBlockIndex /*= 0*/, const m4uint32_t ai_iRegisterIndex /*= 0*/)
{
	m4return_t iRet = M4_ERROR;

    if (ai_pChannelProcess)	{
		iRet = ai_pChannelProcess->Pre_Channel_Process(this);
        if (iRet != M4_SUCCESS) {
            DUMP_CHLOG_DEBUGF(M4_CH_DS_PRE_PROCESS_CHANNEL, M4Obj(GetpCMCR()));
            return iRet;
        }
    }

	ClCsTraceSize* pCsTraceSizeChild = 0;
	ClCsTraceSize* pCsTraceSizePropery = 0;
	if (ai_pCsTraceSize) {
		pCsTraceSizeChild = ai_pCsTraceSize->CreateChild(GetpChannelDef()->Id());
		pCsTraceSizeChild->SetOffsetIni(IOD.GetActualOffset());
	}

	m4uint8_t iVectorDelta = CheckDebugModeAndGetVectorDeltaToSerialize();

	// Send the statistics generation state
    iRet = IOD.Write(m_iSttInit);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

	iRet = m_vLabel.Serialize(IOD);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

	// The address of this nil object will be referred
	// to later in the access and so we need to store its
	// address so that we can refernce it in the deserialize
	m4bool_t exists;
	iRet = IOD.Write((void*)&g_oNihilBlock, &exists);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

	if (pCsTraceSizeChild) {
		pCsTraceSizePropery = pCsTraceSizeChild->CreateChild("Properties");
		pCsTraceSizePropery->SetOffsetIni(IOD.GetActualOffset());
	}

	// La conexión lógica
	iRet = LDB_LastLogicConn._Serialize(IOD);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

	iRet = Property.Serialize( IOD, m_iIteSerMod );
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

	if (pCsTraceSizePropery) {
		pCsTraceSizePropery->SetOffsetEnd(IOD.GetActualOffset());
	}

    // Serializamos la autoload
	iRet = IOD.Write(GetAutoLoadMode());
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

    if (m_pClChannel_Data) {
        iRet = m_pClChannel_Data->Serialize(IOD, m_iRecSerMod, m_iIteSerMod, ai_pChannelProcess, pCsTraceSizeChild, ai_iBlockIndex, ai_iRegisterIndex, iVectorDelta);
		if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	}

	if (pCsTraceSizeChild) {
		pCsTraceSizeChild->SetOffsetEnd(IOD.GetActualOffset());
	}

    if (ai_pChannelProcess) {
        iRet = ai_pChannelProcess->Post_Channel_Process(this);
		
		if (iRet != M4_SUCCESS) goto RETURN_ERROR;
    }

	// bg 114200. It will be synchronized when client side deserialize it.
	m_bIsSynchronized = M4_FALSE;

    return iRet;

RETURN_ERROR:
	DUMP_CHLOG_DEBUGF(M4_CH_DS_SERIALIZE_CHANNEL, M4Obj(GetpCMCR()));
	return iRet;
}

/**
 * Comprueba el estado de depuración, y en función de este determina el número de vectores de deltas
 * que hay que utilizar. Estos vectores de deltas se crearán al terminar la deserialización.
 * Si en el cliente el estado de depuración es IS_DEBUG_LAST_TIME lo cambia por IS_NOT_DEBUG.
 *
 * @return Número de vectores de deltas que hay que usar, en función del modo de depuración.
 */
m4uint8_t ClChannel::CheckDebugModeAndGetVectorDeltaCount(void)
{
	const m4bool_t bExecutingInClient = m_poClChannelManager->GetDMContext()->ExecutingInClient();
	m4uint8_t iRet = (m4uint8_t)-1;
	m4uint8_t csExeType = (m4uint8_t)-1;

	if (bExecutingInClient == M4_FALSE) {
		switch (m_eDebugMode) {
		case IS_NOT_DEBUG:
		case IS_DEBUG:
		case IS_DEBUG_LAST_TIME:
			// Crea un vector de delta, si ya existe uno lo resetea.
			iRet = 1;
			break;

		case IS_DEBUG_FIRST_TIME:
			// Crear dos vectores de deltas, si ya exsite alguno lo reseteo.
			iRet = 2;
			break;

		default:
			M4_ASSERT(0);
		}
	} else {
		switch (m_eDebugMode) {
		case IS_DEBUG_LAST_TIME:
			SetDebugMode(IS_NOT_DEBUG);

		case IS_NOT_DEBUG:
			csExeType = GetpChannelDef()->CsExeType() ;
			if (csExeType == M4CL_CSEXETYPE_OLTP) {
				iRet = 0;	// Me cargo los vectores de deltas si existe alguno.
			} else if (csExeType == M4CL_CSEXETYPE_AUTO) {
				iRet = 1;	// Si es AUTO no se puede cargar el vector de deltas
			} else {
				iRet = 1;	// Se trata de un proxy.
			}
			break;
			
		case IS_DEBUG_FIRST_TIME:
		case IS_DEBUG:
		case IS_DEBUG_VIEW:
			// Crea un vector de delta, si ya existe uno lo resetea.
			iRet = 1;
			break;

		default:
			M4_ASSERT(0);
		}
	}
	return iRet;
}

/**
 * Comprueba el estado de depuración, y en función de este determina el vectores de deltas
 * que hay que utilizar durante la serialización.
 *
 * @return Índice del vector de deltas que hay que usar durante la serialización.
 */
m4uint8_t ClChannel::CheckDebugModeAndGetVectorDeltaToSerialize(void) const
{
	switch (m_eDebugMode) {
	case IS_DEBUG_LAST_TIME:
		{
			const m4bool_t bExecutingInClient = m_poClChannelManager->GetDMContext()->ExecutingInClient();
			if (bExecutingInClient == M4_FALSE) {
				return 1;
			}
		}

	case IS_NOT_DEBUG:
	case IS_DEBUG_FIRST_TIME:
	case IS_DEBUG:
		return 0;
		break;

	default:
		M4_ASSERT(0);
	}
	return 0;
}

m4return_t ClChannel::DeSerialize(ClGenericIODriver& IOD)
{
    m4uint32_t iBlockIndex = 0;
    m4uint32_t iRegisterIndex = 0;

    return DeSerialize(IOD, iBlockIndex, iRegisterIndex, 0, M4_TRUE);
}

m4return_t ClChannel::DeSerialize(ClGenericIODriver& IOD, m4uint32_t& ai_iBlockIndex, m4uint32_t& ai_iRegisterIndex, ClCsTraceSize* const ai_pCsTraceSize, m4bool_t ai_bSetSttLevel)
{
	m4bool_t exists = M4_FALSE;
	m4uint16_t iAuxSttInit = 0;
	ClCsTraceSize* pCsTraceSizeChild = 0;
	ClCsTraceSize* pCsTraceSizePropery = 0;
	
	if (ai_pCsTraceSize) {
		pCsTraceSizeChild = ai_pCsTraceSize->CreateChild(GetpChannelDef()->Id());
		pCsTraceSizeChild->SetOffsetIni(IOD.GetActualOffset());
	}
	
	m4uint8_t iVectorDeltaCount = CheckDebugModeAndGetVectorDeltaCount();
	
	m4return_t iRet = IOD.Read(iAuxSttInit);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	
	if (ai_bSetSttLevel == M4_TRUE) {
		SetStatisticsLevel(iAuxSttInit);
	}
	
	iRet = m_vLabel.DeSerialize(IOD);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	
	// For a global object (of which this nil object is a 
	// particularly fine example) we need to check to
	// see if we have referenced the object before.
	// If not, then we need to store the address of
	// this global object which is initialised previously
	void* poAuxNil;
	iRet = IOD.Read(&poAuxNil, &exists);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	
	if (exists == M4_FALSE) {
		iRet = IOD.StoreAddress((void*)&g_oNihilBlock);
		if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	}
	
	if (pCsTraceSizeChild) {
		pCsTraceSizePropery = pCsTraceSizeChild->CreateChild("Properties");
		pCsTraceSizePropery->SetOffsetIni(IOD.GetActualOffset());
	}
	
	// La conexión lógica
	iRet = LDB_LastLogicConn._DeSerialize(IOD);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	
	iRet = Property.DeSerialize(IOD, 1);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	
	if (pCsTraceSizePropery) {
		pCsTraceSizePropery->SetOffsetEnd(IOD.GetActualOffset());
	}
	
	// Deserializamos la autoload
	m4uint8_t iAutoLoadMode;
	iRet = IOD.Read(iAutoLoadMode);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	/* Corrige el bug 51424.
	 * Antes se llamaba al SetAutoLoadMode del canal, esto provocaba que se cambiara el modo de autoload de
	 * todos los accesos, incluyendo los que no se han serializado. Es más correcto cambiar sólo el del canal,
	 * los accesos que se quedaron en cliente mantendrán su modo de autoload, y los accesos que viajaron al
	 * servidor se actualizaran al deserializar.
	 */
	ClAutoLoad_PlugIn::SetAutoLoadMode(iAutoLoadMode);
	
	if (m_pClChannel_Data != NULL) {
		iRet = m_pClChannel_Data->DeSerialize(IOD, m_iIteSerMod, ai_iBlockIndex, ai_iRegisterIndex, pCsTraceSizeChild, iVectorDeltaCount);

		if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	}
	
	if (pCsTraceSizeChild) {
		pCsTraceSizeChild->SetOffsetEnd(IOD.GetActualOffset());
	}
	
	// bg 114200. Now we are sure that this channel is synchronized with its server side.
	m_bIsSynchronized = M4_TRUE;

	return iRet;
	
RETURN_ERROR:
#ifdef _DEBUG
	DUMP_CHLOG_DEBUGF(M4_CH_DS_DESERIALIZE_CHANNEL, M4Obj(GetpCMCR()));
#endif
	
	return iRet;
}



//----------------------------------------------------------------------------
// class ClChannel_Data
//----------------------------------------------------------------------------


#define CLCHANNELDATA_SERIALIZATION_VERSION		12

m4uint16_t ClChannel_Data::GetSerializationVersion(void)
{
   return CLCHANNELDATA_SERIALIZATION_VERSION;
   // + la suma de los elementos de los que depende
}

//
// Function: ClChannel_Kernel::Serialize
//
// Arguments:
//    ClGenericIODriver& IOD - Where we serialize to
//
// Returns: m4return_t
//
// Description:
//   To serialize the channel, we first serialize the data in all
//   of the nodes and then we serialize all of the blocks in the
//   root nodes.  Normally there is only one root node but
//   in the case of a document channel which has a track channel
//   associated, there may be more than one block in a root node.
//
// Author: Stuart Murdoch (mailto:stuartm@meta4.es)
// Created: 5 December 1998
//
m4return_t ClChannel_Data::Serialize(ClGenericIODriver&      IOD,
                                     const m4uint8_t         ai_iRecSerMod,
                                     const m4uint8_t         ai_iIteSerMod,
                                     ClChannelProcess* const ai_pChannelProcess,
									 ClCsTraceSize*    const ai_pCsTraceSize,
                                     const m4uint32_t        ai_iBlockIndex /*=0*/,
                                     const m4uint32_t        ai_iRegisterIndex /*=0*/,
									 const m4uint8_t         ai_iVectorDelta /*= 0*/)
{
	const m4uint16_t		iNumRoots = GetpExtInfo()->NumberOfRoots();
	const m4uint32_t		iNumNodes = Node.Count();
	ClCompiledMCR* const	pCMCR = GetpChannelDef()->GetpCMCR();

	m4uint16_t			i = 0;
	const m4char_t*		pNodeId = 0;
	m4uint32_t			iBlockToSerialize = 0;
    m4uint32_t			iRegisterToSerialize = 0;
	m4uint8_t			iNewRecSerMod = 0;
	ClNode_Data*		pNode = 0;
	ClNodeIndex			oNodeIndex;
    ClHandle			nodeHandle;
	ClBlockReference	oBlockRef;

    if (ai_pChannelProcess) {
        if (ai_pChannelProcess->Pre_ChannelData_Process(this) != M4_SUCCESS) {
            DUMP_CHLOG_DEBUGF(M4_CH_DS_PRE_PROCESS_CHANNELDATA, M4Obj(m_pClChannel_Kernel->GetpCMCR()));
            return M4_ERROR;
        }
    }

    // Para saber como hacer la deserializacion
    // No me hace falta serializar ai_iRecSerMod ya qua la informacion
    // de los registros que se han enviado va en los vectores de deltas
    m4return_t iRet = IOD.Write(ai_iIteSerMod);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

	iRet = IOD.Write(m_KeepStatusOnRemote);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

    iRet = IOD.Write(m_iUniqueRowId);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

    iRet = Operation.Serialize(IOD);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

    // ??? Perhaps we need to serialize the number of nodes here !!!
    // ??? RetVal = IOD.Write(iNumNodes);

    //
    // For every node, serialize the information specific to the node the operation and the RWT
    //
    for (oNodeIndex = 0; oNodeIndex < iNumNodes; oNodeIndex++) {
        pNode = Node.Get(oNodeIndex);

        if (pNode->SerializationRequired(pNode, ai_iIteSerMod) == M4_TRUE) {
            iRet = pNode->Serialize(IOD, ai_iRecSerMod, ai_iIteSerMod, ai_pChannelProcess, ai_pCsTraceSize, ai_iVectorDelta);
			if (iRet != M4_SUCCESS) goto RETURN_ERROR;
		}
    }

    //
    // For each root node...
    //
    for (i = 0; i < iNumRoots; i++) {
        nodeHandle = GetpExtInfo()->RootHandle(i);
        pNode = Node.Get(nodeHandle);

        if (pNode->SerializationRequired(pNode, ai_iIteSerMod) == M4_FALSE)
            continue;

        // Compruebo si el nodo root esta encarrilado, en tal caso serializo solo el bloque que
        // me han inidcado, en caso contrario serializo el unico bloque que tiene el root (el 0)
		pNodeId = pNode->GetpNodeDef()->Id();
        if (pCMCR->GetLaneSonNodeId() != 0 && strcmp(pNodeId, pCMCR->GetLaneSonNodeId()) == 0) {
            if (ai_iBlockIndex == -1 || ai_iRegisterIndex == -1) {
                // Estoy intentando serializar un canal DOC donde el bloque root es nihil
                // Indico que serializo un bloque nihil
                iRet = IOD.Write((m4bool_t) M4_TRUE);
				if (iRet != M4_SUCCESS) goto RETURN_ERROR;
                continue;
            } else {
                iBlockToSerialize = ai_iBlockIndex;
                iRegisterToSerialize = ai_iRegisterIndex;
            }
        } else {
            iBlockToSerialize = 0;
            iRegisterToSerialize = 0;
        }

        // Le pido al BlockStore el proxy que tengo que serializar
        iRet = pNode->GetpBlockStore()->Get(iBlockToSerialize, iRegisterToSerialize, oBlockRef);
		if (iRet != M4_SUCCESS) goto RETURN_ERROR;

        // En un nodo root siempre debe existir el proxy
        M4_ASSERT(oBlockRef.IsNull() == M4_FALSE);

        // Indico que no es un proxy nihil
        iRet = IOD.Write((m4bool_t) M4_FALSE);
		if (iRet != M4_SUCCESS) goto RETURN_ERROR;

        // Serializo el nodo root normalmente
        // Compruebo si el nodo tiene activo el modo branch de la serializacion avanzada
        iNewRecSerMod = pNode->CheckBranchMode(ai_iRecSerMod);

		ClCsTraceSize* pCsTraceSizeChild = 0;
		if (ai_pCsTraceSize) {
			pCsTraceSizeChild = ai_pCsTraceSize->CreateChild(pNodeId);
		}

        iRet = oBlockRef->Serialize(IOD, iNewRecSerMod, ai_iIteSerMod, ai_pChannelProcess, pCsTraceSizeChild, ai_iVectorDelta, M4_FALSE);
		if (iRet != M4_SUCCESS) goto RETURN_ERROR;
    }

    //
    // For each free node...
    //
    m4uint16_t iNumberInverseConector;
    m4bool_t IsFree;

    for (oNodeIndex = 0; oNodeIndex < iNumNodes; oNodeIndex++) {
        pNode = Node.Get (oNodeIndex);

        // Si no es mapped no te lo pienses más que no tienes nada que hacer, sigo con el siguiente
        if (pNode->SerializationRequired(pNode, ai_iIteSerMod) == M4_FALSE) {
            continue;
		}

        // Cambio para soportar problema de carriles de JESUS
        if (pCMCR->GetNodeIsRealRoot(pNode->GetpNodeDef()->GetHandle())) {
            continue;
		}
        // Cambio para soportar problema de carriles de JESUS

        iNumberInverseConector = pNode->GetpExtInfo()->GetNodeNumberOfInvConnectors();

        if (iNumberInverseConector > 0) {
            IsFree = M4_TRUE;

            for (i = 0; IsFree == M4_TRUE && i < iNumberInverseConector; i++) {
                nodeHandle = pNode->GetpExtInfo()->GetNodeInvConnectorHandleByPosition(i);
                if (pNode->GetpExtInfo()->GetNodeConnectorType(nodeHandle) != M4CL_NODE_CONNECTOR_TYPE_FREE) {
                    IsFree = M4_FALSE;
				}
            }

            if (IsFree == M4_TRUE) {
                pNode->GetpUniqueBlock(oBlockRef);

                // Compruebo si el nodo tiene activo el modo branch de la serializacion avanzada 
                iNewRecSerMod = pNode->CheckBranchMode(ai_iRecSerMod);

				ClCsTraceSize* pCsTraceSizeChild = 0;
				if (ai_pCsTraceSize) {
					pCsTraceSizeChild = ai_pCsTraceSize->CreateChild(pNode->GetpNodeDef()->Id());
				}

                iRet = oBlockRef->Serialize(IOD, iNewRecSerMod, ai_iIteSerMod, ai_pChannelProcess, pCsTraceSizeChild, ai_iVectorDelta, M4_FALSE);
				if (iRet != M4_SUCCESS) goto RETURN_ERROR;
            }
        }
    }

    if (ai_pChannelProcess) {
        iRet = ai_pChannelProcess->Post_ChannelData_Process(this);
#ifdef _DEBUG
        if (iRet != M4_SUCCESS) {
            DUMP_CHLOG_DEBUGF(M4_CH_DS_POST_PROCESS_CHANNELDATA, M4Obj(m_pClChannel_Kernel->GetpCMCR()));
		}
#endif
    }

    return iRet;

RETURN_ERROR:
	DUMP_CHLOG_DEBUGF(M4_CH_DS_SERIALIZE_CHANNEL, M4Obj(m_pClChannel_Kernel->GetpCMCR()));
	return iRet;
}

m4return_t ClChannel_Data::DeSerialize(ClGenericIODriver& IOD)
{
    m4uint8_t  iIteSerMod;
    m4uint32_t iBlockIndex = 0;
    m4uint32_t iRegisterIndex = 0;

    return DeSerialize(IOD, iIteSerMod, iBlockIndex, iRegisterIndex, 0, ClChannel::IS_NOT_DEBUG);
}

//
// Function: ClChannel_Kernel::DeSerialize
//
// Arguments:
//    ClGenericIODriver& IOD - Where we deserialize from
//
// Returns: m4return_t
//
// Description:
//
// Author: Stuart Murdoch (mailto:stuartm@meta4.es)
// Created: 5 December 1998
//
m4return_t ClChannel_Data::DeSerialize(ClGenericIODriver&   IOD,
                                       m4uint8_t&           ao_iIteSerMod,
                                       m4uint32_t&          aio_iBlockIndex,
                                       m4uint32_t&          aio_iRegisterIndex,
									   ClCsTraceSize* const ai_pCsTraceSize,
									   m4uint8_t		    ai_iVectorDeltaCount)
{
    const m4uint16_t			iNumRoots = GetpExtInfo()->NumberOfRoots();
	const m4uint32_t			iNumNodes = Node.Count();
    const ClCompiledMCR* const	pCMCR = GetpChannelDef()->GetpCMCR();

	m4uint32_t			iBlockToDeSerialize = 0;
    m4uint32_t			iRegisterToDeSerialize = 0;
	m4uint16_t			i = 0;
	m4uint16_t			iNumberInverseConectors = 0;
	m4bool_t			bPxBlockIsNihil = M4_FALSE;
	m4bool_t			bIsFree = M4_FALSE;
	const m4char_t*		pNodeId = 0;
    ClNode_Data*		pNode = 0;
    ClHandle			iNodeHandle = 0;
	ClNodeIndex			iNodeIndex = 0;
	ClBlockReference	oBlockRef;


    // Para saber como hacer la deserializacion
    // No me hace falta serializar ai_iRecSerMod ya qua la informacion
    // de los registros que se han enviado va en los vectores de deltas
    m4return_t iRet = IOD.Read(ao_iIteSerMod);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

	iRet = IOD.Read(m_KeepStatusOnRemote);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

	if (m_KeepStatusOnRemote == M4_FALSE) {
		ai_iVectorDeltaCount = 0;
	}

    iRet = IOD.Read(m_iUniqueRowId);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

    iRet = Operation.DeSerialize(IOD);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

    // ??? Perhaps we need to Deserialize the number of nodes here !!!
    // ??? RetVal = IOD.Read(numNodes);

    //
    // For every node, deserialize the information specific to the node the operation and the RWT
    //
    for (iNodeIndex = 0; iNodeIndex < iNumNodes; iNodeIndex++) {
        pNode = Node.Get(iNodeIndex);

        if (pNode->SerializationRequired(pNode, ao_iIteSerMod) == M4_TRUE) {
            iRet = pNode->DeSerialize(IOD, ao_iIteSerMod, ai_pCsTraceSize, ai_iVectorDeltaCount);
			if (iRet != M4_SUCCESS) goto RETURN_ERROR;
		}
    }

    //
    // For each root node...
    //
    for (i = 0; i < iNumRoots; i++) {
        iNodeHandle = GetpExtInfo()->RootHandle(i);
        pNode = Node.Get(iNodeHandle);

        if (pNode->SerializationRequired(pNode, ao_iIteSerMod) == M4_TRUE) {
            // Propagate inheritance of node scope items...
            iRet = pNode->InheritConnectedData(NULL);
			if (iRet != M4_SUCCESS) goto RETURN_ERROR;

            // Compruebo si el nodo root esta encarrilado, en tal caso deserializo solo el bloque que
            // me han indicado, en caso contrario deserializo el unico bloque que tiene el root (el 0)
            // Cuando estoy en el server no se tiene que usar este parametro ya que el bloque hay
            // que crearlo nuevo
			if (pCMCR->GetLaneSonNodeId() != 0 && strcmp(pNode->GetpNodeDef()->Id(), pCMCR->GetLaneSonNodeId()) == 0) {
				iBlockToDeSerialize = aio_iBlockIndex;
				iRegisterToDeSerialize = aio_iRegisterIndex;
			} else {
				iBlockToDeSerialize = 0;
				iRegisterToDeSerialize = 0;
			}

                // Ahora puede no existir el proxy
			iRet = IOD.Read(bPxBlockIsNihil);
			if (iRet != M4_SUCCESS) goto RETURN_ERROR;

            if (bPxBlockIsNihil == M4_FALSE) {
                // Le pido al BlockStore el proxy que tengo que deserializar
                // Si no existe me lo crea
                iRet = pNode->GetpBlockStore()->Get(iBlockToDeSerialize, iRegisterToDeSerialize, oBlockRef, 0, 0);
				if (iRet != M4_SUCCESS) goto RETURN_ERROR;

				ClCsTraceSize* pCsTraceSizeChild = 0;
				if (ai_pCsTraceSize) {
					pNodeId = pNode->GetpNodeDef()->Id();
					pCsTraceSizeChild = ai_pCsTraceSize->CreateChild(pNodeId);
				}

                // DeSerializo el nodo root normalmente
				iRet = oBlockRef->DeSerialize(IOD, ao_iIteSerMod, pCsTraceSizeChild, ai_iVectorDeltaCount);
				if (iRet != M4_SUCCESS) goto RETURN_ERROR;
            } else {
                // Estoy deserializando un proxy nihil
                // (en el otro lado exite el proxy, pero es un proxy nihil)
                // Si estoy en la deserializacion del server tengo que añadir el proxy nihil al blockStore
                if (iBlockToDeSerialize == 0) {
                    // Ojo, le digo al ObjDir que el bloque del root es un nihil
                    aio_iBlockIndex = 0;
                    aio_iRegisterIndex = (m4uint32_t)-1;
                }
            }
        }
    }

    //
    // For each free node...
    //
    for (iNodeIndex = 0; iNodeIndex < iNumNodes; iNodeIndex++) {
        pNode = Node.Get (iNodeIndex);

        if (pNode->SerializationRequired(pNode, ao_iIteSerMod) == M4_FALSE) {
            continue;
		}

        // Cambio para soportar problema de carriles de JESUS
        if (pCMCR->GetNodeIsRealRoot(pNode->GetpNodeDef()->GetHandle())) {
            continue;
		}
        // Cambio para soportar problema de carriles de JESUS

        iNumberInverseConectors = pNode->GetpExtInfo()->GetNodeNumberOfInvConnectors();

        if (iNumberInverseConectors > 0) {
            bIsFree = M4_TRUE;

            for (i = 0; bIsFree == M4_TRUE && i < iNumberInverseConectors; i++) {
                iNodeHandle = pNode->GetpExtInfo()->GetNodeInvConnectorHandleByPosition(i);
                if (pNode->GetpExtInfo()->GetNodeConnectorType(iNodeHandle) != M4CL_NODE_CONNECTOR_TYPE_FREE) {
                    bIsFree = M4_FALSE;
				}
            }

            if (bIsFree == M4_TRUE) {
                // Propagate inheritance of node scope items...
                iRet = pNode->InheritConnectedData(NULL);
				if (iRet != M4_SUCCESS) goto RETURN_ERROR;

				ClCsTraceSize* pCsTraceSizeChild = 0;
				if (ai_pCsTraceSize) {
					pCsTraceSizeChild = ai_pCsTraceSize->CreateChild(pNode->GetpNodeDef()->Id());
				}

                pNode->GetpUniqueBlock(oBlockRef);
                iRet = oBlockRef->DeSerialize(IOD, ao_iIteSerMod, pCsTraceSizeChild, ai_iVectorDeltaCount);
				if (iRet != M4_SUCCESS) goto RETURN_ERROR;
            }
        }
    }

	return iRet;

RETURN_ERROR:
	// User readable error saying rows quota exceeded already registered.
	if( iRet != M4_VRB_ERROR_ROWS_QUOTA_EXCEEDED )
	{
		DUMP_CHLOG_ERRORF(M4_CH_DS_DESERIALIZE_CHANNEL, M4Obj(m_pClChannel_Kernel->GetpCMCR()));
	}
    return iRet;
}

m4return_t ClChannel_Data::SetKeepStatus(const m4bool_t ai_bKeepIt, const m4uint8_t ai_iVectorCount)
{
   m_KeepStatus = ai_bKeepIt;
   return Node.SetKeepStatus (ai_bKeepIt, ai_iVectorCount);
}

m4bool_t ClChannel_Data::IsKeepStatus(void) const
{
	return m_KeepStatus;
}

void ClChannel_Data::SetKeepStatusOnRemote(const m4bool_t ai_bKeepIt)
{
   m_KeepStatusOnRemote = ai_bKeepIt;
}

m4bool_t ClChannel_Data::IsKeepStatusOnRemote(void) const
{
   return m_KeepStatusOnRemote;
}

void ClChannel_Data::ResetSerialMarks(void)
{
   Node.ResetSerialMarks();
}


// ------------------ NEW CRG


m4return_t ClChannel::MarkForPersistSerial(const m4uint32_t ai_iBlockIndex,
                                           const m4uint32_t ai_iRegisterIndex)
{
   if (m_pClChannel_Data)
      return m_pClChannel_Data->MarkForPersistSerial(ai_iBlockIndex,
                                                     ai_iRegisterIndex);
   else
      return M4_SUCCESS;
}


m4return_t ClChannel_Data::MarkForPersistSerial(const m4uint32_t ai_iBlockIndex,
                                                const m4uint32_t ai_iRegisterIndex)

{
    ClNode_Data* pNode;
    ClNodeIndex  oNodeIndex;
    ClHandle     nodeHandle;
    m4uint32_t   numNodes = Node.Count();
    m4uint16_t   i;
    
    m4uint16_t numRoots = GetpExtInfo()->NumberOfRoots();
    
    //
    // For each root node...
    //
    m4uint32_t iBlockToMark    = ai_iBlockIndex;
    m4uint32_t iRegisterToMark = ai_iRegisterIndex;
    m4uint8_t  iRecSerMod;
    m4uint8_t  iIteSerMod;
    
    ClBlockReference oBlockRef;
    
    ClCompiledMCR* pCMCR = GetpChannelDef()->GetpCMCR();
    
    for (i = 0; i < numRoots; i++)
    {
        nodeHandle = GetpExtInfo()->RootHandle(i);
        
        // Ojo CRG !!!
        iRecSerMod = M4CL_RECORD_SERIAL_ALL;
        iIteSerMod = M4CL_ITEM_SERIAL_MAPPED;
        
        pNode = Node.Get(nodeHandle);
        M4_ASSERT(pNode);
        
        if (pNode->SerializationRequired(pNode, iIteSerMod) == M4_TRUE)
        {
            // Compruebo si el nodo root esta encarrilado, en tal caso marco solo el bloque que
            // me han inidcado, en caso contrario marco el unico bloque que tiene el root (el 0)
            if (pCMCR->GetLaneSonNodeId() != 0 &&
                strcmp(pNode->GetpNodeDef()->Id(), pCMCR->GetLaneSonNodeId()) == 0)
            {
                if (ai_iBlockIndex == -1 || ai_iRegisterIndex == -1)
                {
                    // Estoy intentando marcar un canal DOC donde el bloque root es nihil
                    // No tengo que hacer nada con este nodo
                    continue;
                }
                else
                {
                    iBlockToMark = ai_iBlockIndex;
                    iRegisterToMark = ai_iRegisterIndex;
                }
            }
            else
            {
                iBlockToMark = 0;
                iRegisterToMark = 0;
            }
            
            // Le pido al BlockStore el proxy que tengo que serializar
            if (pNode->GetpBlockStore()->Get(iBlockToMark, iRegisterToMark, oBlockRef) != M4_SUCCESS)
                return M4_ERROR;
            
            M4_ASSERT(oBlockRef.IsNull() == M4_FALSE);
                
            if (oBlockRef->MarkForPersistSerial() != M4_SUCCESS)
                return M4_ERROR;
        }
    }
    
    //
    // For each free node...
    //
    m4uint16_t iNumberInverseConector;
    m4bool_t IsFree;
    for (oNodeIndex = 0; oNodeIndex < numNodes; oNodeIndex++)
    {
        pNode = Node.Get (oNodeIndex);
        
        // Si no es mapped no te lo pienses más que no tienes nada que hacer, sigo con el siguiente
        iRecSerMod = M4CL_RECORD_SERIAL_ALL;
        iIteSerMod = M4CL_ITEM_SERIAL_MAPPED;
        
        if (pNode->SerializationRequired(pNode, iIteSerMod) == M4_FALSE)
            continue;
        
        // Cambio para soportar problema de carriles de JESUS
        if (pCMCR->GetNodeIsRealRoot(pNode->GetpNodeDef()->GetHandle()))
            continue;
        // Cambio para soportar problema de carriles de JESUS
        
        iNumberInverseConector = pNode->GetpExtInfo()->GetNodeNumberOfInvConnectors();
        
        if (iNumberInverseConector > 0)
        {
            IsFree = M4_TRUE;
            
            for (i = 0
                ; IsFree == M4_TRUE && i < iNumberInverseConector
                ; i++)
            {
                nodeHandle = pNode->GetpExtInfo()->GetNodeInvConnectorHandleByPosition(i);
                if (pNode->GetpExtInfo()->GetNodeConnectorType(nodeHandle) != M4CL_NODE_CONNECTOR_TYPE_FREE)
                    IsFree = M4_FALSE;
            }
            
            if (IsFree == M4_TRUE)
            {
                pNode->GetpUniqueBlock(oBlockRef);
                if (oBlockRef->MarkForPersistSerial() != M4_SUCCESS)
                    return M4_ERROR;
            }
        }
    }
    
    return M4_SUCCESS;
}

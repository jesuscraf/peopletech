//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             ch_block.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             01/09/97
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

#include "px_block.hpp"

#include "access.hpp"
#include "ch_block.hpp"
#include "chan_knl.hpp"
#include "channelprocess.hpp"
#include "chlog.hpp"
#include "clfileiod.hpp"
#include "compst_common.hpp"
#include "cstracesize.hpp"
#include "dsres.hpp"
#include "file_manager.hpp"
#include "file_misc.hpp"
#include "m4mdrt.hpp"
#include "not_cdat.hpp"
#include "node_knl.hpp"
#include "pl_data.hpp"
#include "prnewrecid.hpp"
#include "prsetrecins.hpp"
#include "rthierdes.hpp"
#include "vdelta.hpp"

#define PX_BLOCK_NO_EXIST   0
#define PX_BLOCK_EXIST      1
#define PX_BLOCK_IS_INVALID 2


m4return_t ClBlock_Channel_Proxy::Serialize(ClGenericIODriver&      IOD,
                                            const m4uint8_t         ai_iRecSerMod,
                                            const m4uint8_t         ai_iIteSerMod,
                                            ClChannelProcess* const ai_pChannelProcess,
											ClCsTraceSize*    const ai_pCsTraceSize,
											const m4uint8_t         ai_iVectorDelta,
											m4bool_t                ai_bWritePLFileMap)
{
	m4bool_t bSerializeBlock = M4_FALSE;

    if (ai_pChannelProcess) {
        if (ai_pChannelProcess->Pre_BlockChannelProxy_Process(this) != M4_SUCCESS) {
            DUMP_CHLOG_DEBUGF(M4_CH_DS_PRE_PROCESS_PXBLOCK, m_pClNode_Data->GetpExtInfo()->GetHandle() << ID.RegisterId);
            return M4_ERROR;
        }
    }

	ClCsTraceSize* pCsTraceSizeChild = 0;
	if (ai_pCsTraceSize) {
		pCsTraceSizeChild = ai_pCsTraceSize->CreateChild();
	}

    m4return_t iRet = IOD.Write(m_bConnectedChanged);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	m_bConnectedChanged = M4_FALSE;

    iRet = IOD.Write(m_bLoaded);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

	iRet = IOD.Write(m_bCheckPointMode);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

	// Bug 0317759 Hay que serializar la ordenación actual de los datos
	iRet = IOD.Write(m_CurrentSearchIndexId);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;
    
    // Si el modo de serializacion de registros es M4CL_RECORD_SERIAL_ADVANCED
    // solo tengo que mandar el bloque si este esta marcado (y existe)
    if (ai_iRecSerMod == M4CL_RECORD_SERIAL_ADVANCED && m_iNeedSerial == M4_PXB_NO_SEND) {
        bSerializeBlock = M4_FALSE;
        m_bBlockNotSentBecauseMarks = M4_TRUE;
    } else {
        bSerializeBlock = m_pClBlock ? M4_TRUE : M4_FALSE;
        m_bBlockNotSentBecauseMarks = M4_FALSE;
    }

    // Indico si detras viene el bloque, o no
    iRet = IOD.Write(bSerializeBlock);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

    // If the proxy points to something ... serialize the block
    if (bSerializeBlock == M4_TRUE) {
        // Compruebo si el bloque tiene activo el modo branch de la serializacion avanzada 
        // Si esta activo cambio el modo de serializacion ya que de aqui
        // en adelante hay que serializar todos los registros
        m4uint8_t iNewRecSerMod = ai_iRecSerMod;
        if (ai_iRecSerMod == M4CL_RECORD_SERIAL_ADVANCED && m_bBranchMode == M4_TRUE) {
            iNewRecSerMod = M4CL_RECORD_SERIAL_ALL;
        }
    
        iRet = _SerializeNT(IOD, iNewRecSerMod, ai_iIteSerMod, ai_pChannelProcess, pCsTraceSizeChild, ai_iVectorDelta, ai_bWritePLFileMap);
		if (iRet != M4_SUCCESS) goto RETURN_ERROR;
    }
    
    if (ai_pChannelProcess) {
        iRet = ai_pChannelProcess->Post_BlockChannelProxy_Process(this);
#ifdef _DEBUG
        if (iRet != M4_SUCCESS) {
            DUMP_CHLOG_DEBUGF(M4_CH_DS_POST_PROCESS_PXBLOCK, m_pClNode_Data->GetpExtInfo()->GetHandle() << ID.RegisterId);
		}
#endif
    }

    return iRet;

RETURN_ERROR:
#ifdef _DEBUG
    if (iRet != M4_SUCCESS) {
        DUMP_CHLOG_DEBUGF(M4_CH_DS_SERIALIZE_PXBLOCK, m_pClNode_Data->GetpExtInfo()->GetHandle() << ID.RegisterId);
	}
#endif
	return iRet;
}

m4return_t ClBlock_Channel_Proxy::_SerializeNT(ClGenericIODriver&      IOD,
                                               const m4uint8_t         ai_iRecSerMod,
                                               const m4uint8_t         ai_iIteSerMod,
                                               ClChannelProcess* const ai_pChannelProcess,
					                           ClCsTraceSize*    const ai_pCsTraceSize,
					                           const m4uint8_t         ai_iVectorDelta,
											   m4bool_t                ai_bWritePLFileMap)
{
	m4uint16_t	 i = 0;
	m4uint16_t	 iNodeCount = m_pClNode_Data->GetpExtInfo()->ChildNodeBB.Count();
	const m4char_t*	 pChildNodeId = 0;
	m4uint8_t	 iNewRecSerMod = ai_iRecSerMod;;
	ClBlockReference oBlockRef;
	ClNode_Data*	 pchildnode = 0;
	
	
	// Si el modo de serializacion es avanzado, el bloque no puede estar marcado como para no
	// ser enviado, ya que el ClBlock_Channel_Proxy::Serialize controla esta situacion
	M4_ASSERT (ai_iRecSerMod != M4CL_RECORD_SERIAL_ADVANCED || m_iNeedSerial != M4_PXB_NO_SEND);
	
	// BLOCK LEVEL -------------------------------------------------------
	
	m4bool_t bThereIsAnyChange = M4_FALSE;
	m4return_t iRet = m_pClBlock->Block.SerializeStaticPart(IOD, ai_iRecSerMod, ai_pChannelProcess, bThereIsAnyChange, ai_iVectorDelta);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	
	// If there is any change in values of the record serialize it
	if (bThereIsAnyChange == M4_TRUE) {
		iRet = m_pClBlock->Block.SerializeRecord(IOD, iNewRecSerMod, ai_iIteSerMod, ai_pChannelProcess, ai_pCsTraceSize, 0, ai_iVectorDelta);
		if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	}
	
	// El recorrido por los hijos es independiente de si ha habido cambios o no en los registros
	for (i = 0; i < iNodeCount; i ++) {
		pchildnode = m_pClNode_Data->GetpExtInfo()->ChildNodeBB[(ClChildIndex)i].GetAddress();
		
		// Comprobamos si hay que serializar el nodo hijo
		if (pchildnode->SerializationRequired(m_pClNode_Data, ai_iIteSerMod) == M4_FALSE) {
			continue;
		}
		
		// Conseguimos el bloque hermano
		pchildnode->GetpBlockStore()->Get(ID.BlockUniqueKey, 0, oBlockRef);
		
		// Ahora puede ser cero. Serializamos un m4bool_t indicando esta situacion
		if (oBlockRef.IsNull() == M4_FALSE) {
			iRet = IOD.Write((m4uint8_t)PX_BLOCK_EXIST);
			if (iRet != M4_SUCCESS) goto RETURN_ERROR;
			
			// Compruebo si el nodo tiene activo el modo branch de la serializacion avanzada
			iNewRecSerMod = pchildnode->CheckBranchMode(ai_iRecSerMod);
			
			ClCsTraceSize* pCsTraceSizeChild = 0;
			if (ai_pCsTraceSize) {
				pChildNodeId = pchildnode->GetpNodeDef()->Id();
				pCsTraceSizeChild = ai_pCsTraceSize->GetpParent(ClCsTraceSize::CHANNEL)->CreateChild(pChildNodeId);
			}
			
			iRet = oBlockRef->Serialize(IOD, iNewRecSerMod, ai_iIteSerMod, ai_pChannelProcess, pCsTraceSizeChild, ai_iVectorDelta, ai_bWritePLFileMap);
			if (iRet != M4_SUCCESS) goto RETURN_ERROR;
		} else {
			iRet = IOD.Write((m4uint8_t)PX_BLOCK_NO_EXIST);
			if (iRet != M4_SUCCESS) goto RETURN_ERROR;
		}
	}
	
	// RECORDS LEVEL -------------------------------------------------------
	
	// Si estamos en serializacion avanzada le decimos al VRB que prepare
	// un vector de deltas con los registros que estan marcados
	if (ai_iRecSerMod == M4CL_RECORD_SERIAL_ADVANCED && m_iNeedSerial == M4_PXB_SEND_SOME_RECORDS) {
		iRet = m_pClBlock->Records.PrepareAdvanzedSerial();
		if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	}
	
	iRet = m_pClBlock->Records.SerializeStaticPart(IOD, ai_iRecSerMod, ai_pChannelProcess, bThereIsAnyChange, ai_iVectorDelta);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	
	iNodeCount = m_pClNode_Data->GetpExtInfo()->ChildNodeRB.Count();
	
	if (bThereIsAnyChange == M4_TRUE || iNodeCount > 0) {
		regindex_t iNumRecords = m_pClBlock->Records.GetCount();
		for (regindex_t ri = 0; ri < iNumRecords; ri++) {
			iRet = _SerializeRecord(IOD, ai_iRecSerMod, ai_iIteSerMod, ai_pChannelProcess, ai_pCsTraceSize, ri, bThereIsAnyChange, iNodeCount, ai_iVectorDelta, ai_bWritePLFileMap);
			if (iRet != M4_SUCCESS) goto RETURN_ERROR;
		}
	}
	
	// PARTIAL LOAD -------------------------------------------------------
	
	// Serialize de partial load info
	// CRG Estoy suponiendo que nadie comparte estos ebjetos entre bloques ???
	if (m_pClBlock->m_pClPL_Data) {
		iRet = IOD.Write((m4bool_t)M4_TRUE);
		if (iRet != M4_SUCCESS) goto RETURN_ERROR;
		
		iRet = m_pClBlock->m_pClPL_Data->Serialize(IOD, ai_bWritePLFileMap);
		if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	} else {
		iRet = IOD.Write((m4bool_t)M4_FALSE);
		if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	}

	iRet = IOD.Write(m_pClBlock->m_iRangeSize);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

	iRet = IOD.Write(m_pClBlock->m_iRangeStart);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

	iRet = IOD.Write(m_pClBlock->m_iTimeOut);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

	iRet = IOD.Write(m_pClBlock->m_iLastLoadStatus);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

	iRet = IOD.Write(m_pClBlock->m_iLastLoadRows);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

	iRet = IOD.Write(m_pClBlock->m_iLastLoadFetchs);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

	return iRet;
	
RETURN_ERROR:
#ifdef _DEBUG   
	if (iRet != M4_SUCCESS) {
		DUMP_CHLOG_DEBUGF(M4_CH_DS_SERIALIZE_PXBLOCK, m_pClNode_Data->GetpExtInfo()->GetHandle() << ID.RegisterId);
	}
#endif
	return iRet;
}

m4return_t ClBlock_Channel_Proxy::_SerializeRecord(ClGenericIODriver&      IOD,
                                                   const m4uint8_t         ai_iRecSerMod,
                                                   const m4uint8_t         ai_iIteSerMod,
                                                   ClChannelProcess* const ai_pChannelProcess,
						                           ClCsTraceSize*    const ai_pCsTraceSize,
                                                   const regindex_t        ai_RecordIndex,
                                                   const m4bool_t          ai_bThereIsAnyChange,
                                                   const m4uint16_t        ai_iNumChildNodesRB,
						                           const m4uint8_t         ai_iVectorDelta,
												   m4bool_t                ai_bWritePLFileMap)
{
	m4return_t	 iRet = M4_SUCCESS;
	m4uint16_t	 i = 0;
	m4uint8_t	 iNewRecSerMod = ai_iRecSerMod;
	m4bool_t	 bTheRecodIsMarked = M4_TRUE;
	const m4char_t*	 pChildNodeId = 0;
	ClCsTraceSize*	 pCsTraceSizeChild = 0;
	ClNode_Data*	 pchildnode = 0;
	ClBlockReference oBlockRef;
	
	
	if (ai_bThereIsAnyChange == M4_TRUE) {
		
		if (ai_pCsTraceSize) {
			pCsTraceSizeChild = ai_pCsTraceSize->CreateChild();
		}
		
		// Values of the record
		iRet = m_pClBlock->Records.SerializeRecord(IOD, iNewRecSerMod, ai_iIteSerMod, ai_pChannelProcess, pCsTraceSizeChild, ai_RecordIndex, ai_iVectorDelta);
		if (iRet != M4_SUCCESS) goto RETURN_ERROR;
		
		// Si estoy en serializacion avanzada solo tendre que serializar 
		// los bloques hijos si el regsitro padre es valido y esta marcado
		m4bool_t bBranchMode = M4_FALSE;
		if (iNewRecSerMod == M4CL_RECORD_SERIAL_ADVANCED && m_iNeedSerial == M4_PXB_SEND_SOME_RECORDS) {
			m_pClBlock->Records.GetNeedSerial(ai_RecordIndex, bTheRecodIsMarked, bBranchMode);
		}
	}
	
	// El recorrido por los hijos es independiente de si ha habido cambios o no en los registros
	for (i = 0; i < ai_iNumChildNodesRB; i ++) {
		pchildnode = m_pClNode_Data->GetpExtInfo()->ChildNodeRB[(ClChildIndex)i].GetAddress();
		
		// TODO: Se presenta un problema de eficiencia, los RBs de los registros apuntan a los
		// mismos nodos, luego si un nodo para un RB de un registro no es mapped, tampoco lo sera
		// para el mismo RB del siguiente registro, ...
		
		// Comprobamos si hay que serializar el nodo hijo
		if (pchildnode->SerializationRequired(m_pClNode_Data, ai_iIteSerMod) == M4_FALSE) {
			continue;
		}
		
		// Si estoy en serializacion avanzada solo serializo el bloque
		// hijo si el registro padre es valido y esta marcado
		if (m_pClBlock->Records.IsRecordValid(ai_RecordIndex) == M4_FALSE || bTheRecodIsMarked == M4_FALSE) {
			iRet = IOD.Write((m4uint8_t)PX_BLOCK_IS_INVALID);
			if (iRet != M4_SUCCESS) goto RETURN_ERROR;
			continue;
		}
		
		// Conseguimos el bloque hermano
		pchildnode->GetpBlockStore()->Get(ID.BlockUniqueKey, ai_RecordIndex, oBlockRef);
		
		// Ahora puede ser cero
		if (oBlockRef.IsNull() == M4_FALSE) {
			iRet = IOD.Write((m4uint8_t)PX_BLOCK_EXIST);
			if (iRet != M4_SUCCESS) goto RETURN_ERROR;
			
			// Tengo que comprobar si tengo que activar el modo branch de la serializacion avanzada 
			iNewRecSerMod = pchildnode->CheckBranchMode(iNewRecSerMod);
			
			if (ai_pCsTraceSize) {
				pChildNodeId = pchildnode->GetpNodeDef()->Id();
				pCsTraceSizeChild = ai_pCsTraceSize->GetpParent(ClCsTraceSize::CHANNEL)->CreateChild(pChildNodeId);
			}
			
			iRet = oBlockRef->Serialize(IOD, iNewRecSerMod, ai_iIteSerMod, ai_pChannelProcess, pCsTraceSizeChild, ai_iVectorDelta, ai_bWritePLFileMap);
			if (iRet != M4_SUCCESS) goto RETURN_ERROR;
		} else {
			iRet = IOD.Write((m4uint8_t)PX_BLOCK_NO_EXIST);
			if (iRet != M4_SUCCESS) goto RETURN_ERROR;
		}
	}
	
	return iRet;
	
RETURN_ERROR:
#ifdef _DEBUG
	if (iRet != M4_SUCCESS) {
		DUMP_CHLOG_DEBUGF(M4_CH_DS_SERIALIZE_PXBLOCK, m_pClNode_Data->GetpExtInfo()->GetHandle() << ID.RegisterId);
	}
#endif
	return iRet;
}

m4return_t ClBlock_Channel_Proxy::DeSerialize(ClGenericIODriver&   IOD,
                                              const m4uint8_t      ai_iIteSerMod,
											  ClCsTraceSize* const ai_pCsTraceSize,
											  const m4uint8_t      ai_iVectorDeltaCount)
{
	ClCsTraceSize* pCsTraceSizeChild = 0;
	if (ai_pCsTraceSize) {
		pCsTraceSizeChild = ai_pCsTraceSize->CreateChild();
	}
    
	m4bool_t bAux;
    m4return_t iRet = IOD.Read(bAux);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

	if( bAux == M4_TRUE )
	{
		SetChange();
	}

    iRet = IOD.Read(bAux);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;
    m_bLoaded = bAux;

	iRet = IOD.Read(bAux);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;
    m_bCheckPointMode = bAux;

	iRet = IOD.Read(m_CurrentSearchIndexId);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

    iRet = IOD.Read(bAux);      // Deserializo si existe el bloque
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;
    if (bAux == M4_TRUE) {
		// Comprobamos un caso de error un poco rarito (se supone que solo le puede pasar a Dani
		// con el canal de estadisticas, ya que puede vivir en el server sin ser proxy)
		// Si estando en serializacion avanzada no marco un bloque para enviar y
		// a la vuelta si me viene, tengo que dar un error
		if (m_bBlockNotSentBecauseMarks == M4_TRUE) {
			ClCompiledMCR* poCMCR = m_pClNode_Data->GetpChannel_Data()->GetpChannelDef()->GetpCMCR();
			m4uint32_t hNode = m_pClNode_Data->GetpExtInfo()->GetHandle();
			DUMP_CHLOG_ERRORF(M4_CH_DS_ADV_SERIAL_BLOCK_NOT_MARKED, M4ObjNodeN(poCMCR, hNode));
			iRet = M4_ERROR;
			goto RETURN_ERROR;
		}

		iRet = _DeSerializeNT(IOD, ai_iIteSerMod, pCsTraceSizeChild, ai_iVectorDeltaCount);
		if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	} else {
        // CRG Destruccion del bloque en caso necesario:
        // Cuando el fuente no existe y fue enviado por mi inicialmente
        if (m_pClBlock != 0 && m_bBlockNotSentBecauseMarks == M4_FALSE) {
            DestroyBlock(M4_TRUE); // Notify, destroy all my sons !!!
		}
    }
	
	return iRet;

RETURN_ERROR:
#ifdef _DEBUG    
	DUMP_CHLOG_DEBUGF(M4_CH_DS_DESERIALIZE_PXBLOCK, m_pClNode_Data->GetpExtInfo()->GetHandle() << ID.RegisterId);
#endif
    return iRet;
}

m4return_t ClBlock_Channel_Proxy::_DeSerializeNT(ClGenericIODriver&   IOD,
                                                 const m4uint8_t      ai_iIteSerMod,
						 ClCsTraceSize* const ai_pCsTraceSize,
						 const m4uint8_t      ai_iVectorDeltaCount)
{
	ClPartialLoadData* pPartialLoadData = 0;
	ClNotifier_Channel_Data* pNotifier = 0;
	m4bool_t bExist = M4_FALSE;
	m4bool_t bThereIsAnyChange;
	m4bool_t bAreDeltas;
	m4uint8_t iExistPxBlock = 0;
	m4uint8_t iLastLoadStatus = 0;
	m4uint32_t iLastLoadRows = 0;
	m4uint32_t iLastLoadFetchs = 0;
	regindex_t i = 0;
	regindex_t iNumRecords = 0;
	m4uint16_t iChildCount = m_pClNode_Data->GetpExtInfo()->ChildNodeBB.Count();
	ClNode_Data* pchildnode = 0;
	ClBlockReference oBlockRef;
	const m4char_t* pChildNodeId = 0;
	
	// Si el bloque ya existe es que estoy deserializadeo deltas,
	// sino es que son absolutos y tengo que crear un bloque nuevo
	if (!m_pClBlock) {
		if (!_CreateBlock()) return M4_ERROR;    //aquí se inicializan los hermanos BB, etc...
	} else {
		bExist = M4_TRUE;
	}
	
	M4_ASSERT (m_pClBlock);
	
	// BLOCK LEVEL -------------------------------------------------------
	
	// Hago la deserializacion en dos partes para saber si tengo que destruir el bloque
	m4return_t iRet = m_pClBlock->Block.DeSerializeFirstStaticPart(IOD);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	
	// Si tengo que deserializar un absoluto y ya tenia bloque, tengo que destruirlo a el y sus hijos
	bAreDeltas = m_pClBlock->Block.IsDeSerializingDeltas();
	if (bAreDeltas == M4_FALSE && bExist == M4_TRUE) {
		// No quiero perder los valores del proxy que ya he deserializado
		m4bool_t bLoaded = m_bLoaded;
		m4bool_t bCheckPointMode = m_bCheckPointMode;
		
		iRet = DestroyBlock();
		if (iRet != M4_SUCCESS) goto RETURN_ERROR;
		
            if (!_CreateBlock()) return M4_ERROR;
		m_bLoaded = bLoaded;
		m_bCheckPointMode = bCheckPointMode;
	}
	
	iRet = m_pClBlock->Block.DeSerializeSecondStaticPart(IOD, bThereIsAnyChange);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	
	if (bThereIsAnyChange == M4_TRUE) {
		// Values of the record
		ClDeltaInfo oDeltaInfo;
		iRet = m_pClBlock->Block.DeSerializeRecord(IOD, ai_iIteSerMod, 0, oDeltaInfo, ai_pCsTraceSize);
		if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	}
	
	// El recorrido por los hijos es independiente de si ha habido cambios o no en los registros
	for (i = 0; i < iChildCount && iRet == M4_SUCCESS; i ++) {
		pchildnode = m_pClNode_Data->GetpExtInfo()->ChildNodeBB[(ClChildIndex)i].GetAddress();
		
		// Comprobamos si hay que deserializar el nodo hijo
		if (pchildnode->SerializationRequired(m_pClNode_Data, ai_iIteSerMod) == M4_FALSE) {
			continue;
		}
		
		iRet = IOD.Read(iExistPxBlock);
		if (iRet != M4_SUCCESS) goto RETURN_ERROR;
		
		if (iExistPxBlock == PX_BLOCK_EXIST) {
			// Conseguimos el bloque hermano
			pchildnode->GetpBlockStore()->Get(ID.BlockUniqueKey, 0, oBlockRef, ID.BlockId, ID.RegisterId);
			
			ClCsTraceSize* pCsTraceSizeChild = 0;
			if (ai_pCsTraceSize) {
				pChildNodeId = pchildnode->GetpNodeDef()->Id();
				pCsTraceSizeChild = ai_pCsTraceSize->GetpParent(ClCsTraceSize::CHANNEL)->CreateChild(pChildNodeId);
			}
			
			iRet = oBlockRef->DeSerialize(IOD, ai_iIteSerMod, pCsTraceSizeChild, ai_iVectorDeltaCount);
			if (iRet != M4_SUCCESS) goto RETURN_ERROR;
		}
	}
	
	m_pClBlock->Block.EndDeSerialize(ai_iVectorDeltaCount);
	
	// RECORDS LEVEL -------------------------------------------------------
	
	iRet = m_pClBlock->Records.DeSerializeStaticPart(IOD, bThereIsAnyChange);

	if (iRet == M4_VRB_ERROR_ROWS_QUOTA_EXCEEDED)
	{
		ClHandle oNodeHandle = m_pClNode_Data->GetpNodeDef()->GetHandle();
		ClCompiledMCR * pmcr  = m_pClNode_Data->GetpNodeDef()->GetpCMCR ();
		DUMP_CHLOG_ERRORF(M4_CH_DM_ROWS_QUOTA_EXCEEDED, M4ObjNodeN(pmcr, oNodeHandle));
	}

	if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	
	iNumRecords = m_pClBlock->Records.GetCount();
	pNotifier = m_pClNode_Data->GetpChannel_Data()->GetpNotifier();
	bAreDeltas = m_pClBlock->Records.IsDeSerializingDeltas();
	
	if (bThereIsAnyChange == M4_TRUE && bAreDeltas == M4_TRUE) {
		if (iNumRecords > 0) {
			//
			// Necesito preparar los registros y sus hijos
			//
			regindex_t  iNumOfDeletedIndex;
			m4uint32_t* vMovedIndex = (m4uint32_t*)IOD.GetTempBuffer(iNumRecords * sizeof(m4uint32_t));
			
			if (vMovedIndex == 0) {
				iRet = M4_ERROR;
				goto RETURN_ERROR;
			}
			
			m4bool_t bArrange;
			
			iRet = m_pClBlock->Records.GetDeltaInfo(vMovedIndex, iNumOfDeletedIndex, bArrange);
			if (iRet != M4_SUCCESS) goto RETURN_ERROR;
			
			// 1-. Reordeno los registros, los hijos RB, y todo lo que haga falta reordenar ...
			if (bArrange == M4_TRUE) {
				_UpdateIndex(vMovedIndex, iNumRecords);
				// Actualizamos los accesos del canal
				oBlockRef.Set(this);
				pNotifier->Arrange_After(m_pClNode_Data, oBlockRef, vMovedIndex);
			}
			
			// 2-. Borro los registros que me sobran
			// No se por que los borro desde el final, creo que lo he visto en alguna parte.
			regindex_t iFirstToDelete = iNumRecords - iNumOfDeletedIndex;
			for (i = iNumRecords - 1; i >= iFirstToDelete; i--) {
				iRet = R_Destroy(i);
				if (iRet != M4_SUCCESS) goto RETURN_ERROR;
			}
			
			// 3-. Inserto los nuevos registros
			//for (regindex_t i; i < NumOfInsertedIndex; i++) {
			//   if (R_InsertRegister(vInsertedIndex[i]) != M4_SUCCESS)
			//      return M4_ERROR;
			//}
			
			// Desues de esto supongo que el VRB ha quedado listo para la deserializacion de los registros
		}
		iNumRecords = m_pClBlock->Records.GetCountDelta();
	}
	
	iChildCount = m_pClNode_Data->GetpExtInfo()->ChildNodeRB.Count();
	
	if (bThereIsAnyChange == M4_TRUE || iChildCount > 0) {
		for (regindex_t ri = 0; ri < iNumRecords; ri++) {
			iRet = _DeSerializeRecord(IOD, ai_iIteSerMod, ri, bThereIsAnyChange, iChildCount, bAreDeltas, pNotifier, ai_pCsTraceSize, ai_iVectorDeltaCount);
			if (iRet != M4_SUCCESS) goto RETURN_ERROR;
		}
	}
	
	m_pClBlock->Records.EndDeSerialize(ai_iVectorDeltaCount);
	
	// PARTIAL LOAD -------------------------------------------------------
	
	// Deserialize de partial load info
	// CRG Estoy suponiendo que nadie comparte estos ebjetos entre bloques ???
	iRet = IOD.Read(bExist);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	
	if (bExist == M4_TRUE) {
		if (!m_pClBlock->m_pClPL_Data) {
			pPartialLoadData = new ClPartialLoadData(0);
			m_pClBlock->m_pClPL_Data = pPartialLoadData;
		}
		
		iRet = m_pClBlock->m_pClPL_Data->DeSerialize(IOD);
		if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	} else {
		// TODO: puede pasar que ya tenga uno??? M4_ASSERT(m_pClBlock->m_pClPL_Data);
		m_pClBlock->m_pClPL_Data = 0;
	}

	iRet = IOD.Read(m_pClBlock->m_iRangeSize);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

	iRet = IOD.Read(m_pClBlock->m_iRangeStart);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

	iRet = IOD.Read(m_pClBlock->m_iTimeOut);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

	iRet = IOD.Read(iLastLoadStatus);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

	iRet = IOD.Read(iLastLoadRows);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

	iRet = IOD.Read(iLastLoadFetchs);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

	m_pClBlock->SetLastLoadStatus(iLastLoadStatus, iLastLoadRows, iLastLoadFetchs);

	return iRet;
	
RETURN_ERROR:
	if (pPartialLoadData) {
		delete m_pClBlock->m_pClPL_Data;
		m_pClBlock->m_pClPL_Data = 0;
	}
#ifdef _DEBUG
	DUMP_CHLOG_DEBUGF(M4_CH_DS_DESERIALIZE_PXBLOCK, m_pClNode_Data->GetpExtInfo()->GetHandle() << ID.RegisterId);
#endif
	return iRet;
}

m4return_t ClBlock_Channel_Proxy::_DeSerializeRecord(ClGenericIODriver&             IOD,
                                                     const m4uint8_t                ai_iIteSerMod,
                                                     const regindex_t               ai_RecordIndex,
                                                     const m4bool_t                 ai_bThereIsAnyChange,
                                                     const m4uint16_t               ai_iNumChildNodesRB,
                                                     const m4bool_t                 ai_bDeSerializingDeltas,
                                                     ClNotifier_Channel_Data* const ai_pNotifier,
													 ClCsTraceSize*           const ai_pCsTraceSize,
													 const m4uint8_t                ai_iVectorDeltaCount)
{
	m4return_t iRet = M4_SUCCESS;
	ClBlockReference oBlockRef;
	ClNode_Data* pchildnode = 0;
	regindex_t i = 0;
	ClCsTraceSize* pCsTraceSizeChild = 0;
	
	if (ai_bThereIsAnyChange == M4_TRUE) {
		
		if (ai_pCsTraceSize) {
			pCsTraceSizeChild = ai_pCsTraceSize->CreateChild();
		}
		
		ClDeltaInfo oDeltaInfo;
		
		// Values of the record
		iRet = m_pClBlock->Records.DeSerializeRecord(IOD, ai_iIteSerMod, ai_RecordIndex, oDeltaInfo, pCsTraceSizeChild);

		if (iRet == M4_VRB_ERROR_ROWS_QUOTA_EXCEEDED)
		{
			ClHandle oNodeHandle = m_pClNode_Data->GetpNodeDef()->GetHandle();
			ClCompiledMCR * pmcr  = m_pClNode_Data->GetpNodeDef()->GetpCMCR ();
			DUMP_CHLOG_ERRORF(M4_CH_DM_ROWS_QUOTA_EXCEEDED, M4ObjNodeN(pmcr, oNodeHandle));

			return iRet;
		}

		if (iRet != M4_SUCCESS) goto RETURN_ERROR;

		if (ai_bDeSerializingDeltas == M4_TRUE && oDeltaInfo.IsInserted()) {
			// Solo tengo que notificar a los accesos la insercion y reordenar los proxys de los
			// hijos si estoy deserializando deltas.
			// En principio no parece necesario para el caso de absolutos ya que todo lo que
			// viene es nuevo
			
			//Notificación:
			// utilizamos esta función para que todos los current y reg de todos los
			// access existentes se enteren de que se han movido (sólo si apuntaban
			// a un registro mayor o igual al del Index actual): se mueven al siguiente
			// para que no se enteren del cambio
			// se mueven todos excepto yo mismo que me quedo apuntando a Index:
			
			oBlockRef.Set(this);
			ai_pNotifier->InsertRegister_After(m_pClNode_Data, oBlockRef, ai_RecordIndex);
			
			//que se actualizen los parentindex de los bloques hijos RB avisamos al store:
			for (i = 0; i < ai_iNumChildNodesRB; i++) {
				pchildnode = m_pClNode_Data->GetpExtInfo()->ChildNodeRB[(ClChildIndex)i].GetAddress();
				M4_ASSERT (pchildnode);
				
				pchildnode->GetpBlockStore()->Notify_ParentAddRegister(ID.BlockUniqueKey, ai_RecordIndex);
			}
		}
	}
	
	// El recorrido por los hijos es independiente de si ha habido cambios o no en los registros
	m4uint8_t iExistPxBlock;
	const m4char_t* pChildNodeId;
	
	for (i = 0; i < ai_iNumChildNodesRB && iRet == M4_SUCCESS; i ++) {
		pchildnode = m_pClNode_Data->GetpExtInfo()->ChildNodeRB[(ClChildIndex)i].GetAddress();
		
		// ??? Se presenta un problema de eficiencia, los RBs de los registros apuntan a los
		// mismos nodos, luego si un nodo para un RB de un registro no es mapped, tampoco lo sera
		// para el mismo RB del siguiente registro, ...
		
		// Comprobamos si hay que deserializar el nodo hijo
		if (pchildnode->SerializationRequired(m_pClNode_Data, ai_iIteSerMod) == M4_FALSE)
			continue;
		
		iRet = IOD.Read(iExistPxBlock);
		if (iRet != M4_SUCCESS) goto RETURN_ERROR;
		
		switch (iExistPxBlock) {
		case PX_BLOCK_EXIST:
			if (ai_pCsTraceSize) {
				pChildNodeId = pchildnode->GetpNodeDef()->Id();
				pCsTraceSizeChild = ai_pCsTraceSize->GetpParent(ClCsTraceSize::CHANNEL)->CreateChild(pChildNodeId);
			}
			
			// Conseguimos el bloque hermano
			pchildnode->GetpBlockStore()->Get(ID.BlockUniqueKey, ai_RecordIndex, oBlockRef, ID.BlockId, ID.RegisterId);
			iRet = oBlockRef->DeSerialize(IOD, ai_iIteSerMod, pCsTraceSizeChild, ai_iVectorDeltaCount);
			if (iRet != M4_SUCCESS) goto RETURN_ERROR;
			break;
			
		case PX_BLOCK_NO_EXIST:
			// Puede ser que el proxy se borrase en el otro extremo
			// Lo pido (sin creacion), y si existe lo borro
			
			// Conseguimos el bloque hermano
			pchildnode->GetpBlockStore()->Get(ID.BlockUniqueKey, ai_RecordIndex, oBlockRef);
			if (oBlockRef.IsNull() == M4_FALSE) {
				R_DestroyRBChilds(ai_RecordIndex);
			}
			break;
			
		case PX_BLOCK_IS_INVALID:
			// El bloque es invalido, no tego que hacer nada
			break;
			
		default:
			M4_ASSERT(0);
			iRet = M4_ERROR;
			goto RETURN_ERROR;
		}
	} 
	
	return iRet;
	
RETURN_ERROR:
	DUMP_CHLOG_ERRORF(M4_CH_DS_DESERIALIZE_PXBLOCK, m_pClNode_Data->GetpExtInfo()->GetHandle() << ID.RegisterId);
	return iRet;
}

m4return_t ClBlock_Channel_Proxy::SetKeepStatus(const m4bool_t ai_bKeepIt, const m4uint8_t ai_iVectorCount)
{
   if (m_pClBlock)
      return m_pClBlock->SetKeepStatus(ai_bKeepIt, ai_iVectorCount);

   return M4_SUCCESS;
}

//
// Serializacion avanzada: nos permite marcar concretamente que es lo que se va a a serializar
//

void ClBlock_Channel_Proxy::ResetSerialMarks(void)
{
    // Hago un reset de todos los registros del bloque
    // Puedo optimizar: si el bloque no esta marcado no tengo que intentar desmarcar ningun registro
    if (m_pClBlock && m_iNeedSerial != M4_PXB_NO_SEND)
        m_pClBlock->ResetSerialMarks();
    
    // Hago un reset del bloque
    m_iNeedSerial = M4_PXB_NO_SEND;
    m_bBranchMode = M4_FALSE;
}

void ClBlock_Channel_Proxy::FindRecordAndSetVersion(const m4uint32_t ai_iRecordId,
                                                    const m4date_t   ai_dNewVersion,
                                                    m4bool_t&        ao_bContinue)
{
    if (m_pClBlock != NULL)
    {
        VersionMark oVersionMark;
        oVersionMark.Date = ai_dNewVersion;
        m_pClBlock->FindRecordAndSetVersion(ai_iRecordId, oVersionMark, ao_bContinue);
    }
}

void ClBlock_Channel_Proxy::GetNeedSerial(m4uint8_t& ai_iNeedSerial,
                                          m4bool_t&  ai_bBranchMode) const
{
   ai_iNeedSerial = m_iNeedSerial;
   ai_bBranchMode = m_bBranchMode;
}

void ClBlock_Channel_Proxy::SetNeedSerial(const m4uint8_t ai_iNeedSerial,
                                          const m4bool_t  ai_bBranchMode)
{
   m_iNeedSerial = ai_iNeedSerial;
   m_bBranchMode = ai_bBranchMode;
}

void ClBlock_Channel_Proxy::TestAndSetNeedSerial(m4uint8_t&      ao_iBeforeNeedSerial,
                                                 m4bool_t&       ao_bBeforeBranchMode,
                                                 const m4uint8_t ai_iNeedSerial,
                                                 const m4bool_t  ai_bBranchMode)
{
   GetNeedSerial(ao_iBeforeNeedSerial, ao_bBeforeBranchMode);
   if (ao_iBeforeNeedSerial != M4_PXB_SEND_ALL_RECORDS)
      SetNeedSerial(ai_iNeedSerial, ai_bBranchMode);
}

/**
 * Marca para serilizar el bloque que se pasa como parámetro. También marca para serializar la jerarquía de padres necesaria para
 * para que el bloque que se está marcando pueda viajar (siempre necesita colgar de alguien). También se puede indicar que viajen
 * todos los descendientes del bloque que se está marcando.
 *
 * @param ai_bBranchMode	Booleano que indica si se quiere que viajen también todos los descendientes del registro que se está marcando.
 * @param ai_iNeedSerial	Mode en que se marca el bloque. Puede tomar los siguientes valores:
 *								M4_PXB_NO_SEND				= El bloque no esta marcado, no hay que enviarlo.
 *								M4_PXB_SEND_SOME_RECORDS	= Hay que enviar los registros que estan marcados.
 *								M4_PXB_SEND_ALL_RECORDS		= Hay que enviar todos los registros del bloque.
 */
void ClBlock_Channel_Proxy::MarkBlockAndParentRecordsForSerialize(const m4bool_t ai_bBranchMode, const m4uint8_t ai_iNeedSerial)
{
    // Marco el bloque para serializar todos los registros
    m4uint8_t iBeforeNeedSerial;
    m4bool_t  bBeforeBranchMode;
    TestAndSetNeedSerial(iBeforeNeedSerial, bBeforeBranchMode, ai_iNeedSerial, ai_bBranchMode);
    
    // Si yo no estaba marcado, marco mi estructura de soporte
    if (iBeforeNeedSerial == M4_PXB_NO_SEND) {
        // Marco el nodo
        m_pClNode_Data->SetNeedSerial(M4_TRUE, M4_FALSE);
        
        // Marco mi registro padre
        if (ExistParentBlock() == M4_TRUE) {
            ClBlockReference oBlockReference;
            GetParentBlock(oBlockReference);
            ClBlock_Channel_Proxy* pParentProxy = oBlockReference.GetpProxy();
            
            // Una ultima comprobacion para ver si esta encarrialdo
            if (m_pClNode_Data->GetpChannel_Data() == pParentProxy->GetpNode_Data()->GetpChannel_Data()) {
                pParentProxy->MarkRecordAndParentRecordsForSerialize(ID.RegisterId, M4_FALSE);
			}
        }
    }
}

/**
 * Marca para serilizar el registro que se pasa como parámetro. También marca para serializar la jerarquía de padres necesaria para
 * para que el registro que se está marcando pueda viajar (siempre necesita colgar de alguien). También se puede indicar que viajen
 * todos los descendientes del registro que se está marcando.
 * Si el bloque no existe (ej. porque todavía no se ha creado) dejará un warning en la log.
 *
 * @param ai_iRecordToMark	Índice del registro que se quiere marcar.
 * @param ai_bBranchMode	Booleano que indica si se quiere que viajen también todos los descendientes del registro que se está marcando.
 */
void ClBlock_Channel_Proxy::MarkRecordAndParentRecordsForSerialize(const regindex_t ai_iRecordToMark, const m4bool_t ai_bBranchMode)
{
    // Marco el registro
    m4bool_t bBeforeNeedSerial = M4_FALSE;
    m4bool_t bBeforeBranchMode = M4_FALSE;
    
	if (!m_pClBlock) {
		const ClNode_Kernel* const pNodeKernel = m_pClNode_Data->GetpNodeDef();
		DUMP_CHLOG_WARNINGF(M4_CH_DS_ADV_SERIAL_BLOCK_DONT_EXIST, ai_iRecordToMark << M4ObjNodeN(pNodeKernel->GetpCMCR(), pNodeKernel->GetHandle()));
		return;
	}

    m_pClBlock->Records.TestAndSetNeedSerial(ai_iRecordToMark, bBeforeNeedSerial, bBeforeBranchMode, M4_TRUE, ai_bBranchMode);

    if (bBeforeNeedSerial == M4_FALSE && m_iNeedSerial == M4_PXB_NO_SEND) {
        // Marco el bloque
        m_iNeedSerial = M4_PXB_SEND_SOME_RECORDS;
        
        // Marco el nodo
        m_pClNode_Data->SetNeedSerial(M4_TRUE, M4_FALSE);
        
        // Marco mi registro padre
        if (ExistParentBlock() == M4_TRUE) {
            ClBlockReference oBlockReference;
            GetParentBlock(oBlockReference);
            ClBlock_Channel_Proxy* pParentProxy = oBlockReference.GetpProxy();
            
            // Una ultima comprobacion para ver si esta encarrialdo
            if (m_pClNode_Data->GetpChannel_Data() == pParentProxy->GetpNode_Data()->GetpChannel_Data()) {
                pParentProxy->MarkRecordAndParentRecordsForSerialize(ID.RegisterId, M4_FALSE);
			}
        }
    }
}

m4return_t ClBlock_Channel_Proxy::MarkForPersistSerial(void)
{
	// If the proxy doesn't points to something ... dont mark it
	if (!m_pClBlock) return M4_SUCCESS;
	
	// BLOCK LEVEL -------------------------------------------------------
	
	ClNode_Data* pchildnode = 0;
	ClBlockReference br;
	m4uint8_t iRecordsStatus;
	
	m4return_t iRet = m_pClBlock->Block.Status(iRecordsStatus);
	if (iRet != M4_SUCCESS) return iRet;
	
	m4bool_t iNeedToMarkAnyRecord = M4_BOOL( (iRecordsStatus & VS_DELETED) || (iRecordsStatus & VS_NEW) || (iRecordsStatus & VS_MODIFIED_IN_DB) );
	
	if (iNeedToMarkAnyRecord == M4_TRUE) {
		// Tengo que marcar el bloque, ya que han cambiado items a nivel de bloque,
		// y estos tambien hay que persistirlos
		MarkBlockAndParentRecordsForSerialize(M4_FALSE, M4_PXB_SEND_SOME_RECORDS);
	}
	
	// El recorrido por los hijos es independiente de si ha habido cambios o no en los registros
	m4uint16_t n = m_pClNode_Data->GetpExtInfo()->ChildNodeBB.Count();
	m4uint16_t i;
	for (i = 0; i < n; i ++) {
		pchildnode = m_pClNode_Data->GetpExtInfo()->ChildNodeBB[(ClChildIndex)i].GetAddress();
		M4_ASSERT (pchildnode);
		
		// Comprobamos si hay que marcar el nodo hijo
		if (pchildnode->SerializationRequired(m_pClNode_Data, M4CL_ITEM_SERIAL_MAPPED) == M4_FALSE)
			continue;
		
		// Conseguimos el bloque hermano
		pchildnode->GetpBlockStore()->Get(ID.BlockUniqueKey, 0, br);
		
		// Ahora puede ser cero. Serializamos un m4bool_t indicando esta situacion
		if (!br.IsNull()) {
			iRet = br->MarkForPersistSerial();
			if (iRet != M4_SUCCESS)	return iRet;
		}
	}
	
	// RECORDS LEVEL -------------------------------------------------------

	iRet = m_pClBlock->Records.Status(iRecordsStatus);
	if (iRet != M4_SUCCESS)	return iRet;
	
	iNeedToMarkAnyRecord = M4_BOOL( (iRecordsStatus & VS_DELETED) || (iRecordsStatus & VS_NEW) || (iRecordsStatus & VS_MODIFIED_IN_DB) );
	
	regindex_t NumRecords = m_pClBlock->Records.GetCount();
	
	// El recorrido por los hijos es independiente de si ha habido cambios o no en los registros
	n = m_pClNode_Data->GetpExtInfo()->ChildNodeRB.Count();
	
	m4bool_t iIsFirstMarkedRecord = M4_TRUE;
	m4bool_t iNeedToMarkThisRecord;
	
	if (iNeedToMarkAnyRecord == M4_TRUE || n > 0) {
		for (regindex_t ri = 0; ri < NumRecords; ri++) {
			if (iNeedToMarkAnyRecord == M4_TRUE) {
				iNeedToMarkThisRecord = m_pClBlock->Records.IsRecordModifiedInDB(ri);
				
				if (iNeedToMarkThisRecord == M4_FALSE) {
					iNeedToMarkThisRecord = m_pClBlock->Records.IsRecordNew(ri);
					
					if (iNeedToMarkThisRecord == M4_FALSE) {
						iNeedToMarkThisRecord = m_pClBlock->Records.IsRecordDeleted(ri);
					}
				}
				
				if (iNeedToMarkThisRecord == M4_TRUE) {
					// Values of the record
					if (iIsFirstMarkedRecord == M4_TRUE) {
						MarkRecordAndParentRecordsForSerialize(ri, M4_FALSE);
						iIsFirstMarkedRecord = M4_FALSE;
					} else {
						iRet = m_pClBlock->Records.SetNeedSerial(ri, M4_TRUE, M4_FALSE);
						if (iRet != M4_SUCCESS) return iRet;
					}
				}
			}
			
			// Go to my childs...
			for (i = 0; i < n; i ++) {       
				pchildnode = m_pClNode_Data->GetpExtInfo()->ChildNodeRB[(ClChildIndex)i].GetAddress();
				M4_ASSERT (pchildnode);
				
				// Comprobamos si hay que marcar el nodo hijo
				if (pchildnode->SerializationRequired(m_pClNode_Data, M4CL_ITEM_SERIAL_MAPPED) == M4_FALSE)
					continue;
				
				// Conseguimos el bloque hijos
				pchildnode->GetpBlockStore()->Get(ID.BlockUniqueKey, ri, br);
				
				if (!br.IsNull()) {
					iRet = br->MarkForPersistSerial();
					if (iRet != M4_SUCCESS) return iRet;
				}
			}
		}
	}
	
	// Si no hay ningún registro modificado y el status del VRB dice lo contrario, es que este se
	// equivoca (por ejemplo si inserto un registro, y lo destruyo, el status del VRB sigue pensando
	// que hay alguna insercion). Por esto ponemos el status del VRB a normal
	if (iIsFirstMarkedRecord == M4_TRUE && iRecordsStatus != VS_NORMAL) {
		m_pClBlock->Records.SetStatus(VS_NORMAL);
	}
	
	return iRet;
}

//
// Copy/Paste de una rama de una canal utilizando serializacion
//

m4return_t ClBlock_Channel_Proxy::CopyRecordWithSerialize(const m4uint32_t   ai_iRecordIndex,
                                                          ClGenericIODriver& IOD)
{
    if (m_pClBlock == NULL || ai_iRecordIndex == M4DM_EOF_INDEX)
        return M4_ERROR;

	/*
	Bug 0101806
	Ahora con los autos el keepstatus puede ser true, pero no pasa nada,
	porque en la serialización le estamos diciendo el modo de serialización all

    // Si tengo deltas no puedo copiar, ya que solo se van a serializar los cambios
    if (m_pClBlock->Records.GetKeepStatus() == M4_TRUE)
    {
        M4_ASSERT(0);
        return M4_ERROR;
    }
	*/

    ClChannelDef*  pChannelDef = m_pClNode_Data->GetpChannel_Data()->GetpChannelDef();
    ClNode_Kernel* pNodeDef    = m_pClNode_Data->GetpNodeDef();
    ClCompiledMCR* pCMCR       = pChannelDef->GetpCMCR();

    // Como medida de seguridad al hacer el paste serializo el
    // nombre del canal, version del metacanal, RSM, cs_type, y nombre del nodo
    if (IOD.Write(pChannelDef->Id()) != M4_SUCCESS)
        return M4_ERROR;
    if (IOD.Write(pCMCR->GetChannelVersion()) != M4_SUCCESS)
        return M4_ERROR;
    if (IOD.Write(pChannelDef->CsType()) != M4_SUCCESS)
        return M4_ERROR;
    if (IOD.Write(pCMCR->GetSecurityMaskId()) != M4_SUCCESS)
        return M4_ERROR;
    if (IOD.Write(pNodeDef->Id()) != M4_SUCCESS)
        return M4_ERROR;

    // Necesito serializar el UseCP para saber, al hacer el paste, si este regsitro tenía oldValue
    if (IOD.Write(m_pClBlock->Records.UsingCheckPoints()) != M4_SUCCESS)
        return M4_ERROR;

    // Como es una copia serializo todo (lo maped y lo no mapped)
    return _SerializeRecord(IOD,
                            M4CL_RECORD_SERIAL_ALL, M4CL_ITEM_SERIAL_ALL,
                            NULL, 0, ai_iRecordIndex, M4_TRUE, pNodeDef->ChildNodeRB.Count(), 0, M4_FALSE);
}

m4return_t ClBlock_Channel_Proxy::PasteRecordWithDeSerialize(m4uint32_t&        aio_iRecordIndex,
                                                             const m4bool_t     ai_bKeepMarks,
                                                             const m4bool_t     ai_bDestroyRecords,
                                                             const m4char_t*    ai_vpExclusionNodeName[],
                                                             const m4uint16_t   ai_iNumExclusionNodes,
                                                             ClGenericIODriver& IOD)
{
    m4char_t  pChannelId [M4CL_MAX_T3_ID + 1];
    m4char_t  pNodeId    [M4CL_MAX_NODE_ID + 1];
    m4char_t  pMask      [M4CL_MAX_RSM_ID + 1];

    m4uint8_t iCsType;
    m4date_t  dVersion;

    ClChannel_Data* pChannelData = m_pClNode_Data->GetpChannel_Data();
    ClChannelDef*   pChannelDef  = pChannelData->GetpChannelDef();
    ClNode_Kernel*  pNodeDef     = m_pClNode_Data->GetpNodeDef();
    ClCompiledMCR*  pCMCR        = pChannelDef->GetpCMCR();

    // Como medida de seguridad al hacer el paste serializo el
    // nombre del canal, version del metacanal, RSM, cs_type, y nombre del nodo
    if (IOD.Read(pChannelId) != M4_SUCCESS)
        return M4_ERROR;
    if (strcmp(pChannelId, pChannelDef->Id()) != 0)
        return M4_ERROR;

    if (IOD.Read(dVersion) != M4_SUCCESS)
        return M4_ERROR;
    if (fabs(pCMCR->GetChannelVersion() - dVersion) >= M4_LN4_DATE_PRECISION)
        return M4_ERROR;

    if (IOD.Read(iCsType) != M4_SUCCESS)
        return M4_ERROR;
    if (iCsType != pChannelDef->CsType())
        return M4_ERROR;

    if (IOD.Read(pMask) != M4_SUCCESS)
        return M4_ERROR;
    if (strcmp(pMask, pCMCR->GetSecurityMaskId()) != 0)
        return M4_ERROR;

    if (IOD.Read(pNodeId) != M4_SUCCESS)
        return M4_ERROR;
    if (strcmp(pNodeId, pNodeDef->Id()) != 0)
        return M4_ERROR;
    
    m4bool_t bRecordAdded;

    if (aio_iRecordIndex == M4DM_EOF_INDEX)
    {
        // Tengo que insertar un registro donde hacer al deserializacion
        if (B_AddRegister(aio_iRecordIndex) != M4_SUCCESS)
            return M4_ERROR;

        bRecordAdded = M4_TRUE;
    }
    else
    {
        // Deserializo en un registro existente
        // Tengo que borrar los bloques que hay por debajo ???
        if (m_pClBlock == NULL)
            return M4_ERROR;

        bRecordAdded = M4_FALSE;
    }

    // Necesito serializar el UseCP para saber, al hacer el paste, si este regsitro tenía oldValue
    m4bool_t bUseCPDelta;

    m4return_t iRet = IOD.Read(bUseCPDelta);

    m_pClBlock->Records.SetUseCPDelta(bUseCPDelta);

    // Deserializo la rama
    if (iRet == M4_SUCCESS)
    {
        iRet = _DeSerializeRecord(IOD,
                                  M4CL_ITEM_SERIAL_ALL,
                                  aio_iRecordIndex,
                                  M4_TRUE,
                                  pNodeDef->ChildNodeRB.Count(),
                                  M4_FALSE,
                                  pChannelData->GetpNotifier(),
				  0,
				  0);	// Como los bloques se van a crear nuevos, no necesitan vectores de deltas.
    }

    if (iRet == M4_SUCCESS)
    {
        ClChanRoutHierarchicalDescendent oRout;

        // Tengo que comprobar si tengo que mantener las marcas o ponerlas todas como insertado
        ClChanProcSetRecStatInserted oChanProcSetRecStatInserted(ai_bDestroyRecords);
        if (ai_bKeepMarks == M4_FALSE)
        {
            oChanProcSetRecStatInserted.SetExclusionNodes(ai_vpExclusionNodeName, ai_iNumExclusionNodes);
            oRout.AddProcess(&oChanProcSetRecStatInserted);
        }

        // Siempre tengo que poner nuevos identificadores a los registros copiados
        ClChanProcNewRecordID oChanProcNewRecordID;
        oRout.AddProcess(&oChanProcNewRecordID);

        iRet = oRout.Begin(this, aio_iRecordIndex, NULL);
    }
    
    // Si se añadio una nueva rama y algo ha salido mal, la tengo que borrar
    if (bRecordAdded == M4_TRUE && iRet != M4_SUCCESS)
    {
        iRet = R_Destroy(aio_iRecordIndex);
        aio_iRecordIndex = M4DM_EOF_INDEX;
    }

    return iRet;
}

m4return_t ClBlock_Channel_Proxy::CopyRecordWithSerialize(const m4uint32_t ai_iRecordIndex,
                                                          ClFileName&      ao_oFileName)
{
    ClFileManager* pFileManager = ClFileManager::Instance(); // Singleton
    if (pFileManager->CreateTempFile(ao_oFileName, "dm", NULL, NULL) != M4_SUCCESS)
        return M4_ERROR;

    ClFileIODriver oFileIOD;

    if (oFileIOD.Init(GEN_IO_MODE_WRITE, (m4char_t*) (const m4char_t*) ao_oFileName) != M4_SUCCESS)
        return M4_ERROR;
 
    m4return_t iRet = CopyRecordWithSerialize(ai_iRecordIndex, oFileIOD);

    oFileIOD.End(M4_TRUE);

    return iRet;
}

m4return_t ClBlock_Channel_Proxy::PasteRecordWithDeSerialize(m4uint32_t&       aio_iRecordIndex,
                                                             const m4bool_t    ai_bKeepMarks,
                                                             const m4bool_t    ai_bDestroyRecords,
                                                             const m4char_t*   ai_vpExclusionNodeName[],
                                                             const m4uint16_t  ai_iNumExclusionNodes,
                                                             const ClFileName& ai_oFileName,
                                                             const m4bool_t    ai_bDeleteFile)
{
    ClFileIODriver oFileIOD;

    if (oFileIOD.Init(GEN_IO_MODE_READ, (m4char_t*) (const m4char_t*) ai_oFileName) != M4_SUCCESS)
        return M4_ERROR;

    m4return_t iRet = PasteRecordWithDeSerialize(aio_iRecordIndex,
                                                 ai_bKeepMarks,
                                                 ai_bDestroyRecords,
                                                 ai_vpExclusionNodeName,
                                                 ai_iNumExclusionNodes,
                                                 oFileIOD);

    oFileIOD.End(M4_TRUE);

    if (ai_bDeleteFile == M4_TRUE && iRet == M4_SUCCESS)
    {
        // Tengo que borrar el fichero. Le digo al file manager que ya no tengo interes
        // por el fichero y le dejo que se encargue
        ClFileManager* pFileManager = ClFileManager::Instance(); // Singleton

        iRet = pFileManager->UnSetInterestInFile(ai_oFileName);
    }

    return iRet;
}

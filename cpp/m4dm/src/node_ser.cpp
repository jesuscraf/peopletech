#include "dsres.hpp"

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             ch_node.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             04/09/97
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

#include "node_knl.hpp"

#include "access.hpp"
#include "chan_knl.hpp"
#include "channelprocess.hpp"
#include "chlog.hpp"
#include "computil.hpp"
#include "cstracesize.hpp"
#include "itemsimp.hpp"
#include "m4mdrt.hpp"
#include "rec_tran.hpp"
#include "vdelta.hpp"
#include "m4objglb.hpp"




m4bool_t ClNode_Data::SerializationRequired(const ClNode_Data* const ai_pParentNode,
                                            const m4uint8_t          ai_iIteSerMod) const
{
    // Comprobamos si esta viajando todo el canal o solo la parte mapped
    if (ai_iIteSerMod == M4CL_ITEM_SERIAL_MAPPED)
    {
        if (m_pClNode_Kernel->CsType() != M4CL_CSTYPE_MAPPED)
            return M4_FALSE;
    }
    
    // Comprobamos que no estamos intentando ir desde un nodo
    // de un canal lane a un nodo de un canal doc
    if (m_pClChannel_Data != ai_pParentNode->GetpChannel_Data())
        return M4_FALSE;
    
    return M4_TRUE;
}

/**
 * Comprueba si hay que cambiar el modo de serialización de los registros.
 * Este hay que cambiarlo cuando estoy en serializacion avanzada y en este nodo esta actov el modo branch,
 * de forma que se serializen todos los registros de este nodo.
 *
 * @param ai_iRecSerMod	Modo actual de serialización de los registros, conforme a este modo se toma la decisión de cambiarlo o no.
 * @return	Devuelve el nuevo modo de serialización de registros.
 */
m4uint8_t ClNode_Data::CheckBranchMode(const m4uint8_t ai_iRecSerMod) const
{
	if (ai_iRecSerMod == M4CL_RECORD_SERIAL_ADVANCED && m_bBranchMode == M4_TRUE) {
		return M4CL_RECORD_SERIAL_ALL;
	}
        return ai_iRecSerMod;
}

/**
 * Serializa los items de nodo.
 *
 * @param IOD					IODriver donde serializamos.
 * @param ai_iRecSerMod			Modo de serialización de los registros.
 * @param ai_iIteSerMod			Modo de serialización de los ítems.
 * @param ai_pChannelProcess	Por si hay que hacer algún proceso antes o despues de serializar.
 * @param ai_pCsTraceChannel	Por si hay que guardar información sobre los tamaños de serialización.
 * @param ai_iVectorDelta		Índice del vector de deltas que se tiene que utilizar.
 * @return m4return_t.
 */
m4return_t ClNode_Data::Serialize(ClGenericIODriver&      IOD,
				  const m4uint8_t         ai_iRecSerMod,
				  const m4uint8_t         ai_iIteSerMod,
				  ClChannelProcess* const ai_pChannelProcess,
				  ClCsTraceSize*    const ai_pCsTraceSize,
				  const m4uint8_t         ai_iVectorDelta)
{
	m4return_t ret = M4_ERROR;
	
	if (ai_pChannelProcess) {
		ret = ai_pChannelProcess->Pre_NodeData_Process(this);
		if (ret != M4_SUCCESS) {
			DUMP_CHLOG_DEBUGF(M4_CH_DS_PRE_PROCESS_NODECHANNEL, M4ObjNodeN(m_pClChannel_Data->GetpChannelDef()->GetpCMCR(), m_pClNode_Kernel->GetHandle()));
			return ret;
		}
	}

	ClCsTraceSize* pCsTraceSizeChild = 0;
	if (ai_pCsTraceSize) {
		pCsTraceSizeChild = ai_pCsTraceSize->CreateChild(GetpNodeDef()->Id());
	}
        
	// Serialize the operation
	ret = Operation.Serialize(IOD);
	if (ret != M4_SUCCESS) goto RETURN_ERROR;
        
	// Serialize the current value
	m4bool_t bIsThereAnyChange;
	
	ret = m_poRecordWithTransaction->SerializeStaticPart(IOD, ai_iRecSerMod, ai_pChannelProcess, bIsThereAnyChange, ai_iVectorDelta);
	if (ret != M4_SUCCESS)  goto RETURN_ERROR;
	
	if (bIsThereAnyChange == M4_TRUE) {
		m4uint8_t iNewRecSerMod = ai_iRecSerMod;
		ret = m_poRecordWithTransaction->SerializeRecord(IOD, iNewRecSerMod, ai_iIteSerMod, ai_pChannelProcess, pCsTraceSizeChild, 0, ai_iVectorDelta);
		if (ret != M4_SUCCESS) goto RETURN_ERROR;
	}
	
	if (ai_pChannelProcess) {
		ret = ai_pChannelProcess->Post_NodeData_Process(this);
		if (ret != M4_SUCCESS) {
			DUMP_CHLOG_DEBUGF(M4_CH_DS_POST_PROCESS_NODECHANNEL, M4ObjNodeN(m_pClChannel_Data->GetpChannelDef()->GetpCMCR(), m_pClNode_Kernel->GetHandle()));
			return ret;
		}
	}
	
	return ret;
	
RETURN_ERROR:
	DUMP_CHLOG_DEBUGF(M4_CH_DS_SERIALIZE_NODECHANNEL, M4ObjNodeN(m_pClChannel_Data->GetpChannelDef()->GetpCMCR(), m_pClNode_Kernel->GetHandle()));
	return ret;
}

/**
 * Deserializa los ítems a nivel de nodo.
 * Author: Stuart Murdoch (mailto:stuartm@meta4.es)
 * Created: 5 December 1998
 *
 * @param IOD				IODriver desde el que hacemos la deserialización (las lecturas).
 * @param ai_iIteSerMod		Modo de serializacion de los ítems.
 * @param ai_pCsTraceSize	Por si hay que guardar información sobre los tamaños de deserialización.
 * @return m4return_t.
 */
m4return_t ClNode_Data::DeSerialize(ClGenericIODriver&   IOD,
                                    const m4uint8_t      ai_iIteSerMod,
				    ClCsTraceSize* const ai_pCsTraceSize,
				    const m4uint8_t      ai_iVectorDeltaCount)
{
	m4bool_t bIsThereAnyToDeSerialize = M4_FALSE;
	ClCsTraceSize* pCsTraceSizeChild = 0;
	if (ai_pCsTraceSize) {
		pCsTraceSizeChild = ai_pCsTraceSize->CreateChild(GetpNodeDef()->Id());
	}
	
	// Deserialize the operation
	m4return_t iRet = Operation.DeSerialize(IOD);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	
	// DeSerialize the current value
	iRet = m_poRecordWithTransaction->DeSerializeStaticPart(IOD, bIsThereAnyToDeSerialize);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	
	if (bIsThereAnyToDeSerialize == M4_TRUE) {
		ClDeltaInfo oDeltaInfo;
		
		iRet = m_poRecordWithTransaction->DeSerializeRecord(IOD, ai_iIteSerMod, 0, oDeltaInfo, pCsTraceSizeChild);
		if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	}
	

RETURN_ERROR:

	/* Bug 0075038
	Siempre hay que hacer el EndDeSerialize aunque falle la deserialización.
	*/
	m4return_t	iRet2 = m_poRecordWithTransaction->EndDeSerialize(ai_iVectorDeltaCount);

	if( iRet != M4_SUCCESS || iRet2 != M4_SUCCESS )
	{
		DUMP_CHLOG_DEBUGF(M4_CH_DS_DESERIALIZE_NODECHANNEL, M4ObjNodeN(m_pClChannel_Data->GetpChannelDef()->GetpCMCR(), m_pClNode_Kernel->GetHandle()));
		return( M4_ERROR ) ;
	}
	return M4_SUCCESS;
}

m4return_t ClNode_Data::InheritConnectedData(ClNode_Data* ai_poParentNode)
{
    ClNoCallableItem Item;
    ClNoCallableItem ParentItem;
    
    if (ai_poParentNode != NULL)
    {
        Item.Attach(this);
        //		ParentItem.Attach(ai_poParentNode);

        Item.SetMode(M4DM_SCOPE_NODE, M4DM_TYPE_PROPERTY | M4DM_TYPE_FIELD | M4DM_TYPE_CONCEPT);  

        Item.Begin();

        m4VariantType Var;

        // Code to solve the problem of the change of status in the assignment (CRG)
        m4uint8_t iVectorStatus;
        RegisterStatus_t iRecordStatus = m_poRecordWithTransaction->RecordStatus(0);
        m_poRecordWithTransaction->Status(iVectorStatus);
        // End of patch (CRG)

        while (Item.GetIndex() != M4DM_EOF_INDEX)
        {
            if (Item.GetpConnectItemInfo()->IsConnected()) 
            {
                //está conectado:
                
                ParentItem.Attach(Item.GetpConnectItemInfo()->Parent.GetAddress());
                // Reading from the father
                // Getting the parent item index
                ParentItem[Item.GetpConnectItemInfo()->Parent.GetItemIndex()];
                
                // The parent must be a node scope item
                if (ParentItem.ItemDef.Type() != M4CL_ITEM_TYPE_METHOD)
                {
					m4uint8_t itemScope       = Item.ItemDef.Scope();
					m4uint8_t parentItemScope = ParentItem.ItemDef.Scope();

					if (parentItemScope != M4CL_ITEM_SCOPE_NODE) {
						ClCompiledMCR* pCMVR       = Item.GetpNode_Data()->GetpChannel_Data()->GetpChannelDef()->GetpCMCR();
						ClCompiledMCR* pParentCMVR = ParentItem.GetpNode_Data()->GetpChannel_Data()->GetpChannelDef()->GetpCMCR();

						ClHandle itemHandle       = Item.ItemDef.GetHandle();
						ClHandle parentItemHandle = ParentItem.ItemDef.GetHandle();

						DUMP_CHLOG_ERRORF(M4_CH_DM_BAD_ITEM_CONECTOR, M4ObjNodeItemI(pParentCMVR, parentItemHandle) << M4ClItemScope(parentItemScope) << M4ObjNodeItemI(pCMVR, itemHandle) << M4ClItemScope(itemScope));
						return M4_ERROR;
					}
                    
                    ParentItem.Value.Get(Var); //puede devolver un warning por temas de seguridad, no pasa nada            
                    
                    Item.Value._SetNoNotify(Var, M4_FALSE);
                }
            }
            Item.Next();
        }

        // Code to solve the problem of the change of status in the assignment (CRG)
		/*
		Bug 0084826
		Ahora esto sobra porque no se modifica el estado
        m_poRecordWithTransaction->SetStatus(0, iRecordStatus);
        m_poRecordWithTransaction->SetStatus(iVectorStatus);
		*/
        // End of patch (CRG)
    }

    ClNode_Data* pChildNode = 0;

    // First, BB sons
    m4uint16_t iNumChildNodes = GetpExtInfo()->ChildNodeBB.Count();
    m4uint16_t iNodeIndex;
    for (iNodeIndex = 0; iNodeIndex < iNumChildNodes; iNodeIndex++)
    {
        pChildNode = GetpExtInfo()->ChildNodeBB[(ClChildIndex)iNodeIndex].GetAddress();
        
        if (pChildNode->InheritConnectedData(this) != M4_SUCCESS)
            return M4_ERROR;
    }
    
    // And RB sons
    iNumChildNodes = GetpExtInfo()->ChildNodeRB.Count();
    
    for (iNodeIndex = 0; iNodeIndex < iNumChildNodes; iNodeIndex++)
    {
        pChildNode = GetpExtInfo()->ChildNodeRB[(ClChildIndex)iNodeIndex].GetAddress();
        
        if (pChildNode->InheritConnectedData(this) != M4_SUCCESS)
            return M4_ERROR;
    }
    
    return M4_SUCCESS;
}

m4return_t ClNode_Data::SetKeepStatus(const m4bool_t ai_bKeepIt, const m4uint8_t ai_iVectorCount)
{
    m4return_t res;

	ClKeepStatusInfo keepStatusInfo(ai_bKeepIt, ai_iVectorCount);

    res = ClCStoreUtilities::ForEach(m_pBlockStore, ClCStoreUtilities::SetKeepStatusOne, (void *)&keepStatusInfo);

    if (res == M4_SUCCESS)
        res = m_poRecordWithTransaction->SetKeepStatus (ai_bKeepIt, ai_iVectorCount);

    return res;
}

void ClNode_Data::ResetSerialMarks(void)
{
    // Si el nodo no es mapped no hace falta que haga reset de las marcas
    // Si el nodo no está marcado no tiene ningun bloque marcado
    if (m_bNeedSerial == M4_TRUE && m_pClNode_Kernel->CsType() == M4CL_CSTYPE_MAPPED)
    {
        m_bNeedSerial = M4_FALSE;
        m_bBranchMode = M4_FALSE;  

        ClCStoreUtilities::ForEach(m_pBlockStore, ClCStoreUtilities::ResetSerialMarks, NULL);
    }
}

void ClNode_Data::MarkNodeAndParentRecordsForSerialize(const m4bool_t ai_bBranchMode)
{
    // Una primera version marcara todos los registros para que viajen todos los bloques
    // Esto tiene el problema de que si despues de marcar el nodo se añade un nuevo bloque
    // este estara sin marcar, con lo que no viajara al servidor
    // Para eliminar esta carencia habra que cambiar la serializacion avanzada

    if (m_pClNode_Kernel->CsType() == M4CL_CSTYPE_MAPPED)
        ClCStoreUtilities::ForEach(m_pBlockStore, 
                                   ClCStoreUtilities::MarkBlockAndParentRecordsForSerialize,
                                   (void*)&ai_bBranchMode);
}

void ClNode_Data::GetNeedSerial(m4bool_t& ao_bNeedSerial,
                                m4bool_t& ao_bBranchMode) const
{
   ao_bNeedSerial = m_bNeedSerial;
   ao_bBranchMode = m_bBranchMode;
}

void ClNode_Data::SetNeedSerial(const m4bool_t ai_bNeedSerial,
                                const m4bool_t ai_bBranchMode)
{
   M4_MSG_ASSERT(m_pClNode_Kernel->CsType() == M4CL_CSTYPE_MAPPED, "Se esta marcarndo un nodo que no es serializable");

   m_bNeedSerial = ai_bNeedSerial;
   m_bBranchMode = ai_bBranchMode;
}

void ClNode_Data::TestAndSetNeedSerial(m4bool_t&      ao_bBeforeNeedSerial,
                                       m4bool_t&      ao_bBeforeBranchMode,
                                       const m4bool_t ai_bNeedSerial,
                                       const m4bool_t ai_bBranchMode)
{
   GetNeedSerial(ao_bBeforeNeedSerial, ao_bBeforeBranchMode);
   SetNeedSerial(ai_bNeedSerial, ai_bBranchMode);
}

void ClNode_Data::FindRecordAndSetVersion(const m4uint32_t ai_iRecordId,
                                          const m4date_t   ai_dNewRecordVersion,
                                          m4bool_t&        ao_bContinue)
{
    // Si el nodo no es mapped no hace falta que siga buscando el registro,
    // solo se puede dar en un registro que existe tanto en el cliente como en el server
    if (m_pClNode_Kernel->CsType() == M4CL_CSTYPE_MAPPED)
    {
        ClChangeRecordVersionInfo oChanRecVer(ai_iRecordId, ai_dNewRecordVersion);
      
        ClCStoreUtilities::ForEach(m_pBlockStore,
                                   ClCStoreUtilities::FindRecordAndSetVersion,
                                   (void*)&oChanRecVer);

        ao_bContinue = oChanRecVer.m_bContinue;
    }
}

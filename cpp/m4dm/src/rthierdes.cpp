
#include "rthierdes.hpp"

#include "ch_process.hpp"
#include "channel.hpp"
#include "chan_knl.hpp"
#include "compst_common.hpp"
#include "node_knl.hpp"

#include "px_block.hpp"
#include "m4mdrt.hpp"


ClChanRoutHierarchicalDescendent::ClChanRoutHierarchicalDescendent(void)
{
}

m4return_t ClChanRoutHierarchicalDescendent::Begin(ClChannel* const ai_pChannel, void* ai_pGenericParam)
{
    return RootAndFreeNodeIteration(ai_pChannel, ai_pGenericParam);
}

m4return_t ClChanRoutHierarchicalDescendent::Begin(ClNode_Data*           const ai_pNodeData,
                                                   void*                        ai_pGenericParam)
{
    // En un recorrido jerarquico no puedo empezar por un nodo porque no se donde estoy en el padre
    return M4_ERROR;
}

m4return_t ClChanRoutHierarchicalDescendent::Begin(ClBlock_Channel_Proxy* const ai_pBlockProxy,
                                                   void*                        ai_pGenericParam)
{
    // En un recorrido jerarquico no puedo empezar por un bloque porque no se donde estoy en el padre
    return M4_ERROR;
}

m4return_t ClChanRoutHierarchicalDescendent::Begin(ClBlock_Channel_Proxy* const ai_pBlockProxy,
                                                   const m4int32_t              ai_iRecordIndex,
                                                   void*                        ai_pGenericParam)
{
    return NodeChildRBIteration(ai_pBlockProxy, ai_iRecordIndex, ai_pGenericParam);
}

m4return_t ClChanRoutHierarchicalDescendent::RootAndFreeNodeIteration(ClChannel* const ai_pChannel,
                                                                      void*            ai_pGenericParam)
{
    M4_ASSERT (ai_pChannel != NULL);

    //
    // For each root node...
    //
    m4bool_t               bContinue;
    m4uint32_t             iBlockToProcess;
    m4uint32_t             iRegisterToProcess;
    ClBlock_Channel_Proxy* pBlockProxy;
    ClBlockReference       oBlockRef;
    ClHandle               nodeHandle;
    ClNode_Data*           pNode;
    ClNode_Kernel*         pNodeKernel;
    ClChannel_Data*        pChannelData   = ai_pChannel->GetpChannel_Data();
    ClChannel_Kernel*      pChannelKernel = pChannelData->GetpExtInfo();
    ClCompiledMCR*         pCMCR          = pChannelKernel->GetpCMCR();
    m4uint16_t             iNumRoots      = pChannelKernel->NumberOfRoots();

   	m4uint16_t i; 
    for (i = 0; i < iNumRoots; i++)
    {
        nodeHandle  = pChannelKernel->RootHandle(i);
        pNode       = pChannelData->Node.Get(nodeHandle);
        pNodeKernel = pNode->GetpExtInfo();
        
        //
        // Proceso a nivel de nodo >>>>>>>>>>>>>>>>>>>>>
        //
        if (NodeProcess(pNode, ai_pGenericParam, bContinue) != M4_SUCCESS)
            return M4_ERROR;
        
        if (bContinue == M4_FALSE)
            continue;
        //
        // Proceso a nivel de nodo <<<<<<<<<<<<<<<<<<<<<
        //
        
        // Si el canal esta encarrilado hago el recorrido por el bloque seleccionado
        // Sino, hago el recorrido por el único bloque que tiene el root
        if (pCMCR->GetLaneSonNodeId() != NULL &&
            strcmp(pNodeKernel->Id(), pCMCR->GetLaneSonNodeId()) == 0)
        {
            iBlockToProcess = 0;
            iRegisterToProcess = 0;
        }
        else
        {
            iBlockToProcess = 0;
            iRegisterToProcess = 0;
        }
        
        // Le pido al BlockStore el proxy que tengo que serializar
        if (pNode->GetpBlockStore()->Get(iBlockToProcess, iRegisterToProcess, oBlockRef) != M4_SUCCESS)
            return M4_ERROR;
        pBlockProxy = oBlockRef.GetpProxy();

        // El proxy puede no exitir, o ser nihil. En estos casos no hago nada
        if (pBlockProxy == NULL || pBlockProxy->IsNihil() == M4_TRUE)
            continue;
        
        // Itero por los registros del bloque
        if (RecordIteration(pBlockProxy, ai_pGenericParam) != M4_SUCCESS)
            return M4_ERROR;
    }
    
    //
    // For each free node...
    //
    m4bool_t   IsFree;
    m4uint16_t NumInverseConectors;
    m4uint32_t iNumNodes = pChannelData->Node.Count();
    
    for (ClNodeIndex oNodeIndex = 0; oNodeIndex < iNumNodes; oNodeIndex++)
    {
        pNode = pChannelData->Node.Get(oNodeIndex);
        pNodeKernel = pNode->GetpExtInfo();
        
        // Cambio para soportar problema de carriles de JESUS >>>>>>>>>>>>>>>
        if (pCMCR->GetNodeIsRealRoot(pNodeKernel->GetHandle()))
            continue;
        // Cambio para soportar problema de carriles de JESUS <<<<<<<<<<<<<<<
        
        NumInverseConectors = pNodeKernel->GetNodeNumberOfInvConnectors();
        
        if (NumInverseConectors > 0)
        {
            IsFree = M4_TRUE;

            for (i = 0; i < NumInverseConectors; i++)
            {
                nodeHandle = pNodeKernel->GetNodeInvConnectorHandleByPosition(i);
                if (pNodeKernel->GetNodeConnectorType(nodeHandle) != M4CL_NODE_CONNECTOR_TYPE_FREE)
                {
                    IsFree = M4_FALSE;
                    break;
                }
            }
            
            if (IsFree == M4_TRUE)
            {
                //
                // Proceso a nivel de nodo >>>>>>>>>>>>>>>>>>>>>
                //
                if (NodeProcess(pNode, ai_pGenericParam, bContinue) != M4_SUCCESS)
                    return M4_ERROR;
                
                if (bContinue == M4_FALSE)
                    continue;
                //
                // Proceso a nivel de nodo <<<<<<<<<<<<<<<<<<<<<
                //

                pNode->GetpUniqueBlock(oBlockRef);
                pBlockProxy = oBlockRef.GetpProxy();
                
                // El proxy puede no exitir, o ser nihil. En estos casos no hago nada
                if (pBlockProxy == NULL || pBlockProxy->IsNihil() == M4_TRUE)
                    continue;

                // Itero por los registros del bloque
                if (RecordIteration(pBlockProxy, ai_pGenericParam) != M4_SUCCESS)
                    return M4_ERROR;
            }
        }
    }
    
    return M4_SUCCESS;
}

m4return_t ClChanRoutHierarchicalDescendent::RecordIteration(ClBlock_Channel_Proxy* const ai_pBlockProxy,
                                                             void*                        ai_pGenericParam)
{
    m4bool_t bContinue;

    //
    // Proceso a nivel de bloque >>>>>>>>>>>>>>>>>>>>>
    //
    if (BlockProcess(ai_pBlockProxy, ai_pGenericParam, bContinue) != M4_SUCCESS)
        return M4_ERROR;
    
    if (bContinue == M4_FALSE)
        return M4_SUCCESS;
    //
    // Proceso a nivel de bloque <<<<<<<<<<<<<<<<<<<<<
    //
    
    regindex_t iNumRecords = ai_pBlockProxy->Count();
    for (regindex_t iRecordIndex = 0; iRecordIndex < iNumRecords; iRecordIndex++)
    {
        // Si el registro no es valido no hago nada mas con el
        if (ai_pBlockProxy->R_IsValid(iRecordIndex) == M4_FALSE)
            continue;

        // Itero por los nodos hijos (RB)
        if (NodeChildRBIteration(ai_pBlockProxy, iRecordIndex, ai_pGenericParam) != M4_SUCCESS)
            return M4_ERROR;
    }
    
    return M4_SUCCESS;
}


m4return_t ClChanRoutHierarchicalDescendent::NodeChildRBIteration(ClBlock_Channel_Proxy* const ai_pBlockProxy,
                                                                  const regindex_t             ai_iRecordIndex,
                                                                  void*                        ai_pGenericParam)
{
    ClNode_Data*           pchildnode;
    ClNode_Kernel*         pNode_Kernel     = ai_pBlockProxy->GetpNode_Data()->GetpExtInfo();
    ClChildIndex           iNumChildNodesRB = pNode_Kernel->ChildNodeRB.Count();
    ClBlockReference       oBlockRef;
    ClBlock_Channel_Proxy* pBlockProxy;

    //
    // Proceso a nivel de registro >>>>>>>>>>>>>>>>>>>>>
    //
    m4bool_t bContinue;
    if (RecordProcess(ai_pBlockProxy, ai_iRecordIndex, ai_pGenericParam, bContinue) != M4_SUCCESS)
        return M4_ERROR;
    
    if (bContinue == M4_FALSE)
        return M4_SUCCESS;
    //
    // Proceso a nivel de registro <<<<<<<<<<<<<<<<<<<<<
    //

    for (ClChildIndex i = 0; i < iNumChildNodesRB; i++)
    {
        // Conseguimos el nodo hijo
        pchildnode = pNode_Kernel->ChildNodeRB[i].GetAddress();
        
        // Conseguimos el bloque hijo
        pchildnode->GetpBlockStore()->Get(ai_pBlockProxy->ID.BlockUniqueKey, ai_iRecordIndex, oBlockRef);
        pBlockProxy = oBlockRef.GetpProxy();

        // El proxy puede no exitir, o ser nihil. En estos casos no hago nada
        if (pBlockProxy == NULL || pBlockProxy->IsNihil() == M4_TRUE)
            continue;

        // Itero por los registros del bloque
        if (RecordIteration(pBlockProxy, ai_pGenericParam) != M4_SUCCESS)
            return M4_ERROR;
    }

    return M4_SUCCESS;
}


m4return_t ClChanRoutHierarchicalDescendent::GetSize(ClSize &ao_size)
{
    return M4_ERROR;
}


#include "ch_process.hpp"
#include "node_knl.hpp"
#include "px_block.hpp"


ClChannelProcess2::ClChannelProcess2(void) :
    m_vpExclusionNodeName(NULL),
    m_iNumExclusionNodes(0),
    m_bExclusionNode(M4_FALSE)
{
}

void ClChannelProcess2::SetExclusionNodes(const m4char_t*  ai_vpExclusionNodeName[],
                                          const m4uint16_t ai_iNumExclusionNodes)
{
    m_vpExclusionNodeName = ai_vpExclusionNodeName;
    m_iNumExclusionNodes  = ai_iNumExclusionNodes;
}

m4return_t ClChannelProcess2::ChannelProcess(ClChannel* const ai_pChannel,
                                             void*            ai_pGenericParam,
                                             m4bool_t&        ao_bContinue)
{
    return _ChannelProcess(ai_pChannel, ai_pGenericParam, ao_bContinue);
}

m4return_t ClChannelProcess2::NodeProcess(ClNode_Data* const ai_pNodeData,
                                          void*              ai_pGenericParam,
                                          m4bool_t&          ao_bContinue)
{
    // Compruebo si estoy entrando en un nodo de exclusion
    if (IsExclusionNode(ai_pNodeData->GetpNodeDef()->Id()) == M4_TRUE)
    {
        ao_bContinue = M4_TRUE;
        return M4_SUCCESS;
    }
    return _NodeProcess(ai_pNodeData, ai_pGenericParam, ao_bContinue);
}

m4return_t ClChannelProcess2::BlockProcess(ClBlock_Channel_Proxy* const ai_pBlockProxy,
                                           void*                        ai_pGenericParam,
                                           m4bool_t&                    ao_bContinue)
{
    // Compruebo si estoy entrando en un nodo de exclusion
    if (IsExclusionNode(ai_pBlockProxy->GetpNode_Data()->GetpNodeDef()->Id()) == M4_TRUE)
    {
        ao_bContinue = M4_TRUE;
        return M4_SUCCESS;
    }
    return _BlockProcess(ai_pBlockProxy, ai_pGenericParam, ao_bContinue);
}

m4return_t ClChannelProcess2::RecordProcess(ClBlock_Channel_Proxy* const ai_pBlockProxy,
                                            const m4int32_t              ai_iRecordIndex,
                                            void*                        ai_pGenericParam,
                                            m4bool_t&                    ao_bContinue)
{
    // Compruebo si estoy entrando en un nodo de exclusion
    if (IsExclusionNode(ai_pBlockProxy->GetpNode_Data()->GetpNodeDef()->Id()) == M4_TRUE)
    {
        ao_bContinue = M4_TRUE;
        return M4_SUCCESS;
    }
    return _RecordProcess(ai_pBlockProxy, ai_iRecordIndex, ai_pGenericParam, ao_bContinue);
}

m4bool_t ClChannelProcess2::IsExclusionNode(const m4char_t* ai_pNodeName) const
{
    // Compruebo si estoy entrando en un nodo de exclusion
    if (m_vpExclusionNodeName != NULL)
    {
        for (m4uint16_t i = 0; i < m_iNumExclusionNodes; i++)
        {
            if (strcmp(ai_pNodeName, m_vpExclusionNodeName[i]) == 0)
                return M4_TRUE;
        }
    }
    return M4_FALSE;
}

m4return_t ClChannelProcess2::_ChannelProcess(ClChannel* const ai_pChannel,
                                              void*            ai_pGenericParam,
                                              m4bool_t&        ao_bContinue)
{
    ao_bContinue = M4_TRUE;
    return M4_SUCCESS;
}

m4return_t ClChannelProcess2::_NodeProcess(ClNode_Data* const ai_pNodeData,
                                           void*              ai_pGenericParam,
                                           m4bool_t&          ao_bContinue)
{
    ao_bContinue = M4_TRUE;
    return M4_SUCCESS;
}

m4return_t ClChannelProcess2::_BlockProcess(ClBlock_Channel_Proxy* const ai_pBlockProxy,
                                            void*                        ai_pGenericParam,
                                            m4bool_t&                    ao_bContinue)
{
    ao_bContinue = M4_TRUE;
    return M4_SUCCESS;
}

m4return_t ClChannelProcess2::_RecordProcess(ClBlock_Channel_Proxy* const ai_pBlockProxy,
                                             const m4int32_t              ai_iRecordIndex,
                                             void*                        ai_pGenericParam,
                                             m4bool_t&                    ao_bContinue)
{
    ao_bContinue = M4_TRUE;
    return M4_SUCCESS;
}

m4return_t ClChannelProcess2::GetSize(ClSize &ao_size) const
{
    return M4_ERROR;
}

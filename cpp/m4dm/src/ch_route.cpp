
#include "m4stl.hpp"
#include "ch_route.hpp"
#include "ch_process.hpp"


ClChannelRoute::ClChannelRoute(void)
{
}

void ClChannelRoute::Reset(void)
{
    m_vChannelProcess.clear();
}

void ClChannelRoute::AddProcess(ClChannelProcess2* const ai_pChannelProcess)
{
    M4_ASSERT(ai_pChannelProcess != NULL);

    m_vChannelProcess.push_back(ai_pChannelProcess);
}

void ClChannelRoute::DelProcess(ClChannelProcess2* const ai_pChannelProcess)
{
    M4_ASSERT(ai_pChannelProcess != NULL);

    vector<ClChannelProcess2*>::iterator i =
        find(m_vChannelProcess.begin(), m_vChannelProcess.end(), ai_pChannelProcess);

    if (i != m_vChannelProcess.end())
        m_vChannelProcess.erase(i);
}

m4return_t ClChannelRoute::GetSize(ClSize &ao_size)
{
    return M4_ERROR;
}

m4return_t ClChannelRoute::ChannelProcess(ClChannel* const ai_pChannel,
                                          void*            ai_pGenericParam,
                                          m4bool_t&        ao_bContinue)
{
    M4_ASSERT(ai_pChannel != NULL);

    vector<ClChannelProcess2*>::iterator i;
    
    for (i = m_vChannelProcess.begin(); i != m_vChannelProcess.end(); i++)
    {
        if ((*i)->ChannelProcess(ai_pChannel, ai_pGenericParam, ao_bContinue) != M4_SUCCESS)
            return M4_ERROR;
    }
    return M4_SUCCESS;
}

m4return_t ClChannelRoute::NodeProcess(ClNode_Data* const ai_pNodeData,
                                       void*              ai_pGenericParam,
                                       m4bool_t&          ao_bContinue)
{
    M4_ASSERT(ai_pNodeData != NULL);

    vector<ClChannelProcess2*>::iterator i;
    
    for (i = m_vChannelProcess.begin(); i != m_vChannelProcess.end(); i++)
    {
        if ((*i)->NodeProcess(ai_pNodeData, ai_pGenericParam, ao_bContinue) != M4_SUCCESS)
            return M4_ERROR;
    }
    return M4_SUCCESS;
}

m4return_t ClChannelRoute::BlockProcess(ClBlock_Channel_Proxy* const ai_pBlockProxy,
                                        void*                        ai_pGenericParam,
                                        m4bool_t&                    ao_bContinue)
{
    M4_ASSERT(ai_pBlockProxy != NULL);

    vector<ClChannelProcess2*>::iterator i;
    
    for (i = m_vChannelProcess.begin(); i != m_vChannelProcess.end(); i++)
    {
        if ((*i)->BlockProcess(ai_pBlockProxy, ai_pGenericParam, ao_bContinue) != M4_SUCCESS)
            return M4_ERROR;
    }
    return M4_SUCCESS;
}

m4return_t ClChannelRoute::RecordProcess(ClBlock_Channel_Proxy* const ai_pBlockProxy,
                                         const m4int32_t              ai_iRecordIndex,
                                         void*                        ai_pGenericParam,
                                         m4bool_t&                    ao_bContinue)
{
    M4_ASSERT(ai_pBlockProxy != NULL);

    vector<ClChannelProcess2*>::iterator i;
    
    for (i = m_vChannelProcess.begin(); i != m_vChannelProcess.end(); i++)
    {
        if ((*i)->RecordProcess(ai_pBlockProxy, ai_iRecordIndex, ai_pGenericParam, ao_bContinue) != M4_SUCCESS)
            return M4_ERROR;
    }
    return M4_SUCCESS;
}

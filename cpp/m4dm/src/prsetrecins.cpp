
#include "prsetrecins.hpp"

#include "channel.hpp"
#include "chan_knl.hpp"
#include "node_knl.hpp"

#include "px_block.hpp"

#include "rm.hpp"


ClChanProcSetRecStatInserted::ClChanProcSetRecStatInserted(const m4bool_t ai_bDestroyRecords) :
    m_bDestroyRecords(ai_bDestroyRecords)
{
}

m4return_t ClChanProcSetRecStatInserted::_ChannelProcess(ClChannel* const ai_pChannel,
                                                         void*            ai_pGenericParam,
                                                         m4bool_t&        ao_bContinue)
{
    ai_pChannel->GetpChannel_Data()->Operation.Set(M4DM_OPERATION_INSERT);

    ao_bContinue = M4_TRUE;
    return M4_SUCCESS;
}

m4return_t ClChanProcSetRecStatInserted::_NodeProcess(ClNode_Data* const ai_pNodeData,
                                                      void*              ai_pGenericParam,
                                                      m4bool_t&          ao_bContinue)
{
    ai_pNodeData->Operation.Set(M4DM_OPERATION_INSERT);

    ao_bContinue = M4_TRUE;
    return M4_SUCCESS;
}

m4return_t ClChanProcSetRecStatInserted::_BlockProcess(ClBlock_Channel_Proxy* const ai_pBlockProxy,
                                                       void*                        ai_pGenericParam,
                                                       m4bool_t&                    ao_bContinue)
{
    m4return_t iRet = M4_SUCCESS;

    if (m_bDestroyRecords == M4_TRUE)
    {
        // Tengo que destruir los registros marcados para borrar
        // Utilizo el commit
        iRet = ai_pBlockProxy->CheckPoint();
    }

    ao_bContinue = M4_TRUE;
    return iRet;
}

m4return_t ClChanProcSetRecStatInserted::_RecordProcess(ClBlock_Channel_Proxy* const ai_pBlockProxy,
                                                        const m4int32_t              ai_iRecordIndex,
                                                        void*                        ai_pGenericParam,
                                                        m4bool_t&                    ao_bContinue)
{
    if (ai_pBlockProxy->R_SetStatus(ai_iRecordIndex, M4_RS_NEW) != M4_SUCCESS)
        return M4_ERROR;

    ao_bContinue = M4_TRUE;
    return M4_SUCCESS;
}

m4return_t ClChanProcSetRecStatInserted::GetSize(ClSize &ao_size) const
{
    return M4_ERROR;
}


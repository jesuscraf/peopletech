
#include "prnewrecid.hpp"

#include "chan_knl.hpp"
#include "node_knl.hpp"
#include "px_block.hpp"


ClChanProcNewRecordID::ClChanProcNewRecordID(void)
{
}

m4return_t ClChanProcNewRecordID::_RecordProcess(ClBlock_Channel_Proxy* const ai_pBlockProxy,
                                                 const m4int32_t              ai_iRecordIndex,
                                                 void*                        ai_pGenericParam,
                                                 m4bool_t&                    ao_bContinue)
{
    m4uint32_t iUniqueRowId = ai_pBlockProxy->GetpNode_Data()->GetpChannel_Data()->GetUniqueRowId();

    if (ai_pBlockProxy->R_SetRecordID(ai_iRecordIndex, iUniqueRowId) != M4_SUCCESS)
        return M4_ERROR;

    ao_bContinue = M4_TRUE;
    return M4_SUCCESS;
}

m4return_t ClChanProcNewRecordID::GetSize(ClSize &ao_size) const
{
    return M4_ERROR;
}


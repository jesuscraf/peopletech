#include "cp_find.hpp"
#include "node_knl.hpp"
#include "px_block.hpp"
#include "bookmark.hpp"
#include "access.hpp"

m4return_t ClChanProc_FindRegister::_BlockProcess(ClBlock_Channel_Proxy* const ai_pBlockProxy,
                                               void*                        ai_pGenericParam,
                                               m4bool_t&                    ao_bContinue)
{
    if (m_poAccess)
    {
        ClAccessRecordSet * pRS;
        ClNodeIndex index = ai_pBlockProxy->GetpNode_Data()->GetpExtInfo()->Index;
        pRS = &m_poAccess->Node.Get(index)->RecordSet;

        if (pRS->GetCurrentBlock() == ai_pBlockProxy)
        {
            if (m_pCurrents_Info[index].m_iItemCount)
                pRS->Current.FindSec (m_pCurrents_Info[index].m_pArguments, (m4uint16_t) (3 * m_pCurrents_Info[index].m_iItemCount));
            else
                pRS->Current.MoveToEOF();

            if (m_pRegisters_Info[index].m_iItemCount)
                pRS->Register.FindSec (m_pRegisters_Info[index].m_pArguments, (m4uint16_t) (3 * m_pRegisters_Info[index].m_iItemCount));
            else
                pRS->Register.MoveToEOF();
        }

        return M4_SUCCESS;
    }

    return M4_ERROR;
}

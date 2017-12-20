
#include "cp_exe.hpp"
#include "access.hpp"
#include "channel.hpp"
#include "chan_knl.hpp"
#include "node_knl.hpp"
#include "iexecutor.hpp"
#include "px_block.hpp"
#include "rm.hpp"


ClChanProc_Execution::ClChanProc_Execution(m4pchar_t ai_pItemName, ClAccess* ai_poAccess) :
    m_pItemName(ai_pItemName)
{
    m_poAccess = ai_poAccess->GetpChannel()->CreateAccess();
}

ClChanProc_Execution::~ClChanProc_Execution(void)
{
    m_poAccess->Destroy();
}

/*
m4return_t ClChanProc_Execution::_ChannelProcess(ClChannel* const ai_pChannel,
                                                 void*            ai_pGenericParam,
                                                 m4bool_t&        ao_bContinue)
{
    //if (m_poAccess)
    //    m_poAccess->Destroy ();

    // Como me llaman siempre, me creo aqui el access que necesito para ejecutar
    //m_poAccess = ai_pChannel->CreateAccess();

    ao_bContinue = M4_TRUE;
    return M4_SUCCESS;
}
*/

m4return_t ClChanProc_Execution::_NodeProcess(ClNode_Data* const ai_pNodeData,
                                              void*              ai_pGenericParam,
                                              m4bool_t&          ao_bContinue)
{
    m4return_t res = M4_SUCCESS;
    m4VariantType v;

    if (m_poAccess)
    {
        ClNode * poNode;
        poNode = m_poAccess->Node.Get(ai_pNodeData->GetpNodeDef()->GetHandle());

        ao_bContinue = M4_TRUE;

        if (poNode->Item.MoveTo(m_pItemName))
        {
            if (poNode->Item.ItemDef.Scope() == M4CL_ITEM_SCOPE_NODE)
            {
                res = poNode->Item.Call();
                m_poAccess->GetpExecutor()->Stack.Pop (v);
            }
        }

        return res;
    }

    return M4_ERROR;
}

m4return_t ClChanProc_Execution::_BlockProcess(ClBlock_Channel_Proxy* const ai_pBlockProxy,
                                               void*                        ai_pGenericParam,
                                               m4bool_t&                    ao_bContinue)
{
    m4return_t res = M4_SUCCESS;
    m4VariantType v;

    if (m_poAccess)
    {
        ClNode * poNode;
        poNode = m_poAccess->Node.Get(ai_pBlockProxy->GetpNode_Data()->GetpNodeDef()->GetHandle());
        res = poNode->RecordSet.Follow (ai_pBlockProxy);

        ao_bContinue = M4_TRUE;

        if (poNode->RecordSet.Item.MoveTo(m_pItemName))
        {
            if (poNode->RecordSet.Item.ItemDef.Scope() == M4CL_ITEM_SCOPE_BLOCK)
            {
                res = poNode->RecordSet.Item.Call();
                m_poAccess->GetpExecutor()->Stack.Pop (v);
            }
        }

        return res;
    }

    return M4_ERROR;
}

m4return_t ClChanProc_Execution::_RecordProcess(ClBlock_Channel_Proxy* const ai_pBlockProxy,
                                                const m4int32_t              ai_iRecordIndex,
                                                void*                        ai_pGenericParam,
                                                m4bool_t&                    ao_bContinue)
{
    m4return_t res = M4_SUCCESS;
    m4VariantType v;

    if (m_poAccess)
    {
        ClNode * poNode;
        poNode = m_poAccess->Node.Get(ai_pBlockProxy->GetpNode_Data()->GetpNodeDef()->GetHandle());
        res = poNode->RecordSet.Follow (ai_pBlockProxy);
        poNode->RecordSet.Current.MoveTo(ai_iRecordIndex);

        ao_bContinue = M4_TRUE;

        if (poNode->RecordSet.Current.Item.MoveTo(m_pItemName))
        {
            if (poNode->RecordSet.Current.Item.ItemDef.Scope() == M4CL_ITEM_SCOPE_REGISTER)
            {
                res = poNode->RecordSet.Current.Item.Call();
                m_poAccess->GetpExecutor()->Stack.Pop (v);
            }
        }

        return res;
    }

    return M4_ERROR;
}

m4return_t ClChanProc_Execution::GetSize(ClSize &ao_size)
{
    return M4_ERROR;
}



#ifndef _CP_FIND_HPP_
#define _CP_FIND_HPP_


#include "m4chcommon.hpp"
#include "ch_process.hpp"
#include "sizer.hpp"

class ClAccess;
class ClNode_PK_Info;

class ClChanProc_FindRegister : public ClChannelProcess2
{
public:
    ClChanProc_FindRegister(ClNode_PK_Info * ai_pCurrents_Info,
        ClNode_PK_Info * ai_pRegisters_Info, ClAccess* ai_poAccess)
        : m_pCurrents_Info(ai_pCurrents_Info), m_pRegisters_Info(ai_pRegisters_Info),
        m_poAccess(ai_poAccess)
    {
    }

    m4return_t GetSize(ClSize &ao_size);

protected:

    m4return_t _BlockProcess  (ClBlock_Channel_Proxy* const ai_pBlockProxy,
                               void*                        ai_pGenericParam,
                               m4bool_t&                    ao_bContinue);

private:

    ClAccess* m_poAccess;
    ClNode_PK_Info * m_pCurrents_Info, * m_pRegisters_Info;
};


#endif //_CP_FIND_HPP_

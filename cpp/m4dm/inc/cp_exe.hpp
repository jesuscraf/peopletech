
#ifndef _CP_EXE_HPP_
#define _CP_EXE_HPP_

#include "m4dm_dll.hpp"
#include "m4chcommon.hpp"
#include "ch_process.hpp"
#include "sizer.hpp"

class ClAccess;


class M4_DECL_M4DM ClChanProc_Execution : public ClChannelProcess2
{
public:
    ClChanProc_Execution(m4pchar_t ai_pItemName, ClAccess* ai_poAccess);

    ~ClChanProc_Execution(void);

    virtual m4return_t GetSize(ClSize &ao_size);

protected:
    //virtual m4return_t _ChannelProcess(ClChannel*             const ai_pChannel,
    //                                   void*                        ai_pGenericParam,
    //                                   m4bool_t&                    ao_bContinue);

    virtual m4return_t _NodeProcess   (ClNode_Data*           const ai_pNodeData,
                                       void*                        ai_pGenericParam,
                                       m4bool_t&                    ao_bContinue);

    virtual m4return_t _BlockProcess  (ClBlock_Channel_Proxy* const ai_pBlockProxy,
                                       void*                        ai_pGenericParam,
                                       m4bool_t&                    ao_bContinue);

    virtual m4return_t _RecordProcess (ClBlock_Channel_Proxy* const ai_pBlockProxy,
                                       const m4int32_t              ai_iRecordIndex,
                                       void*                        ai_pGenericParam,
                                       m4bool_t&                    ao_bContinue);

private:
    ClAccess* m_poAccess;
    m4pchar_t m_pItemName;
};


#endif

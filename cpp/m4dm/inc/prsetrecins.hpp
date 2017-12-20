
#ifndef _PRSETRECINS_HPP_
#define _PRSETRECINS_HPP_


#include "m4chcommon.hpp"
#include "ch_process.hpp"
#include "sizer.hpp"


class ClChanProcSetRecStatInserted : public ClChannelProcess2
{
public:
    ClChanProcSetRecStatInserted(const m4bool_t ai_bDestroyRecords);

    virtual m4return_t GetSize(ClSize &ao_size) const;

protected:
    virtual m4return_t _ChannelProcess(ClChannel*             const ai_pChannel,
                                       void*                        ai_pGenericParam,
                                       m4bool_t&                    ao_bContinue);

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
    m4bool_t m_bDestroyRecords;
};


#endif

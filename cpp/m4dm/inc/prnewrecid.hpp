
#ifndef _PRNEWRECID_HPP_
#define _PRNEWRECID_HPP_


#include "m4chcommon.hpp"
#include "ch_process.hpp"
#include "sizer.hpp"


class ClChanProcNewRecordID : public ClChannelProcess2
{
public:
    ClChanProcNewRecordID(void);

    virtual m4return_t GetSize(ClSize &ao_size) const;

protected:
    //virtual m4return_t ChannelProcess(ClChannel*             const ai_pChannel,
    //                                  void*                        ai_pGenericParam,
    //                                  m4bool_t&                    ao_bContinue);

    //virtual m4return_t NodeProcess   (ClNode_Data*           const ai_pNodeData,
    //                                  void*                        ai_pGenericParam,
    //                                  m4bool_t&                    ao_bContinue);

    //virtual m4return_t BlockProcess  (ClBlock_Channel_Proxy* const ai_pBlockProxy,
    //                                  void*                        ai_pGenericParam,
    //                                  m4bool_t&                    ao_bContinue);

    virtual m4return_t _RecordProcess (ClBlock_Channel_Proxy* const ai_pBlockProxy,
                                       const m4int32_t              ai_iRecordIndex,
                                       void*                        ai_pGenericParam,
                                       m4bool_t&                    ao_bContinue);
};


#endif

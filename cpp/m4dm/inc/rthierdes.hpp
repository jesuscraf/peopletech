
#ifndef _RTHIERDES_HPP_
#define _RTHIERDES_HPP_

#include "m4dm_dll.hpp"
#include "m4chcommon.hpp"
#include "ch_route.hpp"
#include "sizer.hpp"

class ClChannel;
class ClNode_Data;
class ClBlock_Channel_Proxy;


class M4_DECL_M4DM ClChanRoutHierarchicalDescendent : public ClChannelRoute
{
public:
    ClChanRoutHierarchicalDescendent(void);

    virtual m4return_t Begin(ClChannel*             const ai_pChannel,
                             void*                        ai_pGenericParam);

    virtual m4return_t Begin(ClNode_Data*           const ai_pNodeData,
                             void*                        ai_pGenericParam);

    virtual m4return_t Begin(ClBlock_Channel_Proxy* const ai_pBlockProxy,
                             void*                        ai_pGenericParam);

    virtual m4return_t Begin(ClBlock_Channel_Proxy* const ai_pBlockProxy,
                             const m4int32_t              ai_iRecordIndex,
                             void*                        ai_pGenericParam);

    virtual m4return_t GetSize(ClSize &ao_size);

private:
    m4return_t RootAndFreeNodeIteration(ClChannel*             const ai_pChannel,
                                        void*                        ai_pGenericParam);
    
    m4return_t RecordIteration         (ClBlock_Channel_Proxy* const ai_pBlockProxy,
                                        void*                        ai_pGenericParam);

    m4return_t NodeChildRBIteration    (ClBlock_Channel_Proxy* const ai_pBlockProxy,
                                        const m4int32_t              ai_iRecordIndex,
                                        void*                        ai_pGenericParam);	
};


#endif

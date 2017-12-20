
#ifndef _CH_ROUTE_HPP_
#define _CH_ROUTE_HPP_

#include "m4dm_dll.hpp"
#include "m4stl.hpp"
#include "m4chcommon.hpp"
#include "sizer.hpp"

class ClChannel;
class ClNode_Data;
class ClBlock_Channel_Proxy;
class ClChannelProcess2;


class M4_DECL_M4DM ClChannelRoute
{
public:
    ClChannelRoute(void);

    void Reset(void);

    // Los distintos procesamientos se realizan en el orden en el que se han añadido
    void AddProcess(ClChannelProcess2* const ai_pChannelProcess);
    void DelProcess(ClChannelProcess2* const ai_pChannelProcess);
    
    // Comienza el recorido
    virtual m4return_t Begin(ClChannel*             const ai_pChannel,
                             void*                        ai_pGenericParam) = 0;

    virtual m4return_t Begin(ClNode_Data*           const ai_pNodeData,
                             void*                        ai_pGenericParam) = 0;

    virtual m4return_t Begin(ClBlock_Channel_Proxy* const ai_pBlockProxy,
                             void*                        ai_pGenericParam) = 0;

    virtual m4return_t Begin(ClBlock_Channel_Proxy* const ai_pBlockProxy,
                             const m4int32_t              ai_iRecordIndex,
                             void*                        ai_pGenericParam) = 0;


	virtual m4return_t GetSize(ClSize &ao_size);

protected:
    // Estas funciones miembro iteran por el vector de procesamientos
    m4return_t ChannelProcess(ClChannel*             const ai_pChannel,
                              void*                        ai_pGenericParam,
                              m4bool_t&                    ao_bContinue);

    m4return_t NodeProcess   (ClNode_Data*           const ai_pNodeData,
                              void*                        ai_pGenericParam,
                              m4bool_t&                    ao_bContinue);

    m4return_t BlockProcess  (ClBlock_Channel_Proxy* const ai_pBlockProxy,
                              void*                        ai_pGenericParam,
                              m4bool_t&                    ao_bContinue);

    m4return_t RecordProcess (ClBlock_Channel_Proxy* const ai_pBlockProxy,
                              const m4int32_t              ai_iRecordIndex,
                              void*                        ai_pGenericParam,
                              m4bool_t&                    ao_bContinue);

    vector<ClChannelProcess2*> m_vChannelProcess;
};


#endif

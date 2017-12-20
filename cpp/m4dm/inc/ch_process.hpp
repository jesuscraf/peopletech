
#ifndef _CH_PROCESS_HPP_
#define _CH_PROCESS_HPP_


#include "m4chcommon.hpp"
#include "sizer.hpp"

class ClChannel;
class ClNode_Data;
class ClBlock_Channel_Proxy;


class ClChannelProcess2
{
public:
    ClChannelProcess2(void);

    void SetExclusionNodes(const m4char_t*  ai_vpExclusionNodeName[],
                           const m4uint16_t ai_iNumExclusionNodes);

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

    virtual m4return_t GetSize(ClSize &ao_size) const;

protected:
    // Funciones miembro que redefinen el comportamiento del proceso
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

    // Miembros para controlar los nodos de exclusion
    m4bool_t IsExclusionNode(const m4char_t* ai_pNodeName) const;

    const m4char_t** m_vpExclusionNodeName; // Vector de punteros a los nombres de los nodos que hay que excluir en el proceso
    m4uint16_t       m_iNumExclusionNodes;  // Numero de nodos que hay que excluir en el proceso
    m4bool_t         m_bExclusionNode;      // Indica si estoy en un nodo que hay que excluir en el proceso
};


#endif

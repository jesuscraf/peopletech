
#ifndef __LANEINFO__HPP__
#define __LANEINFO__HPP__

#include "m4types.hpp"
#include "childnod.hpp"

class ClConnectItemInfo;
class ClHandle;
class ClNode;

class ClLane_Item_Info
{
public:

    ClLane_Item_Info (m4uint32_t ai_icount);
   ~ClLane_Item_Info (void);

    ClConnectItemInfo * m_pConnectItemInfo;
    ClHandle * m_pHandles;

	m4uint32_t m_iCount;
	m4return_t GetSize(ClSize &ao_size);
};

class ClLane_Info
{
public:

    ClLane_Info (void);
   ~ClLane_Info (void);

    ClChildNodes<ClNode>   ChildNodeRB;

    m4uint32_t AddNewDoc(m4uint32_t ai_icount);

    m4return_t DeleteDoc(ClNode * ai_pNodeDoc);

    ClLane_Item_Info * GetpLane_Item_Info_by_Doc(m4uint32_t ai_doc);

	ClLane_Item_Info ** m_poLanItInf;

	m4uint32_t m_iDocs;
	m4uint32_t m_iArrayDim;

    void IncreaseArrayDim();

	m4return_t GetSize(ClSize &ao_size);
};

#endif //__LANEINFO__HPP__






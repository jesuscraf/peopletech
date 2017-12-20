//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             laneman.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             16/10/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#ifndef __LANEMAN__HPP__
#define __LANEMAN__HPP__

#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include "m4stl.hpp"

#include "lanecon.hpp"

class ClAccess;
class ClLaneManager;
class ClNode;
class ClNode;

///////////////////////////////////////////////////////////////////////////////
//
// Node List (lista de documentos)
//

struct StDocumentList
{
	StDocumentList () : m_pNode(0), m_pNext(0)
	{
    }

	~StDocumentList ()
	{
        if (m_pNext)
            delete m_pNext;
        m_pNext = 0;
        m_pNode = 0;
    }

    ClNode_Data * m_pNode;
    StDocumentList * m_pNext;
};


///////////////////////////////////////////////////////////////////////////////
//
// ClLaneManager
//

class ClChannelManager;

class M4_DECL_M4DM ClLaneManager
{
public:

    ClLaneConnection *AccessGetLaneConnection  (ClNode *ai_pnodedoc, ClNode *ai_pnodelane);
    ClLaneConnection *ChannelGetLaneConnection (ClNode *ai_pnodedoc, ClNode *ai_pnodelane);

    m4uint32_t ConnectionsCount (void);

    m4return_t ConnectIt        (ClNode *ai_pnodedoc, ClNode *ai_pnodelane);
    m4return_t DisConnectIt     (ClNode *ai_pnodedoc, ClNode *ai_pnodelane);
    m4return_t DisConnectItTemp (ClNode *ai_pnodedoc, ClNode *ai_pnodelane);
    m4return_t ReConnectIt      (ClNode *ai_pnodedoc, ClNode *ai_pnodelane);

    m4return_t GetAllDocuments (ClNode_Data * ai_pnodelane, StDocumentList & ao_List);

	m4return_t GetSize(ClSize &ao_size);

private:
    //ClChannelManager:
    ClLaneManager ();
   ~ClLaneManager ();

   m4bool_t EvaluateCondition(ClNode *ai_pnodedoc, ClNode *ai_pnodelane,
       ClLaneConnection *ai_pClConn);

    // lista de canales conectados:
    list<ClLaneConnection> m_LaneConnectionList;
    typedef list<ClLaneConnection>::iterator LaneConnectionListIt_t;

friend class ClChannelManager;

};

#endif // __LANEMAN__HPP__

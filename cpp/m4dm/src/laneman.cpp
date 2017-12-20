//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             laneman.cpp
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
//    The Lane (carril) Manager
//
//
//==============================================================================

//----------------------------------------------------------------------------
#include "dm_def.hpp"
#include "m4define.hpp"
//----------------------------------------------------------------------------

#include "laneman.hpp"

#include "node.hpp"
#include "channel.hpp"
#include "node_knl.hpp"
#include "sizerutils.hpp"
#include "m4mdrt.hpp"


///////////////////////////////////////////////////////////////////////////////
//
// ClLaneManager
//

ClLaneManager::ClLaneManager ()
{
}

ClLaneManager::~ClLaneManager ()
{
}

m4uint32_t ClLaneManager::ConnectionsCount (void)
{
    return m_LaneConnectionList.size ();
}

ClLaneConnection *ClLaneManager::AccessGetLaneConnection (ClNode *ai_pnodedoc, ClNode *ai_pnodelane)
{
    LaneConnectionListIt_t start, end, it;

    start = m_LaneConnectionList.begin();
    end   = m_LaneConnectionList.end();
    
    for (it = start; it != end; it++) {
        if ( (*it).AccessIsEqual (ai_pnodedoc, ai_pnodelane) )
            return &(*it);
    }

    // no se ha encontrado
    return 0;
}

ClLaneConnection *ClLaneManager::ChannelGetLaneConnection (ClNode *ai_pnodedoc, ClNode *ai_pnodelane)
{
    LaneConnectionListIt_t start, end, it;

    start = m_LaneConnectionList.begin();
    end   = m_LaneConnectionList.end();
    
    for (it = start; it != end; it++) {
        if ( (*it).ChannelIsEqual (ai_pnodedoc, ai_pnodelane) )
            return &(*it);
    }

    // no se ha encontrado
    return 0;
}

m4return_t ClLaneManager::ConnectIt (ClNode *ai_pnodedoc, ClNode *ai_pnodelane)
{
    m4return_t          res;
    ClLaneConnection *  plcon, * plconaux;

    if ((ai_pnodedoc == 0) || (ai_pnodelane == 0))
        return M4_ERROR;

	/* Bug 0074330
	Hay que comprobar que el documento no sea cacheable.
	*/
	ClCompiledMCR	*poDocCMCR = ai_pnodedoc->GetpExtInfo()->GetpCMCR() ;

	if( poDocCMCR->GetChannelIsCacheable() == M4CL_CACHEABLE_TRUE )
	{
		m4uint32_t		iDocHandle = ai_pnodedoc->GetpNodeDef()->GetHandle() ;
		m4uint32_t		iLaneHandle = ai_pnodelane->GetpNodeDef()->GetHandle() ;
		ClCompiledMCR	*poLaneCMCR = ai_pnodelane->GetpExtInfo()->GetpCMCR() ;

		DUMP_CHLOG_ERRORF( M4_CH_DM_CACHEABLE_DOC, LOG_CAT_AND( poDocCMCR->iNodeN( iDocHandle ), poDocCMCR->nNodeN( iDocHandle ) ) << LOG_CAT_AND( poDocCMCR->iM4Obj(), poDocCMCR->nM4Obj() ) << LOG_CAT_AND( poLaneCMCR->iNodeN( iLaneHandle ), poLaneCMCR->nNodeN( iLaneHandle ) ) << LOG_CAT_AND( poLaneCMCR->iM4Obj(), poLaneCMCR->nM4Obj() ) ) ;
		return( M4_ERROR ) ;
	}

    // 1) buscamos si ya existe una conexion entre los canales
    plcon = AccessGetLaneConnection (ai_pnodedoc, ai_pnodelane);

    // 2 ) Pregunto si hay alguna conexion entre los canales
    plconaux = ChannelGetLaneConnection (ai_pnodedoc, ai_pnodelane);

    if (plcon)
        // ya existe LA conexión entre los access
        return M4_SUCCESS;

    ClLaneConnection auxlcon;
    m_LaneConnectionList.push_back (auxlcon);

    // es una lista, apuntamos al último elemento (el recién añadido)
    plcon = &(m_LaneConnectionList.back ());

    if (plconaux)
    {
        // ya existe UNA conexión entre los canales
        // Meto una nueva conexion en la lista de carriles
        res = plcon->Init (ai_pnodedoc, ai_pnodelane, plconaux);
    }

    else
    {
        ClChannel_Data *pchannellane = ai_pnodelane->GetpNode_Data()->GetpChannel_Data ();
        ClChannel_Data *pchanneldoc = ai_pnodedoc->GetpNode_Data()->GetpChannel_Data ();

        res = plcon->Init (pchanneldoc, ai_pnodedoc, pchannellane, ai_pnodelane);
    }

    if (M4_ERROR == res)
        m_LaneConnectionList.pop_back ();
    else
    {
        //bucle para recorrer todos los accesos documento y poner los items del nodo
        //en eof.
        m4uint32_t an = ai_pnodedoc->GetpAccess()->GetpChannel()->Access.Count();

        ClNode *pna;
        ClAccessIndex ai;
        ClHandle nh = ai_pnodedoc->GetpNodeDef()->GetHandle();

        for (ai = 0; ai < an; ai++)
        {
            // por el access ...
            pna = ai_pnodedoc->GetpAccess()->GetpChannel()->Access[ai]->Node.Get (nh);

            pna->Item.MoveToEOF();
            pna->RecordSet.Item.MoveToEOF();
        }
    }

    return res;
}


m4return_t ClLaneManager::DisConnectIt (ClNode *ai_pnodedoc, ClNode *ai_pnodelane)
{
    m4return_t res = M4_WARNING;

    LaneConnectionListIt_t start, end, it, it2;

    start = m_LaneConnectionList.begin();
    end   = m_LaneConnectionList.end();

    ClLaneConnection *plcon;

    it = start;

    while (it != end) {
        plcon = &(*it);
        if (EvaluateCondition(ai_pnodedoc, ai_pnodelane, plcon))
        {
            plcon->Doc.pClNode->Item.MoveToEOF();
            plcon->Doc.pClNode->RecordSet.Item.MoveToEOF();

            //bucle para recorrer todos los accesos documento y poner los items del nodo
            //en eof.
            m4uint32_t an = plcon->Doc.pClNode->GetpAccess()->GetpChannel()->Access.Count();

            ClNode *pna;
            ClAccessIndex ai;
            ClHandle nh = plcon->Doc.pClNode->GetpNodeDef()->GetHandle();

            for (ai = 0; ai < an; ai++)
            {
                // por el access ...
                pna = plcon->Doc.pClNode->GetpAccess()->GetpChannel()->Access[ai]->Node.Get (nh);

                pna->Item.MoveToEOF();
                pna->RecordSet.Item.MoveToEOF();
            }

            //borra el elemento, it se queda corrusto:
            it2 = it;
            ++it2;
            m_LaneConnectionList.erase (it);
            it = it2;
            res = M4_SUCCESS;
        }
        else
            ++it;
    }

    return res;
}

m4return_t ClLaneManager::DisConnectItTemp (ClNode *ai_pnodedoc, ClNode *ai_pnodelane)
{
    m4return_t res = M4_WARNING;

    LaneConnectionListIt_t start, end, it, it2;

    start = m_LaneConnectionList.begin();
    end   = m_LaneConnectionList.end();

    ClLaneConnection *plcon;

    it = start;

    while (it != end) {
        plcon = &(*it);
        if (EvaluateCondition(ai_pnodedoc, ai_pnodelane, plcon))
        {
            res = plcon->DestroyTemp ();
            if (res == M4_SUCCESS)
            {
                plcon->Doc.pClNode->Item.MoveToEOF();
                plcon->Doc.pClNode->RecordSet.Item.MoveToEOF();
            }
        }
        ++it;
    }

    return res;
}

m4return_t ClLaneManager::ReConnectIt (ClNode *ai_pnodedoc, ClNode *ai_pnodelane)
{
    m4return_t res = M4_WARNING;

    LaneConnectionListIt_t start, end, it, it2;

    start = m_LaneConnectionList.begin();
    end   = m_LaneConnectionList.end();

    ClLaneConnection *plcon;

    it = start;

    while (it != end) {
        plcon = &(*it);
        if (EvaluateCondition(ai_pnodedoc, ai_pnodelane, plcon))
        {
            ClChannel_Data *pchannellane = plcon->Lane.pClNode->GetpNode_Data()->GetpChannel_Data ();
            ClChannel_Data *pchanneldoc = plcon->Doc.pClNode->GetpNode_Data()->GetpChannel_Data ();

            res = plcon->Init (pchanneldoc, plcon->Doc.pClNode, pchannellane, plcon->Lane.pClNode);
        }
        ++it;
    }

    return res;
}

m4bool_t ClLaneManager::EvaluateCondition(ClNode *ai_pnodedoc, ClNode *ai_pnodelane,
                                          ClLaneConnection *ai_pClConn)
{
    if ((ai_pnodedoc != 0) && (ai_pnodelane != 0))
    {
        return (ai_pClConn->Lane.pClNode == ai_pnodelane && ai_pClConn->Doc.pClNode == ai_pnodedoc)
            ? M4_TRUE : M4_FALSE;
    }

    if ((ai_pnodedoc == 0) && (ai_pnodelane != 0))
    {
        return (ai_pClConn->Lane.pClNode == ai_pnodelane) ? M4_TRUE : M4_FALSE;
    }

    if ((ai_pnodedoc != 0) && (ai_pnodelane == 0))
    {
        return (ai_pClConn->Doc.pClNode == ai_pnodedoc) ? M4_TRUE : M4_FALSE;
    }

    return M4_TRUE;
}

m4return_t ClLaneManager::GetAllDocuments (ClNode_Data * ai_pnodelane, StDocumentList & ao_List)
{
    m4return_t res = M4_WARNING;

    LaneConnectionListIt_t start, end, it, it2;

    start = m_LaneConnectionList.begin();
    end   = m_LaneConnectionList.end();

    ClLaneConnection *plcon;

    it = start;
    StDocumentList * iter = &ao_List;
    StDocumentList * last;
    ClNode_Data * pNode;
    m4bool_t bFound;

    while (it != end)
    {
        plcon = &(*it);
        if (plcon->Lane.pClNode->GetpNode_Data() == ai_pnodelane)
        {
            iter = &ao_List;
            pNode = plcon->Doc.pClNode->GetpNode_Data();
            bFound = M4_FALSE;
            while (iter)
            {
                if (iter->m_pNode == pNode)
                {
                    bFound = M4_TRUE;
                    break;
                }
                last = iter;
                iter = iter->m_pNext;
            }
            if (!bFound)
            {
                if (last->m_pNode == 0)
                    last->m_pNode = pNode;
                else
                {
                    last->m_pNext = new StDocumentList();
                    last->m_pNext->m_pNode = pNode;
                }
            }
        }
        ++it;
    }
    return M4_SUCCESS;
}
m4return_t
ClLaneManager::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClLaneManager);
		
	ao_size-=sizeof(m_LaneConnectionList);
		if (GetObjectListSize(m_LaneConnectionList,ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}

	return M4_SUCCESS;
}


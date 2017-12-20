//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             lanecon.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             31/10/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Conection between two channels (the lane)
//
//==============================================================================

//----------------------------------------------------------------------------
#include "dm_def.hpp"
#include "m4define.hpp"
//----------------------------------------------------------------------------

#include "lanecon.hpp"

#include "node_knl.hpp"
#include "node.hpp"
#include "m4mdrt.hpp"

#include "chan_knl.hpp"


ClLaneConnection::ClLaneConnection ()
{
    Lane.pClChannel = 0;
    Lane.pClNode = 0;
    Doc.pClChannel = 0;
    Doc.pClNode = 0;

    m_piRefConections = 0;

    m_hPlug = 0;
}

ClLaneConnection::~ClLaneConnection ()
{
    Destroy ();
}

ClHandle ClLaneConnection::GetChildItemHandle (m4uint16_t ai_index)
{
    ClCompiledMCR *pcmcrdoc = Doc.pClChannel->GetpChannelDef ()->GetpCMCR ();
    ClHandle hitemplug = pcmcrdoc->GetPlugItemHandleByPosition (m_hPlug, ai_index);
    return pcmcrdoc->GetPlugItemSonItemHandle (hitemplug);
}

m4uint16_t ClLaneConnection::NumberOfItemsConnected (void)
{
	/* Bug 0167675
	Si no está conectado se devuelve 0
	*/
	if( m_hPlug == 0 )
	{
		return 0;
	}

    ClCompiledMCR *pcmcrdoc = Doc.pClChannel->GetpChannelDef ()->GetpCMCR ();
    return pcmcrdoc->GetPlugNumberOfItems (m_hPlug);
}

m4uint8_t ClLaneConnection::Type (void)
{
    ClCompiledMCR *pcmcrdoc = Doc.pClChannel->GetpChannelDef ()->GetpCMCR ();
    return pcmcrdoc->GetPlugType (m_hPlug);
}

m4pcchar_t ClLaneConnection::GetFatherItemName (m4uint16_t ai_index)
{
    ClCompiledMCR *pcmcrdoc = Doc.pClChannel->GetpChannelDef ()->GetpCMCR ();
    ClHandle hitemplug = pcmcrdoc->GetPlugItemHandleByPosition (m_hPlug, ai_index);
    return pcmcrdoc->GetPlugItemFatherItemId (hitemplug);
}

m4return_t ClLaneConnection::Init (ClChannel_Data *ai_pchdoc, ClNode *ai_pndoc, ClChannel_Data *ai_pchlane, ClNode *ai_pnlane)
{
    m4return_t res;

//  pasamos:
//      - handle del nodo hijo (doc)
//      - handle del que va a ser nodo padre (carril) + puntero al cmcr del padre
//  llamando al cmcr del hijo

    Lane.pClChannel = ai_pchlane;
    Lane.pClNode = ai_pnlane;

    Doc.pClChannel = ai_pchdoc;
    Doc.pClNode = ai_pndoc;

    ClCompiledMCR *pcmcrlane = Lane.pClChannel->GetpChannelDef()->GetpCMCR();    
    ClCompiledMCR *pcmcrdoc = Doc.pClChannel->GetpChannelDef()->GetpCMCR();

    ClHandle hnodelane = Lane.pClNode->GetpExtInfo()->GetHandle();
    ClHandle hnodedoc = Doc.pClNode->GetpExtInfo()->GetHandle();

	/* Bug 0104664
	A la conexión hay que pasar el nombre lógico del carril
	*/
	m4pchar_t	pcMetaIdentifier = NULL ;
	ClChannel	*poFatherChannel = Lane.pClNode->GetpAccess()->GetpChannel() ;

	res = poFatherChannel->MetaIdentifier.Get( pcMetaIdentifier ) ;

	if( res != M4_SUCCESS || pcMetaIdentifier == NULL )
	{
		pcMetaIdentifier = (m4pchar_t) poFatherChannel->GetpChannelDef()->Id() ;
	}

    // la gran llamada:
    res = pcmcrdoc->ConnectNodeToNode(hnodedoc, pcmcrlane, pcMetaIdentifier, hnodelane, m_hPlug);

	/* Bug 0102671
	Si cambia el metacanal hay que desvincular los autos de su remoto y
	marcar que la siguiente vez se serialice todo
	*/
	if( pcmcrdoc->GetChannelCsExeType() == M4CL_CSEXETYPE_AUTO )
	{
		ClChannel	*poChannel = Doc.pClNode->GetpAccess()->GetpChannel() ;

		if( poChannel->GetRemoteId() != 0 )
		{
			poChannel->SetRemoteId( 0 ) ;
			Doc.pClChannel->SetKeepStatus( M4_FALSE, 1 ) ;
		}
	}

    if (res == M4_SUCCESS)
    {
        res |= Doc.pClNode ->LaneConnectToNode_Lane(*this);
        res |= Lane.pClNode->LaneConnectToNode_Doc(*this);
    }

    return res;
}

m4return_t ClLaneConnection::Init (ClNode *ai_pndoc, ClNode *ai_pnlane, ClLaneConnection * ai_poCon)
{
    m4return_t res;

    Lane.pClChannel = ai_pnlane->GetpAccess()->GetpChannel()->GetpChannel_Data();
    Lane.pClNode = ai_pnlane;

    Doc.pClChannel = ai_pndoc->GetpAccess()->GetpChannel()->GetpChannel_Data();
    Doc.pClNode = ai_pndoc;

    m_piRefConections = ai_poCon->GetpRefConections();
    m_hPlug = ai_poCon->m_hPlug;

//    res |= Lane.pClNode->CloneConnection (*this, * ai_poCon);
    res = Lane.pClNode ->LaneConnectToNode_Doc (*this);

    return res;
}


m4return_t ClLaneConnection::Destroy (void)
{
    m4return_t res = M4_SUCCESS;

    if (Doc.pClChannel)
    {
        if ((m_piRefConections == 0) || (* m_piRefConections == 0))
		{
			ClCompiledMCR *pcmcrdoc = Doc.pClChannel->GetpChannelDef ()->GetpCMCR();

            res = pcmcrdoc->DisconnectLane();

			/* Bug 0102671
			Si cambia el metacanal hay que desvincular los autos de su remoto y
			marcar que la siguiente vez se serialice todo
			*/
			if( pcmcrdoc->GetChannelCsExeType() == M4CL_CSEXETYPE_AUTO )
			{
				ClChannel	*poChannel = Doc.pClNode->GetpAccess()->GetpChannel() ;

				if( poChannel->GetRemoteId() != 0 )
				{
					poChannel->SetRemoteId( 0 ) ;
					Doc.pClChannel->SetKeepStatus( M4_FALSE, 1 ) ;
				}
			}
		}

        if (res == M4_SUCCESS)
        {
            res |= Lane.pClNode->LaneDisConnectToNode_Doc  (*this);
            if ((m_piRefConections == 0) || (* m_piRefConections == 0))
            {
                res |= Doc.pClNode ->LaneDisConnectToNode_Lane (*this);
                if (m_piRefConections != 0)
                {
                    delete m_piRefConections;
                    m_piRefConections = 0;
                }
            }
            else
                DecRefConections();

            return res;
        }
    }

    return M4_ERROR;
}

m4return_t ClLaneConnection::DestroyTemp (void)
{
    m4return_t res = M4_SUCCESS;

    if (Doc.pClChannel)
    {
        res |= Lane.pClNode->LaneDisConnectToNode_Doc  (*this);
        res |= Doc.pClNode ->LaneDisConnectToNode_Lane (*this);
    }

    return res;
}

m4bool_t ClLaneConnection::ChannelIsEqual (ClNode *ai_pndoc, ClNode *ai_pnlane)
{
    return ( ( (Doc.pClNode->GetpNode_Data() == ai_pndoc->GetpNode_Data())
        && (Lane.pClNode->GetpNode_Data() == ai_pnlane->GetpNode_Data()) ) ? M4_TRUE : M4_FALSE );
}

m4bool_t ClLaneConnection::AccessIsEqual (ClNode *ai_pndoc, ClNode *ai_pnlane)
{
    return ( ( (Doc.pClNode == ai_pndoc) && (Lane.pClNode == ai_pnlane) ) ? M4_TRUE : M4_FALSE );
}

m4uint32_t * ClLaneConnection::GetpRefConections(void)
{
    if (m_piRefConections == 0)
    {
        m_piRefConections = new m4uint32_t;
        * m_piRefConections = 1;
    }
    else
        (* m_piRefConections) ++;

    return m_piRefConections;
}

void ClLaneConnection::DecRefConections(void)
{
    if (m_piRefConections != 0)
    {
        if (* m_piRefConections >= 1)
            (* m_piRefConections) --;
        else
        {
/*
            delete m_piRefConections;
            m_piRefConections = 0;
*/
            M4_ASSERT(0);
        }
    }
}

#ifdef _AIX
// estoy hasta los guevos del VC++
m4bool_t ClLaneConnection::operator == (const ClLaneConnection &ai_accesscon) const
{
	
	m4bool_t bLane = Lane.pClNode->GetpNode_Data() == ai_accesscon.Lane.pClNode->GetpNode_Data() ? M4_TRUE : M4_FALSE;
	m4bool_t bDoc  = Doc.pClNode->GetpNode_Data() == ai_accesscon.Doc.pClNode->GetpNode_Data() ? M4_TRUE : M4_FALSE;

	if (bLane && bDoc)
		return M4_TRUE;
	else
		return M4_FALSE;
}

m4bool_t ClLaneConnection::operator != (const ClLaneConnection &ai_accesscon) const
{
	m4bool_t bLane = Lane.pClNode->GetpNode_Data() == ai_accesscon.Lane.pClNode->GetpNode_Data() ? M4_TRUE : M4_FALSE;
	m4bool_t bDoc  = Doc.pClNode->GetpNode_Data() == ai_accesscon.Doc.pClNode->GetpNode_Data() ? M4_TRUE : M4_FALSE;

	if (bLane && bDoc)
		return M4_FALSE;
	else
		return M4_TRUE;
}

m4bool_t ClLaneConnection::operator >  (const ClLaneConnection &ai_accesscon) const
{
	return Lane.pClNode->GetpNode_Data() > ai_accesscon.Lane.pClNode->GetpNode_Data() ? M4_TRUE : M4_FALSE;
}

m4bool_t ClLaneConnection::operator <  (const ClLaneConnection &ai_accesscon) const
{
	return Lane.pClNode->GetpNode_Data() <= ai_accesscon.Lane.pClNode->GetpNode_Data() ? M4_TRUE : M4_FALSE;
}
#endif	

m4return_t 
ClLaneConnection::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClLaneConnection);

	ao_size-=sizeof(Doc);
		if (Doc.GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}

	ao_size-=sizeof(Lane);
		if (Lane.GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}

//	(*m_piRefConections). No lo cuento. Son 4 bytes compartidos entre distintos LaneConnection.
		return M4_SUCCESS;
}

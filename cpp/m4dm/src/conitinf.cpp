#include "dmres.hpp"

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             conitinf.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             27/10/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Información acerca de los items conectados a un cierto item
//
//
//==============================================================================

//----------------------------------------------------------------------------
#include "dm_def.hpp"
#include "m4define.hpp"
//----------------------------------------------------------------------------

#include "conitinf.hpp"

#include "node_knl.hpp"
#include "itemdef.hpp"
#include "nod_data.hpp"
#include "m4mdrt.hpp"

#include "itemconn.hpp"

ClConnectItemInfo::ClConnectItemInfo ()
{
    m_pool = 0;
    m_iCount = 0;
}

ClConnectItemInfo::~ClConnectItemInfo ()
{
    if (m_pool) {
        delete []((m4uint8_t *)m_pool);
        m_pool = 0;
    }
}

m4return_t ClConnectItemInfo::InitParent (ClNodeDirectory_Data &ai_nodedir, ClCompiledMCR *ai_pcmcr, ClHandle ai_hitem)
{
	ClHandle hc = ai_pcmcr->GetItemInverseConnector (ai_hitem);

	ClNode_Data* pn = 0;
	ClItemIndex ii = M4DM_EOF_INDEX;

	if (hc && hc != -1) {
		ClHandle hi = ai_pcmcr->GetItemConnectorFatherItemHandle (hc);
		M4_ASSERT (hi);
		
		ClHandle hparentnode = ai_pcmcr->GetItemNodeHandle (hi);
		M4_ASSERT (hparentnode);
		
		M4_ASSERT (ai_nodedir.Get (hparentnode));
		pn = ai_nodedir.Get (hparentnode);
		ii = ai_pcmcr->GetItemIndex (hi);
		
		//ojo pq luego el itemindex es el que se va a usar para buscar en el
		//ClItem::Parent, en item [Parent.Index]
	}
	
	Parent.Init (pn, ii);
	
	return M4_SUCCESS;
}


// aquí se utiliza la coleción de nodos del canal:

m4return_t ClConnectItemInfo::Init (ClNodeDirectory_Data &ai_nodedir, ClCompiledMCR *ai_pcmcr, ClHandle ai_hitem)
{
    m4return_t res = M4_SUCCESS;

    //sólo guardamos esta información cuando el item no es un método
    if (ai_pcmcr->GetItemType (ai_hitem) != M4CL_ITEM_TYPE_METHOD) {

        m4uint16_t i;
        m_iCount = ai_pcmcr->GetItemNumberOfConnectedItems (ai_hitem);

        if (m_iCount) {
            // se parte de que el item está conectado a otro del mismo canal

            m4return_t res;

            //comprobamos que todos estos objetos ocupen lo mismo para que no haya lios
            //con el tema del pool
            M4_ASSERT (sizeof (ClConnectorImplementation_Any_Any_Node) == sizeof (ClConnectorImplementation_BB_Block_Block));
            M4_ASSERT (sizeof (ClConnectorImplementation_Any_Any_Node) == sizeof (ClConnectorImplementation_RB_Block_Block));
            M4_ASSERT (sizeof (ClConnectorImplementation_Any_Any_Node) == sizeof (ClConnectorImplementation_Any_Any_Register));
            M4_ASSERT (sizeof (ClConnectorImplementation_Any_Any_Node) == sizeof (ClConnectorImplementation_Any_Register_Block));
            M4_ASSERT (sizeof (ClConnectorImplementation_Any_Any_Node) == sizeof (ClConnectorImplementation_Free_Any_Block));

            _AClConnectorImplementation *ppp;
            m_pool = (_AClConnectorImplementation *)new m4uint8_t [sizeof (ClConnectorImplementation_Any_Any_Node) * m_iCount];
            M4_ASSERT (m_pool);
            ppp = m_pool;

            
            StInfo sinfo;


            for (i = 0; i < m_iCount; i++) {

                ClConnectItemInfo::s_f (sinfo, ai_pcmcr, ai_hitem, i);

                //susrutina:
                ClNode_Data *pnchild;
                m4uint16_t ichildnodepos;
                {
                    pnchild = ai_nodedir.Get (ClHandle(sinfo.hchildnode));
                    M4_ASSERT (pnchild);

                    //nodo padre:
                    m4uint32_t hfathernode;
                    hfathernode = ai_pcmcr->GetItemNodeHandle (ai_hitem);
                    M4_ASSERT (hfathernode);
                    ClNode_Data *pnfather;
                    pnfather = ai_nodedir.Get (ClHandle(hfathernode));
                    M4_ASSERT (pnfather);
        

                    if ((sinfo.hchildnode == hfathernode) || (sinfo.iconnectiontype == M4CL_NODE_CONNECTOR_TYPE_FREE)) {
                        //estamos conectados con nosotros mismos, ok
                        //ó
                        //la conexión es free, en este caso no se utiliza el ichildnodepos, con lo cual ok
                        ichildnodepos = 0xffff; //16bits
                    }
                    else {            
                        //indice de la conexión:
                        res = pnfather->GetpExtInfo ()->ChildNodeRB.GetPos (sinfo.hchildnode, ichildnodepos);
                        if (res == M4_ERROR) {
                            //no encontrado
                            res = pnfather->GetpExtInfo ()->ChildNodeBB.GetPos (sinfo.hchildnode, ichildnodepos);

                            M4_ASSERT (res != M4_ERROR);    //lo tiene que haber encontrado por vuebos
                        }
                    }
                }

                res = Add (
                        sinfo.ichildscope,
                        sinfo.ifatherscope, 
                        sinfo.iconnectiontype,
                        sinfo.iconnectionrelation,
                        ppp
                    );

                if (M4_ERROR == res)
                {
                    break;
                }
            
                ppp->Init (pnchild, sinfo.hchilditem, ichildnodepos);

                ppp++;
            }
        }
    }

    return InitParent (ai_nodedir, ai_pcmcr, ai_hitem);
}

m4uint16_t ClConnectItemInfo::Count (void) const
{
    return m_iCount;
}

//----------------------------------------------------------------------------
// Add
//      puntero al nodo 'hijo'
//      handle del item 'hijo'
//      ai_ichildscope, scope del item 'hijo'
//      ai_ifatherscope, scope del item 'padre'
//      ai_iconnectiontype, tipo de conexión entre los nodos
//----------------------------------------------------------------------------

//static
void ClConnectItemInfo::s_f (StInfo &ao_info, ClCompiledMCR *ai_pcmcr, ClHandle ai_hitem, m4uint16_t ai_i)
{    
    m4uint32_t hcon;

    hcon = ai_pcmcr->GetItemConnectorHandleByPosition (ai_hitem, ai_i);
    M4_ASSERT (hcon);
    ao_info.hchilditem = ai_pcmcr->GetItemConnectorSonItemHandle (hcon);
    M4_ASSERT (ao_info.hchilditem);
    //nodo hijo:
    ao_info.hchildnode = ai_pcmcr->GetItemNodeHandle (ao_info.hchilditem);
    M4_ASSERT (ao_info.hchildnode);

    ao_info.ifatherscope = ai_pcmcr->GetItemScope (ai_hitem);
    ao_info.ichildscope = ai_pcmcr->GetItemScope (ao_info.hchilditem);

    m4uint32_t hnodecon = ai_pcmcr->GetItemConnectorNodeConnectorHandle (hcon);

    ao_info.iconnectionrelation = ai_pcmcr->GetItemConnectorRelation(hcon);

// jesus, lo del conector a nulo ============
//            M4_ASSERT (hnodecon);
	if( hnodecon == 0 )
	{
		ao_info.iconnectiontype = M4CL_NODE_CONNECTOR_TYPE_AUTO ;
	}
	else
	{
		ao_info.iconnectiontype = ai_pcmcr->GetNodeConnectorType (hnodecon);
	}
}

void ClConnectItemInfo::Refresh (ClCompiledMCR *ai_pcmcr, ClHandle ai_hitem)
{
    m4return_t res;    

    if (m_iCount) {
        _AClConnectorImplementation *ppp = m_pool;
        StInfo sinfo;

        m4uint16_t i;
        for (i = 0; i < m_iCount; i++) {

            ClConnectItemInfo::s_f (sinfo, ai_pcmcr, ai_hitem, i);
            
            _AClConnectorImplementation *p;

            //copia de seguridad:            
            ClNode_Data *pnodechild    = ppp->GetpNode_Data ();
            //el hp del constructor que se llama en s_... borra el contenido del objeto
            ClHandle     hitemchild    = ppp->GetHandle ();
            m4uint16_t   ichildnodepos = ppp->GetChildNodePos ();

            res = _AClConnectorImplementation::s_GetNewConnectorImplementation (p,
                sinfo.ichildscope, sinfo.ifatherscope, sinfo.iconnectiontype,
                sinfo.iconnectionrelation, ppp);

            //tiramos de la copia:
            ppp->Init (pnodechild, hitemchild, ichildnodepos);

            ppp++;
        }
    }
}

m4return_t ClConnectItemInfo::Add (m4uint8_t ai_ichildscope, m4uint8_t ai_ifatherscope, m4uint8_t ai_iconnectiontype, m4uint8_t ai_iconnectionrelation, _AClConnectorImplementation *&ao_ppp)
{
    m4return_t res;

    //M4_ASSERT (ai_pNode);

    if (ao_ppp == 0) {
        // nos están llamando desde fuera del init => caso carril
        // se tira el pool y se crea uno nuevo para alojar al nuevo elemento
        _AClConnectorImplementation *paux;
        paux = (_AClConnectorImplementation *)new m4uint8_t [sizeof (ClConnectorImplementation_Any_Any_Node) * (m_iCount + 1)];
        M4_ASSERT (paux);

        memcpy (paux, m_pool, sizeof (ClConnectorImplementation_Any_Any_Node) * m_iCount);

        delete []((m4uint8_t *)m_pool);

        m_pool = paux;
        ao_ppp = m_pool + m_iCount;
        m_iCount ++;
    }

    _AClConnectorImplementation *p;
    res = _AClConnectorImplementation::s_GetNewConnectorImplementation (p,
        ai_ichildscope, ai_ifatherscope, ai_iconnectiontype, ai_iconnectionrelation,
        ao_ppp);
    
    return res;
}

_AClConnectorImplementation *ClConnectItemInfo::operator [] (m4uint16_t ai_index) const
{
    M4_ASSERT (ai_index < m_iCount);
    M4_ASSERT (m_pool);
    
    return m_pool + ai_index;
}
m4return_t 
ClConnectItemInfo::GetSize(ClSize &ao_size)
{
	ao_size += sizeof(ClConnectItemInfo);
		ao_size -= sizeof(Parent);

	if (Parent.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	
	for (m4uint32_t i=0;i<m_iCount;++i)
	{
		if (m_pool[i].GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}
	return M4_SUCCESS;
}

m4return_t ClConnectItemInfo::ClParent::GetSize(ClSize &ao_size)
{
	ao_size += sizeof(ClConnectItemInfo::ClParent);
	return M4_SUCCESS;
}

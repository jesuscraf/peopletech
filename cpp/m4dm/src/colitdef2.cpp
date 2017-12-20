//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             colitdef2.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             26/08/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    This module defines class ClNodeDef, static info about a node
//
//
//==============================================================================

//## begin module.includes preserve=yes
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

#include "colitdef2.hpp"

#include "item_prx.hpp"
//#include "nodedef.hpp"

#include "node_knl.hpp"
//## end module.includes preserve=yes



//////////////////////////////////////////////////////////////////////////////
//
// ClColItemDef_Kernel
//

ClColItemDef_Kernel::ClColItemDef_Kernel ()
{
    m_pPool = 0;
    m_iCount = 0;

    //s_DonNegativo.Handle = 0;
}

ClColItemDef_Kernel::~ClColItemDef_Kernel()
{
    Destroy ();
}

void ClColItemDef_Kernel::Destroy (void)
{
    if (m_pPool) {
        //a ver, hay que llamar a mano a los destructores de los proxys del pool, ahora
        //cada uno de ellos tiene información adicional de los items conectados:
        PATRONITEMPROXY *p = m_pPool;
        while (m_iCount) {
            delete (_AClItemProxy *)p;
            p++;
            m_iCount--;
        }

        delete []((m4uint8_t *)m_pPool);
        m_pPool = 0;
    }
}

void ClColItemDef_Kernel::CalcPos (void)
{
    m4uint16_t i;

    M4_ASSERT (m_pClNodeDef);
    m_iCount = m_pClNodeDef->NumberOfItems ();

    m4uint16_t iRPos = 0;
    m4uint16_t iBPos = 0;
    m4uint16_t iNPos = 0;
    m4uint16_t pos;

    m4uint16_t type;
    m4uint16_t scope;

    _AClItemProxy *pitemproxy = m_pPool;

    for (i = 0; i < m_iCount; i++) {
        // creación de itemproxy adecuado:
        type = m_pClNodeDef->GetNodeItemType (i);
        scope = m_pClNodeDef->GetNodeItemScope (i);

        if (type != M4CL_ITEM_TYPE_METHOD) {
            switch (scope) {
            case M4CL_ITEM_SCOPE_NODE:            
                pos = iNPos++;
                break;
        
            case M4CL_ITEM_SCOPE_BLOCK:            
                pos = iBPos++;
                break;
        
            case M4CL_ITEM_SCOPE_REGISTER:
                pos = iRPos++;
                break;

            default:
                M4_ASSERT (0);
            }
        }
        else {
            //para que no cante el purify:
            pos = (m4uint16_t)-1;
        }

        pitemproxy->SetPos (pos);

        pitemproxy = ((PATRONITEMPROXY *)pitemproxy) + 1;
    }
}

m4return_t ClColItemDef_Kernel::Init (ClNodeDef &ai_nodedef, ClNodeDirectory_Data &ai_nodedir)
{
    m4return_t res = M4_SUCCESS;

    // _Init puede llamarse varias veces, por eso primero hacemos Destroy:
    Destroy ();

    m_pClNodeDef = &ai_nodedef;

    m4uint16_t i;
    m_iCount = m_pClNodeDef->NumberOfItems ();

    m_pPool = (PATRONITEMPROXY *)new m4uint8_t [m_iCount * sizeof (PATRONITEMPROXY)];
    _AClItemProxy *pitemproxy = m_pPool;

    m4uint16_t type;
    m4uint16_t scope;

    for (i = 0; i < m_iCount; i++) {
        // creación de itemproxy adecuado:
        type = m_pClNodeDef->GetNodeItemType (i);
        scope = m_pClNodeDef->GetNodeItemScope (i);
        _CreateItemProxy (type, scope, pitemproxy);

        res = pitemproxy->Init (ai_nodedir, m_pClNodeDef->GetpCMCR (), m_pClNodeDef->GetNodeItemHandle (i));
        if (res == M4_ERROR)
            break;

        pitemproxy = ((PATRONITEMPROXY *)pitemproxy) + 1;
    }
    
//y ahora actualizo el 'pos' 
    CalcPos ();

    return res;
}

//llamada cuando nos 'encarrilamos'

void ClColItemDef_Kernel::Refresh (ClNode_Data *ai_pchildnode)
{
    m4uint16_t i;
    _AClItemProxy *pitemproxy = m_pPool;

    m4uint16_t type;
    m4uint16_t scope;

    //StItemProxyData buffer; //buffer auxiliar
	
	//Mucho ojo, que esto dió un Gp dificil de pillar: con buffer como variable local,
	//al salir se llama al destructor, buffer tenía un objeto Coniteminfo, con un destructor
	//que elimina el m_pool, el problema que es no se debe destruir puesto que pertenece al objeto
	//itemproxy !!!!!

	m4uint8_t buffer [sizeof (StItemProxyData)];

    for (i = 0; i < m_iCount; i++) {
        // creación de itemproxy adecuado:
        type = m_pClNodeDef->GetNodeItemType (i);
        scope = m_pClNodeDef->GetNodeItemScope (i);

        //me copio la información del item_proxy original:
        //mejora: hacerlo sólo para aquellos item_prx cuyo scope o tipo haya cambiado
        //y que no sean métodos:
        _AClItemProxy::Type_e originaltype = pitemproxy->Type ();
        if (originaltype != _AClItemProxy::Method) {
            
            m4bool_t bchanged = M4_FALSE;

            switch (scope) {
                case M4CL_ITEM_SCOPE_NODE:
                    bchanged = M4_BOOL(originaltype != _AClItemProxy::Node);
                    break;

                case M4CL_ITEM_SCOPE_BLOCK:
                    bchanged = M4_BOOL(originaltype != _AClItemProxy::Block);
                    break;

                case M4CL_ITEM_SCOPE_REGISTER:
                    bchanged = M4_BOOL(originaltype != _AClItemProxy::Register);
                    break;
            }

            if (bchanged) {
                memcpy (&buffer, pitemproxy->GetpData (), sizeof (StItemProxyData));

                _CreateItemProxy (type, scope, pitemproxy);

                //le devuelvo su información:
                memcpy (pitemproxy->GetpData (), &buffer, sizeof (StItemProxyData));

                //actualizo la información de la conexión
                pitemproxy->GetpConnectItemInfo ()->Refresh (ai_pchildnode->GetpNodeDef ()->GetpCMCR (), pitemproxy->GetHandle ());
            }
        }

        pitemproxy = ((PATRONITEMPROXY *)pitemproxy) + 1;
    }
    
//y ahora actualizo el 'pos', que también puede haber cambiado
    CalcPos ();
}

m4return_t ClColItemDef_Kernel::SetSliceMode (m4bool_t ai_bsenable)
{
    M4_ASSERT (m_pClNodeDef);
    M4_ASSERT (m_pPool);
    
    M4_ASSERT (m_iCount == m_pClNodeDef->NumberOfItems ());

    _AClItemProxy *pitemproxy = m_pPool;

    m4uint16_t i;
    for (i = 0; i < m_iCount; i++) {
        pitemproxy->InitValue (m_pClNodeDef->GetpCMCR (), ai_bsenable);

        pitemproxy = ((PATRONITEMPROXY *)pitemproxy) + 1;
    }

    return M4_SUCCESS;
}

m4return_t ClColItemDef_Kernel::InitSecurity (void)
{
    M4_ASSERT (m_pClNodeDef);
    M4_ASSERT (m_pPool);
    
    M4_ASSERT (m_iCount == m_pClNodeDef->NumberOfItems ());

    _AClItemProxy *pitemproxy = m_pPool;

    m4uint16_t i;
    for (i = 0; i < m_iCount; i++) {
        pitemproxy->InitSecurity (m_pClNodeDef->GetpCMCR ());
        pitemproxy = ((PATRONITEMPROXY *)pitemproxy) + 1;
    }

    return M4_SUCCESS;
}

_AClItemProxy *ClColItemDef_Kernel::GetByIndex (ClItemIndex ai_itemindex)
{
    M4_ASSERT (m_pPool);
    _AClItemProxy *p;

    if (ai_itemindex < m_iCount) {
        //ok:        
        p = ((PATRONITEMPROXY *)m_pPool) + ai_itemindex;
    }
    else {
        // eof
        p = &s_oClItemProxy_Nihil;
    }

    return p;
}

_AClItemProxy *ClColItemDef_Kernel::GetByHandle (ClHandle ai_handle)
{
    M4_ASSERT (m_pPool);
    _AClItemProxy *p;

    //utilizamos de momento el NodeDef (el cmcr en definitiva) para las búsquedas
    M4_ASSERT (m_pClNodeDef);
    
    if (ai_handle) {
        // como no exista el item nos la cargamos:
        m4uint32_t i = m_pClNodeDef->GetItemIndex (ai_handle);
        p = ((PATRONITEMPROXY *)m_pPool) + i;
    }
    else {
        // eof
        p = &s_oClItemProxy_Nihil;
    }

    return p;
}

_AClItemProxy *ClColItemDef_Kernel::GetById (m4pcchar_t ai_pcId)
{
    M4_ASSERT (m_pPool);
    _AClItemProxy *p;

    //utilizamos de momento el NodeDef (el cmcr en definitiva) para las búsquedas
    M4_ASSERT (m_pClNodeDef);

    ClHandle hi = m_pClNodeDef->GetNodeItemHandle (ai_pcId);

    if (hi) {
        p = GetByHandle (hi);
    }
    else {
        // eof
        p = &s_oClItemProxy_Nihil;        
    }

    return p;
}

void ClColItemDef_Kernel::_CreateItemProxy (m4uint16_t ai_type, m4uint16_t ai_scope,_AClItemProxy *ai_paddress)
{
    switch (ai_type) {
        case M4CL_ITEM_TYPE_METHOD:
            new (ai_paddress) ClItemProxy_Method ();
            break;

        default:
            switch (ai_scope) {
            case M4CL_ITEM_SCOPE_NODE:
                new (ai_paddress) ClItemProxy_Node ();
                break;

            case M4CL_ITEM_SCOPE_BLOCK:
                new (ai_paddress) ClItemProxy_Block ();
                break;

            case M4CL_ITEM_SCOPE_REGISTER:
                new (ai_paddress) ClItemProxy_Register ();
                break;
            }
    }
}
m4return_t 
ClColItemDef_Kernel::GetSize(ClSize &ao_size)
{
	ao_size += sizeof(ClColItemDef_Kernel);
	
	ao_size += m_iCount*sizeof(*m_pPool);

	if (m_pPool){
		for (m4uint32_t i=0;i<m_iCount;++i)
		{
			ao_size -= sizeof(*m_pPool);
			if (m_pPool[i].GetSize(ao_size)!=M4_SUCCESS){
				return M4_ERROR;
			}
		}
	}

	return M4_SUCCESS;
}


#include "dmres.hpp"
#include "cldefine.hpp"
#include "chlog.hpp"

#include "itemconn.hpp"
#include "node_knl.hpp"
#include "pl_data.hpp"
#include "px_block.hpp"
#include "blockref.hpp"
#include "compst_common.hpp"

#include "arecset.hpp"
#include "node_x.hpp"
#include "access.hpp"

#include "laneinfo.hpp"
#include "m4mdrt.hpp"

#include "medvarin.hpp"


//conexión free:
// child\father>   R           B           N
//
//   R           error       error       error
//
//   B          _Free_AB    _Free_AB    _Free_AB
//
//   N           _A_AN       _A_AN       _A_AN
//

//conexión not free:
// child\father>   R           B           N
//
//   R           _A_AR       _A_AR       _A_AR
//
//   B           _A_RB   _RB_BB,_BB_BB   error
//
//   N           _A_AN       _A_AN       _A_AN
//

// gestión de memoria especializada, utilizamos un pool, en vez de alocar las
// instancias ItemInfo de una en una

void *_AClConnectorImplementation::operator new (size_t, void *ai_pbuffer)
{
    return ai_pbuffer;  // dirección del pool
}

void _AClConnectorImplementation::operator delete (void *)
{
}

/*
_AClConnectorImplementation::_AClConnectorImplementation (ClNode_Data *ai_pNodeChild, ClHandle ai_hitemchild, m4uint16_t ai_ichildnodepos)
{
    m_pNodeChild = ai_pNodeChild;
    m_hItemChild = ai_hitemchild;
    m_ichildnodepos = ai_ichildnodepos;

    M4_ASSERT (m_pNodeChild);
    M4_ASSERT (m_hItemChild);
}
*/

m4return_t _AClConnectorImplementation::s_GetNewConnectorImplementation (_AClConnectorImplementation *&ao_p, m4uint8_t ai_ichildscope, m4uint8_t ai_ifatherscope, m4uint8_t ai_iconnectiontype, m4uint8_t ai_iconnectionrelation, _AClConnectorImplementation *ai_pdir)
{
    m4return_t res = M4_SUCCESS;

    M4_ASSERT (ai_pdir);

    switch (ai_ichildscope) {
        case M4CL_ITEM_SCOPE_REGISTER:
            {
            switch (ai_iconnectiontype)
            {
            case M4CL_NODE_CONNECTOR_TYPE_FREE:
                // chungo, conexión no permitida ¿a qué registro en el hijo (que es free) se asigna?
                // ¿al current?, sí, pero al current de qué access?
                ao_p = new (ai_pdir) ClConnectorImplementation_Null_Any_Any();
                break;
            case M4CL_NODE_CONNECTOR_TYPE_AUTO:
                if (ai_ifatherscope == M4CL_ITEM_SCOPE_REGISTER)
                    ao_p = new (ai_pdir) ClConnectorImplementation_Auto_Register_Register ();
                else
                {
                    ao_p = new (ai_pdir) ClConnectorImplementation_Null_Any_Any();
                }
                break;
            default:
                switch (ai_ifatherscope) {
                    case M4CL_ITEM_SCOPE_REGISTER:
                    case M4CL_ITEM_SCOPE_BLOCK:
                    case M4CL_ITEM_SCOPE_NODE:
                        ao_p = new (ai_pdir) ClConnectorImplementation_Any_Any_Register ();
                        break;
                    default:
                        ao_p = new (ai_pdir) ClConnectorImplementation_Null_Any_Any();
                        DUMP_CHLOG_DEBUGF (M4_CH_DM_INVALIDSCOPE, ai_ifatherscope);
                        res = M4_ERROR;
                }
                break;
            }
            }
            break;

        case M4CL_ITEM_SCOPE_BLOCK:
            if (ai_iconnectiontype == M4CL_NODE_CONNECTOR_TYPE_FREE) {
                ao_p = new (ai_pdir) ClConnectorImplementation_Free_Any_Block ();
            }
            else {
                switch (ai_ifatherscope) {

                    case M4CL_ITEM_SCOPE_REGISTER:
                        ao_p = new (ai_pdir) ClConnectorImplementation_Any_Register_Block ();
                        break;

                    case M4CL_ITEM_SCOPE_BLOCK:
                        switch (ai_iconnectiontype) {
                            case M4CL_NODE_CONNECTOR_TYPE_RB:
                                ao_p = new (ai_pdir) ClConnectorImplementation_RB_Block_Block ();
                                break;

                            case M4CL_NODE_CONNECTOR_TYPE_BB:
                                ao_p = new (ai_pdir) ClConnectorImplementation_BB_Block_Block ();
                                break;

                            case M4CL_NODE_CONNECTOR_TYPE_AUTO:
                                ao_p = new (ai_pdir) ClConnectorImplementation_Free_Any_Block ();
                                break;
                            default:
                                ao_p = new (ai_pdir) ClConnectorImplementation_Null_Any_Any();
                        }
                        break;

                    case M4CL_ITEM_SCOPE_NODE:
                        ao_p = new (ai_pdir) ClConnectorImplementation_Null_Any_Any();
                        break;

                    default:
                        ao_p = new (ai_pdir) ClConnectorImplementation_Null_Any_Any();
                        DUMP_CHLOG_DEBUGF (M4_CH_DM_INVALIDSCOPE, ai_ifatherscope);
                        res = M4_ERROR;            
                }
            }
            break;

        case M4CL_ITEM_SCOPE_NODE:
            ao_p = new (ai_pdir) ClConnectorImplementation_Any_Any_Node ();
            break;

        default:
            ao_p = new (ai_pdir) ClConnectorImplementation_Null_Any_Any();
            DUMP_CHLOG_DEBUGF (M4_CH_DM_INVALIDSCOPE, ai_ichildscope);
            res = M4_ERROR;
    }

    return res;
}

void ClConnectorImplementation_Any_Any_Node::SetChildValue (ClItem *ai_pitem)
{
    m4return_t res;

    // DE xxx A NODO:
    M4_ASSERT (ai_pitem);
    M4_ASSERT (m_pNodeChild);
    M4_ASSERT (m_hItemChild);

    // me creo un item temporal:
    ClItem item (ai_pitem->GetpAccess(), M4DM_SCOPE_NODE);
    item.Attach (m_pNodeChild);    //que facil !

    m4VariantType var;
    ai_pitem->Value.Get (var, &res);
    if (res != M4_SUCCESS)
    {
        m4uint32_t ItemHandle = ai_pitem->ItemDef.GetHandle();
        ClCompiledMCR * pmcr  = ai_pitem->ItemDef.GetpCMCR();
        DUMP_CHLOG_WARNINGF(M4_CH_DM_SEC_GET_IN_PARENT, M4ObjNodeItemI(pmcr, ItemHandle));
    }

    item.MoveTo (m_hItemChild);
    M4_ASSERT (item.GetIndex() != M4DM_EOF_INDEX);
	/* Bug 0087819 En la propagación no se cambia el estado*/
    item.Value.SetWithOutSec (var, M4_FALSE);
}

void ClConnectorImplementation_Free_Any_Block::SetChildValue (ClItem *ai_pitem)
{
    m4return_t res;

    // DE xxx A NODO:
    M4_ASSERT (ai_pitem);
    M4_ASSERT (m_pNodeChild);
    M4_ASSERT (m_hItemChild);

	// Obtenemos el valor del ítem padre.
    m4VariantType var;
    ai_pitem->Value.Get (var, &res);
    if (res != M4_SUCCESS) {
        m4uint32_t ItemHandle = ai_pitem->ItemDef.GetHandle();
        ClCompiledMCR * pmcr  = ai_pitem->ItemDef.GetpCMCR();
        DUMP_CHLOG_WARNINGF(M4_CH_DM_SEC_GET_IN_PARENT, M4ObjNodeItemI(pmcr, ItemHandle));
    }

    ClItem item (ai_pitem->GetpAccess(), M4DM_SCOPE_NODE | M4DM_SCOPE_BLOCK);

	ClBlockReference br;
	ClNode_Data* pNodeData = ai_pitem->GetpNode_Data();
    if (pNodeData != m_pNodeChild)
	    m_pNodeChild->GetpUniqueBlock (br);
    else
        br = ai_pitem->GetCurrentBlockRef();

	if (br.GetpProxy()->IsNihil()) {
		// Si es bloque un bloque nihil es porque no se a podido determinar el bloque correspondiente.
		// Esto puede suceder, por ejemplo, con una conxión free entre dos nodos root, donde el hijo
		// de la relación a sido encarrilado. Al pasar de monobloque a multibloque la relación entre
		// bloques ya no es unívoca.
		ClCompiledMCR* pCMCR      = pNodeData->GetpNodeDef()->GetpCMCR();
		ClCompiledMCR* pChildCMCR = m_pNodeChild->GetpNodeDef()->GetpCMCR();
		ClHandle itemHandle       = ai_pitem->ItemDef.GetHandle();
		ClVariantConversor variantConversor;
		variantConversor.ToCharString(var);
		
		DUMP_CHLOG_ERRORF(M4_CH_DM_PROPAGATE_BLOCK_NIHIL_IN_SON, (m4pchar_t)var << M4ObjNodeItemI(pCMCR, itemHandle) << M4ObjNodeItemI(pChildCMCR, m_hItemChild));
		return;
	}

	// Nos colocamos sobre el ítem hijo y hacemos el set del valor.
    item.SetCurrentBlock (br);
    item.MoveTo (m_hItemChild);
    M4_ASSERT (item.GetIndex() != M4DM_EOF_INDEX);
	/* Bug 0087819 En la propagación no se cambia el estado*/
    item.Value.SetWithOutSec (var, M4_FALSE);
}

void ClConnectorImplementation_BB_Block_Block::SetChildValue (ClItem *ai_pitem)
{
    m4return_t res;

    // DE BLOQUE A BLOQUE:
    M4_ASSERT (ai_pitem);
    M4_ASSERT (m_pNodeChild);
    M4_ASSERT (m_hItemChild);
    M4_ASSERT (m_ichildnodepos != 0xffff);  //16bits

    // compruebo que el bloque activo es válido:
    ClBlock_Channel_Proxy *pfatherblock = ai_pitem->GetCurrentBlock ();
    ClNode_Data *pfathernode = ai_pitem->GetpNode_Data ();
    M4_ASSERT (pfatherblock);

    if (pfatherblock->IsValid ()) {

		ClBlockReference br;
        //ClBlock_Channel_Proxy *pchildblock;

        //a pelo:
        ClNode_Data *pnchild = pfathernode->GetpExtInfo ()->ChildNodeBB [(ClChildIndex)m_ichildnodepos].GetAddress ();
        M4_ASSERT (pnchild);
        M4_ASSERT (pnchild->GetpBlockStore ());
		//si no lo encuentra que no lo cree:
        pnchild->GetpBlockStore ()->Get (pfatherblock->ID.BlockUniqueKey, 0/*no se usa*/, br);
        if (br) {
			// efectivamente es BB:
			// compruebo que el bloque activo es válido:
			if (br->IsValid ()) {
				ClItem item (ai_pitem->GetpAccess(), M4DM_SCOPE_NODE | M4DM_SCOPE_BLOCK);
				//item.Attach ();
				item.SetCurrentBlock (br);

				m4VariantType var;
				ai_pitem->Value.Get (var, &res);
                if (res != M4_SUCCESS)
                {
                    m4uint32_t ItemHandle = ai_pitem->ItemDef.GetHandle();
                    ClCompiledMCR * pmcr  = ai_pitem->ItemDef.GetpCMCR();
                    DUMP_CHLOG_WARNINGF(M4_CH_DM_SEC_GET_IN_PARENT, M4ObjNodeItemI(pmcr, ItemHandle));
                }
				item.MoveTo (m_hItemChild);
				M4_ASSERT (item.GetIndex() != M4DM_EOF_INDEX);
				/* Bug 0087819 En la propagación no se cambia el estado*/
				item.Value.SetWithOutSec (var, M4_FALSE);
			}
			else {
				br->SetConnectedChange();
			}

			br.Reset ();
		}
    }
    else {
        //comprobar pq el bloque puede no ser válido
    }
}

void ClConnectorImplementation_RB_Block_Block::SetChildValue (ClItem *ai_pitem)
{
    m4return_t res;

    // RB
    M4_ASSERT (ai_pitem);
    M4_ASSERT (m_pNodeChild);
    M4_ASSERT (m_hItemChild);
    M4_ASSERT (m_ichildnodepos != 0xffff);  //16 bits

    // compruebo que el bloque activo es válido:
    ClBlock_Channel_Proxy *pfatherblock = ai_pitem->GetCurrentBlock ();
    M4_ASSERT (pfatherblock);

    if (pfatherblock->IsValid ()) {
        ClRegister regfather (ai_pitem->GetpAccess(), M4_FALSE);   //sin autoload
        regfather.SetCurrentBlock (pfatherblock);

        m4uint32_t nr = regfather.GetpNode_Data()->GetpNodeDef()->NumRows();
        m4uint32_t first, last, i;

        if (nr)
        {
            ClBlockReference br = regfather.GetCurrentBlock();
            first = br->GetpPL_Data()->GetFirstVisible();
            last  = br->GetpPL_Data()->GetLastVisible();
        }
        else
        {
            first = 0;
            last  = regfather.Count();
        }

        // se itera a lo largo de todos los registros y
        // aplico la llamada a todos los bloques

        //ClHandle hchildnode = m_pNodeChild->NodeDef.GetHandle ();
        m4VariantType var;
        ClBlock_Channel_Proxy *pchildblock;

        ClItem item (ai_pitem->GetpAccess(), M4DM_SCOPE_NODE | M4DM_SCOPE_BLOCK);
        //item.Attach ();

        for (i = first; i < last; i ++)
        {
            regfather.MoveTo(i);

			// Bug 0301844 Se usa la función que hace la gestión de carril
			ClBlockReference br;
			regfather.GetChildBlock (m_ichildnodepos, br);
			pchildblock = br.GetpProxy ();

            // compruebo que el bloque activo es válido:
            if (pchildblock->IsValid ()) {
                item.SetCurrentBlock (pchildblock);

                ai_pitem->Value.Get (var, &res);
                if (res != M4_SUCCESS)
                {
                    m4uint32_t ItemHandle = ai_pitem->ItemDef.GetHandle();
                    ClCompiledMCR * pmcr  = ai_pitem->ItemDef.GetpCMCR();
                    DUMP_CHLOG_WARNINGF(M4_CH_DM_SEC_GET_IN_PARENT, M4ObjNodeItemI(pmcr, ItemHandle));
                }

                item.MoveTo (m_hItemChild);
                M4_ASSERT (item.GetIndex() != M4DM_EOF_INDEX);
				/* Bug 0087819 En la propagación no se cambia el estado*/
                item.Value.SetWithOutSec (var, M4_FALSE);
            }
			else {
				pchildblock->SetConnectedChange();
			}
        }
    }
    else {
        //comprobar pq el bloque puede no ser válido
    }
}

void ClConnectorImplementation_Any_Any_Register::SetChildValue (ClItem *ai_pitem)
{
    // El item destino es de registro (el caso especial de los históricos, 
    // en que se hacen de registro para que al cargar en cada registro haya
    // un valor, pero se ponen conectados por > y < con el padre).
    // En este caso no se hace nada, no ha lugar a la propagación
}

void ClConnectorImplementation_Auto_Register_Register::SetChildValue (ClItem *ai_pitem)
{
    m4return_t res;

	ClItem item = *ai_pitem;

	m4VariantType var;
	ai_pitem->Value.Get (var, &res);
    if (res != M4_SUCCESS)
    {
        m4uint32_t ItemHandle = ai_pitem->ItemDef.GetHandle();
        ClCompiledMCR * pmcr  = ai_pitem->ItemDef.GetpCMCR();
        DUMP_CHLOG_WARNINGF(M4_CH_DM_SEC_GET_IN_PARENT, M4ObjNodeItemI(pmcr, ItemHandle));
    }

	item.MoveTo (m_hItemChild);
	/* Bug 0087819 En la propagación no se cambia el estado*/
	item.Value.SetWithOutSec (var, M4_FALSE);
}

void ClConnectorImplementation_Any_Register_Block::SetChildValue (ClItem *ai_pitem)
{
    m4return_t res;

    // DE REGISTRO A BLOQUE:
    // RB
    // compruebo que el bloque activo es válido:
    ClBlock_Channel_Proxy *pfatherblock = ai_pitem->GetCurrentBlock ();
    ClNode_Data *pfathernode = ai_pitem->GetpNode_Data ();

    if (pfatherblock->IsValid ()) {
        //obtengo el índice al que está asociado el item:
        ClMMIndex mmi = ai_pitem->Value.GetMMIndex ();

		m4VariantType var;
		ai_pitem->Value.Get (var, &res);
        if (res != M4_SUCCESS) {
            m4uint32_t ItemHandle = ai_pitem->ItemDef.GetHandle();
            ClCompiledMCR * pmcr  = ai_pitem->ItemDef.GetpCMCR();
            DUMP_CHLOG_WARNINGF(M4_CH_DM_SEC_GET_IN_PARENT, M4ObjNodeItemI(pmcr, ItemHandle));
        }

		ClBlockReference br;
        ClNode_Data* pnchild = 0;
        ClNode* pnode = 0;
		ClNode* pnauxchild = 0;
		ClRegister * poReg = 0;

        //a pelo:
        m4uint32_t n = pfathernode->GetpExtInfo ()->ChildNodeRB.Count();
        if (m_ichildnodepos < n) {
            pnchild = pfathernode->GetpExtInfo ()->ChildNodeRB [(ClChildIndex)m_ichildnodepos].GetAddress ();
            poReg = new ClRegister(ai_pitem->GetpAccess());
        }
        else {
            pnode = ai_pitem->GetpNodeAccess();
            if (pnode) {
                if (pnode->GetpLane_Info()) {
                    pnauxchild = pnode->GetpLane_Info()->ChildNodeRB [(ClChildIndex)(m_ichildnodepos - n)].GetAddress ();
                    pnchild = pnauxchild->GetpNode_Data();
                    poReg = new ClRegister(pnauxchild->GetpAccess());
                }
            }
        }

		if (!pnchild) {
			ClCompiledMCR* pCMCR      = pfathernode->GetpNodeDef()->GetpCMCR();
			ClCompiledMCR* pChildCMCR = m_pNodeChild->GetpNodeDef()->GetpCMCR();
			ClHandle itemHandle       = ai_pitem->ItemDef.GetHandle();
			ClVariantConversor variantConversor;
			variantConversor.ToCharString(var);

			DUMP_CHLOG_WARNINGF(M4_CH_DM_PROPAGATE_BLOCK_NULL_IN_SONMETH, (m4pchar_t)var << M4ObjNodeItemI(pCMCR, itemHandle) << M4ObjNodeItemI(pChildCMCR, m_hItemChild));
			return;
		}

        pnchild->GetpBlockStore ()->Get (pfatherblock->ID.BlockUniqueKey, mmi, br);
		if (br) {
			// compruebo que el bloque hijo también sea válido:
			if (br->IsValid ()) {
        		poReg->SetCurrentBlock (br);

				if (poReg->Item.MoveTo (m_hItemChild))
				{
					/* Bug 0087819 En la propagación no se cambia el estado*/
    			    poReg->Item.Value.SetWithOutSec (var, M4_FALSE);
				}
			}
			else {
				br->SetConnectedChange();
			}

			br.Reset ();
		}
        if (poReg)
            delete poReg;
    }
}

m4return_t 
_AClConnectorImplementation::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(_AClConnectorImplementation);
		ao_size-=sizeof(m_hItemChild);

    if (m_hItemChild.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	return M4_SUCCESS;
}



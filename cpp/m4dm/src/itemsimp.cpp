#include "dmres.hpp"

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             item.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             13/06/97
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

//## begin module.includes preserve=yes
//----------------------------------------------------------------------------
#include "dm_def.hpp"
#include "m4define.hpp"
#include "itemsimp.hpp"
#include "chan_knl.hpp"
#include "node_knl.hpp"
#include "px_block.hpp"
#include "checkmem.hpp"
#include "blockref.hpp"
#include "compst_common.hpp"
#include "itempx_c.hpp"
#include "itemconn.hpp"
#include "conitinf.hpp"
#include "item_prx.hpp"
#include "bnihil.hpp"
#include "chlog.hpp"
#include "m4mdrt.hpp"
#include "syncro.hpp"
#include "blocksynchronization.hpp"


//## end module.includes preserve=yes

///////////////////////////////////////////////////////////////////////////////
//
// ClNoCallableItem
//

ClNoCallableItem &ClNoCallableItem::operator= (const ClNoCallableItem &ai_item)
{
    _Copy (ai_item);

    return *this;
}

void ClNoCallableItem::_Copy (const ClNoCallableItem &ai_item)
{
    m_iScope = ai_item.m_iScope;
    m_pClNode_Data = ai_item.m_pClNode_Data;

    Value.m_oContext.pNode_Data = ai_item.m_pClNode_Data;
    OldValue.m_oContext.pNode_Data = ai_item.m_pClNode_Data;

    _SetRTII (ai_item.m_pClRTItemIterator); 
    
    ItemDef = ai_item.ItemDef;  //operador = sobrecargado

    Index = M4DM_EOF_INDEX; //importante
    _withitemproxy::SetEOF (); //también importante
    MoveTo (ai_item.Index);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

void ClNoCallableItem::Attach (_ClRegister_Base *ai_pregister)
{
    Value._Attach1 (ai_pregister);
    OldValue._Attach1 (ai_pregister);
}

void ClNoCallableItem::Attach (ClMMIndex ai_index)
{
    Value._Attach1 (ai_index);
    OldValue._Attach1 (ai_index);
}

void ClNoCallableItem::Attach (ClNode_Data *ai_pnode_data)
{
    M4_ASSERT (ai_pnode_data);

    ClChannel_Data *pchannel_data = ai_pnode_data->GetpChannel_Data ();

    //por si acaso:
    if (m_pClNode_Data) {
        if (m_pClNode_Data->GetpChannel_Data () != pchannel_data) {
            ItemDef.Attach (pchannel_data->GetpChannelDef ()->GetpCMCR ());
        }
    }
    else {
        ItemDef.Attach (pchannel_data->GetpChannelDef ()->GetpCMCR ());
    }

    m_pClNode_Data = ai_pnode_data;
    
    //hay que conseguir el rtii !
    SetMode (m_iScope, M4DM_TYPE_ALL);
    //MoveTo Mandatory:
    if (Index != M4DM_EOF_INDEX) {
        _AClItemProxy *p;
        m_pClRTItemIterator->Get (Index, p);
        M4_ASSERT (p);
        SetpItemProxy (p);
    }

    Value._Attach2 (pchannel_data->GetpExtInfo (), ai_pnode_data);
    OldValue._Attach2 (pchannel_data->GetpExtInfo (), ai_pnode_data);
}

void ClNoCallableItem::SetCurrentBlock (ClBlock_Channel_Proxy *ai_pblock)
{
	ClBlockReference br;
	br.Set (ai_pblock);
	SetCurrentBlock (br);
}

void ClNoCallableItem::SetCurrentBlock (const ClBlockReference &ai_br)
{
	M4_ASSERT (ai_br);

    if (ai_br->IsNihil ()) {
        //no tocamos el estado del item
    }
    else {
        ClNode_Data *pnode_data = ai_br->GetpNode_Data ();
        if (m_pClNode_Data != pnode_data) {
            Attach (pnode_data);
        }
    }

    Value._SetCurrentBlock (ai_br);
    OldValue._SetCurrentBlock (ai_br);
}

void ClNoCallableItem::Detach (void)
{
    _SetRTII (0);
    m_pClNode_Data = 0;

    SetCurrentBlock (&g_oNihilBlock);   // a saco
}

//-----------------------------------------------------------------------------
// operator ++
//-----------------------------------------------------------------------------

m4bool_t ClNoCallableItem::Next (void)
{
    if (Index != M4DM_EOF_INDEX) {
        ClItemIndex ii = Index + 1;
        MoveTo (ii);
    }
    else {
        if (GetpNode_Data ())
        {
            ClHandle NodeHandle = GetpNode_Data ()->GetpNodeDef ()->GetHandle();
            ClCompiledMCR * pmcr   = GetpNode_Data ()->GetpChannel_Data ()->GetpChannelDef ()->GetpCMCR ();
            DUMP_CHLOG_WARNINGF(M4_CH_DM_ITEMOUTOFRANGE, M4ObjNodeN(pmcr, NodeHandle));
        }
        else {
            DUMP_CHLOG_WARNING(M4_CH_DM_ITEMISEOF_CNB);
        }
    }

    return (Index != M4DM_EOF_INDEX);
}

m4bool_t ClNoCallableItem::Prev (void)
{
    if (Index != M4DM_EOF_INDEX) {
        ClItemIndex ii = Index - 1;
        MoveTo (ii);
    }
    else {
        if (GetpNode_Data ()) {
            ClHandle NodeHandle = GetpNode_Data ()->GetpNodeDef ()->GetHandle();
            ClCompiledMCR * pmcr   = GetpNode_Data ()->GetpChannel_Data ()->GetpChannelDef ()->GetpCMCR ();
            DUMP_CHLOG_WARNINGF(M4_CH_DM_ITEMOUTOFRANGE, M4ObjNodeN(pmcr, NodeHandle));
        }
        else {
            DUMP_CHLOG_WARNING(M4_CH_DM_ITEMISEOF_CNB);
        }
    }

    return (Index != M4DM_EOF_INDEX);
}

ClNoCallableItem &ClNoCallableItem::operator++ (void)
{
    Next ();

    return *this;
}

//-----------------------------------------------------------------------------
// operator --
//-----------------------------------------------------------------------------

ClNoCallableItem &ClNoCallableItem::operator-- (void)
{
    Prev ();

    return *this;
}

//-----------------------------------------------------------------------------
// Begin
//-----------------------------------------------------------------------------

ClNoCallableItem &ClNoCallableItem::Begin (void)
{
    ClItemIndex i (0);
    MoveTo (i);

    return *this;
}

//-----------------------------------------------------------------------------
// End
//-----------------------------------------------------------------------------

ClNoCallableItem &ClNoCallableItem::End (void)
{
    M4_ASSERT (m_pClRTItemIterator);

    ClItemIndex i = m_pClRTItemIterator->Count ();

    MoveTo (i);

    return *this;
}

//-----------------------------------------------------------------------------
// devuelve isnoteof
//-----------------------------------------------------------------------------

m4bool_t ClNoCallableItem::MoveTo (const ClItemIndex ai_itemindex)
{
	M4_ASSERT (m_pClRTItemIterator);

	m4bool_t bisnoteof;

	if (ai_itemindex != Index) {
		if (ai_itemindex != M4DM_EOF_INDEX) {
			_AClItemProxy *p;

			m_pClRTItemIterator->Get(ai_itemindex, p);

			if (p) {
				Index = ai_itemindex;
				SetpItemProxy (p);
				bisnoteof = M4_TRUE;
			} else {
				// eof
				MoveToEOF ();
				bisnoteof = M4_FALSE;
			}
		} else {
			// eof
			MoveToEOF ();
			bisnoteof = M4_FALSE;
		}
	} else {    
#if _DEBUG    
		//esquizofrenia:
		if (Index != M4DM_EOF_INDEX) {
			_AClItemProxy *p;

			m_pClRTItemIterator->Get(ai_itemindex, p);
			M4_ASSERT (p);
			M4_ASSERT (GetpItemProxy () == p);
		} else {
			// eof
			M4_ASSERT (GetpItemProxy () == &s_oClItemProxy_Nihil);
		}
#endif

		bisnoteof = Index != M4DM_EOF_INDEX;
	}

	return bisnoteof;
}

//-----------------------------------------------------------------------------
// devuelve isnoteof
//-----------------------------------------------------------------------------

m4bool_t ClNoCallableItem::MoveTo (const ClHandle ai_itemhandle)
{
	M4_ASSERT (m_pClRTItemIterator);

	m4bool_t bisnoteof;
	ClItemIndex itemindex;

	if (GetpItemProxy ()->GetHandle () != ai_itemhandle) {
		_AClItemProxy *p;

		m_pClRTItemIterator->Get (ai_itemhandle, p, itemindex);

		if (p) {
			Index = itemindex;
			SetpItemProxy (p);
			bisnoteof = M4_TRUE;
		} else {
			// eof
			MoveToEOF ();
			bisnoteof = M4_FALSE;
			//ojo si el canal no está construido:
			if (GetpNode_Data ()->GetpChannel_Data ()) {
				ClHandle NodeHandle = GetpNode_Data ()->GetpNodeDef()->GetHandle();
				ClCompiledMCR * pmcr = GetpNode_Data ()->GetpChannel_Data ()->GetpChannelDef ()->GetpCMCR ();
				DUMP_CHLOG_WARNINGF (M4_CH_DM_MOVETOEOF, (m4uint32_t)ai_itemhandle <<
					M4ObjNodeN(pmcr, NodeHandle));
			} else {
				DUMP_CHLOG_WARNINGF (M4_CH_DM_MOVETOEOF_CNB, (m4uint32_t)ai_itemhandle);
			}
		}
	} else {    
#if _DEBUG    
		//esquizofrenia:
		if (Index != M4DM_EOF_INDEX) {
			_AClItemProxy *p;

			m_pClRTItemIterator->Get(ai_itemhandle, p, itemindex);
			M4_ASSERT (p);
			M4_ASSERT (GetpItemProxy () == p);
			M4_ASSERT (Index == itemindex);
		} else {
			// eof
			M4_ASSERT (GetpItemProxy () == &s_oClItemProxy_Nihil);
		}
#endif

		bisnoteof = Index != M4DM_EOF_INDEX;
	}
	return bisnoteof;
}

//-----------------------------------------------------------------------------
// devuelve isnoteof
//-----------------------------------------------------------------------------

ClItemIndex ClNoCallableItem::TryMove (m4pcchar_t ai_itemid, m4bool_t ai_bIsUpper)
{
	ClItemIndex itemindex = M4DM_EOF_INDEX;
	_AClItemProxy *p = 0;

	if (ai_itemid) {
		m_pClRTItemIterator->Get (ai_itemid, p, itemindex, ai_bIsUpper);
		if (p == 0) {
			itemindex = M4DM_EOF_INDEX;
		}
	}

	return itemindex;
}

ClItemIndex ClNoCallableItem::TryMove (ClHandle ai_itemhandle)
{
	if (GetpItemProxy ()->GetHandle () != ai_itemhandle) {
		ClItemIndex itemindex;
		
		_AClItemProxy *p;
		m_pClRTItemIterator->Get (ai_itemhandle, p, itemindex);
		if (p == 0) {
			itemindex = M4DM_EOF_INDEX;
		}
		
		return itemindex;
	}
	return Index;
}

m4bool_t ClNoCallableItem::MoveTo (m4pcchar_t ai_itemid)
{
    M4_ASSERT (m_pClRTItemIterator);

    m4bool_t bisnoteof;
    ClItemIndex itemindex;

    _AClItemProxy *p = 0;

    if (ai_itemid)
        m_pClRTItemIterator->Get (ai_itemid, p, itemindex);

    if (p) {
        Index = itemindex;
        SetpItemProxy (p);
        bisnoteof = M4_TRUE;
    }
    else {
        // eof
        MoveToEOF ();
        bisnoteof = M4_FALSE;        
        if (GetpNode_Data ()->GetpChannel_Data ())
        {
            m4uint32_t NodeHandle  = GetpNode_Data ()->GetpNodeDef()->GetHandle();
            ClCompiledMCR * pmcr   = GetpNode_Data ()->GetpNodeDef()->GetpCMCR ();
            DUMP_CHLOG_WARNINGF(M4_CH_DM_MOVETOEOF_S, ai_itemid << M4ObjNodeN(pmcr, NodeHandle));
        }
        else {
            DUMP_CHLOG_WARNINGF(M4_CH_DM_MOVETOEOF_S_CNB, ai_itemid);
        }
    }

    return bisnoteof;
}

m4return_t ClNoCallableItem::MoveToEOF (void)
{
    _withitemproxy::SetEOF ();
    Index = M4DM_EOF_INDEX;
    ItemDef.SetEOF ();

    // a Value y a todos los atachados !!!
        Value.MoveToEOF ();
        OldValue.MoveToEOF ();

    return M4_SUCCESS;
}

m4uint32_t ClNoCallableItem::Count (void)
{
    M4_ASSERT (m_pClRTItemIterator);

    return m_pClRTItemIterator->Count ();
}

void ClNoCallableItem::SetpItemProxy (_AClItemProxy *ai_p)
{
    M4_ASSERT (ai_p);

    _withitemproxy::SetpItemProxy (ai_p);

    ItemDef.SetHandle (ai_p->GetHandle ());

    // a Value y a todos los atachados !!!
        Value._SetItemProxy (ai_p);
        OldValue._SetItemProxy (ai_p);
}

void ClNoCallableItem::_NotifyOperation (m4uint8_t ai_operation)
{
    // le avisamos directamente al nodo, pq el registro y el bloque ya se controla desde el 'DS'
    m_pClNode_Data->_NotifyOperation (ai_operation);
}

//-----------------------------------------------------------------------------
// Get
//-----------------------------------------------------------------------------


ClNodeDef *ClNoCallableItem::GetpNodeDef (void) const
{
    M4_ASSERT (m_pClNode_Data);

    return m_pClNode_Data->GetpNodeDef ();
}

ClNode_Kernel *ClNoCallableItem::GetpNode_ExtInfo (void) const
{
    M4_ASSERT (m_pClNode_Data);

    return m_pClNode_Data->GetpExtInfo ();
}

m4return_t ClNoCallableItem::SetMode (m4uint8_t ai_newscope, m4uint8_t ai_newtype)
{
    M4_ASSERT (m_pClNode_Data);

    ClRTItemIterator *pnewrtii = m_pClNode_Data->GetpExtInfo ()->ItemProxy_Array.GetpRTII (m_pClNode_Data->GetpExtInfo(), ai_newscope, ai_newtype);
    M4_ASSERT (pnewrtii);

    _SetRTII (pnewrtii);
    M4_ASSERT (m_pClRTItemIterator);

    m_iScope = ai_newscope;

	MoveToEOF();

    return M4_SUCCESS;
}

void ClNoCallableItem::_SetRTII (ClRTItemIterator *ai_pnewrtii)
{
	ClNode_Kernel	*pNodeKernel = NULL;
	ClMutex			*poMutex = NULL;

	/* Bug 0245699
	Se bloquea el cambio de referencias en multithread
	*/
	if( m_pClNode_Data != NULL )
	{
		pNodeKernel = m_pClNode_Data->GetpExtInfo();

		if( pNodeKernel != NULL )
		{
			poMutex = pNodeKernel->ItemProxyMutex;

			if( poMutex != NULL )
			{
				poMutex->Lock();
			}
		}
	}

    if (m_pClRTItemIterator) {
        m_pClRTItemIterator->DecReference ();
    }

    m_pClRTItemIterator = ai_pnewrtii;

    if (m_pClRTItemIterator) {
        m_pClRTItemIterator->IncReference ();
    }

	if( poMutex != NULL )
	{
		poMutex->Unlock();
	}
}

ClNoCallableItem ClNoCallableItem::Parent (void)
{
    ClNoCallableItem item (M4DM_SCOPE_ALL);

    InitAsMyParent (item);

    return item;
}

m4return_t ClNoCallableItem::InitAsMyParent (ClNoCallableItem &ai_item)
{
    M4_ASSERT (Index != M4DM_EOF_INDEX);
    //M4_ASSERT (!GetCurrentBlock ()->IsNihil ());
        //puede ser nihil por error, no debemos dar assert
    
    ClConnectItemInfo *pcii = GetpConnectItemInfo ();
    M4_ASSERT (pcii);

    ClNode_Data *pfathernode_data = pcii->Parent.GetAddress ();
    
	/* Bug 0114798
	Si no hay padre puede que estemos en server. hay que dar un error controlado
	*/
	CHECK_CHLOG_ERRORF( pfathernode_data == NULL, M4_ERROR, M4_CH_DM_BAD_INIT_AS_MY_PARENT, M4ObjNodeItemI( ItemDef.GetpCMCR(), ItemDef.GetHandle() ) ) ;


    //tenemos que irnos al nodo padre y pedirle el cmcr, pq con el caso
    //del carril, el nodo padre puede ser de otro canal !
    //ai_item.ItemDef.Attach (ai_item.m_pClNode_Channel->GetpChannel_Kernel ()->GetpCMCR ());

    //Se puede dar el caso que el item este conectado con otro item, pero 
    //la conexion de los nodos sea free
    //(preguntar a Tom y sus canales fantásticos).
    //En este caso sólo puedo coger items de nodo de mi padre

    //El caso le detecto cuando el padre que dice el pcii es distinto
    //del que me dice el nodo

    if (pfathernode_data == m_pClNode_Data)
    {
        //estamos en una relacion AUTO
		// ai_item = *this;
		// Atachamos el iterador de item a nuestro bloque, de manera que tengamos todos los scope y tipos posibles.
		// El indice que hay en pcii->Parent está cogido del metacanal y hace referencia a todos los items.
		// Antes el iterador quedaba solo con scope de bloque y todos los tipos menos metodo, con lo que los indices no coincidían.
		ai_item.SetCurrentBlock(GetCurrentBlock());
		ai_item.MoveToEOF();
    }
    else
    {
        if (m_pClNode_Data->GetpExtInfo ()->Parent.GetpNotFree ())
        {
            if (m_pClNode_Data->GetpExtInfo ()->Parent.GetpNotFree ()->GetAddress ()
                != pfathernode_data)
            {
                //aquí:
                ai_item.Attach (pfathernode_data);
                //current block del item: el nihil
            }
            else {
                //ok, no ha lugar

                //Vamos a ser menos restrictivos, en general si el bloque actual es nihil,
                //pues no pasa nada, seguimos adelante y ya está (no peta)

                if (GetCurrentBlock ()->IsNihil ()) {
                    ai_item.Attach (pfathernode_data);
                        //es lo más que podemos hacer
                }
                else {        
                    m4uint32_t parent_bi = GetCurrentBlock ()->ID.Parent_BlockId;
                    m4uint32_t parent_ri = GetCurrentBlock ()->ID.Parent_RegisterIndex;
                    m4uint32_t ri = GetCurrentBlock ()->ID.RegisterId;

                    ai_item.Attach (pfathernode_data, ri);

                    ClBlockReference brparent;
                    pfathernode_data->GetpBlockStore ()->Get (parent_bi, parent_ri, brparent);
			        //que no lo cree, pero tiene que existir:
			        M4_ASSERT (!brparent.IsNull ());

                    ai_item.SetCurrentBlock (brparent);
			        //brparent.Reset ();    //consume un tiempo y no es necesario
                }
            }
        }
        else
        {
            // Es free.
            ai_item.Attach (pfathernode_data);
        }
    }

    ai_item.MoveTo (pcii->Parent.GetItemIndex ());

    if (ai_item.Index == M4DM_EOF_INDEX) return M4_ERROR;

    return M4_SUCCESS;
}

ClBlock_Channel_Proxy* ClNoCallableItem::GetCurrentBlock (void) const
{
    M4_ASSERT (Value.GetCurrentBlock ()); // no debería devolver 0 nunca !

    return Value.GetCurrentBlock ();
}

const ClBlockReference &ClNoCallableItem::GetCurrentBlockRef (void) const
{
	return Value.GetCurrentBlockRef ();
}

m4return_t ClNoCallableItem::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClNoCallableItem);
	ao_size-=sizeof(_withitemproxy);
	ao_size-=sizeof(Value);
	ao_size-=sizeof(OldValue);
	ao_size-=sizeof(ItemDef);

	m4return_t iRet = _withitemproxy::GetSize(ao_size);
	if (iRet != M4_SUCCESS) return iRet;

	iRet = Value.GetSize(ao_size);
	if (iRet != M4_SUCCESS) return iRet;

	iRet = OldValue.GetSize(ao_size);
	if (iRet != M4_SUCCESS) return iRet;

	return ItemDef.GetSize(ao_size);
	
	// ¿Tengo que contar esto?. Creo que no, que esta contado en ItemProxyArray de Node_knl
	// OJO . Creo que NO.
	/*	if (m_pClRTItemIterator){
	if (m_pClRTItemIterator->GetSize(ao_size)!=M4_SUCCESS){
	return M4_ERROR;
	}
	}
	*/
}

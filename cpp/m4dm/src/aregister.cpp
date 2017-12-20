//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             aregister.cpp
// Project:          CVM-DM
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


#include "aregister.hpp"
#include "dmres.hpp"
#include "dm_def.hpp"
#include "m4define.hpp"
#include "dmtypes.hpp"
#include "aregwrite.hpp"
#include "node_knl.hpp"
#include "recorset.hpp"
#include "px_block.hpp"
#include "bnihil.hpp"
#include "checkmem.hpp"
#include "regpipe.hpp"
#include "chan_knl.hpp"
#include "compst_common.hpp"
#include "pl_data.hpp"
#include "regi_smp.hpp"
#include "reg_filt.hpp"
#include "reg_plf.hpp"
#include "access.hpp"
#include "not_cdat.hpp"
#include "file_misc.hpp"
#include "laneinfo.hpp"
#include "clregexp.hpp"
#include "bookmark.hpp"
#include "m4mdrt.hpp"
#include "m4objglb.hpp"
#include "rm.hpp"




///////////////////////////////////////////////////////////////////////////////
//
// ClLastUpdated
//

ClLastUpdated::ClLastUpdated () 
{
    m_pClRegister = 0;
}

void ClLastUpdated::Attach (_ClRegister_Base *ai_pr)
{
    M4_ASSERT (ai_pr);

    m_pClRegister = ai_pr;
}

m4return_t ClLastUpdated::Get (VersionMark &ao_vm)
{
    M4_ASSERT (m_pClRegister);

    return m_pClRegister->GetLastUpdated (ao_vm);
}

m4return_t ClLastUpdated::Set (VersionMark ai_vm)
{
    M4_ASSERT (m_pClRegister);

    return m_pClRegister->SetLastUpdated (ai_vm);
}

///////////////////////////////////////////////////////////////////////////////
//
// ClPersist
//

ClPersist_Register::ClPersist_Register () 
{
    m_pClRegister = 0;
}

void ClPersist_Register::Attach (_ClRegister_Base *ai_pr)
{
    M4_ASSERT (ai_pr);

    m_pClRegister = ai_pr;
}

m4return_t ClPersist_Register::Get (m4bool_t &ao_persist)
{
    M4_ASSERT (m_pClRegister);

    return m_pClRegister->GetFlagPersisted (ao_persist);
}

m4return_t ClPersist_Register::Set (m4bool_t ai_persist)
{
    M4_ASSERT (m_pClRegister);

    return m_pClRegister->SetFlagPersisted (ai_persist);
}

///////////////////////////////////////////////////////////////////////////////
//
// _ClRegister_Base
//

_ClRegister_Base::~_ClRegister_Base ()
{
    Detach ();

    #ifdef _DEBUG
        g_StMemInfo.iRegister--;
    #endif    
}



void _ClRegister_Base::_Copy (const _ClRegister_Base &ai_reg, m4bool_t ai_bTotal)
{
    //debería estar limpio, pq sino, con el nofifier hay lio:
    if (IsAttached ()) {
        Detach ();

		//Daniel. No se si es lo mas correcto.
		//Aregla el sig. problema:
		//   Despues del detach el access del item queda a NULL, esto sucede p ej si
		//   asignamos dos veces un registro a otro (la segunda vez el access queda a NULL).
		Item.Attach(ai_reg.GetpAccess ());
    }    

    M4_ASSERT (GetpNode_Data () == 0);
    
    //Nodo:
    Attach (ai_reg.GetpNode_Data ());
    
    //aqui se copia también el tema de autoload, etc.:
    Filter.CopyFilter(ai_reg.Filter, ai_bTotal);

    _SetpAccess (ai_reg.GetpAccess ());

    //primero el bloque, que pone los index a EOF
    Item.SetCurrentBlock (ai_reg.GetCurrentBlock ());

    Index = ai_reg.Index;
    MMIndex = ai_reg.MMIndex;

    //y también el item:
    Item.MoveTo (ai_reg.Item.GetIndex());
}

//-----------------------------------------------------------------------------
// Init
//-----------------------------------------------------------------------------

void _ClRegister_Base::Detach (void)
{
    if (IsAttached ()) {
        _SetpNode_Data (0);

        Filter.DeleteAll ();
    }

    Item.Detach ();
}

m4bool_t _ClRegister_Base::IsAttached (void)
{
    return (GetpNode_Data () != 0 ? M4_TRUE : M4_FALSE);
}

void _ClRegister_Base::Attach (ClNode_Data *ai_pnc)
{
    M4_ASSERT (ai_pnc);

    _SetpNode_Data (ai_pnc);

    //Attach del item
    Item.Attach (GetpNode_Data ());

    m4bool_t ispartialload = GetpNodeDef ()->NumRows() != 0 ? M4_TRUE : M4_FALSE;

    m4uint8_t iautoloadmode;

    if (GetpAccess ()) {  //de momento permitimos que pueda ser cero        

        iautoloadmode = _GetAutoLoadMode ();
    }
    else {
        iautoloadmode = M4CL_AUTOLOAD_OFF;
    }

    Filter.Init (ispartialload, iautoloadmode);
}

m4uint8_t _ClRegister_Base::_GetAutoLoadMode (void)
{
    M4_ASSERT (GetpNode_Data ());
    M4_ASSERT (GetpAccess ());

    ClNode *pnode;
    pnode = GetpAccess ()->Node.Get (GetpNode_ExtInfo ()->Index);
    M4_ASSERT (pnode);

    return pnode->GetAutoLoadRealMode ();
}

//----------------------------------------------------------------------------
// Exist
//      Empieza a buscar a partir de la posición del registro
//----------------------------------------------------------------------------

m4bool_t _ClRegister_Base::Exist (m4VariantType *ai_pstArguments)
{
    return M4_FALSE;
}

ClTotalize <_ClValue_Read_Slices, m4VariantType, ClGetValue> * _ClRegister_Base::GetValueAddr(void)
{
	return &Item.Value.GetpSliceFooter ();
}

m4uint8_t _ClRegister_Base::GetTotType ( void ) const
{
	return Item.ItemDef.IdTotalize();
}

m4return_t _ClRegister_Base::SetLotOfItems (ClRegister_Pipe * ai_pRegPipe, m4bool_t ai_bAddReg)
{
	m4uint16_t  i, ai_Pos;
	m4uint32_t  NumItem = ai_pRegPipe->GetNumItem();
	ClHandle *  ai_ItemHandle;
	m4return_t  res;
    ClMMIndex   newindex = 0;
	m4bool_t    IsReady;

	res = ai_pRegPipe->GetIsReady(IsReady);

	if (!IsReady)
	{
		for (i=0; i<NumItem; i++)
		{
			ai_ItemHandle = ai_pRegPipe->GetItemHandle(i);
			ai_Pos = Item[(*ai_ItemHandle)].GetPos();
			ai_pRegPipe->SetItemPos(i, ai_Pos);
		}
		IsReady = M4_TRUE;
		res = ai_pRegPipe->SetIsReady(IsReady);
	}

    if (ai_bAddReg)
    {
        res = GetCurrentBlock()->R_AddRecordAndSetLDBItems(newindex, ai_pRegPipe, M4_TRUE, M4_FALSE);
        if (res == M4_SUCCESS) {        
		    MMIndex = (m4uint32_t)newindex;

            if ( ! Filter->IsTerminator ())
            {
		        ClBlockReference br;
		        br.Set (GetCurrentBlock());
                Filter->Notify_InsertRegister (newindex, br, this);
            }

            Index = (m4uint32_t)newindex;
        }
    }
    else
    {
        m4uint32_t readindex = GetCurrentBlock()->GetpPL_Data()->GetCurrentRegister();
        MMIndex = readindex;

        res = GetCurrentBlock()->R_AddRecordAndSetLDBItems(MMIndex, ai_pRegPipe, M4_FALSE, M4_FALSE);

        GetCurrentBlock()->GetpPL_Data()->SetCurrentRegister(++readindex);
    }

    return res;
}

m4return_t _ClRegister_Base::SetLotOfItemsPartial (ClRegister_Pipe * ai_pRegPipe, m4bool_t ai_bAuxLoad)
{
	m4uint16_t  i, ai_Pos;
	m4uint32_t  NumItem = ai_pRegPipe->GetNumItem();
	ClHandle *  ai_ItemHandle;
	m4return_t  res;

	m4bool_t IsReady;
	res = ai_pRegPipe->GetIsReady(IsReady);

    m4uint32_t readindex = GetCurrentBlock()->GetpPL_Data()->GetCurrentRegister();

    MMIndex = readindex;
// Este booleano me dice si ya he completado la primera carga cuando hay selects divididas
// True: Ya se han cargados las primeras columnas False: lo contrario.
    m4bool_t bMoreToAdd = GetCurrentBlock()->GetpPL_Data()->GetEndOfRegisters();

	if ((MMIndex<GetpNodeDef ()->NumRows())||(GetpNodeDef ()->NumKeepRows() < 0)) //no hay mas registros!!! o hay que añadir nuevos
	{
	    if (!IsReady)
	    {
		    for (i=0; i<NumItem; i++)
		    {
			    ai_ItemHandle = ai_pRegPipe->GetItemHandle(i);
			    ai_Pos = Item[(*ai_ItemHandle)].GetPos();
			    ai_pRegPipe->SetItemPos(i, ai_Pos);
		    }
		    IsReady = M4_TRUE;
		    res = ai_pRegPipe->SetIsReady(IsReady);
	    }
        if (GetpNodeDef ()->NumKeepRows() >= 0)
        {
            // Es la primera vez que hago una carga, tengo que crear los registros.
			/* Bug 0070883
			Hay que poner hacer la comprobación por el flag y no por los visibles.
			*/
            if (GetCurrentBlock()->GetpPL_Data()->GetFirstWindowLoaded() == M4_FALSE)
            {
                // ni MMIndex ni registerindex se deben modificar, por que se usan luego
                ClRegisterIndex auxIndex = Index;
                m4uint32_t auxindex = readindex;

                if (!bMoreToAdd)
                {
                    res = GetCurrentBlock()->B_AddRegister(auxindex);
                    // Se le notifoca a este register que se ha insertado un registro.
                    // Lo vuelvo a dejar como estaba. Tanto Index como MMIndex.
                    MMIndex = readindex;
                    Index = auxIndex;
                    // Ademas dejo el registro marcado como NORMAL!!!!!
                    auxindex = readindex;
                    res = GetCurrentBlock()->CheckPoint (auxindex);
                }
            }
            else
            {
                GetCurrentBlock()->R_DestroyRBChilds(MMIndex);
            }
            m4bool_t bAuxLoad = bMoreToAdd ? M4_FALSE : M4_TRUE;
            res = GetCurrentBlock()->R_AddRecordAndSetLDBItems(MMIndex, ai_pRegPipe, M4_FALSE, bAuxLoad);
        }
        else
        {
            if (bMoreToAdd)
                res = GetCurrentBlock()->R_AddRecordAndSetLDBItems(MMIndex, ai_pRegPipe,
                M4_TRUE, M4_FALSE);
            else
                res = GetCurrentBlock()->R_AddRecordAndSetLDBItems(MMIndex, ai_pRegPipe,
                M4_TRUE, M4_TRUE);
        }
	}

    GetCurrentBlock()->GetpPL_Data()->SetCurrentRegister(++readindex);

    if (!bMoreToAdd)
    {
        m4uint32_t ilv = GetCurrentBlock()->GetpPL_Data()->GetNextLV();
        GetCurrentBlock()->GetpPL_Data()->SetNextLV(++ilv);
    }

    return res;
}

void _ClRegister_Base::SetFirstToWrite (m4uint32_t ai_index)
{
    GetCurrentBlock()->GetpPL_Data()->SetCurrentRegister((m4int32_t) ai_index);
}

m4uint32_t _ClRegister_Base::GetFirstToWrite (void)
{
    return GetCurrentBlock()->GetpPL_Data()->GetCurrentRegister();
}

void _ClRegister_Base::SetEndOfRegisters (m4bool_t ai_bEOR)
{
    GetCurrentBlock()->GetpPL_Data()->SetEndOfRegisters(ai_bEOR);
}

//-----------------------------------------------------------------------------
// GetChildBlock, llamada para la propagación y cosas así
// => utilizo el ChildNode del Channel
//-----------------------------------------------------------------------------

template <class T>
inline m4return_t s_getchildblock (ClNode *pn, const ClBlockReference &ai_brcurrent, ClBlockReference &ai_brchild, ClMMIndex mmindex, T ai_id)
{
	m4return_t res;

    if (pn)
    {
        if (pn->GetpLane_Info())
        {
            ClNode *pnchild = pn->GetpLane_Info()->ChildNodeRB [ai_id].GetAddress ();
            if (0 == pnchild)
            {
                m4uint32_t NodeHandle = pn->GetpExtInfo()->GetHandle();
                ClCompiledMCR * pmcr  = pn->GetpExtInfo()->GetpCMCR ();
                DUMP_CHLOG_ERRORF(M4_CH_DM_CHILDNODEDONTEXIST, M4ObjNodeN(pmcr, NodeHandle));

                ai_brchild.Set (&g_oNihilBlock);
		        res = M4_ERROR;

            }
            else {
		        if (ClMMIndex_EOF != mmindex)
                {
                    m4uint32_t iMNB = pnchild->GetpNode_Data()->GetpNodeDef()->MaxNumBlocks();
                    if ((iMNB == 0)||(iMNB > pnchild->GetpNode_Data()->GetpBlockStore ()->Count()))
                    {
			            res = pnchild->GetpNode_Data()->GetpBlockStore ()->
                            Get (ai_brcurrent->ID.BlockUniqueKey, mmindex, ai_brchild,
                            ai_brcurrent->ID.BlockId, ai_brcurrent->ID.RegisterId);
                    }
                    else
                    {
                        ai_brchild.Set (&g_oNihilBlock_Memory);
    			        res = M4_WARNING;
                    }
		        }
                else
                {
                    ai_brchild.Set (&g_oNihilBlock);
			        res = M4_WARNING;
		        }
            }
        
            return res;
        }
    }
    return M4_ERROR;
}


template <class T>
inline m4return_t s_getchildblock (ClNode_Data *pn, const ClBlockReference &ai_brcurrent, ClBlockReference &ai_brchild, ClMMIndex mmindex, T ai_id)
{
	m4return_t res;

    ClNode_Data *pnchild = pn->GetpExtInfo()->ChildNodeRB [ai_id].GetAddress ();

    if (0 == pnchild)
    {
        m4uint32_t NodeHandle = pn->GetpExtInfo()->GetHandle();
        ClCompiledMCR * pmcr  = pn->GetpExtInfo()->GetpCMCR ();
        DUMP_CHLOG_ERRORF(M4_CH_DM_CHILDNODEDONTEXIST, M4ObjNodeN(pmcr, NodeHandle));

        ai_brchild.Set (&g_oNihilBlock);
		res = M4_ERROR;

    }
    else {
		if (ClMMIndex_EOF != mmindex)
        {
            m4uint32_t iMNB = pnchild->GetpNodeDef()->MaxNumBlocks();
            if ((iMNB == 0) || (iMNB > pnchild->GetpBlockStore ()->Count()))
            {
			    res = pnchild->GetpBlockStore ()->
                    Get (ai_brcurrent->ID.BlockUniqueKey, mmindex, ai_brchild,
                    ai_brcurrent->ID.BlockId, ai_brcurrent->ID.RegisterId);
            }
            else
            {
                ai_brchild.Set (&g_oNihilBlock_Memory);
    			res = M4_WARNING;
            }
		}
        else {
            ai_brchild.Set (&g_oNihilBlock);
			res = M4_WARNING;
		}
    }
        
    return res;
}


//a extinguir:

ClBlock_Channel_Proxy *_ClRegister_Base::GetChildBlock (ClHandle ai_handle)
{
	ClBlockReference brchild;

    m4return_t res = s_getchildblock (GetpNode_Data(), GetCurrentBlockRef (), brchild, GetMMIndex (), ai_handle);
    if (M4_ERROR == res)
    {
        m4uint32_t NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
        ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();

        DUMP_CHLOG_ERRORF(M4_CH_DM_CHILDBLOCK_ERROR_ID, NodeN(pmcr, ai_handle)
            << M4ObjNodeN(pmcr, NodeHandle));
    }

	ClBlock_Channel_Proxy *pb = brchild.GetpProxy ();

	//al salir, en el destructor de br se destruye el proxy si nadie más le estaba usando
	// --> mucho ojo ahora con esta función

    //para evitar esto, mientras dure la función, hago un truco para que no se elimine
    brchild.Invalide ();
    pb->DecRef ();

    return pb;
}

ClBlock_Channel_Proxy *_ClRegister_Base::GetChildBlock (m4pchar_t ai_childid)
{
	ClBlockReference brchild;
    m4return_t res = s_getchildblock (GetpNode_Data(), GetCurrentBlockRef (), brchild, GetMMIndex (), ai_childid);
    if (M4_ERROR == res)
    {
        m4uint32_t NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
        ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
        m4uint32_t ChildNodeHandle = pmcr->GetChannelNodeHandleById (ai_childid);

        DUMP_CHLOG_ERRORF(M4_CH_DM_CHILDBLOCK_ERROR_ID, NodeN(pmcr, ChildNodeHandle)
            << M4ObjNodeN(pmcr, NodeHandle));
    }

	ClBlock_Channel_Proxy *pb = brchild.GetpProxy ();

	//al salir, en el destructor de br se destruye el proxy si nadie más le estaba usando
	// --> mucho ojo ahora con esta función

    //para evitar esto, mientras dure la función, hago un truco para que no se elimine
    brchild.Invalide ();
    pb->DecRef ();

    return pb;
}

ClBlock_Channel_Proxy *_ClRegister_Base::GetChildBlock (m4uint16_t ai_pos)
{
	ClBlockReference brchild;
    m4return_t res = s_getchildblock (GetpNode_Data(), GetCurrentBlockRef (), brchild, GetMMIndex (), (ClChildIndex)ai_pos);
    if (M4_ERROR == res)
    {
        m4uint32_t NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
        ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
        m4uint32_t ChildNodeHandle = pmcr->GetChannelNodeHandleByPosition(ai_pos);

        DUMP_CHLOG_ERRORF(M4_CH_DM_CHILDBLOCK_ERROR_ID, NodeN(pmcr, ChildNodeHandle) <<
            M4ObjNodeN(pmcr, NodeHandle));
    }

	ClBlock_Channel_Proxy *pb = brchild.GetpProxy ();

	//al salir, en el destructor de br se destruye el proxy si nadie más le estaba usando
	// --> mucho ojo ahora con esta función

    //para evitar esto, mientras dure la función, hago un truco para que no se elimine
    brchild.Invalide ();
    pb->DecRef ();

    return pb;
}

//nuevas:

m4return_t _ClRegister_Base::GetChildBlock (ClHandle ai_handle, ClBlockReference &ai_brchild)
{
    m4return_t res = s_getchildblock (GetpNode_Data(), GetCurrentBlockRef (), ai_brchild, GetMMIndex (), ai_handle);
    if (M4_ERROR == res)
    {
        m4uint32_t NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
        ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();

        DUMP_CHLOG_ERRORF(M4_CH_DM_CHILDBLOCK_ERROR_ID, NodeN(pmcr, ai_handle)
            << M4ObjNodeN(pmcr, NodeHandle));
    }
    return res;
}

m4return_t  _ClRegister_Base::GetChildBlock (m4pchar_t ai_childid, ClBlockReference &ai_brchild)
{
    m4return_t res = s_getchildblock (GetpNode_Data (), GetCurrentBlockRef (), ai_brchild, GetMMIndex (), ai_childid);
    if (M4_ERROR == res)
    {
        m4uint32_t NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
        ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
        m4uint32_t ChildNodeHandle = pmcr->GetChannelNodeHandleById (ai_childid);

        DUMP_CHLOG_ERRORF(M4_CH_DM_CHILDBLOCK_ERROR_ID, NodeN(pmcr, ChildNodeHandle)
            << M4ObjNodeN(pmcr, NodeHandle));
    }
    return res;
}

m4return_t _ClRegister_Base::GetChildBlock (m4uint16_t ai_pos, ClBlockReference &ai_brchild)
{
    m4return_t res;

    if (ai_pos < GetpNode_Data()->GetpExtInfo ()->ChildNodeRB.Count())
    {
        res = s_getchildblock (GetpNode_Data (), GetCurrentBlockRef (), ai_brchild, GetMMIndex (), (ClChildIndex)ai_pos);
        if (M4_ERROR == res)
        {
            m4uint32_t NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
            ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
            m4uint32_t ChildNodeHandle = pmcr->GetChannelNodeHandleByPosition(ai_pos);

            DUMP_CHLOG_ERRORF(M4_CH_DM_CHILDBLOCK_ERROR_ID, NodeN(pmcr, ChildNodeHandle) <<
                M4ObjNodeN(pmcr, NodeHandle));
        }
    }
    else
        res = s_getchildblock (GetpNode (), GetCurrentBlockRef (), ai_brchild, GetMMIndex (), (ClChildIndex)(ai_pos - GetpNode_Data()->GetpExtInfo ()->ChildNodeRB.Count()));

    return res;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

ClBlock_Channel_Proxy* _ClRegister_Base::GetCurrentBlock (void) const
{
	ClBlock_Channel_Proxy* pProxy = Item.GetCurrentBlock();
    M4_ASSERT (pProxy); // no debería devolver 0 nunca !

    return pProxy;
}


m4return_t _ClRegister_Base::CopyDataFrom (_ClRegister_Base &ai_reg)
{
    //m4VariantType aux;

    ai_reg.Item.Begin ();
    Item.Begin ();    

    while (Item.GetIndex() != M4DM_EOF_INDEX) {

        //ai_reg.Item.Value.Get (aux);
        Item.Value.CopyItem (&ai_reg.Item.Value);
        
        ai_reg.Item.Next ();
        Item.Next ();
    }

    return M4_SUCCESS;
}

_ClRegister_Base &_ClRegister_Base::operator <<(_ClRegister_Base &ai_reg)
{
    CopyDataFrom (ai_reg);

    return *this;
}

//
// Function: _ClRegister_Base::Serialize
//
// Arguments:
//    ClGenericIODriver& IOD - Where we serialize to
//
// Returns: m4return_t
//
// Description:
//   Serialize the register index, and the item iterator
//
// Author:  Stuart Murdoch (mailto:stuartm@meta4.es)
// Created: 10 December 1998
//
m4return_t _ClRegister_Base::Serialize(ClGenericIODriver& IOD)
{
	// Serialize the index of the register
	m4return_t iRet = IOD.Write(Index);
	if (iRet != M4_SUCCESS) return iRet;
	
	iRet = IOD.Write(MMIndex);
	if (iRet != M4_SUCCESS) return iRet;
	
	// Serialize the item iterator
	/*
	Mejora de ancho de banda. No se serializa la posición del item
	iRet = Item.Serialize(IOD);
	if (iRet != M4_SUCCESS) return iRet;
	*/

	return iRet;
}

//
// Function: _ClRegister_Base::DeSerialize
//
// Arguments:
//    ClGenericIODriver& IOD - Where we deserialize from
//
// Returns: m4return_t
//
// Description:
//
// Author:  Stuart Murdoch (mailto:stuartm@meta4.es)
// Created: 10 December 1998
//
m4return_t _ClRegister_Base::DeSerialize(ClGenericIODriver& IOD)
{
    // DeSerialize the index of the register
    m4uint32_t iIndex, iMMIndex;
    m4return_t RetVal = IOD.Read(iIndex);

    if (RetVal == M4_SUCCESS)
        RetVal = IOD.Read(iMMIndex);
    
    if (RetVal == M4_SUCCESS)
    {
        // Move to the appropriate index
        if (GetpNode_Data()->GetpNodeDef()->NumRows())
        {
            if (iIndex != (m4uint32_t)Index)
			{
				/* Bug 0098321
				Si donde nos vamos a mover no está en la ventana actual no nos movemos
				porque dispararía una carga y lo dejamos en EOF para que sea consistente
				Alternativas serían
					1- Moverlo al primer registro visible
					2- Cambiar a mano el índice sin hacer el moveto
				Ninguna solución parece acertada
				*/
				ClPartialLoadData	*poPartial = NULL ;
				m4int32_t			iFirst = -1 ;
				m4int32_t			iLast = -1 ;

				poPartial = GetCurrentBlockRef()->GetpPL_Data() ;

				/* Bug 0103756
				Si el bloque es nil no tendrá carga parcial asociada
				Esto puede pasar si en el padre nos hemos desposicionado en la propia
				deserialización. Y eso sólo puede pasar si el padre (abuelo, etc)
				es de carga parcial y pasa por este código y se pone en EOF.
				*/
				if( poPartial != NULL )
				{
					iFirst = poPartial->GetFirstVisible() ;
					iLast = poPartial->GetLastVisible() ;

					if( iIndex >= iFirst && iIndex < iLast )
					{
						MoveTo( ClRegisterIndex( iIndex ) ) ;
					}
					else
					{
						MoveToEOF() ;
					}
				}
				else
				{
					MoveToEOF() ;
				}
			}
        }
        else
        {
            if (Filter.Count() != 0) // local filtrado
            {
                ClRegisterIndex auxIndex;
                Filter->UpdateIndex (auxIndex, iMMIndex, GetCurrentBlockRef(), this);
                MoveTo(auxIndex);
            }
            else // local NO filtrado OK!!!
            {
                if (iMMIndex != MMIndex)
                    MoveTo((ClRegisterIndex)iMMIndex);
            }
        }

        // DeSerialize the item iterator
		/*
		Mejora de ancho de banda. No se deserializa la posición del item
        RetVal = Item.DeSerialize(IOD);
		*/
    }
    
    return RetVal;
}

/*
 * Function: _ClRegister_Base::SendRegisterMark
 *
 * Arguments:
 *      ai_bBranchMode - si es M4_TRUE se serializa todo lo que cuelga del registro
 *
 * Description:
 *      Marca para serializar el registro y la jerarquia de registros que lo soporta
 */
void _ClRegister_Base::SendRegisterMark(const m4bool_t ai_bBranchMode)
{
    GetCurrentBlock()->MarkRecordAndParentRecordsForSerialize(MMIndex, ai_bBranchMode);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

m4return_t _ClRegister_Base::GetLastUpdated (VersionMark &ao_vm)
{
	M4_ASSERT (GetCurrentBlock ());
	
	if (Index == M4DM_EOF_INDEX) {
		m4uint32_t NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
		ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
		DUMP_CHLOG_WARNINGF(M4_CH_DM_REGISTERISEOF, M4ObjNodeN(pmcr, NodeHandle));
		return M4_ERROR;
	}

	return GetCurrentBlock ()->R_GetVersion (GetMMIndex (), ao_vm);
}

m4return_t _ClRegister_Base::SetLastUpdated (VersionMark &ai_vm)
{
	M4_ASSERT (GetCurrentBlock ());
	
	if (Index == M4DM_EOF_INDEX) {
		m4uint32_t NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
		ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
		DUMP_CHLOG_WARNINGF(M4_CH_DM_REGISTERISEOF, M4ObjNodeN(pmcr, NodeHandle));
		return M4_ERROR;
	}
	
	return GetCurrentBlock ()->R_SetVersion (GetMMIndex (), ai_vm);
}

m4return_t _ClRegister_Base::GetFlagPersisted (m4bool_t &ao_persist)
{
	M4_ASSERT (GetCurrentBlock ());
	
	if (Index == M4DM_EOF_INDEX) {
		m4uint32_t NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
		ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
		DUMP_CHLOG_WARNINGF(M4_CH_DM_REGISTERISEOF, M4ObjNodeN(pmcr, NodeHandle));
		return M4_ERROR;
	}
	
	return GetCurrentBlock ()->R_GetRecordPersist (GetMMIndex (), ao_persist);
}

m4return_t _ClRegister_Base::SetFlagPersisted (m4bool_t ai_persist)
{
	M4_ASSERT (GetCurrentBlock ());
	
	if (Index == M4DM_EOF_INDEX) {
		m4uint32_t NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
		ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
		DUMP_CHLOG_WARNINGF(M4_CH_DM_REGISTERISEOF, M4ObjNodeN(pmcr, NodeHandle));
		return M4_ERROR;
	}
	
	return GetCurrentBlock ()->R_SetRecordPersist (GetMMIndex (), ai_persist);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

m4return_t _ClRegister_Base::GetStatus  (RegisterStatus_t &ao_riStatus)
{
	M4_ASSERT (GetCurrentBlock ());
	
	if (Index == M4DM_EOF_INDEX) {
		m4uint32_t NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
		ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
		DUMP_CHLOG_WARNINGF(M4_CH_DM_REGISTERISEOF, M4ObjNodeN(pmcr, NodeHandle));
		ao_riStatus = M4_RS_INVALID;
		return M4_ERROR;
	}
	
	return GetCurrentBlock ()->R_GetStatus (GetMMIndex (), ao_riStatus);
}

m4return_t _ClRegister_Base::IsModified (m4bool_t &ao_is)
{
	M4_ASSERT (GetCurrentBlock ());
	
	if (Index == M4DM_EOF_INDEX) {
		m4uint32_t NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
		ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
		DUMP_CHLOG_WARNINGF(M4_CH_DM_REGISTERISEOF, M4ObjNodeN(pmcr, NodeHandle));
		ao_is = M4_FALSE;
		return M4_ERROR;
	}
	
	ao_is = GetCurrentBlock ()->R_IsModified (GetMMIndex ());
	return M4_SUCCESS;
}

m4return_t _ClRegister_Base::IsNew (m4bool_t &ao_is)
{
	M4_ASSERT (GetCurrentBlock ());
	
	if (Index == M4DM_EOF_INDEX) {
		m4uint32_t NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
		ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
		DUMP_CHLOG_WARNINGF(M4_CH_DM_REGISTERISEOF, M4ObjNodeN(pmcr, NodeHandle));
		ao_is = M4_FALSE;
		return M4_ERROR;
	}
	
	ao_is = GetCurrentBlock ()->R_IsNew (GetMMIndex ());
	return M4_SUCCESS;
}

m4return_t _ClRegister_Base::IsUpdated (m4bool_t &ao_is)
{
	M4_ASSERT (GetCurrentBlock ());
	
	if (Index == M4DM_EOF_INDEX) {
		m4uint32_t NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
		ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
		DUMP_CHLOG_WARNINGF(M4_CH_DM_REGISTERISEOF, M4ObjNodeN(pmcr, NodeHandle));
		ao_is = M4_FALSE;
		return M4_ERROR;
	}
	
	ao_is = GetCurrentBlock ()->R_IsUpdated (GetMMIndex ());
	return M4_SUCCESS;
}

m4return_t _ClRegister_Base::IsDeleted (m4bool_t &ao_is)
{
	M4_ASSERT (GetCurrentBlock ());
	
	if (Index == M4DM_EOF_INDEX) {
		m4uint32_t NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
		ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
		DUMP_CHLOG_WARNINGF(M4_CH_DM_REGISTERISEOF, M4ObjNodeN(pmcr, NodeHandle));
		ao_is = M4_FALSE;
		return M4_ERROR;
	}
	
	ao_is = GetCurrentBlock ()->R_IsDeleted (GetMMIndex ());
	return M4_SUCCESS;
}

m4return_t _ClRegister_Base::IsValid (m4bool_t &ao_is)
{
	M4_ASSERT (GetCurrentBlock ());
	
	if (Index == M4DM_EOF_INDEX) {
		m4uint32_t NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
		ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
		DUMP_CHLOG_WARNINGF(M4_CH_DM_REGISTERISEOF, M4ObjNodeN(pmcr, NodeHandle));
		ao_is = M4_FALSE;
		return M4_ERROR;
	}
	
	ao_is = GetCurrentBlock ()->R_IsValid (GetMMIndex ());
	return M4_SUCCESS;
}

m4return_t _ClRegister_Base::IsModifiedForDDBB (m4bool_t &ao_is)
{
	M4_ASSERT (GetCurrentBlock ());
	
	if (Index == M4DM_EOF_INDEX) {
		m4uint32_t NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
		ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
		DUMP_CHLOG_WARNINGF(M4_CH_DM_REGISTERISEOF, M4ObjNodeN(pmcr, NodeHandle));
		ao_is = M4_FALSE;
		return M4_ERROR;
	}
	
	ao_is = GetCurrentBlock ()->R_IsModifiedByDDBB (GetMMIndex ());
	return M4_SUCCESS;
}

m4return_t _ClRegister_Base::SetUpdated (void)
{
	M4_ASSERT (GetCurrentBlock ());
	
	if (Index == M4DM_EOF_INDEX) {
		m4uint32_t NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
		ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
		DUMP_CHLOG_WARNINGF(M4_CH_DM_REGISTERISEOF, M4ObjNodeN(pmcr, NodeHandle));
		return M4_ERROR;
	}
	
	return GetCurrentBlock ()->R_SetUpdated (GetMMIndex ());
}

// esta es la más bestia de todas, asigna el índice y ya está
// se ha utilizado por el tema de las ordenaciones, por ahora
// se llama sólo en g_arrange_sub_refreshcurrent y g_destroy_sub_setaccesstoindex

void _ClRegister_Base::_SetIndex (ClRegisterIndex ai_newindex)
{
    Index = ai_newindex;    // fácil ¿no?

//provisional:
//OJO SOLO VALIDO CUANDO NO HAY FILTROS:
    MMIndex = Index;
}

//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// Begin
//----------------------------------------------------------------------------

m4bool_t _ClRegister_Base::Begin (void)
{
    Filter->Begin (Index, MMIndex, GetCurrentBlockRef (), this);

    if (MMIndex != ClMMIndex_EOF) {
        Index = 0;
    } else {
        Index = M4DM_EOF_INDEX;
    }
    
    return Index != M4DM_EOF_INDEX;
}

//----------------------------------------------------------------------------
// End
//----------------------------------------------------------------------------

m4bool_t _ClRegister_Base::End (void)
{
	Filter->End (Index, MMIndex, GetCurrentBlockRef (), this);
	
	if (MMIndex != ClMMIndex_EOF)
	{
		m4uint32_t n = Count();
		Index = n - 1;
	} else {
		Index = M4DM_EOF_INDEX;
	}
	
	return Index != M4DM_EOF_INDEX;
}

//----------------------------------------------------------------------------
// Next
//----------------------------------------------------------------------------

m4bool_t _ClRegister_Base::Next (void)
{
    if (Index == M4DM_EOF_INDEX) {
        m4uint32_t NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
        ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
        DUMP_CHLOG_WARNINGF(M4_CH_DM_REGISTERISEOF, M4ObjNodeN(pmcr, NodeHandle));
    } else  {
        Filter->Next (Index, MMIndex, GetCurrentBlockRef (), this);

        if (MMIndex != ClMMIndex_EOF) {
            Index++;
	} else {
            Index = M4DM_EOF_INDEX;
        }
    }

    return Index != M4DM_EOF_INDEX;
}

//----------------------------------------------------------------------------
// Prev
//----------------------------------------------------------------------------

m4bool_t _ClRegister_Base::Prev (void)
{
	if (Index == M4DM_EOF_INDEX) {
		m4uint32_t NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
		ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
		DUMP_CHLOG_WARNINGF(M4_CH_DM_REGISTERISEOF, M4ObjNodeN(pmcr, NodeHandle));
	} else {
		Filter->Prev (Index, MMIndex, GetCurrentBlockRef (), this);
		
		if (MMIndex != ClMMIndex_EOF) {
			Index--;
		} else {
			Index = M4DM_EOF_INDEX;
		}
	}
	
	return Index != M4DM_EOF_INDEX;
}

//----------------------------------------------------------------------------
// MoveTo
//----------------------------------------------------------------------------

void _ClRegister_Base::MoveToEOF (void)
{
    Index = M4DM_EOF_INDEX;
    MMIndex = ClMMIndex_EOF;
}

m4bool_t _ClRegister_Base::MoveToMM (ClMMIndex ai_MMIndex)
{
    MMIndex = ai_MMIndex;
	return M4_BOOL(MMIndex != ClMMIndex_EOF);
}
m4bool_t _ClRegister_Base::MoveTo (ClRegisterIndex ai_newindex)
{
	if (ai_newindex == M4DM_EOF_INDEX) {
		MoveToEOF ();
	} else {
		Filter->MoveTo (ai_newindex, Index, MMIndex, GetCurrentBlockRef (), this);
		
		if (MMIndex != ClMMIndex_EOF) {
			Index = ai_newindex;
		} else {
			Index = M4DM_EOF_INDEX;
		}
	}
	
	return Index != M4DM_EOF_INDEX;
}

m4bool_t _ClRegister_Base::MoveToMandatory (ClRegisterIndex ai_newindex)
{
    return MoveTo (ai_newindex);
}

m4bool_t _ClRegister_Base::BlockIsLoaded(void)
{
    return GetCurrentBlock()->IsLoaded ();
}

//----------------------------------------------------------------------------
// Count
//----------------------------------------------------------------------------

m4uint32_t _ClRegister_Base::Count (void)
{
    m4uint32_t n;
    Filter->Count (n, GetCurrentBlockRef (), this);

	return n;
}


m4return_t _ClRegister_Base :: FindSec (m4VariantType *ai_pstArguments, m4uint16_t ai_iArguments, m4int32_t  * ao_iRegister)
{
	m4return_t           iResult = M4_SUCCESS ;
	int                  iEqual ;
	m4uint8_t            iMiss = 0 ;
	m4uint8_t            iFunction ;
	m4uint16_t           i, j ;
	/* Bug 0094810
	El tipo no puede ser m4uint16_t
	*/
	ClRegisterIndex      iStart = 0;
	m4uint16_t           iColumns ;
	m4VariantType        vValue ;
	m4uint32_t           iItemHandle;
	m4bool_t             bFound = M4_FALSE;
	
	StFindRegisterCol_t  stColumnsBuffer [3] ;
	StFindRegisterCol_t* pstColumns = stColumnsBuffer ;  //truco, ver comentario más abajo
	ClRegisterIndex      iOldIndex = Index;
	
	if ( ai_iArguments < 3 )
	{
		ClCompiledMCR * pmcr   = GetpNode_Data()->GetpChannel_Data ()->GetpChannelDef()->GetpCMCR ();
		ClHandle NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
		
		DUMP_CHLOG_ERRORF(M4_CH_DM_WRONG_PARAM_NUM, ai_iArguments << M4ObjNodeN(pmcr, NodeHandle));
		
		iResult = M4_ERROR ;
	}
	else if( ( ai_iArguments  % 3 ) == 1 )
	{
		ai_iArguments -- ;
		
		if( ai_pstArguments[ ai_iArguments  ].Type != M4CL_CPP_TYPE_NUMBER )
		{
			ClCompiledMCR * pmcr   = GetpNode_Data()->GetpChannel_Data ()->GetpChannelDef()->GetpCMCR ();
			ClHandle NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
			
			DUMP_CHLOG_ERRORF(M4_CH_DM_TYPE_MISMATCH, ai_iArguments <<
				M4ClCppType(ai_pstArguments[ ai_iArguments  ].Type) << 
				M4ObjNodeN(pmcr, NodeHandle));
			
			iResult = M4_ERROR ;
		}
		
		iStart = (ClRegisterIndex) ai_pstArguments[ ai_iArguments  ].Data.DoubleData ;
	}
	else if( ( ai_iArguments  % 3 ) != 0 )
	{
		ClCompiledMCR * pmcr   = GetpNode_Data()->GetpChannel_Data ()->GetpChannelDef()->GetpCMCR ();
		ClHandle NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
		
		DUMP_CHLOG_ERRORF(M4_CH_DM_WRONG_PARAM_NUM, ai_iArguments << M4ObjNodeN(pmcr, NodeHandle));
		
		iResult = M4_ERROR ;
	}
	
	iColumns = ai_iArguments  / 3 ;
	
	
	if( iResult == M4_SUCCESS )
	{
		//EDU: optimizacion en velocidad, si hay menos de x columnas
		//usamos el buffer auxiliar, sino alocamos dinamicamente
		if (iColumns > 3) {
			pstColumns = new StFindRegisterCol_t[ iColumns ] ;
		}
		
		if( pstColumns == NULL )
		{
			// DUMP_CHLOG_ERRORF(M4_CH_MCR_NO_MEMORY, (sizeof( StFindRegisterCol_t ) * iColumns));
			iResult = M4_ERROR ;
		}
	}
	
	
	if( iResult == M4_SUCCESS )
	{
		i = 0 ;
		
		for( j = iColumns ; j > 0; j-- )
		{
			// sacamos el item
			
			//el item->por indice
			if (ai_pstArguments[ i ].Type == M4CL_CPP_TYPE_NUMBER ){ //viene por indice
				pstColumns[ j - 1 ].iItemIndex = (m4uint32_t) ai_pstArguments[ i ].Data.DoubleData ;
			}
			else if (ai_pstArguments[ i ].Type == M4CL_CPP_TYPE_STRING_VAR ){ //viene por nombre
				iItemHandle = GetpNodeDef()->GetNodeItemHandle( ai_pstArguments[ i ].Data.PointerChar  ) ;
				if( iItemHandle == 0 )
				{
					ClCompiledMCR * pmcr   = GetpNode_Data()->GetpChannel_Data ()->GetpChannelDef()->GetpCMCR ();
					ClHandle NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
					
					DUMP_CHLOG_ERRORF(M4_CH_DM_NO_SUCH_ITEM,
						ai_pstArguments[ i ].Data.PointerChar << M4ObjNodeN(pmcr, NodeHandle));
					iResult = M4_ERROR ;
				}
				pstColumns[ j - 1 ].iItemIndex = GetpNodeDef()->GetItemIndex( iItemHandle ) ; 
				
			}
			else
			{
				ClCompiledMCR * pmcr   = GetpNode_Data()->GetpChannel_Data ()->GetpChannelDef()->GetpCMCR ();
				ClHandle NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
				
				DUMP_CHLOG_ERRORF(M4_CH_DM_ITEM_TYPE_MISMATCH2, j << i <<
					M4ClCppType(ai_pstArguments[ i ].Type) << M4ObjNodeN(pmcr, NodeHandle));
				
				iResult = M4_ERROR ;
			}
			
			i++ ;
			
			// sacamos el valor
			
			pstColumns[ j - 1 ].pvValue = &ai_pstArguments[ i ] ;
			
			i++ ;
			
			// sacamos la funcion
			
			if( ai_pstArguments[ i ].Type != M4CL_CPP_TYPE_NUMBER )
			{
				DUMP_CHLOG_ERRORF( M4_CH_DM_ARGUMENT_TYPE_MISMATCH, i << M4ClCppType( ai_pstArguments[ i ].Type ) << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) ) ;
				iResult = M4_ERROR ;
			}
			
			iFunction = m4uint8_t( ai_pstArguments[ i ].Data.DoubleData ) ;
			
			if( iFunction < 1 || iFunction > M4CL_MAX_VARIANT_COMPARE_FUNCTION )
			{
				DUMP_CHLOG_ERRORF( M4_CH_DM_INVALID_COMPARE_FUNCTION, iFunction << 1 << M4CL_MAX_VARIANT_COMPARE_FUNCTION ) ;
				iResult = M4_ERROR ;
			}
			else
			{
				pstColumns[ j - 1 ].pfFunction = g_apfM4VariantCompareFunctions[ iFunction - 1 ] ;
			}
			
			i++ ;
		}
	}
        
	ClRegExp oRegex;
	if( iResult == M4_SUCCESS )
	{
		MoveTo (iStart);
		
		while (Index != M4DM_EOF_INDEX) {
			iMiss = 0 ;

			for( j = 0 ; j < iColumns; j++ )
			{
				iResult = Item[ pstColumns[ j ].iItemIndex ].Value.Get( vValue ) ;
				
				if( iResult != M4_SUCCESS )
				{
					m4uint32_t ItemHandle  = Item.ItemDef.GetHandle();
					if (ItemHandle != 0)
					{
						ClCompiledMCR * pmcr   = GetpNode_Data()->GetpChannel_Data ()->GetpChannelDef()->GetpCMCR ();
						
						//JCM Bug 0131197 sustituimos i por Index
						DUMP_CHLOG_ERRORF(M4_CH_DM_OPERATION_VALUE_WITH_INDEX, Index << M4ObjNodeItemI(pmcr, ItemHandle));
					}
					else
					{
						m4uint32_t NodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
						ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
						
						/*JCM Bug 0131197 Da error al usar la función M4ObjNodeItemI
						Pq el handle del item es inválido*/
						//DUMP_CHLOG_ERRORF(M4_CH_DM_OPERATION_VALUE_WITH_INDEX_EOF, i << M4ObjNodeItemI(pmcr, NodeHandle));
						DUMP_CHLOG_ERRORF(M4_CH_DM_OPERATION_VALUE_WITH_INDEX_EOF, (iColumns-j) << M4ObjNodeN(pmcr, NodeHandle));

					}

					
					if( pstColumns != stColumnsBuffer )
					{
						delete [] pstColumns ;
					}

					return M4_ERROR;

					iMiss = 1 ;
					break;
				}
				else
				{
					iEqual = pstColumns[ j ].pfFunction( &vValue, pstColumns[ j ].pvValue, &oRegex ) ;
					
					if( iEqual == 0 )
					{
						iMiss = 1 ;
						break;
					}
				}
			}
			
			if( iMiss == 0 )
			{
				bFound = M4_TRUE;
				break;
			}

			Next ();
		}
	}
	
	if (!bFound) {
		MoveToEOF();
	} else {
		if (Index != iOldIndex)
		{
			iOldIndex = Index;
			MoveToEOF();
			MoveTo(iOldIndex);
		}
	}
	
	if (ao_iRegister)
		*ao_iRegister = Index;
	
	if( pstColumns != stColumnsBuffer )
	{
		delete [] pstColumns ;
	}
	
	return iResult;
}

void _ClRegister_Base::Follow (const _ClRegister_Base &ai_reg)
{
    //copiamos el estado (los index)

    M4_ASSERT (GetpNode_Data () == ai_reg.GetpNode_Data ());

    //primero el bloque, que pone los index a EOF
    Item.SetCurrentBlock (ai_reg.GetCurrentBlock ());

    Index = ai_reg.Index;
    MMIndex = ai_reg.MMIndex;
}

//********************************************************************************
// FUNCIONES DE ESCRITURA: ESTAN IMPLEMENTADAS EN OTRA CLASE
//********************************************************************************

//----------------------------------------------------------------------------
// Delete
//----------------------------------------------------------------------------

m4return_t _ClRegister_Base::Delete (m4return_t * ao_pDone)
{
    return GetpNode_ExtInfo()->GetpRegDelete()->Delete(this, ao_pDone);
}

// marca para borrar todos los registros del recordset:

m4return_t _ClRegister_Base::DeleteAll (m4return_t * ao_pDone)
{
    return GetpNode_ExtInfo()->GetpRegDelete()->DeleteAll(this, ao_pDone);
}

//----------------------------------------------------------------------------
// Destroy
//	borra fisicamente el registro
//----------------------------------------------------------------------------

m4return_t _ClRegister_Base::Destroy (m4return_t * ao_pDone)
{
    return GetpNode_ExtInfo()->GetpRegDelete()->Destroy(this, ao_pDone);
}

// 'destruye' directamente todos los registros del recordset:

m4return_t _ClRegister_Base::DestroyAll (m4return_t * ao_pDone)
{
    return GetpNode_ExtInfo()->GetpRegDelete()->DestroyAll(this, ao_pDone);
}

//----------------------------------------------------------------------------
// Add
//      Añade y lleva el register allí
//----------------------------------------------------------------------------

m4return_t _ClRegister_Base::Add (m4return_t * ao_pDone)
{
    return GetpNode_ExtInfo()->GetpRegInsert()->Add(this, ao_pDone);
}

m4return_t _ClRegister_Base::Insert (m4return_t * ao_pDone)
{
    return GetpNode_ExtInfo()->GetpRegInsert()->Insert(this, ao_pDone);
}

m4bool_t _ClRegister_Base::GetNewNotPersist(void) const
{
    return GetCurrentBlock()->R_GetNewNotPersist(MMIndex);
}

m4return_t _ClRegister_Base::SetNewNotPersist(const m4bool_t ai_bNewNotPersist)
{
    return GetCurrentBlock()->R_SetNewNotPersist(MMIndex, ai_bNewNotPersist);
}

void _ClRegister_Base::SetCurrentBlock (ClBlock_Channel_Proxy *ai_pblock)
{
	ClBlockReference br;
	br.Set (ai_pblock);

	SetCurrentBlock (br);
}

void _ClRegister_Base::SetCurrentBlock (const ClBlockReference &ai_br)
{
    if (ai_br->IsNihil ()) {
        //no tocamos el estado del register
    }
    else {
        ClNode_Data *pn = ai_br->GetpNode_Data ();

        if (GetpNode_Data () != pn) {
            /*
            //soy un expontaneo?
            if (0 == GetpNode_Data ()) {
                //si, entonces me engancho automáticamente al notifier
                _AttachNotifier (pn->GetpChannel_Data ()->GetpNotifier ());
            }
            */

            Attach (pn);
        }
    }

    //que no se me olvide ...
    MoveToEOF ();   //virtual

    Item.SetCurrentBlock (ai_br);
}

m4return_t _ClRegister_Base::_UndoCascade (void)
{
	m4uint16_t fisrtCon, numCon, i;
	m4uint32_t hConn;


    ClNodeDef *pndef = GetpNodeDef ();

	numCon = pndef->GetNodeNumberOfRBConnectors ();
	fisrtCon = pndef->GetNodeRBFirstConnector ();

	for (i = fisrtCon; i < numCon; i++)
	{
		//Recorremos los conectores RB
		hConn = pndef->GetNodeConnectorHandleByPosition ( i );
			
		if ( pndef->GetNodeConnectorExpandUndo ( hConn ) )
		{
			//hay que borrar los registros del bloque
			//ClRecordSet *poRecordSet;				
			ClRegisterIndex j(0);

			m4uint32_t hSonNode = pndef->GetNodeConnectorSonNodeHandle ( hConn );
				
			ClBlock_Channel_Proxy * pBlock = GetChildBlock ( (ClHandle)hSonNode );

			if ( pBlock->IsValid () )    // testear
			{
				/* Bug 0087819
				Hay que hacer el Undo del bloque hijo también, no solo de
				los registros hijos
				*/
				pBlock->B_Undo() ;

                ClRegister reg (0); //sin autoload
				reg.SetCurrentBlock (pBlock);
                
				// Recorro los registros de atras a alante por si al hacer el undo
				// se borra algún registro, que no nos afecte.
                m4bool_t isNotEOF = M4_FALSE;
				m4int32_t numReg = reg.Count();
				for (m4int32_t j = numReg - 1; j >= 0; j--) {
					isNotEOF = reg.MoveTo (j);
					if (isNotEOF)
					{
						/* Bug 0087819
						No hay que hacer el undo de los registros hijos, porque
						ya lo hace el undo del bloque hijo, pero si que hay que
						seguir propagando el undo
						*/
						reg._UndoCascade();
					}
				}
			}
		}
	}		
	
	/* Bug 0087819
	Hay que actualizar los conectores, por si cambia el valor
	*/
	m4uint32_t	n = Item.Count() ;

	for( i = 0 ; i < n ; i++ )
	{
		Item[ ClItemIndex( i ) ] ;

		if( Item.ItemDef.Type() != M4CL_ITEM_TYPE_METHOD )
		{
			Item._NotifyChange() ;
		}
	}

	return M4_SUCCESS;
}


m4return_t _ClRegister_Base::Undo(void)
{
    m4return_t res = M4_SUCCESS;

    if (MMIndex != ClMMIndex_EOF)
    {
        if (GetCurrentBlock()->R_Undo (MMIndex))
        {
            MoveToEOF();
        }

		res = _UndoCascade ();

		/* Bug 0087819
		El refresco hay que hacerlo siempre aunque lo anterior haya ido mal
		*/
		GetpNode_Data()->GetpChannel_Data()->RefreshFlags();
    }

    return res;
}
m4return_t _ClRegister_Base::CheckPoint(void)
{
    if (MMIndex != ClMMIndex_EOF)
    {
        GetCurrentBlock()->CheckPoint (MMIndex);
        GetpNode_Data()->GetpChannel_Data()->RefreshFlags();
    }

    return M4_SUCCESS; //Si estoy en EOF no hago nada
}


ClNode * _ClRegister_Base::GetpNode (void) const
{
    ClAccess * paccess = GetpAccess ();
    if (paccess)
        return paccess->Node.Get(m_pClNode_Data->GetpNodeDef()->GetHandle());
    else
        return 0;
}

ClNode_Kernel *_ClRegister_Base::GetpNode_ExtInfo (void) const
{
    M4_ASSERT (GetpNode_Data ());
    return GetpNode_Data ()->GetpExtInfo ();
}

ClChannelDef *_ClRegister_Base::GetpChannelDef (void) const
{
    M4_ASSERT (GetpNode_Data ());
    return GetpNode_Data ()->GetpChannel_Data ()->GetpChannelDef ();
}

ClNodeDef *_ClRegister_Base::GetpNodeDef (void) const
{
    return GetpNode_ExtInfo ();
}

ClChannel *_ClRegister_Base::GetpChannel (void)
{
    ClAccess * pAccess = GetpAccess ();
    if (pAccess)
        return GetpAccess ()->GetpChannel ();
    else
        return 0;
}


m4return_t _ClRegister_Base::CopyRecordWithSerialize(ClFileName & ai_oFileName)
{
	if (Index == M4DM_EOF_INDEX) {
		ai_oFileName.SetFileName(0);
		return M4_ERROR;
	}
        return GetCurrentBlock()->CopyRecordWithSerialize(MMIndex, ai_oFileName);
}

m4return_t _ClRegister_Base::PasteRecordWithDeSerialize(const m4bool_t     ai_bNewRecord,
                                                        const m4bool_t     ai_bKeepMarks,
                                                        const ClFileName & ai_oFileName,
                                                        const m4bool_t     ai_bDeleteFile,
                                                        const m4bool_t     ai_bDestroyRecords,
                                                        const m4char_t*    ai_vpExclusionNodeName[],
                                                        const m4uint16_t   ai_iNumExclusionNodes,
                                                        const m4bool_t     ai_bKeepAccesses)
{
    ClMMIndex   auxIndex;
    m4return_t  res;
    ClAccessBookMark * pBM = 0;

    if (ai_bNewRecord)
        auxIndex = M4DM_EOF_INDEX;
    else
        auxIndex = MMIndex;

    if (!ai_oFileName.IsNull())
    {
        if (ai_bKeepAccesses)
        {
            if ((GetpAccess())&&(!ai_bNewRecord))
            {
                ClChannel * pChannel = GetpAccess()->GetpChannel();

                m4uint32_t n = pChannel->Access.Count(), i;
                pBM = new ClAccessBookMark[n];

                for (i = 0; i < n; i++)
                {
                    if (pChannel->Access[i] != GetpAccess())
                        pBM[i].GetFrom(*(pChannel->Access[i]));
                }
            }
        }

        res = GetCurrentBlock()->PasteRecordWithDeSerialize(auxIndex,
                                                            ai_bKeepMarks,
                                                            ai_bDestroyRecords,
                                                            ai_vpExclusionNodeName,
                                                            ai_iNumExclusionNodes,
                                                            ai_oFileName,
                                                            ai_bDeleteFile);
        if (ai_bKeepAccesses)
        {
            if ((GetpAccess())&&(!ai_bNewRecord))
            {
                ClChannel * pChannel = GetpAccess()->GetpChannel();

                m4uint32_t n = pChannel->Access.Count(), i;

                for (i = 0; i < n; i++)
                {
                    if (pChannel->Access[i] != GetpAccess())
                        pBM[i].ApplyTo(*(pChannel->Access[i]));
                }
				delete[] pBM;
            }
        }
    }
    else
    {
        MoveToEOF();
        return M4_ERROR;
    }

    if (res == M4_SUCCESS)
        MoveTo(ClRegisterIndex(auxIndex));

    GetpNode_Data()->GetpChannel_Data()->RefreshFlags();

    return res;
}

m4return_t _ClRegister_Base::ReviewNewRegisters (m4uint32_t & ao_index)
{
	if (GetpNodeDef ()->NumRows() != 0)
	{
		ao_index = M4DM_EOF_INDEX;
		return M4_SUCCESS;
	}

	ClRegister oAuxReg = (*this);

	m4bool_t bpassed = M4_TRUE;
	m4return_t res = M4_SUCCESS;
	m4VariantType var, vDefVal;

	ao_index = M4DM_EOF_INDEX;

	oAuxReg.Item.SetMode(M4DM_SCOPE_REGISTER, M4DM_TYPE_PROPERTY | M4DM_TYPE_FIELD | M4DM_TYPE_CONCEPT);
	oAuxReg.End();
	while (oAuxReg.Index != M4DM_EOF_INDEX)
	{
		if (oAuxReg.Operation.IsNew())
		{
			// jcr Sólo si realmente es nuevo
			if (oAuxReg.GetNewNotPersist())
			{
				bpassed = M4_FALSE;
				break;
			}
			
			oAuxReg.Item.Begin ();
			while (oAuxReg.Item.GetIndex() != M4DM_EOF_INDEX)
			{
				bpassed = M4_FALSE;
				
				oAuxReg.Item.Value.GetWithOutSec(var);
				oAuxReg.Item.ItemDef.GetItemDefaultValue (vDefVal);
				
				if ((var.Type != 0)&&(var != vDefVal))
				{
					bpassed = M4_TRUE;
					break;
				}
				
				oAuxReg.Item.Next ();
			}
			if (bpassed) {
				oAuxReg.Prev();
			} else {
				break;
			}
		} else {
			oAuxReg.Prev();
		}
	}
	
	if (bpassed) {
		ao_index = M4DM_EOF_INDEX;
	} else {
		ao_index = oAuxReg.Index;
	}
	
	return res;
}

// Nuevo: Identificador unico de registros:
m4return_t _ClRegister_Base::GetRecordID (m4uint32_t &ao_iUniqueId)
{
    if (Index == M4DM_EOF_INDEX) return M4_ERROR;

    return GetCurrentBlock()->R_GetRecordID(MMIndex, ao_iUniqueId);
}

m4return_t _ClRegister_Base::SetRecordID (m4uint32_t ai_iUniqueId)
{
    if (Index == M4DM_EOF_INDEX) return M4_ERROR;

    return GetCurrentBlock()->R_SetRecordID(MMIndex, ai_iUniqueId);
}

m4return_t _ClRegister_Base::GetIndexOf (m4uint32_t ai_iUniqueId, ClRegisterIndex& ao_riIndex)
{

	m4return_t				iResult = M4_SUCCESS ;
	m4uint32_t				iUniqueId = 0 ;
	ClRegisterIndex			iOldIndex = -1 ;
	ClBlock_Channel_Proxy	*poCurrentBlock = NULL ;
	m4uint32_t				iFirst = 0;
	m4uint32_t				iRows = 0;
	m4uint32_t				iNumRows = 0;
	ClPartialLoadData		*poPartial = NULL ;


	ao_riIndex = -1 ;

    poCurrentBlock = GetCurrentBlock() ;

	// Antes de iterar se comprueba el registro actual
	if( MMIndex != M4DM_EOF_INDEX )
	{
		iResult = poCurrentBlock->R_GetRecordID( MMIndex, iUniqueId ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		if( iUniqueId == ai_iUniqueId )
		{
			ao_riIndex = Index ;
			return( M4_SUCCESS ) ;
		}
	}

	iNumRows = GetpNode_Data()->GetpNodeDef()->NumRows() ;

	if( iNumRows != 0 )
	{
		// Hay carga parcial
		poPartial = GetCurrentBlock()->GetpPL_Data();

		if( poPartial != NULL )
		{
			iFirst = poPartial->GetFirstVisible() ;
			iRows = poPartial->GetLastVisible() - iFirst ;
		}
		else
		{
			iFirst = 0 ;
			iRows = 0 ;
		}
	}
	else
	{
		iFirst = 0 ;
		iRows = Count() ;
	}

	iOldIndex = Index ;

	for (int i = 0; i < iRows; i++)
	{
		_ClRegister_Base::MoveTo(i + iFirst);

		iResult = poCurrentBlock->R_GetRecordID( MMIndex, iUniqueId ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		if( iUniqueId == ai_iUniqueId )
		{
			ao_riIndex = Index ;
			break ;
		}
	}

	// Se deja la posición que estaba
	_ClRegister_Base::MoveTo( iOldIndex ) ;

	return( M4_SUCCESS ) ;
}



void _ClRegister_Base::DestroyRBChilds (ClMMIndex ai_regindex)
{
   m4uint16_t i, numRBs;

   if (GetpNode())
   {
       if (GetpNode()->GetpLane_Info())
       {
            numRBs = GetpNode()->GetpLane_Info()->ChildNodeRB.Count ();
            if (numRBs) {
                for (i = 0; i < numRBs; i++) {
                    ClNode * pnchild = GetpNode()->GetpLane_Info()->ChildNodeRB [(ClChildIndex)i].GetAddress ();

                    //que no cree el bloque si no existe:
                    ClBlockReference br;
                    pnchild->GetpNode_Data()->GetpBlockStore()->Get(GetCurrentBlock()->ID.BlockUniqueKey, ai_regindex, br);
                    //ahora puede ser cero:
                    if (br) {
                        //elimino el bloque:
                        br->DestroyProxy (M4_TRUE); // hace un delete de pb
                        //hay que hacerlo así para que no pete:
                        br.Invalide ();
                        //M4_TRUE ==> que elimine el bloque del store			  
                    }

			        //y avisamos para que se corran (con perdon) para atras
			        pnchild->GetpNode_Data()->GetpBlockStore ()->Notify_ParentRemoveRegister (GetCurrentBlock()->ID.BlockUniqueKey, GetMMIndex ());
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// _ClRegister_Free
//


void _ClRegister_Free::NotifierDetach (void)
{
if (m_bNotifierEnabled) {
    //por si acaso estuviera atachado a un notifier ...
    if (GetpNotifier ()) {
        GetpNotifier ()->Detach (this);

        //aquí me tiene que llegar un Detached_Before
        //m_pClNotifier = 0;
    }
}
}

void _ClRegister_Free::NotifierAttach (ClNotifier_Channel_Data *ai_pnotifier)
{
if (m_bNotifierEnabled) {

    NotifierDetach ();

    //no hace falta, se hace en el attached_after
    //m_pClNotifier = ai_pnotifier;

    //si me indican un notifier, me apunto a él.
    if (ai_pnotifier) {
        ai_pnotifier->Attach (this);

        //y aquí un Attached_After
        //m_pClNotifier = 0;
    }
    //else {
    //    m_pClNotifier = 0;
    //}

    //M4_ASSERT (m_pClNotifier == ai_pnotifier);
}
}

void _ClRegister_Free::Attach (ClNode_Data *ai_pnc)
{
if (m_bNotifierEnabled) {
    //estoy apuntado al sistema de notificaciones, así que me actualizo
    ClNotifier_Channel_Data *pnewn = ai_pnc->GetpChannel_Data ()->GetpNotifier ();
    
    if (GetpNotifier () != pnewn) {
        NotifierAttach (pnewn);
    }

    //comprobación:
    //M4_ASSERT (GetpNotifier () == pnewn); //no hasta que no tenga nodo
}

    _ClRegister_Base::Attach (ai_pnc);
}

ClNotifier_Channel_Data *_ClRegister_Free::GetpNotifier (void) const
{
if (m_bNotifierEnabled) {
    if (GetpNode_Data ()) {
		if (GetpNode_Data ()->GetpChannel_Data ()) {
			//puede ser cero si el canal no está construido
        	M4_ASSERT (GetpNode_Data ()->GetpChannel_Data ()->GetpNotifier ());

        	return GetpNode_Data ()->GetpChannel_Data ()->GetpNotifier ();
		}
    }
}

return 0;
}

void _ClRegister_Free::Detach (void)
{
    NotifierDetach ();

    _ClRegister_Base::Detach ();
}

void _ClRegister_Free::Attach (ClAccess *ai_pacccess)
{
    _SetpAccess (ai_pacccess);

    if (GetpNode_Data ()) {
        //se actualiza:
        Filter.SetAutoLoadMode (_GetAutoLoadMode ());
    }

    Item.Attach (GetpAccess ());
}

_ClRegister_Free::_ClRegister_Free (const _ClRegister_Base &ai_register) : _ClRegister_Base (ai_register)
{
    m_bNotifierEnabled = M4_TRUE;    //siempre activo con este constructor

    //m_pClNotifier = 0;
    //_Copy (ai_register);

    //ojo al dato: el Attach (pnode) al que se llama
    //en el constructor de la clase base NO llama al Attach mio que tengo redefinido
    //(precisamente por estar llamado en el constructor
    NotifierAttach (GetpNode_Data ()->GetpChannel_Data ()->GetpNotifier ());
}

_ClRegister_Free::_ClRegister_Free (const _ClRegister_Free &ai_register) : _ClRegister_Base (ai_register)
{
    m_bNotifierEnabled = M4_TRUE;    //siempre activo con este constructor

    //m_pClNotifier = 0;
    //_Copy (ai_register);

    //mismo comentario que antes
    NotifierAttach (GetpNode_Data ()->GetpChannel_Data ()->GetpNotifier ());
}

m4return_t 
_ClRegister_Free::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(_ClRegister_Free);
		ao_size-=sizeof(_ClRegister_Base);
	if (_ClRegister_Base::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	return M4_SUCCESS;
}

m4return_t _ClRegister_Base::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(_ClRegister_Base);
	ao_size-=sizeof(ClAccessProvider);
	ao_size-=sizeof(_AClClient_Notifier);
	ao_size-=sizeof(Item);
	ao_size-=sizeof(LastUpdated);
	ao_size-=sizeof(FlagPersist);
	ao_size-=sizeof(Operation);
	ao_size-=sizeof(Filter);

	if (ClAccessProvider::GetSize(ao_size) != M4_SUCCESS){
		return M4_ERROR;
	}
	if (_AClClient_Notifier::GetSize(ao_size) != M4_SUCCESS){
		return M4_ERROR;
	}
	if (Item.GetSize(ao_size) != M4_SUCCESS){
		return M4_ERROR;
	}
	if (LastUpdated.GetSize(ao_size) != M4_SUCCESS){
		return M4_ERROR;
	}
	if (FlagPersist.GetSize(ao_size) != M4_SUCCESS){
		return M4_ERROR;
	}
	if (Operation.GetSize(ao_size) != M4_SUCCESS){
		return M4_ERROR;
	}
	return Filter.GetSize(ao_size);
}

m4return_t 
ClLastUpdated::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClLastUpdated);
	return M4_SUCCESS;
}


m4return_t 
ClPersist_Register::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClPersist_Register);
	return M4_SUCCESS;
}




#include "dmres.hpp"

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             arecset.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             22/09/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    At last the ClRecordSet !!
//
//
//==============================================================================

//## begin module.includes preserve=yes
//----------------------------------------------------------------------------
#include "dm_def.hpp"
#include "m4define.hpp"
//----------------------------------------------------------------------------

#include "arecset.hpp"
#include "node_knl.hpp"
#include "access.hpp"
#include "blockref.hpp"
#include "px_block.hpp"
#include "compst_common.hpp"
#include "chan_knl.hpp"
#include "iexecutor.hpp"
#include "not_cdat.hpp"
#include "bnihil.hpp"
#include "m4mdrt.hpp"

//## end module.includes preserve=yes


///////////////////////////////////////////////////////////////////////////////
//
// ClFlagPersist_RecordSet
//

m4bool_t ClFlagPersist_RecordSet::Get (void)
{
    M4_ASSERT (m_pClRecordSet);

    m4bool_t ai_bool;

    m_pClRecordSet->GetFlagPersisted (ai_bool);

    return ai_bool;
}

void ClFlagPersist_RecordSet::Set (m4bool_t ai_bool)
{
    M4_ASSERT (m_pClRecordSet);

    m_pClRecordSet->SetFlagPersisted (ai_bool);
}

ClFlagPersist_RecordSet::ClFlagPersist_RecordSet ()
{
    m_pClRecordSet = 0;    
}

void ClFlagPersist_RecordSet::Attach (_ClRecordSet_Base *ai_prs)
{
    m_pClRecordSet = ai_prs;
}


///////////////////////////////////////////////////////////////////////////////
//
// _ClRecordSet_Base
//

_ClRecordSet_Base::_ClRecordSet_Base (ClAccess *ai_paccess) : Item (ai_paccess, M4DM_SCOPE_NODE | M4DM_SCOPE_BLOCK)
{
    Operation.Attach (this);
    FlagPersist.Attach (this);

    m_pFooter = 0;

    m_pClNode_Data = 0;

    // item
    //Item.Attach ();
}

void _ClRecordSet_Base::Attach (ClNode_Data *ai_pClNode_Data)
{
    M4_ASSERT (ai_pClNode_Data);

    m_pClNode_Data = ai_pClNode_Data;

    // item
    Item.Attach (ai_pClNode_Data);
}

void _ClRecordSet_Base::Detach (void)
{
    m_pClNode_Data = 0;

    // item
    Item.Detach ();
}

m4bool_t _ClRecordSet_Base::IsAttached (void)
{
    return ((m_pClNode_Data != 0) ? M4_TRUE : M4_FALSE);
}

m4bool_t _ClRecordSet_Base::IsEOF (void)
{
    return IsValid ();
}


void _ClRecordSet_Base::_SetCurrentBlock (const ClBlockReference &ai_br)
{
    M4_ASSERT (ai_br);

    //el bloque de entrada puede ser nihil:
    //M4_ASSERT (ai_pblock->GetpNode_Kernel ()->NodeDef.GetHandle () == m_pClNode_Data->NodeDef.GetHandle ());

    Item.SetCurrentBlock (ai_br);
}


ClNode * _ClRecordSet_Base::GetpNode (void) const
{
    ClAccess * paccess = GetpAccess ();
    if (paccess)
        return paccess->Node.Get(m_pClNode_Data->GetpNodeDef()->GetHandle());
    else
        return 0;
}

ClBlock_Channel_Proxy *_ClRecordSet_Base::GetCurrentBlock (void) const
{
    M4_ASSERT (Item.GetCurrentBlock ()); // no debería devolver 0 nunca !

    return Item.GetCurrentBlock ();
}

m4bool_t _ClRecordSet_Base::SetCurrentBlock (const ClBlockReference &ai_br)
{
    // en cualquier caso:
    m4bool_t bismodified;

    if (ai_br != GetCurrentBlockRef ()) {	// operator != sobrecargado
        if (!ai_br->IsNihil ()) {

            ClNode_Data *pn = ai_br->GetpNode_Data ();

            //si estoy 'atachado' a otro nodo, me atacho al nuevo:
            if (m_pClNode_Data != pn)
                Attach (pn);
        }

        //le paso la bola al resto de gente:
        _SetCurrentBlock (ai_br);   //virtual

        bismodified = M4_TRUE;
    }
    else {
        bismodified = M4_FALSE;
    }

    return bismodified;
}

//----------------------------------------------------------------------------
// IsValid:
//      devuelve true si el bloque activo es válido (si no es nihil)
//----------------------------------------------------------------------------

m4bool_t _ClRecordSet_Base::IsValid (void)
{
    M4_ASSERT (GetCurrentBlock ());

    return ( (!GetCurrentBlock ()->IsNihil ()) ? M4_TRUE : M4_FALSE );
}

//a extinguir:
m4uint32_t _ClRecordSet_Base::Count (void)
{
    M4_ASSERT (GetCurrentBlock ());

    return GetCurrentBlock ()->Count ();
}

// destruye completamente el bloque activo 
// (los registros no se marcan de ninguna forma, se eliminan directamente)

m4return_t _ClRecordSet_Base::Destroy (void)
{
    M4_ASSERT (GetCurrentBlock ());

    m4return_t res;

    res = GetCurrentBlock ()->DestroyBlock ();

/*
    //novedad, en la nueva implementación saco el proxy del store y le elimino:

    ClBlock_Channel_Proxy *pb;
    pb = GetCurrentBlock ();

    M4_ASSERT (m_pClNode_Data->GetpBlockStore ()->Get (pb->ID.BlockId, pb->ID.RegisterId, pb) == M4_SUCCESS);
    M4_ASSERT (pb == GetCurrentBlock ());    

    res = m_pClNode_Data->GetpBlockStore ()->Remove (pb->ID.BlockId, pb->ID.RegisterId);

    delete pb;
*/

    return res;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

m4return_t _ClRecordSet_Base::CheckPoint (void)
{
    m4return_t res;
    res = GetCurrentBlock ()->CheckPoint ();
    GetpNode_Data()->GetpChannel_Data()->RefreshFlags();

    return res;
}

m4return_t _ClRecordSet_Base::CheckPoint (m4uint32_t iMMindex)
{
    return GetCurrentBlock ()->CheckPoint (iMMindex);
}

m4return_t _ClRecordSet_Base::Undo (void)
{
    m4return_t res;

    res = GetCurrentBlock ()->B_Undo ();
    GetpNode_Data()->GetpChannel_Data()->RefreshFlags();

    return res;
}


m4return_t _ClRecordSet_Base::SetCheckPointMode (m4bool_t ai_operation)
{
    M4_ASSERT (GetCurrentBlock ());

    return GetCurrentBlock ()->SetCheckPointMode (ai_operation);
}

m4return_t _ClRecordSet_Base::GetCheckPointMode (m4bool_t& ao_operation)
{
    M4_ASSERT (GetCurrentBlock ());

    return GetCurrentBlock ()->GetCheckPointMode (ao_operation);
}

//----------------------------------------------------------------------------
// only for Joseco:
//  hace commit en los bloques en los que se marcó el flag de 'persist'
//  y se limpia el flag de 'persist'
//----------------------------------------------------------------------------

m4return_t _ClRecordSet_Base::CommitPersisted (VersionMark &ai_version)
{
    m4return_t res = M4_SUCCESS;

    if (FlagPersist.Get ()) {
        const ClBlockReference &br = GetCurrentBlockRef ();
        res = br->CommitPersisted (ai_version);

        if (res == M4_SUCCESS) {

            if (res == M4_SUCCESS) {
                FlagPersist.Set (M4_FALSE);
                //Operation.Reset ();
            }
        }
    }

    return res;
}

//----------------------------------------------------------------------------
// only for Joseco:
//  hace rollback en los bloques en los que se marcó el flag de 'persist'
//  y se limpia el flag de 'persist'
//----------------------------------------------------------------------------

m4return_t _ClRecordSet_Base::RollbackPersisted (void)
{
    m4return_t res = M4_SUCCESS;

    if (FlagPersist.Get ()) {
        const ClBlockReference &br = GetCurrentBlockRef ();
        res = br->RollbackPersisted ();

        if (res == M4_SUCCESS) {
            FlagPersist.Set (M4_FALSE);
            //Operation.Reset ();
        }
    }

    return M4_SUCCESS;
}

//----------------------------------------------------------------------------
// get child block:
//----------------------------------------------------------------------------

ClBlock_Channel_Proxy *_ClRecordSet_Base::GetChildBlock (ClChildIndex ai_pos)
{
	ClBlockReference br;
	GetChildBlock (ai_pos, br);

	ClBlock_Channel_Proxy *pb = br.GetpProxy ();

	//al salir, en el destructor de br se destruye el proxy si nadie más le estaba usando
	// --> mucho ojo ahora con esta función

    //para evitar esto, mientras dure la función, hago un truco para que no se elimine
    br.Invalide ();
    pb->DecRef ();

    return pb;
}

m4return_t _ClRecordSet_Base::GetChildBlock (ClChildIndex ai_pos, ClBlockReference &ao_br)
{
	m4return_t res;

    ClNode_Data *pnchild = m_pClNode_Data->GetpExtInfo ()->ChildNodeBB [ai_pos].GetAddress ();

    if (0 == pnchild)
    {
        ClCompiledMCR * pmcr   = GetpNode_Data ()->GetpChannel_Data ()->GetpChannelDef ()->GetpCMCR ();
        m4uint32_t NodeHandle  =  GetpNode_Data()->GetpNodeDef()->GetHandle();

        DUMP_CHLOG_ERRORF(M4_CH_DM_CHILDNODEDONTEXIST, M4ObjNodeN(pmcr, NodeHandle));

        ao_br.Set (&g_oNihilBlock);
		res = M4_ERROR;
    }
    else {
        M4_ASSERT (pnchild->GetpBlockStore ());

		const ClBlockReference &br = GetCurrentBlockRef ();

		//Si no existe el bloque, se crea:
        res = pnchild->GetpBlockStore ()->Get (br->ID.BlockUniqueKey, 0/*no se usa*/, ao_br, br->ID.BlockId, br->ID.RegisterId);
    }

    return res;
}

ClBlock_Channel_Proxy *_ClRecordSet_Base::GetChildBlock (ClHandle ai_handle)
{
	ClBlockReference br;
	GetChildBlock (ai_handle, br);

	ClBlock_Channel_Proxy *pb = br.GetpProxy ();

	//al salir, en el destructor de br se destruye el proxy si nadie más le estaba usando
	// --> mucho ojo ahora con esta función

    //para evitar esto, mientras dure la función, hago un truco para que no se elimine
    br.Invalide ();
    pb->DecRef ();

    return pb;
}

m4return_t _ClRecordSet_Base::GetChildBlock (ClHandle ai_handle, ClBlockReference &ao_br)
{
	m4return_t res;

    ClNode_Data *pnchild = m_pClNode_Data->GetpExtInfo ()->ChildNodeBB [ai_handle].GetAddress ();

    if (0 == pnchild)
    {
        ClCompiledMCR * pmcr   = GetpNode_Data ()->GetpChannel_Data ()->GetpChannelDef ()->GetpCMCR ();
        m4uint32_t NodeHandle  =  GetpNode_Data()->GetpNodeDef()->GetHandle();

        DUMP_CHLOG_ERRORF (M4_CH_DM_CHILDNODEDONTEXIST, M4ObjNodeN(pmcr, NodeHandle));

        ao_br.Set (&g_oNihilBlock);
		res = M4_ERROR;
    }
    else {
		M4_ASSERT (pnchild->GetpBlockStore ());

		const ClBlockReference &br = GetCurrentBlockRef ();

		res = pnchild->GetpBlockStore ()->Get (br->ID.BlockUniqueKey, 0/*no se usa*/, ao_br, br->ID.BlockId, br->ID.RegisterId);
	}

	return res;
}


//----------------------------------------------------------------------------
// funciones get cutres:
//----------------------------------------------------------------------------

ClNodeDef *_ClRecordSet_Base::GetpNodeDef (void) const
{
    return GetpNode_ExtInfo ();
}

ClNode_Kernel *_ClRecordSet_Base::GetpNode_ExtInfo (void) const
{
    M4_ASSERT (m_pClNode_Data);

    return m_pClNode_Data->GetpExtInfo ();
}

ClChannel_Kernel *_ClRecordSet_Base::GetpChannel_ExtInfo (void) const
{
    M4_ASSERT (m_pClNode_Data);

    return m_pClNode_Data->GetpChannel_Data ()->GetpExtInfo ();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

m4return_t _ClRecordSet_Base::IsModified (m4bool_t &ao_is) const
{
    M4_ASSERT (GetCurrentBlock ());

    ao_is = GetCurrentBlock ()->B_IsModified ();
    return M4_SUCCESS;
}

m4return_t _ClRecordSet_Base::IsNew (m4bool_t &ao_is) const
{
    M4_ASSERT (GetCurrentBlock ());

    ao_is = GetCurrentBlock ()->B_IsNew ();
    return M4_SUCCESS;
}

m4return_t _ClRecordSet_Base::IsUpdated (m4bool_t &ao_is) const
{
    M4_ASSERT (GetCurrentBlock ());

    ao_is = GetCurrentBlock ()->B_IsUpdated ();
    return M4_SUCCESS;
}

m4return_t _ClRecordSet_Base::IsDeleted (m4bool_t &ao_is) const
{
    M4_ASSERT (GetCurrentBlock ());

    ao_is = GetCurrentBlock ()->B_IsDeleted ();
    return M4_SUCCESS;
}

m4return_t _ClRecordSet_Base::IsModifiedByDDBB (m4bool_t &ao_is) const
{
    M4_ASSERT (GetCurrentBlock ());

    ao_is = GetCurrentBlock ()->B_IsModifiedByDDBB ();
    return M4_SUCCESS;
}

m4return_t _ClRecordSet_Base::GetFlagPersisted (m4bool_t &ao_is)
{
    M4_ASSERT (GetCurrentBlock ());

    return GetCurrentBlock ()->B_GetPersist (ao_is);
}

m4return_t _ClRecordSet_Base::SetFlagPersisted (m4bool_t ai_bool)
{
    M4_ASSERT (GetCurrentBlock ());

    return GetCurrentBlock ()->B_SetPersist (ai_bool);
}
m4return_t _ClRecordSet_Base::GetFlagReLoad (m4bool_t &ao_rl)
{
    M4_ASSERT (GetCurrentBlock ());

    return GetCurrentBlock ()->B_GetReLoad (ao_rl);
}
m4return_t _ClRecordSet_Base::SetFlagReLoad (m4bool_t ai_rl)
{
    M4_ASSERT (GetCurrentBlock ());

    return GetCurrentBlock ()->B_SetReLoad (ai_rl);
}


void _ClRecordSet_Base::FollowSimple (const _ClRecordSet_Base &ai_rs)
{
    //asignamos a saco el bloque:
    Item.SetCurrentBlock (ai_rs.Item.GetCurrentBlock ());

        M4_ASSERT (Item.GetCurrentBlock () == ai_rs.Item.GetCurrentBlock ());
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

m4return_t _ClRecordSet_Base::Load_Block (void)
{
    return Execute (M4CL_ITEM_LOAD_BLK, M4_FALSE);
}

m4return_t _ClRecordSet_Base::Load_Prg (void)
{
    return Execute (M4CL_ITEM_LOAD_PRG, M4_FALSE);
}

m4return_t _ClRecordSet_Base::Persist_Tree (void)
{
    return Execute (M4CL_ITEM_PERSIST_TREE, M4_FALSE);
}

// ojo: cableada para la Load y la Persist -> saca a capón un elemento
// de la pila después de cada Call

m4return_t _ClRecordSet_Base::Execute (m4pcchar_t ai_pMethodId, m4bool_t ai_bFromWrapper)
{
	m4return_t res;
		
	M4_ASSERT (ai_pMethodId);
	
	//consigo el access:
	ClAccess *paccess = Item.GetpAccess ();
	if (0 == paccess) {
		//no tengo access, a ver como narices quieren que ejecute nada
		ClCompiledMCR * pmcr   = GetpNode_Data ()->GetpChannel_Data ()->GetpChannelDef ()->GetpCMCR ();
		m4uint32_t NodeHandle  =  GetpNode_Data()->GetpNodeDef()->GetHandle();
		
		DUMP_CHLOG_ERRORF(M4_CH_DM_ITEMCALLNOACCESS, ai_pMethodId << M4ObjNodeN(pmcr, NodeHandle));
		
		return M4_ERROR;
	}
	
	//comprobación MUY importante:
	if (paccess->Node.Get (GetpNode_ExtInfo ()->Index)->RecordSet.GetCurrentBlock () != GetCurrentBlock ())	{
		return M4_ERROR;
	}
	
	//Comprobación más por el tema del carril:
	//Puede ser que el bloque esté a nihil, para el caso de una load o persist
	//no dejamos ejecutar en este caso.
	//Puede ser nihil si hay carril y el carril no tiene registros o está en eof, o somos
	//un access que no está conectado
	if (GetCurrentBlockRef ()->IsNihil ()) {
		ClCompiledMCR * pmcr   = GetpNode_Data ()->GetpChannel_Data ()->GetpChannelDef ()->GetpCMCR ();
		m4uint32_t NodeHandle  =  GetpNode_Data()->GetpNodeDef()->GetHandle();
		
		
		DUMP_CHLOG_ERRORF(M4_CH_DM_RECORDSETEXEBLOCKNIHIL, ai_pMethodId << M4ObjNodeN(pmcr, NodeHandle));
		
		return M4_ERROR;
	}

	// Cogo el indice del item antes de moverme al que tengo que llamar.
	ClItemIndex old_Index = Item.GetIndex();
	
	m4bool_t bisnoteof = Item.MoveTo (ai_pMethodId);
	
	if (!bisnoteof)	{
		ClCompiledMCR * pmcr   = GetpNode_Data ()->GetpChannel_Data ()->GetpChannelDef ()->GetpCMCR ();
		m4uint32_t NodeHandle  =  GetpNode_Data()->GetpNodeDef()->GetHandle();
		
		DUMP_CHLOG_ERRORF(M4_CH_DM_ITEMDONTEXIST2, ai_pMethodId << 
			M4ObjNodeN(pmcr, NodeHandle));
		
		return M4_ERROR;
	}
	
	res = Item.Call( NULL, -1, ai_bFromWrapper, M4_FALSE);
	
	// Devuelvo el indice a su estado.
	Item.MoveTo(old_Index);
	
	if (res == M4_ERROR) {
		return M4_ERROR;
	}
	
	//saco a capón un elemento de la pila:
	m4VariantType v;
	m4return_t res2 = paccess->GetpExecutor()->Stack.Pop (v);   //siempre hacemos pop
	
	if (res2 != M4_ERROR) {
		if ((v.Type == M4CL_M4_TYPE_NULL)||(v.Data.DoubleData == 0.0)||(v.Data.DoubleData == 1.0))
			res = M4_SUCCESS;
		else
			res = M4_ERROR;
	} else {
		ClCompiledMCR * pmcr   = GetpNode_Data ()->GetpChannel_Data ()->GetpChannelDef ()->GetpCMCR ();
		m4uint32_t NodeHandle  =  GetpNode_Data()->GetpNodeDef()->GetHandle();
		
		DUMP_CHLOG_ERRORF (M4_CH_DM_RSEXEPOPERROR, M4ObjNodeN(pmcr, NodeHandle));
		
		return M4_ERROR;
	}
	
	return res;
}

m4return_t _ClRecordSet_Base::FindRegisterByID(m4uint32_t ai_iUniqueRowId,
                                               ClRegisterIndex &ao_iRegIndex)
{
    m4uint32_t auxindex;
    m4return_t res;

    res = GetCurrentBlock()->B_FindByRecordID (auxindex, ai_iUniqueRowId);
    ao_iRegIndex = auxindex;

    return res;
}


///////////////////////////////////////////////////////////////////////////////
//
// ClRecordSet
//

ClRecordSet::~ClRecordSet ()
{
    NotifierDetach ();
}

void ClRecordSet::Attach (ClAccess *ai_paccess)
{
    Item.Attach (ai_paccess);

    Register.Attach (ai_paccess);
}

void ClRecordSet::Attach (ClNode_Data *ai_pClNode_Data)
{
    //soy un independiente, me gestiono el tema del notifier:
    ClNotifier_Channel_Data *pnewn = ai_pClNode_Data->GetpChannel_Data ()->GetpNotifier ();
    if (GetpNotifier () != pnewn) {
        NotifierAttach (pnewn);
    }

    _ClRecordSet_Base::Attach (ai_pClNode_Data);

    // register
    Register.Attach (ai_pClNode_Data);
}


void ClRecordSet::Detach (void)
{
    NotifierDetach ();

    _ClRecordSet_Base::Detach ();

    // register
    Register.Detach ();
}

//----------------------------------------------------------------------------
//  SetCurrentBlock:
//      pública en esta clase
//      valor de retorno: true, si el nuevo bloque es distinto del actual:
//----------------------------------------------------------------------------

void ClRecordSet::_SetCurrentBlock (const ClBlockReference &ai_br)
{
    _ClRecordSet_Base::_SetCurrentBlock (ai_br);

    //Register.SetCurrentBlock (ai_br);
        //cuidado, esta pone el register en EOF
    Register.Item.SetCurrentBlock (ai_br);
}

m4bool_t ClRecordSet::SetCurrentBlock (const ClBlockReference &ai_br)
{
    m4bool_t bismodified = _ClRecordSet_Base::SetCurrentBlock (ai_br);

    if (bismodified) {
        Register.MoveToEOF ();
    }

    return bismodified;
}

void ClRecordSet::FollowSimple (const ClRecordSet &ai_rs)
{
    _ClRecordSet_Base::FollowSimple (ai_rs);

    //y ahora los registros:
    Register.Follow (ai_rs.Register);

        M4_ASSERT (Register.GetCurrentBlock () == ai_rs.Register.GetCurrentBlock ());
        M4_ASSERT (Register.Index == ai_rs.Register.Index);
        M4_ASSERT (Register.GetMMIndex () == ((ClRecordSet &)ai_rs).Register.GetMMIndex ());
}

ClRegTotalize & ClRecordSet::GetFooter(void)
{
    if (!m_pFooter)
    {
        m_pFooter = new ClRegTotalize;
        m_pFooter->Init (&Register);
    }

    M4_ASSERT(m_pFooter);

    return * m_pFooter;
}


//esto es copy-paste del registro ==> apañar

void ClRecordSet::NotifierDetach (void)
{
    //por si acaso estuviera atachado a un notifier ...
    if (GetpNotifier ()) {
        GetpNotifier ()->Detach (this);
    }
}

void ClRecordSet::NotifierAttach (ClNotifier_Channel_Data *ai_pnotifier)
{
    NotifierDetach ();

    //si me indican un notifier, me apunto a él.
    if (ai_pnotifier) {
        ai_pnotifier->Attach (this);
    }
}

ClNotifier_Channel_Data *ClRecordSet::GetpNotifier (void) const
{
    if (GetpNode_Data ()) {
        M4_ASSERT (GetpNode_Data ()->GetpChannel_Data ());
        M4_ASSERT (GetpNode_Data ()->GetpChannel_Data ()->GetpNotifier ());

        return GetpNode_Data ()->GetpChannel_Data ()->GetpNotifier ();
    }
    else {
        return 0;
    }
}

m4return_t _ClRecordSet_Base::GetParentBlock (ClBlockReference &ao_br)
{
	GetCurrentBlockRef ()->GetParentBlock (ao_br);

	if (!(m4bool_t)ao_br)
        return M4_ERROR;

	return M4_SUCCESS;
}

m4return_t _ClRecordSet_Base::GetSize(ClSize &ao_size)
{

	ao_size+=sizeof(_ClRecordSet_Base);
		ao_size-=sizeof(Operation);
		ao_size-=sizeof(FlagPersist);
		ao_size-=sizeof(Item);

    if (Operation.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
    if (FlagPersist.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
    if (Item.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	if (m_pFooter){
		if (m_pFooter->GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}

	return M4_SUCCESS;
}

m4return_t ClRecordSet::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClRecordSet);
		ao_size-=sizeof(_AClClient_Notifier);
		ao_size-=sizeof(_ClRecordSet_Base);
		ao_size-=sizeof(Register);

	if (_AClClient_Notifier::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	if (_ClRecordSet_Base::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	if (Register.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

m4return_t ClFlagPersist_RecordSet::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClFlagPersist_RecordSet);
	return M4_SUCCESS;
}



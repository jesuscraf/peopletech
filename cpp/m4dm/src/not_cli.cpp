//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             not_cli.cpp
// Project:          CVM-DM
// Author:           Meta Software M.S. , S.A
// Date:             10/02/98
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

#include "recorset.hpp"
#include "register.hpp"
#include "aregister.hpp"
#include "regi_smp.hpp"
#include "reg_plf.hpp"
#include "reg_filt.hpp"
#include "clrgstrs.h"
#include "node_knl.hpp"
#include "chan_knl.hpp"

#include "channel.hpp"
#include "acc_dir.hpp"

#include "bnihil.hpp"

#include "compst_common.hpp"

#include "blockref.hpp"
#include "not_cdat.hpp"
#include "node.hpp"
#include "laneman.hpp"

///////////////////////////////////////////////////////////////////////////////
//
// Register
//

void _ClRegister_Base::DestroyRegister_Before (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_mmindex)
{
    if (GetCurrentBlockRef () == ai_br)
    {
        if (MMIndex != ClMMIndex_EOF) {
            if (((m4uint32_t)MMIndex) > ((m4uint32_t)ai_mmindex)) {
                //estoy sobre un registro posterior al que se ha destruido
                //me voy al anterior para seguir apuntando a los mismos datos:
                --MMIndex;
                //se cumple que el MMIndex es el anterior:
            
                //ANTES:
                //Index     Block (MMIndex)
                //  0         A      0   
                //            B      1   <-borramos este
                //            C      2
                //  1         D      3   <-estamos sobre este, vemos 'D'
                //            E      4
                //  2         F      5
            
                //DESPUES:
                //Index     Block (MMIndex)
                //  0         A      0
                //            C      1
                //  1         D      2   <-seguimos sobre el 'D'
                //            E      3
                //  2         F      4

                //OJO, eso siempre que se mantenga en orden en la correspondencias
                //entre Index y MMIndex --> si tenemos un Indice estático en el
                //que hemos aplicado una ordenación esto no se cumpliria necesariamente:

                //-------otro caso:
                //ANTES:
                //Index     Block (MMIndex)
                //            A      0   
                //  0         B      1   <-borramos este
                //            C      2
                //  1         D      3   <-estamos sobre este, vemos 'D'
                //            E      4
                //  2         F      5
            
                //DESPUES:
                //Index     Block (MMIndex)
                //            A      0
                //            C      1
                //  0         D      2   <-seguimos sobre el 'D'
                //            E      3
                //  1         F      4

                //=>si veo el mmindex, decremento Index en 1, sino no.
                if (Filter->IsVisible (ai_mmindex, GetCurrentBlockRef (), this)) {
                    if (Index != M4DM_EOF_INDEX)
                    Index--;
                }
            }   
        }
    }
}

void _ClRegister_Base::DestroyRegister_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_mmindex)
{
    //ahora avisamos a la cadena de filtros:
    //por coherencia hay que llevarlo al notifier y que llame él
    if (ai_pnode->GetpNodeDef()->GetHandle() ==
        m_pClNode_Data->GetpNodeDef()->GetHandle())
        Filter->Notify_DestroyRegister (ai_mmindex, ai_br, this);

    //sólo si estamos sobre el mismo bloque:
    if (GetCurrentBlockRef () == ai_br)
    {
        if (MMIndex != ClMMIndex_EOF) {
            if (MMIndex == ai_mmindex) {
                //estoy sobre el que se ha destruido
                MoveToMandatory (Index);
                //De esta forma, al estar sobre el que se destruye, ahora vamos a apuntar
                //al siguiente, (si lo hay pq si no nos quedamos en eof --> esto es ok).
                //Por otra parte nos evitamos hacer Count y cosas así para saber si es
                //el último, esto es bueno, pq con el tema de carga parcial el count
                //puede ser muy costoso.
                //Y es mandatory para que se haga en cualquier caso, pq el moveto normal
                //comprueba primero que vayamos a un index distinto del que estamos.
            }
            //else, no pasa nada, estoy por encima del que se ha destruido
        }
        //else --> si está en eof, no pasa nada, no hace falta moverle
    }
}

void _ClRegister_Base::AddRegister_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_mmindex)
{
    InsertRegister_After (ai_pnode, ai_br, ai_mmindex);
}

void _ClRegister_Base::InsertRegister_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_mmindex)
{
//sólo si estamos sobre el mismo bloque:
/*    if (GetCurrentBlock () != ai_pblock)
        return;
*/
    if (GetCurrentBlockRef () == ai_br)
    {
        //me insertan un registro, es nuevo y por definición voy a verle (->en realidad
        //se va a ver desde todos los filtros)

        // Index    MMIndex     Data
        //   0         0          A
        //             1          B     <- R2 está aquí (no tiene filtro) (=> Index = 1)
        //   1         2          C     <- R1 y R3 están aquí
        //             3          D     <- R4 está aquí (no tiene filtro) (=> Index = 3)
    
        // R3.Insert ()

        // Index    MMIndex     Data
        //   0         0          A
        //             1          B     <- R2 está aquí (=> Index = 1)
        //   1         2          -     <- R3 está aquí
        //   2         3          C     <- R1 está aquí
        //             4          D     <- R4 está aquí (=> Index = 4)

        //como se ve desde todos los filtros, también R4 incrementa su indice
    
        if (MMIndex != ClMMIndex_EOF)
        {
            if (((m4uint32_t)MMIndex) >= ((m4uint32_t)ai_mmindex)) {
                ++MMIndex;
                if (Index != M4DM_EOF_INDEX) //Por si acaso.
                    Index++;    //¿vale o no vale?
            }
        }
    }

    if (ai_pnode->GetpNodeDef()->GetHandle() ==
        m_pClNode_Data->GetpNodeDef()->GetHandle())
    //ahora avisamos a la cadena de filtros:
        Filter->Notify_InsertRegister (ai_mmindex, ai_br, this);
    //Pues no vale, hay un error en el código, la historia es que R3 no debe
    //incrementarse, sino quedarse donde estaba. Como sería un poco complicado distinguir
    //cuando estoy operando sobre el registro en el que se inserto, esto lo hago en 
    //ClRegister::Insert y ClCurrent::Insert. En Register decremento MMIndex e Index, 
    //deshaciendo el cambio; y en Current hago además un refresh, para que se espabile.
}

void _ClRegister_Base::DestroyAllRegisters_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br)
{
//sólo si estamos sobre el mismo bloque:
    if (GetCurrentBlockRef () != ai_br)
        return;

    MoveToEOF ();
}

void _ClRegister_Base::DestroyBlock_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br)
{
//sólo si estamos sobre el mismo bloque:
    if (GetCurrentBlockRef () != ai_br)
        return;

    DestroyAllRegisters_After (ai_pnode, ai_br);

    if (ai_pnode->GetpNodeDef()->GetHandle() ==
        m_pClNode_Data->GetpNodeDef()->GetHandle())
    {
        Filter->Notify_DestroyBlock_After(ai_br, this);
    }
}

void _ClRegister_Base::Arrange_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, m4uint32_t *ai_pindex_qtc)
{
    m4uint32_t newpos;

//sólo si estamos sobre el mismo bloque:
    if (GetCurrentBlockRef () != ai_br)
        return;

    if (Index != M4DM_EOF_INDEX) {

        //newpos = Index;
        newpos = MMIndex;

        //se trata de averiguar donde ha ido a parar el elemento que nos indican (ai_itarget)
    
        //el algoritmo es muy sencillo: vamos remontando las permutaciones hasta encontrarnos con
        //nosotros mismos
        //
        //  B 1                            A 3
        //  C 2  --despues de ordenar-->   B 1
        //  A 3                            C 2
        //  D 4                            D 4
        //                                   ^--- este es el array que nos pasan (ai_pindex_qtc)
        //
        // queremos saber donde ha ido a parar el elemento 3 (la A)
        // ai_pindex_qtc[3]: 2, vamos al 2
        // ai_pindex_qtc[2]: 1, vamos al 1
        // ai_pindex_qtc[1]: 3, encontrado, ha ido a la posición 1

        //while (Index != ai_pindex_qtc [newpos]) {
        while (MMIndex != ai_pindex_qtc [newpos]) {
            newpos = ai_pindex_qtc [newpos];
        }
        
        MMIndex = newpos;

        ClRegisterIndex auxIndex;
        Filter->UpdateIndex (auxIndex, MMIndex, GetCurrentBlockRef (), this);        
        MoveTo(auxIndex);
    }
    //else --> si está en eof, no pasa nada, no hace falta moverle
}

void _ClRegister_Base::ChannelDataChanged_Before (ClChannel_Data *ai_pClChannel_Data, ClNotifier_Channel_Data *ai_pnewnotifier, ClChannel * ai_poCh)
{
    if (GetpNode_Data ()) {
        ClNodeIndex ni = GetpNode_Data ()->GetpExtInfo ()->Index;
        ClNode_Data *pnd = ai_pClChannel_Data->Node.Get (ni);
        ClNode_Data *pndold = GetpNode_Data ();

        Attach (pnd);   //con todas las consecuencias

        // Tengo que decirle a todos los filtros que borren los indices que se habian
        // creado para los bloques del nodo, ya que ahora los bloques pueden haber cambiado
        Filter->Notify_ReleaseAll_Before (this);

    	if (pndold != pnd)
        {
            ClBlockReference br;
            pnd ->GetpUniqueBlock (br);
            SetCurrentBlock (br);
        }

//        SetCurrentBlock (&g_oNihilBlock);
    }
}
/*
ClChannel * _ClRegister_Base::GetpChannel (void)
{
    ClAccess pAccess;

    pAccess = GetpAccess ();
    if (pAccess)
        return pAccess->GetpChannel();
    else
        return 0;
}
*/
//¿donde he visto yo este código?

void ClRecordSet::ChannelDataChanged_Before (ClChannel_Data *ai_pClChannel_Data, ClNotifier_Channel_Data *ai_pnewnotifier, ClChannel * ai_poCh)
{
    if (GetpNode_Data ()) {
        ClNodeIndex ni = GetpNode_Data ()->GetpExtInfo ()->Index;
        ClNode_Data *pnd = ai_pClChannel_Data->Node.Get (ni);
        ClNode_Data *pndold = GetpNode_Data ();

        Attach (pnd);   //con todas las consecuencias

        // Nuevo, para que los filtros borren los indices que tienen creados.
        Register.Filter->Notify_ReleaseAll_Before (&Register);
//        Current.Filter->Notify_ReleaseAll_Before (&Current);

    	if (pndold != pnd)
        {
            ClBlockReference br;
            pnd ->GetpUniqueBlock (br);
            SetCurrentBlock (br);
        }
    }
}

ClChannel * ClRecordSet::GetpChannel (void)
{
    ClAccess * pAccess;

    pAccess = GetpAccess ();
    if (pAccess)
        return pAccess->GetpChannel();
    else
        return 0;
}

//-----------------------------------------------------------------------------
// pendiente de apañar:

void _ClRegister_Base::Notify_DeleteRegister_After (void)
{
    m_pClNode_Data->_NotifyOperation (M4DM_OPERATION_DELETE);
    if (GetpNode_Data ()->GetpNodeDef()->AffectsDB())
    {
        GetpNode_Data()->_NotifyOperation (M4DM_OPERATION_DELETE_BY_DDBB);
        GetpNode_Data()->GetpChannel_Data()->_NotifyOperation (M4DM_OPERATION_DELETE_BY_DDBB);
    }
}

void _ClRegister_Base::Notify_DeleteAllRegister_After (void)
{
    m_pClNode_Data->_NotifyOperation (M4DM_OPERATION_DELETE);
    if (GetpNode_Data ()->GetpNodeDef()->AffectsDB())
    {
        GetpNode_Data()->_NotifyOperation (M4DM_OPERATION_DELETE_BY_DDBB);
        GetpNode_Data()->GetpChannel_Data()->_NotifyOperation (M4DM_OPERATION_DELETE_BY_DDBB);
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// Filtros
//

//-----------------------------------------------------------------------------
// pendiente de apañar:

void ClRegisterDynFilter::Notify_DestroyRegister (ClMMIndex ai_mmindex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    //se lo paso al siguiente por si le interesa:
    M4_ASSERT (m_pSubFilter);
    m_pSubFilter->Notify_DestroyRegister (ai_mmindex, ai_br, ai_pap);
	//Esto no va a hacer falta luego, solo se hara una vez sin recursion
}


void ClRegisterDynFilter::Notify_InsertRegister (ClMMIndex ai_mmindex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    //se lo paso al siguiente por si le interesa:
    M4_ASSERT (m_pSubFilter);
    m_pSubFilter->Notify_InsertRegister (ai_mmindex, ai_br, ai_pap);
	//Esto no va a hacer falta luego, solo se hara una vez sin recursion
}


void ClRegisterDynFilter::Notify_DestroyBlock_After (const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    //se lo paso al siguiente por si le interesa:
    M4_ASSERT (m_pSubFilter);
    m_pSubFilter->Notify_DestroyBlock_After (ai_br, ai_pap);
}

void ClRegisterDynFilter::Notify_ReleaseAll_Before (ClAccessProvider *ai_pap)
{
    //se lo paso al siguiente por si le interesa:
    M4_ASSERT (m_pSubFilter);
    m_pSubFilter->Notify_ReleaseAll_Before (ai_pap);
}

///////////////////////////////////////////////////////////////////////////////
//
// Channel
//

void ClChannel::DestroyRegister_Before (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index)
{
    m4uint32_t an = Access.Count ();

    ClNode *pna;
    ClAccessIndex ai;

    for (ai = 0; ai < an; ai++) {
        // por el access ...
        pna = Access[ai]->Node.Get (ai_pnode->GetpExtInfo ()->Index);

        pna->RecordSet.Register.DestroyRegister_Before (ai_pnode, ai_br, ai_index);
        pna->RecordSet.Current.DestroyRegister_Before (ai_pnode, ai_br, ai_index);
    }
}

void ClChannel::DestroyRegister_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index)
{
    StDocumentList StList, * iter;

    GetpChannelManager()->LaneManager.GetAllDocuments(ai_pnode, StList);

    if (StList.m_pNode)
    {
        iter = &StList;
        while (iter)
        {
            ClBlockReference br;
            iter->m_pNode->GetpBlockStore()->Get(ai_br->ID.BlockUniqueKey, ai_index, br);
            //ahora puede ser cero:
            if (br) {
                //elimino el bloque:
                br->DestroyProxy (M4_TRUE); // hace un delete de pb
                //hay que hacerlo así para que no pete:
                br.Invalide ();
                //M4_TRUE ==> que elimine el bloque del store			  
            }

			//y avisamos para que se corran (con perdon) para atras
            iter->m_pNode->GetpBlockStore ()->Notify_ParentRemoveRegister (ai_br->ID.BlockUniqueKey, ai_index);
            iter = iter->m_pNext;
        }
    }

    m4uint32_t an = Access.Count ();

    ClNode *pna;
    ClAccessIndex ai;

    for (ai = 0; ai < an; ai++)
    {
        // por el access ...
        pna = Access[ai]->Node.Get (ai_pnode->GetpExtInfo ()->Index);

        pna->RecordSet.Register.DestroyRegister_After (ai_pnode, ai_br, ai_index);
        pna->RecordSet.Current.DestroyRegister_After (ai_pnode, ai_br, ai_index);
    }
}

void ClChannel::AddRegister_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index)
{
    m4uint32_t an = Access.Count ();

    ClNode *pna;
    ClAccessIndex ai;

    for (ai = 0; ai < an; ai++) {
        // por el access ...
        pna = Access[ai]->Node.Get (ai_pnode->GetpExtInfo ()->Index);

        pna->RecordSet.Register.AddRegister_After (ai_pnode, ai_br, ai_index);
        pna->RecordSet.Current.AddRegister_After (ai_pnode, ai_br, ai_index);
    }
}

void ClChannel::InsertRegister_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index)
{
    //el copypaste de siempre (prometo parametrizarlo)
    m4uint32_t an = Access.Count ();

    ClNode *pna;   

    ClAccessIndex ai;

    for (ai = 0; ai < an; ai++) {
        // por el access ...
        pna = Access[ai]->Node.Get (ai_pnode->GetpExtInfo ()->Index);
        M4_ASSERT (pna);

//        if (pna->RecordSet.GetCurrentBlock () == ai_pblock) {
            //sólo si el recordset está sobre el mismo bloque !!!
            pna->RecordSet.Register.InsertRegister_After (ai_pnode, ai_br, ai_index);
            pna->RecordSet.Current.InsertRegister_After (ai_pnode, ai_br, ai_index);
//        }
    }
}

void ClChannel::DestroyAllRegisters_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br)
{
    StDocumentList StList, * iter;

    GetpChannelManager()->LaneManager.GetAllDocuments(ai_pnode, StList);

    if (StList.m_pNode)
    {
        iter = &StList;
        while (iter)
        {
			iter->m_pNode->GetpBlockStore ()->ParentDestroyAllRegisters (ai_br->ID.BlockUniqueKey);
            iter = iter->m_pNext;
        }
    }

    //el copypaste de siempre (prometo parametrizarlo)
    m4uint32_t an = Access.Count ();

    ClNode *pna;   

    ClAccessIndex ai;

    for (ai = 0; ai < an; ai++) {
        // por el access ...
        pna = Access[ai]->Node.Get (ai_pnode->GetpExtInfo ()->Index);
        M4_ASSERT (pna);

        if (pna->RecordSet.GetCurrentBlockRef () == ai_br) {
            //sólo si el recordset está sobre el mismo bloque !!!
            pna->RecordSet.Register.MoveToEOF ();
            pna->RecordSet.Current.MoveToEOF ();
        }
        if (ai_pnode->GetpNodeDef()->GetHandle() == pna->GetpNodeDef()->GetHandle())
        {
            pna->RecordSet.Register.Filter->Notify_DestroyBlock_After (ai_br, &(pna->RecordSet.Register));
            pna->RecordSet.Current.Filter->Notify_DestroyBlock_After (ai_br, &(pna->RecordSet.Current));
        }
    }
}

void ClChannel::DestroyBlock_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br)
{
    DestroyAllRegisters_After (ai_pnode, ai_br);
}

void ClChannel::Arrange_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, m4uint32_t *ai_pindex_qtc)
{
    //el copypaste de siempre (prometo parametrizarlo)
    m4uint32_t an = Access.Count ();

    ClNode *pna;   

    ClAccessIndex ai;

    for (ai = 0; ai < an; ai++) {
        // por el access ...
        pna = Access[ai]->Node.Get (ai_pnode->GetpExtInfo ()->Index);
        M4_ASSERT (pna);

        if (pna->RecordSet.GetCurrentBlockRef () == ai_br) {
            //sólo si el recordset está sobre el mismo bloque !!!
            pna->RecordSet.Register.Arrange_After (ai_pnode, ai_br, ai_pindex_qtc);
            pna->RecordSet.Current.Arrange_After (ai_pnode, ai_br, ai_pindex_qtc);
        }
    }
}

void ClChannel::ChannelDataChanged_Before (ClChannel_Data *ai_pClChannel_Data, ClNotifier_Channel_Data *ai_pnewnotifier, ClChannel * ai_poCh)
{
    //el copypaste de siempre (prometo parametrizarlo)
    m4uint32_t      an = Access.Count ();
    ClAccessIndex   ai;
    ClAccess *      paccess;

    for (ai = 0; ai < an; ai++) {
        // por el access ...
        paccess = Access[ai];

        //1
        //Reseteamos el access:
        //antes de hacer el attach, (si está encarrilado hay problemas)
        paccess->Reset ();

        //2
        //asociamos cada nodo con el suyo correspondiente del nuevo channel_data
        paccess->Node.Attach (ai_pClChannel_Data->GetpChannelDef (), &ai_pClChannel_Data->Node);

        //3
        //para en caso en que un canal cacheado está encarrilado:
        //(si no se hace esto, los recordset de los nodos raices se quedan
        //apuntando a bloques que se van a borrar después, en el releaseall del canal:
        paccess->Reset ();
    }

    //me elimino del notifier antiguo:
    M4_ASSERT (GetpChannel_Data ());
    M4_ASSERT (GetpChannel_Data ()->GetpNotifier ());
    GetpChannel_Data ()->GetpNotifier ()->Detach (this);

    //y me apunto al nuevo:
    ai_pnewnotifier->Attach (this);
}

ClChannel * ClChannel::GetpChannel (void)
{
    return this;
}

void ClChannel::DestroyProxyBlock_Before (ClNode_Data *ai_pnode, const ClBlockReference &ai_br)
{
    //el copypaste de siempre (prometo parametrizarlo)
    m4uint32_t an = Access.Count ();

    ClNode *pna;   

    ClAccessIndex ai;

    for (ai = 0; ai < an; ai++) {
        // por el access ...
        // por el access ...
        pna = Access[ai]->Node.Get (ai_pnode->GetpExtInfo ()->Index);
        M4_ASSERT (pna);

        pna->RecordSet.DestroyProxyBlock_Before (ai_pnode, ai_br);
    }
}

void _ClRegister_Base::DestroyProxyBlock_Before (ClNode_Data *ai_pnode, const ClBlockReference &ai_br)
{
    //un destroy de un proxy tiene que venir por un destroy_register en un nodo padre
    //o también por un encarrilado
    if (GetCurrentBlockRef () == ai_br) {
        //me asigno el bloque nihil
        //((ClBlockReference &)GetCurrentBlockRef ()).Invalide ();
        SetCurrentBlock (&g_oNihilBlock);
    }
}

void ClRecordSet::DestroyProxyBlock_Before (ClNode_Data *ai_pnode, const ClBlockReference &ai_br)
{
    //un destroy de un proxy tiene que venir por un destroy_register en un nodo padre
    //o también por un encarrilado
    if (GetCurrentBlockRef () == ai_br) {
        //lo mismo, me asigno el bloque nihil
        SetCurrentBlock (&g_oNihilBlock);
    }
}

void ClChannel::ReleaseAll_Before (void)
{
    //llamo a reset en todos los access:
    //el copypaste de siempre (prometo parametrizarlo)
    m4uint32_t an = Access.Count ();

    ClAccessIndex ai;

    ClNodeIndex i = 0;
    ClNode *pnode;

    for (ai = 0; ai < an; ai++)
    {
        Access[ai]->Reset ();
        // por el access ...
        m4uint16_t nnodes;
        nnodes = Access[ai]->Node.Count ();

        for (i = 0; i < nnodes; i++)
        {
            pnode = Access[ai]->Node.Get (i);
            M4_ASSERT (pnode);        
        
            pnode->RecordSet.Register.Filter->Notify_ReleaseAll_Before(&pnode->RecordSet.Register);
            pnode->RecordSet.Current.Filter->Notify_ReleaseAll_Before(&pnode->RecordSet.Current);
        }

    }
}

void _ClRegister_Base::ReleaseAll_Before (void)
{
    if (GetpNode_Data ()) {
        //ahora son los mono los que no se borran !:
        //if (!GetpNode_Data ()->GetpExtInfo ()->IsFree ()) {
        if (GetpNode_Data ()->GetpBlockStore ()->GetType () != _AClComponentStore::MONO) {
            SetCurrentBlock (&g_oNihilBlock);
        }
        else {
            MoveToEOF ();
        }
    }

    Filter->Notify_ReleaseAll_Before(this);

    M4_ASSERT (Index == M4DM_EOF_INDEX);
}

void ClRecordSet::ReleaseAll_Before (void)
{
    if (GetpNode_Data ()) {
        //ahora son los mono los que no se borran !:
        //if (!GetpNode_Data ()->GetpExtInfo ()->IsFree ()) {
        if (GetpNode_Data ()->GetpBlockStore ()->GetType () != _AClComponentStore::MONO) {
            SetCurrentBlock (&g_oNihilBlock);
        }
        else {
            Register.MoveToEOF ();
        }
    }

    M4_ASSERT (Register.Index == M4DM_EOF_INDEX);
}


void ClChannel::DestroyChannelData_Before (void)
{
    //nunca deberíamos estar aquí, destruyendo el channel_data y con channels todavía vivos !!
    M4_ASSERT (0);    
}

void ClChannel::DestroyChannel_Before (ClChannel * ai_poCh)
{
    if (this == ai_poCh)
        //me borro del notifier del channel_data:
        m_pClChannel_Data->GetpNotifier ()->Detach (this);
}

void _ClRegister_Base::DestroyChannel_Before (ClChannel * ai_poCh)
{
    Filter.DestroyChannel_Before (ai_poCh);
}

void _ClRegister_Base::DestroyChannelData_Before (void)
{
    Detach ();
}

void ClRecordSet::DestroyChannel_Before (ClChannel * ai_poCh)
{
    Register.DestroyChannel_Before (ai_poCh);
}

void ClRecordSet::DestroyChannelData_Before (void)
{
    Detach ();    
}

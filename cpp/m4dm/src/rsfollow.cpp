//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             recorset.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             06/09/97
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

//----------------------------------------------------------------------------
#include "dm_def.hpp"
#include "m4define.hpp"
//----------------------------------------------------------------------------

#include "recorset.hpp"
#include "px_block.hpp"

#include "access.hpp"
#include "node_knl.hpp"

#include "regi_smp.hpp"

#include "blockref.hpp"

m4return_t ClAccessRecordSet::Follow (const _ClRecordSet_Base &ai_recordset)
{
    return Follow (ai_recordset.GetCurrentBlock ());
}


m4return_t ClAccessRecordSet::Follow (ClBlock_Channel_Proxy *ai_pb)
{
	ClBlockReference br;
	br.Set (ai_pb);
	return Follow (br);
}

m4return_t ClAccessRecordSet::Follow (const ClBlockReference &ai_br)
{
    ClAccess *paccess;

    paccess = Current.GetpAccess ();    
    
    // pedazo de código
    M4_ASSERT (m_pClNode_Data);
    ClNode_Connector *pncon = m_pClNode_Data->GetpExtInfo ()->Parent.GetpNotFree ();    

    ClNode_Data *pnodeparent_kernel;

    m4bool_t bok = M4_FALSE;
    
    if (pncon) {
        pnodeparent_kernel = pncon->GetAddress ();
        M4_ASSERT (pnodeparent_kernel);

        //está encarrilado ?
        bok = M4_BOOL(!m_pClNode_Data->IsLaned ());  //no debe estarlo para seguir
    }

    if (bok) {
        ClNode *paccessnodeparent = paccess->Node.Get (pnodeparent_kernel->GetpExtInfo ()->Index);
		M4_ASSERT (paccessnodeparent);

        //cuidado a ver si me salgo del canal, el follow sólo se hace dentro de un
        //mismo canal, de momento:
        //M4_ASSERT (GetpNode_Data ()->GetpChannel_Data () == paccessnodeparent->GetpNode_Data ()->GetpChannel_Data ());

        //bueno, pues allí vamos...
        if (ai_br) {
            if (! ai_br->IsNihil ()) {
                // tengo padre, sigo iterando
                // comprobaciones
                ClNode_Data *pnn;
                pnn = ai_br->GetpNode_Data ();

                if (GetpNodeDef ()->GetHandle () != pnn->GetpExtInfo ()->GetHandle ()) {
                    // los recordset pasados no son del mismo tipo de nodo !
                    return M4_ERROR;
                }

				ClBlockReference br;
                ai_br->GetParentBlock (br);
                m4uint32_t indexparent = ai_br->ID.RegisterId;
                // itero recursivamente
                return paccessnodeparent->RecordSet._follow_s1 (br, indexparent);
            }
        }

		ClBlockReference br2;
		br2.Reset ();
        return  paccessnodeparent->RecordSet._follow_s1 (br2, M4DM_EOF_INDEX);
    }
    else {
        //comprobación por el carril, podemos estar en un root:
        if (GetCurrentBlockRef () != ai_br)	//operator != sobrecargado
            SetCurrentBlock (ai_br);

        return M4_SUCCESS;
    }
}

m4return_t ClAccessRecordSet::_follow_s1 (const ClBlockReference &ai_br, m4uint32_t ai_index)
{
    m4return_t res = M4_SUCCESS;

    if (ai_br) {
        res = Follow (ai_br);
    }

    if (res == M4_SUCCESS) {
        // Calculo, a partir del MMIndex, unico dato que conozco, el Index
        // solo entonces puedo hacer el moveto.
		if (ai_index != Current.MMIndex) {
			ClRegisterIndex aux_index;
			Current.Filter->UpdateIndex(aux_index, ai_index, Current.GetCurrentBlockRef (), &Current);
			Current.MoveTo(aux_index);
		}
    }

    return res;
}

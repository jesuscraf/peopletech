#include "dmres.hpp"

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             aregwrite.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             14/05/98
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

#include "aregwrite.hpp"
#include "aregister.hpp"
#include "px_block.hpp"
#include "filter_i.hpp"
#include "regi_smp.hpp"

#include "nodedef.hpp"
#include "register.hpp"

#include "node_knl.hpp"
#include "chan_knl.hpp"
#include "sizer.hpp"
#include "chlog.hpp"

#include "blockref.hpp"
#include "not_cdat.hpp"
#include "node_x.hpp"
#include "compst_common.hpp"
#include "access.hpp"
#include "m4mdrt.hpp"

#include "laneinfo.hpp"

m4return_t ClRegister_Insert::Add (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone)
{
    m4return_t res;

    m4uint32_t n;
    
    if ( ! ai_pClReg->Filter->IsTerminator ()) {
        n = ai_pClReg->Count();
    }

    ClMMIndex newindex = 0; //chapucilla

    // Aquí dentro se chequea lo del autoload
    res = ai_pClReg->GetCurrentBlock ()->B_AddRegister (newindex);

    if (res == M4_SUCCESS) {
        //aviso al nodo y al canal:
        ai_pClReg->GetpNode_Data ()->_NotifyOperation (M4DM_OPERATION_INSERT);
        ai_pClReg->GetpNode_Data ()->GetpChannel_Data ()->_NotifyOperation (M4DM_OPERATION_INSERT);
        if (ai_pClReg->GetpNode_Data ()->GetpNodeDef()->AffectsDB())
        {
            ai_pClReg->GetpNode_Data()->_NotifyOperation (M4DM_OPERATION_INSERT_BY_DDBB);
            ai_pClReg->GetpNode_Data()->GetpChannel_Data()->_NotifyOperation (M4DM_OPERATION_INSERT_BY_DDBB);
        }

        //actualizo los indices:
		ai_pClReg->MMIndex = (m4uint32_t)newindex;

        if ( ! ai_pClReg->Filter->IsTerminator ())
            ai_pClReg->Index = n; //me coloco en la siguiente posicion visible para mi            
        else
            ai_pClReg->Index = (m4uint32_t)newindex;            
    }

    return res;
}
m4return_t ClRegister_Insert::Insert (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone)
{
    m4return_t res;

    if (ai_pClReg->Index == M4DM_EOF_INDEX) {   // si estoy en eof, llamo al add
         return Add (ai_pClReg, 0);
    }

    // Aquí dentro se chequea lo del autoload
    res = ai_pClReg->GetCurrentBlock ()->R_InsertRegister (ai_pClReg->GetMMIndex ());

    //que se actualizen los parentindex de los bloques hijos RB del carril
	//avisamos al store:
	m4uint16_t i;
    if (ai_pClReg->GetpNode())
    {
        if (ai_pClReg->GetpNode()->GetpLane_Info())
        {
	        m4uint16_t n = ai_pClReg->GetpNode()->GetpLane_Info()->ChildNodeRB.Count ();

	        for (i = 0; i < n; i++) {
		        ClNode *pchildnode = ai_pClReg->GetpNode()->GetpLane_Info()->ChildNodeRB[(ClChildIndex)i].GetAddress ();
		        M4_ASSERT (pchildnode);

		        pchildnode->GetpNode_Data()->GetpBlockStore ()->Notify_ParentAddRegister (ai_pClReg->GetCurrentBlock()->ID.BlockUniqueKey, ai_pClReg->GetMMIndex ());
	        }
        }
    }

    if (res != M4_ERROR) {
        //aviso al nodo y al canal:    
        ai_pClReg->GetpNode_Data ()->_NotifyOperation (M4DM_OPERATION_INSERT);
        ai_pClReg->GetpNode_Data ()->GetpChannel_Data ()->_NotifyOperation (M4DM_OPERATION_INSERT);
        if (ai_pClReg->GetpNode_Data ()->GetpNodeDef()->AffectsDB())
        {
            ai_pClReg->GetpNode_Data()->_NotifyOperation (M4DM_OPERATION_INSERT_BY_DDBB);
            ai_pClReg->GetpNode_Data()->GetpChannel_Data()->_NotifyOperation (M4DM_OPERATION_INSERT_BY_DDBB);
        }

        //esto viene a cuento del comentario en _ClRegister_Base::Notify_InsertRegister_After
        --(ai_pClReg->MMIndex);
        (ai_pClReg->Index)--;
    }

    return res;
}

m4return_t ClRegister_Delete::_DeleteCascade (_ClRegister_Base * ai_pClReg)
{
	m4uint32_t hConn;
	m4uint16_t numCon = ai_pClReg->GetpNodeDef ()->GetNodeNumberOfRBConnectors ();
	m4uint16_t fisrtCon = ai_pClReg->GetpNodeDef ()->GetNodeRBFirstConnector ();
	
	ClNodeDef *pndef = ai_pClReg->GetpNodeDef ();
	
	for (m4uint16_t i = fisrtCon; i < numCon; i++)
	{
		//Recorremos los conectores RB
		hConn = pndef->GetNodeConnectorHandleByPosition ( i );

		if ( pndef->GetNodeConnectorExpandDelete ( hConn ) )
		{
			//hay que borrar los registros del bloque
			//ClRecordSet *poRecordSet;				
			ClRegisterIndex j(0);

			m4uint32_t hSonNode = pndef->GetNodeConnectorSonNodeHandle ( hConn );

			ClBlock_Channel_Proxy * pBlock = ai_pClReg->GetChildBlock ( (ClHandle)hSonNode );

			if ( pBlock->IsValid () )    // testear
			{
				ClRegister reg (0); //sin autoload
				reg.SetCurrentBlock (pBlock);

				//Recorre los registros
				reg.Begin ();
				while (reg.Index != M4DM_EOF_INDEX) {
					reg.Delete();
					reg.Next ();
				}
			}
		}
	}		
	
	return M4_SUCCESS;
}

m4return_t ClRegister_Delete::Delete (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone)
{
    m4return_t res;

    if (ai_pClReg->Index != M4DM_EOF_INDEX) {
        
        res = _DeleteCascade (ai_pClReg);
        if (M4_ERROR == res)
            return res;

        res = ai_pClReg->GetCurrentBlock ()->R_DeleteRegister (ai_pClReg->GetMMIndex ());

        if (res != M4_ERROR) {
            ai_pClReg->Notify_DeleteRegister_After ();
        }
    }
    else
    {
        m4uint32_t NodeHandle = ai_pClReg->GetpNode_Data()->GetpNodeDef()->GetHandle();
        ClCompiledMCR * pmcr  = ai_pClReg->GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
        DUMP_CHLOG_WARNINGF(M4_CH_DM_REGISTERISEOF, M4ObjNodeN(pmcr, NodeHandle));

        res = M4_ERROR;
    }

    // aquí la actualización del índice a un nuevo lugar, si se quiere

    return res;
}

m4return_t ClRegister_Delete::DeleteAll (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone)
{
    M4_ASSERT (ai_pClReg->GetCurrentBlock ());

    m4return_t res;

	//Se marcan para borrar los registros de los bloques de nodos hijos
	ClBlock_Channel_Proxy * pBlock = ai_pClReg->GetCurrentBlock ();
	if ( pBlock->IsValid () )    // testear
	{
		ClRegister reg (0); //sin autoload
		reg.SetCurrentBlock (pBlock);

		//Recorre los registros
		res = M4_SUCCESS;
		reg.Begin ();
		while ((reg.Index != M4DM_EOF_INDEX) && (res == M4_SUCCESS)) {
			res = _DeleteCascade(&reg);
			reg.Next ();
		}
		if(res == M4_ERROR)
			return M4_ERROR;
	}


    res = ai_pClReg->GetCurrentBlock ()->DeleteAll ();

    if (res != M4_ERROR) {
        ai_pClReg->Notify_DeleteAllRegister_After ();
    }

    // hay que poner en eof todos los registros asociados al recordset

    return res;
}

m4return_t ClRegister_Delete::Destroy (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone)
{
    M4_ASSERT (ai_pClReg->GetCurrentBlock ());

    m4return_t res;

    if (ai_pClReg->Index != M4DM_EOF_INDEX) {

        //que se actualizen los parentindex de los bloques hijos RB
        //_UpdateParentBlockIndex (-1);   //para atrás
        //Ya no hace falta, el store de bloques se mantiene 'automáticamente'

        //destruimos el registro:
        res = ai_pClReg->GetCurrentBlock ()->R_Destroy (ai_pClReg->GetMMIndex ());
        if (res == M4_ERROR)
            return res;

		ClBlockReference br;
		br.Set (ai_pClReg->GetCurrentBlock());
        // Se destruyen los bloques hijos (RB):
//        ai_pClReg->DestroyRBChilds(ai_pClReg->GetMMIndex ());
    }
    else {
        m4uint32_t NodeHandle = ai_pClReg->GetpNode_Data()->GetpNodeDef()->GetHandle();
        ClCompiledMCR * pmcr  = ai_pClReg->GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
        DUMP_CHLOG_WARNINGF(M4_CH_DM_REGISTERISEOF, M4ObjNodeN(pmcr, NodeHandle));

        res = M4_ERROR;
    }

    return res;
}
m4return_t ClRegister_Delete::DestroyAll (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone)
{
    M4_ASSERT (ai_pClReg->GetCurrentBlock ());

    m4return_t res;

    res = ai_pClReg->GetCurrentBlock ()->DestroyAllRegisters ();

    return res;
}



//*************************************************************************
//*************************************************************************
//*************************************************************************


m4return_t   ClRegister_DeleteNihil::Delete (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone)
{
    m4uint32_t NodeHandle = ai_pClReg->GetpNode_Data()->GetpNodeDef()->GetHandle();
    ClCompiledMCR * pmcr  = ai_pClReg->GetpNode_Data()->GetpNodeDef()->GetpCMCR ();

    if (ai_pClReg->GetpNodeDef ()->NumRows())
    {
        DUMP_CHLOG_ERRORF(M4_CH_DM_PARTIAL_LOAD_INVALID_OP, M4ObjNodeN(pmcr,
            NodeHandle));

        return M4_WARNING;
    }
    else
    {
        if (ao_pDone)
            * ao_pDone = M4_ERROR;

        DUMP_CHLOG_WARNINGF(M4_CH_DM_SECURITY_NODE_DELETE, M4ObjNodeN(pmcr,
            NodeHandle));

        return M4_SUCCESS;
    }
}

m4return_t   ClRegister_DeleteNihil::Destroy (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone)
{
    m4uint32_t NodeHandle = ai_pClReg->GetpNode_Data()->GetpNodeDef()->GetHandle();
    ClCompiledMCR * pmcr  = ai_pClReg->GetpNode_Data()->GetpNodeDef()->GetpCMCR ();

    if (ai_pClReg->GetpNodeDef ()->NumRows())
    {
        DUMP_CHLOG_ERRORF(M4_CH_DM_PARTIAL_LOAD_INVALID_OP, M4ObjNodeN(pmcr,
            NodeHandle));

        return M4_WARNING;
    }
    else
    {
        if (ao_pDone)
            * ao_pDone = M4_ERROR;

        DUMP_CHLOG_WARNINGF(M4_CH_DM_SECURITY_NODE_DELETE, M4ObjNodeN(pmcr,
            NodeHandle));

        return M4_SUCCESS;
    }
}

m4return_t   ClRegister_DeleteNihil::DeleteAll (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone)
{
    m4uint32_t NodeHandle = ai_pClReg->GetpNode_Data()->GetpNodeDef()->GetHandle();
    ClCompiledMCR * pmcr  = ai_pClReg->GetpNode_Data()->GetpNodeDef()->GetpCMCR ();

    if (ai_pClReg->GetpNodeDef ()->NumRows())
    {
        DUMP_CHLOG_ERRORF(M4_CH_DM_PARTIAL_LOAD_INVALID_OP, M4ObjNodeN(pmcr,
            NodeHandle));

        return M4_WARNING;
    }
    else
    {
        if (ao_pDone)
            * ao_pDone = M4_ERROR;

        DUMP_CHLOG_WARNINGF(M4_CH_DM_SECURITY_NODE_DELETE, M4ObjNodeN(pmcr,
            NodeHandle));

        return M4_SUCCESS;
    }
}

m4return_t   ClRegister_DeleteNihil::DestroyAll (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone)
{
    m4uint32_t NodeHandle = ai_pClReg->GetpNode_Data()->GetpNodeDef()->GetHandle();
    ClCompiledMCR * pmcr  = ai_pClReg->GetpNode_Data()->GetpNodeDef()->GetpCMCR ();

    if (ai_pClReg->GetpNodeDef ()->NumRows())
    {
        DUMP_CHLOG_ERRORF(M4_CH_DM_PARTIAL_LOAD_INVALID_OP, M4ObjNodeN(pmcr,
            NodeHandle));

        return M4_WARNING;
    }
    else
    {
        if (ao_pDone)
            * ao_pDone = M4_ERROR;

        DUMP_CHLOG_WARNINGF(M4_CH_DM_SECURITY_NODE_DELETE, M4ObjNodeN(pmcr,
            NodeHandle));

        return M4_SUCCESS;
    }
}

m4return_t   ClRegister_InsertNihil::Add (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone)
{
    m4uint32_t NodeHandle = ai_pClReg->GetpNode_Data()->GetpNodeDef()->GetHandle();
    ClCompiledMCR * pmcr  = ai_pClReg->GetpNode_Data()->GetpNodeDef()->GetpCMCR ();

    if (ai_pClReg->GetpNodeDef ()->NumRows())
    {
        DUMP_CHLOG_ERRORF(M4_CH_DM_PARTIAL_LOAD_INVALID_OP, M4ObjNodeN(pmcr,
            NodeHandle));
        return M4_WARNING;
    }
    else
    {
        if (ao_pDone)
            * ao_pDone = M4_ERROR;

        DUMP_CHLOG_WARNINGF(M4_CH_DM_SECURITY_NODE_WRITE, M4ObjNodeN(pmcr,
            NodeHandle));
        return M4_SUCCESS;
    }
}

m4return_t   ClRegister_InsertNihil::Insert (_ClRegister_Base * ai_pClReg, m4return_t * ao_pDone)
{
    m4uint32_t NodeHandle = ai_pClReg->GetpNode_Data()->GetpNodeDef()->GetHandle();
    ClCompiledMCR * pmcr  = ai_pClReg->GetpNode_Data()->GetpNodeDef()->GetpCMCR ();

    if (ai_pClReg->GetpNodeDef ()->NumRows())
    {
        DUMP_CHLOG_ERRORF(M4_CH_DM_PARTIAL_LOAD_INVALID_OP, M4ObjNodeN(pmcr,
            NodeHandle));

        return M4_WARNING;
    }
    else
    {
        if (ao_pDone)
            * ao_pDone = M4_ERROR;

        DUMP_CHLOG_WARNINGF(M4_CH_DM_SECURITY_NODE_WRITE, M4ObjNodeN(pmcr,
            NodeHandle));

        return M4_SUCCESS;
    }
}

m4return_t	 
_ClRegister_Insert::GetSize(ClSize &ao_size)
{
	ao_size += sizeof(_ClRegister_Insert);
	return M4_SUCCESS;
}

m4return_t	 
ClRegister_Insert::GetSize(ClSize &ao_size)
{
	ao_size += sizeof(ClRegister_Insert);
		ao_size -= sizeof(_ClRegister_Insert);

	if (_ClRegister_Insert::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	return M4_SUCCESS;
}

m4return_t	 
ClRegister_InsertNihil::GetSize(ClSize &ao_size)
{
	ao_size += sizeof(ClRegister_InsertNihil);
		ao_size -= sizeof(_ClRegister_Insert);

	if (_ClRegister_Insert::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	return M4_SUCCESS;
}

m4return_t	 
_ClRegister_Delete::GetSize(ClSize &ao_size)
{
	ao_size += sizeof(_ClRegister_Delete);
	return M4_SUCCESS;
}

m4return_t	 
ClRegister_Delete::GetSize(ClSize &ao_size)
{
	ao_size += sizeof(ClRegister_Delete);
		ao_size -= sizeof(_ClRegister_Delete);

	if (_ClRegister_Delete::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	return M4_SUCCESS;
}

m4return_t	 
ClRegister_DeleteNihil::GetSize(ClSize &ao_size)
{
	ao_size += sizeof(ClRegister_DeleteNihil);
		ao_size -= sizeof(_ClRegister_Delete);

	if (_ClRegister_Delete::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	return M4_SUCCESS;
}




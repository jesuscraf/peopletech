//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             ch_block.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             01/09/97
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
//----------------------------------------------------------------------------

#include "px_block.hpp"
#include "bnihil.hpp"
#include "node_knl.hpp"
#include "access.hpp"
#include "chan_knl.hpp"

#ifndef M4_SIZEOF_RBCONNECTOR
#include "pxbsize.hpp"          // para el assert que he colocado en el contructor del proxy
#endif

#include "checkmem.hpp"
#include "itemconn.hpp"
#include "compst_common.hpp"
#include "compstit_common.hpp"

#include "itemdfit.hpp"
#include "conitinf.hpp"

#include "not_cdat.hpp"

#include "itemsimp.hpp"

#include "prxyfact.hpp"

#include "ch_block.hpp"
#include "regpipe.hpp"
#include "m4mdrt.hpp"

//## end module.includes preserve=yes


ClBlock_Nihil			M4_DECL_M4DM	g_oNihilBlock;
ClBlock_Nihil_Memory	M4_DECL_M4DM	g_oNihilBlock_Memory;


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------


// CONSTRUCTOR POR DEFECTO PARA CARLOS:
ClBlock_Channel_Proxy::ClBlock_Channel_Proxy () : ID (0, 0, 0, 0)
{
    #ifdef _DEBUG
        g_StMemInfo.iBlock_Proxy++;
    #endif

    m_iNeedSerial = M4_PXB_NO_SEND;
    m_bBranchMode = M4_FALSE;

    m_bBlockNotSentBecauseMarks = M4_FALSE;

    m_iRefCount = 0;

	m_bConnectedChanged = M4_FALSE ;
}

ClBlock_Channel_Proxy::~ClBlock_Channel_Proxy ()
{
    #ifdef _DEBUG
        g_StMemInfo.iBlock_Proxy--;
    #endif

    ResetProxy ();

    //nuevo, llamo a la factoria y le digo que me libere:

    ////deshabilitado de momento
    if (GetpNode_Data ()) {  //para que no pete al destruir el nihil !!
                            //la corrección buena es montar bien las clases
                            //nihil y proxy de forma que no derive una de otra
        GetpNode_Data ()->GetpChannel_Data ()->m_pProxyFactory->Reuse (this);
    }
}

//-----------------------------------------------------------------------------
// ResetProxy
//-----------------------------------------------------------------------------

m4return_t ClBlock_Channel_Proxy::ResetProxy (void)
{
    //antes de nada:
    _SetIsLoaded (M4_FALSE);

	//marco el bloque como 'NO ordenado':
	ClBlockSearchInfo::SetNotSorted ();

    if (m_pClBlock) {
        delete m_pClBlock;
        m_pClBlock = 0;

		SetChange();
    }

    return M4_SUCCESS;
}

//----------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------

m4bool_t ClBlock_Channel_Proxy::ExistParentBlock(void) const
{
   return ( m_pClNode_Data->GetpExtInfo()->Parent.GetpNotFree() )? M4_TRUE : M4_FALSE;
}

void ClBlock_Channel_Proxy::GetParentBlock (ClBlockReference &ao_br)
{
    //tenemos un padre no-free (rb o bb)
    if (ExistParentBlock() != M4_TRUE)
    {
        ao_br.Reset();
        return;
    }

    //pillamos el nodo padre:
    ClNode_Data *pparentnode = m_pClNode_Data->GetpExtInfo ()->Parent.GetpNotFree ()->GetAddress ();
    if (pparentnode == 0)
    {
        ao_br.Reset();
        return;
    }

    //ClBlock_Channel_Proxy *pparentblock;
	//no quiero que me cree el bloque si no existe:
    pparentnode->GetpBlockStore ()->Get (ID.Parent_BlockId, ID.Parent_RegisterIndex, ao_br);
}

ClBlock_Channel *ClBlock_Channel_Proxy::_CreateBlock (void)
{
    M4_ASSERT (m_pClNode_Data);
    M4_ASSERT (m_pClBlock == 0);    // si han llamado a CreateBlock, es porque el bloque no estaba creado

    m_pClBlock = m_pClNode_Data->CreateBlock (/*this*/);
    M4_ASSERT (m_pClBlock);

    m4bool_t bcp = m_pClBlock->GetCheckPointMode();
    if (bcp != m_bCheckPointMode) {
        m_pClBlock->SetCheckPointMode (m_bCheckPointMode);
    }

    if (m_pClNode_Data->GetpExtInfo ()->IsFree ()) {
        // si soy un 'free', se lo pido al canal, por si ya lo tiene
        //soy el proxy de un nodo free

        //... pero como el bloque ya le he creado, pues nada más
        M4_ASSERT (m_pClBlock);
    }
    else {
        _UpdateConnectedItems ();
    }

    return m_pClBlock;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

void ClBlock_Channel_Proxy::_UpdateConnectedItems (void/*ClItem &ai_fatheritem*/)
{
    m4return_t res; // -> lo utilizo para la seguridad
    ClItem item(0);
    ClItem itemparent(0);


	m_bConnectedChanged = M4_FALSE ;

    //item.Attach ();
    item.SetCurrentBlock (this);
    
    item.SetMode (M4DM_SCOPE_BLOCK, M4DM_TYPE_PROPERTY | M4DM_TYPE_FIELD | M4DM_TYPE_CONCEPT);   //sólo los de bloque y no método !!
        //no cambiar el scope ni el tipo, hay una dependencia en ClItemProxy_Array::GetpRTII

    item.Begin ();

    m4VariantType var;

    while (item.GetIndex() != M4DM_EOF_INDEX)
    {
        if (item.GetpConnectItemInfo ()->IsConnected ())
        {
            //está conectado:

			/* Bug 0114798
			Hay que pasar el error hacia arriba
			*/
			res = item.InitAsMyParent( itemparent ) ;

			if( res == M4_SUCCESS )
			{
				itemparent.Value.Get (var, &res);
				// En res, se guarda si se ha podido hacer o no el get por razones de
				// seguridad.
				if (res != M4_SUCCESS)
				{
					m4uint32_t ItemHandle = itemparent.ItemDef.GetHandle();
					ClCompiledMCR * pmcr  = itemparent.ItemDef.GetpCMCR();
					DUMP_CHLOG_WARNINGF(M4_CH_DM_SEC_GET_IN_PARENT, M4ObjNodeItemI(pmcr, ItemHandle));
				}

				item.Value._SetNoNotify (var, M4_FALSE);
			}
        }

        item.Next ();
    }

    /*
	Bug 0084826
	Ahora esto sobra porque no se modifica el estado
	m_pClBlock->Block.CheckPoint();
    m_pClBlock->Records.CheckPoint();
	*/
}

//----------------------------------------------------------------------------

void ClBlock_Channel_Proxy::_SetIsLoaded (m4bool_t ai_bloaded)
{
    m_bLoaded = ai_bloaded;
}

m4bool_t ClBlock_Channel_Proxy::IsLoaded (void) const
{
    return m_bLoaded;
}

//----------------------------------------------------------------------------
// Count
//----------------------------------------------------------------------------

size_t ClBlock_Channel_Proxy::Count (void)
{
    //Autoload eliminada !!!
    if (m_pClBlock) {
        return m_pClBlock->Count ();
    }
    else {
        // por definicion no tiene registros
        return 0;
    }
}

//----------------------------------------------------------------------------
// DeleteAll
//      operación de escritura
//		marca todos los registros como borrados (repito: marca)
//----------------------------------------------------------------------------

m4return_t ClBlock_Channel_Proxy::DeleteAll (void)
{
    if (m_pClBlock) {
        return m_pClBlock->DeleteAll ();
    }

    // por definicion no tiene registros
    return M4_SUCCESS;
}

//----------------------------------------------------------------------------
// AddRegister
//      operación de escritura
//		añade un nuevo registro al final del bloque
//----------------------------------------------------------------------------

m4return_t ClBlock_Channel_Proxy::B_AddRegister (ClMMIndex &ao_newindex)
{
    m4return_t res;
    m4uint32_t iUniqueRowId;

    if (0 == m_pClBlock) {
        if (!_CreateBlock()) return M4_ERROR;
        M4_ASSERT (m_pClBlock);
    }    

// Nuevo identificador unico de registro
    iUniqueRowId = GetpNode_Data()->GetpChannel_Data()->GetUniqueRowId();

    res = m_pClBlock->B_AddRegister (ao_newindex, iUniqueRowId);

    if (res == M4_SUCCESS) {
		//añadimos inmediatamente los bloques proxy correspondientes a este registro
		//en los nodos hijos RB:
		_R_InitChildren (ao_newindex);

		//una vez que ha sido modificado, la autoload ya no se va a disparar
        _SetIsLoaded (M4_TRUE); 

        //marco el bloque como 'NO ordenado':
        ClBlockSearchInfo::SetNotSorted ();

        //notificación a la gente
		ClBlockReference br;
		br.Set (this);
		m_pClNode_Data->GetpChannel_Data ()->GetpNotifier ()->AddRegister_After (m_pClNode_Data, br, ao_newindex);
    }

    if (res == M4_VRB_ERROR_MAX_COUNT)
    {
        ClHandle oNodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
        ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
        DUMP_CHLOG_ERRORF(M4_CH_DM_NUM_RECORDS, M4ObjNodeN(pmcr, oNodeHandle));
        res = M4_ERROR; // ERROR STANDARD
    }

	if (res == M4_VRB_ERROR_ROWS_QUOTA_EXCEEDED)
	{
        ClHandle oNodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
        ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
        DUMP_CHLOG_ERRORF(M4_CH_DM_ROWS_QUOTA_EXCEEDED, M4ObjNodeN(pmcr, oNodeHandle));
        res = M4_ERROR; // ERROR STANDARD
	}

    return res;
}

//función especial para optimización de la load: inserta un nuevo registro y le inicializa
//con los valores que se acaban de leer de la base de datos:

m4return_t ClBlock_Channel_Proxy::R_AddRecordAndSetLDBItems(ClMMIndex & ao_Index, ClRegister_Pipe* ai_pRegPipe, m4bool_t ai_bAddRegister, m4bool_t ai_bAuxLoad)
{
    m4return_t res;
    m4uint32_t iUniqueRowId;

    if (0 == m_pClBlock) {
        if (!_CreateBlock()) return M4_ERROR;
        M4_ASSERT (m_pClBlock);
    }

// Nuevo identificador unico de registro
    iUniqueRowId = GetpNode_Data()->GetpChannel_Data()->GetUniqueRowId();

	res = m_pClBlock->R_AddRecordAndSetLDBItems(ao_Index, ai_pRegPipe, iUniqueRowId, ai_bAddRegister, ai_bAuxLoad);

    if (M4_SUCCESS == res)
    {
        _R_InitChildren (ao_Index);

	    //marco el bloque como 'NO ordenado':
	    ClBlockSearchInfo::SetNotSorted ();

    }

// no hace falta comprobar que MaxNumREcors es != 0 ya que Count es siempre > 0
    if (m_pClBlock->Count() == GetpNode_Data()->GetpNodeDef()->MaxNumRecords())
        ai_pRegPipe->m_MaxRecords = M4_TRUE;
    else
    {
        if (m_pClBlock->Count() < GetpNode_Data()->GetpNodeDef()->MaxNumRecords())
            ai_pRegPipe->m_MaxRecords = M4_FALSE;
    }


    if (res == M4_VRB_ERROR_MAX_COUNT)
    {
        ClHandle oNodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
        ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
        DUMP_CHLOG_ERRORF(M4_CH_DM_NUM_RECORDS, M4ObjNodeN(pmcr, oNodeHandle));
        ai_pRegPipe->m_MaxRecords = M4_TRUE;
        res = M4_ERROR; // ERROR STANDARD
    }

	if (res == M4_VRB_ERROR_ROWS_QUOTA_EXCEEDED)
	{
        ClHandle oNodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
        ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
        DUMP_CHLOG_ERRORF(M4_CH_DM_ROWS_QUOTA_EXCEEDED, M4ObjNodeN(pmcr, oNodeHandle));
        res = M4_ERROR; // ERROR STANDARD
	}

    return res;
}

//----------------------------------------------------------------------------
// InsertRegister
//      operación de escritura
//		inserta un registro en la posición indicada
//----------------------------------------------------------------------------

m4return_t ClBlock_Channel_Proxy::R_InsertRegister (ClMMIndex ai_regindex)
{
    m4return_t res;
    m4uint32_t iUniqueRowId;

    if (0 == m_pClBlock) {
        if (!_CreateBlock()) return M4_ERROR;
        M4_ASSERT (m_pClBlock);
    }

// Nuevo identificador unico de registro
    iUniqueRowId = GetpNode_Data()->GetpChannel_Data()->GetUniqueRowId();

    res = m_pClBlock->R_InsertRegister (ai_regindex, iUniqueRowId);

    if (M4_SUCCESS == res) {
		//añadimos inmediatamente los bloques proxy correspondientes a este registro
		//en los nodos hijos RB:
        //(en el notify se puede poner un current sobre el nuevo registro
        // y cuando pide el bloque hijo ... catapún!)
	    _R_InitChildren (ai_regindex);

		//una vez que ha sido modificado, la autoload ya no se dispara
        _SetIsLoaded (M4_TRUE);

        //marco el bloque como 'NO ordenado':
        //M4_ASSERT (GetpSearchInfo ());
        ClBlockSearchInfo::SetNotSorted ();

		//Notificación:
        // utilizamos esta función para que todos los current y reg de todos los
        // access existentes se enteren de que se han movido (sólo si apuntaban
        // a un registro mayor o igual al del Index actual): se mueven al siguiente
        // para que no se enteren del cambio
        // se mueven todos excepto yo mismo que me quedo apuntando a Index:

		ClBlockReference br;
		br.Set (this);
        m_pClNode_Data->GetpChannel_Data ()->GetpNotifier ()->InsertRegister_After (m_pClNode_Data, br, ai_regindex);

        //que se actualizen los parentindex de los bloques hijos RB
		//avisamos al store:
			m4uint16_t i;
			m4uint16_t n = m_pClNode_Data->GetpExtInfo ()->ChildNodeRB.Count ();

			for (i = 0; i < n; i++) {
				ClNode_Data *pchildnode = m_pClNode_Data->GetpExtInfo ()->ChildNodeRB[(ClChildIndex)i].GetAddress ();
				M4_ASSERT (pchildnode);

				pchildnode->GetpBlockStore ()->Notify_ParentAddRegister (ID.BlockUniqueKey, ai_regindex);
			}
    }

    if (res == M4_VRB_ERROR_MAX_COUNT)
    {
        ClHandle oNodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
        ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
        DUMP_CHLOG_ERRORF(M4_CH_DM_NUM_RECORDS, M4ObjNodeN(pmcr, oNodeHandle));
        res = M4_ERROR; // ERROR STANDARD
    }

	if (res == M4_VRB_ERROR_ROWS_QUOTA_EXCEEDED)
	{
        ClHandle oNodeHandle = GetpNode_Data()->GetpNodeDef()->GetHandle();
        ClCompiledMCR * pmcr  = GetpNode_Data()->GetpNodeDef()->GetpCMCR ();
        DUMP_CHLOG_ERRORF(M4_CH_DM_ROWS_QUOTA_EXCEEDED, M4ObjNodeN(pmcr, oNodeHandle));
        res = M4_ERROR; // ERROR STANDARD
	}

    return res;
}

//----------------------------------------------------------------------------
// DeleteRegister
//      operación de escritura
//		marca el registro como borrado
//----------------------------------------------------------------------------

m4return_t ClBlock_Channel_Proxy::R_DeleteRegister (ClMMIndex ai_regindex)
{
    if (0 == m_pClBlock) {
		//un poco raro ¿no?, si queremos borrar un registro, el registro
		//debería de existir y el bloque estaría creado
		if (!_CreateBlock()) return M4_ERROR;
		M4_ASSERT (m_pClBlock);        
    }

    return m_pClBlock->R_DeleteRegister (ai_regindex);
}


m4return_t ClBlock_Channel_Proxy::R_GetRecordID (ClMMIndex ai_regindex, m4uint32_t &ao_iUniqueRowId)
{
    if (0 == m_pClBlock)
    {
        return M4_ERROR;
    }

    ao_iUniqueRowId = m_pClBlock->R_GetRecordID (ai_regindex);
    return M4_SUCCESS;
}


m4return_t ClBlock_Channel_Proxy::R_SetRecordID (const ClMMIndex ai_regindex, const m4uint32_t ai_iUniqueRowId)
{
    if (0 == m_pClBlock)
    {
        return M4_ERROR;
    }

    m_pClBlock->R_SetRecordID (ai_regindex, ai_iUniqueRowId);
    return M4_SUCCESS;
}


m4return_t ClBlock_Channel_Proxy::B_FindByRecordID (ClMMIndex &ao_regindex, m4uint32_t ai_iUniqueRowId)
{
    if (0 == m_pClBlock)
    {
        ao_regindex = M4DM_EOF_INDEX;
        return M4_ERROR;
    }

    m4uint32_t i, n;

    n = m_pClBlock->Count ();
    for (i = 0; i < n; i++)
    {
        if ( m_pClBlock->R_GetRecordID (i) == ai_iUniqueRowId)
        {
            ao_regindex = i;
            return M4_SUCCESS;
        }
    }

    ao_regindex = M4DM_EOF_INDEX;

    return M4_ERROR;
}


m4return_t ClBlock_Channel_Proxy::Reserve (const regindex_t ai_iSize)
{
    if (0 == m_pClBlock)
    {
        return M4_ERROR;
    }

    m_pClBlock->Reserve (ai_iSize);
    return M4_SUCCESS;
}


//----------------------------------------------------------------------------

//función axiliar: borra todos los bloques hijos (RB) del registro indicado
//se llama por R_DestroyRegister

void ClBlock_Channel_Proxy::R_DestroyRBChilds (ClMMIndex ai_regindex)
{
   m4uint16_t i, numRBs;
   numRBs = m_pClNode_Data->GetpExtInfo ()->ChildNodeRB.Count ();

   if (numRBs) {
      //ClBlock_Channel_Proxy *pchildblock;

      for (i = 0; i < numRBs; i++) {
          ClNode_Data *pnchild = m_pClNode_Data->GetpExtInfo ()->ChildNodeRB [(ClChildIndex)i].GetAddress ();

          M4_ASSERT (pnchild);
          M4_ASSERT (pnchild->GetpBlockStore());
		
		  //que no cree el bloque si no existe:
		  ClBlockReference br;
          pnchild->GetpBlockStore()->Get(ID.BlockUniqueKey, ai_regindex, br);
		  //ahora puede ser cero:
		  if (br) {
			  M4_ASSERT (! br->IsNihil ());  // no debe ser nihil
			  //elimino el bloque:
			  br->DestroyProxy (M4_TRUE); // hace un delete de pb
              //hay que hacerlo así para que no pete:
              br.Invalide ();
				//M4_TRUE ==> que elimine el bloque del store			  
		  }
      }
   }
}

m4return_t ClBlock_Channel_Proxy::R_Destroy (ClMMIndex ai_regindex)
{
    m4return_t res;

    //si vamos a destruir un registro deberíamos estar posicionados, y por tanto
    //el bloque debería de existir:
    if (m_pClBlock) {

        // avisamos antes de que se va a borrar el registro (hacía falta)
		ClBlockReference br;
		br.Set (this);
        m_pClNode_Data->GetpChannel_Data ()->GetpNotifier ()->DestroyRegister_Before (m_pClNode_Data, br, ai_regindex);

        // Se destruyen los bloques hijos (RB):
        R_DestroyRBChilds(ai_regindex);

        // Se destruye el registro
        res = m_pClBlock->R_Destroy (ai_regindex);

        /*NO: por borrar un registro el bloque no queda desordenado !
        if (res == M4_SUCCESS) {
            //marco el bloque como 'NO ordenado':
            M4_ASSERT (GetpSearchInfo ());
            GetpSearchInfo ()->_SetIsSorted (M4_FALSE);
        }
        */

		m4uint16_t i, numRBs;
		numRBs = m_pClNode_Data->GetpExtInfo ()->ChildNodeRB.Count ();

		for (i = 0; i < numRBs; i++) {
			ClNode_Data *pnchild = m_pClNode_Data->GetpExtInfo ()->ChildNodeRB [(ClChildIndex)i].GetAddress ();
			M4_ASSERT (pnchild);
			M4_ASSERT (pnchild->GetpBlockStore());

			//y avisamos para que se corran (con perdon) para atras
			pnchild->GetpBlockStore ()->Notify_ParentRemoveRegister (ID.BlockUniqueKey, ai_regindex);
		}

        // avisamos después también a todos los access asociados al canal para que se espabilen
        // y pogan sus iteradores de registros consistentes con el canal
        m_pClNode_Data->GetpChannel_Data ()->GetpNotifier ()->DestroyRegister_After (m_pClNode_Data, br, ai_regindex);
    }
    else {
        res = M4_ERROR;
    }

    return res;
}

//----------------------------------------------------------------------------
// CheckPoint, borra el código de operación
//----------------------------------------------------------------------------

m4return_t ClBlock_Channel_Proxy::CheckPoint (void)
{
    //tambien en pruebas:
    m4return_t res = M4_SUCCESS;
    
    if (m_pClBlock)
    {
        res = m_pClBlock->Block.CheckPoint();

        if (res == M4_SUCCESS)
        {
            //lo hago yo:
            m4uint8_t status;
            m_pClBlock->Records.Status(status);
            if (status == VS_NORMAL)
                return M4_SUCCESS;
            
            m4bool_t RecordIsDeleted;
            
            if (!m_pClBlock->Records.UsingCheckPoints ()) // ESTADO
            {
                m_pClBlock->Records.ResetStatus();
                
                return M4_SUCCESS;
            }
            
            regindex_t regIndex = 0;
            m4uint32_t n = m_pClBlock->Records.GetCount();
            
            for (; n; n--)
            {
                if (m_pClBlock->Records.IsRecordValid(regIndex) == M4_TRUE)    // Alex 12/1/98
                {
                    status = m_pClBlock->Records.RecordStatus(regIndex);
                    
                    //parte nueva:
                    if (status == M4_RS_DELETED || status == M4_RS_NEW_AND_DELETED || status == M4_RS_MODIFIED_AND_DELETED)
                    {
                        //llamada estandar para borrar un registro:
                        R_Destroy (regIndex);
                    }
                    else
                    {
                        /* Alex 12/1/98
                        if (m_pClBlock->m_oDataBlock.Records.UsingCheckPoints ())
                        {
                        */
                            //Con CP
                            res = m_pClBlock->Records.CommitRecord(regIndex,RecordIsDeleted);
                            M4_ASSERT (RecordIsDeleted == M4_FALSE);
                            if (res == M4_ERROR)
                                return res;
                        /* Alex 12/1/98 
                        }
                        else
                        {
                           //reset del status
                            m_pClBlock->m_oDataBlock.Records.ResetStatusRecord(regIndex);
                        } */
                        
                        //if (!RecordIsDeleted)
                        regIndex++;
                    }
                }
                else
                    regIndex++;
            }
            
            m_pClBlock->Records.SetStatus(VS_NORMAL);
            
            return M4_SUCCESS;
        }
    }
    
    // por definicion no tiene registros
    return res;
}

m4return_t ClBlock_Channel_Proxy::CheckPoint (ClMMIndex ai_regindex)
{
    m4return_t res = M4_SUCCESS;

    if (m_pClBlock) {
        if (R_IsDeleted (ai_regindex)) {
            // Si esta marcado para borrar me lo cargo para que borre a los hijos.
            R_Destroy (ai_regindex);
        }
        else {
            m4bool_t bIsDeleted;
            res = m_pClBlock->CheckPoint (ai_regindex, bIsDeleted);
            if (bIsDeleted)
                return M4_ERROR;
        }
    }
    return res;
}

m4return_t ClBlock_Channel_Proxy::B_Undo(void)
{
	if (m_pClBlock) {
		//m4bool_t bdeleted;
		m4uint32_t n = m_pClBlock->Records.GetCount ();
		m4uint32_t i = 0;
		regindex_t regIndex = i;

		while (i < n) {
			RegisterStatus_t status = m_pClBlock->Records.RecordStatus (regIndex);
			if (status == M4_RS_NEW || status == M4_RS_NEW_AND_DELETED) {
				R_Destroy (regIndex);
				n --;
			} else {
				i ++;
				regIndex = i;
			}
		}

		return m_pClBlock->B_Undo();
	}
	return M4_SUCCESS;
}

m4bool_t ClBlock_Channel_Proxy::R_Undo(ClMMIndex ai_regindex)
{
    m4return_t res;
    if (m_pClBlock) {
        regindex_t  regIndex = (m4uint32_t) ai_regindex;
        RegisterStatus_t status = m_pClBlock->Records.RecordStatus (regIndex);

        if (status == M4_RS_NEW || status == M4_RS_NEW_AND_DELETED) {
            // Si esta marcado como nuevo me lo cargo para que borre a los hijos.
            R_Destroy (ai_regindex);
			return M4_TRUE;
        }
		else {
            res = m_pClBlock->R_Undo (ai_regindex);
			return M4_FALSE;
		}
    }
    else {
        //No se si devolver error o exito!!!!!
        return M4_FALSE;
    }
}

m4return_t ClBlock_Channel_Proxy::SetCheckPointMode (m4bool_t ai_operation)
{
    if (m_pClBlock) {
        return m_pClBlock->SetCheckPointMode (ai_operation);
    }

    m_bCheckPointMode = ai_operation;

    return M4_SUCCESS;
}

m4return_t ClBlock_Channel_Proxy::GetCheckPointMode(m4bool_t& ao_operation)
{
    if (m_pClBlock) {
	    ao_operation = m_pClBlock->GetCheckPointMode();
    } else {
	    ao_operation = m_bCheckPointMode;
    }
    return M4_SUCCESS;
}

m4return_t ClBlock_Channel_Proxy::CommitPersisted (VersionMark &ai_version)
{
	//en pruebas:
	//ahora me encargo yo de hacerlo:
	if (m_pClBlock) {
		m4bool_t ai_Dirty = M4_FALSE;
		m4bool_t Dirty = m_pClBlock->GetBlockPersistLDB();;
		
		if (Dirty != ai_Dirty) {
			m4return_t ret = m_pClBlock->Block.Commit();
			if (ret != M4_SUCCESS) return ret;
			
			ret = m_pClBlock->SetBlockPersistLDB(ai_Dirty);
			if (ret != M4_SUCCESS) return ret;
			
			m4bool_t RecordIsDeleted;
			regindex_t I = 0;
			m4uint32_t n = m_pClBlock->Records.GetCount();          
			RegisterStatus_t status;
			
			for (; n; n--) {
				if (m_pClBlock->Records.IsRecordValid(I) == M4_FALSE) {
					I++;
					continue;
				}
				
				Dirty = m_pClBlock->R_GetRecordPersist(I);
				
				//EDU------------------>>
				//if (Dirty!=ai_Dirty)
				//modificación: se trata de cometer también aquellos registros que 
				//estén new&deleted, porque estos no van a llevar la marca de persisted (dirty)             
				status = m_pClBlock->Records.RecordStatus(I);
				if ( (Dirty != ai_Dirty) || (status == M4_RS_NEW_AND_DELETED) )
				//EDU------------------<<
				{
					//parte nueva:
					if (status == M4_RS_DELETED || status == M4_RS_NEW_AND_DELETED || status == M4_RS_MODIFIED_AND_DELETED) {
						//llamada estandar para borrar un registro:
						R_Destroy (I);
					} else {
						if (m_pClBlock->Records.UsingCheckPoints()) {
							// con CP
							ret = m_pClBlock->Records.CommitRecord(I, RecordIsDeleted);
							M4_ASSERT (RecordIsDeleted == M4_FALSE);
						} else {
							//reset del status
							//m_pClBlock->m_oDataBlock.Records.ResetStatusRecord (I);
							m_pClBlock->Records.SetStatus(I, M4_RS_NORMAL);
							ret = M4_SUCCESS;
						}
						
						if (ret != M4_ERROR) {
							//if (!RecordIsDeleted)
							//{
							ret = m_pClBlock->R_SetRecordPersist(I, ai_Dirty);
							if (ret != M4_SUCCESS) return ret;
							
							//if ((ret=m_pClBlock->m_oDataBlock.SetVersionByPos(I,ai_version))!=M4_SUCCESS)
							//   return ret;
							
							I++;
							//}
						}
					}
				} else {
					I++;
				}
			}
			ret = m_pClBlock->Records.Commit();
			if (ret != M4_SUCCESS) return ret;
		}
		
	}
	
	return M4_SUCCESS;
}

m4return_t ClBlock_Channel_Proxy::RollbackPersisted (void)
{
    if (m_pClBlock) {
		m_pClBlock->B_SetReLoad(M4_FALSE);
        return m_pClBlock->RollbackPersisted ();
    }

    return M4_SUCCESS;
}

// jcr
m4return_t	ClBlock_Channel_Proxy :: CheckModify( m4bool_t &ao_bModified )
{
	m4bool_t		bIsValid ;
	m4bool_t		bModifiedDB ;
	m4bool_t		bNewNotPersist ;
	m4uint8_t		iAffectsDB ;
	m4uint8_t		iScope ;
	m4uint32_t		iHandle ;
	m4uint32_t		iNodeHandle ;
	m4uint32_t		iPlugHandle ;
	m4uint32_t		iInverse ;
	m4pcchar_t		pccFatherM4ObjId ;
	m4pcchar_t		pccFatherNodeId ;
	m4VariantType		vValue, vDefault, vOldValue ;
	RegisterStatus_t	sStatus ;
	ClCompiledMCR		*poCMCR ;
	
	
	// No porque el recorrido de bloques no para
	//	ao_bModified = M4_FALSE ;
	
	if( m_pClBlock == NULL )
	{
		return( M4_SUCCESS ) ;
	}
	
	// La relacion va a ir cacheada
	m4int8_t iRelation = -1 ;
	
	m4uint32_t iLength = m_pClBlock->Records.GetCount() ;
	
	for( m4uint32_t i = 0 ; i < iLength ; i++ )
	{
		bIsValid = m_pClBlock->Records.IsRecordValid( i ) ;
		
		if( bIsValid == M4_FALSE )
		{
			continue ;
		}
		
		sStatus = m_pClBlock->Records.RecordStatus( i ) ;
		
		if( sStatus == M4_RS_DELETED || sStatus == M4_RS_MODIFIED_AND_DELETED )
		{
			ao_bModified = M4_TRUE ;
			return( M4_SUCCESS ) ;
		}
		
		if( sStatus == M4_RS_NEW )
		{
			// Si tiene marca de no persistir continuamos
			bNewNotPersist = m_pClBlock->Records.GetRecordNewNotPersist( i ) ;
			
			if( bNewNotPersist == M4_TRUE )
			{
				continue ;
			}
			
			ClRegister oRegister( 0 ) ;
			
			oRegister.SetCurrentBlock( this ) ;
			oRegister.MoveTo( i ) ;
			
			oRegister.Item.SetMode( M4DM_SCOPE_ALL, M4DM_TYPE_PROPERTY | M4DM_TYPE_FIELD | M4DM_TYPE_CONCEPT ) ;
			
			oRegister.Item.Begin() ;
			while( oRegister.Item.GetIndex() != M4DM_EOF_INDEX )
			{
				iHandle = oRegister.Item.ItemDef.GetHandle() ;
				poCMCR = oRegister.Item.ItemDef.GetpCMCR() ;
				
				iScope = poCMCR->GetItemScope( iHandle ) ;
				iInverse = poCMCR->GetItemInverseConnector( iHandle ) ;
				
				// Sólo si es de registro, o bien está conectado por carril
				if( iScope == M4CL_ITEM_SCOPE_REGISTER || iInverse == m4uint32_t( -1 ) )
				{
					// Si es de carril vamos a ver si es 1:1
					if( iInverse == m4uint32_t( -1 ) )
					{
						if( iRelation == -1 )
						{
							iNodeHandle = poCMCR->GetItemNodeHandle( iHandle ) ;
							
							pccFatherM4ObjId = poCMCR->GetLaneFatherChannelId() ;
							pccFatherNodeId = poCMCR->GetLaneFatherNodeId() ;
							
							if( pccFatherM4ObjId != NULL && pccFatherNodeId != NULL )
							{
								iPlugHandle = poCMCR->GetNodePlugHandleByIds( iNodeHandle, pccFatherM4ObjId, pccFatherNodeId ) ;
								
								if( iPlugHandle != 0 )
								{
									iRelation = poCMCR->GetPlugRelationTypeId( iPlugHandle ) ;
								}
							}
						}
						
						if( iRelation != 0 )
						{
							oRegister.Item.Next() ;
							continue ;
						}
					}
					
					iAffectsDB = poCMCR->GetItemAffectsDB( iHandle ) ;
					
					if( iAffectsDB == 1 )
					{
						oRegister.Item.Value.GetWithOutSec( vValue ) ;
						poCMCR->GetItemDefaultValue( iHandle, vDefault ) ;
						
						if( ( vValue.Type != M4CL_CPP_TYPE_NULL ) && ( vValue != vDefault ) )
						{
							ao_bModified = M4_TRUE ;
							return( M4_SUCCESS ) ;
						}
					}
				}
				oRegister.Item.Next() ;
			}
		}
		
		bModifiedDB = m_pClBlock->Records.IsRecordModifiedInDB( i ) ;
		
		if( bModifiedDB == M4_TRUE )
		{
			// Miramos a ver si hay algún valor distinto del oldvalue
			ClRegister oRegister( 0 ) ;
			
			oRegister.SetCurrentBlock( this ) ;
			oRegister.MoveTo( i ) ;
			
			oRegister.Item.SetMode( M4DM_SCOPE_REGISTER, M4DM_TYPE_PROPERTY | M4DM_TYPE_FIELD | M4DM_TYPE_CONCEPT ) ;
			
			oRegister.Item.Begin() ;
			while( oRegister.Item.GetIndex() != M4DM_EOF_INDEX )
			{
				iHandle = oRegister.Item.ItemDef.GetHandle() ;
				poCMCR = oRegister.Item.ItemDef.GetpCMCR() ;
				
				iAffectsDB = poCMCR->GetItemAffectsDB( iHandle ) ;
				
				if( iAffectsDB == 1 )
				{
					oRegister.Item.Value.GetWithOutSec( vValue ) ;
					oRegister.Item.OldValue.GetWithOutSec( vOldValue ) ;
					
					// Si son nulos los dos son iguales
					if( vValue.Type != M4CL_CPP_TYPE_NULL || vOldValue.Type != M4CL_CPP_TYPE_NULL )
					{
						if( vValue != vOldValue )
						{
							ao_bModified = M4_TRUE ;
							return( M4_SUCCESS ) ;
						}
					}
				}
				oRegister.Item.Next() ;
			}
		}
	}
	return( M4_SUCCESS ) ;
}

m4bool_t ClBlock_Channel_Proxy::R_GetNewNotPersist(ClMMIndex ai_regindex) const
{
    if (m_pClBlock && ai_regindex != M4DM_EOF_INDEX) {
        return m_pClBlock->Records.GetRecordNewNotPersist(ai_regindex);
    }

    return M4_FALSE;
}

m4return_t ClBlock_Channel_Proxy::R_SetNewNotPersist(ClMMIndex ai_regindex, const m4bool_t ai_bNewNotPersist)
{
    if (m_pClBlock && ai_regindex != M4DM_EOF_INDEX) {
        m_pClBlock->Records.SetRecordNewNotPersist(ai_regindex, ai_bNewNotPersist);
    }
    return M4_SUCCESS;
}


//****************************************************************************
// FUNCIONES DE OPERACION
//****************************************************************************

m4return_t ClBlock_Channel_Proxy::R_GetStatus (const ClMMIndex ai_index, RegisterStatus_t &ao_riStatus)
{
    if (m_pClBlock && ai_index != M4DM_EOF_INDEX) {
        return m_pClBlock->R_GetStatus (ai_index, ao_riStatus);
    }

    //el bloque debería estar creado, pq para trabajar sobre un registro, primero debería
    //haber hecho un moveto al registro.

	ao_riStatus = M4_RS_INVALID;
	return M4_ERROR;
}

m4return_t ClBlock_Channel_Proxy::R_SetStatus (const ClMMIndex        ai_index,
                                               const RegisterStatus_t ai_NewStatus)
{
    if (m_pClBlock && ai_index != M4DM_EOF_INDEX) {
        return m_pClBlock->R_SetStatus (ai_index, ai_NewStatus);
    }

    //el bloque debería estar creado, pq para trabajar sobre un registro, primero debería
    //haber hecho un moveto al registro.

    return M4_ERROR;
}

m4bool_t ClBlock_Channel_Proxy::R_IsModified (const ClMMIndex ai_index) const
{
    if (m_pClBlock  && ai_index != M4DM_EOF_INDEX) {
        return m_pClBlock->R_IsModified (ai_index);
    }

    //el bloque debería estar creado, pq para trabajar sobre un registro, primero debería
    //haber hecho un moveto al registro.

    return M4_FALSE;
}

m4bool_t ClBlock_Channel_Proxy::R_IsNew (const ClMMIndex ai_index) const
{
    if (m_pClBlock && ai_index != M4DM_EOF_INDEX) {
        return m_pClBlock->R_IsNew (ai_index);
    }

    //el bloque debería estar creado, pq para trabajar sobre un registro, primero debería
    //haber hecho un moveto al registro.

    return M4_FALSE;
}

m4bool_t ClBlock_Channel_Proxy::R_IsUpdated (const ClMMIndex ai_index) const
{
    if (m_pClBlock && ai_index != M4DM_EOF_INDEX) {
        return m_pClBlock->R_IsUpdated (ai_index);
    }

    //el bloque debería estar creado, pq para trabajar sobre un registro, primero debería
    //haber hecho un moveto al registro.

    return M4_FALSE;
}
m4bool_t ClBlock_Channel_Proxy::R_IsNewAndDeleted	(const ClMMIndex ai_index) const
{
    if (m_pClBlock && ai_index != M4DM_EOF_INDEX) {
		return m_pClBlock->R_IsNewAndDeleted (ai_index);
    }

    return M4_FALSE;
}

m4bool_t ClBlock_Channel_Proxy::R_IsDeleted (const ClMMIndex ai_index) const
{
    if (m_pClBlock && ai_index != M4DM_EOF_INDEX) {
        return m_pClBlock->R_IsDeleted (ai_index);
    }

    //el bloque debería estar creado, pq para trabajar sobre un registro, primero debería
    //haber hecho un moveto al registro.

    return M4_FALSE;
}

m4bool_t ClBlock_Channel_Proxy::R_IsModifiedByDDBB (const ClMMIndex ai_index) const
{
    if (m_pClBlock && ai_index != M4DM_EOF_INDEX) {
        if (GetpNode_Data()->GetpNodeDef()->AffectsDB())
        {
            return (m_pClBlock->R_IsNew (ai_index) || m_pClBlock->R_IsDeleted(ai_index) ||
                m_pClBlock->R_IsUpdatedByDDBB (ai_index)) ? M4_TRUE : M4_FALSE;
        }
    }

    return M4_FALSE;
}

m4bool_t ClBlock_Channel_Proxy::R_IsNewByDDBB (const ClMMIndex ai_index) const
{
    if (m_pClBlock && ai_index != M4DM_EOF_INDEX) {
        if (GetpNode_Data()->GetpNodeDef()->AffectsDB())
        {
            return m_pClBlock->R_IsNew (ai_index);
        }
    }

    return M4_FALSE;
}

m4bool_t ClBlock_Channel_Proxy::R_IsUpdatedByDDBB (const ClMMIndex ai_index) const
{
    if (m_pClBlock && ai_index != M4DM_EOF_INDEX) {
        if (GetpNode_Data()->GetpNodeDef()->AffectsDB())
        {
            return m_pClBlock->R_IsUpdatedByDDBB (ai_index);
        }
    }

    return M4_FALSE;
}

m4bool_t ClBlock_Channel_Proxy::R_IsDeletedByDDBB (const ClMMIndex ai_index) const
{
    if (m_pClBlock && ai_index != M4DM_EOF_INDEX) {
        if (GetpNode_Data()->GetpNodeDef()->AffectsDB())
        {
            return m_pClBlock->R_IsDeleted (ai_index);
        }
    }

    return M4_FALSE;
}

m4bool_t ClBlock_Channel_Proxy::R_IsModified (void) const
{
    if (m_pClBlock) {
        return m_pClBlock->R_IsModified ();
    }
    else {
        //malamente va a estar modificado si no se ha creado siquiera
        return M4_FALSE;
    }
}

m4bool_t ClBlock_Channel_Proxy::R_IsNew (void) const
{
    if (m_pClBlock) {
        return m_pClBlock->R_IsNew ();
    }
    else {
        //malamente va a estar modificado si no se ha creado siquiera
        return M4_FALSE;
    }
}

m4bool_t ClBlock_Channel_Proxy::R_IsUpdated (void) const
{
    if (m_pClBlock) {
        return m_pClBlock->R_IsUpdated ();
    }
    else {
        //malamente va a estar modificado si no se ha creado siquiera
        return M4_FALSE;
    }
}

m4bool_t ClBlock_Channel_Proxy::R_IsDeleted (void) const
{
    if (m_pClBlock) {
        return m_pClBlock->R_IsDeleted ();
    }
    else {
        //malamente va a estar modificado si no se ha creado siquiera
        return M4_FALSE;
    }
}

m4bool_t ClBlock_Channel_Proxy::R_IsModifiedByDDBB (void) const
{
    if (m_pClBlock) {
        if (GetpNode_Data()->GetpNodeDef()->AffectsDB())
        {
            return (m_pClBlock->R_IsNew () || m_pClBlock->R_IsDeleted() ||
                m_pClBlock->R_IsUpdatedByDDBB ()) ? M4_TRUE : M4_FALSE;
        }
    }

    return M4_FALSE;
}

m4bool_t ClBlock_Channel_Proxy::R_IsNewByDDBB (void) const
{
    if (m_pClBlock) {
        if (GetpNode_Data()->GetpNodeDef()->AffectsDB())
        {
            return m_pClBlock->R_IsNew ();
        }
    }

    return M4_FALSE;
}

m4bool_t ClBlock_Channel_Proxy::R_IsUpdatedByDDBB (void) const
{
    if (m_pClBlock) {
        if (GetpNode_Data()->GetpNodeDef()->AffectsDB())
        {
            return m_pClBlock->R_IsUpdatedByDDBB ();
        }
    }

    return M4_FALSE;
}

m4bool_t ClBlock_Channel_Proxy::R_IsDeletedByDDBB (void) const
{
    if (m_pClBlock) {
        if (GetpNode_Data()->GetpNodeDef()->AffectsDB())
        {
            return m_pClBlock->R_IsDeleted ();
        }
    }

    return M4_FALSE;
}

m4bool_t ClBlock_Channel_Proxy::B_IsModified (void) const
{
    if (m_pClBlock) {
        return m_pClBlock->B_IsModified ();
    }
    else {
        //malamente va a estar modificado si no se ha creado siquiera
        return M4_FALSE;
    }
}

m4bool_t ClBlock_Channel_Proxy::B_IsNew (void) const
{
    if (m_pClBlock) {
        return m_pClBlock->B_IsNew ();
    }
    else {
        //malamente va a estar modificado si no se ha creado siquiera
        return M4_FALSE;
    }
}

m4bool_t ClBlock_Channel_Proxy::B_IsUpdated (void) const
{
    if (m_pClBlock) {
        return m_pClBlock->B_IsUpdated ();
    }
    else {
        //malamente va a estar modificado si no se ha creado siquiera
        return M4_FALSE;
    }
}

m4bool_t ClBlock_Channel_Proxy::B_IsDeleted (void) const
{
    if (m_pClBlock) {
        return m_pClBlock->B_IsDeleted ();
    }
    else {
        //malamente va a estar modificado si no se ha creado siquiera
        return M4_FALSE;
    }
}

m4bool_t ClBlock_Channel_Proxy::B_IsModifiedByDDBB (void) const
{
    if (m_pClBlock) {
        if (GetpNode_Data()->GetpNodeDef()->AffectsDB())
        {
            return (m_pClBlock->B_IsNew () || m_pClBlock->B_IsDeleted() ||
                m_pClBlock->B_IsUpdatedByDDBB ()) ? M4_TRUE : M4_FALSE;
        }
    }

    return M4_FALSE;
}

m4bool_t ClBlock_Channel_Proxy::B_IsNewByDDBB (void) const
{
    if (m_pClBlock) {
        if (GetpNode_Data()->GetpNodeDef()->AffectsDB())
        {
            return m_pClBlock->B_IsNew ();
        }
    }

    return M4_FALSE;
}

m4bool_t ClBlock_Channel_Proxy::B_IsUpdatedByDDBB (void) const
{
    if (m_pClBlock) {
        if (GetpNode_Data()->GetpNodeDef()->AffectsDB())
        {
            return m_pClBlock->B_IsUpdatedByDDBB ();
        }
    }

    return M4_FALSE;
}

m4bool_t ClBlock_Channel_Proxy::B_IsDeletedByDDBB (void) const
{
    if (m_pClBlock) {
        if (GetpNode_Data()->GetpNodeDef()->AffectsDB())
        {
            return m_pClBlock->B_IsDeleted ();
        }
    }

    return M4_FALSE;
}

m4return_t ClBlock_Channel_Proxy::RefreshFlags (m4uint8_t &ao_Operation)
{

    ao_Operation = M4DM_OPERATION_NONE;

    if (m_pClBlock)
    {
        // Primero refresco las marcas del vrb y luego se las doy al nodo.
        m_pClBlock->B_RefreshFlags();

        if (B_IsUpdated()||R_IsUpdated())
            ao_Operation |= M4DM_OPERATION_UPDATE;
        if (B_IsNew()||R_IsNew())
            ao_Operation |= M4DM_OPERATION_INSERT;
        if (B_IsDeleted()||R_IsDeleted())
            ao_Operation |= M4DM_OPERATION_DELETE;

        if (B_IsUpdatedByDDBB()||R_IsUpdatedByDDBB())
            ao_Operation |= M4DM_OPERATION_UPDATE_BY_DDBB;
        if (B_IsNewByDDBB()||R_IsNewByDDBB())
            ao_Operation |= M4DM_OPERATION_INSERT_BY_DDBB;
        if (B_IsDeletedByDDBB()||R_IsDeletedByDDBB())
            ao_Operation |= M4DM_OPERATION_DELETE_BY_DDBB;
    }

    return M4_SUCCESS;
}


//****************************************************************************
//****************************************************************************

m4bool_t ClBlock_Channel_Proxy::R_IsValid (ClMMIndex ai_index)
{
    if (m_pClBlock) {
        return m_pClBlock->R_IsValid (ai_index);
    }

    //el bloque debería estar creado, pq para trabajar sobre un registro, primero debería
    //haber hecho un moveto al registro.

    return M4_FALSE;
}

m4return_t ClBlock_Channel_Proxy::R_SetUpdated (ClMMIndex ai_index)
{
    if (m_pClBlock) {
        return m_pClBlock->R_SetUpdated (ai_index);
    }

    //el bloque debería estar creado, pq para trabajar sobre un registro, primero debería
    //haber hecho un moveto al registro.

    return M4_FALSE;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

m4bool_t ClBlock_Channel_Proxy::IsProxy (void) const
{
    return M4_TRUE;
}

m4bool_t ClBlock_Channel_Proxy::IsValid (void) const
{
    return (m_pClBlock)? M4_TRUE : M4_FALSE;
}

/*
ClNode_Data *ClBlock_Channel_Proxy::GetpNode_Data (void) const
{
    return m_pClNode_Data;    
}
*/

void ClBlock_Channel_Proxy::UpdateIndex (m4uint32_t *ai_pindex_qtc, m4uint32_t ai_n)
{
    if (m_pClBlock) {
        m_pClBlock->UpdateIndex (ai_pindex_qtc, ai_n);
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// los destroys:
//-----------------------------------------------------------------------------

//destroy de bloque
// al loro porque cuando se destruye un bloque, en la implementación siguiente
// se destruyen todos los bloques 'hermanos' (BB)
// -->> aunque los proxys no se eliminan del store

m4return_t ClBlock_Channel_Proxy::DestroyBlock (m4bool_t ai_notify/*= M4_TRUE*/)
{
    M4_ASSERT (m_pClNode_Data);

    m4return_t res;

    m4uint16_t i, n;
    n = m_pClNode_Data->GetpExtInfo ()->ChildNodeBB.Count ();

    if (n) {
        ClNode_Data *pchildnode;
        //ClBlock_Channel_Proxy *pchildblock;

        for (i = 0; i < n; i ++) {
            pchildnode = m_pClNode_Data->GetpExtInfo ()->ChildNodeBB [(ClChildIndex)i].GetAddress ();
            M4_ASSERT (pchildnode);

            //conseguimos el bloque hermano:
			//que no cree el bloque si no existe:
			ClBlockReference br;
            pchildnode->GetpBlockStore ()->Get (ID.BlockUniqueKey, 0, br);
			if (br) {
				res = br->DestroyBlock ();
				br.Reset ();
				if (res == M4_ERROR)
					return res; // muy chungo
			}
        }
    }

	// itero por los nodos hijos RB, para que enteren que en el padre se borra
	// el bloque:
	n = m_pClNode_Data->GetpExtInfo ()->ChildNodeRB.Count ();

	if (n) {
		ClNode_Data *pchildnode;

		//para cada uno de mis hijos RB:    

		for (i = 0; i < n; i ++) {        
			pchildnode = m_pClNode_Data->GetpExtInfo ()->ChildNodeRB [(ClChildIndex)i].GetAddress ();
			M4_ASSERT (pchildnode);

			pchildnode->GetpBlockStore ()->ParentDestroyBlock (ID.BlockUniqueKey);
			//aquí lo que se hace es vaciar de un tirón las estructuras que mantienen
			//los bloques hijos de este en el que estamos borrando todos los registros
		}
	}

    //reseteo el proxy --> borro el ChBlock
    res = ResetProxy ();

	//notificación si corresponde
	if (ai_notify) {
		ClBlockReference br;
		br.Set (this);
		m_pClNode_Data->GetpChannel_Data ()->GetpNotifier ()->DestroyBlock_After(m_pClNode_Data, br);
        //aquí se produce un bucle infinito muy curioso, hay que hacer esto
        //para evitarlo
        br.Invalide ();
	}

    return res;
}

m4return_t ClBlock_Channel_Proxy::DestroyBlockNoUpdateAccesses (void)
{
    return DestroyBlock (M4_FALSE);
}

//llamada desde el register para que me elime del store y me suicide:
//pero no sólo me mato yo, mato también a mis bloques hermanos:

void ClBlock_Channel_Proxy::DestroyProxy (m4bool_t ai_bremovefromstore)
{
    DestroyBlock ();

    M4_ASSERT (m_pClNode_Data);

    m4uint16_t i, n;    

    n = m_pClNode_Data->GetpExtInfo ()->ChildNodeBB.Count ();

    if (n) {
        ClNode_Data *pchildnode;
        //ClBlock_Channel_Proxy *pchildblock;        

        for (i = 0; i < n; i ++) {
            pchildnode = m_pClNode_Data->GetpExtInfo ()->ChildNodeBB [(ClChildIndex)i].GetAddress ();
            M4_ASSERT (pchildnode);

            //conseguimos el bloque hermano:
			//que no cree el bloque si no existe:
			ClBlockReference br;
            pchildnode->GetpBlockStore ()->Get (ID.BlockUniqueKey, 0, br);
			if (br) {
				//siempre a true, sino un bug que te cagas
				//sólamente se optimiza (no eliminando del store) en 
				//el primer nivel de recursividad:

				br->DestroyProxy (M4_TRUE);
                //hay que hacerlo así para que no pete:
                br.Invalide ();				
			}
        }
    }

    //muy cutre con un flag 
    if (ai_bremovefromstore) {
        //ahora me elimino del store del nodo:
        //compruebo a mala leche que el que voy a borrar soy yo mismo:
		#if _DEBUG
			ClBlockReference br;
			//que no cree el bloque si no existe:
			m_pClNode_Data->GetpBlockStore ()->Get (ID.BlockId, ID.RegisterId, br);
			M4_ASSERT (br.GetpProxy () == this);
		#endif
        m4return_t res = m_pClNode_Data->GetpBlockStore ()->Remove (ID.BlockId, ID.RegisterId);
        M4_ASSERT (res == M4_SUCCESS);  //ok
    }

    //DestroyAllRegisters ();	ya está echo con el destroyblock de arriba

    //truco vil para que no se llame recursivamente por el DestroyProxyBlock_Before
    m_iRefCount = -1;

    //aviso a la peña de que me voy a suicidar:
	ClBlockReference br;
	br.Set (this);
    m_pClNode_Data->GetpChannel_Data ()->GetpNotifier ()->DestroyProxyBlock_Before (m_pClNode_Data, br);    
    //br.Reset ();    //antes del delete this, pq sino peta
    br.Invalide ();     //mejor

    //ahora me mato yo
    delete this;        //llama a reset proxy y todo eso
}

m4return_t ClBlock_Channel_Proxy::DestroyAllRegisters (m4bool_t ai_notify/*= M4_TRUE*/)
{
    m4return_t res = M4_SUCCESS;

    if (m_pClBlock) {
        //IMPORTANTE QUE NO SE DISPARE LA AUTOLOAD !!!!!!!!!!
        m4uint32_t count = m_pClBlock->Count ();
		if (count) {

			//1) itero por los nodos hijos RB, para que enteren que en el padre se borran
			//   todos los registros

			M4_ASSERT (m_pClNode_Data);

			m4uint16_t i, n;
			n = m_pClNode_Data->GetpExtInfo ()->ChildNodeRB.Count ();

			if (n) {
				ClNode_Data *pchildnode;

				//para cada uno de mis hijos RB:    

				for (i = 0; i < n; i ++) {        
					pchildnode = m_pClNode_Data->GetpExtInfo ()->ChildNodeRB [(ClChildIndex)i].GetAddress ();
					M4_ASSERT (pchildnode);

					pchildnode->GetpBlockStore ()->ParentDestroyAllRegisters (ID.BlockUniqueKey);
					//aquí lo que se hace es vaciar de un tirón las estructuras que mantienen
					//los bloques hijos de este en el que estamos borrando todos los registros
				}
			}

			//2) elimino a manubrio todos los registros llamando directamente a la capa 'DS'

			m4uint32_t ireg;
			//del último al primero !, no es ninguna tontería:
			ireg = count - 1;
			do {
				// destruyo el registro a cañón:
				res = m_pClBlock->R_Destroy (ireg);
				if (M4_ERROR == res)
					return res;

			} while (ireg--);   //postdec

			//3) Notificación:

			if (ai_notify) {
				M4_ASSERT (m_pClNode_Data);
				ClBlockReference br;
				br.Set (this);
				m_pClNode_Data->GetpChannel_Data ()->GetpNotifier ()->DestroyAllRegisters_After (m_pClNode_Data, br);
			}
		}
    }

    _SetIsLoaded (M4_FALSE);    //no pasa nada y seguro que a Tom le viene bien

    return res;
}

m4return_t ClBlock_Channel_Proxy::DestroyAllRegistersNoUpdateAccesses(void)
{
	return DestroyAllRegisters (M4_FALSE);
}


ClPartialLoadData * ClBlock_Channel_Proxy::GetpPL_Data(void)
{
    if (m_pClBlock == 0) {
        if (!_CreateBlock()) return 0;
        M4_ASSERT (m_pClBlock);
    }

    return m_pClBlock->GetpPL_Data();
}

m4return_t ClBlock_Channel_Proxy::R_PrepareIndexPL (const m4bool_t ai_bUp, const m4uint32_t ai_PrevReg)
{
    if (m_pClBlock == 0) {
        if (!_CreateBlock()) return M4_ERROR;
        M4_ASSERT (m_pClBlock);
    }

    m4uint32_t i, n, k, * pindex;

    n = GetpNode_Data ()->GetpExtInfo ()->NumRows();
    k = GetpNode_Data ()->GetpExtInfo ()->NumKeepRows();
    pindex = new m4uint32_t [n];

    if (ai_bUp)
    {
        for (i = 0; i < n; i ++)
        {
            pindex[i] = (i + ai_PrevReg) % n;
        }
    }
    else
    {
        for (i = 0; i < n; i ++)
        {
            pindex[i] = (i + n - ai_PrevReg) % n;
        }
    }

    _UpdateIndex (pindex, n);

	delete pindex;

    return M4_SUCCESS;
}


//----------------------------------------------------------------------------
// _R_InitChildren:
//		Añadimos inmediatamente los bloques proxy correspondientes a este registro
//		en los nodos hijos RB
//      Recorro los hijos y les inicializo con el valor del proxy:
//      Llamada desde el _ClRegister_Base::Add y _ClRegister_Base::Insert
// => utilizo el ChildNode del Channel
//----------------------------------------------------------------------------

void ClBlock_Channel_Proxy::_R_InitChildren (ClMMIndex ao_Index)
{
}

/*static*/
m4return_t ClBlock_Channel_Proxy::GetNewProxy (ClNode_Data *pnode, m4uint32_t blockid, m4uint32_t registerid, m4uint32_t ai_Parent_BlockId, m4uint32_t ai_Parent_RegisterIndex, ClBlockReference &ao_br)
{
    M4_ASSERT (pnode);

//    m4uint32_t blockuniquekey;

    //sólo aquí hacemos new:

    //nuevo, llamo a la factoria para que me dé una dirección:
    ClBlock_Channel_Proxy *pbcp;
    
        ////deshabilitado de momento
        pbcp = pnode->GetpChannel_Data ()->m_pProxyFactory->Get ();
        //pbcp = (ClBlock_Channel_Proxy *)new char [sizeof (ClBlock_Channel_Proxy)];

    //operator new overloaded
    new (pbcp) ClBlock_Channel_Proxy (pnode, blockid, registerid, ai_Parent_BlockId, ai_Parent_RegisterIndex);
    ao_br.Set (pbcp);

    #ifdef _DEBUG
        g_StMemInfo.iBlock_Proxy++;
    #endif

    M4_ASSERT (pbcp);

    /*
    pnode->GetpBlockStore ()->Add (blockid, registerid, ao_br, 
                                   blockuniquekey//out
                                   );
    pbcp->ID.BlockUniqueKey = blockuniquekey;
    #ifdef _DEBUG
		ClBlockReference br;
		//que no cree el bloque si no existe:
        pnode->GetpBlockStore ()->Get (blockid, registerid, br);
        M4_ASSERT (br == ao_br);    //operator == overloaded
    #endif
    */

    // Importante!!
    //  en el caso abuelo-padre RB, con padre-hijo BB, podemos insertar un registro en el abuelo
    //  e inmediatamente irnos al hijo trabajar con él ==> el proxy del hijo debe estar creado.

	return M4_SUCCESS;
}

//cañero:
void *ClBlock_Channel_Proxy::operator new (size_t, void *ai_pbuffer)
{
    //facil:
    return ai_pbuffer;
}

void ClBlock_Channel_Proxy::operator delete (void *ai_address)
{
    //nothing to do
    ////deshabilitado de momento
        //::delete ai_address;
}

m4uint32_t	ClBlock_Channel_Proxy::GetRangeSize( void ) const
{
	if( m_pClBlock == NULL )
	{
		return 0;
	}
	return m_pClBlock->GetRangeSize() ;
}

m4uint32_t	ClBlock_Channel_Proxy::GetRangeStart( void ) const
{
	if( m_pClBlock == NULL )
	{
		return 0;
	}
	return m_pClBlock->GetRangeStart() ;
}

m4return_t	ClBlock_Channel_Proxy::SetRangeSize( m4uint32_t ai_iRangeSize )
{
	if( m_pClBlock == NULL )
	{
		if( _CreateBlock() == NULL )
		{
			return M4_ERROR;
		}
	}

	m_pClBlock->SetRangeSize( ai_iRangeSize ) ;
	return M4_SUCCESS;
}

m4return_t	ClBlock_Channel_Proxy::SetRangeStart( m4uint32_t ai_iRangeStart )
{
	if( m_pClBlock == NULL )
	{
		if( _CreateBlock() == NULL )
		{
			return M4_ERROR;
		}
	}

	m_pClBlock->SetRangeStart( ai_iRangeStart ) ;
	return M4_SUCCESS;
}

m4return_t	ClBlock_Channel_Proxy::SetRangeStartNext( void )
{
	if( m_pClBlock == NULL )
	{
		return M4_SUCCESS;
	}

	m_pClBlock->SetRangeStartNext() ;
	return M4_SUCCESS;
}

m4uint32_t	ClBlock_Channel_Proxy::GetTimeOut( void ) const
{
	if( m_pClBlock == NULL )
	{
		return 0;
	}
	return m_pClBlock->GetTimeOut() ;
}

m4return_t	ClBlock_Channel_Proxy::SetTimeOut( m4uint32_t ai_iTimeOut )
{
	if( m_pClBlock == NULL )
	{
		if( _CreateBlock() == NULL )
		{
			return M4_ERROR;
		}
	}

	m_pClBlock->SetTimeOut( ai_iTimeOut ) ;
	return M4_SUCCESS;
}

m4return_t	ClBlock_Channel_Proxy::GetLastLoadStatus( m4uint8_t &ao_riLastLoadStatus, m4uint32_t &ao_riLastLoadRows, m4uint32_t &ao_riLastLoadFetchs ) const
{
	ao_riLastLoadStatus = 0 ;
	ao_riLastLoadRows = 0 ;
	ao_riLastLoadFetchs = 0 ;

	if( m_pClBlock == NULL )
	{
		return M4_SUCCESS;
	}
	m_pClBlock->GetLastLoadStatus( ao_riLastLoadStatus, ao_riLastLoadRows, ao_riLastLoadFetchs ) ;
	return M4_SUCCESS;
}

m4return_t	ClBlock_Channel_Proxy::SetLastLoadStatus( m4uint8_t ai_iLastLoadStatus, m4uint32_t ai_iLastLoadRows, m4uint32_t ai_iLastLoadFetchs )
{
	if( m_pClBlock == NULL )
	{
		if( _CreateBlock() == NULL )
		{
			return M4_ERROR;
		}
	}

	m_pClBlock->SetLastLoadStatus( ai_iLastLoadStatus, ai_iLastLoadRows, ai_iLastLoadFetchs ) ;
	return M4_SUCCESS;
}

m4uint64_t ClBlock_Channel_Proxy::GetBlockTimeStamp( void ) const
{

	m4uint64_t	iTimeStamp = 0 ;


	if( m_pClBlock != NULL )
	{
		iTimeStamp = m_pClBlock->GetBlockTimeStamp() ;
	}
	else
	{
		iTimeStamp = m_oConnectedChangeListener.GetTimeStamp() ;
	}

	if( m_iTimeStamp > iTimeStamp )
	{
		iTimeStamp = m_iTimeStamp ;
	}

	return( iTimeStamp ) ;
}

m4uint64_t ClBlock_Channel_Proxy::GetRecordTimeStamp( void ) const
{

	m4uint64_t	iTimeStamp = 0 ;


	if( m_pClBlock != NULL )
	{
		iTimeStamp = m_pClBlock->GetRecordTimeStamp() ;
	}

	if( m_iTimeStamp > iTimeStamp )
	{
		iTimeStamp = m_iTimeStamp ;
	}

	return( iTimeStamp ) ;
}

m4return_t ClBlock_Channel_Proxy::GetSize(ClSize &ao_size)
{

	ao_size+=sizeof(ClBlock_Channel_Proxy);
		ao_size-=sizeof(ClBlockSearchInfo);
		ao_size-=sizeof(ID);

	if (ClBlockSearchInfo::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	if (ID.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	if (m_pClBlock){
		if (m_pClBlock->GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}

	return M4_SUCCESS;
}

m4return_t  ClBlockSearchInfo::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClBlockSearchInfo);
	return M4_SUCCESS;
}

m4return_t  StBlockId::GetSize(ClSize &ao_size)
{
    ao_size+=sizeof(StBlockId);
	return M4_SUCCESS;
}


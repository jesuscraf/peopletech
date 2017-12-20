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

#include "dm_def.hpp"
#include "m4define.hpp"
#include "recorset.hpp"
#include "px_block.hpp"
#include "access.hpp"
#include "node_knl.hpp"
#include "chan_knl.hpp"
#include "chandef.hpp"
#include "access.hpp"
#include "blockref.hpp"


void ClAccessRecordSet::Attach (ClNode_Data *ai_pClNode_Data/*, ClNotifier_Channel *ai_pnotifier*/)
{
    _ClRecordSet_Base::Attach (ai_pClNode_Data);
        
    Current.Attach (ai_pClNode_Data);
    Register.Attach (ai_pClNode_Data);
}

void ClAccessRecordSet::Detach (void)
{
    _ClRecordSet_Base::Detach ();

    Current.Detach ();
    Register.Detach ();
}

//----------------------------------------------------------------------------
// Función muy privada, puesto que cambia el estado del access
//      devuelve true si el estado interno se modifica
//----------------------------------------------------------------------------


m4bool_t ClAccessRecordSet::SetCurrentBlock (const ClBlockReference &ai_br)
{
    m4bool_t bismodified = _ClRecordSet_Base::SetCurrentBlock (ai_br);

    if (bismodified) {
        Register.MoveToEOF ();
        Current.MoveToEOF ();

        ClNode *pnode = GetpAccess ()->Node.Get (GetpNode_ExtInfo ()->Index);
        M4_ASSERT (pnode);

        pnode->Reset ();
    }

    return bismodified;
}

//----------------------------------------------------------------------------
// además del bloque activo, tiene en cuenta el current:
//----------------------------------------------------------------------------

m4bool_t ClAccessRecordSet::IsEOF (void)
{
    m4bool_t biseof;

    if (IsValid ()) {
        biseof = Current.Index == M4DM_EOF_INDEX;
    }
    else {
        biseof = M4_FALSE;
    }

    return biseof;
}

// Function: ClAccessRecordSet::Serialize
//
// Arguments:
//    ClGenericIODriver& IOD - Where we serialize to
//
// Returns: m4return_t
//
// Description:
//   To serialize the record set we need to serialize the
//   items at the block level, store the reference to
//   current block, and serialize the register and the 
//   current register
//
// Author:  Stuart Murdoch (mailto:stuartm@meta4.es)
// Created: 10 December 1998
//
m4return_t ClAccessRecordSet::Serialize(ClGenericIODriver& IOD)
{
    m4return_t RetVal = M4_SUCCESS;
    
    //
    // Here, I can store the address of the
    // current block so that we can
    // set the current block in deserialize
    //
    IOD.StoreAddress(this);

    // Serialize the items at the block level
	/*
	Mejora de ancho de banda. No se serializa la posición del item
    RetVal = Item.Serialize(IOD);
	*/

    // Serialize the current register
	RetVal = Current.Serialize(IOD);

    // Serialize the register
    if (RetVal == M4_SUCCESS)
        RetVal = Register.Serialize(IOD);

    return RetVal;
}

//
// Function: ClAccessRecordSet::DeSerialize
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
m4return_t ClAccessRecordSet::DeSerialize(ClGenericIODriver& IOD)
{
    m4return_t RetVal = M4_SUCCESS;

    // set the current block
    IOD.StoreAddress(this);
    
    // DeSerialize the items at the block level
	/*
	Mejora de ancho de banda. No se deserializa la posición del item
    m4return_t RetVal = Item.DeSerialize(IOD);
	*/
    
    // DeSerialize the current register
	RetVal = Current.DeSerialize(IOD);
        
    // DeSerialize the register
    if (RetVal == M4_SUCCESS)
        RetVal = Register.DeSerialize(IOD);

    return RetVal;
}

/*
 * Function: ClAccessRecordSet::SendBlockMark
 *
 * Arguments:
 *      ai_bBranchMode - si es M4_TRUE se serializa todo lo que cuelga del bloque
 *
 * Description:
 *      Marca para serializar el bloque y la jerarquia de registros que lo soporta
 */
void ClAccessRecordSet::SendBlockMark(const m4bool_t ai_bBranchMode)
{
    GetCurrentBlock()->MarkBlockAndParentRecordsForSerialize(ai_bBranchMode, M4_PXB_SEND_ALL_RECORDS);
}

void ClAccessRecordSet::FollowSimple (const ClAccessRecordSet &ai_rs)
{
    _ClRecordSet_Base::FollowSimple (ai_rs);

    Current.Follow (ai_rs.Current);

        M4_ASSERT (Current.GetCurrentBlock () == ai_rs.Current.GetCurrentBlock ());
        M4_ASSERT (Current.Index == ai_rs.Current.Index);
        M4_ASSERT (Current.GetMMIndex () == ((ClAccessRecordSet &)ai_rs).Current.GetMMIndex ());

    //y ahora los registros:
    Register.Follow (ai_rs.Register);

        M4_ASSERT (Register.GetCurrentBlock () == ai_rs.Register.GetCurrentBlock ());
        M4_ASSERT (Register.Index == ai_rs.Register.Index);
        M4_ASSERT (Register.GetMMIndex () == ((ClRecordSet &)ai_rs).Register.GetMMIndex ());
}
    
void ClAccessRecordSet::_SetCurrentBlock (const ClBlockReference &ai_br)
{
    M4_ASSERT (ai_br);

    _ClRecordSet_Base::_SetCurrentBlock (ai_br);

		//cuidado, esta pone el register en EOF
    Current.Item.SetCurrentBlock (ai_br);
    Register.Item.SetCurrentBlock (ai_br);

	SetChange() ;
}

ClRegTotalize & ClAccessRecordSet::GetFooter(void)
{
    if (!m_pFooter)
    {
        m_pFooter = new ClRegTotalize;
        m_pFooter->Init (&Register);
    }

    M4_ASSERT(m_pFooter);

    return * m_pFooter;
}

m4return_t	ClAccessRecordSet::GetLastLoadStatus( m4uint8_t &ao_riLastLoadStatus, m4uint32_t &ao_riLastLoadRows, m4uint32_t &ao_riLastLoadFetchs ) const
{
	ao_riLastLoadStatus = 0 ;
	ao_riLastLoadRows = 0 ;
	ao_riLastLoadFetchs = 0 ;

	ClBlock_Channel_Proxy	*poBlock = GetCurrentBlock() ;

	if( poBlock != NULL )
	{
		return poBlock->GetLastLoadStatus( ao_riLastLoadStatus, ao_riLastLoadRows, ao_riLastLoadFetchs ) ;
	}
	return M4_SUCCESS ;
}

m4return_t	ClAccessRecordSet::SetLastLoadStatus( m4uint8_t ai_iLastLoadStatus, m4uint32_t ai_iLastLoadRows, m4uint32_t ai_iLastLoadFetchs )
{
	ClBlock_Channel_Proxy	*poBlock = GetCurrentBlock() ;

	if( poBlock != NULL )
	{
		return poBlock->SetLastLoadStatus( ai_iLastLoadStatus, ai_iLastLoadRows, ai_iLastLoadFetchs ) ;
	}
	return M4_ERROR ;
}

m4uint64_t ClAccessRecordSet::GetNodeTimeStamp( void ) const
{

	m4uint64_t	iTimeStamp = 0 ;


	if( m_pClNode_Data != NULL )
	{
		iTimeStamp = m_pClNode_Data->GetNodeTimeStamp() ;
	}

	return( iTimeStamp ) ;
}

m4uint64_t ClAccessRecordSet::GetBlockTimeStamp( void ) const
{

	m4uint64_t				iTimeStamp = 0 ;
	ClBlock_Channel_Proxy	*poBlock = NULL ;


	poBlock = GetCurrentBlock() ;

	if( poBlock != NULL )
	{
		iTimeStamp = poBlock->GetBlockTimeStamp() ;
	}

	if( m_iTimeStamp > iTimeStamp )
	{
		iTimeStamp = m_iTimeStamp ;
	}

	return( iTimeStamp ) ;
}

m4uint64_t ClAccessRecordSet::GetRecordTimeStamp( void ) const
{

	m4uint64_t				iTimeStamp = 0 ;
	ClBlock_Channel_Proxy	*poBlock = NULL ;


	poBlock = GetCurrentBlock() ;

	if( poBlock != NULL )
	{
		iTimeStamp = poBlock->GetRecordTimeStamp() ;
	}

	if( m_oFilterChangeListener.GetTimeStamp() > iTimeStamp )
	{
		iTimeStamp = m_oFilterChangeListener.GetTimeStamp() ;
	}

	if( m_iTimeStamp > iTimeStamp )
	{
		iTimeStamp = m_iTimeStamp ;
	}

	return( iTimeStamp ) ;
}

m4uint64_t ClAccessRecordSet::GetCurrentTimeStamp( void ) const
{
	return( Current.GetTimeStamp() ) ;
}

m4return_t ClAccessRecordSet::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClAccessRecordSet);
		ao_size-=sizeof(ClRecordSet);
		ao_size-=sizeof(Current);

	if (ClRecordSet::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	if (Current.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	return M4_SUCCESS;
}

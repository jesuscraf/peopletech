//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             itemdef.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             23/07/97
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

#include "itemdef.hpp"

#include "m4var.hpp"
#include "m4mdrt.hpp"

#include "item_prx.hpp"
//## end module.includes preserve=yes

///////////////////////////////////////////////////////////////////////////////
//
// ClItemDef
//

void ClItemDef::Attach (ClCompiledMCR *ai_pCMCR)
{
    m_pCMCR = ai_pCMCR;
}

void ClItemDef::Attach (const ClItemDef &ai_itemdef)
{
    m_pCMCR = ai_itemdef.m_pCMCR;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------


m4pcchar_t ClItemDef::Id (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iHandle ) ) ;
// -jcr
}


m4pcchar_t ClItemDef::TiId (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

    return( m_pCMCR->GetItemTiId( iHandle ) ) ;
// -jcr
}


m4int32_t ClItemDef::Synonym (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemSynonymId( iHandle ) ) ;
// -jcr
}


m4pcchar_t ClItemDef::Name (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

    return( m_pCMCR->GetItemName( iHandle ) ) ;
// -jcr
}


m4pcchar_t ClItemDef::Dmd (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

    return( m_pCMCR->GetItemDmd( iHandle ) ) ;
// -jcr
}


m4pcchar_t ClItemDef::DmdComponent (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

    return( m_pCMCR->GetItemDmdComponent( iHandle ) ) ;
// -jcr
}


m4pcchar_t ClItemDef::DmdCrossTab (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

    return( m_pCMCR->GetItemDmdCrosstab( iHandle ) ) ;
}


m4pcchar_t ClItemDef::DmdField (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

    return( m_pCMCR->GetItemDmdField( iHandle ) ) ;
// -jcr
}


m4int32_t ClItemDef::Precision (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemPrecision( iHandle ) ) ;
// -jcr
}


m4int32_t ClItemDef::MaxSize (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemMaxSize( iHandle ) ) ;
// -jcr
}


m4int32_t ClItemDef::RealSize (void) const
{
	m4uint8_t ItemType;
	m4uint32_t ItemSize;

    if (GetHandle () == 0) {
        return 0;
    }

	ItemType = M4Type();
	if ((ItemType == M4CL_M4_TYPE_VAR_STRING) ||
		(ItemType == M4CL_M4_TYPE_UNI_VAR_STRING) ||
		(ItemType == M4CL_M4_TYPE_LONG) ||
		(ItemType == M4CL_M4_TYPE_UNI_LONG) ||
		(ItemType == M4CL_M4_TYPE_BINARY_STRING))
	{
		ItemSize = Precision() + 1;
	}
	else {
		ItemSize = MaxSize();
	}
    return ItemSize;
}


m4return_t ClItemDef::GetItemDefaultValue(m4VariantType &ao_rvValue) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( M4_ERROR ) ;
    }

    m_pCMCR->GetItemDefaultValue( iHandle, ao_rvValue ) ;
	return M4_SUCCESS;
// -jcr
}


m4pcchar_t ClItemDef::ReadObjectAlias (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

    return( m_pCMCR->GetItemReadObjectAlias( iHandle ) ) ;
// -jcr
}


m4pcchar_t ClItemDef::WriteObjectAlias (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

    return( m_pCMCR->GetItemWriteObjectAlias( iHandle ) ) ;
// -jcr
}


m4pcchar_t ClItemDef::ReadFieldId (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

    return( m_pCMCR->GetItemReadFieldId( iHandle ) ) ;
// -jcr
}


m4pcchar_t ClItemDef::WriteFieldId (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

    return( m_pCMCR->GetItemWriteFieldId( iHandle ) ) ;
// -jcr
}


m4pcchar_t ClItemDef::ReadObjectId (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

    return( m_pCMCR->GetItemReadObjectId( iHandle ) ) ;
// -jcr
}


m4pcchar_t ClItemDef::WriteObjectId (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

    return( m_pCMCR->GetItemWriteObjectId( iHandle ) ) ;
// -jcr
}


m4int16_t ClItemDef::Index (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( -1 ) ;
    }

    return( m_pCMCR->GetItemIndex( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef::Type (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemType( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef::TheoricScope (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemTheoreticScope( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef::M4Type (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemM4Type( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef::CppType (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemCppType( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef::Scale (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemScale( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef::SliceBhrv (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemSliceBhrv( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef::SliceSplit (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemSliceSplit( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef::IdTotalize (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemIdTotalize( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef::SliceTotalize (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemSliceTotalize( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef::TransactionMode (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemTransactionMode( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef::MethodLevel (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemMethodLevel( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef::IsVariableLength (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemIsVariableLength( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef::SentTotType (void) const
{
    M4_ASSERT (m_pCMCR);

//	return m_pCMCR->GetItemSentTotType (GetHandle ());
    return( 0 ) ;
}


m4uint8_t ClItemDef::Scope (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemScope( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef :: InternalType( void ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemInternalType( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef :: ConvertFunction( void ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemConvertFunction( iHandle ) ) ;
// -jcr
}



m4uint8_t ClItemDef :: IsInherited( void ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemIsInherited( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef :: CsType( void ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemCsType( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef :: SyncType( void ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemSyncType( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef :: IsParameter( void ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemIsParameter( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef :: CheckNotNull( void ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemCheckNotNull( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef :: ClientNotNull( void ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemClientNotNull( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef :: NotNull( void ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemNotNull( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef :: AffectsDB( void ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemAffectsDB( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef :: GenerateSlices( void ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemGenerateSlices( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef :: IsPK( void ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemIsPK( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef :: IsNodalItem( void ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemIsNodal( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef :: IsEventItem( void ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemIsEvent( iHandle ) ) ;
// -jcr
}



m4uint16_t ClItemDef::NumberOfConnectedItems (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemNumberOfConnectedItems( iHandle ) ) ;
// -jcr
}


m4uint32_t ClItemDef::ConnectorHandleByPosition (m4uint16_t ai_pos) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemConnectorHandleByPosition( iHandle, ai_pos ) ) ;
// -jcr
}


m4pcchar_t ClItemDef :: ConnectorFatherNodeByPosition( m4uint16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iItemHandle ;
	m4uint32_t	iConnectorHandle ;
	m4uint32_t	iFatherHandle ;
	m4uint32_t	iNodeHandle ;


	iItemHandle =  GetHandle() ;

    if( iItemHandle == 0 )
	{
        return( "" ) ;
    }

	m4uint16_t iNumberOfConnectedItems;
	iNumberOfConnectedItems = m_pCMCR->GetItemNumberOfConnectedItems( iItemHandle );
	if(ai_iPosition >= iNumberOfConnectedItems)
		return ""; //Valor por defecto 

    iConnectorHandle = m_pCMCR->GetItemConnectorHandleByPosition( iItemHandle, ai_iPosition ) ;

    if( iConnectorHandle == 0 )
	{
        return( "" ) ;
    }

	iFatherHandle = m_pCMCR->GetItemConnectorFatherItemHandle( iConnectorHandle ) ;

    if( iFatherHandle == 0 )
	{
        return( "" ) ;
    }

	iNodeHandle = m_pCMCR->GetItemNodeHandle( iFatherHandle ) ;

    if( iNodeHandle == 0 )
	{
        return( "" ) ;
    }

	return( m_pCMCR->GetNodeId( iNodeHandle ) ) ;

}


m4pcchar_t ClItemDef :: ConnectorFatherItemByPosition( m4uint16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iItemHandle ;
	m4uint32_t	iConnectorHandle ;
	m4uint32_t	iFatherHandle ;


	iItemHandle =  GetHandle() ;

    if( iItemHandle == 0 )
	{
        return( "" ) ;
    }

	m4uint16_t iNumberOfConnectedItems;
	iNumberOfConnectedItems = m_pCMCR->GetItemNumberOfConnectedItems( iItemHandle );
	if(ai_iPosition >= iNumberOfConnectedItems)
		return ""; //Valor por defecto 

    iConnectorHandle = m_pCMCR->GetItemConnectorHandleByPosition( iItemHandle, ai_iPosition ) ;

    if( iConnectorHandle == 0 )
	{
        return( "" ) ;
    }

	iFatherHandle = m_pCMCR->GetItemConnectorFatherItemHandle( iConnectorHandle ) ;

    if( iFatherHandle == 0 )
	{
        return( "" ) ;
    }

	return( m_pCMCR->GetItemId( iFatherHandle ) ) ;

}


m4pcchar_t ClItemDef :: ConnectorSonNodeByPosition( m4uint16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iItemHandle ;
	m4uint32_t	iConnectorHandle ;
	m4uint32_t	iSonHandle ;
	m4uint32_t	iNodeHandle ;


	iItemHandle =  GetHandle() ;

    if( iItemHandle == 0 )
	{
        return( "" ) ;
    }

	m4uint16_t iNumberOfConnectedItems;
	iNumberOfConnectedItems = m_pCMCR->GetItemNumberOfConnectedItems( iItemHandle );
	if(ai_iPosition >= iNumberOfConnectedItems)
		return ""; //Valor por defecto 

    iConnectorHandle = m_pCMCR->GetItemConnectorHandleByPosition( iItemHandle, ai_iPosition ) ;

    if( iConnectorHandle == 0 )
	{
        return( "" ) ;
    }

	iSonHandle = m_pCMCR->GetItemConnectorSonItemHandle( iConnectorHandle ) ;

    if( iSonHandle == 0 )
	{
        return( "" ) ;
    }

	iNodeHandle = m_pCMCR->GetItemNodeHandle( iSonHandle ) ;

    if( iNodeHandle == 0 )
	{
        return( "" ) ;
    }

	return( m_pCMCR->GetNodeId( iNodeHandle ) ) ;

}


m4pcchar_t ClItemDef :: ConnectorSonItemByPosition( m4uint16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iItemHandle ;
	m4uint32_t	iConnectorHandle ;
	m4uint32_t	iSonHandle ;


	iItemHandle =  GetHandle() ;

    if( iItemHandle == 0 )
	{
        return( "" ) ;
    }

	m4uint16_t iNumberOfConnectedItems;
	iNumberOfConnectedItems = m_pCMCR->GetItemNumberOfConnectedItems( iItemHandle );
	if(ai_iPosition >= iNumberOfConnectedItems)
		return ""; //Valor por defecto 

    iConnectorHandle = m_pCMCR->GetItemConnectorHandleByPosition( iItemHandle, ai_iPosition ) ;

    if( iConnectorHandle == 0 )
	{
        return( "" ) ;
    }

	iSonHandle = m_pCMCR->GetItemConnectorSonItemHandle( iConnectorHandle ) ;

    if( iSonHandle == 0 )
	{
        return( "" ) ;
    }

	return( m_pCMCR->GetItemId( iSonHandle ) ) ;

}


m4uint8_t  ClItemDef :: ConnectorPrecedenceByPosition( m4uint16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iItemHandle ;
	m4uint32_t	iConnectorHandle ;


	iItemHandle =  GetHandle() ;

    if( iItemHandle == 0 )
	{
        return( 0 ) ;
    }

	m4uint16_t iNumberOfConnectedItems;
	iNumberOfConnectedItems = m_pCMCR->GetItemNumberOfConnectedItems( iItemHandle );
	if(ai_iPosition >= iNumberOfConnectedItems)
		return 0; //Valor por defecto

    iConnectorHandle = m_pCMCR->GetItemConnectorHandleByPosition( iItemHandle, ai_iPosition ) ;

    if( iConnectorHandle == 0 )
	{
        return( 0 ) ;
    }

	return( m_pCMCR->GetItemConnectorPrecedence( iConnectorHandle ) ) ;

}


m4uint8_t  ClItemDef :: ConnectorSpinByPosition( m4uint16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iItemHandle ;
	m4uint32_t	iConnectorHandle ;


	iItemHandle =  GetHandle() ;

    if( iItemHandle == 0 )
	{
        return( 0 ) ;
    }

	m4uint16_t iNumberOfConnectedItems;
	iNumberOfConnectedItems = m_pCMCR->GetItemNumberOfConnectedItems( iItemHandle );
	if(ai_iPosition >= iNumberOfConnectedItems)
		return 0; //Valor por defecto 

    iConnectorHandle = m_pCMCR->GetItemConnectorHandleByPosition( iItemHandle, ai_iPosition ) ;

    if( iConnectorHandle == 0 )
	{
        return( 0 ) ;
    }

	return( m_pCMCR->GetItemConnectorSpin( iConnectorHandle ) ) ;

}


m4uint8_t  ClItemDef :: ConnectorRelationByPosition( m4uint16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iItemHandle ;
	m4uint32_t	iConnectorHandle ;


	iItemHandle =  GetHandle() ;

    if( iItemHandle == 0 )
	{
        return( 0 ) ;
    }

	m4uint16_t iNumberOfConnectedItems;
	iNumberOfConnectedItems = m_pCMCR->GetItemNumberOfConnectedItems( iItemHandle );
	if(ai_iPosition >= iNumberOfConnectedItems)
		return 0; //Valor por defecto

    iConnectorHandle = m_pCMCR->GetItemConnectorHandleByPosition( iItemHandle, ai_iPosition ) ;

    if( iConnectorHandle == 0 )
	{
        return( 0 ) ;
    }

	return( m_pCMCR->GetItemConnectorRelation( iConnectorHandle ) ) ;

}


m4uint8_t  ClItemDef :: ConnectorContextByPosition( m4uint16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iItemHandle ;
	m4uint32_t	iConnectorHandle ;


	iItemHandle =  GetHandle() ;

    if( iItemHandle == 0 )
	{
        return( 0 ) ;
    }

	m4uint16_t iNumberOfConnectedItems;
	iNumberOfConnectedItems = m_pCMCR->GetItemNumberOfConnectedItems( iItemHandle );
	if(ai_iPosition >= iNumberOfConnectedItems)
		return 0; //Valor por defecto 

    iConnectorHandle = m_pCMCR->GetItemConnectorHandleByPosition( iItemHandle, ai_iPosition ) ;

    if( iConnectorHandle == 0 )
	{
        return( 0 ) ;
    }

	return( m_pCMCR->GetItemConnectorContext( iConnectorHandle ) ) ;

}


m4uint8_t  ClItemDef :: ConnectorCsTypeByPosition( m4uint16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iItemHandle ;
	m4uint32_t	iConnectorHandle ;


	iItemHandle =  GetHandle() ;

    if( iItemHandle == 0 )
	{
        return( 0 ) ;
    }

	m4uint16_t iNumberOfConnectedItems;
	iNumberOfConnectedItems = m_pCMCR->GetItemNumberOfConnectedItems( iItemHandle );
	if(ai_iPosition >= iNumberOfConnectedItems)
		return 0; //Valor por defecto

    iConnectorHandle = m_pCMCR->GetItemConnectorHandleByPosition( iItemHandle, ai_iPosition ) ;

    if( iConnectorHandle == 0 )
	{
        return( 0 ) ;
    }

	return( m_pCMCR->GetItemConnectorCsType( iConnectorHandle ) ) ;

}



m4uint16_t ClItemDef::NumberOfArguments (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemNumberOfArguments( iHandle ) ) ;
// -jcr
}


m4uint16_t ClItemDef::NumberOfReferedArguments (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemNumberOfReferedArguments( iHandle ) ) ;
// -jcr
}


m4uint32_t ClItemDef::NodeHandle (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemNodeHandle( iHandle ) ) ;
// -jcr
}


// ### mal, esto aquí no puede ir, es del item connector

m4uint32_t ClItemDef::ConnectorSonItemHandle (m4uint32_t ai_iItemConnectorHandle) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemConnectorSonItemHandle( iHandle ) ) ;
// -jcr
}


m4uint8_t ClItemDef::VariableArguments (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemVariableArguments( iHandle ) ) ;
// -jcr
}

 
m4int16_t
ClItemDef::Order(void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( -1 ) ;
    }

    return( m_pCMCR->GetItemOrder( iHandle ) ) ;
// -jcr
}


m4uint8_t
ClItemDef::IsVisible(void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemIsVisible( iHandle ) ) ;
// -jcr
}


m4uint8_t
ClItemDef::ModifyData(void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemModifyData( iHandle ) ) ;
// -jcr
}



m4pcchar_t ClItemDef::ArgumentIdByPosition( m4uint16_t ai_pos ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

    return( m_pCMCR->GetItemArgumentIdByPosition( iHandle, ai_pos ) ) ;
// -jcr
}



m4pcchar_t ClItemDef::ArgumentNameByPosition( m4uint16_t ai_pos ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

    return( m_pCMCR->GetItemArgumentNameByPosition( iHandle, ai_pos ) ) ;
// -jcr
}



m4uint8_t  ClItemDef::ArgumentTypeByPosition( m4uint16_t ai_pos ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemArgumentTypeByPosition( iHandle, ai_pos ) ) ;
// -jcr
}



m4uint8_t  ClItemDef::ArgumentM4TypeByPosition( m4uint16_t ai_pos ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemArgumentM4TypeByPosition( iHandle, ai_pos ) ) ;
// -jcr
}



m4uint16_t ClItemDef::ArgumentPrecisionByPosition( m4uint16_t ai_pos ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemArgumentPrecisionByPosition( iHandle, ai_pos ) ) ;
// -jcr
}


m4int16_t  ClItemDef::ArgumentPositionById( m4pcchar_t ai_pccArgumentId ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemArgumentPositionById( iHandle, ai_pccArgumentId ) ) ;
// -jcr
}



m4uint16_t ClItemDef::NumberOfConditionants (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemNumberOfConditionants( iHandle ) ) ;
// -jcr
}


m4int16_t ClItemDef::ConditionantIndexByPosition( m4uint16_t ai_pos ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iItemHandle ;
	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( -1 ) ;
    }

    iItemHandle = m_pCMCR->GetItemConditionantHandleByPosition( iHandle, ai_pos ) ;

    if( iItemHandle == 0 )
    {
        return( -1 ) ;
    }

    return( m_pCMCR->GetItemIndex( iItemHandle ) ) ;

}


m4int16_t ClItemDef::ConditionantNodeIndexByPosition( m4uint16_t ai_pos ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iItemHandle ;
    m4uint32_t  iNodeHandle ;
	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( -1 ) ;
    }

    iItemHandle = m_pCMCR->GetItemConditionantHandleByPosition( iHandle, ai_pos ) ;

    if( iItemHandle == 0 )
    {
        return( -1 ) ;
    }

    iNodeHandle = m_pCMCR->GetItemNodeHandle( iItemHandle ) ;

    if( iNodeHandle == 0 )
    {
        return( -1 ) ;
    }

    return( m_pCMCR->GetNodeIndex( iNodeHandle ) ) ;

}


m4pcchar_t ClItemDef::ConditionantIdByPosition( m4uint16_t ai_pos ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iItemHandle ;
	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

    iItemHandle = m_pCMCR->GetItemConditionantHandleByPosition( iHandle, ai_pos ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t ClItemDef::ConditionantNodeIdByPosition( m4uint16_t ai_pos ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iItemHandle ;
    m4uint32_t  iNodeHandle ;
	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }


    iItemHandle = m_pCMCR->GetItemConditionantHandleByPosition( iHandle, ai_pos);

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    iNodeHandle = m_pCMCR->GetItemNodeHandle( iItemHandle ) ;

    if( iNodeHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetNodeId( iNodeHandle ) ) ;
}


m4uint16_t ClItemDef::NumberOfDependents (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemNumberOfDependents( iHandle ) ) ;
// -jcr
}


m4int16_t ClItemDef::DependentIndexByPosition( m4uint16_t ai_pos ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iItemHandle ;
	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( -1 ) ;
    }

	m4uint16_t iNumberOfDependents;
	iNumberOfDependents = m_pCMCR->GetItemNumberOfDependents( iHandle );
	if(ai_pos >= iNumberOfDependents)
		return -1; //Valor por defecto

    iItemHandle = m_pCMCR->GetItemDependentHandleByPosition( iHandle, ai_pos ) ;

    if( iItemHandle == 0 )
    {
        return( -1 ) ;
    }

    return( m_pCMCR->GetItemIndex( iItemHandle ) ) ;

}


m4int16_t ClItemDef::DependentNodeIndexByPosition( m4uint16_t ai_pos ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iItemHandle ;
    m4uint32_t  iNodeHandle ;
	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( -1 ) ;
    }

	m4uint16_t iNumberOfDependents;
	iNumberOfDependents = m_pCMCR->GetItemNumberOfDependents( iHandle );
	if(ai_pos >= iNumberOfDependents)
		return -1; //Valor por defecto 

    iItemHandle = m_pCMCR->GetItemDependentHandleByPosition( iHandle, ai_pos ) ;

    if( iItemHandle == 0 )
    {
        return( -1 ) ;
    }

    iNodeHandle = m_pCMCR->GetItemNodeHandle( iItemHandle ) ;

    if( iNodeHandle == 0 )
    {
        return( -1 ) ;
    }

    return( m_pCMCR->GetNodeIndex( iNodeHandle ) ) ;

}


m4pcchar_t ClItemDef::DependentIdByPosition( m4uint16_t ai_pos ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iItemHandle ;
	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

	m4uint16_t iNumberOfDependents;
	iNumberOfDependents = m_pCMCR->GetItemNumberOfDependents( iHandle );
	if(ai_pos >= iNumberOfDependents)
		return ""; //Valor por defecto 

    iItemHandle = m_pCMCR->GetItemDependentHandleByPosition( iHandle, ai_pos ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t ClItemDef::DependentNodeIdByPosition( m4uint16_t ai_pos ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iItemHandle ;
    m4uint32_t  iNodeHandle ;
	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

	m4uint16_t iNumberOfDependents;
	iNumberOfDependents = m_pCMCR->GetItemNumberOfDependents( iHandle );
	if(ai_pos >= iNumberOfDependents)
		return ""; //Valor por defecto

    iItemHandle = m_pCMCR->GetItemDependentHandleByPosition( iHandle, ai_pos ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    iNodeHandle = m_pCMCR->GetItemNodeHandle( iItemHandle ) ;

    if( iNodeHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetNodeId( iNodeHandle ) ) ;
}


m4pcchar_t  ClItemDef :: SliceItem( void ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

    m4uint32_t iItemHandle = m_pCMCR->GetItemSliceItem( iHandle ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
// -jcr
}


m4pcchar_t  ClItemDef :: AuxiliarItem( void ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

    m4uint32_t iItemHandle = m_pCMCR->GetItemAuxItem( iHandle ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
// -jcr
}


m4pcchar_t  ClItemDef :: AuxiliarItemProrat( void ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

    m4uint32_t iItemHandle = m_pCMCR->GetItemAuxItemProrat( iHandle ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
// -jcr
}


m4uint16_t  ClItemDef :: SliceItemIndex( void ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( -1 ) ;
    }

    m4uint32_t iItemHandle = m_pCMCR->GetItemSliceItem( iHandle ) ;

    if( iItemHandle == 0 )
    {
        return(-1) ;
    }

    return( m_pCMCR->GetItemIndex( iItemHandle ) ) ;
// -jcr
}


m4uint16_t  ClItemDef :: AuxiliarItemIndex ( void ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( -1 ) ;
    }

    m4uint32_t iItemHandle = m_pCMCR->GetItemAuxItem( iHandle ) ;

    if( iItemHandle == 0 )
    {
        return(-1) ;
    }

    return( m_pCMCR->GetItemIndex( iItemHandle ) ) ;
// -jcr
}


m4uint16_t  ClItemDef :: AuxiliarItemProratIndex ( void ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( -1 ) ;
    }

    m4uint32_t iItemHandle = m_pCMCR->GetItemAuxItemProrat( iHandle ) ;

    if( iItemHandle == 0 )
    {
        return(-1) ;
    }

    return( m_pCMCR->GetItemIndex( iItemHandle ) ) ;
// -jcr
}


m4bool_t	ClItemDef :: IsConnected( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( M4_FALSE ) ;
    }

    m4uint32_t iConnectorHandle = m_pCMCR->GetItemInverseConnector( iHandle ) ;

    if( iConnectorHandle == 0 )
    {
        return( M4_FALSE ) ;
    }

    return( M4_TRUE ) ;
}


m4pcchar_t  ClItemDef :: FatherItem( void ) const
{

// jesus
// ojo, aquí tengo en cuenta que de momento sólo hay un item padre
// Al igual que lo tengo en cuenta en el metacanal

// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

    m4uint32_t iConnectorHandle = m_pCMCR->GetItemInverseConnector( iHandle ) ;

    if( iConnectorHandle == 0 || iConnectorHandle == m4uint32_t( -1 ) )
    {
        return( "" ) ;
    }

    m4uint32_t iItemHandle = m_pCMCR->GetItemConnectorFatherItemHandle( iConnectorHandle ) ; 

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
// -jcr
}


m4pcchar_t  ClItemDef :: FatherItemNode( void ) const
{

// jesus
// ojo, aquí tengo en cuenta que de momento sólo hay un item padre
// Al igual que lo tengo en cuenta en el metacanal

// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

    m4uint32_t iConnectorHandle = m_pCMCR->GetItemInverseConnector( iHandle ) ;

    if( iConnectorHandle == 0 || iConnectorHandle == m4uint32_t( -1 ) )
    {
        return( "" ) ;
    }

    m4uint32_t iItemHandle = m_pCMCR->GetItemConnectorFatherItemHandle( iConnectorHandle ) ; 
    m4uint32_t iNodeHandle = m_pCMCR->GetItemNodeHandle( iItemHandle ) ; 

    return( m_pCMCR->GetNodeId( iNodeHandle ) ) ;
// -jcr
}




// mas cosas

m4uint16_t ClItemDef :: NumberOfAssigned( void ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return m_pCMCR->GetItemNumberOfAssigned( iHandle ) ;
// -jcr
}


m4int16_t ClItemDef :: AssignedIndex( m4uint16_t ai_AssignedPosition ) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iItemHandle ;
	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( -1 ) ;
    }

    iItemHandle = m_pCMCR->GetItemAssignedHandleByPosition( iHandle, ai_AssignedPosition ) ;

    if( iItemHandle == 0 )
    {
        return( -1 ) ;
    }

    return( m_pCMCR->GetItemIndex( iItemHandle ) ) ;
// -jcr
}


m4int16_t ClItemDef :: AssignedNodeIndex( m4uint16_t ai_AssignedPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iItemHandle ;
    m4uint32_t  iNodeHandle ;
	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( -1 ) ;
    }

    iItemHandle = m_pCMCR->GetItemAssignedHandleByPosition( iHandle, ai_AssignedPosition ) ;

    if( iItemHandle == 0 )
    {
        return( -1 ) ;
    }

    iNodeHandle = m_pCMCR->GetItemNodeHandle( iItemHandle ) ;

    if( iNodeHandle == 0 )
    {
        return( -1 ) ;
    }

    return( m_pCMCR->GetNodeIndex( iNodeHandle ) ) ;

}


m4pcchar_t ClItemDef :: AssignedId( m4uint16_t ai_AssignedPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iItemHandle ;
	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

    iItemHandle = m_pCMCR->GetItemAssignedHandleByPosition( iHandle, ai_AssignedPosition ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t ClItemDef :: AssignedNodeId( m4uint16_t ai_AssignedPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iItemHandle ;
    m4uint32_t  iNodeHandle ;
	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

    iItemHandle = m_pCMCR->GetItemAssignedHandleByPosition( iHandle, ai_AssignedPosition ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    iNodeHandle = m_pCMCR->GetItemNodeHandle( iItemHandle ) ;

    if( iNodeHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetNodeId( iNodeHandle ) ) ;
}

m4uint16_t ClItemDef::NumberOfAssignedDependents (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemNumberOfAssignedDependents( iHandle ) ) ;
// -jcr
}


m4int16_t ClItemDef::AssignedDependentIndex( m4uint16_t ai_pos ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iItemHandle ;
	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( -1 ) ;
    }

	m4uint16_t iNumberOfAssignedDependents;
	iNumberOfAssignedDependents = m_pCMCR->GetItemNumberOfAssignedDependents( iHandle );
	if(ai_pos >= iNumberOfAssignedDependents)
		return -1; //Valor por defecto

    iItemHandle = m_pCMCR->GetItemAssignedDependentHandleByPosition( iHandle, ai_pos ) ;

    if( iItemHandle == 0 )
    {
        return( -1 ) ;
    }

    return( m_pCMCR->GetItemIndex( iItemHandle ) ) ;

}


m4int16_t ClItemDef::AssignedDependentNodeIndex( m4uint16_t ai_pos ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iItemHandle ;
    m4uint32_t  iNodeHandle ;
	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( -1 ) ;
    }

	m4uint16_t iNumberOfAssignedDependents;
	iNumberOfAssignedDependents = m_pCMCR->GetItemNumberOfAssignedDependents( iHandle );
	if(ai_pos >= iNumberOfAssignedDependents)
		return -1; //Valor por defecto 

    iItemHandle = m_pCMCR->GetItemAssignedDependentHandleByPosition( iHandle, ai_pos ) ;

    if( iItemHandle == 0 )
    {
        return( -1 ) ;
    }

    iNodeHandle = m_pCMCR->GetItemNodeHandle( iItemHandle ) ;

    if( iNodeHandle == 0 )
    {
        return( -1 ) ;
    }

    return( m_pCMCR->GetNodeIndex( iNodeHandle ) ) ;

}


m4pcchar_t ClItemDef::AssignedDependentId( m4uint16_t ai_pos ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iItemHandle ;
	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

	m4uint16_t iNumberOfAssignedDependents;
	iNumberOfAssignedDependents = m_pCMCR->GetItemNumberOfAssignedDependents( iHandle );
	if(ai_pos >= iNumberOfAssignedDependents)
		return ""; //Valor por defecto 

    iItemHandle = m_pCMCR->GetItemAssignedDependentHandleByPosition( iHandle, ai_pos ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t ClItemDef::AssignedDependentNodeId( m4uint16_t ai_pos ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iItemHandle ;
    m4uint32_t  iNodeHandle ;
	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

	m4uint16_t iNumberOfAssignedDependents;
	iNumberOfAssignedDependents = m_pCMCR->GetItemNumberOfAssignedDependents( iHandle );
	if(ai_pos >= iNumberOfAssignedDependents)
		return ""; //Valor por defecto

    iItemHandle = m_pCMCR->GetItemAssignedDependentHandleByPosition( iHandle, ai_pos ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    iNodeHandle = m_pCMCR->GetItemNodeHandle( iItemHandle ) ;

    if( iNodeHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetNodeId( iNodeHandle ) ) ;
}


m4uint16_t ClItemDef :: NumberOfExecuted( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }
    return m_pCMCR->GetItemNumberOfExecuted( iHandle ) ;
}


m4int16_t ClItemDef :: ExecutedIndex( m4uint16_t ai_ExecutedPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iItemHandle ;
	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( -1 ) ;
    }

    iItemHandle = m_pCMCR->GetItemExecutedHandleByPosition( iHandle, ai_ExecutedPosition ) ;

    if( iItemHandle == 0 )
    {
        return( -1 ) ;
    }

    return( m_pCMCR->GetItemIndex( iItemHandle ) ) ;
}


m4int16_t ClItemDef :: ExecutedNodeIndex( m4uint16_t ai_ExecutedPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iItemHandle ;
    m4uint32_t  iNodeHandle ;
	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( -1 ) ;
    }

    iItemHandle = m_pCMCR->GetItemExecutedHandleByPosition( iHandle, ai_ExecutedPosition ) ;

    if( iItemHandle == 0 )
    {
        return( -1 ) ;
    }

    iNodeHandle = m_pCMCR->GetItemNodeHandle( iItemHandle ) ;

    if( iNodeHandle == 0 )
    {
        return( -1 ) ;
    }

    return( m_pCMCR->GetNodeIndex( iNodeHandle ) ) ;

}


m4pcchar_t ClItemDef :: ExecutedId( m4uint16_t ai_ExecutedPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iItemHandle ;
	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

    iItemHandle = m_pCMCR->GetItemExecutedHandleByPosition( iHandle, ai_ExecutedPosition ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t ClItemDef :: ExecutedNodeId( m4uint16_t ai_ExecutedPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iItemHandle ;
    m4uint32_t  iNodeHandle ;
	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

    iItemHandle = m_pCMCR->GetItemExecutedHandleByPosition( iHandle, ai_ExecutedPosition ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    iNodeHandle = m_pCMCR->GetItemNodeHandle( iItemHandle ) ;

    if( iNodeHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetNodeId( iNodeHandle ) ) ;
}


m4uint16_t ClItemDef::NumberOfExecutedDependents (void) const
{
// +jcr
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemNumberOfExecutedDependents( iHandle ) ) ;
// -jcr
}


m4int16_t ClItemDef::ExecutedDependentIndex( m4uint16_t ai_pos ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iItemHandle ;
	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( -1 ) ;
    }

	m4uint16_t iNumberOfExecutedDependents;
	iNumberOfExecutedDependents = m_pCMCR->GetItemNumberOfExecutedDependents( iHandle );
	if(ai_pos >= iNumberOfExecutedDependents)
		return -1; //Valor por defecto

    iItemHandle = m_pCMCR->GetItemExecutedDependentHandleByPosition( iHandle, ai_pos ) ;

    if( iItemHandle == 0 )
    {
        return( -1 ) ;
    }

    return( m_pCMCR->GetItemIndex( iItemHandle ) ) ;

}


m4int16_t ClItemDef::ExecutedDependentNodeIndex( m4uint16_t ai_pos ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iItemHandle ;
    m4uint32_t  iNodeHandle ;
	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( -1 ) ;
    }

	m4uint16_t iNumberOfExecutedDependents;
	iNumberOfExecutedDependents = m_pCMCR->GetItemNumberOfExecutedDependents( iHandle );
	if(ai_pos >= iNumberOfExecutedDependents)
		return -1; //Valor por defecto 

    iItemHandle = m_pCMCR->GetItemExecutedDependentHandleByPosition( iHandle, ai_pos ) ;

    if( iItemHandle == 0 )
    {
        return( -1 ) ;
    }

    iNodeHandle = m_pCMCR->GetItemNodeHandle( iItemHandle ) ;

    if( iNodeHandle == 0 )
    {
        return( -1 ) ;
    }

    return( m_pCMCR->GetNodeIndex( iNodeHandle ) ) ;

}


m4pcchar_t ClItemDef::ExecutedDependentId( m4uint16_t ai_pos ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iItemHandle ;
	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

	m4uint16_t iNumberOfExecutedDependents;
	iNumberOfExecutedDependents = m_pCMCR->GetItemNumberOfExecutedDependents( iHandle );
	if(ai_pos >= iNumberOfExecutedDependents)
		return ""; //Valor por defecto 

    iItemHandle = m_pCMCR->GetItemExecutedDependentHandleByPosition( iHandle, ai_pos ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t ClItemDef::ExecutedDependentNodeId( m4uint16_t ai_pos ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iItemHandle ;
    m4uint32_t  iNodeHandle ;
	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

	m4uint16_t iNumberOfExecutedDependents;
	iNumberOfExecutedDependents = m_pCMCR->GetItemNumberOfExecutedDependents( iHandle );
	if(ai_pos >= iNumberOfExecutedDependents)
		return ""; //Valor por defecto

    iItemHandle = m_pCMCR->GetItemExecutedDependentHandleByPosition( iHandle, ai_pos ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    iNodeHandle = m_pCMCR->GetItemNodeHandle( iItemHandle ) ;

    if( iNodeHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetNodeId( iNodeHandle ) ) ;
}


// Para el tratamiento de fórmulas y código

m4uint16_t  ClItemDef :: NumberOfFormIds( void ) const
{

    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemNumberOfFormIds( iHandle ) ) ;
}


m4pcchar_t  ClItemDef :: FormId( m4uint16_t ai_iFormIdPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

    return( m_pCMCR->GetItemFormIdByPosition( iHandle, ai_iFormIdPosition ) ) ;
}



m4int16_t	ClItemDef :: MaxCodes( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

    return( m_pCMCR->GetItemNumberOfSlices( iHandle ) * m_pCMCR->GetItemNumberOfFormulas( iHandle ) ) ;
}


m4return_t	ClItemDef :: GetNextCode( m4int16_t &ai_rioNext ) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        ai_rioNext = MaxCodes() ;
        return( M4_ERROR ) ;
    }

	m4pcchar_t	pccCode = m_pCMCR->GetItemNextCode( iHandle, ai_rioNext ) ;

    return( M4_SUCCESS ) ;
}


m4pcchar_t	ClItemDef :: CodeRuleId( m4int16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

	if( ai_iPosition >= MaxCodes() )
	{
        return( "" ) ;
	}

	m4uint16_t	iSlices = m_pCMCR->GetItemNumberOfSlices( iHandle ) ;

	if( iSlices == 0 )
	{
        return( "" ) ;
	}

	m4uint16_t	iFormula = ai_iPosition / iSlices ;

    return( m_pCMCR->GetItemFormIdByPosition( iHandle, iFormula ) ) ;
}


m4date_t	ClItemDef :: CodeStartDate( m4int16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( M4CL_MINUS_INFINITE_IN_JULIAN ) ;
    }

	if( ai_iPosition >= MaxCodes() )
	{
        return( M4CL_MINUS_INFINITE_IN_JULIAN ) ;
	}

	m4uint16_t	iSlices = m_pCMCR->GetItemNumberOfSlices( iHandle ) ;

	if( iSlices == 0 )
	{
        return( M4CL_MINUS_INFINITE_IN_JULIAN ) ;
	}

	m4uint16_t	iFormula = ai_iPosition / iSlices ;
	m4uint16_t	iSlice = ai_iPosition % iSlices ;
	m4pcchar_t	pccCode = m_pCMCR->GetItemCodeByPosition( iHandle, iFormula, iSlice ) ;

	if( pccCode == NULL )
	{
        return( M4CL_MINUS_INFINITE_IN_JULIAN ) ;
	}

    return( m_pCMCR->GetCodeStartDate( pccCode ) ) ;
}


m4date_t	ClItemDef :: CodeEndDate( m4int16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( M4CL_PLUS_INFINITE_IN_JULIAN ) ;
    }

	if( ai_iPosition >= MaxCodes() )
	{
        return( M4CL_PLUS_INFINITE_IN_JULIAN ) ;
	}

	m4uint16_t	iSlices = m_pCMCR->GetItemNumberOfSlices( iHandle ) ;

	if( iSlices == 0 )
	{
        return( M4CL_PLUS_INFINITE_IN_JULIAN ) ;
	}

	m4uint16_t	iFormula = ai_iPosition / iSlices ;
	m4uint16_t	iSlice = ai_iPosition % iSlices ;
	m4pcchar_t	pccCode = m_pCMCR->GetItemCodeByPosition( iHandle, iFormula, iSlice ) ;

	if( pccCode == NULL )
	{
        return( M4CL_PLUS_INFINITE_IN_JULIAN ) ;
	}

    return( m_pCMCR->GetCodeEndDate( pccCode ) ) ;
}


m4pcchar_t	ClItemDef :: CodeRuleTiId( m4int16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

	if( ai_iPosition >= MaxCodes() )
	{
        return( "" ) ;
	}

	m4uint16_t	iSlices = m_pCMCR->GetItemNumberOfSlices( iHandle ) ;

	if( iSlices == 0 )
	{
        return( "" ) ;
	}

	m4uint16_t	iFormula = ai_iPosition / iSlices ;
	m4uint16_t	iSlice = ai_iPosition % iSlices ;
	m4pcchar_t	pccCode = m_pCMCR->GetItemCodeByPosition( iHandle, iFormula, iSlice ) ;

	if( pccCode == NULL )
	{
        return( "" ) ;
	}

    return( m_pCMCR->GetCodeRuleTiId( pccCode ) ) ;
}


m4uint32_t	ClItemDef :: CodeNumberOfVariales( m4int16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

	if( ai_iPosition >= MaxCodes() )
	{
        return( 0 ) ;
	}

	m4uint16_t	iSlices = m_pCMCR->GetItemNumberOfSlices( iHandle ) ;

	if( iSlices == 0 )
	{
        return( 0 ) ;
	}

	m4uint16_t	iFormula = ai_iPosition / iSlices ;
	m4uint16_t	iSlice = ai_iPosition % iSlices ;
	m4pcchar_t	pccCode = m_pCMCR->GetItemCodeByPosition( iHandle, iFormula, iSlice ) ;

	if( pccCode == NULL )
	{
        return( 0 ) ;
	}

    return( m_pCMCR->GetCodeNumberOfVariales( pccCode ) ) ;
}


m4uint32_t	ClItemDef :: CodeNumberOfInstructions( m4int16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

	if( ai_iPosition >= MaxCodes() )
	{
        return( 0 ) ;
	}

	m4uint16_t	iSlices = m_pCMCR->GetItemNumberOfSlices( iHandle ) ;

	if( iSlices == 0 )
	{
        return( 0 ) ;
	}

	m4uint16_t	iFormula = ai_iPosition / iSlices ;
	m4uint16_t	iSlice = ai_iPosition % iSlices ;
	m4pcchar_t	pccCode = m_pCMCR->GetItemCodeByPosition( iHandle, iFormula, iSlice ) ;

	if( pccCode == NULL )
	{
        return( 0 ) ;
	}

    return( m_pCMCR->GetCodeNumberOfInstructions( pccCode ) ) ;
}


m4uint32_t	ClItemDef :: CodeSize( m4int16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

	if( ai_iPosition >= MaxCodes() )
	{
        return( 0 ) ;
	}

	m4uint16_t	iSlices = m_pCMCR->GetItemNumberOfSlices( iHandle ) ;

	if( iSlices == 0 )
	{
        return( 0 ) ;
	}

	m4uint16_t	iFormula = ai_iPosition / iSlices ;
	m4uint16_t	iSlice = ai_iPosition % iSlices ;
	m4pcchar_t	pccCode = m_pCMCR->GetItemCodeByPosition( iHandle, iFormula, iSlice ) ;

	if( pccCode == NULL )
	{
        return( 0 ) ;
	}

    return( m_pCMCR->GetCodeSize( pccCode ) ) ;
}


m4uint8_t	ClItemDef :: CodeType( m4int16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( M4CL_NO_LENGUAJE ) ;
    }

	if( ai_iPosition >= MaxCodes() )
	{
        return( M4CL_NO_LENGUAJE ) ;
	}

	m4uint16_t	iSlices = m_pCMCR->GetItemNumberOfSlices( iHandle ) ;

	if( iSlices == 0 )
	{
        return( M4CL_NO_LENGUAJE ) ;
	}

	m4uint16_t	iFormula = ai_iPosition / iSlices ;
	m4uint16_t	iSlice = ai_iPosition % iSlices ;
	m4pcchar_t	pccCode = m_pCMCR->GetItemCodeByPosition( iHandle, iFormula, iSlice ) ;

	if( pccCode == NULL )
	{
        return( M4CL_NO_LENGUAJE ) ;
	}

    return( m_pCMCR->GetCodeType( pccCode ) ) ;
}


m4uint8_t	ClItemDef :: CodeMustAssign( m4int16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 1 ) ;
    }

	if( ai_iPosition >= MaxCodes() )
	{
        return( 1 ) ;
	}

	m4uint16_t	iSlices = m_pCMCR->GetItemNumberOfSlices( iHandle ) ;

	if( iSlices == 0 )
	{
        return( 1 ) ;
	}

	m4uint16_t	iFormula = ai_iPosition / iSlices ;
	m4uint16_t	iSlice = ai_iPosition % iSlices ;
	m4pcchar_t	pccCode = m_pCMCR->GetItemCodeByPosition( iHandle, iFormula, iSlice ) ;

	if( pccCode == NULL )
	{
        return( 1 ) ;
	}

    return( m_pCMCR->GetCodeMustAssign( pccCode ) ) ;
}


m4uint8_t	ClItemDef :: CodePriority( m4int16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( M4CL_RULE_MAX_PRIORITY ) ;
    }

	if( ai_iPosition >= MaxCodes() )
	{
        return( M4CL_RULE_MAX_PRIORITY ) ;
	}

	m4uint16_t	iSlices = m_pCMCR->GetItemNumberOfSlices( iHandle ) ;

	if( iSlices == 0 )
	{
        return( M4CL_RULE_MAX_PRIORITY ) ;
	}

	m4uint16_t	iFormula = ai_iPosition / iSlices ;
	m4uint16_t	iSlice = ai_iPosition % iSlices ;
	m4pcchar_t	pccCode = m_pCMCR->GetItemCodeByPosition( iHandle, iFormula, iSlice ) ;

	if( pccCode == NULL )
	{
        return( M4CL_RULE_MAX_PRIORITY ) ;
	}

    return( m_pCMCR->GetCodePriority( pccCode ) ) ;
}


m4uint8_t	ClItemDef :: CodeAutoGenerated( m4int16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( 0 ) ;
    }

	if( ai_iPosition >= MaxCodes() )
	{
        return( 0 ) ;
	}

	m4uint16_t	iSlices = m_pCMCR->GetItemNumberOfSlices( iHandle ) ;

	if( iSlices == 0 )
	{
        return( 0 ) ;
	}

	m4uint16_t	iFormula = ai_iPosition / iSlices ;
	m4uint16_t	iSlice = ai_iPosition % iSlices ;
	m4pcchar_t	pccCode = m_pCMCR->GetItemCodeByPosition( iHandle, iFormula, iSlice ) ;

	if( pccCode == NULL )
	{
        return( 0 ) ;
	}

    return( m_pCMCR->GetCodeAutoGenerated( pccCode ) ) ;
}


m4return_t	ClItemDef :: CodeSource( m4int16_t ai_iPosition, ClLongString &ao_roString ) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( M4_ERROR ) ;
    }

	if( ai_iPosition >= MaxCodes() )
	{
        return( M4_ERROR ) ;
	}

	m4uint16_t	iSlices = m_pCMCR->GetItemNumberOfSlices( iHandle ) ;

	if( iSlices == 0 )
	{
        return( M4_ERROR ) ;
	}

	m4uint16_t	iFormula = ai_iPosition / iSlices ;
	m4uint16_t	iSlice = ai_iPosition % iSlices ;
	m4pcchar_t	pccCode = m_pCMCR->GetItemCodeByPosition( iHandle, iFormula, iSlice ) ;

	if( pccCode == NULL )
	{
        return( M4_ERROR ) ;
	}

	// Si no es autogenerado no tiene código fuente
	m4uint8_t	iAutoGenerated = m_pCMCR->GetCodeAutoGenerated( pccCode ) ;

	if( iAutoGenerated == 0 )
	{
        return( M4_ERROR ) ;
	}

    return( m_pCMCR->GetCodeSource( pccCode, iHandle, iFormula, ao_roString, M4MDRT_CODE_MODE_PLAIN, 0 ) ) ;
}






m4uint8_t   ClItemDef :: SecRead( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return m_pCMCR->GetItemSecRead( GetHandle() ) ;
}


m4uint8_t   ClItemDef :: SecWrite( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return m_pCMCR->GetItemSecWrite( GetHandle() ) ;
}


m4uint8_t   ClItemDef :: SecDelete( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return m_pCMCR->GetItemSecDelete( GetHandle() ) ;
}


m4uint8_t   ClItemDef :: SecUpdate( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return m_pCMCR->GetItemSecUpdate( GetHandle() ) ;
}


m4uint8_t   ClItemDef :: SecExecute( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return m_pCMCR->GetItemSecExecute( GetHandle() ) ;
}


m4uint8_t   ClItemDef :: SecContinue( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return m_pCMCR->GetItemSecContinue( GetHandle() ) ;
}



m4uint8_t   ClItemDef :: IsPublic( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return m_pCMCR->GetItemIsPublic( GetHandle() ) ;
}






/********************************************************************
 * Funciones cor retorno de errores en caso de items en EOF
 *********************************************************************/

m4return_t ClItemDef::Precision (m4int32_t &ao_precc) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( M4_ERROR ) ;
    }

    ao_precc = m_pCMCR->GetItemPrecision( iHandle ) ;
	return M4_SUCCESS;
}


m4return_t ClItemDef::MaxSize (m4int32_t &ao_maxsize) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( M4_ERROR ) ;
    }

    ao_maxsize = m_pCMCR->GetItemMaxSize( iHandle ) ;
	return M4_SUCCESS;
}


m4return_t ClItemDef::RealSize (m4int32_t &ao_realsize) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint8_t	ItemType;
	m4uint32_t	ItemSize;
	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( M4_ERROR ) ;
    }


	ItemType = M4Type();
	if ((ItemType == M4CL_M4_TYPE_VAR_STRING) ||
		(ItemType == M4CL_M4_TYPE_UNI_VAR_STRING) ||
		(ItemType == M4CL_M4_TYPE_LONG) ||
		(ItemType == M4CL_M4_TYPE_UNI_LONG) ||
		(ItemType == M4CL_M4_TYPE_BINARY_STRING))
	{
		ItemSize = Precision() + 1;
	}
	else {
		ItemSize = MaxSize();
	}
    ao_realsize = ItemSize;
	return M4_SUCCESS;
}


m4return_t ClItemDef::SliceBhrv (m4uint8_t &ao_slibhrv) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( M4_ERROR ) ;
    }

    ao_slibhrv = m_pCMCR->GetItemSliceBhrv( iHandle ) ;
	return M4_SUCCESS;
}


m4return_t ClItemDef::CppType (m4uint8_t &ao_cpp) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( M4_ERROR ) ;
    }

    ao_cpp = m_pCMCR->GetItemCppType( iHandle ) ;
	return M4_SUCCESS;
}


m4return_t ClItemDef::SliceTotalize (m4uint8_t &ao_slitot) const
{
    M4_ASSERT( m_pCMCR ) ;

	m4uint32_t	iHandle = GetHandle() ;

	if( iHandle == 0 )
	{
        return( M4_ERROR ) ;
    }

    ao_slitot = m_pCMCR->GetItemSliceTotalize( iHandle ) ;
	return M4_SUCCESS;
}


m4return_t ClItemDef::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClItemDef);
		ao_size-=sizeof(m_Handle);
	if (m_Handle.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	return M4_SUCCESS;
}

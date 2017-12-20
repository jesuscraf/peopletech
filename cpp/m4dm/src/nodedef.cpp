//## begin module.includes preserve=yes
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             nodedef.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             09/08/97
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

//#include "itemdef.hpp"
#include "m4mdrt.hpp"

#include "nodedef.hpp"
//#include "channel.hpp"

//## end module.includes preserve=yes
ClNodeDef::ClNodeDef ()
{
//## begin ClNodeDef::ClNodeDef%1456443714.body preserve=yes
    m_pCMCR = 0;
    Handle = 0;    
//## end ClNodeDef::ClNodeDef%1456443714.body
}

/*
ClNodeDef::ClNodeDef (const ClNodeDef &ai_itemdef)
{
//## begin ClNodeDef::ClNodeDef%2094799619.body preserve=yes
    _Copy (ai_itemdef);
//## end ClNodeDef::ClNodeDef%2094799619.body
}

ClNodeDef &ClNodeDef::operator = (const ClNodeDef &ai_itemdef)
{
//## begin ClNodeDef::operator =%789440525.body preserve=yes
    _Copy (ai_itemdef);

    return *this;
//## end ClNodeDef::operator =%789440525.body
}

void ClNodeDef::_Copy (const ClNodeDef &ai_nodedef)
{
//## begin ClNodeDef::_Copy%110045179.body preserve=yes
    m_pCMCR = ai_nodedef.m_pCMCR;
    Handle = ai_nodedef.Handle;    
//## end ClNodeDef::_Copy%110045179.body
}
*/

void ClNodeDef::Attach (ClCompiledMCR *ai_pcmcr, ClNodeIndex ai_index/*, ClRTItemIterator *ai_pClRTItemIterator*/)
{
    m_pCMCR = ai_pcmcr;

    m4uint16_t pos;
    M4_ASSERT (ai_index < 0x10000);//Para que el casting de abajo no de problemas
    pos = (m4uint16_t)ai_index;

    Handle = m_pCMCR->GetChannelNodeHandleByPosition (pos);
}

ClNodeDef::~ClNodeDef ()
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------



m4pcchar_t ClNodeDef::Id (void) const
{
//## begin ClNodeDef::Id%-99237752.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeId (Handle);
//## end ClNodeDef::Id%-99237752.body
}

m4pcchar_t ClNodeDef::Name(void) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeName(Handle);
}

m4pcchar_t ClNodeDef::TiId (void) const
{
//## begin ClNodeDef::TiId%-417743003.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeTiId (Handle);
//## end ClNodeDef::TiId%-417743003.body
}

m4pcchar_t ClNodeDef::M4ObjId (void) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeM4ObjId (Handle);
}

m4pcchar_t ClNodeDef::ParentId (void) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

	m4uint8_t	iType = 0 ;
    m4uint16_t	iLength = 0 ;
    m4uint16_t	i = 0 ;
    m4uint32_t	iConnector = 0 ;
    m4uint32_t	iParent = 0 ;

    iLength = m_pCMCR->GetNodeNumberOfInvConnectors( Handle ) ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iConnector = m_pCMCR->GetNodeInvConnectorHandleByPosition( Handle, i ) ;
		iType = m_pCMCR->GetNodeConnectorType( iConnector ) ;

		if( iType == M4CL_NODE_CONNECTOR_TYPE_RB || iType == M4CL_NODE_CONNECTOR_TYPE_BB )
		{
			iParent = m_pCMCR->GetNodeConnectorFatherNodeHandle( iConnector ) ;
			return( m_pCMCR->GetNodeId( iParent ) ) ;
		}
	}

    return( "" ) ;
}


m4pcchar_t ClNodeDef::GroupObjects (void) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeGroupObjects (Handle);
}


m4pcchar_t ClNodeDef::SentenceGroupObjects (void) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    m4uint32_t	iSentence = m_pCMCR->GetNodeReadSentenceHandle( Handle ) ;

	if( iSentence == 0 )
	{
		return( "" ) ;
	}

	return( m_pCMCR->GetSentenceGroupObjects( iSentence ) ) ;
}


m4pcchar_t ClNodeDef::ReadSentenceApiSql (void) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    m4uint32_t	iSentence = m_pCMCR->GetNodeReadSentenceHandle( Handle ) ;

	if( iSentence == 0 )
	{
		return( "" ) ;
	}

	return( m_pCMCR->GetSentenceApiSql( iSentence ) ) ;
}


m4pcchar_t
ClNodeDef::StartDateItem (void) const
{
//## begin ClNodeDef::StartDateItem%-492795218.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    m4uint32_t itemHandle =
		m_pCMCR->GetNodeStartDateItem(Handle);

    if( itemHandle == 0 )
    {
        return( "" ) ;
    }

	return m_pCMCR->GetItemId(itemHandle);

	//## end ClNodeDef::StartDateItem%-492795218.body
}

m4pcchar_t
ClNodeDef::EndDateItem (void) const
{
//## begin ClNodeDef::EndDateItem%2026213709.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    m4uint32_t itemHandle =
		m_pCMCR->GetNodeEndDateItem(Handle);

    if( itemHandle == 0 )
    {
        return( "" ) ;
    }

	return m_pCMCR->GetItemId(itemHandle);

//## end ClNodeDef::EndDateItem%2026213709.body
}

m4uint32_t ClNodeDef::NumRows (void) const
{
//## begin ClNodeDef::NumRows%1112613777.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeNumRows (Handle);
//## end ClNodeDef::NumRows%1112613777.body
}

m4uint32_t ClNodeDef::NumRowsDB (void) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeNumRowsDB(Handle);
}

m4int32_t ClNodeDef::NumKeepRows (void) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeNumKeepRows (Handle);
}


m4uint16_t ClNodeDef::NumberOfItems (void) const
{
//## begin ClNodeDef::NumberOfItems%-1077603439.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeNumberOfItems (Handle);
//## end ClNodeDef::NumberOfItems%-1077603439.body
}


m4uint16_t ClNodeDef::NumberOfNodeItems (void) const
{
//## begin ClNodeDef::NumberOfNodeItems%-1794462518.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeNumberOfNodeItems (Handle);
//## end ClNodeDef::NumberOfNodeItems%-1794462518.body
}

m4uint16_t ClNodeDef::NumberOfBlockItems (void) const
{
//## begin ClNodeDef::NumberOfBlockItems%-1145825674.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeNumberOfBlockItems (Handle);
//## end ClNodeDef::NumberOfBlockItems%-1145825674.body
}

m4uint16_t ClNodeDef::NumberOfRegisterItems (void) const
{
//## begin ClNodeDef::NumberOfRegisterItems%1996747206.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeNumberOfRegisterItems (Handle);
//## end ClNodeDef::NumberOfRegisterItems%1996747206.body
}



m4uint16_t ClNodeDef::Index (void) const
{
//## begin ClNodeDef::Index%1842323967.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeIndex (Handle);
//## end ClNodeDef::Index%1842323967.body
}


m4uint16_t ClNodeDef::NodeType (void) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return( m_pCMCR->GetNodeType( Handle ) ) ;
}

m4uint16_t ClNodeDef::NodeOrder (void) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return( m_pCMCR->GetNodeOrder( Handle ) ) ;
}

m4uint8_t ClNodeDef::Capacity (void) const
{
//## begin ClNodeDef::Capacity%-1811581857.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeCapacity (Handle);
//## end ClNodeDef::Capacity%-1811581857.body
}

m4uint8_t ClNodeDef::IsRoot (void) const
{
//## begin ClNodeDef::IsRoot%-1449669356.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeIsRoot (Handle);
//## end ClNodeDef::IsRoot%-1449669356.body
}

m4uint8_t ClNodeDef::IsRealRoot (void) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeIsRealRoot (Handle);
}

m4uint8_t ClNodeDef::AutoLoad (void) const
{
//## begin ClNodeDef::AutoLoad%1822858022.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeAutoLoad (Handle);
//## end ClNodeDef::AutoLoad%1822858022.body
}

m4uint8_t ClNodeDef::IsOrdered (void) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeIsOrdered (Handle);
}

m4uint8_t ClNodeDef::IsVisible(void) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeIsVisible(Handle);
}

m4uint8_t ClNodeDef :: DynFilter( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    return( m_pCMCR->GetNodeDynFilter( Handle ) ) ;
}


m4uint8_t ClNodeDef::Temporality (void) const
{
//## begin ClNodeDef::Temporality%-588036616.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeTemporality (Handle);
//## end ClNodeDef::Temporality%-588036616.body
}

m4uint8_t ClNodeDef::Completeness (void) const
{
//## begin ClNodeDef::Completeness%-234374460.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeCompleteness (Handle);
//## end ClNodeDef::Completeness%-234374460.body
}

m4uint8_t ClNodeDef::PeriodCorrected (void) const
{
//## begin ClNodeDef::PeriodCorrected%1925926450.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodePeriodCorrected (Handle);
//## end ClNodeDef::PeriodCorrected%1925926450.body
}


m4bool_t ClNodeDef::IsPartialLoad (void) const
{
	return M4_FALSE;
}

m4uint32_t ClNodeDef::MaxReg (void) const
{
	return 10;
}

m4uint32_t ClNodeDef::RegToConserve (void) const
{
	return 3;
}

m4uint16_t ClNodeDef::GetNodeNumberOfInvConnectors (void) const
{
//## begin ClNodeDef::GetNodeNumberOfInvConnectors%1259470821.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeNumberOfInvConnectors (Handle);
//## end ClNodeDef::GetNodeNumberOfInvConnectors%1259470821.body
}

m4uint32_t ClNodeDef::GetNodeInvConnectorHandleByPosition (m4uint16_t ai_iInvConnectedPosition) const
{
//## begin ClNodeDef::GetNodeInvConnectorHandleByPosition%-43730071.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeInvConnectorHandleByPosition (Handle, ai_iInvConnectedPosition);
//## end ClNodeDef::GetNodeInvConnectorHandleByPosition%-43730071.body
}

m4uint8_t ClNodeDef::GetNodeConnectorType (m4uint32_t ai_iNodeConnectorHandle) const
{
//## begin ClNodeDef::GetNodeConnectorType%-1321011989.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeConnectorType (ai_iNodeConnectorHandle);
//## end ClNodeDef::GetNodeConnectorType%-1321011989.body
}

m4uint32_t ClNodeDef::GetNodeConnectorFatherNodeHandle (m4uint32_t ai_iNodeConnectorHandle) const
{
//## begin ClNodeDef::GetNodeConnectorFatherNodeHandle%-533328544.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeConnectorFatherNodeHandle (ai_iNodeConnectorHandle);
//## end ClNodeDef::GetNodeConnectorFatherNodeHandle%-533328544.body
}

m4uint32_t ClNodeDef::GetNodeConnectorHandleByPosition (m4uint16_t ai_iConnectedPosition) const
{
//## begin ClNodeDef::GetNodeConnectorHandleByPosition%-983396599.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeConnectorHandleByPosition (Handle, ai_iConnectedPosition);
//## end ClNodeDef::GetNodeConnectorHandleByPosition%-983396599.body
}

m4uint32_t ClNodeDef::GetNodeConnectorSonNodeHandle ( m4uint32_t ai_iNodeConnectorHandle ) const
{
//## begin ClNodeDef::GetNodeConnectorSonNodeHandle%-905183770.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeConnectorSonNodeHandle (ai_iNodeConnectorHandle);
//## end ClNodeDef::GetNodeConnectorSonNodeHandle%-905183770.body
}

m4uint16_t ClNodeDef::GetNodeNumberOfRBConnectors (void) const
{
//## begin ClNodeDef::GetNodeNumberOfRBConnectors%-643625450.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeNumberOfRBConnectors (Handle);
//## end ClNodeDef::GetNodeNumberOfRBConnectors%-643625450.body
}

m4uint16_t ClNodeDef::GetNodeRBFirstConnector (void) const
{
//## begin ClNodeDef::GetNodeRBFirstConnector%1514610280.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeRBFirstConnector (Handle);
//## end ClNodeDef::GetNodeRBFirstConnector%1514610280.body
}

m4uint16_t ClNodeDef::GetNodeNumberOfBBConnectors (void) const
{
//## begin ClNodeDef::GetNodeNumberOfBBConnectors%-1634687535.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeNumberOfBBConnectors (Handle);
//## end ClNodeDef::GetNodeNumberOfBBConnectors%-1634687535.body
}

m4uint16_t ClNodeDef::GetNodeBBFirstConnector (void) const
{
//## begin ClNodeDef::GetNodeBBFirstConnector%1309186387.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeBBFirstConnector (Handle);
//## end ClNodeDef::GetNodeBBFirstConnector%1309186387.body
}

m4uint16_t ClNodeDef::GetNodeNumberOfNRConnectors (void) const
{
//## begin ClNodeDef::GetNodeNumberOfNRConnectors%-1634687535.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeNumberOfNRConnectors (Handle);
//## end ClNodeDef::GetNodeNumberOfNRConnectors%-1634687535.body
}

m4uint16_t ClNodeDef::GetNodeNRFirstConnector (void) const
{
//## begin ClNodeDef::GetNodeNRFirstConnector%1309186387.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeNRFirstConnector (Handle);
//## end ClNodeDef::GetNodeNRFirstConnector%1309186387.body
}

m4uint8_t ClNodeDef::GetNodeConnectorExpandDelete ( m4uint32_t ai_iNodeConnectorHandle ) const
{
//## begin ClNodeDef::GetNodeBBFirstConnector%1309186387.body preserve=yes
    M4_ASSERT (m_pCMCR);

    return m_pCMCR->GetNodeConnectorExpandDelete (ai_iNodeConnectorHandle);
//## end ClNodeDef::GetNodeBBFirstConnector%1309186387.body
}

m4uint8_t ClNodeDef::GetNodeConnectorExpandUndo ( m4uint32_t ai_iNodeConnectorHandle ) const
{
//## begin ClNodeDef::GetNodeBBFirstConnector%1309186387.body preserve=yes
    M4_ASSERT (m_pCMCR);

    return m_pCMCR->GetNodeConnectorExpandUndo (ai_iNodeConnectorHandle);
//## end ClNodeDef::GetNodeBBFirstConnector%1309186387.body
}


m4uint16_t  ClNodeDef :: GetNumberOfGroups( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    return( m_pCMCR->GetNodeNumberOfGroups( Handle ) ) ;
}


m4pcchar_t  ClNodeDef :: GetGroupIdByPosition( m4uint16_t ai_iGroupPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

	m4uint16_t iNumberOfGroups;
	iNumberOfGroups = m_pCMCR->GetNodeNumberOfGroups( Handle );
	if(ai_iGroupPosition >= iNumberOfGroups)
		return ""; //Valor por defecto

    return( m_pCMCR->GetNodeGroupIdByPosition( Handle, ai_iGroupPosition ) ) ;
}


m4uint16_t  ClNodeDef :: GetGroupNumberOfItems( m4uint16_t ai_iGroupPosition ) const
{

    m4uint32_t  iGroupHandle ;


    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

	m4uint16_t iNumberOfGroups;
	iNumberOfGroups = m_pCMCR->GetNodeNumberOfGroups( Handle );
	if(ai_iGroupPosition >= iNumberOfGroups)
		return 0; //Valor por defecto

    iGroupHandle = m_pCMCR->GetNodeGroupHandleByPosition( Handle, ai_iGroupPosition ) ;

	if(iGroupHandle == 0)
		return 0; //Valor por defecto

    return( m_pCMCR->GetGroupNumberOfItems( iGroupHandle ) ) ;
}


m4pcchar_t  ClNodeDef :: GetGroupItemIdByPosition( m4uint16_t ai_iGroupPosition, m4uint16_t ai_iItemPosition ) const
{

    m4uint32_t  iGroupHandle ;
    m4uint32_t  iItemHandle ;


    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;


	m4uint16_t iNumberOfGroups;
	iNumberOfGroups = m_pCMCR->GetNodeNumberOfGroups( Handle );
	if(ai_iGroupPosition >= iNumberOfGroups)
		return ""; //Valor por defecto

    iGroupHandle = m_pCMCR->GetNodeGroupHandleByPosition( Handle, ai_iGroupPosition ) ;

	m4uint16_t iItemsNumberOfGroup;
	iItemsNumberOfGroup = m_pCMCR->GetGroupNumberOfItems( iGroupHandle );
	if(ai_iItemPosition >= iItemsNumberOfGroup)
		return ""; //Valor por defecto

    iItemHandle = m_pCMCR->GetGroupItemHandleByPosition( iGroupHandle, ai_iItemPosition ) ;

	if (iItemHandle == 0)
		return "";

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}




m4uint16_t ClNodeDef::GetRBChildPos (ClHandle ai_handle)
{
//## begin ClNodeDef::GetRBChildPos%593644563.body preserve=yes
    m4uint16_t i;
    m4uint16_t pos;
    m4uint16_t nrb;
    m4bool_t bfound = M4_FALSE;

    nrb = GetNodeNumberOfRBConnectors ();
    pos = GetNodeRBFirstConnector ();

    for (i = 0; i < nrb; i++) {
        ClHandle hc = GetNodeConnectorHandleByPosition (pos);
        ClHandle hb = GetNodeConnectorSonNodeHandle (hc);

        if (hb == ai_handle) {
            bfound = M4_TRUE;
            break;
        }

        pos++;
    }

    if (!bfound) {
        i = (m4uint16_t)-1;
    }

    return i;
//## end ClNodeDef::GetRBChildPos%593644563.body
}

m4uint16_t ClNodeDef::GetBBChildPos (ClHandle ai_hnode)
{
//## begin ClNodeDef::GetBBChildPos%384220129.body preserve=yes
    m4uint16_t i;
    m4uint16_t pos;
    m4uint16_t nrb;
    m4bool_t bfound = M4_FALSE;

    nrb = GetNodeNumberOfBBConnectors ();
    pos = GetNodeBBFirstConnector ();

    for (i = 0; i < nrb; i++) {
        ClHandle hc = GetNodeConnectorHandleByPosition (pos);
        ClHandle hb = GetNodeConnectorSonNodeHandle (hc);

        if (hb == ai_hnode) {
            bfound = M4_TRUE;
            break;
        }

        pos++;
    }

    if (!bfound) {
        i = (m4uint16_t)-1;
    }

    return i;
//## end ClNodeDef::GetBBChildPos%384220129.body
}


m4uint32_t ClNodeDef::GetNodeItemHandle (m4uint16_t ai_iItemPosition) const
{
//## begin ClNodeDef::GetNodeItemHandle%-1046000060.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeItemHandleByPosition (Handle, ai_iItemPosition);
//## end ClNodeDef::GetNodeItemHandle%-1046000060.body
}

m4uint32_t ClNodeDef::GetNodeItemHandle (m4pcchar_t ai_pccItemId) const
{
//## begin ClNodeDef::GetNodeItemHandle%-886307915.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeItemHandleById (Handle, ai_pccItemId);
//## end ClNodeDef::GetNodeItemHandle%-886307915.body
}



m4uint16_t ClNodeDef::GetNodeItemType (m4uint16_t ai_iItemPosition) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    ClHandle hitem = m_pCMCR->GetNodeItemHandleByPosition (Handle, ai_iItemPosition);

    return m_pCMCR->GetItemType (hitem);
}

m4uint16_t ClNodeDef::GetNodeItemScope (m4uint16_t ai_iItemPosition) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    ClHandle hitem = m_pCMCR->GetNodeItemHandleByPosition (Handle, ai_iItemPosition);

    return m_pCMCR->GetItemScope (hitem);
}

m4uint8_t  ClNodeDef::GetNodeItemInternalType (m4uint16_t ai_iItemPosition) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    ClHandle hitem = m_pCMCR->GetNodeItemHandleByPosition (Handle, ai_iItemPosition);

    return m_pCMCR->GetItemInternalType (hitem);
}

m4int16_t ClNodeDef::GetItemIndex (m4uint32_t ai_iItemHandle) const
{
    M4_ASSERT (m_pCMCR);

    return m_pCMCR->GetItemIndex (ai_iItemHandle);
}

m4pcchar_t  ClNodeDef::StartCorDateItem( void ) const
{
//## begin ClNodeDef::StartDateItem%-492795218.body preserve=yes
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    m4uint32_t itemHandle =
		m_pCMCR->GetNodeStartCorDateItem(Handle);

    if( itemHandle == 0 )
    {
        return( "" ) ;
    }

    return m_pCMCR->GetItemId(itemHandle);

//## end ClNodeDef::StartDateItem%-492795218.body
}


m4pcchar_t  ClNodeDef::EndCorDateItem( void ) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    m4uint32_t itemHandle =
		m_pCMCR->GetNodeEndCorDateItem(Handle);

    if( itemHandle == 0 )
    {
        return( "" ) ;
    }

    return m_pCMCR->GetItemId(itemHandle);

}


m4pcchar_t  ClNodeDef::VirtualFlagItem( void ) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    m4uint32_t itemHandle =
		m_pCMCR->GetNodeVirtualFlagItem(Handle);

    if( itemHandle == 0 )
    {
        return( "" ) ;
    }

    return m_pCMCR->GetItemId(itemHandle);

}


m4pcchar_t  ClNodeDef :: FilterStartDateItem( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    m4uint32_t iItemHandle = m_pCMCR->GetNodeFilterStartDateItem( Handle ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t  ClNodeDef :: FilterEndDateItem( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    m4uint32_t iItemHandle = m_pCMCR->GetNodeFilterEndDateItem( Handle ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t  ClNodeDef :: FilterStartCorDateItem( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    m4uint32_t iItemHandle = m_pCMCR->GetNodeFilterStartCorDateItem( Handle ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t  ClNodeDef :: FilterEndCorDateItem( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    m4uint32_t iItemHandle = m_pCMCR->GetNodeFilterEndCorDateItem( Handle ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t  ClNodeDef :: CurrencyTypeItem( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    m4uint32_t iItemHandle = m_pCMCR->GetNodeCurrencyTypeItem( Handle ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t  ClNodeDef :: CurrencyExchDateItem( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    m4uint32_t iItemHandle = m_pCMCR->GetNodeCurrencyExchDateItem( Handle ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t  ClNodeDef :: ChangeReasonItem( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    m4uint32_t iItemHandle = m_pCMCR->GetNodeChangeReasonItem( Handle ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t  ClNodeDef :: DmdComponentItem( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    m4uint32_t iItemHandle = m_pCMCR->GetNodeDmdComponentItem( Handle ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t  ClNodeDef :: DmdValueItem( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    m4uint32_t iItemHandle = m_pCMCR->GetNodeDmdValueItem( Handle ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t  ClNodeDef :: LoadSQLItem( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    m4uint32_t iItemHandle = m_pCMCR->GetNodeLoadSQLItem( Handle ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}




m4uint16_t
ClNodeDef::GetNumberOfPlugs(void) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

	return(m_pCMCR->GetNodeNumberOfPlugs(Handle));
}


m4pcchar_t
ClNodeDef::GetPlugFatherChanneIdByPosition(
	m4uint16_t ai_iPlugPosition) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

	return(m_pCMCR->
		GetNodePlugFatherChanneIdByPosition(Handle,
		ai_iPlugPosition));
}


m4pcchar_t
ClNodeDef::GetPlugFatherNodeIdByPosition(
	m4uint16_t ai_iPlugPosition) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

	return(m_pCMCR->
		GetNodePlugFatherNodeIdByPosition(Handle,
		ai_iPlugPosition));
}

m4uint8_t  ClNodeDef::GetPlugTypeByPosition(m4uint16_t ai_iPlugPosition) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    m4uint32_t  iPlugHandle ;

    iPlugHandle = m_pCMCR->GetNodePlugHandleByPosition( Handle, ai_iPlugPosition ) ;

	if(iPlugHandle == 0)
		return 0; //Valor por defecto

    return( m_pCMCR->GetPlugType( iPlugHandle ) ) ;
}

m4uint8_t  ClNodeDef::GetPlugIsMultiselectionByPosition(m4uint16_t ai_iPlugPosition) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    m4uint32_t  iPlugHandle ;

    iPlugHandle = m_pCMCR->GetNodePlugHandleByPosition( Handle, ai_iPlugPosition ) ;

	if(iPlugHandle == 0)
		return 0; //Valor por defecto

    return( m_pCMCR->GetPlugIsMultiselection( iPlugHandle ) ) ;
}


m4uint8_t  ClNodeDef::GetPlugIsFilterByPosition(m4uint16_t ai_iPlugPosition) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    m4uint32_t  iPlugHandle ;

    iPlugHandle = m_pCMCR->GetNodePlugHandleByPosition( Handle, ai_iPlugPosition ) ;

	if(iPlugHandle == 0)
		return 0; //Valor por defecto

    return( m_pCMCR->GetPlugIsFilter( iPlugHandle ) ) ;
}


m4uint8_t  ClNodeDef::PlugRelationTypeId(m4uint16_t ai_iPlugPosition) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    m4uint32_t  iPlugHandle ;

    iPlugHandle = m_pCMCR->GetNodePlugHandleByPosition( Handle, ai_iPlugPosition ) ;

	if(iPlugHandle == 0)
		return 0; //Valor por defecto

    return( m_pCMCR->GetPlugRelationTypeId( iPlugHandle ) ) ;
}


m4uint8_t  ClNodeDef::PlugIsApplicable(m4uint16_t ai_iPlugPosition) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    m4uint32_t  iPlugHandle ;

    iPlugHandle = m_pCMCR->GetNodePlugHandleByPosition( Handle, ai_iPlugPosition ) ;

	if(iPlugHandle == 0)
		return 0; //Valor por defecto

    return( m_pCMCR->GetPlugIsApplicable( iPlugHandle ) ) ;
}


m4uint16_t  ClNodeDef::PlugNumberOfItems(m4uint16_t ai_iPlugPosition) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    m4uint32_t  iPlugHandle ;

    iPlugHandle = m_pCMCR->GetNodePlugHandleByPosition( Handle, ai_iPlugPosition ) ;

	if(iPlugHandle == 0)
		return 0; //Valor por defecto

    return( m_pCMCR->GetPlugNumberOfItems( iPlugHandle ) ) ;
}


m4pcchar_t  ClNodeDef::PlugSonItem(m4uint16_t ai_iPlugPosition, m4uint16_t ai_iItemPosition) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    m4uint32_t  iPlugHandle ;
    m4uint32_t  iPlugItemHandle ;
    m4uint32_t  iItemHandle ;


    iPlugHandle = m_pCMCR->GetNodePlugHandleByPosition( Handle, ai_iPlugPosition ) ;

	if(iPlugHandle == 0)
		return "";

	iPlugItemHandle = m_pCMCR->GetPlugItemHandleByPosition( iPlugHandle, ai_iItemPosition ) ;
	iItemHandle = m_pCMCR->GetPlugItemSonItemHandle( iPlugItemHandle ) ;

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t  ClNodeDef::PlugFatherItem(m4uint16_t ai_iPlugPosition, m4uint16_t ai_iItemPosition) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    m4uint32_t  iPlugHandle ;
    m4uint32_t  iPlugItemHandle ;


    iPlugHandle = m_pCMCR->GetNodePlugHandleByPosition( Handle, ai_iPlugPosition ) ;

	if (iPlugHandle == 0)
		return "";

	iPlugItemHandle = m_pCMCR->GetPlugItemHandleByPosition( iPlugHandle, ai_iItemPosition ) ;

    return( m_pCMCR->GetPlugItemFatherItemId( iPlugItemHandle ) ) ;
}


m4uint8_t  ClNodeDef::PlugItemRelation(m4uint16_t ai_iPlugPosition, m4uint16_t ai_iItemPosition) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    m4uint32_t  iPlugHandle ;
    m4uint32_t  iPlugItemHandle ;


    iPlugHandle = m_pCMCR->GetNodePlugHandleByPosition( Handle, ai_iPlugPosition ) ;

	if(iPlugHandle == 0)
		return 0; //Valor por defecto

	iPlugItemHandle = m_pCMCR->GetPlugItemHandleByPosition( iPlugHandle, ai_iItemPosition ) ;

    return( m_pCMCR->GetPlugItemRelation( iPlugItemHandle ) ) ;
}



m4uint32_t ClNodeDef::GetStartDateItem (void) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeStartDateItem (Handle);
}

m4uint32_t ClNodeDef::GetEndDateItem (void) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

	return m_pCMCR->GetNodeEndDateItem (Handle);
}

m4pcchar_t
ClNodeDef::GetNodeReadSentenceId(void) const
{
	M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

	m4pcchar_t pccSentence = m_pCMCR->GetNodeReadSentenceId(Handle);

	if (!pccSentence) {
		return ("");
	}

	return (pccSentence);
}

m4pcchar_t
ClNodeDef::GetNodeWriteSentenceId(void) const
{
	M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

	m4pcchar_t pccSentence = m_pCMCR->GetNodeWriteSentenceId(Handle);

	if (!pccSentence) {
		return ("");
	}

	return (pccSentence);
}

m4pcchar_t
ClNodeDef::GetNodeReadObjectId(void) const
{
	M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

	return m_pCMCR->GetNodeReadObjectId(Handle);
}

m4pcchar_t
ClNodeDef::GetNodeWriteObjectId(void) const
{
	M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

	return m_pCMCR->GetNodeWriteObjectId(Handle);
}

m4uint16_t
ClNodeDef::NumberOfConnectors(void) const
{
	M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

	return m_pCMCR->GetNodeNumberOfConnectors(Handle);

}

m4uint16_t
ClNodeDef::NumberOfRBConnectors(void) const
{
	M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

	return m_pCMCR->GetNodeNumberOfRBConnectors(Handle);
}

m4uint16_t
ClNodeDef::NumberOfBBConnectors(void) const
{
	M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

	return m_pCMCR->GetNodeNumberOfBBConnectors(Handle);
}

m4uint16_t
ClNodeDef::GetNodeNumberOfAutoConnectors (void) const
{
	M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

	return m_pCMCR->GetNodeNumberOfAutoConnectors(Handle);
}

m4uint16_t
ClNodeDef::NumberOfNRConnectors(void) const
{
	M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

	return m_pCMCR->GetNodeNumberOfNRConnectors(Handle);
}

m4uint16_t
ClNodeDef::RBFirstConnector(void) const
{
	M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

	return m_pCMCR->GetNodeRBFirstConnector(Handle);
}

m4uint16_t
ClNodeDef::BBFirstConnector(void) const
{
	M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

	return m_pCMCR->GetNodeBBFirstConnector(Handle);
}

m4uint16_t
ClNodeDef::NRFirstConnector(void) const
{
	M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

	return m_pCMCR->GetNodeNRFirstConnector(Handle);
}

m4uint32_t
ClNodeDef::GetConnectorHandleByPosition(m4uint16_t ai_iConnectedPosition) const
{
	M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);
    
	m4uint16_t iNumberOfConnectors;

	iNumberOfConnectors = m_pCMCR->GetNodeNumberOfConnectors(Handle);

	//Tener en cuenta el autoconector que siempre existe y no entra en la cuenta, por eso en el if es > y no >=
	if(ai_iConnectedPosition > iNumberOfConnectors)
		return 0;

    return m_pCMCR->GetNodeConnectorHandleByPosition(Handle, ai_iConnectedPosition);
}




m4uint32_t   ClNodeDef :: GetChangeReasonItemIndex (void) const{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;
    
    m4uint32_t hItem;
    hItem = m_pCMCR->GetNodeChangeReasonItem( Handle ) ;
    if (hItem==0){
        return -1;
    }
    else{
        return m_pCMCR->GetItemIndex (hItem);
    }
    
}

m4uint32_t   ClNodeDef :: GetFilterStartDateItemIndex (void) const{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;
    
    m4uint32_t hItem;
    
    hItem =  m_pCMCR->GetNodeFilterStartDateItem( Handle ) ;
    if (hItem==0){
        return -1;
    }
    else{
        return m_pCMCR->GetItemIndex (hItem);
    }
    
}

m4uint32_t   ClNodeDef :: GetFilterEndDateItemIndex  (void) const{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;
    
    m4uint32_t hItem;
    
    hItem = m_pCMCR->GetNodeFilterEndDateItem( Handle ) ;
    
    if (hItem==0){
        return -1;
    }
    else{
        return m_pCMCR->GetItemIndex (hItem);
    }
}


m4uint32_t   ClNodeDef :: GetDmdComponentItemIndex  (void) const{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;
    
    m4uint32_t hItem;
    
    hItem = m_pCMCR->GetNodeDmdComponentItem( Handle ) ;
    
    if (hItem==0){
        return -1;
    }
    else{
        return m_pCMCR->GetItemIndex (hItem);
    }
}


// mas cosas

m4pcchar_t  ClNodeDef :: PriorityItem( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    m4uint32_t iItemHandle = m_pCMCR->GetNodePriorityItem( Handle ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t  ClNodeDef :: ImputeDateItem( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    m4uint32_t iItemHandle = m_pCMCR->GetNodeImputeDateItem( Handle ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t  ClNodeDef :: PayDateItem( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    m4uint32_t iItemHandle = m_pCMCR->GetNodePayDateItem( Handle ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t  ClNodeDef :: PayTypeItem( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    m4uint32_t iItemHandle = m_pCMCR->GetNodePayTypeItem( Handle ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t  ClNodeDef :: PayFrequencyItem( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    m4uint32_t iItemHandle = m_pCMCR->GetNodePayFrequencyItem( Handle ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t  ClNodeDef :: ImputePayTypeItem( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    m4uint32_t iItemHandle = m_pCMCR->GetNodeImputePayTypeItem( Handle ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t  ClNodeDef :: ImputePayFrequencyItem( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    m4uint32_t iItemHandle = m_pCMCR->GetNodeImputePayFrequencyItem( Handle ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t  ClNodeDef :: ClosingDateItem( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    m4uint32_t iItemHandle = m_pCMCR->GetNodeClosingDateItem( Handle ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t  ClNodeDef :: SysParamsItem( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    m4uint32_t iItemHandle = m_pCMCR->GetNodeSysParamsItem( Handle ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t  ClNodeDef :: BDLChecksItem( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    m4uint32_t iItemHandle = m_pCMCR->GetNodeBDLChecksItem( Handle ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t  ClNodeDef :: RAMOrderByItem( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    m4uint32_t iItemHandle = m_pCMCR->GetNodeRAMOrderByItem( Handle ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4pcchar_t  ClNodeDef :: OrganizationIdItem( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    m4uint32_t iItemHandle = m_pCMCR->GetNodeOrganizationIdItem( Handle ) ;

    if( iItemHandle == 0 )
    {
        return( "" ) ;
    }

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}




m4uint16_t  ClNodeDef :: NumberOfSysSentences( void ) const
{
	M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

	return( m_pCMCR->GetNodeNumberOfSysSentences( Handle ) ) ;
}


m4pcchar_t  ClNodeDef :: SysSentenceNode( m4uint16_t ai_iPosition ) const
{
	M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    m4uint32_t  iItem = m_pCMCR->GetNodeSysSentenceHandleByPosition( Handle, ai_iPosition ) ;

	if(iItem == 0)
		return "";

	m4uint32_t  iNode = m_pCMCR->GetItemNodeHandle( iItem ) ;

    return( m_pCMCR->GetNodeId( iNode ) ) ;
}


m4pcchar_t  ClNodeDef :: SysSentenceItem( m4uint16_t ai_iPosition ) const
{
	M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    m4uint32_t  iItem = m_pCMCR->GetNodeSysSentenceHandleByPosition( Handle, ai_iPosition ) ;

	if(iItem == 0)
		return "";

    return( m_pCMCR->GetItemId( iItem ) ) ;
}




m4uint8_t ClNodeDef :: IdAutoFilter( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    return( m_pCMCR->GetNodeIdAutoFilter( Handle ) ) ;
}


m4uint8_t ClNodeDef :: CsType( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    return( m_pCMCR->GetNodeCsType( Handle ) ) ;
}


m4uint8_t ClNodeDef :: SyncType( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    return( m_pCMCR->GetNodeSyncType( Handle ) ) ;
}


m4uint32_t  ClNodeDef :: DefaultIndex( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    return( m_pCMCR->GetNodeDefaultIndex( Handle ) ) ;
}



m4uint16_t  ClNodeDef :: NumberOfIndexes( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    return( m_pCMCR->GetNodeNumberOfIndexes( Handle ) ) ;
}


m4uint32_t  ClNodeDef :: IndexId( m4uint16_t ai_iIndexPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

	m4uint16_t iNumberOfIndexes;

	iNumberOfIndexes = m_pCMCR->GetNodeNumberOfIndexes( Handle );

	if(ai_iIndexPosition >= iNumberOfIndexes)
		return 0; //Valor por defecto

    return( m_pCMCR->GetNodeIndexIdByPosition( Handle, ai_iIndexPosition ) ) ;
}


m4uint16_t  ClNodeDef :: IndexNumberOfItems( m4uint16_t ai_iIndexPosition ) const
{

    m4uint32_t  iIndexHandle ;


    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

	m4uint16_t iNumberOfIndexes;

	iNumberOfIndexes = m_pCMCR->GetNodeNumberOfIndexes( Handle );

	if(ai_iIndexPosition >= iNumberOfIndexes)
		return 0; //Valor por defecto

    iIndexHandle = m_pCMCR->GetNodeIndexHandleByPosition( Handle, ai_iIndexPosition ) ;

    return( m_pCMCR->GetIndexNumberOfItems( iIndexHandle ) ) ;
}


m4pcchar_t  ClNodeDef :: IndexItemId( m4uint16_t ai_iIndexPosition, m4uint16_t ai_iItemPosition ) const
{

    m4uint32_t  iIndexHandle ;
    m4uint32_t  iItemHandle ;


    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

	m4uint16_t iNumberOfIndexes, iNumberOfItems;

	iNumberOfIndexes = m_pCMCR->GetNodeNumberOfIndexes( Handle );
	if(ai_iIndexPosition >= iNumberOfIndexes)
		return ""; //Valor por defecto

    iIndexHandle = m_pCMCR->GetNodeIndexHandleByPosition( Handle, ai_iIndexPosition ) ;

	iNumberOfItems = m_pCMCR->GetIndexNumberOfItems( iIndexHandle );
	if(ai_iItemPosition >= iNumberOfItems)
		return ""; //Valor por defecto

    iItemHandle = m_pCMCR->GetIndexItemHandleByPosition( iIndexHandle, ai_iItemPosition ) ;

	if (iItemHandle == 0)
		return ""; //Valor por defecto

    return( m_pCMCR->GetItemId( iItemHandle ) ) ;
}


m4uint8_t   ClNodeDef :: IndexItemFunction( m4uint16_t ai_iIndexPosition, m4uint16_t ai_iItemPosition ) const
{

    m4uint32_t  iIndexHandle ;


    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

	m4uint16_t iNumberOfIndexes, iNumberOfItems;

	iNumberOfIndexes = m_pCMCR->GetNodeNumberOfIndexes( Handle );
	if(ai_iIndexPosition >= iNumberOfIndexes)
		return 0; //Valor por defecto

    iIndexHandle = m_pCMCR->GetNodeIndexHandleByPosition( Handle, ai_iIndexPosition ) ;

	iNumberOfItems = m_pCMCR->GetIndexNumberOfItems( iIndexHandle );
	if(ai_iItemPosition >= iNumberOfItems)
		return 0; //Valor por defecto

    return( m_pCMCR->GetIndexFunctionByPosition( iIndexHandle, ai_iItemPosition ) ) ;
}


m4uint8_t   ClNodeDef :: IndexItemWay( m4uint16_t ai_iIndexPosition, m4uint16_t ai_iItemPosition ) const
{

    m4uint32_t  iIndexHandle ;


    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

	m4uint16_t iNumberOfIndexes, iNumberOfItems;

	iNumberOfIndexes = m_pCMCR->GetNodeNumberOfIndexes( Handle );
	if(ai_iIndexPosition >= iNumberOfIndexes)
		return 0; //Valor por defecto

    iIndexHandle = m_pCMCR->GetNodeIndexHandleByPosition( Handle, ai_iIndexPosition ) ;

	iNumberOfItems = m_pCMCR->GetIndexNumberOfItems( iIndexHandle );
	if(ai_iItemPosition >= iNumberOfItems)
		return 0; //Valor por defecto

    return( m_pCMCR->GetIndexWayByPosition( iIndexHandle, ai_iItemPosition ) ) ;
}



m4uint16_t  ClNodeDef :: NumberOfAlias( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    return( m_pCMCR->GetNodeNumberOfAlias( Handle ) ) ;
}


m4pcchar_t  ClNodeDef :: AliasId( m4uint16_t ai_iAliasPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    return( m_pCMCR->GetNodeAliasIdByPosition( Handle, ai_iAliasPosition ) ) ;
}


m4pcchar_t  ClNodeDef :: AliasNodeId( m4uint16_t ai_iAliasPosition ) const
{
    m4uint32_t  iNodeHandle ;


    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    iNodeHandle = m_pCMCR->GetNodeAliasNodeHandleByPosition( Handle, ai_iAliasPosition ) ;

	if (iNodeHandle == 0)
		return "";

    return( m_pCMCR->GetNodeId( iNodeHandle ) ) ;
}


m4uint16_t  ClNodeDef :: NumberOfT3Alias( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    return( m_pCMCR->GetNodeNumberOfT3Alias( Handle ) ) ;
}


m4pcchar_t  ClNodeDef :: T3AliasId( m4uint16_t ai_iAliasPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    return( m_pCMCR->GetNodeT3AliasIdByPosition( Handle, ai_iAliasPosition ) ) ;
}


m4pcchar_t  ClNodeDef :: T3AliasInstance( m4uint16_t ai_iAliasPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    return( m_pCMCR->GetNodeT3AliasInstanceByPosition( Handle, ai_iAliasPosition ) ) ;
}




m4uint16_t  ClNodeDef :: NumberOfDmds( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    return( m_pCMCR->GetNodeNumberOfDmds( Handle ) ) ;
}


m4pcchar_t  ClNodeDef :: DmdId( m4uint16_t ai_iDmdPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    return( m_pCMCR->GetNodeDmdDmdIdByPosition( Handle, ai_iDmdPosition ) ) ;
}


m4pcchar_t  ClNodeDef :: DmdComponentId( m4uint16_t ai_iDmdPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    return( m_pCMCR->GetNodeDmdDmdComponentIdByPosition( Handle, ai_iDmdPosition ) ) ;
}


m4pcchar_t  ClNodeDef :: DmdFieldId( m4uint16_t ai_iDmdPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    return( m_pCMCR->GetNodeDmdDmdFieldIdByPosition( Handle, ai_iDmdPosition ) ) ;
}


m4uint16_t  ClNodeDef :: NumberOfQBFFilters( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    return( m_pCMCR->GetNodeNumberOfQBFFilters( Handle ) ) ;
}


m4pcchar_t  ClNodeDef :: QBFFilterId( m4uint16_t ai_iFilterPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    return( m_pCMCR->GetNodeQBFFilterIdByPosition( Handle, ai_iFilterPosition ) ) ;
}


m4pcchar_t  ClNodeDef :: QBFFilterT3Id( m4uint16_t ai_iFilterPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

	m4uint32_t	iFilterHandle = m_pCMCR->GetNodeQBFFilterHandleByPosition( Handle, ai_iFilterPosition ) ;

	if(iFilterHandle == 0)
		return ""; //Valor por defecto

    return( m_pCMCR->GetFilterT3Id( iFilterHandle ) ) ;
}


m4pcchar_t  ClNodeDef :: QBFFilterNodeId( m4uint16_t ai_iFilterPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

	m4uint32_t	iFilterHandle = m_pCMCR->GetNodeQBFFilterHandleByPosition( Handle, ai_iFilterPosition ) ;

	if(iFilterHandle == 0)
		return "";

    return( m_pCMCR->GetFilterNodeId( iFilterHandle ) ) ;
}

m4pcchar_t  ClNodeDef :: QBFFilterSysSentenceItem( m4uint16_t ai_iFilterPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

	m4uint32_t	iFilterHandle = m_pCMCR->GetNodeQBFFilterHandleByPosition( Handle, ai_iFilterPosition ) ;

	if(iFilterHandle == 0)
		return "";

    return( m_pCMCR->GetFilterSysSentenceItem( iFilterHandle ) ) ;
}

m4pcchar_t  ClNodeDef :: QBFFilterSysParamItem( m4uint16_t ai_iFilterPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

	m4uint32_t	iFilterHandle = m_pCMCR->GetNodeQBFFilterHandleByPosition( Handle, ai_iFilterPosition ) ;

	if(iFilterHandle == 0)
		return "";

    return( m_pCMCR->GetFilterSysParamItem( iFilterHandle ) ) ;
}

m4uint8_t  ClNodeDef :: QBFFilterBehaviourType( m4uint16_t ai_iFilterPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

	m4uint32_t	iFilterHandle = m_pCMCR->GetNodeQBFFilterHandleByPosition( Handle, ai_iFilterPosition ) ;

	if(iFilterHandle == 0)
		return 0; //Valor por defecto

    return( m_pCMCR->GetFilterBehaviourType( iFilterHandle ) ) ;
}

m4uint8_t  ClNodeDef :: QBFFilterIsOwnNode( m4uint16_t ai_iFilterPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

	m4uint32_t	iFilterHandle = m_pCMCR->GetNodeQBFFilterHandleByPosition( Handle, ai_iFilterPosition ) ;

	if(iFilterHandle == 0)
		return 1; //Valor por defecto

    return( m_pCMCR->GetFilterIsOwnNode( iFilterHandle ) ) ;
}

m4uint8_t  ClNodeDef :: QBFFilterIsApplicable( m4uint16_t ai_iFilterPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

	m4uint32_t	iFilterHandle = m_pCMCR->GetNodeQBFFilterHandleByPosition( Handle, ai_iFilterPosition ) ;

	if(iFilterHandle == 0)
		return 0; //Valor por defecto

    return( m_pCMCR->GetFilterIsApplicable( iFilterHandle ) ) ;
}


m4uint16_t  ClNodeDef :: NumberOfFindFilters( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    return( m_pCMCR->GetNodeNumberOfFindFilters( Handle ) ) ;
}


m4pcchar_t  ClNodeDef :: FindFilterId( m4uint16_t ai_iFilterPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    return( m_pCMCR->GetNodeFindFilterIdByPosition( Handle, ai_iFilterPosition ) ) ;
}


m4pcchar_t  ClNodeDef :: FindFilterT3Id( m4uint16_t ai_iFilterPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

	m4uint32_t	iFilterHandle = m_pCMCR->GetNodeFindFilterHandleByPosition( Handle, ai_iFilterPosition ) ;

	if(iFilterHandle == 0)
		return ""; //Valor por defecto

    return( m_pCMCR->GetFilterT3Id( iFilterHandle ) ) ;
}


m4pcchar_t  ClNodeDef :: FindFilterNodeId( m4uint16_t ai_iFilterPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

	m4uint32_t	iFilterHandle = m_pCMCR->GetNodeFindFilterHandleByPosition( Handle, ai_iFilterPosition ) ;

	if(iFilterHandle == 0)
		return "";

    return( m_pCMCR->GetFilterNodeId( iFilterHandle ) ) ;
}

m4pcchar_t  ClNodeDef :: FindFilterSysSentenceItem( m4uint16_t ai_iFilterPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

	m4uint32_t	iFilterHandle = m_pCMCR->GetNodeFindFilterHandleByPosition( Handle, ai_iFilterPosition ) ;

	if(iFilterHandle == 0)
		return "";

    return( m_pCMCR->GetFilterSysSentenceItem( iFilterHandle ) ) ;
}

m4pcchar_t  ClNodeDef :: FindFilterSysParamItem( m4uint16_t ai_iFilterPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

	m4uint32_t	iFilterHandle = m_pCMCR->GetNodeFindFilterHandleByPosition( Handle, ai_iFilterPosition ) ;

	if(iFilterHandle == 0)
		return "";

    return( m_pCMCR->GetFilterSysParamItem( iFilterHandle ) ) ;
}

m4uint8_t  ClNodeDef :: FindFilterBehaviourType( m4uint16_t ai_iFilterPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

	m4uint32_t	iFilterHandle = m_pCMCR->GetNodeFindFilterHandleByPosition( Handle, ai_iFilterPosition ) ;

	if(iFilterHandle == 0)
		return 0; //Valor por defecto

    return( m_pCMCR->GetFilterBehaviourType( iFilterHandle ) ) ;
}

m4uint8_t  ClNodeDef :: FindFilterIsOwnNode( m4uint16_t ai_iFilterPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

	m4uint32_t	iFilterHandle = m_pCMCR->GetNodeFindFilterHandleByPosition( Handle, ai_iFilterPosition ) ;

	if(iFilterHandle == 0)
		return 1; //Valor por defecto

    return( m_pCMCR->GetFilterIsOwnNode( iFilterHandle ) ) ;
}

m4uint8_t  ClNodeDef :: FindFilterIsApplicable( m4uint16_t ai_iFilterPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

	m4uint32_t	iFilterHandle = m_pCMCR->GetNodeFindFilterHandleByPosition( Handle, ai_iFilterPosition ) ;

	if(iFilterHandle == 0)
		return 0; //Valor por defecto

    return( m_pCMCR->GetFilterIsApplicable( iFilterHandle ) ) ;
}



m4uint8_t   ClNodeDef :: SecRead( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    return( m_pCMCR->GetNodeSecRead( Handle ) ) ;
}


m4uint8_t   ClNodeDef :: SecWrite( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    return( m_pCMCR->GetNodeSecWrite( Handle ) ) ;
}


m4uint8_t   ClNodeDef :: SecDelete( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    return( m_pCMCR->GetNodeSecDelete( Handle ) ) ;
}


m4uint8_t   ClNodeDef :: SecUpdate( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    return( m_pCMCR->GetNodeSecUpdate( Handle ) ) ;
}


m4uint8_t   ClNodeDef :: SecExecute( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    return( m_pCMCR->GetNodeSecExecute( Handle ) ) ;
}


m4uint8_t   ClNodeDef :: SecDynFilter( void ) const
{
    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;

    return( m_pCMCR->GetNodeSecDynFilter( Handle ) ) ;
}


m4pcchar_t   ClNodeDef :: SecFilter( void ) const
{

	m4pcchar_t	pccResult ;

    M4_ASSERT( m_pCMCR ) ;
    M4_ASSERT( Handle ) ;


	pccResult = m_pCMCR->GetNodeSecFilter( Handle ) ;

	if( pccResult == NULL )
	{
		return( "" ) ;
	}

    return( pccResult ) ;
}



// ole

m4uint8_t ClNodeDef::IsExternal (void) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeIsExternal (Handle);
}

m4uint8_t ClNodeDef::IsQBFFilter (void) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeIsQBFFilter (Handle);
}

m4uint8_t ClNodeDef::AffectsDB (void) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeAffectsDB (Handle);
}

m4uint8_t ClNodeDef::DBReload (void) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeDBReload (Handle);
}

m4uint8_t ClNodeDef::TiInheritLevel (void) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeTiInheritLevel (Handle);
}


m4uint8_t ClNodeDef::IsOverWritten (void) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeIsOverWritten (Handle);
}

m4uint32_t ClNodeDef::OleDispId (void) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeOleDispId (Handle);
}

m4pcchar_t ClNodeDef::OleNodeInterId (void) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeOleNodeInterId (Handle);
}

m4uint32_t ClNodeDef::MaxNumBlocks (void) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeMaxNumBlocks (Handle);
}

m4uint32_t ClNodeDef::MaxNumRecords (void) const
{
    M4_ASSERT (m_pCMCR);
    M4_ASSERT (Handle);

    return m_pCMCR->GetNodeMaxNumRecords (Handle);
}

m4return_t ClNodeDef::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClNodeDef);
		ao_size-=sizeof(Handle);
	
	if (Handle.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	return M4_SUCCESS;
}

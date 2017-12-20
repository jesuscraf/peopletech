//## begin module.includes preserve=yes
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             chandef.cpp
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
//    This module defines...
//
//
//==============================================================================

//----------------------------------------------------------------------------
#include "dm_def.hpp"
#include "m4define.hpp"
//----------------------------------------------------------------------------

#include "chandef.hpp"

#include "m4mdrt.hpp"

//## end module.includes preserve=yes
ClChannelDef::ClChannelDef ()
{
//## begin ClChannelDef::ClChannelDef%444226185.body preserve=yes
    m_pCMCR = 0;
//## end ClChannelDef::ClChannelDef%444226185.body
}

ClChannelDef::~ClChannelDef ()
{
//## begin ClChannelDef::~ClChannelDef%-1514410382.body preserve=yes
//## end ClChannelDef::~ClChannelDef%-1514410382.body
}

m4return_t ClChannelDef::Attach (ClCompiledMCR *ai_pcmcr)
{
    m_pCMCR = ai_pcmcr;
    M4_ASSERT (m_pCMCR);

    //return NodeDef.Attach (m_pCMCR, this);
    return M4_SUCCESS;
}

m4pcchar_t ClChannelDef::Id (void) const
{
//## begin ClChannelDef::Id%1646477984.body preserve=yes
    M4_ASSERT (m_pCMCR);

    return m_pCMCR->GetChannelId ();
//## end ClChannelDef::Id%1646477984.body
}

m4pcchar_t ClChannelDef::Name (void) const
{
//## begin ClChannelDef::Name%1348964238.body preserve=yes
    M4_ASSERT (m_pCMCR);

    return m_pCMCR->GetChannelName ();
//## end ClChannelDef::Name%1348964238.body
}

m4date_t ClChannelDef::StartDate (void) const
{
    M4_ASSERT (m_pCMCR);

    return m_pCMCR->GetChannelStartDate ();
}

m4date_t ClChannelDef::EndDate (void) const
{
    M4_ASSERT (m_pCMCR);

    return m_pCMCR->GetChannelEndDate ();
}

m4date_t ClChannelDef::CorStartDate (void) const
{
    M4_ASSERT (m_pCMCR);

    return m_pCMCR->GetChannelCorStartDate ();
}

m4date_t ClChannelDef::CorEndDate (void) const
{
    M4_ASSERT (m_pCMCR);

    return m_pCMCR->GetChannelCorEndDate ();
}

m4date_t ClChannelDef::Version (void) const
{
    M4_ASSERT (m_pCMCR);

    return m_pCMCR->GetChannelVersion ();
}

m4date_t ClChannelDef::ExecutionDate (void) const
{
    M4_ASSERT (m_pCMCR);

    return m_pCMCR->GetChannelCreationDate ();
}

m4uint16_t ClChannelDef::NumberOfRoots (void) const
{
    M4_ASSERT (m_pCMCR);

    return m_pCMCR->GetChannelNumberOfRoots ();
}

ClHandle ClChannelDef::RootHandle (m4uint16_t ai_ipos) const
{
    M4_ASSERT (m_pCMCR);

    return m_pCMCR->GetChannelRootHandleByPosition (ai_ipos);
}

/*
m4return_t ClChannelDef::DestroyCMCR (void)
{
    if (m_pCMCR) {
        delete m_pCMCR;
        m_pCMCR = 0;
    }

    return M4_SUCCESS;
}
*/

m4uint32_t ClChannelDef::NodeHandleById (m4pcchar_t ai_pccNodeId) const
{
    M4_ASSERT (m_pCMCR);

    return m_pCMCR->GetChannelNodeHandleById (ai_pccNodeId);
}

m4uint16_t ClChannelDef::NumberOfNodes (void) const
{
    M4_ASSERT (m_pCMCR);

    return m_pCMCR->GetChannelNumberOfNodes ();
}


m4uint16_t  ClChannelDef :: NumberOfInstances( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelNumberOfInstances() ) ;
}


m4pcchar_t  ClChannelDef :: GetInstanceIdByPosition( m4uint16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iInstanceHandle = m_pCMCR->GetChannelInstanceHandleByPosition( ai_iPosition ) ;

    return( m_pCMCR->GetInstanceId( iInstanceHandle ) ) ;
}


m4pcchar_t  ClChannelDef :: GetInstanceT3ByPosition( m4uint16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iInstanceHandle = m_pCMCR->GetChannelInstanceHandleByPosition( ai_iPosition ) ;

    return( m_pCMCR->GetInstanceT3Id( iInstanceHandle ) ) ;
}


m4pcchar_t  ClChannelDef :: GetInstanceRoleByPosition( m4uint16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iInstanceHandle = m_pCMCR->GetChannelInstanceHandleByPosition( ai_iPosition ) ;

    return( m_pCMCR->GetInstanceRoleId( iInstanceHandle ) ) ;
}



m4pcchar_t  ClChannelDef :: GetInstanceOrganizationByPosition( m4uint16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iInstanceHandle = m_pCMCR->GetChannelInstanceHandleByPosition( ai_iPosition ) ;

    return( m_pCMCR->GetInstanceOrganization( iInstanceHandle ) ) ;
}


m4uint8_t  ClChannelDef :: GetInstanceScopeByPosition( m4uint16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iInstanceHandle = m_pCMCR->GetChannelInstanceHandleByPosition( ai_iPosition ) ;

    return( m_pCMCR->GetInstanceScope( iInstanceHandle ) ) ;
}


m4uint8_t  ClChannelDef :: GetInstanceOpenModeByPosition( m4uint16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iInstanceHandle = m_pCMCR->GetChannelInstanceHandleByPosition( ai_iPosition ) ;

    return( m_pCMCR->GetInstanceOpenMode( iInstanceHandle ) ) ;
}


m4uint8_t  ClChannelDef :: GetInstanceCsAccessByPosition( m4uint16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iInstanceHandle = m_pCMCR->GetChannelInstanceHandleByPosition( ai_iPosition ) ;

    return( m_pCMCR->GetInstanceCsAccess( iInstanceHandle ) ) ;
}


m4uint8_t  ClChannelDef :: GetInstanceUseRoleByPosition( m4uint16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iInstanceHandle = m_pCMCR->GetChannelInstanceHandleByPosition( ai_iPosition ) ;

    return( m_pCMCR->GetInstanceUseRole( iInstanceHandle ) ) ;
}


m4uint8_t  ClChannelDef :: GetInstanceOrganizationTypeByPosition( m4uint16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iInstanceHandle = m_pCMCR->GetChannelInstanceHandleByPosition( ai_iPosition ) ;

    return( m_pCMCR->GetInstanceOrganizationType( iInstanceHandle ) ) ;
}


m4uint16_t ClChannelDef::NodeIndexByHandle (ClHandle ai_nodehandle) const
{
    M4_ASSERT (m_pCMCR);

    return m_pCMCR->GetNodeIndex (ai_nodehandle);
}

m4pcchar_t
ClChannelDef::RootIdByPosition
	(
	m4uint16_t ai_iRootPosition
	) const
{
    M4_ASSERT (m_pCMCR);

    return m_pCMCR->GetChannelRootIdByPosition(ai_iRootPosition);
}


// mas cosas

m4uint8_t   ClChannelDef :: Type( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelType() ) ;
}

m4uint8_t   ClChannelDef :: TimeUnit( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelTimeUnit() ) ;
}

m4uint16_t  ClChannelDef :: Build( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelBuild() ) ;
}

m4uint8_t   ClChannelDef :: CsType( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelCsType() ) ;
}

m4uint8_t   ClChannelDef :: IsSeparable( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelIsSeparable() ) ;
}

m4uint8_t   ClChannelDef :: CsExeType( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelCsExeType() ) ;
}

m4uint8_t   ClChannelDef :: InheritLevel( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelInheritLevel() ) ;
}


m4uint16_t   ClChannelDef :: VMCapacityReq( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelVMCapacityReq() ) ;
}

m4uint32_t   ClChannelDef :: EnvironmentVars( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelEnvironmentVars() ) ;
}


m4uint32_t   ClChannelDef :: OwnerFlag( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelOwnerFlag() ) ;
}


m4date_t   ClChannelDef :: CacheMaxPeriod( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelCacheMaxPeriod() ) ;
}


m4uint8_t   ClChannelDef :: IsCacheable( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelIsCacheable() ) ;
}


m4uint8_t   ClChannelDef :: IsMDCacheable( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelIsMDCacheable() ) ;
}


m4uint8_t   ClChannelDef :: CheckConcurrency( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelCheckConcurrency() ) ;
}



m4uint8_t   ClChannelDef :: OrganizationType( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelOrganizationType() ) ;
}


m4uint8_t   ClChannelDef :: CreationType( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelCreationType() ) ;
}


m4pcchar_t	ClChannelDef :: ServiceId( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelServiceId() ) ;
}


m4pcchar_t	ClChannelDef :: ServiceAltId( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelServiceAltId() ) ;
}


m4uint16_t	ClChannelDef :: NumberOfParameters( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelNumberOfParameters() ) ;
}


m4pcchar_t	ClChannelDef :: ParameterNode( m4uint16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iItem = m_pCMCR->GetChannelParameterHandleByPosition( ai_iPosition ) ;
    m4uint32_t  iNode = m_pCMCR->GetItemNodeHandle( iItem ) ;

    return( m_pCMCR->GetNodeId( iNode ) ) ;
}


m4pcchar_t	ClChannelDef :: ParameterItem( m4uint16_t ai_iPosition ) const
{
    M4_ASSERT( m_pCMCR ) ;

    m4uint32_t  iItem = m_pCMCR->GetChannelParameterHandleByPosition( ai_iPosition ) ;

    return( m_pCMCR->GetItemId( iItem ) ) ;
}




m4uint8_t   ClChannelDef :: HasSecurity( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelHasSecurity() ) ;
}

m4uint8_t   ClChannelDef :: SecRead( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelSecRead() ) ;
}

m4uint8_t   ClChannelDef :: SecWrite( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelSecWrite() ) ;
}

m4uint8_t   ClChannelDef :: SecDelete( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelSecDelete() ) ;
}

m4uint8_t   ClChannelDef :: SecUpdate( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelSecUpdate() ) ;
}

m4uint8_t   ClChannelDef :: SecExecute( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelSecExecute() ) ;
}



m4uint8_t   ClChannelDef :: IsExternal( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelIsExternal() ) ;
}


m4pcchar_t   ClChannelDef :: OleClassId( void ) const
{
    M4_ASSERT( m_pCMCR ) ;

    return( m_pCMCR->GetChannelOleClassId() ) ;
}


m4return_t 
ClChannelDef::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClChannelDef);
	
	if (m_pCMCR){
/*		if (m_pCMCR->GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
*/	}
	return M4_SUCCESS;
}

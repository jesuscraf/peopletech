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

//## begin module.includes preserve=yes

#include "m4types.hpp"
#include "handles.hpp"

//#include "ndef_dir.hpp"

//## end module.includes preserve=yes

//## begin module.additionalDeclarations preserve=yes

#ifndef __CHANDEF_HPP__
#define __CHANDEF_HPP__
#include "m4dm_dll.hpp"
class ClChannel;
class ClCompiledMCR;

class ClSize;

//## end module.additionalDeclarations

class M4_DECL_M4DM ClChannelDef
{
public:
    ClChannelDef ();
   ~ClChannelDef ();

    m4return_t Attach (ClCompiledMCR *ai_pcmcr);
    m4return_t Detach (void)
    {
        m_pCMCR = 0;
        return M4_SUCCESS;
    }

    //Colección de nodedef:
    //ClNodeDef_Directory NodeDef;

    m4pcchar_t Id (void) const;
    m4pcchar_t Name (void) const;

    m4date_t StartDate (void) const;
    m4date_t EndDate (void) const;
    m4date_t CorStartDate (void) const;
    m4date_t CorEndDate (void) const;
    m4date_t Version (void) const;
    m4date_t ExecutionDate (void) const;

    m4uint16_t NumberOfRoots (void) const;
    ClHandle RootHandle (m4uint16_t ai_ipos) const;
	m4pcchar_t RootIdByPosition(m4uint16_t ai_iRootPosition) const ;

    m4uint32_t NodeHandleById (m4pcchar_t ai_pccNodeId) const;
    m4uint16_t NodeIndexByHandle (ClHandle ai_nodehandle) const;

    m4uint16_t NumberOfNodes (void) const;    

    m4uint16_t  NumberOfInstances( void ) const ;
    m4pcchar_t  GetInstanceIdByPosition( m4uint16_t ai_iPosition ) const ;
    m4pcchar_t  GetInstanceT3ByPosition( m4uint16_t ai_iPosition ) const ;
    m4pcchar_t  GetInstanceRoleByPosition( m4uint16_t ai_iPosition ) const ;
    m4pcchar_t  GetInstanceOrganizationByPosition( m4uint16_t ai_iPosition ) const ;
    m4uint8_t   GetInstanceScopeByPosition( m4uint16_t ai_iPosition ) const ;
    m4uint8_t   GetInstanceOpenModeByPosition( m4uint16_t ai_iPosition ) const ;
    m4uint8_t   GetInstanceCsAccessByPosition( m4uint16_t ai_iPosition ) const ;
    m4uint8_t   GetInstanceUseRoleByPosition( m4uint16_t ai_iPosition ) const ;
    m4uint8_t   GetInstanceOrganizationTypeByPosition( m4uint16_t ai_iPosition ) const ;
	

// mas cosas

    m4uint8_t   Type( void ) const ;
    m4uint8_t   TimeUnit( void ) const ;
    m4uint16_t  Build( void ) const ;
    m4uint8_t   CsType( void ) const ;
    m4uint8_t   IsSeparable( void ) const ;
    m4uint8_t   CsExeType( void ) const ;
    m4uint8_t   InheritLevel( void ) const ;
    m4uint16_t  VMCapacityReq( void ) const ;
    m4uint32_t  EnvironmentVars( void ) const ;
    m4uint32_t  OwnerFlag( void ) const ;

    m4date_t	CacheMaxPeriod( void ) const ;
    m4uint8_t   IsCacheable( void ) const ;
    m4uint8_t   IsMDCacheable( void ) const ;
    m4uint8_t   CheckConcurrency( void ) const ;
    m4uint8_t   OrganizationType( void ) const ;
    m4uint8_t   CreationType( void ) const ;
	m4pcchar_t  ServiceId( void ) const ;
	m4pcchar_t  ServiceAltId( void ) const ;

    m4uint16_t  NumberOfParameters( void ) const ;
    m4pcchar_t  ParameterNode( m4uint16_t ai_iPosition ) const ;
    m4pcchar_t  ParameterItem( m4uint16_t ai_iPosition ) const ;

// security

    m4uint8_t   HasSecurity( void ) const ;

    m4uint8_t   SecRead( void ) const ;
    m4uint8_t   SecWrite( void ) const ;
    m4uint8_t   SecDelete( void ) const ;
    m4uint8_t   SecUpdate( void ) const ;
    m4uint8_t   SecExecute( void ) const ;

// ole

	m4uint8_t	IsExternal( void ) const ;
	m4pcchar_t  OleClassId( void ) const ;


    m4bool_t IsValid (void) const
    {
        return (m_pCMCR)? M4_TRUE : M4_FALSE;
    }

    ClCompiledMCR *GetpCMCR (void) const
    {
        return m_pCMCR;
    }
	
    m4return_t GetSize(ClSize &ao_size);

private:
    //m4return_t DestroyCMCR (void);

    ClCompiledMCR *m_pCMCR;
};

#endif // __CHANDEF_HPP__ 

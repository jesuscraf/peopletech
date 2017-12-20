#ifndef __CHANPROP_HPP__
#define __CHANPROP_HPP__

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             chanprop.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             23/05/98
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

#include "m4dm_dll.hpp"

#include "property.h"

class ClChannel;
class ClSize;





// ------------------------------------------------------------------
// class ClWarpOrganizationProperty
// ------------------------------------------------------------------

class M4_DECL_M4DM ClOrganizationProperty : public ClGenericProperty
{
public:
    ClOrganizationProperty(void)
        : m_pClChannel(NULL)
    {
        m_ePropertyId = e_ClOrganizationProperty;
    }

    ClOrganizationProperty( m4uint8_t ai_iStatus )
        : m_pClChannel( NULL )
		, ClGenericProperty( ai_iStatus )
    {
        m_ePropertyId = e_ClOrganizationProperty;
    }

    void Attach (ClChannel* ai_pchannel)
    {
        M4_ASSERT (ai_pchannel);
        m_pClChannel = ai_pchannel;
    }

	virtual m4return_t Set(const m4VariantType& ai_var);
	virtual	m4return_t Set( m4pchar_t ai_pcValue ) ;

protected:
    ClChannel* m_pClChannel;
};





// ------------------------------------------------------------------
// class ClRecordDateProperty
// ------------------------------------------------------------------

class M4_DECL_M4DM ClRecordVersionProperty : public ClProperty
{
public:
    ClRecordVersionProperty(void)
        : ClProperty(M4_PROP_STATIC_NO_MAPPED)
    {
        m_ePropertyId = e_ClRecordVersionProperty;
    }

    ClRecordVersionProperty( m4uint8_t ai_iStatus )
		: ClProperty( ai_iStatus )
    {
        m_ePropertyId = e_ClRecordVersionProperty;
    }

	virtual m4return_t Get(m4VariantType& ao_var) const;
	virtual m4return_t Set(const m4VariantType& ai_var);

	void Get(m4uint32_t& ao_iRecordId, m4date_t& ao_dNewVersion) const;
	void Set(const m4uint32_t ai_iRecordId, const m4date_t ai_dNewVersion);

    virtual m4return_t _Serialize(ClGenericIODriver& IOD);
    virtual m4return_t _DeSerialize(ClGenericIODriver& IOD);

private:
    m4uint32_t m_iRecordId;     // Identifica el registro
    m4date_t m_dNewVersion;     // Nueva fecha de ultima modificacion que hay que asignar al registro
};

#endif //__CHANPROP_HPP__

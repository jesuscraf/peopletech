//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         VM
// File:           medvarin.hpp   
// Project:        Component Name
// Author:         Meta Software M.S. , S.A
// Date:            15/07
// Language:        C++ 
// Operating System: WINDOWS
// Design Document:  EXTCONCI.DOC
//
//
// Definition:
//
//    This module defines...
////
//==============================================================================


#ifndef _MEDVARIN_HPP
#define _MEDVARIN_HPP

#include "m4variant_dll.hpp"

#include "cldefine.hpp"
#include "cltypes.hpp"
#include "m4var.hpp"



#define M4CL_MEDVARINT_MAX_STRING  256


class M4_DECL_M4VARIANT IStringPool
{
public:

	virtual	m4pchar_t	GetString( m4uint32_t ai_iOffset ) const = 0 ;
	virtual	m4return_t	AddString( m4pcchar_t ai_pccInString, m4uint32_t &ao_riOutOffset ) = 0 ;
	virtual	m4return_t	DeleteString( m4uint32_t ai_iOffset ) = 0 ;
};


class M4_DECL_M4VARIANT ClVariantConversor
{
    private:
		IStringPool	*m_poPool;
        m4char_t	m_acBuffer[ M4CL_MEDVARINT_MAX_STRING ];
		m4bool_t	m_bDateOverflow;

	public:
		ClVariantConversor (IStringPool *ai_poPool)
		{
			m_poPool=ai_poPool;
			m_bDateOverflow=M4_FALSE;
		}

		ClVariantConversor (void )
		{
			m_poPool=NULL;
			m_bDateOverflow=M4_FALSE;
		}

		void SetPool (IStringPool *ai_poPool)
		{
			m_poPool=ai_poPool;
		}

		m4return_t ToDouble		(m4VariantType &aio_oVariant);
		m4return_t ToDate		(m4VariantType &aio_oVariant);
		m4return_t ToPoolString	(m4VariantType &aio_oVariant);
		m4return_t ToCharString	(m4VariantType &aio_oVariant);
		m4return_t ToCharSysString	(m4VariantType &aio_oVariant);

        m4return_t ToNull       (m4VariantType &aio_oVariant)
		{
			m_bDateOverflow=M4_FALSE;
			aio_oVariant.IsUser=M4_FALSE;
            aio_oVariant.Type=M4CL_CPP_TYPE_NULL;
            aio_oVariant.Data.DoubleData=0;
            return M4_SUCCESS;
        }
		m4bool_t	IsDateOverflow(void);
};

#endif

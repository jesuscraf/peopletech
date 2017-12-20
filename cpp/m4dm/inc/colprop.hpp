#ifndef __COLPROP__HPP__
#define __COLPROP__HPP__

//==============================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:             colprop.hpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:             22/07/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:
//
//
// Definition:
//
//    This module defines the interface of a
//    collection of properties
//
//==============================================

//## begin module.includes preserve=yes
#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include "m4stl.hpp"
#include "property.h"
#include "changelistener.hpp"

//## end module.includes preserve=yes



// ==========================================================================
// número de propiedades fijas de M4Obj
// Se emplea para optimizar la serialización
#define	M4_DM_M4OBJ_PROPERTIES_NUMBER								23
#define	M4_DM_ACCESS_PROPERTIES_NUMBER								 4



struct m4pchar_t_less
{
	m4bool_t operator()(m4pchar_t ai_psz1, m4pchar_t ai_psz2) const
	{
		return ( (strcmp(ai_psz1, ai_psz2) < 0) ? M4_TRUE : M4_FALSE );
	}
};


class M4_DECL_M4DM ClColProp : public ClChangeListener
{
public:
	virtual ~ClColProp(void);

	size_t Count(void) const;

	// Functions to get the data using id
	const m4pchar_t GetPropName(m4int32_t ai_ui) const;

	m4uint8_t GetPropFlags( m4int32_t ai_ui, m4uint8_t ai_iFlag ) const ;

	m4return_t GetPropValue(m4int32_t ai_ui, m4VariantType&  ao_var) const;

	m4return_t AddProperty(const m4pchar_t ai_psz, ClProperty* ai_property);

    ClProperty* GetProperty(const m4pchar_t ai_psz) const;

	// Functions for accessing the map data in the normal way (viseble al LN4) >>>>>>>>>>>>>>>>>>
	m4return_t GetPropValue(const m4pchar_t ai_pszName, m4VariantType& ao_var) const;

	m4return_t SetPropValue(const m4pchar_t ai_psz, const m4VariantType& ai_var);

	m4return_t NewProperty(const m4pchar_t ai_psz, m4uint8_t ai_iStatus);

    m4return_t SetOrNewProperty( const m4pchar_t ai_psz, const m4VariantType& ai_var, m4uint8_t ai_iStatus ) ;

    m4return_t DeleteProperty(const m4pchar_t ai_psz);
    // Functions for accessing the map data in the normal way (viseble al LN4) <<<<<<<<<<<<<<<<<<

    // Para manejar propiedades que NO queremos que sean visibles desde el LN4 >>>>>>>>>>>>>>>>>>
	m4return_t GetPrivatePropValue(const m4pchar_t ai_pszName, m4VariantType&  ao_var) const;

	m4return_t SetPrivatePropValue(const m4pchar_t ai_psz, const m4VariantType& ai_var);

    m4return_t NewPrivateProperty(const m4pchar_t ai_psz, m4uint8_t ai_iStatus);

    m4return_t DeletePrivateProperty(const m4pchar_t ai_psz);
    // Para manejar propiedades que no queremos que sean visibles desde el LN4 <<<<<<<<<<<<<<<<<<

	m4return_t GetSize(ClSize &ao_size) const;

    m4return_t Serialize  ( ClGenericIODriver& IOD, m4uint8_t ai_iIteSerMod );
    m4return_t DeSerialize( ClGenericIODriver& IOD, m4uint8_t ai_iType ) ;

//## begin ClColProp.initialDeclarations preserve=yes
private:
    m4return_t _GetPropValue(const m4pchar_t ai_psz,
                             m4VariantType&  ao_var,
                             m4bool_t  ai_bCheckPublic) const;

    m4return_t _SetPropValue(const m4pchar_t      ai_psz,
                             const m4VariantType& ai_var,
                                   m4bool_t       ai_bCheckPublic);

	m4return_t _NewProperty(const m4pchar_t ai_psz, m4uint8_t ai_iStatus);

    m4return_t _DeleteProperty(const m4pchar_t ai_psz, m4bool_t ai_bCheckPublic);

	typedef map<m4pchar_t, ClProperty*, m4pchar_t_less>		STRING2PROPERTY;
//## end ClColProp.initialDeclarations

	STRING2PROPERTY m_oMapStringVariant;
};


M4_DECL_M4DM ostream& operator<< ( ostream& ai_roStream, const ClColProp &ai_roColProp ) ;


#endif

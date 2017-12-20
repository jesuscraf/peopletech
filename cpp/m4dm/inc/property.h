#ifndef __PROPERTY__HPP__
#define __PROPERTY__HPP__

//==============================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             property.hpp
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
//    This module defines a property
//
//    
//
//==============================================

#include "m4dm_dll.hpp"
#include "clgeniod.hpp"
#include "m4var.hpp"
#include "chlog.hpp"
#include "sizer.hpp"
#include "dmres.hpp"
#include "cldatesbase.hpp"



class ClGenericIODriver;
class IChangeListener;



// Defines de la serialización de propiedades
#define	M4_DMPROP_NULL							0x00
#define	M4_DMPROP_NUMBER						0x01
#define	M4_DMPROP_STRING						0x02
#define	M4_DMPROP_DATE							0x03
#define	M4_DMPROP_MINUS_INF						0x04
#define	M4_DMPROP_PLUS_INF						0x05
#define	M4_DMPROP_NULL_STRING					0x06
#define	M4_DMPROP_MINUS_ONE						0x07
#define	M4_DMPROP_CERO							0x08
#define	M4_DMPROP_ONE							0x09
#define	M4_DMPROP_TWO							0x0A
#define	M4_DMPROP_THREE							0x0B




// Los distintos tipos de propiedades
enum propertyId_e
{
    e_ClProperty,
    e_ClGenericProperty,
    e_ClAutoLoadProperty,
    e_ClRecordVersionProperty,
	e_ClOrganizationProperty
} ;


#define	M4_PROP_DYNAMIC						0x0001
#define	M4_PROP_MAPPED						0x0002
#define	M4_PROP_READ						0x0004
#define	M4_PROP_WRITE						0x0008
#define	M4_PROP_READ_COL					0x0010
#define	M4_PROP_WRITE_COL					0x0020
#define	M4_PROP_DELETE_COL					0x0040
#define	M4_PROP_SESSION						0x0080

#define	M4_PROP_STATIC_MAPPED				0x003E		// No delete, no dynamic
#define	M4_PROP_STATIC_NO_MAPPED			0x003C		// No delete, no mapped, no dynamic
#define	M4_PROP_FULL_STATE					0x00FF
#define	M4_PROP_STATUS_CONSTRAIN			0x007E		// Ni la sesión ni el dynamic se puede cambiar
#define	M4_PROP_LN4_STATE					0x007F


// ------------------------------------------------------------------
// class ClProperty
// ------------------------------------------------------------------

class M4_DECL_M4DM ClProperty
{
public:
    // Por defecto las propiedades se crean estaticas y no son mapped. Que no sean mapped tiene sentido
    // ya que es mucho mas omptimo serializarlas a parte y no dentro de la coleccion de propiedades
    ClProperty( void )
        : m_ePropertyId( e_ClProperty )
		, m_iStatus( M4_PROP_STATIC_NO_MAPPED )
		, m_poChangeListener( NULL )
    {}

    ClProperty( m4uint8_t ai_iStatus )
        : m_ePropertyId( e_ClProperty )
		, m_iStatus( ai_iStatus )
		, m_poChangeListener( NULL )
    {}

    virtual ~ClProperty() {}

	void	SetChangeListener( IChangeListener *ai_poChangeListener )
	{
		m_poChangeListener = ai_poChangeListener ;
	}

	virtual m4return_t Get( m4VariantType &ao_vValue ) const = 0 ;
	virtual m4return_t Set( const m4VariantType &ai_vValue ) = 0 ;

    m4return_t Serialize( ClGenericIODriver& IOD )
    {
        m4return_t iResult = IOD.Write( m_iStatus ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

        return( _Serialize( IOD ) ) ;
    }

    m4return_t DeSerialize( ClGenericIODriver& IOD )
    {
        m4return_t iResult = IOD.Read( m_iStatus ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

        return( _DeSerialize( IOD ) ) ;
    }

    virtual m4return_t _Serialize( ClGenericIODriver& IOD ) = 0 ;
    virtual m4return_t _DeSerialize( ClGenericIODriver& IOD ) = 0 ;

	virtual m4return_t GetSize(ClSize &ao_size) const
	{
		ao_size+=sizeof(ClProperty);
		return M4_SUCCESS;
	}

    propertyId_e	GetPropertyId( void ) const
    {
        return( m_ePropertyId ) ;
    }

	m4bool_t	GetFlags( m4uint8_t ai_iFlag ) const
	{
		if( ( m_iStatus & ai_iFlag ) != 0 )
		{
			return( M4_TRUE ) ;
		}
		return( M4_FALSE ) ;
	}

	m4uint8_t	GetStatus( void ) const
	{
		return( m_iStatus ) ;
	}

protected:	

    propertyId_e	m_ePropertyId ;
	m4uint8_t		m_iStatus ;
	IChangeListener	*m_poChangeListener ;

// No se deja hacer estatus de flags que no existen ni de la dynamic

	void	SetStatus( m4uint8_t ai_iStatus )
	{
		m_iStatus = ( ai_iStatus & M4_PROP_STATUS_CONSTRAIN ) ;
	}

	void	SetFlags( m4uint8_t ai_iFlags )
	{
		m_iStatus |= ( ai_iFlags & M4_PROP_STATUS_CONSTRAIN ) ;
	}

	void	UnSetFlags( m4uint8_t ai_iFlags )
	{
		//El efecto es imposibilitar el UnSet de algunos flags.
		m_iStatus &=  ~( ai_iFlags & M4_PROP_STATUS_CONSTRAIN ) ;
	}

	friend class ClChannel ;
	friend class ClUserActionsAudit ;
	friend class ClVM1Indirections ;
} ;



inline ostream M4_DECL_M4DM &operator<< ( ostream& ai_roStream, const ClProperty &ai_roPropety )
{
	m4VariantType	vValue ;

	ai_roStream << "[" << m4uint16_t( ai_roPropety.GetStatus() ) << "]" ;
	ai_roPropety.Get( vValue ) ;
	ai_roStream << vValue ;

	return( ai_roStream ) ;
}


// ------------------------------------------------------------------
// class ClGenericProperty
// ------------------------------------------------------------------

class M4_DECL_M4DM ClGenericProperty : public ClProperty
{

public:

    ClGenericProperty( void )
    {
        m_ePropertyId = e_ClGenericProperty ;
    }

    ClGenericProperty( m4uint8_t ai_iStatus )
        : ClProperty( ai_iStatus )
    {
        m_ePropertyId = e_ClGenericProperty ;
    }

	ClGenericProperty( m4uint8_t ai_iStatus, const m4VariantType& ai_rcvValue )
        : ClProperty( ai_iStatus )
	{
        m_ePropertyId = e_ClGenericProperty ;
		m_vValue = ai_rcvValue ;
	}

	virtual m4return_t Get( m4VariantType& ao_rvValue ) const
	{
		if( GetFlags( M4_PROP_READ ) == M4_FALSE )
		{
			// No puedo acceder a una propiedad de no lectura
			DUMP_CHLOG_WARNING( M4_CH_DM_NO_READ_PROP ) ;
			ao_rvValue.Type = M4CL_CPP_TYPE_NULL ;
			ao_rvValue.Data.DoubleData = 0 ;
			return( M4_ERROR ) ;
		}
		ao_rvValue = m_vValue ;
		return( M4_SUCCESS ) ;
	}

	virtual m4return_t Set( const m4VariantType& ai_rcvValue )
	{
		return( _Set( ai_rcvValue, M4_TRUE ) ) ;
	}

	virtual m4return_t Get( m4double_t &ao_rdValue ) const
	{
		if( GetFlags( M4_PROP_READ ) == M4_FALSE )
		{
			// No puedo acceder a una propiedad de no lectura
			DUMP_CHLOG_WARNING( M4_CH_DM_NO_READ_PROP ) ;
			ao_rdValue = 0 ;
			return( M4_ERROR ) ;
		}
		ao_rdValue = m4double_t( m_vValue ) ;
		return( M4_SUCCESS ) ;
	}

	virtual m4return_t Set( m4double_t ai_dValue, m4uint8_t ai_iType )
	{
		m4VariantType	vValue ;

		vValue.Type = ai_iType ;
		vValue.Data.DoubleData = ai_dValue ;

		return( _Set( vValue, M4_TRUE ) ) ;
	}

	virtual m4return_t Get( m4pchar_t &ao_pcValue ) const
	{
		if( GetFlags( M4_PROP_READ ) == M4_FALSE )
		{
			// No puedo acceder a una propiedad de no lectura
			DUMP_CHLOG_WARNING( M4_CH_DM_NO_READ_PROP ) ;
			ao_pcValue = NULL ;
			return( M4_ERROR ) ;
		}
		ao_pcValue = m4pchar_t( m_vValue ) ;
		return( M4_SUCCESS ) ;
	}

	virtual m4return_t Set( m4pchar_t ai_pcValue )
	{
		m4VariantType	vValue ;

		if( ai_pcValue != NULL )
		{
			vValue.Type = M4CL_CPP_TYPE_STRING_VAR ;
			vValue.Data.PointerChar = ai_pcValue ;
		}
		else
		{
			vValue.Type = M4CL_CPP_TYPE_NULL ;
		}

		return( _Set( vValue, M4_TRUE ) ) ;
	}

	virtual m4return_t SetNull()
	{
		m4VariantType	vValue ;

		vValue.Type = M4CL_CPP_TYPE_NULL ;
		return( _Set( vValue, M4_TRUE ) ) ;
	}

	virtual m4return_t GetSize(ClSize &ao_size) const
	{
		ao_size+=sizeof(ClGenericProperty);
			
		ao_size-=sizeof(ClProperty);
		if (ClProperty::GetSize(ao_size) != M4_SUCCESS){
			return M4_ERROR;
		}

		ao_size-=sizeof(m_vValue);
		if (m_vValue.GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR; 
		}
		return M4_SUCCESS;
	}

	virtual m4return_t _Serialize( ClGenericIODriver& IOD );
 
	virtual m4return_t _DeSerialize( ClGenericIODriver& IOD );	

private:

	m4return_t	_Set( const m4VariantType& ai_rcvValue, m4bool_t ai_bCheck ) ;

    m4VariantType	m_vValue;
};


#endif	// __PROPERTY__HPP__

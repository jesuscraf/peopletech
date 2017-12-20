//## begin module.includes preserve=yes
//=============================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:             colprop.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:             23/07/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:
//

//
// Definition:
//
//    This module defines the code for the
//    collection of properties
//
// NB. This has no exception code or
//     error handling - waiting for project
//     standard
//
//=============================================================

#include "colprop.hpp"
#include "chanprop.hpp"
#include "autoload.hpp"
#include "sizerutils.hpp"
#include "changelistener.hpp"





// Para la optimización de las propiedades
extern	m4pchar_t   g_acM4ObjProperties[ M4_DM_M4OBJ_PROPERTIES_NUMBER ] ;
extern	m4pchar_t   g_acAccessProperties[ M4_DM_ACCESS_PROPERTIES_NUMBER ] ;






//
// Function: ClColProp::~ClColProp
//
// Arguments: void
//
// Returns: void
//
// Description:
//   The destructor
//
//## end module.includes preserve=yes
ClColProp::~ClColProp(void)
{
//## begin ClColProp::~ClColProp%1310200079.body preserve=yes
	// If the string and variant were classes
	// This would be so easy, but unfortunately

    STRING2PROPERTY::const_iterator im = m_oMapStringVariant.begin();
	
    // Loop to the correct element or the end
	for (; im != m_oMapStringVariant.end(); ++im)
	{
		// Delete the string
		delete [] (*im).first;

		if (((*im).second)->GetFlags( M4_PROP_DYNAMIC) == M4_TRUE)
		{
			// sólo puedo borrar las dinámicas
			delete (*im).second;    // Delete the property
		}
	}
//## end ClColProp::~ClColProp%1310200079.body
}

//
// Function: ClolProp::Count
//
// Arguments: void
//
// Returns:
//   size_t - the number of entries in the map
//
// Description:
//   Returns the number of entries in the map
//
size_t ClColProp::Count(void) const
{
//## begin ClColProp::Count%-260292360.body preserve=yes
	return m_oMapStringVariant.size();
//## end ClColProp::Count%-260292360.body
}

//
// Function: ClColProp::GetPropName
//
// Arguments:
//   ai_ui - The index of the map
//
// Returns:
//   m4pchar_t - The returned string
//
// Description:
//   Given an index, returns the equivalent
//   name used to access the map
//
const m4pchar_t ClColProp::GetPropName(m4int32_t ai_ui) const
{
//## begin ClColProp::GetPropName%75188992.body preserve=yes
	m4uint16_t ui = 0;

	STRING2PROPERTY::const_iterator im = m_oMapStringVariant.begin();

	// Loop to the correct element or the end
	for (; ui != ai_ui && im != m_oMapStringVariant.end(); ++ui, ++im)
	{
		// Do nothing
	}

	if (ui != ai_ui || im == m_oMapStringVariant.end())
        return NULL;    // No se ha encontrado

    return (*im).first;
//## end ClColProp::GetPropName%75188992.body
}


m4uint8_t ClColProp::GetPropFlags( m4int32_t ai_ui, m4uint8_t ai_iFlag ) const
{
	m4uint16_t ui = 0;

	STRING2PROPERTY::const_iterator im = m_oMapStringVariant.begin();

	// Loop to the correct element or the end
	for (; ui != ai_ui && im != m_oMapStringVariant.end(); ++ui, ++im)
	{
		// Do nothing
	}

	if (ui != ai_ui || im == m_oMapStringVariant.end())
        return 0;    // No se ha encontrado

    return (*im).second->GetFlags( ai_iFlag ) ;
}


//
// Function:  ClColProp::GetPropValueById
//
// Arguments:
//   ai_ui - The index to the array
//
// Returns:
//   m4VariantType& - Refernce to the variant
//                    stored at the index
//
// Description:
//   Given an index returns the variant stored
//   at theat index in the map
//
m4return_t ClColProp::GetPropValue(m4int32_t ai_ui, m4VariantType& ao_var) const
{
//## begin ClColProp::GetPropValueById%-166786330.body preserve=yes
    m4uint16_t ui = 0;

	STRING2PROPERTY::const_iterator im = m_oMapStringVariant.begin();

	for (; ui != ai_ui && im != m_oMapStringVariant.end(); ++ui, ++im)
	{
		// Do nothing
	}

	if (ui != ai_ui || im == m_oMapStringVariant.end()) 
        return M4_ERROR;    // No se ha encontrado

	if( (*im).second->GetFlags( M4_PROP_READ_COL ) == M4_FALSE )
	{
		// No puedo acceder a una propiedad de no lectura
		DUMP_CHLOG_WARNINGF( M4_CH_DM_NO_READ_PROP, (*im).first ) ;
		ao_var.Type = M4CL_CPP_TYPE_NULL ;
		ao_var.Data.DoubleData = 0 ;
		return( M4_ERROR ) ;
	}

	return (*im).second->Get(ao_var);

//## end ClColProp::GetPropValueById%-166786330.body
}

//
// Function: ClColProp::GetPropValue
//
// Arguments:
//   ai_psz - The string used to index the map
//   ao_var - A reference to the variant associated with the given string
//
// Returns:
//    m4return_t
//
// Description:
//   Return the variant type associated with the given string
//
m4return_t ClColProp::_GetPropValue(const m4pchar_t ai_psz,
                                    m4VariantType&  ao_var,
                                    m4bool_t  ai_bCheckPublic) const
{
//## begin ClColProp::GetPropValue%-90416805.body preserve=yes
    // This is a map between a string and a pointer to a property if there is
    // no such string it enters a new entry and a calls the constructor of the type,
    // in this case a pointer. So if the pointer is null, we can delete the entry...
    STRING2PROPERTY::const_iterator it = m_oMapStringVariant.find(ai_psz);

    if (it == m_oMapStringVariant.end())
	{
        return M4_ERROR;    // La propiedad no existe
	}

    if( ai_bCheckPublic == M4_TRUE && ((*it).second)->GetFlags( M4_PROP_READ_COL ) == M4_FALSE )
	{
		// No puedo acceder a una propiedad de no lectura
		DUMP_CHLOG_WARNINGF( M4_CH_DM_NO_READ_PROP, ai_psz ) ;
		ao_var.Type = M4CL_CPP_TYPE_NULL ;
		ao_var.Data.DoubleData = 0 ;
		return( M4_ERROR ) ;
	}

    return (*it).second->Get(ao_var);
//## end ClColProp::GetPropValue%-90416805.body
}

m4return_t ClColProp::GetPropValue(const m4pchar_t ai_psz, m4VariantType& ao_var) const
{
    return _GetPropValue(ai_psz, ao_var, M4_TRUE);
}

m4return_t ClColProp::GetPrivatePropValue(const m4pchar_t ai_psz, m4VariantType& ao_var) const
{
    return _GetPropValue(ai_psz, ao_var, M4_FALSE);
}

//
// Function:  ClColProp::SetPropValue
//
// Arguments:
//   ai_psz - The string used as the map key
//   ai_stVariant - The variant associated with the string
//
// Returns: m4return_t
//
// Description:
//   Store the given variant in the map and associate it with given string
//   Si la propiedad no existe devuelve error
//
m4return_t ClColProp::_SetPropValue(const m4pchar_t      ai_psz,
                                    const m4VariantType& ai_var,
                                          m4bool_t       ai_bCheckPublic)
{
    STRING2PROPERTY::iterator it = m_oMapStringVariant.find(ai_psz);

    if (it == m_oMapStringVariant.end())
        return M4_ERROR;    // La propiedad no existe

    if( ai_bCheckPublic == M4_TRUE && ((*it).second)->GetFlags( M4_PROP_WRITE_COL ) == M4_FALSE )
	{
		/* Bug 0100799
		Si el valor es el mismo no se da ningún error
		*/
		m4VariantType	vValue ;

		(*it).second->Get( vValue ) ;

		if( vValue != ai_var )
		{
			// No puedo asignar a una propiedad de no escritura
			DUMP_CHLOG_WARNINGF( M4_CH_DM_NO_WRITE_PROP, ai_psz ) ;
			return( M4_ERROR ) ;
		}
	}

    return (*it).second->Set(ai_var);
}

m4return_t ClColProp::SetPropValue(const m4pchar_t ai_psz, const m4VariantType& ai_var)
{
    return _SetPropValue(ai_psz, ai_var, M4_TRUE);
}

m4return_t ClColProp::SetPrivatePropValue(const m4pchar_t ai_psz, const m4VariantType& ai_var)
{
    return _SetPropValue(ai_psz, ai_var, M4_FALSE);
}

//
// Function:  ClColProp::AddProperty
//
// Arguments:
//   ai_psz - The string used as the map key
//   ai_property - The property associated with the string
//
// Returns: m4return_t
//
// Description:
//   Store the given property in the map and associate it with given string
//
m4return_t ClColProp::AddProperty(const m4pchar_t ai_psz, ClProperty* ai_property)
{
	M4_ASSERT(ai_property);

	// Copy the name string
	m4pchar_t psz = new m4char_t[strlen(ai_psz) + 1];
	strcpy(psz, ai_psz);

	// First check if it exists
	ClProperty* tmpProp = m_oMapStringVariant[psz];

	// Whatever happened, now we have an entry in the map

	if (tmpProp != NULL)
	{
        // Ya existe una propiedad con ese nombre
		delete [] psz;
        return M4_ERROR;
	}

    m_oMapStringVariant[psz] = ai_property;

	ai_property->SetChangeListener( this ) ;

    return M4_SUCCESS;
}

//
// Function:  ClColProp::GetProperty
//
// Arguments:
//   ai_psz - The string used as the map key
//
// Returns: ClProperty* - puntero a una propiedad
//
// Description:
//   Devuelve la propiedad identificada por la cadena que se pasa como argumento.
//   Si la propiedad no se encuentra en el mapa devuelve NULL
//
ClProperty* ClColProp::GetProperty(const m4pchar_t ai_psz) const
{
    STRING2PROPERTY::const_iterator it = m_oMapStringVariant.find(ai_psz);

    if (it == m_oMapStringVariant.end())
        return NULL;    // La propiedad no existe

    if( ((*it).second)->GetFlags( M4_PROP_READ_COL ) == M4_FALSE )
	{
		// No puedo acceder a una propiedad de no lectura
		DUMP_CHLOG_WARNINGF( M4_CH_DM_NO_READ_PROP, ai_psz ) ;
		return( NULL ) ;
	}

    return (*it).second;
}

//
// Function:  ClColProp::_NewProperty
//
// Arguments:
//   ai_psz - The string used as the map key
//   ai_bIsMapped - Indica si la propiedad debe ser mapped
//   ai_isPublic - Indica si hay que crear una propiedad publica al LN4, o privada
//
// Returns: m4return_t
//
// Description:
//   Crea una nueva propiedad dinamica. Si ya existia una propiedad con el
//   nombre dado, la funcion devuelve error
//
m4return_t ClColProp::_NewProperty(const m4pchar_t ai_psz, m4uint8_t ai_iStatus )
{
	// Copy the name string
	m4pchar_t psz = new m4char_t[strlen(ai_psz)+1];
	strcpy(psz, ai_psz);

	// First check if it exists
	ClProperty* tmpProp = m_oMapStringVariant[psz];

	// Whatever happened, now we have an entry in the map

	if (tmpProp != NULL)
	{
        // Ya existia una propiedad con ese nombre
		delete [] psz;
        return M4_ERROR;
	}

	// Es siempre dinámica
	ai_iStatus |= M4_PROP_DYNAMIC ;

    tmpProp = new ClGenericProperty( ai_iStatus );
    m_oMapStringVariant[psz] = tmpProp;

	SetChange();
	tmpProp->SetChangeListener( this ) ;

    return M4_SUCCESS;
}

m4return_t ClColProp::NewProperty( const m4pchar_t ai_psz, m4uint8_t ai_iStatus )
{
    return _NewProperty( ai_psz, ai_iStatus ) ;
}

m4return_t ClColProp::NewPrivateProperty( const m4pchar_t ai_psz, m4uint8_t ai_iStatus )
{
    return _NewProperty(ai_psz, ai_iStatus ) ;
}

/*
 * Function: ClColProp::SetOrNewProperty
 *
 * Parameters:
 *       ai_psz - Nombre de la propiedad
 *       ai_var - Valor de la propiedad
 *
 * Return: m4return_t
 *
 * Definition:
 *       Intenta hacer un set de la propiedad especificada. Si esta no existe la crea
 * con valores por defecto (visible = si, serializable = si)
 */
m4return_t ClColProp::SetOrNewProperty( const m4pchar_t ai_psz, const m4VariantType& ai_var, m4uint8_t ai_iStatus )
{
    m4return_t iRet = SetPropValue(ai_psz, ai_var);

    if (iRet != M4_SUCCESS)
    {
        // La propiedad no existe, tengo que crearla
        iRet = NewProperty(ai_psz, ai_iStatus);

        if (iRet == M4_SUCCESS)
            iRet = SetPropValue(ai_psz, ai_var);
    }

    return iRet;
}

//
// Function:  ClColProp::DeleteProperty
//
// Arguments:
//   ai_psz - The string used as the map key
//
// Returns: m4return_t
//
// Description:
//   Borra una propiedad dinamica del mapa. Las propiedades estaticas solo se borran
//   del mapa cuando este se destruye
//
m4return_t ClColProp::_DeleteProperty(const m4pchar_t ai_psz, m4bool_t ai_bCheckPublic)
{
    STRING2PROPERTY::iterator it = m_oMapStringVariant.find(ai_psz);

    if (it == m_oMapStringVariant.end())
        return M4_ERROR;    // La propiedad no existe

    if( ai_bCheckPublic == M4_TRUE && ((*it).second)->GetFlags( M4_PROP_DELETE_COL ) == M4_FALSE )
	{
		// No puedo borrar las propiedades no borrables
		DUMP_CHLOG_WARNINGF( M4_CH_DM_DELETE_PROP, (*it).first ) ;
		return( M4_ERROR ) ;
	}

    if( ((*it).second)->GetFlags( M4_PROP_DYNAMIC ) == M4_FALSE )
	{
		// No puedo borrar las propiedades estaticas
		DUMP_CHLOG_WARNINGF( M4_CH_DM_DELETE_PROP, ai_psz ) ;
		return( M4_ERROR ) ;
	}

    delete [] (*it).first;  // Delete the string

    delete (*it).second;    // Delete the property

    m_oMapStringVariant.erase(it);  // Borramos la entrada del mapa

	SetChange();

    return M4_SUCCESS;
}

m4return_t ClColProp::DeleteProperty(const m4pchar_t ai_psz)
{
    return _DeleteProperty(ai_psz, M4_TRUE);
}

m4return_t ClColProp::DeletePrivateProperty(const m4pchar_t ai_psz)
{
    return _DeleteProperty(ai_psz, M4_FALSE);
}

//
// Function:  ClColProp::GetSize
//
// Arguments:
//   ao_size - 
//
// Returns: m4return_t
//
// Description:
//
m4return_t ClColProp::GetSize(ClSize &ao_size) const
{
	ao_size += sizeof(ClColProp);   //Incluye el sizeof del mapa
	ao_size -= sizeof(m_oMapStringVariant); 

	// El casting *(STRING2PROPERTY *)& es bastante ridiculo, pero sino no compila en AIX.
	if (GetMapStructureSize(*(STRING2PROPERTY *)&m_oMapStringVariant,ao_size) != M4_SUCCESS){
		return M4_ERROR;
	}

    STRING2PROPERTY::const_iterator it = m_oMapStringVariant.begin();

	for (; it != m_oMapStringVariant.end(); ++it)
	{
		ao_size += strlen((*it).first)+1;

		if ((*it).second->GetFlags( M4_PROP_DYNAMIC ) == M4_TRUE)
		{
			if ( (*it).second->GetSize(ao_size) != M4_SUCCESS){
				return M4_ERROR;
			}
		}
	}

	return M4_SUCCESS;
}

//
// Function:  ClColProp::Serialize
//
// Arguments:
//   IOD - IO driver generico
//   ai_iIteSerMod - Modo de serializacion
//
// Returns: m4return_t
//
// Description:
//   Serializa el mapa en el IOD dado
//

m4return_t ClColProp::Serialize( ClGenericIODriver& IOD, m4uint8_t ai_iIteSerMod )
{

    m4return_t					iResult ;
	m4uint8_t					iPropertyId ;
	STRING2PROPERTY::iterator	it ;


	it = m_oMapStringVariant.begin() ;

	while( it != m_oMapStringVariant.end() )
	{
        if( (*it).second->GetFlags( M4_PROP_MAPPED ) == M4_TRUE || ai_iIteSerMod == M4CL_ITEM_SERIAL_ALL )
        {
			iPropertyId = (*it).second->GetPropertyId() ;

			if( (*it).second->GetFlags( M4_PROP_DYNAMIC ) == M4_TRUE || (*it).second->GetFlags( M4_PROP_MAPPED ) == M4_FALSE )
			{
				// Flag de tipo cadena, más propiedad 
				iResult = IOD.Write( m4uint8_t( 0x080 | iPropertyId ) ) ;

				if( iResult != M4_SUCCESS )
				{
					return( M4_ERROR ) ;
				}

	            iResult = IOD.Write( (*it).first ) ;

				if( iResult != M4_SUCCESS )
				{
					return( M4_ERROR ) ;
				}
			}
			else
			{
				// Sino sólo la propiedad
				iResult = IOD.Write( m4uint8_t( iPropertyId ) ) ;

				if( iResult != M4_SUCCESS )
				{
					return( M4_ERROR ) ;
				}

			}

			iResult = (*it).second->Serialize( IOD ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
        }

		it++ ;
	}


	// El final es el -1
	iResult = IOD.Write( m4uint8_t( 0x0FF ) ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    return( M4_SUCCESS ) ;
}



#define MAX_PROPERTY_NAME_LENGTH 256

m4return_t ClColProp::DeSerialize( ClGenericIODriver& IOD, m4uint8_t ai_iType )
{

    m4return_t					iResult ;
    m4uint8_t					iInteger ;
    m4uint8_t					iPropertyId ;
	m4uint16_t					i ;
	m4uint16_t					iLength ;
    m4pchar_t					pcPropName ;
    m4char_t					acPropName[ MAX_PROPERTY_NAME_LENGTH + 1 ] ;
    ClProperty					*poTempProp ;
	STRING2PROPERTY::iterator	it ;


	i = 0 ;
	while( 1 )
    {
		// Leemos si hay cadena o no y el tipo de propiedad
		iResult = IOD.Read( iInteger ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		// Es -1 es el final
		if( iInteger == 0x0FF )
		{
			break ;
		}

		iPropertyId = iInteger & 0x00F ;

		if( ( iInteger & 0x080 ) != 0 )
		{
			// Si hay cadena leemos el nombre
	        iResult = IOD.Read( acPropName, MAX_PROPERTY_NAME_LENGTH ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
		else
		{
			if( ai_iType == 1 )
			{
				// Es canal
				pcPropName = g_acM4ObjProperties[ i ] ;
			}
			else
			{
				// Es acceso
				pcPropName = g_acAccessProperties[ i ] ;
			}
			iLength = strlen( pcPropName ) ;
			memcpy( acPropName, pcPropName, iLength ) ;
			acPropName[ iLength ] = '\0' ;
			i++ ;
		}


        // No se si tengo que crearla
		it = m_oMapStringVariant.find( acPropName ) ;

        if( it == m_oMapStringVariant.end() )
        {
            // La propiedad no existe
            // Creamos una nueva propiedad, segun corresponda
            switch( iPropertyId )
            {
            case e_ClGenericProperty:
                poTempProp = new ClGenericProperty ;
                break;

            case e_ClAutoLoadProperty:
                poTempProp = new ClAutoLoadProperty ;
                break;

			case e_ClRecordVersionProperty:
				poTempProp = new ClRecordVersionProperty ;
				break;

			case e_ClOrganizationProperty:
				poTempProp = new ClOrganizationProperty ;
				break;

            default:    // e_ClProperty:
				return( M4_ERROR ) ;
            }
            
            if( poTempProp == NULL )
			{
				return( M4_ERROR ) ;
			}

			iLength = strlen( acPropName ) ;
            pcPropName = new m4char_t[ iLength + 1 ] ;

            if( pcPropName == NULL )
			{
				delete( poTempProp ) ;
				return( M4_ERROR ) ;
			}

			memcpy( pcPropName, acPropName, iLength ) ;
			pcPropName[ iLength ] = '\0' ;

            m_oMapStringVariant[ pcPropName ] = poTempProp ;

			SetChange();
			poTempProp->SetChangeListener( this ) ;
        }
        else
        {
            // La propiedad ya existe
            poTempProp = (*it).second ;
        }


	    iResult = poTempProp->DeSerialize( IOD ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
    }


    return( M4_SUCCESS ) ;
}



ostream& operator<< ( ostream& ai_roStream, const ClColProp &ai_roColProp )
{

	size_t		i, iLength ;
	m4pchar_t	pcName ;
	ClProperty	*poProperty ;


	iLength = ai_roColProp.Count() ;
	ai_roStream << "Size: " << iLength << " " ;

	for( i = 0 ; i < iLength ; i++ )
	{
		ai_roStream << "{" ;

		pcName = ai_roColProp.GetPropName( i ) ;

		if( pcName != NULL )
		{
			ai_roStream << pcName ;
			poProperty = ai_roColProp.GetProperty( pcName ) ;

			if( poProperty != NULL )
			{
				ai_roStream << *poProperty ;
			}
		}

		ai_roStream << "}" ;
	}

	return( ai_roStream ) ;
}



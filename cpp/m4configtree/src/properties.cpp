
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           "properties" Class functions
// File:             Properties.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:2/7/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:ddd
//
//    Functions to manage the pool of properties
//
//
//==============================================================================

#include <m4log.hpp>
#include <m4srvres.hpp>
#include "properties.hpp"
#include <stdio.h>
#include <m4srvtrace.h>



#define M4_SRV_TRACE_COMP_ID	"CNF_T"

//----------------------------------------------------------------------------
//		Error tratement
//----------------------------------------------------------------------------




//----------------------------------------------------------------------------
//This function return the leng of a property value ended with -1
//----------------------------------------------------------------------------

m4int32_t StringLen ( m4pchar_t ai_szString )
{
	m4pchar_t pointer;
	m4int32_t iSize = -1;


	pointer = strchr (ai_szString , -1 ) ;

	if ( pointer )
	{
		iSize = pointer - ai_szString ;
	}

return ( iSize ) ;

}

//----------------------------------------------------------------------------
//		ClProperties destructor
//----------------------------------------------------------------------------

	ClProperties::~ClProperties()
	{
			m_STLproperties.clear();
	}

//----------------------------------------------------------------------------
//		GetProperty: Gets a single property
//----------------------------------------------------------------------------

m4return_t ClProperties::GetProperty(m4pchar_t ai_szProperty , m4pchar_t ao_szValue , m4int32_t ai_iBufferSize )
{

	tProperties::iterator	Iterator;
	PropertyValue::iterator itValue;
	m4int16_t				iLenProp;
	size_t					iLenValue ;

	// Bug 0129577: output value inicialization.
	strcpy(ao_szValue, "");

	//copy the property name to a vector of characters

	iLenProp = strlen ( ai_szProperty ) +1 ;

	PropertyName VtProperty ( ai_szProperty , ai_szProperty + iLenProp );


	// look for the property

	Iterator = m_STLproperties.find ( VtProperty );


	if ( Iterator == m_STLproperties.end() )
	{
		
//		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , DEBUGINFOLOG , "The property %s isn´t in the list" , ai_szProperty);

		return M4_ERROR;
	}

	iLenValue = ( * Iterator).second.size()  ;


	if ( iLenValue >= ai_iBufferSize )
	{
		M4_SRV_WARNING ( M4_SRV_CONFIG_PROP_BUFERROR , "Error getting property: %0:s. The size of the buffer for that roperty is %d and is too small",ai_szProperty << ai_iBufferSize);
//		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "The property value is longer than the buffer, Getting property %s " , ai_szProperty );

		return M4_ERROR;
	}


	//copy the property value in the buffer

	itValue = (*Iterator).second.begin();
	memcpy(ao_szValue, &(*itValue), iLenValue);

	return M4_SUCCESS;
}


//----------------------------------------------------------------------------
//				Set a numeric property
//----------------------------------------------------------------------------


	m4return_t ClProperties::SetProperty(m4pchar_t ai_szProperty, m4int32_t ai_iValue)
	{
		m4char_t szPropertyValue [20];


		sprintf( szPropertyValue , "%d" , ai_iValue ) ;

		return SetProperty ( ai_szProperty , szPropertyValue , strlen ( szPropertyValue ) ) ;
	}


//----------------------------------------------------------------------------
//	Get a numeric property
//----------------------------------------------------------------------------

	m4return_t ClProperties::GetProperty( m4pchar_t ai_szProperty , m4int32_t & ao_iProp)
	{
		m4pchar_t pProp;
		m4int16_t iRet;
		m4pchar_t szAux ;	


		iRet = GetpProperty ( ai_szProperty , pProp ) ;


		if ( iRet == M4_ERROR )
		{
			return M4_ERROR ;
		}

		szAux = new m4char_t [ strlen (pProp) ] ;

		if ( (!  sscanf ( pProp  , "%d\0" , &ao_iProp  )  ) 
			|| ( 2 == sscanf ( pProp  , "%d%s" , &ao_iProp  , szAux )  ) 
			|| ( ! strcmp ( "" , pProp  ) ) )
		{
			delete [] szAux ;
			return M4_ERROR ;
		}

		ao_iProp = atoi ( pProp ) ;

		delete [] szAux ;

		if ( (! ao_iProp) && ( pProp[0] != '0') )
		{
			return M4_ERROR ;
		}

		return M4_SUCCESS;

	}

//----------------------------------------------------------------------------
//		GetProperty: Gets the direction of a single property
//----------------------------------------------------------------------------

m4return_t ClProperties::GetpProperty(m4pchar_t ai_szProperty , m4pchar_t & ao_szValue )
{

	tProperties::iterator	Iterator;
	PropertyValue::iterator itValue;
	m4int16_t				iLenProp;


	//copy the property name to a vector of characters

	iLenProp = strlen ( ai_szProperty ) +1 ;

	PropertyName VtProperty ( ai_szProperty , ai_szProperty + iLenProp );


	// look for the property

	Iterator = m_STLproperties.find ( VtProperty );


	if ( Iterator == m_STLproperties.end() )
	{
//		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , DEBUGINFOLOG , "The property %s is not in the list" , ai_szProperty);

		return M4_ERROR;
	}

	//return the direction

	itValue = (*Iterator).second.begin();
	ao_szValue = &(*itValue);

	return M4_SUCCESS;
}


//----------------------------------------------------------------------------
//	SetProperty: Establece el valor de una propiedad
//----------------------------------------------------------------------------

m4return_t ClProperties::SetProperty (m4pchar_t ai_szProperty, m4pchar_t ai_szValue , m4int32_t ai_iSizeValue)
{
	tProperties::iterator Iterator;
	m4int16_t iLenProp = 0;


	iLenProp =  strlen ( ai_szProperty ) +1;


	//copy the property name to a vector
	PropertyName VtProperty( ai_szProperty , ai_szProperty + iLenProp );

	//copy the property value to a vector
	PropertyValue VtValue ( ai_szValue , ai_szValue + ai_iSizeValue +1 ) ; 

	//Comprobar que no exista la propiedad

	Iterator = m_STLproperties.find ( VtProperty );

	if ( Iterator != m_STLproperties.end() )
	{
//		m_STLproperties.erase(Iterator  );
		(*Iterator).second = VtValue ;
	}
	else
	{

		m_STLproperties.insert (tProperties::value_type ( VtProperty , VtValue ) );
	}

	return M4_SUCCESS;
}


//-------------------------------------------------------------
//Reads a string of properties with the conexion string structure
//		" PROPERTYNAME=PROPERTYVALUE;PROP..."
//	and insert each property in the pool of properties
//-------------------------------------------------------------
m4return_t ClProperties::SetStringProperties(m4pchar_t ai_pszProperties)
{
	size_t iLen;
	m4pchar_t pcBuffer, pcKey, pcValue;


	// Makes a copy for tokenization, appending a semicolon at the end if it is not there.
	iLen = strlen(ai_pszProperties);
	pcBuffer = new m4char_t[iLen + 2];
	strcpy(pcBuffer, ai_pszProperties);

	if (pcBuffer[iLen] != ';')
	{
		strcpy(pcBuffer + iLen, ";");
	}

	// Tokenize key-value pairs.
	pcKey = strtok(pcBuffer, ";");

	while (pcKey != NULL)
	{
		pcValue = strstr(pcKey, "=");

		if (pcValue != NULL)
		{
			*pcValue = 0;
			pcValue++;

			// Remove the quotes.
			if (*pcValue == '\"')
			{
				pcValue++;
				iLen = strlen(pcValue);

				if (pcValue[iLen - 1] == '\"')
				{
					pcValue[iLen - 1] = 0;
				}
			}

			SetProperty(pcKey, pcValue, strlen(pcValue));
		}

		pcKey = strtok(NULL, ";");
	}

	delete pcBuffer;


	return M4_SUCCESS;
}


//----------------------------------------------------------------------------
//Gets a string with all the asigned properties in the connection
//string format
//----------------------------------------------------------------------------

		m4return_t ClProperties::GetProperties ( m4pchar_t ao_szProperties, m4uint32_t ai_iBufferSize)
		{
			tProperties::iterator			Iterator;
			PropertyName::const_iterator	itName;
			PropertyValue::iterator			itValue;
			m4char_t						szProp[MAXNAMEPROP];
			m4char_t						szValue [MAXNAMEPROP] ;


			ao_szProperties[0] = 0;

			for ( Iterator = m_STLproperties.begin() ;
			Iterator != m_STLproperties.end();
			Iterator ++ )
			{
				//Pass to the buffer the property name
				itName = (*Iterator).first.begin();
				memcpy ( szProp , &(*itName), (*Iterator).first.size() );

				szProp [(*Iterator).first.size()] = 0 ;

				//pass to the buffer the property value
				itValue = (*Iterator).second.begin();
				memcpy ( szValue , &(*itValue), (*Iterator).second.size() );

				szValue [ (* Iterator).second.size() ] = 0;

				//chack for the space in the destination buffer
				if ( (strlen (ao_szProperties) + strlen(szProp) + strlen (szValue) + 2 ) > ai_iBufferSize )
				{
//					SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "The output buffer for properties is too small. Function : 'GetProperties'" );
//					M4_SRV_WARNING ( M4_SRV_CONFIG_PROP_BUFERROR , "Error getting property: %0:s. The size of the buffer for that roperty is %d and is too small",ai_szProperty << ai_iBufferSize);

					return M4_ERROR;
				}


				strcat(ao_szProperties , szProp );

				strcat (ao_szProperties , "=" );

				strcat (ao_szProperties , szValue ) ;

				strcat (ao_szProperties , ";" );

			}

			return M4_SUCCESS;
		}

//----------------------------------------------------------------------------
//		Test if the property is in the pool of properties
//----------------------------------------------------------------------------

	m4bool_t ClProperties::IsProperty ( m4pchar_t ai_szProperty ) 
	{

	tProperties::iterator Iterator;
	m4int16_t iLenProp;


	//copy the property name to a vector of characters

	iLenProp = strlen ( ai_szProperty ) +1 ;

	PropertyName VtProperty ( ai_szProperty , ai_szProperty + iLenProp );


	// look for the property

	Iterator = m_STLproperties.find ( VtProperty );

	if ( Iterator == m_STLproperties.end() )
	{
		return M4_FALSE;
	}

	return M4_TRUE;

	}

//----------------------------------------------------------------------------
//		Clear the pool of properties
//----------------------------------------------------------------------------

		m4return_t ClProperties::Vaciar ( )
		{
			m_STLproperties.clear();

			return M4_SUCCESS;
		}
//----------------------------------------------------------------------------
//		Encontrar la primera ocurrencia de la propiedad que case con el patron
//----------------------------------------------------------------------------

		m4return_t ClProperties::MatchFirstProp (m4pchar_t ai_szPatron, m4pchar_t ao_szProperty , m4uint32_t ai_iPropMaxLen , m4pchar_t ao_szValue , m4uint32_t ai_iTamMaxValue , m4uint32_t & ao_iHndProp , void * & ao_iHndMatch)
		{
			m4pchar_t						szProp = NULL ;
			tProperties::iterator *			pIterator = new tProperties::iterator;
			PropertyName::const_iterator	itName;
			PropertyValue::iterator			itValue;
			m4return_t						iRet = M4_ERROR ;
			m4uint32_t						iLenValue  ;

			ao_iHndProp = 0;

			(*pIterator) = m_STLproperties.begin() ;

			if ( ( *pIterator) == m_STLproperties.end() )
			{
				return M4_ERROR ;
			}

			do {
//				szProp = (**pIterator).first.begin() ;
				itName = (**pIterator).first.begin();
				szProp = (m4pchar_t)(&(*itName));
				if (strstr(szProp, ai_szPatron))
				{
					ao_iHndMatch = (void*)pIterator ;

					if (strlen(szProp) >= ai_iPropMaxLen)
						break ;

					strcpy ( ao_szProperty , szProp ) ;

					iRet = M4_SUCCESS ;

					break ;
				}

				(*pIterator)++;
			}
			while ( (*pIterator) != m_STLproperties.end()  ) ;

			if ( M4_SUCCESS == iRet )
			{
				iLenValue = ( ** pIterator).second.size()  ;


				if ( iLenValue >= ai_iTamMaxValue )
				{
					M4_SRV_WARNING ( M4_SRV_CONFIG_PROP_BUFERROR , "Error getting property: %0:s. The size of the buffer for that roperty is %d and is too small",ao_szProperty << ai_iTamMaxValue);
//					SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "The property value is longer than the buffer, Getting property %s " , ao_szProperty   );

					return M4_ERROR;
				}


	//copy the property value in the buffer
				itValue = (**pIterator).second.begin();
				memcpy ( ao_szValue, &(*itValue), iLenValue ) ;

			}

//		delete pIterator ;

		return iRet ;

		}

//----------------------------------------------------------------------------
//		Encontrar la siguiente ocurrencia de la propiedad que case con el patron
//----------------------------------------------------------------------------
		m4return_t ClProperties::MatchNextProp (m4pchar_t ai_szPatron , m4pchar_t  ao_szProp , m4uint32_t ai_iPropMaxLen , m4pchar_t ao_szValue , m4uint32_t ai_iTamMaxValue , m4uint32_t & aio_iHndProp , void * & aio_iHndMatch )
		{
			tProperties::iterator *			pIterator;
			PropertyName::const_iterator	itName;
			PropertyValue::iterator			itValue;
			m4return_t						iRet = M4_ERROR ;
			m4pchar_t						szProp = NULL ;
			m4uint32_t						iLenValue  ;

			//Inicializar el iterador (tamaño 12 bytes) con el iterador resultante de la otra búsqueda


			pIterator = (tProperties::iterator *)aio_iHndMatch ;

			if ((*pIterator) == m_STLproperties.end() )
			{
				return M4_ERROR ;
			}

			(*pIterator) ++ ;

			while ( (*pIterator) != m_STLproperties.end()  )
			{

//				szProp = (**pIterator).first.begin() ;
				itName = (**pIterator).first.begin();
				szProp = (m4pchar_t)(&(*itName));
				if ( strstr(szProp, ai_szPatron ) )
				{
					aio_iHndMatch = (void*) pIterator ;;

					if ( strlen(szProp) >= ai_iPropMaxLen )
						break ;

					strcpy ( ao_szProp , szProp ) ;

					iRet = M4_SUCCESS ;

					break ;
				}

				(*pIterator)++;
			}

			if ( M4_SUCCESS == iRet )
			{

				iLenValue = ( ** pIterator).second.size()  ;


				if ( iLenValue >= ai_iTamMaxValue )
				{
					M4_SRV_WARNING ( M4_SRV_CONFIG_PROP_BUFERROR , "Error getting property: %0:s. The size of the buffer for that roperty is %d and is too small",ao_szProp << ai_iTamMaxValue);
//					SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "The property value is longer than the buffer, Getting property %s " , ao_szProp );

					return M4_ERROR;
				}


	//copy the property value in the buffer
				itValue = (**pIterator).second.begin();
				memcpy (ao_szValue, &(*itValue), iLenValue) ;

			}

		return iRet ;

		}


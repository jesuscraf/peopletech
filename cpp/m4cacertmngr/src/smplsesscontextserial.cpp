//	=====================================================================
//
//	 (c) Copyright 2005 Meta4 Spain S.A.
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              <file>.cpp
//	 Project:           <project>
//	 Author:            Meta4 Spain S.A.
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS, UNIX 
//	 Design Document:
//
//	 Definition:
//          certificate management
//	=====================================================================

#include <smplsesscontextserial.hpp>
#include <sesscontextserial.hpp>
#include <m4trace.hpp>

#define NUM_ZEROS 6

ClSimpleSessContextSerial::ClSimpleSessContextSerial()
{
}

ClSimpleSessContextSerial::~ClSimpleSessContextSerial()
{
}

//-------------------------------------------------------------------------------------------
// [+] Auxiliary utilities (TODO - prototipe)
//-------------------------------------------------------------------------------------------

m4return_t addZeros (int ai_iNumberOfZeros, m4pchar_t ai_sRepresentingInteger, m4string_t &ao_sResult)
{

	m4string_t sLeadingZeros; 

	int iLength = strlen(ai_sRepresentingInteger);  
	
	if ( ai_iNumberOfZeros < iLength ) 
	{
		return M4_ERROR;  
	}
	
	int iZeros = ai_iNumberOfZeros - iLength; 

	for (int i=0; i < iZeros; i++) 
	{
		sLeadingZeros = "0" + sLeadingZeros;
	}

	ao_sResult =  ao_sResult + sLeadingZeros + ai_sRepresentingInteger ; 
	
	return M4_SUCCESS;  

}

m4return_t skipZeros (m4string_t &ai_sStringToReturn, m4string_t &ai_sStringToRead, int ai_iNumberOfZeros)
{
        
		m4bool_t bNonZeroFound = M4_FALSE; 

		int iLength = ai_sStringToRead.size();  
	
		if ( ai_iNumberOfZeros > iLength ) 
		{
			return M4_ERROR;  
		}

        for (int i=0; i<ai_iNumberOfZeros; i ++) 
        {
            if (bNonZeroFound) break; 

            if (ai_sStringToRead[i] != '0')
            {
                ai_sStringToReturn = ai_sStringToRead.substr (i, ai_iNumberOfZeros); 
                bNonZeroFound = M4_TRUE; 
            }
        }
	
		/* Bug 0114323
		Si la cadena no tiene longitud hay que inicializar la variable de salida,
		porque sino se queda con el valor anterior
		*/
		if( bNonZeroFound == M4_FALSE )
		{
			ai_sStringToReturn = "0" ;
		}

		return M4_SUCCESS; 
}

m4return_t getNextInfoField (m4string_t &ao_sSerializedString, m4string_t &ao_sNextInteger, int ai_iNumberOfZeros) 
{	 	

	m4return_t iRes = M4_SUCCESS;
	
	m4string_t sNextInfoField; 
	
	sNextInfoField = ao_sSerializedString.substr(0, ai_iNumberOfZeros);

	iRes = skipZeros (ao_sNextInteger, sNextInfoField, ai_iNumberOfZeros);  
	
	if (iRes == M4_SUCCESS) 
	{
		ao_sSerializedString = ao_sSerializedString.substr(ai_iNumberOfZeros, ao_sSerializedString.size()); 	
	}
	return iRes;  
}


//-------------------------------------------------------------------------------------------
// [+] Methods in the interface
//-------------------------------------------------------------------------------------------


/**
* Inserts an attribute name/value pair in the context map
* @param ai_strAttributeName	Name
* @param ai_strAttributeValue	Value
* @return						Error or Success
*/

m4return_t ClSimpleSessContextSerial::AddAtribute (m4string_t ai_strAttributeName, m4string_t ai_strAttributeValue)
{
	m_oContextMap [ai_strAttributeName] = ai_strAttributeValue; 	
	return M4_SUCCESS ;
}

/**
* Converts the map into a serialized string
* @param ao_pBuffer		Can be null (method usage to know the string length) or not (output is the result).  
* @param aio_bufLen		If buffer passed null, returns the string length (output). 
* @return				Error or Success
*/

m4return_t ClSimpleSessContextSerial::Serialize (m4pchar_t &ao_pBuffer, m4uint32_t &aio_bufLen)
{
  
	m4return_t iRes = M4_SUCCESS;

	// Primero tengo que calcular la longitud del buffer de salida. 
	m4string_t sSerializedString; 

   	m4string_t sKey; 
	m4string_t sValue; 

	m4pchar_t pLength = new char[12]; 
	m4uint32_t iLength = 0; 
	
	map<m4string_t, m4string_t>::iterator oContextIter;
	
	sprintf(pLength,"%d", m_oContextMap.size());

	iRes = addZeros (NUM_ZEROS, pLength, sSerializedString);

	if (iRes == M4_ERROR) 
	{
		return iRes; 
	}

    for(oContextIter = m_oContextMap.begin(); oContextIter != m_oContextMap.end(); oContextIter++)
    {

		  sKey = (*oContextIter).first; 
		  sprintf(pLength,"%d",strlen(sKey.c_str()));
		  iRes = addZeros (NUM_ZEROS, pLength, sSerializedString);
		
		  if (iRes == M4_ERROR) 
		  {
			return iRes; 
		  }

		  
		  sSerializedString += sKey; 
		 
		  sValue = oContextIter->second;
		  sprintf(pLength,"%d", strlen(sValue.c_str()));	
		  iRes = addZeros (NUM_ZEROS, pLength, sSerializedString);

		  if (iRes == M4_ERROR) 
		  {
			return iRes; 
		  }
    
		  sSerializedString += sValue; 
		  
	}


	// En la primera pasada miro la longitud que debo asignar por fuera.
	iLength = strlen(sSerializedString.c_str()); 
	if (ao_pBuffer == NULL) 
	{
		aio_bufLen = iLength ;
		return M4_SUCCESS;
	}

	// En la segunda pasada acepto la longitud. 
	if (iLength > aio_bufLen) {
		return M4_ERROR;
	}

	aio_bufLen = iLength;
	
	strcpy(ao_pBuffer, sSerializedString.c_str());
	
	delete pLength;

	return iRes;
}

/**
* Converts the serialized string into a map. 
* @param ao_pBuffer		Can be null (method usage to know the string length) or not (output is the result).  
* @param aio_bufLen		If buffer passed null, returns the string length (output). 
* @return				Error or Success
*/

m4return_t ClSimpleSessContextSerial::Deserialize (m4pchar_t ai_pBuffer, m4uint32_t ai_iBufLen)
{

	m4return_t iRes = M4_SUCCESS;

	m4string_t sSerializedString = ai_pBuffer; 
	
	m4string_t sKey; 
	m4string_t sValue; 

	int iMapSize = 0; 

	m4string_t sNextInteger; 

	m4Trace( cout << "Deserializing Context of Size: " << sSerializedString.size() << endl );
	m4Trace( cout << sSerializedString << endl ); 
	
	iRes = getNextInfoField(sSerializedString, sNextInteger, NUM_ZEROS); 

	if (iRes == M4_ERROR) 
	{
		return iRes; 
	}

	iMapSize = atoi(sNextInteger.c_str());

	int iCharsToRead = 0; 

    for (int i = 0; i< iMapSize; i++)
    {

        sKey = "";
        sValue = "";         

        iRes = getNextInfoField(sSerializedString, sNextInteger, NUM_ZEROS);

		if (iRes == M4_ERROR) 
		{
			return iRes; 
		}

        iCharsToRead = atoi(sNextInteger.c_str());
	
        for (int j = 0; j < iCharsToRead; j++)
        {
        	sKey = sKey + sSerializedString[j];
        }

        sSerializedString = sSerializedString.substr (iCharsToRead, sSerializedString.size());   
    
        iRes = getNextInfoField(sSerializedString, sNextInteger, NUM_ZEROS);
		
		if (iRes == M4_ERROR) 
		{
			return iRes; 
		}


        iCharsToRead = atoi(sNextInteger.c_str());

        for (int k = 0; k < iCharsToRead; k++)
        {
        	sValue = sValue + sSerializedString[k];                 
        }

        sSerializedString = sSerializedString.substr (iCharsToRead, sSerializedString.size());

		m_oContextMap [sKey] = sValue; 
                
        } 

	return iRes; 
}


/**
* Returns attribute name/value pair from the context map
* @param ai_strAttributeName	Name
* @param ai_strAttributeValue	Value
* @return						Error (if attribute not found) or Success (if found). 
*/

m4return_t ClSimpleSessContextSerial::GetAtribute (m4string_t ai_strAtribute, m4string_t &ao_strValue)
{
	map <m4string_t, m4string_t>::iterator iter; 

	iter = m_oContextMap.find (ai_strAtribute);

	if (iter == m_oContextMap.end())
	{
		return M4_ERROR;
	}

	ao_strValue = (*iter).second;
	
	return M4_SUCCESS ;
}


/**
* Returns an iterator to the map
* @return						SessionIterator
*/

SessionIterator ClSimpleSessContextSerial::GetIterator() {
	
	SimpleSessionIterator *poIterator = new SimpleSessionIterator();
	poIterator->Init(m_oContextMap);
	
	return poIterator;
}



// -------------------------------------------------------
// class SimpleSessionIterator implementation
// -------------------------------------------------------

SimpleSessionIterator::SimpleSessionIterator() {
	Reset();
}

void SimpleSessionIterator::Init (map<m4string_t, m4string_t> ai_oContextMap) {
	m_oContextMap = ai_oContextMap;
	m_iPos = -1;
	Next();
}

void SimpleSessionIterator::Reset() {
	m_iNRef = 0;
	m_pcKey = NULL;
	m_pcValue = NULL;
}

m4pcchar_t SimpleSessionIterator::GetKey() {
	return m_pcKey;
}

m4pcchar_t SimpleSessionIterator::GetValue() {
	return m_pcValue;
}

void SimpleSessionIterator::Next() {
	
	if ( m_iPos == -1 ) 
	{
		m_oContextIter = m_oContextMap.begin(); 
		m_iPos++; 
	}

	if (m_oContextIter != m_oContextMap.end())
	{
		m_pcKey = ((*m_oContextIter).first).c_str();
		if (m_pcKey == NULL ) 
		{
			Reset(); 
			return;
		}
		m_pcValue = (m_oContextIter->second).c_str();
		m_oContextIter++;
		m_iPos++;
	}
	else
	{
		Reset();
	}
		
	return; 

}


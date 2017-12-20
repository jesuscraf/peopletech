//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                presentation.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                16-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines thefunctions of the class of the factory of
//    compiled metachannels
//
//
//==============================================================================


#include "m4types.hpp"
#include "clpresen.hpp"
#include "cldates.hpp"
#include "asndef.h"
#include "prescache.hpp"
#include "m4magicn.hpp"
#include "m4presres.hpp"
#include "m4unicode.hpp"



ClPresentation :: ClPresentation( void ) : ClPersistiableObject ( 0 )
{
	m_pBufferASN = 0;

	m_pCO = NULL;
}

ClPresentation :: ClPresentation( CBufferASN	*ai_pBufferASN ) : ClPersistiableObject ( 0 )
{
	m_pBufferASN = ai_pBufferASN;

	m_pCO = NULL;
}

ClPresentation :: ~ClPresentation( void )
{
	if (m_pBufferASN)
		delete m_pBufferASN;

	m_pCO = NULL;
} 

void ClPresentation :: Destroy( m4bool_t ai_bRemoveFromCache )
{
	// Si esta en la cache (m_pCO != NULL) quita una referencia al objecto,
	// ademas si ai_bRemoveFromCache es TRUE, se elimina el objeto de la cache y se le pasa a la cola de borrado.
	// Si no esta en la cache, lo destruye directamente.

    if( m_pCO != NULL )
    {
		if (ai_bRemoveFromCache == M4_TRUE)
			((ClPresentationContainer*)m_pCO)->RemoveObjectFromCache();

		((ClPresentationContainer*)m_pCO)->FreeObjectFromCache();
    }
	else
	{
		delete this;
	}
}

m4uint32_t	ClPresentation :: GetSize( void ) const
{
	if (m_pBufferASN)
		return( m_pBufferASN->length ) ;
	else
		return 0;
}

m4uchar_t *	ClPresentation :: GetBuffer( void ) const 
{
	if (m_pBufferASN)
		return( m_pBufferASN->asn ) ;
	else
		return 0;
}

CBufferASN * ClPresentation :: GetBufferASN( void ) const 
{
	return m_pBufferASN;
}


m4return_t ClPresentation :: BuildXML( ClLongString &ao_roXML, m4pcchar_t ai_pccOrganization, ClChannelManager *ai_poChannelManager ) const
{

    m4return_t	iResult;
	m4uchar_t	byte;
	m4uint32_t	bytes;
	CAsn		oAsn;


	bytes = GetSize();
	CInputStream oInput( GetBuffer(), bytes );

	// La longitud en xml es algo menor que en asn1
	ao_roXML.Init( bytes, bytes / 10 ) ;

	ao_roXML.Concat( M4XMLHeader() ) ;

	oInput.readByte( byte );

	if( byte != ASNSequence )
	{
		return M4_ERROR;
	}

	oInput.readLength( bytes );
	iResult = oAsn.codeXml( &oInput, bytes, ao_roXML, ai_pccOrganization, ai_poChannelManager );

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF( M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


m4return_t  ClPresentation :: Serialize( ClGenericIODriver &IOD )
{
	m4return_t ret;
	m4bool_t bExists = M4_FALSE;

	ret = IOD.Write((m4uint32_t)M4_CH_MAGIC_NUMBER_PRES);

	if (ret == M4_ERROR)
		return M4_ERROR;

	ret = IOD.Write((m4uint16_t)M4_VM_PRES_VER);

	if (ret == M4_ERROR)
		return M4_ERROR;

	ret = IOD.Write(m_pBufferASN, &bExists);

	if (ret == M4_ERROR)
		return M4_ERROR;

	if ((!bExists) && (m_pBufferASN))
	{
		ret = IOD.Write((void*)m_pBufferASN->asn, m_pBufferASN->length);

		if (ret == M4_ERROR)
			return M4_ERROR;

		ret = IOD.Write((void*)m_pBufferASN->ref, m_pBufferASN->lengthRef);

		if (ret == M4_ERROR)
			return M4_ERROR;
		
		ret = IOD.Write(m_pBufferASN->version);

		if (ret == M4_ERROR)
			return M4_ERROR;

		ret = IOD.Write(m_pBufferASN->style);

		if (ret == M4_ERROR)
			return M4_ERROR;
		
		m4uint32_t i, iNumT3 = m_pBufferASN->numT3;
		m4pchar_t pcT3Id;
		m4double_t dT3Version;

		ret = IOD.Write(iNumT3);

		if (ret == M4_ERROR)
			return M4_ERROR;

		for (i = 0; i < iNumT3; i++)
		{
			pcT3Id = m_pBufferASN->GetIdT3ByIndex(i);
			dT3Version = m_pBufferASN->GetVersionT3ByIndex(i);

			ret = IOD.Write(pcT3Id);

			if (ret == M4_ERROR)
				return M4_ERROR;

			ret = IOD.Write(dT3Version);

			if (ret == M4_ERROR)
				return M4_ERROR;
		}

		//--------------------------------------------------------------
		//Vamos a meter el conjunto de includes que tiene la presentación.
		m4int32_t iSizeIncludes = m_pBufferASN->GetNumInclude();
		ret = IOD.Write((m4uint32_t)iSizeIncludes);
		if (ret == M4_ERROR) {
			DUMP_CHLOG_ERRORF( M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
			return M4_ERROR;
		}

		for (i=0; i<iSizeIncludes; i++) {
			string sIdInclude = m_pBufferASN->GetIdIncludeByPos(i);

			if (sIdInclude.size() > 0) {
				ret = IOD.Write((void*)sIdInclude.c_str(), sIdInclude.size());
				if (ret == M4_ERROR) {
					DUMP_CHLOG_ERRORF( M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
					return M4_ERROR;
				}

				m4date_t dVersionInclude = m_pBufferASN->GetVersionIncludeByPos(i);
				ret = IOD.Write(dVersionInclude);
				if (ret == M4_ERROR) {
					DUMP_CHLOG_ERRORF( M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
					return M4_ERROR;
				}
			}
		}
		//--------------------------------------------------------------
	}

	return ret;
}


m4return_t  ClPresentation :: DeSerialize( ClGenericIODriver &IOD )
{
	m4return_t ret;
	m4bool_t bExists = M4_FALSE;
	m4uint32_t iSize = 0, iSizeRef = 0;
	m4uchar_t * pcAsn = 0, * pcRef = 0;
	m4uint16_t	iPresVer;
	m4uint32_t iMagicNumber;

	ret = IOD.Read(iMagicNumber);

	if (ret == M4_ERROR)
		return M4_ERROR;

	if (iMagicNumber != M4_CH_MAGIC_NUMBER_PRES)
	{
		g_oChLog<<BeginError(M4_CH_PRES_BAD_PRES_MAGIC_NUMBER)<<iMagicNumber<<m4uint32_t(M4_CH_MAGIC_NUMBER_PRES)<<EndLog;
		return M4_ERROR;
	}

	ret = IOD.Read(iPresVer);

	if (ret == M4_ERROR)
		return M4_ERROR;
	
	if (M4_VM_PRES_VER != iPresVer)
	{
		DUMP_CHLOG_ERRORF( M4_CH_PRES_BAD_PRES_VERSION, iPresVer << M4_VM_PRES_VER ) ;
		return M4_ERROR;
	}	

	ret = IOD.Read((void**)&m_pBufferASN, &bExists);

	if (ret == M4_ERROR)
		return M4_ERROR;

	if (!bExists)
	{
		ret = IOD.ReadSize(iSize);
    
		if (ret==M4_ERROR)
			return M4_ERROR;

		if (iSize)
		{
			pcAsn = new m4uchar_t[iSize];

			ret = IOD.ReadBuffer(pcAsn);

			if (ret == M4_ERROR)
			{
				delete [] pcAsn;			
				m_pBufferASN = 0;
				return M4_ERROR;
			}
		}

		ret = IOD.ReadSize(iSizeRef);
    
		if (ret == M4_ERROR)
		{
			if (pcAsn) 
				delete [] pcAsn;		
			
			m_pBufferASN = 0;
			return M4_ERROR;
		}

		if (iSizeRef)
		{
			pcRef = new m4uchar_t[iSizeRef];

			ret = IOD.ReadBuffer(pcRef);

			if (ret == M4_ERROR)
			{
				if (pcAsn) 
					delete [] pcAsn;			

				if (pcRef) 
					delete [] pcRef;			

				m_pBufferASN = 0;

				return M4_ERROR;
			}
		}

		m_pBufferASN = new CBufferASN();

		m_pBufferASN->length = iSize;
		m_pBufferASN->asn = pcAsn;

		m_pBufferASN->lengthRef = iSizeRef;
		m_pBufferASN->ref = pcRef;

		ret = IOD.Read(m_pBufferASN->version);

		if (ret == M4_ERROR)
			return M4_ERROR;

		ret = IOD.Read(m_pBufferASN->style);

		if (ret == M4_ERROR)
			return M4_ERROR;

		m4uint32_t i, iNumT3;
		m4uint32_t iSize = 0;
		m4pchar_t pcT3Id;
		m4double_t dT3Version;

		ret = IOD.Read(iNumT3);

		if (ret == M4_ERROR)
			return M4_ERROR;
		
		m_pBufferASN->numT3 = iNumT3;

		for (i = 0; i < iNumT3; i++)
		{
			ret = IOD.ReadSize(iSize);

			if (ret == M4_ERROR)
				return M4_ERROR;

			pcT3Id = new m4char_t[iSize+1];

			ret = IOD.ReadBuffer(pcT3Id);

			if (ret == M4_ERROR)
			{
				delete [] pcT3Id;
				return M4_ERROR;
			}

			pcT3Id[iSize] = '\0';

			ret = IOD.Read(dT3Version);

			if (ret == M4_ERROR)
				return M4_ERROR;

			m_pBufferASN->listT3[i] = pcT3Id;
			m_pBufferASN->listVersionT3[i] = dT3Version;
		}

		//--------------------------------------------------------------
		//Vamos a obtener la lista de includes.
		m4uint32_t iSizeIncludes = 0;
		m4uint32_t iSizeIdInclude = 0;
		m4pchar_t pcIdInclude = NULL;
		ret = IOD.ReadSize(iSizeIncludes);
		if (ret == M4_ERROR) {
			DUMP_CHLOG_ERRORF( M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
			return M4_ERROR;
		}

		for (i=0; i<iSizeIncludes; i++) 
		{
			ret = IOD.ReadSize(iSizeIdInclude);    
			if (ret == M4_ERROR) {
				DUMP_CHLOG_ERRORF( M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
				return M4_ERROR;
			}

			if (iSizeIdInclude > 0) {

				pcIdInclude = new m4char_t[iSizeIdInclude + 1];

				ret = IOD.ReadBuffer(pcIdInclude);
				if (ret == M4_ERROR) {
					delete [] pcIdInclude;
					DUMP_CHLOG_ERRORF( M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
					return M4_ERROR;
				}
				pcIdInclude[iSizeIdInclude] = NULL;
				string sIdInclude(pcIdInclude);

				delete [] pcIdInclude;

				//Metemos la fecha de ultima actualizacion (contando sus includes)
				m4date_t dVersionIncluded;
				ret = IOD.Read(dVersionIncluded);
				if (ret == M4_ERROR) {
					DUMP_CHLOG_ERRORF( M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
					return M4_ERROR;
				}
				if (m_pBufferASN->AddInfoIncludeInBufferASN(sIdInclude, dVersionIncluded) == M4_ERROR) {
					return M4_ERROR;
				}
			}
		}
		//--------------------------------------------------------------
	}

	return ret;
}


m4date_t ClPresentation :: GetVersion( void ) const
{
	if (m_pBufferASN)
	{
		return( m_pBufferASN->version ) ;
	}
	return( M4CL_MINUS_INFINITE_IN_JULIAN ) ;
}

m4uint16_t ClPresentation :: GetStyle( void ) const
{
	if (m_pBufferASN)
	{
		return( m_pBufferASN->style ) ;
	}
	return( 0 ) ;
}

m4uint32_t ClPresentation :: GetNumT3 ( void ) const
{
	if (m_pBufferASN)
	{
		return( m_pBufferASN->numT3 ) ;
	}
	return( 0 ) ;
}

m4pchar_t ClPresentation :: GetIdT3ByIndex ( m4uint32_t iIndex ) const
{
	if (m_pBufferASN)
	{
		return( m_pBufferASN->GetIdT3ByIndex(iIndex) ) ;
	}
	return( "" ) ;
}


m4date_t ClPresentation :: GetVersionT3ByIndex ( m4uint32_t iIndex ) const
{
	if (m_pBufferASN)
	{
		return( m_pBufferASN->GetVersionT3ByIndex(iIndex) ) ;
	}
	return( M4CL_MINUS_INFINITE_IN_JULIAN ) ;
}

m4uint32_t ClPresentation :: GetNumInclude ( void ) const
{
	if (m_pBufferASN != NULL)
	{
		return m_pBufferASN->GetNumInclude();
	}
	return 0;
}

string ClPresentation::GetIdIncludeByPos ( m4uint32_t ai_iPos ) const
{
	if (m_pBufferASN != NULL)
	{
		return m_pBufferASN->GetIdIncludeByPos(ai_iPos);
	}
	return "";
}

m4date_t ClPresentation::GetVersionIncludeByPos ( m4uint32_t ai_iPos )
{
	if (m_pBufferASN != NULL)
	{
		return m_pBufferASN->GetVersionIncludeByPos(ai_iPos);
	}
	return M4CL_MINUS_INFINITE_IN_JULIAN;
}


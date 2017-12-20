//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// File:             Asn.cpp
// Project:          Service presentation
// Author:           Meta Software M.S. , S.A
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Implementation of the CAsn class.
//
//
//==============================================================================

#include "asndef.h"
#include "cldates.hpp"
#include "chlog.hpp"
#include "m4presres.hpp"
#include "replaceliterals.hpp"
#include "m4unicode.hpp"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAsn::CAsn()
{
	present = new CComponent();
}

CAsn::~CAsn()
{
	delete present;
}

void CAsn::decode(m4bool_t traducir)
{
	COutputStream *m_aux = new COutputStream();
	present->decode(m_aux,traducir);		
	delete m_aux;
}

m4return_t  CAsn::code(ClChannelManager *ai_poChannelManager, m4bool_t traducir, CInputStream *ai_pIn, CAsn *ai_pReference, m4bool_t ai_bIsDef, m4uint32_t ai_iReferences, CAsn *ai_paMultiReference, m4pchar_t *ai_ppcLangCodes, CComponent *ai_oParent)
{

   	m4uchar_t b_T, b_L, b_V;
	m4uint32_t i;	
	m4uint32_t lengthASN = 0;
	m4int32_t length_code;
	m4bool_t acabar = M4_FALSE;

	if (ai_oParent == NULL) 
	{
		ai_oParent = present; 
	}
		
	// este es el método donde empezamos a construir el arbolillo 
	ai_pIn->readByte(b_T);
	
	while( ai_pIn->eof() == 0 && !acabar )
	{
		if (b_T==ASNSequence)
		{
			present->SetTypeProp(T_SEQ);
			ai_pIn->readByte(b_L);
			if(b_L>128)
			{
				b_L -= 128;
				for (i=b_L;i>0;i--)
				{
					m4uint32_t byte;
					ai_pIn->readByte(b_V);
					byte=b_V;
					lengthASN+=(byte<<8*(i-1));
				}
			}
			else
			{
				lengthASN = b_L;
			}
			present->SetLength(lengthASN);
			if(traducir)
			{
				present->length_prop = lengthASN;
			}

			i = 0; 
			while( i < lengthASN && !acabar )
			{
				CComponent *comp = new CComponent();

				if (traducir)
				{
					length_code = comp->codeReferences(ai_pIn);
				}
				else
				{
					length_code = comp->code (ai_poChannelManager, ai_pIn, ai_pReference, ai_bIsDef, ai_iReferences, ai_paMultiReference, ai_ppcLangCodes, ai_oParent);
				}

				if( length_code > 0 )
				{
					i += length_code;
				}
				else
				{
					acabar = M4_TRUE;
				}

				if( !acabar )
				{
					present->propBag->Add(comp);
				}
				else
				{
					if( i < lengthASN )
					{
						return M4_ERROR;
					}
				}
			}
		}
		ai_pIn->readByte(b_T);
	} 
	return M4_SUCCESS;
}


//
// Escribe de manera recursiva una secuencia a xml
//
m4return_t CAsn::codeXml(CInputStream *ai_pInput, m4uint32_t ai_iBytesToRead, ClLongString &ao_roXML, m4pcchar_t ai_pccOrganization, ClChannelManager *ai_poChannelManager)
{

    m4return_t	iResult;
	m4bool_t	bFirstSequence;
	m4bool_t	bModified;
	m4bool_t	bAddAttribute;
	m4uchar_t	bByte;
	m4uint32_t	bBytes;
	m4uint32_t	iLength;
	m4uint32_t	iElementLength;
	m4uint32_t	iAttributeLength;
	m4uint32_t	iInteger;
	m4pchar_t	pcElement;
	m4pchar_t	pcAttribute;
	m4pchar_t	pcValue;
	m4pchar_t	pcCopiedElement;
	m4pchar_t	pcCopiedAttribute;
	m4pchar_t	pcCopiedValue;
	m4pchar_t	pcOut;
	m4char_t	acBuffer[ 128 ];


	// El primer elemento es el nombre
	ai_iBytesToRead -= ai_pInput->readString( pcAttribute, iLength, pcCopiedAttribute, iResult );

//	if( iResult == M4_ERROR || memcmp( pcAttribute, "Classname", iLength ) != 0 )
	if( iResult == M4_ERROR )
	{
		return M4_ERROR;
	}

	if( pcCopiedAttribute != NULL )
	{
		delete pcCopiedAttribute;
		pcCopiedAttribute = NULL;
	}

	ai_iBytesToRead -= ai_pInput->readString( pcElement, iElementLength, pcCopiedElement, iResult );

	if( iResult == M4_ERROR )
	{
		return M4_ERROR;
	}

	ao_roXML.Concat( "<" );
	ao_roXML.Concat( pcElement, iElementLength );

	// El segundo elemento es el alias
	ai_iBytesToRead -= ai_pInput->readString( pcAttribute, iLength, pcCopiedAttribute, iResult );

//	if( iResult == M4_ERROR || memcmp( pcAttribute, "Alias", iLength ) != 0 )
	if( iResult == M4_ERROR )
	{
		return M4_ERROR;
	}

	ao_roXML.Concat( " " );
	ao_roXML.Concat( pcAttribute, iLength );

	if( pcCopiedAttribute != NULL )
	{
		delete pcCopiedAttribute;
		pcCopiedAttribute = NULL;
	}

	ai_iBytesToRead -= ai_pInput->readString( pcValue, iLength, pcCopiedValue, iResult );

	if( iResult == M4_ERROR )
	{
		return M4_ERROR;
	}

	ao_roXML.Concat( "='" );
	ao_roXML.Concat( pcValue, iLength, M4MDRT_CODE_MODE_XML );
	ao_roXML.Concat( "'" );

	if( pcCopiedValue != NULL )
	{
		delete pcCopiedValue;
		pcCopiedValue = NULL;
	}

	bFirstSequence = M4_TRUE;

	while( ai_iBytesToRead > 0 )
	{
		ai_iBytesToRead -= ai_pInput->readByte( bByte );

		if( bByte == ASNSequence )
		{
			if( bFirstSequence == M4_TRUE )
			{
				ao_roXML.Concat( ">" );
				bFirstSequence = M4_FALSE;
			}

			ai_iBytesToRead -= ai_pInput->readLength( bBytes );
			iResult = codeXml( ai_pInput, bBytes, ao_roXML, ai_pccOrganization, ai_poChannelManager );
			ai_iBytesToRead -= bBytes;

			if( iResult != M4_SUCCESS )
			{
				return M4_ERROR;
			}
		}
		else
		{
			// El primer elemento es el nombre
			if( bByte != ASNGeneralString && bByte != ASNUnicodeString )
			{
				return M4_ERROR;
			}

			ai_iBytesToRead -= ai_pInput->readLength( iAttributeLength );
			ai_iBytesToRead -= ai_pInput->readStringReference( pcAttribute, iAttributeLength, bByte, pcCopiedAttribute );

			if( memchr( pcAttribute, '\'', iAttributeLength ) != NULL )
			{
				// Si la linea está comentada se ignora
				bAddAttribute = M4_FALSE;
			}
			else
			{
				bAddAttribute = M4_TRUE;

				ao_roXML.Concat( " " );
				ao_roXML.Concat( pcAttribute, iAttributeLength );
				ao_roXML.Concat( "='" );
			}

			// El segundo elemento es el valor
			ai_iBytesToRead -= ai_pInput->readByte( bByte );

			switch( bByte )
			{
				case ASNBOOLEAN:

					ai_iBytesToRead -= ai_pInput->readByte( bByte );
					ai_iBytesToRead -= ai_pInput->readByte( bByte );

					if( bByte == 0 )
					{
						if( bAddAttribute == M4_TRUE ) ao_roXML.Concat( "FALSE" );
					}
					else
					{
						if( bAddAttribute == M4_TRUE ) ao_roXML.Concat( "TRUE" );
					}
					break;

				case ASNINTEGER:

					ai_iBytesToRead -= ai_pInput->readInteger( iInteger ) ;

					switch( iInteger )
					{
						// Los valores más probables se ponen directamente
						case 0:
							if( bAddAttribute == M4_TRUE ) ao_roXML.Concat( "0" );
							break;
						case 1:
							if( bAddAttribute == M4_TRUE ) ao_roXML.Concat( "1" );
							break;
						case 31:
							if( bAddAttribute == M4_TRUE ) ao_roXML.Concat( "31" );
							break;
						default:
							sprintf( acBuffer, "%d", iInteger );
							if( bAddAttribute == M4_TRUE ) ao_roXML.Concat( acBuffer );
							break;
					}
					break;

				case ASNGeneralString:
				case ASNUnicodeString:

					ai_iBytesToRead -= ai_pInput->readLength( iLength );
					ai_iBytesToRead -= ai_pInput->readStringReference( pcValue, iLength, bByte, pcCopiedValue );

					if( ai_poChannelManager != NULL &&
						( memcmp( pcAttribute, "Text", iAttributeLength ) == 0 ||
						  memcmp( pcAttribute, "Tooltip", iAttributeLength ) == 0 ||
						  memcmp( pcAttribute, "Collabel", iAttributeLength ) == 0 )
						)
					{
						// Si es texto y se puede se remplazan literales
						iResult = M4ReplaceLiterals( ai_poChannelManager, pcValue, iLength, pcOut, ai_pccOrganization, bModified ) ;

						if( iResult == M4_SUCCESS && bModified == M4_TRUE && pcOut != NULL )
						{
							if( pcCopiedValue != NULL )
							{
								delete pcCopiedValue;
							}

							pcCopiedValue = pcOut;
							pcValue = pcOut;
							iLength = strlen( pcValue );
						}
					}

					if( bAddAttribute == M4_TRUE ) ao_roXML.Concat( pcValue, iLength, M4MDRT_CODE_MODE_XML );

					if( pcCopiedValue != NULL )
					{
						delete pcCopiedValue;
						pcCopiedValue = NULL;
					}
					break;

				default:
					return M4_ERROR;
			}

			if( bAddAttribute == M4_TRUE ) ao_roXML.Concat( "'" );

			if( pcCopiedAttribute != NULL )
			{
				delete pcCopiedAttribute;
				pcCopiedAttribute = NULL;
			}
		}
	}

	if( bFirstSequence == M4_TRUE )
	{
		ao_roXML.Concat( "/>" );
	}
	else
	{
		ao_roXML.Concat( "</" );
		ao_roXML.Concat( pcElement, iElementLength );
		ao_roXML.Concat( ">" );
	}

	if( pcCopiedElement != NULL )
	{
		delete pcCopiedElement;
		pcCopiedElement = NULL;
	}

	return M4_SUCCESS;
}


m4return_t CAsn::MergeInheritPresentationRoot( CAsn * & aio_pCAsnParent )
{
	m4return_t iRet = M4_SUCCESS;

	//------------------------------------------------------------------
	//Veamos los casos que no hay nada que mezclar.
	if (aio_pCAsnParent == NULL)
	{
		return iRet;
	}
	if (aio_pCAsnParent->present == NULL)
	{
		return iRet;
	}
	//------------------------------------------------------------------

	//------------------------------------------------------------------
	//Mezclamos arboles.
	if (present != NULL)
	{
		//Debe ser secuencia.
		//Vamos a hacer merge sobreescribiendo la hija con propiedades del padre.		
		iRet = present->MergeInheritPresentationRoot( aio_pCAsnParent->present );
	}
	//------------------------------------------------------------------

	//Hemos podido mezclar, por lo tanto this tiene todo lo necesario.
	delete aio_pCAsnParent;
	aio_pCAsnParent = NULL;
	return iRet;
}

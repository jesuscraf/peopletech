//## begin module%3958CC2701A0.cm preserve=no
//## end module%3958CC2701A0.cm

//## begin module%3958CC2701A0.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//## end module%3958CC2701A0.cp

//## Module: xmlstreamgenerator%3958CC2701A0; Package body
//## Subsystem: M4XMLUtil::src%3906B78B02DB
//## Source file: D:\Work\v600\m4xmlutil\src\xmlstreamgenerator.cpp

//## begin module%3958CC2701A0.additionalIncludes preserve=no
//## end module%3958CC2701A0.additionalIncludes

//## begin module%3958CC2701A0.includes preserve=yes
//## end module%3958CC2701A0.includes

// m4string
#include <m4string.hpp>
// xmlstreamgenerator
#include <xmlstreamgenerator.hpp>
#include "m4unicode.hpp"

//## begin module%3958CC2701A0.declarations preserve=no
//## end module%3958CC2701A0.declarations

//## begin module%3958CC2701A0.additionalDeclarations preserve=yes
#define strFilled	3
//## end module%3958CC2701A0.additionalDeclarations


// Class M4XMLStreamGenerator 





M4XMLStreamGenerator::M4XMLStreamGenerator (m4string_t ai_strQuote)
  //## begin M4XMLStreamGenerator::M4XMLStreamGenerator%962121079.hasinit preserve=no
  //## end M4XMLStreamGenerator::M4XMLStreamGenerator%962121079.hasinit
  //## begin M4XMLStreamGenerator::M4XMLStreamGenerator%962121079.initialization preserve=yes
  : m_oStream(m_strBuffer)
  //## end M4XMLStreamGenerator::M4XMLStreamGenerator%962121079.initialization
{
  //## begin M4XMLStreamGenerator::M4XMLStreamGenerator%962121079.body preserve=yes

	m_strQuote = ai_strQuote;

  //## end M4XMLStreamGenerator::M4XMLStreamGenerator%962121079.body
}

M4XMLStreamGenerator::M4XMLStreamGenerator (ostream& ai_oStream, m4string_t ai_strQuote)
  //## begin M4XMLStreamGenerator::M4XMLStreamGenerator%1083076556.hasinit preserve=no
  //## end M4XMLStreamGenerator::M4XMLStreamGenerator%1083076556.hasinit
  //## begin M4XMLStreamGenerator::M4XMLStreamGenerator%1083076556.initialization preserve=yes
  : m_oStream(ai_oStream)
  //## end M4XMLStreamGenerator::M4XMLStreamGenerator%1083076556.initialization
{
  //## begin M4XMLStreamGenerator::M4XMLStreamGenerator%1083076556.body preserve=yes

	m_strQuote = ai_strQuote;

  //## end M4XMLStreamGenerator::M4XMLStreamGenerator%1083076556.body
}


M4XMLStreamGenerator::~M4XMLStreamGenerator ()
{
  //## begin M4XMLStreamGenerator::~M4XMLStreamGenerator%962121080.body preserve=yes
  //## end M4XMLStreamGenerator::~M4XMLStreamGenerator%962121080.body
}


m4return_t	M4XMLStreamGenerator::_PutString( m4pcchar_t ai_pccString )
{
	m_oStream << ai_pccString ;
	return( M4_SUCCESS ) ;
}


m4return_t	M4XMLStreamGenerator::_PutString( m4pcchar_t ai_pccString, size_t ai_iLength )
{
	m_oStream.write( ai_pccString, ai_iLength ) ;
	return( M4_SUCCESS ) ;
}


m4pcchar_t	M4XMLStreamGenerator::_GetHeader( void ) const
{
	// UNICODE XML
	return( M4XMLHeader() ) ;
}


m4return_t	M4XMLStreamGenerator::_AddScapedString( m4pcchar_t ai_pccString, size_t ai_iLength )
{

	m4char_t	c = 0 ;
	size_t		i = 0 ;
	m4pcchar_t	pccScaped = NULL ;


	if( ai_pccString == NULL )
	{
		return( M4_ERROR ) ;
	}

	for( i = 0 ; i < ai_iLength ; i++ )
	{
		c = ai_pccString[ i ] ;

		// UNICODE XML
		pccScaped = M4XMLScapeChar( c ) ;

		if( pccScaped != NULL )
		{
			m_oStream << pccScaped ;
		}
		else
		{
			if( c == 0 )
			{
				c = strFilled ;
			}

			m_oStream << c ;
		}
	}

	return( M4_SUCCESS ) ;
}



//## Other Operations (implementation)

m4return_t	M4XMLStreamGenerator::MakeXMLDeclaration()
{
	m_oStream.clear() ;

	_PutString( _GetHeader() ) ;

	return( M4_SUCCESS ) ;
}



m4return_t M4XMLStreamGenerator::InsertComment (m4string_t ai_strComment)
{
  //## begin M4XMLStreamGenerator::InsertComment%962121082.body preserve=yes
	return( InsertComment( (m4pcchar_t) ai_strComment.c_str() ) ) ;
  //## end M4XMLStreamGenerator::InsertComment%962121082.body
}

m4return_t	M4XMLStreamGenerator::InsertComment( m4pcchar_t ai_pccComment )
{
	if( ai_pccComment == NULL || *ai_pccComment == '\0' )
	{
		return( M4_ERROR ) ;
	}

	_PutString( "<!-- " ) ;
	_PutString( ai_pccComment ) ;
	_PutString( " -->\n" ) ;

	return( M4_SUCCESS ) ;
}


m4return_t M4XMLStreamGenerator::MakeElement (m4string_t ai_strElement)
{
  //## begin M4XMLStreamGenerator::MakeElement%962121083.body preserve=yes
	return( MakeElement( (m4pcchar_t) ai_strElement.c_str() ) ) ;
}

m4return_t	M4XMLStreamGenerator::MakeElement( m4pcchar_t ai_pccElement )
{
	if( ai_pccElement == NULL || *ai_pccElement == '\0' )
	{
		return( M4_ERROR ) ;
	}

	_PutString( "<" ) ;
	_PutString( ai_pccElement ) ;

	return( M4_SUCCESS ) ;
}

m4return_t M4XMLStreamGenerator::AddElementAttribute (m4string_t ai_strAttribute, m4string_t ai_strAttValue)
{
  //## begin M4XMLStreamGenerator::AddElementAttribute%962272941.body preserve=yes
	return( AddElementAttribute( (m4pcchar_t) ai_strAttribute.c_str(), (m4pcchar_t) ai_strAttValue.c_str() ) ) ;
  //## end M4XMLStreamGenerator::AddElementAttribute%962272941.body
}

m4return_t	M4XMLStreamGenerator::AddElementAttribute( m4pcchar_t ai_pccAttribute, m4pcchar_t ai_pccAttValue )
{
	if( ai_pccAttribute == NULL || *ai_pccAttribute == '\0' )
	{
		return( M4_ERROR ) ;
	}

	if( ai_pccAttValue == NULL )
	{
		ai_pccAttValue = "" ;
	}

	_PutString( " " ) ;
	_PutString( ai_pccAttribute ) ;
	_PutString( "=" ) ;
	_PutString( m_strQuote.c_str() ) ;
	_PutString( ai_pccAttValue ) ;
	_PutString( m_strQuote.c_str() ) ;

	return( M4_SUCCESS ) ;
}


void M4XMLStreamGenerator::EndElement ()
{
  //## begin M4XMLStreamGenerator::EndElement%962272944.body preserve=yes

	_PutString( ">\n" ) ;

  //## end M4XMLStreamGenerator::EndElement%962272944.body
}

m4return_t M4XMLStreamGenerator::CloseElement (m4string_t ai_strElement)
{
  //## begin M4XMLStreamGenerator::CloseElement%962121084.body preserve=yes
	return( CloseElement( (m4pcchar_t) ai_strElement.c_str() ) ) ;
  //## end M4XMLStreamGenerator::CloseElement%962121084.body
}

m4return_t	M4XMLStreamGenerator::CloseElement( m4pcchar_t ai_pccElement )
{
	if( ai_pccElement == NULL || *ai_pccElement == '\0' )
	{
		return( M4_ERROR ) ;
	}

	_PutString( "</" ) ;
	_PutString( ai_pccElement ) ;
	_PutString( ">\n" ) ;

	return( M4_SUCCESS ) ;
}

void M4XMLStreamGenerator::CloseSingleClosedElement ()
{
  //## begin M4XMLStreamGenerator::CloseSingleClosedElement%962272943.body preserve=yes

	_PutString( "/>\n" ) ;

  //## end M4XMLStreamGenerator::CloseSingleClosedElement%962272943.body
}

m4return_t M4XMLStreamGenerator::GetXMLStream (m4pchar_t &aio_strBuffer, long ai_lBufferSize)
{
  //## begin M4XMLStreamGenerator::GetXMLStream%962121086.body preserve=yes

	m_oStream << '\0' ;
	aio_strBuffer = m_strBuffer.str() ;

	if ( aio_strBuffer  )
	{
		return M4_SUCCESS;
	}
	else
	{
		return M4_ERROR;
	}

  //## end M4XMLStreamGenerator::GetXMLStream%962121086.body
}

long M4XMLStreamGenerator::GetXMLStreamSize ()
{
  //## begin M4XMLStreamGenerator::GetXMLStreamSize%962121087.body preserve=yes

	if (0 < m_strBuffer.pcount())
	{
		return (m_strBuffer.pcount() + 1);
	}
	else
	{
		return 0;
	}

  //## end M4XMLStreamGenerator::GetXMLStreamSize%962121087.body
}

m4return_t M4XMLStreamGenerator::AddEscapedElementAttribute (m4string_t ai_strAttribute, m4pchar_t ai_strAttValue, m4uint32_t ai_iStrLen)
{
  //## begin M4XMLStreamGenerator::AddEscapedElementAttribute%982171973.body preserve=yes
	return( AddEscapedElementAttribute( (m4pcchar_t) ai_strAttribute.c_str(), (m4pcchar_t) ai_strAttValue, ai_iStrLen ) ) ;
  //## end M4XMLStreamGenerator::AddEscapedElementAttribute%982171973.body
}

m4return_t	M4XMLStreamGenerator::AddEscapedElementAttribute( m4pcchar_t ai_pccAttribute, m4pcchar_t ai_pccAttValue )
{
	if( ai_pccAttValue == NULL )
	{
		ai_pccAttValue = "" ;
	}

	return( AddEscapedElementAttribute( ai_pccAttribute, ai_pccAttValue, strlen( ai_pccAttValue ) ) ) ;
}

m4return_t	M4XMLStreamGenerator::AddEscapedElementAttribute( m4pcchar_t ai_pccAttribute, m4pcchar_t ai_pccAttValue, size_t ai_iLength )
{

	m4return_t	iResult = M4_ERROR ;

	
	if( ai_pccAttribute == NULL || *ai_pccAttribute == '\0' )
	{
		return( M4_ERROR ) ;
	}

	_PutString( " " ) ;
	_PutString( ai_pccAttribute ) ;
	_PutString( "=" ) ;

	iResult = AddEscapedElementValue( ai_pccAttValue, ai_iLength ) ;

	return( iResult ) ;
}

m4return_t M4XMLStreamGenerator::EscapeXMLStream (m4string_t &ai_strStream)
{
  //## begin M4XMLStreamGenerator::EscapeXMLStream%982230715.body preserve=yes
	m4string_t szEscapedStream = "" ;

	// translates
	if (EscapeXMLStream(ai_strStream,szEscapedStream) != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	ai_strStream = szEscapedStream ;

	return M4_SUCCESS ;


  //## end M4XMLStreamGenerator::EscapeXMLStream%982230715.body
}



m4return_t M4XMLStreamGenerator::EscapeXMLStream (m4string_t &ai_strInput, m4string_t &ao_strOutput)
{
  //## begin M4XMLStreamGenerator::EscapeXMLStream%1085739520.body preserve=yes

	m4char_t	c ;
	size_t		i = 0 ;
	size_t		iLength = 0 ;
	m4pcchar_t	pccScaped = NULL ;


	iLength = ai_strInput.size() ;

	// reservamos el doble, porque en general son cadenas pequeñas
	ao_strOutput.reserve( iLength * 2 ) ;

	for( i = 0 ; i < iLength ; i++ )
	{
		c = ai_strInput[ i ] ;

		// UNICODE XML
		pccScaped = M4XMLScapeChar( c ) ;

		if( pccScaped != NULL )
		{
			ao_strOutput += pccScaped ;
		}
		else
		{
			if( c == 0 )
			{
				c = strFilled ;
			}

			ao_strOutput += c ;
		}
	}

	return M4_SUCCESS ;
  //## end M4XMLStreamGenerator::EscapeXMLStream%1085739520.body
}

m4return_t	M4XMLStreamGenerator::AddScapedString( m4pcchar_t ai_pccString )
{
	if( ai_pccString == NULL )
	{
		ai_pccString = "" ;
	}

	return( AddScapedString( ai_pccString, strlen( ai_pccString ) ) ) ;
}

m4return_t	M4XMLStreamGenerator::AddScapedString( m4pcchar_t ai_pccString, size_t ai_iLength )
{
	return( _AddScapedString( ai_pccString, ai_iLength ) ) ;
}


m4return_t	M4XMLStreamGenerator::AppendScapedString( m4pcchar_t ai_pccString, string &ao_strOutput )
{
	if( ai_pccString == NULL )
	{
		ai_pccString = "" ;
	}

	return( AppendScapedString( ai_pccString, strlen( ai_pccString ), ao_strOutput ) ) ;
}

m4return_t	M4XMLStreamGenerator::AppendScapedString( m4pcchar_t ai_pccString, size_t ai_iLength, string &ao_strOutput )
{
	m4char_t	c = 0 ;
	size_t		i = 0 ;
	m4pcchar_t	pccScaped = NULL ;


	if( ai_pccString == NULL )
	{
		return( M4_ERROR ) ;
	}

	for( i = 0 ; i < ai_iLength ; i++ )
	{
		c = ai_pccString[ i ] ;

		// UNICODE XML
		pccScaped = M4XMLScapeChar( c ) ;

		if( pccScaped != NULL )
		{
			ao_strOutput.append( pccScaped ) ;
		}
		else
		{
			if( c == 0 )
			{
				c = strFilled ;
			}

			ao_strOutput.append( &c, 1 ) ;
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t M4XMLStreamGenerator::AddElementValue (m4string_t ai_strElementValue)
{
  //## begin M4XMLStreamGenerator::AddElementValue%1082986243.body preserve=yes
	return( AddElementValue( ai_strElementValue.c_str(), ai_strElementValue.size() ) ) ;
  //## end M4XMLStreamGenerator::AddElementValue%1082986243.body
}

m4return_t M4XMLStreamGenerator::AddElementValue( m4pcchar_t ai_pccElementValue )
{
	if( ai_pccElementValue == NULL )
	{
		ai_pccElementValue = "" ;
	}

	return( AddElementValue( ai_pccElementValue, strlen( ai_pccElementValue ) ) ) ;
}


m4return_t M4XMLStreamGenerator::AddElementValue (m4pcchar_t ai_pccElementValue, size_t ai_iLength)
{
	_PutString( ai_pccElementValue, ai_iLength ) ;
	return M4_SUCCESS;
}


m4return_t M4XMLStreamGenerator::AddEscapedElementValue (m4pchar_t ai_strElementValue, m4uint32_t ai_iStrLen)
{
	return( AddEscapedElementValue( (m4pcchar_t) ai_strElementValue, ai_iStrLen ) ) ;
}


m4return_t	M4XMLStreamGenerator::AddEscapedElementValue( m4pcchar_t ai_pccElementValue )
{
	if( ai_pccElementValue == NULL )
	{
		ai_pccElementValue = "" ;
	}

	return( AddEscapedElementValue( ai_pccElementValue, strlen( ai_pccElementValue ) ) ) ;
}

m4return_t	M4XMLStreamGenerator::AddEscapedElementValue( m4pcchar_t ai_pccElementValue, size_t ai_iLength )
{
	
	m4return_t	iResult = M4_ERROR ;


	_PutString( m_strQuote.c_str() ) ;

	iResult = AddScapedString( ai_pccElementValue, ai_iLength ) ;

	_PutString( m_strQuote.c_str() ) ;

	return( iResult ) ;
}


// Additional Declarations
  //## begin M4XMLStreamGenerator%3958CAC8008D.declarations preserve=yes
  //## end M4XMLStreamGenerator%3958CAC8008D.declarations


// ANSI Stream Generator

M4ANSIXMLStreamGenerator::M4ANSIXMLStreamGenerator (m4string_t ai_strQuote)
  : M4XMLStreamGenerator(ai_strQuote)
{
}


M4ANSIXMLStreamGenerator::M4ANSIXMLStreamGenerator (ostream& ai_oStream, m4string_t ai_strQuote)
  : M4XMLStreamGenerator(ai_oStream, ai_strQuote)
{
}



M4ANSIXMLStreamGenerator::~M4ANSIXMLStreamGenerator (void)
{
}


m4return_t	M4ANSIXMLStreamGenerator::_PutString( m4pcchar_t ai_pccString )
{

	int			iLength = -1 ;
	m4pchar_t	pccValue = NULL ;


	pccValue = M4CppToANSI( ai_pccString, iLength ) ;
	m_oStream << pccValue ;
	delete [] pccValue ;

	return( M4_SUCCESS ) ;
}


m4return_t	M4ANSIXMLStreamGenerator::_PutString( m4pcchar_t ai_pccString, size_t ai_iLength )
{

	int			iLength = -1 ;
	m4pchar_t	pccValue = NULL ;


	iLength = ai_iLength ;
	pccValue = M4CppToANSI( ai_pccString, iLength ) ;
	m_oStream.write( pccValue, iLength ) ;
	delete [] pccValue ;

	return( M4_SUCCESS ) ;
}


m4pcchar_t	M4ANSIXMLStreamGenerator::_GetHeader( void ) const
{
	return( M4ANSIHeader() ) ;
}


m4return_t	M4ANSIXMLStreamGenerator::AddScapedString( m4pcchar_t ai_pccString, size_t ai_iLength )
{

	m4return_t	iResult = M4_ERROR ;
	int			iLength = -1 ;
	m4pchar_t	pccValue = NULL ;


	iLength = ai_iLength ;
	pccValue = M4CppToANSI( ai_pccString, iLength ) ;

	iResult = _AddScapedString( pccValue, iLength ) ;
	delete [] pccValue ;

	return( iResult ) ;
}


// Utf8 Stream Generator

M4Utf8XMLStreamGenerator::M4Utf8XMLStreamGenerator (m4string_t ai_strQuote)
  : M4XMLStreamGenerator(ai_strQuote)
{
}


M4Utf8XMLStreamGenerator::M4Utf8XMLStreamGenerator (ostream& ai_oStream, m4string_t ai_strQuote)
  : M4XMLStreamGenerator(ai_oStream, ai_strQuote)
{
}



M4Utf8XMLStreamGenerator::~M4Utf8XMLStreamGenerator (void)
{
}


m4return_t	M4Utf8XMLStreamGenerator::_PutString( m4pcchar_t ai_pccString )
{

	int			iLength = -1 ;
	m4pchar_t	pccValue = NULL ;


	pccValue = M4CppToUtf8( ai_pccString, iLength ) ;
	m_oStream << pccValue ;
	delete [] pccValue ;

	return( M4_SUCCESS ) ;
}


m4return_t	M4Utf8XMLStreamGenerator::_PutString( m4pcchar_t ai_pccString, size_t ai_iLength )
{

	int			iLength = -1 ;
	m4pchar_t	pccValue = NULL ;


	iLength = ai_iLength ;
	pccValue = M4CppToUtf8( ai_pccString, iLength ) ;
	m_oStream.write( pccValue, iLength ) ;
	delete [] pccValue ;

	return( M4_SUCCESS ) ;
}


m4pcchar_t	M4Utf8XMLStreamGenerator::_GetHeader( void ) const
{
	return( M4Utf8Header() ) ;
}


m4return_t	M4Utf8XMLStreamGenerator::AddScapedString( m4pcchar_t ai_pccString, size_t ai_iLength )
{

	m4return_t	iResult = M4_ERROR ;
	int			iLength = -1 ;
	m4pchar_t	pccValue = NULL ;


	iLength = ai_iLength ;
	pccValue = M4CppToUtf8( ai_pccString, iLength ) ;

	iResult = _AddScapedString( pccValue, iLength ) ;
	delete [] pccValue ;

	return( iResult ) ;
}





//## begin module%3958CC2701A0.epilog preserve=yes
//## end module%3958CC2701A0.epilog

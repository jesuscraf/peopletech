//==============================================================================
//
// (c) Copyright  1991-2016 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4iputils.dll
// File:             m4ipaddress.cpp
// Project:          IP Address utils
// Author:           Meta Software M.S. , S.A
// Date:             24/08/16
// Language:         C++
// Operating System: WINDOWS, UNIX
//==============================================================================

#include "m4iputils.hpp"
#include "m4log.hpp"
#include "gmp.h"


// ---------------------------------------------------------
// Protected constructor.
//
//	ai_sIP:		IPv6 string with zeroes expanded.
//	ai_bFromV4:	Original IP version.
// ---------------------------------------------------------
ClIPAddress::ClIPAddress( const m4string_t& ai_sIP, m4bool_t ai_bFromV4  )
{
	m_sIP = ai_sIP;
	m_bFromV4 = ai_bFromV4;

	init();
}


// ---------------------------------------------------------
// Destructor.
// ---------------------------------------------------------
ClIPAddress::~ClIPAddress()
{
}


// ---------------------------------------------------------
// Initializes the object.
//
// String IP must be in expanded v6 format.
// ---------------------------------------------------------
void ClIPAddress::init()
{
	size_t		iStrPos = 0, iBinPos = 0;
	m4string_t	sByte;
	m4uint_t	iValue;


	memset( m_acIP, 0, sizeof( m_acIP ) );

	for( size_t iBlock = 0; iBlock < 8; iBlock++ )
	{
		for( size_t iByte = 0; iByte < 2; iByte++ )
		{
			iStrPos += 2 * iByte;

			sByte = m_sIP.substr( iStrPos, 2 );
			sscanf( sByte.c_str(), "%2hhx", &iValue );

			m_acIP[ iBinPos++ ] = iValue;
		}
		
		iStrPos += 3;
	}
}


// ---------------------------------------------------------
// Gets a new class instance.
// [static]
//
//	ai_sIP:		IPv4/v6 string. 
//
//	Return:
//
//		NULL:		Invalid IP string.
//		Pointer:	New instance reference. The caller is 
//					responsible of deleting it.
// ---------------------------------------------------------
ClIPAddress* ClIPAddress::GetInstance( const m4string_t& ai_sIP )
{
	ClIPAddress*	pInstance = NULL;
	size_t			iPos;
	m4string_t		sIP;
	m4return_t		iRet;
	m4bool_t		bFromV4;

	
	if( ai_sIP.empty() == true )
	{
		return NULL;
	}

	iPos = ai_sIP.find( '/' );

	if( iPos != M4_STR_NPOS )
	{
		return NULL;
	}

	if( isV4( ai_sIP ) == true )
	{
		bFromV4 = M4_TRUE;

		iRet = toV6( ai_sIP, sIP );

		if( iRet == M4_ERROR )
		{
		 return NULL;
		}
	}
	else
	{
		if( isV6( ai_sIP ) == false )
		{
			return NULL;
		}

		bFromV4 = M4_FALSE;

		iRet = expandV6( ai_sIP, sIP );

		if( iRet == M4_ERROR )
		{
			return NULL;
		}
	}
	
	pInstance = new ClIPAddress( sIP, bFromV4 );


	return pInstance;
}


// ---------------------------------------------------------
// Gets a 16-byte array with the address binary 
// representation.
// ---------------------------------------------------------
m4uint8_t* ClIPAddress::GetBinary()
{
	return m_acIP;
}


// ---------------------------------------------------------
// Gets if the original IP was V4.
// ---------------------------------------------------------
m4bool_t ClIPAddress::GetFromV4()
{
	return m_bFromV4;
}


// ---------------------------------------------------------
// Gets the decimal representation of the IP.
// ---------------------------------------------------------
m4pchar_t ClIPAddress::GetDecimal()
{
	m4uint16_t*		piIPv6, iBlock;
	mpz_t			mpzTotal, mpzFactor, mpzBlock, mpzConst_2;
	size_t			iSize;
	char*			pcTotal = NULL;


	mpz_init( mpzTotal );
	mpz_init( mpzFactor );
	mpz_init( mpzBlock );
	mpz_init( mpzConst_2 );

	mpz_set_ui( mpzTotal, 0 );
	mpz_set_ui( mpzFactor, 1 );
	mpz_set_ui( mpzConst_2, 2 );
	
	if( m_bFromV4 == M4_FALSE )
	{
		piIPv6 = ( m4uint16_t* )m_acIP;

		for( int i = 0; i < 8; i++ )
		{
#ifdef _WINDOWS
			*( ( m4uint8_t* )( &iBlock ) ) = *( ( ( m4uint8_t* )( &piIPv6[ i ] ) ) + 1 );
			*( ( ( m4uint8_t* )( &iBlock ) ) + 1 ) = *( ( m4uint8_t* )( &piIPv6[ i ] ) );
#else
			iBlock = piIPv6[ i ];
#endif
			mpz_pow_ui( mpzFactor, mpzConst_2, ( 7 - i ) * 16 );
			mpz_mul_ui( mpzBlock, mpzFactor, iBlock );
			mpz_add( mpzTotal, mpzTotal, mpzBlock );
		}
	}
	else
	{
		// Since we use IPv6 DB the IPv4 are in IPv4-mapped IPv6 address form.
		for( int i = 12; i < 16; i++ )
		{
			mpz_pow_ui( mpzFactor, mpzConst_2, ( 15 - i ) * 8 );
			mpz_mul_ui( mpzBlock, mpzFactor, m_acIP[ i ] );
			mpz_add( mpzTotal, mpzTotal, mpzBlock );
		}

		mpz_pow_ui( mpzFactor, mpzConst_2, 32 );
		mpz_mul_ui( mpzBlock, mpzFactor, 0xFFFF );
		mpz_add( mpzTotal, mpzTotal, mpzBlock );
	}

	iSize = mpz_sizeinbase( mpzTotal, 10 ) + 2;
	
	pcTotal = new char[ iSize ];
	pcTotal = mpz_get_str( pcTotal, 10, mpzTotal );
	
	mpz_clear( mpzTotal );
	mpz_clear( mpzFactor );
	mpz_clear( mpzBlock );
	mpz_clear( mpzConst_2 );

	return pcTotal;
}


// ---------------------------------------------------------
// Checks if a string matches to IPv4 format.
// [static]
// 
//	ai_sIP:		Input IP string
//
//	Return:
//				true / false
//
//	The string must contain exactly 3 dots.
// ---------------------------------------------------------
bool ClIPAddress::isV4( const m4string_t& ai_sIP )
{
	size_t		iPrevPos = 0, iPos;
	int			iDots = 0;
	m4string_t	sBlock;


	while( ( iPos = ai_sIP.find( '.', iPrevPos ) ) != M4_STR_NPOS )
	{
		iDots++;

		if( iDots > 3 )
		{
			return false;
		}

		sBlock = ai_sIP.substr( iPrevPos, iPos - iPrevPos );

		if( sBlock.find_first_not_of( "0123456789", 0 ) != M4_STR_NPOS )
		{
			return false;
		}
		
		if( atoi( sBlock.c_str() ) > 255 )
		{
			return false;
		}

		iPrevPos = iPos + 1;
	}

	if( iDots != 3 )
	{
		return false;
	}

	sBlock = ai_sIP.substr( iPrevPos );

	if( sBlock.find_first_not_of( "0123456789", 0 ) != M4_STR_NPOS )
	{
		return false;
	}
	
	if( atoi( sBlock.c_str() ) > 255 )
	{
		return false;
	}


	return true;
}


// ---------------------------------------------------------
// Checks if a string matches to IPv6 format.
// [static]
// 
//	ai_sIP:		Input IP string
//
//	Return:
//				true / false
//
//	The string must contain at least 1 colon and
//  at most 8 (case of single ommited zeroes block).
// ---------------------------------------------------------
bool ClIPAddress::isV6( const m4string_t& ai_sIP )
{
	size_t		iPrevPos = 0, iPos;
	int			iColons = 0;
	m4string_t	sBlock;


	while( ( iPos = ai_sIP.find( ':', iPrevPos ) ) != M4_STR_NPOS )
	{
		iColons++;

		if( iColons > 8 )
		{
			return false;
		}

		sBlock = ai_sIP.substr( iPrevPos, iPos - iPrevPos );

		if( sBlock.find_first_not_of( "0123456789ABCDEFabcdef", 0 ) != M4_STR_NPOS )
		{
			return false;
		}
		
		iPrevPos = iPos + 1;
	}

	if( iColons == 0 )
	{
		return false;
	}

	sBlock = ai_sIP.substr( iPrevPos );

	if( sBlock.find_first_not_of( "0123456789ABCDEFabcdef", 0 ) != M4_STR_NPOS )
	{
		return false;
	}


	return true;
}


// ---------------------------------------------------------
// Converts an IPv4 string to IPv6.
// [static]
//
//	ai_sIP:		Input IP string.
//
//	Return:
//				IPv6 string.
// ---------------------------------------------------------
m4return_t ClIPAddress::toV6( const m4string_t& ai_sIPv4, m4string_t& ao_sIPv6 )
{
	size_t			iPrevPos = 0, iPos;
	m4string_t		sBlock;
	int				iByte;
	unsigned char	acBytes[ 4 ];
	char			acIP[ IP_V6_STR_MAX_LEN ];


	iByte = 0;

	while( ( iPos = ai_sIPv4.find( '.', iPrevPos ) ) != M4_STR_NPOS )
	{
		sBlock = ai_sIPv4.substr( iPrevPos, iPos - iPrevPos );
	
		acBytes[ iByte++ ] = atoi( sBlock.c_str() );

		iPrevPos = iPos + 1;
	}

	sBlock = ai_sIPv4.substr( iPrevPos );

	acBytes[ iByte ] = atoi( sBlock.c_str() );

	sprintf( acIP, "0000:0000:0000:0000:0000:ffff:%02x%02x:%02x%02x", acBytes[ 0 ], acBytes[ 1 ], acBytes[ 2 ], acBytes[ 3 ] );

	ao_sIPv6 = acIP;


	return M4_SUCCESS;
}


// ---------------------------------------------------------
// Expands an IPv6 string.
// [static]
//
//	ai_sIPv6:	Input IPv6 string.
//	ao_sIPv6:	Output expanded IPv6 string
//
//	Return:
//				M4_ERROR / M4_SUCCESS
// ---------------------------------------------------------
m4return_t ClIPAddress::expandV6( const m4string_t& ai_sIPv6, m4string_t& ao_sIPv6 )
{
	size_t		iPos = -1;
	int			iColons = 0;
	size_t		iOmmittedBlocks;
	m4string_t	sExpandedBlocks = "";
	m4string_t	sPattern;
	m4string_t	sIP;
	m4return_t	iRet;


	while( ( iPos = ai_sIPv6.find( ':', iPos + 1 ) ) != M4_STR_NPOS )
	{
		iColons++;
	}


	// Expands the blocks of zeroes ommited (::).
	iPos = ai_sIPv6.find( "::" );

	if( iPos != M4_STR_NPOS )
	{
		iOmmittedBlocks = ( 7 - iColons ) + 1;

		if( iPos == 0 )
		{
			iOmmittedBlocks++;
			sPattern = "0000:";

			if( iPos == ai_sIPv6.length() - 2 )
			{
				sExpandedBlocks = "0000";
				sPattern = ":0000";
			}
		}
		else if( iPos == ai_sIPv6.length() - 2 )
		{
			iOmmittedBlocks++;
			sPattern = ":0000";
		}
		else
		{
			sExpandedBlocks = ":";
			sPattern = "0000:";
		}

		for( int i = 0; i < iOmmittedBlocks; i++ )
		{
			sExpandedBlocks += sPattern;
		}

		sIP = ai_sIPv6.substr( 0, iPos );
		sIP += sExpandedBlocks;
		sIP += ai_sIPv6.substr( iPos + 2 );
	}
	else
	{
		if( iColons < 7 )
		{
			return M4_ERROR;
		}

		sIP = ai_sIPv6;
	}

	
	// Remove zone index.
	iPos = ai_sIPv6.find( "%" );

	if( iPos != M4_STR_NPOS )
	{
		sIP = sIP.substr( 0, iPos );
	}
	
	// Inserts the ommitted zeroes on each block.
	iRet = insertZeroes( sIP, ao_sIPv6);

	
	return iRet;
}


// ---------------------------------------------------------
// Inserts the ommitted zeroes on each block of an IPv6 string.
// [static]
//
//	ai_sIPv6:	Input IPv6 string.
//	ao_sIPv6:	Output expanded IPv6 string
//
//	Return:
//				M4_ERROR / M4_SUCCESS
// ---------------------------------------------------------
m4return_t ClIPAddress::insertZeroes( const m4string_t& ai_sIPv6, m4string_t& ao_sIPv6 )
{
	size_t		iPos;
	int			iWidth;


	if( ai_sIPv6.length() == 0 )
	{
		return M4_SUCCESS;
	}

	iPos = ai_sIPv6.find( ':', 0 );

	if( iPos == M4_STR_NPOS )
	{
		iWidth = 4 - ai_sIPv6.length();

		if( iWidth < 0 )
		{
			return M4_ERROR;
		}

		for( int i = 0; i < iWidth ; i++ )
		{
			ao_sIPv6 += "0";
		}
		
		ao_sIPv6 += ai_sIPv6;
	}
	else
	{
		iWidth = 4 - iPos;

		if( iWidth < 0 )
		{
			return M4_ERROR;
		}

		for( int i = 0; i < iWidth; i++ )
		{
			ao_sIPv6 += "0";
		}
		
		ao_sIPv6 += ai_sIPv6.substr( 0, iPos + 1 );


		return( insertZeroes( ai_sIPv6.substr( iPos + 1 ), ao_sIPv6 ) );
	}


	return M4_SUCCESS;
}

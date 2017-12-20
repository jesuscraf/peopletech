//==============================================================================
//
// (c) Copyright  1991-2016 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4iputils.dll
// File:             m4iprange.cpp
// Project:          IP Address utils
// Author:           Meta Software M.S. , S.A
// Date:             24/08/16
// Language:         C++
// Operating System: WINDOWS, UNIX
//==============================================================================

#include "m4iputils.hpp"
#include "m4log.hpp"


// ---------------------------------------------------------
// Protected constructor.
//
//	ClIPAddress:	ClIPAddress object.
//	ai_iNetBits:	Number of net bits.
// ---------------------------------------------------------
ClIPRange::ClIPRange( ClIPAddress* ai_pAddress, int ai_iNetBits )
{
	m_pAddress = ai_pAddress;
	m_iNetBits = ai_iNetBits;
}


// ---------------------------------------------------------
// Destructor.
// ---------------------------------------------------------
ClIPRange::~ClIPRange()
{
	if( m_pAddress != NULL )
	{
		delete m_pAddress;
	}
}


// ---------------------------------------------------------
// Gets a new class instance.
// [static]
//
//	ai_sRange:		IPv4/v6 string (CIDR notation). 
//
//	Return:
//
//		NULL:		Invalid IP range string.
//		Pointer:	New instance reference. The caller is 
//					responsible of deleting it.
// ---------------------------------------------------------
ClIPRange* ClIPRange::GetInstance( const m4string_t& ai_sRange )
{
	ClIPRange*		pInstance = NULL;
	size_t			iPos;
	int				iNetBits = 128;
	ClIPAddress*	pAddress;

	
	if( ai_sRange.empty() == true )
	{
		return NULL;
	}
	
	iPos = ai_sRange.find( '/' );

	pAddress = ClIPAddress::GetInstance( ai_sRange.substr( 0, iPos ) );

	if( pAddress == NULL )
	{
		return NULL;
	}

	if( iPos != M4_STR_NPOS )
	{
		iNetBits = atoi( ai_sRange.substr( iPos + 1 ).c_str() );

		if( ClIPAddress::isV4( ai_sRange.substr( 0, iPos ) ) == true )
		{
			if( iNetBits < 0 || iNetBits > 32 )
			{
				iNetBits = 32;
			}

			iNetBits += 96;
		}
		else
		{
			if( iNetBits < 0 || iNetBits > 128 )
			{
				iNetBits = 128;
			}
		}
	}

	pInstance = new ClIPRange( pAddress, iNetBits );


	return pInstance;
}


// ---------------------------------------------------------
// Checks if a given address belongs to the range
//
//	ai_pAddress:	ClIPAddress object to check.
//	ao_bMatches:	Output boolean.
//
//	Return:
//				M4_ERROR / M4_SUCCESS
// ---------------------------------------------------------
m4return_t ClIPRange::IsAddressInRange( ClIPAddress* ai_pAddress, bool& ao_bMatches )
{
	unsigned int	iBytes, iBits, iCmp;
	m4uint8_t		*pBinIP, *pRangeBinIP;


	if( ai_pAddress == NULL )
	{
		return M4_ERROR;
	}

	if( m_iNetBits == 0 )
	{
		ao_bMatches = true;
		return M4_SUCCESS;
	}

	pBinIP		= ai_pAddress->GetBinary();
	pRangeBinIP	= m_pAddress->GetBinary();

	iBytes	= m_iNetBits / 8;
	iBits	= m_iNetBits % 8;
	
	if( iBytes > 0 )
	{
		iCmp = memcmp( pRangeBinIP, pBinIP , iBytes );

		if( iCmp != 0 )
		{
			ao_bMatches = false;
			return M4_SUCCESS;
		}
	}

	if( iBits > 0 )
	{
		ao_bMatches = bitsCmp( pRangeBinIP[ iBytes ], pBinIP[ iBytes], iBits );
	}
	else
	{
		ao_bMatches = true;
	}


	return M4_SUCCESS;
}


// ---------------------------------------------------------
// Gets the limits of a range
//
//	ao_sStart:	Output. Start of range.
//	ao_sEnd:	Output.  End of range.
//
//	Return:
//				M4_ERROR / M4_SUCCESS
// ---------------------------------------------------------
m4return_t ClIPRange::GetLimits( m4string_t& ao_sStart, m4string_t& ao_sEnd )
{
	m4uint8_t*	pBinIP; 
	m4uint8_t	acBytes[ 16 ];
	int			iFixedBytes;
	char		acTextIP[ IP_V6_STR_MAX_LEN ];


	ao_sStart = "";
	ao_sEnd = "";

	pBinIP = m_pAddress->GetBinary();

	memcpy( acBytes, pBinIP, sizeof( acBytes ) );

	iFixedBytes = m_iNetBits / 8;

	// Range start IP
	if( iFixedBytes < 16 )
	{
		acBytes[ iFixedBytes ] &= ( 0xFF & ( 0xFF << ( 8 - ( m_iNetBits % 8 ) ) ) );

		for( int i = iFixedBytes + 1; i < 16; i++ )
		{
			acBytes[ i ] = 0;
		}
	}

	if( m_pAddress->GetFromV4() == M4_TRUE )
	{
		sprintf( acTextIP, "%d.%d.%d.%d", acBytes[ 12 ], acBytes[ 13 ], acBytes[ 14 ], acBytes[ 15 ] );
	}
	else
	{
		sprintf( acTextIP, "%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X", acBytes[ 0 ], acBytes[ 1 ], acBytes[ 2 ], acBytes[ 3 ], acBytes[ 4 ], acBytes[ 5 ], acBytes[ 6 ], acBytes[ 7 ], acBytes[ 8 ], acBytes[ 9 ], acBytes[ 10 ], acBytes[ 11 ], acBytes[ 12 ], acBytes[ 13 ], acBytes[ 14 ], acBytes[ 15 ] );
	}

	ao_sStart = acTextIP;

	// Range end IP
	if( iFixedBytes < 16 )
	{
		acBytes[ iFixedBytes ] |= ( 0xFF >> ( m_iNetBits % 8 ) );

		for( int i = iFixedBytes + 1; i < 16; i++ )
		{
			acBytes[ i ] = 0xFF;
		}
	}

	if( m_pAddress->GetFromV4() == M4_TRUE )
	{
		sprintf( acTextIP, "%d.%d.%d.%d", acBytes[ 12 ], acBytes[ 13 ], acBytes[ 14 ], acBytes[ 15 ] );
	}
	else
	{
		sprintf( acTextIP, "%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X", acBytes[ 0 ], acBytes[ 1 ], acBytes[ 2 ], acBytes[ 3 ], acBytes[ 4 ], acBytes[ 5 ], acBytes[ 6 ], acBytes[ 7 ], acBytes[ 8 ], acBytes[ 9 ], acBytes[ 10 ], acBytes[ 11 ], acBytes[ 12 ], acBytes[ 13 ], acBytes[ 14 ], acBytes[ 15 ] );
	}

	ao_sEnd = acTextIP;


	return M4_SUCCESS;
}


// ---------------------------------------------------------
// Compares the n more significant bits of two bytes.
//
//	ai_iOp1:		Operand 1.
//	ai_iOp2:		Operand 2.
//	ai_iBits:		Number of bits to compare.
//
//	Return:
//					true / false
// ---------------------------------------------------------
bool ClIPRange::bitsCmp( m4uint8_t ai_iOp1, m4uint8_t ai_iOp2, unsigned int ai_iBits )
{
	for( int i = 7; i > ( 7 - ai_iBits ); i-- )
	{
		if( ( ( ai_iOp1 & ( 1 << i) ) >> i ) ^ ( ( ai_iOp2 & ( 1 << i) ) >> i ) )	// Shifts to right only for clarity.
		{
			return false;
		}
	}
	
	
	return true;
}

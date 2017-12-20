//==============================================================================
//
// (c) Copyright  1991-2016 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4iputils.dll
// File:             m4iputils.hpp
// Project:          IP Address utils
// Author:           Meta Software M.S. , S.A
// Date:             24/08/16
// Language:         C++
// Operating System: WINDOWS, UNIX
//==============================================================================

#ifndef m4iputils_h
#define m4iputils_h 1

#include "m4iputils_dll.hpp"
#include "m4types.hpp"
#include "m4var.hpp"
#include "m4string.hpp"


extern "C" M4_DECL_M4IPUTILS m4return_t CheckAddressInRange	( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
extern "C" M4_DECL_M4IPUTILS m4return_t CheckAddressFormat	( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
extern "C" M4_DECL_M4IPUTILS m4return_t CheckRangeFormat	( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
extern "C" M4_DECL_M4IPUTILS m4return_t GetRangeLimits		( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
extern "C" M4_DECL_M4IPUTILS m4return_t GetIPDecString		( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );

// 8*4+7+1
#define IP_V6_STR_MAX_LEN	40

// ---------------------------------------------------------
// class ClIPAddress
//
// Stores a IPv6 representation of an address.
// ---------------------------------------------------------
class M4_DECL_M4IPUTILS ClIPAddress
{
	public:

		~ClIPAddress();

		// Gets a new class instance.
		static ClIPAddress*		GetInstance( const m4string_t& ai_sIp );

		// Gets a 16-byte array with the address binary representation.
		m4uint8_t*				GetBinary();

		// Gets if the original IP was V4.
		m4bool_t				GetFromV4();

		// Gets the decimal representation of the IP.
		m4pchar_t				GetDecimal();

	protected:

		m4string_t	m_sIP;
		m4uint8_t	m_acIP[ 16 ];
		bool		m_bFromV4;

		ClIPAddress	( const m4string_t& ai_sIp, m4bool_t ai_bFromV4 );
		void init	();


		static bool				isV4		( const m4string_t& ai_sIP );
		static bool				isV6		( const m4string_t& ai_sIP );
		static m4return_t		toV6		( const m4string_t& ai_sIPv4, m4string_t& ao_sIPv6 );
		static m4return_t		expandV6	( const m4string_t& ai_sIPv6, m4string_t& ao_sIPv6 );
		static m4return_t		insertZeroes( const m4string_t& ai_sIPv6, m4string_t& ao_sIPv6 );

	friend class ClIPRange; // accesses to isV4 method.
};


// ---------------------------------------------------------
// class ClIPRange
//
// Stores a IPv6 representation of a range of addresses.
// ---------------------------------------------------------
class M4_DECL_M4IPUTILS ClIPRange
{
	public:

		~ClIPRange();

		// Gets a new class instance.
		static ClIPRange*	GetInstance	( const m4string_t& ai_sRange );

		// Checks if a given address belongs to the range
		m4return_t			IsAddressInRange( ClIPAddress* ai_pAddress, bool& ao_bMatches );

		// Gets the starting and ending IP addresses of a range.
		m4return_t			GetLimits( m4string_t& ao_sStart, m4string_t& ao_sEnd );

	protected:

		ClIPAddress*	m_pAddress;
		int				m_iNetBits;

		ClIPRange	( ClIPAddress* ai_pAddress, int ai_iNetBits );

		bool bitsCmp( m4uint8_t ai_iOp1, m4uint8_t ai_iOp2, unsigned int ai_iBits );
};

#endif

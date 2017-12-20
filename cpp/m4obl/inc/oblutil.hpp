
#ifndef _OBLUTIL_HPP_
#define _OBLUTIL_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <m4types.hpp>
#include <m4obl_dll.hpp>


class M4_DECL_M4OBL ClOblUtil
{

public:

	ClOblUtil(m4pchar_t ai_szOblPath ) ;

	~ClOblUtil() ;

	m4bool_t Error( ) ;

	m4return_t PersistOblProp ( m4pchar_t ai_szProperty , m4pchar_t ai_szValue ) ;

	m4return_t GetProperty ( m4pchar_t ai_szProperty , m4pchar_t ao_szValue , m4uint32_t ai_szBuffLen ) ;

	m4return_t GetProperty ( m4pchar_t ai_szBolckName , m4pchar_t ai_szProperty , m4pchar_t ao_szValue , m4uint32_t ai_szBuffLen ) ;

private :

	m4pchar_t m_szOblPath ;

	FILE * m_oblFile ;

	m4bool_t m_err ;

	fpos_t m_pos ;
};



#endif


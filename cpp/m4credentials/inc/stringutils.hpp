//## begin module%396C9B9B0269.cm preserve=no
//## end module%396C9B9B0269.cm

//## begin module%396C9B9B0269.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.hpp
//	 Project:            M4ComSrv
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
//	=
//## end module%396C9B9B0269.cp

//## Module: StringUtils%396C9B9B0269; Subprogram specification
//## Subsystem: M4Credentials::inc%3959D0BB013A
//## Source file: D:\eduardoma\m4credentials\inc\stringutils.hpp

#ifndef stringutils_h
#define stringutils_h 1

//## begin module%396C9B9B0269.additionalIncludes preserve=no
//## end module%396C9B9B0269.additionalIncludes

//## begin module%396C9B9B0269.includes preserve=yes
#include "m4credentials_dll.hpp"
//## end module%396C9B9B0269.includes

// m4string
#include <m4string.hpp>
//## begin module%396C9B9B0269.declarations preserve=no
//## end module%396C9B9B0269.declarations

//## begin module%396C9B9B0269.additionalDeclarations preserve=yes
//## end module%396C9B9B0269.additionalDeclarations


//## Class: TStringUtils%396D9BCF0163; Class Utility
//## Category: M4Credentials%3959C61A006F
//## Subsystem: M4Credentials::inc%3959D0BB013A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%396D9EE800AF;m4string_t { -> }
//## Uses: <unnamed>%396DDAF1016C;ostrstream { -> F}

class M4_DECL_M4CREDENTIALS TStringUtils
{
  public:

    //## Other Operations (specified)
      //## Operation: GetToken%963485957
      static m4return_t GetToken (const m4string_t &ai_strRawString, m4string_t::size_type &aio_iStartPos, const m4string_t::size_type &ai_iEndPos, m4char_t ai_cSeparator, m4string_t &ao_strToken);

      //## Operation: Cipher%963485958
      static m4return_t Cipher (const m4string_t &ai_strRawString, m4string_t &ao_strCipheredString, const m4string_t &ai_strKey, m4uint_t ai_iPasses = 1);

      //## Operation: Decipher%963485959
      static m4return_t Decipher (const m4string_t &ai_strRawString, m4string_t &ao_strDecipheredString, const m4string_t &ai_strKey, m4uint_t ai_iPasses = 1);

      //## Operation: GetDigest%963485960
      static m4return_t GetDigest (const m4string_t &ai_strRawString, m4string_t &ao_strDigest);

      //## Operation: CheckDigest%963485961
      static m4return_t CheckDigest (const m4string_t &ai_strRawString, const m4string_t &ai_strDigest);

  protected:
  private:
  private: //## implementation
};

//## begin module%396C9B9B0269.epilog preserve=yes
//## end module%396C9B9B0269.epilog


#endif

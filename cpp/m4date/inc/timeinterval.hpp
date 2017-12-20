//## begin module%383E4DE20075.cm preserve=no
//## end module%383E4DE20075.cm

//## begin module%383E4DE20075.cp preserve=no
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
//## end module%383E4DE20075.cp

//## Module: timeinterval%383E4DE20075; Package specification
//## Subsystem: M4Date::inc%3779DEF502DA
//	d:\compon\m4date\inc
//## Source file: D:\FuentesServidor\m4date\inc\timeinterval.hpp

#ifndef timeinterval_h
#define timeinterval_h 1

//## begin module%383E4DE20075.additionalIncludes preserve=no
//## end module%383E4DE20075.additionalIncludes

//## begin module%383E4DE20075.includes preserve=yes
#include <m4date_dll.hpp>
#include <m4string.hpp>
//## end module%383E4DE20075.includes

// m4types
#include <m4types.hpp>
//## begin module%383E4DE20075.declarations preserve=no
//## end module%383E4DE20075.declarations

//## begin module%383E4DE20075.additionalDeclarations preserve=yes
//## end module%383E4DE20075.additionalDeclarations


//## begin ClTimeInterval%383D4CE701B6.preface preserve=yes
//## end ClTimeInterval%383D4CE701B6.preface

//## Class: ClTimeInterval%383D4CE701B6
//	Clase que almacena tiempo en segundos y permite recuperarlo e introducierlo
//	con el formato cadena:
//	 nd nh nmin nsec
//	es decir, devuelve el número de dias, hora, minutos y segundos que hay en
//	los segundos almancenados.
//## Category: M4Date%3779DEF60096
//## Subsystem: M4Date::inc%3779DEF502DA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4DATE ClTimeInterval 
{
  //## begin ClTimeInterval%383D4CE701B6.initialDeclarations preserve=yes
  //## end ClTimeInterval%383D4CE701B6.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClTimeInterval%943607443; C++
      ClTimeInterval (const m4uint32_t ai_uiSeconds);

      //## Operation: ClTimeInterval%943607444; C++
      ClTimeInterval (const M4ClString& ai_strFormat);


    //## Other Operations (specified)
      //## Operation: Set%943607445; C++
      m4return_t Set (const m4uint32_t ai_uiSeconds);

      //## Operation: Set%943607446; C++
      m4return_t Set (const M4ClString& ai_strFormat);

      //## Operation: Get%943607447; C++
      m4return_t Get (m4uint32_t &ao_uiSeconds);

      //## Operation: Get%943607448; C++
      m4return_t Get (M4ClString& ao_strFormat);

    // Additional Public Declarations
      //## begin ClTimeInterval%383D4CE701B6.public preserve=yes
      //## end ClTimeInterval%383D4CE701B6.public

  protected:

    //## Other Operations (specified)
      //## Operation: Sec2StrFormat%943607441; C++
      m4return_t Sec2StrFormat (m4uint32_t ai_uiSec, M4ClString& ao_strFormat);

      //## Operation: StrFormat2Sec%943607442
      m4return_t StrFormat2Sec ( const M4ClString& ai_strFormat, m4uint32_t& ao_uiSec);

    // Data Members for Associations

      //## Association: M4Date::<unnamed>%383D4D3B0008
      //## Role: ClTimeInterval::m_uiSeconds%383D4D3C0123
      //## begin ClTimeInterval::m_uiSeconds%383D4D3C0123.role preserve=no  protected: m4uint32_t { -> VHAN}
      m4uint32_t m_uiSeconds;
      //## end ClTimeInterval::m_uiSeconds%383D4D3C0123.role

    // Additional Protected Declarations
      //## begin ClTimeInterval%383D4CE701B6.protected preserve=yes
      //## end ClTimeInterval%383D4CE701B6.protected

  private:
    // Additional Private Declarations
      //## begin ClTimeInterval%383D4CE701B6.private preserve=yes
      //## end ClTimeInterval%383D4CE701B6.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTimeInterval%383D4CE701B6.implementation preserve=yes
      //## end ClTimeInterval%383D4CE701B6.implementation

};

//## begin ClTimeInterval%383D4CE701B6.postscript preserve=yes
//## end ClTimeInterval%383D4CE701B6.postscript

// Class ClTimeInterval 

inline ClTimeInterval::ClTimeInterval (const m4uint32_t ai_uiSeconds)
  //## begin ClTimeInterval::ClTimeInterval%943607443.hasinit preserve=no
  //## end ClTimeInterval::ClTimeInterval%943607443.hasinit
  //## begin ClTimeInterval::ClTimeInterval%943607443.initialization preserve=yes
  //## end ClTimeInterval::ClTimeInterval%943607443.initialization
{
  //## begin ClTimeInterval::ClTimeInterval%943607443.body preserve=yes
	m_uiSeconds = ai_uiSeconds;
  //## end ClTimeInterval::ClTimeInterval%943607443.body
}

inline ClTimeInterval::ClTimeInterval (const M4ClString& ai_strFormat)
  //## begin ClTimeInterval::ClTimeInterval%943607444.hasinit preserve=no
  //## end ClTimeInterval::ClTimeInterval%943607444.hasinit
  //## begin ClTimeInterval::ClTimeInterval%943607444.initialization preserve=yes
  //## end ClTimeInterval::ClTimeInterval%943607444.initialization
{
  //## begin ClTimeInterval::ClTimeInterval%943607444.body preserve=yes
	StrFormat2Sec(ai_strFormat, m_uiSeconds);
  //## end ClTimeInterval::ClTimeInterval%943607444.body
}



//## Other Operations (inline)
inline m4return_t ClTimeInterval::Set (const m4uint32_t ai_uiSeconds)
{
  //## begin ClTimeInterval::Set%943607445.body preserve=yes
	m_uiSeconds = ai_uiSeconds;
	return M4_SUCCESS;
  //## end ClTimeInterval::Set%943607445.body
}

inline m4return_t ClTimeInterval::Set (const M4ClString& ai_strFormat)
{
  //## begin ClTimeInterval::Set%943607446.body preserve=yes
	return StrFormat2Sec(ai_strFormat, m_uiSeconds);
  //## end ClTimeInterval::Set%943607446.body
}

inline m4return_t ClTimeInterval::Get (m4uint32_t &ao_uiSeconds)
{
  //## begin ClTimeInterval::Get%943607447.body preserve=yes
	ao_uiSeconds = m_uiSeconds;
	return M4_SUCCESS;
  //## end ClTimeInterval::Get%943607447.body
}

inline m4return_t ClTimeInterval::Get (M4ClString& ao_strFormat)
{
  //## begin ClTimeInterval::Get%943607448.body preserve=yes
	return Sec2StrFormat(m_uiSeconds, ao_strFormat);
  //## end ClTimeInterval::Get%943607448.body
}

//## begin module%383E4DE20075.epilog preserve=yes
//## end module%383E4DE20075.epilog


#endif

//## begin module%3777684500DC.cm preserve=no
//## end module%3777684500DC.cm

//## begin module%3777684500DC.cp preserve=no
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
//## end module%3777684500DC.cp

//## Module: windowsselectmask%3777684500DC; Package specification
//## Subsystem: M4Tsap::inc::win32%3777684500DB
//	d:\compon\m4tsap\version\inc\win32
//## Source file: d:\compon\m4tsap\version\inc\win32\windowsselectmask.hpp

#ifndef windowsselectmas_h
#define windowsselectmas_h 1

//## begin module%3777684500DC.additionalIncludes preserve=no
//## end module%3777684500DC.additionalIncludes

//## begin module%3777684500DC.includes preserve=yes

#include "m4types.hpp"
#include "m4winsock.hpp"
//## end module%3777684500DC.includes

//## begin module%3777684500DC.declarations preserve=no
//## end module%3777684500DC.declarations

//## begin module%3777684500DC.additionalDeclarations preserve=yes
//## end module%3777684500DC.additionalDeclarations


//## Class: m4fd_set%377893520159
//## Category: M4Tsap%3777684001C4
//## Subsystem: M4Tsap::inc::win32%3777684500DB
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef fd_set m4fd_set;

//## Class: m4stfd_set%37776845015D
//## Category: M4Tsap%3777684001C4
//## Subsystem: M4Tsap::inc::win32%3777684500DB
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37789AA8005F;m4fd_set { -> }

typedef m4fd_set m4stfd_set;

//## begin ClWindowsSelectMask%3777684500EF.preface preserve=yes
//## end ClWindowsSelectMask%3777684500EF.preface

//## Class: ClWindowsSelectMask%3777684500EF
//## Category: M4Tsap%3777684001C4
//## Subsystem: M4Tsap::inc::win32%3777684500DB
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%377768450117;m4bool_t { -> }
//## Uses: <unnamed>%377899CD017C;m4fd_set { -> }

class ClWindowsSelectMask 
{
  //## begin ClWindowsSelectMask%3777684500EF.initialDeclarations preserve=yes
  //## end ClWindowsSelectMask%3777684500EF.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClWindowsSelectMask%1916352889; C++
      ClWindowsSelectMask (unsigned int size = 1024);


    //## Other Operations (specified)
      //## Operation: Set%-93488872; C++
      m4bool_t Set (unsigned int n);

      //## Operation: Clear%-1537213506; C++
      //	Borro un identificador de la lista de los select
      void Clear (unsigned int n);

      //## Operation: IsSet%1495745771; C++
      m4bool_t IsSet (unsigned int n) const;

      //## Operation: NextSet%-118406073; C++
      //	Devuelvo el siquiente al úlitmo pasado que coincide con
      //	n:
      //	En unix es distinto el planteamiento
      //	-- {AddDecl: 190} region.unprotectedFunction [1400..1927]
      //	Devuelvo el siquiente al úlitmo pasado que coincide con
      //	n:
      //	En unix es distinto el planteamiento
      int NextSet (unsigned int n = 0);

      //## Operation: Zero%677878077; C++
      void Zero ();

      //## Operation: GetSize%1783046207; C++
      unsigned int GetSize () const;

      //## Operation: GetMask%-1729912024; C++
      void* GetMask () const;

      //## Operation: CopyFrom%613146576; C++
      void CopyFrom (const ClWindowsSelectMask& ai_right);

      //## Operation: IsIn%1952606541; C++
      m4bool_t IsIn (unsigned int ai_iIdentificador);

      //## Operation: operator =%91679548; C++
      ClWindowsSelectMask& operator = (const ClWindowsSelectMask& ai_right);

    // Additional Public Declarations
      //## begin ClWindowsSelectMask%3777684500EF.public preserve=yes
      //## end ClWindowsSelectMask%3777684500EF.public

  protected:
    // Additional Protected Declarations
      //## begin ClWindowsSelectMask%3777684500EF.protected preserve=yes
      //## end ClWindowsSelectMask%3777684500EF.protected

  private:
    // Data Members for Associations

      //## Association: M4Tsap::<unnamed>%37776845014A
      //## Role: ClWindowsSelectMask::m_iSetSize%37776845014C
      //## begin ClWindowsSelectMask::m_iSetSize%37776845014C.role preserve=no  private: m4uint16_t {1 -> 1VHAPN}
      m4uint16_t m_iSetSize;
      //## end ClWindowsSelectMask::m_iSetSize%37776845014C.role

      //## Association: M4Tsap::<unnamed>%37776845015E
      //## Role: ClWindowsSelectMask::m_stFdSet%377768450160
      //## begin ClWindowsSelectMask::m_stFdSet%377768450160.role preserve=no  private: m4stfd_set {1 -> 1VHAPN}
      m4stfd_set m_stFdSet;
      //## end ClWindowsSelectMask::m_stFdSet%377768450160.role

      //## Association: M4Tsap::<unnamed>%377768450168
      //## Role: ClWindowsSelectMask::m_iLastUsed%37776845016A
      //## begin ClWindowsSelectMask::m_iLastUsed%37776845016A.role preserve=no  private: m4int16_t {1 -> 1VHAPN}
      m4int16_t m_iLastUsed;
      //## end ClWindowsSelectMask::m_iLastUsed%37776845016A.role

    // Additional Private Declarations
      //## begin ClWindowsSelectMask%3777684500EF.private preserve=yes
      //## end ClWindowsSelectMask%3777684500EF.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClWindowsSelectMask%3777684500EF.implementation preserve=yes
      //## end ClWindowsSelectMask%3777684500EF.implementation

};

//## begin ClWindowsSelectMask%3777684500EF.postscript preserve=yes
//## end ClWindowsSelectMask%3777684500EF.postscript

// Class ClWindowsSelectMask 

//## begin module%3777684500DC.epilog preserve=yes
//## end module%3777684500DC.epilog


#endif

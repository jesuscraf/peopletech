//## begin module%3912C5840351.cm preserve=no
//## end module%3912C5840351.cm

//## begin module%3912C5840351.cp preserve=no
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
//## end module%3912C5840351.cp

//## Module: sizer%3912C5840351; Package specification
//## Subsystem: m4sysall::inc%3912C5D60363
//## Source file: d:\new\m4sysall\inc\sizer.hpp

#ifndef _SIZER_HPP_
#define _SIZER_HPP_ 1

//## begin module%3912C5840351.additionalIncludes preserve=no
//## end module%3912C5840351.additionalIncludes

//## begin module%3912C5840351.includes preserve=yes

#include "m4sysall_dll.hpp"
#include "m4types.hpp"
#include "m4stl.hpp"
//## end module%3912C5840351.includes

//## begin module%3912C5840351.declarations preserve=no
//## end module%3912C5840351.declarations

//## begin module%3912C5840351.additionalDeclarations preserve=yes
//## end module%3912C5840351.additionalDeclarations


//## begin ClSize%3912C584035B.preface preserve=yes
//## end ClSize%3912C584035B.preface

//## Class: ClSize%3912C584035B
//	-- {AddDecl: 88} class [160..782]
//## Category: m4sysall%3912C5830151
//## Subsystem: m4sysall::inc%3912C5D60363
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SYSALL ClSize 
{
  //## begin ClSize%3912C584035B.initialDeclarations preserve=yes
  //## end ClSize%3912C584035B.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSize%965426979; C++
      ClSize ();

      //## Operation: ClSize%-1336692163; C++
      //	-- {AddDecl: 167} region.unprotectedFunction [123..351]
      ClSize (m4pchar_t ai_pFile);

    //## Destructor (specified)
      //## Operation: ~ClSize%-1295812321; C++
      //	-- {AddDecl: 168} region.unprotectedFunction [352..450]
      virtual ~ClSize ();


    //## Other Operations (specified)
      //## Operation: operator +=%1171636332; C++
      //	-- {AddDecl: 170} region.unprotectedFunction [692..822]
      void operator += (m4uint32_t ai_i);

      //## Operation: operator -=%-1407478999; C++
      //	-- {AddDecl: 171} region.unprotectedFunction [823..953]
      void operator -= (m4uint32_t ai_i);

      //## Operation: Label%475172456; C++
      //	-- {AddDecl: 169} region.unprotectedFunction [451..691]
      void Label (m4pcchar_t ai_label, m4bool_t ai_bShowSize = M4_TRUE);

      //## Operation: operator =%931297147; C++
      //	-- {AddDecl: 172} region.unprotectedFunction [954..1032]
      void operator = (ClSize& ai_size);

      //## Operation: operator =%-1988985010; C++
      void operator = (m4uint32_t ai_iSize)
      {
        //## begin ClSize::operator =%-1988985010.body preserve=yes
        m_iSize = ai_iSize ;
    
        //## end ClSize::operator =%-1988985010.body
      }

      //## Operation: Attach%7807315; C++
      //	-- {AddDecl: 90} region.unprotectedFunction [544..648]
      //	-- {AddDecl: 173} region.unprotectedFunction [1033..1314]
      void Attach (m4pcchar_t ai_pFile);

      //## Operation: operator m4uint32_t%1710144705; C++
      operator m4uint32_t () const
      {
        //## begin ClSize::operator m4uint32_t%1710144705.body preserve=yes
        return m_iSize;
    
        //## end ClSize::operator m4uint32_t%1710144705.body
      }

    // Additional Public Declarations
      //## begin ClSize%3912C584035B.public preserve=yes
      //## end ClSize%3912C584035B.public

  protected:
    // Additional Protected Declarations
      //## begin ClSize%3912C584035B.protected preserve=yes
      //## end ClSize%3912C584035B.protected

  private:
    // Data Members for Associations

      //## Association: m4sysall::<unnamed>%3912C5850046
      //## Role: ClSize::m_iSize%3912C5850048
      //## begin ClSize::m_iSize%3912C5850048.role preserve=no  private: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_iSize;
      //## end ClSize::m_iSize%3912C5850048.role

      //## Association: m4sysall::<unnamed>%3912C58500BD
      //## Role: ClSize::m_pofs%3912C58500BF
      //## begin ClSize::m_pofs%3912C58500BF.role preserve=no  private: ofstream {1 -> 0..1RHAPN}
      ofstream *m_pofs;
      //## end ClSize::m_pofs%3912C58500BF.role

    // Additional Private Declarations
      //## begin ClSize%3912C584035B.private preserve=yes
      //## end ClSize%3912C584035B.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSize%3912C584035B.implementation preserve=yes
      //## end ClSize%3912C584035B.implementation

};

//## begin ClSize%3912C584035B.postscript preserve=yes
//## end ClSize%3912C584035B.postscript

//## begin module%3912C5840351.epilog preserve=yes
//## end module%3912C5840351.epilog


#endif

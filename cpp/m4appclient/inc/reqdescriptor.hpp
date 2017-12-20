//## begin module%3ADB03F5035B.cm preserve=no
//## end module%3ADB03F5035B.cm

//## begin module%3ADB03F5035B.cp preserve=no
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
//## end module%3ADB03F5035B.cp

//## Module: reqdescriptor%3ADB03F5035B; Package specification
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Source file: F:\integration\m4appclient\inc\reqdescriptor.hpp

#ifndef reqdescriptor_h
#define reqdescriptor_h 1

//## begin module%3ADB03F5035B.additionalIncludes preserve=no
//## end module%3ADB03F5035B.additionalIncludes

//## begin module%3ADB03F5035B.includes preserve=yes
#include <m4string.hpp>
//## end module%3ADB03F5035B.includes

//## begin module%3ADB03F5035B.declarations preserve=no
//## end module%3ADB03F5035B.declarations

//## begin module%3ADB03F5035B.additionalDeclarations preserve=yes
//## end module%3ADB03F5035B.additionalDeclarations


//## begin ClRequestDescriptor%3ADAFED80010.preface preserve=yes
//## end ClRequestDescriptor%3ADAFED80010.preface

//## Class: ClRequestDescriptor%3ADAFED80010
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClRequestDescriptor 
{
  //## begin ClRequestDescriptor%3ADAFED80010.initialDeclarations preserve=yes
  //## end ClRequestDescriptor%3ADAFED80010.initialDeclarations

  public:
    // Data Members for Class Attributes

      //## Attribute: m_strDescriptor%3ADAFF0901A1
      //## begin ClRequestDescriptor::m_strDescriptor%3ADAFF0901A1.attr preserve=no  public: m4string_t {UA} 
      m4string_t m_strDescriptor;
      //## end ClRequestDescriptor::m_strDescriptor%3ADAFF0901A1.attr

    // Additional Public Declarations
      //## begin ClRequestDescriptor%3ADAFED80010.public preserve=yes
      //## end ClRequestDescriptor%3ADAFED80010.public

  protected:
    // Additional Protected Declarations
      //## begin ClRequestDescriptor%3ADAFED80010.protected preserve=yes
      //## end ClRequestDescriptor%3ADAFED80010.protected

  private:
    // Additional Private Declarations
      //## begin ClRequestDescriptor%3ADAFED80010.private preserve=yes
      //## end ClRequestDescriptor%3ADAFED80010.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClRequestDescriptor%3ADAFED80010.implementation preserve=yes
      //## end ClRequestDescriptor%3ADAFED80010.implementation

};

//## begin ClRequestDescriptor%3ADAFED80010.postscript preserve=yes
//## end ClRequestDescriptor%3ADAFED80010.postscript

// Class ClRequestDescriptor 

//## begin module%3ADB03F5035B.epilog preserve=yes
//## end module%3ADB03F5035B.epilog


#endif

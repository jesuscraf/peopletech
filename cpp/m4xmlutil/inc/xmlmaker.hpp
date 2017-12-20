//## begin module%391BBD5F02A4.cm preserve=no
//## end module%391BBD5F02A4.cm

//## begin module%391BBD5F02A4.cp preserve=no
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
//## end module%391BBD5F02A4.cp

//## Module: xmlmaker%391BBD5F02A4; Package specification
//## Subsystem: M4XMLUtil::inc%3906B78502AA
//## Source file: F:\integration\m4xmlutil\inc\xmlmaker.hpp

#ifndef xmlmaker_h
#define xmlmaker_h 1

//## begin module%391BBD5F02A4.additionalIncludes preserve=no
//## end module%391BBD5F02A4.additionalIncludes

//## begin module%391BBD5F02A4.includes preserve=yes
#include <m4xmlutil_dll.hpp>
//## end module%391BBD5F02A4.includes

// m4string
#include <m4string.hpp>
//## begin module%391BBD5F02A4.declarations preserve=no
//## end module%391BBD5F02A4.declarations

//## begin module%391BBD5F02A4.additionalDeclarations preserve=yes
//## end module%391BBD5F02A4.additionalDeclarations


//## begin ClXMLMaker%391BBBBD00A4.preface preserve=yes
//## end ClXMLMaker%391BBBBD00A4.preface

//## Class: ClXMLMaker%391BBBBD00A4
//## Category: M4XMLUtil%3906B6A20380
//## Subsystem: M4XMLUtil::inc%3906B78502AA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%391BBDA002B1;M4ClString { -> }

class M4_DECL_M4XMLUTIL ClXMLMaker
{
  //## begin ClXMLMaker%391BBBBD00A4.initialDeclarations preserve=yes
  //## end ClXMLMaker%391BBBBD00A4.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: StartElement%958118075
      void StartElement (m4pcchar_t ai_szElement, M4ClString &ao_buffer);

      //## Operation: StartDocument%958118076
      void StartDocument (M4ClString &ao_buffer);

      //## Operation: EndElement%958118077
      void EndElement (m4pcchar_t ai_szElement, M4ClString &ao_buffer);

      //## Operation: AddParam%958118078
      void AddParam (m4pcchar_t ai_szParamName, m4pcchar_t ai_szParamValue, M4ClString &ao_buffer);

    // Additional Public Declarations
      //## begin ClXMLMaker%391BBBBD00A4.public preserve=yes
      //## end ClXMLMaker%391BBBBD00A4.public

  protected:
    // Additional Protected Declarations
      //## begin ClXMLMaker%391BBBBD00A4.protected preserve=yes
      //## end ClXMLMaker%391BBBBD00A4.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_szCurrentElement%391BC2C10328
      //## begin ClXMLMaker::m_szCurrentElement%391BC2C10328.attr preserve=no  private: M4ClString {UA} 
      M4ClString m_szCurrentElement;
      //## end ClXMLMaker::m_szCurrentElement%391BC2C10328.attr

      //## Attribute: m_iDeep%391BC2E303B3
      //## begin ClXMLMaker::m_iDeep%391BC2E303B3.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iDeep;
      //## end ClXMLMaker::m_iDeep%391BC2E303B3.attr

    // Additional Private Declarations
      //## begin ClXMLMaker%391BBBBD00A4.private preserve=yes
      //## end ClXMLMaker%391BBBBD00A4.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClXMLMaker%391BBBBD00A4.implementation preserve=yes
      //## end ClXMLMaker%391BBBBD00A4.implementation

};

//## begin ClXMLMaker%391BBBBD00A4.postscript preserve=yes
//## end ClXMLMaker%391BBBBD00A4.postscript

// Class ClXMLMaker 

//## begin module%391BBD5F02A4.epilog preserve=yes
//## end module%391BBD5F02A4.epilog


#endif

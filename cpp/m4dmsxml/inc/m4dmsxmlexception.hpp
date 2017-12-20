//## begin module%3A54BE010136.cm preserve=no
//## end module%3A54BE010136.cm

//## begin module%3A54BE010136.cp preserve=no
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
//## end module%3A54BE010136.cp

//## Module: m4dmsxmlexception%3A54BE010136; Package specification
//## Subsystem: M4DMSXml::inc%3A3E5D200088
//## Source file: e:\m4server\m4dmsxml\inc\m4dmsxmlexception.hpp

#ifndef m4dmsxmlexception_h
#define m4dmsxmlexception_h 1

//## begin module%3A54BE010136.additionalIncludes preserve=no
//## end module%3A54BE010136.additionalIncludes

//## begin module%3A54BE010136.includes preserve=yes
#include <m4types.hpp>
//## end module%3A54BE010136.includes

//## begin module%3A54BE010136.declarations preserve=no
//## end module%3A54BE010136.declarations

//## begin module%3A54BE010136.additionalDeclarations preserve=yes
// ---------------------------------------------------------------------------
//  This macro is used to actually throw an exception. It is used in order
//  to make sure that source code line/col info is stored correctly, and to
//  give flexibility for other stuff in the future.
//
//	To avoid multiple concurrent exceptions, we check the
//	ClDMSXmlException::in_exception flag.
// ---------------------------------------------------------------------------
// if (!ClDMSXmlException::m_bInException)
#define ThrowDMSXMLException(ai_uiCode, ai_uiCurrentRegister) \
{ \
	throw ClDMSXmlException(__FILE__, __LINE__, ai_uiCode, ai_uiCurrentRegister); \
}

//## end module%3A54BE010136.additionalDeclarations


//## begin eXmlExceptionCodes%3A70578B019B.preface preserve=yes
//## end eXmlExceptionCodes%3A70578B019B.preface

//## Class: eXmlExceptionCodes%3A70578B019B
//## Category: M4DMSXml%3A3DFB720294
//## Subsystem: M4DMSXml::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef enum {NoError=0, StartElement, Characters, EndElement} eXmlExceptionCodes;

//## begin eXmlExceptionCodes%3A70578B019B.postscript preserve=yes
//## end eXmlExceptionCodes%3A70578B019B.postscript

//## begin ClDMSXmlException%3A54A86100E5.preface preserve=yes
//## end ClDMSXmlException%3A54A86100E5.preface

//## Class: ClDMSXmlException%3A54A86100E5
//## Category: M4DMSXml%3A3DFB720294
//## Subsystem: M4DMSXml::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3A707D970182;eXmlExceptionCodes { -> }

class ClDMSXmlException 
{
  //## begin ClDMSXmlException%3A54A86100E5.initialDeclarations preserve=yes
  //## end ClDMSXmlException%3A54A86100E5.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClDMSXmlException%978515141
      ClDMSXmlException (m4pcchar_t ai_pcSrcFile, const m4uint32_t &ai_uiLine, const eXmlExceptionCodes &ai_eToThrow, const m4int32_t &ai_iCurrentRegister);

    //## Destructor (specified)
      //## Operation: ~ClDMSXmlException%978515142
      virtual ~ClDMSXmlException ();

    // Data Members for Class Attributes

      //## Attribute: m_pcFile%3A54B9FA01C9
      //## begin ClDMSXmlException::m_pcFile%3A54B9FA01C9.attr preserve=no  public: m4pcchar_t {UA} ai_pcSrcFile
      m4pcchar_t m_pcFile;
      //## end ClDMSXmlException::m_pcFile%3A54B9FA01C9.attr

      //## Attribute: m_uiLine%3A54BA01015B
      //## begin ClDMSXmlException::m_uiLine%3A54BA01015B.attr preserve=no  public: m4uint32_t {UA} ai_uiLine
      m4uint32_t m_uiLine;
      //## end ClDMSXmlException::m_uiLine%3A54BA01015B.attr

      //## Attribute: m_uiCurrentRegister%3A5AD4A502F7
      //## begin ClDMSXmlException::m_uiCurrentRegister%3A5AD4A502F7.attr preserve=no  public: m4int32_t {UA} ai_iCurrentRegister
      m4int32_t m_uiCurrentRegister;
      //## end ClDMSXmlException::m_uiCurrentRegister%3A5AD4A502F7.attr

      //## Attribute: m_eCode%3A707F2202CB
      //## begin ClDMSXmlException::m_eCode%3A707F2202CB.attr preserve=no  public: eXmlExceptionCodes {UA} ai_eToThrow
      eXmlExceptionCodes m_eCode;
      //## end ClDMSXmlException::m_eCode%3A707F2202CB.attr

    // Additional Public Declarations
      //## begin ClDMSXmlException%3A54A86100E5.public preserve=yes
      //## end ClDMSXmlException%3A54A86100E5.public

  protected:
    // Additional Protected Declarations
      //## begin ClDMSXmlException%3A54A86100E5.protected preserve=yes
      //## end ClDMSXmlException%3A54A86100E5.protected

  private:
    // Additional Private Declarations
      //## begin ClDMSXmlException%3A54A86100E5.private preserve=yes
      //## end ClDMSXmlException%3A54A86100E5.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDMSXmlException%3A54A86100E5.implementation preserve=yes
      //## end ClDMSXmlException%3A54A86100E5.implementation

};

//## begin ClDMSXmlException%3A54A86100E5.postscript preserve=yes
//## end ClDMSXmlException%3A54A86100E5.postscript

// Class ClDMSXmlException 

//## begin module%3A54BE010136.epilog preserve=yes
//## end module%3A54BE010136.epilog


#endif

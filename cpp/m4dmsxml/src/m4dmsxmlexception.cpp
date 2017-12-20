//## begin module%3A54BE5E03A6.cm preserve=no
//## end module%3A54BE5E03A6.cm

//## begin module%3A54BE5E03A6.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
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
//## end module%3A54BE5E03A6.cp

//## Module: m4dmsxmlexception%3A54BE5E03A6; Package body
//## Subsystem: M4DMSXml::src%3A3E5D2D0159
//## Source file: e:\m4server\m4dmsxml\src\m4dmsxmlexception.cpp

//## begin module%3A54BE5E03A6.additionalIncludes preserve=no
//## end module%3A54BE5E03A6.additionalIncludes

//## begin module%3A54BE5E03A6.includes preserve=yes
//## end module%3A54BE5E03A6.includes

// m4dmsxmlexception
#include <m4dmsxmlexception.hpp>
//## begin module%3A54BE5E03A6.declarations preserve=no
//## end module%3A54BE5E03A6.declarations

//## begin module%3A54BE5E03A6.additionalDeclarations preserve=yes
//## end module%3A54BE5E03A6.additionalDeclarations


// Class ClDMSXmlException 





ClDMSXmlException::ClDMSXmlException (m4pcchar_t ai_pcSrcFile, const m4uint32_t &ai_uiLine, const eXmlExceptionCodes &ai_eToThrow, const m4int32_t &ai_iCurrentRegister)
  //## begin ClDMSXmlException::ClDMSXmlException%978515141.hasinit preserve=no
      : m_pcFile(ai_pcSrcFile), m_uiLine(ai_uiLine), m_uiCurrentRegister(ai_iCurrentRegister), m_eCode(ai_eToThrow)
  //## end ClDMSXmlException::ClDMSXmlException%978515141.hasinit
  //## begin ClDMSXmlException::ClDMSXmlException%978515141.initialization preserve=yes
  //## end ClDMSXmlException::ClDMSXmlException%978515141.initialization
{
  //## begin ClDMSXmlException::ClDMSXmlException%978515141.body preserve=yes
  //## end ClDMSXmlException::ClDMSXmlException%978515141.body
}


ClDMSXmlException::~ClDMSXmlException ()
{
  //## begin ClDMSXmlException::~ClDMSXmlException%978515142.body preserve=yes
  //## end ClDMSXmlException::~ClDMSXmlException%978515142.body
}


// Additional Declarations
  //## begin ClDMSXmlException%3A54A86100E5.declarations preserve=yes
  //## end ClDMSXmlException%3A54A86100E5.declarations

//## begin module%3A54BE5E03A6.epilog preserve=yes
//## end module%3A54BE5E03A6.epilog

//## begin module%3906B79202C7.cm preserve=no
//## end module%3906B79202C7.cm

//## begin module%3906B79202C7.cp preserve=no
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
//## end module%3906B79202C7.cp

//## Module: m4xmlbase%3906B79202C7; Package body
//## Subsystem: M4XMLUtil::src%3906B78B02DB
//## Source file: C:\m4server\m4xmlutil\src\m4xmlbase.cpp

//## begin module%3906B79202C7.additionalIncludes preserve=no
//## end module%3906B79202C7.additionalIncludes

//## begin module%3906B79202C7.includes preserve=yes
//## end module%3906B79202C7.includes

// SAXHandlerBase
#include <xercesc/sax/HandlerBase.hpp>
// m4xmlbase
#include <m4xmlbase.hpp>
#include "m4unicode.hpp"


//## begin module%3906B79202C7.declarations preserve=no
//## end module%3906B79202C7.declarations

//## begin module%3906B79202C7.additionalDeclarations preserve=yes
//## end module%3906B79202C7.additionalDeclarations


// Class M4SAXHandlerBase 





M4SAXHandlerBase::M4SAXHandlerBase ()
  //## begin M4SAXHandlerBase::M4SAXHandlerBase%956652051.hasinit preserve=no
  //## end M4SAXHandlerBase::M4SAXHandlerBase%956652051.hasinit
  //## begin M4SAXHandlerBase::M4SAXHandlerBase%956652051.initialization preserve=yes
  //## end M4SAXHandlerBase::M4SAXHandlerBase%956652051.initialization
{
  //## begin M4SAXHandlerBase::M4SAXHandlerBase%956652051.body preserve=yes
	m_bIsOk = M4_TRUE ;
	m_iError = M4_SUCCESS ;

	//los constructores de las clases derivadas de esta deben de registrar sus métodos de la siguiente forma
	//	RegisterElementProc ("commnad",&M4SAXHandlerBase::startCommnad));

  //## end M4SAXHandlerBase::M4SAXHandlerBase%956652051.body
}


M4SAXHandlerBase::~M4SAXHandlerBase ()
{
  //## begin M4SAXHandlerBase::~M4SAXHandlerBase%956652052.body preserve=yes
  //## end M4SAXHandlerBase::~M4SAXHandlerBase%956652052.body

	ElementProcIter		StartIter ;
	ElementProcEndIter	EndIter ;


	StartIter = m_oElementProcMap.begin() ;

	while( StartIter != m_oElementProcMap.end() )
	{
		delete [] (*StartIter).first ;
		StartIter++ ;
	}

	EndIter = m_oElementEndProcMap.begin() ;

	while( EndIter != m_oElementEndProcMap.end() )
	{
		delete [] (*EndIter).first ;
		EndIter++ ;
	}
}



//## Other Operations (implementation)
void M4SAXHandlerBase::endDocument ()
{
  //## begin M4SAXHandlerBase::endDocument%956652054.body preserve=yes
  //## end M4SAXHandlerBase::endDocument%956652054.body
}

void M4SAXHandlerBase::endElement (const XMLCh * const name)
{
  //## begin M4SAXHandlerBase::endElement%956652055.body preserve=yes
	ElementProcEndIter it ;
	m4uint32_t index = 0 ;

	if( m_bIsOk == M4_TRUE )
	{
		it = m_oElementEndProcMap.find((XMLCharPointer_t)name);
		if (it == m_oElementEndProcMap.end())
			return;

		(this->*((*it).second))(name);
	}
  //## end M4SAXHandlerBase::endElement%956652055.body
}

void M4SAXHandlerBase::startDocument ()
{
  //## begin M4SAXHandlerBase::startDocument%956652056.body preserve=yes
  //## end M4SAXHandlerBase::startDocument%956652056.body
}

void M4SAXHandlerBase::startElement (const XMLCh* const name, AttributeList& attributes)
{
  //## begin M4SAXHandlerBase::startElement%956652057.body preserve=yes
	ElementProcIter it ;
	m4uint32_t index = 0 ;

	if( m_bIsOk == M4_TRUE )
	{
		it = m_oElementProcMap.find((XMLCharPointer_t)name);
		if (it == m_oElementProcMap.end())
			return;

		(this->*((*it).second))(name,&attributes);
	}
  //## end M4SAXHandlerBase::startElement%956652057.body
}

void M4SAXHandlerBase::characters (const XMLCh* const chars, const unsigned int length)
{
  //## begin M4SAXHandlerBase::characters%1082986242.body preserve=yes
 //## end M4SAXHandlerBase::characters%1082986242.body
}

void M4SAXHandlerBase::warning (const SAXParseException& exception)
{
  //## begin M4SAXHandlerBase::warning%956652058.body preserve=yes
  //## end M4SAXHandlerBase::warning%956652058.body
}

void M4SAXHandlerBase::error (const SAXParseException& exception)
{
  //## begin M4SAXHandlerBase::error%956652059.body preserve=yes
	m_bIsOk = M4_FALSE ;
	m_iError = M4_ERROR ;
  //## end M4SAXHandlerBase::error%956652059.body
}

void M4SAXHandlerBase::fatalError (const SAXParseException& exception)
{
  //## begin M4SAXHandlerBase::fatalError%956652060.body preserve=yes
	m_bIsOk = M4_FALSE ;
	m_iError = M4_ERROR ;
  //## end M4SAXHandlerBase::fatalError%956652060.body
}

void M4SAXHandlerBase::resetErrors ()
{
  //## begin M4SAXHandlerBase::resetErrors%956652061.body preserve=yes
	m_bIsOk = M4_TRUE ;
	m_iError = M4_SUCCESS ;
  //## end M4SAXHandlerBase::resetErrors%956652061.body
}

void M4SAXHandlerBase::RegisterElementProc (m4pchar_t ai_string, ElementProcStart ai_pElementProc)
{
  //## begin M4SAXHandlerBase::RegisterElementProc%956741691.body preserve=yes
	// UNICODE XML
	XMLCh* pcName = (XMLCh*)M4CppToXML( ai_string ) ;
	m_oElementProcMap.insert(ElementProcStartMap::value_type(pcName,ai_pElementProc));
  //## end M4SAXHandlerBase::RegisterElementProc%956741691.body
}

void M4SAXHandlerBase::RegisterElementEndProc (m4pchar_t ai_string, ElementProcEnd ai_pElementProc)
{
  //## begin M4SAXHandlerBase::RegisterElementEndProc%960828891.body preserve=yes
	// UNICODE XML
	XMLCh* pcName = (XMLCh*)M4CppToXML( ai_string ) ;
	m_oElementEndProcMap.insert(ElementProcEndMap::value_type(pcName,ai_pElementProc));
  //## end M4SAXHandlerBase::RegisterElementEndProc%960828891.body
}

// Additional Declarations
  //## begin M4SAXHandlerBase%3905593E00DA.declarations preserve=yes
  //## end M4SAXHandlerBase%3905593E00DA.declarations


//## begin module%3906B79202C7.epilog preserve=yes
//## end module%3906B79202C7.epilog

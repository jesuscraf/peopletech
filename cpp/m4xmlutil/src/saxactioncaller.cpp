//## begin module%39466D8301B8.cm preserve=no
//## end module%39466D8301B8.cm

//## begin module%39466D8301B8.cp preserve=no
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
//## end module%39466D8301B8.cp

//## Module: saxactioncaller%39466D8301B8; Package body
//## Subsystem: M4XMLUtil::src%3906B78B02DB
//## Source file: F:\integration\m4xmlutil\src\saxactioncaller.cpp

//## begin module%39466D8301B8.additionalIncludes preserve=no
//## end module%39466D8301B8.additionalIncludes

//## begin module%39466D8301B8.includes preserve=yes
//## end module%39466D8301B8.includes

// ParamList
#include <paramlist.hpp>
// saxactioncaller
#include <saxactioncaller.hpp>
// m4xmlbase
#include <m4xmlbase.hpp>
// iexecutable
#include <iexecutable.hpp>
#include "m4unicode.hpp"

//## begin module%39466D8301B8.declarations preserve=no
//## end module%39466D8301B8.declarations

//## begin module%39466D8301B8.additionalDeclarations preserve=yes
//## end module%39466D8301B8.additionalDeclarations


// Class ClSAXHandlerActionCaller 






ClSAXHandlerActionCaller::ClSAXHandlerActionCaller (ClExecutableInterface *ai_poExecutableInterface, ClParamList *ai_poParamList)
  //## begin ClSAXHandlerActionCaller::ClSAXHandlerActionCaller%960917069.hasinit preserve=no
  //## end ClSAXHandlerActionCaller::ClSAXHandlerActionCaller%960917069.hasinit
  //## begin ClSAXHandlerActionCaller::ClSAXHandlerActionCaller%960917069.initialization preserve=yes
  //## end ClSAXHandlerActionCaller::ClSAXHandlerActionCaller%960917069.initialization
{
  //## begin ClSAXHandlerActionCaller::ClSAXHandlerActionCaller%960917069.body preserve=yes
	m_poExecutive = ai_poExecutableInterface ;
	m_poContext = NULL ;
//	m_iSessionId = ai_iSessionId ;
//	m_iXMLId =  ai_iXMLId ;
	m_poParamList = ai_poParamList ; 
	m_bParamListOwner = M4_FALSE;

	SetError () ;

	RegisterElementProc ( "command" , (ElementProcStart)&ClSAXHandlerActionCaller::procStartCommand );
	RegisterElementProc ( "config" , (ElementProcStart)&ClSAXHandlerActionCaller::procStartCommand );
	RegisterElementEndProc ( "command" , (ElementProcEnd)&ClSAXHandlerActionCaller::procEndCommand  );
	RegisterElementEndProc ( "config" , (ElementProcEnd)&ClSAXHandlerActionCaller::procEndCommand  );
	RegisterElementProc ( "params" , (ElementProcStart)&ClSAXHandlerActionCaller::procStartParams );

  //## end ClSAXHandlerActionCaller::ClSAXHandlerActionCaller%960917069.body
}


ClSAXHandlerActionCaller::~ClSAXHandlerActionCaller ()
{
  //## begin ClSAXHandlerActionCaller::~ClSAXHandlerActionCaller%960917070.body preserve=yes
  //## end ClSAXHandlerActionCaller::~ClSAXHandlerActionCaller%960917070.body
}



//## Other Operations (implementation)
void ClSAXHandlerActionCaller::procStartCommand (const XMLCh* const name, AttributeList *attributes)
{
  //## begin ClSAXHandlerActionCaller::procStartCommand%960917071.body preserve=yes
	m4int_t iParam = 0 ;

	resetErrors () ;

	if ( m_poContext )
	{
		SetError () ;

		return ;
	}


	if ( ! attributes -> getLength() )
	{
		return ;
	}

	if (! m_poParamList )
	{
		m_poParamList = new ClParamList ;
	}

	// UNICODE XML
	string sParam;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue( iParam ), sParam ) ;

	m_poContext = new ClCurrentContext ( sParam, m_poParamList ) ;

  //## end ClSAXHandlerActionCaller::procStartCommand%960917071.body
}

void ClSAXHandlerActionCaller::procEndCommand (const XMLCh* const name)
{
  //## begin ClSAXHandlerActionCaller::procEndCommand%960917072.body preserve=yes
	ClParamList *poParamList ;


	if ( ! m_poContext )
	{
		SetError () ;

		return ;
	}


	if ( M4_ERROR == GetError () )
	{
		return ;
	}

	if ( ! ( poParamList = m_poContext -> GetParamList () ) )
	{
		return ;
	}

	m_poExecutive -> ExecuteAction ( (m4pchar_t)(m_poContext -> GetCommand ( )).c_str() , m_poContext -> GetParamList () ) ;
			
	delete m_poContext ;

	m_poContext = NULL ;

	//El siguiente comando del XML no usará el mismo paramlist
	m_poParamList = NULL ;

  //## end ClSAXHandlerActionCaller::procEndCommand%960917072.body
}

void ClSAXHandlerActionCaller::procStartParams (const XMLCh* const name, AttributeList *attributes)
{
  //## begin ClSAXHandlerActionCaller::procStartParams%960917073.body preserve=yes
	ClParamList * poParamList ;
	m4uint32_t iterParams ;


	if ( ! m_poContext )
	{
		return ;
	}

	if ( M4_ERROR == GetError () )
	{
		return	;
	}

	m4uint32_t iNumParams = attributes -> getLength() ;

	if ( ! ( poParamList = m_poContext -> GetParamList () ) )
	{
		poParamList = new ClParamList ;
	}


	for ( iterParams = 0 ; iterParams < iNumParams ; iterParams  ++ )
	{
		// UNICODE XML
		string sName;
		string sValue;

		M4XMLToSTL( (M4XMLCh*)attributes->getName( iterParams ), sName ) ;
		M4XMLToSTL( (M4XMLCh*)attributes->getValue( iterParams ), sValue ) ;

		poParamList -> SetString ( sName.c_str(), sValue.c_str() ) ;
	}
  //## end ClSAXHandlerActionCaller::procStartParams%960917073.body
}

ClCurrentContext * ClSAXHandlerActionCaller::GetContext ()
{
  //## begin ClSAXHandlerActionCaller::GetContext%962094004.body preserve=yes
	return m_poContext ;
  //## end ClSAXHandlerActionCaller::GetContext%962094004.body
}

// Additional Declarations
  //## begin ClSAXHandlerActionCaller%3946685503CE.declarations preserve=yes
  //## end ClSAXHandlerActionCaller%3946685503CE.declarations

// Class ClCurrentContext 




ClCurrentContext::ClCurrentContext (m4string_t ai_szCommand, ClParamList *ai_poParamList)
  //## begin ClCurrentContext::ClCurrentContext%960917066.hasinit preserve=no
  //## end ClCurrentContext::ClCurrentContext%960917066.hasinit
  //## begin ClCurrentContext::ClCurrentContext%960917066.initialization preserve=yes
  //## end ClCurrentContext::ClCurrentContext%960917066.initialization
{
  //## begin ClCurrentContext::ClCurrentContext%960917066.body preserve=yes
	if ( ! ai_poParamList )
	{
		m_poParamList = new ClParamList ;
	}
	else
	{
		m_poParamList = ai_poParamList ;
	}

	m_szCommand = ai_szCommand ;
  //## end ClCurrentContext::ClCurrentContext%960917066.body
}


ClCurrentContext::~ClCurrentContext ()
{
  //## begin ClCurrentContext::~ClCurrentContext%960917067.body preserve=yes
  //## end ClCurrentContext::~ClCurrentContext%960917067.body
}



//## Other Operations (implementation)
ClParamList * ClCurrentContext::GetParamList ()
{
  //## begin ClCurrentContext::GetParamList%960917068.body preserve=yes
	return m_poParamList ;
  //## end ClCurrentContext::GetParamList%960917068.body
}

m4string_t ClCurrentContext::GetCommand ()
{
  //## begin ClCurrentContext::GetCommand%960917074.body preserve=yes
	return m_szCommand ;
  //## end ClCurrentContext::GetCommand%960917074.body
}

// Additional Declarations
  //## begin ClCurrentContext%3946699000CE.declarations preserve=yes
  //## end ClCurrentContext%3946699000CE.declarations

//## begin module%39466D8301B8.epilog preserve=yes
//## end module%39466D8301B8.epilog

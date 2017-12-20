//## begin module%34D5CAB40378.cm preserve=no
//## end module%34D5CAB40378.cm

//## begin module%34D5CAB40378.cp preserve=no
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
//## end module%34D5CAB40378.cp

//## Module: propertiestable%34D5CAB40378; Package body
//## Subsystem: M4Configuration::src%379DD22F012D
//## Source file: F:\integration\M4Configuration\version\src\propertiestable.cpp

//## begin module%34D5CAB40378.additionalIncludes preserve=no
//## end module%34D5CAB40378.additionalIncludes

//## begin module%34D5CAB40378.includes preserve=yes
#include <m4log.hpp>
#include <m4srvres.hpp>
#include <cloblconfiguration.hpp>
//## end module%34D5CAB40378.includes

// properties
#include <properties.hpp>
// Directory
#include <directory.hpp>
// propertiestable
#include <propertiestable.hpp>
//## begin module%34D5CAB40378.declarations preserve=no
//## end module%34D5CAB40378.declarations

//## begin module%34D5CAB40378.additionalDeclarations preserve=yes
//## end module%34D5CAB40378.additionalDeclarations


// Class ClTables 










ClTables::ClTables ()
  //## begin ClTables::ClTables%886431194.hasinit preserve=no
  //## end ClTables::ClTables%886431194.hasinit
  //## begin ClTables::ClTables%886431194.initialization preserve=yes
  //## end ClTables::ClTables%886431194.initialization
{
  //## begin ClTables::ClTables%886431194.body preserve=yes
  m_oBook = new ClOblConfiguration ;

  m_oTable = new ClOblConfiguration ;

  //## end ClTables::ClTables%886431194.body
}


ClTables::~ClTables ()
{
  //## begin ClTables::~ClTables%886431195.body preserve=yes

	if ( m_oBook  )
	{
		delete m_oBook ;

		m_oBook = NULL ;
	}
	
	
	if ( m_oTable  )
	{
		delete m_oTable ;

		 m_oTable = NULL ;
	}

  //## end ClTables::~ClTables%886431195.body
}



//## Other Operations (implementation)
m4int16_t ClTables::GetId (m4pchar_t ai_szProperty)
{
  //## begin ClTables::GetId%886431199.body preserve=yes
	m4pchar_t szId ;
	m4uint16_t iId ;
	m4int16_t iRet ;

	
	iRet = m_oIds.GetpProperty( ai_szProperty , szId ) ;

	if ( iRet == M4_ERROR )
	{
		return M4_ERROR ;
	}

	iId = atoi ( szId ) ;

	return iId;
  //## end ClTables::GetId%886431199.body
}

m4return_t ClTables::GetAttrib (m4pchar_t ai_szProperty, m4uint16_t &ao_iType, m4char_t &ao_iScale, m4pchar_t &ao_szLiteral)
{
  //## begin ClTables::GetAttrib%886431200.body preserve=yes
	m4pchar_t szValue ;

	
	
	if ( M4_ERROR == m_oTypes.GetpProperty ( ai_szProperty , szValue ) )
	{
		return M4_ERROR ;
	}

	ao_iType = atoi ( szValue ) ;

	if ( M4_ERROR == m_oScales.GetpProperty ( ai_szProperty , szValue ) )
	{
		ao_iScale = 0;
	}
	else
	{
		ao_iScale = atoi ( szValue ) ;
	}

	if( M4_ERROR == m_oLiterals.GetpProperty ( ai_szProperty , ao_szLiteral ) )
	{
		return M4_ERROR ;
	}
	
	return M4_SUCCESS;

  //## end ClTables::GetAttrib%886431200.body
}

m4pchar_t ClTables::GetLiteral (m4pchar_t ai_szProperty)
{
  //## begin ClTables::GetLiteral%889693443.body preserve=yes
	m4pchar_t szLiteral ;
	
	if ( m_oLiterals.GetpProperty ( ai_szProperty , szLiteral ) == M4_ERROR )
	{
		SETCODEF ( M4_SRV_SS_ATTRIBUTES_ERROR , ERRORLOG , "Property %s hasn't literal" , ai_szProperty ) ;

		return NULL ;
	}

	return szLiteral ;
  //## end ClTables::GetLiteral%889693443.body
}

// Additional Declarations
  //## begin ClTables%34D5B45103C7.declarations preserve=yes
  //## end ClTables%34D5B45103C7.declarations

//## begin module%34D5CAB40378.epilog preserve=yes
//## end module%34D5CAB40378.epilog

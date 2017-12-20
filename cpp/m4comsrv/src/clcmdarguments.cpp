//## begin module.cm preserve=no
//## end module.cm

//## begin module.cp preserve=no
//	=========================================================
//	=====================
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
//	=========================================================
//	====================
//## end module.cp

//## Module: ClCmdArguments; Package body
//## Subsystem: CPM4ComSrv::version
//## Source file: X:\m4rel\m4source\3x_work\core\communication\working\oscar\M4ComSrv\version\src\clcmdarguments.cpp

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
#include <m4log.hpp>
#include <m4srvres.hpp>
//## end module.includes

// ClCmdArguments
#include <clcmdarguments.hpp>
//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes
//## end module.additionalDeclarations


// Class ClCmdArguments 



ClCmdArguments::ClCmdArguments ()
  //## begin ClCmdArguments::ClCmdArguments%563430548.hasinit preserve=no
  //## end ClCmdArguments::ClCmdArguments%563430548.hasinit
  //## begin ClCmdArguments::ClCmdArguments%563430548.initialization preserve=yes
  //## end ClCmdArguments::ClCmdArguments%563430548.initialization
{
  //## begin ClCmdArguments::ClCmdArguments%563430548.body preserve=yes
  //## end ClCmdArguments::ClCmdArguments%563430548.body
}

ClCmdArguments::ClCmdArguments (void* ai_poArgument)
  //## begin ClCmdArguments::ClCmdArguments%1589632728.hasinit preserve=no
  //## end ClCmdArguments::ClCmdArguments%1589632728.hasinit
  //## begin ClCmdArguments::ClCmdArguments%1589632728.initialization preserve=yes
  //## end ClCmdArguments::ClCmdArguments%1589632728.initialization
{
  //## begin ClCmdArguments::ClCmdArguments%1589632728.body preserve=yes
	m_pArguments.push_back( ai_poArgument ) ;
//	m_lArguments.push_back( ai_lArgument ) ;
  //## end ClCmdArguments::ClCmdArguments%1589632728.body
}


ClCmdArguments::~ClCmdArguments ()
{
  //## begin ClCmdArguments::~ClCmdArguments%-1098612200.body preserve=yes
  //## end ClCmdArguments::~ClCmdArguments%-1098612200.body
}



//## Other Operations (implementation)
void ClCmdArguments::SetNArgument (m4int32_t ai_lArgument)
{
  //## begin ClCmdArguments::SetNArgument%1868093859.body preserve=yes

	m_lArguments.push_back ( ai_lArgument ) ;

  //## end ClCmdArguments::SetNArgument%1868093859.body
}

void ClCmdArguments::SetPArgument (void* ai_poArgument)
{
  //## begin ClCmdArguments::SetPArgument%877369599.body preserve=yes
	m_pArguments.push_back ( ai_poArgument ) ;
  //## end ClCmdArguments::SetPArgument%877369599.body
}

m4int32_t ClCmdArguments::GetNArgument ()
{
  //## begin ClCmdArguments::GetNArgument%875106861.body preserve=yes
//	m_lArguments.push_back ( ai_lArgument ) ;

  m4int32_t lArgument = M4_ERROR;

  if ( m_lArguments.begin ( ) == m_lArguments.end ( ) )
  {
	  SETCODEF ( M4_SRV_LM_ARGUMENT_ERROR , ERRORLOG , "Cannot return an argument, the numeric argument list is empty" ) ;

	  return M4_ERROR ;
  }
	lArgument = ( * m_lArguments.begin ( ) ) ;

	m_lArguments.pop_front() ;

  if ( lArgument == M4_ERROR )
  {
		SETCODEF ( M4_SRV_LM_ARGUMENT_ERROR , ERRORLOG , "triying to get a numeric argument that is not asigned"  ); 
		
		return M4_ERROR;
  }

  return lArgument ;
  //## end ClCmdArguments::GetNArgument%875106861.body
}

void* ClCmdArguments::GetPArgument ()
{
  //## begin ClCmdArguments::GetPArgument%877369600.body preserve=yes
  void * poArgument = NULL;

	poArgument = ( * m_pArguments.begin ( ) ) ;

	m_pArguments.pop_front() ;

  if ( poArgument == NULL )
  {
		SETCODEF ( M4_SRV_LM_ARGUMENT_ERROR , ERRORLOG , "Trying to get a numeric argument that is not asigned"  ); 
		
		return NULL;
  }

  return poArgument ;
  //## end ClCmdArguments::GetPArgument%877369600.body
}

void ClCmdArguments::Clear ()
{
  //## begin ClCmdArguments::Clear%877369601.body preserve=yes
	m_lArguments.clear();

	m_pArguments.clear();
  //## end ClCmdArguments::Clear%877369601.body
}

// Additional Declarations
  //## begin ClCmdArguments.declarations preserve=yes
  //## end ClCmdArguments.declarations



//## begin module.epilog preserve=yes
//## end module.epilog

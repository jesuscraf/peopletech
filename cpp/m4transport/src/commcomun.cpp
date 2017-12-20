//## begin module%38D2274400EF.cm preserve=no
//## end module%38D2274400EF.cm

//## begin module%38D2274400EF.cp preserve=no
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
//## end module%38D2274400EF.cp

//## Module: CommComun%38D2274400EF; Package body
//## Subsystem: m4transport::comunes::src%38D22739017F
//## Source file: c:\Componentes\m4transport\src\commcomun.cpp

//## begin module%38D2274400EF.additionalIncludes preserve=no
//## end module%38D2274400EF.additionalIncludes

//## begin module%38D2274400EF.includes preserve=yes
//## end module%38D2274400EF.includes

// CommComun
#include <commcomun.hpp>
// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
//## begin module%38D2274400EF.declarations preserve=no
//## end module%38D2274400EF.declarations

//## begin module%38D2274400EF.additionalDeclarations preserve=yes
//## end module%38D2274400EF.additionalDeclarations


// Class ClIOEventHandler 









//## Other Operations (implementation)
char * ClIOEventHandler::GetBuffer ()
{
  //## begin ClIOEventHandler::GetBuffer%954835567.body preserve=yes
	return m_pcBuffer;
  //## end ClIOEventHandler::GetBuffer%954835567.body
}

size_t ClIOEventHandler::GetBufferSize ()
{
  //## begin ClIOEventHandler::GetBufferSize%954835568.body preserve=yes
	return m_bufferSize;
  //## end ClIOEventHandler::GetBufferSize%954835568.body
}

void ClIOEventHandler::SetBufferSize (size_t ai_Size)
{
  //## begin ClIOEventHandler::SetBufferSize%954835569.body preserve=yes
	m_bufferSize = ai_Size;
  //## end ClIOEventHandler::SetBufferSize%954835569.body
}

void ClIOEventHandler::SetBuffer (char *ai_pBuffer)
{
  //## begin ClIOEventHandler::SetBuffer%954835570.body preserve=yes
	m_pcBuffer = ai_pBuffer;
  //## end ClIOEventHandler::SetBuffer%954835570.body
}

// Additional Declarations
  //## begin ClIOEventHandler%38E30F2A00AD.declarations preserve=yes
  //## end ClIOEventHandler%38E30F2A00AD.declarations

// Class ClSSLParams 

// Additional Declarations
  //## begin ClSSLParams%38DF823F02DC.declarations preserve=yes
  //## end ClSSLParams%38DF823F02DC.declarations

// Class ClCommURL 

ClCommURL::ClCommURL (m4string_t &ai_strURL)
  //## begin ClCommURL::ClCommURL%954405689.hasinit preserve=no
  //## end ClCommURL::ClCommURL%954405689.hasinit
  //## begin ClCommURL::ClCommURL%954405689.initialization preserve=yes
  //## end ClCommURL::ClCommURL%954405689.initialization
{
  //## begin ClCommURL::ClCommURL%954405689.body preserve=yes
  //## end ClCommURL::ClCommURL%954405689.body
}


// Additional Declarations
  //## begin ClCommURL%38DF80F10231.declarations preserve=yes
  //## end ClCommURL%38DF80F10231.declarations

// Class ClNetObject 

// Additional Declarations
  //## begin ClNetObject%38D662360282.declarations preserve=yes
  //## end ClNetObject%38D662360282.declarations

//## begin module%38D2274400EF.epilog preserve=yes
//## end module%38D2274400EF.epilog

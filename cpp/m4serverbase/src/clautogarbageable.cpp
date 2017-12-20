//## begin module%3821B13600A5.cm preserve=no
//## end module%3821B13600A5.cm

//## begin module%3821B13600A5.cp preserve=no
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
//## end module%3821B13600A5.cp

//## Module: clautogarbageable%3821B13600A5; Package body
//## Subsystem: M4ServerBase::garbagecollector::src%382154160271
//## Source file: D:\eduardoma\m4serverbase\src\clautogarbageable.cpp

//## begin module%3821B13600A5.additionalIncludes preserve=no
//## end module%3821B13600A5.additionalIncludes

//## begin module%3821B13600A5.includes preserve=yes
//## end module%3821B13600A5.includes

// clautogarbageable
#include <clautogarbageable.hpp>
// garbagecollectorinterface
#include <garbagecollectorinterface.hpp>
// garbageableinterface
#include <garbageableinterface.hpp>
//## begin module%3821B13600A5.declarations preserve=no
//## end module%3821B13600A5.declarations

//## begin module%3821B13600A5.additionalDeclarations preserve=yes
#include <stdio.h>
//## end module%3821B13600A5.additionalDeclarations


// Class ClAutoGarbageable 

//## begin ClAutoGarbageable::m_poBin%3821AC7600D8.role preserve=yes  protected: static IGarbageCollector { -> RHAN}
IGarbageCollector *ClAutoGarbageable::m_poBin=0;
//## end ClAutoGarbageable::m_poBin%3821AC7600D8.role

ClAutoGarbageable::ClAutoGarbageable ()
  //## begin ClAutoGarbageable::ClAutoGarbageable%941721573.hasinit preserve=no
  //## end ClAutoGarbageable::ClAutoGarbageable%941721573.hasinit
  //## begin ClAutoGarbageable::ClAutoGarbageable%941721573.initialization preserve=yes
  //## end ClAutoGarbageable::ClAutoGarbageable%941721573.initialization
{
  //## begin ClAutoGarbageable::ClAutoGarbageable%941721573.body preserve=yes
  //## end ClAutoGarbageable::ClAutoGarbageable%941721573.body
}



//## Other Operations (implementation)
void ClAutoGarbageable::SetBin (IGarbageCollector &ai_oBin)
{
  //## begin ClAutoGarbageable::SetBin%941721574.body preserve=yes
	m_poBin = &ai_oBin ;
  //## end ClAutoGarbageable::SetBin%941721574.body
}

m4return_t ClAutoGarbageable::SendToBin ()
{
  //## begin ClAutoGarbageable::SendToBin%941721575.body preserve=yes
	if ( ! m_poBin )
		return M4_ERROR ;

	m_poBin ->DisposeOf ( *this ) ;
	return M4_SUCCESS ;
  //## end ClAutoGarbageable::SendToBin%941721575.body
}

// Additional Declarations
  //## begin ClAutoGarbageable%3821B0B60000.declarations preserve=yes
  //## end ClAutoGarbageable%3821B0B60000.declarations

//## begin module%3821B13600A5.epilog preserve=yes
//## end module%3821B13600A5.epilog

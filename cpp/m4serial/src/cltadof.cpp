//## begin module%3911AA2902E3.cm preserve=no
//## end module%3911AA2902E3.cm

//## begin module%3911AA2902E3.cp preserve=no
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
//## end module%3911AA2902E3.cp

//## Module: cltadof%3911AA2902E3; Package body
//## Subsystem: M4Serial::src%3911B1010152
//## Source file: d:\new\m4serial\src\cltadof.cpp

//## begin module%3911AA2902E3.additionalIncludes preserve=no
//## end module%3911AA2902E3.additionalIncludes

//## begin module%3911AA2902E3.includes preserve=yes

//==============================================================================
//
// (c) Copyright 1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:                ClTAdOf.cpp
// Project:             Canales
// Author:              Meta Software M.S. , S.A
// Date:                19/01/98
// Language:            C++
// Operating System:    WINDOWS, ??
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    Address to Offset (and viceversa) module
//    This class is used for index the address to offset relation on address or offset (not both)
//
//==============================================================================

#include "cltadof.hpp"
//## end module%3911AA2902E3.includes

// m4types
#include <m4types.hpp>
//## begin module%3911AA2902E3.declarations preserve=no
//## end module%3911AA2902E3.declarations

//## begin module%3911AA2902E3.additionalDeclarations preserve=yes
//## end module%3911AA2902E3.additionalDeclarations


// Class ClTableAddressOffset 






//## begin ClTableAddressOffset::NOT_FOUND%3911AA2800FE.role preserve=no  private: static m4int32_t {1 -> 1VHAPNC}
const m4int32_t  ClTableAddressOffset::NOT_FOUND = -1;
//## end ClTableAddressOffset::NOT_FOUND%3911AA2800FE.role


ClTableAddressOffset::ClTableAddressOffset (m4uint32_t ai_iMode)
  //## begin ClTableAddressOffset::ClTableAddressOffset%672414148.hasinit preserve=no
  //## end ClTableAddressOffset::ClTableAddressOffset%672414148.hasinit
  //## begin ClTableAddressOffset::ClTableAddressOffset%672414148.initialization preserve=yes
  //## end ClTableAddressOffset::ClTableAddressOffset%672414148.initialization
{
  //## begin ClTableAddressOffset::ClTableAddressOffset%672414148.body preserve=yes
   // Por ahora no tomo en cuenta el modo
   m_iMode=ai_iMode;
  //## end ClTableAddressOffset::ClTableAddressOffset%672414148.body
}


ClTableAddressOffset::~ClTableAddressOffset ()
{
  //## begin ClTableAddressOffset::~ClTableAddressOffset%-310045900.body preserve=yes
  //## end ClTableAddressOffset::~ClTableAddressOffset%-310045900.body
}



//## Other Operations (implementation)
m4uint32_t ClTableAddressOffset::GetOffset (void* ai_pPtr)
{
  //## begin ClTableAddressOffset::GetOffset%-1068731964.body preserve=yes
//	M4_ASSERT(m_iMode==INDEX_BY_ADDRESS);

#ifdef _HASH_
	hash_map<void*, m4uint32_t, hash<void*>, equal_to<void*> >::iterator Index;
#endif

#ifndef _HASH_
	map<void*, m4uint32_t, less<void*> >::iterator Index;
#endif

    Index=m_oTableAddress.find(ai_pPtr);
	if (Index==m_oTableAddress.end())
		return 0;
	else
		return (*Index).second;

  //## end ClTableAddressOffset::GetOffset%-1068731964.body
}

void* ClTableAddressOffset::GetAddress (m4uint32_t ai_iOffset)
{
  //## begin ClTableAddressOffset::GetAddress%154562017.body preserve=yes
//	M4_ASSERT(m_iMode==INDEX_BY_OFFSET);

#ifdef _HASH_
	hash_map<m4uint32_t, void*, hash<m4uint32_t>, equal_to<m4uint32_t> >::iterator Index;
#endif

#ifndef _HASH_
	map<m4uint32_t, void*, less<m4uint32_t> >::iterator Index;
#endif
	
    Index=m_oTableOffset.find(ai_iOffset);
	if (Index==m_oTableOffset.end())
		return (void*)0;
	else
		return (*Index).second;
  //## end ClTableAddressOffset::GetAddress%154562017.body
}

m4bool_t ClTableAddressOffset::Add (void* ai_pPtr, const m4uint32_t ai_iOffset)
{
  //## begin ClTableAddressOffset::Add%-1442246846.body preserve=yes
   m4uint32_t Index;
	

   if ((Index=GetOffset(ai_pPtr))!=0)
      return M4_FALSE;

   m_oTableAddress[ai_pPtr]=ai_iOffset;

   return M4_TRUE;
  //## end ClTableAddressOffset::Add%-1442246846.body
}

m4bool_t ClTableAddressOffset::Add (const m4uint32_t ai_iOffset, const void* ai_pPtr)
{
  //## begin ClTableAddressOffset::Add%1162784538.body preserve=yes
   void *Index;

   if ((Index=GetAddress(ai_iOffset))!=(void *)0)
      return M4_FALSE;

   m_oTableOffset[ai_iOffset]=(void*)ai_pPtr;

   return M4_TRUE;
  //## end ClTableAddressOffset::Add%1162784538.body
}

void ClTableAddressOffset::Reset (void )
{
  //## begin ClTableAddressOffset::Reset%834609009.body preserve=yes
#ifdef _HASH_
   hash_map<void*, m4uint32_t, hash<void*>, equal_to<void*> >::iterator IndexAddress;
   hash_map<m4uint32_t, void*, hash<m4uint32_t>, equal_to<m4uint32_t> >::iterator IndexOffset;
#endif

#ifndef _HASH_
   map<void*, m4uint32_t, less<void*> >::iterator IndexAddress;
   map<m4uint32_t, void*, less<m4uint32_t> >::iterator IndexOffset;
#endif

   m_oTableAddress.erase(m_oTableAddress.begin(),m_oTableAddress.end());
   m_oTableOffset.erase(m_oTableOffset.begin(),m_oTableOffset.end());
  //## end ClTableAddressOffset::Reset%834609009.body
}

// Additional Declarations
  //## begin ClTableAddressOffset%3911AA2401F6.declarations preserve=yes
  //## end ClTableAddressOffset%3911AA2401F6.declarations

//## begin module%3911AA2902E3.epilog preserve=yes
//## end module%3911AA2902E3.epilog

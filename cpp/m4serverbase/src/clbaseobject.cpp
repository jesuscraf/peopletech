//## begin module%342240670055.cm preserve=no
//## end module%342240670055.cm

//## begin module%342240670055.cp preserve=no
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
//## end module%342240670055.cp

//## Module: clbaseobject%342240670055; Package body
//## Subsystem: M4ServerBase::src%379431BE02D0
//## Source file: F:\integration\m4serverbase\src\clbaseobject.cpp

//## begin module%342240670055.additionalIncludes preserve=no
//## end module%342240670055.additionalIncludes

//## begin module%342240670055.includes preserve=yes
#include <statistics.hpp>
//## end module%342240670055.includes

// m4types
#include <m4types.hpp>
// clbaseobject
#include <clbaseobject.hpp>
// clautogarbageable
#include <clautogarbageable.hpp>


//## begin module%342240670055.declarations preserve=no
//## end module%342240670055.declarations

//## begin module%342240670055.additionalDeclarations preserve=yes
//## end module%342240670055.additionalDeclarations


// Class ClBaseObject 



ClBaseObject::ClBaseObject (ClStatistic *ai_poStatistic)
  //## begin ClBaseObject::ClBaseObject%890086938.hasinit preserve=no
  //## end ClBaseObject::ClBaseObject%890086938.hasinit
  //## begin ClBaseObject::ClBaseObject%890086938.initialization preserve=yes
  //## end ClBaseObject::ClBaseObject%890086938.initialization
{
  //## begin ClBaseObject::ClBaseObject%890086938.body preserve=yes
	m_poStatistic = ai_poStatistic ;
	m_lId = 0 ;
  //## end ClBaseObject::ClBaseObject%890086938.body
}


ClBaseObject::~ClBaseObject ()
{
  //## begin ClBaseObject::~ClBaseObject%890086939.body preserve=yes
  //## end ClBaseObject::~ClBaseObject%890086939.body
}



//## Other Operations (implementation)
m4objid_t ClBaseObject::GetId ()
{
  //## begin ClBaseObject::GetId%887966446.body preserve=yes
	return m_lId ;
  //## end ClBaseObject::GetId%887966446.body
}

void ClBaseObject::SetId (m4objid_t ai_lId)
{
  //## begin ClBaseObject::SetId%887966447.body preserve=yes
	m_lId = ai_lId ;
  //## end ClBaseObject::SetId%887966447.body
}

ClStatistic * ClBaseObject::GetStats ()
{
  //## begin ClBaseObject::GetStats%890086940.body preserve=yes

		return m_poStatistic ;
		
  //## end ClBaseObject::GetStats%890086940.body
}

// Additional Declarations
  //## begin ClBaseObject%3422401402D5.declarations preserve=yes
  //## end ClBaseObject%3422401402D5.declarations

//## begin module%342240670055.epilog preserve=yes
//## end module%342240670055.epilog

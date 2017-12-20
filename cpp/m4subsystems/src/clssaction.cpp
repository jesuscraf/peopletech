//## begin module%38F20BED00A2.cm preserve=no
//## end module%38F20BED00A2.cm

//## begin module%38F20BED00A2.cp preserve=no
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
//## end module%38F20BED00A2.cp

//## Module: clssaction%38F20BED00A2; Package body
//## Subsystem: M4Subsystems::src%37D3A22003E4
//## Source file: F:\integration\m4subsystems\src\clssaction.cpp

//## begin module%38F20BED00A2.additionalIncludes preserve=no
//## end module%38F20BED00A2.additionalIncludes

//## begin module%38F20BED00A2.includes preserve=yes
//## end module%38F20BED00A2.includes

// clbaseaction
#include <clbaseaction.hpp>
// garbageableinterface
#include <garbageableinterface.hpp>
// clssaction
#include <clssaction.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>
//## begin module%38F20BED00A2.declarations preserve=no
//## end module%38F20BED00A2.declarations

//## begin module%38F20BED00A2.additionalDeclarations preserve=yes
//## end module%38F20BED00A2.additionalDeclarations


// Class ClSSAction 



ClSSAction::ClSSAction (ClSSImplementation *ai_poExecutive, m4bool_t ai_bDeleteParamList)
  //## begin ClSSAction::ClSSAction%955440314.hasinit preserve=no
  //## end ClSSAction::ClSSAction%955440314.hasinit
  //## begin ClSSAction::ClSSAction%955440314.initialization preserve=yes
  :ClBaseAction ( ai_poExecutive , ai_bDeleteParamList )
  //## end ClSSAction::ClSSAction%955440314.initialization
{
  //## begin ClSSAction::ClSSAction%955440314.body preserve=yes
  //## end ClSSAction::ClSSAction%955440314.body
}


// Additional Declarations
  //## begin ClSSAction%38F209F60095.declarations preserve=yes
  //## end ClSSAction%38F209F60095.declarations

//## begin module%38F20BED00A2.epilog preserve=yes
//## end module%38F20BED00A2.epilog

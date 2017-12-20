//## begin module%391ABB58035F.cm preserve=no
//## end module%391ABB58035F.cm

//## begin module%391ABB58035F.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.hpp
//	 Project:            M4ComSrv
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
//	=
//## end module%391ABB58035F.cp

//## Module: iexecutable%391ABB58035F; Package specification
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Source file: F:\integration\m4serverbase\inc\iexecutable.hpp

#ifndef iexecutable_h
#define iexecutable_h 1

//## begin module%391ABB58035F.additionalIncludes preserve=no
//## end module%391ABB58035F.additionalIncludes

//## begin module%391ABB58035F.includes preserve=yes
#include <m4types.hpp>
//## end module%391ABB58035F.includes

// ParamList
#include <paramlist.hpp>
//## begin module%391ABB58035F.declarations preserve=no
//## end module%391ABB58035F.declarations

//## begin module%391ABB58035F.additionalDeclarations preserve=yes
//## end module%391ABB58035F.additionalDeclarations


//## begin ClExecutableInterface%391ABAEA0099.preface preserve=yes
//## end ClExecutableInterface%391ABAEA0099.preface

//## Class: ClExecutableInterface%391ABAEA0099
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%391ABC100150;ClParamList { -> }

class ClExecutableInterface 
{
  //## begin ClExecutableInterface%391ABAEA0099.initialDeclarations preserve=yes
  //## end ClExecutableInterface%391ABAEA0099.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: ExecuteAction%958053242
      virtual m4return_t ExecuteAction (m4pchar_t ai_oActionString, ClParamList *ai_poParamList) = 0;

    // Additional Public Declarations
      //## begin ClExecutableInterface%391ABAEA0099.public preserve=yes
      //## end ClExecutableInterface%391ABAEA0099.public

  protected:
    // Additional Protected Declarations
      //## begin ClExecutableInterface%391ABAEA0099.protected preserve=yes
      //## end ClExecutableInterface%391ABAEA0099.protected

  private:
    // Additional Private Declarations
      //## begin ClExecutableInterface%391ABAEA0099.private preserve=yes
      //## end ClExecutableInterface%391ABAEA0099.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClExecutableInterface%391ABAEA0099.implementation preserve=yes
      //## end ClExecutableInterface%391ABAEA0099.implementation

};

//## begin ClExecutableInterface%391ABAEA0099.postscript preserve=yes
//## end ClExecutableInterface%391ABAEA0099.postscript

// Class ClExecutableInterface 

//## begin module%391ABB58035F.epilog preserve=yes
//## end module%391ABB58035F.epilog


#endif

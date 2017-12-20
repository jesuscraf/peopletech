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
//	=========================================================
//	=====================
//## end module.cp

//## Module: ClCmdArguments; Package specification
//## Subsystem: CPM4ComSrv::version
//## Source file: X:\m4rel\m4source\3x_work\core\communication\working\oscar\M4ComSrv\version\inc\clcmdarguments.hpp

#ifndef clcmdarguments_h
#define clcmdarguments_h 1

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
//## end module.includes
#include "m4stl.hpp"
// m4string
#include <m4string.hpp>
//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes
//## end module.additionalDeclarations


//## Class: ClCmdArguments
//	Class to send arguments to the commands.
//## Category: M4ComSrv::LocalMonitor
//## Subsystem: CPM4ComSrv::version
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>; M4ClString { -> }
//## Uses: <unnamed>; list { -> }

class ClCmdArguments 
{
  //## begin ClCmdArguments.initialDeclarations preserve=yes
  //## end ClCmdArguments.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCmdArguments%563430548; C++
      ClCmdArguments ();

      //## Operation: ClCmdArguments%1589632728; C++
      ClCmdArguments (void* ai_poArgument);

    //## Destructor (specified)
      //## Operation: ~ClCmdArguments%-1098612200; C++
      ~ClCmdArguments ();


    //## Other Operations (specified)
      //## Operation: SetNArgument%1868093859; C++
      //	Add a new numeric argument at the end of the list of
      //	numeric arguments
      void SetNArgument (m4int32_t ai_lArgument);

      //## Operation: SetPArgument%877369599; C++
      //	Add a new pointer argument at the end of the list of
      //	pointer arguments
      void SetPArgument (void* ai_poArgument);

      //## Operation: GetNArgument%875106861
      //	Return the first element in the list of numeric
      //	artguments
      m4int32_t GetNArgument ();

      //## Operation: GetPArgument%877369600
      //	Return the first element in the list of pointer
      //	artguments
      void* GetPArgument ();

      //## Operation: Clear%877369601
      //	Empties the lists of arguments
      void Clear ();

    // Additional Public Declarations
      //## begin ClCmdArguments.public preserve=yes
      //## end ClCmdArguments.public

  protected:
    // Additional Protected Declarations
      //## begin ClCmdArguments.protected preserve=yes
      //## end ClCmdArguments.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_lArguments
      //	A list of numeric values to send
      //## begin ClCmdArguments::m_lArguments.attr preserve=no  private: list < m4int32_t > {UA} 
      list < m4int32_t > m_lArguments;
      //## end ClCmdArguments::m_lArguments.attr

      //## Attribute: m_pArguments
      //	A list of pointer arguments to be passed to the command
      //## begin ClCmdArguments::m_pArguments.attr preserve=no  private: list <void *> {UA} 
      list <void *> m_pArguments;
      //## end ClCmdArguments::m_pArguments.attr

    // Additional Private Declarations
      //## begin ClCmdArguments.private preserve=yes
      //## end ClCmdArguments.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClCmdArguments.implementation preserve=yes
      //## end ClCmdArguments.implementation

};

//## begin ClCmdArguments.postscript preserve=yes
//## end ClCmdArguments.postscript



// Class ClCmdArguments 





//## begin module.epilog preserve=yes
//## end module.epilog
#endif

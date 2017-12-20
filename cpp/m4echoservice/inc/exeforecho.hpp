//## begin module%34E2EAB8004B.cm preserve=no
//## end module%34E2EAB8004B.cm

//## begin module%34E2EAB8004B.cp preserve=no
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
//## end module%34E2EAB8004B.cp

//## Module: ExeForEcho%34E2EAB8004B; Package specification
//## Subsystem: M4ExeFactory::inc%37D781A3038C
//## Source file: d:\eduardoma\M4ExeFactory\version\inc\exeforecho.hpp

#ifndef exeforecho_h
#define exeforecho_h 1

//## begin module%34E2EAB8004B.additionalIncludes preserve=no
//## end module%34E2EAB8004B.additionalIncludes

//## begin module%34E2EAB8004B.includes preserve=yes
//## end module%34E2EAB8004B.includes

// Executive
#include <executive.hpp>
//## begin module%34E2EAB8004B.declarations preserve=no
//## end module%34E2EAB8004B.declarations

//## begin module%34E2EAB8004B.additionalDeclarations preserve=yes
//## end module%34E2EAB8004B.additionalDeclarations


//## begin ClExeForEcho%34DF52F30339.preface preserve=yes
//## end ClExeForEcho%34DF52F30339.preface

//## Class: ClExeForEcho%34DF52F30339
//## Category: M4ExeFactory::Exec  Echo%34DF52CB016F
//## Subsystem: M4ExeFactory::inc%37D781A3038C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%36A61ED00082;ClDataUnit { -> F}
//## Uses: <unnamed>%3744445C00B0;ClLog { -> F}

class ClExeForEcho : public ClExecutive  //## Inherits: <unnamed>%34DF536703B8
{
  //## begin ClExeForEcho%34DF52F30339.initialDeclarations preserve=yes
  //## end ClExeForEcho%34DF52F30339.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClExeForEcho%887109594
      ClExeForEcho (ClEngine *ai_pEngine);

    //## Destructor (specified)
      //## Operation: ~ClExeForEcho%887109595
      ~ClExeForEcho ();


    //## Other Operations (specified)
      //## Operation: StartUp%887109596
      m4return_t  StartUp (ClNavigatorFirewall *ai_pNavigator = NULL, ClConfiguration *ai_pConfig = NULL);

      //## Operation: ShutDown%887109597
      m4return_t  ShutDown ();

      //## Operation: Reset%887109598
      m4return_t Reset ();

      //## Operation: Execute%887109599
      m4return_t Execute (ClImage &aio_pImage);

      //## Operation: GetNDumpLevels%921077501
      m4int16_t GetNDumpLevels () const;

      //## Operation: DumpLevel%921077500; C++
      m4return_t DumpLevel (const m4int16_t ai_iLevel);

    // Additional Public Declarations
      //## begin ClExeForEcho%34DF52F30339.public preserve=yes
      //## end ClExeForEcho%34DF52F30339.public

  protected:
    // Additional Protected Declarations
      //## begin ClExeForEcho%34DF52F30339.protected preserve=yes
      //## end ClExeForEcho%34DF52F30339.protected

  private:
    // Additional Private Declarations
      //## begin ClExeForEcho%34DF52F30339.private preserve=yes
      //## end ClExeForEcho%34DF52F30339.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClExeForEcho%34DF52F30339.implementation preserve=yes
      //## end ClExeForEcho%34DF52F30339.implementation

};

//## begin ClExeForEcho%34DF52F30339.postscript preserve=yes
//## end ClExeForEcho%34DF52F30339.postscript

// Class ClExeForEcho 


//## Other Operations (inline)
inline m4int16_t ClExeForEcho::GetNDumpLevels () const
{
  //## begin ClExeForEcho::GetNDumpLevels%921077501.body preserve=yes
	return 0;
  //## end ClExeForEcho::GetNDumpLevels%921077501.body
}

//## begin module%34E2EAB8004B.epilog preserve=yes
//## end module%34E2EAB8004B.epilog


#endif

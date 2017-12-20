//## begin module%3993A3AE0394.cm preserve=no
//## end module%3993A3AE0394.cm

//## begin module%3993A3AE0394.cp preserve=no
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
//## end module%3993A3AE0394.cp

//## Module: connects40%3993A3AE0394; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\inc\connects40.hpp

#ifndef connects40_h
#define connects40_h 1

//## begin module%3993A3AE0394.additionalIncludes preserve=no
//## end module%3993A3AE0394.additionalIncludes

//## begin module%3993A3AE0394.includes preserve=yes
#include <m4string.hpp>
//## end module%3993A3AE0394.includes

// conninstance
#include <conninstance.hpp>

class ClConnection;

//## begin module%3993A3AE0394.declarations preserve=no
//## end module%3993A3AE0394.declarations

//## begin module%3993A3AE0394.additionalDeclarations preserve=yes
//## end module%3993A3AE0394.additionalDeclarations


//## begin ClConnection40%3992D587023E.preface preserve=yes
//## end ClConnection40%3992D587023E.preface

//## Class: ClConnection40%3992D587023E
//## Category: Dictionary%34183E8601C0
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39AA296B0057;ClConnection { -> F}

class ClConnection40 : public ClConnInstance  //## Inherits: <unnamed>%39A53CA100AA
{
  //## begin ClConnection40%3992D587023E.initialDeclarations preserve=yes
  //## end ClConnection40%3992D587023E.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClConnection40%965977089
      ClConnection40 (m4uint32_t ai_lID);

      //## Operation: ClConnection40%967450713
      ClConnection40 (ClConnection *ai_poConnection);


    //## Other Operations (specified)
      //## Operation: CanDestroy%965977076
      //	Returns M4_TRUE if the object may be destroyed, M4_FALSE otherwise.
      virtual m4bool_t CanDestroy ();

      //## Operation: CanRecycle%965977077
      //	Returns M4_TRUE if the object can be recycled, M4_FALSE otherwise.
      virtual m4bool_t CanRecycle ();

      //## Operation: Destroy%965977078
      //	Returns M4_ERROR if CanDestroy() != M4_TRUE, M4_SUCCESS if Destroy is
      //	completed. Other return Values may be defined.
      virtual m4return_t Destroy ();

      //## Operation: Recycle%965977081; C++
      //	Returns M4_ERROR if CanRecycle() != M4_TRUE, M4_SUCCESS if Recycle is
      //	completed. Other return values may be defined.
      virtual m4return_t Recycle ();

      //## Operation: SetStatus%967450719
      m4return_t SetStatus (m4uint32_t ai_iValue);

    // Additional Public Declarations
      //## begin ClConnection40%3992D587023E.public preserve=yes
      ~ClConnection40 ();
      //## end ClConnection40%3992D587023E.public

  protected:
    // Additional Protected Declarations
      //## begin ClConnection40%3992D587023E.protected preserve=yes
      //## end ClConnection40%3992D587023E.protected

  private:
    //## Constructors (specified)
      //## Operation: ClConnection40%965977090
      ClConnection40 ();

    // Additional Private Declarations
      //## begin ClConnection40%3992D587023E.private preserve=yes
      //## end ClConnection40%3992D587023E.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClConnection40%3992D587023E.implementation preserve=yes
      //## end ClConnection40%3992D587023E.implementation

};

//## begin ClConnection40%3992D587023E.postscript preserve=yes
//## end ClConnection40%3992D587023E.postscript

// Class ClConnection40 

//## begin module%3993A3AE0394.epilog preserve=yes
//## end module%3993A3AE0394.epilog


#endif

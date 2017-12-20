//## begin module%37E6033E003D.cm preserve=no
//## end module%37E6033E003D.cm

//## begin module%37E6033E003D.cp preserve=no
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
//## end module%37E6033E003D.cp

//## Module: jsjobbuilderfromm4object%37E6033E003D; Package specification
//## Subsystem: M4JSRecover::inc%37F8B6990107
//## Source file: C:\Trabajo\Server\m4jsrecover\version\inc\jsjobbuilderfromm4object.hpp

#ifndef jsjobbuilderfromm4object_h
#define jsjobbuilderfromm4object_h 1

//## begin module%37E6033E003D.additionalIncludes preserve=no
//## end module%37E6033E003D.additionalIncludes

#include <m4jsrecover_dll.hpp>
// jsjobbuilder
#include <jsjobbuilder.hpp>

class ClAccess;

//## begin module%37E6033E003D.declarations preserve=no
//## end module%37E6033E003D.declarations

//## begin module%37E6033E003D.additionalDeclarations preserve=yes
//## end module%37E6033E003D.additionalDeclarations


//## begin ClJSJobBuilderFromM4Object%3773A3520184.preface preserve=yes
//## end ClJSJobBuilderFromM4Object%3773A3520184.preface

//## Class: ClJSJobBuilderFromM4Object%3773A3520184
//## Category: M4JSRecover%37F8B5B70134
//## Subsystem: M4JSRecover::inc%37F8B6990107
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4JSRECOVER ClJSJobBuilderFromM4Object : public ClJSJobBuilder  //## Inherits: <unnamed>%3773A372014E
{
  //## begin ClJSJobBuilderFromM4Object%3773A3520184.initialDeclarations preserve=yes
  //## end ClJSJobBuilderFromM4Object%3773A3520184.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClJSJobBuilderFromM4Object%938073478
      ClJSJobBuilderFromM4Object ();


    //## Other Operations (specified)
      //## Operation: SetSource%938073479
      m4return_t SetSource (ClAccess *ai_poSource = NULL);

      //## Operation: ReadScheduledJobs%941096725
      virtual m4return_t ReadScheduledJobs (m4uint_t &ai_uiMaxNumJobs);

      //## Operation: ReadCancelledJobs%941096726
      virtual m4return_t ReadCancelledJobs ();

    // Additional Public Declarations
      //## begin ClJSJobBuilderFromM4Object%3773A3520184.public preserve=yes
      //## end ClJSJobBuilderFromM4Object%3773A3520184.public

  protected:
    // Data Members for Associations

      //## Association: M4JSRecover::<unnamed>%3807033102E2
      //## Role: ClJSJobBuilderFromM4Object::m_poSource%38070332015D
      //## begin ClJSJobBuilderFromM4Object::m_poSource%38070332015D.role preserve=no  protected: ClAccess { -> 1RFHAN}
      ClAccess *m_poSource;
      //## end ClJSJobBuilderFromM4Object::m_poSource%38070332015D.role

    // Additional Protected Declarations
      //## begin ClJSJobBuilderFromM4Object%3773A3520184.protected preserve=yes
      //## end ClJSJobBuilderFromM4Object%3773A3520184.protected

  private:

    //## Other Operations (specified)
      //## Operation: BuildJobs%939977802
      m4return_t BuildJobs (m4bool_t ai_bBuildCancellations = M4_FALSE);

      //## Operation: BuildJobStructure%940243353
      m4return_t BuildJobStructure (ClJSJob *ai_pMainJob);

      //## Operation: BuildJobParameters%941187550
      m4return_t BuildJobParameters (ClJSJob *ai_pJob);

      //## Operation: BuildParameterConnectors%941187551
      m4return_t BuildParameterConnectors (ClJSJob *ai_pMainJob);

      //## Operation: InitializeScheduledJobsNode%941120255
      m4return_t InitializeScheduledJobsNode (m4uint_t &ai_uiMaxNumJobs);

      //## Operation: InitializeCancelledJobsNode%941120256
      m4return_t InitializeCancelledJobsNode ();

    // Additional Private Declarations
      //## begin ClJSJobBuilderFromM4Object%3773A3520184.private preserve=yes
      //## end ClJSJobBuilderFromM4Object%3773A3520184.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClJSJobBuilderFromM4Object%3773A3520184.implementation preserve=yes
      //## end ClJSJobBuilderFromM4Object%3773A3520184.implementation

};

//## begin ClJSJobBuilderFromM4Object%3773A3520184.postscript preserve=yes
//## end ClJSJobBuilderFromM4Object%3773A3520184.postscript

// Class ClJSJobBuilderFromM4Object 

//## begin module%37E6033E003D.epilog preserve=yes
//## end module%37E6033E003D.epilog


#endif

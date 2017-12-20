//## begin module%37F38E370212.cm preserve=no
//## end module%37F38E370212.cm

//## begin module%37F38E370212.cp preserve=no
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
//## end module%37F38E370212.cp

//## Module: jsexecutionstrategy%37F38E370212; Package specification
//## Subsystem: M4JSExeJob::m4jsjob::inc%37E244DF010A
//## Source file: C:\Trabajo\Server\m4jsexejob\version\inc\jsexecutionstrategy.hpp

#ifndef jsexecutionstrategy_h
#define jsexecutionstrategy_h 1

//## begin module%37F38E370212.additionalIncludes preserve=no
//## end module%37F38E370212.additionalIncludes

//## begin module%37F38E370212.includes preserve=yes
#include "m4types.hpp"
//## end module%37F38E370212.includes

// m4jsjob_dll
#include <m4jsexejob_dll.hpp>
//## begin module%37F38E370212.declarations preserve=no
//## end module%37F38E370212.declarations

//## begin module%37F38E370212.additionalDeclarations preserve=yes
//## end module%37F38E370212.additionalDeclarations


//## begin ClJSExecutionStrategy%37F3401802A1.preface preserve=yes
class ClJSJob;
//## end ClJSExecutionStrategy%37F3401802A1.preface

//## Class: ClJSExecutionStrategy%37F3401802A1
//## Category: M4JSExeJob::M4JSJob%379F03BD0354
//## Subsystem: M4JSExeJob::m4jsjob::inc%37E244DF010A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4JSEXEJOB ClJSExecutionStrategy 
{
  //## begin ClJSExecutionStrategy%37F3401802A1.initialDeclarations preserve=yes
  //## end ClJSExecutionStrategy%37F3401802A1.initialDeclarations

  public:
    //## begin ClJSExecutionStrategy::eStrategyType%37F47EE303CC.preface preserve=yes
    //## end ClJSExecutionStrategy::eStrategyType%37F47EE303CC.preface

    //## Class: eStrategyType%37F47EE303CC
    //## Category: M4JSExeJob::M4JSJob%379F03BD0354
    //## Subsystem: M4JSExeJob::m4jsjob%37E244C503BF
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { LN4SimpleStrategy, AdminSimpleStrategy, InitialCompoundStrategy, FinalCompoundStrategy } eStrategyType;

    //## begin ClJSExecutionStrategy::eStrategyType%37F47EE303CC.postscript preserve=yes
    //## end ClJSExecutionStrategy::eStrategyType%37F47EE303CC.postscript


    //## Other Operations (specified)
      //## Operation: GetStrategyType%938709377
      static ClJSExecutionStrategy * GetStrategyType (eStrategyType uiStrategyType);

      //## Operation: Execute%938766912
      virtual m4return_t Execute (ClJSJob *ui_pJob) = 0;

  public:
    // Additional Public Declarations
      //## begin ClJSExecutionStrategy%37F3401802A1.public preserve=yes
      //## end ClJSExecutionStrategy%37F3401802A1.public

  protected:
    // Additional Protected Declarations
      //## begin ClJSExecutionStrategy%37F3401802A1.protected preserve=yes
      //## end ClJSExecutionStrategy%37F3401802A1.protected

  private:
    // Additional Private Declarations
      //## begin ClJSExecutionStrategy%37F3401802A1.private preserve=yes
      //## end ClJSExecutionStrategy%37F3401802A1.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClJSExecutionStrategy%37F3401802A1.implementation preserve=yes
      //## end ClJSExecutionStrategy%37F3401802A1.implementation

};

//## begin ClJSExecutionStrategy%37F3401802A1.postscript preserve=yes
//## end ClJSExecutionStrategy%37F3401802A1.postscript

//## begin ClJSLN4SimpleExecStrategy%37F340AA01C4.preface preserve=yes
//## end ClJSLN4SimpleExecStrategy%37F340AA01C4.preface

//## Class: ClJSLN4SimpleExecStrategy%37F340AA01C4
//## Category: M4JSExeJob::M4JSJob%379F03BD0354
//## Subsystem: M4JSExeJob::m4jsjob::inc%37E244DF010A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4JSEXEJOB ClJSLN4SimpleExecStrategy : public ClJSExecutionStrategy  //## Inherits: <unnamed>%37F3412C03DE
{
  //## begin ClJSLN4SimpleExecStrategy%37F340AA01C4.initialDeclarations preserve=yes
  //## end ClJSLN4SimpleExecStrategy%37F340AA01C4.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: Instance%938709384
      static ClJSLN4SimpleExecStrategy * Instance ();

      //## Operation: Execute%938766908
      virtual m4return_t Execute (ClJSJob *ai_pJob);

      //## Operation: DestroyInstance%938766918
      static void DestroyInstance ();

    // Additional Public Declarations
      //## begin ClJSLN4SimpleExecStrategy%37F340AA01C4.public preserve=yes
      //## end ClJSLN4SimpleExecStrategy%37F340AA01C4.public

  protected:
    //## Constructors (specified)
      //## Operation: ClJSLN4SimpleExecStrategy%938709385
      ClJSLN4SimpleExecStrategy ();

    // Additional Protected Declarations
      //## begin ClJSLN4SimpleExecStrategy%37F340AA01C4.protected preserve=yes
      //## end ClJSLN4SimpleExecStrategy%37F340AA01C4.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: s_pInstance%37F38B8802F0
      //## begin ClJSLN4SimpleExecStrategy::s_pInstance%37F38B8802F0.attr preserve=no  private: static ClJSLN4SimpleExecStrategy {RA} NULL
      static ClJSLN4SimpleExecStrategy *s_pInstance;
      //## end ClJSLN4SimpleExecStrategy::s_pInstance%37F38B8802F0.attr

    // Additional Private Declarations
      //## begin ClJSLN4SimpleExecStrategy%37F340AA01C4.private preserve=yes
      //## end ClJSLN4SimpleExecStrategy%37F340AA01C4.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClJSLN4SimpleExecStrategy%37F340AA01C4.implementation preserve=yes
      //## end ClJSLN4SimpleExecStrategy%37F340AA01C4.implementation

};

//## begin ClJSLN4SimpleExecStrategy%37F340AA01C4.postscript preserve=yes
//## end ClJSLN4SimpleExecStrategy%37F340AA01C4.postscript

//## begin ClJSAdminSimpleExecStrategy%37F340C201D3.preface preserve=yes
//## end ClJSAdminSimpleExecStrategy%37F340C201D3.preface

//## Class: ClJSAdminSimpleExecStrategy%37F340C201D3
//## Category: M4JSExeJob::M4JSJob%379F03BD0354
//## Subsystem: M4JSExeJob::m4jsjob::inc%37E244DF010A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4JSEXEJOB ClJSAdminSimpleExecStrategy : public ClJSExecutionStrategy  //## Inherits: <unnamed>%37F34130007D
{
  //## begin ClJSAdminSimpleExecStrategy%37F340C201D3.initialDeclarations preserve=yes
  //## end ClJSAdminSimpleExecStrategy%37F340C201D3.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: Instance%938709382
      static ClJSAdminSimpleExecStrategy * Instance ();

      //## Operation: Execute%938766909
      virtual m4return_t Execute (ClJSJob *ai_pJob);

      //## Operation: DestroyInstance%938766919
      static void DestroyInstance ();

    // Additional Public Declarations
      //## begin ClJSAdminSimpleExecStrategy%37F340C201D3.public preserve=yes
      //## end ClJSAdminSimpleExecStrategy%37F340C201D3.public

  protected:
    //## Constructors (specified)
      //## Operation: ClJSAdminSimpleExecStrategy%938709383
      ClJSAdminSimpleExecStrategy ();

    // Additional Protected Declarations
      //## begin ClJSAdminSimpleExecStrategy%37F340C201D3.protected preserve=yes
      //## end ClJSAdminSimpleExecStrategy%37F340C201D3.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: s_pInstance%37F38F00017B
      //## begin ClJSAdminSimpleExecStrategy::s_pInstance%37F38F00017B.attr preserve=no  private: static ClJSAdminSimpleExecStrategy {RA} NULL
      static ClJSAdminSimpleExecStrategy *s_pInstance;
      //## end ClJSAdminSimpleExecStrategy::s_pInstance%37F38F00017B.attr

    // Additional Private Declarations
      //## begin ClJSAdminSimpleExecStrategy%37F340C201D3.private preserve=yes
      //## end ClJSAdminSimpleExecStrategy%37F340C201D3.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClJSAdminSimpleExecStrategy%37F340C201D3.implementation preserve=yes
      //## end ClJSAdminSimpleExecStrategy%37F340C201D3.implementation

};

//## begin ClJSAdminSimpleExecStrategy%37F340C201D3.postscript preserve=yes
//## end ClJSAdminSimpleExecStrategy%37F340C201D3.postscript

//## begin ClJSInitialCompoundExecStrategy%37F340D203C1.preface preserve=yes
//## end ClJSInitialCompoundExecStrategy%37F340D203C1.preface

//## Class: ClJSInitialCompoundExecStrategy%37F340D203C1
//## Category: M4JSExeJob::M4JSJob%379F03BD0354
//## Subsystem: M4JSExeJob::m4jsjob::inc%37E244DF010A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4JSEXEJOB ClJSInitialCompoundExecStrategy : public ClJSExecutionStrategy  //## Inherits: <unnamed>%37F341340000
{
  //## begin ClJSInitialCompoundExecStrategy%37F340D203C1.initialDeclarations preserve=yes
  //## end ClJSInitialCompoundExecStrategy%37F340D203C1.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: Instance%938709380
      static ClJSInitialCompoundExecStrategy * Instance ();

      //## Operation: Execute%938766910
      virtual m4return_t Execute (ClJSJob *ai_pJob);

      //## Operation: DestroyInstance%938766920
      static void DestroyInstance ();

    // Additional Public Declarations
      //## begin ClJSInitialCompoundExecStrategy%37F340D203C1.public preserve=yes
      //## end ClJSInitialCompoundExecStrategy%37F340D203C1.public

  protected:
    //## Constructors (specified)
      //## Operation: ClJSInitialCompoundExecStrategy%938709381
      ClJSInitialCompoundExecStrategy ();

    // Additional Protected Declarations
      //## begin ClJSInitialCompoundExecStrategy%37F340D203C1.protected preserve=yes
      //## end ClJSInitialCompoundExecStrategy%37F340D203C1.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: s_pInstance%37F38FD500EB
      //## begin ClJSInitialCompoundExecStrategy::s_pInstance%37F38FD500EB.attr preserve=no  private: static ClJSInitialCompoundExecStrategy {RA} 
      static ClJSInitialCompoundExecStrategy *s_pInstance;
      //## end ClJSInitialCompoundExecStrategy::s_pInstance%37F38FD500EB.attr

    // Additional Private Declarations
      //## begin ClJSInitialCompoundExecStrategy%37F340D203C1.private preserve=yes
      //## end ClJSInitialCompoundExecStrategy%37F340D203C1.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClJSInitialCompoundExecStrategy%37F340D203C1.implementation preserve=yes
      //## end ClJSInitialCompoundExecStrategy%37F340D203C1.implementation

};

//## begin ClJSInitialCompoundExecStrategy%37F340D203C1.postscript preserve=yes
//## end ClJSInitialCompoundExecStrategy%37F340D203C1.postscript

//## begin ClJSFinalCompoundExecStrategy%37F340FE0298.preface preserve=yes
//## end ClJSFinalCompoundExecStrategy%37F340FE0298.preface

//## Class: ClJSFinalCompoundExecStrategy%37F340FE0298
//## Category: M4JSExeJob::M4JSJob%379F03BD0354
//## Subsystem: M4JSExeJob::m4jsjob::inc%37E244DF010A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4JSEXEJOB ClJSFinalCompoundExecStrategy : public ClJSExecutionStrategy  //## Inherits: <unnamed>%37F34136032E
{
  //## begin ClJSFinalCompoundExecStrategy%37F340FE0298.initialDeclarations preserve=yes
  //## end ClJSFinalCompoundExecStrategy%37F340FE0298.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: Instance%938709378
      static ClJSFinalCompoundExecStrategy * Instance ();

      //## Operation: Execute%938766911
      virtual m4return_t Execute (ClJSJob *ai_pJob);

      //## Operation: DestroyInstance%938766921
      static void DestroyInstance ();

    // Additional Public Declarations
      //## begin ClJSFinalCompoundExecStrategy%37F340FE0298.public preserve=yes
      //## end ClJSFinalCompoundExecStrategy%37F340FE0298.public

  protected:
    //## Constructors (specified)
      //## Operation: ClJSFinalCompoundExecStrategy%938709379
      ClJSFinalCompoundExecStrategy ();

    // Additional Protected Declarations
      //## begin ClJSFinalCompoundExecStrategy%37F340FE0298.protected preserve=yes
      //## end ClJSFinalCompoundExecStrategy%37F340FE0298.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: s_pInstance%37F390E70211
      //## begin ClJSFinalCompoundExecStrategy::s_pInstance%37F390E70211.attr preserve=no  private: static ClJSFinalCompoundExecStrategy {RA} NULL
      static ClJSFinalCompoundExecStrategy *s_pInstance;
      //## end ClJSFinalCompoundExecStrategy::s_pInstance%37F390E70211.attr

    // Additional Private Declarations
      //## begin ClJSFinalCompoundExecStrategy%37F340FE0298.private preserve=yes
      //## end ClJSFinalCompoundExecStrategy%37F340FE0298.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClJSFinalCompoundExecStrategy%37F340FE0298.implementation preserve=yes
      //## end ClJSFinalCompoundExecStrategy%37F340FE0298.implementation

};

//## begin ClJSFinalCompoundExecStrategy%37F340FE0298.postscript preserve=yes
//## end ClJSFinalCompoundExecStrategy%37F340FE0298.postscript

// Class ClJSExecutionStrategy 

// Class ClJSLN4SimpleExecStrategy 

// Class ClJSAdminSimpleExecStrategy 

// Class ClJSInitialCompoundExecStrategy 

// Class ClJSFinalCompoundExecStrategy 

//## begin module%37F38E370212.epilog preserve=yes
//## end module%37F38E370212.epilog


#endif

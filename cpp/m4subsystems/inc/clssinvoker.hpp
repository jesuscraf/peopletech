//## begin module%38F2108E016E.cm preserve=no
//## end module%38F2108E016E.cm

//## begin module%38F2108E016E.cp preserve=no
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
//## end module%38F2108E016E.cp

//## Module: clssinvoker%38F2108E016E; Package specification
//## Subsystem: M4Subsystems::inc%37D3A21301DC
//## Source file: F:\integration\m4subsystems\inc\clssinvoker.hpp

#ifndef clssinvoker_h
#define clssinvoker_h 1

//## begin module%38F2108E016E.additionalIncludes preserve=no
//## end module%38F2108E016E.additionalIncludes

//## begin module%38F2108E016E.includes preserve=yes
#include <m4subsystems_dll.hpp>
#include <m4types.hpp>
//## end module%38F2108E016E.includes

// actionworker
#include <actionworker.hpp>
// actioninvoker
#include <actioninvoker.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>
//## begin module%38F2108E016E.declarations preserve=no
//## end module%38F2108E016E.declarations

//## begin module%38F2108E016E.additionalDeclarations preserve=yes


const m4uint32_t MAX_NUM_ACTION_PARAMS		=	100 ;
//## end module%38F2108E016E.additionalDeclarations


//## begin ClSSInvokerAbstract%38F20F580263.preface preserve=yes
//## end ClSSInvokerAbstract%38F20F580263.preface

//## Class: ClSSInvokerAbstract%38F20F580263
//## Category: M4Subsystems%37D39FC60059
//## Subsystem: M4Subsystems::inc%37D3A21301DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3918368B0100;ClSSImplementation { -> }
//## Uses: <unnamed>%391836C80374;ClActionInvoker { -> }

typedef ClInvokerCacheParams <ClSSImplementation*> ClSSInvokerAbstract;

//## begin ClSSInvokerAbstract%38F20F580263.postscript preserve=yes
//## end ClSSInvokerAbstract%38F20F580263.postscript

//## begin ClSSInvoker%391923800213.preface preserve=yes
//## end ClSSInvoker%391923800213.preface

//## Class: ClSSInvoker%391923800213
//## Category: M4Subsystems%37D39FC60059
//## Subsystem: M4Subsystems::inc%37D3A21301DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SUBSYSTEMS ClSSInvoker : public ClSSInvokerAbstract  //## Inherits: <unnamed>%391923900252
{
  //## begin ClSSInvoker%391923800213.initialDeclarations preserve=yes
  //## end ClSSInvoker%391923800213.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GetInstance%957943667
      static ClSSInvoker * GetInstance ();

    // Additional Public Declarations
      //## begin ClSSInvoker%391923800213.public preserve=yes
      //## end ClSSInvoker%391923800213.public

  protected:
    // Data Members for Associations

      //## Association: M4Subsystems::<unnamed>%391923D202F8
      //## Role: ClSSInvoker::m_poInstance%391923D401EC
      //## begin ClSSInvoker::m_poInstance%391923D401EC.role preserve=no  protected: static ClSSInvoker { -> RHAN}
      static ClSSInvoker *m_poInstance;
      //## end ClSSInvoker::m_poInstance%391923D401EC.role

    // Additional Protected Declarations
      //## begin ClSSInvoker%391923800213.protected preserve=yes
      //## end ClSSInvoker%391923800213.protected

  private:
    // Additional Private Declarations
      //## begin ClSSInvoker%391923800213.private preserve=yes
      //## end ClSSInvoker%391923800213.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSSInvoker%391923800213.implementation preserve=yes
      //## end ClSSInvoker%391923800213.implementation

};

//## begin ClSSInvoker%391923800213.postscript preserve=yes
//## end ClSSInvoker%391923800213.postscript

//## begin ClSSActionWorker%3A24F710003A.preface preserve=yes
//## end ClSSActionWorker%3A24F710003A.preface

//## Class: ClSSActionWorker%3A24F710003A
//## Category: M4Subsystems%37D39FC60059
//## Subsystem: M4Subsystems::inc%37D3A21301DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SUBSYSTEMS ClSSActionWorker : public ClActionWorker  //## Inherits: <unnamed>%3A24F71C025E
{
  //## begin ClSSActionWorker%3A24F710003A.initialDeclarations preserve=yes
  //## end ClSSActionWorker%3A24F710003A.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSSActionWorker%975501556
      ClSSActionWorker (ClActionsQ &ai_poActionsQ, ClActionsQ *ai_poActionsRecycleBin);


    //## Other Operations (specified)
      //## Operation: ExecuteAction%975501557
      void ExecuteAction (ClBaseAction &ai_oAction);

    // Additional Public Declarations
      //## begin ClSSActionWorker%3A24F710003A.public preserve=yes
      //## end ClSSActionWorker%3A24F710003A.public

  protected:
    // Data Members for Associations

      //## Association: M4Subsystems::<unnamed>%3A24FA81000F
      //## Role: ClSSActionWorker::m_poRecycleBin%3A24FA840399
      //## begin ClSSActionWorker::m_poRecycleBin%3A24FA840399.role preserve=no  protected: ClActionsQ { -> RHAN}
      ClActionsQ *m_poRecycleBin;
      //## end ClSSActionWorker::m_poRecycleBin%3A24FA840399.role

    // Additional Protected Declarations
      //## begin ClSSActionWorker%3A24F710003A.protected preserve=yes
      //## end ClSSActionWorker%3A24F710003A.protected

  private:
    // Additional Private Declarations
      //## begin ClSSActionWorker%3A24F710003A.private preserve=yes
      //## end ClSSActionWorker%3A24F710003A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSSActionWorker%3A24F710003A.implementation preserve=yes
      //## end ClSSActionWorker%3A24F710003A.implementation

};

//## begin ClSSActionWorker%3A24F710003A.postscript preserve=yes
//## end ClSSActionWorker%3A24F710003A.postscript

// Class ClSSInvoker 

// Class ClSSActionWorker 

//## begin module%38F2108E016E.epilog preserve=yes
//## end module%38F2108E016E.epilog


#endif

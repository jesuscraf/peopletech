//## begin module%34929EA5009D.cm preserve=no
//## end module%34929EA5009D.cm

//## begin module%34929EA5009D.cp preserve=no
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
//## end module%34929EA5009D.cp

//## Module: ConnBase%34929EA5009D; Package specification
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Source file: F:\integration\m4serverbase\inc\connbase.hpp

#ifndef connbase_h
#define connbase_h 1

//## begin module%34929EA5009D.additionalIncludes preserve=no
#include <m4serverbase_dll.hpp>
//## end module%34929EA5009D.additionalIncludes

//## begin module%34929EA5009D.includes preserve=yes
//## end module%34929EA5009D.includes

// conninstance
#include <conninstance.hpp>
//## begin module%34929EA5009D.declarations preserve=no
//## end module%34929EA5009D.declarations

//## begin module%34929EA5009D.additionalDeclarations preserve=yes
class ClTSAP ;
//## end module%34929EA5009D.additionalDeclarations


//## begin ClConnBase%34929E7900FE.preface preserve=yes
//## end ClConnBase%34929E7900FE.preface

//## Class: ClConnBase%34929E7900FE
//	Base class for connection class. This class will be used by many components
//	as the client so it contain the comon parameters that all they need.
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERVERBASE ClConnBase : public ClConnInstance  //## Inherits: <unnamed>%3833E83E01F8
{
  //## begin ClConnBase%34929E7900FE.initialDeclarations preserve=yes
  //## end ClConnBase%34929E7900FE.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClConnBase%882023677
      //	Constructor that takes a TSAp as entry parameter
      ClConnBase (m4objid_t ai_lIdConnection, ClTSAP *ai_pTSAP);

      //## Operation: ClConnBase%882023680
      //	Default constructor
      ClConnBase (m4uint32_t ai_lIdConnection);

	  ClConnBase (ClConnInstance *ai_poConnInst);

    //## Destructor (specified)
      //## Operation: ~ClConnBase%902904500
      ~ClConnBase ();


    //## Other Operations (specified)
      //## Operation: GetProtocol%884258328
      ClProtocol GetProtocol ();

    // Data Members for Associations

      //## Association: M4ServerBase::<unnamed>%39A53E8801FF
      //## Role: ClConnBase::m_poTSAP%39A53E890193
      //## begin ClConnBase::m_poTSAP%39A53E890193.role preserve=no  public: ClTSAP { -> RHAN}
      ClTSAP *m_poTSAP;
      //## end ClConnBase::m_poTSAP%39A53E890193.role

    // Additional Public Declarations
      //## begin ClConnBase%34929E7900FE.public preserve=yes
      //## end ClConnBase%34929E7900FE.public

  protected:
    // Additional Protected Declarations
      //## begin ClConnBase%34929E7900FE.protected preserve=yes
      //## end ClConnBase%34929E7900FE.protected

  private:
    // Additional Private Declarations
      //## begin ClConnBase%34929E7900FE.private preserve=yes
      //## end ClConnBase%34929E7900FE.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClConnBase%34929E7900FE.implementation preserve=yes
      //## end ClConnBase%34929E7900FE.implementation

};

//## begin ClConnBase%34929E7900FE.postscript preserve=yes
//## end ClConnBase%34929E7900FE.postscript

// Class ClConnBase 

//## begin module%34929EA5009D.epilog preserve=yes
//## end module%34929EA5009D.epilog


#endif

//## begin module%37E602CA0362.cm preserve=no
//## end module%37E602CA0362.cm

//## begin module%37E602CA0362.cp preserve=no
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
//## end module%37E602CA0362.cp

//## Module: jsparameterconnector%37E602CA0362; Package specification
//## Subsystem: M4JSExeJob::m4jsjob::inc%37E244DF010A
//## Source file: C:\Trabajo\Server\m4jsexejob\version\inc\jsparameterconnector.hpp

#ifndef jsparameterconnector_h
#define jsparameterconnector_h 1

//## begin module%37E602CA0362.additionalIncludes preserve=no
//## end module%37E602CA0362.additionalIncludes

//## begin module%37E602CA0362.includes preserve=yes
#include "m4types.hpp"
//## end module%37E602CA0362.includes

// jsparameter
#include <jsparameter.hpp>
//## begin module%37E602CA0362.declarations preserve=no
//## end module%37E602CA0362.declarations

//## begin module%37E602CA0362.additionalDeclarations preserve=yes
//## end module%37E602CA0362.additionalDeclarations


//## begin ClJSParameterConnector%375BE8C903CE.preface preserve=yes
//## end ClJSParameterConnector%375BE8C903CE.preface

//## Class: ClJSParameterConnector%375BE8C903CE
//## Category: M4JSExeJob::M4JSJob%379F03BD0354
//## Subsystem: M4JSExeJob::m4jsjob::inc%37E244DF010A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClJSParameterConnector 
{
  //## begin ClJSParameterConnector%375BE8C903CE.initialDeclarations preserve=yes
  //## end ClJSParameterConnector%375BE8C903CE.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClJSParameterConnector%933158857
      ClJSParameterConnector (ClJSParameter *ai_poOrigin, ClJSParameter *ai_poDestination);


    //## Other Operations (specified)
      //## Operation: PropagateValue%933158860
      m4return_t PropagateValue ();

    // Additional Public Declarations
      //## begin ClJSParameterConnector%375BE8C903CE.public preserve=yes
      //## end ClJSParameterConnector%375BE8C903CE.public

  protected:
    // Data Members for Associations

      //## Association: M4JSExeJob::M4JSJob::<unnamed>%37E909150091
      //## Role: ClJSParameterConnector::m_poOrigin%37E909160128
      //## begin ClJSParameterConnector::m_poOrigin%37E909160128.role preserve=no  protected: ClJSParameter { -> RHAN}
      ClJSParameter *m_poOrigin;
      //## end ClJSParameterConnector::m_poOrigin%37E909160128.role

      //## Association: M4JSExeJob::M4JSJob::<unnamed>%37E9095C01F1
      //## Role: ClJSParameterConnector::m_poDestination%37E9095D0102
      //## begin ClJSParameterConnector::m_poDestination%37E9095D0102.role preserve=no  protected: ClJSParameter { -> RHAN}
      ClJSParameter *m_poDestination;
      //## end ClJSParameterConnector::m_poDestination%37E9095D0102.role

    // Additional Protected Declarations
      //## begin ClJSParameterConnector%375BE8C903CE.protected preserve=yes
      //## end ClJSParameterConnector%375BE8C903CE.protected

  private:
    // Additional Private Declarations
      //## begin ClJSParameterConnector%375BE8C903CE.private preserve=yes
      //## end ClJSParameterConnector%375BE8C903CE.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClJSParameterConnector%375BE8C903CE.implementation preserve=yes
      //## end ClJSParameterConnector%375BE8C903CE.implementation

};

//## begin ClJSParameterConnector%375BE8C903CE.postscript preserve=yes
//## end ClJSParameterConnector%375BE8C903CE.postscript

// Class ClJSParameterConnector 

//## begin module%37E602CA0362.epilog preserve=yes
//## end module%37E602CA0362.epilog


#endif

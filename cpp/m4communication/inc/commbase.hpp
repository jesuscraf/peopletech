//## begin module%3B4971A80294.cm preserve=no
//## end module%3B4971A80294.cm

//## begin module%3B4971A80294.cp preserve=no
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
//## end module%3B4971A80294.cp

//## Module: commbase%3B4971A80294; Package specification
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Source file: e:\mybuild\m4communication\inc\commbase.hpp

#ifndef commbase_h
#define commbase_h 1

//## begin module%3B4971A80294.additionalIncludes preserve=no
//## end module%3B4971A80294.additionalIncludes

//## begin module%3B4971A80294.includes preserve=yes
#include <syncrowait.hpp>
//## end module%3B4971A80294.includes

// m4types
#include <m4types.hpp>
// m4condition
#include <m4condition.hpp>
//## begin module%3B4971A80294.declarations preserve=no
//## end module%3B4971A80294.declarations

//## begin module%3B4971A80294.additionalDeclarations preserve=yes
//## end module%3B4971A80294.additionalDeclarations


//## begin ClCommCondition%3B4971F201A0.preface preserve=yes
//## end ClCommCondition%3B4971F201A0.preface

//## Class: ClCommCondition%3B4971F201A0
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3B497331027B;m4int16_t { -> }
//## Uses: <unnamed>%3B49733800B9;m4return_t { -> }

class ClCommCondition 
{
  //## begin ClCommCondition%3B4971F201A0.initialDeclarations preserve=yes
  //## end ClCommCondition%3B4971F201A0.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommCondition%994669370
      ClCommCondition ();

    //## Destructor (specified)
      //## Operation: ~ClCommCondition%994669371
      ~ClCommCondition ();


    //## Other Operations (specified)
      //## Operation: Signal%994669372
      m4return_t Signal ();

      //## Operation: Wait%994669373
      m4return_t Wait (m4int32_t ai_millisecs = -1);

      //## Operation: Reset%994669374
      m4return_t Reset ();

    // Data Members for Associations

      //## Association: M4Communication::M4COML::<unnamed>%3B49CEBA001D
      //## Role: ClCommCondition::m_oCondition%3B49CEBA00A0
      //## begin ClCommCondition::m_oCondition%3B49CEBA00A0.role preserve=no  public: ClCondition { -> VHAN}
      ClSyncroWait m_oCondition;
      //## end ClCommCondition::m_oCondition%3B49CEBA00A0.role

    // Additional Public Declarations
      //## begin ClCommCondition%3B4971F201A0.public preserve=yes
      //## end ClCommCondition%3B4971F201A0.public

  protected:

    //## Other Operations (specified)
      //## Operation: CondSleep%994669375
      void CondSleep (m4int_t ai_milisecond);

    // Additional Protected Declarations
      //## begin ClCommCondition%3B4971F201A0.protected preserve=yes
      //## end ClCommCondition%3B4971F201A0.protected

  private:
    // Additional Private Declarations
      //## begin ClCommCondition%3B4971F201A0.private preserve=yes
      //## end ClCommCondition%3B4971F201A0.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommCondition%3B4971F201A0.implementation preserve=yes
      //## end ClCommCondition%3B4971F201A0.implementation

};

//## begin ClCommCondition%3B4971F201A0.postscript preserve=yes
//## end ClCommCondition%3B4971F201A0.postscript

// Class ClCommCondition 

//## begin module%3B4971A80294.epilog preserve=yes
//## end module%3B4971A80294.epilog


#endif

//## begin module%3961A27E0305.cm preserve=no
//## end module%3961A27E0305.cm

//## begin module%3961A27E0305.cp preserve=no
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
//## end module%3961A27E0305.cp

//## Module: commproccessaction%3961A27E0305; Package specification
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Source file: e:\mybuild\m4communication\inc\commproccessaction.hpp

#ifndef commproccessaction_h
#define commproccessaction_h 1

//## begin module%3961A27E0305.additionalIncludes preserve=no
//## end module%3961A27E0305.additionalIncludes

//## begin module%3961A27E0305.includes preserve=yes
//## end module%3961A27E0305.includes

// m4types
#include <m4types.hpp>
// CommAction
#include <commaction.hpp>
// CommComun
#include <commcomun.hpp>
//## begin module%3961A27E0305.declarations preserve=no
//## end module%3961A27E0305.declarations

//## begin module%3961A27E0305.additionalDeclarations preserve=yes
//## end module%3961A27E0305.additionalDeclarations


//## begin ClCommProccessBaseAction%3992C56E0149.preface preserve=yes
//## end ClCommProccessBaseAction%3992C56E0149.preface

//## Class: ClCommProccessBaseAction%3992C56E0149
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3993AF610255;ClCommTask { -> F}
//## Uses: <unnamed>%3993AFE803C2;ClCommunicationImplementation { -> }

class ClCommProccessBaseAction : public ClCommAction  //## Inherits: <unnamed>%3992C5810377
{
  //## begin ClCommProccessBaseAction%3992C56E0149.initialDeclarations preserve=yes
  //## end ClCommProccessBaseAction%3992C56E0149.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommProccessBaseAction%965898339; C++
      ClCommProccessBaseAction (m4tliid_t ai_Identifier, m4return_t ai_Success, ClCommTask *ai_pTask);


    //## Other Operations (specified)
      //## Operation: Execute%965898340; C++
      virtual m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClCommProccessBaseAction%3992C56E0149.public preserve=yes
      //## end ClCommProccessBaseAction%3992C56E0149.public

  protected:
    // Data Members for Associations

      //## Association: M4Communication::M4COML::<unnamed>%3961B1F703D3
      //## Role: ClCommProccessBaseAction::m_TLIdentifier%3961B1F902B3
      //## begin ClCommProccessBaseAction::m_TLIdentifier%3961B1F902B3.role preserve=no  protected: m4tliid_t { -> VHAN}
      m4tliid_t m_TLIdentifier;
      //## end ClCommProccessBaseAction::m_TLIdentifier%3961B1F902B3.role

      //## Association: M4Communication::M4COML::<unnamed>%3961C090032C
      //## Role: ClCommProccessBaseAction::m_Success%3961C092032F
      //## begin ClCommProccessBaseAction::m_Success%3961C092032F.role preserve=no  protected: m4return_t { -> VHAN}
      m4return_t m_Success;
      //## end ClCommProccessBaseAction::m_Success%3961C092032F.role

    // Additional Protected Declarations
      //## begin ClCommProccessBaseAction%3992C56E0149.protected preserve=yes
      //## end ClCommProccessBaseAction%3992C56E0149.protected

  private:
    // Additional Private Declarations
      //## begin ClCommProccessBaseAction%3992C56E0149.private preserve=yes
      //## end ClCommProccessBaseAction%3992C56E0149.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommProccessBaseAction%3992C56E0149.implementation preserve=yes
      //## end ClCommProccessBaseAction%3992C56E0149.implementation

};

//## begin ClCommProccessBaseAction%3992C56E0149.postscript preserve=yes
//## end ClCommProccessBaseAction%3992C56E0149.postscript

//## begin ClCommProccessOpenPortAction%3961963001CF.preface preserve=yes
//## end ClCommProccessOpenPortAction%3961963001CF.preface

//## Class: ClCommProccessOpenPortAction%3961963001CF
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3961CD8D0265;ClCommReadingTask { -> }

class ClCommProccessOpenPortAction : public ClCommProccessBaseAction  //## Inherits: <unnamed>%3992C5930279
{
  //## begin ClCommProccessOpenPortAction%3961963001CF.initialDeclarations preserve=yes
  //## end ClCommProccessOpenPortAction%3961963001CF.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommProccessOpenPortAction%962701624
      ClCommProccessOpenPortAction (m4tliid_t ai_Identifier, m4return_t ai_Success, ClCommTask *ai_pTask);


    //## Other Operations (specified)
      //## Operation: Execute%962701632; C++
      virtual m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClCommProccessOpenPortAction%3961963001CF.public preserve=yes
      //## end ClCommProccessOpenPortAction%3961963001CF.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommProccessOpenPortAction%3961963001CF.protected preserve=yes
      //## end ClCommProccessOpenPortAction%3961963001CF.protected

  private:
    // Additional Private Declarations
      //## begin ClCommProccessOpenPortAction%3961963001CF.private preserve=yes
      //## end ClCommProccessOpenPortAction%3961963001CF.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommProccessOpenPortAction%3961963001CF.implementation preserve=yes
      //## end ClCommProccessOpenPortAction%3961963001CF.implementation

};

//## begin ClCommProccessOpenPortAction%3961963001CF.postscript preserve=yes
//## end ClCommProccessOpenPortAction%3961963001CF.postscript

//## begin ClCommProccessClosePortAction%3961981A01D3.preface preserve=yes
//## end ClCommProccessClosePortAction%3961981A01D3.preface

//## Class: ClCommProccessClosePortAction%3961981A01D3
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommProccessClosePortAction : public ClCommProccessBaseAction  //## Inherits: <unnamed>%3961981A01D4
{
  //## begin ClCommProccessClosePortAction%3961981A01D3.initialDeclarations preserve=yes
  //## end ClCommProccessClosePortAction%3961981A01D3.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommProccessClosePortAction%962701625
      ClCommProccessClosePortAction (m4tliid_t ai_Identifier, m4return_t ai_Success, ClCommTask *ai_pTask);


    //## Other Operations (specified)
      //## Operation: Execute%962701633; C++
      virtual m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClCommProccessClosePortAction%3961981A01D3.public preserve=yes
      //## end ClCommProccessClosePortAction%3961981A01D3.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommProccessClosePortAction%3961981A01D3.protected preserve=yes
      //## end ClCommProccessClosePortAction%3961981A01D3.protected

  private:
    // Additional Private Declarations
      //## begin ClCommProccessClosePortAction%3961981A01D3.private preserve=yes
      //## end ClCommProccessClosePortAction%3961981A01D3.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommProccessClosePortAction%3961981A01D3.implementation preserve=yes
      //## end ClCommProccessClosePortAction%3961981A01D3.implementation

};

//## begin ClCommProccessClosePortAction%3961981A01D3.postscript preserve=yes
//## end ClCommProccessClosePortAction%3961981A01D3.postscript

//## begin ClCommProccessBeginReadAction%3961985B0014.preface preserve=yes
//## end ClCommProccessBeginReadAction%3961985B0014.preface

//## Class: ClCommProccessBeginReadAction%3961985B0014
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommProccessBeginReadAction : public ClCommProccessBaseAction  //## Inherits: <unnamed>%3961985B0015
{
  //## begin ClCommProccessBeginReadAction%3961985B0014.initialDeclarations preserve=yes
  //## end ClCommProccessBeginReadAction%3961985B0014.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommProccessBeginReadAction%962701626
      ClCommProccessBeginReadAction (m4tliid_t ai_Identifier, m4return_t ai_Success, ClCommTask *ai_pTask);


    //## Other Operations (specified)
      //## Operation: Execute%962701634; C++
      virtual m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClCommProccessBeginReadAction%3961985B0014.public preserve=yes
      //## end ClCommProccessBeginReadAction%3961985B0014.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommProccessBeginReadAction%3961985B0014.protected preserve=yes
      //## end ClCommProccessBeginReadAction%3961985B0014.protected

  private:
    // Additional Private Declarations
      //## begin ClCommProccessBeginReadAction%3961985B0014.private preserve=yes
      //## end ClCommProccessBeginReadAction%3961985B0014.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommProccessBeginReadAction%3961985B0014.implementation preserve=yes
      //## end ClCommProccessBeginReadAction%3961985B0014.implementation

};

//## begin ClCommProccessBeginReadAction%3961985B0014.postscript preserve=yes
//## end ClCommProccessBeginReadAction%3961985B0014.postscript

//## begin ClCommProccessStopReadingAction%39619AE30156.preface preserve=yes
//## end ClCommProccessStopReadingAction%39619AE30156.preface

//## Class: ClCommProccessStopReadingAction%39619AE30156
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommProccessStopReadingAction : public ClCommProccessBaseAction  //## Inherits: <unnamed>%39619AE30157
{
  //## begin ClCommProccessStopReadingAction%39619AE30156.initialDeclarations preserve=yes
  //## end ClCommProccessStopReadingAction%39619AE30156.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommProccessStopReadingAction%962701627
      ClCommProccessStopReadingAction (m4tliid_t ai_Identifier, m4return_t ai_Success, ClCommTask *ai_pTask);


    //## Other Operations (specified)
      //## Operation: Execute%962701635; C++
      virtual m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClCommProccessStopReadingAction%39619AE30156.public preserve=yes
      //## end ClCommProccessStopReadingAction%39619AE30156.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommProccessStopReadingAction%39619AE30156.protected preserve=yes
      //## end ClCommProccessStopReadingAction%39619AE30156.protected

  private:
    // Additional Private Declarations
      //## begin ClCommProccessStopReadingAction%39619AE30156.private preserve=yes
      //## end ClCommProccessStopReadingAction%39619AE30156.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommProccessStopReadingAction%39619AE30156.implementation preserve=yes
      //## end ClCommProccessStopReadingAction%39619AE30156.implementation

};

//## begin ClCommProccessStopReadingAction%39619AE30156.postscript preserve=yes
//## end ClCommProccessStopReadingAction%39619AE30156.postscript

//## begin ClCommProccessStopWritingAction%39619E1D0208.preface preserve=yes
//## end ClCommProccessStopWritingAction%39619E1D0208.preface

//## Class: ClCommProccessStopWritingAction%39619E1D0208
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommProccessStopWritingAction : public ClCommProccessBaseAction  //## Inherits: <unnamed>%39619E1D0209
{
  //## begin ClCommProccessStopWritingAction%39619E1D0208.initialDeclarations preserve=yes
  //## end ClCommProccessStopWritingAction%39619E1D0208.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommProccessStopWritingAction%962701628; C++
      ClCommProccessStopWritingAction (m4tliid_t ai_Identifier, m4return_t ai_Success, ClCommTask *ai_pTask);


    //## Other Operations (specified)
      //## Operation: Execute%962701636; C++
      virtual m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClCommProccessStopWritingAction%39619E1D0208.public preserve=yes
      //## end ClCommProccessStopWritingAction%39619E1D0208.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommProccessStopWritingAction%39619E1D0208.protected preserve=yes
      //## end ClCommProccessStopWritingAction%39619E1D0208.protected

  private:
    // Additional Private Declarations
      //## begin ClCommProccessStopWritingAction%39619E1D0208.private preserve=yes
      //## end ClCommProccessStopWritingAction%39619E1D0208.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommProccessStopWritingAction%39619E1D0208.implementation preserve=yes
      //## end ClCommProccessStopWritingAction%39619E1D0208.implementation

};

//## begin ClCommProccessStopWritingAction%39619E1D0208.postscript preserve=yes
//## end ClCommProccessStopWritingAction%39619E1D0208.postscript

//## begin ClCommProccessStopConnectionAction%39619E6101DE.preface preserve=yes
//## end ClCommProccessStopConnectionAction%39619E6101DE.preface

//## Class: ClCommProccessStopConnectionAction%39619E6101DE
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommProccessStopConnectionAction : public ClCommProccessBaseAction  //## Inherits: <unnamed>%39619E6101DF
{
  //## begin ClCommProccessStopConnectionAction%39619E6101DE.initialDeclarations preserve=yes
  //## end ClCommProccessStopConnectionAction%39619E6101DE.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommProccessStopConnectionAction%962701629
      ClCommProccessStopConnectionAction (m4tliid_t ai_Identifier, m4return_t ai_Success, ClCommTask *ai_pTask);


    //## Other Operations (specified)
      //## Operation: Execute%962701637; C++
      virtual m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClCommProccessStopConnectionAction%39619E6101DE.public preserve=yes
      //## end ClCommProccessStopConnectionAction%39619E6101DE.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommProccessStopConnectionAction%39619E6101DE.protected preserve=yes
      //## end ClCommProccessStopConnectionAction%39619E6101DE.protected

  private:
    // Additional Private Declarations
      //## begin ClCommProccessStopConnectionAction%39619E6101DE.private preserve=yes
      //## end ClCommProccessStopConnectionAction%39619E6101DE.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommProccessStopConnectionAction%39619E6101DE.implementation preserve=yes
      //## end ClCommProccessStopConnectionAction%39619E6101DE.implementation

};

//## begin ClCommProccessStopConnectionAction%39619E6101DE.postscript preserve=yes
//## end ClCommProccessStopConnectionAction%39619E6101DE.postscript

//## begin ClCommProccessConnectAction%39619F7F015D.preface preserve=yes
//## end ClCommProccessConnectAction%39619F7F015D.preface

//## Class: ClCommProccessConnectAction%39619F7F015D
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommProccessConnectAction : public ClCommProccessBaseAction  //## Inherits: <unnamed>%39619F7F015E
{
  //## begin ClCommProccessConnectAction%39619F7F015D.initialDeclarations preserve=yes
  //## end ClCommProccessConnectAction%39619F7F015D.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommProccessConnectAction%962701630
      ClCommProccessConnectAction (m4tliid_t ai_Identifier, m4return_t ai_Success, ClCommTask *ai_pTask);


    //## Other Operations (specified)
      //## Operation: Execute%962701638; C++
      virtual m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClCommProccessConnectAction%39619F7F015D.public preserve=yes
      //## end ClCommProccessConnectAction%39619F7F015D.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommProccessConnectAction%39619F7F015D.protected preserve=yes
      //## end ClCommProccessConnectAction%39619F7F015D.protected

  private:
    // Additional Private Declarations
      //## begin ClCommProccessConnectAction%39619F7F015D.private preserve=yes
      //## end ClCommProccessConnectAction%39619F7F015D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommProccessConnectAction%39619F7F015D.implementation preserve=yes
      //## end ClCommProccessConnectAction%39619F7F015D.implementation

};

//## begin ClCommProccessConnectAction%39619F7F015D.postscript preserve=yes
//## end ClCommProccessConnectAction%39619F7F015D.postscript

//## begin ClCommProccessNewConnectionAction%3961A04100C6.preface preserve=yes
//## end ClCommProccessNewConnectionAction%3961A04100C6.preface

//## Class: ClCommProccessNewConnectionAction%3961A04100C6
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommProccessNewConnectionAction : public ClCommProccessBaseAction  //## Inherits: <unnamed>%3961A04100C7
{
  //## begin ClCommProccessNewConnectionAction%3961A04100C6.initialDeclarations preserve=yes
  //## end ClCommProccessNewConnectionAction%3961A04100C6.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommProccessNewConnectionAction%962701631
      ClCommProccessNewConnectionAction (m4tliid_t ai_Identifier, m4return_t ai_Success, ClCommTask *ai_pTask);


    //## Other Operations (specified)
      //## Operation: Execute%962701639; C++
      virtual m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClCommProccessNewConnectionAction%3961A04100C6.public preserve=yes
      //## end ClCommProccessNewConnectionAction%3961A04100C6.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommProccessNewConnectionAction%3961A04100C6.protected preserve=yes
      //## end ClCommProccessNewConnectionAction%3961A04100C6.protected

  private:
    // Additional Private Declarations
      //## begin ClCommProccessNewConnectionAction%3961A04100C6.private preserve=yes
      //## end ClCommProccessNewConnectionAction%3961A04100C6.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommProccessNewConnectionAction%3961A04100C6.implementation preserve=yes
      //## end ClCommProccessNewConnectionAction%3961A04100C6.implementation

};

//## begin ClCommProccessNewConnectionAction%3961A04100C6.postscript preserve=yes
//## end ClCommProccessNewConnectionAction%3961A04100C6.postscript

//## begin ClCommProccessExternalCloseAction%3A4090A002FE.preface preserve=yes
//## end ClCommProccessExternalCloseAction%3A4090A002FE.preface

//## Class: ClCommProccessExternalCloseAction%3A4090A002FE
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommProccessExternalCloseAction : public ClCommProccessBaseAction  //## Inherits: <unnamed>%3A40911701FA
{
  //## begin ClCommProccessExternalCloseAction%3A4090A002FE.initialDeclarations preserve=yes
  //## end ClCommProccessExternalCloseAction%3A4090A002FE.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommProccessExternalCloseAction%977310088; C++
      ClCommProccessExternalCloseAction (m4tliid_t ai_Identifier, m4return_t ai_Success, ClCommTask *ai_pTask);


    //## Other Operations (specified)
      //## Operation: Execute%977310089; C++
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClCommProccessExternalCloseAction%3A4090A002FE.public preserve=yes
      //## end ClCommProccessExternalCloseAction%3A4090A002FE.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommProccessExternalCloseAction%3A4090A002FE.protected preserve=yes
      //## end ClCommProccessExternalCloseAction%3A4090A002FE.protected

  private:
    // Additional Private Declarations
      //## begin ClCommProccessExternalCloseAction%3A4090A002FE.private preserve=yes
      //## end ClCommProccessExternalCloseAction%3A4090A002FE.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommProccessExternalCloseAction%3A4090A002FE.implementation preserve=yes
      //## end ClCommProccessExternalCloseAction%3A4090A002FE.implementation

};

//## begin ClCommProccessExternalCloseAction%3A4090A002FE.postscript preserve=yes
//## end ClCommProccessExternalCloseAction%3A4090A002FE.postscript

// Class ClCommProccessBaseAction 

// Class ClCommProccessOpenPortAction 

// Class ClCommProccessClosePortAction 

// Class ClCommProccessBeginReadAction 

// Class ClCommProccessStopReadingAction 

// Class ClCommProccessStopWritingAction 

// Class ClCommProccessStopConnectionAction 

// Class ClCommProccessConnectAction 

// Class ClCommProccessNewConnectionAction 

// Class ClCommProccessExternalCloseAction 

//## begin module%3961A27E0305.epilog preserve=yes
//## end module%3961A27E0305.epilog


#endif

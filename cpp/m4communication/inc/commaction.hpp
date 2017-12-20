//## begin module%38DB63340186.cm preserve=no
//## end module%38DB63340186.cm

//## begin module%38DB63340186.cp preserve=no
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
//## end module%38DB63340186.cp

//## Module: CommAction%38DB63340186; Package specification
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Source file: e:\mybuild\m4communication\inc\commaction.hpp

#ifndef commaction_h
#define commaction_h 1

//## begin module%38DB63340186.additionalIncludes preserve=no
//## end module%38DB63340186.additionalIncludes

//## begin module%38DB63340186.includes preserve=yes
#define M4_COMM_SHUTDOWN_WORKER		((m4return_t)100)
//## end module%38DB63340186.includes

// FastQ
#include <fastfifoq.hpp>
// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
// clbaseaction
#include <clbaseaction.hpp>
// CommComun
#include <commcomun.hpp>
// m4crono
#include <m4crono.hpp>

class ClCommunicationImplementation;
class ClCommTask;
class ClCommProtocol;
class ClExecutableInterface;
class ClCommCondition;

//## begin module%38DB63340186.declarations preserve=no
//## end module%38DB63340186.declarations

//## begin module%38DB63340186.additionalDeclarations preserve=yes
//## end module%38DB63340186.additionalDeclarations


//## begin ClCommExternalClock%39AA857A00D0.preface preserve=yes
//## end ClCommExternalClock%39AA857A00D0.preface

//## Class: ClCommExternalClock%39AA857A00D0
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39AA8AD103C5;ClCrono { -> }

class ClCommExternalClock 
{
  //## begin ClCommExternalClock%39AA857A00D0.initialDeclarations preserve=yes
  //## end ClCommExternalClock%39AA857A00D0.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: Start%967474877
      m4return_t Start ();

      //## Operation: Stop%967474878
      m4return_t Stop ();

      //## Operation: SetCounter%967474879; C++
      m4return_t SetCounter (m4millisec_t &ai_msExternalCounter);

      //## Operation: SetClock%967474880; C++
      m4return_t SetClock (ClCrono *ai_poCrono);

    // Additional Public Declarations
      //## begin ClCommExternalClock%39AA857A00D0.public preserve=yes
      //## end ClCommExternalClock%39AA857A00D0.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommExternalClock%39AA857A00D0.protected preserve=yes
      //## end ClCommExternalClock%39AA857A00D0.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_pmsExternalCounter%39AA85DF0252
      //## begin ClCommExternalClock::m_pmsExternalCounter%39AA85DF0252.attr preserve=no  private: m4millisec_t* {UA} NULL
      m4millisec_t* m_pmsExternalCounter;
      //## end ClCommExternalClock::m_pmsExternalCounter%39AA85DF0252.attr

      //## Attribute: m_poCrono%39AA86AC0397
      //## begin ClCommExternalClock::m_poCrono%39AA86AC0397.attr preserve=no  private: ClCrono * {UA} NULL
      ClCrono *m_poCrono;
      //## end ClCommExternalClock::m_poCrono%39AA86AC0397.attr

    // Additional Private Declarations
      //## begin ClCommExternalClock%39AA857A00D0.private preserve=yes
      //## end ClCommExternalClock%39AA857A00D0.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommExternalClock%39AA857A00D0.implementation preserve=yes
      //## end ClCommExternalClock%39AA857A00D0.implementation

};

//## begin ClCommExternalClock%39AA857A00D0.postscript preserve=yes
//## end ClCommExternalClock%39AA857A00D0.postscript

//## begin ClCommAction%38D9E14E0268.preface preserve=yes
//## end ClCommAction%38D9E14E0268.preface

//## Class: ClCommAction%38D9E14E0268
//	Las acciones son las peticiones que tienen que satisfacer los workers
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3992D17C02F5;m4tliid_t { -> }
//## Uses: <unnamed>%3993AE6E0396;ClCommProtocol { -> F}

class ClCommAction : public ClBaseAction  //## Inherits: <unnamed>%390810320201
{
  //## begin ClCommAction%38D9E14E0268.initialDeclarations preserve=yes
  //## end ClCommAction%38D9E14E0268.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommAction%955103952; C++
      ClCommAction ();

    //## Destructor (specified)
      //## Operation: ~ClCommAction%955103953
      ~ClCommAction ();


    //## Other Operations (specified)
      //## Operation: Execute%954227121; C++
      virtual m4return_t Execute ();

      //## Operation: SetCondition%965219972; C++
      m4return_t SetCondition (ClCommCondition *ai_poCondition);

      //## Operation: GetCondition%965219973; C++
      ClCommCondition * GetCondition ();

      //## Operation: SetActionName%965219974; C++
      m4return_t SetActionName (m4pchar_t ai_szActionName);

      //## Operation: GetActionName%965219975; C++
      m4pcchar_t GetActionName ();

      //## Operation: SetExecutable%965219976; C++
      m4return_t SetExecutable (ClExecutableInterface *ai_pExecutable);

      //## Operation: GetExecutable%965219977; C++
      ClExecutableInterface * GetExecutable ();

      //## Operation: PrepareTask%965898335; C++
      virtual m4return_t PrepareTask (m4handle_t ai_hConnectionID, m4tliid_t  ai_TLIID, ClCommProtocol *  ai_pProtocol);

      //## Operation: Notify%965977805; C++
      m4return_t Notify (ClParamList *ai_poParams);

      //## Operation: SetExtExecutionCrono%967474876; C++
      m4return_t SetExtExecutionCrono (ClCommExternalClock *ai_pExtExecutionCrono);

    // Data Members for Associations

      //## Association: M4Communication::M4COML::<unnamed>%3992D1B502E3
      //## Role: ClCommAction::m_poTask%3992D1B601F4
      //## begin ClCommAction::m_poTask%3992D1B601F4.role preserve=no  public: ClCommTask { -> RFHAN}
      ClCommTask *m_poTask;
      //## end ClCommAction::m_poTask%3992D1B601F4.role

    // Additional Public Declarations
      //## begin ClCommAction%38D9E14E0268.public preserve=yes
      //## end ClCommAction%38D9E14E0268.public

  protected:

    //## Other Operations (specified)
      //## Operation: _PassReturnInfo%965219986; C++
      m4return_t _PassReturnInfo ();

    // Data Members for Associations

      //## Association: M4Communication::M4COML::<unnamed>%38F582E30342
      //## Role: ClCommAction::m_poCommImplementation%38F582E4029A
      //## begin ClCommAction::m_poCommImplementation%38F582E4029A.role preserve=no  protected: ClCommunicationImplementation { -> 1RFHAN}
      ClCommunicationImplementation *m_poCommImplementation;
      //## end ClCommAction::m_poCommImplementation%38F582E4029A.role

      //## Association: M4Communication::M4COML::<unnamed>%39634DC201ED
      //## Role: ClCommAction::m_hConnectionID%39634DC40146
      //## begin ClCommAction::m_hConnectionID%39634DC40146.role preserve=no  protected: m4handle_t { -> VHAN}
      m4handle_t m_hConnectionID;
      //## end ClCommAction::m_hConnectionID%39634DC40146.role

      //## Association: M4Communication::M4COML::<unnamed>%398817650160
      //## Role: ClCommAction::m_pExecutableInterface%3988176600A3
      //## begin ClCommAction::m_pExecutableInterface%3988176600A3.role preserve=no  protected: ClExecutableInterface { -> RFHAN}
      ClExecutableInterface *m_pExecutableInterface;
      //## end ClCommAction::m_pExecutableInterface%3988176600A3.role

      //## Association: M4Communication::M4COML::<unnamed>%398817F101A7
      //## Role: ClCommAction::m_strActionName%398817F200C2
      //## begin ClCommAction::m_strActionName%398817F200C2.role preserve=no  protected: m4string_t { -> VHAN}
      m4string_t m_strActionName;
      //## end ClCommAction::m_strActionName%398817F200C2.role

      //## Association: M4Communication::M4COML::<unnamed>%3988180803C7
      //## Role: ClCommAction::m_poCondition%3988180A01FD
      //## begin ClCommAction::m_poCondition%3988180A01FD.role preserve=no  protected: ClCommCondition { -> RFHAN}
      ClCommCondition *m_poCondition;
      //## end ClCommAction::m_poCondition%3988180A01FD.role

      //## Association: M4Communication::M4COML::<unnamed>%39AA81BF020E
      //## Role: ClCommAction::m_poExtExecutionCrono%39AA81BF03BC
      //## begin ClCommAction::m_poExtExecutionCrono%39AA81BF03BC.role preserve=no  protected: ClCommExternalClock { -> RHAN}
      ClCommExternalClock *m_poExtExecutionCrono;
      //## end ClCommAction::m_poExtExecutionCrono%39AA81BF03BC.role

    // Additional Protected Declarations
      //## begin ClCommAction%38D9E14E0268.protected preserve=yes
      //## end ClCommAction%38D9E14E0268.protected

  private:
    // Additional Private Declarations
      //## begin ClCommAction%38D9E14E0268.private preserve=yes
      //## end ClCommAction%38D9E14E0268.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommAction%38D9E14E0268.implementation preserve=yes
      //## end ClCommAction%38D9E14E0268.implementation

};

//## begin ClCommAction%38D9E14E0268.postscript preserve=yes
//## end ClCommAction%38D9E14E0268.postscript

//## begin ClActionQueue%38D9EDD0005A.preface preserve=yes
//## end ClActionQueue%38D9EDD0005A.preface

//## Class: ClActionQueue%38D9EDD0005A; Instantiated Class
//	En esta cola almacenamos las acciones que tenemos pendientes de ser
//	realizadas.
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%38D9EE0F0317;ClCommAction { -> }

typedef FastQueue< ClCommAction * > ClActionQueue;

//## begin ClActionQueue%38D9EDD0005A.postscript preserve=yes
//## end ClActionQueue%38D9EDD0005A.postscript

//## begin ClCommStartReadingAction%38E06F94010A.preface preserve=yes
//## end ClCommStartReadingAction%38E06F94010A.preface

//## Class: ClCommStartReadingAction%38E06F94010A
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommStartReadingAction : public ClCommAction  //## Inherits: <unnamed>%38E06FD501AD
{
  //## begin ClCommStartReadingAction%38E06F94010A.initialDeclarations preserve=yes
  //## end ClCommStartReadingAction%38E06F94010A.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommStartReadingAction%955103954; C++
      ClCommStartReadingAction ();

    //## Destructor (specified)
      //## Operation: ~ClCommStartReadingAction%955103955
      ~ClCommStartReadingAction ();


    //## Other Operations (specified)
      //## Operation: Execute%954227123; C++
      m4return_t Execute ();

      //## Operation: PrepareTask%965898334; C++
      m4return_t PrepareTask (m4handle_t ai_hConnectionID, m4tliid_t  ai_TLIID, ClCommProtocol *  ai_pProtocol);

    // Additional Public Declarations
      //## begin ClCommStartReadingAction%38E06F94010A.public preserve=yes
      //## end ClCommStartReadingAction%38E06F94010A.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommStartReadingAction%38E06F94010A.protected preserve=yes
      //## end ClCommStartReadingAction%38E06F94010A.protected

  private:
    // Additional Private Declarations
      //## begin ClCommStartReadingAction%38E06F94010A.private preserve=yes
      //## end ClCommStartReadingAction%38E06F94010A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommStartReadingAction%38E06F94010A.implementation preserve=yes
      //## end ClCommStartReadingAction%38E06F94010A.implementation

};

//## begin ClCommStartReadingAction%38E06F94010A.postscript preserve=yes
//## end ClCommStartReadingAction%38E06F94010A.postscript

//## begin ClCommReadedAction%38E06FA801EF.preface preserve=yes
//## end ClCommReadedAction%38E06FA801EF.preface

//## Class: ClCommReadedAction%38E06FA801EF
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommReadedAction : public ClCommAction  //## Inherits: <unnamed>%38E06FD702F1
{
  //## begin ClCommReadedAction%38E06FA801EF.initialDeclarations preserve=yes
  //## end ClCommReadedAction%38E06FA801EF.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommReadedAction%955469192; C++
      ClCommReadedAction ();

    //## Destructor (specified)
      //## Operation: ~ClCommReadedAction%955469193
      ~ClCommReadedAction ();


    //## Other Operations (specified)
      //## Operation: Execute%954227122; C++
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClCommReadedAction%38E06FA801EF.public preserve=yes
      //## end ClCommReadedAction%38E06FA801EF.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommReadedAction%38E06FA801EF.protected preserve=yes
      //## end ClCommReadedAction%38E06FA801EF.protected

  private:
    // Additional Private Declarations
      //## begin ClCommReadedAction%38E06FA801EF.private preserve=yes
      //## end ClCommReadedAction%38E06FA801EF.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommReadedAction%38E06FA801EF.implementation preserve=yes
      //## end ClCommReadedAction%38E06FA801EF.implementation

};

//## begin ClCommReadedAction%38E06FA801EF.postscript preserve=yes
//## end ClCommReadedAction%38E06FA801EF.postscript

//## begin ClCommStopReadingAction%38E06FC703CA.preface preserve=yes
//## end ClCommStopReadingAction%38E06FC703CA.preface

//## Class: ClCommStopReadingAction%38E06FC703CA
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommStopReadingAction : public ClCommAction  //## Inherits: <unnamed>%38E06FDB0007
{
  //## begin ClCommStopReadingAction%38E06FC703CA.initialDeclarations preserve=yes
  //## end ClCommStopReadingAction%38E06FC703CA.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommStopReadingAction%955103956; C++
      ClCommStopReadingAction ();

    //## Destructor (specified)
      //## Operation: ~ClCommStopReadingAction%955103957
      ~ClCommStopReadingAction ();


    //## Other Operations (specified)
      //## Operation: Execute%954227124; C++
      m4return_t Execute ();

      //## Operation: PrepareTask%966440286; C++
      m4return_t PrepareTask (m4handle_t ai_hConnectionID, m4tliid_t  ai_TLIID, ClCommProtocol *  ai_pProtocol);

    // Additional Public Declarations
      //## begin ClCommStopReadingAction%38E06FC703CA.public preserve=yes
      //## end ClCommStopReadingAction%38E06FC703CA.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommStopReadingAction%38E06FC703CA.protected preserve=yes
      //## end ClCommStopReadingAction%38E06FC703CA.protected

  private:
    // Additional Private Declarations
      //## begin ClCommStopReadingAction%38E06FC703CA.private preserve=yes
      //## end ClCommStopReadingAction%38E06FC703CA.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommStopReadingAction%38E06FC703CA.implementation preserve=yes
      //## end ClCommStopReadingAction%38E06FC703CA.implementation

};

//## begin ClCommStopReadingAction%38E06FC703CA.postscript preserve=yes
//## end ClCommStopReadingAction%38E06FC703CA.postscript

//## begin ClCommWriteAction%38E070210008.preface preserve=yes
//## end ClCommWriteAction%38E070210008.preface

//## Class: ClCommWriteAction%38E070210008
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommWriteAction : public ClCommAction  //## Inherits: <unnamed>%38E070750316
{
  //## begin ClCommWriteAction%38E070210008.initialDeclarations preserve=yes
  //## end ClCommWriteAction%38E070210008.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommWriteAction%955103958; C++
      ClCommWriteAction ();

    //## Destructor (specified)
      //## Operation: ~ClCommWriteAction%955103959
      ~ClCommWriteAction ();


    //## Other Operations (specified)
      //## Operation: Execute%954227127; C++
      m4return_t Execute ();

      //## Operation: PrepareTask%965898336; C++
      m4return_t PrepareTask (m4handle_t ai_hConnectionID, m4tliid_t  ai_TLIID, ClCommProtocol *  ai_pProtocol);

    // Additional Public Declarations
      //## begin ClCommWriteAction%38E070210008.public preserve=yes
      //## end ClCommWriteAction%38E070210008.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommWriteAction%38E070210008.protected preserve=yes
      //## end ClCommWriteAction%38E070210008.protected

  private:
    // Additional Private Declarations
      //## begin ClCommWriteAction%38E070210008.private preserve=yes
      //## end ClCommWriteAction%38E070210008.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommWriteAction%38E070210008.implementation preserve=yes
      //## end ClCommWriteAction%38E070210008.implementation

};

//## begin ClCommWriteAction%38E070210008.postscript preserve=yes
//## end ClCommWriteAction%38E070210008.postscript

//## begin ClCommWrittenAction%38E0703401DC.preface preserve=yes
//## end ClCommWrittenAction%38E0703401DC.preface

//## Class: ClCommWrittenAction%38E0703401DC
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommWrittenAction : public ClCommAction  //## Inherits: <unnamed>%38E0707E02E7
{
  //## begin ClCommWrittenAction%38E0703401DC.initialDeclarations preserve=yes
  //## end ClCommWrittenAction%38E0703401DC.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommWrittenAction%955103960; C++
      ClCommWrittenAction ();

    //## Destructor (specified)
      //## Operation: ~ClCommWrittenAction%955103961
      ~ClCommWrittenAction ();


    //## Other Operations (specified)
      //## Operation: Execute%954227126; C++
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClCommWrittenAction%38E0703401DC.public preserve=yes
      //## end ClCommWrittenAction%38E0703401DC.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommWrittenAction%38E0703401DC.protected preserve=yes
      //## end ClCommWrittenAction%38E0703401DC.protected

  private:
    // Additional Private Declarations
      //## begin ClCommWrittenAction%38E0703401DC.private preserve=yes
      //## end ClCommWrittenAction%38E0703401DC.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommWrittenAction%38E0703401DC.implementation preserve=yes
      //## end ClCommWrittenAction%38E0703401DC.implementation

};

//## begin ClCommWrittenAction%38E0703401DC.postscript preserve=yes
//## end ClCommWrittenAction%38E0703401DC.postscript

//## begin ClCommStopWrittingAction%38E0706601F2.preface preserve=yes
//## end ClCommStopWrittingAction%38E0706601F2.preface

//## Class: ClCommStopWrittingAction%38E0706601F2
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommStopWrittingAction : public ClCommAction  //## Inherits: <unnamed>%38E0707801F8
{
  //## begin ClCommStopWrittingAction%38E0706601F2.initialDeclarations preserve=yes
  //## end ClCommStopWrittingAction%38E0706601F2.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommStopWrittingAction%955103962; C++
      ClCommStopWrittingAction ();

    //## Destructor (specified)
      //## Operation: ~ClCommStopWrittingAction%955103963
      ~ClCommStopWrittingAction ();


    //## Other Operations (specified)
      //## Operation: Execute%954227125; C++
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClCommStopWrittingAction%38E0706601F2.public preserve=yes
      //## end ClCommStopWrittingAction%38E0706601F2.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommStopWrittingAction%38E0706601F2.protected preserve=yes
      //## end ClCommStopWrittingAction%38E0706601F2.protected

  private:
    // Additional Private Declarations
      //## begin ClCommStopWrittingAction%38E0706601F2.private preserve=yes
      //## end ClCommStopWrittingAction%38E0706601F2.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommStopWrittingAction%38E0706601F2.implementation preserve=yes
      //## end ClCommStopWrittingAction%38E0706601F2.implementation

};

//## begin ClCommStopWrittingAction%38E0706601F2.postscript preserve=yes
//## end ClCommStopWrittingAction%38E0706601F2.postscript

//## begin ClCommFindFreeConnAction%38E0815700D6.preface preserve=yes
//## end ClCommFindFreeConnAction%38E0815700D6.preface

//## Class: ClCommFindFreeConnAction%38E0815700D6
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommFindFreeConnAction : public ClCommAction  //## Inherits: <unnamed>%38E0816703E6
{
  //## begin ClCommFindFreeConnAction%38E0815700D6.initialDeclarations preserve=yes
  //## end ClCommFindFreeConnAction%38E0815700D6.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommFindFreeConnAction%955103964; C++
      ClCommFindFreeConnAction ();

    //## Destructor (specified)
      //## Operation: ~ClCommFindFreeConnAction%955103965
      ~ClCommFindFreeConnAction ();


    //## Other Operations (specified)
      //## Operation: Execute%954835556; C++
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClCommFindFreeConnAction%38E0815700D6.public preserve=yes
      //## end ClCommFindFreeConnAction%38E0815700D6.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommFindFreeConnAction%38E0815700D6.protected preserve=yes
      //## end ClCommFindFreeConnAction%38E0815700D6.protected

  private:
    // Additional Private Declarations
      //## begin ClCommFindFreeConnAction%38E0815700D6.private preserve=yes
      //## end ClCommFindFreeConnAction%38E0815700D6.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommFindFreeConnAction%38E0815700D6.implementation preserve=yes
      //## end ClCommFindFreeConnAction%38E0815700D6.implementation

};

//## begin ClCommFindFreeConnAction%38E0815700D6.postscript preserve=yes
//## end ClCommFindFreeConnAction%38E0815700D6.postscript

//## begin ClCommStartAcceptingAction%38F34EDB00EB.preface preserve=yes
//## end ClCommStartAcceptingAction%38F34EDB00EB.preface

//## Class: ClCommStartAcceptingAction%38F34EDB00EB
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommStartAcceptingAction : public ClCommAction  //## Inherits: <unnamed>%38F34EE10180
{
  //## begin ClCommStartAcceptingAction%38F34EDB00EB.initialDeclarations preserve=yes
  //## end ClCommStartAcceptingAction%38F34EDB00EB.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommStartAcceptingAction%955469194; C++
      ClCommStartAcceptingAction ();

    //## Destructor (specified)
      //## Operation: ~ClCommStartAcceptingAction%955469195
      ~ClCommStartAcceptingAction ();


    //## Other Operations (specified)
      //## Operation: Execute%955469196; C++
      m4return_t Execute ();

      //## Operation: PrepareTask%966411245; C++
      m4return_t PrepareTask (m4handle_t ai_hConnectionID, m4tliid_t  ai_TLIID, ClCommProtocol *  ai_pProtocol);

    // Additional Public Declarations
      //## begin ClCommStartAcceptingAction%38F34EDB00EB.public preserve=yes
      //## end ClCommStartAcceptingAction%38F34EDB00EB.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommStartAcceptingAction%38F34EDB00EB.protected preserve=yes
      //## end ClCommStartAcceptingAction%38F34EDB00EB.protected

  private:
    // Additional Private Declarations
      //## begin ClCommStartAcceptingAction%38F34EDB00EB.private preserve=yes
      //## end ClCommStartAcceptingAction%38F34EDB00EB.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommStartAcceptingAction%38F34EDB00EB.implementation preserve=yes
      //## end ClCommStartAcceptingAction%38F34EDB00EB.implementation

};

//## begin ClCommStartAcceptingAction%38F34EDB00EB.postscript preserve=yes
//## end ClCommStartAcceptingAction%38F34EDB00EB.postscript

//## begin ClCommStopAcceptingAction%38F351470146.preface preserve=yes
//## end ClCommStopAcceptingAction%38F351470146.preface

//## Class: ClCommStopAcceptingAction%38F351470146
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommStopAcceptingAction : public ClCommAction  //## Inherits: <unnamed>%38F3524B0371
{
  //## begin ClCommStopAcceptingAction%38F351470146.initialDeclarations preserve=yes
  //## end ClCommStopAcceptingAction%38F351470146.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommStopAcceptingAction%955469197; C++
      ClCommStopAcceptingAction ();

    //## Destructor (specified)
      //## Operation: ~ClCommStopAcceptingAction%955469198
      ~ClCommStopAcceptingAction ();


    //## Other Operations (specified)
      //## Operation: Execute%955469199; C++
      m4return_t Execute ();

      //## Operation: PrepareTask%966496757; C++
      m4return_t PrepareTask (m4handle_t ai_hConnectionID, m4tliid_t  ai_TLIID, ClCommProtocol *  ai_pProtocol);

    // Additional Public Declarations
      //## begin ClCommStopAcceptingAction%38F351470146.public preserve=yes
      //## end ClCommStopAcceptingAction%38F351470146.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommStopAcceptingAction%38F351470146.protected preserve=yes
      //## end ClCommStopAcceptingAction%38F351470146.protected

  private:
    // Additional Private Declarations
      //## begin ClCommStopAcceptingAction%38F351470146.private preserve=yes
      //## end ClCommStopAcceptingAction%38F351470146.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommStopAcceptingAction%38F351470146.implementation preserve=yes
      //## end ClCommStopAcceptingAction%38F351470146.implementation

};

//## begin ClCommStopAcceptingAction%38F351470146.postscript preserve=yes
//## end ClCommStopAcceptingAction%38F351470146.postscript

//## begin ClCommOpenConnectionAction%38F45B6B0248.preface preserve=yes
//## end ClCommOpenConnectionAction%38F45B6B0248.preface

//## Class: ClCommOpenConnectionAction%38F45B6B0248
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommOpenConnectionAction : public ClCommAction  //## Inherits: <unnamed>%38F45B84014A
{
  //## begin ClCommOpenConnectionAction%38F45B6B0248.initialDeclarations preserve=yes
  //## end ClCommOpenConnectionAction%38F45B6B0248.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommOpenConnectionAction%955537525; C++
      ClCommOpenConnectionAction ();

    //## Destructor (specified)
      //## Operation: ~ClCommOpenConnectionAction%955537526; C++
      ~ClCommOpenConnectionAction ();


    //## Other Operations (specified)
      //## Operation: Execute%955537527; C++
      m4return_t Execute ();

      //## Operation: PrepareTask%965977803; C++
      virtual m4return_t PrepareTask (m4handle_t ai_hConnectionID, m4tliid_t  ai_TLIID, ClCommProtocol *  ai_pProtocol);

    // Additional Public Declarations
      //## begin ClCommOpenConnectionAction%38F45B6B0248.public preserve=yes
      //## end ClCommOpenConnectionAction%38F45B6B0248.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommOpenConnectionAction%38F45B6B0248.protected preserve=yes
      //## end ClCommOpenConnectionAction%38F45B6B0248.protected

  private:
    // Additional Private Declarations
      //## begin ClCommOpenConnectionAction%38F45B6B0248.private preserve=yes
      //## end ClCommOpenConnectionAction%38F45B6B0248.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommOpenConnectionAction%38F45B6B0248.implementation preserve=yes
      //## end ClCommOpenConnectionAction%38F45B6B0248.implementation

};

//## begin ClCommOpenConnectionAction%38F45B6B0248.postscript preserve=yes
//## end ClCommOpenConnectionAction%38F45B6B0248.postscript

//## begin ClCommCloseConnectionAction%38F45B8F01C8.preface preserve=yes
//## end ClCommCloseConnectionAction%38F45B8F01C8.preface

//## Class: ClCommCloseConnectionAction%38F45B8F01C8
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommCloseConnectionAction : public ClCommAction  //## Inherits: <unnamed>%38F45BA6001C
{
  //## begin ClCommCloseConnectionAction%38F45B8F01C8.initialDeclarations preserve=yes
  //## end ClCommCloseConnectionAction%38F45B8F01C8.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommCloseConnectionAction%955613876; C++
      ClCommCloseConnectionAction ();

    //## Destructor (specified)
      //## Operation: ~ClCommCloseConnectionAction%955613877
      ~ClCommCloseConnectionAction ();


    //## Other Operations (specified)
      //## Operation: Execute%955613878; C++
      m4return_t Execute ();

      //## Operation: PrepareTask%966496756; C++
      m4return_t PrepareTask (m4handle_t ai_hConnectionID, m4tliid_t  ai_TLIID, ClCommProtocol *  ai_pProtocol);

    // Additional Public Declarations
      //## begin ClCommCloseConnectionAction%38F45B8F01C8.public preserve=yes
      //## end ClCommCloseConnectionAction%38F45B8F01C8.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommCloseConnectionAction%38F45B8F01C8.protected preserve=yes
      //## end ClCommCloseConnectionAction%38F45B8F01C8.protected

  private:
    // Additional Private Declarations
      //## begin ClCommCloseConnectionAction%38F45B8F01C8.private preserve=yes
      //## end ClCommCloseConnectionAction%38F45B8F01C8.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommCloseConnectionAction%38F45B8F01C8.implementation preserve=yes
      //## end ClCommCloseConnectionAction%38F45B8F01C8.implementation

};

//## begin ClCommCloseConnectionAction%38F45B8F01C8.postscript preserve=yes
//## end ClCommCloseConnectionAction%38F45B8F01C8.postscript

//## begin ClCommShutDownAction%3923F7E3002B.preface preserve=yes
//## end ClCommShutDownAction%3923F7E3002B.preface

//## Class: ClCommShutDownAction%3923F7E3002B
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommShutDownAction : public ClCommAction  //## Inherits: <unnamed>%3923F7FA0331
{
  //## begin ClCommShutDownAction%3923F7E3002B.initialDeclarations preserve=yes
  //## end ClCommShutDownAction%3923F7E3002B.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommShutDownAction%958646099
      ClCommShutDownAction ();

    //## Destructor (specified)
      //## Operation: ~ClCommShutDownAction%958646100
      ~ClCommShutDownAction ();


    //## Other Operations (specified)
      //## Operation: Execute%958646101; C++
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClCommShutDownAction%3923F7E3002B.public preserve=yes
	  m4bool_t m_bFirstShutdownStep;
      //## end ClCommShutDownAction%3923F7E3002B.public
  protected:
    // Additional Protected Declarations
      //## begin ClCommShutDownAction%3923F7E3002B.protected preserve=yes
      //## end ClCommShutDownAction%3923F7E3002B.protected

  private:
    // Additional Private Declarations
      //## begin ClCommShutDownAction%3923F7E3002B.private preserve=yes
      //## end ClCommShutDownAction%3923F7E3002B.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommShutDownAction%3923F7E3002B.implementation preserve=yes
      //## end ClCommShutDownAction%3923F7E3002B.implementation

};


class ClCommSetLangAction : public ClCommAction
{
  public:
      ClCommSetLangAction () ;

      ~ClCommSetLangAction () ;

	  m4return_t Execute () ;
};

//## begin ClCommShutDownAction%3923F7E3002B.postscript preserve=yes
//## end ClCommShutDownAction%3923F7E3002B.postscript

// Class ClCommExternalClock 

// Class ClCommAction 

// Class ClCommStartReadingAction 

// Class ClCommReadedAction 

// Class ClCommStopReadingAction 

// Class ClCommWriteAction 

// Class ClCommWrittenAction 

// Class ClCommStopWrittingAction 

// Class ClCommFindFreeConnAction 

// Class ClCommStartAcceptingAction 

// Class ClCommStopAcceptingAction 

// Class ClCommOpenConnectionAction 

// Class ClCommCloseConnectionAction 

// Class ClCommShutDownAction 

//## begin module%38DB63340186.epilog preserve=yes
class ClCommRemainReadedAction : public ClCommAction  
{
  public:
      ClCommRemainReadedAction ();
      m4return_t Execute ();
};
//## end module%38DB63340186.epilog


#endif

//## begin module%34267D070177.cm preserve=no
//## end module%34267D070177.cm

//## begin module%34267D070177.cp preserve=no
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
//## end module%34267D070177.cp

//## Module: ClAdminFuncs%34267D070177; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\inc\cladminfuncs.hpp

#ifndef cladminfuncs_h
#define cladminfuncs_h 1

//## begin module%34267D070177.additionalIncludes preserve=no
//## end module%34267D070177.additionalIncludes

//## begin module%34267D070177.includes preserve=yes
#include <exefactory.hpp>
#include <clcommand.hpp>
#include <fifoq.hpp>

class ClCSSessManager;
class ClServiceInfo;
class ClConnection;



//## begin module%34267D070177.declarations preserve=no
//## end module%34267D070177.declarations

//## begin module%34267D070177.additionalDeclarations preserve=yes
//## end module%34267D070177.additionalDeclarations


//## begin ClCreateService%3426525D0230.preface preserve=yes
//## end ClCreateService%3426525D0230.preface

//## Class: ClCreateService%3426525D0230
//	Creates first the service queue and then the service, inserting it into the
//	services dictionary
//## Category: LocalMonitor%379EC99A01CC
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3447844B0073;ClLauncherInterface { -> F}
//## Uses: <unnamed>%344B5CCA03D1;ClServiceInfo { -> F}
//## Uses: <unnamed>%3749822B00C6;ClOutputQ { -> }

class ClCreateService : public ClCommand  //## Inherits: <unnamed>%342652720367
{
  //## begin ClCreateService%3426525D0230.initialDeclarations preserve=yes
  //## end ClCreateService%3426525D0230.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCreateService%875000095
      //	Default constructor
      ClCreateService (void* ai_manager = NULL);

    //## Destructor (specified)
      //## Operation: ~ClCreateService%875000096
      //	Destructor.
      ~ClCreateService ();


    //## Other Operations (specified)
      //## Operation: Execute%878114635
      //	The method that execute the action for create a service.
      m4return_t Execute (void*& ao_oReturn, ClCmdArguments* ai_Arguments = NULL);

    // Additional Public Declarations
      //## begin ClCreateService%3426525D0230.public preserve=yes
      //## end ClCreateService%3426525D0230.public

  protected:
    // Additional Protected Declarations
      //## begin ClCreateService%3426525D0230.protected preserve=yes
      //## end ClCreateService%3426525D0230.protected

  private:
    // Additional Private Declarations
      //## begin ClCreateService%3426525D0230.private preserve=yes
      //## end ClCreateService%3426525D0230.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCreateService%3426525D0230.implementation preserve=yes
      //## end ClCreateService%3426525D0230.implementation

};

//## begin ClCreateService%3426525D0230.postscript preserve=yes
//## end ClCreateService%3426525D0230.postscript

//## begin ClNewConnection%3428E1EF032F.preface preserve=yes
//## end ClNewConnection%3428E1EF032F.preface

//## Class: ClNewConnection%3428E1EF032F
//	This object process the acept received connection requests in an object Cl
//	Connection.
//	The NewConnectionmethod must make a request object and put it into the
//	control queue.
//	When the connection manager process the request call the LocalCheckEnd
//	method giving to it the connection object
//## Category: LocalMonitor%379EC99A01CC
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%344B4FE601F5;ClRequest { -> F}
//## Uses: <unnamed>%344B5C0B00B5;ClConnection { -> F}
//## Uses: <unnamed>%3469EFD3036A;ClConnectPDU { -> F}
//## Uses: <unnamed>%34CCF654017E;ClUSSession { -> F}

class ClNewConnection : public ClCommand  //## Inherits: <unnamed>%3428E2410279
{
  //## begin ClNewConnection%3428E1EF032F.initialDeclarations preserve=yes
  //## end ClNewConnection%3428E1EF032F.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClNewConnection%875095490
      //	Constructor
      ClNewConnection (void* ai_manager = NULL);

    //## Destructor (specified)
      //## Operation: ~ClNewConnection%875095491
      //	Destructor
      ~ClNewConnection ();


    //## Other Operations (specified)
      //## Operation: Execute%878114640
      //	This method has the main body of the action of accept the connection and
      //	asign to it the other element it will need to be satisfected such as the
      //	receiver
      m4return_t Execute (void*& ao_oReturn, ClCmdArguments* ai_Arguments = NULL);

    // Additional Public Declarations
      //## begin ClNewConnection%3428E1EF032F.public preserve=yes
      //## end ClNewConnection%3428E1EF032F.public

  protected:
    // Additional Protected Declarations
      //## begin ClNewConnection%3428E1EF032F.protected preserve=yes
      //## end ClNewConnection%3428E1EF032F.protected

  private:
    // Additional Private Declarations
      //## begin ClNewConnection%3428E1EF032F.private preserve=yes
      //## end ClNewConnection%3428E1EF032F.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClNewConnection%3428E1EF032F.implementation preserve=yes
      //## end ClNewConnection%3428E1EF032F.implementation

};

//## begin ClNewConnection%3428E1EF032F.postscript preserve=yes
//## end ClNewConnection%3428E1EF032F.postscript

//## begin ClDownConnection%3428E1FF0075.preface preserve=yes
//## end ClDownConnection%3428E1FF0075.preface

//## Class: ClDownConnection%3428E1FF0075
//	Administrative command class  for shut down one connection in the server.
//	The connection is not deleted but is put into another list of connections
//	pendient to be deleted.
//## Category: LocalMonitor%379EC99A01CC
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%36A478560033;ClUSUserInterface { -> F}

class ClDownConnection : public ClCommand  //## Inherits: <unnamed>%3428E24401E7
{
  //## begin ClDownConnection%3428E1FF0075.initialDeclarations preserve=yes
  //## end ClDownConnection%3428E1FF0075.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClDownConnection%875095494
      //	Constructor
      ClDownConnection (void* ai_manager = NULL);

    //## Destructor (specified)
      //## Operation: ~ClDownConnection%875095495
      //	Destructor
      ~ClDownConnection ();


    //## Other Operations (specified)
      //## Operation: Execute%878114641
      //	The metod that is realy the hold class
      m4return_t Execute (void*& ao_oReturn, ClCmdArguments* ai_Arguments = NULL);

    // Additional Public Declarations
      //## begin ClDownConnection%3428E1FF0075.public preserve=yes
      //## end ClDownConnection%3428E1FF0075.public

  protected:
    // Additional Protected Declarations
      //## begin ClDownConnection%3428E1FF0075.protected preserve=yes
      //## end ClDownConnection%3428E1FF0075.protected

  private:
    // Additional Private Declarations
      //## begin ClDownConnection%3428E1FF0075.private preserve=yes
      //## end ClDownConnection%3428E1FF0075.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDownConnection%3428E1FF0075.implementation preserve=yes
      //## end ClDownConnection%3428E1FF0075.implementation

};

//## begin ClDownConnection%3428E1FF0075.postscript preserve=yes
//## end ClDownConnection%3428E1FF0075.postscript

//## begin ClShutDownService%343B72EB03C3.preface preserve=yes
//## end ClShutDownService%343B72EB03C3.preface

//## Class: ClShutDownService%343B72EB03C3
//	This class pretend to be an interface for an administration command, in
//	particular for shut down a service.
//## Category: LocalMonitor%379EC99A01CC
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClShutDownService : public ClCommand  //## Inherits: <unnamed>%343B734E0126
{
  //## begin ClShutDownService%343B72EB03C3.initialDeclarations preserve=yes
  //## end ClShutDownService%343B72EB03C3.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClShutDownService%877008205
      ClShutDownService (void* ai_manager = NULL);

    //## Destructor (specified)
      //## Operation: ~ClShutDownService%877008206
      ~ClShutDownService ();


    //## Other Operations (specified)
      //## Operation: Execute%878114643
      m4return_t Execute (void*& ao_oReturn, ClCmdArguments* ai_Arguments = NULL);

    // Additional Public Declarations
      //## begin ClShutDownService%343B72EB03C3.public preserve=yes
      //## end ClShutDownService%343B72EB03C3.public

  protected:
    // Additional Protected Declarations
      //## begin ClShutDownService%343B72EB03C3.protected preserve=yes
      //## end ClShutDownService%343B72EB03C3.protected

  private:
    // Additional Private Declarations
      //## begin ClShutDownService%343B72EB03C3.private preserve=yes
      //## end ClShutDownService%343B72EB03C3.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClShutDownService%343B72EB03C3.implementation preserve=yes
      //## end ClShutDownService%343B72EB03C3.implementation

};

//## begin ClShutDownService%343B72EB03C3.postscript preserve=yes
//## end ClShutDownService%343B72EB03C3.postscript

//## begin ClCreateCM%344614680321.preface preserve=yes
//## end ClCreateCM%344614680321.preface

//## Class: ClCreateCM%344614680321
//	The class of Create connection manager command. Each time the Local Monitor
//	has to create a new instance of a connection manager, it calls this class
//	execute method
//## Category: LocalMonitor%379EC99A01CC
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%344783EB0223;ClCSSessManager { -> F}

class ClCreateCM : public ClCommand  //## Inherits: <unnamed>%3446152603CE
{
  //## begin ClCreateCM%344614680321.initialDeclarations preserve=yes
  //## end ClCreateCM%344614680321.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCreateCM%877008208
      ClCreateCM (void* ai_manager);

    //## Destructor (specified)
      //## Operation: ~ClCreateCM%877008209
      ~ClCreateCM ();


    //## Other Operations (specified)
      //## Operation: Execute%878114644
      m4return_t Execute (void*& ao_oReturn, ClCmdArguments* ai_Arguments = NULL);

    // Additional Public Declarations
      //## begin ClCreateCM%344614680321.public preserve=yes
      //## end ClCreateCM%344614680321.public

  protected:
    // Additional Protected Declarations
      //## begin ClCreateCM%344614680321.protected preserve=yes
      //## end ClCreateCM%344614680321.protected

  private:
    // Additional Private Declarations
      //## begin ClCreateCM%344614680321.private preserve=yes
      //## end ClCreateCM%344614680321.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCreateCM%344614680321.implementation preserve=yes
      //## end ClCreateCM%344614680321.implementation

};

//## begin ClCreateCM%344614680321.postscript preserve=yes
//## end ClCreateCM%344614680321.postscript

//## begin ClCreateLauncher%34562B45017A.preface preserve=yes
//## end ClCreateLauncher%34562B45017A.preface

//## Class: ClCreateLauncher%34562B45017A
//	This object is a command object that creates a launcher thread and then puts
//	it into the launcher objects list attached to an existing service queue
//## Category: LocalMonitor%379EC99A01CC
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34BB55F202CE;ClExeFactory { -> F}

class ClCreateLauncher : public ClCommand  //## Inherits: <unnamed>%34562B62029E
{
  //## begin ClCreateLauncher%34562B45017A.initialDeclarations preserve=yes
  //## end ClCreateLauncher%34562B45017A.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCreateLauncher%878029383
      ClCreateLauncher (void* ai_manager);

    //## Destructor (specified)
      //## Operation: ~ClCreateLauncher%878029384
      ~ClCreateLauncher ();


    //## Other Operations (specified)
      //## Operation: Execute%878114645
      m4return_t Execute (void*& ao_oReturn, ClCmdArguments* ai_Arguments = NULL);

    // Additional Public Declarations
      //## begin ClCreateLauncher%34562B45017A.public preserve=yes
      //## end ClCreateLauncher%34562B45017A.public

  protected:
    // Additional Protected Declarations
      //## begin ClCreateLauncher%34562B45017A.protected preserve=yes
      //## end ClCreateLauncher%34562B45017A.protected

  private:
    // Additional Private Declarations
      //## begin ClCreateLauncher%34562B45017A.private preserve=yes
      //## end ClCreateLauncher%34562B45017A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCreateLauncher%34562B45017A.implementation preserve=yes
      //## end ClCreateLauncher%34562B45017A.implementation

};

//## begin ClCreateLauncher%34562B45017A.postscript preserve=yes
//## end ClCreateLauncher%34562B45017A.postscript

//## begin ClShutDownCM%3457003A0003.preface preserve=yes
//## end ClShutDownCM%3457003A0003.preface

//## Class: ClShutDownCM%3457003A0003
//	This class shuts down one instance of a connection manager removing it from
//	the list of active connection managers. The connection manager that is
//	removed is the first in the list of them
//## Category: LocalMonitor%379EC99A01CC
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClShutDownCM : public ClCommand  //## Inherits: <unnamed>%345703310195
{
  //## begin ClShutDownCM%3457003A0003.initialDeclarations preserve=yes
  //## end ClShutDownCM%3457003A0003.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClShutDownCM%878114646
      ClShutDownCM (void* ai_manager);

    //## Destructor (specified)
      //## Operation: ~ClShutDownCM%878114647
      ~ClShutDownCM ();


    //## Other Operations (specified)
      //## Operation: Execute%878114648
      m4return_t Execute (void*& ao_oReturn, ClCmdArguments* ai_Arguments = NULL);

    // Additional Public Declarations
      //## begin ClShutDownCM%3457003A0003.public preserve=yes
      //## end ClShutDownCM%3457003A0003.public

  protected:
    // Additional Protected Declarations
      //## begin ClShutDownCM%3457003A0003.protected preserve=yes
      //## end ClShutDownCM%3457003A0003.protected

  private:
    // Additional Private Declarations
      //## begin ClShutDownCM%3457003A0003.private preserve=yes
      //## end ClShutDownCM%3457003A0003.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClShutDownCM%3457003A0003.implementation preserve=yes
      //## end ClShutDownCM%3457003A0003.implementation

};

//## begin ClShutDownCM%3457003A0003.postscript preserve=yes
//## end ClShutDownCM%3457003A0003.postscript

//## begin ClShutDownLauncher%345701E201A8.preface preserve=yes
//## end ClShutDownLauncher%345701E201A8.preface

//## Class: ClShutDownLauncher%345701E201A8
//	Shuts down one launcher, the first one in the list of active schedulers in a
//	determined group of a determined service. Is not important witch launcher is
//	shuted down because all of the launchers in each list are equals about the
//	queue from with are reading and about the processes they are making
//## Category: LocalMonitor%379EC99A01CC
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClShutDownLauncher : public ClCommand  //## Inherits: <unnamed>%3457030E0267
{
  //## begin ClShutDownLauncher%345701E201A8.initialDeclarations preserve=yes
  //## end ClShutDownLauncher%345701E201A8.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClShutDownLauncher%878114658
      ClShutDownLauncher (void* ai_manager);

    //## Destructor (specified)
      //## Operation: ~ClShutDownLauncher%878114659
      ~ClShutDownLauncher ();


    //## Other Operations (specified)
      //## Operation: Execute%878114660
      m4return_t Execute (void*& ao_oReturn, ClCmdArguments* ai_Arguments = NULL);

    // Additional Public Declarations
      //## begin ClShutDownLauncher%345701E201A8.public preserve=yes
      //## end ClShutDownLauncher%345701E201A8.public

  protected:
    // Additional Protected Declarations
      //## begin ClShutDownLauncher%345701E201A8.protected preserve=yes
      //## end ClShutDownLauncher%345701E201A8.protected

  private:
    // Additional Private Declarations
      //## begin ClShutDownLauncher%345701E201A8.private preserve=yes
      //## end ClShutDownLauncher%345701E201A8.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClShutDownLauncher%345701E201A8.implementation preserve=yes
      //## end ClShutDownLauncher%345701E201A8.implementation

};

//## begin ClShutDownLauncher%345701E201A8.postscript preserve=yes
//## end ClShutDownLauncher%345701E201A8.postscript

//## begin ClCreateAdministrator%345787EE0242.preface preserve=yes
//## end ClCreateAdministrator%345787EE0242.preface

//## Class: ClCreateAdministrator%345787EE0242
//	To create a new instance of Administrator the Local monitor uses this class
//	methods.
//## Category: LocalMonitor%379EC99A01CC
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%347EF8A2000A; { -> F}
//## Uses: <unnamed>%36A47A3F0176;ClSystem { -> F}
//## Uses: <unnamed>%377BA26F0352;ClNavigatorSS { -> F}

class ClCreateAdministrator : public ClCommand  //## Inherits: <unnamed>%3457886E02FA
{
  //## begin ClCreateAdministrator%345787EE0242.initialDeclarations preserve=yes
  //## end ClCreateAdministrator%345787EE0242.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCreateAdministrator%878114670
      ClCreateAdministrator (void* ai_manager);

    //## Destructor (specified)
      //## Operation: ~ClCreateAdministrator%878114671
      ~ClCreateAdministrator ();


    //## Other Operations (specified)
      //## Operation: Execute%878114672
      m4return_t Execute (void*& ao_oReturn, ClCmdArguments* ai_Arguments = NULL);

    // Additional Public Declarations
      //## begin ClCreateAdministrator%345787EE0242.public preserve=yes
      //## end ClCreateAdministrator%345787EE0242.public

  protected:
    // Additional Protected Declarations
      //## begin ClCreateAdministrator%345787EE0242.protected preserve=yes
      //## end ClCreateAdministrator%345787EE0242.protected

  private:
    // Additional Private Declarations
      //## begin ClCreateAdministrator%345787EE0242.private preserve=yes
      //## end ClCreateAdministrator%345787EE0242.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCreateAdministrator%345787EE0242.implementation preserve=yes
      //## end ClCreateAdministrator%345787EE0242.implementation

};

//## begin ClCreateAdministrator%345787EE0242.postscript preserve=yes
//## end ClCreateAdministrator%345787EE0242.postscript

//## begin ClShutDownAdministrator%34578814011A.preface preserve=yes
//## end ClShutDownAdministrator%34578814011A.preface

//## Class: ClShutDownAdministrator%34578814011A
//	Is the oposite class of ClCreateAdministrator, so this class shuts down one
//	instance of Administrator.
//## Category: LocalMonitor%379EC99A01CC
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClShutDownAdministrator : public ClCommand  //## Inherits: <unnamed>%3457887900C5
{
  //## begin ClShutDownAdministrator%34578814011A.initialDeclarations preserve=yes
  //## end ClShutDownAdministrator%34578814011A.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClShutDownAdministrator%878114673
      ClShutDownAdministrator (void* ai_manager);

    //## Destructor (specified)
      //## Operation: ~ClShutDownAdministrator%878114674
      ~ClShutDownAdministrator ();


    //## Other Operations (specified)
      //## Operation: Execute%878114675
      m4return_t Execute (void*& ao_oReturn, ClCmdArguments* ai_Arguments = NULL);

    // Additional Public Declarations
      //## begin ClShutDownAdministrator%34578814011A.public preserve=yes
      //## end ClShutDownAdministrator%34578814011A.public

  protected:
    // Additional Protected Declarations
      //## begin ClShutDownAdministrator%34578814011A.protected preserve=yes
      //## end ClShutDownAdministrator%34578814011A.protected

  private:
    // Additional Private Declarations
      //## begin ClShutDownAdministrator%34578814011A.private preserve=yes
      //## end ClShutDownAdministrator%34578814011A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClShutDownAdministrator%34578814011A.implementation preserve=yes
      //## end ClShutDownAdministrator%34578814011A.implementation

};

//## begin ClShutDownAdministrator%34578814011A.postscript preserve=yes
//## end ClShutDownAdministrator%34578814011A.postscript

// Class ClCreateService 

// Class ClNewConnection 

// Class ClDownConnection 

// Class ClShutDownService 

// Class ClCreateCM 

// Class ClCreateLauncher 

// Class ClShutDownCM 

// Class ClShutDownLauncher 

// Class ClCreateAdministrator 

// Class ClShutDownAdministrator 

//## begin module%34267D070177.epilog preserve=yes
//## end module%34267D070177.epilog


#endif

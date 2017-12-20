//## begin module.cm preserve=no
//## end module.cm

//## begin module.cp preserve=no
//	=========================================================
//	=====================
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
//	=========================================================
//	=====================
//## end module.cp

//## Module: invoker; Package specification
//## Subsystem: CPM4ComSrv::version
//## Source file: X:\M4REL\M4SOURCE\3x_work\Core\communication\working\oscar\M4ComSrv\version\inc\invoker.hpp

#ifndef invoker_h
#define invoker_h 1

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
//## end module.includes

// ClCmdArguments
#include "clcmdarguments.hpp"
// cmdtable
#include "cmdtable.hpp"
// Command
#include "clcommand.hpp"
//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes
//## end module.additionalDeclarations


//## Class: ClInvoker
//	Invoker is a class for create, execute and delete
//	commands.
//## Category: M4ComSrv::LocalMonitor
//## Subsystem: CPM4ComSrv::version
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>; ClCmdArguments { -> }
//## Uses: <unnamed>; ClCommand { -> }

class ClInvoker 
{
  //## begin ClInvoker.initialDeclarations preserve=yes
  //## end ClInvoker.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClInvoker%-731737672; C++
      ClInvoker ();

      //## Operation: ClInvoker%1651197723; C++
      ClInvoker (void* manager);

    //## Destructor (specified)
      //## Operation: ~ClInvoker%-1270919577; C++
      ~ClInvoker ();


    //## Other Operations (specified)
      //## Operation: CreateCommand%-2145844697; C++
      //	Makes a command with an identification and registers it
      //	into a command container.
      m4return_t CreateCommand (m4char_t ai_iCommand, void* ai_manager = NULL);

      //## Operation: DeleteCommand%-1978394898; C++
      m4return_t DeleteCommand (m4char_t ai_iCommand);

      //## Operation: SetManager%495790292; C++
      //	m4int16_t	SetInterpreter( ClInterpreter *interpreter);
      //	m4int16_t	SetCommands( ClCommandTable	*commands );
      //	m4int16_t	SetProgram( M4ClString program );
      m4return_t SetManager (void* manager);

      //## Operation: ExecuteCommand%877335854
      //	Look for the command i the contatiner and then executes
      //	it
      m4return_t ExecuteCommand (m4char_t ai_iCommand, void *&ao_poReturn, ClCmdArguments *ai_Arguments = NULL);

    // Data Members for Associations

      //## Association: M4ComSrv::LocalMonitor::<unnamed>%342262C90355
      //## Role: ClInvoker::<m_pClLocalMonitor>
      //## begin ClInvoker::<m_pClLocalMonitor>.role preserve=no  public: ClLocalMonitor {1 -> 0RHGA}
      //## end ClInvoker::<m_pClLocalMonitor>.role

      //## Association: M4ComSrv::LocalMonitor::<unnamed>%344B8201015A
      //## Role: ClInvoker::m_commands
      //## begin ClInvoker::m_commands.role preserve=no  public: ClCommandTable { -> RHAN}
      ClCommandTable *m_commands;
      //## end ClInvoker::m_commands.role

    // Additional Public Declarations
      //## begin ClInvoker.public preserve=yes
      //## end ClInvoker.public

  protected:
    // Additional Protected Declarations
      //## begin ClInvoker.protected preserve=yes
      //## end ClInvoker.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_manager
      //## begin ClInvoker::m_manager.attr preserve=no  private: void* {VA} 
      void* m_manager;
      //## end ClInvoker::m_manager.attr

    // Additional Private Declarations
      //## begin ClInvoker.private preserve=yes
      //## end ClInvoker.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClInvoker.implementation preserve=yes
      //## end ClInvoker.implementation

};

//## begin ClInvoker.postscript preserve=yes
//## end ClInvoker.postscript



// Class ClInvoker 





//## begin module.epilog preserve=yes
//## end module.epilog
#endif

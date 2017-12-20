//## begin module%344B8D060369.cm preserve=no
//## end module%344B8D060369.cm

//## begin module%344B8D060369.cp preserve=no
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
//## end module%344B8D060369.cp

//## Module: cmdtable%344B8D060369; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\version\inc\cmdtable.hpp

#ifndef cmdtable_h
#define cmdtable_h 1

//## begin module%344B8D060369.additionalIncludes preserve=no
//## end module%344B8D060369.additionalIncludes

//## begin module%344B8D060369.includes preserve=yes
#ifdef _KCC
using std::map;
using std::less;
#endif
//## end module%344B8D060369.includes

#include "m4stl.hpp"
// m4types
#include <m4types.hpp>
// syncronization
#include <m4syncronization.hpp>
// Command
#include <clcommand.hpp>


//## begin module%344B8D060369.declarations preserve=no
#define CREATERECEIVER 0
#define CREATEACCEPTOR 1
#define CREATESCHEDULER 2
#define CREATESENDER 3
#define CREATECONNMANAGER 4
#define CREATEDISTRIBUTOR 5
#define CREATELAUNCHER 6
#define CREATESERVICE 7
#define CREATEADMINISTRATOR 8
#define NEWCONNECTION 9
#define DOWNCONNECTION 10
#define LOCALCHECKEND 11
#define SHUTDOWNRECEIVER 12
#define SHUTDOWNACCEPTOR 13
#define SHUTDOWNSCHEDULER 14
#define SHUTDOWNSENDER 15
#define SHUTDOWNCONNMANAGER 16
#define SHUTDOWNDISTRIBUTOR 17
#define SHUTDOWNLAUNCHER 18
#define SHUTDOWNSERVICE 19
#define CREATEADMINACCEPTOR 20
#define SHUTDOWNADMINISTRATOR 21
//## end module%344B8D060369.declarations

//## begin module%344B8D060369.additionalDeclarations preserve=yes
//## end module%344B8D060369.additionalDeclarations


//## begin COMMANDMAP%341F92230347.preface preserve=yes
//## end COMMANDMAP%341F92230347.preface

//## Class: COMMANDMAP%341F92230347; Instantiated Class
//	The base class for commands containing. its is an STL implementation of a
//	map type
//## Category: LocalMonitor%379EC99A01CC
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34449AA701C3;ClCommand { -> F}

typedef map< m4char_t , class ClCommand*, less< m4char_t > > COMMANDMAP;

//## begin COMMANDMAP%341F92230347.postscript preserve=yes
//## end COMMANDMAP%341F92230347.postscript

//## begin ClCommandTable%341F9222023D.preface preserve=yes
//## end ClCommandTable%341F9222023D.preface

//## Class: ClCommandTable%341F9222023D
//	This class is a contaoner for command classes, each command has a code and
//	is stored in the same list
//## Category: LocalMonitor%379EC99A01CC
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34449AA7004C;m4int16_t { -> }
//## Uses: <unnamed>%34449AA7004E; { -> }
//## Uses: <unnamed>%35F4EAB3009A;deque { -> }

class ClCommandTable : public ClWriterReaders  //## Inherits: <unnamed>%35F41157002A
{
  //## begin ClCommandTable%341F9222023D.initialDeclarations preserve=yes
  //## end ClCommandTable%341F9222023D.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommandTable%1961923900; C++
      //	Default constructor
      ClCommandTable ();

    //## Destructor (specified)
      //## Operation: ~ClCommandTable%1431135431; C++
      ~ClCommandTable ();


    //## Other Operations (specified)
      //## Operation: CreateCommand%-363128453; C++
      //	Creates a new object command and stores it in the list of commands
      m4return_t CreateCommand (void* manager, m4char_t ai_iCommand);

      //## Operation: DeleteCommand%1567923947; C++
      //	Removes a command from the list of commands and destry the object of this
      //	command class
      m4return_t DeleteCommand (m4char_t ai_command);

      //## Operation: GetCommand%-351209901; C++
      //	Return the command that matches to the code passed as argument
      ClCommand* GetCommand (m4char_t ai_iCommand);

    // Data Members for Associations

      //## Association: LocalMonitor::<unnamed>%344B8A6A02DC
      //## Role: ClCommandTable::m_commands%344B8A6B01ED
      //## begin ClCommandTable::m_commands%344B8A6B01ED.role preserve=no  public: COMMANDMAP { -> VHAN}
      COMMANDMAP m_commands;
      //## end ClCommandTable::m_commands%344B8A6B01ED.role

      //## Association: LocalMonitor::<unnamed>%379ECB0901C1
      //## Role: ClCommandTable::m_poCommands%379ECB0B02B5
      //## begin ClCommandTable::m_poCommands%379ECB0B02B5.role preserve=no  public: ClCommand { -> UHAN}
      deque<ClCommand> m_poCommands;
      //## end ClCommandTable::m_poCommands%379ECB0B02B5.role

    // Additional Public Declarations
      //## begin ClCommandTable%341F9222023D.public preserve=yes
      //## end ClCommandTable%341F9222023D.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommandTable%341F9222023D.protected preserve=yes
      //## end ClCommandTable%341F9222023D.protected

  private:
    // Additional Private Declarations
      //## begin ClCommandTable%341F9222023D.private preserve=yes
      //## end ClCommandTable%341F9222023D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommandTable%341F9222023D.implementation preserve=yes
      //## end ClCommandTable%341F9222023D.implementation

};

//## begin ClCommandTable%341F9222023D.postscript preserve=yes
//## end ClCommandTable%341F9222023D.postscript

// Class ClCommandTable 

//## begin module%344B8D060369.epilog preserve=yes
//## end module%344B8D060369.epilog


#endif

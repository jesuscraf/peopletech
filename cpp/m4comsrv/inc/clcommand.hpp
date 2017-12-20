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

//## Module: Command; Package specification
//## Subsystem: CPM4ComSrv::version
//## Source file: X:\m4rel\m4source\3x_work\core\communication\working\oscar\M4ComSrv\version\inc\clcommand.hpp

#ifndef command_h
#define command_h 1

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
//## end module.includes

// m4string
#include <m4string.hpp>
// m4types
#include <m4types.hpp>

class ClCmdArguments;

//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes
//## end module.additionalDeclarations


//## Class: ClCommand
//## Category: M4ComSrv::LocalMonitor
//## Subsystem: CPM4ComSrv::version
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>; void* { -> }
//## Uses: <unnamed>; m4int16_t { -> }
//## Uses: <unnamed>; M4ClString { -> }
//## Uses: <unnamed>; ClCmdArguments { -> F}

class ClCommand 
{
  //## begin ClCommand.initialDeclarations preserve=yes
  //## end ClCommand.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommand%918762194; C++
      ClCommand ();

      //## Operation: ClCommand%1493720844; C++
      ClCommand (void* manager);

    //## Destructor (specified)
      //## Operation: ~ClCommand%541022595; C++
      virtual ~ClCommand ();


    //## Other Operations (specified)
      //## Operation: SetManager%-1393546573; C++
      m4int16_t SetManager (void* manager);

      //## Operation: Execute%-1378648674; C++
      virtual m4return_t Execute (void*& exit, ClCmdArguments *args = NULL);

    // Data Members for Associations

      //## Association: M4AdminClient::Parser::<unnamed>%3494FE610320
      //## Role: ClCommand::<m_pClSentenceFrame>
      //## begin ClCommand::<m_pClSentenceFrame>.role preserve=no  public: ClSentenceFrame {1 -> RHA}
      //## end ClCommand::<m_pClSentenceFrame>.role

      //## Association: M4AdminClient::Parser::<unnamed>%3494FE610316
      //## Role: ClCommand::<m_pClCommandTable>
      //## begin ClCommand::<m_pClCommandTable>.role preserve=no  public: ClCommandTable {0..n -> RHGA}
      //## end ClCommand::<m_pClCommandTable>.role

    // Additional Public Declarations
      //## begin ClCommand.public preserve=yes
      //## end ClCommand.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_manager
      //## begin ClCommand::m_manager.attr preserve=no  protected: void* {VA} 
      void* m_manager;
      //## end ClCommand::m_manager.attr

    // Additional Protected Declarations
      //## begin ClCommand.protected preserve=yes
      //## end ClCommand.protected

  private:
    // Additional Private Declarations
      //## begin ClCommand.private preserve=yes
      //## end ClCommand.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClCommand.implementation preserve=yes
      //## end ClCommand.implementation

};

//## begin ClCommand.postscript preserve=yes
//## end ClCommand.postscript



// Class ClCommand 





//## begin module.epilog preserve=yes
//## end module.epilog
#endif

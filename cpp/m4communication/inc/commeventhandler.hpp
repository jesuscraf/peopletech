//## begin module%3961A47702BB.cm preserve=no
//## end module%3961A47702BB.cm

//## begin module%3961A47702BB.cp preserve=no
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
//## end module%3961A47702BB.cp

//## Module: commeventhandler%3961A47702BB; Package specification
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Source file: e:\mybuild\m4communication\inc\commeventhandler.hpp

#ifndef commeventhandler_h
#define commeventhandler_h 1

//## begin module%3961A47702BB.additionalIncludes preserve=no
//## end module%3961A47702BB.additionalIncludes

//## begin module%3961A47702BB.includes preserve=yes
//## end module%3961A47702BB.includes

// CommComun
#include <commcomun.hpp>

class ClCommTask;

//## begin module%3961A47702BB.declarations preserve=no
//## end module%3961A47702BB.declarations

//## begin module%3961A47702BB.additionalDeclarations preserve=yes
//## end module%3961A47702BB.additionalDeclarations


//## begin ClCommEventHandler%38EDC69303DF.preface preserve=yes
//## end ClCommEventHandler%38EDC69303DF.preface

//## Class: ClCommEventHandler%38EDC69303DF
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommEventHandler : public ClIOEventHandler  //## Inherits: <unnamed>%38EDC69E020E
{
  //## begin ClCommEventHandler%38EDC69303DF.initialDeclarations preserve=yes
  //## end ClCommEventHandler%38EDC69303DF.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: ProccessRead%955107286; C++
      m4return_t ProccessRead (m4tliid_t ai_Identifier, int ai_Len);

      //## Operation: ProccessWrite%955107287; C++
      m4return_t ProccessWrite (m4tliid_t ai_Identifier, m4return_t ai_Return);

      //## Operation: ProccessOpenPort%955107288; C++
      m4return_t ProccessOpenPort (m4tliid_t ai_Identifier, m4return_t ai_Success);

      //## Operation: ProccessClosePort%955107289; C++
      m4return_t ProccessClosePort (m4tliid_t ai_Identifier, m4return_t ai_Success);

      //## Operation: ProccessBeginRead%955107290; C++
      m4return_t ProccessBeginRead (m4tliid_t ai_Identifier, m4return_t ai_Success);

      //## Operation: ProccessStopReading%955107291; C++
      m4return_t ProccessStopReading (m4tliid_t ai_Identifier, m4return_t ai_Success);

      //## Operation: ProccessStopWriting%955107292; C++
      m4return_t ProccessStopWriting (m4tliid_t ai_Identifier, m4return_t ai_Success);

      //## Operation: ProccessConnect%955107293; C++
      m4return_t ProccessConnect (m4tliid_t ai_Identifier, m4return_t ai_Success);

      //## Operation: ProccessStopConnection%955107294; C++
      m4return_t ProccessStopConnection (m4tliid_t ai_Identifier, m4return_t ai_Success);

      //## Operation: ProccessNewConnection%955613881; C++
      m4return_t ProccessNewConnection (m4tliid_t ai_Identifier, m4return_t ai_Success);

      //## Operation: ProccessStop%957360601; C++
      m4return_t ProccessStop (m4return_t ai_Success);

      //## Operation: ProccessGetTransferedBytes%958059393; C++
      m4return_t ProccessGetTransferedBytes (m4tliid_t ai_Identifier, size_t ai_Readed, size_t ai_Writed, m4return_t ai_Return);

    // Data Members for Associations

      //## Association: M4Communication::M4COML::<unnamed>%38F5DE610156
      //## Role: ClCommEventHandler::m_poTask%38F5DE620107
      //## begin ClCommEventHandler::m_poTask%38F5DE620107.role preserve=no  public: ClCommTask {1 -> RFHAN}
      ClCommTask *m_poTask;
      //## end ClCommEventHandler::m_poTask%38F5DE620107.role

    // Additional Public Declarations
      //## begin ClCommEventHandler%38EDC69303DF.public preserve=yes
      //## end ClCommEventHandler%38EDC69303DF.public

  protected:

    //## Other Operations (specified)
      //## Operation: UpdateTask%955613888
      m4return_t UpdateTask ();

    // Additional Protected Declarations
      //## begin ClCommEventHandler%38EDC69303DF.protected preserve=yes
      //## end ClCommEventHandler%38EDC69303DF.protected

  private:
    // Additional Private Declarations
      //## begin ClCommEventHandler%38EDC69303DF.private preserve=yes
      //## end ClCommEventHandler%38EDC69303DF.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommEventHandler%38EDC69303DF.implementation preserve=yes
      //## end ClCommEventHandler%38EDC69303DF.implementation

};

//## begin ClCommEventHandler%38EDC69303DF.postscript preserve=yes
//## end ClCommEventHandler%38EDC69303DF.postscript

// Class ClCommEventHandler 

//## begin module%3961A47702BB.epilog preserve=yes
//## end module%3961A47702BB.epilog


#endif

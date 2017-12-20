//## begin module%38D22742039F.cm preserve=no
//## end module%38D22742039F.cm

//## begin module%38D22742039F.cp preserve=no
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
//## end module%38D22742039F.cp

//## Module: CommComun%38D22742039F; Package specification
//## Subsystem: m4transport::comunes::inc%38D2272F02D9
//## Source file: c:\Componentes\m4transport\inc\commcomun.hpp

#ifndef commcomun_h
#define commcomun_h 1

//## begin module%38D22742039F.additionalIncludes preserve=no
//## end module%38D22742039F.additionalIncludes

//## begin module%38D22742039F.includes preserve=yes
#include "m4transport_dll.hpp"
//## end module%38D22742039F.includes

// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
//## begin module%38D22742039F.declarations preserve=no
//## end module%38D22742039F.declarations

//## begin module%38D22742039F.additionalDeclarations preserve=yes
//## end module%38D22742039F.additionalDeclarations


//## begin m4tliid_t%38EDC8A603E2.preface preserve=yes
//## end m4tliid_t%38EDC8A603E2.preface

//## Class: m4tliid_t%38EDC8A603E2
//## Category: m4transport::comunes%38F1B4750198
//## Subsystem: m4transport::comunes::inc%38D2272F02D9
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef void *m4tliid_t;

//## begin m4tliid_t%38EDC8A603E2.postscript preserve=yes
//## end m4tliid_t%38EDC8A603E2.postscript

//## begin ClIOEventHandler%38E30F2A00AD.preface preserve=yes
//## end ClIOEventHandler%38E30F2A00AD.preface

//## Class: ClIOEventHandler%38E30F2A00AD
//## Category: m4transport::comunes%38F1B4750198
//## Subsystem: m4transport::comunes::inc%38D2272F02D9
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%38F1F9C80241;m4tliid_t { -> }

class M4_DECL_M4TRANSPORT ClIOEventHandler 
{
  //## begin ClIOEventHandler%38E30F2A00AD.initialDeclarations preserve=yes
  //## end ClIOEventHandler%38E30F2A00AD.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: ProccessRead%954405690; C++
      virtual m4return_t ProccessRead (m4tliid_t ai_Identifier, int ai_Len) = 0;

      //## Operation: ProccessWrite%954835559; C++
      virtual m4return_t ProccessWrite (m4tliid_t ai_Identifier, m4return_t ai_Return) = 0;

      //## Operation: ProccessOpenPort%954835560; C++
      virtual m4return_t ProccessOpenPort (m4tliid_t ai_Identifier, m4return_t ai_Success) = 0;

      //## Operation: ProccessClosePort%954835561; C++
      virtual m4return_t ProccessClosePort (m4tliid_t ai_Identifier, m4return_t ai_Success) = 0;

      //## Operation: ProccessNewConnection%955436196; C++
      virtual m4return_t ProccessNewConnection (m4tliid_t ai_Identifier, m4return_t ai_Success) = 0;

      //## Operation: ProccessBeginRead%954835562; C++
      virtual m4return_t ProccessBeginRead (m4tliid_t ai_Identifier, m4return_t ai_Success) = 0;

      //## Operation: ProccessStopReading%954835563
      virtual m4return_t ProccessStopReading (m4tliid_t ai_Identifier, m4return_t ai_Success) = 0;

      //## Operation: ProccessStopWriting%954835564; C++
      virtual m4return_t ProccessStopWriting (m4tliid_t ai_Identifier, m4return_t ai_Success) = 0;

      //## Operation: ProccessConnect%954835565
      virtual m4return_t ProccessConnect (m4tliid_t ai_Identifier, m4return_t ai_Success) = 0;

      //## Operation: ProccessStopConnection%954835566; C++
      virtual m4return_t ProccessStopConnection (m4tliid_t ai_Identifier, m4return_t ai_Success) = 0;

      //## Operation: GetBuffer%954835567; C++
      char * GetBuffer ();

      //## Operation: GetBufferSize%954835568; C++
      size_t GetBufferSize ();

      //## Operation: SetBufferSize%954835569; C++
      void SetBufferSize (size_t ai_Size);

      //## Operation: SetBuffer%954835570; C++
      void SetBuffer (char *ai_pBuffer);

      //## Operation: ProccessStop%956912724; C++
      virtual m4return_t ProccessStop (m4return_t ai_Success) = 0;

      //## Operation: ProccessGetTransferedBytes%958059166; C++
      virtual m4return_t ProccessGetTransferedBytes (m4tliid_t ai_Identifier, size_t ai_Readed, size_t ai_Writed, m4return_t ai_Return) = 0;

    // Data Members for Associations

      //## Association: m4transport::comunes::<unnamed>%38F1BE540001
      //## Role: ClIOEventHandler::m_bufferSize%38F1BE5401A6
      //## begin ClIOEventHandler::m_bufferSize%38F1BE5401A6.role preserve=no  public: m4int_t { -> UHAN}
      m4int_t m_bufferSize;
      //## end ClIOEventHandler::m_bufferSize%38F1BE5401A6.role

      //## Association: m4transport::comunes::<unnamed>%38F1BE610263
      //## Role: ClIOEventHandler::m_pcBuffer%38F1BE620001
      //## begin ClIOEventHandler::m_pcBuffer%38F1BE620001.role preserve=no  public: m4char_t { -> RHAN}
      m4char_t *m_pcBuffer;
      //## end ClIOEventHandler::m_pcBuffer%38F1BE620001.role

      //## Association: m4transport::comunes::<unnamed>%38F1BE73016E
      //## Role: ClIOEventHandler::m_return%38F1BE74006B
      //## begin ClIOEventHandler::m_return%38F1BE74006B.role preserve=no  public: m4return_t { -> UHAN}
      m4return_t m_return;
      //## end ClIOEventHandler::m_return%38F1BE74006B.role

    // Additional Public Declarations
      //## begin ClIOEventHandler%38E30F2A00AD.public preserve=yes
      //## end ClIOEventHandler%38E30F2A00AD.public

  protected:
    // Additional Protected Declarations
      //## begin ClIOEventHandler%38E30F2A00AD.protected preserve=yes
      //## end ClIOEventHandler%38E30F2A00AD.protected

  private:
    // Additional Private Declarations
      //## begin ClIOEventHandler%38E30F2A00AD.private preserve=yes
      //## end ClIOEventHandler%38E30F2A00AD.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClIOEventHandler%38E30F2A00AD.implementation preserve=yes
      //## end ClIOEventHandler%38E30F2A00AD.implementation

};

//## begin ClIOEventHandler%38E30F2A00AD.postscript preserve=yes
//## end ClIOEventHandler%38E30F2A00AD.postscript

//## begin ClSSLParams%38DF823F02DC.preface preserve=yes
//## end ClSSLParams%38DF823F02DC.preface

//## Class: ClSSLParams%38DF823F02DC
//## Category: m4transport::comunes%38F1B4750198
//## Subsystem: m4transport::comunes::inc%38D2272F02D9
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClSSLParams 
{
  //## begin ClSSLParams%38DF823F02DC.initialDeclarations preserve=yes
  //## end ClSSLParams%38DF823F02DC.initialDeclarations

  public:
    // Additional Public Declarations
      //## begin ClSSLParams%38DF823F02DC.public preserve=yes
      //## end ClSSLParams%38DF823F02DC.public

  protected:
    // Additional Protected Declarations
      //## begin ClSSLParams%38DF823F02DC.protected preserve=yes
      //## end ClSSLParams%38DF823F02DC.protected

  private:
    // Additional Private Declarations
      //## begin ClSSLParams%38DF823F02DC.private preserve=yes
      //## end ClSSLParams%38DF823F02DC.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSSLParams%38DF823F02DC.implementation preserve=yes
      //## end ClSSLParams%38DF823F02DC.implementation

};

//## begin ClSSLParams%38DF823F02DC.postscript preserve=yes
//## end ClSSLParams%38DF823F02DC.postscript

//## begin ClCommURL%38DF80F10231.preface preserve=yes
//## end ClCommURL%38DF80F10231.preface

//## Class: ClCommURL%38DF80F10231
//## Category: m4transport::comunes%38F1B4750198
//## Subsystem: m4transport::comunes::inc%38D2272F02D9
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%38F1FE9C02C1;m4string_t { -> }

class ClCommURL 
{
  //## begin ClCommURL%38DF80F10231.initialDeclarations preserve=yes
  //## end ClCommURL%38DF80F10231.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommURL%954405689; C++
      ClCommURL (m4string_t &ai_strURL);

    // Additional Public Declarations
      //## begin ClCommURL%38DF80F10231.public preserve=yes
      //## end ClCommURL%38DF80F10231.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommURL%38DF80F10231.protected preserve=yes
      //## end ClCommURL%38DF80F10231.protected

  private:
    // Additional Private Declarations
      //## begin ClCommURL%38DF80F10231.private preserve=yes
      //## end ClCommURL%38DF80F10231.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommURL%38DF80F10231.implementation preserve=yes
      //## end ClCommURL%38DF80F10231.implementation

};

//## begin ClCommURL%38DF80F10231.postscript preserve=yes
//## end ClCommURL%38DF80F10231.postscript

//## begin ClNetObject%38D662360282.preface preserve=yes
//## end ClNetObject%38D662360282.preface

//## Class: ClNetObject%38D662360282
//## Category: m4transport::comunes%38F1B4750198
//## Subsystem: m4transport::comunes::inc%38D2272F02D9
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClNetObject 
{
  //## begin ClNetObject%38D662360282.initialDeclarations preserve=yes
  //## end ClNetObject%38D662360282.initialDeclarations

  public:
    // Additional Public Declarations
      //## begin ClNetObject%38D662360282.public preserve=yes
      //## end ClNetObject%38D662360282.public

  protected:
    // Additional Protected Declarations
      //## begin ClNetObject%38D662360282.protected preserve=yes
      //## end ClNetObject%38D662360282.protected

  private:
    // Additional Private Declarations
      //## begin ClNetObject%38D662360282.private preserve=yes
      //## end ClNetObject%38D662360282.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClNetObject%38D662360282.implementation preserve=yes
      //## end ClNetObject%38D662360282.implementation

};

//## begin ClNetObject%38D662360282.postscript preserve=yes
//## end ClNetObject%38D662360282.postscript

// Class ClIOEventHandler 

// Class ClSSLParams 

// Class ClCommURL 

// Class ClNetObject 

//## begin module%38D22742039F.epilog preserve=yes
//## end module%38D22742039F.epilog


#endif

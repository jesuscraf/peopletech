//## begin module%390FEF2E02C2.cm preserve=no
//## end module%390FEF2E02C2.cm

//## begin module%390FEF2E02C2.cp preserve=no
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
//## end module%390FEF2E02C2.cp

//## Module: commprotocols%390FEF2E02C2; Package specification
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Source file: e:\source\m4communication\inc\commprotocols.hpp

#ifndef commprotocols_h
#define commprotocols_h 1

//## begin module%390FEF2E02C2.additionalIncludes preserve=no
//## end module%390FEF2E02C2.additionalIncludes

//## begin module%390FEF2E02C2.includes preserve=yes
//## end module%390FEF2E02C2.includes

// m4types
#include <m4types.hpp>
// Protocol
#include <protocol.hpp>
// basepdu
#include <basepdus.hpp>
//## begin module%390FEF2E02C2.declarations preserve=no
//## end module%390FEF2E02C2.declarations

//## begin module%390FEF2E02C2.additionalDeclarations preserve=yes

#define M4_PDU_COMM_PROTOCOL_RETURN_CONTINUE_READING 0
#define M4_PDU_COMM_PROTOCOL_RETURN_COMPLETED					1
#define M4_PDU_COMM_PROTOCOL_RETURN_ERROR							-1

//## end module%390FEF2E02C2.additionalDeclarations


//## begin ClCommPDUProtocol%390FEC49000A.preface preserve=yes
//## end ClCommPDUProtocol%390FEC49000A.preface

//## Class: ClCommPDUProtocol%390FEC49000A
//## Category: M4Communication::M4COML::Protocols%390FEB4C020F
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4PDUS ClPDUProtocol //## Inherits: <unnamed>%390FEC550224
{
  //## begin ClCommPDUProtocol%390FEC49000A.initialDeclarations preserve=yes
  //## end ClCommPDUProtocol%390FEC49000A.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommPDUProtocol%957345693
      ClPDUProtocol ();

    //## Destructor (specified)
      //## Operation: ~ClCommPDUProtocol%957345694
      ~ClPDUProtocol ();


    //## Other Operations (specified)
      //## Operation: AddChunk%957345695; C++
      m4return_t AddChunk (m4pchar_t ai_pcChunk, m4uint_t ai_uiChunkSize, m4uint_t &ao_uiUssed, m4uint_t &ao_uiNextBLockSize);

       //## Operation: GetReadedPDU%957345698; C++
      m4return_t GetReadedPDU (ClPDU *&ao_pPDU);

      //## Operation: GetCommProtocolType%957345712
      m4int_t GetCommProtocolType ();

      //## Operation: SetWritePDU%957364994; C++
      m4return_t SetWritePDU (ClPDU *ai_pPDU);

      //## Operation: Initialize%957517150
      m4return_t Initialize ();

      //## Operation: ReleaseContents%957944567; C++
      m4return_t ReleaseContents ();

      //## Operation: GetNextChunk%959266651; C++
      m4return_t GetNextChunk (m4pchar_t &ao_pcChunk, m4uint_t &ao_uiChunkSize);

      //## Operation: ExtractLog%962101127
      m4return_t ExtractLog ();

      //## Operation: AddLog%962101128
      m4return_t AddLog ();

    // Data Members for Associations
  public:
      //## Association: M4Communication::M4COML::Protocols::<unnamed>%390FFB8502BB
      //## Role: ClCommPDUProtocol::m_Protocol%390FFB8600DC
      //## begin ClCommPDUProtocol::m_Protocol%390FFB8600DC.role preserve=no  public: ClProtocol { -> 1VHAN}
      ClProtocol m_Protocol;
      //## end ClCommPDUProtocol::m_Protocol%390FFB8600DC.role

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%39196A710133
      //## Role: ClCommPDUProtocol::m_pcNextBlockBuffer%39196A720148
      //## begin ClCommPDUProtocol::m_pcNextBlockBuffer%39196A720148.role preserve=no  public: m4pchar_t { -> VHAN}
      m4pchar_t m_pcNextBlockBuffer;
      //## end ClCommPDUProtocol::m_pcNextBlockBuffer%39196A720148.role

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%395C64A5036E
      //## Role: ClCommPDUProtocol::m_iSizeLostBlock%395C64A70230
      //## begin ClCommPDUProtocol::m_iSizeLostBlock%395C64A70230.role preserve=no  public: m4int_t { -> UHAN}
      m4int_t m_iSizeLostBlock;
      //## end ClCommPDUProtocol::m_iSizeLostBlock%395C64A70230.role

    // Additional Public Declarations
      //## begin ClCommPDUProtocol%390FEC49000A.public preserve=yes
      //## end ClCommPDUProtocol%390FEC49000A.public

  protected:
    // Data Members for Associations

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%39103ADC0090
      //## Role: ClCommPDUProtocol::m_poReadPDU%39103ADC0272
      //## begin ClCommPDUProtocol::m_poReadPDU%39103ADC0272.role preserve=no  protected: ClPDU { -> 1RHAN}
      ClPDU *m_poReadPDU;
      //## end ClCommPDUProtocol::m_poReadPDU%39103ADC0272.role

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%3912BBC00212
      //## Role: ClCommPDUProtocol::m_iProcessedBytes%3912BBC101D7
      //## begin ClCommPDUProtocol::m_iProcessedBytes%3912BBC101D7.role preserve=no  protected: m4int_t { -> VHAN}
      m4int_t m_iProcessedBytes;
      //## end ClCommPDUProtocol::m_iProcessedBytes%3912BBC101D7.role

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%39196A2E01A4
      //## Role: ClCommPDUProtocol::m_iNextBlockSize%39196A2F0200
      //## begin ClCommPDUProtocol::m_iNextBlockSize%39196A2F0200.role preserve=no  protected: m4int_t { -> VHAN}
      m4int_t m_iNextBlockSize;
      //## end ClCommPDUProtocol::m_iNextBlockSize%39196A2F0200.role

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%3923D1D901CD
      //## Role: ClCommPDUProtocol::m_poWritePDU%3923D1DA01ED
      //## begin ClCommPDUProtocol::m_poWritePDU%3923D1DA01ED.role preserve=no  protected: ClPDU { -> RHAN}
      ClPDU *m_poWritePDU;
      //## end ClCommPDUProtocol::m_poWritePDU%3923D1DA01ED.role

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%392BF9010112
      //## Role: ClCommPDUProtocol::m_pcWriteBuffer%392BF902027C
      //## begin ClCommPDUProtocol::m_pcWriteBuffer%392BF902027C.role preserve=no  protected: m4pchar_t { -> VHAN}
      m4pchar_t m_pcWriteBuffer;
      //## end ClCommPDUProtocol::m_pcWriteBuffer%392BF902027C.role

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%392BF9750304
      //## Role: ClCommPDUProtocol::m_iWriteBufferSize%392BF976020C
      //## begin ClCommPDUProtocol::m_iWriteBufferSize%392BF976020C.role preserve=no  protected: m4int_t { -> VHAN}
      m4int_t m_iWriteBufferSize;
      //## end ClCommPDUProtocol::m_iWriteBufferSize%392BF976020C.role

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%395C641E019D
      //## Role: ClCommPDUProtocol::m_pLostBlock%395C641F02C2
      //## begin ClCommPDUProtocol::m_pLostBlock%395C641F02C2.role preserve=no  protected: m4pchar_t { -> VHAN}
      m4pchar_t m_pLostBlock;
      //## end ClCommPDUProtocol::m_pLostBlock%395C641F02C2.role

    // Additional Protected Declarations
      //## begin ClCommPDUProtocol%390FEC49000A.protected preserve=yes
      //## end ClCommPDUProtocol%390FEC49000A.protected

       //## Association: M4Communication::M4COML::Protocols::<unnamed>%39294DD70054
      //## Role: ClCommProtocol::m_uiSentBytes%39294DD80150
      //## begin ClCommProtocol::m_uiSentBytes%39294DD80150.role preserve=no  protected: m4uint_t { -> VHAN}
      m4uint_t m_uiSentBytes;
      //## end ClCommProtocol::m_uiSentBytes%39294DD80150.role

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%39294E0B0095
      //## Role: ClCommProtocol::m_uiReadedBytes%39294E0B0321
      //## begin ClCommProtocol::m_uiReadedBytes%39294E0B0321.role preserve=no  protected: m4uint_t { -> VHAN}
      m4uint_t m_uiReadedBytes;
      //## end ClCommProtocol::m_uiReadedBytes%39294E0B0321.role
 
};

//## begin ClCommPDUProtocol%390FEC49000A.postscript preserve=yes
//## end ClCommPDUProtocol%390FEC49000A.postscript

//## begin ClCommASCIIProtocol%390FEDC2037D.preface preserve=yes
//## end ClCommASCIIProtocol%390FEDC2037D.preface

//## Class: ClCommASCIIProtocol%390FEDC2037D
//## Category: M4Communication::M4COML::Protocols%390FEB4C020F
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n


//## begin module%390FEF2E02C2.epilog preserve=yes
//## end module%390FEF2E02C2.epilog


#endif

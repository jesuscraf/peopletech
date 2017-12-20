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
//## Source file: e:\mybuild\m4communication\inc\commprotocols.hpp

#ifndef commprotocols_h
#define commprotocols_h 1

//## begin module%390FEF2E02C2.additionalIncludes preserve=no
//## end module%390FEF2E02C2.additionalIncludes

//## begin module%390FEF2E02C2.includes preserve=yes
#include "m4communication_dll.hpp"
//## end module%390FEF2E02C2.includes

// m4types
#include <m4types.hpp>
// stdio
#include <stdio.h>
// basepdu
#include <basepdus.hpp>
// Protocol
#include <protocol.hpp>
//## begin module%390FEF2E02C2.declarations preserve=no
//## end module%390FEF2E02C2.declarations

//## begin module%390FEF2E02C2.additionalDeclarations preserve=yes
#define M4_COMM_NULL_PROTOCOL		0
#define M4_COMM_PDU_PROTOCOL		1
#define M4_COMM_ASCII_PROTOCOL		2
#define M4_COMM_PDU_HTTP_PROTOCOL	3

#define M4_COMM_PROTOCOL_RETURN_CONTINUE_READING	0
#define M4_COMM_PROTOCOL_RETURN_COMPLETED			1
#define M4_COMM_PROTOCOL_RETURN_ERROR				-1

#define M4_COMM_DEFAULT_BUFFER_SIZE 262144
#define M4_COMM_PDU_PROTOCOL_DEFAULT_BUFFER_SIZE M4_COMM_DEFAULT_BUFFER_SIZE 

#include "commpduzdelta.hpp"
//## end module%390FEF2E02C2.additionalDeclarations


//## begin ClCommProtocol%390FEB93008A.preface preserve=yes
//## end ClCommProtocol%390FEB93008A.preface

//## Class: ClCommProtocol%390FEB93008A
//## Category: M4Communication::M4COML::Protocols%390FEB4C020F
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3A2242D4006E;ClPrecisionTicker { -> F}

class M4_DECL_M4COMMUNICATION ClCommProtocol 
{
  //## begin ClCommProtocol%390FEB93008A.initialDeclarations preserve=yes
  //## end ClCommProtocol%390FEB93008A.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommProtocol%957345699
      ClCommProtocol ();

    //## Destructor (specified)
      //## Operation: ~ClCommProtocol%957345700
      virtual ~ClCommProtocol ();


    //## Other Operations (specified)
      //## Operation: AddChunk%957345707; C++
      virtual m4return_t AddChunk (m4pchar_t ai_pcChunk, m4uint_t ai_uiChunkSize, m4uint_t &ao_uiUssed, m4uint_t &ao_uiNextBLockSize) = 0;

      //## Operation: IsCompleted%957345708; C++
      virtual m4return_t IsCompleted () = 0;

      //## Operation: GetNextChunk%957345709; C++
      virtual m4return_t GetNextChunk (m4pchar_t &ao_pcChunk, m4uint_t &ao_uiChunkSize) = 0;

      //## Operation: GetCommProtocolType%957345713
      virtual m4int_t GetCommProtocolType ();

      //## Operation: Initialize%957517152
      virtual m4return_t Initialize () = 0;

      //## Operation: ReleaseContents%957944569; C++
      virtual m4return_t ReleaseContents () = 0;

      //## Operation: GetReadedBytes%959007682; C++
      m4uint_t GetReadedBytes ();

      //## Operation: GetSentBytes%959007683; C++
      m4uint_t GetSentBytes ();

      //## Operation: GetStartWrittingTime%963233189
      m4uint64_t GetStartWrittingTime ();

      //## Operation: GetFinishWrittingTime%963233190
      m4uint64_t GetFinishWrittingTime ();

      //## Operation: GetStartReadingTime%963233191
      m4uint64_t GetStartReadingTime ();

      //## Operation: GetFinishReadingTime%963233192
      m4uint64_t GetFinishReadingTime ();

      //## Operation: GetNSentRequests%964423551
      m4uint_t GetNSentRequests ();

      //## Operation: GetNReceivedRequests%964423552
      m4uint_t GetNReceivedRequests ();

      //## Operation: EnableDump%967562173; C++
      m4return_t EnableDump (m4pchar_t ai_pcFileName);

      //## Operation: DisableDump%967562174
      m4return_t DisableDump ();

      //## Operation: Dump%967646980; C++
      m4return_t Dump (m4pchar_t ai_pcChunk, m4int_t ai_iChunkSize);

      //## Operation: SetVersion%973247123; C++
      virtual m4return_t SetVersion (m4int_t ai_iVersion);

      //## Operation: GetVersion%973247124
      virtual m4int_t GetVersion ();

      //## Operation: GetStartProccessingWrittingTime%975501045
      m4uint64_t GetStartProccessingWrittingTime ();

      //## Operation: GetFinishProccessingReadingTime%975501046
      m4uint64_t GetFinishProccessingReadingTime ();

    // Additional Public Declarations
      //## begin ClCommProtocol%390FEB93008A.public preserve=yes
	  m4return_t EnableWriteDump (m4pchar_t ai_pcFileName);
      m4return_t DisableWriteDump ();
      m4return_t DumpWrite (m4pchar_t ai_pcChunk, m4int_t ai_iChunkSize);
	  m4return_t EnableReadDump (m4pchar_t ai_pcFileName);
      m4return_t DisableReadDump ();
      m4return_t DumpRead (m4pchar_t ai_pcChunk, m4int_t ai_iChunkSize);
      //## end ClCommProtocol%390FEB93008A.public

  protected:
    // Data Members for Associations

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

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%3969C497022F
      //## Role: ClCommProtocol::m_ui64StartWrittingTime%3969C49801FF
      //## begin ClCommProtocol::m_ui64StartWrittingTime%3969C49801FF.role preserve=no  protected: m4uint64_t { -> VHAN}
      m4uint64_t m_ui64StartWrittingTime;
      //## end ClCommProtocol::m_ui64StartWrittingTime%3969C49801FF.role

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%3969C4E5005B
      //## Role: ClCommProtocol::m_ui64FinishWrittingTime%3969C4E5035E
      //## begin ClCommProtocol::m_ui64FinishWrittingTime%3969C4E5035E.role preserve=no  protected: m4uint64_t { -> VHAN}
      m4uint64_t m_ui64FinishWrittingTime;
      //## end ClCommProtocol::m_ui64FinishWrittingTime%3969C4E5035E.role

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%3969C53D0242
      //## Role: ClCommProtocol::m_ui64StartReadingTime%3969C53E01F3
      //## begin ClCommProtocol::m_ui64StartReadingTime%3969C53E01F3.role preserve=no  protected: m4uint64_t { -> VHAN}
      m4uint64_t m_ui64StartReadingTime;
      //## end ClCommProtocol::m_ui64StartReadingTime%3969C53E01F3.role

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%3969C56E0166
      //## Role: ClCommProtocol::m_ui64FinishReadingTime%3969C56F0081
      //## begin ClCommProtocol::m_ui64FinishReadingTime%3969C56F0081.role preserve=no  protected: m4uint64_t { -> VHAN}
      m4uint64_t m_ui64FinishReadingTime;
      //## end ClCommProtocol::m_ui64FinishReadingTime%3969C56F0081.role

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%397BEEBD0275
      //## Role: ClCommProtocol::m_uiNSentRequests%397BEEBE0259
      //## begin ClCommProtocol::m_uiNSentRequests%397BEEBE0259.role preserve=no  protected: m4uint_t { -> VHAN}
      m4uint_t m_uiNSentRequests;
      //## end ClCommProtocol::m_uiNSentRequests%397BEEBE0259.role

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%397BEEF20344
      //## Role: ClCommProtocol::m_uiNReceivedRequests%397BEEF40030
      //## begin ClCommProtocol::m_uiNReceivedRequests%397BEEF40030.role preserve=no  protected: m4uint_t { -> VHAN}
      m4uint_t m_uiNReceivedRequests;
      //## end ClCommProtocol::m_uiNReceivedRequests%397BEEF40030.role

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%39ABD9580089
      //## Role: ClCommProtocol::m_pDumpFile%39ABD9580292
      //## begin ClCommProtocol::m_pDumpFile%39ABD9580292.role preserve=no  protected: FILE { -> RHAN}
      FILE *m_pDumpFile;
      //## end ClCommProtocol::m_pDumpFile%39ABD9580292.role

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%3A24F66500F7
      //## Role: ClCommProtocol::m_ui64StartProccessingWrittingTime%3A24F6660030
      //## begin ClCommProtocol::m_ui64StartProccessingWrittingTime%3A24F6660030.role preserve=no  protected: m4uint64_t { -> VHAN}
      m4uint64_t m_ui64StartProccessingWrittingTime;
      //## end ClCommProtocol::m_ui64StartProccessingWrittingTime%3A24F6660030.role

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%3A24F6A30236
      //## Role: ClCommProtocol::m_ui64FinishProccessingReadingTime%3A24F6A4011F
      //## begin ClCommProtocol::m_ui64FinishProccessingReadingTime%3A24F6A4011F.role preserve=no  protected: m4uint64_t { -> VHAN}
      m4uint64_t m_ui64FinishProccessingReadingTime;
      //## end ClCommProtocol::m_ui64FinishProccessingReadingTime%3A24F6A4011F.role

    // Additional Protected Declarations
      //## begin ClCommProtocol%390FEB93008A.protected preserve=yes
	  FILE *m_pDumpReadFile;
	  FILE *m_pDumpWriteFile;
	  m4bool_t m_bDump;
      //## end ClCommProtocol%390FEB93008A.protected

  private:
    // Additional Private Declarations
      //## begin ClCommProtocol%390FEB93008A.private preserve=yes
      //## end ClCommProtocol%390FEB93008A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommProtocol%390FEB93008A.implementation preserve=yes
  protected:
      ClPDUCommZDelta m_oZDelta;
      //## end ClCommProtocol%390FEB93008A.implementation
};

//## begin ClCommProtocol%390FEB93008A.postscript preserve=yes
//## end ClCommProtocol%390FEB93008A.postscript

//## begin ClCommPDUProtocol%390FEC49000A.preface preserve=yes
//## end ClCommPDUProtocol%390FEC49000A.preface

//## Class: ClCommPDUProtocol%390FEC49000A
//## Category: M4Communication::M4COML::Protocols%390FEB4C020F
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4COMMUNICATION ClCommPDUProtocol : public ClCommProtocol  //## Inherits: <unnamed>%390FEC550224
{
  //## begin ClCommPDUProtocol%390FEC49000A.initialDeclarations preserve=yes
  //## end ClCommPDUProtocol%390FEC49000A.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommPDUProtocol%957345693
      ClCommPDUProtocol ();

    //## Destructor (specified)
      //## Operation: ~ClCommPDUProtocol%957345694
      ~ClCommPDUProtocol ();


    //## Other Operations (specified)
      //## Operation: AddChunk%957345695; C++
      m4return_t AddChunk (m4pchar_t ai_pcChunk, m4uint_t ai_uiChunkSize, m4uint_t &ao_uiUssed, m4uint_t &ao_uiNextBLockSize);

      //## Operation: IsCompleted%957345696; C++
      m4return_t IsCompleted ();

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

      //## Operation: SetVersion%967707113; C++
      m4return_t SetVersion (m4int_t ai_iVersion);

      //## Operation: GetVersion%967707114
      m4int_t GetVersion ();

    // Additional Public Declarations
      //## begin ClCommPDUProtocol%390FEC49000A.public preserve=yes
      //## end ClCommPDUProtocol%390FEC49000A.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_bLog%3976FB4B02C3
      //## begin ClCommPDUProtocol::m_bLog%3976FB4B02C3.attr preserve=no  protected: m4bool_t {UA} M4_FALSE
      m4bool_t m_bLog;
      //## end ClCommPDUProtocol::m_bLog%3976FB4B02C3.attr

      //## Attribute: m_iVersion%39AE0579015A
      //## begin ClCommPDUProtocol::m_iVersion%39AE0579015A.attr preserve=no  protected: m4int_t {UA} M4_PDU_40_VERSION
      m4int_t m_iVersion;
      //## end ClCommPDUProtocol::m_iVersion%39AE0579015A.attr

    // Data Members for Associations

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%390FFB8502BB
      //## Role: ClCommPDUProtocol::m_Protocol%390FFB8600DC
      //## begin ClCommPDUProtocol::m_Protocol%390FFB8600DC.role preserve=no  protected: ClProtocol { -> 1VHAN}
      ClProtocol m_Protocol;
      //## end ClCommPDUProtocol::m_Protocol%390FFB8600DC.role

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

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%39196A710133
      //## Role: ClCommPDUProtocol::m_pcNextBlockBuffer%39196A720148
      //## begin ClCommPDUProtocol::m_pcNextBlockBuffer%39196A720148.role preserve=no  protected: m4pchar_t { -> VHAN}
      m4pchar_t m_pcNextBlockBuffer;
      //## end ClCommPDUProtocol::m_pcNextBlockBuffer%39196A720148.role

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

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%395C64A5036E
      //## Role: ClCommPDUProtocol::m_iSizeLostBlock%395C64A70230
      //## begin ClCommPDUProtocol::m_iSizeLostBlock%395C64A70230.role preserve=no  protected: m4int_t { -> UHAN}
      m4int_t m_iSizeLostBlock;
      //## end ClCommPDUProtocol::m_iSizeLostBlock%395C64A70230.role

    // Additional Protected Declarations
      //## begin ClCommPDUProtocol%390FEC49000A.protected preserve=yes
      //## end ClCommPDUProtocol%390FEC49000A.protected

  private:
    // Additional Private Declarations
      //## begin ClCommPDUProtocol%390FEC49000A.private preserve=yes
      //## end ClCommPDUProtocol%390FEC49000A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommPDUProtocol%390FEC49000A.implementation preserve=yes
      //## end ClCommPDUProtocol%390FEC49000A.implementation

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

class ClCommASCIIProtocol : public ClCommProtocol  //## Inherits: <unnamed>%390FEDD100FE
{
  //## begin ClCommASCIIProtocol%390FEDC2037D.initialDeclarations preserve=yes
  //## end ClCommASCIIProtocol%390FEDC2037D.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommASCIIProtocol%957345701
      ClCommASCIIProtocol ();

    //## Destructor (specified)
      //## Operation: ~ClCommASCIIProtocol%957345702
      ~ClCommASCIIProtocol ();


    //## Other Operations (specified)
      //## Operation: AddChunk%957345703; C++
      m4return_t AddChunk (m4pchar_t ai_pcChunk, m4uint_t ai_uiChunkSize, m4uint_t &ao_uiUssed, m4uint_t &ao_uiNextBLockSize);

      //## Operation: IsCompleted%957345704; C++
      m4return_t IsCompleted ();

      //## Operation: GetNextChunk%957345705; C++
      m4return_t GetNextChunk (m4pchar_t &ao_pcChunk, m4uint_t &ao_uiChunkSize);

      //## Operation: GetFullChunk%957345706; C++
      m4return_t GetFullChunk (m4pchar_t &ao_pcXML, m4uint_t &ao_uiXMLSize);

      //## Operation: GetCommProtocolType%957345714
      m4int_t GetCommProtocolType ();

      //## Operation: Initialize%957517151
      m4return_t Initialize ();

      //## Operation: GetReadedBuffer%957856081; C++
      m4pchar_t GetReadedBuffer ();

      //## Operation: ReleaseContents%957944568; C++
      m4return_t ReleaseContents ();

      //## Operation: SetWriteBuffer%958730350; C++
      m4return_t SetWriteBuffer (m4pchar_t ai_pcBuffer, m4int_t ai_iBufferSize);

    // Additional Public Declarations
      //## begin ClCommASCIIProtocol%390FEDC2037D.public preserve=yes
      //## end ClCommASCIIProtocol%390FEDC2037D.public

  protected:
    // Data Members for Associations

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%3917DC9500BF
      //## Role: ClCommASCIIProtocol::m_pcReadBuffer%3917DC9503AE
      //## begin ClCommASCIIProtocol::m_pcReadBuffer%3917DC9503AE.role preserve=no  protected: m4pchar_t { -> UHAN}
      m4pchar_t m_pcReadBuffer;
      //## end ClCommASCIIProtocol::m_pcReadBuffer%3917DC9503AE.role

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%3917DCAB0279
      //## Role: ClCommASCIIProtocol::m_iBufferSize%3917DCAC00A4
      //## begin ClCommASCIIProtocol::m_iBufferSize%3917DCAC00A4.role preserve=no  protected: m4int_t { -> VHAN}
      m4int_t m_iBufferSize;
      //## end ClCommASCIIProtocol::m_iBufferSize%3917DCAC00A4.role

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%3925103301D6
      //## Role: ClCommASCIIProtocol::m_pcWriteBuffer%3925103400E8
      //## begin ClCommASCIIProtocol::m_pcWriteBuffer%3925103400E8.role preserve=no  protected: m4pchar_t { -> 1UHAN}
      m4pchar_t m_pcWriteBuffer;
      //## end ClCommASCIIProtocol::m_pcWriteBuffer%3925103400E8.role

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%39252380037F
      //## Role: ClCommASCIIProtocol::m_iSentBytes%3925238103E5
      //## begin ClCommASCIIProtocol::m_iSentBytes%3925238103E5.role preserve=no  protected: m4int_t { -> VHAN}
      m4int_t m_iSentBytes;
      //## end ClCommASCIIProtocol::m_iSentBytes%3925238103E5.role

    // Additional Protected Declarations
      //## begin ClCommASCIIProtocol%390FEDC2037D.protected preserve=yes
      //## end ClCommASCIIProtocol%390FEDC2037D.protected

  private:
    // Additional Private Declarations
      //## begin ClCommASCIIProtocol%390FEDC2037D.private preserve=yes
      //## end ClCommASCIIProtocol%390FEDC2037D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommASCIIProtocol%390FEDC2037D.implementation preserve=yes
      //## end ClCommASCIIProtocol%390FEDC2037D.implementation

};

//## begin ClCommASCIIProtocol%390FEDC2037D.postscript preserve=yes
//## end ClCommASCIIProtocol%390FEDC2037D.postscript

//## begin ClCommPDUHTTPProtocol%39F7FE6902A1.preface preserve=yes
//## end ClCommPDUHTTPProtocol%39F7FE6902A1.preface

//## Class: ClCommPDUHTTPProtocol%39F7FE6902A1
//## Category: M4Communication::M4COML::Protocols%390FEB4C020F
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommPDUHTTPProtocol : public ClCommPDUProtocol  //## Inherits: <unnamed>%39F7FE7B00D0
{
  //## begin ClCommPDUHTTPProtocol%39F7FE6902A1.initialDeclarations preserve=yes
  //## end ClCommPDUHTTPProtocol%39F7FE6902A1.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommPDUHTTPProtocol%972554298
      ClCommPDUHTTPProtocol ();


    //## Other Operations (specified)
      //## Operation: SetWriteHeaders%972554295; C++
      m4return_t SetWriteHeaders (m4pchar_t ai_pcWriteHeaders);

      //## Operation: GetReadedHeaders%972554296
      m4pchar_t GetReadedHeaders ();

      //## Operation: AddChunk%972554302; C++
      m4return_t AddChunk (m4pchar_t ai_pcChunk, m4uint_t ai_uiChunkSize, m4uint_t &ao_uiUssed, m4uint_t &ao_uiNextBLockSize);

      //## Operation: GetNextChunk%972554303; C++
      m4return_t GetNextChunk (m4pchar_t &ao_pcChunk, m4uint_t &ao_uiChunkSize);

      //## Operation: GetCommProtocolType%972554304
      m4int_t GetCommProtocolType ();

    // Additional Public Declarations
      //## begin ClCommPDUHTTPProtocol%39F7FE6902A1.public preserve=yes
      //## end ClCommPDUHTTPProtocol%39F7FE6902A1.public

  protected:
    // Data Members for Associations

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%39F7FFC70377
      //## Role: ClCommPDUHTTPProtocol::m_pcWriteHeaders%39F7FFC801B6
      //## begin ClCommPDUHTTPProtocol::m_pcWriteHeaders%39F7FFC801B6.role preserve=no  protected: m4pchar_t { -> VHAN}
      m4pchar_t m_pcWriteHeaders;
      //## end ClCommPDUHTTPProtocol::m_pcWriteHeaders%39F7FFC801B6.role

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%39F8000A0305
      //## Role: ClCommPDUHTTPProtocol::m_pcReadedHeaders%39F8000B0159
      //## begin ClCommPDUHTTPProtocol::m_pcReadedHeaders%39F8000B0159.role preserve=no  protected: m4pchar_t { -> VHAN}
      m4pchar_t m_pcReadedHeaders;
      //## end ClCommPDUHTTPProtocol::m_pcReadedHeaders%39F8000B0159.role

      //## Association: M4Communication::M4COML::Protocols::<unnamed>%39F834D700BC
      //## Role: ClCommPDUHTTPProtocol::m_bHeaderSent%39F834D7026C
      //## begin ClCommPDUHTTPProtocol::m_bHeaderSent%39F834D7026C.role preserve=no  protected: m4bool_t { -> VHAN}
      m4bool_t m_bHeaderSent;
      //## end ClCommPDUHTTPProtocol::m_bHeaderSent%39F834D7026C.role

    // Additional Protected Declarations
      //## begin ClCommPDUHTTPProtocol%39F7FE6902A1.protected preserve=yes
      //## end ClCommPDUHTTPProtocol%39F7FE6902A1.protected

  private:
    // Additional Private Declarations
      //## begin ClCommPDUHTTPProtocol%39F7FE6902A1.private preserve=yes
      //## end ClCommPDUHTTPProtocol%39F7FE6902A1.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommPDUHTTPProtocol%39F7FE6902A1.implementation preserve=yes
      //## end ClCommPDUHTTPProtocol%39F7FE6902A1.implementation

};

//## begin ClCommPDUHTTPProtocol%39F7FE6902A1.postscript preserve=yes
//## end ClCommPDUHTTPProtocol%39F7FE6902A1.postscript

// Class ClCommProtocol 


//## Other Operations (inline)
inline m4int_t ClCommProtocol::GetCommProtocolType ()
{
  //## begin ClCommProtocol::GetCommProtocolType%957345713.body preserve=yes
	 return M4_COMM_NULL_PROTOCOL;
  //## end ClCommProtocol::GetCommProtocolType%957345713.body
}

inline m4uint_t ClCommProtocol::GetReadedBytes ()
{
  //## begin ClCommProtocol::GetReadedBytes%959007682.body preserve=yes
	return m_uiReadedBytes;
  //## end ClCommProtocol::GetReadedBytes%959007682.body
}

inline m4uint_t ClCommProtocol::GetSentBytes ()
{
  //## begin ClCommProtocol::GetSentBytes%959007683.body preserve=yes
	return m_uiSentBytes;
  //## end ClCommProtocol::GetSentBytes%959007683.body
}

// Class ClCommPDUProtocol 


//## Other Operations (inline)
inline m4int_t ClCommPDUProtocol::GetCommProtocolType ()
{
  //## begin ClCommPDUProtocol::GetCommProtocolType%957345712.body preserve=yes
	 return M4_COMM_PDU_PROTOCOL ;
  //## end ClCommPDUProtocol::GetCommProtocolType%957345712.body
}

// Class ClCommASCIIProtocol 


//## Other Operations (inline)
inline m4int_t ClCommASCIIProtocol::GetCommProtocolType ()
{
  //## begin ClCommASCIIProtocol::GetCommProtocolType%957345714.body preserve=yes
	return M4_COMM_ASCII_PROTOCOL;
  //## end ClCommASCIIProtocol::GetCommProtocolType%957345714.body
}

// Class ClCommPDUHTTPProtocol 


//## Other Operations (inline)
inline m4int_t ClCommPDUHTTPProtocol::GetCommProtocolType ()
{
  //## begin ClCommPDUHTTPProtocol::GetCommProtocolType%972554304.body preserve=yes
	return M4_COMM_PDU_HTTP_PROTOCOL;
  //## end ClCommPDUHTTPProtocol::GetCommProtocolType%972554304.body
}

//## begin module%390FEF2E02C2.epilog preserve=yes
//## end module%390FEF2E02C2.epilog


#endif

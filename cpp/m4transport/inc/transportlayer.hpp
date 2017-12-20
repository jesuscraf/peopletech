//## begin module%38D1FD51010A.cm preserve=no
//## end module%38D1FD51010A.cm

//## begin module%38D1FD51010A.cp preserve=no
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
//## end module%38D1FD51010A.cp

//## Module: TransportLayer%38D1FD51010A; Package specification
//## Subsystem: m4transport::M4CTL::inc%38D1FCBC0200
//## Source file: D:\Work\v600\m4transport\inc\transportlayer.hpp

#ifndef transportlayer_h
#define transportlayer_h 1

//## begin module%38D1FD51010A.additionalIncludes preserve=no
//## end module%38D1FD51010A.additionalIncludes

//## begin module%38D1FD51010A.includes preserve=yes
#define	M4_TRANSPORT_OK							0
#define	M4_TRANSPORT_INVALID_IDENTIFIER			201
#define	M4_TRANSPORT_CONTROL_PIPES_MAKER		202
#define	M4_TRANSPORT_SELECT						203
#define	M4_TRANSPORT_CONTROL_PIPES_SETSOCKOPT	204
#define	M4_TRANSPORT_SOCKET_INIT				205
#define	M4_TRANSPORT_MEMORY						206
//## end module%38D1FD51010A.includes

#define M4_COMM_DEFAULT_BUFFER_MARGIN_SIZE		32000

// CommComun
#include <commcomun.hpp>
// m4thread
#include <m4thread.hpp>
// m4types
#include <m4types.hpp>
// stl
#include "m4stl.hpp"
// syncro
#include <syncro.hpp>
// BlockSynchronization
#include <blocksynchronization.hpp>
//## begin module%38D1FD51010A.declarations preserve=no
//## end module%38D1FD51010A.declarations

//## begin module%38D1FD51010A.additionalDeclarations preserve=yes
//## end module%38D1FD51010A.additionalDeclarations


//## begin ClTransportLayer%38D735EA02F6.preface preserve=yes
//## end ClTransportLayer%38D735EA02F6.preface

//## Class: ClTransportLayer%38D735EA02F6
//## Category: m4transport::M4CTL%38D0B5F00369
//## Subsystem: m4transport::M4CTL::inc%38D1FCBC0200
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%38D9CFB40219;m4return_t { -> }
//## Uses: <unnamed>%38E99C5700C0;ClIOEventHandler { -> }
//## Uses: <unnamed>%38F5811E01BB;ClTSAP { -> }
//## Uses: <unnamed>%3E7723A501BF;ClMutBlock { -> }

class M4_DECL_M4TRANSPORT ClTransportLayer : public M4Thread  //## Inherits: <unnamed>%38DB44AC03C4
{
  //## begin ClTransportLayer%38D735EA02F6.initialDeclarations preserve=yes
  //## end ClTransportLayer%38D735EA02F6.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClTransportLayer%954835557
      ClTransportLayer ();


    //## Other Operations (specified)
      //## Operation: OpenPort%953651954; C++
      virtual m4return_t OpenPort (int ai_iPort, ClIOEventHandler *ai_pIOEventHandler) = 0;

      //## Operation: ClosePort%953651955; C++
      virtual m4return_t ClosePort (m4tliid_t ai_Identifier, ClIOEventHandler *ai_pIOEventHandler) = 0;

      //## Operation: BeginRead%953651956; C++
      virtual m4return_t BeginRead (m4tliid_t ao_Identifier, ClIOEventHandler *ai_pIOEventHandler) = 0;

      //## Operation: Write%953651957; C++
      virtual m4return_t Write (m4tliid_t ao_Identifier, ClIOEventHandler *ai_pIOEventHandler) = 0;

      //## Operation: StopReading%953651958; C++
      virtual m4return_t StopReading (m4tliid_t ai_Identifier, ClIOEventHandler *ai_pIOEventHandler) = 0;

      //## Operation: StopWriting%953651959; C++
      virtual m4return_t StopWriting (m4tliid_t ai_Identifier, ClIOEventHandler *ai_pIOEventHandler) = 0;

      //## Operation: Connect%953651960
      virtual m4return_t Connect (char *ai_Host, int ai_iPort, ClIOEventHandler *ai_pIOEventHandler) = 0;

      //## Operation: StopConnection%953901230; C++
      virtual m4return_t StopConnection (m4tliid_t ai_Identifier, ClIOEventHandler *ai_pIOEventHandler) = 0;

      //## Operation: ConnectSSL%956570159
      virtual m4return_t ConnectSSL (char *ai_Host, int ai_iPort, ClIOEventHandler *ai_pIOEventHandler, char *ai_Cipher, char *ai_Certificate) = 0;

      //## Operation: OpenSSLPort%956570165; C++
      virtual m4return_t OpenSSLPort (int ai_iPort, ClIOEventHandler *ai_pIOEventHandler, char *ai_pCertificateFile, char *ai_pPrivateKey, char *ai_pCipher = NULL) = 0;

      //## Operation: GetTSAPError%956912719
      virtual int GetTSAPError (m4tliid_t ai_Identifier) = 0;

      //## Operation: StopTransportLayer%956912720
      virtual m4return_t StopTransportLayer (ClIOEventHandler *ai_pIOEventHandler) = 0;

      //## Operation: GetTransferedBytes%958057177
      virtual m4return_t GetTransferedBytes (m4tliid_t ai_Identifier, ClIOEventHandler *ai_pIOEventHandler) = 0;

      //## Operation: Error%958057178
      int Error ();

      //## Operation: EnableDumpData%1047994131
	  // Bug 0099587.
      static m4return_t EnableDumpData (m4pchar_t ai_pcBasePath, m4uint32_t ai_uiMaxSize, m4pchar_t ai_pcBasePrefix, m4uint32_t ai_uiMaxFiles);

      //## Operation: DisableDumpData%1047994132
      static void DisableDumpData ();

      //## Operation: DumpData%1047994133
      void DumpData (m4uint64_t ai_pId, void *ai_pBuffer, m4uint32_t ai_uiLen);

      //## Operation: GetDumpStatus%1049711019
      static m4bool_t GetDumpStatus (m4pcchar_t &ao_pcBasePath, m4uint32_t &ao_uiMaxSize, m4pcchar_t &ao_pcBasePrefix, m4int32_t &ao_iMaxFiles);

    // Additional Public Declarations
      //## begin ClTransportLayer%38D735EA02F6.public preserve=yes
      //## end ClTransportLayer%38D735EA02F6.public

  protected:
    //## Destructor (specified)
      //## Operation: ~ClTransportLayer%1047994134
      virtual ~ClTransportLayer ();

    // Data Members for Class Attributes

      //## Attribute: m_iError%391ACDBB02CA
      //## begin ClTransportLayer::m_iError%391ACDBB02CA.attr preserve=no  protected: int {UA} M4_TRANSPORT_OK
      int m_iError;
      //## end ClTransportLayer::m_iError%391ACDBB02CA.attr

      //## Attribute: m_bDumpData%3E7720400057
      //## begin ClTransportLayer::m_bDumpData%3E7720400057.attr preserve=no  protected: static m4bool_t {UA} M4_FALSE
      static m4bool_t m_bDumpData;
      //## end ClTransportLayer::m_bDumpData%3E7720400057.attr

      //## Attribute: m_pcBaseDumpDir%3E7721B40355
      //## begin ClTransportLayer::m_pcBaseDumpDir%3E7721B40355.attr preserve=no  protected: static m4pchar_t {UA} NULL
      static m4pchar_t m_pcBaseDumpDir;
      //## end ClTransportLayer::m_pcBaseDumpDir%3E7721B40355.attr

      //## Attribute: m_pcDumpPrefix%3E7721C603B5
      //## begin ClTransportLayer::m_pcDumpPrefix%3E7721C603B5.attr preserve=no  protected: static m4pchar_t {UA} NULL
      static m4pchar_t m_pcDumpPrefix;
      //## end ClTransportLayer::m_pcDumpPrefix%3E7721C603B5.attr

      //## Attribute: m_uiMaxDumpSize%3E7722980294
      //## begin ClTransportLayer::m_uiMaxDumpSize%3E7722980294.attr preserve=no  protected: static m4uint32_t {UA} 0
      static m4uint32_t m_uiMaxDumpSize;
      //## end ClTransportLayer::m_uiMaxDumpSize%3E7722980294.attr

      //## Attribute: m_uiCurrentDumpSize%3E7739560250
      //## begin ClTransportLayer::m_uiCurrentDumpSize%3E7739560250.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_uiCurrentDumpSize;
      //## end ClTransportLayer::m_uiCurrentDumpSize%3E7739560250.attr

      //## Attribute: m_uiDumpNumber%3E77396700C4
      //## begin ClTransportLayer::m_uiDumpNumber%3E77396700C4.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_uiDumpNumber;
      //## end ClTransportLayer::m_uiDumpNumber%3E77396700C4.attr

      //## Attribute: m_pFile%3E7739C201DD
      //## begin ClTransportLayer::m_pFile%3E7739C201DD.attr preserve=no  protected: FILE * {UA} NULL
      FILE *m_pFile;
      //## end ClTransportLayer::m_pFile%3E7739C201DD.attr

      //## Attribute: m_bDoReset%3EE5A6590072
      //## begin ClTransportLayer::m_bDoReset%3EE5A6590072.attr preserve=no  protected: static m4bool_t {UA} M4_FALSE
      static m4bool_t m_bDoReset;
      //## end ClTransportLayer::m_bDoReset%3EE5A6590072.attr

      //## Attribute: m_iMaxFiles%3EE5CA170173
      //## begin ClTransportLayer::m_iMaxFiles%3EE5CA170173.attr preserve=no  protected: static m4int32_t {UA} 0
      static m4int32_t m_iMaxFiles;
      //## end ClTransportLayer::m_iMaxFiles%3EE5CA170173.attr

    // Data Members for Associations

      //## Association: m4transport::M4CTL::<unnamed>%3E7735C400CF
      //## Role: ClTransportLayer::m_poMutex%3E7735C40328
      //## begin ClTransportLayer::m_poMutex%3E7735C40328.role preserve=no  protected: static ClMutex { -> RHAN}
      static ClMutex *m_poMutex;
      //## end ClTransportLayer::m_poMutex%3E7735C40328.role

      //## Association: m4transport::M4CTL::<unnamed>%3EE59E2300CA
      //## Role: ClTransportLayer::m_sBaseName%3EE59E230373
      //## begin ClTransportLayer::m_sBaseName%3EE59E230373.role preserve=no  protected: string { -> UHAN}
      string m_sBaseName;
      //## end ClTransportLayer::m_sBaseName%3EE59E230373.role

      //## Association: m4transport::M4CTL::<unnamed>%3EE5C5C70216
      //## Role: ClTransportLayer::m_vFiles%3EE5C5C70393
      //## begin ClTransportLayer::m_vFiles%3EE5C5C70393.role preserve=no  protected: vector { -> VHAN}
      vector<string> m_vFiles;
      //## end ClTransportLayer::m_vFiles%3EE5C5C70393.role

    // Additional Protected Declarations
      //## begin ClTransportLayer%38D735EA02F6.protected preserve=yes
      //## end ClTransportLayer%38D735EA02F6.protected

  private:
    // Additional Private Declarations
      //## begin ClTransportLayer%38D735EA02F6.private preserve=yes
      //## end ClTransportLayer%38D735EA02F6.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTransportLayer%38D735EA02F6.implementation preserve=yes
      //## end ClTransportLayer%38D735EA02F6.implementation

};

//## begin ClTransportLayer%38D735EA02F6.postscript preserve=yes
//## end ClTransportLayer%38D735EA02F6.postscript

// Class ClTransportLayer 


//## Other Operations (inline)
inline int ClTransportLayer::Error ()
{
  //## begin ClTransportLayer::Error%958057178.body preserve=yes
	return m_iError;
  //## end ClTransportLayer::Error%958057178.body
}

//## begin module%38D1FD51010A.epilog preserve=yes
//## end module%38D1FD51010A.epilog


#endif

//## begin module%345F71850388.cm preserve=no
//## end module%345F71850388.cm

//## begin module%345F71850388.cp preserve=no
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
//## end module%345F71850388.cp

//## Module: pduherram%345F71850388; Package specification
//	En este componente meteré todo lo relacionado con herramienta de utilización
//	de PDU.
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Source file: e:\source\m4appclient\inc\pduherram.hpp

#ifndef pduherram_h
#define pduherram_h 1

//## begin module%345F71850388.additionalIncludes preserve=yes
// #define _WIN32_WINNT 0x0400
#include <m4trace.hpp>
#include <m4log.hpp>
#include <m4srvres.hpp>

//## end module%345F71850388.additionalIncludes

//## begin module%345F71850388.includes preserve=yes
//## end module%345F71850388.includes

// tsap
#include <tsap.hpp>
// Protocol
#include <protocol.hpp>
// basepdu
#include <basepdus.hpp>
//## begin module%345F71850388.declarations preserve=no
//## end module%345F71850388.declarations

//## begin module%345F71850388.additionalDeclarations preserve=yes
//## end module%345F71850388.additionalDeclarations


//## Class: ClGetPDU%345F6A34021D
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%345F6ADA0352;ClPDU { -> }

class ClGetPDU 
{
  public:
    //## Constructors (specified)
      //## Operation: ClGetPDU%878671640
      ClGetPDU (ClProtocol *ai_pProtocol);

    //## Destructor (specified)
      //## Operation: ~ClGetPDU%878671641
      ~ClGetPDU ();


    //## Other Operations (specified)
      //## Operation: GetPDU%878671642; C++
      m4return_t GetPDU (ClPDU *&ao_poPDU);

      //## Operation: Read%878671643
      virtual m4int32_t Read (m4pchar_t ai_pBuffer, m4int32_t ai_iSizeBuffer);

      //## Operation: GetPDU%922271317; C++
      m4return_t GetPDU (ClPDU *&ao_poPDU, m4pchar_t ai_pcBuffer = NULL, m4uint32_t ai_uiBufferSize = 0);

  protected:
    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%379758AC0256
      //## Role: ClGetPDU::m_poProtocol%379758AD004F
      //## begin ClGetPDU::m_poProtocol%379758AD004F.role preserve=no  protected: ClProtocol { -> 1RHAN}
      ClProtocol *m_poProtocol;
      //## end ClGetPDU::m_poProtocol%379758AD004F.role

  private:
  private: //## implementation
};

//## begin ClGetPDUFile%345F707F0255.preface preserve=yes
//## end ClGetPDUFile%345F707F0255.preface

//## Class: ClGetPDUFile%345F707F0255
//	Esta función intenta conseguir una PDU a partir de un fichero que se le pasa
//	en el constructor.
//
//	Como sería un ejemplo del uso de esta clase
//
//
//	#include <stdio.h>
//	#include "pduherram.hpp"
//	#include "protocol.hpp"
//
//	int main ( void )
//
//	{
//
//		ClProtocol oProtocol;
//		FILE * fFile;
//		ClGetPDUFile *pGetPDU;
//		ClPDU *pPDU;
//		M4ClString strDumpPDU;
//
//	                // Nombre del fichero con el fuente en binario de una PDU.
//		UNICODE FILE revised
//		fFile = fopen ( fileName, "rb");
//
//		pGetPDU = new ClGetPDUFile ( fFile , &oProtocol );
//
//		pPDU = pGetPDU -> GetPDU ();
//
//		pPDU -> Dump ( strDumpPDU );
//
//
//		printf ( strDumpPDU);
//
//		return -1;
//
//	}
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClGetPDUFile : public ClGetPDU  //## Inherits: <unnamed>%345F70AB0295
{
  //## begin ClGetPDUFile%345F707F0255.initialDeclarations preserve=yes
  //## end ClGetPDUFile%345F707F0255.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClGetPDUFile%878671644
      //	Se le pasa un apuntador a un fichero abierto y funcionando.
      ClGetPDUFile (FILE *ai_pFile, ClProtocol *ai_pProtocol);

    //## Destructor (specified)
      //## Operation: ~ClGetPDUFile%878671645
      ~ClGetPDUFile ();


    //## Other Operations (specified)
      //## Operation: Read%878671646
      virtual m4int32_t Read (m4pchar_t ai_pBuffer, m4int32_t ai_iSizeBuffer);

    // Additional Public Declarations
      //## begin ClGetPDUFile%345F707F0255.public preserve=yes
      //## end ClGetPDUFile%345F707F0255.public

  protected:
    // Additional Protected Declarations
      //## begin ClGetPDUFile%345F707F0255.protected preserve=yes
      //## end ClGetPDUFile%345F707F0255.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_pFile%345F7BAF01D5
      //## begin ClGetPDUFile::m_pFile%345F7BAF01D5.attr preserve=no  private: FILE * {UA} 
      FILE *m_pFile;
      //## end ClGetPDUFile::m_pFile%345F7BAF01D5.attr

    // Additional Private Declarations
      //## begin ClGetPDUFile%345F707F0255.private preserve=yes
      //## end ClGetPDUFile%345F707F0255.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClGetPDUFile%345F707F0255.implementation preserve=yes
      //## end ClGetPDUFile%345F707F0255.implementation

};

//## begin ClGetPDUFile%345F707F0255.postscript preserve=yes
//## end ClGetPDUFile%345F707F0255.postscript

//## Class: ClGetPDUTSAP%345F850D0273
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%345F872A0126;ClTSAP { -> }

class ClGetPDUTSAP : public ClGetPDU  //## Inherits: <unnamed>%345F85460275
{
  public:
    //## Constructors (specified)
      //## Operation: ClGetPDUTSAP%878671647
      ClGetPDUTSAP (ClTSAP *ai_pTSAP, ClProtocol *ai_pProtocol);

    //## Destructor (specified)
      //## Operation: ~ClGetPDUTSAP%878671648
      ~ClGetPDUTSAP ();


    //## Other Operations (specified)
      //## Operation: Read%878671649
      virtual m4int32_t Read (m4pchar_t ai_pBuffer, m4int32_t ai_iSizeBuffer);

  protected:
  private:
    // Data Members for Class Attributes

      //## Attribute: m_pTSAP%345F86A90211
      //## begin ClGetPDUTSAP::m_pTSAP%345F86A90211.attr preserve=no  private: ClTSAP  * {UA} 
      ClTSAP  *m_pTSAP;
      //## end ClGetPDUTSAP::m_pTSAP%345F86A90211.attr

  private: //## implementation
};

//## begin ClGetPDUBuffer%346048FA02EF.preface preserve=yes
//## end ClGetPDUBuffer%346048FA02EF.preface

//## Class: ClGetPDUBuffer%346048FA02EF
//	Constructor por defecto
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClGetPDUBuffer : public ClGetPDU  //## Inherits: <unnamed>%3460491B0293
{
  //## begin ClGetPDUBuffer%346048FA02EF.initialDeclarations preserve=yes
  //## end ClGetPDUBuffer%346048FA02EF.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClGetPDUBuffer%878730735; C++
      ClGetPDUBuffer (m4pchar_t ai_pszBuffer, m4int32_t ai_lSizeBuffer, ClProtocol *ai_pProtocol);

    //## Destructor (specified)
      //## Operation: ~ClGetPDUBuffer%878730736
      //	Destructor por defecto
      ~ClGetPDUBuffer ();


    //## Other Operations (specified)
      //## Operation: Read%878730737
      //	Lee del buffer para generar una PDU.
      virtual m4int32_t Read (m4pchar_t ai_pBuffer, m4int32_t ai_iSizeBuffer);

    // Additional Public Declarations
      //## begin ClGetPDUBuffer%346048FA02EF.public preserve=yes
      //## end ClGetPDUBuffer%346048FA02EF.public

  protected:
    // Additional Protected Declarations
      //## begin ClGetPDUBuffer%346048FA02EF.protected preserve=yes
      //## end ClGetPDUBuffer%346048FA02EF.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_pszBuffer%34604B6E0068
      //## begin ClGetPDUBuffer::m_pszBuffer%34604B6E0068.attr preserve=no  private: m4pchar_t {UA} 
      m4pchar_t m_pszBuffer;
      //## end ClGetPDUBuffer::m_pszBuffer%34604B6E0068.attr

      //## Attribute: m_iBufferSize%34604B7D027C
      //## begin ClGetPDUBuffer::m_iBufferSize%34604B7D027C.attr preserve=no  private: m4int32_t {UA} 
      m4int32_t m_iBufferSize;
      //## end ClGetPDUBuffer::m_iBufferSize%34604B7D027C.attr

      //## Attribute: m_iCurrentPos%34605EB00050
      //## begin ClGetPDUBuffer::m_iCurrentPos%34605EB00050.attr preserve=no  private: m4int32_t {UA} 
      m4int32_t m_iCurrentPos;
      //## end ClGetPDUBuffer::m_iCurrentPos%34605EB00050.attr

    // Additional Private Declarations
      //## begin ClGetPDUBuffer%346048FA02EF.private preserve=yes
      //## end ClGetPDUBuffer%346048FA02EF.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClGetPDUBuffer%346048FA02EF.implementation preserve=yes
      //## end ClGetPDUBuffer%346048FA02EF.implementation

};

//## begin ClGetPDUBuffer%346048FA02EF.postscript preserve=yes
//## end ClGetPDUBuffer%346048FA02EF.postscript

//## begin ClGetPDUTSAPBuffer%36FB54170036.preface preserve=yes
//## end ClGetPDUTSAPBuffer%36FB54170036.preface

//## Class: ClGetPDUTSAPBuffer%36FB54170036
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClGetPDUTSAPBuffer : public ClGetPDU  //## Inherits: <unnamed>%36FB675101EC
{
  //## begin ClGetPDUTSAPBuffer%36FB54170036.initialDeclarations preserve=yes
  //## end ClGetPDUTSAPBuffer%36FB54170036.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClGetPDUTSAPBuffer%922441067; C++
      ClGetPDUTSAPBuffer (ClTSAP *ai_pTSAP, ClProtocol *ai_pProtocol, m4pchar_t ai_pszBuffer, m4int32_t ai_lSizeBuffer);

    //## Destructor (specified)
      //## Operation: ~ClGetPDUTSAPBuffer%922441068
      ~ClGetPDUTSAPBuffer ();


    //## Other Operations (specified)
      //## Operation: Read%922441069
      virtual m4int32_t Read (m4pchar_t ai_pBuffer, m4int32_t ai_iSizeBuffer);

    // Additional Public Declarations
      //## begin ClGetPDUTSAPBuffer%36FB54170036.public preserve=yes
      //## end ClGetPDUTSAPBuffer%36FB54170036.public

  protected:
    // Additional Protected Declarations
      //## begin ClGetPDUTSAPBuffer%36FB54170036.protected preserve=yes
      //## end ClGetPDUTSAPBuffer%36FB54170036.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_pszBuffer%36FB54BE023F
      //## begin ClGetPDUTSAPBuffer::m_pszBuffer%36FB54BE023F.attr preserve=no  private: m4pchar_t {UA} ai_pszBuffer
      m4pchar_t m_pszBuffer;
      //## end ClGetPDUTSAPBuffer::m_pszBuffer%36FB54BE023F.attr

      //## Attribute: m_iBufferSize%36FB54BE0267
      //## begin ClGetPDUTSAPBuffer::m_iBufferSize%36FB54BE0267.attr preserve=no  private: m4int32_t {UA} ai_lSizeBuffer
      m4int32_t m_iBufferSize;
      //## end ClGetPDUTSAPBuffer::m_iBufferSize%36FB54BE0267.attr

      //## Attribute: m_iCurrentPos%36FB54BE0299
      //## begin ClGetPDUTSAPBuffer::m_iCurrentPos%36FB54BE0299.attr preserve=no  private: m4int32_t {UA} 0
      m4int32_t m_iCurrentPos;
      //## end ClGetPDUTSAPBuffer::m_iCurrentPos%36FB54BE0299.attr

      //## Attribute: m_pTSAP%36FB54EF031C
      //## begin ClGetPDUTSAPBuffer::m_pTSAP%36FB54EF031C.attr preserve=no  private: ClTSAP  * {UA} ai_pTSAP
      ClTSAP  *m_pTSAP;
      //## end ClGetPDUTSAPBuffer::m_pTSAP%36FB54EF031C.attr

    // Additional Private Declarations
      //## begin ClGetPDUTSAPBuffer%36FB54170036.private preserve=yes
      //## end ClGetPDUTSAPBuffer%36FB54170036.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClGetPDUTSAPBuffer%36FB54170036.implementation preserve=yes
      //## end ClGetPDUTSAPBuffer%36FB54170036.implementation

};

//## begin ClGetPDUTSAPBuffer%36FB54170036.postscript preserve=yes
//## end ClGetPDUTSAPBuffer%36FB54170036.postscript

// Class ClGetPDU 

// Class ClGetPDUFile 

// Class ClGetPDUTSAP 

// Class ClGetPDUBuffer 

// Class ClGetPDUTSAPBuffer 

//## begin module%345F71850388.epilog preserve=yes
//## end module%345F71850388.epilog


#endif

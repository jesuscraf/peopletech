//## begin module%3977FF2A01F4.cm preserve=no
//## end module%3977FF2A01F4.cm

//## begin module%3977FF2A01F4.cp preserve=no
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
//## end module%3977FF2A01F4.cp

//## Module: pduserializer%3977FF2A01F4; Package specification
//## Subsystem: M4PDU::inc%3794593C0398
//## Source file: e:\mybuild\m4pdus\inc\pduserializer.hpp

#ifndef pduserializer_h
#define pduserializer_h 1

//## begin module%3977FF2A01F4.additionalIncludes preserve=no
//## end module%3977FF2A01F4.additionalIncludes

//## begin module%3977FF2A01F4.includes preserve=yes
//## end module%3977FF2A01F4.includes

// m4types
#include <m4types.hpp>
// pdutools
#include <pdutools.hpp>
//## begin module%3977FF2A01F4.declarations preserve=no
//## end module%3977FF2A01F4.declarations

//## begin module%3977FF2A01F4.additionalDeclarations preserve=yes
#ifdef _UNIX
	typedef uint8_t		m4pduuint8_t;
	typedef uint16_t	m4pduuint16_t;
	typedef uint32_t	m4pduuint32_t;
	typedef uint64_t	m4pduuint64_t;
#else
	typedef unsigned __int8		m4pduuint8_t;
	typedef unsigned __int16	m4pduuint16_t;
	typedef unsigned __int32	m4pduuint32_t;
	typedef unsigned __int64	m4pduuint64_t;
#endif

#define M4_PDUSERIALIZER_UINT64_SIZE	sizeof(m4pduuint64_t)
#define M4_PDUSERIALIZER_UINT32_SIZE	sizeof(m4pduuint32_t)
#define M4_PDUSERIALIZER_UINT16_SIZE	sizeof(m4pduuint16_t)
#define M4_PDUSERIALIZER_UINT8_SIZE		sizeof(m4pduuint8_t)

#define M4_PDU_ALIGN_SIZE 8
//## end module%3977FF2A01F4.additionalDeclarations


//## begin ClAdvNetHost%344B138F003C.preface preserve=yes
//## end ClAdvNetHost%344B138F003C.preface

//## Class: ClAdvNetHost%344B138F003C
//	  Esta clase existe con la única función de pasar datos del formato de red
//	al formato de host ( o viceversa ).
//	  Gracias a esta clase, nos conseguiremos independizar del sistema operativo
//	con el que estemos trabajando.
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4PDUS ClAdvNetHost 
{
  //## begin ClAdvNetHost%344B138F003C.initialDeclarations preserve=yes
  //## end ClAdvNetHost%344B138F003C.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: m4ntohul%877341252; C++
      m4pduuint32_t m4ntohul (m4pduuint32_t ai_ulNet);

      //## Operation: m4ntohui%877341253; C++
      m4pduuint16_t m4ntohui (m4pduuint16_t ai_uiNet);

      //## Operation: m4htonui%877341254; C++
      m4pduuint16_t m4htonui (m4pduuint16_t ai_uiHost);

      //## Operation: m4htonul%877341255; C++
      m4pduuint32_t m4htonul (m4pduuint32_t ai_ulHost);

      //## Operation: m4ntohulw%880122148; C++
      m4pduuint64_t m4ntohulw (m4pduuint64_t ai_ulwHost);

      //## Operation: m4htonulw%880122149; C++
      m4pduuint64_t m4htonulw (m4pduuint64_t ai_ulwHost);

    // Additional Public Declarations
      //## begin ClAdvNetHost%344B138F003C.public preserve=yes
      //## end ClAdvNetHost%344B138F003C.public

  protected:
    // Additional Protected Declarations
      //## begin ClAdvNetHost%344B138F003C.protected preserve=yes
      //## end ClAdvNetHost%344B138F003C.protected

  private:
    // Additional Private Declarations
      //## begin ClAdvNetHost%344B138F003C.private preserve=yes
      //## end ClAdvNetHost%344B138F003C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClAdvNetHost%344B138F003C.implementation preserve=yes
      //## end ClAdvNetHost%344B138F003C.implementation

};

//## begin ClAdvNetHost%344B138F003C.postscript preserve=yes
//## end ClAdvNetHost%344B138F003C.postscript

//## begin ClPDUAdvSerializer%396D652C004D.preface preserve=yes
//## end ClPDUAdvSerializer%396D652C004D.preface

//## Class: ClPDUAdvSerializer%396D652C004D
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39780743029F;m4return_t { -> }
//## Uses: <unnamed>%39DAEFC301C2;ClPairIntpChar { -> }

class ClPDUAdvSerializer 
{
  //## begin ClPDUAdvSerializer%396D652C004D.initialDeclarations preserve=yes
  //## end ClPDUAdvSerializer%396D652C004D.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClPDUAdvSerializer%963471068
      ClPDUAdvSerializer ();

    //## Destructor (specified)
      //## Operation: ~ClPDUAdvSerializer%963471069
      ~ClPDUAdvSerializer ();


    //## Other Operations (specified)
      //## Operation: AddUShort%963471070
      m4return_t AddUShort (m4uint8_t ai_usNum);

      //## Operation: AddUInt%963471071
      m4return_t AddUInt (m4uint16_t ai_uiNum);

      //## Operation: AddULong%963471072
      m4return_t AddULong (m4uint32_t ai_ulNum);

      //## Operation: AddUWLong%963471073
      m4return_t AddUWLong (m4uint64_t ai_uwlNum);

      //## Operation: AddpBuffer%963471074
      m4return_t AddpBuffer (m4pchar_t ai_pBuffer);

      //## Operation: AddpBufferC%963471075
      m4return_t AddpBufferC (m4pchar_t ai_pBuffer, m4uint32_t ai_ulBufferSize);

      //## Operation: AddBlockFormat%963471076
      m4return_t AddBlockFormat (m4uint32_t ai_ulFormat);

      //## Operation: AddPairIntpChar%963471077
      m4return_t AddPairIntpChar (ClPairIntpChar *ai_pData);

      //## Operation: GetSerializedBuffer%963471078
      m4pchar_t GetSerializedBuffer ();

      //## Operation: SetpBuffer%963471079
      m4return_t SetpBuffer (m4pchar_t ai_pBuffer);

      //## Operation: Initialize%963471080
      m4return_t Initialize ();

      //## Operation: Initialize%963471081
      m4return_t Initialize (m4pchar_t ai_pBuffer, m4uint32_t ai_ulBufferSize);

      //## Operation: GetBufferSize%963471082
      m4uint32_t GetBufferSize ();

      //## Operation: _GetUShort%963471083
      m4uint8_t _GetUShort ();

      //## Operation: _GetUInt%963471084
      m4uint16_t _GetUInt ();

      //## Operation: _GetULong%963471085
      m4uint32_t _GetULong ();

      //## Operation: _GetUWLong%963471086
      m4uint64_t _GetUWLong ();

      //## Operation: GetpBuffers%963471087
      m4pchar_t GetpBuffers (m4uint32_t ai_ulBufferSize);

      //## Operation: GetBlockFormat%963471088
      m4uint32_t GetBlockFormat ();

      //## Operation: Check4Space%963492921; C++
      m4return_t Check4Space (m4uint32_t ai_uiNeededSpace);

      //## Operation: GetUInt8%964092281; C++
      m4uint8_t GetUInt8 ();

      //## Operation: GetUInt16%964092282; C++
      m4uint16_t GetUInt16 ();

      //## Operation: GetUInt32%964092283; C++
      m4uint32_t GetUInt32 ();

      //## Operation: GetUInt64%964092284; C++
      m4uint64_t GetUInt64 ();

      //## Operation: SkipBytes%964092285; C++
      m4return_t SkipBytes (m4int_t ai_iNBytes);

      //## Operation: AddUInt8%964101575
      m4return_t AddUInt8 (m4uint8_t ai_usNum);

      //## Operation: AddUInt16%964101576; C++
      m4return_t AddUInt16 (m4uint16_t ai_uiNum);

      //## Operation: AddUInt32%964101577
      m4return_t AddUInt32 (m4uint32_t ai_ulNum);

      //## Operation: AddUInt64%964101578; C++
      m4return_t AddUInt64 (m4uint64_t ai_uwlNum);

      //## Operation: AddZero%964423559; C++
      m4return_t AddZero (m4int_t ai_iBytes);

    // Additional Public Declarations
      //## begin ClPDUAdvSerializer%396D652C004D.public preserve=yes
	  m4bool_t m_bBufferOwner;
      //## end ClPDUAdvSerializer%396D652C004D.public
  protected:
    // Data Members for Associations

      //## Association: M4PDU::<unnamed>%396DBDB1031C
      //## Role: ClPDUAdvSerializer::m_oNH%396DBDB201AB
      //## begin ClPDUAdvSerializer::m_oNH%396DBDB201AB.role preserve=no  protected: ClAdvNetHost { -> VHAN}
      ClAdvNetHost m_oNH;
      //## end ClPDUAdvSerializer::m_oNH%396DBDB201AB.role

    // Additional Protected Declarations
      //## begin ClPDUAdvSerializer%396D652C004D.protected preserve=yes
      //## end ClPDUAdvSerializer%396D652C004D.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_pBuffer%396D66D6014A
      //## begin ClPDUAdvSerializer::m_pBuffer%396D66D6014A.attr preserve=no  private: m4pchar_t {UA} NULL
      m4pchar_t m_pBuffer;
      //## end ClPDUAdvSerializer::m_pBuffer%396D66D6014A.attr

      //## Attribute: m_ulBufferSize%396D66D6014B
      //## begin ClPDUAdvSerializer::m_ulBufferSize%396D66D6014B.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_ulBufferSize;
      //## end ClPDUAdvSerializer::m_ulBufferSize%396D66D6014B.attr

      //## Attribute: m_ulIterator%396D66D6014C
      //## begin ClPDUAdvSerializer::m_ulIterator%396D66D6014C.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_ulIterator;
      //## end ClPDUAdvSerializer::m_ulIterator%396D66D6014C.attr

      //## Attribute: m_ulUsedBuffer%396D83F100D5
      //## begin ClPDUAdvSerializer::m_ulUsedBuffer%396D83F100D5.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_ulUsedBuffer;
      //## end ClPDUAdvSerializer::m_ulUsedBuffer%396D83F100D5.attr

    // Additional Private Declarations
      //## begin ClPDUAdvSerializer%396D652C004D.private preserve=yes
      //## end ClPDUAdvSerializer%396D652C004D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClPDUAdvSerializer%396D652C004D.implementation preserve=yes
      //## end ClPDUAdvSerializer%396D652C004D.implementation

};

//## begin ClPDUAdvSerializer%396D652C004D.postscript preserve=yes
//## end ClPDUAdvSerializer%396D652C004D.postscript

// Class ClAdvNetHost 

// Class ClPDUAdvSerializer 

//## begin module%3977FF2A01F4.epilog preserve=yes
//## end module%3977FF2A01F4.epilog


#endif

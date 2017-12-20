//## begin module%3468A60B0332.cm preserve=no
//## end module%3468A60B0332.cm

//## begin module%3468A60B0332.cp preserve=no
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
//## end module%3468A60B0332.cp

//## Module: servicepdus%3468A60B0332; Package specification
//## Subsystem: M4PDU::inc%3794593C0398
//## Source file: e:\mybuild\m4pdus\inc\servicepdus.hpp

#ifndef servicepdus_h
#define servicepdus_h 1

//## begin module%3468A60B0332.additionalIncludes preserve=no
//## end module%3468A60B0332.additionalIncludes

//## begin module%3468A60B0332.includes preserve=yes
//## end module%3468A60B0332.includes

// basepdu
#include "m4pdus_dll.hpp"
#include "basepdus.hpp"
//## begin module%3468A60B0332.declarations preserve=no
//## end module%3468A60B0332.declarations

//## begin module%3468A60B0332.additionalDeclarations preserve=yes
//## end module%3468A60B0332.additionalDeclarations


//## Class: ClReplyPDU%342FBAB600B3
//	Esta función se pone de forma temporal, para la primera prueba.
//	Posteriormente, solo deberá estar contenida en la clase padre.
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4PDUS ClReplyPDU : public ClPDUWithData  //## Inherits: <unnamed>%342FBAE80368
{
  public:
    //## Constructors (specified)
      //## Operation: ClReplyPDU%875780549; C++
      ClReplyPDU (m4int_t ai_iVersion = M4_PDU_40_VERSION);

    //## Destructor (specified)
      //## Operation: ~ClReplyPDU%878377096
      ~ClReplyPDU ();


    //## Other Operations (specified)
      //## Operation: GetHeadSize%876211524
      m4uint32_t GetHeadSize ();

      //## Operation: Dump%878139980
      void Dump (m4pchar_t  &ai_strBuffer);

      //## Operation: AddBlock%878468345
      //	   Añade un bloque datos, de cabecera o datos,  a nuestra PDU.
      //	   Retorna M4_SUCCESS
      m4return_t AddBlock (m4pchar_t  &ai_pBlock);

      //## Operation: GetBinaryHead%878662676
      //	   Pasa todos los campos significativos de la PDU a una cadena de
      //	caracteres. El formato de los campos numéricos será binario.
      m4return_t GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize);

      //## Operation: GetReturnCode%878723090
      m4uint32_t GetReturnCode ();

      //## Operation: SetReturnCode%878723091
      void SetReturnCode (m4uint32_t ai_ulReturnCode);

      //## Operation: GetServiceId%881934553
      m4uint32_t GetServiceId ();

      //## Operation: SetServiceId%881934554
      void SetServiceId (m4uint32_t ai_ulServiceId);

      //## Operation: GetOperationId%881934555
      m4uint32_t GetOperationId ();

      //## Operation: SetOperationId%881934556
      void SetOperationId (m4uint32_t ai_ulOperationId);

      //## Operation: GetNextSizeBlock%881934560
      m4uint32_t GetNextSizeBlock (m4pchar_t &ao_pBlock);

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_ulReturnCode%343233E001C4
      //## begin ClReplyPDU::m_ulReturnCode%343233E001C4.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_ulReturnCode;
      //## end ClReplyPDU::m_ulReturnCode%343233E001C4.attr

      //## Attribute: m_ulServiceId%3491402100A6
      //## begin ClReplyPDU::m_ulServiceId%3491402100A6.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_ulServiceId;
      //## end ClReplyPDU::m_ulServiceId%3491402100A6.attr

      //## Attribute: m_ulOperationId%3491402100C4
      //## begin ClReplyPDU::m_ulOperationId%3491402100C4.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_ulOperationId;
      //## end ClReplyPDU::m_ulOperationId%3491402100C4.attr

  private:
  private: //## implementation
};

//## Class: ClRequestPDU%342FBA960107
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4PDUS ClRequestPDU : public ClPDUWithData  //## Inherits: <unnamed>%342FBAB30023
{
  public:
    //## Constructors (specified)
      //## Operation: ClRequestPDU%875692543; C++
      ClRequestPDU (m4int_t ai_iVersion = M4_PDU_40_VERSION);

    //## Destructor (specified)
      //## Operation: ~ClRequestPDU%878377095
      ~ClRequestPDU ();


    //## Other Operations (specified)
      //## Operation: GetHeadSize%876211523
      m4uint32_t GetHeadSize ();

      //## Operation: Dump%878139979
      void Dump (m4pchar_t &ai_strBuffer);

      //## Operation: AddBlock%878468344
      //	   Añade un bloque datos, de cabecera o datos,  a nuestra PDU.
      //	   Retorna M4_SUCCESS
      m4return_t AddBlock (m4pchar_t  &ai_pBlock);

      //## Operation: GetBinaryHead%878662675
      //	   Pasa todos los campos significativos de la PDU a una cadena de
      //	caracteres. El formato de los campos numéricos será binario.
      m4return_t GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize);

      //## Operation: GetFlags%878723086
      m4uint32_t GetFlags ();

      //## Operation: SetFlags%878723087
      void SetFlags (m4uint32_t ai_ulFlags);

      //## Operation: GetCharge%878723088
      m4uint16_t GetCharge ();

      //## Operation: SetCharge%878723089
      void SetCharge (m4uint16_t ai_uiCharge);

      //## Operation: GetServiceId%880451467
      m4uint32_t GetServiceId (m4pchar_t &ao_pServiceName);

      //## Operation: SetServiceId%880451468
      void SetServiceId (m4pchar_t ai_pServiceName, m4uint32_t ai_ulServiceNameLength);

      //## Operation: GetOperationId%880451469
      m4uint32_t GetOperationId ();

      //## Operation: SetOperationId%880451470
      void SetOperationId (m4uint32_t ai_ulOperationId);

      //## Operation: GetNextSizeBlock%881934559
      m4uint32_t GetNextSizeBlock (m4pchar_t &ao_pBlock);

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_ulServiceId%347AA1D700D9
      //## begin ClRequestPDU::m_ulServiceId%347AA1D700D9.attr preserve=no  protected: ClPairLongIntpChar {UA} 
      ClPairLongIntpChar m_ulServiceId;
      //## end ClRequestPDU::m_ulServiceId%347AA1D700D9.attr

      //## Attribute: m_ulOperationId%347AA1D7013D
      //## begin ClRequestPDU::m_ulOperationId%347AA1D7013D.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_ulOperationId;
      //## end ClRequestPDU::m_ulOperationId%347AA1D7013D.attr

      //## Attribute: m_ulFlags%342FBBC00355
      //## begin ClRequestPDU::m_ulFlags%342FBBC00355.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_ulFlags;
      //## end ClRequestPDU::m_ulFlags%342FBBC00355.attr

      //## Attribute: m_uiCharge%342FBBCC01B7
      //## begin ClRequestPDU::m_uiCharge%342FBBCC01B7.attr preserve=no  protected: m4uint16_t {UA} 
      m4uint16_t m_uiCharge;
      //## end ClRequestPDU::m_uiCharge%342FBBCC01B7.attr

  private:
  private: //## implementation
};

// Class ClReplyPDU 

// Class ClRequestPDU 

//## begin module%3468A60B0332.epilog preserve=yes
//## end module%3468A60B0332.epilog


#endif

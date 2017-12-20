//## begin module%3468A64C024F.cm preserve=no
//## end module%3468A64C024F.cm

//## begin module%3468A64C024F.cp preserve=no
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
//## end module%3468A64C024F.cp

//## Module: controlpdus%3468A64C024F; Package specification
//## Subsystem: M4PDU::inc%3794593C0398
//## Source file: e:\mybuild\m4pdus\inc\controlpdus.hpp

#ifndef controlpdus_h
#define controlpdus_h 1

//## begin module%3468A64C024F.additionalIncludes preserve=no
//## end module%3468A64C024F.additionalIncludes

//## begin module%3468A64C024F.includes preserve=yes
//## end module%3468A64C024F.includes

// basepdu
#include <basepdus.hpp>
#include "m4stl.hpp"
//## begin module%3468A64C024F.declarations preserve=no
//## end module%3468A64C024F.declarations

//## begin module%3468A64C024F.additionalDeclarations preserve=yess
const m4uint16_t M4_NORMAL_LOGIN			=	0x0000 ;
const m4uint16_t M4_GET_CERTIFICATE_FLAG	=	0x0001 ;
const m4uint16_t M4_CERTIFICATE_LOGIN		=	0x0002 ;
//## end module%3468A64C024F.additionalDeclarations


//## begin ClIntDeque%347174200169.preface preserve=yes
//## end ClIntDeque%347174200169.preface

//## Class: ClIntDeque%347174200169
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3475A31B00F4;deque { -> }

typedef deque<m4uint16_t> ClIntDeque;

//## begin ClIntDeque%347174200169.postscript preserve=yes
//## end ClIntDeque%347174200169.postscript

//## begin ClControlPDU%345F0D430012.preface preserve=yes
//## end ClControlPDU%345F0D430012.preface

//## Class: ClControlPDU%345F0D430012
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4PDUS ClControlPDU : public ClPDU  //## Inherits: <unnamed>%34E1DB200075
{
  //## begin ClControlPDU%345F0D430012.initialDeclarations preserve=yes
  //## end ClControlPDU%345F0D430012.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClControlPDU%878806460; C++
      ClControlPDU (m4int_t ai_iVersion = M4_PDU_40_VERSION);

    //## Destructor (specified)
      //## Operation: ~ClControlPDU%878806461
      ~ClControlPDU ();


    //## Other Operations (specified)
      //## Operation: GetBinaryHead%878662677
      //	   Pasa todos los campos significativos de la PDU a una cadena de
      //	caracteres. El formato de los campos numéricos será binario.
      m4return_t GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize);

      //## Operation: GetHeadSize%878806448
      virtual m4uint32_t GetHeadSize ();

      //## Operation: Dump%879157701
      void Dump (m4pchar_t  &ao_pTexto);

      //## Operation: GetNextBlock%879502155
      m4return_t GetNextBlock (m4uint32_t ai_ulReadSize, m4pchar_t &ao_pBlock, m4uint32_t &ao_ulBlockSize);

      //## Operation: GetPDUSize%970646986
      virtual m4uint32_t GetPDUSize ();

    // Additional Public Declarations
      //## begin ClControlPDU%345F0D430012.public preserve=yes
      //## end ClControlPDU%345F0D430012.public

  protected:
    // Additional Protected Declarations
      //## begin ClControlPDU%345F0D430012.protected preserve=yes
      //## end ClControlPDU%345F0D430012.protected

  private:
    // Additional Private Declarations
      //## begin ClControlPDU%345F0D430012.private preserve=yes
      //## end ClControlPDU%345F0D430012.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClControlPDU%345F0D430012.implementation preserve=yes
      //## end ClControlPDU%345F0D430012.implementation

};

//## begin ClControlPDU%345F0D430012.postscript preserve=yes
//## end ClControlPDU%345F0D430012.postscript

//## begin ClErrorPDU%342FBA02026D.preface preserve=yes
//## end ClErrorPDU%342FBA02026D.preface

//## Class: ClErrorPDU%342FBA02026D
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4PDUS ClErrorPDU : public ClControlPDU  //## Inherits: <unnamed>%396D9FD601C4
{
  //## begin ClErrorPDU%342FBA02026D.initialDeclarations preserve=yes
  //## end ClErrorPDU%342FBA02026D.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClErrorPDU%875780548; C++
      ClErrorPDU (m4int_t ai_iVersion = M4_PDU_40_VERSION);

    //## Destructor (specified)
      //## Operation: ~ClErrorPDU%878377094
      ~ClErrorPDU ();


    //## Other Operations (specified)
      //## Operation: GetNextSizeBlock%875780567
      m4uint32_t GetNextSizeBlock (m4pchar_t &ao_pBlock);

      //## Operation: AddBlock%875780568
      m4return_t AddBlock (m4pchar_t &ai_pBlock);

      //## Operation: GetHeadSize%876211534
      m4uint32_t GetHeadSize ();

      //## Operation: Dump%878139978
      void Dump (m4pchar_t &ai_strBuffer);

      //## Operation: GetErrorCode%878662672
      m4uint32_t GetErrorCode ();

      //## Operation: SetErrorCode%878662673
      void SetErrorCode (m4uint32_t ai_ulErrorCode);

      //## Operation: GetBinaryHead%878662674
      //	   Pasa todos los campos significativos de la PDU a una cadena de
      //	caracteres. El formato de los campos numéricos será binario.
      m4return_t GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize);

    // Additional Public Declarations
      //## begin ClErrorPDU%342FBA02026D.public preserve=yes
      //## end ClErrorPDU%342FBA02026D.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_ulErrorCode%345F778F0242
      //## begin ClErrorPDU::m_ulErrorCode%345F778F0242.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_ulErrorCode;
      //## end ClErrorPDU::m_ulErrorCode%345F778F0242.attr

    // Additional Protected Declarations
      //## begin ClErrorPDU%342FBA02026D.protected preserve=yes
      //## end ClErrorPDU%342FBA02026D.protected

  private:
    // Additional Private Declarations
      //## begin ClErrorPDU%342FBA02026D.private preserve=yes
      //## end ClErrorPDU%342FBA02026D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClErrorPDU%342FBA02026D.implementation preserve=yes
      //## end ClErrorPDU%342FBA02026D.implementation

};

//## begin ClErrorPDU%342FBA02026D.postscript preserve=yes
//## end ClErrorPDU%342FBA02026D.postscript

//## begin ClAckPDU%342FB9EF0201.preface preserve=yes
//## end ClAckPDU%342FB9EF0201.preface

//## Class: ClAckPDU%342FB9EF0201
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4PDUS ClAckPDU : public ClPDUWithData  //## Inherits: <unnamed>%359276350179
{
  //## begin ClAckPDU%342FB9EF0201.initialDeclarations preserve=yes
  //## end ClAckPDU%342FB9EF0201.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClAckPDU%875780547; C++
      ClAckPDU (m4int_t ai_iVersion = M4_PDU_40_VERSION);

    //## Destructor (specified)
      //## Operation: ~ClAckPDU%878377093
      ~ClAckPDU ();


    //## Other Operations (specified)
      //## Operation: GetNextSizeBlock%875780565
      m4uint32_t GetNextSizeBlock (m4pchar_t &ao_pBlock);

      //## Operation: AddBlock%875780566
      m4return_t AddBlock (m4pchar_t  &ai_pBlock);

      //## Operation: GetHeadSize%876211533
      m4uint32_t GetHeadSize ();

      //## Operation: Dump%878139977
      void Dump (m4pchar_t  &ai_strBuffer);

      //## Operation: GetBinaryHead%878662671
      //	   Pasa todos los campos significativos de la PDU a una cadena de
      //	caracteres. El formato de los campos numéricos será binario.
      m4return_t GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize);

      //## Operation: GetWhy%892545848
      m4uint16_t GetWhy ();

      //## Operation: SetWhy%892545849
      void SetWhy (m4uint16_t ai_uiWhy);

    // Additional Public Declarations
      //## begin ClAckPDU%342FB9EF0201.public preserve=yes
      //## end ClAckPDU%342FB9EF0201.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_uiWhy%35332B120056
      //## begin ClAckPDU::m_uiWhy%35332B120056.attr preserve=no  protected: m4uint16_t {UA} 
      m4uint16_t m_uiWhy;
      //## end ClAckPDU::m_uiWhy%35332B120056.attr

      //## Attribute: m_ulRequestId%35332B13018E
      //## begin ClAckPDU::m_ulRequestId%35332B13018E.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_ulRequestId;
      //## end ClAckPDU::m_ulRequestId%35332B13018E.attr

    // Additional Protected Declarations
      //## begin ClAckPDU%342FB9EF0201.protected preserve=yes
      //## end ClAckPDU%342FB9EF0201.protected

  private:
    // Additional Private Declarations
      //## begin ClAckPDU%342FB9EF0201.private preserve=yes
      //## end ClAckPDU%342FB9EF0201.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClAckPDU%342FB9EF0201.implementation preserve=yes
      //## end ClAckPDU%342FB9EF0201.implementation

};

//## begin ClAckPDU%342FB9EF0201.postscript preserve=yes
//## end ClAckPDU%342FB9EF0201.postscript

//## begin ClDisconnectPDU%342FB9770169.preface preserve=yes
//## end ClDisconnectPDU%342FB9770169.preface

//## Class: ClDisconnectPDU%342FB9770169
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4PDUS ClDisconnectPDU : public ClControlPDU  //## Inherits: <unnamed>%396D9FE0020F
{
  //## begin ClDisconnectPDU%342FB9770169.initialDeclarations preserve=yes
  //## end ClDisconnectPDU%342FB9770169.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClDisconnectPDU%875780545; C++
      ClDisconnectPDU (m4int_t ai_iVersion = M4_PDU_40_VERSION);

    //## Destructor (specified)
      //## Operation: ~ClDisconnectPDU%878377091
      ~ClDisconnectPDU ();


    //## Other Operations (specified)
      //## Operation: GetNextSizeBlock%875780563
      m4uint32_t GetNextSizeBlock (m4pchar_t &ao_pBlock);

      //## Operation: AddBlock%875780564
      m4return_t AddBlock (m4pchar_t  &ai_pBlock);

      //## Operation: GetHeadSize%876211531
      m4uint32_t GetHeadSize ();

      //## Operation: Dump%878139975
      void Dump (m4pchar_t  &ai_strBuffer);

      //## Operation: GetWhy%878662661
      m4uint16_t GetWhy ();

      //## Operation: SetWhy%878662662
      void SetWhy (m4uint16_t ai_uiWhy);

      //## Operation: SetUserId%889708088
      void SetUserId (m4handle_t ai_hUserId);

      //## Operation: GetUserId%889708089
      m4handle_t GetUserId ();

      //## Operation: GetBinaryHead%878662663
      //	   Pasa todos los campos significativos de la PDU a una cadena de
      //	caracteres. El formato de los campos numéricos será binario.
      m4return_t GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize);

    // Additional Public Declarations
      //## begin ClDisconnectPDU%342FB9770169.public preserve=yes
      //## end ClDisconnectPDU%342FB9770169.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_uiWhy%3432391E019C
      //## begin ClDisconnectPDU::m_uiWhy%3432391E019C.attr preserve=no  protected: m4uint16_t {UA} 
      m4uint16_t m_uiWhy;
      //## end ClDisconnectPDU::m_uiWhy%3432391E019C.attr

      //## Attribute: m_hUserId%3505190403B3
      //## begin ClDisconnectPDU::m_hUserId%3505190403B3.attr preserve=no  protected: m4handle_t {UA} 0
      m4handle_t m_hUserId;
      //## end ClDisconnectPDU::m_hUserId%3505190403B3.attr

    // Additional Protected Declarations
      //## begin ClDisconnectPDU%342FB9770169.protected preserve=yes
      //## end ClDisconnectPDU%342FB9770169.protected

  private:
    // Additional Private Declarations
      //## begin ClDisconnectPDU%342FB9770169.private preserve=yes
      //## end ClDisconnectPDU%342FB9770169.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDisconnectPDU%342FB9770169.implementation preserve=yes
      //## end ClDisconnectPDU%342FB9770169.implementation

};

//## begin ClDisconnectPDU%342FB9770169.postscript preserve=yes
//## end ClDisconnectPDU%342FB9770169.postscript

//## Class: ClAuthenticationPDU%342FB54002EE
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4PDUS ClAuthenticationPDU : public ClPDUWithData  //## Inherits: <unnamed>%344B93BF0011
{
  public:
    //## Constructors (specified)
      //## Operation: ClAuthenticationPDU%875780541; C++
      ClAuthenticationPDU (m4int_t ai_iVersion = M4_PDU_40_VERSION);

    //## Destructor (specified)
      //## Operation: ~ClAuthenticationPDU%878377087
      ~ClAuthenticationPDU ();


    //## Other Operations (specified)
      //## Operation: GetNextSizeBlock%875780555
      m4uint32_t GetNextSizeBlock (m4pchar_t &ao_pBlock);

      //## Operation: AddBlock%875780556
      m4return_t AddBlock (m4pchar_t &ai_pBlock);

      //## Operation: GetHeadSize%876211527
      m4uint32_t GetHeadSize ();

      //## Operation: Dump%878139971
      void Dump (m4pchar_t  &ai_strBuffer);

      //## Operation: GetFlags%878662639
      m4uint16_t GetFlags ();

      //## Operation: SetFlags%878662640
      void SetFlags (m4uint16_t ai_uiFlags);

      //## Operation: GetDefaultRole%932487953
      m4bool_t GetDefaultRole ();

      //## Operation: GetUserId%878662641
      m4uint16_t GetUserId (m4pchar_t &ao_pUserId);

      //## Operation: SetUserId%878662642
      void SetUserId (m4pchar_t ai_pUserId, m4uint16_t ai_uiIdSize);

      //## Operation: GetPassword%878662643
      m4uint16_t GetPassword (m4pchar_t &ao_pPassword);

      //## Operation: SetPassword%878662644
      void SetPassword (m4pchar_t ai_pPassword, m4uint16_t ai_uiPasswordLength);

      //## Operation: GetBinaryHead%878662645
      //	   Pasa todos los campos de la cabecera de la PDU a una cadena de
      //	caracteres.. El formato de los campos numéricos será binario.
      m4return_t GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize);

      //## Operation: SetDefaultRole%932487956; C++
      void SetDefaultRole (m4bool_t ai_DefaultRole);

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_uiFlags%342FB8E0018A
      //## begin ClAuthenticationPDU::m_uiFlags%342FB8E0018A.attr preserve=no  protected: m4uint16_t {UA} 0
      m4uint16_t m_uiFlags;
      //## end ClAuthenticationPDU::m_uiFlags%342FB8E0018A.attr

      //## Attribute: m_oUserId%342FB8E601E2
      //## begin ClAuthenticationPDU::m_oUserId%342FB8E601E2.attr preserve=no  protected: ClPairIntpChar {UA} 
      ClPairIntpChar m_oUserId;
      //## end ClAuthenticationPDU::m_oUserId%342FB8E601E2.attr

      //## Attribute: m_oPassword%342FB92C0175
      //## begin ClAuthenticationPDU::m_oPassword%342FB92C0175.attr preserve=no  protected: ClPairIntpChar {UA} 
      ClPairIntpChar m_oPassword;
      //## end ClAuthenticationPDU::m_oPassword%342FB92C0175.attr

      //## Attribute: m_ulDefaultRole%3794BB1C0257
      //## begin ClAuthenticationPDU::m_ulDefaultRole%3794BB1C0257.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_ulDefaultRole;
      //## end ClAuthenticationPDU::m_ulDefaultRole%3794BB1C0257.attr

  private:
  private: //## implementation
};

//## begin ClConnectPDU%342FA0480209.preface preserve=yes
//## end ClConnectPDU%342FA0480209.preface

//## Class: ClConnectPDU%342FA0480209
//	Clase que empaqueta la PDU de inicio de conexión.
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4PDUS ClConnectPDU : public ClPDUWithData  //## Inherits: <unnamed>%3575809C02EF
{
  //## begin ClConnectPDU%342FA0480209.initialDeclarations preserve=yes
  //## end ClConnectPDU%342FA0480209.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClConnectPDU%875780539; C++
      ClConnectPDU (m4int_t ai_iVersion = M4_PDU_40_VERSION);

    //## Destructor (specified)
      //## Operation: ~ClConnectPDU%878377085
      ~ClConnectPDU ();


    //## Other Operations (specified)
      //## Operation: GetNextSizeBlock%875780551
      m4uint32_t GetNextSizeBlock (m4pchar_t &ao_pBlock);

      //## Operation: AddBlock%875780552
      m4return_t AddBlock (m4pchar_t  &ai_pBlock);

      //## Operation: GetHeadSize%876211525
      m4uint32_t GetHeadSize ();

      //## Operation: Dump%876384556
      void Dump (m4pchar_t &ai_strBuffer);

      //## Operation: GetBinaryHead%878662625
      //	   Pasa todos los campos significativos de la PDU a una cadena de
      //	caracteres. El formato de los campos numéricos será binario.
      m4return_t GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize);

    // Data Members for Class Attributes

      //## Attribute: m_uiHostId%3497D668004E
      //## begin ClConnectPDU::m_uiHostId%3497D668004E.attr preserve=no  public: m4uint16_t {UA} 0
      m4uint16_t m_uiHostId;
      //## end ClConnectPDU::m_uiHostId%3497D668004E.attr

    // Data Members for Associations

      //## Association: M4PDU::<unnamed>%378C52B7009B
      //## Role: ClConnectPDU::m_oVersions%378C52B7036D
      //## begin ClConnectPDU::m_oVersions%378C52B7036D.role preserve=no  public: ClIntDeque { -> 1VHAN}
      ClIntDeque m_oVersions;
      //## end ClConnectPDU::m_oVersions%378C52B7036D.role

    // Additional Public Declarations
      //## begin ClConnectPDU%342FA0480209.public preserve=yes
      //## end ClConnectPDU%342FA0480209.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_ulMagicNumber%342FB03202D9
      //## begin ClConnectPDU::m_ulMagicNumber%342FB03202D9.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_ulMagicNumber;
      //## end ClConnectPDU::m_ulMagicNumber%342FB03202D9.attr

      //## Attribute: m_ulFlags%345F13BE01E4
      //## begin ClConnectPDU::m_ulFlags%345F13BE01E4.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_ulFlags;
      //## end ClConnectPDU::m_ulFlags%345F13BE01E4.attr

      //## Attribute: m_ulSiteId%342FB06701B3
      //## begin ClConnectPDU::m_ulSiteId%342FB06701B3.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_ulSiteId;
      //## end ClConnectPDU::m_ulSiteId%342FB06701B3.attr

      //## Attribute: m_uiNVersions%342FB08C0007
      //## begin ClConnectPDU::m_uiNVersions%342FB08C0007.attr preserve=no  protected: m4uint16_t {UA} 
      m4uint16_t m_uiNVersions;
      //## end ClConnectPDU::m_uiNVersions%342FB08C0007.attr

      //## Attribute: m_oAuthenticationDH%345F15490214
      //## begin ClConnectPDU::m_oAuthenticationDH%345F15490214.attr preserve=no  protected: ClPairIntpChar {UA} 
      ClPairIntpChar m_oAuthenticationDH;
      //## end ClConnectPDU::m_oAuthenticationDH%345F15490214.attr

      //## Attribute: m_ulClientSign%345F151C03B4
      //## begin ClConnectPDU::m_ulClientSign%345F151C03B4.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_ulClientSign;
      //## end ClConnectPDU::m_ulClientSign%345F151C03B4.attr

      //## Attribute: m_oParam1DH%347400080277
      //## begin ClConnectPDU::m_oParam1DH%347400080277.attr preserve=no  protected: ClPairIntpChar {UA} 
      ClPairIntpChar m_oParam1DH;
      //## end ClConnectPDU::m_oParam1DH%347400080277.attr

      //## Attribute: m_oParam2DH%347400270253
      //## begin ClConnectPDU::m_oParam2DH%347400270253.attr preserve=no  protected: ClPairIntpChar {UA} 
      ClPairIntpChar m_oParam2DH;
      //## end ClConnectPDU::m_oParam2DH%347400270253.attr

      //## Attribute: m_oPublicPasswordDH%3474004B03D2
      //## begin ClConnectPDU::m_oPublicPasswordDH%3474004B03D2.attr preserve=no  protected: ClPairIntpChar {UA} 
      ClPairIntpChar m_oPublicPasswordDH;
      //## end ClConnectPDU::m_oPublicPasswordDH%3474004B03D2.attr

      //## Attribute: m_oClientSign%3474008901F0
      //## begin ClConnectPDU::m_oClientSign%3474008901F0.attr preserve=no  protected: ClPairIntpChar {UA} 
      ClPairIntpChar m_oClientSign;
      //## end ClConnectPDU::m_oClientSign%3474008901F0.attr

      //## Attribute: m_ulRequestId%35C5D8C302FF
      //## begin ClConnectPDU::m_ulRequestId%35C5D8C302FF.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_ulRequestId;
      //## end ClConnectPDU::m_ulRequestId%35C5D8C302FF.attr

    // Additional Protected Declarations
      //## begin ClConnectPDU%342FA0480209.protected preserve=yes
      //## end ClConnectPDU%342FA0480209.protected

  private:

    //## Other Operations (specified)
      //## Operation: InitializeIteration%880122145
      void InitializeIteration ();

    // Data Members for Class Attributes

      //## Attribute: m_oIterator%3474255401AE
      //## begin ClConnectPDU::m_oIterator%3474255401AE.attr preserve=no  private: ClIntDeque :: iterator {UA} 
      ClIntDeque :: iterator m_oIterator;
      //## end ClConnectPDU::m_oIterator%3474255401AE.attr

    // Additional Private Declarations
      //## begin ClConnectPDU%342FA0480209.private preserve=yes
      //## end ClConnectPDU%342FA0480209.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClConnectPDU%342FA0480209.implementation preserve=yes
      //## end ClConnectPDU%342FA0480209.implementation

};

//## begin ClConnectPDU%342FA0480209.postscript preserve=yes
//## end ClConnectPDU%342FA0480209.postscript

//## begin ClSetRolePDU%350959CF0278.preface preserve=yes
//## end ClSetRolePDU%350959CF0278.preface

//## Class: ClSetRolePDU%350959CF0278
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4PDUS ClSetRolePDU : public ClControlPDU  //## Inherits: <unnamed>%35095ACD01E7
{
  //## begin ClSetRolePDU%350959CF0278.initialDeclarations preserve=yes
  //## end ClSetRolePDU%350959CF0278.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSetRolePDU%889805593; C++
      ClSetRolePDU (m4int_t ai_iVersion = M4_PDU_40_VERSION);

    //## Destructor (specified)
      //## Operation: ~ClSetRolePDU%889805594
      ~ClSetRolePDU ();


    //## Other Operations (specified)
      //## Operation: GetNextSizeBlock%889805595
      m4uint32_t GetNextSizeBlock (m4pchar_t &ao_pBlock);

      //## Operation: AddBlock%889805596
      m4return_t AddBlock (m4pchar_t &ai_pBlock);

      //## Operation: GetHeadSize%889805598
      m4uint32_t GetHeadSize ();

      //## Operation: Dump%889805601
      void Dump (m4pchar_t &ai_strBuffer);

      //## Operation: GetBinaryHead%889805604
      //	   Pasa todos los campos significativos de la PDU a una cadena de
      //	caracteres. El formato de los campos numéricos será binario.
      m4return_t GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize);

      //## Operation: GetRole%896888578
      m4uint16_t GetRole (m4pchar_t &ao_pRoleName);

      //## Operation: SetRole%896888579
      void SetRole (m4pchar_t ai_pRoleName, m4uint16_t ai_uiRoleLength);

      //## Operation: SetDefaultRole%897314596
      void SetDefaultRole (m4bool_t ai_bDefaultRole);

      //## Operation: IsDefaultRole%897314597
      m4bool_t IsDefaultRole ();

    // Additional Public Declarations
      //## begin ClSetRolePDU%350959CF0278.public preserve=yes
      //## end ClSetRolePDU%350959CF0278.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_oRole%35756E7E00DD
      //## begin ClSetRolePDU::m_oRole%35756E7E00DD.attr preserve=no  protected: ClPairIntpChar {UA} 
      ClPairIntpChar m_oRole;
      //## end ClSetRolePDU::m_oRole%35756E7E00DD.attr

    // Additional Protected Declarations
      //## begin ClSetRolePDU%350959CF0278.protected preserve=yes
      //## end ClSetRolePDU%350959CF0278.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_bDefaultRole%357BEDCF03CA
      //## begin ClSetRolePDU::m_bDefaultRole%357BEDCF03CA.attr preserve=no  private: m4uint16_t {UA} M4_FALSE
      m4uint16_t m_bDefaultRole;
      //## end ClSetRolePDU::m_bDefaultRole%357BEDCF03CA.attr

    // Additional Private Declarations
      //## begin ClSetRolePDU%350959CF0278.private preserve=yes
      //## end ClSetRolePDU%350959CF0278.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSetRolePDU%350959CF0278.implementation preserve=yes
      //## end ClSetRolePDU%350959CF0278.implementation

};

//## begin ClSetRolePDU%350959CF0278.postscript preserve=yes
//## end ClSetRolePDU%350959CF0278.postscript

//## begin ClChangePasswordPDU%35912C8D0201.preface preserve=yes
//## end ClChangePasswordPDU%35912C8D0201.preface

//## Class: ClChangePasswordPDU%35912C8D0201
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4PDUS ClChangePasswordPDU : public ClControlPDU  //## Inherits: <unnamed>%35912DDB00CA
{
  //## begin ClChangePasswordPDU%35912C8D0201.initialDeclarations preserve=yes
  //## end ClChangePasswordPDU%35912C8D0201.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClChangePasswordPDU%898707101; C++
      ClChangePasswordPDU (m4int_t ai_iVersion = M4_PDU_40_VERSION);

    //## Destructor (specified)
      //## Operation: ~ClChangePasswordPDU%898707102
      ~ClChangePasswordPDU ();


    //## Other Operations (specified)
      //## Operation: GetNextSizeBlock%898707103
      m4uint32_t GetNextSizeBlock (m4pchar_t &ao_pBlock);

      //## Operation: AddBlock%898707104
      m4return_t AddBlock (m4pchar_t &ai_pBlock);

      //## Operation: GetHeadSize%898707106
      m4uint32_t GetHeadSize ();

      //## Operation: Dump%898707109
      void Dump (m4pchar_t  &ai_strBuffer);

      //## Operation: GetCurrentPassword%898707110
      m4uint16_t GetCurrentPassword (m4pchar_t &ao_pPass);

      //## Operation: SetCurrentPassword%898707111
      void SetCurrentPassword (m4pchar_t ai_pPass, m4uint16_t ai_uiPassSize);

      //## Operation: GetNewPassword%898707112
      m4uint16_t GetNewPassword (m4pchar_t &ao_pPassword);

      //## Operation: SetNewPassword%898707113
      void SetNewPassword (m4pchar_t ai_pPassword, m4uint16_t ai_uiPasswordLength);

      //## Operation: GetBinaryHead%898707114
      //	   Pasa todos los campos de la cabecera de la PDU a una cadena de
      //	caracteres.. El formato de los campos numéricos será binario.
      m4return_t GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize);

    // Additional Public Declarations
      //## begin ClChangePasswordPDU%35912C8D0201.public preserve=yes
      //## end ClChangePasswordPDU%35912C8D0201.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_oCurrentPassword%35912D1C0351
      //## begin ClChangePasswordPDU::m_oCurrentPassword%35912D1C0351.attr preserve=no  protected: ClPairIntpChar {UA} 
      ClPairIntpChar m_oCurrentPassword;
      //## end ClChangePasswordPDU::m_oCurrentPassword%35912D1C0351.attr

      //## Attribute: m_oNewPassword%35912D1D00C7
      //## begin ClChangePasswordPDU::m_oNewPassword%35912D1D00C7.attr preserve=no  protected: ClPairIntpChar {UA} 
      ClPairIntpChar m_oNewPassword;
      //## end ClChangePasswordPDU::m_oNewPassword%35912D1D00C7.attr

    // Additional Protected Declarations
      //## begin ClChangePasswordPDU%35912C8D0201.protected preserve=yes
      //## end ClChangePasswordPDU%35912C8D0201.protected

  private:
    // Additional Private Declarations
      //## begin ClChangePasswordPDU%35912C8D0201.private preserve=yes
      //## end ClChangePasswordPDU%35912C8D0201.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClChangePasswordPDU%35912C8D0201.implementation preserve=yes
      //## end ClChangePasswordPDU%35912C8D0201.implementation

};

//## begin ClChangePasswordPDU%35912C8D0201.postscript preserve=yes
//## end ClChangePasswordPDU%35912C8D0201.postscript

//## begin ClCancelPDU%35924B610246.preface preserve=yes
//## end ClCancelPDU%35924B610246.preface

//## Class: ClCancelPDU%35924B610246
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4PDUS ClCancelPDU : public ClControlPDU  //## Inherits: <unnamed>%35924BE20346
{
  //## begin ClCancelPDU%35924B610246.initialDeclarations preserve=yes
  //## end ClCancelPDU%35924B610246.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCancelPDU%898780370; C++
      ClCancelPDU (m4int_t ai_iVersion = M4_PDU_40_VERSION);

    //## Destructor (specified)
      //## Operation: ~ClCancelPDU%898780371
      ~ClCancelPDU ();


    //## Other Operations (specified)
      //## Operation: GetNextSizeBlock%898780372
      m4uint32_t GetNextSizeBlock (m4pchar_t &ao_pBlock);

      //## Operation: AddBlock%898780373
      m4return_t AddBlock (m4pchar_t &ai_pBlock);

      //## Operation: GetHeadSize%898780375
      m4uint32_t GetHeadSize ();

      //## Operation: Dump%898780378
      void Dump (m4pchar_t &ai_strBuffer);

      //## Operation: GetBinaryHead%898780381
      //	   Pasa todos los campos significativos de la PDU a una cadena de
      //	caracteres. El formato de los campos numéricos será binario.
      m4return_t GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize);

      //## Operation: SetCancelRequestId%964182213; C++
      void SetCancelRequestId (m4uint32_t ai_ulCancelRequestId);

      //## Operation: GetCancelRequestId%964182214; C++
      m4uint32_t GetCancelRequestId ();

    // Additional Public Declarations
      //## begin ClCancelPDU%35924B610246.public preserve=yes
      //## end ClCancelPDU%35924B610246.public

  protected:
    // Additional Protected Declarations
      //## begin ClCancelPDU%35924B610246.protected preserve=yes
      //## end ClCancelPDU%35924B610246.protected

  private:
    // Additional Private Declarations
      //## begin ClCancelPDU%35924B610246.private preserve=yes
      //## end ClCancelPDU%35924B610246.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCancelPDU%35924B610246.implementation preserve=yes
      //## end ClCancelPDU%35924B610246.implementation

};

//## begin ClCancelPDU%35924B610246.postscript preserve=yes
//## end ClCancelPDU%35924B610246.postscript

//## begin ClNewCancelPDU%39856E0002FC.preface preserve=yes
//## end ClNewCancelPDU%39856E0002FC.preface

//## Class: ClNewCancelPDU%39856E0002FC
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4PDUS ClNewCancelPDU : public ClPDUWithData  //## Inherits: <unnamed>%39856E170273
{
  //## begin ClNewCancelPDU%39856E0002FC.initialDeclarations preserve=yes
  //## end ClNewCancelPDU%39856E0002FC.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClNewCancelPDU%965046694; C++
      ClNewCancelPDU (m4int_t ai_iVersion = M4_PDU_40_VERSION);

    //## Destructor (specified)
      //## Operation: ~ClNewCancelPDU%965046695
      ~ClNewCancelPDU ();


    //## Other Operations (specified)
      //## Operation: GetNextSizeBlock%965046696
      m4uint32_t GetNextSizeBlock (m4pchar_t &ao_pBlock);

      //## Operation: AddBlock%965046697
      m4return_t AddBlock (m4pchar_t &ai_pBlock);

      //## Operation: GetHeadSize%965046699
      m4uint32_t GetHeadSize ();

      //## Operation: Dump%965046702
      void Dump (m4pchar_t &ai_strBuffer);

      //## Operation: GetBinaryHead%965046703
      //	   Pasa todos los campos significativos de la PDU a una cadena de
      //	caracteres. El formato de los campos numéricos será binario.
      m4return_t GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize);

      //## Operation: SetCancelRequestId%965046704; C++
      void SetCancelRequestId (m4uint32_t ai_ulCancelRequestId);

      //## Operation: GetCancelRequestId%965046705; C++
      m4uint32_t GetCancelRequestId ();

    // Additional Public Declarations
      //## begin ClNewCancelPDU%39856E0002FC.public preserve=yes
      //## end ClNewCancelPDU%39856E0002FC.public

  protected:
    // Additional Protected Declarations
      //## begin ClNewCancelPDU%39856E0002FC.protected preserve=yes
      //## end ClNewCancelPDU%39856E0002FC.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_uiRequestID2Cancel%39CF312402E5
      //## begin ClNewCancelPDU::m_uiRequestID2Cancel%39CF312402E5.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_uiRequestID2Cancel;
      //## end ClNewCancelPDU::m_uiRequestID2Cancel%39CF312402E5.attr

    // Additional Private Declarations
      //## begin ClNewCancelPDU%39856E0002FC.private preserve=yes
      //## end ClNewCancelPDU%39856E0002FC.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClNewCancelPDU%39856E0002FC.implementation preserve=yes
      //## end ClNewCancelPDU%39856E0002FC.implementation

};

//## begin ClNewCancelPDU%39856E0002FC.postscript preserve=yes
//## end ClNewCancelPDU%39856E0002FC.postscript

//## begin ClNewErrorPDU%39856E9502ED.preface preserve=yes
//## end ClNewErrorPDU%39856E9502ED.preface

//## Class: ClNewErrorPDU%39856E9502ED
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4PDUS ClNewErrorPDU : public ClPDUWithData  //## Inherits: <unnamed>%39856EA00216
{
  //## begin ClNewErrorPDU%39856E9502ED.initialDeclarations preserve=yes
  //## end ClNewErrorPDU%39856E9502ED.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClNewErrorPDU%965046735; C++
      ClNewErrorPDU (m4int_t ai_iVersion = M4_PDU_40_VERSION);

    //## Destructor (specified)
      //## Operation: ~ClNewErrorPDU%965046736
      ~ClNewErrorPDU ();


    //## Other Operations (specified)
      //## Operation: GetNextSizeBlock%965046737
      m4uint32_t GetNextSizeBlock (m4pchar_t &ao_pBlock);

      //## Operation: AddBlock%965046738
      m4return_t AddBlock (m4pchar_t &ai_pBlock);

      //## Operation: GetHeadSize%965046740
      m4uint32_t GetHeadSize ();

      //## Operation: Dump%965046743
      void Dump (m4pchar_t &ai_strBuffer);

      //## Operation: GetErrorCode%965046744
      m4uint32_t GetErrorCode ();

      //## Operation: SetErrorCode%965046745
      void SetErrorCode (m4uint32_t ai_ulErrorCode);

      //## Operation: GetBinaryHead%965046746
      //	   Pasa todos los campos significativos de la PDU a una cadena de
      //	caracteres. El formato de los campos numéricos será binario.
      m4return_t GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize);

    // Additional Public Declarations
      //## begin ClNewErrorPDU%39856E9502ED.public preserve=yes
      //## end ClNewErrorPDU%39856E9502ED.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_ulErrorCode%39856ED000D5
      //## begin ClNewErrorPDU::m_ulErrorCode%39856ED000D5.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_ulErrorCode;
      //## end ClNewErrorPDU::m_ulErrorCode%39856ED000D5.attr

    // Additional Protected Declarations
      //## begin ClNewErrorPDU%39856E9502ED.protected preserve=yes
      //## end ClNewErrorPDU%39856E9502ED.protected

  private:
    // Additional Private Declarations
      //## begin ClNewErrorPDU%39856E9502ED.private preserve=yes
      //## end ClNewErrorPDU%39856E9502ED.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClNewErrorPDU%39856E9502ED.implementation preserve=yes
      //## end ClNewErrorPDU%39856E9502ED.implementation

};

//## begin ClNewErrorPDU%39856E9502ED.postscript preserve=yes
//## end ClNewErrorPDU%39856E9502ED.postscript

//## begin ClNewChangePasswordPDU%39856ED60079.preface preserve=yes
//## end ClNewChangePasswordPDU%39856ED60079.preface

//## Class: ClNewChangePasswordPDU%39856ED60079
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4PDUS ClNewChangePasswordPDU : public ClPDUWithData  //## Inherits: <unnamed>%39856EE30303
{
  //## begin ClNewChangePasswordPDU%39856ED60079.initialDeclarations preserve=yes
  //## end ClNewChangePasswordPDU%39856ED60079.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClNewChangePasswordPDU%965046706; C++
      ClNewChangePasswordPDU (m4int_t ai_iVersion = M4_PDU_40_VERSION);

    //## Destructor (specified)
      //## Operation: ~ClNewChangePasswordPDU%965046707
      ~ClNewChangePasswordPDU ();


    //## Other Operations (specified)
      //## Operation: GetNextSizeBlock%965046708
      m4uint32_t GetNextSizeBlock (m4pchar_t &ao_pBlock);

      //## Operation: AddBlock%965046709
      m4return_t AddBlock (m4pchar_t &ai_pBlock);

      //## Operation: GetHeadSize%965046711
      m4uint32_t GetHeadSize ();

      //## Operation: Dump%965046714
      void Dump (m4pchar_t  &ai_strBuffer);

      //## Operation: GetCurrentPassword%965046715
      m4uint16_t GetCurrentPassword (m4pchar_t &ao_pPass);

      //## Operation: SetCurrentPassword%965046716
      void SetCurrentPassword (m4pchar_t ai_pPass, m4uint16_t ai_uiPassSize);

      //## Operation: GetNewPassword%965046717
      m4uint16_t GetNewPassword (m4pchar_t &ao_pPassword);

      //## Operation: SetNewPassword%965046718
      void SetNewPassword (m4pchar_t ai_pPassword, m4uint16_t ai_uiPasswordLength);

      //## Operation: GetBinaryHead%965046719
      //	   Pasa todos los campos de la cabecera de la PDU a una cadena de
      //	caracteres.. El formato de los campos numéricos será binario.
      m4return_t GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize);

    // Additional Public Declarations
      //## begin ClNewChangePasswordPDU%39856ED60079.public preserve=yes
      //## end ClNewChangePasswordPDU%39856ED60079.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_oCurrentPassword%39856F070373
      //## begin ClNewChangePasswordPDU::m_oCurrentPassword%39856F070373.attr preserve=no  protected: ClPairIntpChar {UA} 
      ClPairIntpChar m_oCurrentPassword;
      //## end ClNewChangePasswordPDU::m_oCurrentPassword%39856F070373.attr

      //## Attribute: m_oNewPassword%39856F070374
      //## begin ClNewChangePasswordPDU::m_oNewPassword%39856F070374.attr preserve=no  protected: ClPairIntpChar {UA} 
      ClPairIntpChar m_oNewPassword;
      //## end ClNewChangePasswordPDU::m_oNewPassword%39856F070374.attr

    // Additional Protected Declarations
      //## begin ClNewChangePasswordPDU%39856ED60079.protected preserve=yes
      //## end ClNewChangePasswordPDU%39856ED60079.protected

  private:
    // Additional Private Declarations
      //## begin ClNewChangePasswordPDU%39856ED60079.private preserve=yes
      //## end ClNewChangePasswordPDU%39856ED60079.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClNewChangePasswordPDU%39856ED60079.implementation preserve=yes
      //## end ClNewChangePasswordPDU%39856ED60079.implementation

};

//## begin ClNewChangePasswordPDU%39856ED60079.postscript preserve=yes
//## end ClNewChangePasswordPDU%39856ED60079.postscript

//## begin ClNewDisconnectPDU%39856F730179.preface preserve=yes
//## end ClNewDisconnectPDU%39856F730179.preface

//## Class: ClNewDisconnectPDU%39856F730179
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4PDUS ClNewDisconnectPDU : public ClPDUWithData  //## Inherits: <unnamed>%39856FB003C6
{
  //## begin ClNewDisconnectPDU%39856F730179.initialDeclarations preserve=yes
  //## end ClNewDisconnectPDU%39856F730179.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClNewDisconnectPDU%965046720; C++
      ClNewDisconnectPDU (m4int_t ai_iVersion = M4_PDU_40_VERSION);

    //## Destructor (specified)
      //## Operation: ~ClNewDisconnectPDU%965046721
      ~ClNewDisconnectPDU ();


    //## Other Operations (specified)
      //## Operation: GetNextSizeBlock%965046722
      m4uint32_t GetNextSizeBlock (m4pchar_t &ao_pBlock);

      //## Operation: AddBlock%965046723
      m4return_t AddBlock (m4pchar_t  &ai_pBlock);

      //## Operation: GetHeadSize%965046725
      m4uint32_t GetHeadSize ();

      //## Operation: Dump%965046728
      void Dump (m4pchar_t  &ai_strBuffer);

      //## Operation: GetWhy%965046729
      m4uint16_t GetWhy ();

      //## Operation: SetWhy%965046730
      void SetWhy (m4uint16_t ai_uiWhy);

      //## Operation: SetUserId%965046731
      void SetUserId (m4handle_t ai_hUserId);

      //## Operation: GetUserId%965046732
      m4handle_t GetUserId ();

      //## Operation: GetBinaryHead%965046733
      //	   Pasa todos los campos significativos de la PDU a una cadena de
      //	caracteres. El formato de los campos numéricos será binario.
      m4return_t GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize);

    // Additional Public Declarations
      //## begin ClNewDisconnectPDU%39856F730179.public preserve=yes
      //## end ClNewDisconnectPDU%39856F730179.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_uiWhy%39856FAB0364
      //## begin ClNewDisconnectPDU::m_uiWhy%39856FAB0364.attr preserve=no  protected: m4uint16_t {UA} 
      m4uint16_t m_uiWhy;
      //## end ClNewDisconnectPDU::m_uiWhy%39856FAB0364.attr

      //## Attribute: m_hUserId%39856FAB0365
      //## begin ClNewDisconnectPDU::m_hUserId%39856FAB0365.attr preserve=no  protected: m4handle_t {UA} 0
      m4handle_t m_hUserId;
      //## end ClNewDisconnectPDU::m_hUserId%39856FAB0365.attr

    // Additional Protected Declarations
      //## begin ClNewDisconnectPDU%39856F730179.protected preserve=yes
      //## end ClNewDisconnectPDU%39856F730179.protected

  private:
    // Additional Private Declarations
      //## begin ClNewDisconnectPDU%39856F730179.private preserve=yes
      //## end ClNewDisconnectPDU%39856F730179.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClNewDisconnectPDU%39856F730179.implementation preserve=yes
      //## end ClNewDisconnectPDU%39856F730179.implementation

};

//## begin ClNewDisconnectPDU%39856F730179.postscript preserve=yes
//## end ClNewDisconnectPDU%39856F730179.postscript

//## begin ClNewSetRolePDU%39856FB90260.preface preserve=yes
//## end ClNewSetRolePDU%39856FB90260.preface

//## Class: ClNewSetRolePDU%39856FB90260
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4PDUS ClNewSetRolePDU : public ClPDUWithData  //## Inherits: <unnamed>%39856FC6013C
{
  //## begin ClNewSetRolePDU%39856FB90260.initialDeclarations preserve=yes
  //## end ClNewSetRolePDU%39856FB90260.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClNewSetRolePDU%965046748; C++
      ClNewSetRolePDU (m4int_t ai_iVersion = M4_PDU_40_VERSION);

    //## Destructor (specified)
      //## Operation: ~ClNewSetRolePDU%965046749
      ~ClNewSetRolePDU ();


    //## Other Operations (specified)
      //## Operation: GetNextSizeBlock%965046750
      m4uint32_t GetNextSizeBlock (m4pchar_t &ao_pBlock);

      //## Operation: AddBlock%965046751
      m4return_t AddBlock (m4pchar_t &ai_pBlock);

      //## Operation: GetHeadSize%965046753
      m4uint32_t GetHeadSize ();

      //## Operation: Dump%965046756
      void Dump (m4pchar_t &ai_strBuffer);

      //## Operation: GetBinaryHead%965046757
      //	   Pasa todos los campos significativos de la PDU a una cadena de
      //	caracteres. El formato de los campos numéricos será binario.
      m4return_t GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize);

      //## Operation: GetRole%965046758
      m4uint16_t GetRole (m4pchar_t &ao_pRoleName);

      //## Operation: SetRole%965046759
      void SetRole (m4pchar_t ai_pRoleName, m4uint16_t ai_uiRoleLength);

      //## Operation: SetDefaultRole%965046760
      void SetDefaultRole (m4bool_t ai_bDefaultRole);

      //## Operation: IsDefaultRole%965046761
      m4bool_t IsDefaultRole ();

    // Additional Public Declarations
      //## begin ClNewSetRolePDU%39856FB90260.public preserve=yes
      //## end ClNewSetRolePDU%39856FB90260.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_oRole%39856FEF03E4
      //## begin ClNewSetRolePDU::m_oRole%39856FEF03E4.attr preserve=no  protected: ClPairIntpChar {UA} 
      ClPairIntpChar m_oRole;
      //## end ClNewSetRolePDU::m_oRole%39856FEF03E4.attr

    // Additional Protected Declarations
      //## begin ClNewSetRolePDU%39856FB90260.protected preserve=yes
      //## end ClNewSetRolePDU%39856FB90260.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_bDefaultRole%39856FEF03E5
      //## begin ClNewSetRolePDU::m_bDefaultRole%39856FEF03E5.attr preserve=no  private: m4uint16_t {UA} M4_FALSE
      m4uint16_t m_bDefaultRole;
      //## end ClNewSetRolePDU::m_bDefaultRole%39856FEF03E5.attr

    // Additional Private Declarations
      //## begin ClNewSetRolePDU%39856FB90260.private preserve=yes
      //## end ClNewSetRolePDU%39856FB90260.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClNewSetRolePDU%39856FB90260.implementation preserve=yes
      //## end ClNewSetRolePDU%39856FB90260.implementation

};

//## begin ClNewSetRolePDU%39856FB90260.postscript preserve=yes
//## end ClNewSetRolePDU%39856FB90260.postscript

// Class ClControlPDU 

// Class ClErrorPDU 

// Class ClAckPDU 

// Class ClDisconnectPDU 

// Class ClAuthenticationPDU 

// Class ClConnectPDU 

// Class ClSetRolePDU 

// Class ClChangePasswordPDU 

// Class ClCancelPDU 

// Class ClNewCancelPDU 

// Class ClNewErrorPDU 

// Class ClNewChangePasswordPDU 

// Class ClNewDisconnectPDU 

// Class ClNewSetRolePDU 

//## begin module%3468A64C024F.epilog preserve=yes
//## end module%3468A64C024F.epilog



class M4_DECL_M4PDUS ClPingPDU : public ClPDUWithData
{
  public:
    //## Constructors (specified)
      //## Operation: ClPingPDU
      ClPingPDU (m4int_t ai_iVersion = M4_PDU_40_VERSION);

    //## Destructor (specified)
      //## Operation: ~ClPingPDU
      ~ClPingPDU ();


    //## Other Operations (specified)
      //## Operation: GetNextSizeBlock
      m4uint32_t GetNextSizeBlock (m4pchar_t &ao_pBlock);

      //## Operation: AddBlock
      m4return_t AddBlock (m4pchar_t &ai_pBlock);

      //## Operation: GetHeadSize
      m4uint32_t GetHeadSize ();

      //## Operation: Dump
      void Dump (m4pchar_t &ai_strBuffer);

      //## Operation: GetElapsedTime
      m4uint32_t GetElapsedTime ();

      //## Operation: SetTimeOut
      void SetElapsedTime (m4uint32_t ai_uiElapsedTime);

      //## Operation: GetBinaryHead
      //	   Pasa todos los campos significativos de la PDU a una cadena de
      //	caracteres. El formato de los campos numéricos será binario.
      m4return_t GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize);

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_uiElapsedTime
      m4uint32_t m_uiElapsedTime;

};

#endif

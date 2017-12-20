//## begin module%3468A69602A6.cm preserve=no
//## end module%3468A69602A6.cm

//## begin module%3468A69602A6.cp preserve=no
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
//## end module%3468A69602A6.cp

//## Module: pdutools%3468A69602A6; Package specification
//## Subsystem: M4PDU::inc%3794593C0398
//## Source file: e:\mybuild\m4pdus\inc\pdutools.hpp

#ifndef pdutools_h
#define pdutools_h 1

//## begin module%3468A69602A6.additionalIncludes preserve=no
//## end module%3468A69602A6.additionalIncludes

//## begin module%3468A69602A6.includes preserve=yes
#include "m4stl.hpp"
//## end module%3468A69602A6.includes

// DataStorages
#include <datastorages.hpp>
//## begin module%3468A69602A6.declarations preserve=no
//## end module%3468A69602A6.declarations

//## begin module%3468A69602A6.additionalDeclarations preserve=yes
#ifdef _KCC
using std::deque;
#endif
//## end module%3468A69602A6.additionalDeclarations


//## begin ClPairLongIntpChar%348E7C950238.preface preserve=yes
//## end ClPairLongIntpChar%348E7C950238.preface

//## Class: ClPairLongIntpChar%348E7C950238
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClPairLongIntpChar 
{
  //## begin ClPairLongIntpChar%348E7C950238.initialDeclarations preserve=yes
  //## end ClPairLongIntpChar%348E7C950238.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClPairLongIntpChar%881748108
      ClPairLongIntpChar (m4int32_t ai_lSize = 0, m4pchar_t ai_pChar = NULL);

    //## Destructor (specified)
      //## Operation: ~ClPairLongIntpChar%881748109
      ~ClPairLongIntpChar ();

    // Data Members for Class Attributes

      //## Attribute: m_lSize%348E7CAB0077
      //## begin ClPairLongIntpChar::m_lSize%348E7CAB0077.attr preserve=no  public: m4int32_t {UA} 0
      m4int32_t m_lSize;
      //## end ClPairLongIntpChar::m_lSize%348E7CAB0077.attr

      //## Attribute: m_pChar%348E7CBE02B0
      //## begin ClPairLongIntpChar::m_pChar%348E7CBE02B0.attr preserve=no  public: m4pchar_t {UA} NULL
      m4pchar_t m_pChar;
      //## end ClPairLongIntpChar::m_pChar%348E7CBE02B0.attr

    // Additional Public Declarations
      //## begin ClPairLongIntpChar%348E7C950238.public preserve=yes
      //## end ClPairLongIntpChar%348E7C950238.public

  protected:
    // Additional Protected Declarations
      //## begin ClPairLongIntpChar%348E7C950238.protected preserve=yes
      //## end ClPairLongIntpChar%348E7C950238.protected

  private:
    // Additional Private Declarations
      //## begin ClPairLongIntpChar%348E7C950238.private preserve=yes
      //## end ClPairLongIntpChar%348E7C950238.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClPairLongIntpChar%348E7C950238.implementation preserve=yes
      //## end ClPairLongIntpChar%348E7C950238.implementation

};

//## begin ClPairLongIntpChar%348E7C950238.postscript preserve=yes
//## end ClPairLongIntpChar%348E7C950238.postscript

//## begin ClBlockFormat%345CC84F01B5.preface preserve=yes
//## end ClBlockFormat%345CC84F01B5.preface

//## Class: ClBlockFormat%345CC84F01B5
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClBlockFormat 
{
  //## begin ClBlockFormat%345CC84F01B5.initialDeclarations preserve=yes
  //## end ClBlockFormat%345CC84F01B5.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClBlockFormat%878806464
      ClBlockFormat (m4uint32_t ai_ulSize, m4bool_t ai_bEncrypted);

      //## Operation: ClBlockFormat%878893920
      ClBlockFormat (m4uint32_t ai_ulSizef);

    //## Destructor (specified)
      //## Operation: ~ClBlockFormat%878806465
      ~ClBlockFormat ();

    // Data Members for Class Attributes

      //## Attribute: m_ulSize%345CC870016D
      //## begin ClBlockFormat::m_ulSize%345CC870016D.attr preserve=no  public: m4uint32_t {UA} 
      m4uint32_t m_ulSize;
      //## end ClBlockFormat::m_ulSize%345CC870016D.attr

      //## Attribute: m_bEncrypted%345CC87B030D
      //## begin ClBlockFormat::m_bEncrypted%345CC87B030D.attr preserve=no  public: m4bool_t {UA} M4_FALSE
      m4bool_t m_bEncrypted;
      //## end ClBlockFormat::m_bEncrypted%345CC87B030D.attr

    // Additional Public Declarations
      //## begin ClBlockFormat%345CC84F01B5.public preserve=yes
      //## end ClBlockFormat%345CC84F01B5.public

  protected:
    // Additional Protected Declarations
      //## begin ClBlockFormat%345CC84F01B5.protected preserve=yes
      //## end ClBlockFormat%345CC84F01B5.protected

  private:
    // Additional Private Declarations
      //## begin ClBlockFormat%345CC84F01B5.private preserve=yes
      //## end ClBlockFormat%345CC84F01B5.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClBlockFormat%345CC84F01B5.implementation preserve=yes
      //## end ClBlockFormat%345CC84F01B5.implementation

};

//## begin ClBlockFormat%345CC84F01B5.postscript preserve=yes
//## end ClBlockFormat%345CC84F01B5.postscript

//## begin ClBlockFormats%345CC9020018.preface preserve=yes
//## end ClBlockFormats%345CC9020018.preface

//## Class: ClBlockFormats%345CC9020018
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%345CC9B20116;ClBlockFormat { -> }
//## Uses: <unnamed>%3468B253027F;deque { -> }

class ClBlockFormats 
{
  //## begin ClBlockFormats%345CC9020018.initialDeclarations preserve=yes
  //## end ClBlockFormats%345CC9020018.initialDeclarations

  public:
    //## begin ClBlockFormats::CLFORMATQUEUE%345DABD603B2.preface preserve=yes
    //## end ClBlockFormats::CLFORMATQUEUE%345DABD603B2.preface

    //## Class: CLFORMATQUEUE%345DABD603B2
    //## Category: M4PDU%34BA420E0375
    //## Subsystem: M4PDU::inc%3794593C0398
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef deque < ClBlockFormat * > CLFORMATQUEUE;

    //## begin ClBlockFormats::CLFORMATQUEUE%345DABD603B2.postscript preserve=yes
    //## end ClBlockFormats::CLFORMATQUEUE%345DABD603B2.postscript

    //## Constructors (specified)
      //## Operation: ClBlockFormats%878806466
      ClBlockFormats ();

    //## Destructor (specified)
      //## Operation: ~ClBlockFormats%878806467
      ~ClBlockFormats ();


    //## Other Operations (specified)
      //## Operation: AddFormat%878723092
      void AddFormat (m4uint32_t ai_ulSizef);

      //## Operation: IsEmpty%878806474
      m4bool_t IsEmpty ();

      //## Operation: Clear%878848732
      void Clear ();

      //## Operation: GetNext%878893911
      m4return_t GetNext (m4uint32_t &ao_ulSizef);

      //## Operation: GetNext%878893912
      m4return_t GetNext (m4uint32_t &ao_ulSize, m4bool_t &ao_bEncrypted);

      //## Operation: InitializeIteration%878893913
      void InitializeIteration ();

      //## Operation: GetTop%878893916
      m4return_t GetTop (m4uint32_t &ao_ulSizef);

      //## Operation: GetTop%878893914
      m4return_t GetTop (m4uint32_t &ao_ulSize, m4bool_t &ao_bEncrypted);

      //## Operation: QuitTop%878893915
      m4return_t QuitTop ();

      //## Operation: AddFormat%878893919
      void AddFormat (m4uint32_t ai_ulSize, m4bool_t ai_bEncrypted);

      //## Operation: GetNextNInc%879243180
      m4return_t GetNextNInc (m4uint32_t &ao_ulSize, m4bool_t &ao_bEncrypted);

      //## Operation: GetNextNInc%879243181
      m4return_t GetNextNInc (m4uint32_t &ao_ulSizef);

      //## Operation: GetFormatsSize%879502156
      m4uint32_t GetFormatsSize ();

    // Data Members for Associations

      //## Association: M4PDU::<unnamed>%34F1477B012E
      //## Role: ClBlockFormats::m_oFormatQueue%34F1477C0049
      //## begin ClBlockFormats::m_oFormatQueue%34F1477C0049.role preserve=no  public: ClFormatQueue { -> 1VHAN}
      ClFormatQueue m_oFormatQueue;
      //## end ClBlockFormats::m_oFormatQueue%34F1477C0049.role

  public:
    // Additional Public Declarations
      //## begin ClBlockFormats%345CC9020018.public preserve=yes
      //## end ClBlockFormats%345CC9020018.public

  protected:
    // Additional Protected Declarations
      //## begin ClBlockFormats%345CC9020018.protected preserve=yes
      //## end ClBlockFormats%345CC9020018.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_oIterator%3462F1EB0346
      //## begin ClBlockFormats::m_oIterator%3462F1EB0346.attr preserve=no  private: ClFormatQueue::iterator {UA} 
      ClFormatQueue::iterator m_oIterator;
      //## end ClBlockFormats::m_oIterator%3462F1EB0346.attr

    // Additional Private Declarations
      //## begin ClBlockFormats%345CC9020018.private preserve=yes
      //## end ClBlockFormats%345CC9020018.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClBlockFormats%345CC9020018.implementation preserve=yes
      //## end ClBlockFormats%345CC9020018.implementation

};

//## begin ClBlockFormats%345CC9020018.postscript preserve=yes
//## end ClBlockFormats%345CC9020018.postscript

//## begin ClPairIntpChar%34323AC7000C.preface preserve=yes
//## end ClPairIntpChar%34323AC7000C.preface

//## Class: ClPairIntpChar%34323AC7000C
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClPairIntpChar 
{
  //## begin ClPairIntpChar%34323AC7000C.initialDeclarations preserve=yes
  //## end ClPairIntpChar%34323AC7000C.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClPairIntpChar%875697578
      ClPairIntpChar (m4int16_t ai_iSize = 0, m4pchar_t ai_pChar = NULL);

    //## Destructor (specified)
      //## Operation: ~ClPairIntpChar%880712904
      ~ClPairIntpChar ();

    // Data Members for Class Attributes

      //## Attribute: m_iSize%34323AEB0144
      //## begin ClPairIntpChar::m_iSize%34323AEB0144.attr preserve=no  public: m4int16_t {UA} 0
      m4int16_t m_iSize;
      //## end ClPairIntpChar::m_iSize%34323AEB0144.attr

      //## Attribute: m_pChar%34323B0F0377
      //## begin ClPairIntpChar::m_pChar%34323B0F0377.attr preserve=no  public: m4pchar_t {UA} NULL
      m4pchar_t m_pChar;
      //## end ClPairIntpChar::m_pChar%34323B0F0377.attr

    // Additional Public Declarations
      //## begin ClPairIntpChar%34323AC7000C.public preserve=yes
      //## end ClPairIntpChar%34323AC7000C.public

  protected:
    // Additional Protected Declarations
      //## begin ClPairIntpChar%34323AC7000C.protected preserve=yes
      //## end ClPairIntpChar%34323AC7000C.protected

  private:
    // Additional Private Declarations
      //## begin ClPairIntpChar%34323AC7000C.private preserve=yes
      //## end ClPairIntpChar%34323AC7000C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClPairIntpChar%34323AC7000C.implementation preserve=yes
      //## end ClPairIntpChar%34323AC7000C.implementation

};

//## begin ClPairIntpChar%34323AC7000C.postscript preserve=yes
//## end ClPairIntpChar%34323AC7000C.postscript

// Class ClPairLongIntpChar 

// Class ClBlockFormat 

// Class ClBlockFormats 

// Class ClPairIntpChar 

//## begin module%3468A69602A6.epilog preserve=yes
//## end module%3468A69602A6.epilog


#endif

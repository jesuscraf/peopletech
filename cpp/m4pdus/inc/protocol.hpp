//## begin module%344219BD0314.cm preserve=no
//## end module%344219BD0314.cm

//## begin module%344219BD0314.cp preserve=no
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
//## end module%344219BD0314.cp

//## Module: Protocol%344219BD0314; Package specification
//## Subsystem: M4PDU::inc%3794593C0398
//## Source file: e:\mybuild\m4pdus\inc\protocol.hpp

#ifndef protocol_h
#define protocol_h 1

//## begin module%344219BD0314.additionalIncludes preserve=no
//## end module%344219BD0314.additionalIncludes

//## begin module%344219BD0314.includes preserve=yes
//## end module%344219BD0314.includes

// m4types
#include "m4types.hpp"
#include "m4pdus_dll.hpp"

class ClPDU;

//## begin module%344219BD0314.declarations preserve=no
//## end module%344219BD0314.declarations

//## begin module%344219BD0314.additionalDeclarations preserve=yes
//## end module%344219BD0314.additionalDeclarations


//## Class: ClProtocol%342B8E670041
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3454D4A90353;m4pchar_t { -> }
//## Uses: <unnamed>%3960579E01BA;ClAdvNetHost { -> F}
//## Uses: <unnamed>%396058F40270;ClPDUWithData { -> F}
//## Uses: <unnamed>%396058F6034F;ClControlPDU { -> F}

class M4_DECL_M4PDUS ClProtocol 
{
  public:
    //## Constructors (specified)
      //## Operation: ClProtocol%875692547
      ClProtocol ();


    //## Other Operations (specified)
      //## Operation: GetPDU%875544566
      ClPDU * GetPDU (m4pchar_t ai_pBlock);

      //## Operation: GetHeadSize%875692546
      m4int16_t GetHeadSize ();

      //## Operation: GetPDU%877013199; C++
      ClPDU * GetPDU (m4uint16_t ai_uiPDUId, m4bool_t ai_bExact = M4_FALSE);

      //## Operation: SetVersion%965046762; C++
      m4return_t SetVersion (m4int_t ai_iVersion);

      //## Operation: GetVersion%965046763; C++
      m4uint_t GetVersion ();

    // Data Members for Associations

      //## Association: M4PDU::<unnamed>%3960572603C0
      //## Role: ClProtocol::m_pPDU%396057270214
      //## begin ClProtocol::m_pPDU%396057270214.role preserve=no  public: ClPDU { -> RFHAN}
      ClPDU *m_pPDU;
      //## end ClProtocol::m_pPDU%396057270214.role

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_iVersion%39856C890253
      //## begin ClProtocol::m_iVersion%39856C890253.attr preserve=no  protected: m4int_t {UA} M4_PDU_40_VERSION
      m4int_t m_iVersion;
      //## end ClProtocol::m_iVersion%39856C890253.attr

  private:
  private: //## implementation
};

// Class ClProtocol 

//## begin module%344219BD0314.epilog preserve=yes
//## end module%344219BD0314.epilog


#endif

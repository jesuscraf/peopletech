//## begin module%40A9D19A0158.cm preserve=no
//## end module%40A9D19A0158.cm

//## begin module%40A9D19A0158.cp preserve=no
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
//## end module%40A9D19A0158.cp

//## Module: m4dspapi4snmp%40A9D19A0158; Package specification
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Source file: z:\m4dispatcher\inc\m4dspapi4snmp.hpp

#ifndef m4dspapi4snmp_h
#define m4dspapi4snmp_h 1

//## begin module%40A9D19A0158.additionalIncludes preserve=no
//## end module%40A9D19A0158.additionalIncludes

//## begin module%40A9D19A0158.includes preserve=yes
//## end module%40A9D19A0158.includes

// snmpinterfaces
#include <snmpinterfaces.hpp>
// SiteNodeInfo
#include <sitenodeinfo.hpp>
// m4dispatchermain
#include <m4dispatchermain.hpp>
// ParamList
#include <paramlist.hpp>
//## begin module%40A9D19A0158.declarations preserve=no
//## end module%40A9D19A0158.declarations

//## begin module%40A9D19A0158.additionalDeclarations preserve=yes
//## end module%40A9D19A0158.additionalDeclarations


//## begin M4DspApi4Snmp%40A9CFA10139.preface preserve=yes
//## end M4DspApi4Snmp%40A9CFA10139.preface

//## Class: M4DspApi4Snmp%40A9CFA10139
//## Category: M4Dispatcher%3923B57F0208
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%40A9D6ED032D;ClSiteNodeRepository { -> }
//## Uses: <unnamed>%40A9D94D02A8;ClSiteNodeInformation { -> }
//## Uses: <unnamed>%40AC6A7101BE;ClDpchMainSingleton { -> }
//## Uses: <unnamed>%40AC6A73024A;ClDpchMain { -> }
//## Uses: <unnamed>%40AC6AEC01CD;ClParamList { -> }

class M4DspApi4Snmp : public M4ISSApi4Snmp  //## Inherits: <unnamed>%40A9D14F032D
{
  //## begin M4DspApi4Snmp%40A9CFA10139.initialDeclarations preserve=yes
  //## end M4DspApi4Snmp%40A9CFA10139.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4DspApi4Snmp%1084868909
      M4DspApi4Snmp (const ClSiteNodeInformation& ai_node, ClSiteNodeRepository* ai_poRepository);


    //## Other Operations (specified)
      //## Operation: ExecuteCommand%1084868906
      m4return_t ExecuteCommand (m4pcchar_t ai_commandID, m4pcchar_t ai_paramID, m4pcchar_t ai_paramValue) const;

      //## Operation: GetPropertyValue%1084868907
      m4return_t GetPropertyValue (const m4string_t& ai_name, m4int32_t ai_index, m4string_t& ao_value) const;

      //## Operation: GetStatisticValue%1084868908
      m4return_t GetStatisticValue (const m4string_t& ai_name, m4int32_t ai_index, m4string_t& ao_value) const;

    // Additional Public Declarations
      //## begin M4DspApi4Snmp%40A9CFA10139.public preserve=yes
      //## end M4DspApi4Snmp%40A9CFA10139.public

  protected:

    //## Other Operations (specified)
      //## Operation: StopNode%1085040984
      m4return_t StopNode () const;

      //## Operation: StartNode%1085478477
      m4return_t StartNode () const;

      //## Operation: SetNodeState%1085478478
      m4return_t SetNodeState (m4int32_t ai_state) const;

    // Additional Protected Declarations
      //## begin M4DspApi4Snmp%40A9CFA10139.protected preserve=yes
      //## end M4DspApi4Snmp%40A9CFA10139.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_node%40A9D5C400DB
      //## begin M4DspApi4Snmp::m_node%40A9D5C400DB.attr preserve=no  private: ClSiteNodeInformation {UA} 
      ClSiteNodeInformation m_node;
      //## end M4DspApi4Snmp::m_node%40A9D5C400DB.attr

      //## Attribute: m_repository%40A9D5CF02EE
      //## begin M4DspApi4Snmp::m_repository%40A9D5CF02EE.attr preserve=no  private: ClSiteNodeRepository* {UA} 
      ClSiteNodeRepository* m_repository;
      //## end M4DspApi4Snmp::m_repository%40A9D5CF02EE.attr

    // Additional Private Declarations
      //## begin M4DspApi4Snmp%40A9CFA10139.private preserve=yes
      //## end M4DspApi4Snmp%40A9CFA10139.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4DspApi4Snmp%40A9CFA10139.implementation preserve=yes
      //## end M4DspApi4Snmp%40A9CFA10139.implementation

};

//## begin M4DspApi4Snmp%40A9CFA10139.postscript preserve=yes
//## end M4DspApi4Snmp%40A9CFA10139.postscript

// Class M4DspApi4Snmp 

//## begin module%40A9D19A0158.epilog preserve=yes
//## end module%40A9D19A0158.epilog


#endif

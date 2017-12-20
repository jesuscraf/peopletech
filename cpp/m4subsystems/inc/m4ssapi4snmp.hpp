//## begin module%4084F8670055.cm preserve=no
//## end module%4084F8670055.cm

//## begin module%4084F8670055.cp preserve=no
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
//## end module%4084F8670055.cp

//## Module: m4ssapi4snmp%4084F8670055; Package specification
//## Subsystem: M4Subsystems::inc%37D3A21301DC
//## Source file: z:\m4subsystems\inc\m4ssapi4snmp.hpp

#ifndef m4ssapi4snmp_h
#define m4ssapi4snmp_h 1

//## begin module%4084F8670055.additionalIncludes preserve=no
//## end module%4084F8670055.additionalIncludes

//## begin module%4084F8670055.includes preserve=yes
//## end module%4084F8670055.includes

// snmpinterfaces
#include <snmpinterfaces.hpp>
// clssinterfaz
#include <clssinterfaz.hpp>
//## begin module%4084F8670055.declarations preserve=no
//## end module%4084F8670055.declarations

//## begin module%4084F8670055.additionalDeclarations preserve=yes
//## end module%4084F8670055.additionalDeclarations


//## begin M4SSApi4Snmp%4084FBE20183.preface preserve=yes
//## end M4SSApi4Snmp%4084FBE20183.preface

//## Class: M4SSApi4Snmp%4084FBE20183
//## Category: M4Subsystems%37D39FC60059
//## Subsystem: M4Subsystems::inc%37D3A21301DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4SSApi4Snmp : public M4ISSApi4Snmp  //## Inherits: <unnamed>%40A9C26E008D
{
  //## begin M4SSApi4Snmp%4084FBE20183.initialDeclarations preserve=yes
  //## end M4SSApi4Snmp%4084FBE20183.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4SSApi4Snmp%1082361896
      M4SSApi4Snmp (ClSSInterfaz* ai_poSSInterfaz);


    //## Other Operations (specified)
      //## Operation: ExecuteCommand%1082361895
      m4return_t ExecuteCommand (m4pcchar_t ai_commandID, m4pcchar_t ai_paramID, m4pcchar_t ai_paramValue) const;

      //## Operation: GetPropertyValue%1082361899
      m4return_t GetPropertyValue (const m4string_t& ai_name, m4int32_t ai_index, m4string_t& ao_value) const;

      //## Operation: GetStatisticValue%1083310100
      m4return_t GetStatisticValue (const m4string_t& ai_name, m4int32_t ai_index, m4string_t& ao_value) const;

    // Additional Public Declarations
      //## begin M4SSApi4Snmp%4084FBE20183.public preserve=yes
      //## end M4SSApi4Snmp%4084FBE20183.public

  protected:
    // Additional Protected Declarations
      //## begin M4SSApi4Snmp%4084FBE20183.protected preserve=yes
      //## end M4SSApi4Snmp%4084FBE20183.protected

  private:
    // Data Members for Associations

      //## Association: M4Subsystems::<unnamed>%4084FE180082
      //## Role: M4SSApi4Snmp::<m_pClSSInterfaz>%4084FE18028C
      //## begin M4SSApi4Snmp::<m_pClSSInterfaz>%4084FE18028C.role preserve=no  private: ClSSInterfaz { -> RHAN}
      ClSSInterfaz *m_pClSSInterfaz;
      //## end M4SSApi4Snmp::<m_pClSSInterfaz>%4084FE18028C.role

    // Additional Private Declarations
      //## begin M4SSApi4Snmp%4084FBE20183.private preserve=yes
      //## end M4SSApi4Snmp%4084FBE20183.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4SSApi4Snmp%4084FBE20183.implementation preserve=yes
      //## end M4SSApi4Snmp%4084FBE20183.implementation

};

//## begin M4SSApi4Snmp%4084FBE20183.postscript preserve=yes
//## end M4SSApi4Snmp%4084FBE20183.postscript

// Class M4SSApi4Snmp 

//## begin module%4084F8670055.epilog preserve=yes
//## end module%4084F8670055.epilog


#endif

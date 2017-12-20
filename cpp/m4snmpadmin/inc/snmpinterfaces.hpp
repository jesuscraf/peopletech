//## begin module%406BD74E001A.cm preserve=no
//## end module%406BD74E001A.cm

//## begin module%406BD74E001A.cp preserve=no
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
//## end module%406BD74E001A.cp

//## Module: snmpinterfaces%406BD74E001A; Package specification
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Source file: z:\m4snmpadmin\inc\snmpinterfaces.hpp

#ifndef snmpinterfaces_h
#define snmpinterfaces_h 1

//## begin module%406BD74E001A.additionalIncludes preserve=no
//## end module%406BD74E001A.additionalIncludes

//## begin module%406BD74E001A.includes preserve=yes
#include "m4snmpadmin_dll.hpp"
//## end module%406BD74E001A.includes

// m4string
#include <m4string.hpp>
// m4types
#include <m4types.hpp>
// ClConfiguration
#include <clconfiguration.hpp>
//## begin module%406BD74E001A.declarations preserve=no
//## end module%406BD74E001A.declarations

//## begin module%406BD74E001A.additionalDeclarations preserve=yes
// Xerces
#include <xercesc/util/XercesDefs.hpp>

XERCES_CPP_NAMESPACE_USE

XERCES_CPP_NAMESPACE_BEGIN
class DOMNode;
XERCES_CPP_NAMESPACE_END
//## end module%406BD74E001A.additionalDeclarations


//## begin M4SnmpAdministratorType%406C1F4800AB.preface preserve=yes
//## end M4SnmpAdministratorType%406C1F4800AB.preface

//## Class: M4SnmpAdministratorType%406C1F4800AB
//## Category: M4SnmpAdmin%37EA673500B9
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef enum {M4_SNMP_STANDALONE_ADMIN=0, M4_SNMP_MASTER_ADMIN, M4_SNMP_SLAVE_ADMIN} M4SnmpAdministratorType;

//## begin M4SnmpAdministratorType%406C1F4800AB.postscript preserve=yes
//## end M4SnmpAdministratorType%406C1F4800AB.postscript

//## begin M4MibType%408D1F90033C.preface preserve=yes
//## end M4MibType%408D1F90033C.preface

//## Class: M4MibType%408D1F90033C
//## Category: M4SnmpAdmin%37EA673500B9
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef enum {M4_MIB_LEAF=0, M4_MIB_STATIC_TABLE_ENTRY, M4_MIB_DYNAMIC_TABLE_ENTRY} M4MibType;

//## begin M4MibType%408D1F90033C.postscript preserve=yes
//## end M4MibType%408D1F90033C.postscript

//## begin M4SnmpObjectStatus%408E81010190.preface preserve=yes
//## end M4SnmpObjectStatus%408E81010190.preface

//## Class: M4SnmpObjectStatus%408E81010190
//## Category: M4SnmpAdmin%37EA673500B9
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef enum {CREATED=0, CREATING_TABLE} M4SnmpObjectStatus;

//## begin M4SnmpObjectStatus%408E81010190.postscript preserve=yes
//## end M4SnmpObjectStatus%408E81010190.postscript

//## begin M4ISSApi4Snmp%40A8CE5702C7.preface preserve=yes
//## end M4ISSApi4Snmp%40A8CE5702C7.preface

//## Class: M4ISSApi4Snmp%40A8CE5702C7
//## Category: M4SnmpAdmin%37EA673500B9
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%40A9C517004F;m4string_t { -> }

class M4ISSApi4Snmp 
{
  //## begin M4ISSApi4Snmp%40A8CE5702C7.initialDeclarations preserve=yes
  //## end M4ISSApi4Snmp%40A8CE5702C7.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: ExecuteCommand%1084864429
      virtual m4return_t ExecuteCommand (m4pcchar_t ai_commandID, m4pcchar_t ai_paramID, m4pcchar_t ai_paramValue) const = 0;

      //## Operation: GetPropertyValue%1084864430
      virtual m4return_t GetPropertyValue (const m4string_t& ai_name, m4int32_t ai_index, m4string_t& ao_value) const = 0;

      //## Operation: GetStatisticValue%1084864431
      virtual m4return_t GetStatisticValue (const m4string_t& ai_name, m4int32_t ai_index, m4string_t& ao_value) const = 0;

    // Additional Public Declarations
      //## begin M4ISSApi4Snmp%40A8CE5702C7.public preserve=yes
      //## end M4ISSApi4Snmp%40A8CE5702C7.public

  protected:
    // Additional Protected Declarations
      //## begin M4ISSApi4Snmp%40A8CE5702C7.protected preserve=yes
      //## end M4ISSApi4Snmp%40A8CE5702C7.protected

  private:
    // Additional Private Declarations
      //## begin M4ISSApi4Snmp%40A8CE5702C7.private preserve=yes
      //## end M4ISSApi4Snmp%40A8CE5702C7.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4ISSApi4Snmp%40A8CE5702C7.implementation preserve=yes
      //## end M4ISSApi4Snmp%40A8CE5702C7.implementation

};

//## begin M4ISSApi4Snmp%40A8CE5702C7.postscript preserve=yes
//## end M4ISSApi4Snmp%40A8CE5702C7.postscript

//## begin M4ISnmpObject%407BA7170020.preface preserve=yes
//## end M4ISnmpObject%407BA7170020.preface

//## Class: M4ISnmpObject%407BA7170020
//## Category: M4SnmpAdmin%37EA673500B9
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%4085034D0057; { -> }
//## Uses: <unnamed>%408D22000249;M4MibType { -> }
//## Uses: <unnamed>%408E83CB0268;M4SnmpObjectStatus { -> }
//## Uses: <unnamed>%40A8D684020C;M4ISSApi4Snmp { -> }

class M4ISnmpObject 
{
  //## begin M4ISnmpObject%407BA7170020.initialDeclarations preserve=yes
  //## end M4ISnmpObject%407BA7170020.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GetClassId%1081862810
      virtual m4string_t GetClassId () = 0;

      //## Operation: GetInstanceId%1081862811
      virtual m4string_t GetInstanceId () = 0;

      //## Operation: GetOID%1081862812
      virtual m4string_t GetOID () = 0;

      //## Operation: GetStatus%1082986506
      virtual M4SnmpObjectStatus GetStatus () = 0;

      //## Operation: SetStatus%1082986509
      virtual void SetStatus (M4SnmpObjectStatus ai_status) = 0;

    // Additional Public Declarations
      //## begin M4ISnmpObject%407BA7170020.public preserve=yes
      //## end M4ISnmpObject%407BA7170020.public

  protected:
    // Additional Protected Declarations
      //## begin M4ISnmpObject%407BA7170020.protected preserve=yes
      //## end M4ISnmpObject%407BA7170020.protected

  private:
    // Additional Private Declarations
      //## begin M4ISnmpObject%407BA7170020.private preserve=yes
      //## end M4ISnmpObject%407BA7170020.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4ISnmpObject%407BA7170020.implementation preserve=yes
      //## end M4ISnmpObject%407BA7170020.implementation

};

//## begin M4ISnmpObject%407BA7170020.postscript preserve=yes
//## end M4ISnmpObject%407BA7170020.postscript

//## begin M4ISnmpAdministrator%406BD69F02C1.preface preserve=yes
//## end M4ISnmpAdministrator%406BD69F02C1.preface

//## Class: M4ISnmpAdministrator%406BD69F02C1
//## Category: M4SnmpAdmin%37EA673500B9
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%406BFD8E02EB;m4return_t { -> }
//## Uses: <unnamed>%407416F2039B;ClConfiguration { -> }
//## Uses: <unnamed>%407CF85600A6; { -> }
//## Uses: <unnamed>%40A9BB070214;M4ISnmpObject { -> }

class M4_DECL_M4SNMPADMIN M4ISnmpAdministrator 
{
  //## begin M4ISnmpAdministrator%406BD69F02C1.initialDeclarations preserve=yes
  //## end M4ISnmpAdministrator%406BD69F02C1.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: RegisterSubsystem%1080808683
      virtual M4ISnmpObject* RegisterSubsystem (M4ISnmpObject* ai_pParentSnmpObject, M4ISSApi4Snmp* ai_pSSApi, m4pcchar_t ai_type, m4pcchar_t ai_name) = 0;

      //## Operation: RegisterProperty%1080808684
      virtual m4return_t RegisterProperty (m4pcchar_t ai_name, m4int32_t ai_propertyIndex, M4ISnmpObject* ai_poParentSnmpObject, ClConfiguration* ai_poConfig) = 0;

      //## Operation: Init%1081331510
      virtual m4return_t Init (ClConfiguration* ai_config) = 0;

      //## Operation: Destroy%1081341185
      virtual m4return_t Destroy () = 0;

      //## Operation: Start%1081862819
      virtual m4return_t Start () = 0;

      //## Operation: Stop%1081862820
      virtual m4return_t Stop () = 0;

      //## Operation: RegisterStatistic%1083310096
      virtual m4return_t RegisterStatistic (m4pcchar_t ai_name, m4int32_t ai_statisticIndex, M4ISnmpObject* ai_poParentSnmpObject, ClConfiguration* ai_poConfig) = 0;

      //## Operation: UnregisterSubsystem%1083666426
      virtual m4return_t UnregisterSubsystem (M4ISnmpObject* ai_poSnmpObject) = 0;

      //## Operation: Init%1084791065
      virtual m4return_t Init (const DOMNode* ai_config) = 0;

      //## Operation: RegisterUser%1084894579
      virtual m4return_t RegisterUser (m4pcchar_t ai_name, m4pcchar_t ai_password) = 0;

    // Additional Public Declarations
      //## begin M4ISnmpAdministrator%406BD69F02C1.public preserve=yes
      //## end M4ISnmpAdministrator%406BD69F02C1.public

  protected:
    // Additional Protected Declarations
      //## begin M4ISnmpAdministrator%406BD69F02C1.protected preserve=yes
      //## end M4ISnmpAdministrator%406BD69F02C1.protected

  private:
    // Additional Private Declarations
      //## begin M4ISnmpAdministrator%406BD69F02C1.private preserve=yes
      //## end M4ISnmpAdministrator%406BD69F02C1.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4ISnmpAdministrator%406BD69F02C1.implementation preserve=yes
      //## end M4ISnmpAdministrator%406BD69F02C1.implementation

};

//## begin M4ISnmpAdministrator%406BD69F02C1.postscript preserve=yes
//## end M4ISnmpAdministrator%406BD69F02C1.postscript

//## begin M4ISnmpAdministratorFactory%40729ED502D2.preface preserve=yes
//## end M4ISnmpAdministratorFactory%40729ED502D2.preface

//## Class: M4ISnmpAdministratorFactory%40729ED502D2
//## Category: M4SnmpAdmin%37EA673500B9
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%40729FCD01AC;M4ISnmpAdministrator { -> }

class M4_DECL_M4SNMPADMIN M4ISnmpAdministratorFactory 
{
  //## begin M4ISnmpAdministratorFactory%40729ED502D2.initialDeclarations preserve=yes
  //## end M4ISnmpAdministratorFactory%40729ED502D2.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: getStandAloneAdmin%1081250848
      virtual M4ISnmpAdministrator* getStandAloneAdmin () = 0;

      //## Operation: getMasterAdmin%1081250849
      virtual M4ISnmpAdministrator* getMasterAdmin () = 0;

      //## Operation: getSlaveAdmin%1081250850
      virtual M4ISnmpAdministrator* getSlaveAdmin () = 0;

    // Additional Public Declarations
      //## begin M4ISnmpAdministratorFactory%40729ED502D2.public preserve=yes
      //## end M4ISnmpAdministratorFactory%40729ED502D2.public

  protected:
    // Additional Protected Declarations
      //## begin M4ISnmpAdministratorFactory%40729ED502D2.protected preserve=yes
      //## end M4ISnmpAdministratorFactory%40729ED502D2.protected

  private:
    // Additional Private Declarations
      //## begin M4ISnmpAdministratorFactory%40729ED502D2.private preserve=yes
      //## end M4ISnmpAdministratorFactory%40729ED502D2.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4ISnmpAdministratorFactory%40729ED502D2.implementation preserve=yes
      //## end M4ISnmpAdministratorFactory%40729ED502D2.implementation

};

//## begin M4ISnmpAdministratorFactory%40729ED502D2.postscript preserve=yes
//## end M4ISnmpAdministratorFactory%40729ED502D2.postscript

// Class M4ISSApi4Snmp 

// Class M4ISnmpObject 

// Class M4ISnmpAdministrator 

// Class M4ISnmpAdministratorFactory 

//## begin module%406BD74E001A.epilog preserve=yes
//## end module%406BD74E001A.epilog


#endif

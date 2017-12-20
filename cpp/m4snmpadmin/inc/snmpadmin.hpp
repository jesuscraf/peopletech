//## begin module%406BE37C02A8.cm preserve=no
//## end module%406BE37C02A8.cm

//## begin module%406BE37C02A8.cp preserve=no
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
//## end module%406BE37C02A8.cp

//## Module: snmpadmin%406BE37C02A8; Package specification
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Source file: z:\m4snmpadmin\inc\snmpadmin.hpp

#ifndef snmpadmin_h
#define snmpadmin_h 1


//## begin module%406BE37C02A8.additionalIncludes preserve=no
//## end module%406BE37C02A8.additionalIncludes

//## begin module%406BE37C02A8.includes preserve=yes
#define SNMP_TREE_ROOT ""

//## end module%406BE37C02A8.includes

#include <m4thread.hpp>
#include <cloblconfiguration.hpp>
#include <snmpinterfaces.hpp>

#include "xercesc/parsers/XercesDOMParser.hpp"
#include "xercesc/util/XercesDefs.hpp"

XERCES_CPP_NAMESPACE_USE

XERCES_CPP_NAMESPACE_BEGIN
class DOMDocument;
class DOMNode;
XERCES_CPP_NAMESPACE_END

class Mib;
class RequestList;
class Snmpx;
class Vacm;



//## begin module%406BE37C02A8.declarations preserve=no
//## end module%406BE37C02A8.declarations

//## begin module%406BE37C02A8.additionalDeclarations preserve=yes
//## end module%406BE37C02A8.additionalDeclarations


//## begin M4SnmpAdministrator%406BD9E60047.preface preserve=yes
//## end M4SnmpAdministrator%406BD9E60047.preface

//## Class: M4SnmpAdministrator%406BD9E60047
//## Category: M4SnmpAdmin%37EA673500B9
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%407BEA9102B4;ClOblConfiguration { -> }

class M4SnmpAdministrator : public M4ISnmpAdministrator, //## Inherits: <unnamed>%406BD9F90134
                            	public M4Thread  //## Inherits: <unnamed>%406BDA12031B
{
  //## begin M4SnmpAdministrator%406BD9E60047.initialDeclarations preserve=yes
  //## end M4SnmpAdministrator%406BD9E60047.initialDeclarations

  public:
    //## Constructors (generated)
      M4SnmpAdministrator();

    //## Destructor (generated)
      virtual ~M4SnmpAdministrator();


    //## Other Operations (specified)
      //## Operation: Run%1080808685
      m4void_t Run ();

      //## Operation: RegisterSubsystem%1081341182
      M4ISnmpObject* RegisterSubsystem (M4ISnmpObject* ai_pParentSnmpObject, M4ISSApi4Snmp* ai_pSSApi, m4pcchar_t ai_type, m4pcchar_t ai_name);

      //## Operation: RegisterProperty%1081341183
      m4return_t RegisterProperty (m4pcchar_t ai_name, m4int32_t ai_propertyIndex, M4ISnmpObject* ai_poParentSnmpObject, ClConfiguration* ai_poConfig);

      //## Operation: Init%1081341184
      virtual m4return_t Init (ClConfiguration* ai_config);

      //## Operation: Start%1081862821
      m4return_t Start ();

      //## Operation: Stop%1081862822
      m4return_t Stop ();

      //## Operation: Destroy%1081862823
      m4return_t Destroy ();

      //## Operation: RegisterStatistic%1083310097
      m4return_t RegisterStatistic (m4pcchar_t ai_name, m4int32_t ai_statisticIndex, M4ISnmpObject* ai_poParentSnmpObject, ClConfiguration* ai_poConfig);

      //## Operation: UnregisterSubsystem%1083666427
      m4return_t UnregisterSubsystem (M4ISnmpObject* ai_poSnmpObject);

      //## Operation: Init%1084791066
      virtual m4return_t Init (const DOMNode* ai_config);

      //## Operation: RegisterUser%1084894580
      m4return_t RegisterUser (m4pcchar_t ai_name, m4pcchar_t ai_password);

    // Additional Public Declarations
      //## begin M4SnmpAdministrator%406BD9E60047.public preserve=yes
      //## end M4SnmpAdministrator%406BD9E60047.public

  protected:

    //## Other Operations (specified)
      //## Operation: _Run%1080808686
      virtual m4void_t _Run () = 0;

      //## Operation: LoadXmlConfig%1081862805
      m4return_t LoadXmlConfig (m4pcchar_t ai_pFile, m4pcchar_t ai_product);

      //## Operation: _Destroy%1082032850
      virtual m4return_t _Destroy () = 0;

      //## Operation: _Init%1084791691
      virtual m4return_t _Init (const DOMNode* ai_config) = 0;

    // Data Members for Class Attributes

      //## Attribute: m_snmpPort%40741F0D004B
      //## begin M4SnmpAdministrator::m_snmpPort%40741F0D004B.attr preserve=no  protected: m4int32_t {UA} -1
      m4int32_t m_snmpPort;
      //## end M4SnmpAdministrator::m_snmpPort%40741F0D004B.attr

    // Data Members for Associations

      //## Association: M4SnmpAdmin::<unnamed>%4073F825032F
      //## Role: M4SnmpAdministrator::<m_pRequestList>%4073F82602AF
      //## begin M4SnmpAdministrator::<m_pRequestList>%4073F82602AF.role preserve=no  protected: RequestList { -> RFHAN}
      Agentpp::RequestList *m_pRequestList;
      //## end M4SnmpAdministrator::<m_pRequestList>%4073F82602AF.role

      //## Association: M4SnmpAdmin::<unnamed>%4073F83A0112
      //## Role: M4SnmpAdministrator::<m_pMib>%4073F83A03E4
      //## begin M4SnmpAdministrator::<m_pMib>%4073F83A03E4.role preserve=no  protected: Mib { -> RFHAN}
	  Agentpp::Mib *m_pMib;
      //## end M4SnmpAdministrator::<m_pMib>%4073F83A03E4.role

    // Additional Protected Declarations
      //## begin M4SnmpAdministrator%406BD9E60047.protected preserve=yes
      //## end M4SnmpAdministrator%406BD9E60047.protected

  private:

    //## Other Operations (specified)
      //## Operation: CreateVacm%1082032854
	  Agentpp::Vacm* CreateVacm (const DOMNode* ai_config);

      //## Operation: CreateV3mp%1082032855
	  Snmp_pp::v3MP* CreateV3mp (const DOMNode* ai_config);

      //## Operation: SnmpPPInit%1084791690
      m4return_t SnmpPPInit (const DOMNode* ai_config);

      //## Operation: Obl2Xml%1084894578
      const DOMNode* Obl2Xml (ClConfiguration* ai_config);

    // Data Members for Class Attributes

      XercesDOMParser m_oParser;

      //## Attribute: m_snmpDOMDoc%407BEBA40024
      //## begin M4SnmpAdministrator::m_snmpDOMDoc%407BEBA40024.attr preserve=no  private: DOM_Document  {UA} 
      xercesc::DOMDocument* m_snmpDOMDoc;
      //## end M4SnmpAdministrator::m_snmpDOMDoc%407BEBA40024.attr

      //## Attribute: m_snmpRootOid%407BFEBD004C
      //## begin M4SnmpAdministrator::m_snmpRootOid%407BFEBD004C.attr preserve=no  private: m4string_t {UA} ""
      m4string_t m_snmpRootOid;
      //## end M4SnmpAdministrator::m_snmpRootOid%407BFEBD004C.attr

    // Additional Private Declarations
      //## begin M4SnmpAdministrator%406BD9E60047.private preserve=yes
      //## end M4SnmpAdministrator%406BD9E60047.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4SnmpAdministrator%406BD9E60047.implementation preserve=yes
      //## end M4SnmpAdministrator%406BD9E60047.implementation

};

//## begin M4SnmpAdministrator%406BD9E60047.postscript preserve=yes
//## end M4SnmpAdministrator%406BD9E60047.postscript

//## begin M4SnmpStandAloneAdministrator%406BE15F01A6.preface preserve=yes
//## end M4SnmpStandAloneAdministrator%406BE15F01A6.preface

//## Class: M4SnmpStandAloneAdministrator%406BE15F01A6
//## Category: M4SnmpAdmin%37EA673500B9
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4SnmpStandAloneAdministrator : public M4SnmpAdministrator  //## Inherits: <unnamed>%406BE17C028E
{
  //## begin M4SnmpStandAloneAdministrator%406BE15F01A6.initialDeclarations preserve=yes
  //## end M4SnmpStandAloneAdministrator%406BE15F01A6.initialDeclarations

  public:
    //## Constructors (generated)
      M4SnmpStandAloneAdministrator();

    //## Destructor (generated)
      virtual ~M4SnmpStandAloneAdministrator();


    //## Other Operations (specified)
      //## Operation: _Run%1080808687
      m4void_t _Run ();

    // Additional Public Declarations
      //## begin M4SnmpStandAloneAdministrator%406BE15F01A6.public preserve=yes
      //## end M4SnmpStandAloneAdministrator%406BE15F01A6.public

  protected:

    //## Other Operations (specified)
      //## Operation: _Destroy%1082032851
      m4return_t _Destroy ();

      //## Operation: _Init%1084791689
      m4return_t _Init (const DOMNode* ai_config);

    // Additional Protected Declarations
      //## begin M4SnmpStandAloneAdministrator%406BE15F01A6.protected preserve=yes
      //## end M4SnmpStandAloneAdministrator%406BE15F01A6.protected

  private:
    // Data Members for Associations




      //## Association: M4SnmpAdmin::<unnamed>%4073F5710289
      //## Role: M4SnmpStandAloneAdministrator::<m_pSnmpx>%4073F57200C9
      //## begin M4SnmpStandAloneAdministrator::<m_pSnmpx>%4073F57200C9.role preserve=no  private: Snmpx { -> RFHAN}
      Agentpp::Snmpx *m_pSnmpx;
      //## end M4SnmpStandAloneAdministrator::<m_pSnmpx>%4073F57200C9.role

    // Additional Private Declarations
      //## begin M4SnmpStandAloneAdministrator%406BE15F01A6.private preserve=yes
      //## end M4SnmpStandAloneAdministrator%406BE15F01A6.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4SnmpStandAloneAdministrator%406BE15F01A6.implementation preserve=yes
      //## end M4SnmpStandAloneAdministrator%406BE15F01A6.implementation

};

//## begin M4SnmpStandAloneAdministrator%406BE15F01A6.postscript preserve=yes
//## end M4SnmpStandAloneAdministrator%406BE15F01A6.postscript

// Class M4SnmpAdministrator 

// Class M4SnmpStandAloneAdministrator 

//## begin module%406BE37C02A8.epilog preserve=yes
//## end module%406BE37C02A8.epilog


#endif

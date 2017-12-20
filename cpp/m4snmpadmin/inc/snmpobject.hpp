//## begin module%406BEE62014C.cm preserve=no
//## end module%406BEE62014C.cm

//## begin module%406BEE62014C.cp preserve=no
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
//## end module%406BEE62014C.cp

//## Module: snmpobject%406BEE62014C; Package specification
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Source file: z:\m4snmpadmin\inc\snmpobject.hpp

#ifndef snmpobject_h
#define snmpobject_h 1

//## begin module%406BEE62014C.additionalIncludes preserve=no
//## end module%406BEE62014C.additionalIncludes

#include <agent_pp/mib.h>

#include <xercesc/util/XercesDefs.hpp>

#include "m4stl.hpp"
#include <snmpinterfaces.hpp>

//## begin module%406BEE62014C.declarations preserve=no
//## end module%406BEE62014C.declarations


XERCES_CPP_NAMESPACE_BEGIN
class DOMNode;
XERCES_CPP_NAMESPACE_END


//## begin module%406BEE62014C.additionalDeclarations preserve=yes
class M4MibTable;
class M4MibDynamicTable;
class M4MibStaticTable;
//## end module%406BEE62014C.additionalDeclarations


//## begin M4SnmpObject%406BEAB202CB.preface preserve=yes
//## end M4SnmpObject%406BEAB202CB.preface

//## Class: M4SnmpObject%406BEAB202CB
//## Category: M4SnmpAdmin%37EA673500B9
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4SnmpObject : public M4ISnmpObject, public Agentpp::MibLeaf  //## Inherits: <unnamed>%407D1CEE01F1
{
  //## begin M4SnmpObject%406BEAB202CB.initialDeclarations preserve=yes
  //## end M4SnmpObject%406BEAB202CB.initialDeclarations

  public:
    //## Constructors (generated)
      M4SnmpObject();

      M4SnmpObject(const M4SnmpObject &right);

    //## Destructor (generated)
      virtual ~M4SnmpObject();


    //## Other Operations (specified)
      //## Operation: GetClassId%1081862807
      m4string_t GetClassId ();

      //## Operation: GetInstanceId%1081862808
      m4string_t GetInstanceId ();

      //## Operation: GetOID%1081862809
      m4string_t GetOID ();

      //## Operation: GetDOMNode%1082042651
      const DOMNode* GetDOMNode ();

      //## Operation: GetMibType%1082986489
      M4MibType GetMibType ();

      //## Operation: GetStatus%1082986507
      virtual M4SnmpObjectStatus GetStatus ();

      //## Operation: SetStatus%1082986508
      virtual void SetStatus (M4SnmpObjectStatus ai_status);

    // Additional Public Declarations
      //## begin M4SnmpObject%406BEAB202CB.public preserve=yes
      //## end M4SnmpObject%406BEAB202CB.public

  protected:

    //## Other Operations (specified)
      //## Operation: SetSnmpAccess%1082361886
      void SetSnmpAccess ();

      //## Operation: SetSnmpSyntax%1082361888
      void SetSnmpSyntax (m4pcchar_t ai_value);

      //## Operation: SetDOMNode%1082042652
      void SetDOMNode (const DOMNode* ai_node);

      //## Operation: SetSnmpOid%1082361890
      void SetSnmpOid ();

      //## Operation: SetMibType%1082986488
      void SetMibType ();

    // Data Members for Class Attributes

      //## Attribute: m_classId%407272CC0105
      //## begin M4SnmpObject::m_classId%407272CC0105.attr preserve=no  protected: m4string_t {UA} 
      m4string_t m_classId;
      //## end M4SnmpObject::m_classId%407272CC0105.attr

      //## Attribute: m_instanceId%407272E3025D
      //## begin M4SnmpObject::m_instanceId%407272E3025D.attr preserve=no  protected: m4string_t {UA} 
      m4string_t m_instanceId;
      //## end M4SnmpObject::m_instanceId%407272E3025D.attr

      //## Attribute: m_domNode%407EA81700C8
      //## begin M4SnmpObject::m_domNode%407EA81700C8.attr preserve=no  protected: DOMNode {UA} 
      const DOMNode *m_domNode;
      //## end M4SnmpObject::m_domNode%407EA81700C8.attr

      //## Attribute: m_iSnmpSyntax%4085241C01B3
      //## begin M4SnmpObject::m_iSnmpSyntax%4085241C01B3.attr preserve=no  protected: int {UA} 
      int m_iSnmpSyntax;
      //## end M4SnmpObject::m_iSnmpSyntax%4085241C01B3.attr

      //## Attribute: m_mibType%408D226A038C
      //## begin M4SnmpObject::m_mibType%408D226A038C.attr preserve=no  protected: M4MibType {UA} 
      M4MibType m_mibType;
      //## end M4SnmpObject::m_mibType%408D226A038C.attr

      //## Attribute: m_status%408E6157024B
      //## begin M4SnmpObject::m_status%408E6157024B.attr preserve=no  protected: M4SnmpObjectStatus {UA} CREATED
      M4SnmpObjectStatus m_status;
      //## end M4SnmpObject::m_status%408E6157024B.attr

    // Additional Protected Declarations
      //## begin M4SnmpObject%406BEAB202CB.protected preserve=yes
      //## end M4SnmpObject%406BEAB202CB.protected

  private:
    // Additional Private Declarations
      //## begin M4SnmpObject%406BEAB202CB.private preserve=yes
      //## end M4SnmpObject%406BEAB202CB.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4SnmpObject%406BEAB202CB.implementation preserve=yes
      //## end M4SnmpObject%406BEAB202CB.implementation

};

//## begin M4SnmpObject%406BEAB202CB.postscript preserve=yes
//## end M4SnmpObject%406BEAB202CB.postscript

//## begin M4SnmpObjectVector%4097794A0115.preface preserve=yes
//## end M4SnmpObjectVector%4097794A0115.preface

//## Class: M4SnmpObjectVector%4097794A0115; Instantiated Class
//## Category: M4SnmpAdmin%37EA673500B9
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%40977CBE0148;M4SnmpObject { -> }

typedef vector< M4SnmpObject* > M4SnmpObjectVector;

//## begin M4SnmpObjectVector%4097794A0115.postscript preserve=yes
//## end M4SnmpObjectVector%4097794A0115.postscript

//## begin M4SnmpSubsystem%406BEB54025F.preface preserve=yes
//## end M4SnmpSubsystem%406BEB54025F.preface

//## Class: M4SnmpSubsystem%406BEB54025F
//## Category: M4SnmpAdmin%37EA673500B9
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4SnmpSubsystem : public M4SnmpObject  //## Inherits: <unnamed>%406BEB820189
{
  //## begin M4SnmpSubsystem%406BEB54025F.initialDeclarations preserve=yes
  //## end M4SnmpSubsystem%406BEB54025F.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4SnmpSubsystem%1081862813
      M4SnmpSubsystem (const m4string_t& ai_name, const DOMNode *ai_domNode, const m4string_t& ai_parentClassId, M4SnmpSubsystem* ai_poParentSubsystem, M4ISSApi4Snmp* ai_poSSApi);

    //## Destructor (generated)
      virtual ~M4SnmpSubsystem();


    //## Other Operations (specified)
      //## Operation: GetDOMPropertiesNode%1082108038
      const DOMNode* GetDOMPropertiesNode ();

      //## Operation: GetDOMStatisticsNode%1082108039
      const DOMNode* GetDOMStatisticsNode ();

      //## Operation: SetDOMNode%1082361889
      void SetDOMNode (const DOMNode *ai_node);

      //## Operation: GetSSApi%1082361897
      const M4ISSApi4Snmp* GetSSApi ();

      //## Operation: GetMibTable%1082986496
      virtual M4MibTable* GetMibTable ();

      //## Operation: GetMibTableRow%1083080474
	  Agentpp::MibTableRow* GetMibTableRow ();

      //## Operation: SetMibTableRow%1083080475
      void SetMibTableRow (Agentpp::MibTableRow* ai_poRow);

      //## Operation: AddChildObject%1083666428
      void AddChildObject (M4SnmpObject* ai_poSnmpObject);

      //## Operation: GetChildrenVector%1083666429
      const M4SnmpObjectVector& GetChildrenVector ();

      //## Operation: SetMibTable%1083741949
      virtual void SetMibTable (M4MibTable* ai_table);

      //## Operation: GetType%1083741950
      m4string_t GetType ();

      //## Operation: GetMibTableRowIndex%1083741956
      Oid GetMibTableRowIndex ();

    // Additional Public Declarations
      //## begin M4SnmpSubsystem%406BEB54025F.public preserve=yes
      //## end M4SnmpSubsystem%406BEB54025F.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_poMibTableRow%408E1BA602F2
      //## begin M4SnmpSubsystem::m_poMibTableRow%408E1BA602F2.attr preserve=no  protected: MibTableRow* {UA} NULL
	  Agentpp::MibTableRow* m_poMibTableRow;
      //## end M4SnmpSubsystem::m_poMibTableRow%408E1BA602F2.attr

    // Data Members for Associations


      //## Association: M4SnmpAdmin::<unnamed>%40977CCC03D3
      //## Role: M4SnmpSubsystem::<m_pM4SnmpObjectVector>%40977CCF01F7
      //## begin M4SnmpSubsystem::<m_pM4SnmpObjectVector>%40977CCF01F7.role preserve=no  protected: M4SnmpObjectVector { -> VHAN}
      M4SnmpObjectVector m_pM4SnmpObjectVector;
      //## end M4SnmpSubsystem::<m_pM4SnmpObjectVector>%40977CCF01F7.role

      //## Association: M4SnmpAdmin::<unnamed>%409A103D03DA
      //## Role: M4SnmpSubsystem::m_poParentSubsystem%409A1040026C
      //## begin M4SnmpSubsystem::m_poParentSubsystem%409A1040026C.role preserve=no  protected: M4SnmpSubsystem { -> RHAN}
      M4SnmpSubsystem *m_poParentSubsystem;
      //## end M4SnmpSubsystem::m_poParentSubsystem%409A1040026C.role

      //## Association: M4SnmpAdmin::<unnamed>%40A9BC280000
      //## Role: M4SnmpSubsystem::<m_pM4ISSApi4Snmp>%40A9BC2802B1
      //## begin M4SnmpSubsystem::<m_pM4ISSApi4Snmp>%40A9BC2802B1.role preserve=no  protected: M4ISSApi4Snmp { -> RHAN}
      M4ISSApi4Snmp *m_pM4ISSApi4Snmp;
      //## end M4SnmpSubsystem::<m_pM4ISSApi4Snmp>%40A9BC2802B1.role

    // Additional Protected Declarations
      //## begin M4SnmpSubsystem%406BEB54025F.protected preserve=yes
      //## end M4SnmpSubsystem%406BEB54025F.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_domPropertiesNode%407F8C200110
      //## begin M4SnmpSubsystem::m_domPropertiesNode%407F8C200110.attr preserve=no  private: DOMNode {UA} 
      const DOMNode *m_domPropertiesNode;
      //## end M4SnmpSubsystem::m_domPropertiesNode%407F8C200110.attr

      //## Attribute: m_domStatisticsNode%407F8C220177
      //## begin M4SnmpSubsystem::m_domStatisticsNode%407F8C220177.attr preserve=no  private: DOMNode {UA} 
      const DOMNode *m_domStatisticsNode;
      //## end M4SnmpSubsystem::m_domStatisticsNode%407F8C220177.attr

      //## Attribute: m_sstype%4085048202FA
      //## begin M4SnmpSubsystem::m_sstype%4085048202FA.attr preserve=no  private: m4string_t {UA} 
      m4string_t m_sstype;
      //## end M4SnmpSubsystem::m_sstype%4085048202FA.attr

      //## Attribute: m_poMibTable%408E35D501C4
      //## begin M4SnmpSubsystem::m_poMibTable%408E35D501C4.attr preserve=no  private: M4MibTable* {UA} NULL
      M4MibTable* m_poMibTable;
      //## end M4SnmpSubsystem::m_poMibTable%408E35D501C4.attr

    // Additional Private Declarations
      //## begin M4SnmpSubsystem%406BEB54025F.private preserve=yes
      //## end M4SnmpSubsystem%406BEB54025F.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4SnmpSubsystem%406BEB54025F.implementation preserve=yes
      //## end M4SnmpSubsystem%406BEB54025F.implementation

};

//## begin M4SnmpSubsystem%406BEB54025F.postscript preserve=yes
//## end M4SnmpSubsystem%406BEB54025F.postscript

//## begin M4SnmpProperty%406BEB66016B.preface preserve=yes
//## end M4SnmpProperty%406BEB66016B.preface

//## Class: M4SnmpProperty%406BEB66016B
//## Category: M4SnmpAdmin%37EA673500B9
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4SnmpProperty : public M4SnmpObject  //## Inherits: <unnamed>%406BEB8603B6
{
  //## begin M4SnmpProperty%406BEB66016B.initialDeclarations preserve=yes
  //## end M4SnmpProperty%406BEB66016B.initialDeclarations

  public:
    //## Constructors (generated)
      M4SnmpProperty(const M4SnmpProperty &right);

    //## Constructors (specified)
      //## Operation: M4SnmpProperty%1082108040
      M4SnmpProperty (const m4string_t& ai_name, m4int32_t ai_propertyIndex, const m4string_t& ai_classid, const DOMNode *ai_domNode, M4SnmpSubsystem* ai_poParentSnmp);

    //## Destructor (generated)
      virtual ~M4SnmpProperty();


    //## Other Operations (specified)
      //## Operation: get_request%1082361891; C++
      virtual void get_request (Agentpp::Request* ai_req, int ai_ind);

      //## Operation: set_value%1082361893; C++
      virtual int set_value (const Agentpp::Vbx& ai_vb);

      //## Operation: GetSnmpSubsystem%1083080478
      M4SnmpSubsystem* GetSnmpSubsystem ();

      //## Operation: SetSnmpSubsystem%1083080479
      void SetSnmpSubsystem (M4SnmpSubsystem* ai_poSubsystem);

      //## Operation: clone%1083080480; C++
      //	*
      //	* Serialize the value of the receiver.
      //	*
      //	* @param buf - A pointer to byte stream buffer returned.
      //	* @param sz - The size of the buffer returned.
      //	* @return TRUE if serialization was successful, FALSE otherwise.
      virtual Agentpp::MibEntryPtr clone ();

      //## Operation: SetName%1083080481
      void SetName (const m4string_t& ai_name);

      //## Operation: GetName%1083080482
      m4string_t GetName ();

      //## Operation: GetValue%1083310094
      virtual m4return_t GetValue (m4string_t& ao_value);

      //## Operation: SetPropertyIndex%1084894576
      void SetPropertyIndex (m4int32_t ai_propertyIndex);

    // Additional Public Declarations
      //## begin M4SnmpProperty%406BEB66016B.public preserve=yes
      //## end M4SnmpProperty%406BEB66016B.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_name%40839889032B
      //## begin M4SnmpProperty::m_name%40839889032B.attr preserve=no  protected: m4string_t {UA} 
      m4string_t m_name;
      //## end M4SnmpProperty::m_name%40839889032B.attr

      //## Attribute: m_propertyIndex%40AA2E6002B8
      //## begin M4SnmpProperty::m_propertyIndex%40AA2E6002B8.attr preserve=no  protected: m4int32_t {UA} 
      m4int32_t m_propertyIndex;
      //## end M4SnmpProperty::m_propertyIndex%40AA2E6002B8.attr

    // Data Members for Associations

      //## Association: M4SnmpAdmin::<unnamed>%408506DE0014
      //## Role: M4SnmpProperty::<m_pM4SnmpSubsystem>%408506DE028C
      //## begin M4SnmpProperty::<m_pM4SnmpSubsystem>%408506DE028C.role preserve=no  protected: M4SnmpSubsystem { -> RHAN}
      M4SnmpSubsystem *m_pM4SnmpSubsystem;
      //## end M4SnmpProperty::<m_pM4SnmpSubsystem>%408506DE028C.role

    // Additional Protected Declarations
      //## begin M4SnmpProperty%406BEB66016B.protected preserve=yes
      //## end M4SnmpProperty%406BEB66016B.protected

  private:
    // Additional Private Declarations
      //## begin M4SnmpProperty%406BEB66016B.private preserve=yes
      //## end M4SnmpProperty%406BEB66016B.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4SnmpProperty%406BEB66016B.implementation preserve=yes
      //## end M4SnmpProperty%406BEB66016B.implementation

};

//## begin M4SnmpProperty%406BEB66016B.postscript preserve=yes
//## end M4SnmpProperty%406BEB66016B.postscript

//## begin M4SnmpStatistic%4091FF7503B3.preface preserve=yes
//## end M4SnmpStatistic%4091FF7503B3.preface

//## Class: M4SnmpStatistic%4091FF7503B3
//## Category: M4SnmpAdmin%37EA673500B9
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4SnmpStatistic : public M4SnmpProperty  //## Inherits: <unnamed>%4091FF8301E7
{
  //## begin M4SnmpStatistic%4091FF7503B3.initialDeclarations preserve=yes
  //## end M4SnmpStatistic%4091FF7503B3.initialDeclarations

  public:
    //## Constructors (generated)
      M4SnmpStatistic(const M4SnmpStatistic &right);

    //## Constructors (specified)
      //## Operation: M4SnmpStatistic%1084894577
      M4SnmpStatistic (const m4string_t& ai_name, m4int32_t ai_propertyIndex, const m4string_t& ai_classid, const DOMNode *ai_domNode, M4SnmpSubsystem* ai_poParentSnmp);

    //## Destructor (generated)
      virtual ~M4SnmpStatistic();


    //## Other Operations (specified)
      //## Operation: GetValue%1083310095
      virtual m4return_t GetValue (m4string_t& ao_value);

      //## Operation: clone%1084977518; C++
      //	*
      //	* Serialize the value of the receiver.
      //	*
      //	* @param buf - A pointer to byte stream buffer returned.
      //	* @param sz - The size of the buffer returned.
      //	* @return TRUE if serialization was successful, FALSE otherwise.
      virtual Agentpp::MibEntryPtr clone ();

    // Additional Public Declarations
      //## begin M4SnmpStatistic%4091FF7503B3.public preserve=yes
      //## end M4SnmpStatistic%4091FF7503B3.public

  protected:
    // Additional Protected Declarations
      //## begin M4SnmpStatistic%4091FF7503B3.protected preserve=yes
      //## end M4SnmpStatistic%4091FF7503B3.protected

  private:
    // Additional Private Declarations
      //## begin M4SnmpStatistic%4091FF7503B3.private preserve=yes
      //## end M4SnmpStatistic%4091FF7503B3.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4SnmpStatistic%4091FF7503B3.implementation preserve=yes
      //## end M4SnmpStatistic%4091FF7503B3.implementation

};

//## begin M4SnmpStatistic%4091FF7503B3.postscript preserve=yes
//## end M4SnmpStatistic%4091FF7503B3.postscript

// Class M4SnmpObject 

// Class M4SnmpSubsystem 

// Class M4SnmpProperty 

// Class M4SnmpStatistic 

//## begin module%406BEE62014C.epilog preserve=yes
//## end module%406BEE62014C.epilog


#endif

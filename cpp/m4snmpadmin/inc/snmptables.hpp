//## begin module%408D08FB02FE.cm preserve=no
//## end module%408D08FB02FE.cm

//## begin module%408D08FB02FE.cp preserve=no
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
//## end module%408D08FB02FE.cp

//## Module: snmptables%408D08FB02FE; Package specification
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Source file: z:\m4snmpadmin\inc\snmptables.hpp

#ifndef snmptables_h
#define snmptables_h 1

//## begin module%408D08FB02FE.additionalIncludes preserve=no
//## end module%408D08FB02FE.additionalIncludes

#include "snmpobject.hpp"
#include "m4stl.hpp"

#include <agent_pp/mib.h>

//## end module%408D08FB02FE.declarations

//## begin module%408D08FB02FE.additionalDeclarations preserve=yes
//## end module%408D08FB02FE.additionalDeclarations


//## begin M4MibTable%4096132C0277.preface preserve=yes
//## end M4MibTable%4096132C0277.preface

//## Class: M4MibTable%4096132C0277
//## Category: M4SnmpAdmin%37EA673500B9
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4MibTable : public Agentpp::MibTable  //## Inherits: <unnamed>%4096137C010A
{
  //## begin M4MibTable%4096132C0277.initialDeclarations preserve=yes
  //## end M4MibTable%4096132C0277.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4MibTable%1083571295
      M4MibTable (const m4string_t& ai_classId, m4int32_t ai_indexSize);


    //## Other Operations (specified)
      //## Operation: AddCol%1083571296
      virtual void AddCol (Agentpp::MibLeaf* ai_poSnmpObject);

      //## Operation: AddRow%1083571297
      virtual Agentpp::MibTableRow* AddRow (const Oid& ai_index);

      //## Operation: AddCols%1083666430
      void AddCols (const DOMNode *ai_domNode);

      //## Operation: GetNextRowIndex%1083741955
      Oid GetNextRowIndex ();

    // Additional Public Declarations
      //## begin M4MibTable%4096132C0277.public preserve=yes
      //## end M4MibTable%4096132C0277.public

  protected:
    // Additional Protected Declarations
      //## begin M4MibTable%4096132C0277.protected preserve=yes
      //## end M4MibTable%4096132C0277.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_rowIndex%4099EA940106
      //## begin M4MibTable::m_rowIndex%4099EA940106.attr preserve=no  private: m4uint32_t {UA} 1
      m4uint32_t m_rowIndex;
      //## end M4MibTable::m_rowIndex%4099EA940106.attr

    // Additional Private Declarations
      //## begin M4MibTable%4096132C0277.private preserve=yes
      //## end M4MibTable%4096132C0277.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4MibTable%4096132C0277.implementation preserve=yes
      //## end M4MibTable%4096132C0277.implementation

};

//## begin M4MibTable%4096132C0277.postscript preserve=yes
//## end M4MibTable%4096132C0277.postscript

//## begin M4MibStaticTable%408D0854025D.preface preserve=yes
//## end M4MibStaticTable%408D0854025D.preface

//## Class: M4MibStaticTable%408D0854025D
//## Category: M4SnmpAdmin%37EA673500B9
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4MibStaticTable : public M4MibTable  //## Inherits: <unnamed>%409613B103A5
{
  //## begin M4MibStaticTable%408D0854025D.initialDeclarations preserve=yes
  //## end M4MibStaticTable%408D0854025D.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4MibStaticTable%1083571298
      M4MibStaticTable (const m4string_t& ai_classId, m4int32_t ai_indexSize);

    // Additional Public Declarations
      //## begin M4MibStaticTable%408D0854025D.public preserve=yes
      //## end M4MibStaticTable%408D0854025D.public

  protected:
    // Additional Protected Declarations
      //## begin M4MibStaticTable%408D0854025D.protected preserve=yes
      //## end M4MibStaticTable%408D0854025D.protected

  private:
    // Additional Private Declarations
      //## begin M4MibStaticTable%408D0854025D.private preserve=yes
      //## end M4MibStaticTable%408D0854025D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4MibStaticTable%408D0854025D.implementation preserve=yes
      //## end M4MibStaticTable%408D0854025D.implementation

};

//## begin M4MibStaticTable%408D0854025D.postscript preserve=yes
//## end M4MibStaticTable%408D0854025D.postscript

//## begin M4MibDynamicTable%408D085C03DB.preface preserve=yes
//## end M4MibDynamicTable%408D085C03DB.preface

//## Class: M4MibDynamicTable%408D085C03DB
//## Category: M4SnmpAdmin%37EA673500B9
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4MibDynamicTable : public M4MibTable  //## Inherits: <unnamed>%4096138B0328
{
  //## begin M4MibDynamicTable%408D085C03DB.initialDeclarations preserve=yes
  //## end M4MibDynamicTable%408D085C03DB.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4MibDynamicTable%1082986497
      M4MibDynamicTable (const m4string_t& ai_classId, m4int32_t ai_indexSize);

    // Additional Public Declarations
      //## begin M4MibDynamicTable%408D085C03DB.public preserve=yes
      //## end M4MibDynamicTable%408D085C03DB.public

  protected:
    // Additional Protected Declarations
      //## begin M4MibDynamicTable%408D085C03DB.protected preserve=yes
      //## end M4MibDynamicTable%408D085C03DB.protected

  private:
    // Additional Private Declarations
      //## begin M4MibDynamicTable%408D085C03DB.private preserve=yes
      //## end M4MibDynamicTable%408D085C03DB.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4MibDynamicTable%408D085C03DB.implementation preserve=yes
      //## end M4MibDynamicTable%408D085C03DB.implementation

};

//## begin M4MibDynamicTable%408D085C03DB.postscript preserve=yes
//## end M4MibDynamicTable%408D085C03DB.postscript

//## begin M4MibTableKeyLess%4098E27400B9.preface preserve=yes
//## end M4MibTableKeyLess%4098E27400B9.preface

//## Class: M4MibTableKeyLess%4098E27400B9
//## Category: M4SnmpAdmin%37EA673500B9
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4MibTableKeyLess 
{
  //## begin M4MibTableKeyLess%4098E27400B9.initialDeclarations preserve=yes
  //## end M4MibTableKeyLess%4098E27400B9.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: operator()%1083741953
      m4bool_t operator () (const m4string_t& ai_right, const m4string_t& ai_left) const;

    // Additional Public Declarations
      //## begin M4MibTableKeyLess%4098E27400B9.public preserve=yes
      //## end M4MibTableKeyLess%4098E27400B9.public

  protected:
    // Additional Protected Declarations
      //## begin M4MibTableKeyLess%4098E27400B9.protected preserve=yes
      //## end M4MibTableKeyLess%4098E27400B9.protected

  private:
    // Additional Private Declarations
      //## begin M4MibTableKeyLess%4098E27400B9.private preserve=yes
      //## end M4MibTableKeyLess%4098E27400B9.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4MibTableKeyLess%4098E27400B9.implementation preserve=yes
      //## end M4MibTableKeyLess%4098E27400B9.implementation

};

//## begin M4MibTableKeyLess%4098E27400B9.postscript preserve=yes
//## end M4MibTableKeyLess%4098E27400B9.postscript

//## begin M4MibTableMap_t%4098C180033A.preface preserve=yes
//## end M4MibTableMap_t%4098C180033A.preface

//## Class: M4MibTableMap_t%4098C180033A; Instantiated Class
//## Category: M4SnmpAdmin%37EA673500B9
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%4098C1F100F7;M4MibTable { -> }
//## Uses: <unnamed>%4098E3730015;M4MibTableKeyLess { -> }

typedef map< m4string_t,M4MibTable*,M4MibTableKeyLess > M4MibTableMap_t;

//## begin M4MibTableMap_t%4098C180033A.postscript preserve=yes
//## end M4MibTableMap_t%4098C180033A.postscript

//## begin M4MibTableContainer%4098C29F01DE.preface preserve=yes
//## end M4MibTableContainer%4098C29F01DE.preface

//## Class: M4MibTableContainer%4098C29F01DE
//## Category: M4SnmpAdmin%37EA673500B9
//## Subsystem: m4snmpadmin::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4MibTableContainer 
{
  //## begin M4MibTableContainer%4098C29F01DE.initialDeclarations preserve=yes
  //## end M4MibTableContainer%4098C29F01DE.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GetTable%1083741951
      static M4MibTable* GetTable (const m4string_t& ai_sstype);

      //## Operation: AddTable%1083741952
      static void AddTable (const m4string_t& ai_sstype, M4MibTable* ai_poTable);

      //## Operation: CreateTable%1083741954
      static M4MibTable* CreateTable (M4SnmpSubsystem* ai_poSnmpSubsystem);

    // Additional Public Declarations
      //## begin M4MibTableContainer%4098C29F01DE.public preserve=yes
      //## end M4MibTableContainer%4098C29F01DE.public

  protected:
    // Additional Protected Declarations
      //## begin M4MibTableContainer%4098C29F01DE.protected preserve=yes
      //## end M4MibTableContainer%4098C29F01DE.protected

  private:
    // Data Members for Associations

      //## Association: M4SnmpAdmin::<unnamed>%4098C2BC01D5
      //## Role: M4MibTableContainer::<m_container>%4098C2BC03DF
      //## begin M4MibTableContainer::<m_container>%4098C2BC03DF.role preserve=no  private: static M4MibTableMap_t { -> VHAN}
      static M4MibTableMap_t m_container;
      //## end M4MibTableContainer::<m_container>%4098C2BC03DF.role

    // Additional Private Declarations
      //## begin M4MibTableContainer%4098C29F01DE.private preserve=yes
      //## end M4MibTableContainer%4098C29F01DE.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4MibTableContainer%4098C29F01DE.implementation preserve=yes
      //## end M4MibTableContainer%4098C29F01DE.implementation

};

//## begin M4MibTableContainer%4098C29F01DE.postscript preserve=yes
//## end M4MibTableContainer%4098C29F01DE.postscript

// Class M4MibTable 

// Class M4MibStaticTable 

// Class M4MibDynamicTable 

// Class M4MibTableKeyLess 

// Class M4MibTableContainer 

//## begin module%408D08FB02FE.epilog preserve=yes
//## end module%408D08FB02FE.epilog


#endif

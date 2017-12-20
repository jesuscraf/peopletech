//## begin module%3A3E58540028.cm preserve=no
//## end module%3A3E58540028.cm

//## begin module%3A3E58540028.cp preserve=no
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
//## end module%3A3E58540028.cp

//## Module: m4mdtodbc%3A3E58540028; Package specification
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Source file: C:\m4server\m4mdtransfer\inc\m4mdtodbc.hpp

#ifndef m4mdtodbc_h
#define m4mdtodbc_h 1

//## begin module%3A3E58540028.additionalIncludes preserve=no
//## end module%3A3E58540028.additionalIncludes

//## begin module%3A3E58540028.includes preserve=yes
// m4types
#include <m4types.hpp>
#include "m4stl.hpp"
#include "m4mdtransfer_dll.hpp"

// DBWrap.
#include "conex.hpp"
//## end module%3A3E58540028.includes

// m4mdtinterfaces
#include <m4mdtinterfaces.hpp>
// m4dbtypes
#include <m4dbtypes.hpp>

//## begin module%3A3E58540028.declarations preserve=no
//## end module%3A3E58540028.declarations

//## begin module%3A3E58540028.additionalDeclarations preserve=yes
//## end module%3A3E58540028.additionalDeclarations


//## begin fieldValue_t%4056C00F014C.preface preserve=yes
//## end fieldValue_t%4056C00F014C.preface

//## Class: fieldValue_t%4056C00F014C; Instantiated Class
//	Vector containing field values (strings).
//## Category: M4MDTransfer%3A3DFB720294
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef vector< ClMDTValue > fieldValue_t;

//## begin fieldValue_t%4056C00F014C.postscript preserve=yes
//## end fieldValue_t%4056C00F014C.postscript

//## begin M4MDTransferConnection%40599E870364.preface preserve=yes
//## end M4MDTransferConnection%40599E870364.preface

//## Class: M4MDTransferConnection%40599E870364
//## Category: M4MDTransfer%3A3DFB720294
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%40599FD80098;string { -> }
//## Uses: <unnamed>%406153BA014E;eDDBBType_t { -> }

class M4MDTransferConnection 
{
  //## begin M4MDTransferConnection%40599E870364.initialDeclarations preserve=yes
  //## end M4MDTransferConnection%40599E870364.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4MDTransferConnection%1079428405
      M4MDTransferConnection (const string &ai_sConnectionName, const string &ai_sConnectionString, eDDBBType_t ai_eDDBBType);

    //## Destructor (specified)
      //## Operation: ~M4MDTransferConnection%1079428406
      virtual ~M4MDTransferConnection ();


    //## Other Operations (specified)
      //## Operation: Init%1083142244
      m4return_t Init ();

      //## Operation: GetDBConnection%1079964156
      ClDBConnection * GetDBConnection ();

      //## Operation: GetDDBBType%1079428404
      eDDBBType_t GetDDBBType ();

    // Additional Public Declarations
      //## begin M4MDTransferConnection%40599E870364.public preserve=yes
      //## end M4MDTransferConnection%40599E870364.public

  protected:
    // Additional Protected Declarations
      //## begin M4MDTransferConnection%40599E870364.protected preserve=yes
      //## end M4MDTransferConnection%40599E870364.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_pDBConnection%406150070188
      //## begin M4MDTransferConnection::m_pDBConnection%406150070188.attr preserve=no  private: ClDBConnection * {UA} NULL
      ClDBConnection *m_pDBConnection;
      //## end M4MDTransferConnection::m_pDBConnection%406150070188.attr

      //## Attribute: m_sConnectionName%408F747B0009
      //## begin M4MDTransferConnection::m_sConnectionName%408F747B0009.attr preserve=no  private: string {UA} 
      string m_sConnectionName;
      //## end M4MDTransferConnection::m_sConnectionName%408F747B0009.attr

      //## Attribute: m_stConnectionProperties%408F703400C3
      //## begin M4MDTransferConnection::m_stConnectionProperties%408F703400C3.attr preserve=no  private: stConexionProperties {UA} 
      stConexionProperties m_stConnectionProperties;
      //## end M4MDTransferConnection::m_stConnectionProperties%408F703400C3.attr

    // Additional Private Declarations
      //## begin M4MDTransferConnection%40599E870364.private preserve=yes
      //## end M4MDTransferConnection%40599E870364.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4MDTransferConnection%40599E870364.implementation preserve=yes
      //## end M4MDTransferConnection%40599E870364.implementation

};

//## begin M4MDTransferConnection%40599E870364.postscript preserve=yes
//## end M4MDTransferConnection%40599E870364.postscript

//## begin connectionMap_t%40598BC700D7.preface preserve=yes
//## end connectionMap_t%40598BC700D7.preface

//## Class: connectionMap_t%40598BC700D7; Instantiated Class
//	This map contains the connection name and connections objects, composed by:
//	DB connection (ClDBConnection) and some connection properties.
//## Category: M4MDTransfer%3A3DFB720294
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%4059A03C01F0;M4MDTransferConnection { -> }

typedef map< string,M4MDTransferConnection *,less <string> > connectionMap_t;

//## begin connectionMap_t%40598BC700D7.postscript preserve=yes
//## end connectionMap_t%40598BC700D7.postscript

//## begin bindColumn_t%40614EBE0044.preface preserve=yes
//## end bindColumn_t%40614EBE0044.preface

//## Class: bindColumn_t%40614EBE0044; Instantiated Class
//	Database record set.
//## Category: M4MDTransfer%3A3DFB720294
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n


// UNICODE DB
struct StData
{
	m4pchar_t	m_pcValue;
	m4int32_t	m_iPrec;

	StData (void)
	{
		m_pcValue = NULL;
		m_iPrec = 0;
	}
	
	StData( m4pchar_t ai_pcValue, m4int32_t ai_iPrec )
	{
		m_pcValue = ai_pcValue;
		m_iPrec = ai_iPrec;
	}
};


typedef vector< StData* > bindColumn_t;

//## begin bindColumn_t%40614EBE0044.postscript preserve=yes
//## end bindColumn_t%40614EBE0044.postscript

//## begin M4FieldDescription%4063EC8A037C.preface preserve=yes
//## end M4FieldDescription%4063EC8A037C.preface

//## Class: M4FieldDescription%4063EC8A037C
//## Category: M4MDTransfer%3A3DFB720294
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4MDTRANSFER M4FieldDescription 
{
  //## begin M4FieldDescription%4063EC8A037C.initialDeclarations preserve=yes
  //## end M4FieldDescription%4063EC8A037C.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4FieldDescription%1080297558
      //	Field description class constructor. It recieves field info like: m4type,
      //	precision, scale and field position.
      //
      //	 @param ai_iM4Type field m4type.
      //	 @param ai_iPrecision field precision.
      //	 @param ai_iScale field scale.
      M4FieldDescription (m4uint8_t ai_iM4Type, m4uint32_t ai_iPrecision, m4uint32_t ai_iScale);

      //## Operation: M4FieldDescription%1080551835
      //	Default class constructor.
      M4FieldDescription ();

    //## Destructor (specified)
      //## Operation: ~M4FieldDescription%1080297559
      //	Field Description class destructor.
      virtual ~M4FieldDescription ();


    //## Other Operations (specified)
      //## Operation: GetM4Type%1080297560
      //	Returns field M4Type.
      //
      //	 @return field m4type.
      m4uint8_t GetM4Type () const;

      //## Operation: GetPrecision%1080297561
      //	Returns field precision.
      //
      //	 @return field precision.
      m4uint32_t GetPrecision () const;

      //## Operation: GetScale%1080297562
      //	Returns field scale.
      //
      //	 @return field scale.
      m4uint32_t GetScale () const;

      //## Operation: SetM4Type%1080551836
      //	Sets field m4type.
      //
      //	 @param ai_iM4Type field m4type.
      m4return_t SetM4Type (m4uint8_t ai_iM4Type);

      //## Operation: SetPrecision%1080551837
      //	Sets field precision.
      //
      //	 @param ai_iPrecision field precision.
      m4return_t SetPrecision (m4uint32_t ai_iPrecision);

      //## Operation: SetScale%1080551838
      //	Sets field scale.
      //
      //	 @param ai_iScale field scale.
      m4return_t SetScale (m4uint32_t ai_iScale);

    // Additional Public Declarations
      //## begin M4FieldDescription%4063EC8A037C.public preserve=yes
      //## end M4FieldDescription%4063EC8A037C.public

  protected:
    // Additional Protected Declarations
      //## begin M4FieldDescription%4063EC8A037C.protected preserve=yes
      //## end M4FieldDescription%4063EC8A037C.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iM4Type%4063ECA2014F
      //## begin M4FieldDescription::m_iM4Type%4063ECA2014F.attr preserve=no  private: m4int8_t {UA} 
      m4int8_t m_iM4Type;
      //## end M4FieldDescription::m_iM4Type%4063ECA2014F.attr

      //## Attribute: m_iPrecision%4063ECD1000D
      //## begin M4FieldDescription::m_iPrecision%4063ECD1000D.attr preserve=no  private: m4int32_t {UA} 
      m4int32_t m_iPrecision;
      //## end M4FieldDescription::m_iPrecision%4063ECD1000D.attr

      //## Attribute: m_iScale%4063ECDA0128
      //## begin M4FieldDescription::m_iScale%4063ECDA0128.attr preserve=no  private: m4int32_t {UA} 
      m4int32_t m_iScale;
      //## end M4FieldDescription::m_iScale%4063ECDA0128.attr

    // Additional Private Declarations
      //## begin M4FieldDescription%4063EC8A037C.private preserve=yes
      //## end M4FieldDescription%4063EC8A037C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4FieldDescription%4063EC8A037C.implementation preserve=yes
      //## end M4FieldDescription%4063EC8A037C.implementation

};

//## begin M4FieldDescription%4063EC8A037C.postscript preserve=yes
//## end M4FieldDescription%4063EC8A037C.postscript

//## begin fieldDescriptionMap_t%4056CD680196.preface preserve=yes
//## end fieldDescriptionMap_t%4056CD680196.preface

//## Class: fieldDescriptionMap_t%4056CD680196; Instantiated Class
//## Category: M4MDTransfer%3A3DFB720294
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%4063EE18019D;M4FieldDescription { -> }
//## Uses: <unnamed>%4098EAB4013B;m4uint32_t { -> }

typedef map< m4uint32_t,M4FieldDescription *,less <m4uint32_t> > fieldDescriptionMap_t;

//## begin fieldDescriptionMap_t%4056CD680196.postscript preserve=yes
//## end fieldDescriptionMap_t%4056CD680196.postscript

//## begin fieldDescription_t%406953C002AF.preface preserve=yes
//## end fieldDescription_t%406953C002AF.preface

//## Class: fieldDescription_t%406953C002AF; Instantiated Class
//## Category: M4MDTransfer%3A3DFB720294
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%4069547101EB;M4FieldDescription { -> }

typedef vector< M4FieldDescription * > fieldDescription_t;

//## begin fieldDescription_t%406953C002AF.postscript preserve=yes
//## end fieldDescription_t%406953C002AF.postscript

//## begin M4Row%4056BEA9002F.preface preserve=yes
//## end M4Row%4056BEA9002F.preface

//## Class: M4Row%4056BEA9002F
//	This class implements M4IReadRow interface and contains row field values.
//## Category: M4MDTransfer%3A3DFB720294
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4MDTRANSFER M4Row : public M4IReadRow  //## Inherits: <unnamed>%4056BEBB02E8
{
  //## begin M4Row%4056BEA9002F.initialDeclarations preserve=yes
  //## end M4Row%4056BEA9002F.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4Row%1079428372
      //	Row class constructor.
      M4Row ();

    //## Destructor (specified)
      //## Operation: ~M4Row%1079428373
      //	Row class destructor.
      virtual ~M4Row ();


    //## Other Operations (specified)
      //## Operation: Init%1083569620
      //	Initialize field value vector.
      //
      //	 @param ai_iColNumber number of fields.
      m4return_t Init (m4uint32_t ai_iColNumber);

      //## Operation: Reset%1084174996
      m4return_t Reset ();

      //## Operation: SetValue%1079428374
      //	Sets column value.
      //
      //	  @param ai_iColNumber column number.
      //	  @param ai_oColValue column value.
      m4return_t SetValue (m4uint32_t ai_iColNumber, const ClMDTValue &ai_oColValue);

      //## Operation: GetValue%1079428383
      //	Retrieves the value of a given column.
      //
      //	  @param ai_iColNumber column number.
      //	  @param ao_oColValue column value.
      virtual m4return_t GetValue (m4uint32_t ai_iColNumber, ClMDTValue &ao_oColValue) const;

      //## Operation: GetSize%1079428384
      //	Returns row size.
      //
      //	 @return row size.
      virtual m4uint32_t GetSize () const;

      //## Operation: GetFieldDescription%1080631725
      //	Retrieves field description, identified by its position.
      //
      //	  @param ai_iColNumber field position.
      //	  @param ao_iFieldM4Type field m4type.
      //	  @param ao_iFieldPrecision field precision.
      //	  @param ao_iFieldScale field scale.
      virtual m4return_t GetFieldDescription (m4uint32_t ai_iColNumber, m4uint8_t &ao_iFieldM4Type, m4uint32_t &ao_iFieldPrecision, m4uint32_t &ao_iFieldScale) const;

      //## Operation: SetFieldDescription%1080631726
      //	Sets field description for a field, identified by its position.
      //
      //	 @param ai_iColNumber field position.
      //	 @param ai_iM4Type field m4type.
      //	 @param ai_iPrecision field precision.
      //	 @param ai_iScale field scale.
      m4return_t SetFieldDescription (m4uint32_t ai_iColNumber, m4uint8_t ai_iM4Type, m4uint32_t ai_iPrecision, m4uint32_t ai_iScale);

    // Additional Public Declarations
      //## begin M4Row%4056BEA9002F.public preserve=yes
      //## end M4Row%4056BEA9002F.public

  protected:
    // Data Members for Associations

      //## Association: M4MDTransfer::<unnamed>%4056C07901DB
      //## Role: M4Row::m_vFieldValueList%4056C07903B3
      //## begin M4Row::m_vFieldValueList%4056C07903B3.role preserve=no  protected: fieldValue_t { -> VHAN}
      fieldValue_t m_vFieldValueList;
      //## end M4Row::m_vFieldValueList%4056C07903B3.role

      //## Association: M4MDTransfer::<unnamed>%4069543701B5
      //## Role: M4Row::m_vFieldDescriptionList%406954380012
      //## begin M4Row::m_vFieldDescriptionList%406954380012.role preserve=no  protected: fieldDescription_t { -> VHAN}
      fieldDescription_t m_vFieldDescriptionList;
      //## end M4Row::m_vFieldDescriptionList%406954380012.role

    // Additional Protected Declarations
      //## begin M4Row%4056BEA9002F.protected preserve=yes
      //## end M4Row%4056BEA9002F.protected

  private:
    // Additional Private Declarations
      //## begin M4Row%4056BEA9002F.private preserve=yes
      //## end M4Row%4056BEA9002F.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4Row%4056BEA9002F.implementation preserve=yes
      //## end M4Row%4056BEA9002F.implementation

};

//## begin M4Row%4056BEA9002F.postscript preserve=yes
//## end M4Row%4056BEA9002F.postscript

//## begin rowList_t%405719A502DA.preface preserve=yes
//## end rowList_t%405719A502DA.preface

//## Class: rowList_t%405719A502DA; Instantiated Class
//	This vector contains all registers or rows (M4Row) returned by the database
//	when a query is executed.
//## Category: M4MDTransfer%3A3DFB720294
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%405719DC001C;M4Row { -> }

typedef vector< M4Row * > rowList_t;

//## begin rowList_t%405719A502DA.postscript preserve=yes
//## end rowList_t%405719A502DA.postscript

//## begin bindParameter_t%4073AB6D037F.preface preserve=yes
//## end bindParameter_t%4073AB6D037F.preface

//## Class: bindParameter_t%4073AB6D037F; Instantiated Class
//## Category: M4MDTransfer%3A3DFB720294
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef vector< m4pchar_t > bindParameter_t;

//## begin bindParameter_t%4073AB6D037F.postscript preserve=yes
//## end bindParameter_t%4073AB6D037F.postscript

//## begin bindColumnLength_t%4098C42C0296.preface preserve=yes
//## end bindColumnLength_t%4098C42C0296.preface

//## Class: bindColumnLength_t%4098C42C0296; Instantiated Class
//## Category: M4MDTransfer%3A3DFB720294
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef vector< m4int32_t * > bindColumnLength_t;

//## begin bindColumnLength_t%4098C42C0296.postscript preserve=yes
//## end bindColumnLength_t%4098C42C0296.postscript

//## begin M4SorteableRow%40E9045F02E5.preface preserve=yes
//## end M4SorteableRow%40E9045F02E5.preface

//## Class: M4SorteableRow%40E9045F02E5
//## Category: M4MDTransfer%3A3DFB720294
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4SorteableRow 
{
  //## begin M4SorteableRow%40E9045F02E5.initialDeclarations preserve=yes
  //## end M4SorteableRow%40E9045F02E5.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4SorteableRow%1089013113
      M4SorteableRow (const m4int32_t ai_viOrder[], M4Row* ai_poRow);

    // Data Members for Class Attributes

      //## Attribute: m_viSortOrder%40E904D80372
      //## begin M4SorteableRow::m_viSortOrder%40E904D80372.attr preserve=no  public: const m4int32_t* {UA} ai_viOrder
      const m4int32_t* m_viSortOrder;
      //## end M4SorteableRow::m_viSortOrder%40E904D80372.attr

    // Data Members for Associations

      //## Association: M4MDTransfer::<unnamed>%40E904AC00A3
      //## Role: M4SorteableRow::m_poRow%40E904AC01FC
      //## begin M4SorteableRow::m_poRow%40E904AC01FC.role preserve=no  public: M4Row { -> RHAN}
      M4Row *m_poRow;
      //## end M4SorteableRow::m_poRow%40E904AC01FC.role

    // Additional Public Declarations
      //## begin M4SorteableRow%40E9045F02E5.public preserve=yes
      //## end M4SorteableRow%40E9045F02E5.public

  protected:
    // Additional Protected Declarations
      //## begin M4SorteableRow%40E9045F02E5.protected preserve=yes
      //## end M4SorteableRow%40E9045F02E5.protected

  private:
    // Additional Private Declarations
      //## begin M4SorteableRow%40E9045F02E5.private preserve=yes
      //## end M4SorteableRow%40E9045F02E5.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4SorteableRow%40E9045F02E5.implementation preserve=yes
      //## end M4SorteableRow%40E9045F02E5.implementation

};

//## begin M4SorteableRow%40E9045F02E5.postscript preserve=yes
//## end M4SorteableRow%40E9045F02E5.postscript

//## begin M4ResultSet%3A3DFC4E01D2.preface preserve=yes
//## end M4ResultSet%3A3DFC4E01D2.preface

//## Class: M4ResultSet%3A3DFC4E01D2
//	Represents the columns returned by a database query execution. It gives
//	methods to read and write within the record set.
//## Category: M4MDTransfer%3A3DFB720294
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%40E9049400B3;M4SorteableRow { -> }

class M4_DECL_M4MDTRANSFER M4ResultSet : public M4IReadResultSet, //## Inherits: <unnamed>%3AE689E2014B
    	public M4IWriteResultSet  //## Inherits: <unnamed>%4061494700A7
{
  //## begin M4ResultSet%3A3DFC4E01D2.initialDeclarations preserve=yes
  //## end M4ResultSet%3A3DFC4E01D2.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4ResultSet%1079428368
      //	Class constructor.
      M4ResultSet ();

    //## Destructor (specified)
      //## Operation: ~M4ResultSet%1079428369
      //	Class destructor.
      virtual ~M4ResultSet ();


    //## Other Operations (specified)
      //## Operation: SetValue%1079428371
      //	Sets the field value within a row identified by its position or column.
      //
      //	  @param ai_iRowNumber row number.
      //	  @param ai_iColNumber column number.
      //	  @param ai_oFieldValue field value.
      virtual m4return_t SetValue (m4uint32_t ai_iRowNumber, m4uint32_t ai_iColNumber, const ClMDTValue &ai_oFieldValue);

      //## Operation: GetRowNumber%1079428389
      //	Returns number of rows for result set.
      //
      //	 @return row number.
      virtual m4uint32_t GetRowNumber () const;

      //## Operation: GetValue%1079428391
      //	Returns the field value within a row identified by its column or position.
      //
      //	  @param ai_iRowNumber row number.
      //	  @param ai_iColNumber column number .
      //	  @param ao_oFieldValue field value.
      virtual m4return_t GetValue (m4uint32_t ai_iRowNumber, m4uint32_t ai_iColNumber, ClMDTValue &ao_oFieldValue) const;

      //## Operation: AddRow%1079428392
      //	Adds a new row to the result set.
      //	The row size will be the size of field description map.
      //
      //	 @return position of new row, -1 if an error happens.
      virtual m4int32_t AddRow ();

      //## Operation: GetFieldDescription%1080551834
      //	Retrieves the field description, identified by its position.
      //
      //	  @param ai_iColNumber field position.
      //	  @param ao_iFieldM4Type field m4type.
      //	  @param ao_iFieldPrecision field precision.
      //	  @param ao_iFieldScale field scale.
      virtual m4return_t GetFieldDescription (m4uint32_t ai_iColNumber, m4uint8_t &ao_iFieldM4Type, m4uint32_t &ao_iFieldPrecision, m4uint32_t &ao_iFieldScale) const;

      //## Operation: SetFieldDescription%1080631723
      //	Sets a new field description.
      //
      //	  @param ai_iColNumber field position.
      //	  @param ai_iFieldM4Type m4type.
      //	  @param ai_iFieldPrecision field precision.
      //	  @param ai_iFieldScale field scale.
      virtual m4return_t SetFieldDescription (m4uint32_t ai_iColNumber, m4uint8_t ai_iFieldM4Type, m4uint32_t ai_iFieldPrecision, m4uint32_t ai_iFieldScale);

      //## Operation: GetFieldNumber%1080551842
      //	Returns number of fields.
      //
      //	 @return field number.
      virtual m4uint32_t GetFieldNumber () const;

      //## Operation: Sort%1085386249
      virtual m4return_t Sort (m4comparef_t ai_fFunction, const m4int32_t ai_viOrder[]);

    // Additional Public Declarations
      //## begin M4ResultSet%3A3DFC4E01D2.public preserve=yes
      //## end M4ResultSet%3A3DFC4E01D2.public

  protected:

    //## Other Operations (specified)
      //## Operation: GetRow%1079964159
      //	Returns a row identified by its position (row number)
      //
      //	 @param ai_iRowNumber row number
      //	 @param ao_oRow row.
      m4return_t GetRow (m4uint32_t ai_iRowNumber, M4Row *&ao_oRow) const;

    // Data Members for Associations

      //## Association: M4MDTransfer::<unnamed>%4056CD9701CF
      //## Role: M4ResultSet::m_oFieldDescriptionMap%4056CD98009B
      //## begin M4ResultSet::m_oFieldDescriptionMap%4056CD98009B.role preserve=no  protected: fieldDescriptionMap_t { -> VHAN}
      fieldDescriptionMap_t m_oFieldDescriptionMap;
      //## end M4ResultSet::m_oFieldDescriptionMap%4056CD98009B.role

      //## Association: M4MDTransfer::<unnamed>%405719E10313
      //## Role: M4ResultSet::m_vRowList%405719E202D9
      //## begin M4ResultSet::m_vRowList%405719E202D9.role preserve=no  protected: rowList_t { -> VHAN}
      rowList_t m_vRowList;
      //## end M4ResultSet::m_vRowList%405719E202D9.role

    // Additional Protected Declarations
      //## begin M4ResultSet%3A3DFC4E01D2.protected preserve=yes
      //## end M4ResultSet%3A3DFC4E01D2.protected

  private:
    // Additional Private Declarations
      //## begin M4ResultSet%3A3DFC4E01D2.private preserve=yes
      //## end M4ResultSet%3A3DFC4E01D2.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4ResultSet%3A3DFC4E01D2.implementation preserve=yes
      //## end M4ResultSet%3A3DFC4E01D2.implementation

};

//## begin M4ResultSet%3A3DFC4E01D2.postscript preserve=yes
//## end M4ResultSet%3A3DFC4E01D2.postscript

//## begin M4ODBC%4056C67E001E.preface preserve=yes
//## end M4ODBC%4056C67E001E.preface

//## Class: M4ODBC%4056C67E001E
//	This class will provide an API to execute database queries.
//## Category: M4MDTransfer%3A3DFB720294
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%4056CB35039F;M4ResultSet { -> }
//## Uses: <unnamed>%4056CB380028;M4Row { -> }
//## Uses: <unnamed>%406A744A0371;m4uint32_t { -> F}

class M4_DECL_M4MDTRANSFER M4ODBC : public M4IODBC  //## Inherits: <unnamed>%4056C68A0148
{
  //## begin M4ODBC%4056C67E001E.initialDeclarations preserve=yes
  //## end M4ODBC%4056C67E001E.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4ODBC%1079428379
      //	Class constructor.
      M4ODBC ();

    //## Destructor (specified)
      //## Operation: ~M4ODBC%1079428380
      //	Class destructor.
      virtual ~M4ODBC ();


    //## Other Operations (specified)
      //## Operation: Init%1079428385
      //	Initialization.
      m4return_t Init (m4uint32_t ai_iTraceLevel, m4pcchar_t ai_pccFileDir);

      //## Operation: AddConnection%1079428388
      //	Adds a new connection.
      //
      //	  @param ai_sConnectionName connection name.
      //	  @param ai_sConnectionString connection string.
      //	  @param ai_eDBType database type.
      virtual m4return_t AddConnection (const string &ai_sConnectionName, const string &ai_sConnectionString, const eDDBBType_t &ai_eDBType);

      //## Operation: ExecuteQuery%1079428386
      //	Executes a given SQL statement with the given parameters (M4IReadRow). If
      //	the statement has returned data, it will be returned within the M4IWrite
      //	ResultSet object.
      //
      //	The statement will be executed using the given connection.
      //
      //	  @param ai_sConnectionName connection name.
      //	  @param ai_sStatement SQL statement.
      //	  @param ai_poStmtParam statement parameters.
      //	  @param ai_bIsSelect true if it is a select statement.
      //	  @param ao_oResultSet statement returned data.
      virtual m4return_t ExecuteQuery (const string &ai_sConnectionName, const string &ai_sStatement, const M4IReadRow *ai_poStmtParam,  m4bool_t ai_bIsSelect, M4IWriteResultSet &ao_oResultSet);

      //## Operation: BeginTransaction%1084786093
      //	Begin transaction form the given connection.
      //
      //	 @param ai_sConnectionName connection name.
      virtual m4return_t BeginTransaction (const string &ai_sConnectionName);

      //## Operation: Commit%1084786094
      //	Request a commit operation for all active operations on all statements
      //	associated with a connection.
      //
      //	 @param ai_sConnectionName connection name.
      virtual m4return_t Commit (const string &ai_sConnectionName);

      //## Operation: RollBack%1084786095
      //	Request a rollback operation for all active operations on all statements
      //	associated with a connection.
      //
      //	 @param ai_sConnectionName connection name.
      virtual m4return_t RollBack (const string &ai_sConnectionName);

    // Additional Public Declarations
      //## begin M4ODBC%4056C67E001E.public preserve=yes
      //## end M4ODBC%4056C67E001E.public

  protected:

    //## Other Operations (specified)
      //## Operation: GetConnection%1079428381
      //	Recover connection object (identified by its id.) from connection map.
      //
      //	 @param ai_sConnectionName connection name.
      //	 @param ao_oConnection connection object.
      m4return_t GetConnection (const string &ai_sConnectionName, ClDBConnection *&ao_oConnection);

      //## Operation: GetDDBBType%1079428397
      //	Returns database type for the given connection.
      //
      //	 @param ai_sConnectionName connection name.
      //	 @param ao_eDDBBType database type.
      m4return_t GetDDBBType (const string &ai_sConnectionName, eDDBBType_t &ao_eDDBBType);

      //## Operation: GetMaxBlobLength%1089121924
      //	Calculate max. blob length, parameter necessary to bind blobs.
      //
      //	 @param ai_pcBlobParamValue param value to bind for  blobs: blob
      //	Header+descripción\0\Ext\0pathFile, where blob header is ~BLOBD.
      //	 @param ao_iMaxLenBlobField length to bind blob  parameter.
      m4return_t GetMaxBlobLength (m4pcchar_t ai_pcBlobParamValue, m4int32_t &ao_iMaxLenBlobField);

      //## Operation: GetData%1079428395
      //	Gets column descriptions from database and binds them. It fetches the data
      //	from the database and store them in M4IWriteResultSet object.
      //
      //	 @param ai_pCursor database cursor.
      //	 @param ai_sConnectionName connection name.
      //	 @param ao_oResultSet record set to be filled with the data   fetched from
      //	the database.
      m4return_t GetData (ClCursor *ai_pCursor, const string &ai_sConnectionName, vector <m4uint32_t> &ai_vFieldDescription, M4IWriteResultSet &ao_oResultSet);

      //## Operation: GetLongData%1081845829
      //	Gets data from a LONG field.
      //
      //	 @param ai_pCursor database cursor.
      //	 @param ai_FieldCol filed position.
      //	 @param ao_sLongData long data.
      m4return_t GetLongData (ClCursor *ai_pCursor, m4uint32_t ai_FieldCol, m4int32_t ai_iM4Type, string &ao_sLongData, m4bool_t &ao_bIsNull);

      //## Operation: GetODBCError%1082109913
      //	Retrieves an ODBC error.
      //
      //	 @param ai_pCursor statement cursor.
      m4return_t GetODBCError (ClCursor *ai_pCursor, m4return_t ai_iErrorCode);

      //## Operation: DeleteConnectionMap%1079428399
      //	Release connection map and disconnet each connection before delete it.
      m4void_t DeleteConnectionMap ();

      //## Operation: DeleteBindColumns%1079428396
      //	Delete result set class member.
      m4return_t DeleteBindColumns ();

      //## Operation: DeleteParameters%1081322133
      m4return_t DeleteParameters ();

    // Data Members for Associations

      //## Association: M4MDTransfer::<unnamed>%40598C2903B3
      //## Role: M4ODBC::m_oConnectionMap%40598C2A01B7
      //## begin M4ODBC::m_oConnectionMap%40598C2A01B7.role preserve=no  protected: connectionMap_t { -> VHAN}
      connectionMap_t m_oConnectionMap;
      //## end M4ODBC::m_oConnectionMap%40598C2A01B7.role

      //## Association: M4MDTransfer::<unnamed>%40614EF102A1
      //## Role: M4ODBC::m_vBindColumns%40614EF202F2
      //## begin M4ODBC::m_vBindColumns%40614EF202F2.role preserve=no  protected: bindColumn_t { -> VHAN}
      bindColumn_t m_vBindColumns;
      //## end M4ODBC::m_vBindColumns%40614EF202F2.role

      //## Association: M4MDTransfer::<unnamed>%4073ABAC03A8
      //## Role: M4ODBC::m_vParameters%4073ABAD0223
      //## begin M4ODBC::m_vParameters%4073ABAD0223.role preserve=no  protected: bindParameter_t { -> VHAN}
      bindParameter_t m_vParameters;
      //## end M4ODBC::m_vParameters%4073ABAD0223.role

      //## Association: M4MDTransfer::<unnamed>%4098C5AD0041
      //## Role: M4ODBC::m_vBindColumnsLen%4098C5AD0203
      //## begin M4ODBC::m_vBindColumnsLen%4098C5AD0203.role preserve=no  protected: bindColumnLength_t { -> VHAN}
      bindColumnLength_t m_vBindColumnsLen;
      //## end M4ODBC::m_vBindColumnsLen%4098C5AD0203.role

    // Additional Protected Declarations
      //## begin M4ODBC%4056C67E001E.protected preserve=yes
      //## end M4ODBC%4056C67E001E.protected

	  bindColumnLength_t m_vParametersLen;

	  m4uint32_t	m_iTraceLevel ;

	  FILE			*m_pfTraceFile ;

  private:
    // Additional Private Declarations
      //## begin M4ODBC%4056C67E001E.private preserve=yes
      //## end M4ODBC%4056C67E001E.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4ODBC%4056C67E001E.implementation preserve=yes
      //## end M4ODBC%4056C67E001E.implementation

};

//## begin M4ODBC%4056C67E001E.postscript preserve=yes
//## end M4ODBC%4056C67E001E.postscript

// Class M4MDTransferConnection 

// Class M4FieldDescription 

// Class M4Row 

// Class M4SorteableRow 

// Class M4ResultSet 

// Class M4ODBC 

//## begin module%3A3E58540028.epilog preserve=yes
//## end module%3A3E58540028.epilog


#endif

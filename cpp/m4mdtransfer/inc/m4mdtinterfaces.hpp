//## begin module%406156EA02A5.cm preserve=no
//## end module%406156EA02A5.cm

//## begin module%406156EA02A5.cp preserve=no
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
//## end module%406156EA02A5.cp

//## Module: m4mdtinterfaces%406156EA02A5; Package specification
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Source file: D:\Work\v600\m4mdtransfer\inc\m4mdtinterfaces.hpp

#ifndef m4mdtinterfaces_h
#define m4mdtinterfaces_h 1

//## begin module%406156EA02A5.additionalIncludes preserve=no
//## end module%406156EA02A5.additionalIncludes

//## begin module%406156EA02A5.includes preserve=yes
#include "m4mdtvalue.hpp"
#include "m4mdtransfer_dll.hpp"
#include "cltypes.hpp"
//## end module%406156EA02A5.includes

// m4types
#include <m4types.hpp>
#include "m4stl.hpp"
//## begin module%406156EA02A5.declarations preserve=no
//## end module%406156EA02A5.declarations

//## begin module%406156EA02A5.additionalDeclarations preserve=yes
//## end module%406156EA02A5.additionalDeclarations


//## begin M4IErrorDevice%4063EBD500AA.preface preserve=yes
//## end M4IErrorDevice%4063EBD500AA.preface

//## Class: M4IErrorDevice%4063EBD500AA
//	This interface provides an error device to store errors.
//## Category: M4MDTransfer%3A3DFB720294
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%406A9AE00202;m4uint32_t { -> }
//## Uses: <unnamed>%406A9AE20350;string { -> }

class M4_DECL_M4MDTRANSFER M4IErrorDevice 
{
  //## begin M4IErrorDevice%4063EBD500AA.initialDeclarations preserve=yes
  //## end M4IErrorDevice%4063EBD500AA.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: DumpError%1080631729
      //	Sets error code, error severity and error parameters (separated by #).
      //
      //	 @param ai_iErrorCode error code.
      //	 @param ai_iErrorSeverity error severity.
      //	 @param ai_sErrorParameters error parameters.
      virtual m4return_t DumpError (m4uint32_t ai_iErrorCode, m4uint32_t ai_iErrorSeverity, const string &ai_sErrorParameters) const = 0;

      //## Operation: GetError%1080631730
      //	Retrieves  error code, error severity and error parameters (separated by #).
      //
      //	 @param ao_iErrorCode error code.
      //	 @param ao_iErrorSeverity error severity.
      //	 @param ao_sErrorParameters error parameters.
      virtual m4return_t GetError (m4uint32_t &ao_iErrorCode, m4uint32_t &ao_iErrorSeverity, string &ao_sErrorParameters) const = 0;

    // Additional Public Declarations
      //## begin M4IErrorDevice%4063EBD500AA.public preserve=yes
      //## end M4IErrorDevice%4063EBD500AA.public

  protected:
    // Additional Protected Declarations
      //## begin M4IErrorDevice%4063EBD500AA.protected preserve=yes
      //## end M4IErrorDevice%4063EBD500AA.protected

  private:
    // Additional Private Declarations
      //## begin M4IErrorDevice%4063EBD500AA.private preserve=yes
      //## end M4IErrorDevice%4063EBD500AA.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4IErrorDevice%4063EBD500AA.implementation preserve=yes
      //## end M4IErrorDevice%4063EBD500AA.implementation

};

//## begin M4IErrorDevice%4063EBD500AA.postscript preserve=yes
//## end M4IErrorDevice%4063EBD500AA.postscript

//## begin M4IReadResultSet%3AE6897601B4.preface preserve=yes
//## end M4IReadResultSet%3AE6897601B4.preface

//## Class: M4IReadResultSet%3AE6897601B4
//	This interface represents the record set returned by a database when a query
//	is executed. It provides method for reading the information.
//## Category: M4MDTransfer%3A3DFB720294
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%406AB7090277;M4IErrorDevice { -> }

class M4_DECL_M4MDTRANSFER M4IReadResultSet 
{
  //## begin M4IReadResultSet%3AE6897601B4.initialDeclarations preserve=yes
  //## end M4IReadResultSet%3AE6897601B4.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GetRowNumber%1079428364
      //	Returns number of rows for result set.
      //
      //	 @return row number.
      virtual m4uint32_t GetRowNumber () const = 0;

      //## Operation: GetValue%1079428366
      //	Returns the field value within a row identified by its column or position.
      //
      //	  @param ai_iRowNumber row number.
      //	  @param ai_iColNumber column number.
      //	  @param ao_oFieldValue field value.
      virtual m4return_t GetValue (m4uint32_t ai_iRowNumber, m4uint32_t ai_iColNumber, ClMDTValue &ao_oFieldValue) const = 0;

      //## Operation: GetFieldDescription%1080551839
      //	Retrieves the field description, identified by its position.
      //
      //	  @param ai_iColNumber field position.
      //	  @param ao_iFieldM4Type field m4type.
      //	  @param ao_iFieldPrecision field precision.
      //	  @param ao_iFieldScale field scale.
      virtual m4return_t GetFieldDescription (m4uint32_t ai_iColNumber, m4uint8_t &ao_iFieldM4Type, m4uint32_t &ao_iFieldPrecision, m4uint32_t &ao_iFieldScale) const = 0;

      //## Operation: GetFieldNumber%1080551840
      //	Returns number of fields.
      //
      //	 @return field number.
      virtual m4uint32_t GetFieldNumber () const = 0;

      //## Operation: Sort%1085386248
      virtual m4return_t Sort (m4comparef_t ai_fFunction, const m4int32_t ai_viOrder[]) = 0;

    // Additional Public Declarations
      //## begin M4IReadResultSet%3AE6897601B4.public preserve=yes
      //## end M4IReadResultSet%3AE6897601B4.public

  protected:
    // Additional Protected Declarations
      //## begin M4IReadResultSet%3AE6897601B4.protected preserve=yes
      //## end M4IReadResultSet%3AE6897601B4.protected

  private:
    // Additional Private Declarations
      //## begin M4IReadResultSet%3AE6897601B4.private preserve=yes
      //## end M4IReadResultSet%3AE6897601B4.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4IReadResultSet%3AE6897601B4.implementation preserve=yes
      //## end M4IReadResultSet%3AE6897601B4.implementation

};

//## begin M4IReadResultSet%3AE6897601B4.postscript preserve=yes
//## end M4IReadResultSet%3AE6897601B4.postscript

//## begin M4IReadRow%4056BE910247.preface preserve=yes
//## end M4IReadRow%4056BE910247.preface

//## Class: M4IReadRow%4056BE910247
//	This interface represents a field row for reading. It provides methods to
//	get the row size and a specific column value within the row.
//## Category: M4MDTransfer%3A3DFB720294
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%406AB72D0047;M4IErrorDevice { -> }

class M4_DECL_M4MDTRANSFER M4IReadRow 
{
  //## begin M4IReadRow%4056BE910247.initialDeclarations preserve=yes
  //## end M4IReadRow%4056BE910247.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GetValue%1079428362
      //	Retrieves the value of a given column.
      //
      //	  @param ai_iColNumber column number.
      //	  @param ao_oColValue column value.
      virtual m4return_t GetValue (m4uint32_t ai_iColNumber, ClMDTValue &ao_oColValue) const = 0;

      //## Operation: GetSize%1079428363
      //	Returns row size.
      //
      //	 @return row size.
      virtual m4uint32_t GetSize () const = 0;

      //## Operation: GetFieldDescription%1080631724
      //	Retrieves field description, identified by its position.
      //
      //	  @param ai_iColNumber field position.
      //	  @param ao_iFieldM4Type field m4type.
      //	  @param ao_iFieldPrecision field precision.
      //	  @param ao_iFieldScale field scale.
      virtual m4return_t GetFieldDescription (m4uint32_t ai_iColNumber, m4uint8_t &ao_iFieldM4Type, m4uint32_t &ao_iFieldPrecision, m4uint32_t &ao_iFieldScale) const = 0;

    // Additional Public Declarations
      //## begin M4IReadRow%4056BE910247.public preserve=yes
      //## end M4IReadRow%4056BE910247.public

  protected:
    // Additional Protected Declarations
      //## begin M4IReadRow%4056BE910247.protected preserve=yes
      //## end M4IReadRow%4056BE910247.protected

  private:
    // Additional Private Declarations
      //## begin M4IReadRow%4056BE910247.private preserve=yes
      //## end M4IReadRow%4056BE910247.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4IReadRow%4056BE910247.implementation preserve=yes
      //## end M4IReadRow%4056BE910247.implementation

};

//## begin M4IReadRow%4056BE910247.postscript preserve=yes
//## end M4IReadRow%4056BE910247.postscript

//## begin M4IWriteResultSet%406145FF0207.preface preserve=yes
//## end M4IWriteResultSet%406145FF0207.preface

//## Class: M4IWriteResultSet%406145FF0207
//	This interface represents the record set returned by a database when a query
//	is executed. It provides method for writing the information.
//## Category: M4MDTransfer%3A3DFB720294
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%406AB70D0196;M4IErrorDevice { -> }

class M4_DECL_M4MDTRANSFER M4IWriteResultSet 
{
  //## begin M4IWriteResultSet%406145FF0207.initialDeclarations preserve=yes
  //## end M4IWriteResultSet%406145FF0207.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: SetValue%1079964155
      //	Sets the field value within a row identified by its position or column.
      //
      //	  @param ai_iRowNumber row number.
      //	  @param ai_iColNumber column number.
      //	  @param ai_oFieldValue field value.
      virtual m4return_t SetValue (m4uint32_t ai_iRowNumber, m4uint32_t ai_iColNumber, const ClMDTValue &ai_oFieldValue) = 0;

      //## Operation: AddRow%1079964158
      //	Adds a new row to the result set.
      //
      //	 @return position of new row, -1 if an error happens.
      virtual m4int32_t AddRow () = 0;

      //## Operation: SetFieldDescription%1080631722
      //	Sets a new field description.
      //
      //	  @param ai_iColNumber field position.
      //	  @param ai_iFieldM4Type m4type.
      //	  @param ai_iFieldPrecision field precision.
      //	  @param ai_iFieldScale field scale.
      virtual m4return_t SetFieldDescription (m4uint32_t ai_iColNumber, m4uint8_t ai_iFieldM4Type, m4uint32_t ai_iFieldPrecision, m4uint32_t ai_iFieldScale) = 0;

    // Additional Public Declarations
      //## begin M4IWriteResultSet%406145FF0207.public preserve=yes
      //## end M4IWriteResultSet%406145FF0207.public

  protected:
    // Additional Protected Declarations
      //## begin M4IWriteResultSet%406145FF0207.protected preserve=yes
      //## end M4IWriteResultSet%406145FF0207.protected

  private:
    // Additional Private Declarations
      //## begin M4IWriteResultSet%406145FF0207.private preserve=yes
      //## end M4IWriteResultSet%406145FF0207.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4IWriteResultSet%406145FF0207.implementation preserve=yes
      //## end M4IWriteResultSet%406145FF0207.implementation

};

//## begin M4IWriteResultSet%406145FF0207.postscript preserve=yes
//## end M4IWriteResultSet%406145FF0207.postscript

//## begin M4IODBC%4056C66D0359.preface preserve=yes
//## end M4IODBC%4056C66D0359.preface

//## Class: M4IODBC%4056C66D0359
//	Interface for database access. It provides methods to initialize and release
//	members and internal parameters, add connections and execute database
//	statements.
//## Category: M4MDTransfer%3A3DFB720294
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%40616BA401C9;M4IWriteResultSet { -> }
//## Uses: <unnamed>%4062F47A010B;M4IReadRow { -> }
//## Uses: <unnamed>%406AA0FD0090;M4IErrorDevice { -> }

class M4_DECL_M4MDTRANSFER M4IODBC 
{
  //## begin M4IODBC%4056C66D0359.initialDeclarations preserve=yes
  //## end M4IODBC%4056C66D0359.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: ExecuteQuery%1079428376
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
      virtual m4return_t ExecuteQuery (const string &ai_sConnectionName, const string &ai_sStatement, const M4IReadRow *ai_poStmtParam,  m4bool_t ai_bIsSelect, M4IWriteResultSet &ao_oResultSet) = 0;

      //## Operation: BeginTransaction%1084786090
      //	Begin transaction form the given connection.
      //
      //	 @param ai_sConnectionName connection name.
      virtual m4return_t BeginTransaction (const string &ai_sConnectionName) = 0;

      //## Operation: Commit%1084786091
      //	Request a commit operation for all active operations on all statements
      //	associated with a connection.
      //
      //	 @param ai_sConnectionName connection name.
      virtual m4return_t Commit (const string &ai_sConnectionName) = 0;

      //## Operation: RollBack%1084786092
      //	Request a rollback operation for all active operations on all statements
      //	associated with a connection.
      //
      //	 @param ai_sConnectionName connection name.
      virtual m4return_t RollBack (const string &ai_sConnectionName) = 0;

    // Additional Public Declarations
      //## begin M4IODBC%4056C66D0359.public preserve=yes
      //## end M4IODBC%4056C66D0359.public

  protected:
    // Additional Protected Declarations
      //## begin M4IODBC%4056C66D0359.protected preserve=yes
      //## end M4IODBC%4056C66D0359.protected

  private:
    // Additional Private Declarations
      //## begin M4IODBC%4056C66D0359.private preserve=yes
      //## end M4IODBC%4056C66D0359.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4IODBC%4056C66D0359.implementation preserve=yes
      //## end M4IODBC%4056C66D0359.implementation

};

//## begin M4IODBC%4056C66D0359.postscript preserve=yes
//## end M4IODBC%4056C66D0359.postscript

// Class M4IErrorDevice 

// Class M4IReadResultSet 

// Class M4IReadRow 

// Class M4IWriteResultSet 

// Class M4IODBC 

//## begin module%406156EA02A5.epilog preserve=yes
//## end module%406156EA02A5.epilog


#endif

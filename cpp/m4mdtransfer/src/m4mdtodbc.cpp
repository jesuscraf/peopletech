//## begin module%3A3E57A701A6.cm preserve=no
//## end module%3A3E57A701A6.cm

//## begin module%3A3E57A701A6.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
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
//## end module%3A3E57A701A6.cp

//## Module: m4mdtodbc%3A3E57A701A6; Package body
//## Subsystem: M4MDTransfer::src%3A3E5D2D0159
//## Source file: C:\m4server\m4mdtransfer\src\m4mdtodbc.cpp

//## begin module%3A3E57A701A6.additionalIncludes preserve=no
//## end module%3A3E57A701A6.additionalIncludes

//## begin module%3A3E57A701A6.includes preserve=yes
#ifdef _WINDOWS
#include <windows.h>
#endif

#include <stdio.h>

// ODBC
#include "sqlext.h"

// EasySP
#include "m4mdtdef.hpp"
#include "m4mdtres.hpp"

// DBWrap.
#include "cursor.hpp"

// Date
#include "cldates.hpp"

#include <sys/stat.h>
//## end module%3A3E57A701A6.includes

// m4mdtinterfaces
#include <m4mdtinterfaces.hpp>
// m4mdtodbc
#include <m4mdtodbc.hpp>
// m4dbtypes
#include <m4dbtypes.hpp>
// m4types
#include <m4types.hpp>
#include "m4unicode.hpp"


//## begin module%3A3E57A701A6.declarations preserve=no
//## end module%3A3E57A701A6.declarations

//## begin module%3A3E57A701A6.additionalDeclarations preserve=yes
//## end module%3A3E57A701A6.additionalDeclarations


// Class M4Row 





M4Row::M4Row ()
  //## begin M4Row::M4Row%1079428372.hasinit preserve=no
  //## end M4Row::M4Row%1079428372.hasinit
  //## begin M4Row::M4Row%1079428372.initialization preserve=yes
  //## end M4Row::M4Row%1079428372.initialization
{
  //## begin M4Row::M4Row%1079428372.body preserve=yes
  //## end M4Row::M4Row%1079428372.body
}


M4Row::~M4Row ()
{
  //## begin M4Row::~M4Row%1079428373.body preserve=yes
	Reset();
  //## end M4Row::~M4Row%1079428373.body
}



//## Other Operations (implementation)
m4return_t M4Row::Init (m4uint32_t ai_iColNumber)
{
  //## begin M4Row::Init%1083569620.body preserve=yes
	// Allocates vector for the given size. Vector size will be changed to allocate the
	// given number of elements.
	m_vFieldValueList.reserve(ai_iColNumber);
	m_vFieldDescriptionList.reserve(ai_iColNumber);
	return M4_SUCCESS;
  //## end M4Row::Init%1083569620.body
}

m4return_t M4Row::Reset ()
{
  //## begin M4Row::Reset%1084174996.body preserve=yes
	// Clear field value list.
	m_vFieldValueList.clear();
	
	// Delete field description list.
	fieldDescription_t::iterator it;
	M4FieldDescription * fieldDesc = NULL;
	it = m_vFieldDescriptionList.begin();
	while (it != m_vFieldDescriptionList.end())
	{
		fieldDesc = *it;
		if (fieldDesc != NULL)
		{
			delete fieldDesc;
		}
		it++;
	}
	m_vFieldDescriptionList.clear();

	return M4_SUCCESS;
  //## end M4Row::Reset%1084174996.body
}

m4return_t M4Row::SetValue (m4uint32_t ai_iColNumber, const ClMDTValue &ai_oColValue)
{
  //## begin M4Row::SetValue%1079428374.body preserve=yes
	// Set field value for the given position.
	m4uint32_t iFieldValueSize = m_vFieldValueList.size();

	if (ai_iColNumber >= iFieldValueSize)
	{
		m_vFieldValueList.resize(ai_iColNumber + 1);
	}
	m_vFieldValueList[ai_iColNumber] = ai_oColValue;

	return M4_SUCCESS;
  //## end M4Row::SetValue%1079428374.body
}

m4return_t M4Row::GetValue (m4uint32_t ai_iColNumber, ClMDTValue &ao_oColValue) const
{
  //## begin M4Row::GetValue%1079428383.body preserve=yes
	m4uint32_t iFieldValueSize = m_vFieldValueList.size();
	M4MDT_RETURN_ERROR2(ai_iColNumber >= iFieldValueSize, M4_ERROR, M4MDTERROR_BAD_COLUMN, ai_iColNumber, iFieldValueSize, "%d%d");
	
	// Get field value.
	ao_oColValue = m_vFieldValueList[ai_iColNumber];
	
	return M4_SUCCESS;
  //## end M4Row::GetValue%1079428383.body
}

m4uint32_t M4Row::GetSize () const
{
  //## begin M4Row::GetSize%1079428384.body preserve=yes
	return m_vFieldValueList.size();
  //## end M4Row::GetSize%1079428384.body
}

m4return_t M4Row::GetFieldDescription (m4uint32_t ai_iColNumber, m4uint8_t &ao_iFieldM4Type, m4uint32_t &ao_iFieldPrecision, m4uint32_t &ao_iFieldScale) const
{
  //## begin M4Row::GetFieldDescription%1080631725.body preserve=yes
	m4uint32_t iFieldDescNum = m_vFieldDescriptionList.size();

	M4MDT_RETURN_ERROR2(ai_iColNumber >= iFieldDescNum, M4_ERROR, M4MDTERROR_BAD_COLUMN, ai_iColNumber, iFieldDescNum, "%d%d");
	
	M4FieldDescription * fieldDesc = m_vFieldDescriptionList[ai_iColNumber];

	M4MDT_RETURN_ERROR1(fieldDesc == NULL, M4_ERROR, M4MDTERROR_NO_FIELD_DESCRIPTION_BY_COLUMN, ai_iColNumber, "%d");
	
	ao_iFieldM4Type = fieldDesc->GetM4Type();
	ao_iFieldPrecision = fieldDesc->GetPrecision();
	ao_iFieldScale = fieldDesc->GetScale();
	
	return M4_SUCCESS;
  //## end M4Row::GetFieldDescription%1080631725.body
}

m4return_t M4Row::SetFieldDescription (m4uint32_t ai_iColNumber, m4uint8_t ai_iM4Type, m4uint32_t ai_iPrecision, m4uint32_t ai_iScale)
{
  //## begin M4Row::SetFieldDescription%1080631726.body preserve=yes
	// Get size.
	m4uint32_t iFieldDescNum = m_vFieldDescriptionList.size();

	// Check there is not a previous element introduced for the same position.
	if (ai_iColNumber < iFieldDescNum)
	{
		M4FieldDescription * pFieldDesc = m_vFieldDescriptionList[ai_iColNumber];
		if (pFieldDesc != NULL)
		{
			delete pFieldDesc;
		}
	}

	// Build field description object.
	M4FieldDescription * fieldDesc = new M4FieldDescription(ai_iM4Type, ai_iPrecision, ai_iScale);
	M4MDT_RETURN_ERROR1(fieldDesc == NULL, M4_ERROR, M4MDTERROR_NO_FIELD_DESCRIPTION_BY_COLUMN, ai_iColNumber, "%d");
	
	if (ai_iColNumber >= iFieldDescNum)
	{
		m_vFieldDescriptionList.resize(ai_iColNumber + 1);
	}
	m_vFieldDescriptionList[ai_iColNumber] = fieldDesc;

	return M4_SUCCESS;
  //## end M4Row::SetFieldDescription%1080631726.body
}

// Additional Declarations
  //## begin M4Row%4056BEA9002F.declarations preserve=yes
  //## end M4Row%4056BEA9002F.declarations

// Class M4ResultSet 




M4ResultSet::M4ResultSet ()
  //## begin M4ResultSet::M4ResultSet%1079428368.hasinit preserve=no
  //## end M4ResultSet::M4ResultSet%1079428368.hasinit
  //## begin M4ResultSet::M4ResultSet%1079428368.initialization preserve=yes
  //## end M4ResultSet::M4ResultSet%1079428368.initialization
{
  //## begin M4ResultSet::M4ResultSet%1079428368.body preserve=yes
	m_vRowList.reserve(M4MDT_VECTOR_DEFAULT_SIZE);
  //## end M4ResultSet::M4ResultSet%1079428368.body
}


M4ResultSet::~M4ResultSet ()
{
  //## begin M4ResultSet::~M4ResultSet%1079428369.body preserve=yes
	// Delete field description map.
	fieldDescriptionMap_t::iterator fieldIt;
	M4FieldDescription * oFieldDesc = NULL;
	fieldIt = m_oFieldDescriptionMap.begin();
	while (fieldIt != m_oFieldDescriptionMap.end())
	{
		oFieldDesc = (*fieldIt).second;
		if (oFieldDesc != NULL)
		{
			delete oFieldDesc;
		}
		fieldIt++;
	}
	m_oFieldDescriptionMap.clear();

	// Clear field rows.
	rowList_t::iterator rowIt;
	M4Row * oRow = NULL;
	rowIt = m_vRowList.begin();
	while (rowIt != m_vRowList.end())
	{
		oRow = *rowIt;
		if (oRow != NULL)
		{
			delete oRow;
		}
		rowIt++;
	}
	m_vRowList.clear();
  //## end M4ResultSet::~M4ResultSet%1079428369.body
}



//## Other Operations (implementation)
m4return_t M4ResultSet::SetValue (m4uint32_t ai_iRowNumber, m4uint32_t ai_iColNumber, const ClMDTValue &ai_oFieldValue)
{
  //## begin M4ResultSet::SetValue%1079428371.body preserve=yes
	m4return_t iRet;
	M4Row * oRow = NULL;
	if (GetRow(ai_iRowNumber, oRow) != M4_SUCCESS)
	{
		iRet = M4_ERROR;
	}
	else
	{
		iRet = oRow->SetValue(ai_iColNumber, ai_oFieldValue);
	}

	return iRet;
  //## end M4ResultSet::SetValue%1079428371.body
}

m4uint32_t M4ResultSet::GetRowNumber () const
{
  //## begin M4ResultSet::GetRowNumber%1079428389.body preserve=yes
	return m_vRowList.size();
  //## end M4ResultSet::GetRowNumber%1079428389.body
}

m4return_t M4ResultSet::GetValue (m4uint32_t ai_iRowNumber, m4uint32_t ai_iColNumber, ClMDTValue &ao_oFieldValue) const
{
  //## begin M4ResultSet::GetValue%1079428391.body preserve=yes
	m4return_t iRet;
	M4Row * oRow = NULL;
	if (GetRow(ai_iRowNumber, oRow) != M4_SUCCESS)
	{
		iRet = M4_ERROR;
	}
	else
	{
		iRet = oRow->GetValue(ai_iColNumber, ao_oFieldValue);
	}
	return iRet;
  //## end M4ResultSet::GetValue%1079428391.body
}

m4int32_t M4ResultSet::AddRow ()
{
  //## begin M4ResultSet::AddRow%1079428392.body preserve=yes
	// Number of fields of new row.
	m4uint32_t iColNumber = m_oFieldDescriptionMap.size();
	M4Row * oRow = new M4Row();
	M4MDT_RETURN_ERROR1(oRow == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof(M4Row), "%d");

	// If there is an error in initialization, return -1.
	m4int32_t iRowPos = -1;
	m4return_t iErrorCode = oRow->Init(iColNumber);
	M4MDT_RETURN_ERROR1(iErrorCode != M4_SUCCESS, iRowPos, M4MDTERROR_INIT_ROW, iColNumber, "%d");
	
	m_vRowList.push_back(oRow);

	iRowPos = m_vRowList.size() - 1;
	return iRowPos;
  //## end M4ResultSet::AddRow%1079428392.body
}

m4return_t M4ResultSet::GetRow (m4uint32_t ai_iRowNumber, M4Row *&ao_oRow) const
{
  //## begin M4ResultSet::GetRow%1079964159.body preserve=yes
	ao_oRow = NULL;
	m4uint32_t iRowNum = m_vRowList.size();

	M4MDT_RETURN_ERROR2(ai_iRowNumber >= iRowNum, M4_ERROR, M4MDTERROR_BAD_ROW, ai_iRowNumber, iRowNum, "%d%d");
	
	ao_oRow = m_vRowList[ai_iRowNumber];
	
	return M4_SUCCESS;
  //## end M4ResultSet::GetRow%1079964159.body
}

m4return_t M4ResultSet::GetFieldDescription (m4uint32_t ai_iColNumber, m4uint8_t &ao_iFieldM4Type, m4uint32_t &ao_iFieldPrecision, m4uint32_t &ao_iFieldScale) const
{
  //## begin M4ResultSet::GetFieldDescription%1080551834.body preserve=yes
	// Look for the field description by its position.
	fieldDescriptionMap_t::const_iterator it;
	it = m_oFieldDescriptionMap.find(ai_iColNumber);
	M4MDT_RETURN_ERROR1(it == m_oFieldDescriptionMap.end(), M4_ERROR, M4MDTERROR_NO_FIELD_DESCRIPTION_BY_COLUMN, ai_iColNumber, "%d");
	
	M4FieldDescription * fieldDesc = (*it).second;
	M4MDT_RETURN_ERROR1(fieldDesc == NULL, M4_ERROR, M4MDTERROR_NO_FIELD_DESCRIPTION_BY_COLUMN, ai_iColNumber, "%d");

	ao_iFieldM4Type = fieldDesc->GetM4Type();
	ao_iFieldPrecision = fieldDesc->GetPrecision();
	ao_iFieldScale = fieldDesc->GetScale();

	return M4_SUCCESS;
  //## end M4ResultSet::GetFieldDescription%1080551834.body
}

m4return_t M4ResultSet::SetFieldDescription (m4uint32_t ai_iColNumber, m4uint8_t ai_iFieldM4Type, m4uint32_t ai_iFieldPrecision, m4uint32_t ai_iFieldScale)
{
  //## begin M4ResultSet::SetFieldDescription%1080631723.body preserve=yes
	// Check there is not a previous element introduced for the same position.
	fieldDescriptionMap_t::iterator it;
	it = m_oFieldDescriptionMap.find(ai_iColNumber);
	if (it != m_oFieldDescriptionMap.end())
	{
		M4FieldDescription * pFieldDesc = (*it).second;
		if (pFieldDesc != NULL)
		{
			delete pFieldDesc;
		}
		m_oFieldDescriptionMap.erase(it);
	}
	
	// Create field description.
	M4FieldDescription * pFieldDesc = new M4FieldDescription(ai_iFieldM4Type, ai_iFieldPrecision, ai_iFieldScale);
	M4MDT_RETURN_ERROR1(pFieldDesc == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof(M4FieldDescription), "%d");
	
	// Insert field description for the given position.
	pair<fieldDescriptionMap_t::iterator, bool> mapRes;
	fieldDescriptionMap_t::value_type mapEntry(ai_iColNumber, pFieldDesc);
	mapRes = m_oFieldDescriptionMap.insert(mapEntry);
	M4MDT_RETURN_ERROR1(mapRes.second == M4_FALSE, M4_ERROR, M4MDTERROR_ADD_FIELD_DESCRIPTION, ai_iColNumber, "%d");
	
	return M4_SUCCESS;
  //## end M4ResultSet::SetFieldDescription%1080631723.body
}

m4uint32_t M4ResultSet::GetFieldNumber () const
{
  //## begin M4ResultSet::GetFieldNumber%1080551842.body preserve=yes
	return m_oFieldDescriptionMap.size();
  //## end M4ResultSet::GetFieldNumber%1080551842.body
}

m4return_t M4ResultSet::Sort (m4comparef_t ai_fFunction, const m4int32_t ai_viOrder[])
{
  //## begin M4ResultSet::Sort%1085386249.body preserve=yes
	m4uint32_t	i = 0 ;
	m4uint32_t	iSize = 0 ;
	M4SorteableRow	**apoRow = NULL ;


	iSize = m_vRowList.size() ;

	apoRow = new M4SorteableRow*[ iSize ] ;
	M4MDT_RETURN_ERROR1( apoRow == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof( M4Row** ) * iSize, "%d" ) ;

	for( i = 0 ; i < iSize ; i++ )
	{
		apoRow[ i ] = new M4SorteableRow(ai_viOrder,m_vRowList[ i ]) ;
	}

    qsort( apoRow, size_t( iSize ), sizeof( M4SorteableRow* ), ai_fFunction ) ;

	for( i = 0 ; i < iSize ; i++ )
	{
		m_vRowList[ i ] = apoRow[ i ]->m_poRow ;
	}

	delete [] apoRow ;

	return( M4_SUCCESS ) ;
  //## end M4ResultSet::Sort%1085386249.body
}

// Additional Declarations
  //## begin M4ResultSet%3A3DFC4E01D2.declarations preserve=yes
  //## end M4ResultSet%3A3DFC4E01D2.declarations

// Class M4ODBC 





M4ODBC::M4ODBC ()
  //## begin M4ODBC::M4ODBC%1079428379.hasinit preserve=no
  //## end M4ODBC::M4ODBC%1079428379.hasinit
  //## begin M4ODBC::M4ODBC%1079428379.initialization preserve=yes
  //## end M4ODBC::M4ODBC%1079428379.initialization
{
  //## begin M4ODBC::M4ODBC%1079428379.body preserve=yes
	m_iTraceLevel = 0 ;
	m_pfTraceFile = NULL ;

	m_vBindColumns.reserve(M4MDT_VECTOR_DEFAULT_SIZE);
	m_vBindColumnsLen.reserve(M4MDT_VECTOR_DEFAULT_SIZE);
	m_vParameters.reserve(M4MDT_VECTOR_DEFAULT_SIZE);
	m_vParametersLen.reserve(M4MDT_VECTOR_DEFAULT_SIZE);
  //## end M4ODBC::M4ODBC%1079428379.body
}


M4ODBC::~M4ODBC ()
{
  //## begin M4ODBC::~M4ODBC%1079428380.body preserve=yes
	// Delete connection map.
	DeleteConnectionMap();

	// Delete result set vector.
	DeleteBindColumns();

	// Delete bind parameter list.
	DeleteParameters();

	if( m_pfTraceFile != NULL )
	{
		fclose( m_pfTraceFile ) ;
		m_pfTraceFile = NULL ;
	}

  //## end M4ODBC::~M4ODBC%1079428380.body
}



//## Other Operations (implementation)
m4return_t M4ODBC::Init (m4uint32_t ai_iTraceLevel, m4pcchar_t ai_pccFileDir)
{
  //## begin M4ODBC::Init%1079428385.body preserve=yes

	int			iWrite = 0 ;
	int			iFlush = 0 ;
	m4pcchar_t	pccFileName = NULL ;
	m4char_t	acDate[ M4_SECOND_END + 1 ] ;
	string		sFileName ;


	if( ai_iTraceLevel > 0 )
	{
		m_iTraceLevel = ai_iTraceLevel ;

		if( m_pfTraceFile != NULL )
		{
			fclose( m_pfTraceFile ) ;
			m_pfTraceFile = NULL ;
		}

		if( ai_pccFileDir != NULL )
		{
			sFileName.append( ai_pccFileDir ) ;
			sFileName.append( M4_BACK_SLASH ) ;
		}
		sFileName.append( M4MDT_ODBC_TRACE_FILE_NAME ) ;
		pccFileName = sFileName.c_str() ;

		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniNative ;

		m_pfTraceFile = M4Fopen( pccFileName, M4UniAppendBinary, eEncoding ) ;
		M4MDT_RETURN_ERROR2( m_pfTraceFile == NULL, M4_ERROR, M4MDTERROR_NO_OPEN_FILE, pccFileName, strerror( errno ), "%s#%s" ) ;

		ClActualDate( acDate, M4_SECOND_END + 1 ) ;

		iWrite = fprintf( m_pfTraceFile, "\n-------------- %s %s --------------\n\n", M4MDT_ODBC_TRACE_FILE_NAME, acDate ) ;
		M4MDT_RETURN_ERROR2( iWrite < 0, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, pccFileName, strerror( errno ), "%s#%s" ) ;

		iFlush = fflush( m_pfTraceFile ) ;
		M4MDT_RETURN_ERROR2( iFlush == EOF, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, pccFileName, strerror( errno ), "%s#%s" ) ;
	}

	return M4_SUCCESS;
  //## end M4ODBC::Init%1079428385.body
}

m4return_t M4ODBC::AddConnection (const string &ai_sConnectionName, const string &ai_sConnectionString, const eDDBBType_t &ai_eDBType)
{
  //## begin M4ODBC::AddConnection%1079428388.body preserve=yes
	// Create connection properties object.
	M4MDTransferConnection * pConnectionInfo = new M4MDTransferConnection(ai_sConnectionName, ai_sConnectionString, ai_eDBType);
	if (pConnectionInfo == NULL)
	{
		M4MDT_DUMP_ERROR1(M4MDTERROR_NO_MEMORY, sizeof(M4MDTransferConnection), "%d");
		return M4_ERROR;
	}

	// Init connection.
	m4return_t iErrorCode = pConnectionInfo->Init();
	if (iErrorCode != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR3(M4MDTERROR_INTERNAL_ERROR, "M4ODBC::AddConnection", "pConnectionInfo->Init()", ai_sConnectionName.c_str(), "%s#%s#%s");
		delete pConnectionInfo;
		return M4_ERROR;
	}

	// Insert connection and connection properties.
	pair<connectionMap_t::iterator, bool> mapRes;
	connectionMap_t::value_type mapEntry(ai_sConnectionName, pConnectionInfo);
	mapRes = m_oConnectionMap.insert(mapEntry);

	if (mapRes.second == M4_FALSE)
	{
		M4MDT_DUMP_ERROR1(M4MDTERROR_ADD_CONNECTION, ai_sConnectionName.c_str(), "%s");
		delete pConnectionInfo;
		return M4_ERROR;
	}

	return M4_SUCCESS;
  //## end M4ODBC::AddConnection%1079428388.body
}

m4return_t M4ODBC::GetConnection (const string &ai_sConnectionName, ClDBConnection *&ao_oConnection)
{
  //## begin M4ODBC::GetConnection%1079428381.body preserve=yes
	ao_oConnection = NULL;
	connectionMap_t::iterator it;
	it = m_oConnectionMap.find(ai_sConnectionName);

	M4MDT_RETURN_ERROR1(it == m_oConnectionMap.end(), M4_ERROR, M4MDTERROR_NO_CONNECTION, ai_sConnectionName.c_str(), "%s");

	// Get connecion info.
	M4MDTransferConnection * pConnection = (*it).second;
	
	M4MDT_RETURN_ERROR1(pConnection == NULL, M4_ERROR, M4MDTERROR_NO_CONNECTION, ai_sConnectionName.c_str(), "%s");

	ao_oConnection = pConnection->GetDBConnection();
	
	return M4_SUCCESS;
  //## end M4ODBC::GetConnection%1079428381.body
}

m4return_t M4ODBC::GetDDBBType (const string &ai_sConnectionName, eDDBBType_t &ao_eDDBBType)
{
  //## begin M4ODBC::GetDDBBType%1079428397.body preserve=yes
	connectionMap_t::iterator it;
	it = m_oConnectionMap.find(ai_sConnectionName);

	M4MDT_RETURN_ERROR1(it == m_oConnectionMap.end(), M4_ERROR, M4MDTERROR_NO_CONNECTION, ai_sConnectionName.c_str(), "%s");

	// Get connection info.
	M4MDTransferConnection * pConnection = (*it).second;

	M4MDT_RETURN_ERROR1(pConnection == NULL, M4_ERROR, M4MDTERROR_NO_CONNECTION, ai_sConnectionName.c_str(), "%s");

	ao_eDDBBType = pConnection->GetDDBBType();
	
	return M4_SUCCESS;
  //## end M4ODBC::GetDDBBType%1079428397.body
}

m4return_t M4ODBC::ExecuteQuery (const string &ai_sConnectionName, const string &ai_sStatement, const M4IReadRow *ai_poStmtParam,  m4bool_t ai_bIsSelect, M4IWriteResultSet &ao_oResultSet)
{
  //## begin M4ODBC::ExecuteQuery%1079428386.body preserve=yes
	// Get connection from connection map.

	size_t		iSize = 0 ;
	int			iWrite = 0 ;
	int			iFlush = 0 ;
	m4pcchar_t	pccValue = NULL ;
	m4char_t	acDate[ M4_SECOND_END + 1 ] ;


	if( m_pfTraceFile != NULL && ( m_iTraceLevel & M4MDT_TRACE_SENTENCE ) != 0 )
	{
		ClActualDate( acDate, M4_SECOND_END + 1 ) ;

		iWrite = fprintf( m_pfTraceFile, "Date = %s\tConnection = %s\n%s ", acDate, ai_sConnectionName.c_str(), ai_sStatement.c_str() ) ;
		M4MDT_RETURN_ERROR2( iWrite < 0, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, M4MDT_ODBC_TRACE_FILE_NAME, strerror( errno ), "%s#%s" ) ;
	}


	ClDBConnection * pConnection = NULL;
	if (GetConnection(ai_sConnectionName, pConnection) != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	// Get connection properties.
	eDDBBType_t eDDBBType;
	if (GetDDBBType(ai_sConnectionName, eDDBBType) != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	// Create cursor.
	ClCursor * pCursor = new ClCursor();
	M4MDT_RETURN_ERROR1(pCursor == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof(ClCursor), "%d");
	
	// Open cursor.
	m4return_t iErrorCode;
	iErrorCode = pCursor->Open();
	if (iErrorCode != M4_SUCCESS)
	{
		GetODBCError(pCursor, iErrorCode);
		M4MDT_DUMP_ERROR1(M4MDTERROR_OPEN_CURSOR, ai_sConnectionName.c_str(), "%s");
		delete pCursor;
		return M4_ERROR;
	}

	// Prepare the statement.
	iErrorCode = pCursor->Prepare((m4pchar_t) ai_sStatement.c_str(), eDDBBType);
	if (iErrorCode != M4_SUCCESS)
	{
		GetODBCError(pCursor, iErrorCode);
		M4MDT_DUMP_ERROR2(M4MDTERROR_PREPARE_STATEMENT, ai_sConnectionName.c_str(), ai_sStatement.c_str(), "%s#%s");
		delete pCursor;
		return M4_ERROR;
	}

	// Get number of parameters.
	m4uint32_t iParamNum = 0;
	if (ai_poStmtParam != NULL)
	{
		iParamNum = ai_poStmtParam->GetSize();
	}

	// Bind parameters.
	ClMDTValue oParamValue;
	m4double_t dParamValue = 0.0;
	m4date_t dtValue;
	m4uint8_t iM4Type = 0;
	m4uint32_t iPrecision = 0;
	m4uint32_t iScale = 0;
	m4int32_t * pParamLen = NULL;
    
	for (m4uint32_t iParam = 0; iParam < iParamNum; iParam++)
	{
		// Get param value (field value).
		if (ai_poStmtParam->GetValue(iParam, oParamValue) != M4_SUCCESS)
		{
			M4MDT_DUMP_ERROR2(M4MDTERROR_INTERNAL_ERROR, "M4ODBC::ExecuteQuery", iParam, "%s#GetValue = <%d>");
			DeleteParameters();
			delete pCursor;
			return M4_ERROR;
		}


		if( m_pfTraceFile != NULL && ( m_iTraceLevel & M4MDT_TRACE_SENTENCE ) != 0 )
		{

			if( oParamValue.IsNull() == M4_TRUE )
			{
				pccValue = "<NULL>" ;
				iSize = 6 ;
			}
			else
			{
				pccValue = oParamValue.c_str() ;
				iSize = oParamValue.size() ;
			}

			iWrite = fwrite( "[", 1, 1, m_pfTraceFile ) ;
			M4MDT_RETURN_ERROR2( iWrite != 1, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, M4MDT_ODBC_TRACE_FILE_NAME, strerror( errno ), "%s#%s" ) ;
			iWrite = fwrite( pccValue, 1, iSize, m_pfTraceFile ) ;
			M4MDT_RETURN_ERROR2( iWrite != iSize, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, M4MDT_ODBC_TRACE_FILE_NAME, strerror( errno ), "%s#%s" ) ;
			iWrite = fwrite( "]", 1, 1, m_pfTraceFile ) ;
			M4MDT_RETURN_ERROR2( iWrite != 1, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, M4MDT_ODBC_TRACE_FILE_NAME, strerror( errno ), "%s#%s" ) ;
		}


		// Get field description.
		if(ai_poStmtParam->GetFieldDescription(iParam, iM4Type, iPrecision, iScale) != M4_SUCCESS)
		{
			M4MDT_DUMP_ERROR2(M4MDTERROR_INTERNAL_ERROR, "M4ODBC::ExecuteQuery", iParam, "%s#GetFieldDescription = <%d>");
			DeleteParameters();
			delete pCursor;
			return M4_ERROR;
		}

		// Check if param value is null. In this case, SQL_NULL_DATA value will be sent.
		m4pchar_t pParamValue = NULL;
		m4pchar_t pParamNullValue = NULL; // Null pointer to bind NULL parameters.
		m4uint32_t iLen = 0;

		// Bug 161822. Falla la inserción de un campo tipo VARCHAR en ACCESS si la precisión
		// es superior a 255.
		// Si se da la circunstancia indicada anteriormente, cambiamos a tipo M4CL_M4_TYPE_LONG
		// y así el Access admite el valor para el campo Text
		if( eDDBBType == M4DBW_ACCESS_DDBB && iPrecision > M4MDT_MAX_SIZE_VARCHAR_ACCESS )
		{
			if(     iM4Type == M4CL_M4_TYPE_FIXED_STRING
				||	iM4Type == M4CL_M4_TYPE_VAR_STRING
				||	iM4Type == M4CL_M4_TYPE_VARIANT
			)
			{
				iM4Type = M4CL_M4_TYPE_LONG;
			}

			if(     iM4Type == M4CL_M4_TYPE_UNI_VAR_STRING
				||	iM4Type == M4CL_M4_TYPE_UNI_VARIANT
			)
			{
				iM4Type = M4CL_M4_TYPE_UNI_LONG;
			}
		}

		if (oParamValue.IsNull() == M4_TRUE)
		{
			// If we are executing a select statement, all parameters should be not null.
			if (ai_bIsSelect == M4_TRUE)
			{
				M4MDT_DUMP_ERROR2(M4MDTERROR_INTERNAL_ERROR, "M4ODBC::ExecuteQuery", iParam, "%s#Null Parameter Value in Select Statement = <%d>");
				DeleteParameters();
				delete pCursor;
				return M4_ERROR;
			}

			// Bug 0151532: Longitud del parámetro a bindar.
			pParamLen = new m4int32_t;
			M4MDT_RETURN_ERROR1(pParamLen == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof(m4int32_t), "%d");

			// Bind NULL parameter.
			*pParamLen = SQL_NULL_DATA;
			m_vParameters.push_back(pParamNullValue);
			m_vParametersLen.push_back(pParamLen);

			if (   iM4Type == M4CL_M4_TYPE_LONG
				|| iM4Type == M4CL_M4_TYPE_UNI_LONG
				|| iM4Type == M4CL_M4_TYPE_BINARY_STRING
				|| iM4Type == M4CL_M4_TYPE_BLOB)
			{
				// Bug 0128294. Los parámetros LONG se bindan ahora de forma diferente. Hay que
				// pasar la dirección al parámetro que se quiere bindar.
				iErrorCode = pCursor->BindParam(iParam + 1, NULL, &m_vParameters[iParam], iPrecision, iM4Type, iScale, m_vParametersLen[iParam]);
			}
			else
			{
				iErrorCode = pCursor->BindParam(iParam + 1, m_vParameters[iParam], NULL, iPrecision, iM4Type, iScale, m_vParametersLen[iParam]);
			}
			M4MDT_RETURN_ERROR5(iErrorCode != M4_SUCCESS, M4_ERROR, M4MDTERROR_BIND_PARAM, iParam + 1, oParamValue.c_str(), iM4Type, iPrecision, iScale, "%s%d%s%d%d%d");
		}
		else
		{
			m4int32_t iMaxLenLongField;
			m4int32_t iMaxLenBlobField;
			
			// Bug 0151532: Longitud del parámetro a bindar.
			pParamLen = new m4int32_t;
			M4MDT_RETURN_ERROR1(pParamLen == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof(m4int32_t), "%d");

			switch (iM4Type)
			{
			case M4CL_M4_TYPE_FIXED_STRING:
			case M4CL_M4_TYPE_VAR_STRING:
			case M4CL_M4_TYPE_UNI_VAR_STRING:
			case M4CL_M4_TYPE_VARIANT:
			case M4CL_M4_TYPE_UNI_VARIANT:

				iLen = oParamValue.size() + 1 ;
				pParamValue = new m4char_t[iLen];
				M4MDT_RETURN_ERROR1(pParamValue == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof(m4char_t) * iLen, "%d");
				memcpy(pParamValue, oParamValue.c_str(), iLen);
				*pParamLen = SQL_NTS;
				break;

			case M4CL_M4_TYPE_NUMBER:
			case M4CL_M4_TYPE_CURRENCY:
			case M4CL_M4_TYPE_NUM_VARIANT:

				dParamValue = atof(oParamValue.c_str());
				pParamValue = (m4pchar_t) new m4double_t;
				M4MDT_RETURN_ERROR1(pParamValue == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof(m4double_t), "%d");
				*((m4double_t*) pParamValue) = dParamValue;
				*pParamLen = SQL_NTS;
				break;

			case M4CL_M4_TYPE_TIMESTAMP:
			case M4CL_M4_TYPE_DATE:
			case M4CL_M4_TYPE_HOUR:

				iPrecision = M4MDT_SIZE_TIMESTAMP;
				ClStringToDate(oParamValue.c_str(), dtValue);
				pParamValue = (m4pchar_t) new m4date_t;
				M4MDT_RETURN_ERROR1(pParamValue == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof(m4date_t), "%d");
				*((m4date_t*) pParamValue) = dtValue;
				*pParamLen = SQL_NTS;
				break;

			case M4CL_M4_TYPE_LONG:
			case M4CL_M4_TYPE_UNI_LONG:
			case M4CL_M4_TYPE_BINARY_STRING:
			
				iLen = oParamValue.size() ;
				iMaxLenLongField = iLen;
				iPrecision = M4MDT_SIZE_LONG;
				
				pParamValue = new m4char_t[iLen];
				M4MDT_RETURN_ERROR1(pParamValue == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof(m4char_t) * iLen, "%d");
				memcpy(pParamValue, oParamValue.c_str(), iLen);
				
				*pParamLen = iMaxLenLongField;
				break;

			case M4CL_M4_TYPE_BLOB:

				iPrecision = M4MDT_SIZE_LONG;

				// Bind path to blob temp. file.
				iLen = oParamValue.size() + 1;
				pParamValue = new m4char_t[iLen];
				M4MDT_RETURN_ERROR1(pParamValue == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof(m4char_t) * iLen, "%d");
				memcpy(pParamValue, oParamValue.c_str(), iLen);
				
				// Calculate blob length.
				iErrorCode = GetMaxBlobLength(pParamValue, iMaxLenBlobField);
				M4MDT_RETURN_ERROR2(iErrorCode != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "M4ODBC::ExecuteQuery", "GetMaxBlobLength", "%s#%s");
				*pParamLen = iMaxLenBlobField;
				break;

			default:
				M4MDT_DUMP_ERROR2(M4MDTERROR_NO_SUCH_PARAM_TYPE, iParam, iM4Type, "%d#%d");
				return M4_ERROR;
			}

			m_vParameters.push_back(pParamValue);
			m_vParametersLen.push_back(pParamLen);

			if (   iM4Type == M4CL_M4_TYPE_LONG
				|| iM4Type == M4CL_M4_TYPE_UNI_LONG
				|| iM4Type == M4CL_M4_TYPE_BINARY_STRING
				|| iM4Type == M4CL_M4_TYPE_BLOB)
			{
				// Bug 0128294. Los parámetros LONG se bindan ahora de forma diferente. Hay que
				// pasar la dirección al parámetro que se quiere bindar.
				iErrorCode = pCursor->BindParam(iParam + 1, NULL, &m_vParameters[iParam], iPrecision, iM4Type, iScale, m_vParametersLen[iParam]);
			}
			else
			{
				iErrorCode = pCursor->BindParam(iParam + 1, m_vParameters[iParam], NULL, iPrecision, iM4Type, iScale, m_vParametersLen[iParam]);
			}
			M4MDT_RETURN_ERROR5(iErrorCode != M4_SUCCESS, M4_ERROR, M4MDTERROR_BIND_PARAM, iParam + 1, oParamValue.c_str(), iM4Type, iPrecision, iScale, "%s%d%s%d%d%d");
		}	
	}


	if( m_pfTraceFile != NULL && ( m_iTraceLevel & M4MDT_TRACE_SENTENCE ) != 0 )
	{
		iWrite = fprintf( m_pfTraceFile, "\n" ) ;
		M4MDT_RETURN_ERROR2( iWrite < 0, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, M4MDT_ODBC_TRACE_FILE_NAME, strerror( errno ), "%s#%s" ) ;

		iFlush = fflush( m_pfTraceFile ) ;
		M4MDT_RETURN_ERROR2( iFlush == EOF, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, M4MDT_ODBC_TRACE_FILE_NAME, strerror( errno ), "%s#%s" ) ;
	}

	
	// Associates database connection and cursor.
	m4bool_t bReallyInitialized = M4_FALSE;
	iErrorCode = pCursor->InitCursor(pConnection, bReallyInitialized);
	if (iErrorCode != M4_SUCCESS)
	{
		GetODBCError(pCursor, iErrorCode);
		M4MDT_DUMP_ERROR1(M4MDTERROR_INIT_CURSOR, ai_sConnectionName.c_str(), "%s");
		
		DeleteParameters();
		delete pCursor;
		return M4_ERROR;
	}
	
  	// Execute statement.
	m4return_t iCanceled = M4_SUCCESS;
	iErrorCode = pCursor->Execute(0, NULL, iCanceled);
	if (iErrorCode != M4_SUCCESS)
	{
		GetODBCError(pCursor, iErrorCode);
		M4MDT_DUMP_ERROR2(M4MDTERROR_EXECUTE_STATEMENT, ai_sStatement.c_str(), ai_sConnectionName.c_str(), "%s#%s");
	
		DeleteParameters();
		DeleteBindColumns();
		delete pCursor;		
		return M4_ERROR;
	}

	// Get number of colums to be retrieved from the statement. This is only available
	// for select statements. For update, insert or delete statements, columns is 0.
	m4int32_t iNumCol = 0;
	if (ai_bIsSelect == M4_TRUE)
	{
		iNumCol = (m4int32_t) pCursor->GetNumCol();
	}

	// Bind columns.
	m4int32_t iLength = 0;
	m4uint32_t iFieldScale = 0;
	vector <m4uint32_t> vColDesc;
	for (m4int8_t iCol = 0; iCol < iNumCol; iCol++)
	{
		// Get column description.
		m4int32_t iPrec = 0;
		m4int8_t iM4Type = 0;
		iErrorCode = pCursor->DescribeCol (iCol + 1, &iPrec, &iM4Type);
		if (iErrorCode != M4_SUCCESS)
		{
			GetODBCError(pCursor, iErrorCode);
			M4MDT_DUMP_ERROR1(M4MDTERROR_CLOSE_CURSOR, ai_sConnectionName.c_str(), "%s");
			
			DeleteParameters();
			DeleteBindColumns();
			delete pCursor;
			return M4_ERROR;
		}
		vColDesc.push_back(iM4Type);

		// Set field description by position.
		if (ao_oResultSet.SetFieldDescription(iCol, iM4Type, iPrec, iFieldScale) != M4_SUCCESS)
		{
			M4MDT_DUMP_ERROR2(M4MDTERROR_INTERNAL_ERROR, "M4ODBC::ExecuteQuery", iCol, "%s#SetFieldDescription = <%d>");

			DeleteParameters();
			DeleteBindColumns();
			delete pCursor;
			return M4_ERROR;
		}

		// Bind columns. For long fields is not necessary to reserve memory.
		m4char_t * pFieldValue = NULL;
		m4int32_t * pFieldLen = new m4int32_t;
		M4MDT_RETURN_ERROR1(pFieldLen == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof(m4int32_t), "%d");

		switch (iM4Type)
		{
		case M4CL_M4_TYPE_FIXED_STRING:
		case M4CL_M4_TYPE_VAR_STRING:
		case M4CL_M4_TYPE_UNI_VAR_STRING:
		case M4CL_M4_TYPE_VARIANT:
		case M4CL_M4_TYPE_UNI_VARIANT:

			pFieldValue = new m4char_t[iPrec + 1];
			M4MDT_RETURN_ERROR1(pFieldValue == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof(m4char_t) * (iPrec + 1), "%d");
			break;

		case M4CL_M4_TYPE_NUMBER:
		case M4CL_M4_TYPE_CURRENCY:
		case M4CL_M4_TYPE_NUM_VARIANT:

			iPrec = M4MDT_SIZE_DOUBLE;
			pFieldValue = new m4char_t[M4MDT_DEFAULT_ARRAY_SIZE + 1];
			M4MDT_RETURN_ERROR1(pFieldValue == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof(m4char_t) * (M4MDT_DEFAULT_ARRAY_SIZE + 1), "%d");
			break;

		case M4CL_M4_TYPE_TIMESTAMP:
		case M4CL_M4_TYPE_DATE:
		case M4CL_M4_TYPE_HOUR:

			iPrec = M4MDT_SIZE_TIMESTAMP;
			pFieldValue = new m4char_t[M4MDT_SIZE_TIMESTAMP + 1];
			M4MDT_RETURN_ERROR1(pFieldValue == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof(m4char_t) * (M4MDT_SIZE_TIMESTAMP + 1), "%d");
			break;

		/* Bug 0125267
		Ahora puede venir binario
		*/
		case M4CL_M4_TYPE_LONG:
		case M4CL_M4_TYPE_UNI_LONG:
		case M4CL_M4_TYPE_BINARY_STRING:
		case M4CL_M4_TYPE_BLOB:

			pFieldValue = NULL;
			break;

		default:
			M4MDT_DUMP_ERROR2(M4MDTERROR_NO_SUCH_PARAM_TYPE, iCol, iM4Type, "%d#%d");
			return M4_ERROR;
		}
		
		StData *pData = new StData( pFieldValue, iPrec ) ;
		m_vBindColumns.push_back(pData);
		m_vBindColumnsLen.push_back(pFieldLen);

		iErrorCode = pCursor->BindResult(iCol + 1, &( m_vBindColumns[iCol]->m_pcValue ), &( m_vBindColumns[iCol]->m_iPrec ), iM4Type, m_vBindColumnsLen[iCol], 0);
		if (iErrorCode != M4_SUCCESS)
		{
			GetODBCError(pCursor, iErrorCode);
			M4MDT_DUMP_ERROR4(M4MDTERROR_BIND_COLUMN, ai_sConnectionName.c_str(), iCol + 1, iM4Type, iPrec, "%s%d%d%d");
			
			DeleteParameters();
			DeleteBindColumns();
			delete pCursor;
			return M4_ERROR;
		}
	}

	// Get data. Just, get data if there are columns to retrieved. This only happens for 
	// select statements.
	if (iNumCol > 0)
	{
		if (GetData(pCursor, ai_sConnectionName, vColDesc, ao_oResultSet) != M4_SUCCESS)
		{
			M4MDT_DUMP_ERROR2(M4MDTERROR_INTERNAL_ERROR, "M4ODBC::ExecuteQuery", ai_sConnectionName.c_str(), "%s#GetData = <%s>");
			
			DeleteParameters();
			DeleteBindColumns();
			delete pCursor;
			return M4_ERROR;
		}
	}

	// Close cursor.
	iErrorCode = pCursor->Close();
	if (iErrorCode != M4_SUCCESS)
	{
		GetODBCError(pCursor, iErrorCode);
		M4MDT_DUMP_ERROR1(M4MDTERROR_CLOSE_CURSOR, ai_sConnectionName.c_str(), "%s");

		DeleteParameters();
		DeleteBindColumns();
		delete pCursor;
		return M4_ERROR;
	}

	DeleteParameters();
	DeleteBindColumns();
	delete pCursor;

	return M4_SUCCESS;
  //## end M4ODBC::ExecuteQuery%1079428386.body
}

m4return_t M4ODBC::GetMaxBlobLength (m4pcchar_t ai_pcBlobParamValue, m4int32_t &ao_iMaxLenBlobField)
{
  //## begin M4ODBC::GetMaxBlobLength%1089121924.body preserve=yes
	ao_iMaxLenBlobField = 0;
	struct stat stFile;

	// UNICODE FILE
	int iLength = -1 ;
	char* pcFileName = M4CppToANSI( ai_pcBlobParamValue, iLength ) ;
	int ret = stat(pcFileName, &stFile) ;
	delete [] pcFileName ;

	if (ret == 0)
	{
		ao_iMaxLenBlobField = stFile.st_size;
		if (ao_iMaxLenBlobField != 0)
		{
			m4int32_t lFile = strlen(ai_pcBlobParamValue);
			m4int32_t lPos = 0;
			for (lPos = lFile; lPos; lPos--)
			{
				if (ai_pcBlobParamValue[lPos] == '.')
				{
					break;
				}
			}
			
			if (lPos)
			{
				ao_iMaxLenBlobField += lFile - lPos;
			}
			else
			{
				ao_iMaxLenBlobField++;
			}	
			ao_iMaxLenBlobField += strlen(ai_pcBlobParamValue + strlen(ai_pcBlobParamValue) + 1) + 1;			
		}
		else
		{
			ao_iMaxLenBlobField = SQL_NULL_DATA;
		}
	}
	else
	{
		ao_iMaxLenBlobField = SQL_NULL_DATA;
	}

	return M4_SUCCESS;
  //## end M4ODBC::GetMaxBlobLength%1089121924.body
}

m4return_t M4ODBC::BeginTransaction (const string &ai_sConnectionName)
{
  //## begin M4ODBC::BeginTransaction%1084786093.body preserve=yes
	m4return_t iResult = M4_ERROR;
	ClDBConnection * poConnection = NULL;

	iResult = GetConnection(ai_sConnectionName, poConnection);
	M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "M4ODBC::BeginTransaction", "GetConnection", "%s#%s");

	iResult = poConnection->Transact(M4LDB_BEGIN_TRANS);
	M4MDT_RETURN_ERROR1( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_BEGIN_TRANSACTION, ai_sConnectionName.c_str(), "%s");

	return M4_SUCCESS;
  //## end M4ODBC::BeginTransaction%1084786093.body
}

m4return_t M4ODBC::Commit (const string &ai_sConnectionName)
{
  //## begin M4ODBC::Commit%1084786094.body preserve=yes
	m4return_t iResult = M4_ERROR;
	ClDBConnection	* poConnection = NULL;

	iResult = GetConnection(ai_sConnectionName, poConnection);
	M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "M4ODBC::BeginTransaction", "GetConnection", "%s#%s");

	iResult = poConnection->Transact(M4LDB_COMMIT);
	M4MDT_RETURN_ERROR1(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_COMMIT, ai_sConnectionName.c_str(), "%s");

	return M4_SUCCESS;
  //## end M4ODBC::Commit%1084786094.body
}

m4return_t M4ODBC::RollBack (const string &ai_sConnectionName)
{
  //## begin M4ODBC::RollBack%1084786095.body preserve=yes
	m4return_t iResult = M4_ERROR;
	ClDBConnection	* poConnection = NULL;

	iResult = GetConnection(ai_sConnectionName, poConnection);
	M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "M4ODBC::BeginTransaction", "GetConnection", "%s#%s");

	iResult = poConnection->Transact(M4LDB_ROLLBACK);
	M4MDT_RETURN_ERROR1(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_ROLLBACK, ai_sConnectionName.c_str(), "%s");

	return M4_SUCCESS;
  //## end M4ODBC::RollBack%1084786095.body
}

m4return_t M4ODBC::GetData (ClCursor *ai_pCursor, const string &ai_sConnectionName, vector <m4uint32_t> &ai_vFieldDescription, M4IWriteResultSet &ao_oResultSet)
{
  //## begin M4ODBC::GetData%1079428395.body preserve=yes

	m4uint32_t	iRow = 0;
	size_t		iSize = 0 ;
	int			iWrite = 0 ;
	int			iFlush = 0 ;
	m4pcchar_t	pccValue = NULL ;

	
	for (;;)
	{
		m4return_t iErrorCode = ai_pCursor->Fetch();
		if (iErrorCode == SQL_NO_DATA_FOUND)
		{
			break;
		}

		if ((iErrorCode != SQL_SUCCESS) && (iErrorCode != SQL_SUCCESS_WITH_INFO))
		{
			GetODBCError(ai_pCursor, iErrorCode);
			M4MDT_DUMP_ERROR1(M4MDTERROR_FETCH, ai_sConnectionName.c_str(), "%s");
			return M4_ERROR;
		}
		
		// Save result set.
		iRow = ao_oResultSet.AddRow();
		M4MDT_RETURN_ERROR2(iRow == -1, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "M4ODBC::GetData", ai_sConnectionName.c_str(), "%s#AddRow = <%s>");

		if( m_pfTraceFile != NULL && ( m_iTraceLevel & M4MDT_TRACE_DATA ) != 0 )
		{
			iWrite = fprintf( m_pfTraceFile, "Row = %-4d ", iRow ) ;
			M4MDT_RETURN_ERROR2( iWrite < 0, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, M4MDT_ODBC_TRACE_FILE_NAME, strerror( errno ), "%s#%s" ) ;
		}


		for (m4uint32_t iCol = 0; iCol < m_vBindColumns.size(); iCol++)
		{
			// Get bind column length. If it is SQL_NULL_DATA, no data retrieved 
			// for the field.
			string sFieldValue;
			m4bool_t bNullData = M4_FALSE;

			m4uint32_t iM4Type = ai_vFieldDescription[iCol];

			if (iM4Type == M4CL_M4_TYPE_LONG || iM4Type == M4CL_M4_TYPE_UNI_LONG || iM4Type == M4CL_M4_TYPE_BINARY_STRING || iM4Type == M4CL_M4_TYPE_BLOB)
			{
				// Bug 0312304 El nulo de los campos long se debe comprobar a través del GetData, porque m_vBindColumnsLen no se binda
				if (GetLongData(ai_pCursor, iCol, iM4Type, sFieldValue, bNullData) != M4_SUCCESS)
				{
					M4MDT_DUMP_ERROR2(M4MDTERROR_INTERNAL_ERROR, "M4ODBC::GetData", iCol, "%s#Column = <%d>");
					return M4_ERROR;
				}
			}
			else
			{
				m4int32_t iValueLen = *(m_vBindColumnsLen[iCol]);

				if (iValueLen == SQL_NULL_DATA)
				{
					bNullData = M4_TRUE;
				}
				else
				{
					m4double_t dValue = 0.0;
					m4date_t dDate;
					m4char_t * pValue = NULL;
					size_t iLength = 0 ;

					switch (iM4Type)
					{
					case M4CL_M4_TYPE_FIXED_STRING:
					case M4CL_M4_TYPE_VAR_STRING:
					case M4CL_M4_TYPE_UNI_VAR_STRING:
					case M4CL_M4_TYPE_VARIANT:
					case M4CL_M4_TYPE_UNI_VARIANT:

						sFieldValue.assign(m_vBindColumns[iCol]->m_pcValue, iValueLen);
						break;

					case M4CL_M4_TYPE_NUMBER:
					case M4CL_M4_TYPE_CURRENCY:
					case M4CL_M4_TYPE_NUM_VARIANT:

						pValue = new m4char_t[M4MDT_DEFAULT_ARRAY_SIZE + 1];
						M4MDT_RETURN_ERROR1(pValue == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof(m4char_t) * (M4MDT_SIZE_DOUBLE + 1), "%d");

						/* Bug 0124974
						Hay que leer el valor, no la estructura
						*/
						dValue = *( (m4double_t*) m_vBindColumns[iCol]->m_pcValue );
						sprintf(pValue, "%.10f", dValue);

						// Se quitan los ceros no significativos
						iLength = strlen( pValue ) - 1 ;

						while( pValue[ iLength ] == '0' )
						{
							iLength-- ;
						}

						if( pValue[ iLength ] != '.' )
						{
							iLength++ ;
						}

						sFieldValue.assign(pValue, iLength);
						delete [] pValue ;
						break;

					case M4CL_M4_TYPE_TIMESTAMP:
					case M4CL_M4_TYPE_DATE:
					case M4CL_M4_TYPE_HOUR:

						pValue = new m4char_t[M4MDT_SIZE_TIMESTAMP + 1];
						M4MDT_RETURN_ERROR1(pValue == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof(m4char_t) * (M4MDT_SIZE_TIMESTAMP + 1), "%d");

						/* Bug 0124974
						Hay que leer el valor, no la estructura
						*/
						dDate = *( (m4date_t*) m_vBindColumns[iCol]->m_pcValue );
						ClDateToString(dDate, pValue, M4MDT_SIZE_TIMESTAMP + 1);
						sFieldValue.assign(pValue);
						delete [] pValue ;
						break;

					default:
						sFieldValue.assign(m_vBindColumns[iCol]->m_pcValue);
						break;
					}
				}
			}

			// Set field value. If the value recovered is null, set to null field 
			// value.
			ClMDTValue oFieldValue;
			if (bNullData == M4_TRUE)
			{
				// Null column data.
				oFieldValue.SetNull();
			}
			else
			{
				// Set column data.
				oFieldValue.Set(sFieldValue);
			}


			if( m_pfTraceFile != NULL && ( m_iTraceLevel & M4MDT_TRACE_DATA ) != 0 )
			{
				if( oFieldValue.IsNull() == M4_TRUE )
				{
					pccValue = "<NULL>" ;
					iSize = 6 ;
				}
				else
				{
					pccValue = oFieldValue.c_str() ;
					iSize = oFieldValue.size() ;
				}

				iWrite = fwrite( "[", 1, 1, m_pfTraceFile ) ;
				M4MDT_RETURN_ERROR2( iWrite != 1, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, M4MDT_ODBC_TRACE_FILE_NAME, strerror( errno ), "%s#%s" ) ;
				iWrite = fwrite( pccValue, 1, iSize, m_pfTraceFile ) ;
				M4MDT_RETURN_ERROR2( iWrite != iSize, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, M4MDT_ODBC_TRACE_FILE_NAME, strerror( errno ), "%s#%s" ) ;
				iWrite = fwrite( "]", 1, 1, m_pfTraceFile ) ;
				M4MDT_RETURN_ERROR2( iWrite != 1, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, M4MDT_ODBC_TRACE_FILE_NAME, strerror( errno ), "%s#%s" ) ;
			}


			// Set column value.
			if (ao_oResultSet.SetValue(iRow, iCol, oFieldValue) != M4_SUCCESS)
			{
				return M4_ERROR;
			}
		}


		if( m_pfTraceFile != NULL && ( m_iTraceLevel & M4MDT_TRACE_DATA ) != 0 )
		{
			iWrite = fprintf( m_pfTraceFile, "\n" ) ;
			M4MDT_RETURN_ERROR2( iWrite < 0, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, M4MDT_ODBC_TRACE_FILE_NAME, strerror( errno ), "%s#%s" ) ;
		}
	} 


	if( m_pfTraceFile != NULL && ( m_iTraceLevel & M4MDT_TRACE_DATA ) != 0 )
	{
		iWrite = fprintf( m_pfTraceFile, "\n" ) ;
		M4MDT_RETURN_ERROR2( iWrite < 0, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, M4MDT_ODBC_TRACE_FILE_NAME, strerror( errno ), "%s#%s" ) ;

		iFlush = fflush( m_pfTraceFile ) ;
		M4MDT_RETURN_ERROR2( iFlush == EOF, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, M4MDT_ODBC_TRACE_FILE_NAME, strerror( errno ), "%s#%s" ) ;
	}


	return M4_SUCCESS;
  //## end M4ODBC::GetData%1079428395.body
}

m4return_t M4ODBC::GetLongData (ClCursor *ai_pCursor, m4uint32_t ai_FieldCol, m4int32_t ai_iM4Type, string &ao_sLongData, m4bool_t &ao_bIsNull)
{
  //## begin M4ODBC::GetLongData%1081845829.body preserve=yes
	m4return_t iErrorCode;
	m4int32_t lLen;
	m4int32_t lLength = SQL_NULL_DATA;
	ao_bIsNull = M4_FALSE;
	m4int32_t iMaxLen = 0;
	m4pchar_t pcValue = NULL;
	string sTemp;


	// UNICODE DB
	iMaxLen = M4LDB_SIZE_LONG * M4UNI_FACTOR;
	pcValue = new char[ iMaxLen + 1 ];

	for (;;)
	{
		lLen = 0;
		iErrorCode = ai_pCursor->GetData(ai_FieldCol + 1, &pcValue, &iMaxLen, ai_iM4Type, &lLen);
	
		if (iErrorCode == SQL_NO_DATA_FOUND)
		{
			break;
		}

		if ((iErrorCode != SQL_SUCCESS) && (iErrorCode != SQL_SUCCESS_WITH_INFO))
		{
			delete [] pcValue ;
			GetODBCError(ai_pCursor, iErrorCode);
			M4MDT_DUMP_ERROR1(M4MDTERROR_GET_DATA, ai_FieldCol, "%d");
			return M4_ERROR;
		}

		if( lLen > 0 )
		{
			lLength = lLen;
			ao_sLongData.append(pcValue, lLength);
		}
	}

	delete [] pcValue ;


	if (lLength == SQL_NULL_DATA)
	{
		ao_sLongData = "";
		ao_bIsNull = M4_TRUE;
	}
	else
	{
		/* Bug 0156625
		Si es un campo long se copia sin los ceros finales
		*/
		if( ai_iM4Type == M4CL_M4_TYPE_LONG || ai_iM4Type == M4CL_M4_TYPE_UNI_LONG || ai_iM4Type == M4CL_M4_TYPE_BLOB)
		{
			if( ao_sLongData.length() > strlen( ao_sLongData.c_str() ) )
			{
				sTemp = ao_sLongData.c_str();
				ao_sLongData = sTemp;
			}
		}
	}

	return M4_SUCCESS;
  //## end M4ODBC::GetLongData%1081845829.body
}

m4return_t M4ODBC::GetODBCError (ClCursor *ai_pCursor, m4return_t ai_iErrorCode)
{
  //## begin M4ODBC::GetODBCError%1082109913.body preserve=yes
	m4return_t iErrorCode = ai_iErrorCode;
	m4pchar_t pcODBCError = NULL;
	m4int32_t iODBCErrorCode;
	while (iErrorCode != SQL_NO_DATA_FOUND)
	{	
		iErrorCode = ai_pCursor->GetErrorString(ai_iErrorCode, iODBCErrorCode, &pcODBCError);
		if (iErrorCode != SQL_NO_DATA_FOUND)
		{
			if (iErrorCode == M4_ERROR)
			{
				return M4_ERROR;
			}
			else
			{
				M4MDT_DUMP_ERROR2(M4MDTERROR_ODBC, iODBCErrorCode, pcODBCError, "%d#%s");
				delete [] pcODBCError ;
			}
		}
	}
		
	return M4_SUCCESS;
  //## end M4ODBC::GetODBCError%1082109913.body
}

m4void_t M4ODBC::DeleteConnectionMap ()
{
  //## begin M4ODBC::DeleteConnectionMap%1079428399.body preserve=yes
	connectionMap_t::iterator it = m_oConnectionMap.begin();
	ClDBConnection * pDBConnection = NULL;
	M4MDTransferConnection * pConnectionInfo = NULL;
	while (it != m_oConnectionMap.end())
	{
		pConnectionInfo = (*it).second;
		
		// Get database connection and close it.
		if (pConnectionInfo)
		{
			delete pConnectionInfo;
		}
		it++;
	}
	m_oConnectionMap.clear();
  //## end M4ODBC::DeleteConnectionMap%1079428399.body
}

m4return_t M4ODBC::DeleteBindColumns ()
{
  //## begin M4ODBC::DeleteBindColumns%1079428396.body preserve=yes
	// Delete bind column values vector.
	if (m_vBindColumns.size() > 0)
	{
		bindColumn_t::iterator it;
		m4pchar_t pFieldValue = NULL;
		it = m_vBindColumns.begin();
		while (it != m_vBindColumns.end())
		{
			if( *it != NULL )
			{
				pFieldValue = (*it)->m_pcValue;
				if( pFieldValue != NULL )
				{
					delete pFieldValue;
				}
				delete( *it ) ;
			}
			it++;
		}
		m_vBindColumns.clear();
	}

	// Delete bind columns length vector.
	if (m_vBindColumnsLen.size() > 0)
	{
		bindColumnLength_t::iterator it;
		m4int32_t * pFieldLen = NULL;
		it = m_vBindColumnsLen.begin();
		while (it != m_vBindColumnsLen.end())
		{
			pFieldLen = *it;
			if (pFieldLen)
			{
				delete pFieldLen;
			}
			it++;
		}
		m_vBindColumnsLen.clear();
	}
	return M4_SUCCESS;
  //## end M4ODBC::DeleteBindColumns%1079428396.body
}

m4return_t M4ODBC::DeleteParameters ()
{
  //## begin M4ODBC::DeleteParameters%1081322133.body preserve=yes
	// Delete bind parameters vector.
	if (m_vParameters.size() > 0)
	{
		bindParameter_t::iterator it;
		m4pchar_t pParam = NULL;
		it = m_vParameters.begin();
		while (it != m_vParameters.end())
		{
			pParam = *it;
			if (pParam)
			{
				delete pParam;
			}
			it++;
		}
		m_vParameters.clear();
	}

	// Delete bind parameters length vector.
	if (m_vParametersLen.size() > 0)
	{
		bindColumnLength_t::iterator it;
		m4int32_t * pFieldLen = NULL;
		it = m_vParametersLen.begin();
		while (it != m_vParametersLen.end())
		{
			pFieldLen = *it;
			if (pFieldLen)
			{
				delete pFieldLen;
			}
			it++;
		}
		m_vParametersLen.clear();
	}
	return M4_SUCCESS;
  //## end M4ODBC::DeleteParameters%1081322133.body
}

// Additional Declarations
  //## begin M4ODBC%4056C67E001E.declarations preserve=yes
  //## end M4ODBC%4056C67E001E.declarations

// Class M4MDTransferConnection 




M4MDTransferConnection::M4MDTransferConnection (const string &ai_sConnectionName, const string &ai_sConnectionString, eDDBBType_t ai_eDDBBType)
  //## begin M4MDTransferConnection::M4MDTransferConnection%1079428405.hasinit preserve=no
      : m_pDBConnection(NULL)
  //## end M4MDTransferConnection::M4MDTransferConnection%1079428405.hasinit
  //## begin M4MDTransferConnection::M4MDTransferConnection%1079428405.initialization preserve=yes
  //## end M4MDTransferConnection::M4MDTransferConnection%1079428405.initialization
{
  //## begin M4MDTransferConnection::M4MDTransferConnection%1079428405.body preserve=yes
	m_sConnectionName = ai_sConnectionName;
	
	// Set connection properties.
	m_stConnectionProperties.m_stConnStr = ai_sConnectionString;
	m_stConnectionProperties.m_eDBW = ai_eDDBBType;
	m_stConnectionProperties.m_stConExec = "";
  //## end M4MDTransferConnection::M4MDTransferConnection%1079428405.body
}


M4MDTransferConnection::~M4MDTransferConnection ()
{
  //## begin M4MDTransferConnection::~M4MDTransferConnection%1079428406.body preserve=yes
	if (m_pDBConnection)
	{
		// Disconnect.
		m4return_t iErrorCode = m_pDBConnection->Disconnect();
		M4MDT_CHECK_ERROR3(iErrorCode != M4_SUCCESS, M4MDTERROR_CLOSE_CONNECTION, "M4MDTransferConnection::End ()", "m_pDBConnection->Disconnect()", m_sConnectionName.c_str(), "%s#%s - Connection name <%s>");
		delete m_pDBConnection;
	}
  //## end M4MDTransferConnection::~M4MDTransferConnection%1079428406.body
}



//## Other Operations (implementation)
m4return_t M4MDTransferConnection::Init ()
{
  //## begin M4MDTransferConnection::Init%1083142244.body preserve=yes
	// Create new connection.
	m_pDBConnection = new ClDBConnection();
	M4MDT_RETURN_ERROR1(m_pDBConnection == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof(ClDBConnection), "%d");

	// Init connection.
	static m4int32_t iConnId = 0;
	if (m_pDBConnection->Init(iConnId, &m_stConnectionProperties) != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR1(M4MDTERROR_INIT_CONNECTION, m_sConnectionName.c_str(), "%s");
		delete m_pDBConnection;
		m_pDBConnection = NULL;
		return M4_ERROR;
	}

	// Init database connection.
	if (m_pDBConnection->InitConnection() != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR1(M4MDTERROR_INIT_CONNECTION, m_sConnectionName.c_str(), "%s");
		delete m_pDBConnection;
		m_pDBConnection = NULL;
		return M4_ERROR;
	}
	return M4_SUCCESS;
  //## end M4MDTransferConnection::Init%1083142244.body
}

ClDBConnection * M4MDTransferConnection::GetDBConnection ()
{
  //## begin M4MDTransferConnection::GetDBConnection%1079964156.body preserve=yes
	return m_pDBConnection;
  //## end M4MDTransferConnection::GetDBConnection%1079964156.body
}

eDDBBType_t M4MDTransferConnection::GetDDBBType ()
{
  //## begin M4MDTransferConnection::GetDDBBType%1079428404.body preserve=yes
	return m_stConnectionProperties.m_eDBW;
  //## end M4MDTransferConnection::GetDDBBType%1079428404.body
}

// Additional Declarations
  //## begin M4MDTransferConnection%40599E870364.declarations preserve=yes
  //## end M4MDTransferConnection%40599E870364.declarations

// Class M4FieldDescription 




M4FieldDescription::M4FieldDescription (m4uint8_t ai_iM4Type, m4uint32_t ai_iPrecision, m4uint32_t ai_iScale)
  //## begin M4FieldDescription::M4FieldDescription%1080297558.hasinit preserve=no
  //## end M4FieldDescription::M4FieldDescription%1080297558.hasinit
  //## begin M4FieldDescription::M4FieldDescription%1080297558.initialization preserve=yes
  //## end M4FieldDescription::M4FieldDescription%1080297558.initialization
{
  //## begin M4FieldDescription::M4FieldDescription%1080297558.body preserve=yes
	m_iM4Type = ai_iM4Type;
	m_iPrecision = ai_iPrecision;
	m_iScale = ai_iScale;
  //## end M4FieldDescription::M4FieldDescription%1080297558.body
}

M4FieldDescription::M4FieldDescription ()
  //## begin M4FieldDescription::M4FieldDescription%1080551835.hasinit preserve=no
  //## end M4FieldDescription::M4FieldDescription%1080551835.hasinit
  //## begin M4FieldDescription::M4FieldDescription%1080551835.initialization preserve=yes
  //## end M4FieldDescription::M4FieldDescription%1080551835.initialization
{
  //## begin M4FieldDescription::M4FieldDescription%1080551835.body preserve=yes
  //## end M4FieldDescription::M4FieldDescription%1080551835.body
}


M4FieldDescription::~M4FieldDescription ()
{
  //## begin M4FieldDescription::~M4FieldDescription%1080297559.body preserve=yes
  //## end M4FieldDescription::~M4FieldDescription%1080297559.body
}



//## Other Operations (implementation)
m4uint8_t M4FieldDescription::GetM4Type () const
{
  //## begin M4FieldDescription::GetM4Type%1080297560.body preserve=yes
	return m_iM4Type;
  //## end M4FieldDescription::GetM4Type%1080297560.body
}

m4uint32_t M4FieldDescription::GetPrecision () const
{
  //## begin M4FieldDescription::GetPrecision%1080297561.body preserve=yes
	return m_iPrecision;
  //## end M4FieldDescription::GetPrecision%1080297561.body
}

m4uint32_t M4FieldDescription::GetScale () const
{
  //## begin M4FieldDescription::GetScale%1080297562.body preserve=yes
	return m_iScale;
  //## end M4FieldDescription::GetScale%1080297562.body
}

m4return_t M4FieldDescription::SetM4Type (m4uint8_t ai_iM4Type)
{
  //## begin M4FieldDescription::SetM4Type%1080551836.body preserve=yes
	m_iM4Type = ai_iM4Type;
	return M4_SUCCESS;
  //## end M4FieldDescription::SetM4Type%1080551836.body
}

m4return_t M4FieldDescription::SetPrecision (m4uint32_t ai_iPrecision)
{
  //## begin M4FieldDescription::SetPrecision%1080551837.body preserve=yes
	m_iPrecision = ai_iPrecision;
	return M4_SUCCESS;
  //## end M4FieldDescription::SetPrecision%1080551837.body
}

m4return_t M4FieldDescription::SetScale (m4uint32_t ai_iScale)
{
  //## begin M4FieldDescription::SetScale%1080551838.body preserve=yes
	m_iScale = ai_iScale;
	return M4_SUCCESS;
  //## end M4FieldDescription::SetScale%1080551838.body
}

// Additional Declarations
  //## begin M4FieldDescription%4063EC8A037C.declarations preserve=yes
  //## end M4FieldDescription%4063EC8A037C.declarations

// Class M4SorteableRow 



M4SorteableRow::M4SorteableRow (const m4int32_t ai_viOrder[], M4Row* ai_poRow)
  //## begin M4SorteableRow::M4SorteableRow%1089013113.hasinit preserve=no
      : m_viSortOrder(ai_viOrder), m_poRow(ai_poRow)
  //## end M4SorteableRow::M4SorteableRow%1089013113.hasinit
  //## begin M4SorteableRow::M4SorteableRow%1089013113.initialization preserve=yes
  //## end M4SorteableRow::M4SorteableRow%1089013113.initialization
{
  //## begin M4SorteableRow::M4SorteableRow%1089013113.body preserve=yes
  //## end M4SorteableRow::M4SorteableRow%1089013113.body
}


// Additional Declarations
  //## begin M4SorteableRow%40E9045F02E5.declarations preserve=yes
  //## end M4SorteableRow%40E9045F02E5.declarations

//## begin module%3A3E57A701A6.epilog preserve=yes
//## end module%3A3E57A701A6.epilog

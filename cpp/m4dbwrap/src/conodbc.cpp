//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                conodbc.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                01-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module...
//
//
//==============================================================================


#ifdef _UNIX
#include "m4win.hpp"
#endif

#include "conodbc.hpp"
#include "m4stl.hpp"
#include "m4unicode.hpp"
extern int	M4IsUnicode( void ) ;


#ifdef DBWRAP_SERIALIZE
ClMutex g_oODBCAccess(M4_TRUE);
#endif

ClDBConnection_odbc::ClDBConnection_odbc (eDDBBType_t ai_eDDBB)
{ 
#ifdef DRIVER_ODBC
    m_hEnv = SQL_NULL_HENV;

	m_hDbc = SQL_NULL_HDBC;

	m_eDDBB = ai_eDDBB;

	m_indError = 1;
#else // DRIVER_ODBC
#endif
}


m4return_t ClDBConnection_odbc::Init (void) 
{
#ifdef DRIVER_ODBC
	SQLRETURN	result = SQL_SUCCESS;

	M4DBW_ODBC_CALL( result = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv ) );

	if (result != SQL_SUCCESS) {
		return (M4DBW_ODBC_ERROR(result));
	}

	M4DBW_ODBC_CALL( result = SQLSetEnvAttr( m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER ) );
	
	return M4DBW_ODBC_ERROR (result);

#else // DRIVER_ODBC
	return (M4_ERROR);
#endif
}


m4return_t ClDBConnection_odbc::End (void) 
{
#ifdef DRIVER_ODBC

	SQLRETURN	result = SQL_SUCCESS;

	Disconnect ();

	M4DBW_ODBC_CALL( result = SQLFreeHandle( SQL_HANDLE_ENV, m_hEnv ) );

	m_hEnv = SQL_NULL_HENV;

	return (M4_SUCCESS);

#else // DRIVER_ODBC
	return (M4_ERROR);
#endif
}


m4return_t ClDBConnection_odbc::Connect (m4pcchar_t ai_pConnStr)
{
#ifdef DRIVER_ODBC

	SQLRETURN		result = SQL_SUCCESS;
	int				iLength = 0;
	unsigned short*	pwcConnStr = NULL;


	M4DBW_ODBC_CALL( result = SQLAllocHandle( SQL_HANDLE_DBC, m_hEnv, &m_hDbc ) );

	if (result != SQL_SUCCESS) {
		return (M4DBW_ODBC_ERROR(result));
	}

	iLength = (int) strlen( ai_pConnStr ) ;

	// UNICODE DB
	if( M4IsUnicode() == 1 )
	{
		pwcConnStr = (unsigned short*) M4CppToDB( ai_pConnStr, iLength ) ;
		M4DBW_ODBC_CALL( result = SQLDriverConnectW( m_hDbc, NULL, (SQLWCHAR*) pwcConnStr, (SQLSMALLINT) iLength, NULL, 0, NULL, SQL_DRIVER_NOPROMPT ) );
		delete [] pwcConnStr ;
	}
	else
	{
		M4DBW_ODBC_CALL( result = SQLDriverConnect ( m_hDbc, NULL, (SQLCHAR*) ai_pConnStr, (SQLSMALLINT) iLength, NULL, 0, NULL, SQL_DRIVER_NOPROMPT ) );
	}


	if( m_eDDBB == M4DBW_ORACLE_DDBB )
	{
		// El branding es sólo para oracle

		//Mejora: 0090185
		//Importante: Establecemos los atributos sin controlar error pq el connect puede dar warnings que me impedirían
		//establecer atributos!.

		//Vamos a utilizar un driver con una licencia determinada. Se debe establecer despues de la conexión.
		//Esta llamada es set de atributos, se tratan de atributos específicos de DataDirect por lo tanto en el 
		//driver de SQLServer de Microsoft puede dar un error pq el atributo no existe.
		//No vamos a controlar el error!.
		m4return_t resultBrandDriver = _UseBrandDriver (m_hDbc);
		//En realidad siempre vamos a dar success!!.
		if (resultBrandDriver != SQL_SUCCESS) {
			return (M4DBW_ODBC_ERROR(result));
		}
	}

	//Ahora si comprobamos el retorno de connect.
	// Se aborta sólo si es error, no si es warning
	m4return_t iResult = M4DBW_ODBC_ERROR(result);

	if (iResult == M4_ERROR) {
		return (M4_ERROR);
	}

	M4DBW_ODBC_CALL( result = SQLSetConnectAttr( m_hDbc, SQL_AUTOCOMMIT, (SQLPOINTER) SQL_AUTOCOMMIT_ON, SQL_IS_INTEGER ) );

	return (M4DBW_ODBC_ERROR (result));
#else // DRIVER_ODBC
   return (M4_ERROR);
#endif
} 

m4return_t ClDBConnection_odbc::_UseBrandDriver (HDBC ai_hDbc)
{
#ifdef DRIVER_ODBC
	SQLRETURN	result = SQL_SUCCESS;

	const m4int32_t M4DB_BRAND_PASSWORD_SIZE = 24;

	string sPASSWORD("S<.BpFA:?;FL=HAM>PDO");
	string sLICENCE ("IVEM.LIC");
	size_t iIndex = 0;
	size_t iLenPwd = 0;
	
	//Ojo, no vamos a controlar errores!.
	M4DBW_ODBC_CALL( result = SQLSetConnectAttr( ai_hDbc, 1041, (SQLPOINTER) sLICENCE.c_str(),  (SQLINTEGER) sLICENCE.length() ) );
	
	//Creamos una passwd de 24 caracteres inicializada con Q.
	string sPwd("");	
	for(iIndex = 0; iIndex < M4DB_BRAND_PASSWORD_SIZE; iIndex++) {
		sPwd.append("Q");
	}

	//Sustituimos con la passwd.
	iLenPwd = sPASSWORD.length();
	sPwd.replace(0, iLenPwd, sPASSWORD);
	for(iIndex = 0; iIndex < iLenPwd; iIndex++) {
		sPwd[iIndex] = (char) (160 + iIndex - sPASSWORD[iIndex]);
	}

	//Ojo, no vamos a controlar errores!.
	M4DBW_ODBC_CALL( result = SQLSetConnectAttr( ai_hDbc, 1042, (SQLPOINTER) sPwd.c_str(), (SQLINTEGER) sPwd.length() ) );

	//Ojo, no vamos a controlar errores!.
	return  SQL_SUCCESS;

#else // DRIVER_ODBC
   return (M4_ERROR);
#endif
}

m4return_t ClDBConnection_odbc::Disconnect (void)
{
#ifdef DRIVER_ODBC

	SQLRETURN	result = SQL_SUCCESS;

	if (m_hDbc)
	{
		// Hacemos RollBack por si acaso.
		result = Transact (M4LDB_ROLLBACK);

		M4DBW_ODBC_CALL( result = SQLDisconnect( m_hDbc ) );
		M4DBW_ODBC_CALL( result = SQLFreeHandle( SQL_HANDLE_DBC, m_hDbc ) );

		m_hDbc = SQL_NULL_HDBC;
	}
  
	return (M4_SUCCESS);

#else // DRIVER_ODBC
	return (M4_ERROR);
#endif
} 


m4return_t ClDBConnection_odbc::Transact (eTransType_t ai_eTransType)
{
#ifdef DRIVER_ODBC

	SQLRETURN	result = SQL_SUCCESS;

	switch (ai_eTransType)
	{
		case M4LDB_BEGIN_TRANS:
			M4DBW_ODBC_CALL( result = SQLEndTran( SQL_HANDLE_DBC, m_hDbc, SQL_ROLLBACK ) );
			M4DBW_ODBC_CALL( result = SQLSetConnectAttr( m_hDbc, SQL_AUTOCOMMIT, (SQLPOINTER) SQL_AUTOCOMMIT_OFF, SQL_IS_INTEGER ) );
		break;

		case M4LDB_COMMIT:
			M4DBW_ODBC_CALL( result = SQLEndTran( SQL_HANDLE_DBC, m_hDbc, SQL_COMMIT ) );
			M4DBW_ODBC_CALL( result = SQLSetConnectAttr( m_hDbc, SQL_AUTOCOMMIT, (SQLPOINTER) SQL_AUTOCOMMIT_ON, SQL_IS_INTEGER ) );
		break;

		case M4LDB_ROLLBACK:
			M4DBW_ODBC_CALL( result = SQLEndTran( SQL_HANDLE_DBC, m_hDbc, SQL_ROLLBACK ) );
			M4DBW_ODBC_CALL( result = SQLSetConnectAttr( m_hDbc, SQL_AUTOCOMMIT, (SQLPOINTER) SQL_AUTOCOMMIT_ON, SQL_IS_INTEGER ) );
		break;
	}

	if (result != SQL_SUCCESS){
		return (M4DBW_ODBC_ERROR (result));
	}

	return (M4_SUCCESS);

#else // DRIVER_ODBC
   return (M4_ERROR);
#endif
}

m4return_t ClDBConnection_odbc::GetErrorString (m4return_t ai_retcode, m4int32_t &ao_iErrorCode, m4pchar_t *ao_ppcErrorString)
{
	ao_iErrorCode = 0 ;
	*ao_ppcErrorString = NULL ;

#ifdef DRIVER_ODBC

	SQLRETURN	result = SQL_SUCCESS;
	int			iLength = 0;
	SQLSMALLINT	iErrorLength = 0;
	SQLINTEGER	iErrorCode = 0;
	SQLWCHAR	awcErrorMessage[ M4LDB_SIZE_LONG + 1 ] ;


	*awcErrorMessage = '\0' ;

	// UNICODE DB
	if( M4IsUnicode() == 1 )
	{
		M4DBW_ODBC_CALL( result = SQLGetDiagRecW( SQL_HANDLE_DBC, m_hDbc, m_indError, NULL, &iErrorCode, (SQLWCHAR*) awcErrorMessage, M4LDB_SIZE_LONG, &iErrorLength ) );
	}
	else
	{
		M4DBW_ODBC_CALL( result = SQLGetDiagRec ( SQL_HANDLE_DBC, m_hDbc, m_indError, NULL, &iErrorCode, (SQLCHAR*) awcErrorMessage,  M4LDB_SIZE_LONG, &iErrorLength ) );
	}

	m_indError++;
	
	ao_iErrorCode = (m4int32_t) iErrorCode ;
	
	switch (result) {

		case (SQL_SUCCESS) :
			
			// UNICODE DB
			iLength = iErrorLength ;
			*ao_ppcErrorString = M4DBToCpp( awcErrorMessage, iLength ) ;

			if (ai_retcode == M4_WARNING) {
				return (M4_WARNING);
			}
			else {
				return (M4_SUCCESS);
			}
			break;
			
		case (SQL_NO_DATA) :
			m_indError = 1;
			return (M4LDB_SQL_NO_DATA_FOUND);
			break;
			
		default:
			m_indError = 1;
			return (M4_ERROR);
	}

#else // DRIVER_ODBC
   return (M4_ERROR);
#endif
}


m4return_t ClDBConnection_odbc::ExecuteSQL (m4pcchar_t ai_pSQL)
{
#ifdef DRIVER_ODBC

	SQLRETURN		result = SQL_SUCCESS;
	HSTMT			hStmt = 0;
	int				iLength = 0;
	unsigned short*	pwcSQL = NULL;


	M4DBW_ODBC_CALL( result = SQLAllocStmt( m_hDbc, &hStmt ) );

	if (result != SQL_SUCCESS) {
		return (M4DBW_ODBC_ERROR (result));
	}

	iLength = (int) strlen( ai_pSQL ) ;

	// UNICODE DB
	if( M4IsUnicode() == 1 )
	{
		pwcSQL = (unsigned short*) M4CppToDB( ai_pSQL, iLength ) ;
		M4DBW_ODBC_CALL( result = SQLExecDirectW( hStmt, (SQLWCHAR*) pwcSQL, (SQLINTEGER) iLength ) );
		delete [] pwcSQL ;
	}
	else
	{
		M4DBW_ODBC_CALL( result = SQLExecDirect( hStmt, (SQLCHAR*) ai_pSQL, (SQLINTEGER) iLength ) );
	}

	if (result != SQL_SUCCESS) {
		return (M4DBW_ODBC_ERROR (result));
	}

	M4DBW_ODBC_CALL( result = SQLFreeStmt( hStmt, SQL_DROP ) );

	return (M4DBW_ODBC_ERROR (result));
#else // DRIVER_ODBC
   return (M4_ERROR);
#endif
}


m4return_t ClDBConnection_odbc::SetIsolationLevel (eIsolationLevel_t ai_eILevel)
{
#ifdef DRIVER_ODBC
	SQLULEN		uILevel;
	SQLRETURN	result = SQL_SUCCESS;

	switch (ai_eILevel)
	{
		case M4LDB_IL_READ_UNCOMMITED:
			uILevel = SQL_TXN_READ_UNCOMMITTED;
			break;

		case M4LDB_IL_READ_COMMITED:
			uILevel = SQL_TXN_READ_COMMITTED;
			break;

		case M4LDB_IL_REPEATABLE_READ:
			uILevel = SQL_TXN_REPEATABLE_READ;
			break;

		case M4LDB_IL_SERIALIZABLE:
			uILevel = SQL_TXN_SERIALIZABLE;
			break;

		default: 
			// No soportada.
			return (M4_SUCCESS);
	}
 
	// (In SQL Server, repeatable read is treated as serializable.) 
	M4DBW_ODBC_CALL( result = SQLSetConnectAttr( m_hDbc, SQL_TXN_ISOLATION, (SQLPOINTER) uILevel, SQL_IS_INTEGER ) );

	return (M4DBW_ODBC_ERROR (result));
#else // DRIVER_ODBC
   return (M4_ERROR);
#endif
}


//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb
// File:                curodbc.inl
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


#include "conodbc.hpp"
#include "m4unicode.hpp"
extern int	M4IsUnicode( void ) ;



M4_INLINE ClCursor_odbc::ClCursor_odbc (void)
{
#ifdef DRIVER_ODBC

	m_hStmt = SQL_NULL_HSTMT;
	m_pConn = NULL;

	m_indError = 1;
	m_bBindBinaryZeroPrec = M4_FALSE;
#else // DRIVER_ODBC
#endif
}


M4_INLINE ClCursor_odbc::~ClCursor_odbc (void)
{
#ifdef DRIVER_ODBC
	
	Destroy ();

#else // DRIVER_ODBC
#endif
}


M4_INLINE m4return_t ClCursor_odbc::Open (ClDBConnection_Base *ai_pConn)
{
#ifdef DRIVER_ODBC
	SQLRETURN	result = SQL_SUCCESS;

	m_pConn = (ClDBConnection_odbc *) ai_pConn;

	M4DBW_ODBC_CALL( result = SQLAllocHandle( SQL_HANDLE_STMT, m_pConn->m_hDbc, &m_hStmt ) );

	return (M4DBW_ODBC_ERROR (result));
#else // DRIVER_ODBC
   return (M4_ERROR);
#endif
}


M4_INLINE m4return_t ClCursor_odbc::Close (void)
{
#ifdef DRIVER_ODBC

	if (m_hStmt)
	{
		SQLRETURN	result = SQL_SUCCESS;

		//Bug 0080517
		//Podemos utilizar tanto SQLCloseCursor como SQLFreeStmt (opción SQL_CLOSE). Sin embargo
		//la primera puede dar cursor inválido que afecta a un posible trigger físico de actualización
		//que se ha lanzado en la BD SQLServer. En internet he comprobado que hay casos en los que recomiendan 
		//tener un define para que se utilize SQLFreeSmtm #define SQLCloseCursor(x) SQLFreeStmt(x, SQL_CLOSE).
		//Sin embargo, parece ser que la recomendación es utilizar SQLCloseCursor ya que SQLFreeStmt es utilizado
		//en versiones ODBC 1.0.		
		M4DBW_ODBC_CALL( result = SQLFreeStmt( m_hStmt, SQL_CLOSE ) );

		return (M4DBW_ODBC_ERROR (result));
	}

	return (M4_SUCCESS);

#else // DRIVER_ODBC
   return (M4_ERROR);
#endif
}


M4_INLINE m4return_t ClCursor_odbc::Prepare (m4pcchar_t ai_pStmt, m4int32_t ai_numPar, m4int32_t ai_numCol, m4bool_t ai_bMultiRow)
{
#ifdef DRIVER_ODBC

	SQLRETURN		result = SQL_SUCCESS;
	int				iLength = -1;
	unsigned short*	pwcStmt = NULL;

	
	// UNICODE DB
	if( M4IsUnicode() == 1 )
	{
		pwcStmt = (unsigned short*) M4CppToDB( ai_pStmt, iLength ) ;
		M4DBW_ODBC_CALL( result = SQLPrepareW( m_hStmt, (SQLWCHAR*) pwcStmt, SQL_NTS ) );
		delete [] pwcStmt ;
	}
	else
	{
		M4DBW_ODBC_CALL( result = SQLPrepare ( m_hStmt, (SQLCHAR*) ai_pStmt, SQL_NTS ) );
	}

	// bg 202317
	m4pcchar_t pccExternDB = getenv( "M4_SSR_EXTERN_DB" );

	if( pccExternDB != NULL && strcmp( pccExternDB, "1" ) == 0 )
	{
		m_bBindBinaryZeroPrec = M4_TRUE;
	}

	return (M4DBW_ODBC_ERROR (result));
#else // DRIVER_ODBC
   return (M4_ERROR);
#endif
}

	
M4_INLINE m4int32_t ClCursor_odbc::GetNumCol (void)
{
#ifdef DRIVER_ODBC

	SQLRETURN	result = SQL_SUCCESS;
	SQLSMALLINT	numCol;

	M4DBW_ODBC_CALL( result = SQLNumResultCols( m_hStmt, &numCol ) );

	if (result == SQL_ERROR) {
		return (M4LDB_NULL_INDEX);
	}

	return ((m4int32_t) numCol);

#else // DRIVER_ODBC
   return (M4_ERROR);
#endif
}




//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb
// File:                curodbc.cpp
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
#include "m4sqltypes.h"
#include "m4win.hpp"
#endif

#include "conodbc.hpp"
#include "curodbc.hpp"
#include "m4clock.hpp"
#include "m4date.hpp"


#ifndef _M4_USING_INLINE
	#include "curodbc.inl"
#endif

#include <stdio.h>

#ifdef DBWRAP_TRACE
extern	FILE *g_pFile;
#endif


// En SQLServer, si estamos utilizando la version 3 de ODBC, al pasar precision -1 da error de Invalid Precision Value.
// Si le pasamos una precision menor de la que vayamos a introducir, da error en el SQLPutData de Sting data, right truncation.
// Realmente no sabemos la precision de lo que vamos a meter, asi que habría que pasar la maxima.
// Tenemos dos soluciones :
//		- Rebindar cada vez.
//		- Pasar la maxima precision.

// Elegimos la segunda, y esperamos que nos contesten de Microsoft.
// En unicode la precisión es la mitad redondeada a la baja

#define LONGBLOB_MAX_PRECISION						2147483647
#define WLONGBLOB_MAX_PRECISION						1073741823




m4return_t ClCursor_odbc::Destroy (void)
{
#ifdef DRIVER_ODBC

	SQLRETURN		result = SQL_SUCCESS;
	StParamBinding	*poParamBinding = NULL;
	StResultBinding	*poResultBinding = NULL;


	if( m_hStmt != SQL_NULL_HSTMT )
	{
		M4DBW_ODBC_CALL( result = SQLFreeHandle( SQL_HANDLE_STMT, m_hStmt ) );
		m_hStmt = SQL_NULL_HSTMT;
	}

	m_pConn = NULL;

	for( vector<StParamBinding*>::const_iterator it = m_vpcParamBinding.begin () ; it != m_vpcParamBinding.end() ; it++ )
	{
		poParamBinding = *it;

		if( poParamBinding != NULL )
		{
			if( poParamBinding->m_poTimeStamp != NULL )
			{
				delete( poParamBinding->m_poTimeStamp );
			}

			if( poParamBinding->m_poUniString != NULL )
			{
				if( poParamBinding->m_poUniString->m_pvData != NULL )
				{
					delete( poParamBinding->m_poUniString->m_pvData );
				}
				delete( poParamBinding->m_poUniString );
			}

			if( poParamBinding->m_poLongString != NULL )
			{
				if( poParamBinding->m_poLongString->m_pUniData != NULL )
				{
					delete( poParamBinding->m_poLongString->m_pUniData );
				}
				delete( poParamBinding->m_poLongString );
			}

			delete( poParamBinding );
		}
	}

	m_vpcParamBinding.clear();

	for( vector<StResultBinding*>::const_iterator it = m_vpcResultBinding.begin () ; it != m_vpcResultBinding.end() ; it++ )
	{
		poResultBinding = *it;

		if( poResultBinding != NULL )
		{
			if( poResultBinding->m_poTimeStamp != NULL )
			{
				delete( poResultBinding->m_poTimeStamp );
			}

			if( poResultBinding->m_poUniString != NULL )
			{
				if( poResultBinding->m_poUniString->m_pvData != NULL )
				{
					delete( poResultBinding->m_poUniString->m_pvData );
				}
				delete( poResultBinding->m_poUniString );
			}

			delete( poResultBinding );
		}
	}

	m_vpcResultBinding.clear();

	return( M4_SUCCESS );

#else // DRIVER_ODBC
   return( M4_ERROR );
#endif
}


m4return_t ClCursor_odbc::BindParam (m4int32_t ai_indPar, m4pchar_t ai_pValue, m4pchar_t *ai_ppValue, m4int32_t ai_lPrec, m4int32_t ai_lM4Type, m4int32_t ai_lScale, m4pint32_t ao_lLength)
{
#ifdef DRIVER_ODBC

	SQLRETURN		result = SQL_SUCCESS;
	m4bool_t		bIsUnicode = M4_FALSE;
	SQLPOINTER		pValue = NULL;
	SQLSMALLINT		sSqlType = 0;
	SQLSMALLINT		cppType = 0;
	SQLUINTEGER		lPrec = 0;
	StParamBinding*	poBinding = NULL;


	// Si viene dirección de puntero el puntero debe venir a NULL y lo cogemos de la dirección
	if( ai_ppValue != NULL )
	{
		ai_pValue = *ai_ppValue ;
	}

	poBinding = new StParamBinding( ao_lLength );
	m_vpcParamBinding.push_back( poBinding );

	switch( ai_lM4Type )
	{
		case M4CL_M4_TYPE_FIXED_STRING:
		case M4CL_M4_TYPE_VAR_STRING:
		case M4CL_M4_TYPE_UNI_VAR_STRING:
		case M4CL_M4_TYPE_VARIANT:
		case M4CL_M4_TYPE_UNI_VARIANT:

			// UNICODE DB
			if( M4IsUnicode() == 1 )
			{
				// Se necesita crear una estructura intermedia para bindar el unicode
				if( ai_lM4Type == M4CL_M4_TYPE_UNI_VAR_STRING || ai_lM4Type == M4CL_M4_TYPE_UNI_VARIANT )
				{
					bIsUnicode = M4_TRUE;
				}

				// Bug 128689.
				poBinding->m_poUniString = new StUniStringParam( ai_pValue, ai_ppValue, ai_lPrec, bIsUnicode );

				pValue = poBinding->m_poUniString->m_pvData;

				if( bIsUnicode == M4_TRUE )
				{
					sSqlType = SQL_WVARCHAR;
					cppType = SQL_C_WCHAR;
				}
				else
				{
					sSqlType = SQL_VARCHAR;
					cppType = SQL_C_CHAR;
				}

				// Bug 156415. En ODBC Oracle Unicode se debe meter la precisión en bytes
				if( bIsUnicode == M4_TRUE && m_pConn->m_eDDBB == M4DBW_ORACLE_DDBB )
				{
					lPrec = ai_lPrec * sizeof(unsigned short);
				}
				else
				{
					lPrec = ai_lPrec;
				}
			}
			else
			{
				pValue = ai_pValue;
				sSqlType = SQL_VARCHAR;
				cppType = SQL_C_CHAR;
				lPrec = ai_lPrec;
			}
			break;

		case M4CL_M4_TYPE_BINARY_STRING:
		case M4CL_M4_TYPE_BLOB:

			poBinding->m_poLongString = new StLong( ai_indPar, ai_ppValue, ai_lM4Type, M4_FALSE );

			pValue = (SQLPOINTER) (size_t) ai_indPar;
			cppType = SQL_C_BINARY;

			if( m_bBindBinaryZeroPrec == M4_TRUE )
			{
				// bg 187621.
				// bg 202317. Blob.
				sSqlType = SQL_VARBINARY;
				lPrec = 0;
			}
			else
			{
				sSqlType = SQL_LONGVARBINARY;
				lPrec = LONGBLOB_MAX_PRECISION;
			}

			break;

		case M4CL_M4_TYPE_LONG:
		case M4CL_M4_TYPE_UNI_LONG:

			if( M4IsUnicode() == 1 && ai_lM4Type == M4CL_M4_TYPE_UNI_LONG )
			{
				bIsUnicode = M4_TRUE;
			}

			poBinding->m_poLongString = new StLong( ai_indPar, ai_ppValue, ai_lM4Type, bIsUnicode );
				
			pValue = (SQLPOINTER) (size_t) ai_indPar;

			// UNICODE DB
			if( M4IsUnicode() == 1 )
			{
				if( bIsUnicode == M4_TRUE )
				{
					sSqlType = SQL_WLONGVARCHAR;
					cppType = SQL_C_WCHAR;
					lPrec = WLONGBLOB_MAX_PRECISION;
				}
				else
				{
					sSqlType = SQL_LONGVARCHAR;
					cppType = SQL_C_CHAR;
					lPrec = LONGBLOB_MAX_PRECISION;
				}
			}
			else
			{
				sSqlType = SQL_LONGVARCHAR;
				cppType = SQL_C_CHAR;
				lPrec = LONGBLOB_MAX_PRECISION;
			}

			break;

		case M4CL_M4_TYPE_DATE:
		case M4CL_M4_TYPE_TIMESTAMP:
		case M4CL_M4_TYPE_HOUR:

			poBinding->m_poTimeStamp = new StTimeStamp( ai_pValue );

			pValue = &poBinding->m_poTimeStamp->m_stTimestamp;
			sSqlType = SQL_TIMESTAMP;
			cppType = SQL_C_TIMESTAMP;
			lPrec = ai_lPrec;

			break;

		case M4CL_M4_TYPE_NUMBER:
		case M4CL_M4_TYPE_CURRENCY:
		case M4CL_M4_TYPE_NUM_VARIANT:
		case M4CL_M4_TYPE_TIME_SPAN:
		default:

			pValue = ai_pValue;
			/* Bug 0110437
			Habría que bindar como numeric y no como double
			sSqlType = SQL_NUMERIC;
			*/
			sSqlType = SQL_DOUBLE;
			cppType = SQL_C_DOUBLE;
			lPrec = ai_lPrec;

			break;
	}

	M4DBW_ODBC_CALL( result = SQLBindParameter( m_hStmt, (SQLUSMALLINT) ai_indPar, SQL_PARAM_INPUT, cppType, sSqlType, lPrec, 0, pValue, 0, &poBinding->m_iBindingLength ) );

	return( M4DBW_ODBC_ERROR( result ) );

#else // DRIVER_ODBC
   return( M4_ERROR );
#endif
}


m4return_t ClCursor_odbc::BindResult (m4int32_t ai_lCol, m4pchar_t *aio_ppResult, m4int32_t *aio_plMaxLen, m4int32_t ai_lM4Type, m4pint32_t ao_lLength, m4int32_t ai_lMaxLenFld)
{
#ifdef DRIVER_ODBC

	SQLRETURN			result = SQL_SUCCESS;
	m4bool_t			bIsUnicode = M4_FALSE;
	SQLPOINTER			pValue = NULL;
	SQLSMALLINT			cppType;
	SQLINTEGER			lMaxLen = 0;
	StResultBinding*	poBinding = NULL;


	poBinding = new StResultBinding( ao_lLength );
	m_vpcResultBinding.push_back( poBinding );

	switch( ai_lM4Type )
	{
		case M4CL_M4_TYPE_LONG:
		case M4CL_M4_TYPE_UNI_LONG:
		case M4CL_M4_TYPE_BLOB:
		case M4CL_M4_TYPE_BINARY_STRING:

			return (M4_SUCCESS);
			break;

		case M4CL_M4_TYPE_NUMBER:
		case M4CL_M4_TYPE_CURRENCY:
		case M4CL_M4_TYPE_NUM_VARIANT:
		case M4CL_M4_TYPE_TIME_SPAN:

			pValue = *aio_ppResult;
			cppType = SQL_C_DOUBLE;
			lMaxLen = sizeof (m4double_t);

			break;

		case M4CL_M4_TYPE_DATE:
		case M4CL_M4_TYPE_TIMESTAMP:
		case M4CL_M4_TYPE_HOUR:

			poBinding->m_poTimeStamp = new StTimeStamp( *aio_ppResult );

			pValue = &poBinding->m_poTimeStamp->m_stTimestamp;
			cppType = SQL_C_TIMESTAMP;
			lMaxLen = sizeof (TIMESTAMP_STRUCT);

			break;

		case M4CL_M4_TYPE_FIXED_STRING:
		case M4CL_M4_TYPE_VAR_STRING:
		case M4CL_M4_TYPE_UNI_VAR_STRING:
		case M4CL_M4_TYPE_VARIANT:
		case M4CL_M4_TYPE_UNI_VARIANT:

			// UNICODE DB
			if( M4IsUnicode() == 1 )
			{
				// Se necesita crear una estructura intermedia para bindar el unicode
				if( ai_lM4Type == M4CL_M4_TYPE_UNI_VAR_STRING || ai_lM4Type == M4CL_M4_TYPE_UNI_VARIANT )
				{
					bIsUnicode = M4_TRUE;
				}

				poBinding->m_poUniString = new StUniStringResult( aio_ppResult, aio_plMaxLen, bIsUnicode );

				pValue = poBinding->m_poUniString->m_pvData;

				if( bIsUnicode == M4_TRUE )
				{
					cppType = SQL_C_WCHAR;
					lMaxLen = ( *aio_plMaxLen + 1 ) * sizeof( unsigned short );
				}
				else
				{
					cppType = SQL_C_CHAR;
					lMaxLen = *aio_plMaxLen + 1;
				}
			}
			else
			{
				pValue = *aio_ppResult;
				cppType = SQL_C_CHAR;
				lMaxLen = *aio_plMaxLen + 1;
			}
			break;

		default:
			break;
	}

	M4DBW_ODBC_CALL( result = SQLBindCol( m_hStmt, (SQLUSMALLINT) ai_lCol, cppType, pValue, lMaxLen, &poBinding->m_iBindingLength ) );

	return( M4DBW_ODBC_ERROR( result ) );

#else // DRIVER_ODBC
   return( M4_ERROR );
#endif
}


m4return_t ClCursor_odbc::DescribeCol (m4int32_t ai_lCol, m4pint32_t ao_plPrec, m4pint8_t ao_cM4Type)
{
#ifdef DRIVER_ODBC

	SQLRETURN	result = SQL_SUCCESS;
	SQLSMALLINT	lSqlType;
	SQLULEN		iSqlPrec;

	*ao_cM4Type = M4CL_M4_TYPE_FIXED_STRING;
	*ao_plPrec = M4LDB_SIZE_DEFAULT_STRING;

	M4DBW_ODBC_CALL( result = SQLDescribeCol( m_hStmt, (SQLUSMALLINT) ai_lCol, NULL, 0, NULL, &lSqlType, &iSqlPrec, NULL, NULL ) );

	*ao_plPrec = (m4int32_t) iSqlPrec;

	switch( lSqlType )
	{
		case SQL_LONGVARCHAR:
			*ao_cM4Type = M4CL_M4_TYPE_LONG;
			break;
	
		case SQL_WLONGVARCHAR:
			*ao_cM4Type = M4CL_M4_TYPE_UNI_LONG;
			break;
	
		/* Bug 0125267
		El binario debe distinguirse del long
		*/
		case SQL_VARBINARY:
		case SQL_LONGVARBINARY:
			*ao_cM4Type = M4CL_M4_TYPE_BINARY_STRING;
			break;
	
		case SQL_DATE:
		case SQL_TIME:
		case SQL_TIMESTAMP:
		case SQL_TYPE_DATE:
		case SQL_TYPE_TIME:
		case SQL_TYPE_TIMESTAMP:
			*ao_cM4Type = M4CL_M4_TYPE_TIMESTAMP;
			break;

		case SQL_BIGINT:
		case SQL_BINARY:
		case SQL_BIT:
		case SQL_DECIMAL:
		case SQL_DOUBLE:
		case SQL_FLOAT:
		case SQL_INTEGER:
		case SQL_NUMERIC:
		case SQL_REAL:
		case SQL_SMALLINT:
		case SQL_TINYINT:
			*ao_cM4Type = M4CL_M4_TYPE_NUMBER;
			break;

		case SQL_WCHAR:
		case SQL_WVARCHAR:
			*ao_cM4Type = M4CL_M4_TYPE_UNI_VAR_STRING;
			break;

		case SQL_CHAR:
		case SQL_VARCHAR:
		default:
			*ao_cM4Type = M4CL_M4_TYPE_FIXED_STRING;
			break;
	}

	return( M4DBW_ODBC_ERROR( result ) );

#else // DRIVER_ODBC
   return( M4_ERROR );
#endif
}


m4return_t ClCursor_odbc::Execute( m4uint32_t ai_iTimeOut, ICancelStatusProvider* ai_poCancelStatus, m4return_t& ao_riCanceled )
{
	ao_riCanceled = 0;

#ifdef DRIVER_ODBC

	SQLRETURN		result = SQL_SUCCESS;
	SQLRETURN		result2 = 0;
	SQLPOINTER		pParam = NULL;
	m4int32_t		indPar = 0;
	int				year = 0;
	int				month = 0;
	int				day = 0;
	m4int32_t		hour = 0;
	m4int32_t		minute = 0;
	m4int32_t		second = 0;
	int				iLength = 0 ;
	int				iCharSize = 1;
	void*			pUniValue = NULL;
	StParamBinding*	poBinding = NULL;


	// Recorremos los parametros y realizamos la conversion
	for( vector<StParamBinding*>::const_iterator it = m_vpcParamBinding.begin() ; it != m_vpcParamBinding.end() ; it++ )
	{
		poBinding = *it;

		if( poBinding != NULL )
		{
			poBinding->m_iBindingLength = *poBinding->m_piSourceLength;

			// Fechas
			if( poBinding->m_poTimeStamp != NULL )
			{
				if( poBinding->m_iBindingLength != SQL_NULL_DATA )
				{
					ClDateToNumbers( *(poBinding->m_poTimeStamp->m_pdDate), year, month, day, hour, minute, second );
					poBinding->m_poTimeStamp->m_stTimestamp.year = year;
					poBinding->m_poTimeStamp->m_stTimestamp.month = month;
					poBinding->m_poTimeStamp->m_stTimestamp.day = day;
					poBinding->m_poTimeStamp->m_stTimestamp.hour = (SQLUSMALLINT) hour;
					poBinding->m_poTimeStamp->m_stTimestamp.minute = (SQLUSMALLINT) minute;
					poBinding->m_poTimeStamp->m_stTimestamp.second = (SQLUSMALLINT) second;
				}
			}

			// Cadenas unicode
			if( poBinding->m_poUniString != NULL )
			{
				if( poBinding->m_iBindingLength != SQL_NULL_DATA )
				{
					/* Bug 0221741
					Se calcula la longitud siempre, porque si venía con longitud más de una vez se ha duplicado
					*/
					iLength = -1 ;

					if( poBinding->m_poUniString->m_bIsUnicode == M4_TRUE )
					{
						pUniValue = M4CppToDB( *(poBinding->m_poUniString->m_ppcData), iLength ) ;
						iCharSize = sizeof( unsigned short );
					}
					else
					{
						pUniValue = M4CppToANSI( *(poBinding->m_poUniString->m_ppcData), iLength ) ;
						iCharSize = sizeof( unsigned char );
					}

					if( iLength > poBinding->m_poUniString->m_iPrec )
					{
						iLength = poBinding->m_poUniString->m_iPrec ;
					}

					memcpy( poBinding->m_poUniString->m_pvData, pUniValue, iLength * iCharSize ) ;
					memset( (unsigned char*)poBinding->m_poUniString->m_pvData + iLength * iCharSize, '\0', iCharSize ) ;

					DBWRAP_BIN_TRACE( "Uni", "Param", iLength * sizeof( unsigned short ), poBinding->m_poUniString->m_pvData ) ;

					delete [] pUniValue ;
					pUniValue = NULL ;

					/* Bug 0125942
					Si es unicode la longitud hay que doblarla, al igual que hay que dividirla en el fetch
					*/
					if( poBinding->m_iBindingLength != SQL_NTS )
					{
						poBinding->m_iBindingLength = iLength * iCharSize ;
					}
				}
			}

			// Long
			if( poBinding->m_poLongString != NULL )
			{
				if( poBinding->m_iBindingLength == 0 || poBinding->m_iBindingLength == M4LDB_SQL_NULL_DATA )
				{
					// Problema cuando se utiliza una sentencia en la que se binda un parametro Long.
					// Si la primera vez, iba con valor, y la segunda va a Null, hay comportamiento diferente segun en que base de datos estemos
					// En Oracle, no funciona el pasarle Null Data como longitud. Sale el error de Data Type has changed.
					// En informix, gepea si se habia utilizado antes con valor no nulo, y le damos como longitud M4LDB_SQL_LEN_DATA_AT_EXEC(0).
					// En SQLServer funciona de las dos formas.
					// Nota aclaratoria: Se ha comprobado que en SQLServer lo anterior no iba bien por lo que hacemos que vaya como Oracle.

					/* Bug 0209429
					En SQLServer nos falla el SQLParamData si no ponemos nulo
					De momento ignoramos el comentario anterior
					*/
					if( m_pConn->m_eDDBB == M4DBW_ORACLE_DDBB )
					{
						poBinding->m_iBindingLength = M4LDB_SQL_LEN_DATA_AT_EXEC( 0 );
					}
					else
					{
						// En access parece que va bien si se pone directamente nulo
						poBinding->m_iBindingLength = M4LDB_SQL_NULL_DATA;
					}
				}
				else
				{
					// UNICODE DB
					/* Bug 0159011
					En unicode hay que pasar la longitud de los bytes del utf-16
					*/
					m4int32_t lLength = (m4int32_t) poBinding->m_iBindingLength;

					if( ( poBinding->m_poLongString->m_iM4Type == M4CL_M4_TYPE_LONG || poBinding->m_poLongString->m_iM4Type == M4CL_M4_TYPE_UNI_LONG ) && M4IsUnicode() == 1 )
					{
						iLength = lLength ;

						if( poBinding->m_poLongString->m_pUniData != NULL )
						{
							delete( poBinding->m_poLongString->m_pUniData ) ;
							poBinding->m_poLongString->m_pUniData = NULL ;
						}

						if( poBinding->m_poLongString->m_bIsUnicode == M4_TRUE )
						{
							poBinding->m_poLongString->m_pUniData = M4CppToDB( *(poBinding->m_poLongString->m_ppcData), iLength ) ;
							lLength = iLength * sizeof( unsigned short ) ;
						}
						else
						{
							poBinding->m_poLongString->m_pUniData = M4CppToANSI( *(poBinding->m_poLongString->m_ppcData), iLength ) ;
							lLength = iLength ;
						}
					}

					poBinding->m_iBindingLength = M4LDB_SQL_LEN_DATA_AT_EXEC( lLength );
				}
			}
		}
	}

	if( ai_iTimeOut > 0 )
	{
		m4uint32_t		iTimes = 0 ;
		m4millisec_t	endTime = ClPrecisionTicker::GetTimeTick() + ai_iTimeOut * 1000 ;

		do
		{
			M4DBW_ODBC_SAFE( result = SQLExecute( m_hStmt ) ) ;

			if( result == SQL_STILL_EXECUTING )
			{
				iTimes++ ;
				if( iTimes % 1000 == 0 )
				{
					if( ClPrecisionTicker::GetTimeTick() >= endTime )
					{
						ao_riCanceled = -2 ;
					}
					else if( ai_poCancelStatus != NULL && ai_poCancelStatus->GetCancelStatus() == M4_TRUE )
					{
						ao_riCanceled = -1 ;
					}

					if( ao_riCanceled != 0 )
					{
						M4DBW_ODBC_CALL( SQLCancel( m_hStmt ) ) ;
						M4DBW_ODBC_CALL( result = SQLExecute( m_hStmt ) ) ;
						break;
					}
				}
				m4usleep( 500 ) ;
			}
		}
		while( result == SQL_STILL_EXECUTING ) ;
	}
	else
	{
		M4DBW_ODBC_CALL( result = SQLExecute( m_hStmt ) );
	}

	while( result == SQL_NEED_DATA )
	{
		M4DBW_ODBC_CALL( result = SQLParamData( m_hStmt, &pParam ) );

		if( result == SQL_NEED_DATA )
		{
			StParamBinding	*poLongBinding = NULL;

			for( vector<StParamBinding*>::const_iterator it = m_vpcParamBinding.begin() ; it != m_vpcParamBinding.end() ; it++ )
			{
				poBinding = *it;

				if( poBinding != NULL && poBinding->m_poLongString != NULL && poBinding->m_poLongString->m_indParam == (m4int32_t) (size_t) pParam )
				{
					poLongBinding = poBinding;
					break;
				}
			}

			if( poLongBinding != NULL )
			{
				m4int32_t lLength = *poLongBinding->m_piSourceLength;

				if( lLength == M4LDB_SQL_NULL_DATA )
				{
					M4DBW_ODBC_CALL( result2 = SQLPutData( m_hStmt, NULL, M4LDB_SQL_NULL_DATA ) );

					if( result2 == M4_ERROR )
					{
						return( M4DBW_ODBC_ERROR( result2 ) );
					}
				}
				else
				{
					m4int32_t m4type = poLongBinding->m_poLongString->m_iM4Type;

					if( m4type == M4CL_M4_TYPE_LONG || m4type == M4CL_M4_TYPE_UNI_LONG || m4type == M4CL_M4_TYPE_BINARY_STRING )
					{
						m4pchar_t pValue =  *(poLongBinding->m_poLongString->m_ppcData);

						// UNICODE DB
						// Si es un long se pasa a unicode
						if( ( m4type == M4CL_M4_TYPE_LONG || m4type == M4CL_M4_TYPE_UNI_LONG ) && M4IsUnicode() == 1 )
						{
							pUniValue = poLongBinding->m_poLongString->m_pUniData ;

							if( poLongBinding->m_poLongString->m_bIsUnicode == M4_TRUE )
							{
								iCharSize = sizeof( unsigned short );
							}
							else
							{
								iCharSize = sizeof( unsigned char );
							}

							// Bug 0299345 La longitud debe ser física, no lógica
							lLength = (m4int32_t) M4LDB_SQL_LEN_DATA_AT_EXEC( poLongBinding->m_iBindingLength ) / iCharSize;
						}
						else
						{
							pUniValue = NULL ;
						}

						for( m4int32_t lLen = 0 ; lLen < lLength ; lLen += M4LDB_SIZE_LONG )
						{
							m4int32_t lSize = M4LDB_SIZE_LONG;

							if( lLen + M4LDB_SIZE_LONG >= lLength )
							{
								lSize = lLength - lLen;
							}

							// UNICODE DB
							if( pUniValue != NULL )
							{
								M4DBW_ODBC_CALL( result2 = SQLPutData( m_hStmt, ((unsigned char*)pUniValue) + lLen * iCharSize, lSize * iCharSize ) );
							}
							else
							{
								M4DBW_ODBC_CALL( result2 = SQLPutData( m_hStmt, pValue + lLen, lSize ) );
							}

							if( result2 == M4_ERROR )
							{
								return( M4DBW_ODBC_ERROR( result2 ) );
							}
						}
					}
					else
					{
						m4char_t aValue[ M4LDB_SIZE_LONG + 1 ];

						m4pchar_t pNameFile = *(poLongBinding->m_poLongString->m_ppcData);

						FILE *pFile = NULL;

						size_t lSize = M4LDB_SIZE_LONG;

						if( *pNameFile )
						{
							// UNICODE FILE
							eUniFileEncodingType_t eEncoding = M4UniANSI ;
							pFile = M4Fopen( pNameFile, M4UniReadBinary, eEncoding ) ;

						}

						if( !pFile )
						{
							M4DBW_ODBC_CALL( result2 = SQLPutData( m_hStmt, NULL, M4LDB_SQL_NULL_DATA ) );

							if( result2 == M4_ERROR )
							{
								return( M4DBW_ODBC_ERROR( result2 ) );
							}
							continue;
						}

						/* Bug 0159752, 0173439
						Hay que guardar todo el fichero, y puede que la cabecera se haya saltado
						*/
						fseek( pFile, 0, SEEK_SET ) ;

						// Buscamos la descripción. Formato buffer: filename\0HEADER+descr\0ext\0
						m4pchar_t pFileDescr = pNameFile + strlen( pNameFile ) + 1;
						lSize = strlen( pFileDescr ) + 1;

						// buscamos la extensión (con header)
						m4pchar_t pcExt = pFileDescr + strlen(pFileDescr) + 1;

						// apenda la extensión
						lSize += strlen( pcExt ) + 1;

						// Formato del BLOB en BD : HEADER+Descripción\0\Ext\0Fichero
						M4DBW_ODBC_CALL( result2 = SQLPutData( m_hStmt, pFileDescr, lSize ) );

						if( result2 == M4_ERROR )
						{
							return( M4DBW_ODBC_ERROR( result2 ) );
						}

						do
						{
							lSize = M4LDB_SIZE_LONG;

							lSize = fread( (m4pvoid_t) aValue, sizeof (m4char_t), lSize, pFile );

							if( lSize > 0 )
							{
								M4DBW_ODBC_CALL( result2 = SQLPutData( m_hStmt, aValue, lSize ) );

								if( result2 == M4_ERROR )
								{
									return( M4DBW_ODBC_ERROR( result2 ) );
								}
							}
						}
						while( lSize > 0 );

						fclose( pFile );
					}
				}
			}
		}
	}

	if( result == SQL_NO_DATA )
	{
		result = SQL_SUCCESS;
	}
	
	return( M4DBW_ODBC_ERROR( result ) );

#else // DRIVER_ODBC
   return( M4_ERROR );
#endif
}


m4return_t ClCursor_odbc::Fetch (void)
{
#ifdef DRIVER_ODBC

	SQLRETURN			result = SQL_SUCCESS;
	int					iLength = 0 ;
	m4pchar_t			pcValue = NULL;
	StResultBinding*	poBinding = NULL;


	M4DBW_ODBC_CALL( result = SQLFetch( m_hStmt ) );

	if (result == SQL_NO_DATA) {
		return (M4LDB_SQL_NO_DATA_FOUND);
	}

	// Recorrer los resultados y realizamos la conversion
	for( vector<StResultBinding*>::const_iterator it = m_vpcResultBinding.begin() ; it != m_vpcResultBinding.end() ; it++ )
	{
		poBinding = *it;

		if( poBinding != NULL )
		{
			*poBinding->m_piSourceLength = (m4int32_t) poBinding->m_iBindingLength ;

			// Fechas
			if( poBinding->m_poTimeStamp != NULL )
			{
				if( poBinding->m_iBindingLength != SQL_NULL_DATA )
				{
					m4date_t dDate;
					ClNumbersToDate(	poBinding->m_poTimeStamp->m_stTimestamp.year,
										poBinding->m_poTimeStamp->m_stTimestamp.month,
										poBinding->m_poTimeStamp->m_stTimestamp.day,
										poBinding->m_poTimeStamp->m_stTimestamp.hour,
										poBinding->m_poTimeStamp->m_stTimestamp.minute,
										poBinding->m_poTimeStamp->m_stTimestamp.second,
										dDate);
					*poBinding->m_poTimeStamp->m_pdDate = dDate;
				}
			}

			// Cadenas unicode
			if( poBinding->m_poUniString != NULL )
			{
				if( poBinding->m_iBindingLength != SQL_NULL_DATA )
				{
					// UNICODE DB
					/* Bug 0126144
					No nos podemos fiar de la longitud que devuelve el driver
					Hay que asumir que viene null terminada
					*/
					iLength = -1 ;

					if( poBinding->m_poUniString->m_bIsUnicode == M4_TRUE )
					{
						pcValue = M4DBToCpp( poBinding->m_poUniString->m_pvData, iLength ) ;
					}
					else
					{
						pcValue = M4ANSIToCpp( (char*)poBinding->m_poUniString->m_pvData, iLength ) ;
					}

					if( *(poBinding->m_poUniString->m_ppcData) != NULL && iLength > *(poBinding->m_poUniString->m_piMaxLength) )
					{
						// Si el buffer de salida es pequeño se elimina
						delete [] *(poBinding->m_poUniString->m_ppcData) ;
						*(poBinding->m_poUniString->m_piMaxLength) = 0 ;
						*(poBinding->m_poUniString->m_ppcData) = NULL ;
					}

					if( *(poBinding->m_poUniString->m_ppcData) == NULL )
					{
						// Si no hay buffer de salida se crea uno nuevo
						*(poBinding->m_poUniString->m_piMaxLength) = iLength ;
						*(poBinding->m_poUniString->m_ppcData) = new char[ iLength + 1 ] ;
					}

					memcpy( *(poBinding->m_poUniString->m_ppcData), pcValue, iLength * sizeof( m4char_t ) ) ;
					(*(poBinding->m_poUniString->m_ppcData))[ iLength ] = '\0' ;
					delete [] pcValue ;
					pcValue = NULL ;

					*poBinding->m_piSourceLength = iLength ;
				}
			}
		}
	}

	return( M4DBW_ODBC_ERROR( result ) );

#else // DRIVER_ODBC666Langileak

   return( M4_ERROR );
#endif
}


m4return_t ClCursor_odbc::GetData (m4int32_t ai_lCol, m4pchar_t *aio_ppResult, m4int32_t *aio_plMaxLen, m4int32_t ai_lM4Type, m4pint32_t ao_lLength, m4bool_t ai_bByPass)
{
	*ao_lLength = 0 ;

	if( *aio_ppResult != NULL )
	{
		**aio_ppResult = '\0' ;
	}

#ifdef DRIVER_ODBC

	SQLRETURN		result = SQL_SUCCESS;
	SQLSMALLINT		cppType = 0 ;
	SQLLEN			iSqlLength = 0 ;
	int				iLength = 0 ;
	int				iBufferLength = 0 ;
	m4pchar_t		pcValue = NULL ;
	unsigned short	awcValue[ M4LDB_SIZE_LONG + 1 ] ;


	*awcValue = '\0' ;

	if( ai_bByPass == M4_TRUE )
	{
		return M4LDB_SQL_NO_DATA_FOUND;
	}

	iBufferLength = M4LDB_SIZE_LONG ;

	switch( ai_lM4Type )
	{
		case M4CL_M4_TYPE_BLOB:
		case M4CL_M4_TYPE_BINARY_STRING:

			cppType = SQL_C_BINARY;
			break;

		case M4CL_M4_TYPE_NUMBER:
		case M4CL_M4_TYPE_CURRENCY:
		case M4CL_M4_TYPE_NUM_VARIANT:
		case M4CL_M4_TYPE_TIME_SPAN:

			cppType = SQL_C_DOUBLE;
			break;

		case M4CL_M4_TYPE_LONG:
		case M4CL_M4_TYPE_UNI_LONG:
		case M4CL_M4_TYPE_FIXED_STRING:
		case M4CL_M4_TYPE_VAR_STRING:
		case M4CL_M4_TYPE_UNI_VAR_STRING:
		case M4CL_M4_TYPE_VARIANT:
		case M4CL_M4_TYPE_UNI_VARIANT:
		default:

			// UNICODE DB
			if( M4IsUnicode() == 1 )
			{
				if( ai_lM4Type == M4CL_M4_TYPE_UNI_LONG || ai_lM4Type == M4CL_M4_TYPE_UNI_VAR_STRING || ai_lM4Type == M4CL_M4_TYPE_UNI_VARIANT )
				{
					// Si es unicode el buffer es el doble
					iBufferLength *= sizeof( unsigned short );
					cppType = SQL_C_WCHAR;
				}
				else
				{
					cppType = SQL_C_CHAR;
				}
			}
			else
			{
				cppType = SQL_C_CHAR;
			}
			break;
	}

	M4DBW_ODBC_CALL( result = SQLGetData( m_hStmt, (SQLUSMALLINT) ai_lCol, cppType, (SQLPOINTER) awcValue, (SQLLEN) iBufferLength, &iSqlLength ) );

	iLength = (m4int32_t) iSqlLength ;
	*ao_lLength = iLength ;

	if( iLength != 0 && iLength != SQL_NULL_DATA )
	{
		if( iLength == SQL_NO_TOTAL || iLength > iBufferLength )
		{
			iLength = iBufferLength ;

			// Si no se lee en binario viene con un cero final
			if( cppType == SQL_C_CHAR )
			{
				iLength -= sizeof( char ) ;
			}
			else if( cppType == SQL_C_WCHAR )
			{
				iLength -= sizeof( unsigned short ) ;
			}
		}

		if( cppType == SQL_C_WCHAR )
		{
			// Los long en unicode hay que transformarlos y recalcular la longitud
			iLength /= sizeof( unsigned short ) ;
			pcValue = M4DBToCpp( awcValue, iLength ) ;
		}
		else if( cppType == SQL_C_CHAR )
		{
			/* Bug 0179631
			En caso de cadena ansi también hay que pasar el valor a tipo Cpp
			*/
			pcValue = M4ANSIToCpp( (m4pchar_t) awcValue, iLength ) ;
		}
		else
		{
			pcValue = (m4pchar_t) awcValue ;
		}

		if( *aio_ppResult != NULL && iLength > *aio_plMaxLen )
		{
			// Si el buffer de salida es pequeño se elimina
			delete [] *aio_ppResult ;
			*aio_plMaxLen = 0 ;
			*aio_ppResult = NULL ;
		}

		if( *aio_ppResult == NULL )
		{
			// Si no hay buffer de salida se crea uno nuevo
			*aio_plMaxLen = iLength ;
			*aio_ppResult = new char[ *aio_plMaxLen + 1 ] ;
		}

		// Se rellena el buffer de salida
		*ao_lLength = iLength ;
		memcpy( *aio_ppResult, pcValue, iLength ) ;
		(*aio_ppResult)[ iLength ] = '\0' ;

		if( cppType == SQL_C_WCHAR || cppType == SQL_C_CHAR )
		{
			delete [] pcValue ;
		}
	}

	return( M4DBW_ODBC_ERROR( result ) );

#else // DRIVER_ODBC
   return( M4_ERROR );
#endif
}


m4return_t ClCursor_odbc::GetErrorString (m4return_t ai_retcode, m4int32_t &ao_iErrorCode, m4pchar_t *ao_ppcErrorString)
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
		M4DBW_ODBC_CALL( result = SQLGetDiagRecW( SQL_HANDLE_STMT, m_hStmt, m_indError, NULL, &iErrorCode, (SQLWCHAR*) awcErrorMessage, M4LDB_SIZE_LONG, &iErrorLength ) );
	}
	else
	{
		M4DBW_ODBC_CALL( result = SQLGetDiagRec ( SQL_HANDLE_STMT, m_hStmt, m_indError, NULL, &iErrorCode, (SQLCHAR*) awcErrorMessage,  M4LDB_SIZE_LONG, &iErrorLength ) );
	}

	m_indError++;
	
	ao_iErrorCode = (m4int32_t) iErrorCode ;
	
	switch( result )
    {
		case SQL_SUCCESS:
			
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
			
		case SQL_NO_DATA :
			m_indError = 1;
			return (M4LDB_SQL_NO_DATA_FOUND);
			break;
			
		default:
			m_indError = 1;
			return( M4_ERROR );
    }

#else // DRIVER_ODBC
   return( M4_ERROR );
#endif
}


m4return_t ClCursor_odbc::SetBlockingMode( m4uint32_t ai_iMode )
{
#ifdef DRIVER_ODBC

	SQLRETURN	result = SQL_SUCCESS;

	if( ai_iMode == 1 )
	{
		M4DBW_ODBC_CALL( result = SQLSetStmtAttr( m_hStmt, SQL_ATTR_ASYNC_ENABLE, (SQLPOINTER) SQL_ASYNC_ENABLE_ON, 0 ) );
	}
	else
	{
		M4DBW_ODBC_CALL( result = SQLSetStmtAttr( m_hStmt, SQL_ATTR_ASYNC_ENABLE, (SQLPOINTER) SQL_ASYNC_ENABLE_OFF, 0 ) );
	}

	return( M4DBW_ODBC_ERROR( result ) );

#else // DRIVER_ODBC
   return( M4_ERROR );
#endif
}


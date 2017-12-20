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

// ************************************************************
// IMPORTANTE : Para usar esta clase hay que definir DRIVER_OCI
// ************************************************************

#include "m4unicode.hpp"
#include "conoci.hpp"
#include "curoci.hpp"
#include "cldates.hpp"
#include "m4clock.hpp"
#include "m4date.hpp"
#include <stdio.h>

#ifndef _M4_USING_INLINE
	#include "curoci.inl"
#endif

extern int	M4IsUnicode( void ) ;

#ifdef DBWRAP_TRACE
extern	FILE *g_pFile;
#endif


	
m4return_t ClCursor_oci::Prepare (m4pcchar_t ai_pStmt, m4int32_t ai_numPar, m4int32_t ai_numCol, m4bool_t ai_bMultiRow)
{
#ifdef DRIVER_OCI

	sword			result = OCI_SUCCESS;
	string			spStmt = ai_pStmt;
	m4pcchar_t		pStm = spStmt.c_str();
	m4int32_t		lPos = 0;
	m4int32_t		indParam = 1;
	m4char_t		aParam[10];
	m4bool_t		bIsString = M4_FALSE;
	int				iLength = -1;
	unsigned short*	pwcStmt = NULL;


	m_bLongExist = M4_FALSE;
	m_lnumBindParam = ai_numPar;
	m_lnumBindResult = ai_numCol;

	if (ai_bMultiRow)
	{
		m_lNumRow = 100;
	}

	if (m_lnumBindParam)
	{
		//Sustituimos los interrogantes/parametros por el formato correcto de
		//los binds  :V001,:V002, etc
		//Siempre que no esten entre comillas!
		while (pStm[lPos])
		{
			if (pStm[lPos] == '\'') {
				bIsString = M4_BOOL(!bIsString);
			}
			else
			{
				if (pStm[lPos] == '?' && !bIsString)
				{
					if (indParam < 10)
					{
						sprintf (aParam, ":V00%d", indParam);
					}
					else
					{
						if (indParam < 100)
						{
							sprintf (aParam, ":V0%d", indParam);
						}
						else
						{
							sprintf (aParam, ":V%d", indParam);
						}
					}
					spStmt.replace (lPos, 1, aParam);
					indParam++;
					// Por si se realoca.
					pStm = spStmt.c_str();
				}
			}
			lPos++;
		}
	}

	m4int32_t lLen = (m4int32_t) spStmt.size () + 1;

	// Quitamos los blancos del principio.
	while (*pStm == ' ')
	{
		pStm++;
		lLen--;
	}

	m_bIsSelect = !strncmp(pStm, "SELECT", 6) ? M4_TRUE : M4_FALSE;

	/*
	Si se comenta el NOWAIT se puede comentar todo este bloque
	if (m_bIsSelect)
	{
		if (!strncmp (pStm+lLen-11, "FOR UPDATE", 10))
		{
			//spStmt.append (" NOWAIT");
			pStm = spStmt.c_str();
			lLen = (m4int32_t) spStmt.size () + 1;
			// Quitamos los blancos del principio.
			while (*pStm == ' ')
			{
				pStm++;
				lLen--;
			}
		}		
	}
	*/

	// UNICODE DB
	if( M4IsUnicode() == 1 )
	{
		iLength = lLen ;
		pwcStmt = (unsigned short*) M4CppToDB( pStm, iLength ) ;
		iLength *= sizeof( unsigned short ) ;

		DBWRAP_BIN_TRACE( "Uni", "Prepare", iLength, pwcStmt ) ;

		M4DBW_OCI_CALL( result = OCIStmtPrepare( m_hStmt, m_pConn->m_hErr, (text*) pwcStmt, iLength, OCI_NTV_SYNTAX, OCI_DEFAULT ) );
		delete [] pwcStmt ;
	}
	else
	{
		DBWRAP_BIN_TRACE( "Ansi", "Prepare", lLen, pStm ) ;

		M4DBW_OCI_CALL( result = OCIStmtPrepare( m_hStmt, m_pConn->m_hErr, (text*) pStm, lLen, OCI_NTV_SYNTAX, OCI_DEFAULT ) );
	}

	return (M4DBW_OCI_ERROR(result)); 
#else
	return (M4_ERROR);
#endif
}


m4return_t ClCursor_oci::Close (void)
{
#ifdef DRIVER_OCI

	sword result = OCI_SUCCESS;

	//Bug 0083639
	m4int32_t indCol;
	m4int32_t lFirstToDiscard = m_lNumRow - m_lRemainder;
	if (lFirstToDiscard < 0)
	{
		lFirstToDiscard = 0;
	}

	for (indCol=0; indCol<m_lnumBindResult; indCol++)
	{
		for (m4int32_t indRow = lFirstToDiscard; indRow < m_lNumRow; indRow ++)
		{
			//Si encuentro uno que no tiene datos validos ya no sigo pq se supone que el resto no son validos.
			if (m_pstBindResult[indCol].m_pRcode[indRow] == OCI_NO_DATA)
			{
				break;
			}
			//De esta forma descartamos valores, no hace falta recorrer todo el vector, nos basta con
			//poner el valor al primero que todavía no he leído.
			m_pstBindResult[indCol].m_pInd[indRow] = -1;
			m_pstBindResult[indCol].m_pLen[indRow] = -1;
			m_pstBindResult[indCol].m_pRcode[indRow] = OCI_NO_DATA;
		}
	}

	if (m_hStmt && m_bLongExist)
	{
		for (m4int32_t indCol=0; indCol<m_lnumBindResult; indCol++)
		{
			if ((m_pstBindResult[indCol].m_iM4Type == M4CL_M4_TYPE_LONG || 
				 m_pstBindResult[indCol].m_iM4Type == M4CL_M4_TYPE_UNI_LONG || 
				 m_pstBindResult[indCol].m_iM4Type == M4CL_M4_TYPE_BLOB || 
				 m_pstBindResult[indCol].m_iM4Type == M4CL_M4_TYPE_BINARY_STRING) && 
				!m_pstBindResult[indCol].m_bLongRead)
			{
				// Hay resultados pendientes por lo que los desechamos.
				result = ReadLong (indCol + 1);

				if (result == OCI_ERROR)
				{
					return (M4DBW_OCI_ERROR(result));
				}
			}
		}
	}

	return (result); 
#else
	return (M4_ERROR);
#endif
}


m4return_t ClCursor_oci::Destroy (void)
{
#ifdef DRIVER_OCI

	sword result = OCI_SUCCESS;

	if (m_hStmt)
	{
		M4DBW_OCI_CALL( result = OCIHandleFree( m_hStmt, OCI_HTYPE_STMT ) );
		// Aunque retorne -1 no significa que exista error. 
		// Simplemente indica que no hay más datos.
	}
	m_hStmt = NULL;	
	
	if (m_pstBindResult)
	{
		for (m4int32_t indCol=0; indCol<m_lnumBindResult; indCol++)
		{
			if (m_pstBindResult[indCol].m_iM4Type)
			{
				if (m_pstBindResult[indCol].m_iM4Type != M4CL_M4_TYPE_LONG && 
					m_pstBindResult[indCol].m_iM4Type != M4CL_M4_TYPE_UNI_LONG &&
					m_pstBindResult[indCol].m_iM4Type != M4CL_M4_TYPE_BLOB &&
					m_pstBindResult[indCol].m_iM4Type != M4CL_M4_TYPE_BINARY_STRING)
				{
					delete [] m_pstBindResult[indCol].m_pBuf;
				}
				delete [] m_pstBindResult[indCol].m_pInd;
				delete [] m_pstBindResult[indCol].m_pLen;
				delete [] m_pstBindResult[indCol].m_pRcode;
			}
		}
		
		delete [] m_pstBindResult;
	}
	m_pstBindResult = NULL;
	
	if (m_pstBindParam)
	{
		for (m4int32_t indPar=0; indPar<m_lnumBindParam; indPar++)
		{
			if (m_pstBindParam[indPar].m_pwcData != NULL)
			{
				delete [] m_pstBindParam[indPar].m_pwcData ;
				m_pstBindParam[indPar].m_pwcData = NULL ;
			}
		}
	
		delete [] m_pstBindParam;
	}
	m_pstBindParam = NULL;

	return (M4_SUCCESS);
#else
	return (M4_ERROR);
#endif
}


m4return_t ClCursor_oci::BindParam (m4int32_t ai_indPar, m4pchar_t ai_pValue, m4pchar_t *ai_ppValue, m4int32_t ai_lPrec, m4int32_t ai_lM4Type, m4int32_t ai_lScale, m4int32_t *ao_lLength)
{
#ifdef DRIVER_OCI

	sword			datatype;
	sword			result = OCI_SUCCESS;
	ub1				iCForm = SQLCS_NCHAR;
	ub4				mode= OCI_DEFAULT;
	ub4				lLen;
	m4pchar_t		pValue = NULL;
	m4int32_t		indPar = ai_indPar-1;
	m4bool_t		bSetCharacterSet = M4_FALSE ;
	int				iLength = 0 ;
	unsigned short* pwcValue = NULL;
	

	// Si viene dirección de puntero el puntero debe venir a NULL y lo cogemos de la dirección
	if( ai_ppValue != NULL )
	{
		ai_pValue = *ai_ppValue ;
	}

	pValue = ai_pValue;


	if (indPar >= m_lnumBindParam)
	{
		return (M4_ERROR);
	}

	if (!m_pstBindParam)
	{
		m_pstBindParam = new stBindParam[m_lnumBindParam];
	}
	
	m_pstBindParam[indPar].m_piLength = ao_lLength;
	m_pstBindParam[indPar].m_indPar = ai_indPar;
	m_pstBindParam[indPar].m_pBuf = pValue;

	if( ai_ppValue != NULL )
	{
		m_pstBindParam[indPar].m_ppBuf = ai_ppValue;
	}
	else
	{
		m_pstBindParam[indPar].m_ppBuf = &m_pstBindParam[indPar].m_pBuf;
	}
	
	m_pstBindParam[indPar].m_iM4Type = (m4int8_t) ai_lM4Type;
	m_pstBindParam[indPar].m_lPrec = ai_lPrec;
	m_pstBindParam[indPar].m_lScale = ai_lScale;
	m_pstBindParam[indPar].m_pInd = (short) *(ao_lLength);
	
	if (m_pstBindParam[indPar].m_pInd == -3)
	{
		m_pstBindParam[indPar].m_pInd = 0;
	}

	switch (m_pstBindParam[indPar].m_iM4Type)
	{
		case M4CL_M4_TYPE_LONG:
		case M4CL_M4_TYPE_UNI_LONG:
		case M4CL_M4_TYPE_BLOB:
		case M4CL_M4_TYPE_BINARY_STRING:

			m_bRebindAllParameters = M4_TRUE;

			lLen = *(ao_lLength);
			
			if (m_pstBindParam[indPar].m_iM4Type == M4CL_M4_TYPE_LONG || m_pstBindParam[indPar].m_iM4Type == M4CL_M4_TYPE_UNI_LONG)
			{
				datatype = SQLT_LNG;

				// UNICODE DB
				if( M4IsUnicode() == 1 )
				{
					bSetCharacterSet = M4_TRUE ;

					/* Bug 0163890
					En unicode, hay que tener en cuenta caracteres y no bytes
					*/
					if( lLen != M4LDB_SQL_NULL_DATA && lLen != 0 )
					{
						if( m_pstBindParam[indPar].m_pwcData != NULL )
						{
							delete( m_pstBindParam[indPar].m_pwcData ) ;
							m_pstBindParam[indPar].m_pwcData = NULL ;
						}

						iLength = lLen ;
						m_pstBindParam[indPar].m_pwcData = (unsigned short*) M4CppToDB( pValue, iLength ) ;
						m_pstBindParam[indPar].m_lwcSize = iLength ;
						lLen = iLength * sizeof( unsigned short ) ;
					}
				}
			}
			else
			{
				datatype = SQLT_LBI;
			}

			if (!lLen)
			{
				lLen = M4LDB_SQL_NULL_DATA;
				*(ao_lLength) = M4LDB_SQL_NULL_DATA;
			}

			if (lLen == M4LDB_SQL_NULL_DATA)
			{
				m_pstBindParam[indPar].m_pInd = -1;
				lLen = 1;
				mode = OCI_DEFAULT;
			}
			else
			{
				m_pstBindParam[indPar].m_pInd = 0;
				mode = OCI_DATA_AT_EXEC;
			}

			pValue = NULL;
			break;
			
		case M4CL_M4_TYPE_NUMBER:
		case M4CL_M4_TYPE_CURRENCY:
		case M4CL_M4_TYPE_NUM_VARIANT:
		case M4CL_M4_TYPE_TIME_SPAN:

			datatype = SQLT_FLT;
			lLen = sizeof (double);
			break;
			
		case M4CL_M4_TYPE_DATE:
		case M4CL_M4_TYPE_TIMESTAMP:
		case M4CL_M4_TYPE_HOUR:

			datatype = SQLT_ODT;
			lLen = sizeof (OCIDate);
			pValue = (m4pchar_t) &m_pstBindParam[indPar].m_dDate;
			m_pstBindParam[indPar].m_iM4Type = M4CL_M4_TYPE_DATE;

			if (m_pstBindParam[indPar].m_pInd != -1)
			{
				OCIDate   *date;
				int       iYear, iMonth, iDay;
				m4int32_t iHour, iMinute, iSecond;
				
				date = &m_pstBindParam[indPar].m_dDate;
				ClDateToNumbers(*((m4date_t*) *(m_pstBindParam[indPar].m_ppBuf)), 
					iYear, 
					iMonth, 
					iDay, 
					iHour, 
					iMinute, 
					iSecond);
				
				OCIDateSetTime( date, (ub1)iHour, (ub1)iMinute, (ub1)iSecond );
				OCIDateSetDate( date, iYear, iMonth, iDay ); 
			}
			break;
			
		default:

			datatype = SQLT_STR;

			if (ai_lPrec == -3)
			{
				ai_lPrec = (m4int32_t)strlen (pValue);

				// UNICODE pending
				// si vienen a -3 ?????????
				// m_pstBindParam[indPar].m_lPrec = ai_lPrec;
			}

			lLen = ai_lPrec + 1;
			m_pstBindParam[indPar].m_iM4Type = M4CL_M4_TYPE_FIXED_STRING;

			// UNICODE DB
			if( M4IsUnicode() == 1 )
			{
				bSetCharacterSet = M4_TRUE ;

				if( m_pstBindParam[indPar].m_pwcData == NULL )
				{
					m_pstBindParam[indPar].m_pwcData = new unsigned short[ m_pstBindParam[indPar].m_lPrec + 1 ] ;
				}

				pValue = (m4pchar_t) m_pstBindParam[indPar].m_pwcData ;
				lLen *= sizeof( unsigned short ) ;

				iLength = *(m_pstBindParam[indPar].m_piLength) ;

				if( iLength != -1 )
				{
					if( iLength == -3 )
					{
						iLength = -1 ;
					}

					pwcValue = (unsigned short*) M4CppToDB( *(m_pstBindParam[indPar].m_ppBuf), iLength ) ;

					if( iLength > m_pstBindParam[indPar].m_lPrec )
					{
						iLength = m_pstBindParam[indPar].m_lPrec ;
					}

					memcpy( m_pstBindParam[indPar].m_pwcData, pwcValue, iLength * sizeof( unsigned short ) ) ;
					m_pstBindParam[indPar].m_pwcData[ iLength ] = '\0' ;

					DBWRAP_BIN_TRACE( "Uni", "Bind", iLength * sizeof( unsigned short ), m_pstBindParam[indPar].m_pwcData ) ;
					DBWRAP_BIN_TRACE( "Uni", "Len", lLen, NULL ) ;

					delete [] pwcValue ;
					pwcValue = NULL ;
				}
			}
#ifdef DBWRAP_TRACE
			else
			{
				iLength = *(m_pstBindParam[indPar].m_piLength) ;

				if( iLength != -1 )
				{
					if( iLength == -3 )
					{
						iLength = strlen( *(m_pstBindParam[indPar].m_ppBuf) ) ;
					}
					DBWRAP_BIN_TRACE( "Ansi", "Bind", iLength, *(m_pstBindParam[indPar].m_ppBuf) ) ;
					DBWRAP_BIN_TRACE( "Ansi", "Len", lLen, NULL ) ;
				}
			}
#endif

			break;
	}


	M4DBW_OCI_CALL( result = OCIBindByPos(	m_hStmt, 
											&m_pstBindParam[indPar].m_lBind, 
											m_pConn->m_hErr, 
											m_pstBindParam[indPar].m_indPar, 
											pValue, 
											lLen,
											(sword) datatype, 
											&m_pstBindParam[indPar].m_pInd,
											NULL,
											NULL,
											0,
											NULL, 
											mode ) );

	//UNICODE DB
	if( result == OCI_SUCCESS && bSetCharacterSet == M4_TRUE )
	{
		/*
		Hay que setear el tipo de campo para que no se transforme a la página de código
		When you bind or define SQL NCHAR datatypes, you should set the
		OCI_ATTR_CHARSET_FORM attribute to SQLCS_NCHAR.
		Otherwise, you can lose data because the data is converted to the database
		character set before converting to or from the national character set.
		This occurs only if the database character set is not Unicode.
		*/
		M4DBW_OCI_CALL( OCIAttrSet( m_pstBindParam[indPar].m_lBind, OCI_HTYPE_BIND, &iCForm, 0, OCI_ATTR_CHARSET_FORM, m_pConn->m_hErr ) );
	}

	return (M4DBW_OCI_ERROR(result)); 
#else
	return (M4_ERROR);
#endif
}


m4return_t ClCursor_oci::BindResult (m4int32_t ai_lCol, m4pchar_t *aio_ppResult, m4int32_t *aio_plMaxLen, m4int32_t ai_lM4Type, m4pint32_t ao_lLength, m4int32_t ai_lMaxLenFld)
{
#ifdef DRIVER_OCI

	sword		datatype;
	sword		result = OCI_SUCCESS;
	ub4			mode;
	ub1			iCForm = SQLCS_NCHAR;
	m4int32_t	indCol = ai_lCol-1;
	m4bool_t	bSetCharacterSet = M4_FALSE ;
	

	if (indCol >= m_lnumBindResult)
	{
		return (M4_ERROR);
	}

	if (!m_pstBindResult)
	{
		m_pstBindResult = new stBindResult[m_lnumBindResult];
		m_bInvExecFetch = M4_FALSE;
	}
	
	m_pstBindResult[indCol].m_pInd = new sb2[m_lNumRow];
	m_pstBindResult[indCol].m_pLen = new ub2[m_lNumRow];
	m_pstBindResult[indCol].m_pRcode = new ub2[m_lNumRow];

	m_pstBindResult[indCol].m_iM4Type = (m4int8_t) ai_lM4Type;
	m_pstBindResult[indCol].m_lSize = *aio_plMaxLen + 1;
	m_pstBindResult[indCol].m_ppcResult = aio_ppResult;
	m_pstBindResult[indCol].m_piLength = ao_lLength;
	m_pstBindResult[indCol].m_piMaxLength = aio_plMaxLen;

	mode = OCI_DEFAULT;

	switch (ai_lM4Type)
	{
		case M4CL_M4_TYPE_BLOB:
		case M4CL_M4_TYPE_BINARY_STRING:

			m_bLongExist = M4_TRUE;
			datatype = SQLT_LBI;
			mode = OCI_DYNAMIC_FETCH;
			m_pstBindResult[indCol].m_lSize = LONG_MAX;
			m_pstBindResult[indCol].m_pBuf = (ub1*) *aio_ppResult;
			m_bInvExecFetch = M4_TRUE;
			break;
			
		case M4CL_M4_TYPE_LONG:
		case M4CL_M4_TYPE_UNI_LONG:

			m_bLongExist = M4_TRUE;

			// UNICODE DB
			if( M4IsUnicode() == 1 )
			{
				bSetCharacterSet = M4_TRUE ;
			}

			datatype = SQLT_LNG;
			mode = OCI_DYNAMIC_FETCH;
			m_pstBindResult[indCol].m_lSize = LONG_MAX;
			m_pstBindResult[indCol].m_pBuf = (ub1*) *aio_ppResult;
			m_bInvExecFetch = M4_TRUE;
			break;
			
		case M4CL_M4_TYPE_NUMBER:
		case M4CL_M4_TYPE_CURRENCY:
		case M4CL_M4_TYPE_NUM_VARIANT:
		case M4CL_M4_TYPE_TIME_SPAN:

			datatype = SQLT_FLT;	
			m_pstBindResult[indCol].m_lSize = sizeof (m4double_t);
			m_pstBindResult[indCol].m_pBuf = (ub1*) new m4double_t[m_lNumRow];
			break;
			
		case M4CL_M4_TYPE_DATE:
		case M4CL_M4_TYPE_TIMESTAMP:
		case M4CL_M4_TYPE_HOUR:

			datatype = SQLT_ODT;
			m_pstBindResult[indCol].m_lSize = sizeof (OCIDate);
			m_pstBindResult[indCol].m_iM4Type = M4CL_M4_TYPE_DATE;
			m_pstBindResult[indCol].m_pBuf = (ub1*) new OCIDate[m_lNumRow];
			break;
			
		default:

			if ((*aio_plMaxLen + 1) < ai_lMaxLenFld)
			{
				// Esto es para evitar la mayoria de los truncados.
				m_pstBindResult[indCol].m_lSize = ai_lMaxLenFld;
				m_pstBindResult[indCol].m_lSizeItem = *aio_plMaxLen + 1;
			}
			datatype = SQLT_STR;
			m_pstBindResult[indCol].m_iM4Type = M4CL_M4_TYPE_FIXED_STRING;

			// UNICODE DB
			if( M4IsUnicode() == 1 )
			{
				bSetCharacterSet = M4_TRUE ;

				m_pstBindResult[indCol].m_pBuf = (ub1*) new unsigned short[m_lNumRow*m_pstBindResult[indCol].m_lSize];
				m_pstBindResult[indCol].m_lSize *= sizeof( unsigned short );
			}
			else
			{
				m_pstBindResult[indCol].m_pBuf = (ub1*) new m4char_t[m_lNumRow*m_pstBindResult[indCol].m_lSize];
			}

			break;
	}
	
	for (m4int32_t indRow=0; indRow<m_lNumRow; indRow++)
	{
		m_pstBindResult[indCol].m_pInd[indRow] = -1;
		m_pstBindResult[indCol].m_pLen[indRow] = -1;
		m_pstBindResult[indCol].m_pRcode[indRow] = OCI_NO_DATA;
	}	
	
		
	/* Output values for ao_lLength
	-2  The Length of the item is greater than the Length of the output variable; the item has been truncated. Additionally, the original Length is longer than the maximum data Length that can be returned in the sb2 indicator variable. 
	-1  The selected value is null, and the value of the output variable is unchanged. 
	0  Oracle assigned an intact value to the host variable. 
	>0  The Length of the item is greater than the Length of the output variable; the item has been truncated. 
    The positive value returned in the indicator variable is the actual Length before truncation. 
	*/
	
	M4DBW_OCI_CALL( result = OCIDefineByPos(	m_hStmt, 
												&m_pstBindResult[indCol].m_lBind,
												m_pConn->m_hErr, 
												ai_lCol,
												(dvoid *) m_pstBindResult[indCol].m_pBuf,
												m_pstBindResult[indCol].m_lSize, 
												datatype, 
												m_pstBindResult[indCol].m_pInd,
												m_pstBindResult[indCol].m_pLen,
												m_pstBindResult[indCol].m_pRcode,
												mode ) );

	//UNICODE DB
	if( result == OCI_SUCCESS && bSetCharacterSet == M4_TRUE )
	{
		/*
		Hay que setear el tipo de campo para que no se transforme a la página de código
		When you bind or define SQL NCHAR datatypes, you should set the
		OCI_ATTR_CHARSET_FORM attribute to SQLCS_NCHAR.
		Otherwise, you can lose data because the data is converted to the database
		character set before converting to or from the national character set.
		This occurs only if the database character set is not Unicode.
		*/
		M4DBW_OCI_CALL( OCIAttrSet( m_pstBindResult[indCol].m_lBind, OCI_HTYPE_DEFINE, &iCForm, 0, OCI_ATTR_CHARSET_FORM, m_pConn->m_hErr ) );
	}

	return (M4DBW_OCI_ERROR(result)); 
#else
	return (M4_ERROR);
#endif
}


m4return_t ClCursor_oci::Execute( m4uint32_t ai_iTimeOut, ICancelStatusProvider* ai_poCancelStatus, m4return_t& ao_riCanceled )
{
	ao_riCanceled = 0;

#ifdef DRIVER_OCI

	sword			result = OCI_SUCCESS;
	ub4				iters, iters2, mode;
	ub4				type, idx;
	ub1				inout, piece;
	void*			hndlbind;
	int				iLength = 0 ;
	unsigned short* pwcValue = NULL;


	for (m4int32_t indPar=0; indPar<m_lnumBindParam; indPar++)
	{
		if (m_bRebindAllParameters)
		{
			result = BindParam (indPar + 1, m_pstBindParam[indPar].m_pBuf, m_pstBindParam[indPar].m_ppBuf, m_pstBindParam[indPar].m_lPrec, m_pstBindParam[indPar].m_iM4Type, m_pstBindParam[indPar].m_lScale, m_pstBindParam[indPar].m_piLength);

			if (result == OCI_ERROR)
			{
				return (M4DBW_OCI_ERROR(result));
			}
		}
		else
		{
			m_pstBindParam[indPar].m_pInd = (short) *(m_pstBindParam[indPar].m_piLength);
			
			if (m_pstBindParam[indPar].m_pInd == -3)
			{
				m_pstBindParam[indPar].m_pInd = 0;
			}
			
			//Llenamos la estructura OCIDate, con la fecha correcta.
			//En tipos Fecha, no bindamos el buffer 'M4' si no la 
			//estructura de OCI, por tanto, debemos actualizarla
			//antes del execute.
			if ((m_pstBindParam[indPar].m_iM4Type == M4CL_M4_TYPE_DATE ||
				 m_pstBindParam[indPar].m_iM4Type == M4CL_M4_TYPE_TIMESTAMP ||
				 m_pstBindParam[indPar].m_iM4Type == M4CL_M4_TYPE_HOUR) &&
				 m_pstBindParam[indPar].m_pInd != -1)
			{
				OCIDate   *date;
				int       iYear, iMonth, iDay;
				m4int32_t iHour, iMinute, iSecond;
				
				date = &m_pstBindParam[indPar].m_dDate;
				ClDateToNumbers(*((m4date_t*) *(m_pstBindParam[indPar].m_ppBuf)), 
					iYear, 
					iMonth, 
					iDay, 
					iHour, 
					iMinute, 
					iSecond);
				
				OCIDateSetTime( date, (ub1)iHour, (ub1)iMinute, (ub1)iSecond );
				OCIDateSetDate( date, iYear, iMonth, iDay );
			}

			if( m_pstBindParam[indPar].m_iM4Type == M4CL_M4_TYPE_FIXED_STRING ||
				m_pstBindParam[indPar].m_iM4Type == M4CL_M4_TYPE_VAR_STRING ||
				m_pstBindParam[indPar].m_iM4Type == M4CL_M4_TYPE_UNI_VAR_STRING ||
				m_pstBindParam[indPar].m_iM4Type == M4CL_M4_TYPE_VARIANT ||
				m_pstBindParam[indPar].m_iM4Type == M4CL_M4_TYPE_UNI_VARIANT )
			{
				// UNICODE DB
				if( M4IsUnicode() == 1 )
				{
					iLength = *(m_pstBindParam[indPar].m_piLength) ;

					if( iLength != -1 )
					{
						if( iLength == -3 )
						{
							iLength = -1 ;
						}

						pwcValue = (unsigned short*) M4CppToDB( *(m_pstBindParam[indPar].m_ppBuf), iLength ) ;

						if( iLength > m_pstBindParam[indPar].m_lPrec )
						{
							iLength = m_pstBindParam[indPar].m_lPrec ;
						}

						memcpy( m_pstBindParam[indPar].m_pwcData, pwcValue, iLength * sizeof( unsigned short ) ) ;
						m_pstBindParam[indPar].m_pwcData[ iLength ] = '\0' ;

						DBWRAP_BIN_TRACE( "Uni", "Param", iLength * sizeof( unsigned short ), m_pstBindParam[indPar].m_pwcData ) ;

						delete [] pwcValue ;
						pwcValue = NULL ;
					}
				}
#ifdef DBWRAP_TRACE
				else
				{
					iLength = *(m_pstBindParam[indPar].m_piLength) ;

					if( iLength != -1 )
					{
						if( iLength == -3 )
						{
							iLength = strlen( *(m_pstBindParam[indPar].m_ppBuf) ) ;
						}
						DBWRAP_BIN_TRACE( "Ansi", "Param", iLength, *(m_pstBindParam[indPar].m_ppBuf) ) ;
					}
				}
#endif
			}
		}
	}

	iters = (!m_bIsSelect) ? 1 : ((m_bInvExecFetch) ? 0 : m_lNumRow);
	
	mode = OCI_DEFAULT;
	if (m_pConn->m_bAutoCommit && !m_bIsSelect)
	{
		mode = OCI_COMMIT_ON_SUCCESS;
	}

	if( ai_iTimeOut > 0 )
	{
		m4uint32_t		iTimes = 0 ;
		m4millisec_t	endTime = ClPrecisionTicker::GetTimeTick() + ai_iTimeOut * 1000 ;

		do
		{
			result = OCIStmtExecute( m_pConn->m_hSvC, m_hStmt, m_pConn->m_hErr, iters, 0, NULL, NULL, mode ) ;

			if( result == OCI_STILL_EXECUTING )
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
						M4DBW_OCI_CALL( OCIBreak( m_pConn->m_hSvC, m_pConn->m_hErr ) ) ;
						M4DBW_OCI_CALL( result = OCIReset( m_pConn->m_hSvC, m_pConn->m_hErr ) ) ;
						break;
					}
				}
				m4usleep( 500 ) ;
			}
		}
		while( result == OCI_STILL_EXECUTING ) ;
	}
	else
	{
		M4DBW_OCI_CALL( result = OCIStmtExecute( m_pConn->m_hSvC, m_hStmt, m_pConn->m_hErr, iters, 0, NULL, NULL, mode ) );
	}
	
	m_lRemainder = iters;
	
	if (result == OCI_NO_DATA)
	{
		result = OCI_SUCCESS;
	}
	
	while (result == OCI_NEED_DATA)
	{
		/* Bug 0169035
		Se utiliza una variable iters2 distinta para no perder el valor de iters
		*/
		M4DBW_OCI_CALL( result = OCIStmtGetPieceInfo( m_hStmt, m_pConn->m_hErr, &hndlbind, &type, &inout, &iters2, &idx, &piece ) );
		
		if (result != OCI_SUCCESS)
		{
			return (M4DBW_OCI_ERROR(result));
		}
		else
		{
			m4int32_t indPar;
			ub2 rcode;
			ub4 lSize;

			indPar = -1;
			for (idx = 0; idx < (ub4) m_lnumBindParam; idx++)
			{
				if (m_pstBindParam[idx].m_lBind == hndlbind)
				{
					indPar = idx;
					break;
				}
			}
				
			m4int32_t lLength = *(m_pstBindParam[indPar].m_piLength);

			if (lLength == M4LDB_SQL_NULL_DATA)
			{
				lSize = 0;
				m_pstBindParam[indPar].m_pInd = -1;
				piece = OCI_ONE_PIECE;
				
				M4DBW_OCI_CALL( result = OCIStmtSetPieceInfo( hndlbind, type, m_pConn->m_hErr, NULL, &lSize, piece, &m_pstBindParam[indPar].m_pInd, &rcode ) );
				
				if (result != OCI_SUCCESS)
				{
					return (M4DBW_OCI_ERROR(result));
				}

				M4DBW_OCI_CALL( result = OCIStmtExecute( m_pConn->m_hSvC, m_hStmt, m_pConn->m_hErr, iters, 0, NULL, NULL, OCI_DEFAULT ) );
			}
			else
			{
				m4int32_t m4type = m_pstBindParam[indPar].m_iM4Type;

				if (m4type == M4CL_M4_TYPE_LONG || m4type == M4CL_M4_TYPE_UNI_LONG || m4type == M4CL_M4_TYPE_BINARY_STRING)
				{
					m4pchar_t pValue = *(m_pstBindParam[indPar].m_ppBuf);
					m4int32_t lLen;
					m4int32_t lStep = M4LDB_SIZE_LONG;

					// UNICODE DB
					// Si es un long se pasa a unicode
					if( ( m4type == M4CL_M4_TYPE_LONG || m4type == M4CL_M4_TYPE_UNI_LONG ) && M4IsUnicode() == 1 )
					{
						pwcValue = m_pstBindParam[indPar].m_pwcData ;
						// Bug 0299345 La longitud debe ser física, no lógica
						lLength = m_pstBindParam[indPar].m_lwcSize ;
					}
					else
					{
						pwcValue = NULL ;
					}

					piece = OCI_FIRST_PIECE;
					
					if (lLength < lStep)
					{
						lStep = lLength;
						piece = OCI_ONE_PIECE;
					}
					
					for (lLen=0; lLen<lLength; lLen += lStep)
					{
						lSize = lStep;
						
						if (lLen+lStep >= lLength)
						{
							if (piece == OCI_FIRST_PIECE || piece == OCI_NEXT_PIECE)
							{
								piece = OCI_LAST_PIECE;
							}
							lSize = lLength - lLen;
						}
						else
						{
							if (lLen > 0)
							{
								piece = OCI_NEXT_PIECE;
							}
						}
						
						// UNICODE DB
						if( pwcValue != NULL )
						{
							lSize *= sizeof( unsigned short ) ;
							M4DBW_OCI_CALL( result = OCIStmtSetPieceInfo( hndlbind, type, m_pConn->m_hErr, pwcValue+lLen, &lSize, piece, &m_pstBindParam[indPar].m_pInd, &rcode ) );
						}
						else
						{
							M4DBW_OCI_CALL( result = OCIStmtSetPieceInfo( hndlbind, type, m_pConn->m_hErr, pValue+lLen, &lSize, piece, &m_pstBindParam[indPar].m_pInd, &rcode ) );
						}
						
						if (result != OCI_SUCCESS)
						{
							return (M4DBW_OCI_ERROR(result));
						}
						
						M4DBW_OCI_CALL( result = OCIStmtExecute( m_pConn->m_hSvC, m_hStmt, m_pConn->m_hErr, iters, 0, NULL, NULL, OCI_DEFAULT ) );
						
						if (result != OCI_NEED_DATA)
						{
							break;
						}
					}
				}
				else
				{
					m4char_t aValue[M4LDB_SIZE_LONG+1];
					
					m4pchar_t pNameFile = *(m_pstBindParam[indPar].m_ppBuf);
					
					FILE *pFile = NULL;

					lSize = M4LDB_SIZE_LONG;

					if (*pNameFile)
					{
						// UNICODE FILE
						eUniFileEncodingType_t eEncoding = M4UniANSI ;
						pFile = M4Fopen( pNameFile, M4UniReadBinary, eEncoding ) ;

					}
						
					if (!pFile)
					{
						lSize = 0;
						m_pstBindParam[indPar].m_pInd = -1;
						piece = OCI_ONE_PIECE;

						M4DBW_OCI_CALL( result = OCIStmtSetPieceInfo(hndlbind, type, m_pConn->m_hErr, NULL, &lSize, piece, &m_pstBindParam[indPar].m_pInd, &rcode ) );

						if (result != OCI_SUCCESS)
						{
							return (M4DBW_OCI_ERROR(result));
						}

						M4DBW_OCI_CALL( result = OCIStmtExecute( m_pConn->m_hSvC, m_hStmt, m_pConn->m_hErr, iters, 0, NULL, NULL, OCI_DEFAULT ) );
						continue;
					}

					/* Bug 0159752, 0173439
					Hay que guardar todo el fichero, y puede que la cabecera se haya saltado
					*/
					fseek( pFile, 0, SEEK_SET ) ;
						
					piece = OCI_FIRST_PIECE;

					// Buscamos la descripción. Formato buffer: filename\0HEADER+descr\0ext\0
					m4pchar_t pFileDescr = pNameFile + strlen(pNameFile) + 1;
					lSize = ub4( strlen (pFileDescr) + 1 );

					// buscamos la extensión (con header)
					m4pchar_t pcExt = pFileDescr + strlen(pFileDescr) + 1;

					// apenda la extensión
					lSize += ub4( strlen(pcExt) + 1 );

					// Formato del BLOB en BD : HEADER+Descripción\0\Ext\0Fichero
					M4DBW_OCI_CALL( result = OCIStmtSetPieceInfo( hndlbind, type, m_pConn->m_hErr, pFileDescr, &lSize, piece, &m_pstBindParam[indPar].m_pInd, &rcode ) );
					
					if (result != OCI_SUCCESS)
					{
						fclose(pFile);
						return (M4DBW_OCI_ERROR(result));
					}
					
					M4DBW_OCI_CALL( result = OCIStmtExecute( m_pConn->m_hSvC, m_hStmt, m_pConn->m_hErr, iters, 0, NULL, NULL, OCI_DEFAULT ) );
					
					if (result != OCI_NEED_DATA)
					{
						fclose(pFile);
						return (M4DBW_OCI_ERROR(result));
					}
					
					piece = OCI_NEXT_PIECE;
					
					/* Bug 0206455
					Se debe garantizar que se sale si y solo si se ha hecho el OCI_LAST_PIECE
					*/
					do {
						lLength = (m4int32_t) fread ((m4pvoid_t) aValue, sizeof (m4char_t), M4LDB_SIZE_LONG, pFile);

						if (lLength < M4LDB_SIZE_LONG)
						{
							piece = OCI_LAST_PIECE;
						}

						M4DBW_OCI_CALL( result = OCIStmtSetPieceInfo( hndlbind, type, m_pConn->m_hErr, aValue, (ub4*) &lLength, piece, &m_pstBindParam[indPar].m_pInd, &rcode ) );
						
						if (result != OCI_SUCCESS)
						{
							fclose(pFile);
							return (M4DBW_OCI_ERROR(result));
						}
						
						M4DBW_OCI_CALL( result = OCIStmtExecute( m_pConn->m_hSvC, m_hStmt, m_pConn->m_hErr, iters, 0, NULL, NULL, OCI_DEFAULT ) );
						
						if (result == OCI_ERROR)
						{
							fclose(pFile);
							return (M4DBW_OCI_ERROR(result));
						}

					} while( piece != OCI_LAST_PIECE );
					
					fclose (pFile);
				}
		    }
		}
	}
	
	return (M4DBW_OCI_ERROR(result));

#else
	return (M4_ERROR);
#endif
}


m4return_t ClCursor_oci::Fetch (void)
{
#ifdef DRIVER_OCI

	m4int32_t	indCol;
	sword		result = OCI_SUCCESS;
	m4int32_t	lLen = 0;
	int			iLength = 0 ;
	m4pchar_t	pcValue = NULL;
	m4int32_t	iMaxLen = 0;


	if (!m_pstBindResult)
	{
		// Estamos en una SELECT NULL.
		m_lnumBindResult = 1;
		m_lNumRow = 1;

		pcValue = NULL;
		iMaxLen = 0;
		result = BindResult (1, &pcValue, &iMaxLen, M4CL_M4_TYPE_NUMBER, NULL);

		if (result == OCI_ERROR)
		{
			return (M4DBW_OCI_ERROR(result));
		}
	}


	if (m_bLongExist)
	{
		for (m4int32_t indCol=0; indCol<m_lnumBindResult; indCol++)
		{
			if ((m_pstBindResult[indCol].m_iM4Type == M4CL_M4_TYPE_LONG || 
				 m_pstBindResult[indCol].m_iM4Type == M4CL_M4_TYPE_UNI_LONG || 
				 m_pstBindResult[indCol].m_iM4Type == M4CL_M4_TYPE_BLOB || 
				 m_pstBindResult[indCol].m_iM4Type == M4CL_M4_TYPE_BINARY_STRING) && 
				!m_pstBindResult[indCol].m_bLongRead)
			{
				// Hay resultados pendientes por lo que los desechamos.

				result = ReadLong (indCol + 1);

				if (result == OCI_ERROR)
				{
					return (M4DBW_OCI_ERROR(result));
				}
			}
		}
	}


	if (!m_lRemainder)
	{
		m_lRemainder = 	m_lNumRow;
		M4DBW_OCI_CALL( result = OCIStmtFetch( m_hStmt, m_pConn->m_hErr, m_lRemainder, OCI_FETCH_NEXT, OCI_DEFAULT ) );
	}

	if (result == OCI_ERROR) {
		return (M4DBW_OCI_ERROR(result));
	}


	m_lRemainder--;
	
//	m4int32_t indRow = ((m_bInvExecFetch) ? 0 : m_lNumRow-m_lRemainder-1);
	m4int32_t indRow = m_lNumRow-m_lRemainder-1;
			
	m4return_t result1 = OCI_NO_DATA;

	m4bool_t bExistsLong = M4_FALSE;
	m4bool_t bExistsNoLong = M4_FALSE;

	for (indCol=0; indCol<m_lnumBindResult; indCol++)
	{
		ub2 result2 = m_pstBindResult[indCol].m_pRcode[indRow];

		if (result2 == OCI_ERROR)
		{
			return (M4DBW_OCI_ERROR (OCI_ERROR));
		}

		if (result2 != OCI_NO_DATA)
		{
			if (result2 == 1406)
			{
				result1 = OCI_SUCCESS_WITH_INFO;
			}

			if (result1 != OCI_SUCCESS_WITH_INFO)
			{
				result1 = OCI_SUCCESS;
			}

			// Si el puntero a la longitud es nulo significa que no queremos el valor.
			if (m_pstBindResult[indCol].m_piLength)
			{
				*(m_pstBindResult[indCol].m_piLength) = (m4int32_t) m_pstBindResult[indCol].m_pLen[indRow];

				if (m_pstBindResult[indCol].m_pInd[indRow] != -1)
				{
					switch (m_pstBindResult[indCol].m_iM4Type)
					{
						case M4CL_M4_TYPE_FIXED_STRING:
						case M4CL_M4_TYPE_VAR_STRING:
						case M4CL_M4_TYPE_UNI_VAR_STRING:
						case M4CL_M4_TYPE_VARIANT:
						case M4CL_M4_TYPE_UNI_VARIANT:

							if (m_pstBindResult[indCol].m_lSizeItem != -1)
							{
								lLen = (*(m_pstBindResult[indCol].m_piLength) < (m_pstBindResult[indCol].m_lSizeItem-1)) ? *(m_pstBindResult[indCol].m_piLength) : m_pstBindResult[indCol].m_lSizeItem-1;
							}
							else
							{
								lLen = (*(m_pstBindResult[indCol].m_piLength) < (m_pstBindResult[indCol].m_lSize-1)) ? *(m_pstBindResult[indCol].m_piLength) : m_pstBindResult[indCol].m_lSize-1;
							}

							// UNICODE DB
							if( M4IsUnicode() == 1 )
							{
								iLength = lLen ;
								pcValue = M4DBToCpp( &m_pstBindResult[indCol].m_pBuf[indRow*m_pstBindResult[indCol].m_lSize], iLength ) ;

								if( *(m_pstBindResult[indCol].m_ppcResult) != NULL && iLength > *(m_pstBindResult[indCol].m_piMaxLength) )
								{
									// Si el buffer de salida es pequeño se elimina
									delete [] *(m_pstBindResult[indCol].m_ppcResult) ;
									*(m_pstBindResult[indCol].m_piMaxLength) = 0 ;
									*(m_pstBindResult[indCol].m_ppcResult) = NULL ;
								}

								if( *(m_pstBindResult[indCol].m_ppcResult) == NULL )
								{
									// Si no hay buffer de salida se crea uno nuevo
									*(m_pstBindResult[indCol].m_piMaxLength) = iLength ;
									*(m_pstBindResult[indCol].m_ppcResult) = new char[ iLength + 1 ] ;
								}

								memcpy( *(m_pstBindResult[indCol].m_ppcResult), pcValue, iLength * sizeof( m4char_t ) ) ;
								(*(m_pstBindResult[indCol].m_ppcResult))[ iLength ] = '\0' ;
								delete [] pcValue ;
								pcValue = NULL ;

								*(m_pstBindResult[indCol].m_piLength) = iLength ;
							}
							else
							{
								memcpy (*(m_pstBindResult[indCol].m_ppcResult), (const void*) &m_pstBindResult[indCol].m_pBuf[indRow*m_pstBindResult[indCol].m_lSize], lLen);
								(*(m_pstBindResult[indCol].m_ppcResult))[lLen] = '\0';
								*(m_pstBindResult[indCol].m_piLength) = lLen;
							}

							break;

						case M4CL_M4_TYPE_DATE:
						case M4CL_M4_TYPE_TIMESTAMP:
						case M4CL_M4_TYPE_HOUR:
							{
								OCIDate   *date;
								sb2  iYear;
								ub1  iMonth, iDay;
								ub1  iHour, iMinute, iSecond;
								m4date_t m4date;
								
								date = (OCIDate*) &m_pstBindResult[indCol].m_pBuf[indRow*m_pstBindResult[indCol].m_lSize];
								
								OCIDateGetTime( date, &iHour, &iMinute, &iSecond );
								OCIDateGetDate( date, &iYear, &iMonth, &iDay ); 
								
								ClNumbersToDate (	iYear, 
													iMonth, 
													iDay, 
													iHour, 
													iMinute, 
													iSecond,
													m4date);

								*((m4date_t*) *(m_pstBindResult[indCol].m_ppcResult)) = m4date;
							}
							break;

						case M4CL_M4_TYPE_NUMBER:
						case M4CL_M4_TYPE_CURRENCY:
						case M4CL_M4_TYPE_NUM_VARIANT:
						case M4CL_M4_TYPE_TIME_SPAN:

							*((m4double_t*) *(m_pstBindResult[indCol].m_ppcResult)) = *((m4double_t*) &m_pstBindResult[indCol].m_pBuf[indRow*m_pstBindResult[indCol].m_lSize]);
							break;

						case M4CL_M4_TYPE_LONG:
						case M4CL_M4_TYPE_UNI_LONG:
						case M4CL_M4_TYPE_BLOB:
						case M4CL_M4_TYPE_BINARY_STRING:

							if (result != OCI_NEED_DATA)
							{
								// No hay resultados LONG pendientes.
								m_pstBindResult[indCol].m_bLongRead = M4_TRUE;
							}
							else
							{
								m_pstBindResult[indCol].m_bLongRead = M4_FALSE;
							}
							break;
					}
				}
				else
				{
					*(m_pstBindResult[indCol].m_piLength) = -1;
				}
			}
		}
		else
		{
			if (m_pstBindResult[indCol].m_iM4Type == M4CL_M4_TYPE_LONG || 
				m_pstBindResult[indCol].m_iM4Type == M4CL_M4_TYPE_UNI_LONG || 
				m_pstBindResult[indCol].m_iM4Type == M4CL_M4_TYPE_BLOB || 
				m_pstBindResult[indCol].m_iM4Type == M4CL_M4_TYPE_BINARY_STRING)
			{
				if (result != OCI_NEED_DATA)
				{
					// No hay resultados LONG pendientes.
					m_pstBindResult[indCol].m_bLongRead = M4_TRUE;
				}
				else
				{
					m_pstBindResult[indCol].m_bLongRead = M4_FALSE;
				}
				bExistsLong = M4_TRUE;
			}
			else
			{
				bExistsNoLong = M4_TRUE;
			}
		}

		m_pstBindResult[indCol].m_pRcode[indRow] = OCI_NO_DATA;
	}

	if (bExistsLong && !bExistsNoLong && result != OCI_NO_DATA && result1 == OCI_NO_DATA)
	{ 
		result1 = OCI_SUCCESS;
	}

	/*
	if ((bExistsLong || bExistsNoLong) && result == OCI_NO_DATA && result1 != OCI_NO_DATA)
	{ 
		result1 = OCI_NO_DATA;
	}
	*/

	return (M4DBW_OCI_ERROR (result1));

#else
	return (M4_ERROR);
#endif
}


m4return_t ClCursor_oci::GetData (m4int32_t ai_lCol, m4pchar_t *aio_ppResult, m4int32_t *aio_plMaxLen, m4int32_t ai_lM4Type, m4int32_t *ao_lLength, m4bool_t ai_bByPass)
{
	*ao_lLength = 0 ;

	if( *aio_ppResult != NULL )
	{
		**aio_ppResult = '\0' ;
	}

#ifdef DRIVER_OCI

	ub4   type, iter, idx;
	ub1   inout, piece;
	
	void			*hndlbind;
	sword			result = OCI_SUCCESS;
	sb2				ind = 0;
	ub2				rcode;
	m4int32_t		indCol = ai_lCol - 1;
	ub4				lSize;
	int				iLength = 0 ;
	m4pchar_t		pcValue = NULL ;
	m4pchar_t		pcUniValue = NULL ;
	unsigned short	aValue[ M4LDB_SIZE_LONG / sizeof( unsigned short ) + 1 ] ;


	if (m_pstBindResult[indCol].m_bLongRead)
	{
		return (M4DBW_OCI_ERROR(OCI_NO_DATA));
	}

	M4DBW_OCI_CALL( result = OCIStmtGetPieceInfo( m_hStmt, m_pConn->m_hErr, &hndlbind, &type, &inout, &iter, &idx, &piece ) );
	
	if (result != OCI_SUCCESS)
	{
		return (M4DBW_OCI_ERROR(result));
	}
	
	if (m_pstBindResult[indCol].m_lBind != hndlbind)
	{
		m_pstBindResult[indCol].m_bLongRead = M4_TRUE;
		return (M4DBW_OCI_ERROR(OCI_NO_DATA));
	}
    
	lSize = M4LDB_SIZE_LONG;

	M4DBW_OCI_CALL( result = OCIStmtSetPieceInfo( m_pstBindResult[indCol].m_lBind, type, m_pConn->m_hErr, aValue, &lSize, piece, &ind, &rcode ) );

	if (result == OCI_ERROR) {
		return (M4_ERROR);
	}

	M4DBW_OCI_CALL( result = OCIStmtFetch( m_hStmt, m_pConn->m_hErr, 1, OCI_FETCH_NEXT, OCI_DEFAULT ) );

	m_pstBindResult[indCol].m_pRcode[0] = OCI_NO_DATA;

	/* Bug 0225339
	Hay que tratar el success with info
	*/
	if( result == OCI_SUCCESS || result == OCI_SUCCESS_WITH_INFO || result == OCI_NEED_DATA )
	{
		*ao_lLength = ((ind == -1) ? -1 : (m4int32_t) (abs ((m4int_t)lSize)));

		iLength = *ao_lLength ;
		pcValue = (m4pchar_t) aValue ;

		if( iLength > 0 )
		{
			switch( ai_lM4Type )
			{
				case M4CL_M4_TYPE_LONG:
				case M4CL_M4_TYPE_UNI_LONG:
				case M4CL_M4_TYPE_FIXED_STRING:
				case M4CL_M4_TYPE_VAR_STRING:
				case M4CL_M4_TYPE_UNI_VAR_STRING:
				case M4CL_M4_TYPE_VARIANT:
				case M4CL_M4_TYPE_UNI_VARIANT:

				// UNICODE DB
				if( M4IsUnicode() == 1 )
				{
					iLength /= sizeof( unsigned short ) ;
					pcUniValue = M4DBToCpp( aValue, iLength ) ;
					pcValue = pcUniValue  ;
				}
				break;
			}

			if( *aio_ppResult != NULL && iLength > *aio_plMaxLen )
			{
				// Si el buffer es pequeño se crea uno nuevo
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

			if( pcUniValue != NULL )
			{
				delete [] pcUniValue ;
			}
		}
	}

	switch (result)
	{
		case OCI_NEED_DATA : 
			return M4DBW_OCI_ERROR(OCI_SUCCESS);
			break;

		case OCI_NO_DATA : 
			*ao_lLength = 0;
			// No usamos break.

		case OCI_SUCCESS : 
		case OCI_SUCCESS_WITH_INFO : 
			m_pstBindResult[indCol].m_bLongRead = M4_TRUE;
			break;
	}


	return M4DBW_OCI_ERROR(result);

#else
	return (M4_ERROR);
#endif
}


m4return_t ClCursor_oci::DescribeCol (m4int32_t ai_lCol, m4pint32_t ao_lPrec, m4pint8_t ao_cM4Type)

{
#ifdef DRIVER_OCI

	sword		result = OCI_SUCCESS;
	OCIParam*	parmdp; 
	ub2			datatype;
	ub2			prec;

	
	M4DBW_OCI_CALL( result = OCIParamGet( (dvoid *)m_hStmt, OCI_HTYPE_STMT, m_pConn->m_hErr, (void **)&parmdp, ai_lCol ) );

	if (result != OCI_SUCCESS)
	{
		return (M4DBW_OCI_ERROR(result));
	}
	
	M4DBW_OCI_CALL( result = OCIAttrGet( (dvoid*) parmdp, OCI_DTYPE_PARAM, (dvoid*) &datatype, NULL, OCI_ATTR_DATA_TYPE, m_pConn->m_hErr ) );

	if (result != OCI_SUCCESS)
	{
		return (M4DBW_OCI_ERROR(result));
	}

	*ao_cM4Type = OracleToM4Type (datatype);
	
	if (*ao_cM4Type == M4CL_M4_TYPE_LONG || *ao_cM4Type == M4CL_M4_TYPE_UNI_LONG || *ao_cM4Type == M4CL_M4_TYPE_BINARY_STRING)
	{
		m_lNumRow = 1;
	}
	
	if (*ao_cM4Type == M4CL_M4_TYPE_FIXED_STRING)
	{
		M4DBW_OCI_CALL( result = OCIAttrGet( (dvoid*) parmdp, OCI_DTYPE_PARAM, (dvoid*) &prec, NULL, OCI_ATTR_DATA_SIZE, m_pConn->m_hErr ) );

		if (result != OCI_SUCCESS) {
			return (M4DBW_OCI_ERROR(result));
		}

		*ao_lPrec = prec;
	}
	else
	{
		*ao_lPrec = 0;
	}
		
	return (M4DBW_OCI_ERROR (OCI_SUCCESS));
#else
	return (M4_ERROR);
#endif
}


m4return_t ClCursor_oci::SetBlockingMode( m4uint32_t ai_iMode )
{
#ifdef DRIVER_OCI

	sword	result = OCI_SUCCESS;
	ub4		iMode = 0;

	M4DBW_OCI_CALL( result = OCIAttrGet( m_pConn->m_hSrv, OCI_HTYPE_SERVER, (dvoid*) &iMode, NULL, OCI_ATTR_NONBLOCKING_MODE, m_pConn->m_hErr ) ) ;

	if( result != OCI_SUCCESS || ai_iMode != iMode )
	{
		M4DBW_OCI_CALL( result = OCIAttrSet( m_pConn->m_hSrv, OCI_HTYPE_SERVER, NULL, 0, OCI_ATTR_NONBLOCKING_MODE, m_pConn->m_hErr ) ) ;
	}

	return (M4DBW_OCI_ERROR (result));
#else
	return (M4_ERROR);
#endif
}



#ifdef DRIVER_OCI
m4int8_t ClCursor_oci::OracleToM4Type (OCITypeCode datatype)
{
	switch (datatype)
	{
		case SQLT_DAT :     //Date
		case SQLT_ODT :     //Oracle Date
			return (M4CL_M4_TYPE_TIMESTAMP);
			
		case SQLT_NUM :     //Numeric
		case SQLT_INT :     //Integer
		case SQLT_UIN :     //Unsigned Integer
		case SQLT_FLT :     //Float
		case SQLT_VNU :     //Var Num
			return (M4CL_M4_TYPE_NUMBER);
			
		case SQLT_LNG :     //Long
		case SQLT_LVC :     //Long Var Char
		case SQLT_CLOB :    //Character lob
			return (M4CL_M4_TYPE_LONG);
			
		case SQLT_BIN :     //Raw
		case SQLT_VBI :     //Var Raw
		case SQLT_LVB :     //Long Var Raw
		case SQLT_LBI :		//Long Binary
		case SQLT_BLOB :	//Binary lob
			return (M4CL_M4_TYPE_BINARY_STRING);			
	}

	return (M4CL_M4_TYPE_FIXED_STRING);
}


sword ClCursor_oci::ReadLong (m4int32_t ai_indCol)
{
	sword		result = OCI_SUCCESS;
	m4int32_t	lLen;
	m4int32_t	iMaxLen = 0;
	m4pchar_t	pcValue = NULL;


	iMaxLen = m4int32_t( M4LDB_SIZE_LONG * M4UNI_FACTOR );
	pcValue = new char[ iMaxLen + 1 ];

	do {
		result = GetData (ai_indCol, &pcValue, &iMaxLen, M4CL_M4_TYPE_LONG, &lLen);
	} while (result == OCI_SUCCESS);

	delete [] pcValue ;

	return (result);
}

#endif
 


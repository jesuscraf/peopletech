//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                cursor.cpp   
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

#include "m4unicode.hpp"
#include "cursor.hpp"
#include "conex.hpp"
#include "curodbc.hpp"
#include "curoci.hpp"
#include "m4system.hpp"


extern m4bool_t g_bDDBBDisabled;

#define SQL_BLOCK_ORACLE			"FOR UPDATE"
#define SQL_ORDERBY					"ORDER BY"
#define SQL_GROUPBY					"GROUP BY"
#define SQL_FROM					"FROM"


// #define DBWRAP_TRACE


#ifdef DBWRAP_TRACE
#pragma message ("DBWrap with trace")
FILE *g_pFile;

#ifdef _UNIX
#define M4LDB_DBWRAP_TRACE_DIR			"./tmp"
#else
#define M4LDB_DBWRAP_TRACE_DIR			"c:\\temp"
#endif

#ifndef _UNIX
	ClM4Clock g_oClock;
#endif



class ClFileDebug
{
	m4double_t m_dTotalExecute;
	m4double_t m_dNumExecute;
	m4double_t m_dMaxExecute;
	m4double_t m_dTotalFetch;
	m4double_t m_dNumFetch;
	m4double_t m_dMaxFetch;
	m4double_t m_dTotalGetData;
	m4double_t m_dNumGetData;
	m4double_t m_dMaxGetData;

public:

	ClFileDebug (void)
	{
		m_dTotalExecute = 0.0;
		m_dNumExecute = 0.0;
		m_dMaxExecute = 0.0;
		m_dTotalFetch = 0.0;
		m_dNumFetch = 0.0;
		m_dMaxFetch = 0.0;
		m_dTotalGetData = 0.0;
		m_dNumGetData = 0.0;
		m_dMaxGetData = 0.0;

		m4char_t aFile[M4LDB_SIZE_DEFAULT_STRING];
		sprintf (aFile, "%s%sDBWrapIns.txt", M4LDB_DBWRAP_TRACE_DIR, M4_BACK_SLASH);
		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniNative ;

		g_pFile = M4Fopen(aFile, M4UniWriteBinary, eEncoding);
	}

	void IncExecute (m4int32_t ai_lTExecute)
	{
		m4double_t dExecute = (m4double_t) (((m4double_t) ai_lTExecute)/1000.0);
		m_dTotalExecute += dExecute;
		m_dNumExecute++;
		m_dMaxExecute = (dExecute > m_dMaxExecute) ? dExecute : m_dMaxExecute;
		fprintf (g_pFile, " Tiempo = %lG seg.", dExecute);

		if (dExecute >= 0.1)
		{
			fprintf (g_pFile, "**\n");
		}
		else
		{
			fprintf (g_pFile, "\n");
		}
	}

	void IncFetch (m4int32_t ai_lTFetch)
	{
		m4double_t dFetch = (m4double_t) (((m4double_t) ai_lTFetch)/1000.0);
		m_dTotalFetch += dFetch;
		m_dNumFetch++;
		m_dMaxFetch = (dFetch > m_dMaxFetch) ? dFetch : m_dMaxFetch;
		fprintf (g_pFile, " Tiempo = %lG seg.", dFetch);

		if (dFetch >= 0.1)
		{
			fprintf (g_pFile, "**\n");
		}
		else
		{
			fprintf (g_pFile, "\n");
		}
	}

	void IncGetData (m4int32_t ai_lTGetData)
	{
		m4double_t dGetData = (m4double_t) (((m4double_t) ai_lTGetData)/1000.0);
		m_dTotalGetData += dGetData;
		m_dNumGetData++;
		m_dMaxGetData = (dGetData > m_dMaxGetData) ? dGetData : m_dMaxGetData;
		fprintf (g_pFile, " Tiempo = %lG seg.", dGetData);

		if (dGetData >= 0.1)
		{
			fprintf (g_pFile, "**\n");
		}
		else
		{
			fprintf (g_pFile, "\n");
		}
	}

	void Dump (void)
	{
		fprintf (g_pFile, "\n");
		fprintf (g_pFile, "\n Total Execute = %lG", m_dTotalExecute);
		fprintf (g_pFile, "\n Número Execute = %lG", m_dNumExecute);
		fprintf (g_pFile, "\n Media Execute = %lG", m_dTotalExecute/m_dNumExecute);
		fprintf (g_pFile, "\n Máximo Execute = %lG", m_dMaxExecute);
		fprintf (g_pFile, "\n");
		fprintf (g_pFile, "\n Total Fetch = %lG", m_dTotalFetch);
		fprintf (g_pFile, "\n Número Fetch = %lG", m_dNumFetch);
		fprintf (g_pFile, "\n Media Fetch = %lG", m_dTotalFetch/m_dNumFetch);
		fprintf (g_pFile, "\n Máximo Fetch = %lG", m_dMaxFetch);
		fprintf (g_pFile, "\n");
		fprintf (g_pFile, "\n Total GetData = %lG", m_dTotalGetData);
		fprintf (g_pFile, "\n Número GetData = %lG", m_dNumGetData);
		fprintf (g_pFile, "\n Media GetData = %lG", m_dTotalGetData/m_dNumGetData);
		fprintf (g_pFile, "\n Máximo GetData = %lG", m_dMaxGetData);
		fflush (g_pFile);
	}

	~ClFileDebug (void)
	{
		Dump ();
		fclose(g_pFile);
	}

} g_oFileDebug;


#endif



ClCursor::ClCursor (void)
{
	m_pCurB = NULL;
	m_pConn = NULL;
	m_pConnPartialLoad = NULL;
	m_eLastError = M4DBW_UNDEFINED_ERROR;
	m_pMtCursor = NULL;
	m_pBonusUpdate = NULL;
}


ClCursor::~ClCursor (void)
{
	if (m_pBonusUpdate)
	{
		delete m_pBonusUpdate;
	}
	m_pBonusUpdate = NULL;

	Destroy ();

	if (m_pMtCursor)
	{
		m_pMtCursor->Detach ();

		if (!m_pMtCursor->GetReferences ())
		{
			delete m_pMtCursor;
		}
	}

	m_pMtCursor = NULL;
}


m4return_t ClCursor::Open (void) 
{
	if (g_bDDBBDisabled)
	{
		return (M4_ERROR);
	}

	m_pMtCursor = new ClMetaCursor;

	m_pMtCursor->Attach ();

	return (M4_SUCCESS);
}


m4return_t ClCursor::Close (void)
{
//	if (g_bDDBBDisabled) return (M4_ERROR);

	m4return_t result = M4_SUCCESS;

#ifdef DBWRAP_TRACE
	fprintf (g_pFile, "\t Cerrando Cursor. IdCursor = %p.\n", this);
	fflush (g_pFile);
#endif

	if (m_pCurB)
	{
		result = m_pCurB->Close ();

		if (result != M4_SUCCESS)
		{
			m_eLastError = M4DBW_CURSOR_ERROR;
		}
	}

	if (m_pBonusUpdate)
	{
		result = m_pBonusUpdate->Close();

		if (result != M4_SUCCESS)
		{
			m_eLastError = M4DBW_CURSOR_ERROR;
			return M4_ERROR;
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClCursor::Destroy (void)
{
//	if (g_bDDBBDisabled) return (M4_ERROR);

	if (m_pBonusUpdate)
	{
		m_pBonusUpdate->Destroy();
	}

	if (m_pConn)
	{
		return (m_pConn->DecNumCursor (this));
	}

	return (Defuse ());
}


m4return_t ClCursor::Defuse (void)
{
//	if (g_bDDBBDisabled) return (M4_ERROR);

	Close();

	//Vamos a desvincular la conexion fisica del cursor!. Con InitCursor se podra volver a vincular!.
	m_pConn = NULL;

	m4return_t result = M4_SUCCESS;

	if (m_pBonusUpdate)
	{
		m_pBonusUpdate->Defuse();
	}

	if (m_pCurB) {

#ifdef DBWRAP_TRACE
		fprintf (g_pFile, "\t Destruyendo Cursor. IdCursor = %p.\n", this);
		fflush (g_pFile);
#endif

		result = m_pCurB->Destroy ();

		if (m_pCurB)
		{
			delete m_pCurB;
		}

		if (m_pConnPartialLoad)
		{
			delete m_pConnPartialLoad;
		}
		m_pConnPartialLoad = NULL;
	
		m_pCurB = NULL;
	}
	
	return (M4_SUCCESS);
}


m4return_t ClCursor::Prepare (m4pchar_t ai_pStmt, eDDBBType_t ai_eDDBB, m4bool_t ai_bPartialLoad)
{
	if (g_bDDBBDisabled)
	{
		return (M4_ERROR);
	}

	m4return_t result = M4_SUCCESS;

	m_pMtCursor->m_bPartialLoad = ai_bPartialLoad;

/*	M4Zlib oComp;

	oComp.SetBufferIn ((const m4puchar_t) ai_pStmt, strlen (ai_pStmt) + 1);

	oComp.Compress ();

	m_pMtCursor->m_lstrSql = oComp.GetBufferOutSize();

	m_pMtCursor->m_pstrSql = new m4char_t [m_pMtCursor->m_lstrSql + 1];

	memcpy (m_pMtCursor->m_pstrSql, oComp.GetBufferOut (), m_pMtCursor->m_lstrSql);*/

	string strIn(ai_pStmt);

	if (ai_eDDBB==M4DBW_INFORMIX_DDBB || ai_eDDBB==M4DBW_DB2_DDBB){

		ClTrnSQL oTranslator;
		string strBU(M4LDB_EMPTY);
		if ( oTranslator.Translate(strIn,strBU)!=M4_SUCCESS)
		{
			m_eLastError = M4DBW_TRANSLATION_ERROR;
			return M4_ERROR;
		}

		if (!strBU.empty())
		{
			// Le llevamos hasta el Prepare.
			m_pBonusUpdate = new ClCursor();
			
			result = m_pBonusUpdate->Open ();
			
			if (result == M4_ERROR) {
				m_eLastError = M4DBW_CURSOR_ERROR;
				return M4_ERROR;
			}
			
			// Dejamos la posibilidad de que el BU tenga otro BU.
			result = m_pBonusUpdate->Prepare ((m4pchar_t) strBU.c_str(),ai_eDDBB);

			if (result == M4_ERROR)
			{
				m_eLastError = M4DBW_CURSOR_ERROR;
				return M4_ERROR;
			}
		}
	}

	m_pMtCursor->m_lstrSql = (m4int32_t) strlen(strIn.c_str());

	m_pMtCursor->m_pstrSql = new m4char_t [m_pMtCursor->m_lstrSql + 1];

	memcpy (m_pMtCursor->m_pstrSql, strIn.c_str(), m_pMtCursor->m_lstrSql + 1);

	return (result);
}


m4return_t ClCursor::BindParam (m4int32_t ai_lCol, m4pchar_t ai_pValue, m4pchar_t *ai_ppValue, m4int32_t ai_lPrec, m4int32_t ai_lM4Type, m4int32_t ai_lScale , m4pint32_t ao_lLength) 
{
	if (g_bDDBBDisabled)
	{
		return (M4_ERROR);
	}

	if (m_pBonusUpdate)
	{
		m_pBonusUpdate->BindParam ( ai_lCol, ai_pValue, ai_ppValue, ai_lPrec, ai_lM4Type, ai_lScale , ao_lLength);
	}

	if (!m_pMtCursor->m_pvcParam)
	{
		m_pMtCursor->m_pvcParam = new VcParamData;
		m_pMtCursor->m_pvcParam->reserve(10);
	}

	StParamData stParam (ai_lCol, ai_pValue, ai_ppValue, ai_lPrec, ai_lM4Type, ai_lScale, ao_lLength);

	m_pMtCursor->m_pvcParam->push_back (stParam);

	return (M4_SUCCESS);
}


m4return_t ClCursor::BindResult (m4int32_t ai_lCol, m4pchar_t *aio_ppResult, m4int32_t *aio_plMaxLen, m4int32_t ai_lM4Type, m4pint32_t ao_lLength, m4int32_t ai_lMaxLenFld) 
{
	if (g_bDDBBDisabled)
	{
		return (M4_ERROR);
	}

	m4return_t result = M4_SUCCESS,result2 = M4_SUCCESS;

	if (m_pBonusUpdate)
	{
		result2 = m_pBonusUpdate->BindResult (ai_lCol, aio_ppResult, aio_plMaxLen, ai_lM4Type, ao_lLength, ai_lMaxLenFld);

		if (result2!=M4_SUCCESS){
			m_eLastError = M4DBW_CURSOR_ERROR;
		}
	}

	if (!m_pMtCursor->m_pvcData)
	{
		m_pMtCursor->m_pvcData = new VcResultData;
		m_pMtCursor->m_pvcData->reserve(10);
	}

	StResultData stData (ai_lCol, aio_ppResult, aio_plMaxLen, ai_lM4Type, 0, ao_lLength, ai_lMaxLenFld);

	if (ai_lM4Type == M4CL_M4_TYPE_LONG ||
		ai_lM4Type == M4CL_M4_TYPE_UNI_LONG ||
		ai_lM4Type == M4CL_M4_TYPE_BLOB ||
		ai_lM4Type == M4CL_M4_TYPE_BINARY_STRING)
	{
		m_pMtCursor->m_bMultiRow = M4_FALSE;
	}

	m_pMtCursor->m_pvcData->push_back (stData);

	if (m_pCurB)
	{
		result = m_pCurB->BindResult (ai_lCol, aio_ppResult, aio_plMaxLen, ai_lM4Type, ao_lLength, ai_lMaxLenFld);

		if (result != M4_SUCCESS)
		{
			m_eLastError = M4DBW_CURSOR_ERROR;
		}
	}

	return (result|result2);
}


m4return_t ClCursor::Execute( m4uint32_t ai_iTimeOut, ICancelStatusProvider* ai_poCancelStatus, m4return_t& ao_riCanceled )
{
	ao_riCanceled = 0;

	if (g_bDDBBDisabled)
	{
		return (M4_ERROR);
	}

	m4return_t result = M4_SUCCESS;

#ifdef DBWRAP_TRACE
	g_oClock.Start ();
#endif

	if (m_pBonusUpdate)
	{
		result = m_pBonusUpdate->Execute(ai_iTimeOut, ai_poCancelStatus, ao_riCanceled);

		if (result != M4_SUCCESS)
		{
			m_eLastError = M4DBW_CURSOR_ERROR;
			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}
	}

	if( ai_iTimeOut > 0 )
	{
		m_pCurB->SetBlockingMode( 1 ) ;
	}

	result = m_pCurB->Execute(ai_iTimeOut, ai_poCancelStatus, ao_riCanceled);

	if( ai_iTimeOut > 0 && result == M4_SUCCESS )
	{
		m_pCurB->SetBlockingMode( 0 ) ;
	}

#ifdef DBWRAP_TRACE
	fprintf (g_pFile, "\t Executando Cursor. IdCursor = %p.", this);
#ifndef _UNIX
	g_oFileDebug.IncExecute (g_oClock.GetDifference ());
#endif
	fflush (g_pFile);
#endif

	if (result != M4_SUCCESS)
	{
		m_eLastError = M4DBW_CURSOR_ERROR;
	}

	return (result);
}


m4return_t ClCursor::GetData (m4int32_t ai_lCol, m4pchar_t *aio_ppResult, m4int32_t *aio_plMaxLen, m4int32_t ai_lM4Type, m4pint32_t ao_lLength, m4bool_t ai_bByPass)
{
	if (g_bDDBBDisabled)
	{
		return (M4_ERROR);
	}

	m4return_t result = M4_ERROR;
		
	if (m_pCurB) {
#ifdef DBWRAP_TRACE
		g_oClock.Start ();
#endif

		result = m_pCurB->GetData (ai_lCol, aio_ppResult, aio_plMaxLen, ai_lM4Type, ao_lLength,ai_bByPass);

#ifdef DBWRAP_TRACE
		fprintf (g_pFile, "\t Obteniendo LONG. IdCursor = %p.", this);
#ifndef _UNIX
		g_oFileDebug.IncGetData (g_oClock.GetDifference ());
#endif
		fflush (g_pFile);
#endif

		if (result != M4_SUCCESS)
		{
			m_eLastError = M4DBW_CURSOR_ERROR;
		}
	}

	return (result);
}


m4return_t ClCursor::Fetch (void) 
{
	if (g_bDDBBDisabled)
	{
		return (M4_ERROR);
	}

	m4return_t result =	M4_ERROR;
	
	if (m_pCurB)
	{
#ifdef DBWRAP_TRACE
		g_oClock.Start ();
#endif

		result = m_pCurB->Fetch ();

#ifdef DBWRAP_TRACE
		fprintf (g_pFile, "\t Iterando Cursor. IdCursor = %p.", this);
#ifndef _UNIX
		g_oFileDebug.IncFetch (g_oClock.GetDifference ());
#endif
		fflush (g_pFile);
#endif

		if (result != M4_SUCCESS)
		{
			m_eLastError = M4DBW_CURSOR_ERROR;
		}
	}

	return (result);
}


m4return_t ClCursor::DescribeCol (m4int32_t ai_lCol, m4pint32_t ao_lPrec, m4pint8_t ao_cM4Type) 
{
	if (g_bDDBBDisabled)
	{
		return (M4_ERROR);
	}

	m4return_t result = M4_ERROR;
		
	if (m_pCurB)
	{
		result = m_pCurB->DescribeCol (ai_lCol, ao_lPrec, ao_cM4Type);

		if (result != M4_SUCCESS)
		{
			m_eLastError = M4DBW_CURSOR_ERROR;
		}
	}

	return (result);
}


m4int32_t ClCursor::GetNumCol (void) 
{
	if (g_bDDBBDisabled)
	{
		return (M4LDB_NULL_INDEX);
	}

	if (m_pCurB)
	{
		m4int32_t numCol = m_pCurB->GetNumCol ();

		if (!numCol) {
			// El número de columnas no puede ser cero en una select.
			// A pasado algo raro por lo que es mejor que repreparemos la select
			
			/* Bug 176634
			Se guarda la conexión porque en el destroy se pone a nulo
			*/
			ClDBConnection *pConn = m_pConn;

			m4return_t result =	Destroy ();

			if (result == M4_ERROR)
			{
				return (M4LDB_NULL_INDEX);
			}

			m4bool_t bAux;
			result = InitCursor (pConn, bAux);

			if (result == M4_ERROR) {
				return (M4LDB_NULL_INDEX);
			}

			m4return_t iCanceled = M4_SUCCESS;
			result = Execute(0, NULL, iCanceled);

			if (result == M4_ERROR)
			{
				return (M4LDB_NULL_INDEX);
			}

			numCol = m_pCurB->GetNumCol ();

			if (!numCol)
			{
				return (M4LDB_NULL_INDEX);
			}
		}

		return (numCol);
	}

	return (M4LDB_NULL_INDEX);
}


m4return_t ClCursor::GetErrorString (m4return_t ai_retcode, m4int32_t& ao_iErrorCode, m4pchar_t *ao_ppcErrorString)
{
	if (g_bDDBBDisabled) return (M4_ERROR);

	m4return_t result = M4_ERROR;

	if ((m_eLastError == M4DBW_CONNECTION_ERROR) && 
		(m_pConn))
	{
		result = m_pConn->GetErrorString (ai_retcode, ao_iErrorCode, ao_ppcErrorString);
	}
	else
	{
		if ((m_eLastError == M4DBW_CURSOR_ERROR) && 
			(m_pCurB))
		{
			if (m_pBonusUpdate)
			{
				result = m_pBonusUpdate->GetErrorString (ai_retcode, ao_iErrorCode, ao_ppcErrorString);
				switch (result)
				{
				case (M4LDB_SQL_NO_DATA_FOUND) :
					// No ha sido el Bouns Update.
					break;

				default:
					if (ai_retcode == M4_WARNING)
					{
						return (M4_WARNING);
					}
					else
					{
						return (M4_SUCCESS);
					}
					break;
				}
			}

			result = m_pCurB->GetErrorString (ai_retcode, ao_iErrorCode, ao_ppcErrorString);
		}
		else
		{
			if (m_eLastError==M4DBW_CONEX_CLONE_ERROR)
			{
				if (m_pConnPartialLoad)
				{
					result = m_pConnPartialLoad->GetErrorString (ai_retcode, ao_iErrorCode, ao_ppcErrorString);
				}
			}
		}
	}

	if (result == M4_ERROR)
	{
		m_eLastError = M4DBW_UNDEFINED_ERROR;
	}

	return (result);
}


m4return_t ClCursor::GetStmt (string &ao_stStmt) 
{
	if (g_bDDBBDisabled)
	{
		return (M4_ERROR);
	}

	ao_stStmt.erase();

	if (m_pBonusUpdate)
	{
		if (m_pBonusUpdate->GetStmt(ao_stStmt)!=M4_SUCCESS)
		{
			return M4_ERROR;
		}
		ao_stStmt.append("\n\t(Stmt For Update)");
	}

	if (m_pMtCursor->m_pstrSql)
	{
		ao_stStmt.append(m_pMtCursor->m_pstrSql);

		if (m_pMtCursor->m_pvcParam)
		{
			m4int32_t lPos = 0;
			m4int32_t indParam;
			m4int32_t lLen = 0;
//			m4date_t dDate;
			//Bug 0067816. Se añade 1 para el valor nulo pq podemos tener el caso:
			//comillas + 253 + comillas = 255. Pero necesitamos 1 más para el null de la cadena.
			m4char_t aData[M4LDB_SIZE_DEFAULT_STRING + 1];
			m4char_t aDate[M4LDB_SIZE_TIMESTAMP + 1];
			m4pchar_t pLong;
			m4int32_t lLength;

			m4int32_t numParam = (m4int32_t) m_pMtCursor->m_pvcParam->size();

			for (indParam=0; indParam<numParam; indParam++)
			{
				/* Bug 0126926
				No se sustituyen los parámetros porque m_pData puede ser nulo
				Hay que tomar el que no sea nulo
				*/
				m4pchar_t pData = NULL;

				pData = (*m_pMtCursor->m_pvcParam)[indParam].m_pData ;

				if( pData == NULL )
				{
					pData = *((*m_pMtCursor->m_pvcParam)[indParam].m_ppData) ;
				}

				lPos = (m4int32_t) ao_stStmt.find ('?', lPos);

				if (lPos == M4_STR_NPOS)
				{
					return M4_ERROR;
				}

				lLength = * (*m_pMtCursor->m_pvcParam)[indParam].m_plLength;

				/* Bug 0142611
				Se chequea también que el valor no se anulo y sino se pone NULL
				*/
				if (lLength != M4LDB_SQL_NULL_DATA && pData != NULL)
				{
					switch ( (*m_pMtCursor->m_pvcParam)[indParam].m_iM4Type)
					{
						/* Bug 0146180
						Los campos long y binarios se imprimen por trozos y teniendo en cuenta que pueden no ser
						null terminados.
						Se repasan y completan los demás tipos.
						*/
						case M4CL_M4_TYPE_NUMBER:
						case M4CL_M4_TYPE_CURRENCY:
						case M4CL_M4_TYPE_NUM_VARIANT:
						case M4CL_M4_TYPE_TIME_SPAN:

							lLen = sprintf( aData, "%.20G", M4LDB_DOUBLE( pData ) );
							break;

						case M4CL_M4_TYPE_DATE:

							ClDateToString( M4LDB_DOUBLE( pData ), aDate, M4LDB_SIZE_DATE + 1 );
							lLen = sprintf( aData, "{d '%s'}", aDate );
							break;

						case M4CL_M4_TYPE_TIMESTAMP:
						case M4CL_M4_TYPE_HOUR:

							ClDateToString( M4LDB_DOUBLE( pData ), aDate, M4LDB_SIZE_TIMESTAMP + 1 );
							lLen = sprintf( aData, "{ts '%s'}", aDate );
							break;

						case M4CL_M4_TYPE_LONG:
						case M4CL_M4_TYPE_UNI_LONG:
						case M4CL_M4_TYPE_BINARY_STRING:

							pLong = pData;
							if (lLength == M4LDB_SQL_NTS)
							{
								lLength = (m4int32_t) strlen( pLong );
							}
							if (lLength >= M4LDB_SIZE_DEFAULT_STRING-2)
							{ //'' y el 0.
								lLen = sprintf( aData, "'%.32s...%.32s' Size = %ld ; ", pLong, &pLong[lLength-32], lLength );
							}
							else
							{
								// Hay que tener cuidado si no es nullterminado y si tiene ceros por medio
								aData[0] = '\'';
								memcpy( aData + 1, pLong, lLength );
								aData[lLength + 1] = '\0';
								strcat( aData, "\'" );

								lLen = (m4int32_t) strlen( aData );
							}
							break;

						default:
							if (strlen( pData ) > M4LDB_SIZE_DEFAULT_STRING-3 )
							{
								//Comillas + 250 + 3 puntos + Comillas = 255. (aData es 255 mas el nulo).
								lLen = sprintf( aData, "'%.250s...'", pData );
							}
							else
							{
								lLen = sprintf( aData, "'%s'", pData );
							}

							break;
					}

					ao_stStmt.replace (lPos, 1, aData);
					lPos += lLen;
				}
				else
				{
					ao_stStmt.replace (lPos, 1, "NULL");
					lPos += 4;
				}
			}
		}
	}
	else
	{
		ao_stStmt = M4LDB_EMPTY;
	}

	return (M4_SUCCESS);
}


m4return_t ClCursor::GetStmt (m4pcchar_t &ao_pStmt) 
{
	if (g_bDDBBDisabled)
	{
		return (M4_ERROR);
	}

	if (m_pMtCursor->m_pstrSql)
	{
		ao_pStmt = m_pMtCursor->m_pstrSql;
	}
	else
	{
		ao_pStmt = M4LDB_EMPTY;
	}

	return (M4_SUCCESS);
}


m4return_t ClCursor::InitCursor (ClDBConnection *ai_pConn, m4bool_t &ao_bReallyInitialized)
{
	if (g_bDDBBDisabled)
	{
		return (M4_ERROR);
	}

	m_pConn = ai_pConn;

	m4return_t result;

	ao_bReallyInitialized = M4_FALSE;

	if (m_pBonusUpdate)
	{
		result = m_pBonusUpdate->InitCursor(ai_pConn, ao_bReallyInitialized);
		if (result != M4_SUCCESS)
		{
			m_eLastError = M4DBW_CURSOR_ERROR;
			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}
	}

	if (!m_pCurB)
	{
		ao_bReallyInitialized = M4_TRUE;

		switch (m_pConn->m_pstConProp->m_eDBW)
		{
			case M4DBW_ORACLE_DDBB:
				#ifdef DRIVER_OCI
					m_pCurB = new ClCursor_oci;
				#else
					m_pCurB = new ClCursor_odbc;
				#endif
				break;

			case M4DBW_DB2_DDBB:

			case M4DBW_SYBASE_DDBB:

			default:
				m_pCurB = new ClCursor_odbc;
				break;
		}


#ifdef DBWRAP_TRACE
		fprintf (g_pFile, "\t Creando Cursor. IdCursor = %p.\n", this);
		fflush (g_pFile);
#endif

		result = m_pConn->IncNumCursor (this);

		if (result != M4_SUCCESS)
		{
			m_eLastError = M4DBW_CURSOR_ERROR;
			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}

		if (m_pMtCursor->m_bPartialLoad && m_pConn->m_pstConProp->m_eDBW== M4DBW_SQL_SERVER_DDBB)
		{
			result = m_pConn->Clone (m_pConnPartialLoad);

			if (result == M4_ERROR)
			{
				m_eLastError = M4DBW_CONEX_CLONE_ERROR;
				return (M4_ERROR);
			}

			result = m_pCurB->Open (m_pConnPartialLoad->m_pConB);
		}
		else
		{
			result = m_pCurB->Open (m_pConn->m_pConB);
		}

		if (result != M4_SUCCESS)
		{
			m_eLastError = M4DBW_CURSOR_ERROR;
			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}
		
		m4int32_t numPar = 0;

		if (m_pMtCursor->m_pvcParam) {
			numPar = (m4int32_t) m_pMtCursor->m_pvcParam->size();
		}

		m4int32_t numCol = -1;

		if (m_pMtCursor->m_pvcData)
		{
			numCol = (m4int32_t) m_pMtCursor->m_pvcData->size();
		}

		
#ifdef DBWRAP_TRACE
		fprintf (g_pFile, "\t Preparando Cursor. IdCursor = %p. SQL = %s.\n", this, m_pMtCursor->m_pstrSql);
		fflush (g_pFile);
#endif

		result = m_pCurB->Prepare (m_pMtCursor->m_pstrSql, numPar, numCol, m_pMtCursor->m_bMultiRow);

		if (result != M4_SUCCESS)
		{
			m_eLastError = M4DBW_CURSOR_ERROR;
			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}

		if (m_pMtCursor->m_pvcParam)
		{
			for (itVcParamData it=m_pMtCursor->m_pvcParam->begin (); it!=m_pMtCursor->m_pvcParam->end(); it++) {
				result = m_pCurB->BindParam ((*it).m_lCol, (*it).m_pData, (*it).m_ppData, (*it).m_lPrec, (*it).m_iM4Type, (*it).m_lScale, (*it).m_plLength);

				if (result != M4_SUCCESS)
				{
					m_eLastError = M4DBW_CURSOR_ERROR;
					if (result == M4_ERROR)
					{
						return (M4_ERROR);
					}
				}
			}
		}

		if (m_pMtCursor->m_pvcData)
		{
			for (itVcResultData it=m_pMtCursor->m_pvcData->begin (); it!=m_pMtCursor->m_pvcData->end(); it++) {
				result = m_pCurB->BindResult ((*it).m_lCol, (*it).m_ppData, (*it).m_plPrec, (*it).m_iM4Type, (*it).m_plLength, (*it).m_lPrecFld);

				if (result != M4_SUCCESS)
				{
					m_eLastError = M4DBW_CURSOR_ERROR;
					if (result == M4_ERROR)
					{
						return (M4_ERROR);
					}
				}
			}
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClCursor::Clone (ClCursor *ai_pCursor)
{
	if (g_bDDBBDisabled) return (M4_ERROR);

	if (!m_pCurB)
	{
		m_pMtCursor = ai_pCursor->m_pMtCursor;
		m_pMtCursor->Attach ();
		m_eLastError = M4DBW_UNDEFINED_ERROR;
	}

	if (ai_pCursor->m_pBonusUpdate)
	{
		if (m_pBonusUpdate)
		{
			delete m_pBonusUpdate;
			m_pBonusUpdate=0;
		}
	
		m_pBonusUpdate = new ClCursor();
		m_pBonusUpdate ->Clone(ai_pCursor->m_pBonusUpdate);
	}

	return (M4_SUCCESS);
}



m4bool_t ClCursor::AllowCursorOpened (void)
{
	if (m_pConn) {
		return (m_pConn->m_pstConProp->m_bCursorOpened);
	}

	return (M4_FALSE);
}


m4bool_t ClCursor::IsCursorActivated (void)
{
	if (m_pCurB) {
		return (M4_TRUE);
	}
	return (M4_FALSE);
}


m4return_t	ClCursor::SetBlockingMode( m4uint32_t ai_iMode )
{
	if( m_pCurB != NULL )
	{
		return( m_pCurB->SetBlockingMode( ai_iMode ) ) ;
	}
	return( M4_ERROR ) ;
}


m4return_t ClTrnSQL::Translate( string& ao_stStmSQL , string& ao_stStmBonusUpdate )
{
	size_t iOrderBy = M4_STR_NPOS;
	size_t iGroupBy = M4_STR_NPOS;
	size_t iForUpdate = M4_STR_NPOS;
	size_t iFrom = M4_STR_NPOS;
	size_t iPosIni = M4_STR_NPOS;

	m4pcchar_t pString = ao_stStmSQL.c_str();
	m4pchar_t pTemp = 0;


	// Busco ORDER BY, GROUP BY y FOR UPDATE
	pTemp = _FindToken (pString,SQL_ORDERBY);
	if (pTemp == 0)
	{
		iOrderBy=M4_STR_NPOS;
	}
	else
	{
		iOrderBy = pTemp-pString;
	}

	pTemp = _FindToken (pString,SQL_GROUPBY);
	if (pTemp == 0)
	{
		iGroupBy=M4_STR_NPOS;
	}
	else
	{
		iGroupBy = pTemp-pString;
	}

	if (iOrderBy==M4_STR_NPOS&&iGroupBy==M4_STR_NPOS){
		return M4_SUCCESS;
	}
	
	pTemp = _FindToken (pString,SQL_BLOCK_ORACLE);
	if (pTemp == 0)
	{
		iForUpdate = M4_STR_NPOS;
	}
	else
	{
		iForUpdate = pTemp-pString;
	}
	if (iForUpdate==M4_STR_NPOS){
		return M4_SUCCESS;
	}
	pTemp = _FindToken (pString,SQL_FROM);
	if (pTemp == 0)
	{
		iFrom = M4_STR_NPOS;
	}
	else
	{
		iFrom = pTemp-pString;
	}
	if (iFrom==M4_STR_NPOS)
	{
		return M4_ERROR;
	}

	// Cogemos el comienzo del primero.
	if (iOrderBy!=M4_STR_NPOS && iGroupBy!=M4_STR_NPOS)
	{
		iPosIni = (iOrderBy<iGroupBy)?iOrderBy:iGroupBy;
	}
	else
	{
		if (iOrderBy!=M4_STR_NPOS)
		{
			iPosIni = iOrderBy;
		}
		else
		{
			iPosIni = iGroupBy;
		}
	}
	
	ao_stStmBonusUpdate.assign ("SELECT 1 ");
	ao_stStmBonusUpdate.append(ao_stStmSQL,iFrom,iPosIni-iFrom);

	ao_stStmBonusUpdate.append ( "FOR UPDATE");

	// Borramos el For Update.
	ao_stStmSQL.replace( iForUpdate, 10 , "" ); // strlen ("FOR UPDATE")

   return M4_SUCCESS;
}


m4pchar_t ClTrnSQL::_FindToken(m4pcchar_t ai_pstring,m4pcchar_t ai_pToken)
{
	if (ai_pToken==0)
	{
		return 0;
	}
	if (ai_pstring==0)
	{
		return 0;
	}
	
	m4pcchar_t ptemp = 0;
	size_t len = strlen(ai_pToken);

	while (ptemp==0)
	{
		ptemp = ::strstr( ai_pstring , ai_pToken ) ;
		if (ptemp)
		{
				m4char_t ccc = *(ptemp-1);
				// Miaramos  si lo anterior es un separador
				if (ccc==')' || ccc == ' ' || ccc == '\t' || ccc == '[')
				{
					ccc = *(ptemp+len);
					// Miaramos  si lo posterior es un separador
					if (ccc=='(' || ccc == ' ' || ccc == '\t' || ccc == ']' || ccc == 0)
					{
					   break;
					}
				}
				ai_pstring=ptemp;
				ptemp = 0;
				ai_pstring++;
		}
		else
		{
			// No hay Where.
			return 0;
		}
	}

	return (char*)ptemp;
}




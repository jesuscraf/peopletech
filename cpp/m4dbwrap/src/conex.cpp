//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                conex.cpp   
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

#include "conex.hpp"
#include "conodbc.hpp"
#include "conoci.hpp"
#include "cursor.hpp"

#include "blocksynchronization.hpp"

//Se establece el separador de decimales y de lista, por ese orden.
const m4char_t M4DBW_ORACLE_EXECUTE_NLS_NUMERIC[] = "ALTER SESSION SET NLS_NUMERIC_CHARACTERS = '.,'";

// #define DBWRAP_TRACE


#ifdef DBWRAP_TRACE
	extern FILE *g_pFile;
#endif

	
ClMutex g_oMxMpCursor(M4_TRUE);


m4bool_t g_bDDBBDisabled = M4_FALSE;

void DisableDDBB (void)
{
	g_bDDBBDisabled = M4_TRUE;
}

void EnableDDBB (void)
{
	g_bDDBBDisabled = M4_FALSE;
}

m4bool_t IsDDBBDisabled (void)
{
	return (g_bDDBBDisabled);
}

ClDBConnection::ClDBConnection() {

	m_pstConProp = 0;
	m_eLastError = M4DBW_UNDEFINED_ERROR;
	m_pConB = 0;
	m_idConn = -1;
	m_bAllowOperationsWhenDDBBIsDisabled = M4_FALSE;
}



ClDBConnection::~ClDBConnection (void)
{
	Disconnect ();
	m_pstConProp = 0;
	m_eLastError = M4DBW_UNDEFINED_ERROR;
	m_idConn = -1;
	m_bAllowOperationsWhenDDBBIsDisabled = M4_FALSE;
}


m4return_t ClDBConnection::Disconnect (void) 
{
//	if ((g_bDDBBDisabled) && (!m_bAllowOperationsWhenDDBBIsDisabled)) return (M4_ERROR);

	m4return_t result = M4_SUCCESS;

	if (m_pConB) {

		for (itVcClCursor it = m_vcCursor.begin (); it!=m_vcCursor.end (); it++) {
			(*it)->Defuse ();
		}

		m_vcCursor.erase (m_vcCursor.begin (), m_vcCursor.end ());

#ifdef DBWRAP_TRACE
	fprintf (g_pFile, " Destruyendo Conexión. IdConn = %p.\n", this);
	fflush (g_pFile);
#endif
		result = m_pConB->Disconnect();

		result = m_pConB->End();

		if (m_pConB) {
			delete m_pConB;
		}
	}
	m_pConB = NULL;

	return (result);
}


m4return_t ClDBConnection::Transact (eTransType_t ai_eTransType)
{
	if ((g_bDDBBDisabled) && (!m_bAllowOperationsWhenDDBBIsDisabled)) return (M4_ERROR);

	m4return_t result = M4_SUCCESS;

	if (m_pConB) {
		result = m_pConB->Transact (ai_eTransType);

		if (result != M4_SUCCESS) {
			m_eLastError = M4DBW_CONNECTION_ERROR;
			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}
	}

	return (result);
}


m4return_t ClDBConnection::InitConnection (void) 
{
	if ((g_bDDBBDisabled) && (!m_bAllowOperationsWhenDDBBIsDisabled)){
		return (M4_ERROR);
	}

	m4return_t result = M4_SUCCESS;

	if (!m_pConB) {

		switch (m_pstConProp->m_eDBW) {
			case M4DBW_ORACLE_DDBB:
				#ifdef DRIVER_OCI
					m_pConB = new ClDBConnection_oci;
				#else
					m_pConB = new ClDBConnection_odbc(m_pstConProp->m_eDBW);
				#endif
				break;

			case M4DBW_DB2_DDBB:

			case M4DBW_SYBASE_DDBB:

			default:
				m_pConB = new ClDBConnection_odbc(m_pstConProp->m_eDBW);
				break;
		}

#ifdef DBWRAP_TRACE
	fprintf (g_pFile, " Creando Conexión. IdConn = %p. MaxNumCursor = %ld. StrConn = %s.\n", this, m_pstConProp->m_numMaxCursor, m_pstConProp->m_stConnStr.c_str ());
	fflush (g_pFile);
#endif
		result = m_pConB->Init ();

		if (result != M4_SUCCESS) {
			m_eLastError = M4DBW_CONNECTION_ERROR;
			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}

		result = m_pConB->Connect (m_pstConProp->m_stConnStr.c_str ());

		if (result != M4_SUCCESS) {
			m_eLastError = M4DBW_CONNECTION_ERROR;
			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}

		result = m_pConB->SetIsolationLevel (m_pstConProp->m_eILevel);

		if (result != M4_SUCCESS) {
			m_eLastError = M4DBW_CONNECTION_ERROR;
			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}

		result = ExecuteSQL (m_pstConProp->m_stConExec.c_str());

		if (result != M4_SUCCESS) {
			m_eLastError = M4DBW_CONNECTION_ERROR;
			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}

		//Bug 0085738
		//Si estamos en Oracle obligamos que el separador de decimales sea el '.'.
		//Escenario:
		// Item propiedad varchar
		// Item campo numérico
		// APISQL: EXECUTESQL_M = "UPDATE &YY_LDB_GLOBAL SET GB_NUMERO = @NLS_NUMERIC_CHAR"
		// Si el usuario introduce 2.10 y el separador de decimales es ',' se da error.
		// Si el usuario introduce 2,10 y el separador de decimales es '.' también da un error.
		//Solucion: Forzamos a que el separador decimal sea '.' de esta forma el cliente deberá siempre separar con '.'
		if (m_pstConProp->m_eDBW == M4DBW_ORACLE_DDBB) {
			result = ExecuteSQL (M4DBW_ORACLE_EXECUTE_NLS_NUMERIC);
			if (result != M4_SUCCESS) {
				m_eLastError = M4DBW_CONNECTION_ERROR;
				if (result == M4_ERROR) {
					return (M4_ERROR);
				}
			}
		}

	}

 	return (result);
}


m4return_t ClDBConnection::InitTransaction (void)
{
	if ((g_bDDBBDisabled) && (!m_bAllowOperationsWhenDDBBIsDisabled)) return (M4_ERROR);

	m4return_t result = M4_SUCCESS;

		if (m_pConB) {
		result = m_pConB->Transact (M4LDB_BEGIN_TRANS);

		if (result != M4_SUCCESS) {
			m_eLastError = M4DBW_CONNECTION_ERROR;
			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}
	}

	return (result);
}


m4return_t ClDBConnection::GetErrorString (m4return_t ai_retcode, m4int32_t& ao_iErrorCode, m4pchar_t *ao_ppcErrorString)
{
	if ((g_bDDBBDisabled) && (!m_bAllowOperationsWhenDDBBIsDisabled)) return (M4_ERROR);

	m4return_t result = M4_ERROR;

	if ((m_eLastError == M4DBW_CONNECTION_ERROR) && 
		(m_pConB)) {
		result = m_pConB->GetErrorString (ai_retcode, ao_iErrorCode, ao_ppcErrorString);
	}

	if (result != M4_SUCCESS) {
		m_eLastError = M4DBW_UNDEFINED_ERROR;
	}

	return (result);
}


m4return_t ClDBConnection::IncNumCursor (ClCursor *ai_pCursor)
{
	if ((g_bDDBBDisabled) && (!m_bAllowOperationsWhenDDBBIsDisabled)) return (M4_ERROR);

	m4return_t result;

	if (m_vcCursor.size () >= (m4uint32_t)m_pstConProp->m_numMaxCursor) {

		result = DecNumCursor ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}
	}

	m_vcCursor.push_back (ai_pCursor);

#ifdef DBWRAP_TRACE
	fprintf (g_pFile, " Incrementando Cursor. IdConn = %p. NumCursor = %ld.\n", this, m_vcCursor.size ());
	fflush (g_pFile);
#endif

	return (M4_SUCCESS);
}


m4return_t ClDBConnection::DecNumCursor (ClCursor *ai_pCursor)
{
//	if ((g_bDDBBDisabled) && (!m_bAllowOperationsWhenDDBBIsDisabled)) return (M4_ERROR);

	ClMutBlock oCR (g_oMxMpCursor);

	m4return_t result;

	itVcClCursor it = m_vcCursor.begin ();

	if (ai_pCursor) {
		for (;((it!=m_vcCursor.end ()) && ((*it) != ai_pCursor));it++);
	}

	if (it == m_vcCursor.end ()) {
		return (M4_SUCCESS);
	}

#ifdef DBWRAP_TRACE
	fprintf (g_pFile, " Decrementando Cursor. IdConn = %p. NumCursor = %ld.\n", this, m_vcCursor.size ());
	fflush (g_pFile);
#endif

	result = (*it)->Defuse ();

	m_vcCursor.erase (it);

	return (result);
}


eDDBBType_t ClDBConnection::GetDDBBType (void)
{
	return (m_pstConProp->m_eDBW);
}


m4return_t ClDBConnection::ExecuteSQL (m4pcchar_t ai_pSQL)
{
	if ((g_bDDBBDisabled) && (!m_bAllowOperationsWhenDDBBIsDisabled)) return (M4_ERROR);

	m4return_t result;

	m4pchar_t pSQL = (m4pchar_t) ai_pSQL;

	do {
		m4pchar_t pCurSQL = pSQL;

		for (;((*pSQL) && (*pSQL != ';')); pSQL++);

		size_t len = pSQL - pCurSQL;

		if (len > 5) {
			// Suponemos que hay algo distinto de blancos.
			m4char_t aSQL[2048];

			memcpy (aSQL, pCurSQL, len);

			aSQL[len] = '\0';

			result = m_pConB->ExecuteSQL (aSQL);

			if (result != M4_SUCCESS) {
				m_eLastError = M4DBW_CONNECTION_ERROR;
				if (result == M4_ERROR) {
					return (M4_ERROR);
				}
			}
		}
		else {
			if (!*pSQL) {
				break;
			}
			pSQL++;
		}
	} while (1);

	return (M4_SUCCESS);
}

m4return_t ClDBConnection::Clone (ClDBConnection *&ao_pConn)
{
	if ((g_bDDBBDisabled) && (!m_bAllowOperationsWhenDDBBIsDisabled)) return (M4_ERROR);

	m4return_t result;

	ao_pConn = new ClDBConnection ();
	ao_pConn->Init(m_idConn, m_pstConProp);

	result = ao_pConn->InitConnection ();

	if (result == M4_ERROR) {
		m_eLastError = M4DBW_CONNECTION_ERROR;
		if (result == M4_ERROR) {
			return (M4_ERROR);
		}
	}

	/* Bug 0309857 Ya no se inicia transacción en nuevos cursores de carga parcial en SQLServer
	result = ao_pConn->InitTransaction ();

	if (result == M4_ERROR) {
		m_eLastError = M4DBW_CONNECTION_ERROR;
		if (result == M4_ERROR) {
			return (M4_ERROR);
		}
	}
	*/

	return (M4_SUCCESS);
}


m4int32_t ClDBConnection::GetIdConn (void)
{
	return (m_idConn);
}

m4return_t
ClDBConnection::End(void){
	m4return_t ret = M4_SUCCESS;
	ret = Disconnect();
	m_idConn = -1;
	m_eLastError = M4DBW_UNDEFINED_ERROR;
	m_pConB = 0;
	m_pstConProp = 0;
	return ret;
}


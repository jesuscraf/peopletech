//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                conodbc.hpp   
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

#ifndef  _CONEXOCI_HPP_
#define  _CONEXOCI_HPP_


#ifdef _UNIX	// UNIX

#else			// WINDOWS
	#ifndef  _WINDOWS
	#define _WINDOWS
	#endif
#endif


#define M4DBW_OCI_CALL(X)		while( ( X ) == OCI_STILL_EXECUTING )
#define M4DBW_OCI_ERROR(X)		(m4return_t) ((X == OCI_SUCCESS) ? M4_SUCCESS : (((X == OCI_SUCCESS_WITH_INFO) || (X == OCI_NEED_DATA)) ? M4_WARNING : ((X == OCI_NO_DATA) ? M4LDB_SQL_NO_DATA_FOUND : M4_ERROR)))


#include "conexba.hpp"
#ifdef DRIVER_OCI   
#include "oci.h"
#endif

class ClDBConnection_oci: public ClDBConnection_Base
{
public:
    
	ClDBConnection_oci (void);
	~ClDBConnection_oci (void);

	m4return_t Init (void);

	m4return_t End (void);

	m4return_t Connect (m4pcchar_t ai_pConnStr);

	m4return_t Disconnect (void);

	m4return_t Transact (eTransType_t ai_eTransType);

	m4return_t GetErrorString (m4return_t ai_retcode, m4int32_t &ao_iErrorCode, m4pchar_t *ao_ppcErrorString);

	m4return_t SetIsolationLevel (eIsolationLevel_t ai_eILevel);

	m4return_t ExecuteSQL (m4pcchar_t ai_pSQL);

private:

#ifdef DRIVER_OCI   
	BOOL        m_bAutoCommit;
	OCIEnv     *m_hEnv; 					/* the environment handle */
	OCIServer  *m_hSrv; 					/* the server handle */
	OCISvcCtx  *m_hSvC;						/* the server context */ 
	OCIError   *m_hErr; 					/* the error handle */
	OCISession *m_hSes; 				    /* user session handle */
	m4int32_t   m_nErr;
#endif

  friend class ClCursor_oci;

   
};

#endif

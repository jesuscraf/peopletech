//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                curoci.inl
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


#include "conoci.hpp"
#include "curoci.hpp"
#include "cldates.hpp"


M4_INLINE ClCursor_oci::ClCursor_oci ()
{
#ifdef DRIVER_OCI
	m_hStmt = NULL;
	m_pConn = NULL;
	m_lnumBindResult = 0;
	m_pstBindResult = NULL;
	m_lnumBindParam = 0;
	m_pstBindParam  = NULL;
	m_bIsSelect = M4_TRUE;
	m_bInvExecFetch = M4_TRUE;
	m_lRemainder = 0;
	m_lNumRow=1;
	m_bLongExist = M4_FALSE;
	m_bRebindAllParameters = M4_FALSE;
#else
#endif
	
}


M4_INLINE ClCursor_oci::~ClCursor_oci ()
{
#ifdef DRIVER_OCI
	Destroy();
#else
#endif
}


M4_INLINE m4return_t ClCursor_oci::Open (ClDBConnection_Base * clCon)
{
#ifdef DRIVER_OCI

	sword result = OCI_SUCCESS;

	m_pConn = (ClDBConnection_oci *) clCon;

	M4DBW_OCI_CALL( result = OCIHandleAlloc( m_pConn->m_hEnv, (dvoid**) &m_hStmt, OCI_HTYPE_STMT, 0, NULL ) );

	return (M4DBW_OCI_ERROR (result)); 
#else
	return (M4_ERROR);
#endif
}


M4_INLINE m4int32_t ClCursor_oci::GetNumCol (void)
{
#ifdef DRIVER_OCI
	sword result = OCI_SUCCESS;
	ub4 parmcnt; 
	
	M4DBW_OCI_CALL( result = OCIAttrGet( m_hStmt, OCI_HTYPE_STMT, (dvoid*) &parmcnt, NULL, OCI_ATTR_PARAM_COUNT, m_pConn->m_hErr ) );

	if (result == OCI_NO_DATA)
	{
		return (M4LDB_NULL_INDEX);
	}

	m_lnumBindResult = (m4int32_t) parmcnt;
	
	return (m_lnumBindResult);
#else
	return (M4LDB_NULL_INDEX);
#endif
}


M4_INLINE m4return_t ClCursor_oci::GetErrorString ( m4return_t ai_retcode, m4int32_t& ao_iErrorCode, m4pchar_t *ao_ppcErrorString )
{
#ifdef DRIVER_OCI
	return (m_pConn->GetErrorString(ai_retcode, ao_iErrorCode, ao_ppcErrorString));
#else
	return (M4_ERROR);
#endif
}



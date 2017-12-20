//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                conoci.cpp   
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
extern int	M4IsUnicode( void ) ;

#include "conoci.hpp"
#include <string.h>


// En los unix en los que no esté definido se define para que compile
#ifndef OCI_UTF16
#define OCI_UTF16           0x00004000        /* mode for all UTF16 metadata */
#endif

// Este define está sólo en oracle10
#ifndef OCI_NCHAR_LITERAL_REPLACE_ON
#define OCI_NCHAR_LITERAL_REPLACE_ON  0x00400000 /* nchar literal replace on */
#endif



ClDBConnection_oci::ClDBConnection_oci (void) 
{
#ifdef DRIVER_OCI

	OCIInitialize((ub4) OCI_THREADED, NULL, NULL, NULL, NULL);
//	OCIInitialize((ub4) OCI_THREADED | OCI_ENV_NO_MUTEX | OCI_NON_BLOCKING, NULL, NULL, NULL, NULL);
         
	m_hEnv = NULL;
	m_hSrv = NULL;
	m_hErr = NULL;
	m_hSes = NULL;
	m_hSvC = NULL;
	m_nErr = 1;
	m_bAutoCommit = M4_TRUE;
#endif
}


ClDBConnection_oci::~ClDBConnection_oci (void)
{
#ifdef DRIVER_OCI
	End ();
#endif
}


m4return_t ClDBConnection_oci::Init (void) 
{
#ifdef DRIVER_OCI

	sword	result = OCI_SUCCESS;


	// UNICODE DB
	if( M4IsUnicode() == 1 )
	{
		/*
		No se puede utilizar la función OCIEnvNlsCreate porque es del api de oracle 9
		y nosotros debemos poder trabajar con oracle 8 (para clientes no unicode)
		Eso si, los .h deben ser de oracle 9 porque sino la macro OCI_UTF16 no está definida
		El .lib también es de oracle 9 (para ir a la par), pero si se utiliza la función OCIEnvCreate
		se puede utilizar la librería de oracle 8
		Hay que tener mucho cuidado de utilizar solo funciones que estén en oracle 8
		result = OCIEnvNlsCreate(&m_hEnv, OCI_NCHAR_LITERAL_REPLACE_ON, NULL, NULL, NULL, NULL, 0, NULL, OCI_UTF16ID, OCI_UTF16ID);
		Se pone OCI_NCHAR_LITERAL_REPLACE_ON, con lo que en unicode sólo podemos trabajar en oracle 10
		*/
		M4DBW_OCI_CALL( result = OCIEnvCreate( &m_hEnv, OCI_UTF16 | OCI_NCHAR_LITERAL_REPLACE_ON, NULL, NULL, NULL, NULL, 0, NULL ) );
	}
	else
	{
		M4DBW_OCI_CALL( result = OCIEnvInit( &m_hEnv, OCI_DEFAULT, 0, NULL ) );
	}

	if (result != OCI_SUCCESS)
	{
		return (M4DBW_OCI_ERROR(result));
	}

	//Server Handle
	M4DBW_OCI_CALL( result = OCIHandleAlloc( m_hEnv, (dvoid**) &m_hSrv, OCI_HTYPE_SERVER, 0, NULL ) );

	if (result != OCI_SUCCESS)
	{
		return (M4DBW_OCI_ERROR(result));
	}

	//Error handle
	M4DBW_OCI_CALL( result = OCIHandleAlloc( m_hEnv, (dvoid**) &m_hErr,OCI_HTYPE_ERROR, 0, NULL ) );

	if (result != OCI_SUCCESS)
	{
		return (M4DBW_OCI_ERROR(result));
	}

	M4DBW_OCI_CALL( result = OCIHandleAlloc( m_hEnv, (dvoid**) &m_hSes, OCI_HTYPE_SESSION, 0, NULL ) );

	if (result != OCI_SUCCESS)
	{
		return (M4DBW_OCI_ERROR(result));
	}

	return (M4_SUCCESS);

#else
	return (M4_ERROR);
#endif
}


m4return_t ClDBConnection_oci::End (void) 
{
#ifdef DRIVER_OCI

	Disconnect ();

	if (m_hSes)
	{
		M4DBW_OCI_CALL( OCIHandleFree( m_hSes, OCI_HTYPE_SESSION ) );
		m_hSes = NULL;
	}

	if (m_hErr)
	{
		M4DBW_OCI_CALL( OCIHandleFree(m_hErr, OCI_HTYPE_ERROR ) ) ;
		m_hErr = NULL;
	}

	if (m_hSrv)
	{
		M4DBW_OCI_CALL( OCIHandleFree(m_hSrv, OCI_HTYPE_SERVER ) );
		m_hSrv = NULL;
	}

	if (m_hEnv)
	{
		M4DBW_OCI_CALL( OCIHandleFree(m_hEnv, OCI_HTYPE_ENV ) );
		m_hEnv = NULL;
	}

	return (M4_SUCCESS);

#else
	return (M4_ERROR);
#endif
}


m4return_t ClDBConnection_oci::Connect (m4pcchar_t ai_pConnStr)
{
#ifdef DRIVER_OCI

	sword			result = OCI_SUCCESS;
	int				iLength = 0;
	unsigned short*	pwcServer = NULL;
	unsigned short*	pwcUser = NULL;
	unsigned short*	pwcPassword = NULL;


	m4pcchar_t pServer = strstr (ai_pConnStr, "SRVR=");

	if (!pServer)
	{
		return (M4_ERROR);
	}

	pServer += 5;

	m4pcchar_t pUser = strstr (pServer, "UID=");

	if (!pUser)
	{
		return (M4_ERROR);
	}

	pUser += 4;

	m4pcchar_t pPassword = strstr (pUser, "PWD=");

	if (!pPassword) {
		return (M4_ERROR);
	}

	pPassword += 4;

	//Server Context  
	M4DBW_OCI_CALL( result = OCIHandleAlloc( m_hEnv, (dvoid**) &m_hSvC, OCI_HTYPE_SVCCTX, 0, NULL ) );

	if (result != OCI_SUCCESS)
	{
		return (M4DBW_OCI_ERROR(result));
	}

	iLength = int( pUser - 5 - pServer ) ;

	// UNICODE DB
	if( M4IsUnicode() == 1 )
	{
		pwcServer = (unsigned short*) M4CppToDB( pServer, iLength ) ;
		iLength *= sizeof( unsigned short ) ;
		M4DBW_OCI_CALL( result = OCIServerAttach( m_hSrv, m_hErr, (text*) pwcServer, iLength, 0 ) );
		delete [] pwcServer ;
	}
	else
	{
		M4DBW_OCI_CALL( result = OCIServerAttach( m_hSrv, m_hErr, (text*) pServer, iLength, 0 ) );
	}

	if (result == OCI_SUCCESS)
	{
		iLength = int( pPassword - 5 - pUser ) ;

		// UNICODE DB
		if( M4IsUnicode() == 1 )
		{
			pwcUser = (unsigned short*) M4CppToDB( pUser, iLength ) ;
			iLength *= sizeof( unsigned short ) ;
			M4DBW_OCI_CALL( OCIAttrSet( m_hSes, OCI_HTYPE_SESSION, (dvoid*) pwcUser, iLength, OCI_ATTR_USERNAME, m_hErr ) );
			delete [] pwcUser ;
		}
		else
		{
			M4DBW_OCI_CALL( OCIAttrSet( m_hSes, OCI_HTYPE_SESSION, (dvoid*) pUser, iLength, OCI_ATTR_USERNAME, m_hErr ) );
		}

		iLength = (int) strlen(pPassword ) ;

		// UNICODE DB
		if( M4IsUnicode() == 1 )
		{
			pwcPassword = (unsigned short*) M4CppToDB( pPassword, iLength ) ;
			iLength *= sizeof( unsigned short ) ;
			M4DBW_OCI_CALL( OCIAttrSet( m_hSes, OCI_HTYPE_SESSION, (dvoid *) pwcPassword, iLength, OCI_ATTR_PASSWORD, m_hErr ) );
			delete [] pwcPassword ;
		}
		else
		{
			M4DBW_OCI_CALL( OCIAttrSet( m_hSes, OCI_HTYPE_SESSION, (dvoid *) pPassword, iLength, OCI_ATTR_PASSWORD, m_hErr ) );
		}

		M4DBW_OCI_CALL( OCIAttrSet( m_hSvC, OCI_HTYPE_SVCCTX, (dvoid*) m_hSrv, 0, OCI_ATTR_SERVER , m_hErr ) );
		M4DBW_OCI_CALL( OCIAttrSet( m_hSvC, OCI_HTYPE_SVCCTX, (dvoid*) m_hSes, 0, OCI_ATTR_SESSION, m_hErr ) );
             
   		M4DBW_OCI_CALL( result = OCISessionBegin( m_hSvC, m_hErr, m_hSes, OCI_CRED_RDBMS, OCI_DEFAULT ) );

		/* Bug 0182388
		Se aborta sólo si es error, no si es warning
		*/
		m4return_t iResult = M4DBW_OCI_ERROR(result);

		if (iResult != M4_ERROR)
		{
			result = OCI_SUCCESS;
		}
		
		if (result != OCI_SUCCESS)
		{
			M4DBW_OCI_CALL( OCIServerDetach( m_hSrv, m_hErr, 0) );
		}
	}
      
	return (M4DBW_OCI_ERROR(result));

#else
	return (M4_ERROR);
#endif
} 


m4return_t ClDBConnection_oci::Disconnect (void)
{
#ifdef DRIVER_OCI

	if (m_hSvC)
	{
		M4DBW_OCI_CALL( OCISessionEnd( m_hSvC, m_hErr, m_hSes, 0 ) );
		M4DBW_OCI_CALL( OCIServerDetach( m_hSrv, m_hErr, 0 ) );
	}
	m_hSvC = NULL;

	return (M4_SUCCESS);
#else
	return (M4_ERROR);
#endif
} 


m4return_t ClDBConnection_oci::Transact (eTransType_t ai_eTransType)
{
#ifdef DRIVER_OCI

	sword result = OCI_SUCCESS; 
	
	switch (ai_eTransType)
	{
		case M4LDB_BEGIN_TRANS:
			M4DBW_OCI_CALL( result = OCITransRollback( m_hSvC, m_hErr, OCI_DEFAULT ) );
			m_bAutoCommit = M4_FALSE;
			break;

		case M4LDB_COMMIT:
			M4DBW_OCI_CALL( result = OCITransCommit( m_hSvC, m_hErr, OCI_DEFAULT ) );
			m_bAutoCommit = M4_TRUE;
			break;

		case M4LDB_ROLLBACK:
			M4DBW_OCI_CALL( result = OCITransRollback( m_hSvC, m_hErr, OCI_DEFAULT ) );
			m_bAutoCommit = M4_TRUE;
			break;
	}

	return (M4DBW_OCI_ERROR(result));
#else
	return (M4_ERROR);
#endif
}


m4return_t ClDBConnection_oci::GetErrorString ( m4return_t ai_retcode, m4int32_t& ao_iErrorCode, m4pchar_t *ao_ppcErrorString )
{
	ao_iErrorCode = 0 ;
	*ao_ppcErrorString = NULL ;

#ifdef DRIVER_OCI

	m4return_t		iResult = M4_ERROR ;
	sword			result = OCI_SUCCESS ;
	int				iLength = 0 ;
	sb4				errcode = 0 ;
	unsigned short	awcErrorMessage[ M4LDB_SIZE_LONG + 1 ] ;


	*awcErrorMessage = '\0' ;

	// UNICODE DB
	M4DBW_OCI_CALL( result = OCIErrorGet( m_hErr, m_nErr++, NULL, &errcode, (OraText*) awcErrorMessage, M4LDB_SIZE_LONG, OCI_HTYPE_ERROR ) );

	ao_iErrorCode = (m4int32_t) errcode;
	
	if (result == OCI_NO_DATA)
	{
		m_nErr = 1;
	}
	
	iResult = M4DBW_OCI_ERROR(result) ;

	if( iResult != M4_ERROR && iResult != M4LDB_SQL_NO_DATA_FOUND )
	{
		// UNICODE DB
		iLength = -1 ;
		*ao_ppcErrorString = M4DBToCpp( awcErrorMessage, iLength ) ;
	}

	return (iResult);
#else
	return (M4_ERROR);
#endif
}


m4return_t ClDBConnection_oci::ExecuteSQL (m4pcchar_t ai_pSQL)
{
#ifdef DRIVER_OCI

	sword			result = OCI_SUCCESS;
	OCIStmt*		hStmt;
	int				iLength = 0;
	unsigned short*	pwcSQL = NULL;


	M4DBW_OCI_CALL( result = OCIHandleAlloc( m_hEnv, (dvoid**)&hStmt, OCI_HTYPE_STMT, 0, NULL ) );

	if (result != OCI_SUCCESS)
	{
		return (M4DBW_OCI_ERROR(result));
	}

	iLength = (int) strlen( ai_pSQL ) ;

	// UNICODE DB
	if( M4IsUnicode() == 1 )
	{
		pwcSQL = (unsigned short*) M4CppToDB( ai_pSQL, iLength ) ;
		iLength *= sizeof( unsigned short ) ;
		M4DBW_OCI_CALL( result = OCIStmtPrepare( hStmt, m_hErr, (text*) pwcSQL, iLength, OCI_NTV_SYNTAX, OCI_DEFAULT ) );
		delete [] pwcSQL ;
	}
	else
	{
		M4DBW_OCI_CALL( result = OCIStmtPrepare( hStmt, m_hErr, (text*) ai_pSQL, iLength, OCI_NTV_SYNTAX, OCI_DEFAULT ) );
	}

	if (result != OCI_SUCCESS)
	{
		return (M4DBW_OCI_ERROR(result));
	}

	M4DBW_OCI_CALL( result = OCIStmtExecute( m_hSvC, hStmt, m_hErr, 1, 0, NULL, NULL, OCI_DEFAULT ) );

	if (result != OCI_SUCCESS)
	{
		return (M4DBW_OCI_ERROR(result));
	}

	M4DBW_OCI_CALL( result = OCIHandleFree( hStmt, OCI_HTYPE_STMT ) );

	return (M4DBW_OCI_ERROR(result));
#else // DRIVER_OCI
	return (M4_ERROR);
#endif
}


m4return_t ClDBConnection_oci::SetIsolationLevel (eIsolationLevel_t ai_eILevel)
{
#ifdef DRIVER_OCI

	m4return_t result = M4_SUCCESS;

	switch (ai_eILevel)
	{
		case M4LDB_IL_READ_COMMITED:
			M4DBW_OCI_CALL( result = ExecuteSQL( "ALTER SESSION SET ISOLATION_LEVEL = READ COMMITTED" ) );
			break;

		case M4LDB_IL_SERIALIZABLE:
			M4DBW_OCI_CALL( result = ExecuteSQL( "ALTER SESSION SET ISOLATION_LEVEL = SERIALIZABLE" ) );
			break;

		default: 
			// No soportada.
			return (M4_SUCCESS);
	}
 
	return (M4DBW_OCI_ERROR(result));
#else // DRIVER_OCI
	return (M4_ERROR);
#endif
}

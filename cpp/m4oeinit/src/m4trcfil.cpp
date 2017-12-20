
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                m4trcfil.cpp   
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                17-03-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene la implementación de traza a fichero
//
//
//==============================================================================


#include "m4trcfil.hpp"

#include <stdlib.h>
#include <errno.h>

#include "chaninfo.hpp"
#include "cstracesize.hpp"
#include "executor.hpp"
#include "exertinfo.hpp"
#include "m4date.hpp"
#include "m4mdfac.hpp"
#include "m4objglb.hpp"
#include "m4objreg.hpp"
#include "attributes.hpp"
#include "chlog.hpp"
#include "csres.hpp"
#include "m4hook.h"							// User Info




#define M4_INIT_CS_TRACE_FILE_NAME						"cstrace.txt"
#define M4_INIT_VB_TRACE_FILE_NAME						"vbtrace.txt"



//=================================================================================
//
// ClCsFileTrace
//
// Clase que define las trazas CS a fichero
//
//=================================================================================


ClCsFileTrace :: ClCsFileTrace( const m4int8_t ai_iCsTraceLevel, const m4int8_t ai_iVbTraceLevel)
	: m_pSerializeCsTraceSize(0)
	, m_pDeSerializeCsTraceSize(0)
	, m_iCsTraceLevel(ai_iCsTraceLevel)
	, m_iVbTraceLevel(ai_iVbTraceLevel)
{
	m_bIsInit = M4_FALSE ;

	m_bFromCache = M4_FALSE ;
	m_bCacheOperationsEnabled = M4_FALSE ;

	m_pcCsOutputFile = NULL ;
	m_pcVbOutputFile = NULL ;

	m_pcMethodDescription = NULL ;
	m_pcRole = NULL ;

	*m_acTag = '\0' ;

	m_oBeginningOfEvent = 0 ;
	_GetCurrTime( m_oBeginningOfEventAsDate ) ;
}


ClCsFileTrace :: ~ClCsFileTrace( void )
{
	_ReleaseResources() ;
}


m4return_t	ClCsFileTrace :: Init( ClVMBaseEnv *ai_poEnvironment )
{

	m4return_t	iResult = M4_ERROR ;
	m4pcchar_t	pccDebugDir = NULL ;
	m4char_t	acCsTraceFile[ MAX_PATH + 1 ] ;
	m4char_t	acVbTraceFile[ MAX_PATH + 1 ] ;


	if( m_bIsInit == M4_TRUE )
	{
		return( M4_SUCCESS ) ;
	}

	if( ai_poEnvironment == NULL )
	{
		return( M4_ERROR ) ;
	}

	pccDebugDir = ai_poEnvironment->GetDebugDirectory() ;

	sprintf( acCsTraceFile, "%s%s%s", pccDebugDir, M4_BACK_SLASH, M4_INIT_CS_TRACE_FILE_NAME ) ;
	sprintf( acVbTraceFile, "%s%s%s", pccDebugDir, M4_BACK_SLASH, M4_INIT_VB_TRACE_FILE_NAME ) ;

	iResult = SetOutputFileName( acCsTraceFile, acVbTraceFile ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	m_bCacheOperationsEnabled = ai_poEnvironment->GetCsTraceCacheOperations() ;


	m_bIsInit = M4_TRUE ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClCsFileTrace :: End( void )
{
	if( m_bIsInit == M4_FALSE )
	{
		return( M4_SUCCESS ) ;
	}


	_ReleaseResources() ;

	m_bFromCache = M4_FALSE ;
	m_bCacheOperationsEnabled = M4_FALSE ;

	*m_acTag = '\0' ;

	m_bIsInit = M4_FALSE ;

	return( M4_SUCCESS ) ;
}

m4bool_t ClCsFileTrace :: _CheckTraceOptions(m4bool_t & aio_bDumpCsTrace, m4bool_t & aio_bDumpVbTrace, m4bool_t & aio_bCacheOptions)
{
	// Inicialización.
	aio_bDumpCsTrace = M4_TRUE;
	aio_bDumpVbTrace = M4_TRUE;
	aio_bCacheOptions = M4_TRUE;

	if (m_iCsTraceLevel == CS_TRACE_DISABLE)
	{
		aio_bDumpCsTrace = M4_FALSE;
	}

	if (m_iVbTraceLevel == 0)
	{
		aio_bDumpVbTrace = M4_FALSE;
	}

	// Si no están habilitadas ninguna de las opciones de traza, se devuelve false.
	if (aio_bDumpCsTrace == M4_FALSE && aio_bDumpVbTrace == M4_FALSE)
	{
		return M4_FALSE;
	}

	// Opciones de cachés.
	if (m_bFromCache == M4_TRUE && m_bCacheOperationsEnabled == M4_FALSE)
	{
		aio_bCacheOptions = M4_FALSE;
	}

	return M4_TRUE;
}

m4return_t	ClCsFileTrace :: StartupM4ObjectEngine( void )
{
	m4bool_t bDumpCsTrace, bDumpVbTrace, bCacheOptions;
	if (_CheckTraceOptions(bDumpCsTrace, bDumpVbTrace, bCacheOptions) != M4_TRUE)
	{
		return( M4_SUCCESS ) ;
	}

	if (bDumpCsTrace == M4_TRUE)
	{
		_PutDateTime(bDumpCsTrace, M4_FALSE) ;
		m_oCsRollingFile.PrintStr(" M4ObjectEngine Client Started");
		_PutEndOfLine(bDumpCsTrace, M4_FALSE) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClCsFileTrace :: ShutdownM4ObjectEngine( void )
{
	m4bool_t bDumpCsTrace, bDumpVbTrace, bCacheOptions;
	if (_CheckTraceOptions(bDumpCsTrace, bDumpVbTrace, bCacheOptions) != M4_TRUE)
	{
		return( M4_SUCCESS ) ;
	}

	if (bDumpCsTrace == M4_TRUE)
	{
		_PutDateTime(bDumpCsTrace, M4_FALSE) ;
		m_oCsRollingFile.PrintStr(" M4ObjectEngine Client Stopped");
		_PutEndOfLine(bDumpCsTrace, M4_FALSE) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClCsFileTrace :: Logon( m4pcchar_t ai_pccUser, m4pcchar_t ai_pccServer )
{
	m4bool_t bDumpCsTrace, bDumpVbTrace, bCacheOptions;
	if (_CheckTraceOptions(bDumpCsTrace, bDumpVbTrace, bCacheOptions) != M4_TRUE)
	{
		return( M4_SUCCESS ) ;
	}

	_PutDateTime(bDumpCsTrace, bDumpVbTrace) ;

	// Print into cstrace.
	if (bDumpCsTrace == M4_TRUE)
	{
		m_oCsRollingFile.PrintStr(" User Session Started");

		m4char_t pszData[256];
		if ( ai_pccServer != NULL && *ai_pccServer != '\0' )
		{
			sprintf( pszData, ", Server=%s", ai_pccServer ) ;
			m_oCsRollingFile.PrintStr(pszData);
		}
		
		if ( ai_pccUser != NULL && *ai_pccUser != '\0' )
		{
			sprintf( pszData, ", User=%s", ai_pccUser ) ;
			m_oCsRollingFile.PrintStr(pszData);
		}
	}

	// Print into vbtrace.
	if (bDumpVbTrace == M4_TRUE)
	{
		m_oVbRollingFile.PrintStr(" User Session Started");

		m4char_t pszData[256];
		if ( ai_pccServer != NULL && *ai_pccServer != '\0' )
		{
			sprintf( pszData, ", Server=%s", ai_pccServer ) ;
			m_oVbRollingFile.PrintStr(pszData);
		}
		
		if ( ai_pccUser != NULL && *ai_pccUser != '\0' )
		{
			sprintf( pszData, ", User=%s", ai_pccUser ) ;
			m_oVbRollingFile.PrintStr(pszData);
		}
	}

	_PutEndOfLine(bDumpCsTrace, bDumpVbTrace) ;

	// Print into cstrace.
	if (bDumpCsTrace == M4_TRUE)
	{
		_PutCsTraceLabels(bDumpCsTrace) ;
	}

	// Print into vbtrace.
	if (bDumpVbTrace == M4_TRUE)
	{
		_PutVbTraceLabels(bDumpVbTrace) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClCsFileTrace :: Logoff( m4pcchar_t ai_pccUser, m4pcchar_t ai_pccServer )
{
	m4bool_t bDumpCsTrace, bDumpVbTrace, bCacheOptions;
	if (_CheckTraceOptions(bDumpCsTrace, bDumpVbTrace, bCacheOptions) != M4_TRUE)
	{
		return( M4_SUCCESS ) ;
	}

	_PutDateTime(bDumpCsTrace, bDumpVbTrace) ;

	// Print into cstrace.
	if (bDumpCsTrace == M4_TRUE)
	{
		m_oCsRollingFile.PrintStr(" User Session Stopped");

		m4char_t pszData[256];
		if ( ai_pccServer != NULL && *ai_pccServer != '\0' )
		{
			sprintf( pszData, ", Server=%s", ai_pccServer ) ;
			m_oCsRollingFile.PrintStr(pszData);
		}
		
		if ( ai_pccUser != NULL && *ai_pccUser != '\0' )
		{
			sprintf( pszData, ", User=%s", ai_pccUser ) ;
			m_oCsRollingFile.PrintStr(pszData);
		}
	}

	// Print into vbtrace.
	if (bDumpVbTrace == M4_TRUE)
	{
		m_oVbRollingFile.PrintStr(" User Session Stopped");

		m4char_t pszData[256];
		if ( ai_pccServer != NULL && *ai_pccServer != '\0' )
		{
			sprintf( pszData, ", Server=%s", ai_pccServer ) ;
			m_oVbRollingFile.PrintStr(pszData);
		}
		
		if ( ai_pccUser != NULL && *ai_pccUser != '\0' )
		{
			sprintf( pszData, ", User=%s", ai_pccUser ) ;
			m_oVbRollingFile.PrintStr(pszData);
		}
	}

	_PutEndOfLine(bDumpCsTrace, bDumpVbTrace) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClCsFileTrace :: PreMethodExecutionTrace( IExecutor *ai_poExecutor, ClCMObjectDirectory* ai_poObjectDirectory, m4pcchar_t ai_pccRole, m4bool_t ai_bFromCache )
{
	// Save fromCache
	m_bFromCache = ai_bFromCache ;

	m4bool_t bDumpCsTrace, bDumpVbTrace, bCacheOptions;
	if (_CheckTraceOptions(bDumpCsTrace, bDumpVbTrace, bCacheOptions) != M4_TRUE)
	{
		return( M4_SUCCESS ) ;
	}

	ClExecutorRuntimeInfo	oInfo ;
	string					info ;

	ai_poExecutor->GetRTInfo( &oInfo ) ;
	info = oInfo.GetInfoAsString() ;
	oInfo.Reset();

	m4char_t	acString[ 8 * 1024 ] ;

	if( strlen( info.c_str() ) + 1 < sizeof( acString ) )
	{
		strcpy( acString, info.c_str() ) ;
		strcat( acString, " " ) ;
	}
	else
	{
		strcpy( acString, "??? " ) ;
	}

	strcat( acString, " Object List: " ) ;

	ClChannelsInfo	oAuxChannelsInfo( *ai_poObjectDirectory ) ;

	if( strlen( oAuxChannelsInfo.ConvertToString() ) < sizeof( acString ) - strlen( acString ) )
	{
		strcat( acString, oAuxChannelsInfo.ConvertToString() ) ;
	}
	else
	{
		strcat( acString, "???" ) ;
	}

	_SetMethodDescription( acString ) ;
	_SetRole( ai_pccRole ) ;

	_StorePreTime() ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClCsFileTrace :: PostMethodExecutionTrace( m4uint32_t ai_iOutDataSize, m4uint32_t ai_iInDataSize, m4pcchar_t ai_pccService )
{
	FILE*	pfFile = NULL ;

	m4bool_t bDumpCsTrace, bDumpVbTrace, bCacheOptions;
	if (_CheckTraceOptions(bDumpCsTrace, bDumpVbTrace, bCacheOptions) != M4_TRUE)
	{
		return( M4_SUCCESS ) ;
	}

	if (bCacheOptions == M4_FALSE)
	{
		return( M4_SUCCESS ) ;
	}

	_PutCommonData	(
						bDumpCsTrace, 
						bDumpVbTrace,
						ai_pccService, // Pendiente el servicio ... o COnversational
						ai_iOutDataSize,
						ai_iInDataSize
					) ;

	// Print into cstrace
	if (bDumpCsTrace == M4_TRUE)
	{
		m_oCsRollingFile.PrintStr(m_pcMethodDescription);
	}

	// Print into vbtrace
	if (bDumpVbTrace == M4_TRUE)
	{
		m_oVbRollingFile.PrintStr(m_pcMethodDescription);
	}

	_PutEndOfLine(bDumpCsTrace, bDumpVbTrace) ;

	if (m_pSerializeCsTraceSize != NULL)
	{
		pfFile = m_oCsRollingFile.GetFile() ;

		if( pfFile != NULL )
		{
			m_pSerializeCsTraceSize->WriteToFile(pfFile);
		}

		delete m_pSerializeCsTraceSize;
		m_pSerializeCsTraceSize = NULL;
		_PutEndOfLine(bDumpCsTrace, M4_FALSE);
	}

	if (m_pDeSerializeCsTraceSize != NULL)
	{
		pfFile = m_oCsRollingFile.GetFile() ;

		if( pfFile != NULL )
		{
			m_pDeSerializeCsTraceSize->WriteToFile(pfFile);
		}

		delete m_pDeSerializeCsTraceSize;
		m_pDeSerializeCsTraceSize = NULL;
		_PutEndOfLine(bDumpCsTrace, M4_FALSE);
	}

	return( M4_SUCCESS ) ;
}

	
m4return_t	ClCsFileTrace :: PreMetaDataTrace( void )
{
	m4bool_t bDumpCsTrace, bDumpVbTrace, bCacheOptions;
	if (_CheckTraceOptions(bDumpCsTrace, bDumpVbTrace, bCacheOptions) != M4_TRUE)
	{
		return( M4_SUCCESS ) ;
	}

	_StorePreTime() ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClCsFileTrace :: PostMetaDataTrace( m4uint32_t ai_iOutDataSize, m4uint32_t ai_iInDataSize, m4pcchar_t ai_pccM4Obj, m4uint8_t ai_iCsType, m4pcchar_t ai_pccRole, m4bool_t ai_bFromCache )
{
	// Save fromCache
	m4bool_t	bAuxFromCache ;	
	bAuxFromCache = m_bFromCache ;
	m_bFromCache = ai_bFromCache ;

	m4bool_t bDumpCsTrace, bDumpVbTrace, bCacheOptions;
	if (_CheckTraceOptions(bDumpCsTrace, bDumpVbTrace, bCacheOptions) != M4_TRUE)
	{
		return( M4_SUCCESS ) ;
	}

	if (bCacheOptions == M4_FALSE)
	{
		return( M4_SUCCESS ) ;
	}
	
	_SetRole( ai_pccRole ) ;

	_PutCommonData	(
						bDumpCsTrace, 
						bDumpVbTrace,
						"MetaChannel",
						ai_iOutDataSize,
						ai_iInDataSize
					) ;

	m_bFromCache = bAuxFromCache ;

	// Print into cstrace
	if (bDumpCsTrace == M4_TRUE)
	{
		m4char_t pszData[256];
		sprintf( pszData, "%s, %s", ai_pccM4Obj, M4ClCsType( ai_iCsType ) ) ;
		m_oCsRollingFile.PrintStr(pszData);
	}

	// Print into vbtrace
	if (bDumpVbTrace == M4_TRUE)
	{
		m4char_t pszData[256];
		sprintf( pszData, "%s, %s", ai_pccM4Obj, M4ClCsType( ai_iCsType ) ) ;
		m_oVbRollingFile.PrintStr(pszData);
	}

	_PutEndOfLine(bDumpCsTrace, bDumpVbTrace) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClCsFileTrace :: PrePresentationTrace( void )
{
	m4bool_t bDumpCsTrace, bDumpVbTrace, bCacheOptions;
	if (_CheckTraceOptions(bDumpCsTrace, bDumpVbTrace, bCacheOptions) != M4_TRUE)
	{
		return( M4_SUCCESS ) ;
	}

	_StorePreTime() ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClCsFileTrace :: PostPresentationTrace( m4uint32_t ai_iOutDataSize, m4uint32_t ai_iInDataSize, m4pcchar_t ai_pccPresentation, const ClPresentationAttributes & ai_ClPresentationAttributes, m4bool_t ai_bFromCache )
{
	m4bool_t	bAuxFromCache ;
	bAuxFromCache = m_bFromCache ;
	m_bFromCache = ai_bFromCache ;
		
	m4bool_t bDumpCsTrace, bDumpVbTrace, bCacheOptions;
	if (_CheckTraceOptions(bDumpCsTrace, bDumpVbTrace, bCacheOptions) != M4_TRUE)
	{
		return( M4_SUCCESS ) ;
	}

	if (bCacheOptions == M4_FALSE)
	{
		return( M4_SUCCESS ) ;
	}

	_SetRole( ai_ClPresentationAttributes.m_pRole ) ;

	_PutCommonData	(
						bDumpCsTrace, 
						bDumpVbTrace,
						"Presentation",
						ai_iOutDataSize,
						ai_iInDataSize
					) ;

	m_bFromCache = bAuxFromCache ;

	// Print into cstrace.
	if (bDumpCsTrace == M4_TRUE)
	{
		m_oCsRollingFile.PrintStr(ai_pccPresentation);
	}

	// Print into vbtrace.
	if (bDumpVbTrace == M4_TRUE)
	{
		m_oVbRollingFile.PrintStr(ai_pccPresentation);
	}

	_PutEndOfLine(bDumpCsTrace, bDumpVbTrace) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClCsFileTrace :: PreSecurityTrace( void )
{
	m4bool_t bDumpCsTrace, bDumpVbTrace, bCacheOptions;
	if (_CheckTraceOptions(bDumpCsTrace, bDumpVbTrace, bCacheOptions) != M4_TRUE)
	{
		return( M4_SUCCESS ) ;
	}

	_StorePreTime() ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClCsFileTrace :: PostSecurityTrace( m4uint32_t ai_iOutDataSize, m4uint32_t ai_iInDataSize, m4pcchar_t ai_pccM4Obj, m4uint8_t ai_iCsType, m4pcchar_t ai_pccRole, m4bool_t ai_bFromCache )
{
	_SetRole( ai_pccRole ) ;
	m_bFromCache = ai_bFromCache ;

	m4bool_t bDumpCsTrace, bDumpVbTrace, bCacheOptions;
	if (_CheckTraceOptions(bDumpCsTrace, bDumpVbTrace, bCacheOptions) != M4_TRUE)
	{
		return( M4_SUCCESS ) ;
	}

	if (bCacheOptions == M4_FALSE)
	{
		return M4_SUCCESS;
	}

	_PutCommonData	(
						bDumpCsTrace, 
						bDumpVbTrace,
						"Security",
						ai_iOutDataSize,
						ai_iInDataSize
					) ;

	// Print into cstrace.
	if (bDumpCsTrace == M4_TRUE)
	{
		m4char_t pszData[256];
		sprintf( pszData, "%s, %s", ai_pccM4Obj, M4ClCsType( ai_iCsType ) ) ;
		m_oCsRollingFile.PrintStr(pszData);
	}

	// Print into vbtrace.
	if (bDumpVbTrace == M4_TRUE)
	{
		m4char_t pszData[256];
		sprintf( pszData, "%s, %s", ai_pccM4Obj, M4ClCsType( ai_iCsType ) ) ;
		m_oVbRollingFile.PrintStr(pszData);
	}

	_PutEndOfLine(bDumpCsTrace, bDumpVbTrace) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClCsFileTrace :: PrintVBTrace(m4pcchar_t ai_pccService, m4pcchar_t ai_pccObject, m4pcchar_t ai_pccFunction, m4pcchar_t ai_pccAction, m4pcchar_t ai_pccRole, m4pcchar_t ai_pccInitTime, m4pcchar_t ai_pccEndTime, m4pcchar_t ai_pccElapsed)
{
	m4bool_t bDumpCsTrace, bDumpVbTrace, bCacheOptions;
	if (_CheckTraceOptions(bDumpCsTrace, bDumpVbTrace, bCacheOptions) != M4_TRUE)
	{
		return( M4_SUCCESS ) ;
	}

	// Print into vbtrace.
	if (bDumpVbTrace == M4_TRUE)
	{
		/* Bug 0127501
		Para que no se desborde ningún buffer se emplea una cadena dinámica
		*/
		m4string_t	sData ;

		sData.reserve( 1024 ) ;

		// Service, object, function, action.
		sData.append( ai_pccService ) ;
		sData.append( 1, '\t' ) ;
		sData.append( ai_pccObject ) ;
		sData.append( 1, '\t' ) ;
		sData.append( ai_pccFunction ) ;
		sData.append( 1, '\t' ) ;
		sData.append( ai_pccAction ) ;
		sData.append( 1, '\t' ) ;

		// Role
		sData.append( ai_pccRole ) ;
		sData.append( 1, '\t' ) ;

		// Init time y end time.
		sData.append( ai_pccInitTime ) ;
		sData.append( 1, '\t' ) ;
		sData.append( ai_pccEndTime ) ;
		sData.append( 1, '\t' ) ;

		// Elapsed.
		sData.append( ai_pccElapsed ) ;

		// End of line.
		m_oVbRollingFile.PrintLn(sData);
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClCsFileTrace :: Reset( void )
{
	return( SetOutputFileName( m_pcCsOutputFile, m_pcVbOutputFile ) ) ;
}



// Privadas ==============================================================


void	ClCsFileTrace :: _PutSeparator( m4bool_t ai_bDumpCsTrace, m4bool_t ai_bDumpVbTrace )
{
	// Print into cstrace.
	if (ai_bDumpCsTrace == M4_TRUE)
	{
		m_oCsRollingFile.PrintStr("\t");
	}

	// Print into vbtrace.
	if (ai_bDumpVbTrace == M4_TRUE)
	{
		m_oVbRollingFile.PrintStr("\t");
	}
}

void	ClCsFileTrace :: _PutEndOfLine( m4bool_t ai_bDumpCsTrace, m4bool_t ai_bDumpVbTrace )
{
	// Print into cstrace.
	if (ai_bDumpCsTrace == M4_TRUE)
	{
		m_oCsRollingFile.PrintLn("");
	}
	
	// Print into vbtrace.
	if (ai_bDumpVbTrace == M4_TRUE)
	{
		m_oVbRollingFile.PrintLn("");
	}
}

void	ClCsFileTrace :: _PutCsTraceLabels( m4bool_t ai_bDumpCsTrace )
{
	if (ai_bDumpCsTrace == M4_TRUE)
	{
		m_oCsRollingFile.PrintStr("EVENT_COUNT");
		_PutSeparator(ai_bDumpCsTrace, M4_FALSE) ;

		m_oCsRollingFile.PrintStr("TAG");
		_PutSeparator(ai_bDumpCsTrace, M4_FALSE) ;

		m_oCsRollingFile.PrintStr("SERVICE");
		_PutSeparator(ai_bDumpCsTrace, M4_FALSE) ;

		m_oCsRollingFile.PrintStr("ROLE");
		_PutSeparator(ai_bDumpCsTrace, M4_FALSE) ;

		m_oCsRollingFile.PrintStr("INIT_TIME");
		_PutSeparator(ai_bDumpCsTrace, M4_FALSE) ;

		m_oCsRollingFile.PrintStr("END_TIME");
		_PutSeparator(ai_bDumpCsTrace, M4_FALSE) ;

		m_oCsRollingFile.PrintStr("ELAPSED");
		_PutSeparator(ai_bDumpCsTrace, M4_FALSE) ;

		m_oCsRollingFile.PrintStr("FROM_CACHE");
		_PutSeparator(ai_bDumpCsTrace, M4_FALSE) ;

		m_oCsRollingFile.PrintStr("DATA_OUT");
		_PutSeparator(ai_bDumpCsTrace, M4_FALSE) ;

		m_oCsRollingFile.PrintStr("DATA_IN");
		_PutSeparator(ai_bDumpCsTrace, M4_FALSE) ;

		m_oCsRollingFile.PrintStr("INFO");
		_PutEndOfLine(ai_bDumpCsTrace, M4_FALSE) ;
	}
}


void	ClCsFileTrace :: _PutVbTraceLabels( m4bool_t ai_bDumpVbTrace )
{
	if (ai_bDumpVbTrace == M4_TRUE)
	{
		m_oVbRollingFile.PrintStr("SERVICE");
		_PutSeparator(M4_FALSE, ai_bDumpVbTrace) ;

		m_oVbRollingFile.PrintStr("OBJECT");
		_PutSeparator(M4_FALSE, ai_bDumpVbTrace) ;

		m_oVbRollingFile.PrintStr("FUNCTION");
		_PutSeparator(M4_FALSE, ai_bDumpVbTrace) ;

		m_oVbRollingFile.PrintStr("ACTION");
		_PutSeparator(M4_FALSE, ai_bDumpVbTrace) ;

		m_oVbRollingFile.PrintStr("ROLE");
		_PutSeparator(M4_FALSE, ai_bDumpVbTrace) ;

		m_oVbRollingFile.PrintStr("INIT_TIME");
		_PutSeparator(M4_FALSE, ai_bDumpVbTrace) ;

		m_oVbRollingFile.PrintStr("END_TIME");
		_PutSeparator(M4_FALSE, ai_bDumpVbTrace) ;

		m_oVbRollingFile.PrintStr("ELAPSED");
		_PutSeparator(M4_FALSE, ai_bDumpVbTrace) ;

		m_oVbRollingFile.PrintStr("FROM_CACHE");
		_PutSeparator(M4_FALSE, ai_bDumpVbTrace) ;

		m_oVbRollingFile.PrintStr("DATA_OUT");
		_PutSeparator(M4_FALSE, ai_bDumpVbTrace) ;

		m_oVbRollingFile.PrintStr("DATA_IN");
		_PutSeparator(M4_FALSE, ai_bDumpVbTrace) ;

		m_oVbRollingFile.PrintStr("INFO");
		_PutEndOfLine(M4_FALSE, ai_bDumpVbTrace) ;
	}
}

void	ClCsFileTrace :: _GetCurrTime( DateTime_t& ao_oDateTime )
{

   time_t	lTime ;


   time( &lTime ) ;
   M4ClDate :: LocalTime( lTime, ao_oDateTime ) ;
}


void	ClCsFileTrace :: _PutTime( m4bool_t ai_bDumpCsTrace, m4bool_t ai_bDumpVbTrace )
{

	DateTime_t	oCurrentTime ;
	_GetCurrTime( oCurrentTime ) ;

	_PutTime( ai_bDumpCsTrace, ai_bDumpVbTrace, oCurrentTime ) ;
}


void	ClCsFileTrace :: _PutDateTime( m4bool_t ai_bDumpCsTrace, m4bool_t ai_bDumpVbTrace )
{

	DateTime_t	oCurrentTime ;
	_GetCurrTime( oCurrentTime ) ;

	// Build date time.
	m4char_t pszDateTime[16];
	sprintf(pszDateTime, "%04d/%02d/%02d ",
			oCurrentTime.tm_year + 1900,
			oCurrentTime.tm_mon + 1,
			oCurrentTime.tm_mday) ;

	// Print into cstrace
	if (ai_bDumpCsTrace == M4_TRUE)
	{
		m_oCsRollingFile.PrintStr(pszDateTime);
	}

	// Print into vbtrace
	if (ai_bDumpVbTrace == M4_TRUE)
	{
		m_oVbRollingFile.PrintStr(pszDateTime);
	}

	_PutTime( ai_bDumpCsTrace, ai_bDumpVbTrace, oCurrentTime, M4_FALSE ) ;
}

void	ClCsFileTrace :: _PutTime( m4bool_t ai_bDumpCsTrace, m4bool_t ai_bDumpVbTrace, DateTime_t& ai_oTime, m4bool_t ai_bPutSeparator )
{
	// Build time.
	m4char_t pszTime[16];
	sprintf(pszTime, "%02d:%02d:%02d",
				ai_oTime.tm_hour,
				ai_oTime.tm_min,
				ai_oTime.tm_sec) ;

	// Print into cstrace.
	if (ai_bDumpCsTrace == M4_TRUE)
	{
		m_oCsRollingFile.PrintStr(pszTime);
	}

	// Print into vbtrace.
	if (ai_bDumpVbTrace == M4_TRUE)
	{
		m_oVbRollingFile.PrintStr(pszTime);
	}

	if( ai_bPutSeparator == M4_TRUE )
	{
		_PutSeparator(ai_bDumpCsTrace, ai_bDumpVbTrace) ;
	}
}


void	ClCsFileTrace :: _PutEventCount( m4bool_t ai_bDumpCsTrace, m4bool_t ai_bDumpVbTrace )
{
	// Print into cstrace.
	if (ai_bDumpCsTrace == M4_TRUE)
	{
		// Gets the event counter function
		m4char_t pcBuffer[64];
		sprintf(pcBuffer,"%u",GetUserEventsCounter());
		m_oCsRollingFile.PrintStr(pcBuffer);
		_PutSeparator(ai_bDumpCsTrace, M4_FALSE) ;
	}
}

void	ClCsFileTrace :: _PutTag( m4bool_t ai_bDumpCsTrace, m4bool_t ai_bDumpVbTrace )
{
	// Print into cstrace.
	if (ai_bDumpCsTrace == M4_TRUE)
	{
		m_oCsRollingFile.PrintStr(m_acTag);
		_PutSeparator(ai_bDumpCsTrace, M4_FALSE) ;
	}
}


void	ClCsFileTrace :: _PutService( m4bool_t ai_bDumpCsTrace, m4bool_t ai_bDumpVbTrace, m4pcchar_t ai_pccService )
{
	if( ai_pccService != NULL )
	{
		// Print into cstrace.
		if (ai_bDumpCsTrace == M4_TRUE)
		{
			m_oCsRollingFile.PrintStr(ai_pccService);
		}

		// Print into vbtrace.
		if (ai_bDumpVbTrace == M4_TRUE)
		{
			m_oVbRollingFile.PrintStr(ai_pccService);
		}
	}

	_PutSeparator(ai_bDumpCsTrace, ai_bDumpVbTrace) ;
}


void	ClCsFileTrace :: _PutRole( m4bool_t ai_bDumpCsTrace, m4bool_t ai_bDumpVbTrace, m4pcchar_t ai_pccRole )
{
	if( ai_pccRole != NULL )
	{
		// Print into cstrace.
		if (ai_bDumpCsTrace == M4_TRUE)
		{
			m_oCsRollingFile.PrintStr(ai_pccRole);
		}

		// Print into vbtrace.
		if (ai_bDumpVbTrace == M4_TRUE)
		{
			// Tabular las tres nuevas columnas del vbtrace (OBJECT, FUNCTION, ACTION)
			m_oVbRollingFile.PrintStr("\t\t\t");
			m_oVbRollingFile.PrintStr(ai_pccRole);
		}
	}

	_PutSeparator(ai_bDumpCsTrace, ai_bDumpVbTrace) ;
}


void	ClCsFileTrace :: _PutElapsedTime( m4bool_t ai_bDumpCsTrace, m4bool_t ai_bDumpVbTrace, m4double_t ai_dElapsedTimeInSecs )
{
	// Print into cstrace.
	if (ai_bDumpCsTrace == M4_TRUE)
	{
		m4char_t pszData[32];
		sprintf( pszData, "%0.3f", ai_dElapsedTimeInSecs ) ;
		m_oCsRollingFile.PrintStr(pszData);
	}

	// Print into vbtrace.
	if (ai_bDumpVbTrace == M4_TRUE)
	{
		m4char_t pszData[32];
		sprintf( pszData, "%0.3f", ai_dElapsedTimeInSecs ) ;
		m_oVbRollingFile.PrintStr(pszData);
	}

	_PutSeparator(ai_bDumpCsTrace, ai_bDumpVbTrace) ;
}


void	ClCsFileTrace :: _PutFromCache( m4bool_t ai_bDumpCsTrace, m4bool_t ai_bDumpVbTrace, m4bool_t ai_bFromCache )
{
	m4char_t pszFromCache[8];
	if( ai_bFromCache == M4_TRUE )
	{
		strcpy(pszFromCache, "True");
	}
	else
	{
		strcpy(pszFromCache, "False");
	}

	// Print into cstrace.
	if (ai_bDumpCsTrace == M4_TRUE)
	{
		m_oCsRollingFile.PrintStr(pszFromCache);
	}

	// Print into vbtrace.
	if (ai_bDumpVbTrace == M4_TRUE)
	{
		m_oVbRollingFile.PrintStr(pszFromCache);
	}

	_PutSeparator(ai_bDumpCsTrace, ai_bDumpVbTrace) ;
}


void	ClCsFileTrace :: _PutDataSize( m4bool_t ai_bDumpCsTrace, m4bool_t ai_bDumpVbTrace, m4uint32_t ai_iSizeInBytes )
{
	// Print into cstrace.
	if (ai_bDumpCsTrace == M4_TRUE)
	{
		m4char_t pszData[32];
		sprintf( pszData, "%d", ai_iSizeInBytes ) ;
		m_oCsRollingFile.PrintStr(pszData);
	}

	// Print into vbtrace.
	if (ai_bDumpVbTrace == M4_TRUE)
	{
		m4char_t pszData[32];
		sprintf( pszData, "%d", ai_iSizeInBytes ) ;
		m_oVbRollingFile.PrintStr(pszData);
	}

	_PutSeparator(ai_bDumpCsTrace, ai_bDumpVbTrace) ;
}


void	ClCsFileTrace :: _GetPreTime( DateTime_t& ao_oDateTime )
{
	memcpy( &ao_oDateTime, &m_oBeginningOfEventAsDate, sizeof( DateTime_t ) ) ;
}


void	ClCsFileTrace :: _StorePreTime( void )
{
	m_oBeginningOfEvent = ClPrecisionTicker :: GetTimeTick() ;

	_GetCurrTime( m_oBeginningOfEventAsDate ) ;
}


m4millisec_t	ClCsFileTrace :: _GetEllapsedMiliseconds( void )
{
	return( ClPrecisionTicker :: GetAbsDifference( m_oBeginningOfEvent, ClPrecisionTicker :: GetTimeTick() ) ) ;
}


void	ClCsFileTrace :: _PutCommonData( m4bool_t ai_bDumpCsTrace, m4bool_t ai_bDumpVbTrace, m4pcchar_t ai_pccService, m4uint32_t ai_iOutDataSize, m4uint32_t ai_iInDataSize )
{

	DateTime_t	oAuxPreTime ;


	_GetPreTime( oAuxPreTime ) ;

	_PutEventCount(ai_bDumpCsTrace, ai_bDumpVbTrace) ;

	_PutTag(ai_bDumpCsTrace, ai_bDumpVbTrace) ;

	_PutService( ai_bDumpCsTrace, ai_bDumpVbTrace, ai_pccService ) ;

	_PutRole( ai_bDumpCsTrace, ai_bDumpVbTrace, m_pcRole ) ;

	_PutTime( ai_bDumpCsTrace, ai_bDumpVbTrace, oAuxPreTime ) ;

	_PutTime(ai_bDumpCsTrace, ai_bDumpVbTrace) ;

	_PutElapsedTime( ai_bDumpCsTrace, ai_bDumpVbTrace,  _GetEllapsedMiliseconds() / 1000.0 ) ;
	
	_PutFromCache( ai_bDumpCsTrace, ai_bDumpVbTrace, m_bFromCache ) ;

	if( m_bFromCache== M4_FALSE )
	{
		_PutDataSize( ai_bDumpCsTrace, ai_bDumpVbTrace, ai_iOutDataSize ) ;
		_PutDataSize( ai_bDumpCsTrace, ai_bDumpVbTrace, ai_iInDataSize ) ;
	}
	else
	{
		_PutDataSize( ai_bDumpCsTrace, ai_bDumpVbTrace, 0 ) ;
		_PutDataSize( ai_bDumpCsTrace, ai_bDumpVbTrace, 0 ) ;
	}
}


void	ClCsFileTrace :: _SetRole( m4pcchar_t ai_pccRole )
{
	if( m_pcRole != NULL )
	{
		delete [] m_pcRole ;
	}

	if( ai_pccRole == NULL )
	{
		m_pcRole = NULL ;
		return ;
	}

	m_pcRole = new m4char_t[ strlen( ai_pccRole ) + 1 ] ;

	strcpy( m_pcRole, ai_pccRole ) ;
}


void	ClCsFileTrace :: _SetMethodDescription( m4pcchar_t ai_pccMethodDescription )
{
	if( m_pcMethodDescription != NULL )
	{
		delete [] m_pcMethodDescription ;
	}

	if( ai_pccMethodDescription == NULL )
	{
		m_pcMethodDescription = NULL ;
		return ;
	}

	m_pcMethodDescription = new m4char_t[ strlen( ai_pccMethodDescription ) + 1 ] ;

	strcpy( m_pcMethodDescription, ai_pccMethodDescription ) ;
}


m4return_t	ClCsFileTrace :: _OpenCsTraceFile(m4pcchar_t ai_pccCsTraceFile)
{
	if (m_iCsTraceLevel == CS_TRACE_DISABLE)
	{
		return M4_SUCCESS;
	}

	if( ai_pccCsTraceFile == NULL || *ai_pccCsTraceFile == '\0' )
	{
		return( M4_ERROR ) ;
	}

	m_pcCsOutputFile = new m4char_t[ strlen( ai_pccCsTraceFile ) + 1 ] ;

	if( m_pcCsOutputFile == NULL )
	{
		return( M4_ERROR ) ;
	}

	strcpy( m_pcCsOutputFile, ai_pccCsTraceFile ) ;

	if (m_oCsRollingFile.SetFile(m_pcCsOutputFile) != M4_SUCCESS)
	{
		/* Bug 0094040
		Se pone un mensaje de error explícito
		*/
		DUMP_CHLOG_ERRORF( M4_CS_NO_OPEN_FILE, m_pcCsOutputFile << strerror( errno ) ) ; 
		delete [] m_pcCsOutputFile ;
		m_pcCsOutputFile = NULL ;
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClCsFileTrace :: _OpenVbTraceFile(m4pcchar_t ai_pccVbTraceFile)
{
	if (m_iVbTraceLevel == 0)
	{
		return M4_SUCCESS;
	}

	if( ai_pccVbTraceFile == NULL || *ai_pccVbTraceFile == '\0' )
	{
		return( M4_ERROR ) ;
	}

	m_pcVbOutputFile = new m4char_t[ strlen( ai_pccVbTraceFile ) + 1 ] ;

	if( m_pcVbOutputFile == NULL )
	{
		return( M4_ERROR ) ;
	}

	strcpy( m_pcVbOutputFile, ai_pccVbTraceFile ) ;

	if (m_oVbRollingFile.SetFile(m_pcVbOutputFile) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF( M4_CS_NO_OPEN_FILE, m_pcVbOutputFile << strerror( errno ) ) ; 
		delete [] m_pcVbOutputFile ;
		m_pcVbOutputFile = NULL ;
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClCsFileTrace :: SetOutputFileName( m4pcchar_t ai_pccCsTraceFile, m4pcchar_t ai_pccVbTraceFile)
{
	if( _ReleaseResources() != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// Open cstrace.
	if (_OpenCsTraceFile(ai_pccCsTraceFile) != M4_SUCCESS)
	{
		return( M4_ERROR ) ;
	}

	// Open vbtrace.
	if (_OpenVbTraceFile(ai_pccVbTraceFile) != M4_SUCCESS)
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}

m4return_t ClCsFileTrace::GetOutputFileName (m4pchar_t ao_pzFileName)
{
   if (m_pcCsOutputFile==NULL)
   {
      return M4_ERROR;
   }

   strcpy(ao_pzFileName,m_pcCsOutputFile);

   return M4_SUCCESS;

}

m4return_t	ClCsFileTrace :: _ReleaseResources( void )
{
	// Liberar cstrace
	if( m_pcCsOutputFile != NULL )
	{
		delete [] m_pcCsOutputFile ;
		m_pcCsOutputFile = NULL ;
	}

	// Liberar vbtrace
	if( m_pcVbOutputFile != NULL )
	{
		delete [] m_pcVbOutputFile ;
		m_pcVbOutputFile = NULL ;
	}

	if( m_pcRole != NULL )
	{
		delete [] m_pcRole ;
		m_pcRole = NULL ;
	}

	if( m_pcMethodDescription != NULL )
	{
		delete [] m_pcMethodDescription ;
		m_pcMethodDescription = NULL ;
	}

	if (m_pSerializeCsTraceSize != NULL)
	{
		delete m_pSerializeCsTraceSize;
		m_pSerializeCsTraceSize = NULL;
	}

	if (m_pDeSerializeCsTraceSize != NULL)
	{
		delete m_pDeSerializeCsTraceSize;
		m_pDeSerializeCsTraceSize = NULL;
	}

	return( M4_SUCCESS ) ;
}


m4return_t ClCsFileTrace::SetTagName (m4pchar_t ai_pzNewTabName)
{
   ResetTagName();

   if (ai_pzNewTabName==NULL)
   {
      return M4_SUCCESS;
   }

   
   strncpy(m_acTag,ai_pzNewTabName,M4_CS_TRACE_MAX_TAG_SIZE);
   m_acTag[M4_CS_TRACE_MAX_TAG_SIZE]=0; //nulterminamos
   
   return M4_SUCCESS;
}

m4return_t ClCsFileTrace::ResetTagName (void)
{
   m_acTag[0]=0;

   return M4_SUCCESS;
}

ClCsTraceSize* const ClCsFileTrace::CreateSerializeCsTraceSize()
{
	if (m_iCsTraceLevel < CS_TRACE_CHANNEL)
	{
		return NULL;
	}

	/* Bug 007005
	Si no se liberó la vez anterior se libera y continua
	*/
	if( m_pSerializeCsTraceSize != NULL )
	{
		delete( m_pSerializeCsTraceSize ) ;
		m_pSerializeCsTraceSize = NULL ;
	}

	m_pSerializeCsTraceSize = new ClCsTraceSize(0, "SERIALIZE", m_iCsTraceLevel, ClCsTraceSize::NONE);
	return m_pSerializeCsTraceSize;
}

ClCsTraceSize* const ClCsFileTrace::CreateDeSerializeCsTraceSize()
{
	if (m_iCsTraceLevel < CS_TRACE_CHANNEL)
	{
		return NULL;
	}

	/* Bug 007005
	Si no se liberó la vez anterior se libera y continua
	*/
	if( m_pDeSerializeCsTraceSize != NULL )
	{
		delete( m_pDeSerializeCsTraceSize ) ;
		m_pDeSerializeCsTraceSize = NULL ;
	}

	m_pDeSerializeCsTraceSize = new ClCsTraceSize(0, "DESERIALIZE", m_iCsTraceLevel, ClCsTraceSize::NONE);
	return m_pDeSerializeCsTraceSize;
}

//	=============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
// 
//
//	=============================================================================


#include "logmanag.hpp"
#include <time.h>
#include <perslog.hpp>
#include <debuglog.hpp>
#include <m4types.hpp>
#include <m4string.hpp>
#include <stdio.h>
#include "m4buildversion.hpp"
#include "m4unicode.hpp"
#include "cldates.hpp"
#include "m4logfile.hpp"
#include "m4loadll.hpp"
#include "regedit.hpp"
#include "m4regdef.hpp"


// ----------------------------------------- logsys_Wrapper.

// Levanta la m4logsys.dll dinamicamente y llama a la funcion 
// de traduccion.

#define M4_NEW_REGEDIT_KEY_CVM						"CVM"
#define M4_INIT_WORK_DIR							"WorkDir"
#define M4CH_MAX_WORKING_DIRECTORY				    500
#define M4CH_MAX_TEMP_BUFFER						500

m4bool_t			sIsM4LogsysLoaded = M4_FALSE;
m4bool_t			sTryLoadM4Logsys  = M4_FALSE;
M4ClLoaDll			pM4LogSysDLL;



/*
Obtiene el valor del workdir.
Primero mira en la variable de entorno.
Después en el registro server.
Después en el resgistro cliente.
La inicialización se hace una vez bloqueando con mutex y haciendo doble comprobación
*/
static	m4pcchar_t	M4GetWorkingDirectory( void )
{

	m4uint32_t	iSize = 0 ;
	m4pcchar_t	pccPath = NULL ;
	m4pcchar_t	pccSide = NULL ;
	m4char_t	acPath[ M4CH_MAX_TEMP_BUFFER + 1 ] = "" ;

	static	m4bool_t	b_acWorkingInit = M4_FALSE ;
	static	m4char_t	s_acWorkingDirectory[ M4CH_MAX_WORKING_DIRECTORY + 1 ] = "" ;
	static	ClMutex		s_WorkingMutex( M4_TRUE );


	if( b_acWorkingInit == M4_FALSE )
	{
		s_WorkingMutex.Lock() ;

		if( b_acWorkingInit == M4_FALSE )
		{
			b_acWorkingInit = M4_TRUE ;

			pccPath = getenv( "M4_LOGMSG_PATH" ) ;
		
			// bg 164708
			if( pccPath != NULL && *pccPath != '\0' )
			{
				strcpy ( s_acWorkingDirectory, pccPath ) ;
			}
			else
			{
				if( getenv( "M4SERVER_PATH" ) != NULL )
				{
					pccSide = M4_NEW_REGEDIT_KEY_SERVER ;
				}
				else
				{
					pccSide = M4_NEW_REGEDIT_KEY_CLIENT ;
				}

				M4ComposeRegistryPath( pccSide, M4_NEW_REGEDIT_KEY_CVM, acPath, M4CH_MAX_TEMP_BUFFER, iSize ) ;
				memset( s_acWorkingDirectory, '\0', M4CH_MAX_WORKING_DIRECTORY + 1 ) ;

				if( GetRegKeyData( acPath, M4_INIT_WORK_DIR, s_acWorkingDirectory ) == FALSE )
				{
					memset( s_acWorkingDirectory, '\0', M4CH_MAX_WORKING_DIRECTORY + 1 ) ;
				}
			}
		}

		s_WorkingMutex.Unlock() ;
	}

	return( s_acWorkingDirectory ) ;
}




/* Bug 0102154
Se cambian los 3 por '#' por si estaban escapados
*/
static	m4void_t	M4AppedUnescaped( M4ClString &ai_rsDestination, const M4ClString& ai_rsSource )
{

	size_t		i = 0 ;
	size_t		iStart = 0 ;
	size_t		iEnd = 0 ;
	m4pchar_t	pcMessage = NULL ;


	iStart = ai_rsDestination.size() ;

	ai_rsDestination += ai_rsSource ;	

	iEnd = ai_rsDestination.size() ;
	pcMessage = ai_rsDestination ;

	for( i = iStart ; i < iEnd ; i++ )
	{
		if( pcMessage[ i ] == 3 )
		{
			pcMessage[ i ] = '#' ;
		}
	}
}


m4return_t FormatErrorCode_Wrapper (
	
	m4uint32_t ai_ulCode,
	m4pchar_t ai_pszParams,
	m4uchar_t ai_byIdParamPos,
	m4pchar_t ao_pszOutString,
	m4uint32_t ai_uBufferSize,
	m4puint32_t ao_uBufferFilled,
	m4language_t ai_uiLang)
{

	m4return_t	iRet;
	m4typefunreturn		pLogsysFunction;
	m4return_t (*pM4LogSysFormatErrorCode)(m4uint32_t,m4pchar_t,m4uchar_t,m4pchar_t,m4uint32_t,m4puint32_t,m4language_t);

	if( ao_pszOutString != NULL )
	{
		*ao_pszOutString = '\0';
	}

	if (sTryLoadM4Logsys == M4_FALSE)
	{
		sTryLoadM4Logsys = M4_TRUE;

		if (pM4LogSysDLL.Open("m4logsys") != M4_SUCCESS)
		{
			return M4_ERROR;
		}
	
		sIsM4LogsysLoaded = M4_TRUE;
	
		// Inicializamos la M4LogSys

		m4pcchar_t pccWorkDir = M4GetWorkingDirectory() ;

		if( pccWorkDir != NULL && *pccWorkDir != '\0' )
		{
			if (pM4LogSysDLL.GetFunc("LogsysSetWorkingDir_Dll", pLogsysFunction) != M4_SUCCESS)
			{
				return M4_ERROR;
			}

			(*(void(*)(m4pchar_t))pLogsysFunction)( (m4pchar_t) pccWorkDir );
		}
	}

	if (sIsM4LogsysLoaded == M4_TRUE )
	{
		if (pM4LogSysDLL.GetFunc("FormatErrorCode_Dll", pLogsysFunction) != M4_SUCCESS)
		{
			return M4_ERROR;
		}

		pM4LogSysFormatErrorCode = (m4return_t(*)(m4uint32_t,m4pchar_t,m4uchar_t,m4pchar_t,m4uint32_t,m4puint32_t,m4language_t))pLogsysFunction;
		iRet = (*pM4LogSysFormatErrorCode)(ai_ulCode, ai_pszParams, ai_byIdParamPos, ao_pszOutString, ai_uBufferSize, ao_uBufferFilled, ai_uiLang);

		if( iRet != M4_ERROR && ao_pszOutString != NULL )
		{
			// Se aplanan los retornos
			while( *ao_pszOutString != '\0' )
			{
				if( *ao_pszOutString == '\n' )
				{
					*ao_pszOutString = ' ';
				}
				ao_pszOutString++;
			}
		}
	}
	else
	{
		iRet = M4_ERROR;
	}

	return iRet;
}



///////////////////////////////////////////////////////////////////////////////////////

ClStorageList::ClStorageList ()
{
}


ClStorageList::~ClStorageList ()
{
	DestroyStorages();
	m_lstorages.clear();
}



m4uint32_t ClStorageList::CheckforErrorQueue ()
{
	m4uint32_t ipos = 0,ret = 0;

	STORAGELIST::iterator	iter;

	for ( iter = m_lstorages.begin(); iter != m_lstorages.end(); iter++ )
	{
		if ((*iter) != NULL)
		{
           ipos++;
		   if ( (*iter) -> GetType() == M4_STORAGE_TYPE_QUEUE)
		   {
			   ret =1;
			   break;
		   }
		}
	}

	if (ret==0)
	{
		return ret;
	}
	else
	{
		return ipos;
	}
}


m4bool_t ClStorageList::StoreToLocalQueue (ClAppItem* pappItem)
{
	m4uint32_t  sflags = pappItem -> GetFlags();
	m4uint32_t  ipos =  CheckforErrorQueue();
    m4uint32_t	nStorages = (m4uint32_t) m_lstorages.size();
	m4uint32_t	mask = (1 << nStorages);

	if (!ipos) return M4_FALSE;

	mask = mask >> ipos;
	if ( mask & sflags)
	{
		return M4_TRUE;
	}
	else
	{
		return M4_FALSE;
	}
}


m4return_t ClStorageList::AddStorage (ClPersistentStorage* pstorage)
{
	m_lstorages.push_back( pstorage );
	return M4_SUCCESS;
}


ClPersistentStorage* ClStorageList::GetStorage (m4int16_t ipos)
{
	STORAGELIST::iterator	iter;
	m4int16_t	icont = 0;

	if ( ( ipos < 0 ) || ( ipos > 3 ) )
	{
		return NULL;
	}

	for ( iter = m_lstorages.begin(); iter != m_lstorages.end(); iter++ )
	{
		if ( icont == ipos )
		{
			return *iter;
		}
		icont ++;
	}

	return NULL;
}


m4return_t ClStorageList::StoreTo (m4uint32_t sflags, ClAppItem* pappItem)
{
	STORAGELIST::iterator	iter;
	m4uint32_t	sflags1 = sflags;
	m4uint32_t  imask;

	if ( ! pappItem )
	{
		return M4_ERROR;
	}

	sflags1 = sflags & ( pappItem -> GetFlags() );
	sflags = sflags1;
	
	imask = 1 << (m_lstorages.size()-1);
	
	for ( iter = m_lstorages.begin(); iter != m_lstorages.end(); iter++ )
	{		
		if ( * iter )
		{
			if (sflags & imask )
			{
				if ( (*iter)->GetType() != M4_STORAGE_TYPE_QUEUE)
				{
					( *iter )->AddAppoint( pappItem );
				}
			}			
		}
		
		imask >>= 1;
	}
	return M4_SUCCESS;
}


void ClStorageList::DestroyStorages ()
{
	STORAGELIST::iterator	siter, siter1;
	ClPersistentStorage	*pstorage;

	siter = m_lstorages.begin();
	while ( siter != m_lstorages.end() )
	{
		pstorage = *siter;
		if ( pstorage ) delete pstorage;
		m_lstorages.erase( siter );
		siter = m_lstorages.begin();
	}
}





ClFileStorage::ClFileStorage (ClLogManager* ai_plogManager, m4pcchar_t ai_pccName, m4int16_t ai_iFileFormat, m4pcchar_t ai_pccDevice )
: ClPersistentStorage( ai_plogManager, M4_STORAGE_TYPE_FILE )
{

	m4bool_t	bIsUnicode = M4_TRUE;
	m4uint32_t	iSize = 5 * 1024 * 1024;
	m4pchar_t	pccExtension = "log";
	m4char_t	acHeader[ 1024 + 1 ] = "";

	m_iFileFormat = ai_iFileFormat;

	if( ai_iFileFormat == M4_LOGFILE_HTML_FORMAT )
	{
		iSize = 1024 * 1024;
		bIsUnicode = M4_FALSE;
		pccExtension = "html";
		strcpy( acHeader, "<!-- Meta4 " );
		strcat( acHeader, ai_pccDevice );
		strcat( acHeader, " -->\n" );
		strcat( acHeader, M4HTMLHeader() );
		strcat( acHeader, "<font size='4' face='Verdana'><br>Meta4 " );
		strcat( acHeader, ai_pccDevice );
		strcat( acHeader, "<br>\n" );
	}
	else
	{
		strcpy( acHeader, "Meta4 " );
		strcat( acHeader, ai_pccDevice );
	}

	m_poLogFile = new M4LogFile( ai_pccName, iSize, acHeader, pccExtension, M4_FALSE, M4_FALSE, M4_FALSE, bIsUnicode );
}


ClFileStorage::~ClFileStorage ()
{
	if ( m_poLogFile != NULL )
	{
		delete( m_poLogFile ) ;
		m_poLogFile = NULL ;
	}
}


m4return_t ClFileStorage::AddAppoint (ClAppItem* pAppItem)
{

	m4uint32_t	iSize = 0;
	m4return_t	iResult = M4_ERROR;
	m4pchar_t	pcText = NULL;
	M4ClString	*psUser = NULL;
	M4ClString	sText = "";
	m4char_t    acNumber[ 128 + 1 ];
	ClLogSystem *poLog = NULL;
	m4pchar_t	pcString;
	int			iNumber;


	if( pAppItem == NULL || m_pLogManager == NULL )
	{
		return M4_ERROR;
	}

	m_poLogFile->WriteLineStart();

	sprintf( acNumber, "%u", pAppItem->GetErrCode() );

	if( m_iFileFormat == M4_LOGFILE_HTML_FORMAT )
	{
		m_poLogFile->WriteString( "<br>" );
		m_poLogFile->WriteString( "<font color='#330099'>log_type.</font>\t" );
		m_poLogFile->WriteString( pAppItem->GetErrType().c_str() );
		m_poLogFile->WriteString( "\t<br>" );

		if( pAppItem->IsActive_Time() == M4_TRUE )
		{
			m_poLogFile->WriteString( "<font color='#330099'>log_time.</font>\t" );
			m_poLogFile->WriteString( pAppItem->GetTime().c_str() );
			m_poLogFile->WriteString( "\t<br>" );
		}

		m_poLogFile->WriteString( "<font color='#330099'>log_code.</font><font color='#B40404'>\t" );
		m_poLogFile->WriteString( acNumber );
		m_poLogFile->WriteString( "\t</font>\t(" );
		m_poLogFile->WriteString( pAppItem->GetStrErrCode().c_str() );
		m_poLogFile->WriteString( ")\t<br>" );

		if( m_pLogManager->isTranslatable() & M4LOG_TRANSLATABLE )
		{
			iResult = FormatErrorCode_Wrapper(	pAppItem->GetErrCode(), pAppItem->GetBuffer(), 0, NULL, 0, &iSize, m_pLogManager->getDefaultIdiom() );
			
			m_poLogFile->WriteString( "<font color='#330099'>log_message.</font>\t" );

			if( iResult != M4_ERROR )
			{
				pcText = (m4pchar_t) malloc( iSize );
				iResult = FormatErrorCode_Wrapper(	pAppItem->GetErrCode(),	pAppItem->GetBuffer(), 0, pcText, iSize, &iSize, m_pLogManager->getDefaultIdiom() );
				m_poLogFile->WriteString( pcText );
				free( pcText );
			}
			else
			{
				// Bug 0102154 Se desescapan los #
				M4AppedUnescaped( sText, pAppItem->GetBuffer() ) ;
				m_poLogFile->WriteString( sText.c_str() );
			}
			m_poLogFile->WriteString( "\t<br>" );
		} 

		if( m_pLogManager->isTranslatable() & M4LOG_NO_TRANSLATABLE )
		{
			// Bug 0102154 Se desescapan los #
			M4AppedUnescaped( sText, pAppItem->GetBuffer() ) ;
			m_poLogFile->WriteString( "<font color='#330099'>log_params.</font>\t" );
			m_poLogFile->WriteString( sText.c_str() );
			m_poLogFile->WriteString( "\t<br>" );
		}

		poLog = GETPTHREADLOGSYSTEM();

		if( poLog != NULL )
		{
			psUser = poLog->GetAttributeValue( "M4_LOGSYSTEM_USERID" ) ;

			if( psUser != NULL && psUser->size() != 0 )
			{
				m_poLogFile->WriteString( "<font color='#330099'>log_user.</font>\t" );
				m_poLogFile->WriteString( psUser->c_str() );
				m_poLogFile->WriteString( "\t<br>" );
			}

			// eh 279583. Adds user-timed-actions-auditory primary key columns in trace files.
			pcString = poLog->GetSessionKey();
			if( pcString != NULL )
			{
				m_poLogFile->WriteString( "<font color='#330099'>log_auditory.</font>\t" );
				m_poLogFile->WriteString( "\t" );
				m_poLogFile->WriteString( pcString );
				m_poLogFile->WriteString( "\t" );
			}

			pcString = poLog->GetServerName();
			if( pcString != NULL )
			{
				m_poLogFile->WriteString( pcString );
				m_poLogFile->WriteString( "\t" );
			}

			iNumber = poLog->GetServerPort();
			if( iNumber != -1 )
			{
				sprintf( acNumber, "%d\t", iNumber );
				m_poLogFile->WriteString( acNumber );
			}

			iNumber = poLog->GetSessionId();
			if( iNumber != -1 )
			{
				sprintf( acNumber, "%d\t", iNumber );
				m_poLogFile->WriteString( acNumber );
			}

			iNumber = poLog->GetClickNum();
			if( iNumber != -1 )
			{
				sprintf( acNumber, "%d\t", iNumber );
				m_poLogFile->WriteString( acNumber );
			}

			pcString = poLog->GetReqUniqueId();
			if( pcString != NULL )
			{
				m_poLogFile->WriteString( pcString );
				m_poLogFile->WriteString( "\t" );
			}

			m_poLogFile->WriteString( "\t<br>" );
		}
	}
	else
	{
		m_poLogFile->WriteString( "\n" );
		m_poLogFile->WriteString( pAppItem->GetErrType().c_str() );

		if( pAppItem->IsActive_Time() == M4_TRUE )
		{
			m_poLogFile->WriteString( "\t" );
			m_poLogFile->WriteString( pAppItem->GetTime().c_str() );
		}

		m_poLogFile->WriteString( "\t" );
		m_poLogFile->WriteString( acNumber );
		m_poLogFile->WriteString( "\t(" );
		m_poLogFile->WriteString( pAppItem->GetStrErrCode().c_str() );
		m_poLogFile->WriteString( ")\t" );

		if( m_pLogManager->isTranslatable() & M4LOG_TRANSLATABLE )
		{
			iResult = FormatErrorCode_Wrapper(	pAppItem->GetErrCode(), pAppItem->GetBuffer(), 0, NULL, 0, &iSize, m_pLogManager->getDefaultIdiom() );
			
			if( iResult != M4_ERROR )
			{
				pcText = (m4pchar_t) malloc( iSize );
				iResult = FormatErrorCode_Wrapper(	pAppItem->GetErrCode(),	pAppItem->GetBuffer(), 0, pcText, iSize, &iSize, m_pLogManager->getDefaultIdiom() );
				m_poLogFile->WriteString( pcText );
				free( pcText );
			}
			else
			{
				// Bug 0102154 Se desescapan los #
				M4AppedUnescaped( sText, pAppItem->GetBuffer() ) ;
				m_poLogFile->WriteString( sText.c_str() );
			}
		} 

		if( m_pLogManager->isTranslatable() & M4LOG_NO_TRANSLATABLE )
		{
			// Bug 0102154 Se desescapan los #
			M4AppedUnescaped( sText, pAppItem->GetBuffer() ) ;
			m_poLogFile->WriteString( "\t" );
			m_poLogFile->WriteString( sText.c_str() );
		}

		poLog = GETPTHREADLOGSYSTEM();

		if( poLog != NULL )
		{
			psUser = poLog->GetAttributeValue( "M4_LOGSYSTEM_USERID" ) ;

			if( psUser != NULL && psUser->size() != 0 )
			{
				m_poLogFile->WriteString( "\t" );
				m_poLogFile->WriteString( psUser->c_str() );
			}

			// eh 279583. Adds user-timed-actions-auditory primary key columns in trace files.
			pcString = poLog->GetSessionKey();
			if( pcString != NULL )
			{
				m_poLogFile->WriteString( "\t" );
				m_poLogFile->WriteString( pcString );
				m_poLogFile->WriteString( "\t" );
			}

			pcString = poLog->GetServerName();
			if( pcString != NULL )
			{
				m_poLogFile->WriteString( pcString );
				m_poLogFile->WriteString( "\t" );
			}

			iNumber = poLog->GetServerPort();
			if( iNumber != -1 )
			{
				sprintf( acNumber, "%d\t", iNumber );
				m_poLogFile->WriteString( acNumber );
			}

			iNumber = poLog->GetSessionId();
			if( iNumber != -1 )
			{
				sprintf( acNumber, "%d\t", iNumber );
				m_poLogFile->WriteString( acNumber );
			}

			iNumber = poLog->GetClickNum();
			if( iNumber != -1 )
			{
				sprintf( acNumber, "%d\t", iNumber );
				m_poLogFile->WriteString( acNumber );
			}

			pcString = poLog->GetReqUniqueId();
			if( pcString != NULL )
			{
				m_poLogFile->WriteString( pcString );
				m_poLogFile->WriteString( "\t" );
			}
		}
	}

	m_poLogFile->WriteLineEnd();
	return M4_SUCCESS;
}


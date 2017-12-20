//## begin module%3912A1F600AA.cm preserve=no
//## end module%3912A1F600AA.cm

//## begin module%3912A1F600AA.cp preserve=no
//	=============================================================================
//	=
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
//## end module%3912A1F600AA.cp

//## Module: m4regimpl%3912A1F600AA; Package body
//## Subsystem: m4objreg::src%3912A3550163
//## Source file: D:\evol\m4objreg\src\m4regimpl.cpp

//## begin module%3912A1F600AA.additionalIncludes preserve=no
//## end module%3912A1F600AA.additionalIncludes

//## begin module%3912A1F600AA.includes preserve=yes
//==============================================================================
//
// (c) Copyright  1991-2000 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4objreg
// File:                m4objreg.cpp
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                07-03-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module implements the Registry class methods for Client & Server
//
//
//==============================================================================

#include "m4objreg.hpp"
#include "m4regres.hpp"		// Errores
#include "regedit.hpp"		// M4Ini
#include "m4win2ux.hpp"		// M4win2ux + M4Ipcs
#include "m4objglb.hpp"		// M4ObjGlb
#include "m4objlog.hpp"		// M4ObjLog
#include <stdio.h>
#include <ctype.h>
//## end module%3912A1F600AA.includes

#include <m4types.hpp>
#include <m4regimpl.hpp>
#include <blocksynchronization.hpp>
#include "m4buildversion.hpp"
#include "m4logfile.hpp"

extern int	M4IsUnicode( void ) ;


//## begin module%3912A1F600AA.declarations preserve=no
//## end module%3912A1F600AA.declarations

//## begin module%3912A1F600AA.additionalDeclarations preserve=yes

// initial timeout define
#define M4_CS_DEFAULT_TIMEOUT 86400

// Comprobar si el primer carcater de una cadena es un digito
m4bool_t RegIsDigit(m4pcchar_t ai_pcString)
{
	while (*ai_pcString  != '\0')
	{
		if (!isdigit(*ai_pcString))
			return M4_FALSE;

		ai_pcString++;
	}
	return M4_TRUE;
}




// ============= Metodos static para Propiedades Especiales de Registry ======================
//## end module%3912A1F600AA.additionalDeclarations


// Class ClM4ObjRegServer 

ClM4ObjRegServer::ClM4ObjRegServer ()
  //## begin ClM4ObjRegServer::ClM4ObjRegServer%1356527309.hasinit preserve=no
  //## end ClM4ObjRegServer::ClM4ObjRegServer%1356527309.hasinit
  //## begin ClM4ObjRegServer::ClM4ObjRegServer%1356527309.initialization preserve=yes
  //## end ClM4ObjRegServer::ClM4ObjRegServer%1356527309.initialization
{
  //## begin ClM4ObjRegServer::ClM4ObjRegServer%1356527309.body preserve=yes
	Reset();
  //## end ClM4ObjRegServer::ClM4ObjRegServer%1356527309.body
}


ClM4ObjRegServer::~ClM4ObjRegServer ()
{
  //## begin ClM4ObjRegServer::~ClM4ObjRegServer%-37440012.body preserve=yes
	Reset();
  //## end ClM4ObjRegServer::~ClM4ObjRegServer%-37440012.body
}



//## Other Operations (implementation)
m4return_t ClM4ObjRegServer::Init (void )
{
  //## begin ClM4ObjRegServer::Init%-1705269378.body preserve=yes
    if( IsInit()  ) {
        return( M4_SUCCESS ) ;
    }

	// Ponemos el Path de CLIENT
	m4return_t ret = SetPath(M4_NEW_REGEDIT_KEY_SERVER);
	if (ret == M4_SUCCESS) {
		// Inicializar el registry de las bases
		ret = ClVMBaseEnv::Init( GetPath() );
		if (ret == M4_SUCCESS) {
			// Inicializar nuestro registry particular
			ret = ReadRegistry( GetPath() );
		}
	}

	if (ret != M4_SUCCESS) {
		m_bInit = M4_FALSE;
		DUMP_CHLOG_DEBUG( M4_CH_OBJREG_NO_READ_REGISTRY ) ;
	} else {
		m_bInit = M4_TRUE;
	}

	return ret;
  //## end ClM4ObjRegServer::Init%-1705269378.body
}

m4return_t ClM4ObjRegServer::ReadRegistry (m4pchar_t ai_pcPath)
{
  //## begin ClM4ObjRegServer::ReadRegistry%-825915481.body preserve=yes
	if (!ai_pcPath)
		return M4_ERROR;

	m4return_t iResult = M4_SUCCESS;

	// Leemos el registry de la Cache
	iResult = ReadCacheRegistry(ai_pcPath);

	return iResult ;
  //## end ClM4ObjRegServer::ReadRegistry%-825915481.body
}

m4return_t ClM4ObjRegServer::ReadCacheRegistry (m4pchar_t ai_pcPath)
{
  //## begin ClM4ObjRegServer::ReadCacheRegistry%-1576364119.body preserve=yes
	if (!ai_pcPath)
		return M4_ERROR;

	m4char_t    acPath[ M4CH_MAX_TEMP_BUFFER + 1 ] ;
	//path para CVM\CACHE
	strcpy(acPath, ai_pcPath);
	strcat(acPath, M4_NEW_REGEDIT_KEY_CACHE);

	// leemos el directorio de caché
    memset( m_acCacheDirectory, '\0', M4CH_MAX_WORKING_DIRECTORY + 1 ) ;
	if( !GetRegKeyData( acPath, M4_INIT_CACHE_DIR, m_acCacheDirectory ) )
    {
		strcpy(m_acCacheDirectory, m_acTempDirectory);
	}
	strcat(m_acCacheDirectory, M4_BACK_SLASH);	//Dejamos preparado el path: c:\temp\"

	// Montamos el directorio cache
	// Establecemos el nombre del directorio de Cache
	strcat(m_acCacheDirectory,"server");

	// Creamos el directorio de caches
	m4return_t iResult = M4RegMkDir(m_acCacheDirectory, M4_FALSE);

	/* Bug 0151033
	Si es unicode se hace en un subdirectorio para que no coincida con las cachés de nounicode
	*/
	if( M4IsUnicode() == 1 )
	{
		strcat( m_acCacheDirectory, M4_BACK_SLASH ) ;
		strcat( m_acCacheDirectory, "unicode" ) ;
		iResult = M4RegMkDir( m_acCacheDirectory, M4_FALSE ) ;
	}

	// Creamos el directorio emul\LDB o client\LDB o server\LDB (si tenemos que utilizarlo)
	if (m_iLDBDiskCache & M4CL_CACHE_READ_WRITE){
		m4char_t    BDLCacheDirectory  [ M4CH_MAX_WORKING_DIRECTORY + 1 ] ;
		strcpy(BDLCacheDirectory ,GetCacheDirectory() );
		strcat(BDLCacheDirectory , M4_BACK_SLASH);
		strcat(BDLCacheDirectory ,"LDB");
		iResult = M4RegMkDir(BDLCacheDirectory, M4_FALSE);
	}

	return M4_SUCCESS;
  //## end ClM4ObjRegServer::ReadCacheRegistry%-1576364119.body
}

void ClM4ObjRegServer::Reset ()
{
  //## begin ClM4ObjRegServer::Reset%-2071759270.body preserve=yes
	ClVMBaseEnv::Reset();
  //## end ClM4ObjRegServer::Reset%-2071759270.body
}

// Additional Declarations
  //## begin ClM4ObjRegServer%3912A1F503A3.declarations preserve=yes
  //## end ClM4ObjRegServer%3912A1F503A3.declarations

// Class ClM4ObjRegClient 


//## begin ClM4ObjRegClient::m_iDefaultCSTimeOut%3912A1F6000A.role preserve=no  protected: static m4uint32_t {1 -> 1VHAPN}
m4uint32_t ClM4ObjRegClient::m_iDefaultCSTimeOut = 0;
//## end ClM4ObjRegClient::m_iDefaultCSTimeOut%3912A1F6000A.role

//## begin ClM4ObjRegClient::m_iGenCSTimeOut%3912A1F6000D.role preserve=no  protected: static m4uint32_t {1 -> 1VHAPN}
m4uint32_t ClM4ObjRegClient::m_iGenCSTimeOut = 0;
//## end ClM4ObjRegClient::m_iGenCSTimeOut%3912A1F6000D.role

//## begin ClM4ObjRegClient::m_iNextCSTimeOut%3912A1F60010.role preserve=no  protected: static m4uint32_t {1 -> 1VHAPN}
m4uint32_t ClM4ObjRegClient::m_iNextCSTimeOut = 0;
//## end ClM4ObjRegClient::m_iNextCSTimeOut%3912A1F60010.role



ClM4ObjRegClient::ClM4ObjRegClient ()
  //## begin ClM4ObjRegClient::ClM4ObjRegClient%-646752407.hasinit preserve=no
      : m_iCsTraceLevel(0), m_iVbTraceLevel(0)
  //## end ClM4ObjRegClient::ClM4ObjRegClient%-646752407.hasinit
  //## begin ClM4ObjRegClient::ClM4ObjRegClient%-646752407.initialization preserve=yes
  //## end ClM4ObjRegClient::ClM4ObjRegClient%-646752407.initialization
{
  //## begin ClM4ObjRegClient::ClM4ObjRegClient%-646752407.body preserve=yes
	Reset();
  //## end ClM4ObjRegClient::ClM4ObjRegClient%-646752407.body
}


ClM4ObjRegClient::~ClM4ObjRegClient ()
{
  //## begin ClM4ObjRegClient::~ClM4ObjRegClient%-1439801433.body preserve=yes
	Reset();
  //## end ClM4ObjRegClient::~ClM4ObjRegClient%-1439801433.body
}



//## Other Operations (implementation)
m4return_t ClM4ObjRegClient::Init (void )
{
  //## begin ClM4ObjRegClient::Init%1540314371.body preserve=yes
    if( IsInit()  ) {
        return( M4_SUCCESS ) ;
    }

	// Ponemos el Path de CLIENT
	m4return_t ret = SetPath(M4_NEW_REGEDIT_KEY_CLIENT);
	if (ret == M4_SUCCESS) {
		// Inicializar el registry de las bases
		ret = ClVMBaseEnv::Init( GetPath() );
		if (ret == M4_SUCCESS) {
			// Inicializar nuestro registry particular
			ret = ReadRegistry( GetPath() );
		}
	}
	
	if (ret != M4_SUCCESS) {
		m_bInit = M4_FALSE;
		DUMP_CHLOG_DEBUG( M4_CH_OBJREG_NO_READ_REGISTRY ) ;
	} else {
		m_bInit = M4_TRUE;
	}

	return ret;
  //## end ClM4ObjRegClient::Init%1540314371.body
}

m4return_t ClM4ObjRegClient::ReadRegistry (m4pchar_t ai_pcPath)
{
  //## begin ClM4ObjRegClient::ReadRegistry%-802157592.body preserve=yes
	if (!ai_pcPath)
		return M4_ERROR;

	m4return_t	iResult = M4_SUCCESS;
	m4char_t	acPath[ M4CH_MAX_TEMP_BUFFER + 1 ] ;
	m4char_t	acTempBuffer[M4CH_MAX_TEMP_BUFFER+1];

	// resets the execution time out

	//leemos path: CVM\CLIENTSERVER
	strcpy(acPath, ai_pcPath);
	strcat(acPath, M4_NEW_REGEDIT_KEY_CVM_CLIENTSERVER);

	//a por los valores
	memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_CS_DEFAULT_TIMEOUT, acTempBuffer ) == FALSE  ||
		!RegIsDigit(acTempBuffer))
		m_iDefaultCSTimeOut = M4_CS_DEFAULT_TIMEOUT;
	else
	{
		m_iDefaultCSTimeOut = atol(acTempBuffer);
		if (m_iDefaultCSTimeOut < 1 || m_iDefaultCSTimeOut > M4_CS_DEFAULT_TIMEOUT)
			m_iDefaultCSTimeOut = M4_CS_DEFAULT_TIMEOUT;
	}

	m_iDefaultCSTimeOut *= 1000;
	m_iNextCSTimeOut = m_iDefaultCSTimeOut;

	// resets the generic time out
	memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_CS_GENERIC_TIMEOUT, acTempBuffer ) == FALSE  ||
		!RegIsDigit(acTempBuffer))
		m_iGenCSTimeOut = M4_CS_DEFAULT_TIMEOUT;
	else
	{
		m_iGenCSTimeOut = atol(acTempBuffer);
		if (m_iGenCSTimeOut < 1 || m_iGenCSTimeOut > M4_CS_DEFAULT_TIMEOUT)
			m_iGenCSTimeOut = M4_CS_DEFAULT_TIMEOUT;
	}

	m_iGenCSTimeOut *= 1000;


	// Para las trazas CS =======================
    memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_CS_TRACE_ENABLED, acTempBuffer ) == FALSE )
    {
		sprintf( acTempBuffer, "0" ) ;
	}

	switch( *acTempBuffer ) 
	{
		case '0' :
		case '1' :
		case '2' :
		case '3' :
		case '4' :
		case '5' :

			m_iCsTraceLevel = (m4int8_t)atoi(acTempBuffer) ;

		break ;

		default:

			m_iCsTraceLevel = 0 ;
	}

	
	// Para las trazas VB =======================
    memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_VB_TRACE_ENABLED, acTempBuffer ) == FALSE )
    {
		m_iVbTraceLevel = 0;
	}
	else
	{
		// Trazas desactivadas, VBTraceEnabled = 0, trazas activadas, cualquier otro valor.
		m_iVbTraceLevel = (m4int32_t) atoi( acTempBuffer );
		if (m_iVbTraceLevel != 0)
		{
			m_iVbTraceLevel = 1;
		}
	}


	// Operaciones de cache
    memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_CS_TRACE_SHOW_CACHE_OPERATIONS, acTempBuffer ) == FALSE )
    {
		sprintf( acTempBuffer, "0" ) ;
	}

	switch( *acTempBuffer )
	{
		case '0' :
		case 'N' :
		case 'n' :
		case 'F' :
		case 'f' :

			m_bCsTraceCacheOperations = M4_FALSE ;

		break ;

		default:

			m_bCsTraceCacheOperations = M4_TRUE ;
	}


	// Leemos Mecanismos de Logon.
	memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_CS_LOGON_MECHANISM, acTempBuffer ) == FALSE ){
		m_iMechanism = 0 ;
    }
	else{
		m_iMechanism = (m4int32_t) atoi( acTempBuffer );
	}
	
	memset( m_acDefaultDllMechanism, '\0',M4CH_MAX_TEMP_DIRECTORY+1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_CS_DLL_MECHANISM, m_acDefaultDllMechanism ) == FALSE ){
		*m_acDefaultDllMechanism = 0 ;
    }

	memset (m_acTransportProtocol, '\0', M4CH_MAX_TRANSPORT_PROTOCOL+1);
	if( GetRegKeyData( M4_NEW_TRANSPORT_REGEDIT_KEY, M4_INIT_TRANSPORT_DEFAULT_PROTOCOL, m_acTransportProtocol ) == FALSE ){
		*m_acTransportProtocol =  0;		
	}

	// APICOMM parameters.
	memset(acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER+1);
	if (GetRegKeyData(M4_NEW_TRANSPORT_M4_REGEDIT_KEY, M4_INIT_TRANSPORT_M4_CONTROLPORT, acTempBuffer) == FALSE  ||
		!RegIsDigit(acTempBuffer))
    {
		strcpy(acTempBuffer, M4_INIT_TRANSPORT_M4_CONTROLPORT_DEFAULT_VALUE);
    }
    m_ControlPort = atol(acTempBuffer);

	memset(acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER+1);
	if (GetRegKeyData(M4_NEW_TRANSPORT_M4_REGEDIT_KEY, M4_INIT_TRANSPORT_M4_COMMWORKERS, acTempBuffer) == FALSE  ||
		!RegIsDigit(acTempBuffer))
    {
		strcpy(acTempBuffer, M4_INIT_TRANSPORT_M4_COMMWORKERS_DEFAULT_VALUE);
    }
    m_CommWorkers = atol(acTempBuffer);
    
    
    iResult = ReadCacheRegistry(ai_pcPath);

	return iResult;
  //## end ClM4ObjRegClient::ReadRegistry%-802157592.body
}

m4return_t ClM4ObjRegClient::ReadCacheRegistry (m4pchar_t ai_pcPath)
{
  //## begin ClM4ObjRegClient::ReadCacheRegistry%306286329.body preserve=yes
	if (!ai_pcPath)
		return M4_ERROR;

	// leemos el directorio de caché
	m4char_t    acPath[ M4CH_MAX_TEMP_BUFFER + 1 ] ;
	//path para CVM\CACHE
	strcpy(acPath, ai_pcPath);
	strcat(acPath, M4_NEW_REGEDIT_KEY_CACHE);

    memset( m_acCacheDirectory, '\0', M4CH_MAX_WORKING_DIRECTORY + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_CACHE_DIR, m_acCacheDirectory ) == FALSE )
    {
		strcpy(m_acCacheDirectory, m_acTempDirectory);
	}
	strcat(m_acCacheDirectory, M4_BACK_SLASH);	//Dejamos preparado el path: c:\temp\"
	
	// Montamos el directorio cache
	// Establecemos el nombre del directorio de Cache
	strcat(m_acCacheDirectory,"client");

	// Creamos el directorio de caches
	m4return_t iResult = M4RegMkDir(m_acCacheDirectory, M4_FALSE);

	/* Bug 0151033
	Si es unicode se hace en un subdirectorio para que no coincida con las cachés de nounicode
	*/
	if( M4IsUnicode() == 1 )
	{
		strcat( m_acCacheDirectory, M4_BACK_SLASH ) ;
		strcat( m_acCacheDirectory, "unicode" ) ;
		iResult = M4RegMkDir( m_acCacheDirectory, M4_FALSE ) ;
	}

	// Creamos el directorio emul\LDB o client\LDB o server\LDB (si tenemos que utilizarlo)
	if (m_iLDBDiskCache & M4CL_CACHE_READ_WRITE){
		m4char_t    BDLCacheDirectory  [ M4CH_MAX_WORKING_DIRECTORY + 1 ] ;
		strcpy(BDLCacheDirectory ,GetCacheDirectory() );
		strcat(BDLCacheDirectory , M4_BACK_SLASH);
		strcat(BDLCacheDirectory ,"LDB");
		iResult = M4RegMkDir(BDLCacheDirectory, M4_FALSE);
	}

	return M4_SUCCESS;
  //## end ClM4ObjRegClient::ReadCacheRegistry%306286329.body
}

void ClM4ObjRegClient::Reset ()
{
  //## begin ClM4ObjRegClient::Reset%1752870150.body preserve=yes
	ClVMBaseEnv::Reset();

	// timeout members
	//m_iDefaultCSTimeOut = 0;
	//m_iGenCSTimeOut = 0;
	//m_iNextCSTimeOut = 0;

	m_iCsTraceLevel = 0 ;
	m_iVbTraceLevel = 0 ;
	m_bCsTraceCacheOperations = M4_FALSE ;
	*m_acTransportProtocol = '\0';
  //## end ClM4ObjRegClient::Reset%1752870150.body
}


// Additional Declarations
  //## begin ClM4ObjRegClient%3912A1F503AE.declarations preserve=yes
  //## end ClM4ObjRegClient%3912A1F503AE.declarations

// Class ClM4ObjRegSM 


ClM4ObjRegSM::ClM4ObjRegSM ()
  //## begin ClM4ObjRegSM::ClM4ObjRegSM%96582462.hasinit preserve=no
  //## end ClM4ObjRegSM::ClM4ObjRegSM%96582462.hasinit
  //## begin ClM4ObjRegSM::ClM4ObjRegSM%96582462.initialization preserve=yes
  //## end ClM4ObjRegSM::ClM4ObjRegSM%96582462.initialization
{
  //## begin ClM4ObjRegSM::ClM4ObjRegSM%96582462.body preserve=yes
	Reset();
  //## end ClM4ObjRegSM::ClM4ObjRegSM%96582462.body
}


ClM4ObjRegSM::~ClM4ObjRegSM ()
{
  //## begin ClM4ObjRegSM::~ClM4ObjRegSM%-1492792007.body preserve=yes
	Reset();
  //## end ClM4ObjRegSM::~ClM4ObjRegSM%-1492792007.body
}



//## Other Operations (implementation)
m4return_t ClM4ObjRegSM::Init (void )
{
  //## begin ClM4ObjRegSM::Init%1551224479.body preserve=yes
    if( IsInit()  ) {
        return( M4_SUCCESS ) ;
    }

	// Ponemos el Path de CLIENT
	m4return_t ret = SetPath(M4_NEW_REGEDIT_KEY_CLIENT);
	if (ret == M4_SUCCESS) {
		// Inicializar el registry de las bases
		ret = ClM4ObjRegClient::Init();
		if (ret == M4_SUCCESS) {
			// Inicializar nuestro registry particular
			ret = ReadRegistry( GetPath() );
		}
	}

	if (ret != M4_SUCCESS) {
		m_bInit = M4_FALSE;
		DUMP_CHLOG_DEBUG( M4_CH_OBJREG_NO_READ_REGISTRY ) ;
	} else {
		m_bInit = M4_TRUE;
	}

	return ret;
  //## end ClM4ObjRegSM::Init%1551224479.body
}

m4return_t ClM4ObjRegSM::ReadRegistry (m4pchar_t ai_pcPath)
{
  //## begin ClM4ObjRegSM::ReadRegistry%1447089992.body preserve=yes
	if (!ai_pcPath)
		return M4_ERROR;

	// Leemos propiedad SMOLTP
	if ( ClM4ObjRegSM::ReadSMOLTP(m_iSmOltp ) != M4_SUCCESS ) {
		m_iSmOltp = 0 ;
		//return M4_ERROR;
	}

	return ReadCacheRegistry(ai_pcPath);
  //## end ClM4ObjRegSM::ReadRegistry%1447089992.body
}

m4return_t ClM4ObjRegSM::ReadCacheRegistry (m4pchar_t ai_pcPath)
{
  //## begin ClM4ObjRegSM::ReadCacheRegistry%594244098.body preserve=yes
	if (!ai_pcPath)
		return M4_ERROR;

	// leemos el directorio de caché
	m4char_t    acPath[ M4CH_MAX_TEMP_BUFFER + 1 ] ;
	//path para CVM\CACHE
	strcpy(acPath, ai_pcPath);
	strcat(acPath, M4_NEW_REGEDIT_KEY_CACHE);

    memset( m_acCacheDirectory, '\0', M4CH_MAX_WORKING_DIRECTORY + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_CACHE_DIR, m_acCacheDirectory ) == FALSE )
    {
		strcpy(m_acCacheDirectory, m_acTempDirectory);
	}
	strcat(m_acCacheDirectory, M4_BACK_SLASH);	//Dejamos preparado el path: c:\temp\"

	// Montamos el directorio cache
	// Establecemos el nombre del directorio de Cache
	if ( !GetSmOltp() )
	{
		strcat(m_acCacheDirectory,"client");
	}
	else
	{
		/* jcr bug 0061030
		Cuando estamos en SM, también se ha creado la carpeta "client"
		porque este objeto deriva del la clase cliente.
		La manera sencilla de arreglarlo es borrar ese directorio.
		Si el directorio ya existía rmdir falla y no se borra, que es lo que se quiere.
		*/
		m4char_t    acTemp[ M4CH_MAX_WORKING_DIRECTORY + 1 ] ;

		sprintf( acTemp, "%s%s%s%s", m_acCacheDirectory, "client", M4_BACK_SLASH, "LDB" ) ;
		rmdir( acTemp ) ;

		sprintf( acTemp, "%s%s%s%s", m_acCacheDirectory, "client", M4_BACK_SLASH, "unicode" ) ;
		rmdir( acTemp ) ;

		sprintf( acTemp, "%s%s", m_acCacheDirectory, "client" ) ;
		rmdir( acTemp ) ;

		strcat(m_acCacheDirectory,"emul");

		m4int32_t	iAux = 0;
		ClVMBaseEnv::ReadRegistry( acPath, M4_INIT_SM_CACHE_REFRESH, M4_SUCCESS, 0, 1, 0, iAux);
		m_iSMCacheRefresh = (m4uint32_t) iAux;
	}

	// Creamos el directorio de caches
	m4return_t iResult = M4RegMkDir(m_acCacheDirectory, M4_FALSE);

	/* Bug 0151033
	Si es unicode se hace en un subdirectorio para que no coincida con las cachés de nounicode
	*/
	if( M4IsUnicode() == 1 )
	{
		strcat( m_acCacheDirectory, M4_BACK_SLASH ) ;
		strcat( m_acCacheDirectory, "unicode" ) ;
		iResult = M4RegMkDir( m_acCacheDirectory, M4_FALSE ) ;
	}

	// Creamos el directorio emul\LDB o client\LDB o server\LDB (si tenemos que utilizarlo)
	if (m_iLDBDiskCache & M4CL_CACHE_READ_WRITE){
		m4char_t    BDLCacheDirectory  [ M4CH_MAX_WORKING_DIRECTORY + 1 ] ;
		strcpy(BDLCacheDirectory ,GetCacheDirectory() );
		strcat(BDLCacheDirectory , M4_BACK_SLASH);
		strcat(BDLCacheDirectory ,"LDB");
		iResult = M4RegMkDir(BDLCacheDirectory, M4_FALSE);
	}

	return M4_SUCCESS;
  //## end ClM4ObjRegSM::ReadCacheRegistry%594244098.body
}

void ClM4ObjRegSM::Reset ()
{
  //## begin ClM4ObjRegSM::Reset%-1566441361.body preserve=yes
	ClM4ObjRegClient::Reset();

	m_iSmOltp = 0;
  //## end ClM4ObjRegSM::Reset%-1566441361.body
}

m4return_t ClM4ObjRegSM::ReadSMOLTP (m4uint8_t& ao_iSmOltp)
{
  //## begin ClM4ObjRegSM::ReadSMOLTP%1072811341.body preserve=yes
    m4char_t    acPath[ M4CH_MAX_TEMP_BUFFER + 1 ] ;	// Rama de la que leer
	m4uint32_t	iSize = 0;
	m4int32_t	iAux = 0;

	// Leemos la rama de registry: acPath = CLIENT\CVM\CLIENTSERVER
	if (M4_ERROR == M4ComposeRegistryPath( M4_NEW_REGEDIT_KEY_CLIENT, M4_NEW_REGEDIT_KEY_CVM_CLIENTSERVER, acPath, M4CH_MAX_TEMP_BUFFER, iSize ))
	{
		return M4_ERROR;
	}

	//SMOLTP
	if (ClVMBaseEnv::ReadRegistry( acPath, M4_INIT_SM_OLTP, M4_ERROR, 0, 1, 1, iAux) == M4_ERROR) {
		return (M4_ERROR);
	}
	ao_iSmOltp = (m4uint8_t) iAux;

	return M4_SUCCESS;
  //## end ClM4ObjRegSM::ReadSMOLTP%1072811341.body
}

// Debug level environment =========================================================

// static reference
ClVMDebugEnv *ClVMDebugEnv::m_poThis = NULL;
ClMutex *ClVMDebugEnv::m_poMutex = new ClMutex(M4_TRUE);

// constructor
ClVMDebugEnv::ClVMDebugEnv() {
	m_iDebugLevel = 0;
	m_pcDebugDir = NULL;
}

// destructor
ClVMDebugEnv::~ClVMDebugEnv() {
	if (m_poMutex != NULL) {
		delete m_poMutex;
		m_poMutex = 0;
	}

	if (m_pcDebugDir != NULL) {
		delete [] m_pcDebugDir;
		m_pcDebugDir = NULL;
	}
}

ClVMDebugEnv* ClVMDebugEnv::getInstance() {

	if (m_poThis != NULL) {
		return m_poThis;
	}
	
	ClMutBlock oMut(m_poMutex);

	if (m_poThis != NULL) {
		return m_poThis;
	}

	m_poThis = new ClVMDebugEnv();

	return m_poThis;
}

void ClVMDebugEnv::initDebugLevel(m4pcchar_t ai_pcPath) {

    m4char_t acTempBuffer[ M4CH_MAX_TEMP_BUFFER + 1 ] ;
    m4char_t acPath[ M4CH_MAX_TEMP_BUFFER + 1 ] ;
   	
	// path para cvm
	strcpy(acPath, ai_pcPath);
	strcat(acPath, M4_NEW_REGEDIT_KEY_CVM_LOG);

	/* Bug 0108688
	Hay que chequear el rango
	Además no hace falta leer primero el debug enable
	*/
	m4int32_t	iAux = 0 ;
	ClVMBaseEnv::ReadRegistry( acPath, M4_INIT_SYSTEM_DEBUG_LEVEL, M4_SUCCESS, 0, 3, 0, iAux ) ;
	m_iDebugLevel = (m4uint32_t) iAux ;

	ClVMBaseEnv::ReadRegistry( acPath, M4_INIT_MAX_DEBUG_PERIOD, M4_SUCCESS, 1, 365 * 24, 24, iAux ) ;
	M4LogFile::SetMaxPeriod( m4date_t(iAux) / 24 );

   	// Leemos el DebugDir.
	memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	
	if( GetRegKeyData( acPath, M4_INIT_DEBUG_DIR, acTempBuffer ) == TRUE )
    {
		m4int16_t iSize = strlen(acTempBuffer);
		m_pcDebugDir = new m4char_t [iSize+1];
		strcpy(m_pcDebugDir,acTempBuffer);
    }
}

m4int_t ClVMDebugEnv::getDebugLevel() const {
	return m_iDebugLevel;
}

m4return_t ClVMDebugEnv::setDebugLevel(m4int_t ai_iLevel) {

	/* Bug 0108688
	Hay que chequear el rango
	*/
	if( ai_iLevel < 0 || ai_iLevel > 3 )
	{
		DUMP_CHLOG_WARNINGF( M4_CH_OBJREG_WRONG_PROPERTY_VALUE, M4_INIT_SYSTEM_DEBUG_LEVEL << 0 << ai_iLevel ) ;
		m_iDebugLevel = 0;
		return( M4_ERROR ) ;
	}

	m_iDebugLevel = ai_iLevel;
	return( M4_SUCCESS ) ;
}

m4int_t ClVMDebugEnv::getDebugMaxPeriod() const {
	return M4LogFile::GetMaxPeriod() * 24;
}

m4return_t ClVMDebugEnv::setDebugMaxPeriod(m4int_t ai_iMaxPeriod) {

	if( ai_iMaxPeriod < 1 || ai_iMaxPeriod > 365 * 24 )
	{
		DUMP_CHLOG_WARNINGF( M4_CH_OBJREG_WRONG_PROPERTY_VALUE, M4_INIT_MAX_DEBUG_PERIOD << 24 << ai_iMaxPeriod ) ;
		M4LogFile::SetMaxPeriod( 1 );
		return( M4_ERROR ) ;
	}

	M4LogFile::SetMaxPeriod( m4date_t(ai_iMaxPeriod) / 24 );
	return( M4_SUCCESS ) ;
}

void ClVMDebugEnv::setPath(m4pcchar_t ai_pcPath) {

	if (m_pcDebugDir != NULL) {
		return;
	}

	initDebugLevel(ai_pcPath);
}

m4pcchar_t	ClVMDebugEnv::getDebugDir() const {
	if (m_pcDebugDir == NULL) {
		return "";
	}

	return m_pcDebugDir;
}

	
// Additional Declarations
  //## begin ClM4ObjRegSM%3912A1F6001B.declarations preserve=yes
  //## end ClM4ObjRegSM%3912A1F6001B.declarations

//## begin module%3912A1F600AA.epilog preserve=yes





/*
// Ejemplo de uso
int main() {
	m4return_t ret;
	
	// Creamos un Registry de SingleMachine
	ClM4ObjRegSM *pReg = new ClM4ObjRegSM();
	pReg->Init();

	//ClVMBaseEnv *pRegServer = new ClM4ObjRegServer();
	ClVMBaseEnv *pRegClient = new ClM4ObjRegClient();
		
	// Inicializamos el registry
	ret = pRegClient->Init();

	if (ret == M4_SUCCESS) {
		cout << "Registry initilized ok" << endl;
	} else {
		cout << "Registry error initializing" << endl;
	}

	return 0;
}

*/

//## end module%3912A1F600AA.epilog

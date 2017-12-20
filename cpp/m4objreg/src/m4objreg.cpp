//## begin module%3912A1F60078.cm preserve=no
//## end module%3912A1F60078.cm

//## begin module%3912A1F60078.cp preserve=no
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
//## end module%3912A1F60078.cp

//## Module: m4objreg%3912A1F60078; Package body
//## Subsystem: m4objreg::src%3912A3550163
//## Source file: b:\m4objreg\src\m4objreg.cpp

//## begin module%3912A1F60078.additionalIncludes preserve=no
//## end module%3912A1F60078.additionalIncludes

//## begin module%3912A1F60078.includes preserve=yes
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
#include "m4regimpl.hpp"
#include "regedit.hpp"		// M4Ini
#include "m4win2ux.hpp"		// M4win2ux + M4Ipcs
#include "m4objglb.hpp"		// M4ObjGlb
#include "m4date.hpp"		// M4Date
#include "m4regres.hpp"		// Errores
#include "m4objlog.hpp"		// M4ObjLog
#include "m4buildversion.hpp"
#include "m4xmlreg.hpp"
#include "m4unicode.hpp"

//## end module%3912A1F60078.includes

// m4types
#include <m4types.hpp>
//## begin module%3912A1F60078.declarations preserve=no
//## end module%3912A1F60078.declarations

// Variable global definida en m4objglb donde 
// hay que meter la corrección de fecha leida del registry
extern m4date_t M4_DECL_M4OBJGLB g_dTimeCorrection;

//## begin module%3912A1F60078.additionalDeclarations preserve=yes


/*
	Falta por clarificar:
	- Inicialización de M4Log en SM y Client (no en Server)
	- Inicialización de LDB en SM (no en Client, y no en Server)
	- Inicialización de la M4LogSys: LogsysSetWorkingDir
	- Inicialización de la M4FileManager
	- LogonAdaptor
	- Lenguaje de la sesion
	- Licencia de usuario
	- Chequeo de la version
	- Encriptación de Password
*/

// Errores utilizados:
// M4_CH_MCR_NO_READ_REGISTRY
// M4_CH_MCR_BAD_DLL_VERSION
// M4LDB_CERROR_PROPERTY_NOT_FOUND	-> M4_CH_OBJREG_PROPERTY_NOT_FOUND


// Define que ya no se usa.
// Activaba el modo con password no encriptada
//#define M4_NO_ENCRYPTED_DB_PWD


/* Bug 0114783
Si no se pueden crear los directorios de cache se da un error
*/
m4return_t	M4RegMkDir( m4pcchar_t ai_pccDirectory, m4bool_t ai_bDumpLog )
{

	int			iResult = 0 ;
	m4pchar_t	pcError = NULL ;


	iResult = M4MakeDirectory( ai_pccDirectory, &pcError ) ;

	if( iResult == -1 && ai_bDumpLog == M4_TRUE )
	{
		DUMP_CHLOG_ERRORF( M4_CH_OBJREG_BAD_DIRECTORY, ai_pccDirectory << pcError ) ;
	}

	if( pcError != NULL )
	{
		delete( pcError ) ;
	}

	if( iResult == -1 )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}





// Funcion: Convertir el Mode de Cache de String a Integer
// Entrada: String
// Salida:	Byte
m4uint8_t ConvertCacheMode(m4pcchar_t ai_pCachemode)
{

	if (ai_pCachemode){
		if( strcmpi( ai_pCachemode, "READ" ) == 0 ){
			return M4CL_CACHE_READ;
		}
		if( strcmpi( ai_pCachemode, "WRITE" ) == 0 ){
			return M4CL_CACHE_WRITE;
		}
		if( strcmpi( ai_pCachemode, "READ_WRITE" ) == 0 ){
			return M4CL_CACHE_READ_WRITE;
		}
		if( strcmpi( ai_pCachemode, "READWRITE" ) == 0 ){
			return M4CL_CACHE_READ_WRITE;
		}
		if( strcmpi( ai_pCachemode, "MEM_ONLY" ) == 0 ){
			return M4CL_CACHE_MEM_ONLY;
		}
		if( strcmpi( ai_pCachemode, "MEMONLY" ) == 0 ){
			return M4CL_CACHE_MEM_ONLY;
		}
	}
	return M4CL_CACHE_NONE ;
}


// ============= Metodos static para Propiedades Especiales de Registry ======================

// Lectura inicial del WorkDir para el Inicializador
// Entrada:	ai_pcPath = Rama Client o Server del Registry
//			ai_iBufLen = Longitud del buffer
// Salida:	aio_acWorkingDirectory = Buffer con el registry leido
//			return = M4_SUCCESS o M4_ERROR
//## end module%3912A1F60078.additionalDeclarations


// Class ClVMBaseEnv 











































ClVMBaseEnv::ClVMBaseEnv ()
  //## begin ClVMBaseEnv::ClVMBaseEnv%572991009.hasinit preserve=no
      : m_iMechanism(-1), m_iShowData(1)
  //## end ClVMBaseEnv::ClVMBaseEnv%572991009.hasinit
  //## begin ClVMBaseEnv::ClVMBaseEnv%572991009.initialization preserve=yes
  //## end ClVMBaseEnv::ClVMBaseEnv%572991009.initialization
{
  //## begin ClVMBaseEnv::ClVMBaseEnv%572991009.body preserve=yes
    m_bInit = M4_FALSE ;
	Reset();

	m_pcJavaHome = NULL ;
	m_pcJavaClassPath = NULL ;
	m_pcJavaArguments = NULL ;
	m_pcJavaTrace = NULL ;
	m_bJavaAllowInProcess = M4_FALSE ;
	m_iJavaTimeout = 0 ;

  //## end ClVMBaseEnv::ClVMBaseEnv%572991009.body
}


ClVMBaseEnv::~ClVMBaseEnv (void )
{
  //## begin ClVMBaseEnv::~ClVMBaseEnv%-298358039.body preserve=yes
	Reset();

	if( m_pcJavaHome != NULL )
	{
		delete( m_pcJavaHome ) ;
		m_pcJavaHome = NULL ;
	}

	if( m_pcJavaClassPath != NULL )
	{
		delete( m_pcJavaClassPath ) ;
		m_pcJavaClassPath = NULL ;
	}

	if( m_pcJavaArguments != NULL )
	{
		delete( m_pcJavaArguments ) ;
		m_pcJavaArguments = NULL ;
	}

	if( m_pcJavaTrace != NULL )
	{
		delete( m_pcJavaTrace ) ;
		m_pcJavaTrace = NULL ;
	}
  //## end ClVMBaseEnv::~ClVMBaseEnv%-298358039.body
}



//## Other Operations (implementation)
m4return_t ClVMBaseEnv::ReadWorkingDirectory (m4pchar_t ai_pcPath, m4pchar_t aio_acWorkingDirectory, m4int32_t ai_iBufLen, m4bool_t ai_bFullPath)
{
  //## begin ClVMBaseEnv::ReadWorkingDirectory%-472850620.body preserve=yes
	if (!ai_pcPath || ai_iBufLen< (M4CH_MAX_WORKING_DIRECTORY + 1) )
		return M4_ERROR;

    m4char_t    acPath[ M4CH_MAX_TEMP_BUFFER + 1 ] ;

	if (M4_FALSE == ai_bFullPath) { //Solo viene cliente o Server
		sprintf(acPath, "%s%s%s", M4_NEW_REGEDIT_KEY, ai_pcPath, M4_NEW_REGEDIT_KEY_CVM);
	}
	else { //viene ya el path completo: hasta la build...
		sprintf(acPath, "%s%s", ai_pcPath, M4_NEW_REGEDIT_KEY_CVM);
	}

	// Obtenemos el directorio de trabajo: CVM\WorkDir
    memset( aio_acWorkingDirectory, '\0', ai_iBufLen ) ;
	if( GetRegKeyData( acPath, M4_INIT_WORK_DIR, aio_acWorkingDirectory ) == FALSE ){
		// TODO: Volcar error adecuado
//#ifdef	M4_VM_CLIENT
//        MessageBox( NULL, "Could not read working directory from registry.", "Init Error", MB_OK ) ;
//#else // for server
		cerr << "Init Error: Could not read working directory from registry." << endl ;
//#endif
		return M4_ERROR;
	}

	return M4_SUCCESS;
  //## end ClVMBaseEnv::ReadWorkingDirectory%-472850620.body
}

m4return_t ClVMBaseEnv::Init (m4pchar_t ai_pcPath)
{
  //## begin ClVMBaseEnv::Init%1511868948.body preserve=yes

    m4return_t  iResult ;

    if( IsInit() )
    {
        return( M4_SUCCESS ) ;
    }

    iResult = ReadRegistry( ai_pcPath ) ;

	if( iResult != M4_SUCCESS )
    {
        return( M4_ERROR ) ;
	}

   	m_bInit = (iResult == M4_SUCCESS) ? M4_TRUE : M4_FALSE;

    return( M4_SUCCESS ) ;
  //## end ClVMBaseEnv::Init%1511868948.body
}

m4return_t ClVMBaseEnv::ReadRegistry (m4pchar_t ai_pcPath)
{
  //## begin ClVMBaseEnv::ReadRegistry%215321743.body preserve=yes
	if (!ai_pcPath)
		return M4_ERROR;

    m4return_t  iResult = 0;
	int			iLength = 0 ;
    m4uint16_t  iVersion ;
	m4uint16_t	iRealVersion ;
	m4int32_t	iAux = 0;
    m4date_t    dGMDate ;
    m4date_t    dLocalDate ;
    m4date_t    dTimeCorrection ;
    m4char_t    acTempBuffer[ M4CH_MAX_TEMP_BUFFER + 1 ] ;
    m4char_t    acPath[ M4CH_MAX_TEMP_BUFFER + 1 ] ;
	m4pcchar_t	pccAllowInProcess = NULL ;
	m4pcchar_t	pccJavaTimeout = NULL ;


	// Leemos el WorkDir
	iResult = ClVMBaseEnv::ReadWorkingDirectory( ai_pcPath, m_acWorkingDirectory, M4CH_MAX_WORKING_DIRECTORY + 1);
	if (iResult != M4_SUCCESS) {
		return M4_ERROR;
	}

	//path para CVM
	strcpy(acPath, ai_pcPath);
	strcat(acPath, M4_NEW_REGEDIT_KEY_CVM);

	// Obtenemos la Version: CVM\Version
	memset( m_acVersion, '\0', M4CH_MAX_VERSION + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_VERSION, m_acVersion ) == FALSE )
    {
        DUMP_CHLOG_ERRORF( M4_CH_OBJREG_PROPERTY_NOT_FOUND, M4_INIT_VERSION << acPath ) ;
        return( M4_ERROR ) ;
	}

    iVersion = ConvertM2ToNumericVersion( m_acVersion ) ;
	iRealVersion = GetM2VersionNumber() ;

	if( iVersion != iRealVersion )
    {
        DUMP_CHLOG_ERRORF( M4_CH_OBJREG_BAD_DLL_VERSION, m_acVersion << GetM2VersionString() ) ;
        return( M4_ERROR ) ;
	}

	// Obtenemos el directorio de temporal: CVM\TempDir
    memset( m_acTempDirectory, '\0', M4CH_MAX_TEMP_DIRECTORY + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_TEMP_DIR, m_acTempDirectory ) == FALSE )
    {
        *m_acTempDirectory = '\0' ;
	}

	//path de M4_NEW_REGEDIT_KEY_CVM_LOG: CVM\LOG
	strcpy(acPath, ai_pcPath);
	strcat(acPath, M4_NEW_REGEDIT_KEY_CVM_LOG);

	// Obtenemos el directorio de debug: CVM\LOG\DebugDir
    memset( m_acDebugDirectory, '\0', M4CH_MAX_DEBUG_DIRECTORY + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_DEBUG_DIR, m_acDebugDirectory ) == FALSE )
    {
        DUMP_CHLOG_WARNINGF(M4_CH_OBJREG_PROPERTY_NOT_FOUND, M4_INIT_DEBUG_DIR);
        *m_acDebugDirectory = '\0' ;
	}

   	// Leemos el SystemDebugEnable: CVM\LOG\SystemDebugEnable
	if (ReadRegistry( acPath, M4_INIT_SYSTEM_DEBUG_ENABLE, M4_ERROR, 0, 3, 0, iAux) == M4_ERROR) {
		return (M4_ERROR);
	}
	m_iSystemDebugEnable = (m4uint8_t) iAux;

   	// Leemos el SystemDebugDetailLevel: CVM\LOG\SystemDebugDetailLevel
	ReadRegistry( acPath, M4_INIT_SYSTEM_DEBUG_DETAIL_LEVEL, M4_SUCCESS, 0, 0x00000004, 0, iAux);
	m_iSystemDebugDetailLevel = (m4uint32_t) iAux;

   	// Leemos el ShowData.
	ReadRegistry( acPath, M4_INIT_SHOW_DATA, M4_WARNING, 0, 1, 1, iAux);
	m_iShowData = (m4uint8_t) iAux;


	//path para M4_NEW_REGEDIT_KEY_CVM_TIME: CVM\TIME
	strcpy(acPath, ai_pcPath);
	strcat(acPath, M4_NEW_REGEDIT_KEY_CVM_TIME);

	// Obtenemos la unidad de tiempo: CVM\TIME\TimeUnit
    memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_TIME_UNIT, acTempBuffer ) == FALSE )
    {
        strcpy( acTempBuffer, "DAY" ) ;
	}
	if( strcmpi( acTempBuffer, "DAY" ) == 0 )
	{
		m_iTimeUnit = M4_UNIT_ONE_DAY ;
		m_dTimeUnit = M4_ONE_DAY ;
	}
	else if( strcmpi( acTempBuffer, "1" ) == 0 )
	{
		m_iTimeUnit = M4_UNIT_ONE_DAY ;
		m_dTimeUnit = M4_ONE_DAY ;
	}
	else if( strcmpi( acTempBuffer, "HOUR" ) == 0 )
	{
		m_iTimeUnit = M4_UNIT_ONE_HOUR ;
		m_dTimeUnit = M4_ONE_HOUR ;
	}
	else if( strcmpi( acTempBuffer, "2" ) == 0 )
	{
		m_iTimeUnit = M4_UNIT_ONE_HOUR ;
		m_dTimeUnit = M4_ONE_HOUR ;
	}
	else if( strcmpi( acTempBuffer, "MINUTE" ) == 0 )
	{
		m_iTimeUnit = M4_UNIT_ONE_MINUTE ;
		m_dTimeUnit = M4_ONE_MINUTE ;
	}
	else if( strcmpi( acTempBuffer, "3" ) == 0 )
	{
		m_iTimeUnit = M4_UNIT_ONE_MINUTE ;
		m_dTimeUnit = M4_ONE_MINUTE ;
	}
	else if( strcmpi( acTempBuffer, "SECOND" ) == 0 )
	{
		m_iTimeUnit = M4_UNIT_ONE_SECOND ;
		m_dTimeUnit = M4_ONE_SECOND ;
	}
	else if( strcmpi( acTempBuffer, "4" ) == 0 )
	{
		m_iTimeUnit = M4_UNIT_ONE_SECOND ;
		m_dTimeUnit = M4_ONE_SECOND ;
	}
	else
	{
		m_iTimeUnit = M4_UNIT_ONE_DAY ;
		m_dTimeUnit = M4_ONE_DAY ;
	}

	// Obtenemos el valor de corrección de la fecha local: CVM\TIME\LocalTimeCorrection
    memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_LOCAL_TIME_CORRECTION, acTempBuffer ) == FALSE )
    {
        strcpy( acTempBuffer, "AUTO" ) ;
    }
	if( strcmpi( acTempBuffer, "AUTO" ) == 0 )
    {
		ClActualDate( dGMDate, M4CL_TIME_GM_TIME ) ;
		ClActualDate( dLocalDate, M4CL_TIME_LOCAL_TIME ) ;
		m_dTimeCorrection = ClDateRoundToHours( dLocalDate - dGMDate ) ;
	}
	else
    {
		dTimeCorrection = atof( acTempBuffer ) / M4_HOURS_DAY ;
		m_dTimeCorrection = ClDateRoundToHours( dTimeCorrection ) ;
	}

	// Se asigna el valor de correción de la fecha leido del registry
	// a la variable global que se utiliza
	g_dTimeCorrection = m_dTimeCorrection;

	// User Key ( for encryption ).
    memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	// Obtenemos la licencia del usuario.
	if( GetRegKeyData( M4_NEW_MACHINE_REGEDIT_KEY, M4_INIT_MACHINE_USER_LICENCE, acTempBuffer ) == FALSE ){
		*acTempBuffer = 0;
	}
	// La guardamos encriptada ( no se va a utilizar nunca sin encriptar ).
	if( M4ConvertOneWayMD5( acTempBuffer, m_acUserLicence, 2 * M4_MAX_LEN_MD5 ) != M4_SUCCESS )
	{
		// Error. No se puede encriptar la Password.
		*m_acUserLicence = 0;
		return M4_ERROR;
	}

	// Leemos el Registry de la Cache
	iResult = ReadCacheRegistry( ai_pcPath );
	if (iResult != M4_SUCCESS) {
		return M4_ERROR;
	}


	// =========== LDB ============

	//CACHE del Diccionario
	// Defecto NONE o MEM_ONLY ( es lo mismo ).

	// Leemos la rama de registry: acPath = CVM\CACHE
	strcpy(acPath, ai_pcPath);
	strcat(acPath, M4_NEW_REGEDIT_KEY_CACHE);

    memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_LDB_CACHE_MODE, acTempBuffer ) == FALSE ){
		//Valor por defecto.
        m_iLDBDiskCache = M4CL_CACHE_NONE;
    }
	else{
		m_iLDBDiskCache = ConvertCacheMode(acTempBuffer);
	}

	// Leemos la rama de registry: acPath = CVM\LDB
	strcpy(acPath, ai_pcPath);
	strcat(acPath, M4_NEW_REGEDIT_KEY_CVM_LDB);

   	// Leemos si se deben utilizar campos unicode o no.
	ReadRegistry(acPath, M4_INIT_USE_FIELD_UNICODE_TYPES, M4_SUCCESS, 0, 1, 0, iAux);
	m_iUseFieldUnicodeTypes = (m4int8_t) iAux;


	memset( m_acConnectionString, '\0', M4CH_MAX_CONNECTION_STRING + 1 ) ;

#ifdef M4_NO_ENCRYPTED_DB_PWD
	// Obtenemos la conexión específica del sistema.
    if ( GetRegKeyData( acPath, M4_INIT_CONNECTION_STRING, m_acConnectionString ) == FALSE )
    {
        DUMP_CHLOG_ERRORF(M4_CH_OBJREG_PROPERTY_NOT_FOUND, M4_INIT_CONNECTION_STRING);
		return( M4_ERROR ) ;
	}
#else
	// Leemos la cadena de conexión incompleta del registro
    if ( GetRegKeyData( acPath, M4_INIT_CONNECTION_STRING, m_acTempConnString ) == FALSE )
    {
        DUMP_CHLOG_ERRORF( M4_CH_OBJREG_PROPERTY_NOT_FOUND, M4_INIT_CONNECTION_STRING << acPath ) ;
		return( M4_ERROR ) ;
	}

	// Nos guardamos la encriptada.
	strcpy (m_acConnectionString, m_acTempConnString);

	// Leemos la password encriptada del registro
	memset( m_acRegDBPwd , '\0', 2 * M4CH_MAX_DB_PWD + 1 ) ;
	if ( GetRegKeyData( acPath, M4_INIT_DB_PWD, m_acRegDBPwd ) == FALSE )
    {
        DUMP_CHLOG_ERRORF( M4_CH_OBJREG_PROPERTY_NOT_FOUND, M4_INIT_DB_PWD << acPath ) ;
		return( M4_ERROR ) ;
	}

#endif	// M4_NO_ENCRYPTED_DB_PWD

	if (ReadRegistry(acPath, M4_INIT_DDBB, M4_ERROR, 0, 5, 3, iAux) == M4_ERROR) {
		return (M4_ERROR);
	}
	m_DataBase = (m4char_t)iAux;


   	// Leemos el máximo numero de conexiones.
	if (ReadRegistry(acPath, M4_INIT_MAX_NUM_CONN, M4_ERROR, 2, 1000, 60, iAux) == M4_ERROR) {
		return (M4_ERROR);
	}
	m_numConn = (m4int16_t) iAux;


	// Leemos el máximo numero de cursores.
	if (ReadRegistry(acPath, M4_INIT_MAX_NUM_CURSOR, M4_ERROR, 1, 1000, 240, iAux) == M4_ERROR) {
		return (M4_ERROR);
	}
	m_numCursor = (m4int16_t) iAux;

   	
	// Leemos el máximo numero de sentencias.
	if (ReadRegistry(acPath, M4_INIT_MAX_NUM_STMT_CACHE, M4_ERROR, 1, 50000, 1024, iAux) == M4_ERROR) {
		return (M4_ERROR);
	}
	m_numStmtCache = (m4int16_t) iAux;


	// Leemos el máximo numero de objetos a cargar en el diccionario.
	if (ReadRegistry(acPath, M4_INIT_MAX_NUM_OBJECT, M4_ERROR, 1, 50000, 1024, iAux) == M4_ERROR) {
		return (M4_ERROR);
	}
	m_numObject = (m4int16_t) iAux;


	// Leemos el máximo numero de objetos a cargar en el canal de traducciones.
	ReadRegistry(acPath, M4_INIT_MAX_NUM_OBJECT_TRANS, M4_WARNING, 1, 10000, 100, iAux);
	m_numObjectTrans = (m4int16_t) iAux;

   	
	// Leemos el IsolationLevel.
	ReadRegistry(acPath, M4_INIT_ISOLATION_LEVEL, M4_WARNING, 0, 5, 0, iAux);
	m_eILevel = (m4int8_t) iAux;


	// Leemos el BufferSize en Mb.
	ReadRegistry(acPath, M4_INIT_BUFFER_SIZE, M4_WARNING, 1, 1000, 200, iAux);
	m_iBufferSize = iAux * 1024;


	// Leemos el ConExec.
	memset( m_acConExec, '\0', M4CH_MAX_CON_EXEC + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_CON_EXEC, m_acConExec ) == FALSE )
    {
        DUMP_CHLOG_WARNINGF(M4_CH_OBJREG_PROPERTY_NOT_FOUND, M4_INIT_CON_EXEC);
		*m_acConExec = '\0' ;
    }


	// Leemos la rama de registry: acPath = CVM\LOG
	strcpy(acPath, ai_pcPath);
	strcat(acPath, M4_NEW_REGEDIT_KEY_CVM_LOG);


	// Leemos el MultipleFilesTrace.
	ReadRegistry(acPath, M4_INIT_MULTIPLE_FILES_TRACE, M4_WARNING, 0, 1, 0, iAux);
	m_bMultFiles = iAux ? M4_TRUE : M4_FALSE;


	// Leemos el MaxLDBInspSize.
	ReadRegistry(acPath, M4_INIT_MAX_LDBINSP_SIZE, M4_WARNING, 1, 1000, 10, iAux);
	m_iMaxLDBInspSize = iAux;


	// Leemos la rama de registry: acPath = TOOLS\XML
	strcpy(acPath, ai_pcPath);
	strcat(acPath, M4_NEW_REGEDIT_KEY_TOOLS_XML);

	// Leemos el DateSeparator.
	memset( m_acDateSeparator, '\0', M4CH_MAX_DATE_SEPARATOR + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_XML_DATE_SEPARATOR, m_acDateSeparator ) == FALSE )
    {
		*m_acDateSeparator = '/' ;
    }


	// Leemos la rama de registry: acPath = JAVA
	strcpy(acPath, ai_pcPath);
	strcat(acPath, M4_NEW_REGEDIT_KEY_JAVA);

	iResult = M4XmlRegistry::Instance()->GetValue( acPath, M4_INIT_JAVA_HOME, m_pcJavaHome ) ;

	if( iResult != M4_SUCCESS || m_pcJavaHome == NULL )
	{
		iLength = -1 ;
		m_pcJavaHome = M4ANSIToCpp( getenv( "M4JAVAHOME" ), iLength ) ;
	}

	iResult = M4XmlRegistry::Instance()->GetValue( acPath, M4_INIT_JAVA_CLASSPATH, m_pcJavaClassPath ) ;

	if( iResult != M4_SUCCESS || m_pcJavaClassPath == NULL )
	{
		iLength = -1 ;
		m_pcJavaClassPath = M4ANSIToCpp( getenv( "M4CLASSPATH" ), iLength ) ;
	}

	iResult = M4XmlRegistry::Instance()->GetValue( acPath, M4_INIT_JAVA_ARGUEMTS, m_pcJavaArguments ) ;

	if( iResult != M4_SUCCESS || m_pcJavaArguments == NULL )
	{
		iLength = -1 ;
		m_pcJavaArguments = M4ANSIToCpp( getenv( "M4JAVAARGS" ), iLength ) ;
	}

	iResult = M4XmlRegistry::Instance()->GetValue( acPath, M4_INIT_JAVA_TRACE, m_pcJavaTrace ) ;

	if( iResult != M4_SUCCESS || m_pcJavaTrace == NULL )
	{
		iLength = -1 ;
		m_pcJavaTrace = M4ANSIToCpp( getenv( "M4JAVATRACE" ), iLength ) ;
	}

	if( GetRegKeyData( acPath, M4_INIT_JAVA_ALLOW_IN_PROCESS, acTempBuffer ) == FALSE )
    {
		pccAllowInProcess = getenv( "M4JAVAALLOWINPROCESS" ) ;
    }
	else
	{
		pccAllowInProcess = acTempBuffer ;
	}

	if( pccAllowInProcess != NULL && strcmp( pccAllowInProcess, "1" ) == 0 )
	{
		m_bJavaAllowInProcess = M4_TRUE ;
	}
	else
	{
		m_bJavaAllowInProcess = M4_FALSE ;
	}

	if( GetRegKeyData( acPath, M4_INIT_JAVA_TIMEOUT, acTempBuffer ) == FALSE )
    {
		pccJavaTimeout = getenv( "M4JAVATIMEOUT" ) ;
    }
	else
	{
		pccJavaTimeout = acTempBuffer ;
	}

	if( pccJavaTimeout != NULL )
	{
		m_iJavaTimeout = atoi( pccJavaTimeout ) ;
	}

	return( M4_SUCCESS ) ;
  //## end ClVMBaseEnv::ReadRegistry%215321743.body
}

m4return_t ClVMBaseEnv::ReadCacheRegistry (m4pchar_t ai_pcPath)
{
  //## begin ClVMBaseEnv::ReadCacheRegistry%1987159710.body preserve=yes
	if (!ai_pcPath)
		return M4_ERROR;

	m4return_t  iResult = 0;
    m4char_t    acTempBuffer[ M4CH_MAX_TEMP_BUFFER + 1 ] ;
    m4char_t    acPath[ M4CH_MAX_TEMP_BUFFER + 1 ] ;
	m4double_t	dAux;

	//path para CVM\CACHE
	strcpy(acPath, ai_pcPath);
	strcat(acPath, M4_NEW_REGEDIT_KEY_CACHE);


//CACHE de DATOS
//leemos e DiskCache
    memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_DATA_CACHE_MODE, acTempBuffer ) == FALSE ){
		// Valor por defecto.
        m_iDataDiskCache = M4CL_CACHE_READ_WRITE;
    }
	else{
		m_iDataDiskCache = ConvertCacheMode(acTempBuffer);
	}
	
//Leemos CacheMaxMemory
	/*
	Limite superior => 4 Gb
	Si 0 => No hay límite de memoria
	Valores negativos inválidos.
	Defecto 0
	*/
    memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_DATA_CACHE_MAX_MEMORY, acTempBuffer ) == FALSE )
    {
		*acTempBuffer = '\0' ;
    }
	if( *acTempBuffer == '\0' )
    {
		m_iDataCacheMaxMemory = 30000000 ;
	}
	else
    {
		dAux = atof( acTempBuffer );
		if (dAux < 0) dAux = 0;
		else if (dAux > 0xFFFFFFFF) dAux = 0xFFFFFFFF;
		m_iDataCacheMaxMemory = (m4uint32_t) dAux;
	}

	/*if ( !(m_iDataDiskCache & M4CL_CACHE_WRITE) )
		m_iDataCacheMaxMemory = 0; //Para que no haga swapping*/


//Leemos CacheMaxNumObjects
	/*
	Limite superior => 4 Gb
	Si 0 => No hay límite de memoria
	Valores negativos inválidos.
	Defecto 0
	*/
    memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_DATA_CACHE_MAX_NUM_OBJECTS, acTempBuffer ) == FALSE )
    {
		*acTempBuffer = '\0' ;
    }
	if( *acTempBuffer == '\0' )
    {
		m_iDataCacheMaxNumObjects = 100 ;
	}
	else
    {
		dAux = atof( acTempBuffer );
		if (dAux < 0) dAux = 0;
		else if (dAux > 0xFFFFFFFF) dAux = 0xFFFFFFFF;
		m_iDataCacheMaxNumObjects = (m4uint32_t) dAux;
	}
	
	
//Leemos CacheRefreshRatio
	/*
	Limite superior => 4 Gb
	Si 0 => Hace el refresh cada vez que se hace un Put en la caché
	Valores negativos inválidos.
	Defecto 0
	*/
    memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_DATA_CACHE_REFRESH_RATIO, acTempBuffer ) == FALSE )
    {
		*acTempBuffer = '\0' ;
    }
	if( *acTempBuffer == '\0' )
    {
		m_iDataCacheRefreshRatio = 5 ;
	}
	else
    {
		dAux = atof( acTempBuffer );
		if (dAux < 0) dAux = 0;
		else if (dAux > 0xFFFFFFFF) dAux = 0xFFFFFFFF;
		m_iDataCacheRefreshRatio = (m4uint32_t) dAux;
	}


//Leemos CacheExpiryRatio
	/*
	Limite superior => No tiene.
	Si 0 => 0 es inválido, se le asigna el valor 1
	Valores negativos inválidos.
	Defecto 1
	*/
    memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_DATA_CACHE_EXPIRY_RATIO, acTempBuffer ) == FALSE )
    {
		*acTempBuffer = '\0' ;
    }

	if( *acTempBuffer == '\0' )
    {
		m_iDataCacheExpiryRatio = 1 ;
	}
	else
    {
		m_iDataCacheExpiryRatio = (m4double_t) atof( acTempBuffer );

		if (m_iDataCacheExpiryRatio <= 0)
			m_iDataCacheExpiryRatio = 1;
	}


//Leemos CacheDefaultMaxPeriod
	/*
	Limite superior => No tiene
	Si 0 => Los objetos no tienen límite de tiempo (por defecto) en la cache.
	Valores negativos inválidos.
	Defecto 1
	*/
    memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_DATA_CACHE_DEFAULT_MAX_PERIOD, acTempBuffer ) == FALSE )
    {
		*acTempBuffer = '\0' ;
    }

	if( *acTempBuffer == '\0' )
    {
		m_iDataCacheDefaultMaxPeriod = 365 ;
	}
	else
    {
		m_iDataCacheDefaultMaxPeriod = (m4date_t) atof( acTempBuffer );

		if (m_iDataCacheDefaultMaxPeriod < 0)
			m_iDataCacheDefaultMaxPeriod = 1;
	}


//CACHE de METADATOS
//leemos e DiskCache
    memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_MD_CACHE_MODE, acTempBuffer ) == FALSE ){
		// Valor por defecto.
        m_iMDDiskCache = M4CL_CACHE_READ_WRITE;
    }
	else{
		m_iMDDiskCache = ConvertCacheMode(acTempBuffer);
	}
	if ( (m_iDataDiskCache != M4CL_CACHE_NONE) && (m_iMDDiskCache == M4CL_CACHE_NONE) )
	{
		//Igualamos el valor de la cache de MD al de la de Datos.
		//No debe estar desactivada la MD si esta la de Datos
		m_iMDDiskCache = m_iDataDiskCache;
		//g_oChLog<<BeginWarning(M4_CH_MCR_MD_CACHE_SETTINGS)<<EndLog;
	}


//Leemos CacheMaxMemory
	/*
	Limite superior => 4 Gb
	Si 0 => No hay límite de memoria
	Valores negativos inválidos.
	Defecto 0
	*/
    memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_MD_CACHE_MAX_MEMORY, acTempBuffer ) == FALSE )
    {
		*acTempBuffer = '\0' ;
    }

	if( *acTempBuffer == '\0' )
    {
		m_iMDCacheMaxMemory = 30000000 ;
	}
	else
    {
		dAux = atof( acTempBuffer );
		if (dAux < 0) dAux = 0;
		else if (dAux > 0xFFFFFFFF) dAux = 0xFFFFFFFF;
		m_iMDCacheMaxMemory = (m4uint32_t) dAux;
	}

	/*if ( !(m_iMDDiskCache & M4CL_CACHE_WRITE) )
		m_iMDCacheMaxMemory = 0; //Para que no haga swapping*/


//Leemos CacheMaxNumObjects
	/*
	Limite superior => 4 Gb
	Si 0 => No hay límite de memoria
	Valores negativos inválidos.
	Defecto 0
	*/
    memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_MD_CACHE_MAX_NUM_OBJECTS, acTempBuffer ) == FALSE )
    {
		*acTempBuffer = '\0' ;
    }
	if( *acTempBuffer == '\0' )
    {
		m_iMDCacheMaxNumObjects = 1000 ;
	}
	else
    {
		dAux = atof( acTempBuffer );
		if (dAux < 0) dAux = 0;
		else if (dAux > 0xFFFFFFFF) dAux = 0xFFFFFFFF;
		m_iMDCacheMaxNumObjects = (m4uint32_t) dAux;
	}


//Leemos CacheRefreshRatio
	/*
	Limite superior => 4 Gb
	Si 0 => Hace el refresh cada vez que se hace un Put en la caché
	Valores negativos inválidos.
	Defecto 0
	*/
    memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_MD_CACHE_REFRESH_RATIO, acTempBuffer ) == FALSE )
    {
		*acTempBuffer = '\0' ;
    }
	if( *acTempBuffer == '\0' )
    {
		m_iMDCacheRefreshRatio = 5 ;
	}
	else
    {
		dAux = atof( acTempBuffer );
		if (dAux < 0) dAux = 0;
		else if (dAux > 0xFFFFFFFF) dAux = 0xFFFFFFFF;
		m_iMDCacheRefreshRatio = (m4uint32_t) dAux;
	}


//Leemos CacheExpiryRatio
	/*
	Limite superior => No tiene.
	Si 0 => 0 es inválido, se le asigna el valor 1
	Valores negativos inválidos.
	Defecto 1
	No se utiliza por ahora.
	*/
    memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_MD_CACHE_EXPIRY_RATIO, acTempBuffer ) == FALSE )
    {
		*acTempBuffer = '\0' ;
    }
	if( *acTempBuffer == '\0' )
    {
		m_iMDCacheExpiryRatio = 1 ;
	}
	else
    {
		m_iMDCacheExpiryRatio = (m4double_t) atof( acTempBuffer );

		if (m_iMDCacheExpiryRatio <= 0)
			m_iMDCacheExpiryRatio = 1;
	}


//Leemos CacheDefaultMaxPeriod
	/*
	Limite superior => No tiene
	Si 0 => Los objetos no tienen límite de tiempo (por defecto) en la cache.
	Valores negativos inválidos.
	Defecto 1
	No se utiliza por ahora.
	*/
    memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_MD_CACHE_DEFAULT_MAX_PERIOD, acTempBuffer ) == FALSE )
    {
		*acTempBuffer = '\0' ;
    }

	if( *acTempBuffer == '\0' )
    {
		m_iMDCacheDefaultMaxPeriod = 365 ;
	}
	else
    {
		m_iMDCacheDefaultMaxPeriod = (m4date_t) atof( acTempBuffer );

		if (m_iMDCacheDefaultMaxPeriod < 0)
			m_iMDCacheDefaultMaxPeriod = 1;
	}

    
// ObjDataCache. MaxNumObjects
    memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_OBJDATA_CACHE_MAX_NUM_OBJECTS, acTempBuffer ) == FALSE )
    {
		*acTempBuffer = '\0' ;
    }
	if( *acTempBuffer == '\0' )
    {
		m_iObjDataCacheMaxNumObjects = 1000 ;
	}
	else
    {
		dAux = atof( acTempBuffer );
		if (dAux < 0) dAux = 0;
		else if (dAux > 0xFFFFFFFF) dAux = 0xFFFFFFFF;
		m_iObjDataCacheMaxNumObjects = (m4uint32_t) dAux;
	}
    
// ObjDataCache. MaxMemory
    memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	if( GetRegKeyData( acPath, M4_INIT_OBJDATA_CACHE_MAX_MEMORY, acTempBuffer ) == FALSE )
    {
		*acTempBuffer = '\0' ;
    }

	if( *acTempBuffer == '\0' )
    {
		m_iObjDataCacheMaxMemory = 30000000;
	}
	else
    {
		dAux = atof( acTempBuffer );
		if (dAux < 0) dAux = 0;
		else if (dAux > 0xFFFFFFFF) dAux = 0xFFFFFFFF;
		m_iObjDataCacheMaxMemory = (m4uint32_t) dAux;
	}

    return (M4_SUCCESS);
  //## end ClVMBaseEnv::ReadCacheRegistry%1987159710.body
}

m4return_t ClVMBaseEnv::SetPath (m4pchar_t ai_pcPathBase)
{
  //## begin ClVMBaseEnv::SetPath%237450391.body preserve=yes
	if (!ai_pcPathBase)
		ai_pcPathBase = M4_NEW_REGEDIT_KEY_CLIENT;

	//Leemos el path base del registry
	sprintf(m_acPathBase, "%s%s", M4_NEW_REGEDIT_KEY, ai_pcPathBase) ;
	ClVMDebugEnv::getInstance()->setPath(m_acPathBase);

	return M4_SUCCESS;
  //## end ClVMBaseEnv::SetPath%237450391.body
}

m4pchar_t ClVMBaseEnv::GetPath ()
{
  //## begin ClVMBaseEnv::GetPath%-1437626576.body preserve=yes
	return  ( m_acPathBase != NULL ?  m_acPathBase : "" );
  //## end ClVMBaseEnv::GetPath%-1437626576.body
}

void ClVMBaseEnv::Reset ()
{
  //## begin ClVMBaseEnv::Reset%-305520925.body preserve=yes
	// Por el problema del locale con el Internet Explorer, sin ActiveDesktop.
	// Cuando linkemos en estatico con la CRT no será necesario
	//M4ResetLocale();

	//m_Language = M4CL_LANGUAGE_DEFAULT ;
	m_iTimeUnit = M4_UNIT_ONE_DAY ;
    m_dTimeUnit = M4_ONE_DAY ;
	m_dTimeCorrection = 0.0;

	*m_acCacheDirectory   = '\0' ;
	*m_acVersion		  = '\0' ;
	*m_acDebugDirectory   = '\0' ;
	memset( m_acPathBase, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	*m_acTempDirectory    = '\0' ;
	*m_acWorkingDirectory = '\0' ;
	m_iSystemDebugEnable = 0;
	m_iSystemDebugDetailLevel = 0;

	m_iUseFieldUnicodeTypes = 0;
	m_iSMCacheRefresh = 0;

	m_iShowData = 0;

	m_iDataCacheDefaultMaxPeriod = 365 ;
	m_iDataCacheExpiryRatio = 1 ;
	m_iDataCacheMaxMemory = 30000000 ;
	m_iDataCacheMaxNumObjects = 100 ;
	m_iDataCacheRefreshRatio = 5 ;
	m_iDataDiskCache = M4CL_CACHE_READ_WRITE;

	m_iMDDiskCache = M4CL_CACHE_READ_WRITE;
	m_iMDCacheMaxMemory = 30000000 ;
	m_iMDCacheMaxNumObjects = 1000 ;
	m_iMDCacheRefreshRatio = 5 ;
	m_iMDCacheExpiryRatio = 1 ;
	m_iMDCacheDefaultMaxPeriod = 365 ;

	m_iObjDataCacheMaxMemory = 30000000;
	m_iObjDataCacheMaxNumObjects = 1000;

	// Datos miembros para LDB
	*m_acConExec = '\0' ;
    *m_acConnectionString = '\0' ;
	*m_acRegDBPwd = '\0' ;
	*m_acTempConnString = '\0' ;
	m_bMultFiles = M4_FALSE;
	m_DataBase = 0;
	m_eILevel = 0;
	m_iLDBDiskCache = M4CL_CACHE_NONE;
	m_numConn = 2;
	m_numCursor = 1;
	m_numObject = 1;
	m_numObjectTrans = 1;
	m_iBufferSize = 2048000;
	m_numStmtCache = 1;
	m_iMaxLDBInspSize = 100;
	*m_acDefaultDllMechanism = '\0';
	m_iMechanism=-1;
	*m_acDateSeparator = '\0';

  //## end ClVMBaseEnv::Reset%-305520925.body
}

m4int32_t ClVMBaseEnv::GetLogonDefaultMechanism ()
{
  //## begin ClVMBaseEnv::GetLogonDefaultMechanism%960822800.body preserve=yes
	return m_iMechanism;
  //## end ClVMBaseEnv::GetLogonDefaultMechanism%960822800.body
}

m4pcchar_t ClVMBaseEnv::GetLogonDefaultMechanismDllName ()
{
  //## begin ClVMBaseEnv::GetLogonDefaultMechanismDllName%960822801.body preserve=yes
	return m_acDefaultDllMechanism;
  //## end ClVMBaseEnv::GetLogonDefaultMechanismDllName%960822801.body
}

m4pcchar_t ClVMBaseEnv::GetAuthenticateDllByMechanism ()
{
  //## begin ClVMBaseEnv::GetAuthenticateDllByMechanism%960822802.body preserve=yes
	return 0;
  //## end ClVMBaseEnv::GetAuthenticateDllByMechanism%960822802.body
}

m4uint8_t ClVMBaseEnv::GetShowData ()
{
  //## begin ClVMBaseEnv::GetShowData%969966573.body preserve=yes
	return (m_iShowData);
  //## end ClVMBaseEnv::GetShowData%969966573.body
}

void ClVMBaseEnv::SetShowData (m4uint8_t ai_iShowData)
{
  //## begin ClVMBaseEnv::SetShowData%969966574.body preserve=yes
	m_iShowData = ai_iShowData;
  //## end ClVMBaseEnv::SetShowData%969966574.body
}

m4return_t ClVMBaseEnv::ReadRegistry (m4pchar_t ai_pPath, m4pchar_t ai_pKey, m4return_t ai_iShowError, m4int32_t ai_iMinValue, m4int32_t ai_iMaxValue, m4int32_t ai_iDefValue, m4int32_t& ao_iValue)
{
  //## begin ClVMBaseEnv::ReadRegistry%969966575.body preserve=yes
    m4char_t    acTempBuffer[ M4CH_MAX_TEMP_BUFFER + 1 ] ;

	memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;

	ao_iValue = ai_iDefValue;
	
	if( GetRegKeyData( ai_pPath, ai_pKey, acTempBuffer ) == FALSE )
    {
		switch (ai_iShowError) {
		case M4_ERROR:
			DUMP_CHLOG_ERRORF(M4_CH_OBJREG_PROPERTY_NOT_FOUND, ai_pKey);
			return (M4_ERROR);
			break;
		case M4_WARNING:
			DUMP_CHLOG_WARNINGF(M4_CH_OBJREG_PROPERTY_NOT_FOUND, ai_pKey << ao_iValue);
			return (M4_SUCCESS);
			break;
		case M4_SUCCESS:
			return (M4_SUCCESS);
			break;
		}
    }

	ao_iValue = atoi( acTempBuffer );

	if ((ao_iValue < ai_iMinValue) || (ao_iValue > ai_iMaxValue)) {
		DUMP_CHLOG_WARNINGF(M4_CH_OBJREG_WRONG_PROPERTY_VALUE, ai_pKey << ai_iDefValue << ao_iValue);
		ao_iValue = ai_iDefValue;
		return (M4_SUCCESS);
	}

	return (M4_SUCCESS);
  //## end ClVMBaseEnv::ReadRegistry%969966575.body
}

// Additional Declarations
  //## begin ClVMBaseEnv%3912A1F501F4.declarations preserve=yes
  //## end ClVMBaseEnv%3912A1F501F4.declarations

//## begin module%3912A1F60078.epilog preserve=yes
//## end module%3912A1F60078.epilog

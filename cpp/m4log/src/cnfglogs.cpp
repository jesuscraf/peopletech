//## begin module%377C88560120.cm preserve=no
//## end module%377C88560120.cm

//## begin module%377C88560120.cp preserve=no
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
//## end module%377C88560120.cp

//## Module: cnfglogs%377C88560120; Package body
//## Subsystem: M4Log::src%377C88560044
//## Source file: D:\ricardo\m4log\src\cnfglogs.cpp

//## begin module%377C88560120.additionalIncludes preserve=no
//## end module%377C88560120.additionalIncludes

//## begin module%377C88560120.includes preserve=yes
#include "regedit.hpp"
#include "m4tratini.hpp"
#include "logmanag.hpp"
#include <dbgresou.hpp>
#include <cnfglogs.hpp>
#include <m4types.hpp>
#include <m4string.hpp>
#include "m4xmlreg.hpp"
#include "cldefine.hpp"



//## begin module%377C88560120.declarations preserve=no
//## end module%377C88560120.declarations

//## begin module%377C88560120.additionalDeclarations preserve=yes


// Tamaño maximo del buffer que se le puede pasar a la funcion
// GetPrivateProfileSection 

//		w95/98			=> maximo = 32KB
//		NT/w2000/unix	=> no hay maximo

const m4uint32_t	M4LOG_MAX_PROFILE_SECTION_BUFFER_SIZE = 32000;

//## end module%377C88560120.additionalDeclarations


// Class ClDefaultConfiguration 



//## begin ClDefaultConfiguration::myInstance%377C883E0282.role preserve=no  private: static ClDefaultConfiguration {1 -> 1RHAPN}
ClDefaultConfiguration *ClDefaultConfiguration::myInstance = 0;
//## end ClDefaultConfiguration::myInstance%377C883E0282.role



ClDefaultConfiguration::ClDefaultConfiguration ()
  //## begin ClDefaultConfiguration::ClDefaultConfiguration%-60473350.hasinit preserve=no
  //## end ClDefaultConfiguration::ClDefaultConfiguration%-60473350.hasinit
  //## begin ClDefaultConfiguration::ClDefaultConfiguration%-60473350.initialization preserve=yes
  //## end ClDefaultConfiguration::ClDefaultConfiguration%-60473350.initialization
{
  //## begin ClDefaultConfiguration::ClDefaultConfiguration%-60473350.body preserve=yes


	m_sReturn = 10;
	m_sStorageAll = 1;
	m_sTimeDeb = 0;
	m_sTimeWar = 0;
	m_sTimeErr = 1;
	m_sOpWar = 1;
	m_sOpDeb = 1;
	m_sOpErr = 1;
	m_sBase = "111110001000";
	m_iDefault = ALL_STORAGES;           // -->  1110
	m_iPort = 0;
	m_iFileName = "DefaultLogFile.dat";
	m_lBufferSize = 100000;
	m_sType = 0;               // 0 -> File;  1 -> tcp; 2 -> errortail
	m_iStorages = 1;
	m_uiDefaultMaxQueueSize = 50;

  //## end ClDefaultConfiguration::ClDefaultConfiguration%-60473350.body
}


ClDefaultConfiguration::~ClDefaultConfiguration ()
{
  //## begin ClDefaultConfiguration::~ClDefaultConfiguration%-1163554636.body preserve=yes


/*	if (myInstance != 0) {
		delete myInstance;
	};
*/

  //## end ClDefaultConfiguration::~ClDefaultConfiguration%-1163554636.body
}



//## Other Operations (implementation)
ClDefaultConfiguration* ClDefaultConfiguration::Instance ()
{
  //## begin ClDefaultConfiguration::Instance%-761489822.body preserve=yes

	if (myInstance == 0) {
		myInstance = new ClDefaultConfiguration;
	};
	return myInstance;

  //## end ClDefaultConfiguration::Instance%-761489822.body
}

// Additional Declarations
  //## begin ClDefaultConfiguration%377C883E012F.declarations preserve=yes
  //## end ClDefaultConfiguration%377C883E012F.declarations

// Class ClConfigLogs 







ClConfigLogs::ClConfigLogs ()
  //## begin ClConfigLogs::ClConfigLogs%-2092739115.hasinit preserve=no
  //## end ClConfigLogs::ClConfigLogs%-2092739115.hasinit
  //## begin ClConfigLogs::ClConfigLogs%-2092739115.initialization preserve=yes
  //## end ClConfigLogs::ClConfigLogs%-2092739115.initialization
{
  //## begin ClConfigLogs::ClConfigLogs%-2092739115.body preserve=yes

	m_siniFile = "";
	m_iwarning = 0;
	m_ierror = 0;
	m_idebugInfo = 0; 
	m_sbase = "";
	m_plogManager = NULL;
	m_aiDefaultFlag = 7;
	m_iStorages = 4;
	m_numofModules = 0;
	m_oDefaultConfig = ClDefaultConfiguration::Instance();
	m_pModuleList = new ClStaticModuleList();
    m_bModuleListOwner = M4_TRUE;

  //## end ClConfigLogs::ClConfigLogs%-2092739115.body
}

ClConfigLogs::ClConfigLogs (ClLogManager* plogManager, M4ClString siniFile, ClStaticResourceList* resources)
  //## begin ClConfigLogs::ClConfigLogs%-857815257.hasinit preserve=no
  //## end ClConfigLogs::ClConfigLogs%-857815257.hasinit
  //## begin ClConfigLogs::ClConfigLogs%-857815257.initialization preserve=yes
  //## end ClConfigLogs::ClConfigLogs%-857815257.initialization
{
  //## begin ClConfigLogs::ClConfigLogs%-857815257.body preserve=yes

    m_plogManager = plogManager;
	m_siniFile = siniFile;
	m_iwarning = 0;
	m_ierror = 0;
	m_idebugInfo = 0;
	m_sbase = "";	
	m_aiDefaultFlag = 7;
	m_iStorages = 4;
    m_bModuleListOwner = M4_FALSE;
    m_pModuleList = NULL;

  //## end ClConfigLogs::ClConfigLogs%-857815257.body
}


ClConfigLogs::~ClConfigLogs ()
{
  //## begin ClConfigLogs::~ClConfigLogs%140338991.body preserve=yes
    if ((NULL != m_pModuleList) && (M4_TRUE == m_bModuleListOwner))
    {
        delete m_pModuleList;
        m_pModuleList = NULL;
        m_bModuleListOwner = M4_FALSE;
    }
  //## end ClConfigLogs::~ClConfigLogs%140338991.body
}



//## Other Operations (implementation)
m4return_t ClConfigLogs::SetIniFile (const M4ClString& siniFile)
{
  //## begin ClConfigLogs::SetIniFile%51879479.body preserve=yes

	m_siniFile = siniFile;

	return M4_SUCCESS;

  //## end ClConfigLogs::SetIniFile%51879479.body
}

m4return_t ClConfigLogs::SetLogManager (ClLogManager* plogManager)
{
  //## begin ClConfigLogs::SetLogManager%419751567.body preserve=yes

	m_plogManager = plogManager;
	return M4_SUCCESS;

  //## end ClConfigLogs::SetLogManager%419751567.body
}

m4return_t ClConfigLogs::Initialize ()
{
  //## begin ClConfigLogs::Initialize%2020929601.body preserve=yes

	
	m4return_t	 iret=M4_SUCCESS;

	GetDefaultConfig();

	/* Se llama desde INITIALIZELOGMANAGER
	para poder concatenar el path de workdir
	LogBasePath = getenv("M4_LOGBASE_PATH");
	if (LogBasePath==NULL)
		updateConfig("./logbase.ini");	
	else 
		updateConfig(LogBasePath);
	*/

	return iret;

  //## end ClConfigLogs::Initialize%2020929601.body
}

m4int32_t ClConfigLogs::GetDefaultLevelInfo ()
{
  //## begin ClConfigLogs::GetDefaultLevelInfo%148456083.body preserve=yes


	return m_aiDefaultFlag;

  //## end ClConfigLogs::GetDefaultLevelInfo%148456083.body
}

m4int16_t ClConfigLogs::DecodeNextMessage (m4char_t*& ai_Iterator, m4int16_t* ao_level, m4int64_t* ao_Code)
{
  //## begin ClConfigLogs::DecodeNextMessage%-572112631.body preserve=yes


	char *auxIt = NULL, *auxIt2=NULL;
	const char rem = 39;   //  39 => '
	size_t	totalsize = 0;
	
	totalsize = strlen (ai_Iterator)+1;
	
	if (*ai_Iterator=='\0') return M4_ERR;
	
	if (*ai_Iterator!=rem) {

		auxIt = strchr(ai_Iterator,'=');	
		if (!auxIt) {
			ai_Iterator += totalsize;
			return M4_REM;
		}
		
		auxIt = strtok(ai_Iterator,"=");		
		if (compareValues(auxIt,"exception")) return M4_REM;
		auxIt2= auxIt + strlen(auxIt) + 1;		
		
		auxIt2 = strtok(auxIt2,",");
		*ao_Code = atol (auxIt2);
		if ((*ao_Code)==0) return M4_REM;		
		auxIt2 += strlen(auxIt2)+1;
		auxIt2 = strtok(auxIt2,";");
		auxIt2 = strtok(auxIt2,"p");
		auxIt2 += strlen(auxIt2)+1;
		*ao_level = atoi(auxIt2);
		ai_Iterator += totalsize;
		return M4_OK;
	};

	ai_Iterator += totalsize;
	return M4_REM;


  //## end ClConfigLogs::DecodeNextMessage%-572112631.body
}

m4return_t ClConfigLogs::GetDefaultConfig ()
{
  //## begin ClConfigLogs::GetDefaultConfig%1730464281.body preserve=yes


	// ----------------------------------------	   internal logParams.

	m_plogManager ->	SetCReturn( m_oDefaultConfig -> m_sReturn );

	
	// ----------------------------------------    default logParams
	
	m_showTime =		M4_FALSE;
	m_version =			"undefined version";
	m_plogManager ->	ActiveError();
	m_plogManager ->	ActiveWarning();
	m_plogManager ->	ActiveDebugInfo();
	m_plogManager ->	DefuseTraceInfo();
	m_iStorages=		1;
	m_numofModules=		1;
	
	// ----------------------------------------    default storage.

	ClFileStorage		*pstorageFile;
	pstorageFile=		new ClFileStorage( m_plogManager, "defaultLogFile", M4_LOGFILE_LOG_FORMAT, "defaultLogFile" );
	m_plogManager ->	AddStorage( pstorageFile );

	// ----------------------------------------    deviceGroups
	// todos a cero menos el primero.

	m4int16_t iter = 0;
	m_aiflags[iter]=1;
	iter++;
	while (iter<MAX_DEVICES_GROUP) {
		m_aiflags[iter]=0;
		iter++;
	}

	// ----------------------------------------    outputPatterns
	// solo el patron por defecto.
	
	ClOutputPattern *pattern = new ClOutputPattern();

	pattern ->			SetOutputPatternName ( "defaultPattern" );

	pattern ->			SetErrorLevel(1);
	pattern ->			SetWarningLevel(1);	
	pattern ->			SetDebugLevel(0);	
	pattern ->			SetTraceLevel(0);	

	m_pModuleList ->	getOutputPatternMap () -> addOutputPattern ( pattern );

	// ----------------------------------------    applicationModules

//	m_pModuleList ->	SetModuleListSize(m_numofModules-1);
	m_pModuleList ->	SetFlagsLevels(m_aiflags);

	SETCODEF(0,DEBUGINFOLOG,"-- Internal M4Log trace. \n\t\t Setting default configuration. SUCCESS");
	
	return M4_SUCCESS;


  //## end ClConfigLogs::GetDefaultConfig%1730464281.body
}

m4return_t ClConfigLogs::GetOptions ()
{
  //## begin ClConfigLogs::GetOptions%-771835827.body preserve=yes

	m4int16_t	iret = 0;
	M4ClString saux="";

	iret = (m4int16_t) M4GetPrivateProfileInt( "Options", "Warning", 0, m_siniFile );
	if (  iret )
		m_plogManager -> ActiveWarning();
	else
		m_plogManager -> DefuseWarning();
				
	iret = (m4int16_t) M4GetPrivateProfileInt( "Options", "Error", 0, m_siniFile );
	if (  iret )
		m_plogManager -> ActiveError();
	else
		m_plogManager -> DefuseError();


	iret = (m4int16_t) M4GetPrivateProfileInt( "Options", "DebugInfo", 0, m_siniFile );
	if (  iret )
		m_plogManager -> ActiveDebugInfo();
	else
		m_plogManager -> DefuseDebugInfo();

	
	iret = (m4int16_t) M4GetPrivateProfileInt( "Options", "TraceInfo", 0, m_siniFile );
	if (  iret )
		m_plogManager -> ActiveTraceInfo();
	else
		m_plogManager -> DefuseTraceInfo();


	iret = (m4int16_t) M4GetPrivateProfileInt( "Options", "RemoteLog", 0, m_siniFile );
	if (  iret )
		m_plogManager -> EnableRemoteLog();
	else
		m_plogManager -> DisableRemoteLog();
		

	if (M4GetPrivateProfileString( "Options", "MaxQueueSize", "", saux, m_siniFile ) != M4_ERROR )
	
		m_plogManager -> SetMaxQueueSize( strtol ((M4ClString &) saux, NULL, 10) );
	else
		m_plogManager -> SetMaxQueueSize(MAX_QUEUE_SIZE);
	
	
	return M4_SUCCESS;

  //## end ClConfigLogs::GetOptions%-771835827.body
}

m4return_t ClConfigLogs::GetConfig ()
{
  //## begin ClConfigLogs::GetConfig%1853456006.body preserve=yes


	M4ClString	sDefault	= "";
	m4int32_t	iret;

	iret = M4GetPrivateProfileString( "logParameters", "version", "", sDefault, m_siniFile );
	if (iret==M4_SUCCESS) m_version = sDefault;
	else {
		SETCODEF(0,ERRORLOG,"-- Internal M4Log error.\n\t\t Undefined configuration file version. unable to update log configuration.");	
		return M4_ERROR;
	};

	iret = M4GetPrivateProfileString( "logParameters", "showTime", "", sDefault, m_siniFile );
	if (!compareValues((m4pchar_t)sDefault,"TRUE")) m_showTime = M4_TRUE;
	else
		m_showTime = M4_FALSE;
	
	if (iret==M4_SUCCESS)
		m_plogManager -> setShowTime(m_showTime);

	iret = M4GetPrivateProfileString( "logParameters", "processErrors", "", sDefault, m_siniFile );
	if (iret==M4_SUCCESS) {
		if (!compareValues((m4pchar_t)sDefault,"TRUE")) m_plogManager -> ActiveError();
		else
			m_plogManager -> DefuseError();
	}

	iret = M4GetPrivateProfileString( "logParameters", "processWarnings", "", sDefault, m_siniFile );
	if (iret==M4_SUCCESS) {
		if (!compareValues((m4pchar_t)sDefault,"TRUE")) m_plogManager -> ActiveWarning();
		else
			m_plogManager -> DefuseWarning();
	}

	iret = M4GetPrivateProfileString( "logParameters", "processDebugInfo", "", sDefault, m_siniFile );
	if (iret==M4_SUCCESS) {
		if (!compareValues((m4pchar_t)sDefault,"TRUE")) m_plogManager -> ActiveDebugInfo();
		else
			m_plogManager -> DefuseDebugInfo();
	}

	iret = M4GetPrivateProfileString( "logParameters", "processTraceInfo", "", sDefault, m_siniFile );
	if (iret==M4_SUCCESS) {
		if (!compareValues((m4pchar_t)sDefault,"TRUE")) m_plogManager -> ActiveTraceInfo();
		else
			m_plogManager -> DefuseTraceInfo();
	}
	
	iret = M4GetPrivateProfileString( "logParameters", "translateMessages", "", sDefault, m_siniFile );
	if (iret==M4_SUCCESS) {
		if (!compareValues((m4pchar_t)sDefault,"TRUE")) 
			m_plogManager -> enableTranslateMessages();
		else if (!compareValues((m4pchar_t)sDefault,"FALSE")) 
			m_plogManager -> disableTranslateMessages();
		else 
			m_plogManager -> bothTranslateMessages();
	}

	iret  = (m4int32_t) M4GetPrivateProfileInt( "logParameters", "language", 0, m_siniFile );
	if (iret>0)
	m_plogManager -> setDefaultIdiom(iret);

	iret  = (m4int32_t) M4GetPrivateProfileInt( "logParameters", "numOfOutputDevices", 0, m_siniFile );
//	if (iret>0)
	m_iStorages = iret;
	
	
	iret =  (m4int32_t) M4GetPrivateProfileInt( "logParameters", "numOfApplicationModules", 0, m_siniFile );
//	if (iret>0)
	m_numofModules = iret;
	
	return M4_SUCCESS;

  //## end ClConfigLogs::GetConfig%1853456006.body
}

m4return_t ClConfigLogs::GetStorages ()
{
  //## begin ClConfigLogs::GetStorages%-211260275.body preserve=yes

	
	m4uint16_t	icont = 1;
	M4ClString	sstorage = "outputDevice";
	M4ClString	snstorage = "";
	M4ClString	sstrgType = "";
	
	
	snstorage = sstorage;


	for ( snstorage  += icont; icont <= m_iStorages; )
	{
		

		M4GetPrivateProfileString( snstorage, "deviceType", "", sstrgType, m_siniFile );

		if ( !compareValues(sstrgType.c_str(),"PUBLIC_LOG_FILE") )
		{
			if ( GetStorageFile( snstorage ) == M4_ERROR )	{
				m_plogManager -> AddStorage( NULL );
			}
		} else {
			
			if ( !compareValues(sstrgType.c_str(),"PRIVATE_MESSAGE_QUEUE" ) )	{
				if ( GetStorageQueue( snstorage ) == M4_ERROR )	{
						m_plogManager -> AddStorage( NULL );
					}
			}
			else {
				m_plogManager -> AddStorage( NULL );
			}
		}
		
		snstorage = sstorage;
		icont++;
		snstorage += icont;
	}
	
	return M4_SUCCESS;

  //## end ClConfigLogs::GetStorages%-211260275.body
}

m4return_t ClConfigLogs::GetStorageFile (const M4ClString& snstorage)
{
  //## begin ClConfigLogs::GetStorageFiles%1440062945.body preserve=yes

	m4int16_t		iFileFormat = M4_LOGFILE_LOG_FORMAT;
	m4pcchar_t		pccName = NULL;
	m4pcchar_t		pccSlash = NULL;
	m4pchar_t		pcDot = NULL;
	M4ClString		sName = "";
	M4ClString		sFormat = "";
	M4ClString		sDevice = "";
	ClFileStorage	*pstorageFile = NULL;


	M4GetPrivateProfileString( snstorage, "filePath", "", sName, m_siniFile );
	pccName = sName.c_str();

	pccSlash = strrchr( pccName, '\\' );

	if( pccSlash == NULL )
	{
		pccSlash = strrchr( pccName, '/' );
	}

	if( pccSlash != NULL )
	{
		pccName = pccSlash + 1;
	}

	pcDot = (m4pchar_t) strrchr( pccName, '.' );

	if( pcDot != NULL )
	{
		*pcDot = '\0';
	}

	M4GetPrivateProfileString( snstorage, "fileFormat", "LOG_FORMAT", sFormat, m_siniFile );

	if( strcmpi( sFormat, "HTML_FORMAT" ) == 0 )
	{
		iFileFormat = M4_LOGFILE_HTML_FORMAT;
	}
	
	M4GetPrivateProfileString( snstorage, "deviceName", "undefined name", sDevice, m_siniFile );

	pstorageFile = new ClFileStorage( m_plogManager, pccName, iFileFormat, sDevice.c_str() );
	m_plogManager -> AddStorage( pstorageFile );

	return M4_SUCCESS;

  //## end ClConfigLogs::GetStorageFiles%1440062945.body
}

m4return_t ClConfigLogs::GetStorageQueue (const M4ClString& snstorage)
{
  //## begin ClConfigLogs::GetStorageQueue%-412468965.body preserve=yes


	ClPrivateLocalQueue	*pstorageQueue;
	M4ClString	sDefault = "";
	m4int32_t	iret;


	pstorageQueue = new ClPrivateLocalQueue( m_plogManager );

	iret = M4GetPrivateProfileString( snstorage, "deviceName", "undefined name", sDefault, m_siniFile );

	iret = (m4int16_t) M4GetPrivateProfileInt( snstorage , "windowSize", MAX_QUEUE_SIZE, m_siniFile );
	if (iret>0)	
		m_plogManager -> SetMaxQueueSize(iret);

	m_plogManager -> AddStorage( pstorageQueue );
 

	return M4_SUCCESS;

  //## end ClConfigLogs::GetStorageQueue%-412468965.body
}

m4return_t ClConfigLogs::GetModules ()
{
  //## begin ClConfigLogs::GetModules%-1799102039.body preserve=yes


	M4ClString	moduleTag	=	 "applicationModule";
	M4ClString	composed_moduleTag	=	"";
	M4ClString	moduleName = "";
	M4ClString	sDefault = "";
	
	m4uint16_t	iter	=	1;
	m4int32_t	lcode = 0;
	m4int32_t	lfinal = 0;
	
	m4char_t aSectionIterator[M4LOG_MAX_PROFILE_SECTION_BUFFER_SIZE];	
	m4pchar_t SectionIterator = aSectionIterator;
	m4int16_t p=0, cnt=0, size = 1, level = 0, iret=0;
	m4uint32_t iflag = 0;
	m4int64_t code = 0, nSize = M4LOG_MAX_PROFILE_SECTION_BUFFER_SIZE;

	ClModuleInfo	*auxModule;

	composed_moduleTag	=	moduleTag;
	composed_moduleTag	+=	iter;
	
	m_pModuleList -> SetFlagsLevels(m_aiflags);

	while (iter<=m_numofModules) {

		if (M4GetPrivateProfileString( composed_moduleTag, "moduleName", "", moduleName, m_siniFile) == M4_SUCCESS ) {
			
			auxModule = new ClModuleInfo();

			lcode  = M4GetPrivateProfileInt( composed_moduleTag, "initCodeNumber",0 , m_siniFile );
			lfinal = M4GetPrivateProfileInt( composed_moduleTag, "finalCodeNumber",0 , m_siniFile );
			M4GetPrivateProfileString( composed_moduleTag,"outputPatternName","defaultPattern", sDefault, m_siniFile);

			auxModule -> SetOutputPattern ( m_pModuleList -> getOutputPatternMap ()->getOutputPatternByName(sDefault) );
			auxModule -> SetModuleName(moduleName);
			auxModule -> SetInitCodeNumber	( lcode  );
			auxModule -> SetFinalCodeNumber	( lfinal );
			
			SectionIterator = aSectionIterator;

			M4GetPrivateProfileSection (composed_moduleTag, SectionIterator , (long)nSize, m_siniFile);

			while ((iret=DecodeNextMessage(SectionIterator,&level,&code))!=M4_ERR) {

				if (iret==M4_OK) {
					if (level<=0 || level>=MAX_DEVICES_GROUP) 
						iflag = 0;
					else
						iflag = m_aiflags[ level - 1 ];
					auxModule -> AddCodeException( (long)code, iflag);
				}	
			}

			if (m_pModuleList -> AddModule(auxModule) == M4_WARNING )
				delete auxModule;
		}

		iter++;
		composed_moduleTag	=	moduleTag;
		composed_moduleTag	+=	iter;

	}
	
	return M4_SUCCESS;

  //## end ClConfigLogs::GetModules%-1799102039.body
}

m4return_t ClConfigLogs::GetFlags ()
{
  //## begin ClConfigLogs::GetFlags%-1324618017.body preserve=yes

	m4uint32_t	ibase = 0;
	m4uint32_t	iflag = 0;
//	m4uint32_t	iDefault = 7;
	M4ClString	sflag = "";
	m4uint16_t	ilevel = 1;
	M4ClString	slevel = "";
	M4ClString	sbase = "";
	M4ClString	sDefault = "";

	slevel = "deviceGroup";
	slevel += ilevel;
	while ( ilevel<MAX_DEVICES_GROUP) {
		
		if (M4GetPrivateProfileString( "deviceGroup", slevel, ""
				, sflag, m_siniFile ) != M4_ERROR ) {
			iflag = strtol( (M4ClString &) sflag, NULL, 2 );
			m_aiflags[ ilevel - 1 ] = iflag;
		}

		slevel = "deviceGroup";
		ilevel++;
		slevel += ilevel;
	}

	return M4_SUCCESS;

  //## end ClConfigLogs::GetFlags%-1324618017.body
}

m4return_t ClConfigLogs::GetOutputPatterns ()
{
  //## begin ClConfigLogs::GetOutputPatterns%365418924.body preserve=yes


	M4ClString	patternTag	=	 "outputPattern";
	M4ClString	composed_patternTag	=	"";
	M4ClString	outputPatternName = "";
	M4ClString	sDefault = "";
	m4int16_t	iter	=	1;

	composed_patternTag	=	patternTag;
	composed_patternTag	+=	iter;

	while ( M4GetPrivateProfileString( composed_patternTag, "outputPatternName", "", outputPatternName, m_siniFile) == M4_SUCCESS ) {
		
		ClOutputPattern *pattern = new ClOutputPattern();
		m4int32_t deviceGroup=0;

		pattern -> SetOutputPatternName ( outputPatternName.c_str() );

		M4GetPrivateProfileString( composed_patternTag,"errorDeviceGroup","deviceGroup0", sDefault, m_siniFile);
		deviceGroup = atoi(sDefault.substr(strlen("deviceGroup"),sDefault.size()).c_str());
		if (deviceGroup>=MAX_DEVICES_GROUP) deviceGroup=0;
		pattern -> SetErrorLevel(deviceGroup);	

		M4GetPrivateProfileString( composed_patternTag,"warningDeviceGroup","deviceGroup0", sDefault, m_siniFile);
		deviceGroup = atoi(sDefault.substr(strlen("deviceGroup"),sDefault.size()).c_str());
		if (deviceGroup>=MAX_DEVICES_GROUP) deviceGroup=0;
		pattern -> SetWarningLevel(deviceGroup);	

		M4GetPrivateProfileString( composed_patternTag,"debugDeviceGroup","deviceGroup0", sDefault, m_siniFile);
		deviceGroup = atoi(sDefault.substr(strlen("deviceGroup"),sDefault.size()).c_str());
		if (deviceGroup>=MAX_DEVICES_GROUP) deviceGroup=0;
		pattern -> SetDebugLevel(deviceGroup);	

		M4GetPrivateProfileString( composed_patternTag,"traceDeviceGroup","deviceGroup0", sDefault, m_siniFile);
		deviceGroup = atoi(sDefault.substr(strlen("deviceGroup"),sDefault.size()).c_str());
		if (deviceGroup>=MAX_DEVICES_GROUP) deviceGroup=0;
		pattern -> SetTraceLevel(deviceGroup);	

		if ((m_pModuleList -> getOutputPatternMap () -> addOutputPattern ( pattern ))==M4_WARNING) {
			delete pattern;
		}
	
		iter++;
		composed_patternTag	=	patternTag;
		composed_patternTag	+=	iter;

	}

	return M4_SUCCESS;

  //## end ClConfigLogs::GetOutputPatterns%365418924.body
}

m4uint32_t ClConfigLogs::LevelsAnd (m4uint32_t ilevel)
{
  //## begin ClConfigLogs::LevelsAnd%390278744.body preserve=yes

	m4uint32_t	icpbase = 0;
	m4uint32_t	iflag = 0;
	m4uint32_t	iretflag = 0;
	m4uint32_t	irot = 0;
	m4uint32_t	isizeBase;
	m4uint32_t	ibase = 0;

	icpbase = strtoul( (M4ClString &) m_sbase, NULL, 2 );
	isizeBase = (m4uint32_t) m_sbase.size();

	irot = isizeBase - ilevel * 4;

	if ( irot < 0 )
		return 0;

	iflag = m_aiflags[ ilevel - 1 ];
	
	icpbase = icpbase >> irot;

	iretflag = (m4int16_t) icpbase & iflag;

	return iretflag;

  //## end ClConfigLogs::LevelsAnd%390278744.body
}

m4return_t ClConfigLogs::GetDebugResources ()
{
  //## begin ClConfigLogs::GetDebugResources%1690520073.body preserve=yes


	STRQUEUE::iterator iter;
	M4ClString module;

	m4char_t aSectionIterator[M4LOG_MAX_PROFILE_SECTION_BUFFER_SIZE];
	m4pchar_t SectionIterator = aSectionIterator;
	m4int16_t p=0, cnt=0, size = 1, level = 0, iret=0;
	m4uint32_t iflag = 0;
	m4int64_t code = 0, nSize = M4LOG_MAX_PROFILE_SECTION_BUFFER_SIZE;

	
	m4int32_t	lcode = 0;
	m4int32_t	lfinal = 0;
	m4int32_t   lDebugLevel = 0;
	m4int32_t	lWarningLevel = 0;
	m4int32_t	lErrorLevel = 0;
	m4int32_t	lTraceLevel = 0;

	ClModuleInfo	*auxModule;

	while ( ! m_qmodules.empty() )
	{
		
		auxModule = new ClModuleInfo();
		
		iter = m_qmodules.begin();
		module = *iter;
		m_qmodules.pop_front();

		lcode  = M4GetPrivateProfileInt( module, "InitCodeNumber"
					, 0	, m_siniFile );


		lfinal = M4GetPrivateProfileInt( module, "FinalCodeNumber"
					, 0	, m_siniFile );

		
		// Este -1 indica que no esta definido y que se mire la configuracion
		// de la seccion DefaultModuleOptions.

	
	
		auxModule -> SetModuleName		( module );
		auxModule -> SetInitCodeNumber	( lcode  );
		auxModule -> SetFinalCodeNumber	( lfinal );
	

		M4GetPrivateProfileSection (module, SectionIterator , (long)nSize, m_siniFile);

		while ((iret=DecodeNextMessage(SectionIterator,&level,&code))!=M4_ERR) {

			if (iret==M4_OK) {
				if (level<=0 || level>=MAX_DEVICES_GROUP) 
					iflag = 0;
				else
					iflag = m_aiflags[ level - 1 ];
				auxModule -> AddCodeException( (long)code, iflag);
			}	
			
		}

	
		m_pModuleList -> AddModule(auxModule);

	}

	M4GetPrivateProfileSection ("Messages", SectionIterator , (long)nSize, m_siniFile);

	while ((iret=DecodeNextMessage(SectionIterator,&level,&code))!=M4_ERR) {

		if (iret==M4_OK) {
			if (level<=0 || level>=MAX_DEVICES_GROUP) 
				iflag = 0;
			else
				iflag = m_aiflags[ level - 1 ];
			m_pModuleList -> GetDefaultCodeExceptions() -> AddResource( (long)code, iflag);
		}	
	
	}

  return M4_SUCCESS;

  //## end ClConfigLogs::GetDebugResources%1690520073.body
}

m4return_t ClConfigLogs::DecodeMessage (const M4ClString& smessage, m4uint32_t& iflag)
{
  //## begin ClConfigLogs::DecodeMessage%-393270688.body preserve=yes

//	m4int32_t	ibase = m_ibase;
	size_t		pos = 0;
	m4uint32_t	inflag = 0;


	pos = smessage.find( ';' );
    if ( pos == M4_STR_NPOS)
        return M4_ERROR;

	//cambio para la compilación UNIX en HP, pero válido para todas las plataformas.
	//snflag = (M4ClString &) smessage.substr( 0, pos );
	inflag = strtol((smessage.substr( 0, pos)).c_str() , NULL, 10 );
	//inflag = strtol( (M4ClString &) snflag, NULL, 10 );

	iflag = m_aiflags[ inflag - 1 ];
	return M4_SUCCESS;

  //## end ClConfigLogs::DecodeMessage%-393270688.body
}

m4return_t ClConfigLogs::SetModuleList (ClStaticModuleList* ai_ModuleList)
{
  //## begin ClConfigLogs::SetModuleList%-522544410.body preserve=yes
    if ((NULL != m_pModuleList) && (M4_TRUE == m_bModuleListOwner))
    {
        delete m_pModuleList;
        m_pModuleList = NULL;
        m_bModuleListOwner = M4_FALSE;
    }

    m_pModuleList = ai_ModuleList;
	
	return M4_SUCCESS;

  //## end ClConfigLogs::SetModuleList%-522544410.body
}

m4return_t ClConfigLogs::updateConfig ()
{
  //## begin ClConfigLogs::updateConfig%-25023995.body preserve=yes


	SETCODEF(0,DEBUGINFOLOG,"-- Internal M4Log trace. \n\t\t Starting to update log configuration from file \"%s\" .",m_siniFile.c_str());

	if ( GetConfig() == M4_ERROR )
		return M4_ERROR;

	if ( GetStorages() == M4_ERROR )
		return M4_ERROR;
		
	if ( GetFlags() == M4_ERROR )
		return M4_ERROR;
			
	if (GetOutputPatterns() == M4_ERROR )
		return M4_ERROR;
			
	if ( GetModules() == M4_ERROR )
		return M4_ERROR;

	SETCODEF(0,DEBUGINFOLOG,"-- Internal M4Log trace. \n\t\t Update complete. SUCCESS");
			
//	if (aux) fclose (aux);

	return M4_SUCCESS;

  //## end ClConfigLogs::updateConfig%-25023995.body
}

m4return_t ClConfigLogs::updateConfig (const M4ClString& ai_configFile)
{
  //## begin ClConfigLogs::updateConfig%-118056103.body preserve=yes


	m_siniFile = ai_configFile;
	
	if ( m_siniFile.empty() )
	{
		return M4_ERROR;
	}

	return updateConfig();


  //## end ClConfigLogs::updateConfig%-118056103.body
}

m4char_t ClConfigLogs::GetEndMessageChar ()
{
  //## begin ClConfigLogs::GetEndMessageChar%1149524335.body preserve=yes

	m4int32_t	iEndMessage;

	iEndMessage = M4GetPrivateProfileInt( "Config", "MessageChar", '\n', m_siniFile );
	if ( ( iEndMessage > 255 ) || ( iEndMessage < 0  ) )
	{
			return( '\n' ) ;
	}

	return( m4char_t( iEndMessage ) ) ;

  //## end ClConfigLogs::GetEndMessageChar%1149524335.body
}

m4int32_t ClConfigLogs::compareValues (m4pcchar_t ai_Value, m4pcchar_t ai_Default)
{
  //## begin ClConfigLogs::compareValues%933951072.body preserve=yes
 
#ifdef M4LOG_CASESENSITIVE

	return strcmp(ai_Value,ai_Default);

#else

	size_t uiValueSize		= strlen (ai_Value)+1;
	size_t uiDefaultSize	= strlen (ai_Default)+1;

	if (uiValueSize!=uiDefaultSize) return 1;

	m4pchar_t  lowerValue		= (m4pchar_t) malloc (uiValueSize);
	m4pchar_t  lowerDefault		= (m4pchar_t) malloc (uiDefaultSize);

	memcpy(lowerValue,ai_Value,uiValueSize);
	memcpy(lowerDefault,ai_Default,uiDefaultSize);

	m4uint32_t iter;

	for (iter=0;iter<uiValueSize;iter++) {
		if (isupper(lowerValue[iter])) lowerValue[iter]= tolower(lowerValue[iter]);
		if (isupper(lowerDefault[iter])) lowerDefault[iter]= tolower(lowerDefault[iter]);
	}
 
	m4int32_t iret = strcmp(lowerValue,lowerDefault);

	free(lowerValue); free(lowerDefault);

	return iret;

#endif


	
  //## end ClConfigLogs::compareValues%933951072.body
}


// Additional Declarations
  //## begin ClConfigLogs%377C883E0283.declarations preserve=yes
  //## end ClConfigLogs%377C883E0283.declarations

//## begin module%377C88560120.epilog preserve=yes
//## end module%377C88560120.epilog

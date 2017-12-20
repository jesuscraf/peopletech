//## begin module%3A2BBB4203C0.cm preserve=no
//## end module%3A2BBB4203C0.cm

//## begin module%3A2BBB4203C0.cp preserve=no
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
//## end module%3A2BBB4203C0.cp

//## Module: m4dispatchermain%3A2BBB4203C0; Package body
//## Subsystem: M4Dispatcher::src%3923BBFB0332
//## Source file: z:\m4dispatcher\src\m4dispatchermain.cpp

//## begin module%3A2BBB4203C0.additionalIncludes preserve=no
//## end module%3A2BBB4203C0.additionalIncludes

//## begin module%3A2BBB4203C0.includes preserve=yes

#include <adminagent.hpp>
#include <clientagent.hpp>
#include <serveragent.hpp>
#include <m4condition.hpp>
#include <commadapter.hpp>
#include <executiveagent.hpp>

#include <m4log.hpp>
#include <basiclog.hpp>
#include <m4dpchres.hpp>

#include "m4appctlres.hpp"

#include <dspinvoker.hpp>

#include <m4memdbg.h>
#include <m4srvtrace.h>

#include <actionidentifiers.hpp>

#include "snmpxmlutil.hpp"
#include "snmpres.hpp"
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include "xercesc/parsers/XercesDOMParser.hpp"
//## end module%3A2BBB4203C0.includes

// snmpadmininstance
#include <snmpadmininstance.hpp>
// m4dispatchermain
#include <m4dispatchermain.hpp>
// SiteNodeInfo
#include <sitenodeinfo.hpp>
//## begin module%3A2BBB4203C0.declarations preserve=no
//## end module%3A2BBB4203C0.declarations

//## begin module%3A2BBB4203C0.additionalDeclarations preserve=yes

#define M4_SRV_TRACE_COMP_ID "DPCH"

const m4uint32_t num_workers = 10 ;

// Provisional: Config M4Log file
const m4pchar_t pszLogfile = "../conf/logsysdpch.ini";

// Log File of the Dispatcher
const m4pchar_t pszLogfileName = "logsysdpch.ini";

//Provisional
const m4pchar_t Tracefile = "dpchtrace.log";




DOMNode* GetConfigNode (const char* ai_pFile, XercesDOMParser& oParser)
{
    // Initialize the XML4C system
    XMLPlatformUtils::Initialize();
 
    // Instantiate the DOM parser.
    oParser.setValidationScheme(XercesDOMParser::Val_Never);

    // And create our error handler and install it
    SnmpErrorHandler errorHandler;
    oParser.setErrorHandler(&errorHandler);

    // parse the document.
    oParser.parse(ai_pFile);
    if (M4_TRUE == errorHandler.hasErrors())
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_XML_PARSING_ERROR, "");
        return NULL;
    }

    // get document.
    xercesc::DOMDocument *d  = oParser.getDocument();

    // get configuration node.
    DOMNode *n = (DOMNode*)SnmpDOMUtils::findNodeByAtt(d->getElementsByTagName(M4_XERCES_LITERAL("config")), M4_XERCES_LITERAL("name"), "m4dispatcher");
    if (n == NULL)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_XML_NODE_BY_ATT, "name" << "m4dispatcher");
        return NULL;
    }

    // get params node.
    DOMNode *n2 = (DOMNode*)SnmpDOMUtils::findChildNodeByName(n, "params");
    if (n2 == NULL)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_XML_NODE_BY_NAME, "params");
        return NULL;
    }

    return n2;
}

//## end module%3A2BBB4203C0.additionalDeclarations


// Class ClDpchMain 









ClDpchMain::ClDpchMain ()
  //## begin ClDpchMain::ClDpchMain%976007077.hasinit preserve=no
      : m_poStopper(NULL), m_poRepository(NULL), m_poExecutiveAgent(NULL), m_poDispatcherAgent(NULL), m_poServerAgent(NULL), m_poClientAgent(NULL), m_poCommAdapter(NULL), m_pSiteHealthVerifier(NULL)
  //## end ClDpchMain::ClDpchMain%976007077.hasinit
  //## begin ClDpchMain::ClDpchMain%976007077.initialization preserve=yes
  //## end ClDpchMain::ClDpchMain%976007077.initialization
{
  //## begin ClDpchMain::ClDpchMain%976007077.body preserve=yes
  //## end ClDpchMain::ClDpchMain%976007077.body
}


ClDpchMain::~ClDpchMain ()
{
  //## begin ClDpchMain::~ClDpchMain%976007078.body preserve=yes

	M4_MDBG_DELETE( m_poClientAgent, delete m_poClientAgent ) ;
	m_poClientAgent=0x0 ;
	
	M4_MDBG_DELETE(m_poCommAdapter, delete m_poCommAdapter);
	m_poCommAdapter = 0x0;

	M4_MDBG_DELETE(m_poDispatcherAgent, delete m_poDispatcherAgent);
	m_poDispatcherAgent = 0x0;

	M4_MDBG_DELETE(m_poExecutiveAgent, delete m_poExecutiveAgent);
	m_poExecutiveAgent = 0x0;

	M4_MDBG_DELETE(m_poRepository, delete m_poRepository);
	m_poRepository = 0x0;

	M4_MDBG_DELETE(m_poServerAgent, delete m_poServerAgent);
	m_poServerAgent = 0x0;
	
	M4_MDBG_DELETE(m_poStopper, delete m_poStopper);
	m_poStopper = 0x0;

	M4_MDBG_DELETE(m_pSiteHealthVerifier, delete m_pSiteHealthVerifier);
	m_pSiteHealthVerifier = 0x0;

  //## end ClDpchMain::~ClDpchMain%976007078.body
}



//## Other Operations (implementation)
int ClDpchMain::StartDispatcher (m4pchar_t ai_pszDpsConfigPath)
{
  //## begin ClDpchMain::StartDispatcher%975949985.body preserve=yes

    const char* msg = NULL;
	m4pcchar_t pcDumpTrace = NULL, pcServerPolling = NULL, pcPollingDelay = NULL, pcPollingTimeout = NULL, pcPollingExecutorDelay = NULL;

	m4int16_t iRet = M4_ERROR;
	m4pchar_t pszLogFilePath = 0;
	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;
	XercesDOMParser	oParser ;


	// JIB - 99173 - 100469
	m4pchar_t pszDpsConfigPath = NULL;
	if (ai_pszDpsConfigPath == NULL){
		pszDpsConfigPath = m_poDispatcherAgent->GetDspConfigPath();
	}else{
		pszDpsConfigPath = ai_pszDpsConfigPath;
	}	

	M4SrvConfigDevelopEnvByEnvVble(M4_SRV_TRACE_COMP_ID) ;
	M4_SRV_GENERAL_BLOCK_N(BlkStart,"Starting dispatcher") ;

	// Initialize the log
	pszLogFilePath= ComposeDspLogFilePath(pszDpsConfigPath);
	if (0 == pszLogFilePath)
	{
		M4_SRV_ERROR_N( 0, "Log Manager not initialized. Cannot get the Configuration File Path") ;
		pszLogFilePath = pszLogfile;
	}

	if (pszLogFilePath)
	{
		iRet = INITIALIZELOGMANAGER(pszLogFilePath);

		if (M4_ERROR == iRet)
		{
			M4_SRV_ERROR( 0, "Log Manager not initialized. Is the %0:s file OK?", pszLogfile ) ;
		}
		else 
		{
			M4_SRV_GENERAL_N( "Log Manager initilized" ) ;
		}
	}
	SETCODEF(M4_ERR_APPCTL_GENERICMSG, DEBUGINFOLOG, "Initializing Meta4 Dispatcher");

	if (0 == m_poStopper)
	{
		M4_MDBG_NEW(m_poStopper, m_poStopper = new ClCondition());
	}

	m_poStopper->Init () ;
	
	if (0 == m_poRepository)
	{
		m_poRepository = ClSiteRepositorySingleton::GetInstance() ;
	}
	
	if (0 == m_poExecutiveAgent)
	{
		M4_MDBG_NEW(m_poExecutiveAgent, m_poExecutiveAgent = new ClDspExecutiveAgent );
	}
	

	if (0 == m_poDispatcherAgent)
	{
		M4_MDBG_NEW(m_poDispatcherAgent, m_poDispatcherAgent = new ClAdminAgent( m_poRepository , m_poExecutiveAgent ));
	}

	if (0 == m_poServerAgent)
	{
		M4_MDBG_NEW(m_poServerAgent, m_poServerAgent = new ClServerAgent(m_poRepository , m_poExecutiveAgent ));
	}

	if (0 == m_poClientAgent)
	{
		M4_MDBG_NEW(m_poClientAgent, m_poClientAgent = new ClClientAgent (m_poRepository , m_poExecutiveAgent ));
	}
		
	if (0 == m_poCommAdapter)
	{
		M4_MDBG_NEW(m_poCommAdapter, m_poCommAdapter = new ClDptcherCommsAdaptor (m_poRepository , m_poExecutiveAgent ));
	}

	if (0 == m_poDispatcherAgent || M4_ERROR == m_poDispatcherAgent->Start ( ) )
	{
        msg = "Error initializing Dispatcher. Agent for Administration Management not initilized.";
        DUMP_CHLOG_ERRORF(M4_ERR_DPCH_GENERICMSG, msg);
		cout << msg << endl;

        return M4_ERROR;
	}

	if (0 == m_poServerAgent || M4_ERROR == m_poServerAgent->Start () )
	{
        msg = "Error initializing Dispatcher. Agent for Server Management not initilized.";
        DUMP_CHLOG_ERRORF(M4_ERR_DPCH_GENERICMSG, msg);
		cout << msg << endl;

		return M4_ERROR ;
	}

	if (0 == m_poClientAgent || M4_ERROR == m_poClientAgent->Start () )
	{
        msg = "Error initializing Dispatcher. Agent for Client Management not initilized.";
        DUMP_CHLOG_ERRORF(M4_ERR_DPCH_GENERICMSG, msg);
		cout << msg << endl;

		return M4_ERROR ;
	}

	if (0 == m_poCommAdapter || M4_ERROR == m_poCommAdapter->Start () )
	{
        msg = "Error initializing Dispatcher. Agent for Communication Management not initilized.";
        DUMP_CHLOG_ERRORF(M4_ERR_DPCH_GENERICMSG, msg);
		cout << msg << endl;

		return M4_ERROR ;
	}


	// Primero busco los puertos y luego arranco el commadapter
	if (M4_ERROR == m_poDispatcherAgent->InitializeAppctlRepository(pszDpsConfigPath))
	{
        msg = "No M4Appctl configurations initialized.";
        DUMP_CHLOG_DEBUGF(M4_ERR_DPCH_GENERICMSG, msg);
		cout << msg << endl;
	}
	
    // Initialize the SNMP subsystem.
    M4ISnmpAdministrator* poSnmpAdmin = NULL;
    DOMNode *n = GetConfigNode (pszDpsConfigPath, oParser);
    if (n != NULL)
    {
        // get active value.
        m4pcchar_t pcActive = SnmpDOMUtils::getAttVal(n, M4_XERCES_LITERAL("snmp_active"));
        if ((NULL != pcActive) && (!stricmp(pcActive, "true")))
        {
            // create the administrator.
            iRet = M4SnmpAdminInstance::Create(n);
            if (M4_SUCCESS != iRet)
            {
                msg = "Error initializing Dispatcher. SNMP subsystem not initilized.";
                DUMP_CHLOG_ERRORF(M4_ERR_DPCH_GENERICMSG, msg);
		        cout << msg << endl;

                return M4_ERROR;
            }

            // starts the snmp administrator.
            poSnmpAdmin = M4SnmpAdminInstance::Get();
            if (NULL != poSnmpAdmin)
            {
                poSnmpAdmin->Start();
            }
        }
        else
        {
            msg = "SNMP administration disabled.";
            DUMP_CHLOG_DEBUGF(M4_ERR_DPCH_GENERICMSG, msg);
		    cout << msg << endl;
        }
		//bg 89278
		m4pcchar_t pcBasePort = SnmpDOMUtils::getAttVal(n, M4_XERCES_LITERAL("baseport"));
		if ( pcBasePort != NULL )
		{
			cout << "Base port: " << pcBasePort << endl;
		}

		// Configuración del mecanismo de detección de servidores sordos.
		pcDumpTrace				= SnmpDOMUtils::getAttVal(n, M4_XERCES_LITERAL("dump_trace"));
		pcServerPolling			= SnmpDOMUtils::getAttVal(n, M4_XERCES_LITERAL("server_polling"));
		pcPollingDelay			= SnmpDOMUtils::getAttVal(n, M4_XERCES_LITERAL("polling_delay"));
		pcPollingTimeout		= SnmpDOMUtils::getAttVal(n, M4_XERCES_LITERAL("polling_timeout"));
		pcPollingExecutorDelay	= SnmpDOMUtils::getAttVal(n, M4_XERCES_LITERAL("polling_executor_delay"));
    }

    
    // Load appserver configurations.
    if ( M4_ERROR == m_poDispatcherAgent->InitializeTargetConfigurations ( ) )
	{
        msg = "No M4Server configurations initialized.";
        DUMP_CHLOG_DEBUGF(M4_ERR_DPCH_GENERICMSG, msg);
		cout << msg << endl;			
	}

	// eh 197951
	m4bool_t bActive = M4_FALSE;
	if( (NULL != pcServerPolling) && (!stricmp(pcServerPolling, "enabled")) )
	{
		bActive = M4_TRUE;

		msg = "Monitor for server polling enabled.";
        DUMP_CHLOG_DEBUGF( M4_ERR_DPCH_GENERICMSG, msg );
		cout << msg << endl;
	}
	else
	{
		msg = "Monitor for server polling disabled.";
        DUMP_CHLOG_DEBUGF( M4_ERR_DPCH_GENERICMSG, msg );
		cout << msg << endl;
	}

	m4bool_t bDumpPollingTrace = M4_FALSE;
	if( (NULL != pcDumpTrace) && (!stricmp(pcDumpTrace, "enabled")) )
	{
		bDumpPollingTrace = M4_TRUE;
	}

	m4millisec_t iPollingDelay = 0;
	if( (NULL != pcPollingDelay) )
	{
		iPollingDelay = atol( pcPollingDelay );
		if( iPollingDelay < 0 )
		{
			iPollingDelay = 0;
		}
		else
		{
			iPollingDelay *= 1000;
		}
	}

	m4int32_t iPollingTimeout = 0;
	if( (NULL != pcPollingTimeout) )
	{
		iPollingTimeout = atol( pcPollingTimeout );
		if( iPollingTimeout < 0 )
		{
			iPollingTimeout = 0;
		}
	}

	m4millisec_t iPollingExecutorDelay = 0;
	if( (NULL != pcPollingExecutorDelay) )
	{
		iPollingExecutorDelay = atol( pcPollingExecutorDelay );
		if( iPollingExecutorDelay < 0 )
		{
			iPollingExecutorDelay = 0;
		}
		else
		{
			iPollingExecutorDelay *= 1000;
		}
	}

	
	// Contenedor de servidores a monitorizar para mecanismo de detección de servidores sordos.
	M4_MDBG_NEW( m_pSiteHealthVerifier, m_pSiteHealthVerifier = new ClSiteHealthVerifier( bActive, iPollingDelay, bDumpPollingTrace, iPollingTimeout, iPollingExecutorDelay ));

	if( 0 == m_pSiteHealthVerifier )
	{
		msg = "Error initializing Dispatcher. Monitor for server polling not initilized.";
        DUMP_CHLOG_ERRORF( M4_ERR_DPCH_GENERICMSG, msg );
		cout << msg << endl;

		return M4_ERROR ;
	}


    BlkStart.End();

    msg = "Running ...";
    DUMP_CHLOG_DEBUGF(M4_ERR_DPCH_GENERICMSG, msg);
	cout << endl << msg << endl;


    //Si huviera en algún momento más de un worker, podría no haber procesado 
	//aún la configuración.
	if (  m_poCommAdapter->IsStarted () )
	{
		m_poStopper->Wait();
	}

    msg = "Shutdown dispatcher.";
    DUMP_CHLOG_DEBUGF(M4_ERR_DPCH_GENERICMSG, msg);
	cout << endl << msg << endl;

	m_pSiteHealthVerifier->RemoveAllServerMonitors();

	m_poCommAdapter->StopCommunicationLayer() ;

	m_poDispatcherAgent->Terminate () ;

	m_poExecutiveAgent->Terminate() ;

//	ClDispatcherInvoker::FreeInstance() ;	//Antes hay que matar todos los threads

	ClSiteRepositorySingleton::Destroy() ;

    // stops the snmp administrator.
    poSnmpAdmin = M4SnmpAdminInstance::Get();
    if (NULL != poSnmpAdmin)
    {
        poSnmpAdmin->Stop();
        M4SnmpAdminInstance::Destroy();
    }

    // destroy log.
    DESTROYLOGMANAGER();

	return M4_SUCCESS;
  //## end ClDpchMain::StartDispatcher%975949985.body
}

void ClDpchMain::StopDispatcher ()
{
  //## begin ClDpchMain::StopDispatcher%975949986.body preserve=yes

	if (0 != m_poStopper)
		m_poStopper->Signal();
	else
		M4_SRV_ERROR(M4_ERR_DPCH_NOSHUTDOWNDISPATCHER,"Error shutting down the dispatcher: %0:s.", "Cannot stop the Dispatcher.") ;

  //## end ClDpchMain::StopDispatcher%975949986.body
}

m4pchar_t ClDpchMain::ComposeDspLogFilePath (m4pchar_t ai_pszDpsConfigPath)
{
  //## begin ClDpchMain::ComposeDspLogFilePath%979212710.body preserve=yes
	m4pchar_t psCurrentLimit = 0;
	m4pchar_t psForeLimit = 0;

	m4pchar_t pszResult = 0;
	m4pchar_t pszPos = 0;


	if (ai_pszDpsConfigPath)
	{
		pszPos = strrchr(ai_pszDpsConfigPath, '/');
		if (0 == pszPos)
			pszPos = strrchr(ai_pszDpsConfigPath, '\\');
		if (pszPos)
		{
			M4_MDBG_NEW(pszResult, pszResult = new m4char_t[pszPos - ai_pszDpsConfigPath + strlen(pszLogfileName) + 2]);
			(void) strncpy(pszResult, ai_pszDpsConfigPath, strlen(ai_pszDpsConfigPath) - strlen(pszPos) + 1);
			pszResult[strlen(ai_pszDpsConfigPath) - strlen(pszPos) + 1] = '\0';
			(void) strcat(pszResult, pszLogfileName);
		}
	}
	else
	{
	#ifdef _WINDOWS

		m4char_t szPath[2048];

		if (0 !=  GetModuleFileName( NULL, szPath, 2048 ))
		{
			psCurrentLimit = szPath ;

			do
			{
				psCurrentLimit = strstr(psCurrentLimit, DISPATCHER_BIN_RELATIVE_DIR);
				
				if (0 != psCurrentLimit)
				{
					psForeLimit = psCurrentLimit;
					psCurrentLimit += strlen(DISPATCHER_BIN_RELATIVE_DIR);
				}
			} while (0 != psCurrentLimit);
			
			if (0 != psForeLimit)
			{
				M4_MDBG_NEW(pszResult, pszResult = new m4char_t[psForeLimit - szPath + strlen(TARGETAPPCTLFILE_RELATIVE_PATH) + 1]);
				(void) strncpy(pszResult, szPath, psForeLimit - szPath );
				pszResult[psForeLimit - szPath] = '\0';
				(void) strcat(pszResult, TARGETLOGFILE_RELATIVE_PATH);
				pszResult[psForeLimit - szPath + strlen(TARGETLOGFILE_RELATIVE_PATH)] = '\0';	
			}
			else
			{
				pszResult = 0;
			}
		}

	#endif
	}
	return(pszResult);

  //## end ClDpchMain::ComposeDspLogFilePath%979212710.body
}

// Additional Declarations
  //## begin ClDpchMain%3A2BB87D03D5.declarations preserve=yes
  //## end ClDpchMain%3A2BB87D03D5.declarations

// Class ClDpchMainSingleton 

//## begin ClDpchMainSingleton::m_poDpchMain%3A2BBC4402F0.role preserve=no  private: static ClDpchMain { -> RHAN}
ClDpchMain *ClDpchMainSingleton::m_poDpchMain = NULL;
//## end ClDpchMainSingleton::m_poDpchMain%3A2BBC4402F0.role

//## begin ClDpchMainSingleton::m_poInstance%3A2BC4B90338.role preserve=no  private: static ClDpchMainSingleton { -> RHAN}
ClDpchMainSingleton *ClDpchMainSingleton::m_poInstance = NULL;
//## end ClDpchMainSingleton::m_poInstance%3A2BC4B90338.role

//## begin ClDpchMainSingleton::<m_pClDpchMainSingleton>%3A2BC4B9032E.role preserve=no  private: static ClDpchMainSingleton { -> RHGA}
//## end ClDpchMainSingleton::<m_pClDpchMainSingleton>%3A2BC4B9032E.role

ClDpchMainSingleton::ClDpchMainSingleton ()
  //## begin ClDpchMainSingleton::ClDpchMainSingleton%975949987.hasinit preserve=no
  //## end ClDpchMainSingleton::ClDpchMainSingleton%975949987.hasinit
  //## begin ClDpchMainSingleton::ClDpchMainSingleton%975949987.initialization preserve=yes
  //## end ClDpchMainSingleton::ClDpchMainSingleton%975949987.initialization
{
  //## begin ClDpchMainSingleton::ClDpchMainSingleton%975949987.body preserve=yes
  //## end ClDpchMainSingleton::ClDpchMainSingleton%975949987.body
}


ClDpchMainSingleton::~ClDpchMainSingleton ()
{
  //## begin ClDpchMainSingleton::~ClDpchMainSingleton%975949988.body preserve=yes
  //## end ClDpchMainSingleton::~ClDpchMainSingleton%975949988.body
}



//## Other Operations (implementation)
ClDpchMainSingleton * ClDpchMainSingleton::Instance ()
{
  //## begin ClDpchMainSingleton::Instance%975949989.body preserve=yes

	if ( ! m_poInstance )
	{
		M4_MDBG_NEW( m_poInstance, m_poInstance = new ClDpchMainSingleton ) ;
		M4_MDBG_NEW( m_poInstance->m_poDpchMain, m_poInstance->m_poDpchMain = new ClDpchMain) ;
	}

	return m_poInstance ;

  //## end ClDpchMainSingleton::Instance%975949989.body
}

ClDpchMainSingleton * ClDpchMainSingleton::FreeInstance ()
{
  //## begin ClDpchMainSingleton::FreeInstance%975949990.body preserve=yes
	
	M4_MDBG_DELETE( m_poInstance, delete m_poInstance ) ;
	m_poInstance=NULL ;

	return m_poInstance ;


  //## end ClDpchMainSingleton::FreeInstance%975949990.body
}

ClDpchMain * ClDpchMainSingleton::GetDpchMain ()
{
  //## begin ClDpchMainSingleton::GetDpchMain%976007079.body preserve=yes
	
	return m_poDpchMain;

  //## end ClDpchMainSingleton::GetDpchMain%976007079.body
}

// Additional Declarations
  //## begin ClDpchMainSingleton%3A2BBBB002C4.declarations preserve=yes
  //## end ClDpchMainSingleton%3A2BBBB002C4.declarations

//## begin module%3A2BBB4203C0.epilog preserve=yes
//## end module%3A2BBB4203C0.epilog

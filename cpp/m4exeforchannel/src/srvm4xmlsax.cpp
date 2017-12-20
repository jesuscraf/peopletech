//## begin module%37E8E4F602A4.cm preserve=no
//## end module%37E8E4F602A4.cm

//## begin module%37E8E4F602A4.cp preserve=no
//	=====================================================================
//
//	 (c) Copyright 2004 Meta4 Spain S.A.
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              <file>.cpp
//	 Project:           <project>
//	 Author:            Meta4 Spain S.A.
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS, UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=====================================================================
//## end module%37E8E4F602A4.cpp

//## Module: Executive_M4XML_SAX%37E8E4F602A4; Package body
//## Subsystem: M4ExeForChannel::XML::src%3826C5A30000
//## Source file: C:\m4server-xerces2.5.0\m4exeforchannel\src\srvm4xmlsax.cpp

//## begin module%37E8E4F602A4.additionalIncludes preserve=no
//## end module%37E8E4F602A4.additionalIncludes

//## begin module%37E8E4F602A4.includes preserve=yes
#include "m4stl.hpp"
#include "m4srvefcres.hpp"
#include "clm4objservice.hpp"
#include "dm.hpp"
#include "access.hpp"
#include "chan_knl.hpp"
#include "executor.hpp"
#include "cldbgutil.hpp"
#include "cllgadap.hpp"
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/sax/AttributeList.hpp>
#include <clexecutorstatemanager.hpp>
#include <m4xmlutil.hpp>
#include <srvm4xmlsax.hpp>
#include <serverm4xml.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include "m4unicode.hpp"
#include "m4cryptc.hpp"
#include "m4regimpl.hpp"
#include "m4logfile.hpp"
#include "execlogs.hpp"
#include "mdupdate.hpp"

//## begin module%37E8E4F602A4.declarations preserve=no
//## end module%37E8E4F602A4.declarations

//## begin module%37E8E4F602A4.additionalDeclarations preserve=yes
// log entries
#define M4XML_LOG_MAX_TYPE 64
#define M4XML_LOG_MAX_CODE 64
#define M4XML_LOG_MAX_TEXT 2048
#define M4XML_LOG_MAX_TIME 64
#define MAX_XML_SESSION_ID 1024
#define ATTRIB_XML_LOG_INFO "xmlloginfo"

class M4TimerDump
{
private:
	m4uint32_t uStartTime;
	string message;

public:
	M4TimerDump(string message)
	{
		this->message = message;
		uStartTime = m4uint32_t(ClPrecisionTicker::GetTimeTick());
	}

	~M4TimerDump()
	{
		m4uint32_t uCurrentTime = m4uint32_t(ClPrecisionTicker::GetTimeTick());
		m4uint32_t uDelta = uCurrentTime - uStartTime;

		if (uDelta >= 10) {
			printf("%s: Delta time = %u\n", message.c_str(), uDelta);
		}
	}
};


#define	M4CH_DUMMY_B4	"\x3\x7\xB\x4\xB\x9\x8\x6\xC\xD\x5\xA\xA\xE\xA\xB\xA\x1\xB\x1\x8\xD\xB\x4\x1\x5\x8\x8\x5\x5\x8\xC\x2\x4\x5\x2\x4\xF\x7\x2\x6\x4\x2\x6\x3\xC\xA\xC\x2\xF\x4\x6\x2\x6\x4\x2\x5\x7\x9\xA\xC\x4\x5\x0\xC\xF\x6\xF\xA\xF\xF\x2\x2\xD\x5\x7\x7\xC\x1\xC\x1\xA\xA\xF\xF\xA\x8\x3"


static void dumpXML(m4pcchar_t ai_pccType, m4pcchar_t ai_pcName, m4pcchar_t ai_pcInfo, m4pcchar_t ai_pcOrg, m4pcchar_t ai_pcRole, ClM4XMLContext* ai_pContext)
{
	M4ThreadLogger*	pThreadLogger = NULL;
	m4int32_t		iReqNum;

	if( ClVMDebugEnv::getInstance()->getDebugLevel() < 1 )
	{
		return ;
	}

	// m4xml.log trace.
	pThreadLogger = M4ThreadLogger::GetInstance( XML_LOG );

	if( pThreadLogger != NULL )
	{
		if( ai_pContext != NULL )
		{
			// A single XML request can produce several requests, so here it is introduced a counter to distinguish them.
			iReqNum = ai_pContext->GetReqNumber();

			pThreadLogger->SetReqNumber( iReqNum );
			ai_pContext->SetReqNumber( iReqNum + 1 );
		}
		else
		{
			pThreadLogger->SetReqNumber( 0 );
		}

		pThreadLogger->InitLogFile( "m4xml", 5 * 1024 * 1024, "TickCount\tThreadId\tType\tName\tDetail\tOrganization\tRole\tSessionKey\tServerHost\tServerPort\tSessionId\tUserClick\tReqUniqueID", "log", M4_TRUE, M4_FALSE, M4_TRUE );
		pThreadLogger->WriteLine( "\t%s\t%s\t%s\t%s\t%s", ai_pccType, ai_pcName, ai_pcInfo, ai_pcOrg, ai_pcRole );
	}
}

// computes encrytion key
const m4int32_t MAX_KEY_LEN	= 32;
static string M4XMLGetEncryptKey(string &sKey) 
{
	m4pchar_t pcKey = new m4char_t[MAX_KEY_LEN + 1];
	strncpy(pcKey, M4CH_DUMMY_B4 + 7, MAX_KEY_LEN);
	pcKey[MAX_KEY_LEN] = '\0';

	// use input key
	int sizeToCopy = sKey.size();
	if (sizeToCopy > MAX_KEY_LEN)
	{
		sizeToCopy = MAX_KEY_LEN;

	}
	strncpy(pcKey, sKey.c_str(), sizeToCopy);
	return string(pcKey);
}

// encrypts a value in two way mode
m4return_t M4XMLEncryptTwoWay(string &sValue, string &sKey) 
{
	m4return_t	iResult = M4_ERROR ;
	m4int_t		iOut = 0 ;
	m4int_t		iLength = 0 ;
	m4pchar_t	pcValue = NULL ;

	const string sEncryptKey = M4XMLGetEncryptKey( sKey );

	// decrypts value
	iResult = EncryptTwoWay(sValue.c_str(), sValue.size(), sEncryptKey.c_str(), sEncryptKey.size(), pcValue, iOut);

	if( iResult == M4_SUCCESS )
	{
		sValue = pcValue ;
	}
	else
	{
		sValue = "";
		DUMP_CHLOG_DEBUG ( M4_ERR_XML_DECRYPT );
		return iResult;
	}

	delete [] pcValue ;
	return M4_SUCCESS;
}

// decrypts a value in two way mode
m4return_t M4XMLDecryptTwoWay(string &sValue, string &sKey) 
{
	m4return_t	iResult = M4_ERROR ;
	m4int_t		iOut = 0 ;
	m4int_t		iLength = 0 ;
	m4pchar_t	pcValue = NULL ;

	const string sEncryptKey = M4XMLGetEncryptKey( sKey );

	// decrypts value
	iResult = DecryptTwoWay(sValue.c_str(), sValue.size(), sEncryptKey.c_str(), sEncryptKey.size(), pcValue, iOut);

	if( iResult == M4_SUCCESS )
	{
		sValue = pcValue ;
	}
	else
	{
		sValue = "";
		DUMP_CHLOG_DEBUG ( M4_ERR_XML_DECRYPT );
		return iResult;
	}

	delete [] pcValue ;
	return M4_SUCCESS;
}

// decrypts a value in two way mode
void M4XMLDecryptTwoWay(string &sValue) 
{
	m4return_t	iResult = M4_ERROR ;
	m4int_t		iOut = 0 ;
	m4int_t		iLength = 0 ;
	m4pchar_t	pcValue = NULL ;

	// decrypts value
	iResult = DecryptTwoWay(sValue.c_str(), sValue.size(), M4CH_DUMMY_B4 + 23, strlen(M4CH_DUMMY_B4 + 23), pcValue, iOut);

	if( iResult == M4_SUCCESS )
	{
		sValue = pcValue ;
	}
	else
	{
		DUMP_CHLOG_DEBUG ( M4_ERR_XML_DECRYPT );
	}

	delete [] pcValue ;
}

// encryptes a value in two way mode
void M4XMLEncryptTwoWay(string &sValue)
{
	m4return_t	iResult = M4_ERROR;
	m4int_t		iOut = 0;
	m4int_t		iLength = 0;
	m4pchar_t	pcValue = NULL;

	// decrypts value
	iResult = EncryptTwoWay(sValue.c_str(), sValue.size(), M4CH_DUMMY_B4 + 23, strlen(M4CH_DUMMY_B4 + 23), pcValue, iOut);

	if (iResult == M4_SUCCESS)
	{
		sValue = pcValue;
	}
	else
	{
		DUMP_CHLOG_DEBUG(M4_ERR_XML_ENCRYPT);
	}

	delete[] pcValue;
}

//## end module%37E8E4F602A4.additionalDeclarations


// Class M4XMLSAXHandlers 



M4XMLSAXHandlers::M4XMLSAXHandlers (ClCMInitParameters &aio_oClCMInitParameters, ClM4XMLStream &ai_oStream, ClM4XMLInfo* ai_poExecInfo)
  //## begin M4XMLSAXHandlers::M4XMLSAXHandlers%938011536.hasinit preserve=no
      : m_bIsOk(M4_FALSE), m_oOut(ai_oStream), m_poContext(NULL), m_iError(M4_SUCCESS), 
	  m_oCMInitParameters(aio_oClCMInitParameters), m_poExecInfo(ai_poExecInfo), m_eParsingPhase(PARSING_PHASE_PRE),
	  m_bHaveFunctionalGroup(M4_FALSE)
  //## end M4XMLSAXHandlers::M4XMLSAXHandlers%938011536.hasinit
  //## begin M4XMLSAXHandlers::M4XMLSAXHandlers%938011536.initialization preserve=yes
  //## end M4XMLSAXHandlers::M4XMLSAXHandlers%938011536.initialization
{
  //## begin M4XMLSAXHandlers::M4XMLSAXHandlers%938011536.body preserve=yes

	// initializes internal map
	InitHandlers();

	m_bIsOk = M4_TRUE;

#if _M4_DEBUG > 10
	// Archivos relacionados: "srvm4xmlsax.hpp", "srvm4xmlsax.cpp", "exem4xmlsax.cpp"
	bEndContextStart = M4_FALSE;
	bEndContextEnd = M4_FALSE;
#endif

  //## end M4XMLSAXHandlers::M4XMLSAXHandlers%938011536.body
}


M4XMLSAXHandlers::~M4XMLSAXHandlers ()
{
  //## begin M4XMLSAXHandlers::~M4XMLSAXHandlers%938011537.body preserve=yes

	Reset();

	m_bIsOk = M4_FALSE;

  //## end M4XMLSAXHandlers::~M4XMLSAXHandlers%938011537.body
}

m4void_t M4XMLSAXHandlers::InitHandlers ()
{
  //## begin M4XMLSAXHandlers::InitHandlers%938011537.body preserve=yes

	// clears the array
	m_oElementStart.clear();
	m_oElementEnd.clear();

	switch(m_eParsingPhase)
	{
	
	case PARSING_PHASE_PRE:

		// reads security context
		m_oElementStart.insert(ElementHandlerMap::value_type("context",&M4XMLSAXHandlers::procContextPreProcess));
		break;

	case PARSING_PHASE_1:

		// initializes internal map start
		m_oElementStart.insert(ElementHandlerMap::value_type("context",&M4XMLSAXHandlers::procContext));
		m_oElementStart.insert(ElementHandlerMap::value_type("m4oalias",&M4XMLSAXHandlers::procM4OAlias));
		m_oElementStart.insert(ElementHandlerMap::value_type("m4oparam",&M4XMLSAXHandlers::procM4OParam));
		m_oElementStart.insert(ElementHandlerMap::value_type("m4oref",&M4XMLSAXHandlers::procM4ORef));

		// initializes internal map end
		m_oElementEnd.insert(ElementHandlerMap::value_type("m4oalias",&M4XMLSAXHandlers::procEndM4OAlias));

		// adds specific elements for security
		if (m_bHaveFunctionalGroup == M4_TRUE)
		{
			// start elements
			m_oElementStart.insert(ElementHandlerMap::value_type("method",&M4XMLSAXHandlers::procMethodPreProcess));
			m_oElementStart.insert(ElementHandlerMap::value_type("dumplog",&M4XMLSAXHandlers::procLog));

			// end elements
			m_oElementEnd.insert(ElementHandlerMap::value_type("context",&M4XMLSAXHandlers::procEndContextPreProcess));
		}

		break;


	case PARSING_PHASE_2:

		// initializes internal map start
		m_oElementStart.insert(ElementHandlerMap::value_type("commandset",&M4XMLSAXHandlers::procCommandSet));
		m_oElementStart.insert(ElementHandlerMap::value_type("method",&M4XMLSAXHandlers::procMethod));
		m_oElementStart.insert(ElementHandlerMap::value_type("arg",&M4XMLSAXHandlers::procArg));
		m_oElementStart.insert(ElementHandlerMap::value_type("data",&M4XMLSAXHandlers::procData));
		m_oElementStart.insert(ElementHandlerMap::value_type("datadef",&M4XMLSAXHandlers::procDataDef));
		m_oElementStart.insert(ElementHandlerMap::value_type("node",&M4XMLSAXHandlers::procNode));
		m_oElementStart.insert(ElementHandlerMap::value_type("nodedef",&M4XMLSAXHandlers::procNodeDef));
		m_oElementStart.insert(ElementHandlerMap::value_type("register",&M4XMLSAXHandlers::procRegister));
		m_oElementStart.insert(ElementHandlerMap::value_type("registerdef",&M4XMLSAXHandlers::procRegisterDef));
		m_oElementStart.insert(ElementHandlerMap::value_type("item",&M4XMLSAXHandlers::procItem));
		m_oElementStart.insert(ElementHandlerMap::value_type("itemdef",&M4XMLSAXHandlers::procItemDef));
		m_oElementStart.insert(ElementHandlerMap::value_type("filter",&M4XMLSAXHandlers::procFilter));
		m_oElementStart.insert(ElementHandlerMap::value_type("filterarg",&M4XMLSAXHandlers::procFilterArg));
		m_oElementStart.insert(ElementHandlerMap::value_type("transltype",&M4XMLSAXHandlers::procTranslType));
		m_oElementStart.insert(ElementHandlerMap::value_type("translitem",&M4XMLSAXHandlers::procTranslItem));
		m_oElementStart.insert(ElementHandlerMap::value_type("translitemarg",&M4XMLSAXHandlers::procTranslItemArg));
		m_oElementStart.insert(ElementHandlerMap::value_type("move",&M4XMLSAXHandlers::procMove));
		m_oElementStart.insert(ElementHandlerMap::value_type("movedef",&M4XMLSAXHandlers::procMoveDef));
		m_oElementStart.insert(ElementHandlerMap::value_type("dumplog",&M4XMLSAXHandlers::procLog));
		m_oElementStart.insert(ElementHandlerMap::value_type("gencommands",&M4XMLSAXHandlers::procGenCommands));
		m_oElementStart.insert(ElementHandlerMap::value_type("command",&M4XMLSAXHandlers::procCommand));
		m_oElementStart.insert(ElementHandlerMap::value_type("commarg",&M4XMLSAXHandlers::procCommArg));

		// initializes internal map end
		m_oElementEnd.insert(ElementHandlerMap::value_type("context",&M4XMLSAXHandlers::procEndContext));
		m_oElementEnd.insert(ElementHandlerMap::value_type("commandset",&M4XMLSAXHandlers::procEndCommandSet));
		m_oElementEnd.insert(ElementHandlerMap::value_type("method",&M4XMLSAXHandlers::procEndMethod));
		m_oElementEnd.insert(ElementHandlerMap::value_type("datadef",&M4XMLSAXHandlers::procEndDataDef));
		m_oElementEnd.insert(ElementHandlerMap::value_type("filter",&M4XMLSAXHandlers::procEndFilter));
		m_oElementEnd.insert(ElementHandlerMap::value_type("translitem",&M4XMLSAXHandlers::procEndTranslItem));
		m_oElementEnd.insert(ElementHandlerMap::value_type("nodedef",&M4XMLSAXHandlers::procEndNodeDef));
		m_oElementEnd.insert(ElementHandlerMap::value_type("gencommands",&M4XMLSAXHandlers::procEndGenCommands));	
		m_oElementEnd.insert(ElementHandlerMap::value_type("command",&M4XMLSAXHandlers::procEndCommand));

		break;

	}

  //## end M4XMLSAXHandlers::InitHandlers%938011537.body
}


//## Other Operations (implementation)
m4return_t M4XMLSAXHandlers::Reset ()
{
  //## begin M4XMLSAXHandlers::Reset%942060102.body preserve=yes
	m4return_t iResult = M4_SUCCESS;

	if (m_poContext != NULL)
	{
		m_oMethod.Reset();
		
		m_oItem.Reset();

		// proccess pending alias commands
		if (m_poContext->ProcAliasCommands() != M4_SUCCESS)
		{
			SetError();
		}
		
		// frees only logical connection
		if (m_poContext->ReleaseLogicalConnection() != M4_SUCCESS)
		{
			iResult = M4_ERROR;
		}


		// User Actions Auditory.
		// Sincronización del valor de la fecha de logon, desde el canal sesión server a cada subsesion.
		while( m_poContext != NULL )
		{
			m4VariantType	vVariant ;
			ClChannel		*pChannel = NULL ;
			ClAccess		*pAccess = NULL ;
			ClNode			*pNode = NULL ;
			m4return_t		iRet ;

			// blocks for all executors
			ClMutBlock oExecutiveMutex(ClExecutiveForOE::GetExecuteMutex());
			
			// Recuperamos la fecha de logon del canal sesión server.
			vVariant.Data.DoubleData = m_poContext->GetObjectService()->GetpLogonAdaptor()->GetUserActionsAuditSessionDate() ;

			// bg 280829
			pChannel = m_poContext->GetBaseSessionChannel() ;

			if( pChannel == NULL )
			{
				iResult = M4_ERROR ;
				break;
			}
			
			pChannel->CreateAccess( pAccess ) ;
			if( pAccess == NULL )
			{
				iResult = M4_ERROR ;
				break ;
			}

			pNode = &( pAccess->Node[ "ROOT_SESSION" ] ) ;
			if( pNode == NULL )
			{
				iResult = M4_ERROR ;
				pAccess->Destroy() ;
				break ;
			}

			// Asignamos la fecha de logon a la subsesión.
			vVariant.Type = M4CL_CPP_TYPE_DATE ;
			iRet = pNode->RecordSet.Register.Item[ "USR_AUDIT_SESSION_DATE" ].Value.Set( vVariant ) ;

			pAccess->Destroy() ;

			if( iRet != M4_SUCCESS )
			{
				iResult = M4_ERROR ;
			}

			break ;
		}


		if (PersistState() != M4_SUCCESS)
		{
			SetError();
		}
		
		m_poContext = NULL;
	}

	return iResult;

  //## end M4XMLSAXHandlers::Reset%942060102.body
}

void M4XMLSAXHandlers::DumpAttrib (AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::DumpAttrib%938073888.body preserve=yes
	if (attributes == NULL)
	{
		return;
	}
	m4int32_t iLen = attributes->getLength();
	for (m4int32_t i=0; i<iLen; i++)
	{
		string sAttrName;
		M4XMLToSTL((M4XMLCh*)attributes->getName(i), sAttrName);
		if (sAttrName == ATTRIB_XML_LOG_INFO)
		{
			continue;
		}

		m_oOut << " " << sAttrName.c_str() << "=\"" << attributes->getValue(i) << "\"";
	}
  //## end M4XMLSAXHandlers::DumpAttrib%938073888.body
}

void M4XMLSAXHandlers::endDocument ()
{
  //## begin M4XMLSAXHandlers::endDocument%938011540.body preserve=yes
	if (m_eParsingPhase == PARSING_PHASE_2 
		|| (m_bHaveFunctionalGroup == M4_TRUE && m_eParsingPhase == PARSING_PHASE_1 && IsOk() == M4_FALSE))
	{
 		m_poExecInfo->SetFileInfo("endDocument");

		// ends parsing
		m_oCMInitParameters.SetParseStatus(M4_FALSE);

		// writes the end of the document
		m_oOut << "</m4xml>" << endl;
	}
  //## end M4XMLSAXHandlers::endDocument%938011540.body
}

void M4XMLSAXHandlers::endElement (const XMLCh * const name)
{
  //## begin M4XMLSAXHandlers::endElement%938011541.body preserve=yes
	// checks data integrity

	// UNICODE XML
	string sElement;
	M4XMLToSTL( (M4XMLCh*)name, sElement ) ;

	m_poExecInfo->SetFileInfo(string("end" + sElement).c_str());
	m4bool_t bEnd = M4_FALSE;

	// checks the context
	if (m_poContext != NULL && ( 
		m_poContext->GetObjectService() == NULL ||
		m_poContext->GetObjectService()->GetpExecutor() == NULL))
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_SAX_INIT_INTERNAL );
		SetError(M4_ERROR);
		return;
	}

	// checks abort status
	if (m_poContext != NULL &&
		m_poContext->GetObjectService()->GetpExecutor()->GetRequestToAbortWithClStatus() == M4_TRUE)
	{
		DUMP_CHLOG_DEBUG ( M4_ERR_XML_REQ_ABORT );
		SetError(M4CL_USER_ABORT_ALL_REQUEST);
		bEnd = M4_TRUE;
	}

	ElementHandlerIt it = m_oElementEnd.find(sElement);
	if (it == m_oElementEnd.end())
	{
		return;
	}

	if (m_poContext == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_SAX_INIT_INTERNAL );
		SetError(M4_ERROR);
		return;
	}

	// executes the element
	ElementHandler pHandler = (*it).second;
	if ((bEnd == M4_TRUE  && pHandler == &M4XMLSAXHandlers::procEndContext) 
		|| bEnd == M4_FALSE)
	{

#if _M4_DEBUG > 10
			M4TimerDump timer(sElement.c_str());
#endif
			(this->*pHandler)(name,NULL);
	}
  //## end M4XMLSAXHandlers::endElement%938011541.body
}

void M4XMLSAXHandlers::startDocument ()
{
  //## begin M4XMLSAXHandlers::startDocument%938011542.body preserve=yes

	if (m_eParsingPhase == PARSING_PHASE_1)
	{
		// writes the header
		// UNICODE XML
		m_oOut << M4XMLHeader();
		m_oOut << "<m4xml version=\"1.0\">" << endl;
		m_poExecInfo->SetFileInfo("startDocument");
	}
  //## end M4XMLSAXHandlers::startDocument%938011542.body
}

void M4XMLSAXHandlers::startElement (const XMLCh* const name, AttributeList& attributes)
{
  //## begin M4XMLSAXHandlers::startElement%938011543.body preserve=yes

	// UNICODE XML
	string sElement;
	M4XMLToSTL( (M4XMLCh*)name, sElement ) ;

	m_poExecInfo->SetFileInfo(string("start" + sElement).c_str());

	// checks the context
	if (m_poContext != NULL && ( 
		m_poContext->GetObjectService() == NULL ||
		m_poContext->GetObjectService()->GetpExecutor() == NULL))
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_SAX_INIT_INTERNAL );
		SetError(M4_ERROR);
		return;
	}

	// checks abort status
	if (m_poContext != NULL &&
		m_poContext->GetObjectService()->GetpExecutor()->GetRequestToAbortWithClStatus() == M4_TRUE)
	{
		DUMP_CHLOG_DEBUG ( M4_ERR_XML_REQ_ABORT );
		SetError(M4CL_USER_ABORT_ALL_REQUEST);
		return;
	}

	ElementHandlerIt it = m_oElementStart.find(sElement);
	if (it == m_oElementStart.end())
		return;

	// checks data integrity
	if (m_eParsingPhase != PARSING_PHASE_PRE && (*it).second != &M4XMLSAXHandlers::procContext && m_poContext == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_SAX_INIT );
		SetError(M4_ERROR);
		return;
	}

	(this->*((*it).second))(name,&attributes);
  //## end M4XMLSAXHandlers::startElement%938011543.body
}

void M4XMLSAXHandlers::procContextPreProcess (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procContextPreProcess%938011545.body preserve=yes
	string sUseFunctionalGroup;

	M4XMLToSTL( (M4XMLCh*)attributes->getValue("usefunctionalgroup"), sUseFunctionalGroup ) ;

	if (sUseFunctionalGroup == "true")
	{
		m_bHaveFunctionalGroup = M4_TRUE;
	}
  //## end M4XMLSAXHandlers::procContextPreProcess%938011545.body
}
	
void M4XMLSAXHandlers::procContext (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procContext%938011545.body preserve=yes
	// reset members
	Reset();

	string sUserId = GetUserId();

	// processes the context
	m_oOut << "<" << name;
	DumpAttrib(attributes);
	m_oOut << " serveruserid=\"" << sUserId.c_str() << "\"";

	// UNICODE XML
	string sSubSessionId;
	string sDebugInfo;
	string sPersistLevel;
	string sAuthPassword;
	string sEnableNulls;
	string sEnableAuth;
	string sDataMode;
	string sEnableBlobFiles;
	string sXMLLogInfo;
	string sRefUserId;
	string sGetRepositoryKey;

	M4XMLToSTL( (M4XMLCh*)attributes->getValue("subsessionid"), sSubSessionId ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("debuginfo"), sDebugInfo ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("persistlevel"), sPersistLevel ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("authpassword"), sAuthPassword ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("enablenulls"), sEnableNulls ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("enableauth"), sEnableAuth ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("datamode"), sDataMode ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("enableblobfiles"), sEnableBlobFiles ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue(ATTRIB_XML_LOG_INFO), sXMLLogInfo ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("userid"), sRefUserId ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("getrepositorykey"), sGetRepositoryKey);

	// checks session consistency
	if (sUserId.size() != 0 && sRefUserId.size() != 0 && sUserId != sRefUserId)
	{
		// just closes the context and returns
		m_oOut << ">" << endl;
		m_oOut << "</" << name << ">" << endl;
		return;
	}

	m_oCMInitParameters.SetPersistLevel(sPersistLevel);

	// computes the session id
	char pcSessionId [sizeof(m4uint64_t)*2+2];
	sprintf(pcSessionId, "%lu", m_oCMInitParameters.GetSessionId());
	string sSessionId(pcSessionId);

	// reset init parameters
	string sComposedSubSessionId = sSessionId + "-" + sSubSessionId;
	m_oCMInitParameters.SetSubSessionId(sComposedSubSessionId);

	if (attributes->getValue("endsubsessionid") != NULL)
	{
		string sEndSubSessionId;
		M4XMLToSTL( (M4XMLCh*)attributes->getValue("endsubsessionid"), sEndSubSessionId ) ;

		string sComposedEndSubSessionId = sSessionId + "-" + sEndSubSessionId;
		m_oCMInitParameters.SetEndSubSessionId(sComposedEndSubSessionId);
	}
	else 
	{
		m_oCMInitParameters.SetEndSubSessionId(sComposedSubSessionId);
	}

	// efv Dump Info
	dumpXML("Context", sSubSessionId.c_str(), sDebugInfo.c_str(), sXMLLogInfo.c_str(), m_oCMInitParameters.GetEndSubSessionId().c_str(), m_poContext);

	string sInfo = "" ;

	if (m_poExecInfo != NULL)
	{
		// UNICODE XML
		M4XMLToSTL((M4XMLCh*) attributes->getValue("info"), sInfo ) ;

		string sExecInfo;
		sExecInfo.reserve(sSubSessionId.size() + sInfo.size() + 2);
		sExecInfo = sSubSessionId + ":";
		sExecInfo += sInfo;
		m_poExecInfo->SetInfo(sExecInfo.c_str());
		m_poExecInfo->SetDebugInfo(sDebugInfo.c_str());
	}

	// UNICODE XML
	string sSubSessionMode;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("subsessionmode"), sSubSessionMode ) ;

	m_oCMInitParameters.SetSubSessionMode(sSubSessionMode);

	// inits the state
	if (LoadState() != M4_SUCCESS)
	{
		SetError();
		return;
	}

	// loads the repository key
	if (sGetRepositoryKey == "true")
	{
		m4string_t sRepositoryKey = GetRepositoryKey();
		if (sRepositoryKey.size() != 0)
		{
			m_oOut << " repositorykey=\"";
			m_oOut.Transl(sRepositoryKey.c_str());
			m_oOut << "\"";
		}
	}

	// closes the element
	m_oOut << ">" << endl;

	// User Actions Auditory.
	// Sincronización del valor de la fecha de logon, desde cada subsesion al canal sesión server.
	if( m_poContext != NULL )
	{

		// sets members
		m_poContext->ResetM4OReferences();
		m_poContext->SetEnableMethods(M4_TRUE);
		m_poContext->SetAuthPassword(sAuthPassword);
		m_poContext->SetEnableNulls(sEnableNulls == "true");
		m_poContext->SetEnableAuth(sEnableAuth == "true");
		m_poContext->SetDataMode(sDataMode);
		m_poContext->SetHaveFunctionalGroup(m_bHaveFunctionalGroup);
		m_poContext->SetEnableBlobFiles(sEnableBlobFiles != "false");

		while( true )
		{
			ClChannel			*pChannel = NULL ;
			ClAccess			*pAccess = NULL ;
			ClNode				*pNode = NULL ;
			m4VariantType		vVariant ;
			m4return_t			iRet ;

			// blocks for all executors
			ClMutBlock oExecutiveMutex(ClExecutiveForOE::GetExecuteMutex());

			// bg 280829
			pChannel = m_poContext->GetBaseSessionChannel() ;

			if( pChannel == NULL )
			{
				break;
			}

			pChannel->CreateAccess ( pAccess ) ;
			if( pAccess == NULL )
			{
				break ;
			}

			pNode = &( pAccess->Node[ "ROOT_SESSION" ] ) ;
			if( pNode == NULL )
			{
				pAccess->Destroy() ;
				break ;
			}

			// Recuperamos la fecha de logon de la subsesión.
			iRet = pNode->RecordSet.Register.Item[ "USR_AUDIT_SESSION_DATE" ].Value.Get( vVariant ) ;

			pAccess->Destroy() ;

			if( iRet != M4_SUCCESS )
			{
				break ;
			}


			pChannel = m_poContext->GetObjectService()->GetpLogonAdaptor()->GetSessionChannelPointer() ;

			if( pChannel == NULL )
			{
				break;
			}

			pChannel->CreateAccess ( pAccess ) ;
			if( pAccess == NULL )
			{
				break ;
			}

			pNode = &( pAccess->Node[ "ROOT_SESSION" ] ) ;
			if( pNode == NULL )
			{
				pAccess->Destroy() ;
				break ;
			}

			// Asignamos la fecha de logon al canal sesión server.
			iRet = pNode->RecordSet.Register.Item[ "USR_AUDIT_SESSION_DATE" ].Value.Set( vVariant ) ;

			pAccess->Destroy() ;

			break ;
		}

		ClChannelManager* pChannelManager = m_poContext->GetObjectService()->GetpChannelManager();

		// Auditory activation.
		pChannelManager->UserActionsAudit.SwitchOn();

		if( sInfo != "" )
		{
			pChannelManager->UserActionsAudit.AddAction( NULL, NULL, ClUserAction::EV_WEB_PAGE, sInfo.c_str(), NULL, NULL, M4_FALSE ) ;
		}

		if( sDebugInfo != "" && sDebugInfo != sInfo )
		{
			pChannelManager->UserActionsAudit.AddAction( NULL, NULL, ClUserAction::EV_WEB_MENU, sDebugInfo.c_str(), NULL, NULL, M4_FALSE ) ;
		}

		m_poContext->SetReqNumber( 1 );
		
	}


  //## end M4XMLSAXHandlers::procContext%938011545.body
}

string M4XMLSAXHandlers::GetUserId() {

	ClChannel			*pChannel = NULL ;
	ClAccess			*pAccess = NULL ;
	ClNode				*pNode = NULL ;
	m4VariantType		vUserId ;
	m4return_t			iRet ;

	// blocks for all executors
	ClMutBlock oExecutiveMutex(ClExecutiveForOE::GetExecuteMutex());

	pChannel = m_oCMInitParameters.GetSessionChannel();

	if( pChannel == NULL )
	{
		return "";
	}

	pChannel->CreateAccess ( pAccess ) ;
	if( pAccess == NULL )
	{
		return "" ;
	}

	pNode = &( pAccess->Node[ "ROOT_SESSION" ] ) ;
	if( pNode == NULL )
	{
		pAccess->Destroy() ;
		return "";
	}

	// lets retrieve users
	iRet = pNode->RecordSet.Register.Item[ "ID_DEBUG_USER" ].Value.Get( vUserId ) ;
	if( iRet != M4_SUCCESS )
	{
		pAccess->Destroy() ;
		return "" ;
	}
	if (vUserId.Data.PointerChar == NULL || strlen(vUserId.Data.PointerChar) == 0)
	{
		iRet = pNode->RecordSet.Register.Item[ "ID_APP_USER" ].Value.Get( vUserId ) ;
		if( iRet != M4_SUCCESS )
		{
			pAccess->Destroy() ;
			return "" ;
		}
	}

	pAccess->Destroy() ;

	if(vUserId.Data.PointerChar != NULL)
	{
		return vUserId.Data.PointerChar;
	}

	return "";
}

void M4XMLSAXHandlers::procM4OAlias (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procM4OAlias%938073889.body preserve=yes
	// UNICODE XML
	string sAlias;
	string sM4Oid;
	string sMode;
	string sPreserve;
	string sRemove;
	string sFind;
	string sMdInfoLevel;
	string sUseFunctionalGroup;

	M4XMLToSTL( (M4XMLCh*)attributes->getValue("alias"), sAlias ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("m4oid"), sM4Oid ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("mode"), sMode ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("preserve"), sPreserve ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("remove"), sRemove ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("find"), sFind ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("mdinfolevel"), sMdInfoLevel ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("usefunctionalgroup"), sUseFunctionalGroup ) ;

	if (m_poContext->ProcAlias(sAlias, sM4Oid, sMode, sPreserve, sRemove, sFind, sMdInfoLevel, sUseFunctionalGroup) != M4_SUCCESS)
	{
		SetError();
	}
  //## end M4XMLSAXHandlers::procM4OAlias%938073889.body
}

void M4XMLSAXHandlers::procCommandSet (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procCommandSet%938708970.body preserve=yes
	m_oOut << "<dataset";
	DumpAttrib(attributes);
	m_oOut << ">" << endl;
  //## end M4XMLSAXHandlers::procCommandSet%938708970.body
}

void M4XMLSAXHandlers::procMethodPreProcess (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procMethodPreProcess%938096977.body preserve=yes

	// Inits the method
	// UNICODE XML
	string sItem;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("item"), sItem ) ;

	// efv Dump Info
	dumpXML("MethodPreProcess",sItem.c_str(),"", "", "", m_poContext);

	if (m_oMethod.Reset(m_poContext, sItem) != M4_SUCCESS)
	{
		SetError(M4_ERROR);
		return;
	}

	if (m_oMethod.GetNArgs() < 0)
	{
		// UNICODE XML
		string sName;
		M4XMLToSTL( (M4XMLCh*)name, sName ) ;

		DUMP_CHLOG_ERRORF ( M4_ERR_XML_PROC_METHOD_VALID, sItem.c_str()	<< sName.c_str());
		SetError(M4_ERROR);
		return;
	}

	string sId;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("id"), sId ) ;

	if( sId.empty() == M4_TRUE )
	{
		// System methods do not require security
		return;
	}

	// checks method security
	m4return_t iResult = m_oMethod.CheckSecurity();

	if (iResult == M4_SUCCESS)
	{
		// everything is ok... just keep on
		return;
	}

	if (iResult == M4_ERROR)
	{
		// no permission
		SetError(M4_ERROR);
		return;
	}

	// lets se if we have an active authentication at server side
	iResult = m_poContext->CheckActiveAuthentication();

	if (iResult == M4_SUCCESS)
	{
		// everything is ok... just keep on
		return;
	}

	if (iResult == M4_ERROR)
	{
		// no permission
		SetError(M4_ERROR);
		return;
	}

	// ok, we need to send an error to the client and disable methods execution
	m_poContext->SetEnableMethods(M4_FALSE);

	// generates the output tag
	m_oOut << "<" << name << " id=\"" << attributes->getValue("id") << "\"";

	// writes the XML output indicating that the security test failed
	m_oOut << " result=\"0\" value=\"-1\" mustauthenticate=\"true\"/>" << endl;

  //## end M4XMLSAXHandlers::procMethodPreProcess%938096977.body
}

void M4XMLSAXHandlers::procMethod (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procMethod%938096977.body preserve=yes

	// if methods execution is disable we just skip this
	if (m_poContext->GetEnableMethods() == M4_FALSE)
	{
		return;
	}

	// generates the output tag
	m_oOut << "<" << name << " id=\"" << attributes->getValue("id") << "\"";

	// Inits the method
	// UNICODE XML
	string sItem;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("item"), sItem ) ;

	// eh 264940
	string	strRole, strOrg;
	m_oMethod.GetRoleOrg( strRole, strOrg );

	// efv Dump Info
	dumpXML("Method",sItem.c_str(),"", strOrg.c_str(), strRole.c_str(), m_poContext);

	if (m_oMethod.Reset(m_poContext, sItem) != M4_SUCCESS)
	{
		SetError(M4_ERROR);
		return;
	}

	string sUniqueId;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("registeruniqueid"), sUniqueId ) ;

	if (sUniqueId.size())
	{
		m_oMethod.SetRegisterUniqueId(atoi(sUniqueId.c_str()));
	}

	string sEncryptResult;
	M4XMLToSTL((M4XMLCh*)attributes->getValue("encryptresult"), sEncryptResult);

	if (sEncryptResult == "1" || sEncryptResult == "true")
	{
		m_oMethod.SetEncryptResult(M4_TRUE);
	}

	if (m_oMethod.GetNArgs() < 0)
	{
		// UNICODE XML
		string sName;
		M4XMLToSTL( (M4XMLCh*)name, sName ) ;

		DUMP_CHLOG_ERRORF ( M4_ERR_XML_PROC_METHOD_VALID, sItem.c_str()	<< sName.c_str());
		SetError(M4_ERROR);
	}

  //## end M4XMLSAXHandlers::procMethod%938096977.body
}



void M4XMLSAXHandlers::procArg (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procArg%938096978.body preserve=yes

	if (m_oMethod.GetNArgs() < 0)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_ARG_ERROR );
		SetError(M4_ERROR);
		return;
	}
	
	// UNICODE XML
	string sName;
	string sValue;
	string sType;
	string sEncrypted;
	m4bool_t bHasValue = M4_FALSE;

	M4XMLToSTL( (M4XMLCh*)attributes->getValue("name"), sName ) ;
	if (attributes->getValue("value") != NULL) 
	{
		M4XMLToSTL( (M4XMLCh*)attributes->getValue("value"), sValue ) ;
		bHasValue = M4_TRUE;
	}
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("type"), sType ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("encrypted"), sEncrypted ) ;

	/* Bug 0154410
	Se tratan argumentos encriptados
	*/
	if( sEncrypted == "1" || sEncrypted == "true")
	{
		M4XMLDecryptTwoWay(sValue);
	}

	m_oMethod.AddArg( sName, (bHasValue ? &sValue : NULL) , sType);

  //## end M4XMLSAXHandlers::procArg%938096978.body
}

void M4XMLSAXHandlers::procData (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procData%938708973.body preserve=yes
	m_oItem.Reset();
  //## end M4XMLSAXHandlers::procData%938708973.body
}

void M4XMLSAXHandlers::procDataDef (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procDataDef%938437884.body preserve=yes
	
	// writes the output tag
	m_oOut << "<" << "data";
	DumpAttrib(attributes);
	m_oOut << ">" << endl;

	// reset members
	m_oItemList.Reset(m_poContext);

  //## end M4XMLSAXHandlers::procDataDef%938437884.body
}

void M4XMLSAXHandlers::procNode (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procNode%938708974.body preserve=yes

	// saves the node data
	// UNICODE XML
	string sId;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("id"), sId ) ;

	m_oItem.SetNode(sId);

  //## end M4XMLSAXHandlers::procNode%938708974.body
}

void M4XMLSAXHandlers::procNodeDef (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procNodeDef%938437886.body preserve=yes

	// saves the node data
	// UNICODE XML
	string sId;
	string sRecursive;
	string sSyncCurrent;
	string sEnableDeltas;

	M4XMLToSTL( (M4XMLCh*)attributes->getValue("id"), sId ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("recursive"), sRecursive ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("syncCurrent"), sSyncCurrent ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("enableDeltas"), sEnableDeltas ) ;

	if (m_oItemList.AddNode(sId, sRecursive, sSyncCurrent, sEnableDeltas) != M4_SUCCESS)
	{
		SetError(M4_ERROR);
	}

  //## end M4XMLSAXHandlers::procNodeDef%938437886.body
}

void M4XMLSAXHandlers::procRegister (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procRegister%938708975.body preserve=yes

	// saves the register data	
	// UNICODE XML
	string sIndex;

	// read the record identifier (SOAP Persistence) 
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("regid"), sIndex ) ;

	// when the attribute is not present, the M4XMLToSTL blanks.
	if ( sIndex != "" ) 
	{
		// if the record identifier comes, ignore the index
		m_oItem.SetRegisterById(sIndex);
	}
	else
	{
		// if the record identifier does not come, get the index
		M4XMLToSTL( (M4XMLCh*)attributes->getValue("index"), sIndex ) ;
		m_oItem.SetRegister(sIndex);
	}

	if (m_oItem.Move(*m_poContext, M4_TRUE) != M4_SUCCESS)
	{
		SetError(M4_ERROR);
	}

  //## end M4XMLSAXHandlers::procRegister%938708975.body
}

void M4XMLSAXHandlers::procRegisterDef (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procRegisterDef%938437887.body preserve=yes

	// saves the register data
	// UNICODE XML
	string sIndex;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("index"), sIndex ) ;

	if (m_oItemList.AddRegister(sIndex, M4_TRUE) != M4_SUCCESS)
	{
		SetError(M4_ERROR);
	}

  //## end M4XMLSAXHandlers::procRegisterDef%938437887.body
}

void M4XMLSAXHandlers::procItem (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procItem%938708976.body preserve=yes

	// saves the item data
	// UNICODE XML
	string sId;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("id"), sId ) ;

	m_oItem.SetItem(sId);

	const XMLCh *poValue     = attributes->getValue("value");
	const XMLCh *poValue64   = attributes->getValue("value64");
	const XMLCh *poFileIndex = attributes->getValue("fileindex");
	const XMLCh *poFileData  = attributes->getValue("filedata");
	const XMLCh *poAuxLevel  = attributes->getValue("auxlevel");

	if (poValue == NULL && poValue64 == NULL && poFileIndex == NULL && poFileData == NULL)
	{
		if (m_oItem.SetNull(*m_poContext) != M4_SUCCESS)
		{
			SetError(M4_ERROR);
		}
	}
	else
	{
		// gets the level
		m4int32_t iLevel = 0;
		if (poAuxLevel != NULL)
		{
			// UNICODE XML
			string sLevel;
			M4XMLToSTL( (M4XMLCh*)poAuxLevel, sLevel ) ;

			iLevel = atoi(sLevel.c_str());
		}

		// normal data
		if (poValue != NULL)
		{
			// UNICODE XML
			string sValue;
			M4XMLToSTL( (M4XMLCh*)poValue, sValue ) ;

			if (m_oItem.SetValue(*m_poContext, sValue, iLevel) != M4_SUCCESS)
			{
				SetError(M4_ERROR);
			}
		}
		else if (poValue64 != NULL)
		{
			// converts to binary
			// UNICODE XML
			string sOut;
			string sValue64;
			M4XMLToSTL( (M4XMLCh*)poValue64, sValue64 ) ;

			if (m_oOut.Decode64(sValue64.c_str(), sOut) != M4_SUCCESS)
			{
				// UNICODE XML
				string sId;
				M4XMLToSTL( (M4XMLCh*)attributes->getValue("id"), sId ) ;

				DUMP_CHLOG_ERRORF ( M4_ERR_XML_ITEM_DECODE64, sId.c_str()	<< m_oItem.GetM4OAlias() << m_oItem.GetNode() << m_oItem.GetRegister());
				SetError(M4_ERROR);
			}
			else
			{
				if (m_oItem.SetValue(*m_poContext,sOut,iLevel) != M4_SUCCESS)
				{
					SetError(M4_ERROR);
				}
			}
		}
		// blob file
		else
		{
			// data file not supported yet
			const XMLCh *poFileDescr = attributes->getValue("descr");

			// UNICODE XML
			string sIndex;
			string sFileDescr;

			M4XMLToSTL( (M4XMLCh*)poFileIndex, sIndex ) ;
			M4XMLToSTL( (M4XMLCh*)poFileDescr, sFileDescr ) ;

			m4int32_t iIndex = atoi(sIndex.c_str());
			
			if(m_oItem.SetFileValueByRef(*m_poContext,m_poContext->GetInFileName(iIndex), sFileDescr) != M4_SUCCESS)
			{
				SetError(M4_ERROR);
			}
		}
	}
  //## end M4XMLSAXHandlers::procItem%938708976.body
}

void M4XMLSAXHandlers::procItemDef (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procItemDef%938437888.body preserve=yes

	// saves the item data
	// UNICODE XML
	string sId;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("id"), sId ) ;

	if (m_oItemList.AddItem(sId) != M4_SUCCESS)
	{
		SetError(M4_ERROR);
	}

  //## end M4XMLSAXHandlers::procItemDef%938437888.body
}

void M4XMLSAXHandlers::procFilter (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procFilter%939030964.body preserve=yes
	// gets the filter attrib

	// UNICODE XML
	string sType;
	string sItem;
	string sMode;
	string sValue;

	M4XMLToSTL( (M4XMLCh*)attributes->getValue("type"), sType ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("item"), sItem ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("mode"), sMode ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("value"), sValue ) ;

	if (m_oMethod.SetFilter(m_poContext, sType, sItem, sMode, sValue) != M4_SUCCESS)
	{
		SetError(M4_ERROR);
	}
  //## end M4XMLSAXHandlers::procFilter%939030964.body
}

void M4XMLSAXHandlers::procFilterArg (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procFilterArg%939030965.body preserve=yes
	// UNICODE XML
	string sMode;
	string sName;
	string sValue;

	M4XMLToSTL( (M4XMLCh*)attributes->getValue("mode"), sMode ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("name"), sName ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("value"), sValue ) ;

	if (m_oMethod.AddFilterArg(sMode, sName, sValue) != M4_SUCCESS)
	{
		SetError(M4_ERROR);
	}

  //## end M4XMLSAXHandlers::procFilterArg%939030965.body
}

void M4XMLSAXHandlers::procTranslType (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procTranslType%939289638.body preserve=yes

	// UNICODE XML
	string sType;
	string sMethod;
	string sResultItems;

	M4XMLToSTL( (M4XMLCh*)attributes->getValue("type"), sType ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("method"), sMethod ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("resultitems"), sResultItems ) ;

	if (m_poContext->AddTranslType(sType, sMethod, sResultItems) != M4_SUCCESS)
	{
		SetError(M4_ERROR);
	}

  //## end M4XMLSAXHandlers::procTranslType%939289638.body
}

void M4XMLSAXHandlers::procTranslItem (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procTranslItem%939289641.body preserve=yes

	// UNICODE XML
	string sType;
	string sItem;

	M4XMLToSTL( (M4XMLCh*)attributes->getValue("type"), sType ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("item"), sItem ) ;

	if (m_poContext->AddTranslItem(sType, sItem) != M4_SUCCESS)
	{
		SetError(M4_ERROR);
	}

  //## end M4XMLSAXHandlers::procTranslItem%939289641.body
}

void M4XMLSAXHandlers::procTranslItemArg (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procTranslItemArg%939372141.body preserve=yes

	// UNICODE XML
	string sName;
	string sValue;

	M4XMLToSTL( (M4XMLCh*)attributes->getValue("name"), sName ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("value"), sValue ) ;

	if (m_poContext->AddTranslItemArg(sName, sValue) != M4_SUCCESS)
	{
		SetError(M4_ERROR);
	}
  //## end M4XMLSAXHandlers::procTranslItemArg%939372141.body
}

void M4XMLSAXHandlers::procMove (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procMove%940850223.body preserve=yes
	

	// UNICODE XML
	string sRegister;

	// read the record identifier (SOAP Persistence) 
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("regid"), sRegister ) ;

	// when the attribute is not present, the M4XMLToSTL blanks.
	if ( sRegister != "" ) 
	{
		// if the record identifier comes, ignore the index
		m_oItem.SetIsById(true);
	}
	else
	{

		// if the record identifier does not come, get the index
		M4XMLToSTL( (M4XMLCh*)attributes->getValue("register"), sRegister ) ;

		m_oItem.SetIsById(false);
	}	

	// boolean arguments are M4_TRUE (updates the item) and M4_TRUE (is a real item)
	if (m_oItem.Reset(*m_poContext, sRegister, M4_TRUE, M4_TRUE) != M4_SUCCESS)
	{
		SetError(M4_ERROR);
	}

  //## end M4XMLSAXHandlers::procMove%940850223.body
}

void M4XMLSAXHandlers::procMoveDef (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procMoveDef%940850224.body preserve=yes

	bool bIsRegID = M4_FALSE; 

	// UNICODE XML
	string sRegister;

	M4XMLToSTL( (M4XMLCh*)attributes->getValue("register"), sRegister ) ;

	// boolean arguments are M4_TRUE (does not update the item) and M4_TRUE (is a real item)
	if (m_oItem.Reset(*m_poContext, sRegister, M4_FALSE, M4_TRUE) != M4_SUCCESS)
	{
		SetError(M4_ERROR);
	}
  //## end M4XMLSAXHandlers::procMoveDef%940850224.body
}

void M4XMLSAXHandlers::procLog (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procLog%941014279.body preserve=yes
	// writes the log data

	// UNICODE XML
	string sId;
	string sDateMode;
	string sFullMode;

	M4XMLToSTL( (M4XMLCh*)attributes->getValue("id"), sId ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("datemode"), sDateMode ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("fullmode"), sFullMode ) ;

	if (DumpLog(sId, sDateMode, sFullMode) != M4_SUCCESS)
	{
		SetError();
	}
  //## end M4XMLSAXHandlers::procLog%941014279.body
}

void M4XMLSAXHandlers::procM4OParam (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procM4OParam%944575097.body preserve=yes

	// UNICODE XML
	string sName;
	string sValue;
	string sCppType;

	M4XMLToSTL( (M4XMLCh*)attributes->getValue("name"), sName ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("value"), sValue ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("cpptype"), sCppType ) ;

	if (m_poContext->ProcM4OParam(sName, sValue, sCppType) != M4_SUCCESS)
	{
		SetError();
	}
  //## end M4XMLSAXHandlers::procM4OParam%944575097.body
}

void M4XMLSAXHandlers::procGenCommands (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procGenCommands%1089357392.body preserve=yes
	m_oOut << "<" << name << ">" << endl;
  //## end M4XMLSAXHandlers::procGenCommands%1089357392.body
}

void M4XMLSAXHandlers::procCommand (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procCommand%1089357393.body preserve=yes

	// UNICODE XML
	M4XMLToSTL((M4XMLCh*)attributes->getValue("id"), m_sGenCommand);

	m_oGenCommArgs.clear();	

	if( XMLICMP(m_sGenCommand, "refreshmdcache", 1) == M4_TRUE )
	{
		m_oOut << "<" << name << " id=\""<<attributes->getValue("id")<<"\" alias=\"" << attributes->getValue("alias") << "\"";
	}
	else
	{
		SetError(M4_ERROR);
		m_sGenCommand = "";
	}
  //## end M4XMLSAXHandlers::procCommand%1089357393.body
}

void M4XMLSAXHandlers::procCommArg (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procCommArg%1089357394.body preserve=yes
	// UNICODE XML
	string sName;
	string sVale;

	M4XMLToSTL((M4XMLCh*)attributes->getValue("name"), sName);
	M4XMLToSTL((M4XMLCh*)attributes->getValue("value"), sVale);

	m_oGenCommArgs[sName] = sVale;
  //## end M4XMLSAXHandlers::procCommArg%1089357394.body
}

void M4XMLSAXHandlers::procM4ORef (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procM4ORef%944575097.body preserve=yes

	// UNICODE XML
	string sM4OAlias;
	string sL2Alias;

	M4XMLToSTL( (M4XMLCh*)attributes->getValue("m4oalias"), sM4OAlias ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue("l2alias"), sL2Alias ) ;

	if (m_poContext->ProcM4ORef(sM4OAlias, sL2Alias) != M4_SUCCESS)
	{
		SetError();
	}
  //## end M4XMLSAXHandlers::procM4ORef%944575097.body
}

void M4XMLSAXHandlers::procEndSimple (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procEndSimple%938096976.body preserve=yes
	m_oOut << "</" << name << ">" << endl;
  //## end M4XMLSAXHandlers::procEndSimple%938096976.body
}

void M4XMLSAXHandlers::procEndCommandSet (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procEndCommandSet%938708971.body preserve=yes

	// writes the log data
	if (DumpLog("","",""))
	{
		SetError();
	}

	m_oOut << "</dataset>" << endl;
  //## end M4XMLSAXHandlers::procEndCommandSet%938708971.body
}

void M4XMLSAXHandlers::procEndMethod (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procEndMethod%938162622.body preserve=yes

	// if methods execution is disable we just skip this
	if (m_poContext->GetEnableMethods() == M4_FALSE)
	{
		return;
	}

	// invoques the method
	m4return_t iResult = m_oMethod.Execute();

	if (iResult != M4_SUCCESS)
	{
		SetError(iResult);
	}

	// writes the XML output
	m_oOut << " result=\"" << (m4int32_t) m_oMethod.GetResult() << "\"";

	if (m_oMethod.GetEncryptResult() == M4_TRUE)
	{
		m_oOut << " encryptresult=\"true\"";
	}

	if (m_oMethod.GetResultType() >= 0)
	{
		 if (m_oMethod.GetResultIsNull() == M4_FALSE 
			 || (m_poContext != NULL && m_poContext->GetEnableNulls() == M4_FALSE))
		 {
			m_oOut << " value=\"";
			m_oOut.Transl(m_oMethod.GetResultValue().c_str());
			m_oOut << "\"";
		 }

		 m_oOut << " cpptype=\"" << m_oMethod.GetResultType() << "\"";
	}
	
	m_oOut << "/>" << endl;

  //## end M4XMLSAXHandlers::procEndMethod%938162622.body
}

void M4XMLSAXHandlers::procEndDataDef (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procEndDataDef%938437885.body preserve=yes

	// finish data dump
	m_oItemList.Reset(NULL);

	// writes the end tag
	m_oOut << "</data>" << endl;
  //## end M4XMLSAXHandlers::procEndDataDef%938437885.body
}

void M4XMLSAXHandlers::procEndFilter (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procEndFilter%939030966.body preserve=yes
	if (m_oMethod.ExecuteFilter(m_poContext) != M4_SUCCESS)
	{
		SetError(M4_ERROR);
	}
  //## end M4XMLSAXHandlers::procEndFilter%939030966.body
}

void M4XMLSAXHandlers::procEndTranslItem (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procEndTranslItem%939372142.body preserve=yes
	m_poContext->ResetTranslItem();
  //## end M4XMLSAXHandlers::procEndTranslItem%939372142.body
}

void M4XMLSAXHandlers::procEndContextPreProcess (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procEndContextPreProcess%939372142.body preserve=yes

	// we only finish the context in the pre processing if we have an error
	if (IsOk() == M4_FALSE)
	{
		// calls the standard end element routine
		procEndContext(name, attributes);
		m_oCMInitParameters.SetParseStatus(M4_FALSE);
	}
  //## end M4XMLSAXHandlers::procEndContextPreProcess%939372142.body
}

void M4XMLSAXHandlers::procEndContext (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procEndContext%940341738.body preserve=yes

	// dump channels context info
	m_poContext->DumpM4OInfo(m_oOut);
	
#if _M4_DEBUG > 10
	// Archivos relacionados: "srvm4xmlsax.hpp", "srvm4xmlsax.cpp", "exem4xmlsax.cpp"
	bEndContextStart = M4_TRUE;
#endif

	// reset objects references
	m_poContext->ResetM4OReferences();

	// retrieves the channel manager
	ClChannelManager* pChannelManager = m_poContext->GetObjectService()->GetpChannelManager();
	
	// User Actions Auditory.
	pChannelManager->UserActionsAudit.SwitchOff( M4_TRUE );

	// reset jvm
	pChannelManager->ReleaseEmptyJavaContexts();

	// reset members
	Reset();

#if _M4_DEBUG > 10
	// Archivos relacionados: "srvm4xmlsax.hpp", "srvm4xmlsax.cpp", "exem4xmlsax.cpp"
	bEndContextEnd = M4_TRUE;
#endif

	m_oOut << "</" << name << ">" << endl;
  //## end M4XMLSAXHandlers::procEndContext%940341738.body
}

void M4XMLSAXHandlers::procEndNodeDef (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procEndNodeDef%940864584.body preserve=yes
	// writes the output filter
	if (m_oItemList.Execute(m_oOut) != M4_SUCCESS)
	{
		SetError(M4_ERROR);
	}
  //## end M4XMLSAXHandlers::procEndNodeDef%940864584.body
}

void M4XMLSAXHandlers::procEndM4OAlias (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procEndM4OAlias%944575098.body preserve=yes
	if (m_poContext->GenAlias(m_oOut) != M4_SUCCESS)
	{
		SetError();
	}
  //## end M4XMLSAXHandlers::procEndM4OAlias%944575098.body
}

void M4XMLSAXHandlers::procEndGenCommands (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procEndGenCommands%1089357395.body preserve=yes
	m_oOut << "</" << name << ">" << endl;
  //## end M4XMLSAXHandlers::procEndGenCommands%1089357395.body
}

void M4XMLSAXHandlers::procEndCommand (const XMLCh* const name, AttributeList *attributes)
{
  //## begin M4XMLSAXHandlers::procEndCommand%1089357396.body preserve=yes
	if(XMLICMP(m_sGenCommand,"refreshmdcache",1) == M4_TRUE)
	{
		m4return_t iResult;

		if ((iResult=m_oCMInitParameters.GetMetadataSubsystem()->RefreshUpdateList())!=M4_SUCCESS)
		{
			SetError(M4_ERROR);
		}
		m_oOut << " result=\"" << iResult << "\" type=\"1\" value=\"\"/>" << endl;		
	}
	else
	{
		SetError(M4_ERROR);
	}
  //## end M4XMLSAXHandlers::procEndCommand%1089357396.body
}

m4return_t M4XMLSAXHandlers::DumpLog (const string& ai_sId, const string& ai_sDateMode, const string& ai_sFullMode)
{
  //## begin M4XMLSAXHandlers::DumpLog%940504734.body preserve=yes

	// checks the stack
	if (EMPTY() == M4_TRUE)
	{
		return M4_SUCCESS;
	}

	m4bool_t bDateMode = XMLICMP(ai_sDateMode,"yes",1);
	m4bool_t bFullMode = XMLICMP(ai_sFullMode,"yes",1);

	// writes the header
	m_oOut << "<log";
	if (ai_sId.size())
	{
		m_oOut << " id=\"" << ai_sId.c_str() << "\"";
	}
	m_oOut << ">" << endl;

	// writes the messages
	m4char_t pcBufferType[M4XML_LOG_MAX_TYPE];
	m4char_t pcBufferCode[M4XML_LOG_MAX_CODE];
	m4char_t pcBufferText[M4XML_LOG_MAX_TEXT];
	m4char_t pcBufferTime[M4XML_LOG_MAX_TIME];
	m4int32_t lBufferFilled=0;
	m4return_t iResult = M4_SUCCESS;

	while (EMPTY() != M4_TRUE)
	{
		
		// checks abort status	
		if (m_poContext->GetObjectService()->GetpExecutor()->GetRequestToAbortWithClStatus() == M4_TRUE)
		{
			DUMP_CHLOG_DEBUG ( M4_ERR_XML_REQ_ABORT );
			SetError(M4CL_USER_ABORT_ALL_REQUEST);
			iResult = M4_ERROR;
			break;
		}

		// gets the type
		if (GETH_LASTERRORTYPE(pcBufferType,sizeof(pcBufferType),lBufferFilled) != M4_SUCCESS)
		{
			iResult = M4_ERROR;
			break;
		}

		// gets the code
		if (GETH_LASTERRORCODE(pcBufferCode,sizeof(pcBufferCode),lBufferFilled) != M4_SUCCESS)
		{
			iResult = M4_ERROR;
			break;
		}

		// converts to number
		m4uint32_t iCode = ( m4uint32_t)-1;
		sscanf(pcBufferCode,"%xld",&iCode);

		// gets the text
		if (GETH_LASTERRORTEXT(pcBufferText,sizeof(pcBufferText),lBufferFilled) != M4_SUCCESS)
		{
			iResult = M4_ERROR;
			break;
		}

		/* Bugs 0122197 y 0102154
		Se cambian los 3 por '#' por si se habían cambiado previamente
		*/
		m4int32_t	i = 0 ;

		for( i = 0 ; i < lBufferFilled ; i++ )
		{
			if( pcBufferText[ i ] == 3 )
			{
				pcBufferText[ i ] = '#' ;
			}
		}

		// gets the text
		if (bDateMode == M4_TRUE && 
			GETH_LASTERRORTIME(pcBufferTime,sizeof(pcBufferTime),lBufferFilled) != M4_SUCCESS)
		{
			iResult = M4_ERROR;
			break;
		}

		// deletes the entry
		POP_BACK();

		// writes the header
		m_oOut << "<logmess code=\"" << iCode << "\"" << " severity=\"" << pcBufferType << "\"";

		if (bDateMode == M4_TRUE)
		{
			m_oOut << " date=\"" << pcBufferTime << "\"";
		}

		if (bFullMode == M4_TRUE)
		{
			m_oOut << " param=\"";
			
			// bugid 0131247, se cambian los 3 por '#', para que puedan salir al param

			m4uint32_t	i = 0 ;
			m4uint32_t	iStart = 0 ;
			m4uint32_t	iEnd = 0 ;

			iEnd = iStart + strlen(pcBufferText) ;

			for( i = iStart ; i < iEnd ; i++ )
			{
				if( pcBufferText[ i ] == 3 )
				{
					pcBufferText[ i ] = '#' ;
				}
			}
			



			// end
			m_oOut.Transl(pcBufferText);
			m_oOut << "\"";
		}

		m_oOut << " value=\"";

		// gets the translation size
		// -1 or "" -> There is no error
		m4uint32_t iLen = ( m4uint32_t)-1;
		ClChannelManager *poCM = m_poContext->GetObjectService()->GetpChannelManager();
		m4language_t iLang = poCM->GetLogonAdaptor()->GetSessionLanguage("",M4CL_LANGUAGE_NONE);

		iResult = M4GetErrorString	(iCode, pcBufferText, 1, poCM, NULL, iLen, iLang);
		if (iResult != M4_SUCCESS || iLen <= 1)
		{
			// undefined error
			m_oOut << "Undefined Error: ";

			// bg 158180
			m_oOut.Transl(pcBufferText);
		}
		else
		{
			// creates a buffer
			m4pchar_t pcBuffer = new m4char_t [iLen];
			m4uint32_t iLen2 = iLen;

			iResult = M4GetErrorString	(iCode, pcBufferText, 1, m_poContext->GetObjectService()->GetpChannelManager(), pcBuffer, iLen2, iLang);

			if (iResult != M4_SUCCESS || iLen2 != iLen || strlen(pcBuffer) != (iLen-1) )
			{
				M4_ASSERT(0);
				m_oOut << "Internal Translation Error: ";
				m_oOut.Transl(pcBufferText);
			}
			else
			{
				m_oOut.Transl(pcBuffer);
			}

			delete [] pcBuffer;
		}

		// closes the logitem
		m_oOut << "\"/>" << endl;
	}

	// closes the tag
	m_oOut << "</log>" << endl;

	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_DUMP_LOG );
	}

	return iResult;
  //## end M4XMLSAXHandlers::DumpLog%940504734.body
}

m4return_t M4XMLSAXHandlers::LoadState ()
{
  //## begin M4XMLSAXHandlers::LoadState%971105393.body preserve=yes

	// inits members
	m4bool_t bFound = M4_FALSE;
	m_poContext = NULL;

	if (m_oCMInitParameters.GetStateManager() == NULL || m_oCMInitParameters.GetSessionId() == 0)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_INIT );
		return M4_ERROR;
	}

	// checks mode
	if (m_oCMInitParameters.GetSubSessionMode() != "none" && m_oCMInitParameters.GetSubSessionMode() != "new")
	{
		// recover the state (is not new)
		if (m_oCMInitParameters.GetStateManager()->RecoverState(m_oCMInitParameters,bFound, M4_FALSE) != M4_SUCCESS)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_LOAD_STATE, m_oCMInitParameters.GetSubSessionId().c_str() );
			return M4_ERROR;
		}
	}
	// efv: new caché
	else
	{
		// recover the state with a new channel manager (is new)
		if (m_oCMInitParameters.GetStateManager()->RecoverState(m_oCMInitParameters,bFound, M4_TRUE) != M4_SUCCESS)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_LOAD_STATE, m_oCMInitParameters.GetSubSessionId().c_str() );
			return M4_ERROR;
		}
	}

	// efv new caché
	// gets the new channel manager
	m_poContext = NULL;

	if (m_oCMInitParameters.GetExecutorState() != NULL)
	{
		m_poContext = m_oCMInitParameters.GetExecutorState()->GetXMLContext();
	}

	if (m_poContext == NULL)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_LOAD_STATE, m_oCMInitParameters.GetSubSessionId().c_str() );
		return M4_ERROR;
	}

	// ok, here we have a new context. We can't have more than one context running at same time
	m_poContext->ExecLock();

	// inits session channel context
	m4return_t iResult = M4_SUCCESS;
	if (bFound == M4_FALSE && m_poContext->InitSession() != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_INIT_SESSION, m_oCMInitParameters.GetSubSessionId().c_str() );
		iResult = M4_ERROR;
	}

	// resets the session status
	if (m_poContext->SetSession() != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_INIT_SESSION, m_oCMInitParameters.GetSubSessionId().c_str() );
		iResult = M4_ERROR;
	}

	return iResult;
  //## end M4XMLSAXHandlers::LoadState%971105393.body
}

m4return_t M4XMLSAXHandlers::PersistState ()
{
  //## begin M4XMLSAXHandlers::PersistState%971105394.body preserve=yes

	if (m_oCMInitParameters.GetExecutorState() == NULL || 
		m_oCMInitParameters.GetExecutorState()->GetChannelManager() == NULL || 
		m_oCMInitParameters.GetStateManager() == NULL || 
		m_oCMInitParameters.GetSessionId() == 0)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_INIT );
		return M4_ERROR;
	}

	// frees session channel access
	ClChannelManager *poCM = m_oCMInitParameters.GetExecutorState()->GetChannelManager();
	ClChannel *poSessionChannel = poCM->GetSessionChannel();
	if (poCM->SetSessionChannel(poSessionChannel) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_SET_SESSION_CHANNEL, m_oCMInitParameters.GetSubSessionId().c_str() );
	}

	// reset parameters (no erase)
	// frees logical connection and lets internal references for the persist phase
	m_oCMInitParameters.Reset(M4_FALSE,M4_FALSE);

	// persist the state
	if (m_oCMInitParameters.GetStateManager()->PersistState(m_oCMInitParameters) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_PERSIST_STATE, m_oCMInitParameters.GetSubSessionId().c_str() );
		return M4_ERROR;
	}

	// reset parameters
	m_oCMInitParameters.Reset(M4_TRUE,M4_TRUE);

	return M4_SUCCESS;
  //## end M4XMLSAXHandlers::PersistState%971105394.body
}

m4string_t M4XMLSAXHandlers::GetRepositoryKey()
{
	if (m_poContext == NULL || m_poContext->GetObjectService() == NULL)
	{
		return "";
	}

	ClChannelManager *poChannelManager = m_poContext->GetObjectService()->GetpChannelManager();
	if (poChannelManager == NULL)
	{
		return "";
	}

	ClMDUpdateList oUpdateList(ClPlusInfiniteDate(), 0);

	if (oUpdateList.RefreshList(poChannelManager) != M4_SUCCESS)
	{
		return "";
	}

	m4VariantType vRepositoryKey = oUpdateList.GetRepositoryKey();

	if (vRepositoryKey.Type != M4CL_CPP_TYPE_STRING_VAR)
	{
		return "";
	}

	return vRepositoryKey.Data.PointerChar;
}

// Additional Declarations
  //## begin M4XMLSAXHandlers%37E8E47C0295.declarations preserve=yes
  //## end M4XMLSAXHandlers%37E8E47C0295.declarations

// Class ClM4XMLErrorHandler 


ClM4XMLErrorHandler::ClM4XMLErrorHandler ()
  //## begin ClM4XMLErrorHandler::ClM4XMLErrorHandler%941014276.hasinit preserve=no
      : m_bIsOk(M4_TRUE)
  //## end ClM4XMLErrorHandler::ClM4XMLErrorHandler%941014276.hasinit
  //## begin ClM4XMLErrorHandler::ClM4XMLErrorHandler%941014276.initialization preserve=yes
  //## end ClM4XMLErrorHandler::ClM4XMLErrorHandler%941014276.initialization
{
  //## begin ClM4XMLErrorHandler::ClM4XMLErrorHandler%941014276.body preserve=yes
  //## end ClM4XMLErrorHandler::ClM4XMLErrorHandler%941014276.body
}



//## Other Operations (implementation)
void ClM4XMLErrorHandler::warning (const SAXParseException& exception)
{
  //## begin ClM4XMLErrorHandler::warning%940504735.body preserve=yes
	DumpMessage("Warning",exception);
  //## end ClM4XMLErrorHandler::warning%940504735.body
}

void ClM4XMLErrorHandler::error (const SAXParseException& exception)
{
  //## begin ClM4XMLErrorHandler::error%940504736.body preserve=yes
	m_bIsOk = M4_FALSE;
	DumpMessage("Error",exception);
  //## end ClM4XMLErrorHandler::error%940504736.body
}

void ClM4XMLErrorHandler::fatalError (const SAXParseException& exception)
{
  //## begin ClM4XMLErrorHandler::fatalError%940504737.body preserve=yes
	m_bIsOk = M4_FALSE;
	DumpMessage("Fatal Error",exception);
  //## end ClM4XMLErrorHandler::fatalError%940504737.body
}

void ClM4XMLErrorHandler::resetErrors ()
{
  //## begin ClM4XMLErrorHandler::resetErrors%940504738.body preserve=yes
  //## end ClM4XMLErrorHandler::resetErrors%940504738.body
}

void ClM4XMLErrorHandler::DumpMessage (m4pcchar_t ai_pcMessage, const SAXParseException& exception)
{
  //## begin ClM4XMLErrorHandler::DumpMessage%940504739.body preserve=yes

	// ERROR parsing

	// UNICODE XML
	string sPublicId;
	string sSytemId;
	
	M4XMLToSTL( (M4XMLCh*)exception.getPublicId(), sPublicId ) ;
	M4XMLToSTL( (M4XMLCh*)exception.getSystemId(), sSytemId ) ;


	DUMP_CHLOG_ERRORF ( M4_ERR_XML_DUMP_MESSAGE, ai_pcMessage
		<< sPublicId.c_str()
		<< sSytemId.c_str()
		<< (m4int32_t)exception.getLineNumber()
		<< (m4int32_t)exception.getColumnNumber());

#ifdef _DEBUG

	// UNICODE XML
	cerr << ai_pcMessage << " parsing entity " << 
		sPublicId.c_str() <<
		"(" << sSytemId.c_str() << 
		") in line " << exception.getLineNumber() << 
		" column " << exception.getColumnNumber() << endl;
#endif
  //## end ClM4XMLErrorHandler::DumpMessage%940504739.body
}

// Additional Declarations
  //## begin ClM4XMLErrorHandler%380F41150285.declarations preserve=yes
  //## end ClM4XMLErrorHandler%380F41150285.declarations

// Class ClM4XMLMethodStatus 











ClM4XMLMethodStatus::ClM4XMLMethodStatus ()
  //## begin ClM4XMLMethodStatus::ClM4XMLMethodStatus%938162612.hasinit preserve=no
  //## end ClM4XMLMethodStatus::ClM4XMLMethodStatus%938162612.hasinit
  //## begin ClM4XMLMethodStatus::ClM4XMLMethodStatus%938162612.initialization preserve=yes
  //## end ClM4XMLMethodStatus::ClM4XMLMethodStatus%938162612.initialization
{
  //## begin ClM4XMLMethodStatus::ClM4XMLMethodStatus%938162612.body preserve=yes
	Init();
  //## end ClM4XMLMethodStatus::ClM4XMLMethodStatus%938162612.body
}


ClM4XMLMethodStatus::~ClM4XMLMethodStatus ()
{
  //## begin ClM4XMLMethodStatus::~ClM4XMLMethodStatus%938162613.body preserve=yes
	Reset();
  //## end ClM4XMLMethodStatus::~ClM4XMLMethodStatus%938162613.body
}



//## Other Operations (implementation)
m4return_t ClM4XMLMethodStatus::Reset (ClM4XMLContext *ai_poContext, const string &ai_sItem, m4bool_t ai_bUpdate, m4bool_t ai_bRealItem)
{
  //## begin ClM4XMLMethodStatus::Reset%938162614.body preserve=yes
	Reset();

	m_poContext = ai_poContext;

	if (m_oItem.Reset(*m_poContext,ai_sItem,ai_bUpdate,ai_bRealItem) != M4_SUCCESS)
	{
		return M4_ERROR;
	}
	
	// checks method
	m_iNArgs = m_oItem.GetNArgs();
	m_bIsVariableArgs = m_oItem.IsVariableArgs();

	return M4_SUCCESS;
	
  //## end ClM4XMLMethodStatus::Reset%938162614.body
}

void ClM4XMLMethodStatus::Reset ()
{
  //## begin ClM4XMLMethodStatus::Reset%938162617.body preserve=yes
	m_oItem.Reset();

	m_poContext = NULL;
	m_iUsedArgs = 0;
	m_iNArgs= -1;
	m_iResult = M4_ERROR;
	m_bIsVariableArgs = M4_FALSE;
	m_iNFixedArgs = 0;
	m_iNVariableArgs = 0;
	m_iRegisterUniqueId = -1;
	m_oArgVector.clear();
  //## end ClM4XMLMethodStatus::Reset%938162617.body
}

m4return_t ClM4XMLMethodStatus::AddArg (const string &ai_sName, const string *ai_psValue, const string &ai_sType)
{
  //## begin ClM4XMLMethodStatus::AddArg%938162618.body preserve=yes
	if (m_iNArgs < 0)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_ADD_ARG_INVALID, m_oItem.GetM4OAlias() <<
			m_oItem.GetNode() << m_oItem.GetItem());
		return M4_ERROR;
	}

	m4int16_t iIndex = -1;
	m4bool_t bIsVariableArg = M4_FALSE;
	m4bool_t bContinue = M4_TRUE;

	// by name
	if (ai_sName.size() != 0)
	{
		iIndex = m_oItem.GetArgIndex(ai_sName.c_str());
		if (iIndex >= 0)
		{
			m_iNFixedArgs++;
			
			// checks for already used element
			if (((m4int16_t)(m_oArgVector.size())) > iIndex &&
				m_oArgVector[iIndex].IsUsed() == M4_TRUE)
			{
				bContinue = M4_FALSE;
			}
		}
	}

	// by position
	if (bContinue == M4_TRUE && iIndex < 0)
	{
		// checks for anonymous fixed args
		m4int16_t iSize = m_oArgVector.size();
		if (iSize > m_iNArgs)
		{
			iSize = m_iNArgs;
		}
		
		m4int16_t iTempIndex = 0;

		for (; iTempIndex<iSize; iTempIndex++)
		{
			if (m_oArgVector[iTempIndex].IsUsed() == M4_FALSE)
			{
				break;
			}
		}
		
		if (iTempIndex < m_iNArgs)
		{
			iIndex = iTempIndex;
			m_iNFixedArgs++;
		}
	}

	// variable args
	if (bContinue == M4_TRUE && iIndex < 0 && m_bIsVariableArgs == M4_TRUE)
	{
		iIndex = m_iNArgs + (m_iNVariableArgs++);
		bIsVariableArg = M4_TRUE;
	}
	
	if (bContinue != M4_TRUE || iIndex < 0)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_ADD_ARG_INVALID_ARG, m_oItem.GetM4OAlias() <<
			m_oItem.GetNode() << m_oItem.GetItem() << ai_sName.c_str());
		return M4_ERROR;
	}
	
	ClM4XMLVariant vResult;
	if (ai_psValue != NULL)
	{
		// checks encryption
		string sValue = *ai_psValue;
		m4bool_t bIsNull = M4_FALSE;
		if (m_bEncrypted == M4_TRUE && m_poContext != NULL)
		{
			if (m_poContext->DeCrypt(sValue, bIsNull, m_sEncryptionFuncGroups) != M4_SUCCESS) 
			{
				DUMP_CHLOG_ERRORF ( M4_ERR_XML_ADD_ARG_ERROR_DECRYPT, ai_sName.c_str() << m_oItem.GetItem() << m_oItem.GetNode() << m_sEncryptionFuncGroups );
				return M4_ERROR;
			}
		}

		if (bIsNull)
		{
			vResult.SetNull();
		}
		else
		{
			if (m_oItem.ConvArg(iIndex,bIsVariableArg,sValue,ai_sType,vResult) != M4_SUCCESS)
			{
				DUMP_CHLOG_ERRORF ( M4_ERR_XML_ADD_ARG_INVALID_TYPE, ai_sName.c_str() << m_oItem.GetItem() << m_oItem.GetNode());
				return M4_ERROR;
			}
		}
	}
	else
	{
		vResult.SetNull();
	}
		
	// ensures size
	if (m_oArgVector.size() <= (m4uint16_t)iIndex)
	{
		m_oArgVector.resize(iIndex+1);
	}

	// sets the value
	vResult.SetUsed();
	m_oArgVector[iIndex] = vResult;
	m_iUsedArgs++;

	return M4_SUCCESS;

  //## end ClM4XMLMethodStatus::AddArg%938162618.body
}

m4return_t ClM4XMLMethodStatus::Execute ()
{
  //## begin ClM4XMLMethodStatus::Execute%938162623.body preserve=yes

	// push args into stack
	if (m_oItem.GetAccess() == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_EXECUTE_NO_ACCESS );
		return M4_ERROR;
	}

	if (m_iNFixedArgs < m_iNArgs)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_NOT_ENOUGH_FIXED_ARGS, m_oItem.GetM4OAlias() <<
			m_oItem.GetNode() << m_oItem.GetItem() << m_iNFixedArgs << m_iNArgs);
		m_oItem.Reset();
		return M4_ERROR;
	}

	IStackInterface &Stack = m_oItem.GetAccess()->GetpExecutor()->Stack;

	for (m4int16_t i=0; i< m_iUsedArgs; i++)
	{
		if (Stack.Push(m_oArgVector[i]) != M4_SUCCESS)
		{
			m_oItem.Reset();
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_PUSH_ARGS, m_oItem.GetM4OAlias() <<
			m_oItem.GetNode() << m_oItem.GetItem());
			return M4_ERROR;
		}
	}

	// invoques the method
	m_iResult = m_oItem.Execute(*m_poContext, m_iUsedArgs, m_iRegisterUniqueId);

	// checks encryption
	m4bool_t bResultIsNull = m_oItem.GetResultIsNull();
	if (m_iResult != M4_ERROR && m_bEncrypted && m_poContext != NULL)
	{
		// lets encrypt the result
		string sValue = bResultIsNull == M4_TRUE ? "" : m_oItem.GetResult();
		m_poContext->Encrypt(sValue, bResultIsNull, m_sEncryptionFuncGroups);
		m_oItem.SetResult(sValue);
	}
	else if (m_iResult != M4_ERROR && m_bEncryptResult == M4_TRUE)
	{
		string sValue = bResultIsNull == M4_TRUE ? "" : m_oItem.GetResult();
		M4XMLEncryptTwoWay(sValue);
		m_oItem.SetResult(sValue);
	}

	// -- User Actions Auditory.
	if( m_iResult != M4_ERROR )
	{
		_RegisterUserAuditXMLMethodEvent() ;
	}

	return m_iResult;
  //## end ClM4XMLMethodStatus::Execute%938162623.body
}

void ClM4XMLMethodStatus::Init ()
{
  //## begin ClM4XMLMethodStatus::Init%938162625.body preserve=yes
  m_poContext = NULL;
  m_iUsedArgs = 0;
  m_iNArgs= -1;
  m_iResult = M4_ERROR;
  m_bIsVariableArgs = M4_FALSE;
  m_iNFixedArgs = 0;
  m_iNVariableArgs = 0;
  m_iRegisterUniqueId = -1;
  m_bEncrypted = M4_FALSE;
  //## end ClM4XMLMethodStatus::Init%938162625.body
}


m4return_t	ClM4XMLMethodStatus::_RegisterUserAuditXMLMethodEvent()
{
	ClUserAction*		poAction = NULL ;
	ClChannelManager*	pChannelManager ;
	ClAccess*			pAccess ;
	ClCompiledMCR*		pCMCR ;
	m4int32_t			iHandle ;
	m4uint32_t			hNode ;
	m4pcchar_t			pccId ;
	m4pcchar_t			pccName ;
	m4pchar_t			pcOrgId ;
	ClVariantConversor	oVarConv ;
	m4VariantType		vValue ;
	m4pcchar_t			pccArgName ;


	pChannelManager = m_poContext->GetObjectService()->GetpChannelManager() ;

	if( pChannelManager == NULL )
	{
		return M4_SUCCESS ;
	}

	pAccess = m_oItem.GetAccess() ;

	if( pAccess == NULL )
	{
		return M4_ERROR ;
	}

	pAccess->GetpChannel()->Organization.Get( pcOrgId ) ;

	if( pChannelManager->UserActionsAudit.HasAuditory( ClUserAction::EV_METHOD, pcOrgId ) == M4_FALSE )
	{
		return( M4_SUCCESS ) ;
	}

	pCMCR = pAccess->GetpCMCR() ;

	if( pCMCR == NULL )
	{
		return M4_ERROR ;
	}


	if( pCMCR->GetChannelType() > 100 )
	{
		return M4_SUCCESS ;
	}
	
	poAction = new ClUserAction() ;

	poAction->SetEventID( ClUserAction::EV_METHOD ) ;

	pccId	= pCMCR->GetChannelId() ;
	pccName = pCMCR->GetChannelName() ;

	poAction->SetEventObject( pccId ) ;
	poAction->SetEventObjectName( pccName ) ;

	poAction->AppendEventObject( "!" ) ;
	poAction->AppendEventObjectName( "!" ) ;
	

	iHandle = m_oItem.GetHandle() ;

	// -- Método de canal.
	if( iHandle == -1 )
	{
		pccId	= m_oItem.GetItem() ;
		pccName = pccId ;

		poAction->AppendEventObject( pccId ) ;
		poAction->AppendEventObjectName( pccName ) ;
	}
	else
	{
		hNode = pCMCR->GetItemNodeHandle( iHandle ) ;

		pccId = pCMCR->GetNodeId( hNode ) ;
		pccName = pCMCR->GetNodeName( hNode ) ;
	
		if( pccName == NULL || *pccName == '\0' )
		{
			pccName = pccId;
		}

		poAction->AppendEventObject( pccId ) ;
		poAction->AppendEventObjectName( pccName ) ;

		poAction->AppendEventObject( "." ) ;
		poAction->AppendEventObjectName( "." ) ;

		pccId = pCMCR->GetItemId( iHandle ) ;
		pccName = pCMCR->GetItemName( iHandle ) ;
		
		if( pccName == NULL || *pccName == '\0' )
		{
			pccName = pccId;
		}

		poAction->AppendEventObject( pccId ) ;
		poAction->AppendEventObjectName( pccName ) ;
	}

	poAction->SetEventArgs( "" ) ;

	// -- Mientras no se aclare el tema de los argumentos variables
	if( m_bIsVariableArgs == M4_FALSE )
	{
		for (m4int16_t iIdx = 0; iIdx < m_iUsedArgs; iIdx++ )
		{
			if( iIdx > 0 )
			{
				poAction->AppendEventArgs( " | " ) ;
			}
			
			if( iIdx < m_iNFixedArgs )
			{
				if( iHandle != -1 )
				{
					pccArgName = pCMCR->GetItemArgumentIdByPosition ( iHandle, iIdx ) ;
					poAction->AppendEventArgs( pccArgName ) ;
				}
			}
			else
			{
				poAction->AppendEventArgs( "VariableArgument" ) ;
			}

			poAction->AppendEventArgs( " = " ) ;

			vValue = m_oArgVector[ iIdx ] ;
			
			if( oVarConv.ToCharString( vValue ) == M4_SUCCESS )
			{
				if( vValue.Type == M4CL_CPP_TYPE_STRING_VAR )
				{
					poAction->AppendEventArgs( vValue.Data.PointerChar ) ;
				}
			}
		}
	}
	else
	{
		for (m4int16_t iIdx = 0; iIdx < m_iUsedArgs; iIdx++ )
		{
			if( iIdx > 0 )
			{
				poAction->AppendEventArgs( " | " ) ;
			}
			
			poAction->AppendEventArgs( "ArgumentName" ) ;

			poAction->AppendEventArgs( " = " ) ;

			vValue = m_oArgVector[ iIdx ] ;
			
			if( oVarConv.ToCharString( vValue ) == M4_SUCCESS )
			{
				if( vValue.Type == M4CL_CPP_TYPE_STRING_VAR )
				{
					poAction->AppendEventArgs( vValue.Data.PointerChar ) ;
				}
			}
		}
	}

	pChannelManager->UserActionsAudit.AddAction( poAction ) ;

	return M4_SUCCESS ;
}


m4return_t ClM4XMLMethodStatus::SetFilter (ClM4XMLContext *ai_poContext, const string &ai_sType, const string &ai_sItem, const string &ai_sMode, const string &ai_sValue)
{
  //## begin ClM4XMLMethodStatus::SetFilter%939030967.body preserve=yes

	// checks for item type
	m4return_t iResult = M4_SUCCESS;

	if (Reset(ai_poContext,ai_sItem,M4_FALSE) != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	if (ai_sMode == "add")
	{
		m_oItem.SetFilterType(ai_sType);
		m_oItem.SetFilterFormula(ai_sValue);
		m_oFilterArgVector.clear();
	}
	else
	{
		iResult = m_oItem.RemoveFilter(*ai_poContext);
	}

	return iResult;
  //## end ClM4XMLMethodStatus::SetFilter%939030967.body
}

m4return_t ClM4XMLMethodStatus::AddFilterArg (const string &ai_sMode, const string &ai_sName, const string &ai_sValue)
{
  //## begin ClM4XMLMethodStatus::AddFilterArg%939030968.body preserve=yes
	FilterArg oArg(ai_sMode,ai_sName,ai_sValue);
	m_oFilterArgVector.push_back(oArg);
	return M4_SUCCESS;
  //## end ClM4XMLMethodStatus::AddFilterArg%939030968.body
}

m4return_t ClM4XMLMethodStatus::CheckSecurity ()
{
  //## begin ClM4XMLMethodStatus::CheckSecurity%939030968.body preserve=yes
	if( m_oItem.GetM4OItem() != NULL )
	{
		if( ClM4XMLContext::GetUseFunctionalGroup(m_oItem.GetAccess()) == M4_TRUE )
		{
			// Con modelo de seguridad no se pueden ejecutar métodos de canal porque no están securizados
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_ITEM_EXECUTE, m_oItem.GetM4OItem() );
			return M4_ERROR ;
		}
	}
	else
	{
		m4pcchar_t	pccItem = m_oItem.GetItem() ;
		m4pcchar_t	pccNode = m_oItem.GetNode() ;
		ClAccess*	poAccess = m_oItem.GetAccess() ;

		if( m_poContext->CanExecute( poAccess, pccNode, pccItem ) == M4_FALSE )
		{
			// No hay permisos de ejecución
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_SEC_ITEM_EXECUTE, pccItem << pccNode << poAccess->GetpCMCR()->GetChannelId() );
			return M4_ERROR ;
		}

		if( m_poContext->MustAuthenticate( poAccess, pccNode, pccItem ) == M4_TRUE )
		{
			// Requiere autentificación
			return M4_WARNING ;
		}

		// lets check if the method is encrypted
		string sFuncGroups;
		if( m_poContext->IsEncrypted( poAccess, pccNode, pccItem, sFuncGroups ) == M4_TRUE )
		{
			m_bEncrypted = M4_TRUE;
			m_sEncryptionFuncGroups = sFuncGroups;
		}
	}
	
	// No requiere nada
	return M4_SUCCESS;
  //## end ClM4XMLMethodStatus::CheckSecurity%939030968.body
}

m4return_t ClM4XMLMethodStatus::ExecuteFilter (ClM4XMLContext *ai_poContext)
{
  //## begin ClM4XMLMethodStatus::ExecuteFilter%939030969.body preserve=yes

	// checks context
	if (m_poContext == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_METHOD_INIT );
		return M4_ERROR;
	}

	// is there filter to apply ?
	if (!m_oItem.GetFilterType().size())
	{
		return M4_SUCCESS;
	}

	// prepares parameters
	m4return_t iResult = M4_SUCCESS;
	m4int16_t iNArg = m_oFilterArgVector.size();
	m4pcchar_t *pMode=NULL, *pName=NULL, *pValue=NULL;
	if (iNArg)
	{
		pMode = new m4pcchar_t[iNArg];
		pName = new m4pcchar_t[iNArg];
		pValue = new m4pcchar_t[iNArg];
	}
	
	FilterArgIt it = m_oFilterArgVector.begin();
	for (m4int16_t i=0; i<iNArg; i++,it++)
	{
		pMode[i] = (*it).sMode.c_str();
		pName[i] = (*it).sName.c_str();
		pValue[i] = (*it).sValue.c_str();
	}

	iResult = m_oItem.ExecuteFilter(*ai_poContext,iNArg,pMode,pName,pValue);

	if (pMode != NULL)
	{
		delete [] pMode;
	}
	if (pName != NULL)
	{
		delete [] pName;
	}
	if (pValue != NULL)
	{
		delete [] pValue;
	}

	return iResult;

  //## end ClM4XMLMethodStatus::ExecuteFilter%939030969.body
}


m4return_t ClM4XMLMethodStatus::GetRoleOrg( string& ao_strRole, string& ao_strOrg )
{
	ao_strRole	= "";
	ao_strOrg	= "";

	ClAccess * poAccess = m_oItem.GetAccess();

	if( poAccess != NULL )
	{
		m4VariantType		vOrg, vRole;
		ClVariantConversor	varConv;

		ClChannel *poChannel = poAccess->GetpChannel();

		// Gets the role.
		ClProperty *poPropRole = poChannel->Property.GetProperty( M4CMPROP_CHANNEL_ID_APP_ROLE );
		if( poPropRole != NULL )
		{
			poPropRole->Get( vRole );
			if( varConv.ToCharString( vRole ) == M4_SUCCESS )
			{
				ao_strRole = (m4pchar_t)vRole;
			}
		}

		// Gets the organization.
		ClProperty *poPropOrg = poChannel->Property.GetProperty( M4CMPROP_CHANNEL_ID_ORGANIZATION );
		if( poPropOrg != NULL )
		{
			poPropOrg->Get( vOrg );
			if( varConv.ToCharString( vOrg ) == M4_SUCCESS )
			{
				ao_strOrg = (m4pchar_t)vOrg;
			}
		}
	}

	return M4_SUCCESS;
}

// Additional Declarations
  //## begin ClM4XMLMethodStatus%37EB34A9023C.declarations preserve=yes
  //## end ClM4XMLMethodStatus%37EB34A9023C.declarations



FilterArg::FilterArg ()
  //## begin FilterArg::FilterArg%945198101.hasinit preserve=no
  //## end FilterArg::FilterArg%945198101.hasinit
  //## begin FilterArg::FilterArg%945198101.initialization preserve=yes
  //## end FilterArg::FilterArg%945198101.initialization
{
  //## begin FilterArg::FilterArg%945198101.body preserve=yes
  //## end FilterArg::FilterArg%945198101.body
}

FilterArg::FilterArg (const string &ai_sMode, const string &ai_sName, const string &ai_sValue)
  //## begin FilterArg::FilterArg%939117430.hasinit preserve=no
  //## end FilterArg::FilterArg%939117430.hasinit
  //## begin FilterArg::FilterArg%939117430.initialization preserve=yes
  //## end FilterArg::FilterArg%939117430.initialization
{
  //## begin FilterArg::FilterArg%939117430.body preserve=yes
	sMode  = ai_sMode;
	sName  = ai_sName;
	sValue = ai_sValue;
  //## end FilterArg::FilterArg%939117430.body
}


// Additional Declarations
  //## begin FilterArg%37F9C78B0082.declarations preserve=yes
  //## end FilterArg%37F9C78B0082.declarations

//## begin module%37E8E4F602A4.epilog preserve=yes
//## end module%37E8E4F602A4.epilog

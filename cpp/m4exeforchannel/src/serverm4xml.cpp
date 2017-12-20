//## begin module%37DF53F801B7.cm preserve=no
//## end module%37DF53F801B7.cm

//## begin module%37DF53F801B7.cp preserve=no
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
//## end module%37DF53F801B7.cp

//## Module: Executive_M4XML%37DF53F801B7; Package body
//## Subsystem: M4ExeForChannel::XML::src%3826C5A30000
//## Source file: D:\Work\v600\m4exeforchannel\src\serverm4xml.cpp

//## begin module%37DF53F801B7.additionalIncludes preserve=no
//## end module%37DF53F801B7.additionalIncludes

//## begin module%37DF53F801B7.includes preserve=yes
#include "m4stl.hpp"
#include "m4srvefcres.hpp"
#include "clm4objservice.hpp"
#include "m4regimpl.hpp"
#include "dm.hpp"
#include "bookmark.hpp"
#include "channel.hpp"
#include "chan_knl.hpp"
#include "cldatstoiod.hpp"
#include "objdir.hpp"
#include "cllgadap.hpp"
#include "conn_provider.hpp"
#include "iexecutor.hpp"
#include "m4mdfac.hpp"
#include "medvarin.hpp"
#include "regi_smp.hpp"
#include "file_misc.hpp"
#include "file_adv.hpp"
#include "rec_tran.hpp"
#include "vdelta.hpp"
#include "ch_block.hpp"
#include "m4uuid.hpp"
#include "m4cacertdef.hpp"
#include <xercesc/parsers/SAXParser.hpp>
#include <clexecutorstatemanager.hpp>
//## end module%37DF53F801B7.includes

// DataStorages
#include <datastorages.hpp>
// Util_M4XML
#include <m4xmlutil.hpp>
// Executive_M4XML
#include <serverm4xml.hpp>
#include "m4unicode.hpp"
#include "replaceliterals.hpp"
#include "itemaccess.hpp"

// encrypt functions
m4return_t M4XMLEncryptTwoWay(string &sValue, string &sKey);
m4return_t M4XMLDecryptTwoWay(string &sValue, string &sKey);

// The record status
enum M4XML_RECORD_STATUS {
	M4XML_RECORD_STATUS_NORMAL = 0, M4XML_RECORD_STATUS_NEW, M4XML_RECORD_STATUS_MODIFIED, M4XML_RECORD_STATUS_DELETED 
};

//## begin module%37DF53F801B7.declarations preserve=no
//## end module%37DF53F801B7.declarations

//## begin module%37DF53F801B7.additionalDeclarations preserve=yes
void M4XMLDecryptTwoWay(string &sValue);
void M4XMLEncryptTwoWay(string &sValue);

#define MAX_ALIAS_SIZE 1024
#define MAX_NODE_ID_SIZE 1024
#define TRANSL_CHANNEL_ID "SCH_MULTI_VALIDATE"
#define MAX_XML_SESSION_ID 1024
#define M4OBJECT_MAPPINGS "M4OBJECT_MAPPINGS"
#define M4_USE_FUNCTIONAL_GROUP_PROPERTY "M4_USE_FUNCIONAL_GROUP"

//## end module%37DF53F801B7.additionalDeclarations

// Class ClM4XMLContext 

ClM4XMLContext::ClM4XMLContext (ClCMInitParameters &aio_oCMInitParameters)
  //## begin ClM4XMLContext::ClM4XMLContext%937383048.hasinit preserve=no
      : m_poObjectService(NULL), m_poTranslAccess(NULL), m_poTranslItemData(NULL), 
		m_iNInFileRefs(0), m_poInFileRefs(NULL), m_iNOutFileRefs(0), 
		m_poStateManager(NULL), m_iSessionId(0), m_poBaseSessionChannel(NULL), 
		m_iPersistLevel(0), m_uiLastAccess(0),
		m_poT3AccessProvider(NULL),	m_uLastAuthenticationTime(0), m_uMaxAuthenticationTime(0),
		m_bEnableNulls(M4_FALSE), m_bEnableAuth(M4_FALSE), m_bHaveFunctionalGroup(M4_FALSE), m_iReqNumber(1)
  //## end ClM4XMLContext::ClM4XMLContext%937383048.hasinit
  //## begin ClM4XMLContext::ClM4XMLContext%937383048.initialization preserve=yes
  //## end ClM4XMLContext::ClM4XMLContext%937383048.initialization
{
  //## begin ClM4XMLContext::ClM4XMLContext%937383048.body preserve=yes

	m_poExecMutex = new ClMutex(M4_TRUE);

	// Init Context
	Update(aio_oCMInitParameters);
  //## end ClM4XMLContext::ClM4XMLContext%937383048.body
}


ClM4XMLContext::~ClM4XMLContext ()
{
  //## begin ClM4XMLContext::~ClM4XMLContext%937383049.body preserve=yes
	if( m_poT3AccessProvider != NULL )
	{
		delete m_poT3AccessProvider;
	}

	ResetFileRefs();
	Reset();
  //## end ClM4XMLContext::~ClM4XMLContext%937383049.body
}


//## Other Operations (implementation)
m4return_t ClM4XMLContext::Reset ()
{
  //## begin ClM4XMLContext::Reset%942060103.body preserve=yes

	ResetTranslItem();

	if (m_poTranslAccess != NULL)
	{
		ClChannel *poChannel = m_poTranslAccess->GetpChannel();
		m_poTranslAccess->Destroy();
		poChannel->Destroy();
		m_poTranslAccess = NULL;
	}
	
	// m4return_t iResult = ReleaseLogicalConnection();
	
	ResetM4OAlias();

	// releases the execution mutex
	delete m_poExecMutex;

	return SetSession(M4_TRUE);
  //## end ClM4XMLContext::Reset%942060103.body
}

//## Other Operations (implementation)
m4return_t ClM4XMLContext::ResetM4OReferences()
{
	//## begin ClM4XMLContext::ResetM4OReferences%942060103.body preserve=yes

	m_oAliasDef.ResetM4OReferences();

	return M4_SUCCESS;

	//## end ClM4XMLContext::ResetM4OReferences%942060103.body
}

void ClM4XMLContext::ExecLock()
{
  //## begin ClM4XMLContext::ExecLock%937383050.body preserve=yes

	m_poExecMutex->Lock();

  //## end ClM4XMLContext::ExecLock%942060103.body
}


void ClM4XMLContext::ExecUnlock()
{
  //## begin ClM4XMLContext::ExecUnlock%937383050.body preserve=yes

	m_poExecMutex->Unlock();

  //## end ClM4XMLContext::ExecUnlock%942060103.body
}


string& ClM4XMLContext::GetUniqueId ()
{
  //## begin ClM4XMLContext::GetUniqueId%937383050.body preserve=yes

	return m_sUUID;

  //## end ClM4XMLContext::GetUniqueId%942060103.body
}

void ClM4XMLContext::InitUniqueId ()
{
  //## begin ClM4XMLContext::InitUniqueId%937383050.body preserve=yes

	// blocks for all executors
	ClMutBlock oExecutiveMutex(ClExecutiveForOE::GetExecuteMutex());

	if (m_sUUID.length() > 0)
	{
		return ;
	}

	// retrieves session channel property
	ClChannel *poBaseSessionChannel = GetBaseSessionChannel();
	if (poBaseSessionChannel == NULL)
	{
		// the context is not initialized yet (deserialization)
		return;
	}

	m4VariantType oValue;
	m4return_t iResult = poBaseSessionChannel->Property.GetPropValue(M4_SESSION_JSAPI_UUID_PROPERTY, oValue);

	// by default the value came from user
	string sUUID = "DEFAULT";
	if (iResult == M4_SUCCESS)
	{
		m4pchar_t pcValue = (m4pchar_t)oValue;
		if (pcValue != NULL)
		{
			m_sUUID = pcValue;
		}
	}

	if (m_sUUID.length() > 0)
	{
		return;
	}

	// gets a new id
	{
		m4pchar_t pcUUID = new m4char_t[M4CL_M4XML_CONTEXT_UUID_KEY_SIZE];
		m4return_t iRet = GetUUID(pcUUID, M4CL_M4XML_CONTEXT_UUID_KEY_SIZE);
		if (iRet != M4_SUCCESS)
		{
			delete []pcUUID;
			DUMP_CHLOG_ERROR ( M4_ERR_XML_GENERATE_UUID );
			return;
		}

		// assign uuid
		m_sUUID = pcUUID;
		delete []pcUUID;
	}

	// sets session channel property
	iResult = poBaseSessionChannel->Property.SetOrNewProperty(M4_SESSION_JSAPI_UUID_PROPERTY, (const m4pchar_t)m_sUUID.c_str(), M4_PROP_LN4_STATE);
	if(iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_ASSIGN_UUID );
		return;
	}

  //## end ClM4XMLContext::InitUniqueId%942060103.body
}

static string s_sEncryptPrefix("[E]");

m4return_t ClM4XMLContext::Encrypt(string &aio_sValue, m4bool_t ai_bIsNull, string& ai_sFuncGroups) {

	// encrypts the original value
	string sInputValue = (ai_bIsNull == M4_TRUE ? "1" : ( "0" + aio_sValue ) );
	string sValue = s_sEncryptPrefix + ai_sFuncGroups + ";" + sInputValue;
	if ( M4XMLEncryptTwoWay(sValue, GetUniqueId()) != M4_SUCCESS )
	{ 
		return M4_ERROR;
	}

	// composes the output
	aio_sValue = s_sEncryptPrefix + sValue + ";" + sInputValue; 
	return M4_SUCCESS;
}

m4return_t ClM4XMLContext::DeCrypt(string &aio_sValue, m4bool_t &ao_bIsNull, string& ai_sFuncGroups) {
	
	ao_bIsNull = M4_FALSE;
	string sValue = aio_sValue;

	// checks the encrypted prefix
	if (sValue.find(s_sEncryptPrefix) != 0) {
		aio_sValue = string();
		return M4_ERROR;
	}
	
	// gets the encrypted value
	sValue = sValue.substr( s_sEncryptPrefix.size() );
	size_t sepIndex = sValue.find(";");
	if (sepIndex < 0)
	{
		aio_sValue = string();
		return M4_ERROR;
	}
	sValue = sValue.substr(0, sepIndex);

	// decrypts
	if ( M4XMLDecryptTwoWay(sValue, GetUniqueId()) != M4_SUCCESS )
	{
		return M4_ERROR;
	}
	
	// removes encrypt prefix
	string sBuffer = sValue;
	if (sBuffer.find(s_sEncryptPrefix) != 0) {
		aio_sValue = string();
		return M4_ERROR;
	}
	sBuffer = sBuffer.substr(s_sEncryptPrefix.size());

	// ok, now we have to check if we have common groups
	mFunctionalGroupsSet_t oGroupsSet;

	// lets add the encription list
	size_t iGroupsSepIndex = sValue.find(";", s_sEncryptPrefix.size() + 1);
	if ( iGroupsSepIndex == string::npos )
	{
		aio_sValue = string();
		return M4_ERROR;
	}

	sBuffer = sBuffer.substr(0, sBuffer.find(";"));
	while ( sBuffer.empty() == M4_FALSE )
	{
		size_t iIndex = sBuffer.find(",");
		if (iIndex == string::npos) {
			oGroupsSet.insert(sBuffer);
			break;
		}
		string sToken = sBuffer.substr(0, iIndex);
		oGroupsSet.insert(sToken);
		sBuffer = sBuffer.substr(iIndex + 1);
	}

	// ok, now we have to check if some of the current group is in the list
	sBuffer = ai_sFuncGroups;
	m4bool_t bFound = M4_FALSE;
	while ( sBuffer.empty() == M4_FALSE )
	{
		size_t iIndex = sBuffer.find(",");
		if (iIndex == string::npos) {
			bFound = oGroupsSet.find(sBuffer) != oGroupsSet.end();
			break;
		}

		string sToken = sBuffer.substr(0, iIndex);
		bFound = oGroupsSet.find(sToken) != oGroupsSet.end();
		if (bFound == M4_TRUE)
		{
			break;
		}
		sBuffer = sBuffer.substr(iIndex + 1);
	}

	if ( bFound == M4_FALSE )
	{
		// oh oh.... we're in an invalid context
		aio_sValue = string();
		return M4_ERROR;
	}

	// checks null value
	sBuffer = sValue.substr(iGroupsSepIndex + 1, 1);
	ao_bIsNull = sBuffer == "1" ? M4_TRUE : M4_FALSE;

	// everything ok, we can update the return value
	aio_sValue = ao_bIsNull == M4_TRUE ? "" : sValue.substr(iGroupsSepIndex + 2);
	return M4_SUCCESS;
}

m4return_t ClM4XMLContext::CheckActiveAuthentication()
{
  //## begin ClM4XMLContext::CheckActiveAuthentication%937383050.body preserve=yes

	m4uint32_t uLastAuthTime = GetLastAuthenticationTime();
	m4uint32_t uCurrentTime = m4uint32_t(ClPrecisionTicker::GetTimeTick() / 1000);

	// checks if authenticated 
	if (uLastAuthTime != 0)
	{
		// checks delta
		m4uint32_t uDelta = uCurrentTime - uLastAuthTime;
		m4uint32_t uMaxDelta = GetMaxAuthenticationTime();

		if (uMaxDelta >= uDelta)
		{
			return M4_SUCCESS;
		}
	}

	// time expired, so lets try to validate the password
	if (m_sAuthPassword.empty())
	{
		// we can't validate
		return M4_WARNING;
	}

	ClM4ObjService *pOBJService = GetObjectService ();
	if (pOBJService == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_AUTH_OBJ_SERVICE );
		return M4_ERROR;
	}

	ClChannelManager *poCM = pOBJService->GetpChannelManager();
	if (poCM == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_AUTH_CHANNEL_MANAGER );
		return M4_ERROR;
	}

	ClChannel *poChannel = NULL;
	ClAccess *poAccess = NULL;

	ClM4XMLObjectCleaner oCleaner(poChannel, poAccess);
	
	m4return_t iResult = poCM->CreateChannel(poChannel);
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_CREATE_CHANNEL, "SSC_APPUSER" );
		return M4_ERROR;
	}

	// builds the channel
	iResult = poChannel->BuildFromId("SSC_APPUSER");
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_BUILD_CHANNEL, "SSC_APPUSER" );
		return M4_ERROR;
	}

	iResult = poChannel->CreateAccess(poAccess);
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_CREATE_ACCESS, "SSC_APPUSER" );
		return M4_ERROR;
	}

	ClNode *poNode = &( poAccess->Node["SSC_APPUSER"] ) ;

	if( poNode == NULL )
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_GET_NODE, "SSC_APPUSER" << "SSC_APPUSER" );
		return M4_ERROR;
	}

	IStackInterface *poStack = &( poAccess->GetpExecutor()->Stack ) ;

	if( poStack == NULL )
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_GET_EXECUTOR, "SSC_APPUSER" );
		return M4_ERROR;
	}

	m4VariantType	vValue;

	// User id. We first try the debug user. If found we'll use it. If not, we use the standard user.
	m4pcchar_t	pccUser = pOBJService->GetpLogonAdaptor()->GetDebugUser();
	if ( pccUser == NULL || strlen ( pccUser ) == 0 )
	{
		pccUser = pOBJService->GetpLogonAdaptor()->GetUsername();
	}
	vValue.SetStringRef( (char *const) pccUser );
	iResult = poStack->Push( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_PUSH_ARG, "SSC_APPUSER" );
		return M4_ERROR;
	}

	// Password one way. Not needed
	vValue.SetNull();
	iResult = poStack->Push( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_PUSH_ARG, "SSC_APPUSER" );
		return M4_ERROR;
	}

	// Password two way
	vValue.SetStringRef( (char *const) m_sAuthPassword.c_str() );
	iResult = poStack->Push( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_PUSH_ARG, "SSC_APPUSER" );
		return M4_ERROR;
	}

	// Logon mechanism. Output
	vValue.SetNull();
	iResult = poStack->Push( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_PUSH_ARG, "SSC_APPUSER" );
		return M4_ERROR;
	}

	// Real user. Output
	vValue.SetNull();
	iResult = poStack->Push( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_PUSH_ARG, "SSC_APPUSER" );
		return M4_ERROR;
	}

	// Impersonated user. Output
	vValue.SetNull();
	iResult = poStack->Push( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_PUSH_ARG, "SSC_APPUSER" );
		return M4_ERROR;
	}

	// now we execute USER_AUTHENTICATION
	iResult = poNode->RecordSet.Item[ "USER_AUTHENTICATION" ].Call( NULL, 6 ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_EXECUTE_CALL, "USER_AUTHENTICATION" << "SSC_APPUSER" << iResult );
		return M4_ERROR;
	}

	// Logon mechanism
	iResult = poStack->Pop( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_EXECUTE_LN4, "USER_AUTHENTICATION" << "SSC_APPUSER" << iResult );
		return M4_ERROR;
	}

	// Real user
	iResult = poStack->Pop( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_EXECUTE_LN4, "USER_AUTHENTICATION" << "SSC_APPUSER" << iResult );
		return M4_ERROR;
	}

	// Impersonated user
	iResult = poStack->Pop( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_EXECUTE_LN4, "USER_AUTHENTICATION" << "SSC_APPUSER" << iResult );
		return M4_ERROR;
	}

	// Result
	iResult = poStack->Pop( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_EXECUTE_LN4, "USER_AUTHENTICATION" << "SSC_APPUSER" << iResult );
		return M4_ERROR;
	}

	if( vValue.Type == M4CL_CPP_TYPE_NUMBER )
	{
		iResult = m4return_t( vValue.Data.DoubleData ) ;

		if( iResult == -10 || iResult == -21 )
		{
			return M4_WARNING;
		}
	}
	else
	{
		iResult = M4_ERROR ;
	}

	if( iResult != M4_SUCCESS )
	{
		return M4_ERROR;
	}

	SetLastAuthenticationTime( uCurrentTime );

	return M4_SUCCESS;
  //## end ClM4XMLContext::CheckActiveAuthentication%942060103.body
}

m4bool_t	ClM4XMLContext::CanRead( ClAccess *ai_poAccess, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem )
{
	if ( GetUseFunctionalGroup( ai_poAccess ) == M4_FALSE )
	{
		return M4_TRUE;
	}

	if( m_poT3AccessProvider == NULL )
	{
		m_poT3AccessProvider = new ClT3AccessProvider() ;
	}

	return m_poT3AccessProvider->CanRead( ai_poAccess->GetpChannel(), ai_pccNode, ai_pccItem ) ;
}

m4bool_t	ClM4XMLContext::CanWrite( ClAccess *ai_poAccess, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem )
{
	if ( GetUseFunctionalGroup( ai_poAccess ) == M4_FALSE )
	{
		return M4_TRUE;
	}

	if( m_poT3AccessProvider == NULL )
	{
		m_poT3AccessProvider = new ClT3AccessProvider() ;
	}

	return m_poT3AccessProvider->CanWrite( ai_poAccess->GetpChannel(), ai_pccNode, ai_pccItem ) ;
}

m4bool_t	ClM4XMLContext::CanExecute( ClAccess *ai_poAccess, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem )
{
	if ( GetUseFunctionalGroup( ai_poAccess ) == M4_FALSE )
	{
		return M4_TRUE;
	}

	if( m_poT3AccessProvider == NULL )
	{
		m_poT3AccessProvider = new ClT3AccessProvider() ;
	}

	return m_poT3AccessProvider->CanExecute( ai_poAccess->GetpChannel(), ai_pccNode, ai_pccItem ) ;
}

m4bool_t	ClM4XMLContext::MustAuthenticate( ClAccess *ai_poAccess, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem )
{
	if ( GetUseFunctionalGroup( ai_poAccess ) == M4_FALSE )
	{
		return M4_FALSE;
	}

	// lets check if we have the authentication enabled
	if (GetEnableAuth() == M4_FALSE)
	{
		return M4_FALSE;
	}

	if( m_poT3AccessProvider == NULL )
	{
		m_poT3AccessProvider = new ClT3AccessProvider() ;
	}

	return m_poT3AccessProvider->MustAuthenticate( ai_poAccess->GetpChannel(), ai_pccNode, ai_pccItem ) ;
}
	
m4bool_t	ClM4XMLContext::IsEncrypted( ClAccess *ai_poAccess, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, string &ao_sFuncGroups) 
{
	if ( GetUseFunctionalGroup( ai_poAccess ) == M4_FALSE )
	{
		return M4_FALSE;
	}

	if( m_poT3AccessProvider == NULL )
	{
		m_poT3AccessProvider = new ClT3AccessProvider() ;
	}

	return m_poT3AccessProvider->IsEncrypted( ai_poAccess->GetpChannel(), ai_pccNode, ai_pccItem, ao_sFuncGroups ) ;
}
	
m4uint32_t	ClM4XMLContext::GetSecurityMask( ClAccess *ai_poAccess, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem ) 
{
	if ( GetUseFunctionalGroup( ai_poAccess ) == M4_FALSE )
	{
		return 0;
	}

	if( m_poT3AccessProvider == NULL )
	{
		m_poT3AccessProvider = new ClT3AccessProvider() ;
	}

	return m_poT3AccessProvider->GetSecurityMask( ai_poAccess->GetpChannel(), ai_pccNode, ai_pccItem ) ;
}
	
m4return_t ClM4XMLContext::ProcAlias (const string& ai_sAlias, const string& ai_sChannelId, const string& ai_sMode, const string& ai_sPreserve, const string& ai_sRemove, const string& ai_sFind, const string& ai_sMDInfoLevel, const string& ai_sUseFunctionalGroup)
{
  //## begin ClM4XMLContext::ProcAlias%937383050.body preserve=yes

	m_oAliasDef.Reset();
	m_oAliasDef.sName = ai_sAlias;
	m_oAliasDef.sM4OId = ai_sChannelId;
	m_oAliasDef.sMode = ai_sMode;
	m_oAliasDef.sPreserve = ai_sPreserve;
	m_oAliasDef.sRemove = ai_sRemove;
	m_oAliasDef.sFind = ai_sFind;
	m_oAliasDef.sMDInfoLevel = ai_sMDInfoLevel;
	m_oAliasDef.sUseFunctionalGroup = ai_sUseFunctionalGroup;

	return M4_SUCCESS;
  //## end ClM4XMLContext::ProcAlias%937383050.body
}

ClM4XMLCurrent * ClM4XMLContext::GetCurrentByAlias (const string& ai_sAlias)
{
  //## begin ClM4XMLContext::GetCurrentByAlias%937383071.body preserve=yes
	M4OAliasMapIt it = m_oAliasMap.find(ai_sAlias);
	if (it == m_oAliasMap.end())
	{
		return NULL;
	}

	return (*it).second;
  //## end ClM4XMLContext::GetCurrentByAlias%937383071.body
}

void ClM4XMLContext::Indent (ostream &ao_oStream, m4int16_t ai_iIndent)
{
  //## begin ClM4XMLContext::Indent%937826954.body preserve=yes
	while (ai_iIndent--)
		ao_oStream << " ";
  //## end ClM4XMLContext::Indent%937826954.body
}

m4return_t ClM4XMLContext::AddTranslType (const string &ai_sType, const string &ai_sMethod, const string &ai_sResultItems)
{
  //## begin ClM4XMLContext::AddTranslType%939289642.body preserve=yes
	ClAccess *poAccess = GetTranslAccess();
	if (poAccess == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_TRANSL_ACCESS );
		return M4_ERROR;
	}

	ClM4XMLItem oItem(ai_sMethod);

	TranslArgs oArgs(poAccess,"INTERFACE","ADD_VALIDATION_TYPE");

	oArgs.Add("VALIDATION_TYPE",ai_sType);
	oArgs.Add("VALIDATE_CHANNEL","");
	oArgs.Add("NODE_IN_VALIDATE_CHANNEL","");
	oArgs.Add("VALIDATION_CHANNEL",oItem.GetM4OAlias()==NULL?"":oItem.GetM4OAlias());
	oArgs.Add("NODE_IN_VALIDATION_CHANNEL",oItem.GetNode()==NULL?"":oItem.GetNode());
	oArgs.Add("VALIDATION_METHOD",oItem.GetItem()==NULL?"":oItem.GetItem());
	oArgs.Add("ITEMS_IN_RESULT",ai_sResultItems);

	if (ExecuteTransl(oArgs,M4_FALSE) != M4_SUCCESS)
		return M4_ERROR;

	// gets the result from the stack
	m4VariantType vResult, *vvResult=&vResult;
	m4return_t iResult = PopStack(poAccess,1,&vvResult);

	if (iResult != M4_SUCCESS || vResult.Type != M4CL_CPP_TYPE_NUMBER || m4double_t(vResult))
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_TRANSL_POP, "INTERFACE" );
		iResult = M4_ERROR;
	}

	return iResult;
  //## end ClM4XMLContext::AddTranslType%939289642.body
}

m4return_t ClM4XMLContext::AddTranslItem (const string &ai_sType, const string &ai_sItem)
{
  //## begin ClM4XMLContext::AddTranslItem%939289643.body preserve=yes
	ClM4XMLItem oItem(ai_sItem);
	
	// gets the handle
	if (oItem.Move(*this,M4_FALSE) != M4_SUCCESS)
		return M4_ERROR;

	// checks handle
	if (oItem.GetHandle() <= 0 || oItem.GetM4OAlias() == NULL)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_TRANSL_ADD_ITEM, ai_sItem.c_str() );
		return M4_ERROR;
	}

	// adds the item
	TranslItemPK oPK(oItem.GetM4OAlias(),oItem.GetHandle());
	m_oTranslItems.insert(TranslItemsMap::value_type(oPK,TranslItemData(ai_sType)));
	TranslItemsIt it = m_oTranslItems.find(oPK);
	if (it == m_oTranslItems.end())
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_TRANSL_ITEM_INTERNAL );
		return M4_ERROR;
	}

	m_poTranslItemData = &(*it).second;

	return M4_SUCCESS;

  //## end ClM4XMLContext::AddTranslItem%939289643.body
}

m4return_t ClM4XMLContext::ExecuteTransl (TranslArgs &ai_oArgs, m4bool_t ai_bDoReset)
{
  //## begin ClM4XMLContext::ExecuteTransl%939289645.body preserve=yes
	ClAccess *poAccess = GetTranslAccess();
	if (poAccess == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_TRANSL_ACCESS );
		return M4_ERROR;
	}

	// gets the Node
	ClNode *poNode = poAccess->Node.Get(ai_oArgs.GetNode().c_str());
	if (poNode == NULL)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_TRANSL_EXEC_NODE, ai_oArgs.GetNode().c_str() );
		return M4_ERROR;
	}

	// pushes the args
	m4int16_t iNArgs = ai_oArgs.Size();
	for (m4int16_t i=0; i<iNArgs; i++)
		poAccess->GetpExecutor()->Stack.Push(m4VariantType((m4pchar_t)ai_oArgs.GetArg(i)));

	ClAccessRecordSet *poRS = &poNode->RecordSet;

	m4return_t iResult = poRS->Item[ai_oArgs.GetItem().c_str()].Call();
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_CONTEXT_EXECUTE_TRANSL, ai_oArgs.GetItem().c_str() );
	}

	// ignores returned arguments
	if (ai_bDoReset)
		poAccess->GetpExecutor()->Stack.Reset();

	return iResult;

  //## end ClM4XMLContext::ExecuteTransl%939289645.body
}

ClAccess * ClM4XMLContext::GetTranslAccess ()
{
  //## begin ClM4XMLContext::GetTranslAccess%939289647.body preserve=yes
	if (m_poTranslAccess != NULL)
		return m_poTranslAccess;

	if (m_poObjectService == NULL)
		return NULL;

	ClChannel *poChannel = NULL;
	ClChannelManager *poChannelManager = m_poObjectService->GetpChannelManager();
	m4return_t iResult = poChannelManager->CreateChannel(poChannel);
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_TRANSL_CHANNEL );
		return NULL;
	}

	// builds the channel
	poChannel->CsType.Set(m4VariantType(M4CL_CSTYPE_BOTH));
	iResult = poChannel->BuildFromId(TRANSL_CHANNEL_ID);
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_CONTEXT_TRANSL_BUILD, TRANSL_CHANNEL_ID );

		if (poChannel->Destroy() != M4_SUCCESS)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_TRANSL_BUILD_DESTROY, TRANSL_CHANNEL_ID );
		}

		return NULL;
	}

	m_poTranslAccess = poChannel->CreateAccess();

	return m_poTranslAccess;
  //## end ClM4XMLContext::GetTranslAccess%939289647.body
}

m4return_t ClM4XMLContext::AddTranslItemArg (const string &ai_sName, const string &ai_sValue)
{
  //## begin ClM4XMLContext::AddTranslItemArg%939372143.body preserve=yes
	if (m_poTranslItemData == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_ADD_TRANSL_ITEM );
		return M4_ERROR;
	}
	m_poTranslItemData->AddArg(ai_sName,ai_sValue);
	return M4_SUCCESS;
  //## end ClM4XMLContext::AddTranslItemArg%939372143.body
}

void ClM4XMLContext::ResetTranslItem ()
{
  //## begin ClM4XMLContext::ResetTranslItem%939372144.body preserve=yes
	m_poTranslItemData = NULL;
  //## end ClM4XMLContext::ResetTranslItem%939372144.body
}

m4return_t ClM4XMLContext::Translate (const string& ai_sM4OAlias, m4int32_t ai_iHandle, const m4VariantType& ai_vValue, m4VariantType& ao_vValueOut, m4bool_t& ao_bFound)
{
  //## begin ClM4XMLContext::Translate%939372148.body preserve=yes

	// scans the map
	TranslItemsIt it = m_oTranslItems.find(TranslItemPK(ai_sM4OAlias,ai_iHandle));
	
	if (it == m_oTranslItems.end())
	{
		ao_bFound = M4_FALSE;
		return M4_SUCCESS;
	}

	// gets the current properties
	m4VariantType vOrg, vRole, vLang;
	ClM4XMLCurrent *poCurrent = GetCurrentByAlias(ai_sM4OAlias);
	if (poCurrent == NULL)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_TRANSL_NO_ALIAS, ai_sM4OAlias.c_str() );
		return M4_ERROR;
	}

	if (poCurrent->poAccess == NULL)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_TRANSL_NO_ACCESS, ai_sM4OAlias.c_str() );
		return M4_ERROR;
	}

	ClChannel *poChannel = poCurrent->poAccess->GetpChannel();
	ClVariantConversor varConv;
	
	// organization
	{
		ClProperty *poPropOrg = poChannel->Property.GetProperty(M4CMPROP_CHANNEL_ID_ORGANIZATION);
		if (poPropOrg != NULL)
			poPropOrg->Get(vOrg);

		if (varConv.ToCharString(vOrg) != M4_SUCCESS)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_TRANSL_NO_CONVERT, M4CMPROP_CHANNEL_ID_ORGANIZATION );
			return M4_ERROR;
		}
	}

	// role
	{
		ClProperty *poPropOrg = poChannel->Property.GetProperty(M4CMPROP_CHANNEL_ID_APP_ROLE);
		if (poPropOrg != NULL)
			poPropOrg->Get(vRole);

		if (varConv.ToCharString(vRole) != M4_SUCCESS)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_TRANSL_NO_CONVERT, M4CMPROP_CHANNEL_ID_APP_ROLE );
			return M4_ERROR;
		}
	}

	// language
	{
		ClProperty *poPropLang = poChannel->Property.GetProperty(M4CMPROP_CHANNEL_DATA_LANGUAGE);
		if (poPropLang != NULL)
			poPropLang->Get(vLang);

		if (varConv.ToCharString(vLang) != M4_SUCCESS)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_TRANSL_NO_CONVERT, M4CMPROP_CHANNEL_DATA_LANGUAGE );
			return M4_ERROR;
		}
	}

	// we have to translate
	ao_bFound = M4_TRUE;

	// gets the translation access
	ClAccess *poAccess = GetTranslAccess();
	if (poAccess == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_TRANSL_ACCESS );
		return M4_ERROR;
	}

	TranslItemDataSet *poDataSet = &(*it).second.m_oArgSet;
	TranslItemDataIt itData = poDataSet->begin();
	m4int16_t iNArgs = poDataSet->size();

	if (iNArgs < 1)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_TRANSL_MIN_ARGS );
		return M4_ERROR;
	}

	m4int16_t i=0;
	string sArgs,sValue;
	for (i=0; i< iNArgs; i++, itData++)
	{
		if (i)
			sArgs += ",";
		sArgs += (*itData).m_sName;
		sArgs += ",";

		if ((*itData).m_sValue.size())
		{
			ClM4XMLItem oItem((*itData).m_sValue);
			if (oItem.GetValue(*this,sValue) != M4_SUCCESS)
				return M4_ERROR;
		}
		else 
			continue;

		sArgs += sValue;
	}

	// executes the method to add the PK
	m4VariantType vResult, vValueOut2, *vvResult[] = {&vResult}, 
		*vvOut[] = {&ao_vValueOut, &vValueOut2, &vResult} ;

	m4return_t iResult = M4_SUCCESS;
	
	// sets the organization
	if (iResult == M4_SUCCESS)
	{
		TranslArgs oArgs(poAccess,"INTERFACE","SET_SOCIETY");
		
		oArgs.Add("SOCIETY_ID", (m4pchar_t)vOrg);
		
		if (ExecuteTransl(oArgs,M4_FALSE) != M4_SUCCESS)
			return M4_ERROR;

		if (PopStack(poAccess,0,vvResult) != M4_SUCCESS)
			iResult = M4_ERROR;
	}
	
	// sets the role
	if (iResult == M4_SUCCESS)
	{
		TranslArgs oArgs(poAccess,"INTERFACE","SET_ROLE");
		
		oArgs.Add("ROLE_ID", (m4pchar_t)vRole);
		
		if (ExecuteTransl(oArgs,M4_FALSE) != M4_SUCCESS)
			return M4_ERROR;

		if (PopStack(poAccess,0,vvResult) != M4_SUCCESS)
			iResult = M4_ERROR;
	}

	// sets the language
	if (iResult == M4_SUCCESS)
	{
		TranslArgs oArgs(poAccess,"INTERFACE","SET_LANGUAGE");
		
		oArgs.Add("LANGUAGE_ID", (m4pchar_t)vLang);
		
		if (ExecuteTransl(oArgs,M4_FALSE) != M4_SUCCESS)
			return M4_ERROR;

		if (PopStack(poAccess,0,vvResult) != M4_SUCCESS)
			iResult = M4_ERROR;
	}

	// inserts the PK
	if (iResult == M4_SUCCESS)
	{
		TranslArgs oArgs(poAccess,"INTERFACE","ADD_PENDING_PK");
		
		oArgs.Add("VALIDATION_TYPE",(*it).second.m_sType);
		oArgs.Add("PK",sArgs);
		
		if (ExecuteTransl(oArgs,M4_FALSE) != M4_SUCCESS)
			return M4_ERROR;

		if (PopStack(poAccess,1,vvResult) != M4_SUCCESS)
			iResult = M4_ERROR;
		else
			if (vResult.Type != M4CL_CPP_TYPE_NUMBER || m4double_t(vResult))
			{
				DUMP_CHLOG_ERROR ( M4_ERR_XML_TRANSL_INVALID_RESULT );
				iResult = M4_ERROR;
			}
	}

	// fires validation
	if (iResult == M4_SUCCESS)
	{
		TranslArgs oArgs(poAccess,"INTERFACE","FIRE_VALIDATE");

		oArgs.Add("EXECUTE_LOADS","0");
		oArgs.Add("LOAD_METHOD","");
		oArgs.Add("LOAD_METHOD_PARAMS","");

		if (ExecuteTransl(oArgs) != M4_SUCCESS)
			return M4_ERROR;
	}

	// gets the value
	if (iResult == M4_SUCCESS)
	{
		// old style
		// TranslArgs oArgs(poAccess,"INTERFACE","GET_RESULTS_BY_PK");
		TranslArgs oArgs(poAccess,"INTERFACE","GET_RESULTS_BY_XML");

		oArgs.Add("VALIDATION_TYPE",(*it).second.m_sType);
		oArgs.Add("PK",sArgs);
		oArgs.Add("RESULTS_VALUES","");
		oArgs.Add("RESULTS_M4TYPES","");

		if (ExecuteTransl(oArgs,M4_FALSE) != M4_SUCCESS)
			return M4_ERROR;
	}

	// old style
	// if (PopStack(poAccess,2,vvResult) != M4_SUCCESS)
	if (PopStack(poAccess,3,vvOut) != M4_SUCCESS)
		iResult = M4_ERROR;
	else
		if (vResult.Type != M4CL_CPP_TYPE_NUMBER || m4double_t(vResult))
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_TRANSL_INVALID_RESULT );
			iResult = M4_ERROR;
		}
	
	return iResult;
  //## end ClM4XMLContext::Translate%939372148.body
}

string ClM4XMLContext::GetInFileName (m4int32_t ai_iIndex)
{
  //## begin ClM4XMLContext::GetInFileName%939889206.body preserve=yes
	if (ai_iIndex < 0 || ai_iIndex >= m_iNInFileRefs)
		return "";

	return m_poInFileRefs[ai_iIndex]->GetFileName();
  //## end ClM4XMLContext::GetInFileName%939889206.body
}

m4return_t ClM4XMLContext::ProcInFileRefs (M4DataStorage* ai_poDataStorage)
{
  //## begin ClM4XMLContext::ProcInFileRefs%939889207.body preserve=yes
	// gets the number of external references
	m4int32_t iNBlocks = ai_poDataStorage->Blocks() - 1;

	// reset members
	m_iNInFileRefs = 0;
	if (m_poInFileRefs != NULL) {
		delete [] m_poInFileRefs;
		m_poInFileRefs = NULL;
	}
	
	// check input error
	if (iNBlocks < 0)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_PROC_IN_FILES_INVALID );
		return M4_ERROR;
	}

	// checks any extra block
	if (iNBlocks == 0)
	{
		return M4_SUCCESS;
	}

	ClDataUnitInterface *poUnit = NULL;
	
	ai_poDataStorage->InitializeIteration();

	if (ai_poDataStorage->GetNext(poUnit) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_INPUT_TRUNC );
		return M4_ERROR;
	}

	// generates a new vector
	m_poInFileRefs = new ClFileUnit * [iNBlocks];
	
	for (m4int32_t i=0; i<iNBlocks; i++)
	{
		if (ai_poDataStorage->GetNext(poUnit) != M4_SUCCESS)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_INPUT_TRUNC );
			return M4_ERROR;
		}
		
		// ignore non file unit type
		// fix bug 0106278
		if (poUnit->GetType() != M4_FILE_UNIT)
		{
			continue;
		}
		
		m_poInFileRefs[m_iNInFileRefs++] = (ClFileUnit*)poUnit;
	}

	// optimizes memory, since the last elements could be empty
	if (m_iNInFileRefs < iNBlocks) 
	{
		ClFileUnit **poTempBlocks = NULL;
		
		if (m_iNInFileRefs > 0) 
		{
			poTempBlocks = new ClFileUnit * [m_iNInFileRefs];
		}

		for (m4int32_t i=0; i<m_iNInFileRefs; i++) 
		{
			poTempBlocks[i] = m_poInFileRefs[i];
		}

		// replaces the old vector
		delete [] m_poInFileRefs;
		m_poInFileRefs = poTempBlocks;
	}

	return M4_SUCCESS;
  //## end ClM4XMLContext::ProcInFileRefs%939889207.body
}

m4int32_t ClM4XMLContext::AddOutFileRef (M4DataStorage *ao_poDataStorage, m4pcchar_t ai_pcFileName, string& ao_sFileExtension, m4uint32_t& ao_lFileSize)
{
  //## begin ClM4XMLContext::AddOutFileRef%939889210.body preserve=yes

	// checks arguments
	if (ai_pcFileName == NULL || !strlen(ai_pcFileName))
		return -1;

	GetFileInfo(ai_pcFileName, ao_sFileExtension, ao_lFileSize);

	if (GetEnableBlobFiles() == M4_FALSE) 
	{
		return -1;
	}

	ClFileUnit *poFileUnit = NULL;

	if (ao_poDataStorage->GetNewFileUnit(poFileUnit) != M4_SUCCESS)
		return -1;

	// updates counter
	m_iNOutFileRefs++;

	if (poFileUnit->SetFile((m4pchar_t)ai_pcFileName) != M4_SUCCESS)
		return -1;

	return m_iNOutFileRefs - 1;

  //## end ClM4XMLContext::AddOutFileRef%939889210.body
}

m4return_t ClM4XMLContext::GetFileInfo(m4pcchar_t ai_pcFileName, string& ao_sExtension, m4uint32_t& ao_lFileLen)
{
  //## begin ClM4XMLContext::GetFileInfo%939889210.body preserve=yes

	// gets the extension
	m4char_t pcExtension[4096];
	m4return_t iRes = ClFileName(ai_pcFileName).GetFileExtension(pcExtension);
	if (iRes != M4_SUCCESS)
	{
		return M4_ERROR;
	}
	ao_sExtension = pcExtension;

	// gets the file size
	ClFileState oFileState(ai_pcFileName);

	if (oFileState.IsFile() == M4_FALSE)
	{
		return M4_ERROR;
	}

	ao_lFileLen = oFileState.GetSize();
	return M4_SUCCESS;
  //## end ClM4XMLContext::GetFileInfo%939889210.body
}

m4uint32_t ClM4XMLContext::GetMaxAuthenticationTime( )
{
	// checks if we have the value already loaded
	if (m_uMaxAuthenticationTime != 0) 
	{
		return m_uMaxAuthenticationTime;
	}

	// ok, lets loade the value from repository
	string sParam = LoadParamValue("THINCLIENT", "PORTAL_PARAM", "M4JS_SESSION_EXPIRATION");

	m_uMaxAuthenticationTime = atol(sParam.c_str());

	if (m_uMaxAuthenticationTime == 0) 
	{

		// returns the default expiration time, in this case, 10 years
		m_uMaxAuthenticationTime = m4uint32_t(86400)*365*10;
	}

	return m_uMaxAuthenticationTime;
}

// Loads a parameter from repository
string ClM4XMLContext::LoadParamValue(m4pcchar_t ai_pcApplication, m4pcchar_t ai_pcSection, m4pcchar_t ai_pcKey)
{

	ClM4ObjService *pOBJService = GetObjectService ();
	if (pOBJService == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_AUTH_OBJ_SERVICE );
		return "";
	}

	ClChannelManager *poCM = pOBJService->GetpChannelManager();
	if (poCM == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_AUTH_CHANNEL_MANAGER );
		return "";
	}

	ClChannel *poChannel = NULL;
	ClAccess *poAccess = NULL;

	ClM4XMLObjectCleaner oCleaner(poChannel, poAccess);
	
	m4return_t iResult = poCM->CreateChannel(poChannel);
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_CREATE_CHANNEL, "SAV_PARAMS" );
		return "";
	}

	// builds the channel
	iResult = poChannel->BuildFromId("SAV_PARAMS");
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_BUILD_CHANNEL, "SAV_PARAMS" );
		return "";
	}

	iResult = poChannel->CreateAccess(poAccess);
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_CREATE_ACCESS, "SAV_PARAMS" );
		return "";
	}

	ClNode *poNode = &( poAccess->Node["SAV_PARAMS"] ) ;

	if( poNode == NULL )
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_GET_NODE, "SAV_PARAMS" << "SAV_PARAMS" );
		return "";
	}

	IStackInterface *poStack = &( poAccess->GetpExecutor()->Stack ) ;

	if( poStack == NULL )
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_GET_EXECUTOR, "SAV_PARAMS" );
		return "";
	}

	m4VariantType	vValue ;
	vValue.Type = M4CL_CPP_TYPE_STRING_VAR ;
	vValue.Data.PointerChar = (m4pchar_t) ai_pcApplication ;
	iResult = poStack->Push( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_PUSH_ARG, "SAV_PARAMS" );
		return "";
	}

	vValue.Type = M4CL_CPP_TYPE_STRING_VAR ;
	vValue.Data.PointerChar = (m4pchar_t) ai_pcSection ;
	iResult = poStack->Push( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_PUSH_ARG, "SAV_PARAMS" );
		return "";
	}

	vValue.Type = M4CL_CPP_TYPE_STRING_VAR ;
	vValue.Data.PointerChar = (m4pchar_t) ai_pcKey ;
	iResult = poStack->Push( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_PUSH_ARG, "SAV_PARAMS" );
		return "";
	}

	// now we execute RET_VALUE( application, section, key )
	iResult = poNode->RecordSet.Item[ "RET_VALUE" ].Call( NULL, 3 ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_EXECUTE_CALL, "RET_VALUE" << "SAV_PARAMS" << iResult );
		return "";
	}

	iResult = poStack->Pop( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_EXECUTE_LN4, "RET_VALUE" << "SAV_PARAMS" << iResult );
		return "";
	}

	// Se procesan los resultados
	poNode = &( poAccess->Node[ "FINAL_RETURN"] ) ;

	if( poNode == NULL )
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_GET_NODE, "FINAL_RETURN" << "SAV_PARAMS" );
		return "";
	}

	// Se hace una primera pasada buscando el ENABLED
	m4bool_t bIsNotEof = poNode->RecordSet.Register.Begin() ;

	if (bIsNotEof == M4_FALSE)
	{
		// there is no parameter defined. Just returns.
		return "";
	}

	iResult = poNode->RecordSet.Register.Item[ "APP_VALUE" ].Value.Get( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_GET_PARAM_VALUE, "APP_VALUE" << "SAV_PARAMS" );
		return "";
	}

	if( vValue.Type != M4CL_CPP_TYPE_STRING_VAR || vValue.Data.PointerChar == NULL || strlen( vValue.Data.PointerChar ) == 0 )
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_AUTH_GET_INVALID_VALUE, "APP_VALUE" << "SAV_PARAMS" );
		return "";
	}

	return vValue.Data.PointerChar;
}

m4return_t ClM4XMLContext::Serialize (ClGenericIODriver& ao_oIODriver)
{
  //## begin ClM4XMLContext::Serialize%940236198.body preserve=yes

	// serializes the unique id
	m4int32_t iLen = m_sUUID.length();
	if (ao_oIODriver.Write(iLen) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_SERIAL_UUID );
		return M4_ERROR;
	}
	if (iLen > 0) 
	{
		if (ao_oIODriver.Write(m_sUUID.c_str()) != M4_SUCCESS)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_SERIAL_UUID );
			return M4_ERROR;
		}
	}

	// serializes the the authentication status
	if (ao_oIODriver.Write(m_uLastAuthenticationTime) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_SERIAL_AUTH_INFO );
		return M4_ERROR;
	}
	if (ao_oIODriver.Write(m_uMaxAuthenticationTime) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_SERIAL_AUTH_INFO );
		return M4_ERROR;
	}
	
	iLen = m_sAuthPassword.length();
	if (ao_oIODriver.Write(iLen) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_SERIAL_AUTH_PASS );
		return M4_ERROR;
	}
	if (iLen > 0) 
	{
		if (ao_oIODriver.Write(m_sAuthPassword.c_str()) != M4_SUCCESS)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_SERIAL_AUTH_PASS );
			return M4_ERROR;
		}
	}
	
	// serializes the persist level
	if (ao_oIODriver.Write(m_iPersistLevel) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_SERIAL_PERS_LEVEL );
		return M4_ERROR;
	}

	// serializes the translation channel
	m4bool_t bHaveTranslAccess = M4_TRUE;
	if (m_poTranslAccess == NULL)
		bHaveTranslAccess = M4_FALSE;
	
	if (ao_oIODriver.Write(bHaveTranslAccess) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_SERIAL_ACC );
		return M4_ERROR;
	}

	if (m_poTranslAccess != NULL)
	{
		m4bool_t bExist = M4_FALSE;
		if (ao_oIODriver.Write((void*)m_poTranslAccess,&bExist) != M4_SUCCESS || bExist == M4_FALSE)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_SERIAL_ACC );
			return M4_ERROR;
		}
	}

	// serializes the number of elements
	m4int16_t iNAlias = m_oAliasMap.size();
	if (ao_oIODriver.Write(iNAlias) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_SERIAL_MAP );
		return M4_ERROR;
	}

	m4return_t iResult = M4_SUCCESS;

	// writes the map
	M4OAliasMapIt it = m_oAliasMap.begin();
	for (it=m_oAliasMap.begin(); it != m_oAliasMap.end(); it++)
	{
		// writes the alias
		ClM4XMLCurrent *poCurrent = (*it).second;
		m4pcchar_t pcAlias = (*it).first.c_str();
		if (ao_oIODriver.Write(pcAlias) != M4_SUCCESS)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_SERIAL_MAP );
			iResult = M4_ERROR;
			break;
		}
		
		// writes the current
		if (poCurrent == NULL)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_SERIAL_MAP );
			iResult = M4_ERROR;
			break;
		}
		
		if (poCurrent->Serialize(ao_oIODriver) != M4_SUCCESS)
		{
			iResult = M4_ERROR;
			break;
		}
	}

	// returns
	return iResult;
  //## end ClM4XMLContext::Serialize%940236198.body
}

m4return_t ClM4XMLContext::Deserialize (ClGenericIODriver& ai_oIODriver)
{
  //## begin ClM4XMLContext::Deserialize%940236199.body preserve=yes

	// resets the current state
	ResetM4OAlias();

	// deserializes the unique id
	m4int32_t iLen = 0;
	if (ai_oIODriver.Read(iLen) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_DESERIAL_UUID );
		return M4_ERROR;
	}
	if (iLen > 0)
	{
		m4pchar_t buffer = new char[iLen + 1];
		if (ai_oIODriver.Read(buffer, iLen) != M4_SUCCESS)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_DESERIAL_UUID );
			delete [] buffer;
			return M4_ERROR;
		}

		m_sUUID = buffer;
		delete [] buffer;
	}

	// deserializes the the authentication status
	if (ai_oIODriver.Read(m_uLastAuthenticationTime) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_DESERIAL_AUTH_INFO );
		return M4_ERROR;
	}
	if (ai_oIODriver.Read(m_uMaxAuthenticationTime) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_DESERIAL_AUTH_INFO );
		return M4_ERROR;
	}

	iLen = 0;
	if (ai_oIODriver.Read(iLen) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_DESERIAL_AUTH_PASS );
		return M4_ERROR;
	}
	if (iLen > 0)
	{
		m4pchar_t buffer = new char[iLen + 1];
		if (ai_oIODriver.Read(buffer, iLen) != M4_SUCCESS)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_DESERIAL_AUTH_PASS );
			delete [] buffer;
			return M4_ERROR;
		}

		m_sAuthPassword = buffer;
		delete [] buffer;
	}

	// deserializes the persist level
	if (ai_oIODriver.Read(m_iPersistLevel) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_DESERIAL_PERS_LEVEL );
		return M4_ERROR;
	}

	// deserializes the translation channel
	m4bool_t bHaveTranslAccess = M4_TRUE;

	if (ai_oIODriver.Read(bHaveTranslAccess) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_DESERIAL_ACC );
		return M4_ERROR;
	}

	if (bHaveTranslAccess == M4_TRUE)
	{
		m4bool_t bExist = M4_FALSE;
		if (ai_oIODriver.Read((void**)&m_poTranslAccess,&bExist) != M4_SUCCESS || bExist == M4_FALSE)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_DESERIAL_ACC );
			return M4_ERROR;
		}
	}

	// deserializes the number of elements
	m4int16_t iNAlias = 0;
	m4char_t pcBuffer[MAX_ALIAS_SIZE];

	if (ai_oIODriver.Read(iNAlias) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_DESERIAL_MAP );
		return M4_ERROR;
	}

	m4return_t iResult = M4_SUCCESS;

	// reads the iodriver
	for (m4int16_t i=0; i<iNAlias; i++)
	{
		// reads the alias
		m4pchar_t pcAlias=NULL;

		// reads
		if (ai_oIODriver.Read(pcBuffer,sizeof(pcBuffer)) != M4_SUCCESS)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_DESERIAL_MAP );
			iResult = M4_ERROR;
			break;
		}

		// inits the string
		string sAlias(pcBuffer);

		// inits a new context
		ClM4XMLCurrent *poCurrent = new ClM4XMLCurrent;
		if (poCurrent->Deserialize(ai_oIODriver) != M4_SUCCESS)
		{
			iResult = M4_ERROR;
			break;
		}

		// adds the elements
		m_oAliasMap.insert(M4OAliasMap::value_type(sAlias,poCurrent));
	}

	// returns
	return iResult;
  //## end ClM4XMLContext::Deserialize%940236199.body
}

void ClM4XMLContext::ResetM4OAlias ()
{
  //## begin ClM4XMLContext::ResetM4OAlias%940247661.body preserve=yes

	// serializes the number of elements
	m4int16_t iNAlias = m_oAliasMap.size();
	if (!iNAlias)
		return;

	M4OAliasMapIt it = m_oAliasMap.begin();
	for (it=m_oAliasMap.begin(); it != m_oAliasMap.end(); it++)
		delete (*it).second;

	m_oAliasMap.clear();

  //## end ClM4XMLContext::ResetM4OAlias%940247661.body
}

m4return_t ClM4XMLContext::PopStack (ClAccess* ai_poAccess, m4int16_t ai_iNParam, m4VariantType** ao_voParam, m4bool_t ai_bReset)
{
  //## begin ClM4XMLContext::PopStack%940341733.body preserve=yes
	if (ai_poAccess == NULL || ao_voParam == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_INIT );
		return M4_ERROR;
	}

	m4return_t iResult = M4_SUCCESS;

	for (m4int16_t i=0; i<ai_iNParam; i++)
		if (ai_poAccess->GetpExecutor()->Stack.Pop(*ao_voParam[i]) != M4_SUCCESS)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_CONTEXT_POP, ai_iNParam );
			iResult = M4_ERROR;
			break;
		}

	if (ai_bReset == M4_TRUE)
		ai_poAccess->GetpExecutor()->Stack.Reset();

	return iResult;
	
  //## end ClM4XMLContext::PopStack%940341733.body
}

m4return_t ClM4XMLContext::ProcAliasCommands ()
{
  //## begin ClM4XMLContext::ProcAliasCommands%941822487.body preserve=yes

	// gets the number of elements
	m4return_t iResult = M4_SUCCESS;

	// writes the map
	M4OAliasMapIt it = m_oAliasMap.begin();
	while (it != m_oAliasMap.end())
	{
		// gets the preserve state
		ClM4XMLCurrent *poCurrent = (*it).second;
		if (poCurrent->bPreserve == M4_TRUE)
		{
			it++;
			continue;
		}

		if (poCurrent->Reset() != M4_SUCCESS)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_RESET_STATE );
			iResult = M4_ERROR;
			it++;
			continue;
		}

		// deletes the item from the map
		M4OAliasMapIt it2 = it;
		it2++;
		m_oAliasMap.erase(it);
		it = it2;
		delete poCurrent;
	}

	return iResult;
  //## end ClM4XMLContext::ProcAliasCommands%941822487.body
}

m4return_t ClM4XMLContext::GenAlias (ClM4XMLStream &ao_oOut)
{
  //## begin ClM4XMLContext::GenAlias%944575099.body preserve=yes

	if (m_poObjectService == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CONTEXT_INIT );
		return M4_ERROR;
	}

	// checks the alias
	if (!m_oAliasDef.sName.size())
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_INVALID_ALIAS_NAME );
		return M4_ERROR;
	}

	// checks the map
	M4OAliasMapIt it = m_oAliasMap.find(m_oAliasDef.sName);
	m4return_t iResult = M4_SUCCESS;

	// securizes the access
	if (it != m_oAliasMap.end())
	{
		ClM4XMLCurrent *poCurrent = (*it).second;
		if (poCurrent != NULL && poCurrent->poAccess != NULL) 
		{
			// sets the security mode
			SetUseFunctionalGroup( poCurrent->poAccess );
		}
	}
	
	// checks deletion mode
	if (XMLICMP(m_oAliasDef.sRemove,"yes",1))
	{
		if (it != m_oAliasMap.end())
		{
			ClM4XMLCurrent *poCurrent = (*it).second;

			// mark to delete at context end
			poCurrent->bPreserve = M4_FALSE;
		}

		return M4_SUCCESS;
	}
			
	// checks if we had found the alias
	if (it != m_oAliasMap.end())
	{
		// reset deltas
		ClM4XMLCurrent *poCurrent = (*it).second;

		// all done
		return M4_SUCCESS;
	}

	// adds the channel
	ClChannel *poChannel = m_oAliasDef.Generate(m_poObjectService->GetpChannelManager());
	if (poChannel == NULL)
	{
		// internal errors already reported
		return M4_ERROR;
	}

	// adds the entry in the map
	ClAccess *poAccess = poChannel->CreateAccess();
	ClAccess *poAccessGet = poChannel->CreateAccess();
	ClM4XMLCurrent *poCurrent = new ClM4XMLCurrent;
	poCurrent->poAccess = poAccess;
	poCurrent->poAccessGet = poAccessGet;

	// sets the security mode
	SetUseFunctionalGroup( poAccess );

	if (XMLICMP(m_oAliasDef.sFind,"yes",1))
		poCurrent->bOwner = M4_FALSE;

	if (XMLICMP(m_oAliasDef.sPreserve,"no",1))
		poCurrent->bPreserve = M4_FALSE;

	m_oAliasMap.insert(M4OAliasMap::value_type(m_oAliasDef.sName,poCurrent));

	// gets the organization
	m4pchar_t pcOrganization = NULL;
	poChannel->Organization.Get( pcOrganization ) ;

	// processes MD info level
	if (m_oAliasDef.sMDInfoLevel.size())
	{
		ClBuidParameters oBP ;
		ClLongString	oString;
		oString.Init( 200000, 100000 ) ;

		oBP.m_poLane = NULL ;	
		oBP.m_pccM4ObjId = m_oAliasDef.sM4OId.c_str() ;
		oBP.m_pccIdOrg = pcOrganization ;
		oBP.m_pccRSM = NULL ;
		oBP.m_dStartDate = M4CL_MINUS_INFINITE_IN_JULIAN ;
		oBP.m_dEndDate = M4CL_PLUS_INFINITE_IN_JULIAN ;
		oBP.m_dCorDate = M4CL_MINUS_INFINITE_IN_JULIAN ;
		oBP.m_iCsType = (XMLICMP(m_oAliasDef.sMode,"front",1)) ? M4CL_CSTYPE_FRONT : M4CL_CSTYPE_BOTH ;
		oBP.m_iLanguage = M4_LANG_NONE_ID ;
		oBP.m_bConvertRSM = M4_FALSE ;
		oBP.m_bBuildSecurity = M4_FALSE ;
		oBP.m_bApplyMappings = M4_TRUE ;	

		m4int16_t iMask = atoi(m_oAliasDef.sMDInfoLevel.c_str());

		ClCompiledMCR *poCMCR;
		iResult = m_poObjectService->GetpCMCRFactory()->MDFac_BuildFromId(oBP, poCMCR) ;

		if (iResult != M4_ERROR)
		{
			iResult = m_poObjectService->GetpCMCRFactory()->MDFac_BuildXML(poCMCR, pcOrganization, iMask, oString, M4_FALSE) ;

			if (iResult != M4_ERROR)
			{
				// dumps the result
				ao_oOut << oString.GetString();
			}
		}
	}

	return iResult;
  //## end ClM4XMLContext::GenAlias%944575099.body
}

m4void_t ClM4XMLContext::SetUseFunctionalGroup ( ClAccess *ai_poAccess )
{
  //## begin ClM4XMLContext::SetUseFunctionalGroup%944575100.body preserve=yes
	// if not enabled we just ignore
	m4bool_t bUseFunctionalGroup = XMLICMP(m_oAliasDef.sUseFunctionalGroup,"true",1);

	// lets set the channel property
	m4VariantType oValue;
	oValue.SetDouble((bUseFunctionalGroup == M4_TRUE) ? 1.0 : 0.0);
	ClChannel *poChannel = ai_poAccess->GetpChannel();

    poChannel->Property.DeletePrivateProperty(M4_USE_FUNCTIONAL_GROUP_PROPERTY);
	poChannel->Property.NewPrivateProperty( M4_USE_FUNCTIONAL_GROUP_PROPERTY, (M4_PROP_READ | M4_PROP_WRITE));
	poChannel->Property.SetPrivatePropValue( M4_USE_FUNCTIONAL_GROUP_PROPERTY, oValue );
  //## begin ClM4XMLContext::SetUseFunctionalGroup%944575100.body preserve=yes
}

m4bool_t ClM4XMLContext::GetUseFunctionalGroup ( ClAccess *ai_poAccess )
{
  //## begin ClM4XMLContext::GetUseFunctionalGroup%944575100.body preserve=yes

	m4VariantType oValue;
	m4return_t iRes = ai_poAccess->GetpChannel()->Property.GetPrivatePropValue( M4_USE_FUNCTIONAL_GROUP_PROPERTY, oValue );
	if (iRes != M4_SUCCESS)
	{
		return M4_FALSE;
	}

	if (((m4double_t)oValue) == 1.0)
	{
		return M4_TRUE;
	}

	return M4_FALSE;

	//## begin ClM4XMLContext::GetUseFunctionalGroup%944575100.body preserve=yes
}

m4return_t ClM4XMLContext::AddL2ReferencesToAlias ()
{
  //## begin ClM4XMLContext::AddL2ReferencesToAlias%944575100.body preserve=yes

	// gets the references iterator
	ClM4AliasReferenceSet& oReferences = m_oAliasDef.GetReferences();

	for (ClM4AliasRefIt oIt = oReferences.begin(); oIt != oReferences.end(); ++oIt) {

		// gets the info
		const string& sM4OAliasOrigin = (*oIt).sM4OAliasOrigin;
		const string& sM4OAlias = (*oIt).sM4OAlias;
		const string& sL2Alias = (*oIt).sL2Alias;

		ClM4XMLCurrent *poCurrentOrigin = GetCurrentByAlias(sM4OAliasOrigin);

		// the origin object must be already defined
		if (poCurrentOrigin == NULL) {
			DUMP_CHLOG_ERROR(M4_ERR_XML_INVALID_ALIAS_REF);
			return M4_ERROR;
		}

		ClChannel *poOrigin = poCurrentOrigin->poAccess->GetpChannel();
		poOrigin->GetpChannel_Data()->SetKeepStatus(M4_TRUE, 1);

		if (sL2Alias.size() == 0) {
			DUMP_CHLOG_ERROR ( M4_ERR_XML_INVALID_ALIAS_L2_REF );
			return M4_ERROR;
		}

		// gets the object instance
		ClM4XMLCurrent *poCurrent = GetCurrentByAlias(sM4OAlias);

		// the object must be already defined
		if (poCurrent == NULL) {
			DUMP_CHLOG_ERROR ( M4_ERR_XML_INVALID_ALIAS_REF );
			return M4_ERROR;
		}

		// retrieves the object
		ClAccess *poDestAccess = CloneAccess(poCurrent->poAccess);
		if (poDestAccess == NULL) {
			DUMP_CHLOG_ERROR ( M4_ERR_XML_INVALID_M4O_INSTANCE );
			return M4_ERROR;
		}

		// reset deltas
		poDestAccess->GetpChannel()->GetpChannel_Data()->SetKeepStatus(M4_TRUE, 1);

		// inserts the instance, we set it ot onwer = FALSE to avoid foreign destruction
		poOrigin->Level2.EraseL2Instance (sL2Alias.c_str(), M4_FALSE);
		m4return_t iRet = poOrigin->Level2.InsertL2Access (sL2Alias.c_str(), poDestAccess, M4_TRUE);
		if (iRet != M4_SUCCESS)
		{	
			poDestAccess->Destroy();
			DUMP_CHLOG_ERROR ( M4_ERR_XML_INVALID_M4O_INSTANCE );
			return M4_ERROR;
		}

	}

	return M4_SUCCESS;
  //## end ClM4XMLContext::AddL2ReferencesToAlias%944575100.body
}

m4return_t ClM4XMLContext::RetrieveL2ReferencesFromAlias ()
{
  //## begin ClM4XMLContext::RetrieveL2ReferencesFromAlias%944575100.body preserve=yes

	// gets the references iterator
	ClM4AliasReferenceSet& oReferences = m_oAliasDef.GetReferences();

	for (ClM4AliasRefIt oIt = oReferences.begin(); oIt != oReferences.end(); ++oIt) {

		// gets the info
		const string& sM4OAliasOrigin = (*oIt).sM4OAliasOrigin;
		const string& sM4OAlias = (*oIt).sM4OAlias;
		const string& sL2Alias = (*oIt).sL2Alias;

		ClM4XMLCurrent *poCurrentOrigin = GetCurrentByAlias(sM4OAliasOrigin);

		// the origin object must be already defined
		if (poCurrentOrigin == NULL) {
			DUMP_CHLOG_ERROR(M4_ERR_XML_INVALID_ALIAS_REF);
			return M4_ERROR;
		}

		ClChannel *poOrigin = poCurrentOrigin->poAccess->GetpChannel();

		if (sL2Alias.size() == 0) {
			DUMP_CHLOG_ERROR ( M4_ERR_XML_INVALID_ALIAS_L2_REF );
			return M4_ERROR;
		}

		// gets the object instance
		ClM4XMLCurrent *poCurrent = GetCurrentByAlias(sM4OAlias);

		// the object must be already defined
		if (poCurrent == NULL) {
			DUMP_CHLOG_ERROR ( M4_ERR_XML_INVALID_ALIAS_REF );
			return M4_ERROR;
		}

		// retrieves the object
		ClAccess *poOriginAccess = NULL;
		m4return_t iRet = poOrigin->Level2.GetL2Access(sL2Alias.c_str(), &poOriginAccess);
		if (iRet != M4_SUCCESS) {
			DUMP_CHLOG_ERROR ( M4_ERR_XML_INVALID_M4O_INSTANCE );
			return M4_ERROR;
		}

		// restores the access to the original one
		iRet = RestoreAccess(poOriginAccess, poCurrent->poAccess);
		if (iRet != M4_SUCCESS) {
			DUMP_CHLOG_ERROR ( M4_ERR_XML_INVALID_ALIAS_L2_REF );
			return M4_ERROR;
		}

		// now we can remove the reference
		iRet = poOrigin->Level2.EraseL2Instance (sL2Alias.c_str(), M4_FALSE);
		if (iRet != M4_SUCCESS) {
			DUMP_CHLOG_ERROR ( M4_ERR_XML_INVALID_ALIAS_L2_REF );
			return M4_ERROR;
		}
	}

	return M4_SUCCESS;

  //## end ClM4XMLContext::RetrieveL2ReferencesFromAlias%944575100.body
}

ClAccess* ClM4XMLContext::CloneAccess (ClAccess *ai_poOrigin)
{
  //## begin ClM4XMLContext::CloneAccess%944575100.body preserve=yes

	// lets read the input bookmark
	ClAccessBookMark oBookmark;
	m4return_t iRet = oBookmark.GetFrom(*ai_poOrigin);
	if (iRet != M4_SUCCESS) 
	{
		return NULL;
	}

	// creates a new acccess
	ClAccess *poDestine = ai_poOrigin->GetpChannel()->CreateAccess();

	// applies the bookmark
	iRet = oBookmark.ApplyTo(*poDestine);
	if (iRet != M4_SUCCESS) 
	{
		poDestine->Destroy();
		return NULL;
	}

	return poDestine;

  //## end ClM4XMLContext::CloneAccess%944575100.body
}

m4return_t ClM4XMLContext::RestoreAccess (ClAccess *ai_poOrigin, ClAccess *ai_poDestine)
{
  //## begin ClM4XMLContext::RestoreAccess%944575100.body preserve=yes

	// lets check the object... it must be the same
	if (ai_poOrigin->GetpChannel() != ai_poOrigin->GetpChannel())
	{
		return M4_ERROR;
	}

	// lets read the input bookmark
	ClAccessBookMark oBookmark;
	m4return_t iRet = oBookmark.GetFrom(*ai_poOrigin);
	if (iRet != M4_SUCCESS) 
	{
		return M4_ERROR;
	}

	// applies the bookmark
	iRet = oBookmark.ApplyTo(*ai_poDestine);
	if (iRet != M4_SUCCESS) 
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;

  //## end ClM4XMLContext::RestoreAccess%944575100.body

}

m4return_t ClM4XMLContext::ProcM4OParam (const string &ai_sName, const string &ai_sValue, const string &ai_sCppType)
{
  //## begin ClM4XMLContext::ProcM4OParam%944575100.body preserve=yes
	m4int16_t iCPPType = -1;
	if (ai_sCppType.size())
		iCPPType = atoi(ai_sCppType.c_str());
	ClM4AliasParam oParam(ai_sName,ai_sValue,iCPPType);
	return m_oAliasDef.AddParam(oParam);
  //## end ClM4XMLContext::ProcM4OParam%944575100.body
}

m4return_t ClM4XMLContext::ProcM4ORef (const string &ai_sM4OAlias, const string &ai_sL2Alias)
{
  //## begin ClM4XMLContext::ProcM4ORef%944575100.body preserve=yes
	ClM4AliasRef oRef(m_oAliasDef.sName,ai_sM4OAlias,ai_sL2Alias);
	return m_oAliasDef.AddRef(oRef);
  //## end ClM4XMLContext::ProcM4ORef%944575100.body
}

void ClM4XMLContext::AddDumpedItem (const string &ai_sItem)
{
  //## begin ClM4XMLContext::AddDumpedItem%944822920.body preserve=yes
	m_oDumpedItems.AddItem(ai_sItem);
  //## end ClM4XMLContext::AddDumpedItem%944822920.body
}

m4bool_t ClM4XMLContext::CheckDumpedItem (const string &ai_sItem)
{
  //## begin ClM4XMLContext::CheckDumpedItem%944822921.body preserve=yes
	return m_oDumpedItems.CheckItem(ai_sItem);
  //## end ClM4XMLContext::CheckDumpedItem%944822921.body
}

m4return_t ClM4XMLContext::InitSession ()
{
  //## begin ClM4XMLContext::InitSession%945440688.body preserve=yes
	if (m_poObjectService == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_INIT_SESSION_INTERNAL );
		return M4_ERROR;
	}

	if (m_poBaseSessionChannel == NULL || m_poBaseSessionChannel->IsBuilt() == M4_FALSE)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_INIT_SESSION_INTERNAL );
		return M4_ERROR;
	}

	// ok, we need to synchronize before clone the session channel
	static ClMutex oSessionMutex(M4_TRUE);
	ClMutBlock oSessionMutBlock(oSessionMutex);

	M4DataStorage* poDataStorage = NULL;
	poDataStorage = M4DataStorage::GetNewDataStorage();
	if (poDataStorage == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_INIT_SESSION_DATA_STORE );
		return M4_ERROR;
	}

	// serializes
	ClChannelManager *pChannelManager = m_poObjectService->GetpChannelManager();
	{
		ClDataStoreIODriver oDSIOD;
		if (oDSIOD.Init(GEN_IO_MODE_WRITE, poDataStorage) != M4_SUCCESS)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_INIT_IOD_ERROR );
			M4DataStorage::DeleteDataStorage( poDataStorage );
			return M4_ERROR;
		}

		ClCMObjectDirectory oObjDir(pChannelManager);
		oObjDir.SetForceL2RelationSerialize(M4_FALSE);
		oObjDir.SetbProhibitL2RelationSerialize(M4_TRUE);
		m_poBaseSessionChannel->SetRecordSerializationMode(M4CL_RECORD_SERIAL_ALL);
		m_poBaseSessionChannel->SetItemSerializationMode(M4CL_ITEM_SERIAL_ALL);
		
		if ( oObjDir.IncludeChannel(m_poBaseSessionChannel,0) == M4_ERROR )
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_INIT_IOD_ERROR );
			M4DataStorage::DeleteDataStorage( poDataStorage );
			return M4_ERROR;
		}

		oObjDir.SetKeepChannelCsTypeOnRemote(M4_TRUE);
		if ( oObjDir.Serialize(oDSIOD) != M4_SUCCESS)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_INIT_SERIALIZE );
			M4DataStorage::DeleteDataStorage( poDataStorage );
			return M4_ERROR;
		}
	}

	// deserializes
	{
		ClDataStoreIODriver oDSIOD;
		if (oDSIOD.Init(GEN_IO_MODE_READ, poDataStorage) != M4_SUCCESS)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_INIT_IOD_ERROR );
			M4DataStorage::DeleteDataStorage( poDataStorage );
			return M4_ERROR;
		}

		ClCMObjectDirectory oObjDir(pChannelManager);
		if (oObjDir.DeSerialize(oDSIOD, NULL, M4_TRUE) != M4_SUCCESS)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_INIT_DESERIAL );
			M4DataStorage::DeleteDataStorage( poDataStorage );
			return M4_ERROR;
		}

		// keeps the channel alive
		// oObjDir.SetKeepAliveAll(M4_TRUE);

	}

	// ends
	M4DataStorage::DeleteDataStorage( poDataStorage );
	return M4_SUCCESS;
  //## end ClM4XMLContext::InitSession%945440688.body
}

m4return_t ClM4XMLContext::SetSession (m4bool_t ai_bClose)
{
  //## begin ClM4XMLContext::SetSession%945440689.body preserve=yes
	if (m_poObjectService == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_INIT_SESSION_INTERNAL );
		return M4_ERROR;
	}

	if (ai_bClose == M4_TRUE)
	{
		m4return_t iResult = M4_SUCCESS;
		
		// releases the session channel
		ClChannelManager *pChannelManager = m_poObjectService->GetpChannelManager();
		if (pChannelManager->SetSessionChannel(NULL) != M4_SUCCESS)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_SET_SESSION_CHANNEL_BASE );
			iResult = M4_ERROR;
		}

		// releases the channels
		iResult = pChannelManager->Reset();
		if (iResult != M4_SUCCESS)	
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_RESET_DM );
			iResult = M4_ERROR;
		}

		return M4_SUCCESS;

	}

	// the session channel should be the first channel
	// checks the session status
	ClChannelManager	*pChannelManager = m_poObjectService->GetpChannelManager();
	ClChannel			*poSessionChannel = NULL;

	// reads the first object in the channel manager
	if (pChannelManager->Channel.GetNumberOfChannels() > 0)
	{
		poSessionChannel = pChannelManager->Channel[0];
	}
	else {
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Initializing SessionChannel: Missing session channel in context");
	}

	ClChannel *poCurrentSessionChannel = pChannelManager->GetSessionChannel();
	if (poCurrentSessionChannel != poSessionChannel)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_INCONSISTENCE_SESSION );
		return M4_ERROR;
	}

	// resets session channel items
	if (NULL != poSessionChannel) {
		ClAccess	*poAccess=poSessionChannel->CreateAccess();
		if (NULL == poAccess)
		{
			SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Initializing SessionChannel: Error creating Access");
			return M4_ERROR;
		}

		ClNode *poNode=&(poAccess->Node["ROOT_SESSION"]);
		if (NULL == poNode )
		{
			SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Initializing SessionChannel: Error creating Node");
			poAccess->Destroy();
			return M4_ERROR;
		}

		// set object to use only local instance
		m4VariantType ovArg;
		ovArg.Type = M4CL_CPP_TYPE_NUMBER;
		ovArg.Data.DoubleData = 0;
		m4return_t iRet = poNode->RecordSet.Current.Item["SYNCHRONIZE_SERVER"].Value.Set(ovArg);
		if (M4_SUCCESS != iRet)
		{
			SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Error setting SYNCHRONIZE_SERVER");
			poAccess->Destroy();
			return M4_ERROR;
		}

		// clear and return
		poAccess->Destroy();
	}

	return M4_SUCCESS;
  //## end ClM4XMLContext::SetSession%945440689.body
}

m4return_t ClM4XMLContext::ReleaseLogicalConnection ()
{
  //## begin ClM4XMLContext::ReleaseLogicalConnection%971105347.body preserve=yes
	// frees logical connection before release channels
	m4return_t iResult = M4_SUCCESS;
	if (m_poObjectService != NULL)
	{
		iResult = m_poObjectService->GetpConnProvider()->SetConnection();
		if (iResult != M4_SUCCESS)	
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_FREE_CONN );
			iResult = M4_ERROR;
		}
	}

	return iResult;
  //## end ClM4XMLContext::ReleaseLogicalConnection%971105347.body
}

void ClM4XMLContext::Init ()
{
  //## begin ClM4XMLContext::Init%971105356.body preserve=yes
	// reset members before execution
	m_oDumpedItems.Reset();
	m_oDumpedM4O.Reset();
	m_oTranslItems.clear();
	ResetTranslItem();
	ResetFileRefs();
  //## end ClM4XMLContext::Init%971105356.body
}

void ClM4XMLContext::ResetFileRefs ()
{
  //## begin ClM4XMLContext::ResetFileRefs%972490548.body preserve=yes
	if (m_poInFileRefs != NULL)
	{
		delete [] m_poInFileRefs;
		m_poInFileRefs = NULL;
		m_iNInFileRefs = 0;
	}
	m_iNOutFileRefs = 0;
  //## end ClM4XMLContext::ResetFileRefs%972490548.body
}

void ClM4XMLContext::Update (ClCMInitParameters &aio_oCMInitParameters)
{
  //## begin ClM4XMLContext::Update%978518712.body preserve=yes
	if (!GetSessionId())
	{
		SetSessionId( aio_oCMInitParameters.GetSessionId() );
	}

	if (GetSubSessionId().size() == 0)
	{
		SetSubSessionId(aio_oCMInitParameters.GetSubSessionId());
	}
	
	if (GetBaseSessionChannel() == NULL)
	{
		SetBaseSessionChannel( aio_oCMInitParameters.GetSessionChannel() );
	}
	
	if (GetStateManager() == NULL)
	{
		SetStateManager( aio_oCMInitParameters.GetStateManager() );
	}
	
	if (GetObjectService() == NULL && aio_oCMInitParameters.GetExecutorState() != NULL)
	{
		SetObjectService( aio_oCMInitParameters.GetExecutorState()->GetM4ObjService());
	}

	if (GetStateManager() != NULL)
	{
		m4char_t pcSessionId[32] = "";
		sprintf(pcSessionId,"%ld",GetSessionId());
		m_uiLastAccess = GetStateManager()->UpdateSessionMap(pcSessionId);
	}

	if (aio_oCMInitParameters.GetPersistLevel().compare("") != 0)
	{
		m_iPersistLevel = atoi(aio_oCMInitParameters.GetPersistLevel().c_str());
	}

	InitUniqueId();

	//## end ClM4XMLContext::Update%978518712.body
}

void ClM4XMLContext::AddDumpedM4O (const string &ai_sM4O)
{
  //## begin ClM4XMLContext::AddDumpedM4O%1071498570.body preserve=yes
	m_oDumpedM4O.AddItem(ai_sM4O);
  //## end ClM4XMLContext::AddDumpedM4O%1071498570.body
}

m4bool_t ClM4XMLContext::CheckDumpedM4O (const string &ai_sM4O)
{
  //## begin ClM4XMLContext::CheckDumpedM4O%1071498571.body preserve=yes
	return m_oDumpedM4O.CheckItem(ai_sM4O);
  //## end ClM4XMLContext::CheckDumpedM4O%1071498571.body
}

void ClM4XMLContext::DumpM4OInfo (ClM4XMLStream& ao_oOut)
{
  //## begin ClM4XMLContext::DumpM4OInfo%1071498572.body preserve=yes
	DumpedItemSet oSet = m_oDumpedM4O.GetSet();
	if (oSet.size() == 0) {
		return;
	}

	// writes the that
	ao_oOut << "<m4objectinfo>" << endl;

	DumpedItemIt oIt = oSet.begin();
	for (;oIt != oSet.end();++oIt) {
		string sId = (*oIt).GetValue();

		// gets the alias
		ClM4XMLCurrent *poCurrent = this->GetCurrentByAlias(sId);
		if (poCurrent == NULL) {
			// suspicious....
			continue;
		}

		// gets the MCR
		ClCompiledMCR *pcMCR = poCurrent->poAccessGet->GetpCMCR();
		m4uint8_t iCacheable = pcMCR->GetChannelIsCacheable();
		m4pcchar_t pcId = pcMCR->GetChannelId();

		// writes the data
		ao_oOut << "<m4object id=\"" << pcId << "\">" << endl;

		// writes the properties
		ao_oOut << "<prop id=\"dc\" value=\"" << iCacheable << "\"/>" << endl;

		// end of properties
		ao_oOut << "</m4object>" << endl;
	}

	ao_oOut << "</m4objectinfo>" << endl;
  //## end ClM4XMLContext::DumpM4OInfo%1071498572.body
}

// Additional Declarations
  //## begin ClM4XMLContext%37DF520B0399.declarations preserve=yes
  //## end ClM4XMLContext%37DF520B0399.declarations

// Class ClM4XMLCurrent 







ClM4XMLCurrent::ClM4XMLCurrent ()
  //## begin ClM4XMLCurrent::ClM4XMLCurrent%937383069.hasinit preserve=no
      : poAccess(NULL), poNode(NULL), poAccessGet(NULL), poNodeGet(NULL), bPreserve(M4_TRUE), bOwner(M4_TRUE)
  //## end ClM4XMLCurrent::ClM4XMLCurrent%937383069.hasinit
  //## begin ClM4XMLCurrent::ClM4XMLCurrent%937383069.initialization preserve=yes
  //## end ClM4XMLCurrent::ClM4XMLCurrent%937383069.initialization
{
  //## begin ClM4XMLCurrent::ClM4XMLCurrent%937383069.body preserve=yes
	Init();
  //## end ClM4XMLCurrent::ClM4XMLCurrent%937383069.body
}


ClM4XMLCurrent::~ClM4XMLCurrent ()
{
  //## begin ClM4XMLCurrent::~ClM4XMLCurrent%939117435.body preserve=yes
	Reset();
  //## end ClM4XMLCurrent::~ClM4XMLCurrent%939117435.body
}



//## Other Operations (implementation)
void ClM4XMLCurrent::Init ()
{
  //## begin ClM4XMLCurrent::Init%937383070.body preserve=yes
	poAccess = NULL;
	poNode = NULL;
	poAccessGet = NULL;
	poNodeGet = NULL;
	bPreserve = M4_TRUE;
  //## end ClM4XMLCurrent::Init%937383070.body
}

m4return_t ClM4XMLCurrent::Reset ()
{
  //## begin ClM4XMLCurrent::Reset%939117436.body preserve=yes
	m4return_t iResult = M4_SUCCESS;

	ClChannel *poChannel = NULL;
	if (poAccess != NULL)
		poChannel = poAccess->GetpChannel();

	if (poAccess != NULL && poAccess->Destroy() != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_CURRENT_DESTROY, poChannel->GetpChannelDef()->Id() );
		iResult = M4_ERROR;
	}
	if (poAccessGet != NULL && poAccessGet->Destroy() != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_CURRENT_DESTROY, poChannel->GetpChannelDef()->Id() );
		iResult = M4_ERROR;
	}
	if (bOwner == M4_TRUE && poChannel != NULL && poChannel->Destroy() != M4_SUCCESS)
	{
//		DUMP_CHLOG_ERRORF ( M4_ERR_XML_CURRENT_DESTROY, poChannel->GetpChannelDef()->Id() );
//		iResult = M4_ERROR;
	}
	
	Init();

	return iResult;
  //## end ClM4XMLCurrent::Reset%939117436.body
}

m4return_t ClM4XMLCurrent::Serialize (ClGenericIODriver& ao_oIODriver)
{
  //## begin ClM4XMLCurrent::Serialize%940247659.body preserve=yes
	// writes the owner state
	if (ao_oIODriver.Write(bOwner) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CURRENT_SERIAL );
		return M4_ERROR;
	}
	
	// writes the set access
	if (SerializeLL(ao_oIODriver,poAccess,poNode,bOwner) != M4_SUCCESS)
		return M4_ERROR;

	// writes the get access
	if (SerializeLL(ao_oIODriver,poAccessGet,poNodeGet,bOwner) != M4_SUCCESS)
		return M4_ERROR;

	return M4_SUCCESS;
  //## end ClM4XMLCurrent::Serialize%940247659.body
}

m4return_t ClM4XMLCurrent::Deserialize (ClGenericIODriver& ai_oIODriver)
{
  //## begin ClM4XMLCurrent::Deserialize%940247660.body preserve=yes
	// reads the owner state
	if (ai_oIODriver.Read(bOwner) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CURRENT_DESERIAL );
		return M4_ERROR;
	}
	
	// reads the set access
	if (DeserializeLL(ai_oIODriver,poAccess,poNode,bOwner) != M4_SUCCESS)
		return M4_ERROR;

	if (DeserializeLL(ai_oIODriver,poAccessGet,poNodeGet,bOwner) != M4_SUCCESS)
		return M4_ERROR;

	return M4_SUCCESS;
  //## end ClM4XMLCurrent::Deserialize%940247660.body
}

m4return_t ClM4XMLCurrent::SerializeLL (ClGenericIODriver& ao_oIODriver, ClAccess* &ao_poAccess, ClNode* &ao_poNode, m4bool_t ai_bOwner)
{
  //## begin ClM4XMLCurrent::SerializeLL%943275970.body preserve=yes

	m4bool_t bExist=M4_FALSE;
	m4char_t pcBuffer[MAX_NODE_ID_SIZE];

	// writes the set access
	if (ao_oIODriver.Write((void*)ao_poAccess,&bExist) != M4_SUCCESS || 
		ai_bOwner == M4_TRUE && bExist == M4_FALSE)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CURRENT_SERIAL );
		return M4_ERROR;
	}

	if (bExist == M4_FALSE)
	{
		// writes the pointer for external channels
		// the code ought to be plataform independent
		const m4uint32_t iLen = sizeof(sizeof(ClChannel*));
		m4uint8_t bBuffer[iLen];
		ClChannel *poChannel = ao_poAccess->GetpChannel();
		memcpy(bBuffer,&poChannel,iLen);

		if (ao_oIODriver.Write(bBuffer,iLen) != M4_SUCCESS)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_CURRENT_SERIAL );
			return M4_ERROR;
		}

		// now we have to serialize an access without serialize the channel
		if (ao_poAccess->Serialize(ao_oIODriver) != M4_SUCCESS)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_CURRENT_SERIAL );
			return M4_ERROR;
		}
	}
		
	if (ao_poNode != NULL)
		strcpy(pcBuffer,ao_poNode->GetpNodeDef()->Id());
	else
		strcpy(pcBuffer,"");
		
	if (ao_oIODriver.Write(pcBuffer) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CURRENT_SERIAL );
		return M4_ERROR;
	}

	return M4_SUCCESS;

  //## end ClM4XMLCurrent::SerializeLL%943275970.body
}

m4return_t ClM4XMLCurrent::DeserializeLL (ClGenericIODriver& ai_oIODriver, ClAccess* &ai_poAccess, ClNode* &ai_poNode, m4bool_t ai_bOwner)
{
  //## begin ClM4XMLCurrent::DeserializeLL%943275971.body preserve=yes
	m4bool_t bExist=M4_FALSE;
	m4char_t pcBuffer[MAX_NODE_ID_SIZE];
	
	// reads the set access
	if (ai_oIODriver.Read((void**)&ai_poAccess,&bExist) != M4_SUCCESS || 
		ai_bOwner == M4_TRUE && bExist == M4_FALSE)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CURRENT_DESERIAL );
		return M4_ERROR;
	}

	if (bExist == M4_FALSE)
	{
		// reads the pointer for external channels
		// the code ought to be plataform independent
		ClChannel *poChannel = NULL;
		const m4uint32_t iLen = sizeof(sizeof(ClChannel*));
		m4uint32_t iLenRead = 0;
		m4uint8_t bBuffer[iLen];
		if (ai_oIODriver.Read(bBuffer,iLenRead) != M4_SUCCESS ||
			iLenRead != iLen)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_CURRENT_DESERIAL );
			return M4_ERROR;
		}

		// converts to pointer. The channel MUST be there.
		memcpy(&poChannel,bBuffer,iLen);

		if (poChannel->CreateAccess(ai_poAccess) != M4_SUCCESS)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_CURRENT_DESERIAL );
			return M4_ERROR;
		}
		
		if (ai_poAccess->DeSerialize(ai_oIODriver) != M4_SUCCESS)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_CURRENT_DESERIAL );
			return M4_ERROR;
		}
	}

	// gets the node name
	if (ai_oIODriver.Read(pcBuffer,sizeof(pcBuffer)) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CURRENT_DESERIAL );
		return M4_ERROR;
	}

	if (strlen(pcBuffer))
	{
		ai_poNode = ai_poAccess->Node.Get(pcBuffer);
		if (ai_poNode == NULL)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_CURRENT_DESERIAL_NODE, pcBuffer );
			return M4_ERROR;
		}
	}
	else
		ai_poNode = NULL;

	return M4_SUCCESS;
  //## end ClM4XMLCurrent::DeserializeLL%943275971.body
}

// Additional Declarations
  //## begin ClM4XMLCurrent%37DFE587026D.declarations preserve=yes
  //## end ClM4XMLCurrent%37DFE587026D.declarations

// Class ClM4XMLItem 























ClM4XMLItem::ClM4XMLItem (ClM4XMLContext &ai_oContext, const string& ai_sItem)
  //## begin ClM4XMLItem::ClM4XMLItem%937383057.hasinit preserve=no
      : m_iResultType(-1)
  //## end ClM4XMLItem::ClM4XMLItem%937383057.hasinit
  //## begin ClM4XMLItem::ClM4XMLItem%937383057.initialization preserve=yes
  //## end ClM4XMLItem::ClM4XMLItem%937383057.initialization
{
  //## begin ClM4XMLItem::ClM4XMLItem%937383057.body preserve=yes
	Init();

	if (Parse(ai_sItem) != M4_SUCCESS)
	{
		Reset();
		return;
	}

	// moves
	if (Move(ai_oContext) != M4_SUCCESS)
	{
		Reset();
		return;
	}

  //## end ClM4XMLItem::ClM4XMLItem%937383057.body
}

ClM4XMLItem::ClM4XMLItem (const string& ai_sItem, m4bool_t ai_bForceData)
  //## begin ClM4XMLItem::ClM4XMLItem%937555629.hasinit preserve=no
      : m_iResultType(-1)
  //## end ClM4XMLItem::ClM4XMLItem%937555629.hasinit
  //## begin ClM4XMLItem::ClM4XMLItem%937555629.initialization preserve=yes
  //## end ClM4XMLItem::ClM4XMLItem%937555629.initialization
{
  //## begin ClM4XMLItem::ClM4XMLItem%937555629.body preserve=yes
	Init();

	if (Parse(ai_sItem,ai_bForceData))
	{
		Reset();
		return;
	}
  //## end ClM4XMLItem::ClM4XMLItem%937555629.body
}

ClM4XMLItem::ClM4XMLItem ()
  //## begin ClM4XMLItem::ClM4XMLItem%938162619.hasinit preserve=no
      : m_iResultType(-1)
  //## end ClM4XMLItem::ClM4XMLItem%938162619.hasinit
  //## begin ClM4XMLItem::ClM4XMLItem%938162619.initialization preserve=yes
  //## end ClM4XMLItem::ClM4XMLItem%938162619.initialization
{
  //## begin ClM4XMLItem::ClM4XMLItem%938162619.body preserve=yes
	Init();
  //## end ClM4XMLItem::ClM4XMLItem%938162619.body
}


ClM4XMLItem::~ClM4XMLItem ()
{
  //## begin ClM4XMLItem::~ClM4XMLItem%937383058.body preserve=yes
	Reset();
  //## end ClM4XMLItem::~ClM4XMLItem%937383058.body
}



//## Other Operations (implementation)
m4bool_t ClM4XMLItem::Match (ClM4XMLItem &ai_oItem)
{
  //## begin ClM4XMLItem::Match%937383059.body preserve=yes
	M4_ASSERT(0);
	return M4_FALSE;
  //## end ClM4XMLItem::Match%937383059.body
}

m4return_t ClM4XMLItem::Execute (ClM4XMLContext &ai_oContext, m4int16_t ai_iNArgs, m4uint32_t ai_uiRegisterUniqueId)
{
  //## begin ClM4XMLItem::Execute%937383065.body preserve=yes

	m4return_t iResult = M4_SUCCESS;
	m_iResultType = -1;
	m_sResult = "";
	m4bool_t bCheckResult = M4_FALSE;

	if (ai_oContext.AddL2ReferencesToAlias() == M4_ERROR) {

		// internal errors already reported
		return M4_ERROR;
	}

	// reset deltas in the main object
	m_poAccess->GetpChannel()->GetpChannel_Data()->SetKeepStatus(M4_TRUE, 1);

	// checks node level methods
	m4uint8_t iNReferedArgs = 0;

	if (m_pcM4OItem == NULL)
	{
		if (m_pcItem == NULL || m_pcNode == NULL)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_ITEM_EXECUTE_INIT );
			return M4_ERROR;
		}

		ClNode *poNode = m_poAccess->Node.Get(m_pcNode);
		if (poNode == NULL)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_CHANNEL_ITEM_INVALID_NODE, m_pcItem << m_pcNode );
			return M4_ERROR;
		}

		ClAccessRecordSet *poRS = &poNode->RecordSet;
		
		ClItem &oItem = poRS->Current.Item[m_pcItem];
		m4uint8_t iInternalType = oItem.ItemDef.InternalType();
		iNReferedArgs = oItem.ItemDef.NumberOfReferedArguments();

		// lets take the original cpp type since after the call the item will reference the last item used in the ln4 rule
		m_iResultType = oItem.ItemDef.CppType();
		bCheckResult = M4_TRUE;
		iResult = oItem.Call(NULL,ai_iNArgs);		
		if (iResult != M4_SUCCESS)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_ITEM_EXECUTE, m_pcItem );
		}

		// sets the current record id
		if (iResult == M4_SUCCESS && ai_uiRegisterUniqueId != -1)
		{
			if (poNode->RecordSet.Current.SetRecordID(ai_uiRegisterUniqueId) != M4_SUCCESS)
			{
				Reset();
				DUMP_CHLOG_ERRORF(M4_ERR_XML_SET_RECORD_UNIQUE_ID, GetM4OAlias() << GetNode() << GetItem());
				return M4_ERROR;
			}
		}

		// checks if we have to reset other executor status
		if (iInternalType == M4CL_INTERNAL_TYPE_RESET_M4XML_SPACE)
		{
			if (ai_oContext.GetStateManager() == NULL ||
				ai_oContext.GetStateManager()->GetCache() == NULL)
			{
				DUMP_CHLOG_ERRORF ( M4_ERR_XML_ITEM_EXECUTE, m_pcItem );
			}
			else 
			{
				// removes all other subsessions
				string subSessionId = ai_oContext.GetSubSessionId();
				ai_oContext.GetStateManager()->
					GetCache()->RemoveAllOtherObjectsInSession(subSessionId);

				// marks the current object to delete
				if (m_pcM4OAlias != NULL) 
				{
					ClM4XMLCurrent *poCurrent = ai_oContext.GetCurrentByAlias(m_pcM4OAlias);
					if (poCurrent != NULL)
					{
						poCurrent->bPreserve = M4_FALSE;
					}
				}
			}
		}
	}
	else
	{
		// Default is error, because may be a syntax error in the item-system name
		iResult = M4_ERROR;

		// node level methods
		if (!stricmp(m_pcM4OItem,"load"))
			iResult = m_poAccess->Load();
		else
		if (!stricmp(m_pcM4OItem,"unload"))
			iResult = m_poAccess->UnLoad();
		else
		if (!stricmp(m_pcM4OItem,"reload"))
			iResult = m_poAccess->ReLoad();
		else
		if (!stricmp(m_pcM4OItem,"persist"))
			iResult = m_poAccess->Persist();

		if (iResult != M4_SUCCESS)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_CHANNEL_ITEM_EXECUTE, m_pcM4OItem );
		}
	}

	// gets the method result
	m4VariantType vResult;

	// ignores reference variables
	while ((iNReferedArgs--) > 0)
		m_poAccess->GetpExecutor()->Stack.Pop (vResult);

	// gets the return value
	m4return_t iResult2 = M4_SUCCESS;
	
	if (bCheckResult == M4_TRUE) {
		iResult2 = m_poAccess->GetpExecutor()->Stack.Pop (vResult);
	}

	if (iResult2 != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_EXECUTE_POP );
	}
	else {
		if (m_iResultType < 0) {
			m_iResultType = vResult.Type;
		}
	}

	// lets the stack ok for the next iteration
	m_poAccess->GetpExecutor()->Stack.Reset();

	// checks the null value
	m_bResultIsNull = (vResult.Type == M4CL_CPP_TYPE_NULL);

	// converts the value to string (for xml serialization)
	ClVariantConversor varConv;
	varConv.ToCharString(vResult);

	// saves the result as string
	m_sResult = (m4pchar_t)vResult;

	// retrieves L2 references
	if (ai_oContext.RetrieveL2ReferencesFromAlias() == M4_ERROR) {

		// internal errors already reported
		return M4_ERROR;
	}

	return iResult;
  //## end ClM4XMLItem::Execute%937383065.body
}

m4return_t ClM4XMLItem::SetValue (ClM4XMLContext &ai_oContext, const string& ai_sValue, m4int32_t ai_iLevel)
{
  //## begin ClM4XMLItem::SetValue%937383066.body preserve=yes
	if (Move(ai_oContext, M4_TRUE) != M4_SUCCESS)
		return M4_ERROR;

	if (m_iHandle <= 0)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_ITEM_INVALID, m_pcM4OAlias << m_pcNode << m_pcItem );
		return M4_ERROR;
	}

	if (m_poNode == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_ITEM_INIT );
		return M4_ERROR;
	}

	m4return_t iResult = M4_SUCCESS;

	m4VariantType vValue;

	ClItem *poItem = &m_poNode->RecordSet.Current.Item;

	// gets the item name
	m4pcchar_t itemId = poItem->ItemDef.Id();

	m4bool_t bDecrypted = M4_FALSE;

	// lets check if the item is encrypted
	{
		string sFuncGroups;
		if ( ai_oContext.IsEncrypted( m_poAccess, m_pcNode, itemId, sFuncGroups ) == M4_TRUE )
		{
			// ok, lets decrypt the value
			string sValue = ai_sValue;
			m4bool_t bIsNull = M4_FALSE;
			if (ai_oContext.DeCrypt(sValue, bIsNull, sFuncGroups) != M4_SUCCESS)
			{
				// oh oh.... we're in an invalid context
				DUMP_CHLOG_ERRORF ( M4_ERR_XML_ITEM_DECRYPT_VALUE, itemId << m_pcNode << m_poAccess->GetpCMCR()->GetChannelId() << sFuncGroups.c_str() );
				return M4_ERROR;
			}

			// ok, lets replace the value
			bDecrypted = M4_TRUE;
			vValue = ( bIsNull == M4_TRUE ) ? m4VariantType() : m4VariantType((m4pchar_t)sValue.c_str(), M4_TRUE);
		}
	}

	// if not decrypted we can use the input value
	if ( bDecrypted == M4_FALSE )
	{
		vValue = m4VariantType((m4pchar_t)ai_sValue.c_str());
	}

	m4uint16_t iCPPType = poItem->ItemDef.CppType();
	ClVariantConversor varConv;

	switch(iCPPType)
	{
		case M4CL_CPP_TYPE_NULL:
			iResult = varConv.ToNull(vValue);
			break;
		case M4CL_CPP_TYPE_NUMBER:
			iResult = varConv.ToDouble(vValue);
			break;
		case M4CL_CPP_TYPE_STRING:
			break;
		case M4CL_CPP_TYPE_DATE:
			iResult = varConv.ToDate(vValue);
			break;
		default:
			DUMP_CHLOG_ERROR ( M4_ERR_XML_INTERNAL_CPPTYPE );
			iResult = M4_ERROR;
	}

	if (iResult != M4_SUCCESS)
	{
		return iResult;
	}

	while (ai_iLevel-- > 0)
	{
		if (poItem->MoveTo(ClItemIndex(poItem->ItemDef.AuxiliarItemIndex())) == M4_FALSE)
		{
			return M4_ERROR;
		}
	}

	if( ai_oContext.CanWrite( m_poAccess, m_pcNode, itemId ) == M4_TRUE )
	{
		// by default its came from client
		vValue.IsUser = M4_TRUE;
		
		m4VariantType	vOldValue;

		if (poItem->Value.Get(vOldValue) == M4_SUCCESS && M4VariantIsEqual(&vOldValue, &vValue) == 1)
		{
			vValue.IsUser = vOldValue.IsUser;
		}

		if (poItem->Value.Set(vValue) != M4_SUCCESS)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_ITEM_SET_VALUE, m_pcM4OAlias << m_pcNode << itemId );
			return M4_ERROR;
		}
	}
	else 
	{
		// if the values are distintc we generate an error
		m4VariantType vOriginalValue;
		iResult = poItem->Value.Get(vOriginalValue);
		if (iResult != M4_SUCCESS || vOriginalValue != vValue)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_SEC_ITEM_SET_VALUE, itemId << m_pcNode << m_poAccess->GetpCMCR()->GetChannelId() );
			return M4_ERROR;
		}
	}

	return iResult;

  //## end ClM4XMLItem::SetValue%937383066.body
}

m4return_t ClM4XMLItem::SetFileValueByRef (ClM4XMLContext &ai_oContext, const string& ai_sFileName, const string& ai_sFileDescr)
{
  //## begin ClM4XMLItem::SetFileValueByRef%939889209.body preserve=yes
	if (SetValue(ai_oContext,ai_sFileName) != M4_SUCCESS)
		return M4_ERROR;

	m4pcchar_t pcValue = ai_sFileDescr.c_str();
	m4VariantType vValue((m4pchar_t)pcValue);

	// sets the description
	if (m_poNode->RecordSet.Current.Item.Value.SetDescr(vValue) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_ITEM_SET_DESCR, m_pcM4OAlias << m_pcNode << m_pcItem );
		return M4_ERROR;
	}

	return M4_SUCCESS;

  //## end ClM4XMLItem::SetFileValueByRef%939889209.body
}

m4return_t ClM4XMLItem::SetNull (ClM4XMLContext &ai_oContext)
{
  //## begin ClM4XMLItem::SetNull%939630177.body preserve=yes
	if (Move(ai_oContext, M4_TRUE) != M4_SUCCESS)
		return M4_ERROR;

	if (m_iHandle <= 0)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_ITEM_SET_NULL, m_pcM4OAlias << m_pcNode << m_pcItem );
		return M4_ERROR;
	}

	if (m_poNode == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_ITEM_INIT );
		return M4_ERROR;
	}

	m4return_t iResult = m_poNode->RecordSet.Current.Item.Value.SetNULL();
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_ITEM_SET_NULL_VALUE, m_pcM4OAlias << m_pcNode << m_pcItem );
	}

	return iResult;
  //## end ClM4XMLItem::SetNull%939630177.body
}

m4return_t ClM4XMLItem::Parse (const string &ai_sString, m4bool_t ai_bForceData)
{
  //## begin ClM4XMLItem::Parse%937383067.body preserve=yes

	if (m_bIsParsed == M4_TRUE)
		return M4_SUCCESS;

	Reset();

	m4pcchar_t pc = ai_sString.c_str();
	m4pchar_t pcData=NULL;
	m4pchar_t buffer = new m4char_t[ai_sString.size()+1];
	m4int16_t iCount = 0;
	m4bool_t bGo = M4_TRUE, bForceNode = M4_FALSE;
	while (bGo)
	{
		switch(*pc)
		{
		case ' ':
		case '\n':
		case '\r':
			pc++;
			continue;

		case '\0':
			bGo = M4_FALSE;
		case '!':
		case '[':
		case ']':
		case '.':
			if (iCount)
			{
				if (pcData != NULL)
					delete [] pcData;
				pcData = new m4char_t[iCount+1];
				strncpy(pcData,pc-iCount,iCount);
				pcData[iCount] = '\0';
				iCount = 0;
			}
			else
				pcData = NULL;
			break;

		default:
			buffer[iCount] = *(pc++);
			iCount++;
			continue;
		}

		switch(*pc)
		{
		case '!':
			if (m_pcM4OAlias == NULL)
			{
				m_pcM4OAlias = pcData;
				pcData = NULL;
			}
			bForceNode = M4_TRUE;
			break;

		case '[':
		case '.':
			if (m_pcNode == NULL)
			{
				m_pcNode = pcData;
				pcData = NULL;
			}
			bForceNode = M4_FALSE;
			break;

		case '\0':
			if (bForceNode && m_pcNode == NULL)
			{
				m_pcNode = pcData;
				pcData = NULL;
				break;
			}

			if (m_pcItem == NULL)
			{
				m_pcItem = pcData;
				pcData = NULL;
			}
			break;

		case ']':
			if (pcData == NULL || m_pcRegister != NULL)
				break;

			m_pcRegister = pcData;
			pcData = NULL;

			if (*m_pcRegister == '+' || *m_pcRegister == '-')
			{
				// relative register
				m_bRegRelative = M4_TRUE;
				m4pchar_t pcBuffer = new m4char_t[strlen(m_pcRegister)+1];
				sscanf(m_pcRegister,"%s",pcBuffer);
				
				m_sRegisterMin = pcBuffer;
				m_sRegisterMax = "-1";
				
				delete [] pcBuffer;
			}
			else
			if (strstr(m_pcRegister,"-") != NULL)
			{
				m4pchar_t pcBuffer	  = new m4char_t[strlen(m_pcRegister)+1];
				m4pchar_t pcBufferMin = new m4char_t[strlen(m_pcRegister)+1];
				m4pchar_t pcBufferMax = new m4char_t[strlen(m_pcRegister)+1];
				
				strcpy(pcBuffer,m_pcRegister);
				m4pchar_t pcReg = strstr(pcBuffer,"-");
				*pcReg = ' ';
				*pcBufferMin = *pcBufferMax = '\0';

				sscanf(pcBuffer,"%s %s",pcBufferMin,pcBufferMax);
				m_sRegisterMin = pcBufferMin;
				m_sRegisterMax = pcBufferMax;
			
				delete [] pcBufferMin;
				delete [] pcBufferMax;
				delete [] pcBuffer;
			}
			else
			{
			if (!strcmp(m_pcRegister,"*"))
			{
				m_sRegisterMin = "first";
				m_sRegisterMax = "last";
			}
			else
			{ 		
				m4pchar_t pcBuffer = new m4char_t[strlen(m_pcRegister)+1];
				
				sscanf(m_pcRegister,"%s",pcBuffer);
				m_sRegisterMin = pcBuffer;
				m_sRegisterMax = m_sRegisterMin;
				
				delete [] pcBuffer;
			}
			}
			break;
		}

		pc++;

	}

	if (pcData != NULL)
	{
		delete [] pcData;
		pcData = NULL;
	}
	delete buffer;

	// checks node level methods
	if (ai_bForceData == M4_FALSE && m_pcRegister == NULL && 
		m_pcItem == NULL && m_pcNode != NULL)
	{
		m_pcM4OItem = m_pcNode;
		m_pcNode = NULL;
	}

	m_bIsParsed = M4_TRUE;
	return M4_SUCCESS;

  //## end ClM4XMLItem::Parse%937383067.body
}

void ClM4XMLItem::Reset ()
{
  //## begin ClM4XMLItem::Reset%937383068.body preserve=yes

	if (m_pcM4OAlias != NULL)
	{
		delete m_pcM4OAlias;
		m_pcM4OAlias = NULL;
	}
	if (m_pcNode != NULL)
	{
		delete m_pcNode;
		m_pcNode = NULL;
	}
	if (m_pcRegister != NULL)
	{
		delete m_pcRegister;
		m_pcRegister = NULL;
	}
	if (m_pcItem != NULL)
	{
		delete m_pcItem;
		m_pcItem = NULL;
	}
	if (m_pcM4OItem != NULL)
	{
		delete m_pcM4OItem;
		m_pcM4OItem = NULL;
	}
	if (m_pcValue != NULL)
	{
		delete m_pcValue;
		m_pcValue = NULL;
	}

	// resets the stack
	if (m_poAccess != NULL)
	{
		m_poAccess->GetpExecutor()->Stack.Reset();
		m_poAccess = NULL;
	}

	m_bIsParsed = M4_FALSE;
	m_iRegisterMin = m_iRegisterMax = -1;
	m_sResult = "";
	m_poAccess = NULL;
	m_iHandle = -1;
	m_poNode = NULL;
	m_sFilterType = "";
	m_sFilterFormula = "";
	m_bRegRelative = M4_FALSE;
	m_iResultType = -1;
	m_sRegisterMin = "";
	m_sRegisterMax = "";

  //## end ClM4XMLItem::Reset%937383068.body
}

m4return_t ClM4XMLItem::Reset (ClM4XMLContext &ai_oContext, const string& ai_sItem, m4bool_t ai_bUpdate, m4bool_t ai_bRealItem )
{
  //## begin ClM4XMLItem::Reset%938162620.body preserve=yes
	Reset();

	if (Parse(ai_sItem) != M4_SUCCESS)
	{
		Reset();
		return M4_ERROR;
	}

	// moves
	if (Move(ai_oContext,ai_bUpdate,ai_bRealItem) != M4_SUCCESS)
	{
		Reset();
		return M4_ERROR;
	}

	return M4_SUCCESS;
  //## end ClM4XMLItem::Reset%938162620.body
}

m4return_t ClM4XMLItem::Move (ClM4XMLContext &ai_oContext, m4bool_t ai_bUpdate, m4bool_t ai_bRealItem )
{
  //## begin ClM4XMLItem::Move%937470765.body preserve=yes

	m4int32_t iReg = -1;

	// for movebyid (SOAP Persistence)
	m4return_t			iResultRg	= M4_SUCCESS ;	
	m4uint32_t			iUniqueId	= -1 ;
	ClRegisterIndex		iIndex		= -1 ;

	// checks individual setup
	if (Parse() != M4_SUCCESS)
		return M4_ERROR;

	if (m_pcM4OAlias == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_ITEM_INIT );
		return M4_ERROR;
	}

	// gets the channel and access
	ClM4XMLCurrent *poCurrent = ai_oContext.GetCurrentByAlias(m_pcM4OAlias);
	if (poCurrent == NULL)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_ITEM_MOVE_NO_ALIAS, m_pcM4OAlias );
		return M4_ERROR;
	}

	ClAccess *poAccess = poCurrent->poAccess;
	if (poAccess == NULL || poCurrent->poAccessGet == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_ITEM_INIT );
		return M4_ERROR;
	}
	m_poAccess = poAccess;

	// move to the node
	ClNode *poNode = NULL;
	if (m_pcNode != NULL)
	{
		// if the node start with '*' we don't have to move since we'll scan them all
		if (strlen(m_pcNode) > 0 && *m_pcNode == '*')
		{
			// no movement required but we adjust the limits
			m_iRegisterMin = 0;
			m_iRegisterMax = -1; // no max value
			return M4_SUCCESS;
		}

		poNode = poCurrent->poAccessGet->Node.Get(m_pcNode);
		if (poNode == NULL)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_ITEM_INVALID_NODE, m_pcNode << m_pcM4OAlias);
			return M4_ERROR;
		}

		poCurrent->poNodeGet = poNode;
		if (ai_bUpdate == M4_TRUE)
		{
			poNode = poAccess->Node.Get(m_pcNode);
			if (poNode == NULL)
			{
				DUMP_CHLOG_ERRORF ( M4_ERR_XML_ITEM_INVALID_NODE, m_pcNode << m_pcM4OAlias);
				return M4_ERROR;
			}
			poCurrent->poNode = poNode;
		}
	}
	else
	{
		poNode = poCurrent->poNode;
	}

	if (poNode == NULL && m_pcM4OItem == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_ITEM_INIT );
		return M4_ERROR;
	}

	m_poNode = poNode;

	// move to the register
	m_iHandle = -1;
	m4bool_t bMoveToEOF = M4_FALSE;
	if (poNode != NULL)
	{	
		ClAccessRecordSet *poRS = &poNode->RecordSet;

	    if ( m_bIsRegId == M4_TRUE ) 
		{
			// checks special regid's
			if (m_sRegisterMin == "-1") 
			{
				bMoveToEOF = M4_TRUE;
			}
			else 
			{
				// get the unique id (after parsing of course)
				iUniqueId = atoi(m_sRegisterMin.c_str());

				iResultRg = poRS->Register.GetIndexOf( iUniqueId, iIndex ) ;						

				// TODO: check error in log file
				// only move if the index is good and lower than the recordset count.
				if ( iResultRg == M4_ERROR) 
				{
					// can't access the register. Why?
					return M4_ERROR;
				}
				else
				{
					iReg = iIndex;
				}
			}
		}
		else
		{
			// parses min max semantics
			m_iRegisterMin = ParseLimit(m_sRegisterMin,poRS);
			m_iRegisterMax = ParseLimit(m_sRegisterMax,poRS);

			// updates relative registers
			if (m_bRegRelative == M4_TRUE)
			{
				m_iRegisterMin += poRS->Current.Index;
				m_iRegisterMax = m_iRegisterMin;
				m_bRegRelative = M4_FALSE;
			}

			if (m_iRegisterMax >= 0 && (m4uint32_t)m_iRegisterMax < poRS->Count())
			{
				iReg = m_iRegisterMax;
			}
			else
			{
				if (m_iRegisterMin >= 0 && (m4uint32_t)m_iRegisterMin < poRS->Count())
					iReg = m_iRegisterMin;
			}

		}

		if (bMoveToEOF == M4_TRUE)
		{
			poRS->Current.MoveToEOF();
			
			if (ai_bUpdate == M4_TRUE && poCurrent->poNodeGet != NULL)
				poCurrent->poNodeGet->RecordSet.Current.MoveToEOF();
		}
				
		if (bMoveToEOF == M4_FALSE && iReg >= 0)
		{
			poRS->Current[iReg];
			
			if (ai_bUpdate == M4_TRUE && poCurrent->poNodeGet != NULL)
				poCurrent->poNodeGet->RecordSet.Current[iReg];
		}

		// gets the handle
		if (ai_bRealItem == M4_TRUE && m_pcItem != NULL)
			m_iHandle = poRS->Current.Item[m_pcItem].ItemDef.GetHandle();
	}
		
	return M4_SUCCESS;

  //## end ClM4XMLItem::Move%937470765.body
}

void ClM4XMLItem::Init ()
{
  //## begin ClM4XMLItem::Init%937555633.body preserve=yes
	m_pcM4OAlias = NULL;
	m_pcRegister = NULL;
	m_pcNode = NULL;
	m_pcItem = NULL;
	m_pcM4OItem = NULL;
	m_pcValue = NULL;
	m_sResult = "";
	m_iRegisterMin = m_iRegisterMax = -1;
	m_poAccess = NULL;
	m_poNode = NULL;
	m_iHandle = -1;
	m_bIsParsed = M4_FALSE;
	m_sRegisterMin = "";
	m_sRegisterMax = "";
	m_bIsRegId = false;
	m_bResultIsNull = false;
  //## end ClM4XMLItem::Init%937555633.body
}

m4int16_t ClM4XMLItem::GetArgIndex (m4pcchar_t ai_pcArgName)
{
  //## begin ClM4XMLItem::GetArgIndex%937470762.body preserve=yes
	if (m_iHandle <= 0 || m_poAccess == NULL)
		return -1;

	return m_poAccess->GetpCMCR()->GetItemArgumentPositionById(m_iHandle,ai_pcArgName);
  //## end ClM4XMLItem::GetArgIndex%937470762.body
}

void ClM4XMLItem::SetNode (const string &ai_sNode)
{
  //## begin ClM4XMLItem::SetNode%938708977.body preserve=yes
	m_bIsParsed = M4_FALSE;
	m_sNode = ai_sNode;
	m_sRegister = "";
	m_sItem = "";
  //## end ClM4XMLItem::SetNode%938708977.body
}

void ClM4XMLItem::SetRegister (const string &ai_sRegister)
{
  //## begin ClM4XMLItem::SetRegister%938708978.body preserve=yes

	m_bIsRegId = M4_FALSE; 

	m_bIsParsed = M4_FALSE;
	m_sItem = ""; 

	if (ai_sRegister.size() == 0)
	{
		m_sRegister = "current";
	}
	else
	{
		m_sRegister = ai_sRegister;
	}

  //## end ClM4XMLItem::SetRegister%938708978.body
}

void ClM4XMLItem::SetIsById (bool ai_bIsRegId)
{
	m_bIsRegId = ai_bIsRegId; 
}

bool ClM4XMLItem::GetIsById ()
{
	return m_bIsRegId; 
}

void ClM4XMLItem::SetRegisterById (const string &ai_sRegister)
{
	//## begin ClM4XMLItem::SetRegisterById%93870897a.body preserve=yes

	SetRegister (ai_sRegister);	

	m_bIsRegId = M4_TRUE; 
	
  //## end ClM4XMLItem::SetRegisterById%93870897a.body
}

void ClM4XMLItem::SetItem (const string &ai_sItem)
{
  //## begin ClM4XMLItem::SetItem%938708979.body preserve=yes
	m_bIsParsed = M4_FALSE;
	m_sItem = ai_sItem;
  //## end ClM4XMLItem::SetItem%938708979.body
}

m4return_t ClM4XMLItem::Parse ()
{
  //## begin ClM4XMLItem::Parse%938708980.body preserve=yes

	if (m_bIsParsed == M4_TRUE)
		return M4_SUCCESS;

	// checks individual settings
	if (!m_sNode.size())
		return M4_SUCCESS;

	// generates a new string
	m4int16_t iLen = m_sNode.size() + m_sRegister.size() + m_sItem.size() + 5;

	string sNewItem;
	
	sNewItem.reserve(iLen);

	sNewItem.append(m_sNode);

	if (m_sRegister.size())
	{
		sNewItem.append("[");
		sNewItem.append(m_sRegister);
		sNewItem.append("]");
	}

	if (m_sItem.size())
	{
		sNewItem.append(".");
		sNewItem.append(m_sItem);
	}

	return Parse(sNewItem);
	
  //## end ClM4XMLItem::Parse%938708980.body
}

m4return_t ClM4XMLItem::RemoveFilter (ClM4XMLContext &ai_oContext)
{
  //## begin ClM4XMLItem::RemoveFilter%939117432.body preserve=yes
	if (m_pcM4OAlias == NULL || m_pcItem == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_ITEM_INIT );
		return M4_ERROR;
	}

	ClM4XMLCurrent *poCurrent = ai_oContext.GetCurrentByAlias(m_pcM4OAlias);
	if (poCurrent == NULL)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_ITEM_REMOVE_FILTER_NO_ALIAS, m_pcM4OAlias );
		return M4_ERROR;
	}

	// procceed for all filters
	ClNode *poNode = poCurrent->poAccess->Node.Get(m_pcNode);
	m4int16_t i=0;

	for (i=0; i<2; i++)
	{
		if (poNode == NULL)
		{ 
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_ITEM_REMOVE_FILTER_NO_NODE, m_pcNode );
			break;
		}
		
		_ClRegister_Base *poReg = &poNode->RecordSet.Current;
		m4int16_t j;
		for (j=0; j<2; j++)
		{
			if (poReg->Filter[m_pcItem].Delete() != M4_SUCCESS)
				break;
			if (poReg->Filter.UpdateFilter() != M4_SUCCESS)
				break;
			
			poReg = &poNode->RecordSet.Register;
		}
		
		if (j < 2)
			break;
		
		poNode = poCurrent->poAccessGet->Node.Get(m_pcNode);
	}

	if (i < 2)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_ITEM_REMOVE_FILTER, m_pcItem << m_pcM4OAlias );
		return M4_ERROR;
	}

	return M4_SUCCESS;
  //## end ClM4XMLItem::RemoveFilter%939117432.body
}

m4return_t ClM4XMLItem::ExecuteFilter (ClM4XMLContext &ai_oContext, m4int16_t ai_iNArgs, m4pcchar_t *ai_Mode, m4pcchar_t *ai_Name, m4pcchar_t *ai_Value)
{
  //## begin ClM4XMLItem::ExecuteFilter%939117433.body preserve=yes
	
	m4int16_t i=0;
	m4int16_t j=0;


	// implements the sort filter
	if (m_pcM4OAlias == NULL || m_pcItem == NULL || !m_sFilterType.size() || m_pcNode == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_ITEM_INIT );
		return M4_ERROR;
	}

	ClM4XMLCurrent *poCurrent = ai_oContext.GetCurrentByAlias(m_pcM4OAlias);
	if (poCurrent == NULL)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_ITEM_FILTER_NO_ALIAS, m_pcM4OAlias );
		return M4_ERROR;
	}

	// checks init
	if (poCurrent->poAccess == NULL || poCurrent->poAccessGet == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_ITEM_INIT );
		return M4_ERROR;
	}

	m4return_t iResult = M4_SUCCESS;
	
	if (m_sFilterType == "sort")
	{
		m4uint8_t *vOrder = NULL;
		if (ai_iNArgs)
		{
			vOrder = new m4uint8_t[ai_iNArgs];
			for (m4int16_t i=0; i<ai_iNArgs; i++)
				vOrder[i] = stricmp(ai_Mode[i],"asc") ? -1 : 1;
		}

		// procceed
		ClNode *poNode = poCurrent->poAccess->Node.Get(m_pcNode);
		for (i=0; i<2; i++)
		{
			if (poNode == NULL)
			{
				DUMP_CHLOG_ERRORF ( M4_ERR_XML_ITEM_EXECUTE_INVALID_NODE, m_pcNode );
				break;
			}
			
			_ClRegister_Base *poReg = &poNode->RecordSet.Current;
			for (j=0; j<2; j++)
			{
				poReg->Filter[m_pcItem].Delete();
				
				if (poReg->Filter.AddSort(ai_iNArgs,
					(m4pchar_t*)ai_Name,vOrder,m_pcItem) != M4_SUCCESS)
					break;
				
				poReg = &poNode->RecordSet.Register;
			}

			if (j < 2)
				break;

			poNode = poCurrent->poAccessGet->Node.Get(m_pcNode);
		}

		if (i < 2)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_ITEM_APPLY_FILTER, m_pcItem << m_sFilterType.c_str() << m_pcM4OAlias );
			iResult = M4_ERROR;
		}

		if (vOrder != NULL)
			delete [] vOrder;
	}
	
	if (m_sFilterType == "ln4")
	{
		/* Bug 0067491
		Los valores de los argumentos hay que pasarlos en la creación del filtro
		*/
		m4VariantType *pvValues = NULL ;
		
		if( ai_iNArgs > 0 )
		{
			pvValues = new m4VariantType[ ai_iNArgs ] ;

			for( i = 0 ; i < ai_iNArgs ; i++ )
			{
				pvValues[ i ].SetString( (m4pchar_t) ai_Value[ i ] ) ;
			}
		}

		// procceed
		ClNode *poNode = poCurrent->poAccess->Node.Get(m_pcNode);
		for (i=0; i<2; i++)
		{
			if (poNode == NULL)
				break;

			_ClRegister_Base *poReg = &poNode->RecordSet.Current;
			for (j=0; j<2; j++)
			{
				poReg->Filter[m_pcItem].Delete();

				if (poReg->Filter.AddNew((m4pchar_t)m_sFilterFormula.c_str(),
					ClFilterInterface::StaticFilter, ai_iNArgs, (m4pchar_t*)ai_Name, pvValues, 
					m_pcItem) != M4_SUCCESS)
					break;

				if (poReg->Filter.UpdateFilter() != M4_SUCCESS)
					break;
				
				poReg = &poNode->RecordSet.Register;
			}

			if (j < 2)
				break;

			poNode = poCurrent->poAccessGet->Node.Get(m_pcNode);
		}

		if( pvValues != NULL )
		{
			delete [] pvValues ;
			pvValues = NULL ;
		}

		if (i < 2)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_ITEM_APPLY_LN4, m_pcItem << m_pcM4OAlias );
			iResult = M4_ERROR;
		}
	}

	return iResult;

  //## end ClM4XMLItem::ExecuteFilter%939117433.body
}

m4return_t ClM4XMLItem::GetValue (ClM4XMLContext &ai_oContext, string& ao_sValue)
{
  //## begin ClM4XMLItem::GetValue%939630176.body preserve=yes
	if (Move(ai_oContext,M4_FALSE) != M4_SUCCESS)
		return M4_ERROR;

	if (m_iHandle <= 0 || m_pcItem == NULL)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_ITEM_GET_VALUE, m_pcNode << m_pcM4OAlias );
		return M4_ERROR;
	}

	if (m_poNode == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_ITEM_INIT );
		return M4_ERROR;
	}

	m4VariantType vValue;

	if (m_poNode->RecordSet.Register.Item[m_pcItem].Value.Get(vValue) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_ITEM_GET_VALUE_GET, m_pcM4OAlias << m_pcNode << m_pcItem );
		return M4_ERROR;
	}

	// converts the result
	ClVariantConversor varConv;
	varConv.ToCharString(vValue);

	ao_sValue = (m4pchar_t)vValue;

	return M4_SUCCESS;
  //## end ClM4XMLItem::GetValue%939630176.body
}

m4return_t ClM4XMLItem::ConvArg (m4int16_t ai_iIndex, m4bool_t ai_bIsVariableArgs, const string &ai_sValue, const string &ai_sType, m4VariantType& aio_vResult)
{
  //## begin ClM4XMLItem::ConvArg%942245952.body preserve=yes
	if (m_poAccess == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_ITEM_INIT );
		return M4_ERROR;
	}

	// gets the type for the argument
	m4int16_t iM4Type = -1;

	// selects the type
	if (ai_sType.size())
	{
		// prior the explicit type from client
		iM4Type = atoi(ai_sType.c_str());
	}
	else if (ai_bIsVariableArgs == M4_TRUE)
	{
		// default for variable args is string
		iM4Type = M4CL_M4_TYPE_FIXED_STRING;
	}
	else
	{
		// the type from the item argument
		iM4Type = m_poAccess->GetpCMCR()->GetItemArgumentM4TypeByPosition(m_iHandle,ai_iIndex);
	}

	// invoques the conversion
	ClVariantConversor varConv;
	m4VariantType vValue;
	
	if (iM4Type != M4CL_M4_TYPE_NULL)
	{
		vValue.SetString((m4pchar_t)ai_sValue.c_str(), M4_TRUE);
	}

	m4return_t iResult = M4_SUCCESS;
	switch (iM4Type)
	{
		// double
	case M4CL_M4_TYPE_NUMBER:
	case M4CL_M4_TYPE_CURRENCY:
	case M4CL_M4_TYPE_NUM_VARIANT:
		iResult = varConv.ToDouble(vValue);
		if (iResult != M4_SUCCESS)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_CONV_ARG_INVALID_TYPE, iM4Type );
		}
		break;

		// date
	case M4CL_M4_TYPE_DATE:
	case M4CL_M4_TYPE_TIMESTAMP:
	case M4CL_M4_TYPE_HOUR:
		iResult = varConv.ToDate(vValue);
		if (iResult != M4_SUCCESS)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_CONV_ARG_INVALID_TYPE, iM4Type );
		}
		break;

		// invalid input types
	case M4CL_M4_TYPE_BINARY_STRING:
	case M4CL_M4_TYPE_BLOB:
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_CONV_ARG_INVALID, iM4Type );
		iResult = M4_ERROR;

	default:
		break;
	}

	// assignes the result
	if (iResult == M4_SUCCESS)
		aio_vResult = vValue;

	// returns
	return iResult;

  //## end ClM4XMLItem::ConvArg%942245952.body
}

m4int32_t ClM4XMLItem::ParseLimit (string &ai_sValue, ClAccessRecordSet *ai_poRS)
{
  //## begin ClM4XMLItem::ParseLimit%963491328.body preserve=yes
	if (ai_poRS == NULL)
		return -1;

	if (stricmp(ai_sValue.c_str(),"current") == 0 ||
		stricmp(ai_sValue.c_str(),"")        == 0  )
	{
		return ai_poRS->Current.Index;
	}

	if (stricmp(ai_sValue.c_str(),"first") == 0)
		return 0;

	if (stricmp(ai_sValue.c_str(),"last") == 0)
	{
		m4int32_t iCount = ai_poRS->Count();
		if (iCount < 1)
			return 0;
		return iCount -1;
	}

	return atol(ai_sValue.c_str());
  //## end ClM4XMLItem::ParseLimit%963491328.body
}

// Additional Declarations
  //## begin ClM4XMLItem%37DF59240175.declarations preserve=yes
  //## end ClM4XMLItem%37DF59240175.declarations

// Class ClM4XMLItemFilterList 









ClM4XMLItemFilterList::ClM4XMLItemFilterList ()
  //## begin ClM4XMLItemFilterList::ClM4XMLItemFilterList%937555625.hasinit preserve=no
      : m_poContext(NULL), m_bConvRange(M4_FALSE), m_bRecursive(M4_FALSE), m_bSyncCurrent(M4_FALSE), m_bEnableDeltas(M4_FALSE)
  //## end ClM4XMLItemFilterList::ClM4XMLItemFilterList%937555625.hasinit
  //## begin ClM4XMLItemFilterList::ClM4XMLItemFilterList%937555625.initialization preserve=yes
  //## end ClM4XMLItemFilterList::ClM4XMLItemFilterList%937555625.initialization
{
  //## begin ClM4XMLItemFilterList::ClM4XMLItemFilterList%937555625.body preserve=yes
  //## end ClM4XMLItemFilterList::ClM4XMLItemFilterList%937555625.body
}


ClM4XMLItemFilterList::~ClM4XMLItemFilterList ()
{
  //## begin ClM4XMLItemFilterList::~ClM4XMLItemFilterList%937555626.body preserve=yes
  //## end ClM4XMLItemFilterList::~ClM4XMLItemFilterList%937555626.body
}



//## Other Operations (implementation)
m4return_t ClM4XMLItemFilterList::Execute (ClM4XMLStream &ao_oOut)
{
  //## begin ClM4XMLItemFilterList::Execute%937555628.body preserve=yes

	// Updates
	Update();
	if (UpdateVector() != M4_SUCCESS)
		return M4_ERROR;
	m_sNode = "";
	m_sRegister = "";
	m_sItem = "";
	m_bConvRange = M4_FALSE;
	m4return_t iResult = M4_SUCCESS;
	
	// processes the set
	M4ItemSetIt it = m_oItemSet.begin();
	for (it = m_oItemSet.begin(); it != m_oItemSet.end(); it++)
		if(DumpNode(it, NULL, M4_TRUE, ao_oOut) != M4_SUCCESS)
		{
			iResult = M4_ERROR;
			break;
		}

	// resets and exits
	Reset();
	return iResult;

  //## end ClM4XMLItemFilterList::Execute%937555628.body
}

m4return_t ClM4XMLItemFilterList::DumpNode (M4ItemSetIt &ai_iIt, const StSetItem *ai_poSetItem, m4bool_t ai_bDumpData, ClM4XMLStream &ao_oOut)
{
  //## begin ClM4XMLItemFilterList::DumpNode%937826948.body preserve=yes

	if (m_poContext == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_FILTER_LIST_INIT );
		return M4_ERROR;
	}

	// parses the name
	ClM4XMLItem oItem(ai_poSetItem != NULL ? ai_poSetItem->sValue : (*ai_iIt).sValue ,M4_TRUE);

	// goes to the node
	if (oItem.GetM4OAlias() == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_FILTER_LIST_NO_ALIAS );
		return M4_ERROR;
	}
	const string sM4OAlias(oItem.GetM4OAlias());

	m4pchar_t pcM4OAlias = oItem.GetM4OAlias();
	m4pcchar_t pcNode = oItem.GetNode();

	// checks params
	if (pcM4OAlias == NULL || pcNode == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_FILTER_LIST_INIT );
		return M4_ERROR;
	}

	// gets the current
	ClM4XMLCurrent *poCurrent = m_poContext->GetCurrentByAlias(pcM4OAlias);

	if (poCurrent == NULL)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_FILTER_LIST_BAD_ALIAS, pcM4OAlias );
		return M4_ERROR;
	}

	// sets the current register limits - no update
	if (oItem.Move(*m_poContext,M4_FALSE) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_FILTER_LIST_INIT );
		return M4_ERROR;
	}

	// gets a new access
	ClAccess *poAccess = poCurrent->poAccessGet;
	if (poAccess == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_FILTER_LIST_INIT );
		return M4_ERROR;
	}

	// moves to the node
	set<string> nodeSet;
	LoadNodeList(pcNode, nodeSet, poAccess);

	m4bool_t bNodeQuery = M4_FALSE;
	if (pcNode != NULL && strlen(pcNode) > 0 && *pcNode == '*')
	{
		bNodeQuery = M4_TRUE;
	}
	m4uint32_t iRootCount = 0;

	set<string>::iterator nodeId = nodeSet.begin();

	while (nodeId != nodeSet.end()) 
	{

		// updates the node id
		pcNode = (*nodeId++).c_str();

		// processes the node
		ClNode *poNode = &poAccess->Node[pcNode];
		if (poNode == NULL)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_FILTER_LIST_BAD_NODE, pcNode );
			return M4_ERROR;
		}

		// checks the node type depending of the current mode
		if (m_poContext->GetDataMode() == "no_back")
		{
			// if the item is a back item we just ignore it
			if (poNode->GetpNodeDef()->CsType() == M4CL_CSTYPE_BACK)
			{
				return M4_SUCCESS;
			}
		}

		// gets the vector or deltas
		ClVectorDelta* poVDeltaRecords = NULL;
		ClVectorDelta* poVDeltaBlock = NULL;
		ClVectorDelta* poVDeltaNode = NULL;
		if ((*ai_iIt).bEnableDeltas == M4_TRUE)
		{
			ClVVectorDelta *poVVectorDelta = NULL;

			// deltas at node level
			poVVectorDelta = poNode->GetpNode_Data()->GetpRWT()->GetpVVectorDelta();
			if (poVVectorDelta != NULL)
			{
				if (poVVectorDelta->Count() < 1)
				{
					DUMP_CHLOG_ERROR ( M4_ERR_XML_DELTA_INIT );
					return M4_ERROR;
				}
				else
				{
					// gets the vector of deltas
					poVDeltaNode = poVVectorDelta->GetVector(0);
				}
			}
			
			// deltas at block level
			poVVectorDelta = NULL;
			ClBlock_Channel_Proxy *pBCProxy = poNode->RecordSet.Current.GetCurrentBlock();
			if (pBCProxy != NULL && pBCProxy->m_pClBlock != NULL)
			{
				poVVectorDelta = pBCProxy->m_pClBlock->Block.GetpVVectorDelta();
			}

			if (poVVectorDelta != NULL)
			{
				if (poVVectorDelta->Count() < 1)
				{
					DUMP_CHLOG_ERROR ( M4_ERR_XML_DELTA_INIT );
					return M4_ERROR;
				}
				else
				{
					// gets the vector of deltas
					poVDeltaBlock = poVVectorDelta->GetVector(0);
				}
			}

			// deltas at record level
			poVVectorDelta = NULL;
			if (pBCProxy != NULL && pBCProxy->m_pClBlock != NULL)
			{
				poVVectorDelta = pBCProxy->m_pClBlock->Records.GetpVVectorDelta();
			}

			if (poVVectorDelta != NULL)
			{
				if (poVVectorDelta->Count() < 1)
				{
					DUMP_CHLOG_ERROR ( M4_ERR_XML_DELTA_INIT );
					return M4_ERROR;
				}
				else
				{
					// gets the vector of deltas
					poVDeltaRecords = poVVectorDelta->GetVector(0);
				}
			}
		}

		// scans the last effective register
		m4int32_t iNextCurrent = -1;
		if (ai_poSetItem == NULL)
		{
			M4ItemSetIt iItBack = ai_iIt;
			m4int16_t iLevel = LEVEL_REGISTER;
			for (;;)
			{
				if (iLevel == LEVEL_REGISTER)
					iNextCurrent = (*iItBack).iNextCurrent;
				iLevel = (*iItBack).iLevel;
				if (iLevel < LEVEL_REGISTER)
					break;
				iItBack++;
			}
		}
		
		if (iNextCurrent < 0)
		{
			if (m_bSyncCurrent == M4_TRUE)
			{
				// reads the current managed by the server object
				ClAccess *poCurrentAccess = poCurrent->poAccess;
				if (poCurrentAccess == NULL)
				{
					DUMP_CHLOG_ERROR ( M4_ERR_XML_FILTER_LIST_INIT );
					return M4_ERROR;
				}

				// moves to the node
				ClNode *poCurrentNode = &poCurrentAccess->Node[pcNode];
				if (poCurrentNode == NULL)
				{
					DUMP_CHLOG_ERRORF ( M4_ERR_XML_FILTER_LIST_BAD_NODE, poCurrentNode );
					return M4_ERROR;
				}

				// reads the current
				iNextCurrent = poCurrentNode->RecordSet.Current.Index;
			}
			else 
			{
				// compatibility mode
				iNextCurrent = poNode->RecordSet.Current.Index;
			}
		}

		// writes the node tags
		ao_oOut << "<node id=\"" << pcM4OAlias << "!";
		m4bool_t bDoCheckModify = M4_FALSE;
		if (bNodeQuery == M4_TRUE)
		{
			if (iRootCount == 0)
			{
				bDoCheckModify = M4_TRUE;
			}
			ao_oOut << "ROOT_" << iRootCount++;
		}
		else
		{
			ao_oOut << pcNode;
		}
		ao_oOut << "\" nodeId=\"" << pcNode << "\" name=\"";

		m4return_t	iRet = M4_ERROR ;
		m4bool_t	bModified = M4_FALSE ;
		m4pcchar_t	pccInput = NULL ;
		m4pchar_t	pcOutput = NULL ;
		m4pchar_t	pcOrganization = NULL ;
		ClChannel	*poChannel = NULL ;

		poChannel = poAccess->GetpChannel() ;
		poChannel->Organization.Get( pcOrganization ) ;

		pccInput = poNode->GetpNodeDef()->Name() ;
		iRet = M4ReplaceLiterals( poChannel->GetpChannelManager(), pccInput, pcOutput, pcOrganization, bModified ) ;

		if( iRet == M4_SUCCESS && bModified == M4_TRUE && pcOutput != NULL )
		{
			ao_oOut.Transl( pcOutput );
			delete [] pcOutput ;
		}
		else
		{
			ao_oOut.Transl( pccInput );
		}
		
		ao_oOut << "\""
			<< " count=\"" << poNode->RecordSet.Count() << "\"" 
			<< " current=\"" << iNextCurrent << "\"" ;

		if (DumpParentPosition(poNode,ao_oOut,M4_TRUE) != M4_SUCCESS)
			return M4_ERROR;

		// writes the channel modification status
		if (bDoCheckModify == M4_TRUE)
		{
			m4bool_t bIsModified;
			if(poChannel->CheckModify(bIsModified) != M4_SUCCESS)
				return M4_ERROR;

			// writes the flag
			ao_oOut << " ismodified=\"" << (bIsModified == M4_TRUE ? "true" : "false") << "\"";
		}

		// writes the last load info
		m4uint8_t riLastLoadStatus;
		m4uint32_t riLastLoadRows, riLastLoadFetchs;
		poNode->RecordSet.GetLastLoadStatus(riLastLoadStatus, riLastLoadRows, riLastLoadFetchs);
		ao_oOut << " lastloadrows=\""   << riLastLoadRows   << "\"";
		ao_oOut << " lastloadfetchs=\"" << riLastLoadFetchs << "\"";
		ao_oOut << " lastloadstatus=\"" << riLastLoadStatus << "\"";

		// dumps the vector of deltas (sorting and deleting)
		DumpVectorDeltas(poVDeltaRecords, ao_oOut);

		ao_oOut << ">" << endl;

		// writes the iteminfo for all items for non dumped nodes
		{
			// checks the node dump mode
			string sId;
			sId.reserve(strlen(pcM4OAlias)+strlen(pcNode)+2);
			sId =pcM4OAlias;


			// checks the channel dumping
			if (m_poContext->CheckDumpedM4O(sId) == M4_FALSE) {
				m_poContext->AddDumpedM4O(sId);
			}

			// compounds the item
			sId += "!";
			sId += pcNode;
			if (m_poContext->CheckDumpedItem(sId) == M4_FALSE)
			{
				m_poContext->AddDumpedItem(sId);

				ClItem ItIt(poNode->RecordSet.Current.Item);

				// adds the items of scope node and block
				ItIt.SetMode(M4DM_SCOPE_NODE | M4DM_SCOPE_BLOCK | M4DM_SCOPE_REGISTER, M4DM_TYPE_ALL);
				
				// scans for items
				for (ItIt.Begin();ItIt.GetIndex() != M4DM_EOF_INDEX;ItIt.Next())
					DumpItemDef(ItIt,ao_oOut);
			}
		}

		// checks for node and block level items
		if (oItem.GetItem() == NULL)
		{
			ClItem ItIt(poNode->RecordSet.Current.Item);

			// adds the items of scope node
			if (poVDeltaNode == NULL || poVDeltaNode->ThereIsAnyChange())
			{
				ItIt.SetMode(M4DM_SCOPE_NODE, M4DM_TYPE_PROPERTY | M4DM_TYPE_FIELD | M4DM_TYPE_CONCEPT);
				
				// scans for items
				for (ItIt.Begin();ItIt.GetIndex() != M4DM_EOF_INDEX;ItIt.Next())
					DumpItem(sM4OAlias,ItIt,ao_oOut);
			}

			// adds the items of scope block if we have a non null block
			if (poNode->RecordSet.GetCurrentBlock()->IsValid() == M4_TRUE) {
				if (ai_bDumpData == M4_TRUE && (poVDeltaBlock == NULL || poVDeltaBlock->ThereIsAnyChange()))
				{
					ItIt.SetMode(M4DM_SCOPE_BLOCK, M4DM_TYPE_PROPERTY | M4DM_TYPE_FIELD | M4DM_TYPE_CONCEPT);
					
					// scans for items
					for (ItIt.Begin();ItIt.GetIndex() != M4DM_EOF_INDEX;ItIt.Next())
						DumpItem(sM4OAlias,ItIt,ao_oOut);
				}
			}
		}

		// checks for recursive dump for BB sons. We only do it if not in m4jsapi mode
		if ((*ai_iIt).bRecursive == M4_TRUE && m_poContext->GetUseFunctionalGroup(poAccess) == M4_FALSE) {
			
			// gets the child nodes
			m4uint32_t hNodeHandle = poNode->GetpNodeDef()->GetHandle();
			ClCompiledMCR *pCMCR = poNode->GetpNodeDef()->GetpCMCR();

			// dumps BB's
			m4uint16_t nBB = pCMCR->GetNodeNumberOfBBConnectors(hNodeHandle);
			m4uint16_t iBBPos = pCMCR->GetNodeBBFirstConnector(hNodeHandle);
			for (m4uint16_t counter = 0; counter < nBB ; counter ++, iBBPos++ ) {
				m4uint32_t hConnector = pCMCR->GetNodeConnectorHandleByPosition(hNodeHandle,iBBPos);
				m4uint32_t hSonNode = pCMCR->GetNodeConnectorSonNodeHandle(hConnector);
				m4pcchar_t pcSonNode = pCMCR->GetNodeId(hSonNode);

				// builds the item code
				StSetItem oItem;
				oItem.sValue = sM4OAlias + "!" + pcSonNode + "[*]";

				// calls it recursively
				if (DumpNode(ai_iIt, &oItem, ai_bDumpData, ao_oOut ) != M4_SUCCESS)
					return M4_ERROR;
			}
		}

		// Dump child RB items
		m4return_t iResult = M4_SUCCESS;

		// if no data mode we only dump node items and metadata
		if (ai_bDumpData == M4_FALSE)
		{
			// dumps info recursively
			// wee need this due to security propagation and node items
			if (m_poContext->GetUseFunctionalGroup(poAccess) == M4_TRUE)
			{
				if (DumpChildRBNodes(sM4OAlias, ai_iIt, poNode->GetpNodeDef(), M4_FALSE, ao_oOut) != M4_SUCCESS) 
				{
					return M4_ERROR;
				}
			}
		}
		else
		{
			// writes registers values and metadata recursively
			for (;;)
			{
				// checks for registers
				m4int32_t iRegMin=-1, iRegMax = -1;
				if (oItem.GetRegister() != NULL)
				{
					iRegMin = oItem.GetRegisterMin();
					iRegMax = oItem.GetRegisterMax();
				}
				
				// checks limits
				if (iRegMin < 0 && iRegMax < 0)
				{
					iRegMin = poNode->RecordSet.Current.Index;
					iRegMax = iRegMin;
				}
				else
					if (iRegMin < 0)
					{
						iRegMin = 0;
					}
					else
					{
						if (iRegMax < 0)
						{
							iRegMax = poNode->RecordSet.Count() - 1;
						}
					}
				
				// dump registers
				M4ItemSetIt iItBack = ai_iIt;
				m4bool_t bDumped = M4_FALSE;
				for (m4int32_t i=iRegMin; i>=0 && i<=iRegMax; i++)
				{
					bDumped = M4_TRUE;
					poNode->RecordSet.Current[i];
					DumpRegister(sM4OAlias, ai_iIt, &oItem,poNode->RecordSet.Register[i], poVDeltaRecords, ao_oOut);
					if (i != iRegMax)
						ai_iIt = iItBack;
				}

				// dumps recursively if there are no records
				if (bDumped == M4_FALSE && m_poContext->GetUseFunctionalGroup(poAccess) == M4_TRUE)
				{
					if (DumpChildRBNodes(sM4OAlias, ai_iIt, poNode->GetpNodeDef(), M4_FALSE, ao_oOut) != M4_SUCCESS) 
					{
						return M4_ERROR;
					}
				}

				// checks for next register level
				if ((*ai_iIt).iLevel != LEVEL_REGISTER)
					break;

				// goes to the next register
				ai_iIt++;
				if (oItem.Reset(*m_poContext,(*ai_iIt).sValue) != M4_SUCCESS)
				{
					iResult = M4_ERROR;
					break;
				}
			}

		}
			
		// resets the current
		if (iNextCurrent >= 0)
			poNode->RecordSet.Current[iNextCurrent];

		// closes the tag
		ao_oOut << "</node>" << endl;

		if (iResult != M4_SUCCESS)
		{
			return iResult;
		}
		
	}

	return M4_SUCCESS;
  //## end ClM4XMLItemFilterList::DumpNode%937826948.body
}

m4void_t ClM4XMLItemFilterList::DumpVectorDeltas (ClVectorDelta* ai_oVDelta, ClM4XMLStream &ao_oOut)
{
  //## begin ClM4XMLItemFilterList::GetDeletedRecords%937826949.body preserve=yes
	if (ai_oVDelta == NULL)
	{
		return;
	}

	// composes the response
	m4uint32_t nRecords = ai_oVDelta->Count();
	m4bool_t bFirst = M4_TRUE;
	for (m4uint32_t i=0; i<nRecords; ++i)
	{
		if (bFirst)
		{
			bFirst = M4_FALSE;
			ao_oOut << " vdeltas=\"";
		}
		else 
		{
			ao_oOut << ",";
		}

		ao_oOut << (m4uint32_t)ai_oVDelta->ClientIndex(i);
	}

	if (bFirst == M4_FALSE)
	{
		ao_oOut << "\"";
	}
  //## end ClM4XMLItemFilterList::GetDeletedRecords%937826948.body
}

m4void_t ClM4XMLItemFilterList::LoadNodeList (m4pcchar_t ai_pcNodeId, set<string> &ao_oNodeList, ClAccess *ai_poAccess)
{
  //## begin ClM4XMLItemFilterList::LoadNodeList%937826949.body preserve=yes

	// lets check the particular case
	m4bool_t bDoNoBack = M4_FALSE;
	m4bool_t bDoAll = M4_FALSE;
	if (strcmpi(ai_pcNodeId, "*{no_back}") == 0)
	{
		// today we only support the mapped case (m4jsapi)
		bDoNoBack = M4_TRUE;
	}
	else {
		if (strcmp(ai_pcNodeId, "*") == 0) 
		{
			bDoAll = M4_TRUE;
		}
	}

	// checks the normal case
	if (bDoNoBack == M4_FALSE && bDoAll == M4_FALSE)
	{
		// we only return the very same node id (default behavior)
		ao_oNodeList.insert(ai_pcNodeId);
		return;
	}

	// ok, lets traverse the root nodes
	ClCompiledMCR *poCMCR = ai_poAccess->GetpCMCR();
	m4uint16_t iLength = poCMCR->GetChannelNumberOfRoots() ;
	
	for( int i = 0 ; i < iLength ; i++ )
	{
		m4uint32_t iNodeHandle = poCMCR->GetChannelRootHandleByPosition( i ) ;

		// ignores QBF's if we're not in non-back mode
		if ( bDoNoBack == M4_FALSE )
		{
			m4uint8_t iIsQBF = poCMCR->GetNodeIsQBFFilter( iNodeHandle ) ;

			// ignores QBF's nodes
			if (iIsQBF != 0)
			{
				continue;
			}
		}
		
		// ok, checks the node type
		if (bDoNoBack == M4_TRUE && poCMCR->GetNodeCsType( iNodeHandle ) == M4CL_CSTYPE_BACK)
		{
			// if no back is requires and the node is back we ignore it
			continue;
		}

		// gets the node id
		m4pcchar_t pcNodeId = poCMCR->GetNodeId( iNodeHandle );
		ao_oNodeList.insert( pcNodeId );
	}
  //## begin ClM4XMLItemFilterList::LoadNodeList%937826949.body preserve=yes
}

m4return_t ClM4XMLItemFilterList::DumpRegister (const string &ai_sM4OAlias, M4ItemSetIt &ai_iIt, ClM4XMLItem *ai_poItem, ClRegister &ai_oRegister, ClVectorDelta* ai_oVDelta, ClM4XMLStream &ao_oOut)
{
  //## begin ClM4XMLItemFilterList::DumpRegister%937826949.body preserve=yes

	if (m_poContext == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_FILTER_LIST_INIT );
		return M4_ERROR;
	}

	// skips null registers
	m4uint32_t iIndex = ai_oRegister.Index;
	if (iIndex == -1)
	{
		DUMP_CHLOG_DEBUG ( M4_ERR_XML_FILTER_LIST_BAD_REGISTER );
		return M4_SUCCESS;
	}

	// checks the delta serialization mode
	m4bool_t bHasDeltaChanges = M4_FALSE;
	if (ai_oVDelta != NULL)
	{
		if (ai_oVDelta->Count() > iIndex)
		{
			if (ai_oVDelta->GetInsertedByPos(iIndex) || ai_oVDelta->GetModifiedByPos(iIndex))
			{
				bHasDeltaChanges = M4_TRUE;
			}
		}
	}

	// gets the record id (SOAP Persistence)
	m4return_t			iResultId	= M4_SUCCESS ;
	m4uint32_t			iUniqueId	= -1 ;

	// record status
	m4uint8_t			uiStatus;
	m4return_t			iResult = GetRecordStatus(ai_oRegister, uiStatus);
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_DEBUG ( M4_ERR_XML_FILTER_LIST_BAD_REGISTER );
		return M4_ERROR;
	}

	// retrieves the record id
	iResultId = ai_oRegister.GetRecordID( iUniqueId ) ;


	// writes the register tag with the position (called "index")
	ao_oOut << "<register index=\"" << iIndex << "\"";

	if( iResultId == M4_SUCCESS )
	{
		ao_oOut << " id=\"" << (m4int32_t)iUniqueId << "\"";
	}

	// writes the register status
	ao_oOut << " status=\"" << uiStatus << "\"";

	// writes the element end
	ao_oOut << ">" << endl;

	// loop for items in the register
	// checks for specific items
	if (ai_oVDelta == NULL || bHasDeltaChanges)
	{
		m4pchar_t pcItem = ai_poItem->GetItem();
		iResult = M4_SUCCESS;
		if (pcItem != NULL)
		{
			DumpItem(ai_sM4OAlias,ai_oRegister.Item[pcItem],ao_oOut);
			ClM4XMLItem oItem;
			for (;;)
			{
				if ((*ai_iIt).iLevel != LEVEL_ITEM)
					break;
				ai_iIt++;
				if (oItem.Reset(*m_poContext,(*ai_iIt).sValue) != M4_SUCCESS)
				{
					iResult = M4_ERROR;
					break;
				}
				pcItem = oItem.GetItem();
				DumpItem(ai_sM4OAlias,ai_oRegister.Item[pcItem],ao_oOut);
			}
		}
		else
		{
			// write all items
			// adds the items of scope register
			ClItem ItIt(ai_oRegister.Item);
			ItIt.SetMode(M4DM_SCOPE_REGISTER,M4DM_TYPE_PROPERTY | M4DM_TYPE_FIELD | M4DM_TYPE_CONCEPT);
			
			// scans for items
			for (ItIt.Begin();ItIt.GetIndex() != M4DM_EOF_INDEX;ItIt.Next())
				DumpItem(ai_sM4OAlias,ItIt,ao_oOut);
		}
	}

	// checks for recursive dump
	if ((*ai_iIt).bRecursive == M4_TRUE) {
		if (DumpChildRBNodes(ai_sM4OAlias, ai_iIt, ai_oRegister.GetpNodeDef(), M4_TRUE, ao_oOut) != M4_SUCCESS) 
		{
			return M4_ERROR;
		}
	}
		
	// writes the register tag
	ao_oOut << "</register>" << endl;

	return iResult;
  //## end ClM4XMLItemFilterList::DumpRegister%937826949.body
}

m4return_t ClM4XMLItemFilterList::DumpChildRBNodes (const string &ai_sM4OAlias, M4ItemSetIt &ai_iIt, ClNodeDef *ai_poNodeDef, m4bool_t ai_bDumpData, ClM4XMLStream &ao_oOut)
{
	// gets the child nodes
	m4uint32_t hNodeHandle = ai_poNodeDef->GetHandle();
	ClCompiledMCR *pCMCR = ai_poNodeDef->GetpCMCR();

	// dumps RB's
	m4uint16_t nRB = pCMCR->GetNodeNumberOfRBConnectors(hNodeHandle);
	m4uint16_t iRBPos = pCMCR->GetNodeRBFirstConnector(hNodeHandle);
	for (m4uint16_t counter = 0; counter < nRB ; counter ++, iRBPos++ ) {
		m4uint32_t hConnector = pCMCR->GetNodeConnectorHandleByPosition(hNodeHandle,iRBPos);
		m4uint32_t hSonNode = pCMCR->GetNodeConnectorSonNodeHandle(hConnector);
		m4pcchar_t pcSonNode = pCMCR->GetNodeId(hSonNode);

		// builds the item code
		StSetItem oItem;
		oItem.sValue = ai_sM4OAlias + "!" + pcSonNode + "[*]";

		// calls it recursively
		if (DumpNode(ai_iIt, &oItem, ai_bDumpData, ao_oOut) != M4_SUCCESS)
			return M4_ERROR;
	}

	return M4_SUCCESS;
}

m4return_t ClM4XMLItemFilterList::GetRecordStatus (ClRegister &ai_oRegister, m4uint8_t &ao_uiStatus)
{
	m4bool_t bStatus = M4_FALSE;

	if (ai_oRegister.IsDeleted(bStatus) != M4_SUCCESS) {
		return M4_ERROR;
	}
	else {
		if (bStatus == M4_TRUE) {
			ao_uiStatus = M4XML_RECORD_STATUS_DELETED;
			return M4_SUCCESS;
		}
	}

	if (ai_oRegister.IsNew(bStatus) != M4_SUCCESS) {
		return M4_ERROR;
	}
	else {
		if (bStatus == M4_TRUE) {
			ao_uiStatus = M4XML_RECORD_STATUS_NEW;
			return M4_SUCCESS;
		}
	}

	if (ai_oRegister.IsUpdated(bStatus) != M4_SUCCESS) {
		return M4_ERROR;
	}
	else {
		if (bStatus == M4_TRUE) {
			ao_uiStatus = M4XML_RECORD_STATUS_MODIFIED;
			return M4_SUCCESS;
		}
	}

	ao_uiStatus = M4XML_RECORD_STATUS_NORMAL;
	return M4_SUCCESS;
}

m4return_t ClM4XMLItemFilterList::DumpItem (const string &ai_sM4OAlias, ClItem &ai_iItem, ClM4XMLStream &ao_oOut)
{
  //## begin ClM4XMLItemFilterList::DumpItem%937826950.body preserve=yes

	if (m_poContext == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_FILTER_LIST_INIT );
		return M4_ERROR;
	}

	// checks the data mode
	if (m_poContext->GetDataMode() == "no_back")
	{
		// if the item is a back item we just ignore it
		if (ai_iItem.ItemDef.CsType() == M4CL_CSTYPE_BACK)
		{
			return M4_SUCCESS;
		}
	}

	// gets the value
	m4return_t iResult = M4_SUCCESS;
	ClVariantConversor varConv;
	m4VariantType vValue;
	ClSize oSize;
	m4bool_t bCanRead = M4_FALSE;

	if( m_poContext->CanRead( ai_iItem.GetpAccess(), ai_iItem.GetpNodeDef()->Id(), ai_iItem.ItemDef.Id() ) == M4_TRUE )
	{
		bCanRead = M4_TRUE;

		if (ai_iItem.Value.Get(vValue) != M4_SUCCESS)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_ITEM_GET_VALUE_GET, ai_sM4OAlias.c_str() << ai_iItem.GetpNodeDef()->Id() << ai_iItem.ItemDef.Id() );
			return M4_ERROR;
		}
	}
	
	m4uint16_t iM4Type = ai_iItem.ItemDef.M4Type();

	m4bool_t bResultIsNull = ( vValue.Type == M4CL_CPP_TYPE_NULL );

	if (iM4Type != M4CL_M4_TYPE_BINARY_STRING)
	{
		varConv.ToCharString(vValue);
	}

	// processes encrypted values
	m4bool_t bIsEncrypted = M4_FALSE;
	if ( iM4Type != M4CL_M4_TYPE_BLOB ) 
	{
		string sFuncGroups;
		bIsEncrypted = m_poContext->IsEncrypted( ai_iItem.GetpAccess(), ai_iItem.GetpNodeDef()->Id(), ai_iItem.ItemDef.Id(), sFuncGroups );
		if ( bIsEncrypted == M4_TRUE ) 
		{
			m4bool_t bEncryptedHasValue = (bCanRead == M4_TRUE) && bResultIsNull == M4_FALSE && ((m4pchar_t)vValue) != NULL;
			string sValue = bEncryptedHasValue ? (m4pchar_t)vValue : "";
			m_poContext->Encrypt(sValue, bEncryptedHasValue == M4_FALSE, sFuncGroups);
			vValue = m4VariantType((m4pchar_t)sValue.c_str(), M4_TRUE);
			bResultIsNull = M4_FALSE;
		}
	}

	// writes the item tag
	ao_oOut << "<item" << " id=\"" << ai_iItem.ItemDef.Id() << "\"";

	// checks for blob item
	m4int32_t iBlobRef = -1;
	string sFileExtension;
	m4uint32_t lFileSize = 0;
	if (iM4Type == M4CL_M4_TYPE_BLOB)
	{
		iBlobRef = m_poContext->AddOutFileRef(ao_oOut.GetpDataStorage(),(m4pchar_t)vValue, sFileExtension, lFileSize);
		ao_oOut << " fileext=\"" << sFileExtension.c_str() << "\"";
		ao_oOut << " filesize=\"" << lFileSize << "\"";
	}
	
	if (iBlobRef >= 0)
		ao_oOut << " fileindex=\"" << iBlobRef << "\"";
	
	if (vValue.Type != M4CL_CPP_TYPE_NULL && iM4Type != M4CL_M4_TYPE_BLOB)
	{
		m4uint32_t iLen = (iM4Type == M4CL_M4_TYPE_BINARY_STRING) ?
			ai_iItem.Value.Size() : strlen((m4pchar_t)vValue);

		if (ao_oOut.IsBin((m4uint8_t*)(m4pchar_t)vValue,iLen))
		{
			ao_oOut << " value64=\"";
			if (ao_oOut.Transl64((m4pchar_t)vValue,iLen) != M4_SUCCESS)
			{
				DUMP_CHLOG_ERRORF ( M4_ERR_XML_FILTER_LIST_DUMP_ITEM, ai_iItem.ItemDef.Id() << ai_sM4OAlias.c_str());
				iResult = M4_ERROR;
			}
			ao_oOut << "\"";
		}
		else
		{
			// checks the null status
			if (bResultIsNull == M4_FALSE || m_poContext->GetEnableNulls() == M4_FALSE)
			{
				ao_oOut << " value=\"";
				ao_oOut.Transl((m4pchar_t)vValue);
				ao_oOut << "\"";
			}
		}
	}

	if (iM4Type == M4CL_M4_TYPE_BLOB)
	{
		m4pcchar_t pcDescr = ai_iItem.Value.GetDescr();
		if (pcDescr != NULL)
		{
			m4uint32_t iLen = strlen(pcDescr);
			if (ao_oOut.IsBin((m4uint8_t*)pcDescr,iLen))
			{
				ao_oOut << " descr64=\"";
				if (ao_oOut.Transl64(pcDescr,iLen) != M4_SUCCESS)
				{
					DUMP_CHLOG_ERRORF ( M4_ERR_XML_FILTER_LIST_DESCR, ai_iItem.ItemDef.Id() << ai_sM4OAlias.c_str() );
					iResult = M4_ERROR;
				}
			}
			else
			{
				ao_oOut << " descr=\"";
				ao_oOut.Transl(pcDescr);
			}
			ao_oOut << "\"";
		}
	}

	// we don't translate encrypted values
	if ( bIsEncrypted == M4_FALSE  )
	{
		DumpTranslation(ai_sM4OAlias,ai_iItem.ItemDef.GetHandle(),vValue,ao_oOut);
	}

	ao_oOut << "/>" << endl;

	return iResult;

  //## end ClM4XMLItemFilterList::DumpItem%937826950.body
}

m4return_t ClM4XMLItemFilterList::DumpItemDef (ClItem &ai_iItem, ClM4XMLStream &ao_oOut)
{
  //## begin ClM4XMLItemFilterList::DumpItemDef%940504733.body preserve=yes

	// gets the type
	m4char_t pcType [32];
	sprintf(pcType,"%d",ai_iItem.ItemDef.Type());

	m4char_t pcM4Type [32];
	m4uint16_t iM4Type = ai_iItem.ItemDef.M4Type();
	sprintf(pcM4Type,"%d",iM4Type);

	// writes the item tag
	ao_oOut << "<iteminfo" <<
		" id=\"" << ai_iItem.ItemDef.Id() << "\"" <<
		" name=\"";

	m4return_t	iRet = M4_ERROR ;
	m4bool_t	bModified = M4_FALSE ;
	m4pcchar_t	pccInput = NULL ;
	m4pchar_t	pcOutput = NULL ;
	m4pchar_t	pcOrganization = NULL ;
	ClChannel	*poChannel = NULL ;

	poChannel = ai_iItem.GetpAccess()->GetpChannel() ;
	poChannel->Organization.Get( pcOrganization ) ;

	pccInput = ai_iItem.ItemDef.Name() ;
	iRet = M4ReplaceLiterals( poChannel->GetpChannelManager(), pccInput, pcOutput, pcOrganization, bModified ) ;

	if( iRet == M4_SUCCESS && bModified == M4_TRUE && pcOutput != NULL )
	{
		ao_oOut.Transl( pcOutput );
		delete [] pcOutput ;
	}
	else
	{
		ao_oOut.Transl( pccInput );
	}

	ao_oOut << "\"" <<
		" scope=\"" << ai_iItem.ItemDef.Scope() << "\"";

	m4pcchar_t pcAuxItem = ai_iItem.ItemDef.AuxiliarItem();
	if (pcAuxItem != NULL && strlen(pcAuxItem))
		ao_oOut << " auxid=\"" << pcAuxItem << "\"";

	// checks encryption
	m4uint32_t uiSecurityMask = 0;
	if ( m_poContext != NULL &&
		(uiSecurityMask = m_poContext->GetSecurityMask(ai_iItem.GetpAccess(), ai_iItem.GetpNodeDef()->Id(), ai_iItem.ItemDef.Id())) != 0)
	{
		ao_oOut << " securitymask=\"" << uiSecurityMask << "\"";
	}
	
	ao_oOut << " type=\"" << pcType << "\" m4type=\"" << pcM4Type << "\"/>" << endl;

	return M4_SUCCESS;
  //## end ClM4XMLItemFilterList::DumpItemDef%940504733.body
}

void ClM4XMLItemFilterList::DumpTranslation (const string &ai_sM4OAlias, m4int32_t ai_iHandle, const m4VariantType& ai_vValue, ClM4XMLStream &ao_oOut)
{
  //## begin ClM4XMLItemFilterList::DumpTranslation%939372147.body preserve=yes

	if (m_poContext == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_FILTER_LIST_INIT );
		return;
	}

	// computes the translation
	m4VariantType vValueOut;
	m4bool_t bFound = M4_FALSE;

	if (m_poContext->Translate(ai_sM4OAlias,ai_iHandle,ai_vValue,vValueOut,bFound) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_FILTER_LIST_DUMP_TRANSL );
		return;
	}

	if (bFound == M4_FALSE)
		return;
	
	ClVariantConversor varConv;
	varConv.ToCharString(vValueOut);
	ao_oOut << " trans=\"";
	ao_oOut.Transl((m4pchar_t)vValueOut);
	ao_oOut << "\"";
  //## end ClM4XMLItemFilterList::DumpTranslation%939372147.body
}

m4return_t ClM4XMLItemFilterList::DumpParentPosition (ClNode* ai_poNode, ClM4XMLStream &ao_oOut, m4bool_t ai_bFirst)
{
  //## begin ClM4XMLItemFilterList::DumpParentPosition%941119357.body preserve=yes

	if (ai_poNode == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_FILTER_LIST_INIT );
		return M4_ERROR;
	}

	// checks for parents
	ClNodeDef *poNodeDef = ai_poNode->GetpNodeDef();
	m4int32_t iNConn = poNodeDef->GetNodeNumberOfInvConnectors();
	
	if (!iNConn)
		return M4_SUCCESS;

	// scans connectors
	m4bool_t bWrite = M4_FALSE;
	for (m4uint16_t i=0; i<iNConn; i++)
	{
		m4uint32_t iConnHandle = poNodeDef->GetNodeInvConnectorHandleByPosition(i);
		if (!iConnHandle)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_FILTER_LIST_INV_CONNECTOR );
			return M4_ERROR;
		}

		// checks the type
		if (poNodeDef->GetNodeConnectorType(iConnHandle) == M4CL_NODE_CONNECTOR_TYPE_FREE)
			continue;

		// writes the info
		if (bWrite == M4_FALSE && ai_bFirst == M4_TRUE)
		{
			bWrite = M4_TRUE;
			ao_oOut << " parentpos=\"";
		}
		else
			ao_oOut << ",";

		// writes the node index
		m4uint32_t iParentNodeHandle = poNodeDef->GetNodeConnectorFatherNodeHandle(iConnHandle);
		if (!iParentNodeHandle)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_FILTER_LIST_INVALID_PARENT );
			return M4_ERROR;
		}

		ClNode *poParentNode = ai_poNode->GetpAccess()->Node.Get(ClHandle(iParentNodeHandle));
		if (poParentNode == NULL)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_FILTER_LIST_NO_PARENT );
			return M4_ERROR;
		}

		// writes the id and index
		ClNodeDef *poParentNodeDef = poParentNode->GetpNodeDef();
		ao_oOut << poParentNodeDef->Id() << "[" <<
			poParentNode->RecordSet.Current.Index << "]";

		// writes parent nodes
		if (DumpParentPosition(poParentNode,ao_oOut) != M4_SUCCESS)
			return M4_ERROR;

		// goes to the next node
	}

	// ends the dump
	if (bWrite == M4_TRUE && ai_bFirst == M4_TRUE)
		ao_oOut << "\"";

	return M4_SUCCESS;
  //## end ClM4XMLItemFilterList::DumpParentPosition%941119357.body
}

m4return_t ClM4XMLItemFilterList::Reset (ClM4XMLContext *ai_poContext)
{
  //## begin ClM4XMLItemFilterList::Reset%938437889.body preserve=yes

	// resets previous state
	// if the argument is NULL we just terminate
	if (ai_poContext != NULL)
	{
		if (m_poContext != NULL)
		{
			Update();
		}
	}
	else
	{
		Reset();
	}

	m_poContext = ai_poContext;
	return M4_SUCCESS;
  //## end ClM4XMLItemFilterList::Reset%938437889.body
}

m4return_t ClM4XMLItemFilterList::AddNode (const string &ai_sNode, const string &ai_sRecursive, 
										   const string &ai_bSyncCurrent, const string &ai_sEnableDeltas)
{
  //## begin ClM4XMLItemFilterList::AddNode%938437890.body preserve=yes

	// adds pending tags
	if (m_sNode != "")
		Update();

	// gets the node
	ClM4XMLItem oItem(ai_sNode, M4_TRUE);
	if (oItem.GetNode() != NULL)
		m_sNode = string(oItem.GetNode());
	
	// gets the alias
	if (oItem.GetM4OAlias() != NULL)
		m_sM4OAlias = string(oItem.GetM4OAlias());

	m_bRecursive = (ai_sRecursive == "yes") ? M4_TRUE : M4_FALSE;
	m_sRegister = "";
	m_sItem = "";
	m_bConvRange = M4_FALSE;
	m_bSyncCurrent = (ai_bSyncCurrent == "yes") ? M4_TRUE : M4_FALSE;
	m_bEnableDeltas = (ai_sEnableDeltas == "yes") ? M4_TRUE : M4_FALSE;

	return M4_SUCCESS;

  //## end ClM4XMLItemFilterList::AddNode%938437890.body
}

m4return_t ClM4XMLItemFilterList::AddRegister (const string &ai_sRegister, m4bool_t ai_bConvRange)
{
  //## begin ClM4XMLItemFilterList::AddRegister%938437891.body preserve=yes
	// adds pending tags
	if (m_sRegister != "")
		Update();

	m_bConvRange = ai_bConvRange;

	// checks for default register
	if (ai_sRegister == "")
	{
		m4char_t pcReg[64];
		ClM4XMLCurrent *poCurrent = m_poContext->GetCurrentByAlias(m_sM4OAlias);
		if (poCurrent == NULL)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_FILTER_LIST_BAD_ALIAS, m_sM4OAlias.c_str() );
			return M4_ERROR;
		}

		if (poCurrent->poNodeGet == NULL)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_FILTER_LIST_INIT );
			return M4_ERROR;
		}

		sprintf(pcReg,"%d",(m4int32_t)poCurrent->poNodeGet->RecordSet.Current.Index);
		m_sRegister = pcReg;
	}
	else
		// gets the register
		m_sRegister = ai_sRegister;

	m_sItem = "";

	return M4_SUCCESS;
  //## end ClM4XMLItemFilterList::AddRegister%938437891.body
}

m4return_t ClM4XMLItemFilterList::AddItem (const string &ai_sItem)
{
  //## begin ClM4XMLItemFilterList::AddItem%937555634.body preserve=yes

	// adds pending tags
	if (m_sItem != "")
		Update();

	// gets the item
	m_sItem = ai_sItem;
	
	return M4_SUCCESS;
  //## end ClM4XMLItemFilterList::AddItem%937555634.body
}

void ClM4XMLItemFilterList::Update ()
{
  //## begin ClM4XMLItemFilterList::Update%938437892.body preserve=yes

	if (m_poContext == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_FILTER_LIST_INIT );
		return;
	}

	// checks initial values
	if (!m_sM4OAlias.size() || !m_sNode.size())
		return;
	
	// adds the item as an element
	m4int16_t iLen = m_sM4OAlias.size() + m_sNode.size() + m_sRegister.size() + m_sItem.size() + 6;

	string sNewItem;
	m4int32_t iNextCurrent = -1;
	
	sNewItem.reserve(iLen);

	sNewItem.append(m_sM4OAlias);
	sNewItem.append("!");
	sNewItem.append(m_sNode);

	m4return_t iResult = M4_SUCCESS;
	if (m_sRegister.size())
	{
		m4pcchar_t pcReg = m_sRegister.c_str();
		m4char_t pcReg2[64] = "";
		if (*pcReg == '+' || *pcReg == '-')
		{
			ClM4XMLCurrent *poCurrent = m_poContext->GetCurrentByAlias(m_sM4OAlias);
			if (poCurrent != NULL && poCurrent->poNodeGet != NULL)
			{
				m4int32_t iCurrent = poCurrent->poNodeGet->RecordSet.Current.Index;
				if (iCurrent >= 0)
				{
					m4int32_t iOffset = atol(pcReg);
					if (m_bConvRange == M4_FALSE)
						sprintf(pcReg2,"%d",iOffset+iCurrent);
					else
					{
						m4int32_t iMax = iCurrent + iOffset, iMin = iCurrent;
						m4int32_t iMaxReg = poCurrent->poNodeGet->RecordSet.Count()-1;
						
						// prepares indexes
						if (iMax < iMin)
						{
							m4int32_t iBack = iMax;
							iMax = iMin;
							iMin = iBack;
						}
						
						if (iOffset < 0)
							iMin++;
						if (iOffset > 0)
							iMax--;

						if (iMin < 0)
							iMin = 0;
						if (iMax > iMaxReg)
							iMax = iMaxReg;

						sprintf(pcReg2,"%d-%d",iMin,iMax);

						// adjusts next current
						if (iOffset < 0)
						{
							if (--iMin < 0)
								iMin = 0;
							iNextCurrent = iMin;
						}
						else
						{
							if (++iMax > iMaxReg)
								iMax = iMaxReg;
							iNextCurrent = iMax;
						}

						// sets the next current
						poCurrent->poNodeGet->RecordSet.Current[iNextCurrent];
					}
					pcReg = pcReg2;
				}
				else
				{
					DUMP_CHLOG_ERROR ( M4_ERR_XML_FILTER_LIST_UPD_INVALID_REG );
					iResult = M4_ERROR;
				}
			}
			else
			{
				DUMP_CHLOG_ERROR ( M4_ERR_XML_FILTER_LIST_INIT );
				iResult = M4_ERROR;
			}
		}

		sNewItem.append("[");
		sNewItem.append(pcReg);
		sNewItem.append("]");
	}

	if (m_sItem.size())
	{
		sNewItem.append(".");
		sNewItem.append(m_sItem);
	}
	
	// updates current if not already set
	if (iResult == M4_SUCCESS && iNextCurrent < 0)
	{
		ClM4XMLItem oItem(sNewItem,M4_TRUE);
		iResult = oItem.Move(*m_poContext,M4_FALSE);
	}

	// checks error status
	if (iResult != M4_SUCCESS)
		return;

	// inserts the element
	StSetItem oSetItem;
	oSetItem.sValue=sNewItem;
	oSetItem.iNextCurrent = iNextCurrent;
	oSetItem.bRecursive = m_bRecursive;
	oSetItem.bEnableDeltas = m_bEnableDeltas;
	m_oItemSet.insert(oSetItem);

  //## end ClM4XMLItemFilterList::Update%938437892.body
}

m4return_t ClM4XMLItemFilterList::UpdateVector ()
{
  //## begin ClM4XMLItemFilterList::UpdateVector%938598811.body preserve=yes

	if (m_poContext == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_FILTER_LIST_INIT );
		return M4_ERROR;
	}

	// processes the set
	M4ItemSetIt it = m_oItemSet.begin(), it2 = it;
	string sAlias, sNode, sRegister, sItem;

	// check items
	if (it == m_oItemSet.end())
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_FILTER_LIST_NO_LIST );
		return M4_ERROR;
	}

	// instances items
	ClM4XMLItem oItem1, oItem2, *poItem1=&oItem1, *poItem2=&oItem2, *poItemTemp=NULL;

	// inits the first item
	if (poItem1->Reset(*m_poContext,(*it).sValue) != M4_SUCCESS)
		return M4_ERROR;
	M4LevelType iLevel = LEVEL_ALIAS;

	m4return_t iResult = M4_SUCCESS;
	for (it++; it != m_oItemSet.end(); it++)
	{
		// inits item
		if (poItem2->Reset(*m_poContext,(*it).sValue) != M4_SUCCESS)
		{
			iResult = M4_ERROR;
			break;
		}

		// checks node level
		if (poItem1->GetM4OAlias() == NULL || poItem2->GetM4OAlias() == NULL ||
			stricmp(poItem1->GetM4OAlias(),poItem2->GetM4OAlias()))
			iLevel = LEVEL_ALIAS;
		else
			if (poItem1->GetNode() == NULL || poItem2->GetNode() == NULL ||
				stricmp(poItem1->GetNode(),poItem2->GetNode()))
				iLevel = LEVEL_NODE;
			else
				if (poItem1->GetRegister() == NULL || poItem2->GetRegister() == NULL ||
					stricmp(poItem1->GetRegister(),poItem2->GetRegister()))
					iLevel = LEVEL_REGISTER;
				else
					if (poItem1->GetItem() == NULL || poItem2->GetItem() == NULL ||
						stricmp(poItem1->GetItem(),poItem2->GetItem()))
						iLevel = LEVEL_ITEM;
					
		// goes to the next element
		poItemTemp = poItem1;
		poItem1 = poItem2;
		poItem2 = poItemTemp;
		M4ItemSetIt it3 = it2;
		it3++;
		StSetItem oNew = *it2;
		oNew.iLevel = iLevel;
		m_oItemSet.erase(it2);
		m_oItemSet.insert(oNew);
		it2 = it3;
	}

	// sets the end value
	StSetItem oNew = *it2;
	oNew.iLevel = LEVEL_ALIAS;
	m_oItemSet.erase(it2);
	m_oItemSet.insert(oNew);

	return iResult;

  //## end ClM4XMLItemFilterList::UpdateVector%938598811.body
}

void ClM4XMLItemFilterList::Reset ()
{
  //## begin ClM4XMLItemFilterList::Reset%938708972.body preserve=yes
	m_oItemSet.clear();
  //## end ClM4XMLItemFilterList::Reset%938708972.body
}

// Additional Declarations
  //## begin ClM4XMLItemFilterList%37E2282403B0.declarations preserve=yes
  //## end ClM4XMLItemFilterList%37E2282403B0.declarations

// Class StSetItem 




//## begin StSetItem::iCounter%396D8A5103C5.attr preserve=no  private: static m4uint32_t {UA} 0
m4uint32_t StSetItem::iCounter = 0;
//## end StSetItem::iCounter%396D8A5103C5.attr



StSetItem::StSetItem ()
  //## begin StSetItem::StSetItem%963478679.hasinit preserve=no
      : iLevel(LEVEL_NODE), iNextCurrent(-1), iLocalCounter(0), bRecursive(M4_FALSE)
  //## end StSetItem::StSetItem%963478679.hasinit
  //## begin StSetItem::StSetItem%963478679.initialization preserve=yes
  //## end StSetItem::StSetItem%963478679.initialization
{
  //## begin StSetItem::StSetItem%963478679.body preserve=yes
	iLocalCounter = iCounter++;
  //## end StSetItem::StSetItem%963478679.body
}



//## Other Operations (implementation)
m4bool_t StSetItem::operator < (const StSetItem &obj) const
{
  //## begin StSetItem::operator <%938598812.body preserve=yes
	// return (sValue < obj.sValue) ? M4_TRUE : M4_FALSE;
	return (iLocalCounter < obj.iLocalCounter) ? M4_TRUE : M4_FALSE;
  //## end StSetItem::operator <%938598812.body
}

// Additional Declarations
  //## begin StSetItem%37F219E60045.declarations preserve=yes
  //## end StSetItem%37F219E60045.declarations

// Class TranslArgs 






TranslArgs::TranslArgs (ClAccess *ai_poAccess, const string& ai_sNode, const string& ai_sItem)
  //## begin TranslArgs::TranslArgs%939289651.hasinit preserve=no
      : m_poAccess(ai_poAccess), m_iHandle(-1), m_sNode(ai_sNode), m_sItem(ai_sItem)
  //## end TranslArgs::TranslArgs%939289651.hasinit
  //## begin TranslArgs::TranslArgs%939289651.initialization preserve=yes
  //## end TranslArgs::TranslArgs%939289651.initialization
{
  //## begin TranslArgs::TranslArgs%939289651.body preserve=yes
	
	// checks parameters
	if (ai_poAccess == NULL)
		return;

	// gets the Node
	ClNode *poNode = ai_poAccess->Node.Get(ai_sNode.c_str());
	if (poNode == NULL)
		return;

	ClAccessRecordSet *poRS = &poNode->RecordSet;

	m_iHandle = poRS->Current.Item[ai_sItem.c_str()].ItemDef.GetHandle();
  //## end TranslArgs::TranslArgs%939289651.body
}



//## Other Operations (implementation)
void TranslArgs::Add (const string& param, const string& value)
{
  //## begin TranslArgs::Add%939289644.body preserve=yes
	if (m_poAccess == NULL || m_iHandle <= 0)
		return;

	// scans the parameter value
	m4int16_t iIndex = m_poAccess->GetpCMCR()->GetItemArgumentPositionById(m_iHandle,param.c_str());
	
	if (iIndex < 0) 
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_TRANSL_ARG_INVALID, param.c_str() );
		Reset();
		return;
	}

	// adds the element to the map
	m_oArgsMap.insert(TranslArgsMap::value_type(iIndex,value));

  //## end TranslArgs::Add%939289644.body
}

m4pcchar_t TranslArgs::GetArg (m4int16_t ai_iIndex)
{
  //## begin TranslArgs::GetArg%939289648.body preserve=yes

	TranslArgsIt it = m_oArgsMap.find(ai_iIndex);
	if (it == m_oArgsMap.end())
		return "";
	
	return (*it).second.c_str();

  //## end TranslArgs::GetArg%939289648.body
}

void TranslArgs::Reset ()
{
  //## begin TranslArgs::Reset%939289652.body preserve=yes
	m_oArgsMap.clear();
	m_iHandle = -1;
	m_poAccess = NULL;
  //## end TranslArgs::Reset%939289652.body
}

// Additional Declarations
  //## begin TranslArgs%37FCBB7D021A.declarations preserve=yes
  //## end TranslArgs%37FCBB7D021A.declarations

// Class TranslItemPK 



TranslItemPK::TranslItemPK ()
  //## begin TranslItemPK::TranslItemPK%945198098.hasinit preserve=no
  //## end TranslItemPK::TranslItemPK%945198098.hasinit
  //## begin TranslItemPK::TranslItemPK%945198098.initialization preserve=yes
  //## end TranslItemPK::TranslItemPK%945198098.initialization
{
  //## begin TranslItemPK::TranslItemPK%945198098.body preserve=yes
  //## end TranslItemPK::TranslItemPK%945198098.body
}

TranslItemPK::TranslItemPK (const string& ai_sM4OAlias, m4int32_t ai_iHandle)
  //## begin TranslItemPK::TranslItemPK%939289655.hasinit preserve=no
  //## end TranslItemPK::TranslItemPK%939289655.hasinit
  //## begin TranslItemPK::TranslItemPK%939289655.initialization preserve=yes
      : m_sM4OAlias(ai_sM4OAlias), m_iHandle(ai_iHandle)
  //## end TranslItemPK::TranslItemPK%939289655.initialization
{
  //## begin TranslItemPK::TranslItemPK%939289655.body preserve=yes
  //## end TranslItemPK::TranslItemPK%939289655.body
}



//## Other Operations (implementation)
m4bool_t TranslItemPK::operator < (const TranslItemPK& o) const
{
  //## begin TranslItemPK::operator <%939289656.body preserve=yes
	if (m_sM4OAlias < o.m_sM4OAlias)
		return M4_TRUE;
	if (m_sM4OAlias > o.m_sM4OAlias)
		return M4_FALSE;
	if (m_iHandle < o.m_iHandle)
		return M4_TRUE;
	return M4_FALSE;

  //## end TranslItemPK::operator <%939289656.body
}

// Additional Declarations
  //## begin TranslItemPK%37FCF8A80127.declarations preserve=yes
  //## end TranslItemPK%37FCF8A80127.declarations

// Class TranslItemData 



TranslItemData::TranslItemData ()
  //## begin TranslItemData::TranslItemData%945198099.hasinit preserve=no
  //## end TranslItemData::TranslItemData%945198099.hasinit
  //## begin TranslItemData::TranslItemData%945198099.initialization preserve=yes
  //## end TranslItemData::TranslItemData%945198099.initialization
{
  //## begin TranslItemData::TranslItemData%945198099.body preserve=yes
  //## end TranslItemData::TranslItemData%945198099.body
}

TranslItemData::TranslItemData (const string &ai_sType)
  //## begin TranslItemData::TranslItemData%939372145.hasinit preserve=no
  //## end TranslItemData::TranslItemData%939372145.hasinit
  //## begin TranslItemData::TranslItemData%939372145.initialization preserve=yes
      : m_sType(ai_sType)
  //## end TranslItemData::TranslItemData%939372145.initialization
{
  //## begin TranslItemData::TranslItemData%939372145.body preserve=yes
  //## end TranslItemData::TranslItemData%939372145.body
}



//## Other Operations (implementation)
void TranslItemData::AddArg (const string &ai_sName, const string &ai_sValue)
{
  //## begin TranslItemData::AddArg%939372146.body preserve=yes
	m_oArgSet.insert(TranslItemDataStr(ai_sName,ai_sValue));
  //## end TranslItemData::AddArg%939372146.body
}

// Additional Declarations
  //## begin TranslItemData%37FDC2710226.declarations preserve=yes
  //## end TranslItemData%37FDC2710226.declarations

// Class TranslItemDataStr 



TranslItemDataStr::TranslItemDataStr ()
  //## begin TranslItemDataStr::TranslItemDataStr%945198097.hasinit preserve=no
  //## end TranslItemDataStr::TranslItemDataStr%945198097.hasinit
  //## begin TranslItemDataStr::TranslItemDataStr%945198097.initialization preserve=yes
  //## end TranslItemDataStr::TranslItemDataStr%945198097.initialization
{
  //## begin TranslItemDataStr::TranslItemDataStr%945198097.body preserve=yes
  //## end TranslItemDataStr::TranslItemDataStr%945198097.body
}

TranslItemDataStr::TranslItemDataStr (const string &ai_sName, const string &ai_sValue)
  //## begin TranslItemDataStr::TranslItemDataStr%939630172.hasinit preserve=no
  //## end TranslItemDataStr::TranslItemDataStr%939630172.hasinit
  //## begin TranslItemDataStr::TranslItemDataStr%939630172.initialization preserve=yes
      : m_sName(ai_sName), m_sValue(ai_sValue)
  //## end TranslItemDataStr::TranslItemDataStr%939630172.initialization
{
  //## begin TranslItemDataStr::TranslItemDataStr%939630172.body preserve=yes
  //## end TranslItemDataStr::TranslItemDataStr%939630172.body
}



//## Other Operations (implementation)
m4bool_t TranslItemDataStr::operator < (const TranslItemDataStr& o) const
{
  //## begin TranslItemDataStr::operator <%939630173.body preserve=yes
	if (m_sName < o.m_sName)
		return M4_TRUE;
	if (m_sName > o.m_sName)
		return M4_FALSE;
	return (m_sValue < o.m_sValue) ? M4_TRUE : M4_FALSE;
  //## end TranslItemDataStr::operator <%939630173.body
}

// Additional Declarations
  //## begin TranslItemDataStr%37FE14900149.declarations preserve=yes
  //## end TranslItemDataStr%37FE14900149.declarations

// Class ClM4XMLStream 




//## begin ClM4XMLStream::m_oBase64%3816C62702AA.attr preserve=no  private: static C_Base64 {UA} 
C_Base64 ClM4XMLStream::m_oBase64;
//## end ClM4XMLStream::m_oBase64%3816C62702AA.attr



ClM4XMLStream::ClM4XMLStream ()
  //## begin ClM4XMLStream::ClM4XMLStream%938775329.hasinit preserve=no
      : m_iLen(0), m_iLenCurrent(0), DEFAULT_BLOCK(4096), m_poData(NULL), m_poDataUnit(NULL)
  //## end ClM4XMLStream::ClM4XMLStream%938775329.hasinit
  //## begin ClM4XMLStream::ClM4XMLStream%938775329.initialization preserve=yes
  //## end ClM4XMLStream::ClM4XMLStream%938775329.initialization
{
  //## begin ClM4XMLStream::ClM4XMLStream%938775329.body preserve=yes
  //## end ClM4XMLStream::ClM4XMLStream%938775329.body
}


ClM4XMLStream::~ClM4XMLStream ()
{
  //## begin ClM4XMLStream::~ClM4XMLStream%939030963.body preserve=yes
	Close();
  //## end ClM4XMLStream::~ClM4XMLStream%939030963.body
}



//## Other Operations (implementation)
void ClM4XMLStream::Clear ()
{
  //## begin ClM4XMLStream::Clear%938775327.body preserve=yes
	if (m_poData != NULL)
		m_poData->Clear();
  //## end ClM4XMLStream::Clear%938775327.body
}

ClM4XMLStream & ClM4XMLStream::operator << (m4pcchar_t ai_pcString)
{
  //## begin ClM4XMLStream::operator <<%938775328.body preserve=yes
	Write(ai_pcString,strlen(ai_pcString)); 
	return *this;
  //## end ClM4XMLStream::operator <<%938775328.body
}

ClM4XMLStream & ClM4XMLStream::operator << (const XMLCh *ai_pcString)
{
  //## begin ClM4XMLStream::operator <<%938775331.body preserve=yes
	if (ai_pcString == NULL)
		return *this;

	m4char_t buffer [2] = "X";
	while (*ai_pcString)
	{
		*buffer = (char)*(ai_pcString++);
		*this << buffer;
	}
	return *this;
  //## end ClM4XMLStream::operator <<%938775331.body
}

ClM4XMLStream & ClM4XMLStream::operator << (m4int32_t ai_iArg)
{
  //## begin ClM4XMLStream::operator <<%938775333.body preserve=yes
	m4char_t pcBuffer[32];
	sprintf(pcBuffer,"%d",ai_iArg);
	*this << pcBuffer;
	return *this;
  //## end ClM4XMLStream::operator <<%938775333.body
}

void ClM4XMLStream::Close ()
{
  //## begin ClM4XMLStream::Close%939030962.body preserve=yes
	if (m_poDataUnit != NULL)
		m_poDataUnit->SetSize(m_iLenCurrent);
  //## end ClM4XMLStream::Close%939030962.body
}

ClM4XMLStream & ClM4XMLStream::Transl (m4pcchar_t ai_pcString)
{
  //## begin ClM4XMLStream::Transl%940247663.body preserve=yes

	// checks voids
	if( ai_pcString == NULL || *ai_pcString == '\0' )
	{
		return *this;
	}


	// UNICODE XML
	m4char_t	c = '\0' ;
	size_t		i = 0 ;
	size_t		iLength = 0 ;
	m4pcchar_t	pccScaped = NULL ;


	iLength = strlen( ai_pcString ) ;

	for( i = 0 ; i < iLength; i++ )
	{
		c = ai_pcString[ i ] ;

		pccScaped = M4XMLScapeChar( c ) ;

		if( pccScaped != NULL )
		{
			Write( pccScaped ) ;
		}
		else
		{
			// bugid 0136155. Si no es un carácter de los escapados por M4XMLScapeChar
			// (&, <, >, ", ')  comprueba si es de los a escapar (newline) y se escapa. 

			pccScaped = LocalM4XMLEscapeChar( c ) ;
			
			if( pccScaped != NULL )
			{
				Write( pccScaped ) ;
			}
			else
			{
				Write( &c, 1 ) ;
			}
		}

	}

	return *this;

  //## end ClM4XMLStream::Transl%940247663.body
}

const char* const ClM4XMLStream::LocalM4XMLEscapeChar( const char ai_ccChar )
{
	//## begin ClM4XMLStream::LocalM4XMLEscapeChar%940247667.body preserve=yes

	char* pcResult = NULL ;
	
	m4uint8_t iChar = ai_ccChar; 

	switch( iChar )
	{
		case 13:
			pcResult = "&#13;" ;
			break;

		case 10:
			pcResult = "&#10;" ;
			break;

		default:
			pcResult = NULL ;
			break;
	}

	return( pcResult ) ;

	//## begin ClM4XMLStream::LocalM4XMLEscapeChar%940247667.body preserve=yes
}



m4return_t ClM4XMLStream::Write (m4pcchar_t ai_pcString, m4int32_t ai_iSize)
{
  //## begin ClM4XMLStream::Write%940247664.body preserve=yes
	// checks the current buffer
	if (m_poData == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_STREAM_INIT );
		return M4_ERROR;
	}

	// checks the buffer size
	if (m_poDataUnit == NULL)
	{
		m_poData->GetNewDataUnit(m_poDataUnit);
		m_poDataUnit->SetSize(DEFAULT_BLOCK);
		m_iLenCurrent = 0;
	}

	// checks buffer length
	m4int32_t iLen = ((ai_iSize < 0) ? strlen(ai_pcString) : ai_iSize), iOff=0;
	m_iLen += iLen;

	while (iLen)
	{
		m4int32_t iNWrite = 0;

		if (m_iLenCurrent + iLen > DEFAULT_BLOCK)
			iNWrite = DEFAULT_BLOCK - m_iLenCurrent;
		else
			iNWrite = iLen;
		
		// writes the current
		strncpy(m_poDataUnit->GetBuffer()+m_iLenCurrent,ai_pcString+iOff,iNWrite);

		// updates
		iLen -= iNWrite;
		iOff += iNWrite;
		m_iLenCurrent += iNWrite;

		// checks for a new block
		if (iLen && m_iLenCurrent == DEFAULT_BLOCK)
		{
			m_poData->GetNewDataUnit(m_poDataUnit);
			m_poDataUnit->SetSize(DEFAULT_BLOCK);
			m_iLenCurrent = 0;
		}
	}

	// returns
	return M4_SUCCESS;
  //## end ClM4XMLStream::Write%940247664.body
}

m4bool_t ClM4XMLStream::IsBin (const m4uint8_t* ai_pcString, m4uint32_t ai_ulLen)
{
  //## begin ClM4XMLStream::IsBin%941014275.body preserve=yes
	const m4uint8_t *p = ai_pcString;
	for (m4uint32_t i=0; i<ai_ulLen; i++, p++)
	{
		if (*p >= 32 || *p == 9 || *p == 10 || *p == 13)
			continue;

		return M4_TRUE;
	}
	return M4_FALSE;
  //## end ClM4XMLStream::IsBin%941014275.body
}

m4return_t ClM4XMLStream::Transl64 (m4pcchar_t ai_pcString, m4uint32_t ai_iLen)
{
  //## begin ClM4XMLStream::Transl64%941014277.body preserve=yes
	// translates buffer
	m4pchar_t pcBuffer = NULL;
	if (! m_oBase64.Code((m4pchar_t)ai_pcString,ai_iLen,pcBuffer))
	{
		if (pcBuffer != NULL)
			m_oBase64.FreeBuffer(pcBuffer);
		DUMP_CHLOG_ERROR ( M4_ERR_XML_STREAM_TRANSL64 );
		return M4_ERROR;
	}

	Write(pcBuffer,strlen(pcBuffer));

	m_oBase64.FreeBuffer(pcBuffer);

	return M4_SUCCESS;

  //## end ClM4XMLStream::Transl64%941014277.body
}

m4return_t ClM4XMLStream::Decode64 (m4pcchar_t ai_pcString, string &ao_sOutput)
{
  //## begin ClM4XMLStream::Decode64%941014278.body preserve=yes
	m4pchar_t pcBuffer = NULL;
	m4uint32_t iLen = 0;

	if (! m_oBase64.Decode((m4pchar_t)ai_pcString,pcBuffer,(m4int32_t*)&iLen))
	{
		if (pcBuffer != NULL)
			m_oBase64.FreeBuffer(pcBuffer);
		DUMP_CHLOG_ERROR ( M4_ERR_XML_STREAM_DECODE64 );
		return M4_ERROR;
	}
	
	ao_sOutput = pcBuffer;

	m_oBase64.FreeBuffer(pcBuffer);

	return M4_SUCCESS;
  //## end ClM4XMLStream::Decode64%941014278.body
}

// Additional Declarations
  //## begin ClM4XMLStream%37F4869C024F.declarations preserve=yes
  //## end ClM4XMLStream%37F4869C024F.declarations

// Class M4XMLString 


//## Other Operations (implementation)
m4bool_t M4XMLString::ICaseComp (const string& ai_sToken, m4pcchar_t ai_sString, m4int16_t ai_iMin)
{
  //## begin M4XMLString::ICaseComp%944049067.body preserve=yes
	if (ai_iMin < 0)
		return stricmp(ai_sToken.c_str(),ai_sString) ? M4_FALSE : M4_TRUE;

	return strnicmp(ai_sToken.c_str(),ai_sString,ai_iMin) ? M4_FALSE : M4_TRUE;

  //## end M4XMLString::ICaseComp%944049067.body
}

// Additional Declarations
  //## begin M4XMLString%38450A1A007A.declarations preserve=yes
  //## end M4XMLString%38450A1A007A.declarations

// Class ClM4XMLAliasDef 











ClM4XMLAliasDef::ClM4XMLAliasDef ()
  //## begin ClM4XMLAliasDef::ClM4XMLAliasDef%944575103.hasinit preserve=no
  //## end ClM4XMLAliasDef::ClM4XMLAliasDef%944575103.hasinit
  //## begin ClM4XMLAliasDef::ClM4XMLAliasDef%944575103.initialization preserve=yes
  //## end ClM4XMLAliasDef::ClM4XMLAliasDef%944575103.initialization
{
  //## begin ClM4XMLAliasDef::ClM4XMLAliasDef%944575103.body preserve=yes
	// adds default parameters for channels

	// general
	{
		ClM4AliasParam p(M4CMPROP_CHANNEL_WANTS_AUTOLOAD,"",M4CL_CPP_TYPE_NUMBER);
		AddDefParam(p);
	}
	{
		ClM4AliasParam p(M4CMPROP_CHANNEL_UNIT_DATE,"",M4CL_CPP_TYPE_NUMBER);
		AddDefParam(p);
	}

	// metachannel date
	{
		ClM4AliasParam p(M4CMPROP_CHANNEL_START_DATE,"",M4CL_CPP_TYPE_DATE);
		AddDefParam(p);
	}
	{
		ClM4AliasParam p(M4CMPROP_CHANNEL_END_DATE,"",M4CL_CPP_TYPE_DATE);
		AddDefParam(p);
	}
	{
		ClM4AliasParam p(M4CMPROP_CHANNEL_CORRECTION_DATE,"",M4CL_CPP_TYPE_DATE);
		AddDefParam(p);
	}

	// channel date
	{
		ClM4AliasParam p(M4CMPROP_CHANNEL_DATA_START_DATE,"",M4CL_CPP_TYPE_DATE);
		AddDefParam(p);
	}
	{
		ClM4AliasParam p(M4CMPROP_CHANNEL_DATA_END_DATE,"",M4CL_CPP_TYPE_DATE);
		AddDefParam(p);
	}
	{
		ClM4AliasParam p(M4CMPROP_CHANNEL_DATA_CORRECTION_DATE,"",M4CL_CPP_TYPE_DATE);
		AddDefParam(p);
	}

	// execution date
	{
		ClM4AliasParam p(M4CMPROP_CHANNEL_EXECUTION_START_DATE,"",M4CL_CPP_TYPE_DATE);
		AddDefParam(p);
	}
	{
		ClM4AliasParam p(M4CMPROP_CHANNEL_EXECUTION_END_DATE,"",M4CL_CPP_TYPE_DATE);
		AddDefParam(p);
	}
	{
		ClM4AliasParam p(M4CMPROP_CHANNEL_EXECUTION_DATE,"",M4CL_CPP_TYPE_DATE);
		AddDefParam(p);
	}


	// CS
	{
		ClM4AliasParam p(M4CMPROP_CHANNEL_CS_TYPE,"",M4CL_CPP_TYPE_NUMBER);
		AddDefParam(p);
	}

	// security
	{
		ClM4AliasParam p(M4CMPROP_CHANNEL_ID_RSM,"",M4CL_CPP_TYPE_STRING);
		AddDefParam(p);
	}
	{
		ClM4AliasParam p(M4CMPROP_CHANNEL_ID_APP_ROLE,"",M4CL_CPP_TYPE_STRING);
		AddDefParam(p);
	}
	{
		ClM4AliasParam p(M4CMPROP_CHANNEL_ID_ORGANIZATION,"",M4CL_CPP_TYPE_STRING);
		AddDefParam(p);
	}
	{
		ClM4AliasParam p(M4CMPROP_CHANNEL_ID_ORGANIZATION_TYPE,"",M4CL_CPP_TYPE_NUMBER);
		AddDefParam(p);
	}
    
  //## end ClM4XMLAliasDef::ClM4XMLAliasDef%944575103.body
}



//## Other Operations (implementation)
void ClM4XMLAliasDef::Reset ()
{
  //## begin ClM4XMLAliasDef::Reset%944575101.body preserve=yes
	oParams.clear();
	sName = "";
	sM4OId = "";
	sMode = "";
	sPreserve = "";
	sRemove = "";
	sFind = "";
	sMDInfoLevel = "";
  //## end ClM4XMLAliasDef::Reset%944575101.body
}

void ClM4XMLAliasDef::ResetM4OReferences()
{
	//## begin ClM4XMLAliasDef::Reset%944575101.body preserve=yes
	oReferences.clear();
	//## end ClM4XMLAliasDef::Reset%944575101.body
}

ClChannel* ClM4XMLAliasDef::Generate (ClChannelManager* ai_poCM)
{
  //## begin ClM4XMLAliasDef::Generate%944575102.body preserve=yes

	// adds the channel
	ClChannel *poChannel = NULL;

	// checks the channel type
	m4int16_t iMode = -1;
	if (XMLICMP(sMode,"back",-1))
		iMode = M4CL_CSTYPE_BACK;
	else
		iMode = M4CL_CSTYPE_BOTH;

	// checks the find mode
	m4return_t iResult = M4_SUCCESS;
	m4bool_t bFind = M4_FALSE;
	if (XMLICMP(sFind,"yes",1))
	{
		bFind = M4_TRUE;
		poChannel = FindChannel(ai_poCM,sM4OId,iMode);
		if (poChannel == NULL)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_CONTEXT_FIND_CHANNEL, sM4OId.c_str() << iMode );
			return NULL;
		}
	}
	else
	{
		
		iResult = ai_poCM->CreateChannel(poChannel);
		if (iResult != M4_SUCCESS)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_CONTEXT_CREATE_CHANNEL, sM4OId.c_str() );
			return NULL;
		}

		// updates the properties
		iResult = ApplyParams(poChannel);
		if (iResult != M4_SUCCESS)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_CONTEXT_CREATE_CHANNEL, sM4OId.c_str() );
			return NULL;
		}

		// builds the channel
		poChannel->CsType.Set(m4VariantType(iMode));
		iResult = poChannel->BuildFromId(sM4OId.c_str());
		if (iResult != M4_SUCCESS)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_CONTEXT_BUILD_CHANNEL, sM4OId.c_str() );

			if (poChannel->Destroy() != M4_SUCCESS)
			{
				DUMP_CHLOG_ERRORF ( M4_ERR_XML_CONTEXT_BUILD_DESTROY, sM4OId.c_str() );
			}
			return NULL;
		}
	}
		
	return poChannel;
  //## end ClM4XMLAliasDef::Generate%944575102.body
}

m4return_t ClM4XMLAliasDef::AddParam (ClM4AliasParam ai_oAliasParam)
{
  //## begin ClM4XMLAliasDef::AddParam%944575106.body preserve=yes

	// scans the default type
	m4bool_t bFound = M4_FALSE;
	ClM4AliasParam oDefParam;
	m4return_t iResult = FindDef(ai_oAliasParam,oDefParam,bFound);
	if (iResult != M4_SUCCESS)
		return M4_ERROR;

	// converts the type
	if (bFound == M4_TRUE)
		ai_oAliasParam.iCPPType = oDefParam.iCPPType;

	oParams.insert(ai_oAliasParam);

	return M4_SUCCESS;
  //## end ClM4XMLAliasDef::AddParam%944575106.body
}

m4return_t ClM4XMLAliasDef::AddRef (ClM4AliasRef ai_oAliasRef)
{
  //## begin ClM4XMLAliasDef::AddRef%944575106.body preserve=yes

	oReferences.insert(ai_oAliasRef);

	return M4_SUCCESS;
  //## end ClM4XMLAliasDef::AddRef%944575106.body
}

ClM4AliasReferenceSet& ClM4XMLAliasDef::GetReferences ()
{
  //## begin ClM4XMLAliasDef::GetReferences%944575106.body preserve=yes

	return oReferences;
  //## end ClM4XMLAliasDef::GetReferences%944575106.body
}

ClChannel* ClM4XMLAliasDef::FindChannel (ClChannelManager* ai_poCM, const string& ai_sId, m4int16_t ai_iCSMode, ClChannel* ai_poBaseChannel)
{
  //## begin ClM4XMLAliasDef::FindChannel%944575107.body preserve=yes

	// gets the id
	if (!ai_sId.size())
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_FIND_CHANNEL_TRUNC );
		return NULL;
	}

	// gets the sub level
	string sId;
	m4pcchar_t pcIdBase = ai_sId.c_str();
	m4pcchar_t pcIdEnd = strchr(pcIdBase,'.');
	if (pcIdEnd == NULL)
		sId = ai_sId;
	else
	{
		// skips the dot
		++pcIdEnd;

		m4int32_t iLen = pcIdEnd - pcIdBase - 1;
		m4pchar_t pcId = new m4char_t[iLen+1];

		memcpy(pcId,pcIdBase,iLen);
		pcId[iLen] = '\0';
		sId = pcId;
		delete pcId;
	}

	// calculates the final id, taking channel mappings into account
	m4pchar_t pcOrgId = ai_poCM->GetLogonAdaptor()->GetOrganization();

	if( pcOrgId != NULL )
	{
		m4pchar_t pcMappingId = ai_poCM->GetpFactory()->MDFac_GetMappingById( M4OBJECT_MAPPINGS, pcOrgId, sId.c_str(), M4_TRUE ) ;

		if( pcMappingId != NULL )
		{
			// replaces the search id
			sId = pcMappingId;
			delete [] pcMappingId ;
		}
	}

	// gets the channel manager
	ClChannel *poChannel = NULL;
	if (ai_poBaseChannel == NULL)
	{
		// scans the channel manager
		ClChannelDirectory *poClChannelDirectory = &ai_poCM->Channel;
		m4int32_t iNChannels = poClChannelDirectory->Count();
		m4int32_t iCount;
		for (iCount=0;iCount<iNChannels;iCount++)
		{
			ClChannel *poNewChannel = (*poClChannelDirectory)[iCount];
			
			if (poNewChannel->IsBuilt() != M4_TRUE)
				continue;

			if (poNewChannel->GetpChannelDef()->Id() == sId)
			{
				if (poChannel == NULL)
				{
					poChannel = poNewChannel;
				}
				else
				{
					DUMP_CHLOG_ERRORF ( M4_ERR_XML_FIND_MULTI, sId.c_str() );
					return NULL;
				}
			}
		}

		if (poChannel == NULL)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_FIND_NOT_FOUND, sId.c_str() );
		}
	}
	else
	{
		// scans the level 2
		ClAccess *poAccess = NULL;
		if (ai_poBaseChannel->Level2.GetL2Access(sId.c_str(),&poAccess)
			!= M4_SUCCESS)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_FIND_L2_NOT_FOUND, sId.c_str() );
			return NULL;
		}

		// gets the channel
		if (poAccess != NULL) 
		{
			poChannel = poAccess->GetpChannel();
		}
		
		if (poChannel == NULL)
		{
			DUMP_CHLOG_ERRORF ( M4_ERR_XML_FIND_L2_ERROR, sId.c_str() );
		}

	}

	// checks sublevel
	if (poChannel != NULL && pcIdEnd != NULL)
		return FindChannel(ai_poCM,pcIdEnd,ai_iCSMode,poChannel);

	// returns
	return poChannel;
  //## end ClM4XMLAliasDef::FindChannel%944575107.body
}

m4return_t ClM4XMLAliasDef::AddDefParam (ClM4AliasParam& ai_oAliasParam)
{
  //## begin ClM4XMLAliasDef::AddDefParam%944739019.body preserve=yes
	oDefParams.insert(ai_oAliasParam);
	return M4_SUCCESS;
  //## end ClM4XMLAliasDef::AddDefParam%944739019.body
}

m4return_t ClM4XMLAliasDef::FindDef (ClM4AliasParam& ao_iKey, ClM4AliasParam& ao_iResult, m4bool_t& ao_bFound)
{
  //## begin ClM4XMLAliasDef::FindDef%944739020.body preserve=yes
	ClM4AliasParamIt it = oParams.find(ao_iKey);
	ao_bFound = M4_FALSE;
	if (it == oParams.end())
		return M4_SUCCESS;

	ao_iResult = *it;
	return M4_SUCCESS;
  //## end ClM4XMLAliasDef::FindDef%944739020.body
}

m4return_t ClM4XMLAliasDef::ApplyParams (ClChannel* ai_poChannel)
{
  //## begin ClM4XMLAliasDef::ApplyParams%944742373.body preserve=yes
	ClM4AliasParamIt it,it2;
	ClProperty *oProperty = NULL;
	
	m4return_t iResult = M4_SUCCESS;
	for (it = oParams.begin(); it != oParams.end(); it++)
	{
		it2 = oDefParams.find(*it);
		if (it2 != oDefParams.end())
			iResult = ApplyParam(ai_poChannel,*it,&(*it2));
		else
			iResult = ApplyParam(ai_poChannel,*it);

		if (iResult != M4_SUCCESS)
			return M4_ERROR;
	}

	return M4_SUCCESS;
  //## end ClM4XMLAliasDef::ApplyParams%944742373.body
}

m4return_t ClM4XMLAliasDef::ApplyParam (ClChannel* ai_poChannel, const ClM4AliasParam& ai_Param, const ClM4AliasParam* ai_ParamRef)
{
  //## begin ClM4XMLAliasDef::ApplyParam%944742374.body preserve=yes

	if (ai_poChannel == NULL)
		return M4_ERROR;
	
	// converts the type
	m4VariantType vValue((m4pchar_t)ai_Param.sValue.c_str());
	m4return_t iResult = M4_SUCCESS;
	m4int16_t iCPPType = -1;

	if (ai_ParamRef != NULL)
		iCPPType = ai_ParamRef->iCPPType;
	else
		iCPPType = ai_Param.iCPPType;

	if (iCPPType < 0)
		iCPPType = M4CL_CPP_TYPE_STRING;

	ClVariantConversor varConv;
	switch(iCPPType)
	{
	case M4CL_CPP_TYPE_NULL:
		iResult = varConv.ToNull(vValue);
		break;
	case M4CL_CPP_TYPE_NUMBER:
		iResult = varConv.ToDouble(vValue);
		break;
	case M4CL_CPP_TYPE_STRING:
		break;
	case M4CL_CPP_TYPE_DATE:
		iResult = varConv.ToDate(vValue);
		break;
	default:
		DUMP_CHLOG_ERROR ( M4_ERR_XML_INTERNAL_CPPTYPE );
		iResult = M4_ERROR;
	}

	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_CPP_INVALID_CONV, ai_Param.sValue.c_str() << iCPPType << ai_Param.sName.c_str() << ai_poChannel->GetpChannelDef()->Id() );
		return iResult;
	}

	m4uint8_t iStatus = M4_PROP_FULL_STATE;
	ClProperty *poProp = ai_poChannel->Property.GetProperty((const m4pchar_t)ai_Param.sName.c_str());

	// by default the value came from user
	vValue.IsUser = M4_TRUE;
	if (poProp != NULL)
	{
		iStatus = poProp->GetStatus();

		m4VariantType	vOldValue;

		if (poProp->Get(vOldValue) == M4_SUCCESS && M4VariantIsEqual(&vOldValue, &vValue) == 1)
		{
			vValue.IsUser = vOldValue.IsUser;
		}
	}

	iResult = ai_poChannel->Property.SetOrNewProperty((const m4pchar_t)ai_Param.sName.c_str(),vValue,iStatus);
	if(iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_CPP_INVALID_SET_PROP, ai_Param.sValue.c_str() << iCPPType << ai_Param.sName.c_str() << ai_poChannel->GetpChannelDef()->Id());
		return iResult;
	}

	return M4_SUCCESS;
  //## end ClM4XMLAliasDef::ApplyParam%944742374.body
}

// Additional Declarations
  //## begin ClM4XMLAliasDef%384D154D02DD.declarations preserve=yes
  //## end ClM4XMLAliasDef%384D154D02DD.declarations

// Class ClM4AliasParam 




ClM4AliasParam::ClM4AliasParam (const string& ai_sName, const string& ai_sValue, m4int16_t ai_iType)
  //## begin ClM4AliasParam::ClM4AliasParam%944575104.hasinit preserve=no
      : iCPPType(-1)
  //## end ClM4AliasParam::ClM4AliasParam%944575104.hasinit
  //## begin ClM4AliasParam::ClM4AliasParam%944575104.initialization preserve=yes
  //## end ClM4AliasParam::ClM4AliasParam%944575104.initialization
{
  //## begin ClM4AliasParam::ClM4AliasParam%944575104.body preserve=yes
	sName = ai_sName;
	sValue = ai_sValue;
	iCPPType = ai_iType;
  //## end ClM4AliasParam::ClM4AliasParam%944575104.body
}

ClM4AliasParam::ClM4AliasParam ()
  //## begin ClM4AliasParam::ClM4AliasParam%944739021.hasinit preserve=no
      : iCPPType(-1)
  //## end ClM4AliasParam::ClM4AliasParam%944739021.hasinit
  //## begin ClM4AliasParam::ClM4AliasParam%944739021.initialization preserve=yes
  //## end ClM4AliasParam::ClM4AliasParam%944739021.initialization
{
  //## begin ClM4AliasParam::ClM4AliasParam%944739021.body preserve=yes
  //## end ClM4AliasParam::ClM4AliasParam%944739021.body
}



//## Other Operations (implementation)
m4bool_t ClM4AliasParam::operator < (const ClM4AliasParam& v) const
{
  //## begin ClM4AliasParam::operator <%944575105.body preserve=yes
	return (stricmp(sName.c_str(),v.sName.c_str()) < 0) ? M4_TRUE : M4_FALSE;
  //## end ClM4AliasParam::operator <%944575105.body
}

// Additional Declarations
  //## begin ClM4AliasParam%384D1DD0018C.declarations preserve=yes
  //## end ClM4AliasParam%384D1DD0018C.declarations

// Additional Declarations
  //## begin ClM4XMLAliasDef%384D154D02DD.declarations preserve=yes
  //## end ClM4XMLAliasDef%384D154D02DD.declarations

// Class ClM4AliasRef 

ClM4AliasRef::ClM4AliasRef (const string &ai_sM4OAliasOrigin, const string& ai_sM4OAlias, const string& ai_sL2Alias)
  //## begin ClM4AliasRef::ClM4AliasRef%944575104.hasinit preserve=no
  //## end ClM4AliasRef::ClM4AliasRef%944575104.hasinit
  //## begin ClM4AliasRef::ClM4AliasRef%944575104.initialization preserve=yes
  //## end ClM4AliasRef::ClM4AliasRef%944575104.initialization
{
  //## begin ClM4AliasRef::ClM4AliasRef%944575104.body preserve=yes
	sM4OAliasOrigin = ai_sM4OAliasOrigin;
	sM4OAlias = ai_sM4OAlias;
	sL2Alias = ai_sL2Alias;
  //## end ClM4AliasRef::ClM4AliasRef%944575104.body
}

//## Other Operations (implementation)
m4bool_t ClM4AliasRef::operator < (const ClM4AliasRef& v) const
{
  //## begin ClM4AliasRef::operator <%944575105.body preserve=yes
	return (stricmp(sM4OAlias.c_str(),v.sM4OAlias.c_str()) < 0) ? M4_TRUE : M4_FALSE;
  //## end ClM4AliasRef::operator <%944575105.body
}

// Additional Declarations
  //## begin ClM4AliasRef%384D1DD0018C.declarations preserve=yes
  //## end ClM4AliasRef%384D1DD0018C.declarations


// Class ClM4XMLDumpedItems 


//## Other Operations (implementation)
void ClM4XMLDumpedItems::Reset ()
{
  //## begin ClM4XMLDumpedItems::Reset%944822917.body preserve=yes
	m_oItemSet.clear();
  //## end ClM4XMLDumpedItems::Reset%944822917.body
}

void ClM4XMLDumpedItems::AddItem (const string& ai_sItem)
{
  //## begin ClM4XMLDumpedItems::AddItem%944822918.body preserve=yes
	m_oItemSet.insert(DumpedItem(ai_sItem));
  //## end ClM4XMLDumpedItems::AddItem%944822918.body
}

m4bool_t ClM4XMLDumpedItems::CheckItem (const string& ai_sItem)
{
  //## begin ClM4XMLDumpedItems::CheckItem%944822919.body preserve=yes
	DumpedItemIt it = m_oItemSet.find(DumpedItem(ai_sItem));
	if (it == m_oItemSet.end())
		return M4_FALSE;
	return M4_TRUE;
  //## end ClM4XMLDumpedItems::CheckItem%944822919.body
}

DumpedItemSet& ClM4XMLDumpedItems::GetSet ()
{
  //## begin ClM4XMLDumpedItems::GetSet%1071498573.body preserve=yes
	return m_oItemSet;
  //## end ClM4XMLDumpedItems::GetSet%1071498573.body
}

// Additional Declarations
  //## begin ClM4XMLDumpedItems%3850D74B0331.declarations preserve=yes
  //## end ClM4XMLDumpedItems%3850D74B0331.declarations

// Class DumpedItem 


DumpedItem::DumpedItem ()
  //## begin DumpedItem::DumpedItem%945198100.hasinit preserve=no
  //## end DumpedItem::DumpedItem%945198100.hasinit
  //## begin DumpedItem::DumpedItem%945198100.initialization preserve=yes
  //## end DumpedItem::DumpedItem%945198100.initialization
{
  //## begin DumpedItem::DumpedItem%945198100.body preserve=yes
  //## end DumpedItem::DumpedItem%945198100.body
}

DumpedItem::DumpedItem (const string& ai_sItem)
  //## begin DumpedItem::DumpedItem%944822915.hasinit preserve=no
  //## end DumpedItem::DumpedItem%944822915.hasinit
  //## begin DumpedItem::DumpedItem%944822915.initialization preserve=yes
  //## end DumpedItem::DumpedItem%944822915.initialization
{
  //## begin DumpedItem::DumpedItem%944822915.body preserve=yes
	m_sItem = ai_sItem;
  //## end DumpedItem::DumpedItem%944822915.body
}



//## Other Operations (implementation)
m4bool_t DumpedItem::operator < (const DumpedItem& ai_oItem) const
{
  //## begin DumpedItem::operator <%944822916.body preserve=yes
	if (stricmp(m_sItem.c_str(),ai_oItem.m_sItem.c_str()) < 0)
		return M4_TRUE;
	return M4_FALSE;
  //## end DumpedItem::operator <%944822916.body
}

// Additional Declarations
  //## begin DumpedItem%3850D89D0233.declarations preserve=yes
  //## end DumpedItem%3850D89D0233.declarations

//## begin module%37DF53F801B7.epilog preserve=yes
//## end module%37DF53F801B7.epilog

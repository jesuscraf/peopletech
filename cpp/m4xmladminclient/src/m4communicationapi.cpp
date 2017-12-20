//## begin module%39323E160167.cm preserve=no
//## end module%39323E160167.cm

//## begin module%39323E160167.cp preserve=no
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
//## end module%39323E160167.cp

//## Module: M4CommunicationAPI%39323E160167; Package body
//## Subsystem: M4XMLAdminClient::src%39084D03030B
//## Source file: E:\INTEGRATION\m4xmladminclient\src\m4communicationapi.cpp

//## begin module%39323E160167.additionalIncludes preserve=no
//## end module%39323E160167.additionalIncludes

//## begin module%39323E160167.includes preserve=yes

#include <commactionsname.h>
//#include <sax/AttributeList.hpp>
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include "exportingcfunctions.hpp"

//## end module%39323E160167.includes

// M4XmlOutStream
#include <m4xmloutstream.hpp>
// m4condition
#include <m4condition.hpp>
// M4CommunicationAPI
#include <m4communicationapi.hpp>
// M4SAXAdmClient
#include <m4saxadmclient.hpp>
// iexecutable
#include <iexecutable.hpp>
// ParamList
#include <paramlist.hpp>
// m4communication
#include <m4communication.hpp>
// clarraylibres
#include <clarraylibres.hpp>
//## begin module%39323E160167.declarations preserve=no
//## end module%39323E160167.declarations

//## begin module%39323E160167.additionalDeclarations preserve=yes
#include <m4commondeclar.hpp>
//#include <commactionsname.h>

#define M4_COMM_DEFAULT_BUFFER_SIZE 10000

ClCommunication *s_oTheComms;

//ClCommunication s_oTheComms(6666);


static m4string_t s_strDispatcherHost;
static int s_iDispatcherPort = 0;
static m4string_t s_strUser;
static m4string_t s_strPassword;

// Define the commands literals

// Ask for the Server configurations controlled by the Dispatcher
const m4string_t M4ConfigurationsCommand = "configurations";

// Server configurations controlled by the Dispatcher
const m4string_t M4ConfigurationsCommandAnswer = "configurationsanswer";

// Start a Server configuration from the Dispatcher
const m4string_t M4StartServerCommand = "startserver";

// Stop a Server configuration fron the Dispatcher
const m4string_t M4StopServerCommand = "stopserver";

// Stop the Dispatcher
const m4string_t M4StopDispatcherCommand = "stopdpch";

// Remove a Server configuration fron the Dispatcher
const m4string_t M4RemoveServerCommand = "removeserver";

// Authentication answer from the Dispatcher
const m4string_t M4AuthAnswerCommand = "authanswer";

//## end module%39323E160167.additionalDeclarations


// Class ClCommunicationAPI 











ClCommunicationAPI::ClCommunicationAPI ()
  //## begin ClCommunicationAPI::ClCommunicationAPI%959594566.hasinit preserve=no
      : m_pcBuffer(NULL), m_bReadyRead(M4_FALSE), m_bReadyWrite(M4_FALSE), m_iCommStatus(M4_ERROR), m_pReadParams(NULL), m_pWriteParams(NULL)
  //## end ClCommunicationAPI::ClCommunicationAPI%959594566.hasinit
  //## begin ClCommunicationAPI::ClCommunicationAPI%959594566.initialization preserve=yes
  //## end ClCommunicationAPI::ClCommunicationAPI%959594566.initialization
{
  //## begin ClCommunicationAPI::ClCommunicationAPI%959594566.body preserve=yes

	m_WriteCondition.Init();
	m_ReadCondition.Init();


  //## end ClCommunicationAPI::ClCommunicationAPI%959594566.body
}


ClCommunicationAPI::~ClCommunicationAPI ()
{
  //## begin ClCommunicationAPI::~ClCommunicationAPI%959594567.body preserve=yes
  //## end ClCommunicationAPI::~ClCommunicationAPI%959594567.body
}



//## Other Operations (implementation)
m4return_t ClCommunicationAPI::ExecuteAction (m4pchar_t ai_pcActionName, ClParamList *ai_pParams)
{
  //## begin ClCommunicationAPI::ExecuteAction%959594568.body preserve=yes

	void *thepointer;

	if(!strcmp(ai_pcActionName,"ReadingASCII"))
	{
		m4return_t RetValue;
		if(M4_ERROR==ai_pParams->GetM4Return(M4_PARAM_RETURN_RESULT,RetValue))
		{
			ReadReady();
			return M4_ERROR;
		}
		else
		{
			if(M4_ERROR==RetValue)
			{
				ReadReady();
				return M4_ERROR;
			}
			else
			{
				if(M4_ERROR==ai_pParams->GetPointer(M4_PARAM_READED_BUFFER,thepointer))
				{
					ReadReady();
					return M4_ERROR;
				}
				else
				{
					m_iCommStatus = M4_SUCCESS;
					m_pcBuffer=(m4pchar_t)thepointer;
					ReadReady();
					m_pReadParams=ai_pParams;
				}
			}
		}	

		// Return error to stop listening because the listening is done.
		// Due to this reason there is not call to method StopListening
		// from the method "ClCommunicationAPI::Communicate"
		return M4_ERROR;
	}
  
	return M4_SUCCESS;
			
  //## end ClCommunicationAPI::ExecuteAction%959594568.body
}

void ClCommunicationAPI::ReadReady ()
{
  //## begin ClCommunicationAPI::ReadReady%959766908.body preserve=yes

	m_bReadyRead=M4_TRUE;
	m_ReadCondition.Signal();

  //## end ClCommunicationAPI::ReadReady%959766908.body
}

void ClCommunicationAPI::WriteReady ()
{
  //## begin ClCommunicationAPI::WriteReady%959766909.body preserve=yes

	m_bReadyWrite=M4_TRUE;
	m_WriteCondition.Signal();

  //## end ClCommunicationAPI::WriteReady%959766909.body
}

void ClCommunicationAPI::WaitWrite ()
{
  //## begin ClCommunicationAPI::WaitWrite%959766910.body preserve=yes

	while(m_bReadyWrite == M4_FALSE)
	m_WriteCondition.Wait(-1);

  //## end ClCommunicationAPI::WaitWrite%959766910.body
}

void ClCommunicationAPI::WaitRead ()
{
  //## begin ClCommunicationAPI::WaitRead%959766911.body preserve=yes

	while(m_bReadyRead == M4_FALSE)
	m_ReadCondition.Wait(-1);

  //## end ClCommunicationAPI::WaitRead%959766911.body
}

void ClCommunicationAPI::Clear ()
{
  //## begin ClCommunicationAPI::Clear%959766912.body preserve=yes

	m_pcBuffer=NULL;
	m_bReadyWrite=M4_FALSE;
	m_bReadyRead=M4_FALSE;
	m_iCommStatus = M4_ERROR;

  //## end ClCommunicationAPI::Clear%959766912.body
}

m4return_t ClCommunicationAPI::Communicate (char *ai_szBuffer)
{
  //## begin ClCommunicationAPI::Communicate%959869670.body preserve=yes

	if(s_oTheComms->GetError())
		return M4_ERROR;

	m4handle_t hSessionASCII = ClIdGenerator::GetNewId();

	m4return_t iRet;
		
	iRet=s_oTheComms->OpenConnection(hSessionASCII,s_strDispatcherHost,s_iDispatcherPort,M4_COMM_ASCII_PROTOCOL);
	
	if(M4_SUCCESS!=iRet)
	{

		// m4log error message
		if (0 < s_strDispatcherHost.size())
			BL_ERRORF(M4_ERR_XMLADMCLNT_OPENCONNECT, s_strDispatcherHost.c_str() << s_iDispatcherPort);
		else
			BL_ERRORF(M4_ERR_XMLADMCLNT_OPENCONNECT, "unknown" << s_iDispatcherPort);
		return M4_ERROR;
	}
	

	iRet=s_oTheComms->StartListening(hSessionASCII,"ReadingASCII",this);

	iRet=s_oTheComms->Write(hSessionASCII,ai_szBuffer, (strlen(ai_szBuffer) + 1),"WrittingASCII",this);

	if(M4_SUCCESS!=iRet)
	{

		// m4log error message
		//SETCODE(M4_ERR_XMLADMCLNT_WRITEMSG,ERRORLOG,"Error sending command to Dispatcher.\nFailure writing message.");	
		BL_ERROR(M4_ERR_XMLADMCLNT_WRITEMSG);

		return M4_ERROR;
	}
	else
	{
		this->WaitRead();		
	}


	// Important note: The next line is commented because the method
	// "ClCommunicationAPI::ExecuteAction" returns M4_ERROR (al the end of the method)
	// instead of M4_SUCCESS to stop the listeninig of the communication layer (the
	// method "StopListening" is not done at the end of the method
	// "ClCommunicationAPI::ExecuteAction" because it would be worst the performance
	//
	//	iRet=s_oTheComms->StopListening(hSessionASCII);
	//

	if(M4_ERROR==iRet || M4_ERROR == m_iCommStatus)
	{

		// m4log error message
		//SETCODE(M4_ERR_XMLADMCLNT_FAILLISTEN,ERRORLOG,"Error sending command to Dispatcher.\nFailure listening answer from Dispatcher.");	
		BL_ERROR(M4_ERR_XMLADMCLNT_FAILLISTEN);

		return M4_ERROR;
	}

	iRet=s_oTheComms->CloseConnection(hSessionASCII);

	if(M4_ERROR==iRet)
	{
		// m4log error message
		//SETCODE(M4_ERR_XMLADMCLNT_FAILCLOSE,ERRORLOG,"Error sending command to Dispatcher.\nFailure closing connection.");	
		BL_ERROR(M4_ERR_XMLADMCLNT_FAILCLOSE);

		return M4_ERROR;
	}
	else
		return M4_SUCCESS;

  //## end ClCommunicationAPI::Communicate%959869670.body
}

m4pchar_t ClCommunicationAPI::GetBuffer ()
{
  //## begin ClCommunicationAPI::GetBuffer%960193450.body preserve=yes
	return m_pcBuffer;
  //## end ClCommunicationAPI::GetBuffer%960193450.body
}

// Additional Declarations
  //## begin ClCommunicationAPI%39323C3C00DA.declarations preserve=yes

extern "C" {

m4return_t GetParametersFromStream(m4string_t ai_strParameters, m4string_t &ao_strHost,  m4string_t &ao_strConfiguration, m4string_t &ao_strPort)
{
	
	int iPos = 0;
	int iLastPos = 0;

	
	if (0 < ai_strParameters.size())
	{
	
		iPos = ai_strParameters.find_first_of(M4StreamSeparator);

		if (M4_STR_NPOS != iPos) {
			
			ao_strHost = ai_strParameters.substr(0, iPos);

			iLastPos = iPos + 1;

			iPos = ai_strParameters.find_first_of(M4StreamSeparator, iLastPos);

			if (M4_STR_NPOS == iPos)
			{

				iPos = ai_strParameters.find_first_of(M4StreamTerminator, iLastPos);
				
				if (M4_STR_NPOS != iPos) 
				{
					ao_strConfiguration = ai_strParameters.substr(iLastPos, iPos - iLastPos);
					return M4_SUCCESS;
				}
				else
				{
					// m4log error message
					//SETCODE(M4_ERR_XMLADMCLNT_NOMONITORPARAM,ERRORLOG,"Error preparing Dispatcher command.\nCommand parameter stream from Server Monitor incorrect or not provided.");	
					BL_ERROR(M4_ERR_XMLADMCLNT_NOMONITORPARAM);

					return M4_ERROR;
				}
			}
			else
			{
				ao_strConfiguration = ai_strParameters.substr(iLastPos, iPos - iLastPos);
				iLastPos = iPos + 1;
				iPos = ai_strParameters.find_first_of(M4StreamTerminator, iLastPos);
				if (M4_STR_NPOS != iPos)
				{
					ao_strPort = ai_strParameters.substr(iLastPos, iPos - iLastPos);
					return M4_SUCCESS;
				}
				else
				{
					// m4log error message
					//SETCODE(M4_ERR_XMLADMCLNT_NOMONITORPARAM,ERRORLOG,"Error preparing Dispatcher command.\nCommand parameter stream from Server Monitor incorrect or not provided.");	
					BL_ERROR(M4_ERR_XMLADMCLNT_NOMONITORPARAM);

					return M4_ERROR;
				}
			}
		}
		else
		{
			// m4log error message
			//SETCODE(M4_ERR_XMLADMCLNT_NOMONITORPARAM,ERRORLOG,"Error preparing Dispatcher command.\nCommand parameter stream from Server Monitor incorrect or not provided.");	
			BL_ERROR(M4_ERR_XMLADMCLNT_NOMONITORPARAM);

			return M4_ERROR;
		}
	}
	else
	{

		// m4log error message
		//SETCODE(M4_ERR_XMLADMCLNT_NOCOMMANDPARAM,ERRORLOG,"Error preparing Dispatcher command.\nCommand parameters incorrect or not provided.");	
		BL_ERROR(M4_ERR_XMLADMCLNT_NOCOMMANDPARAM);

		return M4_ERROR;
	}
}


m4return_t SendXMLCommand(m4pchar_t ai_szCommand, m4pchar_t *ao_szXMLAnswer)
{
	m4return_t iRet = M4_ERROR;
	ClCommunicationAPI ExecutorAcceptor;
	m4pchar_t szXMLAnswer = NULL;


	if (NULL != ai_szCommand && (0 != strcmp(ai_szCommand, "")) && 
			NULL !=ao_szXMLAnswer)
	{
	
		iRet = ExecutorAcceptor.Communicate(ai_szCommand);

		if (M4_SUCCESS != iRet)
			return M4_ERROR;

		// Get the XML answer

		szXMLAnswer = ExecutorAcceptor.GetBuffer();

		if (NULL == szXMLAnswer)
		{
			// m4log error message
			//SETCODE(M4_ERR_XMLADMCLNT_NOCOMMANDANSWER,ERRORLOG,"Error receiving answer from Dispatcher.\nNo answer for the sent command.");	
			BL_ERROR(M4_ERR_XMLADMCLNT_NOCOMMANDANSWER);

			// Clear the answer buffer from the Dispatcher
			ExecutorAcceptor.Clear();
			return M4_ERROR;
		}

		*ao_szXMLAnswer = new m4char_t[(strlen(szXMLAnswer) + 1)];

		if (NULL == *ao_szXMLAnswer)
		{

			// m4log error message
			//SETCODE(M4_ERR_XMLADMCLNT_NOFREEMEMORY,ERRORLOG,"Error allocating memory.\nNo free memory. Please, close one or more applications.");	
			BL_ERROR(M4_ERR_XMLADMCLNT_NOFREEMEMORY);

			// Clear the answer buffer from the Dispatcher
			ExecutorAcceptor.Clear();
			return M4_ERROR;
		}

		(void) strcpy(*ao_szXMLAnswer, szXMLAnswer);

		// Clear the answer buffer from the Dispatcher
		ExecutorAcceptor.Clear();

		return M4_SUCCESS;
	}
	else
		return M4_ERROR;
}


m4return_t DecodeXMlAnswer(m4pchar_t ai_szXMLAnswer, m4string_t &ao_strAnswer)
{

	long lXMLAnswerLen = 0;
	SAXParser parser;
	M4SAXAdmClient oXMLHandle;
	MemBufInputSource * poMemBuffer = NULL;
	long lAnswerSize = 0;
	m4pchar_t szAnswer = NULL;


	if (NULL != ai_szXMLAnswer && (0 != strcmp(ai_szXMLAnswer, "")))
	{


		parser.setDocumentHandler(&oXMLHandle);
		parser.setErrorHandler(&oXMLHandle);


		lXMLAnswerLen = strlen (ai_szXMLAnswer) + 1;

		poMemBuffer = 
			new MemBufInputSource((const XMLByte *) ai_szXMLAnswer,
								  lXMLAnswerLen,
								  "generic",
								  false);
		try
		{
			parser.parse( * poMemBuffer );
		}

		catch (const XMLException&)
		{

			// m4log error message
			//SETCODE(M4_ERR_XMLADMCLNT_XMLEXCEPTION,ERRORLOG,"Error parsing XML answer from Dispatcher.\nException while parsing the XML Dispatcher answer.");	
			BL_ERROR(M4_ERR_XMLADMCLNT_XMLEXCEPTION);

			if (NULL != poMemBuffer)
				delete poMemBuffer;
			return M4_ERROR;
		}

		if (NULL != poMemBuffer)
		{
			delete poMemBuffer;
		}

		// Check any error in the parser
		if (M4_SUCCESS != oXMLHandle.GetError())
		{

			// m4log error message
			//SETCODE(M4_ERR_XMLADMCLNT_XMLPARSEERROR,ERRORLOG,"Error parsing XML answer from Dispatcher.\nXML Dispatcher answer sintactically bad formed.");	
			BL_ERROR(M4_ERR_XMLADMCLNT_XMLPARSEERROR);

			return M4_ERROR;
		}


		lAnswerSize = oXMLHandle.GetOutStreamSize();

		// Check memory allocation
		if (0 == lAnswerSize)
		{
			// m4log error message
			//SETCODE(M4_ERR_XMLADMCLNT_NOXMLANSWER,ERRORLOG,"Error parsing XML answer from Dispatcher.\nNo XML Dispatcher answer (size = 0).");	
			BL_ERROR(M4_ERR_XMLADMCLNT_NOXMLANSWER);

			return M4_ERROR;
		}
		
		szAnswer = new m4char_t[lAnswerSize];

		if (M4_ERROR == oXMLHandle.GetOutStream(szAnswer, lAnswerSize))
		{
			// m4log error message
			//SETCODE(M4_ERR_XMLADMCLNT_NOFREEMEMORY,ERRORLOG,"Error allocating memory.\nNo free memory. Please, close one or more applications");	
			BL_ERROR(M4_ERR_XMLADMCLNT_NOFREEMEMORY);


			delete szAnswer;
			return M4_ERROR;
		}
		
		ao_strAnswer.assign(szAnswer);

		delete szAnswer;
		return M4_SUCCESS;
	}
	else
		return M4_ERROR;
}


m4return_t GetAdminClientAnswer(m4string_t ai_strAnswer, m4string_t &ao_strReturnCommand, m4string_t &ao_strAdmClientAnswer)
{
/*
	m4pchar_t szAnswerCopy = NULL;
	m4pchar_t szReturnCommand = NULL;
	m4pchar_t szAdmClientAnswer = NULL;
	m4char_t szDelimiter[2];
*/

	int iPos = 0;
	int iLastPos = 0;


	if (0 < ai_strAnswer.size())
	{

		iPos = ai_strAnswer.find_first_of(M4StreamSeparator);

		if (M4_STR_NPOS == iPos) 
		{
			iPos = ai_strAnswer.find_first_of(M4StreamTerminator);

			if (M4_STR_NPOS == iPos) {

				// m4log error message
				//SETCODE(M4_ERR_XMLADMCLNT_XMLANSWERERROR,ERRORLOG,"Error processing answer from Dispatcher.\nIncorrect answer or not provided.");	
				BL_ERROR(M4_ERR_XMLADMCLNT_XMLANSWERERROR);

				return M4_ERROR;
			}
			else
			{
				// There are not parameters but there is command

				ao_strReturnCommand.assign(ai_strAnswer, 0, iPos);
				return M4_SUCCESS;
			}
		}

		ao_strReturnCommand.assign(ai_strAnswer, 0, iPos);

		// Get the answer parameters (the admin. client answer)

		iLastPos = iPos + 1;

		iPos = ai_strAnswer.find_first_of(M4StreamTerminator, iLastPos);

		if (M4_STR_NPOS == iPos) 
		{
			// m4log error message
			//SETCODE(M4_ERR_XMLADMCLNT_XMLANSWERERROR,ERRORLOG,"Error processing answer from Dispatcher.\nIncorrect answer or not provided.");	
			BL_ERROR(M4_ERR_XMLADMCLNT_XMLANSWERERROR);

			return M4_ERROR;
		}

		ao_strAdmClientAnswer.assign(ai_strAnswer, iLastPos, iPos + 1);

		return M4_SUCCESS;
	}
	else
	{
		// m4log error message
		//SETCODE(M4_ERR_XMLADMCLNT_XMLANSWERERROR,ERRORLOG,"Error processing answer from Dispatcher.\nIncorrect answer or not provided.");
		BL_ERROR(M4_ERR_XMLADMCLNT_XMLANSWERERROR);

		return M4_ERROR;
	}

}


m4return_t CheckCommandName (m4string_t ai_strCommandName)
{

	m4return_t iRet;


	iRet = M4_ERROR;

	if (0 < ai_strCommandName.size())

		if (ai_strCommandName == M4ConfigurationsCommand ||
			ai_strCommandName == M4StartServerCommand ||
			ai_strCommandName == M4StopServerCommand ||
			ai_strCommandName == M4StopDispatcherCommand || 
			ai_strCommandName == M4RemoveServerCommand )
		{
			iRet = M4_SUCCESS;
		}
		else
		{
			//SETCODEF(M4_ERR_XMLADMCLNT_NODPCHCOMMAND,ERRORLOG,"Error preparing Dispatcher command.\nCommand \"%s\" not exist.", strCommand.c_str());	
			BL_ERRORF(M4_ERR_XMLADMCLNT_NODPCHCOMMAND, ai_strCommandName.c_str());
		}

	else
	{
		// m4log error message
		//SETCODE(M4_ERR_XMLADMCLNT_NODPCHCOMMANDBIS,ERRORLOG,"Error preparing Dispatcher command.\nThe command not exist.");
		BL_ERROR(M4_ERR_XMLADMCLNT_NODPCHCOMMANDBIS);
	}

	return iRet;
}



long WINAPI M4InitConnect(LPSTR ai_szHost, int ai_iPort, LPSTR ai_szUser, LPSTR ai_szPassword)
{
	m4string_t strHost=ai_szHost;
	ClCommunicationAPI pExecutorASCII;
	m4handle_t hSessionASCII;
	hSessionASCII = ClIdGenerator::GetNewId();
	m4return_t iRet = M4_ERROR;
	//ClCommunication thecomms(6666);
	s_oTheComms = new ClCommunication(6666);



	if (NULL == s_oTheComms)
	{
		// m4log error message
		//SETCODE(M4_ERR_XMLADMCLNT_NOFREEMEMORY,ERRORLOG,"Error allocating memory.\nNo free memory. Please, close one or more applications");	
		BL_ERROR(M4_ERR_XMLADMCLNT_NOFREEMEMORY);

		return(-1);
	}

	// Set global-local variables

	if (ai_szHost && (0 != strcmp(ai_szHost, "")))
		s_strDispatcherHost = ai_szHost;
	else
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOINITCONNECTPARAM,ERRORLOG,"Error initializing Dispatcher connection.\nParameter %s not provided.", "Host");	
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOINITCONNECTPARAM, "Host");

		return(-1);
	}

	if (ai_iPort > 0) {
		s_iDispatcherPort = ai_iPort;
	}
	else
	{

		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOINITCONNECTPARAM,ERRORLOG,"Error initializing Dispatcher connection.\nParameter %s not provided.", "Dispatcher Port");	
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOINITCONNECTPARAM, "Dispatcher Port");

		return(-1);
	}
	
	if (ai_szUser && (0 != strcmp(ai_szUser, "")))
		s_strUser = ai_szUser;
	else
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOINITCONNECTPARAM,ERRORLOG,"Error initializing Dispatcher connection.\nParameter %s not provided.", "User Name");	
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOINITCONNECTPARAM, "User Name");
				
		return(-1);
	}

	if (ai_szPassword && (0 != strcmp(ai_szPassword, "")))
		s_strPassword = ai_szPassword;
	else
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOINITCONNECTPARAM,ERRORLOG,"Error initializing Dispatcher connection.\nParameter %s not provided.", "Password");	
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOINITCONNECTPARAM, "Password");

		return(-1);
	}

	// The communication is no-connection oriented, so this function
	// only check that the connection is posible
	
	if(s_oTheComms->GetError()) {

		// Mensaje de error de la log
		SETCODE(M4_ERR_XMLADMCLNT_COMMUNICALAYERFAIL,ERRORLOG,"Error in communication layer.\nThe Dispatcher communication port could be occupied.");	
		BL_ERROR(M4_ERR_XMLADMCLNT_COMMUNICALAYERFAIL);

		return(-1);
	}
	
		// Initialize the XML4C2 system
		// (this is neccesary to all works fine)
	try
	{
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException&)
	{

		// m4log error message
		//SETCODE(M4_ERR_XMLADMCLNT_NOINITXMLLIB,ERRORLOG,"Error parsing XML answer from Dispatcher.\nFailure initializing the XML library");	
		BL_ERROR(M4_ERR_XMLADMCLNT_NOINITXMLLIB);

		cerr << "Error during initialization the xerces library\n" << endl << flush;
		return -1;
	}

	return 0;
}


long WINAPI M4StopCommunication()
{

	// Stop the threads of the Communication Layer
	// (It is necesary the admon. client stops)
	s_oTheComms->StopCommunicationLayer();

	// Return SUCCESS
	return 0;
}

long WINAPI M4ExecuteRequest(LPSTR ai_szCommand, LPSTR ai_szParameters, LPSTR ao_szAnswer, long FAR* ao_lAnswerSize)
{	
	m4char_t *szBuffer = NULL;
	m4string_t strHost;
	m4string_t strConfiguration;
	m4string_t strBasePort;
	m4char_t *szXMLAnswer = NULL;
	long lAnswerSize = 0;
	m4string_t strAnswer;
	long lXMLAnswerLen = 0;
	m4return_t iRet = M4_ERROR;
	m4string_t strReturnCommand;
	m4string_t strAdmClientAnswer;
	long lAdmClientAnswerSize = 0;

	
	if ((ai_szCommand) && (0 != strcmp(ai_szCommand, ""))) 
	{

		ClXMLOutStream oXMLStream;
		m4string_t strCommand = ai_szCommand;


		if (strCommand != M4ConfigurationsCommand && strCommand != M4StopDispatcherCommand && 
			(NULL == ai_szParameters || (NULL != ai_szParameters && 0 == strcmp("", ai_szParameters)))) 
		{
			// m4log error message
			if (0 < strCommand.size())
				//SETCODEF(M4_ERR_XMLADMCLNT_NODPCHCOMMANDPARAMS,ERRORLOG,"Error preparing Dispatcher command.\nParameters of the command \"%s\" not provided.", strCommand.c_str());
				BL_ERRORF(M4_ERR_XMLADMCLNT_NODPCHCOMMANDPARAMS, strCommand.c_str());
			else
				//SETCODE(M4_ERR_XMLADMCLNT_NODPCHCOMMANDPARAMSBIS,ERRORLOG,"Error preparing Dispatcher command.\nCommand parameters not provided.");
				BL_ERROR(M4_ERR_XMLADMCLNT_NODPCHCOMMANDPARAMSBIS);
			return -1;
		}
		
		if (ai_szParameters && 0 != strcmp("", ai_szParameters))
			m4string_t strParameters = ai_szParameters;

		// Configurations command
		if (strCommand == M4ConfigurationsCommand)
		{
			if (M4_SUCCESS != (iRet = oXMLStream.ConfigurationsCommand(s_strUser, s_strPassword)))
				return -1;
		}

		// Dispatcher Shut Down command
		else if (strCommand == M4StopDispatcherCommand)
		{
			if (M4_SUCCESS != (iRet = oXMLStream.StopDispatcherCommand(s_strUser, s_strPassword)))
				return -1;
		}

		// StartServer command
		else if (strCommand == M4StartServerCommand)
		{
			m4string_t strParameters(ai_szParameters);

			if (0 == strParameters.size())
			{
				// m4log error message
				if (0 < strCommand.size())
					//SETCODEF(M4_ERR_XMLADMCLNT_NODPCHCOMMANDPARAMS,ERRORLOG,"Error preparing Dispatcher command.\nParameters of the command \"%s\" not provided.", strCommand.c_str());	
					BL_ERRORF(M4_ERR_XMLADMCLNT_NODPCHCOMMANDPARAMS, strCommand.c_str());
				else
					//SETCODE(M4_ERR_XMLADMCLNT_NODPCHCOMMANDPARAMS,ERRORLOG,"Error preparing Dispatcher command.\nCommand parameters not provided.");
					BL_ERROR(M4_ERR_XMLADMCLNT_NODPCHCOMMANDPARAMSBIS);
				
				return -1;
			}

			iRet = GetParametersFromStream(strParameters, strHost, strConfiguration, strBasePort);

			if (M4_ERROR == iRet)
				return -1;

			if (0 == strBasePort.size())
			{
				if (M4_SUCCESS != (iRet = oXMLStream.StartServerCommand(strHost, strConfiguration, s_strUser, s_strPassword)))
					return -1;
			}
			else
			{
				if (M4_SUCCESS != (iRet = oXMLStream.StartServerCommand(strHost, strConfiguration, s_strUser, s_strPassword, strBasePort)))
					return -1;
			}
		}
		
		// StopServer command
		else if (strCommand == M4StopServerCommand)
		{

			m4string_t strParameters(ai_szParameters);

			if (0 == strParameters.size())
				return -1;

			iRet = GetParametersFromStream(strParameters, strHost, strConfiguration, strBasePort);

			if (M4_ERROR == iRet)
				return -1;

			if (0 == strBasePort.size())
			{
				if (M4_SUCCESS != (iRet = oXMLStream.StopServerCommand(strHost, strConfiguration, s_strUser, s_strPassword)))
					return -1;
			}
			else
			{
				if (M4_SUCCESS != (iRet = oXMLStream.StopServerCommand(strHost, strConfiguration, s_strUser, s_strPassword, strBasePort)))
					return -1;
			}

		}

		// RemoveServer command
		else if (strCommand == M4RemoveServerCommand)
		{

			m4string_t strParameters(ai_szParameters);

			if (0 == strParameters.size())
				return -1;

			iRet = GetParametersFromStream(strParameters, strHost, strConfiguration, strBasePort);

			if (M4_ERROR == iRet)
				return -1;

			if (M4_SUCCESS != (iRet = oXMLStream.RemoveServerCommand(strHost, strConfiguration, s_strUser, s_strPassword)))
			{
					return -1;
			}
		}

		// Wrong command
		else
		{
			// m4log error message
			if (0 < strCommand.size())
				//SETCODEF(M4_ERR_XMLADMCLNT_NODPCHCOMMAND,ERRORLOG,"Error preparing Dispatcher command.\nCommand \"%s\" not exist.", strCommand.c_str());	
				BL_ERRORF(M4_ERR_XMLADMCLNT_NODPCHCOMMAND, strCommand.c_str());
			else
				//SETCODE(M4_ERR_XMLADMCLNT_NODPCHCOMMANDBIS,ERRORLOG,"Error preparing Dispatcher command.\nThe command not exist.");
				BL_ERROR(M4_ERR_XMLADMCLNT_NODPCHCOMMANDBIS);

			return -1;
		}
		
		// Allocate the buffer

//		szBuffer = new m4char_t[oXMLStream.GetXMLStreamSize()]; 	

		if (M4_SUCCESS == oXMLStream.GetXMLStream(szBuffer, oXMLStream.GetXMLStreamSize()))
		{

			// Send the XML command
			
			iRet = SendXMLCommand(szBuffer, &szXMLAnswer);

			delete szBuffer;
			szBuffer = NULL;
			
			if (M4_SUCCESS != iRet)
				return -1;

			if (NULL == szXMLAnswer) 
				return -1;


			// Decode the XML answer

			iRet = DecodeXMlAnswer(szXMLAnswer, strAnswer);

			delete szXMLAnswer;

			if (M4_SUCCESS != iRet)
				return -1;

			// Get the answer to the admin. client	

			// Only answer to the known Commmands
			if (M4_SUCCESS == CheckCommandName(strCommand))
			{

				iRet = GetAdminClientAnswer(strAnswer, strReturnCommand, strAdmClientAnswer);

				// Check result
				if ( M4_SUCCESS != iRet)
				{
					return -1;
				}
			
				// Check authentication answer error.
				if (strReturnCommand == M4AuthAnswerCommand)
				{
					// Look for "error" in authentication answer.
					if (strAdmClientAnswer.find("error") != M4_STR_NPOS)
					{
						BL_ERROR(M4_ERR_XMLADMCLNT_AUTHENTICATION);
						return -1;
					}
				}

				if ((strlen(strAdmClientAnswer.c_str()) + 1) > (unsigned) *ao_lAnswerSize)
				{

					// Return the necessary answer buffer size
					*ao_lAnswerSize = strlen(strAdmClientAnswer.c_str()) + 1;

					// delete szAdmClientAnswer;
					return -1;
				}
					
				(void) strcpy(ao_szAnswer, strAdmClientAnswer.c_str());
				//delete szAdmClientAnswer;

				// The end
				return 0;
			}

			// The end
			return 0;
		}
		else
		{
			// m4log error message
			//SETCODE(M4_ERR_XMLADMCLNT_NOFREEMEMORY,ERRORLOG,"Error allocating memory.\nNo free memory. Please, close one or more applications");	
			BL_ERROR(M4_ERR_XMLADMCLNT_NOFREEMEMORY);

			if (NULL != szBuffer)
				delete szBuffer;
			return -1;
		}
	}
	else
		return -1;
}


}

  //## end ClCommunicationAPI%39323C3C00DA.declarations
//## begin module%39323E160167.epilog preserve=yes
//## end module%39323E160167.epilog


// Detached code regions:
// WARNING: this code will be lost if code is regenerated.
#if 0
//## begin ClCommunicationAPI::CheckCommandName%1003999216.body preserve=no

	m4return_t iRet;


	iRet = M4_ERROR;

	if (0 < ai_strCommandName.size())

		if (ai_strCommandName == M4ConfigurationsCommand ||
			ai_strCommandName == M4StartServerCommand ||
			ai_strCommandName == M4StopServerCommand ||
			ai_strCommandName == M4StopDispatcherCommand ||
			ai_strCommandName == M4RemoveDispatcherCommand)
		{
			iRet = M4_SUCCESS;
		}
		else
		{
			//SETCODEF(M4_ERR_XMLADMCLNT_NODPCHCOMMAND,ERRORLOG,"Error preparing Dispatcher command.\nCommand \"%s\" not exist.", strCommand.c_str());	
			BL_ERRORF(M4_ERR_XMLADMCLNT_NODPCHCOMMAND, ai_strCommandName.c_str());
		}

	else
	{
		// m4log error message
		//SETCODE(M4_ERR_XMLADMCLNT_NODPCHCOMMANDBIS,ERRORLOG,"Error preparing Dispatcher command.\nThe command not exist.");
		BL_ERROR(M4_ERR_XMLADMCLNT_NODPCHCOMMANDBIS);
	}

	return iRet;

//## end ClCommunicationAPI::CheckCommandName%1003999216.body

#endif

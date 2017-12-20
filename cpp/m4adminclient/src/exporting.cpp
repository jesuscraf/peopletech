//==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            AdminMonClient
//	 File:              exporting.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines the administrator interface with Visual Basic.
//
//=============================================================================


#ifdef _WINDOWS
#include <windows.h>
#endif

#include <adminmonclient.hpp>

#include <m4types.hpp>
#include <m4trace.hpp>
#include "m4unicode.hpp"




//===========================================================================
// Create the administrator client.
//===========================================================================
ClAdminMonClient *administrator=NULL;


//===========================================================================
// DLL entry point. This function is mandatory by Windows interface.
//===========================================================================
m4bool_t WINAPI DllEntryPoint(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) 
{
	return M4_TRUE;
} 


//===========================================================================
// function: M4ParserConnect
// description: This function connect the administrator client to the server.
//
// input parameters:
// 	ai_szHost - server name
//	ai_uport - server port
//	ai_szUser
//	ai_szPassword
//	ai_Role
//
// return:
//		M4_SUCCESS on success.
//		M4_ERROR on error.
//===========================================================================
extern "C" M4_DECL_M4ADMINCLIENT long WINAPI M4ParserConnect(LPSTR ai_szHost, long ai_uport, LPSTR ai_szUser, LPSTR ai_szPassword)
{
	m4AutoTraceLevel(5, "M4ParserConnect");
	m4TraceHeaderLevel(1, cout << "M4ParserConnect: " << ai_szHost << ", " << ai_uport << ", " << ai_szUser << ", " << ai_szPassword << endl);

	if(NULL==administrator)
		administrator=new ClAdminMonClient;

	return administrator->Connect(ai_szHost, (m4uint32_t)ai_uport, ai_szUser, ai_szPassword);
}


//===========================================================================
// function: M4ParserDisconnect
// description: This function disconnect the administrator client from
//		the server.
//
// return:
//		M4_SUCCESS on success.
//		M4_ERROR on error.
//===========================================================================
extern "C" M4_DECL_M4ADMINCLIENT long WINAPI M4ParserDisconnect()
{
	m4AutoTraceLevel(5, "M4ParserDisconnect");

	return administrator->Disconnect();
}



//===========================================================================
// function: M4ParserExecute
// description: This function execute a command in the server.
//
// input parameters:
//	ai_pSentence - sentence to execute. This sentence is a list of commands 
//		separated with semicolons.
//		The valid commands are in the acparser.ini file.
//
// output parameters:
// 	ao_szResult - the server answer. This is a answer list separated with 
//		colons;
//
// input/output parameters:
//	ao_uSize	- on input is the max lenght for the answer. On output is the
//		lenght of the answer.
//
// return:
//		M4_SUCCESS on success.
//		M4_ERROR on error.
//===========================================================================
extern "C" M4_DECL_M4ADMINCLIENT long WINAPI M4ParserExecute (LPSTR ai_pSentence, LPSTR ao_szResult, long FAR* ao_uSize)
{
	m4AutoTraceLevel(5, "M4ParserExecute");
	m4TraceHeaderLevel(1, cout << "M4ParserExecute command: " << ai_pSentence << endl);

	m4int32_t RetVal;

	//-----------------------------------------------------------------------
	// send the sentence to the server.
	//-----------------------------------------------------------------------
	m4uint16_t uiSize = (m4uint16_t)*ao_uSize;
	RetVal = administrator->Execute(ai_pSentence, ao_szResult, uiSize);
	if (RetVal == M4_WARNING) 
	{
		*ao_uSize = uiSize;
	    //--------------------------------------------------------------------------
	    // set RetVal to success because administrator.Execute() can return warning.
	    //--------------------------------------------------------------------------
		RetVal = M4_SUCCESS;
	}

#ifdef _DEBUG
	m4pchar_t outputfile=NULL;
	if(outputfile=getenv("M4_ADMIN_DUMP_FILE") )
	{
		FILE *ofile=NULL;

		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniNative ;

		if(NULL!=(ofile=M4Fopen(outputfile, M4UniAppend, eEncoding)))
		{
			fprintf(ofile,"{%s}\n",ai_pSentence);
			if(ao_uSize)
			{
				fprintf(ofile,"[%s]\n",ao_szResult);
			}
			else
				fprintf(ofile,"[NO ANSWER]\n");
			fclose(ofile);
		}
	}


#endif // _DEBUG
	//-----------------------------------------------------------------------
	// normal return.
	//-----------------------------------------------------------------------
	m4TraceHeaderLevel(1, cout << "M4ParserExecute answer: " << ao_szResult << endl);
	return RetVal;
}
 

 
//===========================================================================
// function: M4ParserCheckConnection
// description: This function checks the connection state.
//
// return:
//		M4_SUCCESS if the administrator is connected.
//		M4_ERROR otherwise.
//===========================================================================
extern "C" M4_DECL_M4ADMINCLIENT long WINAPI M4ParserCheckConnection()
{
	m4AutoTraceLevel(5, "M4ParserCheckConnection");

	if (M4_TRUE == administrator->CheckConnection())
	{
		return M4_SUCCESS;
	}
	
	return M4_ERROR;
}



//===========================================================================
// function: M4AdminGetErrorLog
// description: This function return a string with errors in the admin
//	log object.
//
// return:
//		M4_SUCCESS on success.
//		M4_ERROR otherwise.
//===========================================================================
extern "C" M4_DECL_M4ADMINCLIENT long WINAPI M4AdminGetLogError(LPSTR ao_logType, LPSTR ao_logCode, LPSTR ao_logMessage, long ai_logMessageSize)
{
	m4AutoTraceLevel(5, "M4AdminGetErrorLog");

	M4ClString logType, logCode, logMessage;
	m4return_t retCode = M4_ERROR;
	
	retCode = administrator->GetLogError(logType, logCode, logMessage);

	if (M4_SUCCESS != retCode)
	{
		return M4_ERROR;
	}

	// copy logType.
	strcpy(ao_logType, logType.c_str());

	// copy logCode.
	strcpy(ao_logCode, logCode.c_str());

	// copy logMessage.
	int logSize = strlen(logMessage) + 1;
	if (logSize > ai_logMessageSize)
	{
		strncpy(ao_logMessage, logMessage, ai_logMessageSize);
		ao_logMessage[ai_logMessageSize-1] = '\0';
	}
	else
	{
		strcpy(ao_logMessage, logMessage);
	}

	// if ao_logMessage has '\n' characters, replace by white space.
	char *p = ao_logMessage;
	while (*p != NULL)
	{
		if ('\n' == *p)
		{
			*p = ' ';
		}
		p++;
	}

	return M4_SUCCESS;
}


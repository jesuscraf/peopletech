//## begin module%393CFB8C016F.cm preserve=no
//## end module%393CFB8C016F.cm

//## begin module%393CFB8C016F.cp preserve=no
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
//## end module%393CFB8C016F.cp

//## Module: M4XMLStreamGen%393CFB8C016F; Package body
//## Subsystem: M4Dispatcher::src%3923BBFB0332
//## Source file: F:\integration\m4dispatcher\src\m4xmlstreamgen.cpp

//## begin module%393CFB8C016F.additionalIncludes preserve=no
//## end module%393CFB8C016F.additionalIncludes

//## begin module%393CFB8C016F.includes preserve=yes
#include <xmlstreamgenerator.hpp>
#include <sitenodeinfo.hpp>
#include <dspinvoker.hpp>
#include <dspparams.hpp>
#include <pdus.hpp>
#include <protocol.hpp>
#include <basiclog.hpp>
#include <m4dpchres.hpp>
#include <actionidentifiers.hpp>


#include <m4memdbg.h>
#include <m4srvtrace.h>

#define M4_SRV_TRACE_COMP_ID "DPCH"

//## end module%393CFB8C016F.includes

// dspanswergen
#include <dspanswergen.hpp>
// M4XMLStreamGen
#include <m4xmlstreamgen.hpp>
//## begin module%393CFB8C016F.declarations preserve=no
//## end module%393CFB8C016F.declarations

//## begin module%393CFB8C016F.additionalDeclarations preserve=yes
const m4pchar_t CANDIDATE_SERVER_HOST = "CANDIDATE_SERVER_HOST" ; 
const m4pchar_t CANDIDATE_SERVER_PORT = "CANDIDATE_SERVER_PORT" ;

#define strFilled	3

m4return_t ASCIItoHEX ( m4pcchar_t ai_szASCII , m4int_t ai_iLen , m4pchar_t ao_szHEX , m4int_t ai_iLenBufferOut , m4int_t & ao_iLenOut );
//## end module%393CFB8C016F.additionalDeclarations


// Class ClStarXMLAnswerGen 


//## Other Operations (implementation)
m4return_t ClStarXMLAnswerGen::GenerateAnswer (ClSiteNodeContainer &ai_oSiteNodeContainer, m4void_t *&ao_strStream, m4return_t ai_iRet, ClLogSystem *ai_poLog)
{
  //## begin ClStarXMLAnswerGen::GenerateAnswer%963825081.body preserve=yes
	m4return_t iRet ;
	m4string_t strParameters;
	m4uint32_t  iStreamSize , iLenout , iCount;
	ClSiteNodeInformation oNodeInfo ;
	M4XMLStreamGenerator oXMLGenerator ;
//	m4pchar_t buffer ;
	m4string_t szRet = "SUCCESS" ;
	m4pchar_t pLogBuffer ;
	m4pchar_t pcHEXLog = NULL;
	m4int_t iHexSize ;


	if ( M4_ERROR == ai_oSiteNodeContainer.GetNextNode ( oNodeInfo ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOSITENODE,
			"Error getting site node. There is not any site node." );
		return M4_ERROR ;
	}
	
	oXMLGenerator.MakeXMLDeclaration();

	oXMLGenerator.InsertComment("@version");

	oXMLGenerator.MakeElement("dispatcher"); 
	oXMLGenerator.EndElement() ; //element=dispatcher

	oXMLGenerator.MakeElement("command") ;
	oXMLGenerator.AddEscapedElementAttribute ( "name" , (m4pcchar_t) "startserver"); 
	oXMLGenerator.EndElement() ; //element=command

	if ( M4_SUCCESS != ai_iRet )
	{
		szRet = "ERROR" ;
	}

//	oXMLGenerator.MakeElement("status");
	oXMLGenerator.MakeElement("params");
	oXMLGenerator.AddEscapedElementAttribute ( "status" , oNodeInfo.GetNodeState().c_str() );
	oXMLGenerator.AddEscapedElementAttribute ( "result" , szRet.c_str() );

	if( ai_poLog )
	{
		iRet = ai_poLog -> GetSerialStringSizeFromMessage(iStreamSize);

		if ( ( iStreamSize> 0 ) & (iRet != M4_ERROR ) )
		{
			// La log está vacía.
	
			pLogBuffer = new m4char_t [ iStreamSize +1];
			
			M4_ASSERT ( pLogBuffer != NULL );
			
			iRet = ai_poLog -> GetSerialStringFromMessage( pLogBuffer, iStreamSize , iLenout );
			
			if ( ( iRet != M4_ERROR ) && ( iLenout ) )
			{
				for ( iCount = 0 ; iCount < iStreamSize ; iCount ++ )
				{
					if ( ! pLogBuffer [ iCount ] )
					{
						pLogBuffer [iCount] = strFilled ;
					}
				}

				pLogBuffer [iStreamSize] = 0 ;

				//Conversion from binary log serialization to hexadecimal string.

				iHexSize = (iStreamSize * 2)  + 1 ;

				pcHEXLog = new m4char_t [ iHexSize ] ;

				M4_ASSERT ( pcHEXLog != NULL ) ;

				iRet = ASCIItoHEX ( pLogBuffer , iStreamSize , pcHEXLog , iHexSize , iHexSize ) ;

				if ( iRet == M4_SUCCESS )
				{
					pcHEXLog [ iHexSize ] = 0 ;
					oXMLGenerator.AddEscapedElementAttribute ( "log" , (m4pcchar_t) pcHEXLog );
				}

				delete [] pcHEXLog;
				
			}
			
			delete []pLogBuffer;
		}
	}

	oXMLGenerator.CloseSingleClosedElement(); //element=status

	oXMLGenerator.CloseElement("command");

	oXMLGenerator.CloseElement("dispatcher");

	iStreamSize = oXMLGenerator.GetXMLStreamSize ( ) ;

	iStreamSize++ ;

//	M4_MDBG_NEW( buffer, buffer = new m4char_t [iStreamSize] ) ;

    iRet = oXMLGenerator.GetXMLStream ( ( char *&) ao_strStream , iStreamSize );

//	ao_strStream = (void*) buffer ;


	return iRet ;
  //## end ClStarXMLAnswerGen::GenerateAnswer%963825081.body
}

// Additional Declarations

// this function returns an error XML to let the administrator that the token was wrong
m4return_t ClAuthXMLAnswerGen::GenerateAnswer (ClSiteNodeContainer &ai_oSiteNodeContainer, m4void_t *&ao_strStream, m4return_t ai_iRet, ClLogSystem *ai_poLog)
{
	m4return_t iRet ;
	m4uint32_t  iStreamSize;
	M4XMLStreamGenerator oXMLGenerator ;

	oXMLGenerator.MakeXMLDeclaration();

	oXMLGenerator.InsertComment("@version");

	oXMLGenerator.MakeElement("dispatcher"); 
	oXMLGenerator.EndElement() ; //element=dispatcher

	oXMLGenerator.MakeElement("command") ;
	oXMLGenerator.AddEscapedElementAttribute ( "name" , (m4pcchar_t) "authanswer"); 
	oXMLGenerator.EndElement() ; //element=command

	oXMLGenerator.MakeElement("params");

	oXMLGenerator.AddEscapedElementAttribute ( "status" , (m4pcchar_t) "error" );
	oXMLGenerator.AddEscapedElementAttribute ( "result" , (m4pcchar_t) "-1" );
	
	oXMLGenerator.CloseSingleClosedElement(); //element=params

	oXMLGenerator.CloseElement("command");

	oXMLGenerator.CloseElement("dispatcher");

	iStreamSize = oXMLGenerator.GetXMLStreamSize ( ) ;

	iStreamSize++ ;

    iRet = oXMLGenerator.GetXMLStream ( ( char *&) ao_strStream , iStreamSize );

	return iRet ;
}
  //## end ClStarXMLAnswerGen%3972EF210161.declarations

// Class ClConfigurationsXMLAnswerGen 


//## Other Operations (implementation)
m4return_t ClConfigurationsXMLAnswerGen::GenerateAnswer (ClSiteNodeContainer &ai_oSiteNodeContainer, m4void_t *&ao_strStream, m4return_t ai_iRet, ClLogSystem *ai_poLog)
{
  //## begin ClConfigurationsXMLAnswerGen::GenerateAnswer%963825083.body preserve=yes
	ClSiteNodeInformation oSiteNode ;
	M4XMLStreamGenerator oXMLGenerator ("'");
	m4char_t szBuffer [32] ;
//	m4pchar_t buffer ;
	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;
	m4return_t iRet ;
	m4uint32_t iStreamSize  ;


	oXMLGenerator.MakeXMLDeclaration();

	oXMLGenerator.InsertComment("@version");

	oXMLGenerator.MakeElement("dispatcher"); 
	oXMLGenerator.EndElement() ;


	oXMLGenerator.MakeElement("command");
	oXMLGenerator.AddEscapedElementAttribute ( "name" , (m4pcchar_t) "configurationsanswer" );
	oXMLGenerator.EndElement( ) ;


	while ( M4_SUCCESS == ai_oSiteNodeContainer.GetNextNode ( oSiteNode ) )
	{
		oXMLGenerator.MakeElement("configuration" ) ;

		oXMLGenerator.AddEscapedElementAttribute ( "host" , oSiteNode.GetLocalHost ( ).c_str() ) ;

		oXMLGenerator.AddEscapedElementAttribute ( "servername" , oSiteNode.GetSiteNodeID( ).c_str() ) ;

		sprintf ( szBuffer, "%d" , oSiteNode.GetBasePort( ) ) ;
		oXMLGenerator.AddEscapedElementAttribute ( "baseport" , (m4pcchar_t) szBuffer ) ;

		sprintf ( szBuffer, "%.2f" , oSiteNode.GetCppLoadFactor( ) ) ;
		oXMLGenerator.AddEscapedElementAttribute ( "oltpload" , (m4pcchar_t) szBuffer ) ;

		sprintf ( szBuffer, "%.2f" , oSiteNode.GetJavaLoadFactor( ) ) ;
		oXMLGenerator.AddEscapedElementAttribute ( "xmlload" , (m4pcchar_t) szBuffer ) ;

		sprintf ( szBuffer, "%.2f" , oSiteNode.GetCPUUsage( ) ) ;
		oXMLGenerator.AddEscapedElementAttribute ( "cpu" , (m4pcchar_t) szBuffer ) ;

		sprintf ( szBuffer, "%.2f" , oSiteNode.GetSystemMemory( ) ) ;
		oXMLGenerator.AddEscapedElementAttribute ( "system_memory" , (m4pcchar_t) szBuffer ) ;

		sprintf ( szBuffer, "%d" , oSiteNode.GetNumSessions( ) ) ;
		oXMLGenerator.AddEscapedElementAttribute ( "sessions" , (m4pcchar_t) szBuffer ) ;

		oXMLGenerator.AddEscapedElementAttribute ( "status" , oSiteNode.GetNodeState( ).c_str()  ) ;

		// eh 197951
		sprintf ( szBuffer, "%d" , oSiteNode.GetDiscarded() ) ;
		oXMLGenerator.AddEscapedElementAttribute ( "discarded" , (m4pcchar_t) szBuffer ) ;

		#ifdef _WINDOWS
		sprintf ( szBuffer, "%I64d" , oSiteNode.GetOLTPTime( ) ) ;
		#else
		sprintf ( szBuffer, "%lld" , oSiteNode.GetOLTPTime( ) ) ;
		#endif
		oXMLGenerator.AddEscapedElementAttribute ( "oltptime" , (m4pcchar_t) szBuffer ) ;

		#ifdef _WINDOWS
		sprintf ( szBuffer, "%I64d" , oSiteNode.GetXMLTime( ) ) ;
		#else
		sprintf ( szBuffer, "%lld" , oSiteNode.GetXMLTime( ) ) ;
		#endif
		oXMLGenerator.AddEscapedElementAttribute ( "xmltime" , (m4pcchar_t) szBuffer ) ;

		oXMLGenerator.CloseSingleClosedElement ( ) ;

	}

	oXMLGenerator.CloseElement("command");

	oXMLGenerator.CloseElement("dispatcher");


	iStreamSize = oXMLGenerator.GetXMLStreamSize ( ) ;

	iStreamSize++ ;

//	M4_MDBG_NEW( buffer, buffer = new m4char_t [iStreamSize] ) ;

    iRet = oXMLGenerator.GetXMLStream (( char *&) ao_strStream, iStreamSize );

//	ao_strStream = (void*) buffer ;

	return iRet ;
  //## end ClConfigurationsXMLAnswerGen::GenerateAnswer%963825083.body
}

// Additional Declarations
  //## begin ClConfigurationsXMLAnswerGen%3972EF770043.declarations preserve=yes
  //## end ClConfigurationsXMLAnswerGen%3972EF770043.declarations

// Class ClCommandCtlGen 





ClCommandCtlGen::ClCommandCtlGen (m4string_t ai_szCommand, m4uint32_t ai_iRequestID, m4string_t ai_strUserName, m4string_t ai_strPassword)
  //## begin ClCommandCtlGen::ClCommandCtlGen%963825085.hasinit preserve=no
  //## end ClCommandCtlGen::ClCommandCtlGen%963825085.hasinit
  //## begin ClCommandCtlGen::ClCommandCtlGen%963825085.initialization preserve=yes
  //## end ClCommandCtlGen::ClCommandCtlGen%963825085.initialization
{
  //## begin ClCommandCtlGen::ClCommandCtlGen%963825085.body preserve=yes
	m_strCommand = ai_szCommand ;
	m_iRequestID = ai_iRequestID ;
	m_strUserName = ai_strUserName ;
	m_strPassword = ai_strPassword ;
  //## end ClCommandCtlGen::ClCommandCtlGen%963825085.body
}



//## Other Operations (implementation)
m4return_t ClCommandCtlGen::GenerateAnswer (ClSiteNodeContainer &ai_oSiteNodeContainer, m4void_t *&ao_strStream, m4return_t ai_iRet, ClLogSystem *ai_poLog)
{
  //## begin ClCommandCtlGen::GenerateAnswer%963825084.body preserve=yes
	m4uint32_t  iStreamSize ;
	m4return_t iRet ;
	M4XMLStreamGenerator oXMLGenerator ("'");
	m4char_t szBuffer [100] ;
//	m4pchar_t buffer ;
	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;
	ClSiteNodeRepository * pRepository = ClSiteRepositorySingleton::GetInstance() ;
	ClSiteNodeInformation oNodeInfo ;


	if ( M4_ERROR == ai_oSiteNodeContainer.GetNextNode ( oNodeInfo ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOGETNEXTNODEINFO,
			"Error getting Server configuration XML answer. Cannot get Node information." );
		
		return M4_ERROR ;
	}

	oXMLGenerator.MakeXMLDeclaration();

	oXMLGenerator.InsertComment("@version");

	if ( M4_ERROR == gethostname (szBuffer , 100) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOXMLGETHOSTNAME,
			"Error getting Server configuration XML answer. Cannot get the Dispatcher Host Name." );
		return M4_ERROR ;
	}

	oXMLGenerator.MakeElement("dispatcher" );
	oXMLGenerator.AddEscapedElementAttribute ( "host" , (m4pcchar_t) szBuffer ); //

	sprintf ( szBuffer , "%lu" , (unsigned long)pRepository->GetAdminPort() );
	oXMLGenerator.AddEscapedElementAttribute ( "port" , (m4pcchar_t) szBuffer ); 
	
	if ( ! stricmp ( m_strCommand.c_str()  , "server_startup" ) )
	{
		sprintf ( szBuffer , "%d" , SERVER_UPDATE_SLAPSED_TIME );
		oXMLGenerator.AddEscapedElementAttribute ( "slapsedtime" , (m4pcchar_t) szBuffer ) ;
	}
	oXMLGenerator.EndElement () ; //element=dispatcher
	


	oXMLGenerator.MakeElement("command" ); 
	oXMLGenerator.AddEscapedElementAttribute ( "id" , m_strCommand.c_str()  ); 
	oXMLGenerator.EndElement () ; //element =command

	oXMLGenerator.MakeElement("params");

	
	// Parameters "servername" and "baseport" only belongs to the command
	// "server_startup"
	sprintf ( szBuffer , "%d" , m_iRequestID ) ;
	oXMLGenerator.AddEscapedElementAttribute ( "RequestID" , (m4pcchar_t) szBuffer ) ;

	if ( oNodeInfo.GetSiteNodeID( ).size () )
	{
		oXMLGenerator.AddEscapedElementAttribute ( "servername" , oNodeInfo.GetSiteNodeID( ).c_str() ) ;
	}
	if ( oNodeInfo.GetBasePort( ) )
	{
		sprintf ( szBuffer , "%d" , oNodeInfo.GetBasePort( ) );
		oXMLGenerator.AddEscapedElementAttribute ( "baseport" , (m4pcchar_t) szBuffer ) ;
	}

	oXMLGenerator.AddEscapedElementAttribute ( "user" , m_strUserName.c_str()  ) ;

	oXMLGenerator.AddEscapedElementAttribute ( "password" , m_strPassword.c_str() ) ;

/*
	if ( ! stricmp ( m_strCommand.c_str()  , "server_startup" ) ) {
	
	}
	else
	{
		oXMLGenerator.AddEscapedElementAttribute ( "result" , (m4pcchar_t) "success" ) ;
	}
*/	
	oXMLGenerator.CloseSingleClosedElement ( ) ; //elemet=params


	oXMLGenerator.CloseElement("command"); 

	oXMLGenerator.CloseElement("dispatcher");


	iStreamSize = oXMLGenerator.GetXMLStreamSize ( ) ;

	iStreamSize++ ;

//	M4_MDBG_NEW( buffer, buffer = new m4char_t [iStreamSize] ) ;

    iRet = oXMLGenerator.GetXMLStream (( char *&) ao_strStream, iStreamSize );

//	ao_strStream = (void*) buffer ;

	return iRet ;
  //## end ClCommandCtlGen::GenerateAnswer%963825084.body
}

// Additional Declarations
  //## begin ClCommandCtlGen%3972EF99039F.declarations preserve=yes
  //## end ClCommandCtlGen%3972EF99039F.declarations

// Class ClStopXMLAnswerGen 


//## Other Operations (implementation)
m4return_t ClStopXMLAnswerGen::GenerateAnswer (ClSiteNodeContainer &ai_oSiteNodeContainer, m4void_t *&ao_strStream, m4return_t ai_iRet, ClLogSystem *ai_poLog)
{
  //## begin ClStopXMLAnswerGen::GenerateAnswer%963825082.body preserve=yes
//	m4pchar_t buffer ;
	m4return_t iRet ;
	m4string_t strParameters;
	m4uint32_t  iStreamSize , iLenout , iCount ;
	ClSiteNodeInformation oNodeInfo ;
	M4XMLStreamGenerator oXMLGenerator ;
	m4string_t szRet = "SUCCESS" ;
	m4pchar_t pLogBuffer ;
	m4pchar_t pcHEXLog = NULL;
	m4int_t iHexSize ;


	if ( M4_ERROR == ai_oSiteNodeContainer.GetNextNode ( oNodeInfo ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOSITENODE,
			"Error getting site node. There is not any site node." );
		return M4_ERROR ;
	}

	
	oXMLGenerator.MakeXMLDeclaration();

	oXMLGenerator.InsertComment("@version");

	oXMLGenerator.MakeElement("dispatcher"); 
	oXMLGenerator.EndElement() ; //element=dispatcher

	oXMLGenerator.MakeElement("command") ;
	oXMLGenerator.AddEscapedElementAttribute ( "name" , (m4pcchar_t) "stopserver"); 
	oXMLGenerator.EndElement() ; //element=command

	if ( M4_SUCCESS != ai_iRet )
	{
		szRet = "ERROR" ;
	}

//	oXMLGenerator.MakeElement("status");
	oXMLGenerator.MakeElement("params");
	oXMLGenerator.AddEscapedElementAttribute ( "status" , oNodeInfo.GetNodeState().c_str() );
	oXMLGenerator.AddEscapedElementAttribute ( "result" , szRet.c_str() );

	if( ai_poLog )
	{
		iRet = ai_poLog -> GetSerialStringSizeFromMessage(iStreamSize);

		if ( ( iStreamSize> 0 ) & (iRet != M4_ERROR ) )
		{
			// La log está vacía.
	
			pLogBuffer = new m4char_t [ iStreamSize +1];
			
			M4_ASSERT ( pLogBuffer != NULL );
			
			iRet = ai_poLog -> GetSerialStringFromMessage( pLogBuffer, iStreamSize , iLenout );
			
			if ( ( iRet != M4_ERROR ) && ( iLenout ) )
			{
				for ( iCount = 0 ; iCount < iStreamSize ; iCount ++ )
				{
					if ( ! pLogBuffer [ iCount ] )
					{
						pLogBuffer [iCount] = strFilled ;
					}
				}

				pLogBuffer [iStreamSize] = 0 ;

				//Conversion from binary log serialization to hexadecimal string.

				iHexSize = (iStreamSize * 2)  + 1 ;

				pcHEXLog = new m4char_t [ iHexSize ] ;

				M4_ASSERT ( pcHEXLog != NULL ) ;

				iRet = ASCIItoHEX ( pLogBuffer , iStreamSize , pcHEXLog , iHexSize , iHexSize ) ;				

				if ( iRet == M4_SUCCESS )
				{
					pcHEXLog [ iHexSize ] = 0 ;
					oXMLGenerator.AddEscapedElementAttribute ( "log" , (m4pcchar_t) pcHEXLog );
				}

				delete [] pcHEXLog;
				
			}
			
			delete []pLogBuffer;
		}
	}
	oXMLGenerator.CloseSingleClosedElement(); //element=status

	oXMLGenerator.CloseElement("command");

	oXMLGenerator.CloseElement("dispatcher");

	iStreamSize = oXMLGenerator.GetXMLStreamSize ( ) ;

	iStreamSize++ ;

//	M4_MDBG_NEW( buffer, buffer = new m4char_t [iStreamSize] ) ;

    iRet = oXMLGenerator.GetXMLStream (( char *&) ao_strStream, iStreamSize );

//	ao_strStream = (void*) buffer ;

	return iRet ;
  //## end ClStopXMLAnswerGen::GenerateAnswer%963825082.body
}

// Additional Declarations
  //## begin ClStopXMLAnswerGen%3972EF440003.declarations preserve=yes
  //## end ClStopXMLAnswerGen%3972EF440003.declarations

m4return_t ClRemoveXMLAnswerGen::GenerateAnswer (ClSiteNodeContainer &ai_oSiteNodeContainer, m4void_t *&ao_strStream, m4return_t ai_iRet, ClLogSystem *ai_poLog)
{
	m4return_t iRet ;
	m4string_t strParameters;
	m4uint32_t  iStreamSize , iLenout , iCount;	
	ClSiteNodeInformation oNodeInfo ;
	M4XMLStreamGenerator oXMLGenerator ;
	m4string_t szRet = "SUCCESS" ;
	m4pchar_t pLogBuffer ;
	m4pchar_t pcHEXLog = NULL;
	m4int_t iHexSize ;


	if ( M4_ERROR == ai_oSiteNodeContainer.GetNextNode ( oNodeInfo ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOSITENODE,
			"Error getting site node. There is not any site node." );
		return M4_ERROR ;
	}

	oXMLGenerator.MakeXMLDeclaration();

	oXMLGenerator.InsertComment("@version");

	oXMLGenerator.MakeElement("dispatcher"); 
	oXMLGenerator.EndElement() ; //element=dispatcher

	oXMLGenerator.MakeElement("command") ;
	oXMLGenerator.AddEscapedElementAttribute ( "name" , (m4pcchar_t) "removeserver"); 
	oXMLGenerator.EndElement() ; //element=command

	if ( M4_SUCCESS != ai_iRet )
	{
		szRet = "ERROR" ;
	}

	oXMLGenerator.MakeElement("params");
	oXMLGenerator.AddEscapedElementAttribute ( "status" , oNodeInfo.GetNodeState().c_str() );
	oXMLGenerator.AddEscapedElementAttribute ( "result" , szRet.c_str() );

	if( ai_poLog )
	{
		iRet = ai_poLog -> GetSerialStringSizeFromMessage(iStreamSize);

		if ( ( iStreamSize> 0 ) & (iRet != M4_ERROR ) )
		{
			// La log está vacía.
	
			pLogBuffer = new m4char_t [ iStreamSize +1];
			
			M4_ASSERT ( pLogBuffer != NULL );
			
			iRet = ai_poLog -> GetSerialStringFromMessage( pLogBuffer, iStreamSize , iLenout );
			
			if ( ( iRet != M4_ERROR ) && ( iLenout ) )
			{
				for ( iCount = 0 ; iCount < iStreamSize ; iCount ++ )
				{
					if ( ! pLogBuffer [ iCount ] )
					{
						pLogBuffer [iCount] = strFilled ;
					}
				}

				pLogBuffer [iStreamSize] = 0 ;

				//Conversion from binary log serialization to hexadecimal string.

				iHexSize = (iStreamSize * 2)  + 1 ;

				pcHEXLog = new m4char_t [ iHexSize ] ;

				M4_ASSERT ( pcHEXLog != NULL ) ;

				iRet = ASCIItoHEX ( pLogBuffer , iStreamSize , pcHEXLog , iHexSize , iHexSize ) ;

				if ( iRet == M4_SUCCESS )
				{
					pcHEXLog [ iHexSize ] = 0 ;
					oXMLGenerator.AddEscapedElementAttribute ( "log" , (m4pcchar_t) pcHEXLog );
				}

				delete [] pcHEXLog;
			}
			
			delete []pLogBuffer;			
		}
	}

	oXMLGenerator.CloseSingleClosedElement(); //element=status

	oXMLGenerator.CloseElement("command");

	oXMLGenerator.CloseElement("dispatcher");

	iStreamSize = oXMLGenerator.GetXMLStreamSize ( ) ;

	iStreamSize++ ;

    iRet = oXMLGenerator.GetXMLStream (( char *&) ao_strStream, iStreamSize );

	return iRet ;
}


// Class ClConnectReplyGen 


//## Other Operations (implementation)
m4return_t ClConnectReplyGen::GenerateAnswer (ClSiteNodeContainer &ai_oSiteNodeContainer, m4void_t *&ao_strStream, m4return_t ai_iRet, ClLogSystem *ai_poLog)
{
  //## begin ClConnectReplyGen::GenerateAnswer%963906293.body preserve=yes
	ClPDU * outputPDU ;
	ClProtocol oProtocol ;
	ClTagList				oTagList;
	M4DataStorage	*poDataStorage;
	ClSiteNodeInformation oSiteNode ;
	m4char_t szPort [20];
	m4pchar_t	pcCandidateServer ;



	if ( M4_ERROR == ai_oSiteNodeContainer.GetNextNode ( oSiteNode ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOGETNEXTNODEINFOFORCLIENT,
			"Error making Client answer. Cannot get Node information." );

		return M4_ERROR ;
	}



	if ( ! ( outputPDU =  oProtocol.GetPDU(M4_ID_PDU_ACK) ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOGETACKPDUFORCLIENT,
			"Error making Client answer using communication protocol. Cannot make the reply." );

		return M4_ERROR ;
	}

	//	Construcción del Taglist

	// fix bug 0088880
	// gets the local host

	// bg 175581
	pcCandidateServer = (m4pchar_t) oSiteNode.GetLocalHost().c_str() ;
	if( pcCandidateServer != NULL && *pcCandidateServer != '\0' )
	{
		oTagList.SetArg ( CANDIDATE_SERVER_HOST , pcCandidateServer ) ;
	}
	else
	{
		oTagList.SetArg ( CANDIDATE_SERVER_HOST , -1 ) ;
	}
	
	sprintf ( szPort , "%d" , oSiteNode.GetBasePort ( ) ) ;

	oTagList.SetArg ( CANDIDATE_SERVER_PORT , szPort ) ;


	poDataStorage = outputPDU -> GetDataStorage(M4_TRUE);
	if(poDataStorage)
	{
		oTagList.TagList2DataStorage(poDataStorage);
	}
	
	oTagList.Clear();
	
	ao_strStream = (void *) outputPDU ;

	return M4_SUCCESS ;
  //## end ClConnectReplyGen::GenerateAnswer%963906293.body
}

// Additional Declarations
  //## begin ClConnectReplyGen%39740EA2009D.declarations preserve=yes
  //## end ClConnectReplyGen%39740EA2009D.declarations

// Class ClTargetConfigGen 


//## Other Operations (implementation)
m4return_t ClTargetConfigGen::GenerateAnswer (ClSiteNodeContainer &ai_oSiteNodeContainer, m4void_t *&ao_strStream, m4return_t ai_iRet, ClLogSystem *ai_poLog)
{
  //## begin ClTargetConfigGen::GenerateAnswer%972463864.body preserve=yes
	ClSiteNodeInformation oSiteNode ;
	M4XMLStreamGenerator oXMLGenerator ("'");
	m4char_t szBuffer [32] ;
//	m4pchar_t buffer ;
//	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;
	m4return_t iRet ;
	m4uint32_t iStreamSize  ;


	oXMLGenerator.MakeXMLDeclaration();

	oXMLGenerator.InsertComment("@version");

	oXMLGenerator.MakeElement("dispatcher"); 
	oXMLGenerator.EndElement() ;




	while ( M4_SUCCESS == ai_oSiteNodeContainer.GetNextNode ( oSiteNode ) )
	{
		if ( oSiteNode.IsPersistable ( ) )
		{
			oXMLGenerator.MakeElement("command");
			oXMLGenerator.AddEscapedElementAttribute ( "name" , (m4pcchar_t) "targetconfiguration" );
			oXMLGenerator.EndElement() ;

			oXMLGenerator.MakeElement("params" ) ;

			oXMLGenerator.AddEscapedElementAttribute ( "host" , oSiteNode.GetNodeHost ( ).c_str() ) ;

			oXMLGenerator.AddEscapedElementAttribute ( "configuration" , oSiteNode.GetSiteNodeID( ).c_str() ) ;

			sprintf ( szBuffer, "%d" , oSiteNode.GetBasePort( ) ) ;
			oXMLGenerator.AddEscapedElementAttribute ( "baseport" , (m4pcchar_t) szBuffer ) ;

			oXMLGenerator.AddEscapedElementAttribute ( "user" , oSiteNode.GetUserName( ).c_str()  ) ;

			oXMLGenerator.AddEscapedElementAttribute ( "password" , oSiteNode.GetPassword( ).c_str()  ) ;

            m4string_t isEnabled = oSiteNode.GetIsEnabled() ? "true" : "false";
			oXMLGenerator.AddEscapedElementAttribute("enabled", isEnabled.c_str());

            oXMLGenerator.CloseSingleClosedElement ( ) ;

			oXMLGenerator.CloseElement("command");
		}
	}


	oXMLGenerator.CloseElement("dispatcher");


	iStreamSize = oXMLGenerator.GetXMLStreamSize ( ) ;

	iStreamSize++ ;

//	M4_MDBG_NEW( buffer, buffer = new m4char_t [iStreamSize] ) ;


    iRet = oXMLGenerator.GetXMLStream (( char *&) ao_strStream, iStreamSize );

//	ao_strStream = (void*) buffer ;

	return iRet ;
  //## end ClTargetConfigGen::GenerateAnswer%972463864.body
}

// Additional Declarations
  //## begin ClTargetConfigGen%39F6C6940041.declarations preserve=yes
  //## end ClTargetConfigGen%39F6C6940041.declarations

// Class ClStopDpchAnswerGen 


//## Other Operations (implementation)
m4return_t ClStopDpchAnswerGen::GenerateAnswer (ClSiteNodeContainer &ai_oSiteNodeContainer, m4void_t *&ao_strStream, m4return_t ai_iRet, ClLogSystem *ai_poLog)
{
  //## begin ClStopDpchAnswerGen::GenerateAnswer%991737905.body preserve=yes
//	m4pchar_t buffer ;
	m4return_t iRet ;
	m4string_t strParameters;
	m4uint32_t  iStreamSize , iLenout , iCount ;
	ClSiteNodeInformation oNodeInfo ;
	M4XMLStreamGenerator oXMLGenerator ;
	m4string_t szRet = "SUCCESS" ;
	m4pchar_t pLogBuffer ;



	
	oXMLGenerator.MakeXMLDeclaration();

	oXMLGenerator.InsertComment("@version");

	oXMLGenerator.MakeElement("dispatcher"); 
	oXMLGenerator.EndElement() ; //element=dispatcher

	oXMLGenerator.MakeElement("command") ;
	oXMLGenerator.AddEscapedElementAttribute ( "name" , (m4pcchar_t) STOP_DPCH ); 
	oXMLGenerator.EndElement() ; //element=command

	if ( M4_SUCCESS != ai_iRet )
	{
		szRet = "ERROR" ;
	}

//	oXMLGenerator.MakeElement("status");
	oXMLGenerator.MakeElement("params");
	oXMLGenerator.AddEscapedElementAttribute ( "result" , szRet.c_str() );

	if( ai_poLog )
	{
		iRet = ai_poLog -> GetSerialStringSizeFromMessage(iStreamSize);

		if ( ( iStreamSize> 0 ) & (iRet != M4_ERROR ) )
		{
			// La log está vacía.
	
			pLogBuffer = new m4char_t [ iStreamSize +1];
			
			M4_ASSERT ( pLogBuffer != NULL );
			
			iRet = ai_poLog -> GetSerialStringFromMessage( pLogBuffer, iStreamSize , iLenout );
			
			if ( ( iRet != M4_ERROR ) && ( iLenout ) )
			{
				for ( iCount = 0 ; iCount < iStreamSize ; iCount ++ )
				{
					if ( ! pLogBuffer [ iCount ] )
					{
						pLogBuffer [iCount] = strFilled ;
					}
				}

				pLogBuffer [iStreamSize] = 0 ;

				oXMLGenerator.AddEscapedElementAttribute ( "log" , (m4pcchar_t) pLogBuffer );
			}
			
			delete []pLogBuffer;
		}
	}
	oXMLGenerator.CloseSingleClosedElement(); //element=status

	oXMLGenerator.CloseElement("command");

	oXMLGenerator.CloseElement("dispatcher");

	iStreamSize = oXMLGenerator.GetXMLStreamSize ( ) ;

	iStreamSize++ ;

//	M4_MDBG_NEW( buffer, buffer = new m4char_t [iStreamSize] ) ;

    iRet = oXMLGenerator.GetXMLStream (( char *&) ao_strStream, iStreamSize );

//	ao_strStream = (void*) buffer ;

	return iRet ;
  //## end ClStopDpchAnswerGen::GenerateAnswer%991737905.body
}

// Additional Declarations
  //## begin ClStopDpchAnswerGen%3B1CB7DC03CF.declarations preserve=yes
  //## end ClStopDpchAnswerGen%3B1CB7DC03CF.declarations

//## begin module%393CFB8C016F.epilog preserve=yes
//## end module%393CFB8C016F.epilog

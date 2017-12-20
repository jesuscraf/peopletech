//## begin module%39479DF80327.cm preserve=no
//## end module%39479DF80327.cm

//## begin module%39479DF80327.cp preserve=no
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
//## end module%39479DF80327.cp

//## Module: dspagent%39479DF80327; Package body
//## Subsystem: M4Dispatcher::src%3923BBFB0332
//## Source file: z:\m4dispatcher\src\dspagent.cpp

//## begin module%39479DF80327.additionalIncludes preserve=no
//## end module%39479DF80327.additionalIncludes

//## begin module%39479DF80327.includes preserve=yes
#include <dspactions.hpp>
#include <dspparams.hpp>
#include <dspinvoker.hpp>
#include <actionidentifiers.hpp>
#include <saxactioncaller.hpp>
#include <m4xmlstreamgen.hpp>
#include <dspxmlreq.hpp>
#include <basiclog.hpp>
#include <m4dpchres.hpp>
#include <sitenodeinfo.hpp>

#include <m4memdbg.h>
#include <m4srvtrace.h>

#define M4_SRV_TRACE_COMP_ID "DPCH"

#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
//## end module%39479DF80327.includes

// dspagent
#include <dspagent.hpp>
// clbaseaction
#include <clbaseaction.hpp>

class ClDspExecutiveAgent;

//## begin module%39479DF80327.declarations preserve=no
//## end module%39479DF80327.declarations

//## begin module%39479DF80327.additionalDeclarations preserve=yes
//## end module%39479DF80327.additionalDeclarations


// Class ClDspAgent 

//## begin ClDspAgent::m_iControlPort%3BA6077A002C.attr preserve=no  protected: static m4uint32_t {UA} 
m4uint32_t ClDspAgent::m_iControlPort;
//## end ClDspAgent::m_iControlPort%3BA6077A002C.attr

//## begin ClDspAgent::m_iAdminPort%3BA6077A0054.attr preserve=no  protected: static m4uint32_t {UA} 
m4uint32_t ClDspAgent::m_iAdminPort;
//## end ClDspAgent::m_iAdminPort%3BA6077A0054.attr

//## begin ClDspAgent::m_iClientSSLPort%3BA6077A009A.attr preserve=no  protected: static m4uint32_t {UA} 
m4uint32_t ClDspAgent::m_iClientSSLPort;
//## end ClDspAgent::m_iClientSSLPort%3BA6077A009A.attr

//## begin ClDspAgent::m_iClientPort%3BA6077A0108.attr preserve=no  protected: static m4uint32_t {UA} 
m4uint32_t ClDspAgent::m_iClientPort;
//## end ClDspAgent::m_iClientPort%3BA6077A0108.attr

//## begin ClDspAgent::m_iMaxStartRetries%3BA7094202A0.attr preserve=no  protected: static m4uint32_t {UA} 
m4uint32_t ClDspAgent::m_iMaxStartRetries;
//## end ClDspAgent::m_iMaxStartRetries%3BA7094202A0.attr

//## begin ClDspAgent::m_iServerStartTimeout%3FE1F187012B.attr preserve=no  protected: static m4uint32_t {UA} 
m4uint32_t ClDspAgent::m_iServerStartTimeout;
//## end ClDspAgent::m_iServerStartTimeout%3FE1F187012B.attr

m4uint32_t ClDspAgent::m_iDumpTrace;

m4uint32_t ClDspAgent::m_iSessionsWeight;
m4uint32_t ClDspAgent::m_iSrvMemWeight;
m4uint32_t ClDspAgent::m_iSysMemWeight;
m4uint32_t ClDspAgent::m_iCPUWeight;
m4uint32_t ClDspAgent::m_iExecTimeWeight;
m4uint32_t ClDspAgent::m_iExecTimeThreshold;

ClDspAgent::ClDspAgent (ClSiteNodeRepository *ai_poServerRepository, ClDspExecutiveAgent *ai_poExecutiveAgent)
  //## begin ClDspAgent::ClDspAgent%963314061.hasinit preserve=no
  //## end ClDspAgent::ClDspAgent%963314061.hasinit
  //## begin ClDspAgent::ClDspAgent%963314061.initialization preserve=yes
  //## end ClDspAgent::ClDspAgent%963314061.initialization
{
  //## begin ClDspAgent::ClDspAgent%963314061.body preserve=yes
	m_poServerRepository = ai_poServerRepository ;
	m_poExecutiveAgent  = ai_poExecutiveAgent ;
	m_iClientPort = 0;
	m_iClientSSLPort = 0 ;
	m_iAdminPort = 0;
	m_iControlPort = 0;
  //## end ClDspAgent::ClDspAgent%963314061.body
}



//## Other Operations (implementation)
m4return_t ClDspAgent::ParseXML (ClParamList *ai_poParamList)
{
  //## begin ClDspAgent::ParseXML%963314075.body preserve=yes
	ClDspXMLRequest * poRequest;
	m4pchar_t pXML ;
	m4uint32_t iBufferLen;
	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;


	if ( M4_ERROR == ai_poParamList -> GetPointer ( "REQUEST" , (void*&)poRequest ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOXMLREQUEST,
			"Error parsing XML request. Cannot get the XML request." );
		
		return M4_ERROR ;
	}

	if (! ( pXML = (m4pchar_t)  poRequest  -> GetBufferIn ( ) ) )
	{
		return M4_ERROR ;
	}

	ClParamList * poParamList = NULL;

	M4_MDBG_NEW(  poParamList, poParamList = new ClParamList ) ;

	poParamList -> SetPointer ( "REQUEST" , (void *) poRequest ) ;

	iBufferLen = strlen ( pXML ) +1 ;

	SAXParser parser;
	ClSAXHandlerActionCaller  oDspHandlerParser ( poInvoker , poParamList ) ;

    parser.setDocumentHandler(&oDspHandlerParser);
    parser.setErrorHandler(&oDspHandlerParser);

   
	MemBufInputSource * poMemBuffer = NULL ;

	M4_MDBG_NEW(  poMemBuffer, poMemBuffer =  new 
		MemBufInputSource((const XMLByte *) pXML, iBufferLen, "generic", false) );

    
	try
    {
        parser.parse(  *poMemBuffer ) ;
    }

    catch (const XMLException& /*toCatch*/)
    {
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOINITIALIZEDPSAGENTXMLPARSE,
			"Error initializing Dispatcher Agent. Cannot initialize the XML parse." );					

	    M4_MDBG_DELETE(poMemBuffer, delete poMemBuffer);
        return M4_ERROR ;
    }

	if ( ! oDspHandlerParser.IsOk() ) {
		M4_SRV_ERROR_N(0, "Error parsing XML" );	//Ojo, en el XML viene el password abierto					
		M4_SRV_DEBUG("Error parsing XML %0:s", pXML );					
		M4_MDBG_DELETE(  poMemBuffer, delete poMemBuffer ) ;
		return M4_ERROR ;

	} ;

	M4_MDBG_DELETE(  poMemBuffer, delete poMemBuffer ) ;

	return M4_SUCCESS ; 

  //## end ClDspAgent::ParseXML%963314075.body
}

m4return_t ClDspAgent::SetParams (m4uint32_t ai_iBasePort, m4uint32_t ai_iMaxStartRetries, m4uint32_t ai_iTimeout, m4uint32_t  ai_iDumpTrace, m4uint32_t ai_iSessionsWeight, m4uint32_t ai_iSrvMemWeight, m4uint32_t ai_iSysMemWeight, m4uint32_t ai_iCPUWeight, m4uint32_t ai_iExecTimeWeight, m4uint32_t ai_iExecTimeThreshold)
{
  //## begin ClDspAgent::SetParams%1000738437.body preserve=yes

	m_iMaxStartRetries = ai_iMaxStartRetries ;
	m_iServerStartTimeout = ai_iTimeout  ;
	m_iDumpTrace = ai_iDumpTrace ;
	m_iSessionsWeight = ai_iSessionsWeight;
	m_iSrvMemWeight = ai_iSrvMemWeight;
	m_iSysMemWeight = ai_iSysMemWeight;
	m_iCPUWeight = ai_iCPUWeight;
	m_iExecTimeWeight = ai_iExecTimeWeight;
	m_iExecTimeThreshold = ai_iExecTimeThreshold;
	
	if (! ai_iBasePort )
	{
		return M4_ERROR ;
	}

	m_iClientPort = ai_iBasePort;
	m_iClientSSLPort = ai_iBasePort + 1;
	m_iAdminPort = ai_iBasePort + 2;
	m_iControlPort = ai_iBasePort + 3;

	return M4_SUCCESS ;
  //## end ClDspAgent::SetParams%1000738437.body
}

// Additional Declarations
  //## begin ClDspAgent%396B587301FF.declarations preserve=yes
  //## end ClDspAgent%396B587301FF.declarations

//## begin module%39479DF80327.epilog preserve=yes
//## end module%39479DF80327.epilog

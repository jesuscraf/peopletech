//## begin module%396C4E39026E.cm preserve=no
//## end module%396C4E39026E.cm

//## begin module%396C4E39026E.cp preserve=no
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
//## end module%396C4E39026E.cp

//## Module: dspclientreqpdu%396C4E39026E; Package body
//## Subsystem: M4Dispatcher::src%3923BBFB0332
//## Source file: C:\WorkingFolder\m4dispatcher\src\dspclientreqpdu.cpp

//## begin module%396C4E39026E.additionalIncludes preserve=no
//## end module%396C4E39026E.additionalIncludes

//## begin module%396C4E39026E.includes preserve=yes
#include <pdus.hpp>
#include <protocol.hpp>
#include <clienttag.hpp>
#include <sitenodeinfo.hpp>
#include <dspanswergen.hpp>
#include <basiclog.hpp>
#include <m4dpchres.hpp>

#include <m4memdbg.h>
#include <m4srvtrace.h>

#define M4_SRV_TRACE_COMP_ID "DPCH"

//## end module%396C4E39026E.includes

// dspclientreqpdu
#include <dspclientreqpdu.hpp>
// dspreq
#include <dspreq.hpp>

#include <m4props.hpp>

//## begin module%396C4E39026E.declarations preserve=no
//## end module%396C4E39026E.declarations

//## begin module%396C4E39026E.additionalDeclarations preserve=yes
//## end module%396C4E39026E.additionalDeclarations


// Class ClDspClientRequestPDU 

ClDspClientRequestPDU::ClDspClientRequestPDU (void *ai_poBuffer, m4uint64_t ai_iRequestID, m4uint32_t ai_iConnectionID)
  //## begin ClDspClientRequestPDU::ClDspClientRequestPDU%963388999.hasinit preserve=no
  //## end ClDspClientRequestPDU::ClDspClientRequestPDU%963388999.hasinit
  //## begin ClDspClientRequestPDU::ClDspClientRequestPDU%963388999.initialization preserve=yes
  :ClDspRequest ( ai_poBuffer , ai_iRequestID, ai_iConnectionID)
  //## end ClDspClientRequestPDU::ClDspClientRequestPDU%963388999.initialization
{
  //## begin ClDspClientRequestPDU::ClDspClientRequestPDU%963388999.body preserve=yes
  //## end ClDspClientRequestPDU::ClDspClientRequestPDU%963388999.body
}


ClDspClientRequestPDU::~ClDspClientRequestPDU ()
{
  //## begin ClDspClientRequestPDU::~ClDspClientRequestPDU%963989738.body preserve=yes
	if ( m_poBufferIn )
		M4_MDBG_DELETE( (ClPDU*) m_poBufferIn, delete ( (ClPDU*) m_poBufferIn ) ) ;
	if ( m_poBufferOut ) 
		M4_MDBG_DELETE( (ClPDU*) m_poBufferOut, delete ( (ClPDU*) m_poBufferOut ) ) ;

  //## end ClDspClientRequestPDU::~ClDspClientRequestPDU%963989738.body
}



//## Other Operations (implementation)
m4return_t ClDspClientRequestPDU::BuildReqReply (ClSiteNodeContainer &ai_oSiteNodeContainer, ClDspAnswerGen *ai_poAnswerGen)
{
  //## begin ClDspClientRequestPDU::BuildReqReply%963825076.body preserve=yes
	if ( M4_ERROR == ai_poAnswerGen ->GenerateAnswer ( ai_oSiteNodeContainer , m_poBufferOut ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOBUILDREQUESTREPLY,
			"Error making request reply. Cannot make the reply." );

		return M4_ERROR ;
	}

    // get the input and output pdu.
    ClConnectPDU *pduConnect = (ClConnectPDU *)m_poBufferIn;
    ClAckPDU* pduAck = (ClAckPDU*)m_poBufferOut;

    // set the request id.
    pduAck->SetRequestId(pduConnect->GetRequestId()); 

    // look for M4_CLOSE_CONNECTION tag in input pdu, and copy it to output pdu.
    M4DataStorage* ds = pduConnect->GetDataStorage();
    ClTagList tl;
    tl.DataStorage2TagList(ds);
    
    char* tagValue = new char[64];
    if (M4_SUCCESS == tl.GetArg("M4_CLOSE_CONNECTION", tagValue, 64))
    {
        if (!strcmp(tagValue, "0"))
        {
            // creates a new taglist with this argument.
            ClTagList newTagList;
            newTagList.SetArg ("M4_CLOSE_CONNECTION", "0");

            // insert tag in output pdu.
            ds = pduAck->GetDataStorage();
            newTagList.TagList2DataStorage(ds);
        }
    }
    delete tagValue;

	return 	M4_SUCCESS ;
  //## end ClDspClientRequestPDU::BuildReqReply%963825076.body
}


m4return_t ClDspClientRequestPDU::GetClientType( eDspClientTypes& ao_iClientType )
{
	ClConnectPDU*	pConnectPDU;
	M4DataStorage*	pDataStorage;
	ClTagList		oTagList;
	m4ServerVT		vArg;


	ao_iClientType = CPP_CLIENT;


	pConnectPDU	 = ( ClConnectPDU* ) m_poBufferIn;
	pDataStorage = pConnectPDU->GetDataStorage();

	oTagList.DataStorage2TagList( pDataStorage );

    if( M4_SUCCESS == oTagList.GetArg( M4_SESS_TCLOGIN, vArg ) )
    {
		ao_iClientType = JAVA_CLIENT;
    }


	return M4_SUCCESS;
}

// Additional Declarations
  //## begin ClDspClientRequestPDU%396C494402E2.declarations preserve=yes
  //## end ClDspClientRequestPDU%396C494402E2.declarations

//## begin module%396C4E39026E.epilog preserve=yes
//## end module%396C4E39026E.epilog

//## begin module%344219BF03B7.cm preserve=no
//## end module%344219BF03B7.cm

//## begin module%344219BF03B7.cp preserve=no
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
//## end module%344219BF03B7.cp

//## Module: Protocol%344219BF03B7; Package body
//## Subsystem: M4PDU::src%3794593C0399
//## Source file: e:\mybuild\m4pdus\src\protocol.cpp

//## begin module%344219BF03B7.additionalIncludes preserve=no
//## end module%344219BF03B7.additionalIncludes

//## begin module%344219BF03B7.includes preserve=yes
#ifdef _WINDOWS
#include <windows.h>
#elif defined(_UNIX)
#include <sys/types.h>
#include <netinet/in.h>
#endif
//## end module%344219BF03B7.includes

// m4types
#include <m4types.hpp>
// Protocol
#include <protocol.hpp>

class ClPDU;

//## begin module%344219BF03B7.declarations preserve=no
//## end module%344219BF03B7.declarations

//## begin module%344219BF03B7.additionalDeclarations preserve=yes
#include <time.h>
#include <m4trace.hpp>
// No funciona el bodyreference de Rose
#include <servicepdus.hpp>
#include <controlpdus.hpp>
//## end module%344219BF03B7.additionalDeclarations


// Class ClProtocol 



ClProtocol::ClProtocol ()
  //## begin ClProtocol::ClProtocol%875692547.hasinit preserve=no
      : m_iVersion(M4_PDU_40_VERSION), m_pPDU(NULL)
  //## end ClProtocol::ClProtocol%875692547.hasinit
  //## begin ClProtocol::ClProtocol%875692547.initialization preserve=yes
  //## end ClProtocol::ClProtocol%875692547.initialization
{
  //## begin ClProtocol::ClProtocol%875692547.body preserve=yes
  //## end ClProtocol::ClProtocol%875692547.body
}



//## Other Operations (implementation)
ClPDU * ClProtocol::GetPDU (m4pchar_t ai_pBlock)
{
  //## begin ClProtocol::GetPDU%875544566.body preserve=yes
	ClAdvNetHost	onh;
	if ( ai_pBlock == NULL )
		return NULL;
	
	m4uint16_t	uiIdn = * ( ( m4uint16_t * ) &ai_pBlock[0] );
    m4uint16_t	uiIdh = onh.m4ntohui( uiIdn );
	ClPDU *thepdu=GetPDU(uiIdh,M4_TRUE);
	if(NULL==thepdu)
	{m4Trace(fprintf(stderr,"%d %d->%d->%d\n",*ai_pBlock,*(ai_pBlock+1),uiIdn,uiIdh));}
	return thepdu;
  //## end ClProtocol::GetPDU%875544566.body
}

m4int16_t ClProtocol::GetHeadSize ()
{
  //## begin ClProtocol::GetHeadSize%875692546.body preserve=yes
	return M4_PDU_IDPDU_SIZE;	
  //## end ClProtocol::GetHeadSize%875692546.body
}

ClPDU * ClProtocol::GetPDU (m4uint16_t ai_uiPDUId, m4bool_t ai_bExact)
{
  //## begin ClProtocol::GetPDU%877013199.body preserve=yes
	ClPDU * pPDU = NULL;

	switch ( ai_uiPDUId )
	{
	case M4_ID_PDU_CONNECT:
		pPDU = new ClConnectPDU( );
		break;
	case M4_ID_PDU_AUTHENTICATION:
		pPDU = new ClAuthenticationPDU( );
		break;
	case M4_ID_PDU_REQUEST:
		pPDU = new ClRequestPDU( );
		break;
	case M4_ID_PDU_REPLY:
		pPDU = new ClReplyPDU( );
		break;
	case M4_ID_PDU_DISCONNECT:
		if(ai_bExact || (m_iVersion<M4_PDU_40_VERSION))
			pPDU = new ClDisconnectPDU( );
		else
			pPDU = new ClNewDisconnectPDU( );
		break;
	case M4_ID_PDU_SETROLE:
		if(ai_bExact || (m_iVersion<M4_PDU_40_VERSION))
			pPDU = new ClSetRolePDU( );
		else
			pPDU = new ClNewSetRolePDU( );
		break;	
	case M4_ID_PDU_ACK:
		pPDU = new ClAckPDU( );
		break;
	case M4_ID_PDU_ERROR:
		if(ai_bExact || (m_iVersion<M4_PDU_40_VERSION))
			pPDU = new ClErrorPDU( );
		else
			pPDU = new ClNewErrorPDU( );
		break;
	case M4_ID_PDU_CHANGEPASSWORD:
		if(ai_bExact || (m_iVersion<M4_PDU_40_VERSION))
			pPDU = new ClChangePasswordPDU( );
		else
			pPDU = new ClNewChangePasswordPDU( );
		break;
	case M4_ID_PDU_CANCEL:
		if(ai_bExact || (m_iVersion<M4_PDU_40_VERSION))
			pPDU = new ClCancelPDU( );
		else
			pPDU = new ClNewCancelPDU( );
		break;
	case M4_ID_PDU_NEWSETROLE:
			pPDU = new ClNewSetRolePDU( );
		break;	
	case M4_ID_PDU_NEWCHANGEPASSWORD:
			pPDU = new ClNewChangePasswordPDU( );
		break;
	case M4_ID_PDU_NEWCANCEL: 
			pPDU = new ClNewCancelPDU( );
		break;
	case M4_ID_PDU_NEWERROR:
			pPDU = new ClNewErrorPDU( );
		break;
	case M4_ID_PDU_NEWDISCONNECT:
			pPDU = new ClNewDisconnectPDU( );
		break;
	case M4_ID_PDU_PING:
			pPDU = new ClPingPDU( );
		break;
	default:
		m4Trace(cerr<< "Unkown PDU type:"<< ai_uiPDUId<<endl);
		return NULL;		
	}
	return pPDU;
  //## end ClProtocol::GetPDU%877013199.body
}

m4return_t ClProtocol::SetVersion (m4int_t ai_iVersion)
{
  //## begin ClProtocol::SetVersion%965046762.body preserve=yes
	m_iVersion=ai_iVersion;
	return M4_SUCCESS;
  //## end ClProtocol::SetVersion%965046762.body
}

m4uint_t ClProtocol::GetVersion ()
{
  //## begin ClProtocol::GetVersion%965046763.body preserve=yes
	return m_iVersion;
  //## end ClProtocol::GetVersion%965046763.body
}

//## begin module%344219BF03B7.epilog preserve=yes
//## end module%344219BF03B7.epilog

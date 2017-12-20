//## begin module%353C937201F9.cm preserve=no
//## end module%353C937201F9.cm

//## begin module%353C937201F9.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            Protocol
//	 File:              BaseRequest.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:				980508
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
//## end module%353C937201F9.cp

//## Module: BaseRequest%353C937201F9; Package body
//## Subsystem: M4ServerBase::src%379431BE02D0
//## Source file: F:\integration\m4serverbase\src\baserequest.cpp

//## begin module%353C937201F9.additionalIncludes preserve=no
//## end module%353C937201F9.additionalIncludes

//## begin module%353C937201F9.includes preserve=yes
#include <basepdus.hpp>
#include <servicepdus.hpp>
//## end module%353C937201F9.includes

// clbaseobject
#include <clbaseobject.hpp>
// BaseRequest
#include <baserequest.hpp>
// ImsgContainer
#include <imsgcontainer.hpp>
// clbaseaction
#include <clbaseaction.hpp>


//## begin module%353C937201F9.declarations preserve=no
//## end module%353C937201F9.declarations

//## begin module%353C937201F9.additionalDeclarations preserve=yes
//## end module%353C937201F9.additionalDeclarations


// Class ClBaseRequest 







ClBaseRequest::ClBaseRequest (m4uint32_t ai_uiReqType, m4handle_t ai_uiReqId, ClStatistic *ai_poStatistic)
  //## begin ClBaseRequest::ClBaseRequest%882359169.hasinit preserve=no
      : m_uiRequestID(ai_uiReqId), m_uiPriority(0), m_uiRequestType(ai_uiReqType)
  //## end ClBaseRequest::ClBaseRequest%882359169.hasinit
  //## begin ClBaseRequest::ClBaseRequest%882359169.initialization preserve=yes
	, ClBaseObject(ai_poStatistic), ClBaseAction ( NULL )
  //## end ClBaseRequest::ClBaseRequest%882359169.initialization
{
  //## begin ClBaseRequest::ClBaseRequest%882359169.body preserve=yes
	m_lType = REQUEST_OBJECT_TYPE  ;
  //## end ClBaseRequest::ClBaseRequest%882359169.body
}


ClBaseRequest::~ClBaseRequest ()
{
  //## begin ClBaseRequest::~ClBaseRequest%882359170.body preserve=yes
  //## end ClBaseRequest::~ClBaseRequest%882359170.body
}



//## Other Operations (implementation)
m4return_t ClBaseRequest::SetUniqueRequestID (m4string_t& ai_strServerHost, m4string_t& ai_strServerBasePort, m4string_t& ai_strServerSessionID, m4string_t& ai_strUSerID)
{
  //## begin ClBaseRequest::SetUniqueRequestID%986815273.body preserve=yes
	m4string_t separator = "/" ;
	m4handle_t iRequestID ;
	m4char_t reqid [20] ;

	m_strUniqueId = ai_strServerHost  + separator + ai_strServerBasePort + separator + ai_strServerSessionID + separator + ai_strUSerID + separator;

	iRequestID = GetRequestId () ;

	sprintf ( reqid , M4_PRIdPTR, iRequestID ) ;

	m_strUniqueId+= reqid ;


	return M4_SUCCESS ;
  //## end ClBaseRequest::SetUniqueRequestID%986815273.body
}

m4return_t ClBaseRequest::GetUniqueRequestID (m4string_t &ao_strUniqueRequestID)
{
  //## begin ClBaseRequest::GetUniqueRequestID%986815278.body preserve=yes
	if ( ! m_strUniqueId.empty() )
	{
		ao_strUniqueRequestID  = m_strUniqueId ;
		return M4_SUCCESS ;
	}

	return M4_ERROR ;

  //## end ClBaseRequest::GetUniqueRequestID%986815278.body
}

ClMessageContainer * ClBaseRequest::GetMsgContainer ()
{
  //## begin ClBaseRequest::GetMsgContainer%986914178.body preserve=yes
	return m_poMsgContainer ;
  //## end ClBaseRequest::GetMsgContainer%986914178.body
}

// Additional Declarations
  //## begin ClBaseRequest%3497BDCA02A5.declarations preserve=yes
  //## end ClBaseRequest%3497BDCA02A5.declarations

// Class ClServiceRequest 






ClServiceRequest::ClServiceRequest (m4uint32_t ai_uiReqType, m4pchar_t ai_szServiceId, m4handle_t ai_uiReqId, ClStatistic *ai_poStatistic)
  //## begin ClServiceRequest::ClServiceRequest%894621716.hasinit preserve=no
      : m_bOwnerOutputPDU(M4_TRUE), m_bOwnerInputPDU(M4_TRUE)
  //## end ClServiceRequest::ClServiceRequest%894621716.hasinit
  //## begin ClServiceRequest::ClServiceRequest%894621716.initialization preserve=yes
  ,	ClBaseRequest (ai_uiReqType, ai_uiReqId, ai_poStatistic)
  //## end ClServiceRequest::ClServiceRequest%894621716.initialization
{
  //## begin ClServiceRequest::ClServiceRequest%894621716.body preserve=yes
	m_pInputPDU = NULL;
	m_pOutputPDU = NULL;

	if ( NULL == ai_szServiceId )
	{
		m_szServiceId = NULL ;
	}
	else
	{
		m_szServiceId = new m4char_t [ strlen (ai_szServiceId) +1 ] ;

		strcpy ( m_szServiceId , ai_szServiceId ) ;
	}
  //## end ClServiceRequest::ClServiceRequest%894621716.body
}


ClServiceRequest::~ClServiceRequest ()
{
  //## begin ClServiceRequest::~ClServiceRequest%902248390.body preserve=yes
	if ( m_pInputPDU  && m_bOwnerInputPDU )
	{
		delete m_pInputPDU;

		m_pInputPDU = NULL ;
	}

	if ( m_pOutputPDU && m_bOwnerOutputPDU )
	{
		delete m_pOutputPDU;

		m_pOutputPDU  = NULL ;
	}

//	ADD	EMN	981028	Memory Leak
	if ( NULL != m_szServiceId )
	{
		delete []m_szServiceId ;
		m_szServiceId = NULL ;
	}
//	END	ADD


  //## end ClServiceRequest::~ClServiceRequest%902248390.body
}



//## Other Operations (implementation)
void ClServiceRequest::SetPDUInput (ClPDU *ai_pPDU, m4bool_t ai_bPDUOwner)
{
  //## begin ClServiceRequest::SetPDUInput%894621717.body preserve=yes

	if ( m_pInputPDU != NULL )
	{
		if ( m_bOwnerInputPDU )
		{
			delete m_pInputPDU;
			m_pInputPDU = NULL;
		}
		else
		{
			m_pInputPDU = NULL;
		}
	}
	m_bOwnerInputPDU = ai_bPDUOwner;
	m_pInputPDU = ai_pPDU;
  //## end ClServiceRequest::SetPDUInput%894621717.body
}

void ClServiceRequest::SetPDUOutput (ClPDU *ai_pPDU, m4bool_t ai_bPDUOwner)
{
  //## begin ClServiceRequest::SetPDUOutput%894621718.body preserve=yes
	if ( m_pOutputPDU != NULL  )
	{
		if ( m_bOwnerOutputPDU )
		{
			delete m_pOutputPDU;
			m_pOutputPDU = NULL;
		}
		else
		{
			m_pOutputPDU = NULL;
		}
	}
	m_bOwnerOutputPDU = ai_bPDUOwner;
	m_pOutputPDU = ai_pPDU;

	m_pOutputPDU -> SetRequestId ( m_pInputPDU -> GetRequestId () ) ;

//	ADD	EMN	980513
	m_pOutputPDU->Inicialize(m_pInputPDU);

  //## end ClServiceRequest::SetPDUOutput%894621718.body
}

m4pchar_t ClServiceRequest::GetServiceId ()
{
  //## begin ClServiceRequest::GetServiceId%894621719.body preserve=yes
// MOD EPV 980324		if ( m_pInputPDU == NULL )
	m4pchar_t pService = NULL ;


	if (( m_pInputPDU == NULL ) || ((m_pInputPDU -> GetIdPDU()) != M4_ID_PDU_REQUEST))
	{
		return 0;
	}
	( ( ClRequestPDU * ) m_pInputPDU ) -> GetServiceId(pService) ;

	return pService ;
  //## end ClServiceRequest::GetServiceId%894621719.body
}

ClPDU * ClServiceRequest::GetInputPDU (m4bool_t ai_bPDUOwner)
{
  //## begin ClServiceRequest::GetInputPDU%894621720.body preserve=yes
	m_bOwnerInputPDU = ai_bPDUOwner;
	return m_pInputPDU;
  //## end ClServiceRequest::GetInputPDU%894621720.body
}

ClPDU * ClServiceRequest::GetOutputPDU (m4bool_t ai_bPDUOwner)
{
  //## begin ClServiceRequest::GetOutputPDU%894621721.body preserve=yes
	m_bOwnerOutputPDU = ai_bPDUOwner;
	return m_pOutputPDU;
  //## end ClServiceRequest::GetOutputPDU%894621721.body
}

void ClServiceRequest::SetServiceId (m4pchar_t ai_szServiceId)
{
  //## begin ClServiceRequest::SetServiceId%894621722.body preserve=yes
//	ADD	EMN	981028	Memory Leak
	if (NULL != ai_szServiceId)
	{
		if ( m_szServiceId )
			delete m_szServiceId ;

		m_szServiceId = new m4char_t [ strlen ( ai_szServiceId ) + 1] ;

		strcpy ( m_szServiceId , ai_szServiceId  ) ;
	}

  //## end ClServiceRequest::SetServiceId%894621722.body
}

// Additional Declarations
  //## begin ClServiceRequest%35531DC803A3.declarations preserve=yes
  //## end ClServiceRequest%35531DC803A3.declarations

// Class ClControlRequest 


ClControlRequest::ClControlRequest (m4uint32_t ai_uiReqType, m4handle_t ai_uiReqId, ClStatistic *ai_poStatistic)
  //## begin ClControlRequest::ClControlRequest%896086688.hasinit preserve=no
      : m_hObject(0)
  //## end ClControlRequest::ClControlRequest%896086688.hasinit
  //## begin ClControlRequest::ClControlRequest%896086688.initialization preserve=yes
	, ClBaseRequest(ai_uiReqType, ai_uiReqId, ai_poStatistic)
  //## end ClControlRequest::ClControlRequest%896086688.initialization
{
  //## begin ClControlRequest::ClControlRequest%896086688.body preserve=yes
  //## end ClControlRequest::ClControlRequest%896086688.body
}



//## Other Operations (implementation)
void ClControlRequest::Reset ()
{
  //## begin ClControlRequest::Reset%896086689.body preserve=yes
	m_hObject = 0;
  //## end ClControlRequest::Reset%896086689.body
}

m4return_t ClControlRequest::Recycle ()
{
  //## begin ClControlRequest::Recycle%941708201.body preserve=yes
	m_hObject=0;
	m_rRet=M4_ERROR;
	return M4_SUCCESS ;
  //## end ClControlRequest::Recycle%941708201.body
}

m4bool_t ClControlRequest::CanRecycle ()
{
  //## begin ClControlRequest::CanRecycle%941708202.body preserve=yes
	return M4_TRUE;
  //## end ClControlRequest::CanRecycle%941708202.body
}

m4return_t ClControlRequest::Destroy ()
{
  //## begin ClControlRequest::Destroy%941708203.body preserve=yes
	delete this ;
	return M4_SUCCESS;
  //## end ClControlRequest::Destroy%941708203.body
}

m4bool_t ClControlRequest::CanDestroy ()
{
  //## begin ClControlRequest::CanDestroy%941708204.body preserve=yes
	return M4_TRUE ;
  //## end ClControlRequest::CanDestroy%941708204.body
}

// Additional Declarations
  //## begin ClControlRequest%3569407E003D.declarations preserve=yes
  //## end ClControlRequest%3569407E003D.declarations

//## begin module%353C937201F9.epilog preserve=yes
//## end module%353C937201F9.epilog

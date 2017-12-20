//## begin module%38D1FD09021F.cm preserve=no
//## end module%38D1FD09021F.cm

//## begin module%38D1FD09021F.cp preserve=no
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
//## end module%38D1FD09021F.cp

//## Module: TransportLayerBase%38D1FD09021F; Package body
//## Subsystem: m4transport::M4CTL::src%38D1FCC40071
//## Source file: F:\integration\m4transport\src\transportlayerbase.cpp

//## begin module%38D1FD09021F.additionalIncludes preserve=no
//## end module%38D1FD09021F.additionalIncludes

//## begin module%38D1FD09021F.includes preserve=yes
//## end module%38D1FD09021F.includes

// CommComun
#include <commcomun.hpp>
// TransportLayerBase
#include <transportlayerbase.hpp>
// m4types
#include <m4types.hpp>
// tsap
#include <tsap.hpp>
// stdio
#include <stdio.h>
// FastQ
#include <fastfifoq.hpp>
// garbageableinterface
#include <garbageableinterface.hpp>

class ClTransportLayerContainer;

//## begin module%38D1FD09021F.declarations preserve=no
//## end module%38D1FD09021F.declarations

//## begin module%38D1FD09021F.additionalDeclarations preserve=yes
#include "transportlayercontainer.hpp"
#include "m4trace.hpp"
m4uint32_t g_iTSAPId = 2 ;

//## end module%38D1FD09021F.additionalDeclarations


// Class ClTransportAction 



ClTransportAction::ClTransportAction (int ai_iType, ClTSAP *ai_pTSAP, ClIOEventHandler *ai_pIOEventHandler)
  //## begin ClTransportAction::ClTransportAction%954835574.hasinit preserve=no
  //## end ClTransportAction::ClTransportAction%954835574.hasinit
  //## begin ClTransportAction::ClTransportAction%954835574.initialization preserve=yes
  :m_pTSAP(ai_pTSAP), m_pIOEventHandler(ai_pIOEventHandler)
  //## end ClTransportAction::ClTransportAction%954835574.initialization
{
  //## begin ClTransportAction::ClTransportAction%954835574.body preserve=yes
	m_lType = ai_iType;
  //## end ClTransportAction::ClTransportAction%954835574.body
}

ClTransportAction::ClTransportAction (int ai_iType)
  //## begin ClTransportAction::ClTransportAction%961667113.hasinit preserve=no
  //## end ClTransportAction::ClTransportAction%961667113.hasinit
  //## begin ClTransportAction::ClTransportAction%961667113.initialization preserve=yes
  //## end ClTransportAction::ClTransportAction%961667113.initialization
{
  //## begin ClTransportAction::ClTransportAction%961667113.body preserve=yes
	m_lType = ai_iType;

  //## end ClTransportAction::ClTransportAction%961667113.body
}



//## Other Operations (implementation)
m4return_t ClTransportAction::Recycle ()
{
  //## begin ClTransportAction::Recycle%961597594.body preserve=yes
	return M4_TRUE;
  //## end ClTransportAction::Recycle%961597594.body
}

m4bool_t ClTransportAction::CanRecycle ()
{
  //## begin ClTransportAction::CanRecycle%961597595.body preserve=yes
	return M4_TRUE;
  //## end ClTransportAction::CanRecycle%961597595.body
}

m4return_t ClTransportAction::Destroy ()
{
  //## begin ClTransportAction::Destroy%961667110.body preserve=yes
	return M4_SUCCESS;
  //## end ClTransportAction::Destroy%961667110.body
}

m4bool_t ClTransportAction::CanDestroy ()
{
  //## begin ClTransportAction::CanDestroy%961667111.body preserve=yes
	return M4_TRUE;
  //## end ClTransportAction::CanDestroy%961667111.body
}

// Additional Declarations
  //## begin ClTransportAction%38D226E40141.declarations preserve=yes
  //## end ClTransportAction%38D226E40141.declarations

// Class ClTransportLayerIdentifier 








ClTransportLayerIdentifier::ClTransportLayerIdentifier (ClTSAP *ai_pTSAP, TSAPType ai_TypeTSAP, ClIOEventHandler *ai_pIOEventHandler)
  //## begin ClTransportLayerIdentifier::ClTransportLayerIdentifier%954835571.hasinit preserve=no
      : m_ulReaded(0L), m_ulWrited(0L), m_lId(0)
  //## end ClTransportLayerIdentifier::ClTransportLayerIdentifier%954835571.hasinit
  //## begin ClTransportLayerIdentifier::ClTransportLayerIdentifier%954835571.initialization preserve=yes
  ,m_pTSAP(ai_pTSAP),m_TSAPType(ai_TypeTSAP), m_pIOEventHandlerRead(ai_pIOEventHandler)
  //## end ClTransportLayerIdentifier::ClTransportLayerIdentifier%954835571.initialization
{
  //## begin ClTransportLayerIdentifier::ClTransportLayerIdentifier%954835571.body preserve=yes
	if ( ai_pTSAP )
	{
		if ( ai_pTSAP -> GetId () ) 
		{
			m_lId = ai_pTSAP -> GetId () ;
		}
	}
  //## end ClTransportLayerIdentifier::ClTransportLayerIdentifier%954835571.body
}



//## Other Operations (implementation)
void ClTransportLayerIdentifier::SetTSAP (ClTSAP *ai_pTSAP)
{
  //## begin ClTransportLayerIdentifier::SetTSAP%954835576.body preserve=yes
	m_pTSAP = ai_pTSAP;
  //## end ClTransportLayerIdentifier::SetTSAP%954835576.body
}

ClTSAP * ClTransportLayerIdentifier::GetTSAP ()
{
  //## begin ClTransportLayerIdentifier::GetTSAP%954835577.body preserve=yes
	return m_pTSAP;
  //## end ClTransportLayerIdentifier::GetTSAP%954835577.body
}

void ClTransportLayerIdentifier::SetTSAPType (TSAPType ai_TSAPType)
{
  //## begin ClTransportLayerIdentifier::SetTSAPType%954835587.body preserve=yes
	m_TSAPType = ai_TSAPType;
  //## end ClTransportLayerIdentifier::SetTSAPType%954835587.body
}

TSAPType ClTransportLayerIdentifier::GetTSAPType ()
{
  //## begin ClTransportLayerIdentifier::GetTSAPType%954835588.body preserve=yes
	return m_TSAPType;
  //## end ClTransportLayerIdentifier::GetTSAPType%954835588.body
}

void ClTransportLayerIdentifier::SetIOEventHandlerWrite (ClIOEventHandler *ai_pIOEventHandler)
{
  //## begin ClTransportLayerIdentifier::SetIOEventHandlerWrite%955436197.body preserve=yes
	m_pIOEventHandlerWrite = ai_pIOEventHandler;
  //## end ClTransportLayerIdentifier::SetIOEventHandlerWrite%955436197.body
}

ClIOEventHandler * ClTransportLayerIdentifier::GetIOEventHandlerWrite ()
{
  //## begin ClTransportLayerIdentifier::GetIOEventHandlerWrite%955436198.body preserve=yes
	return m_pIOEventHandlerWrite;
  //## end ClTransportLayerIdentifier::GetIOEventHandlerWrite%955436198.body
}

void ClTransportLayerIdentifier::SetIOEventHandlerRead (ClIOEventHandler *ai_pIOEventHandler)
{
  //## begin ClTransportLayerIdentifier::SetIOEventHandlerRead%955360063.body preserve=yes
	m_pIOEventHandlerRead = ai_pIOEventHandler;
  //## end ClTransportLayerIdentifier::SetIOEventHandlerRead%955360063.body
}

ClIOEventHandler * ClTransportLayerIdentifier::GetIOEventHandlerRead ()
{
  //## begin ClTransportLayerIdentifier::GetIOEventHandlerRead%954835558.body preserve=yes
	return m_pIOEventHandlerRead;
  //## end ClTransportLayerIdentifier::GetIOEventHandlerRead%954835558.body
}

m4uint64_t ClTransportLayerIdentifier::GetId ()
{
  //## begin ClTransportLayerIdentifier::GetId%1014805135.body preserve=yes
	return m_lId ;
  //## end ClTransportLayerIdentifier::GetId%1014805135.body
}

// Additional Declarations
  //## begin ClTransportLayerIdentifier%38D1E2410372.declarations preserve=yes
  //## end ClTransportLayerIdentifier%38D1E2410372.declarations

// Class ClTransportActionOpenPort 


ClTransportActionOpenPort::ClTransportActionOpenPort ()
  //## begin ClTransportActionOpenPort::ClTransportActionOpenPort%961667112.hasinit preserve=no
  //## end ClTransportActionOpenPort::ClTransportActionOpenPort%961667112.hasinit
  //## begin ClTransportActionOpenPort::ClTransportActionOpenPort%961667112.initialization preserve=yes
  :ClTransportAction(TRANSPORT_ACTION_OPEN_PORT)
  //## end ClTransportActionOpenPort::ClTransportActionOpenPort%961667112.initialization
{
  //## begin ClTransportActionOpenPort::ClTransportActionOpenPort%961667112.body preserve=yes
  //## end ClTransportActionOpenPort::ClTransportActionOpenPort%961667112.body
}

ClTransportActionOpenPort::ClTransportActionOpenPort (int ai_iType)
  //## begin ClTransportActionOpenPort::ClTransportActionOpenPort%961742125.hasinit preserve=no
  //## end ClTransportActionOpenPort::ClTransportActionOpenPort%961742125.hasinit
  //## begin ClTransportActionOpenPort::ClTransportActionOpenPort%961742125.initialization preserve=yes
  :ClTransportAction(TRANSPORT_ACTION_OPEN_PORT)
  //## end ClTransportActionOpenPort::ClTransportActionOpenPort%961742125.initialization
{
  //## begin ClTransportActionOpenPort::ClTransportActionOpenPort%961742125.body preserve=yes
  //## end ClTransportActionOpenPort::ClTransportActionOpenPort%961742125.body
}


ClTransportActionOpenPort::~ClTransportActionOpenPort ()
{
  //## begin ClTransportActionOpenPort::~ClTransportActionOpenPort%962001297.body preserve=yes
  //## end ClTransportActionOpenPort::~ClTransportActionOpenPort%962001297.body
}



//## Other Operations (implementation)
m4return_t ClTransportActionOpenPort::Init (int ai_iPort, ClTSAP *ai_pTSAP, ClIOEventHandler *ai_pIOEventHandler)
{
  //## begin ClTransportActionOpenPort::Init%954835573.body preserve=yes
	if (ai_pIOEventHandler == NULL)
		return M4_ERROR;

  m_iPort = ai_iPort;
  m_pTSAP = ai_pTSAP;
  m_pIOEventHandler = ai_pIOEventHandler;

  return M4_SUCCESS;
  //## end ClTransportActionOpenPort::Init%954835573.body
}

m4return_t ClTransportActionOpenPort::Execute (ClTransportLayerContainer *ai_pTransportLayerContainer)
{
  //## begin ClTransportActionOpenPort::Execute%954835575.body preserve=yes
	ClServerTSAPSelectable *pServerTSAPSelectable = new ClServerTSAPSelectable(m_iPort);

	pServerTSAPSelectable  -> SetId ( g_iTSAPId ++ ) ;

	ClTransportLayerIdentifier *pTransportLayerIdentifier = 
		new ClTransportLayerIdentifier(pServerTSAPSelectable , LISTEN_SOCKET, m_pIOEventHandler);
	if (pTransportLayerIdentifier == NULL)
	{
		m4Trace(fprintf(stderr, "Error de memoria en ClTransportActionOpenPort::Execute\n"));
		m_pIOEventHandler->ProccessOpenPort(NULL, M4_ERROR);

		return M4_ERROR;
	}


	if (pServerTSAPSelectable  == NULL)
	{
		m4Trace(fprintf(stderr, "Error de memoria en ClTransportActionOpenPort::Execute\n"));
		m_pIOEventHandler->ProccessOpenPort(NULL, M4_ERROR);

		return M4_ERROR;
	}
	if (pServerTSAPSelectable->Error() == M4_TRUE)
	{
		m4Trace(fprintf(stderr, "Error:pServerTSAPSelectable->Error()== M4_TRUE en función ClTransportActionOpenPort::Execute\n"));
		delete pServerTSAPSelectable;
		m_pIOEventHandler->ProccessOpenPort(NULL, M4_ERROR);

		return M4_ERROR;
	}
	 

	pTransportLayerIdentifier->SetTSAP(pServerTSAPSelectable);
	m4return_t ret;
	if ((ret = pServerTSAPSelectable->Listen()) != M4_SUCCESS)
	{
		m4Trace(fprintf(stderr, "Error en Listen en función ClTransportActionOpenPort::Execute\n"));
		delete pServerTSAPSelectable;
		m_pIOEventHandler->ProccessOpenPort(NULL, M4_ERROR);

		return M4_ERROR;

	}

	if (ai_pTransportLayerContainer->AddTransportLayerIdentifier(pTransportLayerIdentifier)
		 != M4_SUCCESS)
	{
		m4Trace(fprintf(stderr, "Error en AddTransportLayerIdentifier en función ClTransportActionOpenPort::Execute\n"));
		delete pServerTSAPSelectable;
		m_pIOEventHandler->ProccessOpenPort((m4tliid_t *)NULL, M4_ERROR);

		return M4_ERROR;
	}
	if (ai_pTransportLayerContainer->AddSelect(pTransportLayerIdentifier, M4_TRUE, M4_FALSE)
		 != M4_SUCCESS)
	{
		//Tratamiento de error
		m4Trace(fprintf(stderr, "Error en AddTransportLayerIdentifier en función ClTransportActionOpenPort::Execute\n"));
		delete pServerTSAPSelectable;
		m_pIOEventHandler->ProccessOpenPort((m4tliid_t)pServerTSAPSelectable, M4_ERROR);

		return M4_ERROR;
	}
	m_pIOEventHandler->ProccessOpenPort((m4tliid_t)pServerTSAPSelectable, M4_SUCCESS);

	return M4_SUCCESS;
  //## end ClTransportActionOpenPort::Execute%954835575.body
}

// Additional Declarations
  //## begin ClTransportActionOpenPort%38DF327502CF.declarations preserve=yes
  //## end ClTransportActionOpenPort%38DF327502CF.declarations

// Class ClTransportActionBeginRead 

ClTransportActionBeginRead::ClTransportActionBeginRead ()
  //## begin ClTransportActionBeginRead::ClTransportActionBeginRead%961667115.hasinit preserve=no
  //## end ClTransportActionBeginRead::ClTransportActionBeginRead%961667115.hasinit
  //## begin ClTransportActionBeginRead::ClTransportActionBeginRead%961667115.initialization preserve=yes
  :ClTransportAction(TRANSPORT_ACTION_BEGIN_READ)
  //## end ClTransportActionBeginRead::ClTransportActionBeginRead%961667115.initialization
{
  //## begin ClTransportActionBeginRead::ClTransportActionBeginRead%961667115.body preserve=yes
  //## end ClTransportActionBeginRead::ClTransportActionBeginRead%961667115.body
}


ClTransportActionBeginRead::~ClTransportActionBeginRead ()
{
  //## begin ClTransportActionBeginRead::~ClTransportActionBeginRead%962001298.body preserve=yes
  //## end ClTransportActionBeginRead::~ClTransportActionBeginRead%962001298.body
}



//## Other Operations (implementation)
m4return_t ClTransportActionBeginRead::Execute (ClTransportLayerContainer *ai_pTransportLayerContainer)
{
  //## begin ClTransportActionBeginRead::Execute%955035324.body preserve=yes
	ClTransportLayerIdentifier *pTransportLayerIdentifier = ai_pTransportLayerContainer->GetTransportLayerIdentifier(m_pTSAP);
	if (pTransportLayerIdentifier  == NULL)
	{
		m4Trace(fprintf(stderr, "Error en ClTransportActionBeginRead::Execute TSAP %d no está en el mapa\n", (int)m_pTSAP));
//		m_pIOEventHandler->ProccessBeginRead((m4tliid_t)m_pTSAP, M4_ERROR);

		return M4_ERROR;
	}
	pTransportLayerIdentifier->SetIOEventHandlerRead(m_pIOEventHandler); 
	if (ai_pTransportLayerContainer->AddSelect(pTransportLayerIdentifier, M4_TRUE, M4_FALSE)
		 != M4_SUCCESS)
	{
		m4Trace(fprintf(stderr, "Error en ClTransportActionBeginRead::Execute AddSelect %d falla\n", (int)m_pTSAP));
		m_pIOEventHandler->ProccessBeginRead((m4tliid_t)m_pTSAP, M4_ERROR);

		return M4_ERROR;
	}
	m_pIOEventHandler->ProccessBeginRead((m4tliid_t)m_pTSAP, M4_SUCCESS);

	return M4_SUCCESS;
  //## end ClTransportActionBeginRead::Execute%955035324.body
}

m4return_t ClTransportActionBeginRead::Init (ClTSAP *ai_pTSAP, ClIOEventHandler *ai_pIOEventHandler)
{
  //## begin ClTransportActionBeginRead::Init%961667116.body preserve=yes
  m_pTSAP = ai_pTSAP;
  m_pIOEventHandler = ai_pIOEventHandler;

  return M4_SUCCESS;
  //## end ClTransportActionBeginRead::Init%961667116.body
}

// Additional Declarations
  //## begin ClTransportActionBeginRead%38DF353B0009.declarations preserve=yes
  //## end ClTransportActionBeginRead%38DF353B0009.declarations

// Class ClTransportActionClosePort 

ClTransportActionClosePort::ClTransportActionClosePort ()
  //## begin ClTransportActionClosePort::ClTransportActionClosePort%961742128.hasinit preserve=no
  //## end ClTransportActionClosePort::ClTransportActionClosePort%961742128.hasinit
  //## begin ClTransportActionClosePort::ClTransportActionClosePort%961742128.initialization preserve=yes
  :ClTransportAction(TRANSPORT_ACTION_CLOSE_PORT)
  //## end ClTransportActionClosePort::ClTransportActionClosePort%961742128.initialization
{
  //## begin ClTransportActionClosePort::ClTransportActionClosePort%961742128.body preserve=yes
  //## end ClTransportActionClosePort::ClTransportActionClosePort%961742128.body
}


ClTransportActionClosePort::~ClTransportActionClosePort ()
{
  //## begin ClTransportActionClosePort::~ClTransportActionClosePort%962001299.body preserve=yes
  //## end ClTransportActionClosePort::~ClTransportActionClosePort%962001299.body
}



//## Other Operations (implementation)
m4return_t ClTransportActionClosePort::Execute (ClTransportLayerContainer *ai_pTransportLayerContainer)
{
  //## begin ClTransportActionClosePort::Execute%955035320.body preserve=yes
	ClTransportLayerIdentifier *pTransportLayerIdentifier = ai_pTransportLayerContainer->GetTransportLayerIdentifier(m_pTSAP);
	if (pTransportLayerIdentifier  == NULL)
	{
		m4Trace(fprintf(stderr, "Error en ClTransportActionClosePort::Execute GetTransportLayerIdentifier falla\n", (int)m_pTSAP));
		m_pIOEventHandler->ProccessClosePort((m4tliid_t)m_pTSAP, M4_ERROR);

		return M4_ERROR;
	}
	if (ai_pTransportLayerContainer->RemoveSelect(pTransportLayerIdentifier, M4_TRUE, M4_FALSE)
		 != M4_SUCCESS)
	{
		//Tratamiento de error
		m4Trace(fprintf(stderr, "Error en ClTransportActionClosePort::Execute RemoveSelect falla\n", (int)m_pTSAP));
		m_pIOEventHandler->ProccessClosePort((m4tliid_t)m_pTSAP, M4_ERROR);

		return M4_ERROR;
	}
	if (ai_pTransportLayerContainer->RemoveTransportLayerIdentifier(pTransportLayerIdentifier)
		 != M4_SUCCESS)
	{
		//Tratamiento de error
		m4Trace(fprintf(stderr, "Error en ClTransportActionClosePort::Execute RemoveTransportLayerIdentifier falla\n", (int)m_pTSAP));
		m_pIOEventHandler->ProccessClosePort((m4tliid_t)m_pTSAP, M4_ERROR);

		return M4_ERROR;
	}
	m_pIOEventHandler->ProccessClosePort((m4tliid_t)m_pTSAP, M4_SUCCESS);

	return M4_SUCCESS;
  //## end ClTransportActionClosePort::Execute%955035320.body
}

m4return_t ClTransportActionClosePort::Init (ClTSAP *ai_pTSAP, ClIOEventHandler *ai_pIOEventHandler)
{
  //## begin ClTransportActionClosePort::Init%961742129.body preserve=yes
  m_pTSAP = ai_pTSAP;
  m_pIOEventHandler = ai_pIOEventHandler;

  return M4_SUCCESS;
  //## end ClTransportActionClosePort::Init%961742129.body
}

// Additional Declarations
  //## begin ClTransportActionClosePort%38DF357300B4.declarations preserve=yes
  //## end ClTransportActionClosePort%38DF357300B4.declarations

// Class ClTransportActionStopReading 

ClTransportActionStopReading::ClTransportActionStopReading ()
  //## begin ClTransportActionStopReading::ClTransportActionStopReading%961667119.hasinit preserve=no
  //## end ClTransportActionStopReading::ClTransportActionStopReading%961667119.hasinit
  //## begin ClTransportActionStopReading::ClTransportActionStopReading%961667119.initialization preserve=yes
  :ClTransportAction(TRANSPORT_ACTION_STOP_READING)
  //## end ClTransportActionStopReading::ClTransportActionStopReading%961667119.initialization
{
  //## begin ClTransportActionStopReading::ClTransportActionStopReading%961667119.body preserve=yes
  //## end ClTransportActionStopReading::ClTransportActionStopReading%961667119.body
}


ClTransportActionStopReading::~ClTransportActionStopReading ()
{
  //## begin ClTransportActionStopReading::~ClTransportActionStopReading%962001300.body preserve=yes
  //## end ClTransportActionStopReading::~ClTransportActionStopReading%962001300.body
}



//## Other Operations (implementation)
m4return_t ClTransportActionStopReading::Execute (ClTransportLayerContainer *ai_pTransportLayerContainer)
{
  //## begin ClTransportActionStopReading::Execute%955466400.body preserve=yes
	ClTransportLayerIdentifier *pTransportLayerIdentifier = ai_pTransportLayerContainer->GetTransportLayerIdentifier(m_pTSAP);
	if (pTransportLayerIdentifier  == NULL)
	{
		m4Trace(fprintf(stderr, "Error en ClTransportActionStopReading::Execute GetTransportLayerIdentifier falla\n", (int)m_pTSAP));
//		m_pIOEventHandler->ProccessStopReading((m4tliid_t)m_pTSAP, M4_ERROR);

		return M4_ERROR;
	}
	if (ai_pTransportLayerContainer->RemoveSelect(pTransportLayerIdentifier, M4_TRUE, M4_FALSE)
		 != M4_SUCCESS)
	{
		//Tratamiento de error
		m4Trace(fprintf(stderr, "Error en ClTransportActionStopReading::Execute GetTransportLayerIdentifier falla\n", (int)m_pTSAP));
		m_pIOEventHandler->ProccessStopReading((m4tliid_t)m_pTSAP, M4_ERROR);

		return M4_ERROR;
	}
	m_pIOEventHandler->ProccessStopReading((m4tliid_t)m_pTSAP, M4_SUCCESS);

	return M4_SUCCESS;
  //## end ClTransportActionStopReading::Execute%955466400.body
}

m4return_t ClTransportActionStopReading::Init (ClTSAP *ai_pTSAP, ClIOEventHandler *ai_pIOEventHandler)
{
  //## begin ClTransportActionStopReading::Init%961667120.body preserve=yes
  m_pTSAP = ai_pTSAP;
  m_pIOEventHandler = ai_pIOEventHandler;

  return M4_SUCCESS;
  //## end ClTransportActionStopReading::Init%961667120.body
}

// Additional Declarations
  //## begin ClTransportActionStopReading%38DF35C703D6.declarations preserve=yes
  //## end ClTransportActionStopReading%38DF35C703D6.declarations

// Class ClTransportActionWrite 



ClTransportActionWrite::ClTransportActionWrite ()
  //## begin ClTransportActionWrite::ClTransportActionWrite%961667117.hasinit preserve=no
  //## end ClTransportActionWrite::ClTransportActionWrite%961667117.hasinit
  //## begin ClTransportActionWrite::ClTransportActionWrite%961667117.initialization preserve=yes
  :ClTransportAction(TRANSPORT_ACTION_WRITE)
  //## end ClTransportActionWrite::ClTransportActionWrite%961667117.initialization
{
  //## begin ClTransportActionWrite::ClTransportActionWrite%961667117.body preserve=yes
  //## end ClTransportActionWrite::ClTransportActionWrite%961667117.body
}


ClTransportActionWrite::~ClTransportActionWrite ()
{
  //## begin ClTransportActionWrite::~ClTransportActionWrite%962001301.body preserve=yes
  //## end ClTransportActionWrite::~ClTransportActionWrite%962001301.body
}



//## Other Operations (implementation)
m4return_t ClTransportActionWrite::Execute (ClTransportLayerContainer *ai_pTransportLayerContainer)
{
  //## begin ClTransportActionWrite::Execute%955360065.body preserve=yes
	ClTransportLayerIdentifier *pTransportLayerIdentifier = ai_pTransportLayerContainer->GetTransportLayerIdentifier(m_pTSAP);
	if (pTransportLayerIdentifier  == NULL)
	{
		m4Trace(fprintf(stderr, "Error en ClTransportActionWrite::Execute GetTransportLayerIdentifier falla\n", (int)m_pTSAP));
//		m_pIOEventHandler->ProccessWrite((m4tliid_t)m_pTSAP, M4_ERROR);

		return M4_ERROR;
	}

	m_pTSAP->SetBlockingIO(M4_FALSE);

	int ret = m_pTSAP->Write(m_pIOEventHandler->GetBuffer(), 
							m_pIOEventHandler->GetBufferSize());

	if (ret == -1)
	{
		m4Trace(fprintf(stderr, "Error en ClTransportActionWrite::Execute m_pTSAP->Write falla\n", (int)m_pTSAP));
		m_pIOEventHandler->ProccessWrite((m4tliid_t)m_pTSAP, M4_ERROR);

		return M4_ERROR;
	}
	
	if (ret > 0)
		pTransportLayerIdentifier->m_ulWrited += ret;

	if (ret == (int)m_pIOEventHandler->GetBufferSize())
	{
		m_pIOEventHandler->ProccessWrite((m4tliid_t)m_pTSAP, M4_SUCCESS);

		return M4_SUCCESS;
	}

	//No hemos podido escribir todo el buffer, crear una acción que escriba
	// la parte que falta, incluyendo el caso en que Write devuelva -2 (sólo 
	// en Windows, indica que el buffer del sistema está lleno)
	if (ret > 0)
	{
		m_pIOEventHandler->SetBuffer(m_pIOEventHandler->GetBuffer() + ret);
		m_pIOEventHandler->SetBufferSize(m_pIOEventHandler->GetBufferSize() - ret);
	}

	pTransportLayerIdentifier->SetIOEventHandlerWrite(m_pIOEventHandler);


	if (ai_pTransportLayerContainer->AddSelect(pTransportLayerIdentifier, M4_FALSE, M4_TRUE)
		 != M4_SUCCESS)
	{
		m4Trace(fprintf(stderr, "Error en ClTransportActionWrite::Execute AddSelect falla\n", (int)m_pTSAP));
		m_pIOEventHandler->ProccessWrite((m4tliid_t)m_pTSAP, M4_ERROR);

		return M4_ERROR;
	}

	return M4_SUCCESS;
  //## end ClTransportActionWrite::Execute%955360065.body
}

m4return_t ClTransportActionWrite::Init (ClTSAP *ai_pTSAP, ClIOEventHandler *ai_pIOEventHandler)
{
  //## begin ClTransportActionWrite::Init%961667118.body preserve=yes
  m_pTSAP = ai_pTSAP;
  m_pIOEventHandler = ai_pIOEventHandler;

  return M4_SUCCESS;
  //## end ClTransportActionWrite::Init%961667118.body
}

// Additional Declarations
  //## begin ClTransportActionWrite%38DF365000BC.declarations preserve=yes
  //## end ClTransportActionWrite%38DF365000BC.declarations

// Class ClTransportActionStopWriting 

ClTransportActionStopWriting::ClTransportActionStopWriting ()
  //## begin ClTransportActionStopWriting::ClTransportActionStopWriting%961667121.hasinit preserve=no
  //## end ClTransportActionStopWriting::ClTransportActionStopWriting%961667121.hasinit
  //## begin ClTransportActionStopWriting::ClTransportActionStopWriting%961667121.initialization preserve=yes
  :ClTransportAction(TRANSPORT_ACTION_STOP_WRITING)
  //## end ClTransportActionStopWriting::ClTransportActionStopWriting%961667121.initialization
{
  //## begin ClTransportActionStopWriting::ClTransportActionStopWriting%961667121.body preserve=yes
  //## end ClTransportActionStopWriting::ClTransportActionStopWriting%961667121.body
}


ClTransportActionStopWriting::~ClTransportActionStopWriting ()
{
  //## begin ClTransportActionStopWriting::~ClTransportActionStopWriting%962001302.body preserve=yes
  //## end ClTransportActionStopWriting::~ClTransportActionStopWriting%962001302.body
}



//## Other Operations (implementation)
m4return_t ClTransportActionStopWriting::Execute (ClTransportLayerContainer *ai_pTransportLayerContainer)
{
  //## begin ClTransportActionStopWriting::Execute%955466402.body preserve=yes
	ClTransportLayerIdentifier *pTransportLayerIdentifier = ai_pTransportLayerContainer->GetTransportLayerIdentifier(m_pTSAP);
	if (pTransportLayerIdentifier  == NULL)
	{
		m_pIOEventHandler->ProccessStopWriting((m4tliid_t)m_pTSAP, M4_ERROR);

		return M4_ERROR;
	}
	if (ai_pTransportLayerContainer->RemoveSelect(pTransportLayerIdentifier, M4_FALSE, M4_TRUE)
		 != M4_SUCCESS)
	{
		//Tratamiento de error
		m_pIOEventHandler->ProccessStopWriting((m4tliid_t)m_pTSAP, M4_ERROR);

		return M4_ERROR;
	}
	m_pIOEventHandler->ProccessStopWriting((m4tliid_t)m_pTSAP, M4_SUCCESS);

	return M4_SUCCESS;
  //## end ClTransportActionStopWriting::Execute%955466402.body
}

m4return_t ClTransportActionStopWriting::Init (ClTSAP *ai_pTSAP, ClIOEventHandler *ai_pIOEventHandler)
{
  //## begin ClTransportActionStopWriting::Init%961667122.body preserve=yes
  m_pTSAP = ai_pTSAP;
  m_pIOEventHandler = ai_pIOEventHandler;

  return M4_SUCCESS;
  //## end ClTransportActionStopWriting::Init%961667122.body
}

// Additional Declarations
  //## begin ClTransportActionStopWriting%38DF37860361.declarations preserve=yes
  //## end ClTransportActionStopWriting%38DF37860361.declarations

// Class ClTransportActionConnect 




ClTransportActionConnect::ClTransportActionConnect ()
  //## begin ClTransportActionConnect::ClTransportActionConnect%961742122.hasinit preserve=no
  //## end ClTransportActionConnect::ClTransportActionConnect%961742122.hasinit
  //## begin ClTransportActionConnect::ClTransportActionConnect%961742122.initialization preserve=yes
  :ClTransportAction(TRANSPORT_ACTION_CONNECT)
  //## end ClTransportActionConnect::ClTransportActionConnect%961742122.initialization
{
  //## begin ClTransportActionConnect::ClTransportActionConnect%961742122.body preserve=yes
  //## end ClTransportActionConnect::ClTransportActionConnect%961742122.body
}

ClTransportActionConnect::ClTransportActionConnect (int ai_iType)
  //## begin ClTransportActionConnect::ClTransportActionConnect%961742124.hasinit preserve=no
  //## end ClTransportActionConnect::ClTransportActionConnect%961742124.hasinit
  //## begin ClTransportActionConnect::ClTransportActionConnect%961742124.initialization preserve=yes
  :ClTransportAction(ai_iType)
  //## end ClTransportActionConnect::ClTransportActionConnect%961742124.initialization
{
  //## begin ClTransportActionConnect::ClTransportActionConnect%961742124.body preserve=yes
  //## end ClTransportActionConnect::ClTransportActionConnect%961742124.body
}


ClTransportActionConnect::~ClTransportActionConnect ()
{
  //## begin ClTransportActionConnect::~ClTransportActionConnect%962001303.body preserve=yes
  //## end ClTransportActionConnect::~ClTransportActionConnect%962001303.body
}



//## Other Operations (implementation)
m4return_t ClTransportActionConnect::Execute (ClTransportLayerContainer *ai_pTransportLayerContainer)
{
  //## begin ClTransportActionConnect::Execute%955536868.body preserve=yes
	ClTSAP *pTSAP = new ClTSAP(m_pHost, m_iPort);

	pTSAP  -> SetId ( g_iTSAPId ++ ) ;

	if (pTSAP == NULL || pTSAP->Error())
	{
		//Tratamiento de error
		m_pIOEventHandler->ProccessConnect((m4tliid_t)pTSAP, M4_ERROR);

		return M4_ERROR;
	}
	ClTransportLayerIdentifier *pTransportLayerIdentifier = 
		new ClTransportLayerIdentifier(pTSAP, CONNECTION_SOCKET, m_pIOEventHandler);
	if (ai_pTransportLayerContainer->AddTransportLayerIdentifier(pTransportLayerIdentifier)
		 != M4_SUCCESS)
	{
		//Tratamiento de error
		m_pIOEventHandler->ProccessConnect((m4tliid_t *)pTSAP, M4_ERROR);

		return M4_ERROR;
	}
	m_pIOEventHandler->ProccessConnect((m4tliid_t)pTSAP, M4_SUCCESS);

	return M4_SUCCESS;
  //## end ClTransportActionConnect::Execute%955536868.body
}

m4return_t ClTransportActionConnect::Init (char *ai_pHost, int ai_iPort, ClTSAP *ai_pTSAP, ClIOEventHandler *ai_pIOEventHandler)
{
  //## begin ClTransportActionConnect::Init%961742123.body preserve=yes
	m_pHost =ai_pHost;
	m_iPort =ai_iPort; 
	
	m_pTSAP = ai_pTSAP;
	m_pIOEventHandler = ai_pIOEventHandler;
	
	return M4_SUCCESS;
  //## end ClTransportActionConnect::Init%961742123.body
}

// Additional Declarations
  //## begin ClTransportActionConnect%38DF37BC012D.declarations preserve=yes
  //## end ClTransportActionConnect%38DF37BC012D.declarations

// Class ClTransportActionCloseConnection 

ClTransportActionCloseConnection::ClTransportActionCloseConnection ()
  //## begin ClTransportActionCloseConnection::ClTransportActionCloseConnection%961742130.hasinit preserve=no
  //## end ClTransportActionCloseConnection::ClTransportActionCloseConnection%961742130.hasinit
  //## begin ClTransportActionCloseConnection::ClTransportActionCloseConnection%961742130.initialization preserve=yes
  :ClTransportAction(TRANSPORT_ACTION_CLOSE_CONNECTION)
  //## end ClTransportActionCloseConnection::ClTransportActionCloseConnection%961742130.initialization
{
  //## begin ClTransportActionCloseConnection::ClTransportActionCloseConnection%961742130.body preserve=yes
  //## end ClTransportActionCloseConnection::ClTransportActionCloseConnection%961742130.body
}


ClTransportActionCloseConnection::~ClTransportActionCloseConnection ()
{
  //## begin ClTransportActionCloseConnection::~ClTransportActionCloseConnection%962001304.body preserve=yes
  //## end ClTransportActionCloseConnection::~ClTransportActionCloseConnection%962001304.body
}



//## Other Operations (implementation)
m4return_t ClTransportActionCloseConnection::Execute (ClTransportLayerContainer *ai_pTransportLayerContainer)
{
  //## begin ClTransportActionCloseConnection::Execute%955536870.body preserve=yes
	ClTransportLayerIdentifier *pTransportLayerIdentifier = ai_pTransportLayerContainer->GetTransportLayerIdentifier(m_pTSAP);
	if (pTransportLayerIdentifier  == NULL)
	{
		m_pIOEventHandler->ProccessStopConnection((m4tliid_t)m_pTSAP, M4_ERROR);

		return M4_ERROR;
	}
	ai_pTransportLayerContainer->RemoveSelect(pTransportLayerIdentifier, M4_TRUE, M4_TRUE);

	if ( M4_SUCCESS == ai_pTransportLayerContainer->RemoveTransportLayerIdentifier(pTransportLayerIdentifier) )
	{
		
		delete pTransportLayerIdentifier;
//		delete m_pTSAP;

		m_pIOEventHandler->ProccessStopConnection((m4tliid_t)m_pTSAP, M4_SUCCESS);
	}


	return M4_SUCCESS;
  //## end ClTransportActionCloseConnection::Execute%955536870.body
}

m4return_t ClTransportActionCloseConnection::Init (ClTSAP *ai_pTSAP, ClIOEventHandler *ai_pIOEventHandler)
{
  //## begin ClTransportActionCloseConnection::Init%961742131.body preserve=yes
	m_pTSAP = ai_pTSAP;
	m_pIOEventHandler = ai_pIOEventHandler;
	
	return M4_SUCCESS;
  //## end ClTransportActionCloseConnection::Init%961742131.body
}

// Additional Declarations
  //## begin ClTransportActionCloseConnection%38DF390C0235.declarations preserve=yes
  //## end ClTransportActionCloseConnection%38DF390C0235.declarations

// Class ClTransportActionSSLConnect 



ClTransportActionSSLConnect::ClTransportActionSSLConnect ()
  //## begin ClTransportActionSSLConnect::ClTransportActionSSLConnect%961742120.hasinit preserve=no
  //## end ClTransportActionSSLConnect::ClTransportActionSSLConnect%961742120.hasinit
  //## begin ClTransportActionSSLConnect::ClTransportActionSSLConnect%961742120.initialization preserve=yes
  :ClTransportActionConnect(TRANSPORT_ACTION_SSL_CONNECT)
  //## end ClTransportActionSSLConnect::ClTransportActionSSLConnect%961742120.initialization
{
  //## begin ClTransportActionSSLConnect::ClTransportActionSSLConnect%961742120.body preserve=yes
  //## end ClTransportActionSSLConnect::ClTransportActionSSLConnect%961742120.body
}


ClTransportActionSSLConnect::~ClTransportActionSSLConnect ()
{
  //## begin ClTransportActionSSLConnect::~ClTransportActionSSLConnect%962001305.body preserve=yes
  //## end ClTransportActionSSLConnect::~ClTransportActionSSLConnect%962001305.body
}



//## Other Operations (implementation)
m4return_t ClTransportActionSSLConnect::Execute (ClTransportLayerContainer *ai_pTransportLayerContainer)
{
  //## begin ClTransportActionSSLConnect::Execute%956570161.body preserve=yes
	ClTSAP *pTSAP = new ClTSAP(m_iMode, m_pCipher, (const char *)m_pHost, m_iPort);

	pTSAP  -> SetId ( g_iTSAPId ++ ) ;

	if (pTSAP == NULL || pTSAP->Error())
	{
		//Tratamiento de error
		m_pIOEventHandler->ProccessConnect((m4tliid_t)pTSAP, M4_ERROR);

		return M4_ERROR;
	}
	ClTransportLayerIdentifier *pTransportLayerIdentifier = 
		new ClTransportLayerIdentifier(pTSAP, CONNECTION_SOCKET, m_pIOEventHandler);
	if (ai_pTransportLayerContainer->AddTransportLayerIdentifier(pTransportLayerIdentifier)
		 != M4_SUCCESS)
	{
		//Tratamiento de error
		m_pIOEventHandler->ProccessConnect((m4tliid_t *)pTSAP, M4_ERROR);

		return M4_ERROR;
	}
	m_pIOEventHandler->ProccessConnect((m4tliid_t)pTSAP, M4_SUCCESS);

	return M4_SUCCESS;
  //## end ClTransportActionSSLConnect::Execute%956570161.body
}

m4return_t ClTransportActionSSLConnect::Init (int ai_iMode, char *ai_pCipher, char *ai_pHost, int ai_iPort, ClTSAP *ai_pTSAP, ClIOEventHandler *ai_pIOEventHandler)
{
  //## begin ClTransportActionSSLConnect::Init%961742121.body preserve=yes
  m_iMode = ai_iMode;
  m_pCipher = ai_pCipher; 

  m_pHost = ai_pHost;
  m_iPort = ai_iPort;
  m_pTSAP = ai_pTSAP;
  m_pIOEventHandler = ai_pIOEventHandler;

  return M4_SUCCESS;
  //## end ClTransportActionSSLConnect::Init%961742121.body
}

// Additional Declarations
  //## begin ClTransportActionSSLConnect%39042600001F.declarations preserve=yes
  //## end ClTransportActionSSLConnect%39042600001F.declarations

// Class ClTransportActionOpenSSLPort 



ClTransportActionOpenSSLPort::ClTransportActionOpenSSLPort ()
  //## begin ClTransportActionOpenSSLPort::ClTransportActionOpenSSLPort%961742126.hasinit preserve=no
  //## end ClTransportActionOpenSSLPort::ClTransportActionOpenSSLPort%961742126.hasinit
  //## begin ClTransportActionOpenSSLPort::ClTransportActionOpenSSLPort%961742126.initialization preserve=yes
  :ClTransportActionOpenPort(TRANSPORT_ACTION_OPEN_SSL_PORT)
  //## end ClTransportActionOpenSSLPort::ClTransportActionOpenSSLPort%961742126.initialization
{
  //## begin ClTransportActionOpenSSLPort::ClTransportActionOpenSSLPort%961742126.body preserve=yes
  //## end ClTransportActionOpenSSLPort::ClTransportActionOpenSSLPort%961742126.body
}


ClTransportActionOpenSSLPort::~ClTransportActionOpenSSLPort ()
{
  //## begin ClTransportActionOpenSSLPort::~ClTransportActionOpenSSLPort%962001306.body preserve=yes
  //## end ClTransportActionOpenSSLPort::~ClTransportActionOpenSSLPort%962001306.body
}



//## Other Operations (implementation)
m4return_t ClTransportActionOpenSSLPort::Execute (ClTransportLayerContainer *ai_pTransportLayerContainer)
{
  //## begin ClTransportActionOpenSSLPort::Execute%956570163.body preserve=yes

	ClServerTSAPSelectable *pServerTSAPSelectable = new ClServerTSAPSelectable(m_iMode, (void *)m_pCipher, m_iPort);

	pServerTSAPSelectable  -> SetId ( g_iTSAPId ++ ) ;

	ClTransportLayerIdentifier *pTransportLayerIdentifier = 
		new ClTransportLayerIdentifier(pServerTSAPSelectable , LISTEN_SOCKET, m_pIOEventHandler);

	if (pServerTSAPSelectable->Error())
	{
		//Tratamiento de error
		m_pIOEventHandler->ProccessOpenPort((m4tliid_t)pServerTSAPSelectable, M4_ERROR);

		return M4_ERROR;
	}
	pTransportLayerIdentifier->SetTSAP(pServerTSAPSelectable);
	pServerTSAPSelectable->Listen();
	if (ai_pTransportLayerContainer->AddTransportLayerIdentifier(pTransportLayerIdentifier)
		 != M4_SUCCESS)
	{
		//Tratamiento de error
		m_pIOEventHandler->ProccessOpenPort((m4tliid_t *)pServerTSAPSelectable, M4_ERROR);

		return M4_ERROR;
	}
	if (ai_pTransportLayerContainer->AddSelect(pTransportLayerIdentifier, M4_TRUE, M4_FALSE)
		 != M4_SUCCESS)
	{
		//Tratamiento de error
		m_pIOEventHandler->ProccessOpenPort((m4tliid_t)pServerTSAPSelectable, M4_ERROR);

		return M4_ERROR;
	}
	m_pIOEventHandler->ProccessOpenPort((m4tliid_t)pServerTSAPSelectable, M4_SUCCESS);

	return M4_SUCCESS;
  //## end ClTransportActionOpenSSLPort::Execute%956570163.body
}

m4return_t ClTransportActionOpenSSLPort::Init (int ai_iMode, char *ai_pCipher, int ai_iPort, ClTSAP *ai_pTSAP, ClIOEventHandler *ai_pIOEventHandler)
{
  //## begin ClTransportActionOpenSSLPort::Init%961742127.body preserve=yes
	m_iMode = ai_iMode;
	m_pCipher = ai_pCipher; 
	m_iPort = ai_iPort;
	m_pTSAP = ai_pTSAP;
	m_pIOEventHandler = ai_pIOEventHandler;
	
	return M4_SUCCESS;
  //## end ClTransportActionOpenSSLPort::Init%961742127.body
}

// Additional Declarations
  //## begin ClTransportActionOpenSSLPort%390454D10023.declarations preserve=yes
  //## end ClTransportActionOpenSSLPort%390454D10023.declarations

// Class ClTransportActionStop 

ClTransportActionStop::ClTransportActionStop ()
  //## begin ClTransportActionStop::ClTransportActionStop%961742132.hasinit preserve=no
  //## end ClTransportActionStop::ClTransportActionStop%961742132.hasinit
  //## begin ClTransportActionStop::ClTransportActionStop%961742132.initialization preserve=yes
  :ClTransportAction(TRANSPORT_ACTION_STOP)
  //## end ClTransportActionStop::ClTransportActionStop%961742132.initialization
{
  //## begin ClTransportActionStop::ClTransportActionStop%961742132.body preserve=yes
  //## end ClTransportActionStop::ClTransportActionStop%961742132.body
}


ClTransportActionStop::~ClTransportActionStop ()
{
  //## begin ClTransportActionStop::~ClTransportActionStop%962001307.body preserve=yes
  //## end ClTransportActionStop::~ClTransportActionStop%962001307.body
}



//## Other Operations (implementation)
m4return_t ClTransportActionStop::Execute (ClTransportLayerContainer *ai_pTransportLayerContainer)
{
  //## begin ClTransportActionStop::Execute%956912717.body preserve=yes
	ai_pTransportLayerContainer->SetWantStop(m_pIOEventHandler);

	return M4_SUCCESS;
  //## end ClTransportActionStop::Execute%956912717.body
}

m4return_t ClTransportActionStop::Init (ClTSAP *ai_pTSAP, ClIOEventHandler *ai_pIOEventHandler)
{
  //## begin ClTransportActionStop::Init%956912716.body preserve=yes
	m_pTSAP = ai_pTSAP;
	m_pIOEventHandler = ai_pIOEventHandler;
	
	return M4_SUCCESS;
  //## end ClTransportActionStop::Init%956912716.body
}

// Additional Declarations
  //## begin ClTransportActionStop%39094FB900D2.declarations preserve=yes
  //## end ClTransportActionStop%39094FB900D2.declarations

// Class ClTransportActionGetTransferedBytes 

ClTransportActionGetTransferedBytes::ClTransportActionGetTransferedBytes ()
  //## begin ClTransportActionGetTransferedBytes::ClTransportActionGetTransferedBytes%961742133.hasinit preserve=no
  //## end ClTransportActionGetTransferedBytes::ClTransportActionGetTransferedBytes%961742133.hasinit
  //## begin ClTransportActionGetTransferedBytes::ClTransportActionGetTransferedBytes%961742133.initialization preserve=yes
  :ClTransportAction(TRANSPORT_ACTION_GET_TRANSFERED_BYTES)
  //## end ClTransportActionGetTransferedBytes::ClTransportActionGetTransferedBytes%961742133.initialization
{
  //## begin ClTransportActionGetTransferedBytes::ClTransportActionGetTransferedBytes%961742133.body preserve=yes
  //## end ClTransportActionGetTransferedBytes::ClTransportActionGetTransferedBytes%961742133.body
}


ClTransportActionGetTransferedBytes::~ClTransportActionGetTransferedBytes ()
{
  //## begin ClTransportActionGetTransferedBytes::~ClTransportActionGetTransferedBytes%962001308.body preserve=yes
  //## end ClTransportActionGetTransferedBytes::~ClTransportActionGetTransferedBytes%962001308.body
}



//## Other Operations (implementation)
m4return_t ClTransportActionGetTransferedBytes::Execute (ClTransportLayerContainer *ai_pTransportLayerContainer)
{
  //## begin ClTransportActionGetTransferedBytes::Execute%958124561.body preserve=yes
	ClTransportLayerIdentifier *pTransportLayerIdentifier = ai_pTransportLayerContainer->GetTransportLayerIdentifier(m_pTSAP);
	if (pTransportLayerIdentifier  == NULL)
	{
		m_pIOEventHandler->ProccessGetTransferedBytes((m4tliid_t)m_pTSAP, 
											0L,
											0L,
											M4_ERROR);

		return M4_ERROR;
	}
	m_pIOEventHandler->ProccessGetTransferedBytes((m4tliid_t)m_pTSAP, 
		pTransportLayerIdentifier->m_ulReaded,
		pTransportLayerIdentifier->m_ulWrited,
		M4_SUCCESS);

	return M4_SUCCESS;
  //## end ClTransportActionGetTransferedBytes::Execute%958124561.body
}

m4return_t ClTransportActionGetTransferedBytes::Init (ClTSAP *ai_pTSAP, ClIOEventHandler *ai_pIOEventHandler)
{
  //## begin ClTransportActionGetTransferedBytes::Init%961742134.body preserve=yes
	m_pTSAP = ai_pTSAP;
	m_pIOEventHandler = ai_pIOEventHandler;
	
	return M4_SUCCESS;
  //## end ClTransportActionGetTransferedBytes::Init%961742134.body
}

// Additional Declarations
  //## begin ClTransportActionGetTransferedBytes%391BD333017F.declarations preserve=yes
  //## end ClTransportActionGetTransferedBytes%391BD333017F.declarations

//## begin module%38D1FD09021F.epilog preserve=yes
//## end module%38D1FD09021F.epilog

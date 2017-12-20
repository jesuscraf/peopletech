//## begin module%38DB62A6038A.cm preserve=no
//## end module%38DB62A6038A.cm

//## begin module%38DB62A6038A.cp preserve=no
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
//## end module%38DB62A6038A.cp

//## Module: CommTask%38DB62A6038A; Package body
//## Subsystem: M4Communication::M4COML::src%38D1FCE80326
//## Source file: e:\mybuild\m4communication\src\commtask.cpp

//## begin module%38DB62A6038A.additionalIncludes preserve=no
//## end module%38DB62A6038A.additionalIncludes

//## begin module%38DB62A6038A.includes preserve=yes
#include <m4trace.hpp>
#include <communicationlayer.hpp>
#include <clarraylibres.hpp>
#include <commproccessaction.hpp>
//## end module%38DB62A6038A.includes

// commbase
#include <commbase.hpp>
// m4types
#include <m4types.hpp>
#include <basiclog.hpp>
// m4string
#include <m4string.hpp>
// stdio
#include <stdio.h>
// CommBin
#include <commbin.hpp>
// commeventhandler
#include <commeventhandler.hpp>
// commprotocols
#include <commprotocols.hpp>
// CommTask
#include <commtask.hpp>
// iexecutable
#include <iexecutable.hpp>
// CommComun
#include <commcomun.hpp>
// ParamList
#include <paramlist.hpp>
// basepdu
#include <basepdus.hpp>
#include "m4srvres.hpp"

class ClCommAction;

//## begin module%38DB62A6038A.declarations preserve=no
//## end module%38DB62A6038A.declarations

//## begin module%38DB62A6038A.additionalDeclarations preserve=yes
#include <commactionsname.h>
#include <commaction.hpp>
#include <iexecutable.hpp>

#define M4_SRV_TRACE_COMP_ID "COMM"
#include <m4srvtrace.h>
char *g_pcStateNames[]={"NONE","ACCEPTING","ACCEPTED","ERROR_ACCEPTING","WAITING_FOR_READING","READING","PAUSE_READING","START_WRITTING","WRITTING","CLOSING","OPENING","ERROR_OPENING","CONNECTING","CONNECTED","NO_CONNECTED","FINISHING_WRITTING","FINISH_ALL","DETECTED_CLOSED","REQUEST_PENDING","DETECTED_CLOSED_AND_REQUEST_PENDING"};

#ifdef _UNIX
#   include <netinet/in.h>
#endif

//## end module%38DB62A6038A.additionalDeclarations


// Class ClCommTask 












ClCommTask::ClCommTask (m4int_t ai_iRecycleObjID)
  //## begin ClCommTask::ClCommTask%955103966.hasinit preserve=no
      : m_pParamList(NULL), m_pCommProtocol(NULL), m_pExecutableInterface(NULL), m_poCondition(NULL), m_TLIID(0), m_hConnectionID(0)
  //## end ClCommTask::ClCommTask%955103966.hasinit
  //## begin ClCommTask::ClCommTask%955103966.initialization preserve=yes
  //## end ClCommTask::ClCommTask%955103966.initialization
{
  //## begin ClCommTask::ClCommTask%955103966.body preserve=yes
	m_State=NONE;
	m_poCommEventHandler=new ClCommEventHandler;
	m_poCommEventHandler->m_poTask=this;
  //## end ClCommTask::ClCommTask%955103966.body
}


ClCommTask::~ClCommTask ()
{
  //## begin ClCommTask::~ClCommTask%955103967.body preserve=yes
	if(NULL!=m_poCommEventHandler)
		delete m_poCommEventHandler;

	if(NULL!=m_pParamList)
		delete m_pParamList ;

  //## end ClCommTask::~ClCommTask%955103967.body
}



//## Other Operations (implementation)
m4tliid_t ClCommTask::GetTLIID ()
{
  //## begin ClCommTask::GetTLIID%955613879.body preserve=yes
	return  m_TLIID;
  //## end ClCommTask::GetTLIID%955613879.body
}

ClCommEventHandler* ClCommTask::GetCommEventHandler ()
{
  //## begin ClCommTask::GetCommEventHandler%955613880.body preserve=yes
	return m_poCommEventHandler;
  //## end ClCommTask::GetCommEventHandler%955613880.body
}

m4return_t ClCommTask::SetState (eCommTaskState ai_eState)
{
  //## begin ClCommTask::SetState%955613889.body preserve=yes
	m4return_t iRet=M4_SUCCESS;
	if(m_State ==CLOSING)
	{
		iRet=M4_ERROR;
	}
	if(m_State ==ai_eState)
		iRet=M4_WARNING;
	m_State= ai_eState;
	return iRet;
  //## end ClCommTask::SetState%955613889.body
}

eCommTaskState ClCommTask::GetState ()
{
  //## begin ClCommTask::GetState%956672702.body preserve=yes
	return m_State;
  //## end ClCommTask::GetState%956672702.body
}

m4return_t ClCommTask::SetParams (ClParamList *ai_pParamList)
{
  //## begin ClCommTask::SetParams%957429935.body preserve=yes
	if(NULL!=m_pParamList)
		delete m_pParamList;
	m_pParamList=ai_pParamList;
	return M4_SUCCESS;
  //## end ClCommTask::SetParams%957429935.body
}

m4return_t ClCommTask::RemoveParams ()
{
  //## begin ClCommTask::RemoveParams%958463331.body preserve=yes
	m_pParamList=NULL;
	return M4_SUCCESS;
  //## end ClCommTask::RemoveParams%958463331.body
}

m4return_t ClCommTask::SetCondition (ClCommCondition *ai_poCondition)
{
  //## begin ClCommTask::SetCondition%965219980.body preserve=yes
	m_poCondition=ai_poCondition;
	return M4_SUCCESS;
  //## end ClCommTask::SetCondition%965219980.body
}

ClCommCondition * ClCommTask::GetCondition ()
{
  //## begin ClCommTask::GetCondition%965219981.body preserve=yes
	return m_poCondition;
  //## end ClCommTask::GetCondition%965219981.body
}

m4return_t ClCommTask::SetActionName (m4pchar_t ai_szActionName)
{
  //## begin ClCommTask::SetActionName%965219982.body preserve=yes
	m_strActionName=ai_szActionName;
	return M4_SUCCESS;
  //## end ClCommTask::SetActionName%965219982.body
}

m4pcchar_t ClCommTask::GetActionName ()
{
  //## begin ClCommTask::GetActionName%965219983.body preserve=yes
	return m_strActionName.c_str();
  //## end ClCommTask::GetActionName%965219983.body
}

m4return_t ClCommTask::SetExecutable (ClExecutableInterface *ai_pExecutable)
{
  //## begin ClCommTask::SetExecutable%965219984.body preserve=yes
	m_pExecutableInterface=ai_pExecutable;
	return M4_SUCCESS;
  //## end ClCommTask::SetExecutable%965219984.body
}

ClExecutableInterface * ClCommTask::GetExecutable ()
{
  //## begin ClCommTask::GetExecutable%965219985.body preserve=yes
	return m_pExecutableInterface;
  //## end ClCommTask::GetExecutable%965219985.body
}

m4return_t ClCommTask::SetCommProtocol (ClCommProtocol *ai_pCommProtocol)
{
  //## begin ClCommTask::SetCommProtocol%965284296.body preserve=yes
	m_pCommProtocol=ai_pCommProtocol;
	return M4_SUCCESS;
  //## end ClCommTask::SetCommProtocol%965284296.body
}

m4return_t ClCommTask::SetTLIID (m4tliid_t ai_TLIID)
{
  //## begin ClCommTask::SetTLIID%965731402.body preserve=yes
	m_TLIID=ai_TLIID;
	return M4_SUCCESS;
  //## end ClCommTask::SetTLIID%965731402.body
}

m4return_t ClCommTask::Recycle ()
{
  //## begin ClCommTask::Recycle%965834586.body preserve=yes
	if (m_pParamList)
		delete m_pParamList;
	m_pCommProtocol=NULL;
	m_pExecutableInterface=NULL;
	m_poCondition=NULL;
	m_hConnectionID=0;
	m_strActionName="";
	m_TLIID=0;
	return M4_SUCCESS;
  //## end ClCommTask::Recycle%965834586.body
}

m4return_t ClCommTask::SetConnectionID (m4handle_t ai_hConnectionID)
{
  //## begin ClCommTask::SetConnectionID%965898337.body preserve=yes
	m_hConnectionID=ai_hConnectionID;
	return M4_SUCCESS;
  //## end ClCommTask::SetConnectionID%965898337.body
}

m4handle_t ClCommTask::GetConnectionID ()
{
  //## begin ClCommTask::GetConnectionID%965898338.body preserve=yes
	return m_hConnectionID;
  //## end ClCommTask::GetConnectionID%965898338.body
}

m4return_t ClCommTask::_PassReturnInfo (ClCommAction* ai_pAction)
{
  //## begin ClCommTask::_PassReturnInfo%966411265.body preserve=yes
	ai_pAction->SetCondition(m_poCondition);
	ai_pAction->SetExecutable(m_pExecutableInterface);
	ai_pAction->SetActionName((char *)m_strActionName.c_str());
	return M4_SUCCESS;
  //## end ClCommTask::_PassReturnInfo%966411265.body
}

// Additional Declarations
  //## begin ClCommTask%38D9E0840037.declarations preserve=yes
m4return_t ClCommTask::Notify(ClParamList *ai_poParams)
{
	ClExecutableInterface *theExecutor=GetExecutable();	
	m4return_t iRet=M4_SUCCESS;
	if(NULL==theExecutor)
	{
		ClCommCondition *theCondition=GetCondition();
		if(NULL!=theCondition)
        {
            SetCondition(NULL);
			theCondition->Signal();
        }
		else
		{m4TraceLevel(2,cerr <<"Nobody waiting for this action"<<endl);}

	}
	else
	{
		m4pchar_t pcActionName=(char *)	GetActionName();
		iRet=theExecutor->ExecuteAction(pcActionName,ai_poParams);
	}
	return iRet;
}
  //## end ClCommTask%38D9E0840037.declarations
// Class ClCommReadingTask 







ClCommReadingTask::ClCommReadingTask ()
  //## begin ClCommReadingTask::ClCommReadingTask%955103968.hasinit preserve=no
      : m_pcReadedHTTPHeader(NULL)
  //## end ClCommReadingTask::ClCommReadingTask%955103968.hasinit
  //## begin ClCommReadingTask::ClCommReadingTask%955103968.initialization preserve=yes
  ,ClCommTask(M4_COMM_READING_TASK)
  //## end ClCommReadingTask::ClCommReadingTask%955103968.initialization
{
  //## begin ClCommReadingTask::ClCommReadingTask%955103968.body preserve=yes
	m_pcBuffer=new m4char_t[M4_COMM_DEFAULT_BUFFER_SIZE + M4_COMM_DEFAULT_BUFFER_MARGIN_SIZE];
	m_poCommEventHandler->SetBuffer(m_pcBuffer);
	m_BufferSize=M4_COMM_DEFAULT_BUFFER_SIZE;
	m_poCommEventHandler->SetBufferSize(m_BufferSize);
	m_Processed=0;
	m_Readed =0;
  //## end ClCommReadingTask::ClCommReadingTask%955103968.body
}


ClCommReadingTask::~ClCommReadingTask ()
{
  //## begin ClCommReadingTask::~ClCommReadingTask%955103969.body preserve=yes
	if (m_pcBuffer)
		delete m_pcBuffer;
  //## end ClCommReadingTask::~ClCommReadingTask%955103969.body
}



//## Other Operations (implementation)
m4return_t ClCommReadingTask::Readed (m4int_t ai_iReadedBytes)
{
  //## begin ClCommReadingTask::Readed%957517148.body preserve=yes
	m_Readed+=ai_iReadedBytes;

	m4uint_t iUsados=0;
	m4uint_t uiNextBlockSize=0;
	m4bool_t bDataAvailable=M4_TRUE;
	if( (GetState()==REQUEST_PENDING) && m_pCommProtocol && (m_pCommProtocol->GetCommProtocolType ()==M4_COMM_PDU_PROTOCOL))
	{
		ClPDU * thePDU;
		((ClCommPDUProtocol *)m_pCommProtocol)->GetReadedPDU(thePDU);
		if(thePDU!=NULL)
		{
			ClCommRemainReadedAction *poAction =new ClCommRemainReadedAction ;
			poAction ->m_poTask=this;
			ClCommunicationImplementation::GetInstance()->Push(poAction);
			return M4_ERROR;	// Para que la capa de trasnporte deje de leer
		}
	}
	m4return_t iRet=m_pCommProtocol->AddChunk(m_pcBuffer,m_Readed,iUsados,uiNextBlockSize);
	switch(iRet)
	{
	case M4_COMM_PROTOCOL_RETURN_CONTINUE_READING:
		m_pCommProtocol->DumpRead(m_pcBuffer,iUsados);
		if(iUsados != 0) // NOS PULIMOS LO USADO
		{
			m_Readed -= iUsados;
			memmove(m_pcBuffer,m_pcBuffer+iUsados,m_Readed);
		}
		
		if(uiNextBlockSize>m_BufferSize) // VEMOS SI NOS CABE EL SIGUIENTE TROZO
		{
//			M4_ASSERT(uiNextBlockSize!=((m4uint32_t)-1));
			m_BufferSize=uiNextBlockSize;
			char *pcNewBuffer=new char[uiNextBlockSize + M4_COMM_DEFAULT_BUFFER_MARGIN_SIZE];
			memcpy(pcNewBuffer,m_pcBuffer,m_Readed);
			delete m_pcBuffer;
			m_pcBuffer=pcNewBuffer;
		}
		
		m_poCommEventHandler->SetBuffer(m_pcBuffer+m_Readed);
		m_poCommEventHandler->SetBufferSize(m_BufferSize-m_Readed);
		
		return M4_SUCCESS;
		break;
	case M4_COMM_PROTOCOL_RETURN_ERROR:
		m_pCommProtocol->DumpRead(m_pcBuffer,m_Readed);
		m_pCommProtocol->DisableReadDump();
		CloseSocket(M4_TRUE);
		return M4_ERROR;
		break;
	case M4_COMM_PROTOCOL_RETURN_COMPLETED:
		m_pCommProtocol->DumpRead(m_pcBuffer,iUsados);
		m_pCommProtocol->DisableReadDump();

		if(iUsados != 0) // NOS PULIMOS LO USADO
		{
			m_Readed -= iUsados;
			memmove(m_pcBuffer,m_pcBuffer+iUsados,m_Readed);
		}
		
	
		m_poCommEventHandler->SetBuffer(m_pcBuffer+m_Readed);
		m_poCommEventHandler->SetBufferSize(m_BufferSize-m_Readed);

/*		m_poCommEventHandler->SetBuffer(m_pcBuffer);
		m_poCommEventHandler->SetBufferSize(m_BufferSize);
		m_Processed=0;
		m_Readed=0;
*/		
		// eh 311002
		if(m_Readed > 0)
		{
			m4uint16_t iFirst = 0, iSecond = 0;
			ClPDU* pPDU = NULL;

			( ( ClCommPDUProtocol* )m_pCommProtocol )->GetReadedPDU( pPDU );

			if( pPDU )
			{
				iFirst = pPDU->GetIdPDU();
			}

			if( m_pcBuffer && m_Readed > 2 )
			{
				iSecond = ntohs( *( ( m4uint16_t* )m_pcBuffer ) );
			}

			M4_SRV_ERROR( M4_SRV_READED_SEVERAL_PDUS, "Error de protocolo.\n \nSe han recibido dos peticiones consecutivas de tipos %0:d y %1:d en la conexión %2:d.", iFirst << iSecond << m_hConnectionID );
		}

		PrepareNotifyReaded();

		// Bg 125975
		if(m_Readed > 0)
		{
			// Retornamos warning para que nos llamen de nuevo
			// y así procesar lo que queda pendiente.
			return M4_WARNING;
		}

		return M4_ERROR;
		//return M4_ERROR; // Devuelvo esto para que no siga leyendo

		break;
	default:
		M4_SRV_DEBUG_N("Unkown Value!!!  in ClCommReadingTask::Readed");
		return M4_ERROR;
	}

  //## end ClCommReadingTask::Readed%957517148.body
}

m4return_t ClCommReadingTask::CloseSocket (m4bool_t ai_bExternalClose)
{
  //## begin ClCommReadingTask::CloseSocket%959072020.body preserve=yes
//	ClCommCloseConnectionAction *theAction= new ClCommCloseConnectionAction ;
//	theAction->PrepareTask(m_hConnectionID,m_TLIID,(ClCommProtocol *)ai_bExternalClose);
	
	if(ai_bExternalClose)
	{
		if(GetState()!=CLOSING)
		{
			ClCommProccessExternalCloseAction *theAction= new ClCommProccessExternalCloseAction (GetTLIID(),M4_ERROR,this) ;
			if(GetState()!=REQUEST_PENDING)
				SetState(DETECTED_CLOSED);
			else
				SetState(DETECTED_CLOSED_AND_REQUEST_PENDING);
			theAction->PrepareTask(m_hConnectionID,NULL,NULL);
			theAction->SetCondition(m_poCondition);
			theAction->SetExecutable(m_pExecutableInterface);
			theAction->SetActionName((char *)m_strActionName.c_str());
			ClCommunicationImplementation::GetInstance()->Push(theAction);
		}
		else
			M4_SRV_DEBUG_N("forget it");
		return M4_SUCCESS;
	}
	else
	{
		ClCommCloseConnectionAction *theAction=new ClCommCloseConnectionAction  ();
		theAction->PrepareTask(m_hConnectionID,m_TLIID,(ClCommProtocol *)ai_bExternalClose);
		theAction->SetCondition(m_poCondition);
		theAction->SetExecutable(m_pExecutableInterface);
		theAction->SetActionName((char *)m_strActionName.c_str());
		ClCommunicationImplementation::GetInstance()->Push(theAction);
		return M4_SUCCESS;
	}


  //## end ClCommReadingTask::CloseSocket%959072020.body
}

m4return_t ClCommReadingTask::NotifyReaded ()
{
  //## begin ClCommReadingTask::NotifyReaded%966517539.body preserve=yes
	if(NULL==m_pParamList)
	{
		m_pParamList=new ClParamList;
		m_pParamList->SetInt(M4_PARAM_CONNECTIONID,m_hConnectionID);
		m_pParamList->SetInt(M4_PARAM_SESSIONID,m_hConnectionID);
	}
	else
	{
		m_pParamList->DeleteParam(M4_PARAM_RETURN_RESULT);
		m_pParamList->DeleteParam(M4_PARAM_FINISH_READING_TIME);
		m_pParamList->DeleteParam(M4_PARAM_START_READING_TIME);
	}

	m_pParamList->SetM4Return(M4_PARAM_RETURN_RESULT ,M4_SUCCESS);
	m_pParamList->SetInt64(M4_PARAM_FINISH_READING_TIME,m_pCommProtocol->GetFinishReadingTime());
	m_pParamList->SetInt64(M4_PARAM_START_READING_TIME,m_pCommProtocol->GetStartReadingTime());

	ClPDU *thePDU;
	switch(m_pCommProtocol->GetCommProtocolType())
	{
	case M4_COMM_PDU_HTTP_PROTOCOL:
		if(M4_ERROR==((ClCommPDUProtocol *)m_pCommProtocol)->GetReadedPDU(thePDU))
		{
			M4_SRV_DEBUG_N( "Invalid Readed PDU!!!!");
			return M4_ERROR;
		}
		if(thePDU->GetVersion()==0)
			m_pCommProtocol->SetVersion(0);
		m_pParamList->DeleteParam(M4_PARAM_READED_PDU);
		m_pParamList ->SetPointer(M4_PARAM_READED_PDU,(void *)thePDU);
		m_pParamList->DeleteParam(M4_PARAM_READED_PDU_HEADERS);
		m_pParamList ->SetString(M4_PARAM_READED_PDU_HEADERS,((ClCommPDUHTTPProtocol *)m_pCommProtocol)->GetReadedHeaders());
	break;
	case M4_COMM_PDU_PROTOCOL:
		if(M4_ERROR==((ClCommPDUProtocol *)m_pCommProtocol)->GetReadedPDU(thePDU))
		{
			M4_SRV_DEBUG_N("Invalid Readed PDU!!!!");
			return M4_ERROR;
		}
	//	if(thePDU->GetVersion()==0)
	//		m_pCommProtocol->SetVersion(0);
		m_pParamList->DeleteParam(M4_PARAM_READED_PDU);
		m_pParamList ->SetPointer(M4_PARAM_READED_PDU,(void *)thePDU);
	break;
	case M4_COMM_ASCII_PROTOCOL:
		m_pParamList->DeleteParam(M4_PARAM_READED_BUFFER);
		m_pParamList->SetPointer(M4_PARAM_READED_BUFFER,(void *)((ClCommASCIIProtocol *)m_pCommProtocol)->GetReadedBuffer());
	break;
	}
	m_pCommProtocol->ReleaseContents();

	

	m4return_t 	ret=Notify(m_pParamList);

	if(M4_SUCCESS==ret)
	{
		eCommTaskState theState=GetState();
		if((theState!=CLOSING) && (theState!=DETECTED_CLOSED) && (theState!=DETECTED_CLOSED_AND_REQUEST_PENDING))
		{
			SetState(NONE);
			//  BeginRead(m4tliid_t ai_Identifier, ClIOEventHandler *ai_pIOEventHandler);
			ClCommunicationImplementation::GetInstance()->GetCTL().BeginRead(GetTLIID(),GetCommEventHandler());
		}
		else
		{
			if(theState==DETECTED_CLOSED_AND_REQUEST_PENDING)
				SetState(DETECTED_CLOSED);
		}
	}

return M4_SUCCESS;
  //## end ClCommReadingTask::NotifyReaded%966517539.body
}

// Additional Declarations
  //## begin ClCommReadingTask%38DB4DEC00E4.declarations preserve=yes
void ClCommReadingTask::PrepareNotifyReaded()
{
	if(GetState()!=DETECTED_CLOSED)
		SetState(REQUEST_PENDING);
	else
		SetState(DETECTED_CLOSED_AND_REQUEST_PENDING);
	ClCommReadedAction *theAction=new ClCommReadedAction;
	theAction->m_poTask=this;
	if(m_pParamList)
	{
		m_pParamList->DeleteParam(M4_PARAM_COMM_QUEUED_READED_TIME);
		m_pParamList->SetInt64(M4_PARAM_COMM_QUEUED_READED_TIME,ClPrecisionTicker::GetTimeTick());
	}
	ClCommunicationImplementation::GetInstance()->Push(theAction);
}
  //## end ClCommReadingTask%38DB4DEC00E4.declarations
// Class ClCommWrittingTask 







ClCommWrittingTask::ClCommWrittingTask ()
  //## begin ClCommWrittingTask::ClCommWrittingTask%955103970.hasinit preserve=no
      : m_pcWrittingBuffer(NULL), m_pcWriteHTTPHeaders(NULL)
  //## end ClCommWrittingTask::ClCommWrittingTask%955103970.hasinit
  //## begin ClCommWrittingTask::ClCommWrittingTask%955103970.initialization preserve=yes
  ,ClCommTask(M4_COMM_WRITTING_TASK)
  //## end ClCommWrittingTask::ClCommWrittingTask%955103970.initialization
{
  //## begin ClCommWrittingTask::ClCommWrittingTask%955103970.body preserve=yes
  //## end ClCommWrittingTask::ClCommWrittingTask%955103970.body
}


ClCommWrittingTask::~ClCommWrittingTask ()
{
  //## begin ClCommWrittingTask::~ClCommWrittingTask%955103971.body preserve=yes
  //## end ClCommWrittingTask::~ClCommWrittingTask%955103971.body
}



//## Other Operations (implementation)
m4return_t ClCommWrittingTask::SetBuffer (m4pchar_t ai_pcBuffer, m4uint_t ai_uiBufferSize)
{
  //## begin ClCommWrittingTask::SetBuffer%957517147.body preserve=yes
	if(ai_pcBuffer && (ai_uiBufferSize>0))
	{
		m_poCommEventHandler->SetBuffer(ai_pcBuffer);
		m_poCommEventHandler->SetBufferSize(ai_uiBufferSize);
		return M4_SUCCESS;
	}
	else
	{
		M4_SRV_DEBUG_N("Buffer de escritura no valido");
		return M4_ERROR;
	}
  //## end ClCommWrittingTask::SetBuffer%957517147.body
}

m4return_t ClCommWrittingTask::Written (m4return_t ai_retValue)
{
  //## begin ClCommWrittingTask::Written%957944570.body preserve=yes

	if( (M4_SUCCESS==ai_retValue) && (GetState()!=FINISHING_WRITTING) )
		PrepareWrite();			// Dentro se puede cambiar el estado a FINISHING_WRITTING
		
	if ((GetState()==FINISHING_WRITTING) || (M4_SUCCESS!=ai_retValue) )
	{
		m_pCommProtocol->DisableWriteDump();
		m4TraceLevel(2,cout << "wt "<<flush);
		m4bool_t bDelParam=M4_TRUE;
		if(m_pParamList)
			bDelParam=M4_FALSE;

		
		PrepareReturnParams(ai_retValue);
		if(!bDelParam)
			m_pParamList->SetInt("DeleteParam",1);
		ClParamList * theParams=m_pParamList;
		RemoveParams();
		Notify(theParams);
		if(bDelParam)
			delete theParams;
		if(M4_SUCCESS==ai_retValue)
			SetState(NONE);
		else
			SetState(DETECTED_CLOSED);
	}
	return M4_SUCCESS;
  //## end ClCommWrittingTask::Written%957944570.body
}

m4return_t ClCommWrittingTask::PrepareWrite ()
{
  //## begin ClCommWrittingTask::PrepareWrite%965284295.body preserve=yes
	m4pchar_t pcChunk;
	m4int_t iChunkSize;
	m4return_t iRet;

	m4int_t iProtocol=0;
	switch (GetState())
	{
	case START_WRITTING:
		{
			iProtocol=m_pCommProtocol->GetCommProtocolType();

			switch(iProtocol)
			{
			case M4_COMM_PDU_HTTP_PROTOCOL:
				((ClCommPDUHTTPProtocol *)m_pCommProtocol)->SetWriteHeaders(m_pcWriteHTTPHeaders);
			case M4_COMM_PDU_PROTOCOL:
				((ClCommPDUProtocol *)m_pCommProtocol)->SetWritePDU(m_poPDU);
				m_pCommProtocol->Initialize();
				SetState(WRITTING);
				break;
			case M4_COMM_ASCII_PROTOCOL:
				GetBufferToWrite(pcChunk,iChunkSize);
				((ClCommASCIIProtocol *)m_pCommProtocol)->SetWriteBuffer(pcChunk,iChunkSize);
				m_pCommProtocol->Initialize();
				SetState(FINISHING_WRITTING);
				break;
			default:
				M4_SRV_DEBUG("Invalid CommProtocol %0",iProtocol);
					M4_ASSERT(0);
				return M4_ERROR;
				break;
			}
		}
		break;
	case WRITTING:
		break;
	case FINISHING_WRITTING:
		m4Trace(cerr << "Invalid state:FINISHING_WRITTING"<<endl);
		return M4_ERROR;
		break;
	default:
		M4_ASSERT(0);
		break;
	}

	unsigned int thechunksize = 0;
	iRet=m_pCommProtocol->GetNextChunk(pcChunk,thechunksize);
	
	iChunkSize=(int)thechunksize;

	if(M4_SUCCESS==iRet)
	{
		m_pCommProtocol->DumpWrite(pcChunk,thechunksize);

		SetBuffer(pcChunk,iChunkSize);
		m4TraceLevel(3,cout<< GetConnectionID()<<" WrittingChunk:"<< iChunkSize <<endl);
  //      m4return_t Write (m4tliid_t ai_Identifier, ClIOEventHandler *ai_pIOEventHandler);
		ClCommunicationImplementation::GetInstance()->GetCTL().Write(m_TLIID,GetCommEventHandler());
		return M4_SUCCESS;
	}
	else
	{
		m_pCommProtocol->DisableWriteDump();
		SetState(FINISHING_WRITTING);
	//	Written(M4_SUCCESS);
		return M4_SUCCESS;
	}

  //## end ClCommWrittingTask::PrepareWrite%965284295.body
}

m4return_t ClCommWrittingTask::SetRequestID (m4uint64_t ai_ui64RequestID)
{
  //## begin ClCommWrittingTask::SetRequestID%966411259.body preserve=yes
	m_ui64RequestID=ai_ui64RequestID;
	return M4_SUCCESS;
  //## end ClCommWrittingTask::SetRequestID%966411259.body
}

m4uint64_t ClCommWrittingTask::GetRequestID ()
{
  //## begin ClCommWrittingTask::GetRequestID%966411260.body preserve=yes
	return m_ui64RequestID;
  //## end ClCommWrittingTask::GetRequestID%966411260.body
}

m4return_t ClCommWrittingTask::SetPDU (ClPDU *ai_pPDU)
{
  //## begin ClCommWrittingTask::SetPDU%966411261.body preserve=yes
	m_poPDU=ai_pPDU;
	return M4_SUCCESS;
  //## end ClCommWrittingTask::SetPDU%966411261.body
}

ClPDU * ClCommWrittingTask::GetPDU ()
{
  //## begin ClCommWrittingTask::GetPDU%966411262.body preserve=yes
	return m_poPDU;
  //## end ClCommWrittingTask::GetPDU%966411262.body
}

m4return_t ClCommWrittingTask::SetBufferToWrite (m4pchar_t ai_pcBuffer, m4int_t ai_iBufferSize)
{
  //## begin ClCommWrittingTask::SetBufferToWrite%966411263.body preserve=yes
	m_pcWrittingBuffer=ai_pcBuffer;
	m_iBufferSize=ai_iBufferSize;
	return M4_SUCCESS;
  //## end ClCommWrittingTask::SetBufferToWrite%966411263.body
}

m4return_t ClCommWrittingTask::GetBufferToWrite (m4pchar_t &ai_pcBuffer, m4int_t &ai_iBufferSize)
{
  //## begin ClCommWrittingTask::GetBufferToWrite%966411264.body preserve=yes
	ai_pcBuffer=m_pcWrittingBuffer;
	ai_iBufferSize=m_iBufferSize;
	return M4_SUCCESS;
  //## end ClCommWrittingTask::GetBufferToWrite%966411264.body
}

m4return_t ClCommWrittingTask::SetHeaders (m4pchar_t ai_pcHeaders)
{
  //## begin ClCommWrittingTask::SetHeaders%972554300.body preserve=yes
	m_pcWriteHTTPHeaders=ai_pcHeaders;
	return M4_SUCCESS;
  //## end ClCommWrittingTask::SetHeaders%972554300.body
}

m4pchar_t ClCommWrittingTask::GetHeaders ()
{
  //## begin ClCommWrittingTask::GetHeaders%972554301.body preserve=yes
	return m_pcWriteHTTPHeaders;
  //## end ClCommWrittingTask::GetHeaders%972554301.body
}

// Additional Declarations
  //## begin ClCommWrittingTask%38DB4FA00362.declarations preserve=yes
m4return_t ClCommWrittingTask::PrepareReturnParams(m4return_t ai_retValue)
{
		if(m_pParamList==NULL)
		{
			m_pParamList=new ClParamList;
			m_pParamList->SetInt(M4_PARAM_CONNECTIONID,m_hConnectionID);
			m_pParamList->SetInt(M4_PARAM_SESSIONID,m_hConnectionID);
		}
		else
		{
			m_pParamList->DeleteParam(M4_PARAM_PETITION_ID);
			m_pParamList->DeleteParam(M4_PARAM_FINISH_WRITTING_TIME);
			m_pParamList->DeleteParam(M4_PARAM_START_WRITTING_TIME);
			m_pParamList->DeleteParam(M4_PARAM_START_PROCCESSING_WRITTING_TIME);
			m_pParamList->DeleteParam(M4_PARAM_RETURN_RESULT);
		}	

		m_pParamList->SetInt64(M4_PARAM_PETITION_ID,(m4int64_t)m_ui64RequestID);
		m_pParamList->SetInt64(M4_PARAM_START_PROCCESSING_WRITTING_TIME ,m_pCommProtocol->GetStartProccessingWrittingTime());
		m_pParamList->SetInt64(M4_PARAM_START_WRITTING_TIME ,m_pCommProtocol->GetStartWrittingTime());
		m_pParamList->SetInt64(M4_PARAM_FINISH_WRITTING_TIME,m_pCommProtocol->GetFinishWrittingTime());
		m_pParamList->SetM4Return(M4_PARAM_RETURN_RESULT,ai_retValue);
		return M4_SUCCESS;
	
}
  //## end ClCommWrittingTask%38DB4FA00362.declarations
// Class ClCommOpenPortTask 






ClCommOpenPortTask::ClCommOpenPortTask ()
  //## begin ClCommOpenPortTask::ClCommOpenPortTask%955103972.hasinit preserve=no
      : m_iPort(0), m_pcCiphers(NULL), m_pcSSLCert(NULL)
  //## end ClCommOpenPortTask::ClCommOpenPortTask%955103972.hasinit
  //## begin ClCommOpenPortTask::ClCommOpenPortTask%955103972.initialization preserve=yes
  ,ClCommTask(M4_COMM_OPENPORT_TASK),m_pcPrivKey(NULL)
  //## end ClCommOpenPortTask::ClCommOpenPortTask%955103972.initialization
{
  //## begin ClCommOpenPortTask::ClCommOpenPortTask%955103972.body preserve=yes
  //## end ClCommOpenPortTask::ClCommOpenPortTask%955103972.body
}


ClCommOpenPortTask::~ClCommOpenPortTask ()
{
  //## begin ClCommOpenPortTask::~ClCommOpenPortTask%955103973.body preserve=yes
  //## end ClCommOpenPortTask::~ClCommOpenPortTask%955103973.body
}



//## Other Operations (implementation)
m4return_t ClCommOpenPortTask::SetPort (m4int_t ai_iPort)
{
  //## begin ClCommOpenPortTask::SetPort%966411246.body preserve=yes
	m_iPort=ai_iPort;
	return M4_SUCCESS;
  //## end ClCommOpenPortTask::SetPort%966411246.body
}

m4return_t ClCommOpenPortTask::SetCommProtocolType (m4int_t ai_iProtocolType)
{
  //## begin ClCommOpenPortTask::SetCommProtocolType%966411247.body preserve=yes
	m_iProtocolType=ai_iProtocolType;
	return M4_SUCCESS;
  //## end ClCommOpenPortTask::SetCommProtocolType%966411247.body
}

m4int_t ClCommOpenPortTask::GetPort ()
{
  //## begin ClCommOpenPortTask::GetPort%966411248.body preserve=yes
	return m_iPort;
  //## end ClCommOpenPortTask::GetPort%966411248.body
}

m4return_t ClCommOpenPortTask::SetSSLParams (m4pchar_t ai_strSSLPrivKey, m4pchar_t ai_strSSLCert, m4pchar_t ai_strSSLCipher)
{
  //## begin ClCommOpenPortTask::SetSSLParams%966411250.body preserve=yes
	m_pcCiphers=ai_strSSLCipher;
	m_pcPrivKey=ai_strSSLPrivKey;
	m_pcSSLCert=ai_strSSLCert;
	return M4_SUCCESS;
  //## end ClCommOpenPortTask::SetSSLParams%966411250.body
}

m4return_t ClCommOpenPortTask::GetSSLParams (m4pchar_t& ai_strSSLPrivKey, m4pchar_t &ai_strSSLCert, m4pchar_t &ai_strSSLCipher)
{
  //## begin ClCommOpenPortTask::GetSSLParams%966411251.body preserve=yes
	ai_strSSLCipher=m_pcCiphers;
	ai_strSSLPrivKey=m_pcPrivKey;
	ai_strSSLCert=m_pcSSLCert;
	return M4_SUCCESS;
  //## end ClCommOpenPortTask::GetSSLParams%966411251.body
}

// Additional Declarations
  //## begin ClCommOpenPortTask%38DB51640073.declarations preserve=yes
  //## end ClCommOpenPortTask%38DB51640073.declarations

// Class ClCommCloseSocketTask 

ClCommCloseSocketTask::ClCommCloseSocketTask ()
  //## begin ClCommCloseSocketTask::ClCommCloseSocketTask%955103974.hasinit preserve=no
  //## end ClCommCloseSocketTask::ClCommCloseSocketTask%955103974.hasinit
  //## begin ClCommCloseSocketTask::ClCommCloseSocketTask%955103974.initialization preserve=yes
  :ClCommTask(M4_COMM_CLOSESOCKET_TASK)
  //## end ClCommCloseSocketTask::ClCommCloseSocketTask%955103974.initialization
{
  //## begin ClCommCloseSocketTask::ClCommCloseSocketTask%955103974.body preserve=yes
  //## end ClCommCloseSocketTask::ClCommCloseSocketTask%955103974.body
}


ClCommCloseSocketTask::~ClCommCloseSocketTask ()
{
  //## begin ClCommCloseSocketTask::~ClCommCloseSocketTask%955103975.body preserve=yes
  //## end ClCommCloseSocketTask::~ClCommCloseSocketTask%955103975.body
}


// Additional Declarations
  //## begin ClCommCloseSocketTask%38DB51740184.declarations preserve=yes
  //## end ClCommCloseSocketTask%38DB51740184.declarations

// Class ClCommClosePortTask 

ClCommClosePortTask::ClCommClosePortTask ()
  //## begin ClCommClosePortTask::ClCommClosePortTask%955103976.hasinit preserve=no
  //## end ClCommClosePortTask::ClCommClosePortTask%955103976.hasinit
  //## begin ClCommClosePortTask::ClCommClosePortTask%955103976.initialization preserve=yes
  :ClCommTask(M4_COMM_CLOSEPORT_TASK)
  //## end ClCommClosePortTask::ClCommClosePortTask%955103976.initialization
{
  //## begin ClCommClosePortTask::ClCommClosePortTask%955103976.body preserve=yes
  //## end ClCommClosePortTask::ClCommClosePortTask%955103976.body
}


ClCommClosePortTask::~ClCommClosePortTask ()
{
  //## begin ClCommClosePortTask::~ClCommClosePortTask%955103977.body preserve=yes
  //## end ClCommClosePortTask::~ClCommClosePortTask%955103977.body
}


// Additional Declarations
  //## begin ClCommClosePortTask%38DB51940144.declarations preserve=yes
  //## end ClCommClosePortTask%38DB51940144.declarations

// Class ClCommOpeningSocketTask 





ClCommOpeningSocketTask::ClCommOpeningSocketTask ()
  //## begin ClCommOpeningSocketTask::ClCommOpeningSocketTask%955103978.hasinit preserve=no
      : m_pcHost(NULL), m_iProtocolType(0), m_pcSSLCert(NULL)
  //## end ClCommOpeningSocketTask::ClCommOpeningSocketTask%955103978.hasinit
  //## begin ClCommOpeningSocketTask::ClCommOpeningSocketTask%955103978.initialization preserve=yes
  ,ClCommTask(M4_COMM_CLOSESOCKET_TASK)
  //## end ClCommOpeningSocketTask::ClCommOpeningSocketTask%955103978.initialization
{
  //## begin ClCommOpeningSocketTask::ClCommOpeningSocketTask%955103978.body preserve=yes
  //## end ClCommOpeningSocketTask::ClCommOpeningSocketTask%955103978.body
}


ClCommOpeningSocketTask::~ClCommOpeningSocketTask ()
{
  //## begin ClCommOpeningSocketTask::~ClCommOpeningSocketTask%955103979.body preserve=yes
  //## end ClCommOpeningSocketTask::~ClCommOpeningSocketTask%955103979.body
}



//## Other Operations (implementation)
m4return_t ClCommOpeningSocketTask::SetCommProtocolType (m4int_t ai_iCommProtocolType)
{
  //## begin ClCommOpeningSocketTask::SetCommProtocolType%966411252.body preserve=yes
	m_iProtocolType=ai_iCommProtocolType;
	return M4_SUCCESS;
  //## end ClCommOpeningSocketTask::SetCommProtocolType%966411252.body
}

m4pchar_t ClCommOpeningSocketTask::GetSSLCert ()
{
  //## begin ClCommOpeningSocketTask::GetSSLCert%966411253.body preserve=yes
	return m_pcSSLCert;
  //## end ClCommOpeningSocketTask::GetSSLCert%966411253.body
}

m4return_t ClCommOpeningSocketTask::SetPort (m4int_t ai_iPort)
{
  //## begin ClCommOpeningSocketTask::SetPort%966411254.body preserve=yes
	m_iPort=ai_iPort;
	return M4_SUCCESS;
  //## end ClCommOpeningSocketTask::SetPort%966411254.body
}

m4return_t ClCommOpeningSocketTask::SetSSLCert (m4pchar_t ai_pcSSLCert)
{
  //## begin ClCommOpeningSocketTask::SetSSLCert%966411256.body preserve=yes
	m_pcSSLCert=ai_pcSSLCert;
	return M4_SUCCESS;
  //## end ClCommOpeningSocketTask::SetSSLCert%966411256.body
}

m4return_t ClCommOpeningSocketTask::SetHost (m4pchar_t ai_pcHost)
{
  //## begin ClCommOpeningSocketTask::SetHost%966411257.body preserve=yes
	m_pcHost=ai_pcHost;
	return M4_SUCCESS;
  //## end ClCommOpeningSocketTask::SetHost%966411257.body
}

m4pchar_t ClCommOpeningSocketTask::GetHost ()
{
  //## begin ClCommOpeningSocketTask::GetHost%966411258.body preserve=yes
	return m_pcHost;
  //## end ClCommOpeningSocketTask::GetHost%966411258.body
}

// Additional Declarations
  //## begin ClCommOpeningSocketTask%38DB523002CF.declarations preserve=yes
  //## end ClCommOpeningSocketTask%38DB523002CF.declarations

// Class ClCommStopReadingTask 

ClCommStopReadingTask::ClCommStopReadingTask ()
  //## begin ClCommStopReadingTask::ClCommStopReadingTask%958548782.hasinit preserve=no
  //## end ClCommStopReadingTask::ClCommStopReadingTask%958548782.hasinit
  //## begin ClCommStopReadingTask::ClCommStopReadingTask%958548782.initialization preserve=yes
  :ClCommTask()
  //## end ClCommStopReadingTask::ClCommStopReadingTask%958548782.initialization
{
  //## begin ClCommStopReadingTask::ClCommStopReadingTask%958548782.body preserve=yes
  //## end ClCommStopReadingTask::ClCommStopReadingTask%958548782.body
}


ClCommStopReadingTask::~ClCommStopReadingTask ()
{
  //## begin ClCommStopReadingTask::~ClCommStopReadingTask%958548783.body preserve=yes
  //## end ClCommStopReadingTask::~ClCommStopReadingTask%958548783.body
}


// Additional Declarations
  //## begin ClCommStopReadingTask%3922B41E0131.declarations preserve=yes
  //## end ClCommStopReadingTask%3922B41E0131.declarations

// Class ClCommShutDownTask 

ClCommShutDownTask::ClCommShutDownTask ()
  //## begin ClCommShutDownTask::ClCommShutDownTask%958646102.hasinit preserve=no
  //## end ClCommShutDownTask::ClCommShutDownTask%958646102.hasinit
  //## begin ClCommShutDownTask::ClCommShutDownTask%958646102.initialization preserve=yes
  :ClCommTask(M4_COMM_SHUTDOWN_TASK)
  //## end ClCommShutDownTask::ClCommShutDownTask%958646102.initialization
{
  //## begin ClCommShutDownTask::ClCommShutDownTask%958646102.body preserve=yes
  //## end ClCommShutDownTask::ClCommShutDownTask%958646102.body
}


ClCommShutDownTask::~ClCommShutDownTask ()
{
  //## begin ClCommShutDownTask::~ClCommShutDownTask%958646103.body preserve=yes
  //## end ClCommShutDownTask::~ClCommShutDownTask%958646103.body
}


// Additional Declarations
  //## begin ClCommShutDownTask%3923F6850190.declarations preserve=yes
  //## end ClCommShutDownTask%3923F6850190.declarations

//## begin module%38DB62A6038A.epilog preserve=yes
//## end module%38DB62A6038A.epilog

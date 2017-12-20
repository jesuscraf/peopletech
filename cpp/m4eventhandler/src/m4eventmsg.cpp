//## begin module%37EA131902B7.cm preserve=no
//## end module%37EA131902B7.cm

//## begin module%37EA131902B7.cp preserve=no
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
//## end module%37EA131902B7.cp

//## Module: m4eventmsg%37EA131902B7; Package body
//## Subsystem: M4EventHandler::src%37EA1203031B
//## Source file: e:\source\m4eventhandler\version\src\m4eventmsg.cpp

//## begin module%37EA131902B7.additionalIncludes preserve=no
//## end module%37EA131902B7.additionalIncludes

//## begin module%37EA131902B7.includes preserve=yes
#include <m4trace.hpp>
#include <logsys.hpp>
//## end module%37EA131902B7.includes

// m4eventtopic
#include <m4eventtopic.hpp>
// m4eventmsg
#include <m4eventmsg.hpp>
// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
//## begin module%37EA131902B7.declarations preserve=no
//## end module%37EA131902B7.declarations

//## begin module%37EA131902B7.additionalDeclarations preserve=yes

#define EVENT_PRODUCER_TOKEN	"Producer:"
#define EVENT_PRODUCERID_TOKEN	"ProducerID:"
#define EVENT_COMMENT_TOKEN "Comment:"
#define EVENT_REASON_TOKEN "Reason:"
#define EVENT_SOLUTION_TOKEN "Solution:"
#define EVENT_SEVERITY_TOKEN "Severity:"

m4return_t CreateEventMessage(	ClEventMsg &ai_oMsg,
								const M4ClString & ai_strEventProducer,
								const M4ClString & ai_strProducerID,
								const M4ClString & ai_strSubject,
								const M4ClString & ai_strReason,
								const M4ClString & ai_strSolution,
								const M4ClString & ai_strSeverity
								)
{
	m4return_t iRet=M4_SUCCESS;
	
	ClEventSubject  theSubject;

	theSubject.append(EVENT_PRODUCER_TOKEN);
	theSubject.append(ai_strEventProducer);
	theSubject.append(M4_EVENT_FIELD_SEPARATOR);

	theSubject.append(EVENT_PRODUCERID_TOKEN);
	theSubject.append(ai_strProducerID);
	theSubject.append(M4_EVENT_FIELD_SEPARATOR);

	theSubject.append(EVENT_COMMENT_TOKEN);
	theSubject.append(ai_strSubject);
	theSubject.append(M4_EVENT_FIELD_SEPARATOR);

	theSubject.append(EVENT_REASON_TOKEN);
	if(ai_strReason.empty())
		theSubject.append(M4_DEFAULT_REASON);
	else
		theSubject.append(ai_strReason);
	theSubject.append(M4_EVENT_FIELD_SEPARATOR);

	theSubject.append(EVENT_SOLUTION_TOKEN);
	if(ai_strSolution.empty())
		theSubject.append(M4_DEFAULT_SOLUTION);
	else
		theSubject.append(ai_strSolution);
	theSubject.append(M4_EVENT_FIELD_SEPARATOR);

	theSubject.append(EVENT_SEVERITY_TOKEN);
	if(ai_strSeverity.empty())
		theSubject.append(M4_DEFAULT_SEVERITY);
	else
		theSubject.append(ai_strSeverity);
	theSubject.append(M4_EVENT_FIELD_SEPARATOR);


	ClEventHeader theHeader;
	iRet=theHeader.SetSubject(theSubject);

	iRet=ai_oMsg.SetHeader(theHeader);

	return iRet;
}

m4return_t GetDataFromEventMessage(	ClEventMsg &ai_oMsg,
								M4ClString & ai_strEventProducer,
								M4ClString & ai_strProducerID,
								M4ClString & ai_strSubject,
								M4ClString & ai_strCause,
								M4ClString & ai_strSolution,
								M4ClString & ai_strSeverity
								)
{
	m4return_t iRet=M4_ERROR;

	ClEventHeader theHeader;
	ai_oMsg.GetHeader(theHeader);
	ClEventSubject theSubject;
	theHeader.GetSubject(theSubject);

	m4pcchar_t ptrProducer=NULL;
	m4pcchar_t ptrProducerID=NULL;
	m4pcchar_t ptrSubject=NULL;
	m4pcchar_t ptrReason=NULL;
	m4pcchar_t ptrSolution=NULL;
	m4pcchar_t ptrSeverity=NULL;

	m4int_t SeparatorSize=strlen(M4_EVENT_FIELD_SEPARATOR);

	ptrProducer=strstr(theSubject.c_str(),EVENT_PRODUCER_TOKEN);
	ptrProducerID=strstr(theSubject.c_str(),EVENT_PRODUCERID_TOKEN);
	ptrSubject=strstr(theSubject.c_str(),EVENT_COMMENT_TOKEN);
	ptrReason=strstr(theSubject.c_str(),EVENT_REASON_TOKEN);
	ptrSolution=strstr(theSubject.c_str(),EVENT_SOLUTION_TOKEN);
	ptrSeverity=strstr(theSubject.c_str(),EVENT_SEVERITY_TOKEN);

	if(ptrProducer&&ptrProducerID&&ptrSubject&&ptrReason&&ptrSolution&&ptrSeverity)
	{
		ai_strEventProducer.append(ptrProducer,ptrProducerID-ptrProducer-SeparatorSize);
		ai_strProducerID.append(ptrProducerID,ptrSubject-ptrProducerID-SeparatorSize);
		ai_strSubject.append(ptrSubject,ptrReason-ptrSubject-SeparatorSize);
		ai_strCause.append(ptrReason,ptrSolution-ptrReason-SeparatorSize);
		ai_strSolution.append(ptrSolution,ptrSeverity-ptrSolution-SeparatorSize);
		ai_strSeverity.append(ptrSeverity);
	}
	else
	{
		m4Trace(cerr<< "Unkown Format."<<endl);
	}
	return iRet;
}



//## end module%37EA131902B7.additionalDeclarations


// Class ClEventMsg 


ClEventMsg::ClEventMsg (const M4ClString &ai_strTopic)
  //## begin ClEventMsg::ClEventMsg%938164937.hasinit preserve=no
  //## end ClEventMsg::ClEventMsg%938164937.hasinit
  //## begin ClEventMsg::ClEventMsg%938164937.initialization preserve=yes
  :  m_pClEventHeader(ai_strTopic)
  //## end ClEventMsg::ClEventMsg%938164937.initialization
{
  //## begin ClEventMsg::ClEventMsg%938164937.body preserve=yes

  //## end ClEventMsg::ClEventMsg%938164937.body
}

ClEventMsg::ClEventMsg (m4pchar_t ai_pcBuffer)
  //## begin ClEventMsg::ClEventMsg%939283917.hasinit preserve=no
  //## end ClEventMsg::ClEventMsg%939283917.hasinit
  //## begin ClEventMsg::ClEventMsg%939283917.initialization preserve=yes
  //## end ClEventMsg::ClEventMsg%939283917.initialization
{
  //## begin ClEventMsg::ClEventMsg%939283917.body preserve=yes

  //## end ClEventMsg::ClEventMsg%939283917.body
}


ClEventMsg::~ClEventMsg ()
{
  //## begin ClEventMsg::~ClEventMsg%938164938.body preserve=yes
  //## end ClEventMsg::~ClEventMsg%938164938.body
}



//## Other Operations (implementation)
m4return_t ClEventMsg::GetHeader (ClEventHeader &ao_oEventHeader)
{
  //## begin ClEventMsg::GetHeader%938087482.body preserve=yes
	ao_oEventHeader=m_pClEventHeader;
	return M4_SUCCESS;
  //## end ClEventMsg::GetHeader%938087482.body
}

m4return_t ClEventMsg::SetHeader (ClEventHeader &ai_oEventHeader)
{
  //## begin ClEventMsg::SetHeader%938591992.body preserve=yes
	m_pClEventHeader=ai_oEventHeader;
	return M4_SUCCESS;
  //## end ClEventMsg::SetHeader%938591992.body
}

m4return_t ClEventMsg::Serialize (m4pcchar_t ao_pcBuffer, m4int_t ai_iBufferSize, m4int_t &ao_iOutputSize)
{
  //## begin ClEventMsg::Serialize%939206937.body preserve=yes
	m4return_t iRet=M4_ERROR;
	if(ao_pcBuffer && ai_iBufferSize)
	{
		ClEventSubject theSubject;
		iRet=m_pClEventHeader.GetSubject(theSubject);
		ao_iOutputSize=theSubject.size();
		if(M4_ERROR!=iRet)
		{
			if(ai_iBufferSize>ao_iOutputSize)
			{
				strcpy((m4pchar_t)ao_pcBuffer,theSubject.c_str());
			}
/*
			ClEventTopic theTopic;
			iRet=m_pClEventHeader.GetTopic(theTopic);
			if(M4_ERROR!=iRet)
			{
				ClEventTopicID theTopicID;
				iRet=theTopic.GetTopicID(theTopicID);
				if(M4_ERROR!=iRet)
				{
					 int bufferSize =	6   // EVENT: 
										+10 // EventCounter 
										+1
										+6  // TOPIC:
										+ theTopicID.size()
										+1
										+8  // SUBJECT:
										+ theSubject.size();
					 
					if(ai_iBufferSize<bufferSize)
					{
						m4Trace(cerr<< "Buffer too small"<<endl);
					}
					else
					{
						if(strstr(theSubject.c_str(),M4_EVENT_TOPIC_TOKEN))
						{
	sprintf((m4pchar_t)ao_pcBuffer,"%s",theSubject.c_str());
						}
						else
						{
	sprintf((m4pchar_t)ao_pcBuffer,"%s %s  %s %s",M4_EVENT_TOPIC_TOKEN,theTopicID.c_str(),M4_EVENT_SUBJECT_TOKEN,theSubject.c_str());
						}
					}
				}
			}
			*/
		}
	}
	else
	{
		m4Trace(cerr << "Invalid buffer "<<endl);
	}
	return iRet;
  //## end ClEventMsg::Serialize%939206937.body
}

// Additional Declarations
  //## begin ClEventMsg%37E10FC403A3.declarations preserve=yes
  //## end ClEventMsg%37E10FC403A3.declarations

// Class ClEventHeader 




ClEventHeader::ClEventHeader (const M4ClString &ai_strTopic)
  //## begin ClEventHeader::ClEventHeader%938164935.hasinit preserve=no
  //## end ClEventHeader::ClEventHeader%938164935.hasinit
  //## begin ClEventHeader::ClEventHeader%938164935.initialization preserve=yes
  : m_pClEventTopic(ai_strTopic)
  //## end ClEventHeader::ClEventHeader%938164935.initialization
{
  //## begin ClEventHeader::ClEventHeader%938164935.body preserve=yes
  //## end ClEventHeader::ClEventHeader%938164935.body
}

ClEventHeader::ClEventHeader ()
  //## begin ClEventHeader::ClEventHeader%938519904.hasinit preserve=no
  //## end ClEventHeader::ClEventHeader%938519904.hasinit
  //## begin ClEventHeader::ClEventHeader%938519904.initialization preserve=yes
  //## end ClEventHeader::ClEventHeader%938519904.initialization
{
  //## begin ClEventHeader::ClEventHeader%938519904.body preserve=yes
  //## end ClEventHeader::ClEventHeader%938519904.body
}


ClEventHeader::~ClEventHeader ()
{
  //## begin ClEventHeader::~ClEventHeader%938164936.body preserve=yes
  //## end ClEventHeader::~ClEventHeader%938164936.body
}



//## Other Operations (implementation)
m4return_t ClEventHeader::GetTopic (ClEventTopic &ao_oTopic)
{
  //## begin ClEventHeader::GetTopic%938087490.body preserve=yes
	m4return_t iRet=M4_SUCCESS;
	ClEventTopicID theTopicID;
	m_pClEventTopic.GetTopicID(theTopicID);
	if(theTopicID.empty())
	{
		m4Trace(cerr<<"Empty topic"<<endl);
		iRet=M4_ERROR;
	}
	ao_oTopic=m_pClEventTopic;
	return iRet;
  //## end ClEventHeader::GetTopic%938087490.body
}

m4return_t ClEventHeader::GetSubject (ClEventSubject &ao_oSubject)
{
  //## begin ClEventHeader::GetSubject%938087491.body preserve=yes
	m4return_t iRet=M4_SUCCESS;
	if(m_pClEventSubject.empty())
	{
		m4Trace(cerr<<"Empty Subject"<<endl);
		iRet=M4_ERROR;
	}
	ao_oSubject=m_pClEventSubject;
	return iRet;
  //## end ClEventHeader::GetSubject%938087491.body
}

m4return_t ClEventHeader::SetSubject (const ClEventSubject &ai_oSubject)
{
  //## begin ClEventHeader::SetSubject%938087493.body preserve=yes
	m_pClEventSubject=ai_oSubject;
	return M4_SUCCESS;
  //## end ClEventHeader::SetSubject%938087493.body
}

ClEventHeader & ClEventHeader::operator = (const ClEventHeader &ai_oEventHeader)
{
  //## begin ClEventHeader::operator=%938424972.body preserve=yes
	m_pClEventSubject=ai_oEventHeader.m_pClEventSubject;
	m_pClEventTopic=ai_oEventHeader.m_pClEventTopic;
	return *this;
  //## end ClEventHeader::operator=%938424972.body
}

// Additional Declarations
  //## begin ClEventHeader%37E73DB00285.declarations preserve=yes
  //## end ClEventHeader%37E73DB00285.declarations

// Class ClEventMsgHistory 



ClEventMsgHistory::ClEventMsgHistory ()
  //## begin ClEventMsgHistory::ClEventMsgHistory%938164939.hasinit preserve=no
      : m_iCounter(0)
  //## end ClEventMsgHistory::ClEventMsgHistory%938164939.hasinit
  //## begin ClEventMsgHistory::ClEventMsgHistory%938164939.initialization preserve=yes
//  : uiBufferSize(0), pcBuffer(NULL)
  //## end ClEventMsgHistory::ClEventMsgHistory%938164939.initialization
{
  //## begin ClEventMsgHistory::ClEventMsgHistory%938164939.body preserve=yes
  //## end ClEventMsgHistory::ClEventMsgHistory%938164939.body
}


ClEventMsgHistory::~ClEventMsgHistory ()
{
  //## begin ClEventMsgHistory::~ClEventMsgHistory%938164940.body preserve=yes
	EventList::iterator theite=m_EventList.begin();
	while(theite!=m_EventList.end())
	{
		delete (*theite);
		m_EventList.erase(theite);
		theite=m_EventList.begin();
	}
/*	if(!pcBuffer)
		delete pcBuffer;
*/
  //## end ClEventMsgHistory::~ClEventMsgHistory%938164940.body
}



//## Other Operations (implementation)
m4size_t ClEventMsgHistory::GetSize ()
{
  //## begin ClEventMsgHistory::GetSize%938087483.body preserve=yes
	return m_EventList.size();
  //## end ClEventMsgHistory::GetSize%938087483.body
}

m4return_t ClEventMsgHistory::AddNextEventMsg (const ClEventMsg &ai_oEventMsg)
{
  //## begin ClEventMsgHistory::AddNextEventMsg%938087484.body preserve=yes
		return M4_ERROR;
  //## end ClEventMsgHistory::AddNextEventMsg%938087484.body
}

m4return_t ClEventMsgHistory::AddPriorEventMsg (const ClEventMsg &ai_oEventMsg)
{
  //## begin ClEventMsgHistory::AddPriorEventMsg%938087485.body preserve=yes
		return M4_ERROR;
  //## end ClEventMsgHistory::AddPriorEventMsg%938087485.body
}

m4return_t ClEventMsgHistory::GetFirstEventMsg (ClEventMsg &ao_oEventMsg)
{
  //## begin ClEventMsgHistory::GetFirstEventMsg%938087486.body preserve=yes
	m4return_t iRet=M4_ERROR;
	if(!m_EventList.size())
	{
		m4Trace(cerr<<"Empty MsgHistory"<<endl);
	}
	else
	{
		ClEventHeader theHeader;
		iRet=m_EventList[0]->GetHeader(theHeader);
		iRet=ao_oEventMsg.SetHeader(theHeader);
		m_iCounter=1;
	}
	return iRet;
  //## end ClEventMsgHistory::GetFirstEventMsg%938087486.body
}

m4return_t ClEventMsgHistory::GetNextEventMsg (ClEventMsg &ao_oEventMsg)
{
  //## begin ClEventMsgHistory::GetNextEventMsg%938087487.body preserve=yes
	m4return_t iRet=M4_ERROR;
	if(m_EventList.size()<=m_iCounter)
	{
		m4Trace(cerr<<"End of MsgHistory reached"<<endl);
	}
	else
	{
		ClEventHeader theHeader;
		iRet=m_EventList[m_iCounter]->GetHeader(theHeader);
		iRet=ao_oEventMsg.SetHeader(theHeader);
		m_iCounter++;
	}	
	return iRet;
  //## end ClEventMsgHistory::GetNextEventMsg%938087487.body
}

m4return_t ClEventMsgHistory::GetLastEventMsg (ClEventMsg &ao_oEventMsg)
{
  //## begin ClEventMsgHistory::GetLastEventMsg%938087488.body preserve=yes
	m4return_t iRet=M4_ERROR;
	if(!m_EventList.size())
	{
		m4Trace(cerr<<"Empty MsgHistory"<<endl);
	}
	else
	{
		m_iCounter=m_EventList.size()-1;
		ClEventHeader theHeader;
		iRet=m_EventList[m_iCounter]->GetHeader(theHeader);
		iRet=ao_oEventMsg.SetHeader(theHeader);
	}
	return iRet;
  //## end ClEventMsgHistory::GetLastEventMsg%938087488.body
}

m4return_t ClEventMsgHistory::GetPriorEventMsg (ClEventMsg &ao_oEventMsg)
{
  //## begin ClEventMsgHistory::GetPriorEventMsg%938087489.body preserve=yes
	m4return_t iRet=M4_ERROR;
	if(m_iCounter<0)
	{
		m4Trace(cerr<<"Beginig of MsgHistory reached"<<endl);
	}
	else
	{
		ClEventHeader theHeader;
		iRet=m_EventList[m_iCounter]->GetHeader(theHeader);
		iRet=ao_oEventMsg.SetHeader(theHeader);
		m_iCounter--;
	}		
	return iRet;
  //## end ClEventMsgHistory::GetPriorEventMsg%938087489.body
}

m4return_t ClEventMsgHistory::Serialize (m4pcchar_t ao_pcBuffer, m4int_t ai_iBufferSize, m4int_t &ao_iOutputSize)
{
  //## begin ClEventMsgHistory::Serialize%938164941.body preserve=yes
	m4return_t iRet=M4_ERROR;
	if(m_iCounter)
	{
		m4int_t counter;
		m4int_t inlog=0;
		m4uint32_t bufferused=0;
		m4pchar_t buffer=new m4char_t[4096];
		ClLogSystem *logsys=new ClLogSystem(GETLOGMANAGER(),M4_TRUE);

		short Corto=0;
		m4int_t eventsize=0;
		for(counter=0;counter<m_iCounter;counter++)
		{
			(m_EventList[counter])->Serialize(buffer,4096,eventsize);
/*
// Interface antiguo no bloqueante
			iRet=logsys -> StartCode(1, WARNINGLOG);
			iRet=logsys -> AddText ( buffer );
			Corto=logsys -> SetCode();
*/
			m4pchar_t the2point=strchr(buffer,':');
			*the2point='\0';
			the2point++;
			Corto=logsys -> SetCode(atoi(buffer), WARNINGLOG,the2point);
		}
		inlog=logsys -> GetMessageCounter();
		M4_ASSERT(m_iCounter==inlog);
		
		iRet=logsys->GetSerialStringSizeFromMessage(bufferused,0);
		if(bufferused<ai_iBufferSize)
		{
			m4uint32_t lastiter=0;
			iRet=logsys->GetSerialStringFromMessage((m4pchar_t)ao_pcBuffer,ai_iBufferSize,lastiter,0);
			ao_iOutputSize=bufferused;
		}
		else
		{
			iRet=M4_ERROR;
			m4Trace(cerr << "Buffer too small"<<endl);
		}
		delete logsys;
	}
	else
	{
		ao_iOutputSize=0;
	}
/*	if((!pcBuffer) || (!uiBufferSize))
	{
		iRet=M4_ERROR;
		m4Trace(cerr<< "NULL buffer"<<endl);
	}
	else
	{
		if(ai_iBufferSize<uiBufferSize)
		{
			iRet=M4_ERROR;
			m4Trace(cerr<< "Buffer too small"<<endl);
		}
		else
		{
			memcpy((void *)ao_pcBuffer,pcBuffer,uiBufferSize);
			ao_iOutputSize=uiBufferSize;
		}
	}
	*/
	return iRet;
  //## end ClEventMsgHistory::Serialize%938164941.body
}

m4return_t ClEventMsgHistory::Deserialize (m4pcchar_t ai_pcBuffer, m4int_t ai_iBufferSize)
{
  //## begin ClEventMsgHistory::Deserialize%938164942.body preserve=yes
	m4return_t iRet=M4_SUCCESS;
	M4_ASSERT((ai_iBufferSize)&& ai_pcBuffer);
	if((!ai_iBufferSize)&& (!ai_pcBuffer))
	{
		m4Trace(cerr<< "parametros erroneos"<<endl);
		iRet=M4_ERROR;
	}
	else
	{
		ClLogSystem *tempLog=new ClLogSystem(GETLOGMANAGER(),M4_TRUE);
		M4_ASSERT(tempLog);
		tempLog -> SetSerialCodeString ( (char *)ai_pcBuffer);

		//m4uint32_t uiMsg=tempLog -> GetErrorQueueSize();
		m4uint32_t uiMsg=tempLog -> GetMessageCounter();
		if(uiMsg)
		{
			m4int32_t msgSize=0;
			m4pchar_t buffer2=new m4char_t[20000];
			ClEventMsg *theMsg;
			ClEventHeader theHeader;
			m4return_t iRet2=M4_SUCCESS;
			for(m4int_t counter=0;iRet2!=M4_ERROR;counter++)
			{

				iRet2=tempLog->GetErrorText(buffer2,20000,msgSize);

				if(M4_ERROR!=iRet2)
				{
/*					m4pchar_t finalTopic;
					ClEventTopicID theTopic=M4AdminEventTopic;
					m4pchar_t topicpointer=strstr(buffer2,M4_EVENT_TOPIC_TOKEN);
					if(topicpointer)
					{
						topicpointer=strchr(topicpointer,':')+1;
						finalTopic=strchr(topicpointer,' ');
						*finalTopic=0;
						theTopic=topicpointer;
					}
					m4pchar_t subjectpointer=strstr(finalTopic+1,M4_EVENT_SUBJECT_TOKEN);
					if(subjectpointer)
					{
						subjectpointer=strchr(subjectpointer,':')+1;
					}

					theMsg=new ClEventMsg(theTopic);
					theHeader=new ClEventHeader(theTopic);
*/
					m4int32_t thecode;
					iRet2=tempLog->GetErrorCode(thecode);
					m4pchar_t buffer_msg=new m4char_t[msgSize+20];
					sprintf(buffer_msg,"%d:%s",thecode,buffer2);
					
					theMsg=new ClEventMsg("");
					theMsg->GetHeader(theHeader);
					theHeader.SetSubject(buffer_msg);
					theMsg->SetHeader(theHeader);
					m_EventList.push_back(theMsg);
					tempLog->Pop();
					m_iCounter++;
					delete buffer_msg;
				}
			}
			delete buffer2;
		}
		delete tempLog;
	}
	
	return iRet;
  //## end ClEventMsgHistory::Deserialize%938164942.body
}

// Additional Declarations
  //## begin ClEventMsgHistory%37E8B04901BC.declarations preserve=yes
  //## end ClEventMsgHistory%37E8B04901BC.declarations

//## begin module%37EA131902B7.epilog preserve=yes
//## end module%37EA131902B7.epilog


// Detached code regions:
// WARNING: this code will be lost if code is regenerated.
#if 0
//## begin ClEventMsg::Deserialize%939206938.body preserve=no
	m4return_t iRet=M4_ERROR;
	return iRet;
//## end ClEventMsg::Deserialize%939206938.body

#endif

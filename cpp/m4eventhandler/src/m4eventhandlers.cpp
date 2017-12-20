//## begin module%37EF83EA034E.cm preserve=no
//## end module%37EF83EA034E.cm

//## begin module%37EF83EA034E.cp preserve=no
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
//## end module%37EF83EA034E.cp

//## Module: m4eventhandlers%37EF83EA034E; Package body
//## Subsystem: M4EventHandler::src%37EA1203031B
//## Source file: e:\source\m4eventhandler\src\m4eventhandlers.cpp

//## begin module%37EF83EA034E.additionalIncludes preserve=no
//## end module%37EF83EA034E.additionalIncludes

//## begin module%37EF83EA034E.includes preserve=yes
#include <m4trace.hpp>
#include <logsys.hpp>
//## end module%37EF83EA034E.includes

// m4eventhandlers
#include <m4eventhandlers.hpp>
// m4eventhandler
#include <m4eventhandler.hpp>
// m4types
#include <m4types.hpp>
// syncro
#include <syncro.hpp>
//## begin module%37EF83EA034E.declarations preserve=no
//## end module%37EF83EA034E.declarations

//## begin module%37EF83EA034E.additionalDeclarations preserve=yes

class  VBEventCP: public IEventConsumer, public IEventProducer
{
public:

	m4return_t GetConsumerID(m4handle_t &ao_hConsumerID ) 
	{ return M4_ERROR;}
	m4return_t GetProducerID(m4handle_t &ao_hProducerID )
	{ return M4_ERROR;}
	m4return_t State;
	IEventHandler * m_poEH;
	ClEventMsgHistory * m_poMsgList;
	ClEventMsg m_oMsg;
	m4handle_t m_hEventID;
	static ClMutex m_Mutex;

	VBEventCP()
		: m_poEH(NULL),m_poMsgList(NULL),m_hEventID(0),m_oMsg(M4AdminEventTopic)
	{}
	~VBEventCP()
	{}
};

static VBEventCP VBEventTrick;

ClMutex VBEventCP::m_Mutex(M4_TRUE);

m4return_t M4SubscribeTopic(m4char_t *ai_szTopic  )
{
	VBEventTrick.m_Mutex.Lock();
	m4return_t iRet=M4_ERROR;
	IEventHandler * TheEH=NULL;
	if(!ai_szTopic)
	{
		m4Trace(cerr<<"NULL Topic"<<endl);
	}
	else
	{
		if(!TheEH)
			TheEH=ClLogBasedEventHandler::GetEHInstance();
		if(TheEH)
		{
			iRet=TheEH->SubscribeTopic(VBEventTrick,ai_szTopic);
			if(M4_ERROR==iRet)
			{
				m4Trace(cerr<< "Error subscribing " <<ai_szTopic <<endl);
			}
			else
				iRet=M4_SUCCESS;
		}
		else
		{
			m4Trace(cerr << "Initalization ERROR"<<endl);
			iRet=M4_ERROR;
		}
	}
	VBEventTrick.m_Mutex.Unlock();
	return iRet;
}


m4return_t M4UnsubscribeTopic(m4char_t *ai_szTopic)
{
	VBEventTrick.m_Mutex.Lock();
	m4return_t iRet=M4_ERROR;

	IEventHandler * TheEH=NULL;
	if(!ai_szTopic)
	{
		m4Trace(cerr<<"NULL Topic"<<endl);
	}
	else
	{
		if(!TheEH)
			TheEH=ClLogBasedEventHandler::GetEHInstance();
		if(TheEH)
		{
			m4Trace(cout << "Initalization OK"<<endl);
			iRet=TheEH->UnsubscribeTopic(VBEventTrick,ai_szTopic);
			if(M4_ERROR==iRet)
			{
				m4Trace(cerr<< "Error unsubscribing "<<ai_szTopic <<endl);
			}
			else
				iRet=M4_SUCCESS;
		}
		else
		{
			m4Trace(cerr << "Initalization ERROR"<<endl);
			iRet=M4_ERROR;
		}
	}
	VBEventTrick.m_Mutex.Unlock();
	return iRet;
}


m4handle_t M4GetEventHandle(m4char_t *ai_szTopic)
{
	VBEventTrick.m_Mutex.Lock();

	m4return_t iRet=M4_ERROR;
	
	IEventHandler * TheEH=NULL;
	
	if(!ai_szTopic)
	{
		m4Trace(cerr<<"NULL Topic"<<endl);
	}
	else
	{
		if(!TheEH)
			TheEH=ClLogBasedEventHandler::GetEHInstance();
		if(TheEH)
		{
			m4TraceLevel(2,cout << "Initalization OK"<<endl);
			if (!VBEventTrick.m_poMsgList )
			{
				VBEventTrick.m_poMsgList=new ClEventMsgHistory;
			
				iRet=TheEH->GetEventMsg(VBEventTrick,*VBEventTrick.m_poMsgList,ai_szTopic);
			
				if(M4_ERROR==iRet)
				{
					m4Trace(cerr<< "Error getting events "<<ai_szTopic <<endl);
					VBEventTrick.m_hEventID=0;
				}
				else
				{
					VBEventTrick.m_poMsgList->GetFirstEventMsg(VBEventTrick.m_oMsg);
					VBEventTrick.m_hEventID=1;
				}
			}
			else
			{
				if(!VBEventTrick.m_hEventID)
				{
					iRet=TheEH->GetEventMsg(VBEventTrick,*VBEventTrick.m_poMsgList,ai_szTopic);
			
					if(M4_ERROR==iRet)
					{
						m4Trace(cerr<< "Error getting events "<<ai_szTopic <<endl);
						VBEventTrick.m_hEventID=0;
					}
					else
					{
						VBEventTrick.m_poMsgList->GetFirstEventMsg(VBEventTrick.m_oMsg);
						VBEventTrick.m_hEventID=1;
					}
				}
				else
				{
					m4return_t iRet2=VBEventTrick.m_poMsgList->GetNextEventMsg(VBEventTrick.m_oMsg);
					if(iRet2!=M4_ERROR)
						VBEventTrick.m_hEventID++;
					else 
					{
						VBEventTrick.m_hEventID=0;
						delete VBEventTrick.m_poMsgList;
						VBEventTrick.m_poMsgList=NULL;
					}
				}
			}
		}
		else
		{
			m4Trace(cerr << "Initalization ERROR"<<endl);
			iRet=M4_ERROR;
		}
	}
	VBEventTrick.m_Mutex.Unlock();
	return VBEventTrick.m_hEventID;
}


m4return_t M4CloseEventHandle(m4handle_t ai_hEventHandle)
{
	VBEventTrick.m_Mutex.Lock();
	m4return_t iRet=M4_SUCCESS;
	if(VBEventTrick.m_poMsgList)
	{
		if((m4uint32_t)ai_hEventHandle<=(VBEventTrick.m_poMsgList->GetSize()))
		//incrementa contador
			VBEventTrick.m_hEventID++;
	}
	VBEventTrick.m_Mutex.Unlock();
	return iRet;
}


m4return_t M4GetEventSubject(m4handle_t ai_hEventHandle , char *  ao_pszSubject ,  long ai_lSize , long & ao_lSubject )
{
	VBEventTrick.m_Mutex.Lock();
	m4return_t iRet=M4_ERROR;

	ClEventHeader theHeader;
	ClEventSubject theSubject;

	if(ai_hEventHandle==VBEventTrick.m_hEventID)
	{
		iRet=VBEventTrick.m_oMsg.GetHeader(theHeader);
		iRet=theHeader.GetSubject(theSubject);
		ao_lSubject=theSubject.size();
		if(ai_lSize>ao_lSubject)
		{
			sprintf(ao_pszSubject,"%s",theSubject.c_str());
			iRet=M4_SUCCESS;
		}
		else
		{
			sprintf(ao_pszSubject,"...");
			iRet=M4_WARNING;
		}

/*

		ClEventTopic	theTopic;
		ClEventTopicID	theTopicID;
		iRet=VBEventTrick.m_oMsg.GetHeader(theHeader);
		theHeader.GetTopic(theTopic);
		theTopic.GetTopicID(theTopicID);
		iRet=theHeader.GetSubject(theSubject);
		ao_lSubject=7+ theTopicID.size()+ 9 +theSubject.size();
		if(ai_lSize>ao_lSubject)
		{
			sprintf(ao_pszSubject,"%s %s  %s %s",M4_EVENT_TOPIC_TOKEN,theTopicID.c_str(),M4_EVENT_SUBJECT_TOKEN,theSubject.c_str());
			iRet=M4_SUCCESS;
		}
		else
		{
			sprintf(ao_pszSubject,"%s %s  %s ... ",M4_EVENT_TOPIC_TOKEN,theTopicID.c_str(),M4_EVENT_SUBJECT_TOKEN);
			iRet=M4_WARNING;
		}
*/
	}
	VBEventTrick.m_Mutex.Unlock();
	return iRet;
}


m4return_t M4PublishEvent(m4pchar_t ai_szTopic,m4pchar_t  ai_szSubject )
{
	VBEventTrick.m_Mutex.Lock();
	m4return_t iRet=M4_ERROR;
	if(ai_szTopic && ai_szSubject)
	{
		m4pchar_t buffer=new m4char_t[strlen(ai_szSubject)+strlen(ai_szTopic)+1+8];
		if(atoi(ai_szTopic))
			sprintf(buffer,"%s:#*S1*#%s",ai_szTopic,ai_szSubject);
		else
			sprintf(buffer,"65536:#*S1*#%s",ai_szSubject);

		ClEventSubject theSubject(buffer);
		
		ClEventMsg theEvent("");
		ClEventHeader theHeader;
		theEvent.GetHeader(theHeader);
		
		theHeader.SetSubject(theSubject);
		theEvent.SetHeader(theHeader);

		IEventHandler * TheEH=NULL;
		TheEH=ClLogBasedEventHandler::GetEHInstance();
		if(TheEH)
		{
			TheEH->PublishEventMsg(VBEventTrick,theEvent);
		}
		else
		{
			m4Trace(cerr<< "NULL EH"<<endl);
		}
		delete buffer;
	}
	else
	{
		m4Trace(cerr<< "Invalid argument"<<endl);
	}
	VBEventTrick.m_Mutex.Unlock();
	return iRet;
}


//## end module%37EF83EA034E.additionalDeclarations


// Class ClServerEventHandler 

ClServerEventHandler::ClServerEventHandler ()
  //## begin ClServerEventHandler::ClServerEventHandler%938424975.hasinit preserve=no
  //## end ClServerEventHandler::ClServerEventHandler%938424975.hasinit
  //## begin ClServerEventHandler::ClServerEventHandler%938424975.initialization preserve=yes
  //## end ClServerEventHandler::ClServerEventHandler%938424975.initialization
{
  //## begin ClServerEventHandler::ClServerEventHandler%938424975.body preserve=yes
  //## end ClServerEventHandler::ClServerEventHandler%938424975.body
}


ClServerEventHandler::~ClServerEventHandler ()
{
  //## begin ClServerEventHandler::~ClServerEventHandler%938424976.body preserve=yes
  //## end ClServerEventHandler::~ClServerEventHandler%938424976.body
}


// Additional Declarations
  //## begin ClServerEventHandler%37EF83480107.declarations preserve=yes
  //## end ClServerEventHandler%37EF83480107.declarations

// Class ClClientEventHandler 

ClClientEventHandler::ClClientEventHandler ()
  //## begin ClClientEventHandler::ClClientEventHandler%938424973.hasinit preserve=no
  //## end ClClientEventHandler::ClClientEventHandler%938424973.hasinit
  //## begin ClClientEventHandler::ClClientEventHandler%938424973.initialization preserve=yes
  //## end ClClientEventHandler::ClClientEventHandler%938424973.initialization
{
  //## begin ClClientEventHandler::ClClientEventHandler%938424973.body preserve=yes
  //## end ClClientEventHandler::ClClientEventHandler%938424973.body
}


ClClientEventHandler::~ClClientEventHandler ()
{
  //## begin ClClientEventHandler::~ClClientEventHandler%938424974.body preserve=yes
  //## end ClClientEventHandler::~ClClientEventHandler%938424974.body
}


// Additional Declarations
  //## begin ClClientEventHandler%37EF837002B3.declarations preserve=yes
  //## end ClClientEventHandler%37EF837002B3.declarations

// Class ClLogBasedEventHandler 

//## begin ClLogBasedEventHandler::m_poLogSystem%37EF84C20287.role preserve=no  protected: static ClLogSystem {1 -> 1RHAN}
ClLogSystem *ClLogBasedEventHandler::m_poLogSystem = NULL;
//## end ClLogBasedEventHandler::m_poLogSystem%37EF84C20287.role


//## begin ClLogBasedEventHandler::m_oMutex%37F07DDE035A.role preserve=no  protected: static ClMutex { -> UHAN}
ClMutex ClLogBasedEventHandler::m_oMutex(M4_TRUE);
//## end ClLogBasedEventHandler::m_oMutex%37F07DDE035A.role

//## begin ClLogBasedEventHandler::m_EventCounter%37F0C5930053.role preserve=no  protected: static m4uint32_t {1 -> 1UHAN}
m4uint32_t ClLogBasedEventHandler::m_EventCounter = 0;
//## end ClLogBasedEventHandler::m_EventCounter%37F0C5930053.role

ClLogBasedEventHandler::ClLogBasedEventHandler ()
  //## begin ClLogBasedEventHandler::ClLogBasedEventHandler%938424977.hasinit preserve=no
  //## end ClLogBasedEventHandler::ClLogBasedEventHandler%938424977.hasinit
  //## begin ClLogBasedEventHandler::ClLogBasedEventHandler%938424977.initialization preserve=yes
  //## end ClLogBasedEventHandler::ClLogBasedEventHandler%938424977.initialization
{
  //## begin ClLogBasedEventHandler::ClLogBasedEventHandler%938424977.body preserve=yes
	m_poLogSystem = new ClLogSystem(GETLOGMANAGER(),M4_TRUE,M4_TRUE);
	if(m_poLogSystem)
		m_sEventHandlerInstance = this;
	else
	{
		m4Trace(cerr<<"Error creating LogSystem"<<endl);
	}
  //## end ClLogBasedEventHandler::ClLogBasedEventHandler%938424977.body
}


ClLogBasedEventHandler::~ClLogBasedEventHandler ()
{
  //## begin ClLogBasedEventHandler::~ClLogBasedEventHandler%938424978.body preserve=yes
	delete m_poLogSystem;
	m_sEventHandlerInstance=NULL;
  //## end ClLogBasedEventHandler::~ClLogBasedEventHandler%938424978.body
}



//## Other Operations (implementation)
m4return_t ClLogBasedEventHandler::SubscribeTopic (const IEventConsumer &ai_oEventClient, const ClEventTopic &ai_oEventTopic, m4bool_t ai_bHistory)
{
  //## begin ClLogBasedEventHandler::SubscribeTopic%938424979.body preserve=yes
	m4return_t iRet=M4_SUCCESS;
	SubscriptorMap::iterator TheIterator;
	m4handle_t thisconsumer=(m4handle_t)&ai_oEventClient;

	m_oMutex.Lock();
	TheIterator=m_pSubscriptorMap.find(thisconsumer);
	if(TheIterator!=m_pSubscriptorMap.end())
	{
		m4Trace(cerr<< "Subcriptor already in the list" <<endl);
		m4Trace(cerr<< "With "<< (*TheIterator).second <<"events"<<endl)
		iRet=M4_WARNING;
	}
	else
	{
		pair< m4handle_t , m4int32_t> parIn;
	
		parIn.first=thisconsumer;
		parIn.second=0;
		m4Trace(cout<< "(Before "<< m_pSubscriptorMap.size()<<" subscriptors.");
		m_pSubscriptorMap.insert(parIn);
		m4Trace(cout<< " Now "<< m_pSubscriptorMap.size()<<")"<<endl);
	}
	m_oMutex.Unlock();

	return iRet;
  //## end ClLogBasedEventHandler::SubscribeTopic%938424979.body
}

m4return_t ClLogBasedEventHandler::UnsubscribeTopic (const IEventConsumer &ai_oEventClient, const ClEventTopic &ai_oEventTopic)
{
  //## begin ClLogBasedEventHandler::UnsubscribeTopic%938424980.body preserve=yes
	m4return_t iRet=M4_SUCCESS;
	
	m4handle_t thisconsumer=(m4handle_t)&ai_oEventClient;
	m_oMutex.Lock();
	size_t erased=m_pSubscriptorMap.erase(thisconsumer);
	m_oMutex.Unlock();
	if(1!=erased)
	{
		m4Trace(cerr<<"Erased:"<< (int)erased<<endl)
		iRet=M4_ERROR;
	}
	return iRet;
  //## end ClLogBasedEventHandler::UnsubscribeTopic%938424980.body
}

m4return_t ClLogBasedEventHandler::GetEventMsg (const IEventConsumer &ai_oEventClient, ClEventMsg &ao_oEventMsg, const ClEventTopic &ai_oEventTopic)
{
  //## begin ClLogBasedEventHandler::GetEventMsg%938424981.body preserve=yes
	m4return_t	iRet=M4_ERROR;
	m4handle_t	thisconsumer=(m4handle_t)&ai_oEventClient;
	m4int32_t	lastEvent=0;
	SubscriptorMap::iterator TheIterator;
	ClEventTopicID theTopicID;
	((ClEventTopic)ai_oEventTopic).GetTopicID(theTopicID);
	m_oMutex.Lock();
	TheIterator=m_pSubscriptorMap.find(thisconsumer);
	
	if(TheIterator==m_pSubscriptorMap.end())
	{
		iRet=M4_ERROR;
		m4Trace(cerr<< "Please, subscribe first"<<endl);
	}
	else
	{
		lastEvent=(*TheIterator).second;
		m4Trace(cout<<"Last event:="<< lastEvent<<endl)
	
		m4uint32_t buffersize=10000;
		m4pchar_t buffer=new m4char_t[buffersize];
		m4int32_t msgsize;
		iRet=m_poLogSystem-> Get_n_ErrorText ((lastEvent>0)?lastEvent-1:0, buffer, buffersize, msgsize);
		if(iRet==M4_ERROR)
		{
			m4Trace(cout<< "No events"<<endl);
		}
		else
		{
			m4pchar_t topic =strstr(buffer,M4_EVENT_TOPIC_TOKEN);
			if(topic)
			{
				topic=strchr(buffer,M4_EVENT_TOPIC_TOKEN_SEPARATOR);
				theTopicID=(topic+1);
					
				m4pchar_t subject =strstr(buffer,M4_EVENT_SUBJECT_TOKEN);
				if(subject)
				{
					subject=strchr(subject,M4_EVENT_SUBJECT_TOKEN_SEPARATOR);
					ClEventSubject theSubject=(subject+1);
					ClEventHeader theHeader;
					theHeader.SetSubject(theSubject);
					iRet=ao_oEventMsg.SetHeader(theHeader);				
				}
				else
				{
					m4Trace(cerr<< "Error finding subject"<<endl);
				}

			}
			else
			{
				m4Trace(cerr<< "Error finding topic"<<endl);
			}
		}
		delete buffer;
	}
	m_oMutex.Unlock();
	return iRet;
  //## end ClLogBasedEventHandler::GetEventMsg%938424981.body
}

m4return_t ClLogBasedEventHandler::PublishEventMsg (const IEventProducer &ai_oEventClient, const ClEventMsg &ai_oEventMsg)
{
  //## begin ClLogBasedEventHandler::PublishEventMsg%938424982.body preserve=yes
	m4return_t iRet=M4_ERROR;
	m4handle_t thisproducer=(m4handle_t)&ai_oEventClient;
	
	m4int_t outputSize=0;
	m4int_t bufferSize=2048;
	m4pchar_t buffer=NULL;

	while((bufferSize<100000)&& (iRet==M4_ERROR))
	{
		bufferSize*=2;
		if(buffer)
		{
			delete buffer;
			buffer=NULL;
		}
		buffer =new m4char_t[bufferSize];
		iRet=((ClEventMsg)ai_oEventMsg).Serialize(buffer,bufferSize,outputSize);
	}

	M4_ASSERT(bufferSize<100000);

	if(iRet!=M4_ERROR)
	{
		m_oMutex.Lock();

		m4pchar_t the2point=strchr(buffer,':');
		*the2point='\0';
		the2point++;
		short Corto=0;
		Corto=m_poLogSystem -> SetCode(atoi(buffer), WARNINGLOG,the2point);

		m_oMutex.Unlock();

		m_EventCounter++;
	}

	if(buffer)
		delete buffer;
	
	return iRet;
  //## end ClLogBasedEventHandler::PublishEventMsg%938424982.body
}

IEventHandler * ClLogBasedEventHandler::GetEHInstance ()
{
  //## begin ClLogBasedEventHandler::GetEHInstance%938507460.body preserve=yes
	if(!m_sEventHandlerInstance)
	{
		if(M4_ERROR!=m_oMutex.Lock())
		{
			if(!m_sEventHandlerInstance)
			{
				IEventHandler * test=NULL;
				test=new ClLogBasedEventHandler;
				M4_ASSERT((test) && (m_sEventHandlerInstance));
			}
			if(M4_ERROR==m_oMutex.Unlock())
			{m4Trace(cerr<<"Error unlocking mutex"<<endl);}
		}
		else
		{m4Trace(cerr<<"Error locking mutex"<<endl);}
	}
	
	return m_sEventHandlerInstance;
  //## end ClLogBasedEventHandler::GetEHInstance%938507460.body
}

m4return_t ClLogBasedEventHandler::GetEventMsg (const IEventConsumer &ai_oEventClient, ClEventMsgHistory &ao_oEventMsgHistory, const ClEventTopic &ai_oEventTopic)
{
  //## begin ClLogBasedEventHandler::GetEventMsg%938507461.body preserve=yes
	m4return_t	iRet=M4_SUCCESS;
	m4handle_t	thisconsumer=(m4handle_t)&ai_oEventClient;
	m4uint32_t	lastEvent=0;
	SubscriptorMap::iterator TheIterator;

	m_oMutex.Lock();
	TheIterator=m_pSubscriptorMap.find(thisconsumer);
	
	if(TheIterator==m_pSubscriptorMap.end())
	{
		iRet=M4_ERROR;
		m4Trace(cerr<< "Please, subscribe first"<<endl);
		m_oMutex.Unlock();
	}
	else
	{
		lastEvent=(*TheIterator).second;
		m4TraceLevel(2,cout<<"Last event:="<< lastEvent<<endl)
	
		m_oMutex.Unlock();
		m4uint32_t buffersize=0;
		m_poLogSystem-> GetSerialStringSizeFromMessage(buffersize,lastEvent);
		
		if(buffersize)
		{
			m4pchar_t buffer=new m4char_t[buffersize];
			m4uint32_t lastiter=0;
// OLD INTERFACE			m_poLogSystem-> GetSerialErrorsFromMessage(lastEvent,buffer,buffersize);
			m_poLogSystem-> GetSerialStringFromMessage(buffer,buffersize,lastiter,lastEvent);
			iRet=ao_oEventMsgHistory.Deserialize(buffer,buffersize);
			delete buffer;
		
			m4size_t msgnumber=ao_oEventMsgHistory.GetSize();
			m_pSubscriptorMap[thisconsumer]+=msgnumber;
		}
		else
		{
			iRet=M4_ERROR;
			m4TraceLevel(2,cerr<< "No events"<<endl);
		}

	}
	return iRet;
  //## end ClLogBasedEventHandler::GetEventMsg%938507461.body
}

m4return_t ClLogBasedEventHandler::PublishEventMsg (const IEventProducer &ai_oEventClient, const ClEventMsgHistory &ai_oEventHistory)
{
  //## begin ClLogBasedEventHandler::PublishEventMsg%938507462.body preserve=yes
	m4return_t iRet=M4_SUCCESS;
	
	ClEventMsg theMessage(M4AdminEventTopic);
	iRet=((ClEventMsgHistory)ai_oEventHistory).GetFirstEventMsg(theMessage);
	while(iRet!=M4_ERROR)
	{
		iRet=PublishEventMsg(ai_oEventClient,theMessage);
		if(M4_ERROR==iRet)
			break;
		iRet=((ClEventMsgHistory)ai_oEventHistory).GetNextEventMsg(theMessage);
	}
	
	return iRet;
  //## end ClLogBasedEventHandler::PublishEventMsg%938507462.body
}

m4return_t ClLogBasedEventHandler::Deserialize (m4pcchar_t ai_pcBuffer, m4int_t ai_iBufferSize)
{
  //## begin ClLogBasedEventHandler::Deserialize%939113832.body preserve=yes
	m4return_t iRet=M4_SUCCESS;
	M4_ASSERT((ai_iBufferSize)&& ai_pcBuffer);

	if((!ai_iBufferSize)&& (!ai_pcBuffer))
	{
		m4Trace(cerr<< "parametros erroneos"<<endl);
		iRet=M4_ERROR;
	}
	else
	{
		m_oMutex.Lock();
		m4uint32_t uiMsg=m_poLogSystem -> GetMessageCounter();
		m_poLogSystem -> SetSerialCodeString ( (char *)ai_pcBuffer );
		m4uint32_t added=m_poLogSystem -> GetMessageCounter()-uiMsg;
		m4Trace(cout << "Added "<< added<<" events"<<endl);
		m_EventCounter+=added;
		m_oMutex.Unlock();
	}

	return iRet;
  //## end ClLogBasedEventHandler::Deserialize%939113832.body
}

m4return_t ClLogBasedEventHandler::PublishAnonymousEvent (ClEventTopic ai_Topid, ClEventSubject ai_Subject)
{
  //## begin ClLogBasedEventHandler::PublishAnonymousEvent%940586603.body preserve=yes

	m4return_t iRet=M4_ERROR;
	ClEventTopicID thetopicID;
	ai_Topid.GetTopicID(thetopicID);
	m4Trace(cerr<<"\nUsing old style event interface. Please update." <<endl);
	iRet=M4PublishEvent((char *)thetopicID.c_str(),(char *)ai_Subject.c_str());
	
	return iRet;

	IEventHandler *TheEH=GetEHInstance();
	if(!TheEH)
	{
		m4Trace(cerr<< "NULL EventHandler"<<endl);
	}
	else
	{
	
		ClEventTopicID theTopicID;
		ai_Topid.GetTopicID(theTopicID);

		m4pchar_t buffer=new m4char_t[theTopicID.size()+ai_Subject.size()+6+1+7+10];

		sprintf(buffer,"%s %s  %s %s",M4_EVENT_TOPIC_TOKEN,theTopicID.c_str(),M4_EVENT_SUBJECT_TOKEN,ai_Subject.c_str());
		if(!m_poLogSystem)
		{
			m4Trace(cerr << "NULL ClLogSystem" <<endl);
		}
		else
		{
	m_oMutex.Lock();					

/*
	// Interface antiguo no bloqueante
			iRet=m_poLogSystem -> StartCode(1, WARNINGLOG);
			iRet=m_poLogSystem -> AddText ( buffer );
			short Corto=0;
			Corto=m_poLogSystem -> SetCode();
*/
		short Corto=0;
		Corto=m_poLogSystem -> SetCode(1, WARNINGLOG,buffer);

	m_oMutex.Unlock();
		}

		m_EventCounter++;
		m4Trace(cout<<"________________\n"<<buffer <<"\n--------------"<<endl)
		m4Trace(cout<<"events number:"<<m_EventCounter <<endl);
		m4Trace(cout<<"in Log:"<<m_poLogSystem->GetMessageCounter() <<endl);
		delete buffer;
	}
	return iRet;
  //## end ClLogBasedEventHandler::PublishAnonymousEvent%940586603.body
}

m4return_t ClLogBasedEventHandler::GetEventMsg (const IEventConsumer &ai_oEventClient, m4pchar_t ao_pBuffer, m4int_t &ai_oBufferSize, const ClEventTopic &ai_oEventTopic)
{
  //## begin ClLogBasedEventHandler::GetEventMsg%943015143.body preserve=yes
	m4return_t	iRet=M4_SUCCESS;
	m4handle_t	thisconsumer=(m4handle_t)&ai_oEventClient;
	m4uint32_t	lastEvent=0;
	SubscriptorMap::iterator TheIterator;

	m_oMutex.Lock();
	TheIterator=m_pSubscriptorMap.find(thisconsumer);
	
	if(TheIterator==m_pSubscriptorMap.end())
	{
		iRet=M4_ERROR;
		m4Trace(cerr<< "Please, subscribe first"<<endl);
		m_oMutex.Unlock();
	}
	else
	{
		lastEvent=(*TheIterator).second;
		m4TraceLevel(2,cout<<"Last event:="<< lastEvent<<endl)
	
		m_oMutex.Unlock();
		m4uint32_t buffersize=0;
		m_poLogSystem-> GetSerialStringSizeFromMessage(buffersize,lastEvent);
		
		if(buffersize)
		{
			if(((m4int_t)buffersize)<=ai_oBufferSize)
			{
				m4uint32_t lastiter=0;
				m_poLogSystem-> GetSerialStringFromMessage(ao_pBuffer,ai_oBufferSize,lastiter,lastEvent);
				ai_oBufferSize=buffersize;
				m_pSubscriptorMap[thisconsumer]=lastiter;
			}
			else
			{
				iRet=M4_ERROR;
				ai_oBufferSize=buffersize;
			}
		}
		else
		{
			iRet=M4_ERROR;
			m4TraceLevel(2,cerr<< "No events"<<endl);
		}

	}
	return iRet;
  //## end ClLogBasedEventHandler::GetEventMsg%943015143.body
}

// Additional Declarations
  //## begin ClLogBasedEventHandler%37EF859903BC.declarations preserve=yes
  //## end ClLogBasedEventHandler%37EF859903BC.declarations

//## begin module%37EF83EA034E.epilog preserve=yes
//## end module%37EF83EA034E.epilog

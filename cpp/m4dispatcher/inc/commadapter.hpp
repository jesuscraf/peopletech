//## begin module%39479DC702A4.cm preserve=no
//## end module%39479DC702A4.cm

//## begin module%39479DC702A4.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.hpp
//	 Project:            M4ComSrv
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
//	=
//## end module%39479DC702A4.cp

//## Module: commadapter%39479DC702A4; Package specification
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Source file: z:\m4dispatcher\inc\commadapter.hpp

#ifndef commadapter_h
#define commadapter_h 1

//## begin module%39479DC702A4.additionalIncludes preserve=no
//## end module%39479DC702A4.additionalIncludes

//## begin module%39479DC702A4.includes preserve=yes
#include <dspappctlreq.hpp>
//## end module%39479DC702A4.includes

// m4communication
#include <m4communication.hpp>
// dspagent
#include <dspagent.hpp>
// dspreqcontainer
#include <dspreqcontainer.hpp>

class ClDispatcherInvoker;

//## begin module%39479DC702A4.declarations preserve=no
//## end module%39479DC702A4.declarations

//## begin module%39479DC702A4.additionalDeclarations preserve=yes
//## end module%39479DC702A4.additionalDeclarations


//## begin ClDptcherCommsAdaptor%39476AC903E4.preface preserve=yes
//## end ClDptcherCommsAdaptor%39476AC903E4.preface

//## Class: ClDptcherCommsAdaptor%39476AC903E4
//## Category: M4Dispatcher::Agents%39F451CA0292
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39479ADC028C;ClDispatcherInvoker { -> F}

class ClDptcherCommsAdaptor : public ClDspAgent  //## Inherits: <unnamed>%396C31980338
{
  //## begin ClDptcherCommsAdaptor%39476AC903E4.initialDeclarations preserve=yes
  //## end ClDptcherCommsAdaptor%39476AC903E4.initialDeclarations

  public:
    //## begin ClDptcherCommsAdaptor::eCommActions%3947B4AE0060.preface preserve=yes
    //## end ClDptcherCommsAdaptor::eCommActions%3947B4AE0060.preface

    //## Class: eCommActions%3947B4AE0060
    //## Category: M4Dispatcher::Agents%39F451CA0292
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum  { POST_ACCEPT_PDU_ACTION_ID , POST_ACCEPT_XML_ACTION_ID , POST_READ_PDU_ACTION_ID , POST_READ_XML_ACTION_ID , POST_WRITE_PDU_ACTION_ID , POST_WRITE_XML_ACTION_ID , REPLY_PDU_ID , REPLY_XML_ID , SEND_XML_TO_APPCTL_ID , CLOSE_CONNECTION_ID , CONFIGURE_PORTS_ACTION_ID , POST_READ_APPCTL_ACTION_ID , APPCTL_REPLY_ID , TOTAL_ACTION } eCommActions;

    //## begin ClDptcherCommsAdaptor::eCommActions%3947B4AE0060.postscript preserve=yes
    //## end ClDptcherCommsAdaptor::eCommActions%3947B4AE0060.postscript

    //## Constructors (specified)
      //## Operation: ClDptcherCommsAdaptor%960967024
      ClDptcherCommsAdaptor (ClSiteNodeRepository *ai_poServerRepository, ClDspExecutiveAgent *ai_poExecutiveAgent);

    //## Destructor (specified)
      //## Operation: ~ClDptcherCommsAdaptor%960967025
      ~ClDptcherCommsAdaptor ();


    //## Other Operations (specified)
      //## Operation: PostReadClientPDU%960967026
      m4return_t PostReadClientPDU (ClParamList *ai_pParamList);

      //## Operation: PostReadXML%960967027
      m4return_t PostReadXML (ClParamList *ai_pParamList);

      //## Operation: PostAcceptClientPDU%960967028
      m4return_t PostAcceptClientPDU (ClParamList *ai_pParamList);

      //## Operation: PostAccepXML%960967029
      m4return_t PostAccepXML (ClParamList *ai_pParamList);

      //## Operation: PostWriteClientPDU%960967030
      m4return_t PostWriteClientPDU (ClParamList *ai_pParamList);

      //## Operation: PostWriteXML%960967031
      m4return_t PostWriteXML (ClParamList *ai_pParamList);

      //## Operation: SendXMLReply%960967032
      m4return_t SendXMLReply (ClParamList *ai_pParams);

      //## Operation: SendPDUReplay%960967033
      m4return_t SendPDUReplay (ClParamList *ai_pParams);

      //## Operation: Start%960967040
      virtual m4return_t Start ();

      //## Operation: SendXMLtoAppCtl%962179780
      m4return_t SendXMLtoAppCtl (m4pchar_t ai_szHost, m4uint32_t ai_iPort, ClDspAppCtlRequest *ai_poRequest);

      //## Operation: GetHostName%962179784
      m4pchar_t GetHostName ();

      //## Operation: ProcessAction%963314074
      virtual m4return_t ProcessAction (ClActionIdentifier ai_oActionIdentifier, ClParamList *ai_poParamList, ClBaseAction *&ao_poAction);

      //## Operation: CloseConnection%963417633
      m4return_t CloseConnection (ClParamList *ai_pParamList);

      //## Operation: StopCommunicationLayer%972919262
      m4return_t StopCommunicationLayer ();

      //## Operation: IsStarted%972986737
      m4bool_t IsStarted ();

      //## Operation: PostReadAppctl%981385469
      m4return_t PostReadAppctl (ClParamList *ai_pParamList);

      //## Operation: ProcessAppctlReply%981535909
      m4return_t ProcessAppctlReply (m4return_t ai_iResult, m4uint64_t ai_iRequestID, ClDspRequest *ai_poRequest, m4pchar_t ai_szAppCtlLog);

      //## Operation: InitComms%1000797470
      m4return_t InitComms (m4pchar_t ai_szCypherMethod, m4uint32_t ai_iNumWorkers);

  public:
    // Additional Public Declarations
      //## begin ClDptcherCommsAdaptor%39476AC903E4.public preserve=yes
      //## end ClDptcherCommsAdaptor%39476AC903E4.public

  protected:
    // Data Members for Associations

      //## Association: M4Dispatcher::<unnamed>%39744B0401A9
      //## Role: ClDptcherCommsAdaptor::m_oReqContainer%39744B0403A8
      //## begin ClDptcherCommsAdaptor::m_oReqContainer%39744B0403A8.role preserve=no  protected: DspReqContainer { -> VHAN}
      DspReqContainer m_oReqContainer;
      //## end ClDptcherCommsAdaptor::m_oReqContainer%39744B0403A8.role

    // Additional Protected Declarations
      //## begin ClDptcherCommsAdaptor%39476AC903E4.protected preserve=yes
      //## end ClDptcherCommsAdaptor%39476AC903E4.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_szHost%395A217A036A
      //## begin ClDptcherCommsAdaptor::m_szHost%395A217A036A.attr preserve=no  private: m4pchar_t {UA} 
      m4pchar_t m_szHost;
      //## end ClDptcherCommsAdaptor::m_szHost%395A217A036A.attr

      //## Attribute: m_bStarted%39FEA3820374
      //## begin ClDptcherCommsAdaptor::m_bStarted%39FEA3820374.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bStarted;
      //## end ClDptcherCommsAdaptor::m_bStarted%39FEA3820374.attr

      //## Attribute: m_szCypherMethod%3B8125AA02EF
      //## begin ClDptcherCommsAdaptor::m_szCypherMethod%3B8125AA02EF.attr preserve=no  private: m4string_t {UA} 
      m4string_t m_szCypherMethod;
      //## end ClDptcherCommsAdaptor::m_szCypherMethod%3B8125AA02EF.attr

    // Data Members for Associations

      //## Association: M4Dispatcher::<unnamed>%39476B03001C
      //## Role: ClDptcherCommsAdaptor::m_poCommunications%39476B0401B8
      //## begin ClDptcherCommsAdaptor::m_poCommunications%39476B0401B8.role preserve=no  private: ClCommunication { -> RHAN}
      ClCommunication *m_poCommunications;
      //## end ClDptcherCommsAdaptor::m_poCommunications%39476B0401B8.role

    // Additional Private Declarations
      //## begin ClDptcherCommsAdaptor%39476AC903E4.private preserve=yes
      //## end ClDptcherCommsAdaptor%39476AC903E4.private

  private: //## implementation

  protected:
	  
	  // eh 165963
	  m4return_t	CheckAdminHost( m4pcchar_t ai_pccHostName, m4pcchar_t ai_pccHostIP, m4bool_t& ao_bAllowed ) ;

    // Additional Implementation Declarations
      //## begin ClDptcherCommsAdaptor%39476AC903E4.implementation preserve=yes
      //## end ClDptcherCommsAdaptor%39476AC903E4.implementation

};

//## begin ClDptcherCommsAdaptor%39476AC903E4.postscript preserve=yes
//## end ClDptcherCommsAdaptor%39476AC903E4.postscript

// Class ClDptcherCommsAdaptor 


//## Other Operations (inline)
inline m4pchar_t ClDptcherCommsAdaptor::GetHostName ()
{
  //## begin ClDptcherCommsAdaptor::GetHostName%962179784.body preserve=yes
	return m_szHost ;
  //## end ClDptcherCommsAdaptor::GetHostName%962179784.body
}

//## begin module%39479DC702A4.epilog preserve=yes
//## end module%39479DC702A4.epilog


#endif

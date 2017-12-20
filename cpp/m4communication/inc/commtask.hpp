//## begin module%38DB62A501BC.cm preserve=no
//## end module%38DB62A501BC.cm

//## begin module%38DB62A501BC.cp preserve=no
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
//## end module%38DB62A501BC.cp

//## Module: CommTask%38DB62A501BC; Package specification
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Source file: e:\mybuild\m4communication\inc\commtask.hpp

#ifndef commtask_h
#define commtask_h 1

//## begin module%38DB62A501BC.additionalIncludes preserve=no
//## end module%38DB62A501BC.additionalIncludes

//## begin module%38DB62A501BC.includes preserve=yes
#include <time.h>
#include <commactionsname.h>
#include <m4trace.hpp>
//## end module%38DB62A501BC.includes

// commbase
#include <commbase.hpp>
// m4types
#include <m4types.hpp>
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
// iexecutable
#include <iexecutable.hpp>
// CommComun
#include <commcomun.hpp>
// ParamList
#include <paramlist.hpp>
// basepdu
#include <basepdus.hpp>

class ClCommAction;

//## begin module%38DB62A501BC.declarations preserve=no
//## end module%38DB62A501BC.declarations

//## begin module%38DB62A501BC.additionalDeclarations preserve=yes
//## end module%38DB62A501BC.additionalDeclarations


//## begin eCommTaskState%38DB4D000095.preface preserve=yes
//## end eCommTaskState%38DB4D000095.preface

//## Class: eCommTaskState%38DB4D000095
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef enum{NONE,ACCEPTING,ACCEPTED,ERROR_ACCEPTING,WAITING_FOR_READING,READING,PAUSE_READING,START_WRITTING,WRITTING,CLOSING,OPENING,ERROR_OPENING,CONNECTING,CONNECTED,NO_CONNECTED,FINISHING_WRITTING,FINISH_ALL,DETECTED_CLOSED,REQUEST_PENDING,DETECTED_CLOSED_AND_REQUEST_PENDING} eCommTaskState;

//## begin eCommTaskState%38DB4D000095.postscript preserve=yes
//## end eCommTaskState%38DB4D000095.postscript

//## begin ClCommTask%38D9E0840037.preface preserve=yes
//## end ClCommTask%38D9E0840037.preface

//## Class: ClCommTask%38D9E0840037
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%399A914C0265;ClCommAction { -> F}

class ClCommTask : public ClCommRecyclable  //## Inherits: <unnamed>%399176810313
{
  //## begin ClCommTask%38D9E0840037.initialDeclarations preserve=yes
  //## end ClCommTask%38D9E0840037.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommTask%955103966; C++
      ClCommTask (m4int_t ai_iRecycleObjID = -1);

    //## Destructor (specified)
      //## Operation: ~ClCommTask%955103967
      virtual ~ClCommTask ();


    //## Other Operations (specified)
      //## Operation: GetTLIID%955613879
      m4tliid_t GetTLIID ();

      //## Operation: GetCommEventHandler%955613880
      ClCommEventHandler* GetCommEventHandler ();

      //## Operation: SetState%955613889; C++
      m4return_t SetState (eCommTaskState ai_eState);

      //## Operation: GetState%956672702; C++
      eCommTaskState GetState ();

      //## Operation: SetParams%957429935; C++
      m4return_t SetParams (ClParamList *ai_pParamList);

      //## Operation: GetParams%957429936; C++
      ClParamList * GetParams ();

      //## Operation: RemoveParams%958463331; C++
      m4return_t RemoveParams ();

      //## Operation: SetCondition%965219980; C++
      m4return_t SetCondition (ClCommCondition *ai_poCondition);

      //## Operation: GetCondition%965219981; C++
      ClCommCondition * GetCondition ();

      //## Operation: SetActionName%965219982; C++
      m4return_t SetActionName (m4pchar_t ai_szActionName);

      //## Operation: GetActionName%965219983; C++
      m4pcchar_t GetActionName ();

      //## Operation: SetExecutable%965219984; C++
      m4return_t SetExecutable (ClExecutableInterface *ai_pExecutable);

      //## Operation: GetExecutable%965219985; C++
      ClExecutableInterface * GetExecutable ();

      //## Operation: SetCommProtocol%965284296; C++
      m4return_t SetCommProtocol (ClCommProtocol *ai_pCommProtocol);

      //## Operation: GetCommProtocolType%965284297
      m4int_t GetCommProtocolType ();

      //## Operation: GetCommProtocol%965284298
      ClCommProtocol * GetCommProtocol ();

      //## Operation: SetTLIID%965731402; C++
      m4return_t SetTLIID (m4tliid_t ai_TLIID);

      //## Operation: Recycle%965834586; C++
      m4return_t Recycle ();

      //## Operation: SetConnectionID%965898337; C++
      m4return_t SetConnectionID (m4handle_t ai_hConnectionID);

      //## Operation: GetConnectionID%965898338
      m4handle_t GetConnectionID ();

    // Data Members for Associations

      //## Association: M4Communication::M4COML::<unnamed>%3992D0AB001A
      //## Role: ClCommTask::m_TLIID%3992D0AB038B
      //## begin ClCommTask::m_TLIID%3992D0AB038B.role preserve=no  public: m4tliid_t { -> VHAN}
      m4tliid_t m_TLIID;
      //## end ClCommTask::m_TLIID%3992D0AB038B.role

      //## Association: M4Communication::M4COML::<unnamed>%3992D0AE01C2
      //## Role: ClCommTask::m_hConnectionID%3992D0AF0083
      //## begin ClCommTask::m_hConnectionID%3992D0AF0083.role preserve=no  public: m4handle_t { -> VHAN}
      m4handle_t m_hConnectionID;
      //## end ClCommTask::m_hConnectionID%3992D0AF0083.role

    // Additional Public Declarations
      //## begin ClCommTask%38D9E0840037.public preserve=yes
	  m4return_t Notify(ClParamList *ai_poParams);
      //## end ClCommTask%38D9E0840037.public
  protected:

    //## Other Operations (specified)
      //## Operation: _PassReturnInfo%966411265; C++
      m4return_t _PassReturnInfo (ClCommAction* ai_pAction);

    // Data Members for Associations

      //## Association: M4Communication::M4COML::<unnamed>%38DB4D7002F8
      //## Role: ClCommTask::m_State%38DB4D7100AB
      //## begin ClCommTask::m_State%38DB4D7100AB.role preserve=no  protected: eCommTaskState { -> UHAN}
      eCommTaskState m_State;
      //## end ClCommTask::m_State%38DB4D7100AB.role

      //## Association: M4Communication::M4COML::<unnamed>%38EDC7300195
      //## Role: ClCommTask::m_poCommEventHandler%38EDC7300380
      //## begin ClCommTask::m_poCommEventHandler%38EDC7300380.role preserve=no  protected: ClCommEventHandler { -> RHAN}
      ClCommEventHandler *m_poCommEventHandler;
      //## end ClCommTask::m_poCommEventHandler%38EDC7300380.role

      //## Association: M4Communication::M4COML::<unnamed>%39113AED019F
      //## Role: ClCommTask::m_pParamList%39113AED03D0
      //## begin ClCommTask::m_pParamList%39113AED03D0.role preserve=no  protected: ClParamList { -> 1RHAN}
      ClParamList *m_pParamList;
      //## end ClCommTask::m_pParamList%39113AED03D0.role

      //## Association: M4Communication::M4COML::<unnamed>%3912BA8B0113
      //## Role: ClCommTask::m_pCommProtocol%3912BA8B03C6
      //## begin ClCommTask::m_pCommProtocol%3912BA8B03C6.role preserve=no  protected: ClCommProtocol { -> 1RHAN}
      ClCommProtocol *m_pCommProtocol;
      //## end ClCommTask::m_pCommProtocol%3912BA8B03C6.role

      //## Association: M4Communication::M4COML::<unnamed>%39884E7F038A
      //## Role: ClCommTask::m_strActionName%39884E8001AB
      //## begin ClCommTask::m_strActionName%39884E8001AB.role preserve=no  protected: m4string_t { -> VHAN}
      m4string_t m_strActionName;
      //## end ClCommTask::m_strActionName%39884E8001AB.role

      //## Association: M4Communication::M4COML::<unnamed>%39884EA001B1
      //## Role: ClCommTask::m_pExecutableInterface%39884EA1007D
      //## begin ClCommTask::m_pExecutableInterface%39884EA1007D.role preserve=no  protected: ClExecutableInterface { -> RHAN}
      ClExecutableInterface *m_pExecutableInterface;
      //## end ClCommTask::m_pExecutableInterface%39884EA1007D.role

      //## Association: M4Communication::M4COML::<unnamed>%39884EC802DB
      //## Role: ClCommTask::m_poCondition%39884EC901A6
      //## begin ClCommTask::m_poCondition%39884EC901A6.role preserve=no  protected: ClCommCondition { -> RHAN}
      ClCommCondition *m_poCondition;
      //## end ClCommTask::m_poCondition%39884EC901A6.role

    // Additional Protected Declarations
      //## begin ClCommTask%38D9E0840037.protected preserve=yes
      //## end ClCommTask%38D9E0840037.protected

  private:
    // Additional Private Declarations
      //## begin ClCommTask%38D9E0840037.private preserve=yes
      //## end ClCommTask%38D9E0840037.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommTask%38D9E0840037.implementation preserve=yes
      //## end ClCommTask%38D9E0840037.implementation

};

//## begin ClCommTask%38D9E0840037.postscript preserve=yes
//## end ClCommTask%38D9E0840037.postscript

//## begin ClCommReadingTask%38DB4DEC00E4.preface preserve=yes
//## end ClCommReadingTask%38DB4DEC00E4.preface

//## Class: ClCommReadingTask%38DB4DEC00E4
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommReadingTask : public ClCommTask  //## Inherits: <unnamed>%38DB4F9302E1
{
  //## begin ClCommReadingTask%38DB4DEC00E4.initialDeclarations preserve=yes
  //## end ClCommReadingTask%38DB4DEC00E4.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommReadingTask%955103968; C++
      ClCommReadingTask ();

    //## Destructor (specified)
      //## Operation: ~ClCommReadingTask%955103969
      ~ClCommReadingTask ();


    //## Other Operations (specified)
      //## Operation: Readed%957517148; C++
      m4return_t Readed (m4int_t ai_iReadedBytes);

      //## Operation: CloseSocket%959072020; C++
      m4return_t CloseSocket (m4bool_t ai_bExternalClose = M4_FALSE);

      //## Operation: NotifyReaded%966517539
      m4return_t NotifyReaded ();

    // Additional Public Declarations
      //## begin ClCommReadingTask%38DB4DEC00E4.public preserve=yes
      //## end ClCommReadingTask%38DB4DEC00E4.public

  protected:
    // Data Members for Associations

      //## Association: M4Communication::M4COML::<unnamed>%38DB4F0100B0
      //## Role: ClCommReadingTask::m_BufferSize%38DB4F0201F3
      //## begin ClCommReadingTask::m_BufferSize%38DB4F0201F3.role preserve=no  protected: size_t { -> UHAN}
      size_t m_BufferSize;
      //## end ClCommReadingTask::m_BufferSize%38DB4F0201F3.role

      //## Association: M4Communication::M4COML::<unnamed>%38DB4F4000A7
      //## Role: ClCommReadingTask::m_Processed%38DB4F410031
      //## begin ClCommReadingTask::m_Processed%38DB4F410031.role preserve=no  protected: size_t { -> UHAN}
      size_t m_Processed;
      //## end ClCommReadingTask::m_Processed%38DB4F410031.role

      //## Association: M4Communication::M4COML::<unnamed>%38DB4F5B039F
      //## Role: ClCommReadingTask::m_Readed%38DB4F5C0239
      //## begin ClCommReadingTask::m_Readed%38DB4F5C0239.role preserve=no  protected: size_t { -> UHAN}
      size_t m_Readed;
      //## end ClCommReadingTask::m_Readed%38DB4F5C0239.role

      //## Association: M4Communication::M4COML::<unnamed>%38DB4ED50189
      //## Role: ClCommReadingTask::m_pcBuffer%38DB4ED60055
      //## begin ClCommReadingTask::m_pcBuffer%38DB4ED60055.role preserve=no  protected: m4char_t { -> RHAN}
      m4char_t *m_pcBuffer;
      //## end ClCommReadingTask::m_pcBuffer%38DB4ED60055.role

      //## Association: M4Communication::M4COML::<unnamed>%39F8124403A4
      //## Role: ClCommReadingTask::m_pcReadedHTTPHeader%39F8124502D3
      //## begin ClCommReadingTask::m_pcReadedHTTPHeader%39F8124502D3.role preserve=no  protected: m4char_t { -> UHAN}
      m4char_t m_pcReadedHTTPHeader;
      //## end ClCommReadingTask::m_pcReadedHTTPHeader%39F8124502D3.role

    // Additional Protected Declarations
      //## begin ClCommReadingTask%38DB4DEC00E4.protected preserve=yes
	  void PrepareNotifyReaded();
      //## end ClCommReadingTask%38DB4DEC00E4.protected
  private:
    // Additional Private Declarations
      //## begin ClCommReadingTask%38DB4DEC00E4.private preserve=yes
      //## end ClCommReadingTask%38DB4DEC00E4.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommReadingTask%38DB4DEC00E4.implementation preserve=yes
      //## end ClCommReadingTask%38DB4DEC00E4.implementation

};

//## begin ClCommReadingTask%38DB4DEC00E4.postscript preserve=yes
//## end ClCommReadingTask%38DB4DEC00E4.postscript

//## begin ClCommWrittingTask%38DB4FA00362.preface preserve=yes
//## end ClCommWrittingTask%38DB4FA00362.preface

//## Class: ClCommWrittingTask%38DB4FA00362
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommWrittingTask : public ClCommTask  //## Inherits: <unnamed>%38DB514E0035
{
  //## begin ClCommWrittingTask%38DB4FA00362.initialDeclarations preserve=yes
  //## end ClCommWrittingTask%38DB4FA00362.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommWrittingTask%955103970
      ClCommWrittingTask ();

    //## Destructor (specified)
      //## Operation: ~ClCommWrittingTask%955103971
      ~ClCommWrittingTask ();


    //## Other Operations (specified)
      //## Operation: Written%957944570; C++
      m4return_t Written (m4return_t ai_retValue);

      //## Operation: PrepareWrite%965284295
      m4return_t PrepareWrite ();

      //## Operation: SetRequestID%966411259; C++
      m4return_t SetRequestID (m4uint64_t ai_ui64RequestID);

      //## Operation: GetRequestID%966411260
      m4uint64_t GetRequestID ();

      //## Operation: SetPDU%966411261; C++
      m4return_t SetPDU (ClPDU *ai_pPDU);

      //## Operation: GetPDU%966411262
      ClPDU * GetPDU ();

      //## Operation: SetBufferToWrite%966411263; C++
      m4return_t SetBufferToWrite (m4pchar_t ai_pcBuffer, m4int_t ai_iBufferSize);

      //## Operation: GetBufferToWrite%966411264; C++
      m4return_t GetBufferToWrite (m4pchar_t &ai_pcBuffer, m4int_t &ai_iBufferSize);

      //## Operation: SetHeaders%972554300; C++
      m4return_t SetHeaders (m4pchar_t ai_pcHeaders);

      //## Operation: GetHeaders%972554301; C++
      m4pchar_t GetHeaders ();

    // Data Members for Associations

      //## Association: M4Communication::M4COML::<unnamed>%399A821A037A
      //## Role: ClCommWrittingTask::m_poPDU%399A821B02EF
      //## begin ClCommWrittingTask::m_poPDU%399A821B02EF.role preserve=no  public: ClPDU { -> RHAN}
      ClPDU *m_poPDU;
      //## end ClCommWrittingTask::m_poPDU%399A821B02EF.role

    // Additional Public Declarations
      //## begin ClCommWrittingTask%38DB4FA00362.public preserve=yes
	  m4return_t PrepareReturnParams(m4return_t ai_retValue);
      //## end ClCommWrittingTask%38DB4FA00362.public
  protected:

    //## Other Operations (specified)
      //## Operation: SetBuffer%957517147; C++
      m4return_t SetBuffer (m4pchar_t ai_pcBuffer, m4uint_t ai_uiBufferSize);

    // Data Members for Associations

      //## Association: M4Communication::M4COML::<unnamed>%399A8124031C
      //## Role: ClCommWrittingTask::m_ui64RequestID%399A8125031E
      //## begin ClCommWrittingTask::m_ui64RequestID%399A8125031E.role preserve=no  protected: m4uint64_t { -> VHAN}
      m4uint64_t m_ui64RequestID;
      //## end ClCommWrittingTask::m_ui64RequestID%399A8125031E.role

      //## Association: M4Communication::M4COML::<unnamed>%399A866D00D3
      //## Role: ClCommWrittingTask::m_pcWrittingBuffer%399A866F01A8
      //## begin ClCommWrittingTask::m_pcWrittingBuffer%399A866F01A8.role preserve=no  protected: m4char_t { -> RHAN}
      m4char_t *m_pcWrittingBuffer;
      //## end ClCommWrittingTask::m_pcWrittingBuffer%399A866F01A8.role

      //## Association: M4Communication::M4COML::<unnamed>%399A86B10117
      //## Role: ClCommWrittingTask::m_iBufferSize%399A86B103A2
      //## begin ClCommWrittingTask::m_iBufferSize%399A86B103A2.role preserve=no  protected: m4int_t { -> VHAN}
      m4int_t m_iBufferSize;
      //## end ClCommWrittingTask::m_iBufferSize%399A86B103A2.role

      //## Association: M4Communication::M4COML::<unnamed>%39F811FD03D4
      //## Role: ClCommWrittingTask::m_pcWriteHTTPHeaders%39F811FE0321
      //## begin ClCommWrittingTask::m_pcWriteHTTPHeaders%39F811FE0321.role preserve=no  protected: m4char_t { -> RHAN}
      m4char_t *m_pcWriteHTTPHeaders;
      //## end ClCommWrittingTask::m_pcWriteHTTPHeaders%39F811FE0321.role

    // Additional Protected Declarations
      //## begin ClCommWrittingTask%38DB4FA00362.protected preserve=yes
      //## end ClCommWrittingTask%38DB4FA00362.protected

  private:
    // Additional Private Declarations
      //## begin ClCommWrittingTask%38DB4FA00362.private preserve=yes
      //## end ClCommWrittingTask%38DB4FA00362.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommWrittingTask%38DB4FA00362.implementation preserve=yes
      //## end ClCommWrittingTask%38DB4FA00362.implementation

};

//## begin ClCommWrittingTask%38DB4FA00362.postscript preserve=yes
//## end ClCommWrittingTask%38DB4FA00362.postscript

//## begin ClCommOpenPortTask%38DB51640073.preface preserve=yes
//## end ClCommOpenPortTask%38DB51640073.preface

//## Class: ClCommOpenPortTask%38DB51640073
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommOpenPortTask : public ClCommTask  //## Inherits: <unnamed>%38E06BD5013D
{
  //## begin ClCommOpenPortTask%38DB51640073.initialDeclarations preserve=yes
  //## end ClCommOpenPortTask%38DB51640073.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommOpenPortTask%955103972
      ClCommOpenPortTask ();

    //## Destructor (specified)
      //## Operation: ~ClCommOpenPortTask%955103973
      ~ClCommOpenPortTask ();


    //## Other Operations (specified)
      //## Operation: GetCommProtocolType%957345710
      m4int_t GetCommProtocolType ();

      //## Operation: SetPort%966411246; C++
      m4return_t SetPort (m4int_t ai_iPort);

      //## Operation: SetCommProtocolType%966411247; C++
      m4return_t SetCommProtocolType (m4int_t ai_iProtocolType);

      //## Operation: GetPort%966411248
      m4int_t GetPort ();

      //## Operation: SetSSLParams%966411250; C++
      m4return_t SetSSLParams (m4pchar_t ai_strSSLPrivKey, m4pchar_t ai_strSSLCert, m4pchar_t ai_strSSLCipher);

      //## Operation: GetSSLParams%966411251; C++
      m4return_t GetSSLParams (m4pchar_t& ai_strSSLPrivKey, m4pchar_t &ai_strSSLCert, m4pchar_t &ai_strSSLCipher);

    // Additional Public Declarations
      //## begin ClCommOpenPortTask%38DB51640073.public preserve=yes
      //## end ClCommOpenPortTask%38DB51640073.public

  protected:
    // Data Members for Associations

      //## Association: M4Communication::M4COML::<unnamed>%399A4B4303E1
      //## Role: ClCommOpenPortTask::m_iPort%399A4B440374
      //## begin ClCommOpenPortTask::m_iPort%399A4B440374.role preserve=no  protected: m4int_t { -> VHAN}
      m4int_t m_iPort;
      //## end ClCommOpenPortTask::m_iPort%399A4B440374.role

      //## Association: M4Communication::M4COML::<unnamed>%399A4B730245
      //## Role: ClCommOpenPortTask::m_pcCiphers%399A4B7401C6
      //## begin ClCommOpenPortTask::m_pcCiphers%399A4B7401C6.role preserve=no  protected: m4char_t { -> RHAN}
      m4char_t *m_pcCiphers;
      //## end ClCommOpenPortTask::m_pcCiphers%399A4B7401C6.role

      //## Association: M4Communication::M4COML::<unnamed>%399A4B9D030E
      //## Role: ClCommOpenPortTask::m_pcSSLCert%399A4B9E01D9
      //## begin ClCommOpenPortTask::m_pcSSLCert%399A4B9E01D9.role preserve=no  protected: m4char_t { -> RHAN}
      m4char_t *m_pcSSLCert;
      //## end ClCommOpenPortTask::m_pcSSLCert%399A4B9E01D9.role

      //## Association: M4Communication::M4COML::<unnamed>%399A4BB2021E
      //## Role: ClCommOpenPortTask::m_pcPrivKey%399A4BB3012F
      //## begin ClCommOpenPortTask::m_pcPrivKey%399A4BB3012F.role preserve=no  protected: m4char_t { -> RHAN}
      m4char_t *m_pcPrivKey;
      //## end ClCommOpenPortTask::m_pcPrivKey%399A4BB3012F.role

      //## Association: M4Communication::M4COML::<unnamed>%399A4BEE0011
      //## Role: ClCommOpenPortTask::m_iProtocolType%399A4BF001D7
      //## begin ClCommOpenPortTask::m_iProtocolType%399A4BF001D7.role preserve=no  protected: m4int_t { -> VHAN}
      m4int_t m_iProtocolType;
      //## end ClCommOpenPortTask::m_iProtocolType%399A4BF001D7.role

    // Additional Protected Declarations
      //## begin ClCommOpenPortTask%38DB51640073.protected preserve=yes
      //## end ClCommOpenPortTask%38DB51640073.protected

  private:
    // Additional Private Declarations
      //## begin ClCommOpenPortTask%38DB51640073.private preserve=yes
      //## end ClCommOpenPortTask%38DB51640073.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommOpenPortTask%38DB51640073.implementation preserve=yes
      //## end ClCommOpenPortTask%38DB51640073.implementation

};

//## begin ClCommOpenPortTask%38DB51640073.postscript preserve=yes
//## end ClCommOpenPortTask%38DB51640073.postscript

//## begin ClCommCloseSocketTask%38DB51740184.preface preserve=yes
//## end ClCommCloseSocketTask%38DB51740184.preface

//## Class: ClCommCloseSocketTask%38DB51740184
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommCloseSocketTask : public ClCommTask  //## Inherits: <unnamed>%38E06BEE0369
{
  //## begin ClCommCloseSocketTask%38DB51740184.initialDeclarations preserve=yes
  //## end ClCommCloseSocketTask%38DB51740184.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommCloseSocketTask%955103974
      ClCommCloseSocketTask ();

    //## Destructor (specified)
      //## Operation: ~ClCommCloseSocketTask%955103975
      ~ClCommCloseSocketTask ();

    // Additional Public Declarations
      //## begin ClCommCloseSocketTask%38DB51740184.public preserve=yes
      //## end ClCommCloseSocketTask%38DB51740184.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommCloseSocketTask%38DB51740184.protected preserve=yes
      //## end ClCommCloseSocketTask%38DB51740184.protected

  private:
    // Additional Private Declarations
      //## begin ClCommCloseSocketTask%38DB51740184.private preserve=yes
      //## end ClCommCloseSocketTask%38DB51740184.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommCloseSocketTask%38DB51740184.implementation preserve=yes
      //## end ClCommCloseSocketTask%38DB51740184.implementation

};

//## begin ClCommCloseSocketTask%38DB51740184.postscript preserve=yes
//## end ClCommCloseSocketTask%38DB51740184.postscript

//## begin ClCommClosePortTask%38DB51940144.preface preserve=yes
//## end ClCommClosePortTask%38DB51940144.preface

//## Class: ClCommClosePortTask%38DB51940144
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommClosePortTask : public ClCommTask  //## Inherits: <unnamed>%38E06BDA0054
{
  //## begin ClCommClosePortTask%38DB51940144.initialDeclarations preserve=yes
  //## end ClCommClosePortTask%38DB51940144.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommClosePortTask%955103976
      ClCommClosePortTask ();

    //## Destructor (specified)
      //## Operation: ~ClCommClosePortTask%955103977
      ~ClCommClosePortTask ();

    // Additional Public Declarations
      //## begin ClCommClosePortTask%38DB51940144.public preserve=yes
      //## end ClCommClosePortTask%38DB51940144.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommClosePortTask%38DB51940144.protected preserve=yes
      //## end ClCommClosePortTask%38DB51940144.protected

  private:
    // Additional Private Declarations
      //## begin ClCommClosePortTask%38DB51940144.private preserve=yes
      //## end ClCommClosePortTask%38DB51940144.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommClosePortTask%38DB51940144.implementation preserve=yes
      //## end ClCommClosePortTask%38DB51940144.implementation

};

//## begin ClCommClosePortTask%38DB51940144.postscript preserve=yes
//## end ClCommClosePortTask%38DB51940144.postscript

//## begin ClCommOpeningSocketTask%38DB523002CF.preface preserve=yes
//## end ClCommOpeningSocketTask%38DB523002CF.preface

//## Class: ClCommOpeningSocketTask%38DB523002CF
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommOpeningSocketTask : public ClCommTask  //## Inherits: <unnamed>%38E06BDD03DD
{
  //## begin ClCommOpeningSocketTask%38DB523002CF.initialDeclarations preserve=yes
  //## end ClCommOpeningSocketTask%38DB523002CF.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommOpeningSocketTask%955103978
      ClCommOpeningSocketTask ();

    //## Destructor (specified)
      //## Operation: ~ClCommOpeningSocketTask%955103979
      ~ClCommOpeningSocketTask ();


    //## Other Operations (specified)
      //## Operation: GetCommProtocolType%957345711
      m4int_t GetCommProtocolType ();

      //## Operation: SetCommProtocolType%966411252; C++
      m4return_t SetCommProtocolType (m4int_t ai_iCommProtocolType);

      //## Operation: GetSSLCert%966411253
      m4pchar_t GetSSLCert ();

      //## Operation: SetPort%966411254; C++
      m4return_t SetPort (m4int_t ai_iPort);

      //## Operation: GetPort%966411255
      m4int_t GetPort ();

      //## Operation: SetSSLCert%966411256; C++
      m4return_t SetSSLCert (m4pchar_t ai_pcSSLCert);

      //## Operation: SetHost%966411257; C++
      m4return_t SetHost (m4pchar_t ai_pcHost);

      //## Operation: GetHost%966411258; C++
      m4pchar_t GetHost ();

    // Additional Public Declarations
      //## begin ClCommOpeningSocketTask%38DB523002CF.public preserve=yes
      //## end ClCommOpeningSocketTask%38DB523002CF.public

  protected:
    // Data Members for Associations

      //## Association: M4Communication::M4COML::<unnamed>%399A6E9C0311
      //## Role: ClCommOpeningSocketTask::m_iPort%399A6E9D038A
      //## begin ClCommOpeningSocketTask::m_iPort%399A6E9D038A.role preserve=no  protected: m4int_t { -> VHAN}
      m4int_t m_iPort;
      //## end ClCommOpeningSocketTask::m_iPort%399A6E9D038A.role

      //## Association: M4Communication::M4COML::<unnamed>%399A6ED100A0
      //## Role: ClCommOpeningSocketTask::m_pcHost%399A6ED20029
      //## begin ClCommOpeningSocketTask::m_pcHost%399A6ED20029.role preserve=no  protected: m4char_t { -> RHAN}
      m4char_t *m_pcHost;
      //## end ClCommOpeningSocketTask::m_pcHost%399A6ED20029.role

      //## Association: M4Communication::M4COML::<unnamed>%399A6F660221
      //## Role: ClCommOpeningSocketTask::m_iProtocolType%399A6F68035A
      //## begin ClCommOpeningSocketTask::m_iProtocolType%399A6F68035A.role preserve=no  protected: m4int_t { -> VHAN}
      m4int_t m_iProtocolType;
      //## end ClCommOpeningSocketTask::m_iProtocolType%399A6F68035A.role

      //## Association: M4Communication::M4COML::<unnamed>%399A705602D0
      //## Role: ClCommOpeningSocketTask::m_pcSSLCert%399A70570209
      //## begin ClCommOpeningSocketTask::m_pcSSLCert%399A70570209.role preserve=no  protected: m4char_t { -> RHAN}
      m4char_t *m_pcSSLCert;
      //## end ClCommOpeningSocketTask::m_pcSSLCert%399A70570209.role

    // Additional Protected Declarations
      //## begin ClCommOpeningSocketTask%38DB523002CF.protected preserve=yes
      //## end ClCommOpeningSocketTask%38DB523002CF.protected

  private:
    // Additional Private Declarations
      //## begin ClCommOpeningSocketTask%38DB523002CF.private preserve=yes
      //## end ClCommOpeningSocketTask%38DB523002CF.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommOpeningSocketTask%38DB523002CF.implementation preserve=yes
      //## end ClCommOpeningSocketTask%38DB523002CF.implementation

};

//## begin ClCommOpeningSocketTask%38DB523002CF.postscript preserve=yes
//## end ClCommOpeningSocketTask%38DB523002CF.postscript

//## begin ClCommStopReadingTask%3922B41E0131.preface preserve=yes
//## end ClCommStopReadingTask%3922B41E0131.preface

//## Class: ClCommStopReadingTask%3922B41E0131
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommStopReadingTask : public ClCommTask  //## Inherits: <unnamed>%3922B42B00AD
{
  //## begin ClCommStopReadingTask%3922B41E0131.initialDeclarations preserve=yes
  //## end ClCommStopReadingTask%3922B41E0131.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommStopReadingTask%958548782
      ClCommStopReadingTask ();

    //## Destructor (specified)
      //## Operation: ~ClCommStopReadingTask%958548783
      ~ClCommStopReadingTask ();

    // Additional Public Declarations
      //## begin ClCommStopReadingTask%3922B41E0131.public preserve=yes
      //## end ClCommStopReadingTask%3922B41E0131.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommStopReadingTask%3922B41E0131.protected preserve=yes
      //## end ClCommStopReadingTask%3922B41E0131.protected

  private:
    // Additional Private Declarations
      //## begin ClCommStopReadingTask%3922B41E0131.private preserve=yes
      //## end ClCommStopReadingTask%3922B41E0131.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommStopReadingTask%3922B41E0131.implementation preserve=yes
      //## end ClCommStopReadingTask%3922B41E0131.implementation

};

//## begin ClCommStopReadingTask%3922B41E0131.postscript preserve=yes
//## end ClCommStopReadingTask%3922B41E0131.postscript

//## begin ClCommShutDownTask%3923F6850190.preface preserve=yes
//## end ClCommShutDownTask%3923F6850190.preface

//## Class: ClCommShutDownTask%3923F6850190
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommShutDownTask : public ClCommTask  //## Inherits: <unnamed>%3923F73301EB
{
  //## begin ClCommShutDownTask%3923F6850190.initialDeclarations preserve=yes
  //## end ClCommShutDownTask%3923F6850190.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommShutDownTask%958646102
      ClCommShutDownTask ();

    //## Destructor (specified)
      //## Operation: ~ClCommShutDownTask%958646103
      ~ClCommShutDownTask ();

    // Additional Public Declarations
      //## begin ClCommShutDownTask%3923F6850190.public preserve=yes
      //## end ClCommShutDownTask%3923F6850190.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommShutDownTask%3923F6850190.protected preserve=yes
      //## end ClCommShutDownTask%3923F6850190.protected

  private:
    // Additional Private Declarations
      //## begin ClCommShutDownTask%3923F6850190.private preserve=yes
      //## end ClCommShutDownTask%3923F6850190.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommShutDownTask%3923F6850190.implementation preserve=yes
      //## end ClCommShutDownTask%3923F6850190.implementation

};

//## begin ClCommShutDownTask%3923F6850190.postscript preserve=yes
//## end ClCommShutDownTask%3923F6850190.postscript

// Class ClCommTask 


//## Other Operations (inline)
inline ClParamList * ClCommTask::GetParams ()
{
  //## begin ClCommTask::GetParams%957429936.body preserve=yes
	return m_pParamList;
  //## end ClCommTask::GetParams%957429936.body
}

inline m4int_t ClCommTask::GetCommProtocolType ()
{
  //## begin ClCommTask::GetCommProtocolType%965284297.body preserve=yes
	if(NULL==m_pCommProtocol)
	{
		return M4_COMM_NULL_PROTOCOL;
	}
	else
		return m_pCommProtocol->GetCommProtocolType();
  //## end ClCommTask::GetCommProtocolType%965284297.body
}

inline ClCommProtocol * ClCommTask::GetCommProtocol ()
{
  //## begin ClCommTask::GetCommProtocol%965284298.body preserve=yes
	return m_pCommProtocol;
  //## end ClCommTask::GetCommProtocol%965284298.body
}

// Class ClCommReadingTask 

// Class ClCommWrittingTask 

// Class ClCommOpenPortTask 


//## Other Operations (inline)
inline m4int_t ClCommOpenPortTask::GetCommProtocolType ()
{
  //## begin ClCommOpenPortTask::GetCommProtocolType%957345710.body preserve=yes
	return m_iProtocolType;
  //## end ClCommOpenPortTask::GetCommProtocolType%957345710.body
}

// Class ClCommCloseSocketTask 

// Class ClCommClosePortTask 

// Class ClCommOpeningSocketTask 


//## Other Operations (inline)
inline m4int_t ClCommOpeningSocketTask::GetCommProtocolType ()
{
  //## begin ClCommOpeningSocketTask::GetCommProtocolType%957345711.body preserve=yes
	return m_iProtocolType;
  //## end ClCommOpeningSocketTask::GetCommProtocolType%957345711.body
}

inline m4int_t ClCommOpeningSocketTask::GetPort ()
{
  //## begin ClCommOpeningSocketTask::GetPort%966411255.body preserve=yes
	return m_iPort;
  //## end ClCommOpeningSocketTask::GetPort%966411255.body
}

// Class ClCommStopReadingTask 

// Class ClCommShutDownTask 

//## begin module%38DB62A501BC.epilog preserve=yes
//## end module%38DB62A501BC.epilog


#endif

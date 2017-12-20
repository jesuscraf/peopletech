//## begin module%353C93700214.cm preserve=no
//## end module%353C93700214.cm

//## begin module%353C93700214.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            Protocol
//	 File:              baserequest.hpp
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
//	==============================================================================
//## end module%353C93700214.cp

//## Module: BaseRequest%353C93700214; Package specification
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Source file: F:\integration\m4serverbase\inc\baserequest.hpp

#ifndef baserequest_h
#define baserequest_h 1

//## begin module%353C93700214.additionalIncludes preserve=no
//## end module%353C93700214.additionalIncludes

//## begin module%353C93700214.includes preserve=yes
#include <m4serverbase_dll.hpp>
#include <clbaseobject.hpp>
#include <imsgcontainer.hpp>
#include <clbaseaction.hpp>

class ClPDU;



//## begin module%353C93700214.declarations preserve=no
//## end module%353C93700214.declarations

//## begin module%353C93700214.additionalDeclarations preserve=yes
#define		M4_SERVICE_REQ_TYPE		0x00000001
#define		M4_CONTROL_REQ_TYPE		0x00000002
#define		M4_COMMAND_REQ_TYPE		0x00000003
#define		M4_EVENT_REQ_TYPE		0x00000004
#define		M4_TRACE_REQ_TYPE		0x00000005

#define		M4_NO_SESSION_FOR_REQUEST	-99
//## end module%353C93700214.additionalDeclarations


//## begin ClBaseRequest%3497BDCA02A5.preface preserve=yes
//## end ClBaseRequest%3497BDCA02A5.preface

//## Class: ClBaseRequest%3497BDCA02A5
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%36A32E2E0155;ClPDU { -> F}

class M4_DECL_M4SERVERBASE ClBaseRequest : public ClBaseObject, //## Inherits: <unnamed>%355316360152
               	public ClBaseAction  //## Inherits: <unnamed>%38F70D7F00BF
{
  //## begin ClBaseRequest%3497BDCA02A5.initialDeclarations preserve=yes
  //## end ClBaseRequest%3497BDCA02A5.initialDeclarations

  public:
    //## Destructor (specified)
      //## Operation: ~ClBaseRequest%882359170
      virtual ~ClBaseRequest ();


    //## Other Operations (specified)
      //## Operation: GetRequestId%894621707
      virtual m4handle_t GetRequestId () const;

      //## Operation: GetRequestType%894621708
      virtual m4uint32_t GetRequestType () const;

      //## Operation: SetRequestId%894621709; C++
      virtual void SetRequestId (m4handle_t ai_uiReqId);

      //## Operation: GetPriority%894621723
      virtual m4uint32_t GetPriority () const;

      //## Operation: SetPriority%894621724; C++
      virtual void SetPriority (m4uint32_t ai_uiPriority);

      //## Operation: SetUniqueRequestID%986815273
      m4return_t SetUniqueRequestID (m4string_t& ai_strServerHost, m4string_t& ai_strServerBasePort, m4string_t& ai_strServerSessionID, m4string_t& ai_strUSerID);

      //## Operation: GetUniqueRequestID%986815278
      m4return_t GetUniqueRequestID (m4string_t &ao_strUniqueRequestID);

      //## Operation: GetMsgContainer%986914178
      ClMessageContainer * GetMsgContainer ();

    // Additional Public Declarations
      //## begin ClBaseRequest%3497BDCA02A5.public preserve=yes
      //## end ClBaseRequest%3497BDCA02A5.public

  protected:
    //## Constructors (specified)
      //## Operation: ClBaseRequest%882359169; C++
      ClBaseRequest (m4uint32_t ai_uiReqType, m4handle_t ai_uiReqId = 0, ClStatistic *ai_poStatistic = NULL);

    // Data Members for Class Attributes

      //## Attribute: m_uiRequestID%3497BE1C0163
      //## begin ClBaseRequest::m_uiRequestID%3497BE1C0163.attr preserve=no  protected: m4uint32_t {UA} ai_uiReqId
	  m4handle_t m_uiRequestID;
      //## end ClBaseRequest::m_uiRequestID%3497BE1C0163.attr

      //## Attribute: m_uiPriority%3497BE1C0180
      //## begin ClBaseRequest::m_uiPriority%3497BE1C0180.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_uiPriority;
      //## end ClBaseRequest::m_uiPriority%3497BE1C0180.attr

      //## Attribute: m_uiRequestType%3552F019024C
      //## begin ClBaseRequest::m_uiRequestType%3552F019024C.attr preserve=no  protected: m4uint32_t {UA} ai_uiReqType
      m4uint32_t m_uiRequestType;
      //## end ClBaseRequest::m_uiRequestType%3552F019024C.attr

      //## Attribute: m_strUniqueId%3AD1CEBE033F
      //## begin ClBaseRequest::m_strUniqueId%3AD1CEBE033F.attr preserve=no  protected: m4string_t {UA} 
      m4string_t m_strUniqueId;
      //## end ClBaseRequest::m_strUniqueId%3AD1CEBE033F.attr

    // Data Members for Associations

      //## Association: M4ServerBase::<unnamed>%3AD3178E0352
      //## Role: ClBaseRequest::m_poMsgContainer%3AD3178F0119
      //## begin ClBaseRequest::m_poMsgContainer%3AD3178F0119.role preserve=no  protected: ClMessageContainer { -> RHAN}
      ClMessageContainer *m_poMsgContainer;
      //## end ClBaseRequest::m_poMsgContainer%3AD3178F0119.role

    // Additional Protected Declarations
      //## begin ClBaseRequest%3497BDCA02A5.protected preserve=yes
      m4return_t m_rRet;
      //## end ClBaseRequest%3497BDCA02A5.protected
  private:
    // Additional Private Declarations
      //## begin ClBaseRequest%3497BDCA02A5.private preserve=yes
      //## end ClBaseRequest%3497BDCA02A5.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClBaseRequest%3497BDCA02A5.implementation preserve=yes
      //## end ClBaseRequest%3497BDCA02A5.implementation

};

//## begin ClBaseRequest%3497BDCA02A5.postscript preserve=yes
//## end ClBaseRequest%3497BDCA02A5.postscript

//## begin ClServiceRequest%35531DC803A3.preface preserve=yes
//## end ClServiceRequest%35531DC803A3.preface

//## Class: ClServiceRequest%35531DC803A3
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%355321200123;ClRequestPDU { -> F}

class M4_DECL_M4SERVERBASE ClServiceRequest : public ClBaseRequest  //## Inherits: <unnamed>%35531DDB03C3
{
  //## begin ClServiceRequest%35531DC803A3.initialDeclarations preserve=yes
  //## end ClServiceRequest%35531DC803A3.initialDeclarations

  public:
    //## Destructor (specified)
      //## Operation: ~ClServiceRequest%902248390; C++
      ~ClServiceRequest ();


    //## Other Operations (specified)
      //## Operation: SetPDUInput%894621717
      void SetPDUInput (ClPDU *ai_pPDU, m4bool_t ai_bPDUOwner = M4_TRUE);

      //## Operation: SetPDUOutput%894621718
      void SetPDUOutput (ClPDU *ai_pPDU, m4bool_t ai_bPDUOwner = M4_TRUE);

      //## Operation: GetServiceId%894621719
      m4pchar_t GetServiceId ();

      //## Operation: GetInputPDU%894621720
      ClPDU * GetInputPDU (m4bool_t ai_bPDUOwner = M4_TRUE);

      //## Operation: GetOutputPDU%894621721
      ClPDU * GetOutputPDU (m4bool_t ai_bPDUOwner = M4_TRUE);

      //## Operation: SetServiceId%894621722
      void SetServiceId (m4pchar_t ai_szServiceId);

    // Additional Public Declarations
      //## begin ClServiceRequest%35531DC803A3.public preserve=yes
      //## end ClServiceRequest%35531DC803A3.public

  protected:
    //## Constructors (specified)
      //## Operation: ClServiceRequest%894621716; C++
      ClServiceRequest (m4uint32_t ai_uiReqType, m4pchar_t ai_szServiceId, m4handle_t ai_uiReqId = 0, ClStatistic *ai_poStatistic = NULL);

    // Data Members for Class Attributes

      //## Attribute: m_szServiceId%35531EF701A0
      //## begin ClServiceRequest::m_szServiceId%35531EF701A0.attr preserve=no  protected: m4pchar_t {UA} 
      m4pchar_t m_szServiceId;
      //## end ClServiceRequest::m_szServiceId%35531EF701A0.attr

      //## Attribute: m_bOwnerOutputPDU%35531EF7027B
      //## begin ClServiceRequest::m_bOwnerOutputPDU%35531EF7027B.attr preserve=no  protected: m4bool_t {UA} M4_TRUE
      m4bool_t m_bOwnerOutputPDU;
      //## end ClServiceRequest::m_bOwnerOutputPDU%35531EF7027B.attr

      //## Attribute: m_bOwnerInputPDU%35531EF70365
      //## begin ClServiceRequest::m_bOwnerInputPDU%35531EF70365.attr preserve=no  protected: m4bool_t {UA} M4_TRUE
      m4bool_t m_bOwnerInputPDU;
      //## end ClServiceRequest::m_bOwnerInputPDU%35531EF70365.attr

    // Data Members for Associations

      //## Association: M4ServerBase::<unnamed>%379438DB0058
      //## Role: ClServiceRequest::m_pInputPDU%379438DB0347
      //## begin ClServiceRequest::m_pInputPDU%379438DB0347.role preserve=no  protected: ClPDU { -> RFHAN}
      ClPDU *m_pInputPDU;
      //## end ClServiceRequest::m_pInputPDU%379438DB0347.role

      //## Association: M4ServerBase::<unnamed>%37943A670260
      //## Role: ClServiceRequest::m_pOutputPDU%37943A680121
      //## begin ClServiceRequest::m_pOutputPDU%37943A680121.role preserve=no  protected: ClPDU { -> RFHAN}
      ClPDU *m_pOutputPDU;
      //## end ClServiceRequest::m_pOutputPDU%37943A680121.role

    // Additional Protected Declarations
      //## begin ClServiceRequest%35531DC803A3.protected preserve=yes
      //## end ClServiceRequest%35531DC803A3.protected

  private:
    // Additional Private Declarations
      //## begin ClServiceRequest%35531DC803A3.private preserve=yes
      //## end ClServiceRequest%35531DC803A3.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClServiceRequest%35531DC803A3.implementation preserve=yes
      //## end ClServiceRequest%35531DC803A3.implementation

};

//## begin ClServiceRequest%35531DC803A3.postscript preserve=yes
//## end ClServiceRequest%35531DC803A3.postscript

//## begin ClControlRequest%3569407E003D.preface preserve=yes
//## end ClControlRequest%3569407E003D.preface

//## Class: ClControlRequest%3569407E003D
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERVERBASE ClControlRequest : public ClBaseRequest  //## Inherits: <unnamed>%356940F7034B
{
  //## begin ClControlRequest%3569407E003D.initialDeclarations preserve=yes
  //## end ClControlRequest%3569407E003D.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: SetHandle%896086690; C++
      m4return_t SetHandle (m4handle_t ai_hObject);

      //## Operation: GetHandle%896086691
      m4handle_t GetHandle () const;

      //## Operation: Reset%896086689
      void Reset ();

      //## Operation: GetResult%896086692
      m4return_t GetResult () const;

      //## Operation: SetResult%896086693; C++
      void SetResult (m4return_t ai_rResult);

      //## Operation: Recycle%941708201; C++
      //	Returns M4_ERROR if CanRecycle() != M4_TRUE, M4_SUCCESS if Recycle is
      //	completed. Other return values may be defined.
      virtual m4return_t Recycle ();

      //## Operation: CanRecycle%941708202
      //	Returns M4_TRUE if the object can be recycled, M4_FALSE otherwise.
      virtual m4bool_t CanRecycle ();

      //## Operation: Destroy%941708203
      //	Returns M4_ERROR if CanDestroy() != M4_TRUE, M4_SUCCESS if Destroy is
      //	completed. Other return Values may be defined.
      virtual m4return_t Destroy ();

      //## Operation: CanDestroy%941708204
      //	Returns M4_TRUE if the object may be destroyed, M4_FALSE otherwise.
      virtual m4bool_t CanDestroy ();

    // Additional Public Declarations
      //## begin ClControlRequest%3569407E003D.public preserve=yes
      //## end ClControlRequest%3569407E003D.public

  protected:
    //## Constructors (specified)
      //## Operation: ClControlRequest%896086688; C++
      ClControlRequest (m4uint32_t ai_uiReqType, m4handle_t ai_uiReqId = 0, ClStatistic *ai_poStatistic = NULL);

    // Data Members for Class Attributes

      //## Attribute: m_hObject%3569432F02FD
      //## begin ClControlRequest::m_hObject%3569432F02FD.attr preserve=no  protected: m4handle_t {UA} 0
      m4handle_t m_hObject;
      //## end ClControlRequest::m_hObject%3569432F02FD.attr

    // Additional Protected Declarations
      //## begin ClControlRequest%3569407E003D.protected preserve=yes
      //## end ClControlRequest%3569407E003D.protected

  private:
    // Additional Private Declarations
      //## begin ClControlRequest%3569407E003D.private preserve=yes
      //## end ClControlRequest%3569407E003D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClControlRequest%3569407E003D.implementation preserve=yes
      //## end ClControlRequest%3569407E003D.implementation

};

//## begin ClControlRequest%3569407E003D.postscript preserve=yes
//## end ClControlRequest%3569407E003D.postscript

// Class ClBaseRequest 


//## Other Operations (inline)
inline m4handle_t ClBaseRequest::GetRequestId () const
{
  //## begin ClBaseRequest::GetRequestId%894621707.body preserve=yes
	return m_uiRequestID;
  //## end ClBaseRequest::GetRequestId%894621707.body
}

inline m4uint32_t ClBaseRequest::GetRequestType () const
{
  //## begin ClBaseRequest::GetRequestType%894621708.body preserve=yes
	return m_uiRequestType;
  //## end ClBaseRequest::GetRequestType%894621708.body
}

inline void ClBaseRequest::SetRequestId (m4handle_t ai_uiReqId)
{
  //## begin ClBaseRequest::SetRequestId%894621709.body preserve=yes
	m_uiRequestID = ai_uiReqId;
  //## end ClBaseRequest::SetRequestId%894621709.body
}

inline m4uint32_t ClBaseRequest::GetPriority () const
{
  //## begin ClBaseRequest::GetPriority%894621723.body preserve=yes
	return m_uiPriority;
  //## end ClBaseRequest::GetPriority%894621723.body
}

inline void ClBaseRequest::SetPriority (m4uint32_t ai_uiPriority)
{
  //## begin ClBaseRequest::SetPriority%894621724.body preserve=yes
	m_uiPriority = ai_uiPriority;
  //## end ClBaseRequest::SetPriority%894621724.body
}

// Class ClServiceRequest 

// Class ClControlRequest 


//## Other Operations (inline)
inline m4return_t ClControlRequest::SetHandle (m4handle_t ai_hObject)
{
  //## begin ClControlRequest::SetHandle%896086690.body preserve=yes
	m_hObject = ai_hObject;

	return M4_SUCCESS;
  //## end ClControlRequest::SetHandle%896086690.body
}

inline m4handle_t ClControlRequest::GetHandle () const
{
  //## begin ClControlRequest::GetHandle%896086691.body preserve=yes
	return m_hObject;
  //## end ClControlRequest::GetHandle%896086691.body
}

inline m4return_t ClControlRequest::GetResult () const
{
  //## begin ClControlRequest::GetResult%896086692.body preserve=yes
	return m_rRet;
  //## end ClControlRequest::GetResult%896086692.body
}

inline void ClControlRequest::SetResult (m4return_t ai_rResult)
{
  //## begin ClControlRequest::SetResult%896086693.body preserve=yes
	m_rRet = ai_rResult;
  //## end ClControlRequest::SetResult%896086693.body
}

//## begin module%353C93700214.epilog preserve=yes
//## end module%353C93700214.epilog


#endif

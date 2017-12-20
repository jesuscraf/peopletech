//## begin module%35643A18011F.cm preserve=no
//## end module%35643A18011F.cm

//## begin module%35643A18011F.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            Protocol
//	 File:              boomrequest.hpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:				980521
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
//## end module%35643A18011F.cp

//## Module: BoomRequest%35643A18011F; Subprogram specification
//## Subsystem: M4Request::inc%379439BF01B6
//## Source file: F:\integration\m4request\inc\boomrequest.hpp

#ifndef boomrequest_h
#define boomrequest_h 1

//## begin module%35643A18011F.additionalIncludes preserve=no
#include <m4request_dll.hpp>
#include <m4condition.hpp>
#include <baserequest.hpp>
#include <fifoq.hpp>
#include <serviceq.hpp>

//## begin module%35643A18011F.declarations preserve=no
//## end module%35643A18011F.declarations

//## begin module%35643A18011F.additionalDeclarations preserve=yes
//## end module%35643A18011F.additionalDeclarations


//## begin ClBoomerangRequest%356434DF0083.preface preserve=yes
//## end ClBoomerangRequest%356434DF0083.preface

//## Class: ClBoomerangRequest%356434DF0083
//## Category: M4Request%35EFAF8303E2
//## Subsystem: M4Request::inc%379439BF01B6
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%356C39DA00D3;ClServiceQ { -> }

class M4_DECL_M4REQUEST ClBoomerangRequest : public ClBaseRequest  //## Inherits: <unnamed>%3564366502E4
{
  //## begin ClBoomerangRequest%356434DF0083.initialDeclarations preserve=yes
  //## end ClBoomerangRequest%356434DF0083.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClBoomerangRequest%895761854
      ClBoomerangRequest ();

    //## Destructor (specified)
      //## Operation: ~ClBoomerangRequest%895761855
      ~ClBoomerangRequest ();


    //## Other Operations (specified)
      //## Operation: GetRequestId%895761858
      m4handle_t GetRequestId () const;

      //## Operation: GetRequestType%895761859
      m4uint32_t GetRequestType () const;

      //## Operation: SetRequestId%895761860; C++
      void SetRequestId (m4handle_t ai_uiReqId);

      //## Operation: GetPriority%895761861
      m4uint32_t GetPriority () const;

      //## Operation: SetPriority%895761862; C++
      void SetPriority (m4uint32_t ai_uiPriority);

      //## Operation: SetRequest%895761863; C++
      m4return_t SetRequest (ClBaseRequest *ai_pComponent);

      //## Operation: GetRequest%895761864; C++
      m4return_t GetRequest (ClBaseRequest *&ao_pComponent) const;

      //## Operation: Return%895761856
      m4return_t Return ();

      //## Operation: Throw%895761857; C++
      m4return_t Throw (ClServiceQ *ai_pTarget, ClControlQ *ai_pSource = NULL);

      //## Operation: ResetRequest%895761865; C++
      m4return_t ResetRequest ();

      //## Operation: Recycle%941721567; C++
      //	Returns M4_ERROR if CanRecycle() != M4_TRUE, M4_SUCCESS if Recycle is
      //	completed. Other return values may be defined.
      virtual m4return_t Recycle ();

      //## Operation: CanRecycle%941721568
      //	Returns M4_TRUE if the object can be recycled, M4_FALSE otherwise.
      virtual m4bool_t CanRecycle ();

      //## Operation: Destroy%941721569
      //	Returns M4_ERROR if CanDestroy() != M4_TRUE, M4_SUCCESS if Destroy is
      //	completed. Other return Values may be defined.
      virtual m4return_t Destroy ();

      //## Operation: CanDestroy%941721570
      //	Returns M4_TRUE if the object may be destroyed, M4_FALSE otherwise.
      virtual m4bool_t CanDestroy ();

    // Additional Public Declarations
      //## begin ClBoomerangRequest%356434DF0083.public preserve=yes
      //## end ClBoomerangRequest%356434DF0083.public

  protected:
    // Data Members for Associations

      //## Association: M4Request::<unnamed>%356C3BD30344
      //## Role: ClBoomerangRequest::m_pSourceQ%356C3BE201DC
      //## begin ClBoomerangRequest::m_pSourceQ%356C3BE201DC.role preserve=no  protected: ClControlQ {0..n -> 1RHAN}
      ClControlQ *m_pSourceQ;
      //## end ClBoomerangRequest::m_pSourceQ%356C3BE201DC.role

      //## Association: M4Request::<unnamed>%35643D7D0100
      //## Role: ClBoomerangRequest::m_pReturn%35643D7E0025
      //## begin ClBoomerangRequest::m_pReturn%35643D7E0025.role preserve=no  protected: ClCondition {0..n -> 1RHAN}
      ClBooleanCondition *m_pReturn;
      //## end ClBoomerangRequest::m_pReturn%35643D7E0025.role

      //## Association: M4Request::<unnamed>%3564366D01DB
      //## Role: ClBoomerangRequest::m_pRequest%3564366F0015
      //## begin ClBoomerangRequest::m_pRequest%3564366F0015.role preserve=no  protected: ClBaseRequest {0..1 -> 1RHAN}
      ClBaseRequest *m_pRequest;
      //## end ClBoomerangRequest::m_pRequest%3564366F0015.role

    // Additional Protected Declarations
      //## begin ClBoomerangRequest%356434DF0083.protected preserve=yes
      //## end ClBoomerangRequest%356434DF0083.protected

  private:
    // Additional Private Declarations
      //## begin ClBoomerangRequest%356434DF0083.private preserve=yes
      //## end ClBoomerangRequest%356434DF0083.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClBoomerangRequest%356434DF0083.implementation preserve=yes
      //## end ClBoomerangRequest%356434DF0083.implementation

};

//## begin ClBoomerangRequest%356434DF0083.postscript preserve=yes
//## end ClBoomerangRequest%356434DF0083.postscript

// Class ClBoomerangRequest 


//## Other Operations (inline)
inline m4handle_t ClBoomerangRequest::GetRequestId () const
{
  //## begin ClBoomerangRequest::GetRequestId%895761858.body preserve=yes
	if (NULL == m_pRequest)
		return 0xffffffff;

	return m_pRequest->GetRequestId ();
  //## end ClBoomerangRequest::GetRequestId%895761858.body
}

inline m4uint32_t ClBoomerangRequest::GetRequestType () const
{
  //## begin ClBoomerangRequest::GetRequestType%895761859.body preserve=yes
	if (NULL == m_pRequest)
		return 0xffffffff;

	return m_pRequest->GetRequestType ();
  //## end ClBoomerangRequest::GetRequestType%895761859.body
}

inline void ClBoomerangRequest::SetRequestId (m4handle_t ai_uiReqId)
{
  //## begin ClBoomerangRequest::SetRequestId%895761860.body preserve=yes
	if (NULL != m_pRequest)
		m_pRequest->SetRequestId (ai_uiReqId);
  //## end ClBoomerangRequest::SetRequestId%895761860.body
}

inline m4uint32_t ClBoomerangRequest::GetPriority () const
{
  //## begin ClBoomerangRequest::GetPriority%895761861.body preserve=yes
	if (NULL == m_pRequest)
		return 0xffffffff;

	return m_pRequest->GetPriority ();
  //## end ClBoomerangRequest::GetPriority%895761861.body
}

inline void ClBoomerangRequest::SetPriority (m4uint32_t ai_uiPriority)
{
  //## begin ClBoomerangRequest::SetPriority%895761862.body preserve=yes
	if (NULL != m_pRequest)
		m_pRequest->SetPriority (ai_uiPriority);
  //## end ClBoomerangRequest::SetPriority%895761862.body
}

inline m4return_t ClBoomerangRequest::SetRequest (ClBaseRequest *ai_pComponent)
{
  //## begin ClBoomerangRequest::SetRequest%895761863.body preserve=yes
	if (NULL != m_pRequest || NULL == ai_pComponent)
		return M4_ERROR;

	m_pRequest = ai_pComponent;

	return M4_SUCCESS;
  //## end ClBoomerangRequest::SetRequest%895761863.body
}

inline m4return_t ClBoomerangRequest::GetRequest (ClBaseRequest *&ao_pComponent) const
{
  //## begin ClBoomerangRequest::GetRequest%895761864.body preserve=yes
	if (NULL == m_pRequest)
		return M4_ERROR;

	ao_pComponent = m_pRequest;

	return M4_SUCCESS;
  //## end ClBoomerangRequest::GetRequest%895761864.body
}

inline m4return_t ClBoomerangRequest::ResetRequest ()
{
  //## begin ClBoomerangRequest::ResetRequest%895761865.body preserve=yes
	if (NULL == m_pRequest)
		return M4_ERROR;

	m_pRequest = NULL;

	return M4_SUCCESS;
  //## end ClBoomerangRequest::ResetRequest%895761865.body
}

//## begin module%35643A18011F.epilog preserve=yes
//## end module%35643A18011F.epilog


#endif

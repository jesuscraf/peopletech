//## begin module%350915AD0260.cm preserve=no
//## end module%350915AD0260.cm

//## begin module%350915AD0260.cp preserve=no
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
//## end module%350915AD0260.cp

//## Module: Request%350915AD0260; Subprogram specification
//## Subsystem: M4Request::inc%379439BF01B6
//## Source file: D:\Work\v600\m4request\inc\request.hpp

#ifndef request_h
#define request_h 1

//## begin module%350915AD0260.additionalIncludes preserve=no
#include <m4request_dll.hpp>
#include <m4credentials.hpp>
#include <msgcontainers.hpp>
#include <requestcollection.hpp>
#include <m4types.hpp>
#include <staterecord.hpp>
#include <baserequest.hpp>
#include <statistics.hpp>
#include <exvariables.hpp>
#include <logsys.hpp>
#include <m4date.hpp>
#include <criticalsection.hpp>
#include "usinterface.hpp"
#include "m4thread.hpp"
#include "m4logfile.hpp"


class ClConnInstance;
class ClUSSessionInterface;

//## begin module%350915AD0260.declarations preserve=no
//## end module%350915AD0260.declarations

//## begin module%350915AD0260.additionalDeclarations preserve=yes
const m4int16_t	REQ_ACTIVE		= 0;
const m4int16_t	REQ_CANCELING	= 1;
const m4int16_t	REQ_CANCELED	= 2;
const m4int16_t	REQ_TIMED_OUT	= 3;
const m4int16_t	REQ_CLOSED		= 4;

//## end module%350915AD0260.additionalDeclarations


//## Class: ISelfDump%3815B40602CF
//## Category: M4Request%35EFAF8303E2
//## Subsystem: M4Request::inc%379439BF01B6
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ISelfDump 
{
  public:

    //## Other Operations (specified)
      //## Operation: Dump%940939222; C++
      virtual void Dump (m4string_t &ao_sMessage, m4bool_t ai_bOnlyHeader = M4_FALSE) = 0;

  protected:
  private:
  private: //## implementation
};

//## begin ClRequestStatistics%34708E67010D.preface preserve=yes
//## end ClRequestStatistics%34708E67010D.preface

//## Class: ClRequestStatistics%34708E67010D
//## Category: M4Request%35EFAF8303E2
//## Subsystem: M4Request::inc%379439BF01B6
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4REQUEST ClRequestStatistics : public ClStatistic, //## Inherits: <unnamed>%34708E4B0351
    	public ISelfDump  //## Inherits: <unnamed>%3815B42F0116
{
  //## begin ClRequestStatistics%34708E67010D.initialDeclarations preserve=yes
  //## end ClRequestStatistics%34708E67010D.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClRequestStatistics%890732550; C++
      ClRequestStatistics (m4uint64_t ai_uiStartRecordTime = 0);

    //## Destructor (specified)
      //## Operation: ~ClRequestStatistics%890732551
      ~ClRequestStatistics ();


    //## Other Operations (specified)
      //## Operation: Reset%879794950
      m4return_t Reset ();

      //## Operation: Dump%940581805; C++
      virtual void Dump (m4string_t &ao_sMessage, m4bool_t ai_bOnlyHeader = M4_FALSE);

	  virtual void Dump (m4string_t &ao_sMessage, m4pcchar_t ai_pccService, m4bool_t ai_bOnlyHeader = M4_FALSE);

      //## Operation: DumpHeader%959772392; C++
      static void DumpHeader (m4string_t &ao_sHeader);

      //## Operation: GetStat%962640091; C++
      //	Get operation to retrieve a literal value with index ai_hStatId.
      virtual m4return_t GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_poData);

      //## Operation: ResetStat%962640092
      //	This operation resets the value specified by ai_hStatId in the statistics
      //	object. In this class this operation is abstract and must be defined in
      //	derived classes.
      virtual m4return_t ResetStat (ClStatHandle &ai_hStatId);

      //## Operation: StartCPUCount%1068803581
      m4return_t StartCPUCount (void* ai_hThreadHandle);

      //## Operation: StopCPUCount%1068803582
      m4return_t StopCPUCount (m4iden_t ai_iThreadId);

    // Data Members for Class Attributes

      //## Attribute: Reading%350E691003C2
      //## begin ClRequestStatistics::Reading%350E691003C2.attr preserve=no  public: static m4pchar_t {VAC} "Reading"
      static const m4pchar_t  Reading;
      //## end ClRequestStatistics::Reading%350E691003C2.attr

      //## Attribute: Creating%347091390237
      //## begin ClRequestStatistics::Creating%347091390237.attr preserve=no  public: static m4pchar_t {VAC} "Creating"
      static const m4pchar_t  Creating;
      //## end ClRequestStatistics::Creating%347091390237.attr

      //## Attribute: GettingSession%350E691003D6
      //## begin ClRequestStatistics::GettingSession%350E691003D6.attr preserve=no  public: static m4pchar_t {VAC} "GettingSession"
      static const m4pchar_t  GettingSession;
      //## end ClRequestStatistics::GettingSession%350E691003D6.attr

      //## Attribute: Registering%347091B90187
      //## begin ClRequestStatistics::Registering%347091B90187.attr preserve=no  public: static m4pchar_t {VAC} "Registering"
      static const m4pchar_t  Registering;
      //## end ClRequestStatistics::Registering%347091B90187.attr

      //## Attribute: GettingService%3A0929AB0314
      //## begin ClRequestStatistics::GettingService%3A0929AB0314.attr preserve=no  public: static m4pchar_t {UAC} "GettingService"
      static const m4pchar_t  GettingService;
      //## end ClRequestStatistics::GettingService%3A0929AB0314.attr

      //## Attribute: InServiceQ%347093910077
      //## begin ClRequestStatistics::InServiceQ%347093910077.attr preserve=no  public: static m4pchar_t {VAC} "InServiceQ"
      static const m4pchar_t  InServiceQ;
      //## end ClRequestStatistics::InServiceQ%347093910077.attr

      //## Attribute: InService%350E691003CC
      //## begin ClRequestStatistics::InService%350E691003CC.attr preserve=no  public: static m4pchar_t {VAC} "InService"
      static const m4pchar_t  InService;
      //## end ClRequestStatistics::InService%350E691003CC.attr

      //## Attribute: Writing%347091E4002A
      //## begin ClRequestStatistics::Writing%347091E4002A.attr preserve=no  public: static m4pchar_t {VAC} "Writing"
      static const m4pchar_t  Writing;
      //## end ClRequestStatistics::Writing%347091E4002A.attr

      //## Attribute: Deregistering%347096FB01F1
      //## begin ClRequestStatistics::Deregistering%347096FB01F1.attr preserve=no  public: static m4pchar_t {VAC} "Deregistering"
      static const m4pchar_t  Deregistering;
      //## end ClRequestStatistics::Deregistering%347096FB01F1.attr

      //## Attribute: InCommQueue%3AB61B620250
      //## begin ClRequestStatistics::InCommQueue%3AB61B620250.attr preserve=no  public: static m4pchar_t {UAC} "InCommQueue"
      static const m4pchar_t  InCommSubsystem;
      //## end ClRequestStatistics::InCommQueue%3AB61B620250.attr

      //## Attribute: InSSQueue%3AB61B62039B
      //## begin ClRequestStatistics::InSSQueue%3AB61B62039B.attr preserve=no  public: static m4pchar_t {UAC} "InSSQueue"
      static const m4pchar_t  InSSQueueIn;
      //## end ClRequestStatistics::InSSQueue%3AB61B62039B.attr

    // Additional Public Declarations
      //## begin ClRequestStatistics%34708E67010D.public preserve=yes

      static const m4pchar_t InSSQueueOut;
      virtual void Sett(const m4pchar_t ai_szState , m4uint64_t ai_iStartTime = 0);
      virtual void SettFinish ();

      virtual ClStatTimeType Gett(const m4pchar_t ai_szState );
      virtual m4uint64_t GetStartTime ();
      virtual ClStatTimeType GetInProcess ();

      virtual void SetInputSize( m4uint32_t ai_iInputSize );
      virtual void SetOutputSize( m4uint32_t ai_iOutputSize );

      virtual void SetUserInfo( m4uint32_t ai_iUserInfo );
	  virtual m4uint32_t GetUserInfo();

	  virtual void SetRowsQuotaCurrent( m4uint32_t ai_iRows );
	  virtual void SetRowsQuotaPeak( m4uint32_t ai_iRows );
	  virtual void SetRowsQuotaLimit( m4uint32_t ai_iMaxRows );

	  virtual void SetSessionKey( m4pchar_t ai_pcSessionKey );
	  
	  virtual m4pchar_t GetSessionKey();

      //## end ClRequestStatistics%34708E67010D.public
  protected:
    // Data Members for Associations

      //## Association: M4Request::<unnamed>%380EE97B03CD
      //## Role: ClRequestStatistics::m_oStateRecord%380EE97C01C6
      //## begin ClRequestStatistics::m_oStateRecord%380EE97C01C6.role preserve=no  protected: ClStateRecord { -> 1VHAN}
      ClStateRecord m_oStateRecord;
      //## end ClRequestStatistics::m_oStateRecord%380EE97C01C6.role

      //## Association: M4Request::<unnamed>%3815B2830160
      //## Role: ClRequestStatistics::m_iPDUType%3815B28302FB
      //## begin ClRequestStatistics::m_iPDUType%3815B28302FB.role preserve=no  protected: m4uint16_t { -> 1VHAN}
      m4uint16_t m_iPDUType;
      //## end ClRequestStatistics::m_iPDUType%3815B28302FB.role

    // Additional Protected Declarations
      //## begin ClRequestStatistics%34708E67010D.protected preserve=yes
      //## end ClRequestStatistics%34708E67010D.protected

  private:

    //## Other Operations (specified)
      //## Operation: _ElapsedTime%1068803580
      ClStatTimeType _ElapsedTime (const m4pchar_t ai_pszWhere);

    // Data Members for Class Attributes

      //## Attribute: m_iStartKernelCPUCountTime%3FB4B3E90087
      //## begin ClRequestStatistics::m_iStartKernelCPUCountTime%3FB4B3E90087.attr preserve=no  private: m4uint64_t {UA} 
      m4uint64_t m_iStartKernelCPUCountTime;
      //## end ClRequestStatistics::m_iStartKernelCPUCountTime%3FB4B3E90087.attr

      //## Attribute: m_iStartUserCPUCountTime%3FB4B3F903D4
      //## begin ClRequestStatistics::m_iStartUserCPUCountTime%3FB4B3F903D4.attr preserve=no  private: m4uint64_t {UA} 
      m4uint64_t m_iStartUserCPUCountTime;
      //## end ClRequestStatistics::m_iStartUserCPUCountTime%3FB4B3F903D4.attr

      //## Attribute: m_iKernelCPUTime%3FB4B4090033
      //## begin ClRequestStatistics::m_iKernelCPUTime%3FB4B4090033.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iKernelCPUTime;
      //## end ClRequestStatistics::m_iKernelCPUTime%3FB4B4090033.attr

      //## Attribute: m_iUserCPUTime%3FB4B415038E
      //## begin ClRequestStatistics::m_iUserCPUTime%3FB4B415038E.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iUserCPUTime;
      //## end ClRequestStatistics::m_iUserCPUTime%3FB4B415038E.attr

      //## Attribute: m_bCPUCounting%3FB4B4790323
      //## begin ClRequestStatistics::m_bCPUCounting%3FB4B4790323.attr preserve=no  private: m4bool_t {UA} 
      m4bool_t m_bCPUCounting;
      //## end ClRequestStatistics::m_bCPUCounting%3FB4B4790323.attr

      //## Attribute: m_iThreadId%3FB4B4860278
      //## begin ClRequestStatistics::m_iThreadId%3FB4B4860278.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iThreadId;
      //## end ClRequestStatistics::m_iThreadId%3FB4B4860278.attr

	  m4uint32_t m_iInputSize;
	  m4uint32_t m_iOutputSize;

	  m4uint32_t m_iUserInfo;

	  m4uint32_t m_iRowsQuotaCurrent;
	  m4uint32_t m_iRowsQuotaPeak;
	  m4uint32_t m_iRowsQuotaLimit;

	  // eh 259264
	  time_t	 m_lRequestStartTime;

	  m4pchar_t	m_pcSessionKey;

    // Additional Private Declarations
      //## begin ClRequestStatistics%34708E67010D.private preserve=yes
      //## end ClRequestStatistics%34708E67010D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClRequestStatistics%34708E67010D.implementation preserve=yes
      //## end ClRequestStatistics%34708E67010D.implementation

};

//## begin ClRequestStatistics%34708E67010D.postscript preserve=yes
//## end ClRequestStatistics%34708E67010D.postscript

//## begin ClRequest%342522EA0065.preface preserve=yes
//## end ClRequest%342522EA0065.preface

//## Class: ClRequest%342522EA0065
//	Todo lo que recibimos por el Reciever son peticiones o ordenes que provienen
//	del puerto de control.  Se pueden dividir en dos tipos. Las peticiones de
//	control  o las peticiones de los servicios.
//
//	Próximas Actualizaciones:
//
//	La clase ClRequest tienes ahora dos referencias a la ClPDU. ClPDU se
//	dividira en ClPDUInput y ClPDUOutput y la clase ClRequest tendrá una única
//	referencia a cada una de ellas. De esta forma La PDUOutput tendrá una
//	funcionalidad añadida a la de Input, y es que podrá tomar una serie de
//	atributos de la PDUInput. Para ello la ClPDUOutput incluira una referencia a
//	la instancia de ClRequest que es su propietaria.
//	Para esto, ClRequest deberá contener las funciones de creación de ambas PDUs
//	y no se deberá crear la PDU, directamente, por medio de ClProtocol. Esto
//	facilitará la asignación del puntero de PDUInput apuntando a la Request que
//	la ha creado.
//## Category: M4Request%35EFAF8303E2
//## Subsystem: M4Request::inc%379439BF01B6
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3552EF37025B;m4uint16_t { -> }
//## Uses: <unnamed>%3561BD1B0222;ClRequestStatistics { -> }
//## Uses: <unnamed>%356292E30146;M4ClCrono { -> }
//## Uses: <unnamed>%36A3342E024E;ClPDU { -> F}
//## Uses: <unnamed>%36A334FC03D1;ClRequestPDU { -> F}
//## Uses: <unnamed>%37944AB40367;ClLogSystem { -> }
//## Uses: <unnamed>%39491D15038D;ClBlockSync { -> F}
//## Uses: <unnamed>%3AD329610136;ClSingleMsgContainer { -> }

class M4_DECL_M4REQUEST ClRequest : public ClServiceRequest  //## Inherits: <unnamed>%35531DDD0384
{
  //## begin ClRequest%342522EA0065.initialDeclarations preserve=yes
  //## end ClRequest%342522EA0065.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GetStats%890222207; C++
      //	Retrieves a pointer to a "ClStatistics" derived object to hold statistics.
      ClStatistic * GetStats ();

      //## Operation: GetpSession%893943580
      ClUSSessionInterface * GetpSession () const;

      //## Operation: GetServiceId%894621703
      m4pcchar_t GetServiceId ();

      //## Operation: GetpConnection%894708496
      ClConnInstance * GetpConnection () const;

      //## Operation: GetLog%896346104
      ClLogSystem * GetLog ();

      //## Operation: Cancel%898178462
      m4return_t Cancel ();

      //## Operation: Recycle%941721563; C++
      //	Returns M4_ERROR if CanRecycle() != M4_TRUE, M4_SUCCESS if Recycle is
      //	completed. Other return values may be defined.
      virtual m4return_t Recycle ();

      //## Operation: CanRecycle%941721564
      //	Returns M4_TRUE if the object can be recycled, M4_FALSE otherwise.
      virtual m4bool_t CanRecycle ();

      //## Operation: Destroy%941721565
      //	Returns M4_ERROR if CanDestroy() != M4_TRUE, M4_SUCCESS if Destroy is
      //	completed. Other return Values may be defined.
      virtual m4return_t Destroy ();

      //## Operation: CanDestroy%941721566
      //	Returns M4_TRUE if the object may be destroyed, M4_FALSE otherwise.
      virtual m4bool_t CanDestroy ();

      //## Operation: Initialize%962640087
      m4return_t Initialize (m4string_t &ai_strUserName);

      //## Operation: GetSessionId%972630372
      m4objid_t GetSessionId ();

      //## Operation: SetConnection%973522557
      m4return_t SetConnection (ClConnInstance *ai_poConnection);

      //## Operation: SetSession%973522558
      m4return_t SetSession (ClUSSessionInterface *ai_poSession);

      //## Operation: SetError%975430311
      m4return_t SetError (m4return_t ai_iError);

      //## Operation: GenerateId%978434330
      m4return_t GenerateId (m4uint32_t ai_iClientReqId, m4objid_t &ao_lServerReqId);

      //## Operation: DuplicateWrapper%990443888
      ClRequest * DuplicateWrapper ();

      //## Operation: GetCredential%1068803570
      M4Credentials * GetCredential ();

    // Data Members for Associations

      //## Association: M4Request::<unnamed>%37944C990011
      //## Role: ClRequest::m_oStatus%37944C9901DE
      //## begin ClRequest::m_oStatus%37944C9901DE.role preserve=no  public: ClStatus { -> 1VHAN}
      ClStatus m_oStatus;
      //## end ClRequest::m_oStatus%37944C9901DE.role

      //## Association: M4Request::<unnamed>%3950CD740074
      //## Role: ClRequest::s_oGarbageCollector%3950CD740255
      //## begin ClRequest::s_oGarbageCollector%3950CD740255.role preserve=no  public: static ClRequestCollector { -> 1VHAN}
      static ClRequestCollector s_oGarbageCollector;
      //## end ClRequest::s_oGarbageCollector%3950CD740255.role

    // Additional Public Declarations
      //## begin ClRequest%342522EA0065.public preserve=yes
      //## end ClRequest%342522EA0065.public

  protected:
    //## Constructors (specified)
      //## Operation: ClRequest%875278536; C++
      //	El constructor que debe llevar ya la conexión y un puntero al Chunk de
      //	memoria van a tratar.
      ClRequest (m4uint32_t ai_uiReqType, m4pchar_t ai_szServiceId, ClRequestStatistics *ai_poRequestStats, m4handle_t ai_uiReqId = 0, const m4string_t &ai_strDumpFile = "");

    //## Destructor (specified)
      //## Operation: ~ClRequest%878377069
      ~ClRequest ();


    //## Other Operations (specified)
      //## Operation: _GetReqType%941108816; C++
      m4uint_t _GetReqType (m4uint_t ai_uPDUType);

      //## Operation: _Dump%940939221; C++
      virtual void _Dump ();

    // Data Members for Class Attributes

      //## Attribute: m_poCredential%3FB4AB520284
      //## begin ClRequest::m_poCredential%3FB4AB520284.attr preserve=no  protected: M4Credentials * {UA} 
      M4Credentials *m_poCredential;
      //## end ClRequest::m_poCredential%3FB4AB520284.attr

    // Data Members for Associations

      //## Association: M4Request::<unnamed>%36B0370203B3
      //## Role: ClRequest::m_poConnection%36B03703022E
      //## begin ClRequest::m_poConnection%36B03703022E.role preserve=no  protected: ClConnInstance { -> RFHAN}
      ClConnInstance *m_poConnection;
      //## end ClRequest::m_poConnection%36B03703022E.role

      //## Association: M4Request::<unnamed>%35449C5C030B
      //## Role: ClRequest::m_pSession%35449C5D0366
      //## begin ClRequest::m_pSession%35449C5D0366.role preserve=no  protected: ClUSSessionInterface { -> RFHAN}
      ClUSSessionInterface *m_pSession;
      //## end ClRequest::m_pSession%35449C5D0366.role

      //## Association: M4Request::<unnamed>%3815B791034C
      //## Role: ClRequest::s_pszPDUType%3815B79103A6
      //## begin ClRequest::s_pszPDUType%3815B79103A6.role preserve=no  protected: static m4pcchar_t { -> 6VHAN}
      static m4pcchar_t s_pszPDUType[8];
      //## end ClRequest::s_pszPDUType%3815B79103A6.role

      //## Association: M4Request::<unnamed>%39491CD302DE
      //## Role: ClRequest::s_oDumpBlock%39491CD400B8
      //## begin ClRequest::s_oDumpBlock%39491CD400B8.role preserve=no  protected: static ClCriticalSection { -> 1VHAN}
      static ClCriticalSection s_oDumpBlock;
      //## end ClRequest::s_oDumpBlock%39491CD400B8.role

      //## Association: M4Request::<unnamed>%3965D844039E
      //## Role: ClRequest::m_oLog%3965D84501A0
      //## begin ClRequest::m_oLog%3965D84501A0.role preserve=no  protected: ClLogSystem { -> 1VHAN}
      ClLogSystem m_oLog;
      //## end ClRequest::m_oLog%3965D84501A0.role

    // Additional Protected Declarations
      //## begin ClRequest%342522EA0065.protected preserve=yes
      //## end ClRequest%342522EA0065.protected
  private:
    // Data Members for Class Attributes

      //## Attribute: m_iSessionId%39F928CB018C
      //## begin ClRequest::m_iSessionId%39F928CB018C.attr preserve=no  private: m4objid_t {UA} 0
      m4objid_t m_iSessionId;
      //## end ClRequest::m_iSessionId%39F928CB018C.attr

    // Data Members for Associations

    // Additional Private Declarations
      //## begin ClRequest%342522EA0065.private preserve=yes
      //## end ClRequest%342522EA0065.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClRequest%342522EA0065.implementation preserve=yes
      m4pchar_t m_pcServiceId;
      //## end ClRequest%342522EA0065.implementation

  //## begin ClRequest%342522EA0065.friends preserve=no
    friend class ClRequestCollector;
  //## end ClRequest%342522EA0065.friends
};

//## begin ClRequest%342522EA0065.postscript preserve=yes
//## end ClRequest%342522EA0065.postscript

// Class ISelfDump 

// Class ClRequestStatistics 

// Class ClRequest 


//## Other Operations (inline)
inline ClUSSessionInterface * ClRequest::GetpSession () const
{
  //## begin ClRequest::GetpSession%893943580.body preserve=yes
	return m_pSession;
  //## end ClRequest::GetpSession%893943580.body
}

inline ClConnInstance * ClRequest::GetpConnection () const
{
  //## begin ClRequest::GetpConnection%894708496.body preserve=yes
	return m_poConnection;
  //## end ClRequest::GetpConnection%894708496.body
}

inline m4objid_t ClRequest::GetSessionId ()
{
  //## begin ClRequest::GetSessionId%972630372.body preserve=yes
	return m_iSessionId ;
  //## end ClRequest::GetSessionId%972630372.body
}

inline m4return_t ClRequest::SetConnection (ClConnInstance *ai_poConnection)
{
  //## begin ClRequest::SetConnection%973522557.body preserve=yes
//	if ( m_poConnection )
//		return M4_ERROR ;

	m_poConnection = ai_poConnection;

	return M4_SUCCESS ;
  //## end ClRequest::SetConnection%973522557.body
}

inline m4return_t ClRequest::SetSession (ClUSSessionInterface *ai_poSession)
{
  //## begin ClRequest::SetSession%973522558.body preserve=yes
	if ( m_pSession )
		return M4_ERROR ;

	m_pSession = ai_poSession;

	/* Bug 0175560
	Se asigna el status de la sesión al de la request
	*/
	if( m_pSession != NULL )
	{
		m_oStatus.SetNextStatus( &m_pSession->m_oStatus );
	}

	return M4_SUCCESS ;
  //## end ClRequest::SetSession%973522558.body
}

// Class ISelfDump 

//## begin module%350915AD0260.epilog preserve=yes
inline m4uint64_t ClRequestStatistics::GetStartTime()
{
  return m_oStateRecord.GetStartTime();
}

inline ClStatTimeType ClRequestStatistics::Gett (const m4pchar_t ai_szState)
{
  return _ElapsedTime(ai_szState);
}

inline ClStatTimeType ClRequestStatistics::GetInProcess ()
{
	m4uint_t iTime ;
	ClStatTimeType iTotalTime = 0;
	M4ClString oString = Deregistering ;
	if ( M4_SUCCESS == (ClStatTimeType) m_oStateRecord.GetTimePassToState ( oString  , iTime  ) )
	{
		iTotalTime = iTime ;
	}

  return iTotalTime ;
}

inline void ClRequestStatistics::Sett(const m4pchar_t ai_szState , m4uint64_t ai_iStartTime )
{
  m_oStateRecord.ChangeState(ai_szState,ai_iStartTime );
}

inline void ClRequestStatistics::SettFinish()
{
  m_oStateRecord.ResetState();
}

inline void ClRequestStatistics::SetInputSize( m4uint32_t ai_iInputSize )
{
	m_iInputSize = ai_iInputSize ;
}

inline void ClRequestStatistics::SetOutputSize( m4uint32_t ai_iOutputSize )
{
	m_iOutputSize = ai_iOutputSize ;
}

inline void ClRequestStatistics::SetUserInfo( m4uint32_t ai_iUserInfo )
{
	m_iUserInfo = ai_iUserInfo ;
}

inline m4uint32_t ClRequestStatistics::GetUserInfo()
{
	return m_iUserInfo;
}

inline void ClRequestStatistics::SetRowsQuotaCurrent( m4uint32_t ai_iRows )
{
	m_iRowsQuotaCurrent = ai_iRows ;
}

inline void ClRequestStatistics::SetRowsQuotaPeak( m4uint32_t ai_iRowsPeak )
{
	m_iRowsQuotaPeak = ai_iRowsPeak;
}

inline void ClRequestStatistics::SetRowsQuotaLimit( m4uint32_t ai_iMaxRows )
{
	m_iRowsQuotaLimit = ai_iMaxRows ;
}

inline void ClRequestStatistics::SetSessionKey( m4pchar_t ai_pcSessionKey )
{
	if (m_pcSessionKey != NULL)
	{
		delete m_pcSessionKey;
	}

	m_pcSessionKey = strdup(ai_pcSessionKey);
}

inline m4pchar_t ClRequestStatistics::GetSessionKey()
{
	return m_pcSessionKey;
}

//## end module%350915AD0260.epilog


#endif

//## begin module%34C860E00242.cm preserve=no
//## end module%34C860E00242.cm

//## begin module%34C860E00242.cp preserve=no
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
//## end module%34C860E00242.cp

//## Module: USSession%34C860E00242; Package specification
//## Subsystem: M4UserInterface::inc%3795DCBF0108
//## Source file: F:\integration\m4userinterface\inc\ussession.hpp

#ifndef ussession_h
#define ussession_h 1

//## begin module%34C860E00242.additionalIncludes preserve=no
//## end module%34C860E00242.additionalIncludes

//## begin module%34C860E00242.includes preserve=yes
#include <m4userinterface_dll.hpp>
#include <statistics.hpp>
#include <usinterface.hpp>




//## begin module%34C860E00242.declarations preserve=no
//## end module%34C860E00242.declarations

//## begin module%34C860E00242.additionalDeclarations preserve=yes
#define DEF_SESSION_SIZE	20


#define SESSION_TIMED_OUT	10
#define ACTIVE_SESSION		11

typedef m4handle_t ClEngineContext;
//## end module%34C860E00242.additionalDeclarations


//## begin ClUSSessionStats%34E9A8C60175.preface preserve=yes
//## end ClUSSessionStats%34E9A8C60175.preface

//## Class: ClUSSessionStats%34E9A8C60175
//## Category: M4UserInterface%35EFA1CC011D
//## Subsystem: M4UserInterface::inc%3795DCBF0108
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4USERINTERFACE ClUSSessionStats : public ClStatistic  //## Inherits: <unnamed>%34E9A9D30014
{
  //## begin ClUSSessionStats%34E9A8C60175.initialDeclarations preserve=yes
  //## end ClUSSessionStats%34E9A8C60175.initialDeclarations

  public:
    //## begin ClUSSessionStats::eSessionStats%350D952B014F.preface preserve=yes
    //## end ClUSSessionStats::eSessionStats%350D952B014F.preface

    //## Class: eSessionStats%350D952B014F
    //## Category: M4UserInterface%35EFA1CC011D
    //## Subsystem: M4UserInterface::inc%3795DCBF0108
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { LOGON_TIME = ClStatistic::TOTAL_NUM , NUM_PROCESED_REQ , PENDING_REQ , NUM_SUBROGATED_SESSIONS , TOTAL_NUM } eSessionStats;

    //## begin ClUSSessionStats::eSessionStats%350D952B014F.postscript preserve=yes
    //## end ClUSSessionStats::eSessionStats%350D952B014F.postscript

    //## Constructors (specified)
      //## Operation: ClUSSessionStats%890086955
      ClUSSessionStats ();

    //## Destructor (specified)
      //## Operation: ~ClUSSessionStats%890086956
      ~ClUSSessionStats ();


    //## Other Operations (specified)
      //## Operation: Reset%887707931; C++
      //	This operation resets every value in the statistics object.
      m4return_t Reset ();

      //## Operation: GetStat%887707932; C++
      //	Get operation to retrieve a literal value with index ai_hStatId.
      m4return_t GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_poData);

      //## Operation: ResetStat%887707933
      //	This operation resets the value specified by ai_hStatId in the statistics
      //	object. In this class this operation is abstract and must be defined in
      //	derived classes.
      m4return_t ResetStat (ClStatHandle &ai_hStatId);

    //## Get and Set Operations for Associations (generated)

      //## Association: M4UserInterface::<unnamed>%36F8AB7003A0
      //## Role: ClUSSessionStats::tLastTransaction%36F8AB7101CB
      ClStatTimeType& GettLastTransaction ();
      void SettLastTransaction (const ClStatTimeType& value);

      //## Association: M4UserInterface::<unnamed>%373C05220303
      //## Role: ClUSSessionStats::nPendingReqs%373C05220349
      ClStatNumericalType& GetnPendingReqs ();
      void AddnPendingReqs (const ClStatNumericalType& value);

      //## Association: M4UserInterface::<unnamed>%373C052B01D9
      //## Role: ClUSSessionStats::nProcessedReqs%373C052B0216
      ClStatNumericalType& GetnProcessedReqs ();
      void AddnProcessedReqs (const ClStatNumericalType& value);

      //## Association: M4UserInterface::<unnamed>%373C052E0288
      //## Role: ClUSSessionStats::nSubrogatedSessions%373C052E02B0
      ClStatNumericalType& GetnSubrogatedSessions ();
      void AddnSubrogatedSessions (const ClStatNumericalType& value);

      //## Association: M4UserInterface::<unnamed>%397F03370188
      //## Role: ClUSSessionStats::tLogonTime%397F033801E3
      ClStatTimeType& GettLogonTime ();
      void AddtLogonTime (const ClStatTimeType& value);

  public:
    // Additional Public Declarations
      //## begin ClUSSessionStats%34E9A8C60175.public preserve=yes
      //## end ClUSSessionStats%34E9A8C60175.public

  protected:
    // Data Members for Associations

      //## Association: M4UserInterface::<unnamed>%36F8AB7003A0
      //## begin ClUSSessionStats::tLastTransaction%36F8AB7101CB.role preserve=no  public: ClStatTimeType { -> 1VHN}
      ClStatTimeType m_tLastTransaction;
      //## end ClUSSessionStats::tLastTransaction%36F8AB7101CB.role

      //## Association: M4UserInterface::<unnamed>%373C05220303
      //## begin ClUSSessionStats::nPendingReqs%373C05220349.role preserve=no  public: ClStatNumericalType {1 -> 1VHN}
      ClStatNumericalType m_nPendingReqs;
      //## end ClUSSessionStats::nPendingReqs%373C05220349.role

      //## Association: M4UserInterface::<unnamed>%373C052B01D9
      //## begin ClUSSessionStats::nProcessedReqs%373C052B0216.role preserve=no  public: ClStatNumericalType {1 -> 1VHN}
      ClStatNumericalType m_nProcessedReqs;
      //## end ClUSSessionStats::nProcessedReqs%373C052B0216.role

      //## Association: M4UserInterface::<unnamed>%373C052E0288
      //## begin ClUSSessionStats::nSubrogatedSessions%373C052E02B0.role preserve=no  public: ClStatNumericalType {1 -> 1VHN}
      ClStatNumericalType m_nSubrogatedSessions;
      //## end ClUSSessionStats::nSubrogatedSessions%373C052E02B0.role

      //## Association: M4UserInterface::<unnamed>%397F03370188
      //## begin ClUSSessionStats::tLogonTime%397F033801E3.role preserve=no  public: ClStatTimeType { -> 1VHN}
      ClStatTimeType m_tLogonTime;
      //## end ClUSSessionStats::tLogonTime%397F033801E3.role

    // Additional Protected Declarations
      //## begin ClUSSessionStats%34E9A8C60175.protected preserve=yes
      //## end ClUSSessionStats%34E9A8C60175.protected

  private:
    // Additional Private Declarations
      //## begin ClUSSessionStats%34E9A8C60175.private preserve=yes
      //## end ClUSSessionStats%34E9A8C60175.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClUSSessionStats%34E9A8C60175.implementation preserve=yes
      //## end ClUSSessionStats%34E9A8C60175.implementation

};

//## begin ClUSSessionStats%34E9A8C60175.postscript preserve=yes
//## end ClUSSessionStats%34E9A8C60175.postscript

//## Class: ClUSSessionVisitor%34C873330071
//	The session visitor will visit the session object to obtain or calculate the
//	session statistics
//## Category: M4UserInterface%35EFA1CC011D
//## Subsystem: M4UserInterface::inc%3795DCBF0108
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%353F577801AF;ClUSSessionStats { -> }
//## Uses: <unnamed>%373B1ACD0026;ClUSSession { -> F}

class M4_DECL_M4USERINTERFACE ClUSSessionVisitor : public ClVisitor  //## Inherits: <unnamed>%34C8734C027F
{
  public:
    //## Constructors (specified)
      //## Operation: ClUSSessionVisitor%893349365
      ClUSSessionVisitor ();


    //## Other Operations (specified)
      //## Operation: VisitSession%885550067
      //	This method is called in the accept method of the session object or in
      //	accept method of the Session subsystem object too.
      void VisitSession (ClUSSessionInterface &ai_oSession);

  protected:
  private:
  private: //## implementation
};

//## begin ClUSSession%353DE74101AD.preface preserve=yes
//## end ClUSSession%353DE74101AD.preface

//## Class: ClUSSession%353DE74101AD
//## Category: M4UserInterface%35EFA1CC011D
//## Subsystem: M4UserInterface::inc%3795DCBF0108
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%373B1A910016;ClUSSessionStats { -> }
//## Uses: <unnamed>%373B1ABA02FA;ClUSSessionVisitor { -> }

class M4_DECL_M4USERINTERFACE ClUSSession : public ClUSSessionInterface  //## Inherits: <unnamed>%353DE75E0259
{
  //## begin ClUSSession%353DE74101AD.initialDeclarations preserve=yes
  //## end ClUSSession%353DE74101AD.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClUSSession%893349373; C++
      ClUSSession (const m4uint32_t ai_uiId, const m4string_t &ai_strSessID, ClConnBase *ai_poConnection, ClUSUserInterface *ai_poUser, ClStatistic *ai_poStats, ClUSSessionTicketInterface *ai_poUSTicket, ClVMBaseEnv* ai_poEnvironment, const m4uint32_t ai_uiCompType, const m4uint32_t ai_uiCompLevel, m4bool_t ai_bAuditSession);


    //## Other Operations (specified)
      //## Operation: AddRequest%926668808; C++
      void AddRequest ();

      //## Operation: RemoveRequest%926668809; C++
      void RemoveRequest ();

      //## Operation: Recycle%941721555; C++
      //	Returns M4_ERROR if CanRecycle() != M4_TRUE, M4_SUCCESS if Recycle is
      //	completed. Other return values may be defined.
      virtual m4return_t Recycle ();

      //## Operation: CanRecycle%941721556
      //	Returns M4_TRUE if the object can be recycled, M4_FALSE otherwise.
      virtual m4bool_t CanRecycle ();

      //## Operation: Destroy%941721557
      //	Returns M4_ERROR if CanDestroy() != M4_TRUE, M4_SUCCESS if Destroy is
      //	completed. Other return Values may be defined.
      virtual m4return_t Destroy ();

      //## Operation: CanDestroy%941721558
      //	Returns M4_TRUE if the object may be destroyed, M4_FALSE otherwise.
      virtual m4bool_t CanDestroy ();

      //## Operation: CheckTimeout%985177783
      m4return_t CheckTimeout ();

      //## Operation: SetTimeout%985177784
      static m4return_t SetTimeout (m4double_t ai_lTimeout);
      static m4double_t GetTimeout ();

    // Additional Public Declarations
      //## begin ClUSSession%353DE74101AD.public preserve=yes
      //## end ClUSSession%353DE74101AD.public

  protected:
    // Additional Protected Declarations
      //## begin ClUSSession%353DE74101AD.protected preserve=yes
      //## end ClUSSession%353DE74101AD.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_lTimeout%3AB8E1760320
      //## begin ClUSSession::m_lTimeout%3AB8E1760320.attr preserve=no  private: static m4double_t {VA} 0
      static m4double_t m_lTimeout;
      //## end ClUSSession::m_lTimeout%3AB8E1760320.attr

    // Additional Private Declarations
      //## begin ClUSSession%353DE74101AD.private preserve=yes
      //## end ClUSSession%353DE74101AD.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClUSSession%353DE74101AD.implementation preserve=yes
      //## end ClUSSession%353DE74101AD.implementation

};

//## begin ClUSSession%353DE74101AD.postscript preserve=yes
//## end ClUSSession%353DE74101AD.postscript

//## begin ClUSSessionTicket%373B18A90399.preface preserve=yes
//## end ClUSSessionTicket%373B18A90399.preface

//## Class: ClUSSessionTicket%373B18A90399
//## Category: M4UserInterface%35EFA1CC011D
//## Subsystem: M4UserInterface::inc%3795DCBF0108
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%374C2E5D011C;ClChannel { -> F}

class M4_DECL_M4USERINTERFACE ClUSSessionTicket : public ClUSSessionTicketInterface  //## Inherits: <unnamed>%373B18E10123
{
  //## begin ClUSSessionTicket%373B18A90399.initialDeclarations preserve=yes
  //## end ClUSSessionTicket%373B18A90399.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClUSSessionTicket%926583146; C++
      ClUSSessionTicket (m4char_t *ai_poTicketCode);

    //## Destructor (specified)
      //## Operation: ~ClUSSessionTicket%926583148
      ~ClUSSessionTicket ();


    //## Other Operations (specified)
      //## Operation: GetTicketCode%926583149; C++
      m4pchar_t GetTicketCode ();

	  void* GetRowsQuotaCtrl();

	  void SetRowsQuotaCtrl(void* ai_pConCtroller);

	  void Lock();

	  void Unlock();

    // Additional Public Declarations
      //## begin ClUSSessionTicket%373B18A90399.public preserve=yes
      //## end ClUSSessionTicket%373B18A90399.public

  protected:
    // Additional Protected Declarations
      //## begin ClUSSessionTicket%373B18A90399.protected preserve=yes

	  void* m_pRowsQuotaCtrl;

	  // Allows creating a single quota controller per session
	  // while executing several parallel XML requests.
	  ClMutex m_oMutex;
      //## end ClUSSessionTicket%373B18A90399.protected

  private:
    // Additional Private Declarations
      //## begin ClUSSessionTicket%373B18A90399.private preserve=yes
      //## end ClUSSessionTicket%373B18A90399.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClUSSessionTicket%373B18A90399.implementation preserve=yes
      //## end ClUSSessionTicket%373B18A90399.implementation

};

//## begin ClUSSessionTicket%373B18A90399.postscript preserve=yes
//## end ClUSSessionTicket%373B18A90399.postscript

// Class ClUSSessionStats 

//## Get and Set Operations for Associations (inline)

inline ClStatTimeType& ClUSSessionStats::GettLastTransaction ()
{
  //## begin ClUSSessionStats::GettLastTransaction%36F8AB7101CB.get preserve=no
  return m_tLastTransaction;
  //## end ClUSSessionStats::GettLastTransaction%36F8AB7101CB.get
}

inline void ClUSSessionStats::SettLastTransaction (const ClStatTimeType& value)
{
  //## begin ClUSSessionStats::SettLastTransaction%36F8AB7101CB.set preserve=no
  m_tLastTransaction = value;
  //## end ClUSSessionStats::SettLastTransaction%36F8AB7101CB.set
}

inline ClStatNumericalType& ClUSSessionStats::GetnPendingReqs ()
{
  //## begin ClUSSessionStats::GetnPendingReqs%373C05220349.get preserve=no
  return m_nPendingReqs;
  //## end ClUSSessionStats::GetnPendingReqs%373C05220349.get
}

inline void ClUSSessionStats::AddnPendingReqs (const ClStatNumericalType& value)
{
  //## begin ClUSSessionStats::AddnPendingReqs%373C05220349.set preserve=no
  m_nPendingReqs += value;
  //## end ClUSSessionStats::AddnPendingReqs%373C05220349.set
}

inline ClStatNumericalType& ClUSSessionStats::GetnProcessedReqs ()
{
  //## begin ClUSSessionStats::GetnProcessedReqs%373C052B0216.get preserve=no
  return m_nProcessedReqs;
  //## end ClUSSessionStats::GetnProcessedReqs%373C052B0216.get
}

inline void ClUSSessionStats::AddnProcessedReqs (const ClStatNumericalType& value)
{
  //## begin ClUSSessionStats::AddnProcessedReqs%373C052B0216.set preserve=no
  m_nProcessedReqs += value;
  //## end ClUSSessionStats::AddnProcessedReqs%373C052B0216.set
}

inline ClStatNumericalType& ClUSSessionStats::GetnSubrogatedSessions ()
{
  //## begin ClUSSessionStats::GetnSubrogatedSessions%373C052E02B0.get preserve=no
  return m_nSubrogatedSessions;
  //## end ClUSSessionStats::GetnSubrogatedSessions%373C052E02B0.get
}

inline void ClUSSessionStats::AddnSubrogatedSessions (const ClStatNumericalType& value)
{
  //## begin ClUSSessionStats::AddnSubrogatedSessions%373C052E02B0.set preserve=no
  m_nSubrogatedSessions += value;
  //## end ClUSSessionStats::AddnSubrogatedSessions%373C052E02B0.set
}

inline ClStatTimeType& ClUSSessionStats::GettLogonTime ()
{
  //## begin ClUSSessionStats::GettLogonTime%397F033801E3.get preserve=no
  return m_tLogonTime;
  //## end ClUSSessionStats::GettLogonTime%397F033801E3.get
}

inline void ClUSSessionStats::AddtLogonTime (const ClStatTimeType& value)
{
  //## begin ClUSSessionStats::AddtLogonTime%397F033801E3.set preserve=no
  m_tLogonTime += value;
  //## end ClUSSessionStats::AddtLogonTime%397F033801E3.set
}

// Class ClUSSessionVisitor 

// Class ClUSSession 

// Class ClUSSessionTicket 

//## begin module%34C860E00242.epilog preserve=yes
//## end module%34C860E00242.epilog


#endif

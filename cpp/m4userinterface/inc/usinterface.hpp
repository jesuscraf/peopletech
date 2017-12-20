//## begin module%373A8AF40056.cm preserve=no
//## end module%373A8AF40056.cm

//## begin module%373A8AF40056.cp preserve=no
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
//## end module%373A8AF40056.cp

//## Module: USInterface%373A8AF40056; Package specification
//## Subsystem: M4UserInterface::inc%3795DCBF0108
//## Source file: F:\integration\m4userinterface\inc\usinterface.hpp

#ifndef usinterface_h
#define usinterface_h 1

//## begin module%373A8AF40056.additionalIncludes preserve=no
//## end module%373A8AF40056.additionalIncludes

//## begin module%373A8AF40056.includes preserve=yes
#include <m4win.hpp>
#include <m4userinterface_dll.hpp>
#include <subsystemids.hpp>
#include <m4trace.hpp>
#include <baserequest.hpp>
#include <m4types.hpp>
#include <m4string.hpp>
#include <connbase.hpp>
#include <clbaseobject.hpp>
#include <exvariables.hpp>

class ClUSSessionTicketInterface;
class ClVMBaseEnv;


//## begin module%373A8AF40056.declarations preserve=no
//## end module%373A8AF40056.declarations

//## begin module%373A8AF40056.additionalDeclarations preserve=yes
const m4uint32_t DefSessionSize		= 20;

const m4uint32_t SessAuthenticated	= 0;
const m4uint32_t SessAuthorized		= 1;
const m4uint32_t SessBlocked		= 2;
const m4uint32_t SessCanceling		= 3;
const m4uint32_t SessCanceled		= 4;
const m4uint32_t SessClosed			= 5;

const m4pchar_t SystemUser		= "System User";
const m4pchar_t	SystemPassword	= "System Password";
const m4pchar_t SystemHost		= "System Host";
//## end module%373A8AF40056.additionalDeclarations


//## Class: ClUSUserInterface%342240650391
//	This class contain all the information about the user and the apropiate
//	referneces to the nedeed objects such as the connections of that user.
//## Category: M4UserInterface%35EFA1CC011D
//## Subsystem: M4UserInterface::inc%3795DCBF0108
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%354F165C00C6;ClUSSessionInterface { -> F}

class M4_DECL_M4USERINTERFACE ClUSUserInterface : public ClBaseObject  //## Inherits: <unnamed>%3551D97C0229
{
  public:
    //## Destructor (specified)
      //## Operation: ~ClUSUserInterface%876496975
      //	Destructor.
      virtual ~ClUSUserInterface ();


    //## Other Operations (specified)
      //## Operation: GetUserId%898075017
      //	Returns the name of the object
      void GetUserId (m4string_t &ao_strUserID);

      //## Operation: GetName%876496982
      //	Returns the name of the object
      void GetName (m4string_t &ao_strName);

	  void GetUserAlias (m4string_t &ao_strUserAlias);
	  void GetUserImpersonator (m4string_t &ao_strUserImpersonator);

    // Data Members for Associations

      //## Association: M4UserInterface::<unnamed>%378EF6FB0289
      //## Role: ClUSUserInterface::m_oNumSessions%378EF6FC0140
      //## begin ClUSUserInterface::m_oNumSessions%378EF6FC0140.role preserve=no  public: ClNumRequest { -> VHAN}
      ClNumRequest m_oNumSessions;
      //## end ClUSUserInterface::m_oNumSessions%378EF6FC0140.role

  protected:
    //## Constructors (specified)
      //## Operation: ClUSUserInterface%876496976; C++
      //	Default constructor
      ClUSUserInterface (const m4string_t &ai_strUserID, const m4string_t &ai_strName, ClStatistic *ai_poStatistic, const m4string_t ai_strUserAlias , const m4string_t ai_strUserImpersonator );

    // Data Members for Associations

      //## Association: M4UserInterface::<unnamed>%397EC3A503D6
      //## Role: ClUSUserInterface::m_strName%397EC3A601A6
      //## begin ClUSUserInterface::m_strName%397EC3A601A6.role preserve=no  protected: m4string_t { -> 1VHAN}
      m4string_t m_strName;
      //## end ClUSUserInterface::m_strName%397EC3A601A6.role

      //## Association: M4UserInterface::<unnamed>%397EC426027D
      //## Role: ClUSUserInterface::m_strUserID%397EC42700F8
      //## begin ClUSUserInterface::m_strUserID%397EC42700F8.role preserve=no  protected: m4string_t { -> 1VHAN}
      m4string_t m_strUserID;
      //## end ClUSUserInterface::m_strUserID%397EC42700F8.role

	  m4string_t m_strUserAlias;
	  m4string_t m_strUserImpersonator;

  private:
  private: //## implementation
};

//## Class: ClUSSessionInterface%34C7A1210041
//	A session
//## Category: M4UserInterface%35EFA1CC011D
//## Subsystem: M4UserInterface::inc%3795DCBF0108
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4USERINTERFACE ClUSSessionInterface : public ClBaseObject  //## Inherits: <unnamed>%358E8B3B0383
{
  public:
    //## Destructor (specified)
      //## Operation: ~ClUSSessionInterface%893349364
      virtual ~ClUSSessionInterface ();


    //## Other Operations (specified)
      //## Operation: SetUser%893412529
      void SetUser (ClUSUserInterface *ai_poUser);

      //## Operation: GetUser%893412530
      ClUSUserInterface * GetUser () const;

      //## Operation: GetConnection%893427621
      ClConnBase * GetConnection () const;

      //## Operation: GetSessionTicket%895580957; C++
      m4return_t GetSessionTicket (ClUSSessionTicketInterface *&ao_poUSTicket) const;

      //## Operation: AddRequest%901612227; C++
      virtual void AddRequest () = 0;

      //## Operation: RemoveRequest%901612228; C++
      virtual void RemoveRequest () = 0;

      //## Operation: GetSessionID%926504936; C++
      //	Return the object Id
      m4return_t GetSessionID (m4string_t &ao_strID) const;

      //## Operation: GetStatus%970572590
      m4uint32_t GetStatus ();

      //## Operation: SetStatus%970572591
      void SetStatus (m4uint32_t ai_iStatus);

      //## Operation: GetConnProtocol%971774178
      eConnProtocol GetConnProtocol ();

      //## Operation: SetConnProtocol%971774179
      void SetConnProtocol (eConnProtocol ai_lConnProtocol);

      //## Operation: SetHost%982693606
      void SetHost (m4pchar_t ai_szHost);

      //## Operation: GetHost%982693607
      m4pcchar_t GetHost ();

	  m4uint32_t GetCompressionType () const;
	  m4uint32_t GetCompressionLevel() const;

    // Data Members for Associations

      //## Association: M4UserInterface::<unnamed>%3A92B8FE012A
      //## Role: ClUSSessionInterface::m_oStatus%3A92B8FE031E
      //## begin ClUSSessionInterface::m_oStatus%3A92B8FE031E.role preserve=no  public: ClStatus { -> VHAN}
      ClStatus m_oStatus;
      //## end ClUSSessionInterface::m_oStatus%3A92B8FE031E.role  

	  void SetMustCleanDelta( m4bool_t ai_bMustClean );

	  m4bool_t GetAuditSession( void ) const;

  protected:
    //## Constructors (specified)
      //## Operation: ClUSSessionInterface%885806463; C++
      ClUSSessionInterface (const m4uint32_t ai_uiId, const m4string_t &ai_strSessID, ClConnBase *ai_poConnection, ClUSUserInterface *ai_poUser, ClStatistic *ai_poStats, ClUSSessionTicketInterface *ai_poUSTicket, ClVMBaseEnv* ai_poEnvironment, const m4uint32_t ai_uiCompType, const m4uint32_t ai_uiCompLevel, m4bool_t ai_bAuditSession);

    // Data Members for Associations

      //## Association: M4UserInterface::<unnamed>%37395615007A
      //## Role: ClUSSessionInterface::m_strSessionID%3739561502C8
      //## begin ClUSSessionInterface::m_strSessionID%3739561502C8.role preserve=no  protected: m4string_t {1 -> 1VHAN}
      m4string_t m_strSessionID;
      //## end ClUSSessionInterface::m_strSessionID%3739561502C8.role

      //## Association: M4UserInterface::<unnamed>%373B0E6E0018
      //## Role: ClUSSessionInterface::m_poSessionTicket%373B0E6E00CC
      //## begin ClUSSessionInterface::m_poSessionTicket%373B0E6E00CC.role preserve=no  protected: ClUSSessionTicketInterface {1 -> 1RFHAN}
      ClUSSessionTicketInterface *m_poSessionTicket;
      //## end ClUSSessionInterface::m_poSessionTicket%373B0E6E00CC.role

      //## Association: M4UserInterface::<unnamed>%3795E8280021
      //## Role: ClUSSessionInterface::m_pConnection%3795E828018A
      //## begin ClUSSessionInterface::m_pConnection%3795E828018A.role preserve=no  protected: ClConnBase { -> 1RHAN}
      ClConnBase *m_pConnection;
      //## end ClUSSessionInterface::m_pConnection%3795E828018A.role

      //## Association: M4UserInterface::<unnamed>%3A92B899032D
      //## Role: ClUSSessionInterface::m_pUser%3A92B89A0126
      //## begin ClUSSessionInterface::m_pUser%3A92B89A0126.role preserve=no  protected: ClUSUserInterface { -> RHAN}
      ClUSUserInterface *m_pUser;
      //## end ClUSSessionInterface::m_pUser%3A92B89A0126.role

      m4return_t DeleteSessionReferences(ClVMBaseEnv* ai_poEnvironment);

  private:
    // Data Members for Class Attributes

      //## Attribute: m_lConnProtocol%39EC18BC03CA
      //## begin ClUSSessionInterface::m_lConnProtocol%39EC18BC03CA.attr preserve=no  private: eConnProtocol {UA} 
      eConnProtocol m_lConnProtocol;
      //## end ClUSSessionInterface::m_lConnProtocol%39EC18BC03CA.attr

      //## Attribute: m_szHost%3A92B68E01A5
      //## begin ClUSSessionInterface::m_szHost%3A92B68E01A5.attr preserve=no  private: m4string_t {UA} ""
      m4string_t m_szHost;
      //## end ClUSSessionInterface::m_szHost%3A92B68E01A5.attr

	  m4uint32_t m_lCompressionType;
	  m4uint32_t m_lCompressionLevel;

	  m4bool_t m_bMustCleanDelta;

	  m4bool_t m_bAuditSession;

  private: //## implementation
};

//## Class: ClUSSessionTicketInterface%373B0E53027C
//## Category: M4UserInterface%35EFA1CC011D
//## Subsystem: M4UserInterface::inc%3795DCBF0108
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4USERINTERFACE ClUSSessionTicketInterface 
{
  public:
    //## Destructor (specified)
      //## Operation: ~ClUSSessionTicketInterface%926583151
      virtual ~ClUSSessionTicketInterface ();


    //## Other Operations (specified)
      //## Operation: GetTicketCode%926583152; C++
      virtual m4pchar_t GetTicketCode () = 0;

	  virtual void* GetRowsQuotaCtrl() = 0;

	  virtual void SetRowsQuotaCtrl(void* ai_pConCtroller) = 0;

	  virtual void Lock() = 0;

	  virtual void Unlock() = 0;

  protected:
    //## Constructors (specified)
      //## Operation: ClUSSessionTicketInterface%926583154; C++
      ClUSSessionTicketInterface (m4char_t *ai_poTicketCode);

    // Data Members for Associations

      //## Association: M4UserInterface::<unnamed>%374ABF790145
      //## Role: ClUSSessionTicketInterface::m_poTicketCode%374ABF7902F4
      //## begin ClUSSessionTicketInterface::m_poTicketCode%374ABF7902F4.role preserve=no  protected: m4char_t { -> 1RHAN}
      m4char_t *m_poTicketCode;
      //## end ClUSSessionTicketInterface::m_poTicketCode%374ABF7902F4.role

  private:
  private: //## implementation
};

//## begin ClUSSessionInterface40%3993BECC01A1.preface preserve=yes
//## end ClUSSessionInterface40%3993BECC01A1.preface

//## Class: ClUSSessionInterface40%3993BECC01A1
//## Category: M4UserInterface%35EFA1CC011D
//## Subsystem: M4UserInterface::inc%3795DCBF0108
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4USERINTERFACE ClUSSessionInterface40 : public ClBaseObject  //## Inherits: <unnamed>%3993BF3202DE
{
  //## begin ClUSSessionInterface40%3993BECC01A1.initialDeclarations preserve=yes
  //## end ClUSSessionInterface40%3993BECC01A1.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClUSSessionInterface40%965977100
      ClUSSessionInterface40 (const m4uint32_t ai_uiId, const m4string_t &ai_strSessID, ClUSUserInterface *ai_poUser, ClStatistic *ai_poStats, ClUSSessionTicketInterface *ai_poUSTicket = NULL);

    //## Destructor (specified)
      //## Operation: ~ClUSSessionInterface40%965977101
      ~ClUSSessionInterface40 ();


    //## Other Operations (specified)
      //## Operation: SetUser%965977102
      void SetUser (ClUSUserInterface *ai_poUser);

      //## Operation: GetUser%965977103
      ClUSUserInterface * GetUser () const;

      //## Operation: GetSessionTicket%965977104; C++
      m4return_t GetSessionTicket (ClUSSessionTicketInterface *&ao_poUSTicket) const;

      //## Operation: GetSessionID%965977105; C++
      //	Return the object Id
      m4return_t GetSessionID (m4string_t &ao_strID) const;

      //## Operation: AddRequest%965977106; C++
      virtual void AddRequest () = 0;

      //## Operation: RemoveRequest%965977107; C++
      virtual void RemoveRequest () = 0;

      //## Operation: GetStatus%965977108
      m4uint32_t GetStatus ();

      //## Operation: SetStatus%965977109
      void SetStatus (m4uint32_t ai_iStatus);

    // Additional Public Declarations
      //## begin ClUSSessionInterface40%3993BECC01A1.public preserve=yes
      //## end ClUSSessionInterface40%3993BECC01A1.public

  protected:
    // Data Members for Associations

      //## Association: M4UserInterface::<unnamed>%3993D69801F1
      //## Role: ClUSSessionInterface40::m_poSessionTicket%3993D6990094
      //## begin ClUSSessionInterface40::m_poSessionTicket%3993D6990094.role preserve=no  protected: ClUSSessionTicketInterface { -> RHAN}
      ClUSSessionTicketInterface *m_poSessionTicket;
      //## end ClUSSessionInterface40::m_poSessionTicket%3993D6990094.role

      //## Association: M4UserInterface::<unnamed>%3993DDC0034A
      //## Role: ClUSSessionInterface40::m_poUser%3993DDC10125
      //## begin ClUSSessionInterface40::m_poUser%3993DDC10125.role preserve=no  protected: ClUSUserInterface { -> RHAN}
      ClUSUserInterface *m_poUser;
      //## end ClUSSessionInterface40::m_poUser%3993DDC10125.role

    // Additional Protected Declarations
      //## begin ClUSSessionInterface40%3993BECC01A1.protected preserve=yes
      //## end ClUSSessionInterface40%3993BECC01A1.protected

  private:
    // Data Members for Associations

      //## Association: M4UserInterface::<unnamed>%3993BF7403DE
      //## Role: ClUSSessionInterface40::m_strSessionID%3993BF75021C
      //## begin ClUSSessionInterface40::m_strSessionID%3993BF75021C.role preserve=no  private: m4string_t { -> VHAN}
      m4string_t m_strSessionID;
      //## end ClUSSessionInterface40::m_strSessionID%3993BF75021C.role

      //## Association: M4UserInterface::<unnamed>%3993D43D00FA
      //## Role: ClUSSessionInterface40::m_oStatus%3993D43D0317
      //## begin ClUSSessionInterface40::m_oStatus%3993D43D0317.role preserve=no  private: ClStatus { -> VHAN}
      ClStatus m_oStatus;
      //## end ClUSSessionInterface40::m_oStatus%3993D43D0317.role

    // Additional Private Declarations
      //## begin ClUSSessionInterface40%3993BECC01A1.private preserve=yes
      //## end ClUSSessionInterface40%3993BECC01A1.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClUSSessionInterface40%3993BECC01A1.implementation preserve=yes
      //## end ClUSSessionInterface40%3993BECC01A1.implementation

};

//## begin ClUSSessionInterface40%3993BECC01A1.postscript preserve=yes
//## end ClUSSessionInterface40%3993BECC01A1.postscript

// Class ClUSUserInterface 


//## Other Operations (inline)
inline void ClUSUserInterface::GetUserId (m4string_t &ao_strUserID)
{
  //## begin ClUSUserInterface::GetUserId%898075017.body preserve=yes
	ao_strUserID = m_strUserID;
  //## end ClUSUserInterface::GetUserId%898075017.body
}

inline void ClUSUserInterface::GetName (m4string_t &ao_strName)
{
  //## begin ClUSUserInterface::GetName%876496982.body preserve=yes
	ao_strName = m_strName;
  //## end ClUSUserInterface::GetName%876496982.body
}

inline void ClUSUserInterface::GetUserAlias (m4string_t &ao_strUserAlias)
{
	ao_strUserAlias = m_strUserAlias ;
}

inline void ClUSUserInterface::GetUserImpersonator (m4string_t &ao_strUserImpersonator)
{
	ao_strUserImpersonator = m_strUserImpersonator ;
}
// Class ClUSSessionInterface 


//## Other Operations (inline)
inline void ClUSSessionInterface::SetUser (ClUSUserInterface *ai_poUser)
{
  //## begin ClUSSessionInterface::SetUser%893412529.body preserve=yes
	m_pUser = ai_poUser ;
  //## end ClUSSessionInterface::SetUser%893412529.body
}

inline ClUSUserInterface * ClUSSessionInterface::GetUser () const
{
  //## begin ClUSSessionInterface::GetUser%893412530.body preserve=yes
	return m_pUser ;
  //## end ClUSSessionInterface::GetUser%893412530.body
}

inline ClConnBase * ClUSSessionInterface::GetConnection () const
{
  //## begin ClUSSessionInterface::GetConnection%893427621.body preserve=yes
	return m_pConnection ;
  //## end ClUSSessionInterface::GetConnection%893427621.body
}

inline m4return_t ClUSSessionInterface::GetSessionTicket (ClUSSessionTicketInterface *&ao_poUSTicket) const
{
  //## begin ClUSSessionInterface::GetSessionTicket%895580957.body preserve=yes
	ao_poUSTicket = m_poSessionTicket;

	return M4_SUCCESS;
  //## end ClUSSessionInterface::GetSessionTicket%895580957.body
}

inline m4return_t ClUSSessionInterface::GetSessionID (m4string_t &ao_strID) const
{
  //## begin ClUSSessionInterface::GetSessionID%926504936.body preserve=yes
	ao_strID = m_strSessionID;

	 return M4_SUCCESS;
  //## end ClUSSessionInterface::GetSessionID%926504936.body
}

inline m4uint32_t ClUSSessionInterface::GetStatus ()
{
  //## begin ClUSSessionInterface::GetStatus%970572590.body preserve=yes
 	return m_oStatus.GetValue () ;
  //## end ClUSSessionInterface::GetStatus%970572590.body
}

inline void ClUSSessionInterface::SetStatus (m4uint32_t ai_iStatus)
{
  //## begin ClUSSessionInterface::SetStatus%970572591.body preserve=yes
	m_oStatus.SetValue ( (m4int8_t) ai_iStatus  ) ;
  //## end ClUSSessionInterface::SetStatus%970572591.body
}

inline m4uint32_t ClUSSessionInterface::GetCompressionType () const{
	return m_lCompressionType;
}

inline m4uint32_t ClUSSessionInterface::GetCompressionLevel () const{
	return m_lCompressionLevel;
}

// Class ClUSSessionTicketInterface 

// Class ClUSSessionInterface40 


//## Other Operations (inline)
inline void ClUSSessionInterface40::SetUser (ClUSUserInterface *ai_poUser)
{
  //## begin ClUSSessionInterface40::SetUser%965977102.body preserve=yes
	m_poUser = ai_poUser ;
  //## end ClUSSessionInterface40::SetUser%965977102.body
}

inline ClUSUserInterface * ClUSSessionInterface40::GetUser () const
{
  //## begin ClUSSessionInterface40::GetUser%965977103.body preserve=yes
	return m_poUser ;
  //## end ClUSSessionInterface40::GetUser%965977103.body
}

inline m4return_t ClUSSessionInterface40::GetSessionTicket (ClUSSessionTicketInterface *&ao_poUSTicket) const
{
  //## begin ClUSSessionInterface40::GetSessionTicket%965977104.body preserve=yes
	ao_poUSTicket = m_poSessionTicket;

	return M4_SUCCESS;
  //## end ClUSSessionInterface40::GetSessionTicket%965977104.body
}

inline m4return_t ClUSSessionInterface40::GetSessionID (m4string_t &ao_strID) const
{
  //## begin ClUSSessionInterface40::GetSessionID%965977105.body preserve=yes
	ao_strID = m_strSessionID;

	 return M4_SUCCESS;
  //## end ClUSSessionInterface40::GetSessionID%965977105.body
}

inline m4uint32_t ClUSSessionInterface40::GetStatus ()
{
  //## begin ClUSSessionInterface40::GetStatus%965977108.body preserve=yes
	return m_oStatus.GetValue () ;
  //## end ClUSSessionInterface40::GetStatus%965977108.body
}

inline void ClUSSessionInterface40::SetStatus (m4uint32_t ai_iStatus)
{
  //## begin ClUSSessionInterface40::SetStatus%965977109.body preserve=yes
	m_oStatus.SetValue ( (m4int8_t) ai_iStatus  ) ;
  //## end ClUSSessionInterface40::SetStatus%965977109.body
}

//## begin module%373A8AF40056.epilog preserve=yes
//## end module%373A8AF40056.epilog


#endif

//## begin module%3528FAF60049.cm preserve=no
//## end module%3528FAF60049.cm

//## begin module%3528FAF60049.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            M4ExeFactory
//	 File:              exeforuuss.hpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:				980407
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
//## end module%3528FAF60049.cp

//## Module: ExeForUUSS%3528FAF60049; Package specification
//## Subsystem: M4ExeForChannel::UUSS::inc%3922782C01F5
//## Source file: D:\Work\v600\m4exeforchannel\inc\exeforuuss.hpp

#ifndef exeforuuss_h
#define exeforuuss_h 1

//## begin module%3528FAF60049.additionalIncludes preserve=no
//## end module%3528FAF60049.additionalIncludes

//## begin module%3528FAF60049.includes preserve=yes
#include "m4exeforchannel_dll.hpp"

//## end module%3528FAF60049.includes

// executiveforoe
#include <executiveforoe.hpp>
// ChannelUtil
#include <channelutil.hpp>
// m4credentials
#include <m4credentials.hpp>

class ISessionIDGenerator;

//## begin module%3528FAF60049.declarations preserve=no
//## end module%3528FAF60049.declarations

//## begin module%3528FAF60049.additionalDeclarations preserve=yes
//	ADD	EMN	990727	Para compilar. Hay que cambiar Connection por ConnBase
class ClConnection;
class m4VariantType;
class ClNode;
//## end module%3528FAF60049.additionalDeclarations

#define M4_MAX_USER_ALIAS	256
#define M4_MAX_USER_ID		30

//## begin ClExeForUUSS%355ABEBE02E5.preface preserve=yes
//## end ClExeForUUSS%355ABEBE02E5.preface

//## Class: ClExeForUUSS%355ABEBE02E5
//## Category: M4ExeForChannel::UUSS%38F32B29020C
//## Subsystem: M4ExeForChannel::UUSS::inc%3922782C01F5
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%357BACDE0196;IExecutor { -> F}
//## Uses: <unnamed>%357BACE10232;ClChannel { -> F}
//## Uses: <unnamed>%357BACE6004E;ClAccess { -> F}
//## Uses: <unnamed>%37444526027D;ClUSSession { -> F}
//## Uses: <unnamed>%37D8D23C0168;TDestroyer { -> }
//## Uses: <unnamed>%37D8D9C301CD;TChannelUtil { -> F}
//## Uses: <unnamed>%38D922EE02F8; { -> F}
//## Uses: <unnamed>%397F190E021D;ClRawSessionIDGenerator { -> F}
//## Uses: <unnamed>%39D21EB50015;M4Credentials { -> }

class M4_DECL_M4EXEFORCHANNEL ClExeForUUSS : public ClExecutiveForOE  //## Inherits: <unnamed>%355ABF410258
{
  //## begin ClExeForUUSS%355ABEBE02E5.initialDeclarations preserve=yes
  //## end ClExeForUUSS%355ABEBE02E5.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClExeForUUSS%895136725; C++
      ClExeForUUSS (ClEngine *ai_pEngine);

    //## Destructor (specified)
      //## Operation: ~ClExeForUUSS%895136726
      ~ClExeForUUSS ();


    //## Other Operations (specified)
      //## Operation: StartUp%895136727
      m4return_t StartUp (ClNavigatorFirewall *ai_pNavigator = NULL, ClConfiguration *ai_pConfig = NULL);

      //## Operation: Execute%895136728
      //	Remarks
      //	The context information provided is updated upon execution for the caller to
      //	return the status and some other meaningful information.
      m4return_t Execute (ClImage &aio_pImage);

      //## Operation: Reset%895136730; C++
      m4return_t Reset ();

      //## Operation: Authenticate%896786271; C++
      m4return_t Authenticate (m4pchar_t ai_pszUserId, m4pchar_t ai_pszPasswd, const m4bool_t ai_bUse2Way, ClUSUserInterface *&ao_pUser, m4int8_t &ao_iLogonMech, m4pchar_t ao_pcUserIdAlias, m4pchar_t ao_pcImpersonatorUser, m4int8_t ai_iLogonOrigin, m4pchar_t ai_pcClientType, m4pchar_t ai_pcClientPubIP);

      //## Operation: AuthenticateJob%951849538; C++
      m4return_t AuthenticateJob (m4pchar_t ai_pszUserId, ClUSUserInterface *&ao_pUser);

      //## Operation: ValidateCredential%942249961; C++
      m4return_t ValidateCredential (M4Credentials &ai_poCredential, ClUSUserInterface *&ao_pUser, ClUSSessionInterface *&ao_poSession);

      //## Operation: CheckRole%896786272; C++
      m4return_t CheckRole (ClUSSessionInterface &ai_oSession, m4pchar_t ai_pszRolId, m4bool_t ai_bIdDef);

      //## Operation: CreateSession%897296727; C++
      m4return_t CreateSession (ClUSUserInterface *&ai_poUser, const M4ClString &ai_strSessionId, ClUSSessionInterface *&ao_pSession, ClConnInstance *ai_poConnection, m4pchar_t ai_pcLanguage, m4pchar_t ai_pcOrganization, m4bool_t ai_bDefRole, m4bool_t ai_bClone, m4pchar_t ai_szSessContext, m4int8_t ai_iLogonMechanism, m4pchar_t ai_pcClientMachine, m4bool_t ai_bAuditSession, m4pchar_t ai_pcUserIdAlias, m4pchar_t ai_pcImpersonatorUser, m4int8_t ai_iLogonOrigin, m4pchar_t ai_pcSystemInfo, m4pchar_t ai_pcClientType, m4pchar_t ai_pcClientPubIP);

      //## Operation: ChangePassword%897296728; C++
      m4return_t ChangePassword (ClUSSessionInterface &ai_oSession, m4pchar_t ai_pszOldPassword, m4pchar_t ai_pszNewPassword);

      //## Operation: SetDefRole%918564929; C++
      m4return_t SetDefRole (ClUSUserInterface &ai_oUser, m4pchar_t ai_pszRolId);

    // Additional Public Declarations
      //## begin ClExeForUUSS%355ABEBE02E5.public preserve=yes
      m4return_t GetCertificate (m4pchar_t ai_pszUserName , m4pchar_t & ao_szCertificate ) ;

      
	  m4return_t Authenticate (ClUSUserInterface *&ai_poUser, ClUSSessionInterface *&ao_pSession, ClConnInstance *ai_poConnection, m4pchar_t ai_pszCA , m4pchar_t ai_szCertificate , m4uint32_t ai_iCertificateLen , m4pchar_t ai_szSessContext ) ;

	  // Auditoría de sesión
      m4return_t CleanAuditory( void ) ;
      m4return_t CheckAuditory( m4pcchar_t ai_sUser, m4bool_t ai_bCheckConcurrency, m4bool_t &ao_rbAuditSession ) ;
      m4return_t OpenAuditory( m4pcchar_t ai_sUser, m4uint32_t ai_iSession ) ;
      m4return_t CloseAuditory( m4pcchar_t ai_sUser, m4uint32_t ai_iSession ) ;


      //## end ClExeForUUSS%355ABEBE02E5.public
  protected:

    //## Other Operations (specified)
      //## Operation: _AtomicExecuteItem%926504939; C++
      m4return_t _AtomicExecuteItem (m4pcchar_t ai_pccM4Object, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4VariantType *ai_pvArguments, m4uint16_t ai_iLength, m4pcchar_t ai_pcInstance, ClChannel *ai_pcChannel = NULL);

      m4return_t _SetLicenseValue( ClNode * ai_poNode );

	  //## Operation: _ExecuteItem%926504940; C++
      m4return_t _ExecuteItem (ClChannel *ai_poChannel, ClAccess *ai_poAccess, ClNode *ai_poNode, m4pcchar_t ai_pccItem, m4VariantType *ai_pvArguments, m4uint16_t ai_iLength, m4pcchar_t ai_pcInstance, ClChannel *ai_pcChannel = NULL, m4VariantType *ai_pvOuts = NULL, m4uint16_t ai_iOut = 0);

      //## Operation: _CreateCredential%942249964; C++
      m4return_t _CreateCredential (const m4string_t &ai_strUserId, const m4uint_t &ai_oSecondsFromNow, m4string_t &ao_strSessionID);

      //## Operation: _GetUserFromCredential%942249965; C++
      m4return_t _GetUserFromCredential (const m4string_t &ai_strSessionID, m4string_t &ao_strUsername);

      //## Operation: _GetIDFromCredential%967533909; C++
      m4return_t _GetIDFromCredential (const m4string_t &ai_strSessionID, m4uint32_t &ao_iSessionID);

      //## Operation: _CreateSessionChannel%972994487
      m4return_t _CreateSessionChannel (ClUSUserInterface &ai_oUser, m4bool_t ai_bDefRole, m4pchar_t ai_pcLanguage, m4pchar_t ai_pcOrganization, ClChannel *&ao_poSessionChannel, m4pchar_t ai_szSessContext, m4int8_t ai_iLogonMechanism, m4pchar_t ai_pcClientMachine, m4pcchar_t ai_pccCredential, m4pchar_t ai_pcUserIdAlias, m4pchar_t ai_pcImpersonatorUser,  m4int8_t ai_iLogonOrigin, m4pchar_t ai_pcSystemInfo, m4pchar_t ai_pcClientType, m4pchar_t ai_pcClientPubIP);

      //## Operation: _CloneSessionChannel%972994488
      m4return_t _CloneSessionChannel (ClChannel *ai_poOrgSessionChannel, ClChannel *&ao_poDstSessionChannel);

	  // Auditoría de sesión
	  m4bool_t		m_bAuditInit ;
	  m4string_t	m_sHost ;
	  int			m_iPort ;
	  ClNode		*m_poAuditNode ;
	  ClAccess		*m_poAuditAccess ;
	  ClChannel		*m_poAuditChannel ;

      m4return_t _InitAuditory( void ) ;


    // Data Members for Associations

      //## Association: M4ExeForChannel::UUSS::<unnamed>%397EBDF602ED
      //## Role: ClExeForUUSS::m_poSessionIDGenerator%397EBDF7015E
      //## begin ClExeForUUSS::m_poSessionIDGenerator%397EBDF7015E.role preserve=no  protected: ISessionIDGenerator { -> 1RFHAN}
      ISessionIDGenerator *m_poSessionIDGenerator;
      //## end ClExeForUUSS::m_poSessionIDGenerator%397EBDF7015E.role

    // Additional Protected Declarations
      //## begin ClExeForUUSS%355ABEBE02E5.protected preserve=yes
      //## end ClExeForUUSS%355ABEBE02E5.protected

  private:
    // Additional Private Declarations
      //## begin ClExeForUUSS%355ABEBE02E5.private preserve=yes
      //## end ClExeForUUSS%355ABEBE02E5.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClExeForUUSS%355ABEBE02E5.implementation preserve=yes
      //## end ClExeForUUSS%355ABEBE02E5.implementation

  public:
	  // User actions auditory.
	  m4return_t ExeUserAuditoryLogoffAction( ClChannel* ai_poSessionChannel, m4pcchar_t ai_sUser ) ;
	  m4return_t ExeUserAuditoryLogonErrorAction( m4pchar_t ai_sUser, m4pchar_t ai_pcClient, m4pchar_t ai_pcClientType ) ;

  protected:
	  m4return_t _GetTokensFromCredential( const m4string_t &ai_strCredential, m4string_t &ao_strUserName, m4string_t &ao_strServerName, m4string_t &ao_strSessionID, m4string_t &ao_strHost, m4string_t &ao_strBasePort ) ;
};

//## begin ClExeForUUSS%355ABEBE02E5.postscript preserve=yes

// Resets VM state
class ClExeForUUSSVMResetter {

	private:

		IExecutor* m_poExecutor;

	public:
		
		ClExeForUUSSVMResetter(IExecutor* poExecutor);

		~ClExeForUUSSVMResetter();

};


//## end ClExeForUUSS%355ABEBE02E5.postscript

// Class ClExeForUUSS 

//## begin module%3528FAF60049.epilog preserve=yes
//## end module%3528FAF60049.epilog


#endif

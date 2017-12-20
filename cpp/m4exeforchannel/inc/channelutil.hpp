//## begin module%37D8D828003B.cm preserve=no
//## end module%37D8D828003B.cm

//## begin module%37D8D828003B.cp preserve=no
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
//## end module%37D8D828003B.cp

//## Module: ChannelUtil%37D8D828003B; Subprogram specification
//## Subsystem: M4ExeForChannel::UUSS::inc%3922782C01F5
//## Source file: F:\integration\m4exeforchannel\inc\channelutil.hpp

#ifndef channelutil_h
#define channelutil_h 1

//## begin module%37D8D828003B.additionalIncludes preserve=no
//## end module%37D8D828003B.additionalIncludes

//## begin module%37D8D828003B.includes preserve=yes
#include "stdio.h"
#include <m4srvres.hpp>
//## end module%37D8D828003B.includes

// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
// ConnProvider
#include <conn_provider.hpp>

class ClChannel_Kernel;
class ClAccess;
class ClChannel;
class ClChannelManager;

//## begin module%37D8D828003B.declarations preserve=no
//## end module%37D8D828003B.declarations

//## begin module%37D8D828003B.additionalDeclarations preserve=yes
//## end module%37D8D828003B.additionalDeclarations


//## Class: TChannelUtil%37D8D77000D7; Class Utility
//## Category: M4ExeForChannel::UUSS%38F32B29020C
//## Subsystem: M4ExeForChannel::UUSS::inc%3922782C01F5
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37D8D9640004;m4bool_t { -> }
//## Uses: <unnamed>%37D8D96600ED;m4return_t { -> }
//## Uses: <unnamed>%37D8D96A000C;ClAccess { -> F}
//## Uses: <unnamed>%37D8D96D0197;ClChannel { -> F}
//## Uses: <unnamed>%37D8D99A032C;m4pcchar_t { -> }
//## Uses: <unnamed>%37D8DC290202;ClLog { -> F}
//## Uses: <unnamed>%382AC52C010D;ClChannelManager { -> F}
//## Uses: <unnamed>%382AC7780358;ClChannel_Kernel { -> F}
//## Uses: <unnamed>%397ECAE600C9;m4string_t { -> }

class TChannelUtil 
{
  public:

    //## Other Operations (specified)
      //## Operation: CleanUpLinks%936955917; C++
      static m4return_t CleanUpLinks (ClChannel *ai_pChannel);

      //## Operation: CheckChannelID%936955918; C++
      static m4return_t CheckChannelID (ClChannel *ai_pChannel, m4pcchar_t ai_pszChannelID);

      //## Operation: SetAdmMode%942317488; C++
      static m4return_t SetAdmMode (ClChannelManager &aio_oChannelMngr);

      //## Operation: InitializeSession%942686871; C++
      static m4return_t InitializeSession (ClChannelManager &ai_oChannelManager, ClChannel &aio_oSessionChannel, const m4string_t &ai_strUsername, m4language_t ai_iLanguage, m4pchar_t ai_pcOrganization, const m4string_t &ai_strSessionID, m4int8_t ai_iLogonMechanism, m4pchar_t ai_pcClientMachine, m4pcchar_t ai_pccHost, m4pcchar_t ai_pccSessionID, m4pchar_t ai_pcUserIdAlias, m4pchar_t ai_pcImpersonatorUser, m4int8_t ai_iLogonOrigin, m4pchar_t ai_pcSystemInfo, m4pchar_t ai_pcClientType, m4pcchar_t ai_pccPort, m4pchar_t ai_pcClientPubIP ) ;


  protected:
  private:
  private: //## implementation
};

//## Class: TDestroyer%37D8C46C0380
//## Category: M4ExeForChannel::UUSS%38F32B29020C
//## Subsystem: M4ExeForChannel::UUSS::inc%3922782C01F5
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class TDestroyer 
{
  public:
    //## Constructors (specified)
      //## Operation: TDestroyer%936955907; C++
      TDestroyer (ClChannel *ai_pChannel, ClLConn_Provider *ai_pLConnProvider);

    //## Destructor (specified)
      //## Operation: ~TDestroyer%936955909
      ~TDestroyer ();


    //## Other Operations (specified)
      //## Operation: SetAccess%936955910; C++
      void SetAccess (ClAccess *ai_pAccess);

      //## Operation: SetDestroy%936955911; C++
      void SetDestroy (m4bool_t ai_bDestroy = M4_TRUE);

      //## Operation: ResetAccess%936955912
      void ResetAccess ();

      //## Operation: ResetChannel%936955913
      void ResetChannel ();

      //## Operation: ResetEnvironment%936955914
      void ResetEnvironment ();

  protected:
    //## Constructors (specified)
      //## Operation: TDestroyer%936955908
      TDestroyer ();

    // Data Members for Associations

      //## Association: M4ExeForChannel::UUSS::<unnamed>%38F358BF01F2
      //## Role: TDestroyer::m_pChannel%38F358C00009
      //## begin TDestroyer::m_pChannel%38F358C00009.role preserve=no  protected: ClChannel { -> 1RFHAN}
      ClChannel *m_pChannel;
      //## end TDestroyer::m_pChannel%38F358C00009.role

      //## Association: M4ExeForChannel::UUSS::<unnamed>%38F358F801FE
      //## Role: TDestroyer::m_pAccess%38F358F80398
      //## begin TDestroyer::m_pAccess%38F358F80398.role preserve=no  protected: ClAccess { -> 1RFHAN}
      ClAccess *m_pAccess;
      //## end TDestroyer::m_pAccess%38F358F80398.role

      //## Association: M4ExeForChannel::UUSS::<unnamed>%38F35919009D
      //## Role: TDestroyer::m_pLConnProvider%38F359190205
      //## begin TDestroyer::m_pLConnProvider%38F359190205.role preserve=no  protected: ClLConn_Provider { -> 1RHAN}
      ClLConn_Provider *m_pLConnProvider;
      //## end TDestroyer::m_pLConnProvider%38F359190205.role

      //## Association: M4ExeForChannel::UUSS::<unnamed>%38F495F2007F
      //## Role: TDestroyer::m_bDestroy%38F495F20287
      //## begin TDestroyer::m_bDestroy%38F495F20287.role preserve=no  protected: m4bool_t { -> 1VHAN}
      m4bool_t m_bDestroy;
      //## end TDestroyer::m_bDestroy%38F495F20287.role

  private:
  private: //## implementation
};

// Class Utility TChannelUtil 

// Class TDestroyer 


//## Other Operations (inline)
inline void TDestroyer::SetAccess (ClAccess *ai_pAccess)
{
  //## begin TDestroyer::SetAccess%936955910.body preserve=yes
	m_pAccess = ai_pAccess;
  //## end TDestroyer::SetAccess%936955910.body
}

inline void TDestroyer::SetDestroy (m4bool_t ai_bDestroy)
{
  //## begin TDestroyer::SetDestroy%936955911.body preserve=yes
	m_bDestroy = ai_bDestroy;
  //## end TDestroyer::SetDestroy%936955911.body
}

inline void TDestroyer::ResetAccess ()
{
  //## begin TDestroyer::ResetAccess%936955912.body preserve=yes
	m_pAccess = NULL;
  //## end TDestroyer::ResetAccess%936955912.body
}

inline void TDestroyer::ResetChannel ()
{
  //## begin TDestroyer::ResetChannel%936955913.body preserve=yes
	m_pChannel = NULL;
  //## end TDestroyer::ResetChannel%936955913.body
}

inline void TDestroyer::ResetEnvironment ()
{
  //## begin TDestroyer::ResetEnvironment%936955914.body preserve=yes
	m_pLConnProvider = NULL;
  //## end TDestroyer::ResetEnvironment%936955914.body
}

//## begin module%37D8D828003B.epilog preserve=yes
//## end module%37D8D828003B.epilog


#endif

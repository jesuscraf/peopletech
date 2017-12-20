//## begin module%37F095CB003C.cm preserve=no
//## end module%37F095CB003C.cm

//## begin module%37F095CB003C.cp preserve=no
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
//## end module%37F095CB003C.cp

//## Module: LaunchAppServer%37F095CB003C; Package specification
//## Subsystem: M4Appctl::inc%37EA675D02F1
//## Source file: F:\integration\M4Appctl\inc\launchappserver.hpp

#ifndef _LAUNCHAPPSERVER_HPP
#define _LAUNCHAPPSERVER_HPP 1

//## begin module%37F095CB003C.additionalIncludes preserve=no
//## end module%37F095CB003C.additionalIncludes

//## begin module%37F095CB003C.includes preserve=yes
#include <appctlreq.hpp>
//## end module%37F095CB003C.includes

// m4condition
#include <m4condition.hpp>
// reqrepository
#include <reqrepository.hpp>
// appctlsaxhandler
#include <appctlsaxhandler.hpp>
// CommonVariables
#include <commonvariables.hpp>
// m4communication
#include <m4communication.hpp>
// iexecutable
#include <iexecutable.hpp>
// FastQ
#include <fastfifoq.hpp>
//## begin module%37F095CB003C.declarations preserve=no
//## end module%37F095CB003C.declarations

//## begin module%37F095CB003C.additionalDeclarations preserve=yes
const m4pchar_t ACCEPT_ASCII_CONNECTION = "ACCEPT_ASCII_CONNECTION" ;
const m4pchar_t READ_ASCII_CONNECTION = "READ_ASCII_CONNECTION" ;
const m4pchar_t POST_WRITE_ASCII_CONNECTION = "POST_WRITE_ASCII_CONNECTION" ;
const m4pchar_t POST_CLOSE_CONNECTION = "POST_CLOSE_CONNECTION" ;
//## end module%37F095CB003C.additionalDeclarations


//## begin ClRequestQ%3B83BF9A03B6.preface preserve=yes
//## end ClRequestQ%3B83BF9A03B6.preface

//## Class: ClRequestQ%3B83BF9A03B6
//## Category: M4Appctl%37EA673500B9
//## Subsystem: M4Appctl::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3B83BFA901C3;FastQueue { -> }

typedef FastQueue < ClAppCtlRequest* >  ClRequestQ;

//## begin ClRequestQ%3B83BF9A03B6.postscript preserve=yes
//## end ClRequestQ%3B83BF9A03B6.postscript

//## begin ClLaunchAppServer%37F0949602C2.preface preserve=yes
//## end ClLaunchAppServer%37F0949602C2.preface

//## Class: ClLaunchAppServer%37F0949602C2
//## Category: M4Appctl%37EA673500B9
//## Subsystem: M4Appctl::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39045B42018E;ClXMLDoc { -> }
//## Uses: <unnamed>%392D517D0142;ClAppCtlSAXHandler { -> }

class ClLaunchAppServer : public ClExecutableInterface  //## Inherits: <unnamed>%392D495C02F9
{
  //## begin ClLaunchAppServer%37F0949602C2.initialDeclarations preserve=yes
  //## end ClLaunchAppServer%37F0949602C2.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClLaunchAppServer%938506847
      ClLaunchAppServer ();

    //## Destructor (specified)
      //## Operation: ~ClLaunchAppServer%939286310
      ~ClLaunchAppServer ();


    //## Other Operations (specified)
      //## Operation: Initialize%957775214
      m4return_t Initialize ();

      //## Operation: Run%957775215
      m4return_t Run ();

      //## Operation: Stop%957775219
      void Stop ();

      //## Operation: ExecuteAction%959265102
      m4return_t ExecuteAction (m4pchar_t ai_oActionString, ClParamList *ai_poParamList);

      //## Operation: AcceptedConnection%959265103
      m4return_t AcceptedConnection (ClParamList *ai_pParamList);

      //## Operation: ReadingConnection%959265104
      m4return_t ReadingConnection (ClParamList *ai_pParamList);

      //## Operation: SetAppCtlConfigPath%975327348
      void SetAppCtlConfigPath (m4pchar_t ai_psAppCtlExePath);

      //## Operation: PostWriteConnection%981474756
      m4return_t PostWriteConnection (ClParamList *ai_pParamList);

      //## Operation: PostCloseConnection %981535914
      m4return_t PostCloseConnection (ClParamList *ai_pParamList);

      //## Operation: ProcessRequest%998490240
      void ProcessRequest ();

    // Data Members for Class Attributes

      //## Attribute: m_ulMaxNumberOfAttempts%3817345800C1
      //## begin ClLaunchAppServer::m_ulMaxNumberOfAttempts%3817345800C1.attr preserve=no  public: m4uint32_t {UA} 5
      m4uint32_t m_ulMaxNumberOfAttempts;
      //## end ClLaunchAppServer::m_ulMaxNumberOfAttempts%3817345800C1.attr

    // Additional Public Declarations
      //## begin ClLaunchAppServer%37F0949602C2.public preserve=yes
      //## end ClLaunchAppServer%37F0949602C2.public

  protected:

    //## Other Operations (specified)
      //## Operation: LaunchAppServer%938506849
      m4return_t LaunchAppServer (ClAppCtlRequest *ai_poRequest);

      //## Operation: ReadConfigurationFile%938506846
      m4return_t ReadConfigurationFile (m4char_t *  ai_block_name, m4char_t *  ai_property_name, m4char_t * &  ao_pcvalue, m4uint32_t ai_iValueLen, m4pchar_t ai_pcConfigutationFile);

      //## Operation: SplitParseVariable%942399241
      m4return_t SplitParseVariable (m4pchar_t aio_value, m4pchar_t ao_before, m4pchar_t ao_variable, m4pchar_t ao_after);

      //## Operation: BuildLauncherPath%956568310
      m4return_t BuildLauncherPath (ClXMLDoc ai_XMLDoc, M4ClString &ao_pcLauncherPath, M4ClString &ao_pcConfigFile, m4uint32_t &ao_iRequestID, m4string_t &ao_szServerName, m4string_t &ao_szCommnad);

    // Data Members for Associations

      //## Association: M4Appctl::<unnamed>%3916AC09034D
      //## Role: ClLaunchAppServer::m_oCommonVariables%3916AC0A01BE
      //## begin ClLaunchAppServer::m_oCommonVariables%3916AC0A01BE.role preserve=no  protected: ClCommonVariables { -> VHAN}
      ClCommonVariables m_oCommonVariables;
      //## end ClLaunchAppServer::m_oCommonVariables%3916AC0A01BE.role

      //## Association: M4Appctl::<unnamed>%392D391E01AF
      //## Role: ClLaunchAppServer::m_poCommunications%392D391F01DA
      //## begin ClLaunchAppServer::m_poCommunications%392D391F01DA.role preserve=no  protected: ClCommunication { -> RHAN}
      ClCommunication *m_poCommunications;
      //## end ClLaunchAppServer::m_poCommunications%392D391F01DA.role

      //## Association: M4Appctl::<unnamed>%392E52E000BC
      //## Role: ClLaunchAppServer::m_oStopper%392E52E003DE
      //## begin ClLaunchAppServer::m_oStopper%392E52E003DE.role preserve=no  protected: ClCondition { -> VHAN}
      ClCondition m_oStopper;
      //## end ClLaunchAppServer::m_oStopper%392E52E003DE.role

      //## Association: M4Appctl::<unnamed>%3B83BFAE004E
      //## Role: ClLaunchAppServer::m_oRequestQ%3B83BFAF0262
      //## begin ClLaunchAppServer::m_oRequestQ%3B83BFAF0262.role preserve=no  protected: ClRequestQ { -> VHAN}
      ClRequestQ m_oRequestQ;
      //## end ClLaunchAppServer::m_oRequestQ%3B83BFAF0262.role

    // Additional Protected Declarations
      //## begin ClLaunchAppServer%37F0949602C2.protected preserve=yes
	  m4return_t _InitializeDispatchers ( ) ;
      //## end ClLaunchAppServer%37F0949602C2.protected

  private:

    //## Other Operations (specified)
      //## Operation: RunLauncherEXE%974113726
      m4return_t RunLauncherEXE (M4ClString &ai_pcLauncherPath, M4ClString &ai_pcConfigFile, ClXMLDoc &ai_XMLDoc, m4string_t &ai_szServerName, m4string_t &ai_szCommand);

    // Data Members for Class Attributes

      //## Attribute: m_psAppCtlConfigPath%3A2250E403A2
      //## begin ClLaunchAppServer::m_psAppCtlConfigPath%3A2250E403A2.attr preserve=no  private: m4pchar_t {UA} 0
      m4pchar_t m_psAppCtlConfigPath;
      //## end ClLaunchAppServer::m_psAppCtlConfigPath%3A2250E403A2.attr

    // Data Members for Associations

      //## Association: M4Appctl::<unnamed>%3A815E620072
      //## Role: ClLaunchAppServer::m_oRepository%3A815E620299
      //## begin ClLaunchAppServer::m_oRepository%3A815E620299.role preserve=no  private: ClReqRepository { -> VHAN}
      ClReqRepository m_oRepository;
      //## end ClLaunchAppServer::m_oRepository%3A815E620299.role

    // Additional Private Declarations
      //## begin ClLaunchAppServer%37F0949602C2.private preserve=yes
      //## end ClLaunchAppServer%37F0949602C2.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClLaunchAppServer%37F0949602C2.implementation preserve=yes
      //## end ClLaunchAppServer%37F0949602C2.implementation

};

//## begin ClLaunchAppServer%37F0949602C2.postscript preserve=yes
//## end ClLaunchAppServer%37F0949602C2.postscript

// Class ClLaunchAppServer 

//## begin module%37F095CB003C.epilog preserve=yes
//## end module%37F095CB003C.epilog


#endif

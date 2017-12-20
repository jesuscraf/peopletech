//## begin module.cm preserve=no
//## end module.cm

//## begin module.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            
//	 File:              adminmonclient.hpp
//	 Project:           M4AdminClient
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines the administrator client for server 
//		monitoring.
//
//
//	==============================================================================
//## end module.cp

//## Module: AdminMonClient; Package specification
//## Subsystem: CPM4AdminClient
//## Source file: d:\marcoscp\M4AdminClient\version\inc\adminmonclient.hpp

#ifndef adminmonclient_h
#define adminmonclient_h 1

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
//## end module.includes

#include <m4string.hpp>
// MonitorCache
#include <adminmoncache.hpp>
// AdminMonCommands
#include <adminmoncommands.hpp>
// TokenCommandTable
#include <tokencmdtable.hpp>
// Parser
#include <parser.hpp>
// ClientAPI
#include <clientapi.hpp>
//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes

//
// module for export/import dll.
#include "m4adminclient_dll.hpp"
//## end module.additionalDeclarations


//## Class: ClAdminMonClient; implementation
//	Administration client. It receives commands from a human
//	administrator, and sends the appropiate request to the
//	server administrator. Afterwards, receives the answer
//	from the server and sends it back to the human
//	administrator.
//## Category: M4AdminClient
//## Subsystem: CPM4AdminClient
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>; ClAdminMonitorParamConsultCommand { -> }
//## Uses: <unnamed>; vectorXunsigned_intX_class_allocatorXunsigned_intXX { -> }

class M4_DECL_M4ADMINCLIENT ClAdminMonClient 
{
  //## begin ClAdminMonClient.initialDeclarations preserve=yes
  //## end ClAdminMonClient.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClAdminMonClient%884781527
      ClAdminMonClient ();

    //## Destructor (specified)
      //## Operation: ~ClAdminMonClient%882296710
      ~ClAdminMonClient ();


    //## Other Operations (specified)
      //## Operation: Connect%885289654
      //	Connects the administrator to the server. Actually the
      //	conexion is done in three phases:
      //	   1 - open a sesion with the server.
      //	   2 - enable the administrator role.
      //	   3 - open the administration service.
      //
      //	Input Arguments:
      //	   ai_szServerAdress : the serveraddress or serverName.
      //	   ai_iServerPort: the server administration port.
      //	   ai_szUser, ai_szPassword: these parameters are the
      //	user identification.
      //
      //	Return:
      //	   M4_SUCCESS on success
      //	   M4_ERROR otherwise
      m4return_t Connect (const m4pchar_t ai_szServerAddress = NULL, const m4uint32_t ai_iServerPort = -1, const m4pchar_t ai_szUser = NULL, const m4pchar_t ai_szPassword = NULL);

      //## Operation: Disconnect%885289655
      //	Disconnects from the server.  These process has three
      //	phases:
      //	   1 - close administration service.
      //	   2 - disable administrator role.
      //	   3 - close session.
      //
      //	Return:
      //	   M4_SUCCESS on success
      //	   M4_ERROR otherwise
      m4return_t Disconnect ();

      //## Operation: Execute%884261081
      //	This method parses the input sentence (ai_szSentence).
      //	For each command in this sentence sends a request to the
      //	server. With all the individual answers builds the
      //	answer (ao_szAnswer) and set aio_uiSize) with the answer
      //	length.
      //
      //	The function return:
      //	   M4_SUCCESS on success
      //	   M4_WARNING if aio_uiSize is less than answer length
      //	   M4_ERROR on error conexion
      m4return_t Execute (m4pchar_t ai_szSentence, m4pchar_t ao_szAnswer, m4uint16_t &aio_uiSize);

      //## Operation: SendSyncRequest%901526255
      //	Send a sync request to the server.
      //	Input parameters:
      //	   ai_inputDS - DataStorage with the data that the user
      //	wants sends to the server.
      //	   ao_answer - server answer for the command in the
      //	request.
      //
      //	Return:
      //	   M4_SUCCESS on success
      //	   M4_ERROR otherwise
      m4return_t SendSyncRequest (M4DataStorage *ai_inputDS, M4DataStorage *ao_outputDS);

      //## Operation: CheckConnection%911389909
      //	Return if the administrator is connected or not.
      m4bool_t CheckConnection ();

      //## Operation: GetLogError%923332969
      //	Return a string with the first error in the log object,
      //	and clear this error from the object.
      //	Return code:
      //	   M4_SUCCESS on success.
      //	   M4_ERROR if there is not more errors in log object.
      m4return_t GetLogError (M4ClString &ao_type,  // return the error type (error, warning, etc)
      M4ClString &ao_Code,  // return the error code
      M4ClString &ao_Text // return the error string
      );

    // Data Members for Associations

      //## Association: M4AdminClient::<unnamed>%34BCF1170028
      //## Role: ClAdminMonClient::m_poParser
      //## begin ClAdminMonClient::m_poParser.role preserve=no  public: ClParser { -> 1RHAN}
      ClParser *m_poParser;
      //## end ClAdminMonClient::m_poParser.role

      //## Association: M4AdminClient::<unnamed>%34BCF2140253
      //## Role: ClAdminMonClient::m_oSentenceTable
      //## begin ClAdminMonClient::m_oSentenceTable.role preserve=no  public: ClSentenceTable { -> 1VHAN}
      ClSentenceTable m_oSentenceTable;
      //## end ClAdminMonClient::m_oSentenceTable.role

      //## Association: M4AdminClient::<unnamed>%34F5C7E8007F
      //## Role: ClAdminMonClient::m_oTokenCommandTable
      //## begin ClAdminMonClient::m_oTokenCommandTable.role preserve=no  public: ClTokenCommandTable { -> 1VHAN}
      ClTokenCommandTable m_oTokenCommandTable;
      //## end ClAdminMonClient::m_oTokenCommandTable.role

      //## Association: M4AdminClient::<unnamed>%36AF460A03BB
      //## Role: ClAdminMonClient::m_AdminInfoCache
      //## begin ClAdminMonClient::m_AdminInfoCache.role preserve=no  public: ClAdminInfoCache { -> 1UHAN}
      ClAdminInfoCache m_AdminInfoCache;
      //## end ClAdminMonClient::m_AdminInfoCache.role

    // Additional Public Declarations
      //## begin ClAdminMonClient.public preserve=yes
      //## end ClAdminMonClient.public

  protected:
    // Additional Protected Declarations
      //## begin ClAdminMonClient.protected preserve=yes
      //## end ClAdminMonClient.protected

  private:

    //## Other Operations (specified)
      //## Operation: SentenceClear%884781526
      //	This method  clears the m_oCommandTable.
      m4void_t SentenceClear ();

      //## Operation: DecodeDataStorage%897034497
      //	This method translate a dataStorage in a M4ClString. All
      //	dataUnit in the dataStorage are inserted in the string
      //	separated with colons (:).
      //
      //	the function return:
      //	   M4_SUCCESS on success
      //	   M4_ERROR otherwise
      m4return_t DecodeDataStorage (M4DataStorage *ai_data, M4ClString &ao_string);

      //## Operation: PreprocessingLogInfo%923332968
      //	Clear the log object before any request.
      m4return_t PreprocessingLogInfo (m4return_t ai_lastRetCode = M4_SUCCESS);

      //## Operation: PostprocessingLogInfo%923556879
      //	After any request, this function read the log object and
      //	insert as first error a new error that resumes all
      //	errors in the log object .
      m4return_t PostprocessingLogInfo (m4return_t ai_lastRetCode = M4_SUCCESS);

      //## Operation: DumpLogIngo%923556880
      //	Dump errors in log object. Only in debug mode.
      void DumpLogIngo ();

    // Data Members for Class Attributes

      //## Attribute: m_serverAnswer
      //	Aquí se va a almacenar el último resultado obtenido del
      //	subsistema que se administra (por si se vuelve a
      //	realizar la llamada, etc...).
      //## begin ClAdminMonClient::m_serverAnswer.attr preserve=no  private: M4ClString {UA} 
      M4ClString m_serverAnswer;
      //## end ClAdminMonClient::m_serverAnswer.attr

      //## Attribute: m_bConnected
      //## begin ClAdminMonClient::m_bConnected.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bConnected;
      //## end ClAdminMonClient::m_bConnected.attr

      //## Attribute: m_sessionId
      //## begin ClAdminMonClient::m_sessionId.attr preserve=no  private: m4handle_t {UA} 0
      m4handle_t m_sessionId;
      //## end ClAdminMonClient::m_sessionId.attr

      //## Attribute: m_serviceContext
      //## begin ClAdminMonClient::m_serviceContext.attr preserve=no  private: m4handle_t {UA} 0
      m4handle_t m_serviceContext;
      //## end ClAdminMonClient::m_serviceContext.attr

      //## Attribute: m_admServiceId
      //## begin ClAdminMonClient::m_admServiceId.attr preserve=no  private: static M4ClString {UA} "M4_ADMINISTRATION_SERVICE"
      static M4ClString m_admServiceId;
      //## end ClAdminMonClient::m_admServiceId.attr

      //## Attribute: m_logInfo
      //## begin ClAdminMonClient::m_logInfo.attr preserve=no  private: M4ClString {UA} 
      M4ClString m_logInfo;
      //## end ClAdminMonClient::m_logInfo.attr

    // Data Members for Associations

      //## Association: M4AdminClient::<unnamed>%357669300323
      //## Role: ClAdminMonClient::m_oAdmClient
      //## begin ClAdminMonClient::m_oAdmClient.role preserve=no  private: ClClientAPI { -> 1VHAN}
      ClClientAPI* m_pAdmClient;
      //## end ClAdminMonClient::m_oAdmClient.role

    // Additional Private Declarations
      //## begin ClAdminMonClient.private preserve=yes
      //## end ClAdminMonClient.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClAdminMonClient.implementation preserve=yes
      //## end ClAdminMonClient.implementation

};

//## begin ClAdminMonClient.postscript preserve=yes
//## end ClAdminMonClient.postscript



// Class ClAdminMonClient 





//## begin module.epilog preserve=yes
//## end module.epilog
#endif

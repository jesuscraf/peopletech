//## begin module%38DB484A0343.cm preserve=no
//## end module%38DB484A0343.cm

//## begin module%38DB484A0343.cp preserve=no
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
//## end module%38DB484A0343.cp

//## Module: m4communication%38DB484A0343; Package specification
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Source file: e:\mybuild\m4communication\inc\m4communication.hpp

#ifndef m4communication_h
#define m4communication_h 1

//## begin module%38DB484A0343.additionalIncludes preserve=no
#include <m4communication_dll.hpp>
//## end module%38DB484A0343.additionalIncludes

//## begin module%38DB484A0343.includes preserve=yes
#include <basepdus.hpp>
#include <commactionsname.h>
class ClCommAction;
//## end module%38DB484A0343.includes

// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
// iexecutable
#include <iexecutable.hpp>

class ClCommunicationImplementation;

//## begin module%38DB484A0343.declarations preserve=no
//## end module%38DB484A0343.declarations

//## begin module%38DB484A0343.additionalDeclarations preserve=yes
//## end module%38DB484A0343.additionalDeclarations


//## begin ClCommunication%38D1F37501A8.preface preserve=yes
#ifndef M4_COMM_NULL_PROTOCOL
#	define M4_COMM_NULL_PROTOCOL		0
#endif
#ifndef M4_COMM_PDU_PROTOCOL
#	define M4_COMM_PDU_PROTOCOL			1
#endif
#ifndef M4_COMM_ASCII_PROTOCOL
#	define M4_COMM_ASCII_PROTOCOL		2
#endif 
#ifndef M4_COMM_PDU_HTTP_PROTOCOL
#	define M4_COMM_PDU_HTTP_PROTOCOL	3
#endif
//## end ClCommunication%38D1F37501A8.preface

//## Class: ClCommunication%38D1F37501A8
//	Será la capa que haga de interface con el mundo exterior.
//	En una segunda fase, si se trabaja con acciones desaparecerá.
//
//	Todos los metodos tienen 2 parametros ai_pcActionName (char*) y ai_poAction
//	Executor (ClExecutableInterface *).
//	En caso de ser nulos se asumirá que queremos comportamiento sincrono. En
//	otro caso, una vez concluida la accion se ejecutara la accion sobre el
//	Ejecutor proporcionado.
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%38D22636001E;m4return_t { -> }
//## Uses: <unnamed>%38D75C0A0285;m4string_t { -> }
//## Uses: <unnamed>%3905C4B001C7;ClPDU { -> F}
//## Uses: <unnamed>%391ACECE03A0;ClExecutableInterface { -> }

class M4_DECL_M4COMMUNICATION ClCommunication 
{
  //## begin ClCommunication%38D1F37501A8.initialDeclarations preserve=yes
  //## end ClCommunication%38D1F37501A8.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommunication%955469190; C++
      ClCommunication (m4int_t ai_iControlPort = 6666, m4int_t ai_iNWorker = 1);

    //## Destructor (specified)
      //## Operation: ~ClCommunication%955469191
      ~ClCommunication ();


    //## Other Operations (specified)
      //## Operation: StartAccepting%953574268; C++
      m4return_t StartAccepting (m4int_t ai_iPort, m4pchar_t ai_pcActionName, ClExecutableInterface *ai_poActionExecutor, m4int_t ai_iCommProtocol = M4_COMM_PDU_PROTOCOL, m4string_t ai_strCert = "", m4string_t ai_strPrivKey = "", m4string_t ai_strPreferedCipher = "");

      //## Operation: OpenConnection%953574271; C++
      //	Asignamos a la session un socket conectado con los parametros que nos dan.
      m4return_t OpenConnection (m4handle_t ai_hConnectionID, m4string_t &ai_strHost, m4int_t ai_iPort, m4pchar_t ai_pcActionName, ClExecutableInterface *ai_poActionExecutor, m4int_t ai_iCommProtocol = M4_COMM_PDU_PROTOCOL, m4string_t  ai_strSSLCert = "", m4bool_t ai_bIsSocketPool = M4_FALSE);

      //## Operation: OpenConnection%958463332; C++
      //	Asignamos a la session un socket conectado con los parametros que nos dan.
      m4return_t OpenConnection (m4handle_t ai_hConnectionID, m4string_t &ai_strHost, m4int_t ai_iPort, m4int_t ai_iCommProtocol = M4_COMM_PDU_PROTOCOL, m4string_t  ai_strSSLCert = "", m4bool_t ai_bIsSocketPool = M4_FALSE);

      //## Operation: StartListening%953574273; C++
      m4return_t StartListening (m4handle_t ai_hConnectionID, m4pchar_t ai_pcActionName, ClExecutableInterface *ai_poActionExecutor);

      //## Operation: StartListening%958483733; C++
      m4return_t StartListening (m4handle_t ai_hConnectionID, ClParamList &ao_Params);

      //## Operation: Write%953574269; C++
      m4return_t Write (m4handle_t ai_hConnectionID, ClPDU *ai_poNetObject, m4pchar_t ai_pcActionName = NULL, ClExecutableInterface *ai_poActionExecutor = NULL);

      //## Operation: Write%955613882; C++
      m4return_t Write (m4handle_t ai_hConnectionID, m4pchar_t ai_pcChunk, m4size_t ai_iChunkSize, m4pchar_t ai_pcActionName = NULL, ClExecutableInterface *ai_poActionExecutor = NULL);

      //## Operation: Write%960977890; C++
      m4return_t Write (m4handle_t ai_hConnectionID, m4uint64_t ai_ui64RequestID, ClPDU *ai_poNetObject, m4pchar_t ai_pcActionName = NULL, ClExecutableInterface *ai_poActionExecutor = NULL);

      //## Operation: Write%960977891; C++
      m4return_t Write (m4handle_t ai_hConnectionID, m4uint64_t ai_ui64RequestID, m4pchar_t ai_pcChunk, m4size_t ai_iChunkSize, m4pchar_t ai_pcActionName = NULL, ClExecutableInterface *ai_poActionExecutor = NULL);

      //## Operation: StopAccepting%953574270; C++
      m4return_t StopAccepting (m4int_t ai_iPort);

      //## Operation: CloseConnection%953574272; C++
      m4return_t CloseConnection (m4handle_t ai_hConnectionID, m4pchar_t ai_pcActionName = NULL, ClExecutableInterface *ai_poActionExecutor = NULL);

      //## Operation: StopListening%953574274; C++
      m4return_t StopListening (m4handle_t ai_hConnectionID, m4pchar_t ai_pcActionName = NULL, ClExecutableInterface *ai_poActionExecutor = NULL);

      //## Operation: StopWritting%953574275; C++
      m4return_t StopWritting (m4handle_t ai_hConnectionID, m4pchar_t ai_pcActionName = NULL, ClExecutableInterface *ai_poActionExecutor = NULL);

      //## Operation: StopCommunicationLayer%958646097
      m4return_t StopCommunicationLayer ();

      //## Operation: GetCommStat%959072019; C++
      m4return_t GetCommStat (m4handle_t ai_hConnectionID, m4uint_t &ao_uiSentBytes, m4uint_t &ao_uiReadedBytes, m4uint_t &ao_uiSentRequests, m4uint_t &ao_uiReceivedRequests);

      //## Operation: GetCommParams%959072025; C++
      m4return_t GetCommParams (m4handle_t ai_hConnectionID, m4string_t &ao_strURL);

      //## Operation: GetError%959072028
      m4int_t GetError ();

      //## Operation: GetWorkerStat%964423557; C++
      m4return_t GetWorkerStat (m4int_t ai_iWorkerID, m4uint_t &ao_uiExecutedActions, m4uint_t &ao_uiExecutingTime, m4uint_t &ao_uiWaitingTime, m4uint_t &ao_uiExternalExecutingTime);

      //## Operation: AddWorker%967794011
      m4return_t AddWorker ();

      //## Operation: RemoveWorker%967794012
      m4return_t RemoveWorker ();

      //## Operation: GetNWorker%967794013
      m4int_t GetNWorker ();

      //## Operation: GetNPendingActions%967794014
      m4int_t GetNPendingActions ();

      //## Operation: SetNSocketPool%971164983
      m4return_t SetNSocketPool (m4string_t &ai_strURL, m4int_t ai_iNSocket);

      //## Operation: Write%972554299; C++
      m4return_t Write (m4handle_t ai_hConnectionID, m4uint32_t ai_iRequestID, ClPDU *ai_poNetObject, m4pchar_t ai_pcHTTPHeaders, m4pchar_t ai_pcActionName = NULL, ClExecutableInterface *ai_poActionExecutor = NULL);

      //## Operation: SetProtocolVersion%973247121; C++
      m4return_t SetProtocolVersion (m4handle_t ai_hConnectionID, m4int_t ai_iProtocolVersion);

      //## Operation: GetCommParams%977486320; C++
      m4return_t GetCommParams (m4handle_t ai_hConnectionID, ClParamList &aio_oParamList);

      //## Operation: StartShutdown%988885055
      m4return_t StartShutdown ();

      //## Operation: IsShuttedDown%988885056
      m4bool_t IsShuttedDown ();

	  m4return_t SetCommsLanguage( m4language_t ai_iLang ) ;
    // Additional Public Declarations
      //## begin ClCommunication%38D1F37501A8.public preserve=yes
      //## end ClCommunication%38D1F37501A8.public

  protected:

    //## Other Operations (specified)
      //## Operation: _PrepareReturn%965219978; C++
      m4return_t _PrepareReturn (m4pchar_t ai_pcActionName, ClExecutableInterface *ai_poActionExecutor, ClCommAction *ai_poCommAction, ClParamList *ai_poParams);

      //## Operation: _AnyErrors%988885057
      m4bool_t _AnyErrors ();

    // Data Members for Associations

      //## Association: M4Communication::M4COML::<unnamed>%38DB2C6101A5
      //## Role: ClCommunication::m_poImplementation%38DB2C6C03B3
      //## begin ClCommunication::m_poImplementation%38DB2C6C03B3.role preserve=no  protected: ClCommunicationImplementation { -> RFHAN}
      ClCommunicationImplementation *m_poImplementation;
      //## end ClCommunication::m_poImplementation%38DB2C6C03B3.role

    // Additional Protected Declarations
      //## begin ClCommunication%38D1F37501A8.protected preserve=yes
      //## end ClCommunication%38D1F37501A8.protected

  private:
    // Additional Private Declarations
      //## begin ClCommunication%38D1F37501A8.private preserve=yes
      //## end ClCommunication%38D1F37501A8.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommunication%38D1F37501A8.implementation preserve=yes
      //## end ClCommunication%38D1F37501A8.implementation

};

//## begin ClCommunication%38D1F37501A8.postscript preserve=yes
//## end ClCommunication%38D1F37501A8.postscript

// Class ClCommunication 

//## begin module%38DB484A0343.epilog preserve=yes
//## end module%38DB484A0343.epilog


#endif

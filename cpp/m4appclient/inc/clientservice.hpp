//## begin module%34951232028B.cm preserve=no
//## end module%34951232028B.cm

//## begin module%34951232028B.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            M4ClientAPI
//	 File:              clientservice.hpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:				980314
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
//## end module%34951232028B.cp

//## Module: clientservice%34951232028B; Package specification
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Source file: F:\integration\m4appclient\inc\clientservice.hpp

#ifndef clientservice_h
#define clientservice_h 1

//## begin module%34951232028B.additionalIncludes preserve=no
//## end module%34951232028B.additionalIncludes

//## begin module%34951232028B.includes preserve=yes
#include "m4stl.hpp"
#include <logsys.hpp>



#define M4_RETRY_CONNECT 100

//## end module%34951232028B.includes

// Clienttag
#include <clienttag.hpp>
// m4types
#include <m4types.hpp>
// m4date
#include <m4date.hpp>
// m4log
#include <m4log.hpp>
// controlpdus
#include <controlpdus.hpp>
// Protocol
#include <protocol.hpp>
// clientbasicdef
#include <clientbasicdef.hpp>
// clientsession
#include <clientsession.hpp>
// clientrequest
#include <clientrequest.hpp>
// reqdescriptor
#include <reqdescriptor.hpp>

class ClCCConnection;

//## begin module%34951232028B.declarations preserve=no
//## end module%34951232028B.declarations

//## begin module%34951232028B.additionalDeclarations preserve=yes
#define	ADMIN_SERVICE	""
#define	ADMIN_SERVICE_ID	0
class ClCCSession;
//## end module%34951232028B.additionalDeclarations


//## Class: ClCCService%3487D87D00E3
//	.
//	Esta clase está responsabilizada de almacenar todas las operaciones
//	pendientes.
//	.
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%350A50C10026;ClPDU { -> }
//## Uses: <unnamed>%3548AD62001C;M4ClCrono { -> }
//## Uses: <unnamed>%35936C870309;m4pchar_t { -> }
//## Uses: <unnamed>%35936D3400D7;map { -> }
//## Uses: <unnamed>%3593726303C4;ClLog { -> }
//## Uses: <unnamed>%3659AB870146;ClTimeoutFunction { -> }
//## Uses: <unnamed>%380F2D1400C6;ClLogBasedEventHandler { -> }
//## Uses: <unnamed>%38ABD76401DC;ClCCConnection { -> F}
//## Uses: <unnamed>%38AC0CD90374;ClCondition { -> }
//## Uses: <unnamed>%38AC0CFE00BA;ClCCMonitor { -> }
//## Uses: <unnamed>%3ADB043103B2;ClRequestDescriptor { -> }

class ClCCService 
{
  public:
    //## Constructors (specified)
      //## Operation: ClCCService%882789765; C++
      ClCCService (m4pchar_t ai_szServiceId, ClCCSession *ai_pSession);

    //## Destructor (specified)
      //## Operation: ~ClCCService%881313195
      ~ClCCService ();


    //## Other Operations (specified)
      //## Operation: GetSession%881685998; C++
      //	.
      //	Devuelve un puntero a la conexión que corresponde a este servicio.
      //	.
      m4return_t GetSession (ClCCSession *&ao_pSession);

      //## Operation: GetRequest%881833854; C++
      //	.
      //	El servicio contiene un mapa con las peticiones pendientes. Este método
      //	recibe como parámetro un identificador y devuelve un puntero a la request
      //	correspondiente.
      //	.
      m4return_t GetRequest (m4handle_t ai_RequestId, ClCCRequest *&ao_pRequest);

      //## Operation: AsyncRequest%884258324; C++
      m4return_t AsyncRequest (M4DataStorage& ai_oInputData, 	// DataStorage con los datos que deben enviarse al servidor.
      m4handle_t& ao_hRID, 	// Identificador de la request. Se utilizará en el método Receive, para
      	// indicar cual es la petición de la que se desea obtener la respuesta.
      m4bool_t ai_bLogInfo, m4uint16_t ai_uiRequestType = M4_ID_PDU_REQUEST, 	// Representa el tipo de PDU que  hay que enviar al servidor con esos datos
      	// (puede tratarse, por ejemplo, de una pdu de request o de administracion).
      	// Por defecto se construye una pdu de request.
      m4bool_t ai_bOwner = M4_TRUE	// Este parámetro indica si el cliente de comunicaciones se hace propietario
      	// del datastorage o no. Por defecto es que si, eso significa que el cliente
      	// del cliente de comunicaciones que invoca el método no debe destruírlo, y
      	// que debe hacerse una copia local antes de invocar al método si es que va a
      	// utilizarlo más tarde.
      );

      //## Operation: SyncRequest%884258325; C++
      m4return_t SyncRequest (M4DataStorage& ai_oInputData, M4DataStorage& ao_oOutputData, m4handle_t& ao_hRID, m4bool_t ai_bLogInfo, ClLogSystem *ai_poLog, ClRequestDescriptor *ao_poUniqueId, m4uint32_t ai_uiTimeout = 0, m4uint16_t ai_uiRequestType = M4_ID_PDU_REQUEST, m4bool_t ai_bOwner = M4_TRUE, 	// Este parámetro indica si el cliente de comunicaciones se hace propietario
      	// del datastorage o no. Por defecto es que si, eso significa que el cliente
      	// del cliente de comunicaciones que invoca el método no debe destruírlo, y
      	// que debe hacerse una copia local antes de invocar al método si es que va a
      	// utilizarlo más tarde.
      ClTimeoutFunction ai_pTimeoutFunction = NULL);

      //## Operation: Receive%884258326
      m4return_t Receive (m4int32_t ai_RequestId, M4DataStorage& ao_oOutputData, m4int32_t ai_lTimeOut, m4int16_t ai_lReceiveFlag, ClLogSystem *ai_poLog = NULL);

      //## Operation: GetService%884854699
      m4pchar_t GetService () const;

      //## Operation: RemoveRequest%899225540
      m4return_t RemoveRequest (m4handle_t ai_RequestId);

      //## Operation: CancelRequest%884854700; C++
      m4return_t CancelRequest (m4handle_t ai_RequestId, m4uint32_t ai_uiTimeout, m4bool_t ai_bSeverity);

      //## Operation: NotifyAllRequest%923902840; C++
      m4return_t NotifyAllRequest (eRequestStatus ai_eIncidence);

      //## Operation: GetDataStorage%927714163; C++
      void GetDataStorage (ClPDU*  ai_poPDU, M4DataStorage& ao_oData);

      //## Operation: _GetAnswerData%950783985; C++
      m4return_t _GetAnswerData (ClCCRequest *ai_pRequest, m4bool_t &ao_bReSend);

      //## Operation: _SendData%950783986; C++
      m4return_t _SendData (ClCCRequest *ai_pRequest);

  protected:

    //## Other Operations (specified)
      //## Operation: _PrepareRequest%947519641; C++
      m4return_t _PrepareRequest (m4uint32_t &aio_uiTimeout, ClCCRequest * &ao_pRequest, m4handle_t& ao_hRID);

      //## Operation: CancelAllRequests%884854702
      m4return_t CancelAllRequests ();

      //## Operation: CreateCancelRequestPDU%898788004; C++
      m4return_t CreateCancelRequestPDU (m4handle_t ai_hToBeCancelID, ClCCRequest *ai_pRequest);

      //## Operation: FullRemoveRequest%947582395
      m4return_t FullRemoveRequest (m4handle_t ai_RequestId);

      //## Operation: _GetAnswer%947582396; C++
      m4return_t _GetAnswer (ClCCRequest *ai_poRequest, M4DataStorage *ai_poOutputDataStorage = NULL, m4pchar_t ai_szRequestName = NULL, m4pchar_t ai_szDefaultError = NULL, ClTimeoutFunction ai_pTimeoutFunction = NULL, ClTagList *ao_oTag = NULL);

      //## Operation: _CreatePDU%948190723; C++
      m4return_t _CreatePDU (m4int16_t ai_iPDUType, ClCCRequest *ai_poRequest, ClPDU * &ao_pPDU);

    // Data Members for Class Attributes

      //## Attribute: m_MutexDicRequests%3563F3520271
      //## begin ClCCService::m_MutexDicRequests%3563F3520271.attr preserve=no  protected: ClMutex {UA} 
      ClMutex m_MutexDicRequests;
      //## end ClCCService::m_MutexDicRequests%3563F3520271.attr

    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%3487DD0D0377
      //## Role: ClCCService::m_dicRequests%3487DD0E0350
      //## Qualifier: hRequest%35094C0C036A; m4handle_t
      //## begin ClCCService::m_dicRequests%3487DD0E0350.role preserve=no  protected: ClCCRequest {1 -> 1RHAN}
      map<m4int32_t, ClCCRequest*, less<m4int32_t> > m_dicRequests;
      //## end ClCCService::m_dicRequests%3487DD0E0350.role

      //## Association: M4AppClient::<unnamed>%34E44396007D
      //## Role: ClCCService::m_pSession%34E44396007E
      //## begin ClCCService::m_pSession%34E44396007E.role preserve=no  protected: ClCCSession {1 -> 1RFHAN}
      ClCCSession *m_pSession;
      //## end ClCCService::m_pSession%34E44396007E.role

  private:

    //## Other Operations (specified)
      //## Operation: CreateRequestPDU%884258327; C++
      //	.
      //	Se trata de un método privado, que va a ser invocado desde los métodos Async
      //	Request y SyncRequest, para construir una PDU del tipo adecuado a partir de
      //	los datos que recibe como parámetro. En este método se rellena la cabecera
      //	de la PDU y se le añaden los bloques de datos del DataStorage.
      //	.
      m4return_t CreateRequestPDU (M4DataStorage& ai_oInputData, m4uint16_t ai_uiRequestType, m4bool_t ai_bOwner, 	// Indica si la pdu que se va a construir va a ser propietaria o no del data
      	// Storage.
      ClCCRequest *ai_pRequest);

    // Data Members for Class Attributes

      //## Attribute: m_szServiceId%34BDDEA5000A
      //## begin ClCCService::m_szServiceId%34BDDEA5000A.attr preserve=no  private: m4pchar_t {UA} 
      m4pchar_t m_szServiceId;
      //## end ClCCService::m_szServiceId%34BDDEA5000A.attr

    // Additional Private Declarations
      //## begin ClCCService%3487D87D00E3.private preserve=yes
	//  void GetDataStorage(ClPDU* ai_poPDU, M4DataStorage& ao_oData);
      //## end ClCCService%3487D87D00E3.private
  private: //## implementation
};

//## begin ClCCAdminService%34CCD67A002B.preface preserve=yes
//## end ClCCAdminService%34CCD67A002B.preface

//## Class: ClCCAdminService%34CCD67A002B
//	.
//	La clase ClCCAdminService es una especialización de la clase ClCCService.
//	Será el tipo de servicio que se utilice en las conexiones del administrador.
//	Define los métodos AsyncRequest, SyncRequest y Receive para ofrecer la
//	posibilidad de pasar punteros a PDU en lugar de DataStorages. Aunque en
//	general se evita que el cliente del cliente de comunicaciones deba manejar
//	PDU´s, en el caso del administrador resulta más ventajoso de esta forma.
//	.
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%359777C3003A;ClTagList { -> }
//## Uses: <unnamed>%37986CC90315;ClConnectPDU { -> }
//## Uses: <unnamed>%37986CCC02E7;ClAckPDU { -> }
//## Uses: <unnamed>%37986D13027B;ClCCSession { -> }
//## Uses: <unnamed>%37986D3D0307;ClProtocol { -> }
//## Uses: <unnamed>%37986DEB01D1;ClCCUser { -> }

class ClCCAdminService : public ClCCService  //## Inherits: <unnamed>%34CCD6A203E0
{
  //## begin ClCCAdminService%34CCD67A002B.initialDeclarations preserve=yes
  //## end ClCCAdminService%34CCD67A002B.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCCAdminService%885839988; C++
      ClCCAdminService (m4pchar_t ai_szServiceId, ClCCSession *ai_pSession);

    //## Destructor (specified)
      //## Operation: ~ClCCAdminService%885839989
      ~ClCCAdminService ();


    //## Other Operations (specified)
      //## Operation: ConnectRequest%885839990; C++
      m4return_t ConnectRequest (m4handle_t& ao_hRID, ClTagList *ao_oTag, m4uint32_t ai_uiTimeout, ClTimeoutFunction ai_pTimeoutFunction = NULL);

      //## Operation: DisconnectRequest%890127474; C++
      m4return_t DisconnectRequest (m4handle_t& ao_hRID, m4uint32_t ai_uiTimeout, ClTimeoutFunction ai_pTimeoutFunction = NULL);

      //## Operation: SetRoleRequest%890127475; C++
      m4return_t SetRoleRequest (m4handle_t& ao_hRID, m4pchar_t ai_pRoleId, m4bool_t ai_bIsDef, m4uint32_t ai_uiTimeout, ClTimeoutFunction ai_pTimeoutFunction = NULL);

      //## Operation: ChangePasswordRequest%898848503
      m4return_t ChangePasswordRequest (m4handle_t &ao_hRID, M4ClString &ai_oOldPasswd, M4ClString &ai_oNewPasswd, m4uint32_t ai_uiTimeout, ClTimeoutFunction ai_pTimeoutFunction = NULL);

	  m4return_t PingRequest (m4handle_t& ao_hRID, m4uint32_t ai_uiElapsedTime, m4uint32_t ai_uiTimeout, ClTimeoutFunction ai_pTimeoutFunction = NULL);

      //## Operation: AuthenticateRequest%899807018; C++
//      m4return_t AuthenticateRequest (m4handle_t& ao_hRID, ClTagList *ao_oTag = NULL, m4bool_t ai_bDefaultRole = M4_FALSE, m4uint32_t ai_uiTimeout = TIMEOUT_ADMIN_SERV, ClTimeoutFunction ai_pTimeoutFunction = NULL, m4uint16_t ai_iFlags = 0);

    // Additional Public Declarations
      //## begin ClCCAdminService%34CCD67A002B.public preserve=yes
      m4return_t AuthenticateRequest (m4handle_t& ao_hRID, m4pchar_t &ao_strX509Cert, m4uint32_t & aoi_iX509CertBufLen,  m4pchar_t & ao_strSessionContext, m4uint32_t & aoi_iSessContextBufLen, ClTagList *ao_oTag = NULL, m4bool_t ai_bDefaultRole = M4_FALSE, m4uint32_t ai_uiTimeout = TIMEOUT_ADMIN_SERV, ClTimeoutFunction ai_pTimeoutFunction = NULL, m4uint16_t ai_iFlags = 0);

      m4return_t AuthenticateRequest (m4handle_t& ao_hRID, m4pchar_t ai_strX509Cert, m4uint32_t ai_iX509CertLen,  m4pchar_t ai_strSessionContext , m4pchar_t & ao_strX509Cert, m4uint32_t & aoi_iX509CertBufLen,  m4pchar_t & ao_strSessionContext, m4uint32_t & aoi_iSessContextBufLen, ClTagList *ao_oTag = NULL, m4bool_t ai_bDefaultRole = M4_FALSE, m4uint32_t ai_uiTimeout = TIMEOUT_ADMIN_SERV, ClTimeoutFunction ai_pTimeoutFunction = NULL, m4uint16_t ai_iFlags = 0);
      //## end ClCCAdminService%34CCD67A002B.public

  protected:

    //## Other Operations (specified)
      //## Operation: CreateAuthPDU%889606015; C++
      //	.
      //	Se trata de un método privado, que va a ser invocado desde los métodos Async
      //	Request y SyncRequest, para construir una PDU del tipo adecuado a partir de
      //	los datos que recibe como parámetro. En este método se rellena la cabecera
      //	de la PDU y se le añaden los bloques de datos del DataStorage.
      //	.
      m4return_t CreateAuthPDU (ClCCRequest *ai_poRequest, m4bool_t ai_bDefaultRole = M4_FALSE, ClTagList *ao_oTag = NULL , m4uint16_t ai_iFlags = 0);

      //## Operation: CreateDisconnectPDU%889606016; C++
      //	.
      //	Se trata de un método privado, que va a ser invocado desde los métodos Async
      //	Request y SyncRequest, para construir una PDU del tipo adecuado a partir de
      //	los datos que recibe como parámetro. En este método se rellena la cabecera
      //	de la PDU y se le añaden los bloques de datos del DataStorage.
      //	.
      m4return_t CreateDisconnectPDU (ClCCRequest *ai_poRequest);

      //## Operation: CreateSetRolePDU%890127477; C++
      //	.
      //	Se trata de un método privado, que va a ser invocado desde los métodos Async
      //	Request y SyncRequest, para construir una PDU del tipo adecuado a partir de
      //	los datos que recibe como parámetro. En este método se rellena la cabecera
      //	de la PDU y se le añaden los bloques de datos del DataStorage.
      //	.
      m4return_t CreateSetRolePDU (ClCCRequest *ai_poRequest, m4pchar_t ai_pRoleId, m4bool_t ai_bIsDef);

      //## Operation: CreateChangePasswordPDU%898848504; C++
      m4return_t CreateChangePasswordPDU (ClCCRequest *ai_poRequest, M4ClString &ai_oOldPasswd, M4ClString &ai_oNewPasswd);

	  m4return_t CreatePingPDU (ClCCRequest *ai_poRequest, m4uint32_t ai_uiElapsedTime);

      //## Operation: CreateConnectPDU%899807019; C++
      m4return_t CreateConnectPDU (ClCCRequest *ai_poRequest, ClTagList *ao_oTag);

    // Additional Protected Declarations
      //## begin ClCCAdminService%34CCD67A002B.protected preserve=yes
      //## end ClCCAdminService%34CCD67A002B.protected

  private:
    // Additional Private Declarations
      //## begin ClCCAdminService%34CCD67A002B.private preserve=yes
      //## end ClCCAdminService%34CCD67A002B.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCCAdminService%34CCD67A002B.implementation preserve=yes
      //## end ClCCAdminService%34CCD67A002B.implementation

};

//## begin ClCCAdminService%34CCD67A002B.postscript preserve=yes
//## end ClCCAdminService%34CCD67A002B.postscript

// Class ClCCService 

// Class ClCCAdminService 

//## begin module%34951232028B.epilog preserve=yes
//## end module%34951232028B.epilog


#endif

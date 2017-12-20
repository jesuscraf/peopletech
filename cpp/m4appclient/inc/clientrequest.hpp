//## begin module%34795AAF0395.cm preserve=no
//## end module%34795AAF0395.cm

//## begin module%34795AAF0395.cp preserve=yes
//	=========================================================
//	=====================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              ClientRequest.hpp
//	 Project:           M4ComSrv
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
//	=========================================================
//	=====================
//## end module%34795AAF0395.cp

//## Module: clientrequest%34795AAF0395; Package specification
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Source file: C:\m4server\m4appclient\inc\clientrequest.hpp

#ifndef clientrequest_h
#define clientrequest_h 1

//## begin module%34795AAF0395.additionalIncludes preserve=no
//## end module%34795AAF0395.additionalIncludes

//## begin module%34795AAF0395.includes preserve=yes
#include <m4condition.hpp>
#include "m4stl.hpp"
//## end module%34795AAF0395.includes

// BaseRequest
#include <baserequest.hpp>
#include "m4appclient_dll.hpp"

class ClCCService;

//## begin module%34795AAF0395.declarations preserve=no
//## end module%34795AAF0395.declarations

//## begin module%34795AAF0395.additionalDeclarations preserve=yes
//## end module%34795AAF0395.additionalDeclarations


//## begin eRequestStatus%38AD487D0076.preface preserve=yes
//## end eRequestStatus%38AD487D0076.preface

//## Class: eRequestStatus%38AD487D0076
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef enum {BUILDING = 1, SENDING, WAITING_FOR_REPLY, REPLY_ARRIVED, ERROR_ARRIVED , CANCELING , CANCELED,TIMEOUT,BROKEN_CONNECTION,HTTP_TIMEOUT,NO_SERVER,RECEIVING,HTTP_ERROR} eRequestStatus;

//## begin eRequestStatus%38AD487D0076.postscript preserve=yes
//## end eRequestStatus%38AD487D0076.postscript

//## Class: ClCCRequest%344738D402FC
//	.
//	Esta clase hereda toda la funcionalidad de las peticiones en el servidor
//	pero además le tenemos que dar una funcionalidad en el cliente
//
//	Más exactamente, esta clase heredará de la misma clase
//	que el Request del servidor. Esta clase padre tendrá la
//	funcionalidad común de ambas.
//
//	Parece que esta objeto el que llevará el control de las peticiones que lleva
//	bloqueada o no.
//	Si tiene semaforo es que le está bloqueando alguien , un thread del cliente.
//	.
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%380F1E0D0060;stdXXostream { -> }
//## Uses: <unnamed>%380F3343005E;basic_ofstream { -> }
//## Uses: <unnamed>%38B108C10150;eRequestStatus { -> }

class M4_DECL_M4APPCLIENT ClCCRequest : public ClServiceRequest  //## Inherits: <unnamed>%3553243D0123
{
  public:
    //## Constructors (specified)
      //## Operation: ClCCRequest%880965772; C++
      ClCCRequest (ClCCService* ai_pService);

    //## Destructor (specified)
      //## Operation: ~ClCCRequest%881084622
      ~ClCCRequest ();


    //## Other Operations (specified)
      //## Operation: GetService%884854701; C++
      //	.
      //	Devuelve un puntero al servicio que la creo.
      //	.
      m4return_t GetService (ClCCService *&ao_pService);

      //## Operation: GetState%881751289
      eRequestStatus GetState ();

      //## Operation: SetState%881751290; C++
      void SetState (eRequestStatus ai_iState);

      //## Operation: GetCondition%907327031; C++
      ClCondition * GetCondition ();

      //## Operation: SetCondition%907327032; C++
      void SetCondition (ClCondition *pCondition);

      //## Operation: UnsetCondition%907327033; C++
      m4return_t UnsetCondition ();

      //## Operation: Recycle%942252013; C++
      //	Returns M4_ERROR if CanRecycle() != M4_TRUE, M4_SUCCESS if Recycle is
      //	completed. Other return values may be defined.
      m4return_t Recycle ();

      //## Operation: CanRecycle%942252014
      //	Returns M4_TRUE if the object can be recycled, M4_FALSE otherwise.
      m4bool_t CanRecycle ();

      //## Operation: Destroy%942252015
      //	Returns M4_ERROR if CanDestroy() != M4_TRUE, M4_SUCCESS if Destroy is
      //	completed. Other return Values may be defined.
      m4return_t Destroy ();

      //## Operation: CanDestroy%942252016
      //	Returns M4_TRUE if the object may be destroyed, M4_FALSE otherwise.
      m4bool_t CanDestroy ();

    // Data Members for Class Attributes

      //## Attribute: m_iState%3447405101A4
      //	.
      //	Estado en el que puede encontrarse una petición:
      //	BUILDING -> Cuando se está construyendo.
      //	SENDING -> Cuando se está mandando
      //	WAITING_FOR_REPLAY -> Esperando la respuesta.
      //	.
      //## begin ClCCRequest::m_iState%3447405101A4.attr preserve=no  public: eRequestStatus {UA} 
      eRequestStatus m_iState;
      //## end ClCCRequest::m_iState%3447405101A4.attr

      //## Attribute: m_poDebugFile%35B3218501F9
      //## begin ClCCRequest::m_poDebugFile%35B3218501F9.attr preserve=no  public: fstream * {UA} NULL
      fstream *m_poDebugFile;
      //## end ClCCRequest::m_poDebugFile%35B3218501F9.attr

      //## Attribute: m_uiTimeout%36FA6954012B
      //## begin ClCCRequest::m_uiTimeout%36FA6954012B.attr preserve=no  public: m4uint32_t {UA} TIMEOUT_ADMIN_SERV
      m4uint32_t m_uiTimeout;
      //## end ClCCRequest::m_uiTimeout%36FA6954012B.attr

      //## Attribute: m_bRetrying%45EECDEF00FB
      //## begin ClCCRequest::m_bRetrying%45EECDEF00FB.attr preserve=no  public: m4bool_t {UA} 
      m4bool_t m_bRetrying;
      //## end ClCCRequest::m_bRetrying%45EECDEF00FB.attr

  protected:
    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%3487DD0D0377
      //## Role: ClCCRequest::m_pService%3487DD0E031E
      //## begin ClCCRequest::m_pService%3487DD0E031E.role preserve=no  protected: ClCCService {1 -> 1RFHGAN}
      ClCCService *m_pService;
      //## end ClCCRequest::m_pService%3487DD0E031E.role

  private:
  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCCRequest%344738D402FC.implementation preserve=yes
	  ClCondition* m_poCondition;
      //## end ClCCRequest%344738D402FC.implementation
};

// Class ClCCRequest 


//## Other Operations (inline)
inline m4return_t ClCCRequest::Recycle ()
{
  //## begin ClCCRequest::Recycle%942252013.body preserve=yes
	return M4_ERROR ;
  //## end ClCCRequest::Recycle%942252013.body
}

inline m4bool_t ClCCRequest::CanRecycle ()
{
  //## begin ClCCRequest::CanRecycle%942252014.body preserve=yes
	return M4_FALSE ;
  //## end ClCCRequest::CanRecycle%942252014.body
}

inline m4return_t ClCCRequest::Destroy ()
{
  //## begin ClCCRequest::Destroy%942252015.body preserve=yes
	return M4_ERROR ;
  //## end ClCCRequest::Destroy%942252015.body
}

inline m4bool_t ClCCRequest::CanDestroy ()
{
  //## begin ClCCRequest::CanDestroy%942252016.body preserve=yes
	return M4_FALSE ;
  //## end ClCCRequest::CanDestroy%942252016.body
}

//## begin module%34795AAF0395.epilog preserve=yes
//## end module%34795AAF0395.epilog


#endif

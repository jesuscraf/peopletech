//## begin module%38D20096038E.cm preserve=no
//## end module%38D20096038E.cm

//## begin module%38D20096038E.cp preserve=no
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
//## end module%38D20096038E.cp

//## Module: CommunicationLayer%38D20096038E; Package specification
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Source file: e:\mybuild\m4communication\inc\communicationlayer.hpp

#ifndef communicationlayer_h
#define communicationlayer_h 1

//## begin module%38D20096038E.additionalIncludes preserve=no
//## end module%38D20096038E.additionalIncludes

//## begin module%38D20096038E.includes preserve=yes
//## end module%38D20096038E.includes

// CriticalSection
#include <criticalsection.hpp>
// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
// stl
#include "m4stl.hpp"
// commsocketpool
#include <commsocketpool.hpp>
// CommBin
#include <commbin.hpp>
// commprotocols
#include <commprotocols.hpp>
// CommAction
#include <commaction.hpp>
// CommTask
#include <commtask.hpp>
// TransportLayerSynchronous
#include <transportlayersynchronous.hpp>
// CommComun
#include <commcomun.hpp>

class ClCommWorker;

//## begin module%38D20096038E.declarations preserve=no
//## end module%38D20096038E.declarations

//## begin module%38D20096038E.additionalDeclarations preserve=yes

//## end module%38D20096038E.additionalDeclarations


//## begin ClNetResource%390FF4FF0129.preface preserve=yes
//## end ClNetResource%390FF4FF0129.preface

//## Class: ClNetResource%390FF4FF0129
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClNetResource 
{
  //## begin ClNetResource%390FF4FF0129.initialDeclarations preserve=yes
  //## end ClNetResource%390FF4FF0129.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClNetResource%958548777
      ClNetResource ();

    //## Destructor (specified)
      //## Operation: ~ClNetResource%958548778
      ~ClNetResource ();

    // Data Members for Associations

      //## Association: M4Communication::M4COML::<unnamed>%390FF55301B6
      //## Role: ClNetResource::m_TLIid%390FF5530350
      //## begin ClNetResource::m_TLIid%390FF5530350.role preserve=no  public: m4tliid_t { -> 1VHAN}
      m4tliid_t m_TLIid;
      //## end ClNetResource::m_TLIid%390FF5530350.role

      //## Association: M4Communication::M4COML::<unnamed>%390FF57B01A9
      //## Role: ClNetResource::m_CommProtocol%390FF57B03B2
      //## begin ClNetResource::m_CommProtocol%390FF57B03B2.role preserve=no  public: ClCommProtocol { -> 1RHAN}
      ClCommProtocol *m_CommProtocol;
      //## end ClNetResource::m_CommProtocol%390FF57B03B2.role

      //## Association: M4Communication::M4COML::<unnamed>%3922AEB30349
      //## Role: ClNetResource::m_ReadingTask%3922AEB40228
      //## begin ClNetResource::m_ReadingTask%3922AEB40228.role preserve=no  public: ClCommReadingTask { -> RHAN}
      ClCommReadingTask *m_ReadingTask;
      //## end ClNetResource::m_ReadingTask%3922AEB40228.role

      //## Association: M4Communication::M4COML::<unnamed>%392A930F038F
      //## Role: ClNetResource::m_strURL%392A9310025A
      //## begin ClNetResource::m_strURL%392A9310025A.role preserve=no  public: m4string_t { -> 1VHAN}
      m4string_t m_strURL;
      //## end ClNetResource::m_strURL%392A9310025A.role

      //## Association: M4Communication::M4COML::<unnamed>%39910ECF019F
      //## Role: ClNetResource::m_WrittingTask%39910ED00092
      //## begin ClNetResource::m_WrittingTask%39910ED00092.role preserve=no  public: ClCommWrittingTask { -> RHAN}
      ClCommWrittingTask *m_WrittingTask;
      //## end ClNetResource::m_WrittingTask%39910ED00092.role

    // Additional Public Declarations
      //## begin ClNetResource%390FF4FF0129.public preserve=yes
      //## end ClNetResource%390FF4FF0129.public

  protected:
    // Additional Protected Declarations
      //## begin ClNetResource%390FF4FF0129.protected preserve=yes
      //## end ClNetResource%390FF4FF0129.protected

  private:
    // Additional Private Declarations
      //## begin ClNetResource%390FF4FF0129.private preserve=yes
      //## end ClNetResource%390FF4FF0129.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClNetResource%390FF4FF0129.implementation preserve=yes
      //## end ClNetResource%390FF4FF0129.implementation

};

//## begin ClNetResource%390FF4FF0129.postscript preserve=yes
//## end ClNetResource%390FF4FF0129.postscript

//## begin ClTLIMap%39084A05036F.preface preserve=yes
//## end ClTLIMap%39084A05036F.preface

//## Class: ClTLIMap%39084A05036F; Instantiated Class
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%390FF62F03D9;ClNetResource { -> }

typedef map< m4handle_t,ClNetResource *,less<m4handle_t> > ClTLIMap;

//## begin ClTLIMap%39084A05036F.postscript preserve=yes
//## end ClTLIMap%39084A05036F.postscript

//## begin AcceptedPorts%392A5D1203A9.preface preserve=yes
//## end AcceptedPorts%392A5D1203A9.preface

//## Class: AcceptedPorts%392A5D1203A9; Instantiated Class
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef map< m4int_t,ClCommTask *,less<int> > AcceptedPorts;

//## begin AcceptedPorts%392A5D1203A9.postscript preserve=yes
//## end AcceptedPorts%392A5D1203A9.postscript

//## begin WorkerMap%39AF5C2A03E3.preface preserve=yes
//## end WorkerMap%39AF5C2A03E3.preface

//## Class: WorkerMap%39AF5C2A03E3; Instantiated Class
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef map< m4int_t,ClCommWorker *,less<m4int_t> > WorkerMap;

//## begin WorkerMap%39AF5C2A03E3.postscript preserve=yes
//## end WorkerMap%39AF5C2A03E3.postscript

//## begin ClCommunicationImplementation%38DB2BE001D1.preface preserve=yes
//## end ClCommunicationImplementation%38DB2BE001D1.preface

//## Class: ClCommunicationImplementation%38DB2BE001D1
//	Contiene toda la logica de la capa.
//	Arrancará los worker que tenga.
//	Falta un array de workers (para poder pararlos y destruirlos) y el array de
//	los
//	TransportLayerIdentifiers que está procesando cada uno de ellos para evitar
//	que dos worker procesen acciones de uno mismo,
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39490B3B00D8;ClBlockSync { -> F}

class ClCommunicationImplementation 
{
  //## begin ClCommunicationImplementation%38DB2BE001D1.initialDeclarations preserve=yes
  //## end ClCommunicationImplementation%38DB2BE001D1.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommunicationImplementation%955103950; C++
      ClCommunicationImplementation (m4int_t ai_iControlPort, m4int_t ai_iNWorker);

    //## Destructor (specified)
      //## Operation: ~ClCommunicationImplementation%955103951
      ~ClCommunicationImplementation ();


    //## Other Operations (specified)
      //## Operation: Push%955537528; C++
      m4return_t Push (ClCommAction *ai_Action);

      //## Operation: GetTLIID%956910728; C++
      m4tliid_t GetTLIID (m4handle_t ai_hConnectionID);

      //## Operation: AddConnectionID%956910729; C++
      m4return_t AddConnectionID (m4handle_t ai_hConnectionID, m4tliid_t ai_TLIID, ClCommProtocol *ai_pCommProtocol, m4string_t ai_strURL = "");

      //## Operation: VerifyConnection%956915208; C++
      m4return_t VerifyConnection (m4handle_t ai_hConectionID, ClParamList * &ao_pParamList, ClCommAction *ai_poAction, m4bool_t ai_bNoError = M4_FALSE);

      //## Operation: GetProtocol%958483734; C++
      ClCommProtocol * GetProtocol (m4handle_t ai_hConnectionID);

      //## Operation: GetInstance%955613890
      static ClCommunicationImplementation * GetInstance ();

      //## Operation: GetTLIandProtocol%958483735; C++
      m4return_t GetTLIandProtocol (m4handle_t ai_hConnectionID, m4tliid_t &ao_TLIID, ClCommProtocol * &ao_pProtocol);

      //## Operation: RemoveConnectionID%958548776; C++
      m4return_t RemoveConnectionID (m4handle_t ai_hConnectionID);

      //## Operation: AddReadingTask%958548780; C++
      m4return_t AddReadingTask (m4handle_t ai_hConnectionID, ClCommReadingTask *ai_pReadingTask);

      //## Operation: GetReadingTask%958548781; C++
      m4return_t GetReadingTask (m4handle_t ai_hConnectionID, ClCommReadingTask * &ao_pReadingTask);

      //## Operation: StopCommunicationLayer%958646098
      m4return_t StopCommunicationLayer ();

      //## Operation: AddConnectionID%958981534; C++
      m4return_t AddConnectionID (m4handle_t ai_hConnectionID, m4tliid_t ai_TLIID, m4int_t ai_iCommProtocolType, m4string_t ai_strURL = "");

      //## Operation: AddPort%959072021; C++
      m4return_t AddPort (m4int_t ai_iPort, ClCommTask *ai_poTask);

      //## Operation: RemovePort%959072022; C++
      m4return_t RemovePort (m4int_t ai_iPort);

      //## Operation: IsPortInUse%959072023; C++
      m4return_t IsPortInUse (m4int_t ai_iPort);

      //## Operation: GetPortTask%959072024; C++
      ClCommTask * GetPortTask (m4int_t ai_iPort);

      //## Operation: GetURL%959072026; C++
      m4return_t GetURL (m4handle_t ai_hConnectionID, m4string_t& ao_strURL);

      //## Operation: GetError%959072027
      m4int_t GetError ();

      //## Operation: GetCTL%964095362
      ClTransportLayerSynchronous & GetCTL ();

      //## Operation: GetWorkerStats%964423558; C++
      m4return_t GetWorkerStats (m4int_t ai_iWorkerID, m4uint_t &ao_uiExecutedActions, m4uint_t &ao_uiExecutingTime, m4uint_t &ao_uiWaitingTime, m4uint_t &ao_uiExternalExecutingTime);

      //## Operation: AddWrittingTask%965806163; C++
      m4return_t AddWrittingTask (m4handle_t ai_hConnectionID, ClCommWrittingTask *ai_pWrittingTask);

      //## Operation: GetWrittingTask%965806164; C++
      m4return_t GetWrittingTask (m4handle_t ai_hConnectionID, ClCommWrittingTask * &ao_pWrittingTask);

      //## Operation: AddWorker%967794005
      m4return_t AddWorker ();

      //## Operation: RemoveWorker%967794006
      m4return_t RemoveWorker ();

      //## Operation: GetNWorker%967794010
      m4int_t GetNWorker ();

      //## Operation: GetNPendingActions%967794015
      m4int_t GetNPendingActions ();

      //## Operation: SetNSocketPool%971164984
      m4return_t SetNSocketPool (m4string_t &ai_strURL, m4int_t ai_iNSocket);

      //## Operation: SetProtocolVersion%973247122; C++
      m4return_t SetProtocolVersion (m4handle_t ai_hConnectionID, m4int_t ai_iProtocolVersion);

      //## Operation: GetURLAndHostName%977486321; C++
      m4return_t GetURLAndHostName (m4handle_t ai_hConnectionID, m4string_t& ao_strURL, m4string_t &ao_strHostName);

      //## Operation: IsShuttingDown%988885058
      m4bool_t IsShuttingDown ();

    // Data Members for Associations

      //## Association: M4Communication::M4COML::<unnamed>%399176CC0068
      //## Role: ClCommunicationImplementation::m_oBin%399176CC032F
      //## begin ClCommunicationImplementation::m_oBin%399176CC032F.role preserve=no  public: ClCommBin { -> VHAN}
      ClCommBin m_oBin;
      //## end ClCommunicationImplementation::m_oBin%399176CC032F.role

    // Additional Public Declarations
      //## begin ClCommunicationImplementation%38DB2BE001D1.public preserve=yes
	  m4bool_t IsShuttedDown ();
      //## end ClCommunicationImplementation%38DB2BE001D1.public

  protected:
    // Data Members for Associations

      //## Association: M4Communication::M4COML::<unnamed>%38D791840078
      //## Role: ClCommunicationImplementation::m_CTL%38D7918402BD
      //## begin ClCommunicationImplementation::m_CTL%38D7918402BD.role preserve=no  protected: ClTransportLayerSynchronous { -> VHAN}
      ClTransportLayerSynchronous m_CTL;
      //## end ClCommunicationImplementation::m_CTL%38D7918402BD.role

      //## Association: M4Communication::M4COML::<unnamed>%38D9EE5002CB
      //## Role: ClCommunicationImplementation::m_ActionQueue%38D9EE5202CD
      //## begin ClCommunicationImplementation::m_ActionQueue%38D9EE5202CD.role preserve=no  protected: ClActionQueue { -> VHAN}
      ClActionQueue m_ActionQueue;
      //## end ClCommunicationImplementation::m_ActionQueue%38D9EE5202CD.role

      //## Association: M4Communication::M4COML::<unnamed>%38F5EBD10134
      //## Role: ClCommunicationImplementation::m_poImplementation%38F5EBD5020D
      //## begin ClCommunicationImplementation::m_poImplementation%38F5EBD5020D.role preserve=no  protected: static ClCommunicationImplementation {1 -> 1RHAN}
      static ClCommunicationImplementation *m_poImplementation;
      //## end ClCommunicationImplementation::m_poImplementation%38F5EBD5020D.role

      //## Association: M4Communication::M4COML::<unnamed>%39094D30030E
      //## Role: ClCommunicationImplementation::m_TLIMap%39094D3102C9
      //## begin ClCommunicationImplementation::m_TLIMap%39094D3102C9.role preserve=no  protected: ClTLIMap { -> 1VHAN}
      ClTLIMap m_TLIMap;
      //## end ClCommunicationImplementation::m_TLIMap%39094D3102C9.role

      //## Association: M4Communication::M4COML::<unnamed>%39180DA90255
      //## Role: ClCommunicationImplementation::m_MapMutex%39180DAA0333
      //## begin ClCommunicationImplementation::m_MapMutex%39180DAA0333.role preserve=no  protected: ClCriticalSection { -> 1VHAN}
      ClCriticalSection m_MapMutex;
      //## end ClCommunicationImplementation::m_MapMutex%39180DAA0333.role

      //## Association: M4Communication::M4COML::<unnamed>%392A5D2C03B1
      //## Role: ClCommunicationImplementation::m_AcceptedPortsMap%392A5D2D02EA
      //## begin ClCommunicationImplementation::m_AcceptedPortsMap%392A5D2D02EA.role preserve=no  protected: AcceptedPorts { -> VHAN}
      AcceptedPorts m_AcceptedPortsMap;
      //## end ClCommunicationImplementation::m_AcceptedPortsMap%392A5D2D02EA.role

      //## Association: M4Communication::M4COML::<unnamed>%392AA0B401F7
      //## Role: ClCommunicationImplementation::m_iFatalError%392AA0B501B2
      //## begin ClCommunicationImplementation::m_iFatalError%392AA0B501B2.role preserve=no  protected: m4int_t { -> VHAN}
      m4int_t m_iFatalError;
      //## end ClCommunicationImplementation::m_iFatalError%392AA0B501B2.role

      //## Association: M4Communication::M4COML::<unnamed>%3993ACF40054
      //## Role: ClCommunicationImplementation::m_poWorker%3993ACF401BC
      //## begin ClCommunicationImplementation::m_poWorker%3993ACF401BC.role preserve=no  protected: ClCommWorker { -> RFHAN}
      ClCommWorker *m_poWorker;
      //## end ClCommunicationImplementation::m_poWorker%3993ACF401BC.role

      //## Association: M4Communication::M4COML::<unnamed>%39AF5C8500D7
      //## Role: ClCommunicationImplementation::m_WorkerPool%39AF5C860146
      //## begin ClCommunicationImplementation::m_WorkerPool%39AF5C860146.role preserve=no  protected: WorkerMap { -> VHAN}
      WorkerMap m_WorkerPool;
      //## end ClCommunicationImplementation::m_WorkerPool%39AF5C860146.role

      //## Association: M4Communication::M4COML::<unnamed>%39AF5D7E0157
      //## Role: ClCommunicationImplementation::m_iNWorker%39AF5D7E03B0
      //## begin ClCommunicationImplementation::m_iNWorker%39AF5D7E03B0.role preserve=no  protected: m4int_t { -> VHAN}
      m4int_t m_iNWorker;
      //## end ClCommunicationImplementation::m_iNWorker%39AF5D7E03B0.role

      //## Association: M4Communication::M4COML::<unnamed>%3A2273B203A2
      //## Role: ClCommunicationImplementation::m_SocketPool%3A2273B3029F
      //## begin ClCommunicationImplementation::m_SocketPool%3A2273B3029F.role preserve=no  protected: ClCommSocketPoolComm { -> VHAN}
      ClCommSocketPoolComm m_SocketPool;
      //## end ClCommunicationImplementation::m_SocketPool%3A2273B3029F.role

    // Additional Protected Declarations
      //## begin ClCommunicationImplementation%38DB2BE001D1.protected preserve=yes

      //## end ClCommunicationImplementation%38DB2BE001D1.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_bReadyToRun%39E1CB020210
      //## begin ClCommunicationImplementation::m_bReadyToRun%39E1CB020210.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bReadyToRun;
      //## end ClCommunicationImplementation::m_bReadyToRun%39E1CB020210.attr

    // Additional Private Declarations
      //## begin ClCommunicationImplementation%38DB2BE001D1.private preserve=yes
      //## end ClCommunicationImplementation%38DB2BE001D1.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommunicationImplementation%38DB2BE001D1.implementation preserve=yes
      //## end ClCommunicationImplementation%38DB2BE001D1.implementation

};

//## begin ClCommunicationImplementation%38DB2BE001D1.postscript preserve=yes
//## end ClCommunicationImplementation%38DB2BE001D1.postscript

// Class ClNetResource 

// Class ClCommunicationImplementation 


//## Other Operations (inline)
inline m4int_t ClCommunicationImplementation::GetError ()
{
  //## begin ClCommunicationImplementation::GetError%959072027.body preserve=yes
	return m_iFatalError;
  //## end ClCommunicationImplementation::GetError%959072027.body
}

//## begin module%38D20096038E.epilog preserve=yes
//## end module%38D20096038E.epilog


#endif
